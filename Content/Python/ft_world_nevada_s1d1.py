"""Nevada, Stage 1, builder s1d1: district 1 "State Line and the Dry Wash" (0 to 2.43 km).
/Game/World/Nevada/SM_S1D1_*.   Run: import ft_world_nevada_s1d1 as K; K.run()

A border railhead and ranch town: one and two storey board-and-batten boxes with stepped false fronts, steep tin
gables, porches on posts, cupolas and weathervanes. Whitewash, red roofs and doors, navy trim and windows, yellow
lamps and sign boards, teal shutters and tank bands. Every mesh is an ft_people.Scene (one section, M_People).
The hero: THE LINE RIDER, a 34 m ranch windpump (the head 42 m up), its 16 m red wheel rocking in the wind.
Sign boards carry bars, never words. Nothing here is a vehicle with an engine, a pickup-sized box, a dice or a lamp column.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))          # no moving part: true bounds
SMALL = dict(grow=(25, 25, 25), grow_down=(25, 25, 0))     # moving parts that stay close (a vane, a pennant, washing)
HERO = dict(grow=(140, 140, 60), grow_down=(140, 140, 0))

W, RED, NAVY, YEL, TEAL = '#fff6ea', '#ff3355', '#2b2350', '#ffe14d', '#2fc6c0'      # the district's five
GREY, WOOD, BARK, HAY, WARM, WATER, MAG = '#c9c4d6', '#9a6a44', '#8a6a4a', '#d8b060', '#ffc86a', '#3fd6d0', '#ff4fb8'
SAGE, SAGE_D = '#8fb86a', '#5f8f4a'
RK, RK_D, RK_L, RK_R, TALUS, SAND, CONC = '#e0784a', '#b8553a', '#f59a62', '#c9603e', '#a04a34', '#f2c58a', '#c9b8a0'
FOOT = 200.0                                               # a building's footing under z = 0


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def bx(g, col, x, y, z0, w, d, h, glow=0.0, rot=(0, 0, 0)):
    """A box standing on z0."""
    g.box(col, (x, y, z0 + h / 2.0), (w, d, h), rot=rot, glow=glow)


def pyr(g, col, x, y, z, half, h, top=3.0):
    """A square pyramid (or a hip roof with a flat top), `half` from the middle to an eave."""
    g.cyl(col, (x, y, z), half * 1.4142, h, rot=(0, 45, 0), steps=4, top_r=top * 1.4142)


def prof_y(g, col, pts, y0, y1, glow=0.0):
    """A profile [(x, z)] seen from the road, from y0 to y1 deep."""
    g.side(col, ccw(pts), y0, y1, glow=glow)


def prof_x(g, col, pts, x0, x1, glow=0.0):
    """A profile [(y, z)] seen from along the road, from x0 to x1."""
    g.side(col, ccw(pts), -x1, -x0, glow=glow, rot=(0, 90, 0))


def roof_y(g, col, x, y0, y1, hw, z0, rise, over=30.0, t=14.0, wall=None):
    """A gable roof, its ridge running away from the road (the gable end faces the road)."""
    if wall:
        prof_y(g, wall, [(x - hw, z0), (x + hw, z0), (x, z0 + rise)], y0, y1)
    for s in (-1, 1):
        a = (x + s * (hw + over), (y0 + y1) / 2.0, z0 - over * rise / hw + t / 2.0)
        tilt_box(g, col, a, (x, (y0 + y1) / 2.0, z0 + rise + t / 2.0), (y1 - y0) + 2 * over, t)


def roof_x(g, col, y, x0, x1, hd, z0, rise, over=30.0, t=14.0, wall=None):
    """A gable roof, its ridge running along the road (the eaves face the road)."""
    if wall:
        prof_x(g, wall, [(y - hd, z0), (y + hd, z0), (y, z0 + rise)], x0, x1)
    for s in (-1, 1):
        a = ((x0 + x1) / 2.0, y + s * (hd + over), z0 - over * rise / hd + t / 2.0)
        tilt_box(g, col, a, ((x0 + x1) / 2.0, y, z0 + rise + t / 2.0), (x1 - x0) + 2 * over, t)


def battens(g, x, yf, w, z0, z1, n, col=GREY):
    """Board and batten: n upright strips on the wall whose face is at yf."""
    for k in range(n):
        g.box(col, (x - w / 2.0 + (k + 0.5) * w / n, yf - 3, (z0 + z1) / 2.0), (10, 8, z1 - z0))


def false_front(g, x, yf, w, z0, z1, steps=2, cap=NAVY, top=RED):
    """A tall board front, stepped at the top. yf: its face. Returns the top."""
    bx(g, W, x, yf + 12, z0, w, 24, z1 - z0)
    bx(g, cap, x, yf + 8, z1, w + 30, 40, 22)
    ww, z = w, z1 + 22
    for k in range(steps):
        ww *= 0.62
        bx(g, W, x, yf + 12, z, ww, 24, 90)
        bx(g, cap if k < steps - 1 else top, x, yf + 8, z + 90, ww + 26, 38, 20)
        z += 110
    return z


def window(g, x, yf, z, w=90.0, h=130.0, lit=False, shut=TEAL):
    g.box(WARM if lit else NAVY, (x, yf - 2, z), (w, 10, h), glow=0.55 if lit else 0.0)
    g.box(W, (x, yf - 5, z), (8, 8, h))
    g.box(NAVY if lit else TEAL, (x, yf - 6, z - h / 2.0 - 7), (w + 30, 14, 12))
    if shut:
        for s in (-1, 1):
            g.box(shut, (x + s * (w / 2.0 + 15), yf - 4, z), (26, 9, h + 6))


def window_x(g, xf, y, z, sgn, w=90.0, h=130.0, lit=False):
    """A window on a side wall whose face is at xf, looking along sgn * X."""
    g.box(WARM if lit else NAVY, (xf + sgn * 2, y, z), (10, w, h), glow=0.55 if lit else 0.0)
    g.box(TEAL, (xf + sgn * 6, y, z - h / 2.0 - 7), (14, w + 30, 12))


def door(g, x, yf, z0=0.0, w=110.0, h=215.0, col=RED):
    bx(g, NAVY, x, yf - 2, z0, w + 24, 10, h + 12)
    bx(g, col, x, yf - 5, z0, w, 10, h)
    g.box(YEL, (x + w * 0.3, yf - 11, z0 + h * 0.48), (9, 5, 9))


def porch(g, x, yf, w, dep, zr, n=4, deck=40.0, roof=RED):
    """A porch on posts in front of the wall at yf: a deck, n posts, a tin roof sloping to the front."""
    bx(g, WOOD, x, yf - dep / 2.0, -60.0, w, dep, deck + 60.0)
    for k in range(n):
        px = x - w / 2.0 + 12 + k * (w - 24) / (n - 1.0)
        bx(g, W, px, yf - dep + 12, deck, 14, 14, zr - deck - 8)
        g.box(NAVY, (px, yf - dep + 12, zr - 20), (34, 20, 12))
    tilt_box(g, roof, (x, yf + 6, zr + 57), (x, yf - dep - 25, zr + 7), w + 40, 12)
    bx(g, NAVY, x, yf - dep - 22, zr - 12, w + 40, 8, 14)


def lamp(g, x, y, z, s=1.0):
    """A lantern, steady (only on pieces that stand 24 m or more out)."""
    g.box(YEL, (x, y, z), (22 * s, 22 * s, 30 * s), glow=0.7)
    g.box(NAVY, (x, y, z + 19 * s), (30 * s, 30 * s, 8 * s))


def board(g, x, yf, z, w, h, col=YEL, ink=NAVY, n=5, seed=1):
    """A sign board: bars of paint, never letters."""
    r = random.Random(seed)
    g.box(ink, (x, yf - 3, z), (w + 16, 8, h + 16))
    g.box(col, (x, yf - 6, z), (w, 8, h))
    cx = x - w * 0.4
    for k in range(n):
        ww = r.uniform(0.08, 0.14) * w
        g.box(ink, (cx + ww / 2.0, yf - 9, z), (ww, 6, h * r.choice((0.36, 0.5, 0.5))))
        cx += ww + w * 0.045


def barrel(g, x, y, z=0.0, r=28.0, h=80.0, col=WOOD, hoop=NAVY):
    g.cyl(col, (x, y, z), r, h, steps=7)
    g.cyl(hoop, (x, y, z + h * 0.22), r + 2, 7, steps=7)
    g.cyl(hoop, (x, y, z + h * 0.7), r + 2, 7, steps=7)


def crate(g, x, y, z=0.0, s=60.0, col=WOOD, band=NAVY):
    bx(g, col, x, y, z, s, s, s * 0.8)
    bx(g, band, x, y, z + s * 0.32, s + 4, s + 4, s * 0.14)


def wheel(g, x, y, z, r=55.0, t=10.0, rim=RED, hub=NAVY, spoke=W, spokes=True):
    """A wagon wheel facing the road."""
    rod(g, rim, (x, y + t / 2.0, z), (x, y - t / 2.0, z), r, steps=10)
    rod(g, hub, (x, y + t / 2.0 + 3, z), (x, y - t / 2.0 - 6, z), r * 0.24, steps=6)
    if spokes:
        for a in (0, 45, 90, 135):
            g.box(spoke, (x, y - t / 2.0 - 2, z), (r * 1.7, 5, 7), rot=(a, 0, 0))


def vane(sc, x, y, z, phase=0.0, s=1.0):
    """A weathervane: a mast, and an arrow with a cockerel that swings in the wind."""
    g = sc.still()
    bx(g, NAVY, x, y, z, 8, 8, 120 * s)
    g.box(NAVY, (x, y, z + 60 * s), (70 * s, 6, 6))
    g.box(NAVY, (x, y, z + 60 * s), (6, 70 * s, 6))
    v = sc.part(P.motion(swing=24, pivot=(x, y, z), axis=P.UP, rate=0.13, phase=phase))
    v.box(YEL, (x, y, z + 122 * s), (170 * s, 7, 10 * s))
    v.box(YEL, (x + 88 * s, y, z + 122 * s), (30 * s, 7, 38 * s), rot=(45, 0, 0))
    v.box(RED, (x - 70 * s, y, z + 136 * s), (50 * s, 7, 42 * s))
    v.box(YEL, (x + 6 * s, y, z + 148 * s), (46 * s, 7, 30 * s))
    v.box(RED, (x + 26 * s, y, z + 172 * s), (20 * s, 7, 22 * s))


def cupola(sc, x, y, z, s=1.0, phase=0.0):
    g = sc.still()
    bx(g, W, x, y, z, 130 * s, 130 * s, 110 * s)
    g.box(NAVY, (x, y, z + 60 * s), (136 * s, 64 * s, 60 * s))
    g.box(NAVY, (x, y, z + 60 * s), (64 * s, 136 * s, 60 * s))
    bx(g, NAVY, x, y, z + 110 * s, 160 * s, 160 * s, 10 * s)
    pyr(g, RED, x, y, z + 120 * s, 86 * s, 110 * s)
    vane(sc, x, y, z + 228 * s, phase, s)


def pennant(sc, x, y, z, col, col2=None, w=150.0, h=44.0, phase=0.0, rate=0.35, swing=14.0):
    """A pennant on a mast top at (x, y, z): it swings about the mast."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + w / 2.0, y, z - h / 2.0), (w, 5, h))
    if col2:
        f.box(col2, (x + w / 2.0, y, z - h * 1.5), (w * 0.8, 5, h))


