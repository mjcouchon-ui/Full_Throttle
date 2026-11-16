"""Theme Park, Stage 1, builder s1d8: THE LAST RIDE: GIANT'S GARDEN (20.5 km to the end).
/Game/World/ThemePark/SM_S1D8_*.   Run: import ft_world_park_s1d8 as K; K.run()

A giant's garden: the houses are garden things 2 to 4 storeys tall. RED caps and doors, GOLD spots, lamps and the
teapot, CREAM stems and walls, TEAL enamel, WOOD pots and timber (the kit's own hex colours, ft_dress_park.py).
  buildings  ToadHouse, PotTower, Boot, Birdhouse, Can, SeedShop, Acorn
  props      tall: TrowelFork, SnailKiosk, Barrow, Lamp, WashLine;  low (under 2 m, unlit): ToadSeats, PegFence, Thimbles
  hero       Teapot (THE GIANT'S TEAPOT: on three flowerpots, pouring a steady stream into a cup that is a pool)
Every mesh is an ft_people.Scene: one material, one section. Night level: every piece that stands 24 m out or more has a
small steady glow (0.08) on its colours, lit windows (1.0) and lamps (1.3); the three low pieces have none. Nothing
blinks; all movement is the builders' motion parameters (swings of 0.1 to 0.45 a second, the stream's ripple 0.9).
All original: no words, no marks, nothing of any real brand or park. No rolled boxes (only pitch and yaw are used).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'
RED, GOLD, CREAM, TEAL, WOOD = '#ff2d3c', '#ffc93c', '#fff1d6', '#2fc6c0', '#9a6a44'
WOOD_D, WHITE, NAVY, TIN, TIN_D = '#6a4c36', '#fff6ea', '#2b2350', '#a8b4d0', '#7f8aa6'
LEAF, LEAF_D, SPRAY, CYAN = '#4f9a6a', '#2f6b52', '#dff6ff', '#2ee6ff'
GB, GW, GL, GWATER = 0.08, 1.0, 1.3, 0.15
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
_B = [0.0]              # the body glow of the piece being drawn


def _g(gl):
    return _B[0] if gl is None else gl


def lit(fn):
    """A piece that stands 24 m out or more: its colours carry the small steady glow."""
    def f(sc):
        _B[0] = GB
        try:
            fn(sc)
        finally:
            _B[0] = 0.0
    return f


# ------------------------------------------------------------------------------------------ drawing words
def bx(m, col, c, s, rot=(0, 0, 0), gl=None):
    m.box(col, c, s, rot=rot, glow=_g(gl))


def cy(m, col, base, r, h, n=8, top=None, gl=None):
    m.cyl(col, base, r, h, steps=n, glow=_g(gl), top_r=(None if top is None else max(top, 4.0)))


def sp(m, col, c, r, sc=(1, 1, 1), st=(5, 8), gl=None):
    m.sphere(col, c, r, scale=sc, steps=st, glow=_g(gl))


def rd(m, col, a, b, r, n=6, gl=None):
    rod(m, col, a, b, r, steps=n, glow=_g(gl))


def tb(m, col, a, b, w, d=None, gl=None):
    tilt_box(m, col, a, b, w, d, glow=_g(gl))


def crod(m, col, a, b, r0, r1, n=8, gl=None):
    """A tapered rod from a (radius r0) to b (radius r1): ft_desert.rod's own turn, with a top radius."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=n, glow=_g(gl), top_r=max(r1, 4.0))


def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def sd(m, col, pts, y0, y1, gl=None):
    m.side(col, _ccw(pts), y0, y1, glow=_g(gl))


def pl(m, col, pts, z0, z1, gl=None):
    m.plan(col, _ccw(pts), z0, z1, glow=_g(gl))


def _nrm(az):
    a = math.radians(az)
    return (math.sin(a), -math.cos(a))


def at(cx, cy_, R, az, z, off=0.0):
    """A point on a round wall of radius R about (cx, cy): az 0 is the front (-Y), az 90 the +X side; off slides it
    along the wall. On a flat front wall R is the wall's distance and cx slides along it."""
    n = _nrm(az)
    a = math.radians(az)
    return (cx + n[0] * R + math.cos(a) * off, cy_ + n[1] * R + math.sin(a) * off, z)


def frus(m, col, x, y, z, r0, r1, h, n=10, gl=None):
    cy(m, col, (x, y, z), r0, h, n=n, top=r1, gl=gl)


def dome(m, col, x, y, z, R, H, n=10, lv=3, gl=None):
    for k in range(lv):
        a0, a1 = math.radians(90.0 * k / lv), math.radians(90.0 * (k + 1) / lv)
        cy(m, col, (x, y, z + H * math.sin(a0)), R * math.cos(a0), H * (math.sin(a1) - math.sin(a0)), n=n, top=R * math.cos(a1), gl=gl)


def on_dome(x, y, z, R, H, az, el):
    """A point on a dome (or ball: el may be negative) and the way out of it there."""
    e = math.radians(el)
    ce, se = math.cos(e), math.sin(e)
    n = _nrm(az)
    v = (ce * n[0] / R, ce * n[1] / R, se / H)
    L = math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])
    return (x + R * ce * n[0], y + R * ce * n[1], z + H * se), (v[0] / L, v[1] / L, v[2] / L)


def stud(m, col, p, nv, r, sink, out, n=6, gl=None):
    """A flat round stud on a surface at p, facing nv: a spot, a porthole, an eyelet."""
    rd(m, col, (p[0] - nv[0] * sink, p[1] - nv[1] * sink, p[2] - nv[2] * sink), (p[0] + nv[0] * out, p[1] + nv[1] * out, p[2] + nv[2] * out), r, n=n, gl=gl)


def spots(m, col, x, y, z, R, H, lst, r, gl=None):
    for az, el, k in lst:
        p, nv = on_dome(x, y, z, R, H, az, el)
        stud(m, col, p, nv, r * k, R * 0.11, R * 0.025 + 5.0, gl=gl)


def rwin(m, cx, cy_, R, az, z, r=60, frame=CREAM, on=True):
    """A round window on a wall: a frame and a lit pane (dark when on is False), a glazing bar across."""
    a, b, c = at(cx, cy_, R - 14, az, z), at(cx, cy_, R + 10, az, z), at(cx, cy_, R + 17, az, z)
    rd(m, frame, a, b, r + 16, n=8)
    rd(m, GOLD if on else NAVY, a, c, r, n=8, gl=(GW if (on and _B[0]) else None))
    bx(m, frame, at(cx, cy_, R + 18, az, z), (2 * r, 8, 10), rot=(0, az, 0))


def door(m, cx, cy_, R, az, z0, w=150, h=250, col=RED, frame=WOOD_D, deep=0.0):
    """A round-topped door, its frame, knob and step. deep: how far the door case runs back into a wall that leans away."""
    bx(m, frame, at(cx, cy_, R + 2 - deep / 2.0, az, z0 + h / 2.0), (w + 36, 24 + deep, h + 18), rot=(0, az, 0))
    bx(m, col, at(cx, cy_, R + 10, az, z0 + h / 2.0), (w, 24, h), rot=(0, az, 0))
    rd(m, frame, at(cx, cy_, R - 12 - deep, az, z0 + h), at(cx, cy_, R + 14, az, z0 + h), w / 2.0 + 18, n=8)
    rd(m, col, at(cx, cy_, R - 12, az, z0 + h), at(cx, cy_, R + 22, az, z0 + h), w / 2.0, n=8)
    bx(m, GOLD, at(cx, cy_, R + 25, az, z0 + h * 0.45, off=w * 0.3), (14, 10, 14), rot=(0, az, 0))
    bx(m, CREAM, at(cx, cy_, R + 55, az, z0 + 8), (w + 70, 100, 16), rot=(0, az, 0))


def awning(m, cx, cy_, R, az, z, w=300, out=140, drop=70, n=5, c1=RED, c2=CREAM):
    """A striped awning: n stripes sloping out and down from the wall."""
    for k in range(n):
        off = (k - (n - 1) / 2.0) * w / n
        a = at(cx, cy_, R - 10, az, z, off)
        b = at(cx, cy_, R + out, az, z - drop, off)
        tb(m, c1 if k % 2 == 0 else c2, a, b, w / float(n), 10)


def lantern(m, p, s=1.0):
    """A small steady lamp: a cream pane under a dark cap."""
    bx(m, WOOD_D, (p[0], p[1], p[2] + 26 * s), (34 * s, 34 * s, 10 * s))
    bx(m, CREAM, p, (24 * s, 24 * s, 42 * s), gl=(GL if _B[0] else None))


def stones(m, x, y0, n, step=95):
    for k in range(n):
        cy(m, CREAM, (x + (35 if k % 2 else -30), y0 - k * step, -4), 42, 12, n=6)