def mast(g, x, y, z, h, r=6.0, col=GREY, ball=YEL):
    g.cyl(col, (x, y, z), r, h, steps=6)
    g.sphere(ball, (x, y, z + h + r), r * 2.0, steps=(3, 6))


def column(g, seed, h=2000.0, r0=480.0, r1=370.0):
    """A column of the cut's own beds, h tall: the rim pieces stand on it, over the 15 m cut."""
    r = random.Random(seed)
    shares = (0.18, 0.14, 0.16, 0.1, 0.16, 0.1, 0.1, 0.06)
    cols = (RK_D, RK_R, RK, RK_L, RK, RK_R, RK, SAND)
    zz = 0.0
    for k, (sh, col) in enumerate(zip(shares, cols)):
        hh = h * sh
        rr = r0 + (r1 - r0) * (zz / h)
        dn = 300.0 if k == 0 else 0.0
        last = (k == len(shares) - 1)
        g.cyl(col, (0 if last else r.uniform(-22, 22), 0 if last else r.uniform(-22, 22), zz - dn), rr * (1.0 if last else r.uniform(0.95, 1.07)),
              hh + dn, rot=(0, r.uniform(0, 51), 0), steps=7, top_r=rr * (1.0 if last else r.uniform(0.9, 1.0)))
        zz += hh
    for k in range(4):
        a = r.uniform(0, 6.28)
        rr = r.uniform(110, 170)
        g.sphere(TALUS if k % 2 else RK_D, (math.cos(a) * (r0 + 40), math.sin(a) * (r0 + 40), rr * 0.3), rr, scale=(1.2, 1, 0.7), steps=(4, 7))


def flowers(g, x0, x1, y, z, n, seed=1, r0=15.0):
    """A bed of flowers along the foot of a wall."""
    r = random.Random(seed)
    for k in range(n):
        g.sphere((MAG, YEL, W)[k % 3], (x0 + (x1 - x0) * (k + 0.5) / n, y + r.uniform(-6, 6), z + r.uniform(0, 8)), r0, steps=(3, 5))


def ladder(g, x, y, z0, z1, lean=40.0, col=WOOD, w=44.0):
    """A ladder leaning back (towards +y) against a wall."""
    n = max(3, int((z1 - z0) / 60))
    for s in (-1, 1):
        tilt_box(g, col, (x + s * w / 2.0, y - lean, z0), (x + s * w / 2.0, y, z1), 7, 7)
    for k in range(n):
        t = (k + 0.5) / n
        g.box(col, (x, y - lean * (1 - t), z0 + (z1 - z0) * t), (w, 6, 6))


def sage(g, spots):
    """Sage on a rock column's ledges: (angle, radius, height)."""
    for k, (a, rr, zz) in enumerate(spots):
        g.sphere(SAGE if k % 2 else SAGE_D, (math.cos(a) * rr, math.sin(a) * rr, zz), 70, scale=(1, 1, 0.7), steps=(4, 6))


# ------------------------------------------------------------------------------------------ buildings
def border(sc):
    """BUILDING 1, the border inspection station: an office under a hip roof and a cupola, a striped canopy on four
    fat posts over the inspection bay, a raised striped boom, a flag mast with a two-band pennant. 14.6 x 7.4 m."""
    g = sc.still()
    ox = -400.0
    bx(g, W, ox, 60, -FOOT, 520, 440, 330 + FOOT)                 # the office; its front face y = -160
    battens(g, ox, -160, 520, 0, 330, 9)
    bx(g, NAVY, ox, 60, 330, 570, 490, 16)
    pyr(g, RED, ox, 60, 346, 310, 170, top=62)
    cupola(sc, ox, 60, 516, 0.8, 0.1)
    door(g, ox - 140, -160)
    window(g, ox + 100, -160, 190, 150, 120, lit=True)
    window_x(g, ox - 260, 60, 190, -1, 120, 120)
    bx(g, TEAL, ox + 100, -200, 0, 180, 40, 45)                    # a bench under the window
    bx(g, TEAL, ox + 100, -184, 45, 180, 8, 40)
    for px, py in ((60, -200), (560, -200), (60, 240), (560, 240)):          # the fat posts
        g.cyl(W, (px, py, -FOOT), 46, 420 + FOOT, steps=7)
        g.cyl(NAVY, (px, py, 0), 53, 70, steps=7)
        g.cyl(RED, (px, py, 210), 50, 40, steps=7)
    for k in range(8):                                             # the canopy, striped along the road
        bx(g, RED if k % 2 == 0 else W, -160 + (k + 0.5) * 115, 20, 420, 115, 700, 36)
    bx(g, NAVY, 300, -336, 410, 936, 12, 60)
    bx(g, NAVY, 300, 376, 410, 936, 12, 60)
    board(g, 300, -340, 520, 440, 86, seed=3)
    for px in (120, 480):
        bx(g, NAVY, px, -334, 456, 12, 12, 40)
    for px in (-60, 300, 660):
        lamp(g, px, -300, 392)
    bx(g, GREY, 310, 20, 0, 320, 240, 12)                          # the weighing plate and its dial
    bx(g, NAVY, 500, 150, 0, 22, 22, 150)
    rod(g, W, (500, 132, 170), (500, 144, 170), 34, steps=8)
    g.box(RED, (500, 128, 180), (6, 4, 30), rot=(25, 0, 0))
    bx(g, NAVY, -100, -362, 0, 30, 30, 130)                        # the boom, raised (in front of the canopy's edge)
    bx(g, NAVY, -128, -362, 96, 60, 40, 44)
    ang = math.radians(78)
    for k in range(5):
        a = (-100 + math.cos(ang) * k * 104, -362, 118 + math.sin(ang) * k * 104)
        b = (-100 + math.cos(ang) * (k + 1) * 104, -362, 118 + math.sin(ang) * (k + 1) * 104)
        tilt_box(g, RED if k % 2 == 0 else W, a, b, 14, 18)
    mast(g, -700, -250, 0, 820)                                    # the flag mast
    bx(g, NAVY, -700, -250, 0, 44, 44, 40)
    pennant(sc, -700, -250, 812, YEL, TEAL, 170, 44, 0.2)
    for k in range(5):                                             # whitewashed stones along the front
        bx(g, W, -660 + k * 100, -350, 0, 44, 40, 28)
    barrel(g, 640, -300, 0)
    crate(g, 420, 250, 12)
    crate(g, 356, 262, 12, 50)


def depot(sc):
    """BUILDING 2, the railhead depot: a long low station under broad eaves, a bay window, a cupola, a platform with
    mail sacks and a trolley, a signal arm on the gable, a stub of track with a buffer stop. 17.6 x 9.6 m."""
    g = sc.still()
    bx(g, W, 0, 150, -FOOT, 1400, 520, 330 + FOOT)                 # front face y = -110
    battens(g, 0, -110, 1400, 0, 330, 15)
    roof_x(g, RED, 150, -700, 700, 260, 330, 200, over=100, wall=W)
    bx(g, NAVY, 0, -212, 240, 1620, 10, 16)                        # the eave's edge board
    for k in range(6):                                             # brackets under the eaves
        tilt_box(g, NAVY, (-620 + k * 248, -110, 196), (-620 + k * 248, -200, 250), 12, 12)
    cupola(sc, 0, 150, 520, 1.0, 0.4)
    bx(g, W, 0, -150, 0, 240, 80, 300)                             # the agent's bay
    window(g, 0, -190, 185, 150, 130, lit=True, shut=None)
    window_x(g, -120, -150, 185, -1, 50, 130)
    window_x(g, 120, -150, 185, 1, 50, 130)
    bx(g, RED, 0, -150, 300, 270, 110, 14)
    door(g, -330, -110, 60)
    door(g, 480, -110, 60, 190, 230)                               # the freight door
    tilt_box(g, W, (390, -118, 70), (570, -118, 280), 8, 12)
    tilt_box(g, W, (570, -118, 70), (390, -118, 280), 8, 12)
    window(g, -540, -110, 205, 90, 120)
    window(g, 250, -110, 205, 90, 120, lit=True)
    bx(g, WOOD, 0, -260, -80, 1700, 300, 140)                      # the platform
    bx(g, NAVY, 0, -408, 44, 1700, 8, 16)
    for px in (-600, -200, 200, 600):
        lamp(g, px, -180, 236)
    board(g, 0, -216, 332, 420, 70, seed=7)                        # the station's board, standing on the eave
    for px in (-170, 170):
        bx(g, NAVY, px, -212, 258, 12, 12, 44)
    for k, (px, py) in enumerate(((-720, -300), (-670, -330), (-700, -260))):     # mail sacks
        g.sphere(GREY if k % 2 else W, (px, py, 86), 34, scale=(1, 1, 0.8), steps=(4, 6))
    bx(g, TEAL, 700, -310, 78, 150, 70, 10)                        # a luggage trolley
    bx(g, TEAL, 770, -310, 78, 8, 70, 80)
    wheel(g, 660, -350, 70, 16, 6, NAVY, YEL, W, False)
    wheel(g, 740, -350, 70, 16, 6, NAVY, YEL, W, False)
    crate(g, 690, -310, 88, 54)
    crate(g, 730, -306, 88, 40, RED)
    bx(g, TEAL, -150, -180, 60, 200, 40, 45)                       # a bench
    bx(g, TEAL, -150, -164, 105, 200, 8, 40)
    for k in range(9):                                             # the stub of track
        bx(g, BARK, -800 + k * 200, -520, -20, 30, 180, 28)
    for py in (-580, -460):
        bx(g, GREY, 0, py, 8, 1700, 10, 14)
    bx(g, RED, 850, -520, 0, 30, 190, 110)                         # the buffer stop
    bx(g, W, 850, -520, 60, 34, 194, 20)
    tilt_box(g, NAVY, (850, -580, 100), (760, -580, 10), 10, 10)
    tilt_box(g, NAVY, (850, -460, 100), (760, -460, 10), 10, 10)
    bx(g, W, 690, 150, 500, 16, 16, 330)                           # the signal mast on the gable, one arm
    g.sphere(YEL, (690, 150, 842), 14, steps=(3, 6))
    a = sc.part(P.motion(swing=14, pivot=(690, 140, 770), axis=90.0, rate=0.07, phase=0.3))
    a.box(RED, (760, 136, 770), (150, 8, 30))
    a.box(W, (800, 132, 770), (22, 8, 30))
    lamp(g, 690, 132, 710, 0.9)
    g.cyl(GREY, (-420, 260, 430), 16, 170, steps=6)                # the stove pipe
    g.cyl(NAVY, (-420, 260, 600), 24, 12, steps=6)


def store(sc):
    """BUILDING 3, the general store: two storeys behind a tall stepped false front, a porch on five posts with the
    shop windows lit under it, a lean-to, a stove pipe, barrels and crates. 12.4 x 8.8 m, 9.4 m to the top."""
    g = sc.still()
    bx(g, W, 0, 80, -FOOT, 820, 640, 600 + FOOT)                   # body; front y = -240
    roof_y(g, RED, 0, -236, 400, 410, 600, 190, wall=W)
    zt = false_front(g, 0, -264, 880, -FOOT, 700, 2)
    battens(g, 0, -264, 880, 350, 700, 11)
    g.box(YEL, (0, -252, zt + 34), (52, 12, 52), rot=(45, 0, 0))
    board(g, 0, -264, 632, 520, 84, seed=11)
    for k, px in enumerate((-270, 0, 270)):
        window(g, px, -264, 470, 90, 130, lit=(k == 1))
    porch(g, 0, -264, 860, 200, 300, 5)
    door(g, 0, -264, 40)
    for px in (-250, 250):                                         # the shop windows, lit
        g.box(WARM, (px, -267, 185), (210, 10, 150), glow=0.5)
        g.box(NAVY, (px, -270, 185), (10, 8, 150))
        g.box(NAVY, (px, -270, 185), (210, 8, 10))
        bx(g, TEAL, px, -272, 86, 230, 14, 20)
    for px in (-215, 215):
        lamp(g, px, -440, 262)
    barrel(g, -370, -390, 40)
    barrel(g, -312, -420, 40, 24, 66, WOOD, RED)
    crate(g, 350, -400, 40)
    crate(g, 344, -396, 88, 44, RED)
    bx(g, TEAL, 150, -330, 40, 150, 36, 42)
    bx(g, W, -530, 150, -FOOT, 240, 420, 250 + FOOT)               # the lean-to store room
    tilt_box(g, RED, (-400, 150, 336), (-672, 150, 250), 460, 12)
    door(g, -530, -60, 0, 100, 200, TEAL)
    window_x(g, 410, 60, 450, 1)
    window_x(g, 410, 60, 180, 1, lit=True)
    g.cyl(GREY, (240, 260, 690), 15, 190, steps=6)
    g.cyl(NAVY, (240, 260, 880), 24, 12, steps=6)
    g.cyl(W, (480, -150, 0), 44, 110, steps=8)                     # the rain butt
    g.cyl(TEAL, (480, -150, 28), 46, 12, steps=8)
    g.cyl(TEAL, (480, -150, 78), 46, 12, steps=8)
    bx(g, GREY, 440, -200, 110, 10, 10, 190)
    for k, (px, py) in enumerate(((-90, -420), (-140, -436), (-112, -400))):      # flour sacks on the porch
        g.sphere(W if k != 1 else GREY, (px, py, 62), 30, scale=(1, 1, 0.75), steps=(4, 6))
    ladder(g, -600, -70, 0, 300, 50)
    bx(g, NAVY, 300, 280, 600, 8, 8, 150)                          # a lightning rod
    g.sphere(YEL, (300, 280, 760), 12, steps=(3, 6))
    for k in range(6):                                             # bunting along the porch's edge
        g.box((YEL, TEAL, W)[k % 3], (-360 + k * 144, -492, 268), (34, 5, 34), rot=(45, 0, 0))