def toadstool(m, x, y, h, r, cap, spot, n=8, stem_r=None, k=(( -40, 30, 1.0), (55, 40, 0.8), (150, 25, 0.9))):
    """A small toadstool: a stem, gills, a cap with spots. Its top is h + 0.62 r."""
    s = stem_r or r * 0.36
    frus(m, CREAM, x, y, -6, s * 1.25, s, h + 6, n=6)
    cy(m, WHITE, (x, y, h - 6), r * 0.94, 8, n=n)
    dome(m, cap, x, y, h, r, r * 0.62, n=n, lv=2)
    spots(m, spot, x, y, h, r, r * 0.62, k, r * 0.2)


# ------------------------------------------------------------------------------------------ buildings
@lit
def toad_house(sc):
    """BUILDING. A toadstool house, 14.4 m to the cap's top, 16.6 m to the chimney's cowl: three floors in the stem
    under a spotted red cap, a dormer in the cap, two young toadstools beside it. Its picture sign swings."""
    g = sc.still()
    frus(g, CREAM, 0, 0, -150, 410, 340, 400)
    frus(g, CREAM, 0, 0, 250, 340, 295, 450)
    frus(g, CREAM, 0, 0, 700, 295, 345, 200)
    frus(g, WHITE, 0, 0, 640, 362, 318, 60)                                  # the ring under the cap
    cy(g, WHITE, (0, 0, 885), 660, 50, n=12)                                 # gills
    frus(g, RED, 0, 0, 915, 715, 730, 30, n=12)
    dome(g, RED, 0, 0, 945, 730, 500, n=12, lv=3)
    spots(g, GOLD, 0, 0, 945, 730, 500, [(0, 22, 1.2), (48, 30, 1.0), (-55, 26, 1.0), (105, 22, 1.1), (-110, 28, 0.9), (160, 30, 1.0),
                                         (-160, 20, 1.1), (25, 58, 0.9), (-70, 60, 0.8), (130, 62, 0.8), (-150, 58, 0.7)], 95)
    # the crooked chimney through the cap
    p, nv = on_dome(0, 0, 945, 730, 500, 140, 48)
    rd(g, TEAL, (p[0] - 20, p[1] - 20, p[2] - 60), (p[0] + 30, p[1] + 20, p[2] + 200), 42)
    rd(g, TEAL, (p[0] + 30, p[1] + 20, p[2] + 190), (p[0] + 10, p[1] + 20, p[2] + 330), 38)
    frus(g, GOLD, p[0] + 10, p[1] + 20, p[2] + 320, 70, 12, 70, n=6)
    # the dormer in the cap
    p, nv = on_dome(0, 0, 945, 730, 500, -8, 34)
    bx(g, CREAM, (p[0], p[1] + 30, p[2] + 40), (190, 200, 190))
    bx(g, GOLD, (p[0], p[1] - 72, p[2] + 46), (110, 8, 110), gl=GW)
    tb(g, RED, (p[0], p[1] - 40, p[2] + 215), (p[0] - 135, p[1] - 40, p[2] + 120), 250, 22)
    tb(g, RED, (p[0], p[1] - 40, p[2] + 215), (p[0] + 135, p[1] - 40, p[2] + 120), 250, 22)
    # door, windows, awning
    door(g, 0, 0, 362, 0, 0, w=170, h=270)
    awning(g, 0, 0, 345, 0, 400, w=330, out=150, drop=70)
    rwin(g, 0, 0, 330, -42, 300, r=58)
    rwin(g, 0, 0, 322, 40, 520, r=62)
    rwin(g, 0, 0, 312, -18, 660, r=52)
    rwin(g, 0, 0, 318, 100, 470, r=55)
    rwin(g, 0, 0, 318, -105, 520, r=55)
    # a flower box under the middle window
    bx(g, WOOD, at(0, 0, 350, 40, 425), (170, 60, 44), rot=(0, 40, 0))
    for k, c in enumerate((RED, GOLD, RED)):
        sp(g, c, at(0, 0, 352, 40, 468, off=(k - 1) * 52), 24, st=(3, 5))
    # a balcony on the +X side
    bx(g, WOOD, at(0, 0, 395, 78, 560), (250, 150, 16), rot=(0, 78, 0))
    tb(g, WOOD_D, at(0, 0, 320, 78, 430), at(0, 0, 450, 78, 552), 16, 16)
    for k in (-1, 0, 1):
        bx(g, WOOD_D, at(0, 0, 462, 78, 610, off=k * 110), (12, 12, 90), rot=(0, 78, 0))
    bx(g, WOOD_D, at(0, 0, 462, 78, 655), (250, 12, 12), rot=(0, 78, 0))
    rd(g, RED, at(0, 0, 300, 78, 568), at(0, 0, 330, 78, 568), 60, n=8)
    bx(g, RED, at(0, 0, 322, 78, 640), (120, 20, 150), rot=(0, 78, 0))
    lantern(g, at(0, 0, 395, -22, 330))
    stones(g, 0, -520, 4)
    # two young toadstools, each a room
    toadstool(g, 610, -140, 400, 230, GOLD, RED, n=10, stem_r=95)
    rwin(g, 610, -140, 92, 0, 250, r=40)
    toadstool(g, -560, 90, 250, 165, RED, CREAM, n=8, stem_r=66)
    toadstool(g, -700, -180, 110, 90, GOLD, RED, n=6)
    # the picture sign: a toadstool on a board, swinging from an arm
    a0, a1 = at(0, 0, 340, -24, 470), at(0, 0, 530, -24, 470)
    tb(g, WOOD_D, a0, a1, 14, 14)
    s = sc.part(P.motion(swing=7, pivot=(a1[0] + 16, a1[1] + 37, 464), axis=-114, rate=0.4, phase=0.2))
    c = (a1[0] + 16, a1[1] + 37, 395)
    bx(s, CREAM, c, (120, 8, 110), rot=(0, -24, 0))
    bx(s, RED, (c[0] - 3, c[1] - 7, c[2] + 18), (76, 8, 34), rot=(0, -24, 0))
    bx(s, WOOD, (c[0] - 3, c[1] - 7, c[2] - 20), (24, 8, 44), rot=(0, -24, 0))


def _pot(m, x, y, z, r0, r1, h, band=CREAM):
    """An upright flowerpot: the body, a painted band, the lip."""
    frus(m, WOOD, x, y, z, r0, r0 + (r1 - r0) * 0.8, h * 0.8)
    frus(m, band, x, y, z + h * 0.52, r0 + (r1 - r0) * 0.52 + 6, r0 + (r1 - r0) * 0.6 + 6, h * 0.08)
    frus(m, WOOD, x, y, z + h * 0.8, r1 + 30, r1 + 44, h * 0.2)


@lit
def pot_tower(sc):
    """BUILDING. Three flowerpots stacked askew (13.6 m), a fourth tipped on top with a giant tulip that nods (19 m),
    a stair round the first pot, a ladder to the second, a washing line to a bean pole."""
    g = sc.still()
    frus(g, WOOD, 0, 0, -20, 590, 640, 46, n=12)                              # the saucer
    _pot(g, 0, 0, -150, 380, 500, 650)                                        # top 500
    _pot(g, 70, -30, 440, 330, 440, 520, band=TEAL)                           # top 960
    _pot(g, -50, 40, 900, 280, 380, 460, band=RED)                            # top 1360
    cy(g, WOOD_D, (-50, 40, 1330), 372, 22, n=10)                             # soil
    for az in (-30, 60, 150, 250):                                            # ivy over the top lip
        tb(g, LEAF, at(-50, 40, 400, az, 1365), at(-50, 40, 440, az, 1180), 90, 14)
    door(g, 0, 0, 442, 0, 0, w=160, h=250, deep=70)
    awning(g, 0, 0, 482, 0, 364, w=320, out=150, drop=60, c1=TEAL)
    rwin(g, 0, 0, 466, -52, 300, r=60)
    rwin(g, 0, 0, 470, 55, 320, r=60)
    rwin(g, 70, -30, 388, -22, 700, r=58)
    rwin(g, 70, -30, 392, 38, 720, r=50)
    rwin(g, 70, -30, 388, 110, 700, r=50)
    rwin(g, -50, 40, 330, 8, 1120, r=54)
    rwin(g, -50, 40, 328, -75, 1110, r=46)
    lantern(g, at(0, 0, 505, -24, 300))
    # the stair round the first pot, its landing and the ladder up
    for k in range(9):
        az = 22 + k * 11
        bx(g, WOOD, at(0, 0, 505 + k * 6, az, 40 + k * 52), (96, 130, 16), rot=(0, az, 0))
    bx(g, WOOD, at(0, 0, 610, 128, 506), (260, 150, 16), rot=(0, 128, 0))
    for k in (-1, 1):
        bx(g, WOOD_D, at(0, 0, 676, 128, 556, off=k * 120), (12, 12, 90), rot=(0, 128, 0))
    bx(g, WOOD_D, at(0, 0, 676, 128, 600), (260, 12, 12), rot=(0, 128, 0))
    l0, l1 = at(0, 0, 610, 140, 514), at(70, -30, 470, 122, 960)
    for k in (-1, 1):
        tb(g, WOOD_D, (l0[0] + k * 34, l0[1], l0[2]), (l1[0] + k * 34, l1[1], l1[2]), 12, 12)
    for k in range(1, 6):
        t = k / 6.0
        bx(g, WOOD_D, (l0[0] + (l1[0] - l0[0]) * t, l0[1] + (l1[1] - l0[1]) * t, l0[2] + (l1[2] - l0[2]) * t), (80, 12, 12))
    # the tipped pot and its tulip (the flower nods)
    crod(g, WOOD, (-60, 30, 1345), (130, 30, 1560), 150, 215, n=8)
    crod(g, WOOD, (118, 30, 1546), (150, 30, 1582), 240, 250, n=8)
    f = sc.part(P.motion(swing=4, pivot=(40, 30, 1460), axis=90, rate=0.3, phase=0.4))
    rd(f, LEAF, (60, 30, 1480), (250, 30, 1760), 26)
    tb(f, LEAF, (130, 30, 1560), (330, 60, 1600), 80, 12)
    tb(f, LEAF, (160, 30, 1620), (80, 0, 1760), 70, 12)
    sp(f, RED, (268, 30, 1800), 105, sc=(1, 1, 1.15), st=(4, 8))
    for k in range(4):
        a = math.radians(45 + k * 90)
        tb(f, RED, (268 + 60 * math.cos(a), 30 + 60 * math.sin(a), 1790), (268 + 105 * math.cos(a), 30 + 105 * math.sin(a), 1930), 80, 18)
    # the washing line to a bean pole
    rd(g, WOOD, (800, 60, -80), (800, 60, 1180), 24)
    bx(g, WOOD, (800, 60, 1150), (110, 14, 14))
    tb(g, CREAM, (370, 40, 1330), (800, 60, 1150), 6, 6)
    for k, (c, w, h) in enumerate(((RED, 60, 110), (CREAM, 130, 150), (TEAL, 90, 120))):
        t = 0.25 + k * 0.25
        px, py, pz = 370 + 430 * t, 40 + 20 * t, 1330 - 180 * t
        q = sc.part(P.motion(swing=7, pivot=(px, py, pz), axis=0, rate=0.35, phase=0.3 * k))
        bx(q, c, (px, py, pz - h / 2.0 - 4), (w, 8, h))
        bx(q, WOOD, (px, py, pz), (14, 16, 30))