def barn(sc):
    """BUILDING 4, the ranch barn: a gambrel roof end-on to the road, a braced double door, a hayloft door with a
    hoist beam and a hanging bale that sways, a ridge ventilator and a weathervane, a banded silo. 13.6 x 12.4 m."""
    g = sc.still()
    bx(g, W, -150, -6, -FOOT, 1000, 1212, 400 + FOOT)              # front y = -612
    gam = [(-500, 400), (500, 400), (500, 440), (330, 720), (0, 880), (-330, 720), (-500, 440)]
    prof_y(g, RED, [(x - 150, z) for x, z in [(-540, 384), (540, 384), (540, 452), (350, 740), (0, 906), (-350, 740), (-540, 452)]], -590, 620)
    prof_y(g, W, [(x - 150, z) for x, z in gam], -612, -590)
    battens(g, -150, -612, 1000, 0, 400, 11)
    bx(g, NAVY, -150, -606, 392, 1010, 14, 16)
    for s in (-1, 1):                                              # the double door
        px = -150 + s * 112
        bx(g, RED, px, -616, 0, 216, 10, 330)
        tilt_box(g, W, (px - 96, -623, 14), (px + 96, -623, 316), 8, 16)
        tilt_box(g, W, (px + 96, -623, 14), (px - 96, -623, 316), 8, 16)
    bx(g, NAVY, -150, -614, 330, 470, 10, 16)
    g.box(NAVY, (-150, -615, 560), (190, 10, 170))                 # the hayloft door, open
    g.box(HAY, (-150, -619, 505), (170, 10, 56))
    bx(g, TEAL, -150, -620, 468, 220, 12, 12)
    bx(g, WOOD, -150, -700, 800, 22, 240, 26)                      # the hoist beam
    h = sc.part(P.motion(swing=4, pivot=(-150, -790, 800), axis=90.0, rate=0.22, phase=0.1))
    h.box(BARK, (-150, -790, 660), (6, 6, 280))
    h.box(HAY, (-150, -790, 480), (120, 70, 80))
    h.box(RED, (-150, -790, 480), (20, 74, 84))
    for px in (-470, 170):
        window(g, px, -612, 220, 80, 110)
    cupola(sc, -150, 0, 900, 0.9, 0.7)
    bx(g, W, 420, 250, -FOOT, 150, 600, 280 + FOOT)                # a lean-to on the right
    tilt_box(g, RED, (350, 250, 400), (510, 250, 280), 640, 12)
    g.cyl(W, (560, -330, -FOOT), 180, 760 + FOOT, steps=10)        # the silo
    for z in (130, 330, 530, 720):
        g.cyl(TEAL, (560, -330, z), 184, 16, steps=10)
    g.cyl(RED, (560, -330, 760), 196, 150, steps=10, top_r=14)
    g.sphere(YEL, (560, -330, 918), 14, steps=(3, 6))
    bx(g, NAVY, 560, -516, 0, 8, 8, 760)
    for k in range(9):
        g.box(NAVY, (560, -512, 60 + k * 80), (44, 8, 8))
    for k, (px, py, pz) in enumerate(((-560, -680, 0), (-440, -690, 0), (-500, -684, 56))):       # bales by the door
        bx(g, HAY, px, py, pz, 110, 60, 56)
        bx(g, RED, px, py, pz, 16, 64, 58)
    lamp(g, -150, -630, 372)
    g.box(YEL, (-150, -618, 720), (70, 10, 70), rot=(45, 0, 0))    # the barn's star, a plain diamond
    g.box(RED, (-150, -622, 720), (30, 8, 30), rot=(45, 0, 0))
    for py in (-420, 480):                                         # lightning rods on the ridge
        bx(g, NAVY, -150, py, 900, 6, 6, 110)
        g.sphere(YEL, (-150, py, 1016), 11, steps=(3, 6))
    for py in (-300, 0, 300):
        window_x(g, -650, py, 220, -1, 80, 100)
    bx(g, WOOD, -150, -680, -40, 480, 130, 52)                     # the ramp to the door
    ladder(g, 150, -618, 0, 440, 60)
    wheel(g, 290, -626, 150, 50, 8, TEAL)
    for k in range(5):                                             # a rail pen at the left
        bx(g, W, -690, -700 + k * 150, -30, 14, 14, 150)
    for z in (50, 96):
        bx(g, W, -690, -400, z, 8, 610, 12)
    barrel(g, 300, -660, 0, 30, 84, W, TEAL)


def school(sc):
    """BUILDING 5, the one-room schoolhouse: a steep gable end-on to the road, an open bell turret whose bell swings,
    a double door under a fanlight, a pennant mast, a picket fence, a swing whose seat swings, an outhouse. 11 x 11 m."""
    g = sc.still()
    bx(g, W, 80, 60, -FOOT, 560, 820, 360 + FOOT)                  # front y = -350
    roof_y(g, RED, 80, -372, 470, 280, 360, 250, over=36, wall=W)
    battens(g, 80, -350, 560, 0, 360, 7)
    bx(g, WOOD, 80, -420, -40, 300, 140, 60)                       # the steps
    bx(g, WOOD, 80, -480, -40, 300, 60, 50)
    door(g, 80, -350, 20, 150, 230)
    g.box(NAVY, (80, -357, 135), (8, 8, 230))
    rod(g, YEL, (80, -348, 282), (80, -360, 282), 62, steps=8, glow=0.4)
    for py in (-150, 110, 370):
        window_x(g, 360, py, 215, 1, 80, 170, lit=(py == 110))
        window_x(g, -200, py, 215, -1, 80, 170)
    for s in (-1, 1):
        window(g, 80 + s * 190, -350, 215, 60, 150, shut=None)
    for px, py in ((20, -340), (140, -340), (20, -220), (140, -220)):       # the bell turret
        bx(g, W, px, py, 560, 14, 14, 190)
    bx(g, W, 80, -280, 520, 150, 150, 60)
    bx(g, NAVY, 80, -280, 750, 170, 170, 10)
    pyr(g, RED, 80, -280, 760, 96, 150)
    g.sphere(YEL, (80, -280, 916), 12, steps=(3, 6))
    b = sc.part(P.motion(swing=20, pivot=(80, -280, 740), axis=90.0, rate=0.3, phase=0.2))
    b.cyl(YEL, (80, -280, 640), 44, 80, steps=8, top_r=22)
    b.box(NAVY, (80, -280, 626), (14, 14, 26))
    g.cyl(GREY, (-80, 300, 560), 14, 150, steps=6)
    mast(g, -330, -420, 0, 620)
    pennant(sc, -330, -420, 612, RED, W, 140, 40, 0.5)
    for k in range(9):                                             # the picket fence
        px = -470 + k * 44
        bx(g, W, px, -500, 0, 16, 10, 84 + (10 if k % 2 else 0))
    bx(g, W, -294, -500, 44, 380, 6, 10)
    for k in range(5):
        px = 300 + k * 44
        bx(g, W, px, -500, 0, 16, 10, 84 + (10 if k % 2 else 0))
    bx(g, W, 388, -500, 44, 200, 6, 10)
    for s in (-1, 1):                                              # the swing
        tilt_box(g, TEAL, (-400 + s * 120, -150, 0), (-400 + s * 100, -150, 250), 12, 12)
        tilt_box(g, TEAL, (-400 + s * 120, -30, 0), (-400 + s * 100, -90, 250), 12, 12)
    g.box(TEAL, (-400, -110, 252), (230, 12, 12))
    sw = sc.part(P.motion(swing=22, pivot=(-400, -110, 250), axis=0.0, rate=0.4, phase=0.0))
    for s in (-1, 1):
        sw.box(GREY, (-400 + s * 26, -110, 150), (5, 5, 200))
    sw.box(RED, (-400, -110, 50), (70, 26, 8))
    bx(g, W, -380, 400, -100, 130, 130, 330)                       # the outhouse
    tilt_box(g, RED, (-380, 320, 252), (-380, 480, 222), 150, 10)
    bx(g, TEAL, -380, 332, 0, 70, 6, 190)
    lamp(g, 80, -366, 318)
    flowers(g, -180, -20, -372, 14, 4, 3)
    flowers(g, 180, 340, -372, 14, 4, 5)
    bx(g, NAVY, 250, -356, 60, 100, 8, 80)                         # the notice board: bars, no words
    bx(g, W, 250, -361, 68, 84, 5, 64)
    for k in range(3):
        g.box(NAVY, (250, -365, 84 + k * 18), (60 - k * 14, 4, 8))
    for k in range(4):                                             # the wood pile at the side
        rod(g, BARK, (420, 200 + k * 34, 14), (420, 230 + k * 34, 14), 14, steps=6)
    for k in range(3):
        rod(g, WOOD, (420, 218 + k * 34, 40), (420, 248 + k * 34, 40), 13, steps=6)
    bx(g, NAVY, 80, 60, 612, 14, 800, 12)                          # the ridge board
    bx(g, GREY, 80, -286, 520, 5, 5, 100)                          # the bell rope


def hotel(sc):
    """BUILDING 6, the line hotel: two storeys, a three-step false front, a porch with a railed balcony over it, lit
    windows with teal shutters, a hanging board that swings, bunting, an outside stair. 13.6 x 9.4 m, 11 m to the top."""
    g = sc.still()
    bx(g, W, 0, 100, -FOOT, 1060, 620, 640 + FOOT)                 # front y = -210
    roof_y(g, RED, 0, -206, 410, 530, 640, 150, wall=W)
    zt = false_front(g, 0, -234, 1100, -FOOT, 720, 3)
    battens(g, 0, -234, 1100, 0, 720, 13)
    g.sphere(YEL, (0, -222, zt + 22), 22, steps=(4, 6))
    board(g, 0, -234, 762, 300, 66, seed=17, n=4)
    bx(g, WOOD, 0, -349, -60, 1100, 230, 100)                      # the porch's deck
    for k in range(6):
        px = -538 + k * 215.2
        bx(g, W, px, -452, 40, 14, 14, 300)
        g.box(NAVY, (px, -452, 326), (34, 20, 12))
    bx(g, WOOD, 0, -350, 340, 1120, 240, 14)                       # the balcony's floor, over the porch
    bx(g, NAVY, 0, -468, 326, 1120, 8, 16)
    bx(g, W, 0, -464, 436, 1100, 8, 10)                            # its rail
    for k in range(12):
        bx(g, W, -530 + k * 96.4, -464, 352, 10, 8, 86)
    for k in range(6):                                             # the balcony's posts and roof
        px = -538 + k * 215.2
        bx(g, W, px, -452, 352, 12, 12, 250)
    tilt_box(g, RED, (0, -228, 660), (0, -490, 606), 1140, 12)
    bx(g, NAVY, 0, -486, 592, 1140, 8, 14)
    for k, px in enumerate((-400, -135, 135, 400)):
        window(g, px, -234, 470, 80, 140, lit=(k in (0, 2)))
    door(g, 0, -234, 40, 130, 220)
    for k, px in enumerate((-360, 360)):
        window(g, px, -234, 180, 130, 140, lit=(k == 1))
    door(g, 135, -234, 352, 90, 200, TEAL)
    for k in range(9):                                             # bunting under the balcony
        g.box((RED, YEL, TEAL)[k % 3], (-480 + k * 120, -470, 322), (36, 5, 36), rot=(45, 0, 0))
    bx(g, NAVY, 590, -330, 420, 150, 10, 10)                       # the hanging board
    s = sc.part(P.motion(swing=7, pivot=(600, -330, 420), axis=0.0, rate=0.3, phase=0.4))
    s.box(NAVY, (610, -330, 362), (116, 10, 96))
    s.box(YEL, (610, -336, 362), (100, 6, 80))
    s.box(RED, (610, -340, 372), (60, 5, 16))
    s.box(NAVY, (610, -340, 344), (76, 5, 12))
    tilt_box(g, WOOD, (600, 380, 0), (600, -60, 340), 90, 14)      # the outside stair
    bx(g, WOOD, 600, -130, 328, 120, 150, 14)
    for py, pz in ((-190, 0), (380, 0)):
        bx(g, W, 650, py, pz, 12, 12, 340 if py < 0 else 100)
    tilt_box(g, W, (652, 380, 96), (652, -60, 436), 8, 10)
    for px in (-320, 320):
        lamp(g, px, -440, 262)
    g.cyl(W, (-300, 250, 720), 70, 120, steps=8)                   # the roof tank
    g.cyl(TEAL, (-300, 250, 760), 73, 14, steps=8)
    g.cyl(RED, (-300, 250, 840), 80, 50, steps=8, top_r=8)
    barrel(g, -440, -400, 40)
    bx(g, TEAL, 330, -330, 40, 170, 36, 42)


def livery(sc):
    """BUILDING 7, the livery and smithy: a broad low front with one step, a tall dark doorway with a red sliding
    door half across it, a rock forge chimney with the forge glowing, an anvil, wagon wheels against the wall, a wheel
    sign on the front, a railed pen. 13.8 x 9.2 m."""
    g = sc.still()
    bx(g, W, 0, 80, -FOOT, 1180, 660, 400 + FOOT)                  # front y = -250
    roof_y(g, RED, 0, -246, 410, 590, 400, 170, wall=W)
    zt = false_front(g, 0, -274, 1220, -FOOT, 500, 1)
    battens(g, 0, -274, 1220, 0, 500, 14)
    bx(g, NAVY, -80, -276, 0, 380, 10, 360)                        # the doorway
    bx(g, RED, 40, -284, 0, 250, 10, 350)
    tilt_box(g, W, (-70, -291, 14), (150, -291, 336), 8, 16)
    bx(g, NAVY, -30, -288, 356, 560, 12, 16)
    wheel(g, 0, -278, zt + 70, 62, 10, RED, NAVY, W)               # the wheel sign
    bx(g, NAVY, 0, -266, zt - 4, 12, 12, 20)
    board(g, -380, -274, 430, 300, 70, seed=23, n=4)
    window(g, -420, -274, 210, 110, 130, lit=True)
    wheel(g, 190, -292, 58, 56, 9)                                 # wheels against the wall
    wheel(g, 282, -296, 46, 44, 8, TEAL)
    bx(g, CONC, 470, 150, -FOOT, 150, 150, 690 + FOOT)             # the forge chimney
    bx(g, GREY, 470, 150, 690, 176, 176, 24)
    bx(g, CONC, 470, -160, -FOOT, 190, 200, 130 + FOOT)            # the forge, out under a lean-to
    g.box(WARM, (470, -262, 100), (110, 8, 50), glow=0.9)
    bx(g, GREY, 470, -160, 130, 200, 210, 16)
    for px in (372, 568):
        bx(g, W, px, -420, 0, 14, 14, 300)
    tilt_box(g, RED, (470, -250, 352), (470, -450, 300), 240, 12)
    bx(g, BARK, 250, -420, 0, 50, 50, 56)                          # the anvil on its stump
    bx(g, NAVY, 250, -420, 56, 30, 30, 20)
    bx(g, NAVY, 250, -420, 76, 84, 26, 22)
    for k in range(4):                                             # a railed pen on the left
        bx(g, W, -640 - 0, -420 + k * 190, 0, 14, 14, 130)
    for z in (60, 110):
        bx(g, W, -640, -135, z, 8, 580, 12)
    bx(g, W, -615, -420, 60, 60, 8, 12)
    bx(g, W, -615, -420, 110, 60, 8, 12)
    for k, (px, py, pz) in enumerate(((-520, -330, 0), (-520, -330, 56), (-420, -340, 0))):
        bx(g, HAY, px, py, pz, 110, 60, 56)
        bx(g, RED, px, py, pz, 16, 64, 58)
    bx(g, W, -520, -150, 0, 150, 50, 44)                           # a trough
    bx(g, WATER, -520, -150, 40, 134, 36, 6)
    lamp(g, -200, -300, 330)
    lamp(g, 200, -300, 330)
    for px in (-330, -150):                                        # a saddle rail
        bx(g, WOOD, px, -330, 0, 12, 12, 110)
    rod(g, WOOD, (-350, -330, 104), (-130, -330, 104), 8, steps=6)
    for k, c in enumerate((RED, TEAL)):
        bx(g, c, -290 + k * 90, -330, 100, 56, 46, 20)
        bx(g, YEL, -290 + k * 90, -330, 108, 60, 12, 14)
    ladder(g, 560, -70, 0, 400, 50)
    barrel(g, 330, -350, 0, 30, 84, W, TEAL)
    bx(g, NAVY, 0, 80, 584, 14, 660, 12)                           # the ridge board
    cupola(sc, -200, 120, 500, 0.8, 0.55)
    window_x(g, -590, 100, 220, -1, 100, 120)


def rim_cabin(sc):
    """BUILDING 8, the rim signal cabin (for the rock cuts): a 20 m column of the cut's own rock, on it a white cabin
    with a red roof, a railed porch, lit windows, a stove pipe, a signal mast whose arm nods, a pennant. About 27 m."""
    g = sc.still()
    column(g, 5)
    Z = 2000.0
    bx(g, WOOD, 0, 0, Z, 640, 560, 16)                             # the deck
    bx(g, W, 30, 60, Z + 16, 400, 340, 250)                        # front y = -110
    roof_x(g, RED, 60, -170, 230, 170, Z + 266, 150, over=40, wall=W)
    battens(g, 30, -110, 400, Z + 16, Z + 266, 6)
    door(g, -90, -110, Z + 16, 90, 190)
    window(g, 110, -110, Z + 160, 110, 110, lit=True)
    window_x(g, 230, 60, Z + 160, 1, 110, 110, lit=True)
    window_x(g, -170, 60, Z + 160, -1, 110, 110)
    for k in range(7):                                             # the rail round the deck's front
        bx(g, W, -300 + k * 100, -266, Z + 16, 12, 12, 90)
    bx(g, NAVY, 0, -266, Z + 100, 620, 10, 10)
    for py in (-180, -90, 0):
        bx(g, W, -306, py, Z + 16, 12, 12, 90)
        bx(g, W, 306, py, Z + 16, 12, 12, 90)
    bx(g, NAVY, -306, -128, Z + 100, 10, 280, 10)
    bx(g, NAVY, 306, -128, Z + 100, 10, 280, 10)
    for px in (-250, 250):                                         # braces under the deck
        tilt_box(g, WOOD, (px, -260, Z), (px * 0.8, -150, Z - 180), 16, 16)
    g.cyl(GREY, (150, 140, Z + 340), 12, 170, steps=6)
    bx(g, W, -240, -200, Z + 16, 18, 18, 640)                      # the signal mast
    g.sphere(YEL, (-240, -200, Z + 672), 16, steps=(3, 6))
    lamp(g, -240, -220, Z + 520, 1.3)
    a = sc.part(P.motion(swing=16, pivot=(-240, -212, Z + 590), axis=90.0, rate=0.08, phase=0.2))
    a.box(RED, (-150, -214, Z + 590), (200, 8, 36))
    a.box(W, (-96, -219, Z + 590), (26, 8, 36))
    mast(g, 270, -230, Z + 16, 520, 5)
    pennant(sc, 270, -230, Z + 528, YEL, RED, 170, 46, 0.6)
    barrel(g, 250, -60, Z + 16, 24, 70, W, TEAL)
    flowers(g, 60, 160, -124, Z + 96, 3, 7, 13.0)                  # a window box
    bx(g, TEAL, 110, -122, Z + 76, 130, 22, 16)
    for px in (40, 290):                                           # washing on the deck
        bx(g, W, px, -240, Z + 16, 10, 10, 200)
    bx(g, NAVY, 165, -240, Z + 208, 250, 5, 5)
    for k, c in enumerate((RED, W, TEAL)):
        px = 95 + k * 70
        m = sc.part(P.motion(swing=12, pivot=(px, -240, Z + 208), axis=0.0, rate=0.32 + 0.05 * k, phase=0.3 * k))
        m.box(c, (px, -240, Z + 164), (50, 6, 84))
    sage(g, ((2.2, 420, 600), (4.3, 440, 1100), (5.5, 400, 1500), (0.6, 430, 300)))