@lit
def boot(sc):
    """BUILDING. A lace-up boot (toe to +X), 14.4 m long: a red cone roof on the cuff (13.9 m), eyelets and laces up
    the instep, a patched toe, a pull loop at the heel. The bird on its weather vane turns."""
    g = sc.still()
    bx(g, WOOD_D, (0, 0, -70), (1440, 560, 300))                                # the sole and its footing: top 80
    bx(g, WOOD_D, (-470, 0, 100), (480, 580, 60))                               # the heel's welt
    bx(g, TEAL, (130, 0, 250), (820, 500, 340))                                 # the foot: x -280..540, top 420
    sp(g, TEAL, (530, 0, 265), 250, sc=(1.2, 1.0, 0.92), st=(5, 8))             # the toe
    sp(g, CREAM, (640, 0, 330), 200, sc=(1.0, 1.02, 0.8), st=(4, 8))            # the toe cap
    frus(g, TEAL, -400, 0, 80, 275, 255, 900)                                   # the shaft: top 980
    frus(g, CREAM, -400, 0, 950, 275, 300, 120)                                 # the folded cuff: top 1070
    tb(g, TEAL, (-190, 0, 700), (360, 0, 380), 400, 190)                        # the instep
    tb(g, CREAM, (-200, 0, 790), (300, 0, 480), 200, 60)                        # the tongue
    for k in range(4):                                                          # laces and eyelets
        t = 0.12 + k * 0.25
        x, z = -190 + 550 * t, 700 - 320 * t + 105
        tb(g, WHITE, (x - 30, -150, z + 16), (x + 30, 150, z - 16), 22, 14)
        tb(g, WHITE, (x - 30, 150, z + 16), (x + 30, -150, z - 16), 22, 14)
        for s in (-1, 1):
            bx(g, GOLD, (x, s * 172, z), (34, 34, 26))
    # the roof on the cuff, its chimney and the vane
    frus(g, RED, -400, 0, 1070, 360, 40, 320)
    sp(g, GOLD, (-400, 0, 1400), 34, st=(3, 6))
    rd(g, WOOD_D, (-270, 110, 1150), (-250, 130, 1330), 30)
    frus(g, GOLD, -250, 130, 1320, 52, 10, 50, n=6)
    cy(g, TIN, (-400, 0, 1420), 7, 130, n=6)
    v = sc.part(P.motion(swing=24, pivot=(-400, 0, 1500), axis=P.UP, rate=0.12, phase=0.1))
    bx(v, TIN, (-400, 0, 1500), (170, 8, 10))
    bx(v, RED, (-480, 0, 1500), (50, 8, 50))
    sd(v, GOLD, [(-380, 1510), (-330, 1510), (-300, 1560), (-320, 1590), (-350, 1570), (-390, 1580)], -5, 5)
    # door, windows, a patch, the pull loop
    door(g, 110, 0, 252, 0, 80, w=150, h=240)
    awning(g, 110, 0, 250, 0, 410, w=300, out=130, drop=60)
    bx(g, CREAM, (390, -252, 280), (150, 12, 150))
    bx(g, GOLD, (390, -258, 280), (116, 8, 116), gl=GW)
    for s in (-1, 1):
        bx(g, RED, (390 + s * 96, -256, 280), (40, 10, 150))
    bx(g, WOOD, (390, -270, 196), (170, 40, 16))
    rwin(g, -400, 0, 270, 0, 470, r=56)
    rwin(g, -400, 0, 262, 0, 760, r=50)
    rwin(g, -400, 0, 266, -70, 600, r=50)
    rwin(g, -400, 0, 266, 62, 620, r=46)
    bx(g, CREAM, (-140, -253, 200), (130, 10, 110))
    for k in range(4):
        bx(g, WOOD_D, (-140 + (k - 1.5) * 34, -259, 258), (10, 6, 22))
    tb(g, WOOD_D, (-650, 0, 1040), (-760, 0, 900), 70, 30)
    tb(g, WOOD_D, (-760, 0, 900), (-670, 0, 780), 70, 30)
    lantern(g, (-20, -290, 300))
    tb(g, WOOD_D, (-20, -250, 330), (-20, -292, 330), 10, 10)
    stones(g, 110, -420, 3)
    for x in (-40, 270):                                                        # two tubs of flowers by the door
        frus(g, WOOD, x, -330, 0, 44, 58, 70, n=8)
        sp(g, RED if x < 0 else GOLD, (x, -330, 100), 44, st=(3, 6))