def rim_tank(sc):
    """BUILDING 9, the rim tank (for the rock cuts): on a 20 m rock column a banded water tank on stilts under a red
    cone, and a small windpump whose wheel rocks. 30 m."""
    g = sc.still()
    column(g, 9)
    Z = 2000.0
    bx(g, WOOD, 0, 0, Z, 620, 560, 16)
    for px, py in ((-250, -90), (-50, -90), (-250, 110), (-50, 110)):       # the tank's stilts
        bx(g, W, px, py, Z + 16, 18, 18, 300)
    tilt_box(g, NAVY, (-250, -92, Z + 30), (-50, -92, Z + 300), 8, 10)
    tilt_box(g, NAVY, (-50, -92, Z + 30), (-250, -92, Z + 300), 8, 10)
    bx(g, WOOD, -150, 10, Z + 316, 300, 300, 16)
    g.cyl(W, (-150, 10, Z + 332), 160, 300, steps=10)
    for z in (60, 150, 240):
        g.cyl(TEAL, (-150, 10, Z + 332 + z), 164, 16, steps=10)
    g.cyl(RED, (-150, 10, Z + 632), 178, 130, steps=10, top_r=10)
    g.sphere(YEL, (-150, 10, Z + 770), 14, steps=(3, 6))
    tilt_box(g, GREY, (-40, -80, Z + 340), (60, -200, Z + 250), 10, 10)    # the spout
    T = Z + 16
    for sx in (-1, 1):                                             # the windpump's tower
        for sy in (-1, 1):
            tilt_box(g, W, (190 + sx * 100, -40 + sy * 100, T), (190 + sx * 26, -40 + sy * 26, T + 720), 14, 14)
    for z, hw in ((240, 76), (480, 52)):
        for sy in (-1, 1):
            bx(g, NAVY, 190, -40 + sy * hw, T + z, hw * 2, 8, 10)
        for sx in (-1, 1):
            bx(g, NAVY, 190 + sx * hw, -40, T + z, 8, hw * 2, 10)
    bx(g, NAVY, 190, -40, T + 716, 70, 90, 40)
    hub = (190, -110, T + 740)
    w = sc.part(P.motion(swing=40, pivot=hub, axis=90.0, rate=0.09, phase=0.0))
    n = 12
    for k in range(n):
        a = 2 * math.pi * k / n
        tilt_box(w, RED if k % 2 == 0 else W, (hub[0] + math.sin(a) * 40, hub[1], hub[2] + math.cos(a) * 40),
                 (hub[0] + math.sin(a) * 200, hub[1], hub[2] + math.cos(a) * 200), 8, 70)
    rod(w, NAVY, (hub[0], hub[1] + 30, hub[2]), (hub[0], hub[1] - 12, hub[2]), 34, steps=8)
    f = sc.part(P.motion(swing=6, pivot=(190, -40, T + 740), axis=P.UP, rate=0.1, phase=0.3))
    f.box(NAVY, (190, 110, T + 740), (8, 260, 12))
    prof_x(f, NAVY, [(180, T + 740), (300, T + 830), (360, T + 740), (300, T + 650)], 185, 195)
    for k in range(7):                                             # the rail
        bx(g, W, -300 + k * 100, -270, Z + 16, 12, 12, 90)
    bx(g, NAVY, 0, -270, Z + 100, 620, 10, 10)
    lamp(g, 40, -250, Z + 150, 1.2)
    bx(g, NAVY, 40, -250, Z + 16, 10, 10, 120)
    barrel(g, 60, 150, Z + 16)
    bx(g, NAVY, -150, -158, Z + 332, 8, 8, 300)                    # the tank's ladder
    bx(g, NAVY, -110, -158, Z + 332, 8, 8, 300)
    for k in range(5):
        g.box(NAVY, (-130, -158, Z + 370 + k * 56), (40, 8, 8))
    sage(g, ((1.9, 430, 500), (4.1, 440, 1000), (5.6, 400, 1450), (0.4, 430, 250)))


# ------------------------------------------------------------------------------------------ props
def hitch_rail(sc):
    """PROP 1 (LOW, 1.5 m): a hitching rail with a saddle blanket over it, and a row of five painted mail boxes."""
    g = sc.still()
    for px in (-250, -60, 130):
        bx(g, W, px, 0, -30, 16, 16, 140)
        bx(g, NAVY, px, 0, 110, 22, 22, 10)
    rod(g, WOOD, (-280, 0, 96), (160, 0, 96), 8, steps=6)
    bx(g, RED, -150, 0, 92, 70, 44, 20)
    bx(g, YEL, -150, 0, 100, 74, 12, 14)
    bx(g, WOOD, 290, 10, -30, 12, 12, 130)
    bx(g, WOOD, 290, 10, 96, 250, 26, 8)
    for k, c in enumerate((RED, TEAL, YEL, W, NAVY)):
        px = 190 + k * 50
        bx(g, c, px, 4, 104, 34, 46, 30)
        g.box(RED if c != RED else YEL, (px + 19, -8, 130), (5, 5, 26))
    bx(g, WOOD, 200, 10, -30, 12, 12, 130)
    bx(g, WOOD, 380, 10, -30, 12, 12, 130)


def trough(sc):
    """PROP 2 (LOW, 1.8 m): a banded stock trough full of water, a red hand pump at its end, a pail."""
    g = sc.still()
    bx(g, W, 0, 0, -30, 340, 110, 96)
    bx(g, WATER, 0, 0, 62, 320, 90, 6)
    for px in (-120, 0, 120):
        bx(g, TEAL, px, 0, -30, 14, 116, 98)
    bx(g, CONC, 230, 0, -30, 110, 110, 44)
    g.cyl(RED, (230, 0, 14), 16, 120, steps=7)
    g.cyl(NAVY, (230, 0, 134), 20, 18, steps=7)
    tilt_box(g, RED, (230, 0, 142), (300, 0, 176), 8, 8)
    tilt_box(g, RED, (226, 0, 110), (172, 0, 92), 10, 10)
    g.cyl(GREY, (150, -80, 0), 20, 34, steps=7, top_r=24)


def hay_stack(sc):
    """PROP 3 (LOW, 1.9 m): five twine-bound hay bales stacked two deep, a pitchfork stuck in the top, a tarpaulin."""
    g = sc.still()
    for k, (px, py, pz, yw) in enumerate(((-125, 0, 0, 0), (0, 4, 0, 0), (125, -4, 0, 0), (-62, 0, 58, 4), (64, 2, 58, -3))):
        if pz == 0:
            g.box(HAY, (px, py, 14), (118, 64, 88), rot=(0, yw, 0))          # 30 cm of footing
        else:
            g.box(HAY, (px, py, pz + 29), (118, 64, 58), rot=(0, yw, 0))
        g.box(RED, (px - 30, py, pz + 29), (8, 68, 60), rot=(0, yw, 0))
        g.box(RED, (px + 30, py, pz + 29), (8, 68, 60), rot=(0, yw, 0))
    g.box(NAVY, (150, -6, 62), (70, 74, 8), rot=(0, 0, 0))
    tilt_box(g, WOOD, (-70, 10, 110), (-110, 20, 186), 6, 6)
    bx(g, GREY, -72, 10, 108, 26, 8, 12)


def corral(sc):
    """PROP 4 (LOW, 1.4 m): a length of whitewashed rail fence with a red braced gate, and two milk churns."""
    g = sc.still()
    for px in (-350, -180, -10, 190, 350):
        bx(g, W, px, 0, -30, 16, 16, 150)
        bx(g, NAVY, px, 0, 120, 22, 22, 10)
    for z in (50, 96):
        bx(g, W, -180, 0, z, 340, 8, 12)
        bx(g, W, 270, 0, z, 160, 8, 12)
    bx(g, RED, 90, -4, 30, 180, 8, 12)                              # the gate
    bx(g, RED, 90, -4, 100, 180, 8, 12)
    tilt_box(g, RED, (6, -4, 36), (174, -4, 104), 8, 10)
    for px in (4, 176):
        bx(g, RED, px, -4, 26, 10, 8, 90)
    for px, r in ((-300, 20), (-256, 17)):
        g.cyl(GREY, (px, -60, 0), r, 56, steps=7)
        g.cyl(GREY, (px, -60, 56), r * 0.6, 16, steps=7)
        g.cyl(NAVY, (px, -60, 72), r * 0.75, 6, steps=7)


def semaphore(sc):
    """PROP 5 (TALL, 9.6 m): a railway signal mast with two arms (the upper one nods slowly), a lamp, a ladder."""
    g = sc.still()
    bx(g, NAVY, 0, 0, -40, 70, 70, 110)
    bx(g, W, 0, 0, 70, 24, 24, 830)
    bx(g, RED, 0, 0, 300, 28, 28, 40)
    pyr(g, RED, 0, 0, 900, 22, 50)
    g.sphere(YEL, (0, 0, 956), 12, steps=(3, 6))
    a = sc.part(P.motion(swing=18, pivot=(0, -16, 820), axis=90.0, rate=0.08, phase=0.0))
    a.box(RED, (110, -18, 820), (230, 8, 40))
    a.box(W, (170, -23, 820), (30, 8, 40))
    a.box(NAVY, (-30, -18, 820), (50, 8, 60))
    g.box(RED, (-104, -18, 640), (220, 8, 40), rot=(-28, 0, 0))    # the lower arm, at rest
    g.box(YEL, (-160, -23, 669), (30, 8, 40), rot=(-28, 0, 0))
    lamp(g, 0, -26, 730, 1.2)
    lamp(g, 0, -26, 560, 1.2)
    bx(g, NAVY, 26, 14, 0, 6, 6, 700)
    bx(g, NAVY, 26, 54, 0, 6, 6, 700)
    for k in range(9):
        g.box(NAVY, (26, 34, 60 + k * 76), (6, 40, 6))
    bx(g, GREY, -90, 0, 0, 70, 50, 60)                              # the lever frame
    tilt_box(g, RED, (-100, 0, 56), (-120, 0, 140), 8, 8)
    tilt_box(g, W, (-76, 0, 56), (-66, 0, 140), 8, 8)


def wagon(sc):
    """PROP 6 (TALL, 2.9 m): a buckboard wagon, no team: a teal bed on red wheels, a sprung seat, barrels and a crate,
    the shafts down on the ground, a lantern on a pole."""
    g = sc.still()
    bx(g, TEAL, 0, 0, 96, 330, 150, 14)
    for py in (-72, 72):
        bx(g, TEAL, 0, py, 110, 330, 8, 44)
    bx(g, TEAL, -162, 0, 110, 8, 150, 44)
    bx(g, NAVY, 0, -77, 126, 330, 4, 10)
    for px in (-110, 120):
        wheel(g, px, -88, 58, 58, 10)
        wheel(g, px, 88, 58, 58, 10, spokes=False)
        rod(g, NAVY, (px, -84, 58), (px, 84, 58), 6, steps=6)
    bx(g, W, 110, 0, 154, 60, 140, 12)                              # the seat
    bx(g, W, 86, 0, 166, 10, 140, 50)
    tilt_box(g, NAVY, (110, -50, 110), (120, -50, 154), 8, 8)
    tilt_box(g, NAVY, (110, 50, 110), (120, 50, 154), 8, 8)
    barrel(g, -110, -30, 110)
    barrel(g, -46, 34, 110, 26, 74, W, RED)
    barrel(g, -110, 40, 110, 22, 60, WOOD, TEAL)
    crate(g, 20, -20, 110, 56)
    for py in (-40, 40):                                            # the shafts
        tilt_box(g, WOOD, (165, py, 100), (380, py, 8), 8, 10)
    bx(g, WOOD, 372, 0, 4, 10, 100, 10)
    bx(g, NAVY, 150, 66, 110, 8, 8, 160)                            # the lantern pole
    g.box(NAVY, (150, 46, 268), (8, 48, 8))
    lamp(g, 150, 26, 244, 1.0)


def marker(sc):
    """PROP 7 (TALL, 7.4 m): the state line marker, a whitewashed obelisk on a stepped plinth with a yellow cap and
    a red band, between two masts whose pennants swing; a row of painted stones."""
    g = sc.still()
    bx(g, NAVY, 0, 0, -40, 260, 260, 80)
    bx(g, W, 0, 0, 40, 200, 200, 50)
    bx(g, NAVY, 0, 0, 90, 150, 150, 30)
    g.cyl(W, (0, 0, 120), 62 * 1.4142, 440, rot=(0, 45, 0), steps=4, top_r=36 * 1.4142)
    bx(g, RED, 0, 0, 250, 112, 112, 40)
    pyr(g, YEL, 0, 0, 560, 40, 60)
    g.box(YEL, (0, -52, 400), (40, 8, 40), rot=(45, 0, 0))
    bx(g, NAVY, 0, -104, 46, 110, 8, 36)
    bx(g, YEL, -22, -109, 56, 44, 4, 12)
    bx(g, YEL, 26, -109, 56, 30, 4, 12)
    for k, (px, c, c2) in enumerate(((-210, RED, W), (210, TEAL, YEL))):
        bx(g, NAVY, px, 0, -30, 40, 40, 60)
        mast(g, px, 0, 30, 690, 6)
        pennant(sc, px, 0, 712, c, c2, 160, 46, 0.3 * k)
    for k in range(6):
        if k not in (2, 3):
            bx(g, W, -275 + k * 110, -150, 0, 40, 36, 26)


def wash_line(sc):
    """PROP 8 (TALL, 2.7 m): a washing line between two whitewashed posts, five pieces of washing swinging on it,
    a tub and a basket."""
    g = sc.still()
    for px in (-260, 260):
        bx(g, W, px, 0, -30, 14, 14, 290)
        bx(g, W, px, 0, 236, 14, 110, 10)
        tilt_box(g, W, (px, 0, 180), (px, 46, 238), 8, 8)
        tilt_box(g, W, (px, 0, 180), (px, -46, 238), 8, 8)
    bx(g, NAVY, 0, 0, 240, 520, 5, 5)
    r = random.Random(4)
    for k, (c, w, h) in enumerate(((W, 120, 130), (RED, 70, 80), (TEAL, 60, 100), (YEL, 80, 70), (NAVY, 70, 110))):
        px = -196 + k * 98
        m = sc.part(P.motion(swing=11, pivot=(px, 0, 240), axis=0.0, rate=0.3 + 0.05 * (k % 3), phase=r.random()))
        m.box(c, (px, 0, 240 - h / 2.0), (w * 0.82, 6, h))
        if c == W:
            m.box(TEAL, (px, -4, 240 - h + 14), (w * 0.82, 4, 12))
    g.cyl(GREY, (-150, -70, 0), 44, 40, steps=8, top_r=50)
    g.cyl(WATER, (-150, -70, 36), 42, 5, steps=8)
    bx(g, WOOD, 120, -60, 0, 80, 54, 34)
    g.sphere(W, (120, -60, 40), 30, scale=(1.2, 0.8, 0.6), steps=(4, 6))