@lit
def birdhouse(sc):
    """BUILDING. A birdhouse hotel on a thick pole, 16.3 m: six lit round doors each with a perch, a porch over the
    red front door, a ladder up the pole, a second small house on a branch. Two little houses swing from their arms
    and the vane turns."""
    g = sc.still()
    frus(g, WOOD, 0, 0, -200, 135, 112, 920, n=8)                               # the pole: top 720
    for sx in (-1, 1):
        for sy in (-1, 1):
            tb(g, WOOD_D, (sx * 80, sy * 70, 500), (sx * 340, sy * 270, 715), 26, 26)
    bx(g, WOOD_D, (0, -40, 735), (780, 660, 40))                                # the platform: top 755
    bx(g, CREAM, (0, 0, 1035), (600, 500, 560))                                 # the house: 755..1315
    sd(g, CREAM, [(-300, 1315), (300, 1315), (0, 1575)], -250, 250)
    tb(g, RED, (0, 0, 1612), (392, 0, 1272), 620, 40)
    tb(g, RED, (0, 0, 1612), (-392, 0, 1272), 620, 40)
    bx(g, WOOD_D, (0, 0, 1618), (34, 650, 34))
    for s in (-1, 1):
        tb(g, GOLD, (0, -306, 1596), (s * 380, -306, 1266), 12, 44)
    for row, z in enumerate((905, 1065, 1225)):
        for s in (-1, 1):
            x = s * (185 if row < 2 else 150)
            rd(g, WOOD_D, (x, -240, z), (x, -262, z), 70, n=8)
            rd(g, GOLD, (x, -240, z), (x, -268, z), 50, n=8, gl=GW)
            rd(g, WOOD, (x, -250, z - 82), (x, -350, z - 82), 10)
    rd(g, WOOD_D, (0, -240, 1420), (0, -262, 1420), 56, n=8)
    rd(g, NAVY, (0, -240, 1420), (0, -267, 1420), 40, n=8)
    # the porch and the front door
    bx(g, RED, (0, -256, 880), (120, 14, 220))
    bx(g, GOLD, (34, -266, 870), (14, 10, 14))
    for s in (-1, 1):
        bx(g, WOOD, (s * 90, -330, 875), (16, 16, 240))
        tb(g, RED, (0, -310, 1050), (s * 120, -310, 990), 170, 16)
    for k in range(-3, 4):
        bx(g, WOOD, (k * 125, -362, 800), (12, 12, 90))
    bx(g, WOOD, (0, -362, 845), (780, 12, 12))
    for s in (-1, 1):
        bx(g, WOOD, (s * 250, -278, 800), (150, 44, 36))
        for k in (-1, 1):
            bx(g, RED if s * k > 0 else GOLD, (s * 250 + k * 40, -278, 836), (40, 40, 40))
    # the ladder
    for s in (-1, 1):
        tb(g, WOOD_D, (s * 50 - 150, -420, 0), (s * 50 - 150, -372, 760), 12, 12)
    for k in range(1, 8):
        bx(g, WOOD_D, (-150, -420 + 48 * k / 8.0, 760 * k / 8.0), (100, 12, 12))
    # a second, smaller house on a branch
    tb(g, WOOD, (60, 60, 380), (430, 200, 520), 44, 44)
    bx(g, TEAL, (450, 210, 640), (220, 200, 230))
    tb(g, RED, (450, 210, 800), (590, 210, 720), 240, 22)
    tb(g, RED, (450, 210, 800), (310, 210, 720), 240, 22)
    rd(g, GOLD, (450, 120, 650), (450, 104, 650), 44, n=8, gl=GW)
    # two little houses that swing from arms at the platform's ends
    for s, c, ph in ((-1, GOLD, 0.0), (1, TEAL, 0.45)):
        x = s * 520
        bx(g, WOOD_D, (s * 450, -200, 760), (180, 16, 16))
        q = sc.part(P.motion(swing=6, pivot=(x, -200, 752), axis=0, rate=0.35, phase=ph))
        bx(q, CREAM, (x, -200, 700), (6, 6, 110))
        bx(q, c, (x, -200, 585), (130, 120, 130))
        tb(q, RED, (x, -200, 690), (x + 84, -200, 640), 140, 14)
        tb(q, RED, (x, -200, 690), (x - 84, -200, 640), 140, 14)
        rd(q, NAVY, (x, -252, 590), (x, -264, 590), 26, n=6)
    # the weather vane
    cy(g, TIN, (0, 0, 1630), 7, 150, n=6)
    v = sc.part(P.motion(swing=22, pivot=(0, 0, 1720), axis=P.UP, rate=0.1, phase=0.6))
    bx(v, TIN, (0, 0, 1720), (190, 8, 10))
    bx(v, RED, (-90, 0, 1720), (50, 8, 50))
    sd(v, GOLD, [(20, 1728), (80, 1728), (104, 1772), (84, 1800), (56, 1780), (10, 1792)], -5, 5)


@lit
def can(sc):
    """BUILDING. The watering-can workshop, 9 m of teal enamel can (12.2 m over its carrying handle): the spout runs
    out to +X over a striped awning and a potting bench, a stove pipe through the shoulder. A row of little pots hung
    under the spout swings."""
    g = sc.still()
    cy(g, TEAL, (0, 0, -150), 440, 960, n=12)                                    # the body: top 810
    frus(g, GOLD, 0, 0, 110, 452, 452, 44, n=12)
    frus(g, GOLD, 0, 0, 700, 452, 452, 44, n=12)
    frus(g, TEAL, 0, 0, 810, 440, 330, 90, n=12)                                 # the shoulder: top 900
    frus(g, CREAM, 0, 0, 890, 340, 340, 26, n=12)
    for a, b in (((-330, 0, 880), (-210, 0, 1180)), ((-210, 0, 1180), (170, 0, 1180)), ((170, 0, 1180), (310, 0, 880))):
        tb(g, TEAL, a, b, 80, 60)                                                # the carrying handle
    for a, b in (((-420, 0, 720), (-720, 0, 640)), ((-720, 0, 640), (-740, 0, 300)), ((-740, 0, 300), (-430, 0, 190))):
        tb(g, TEAL, a, b, 90, 64)                                                # the back handle
    crod(g, TEAL, (390, 0, 210), (760, 0, 620), 118, 96)                         # the spout
    crod(g, TEAL, (750, 0, 608), (1020, 0, 930), 96, 80)
    crod(g, CREAM, (1010, 0, 918), (1110, 0, 1040), 84, 200)                     # the rose
    crod(g, GOLD, (1106, 0, 1035), (1118, 0, 1050), 204, 204)
    # under the spout: a potting bench, an awning, a row of little pots that swings
    awning(g, 700, 0, 70, 0, 530, w=460, out=250, drop=110, n=6)
    bx(g, WOOD, (700, -170, 96), (420, 150, 16))
    for s in (-1, 1):
        bx(g, WOOD_D, (700 + s * 190, -170, 44), (18, 130, 90))
    for k, c in enumerate((LEAF, RED, LEAF)):
        frus(g, WOOD, 590 + k * 110, -170, 104, 22, 32, 44, n=6)
        bx(g, c, (590 + k * 110, -170, 172), (26, 26, 50))
    for k in range(3):
        px = 600 + k * 110
        q = sc.part(P.motion(swing=7, pivot=(px, -250, 425), axis=0, rate=0.35, phase=0.27 * k))
        bx(q, WOOD_D, (px, -250, 395), (6, 6, 60))
        frus(q, WOOD if k != 1 else RED, px, -250, 322, 22, 34, 46, n=6)
    # door, windows, the stove pipe, a coiled hose
    door(g, 0, 0, 440, -8, 0, w=170, h=270)
    rwin(g, 0, 0, 440, 34, 360, r=84)
    rwin(g, 0, 0, 440, -46, 520, r=58)
    rwin(g, 0, 0, 440, 8, 560, r=50)
    rwin(g, 0, 0, 440, -100, 380, r=56)
    rwin(g, 0, 0, 440, 96, 600, r=50)
    lantern(g, at(0, 0, 478, -30, 320))
    rd(g, WOOD_D, (-160, 160, 860), (-160, 160, 1190), 34)
    frus(g, GOLD, -160, 160, 1180, 60, 10, 56, n=6)
    rd(g, RED, at(0, 0, 428, -72, 200), at(0, 0, 462, -72, 200), 86, n=8)
    rd(g, NAVY, at(0, 0, 428, -72, 200), at(0, 0, 468, -72, 200), 40, n=8)
    stones(g, -60, -560, 3)