def yard_pump(sc):
    """PROP 9 (TALL, 8.6 m): a yard windpump, the small brother of the Line Rider: a white tower, a red wheel that
    rocks, a navy tail, a banded tank at its foot."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, W, (sx * 130, sy * 130, -30), (sx * 30, sy * 30, 700), 14, 14)
    for z, hw in ((150, 108), (360, 78), (560, 50)):
        for sy in (-1, 1):
            bx(g, NAVY, 0, sy * hw, z, hw * 2, 8, 10)
        for sx in (-1, 1):
            bx(g, NAVY, sx * hw, 0, z, 8, hw * 2, 10)
    tilt_box(g, W, (-104, -108, 160), (76, -78, 360), 8, 10)
    tilt_box(g, W, (76, -78, 370), (-50, -50, 560), 8, 10)
    bx(g, NAVY, 0, 0, 696, 80, 110, 44)
    hub = (0, -84, 720)
    w = sc.part(P.motion(swing=40, pivot=hub, axis=90.0, rate=0.1, phase=0.0))
    n = 12
    for k in range(n):
        a = 2 * math.pi * k / n
        tilt_box(w, RED if k % 2 == 0 else W, (math.sin(a) * 34, hub[1], hub[2] + math.cos(a) * 34), (math.sin(a) * 140, hub[1], hub[2] + math.cos(a) * 140), 8, 50)
    rod(w, NAVY, (0, hub[1] + 30, hub[2]), (0, hub[1] - 12, hub[2]), 28, steps=8)
    f = sc.part(P.motion(swing=7, pivot=(0, 0, 720), axis=P.UP, rate=0.11, phase=0.4))
    f.box(NAVY, (0, 130, 720), (8, 220, 12))
    prof_x(f, NAVY, [(180, 720), (270, 790), (320, 720), (270, 650)], -5, 5)
    bx(g, GREY, 0, 0, 0, 8, 8, 700)                                 # the pump rod
    g.cyl(W, (230, 60, -30), 110, 120, steps=9)                     # the tank
    g.cyl(TEAL, (230, 60, 10), 113, 14, steps=9)
    g.cyl(TEAL, (230, 60, 60), 113, 14, steps=9)
    g.cyl(WATER, (230, 60, 86), 100, 5, steps=9)
    tilt_box(g, GREY, (20, 0, 60), (130, 30, 100), 10, 10)


def lamp_sign(sc):
    """PROP 10 (TALL, 4.6 m): a navy lamp post with a steady yellow lantern, a hanging board with an arrow that
    swings, a tub of pink flowers at its foot."""
    g = sc.still()
    bx(g, NAVY, 0, 0, -30, 44, 44, 90)
    bx(g, NAVY, 0, 0, 60, 18, 18, 370)
    bx(g, YEL, 0, 0, 200, 24, 24, 16)
    g.box(NAVY, (60, 0, 410), (140, 12, 12))
    tilt_box(g, NAVY, (0, 0, 330), (90, 0, 404), 8, 8)
    lamp(g, 0, 0, 452, 1.8)
    pyr(g, RED, 0, 0, 494, 34, 30)
    s = sc.part(P.motion(swing=8, pivot=(80, 0, 404), axis=0.0, rate=0.28, phase=0.0))
    s.box(NAVY, (80, 0, 330), (130, 10, 110))
    s.box(W, (80, -6, 330), (114, 6, 94))
    s.box(RED, (72, -10, 344), (70, 5, 16))
    s.box(RED, (114, -10, 344), (26, 5, 26), rot=(45, 0, 0))
    s.box(NAVY, (62, -10, 310), (60, 5, 10))
    s.box(NAVY, (108, -10, 310), (20, 5, 10))
    g.cyl(W, (-70, -20, 0), 36, 50, steps=8, top_r=42)
    g.cyl(TEAL, (-70, -20, 30), 43, 10, steps=8)
    r = random.Random(2)
    for k in range(5):
        g.sphere(MAG if k % 2 == 0 else YEL, (-70 + r.uniform(-22, 22), -20 + r.uniform(-22, 22), 62 + r.uniform(0, 14)), 16, steps=(3, 5))


# ------------------------------------------------------------------------------------------ the hero
def line_rider(sc):
    """THE HERO, the Line Rider: a giant ranch windpump. A 34 m whitewashed timber tower on four splayed legs, girts
    and braces, a ladder, a railed gallery and a lit yellow lamp room under the head; a 16 m red and white vane wheel
    that rocks in the wind; a navy tail fin cut as a long arrow that swings a little; a banded tank at the foot."""
    g = sc.still()
    H = 3200.0

    def hw(z):                                                     # half the tower's width at z
        return 560.0 - 440.0 * (z + 200.0) / (H + 200.0)

    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, W, (sx * hw(-200), sy * hw(-200), -200), (sx * hw(H), sy * hw(H), H), 64, 64)
            bx(g, NAVY, sx * hw(0), sy * hw(0), -60, 130, 130, 110)
    levels = (500.0, 1150.0, 1800.0, 2400.0, 2900.0)
    for z in levels:
        w2 = hw(z)
        for sy in (-1, 1):
            bx(g, NAVY, 0, sy * w2, z, w2 * 2 + 50, 30, 40)
        for sx in (-1, 1):
            bx(g, NAVY, sx * w2, 0, z, 30, w2 * 2 + 50, 40)
    zs = (0.0,) + levels
    for k in range(len(zs) - 1):                                   # braces on the front and the two sides
        z0, z1 = zs[k] + 30, zs[k + 1]
        s = 1 if k % 2 == 0 else -1
        tilt_box(g, W, (-s * hw(z0), -hw(z0), z0), (s * hw(z1), -hw(z1), z1), 26, 30)
        tilt_box(g, W, (hw(z0), -s * hw(z0), z0), (hw(z1), s * hw(z1), z1), 26, 30)
        tilt_box(g, W, (-hw(z0), s * hw(z0), z0), (-hw(z1), -s * hw(z1), z1), 26, 30)
        tilt_box(g, W, (s * hw(z0), hw(z0), z0), (-s * hw(z1), hw(z1), z1), 26, 30)       # and the back
    bx(g, GREY, 0, 0, 0, 24, 24, H)                                # the pump rod
    M = 1800.0                                                     # a landing half way up, railed
    wm = hw(M) + 70
    bx(g, WOOD, 0, 0, M + 20, wm * 2, wm * 2, 16)
    for s in (-1, 1):
        bx(g, NAVY, 0, s * wm, M + 130, wm * 2, 12, 12)
        bx(g, NAVY, s * wm, 0, M + 130, 12, wm * 2, 12)
        for t in (-1, 0, 1):
            bx(g, W, t * wm, s * wm, M + 36, 14, 14, 96)
            bx(g, W, s * wm, t * wm * 0.5, M + 36, 14, 14, 96)
    for sx in (-1, 1):                                             # lanterns at the landing's front corners
        lamp(g, sx * wm, -wm, M + 160, 1.6)
    for sx in (-1, 1):                                             # the ladder, up the front
        tilt_box(g, NAVY, (sx * 40 + 150, -hw(0) - 30, 0), (sx * 40 + 40, -hw(2900) - 30, 2900), 12, 12)
    for k in range(14):
        z = 120 + k * 200
        t = z / 2900.0
        g.box(NAVY, (150 - 110 * t, -hw(z) - 30, z), (90, 12, 12))
    G = 2900.0                                                     # the gallery
    bx(g, WOOD, 0, 0, G + 40, 560, 560, 20)
    for k in range(5):
        for s in (-1, 1):
            bx(g, W, -260 + k * 130, s * 270, G + 60, 14, 14, 110)
            bx(g, W, s * 270, -260 + k * 130, G + 60, 14, 14, 110)
    for s in (-1, 1):
        bx(g, NAVY, 0, s * 270, G + 170, 560, 14, 14)
        bx(g, NAVY, s * 270, 0, G + 170, 14, 560, 14)
    bx(g, YEL, 0, 0, G + 60, 250, 250, 230, glow=0.6)              # the lamp room
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, NAVY, sx * 122, sy * 122, G + 60, 26, 26, 240)
    bx(g, NAVY, 0, 0, G + 160, 262, 262, 14)
    pyr(g, RED, 0, 0, G + 290, 190, 150, top=50)
    for sx in (-1, 1):                                             # lanterns on the gallery's front corners
        lamp(g, sx * 270, -270, G + 200, 1.6)
    mast(g, 0, 60, G + 550, 300, 7)                                # a pennant over the head
    pennant(sc, 0, 60, G + 846, YEL, RED, 260, 70, 0.1, 0.3, 14)
    bx(g, NAVY, 0, -90, G + 440, 150, 420, 110)                    # the head
    hub = (0.0, -330.0, 3400.0)                                    # the wheel turns clear in front of the gallery's rail (y = -277)
    w = sc.part(P.motion(swing=38, pivot=hub, axis=90.0, rate=0.06, phase=0.0))
    n = 18
    R0, R1 = 150.0, 800.0
    for k in range(n):
        a = 2 * math.pi * k / n
        sa, ca = math.sin(a), math.cos(a)
        tilt_box(w, RED if k % 3 else W, (sa * R0, hub[1], hub[2] + ca * R0), (sa * R1, hub[1], hub[2] + ca * R1), 14, 190)
        a2 = 2 * math.pi * (k + 1) / n
        for rr in (R1 - 20, 420.0):
            tilt_box(w, NAVY, (sa * rr, hub[1] - 12, hub[2] + ca * rr), (math.sin(a2) * rr, hub[1] - 12, hub[2] + math.cos(a2) * rr), 12, 22)
    rod(w, NAVY, (0, hub[1] + 20, hub[2]), (0, hub[1] - 34, hub[2]), 120, steps=10)
    rod(w, YEL, (0, hub[1] - 30, hub[2]), (0, hub[1] - 50, hub[2]), 56, steps=8)
    f = sc.part(P.motion(swing=5, pivot=(0, 0, 3400), axis=P.UP, rate=0.07, phase=0.3))
    f.box(NAVY, (0, 460, 3400), (30, 720, 44))                     # the tail: a boom and an arrow fin
    prof_x(f, NAVY, [(700, 3400), (700, 3480), (1100, 3480), (1100, 3640), (1420, 3400), (1100, 3160), (1100, 3320), (700, 3320)], -12, 12)
    prof_x(f, YEL, [(760, 3380), (1080, 3380), (1080, 3420), (760, 3420)], -16, 16)
    prof_x(f, RED, [(1130, 3330), (1130, 3470), (1320, 3400)], -16, 16)
    tilt_box(f, NAVY, (0, 120, 3450), (0, 700, 3470), 12, 12)      # the fin's stay
    g.cyl(W, (1000, 0, -200), 330, 440, steps=12)                # the stock tank at the foot
    for z in (40, 150):
        g.cyl(TEAL, (1000, 0, z), 336, 26, steps=12)
    g.cyl(WATER, (1000, 0, 236), 310, 8, steps=12)
    tilt_box(g, GREY, (30, 0, 120), (700, 0, 270), 24, 24)
    bx(g, W, -960, -300, -200, 300, 240, 440)                      # the pump house
    tilt_box(g, RED, (-960, -450, 250), (-960, -150, 330), 340, 14)
    door(g, -960, -420, 0, 90, 190)
    lamp(g, -860, -428, 200, 1.2)
    for k in range(7):                                             # a rail fence round the foot, in front
        bx(g, W, -900 + k * 200, -720, -100, 22, 22, 240)
    for z in (50, 110):
        bx(g, W, -300, -720, z, 1220, 12, 16)


JOBS = [
    ('SM_S1D1_Border', border, SMALL), ('SM_S1D1_Depot', depot, SMALL), ('SM_S1D1_Store', store, STILL),
    ('SM_S1D1_Barn', barn, SMALL), ('SM_S1D1_School', school, SMALL), ('SM_S1D1_Hotel', hotel, SMALL),
    ('SM_S1D1_Livery', livery, SMALL), ('SM_S1D1_RimCabin', rim_cabin, SMALL), ('SM_S1D1_RimTank', rim_tank, SMALL),
    ('SM_S1D1_HitchRail', hitch_rail, STILL), ('SM_S1D1_Trough', trough, STILL), ('SM_S1D1_HayStack', hay_stack, STILL),
    ('SM_S1D1_Corral', corral, STILL), ('SM_S1D1_Semaphore', semaphore, SMALL), ('SM_S1D1_Wagon', wagon, STILL),
    ('SM_S1D1_Marker', marker, SMALL), ('SM_S1D1_WashLine', wash_line, SMALL), ('SM_S1D1_YardPump', yard_pump, SMALL),
    ('SM_S1D1_LampSign', lamp_sign, SMALL), ('SM_S1D1_LineRider', line_rider, HERO),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