@lit
def seed_shop(sc):
    """BUILDING. The seed-packet shop: a giant cream packet 12.9 m tall with a red flower painted on it (a picture,
    no words) and the shop's door cut in its foot, a smaller gold packet leaning beside it, the shed behind, giant
    seeds spilled at the door. A tag swings from the packet's corner and a seedling nods."""
    g = sc.still()
    X = -170
    bx(g, CREAM, (X, 0, 490), (840, 150, 1180))                                  # the packet: -100..1080
    bx(g, TEAL, (X, 0, 60), (856, 162, 130))
    bx(g, RED, (X, 0, 1145), (856, 162, 130))
    for k in range(6):                                                          # the crimped top edge
        bx(g, RED, (X - 350 + k * 140, 0, 1212), (99, 162, 99), rot=(45, 0, 0))
    # the picture: a flower
    for k in range(6):
        a = math.radians(k * 60)
        rd(g, RED, (X + 150 * math.cos(a), -72, 790 + 150 * math.sin(a)), (X + 150 * math.cos(a), -90, 790 + 150 * math.sin(a)), 96, n=8)
    rd(g, GOLD, (X, -72, 790), (X, -100, 790), 108, n=8)
    bx(g, LEAF, (X, -82, 500), (40, 10, 330))
    tb(g, LEAF, (X, -82, 470), (X + 170, -82, 590), 12, 80)
    tb(g, LEAF, (X, -82, 420), (X - 170, -82, 540), 12, 80)
    # the door in its foot, two lit windows, an awning
    door(g, X, 0, 78, 0, 0, w=160, h=240)
    awning(g, X, 0, 80, 0, 360, w=340, out=150, drop=60)
    for s in (-1, 1):
        bx(g, WOOD_D, (X + s * 270, -80, 220), (190, 12, 200))
        bx(g, GOLD, (X + s * 270, -86, 220), (154, 8, 164), gl=GW)
        bx(g, WOOD_D, (X + s * 270, -90, 220), (10, 8, 164))
        bx(g, WOOD, (X + s * 270, -100, 112), (210, 50, 16))
    lantern(g, (X - 130, -110, 330))
    # the shed behind
    bx(g, WOOD, (X, 300, 230), (700, 460, 660))
    tb(g, RED, (X, 60, 640), (X, 560, 520), 760, 30)
    rd(g, WOOD_D, (X + 220, 380, 540), (X + 220, 380, 800), 30)
    frus(g, GOLD, X + 220, 380, 790, 52, 10, 50, n=6)
    # the gold packet leaning beside it
    b = sc.still(at=(540, -50, 0), yaw=-14)
    bx(b, GOLD, (0, 0, 350), (540, 120, 900))                                    # -100..800
    bx(b, TEAL, (0, 0, 845), (552, 130, 100))
    for k in range(4):
        bx(b, TEAL, (-195 + k * 130, 0, 900), (92, 130, 92), rot=(45, 0, 0))
    rd(b, RED, (0, -58, 520), (0, -78, 520), 120, n=8)                           # the picture: a radish
    tb(b, RED, (0, -68, 430), (20, -68, 320), 16, 50)
    for dx in (-70, 0, 70):
        tb(b, LEAF, (0, -68, 620), (dx, -68, 760), 14, 54)
    bx(b, CREAM, (0, -64, 150), (380, 10, 120))
    for k, c in enumerate((RED, TEAL, LEAF)):
        bx(b, c, (-120 + k * 120, -72, 150), (60, 8, 60))
    g = sc.still()
    # spilled seeds, a rack of little packets
    r = random.Random(8)
    for k in range(5):
        sp(g, WOOD_D if k % 2 else WHITE, (420 + r.uniform(-140, 240), -260 + r.uniform(-90, 60), 26), 48, sc=(1.6, 1.0, 0.6), st=(3, 6))
    bx(g, WOOD, (-660, -120, 110), (40, 60, 220))
    bx(g, WOOD, (-660, -150, 200), (260, 16, 190))
    for k, c in enumerate((RED, TEAL, GOLD, WHITE)):
        bx(g, c, (-750 + (k % 2) * 120 + 30, -162, 245 - (k // 2) * 90), (80, 10, 70))
    stones(g, X, -260, 3)
    # the tag on its string, and the seedling
    t = sc.part(P.motion(swing=8, pivot=(X + 430, 0, 1090), axis=90, rate=0.4, phase=0.5))
    bx(t, WOOD_D, (X + 436, -10, 1040), (6, 6, 100))
    bx(t, WHITE, (X + 436, -10, 940), (110, 10, 130))
    rd(t, GOLD, (X + 436, -14, 950), (X + 436, -22, 950), 28, n=6)
    s = sc.part(P.motion(swing=5, pivot=(820, -200, 0), axis=90, rate=0.3, phase=0.1))
    rd(s, LEAF, (820, -200, -10), (835, -200, 190), 14)
    tb(s, LEAF, (835, -200, 180), (920, -200, 250), 70, 12)
    tb(s, LEAF, (835, -200, 180), (760, -200, 260), 70, 12)


@lit
def acorn(sc):
    """BUILDING. An acorn house, 11.4 m to the cap, 14 m to the tip of its stalk: a gold nut under a scaly wooden
    cap, a timber porch, a woodpile, a toadstool stool. The leaf on the stalk and the oak-leaf awning sway."""
    g = sc.still()
    sp(g, GOLD, (0, 0, 440), 440, sc=(1, 1, 1.3), st=(6, 10))                    # the nut: -132..1012
    frus(g, WOOD_D, 0, 0, 780, 520, 520, 50)
    dome(g, WOOD, 0, 0, 825, 515, 330, n=10, lv=3)                               # the cap: top 1155
    spots(g, WOOD_D, 0, 0, 825, 515, 330, [(k * 36, 20, 1.0) for k in range(10)] + [(k * 60 + 18, 50, 0.85) for k in range(6)], 62)
    rd(g, WOOD_D, (0, 0, 1140), (60, 0, 1310), 46)
    rd(g, WOOD_D, (56, 0, 1296), (170, 0, 1390), 36)
    lf = sc.part(P.motion(swing=6, pivot=(70, 0, 1310), axis=0, rate=0.35, phase=0.2))
    tb(lf, LEAF, (70, 0, 1310), (-120, 40, 1400), 110, 12)
    tb(lf, LEAF_D, (70, 0, 1312), (-110, 40, 1402), 12, 16)
    # the porch and the door
    bx(g, WOOD_D, (0, -345, 140), (214, 140, 300))
    door(g, 0, 0, 415, 0, 0, w=160, h=250)
    for s in (-1, 1):
        bx(g, WOOD, (s * 130, -520, 170), (22, 22, 360))
        tb(g, WOOD, (0, -470, 420), (s * 170, -470, 340), 230, 18)
    bx(g, WOOD_D, (0, -470, 424), (26, 250, 26))
    rwin(g, 0, 0, 436, -44, 480, r=60)
    rwin(g, 0, 0, 436, 44, 500, r=60)
    rwin(g, 0, 0, 392, 0, 690, r=52)
    rwin(g, 0, 0, 432, 112, 520, r=50)
    rwin(g, 0, 0, 434, -112, 480, r=50)
    lantern(g, (-130, -545, 300))
    stones(g, 0, -640, 3)
    # the woodpile and a stool
    for k, (x, z) in enumerate(((-560, 30), (-490, 30), (-420, 30), (-525, 88), (-455, 88), (-490, 146))):
        rd(g, WOOD if k % 2 else WOOD_D, (x, -260, z), (x, -60, z), 32)
    toadstool(g, -330, -400, 60, 70, RED, CREAM, n=8)
    # the oak-leaf awning on two twigs (it sways)
    for x in (470, 760):
        rd(g, WOOD_D, (x, -120, -20), (x, -120, 430), 18)
    a = sc.part(P.motion(swing=3, pivot=(615, -120, 430), axis=0, rate=0.25, phase=0.7))
    pl(a, LEAF, [(380, -120), (440, -250), (520, -200), (600, -330), (680, -210), (770, -270), (850, -120),
                 (770, 30), (680, -30), (600, 90), (520, -40), (440, 10)], 428, 444)
    bx(a, LEAF_D, (615, -120, 448), (470, 16, 8))


# ------------------------------------------------------------------------------------------ props, tall (24 m out or more)
@lit
def trowel_fork(sc):
    """PROP, TALL. The giant's trowel (9.4 m) and fork (10.7 m, a straw hat on its grip) stuck in the earth, a plant
    label with a picture, seedlings, a line of twine between the handles whose three leaf pennants flutter."""
    g = sc.still()
    for x in (-180, 210):
        sp(g, WOOD_D, (x, 0, 0), 150, sc=(1.7, 0.8, 0.32), st=(4, 8))
    X = -180
    sd(g, TIN, [(X - 135, 340), (X - 155, 120), (X - 90, -80), (X, -160), (X + 90, -80), (X + 155, 120), (X + 135, 340), (X + 50, 430), (X - 50, 430)], -14, 14)
    bx(g, TIN_D, (X, -16, 150), (22, 6, 420))
    rd(g, TIN, (X, 0, 420), (X, 0, 580), 28)
    frus(g, RED, X, 0, 570, 50, 60, 310, n=8)
    frus(g, GOLD, X, 0, 560, 56, 56, 40, n=8)
    sp(g, RED, (X, 0, 890), 62, st=(4, 8))
    X = 210
    for k in (-1.5, -0.5, 0.5, 1.5):
        bx(g, TIN, (X + k * 72, 0, 130), (32, 30, 440))
    bx(g, TIN, (X, 0, 350), (290, 44, 56))
    rd(g, TIN, (X, 0, 370), (X, 0, 500), 26)
    frus(g, WOOD, X, 0, 490, 36, 32, 460, n=8)
    tb(g, RED, (X, 0, 940), (X - 80, 0, 1040), 44, 36)
    tb(g, RED, (X, 0, 940), (X + 80, 0, 1040), 44, 36)
    bx(g, RED, (X, 0, 1046), (220, 44, 36))
    cy(g, GOLD, (X + 30, 0, 1064), 120, 10, n=10)                               # the straw hat
    frus(g, GOLD, X + 30, 0, 1074, 66, 52, 60, n=8)
    frus(g, RED, X + 30, 0, 1074, 68, 68, 16, n=8)
    # the label, the seedlings
    bx(g, WOOD, (20, -70, 110), (16, 10, 260))
    bx(g, CREAM, (20, -78, 250), (150, 10, 120))
    bx(g, RED, (20, -85, 268), (44, 6, 44))
    bx(g, LEAF, (20, -85, 222), (12, 6, 50))
    for k in range(4):
        x = -400 + k * 60 if k < 2 else 380 + (k - 2) * 60
        tb(g, LEAF, (x, -60, 0), (x - 26, -60, 70), 30, 8)
        tb(g, LEAF, (x, -60, 0), (x + 26, -60, 76), 30, 8)
    # twine from handle to handle, three pennants
    tb(g, CREAM, (-180, 0, 880), (20, 0, 800), 6, 6)
    tb(g, CREAM, (20, 0, 800), (210, 0, 900), 6, 6)
    for k, (x, z, c) in enumerate(((-90, 842, LEAF), (20, 798, RED), (120, 852, LEAF))):
        q = sc.part(P.motion(swing=11, pivot=(x, 0, z), axis=0, rate=0.45, phase=0.31 * k))
        sd(q, c, [(x - 34, z), (x + 34, z), (x, z - 90)], -4, 4)


@lit
def snail_kiosk(sc):
    """PROP, TALL. A snail-shell kiosk, 6.7 m: the shell's coils are the front, a lit serving hatch in the innermost
    one under a striped awning, a picture menu board and two toadstool stools. The snail's eye stalks sway and a
    pennant turns on the shell."""
    g = sc.still()
    sp(g, CREAM, (60, 0, 72), 100, sc=(4.4, 1.15, 0.78), st=(5, 10))             # the foot: x -380..500
    tb(g, CREAM, (390, 0, 100), (520, 0, 330), 150, 130)
    sp(g, CREAM, (535, 0, 370), 100, st=(5, 8))
    bx(g, NAVY, (628, 0, 345), (8, 70, 10))
    for s, ph in ((-1, 0.0), (1, 0.4)):
        e = sc.part(P.motion(swing=6, pivot=(540, s * 40, 430), axis=90, rate=0.3, phase=ph))
        rd(e, CREAM, (540, s * 40, 430), (570, s * 62, 600), 15)
        sp(e, WHITE, (572, s * 64, 620), 30, st=(4, 6))
        bx(e, NAVY, (598, s * 64, 622), (10, 16, 16))
    # the shell: a drum and its coils, each a little off the last
    rd(g, WOOD, (-70, 130, 370), (-70, -120, 370), 300, n=12)
    for k, (r, c) in enumerate(((236, CREAM), (172, WOOD), (112, CREAM))):
        rd(g, c, (-70 + 26 * (k + 1), -110 - 18 * k, 370 + 20 * (k + 1)), (-70 + 26 * (k + 1), -138 - 18 * k, 370 + 20 * (k + 1)), r, n=12)
    for k, (r, c) in enumerate(((236, CREAM), (160, WOOD))):
        rd(g, c, (-70 + 26 * (k + 1), 120 + 16 * k, 370 + 20 * (k + 1)), (-70 + 26 * (k + 1), 146 + 16 * k, 370 + 20 * (k + 1)), r, n=12)
    hx, hz = 8, 430
    rd(g, GOLD, (hx, -170, hz), (hx, -182, hz), 74, n=8, gl=GW)                  # the hatch
    bx(g, WOOD, (hx, -200, hz - 76), (230, 60, 14))
    tb(g, WOOD_D, (hx - 80, -176, hz - 150), (hx - 80, -220, hz - 84), 12, 12)
    tb(g, WOOD_D, (hx + 80, -176, hz - 150), (hx + 80, -220, hz - 84), 12, 12)
    awning(g, hx, 0, 178, 0, hz + 110, w=280, out=64, drop=36)
    lantern(g, (hx + 150, -190, hz + 20), s=0.9)
    # the menu board (pictures), two stools
    tb(g, WOOD, (-330, -150, 0), (-330, -120, 210), 14, 14)
    bx(g, CREAM, (-330, -140, 150), (130, 10, 170))
    for k, c in enumerate((RED, GOLD, TEAL)):
        bx(g, c, (-330, -147, 200 - k * 50), (60, 6, 30))
    toadstool(g, -180, -150, 46, 44, RED, CREAM, n=6)
    toadstool(g, 190, -150, 46, 44, GOLD, RED, n=6)
    # the pennant on the shell
    cy(g, WOOD_D, (-70, 0, 660), 6, 110, n=6)
    p = sc.part(P.motion(swing=16, pivot=(-70, 0, 740), axis=P.UP, rate=0.3, phase=0.3))
    sd(p, RED, [(-66, 768), (-66, 716), (24, 742)], -4, 4)


@lit
def barrow(sc):
    """PROP, TALL. A giant wheelbarrow stood on its nose as a stage, 10.3 m to the handles' grips: the red tray is the
    shell, a timber deck over the wheel, gold drapes, a row of steady footlights, steps. The pennants strung between
    the handles flutter. The stage is empty (the cast is another builder's)."""
    g = sc.still()
    rd(g, NAVY, (-34, -84, 82), (34, -84, 82), 84, n=10)                         # the wheel, on the ground under the deck
    rd(g, GOLD, (-40, -84, 82), (40, -84, 82), 44, n=8)
    for s in (-1, 1):
        tb(g, WOOD_D, (s * 52, -84, 82), (s * 200, 60, 200), 20, 20)
    bx(g, WOOD, (0, -15, 184), (580, 260, 24))                                   # the deck: top 196
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, WOOD_D, (sx * 262, -15 + sy * 105, 80), (22, 22, 190))
        for k in range(3):                                                      # the striped apron either side of the wheel
            bx(g, CREAM if k % 2 else RED, (sx * (125 + k * 62), -141, 96), (62, 8, 150))
    bx(g, RED, (0, 128, 418), (520, 30, 440))                                    # the tray: its floor is the back cloth
    for s in (-1, 1):
        bx(g, RED, (s * 262, 40, 418), (30, 200, 440), rot=(0, s * 10, 0))
        bx(g, GOLD, (s * 214, 30, 400), (70, 22, 390))                           # drapes
    bx(g, RED, (0, 30, 652), (560, 240, 30))
    for k in range(5):
        bx(g, TEAL if k % 2 else CREAM, (-200 + k * 100, -84, 612), (100, 10, 54))
    rd(g, GOLD, (0, 110, 440), (0, 100, 440), 92, n=10)                          # a painted sun on the back cloth
    for s in (-1, 1):
        rd(g, WOOD, (s * 205, 130, 640), (s * 232, 150, 950), 23)
        rd(g, RED, (s * 231, 149, 940), (s * 238, 154, 1030), 30)
    for k in range(5):
        bx(g, CREAM, (-220 + k * 110, -128, 208), (26, 22, 20), gl=GL)           # footlights
    for k in range(3):
        bx(g, WOOD, (-350 - k * 46, -15, 150 - k * 56), (50, 200, 20))
        bx(g, WOOD_D, (-350 - k * 46, -15, 70 - k * 28), (16, 180, 140 - k * 56))
    frus(g, WOOD, 150, 30, 196, 36, 50, 60, n=8)                                 # a potted sprout on the stage
    tb(g, LEAF, (150, 30, 250), (120, 30, 340), 40, 10)
    tb(g, LEAF, (150, 30, 250), (186, 30, 330), 40, 10)
    tb(g, CREAM, (-232, 150, 950), (0, 150, 900), 6, 6)
    tb(g, CREAM, (0, 150, 900), (232, 150, 950), 6, 6)
    for k in range(5):
        x = -186 + k * 93
        z = 900 + abs(x) * 50.0 / 232.0
        q = sc.part(P.motion(swing=12, pivot=(x, 150, z), axis=0, rate=0.4, phase=0.23 * k))
        sd(q, (RED, GOLD, TEAL, CREAM, RED)[k], [(x - 36, z), (x + 36, z), (x, z - 96)], 146, 154)


@lit
def lamp(sc):
    """PROP, TALL. A pair of snowdrops as a lamp post, 8.1 m: two drooping cream bells with a steady light in each,
    blade leaves, a tuft at the foot. The stems sway a little, slowly."""
    g = sc.still()
    sp(g, LEAF_D, (0, 0, 0), 90, sc=(1.5, 0.8, 0.45), st=(4, 8))
    for k, (dx, h) in enumerate(((-150, 300), (120, 380), (30, 240))):
        tb(g, LEAF, (dx * 0.2, 0, 0), (dx, (-30 if k % 2 else 30), h), 56, 10)
    w = sc.part(P.motion(swing=2, pivot=(0, 0, 0), axis=90, rate=0.25, phase=0.0))
    for s, top, reach, drop in ((1, 720, 150, 150), (-1, 800, 190, 190)):
        x0 = s * 24
        rd(w, LEAF, (x0, 0, -40), (x0 + s * 10, 0, top - 110), 18)
        rd(w, LEAF, (x0 + s * 10, 0, top - 118), (x0 + s * 70, 0, top), 15)
        rd(w, LEAF, (x0 + s * 64, 0, top), (x0 + s * reach, 0, top - 40), 13)
        bx_ = x0 + s * reach
        bz = top - 40 - drop
        sp(w, LEAF, (bx_, 0, top - 52), 28, st=(3, 6))
        cy(w, CREAM, (bx_, 0, bz), 74, drop - 20, n=8, top=22, gl=GL)
        for a in (0, 120, 240):
            r = math.radians(a + 30)
            bx(w, WHITE, (bx_ + 58 * math.cos(r), 58 * math.sin(r), bz - 14), (30, 30, 44), gl=GL)


@lit
def wash_line(sc):
    """PROP, TALL. The giant's washing line between two pencils stuck in the ground, 7.4 m: a striped sock, patched
    trousers, a tea towel, a gardening glove and a pair of small socks, each pegged and swaying on its own; a prop
    pole, a basket."""
    g = sc.still()
    for s in (-1, 1):
        x = s * 430
        frus(g, GOLD, x, 0, -80, 34, 34, 700, n=6)
        frus(g, RED, x, 0, 60, 36, 36, 50, n=6)
        frus(g, WOOD, x, 0, 620, 34, 12, 90, n=6)
        frus(g, NAVY, x, 0, 700, 13, 4, 36, n=6)
    tb(g, CREAM, (-430, 0, 600), (0, 0, 556), 6, 6)
    tb(g, CREAM, (0, 0, 556), (430, 0, 600), 6, 6)
    rd(g, WOOD, (30, 60, 0), (6, 4, 552), 14)
    tb(g, WOOD, (6, 4, 540), (-16, 4, 590), 12, 12)
    tb(g, WOOD, (6, 4, 540), (28, 4, 590), 12, 12)
    frus(g, WOOD, -200, -70, 0, 70, 96, 80, n=8)
    sp(g, WHITE, (-200, -70, 84), 76, sc=(1, 1, 0.5), st=(3, 6))

    def hang(x, ph, sw=7.0):
        z = 556 + abs(x) * 44.0 / 430.0
        q = sc.part(P.motion(swing=sw, pivot=(x, 0, z), axis=0, rate=0.35, phase=ph))
        return q, z

    q, z = hang(-310, 0.0, 8)                                                   # a striped sock
    bx(q, WOOD, (-310, 0, z), (16, 22, 40))
    bx(q, RED, (-310, 0, z - 100), (76, 18, 170))
    for k in range(2):
        bx(q, CREAM, (-310, 0, z - 60 - k * 60), (80, 20, 24))
    bx(q, RED, (-280, 0, z - 210), (136, 18, 64))
    bx(q, CREAM, (-222, 0, z - 210), (30, 20, 68))
    q, z = hang(-140, 0.3)                                                      # patched trousers
    for s in (-1, 1):
        bx(q, WOOD, (-140 + s * 60, 0, z), (16, 22, 40))
        bx(q, TEAL, (-140 + s * 42, 0, z - 180), (72, 16, 230))
    bx(q, TEAL, (-140, 0, z - 50), (160, 16, 70))
    bx(q, GOLD, (-180, -9, z - 190), (44, 6, 44))
    q, z = hang(50, 0.55, 6)                                                    # a tea towel
    for s in (-1, 1):
        bx(q, WOOD, (50 + s * 80, 0, z), (16, 22, 40))
    bx(q, WHITE, (50, 0, z - 125), (200, 8, 220))
    bx(q, RED, (50, -5, z - 50), (200, 6, 18))
    bx(q, RED, (50, -5, z - 200), (200, 6, 18))
    q, z = hang(235, 0.8, 9)                                                    # a gardening glove
    bx(q, WOOD, (235, 0, z), (16, 22, 40))
    bx(q, RED, (235, 0, z - 40), (84, 20, 40))
    bx(q, GOLD, (235, 0, z - 110), (96, 20, 100))
    for k in range(4):
        bx(q, GOLD, (199 + k * 24, 0, z - 190), (20, 20, 66))
    bx(q, GOLD, (296, 0, z - 120), (26, 20, 56))
    q, z = hang(345, 0.15, 10)                                                  # a pair of small socks
    for s, c in ((-1, CREAM), (1, RED)):
        bx(q, WOOD, (345 + s * 24, 0, z), (14, 22, 36))
        bx(q, c, (345 + s * 24, 0, z - 70), (36, 14, 110))
        bx(q, c, (345 + s * 24 + 14, 0, z - 135), (64, 14, 30))


# ------------------------------------------------------------------------------------------ props, low (under 2 m, unlit)
def toad_seats(sc):
    """PROP, LOW (top 1.9 m). A ring of toadstool seats along the verge, a toadstool table with a tea set, one tall
    thin toadstool, grass tufts and stepping stones. Unlit, nothing moves."""
    g = sc.still()
    toadstool(g, -400, 20, 52, 62, RED, CREAM)
    toadstool(g, -250, -30, 44, 56, GOLD, RED)
    toadstool(g, -110, 30, 58, 60, RED, CREAM)
    toadstool(g, 140, 30, 48, 58, GOLD, RED)
    toadstool(g, 280, -30, 56, 62, RED, CREAM)
    toadstool(g, 20, 0, 92, 84, CREAM, RED, n=10, stem_r=34)                    # the table: top 144
    frus(g, RED, 20, 0, 144, 16, 20, 20, n=6)                                    # a teapot and two cups
    bx(g, RED, (42, 0, 156), (20, 8, 8))
    frus(g, WHITE, -14, -22, 140, 8, 11, 12, n=6)
    frus(g, WHITE, 50, 26, 140, 8, 11, 12, n=6)
    toadstool(g, 410, 20, 152, 60, RED, GOLD, stem_r=17)                         # the tall thin one: top 189
    for k, (x, y) in enumerate(((-330, 50), (-180, -50), (70, 62), (210, -55), (350, 55))):
        tb(g, LEAF, (x, y, 0), (x - 14, y, 44), 22, 6)
        tb(g, LEAF, (x, y, 0), (x + 16, y, 50), 22, 6)
    for x in (-320, -40, 210):
        cy(g, CREAM, (x, -62, -4), 30, 10, n=6)


def peg_fence(sc):
    """PROP, LOW (top 1.9 m). A fence of giant clothes pegs, 10 m: nine painted pegs with gold springs on two lines of
    twine, a cotton reel for a post at one end, a button leaning at the other. Three pegged rags stir. Unlit."""
    g = sc.still()
    cols = (WOOD, RED, CREAM, TEAL)
    for k in range(9):
        x = -400 + k * 100
        c = cols[k % 4]
        for s in (-1, 1):
            tb(g, c, (x + s * 22, 0, -10), (x + s * 13, 0, 150), 40, 22)
        bx(g, c, (x, 0, 165), (58, 40, 44))
        bx(g, GOLD, (x, 0, 108), (64, 46, 14))
    bx(g, CREAM, (0, 0, 60), (860, 6, 6))
    bx(g, CREAM, (0, 0, 132), (860, 6, 6))
    # the cotton reel post and the button
    cy(g, WOOD, (-480, 0, -6), 46, 20, n=8)
    cy(g, RED, (-480, 0, 14), 36, 140, n=8)
    cy(g, WOOD, (-480, 0, 154), 46, 20, n=8)
    cy(g, TIN, (-480, 0, 174), 5, 14, n=6)
    rd(g, TEAL, (470, 10, 74), (470, -14, 82), 76, n=10)
    for dx, dz in ((-22, -22), (22, -22), (-22, 22), (22, 22)):
        bx(g, NAVY, (470 + dx, -16, 80 + dz), (16, 8, 16))
    for k, (x, c) in enumerate(((-250, RED), (-50, WHITE), (150, TEAL))):
        q = sc.part(P.motion(swing=6, pivot=(x, 0, 132), axis=0, rate=0.4, phase=0.3 * k))
        bx(q, c, (x, -6, 100), (64, 6, 56))


def thimbles(sc):
    """PROP, LOW (top 1.9 m). Three tin thimbles as planters of red and gold tulips, a pin cushion stuck with pins,
    a darning needle as a post with its thread run to the cushion. Unlit, nothing moves."""
    g = sc.still()
    r = random.Random(4)
    for x, y, h, rr in ((-270, 10, 116, 72), (-60, -20, 88, 60), (300, 10, 122, 78)):
        frus(g, TIN, x, y, -6, rr * 0.72, rr, h + 6)
        frus(g, TIN_D, x, y, h - 22, rr * 0.97 + 4, rr + 6, 22)
        for k in range(6):
            p = at(x, y, rr * 0.82, k * 60 + 20, h * 0.45)
            bx(g, TIN_D, p, (12, 12, 12))
        cy(g, WOOD_D, (x, y, h - 8), rr * 0.9, 10, n=8)
        for k in range(3):
            a = math.radians(k * 120 + r.uniform(0, 60))
            fx, fy = x + rr * 0.45 * math.cos(a), y + rr * 0.45 * math.sin(a)
            fh = 30 + r.uniform(0, 12)
            bx(g, LEAF, (fx, fy, h + fh / 2.0), (8, 8, fh))
            sp(g, RED if (k + int(x)) % 2 else GOLD, (fx, fy, h + fh + 8), 15, sc=(1, 1, 1.25), st=(3, 5))
    # the pin cushion
    cy(g, WOOD, (130, 0, -4), 62, 22, n=8)
    dome(g, RED, 130, 0, 18, 64, 52, n=8, lv=2)
    for k, (dx, dy, c) in enumerate(((-26, 0, GOLD), (10, -18, WHITE), (28, 16, TEAL), (-6, 24, WHITE))):
        rd(g, TIN, (130 + dx, dy, 50), (130 + dx * 1.7, dy * 1.7, 118), 5)
        sp(g, c, (130 + dx * 1.75, dy * 1.75, 124), 12, st=(3, 5))
    # the needle and its thread
    rd(g, TIN, (410, 0, -10), (410, 0, 170), 7)
    bx(g, TIN, (410, 0, 178), (18, 10, 24))
    tb(g, RED, (410, 0, 170), (280, 0, 96), 5, 5)
    tb(g, RED, (280, 0, 96), (170, 0, 76), 5, 5)


# ------------------------------------------------------------------------------------------ the hero landmark
@lit
def teapot(sc):
    """HERO LANDMARK. THE GIANT'S TEAPOT, a teapot house 43 m to its knob (the steam's top wisp 46 m) and 67 m long:
    a gold pot with 33 lit portholes on a saucer carried by three flowerpots, a red lid, the handle to -X, the spout to
    +X pouring a steady stream into a cup that is a pool. The stream ripples, the splash stirs, the steam drifts."""
    g = sc.still()
    XB, CX = -910.0, 2340.0
    BZ, RB, RZ = 2290.0, 1500.0, 1320.0
    # three flowerpots and the saucer
    pots = [(XB - 710, -410), (XB + 710, -410), (XB, 820)]
    for k, (px, py) in enumerate(pots):
        frus(g, WOOD, px, py, -300, 400, 520, 1050)
        frus(g, (CREAM, TEAL, CREAM)[k], px, py, 330, 478, 490, 80)
        frus(g, WOOD, px, py, 750, 560, 584, 150)
    door(g, pots[0][0], pots[0][1], 476, 0, 0, w=190, h=300, deep=80)
    rwin(g, pots[0][0], pots[0][1], 486, -55, 420, r=70)
    rwin(g, pots[0][0], pots[0][1], 488, 50, 440, r=70)
    rwin(g, pots[1][0], pots[1][1], 482, -30, 380, r=76)
    rwin(g, pots[1][0], pots[1][1], 486, 40, 420, r=64)
    lantern(g, at(pots[0][0], pots[0][1], 520, -26, 380), s=1.6)
    frus(g, CREAM, XB, 0, 900, 1180, 1340, 80, n=12)
    frus(g, RED, XB, 0, 980, 1345, 1345, 30, n=12)
    frus(g, CREAM, XB, 0, 960, 700, 640, 60, n=12)
    # the pot
    sp(g, GOLD, (XB, 0, BZ), RB, sc=(1, 1, RZ / RB), st=(8, 14))
    frus(g, TEAL, XB, 0, BZ - 150, RB + 6, RB + 6, 110, n=14)
    frus(g, RED, XB, 0, BZ - 32, RB + 10, RB + 10, 34, n=14)
    rows = ([(-25, a) for a in range(-140, 141, 28)] + [(8, a) for a in range(-140, 113, 28)] +
            [(31, a) for a in range(-112, 113, 32)] + [(52, a) for a in range(-90, 91, 45)])
    for el, az in rows:
        if el == -25 and az == 0:
            continue                                                            # the door is here
        p, nv = on_dome(XB, 0, BZ, RB, RZ, az + (14 if el == 8 else 0), el)
        if el < 20:
            stud(g, CREAM, p, nv, 96, 150, 16)
        stud(g, GOLD, p, nv, 70, 150, 26, gl=GW)
    # the door in its belly, the balcony, the stair down
    p, nv = on_dome(XB, 0, BZ, RB, RZ, 0, -30)
    bx(g, WOOD_D, (p[0], p[1] - 30, p[2] + 20), (340, 240, 470))
    bx(g, RED, (p[0], p[1] - 152, p[2] + 20), (270, 20, 400))
    bx(g, GOLD, (p[0] + 90, p[1] - 164, p[2]), (26, 14, 26))
    by = p[1] - 270
    bz = p[2] - 200
    bx(g, WOOD, (p[0], by, bz), (700, 300, 26))
    for k in range(-3, 4):
        bx(g, WOOD_D, (p[0] + k * 112, by - 140, bz + 70), (16, 16, 130))
    bx(g, WOOD_D, (p[0], by - 140, bz + 130), (700, 16, 16))
    tb(g, WOOD, (p[0] + 330, by, bz), (XB + 1000, -850, 1012), 150, 26)
    tb(g, WOOD, (XB + 1090, -700, 1004), (XB + 1650, -1010, -20), 150, 26)
    # the lid, its knob, the steam
    frus(g, GOLD, XB, 0, BZ + RZ - 120, 800, 760, 60, n=12)
    dome(g, RED, XB, 0, BZ + RZ - 70, 760, 330, n=12, lv=3)
    frus(g, GOLD, XB, 0, BZ + RZ + 250, 100, 130, 130, n=8)
    sp(g, RED, (XB, 0, BZ + RZ + 520), 170, st=(5, 8))
    for k, (dx, dz, r) in enumerate(((330, 330, 90), (390, 540, 130), (470, 790, 170))):
        s = sc.part(P.motion(bob=-45, rate=0.2, phase=0.3 * k))
        sp(s, WHITE, (XB + dx, 0, BZ + RZ + dz), r, st=(4, 7))
    # the handle and the spout
    hp = [(XB - 1360, 0, 2950), (XB - 1950, 0, 3120), (XB - 2300, 0, 2560), (XB - 2060, 0, 1900), (XB - 1380, 0, 1760)]
    for a, b in zip(hp[:-1], hp[1:]):
        crod(g, TEAL, a, b, 165, 165)
    for q in hp[1:-1]:
        sp(g, TEAL, q, 172, st=(3, 6))
    sp_ = [((XB + 1330, 0, 1880), 340), ((XB + 1930, 0, 2330), 265), ((XB + 2330, 0, 2930), 195), ((XB + 2640, 0, 3020), 150)]
    for (a, r0), (b, r1) in zip(sp_[:-1], sp_[1:]):
        crod(g, GOLD, a, b, r0, r1, n=10)
    tip = sp_[-1][0]
    crod(g, TEAL, (tip[0] - 60, 0, tip[2] - 18), (tip[0] + 10, 0, tip[2] + 3), 158, 158, n=10)
    # the stream: eight lengths down a curve, each rippling a beat after the last
    n = 8
    fall = tip[2] - 40 - 640.0
    reach = CX - 120 - tip[0]
    for k in range(n):
        s0, s1 = k / float(n), (k + 1) / float(n)
        a = (tip[0] + reach * s0, 0, tip[2] - 40 - fall * s0 * s0)
        b = (tip[0] + reach * s1, 0, tip[2] - 40 - fall * s1 * s1)
        w = sc.part(P.motion(bob=-30, rate=0.9, phase=k / float(n)))
        crod(w, SPRAY if k % 2 == 0 else CYAN, a, b, 78 + 6 * k, 84 + 6 * k, n=6, gl=GWATER)
    for k, (dx, dy) in enumerate(((-150, 90), (60, -140), (120, 130))):
        s = sc.part(P.motion(bob=45, rate=0.7, phase=0.33 * k))
        sp(s, SPRAY, (CX - 120 + dx, dy, 640), 110, st=(4, 6), gl=GWATER)
    # the cup, its saucer, the pool
    frus(g, CREAM, CX, 0, -300, 1000, 1120, 360, n=12)
    frus(g, CREAM, CX, 0, 40, 560, 800, 620, n=12)
    frus(g, RED, CX, 0, 560, 800, 812, 70, n=12)
    cy(g, TEAL, (CX, 0, 600), 770, 24, n=12, gl=GWATER)
    ch = [(CX + 760, 0, 520), (CX + 1090, 0, 540), (CX + 1100, 0, 250), (CX + 690, 0, 200)]
    for a, b in zip(ch[:-1], ch[1:]):
        crod(g, CREAM, a, b, 62, 62, n=6)
    # a ladder up the cup, sugar-cube steps
    for s in (-1, 1):
        tb(g, WOOD_D, (CX - 300 + s * 50, -860, 60), (CX - 300 + s * 50, -745, 660), 16, 16)
    for k in range(1, 7):
        bx(g, WOOD_D, (CX - 300, -860 + 115 * k / 7.0, 60 + 600 * k / 7.0), (110, 16, 16))
    for k, (dx, dy, dz) in enumerate(((0, 0, 0), (230, 20, 0), (110, 10, 215))):
        bx(g, WHITE, (CX + 300 + dx, -940 + dy, 168 + dz), (210, 210, 210))


JOBS = [('SM_S1D8_ToadHouse', toad_house), ('SM_S1D8_PotTower', pot_tower), ('SM_S1D8_Boot', boot), ('SM_S1D8_Birdhouse', birdhouse),
        ('SM_S1D8_Can', can), ('SM_S1D8_SeedShop', seed_shop), ('SM_S1D8_Acorn', acorn),
        ('SM_S1D8_TrowelFork', trowel_fork), ('SM_S1D8_SnailKiosk', snail_kiosk), ('SM_S1D8_Barrow', barrow), ('SM_S1D8_Lamp', lamp),
        ('SM_S1D8_WashLine', wash_line),
        ('SM_S1D8_ToadSeats', toad_seats, STILL), ('SM_S1D8_PegFence', peg_fence, dict(grow=(12, 12, 6), grow_down=(12, 12, 0))),
        ('SM_S1D8_Thimbles', thimbles, STILL),
        ('SM_S1D8_Teapot', teapot, dict(far=True, grow=(60, 60, 80), grow_down=(60, 60, 0)))]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
