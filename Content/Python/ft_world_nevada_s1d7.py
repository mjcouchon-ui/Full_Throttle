"""Nevada, Stage 1, builder s1d7: district 7 "Red Rock Return" (18.5 to 22 km). /Game/World/Nevada/SM_S1D7_*.
Run: import ft_world_nevada_s1d7 as K; K.run()

A boom-camp silver mine built tall and narrow, to show over the 15 m rock cuts: timber headframes, an ore mill stepping
down in lean-to roofs, tipples on trestle legs, galleried boarding houses, a stone powder house. Teal frames #2fc6c0
over bleached lilac boards #c9c4d6, navy openings #2b2350, cream bins #f4efe6, yellow lamp rooms #ffe14d.
The hero: THE TWIN-WHEEL HEADFRAME, 40 m of teal timber, its two sheave wheels winding to and fro, the cage riding
the shaft. Every mesh is an ft_people.Scene (one section, M_People). Sign boards carry bars, never words.
Nothing here is a vehicle with an engine, a pickup-sized glowing box, a dice, a rocket or a lamp column.
Placed by the block C:/ftwork/s1/L2_s1d7.inl.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/Nevada'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))            # no moving part: true bounds
SMALL = dict(grow=(30, 30, 30), grow_down=(30, 30, 0))       # moving parts that stay close (a vane, washing, a lantern)
LOWMOVE = dict(grow=(12, 12, 8), grow_down=(12, 12, 0))      # a low piece with one small rocking part

TEAL, LILAC, NAVY, CREAM, YEL = '#2fc6c0', '#c9c4d6', '#2b2350', '#f4efe6', '#ffe14d'     # the district's five
ORE, WATER, MAG = '#8f9bb3', '#3fd6d0', '#ff4fb8'                                          # silver ore and iron, flume water, washing
RK, RK_D, RK_L, RK_R, TALUS = '#e0784a', '#b8553a', '#f59a62', '#c9603e', '#a04a34'       # the cut's own rock (the kit's tones)
FOOT = 250.0                                                 # a building's footing under z = 0


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def bx(g, col, x, y, z0, w, d, h, glow=0.0):
    """A box standing on z0: w along the road, d deep, h tall."""
    g.box(col, (x, y, z0 + h / 2.0), (w, d, h), glow=glow)


def beam(g, col, a, b, w=20.0, d=None, glow=0.0):
    """A timber from a to b, w wide (level) and d deep."""
    tilt_box(g, col, a, b, w, d or w, glow)


def pyr(g, col, x, y, z, half, h, top=3.0):
    """A square pyramid roof, `half` from the middle to an eave."""
    g.cyl(col, (x, y, z), half * 1.4142, h, rot=(0, 45, 0), steps=4, top_r=top * 1.4142)


def gable_face(g, col, x, hw, z0, rise, y0, y1):
    """The triangle under a roof whose gable faces the road."""
    g.side(col, ccw([(x - hw, z0), (x + hw, z0), (x, z0 + rise)]), y0, y1)


def roof_gable(g, col, x, yc, w, d, z0, rise, over=30.0, t=14.0, wall=None, trim=CREAM):
    """A roof with its ridge running away from the road: the GABLE faces the road."""
    hw = w / 2.0
    for s in (-1, 1):
        beam(g, col, (x + s * (hw + over), yc, z0 - over * rise / hw + t / 2.0), (x, yc, z0 + rise + t / 2.0), d + 2 * over, t)
    bx(g, trim, x, yc, z0 + rise + t - 2, 26, d + 2 * over + 8, 16)
    if wall:
        gable_face(g, wall, x, hw, z0, rise, yc - d / 2.0, yc + d / 2.0)


def roof_eaves(g, col, xc, y, w, d, z0, rise, over=30.0, t=14.0, wall=None, trim=CREAM):
    """A roof with its ridge along the road: the EAVES face the road. The gable ends are stepped in under the slopes."""
    hd = d / 2.0
    for s in (-1, 1):
        beam(g, col, (xc, y + s * (hd + over), z0 - over * rise / hd + t / 2.0), (xc, y, z0 + rise + t / 2.0), w + 2 * over, t)
    bx(g, trim, xc, y, z0 + rise + t - 2, w + 2 * over + 8, 26, 16)
    if wall:
        bx(g, wall, xc, y, z0, w, d * 0.68, rise * 0.34)
        bx(g, wall, xc, y, z0 + rise * 0.34, w, d * 0.36, rise * 0.32)


def leanto(g, col, x, y0, y1, zf, zb, w, over=40.0, t=14.0, fascia=CREAM):
    """A single slope from the front eave (y0, zf) up to the back (y1, zb), w along the road."""
    k = (zb - zf) / (y1 - y0)
    beam(g, col, (x, y0 - over, zf - over * k + t / 2.0), (x, y1 + 8, zb + 8 * k + t / 2.0), w, t)
    if fascia:
        bx(g, fascia, x, y0 - over - 4, zf - over * k - 14, w + 6, 10, 22)


def win(g, x, yf, z, w=80.0, h=100.0, sill=CREAM, glow=0.0, col=NAVY):
    """A dark opening on a wall whose face is at yf (the road side), its foot at z."""
    bx(g, col, x, yf - 3, z, w, 10, h, glow=glow)
    if sill:
        bx(g, sill, x, yf - 6, z - 10, w + 18, 14, 10)


def win_x(g, xf, y, z, w=80.0, h=100.0, s=1):
    """The same on an end wall whose face is at xf (s: which way it looks along the road)."""
    bx(g, NAVY, xf + s * 3, y, z, 10, w, h)


def xbrace(g, col, a, b, t=14.0):
    """Two crossed braces in the upright rectangle between a (low corner) and b (the opposite high corner)."""
    beam(g, col, (a[0], a[1], a[2]), (b[0], b[1], b[2]), t)
    beam(g, col, (a[0], a[1], b[2]), (b[0], b[1], a[2]), t)


def stair(g, x0, x1, y, z0, z1, w=70.0, col=LILAC, rail=TEAL):
    """A flight on a wall's face, from (x0, z0) up to (x1, z1), and its hand rail."""
    beam(g, col, (x0, y, z0), (x1, y, z1), w, 14)
    beam(g, rail, (x0, y - w / 2.0, z0 + 90), (x1, y - w / 2.0, z1 + 90), 8)


def ladder(g, x, y, z0, z1, col=LILAC, w=50.0, rungs=6):
    for s in (-1, 1):
        bx(g, col, x + s * w / 2.0, y, z0, 8, 8, z1 - z0)
    for k in range(rungs):
        bx(g, col, x, y, z0 + (k + 0.5) * (z1 - z0) / rungs, w, 6, 6)


def lamp_room(g, x, y, z, w, d, h, glow=0.6, finial=True):
    """The district's yellow lamp room: a glazed cabin on a cream sill, navy corner posts, a lilac hip roof."""
    bx(g, CREAM, x, y, z - 12, w + 44, d + 44, 12)
    bx(g, YEL, x, y, z, w, d, h, glow=glow)
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, NAVY, x + sx * w / 2.0, y + sy * d / 2.0, z, 14, 14, h)
    pyr(g, LILAC, x, y, z + h, max(w, d) / 2.0 + 34, h * 0.55)
    if finial:
        bx(g, NAVY, x, y, z + h + h * 0.55, 8, 8, 70)
    return z + h + h * 0.55 + (70 if finial else 0)


def lantern(m, x, y, z, glow=0.6, s=1.0):
    """A hanging lantern, its ring at z (drawn into the model it is given: still or swinging)."""
    bx(m, NAVY, x, y, z - 14 * s, 5, 5, 14 * s)
    bx(m, NAVY, x, y, z - 20 * s, 26 * s, 26 * s, 6 * s)
    bx(m, YEL, x, y, z - 52 * s, 20 * s, 20 * s, 32 * s, glow=glow)
    bx(m, NAVY, x, y, z - 58 * s, 26 * s, 26 * s, 6 * s)


def vane(sc, x, y, z, phase=0.3, col=NAVY, tip=CREAM, L=120.0):
    """A weather vane: an arrow turning a little about its staff."""
    g = sc.still()
    bx(g, NAVY, x, y, z, 8, 8, 90)
    f = sc.part(P.motion(swing=24, pivot=(x, y, z + 80), axis=P.UP, rate=0.22, phase=phase))
    f.box(col, (x, y, z + 84), (L, 6, 8))
    f.box(tip, (x - L * 0.42, y, z + 84), (L * 0.22, 6, 42))
    f.box(tip, (x + L * 0.5, y, z + 84), (22, 8, 22))


def pennant(sc, x, y, z, h=240.0, col=CREAM, tip=TEAL, L=170.0, phase=0.1):
    """A mast and a long pennant swinging about it."""
    g = sc.still()
    bx(g, NAVY, x, y, z, 9, 9, h)
    f = sc.part(P.motion(swing=18, pivot=(x, y, z + h - 30), axis=P.UP, rate=0.4, phase=phase))
    f.box(col, (x + L * 0.3, y, z + h - 34), (L * 0.6, 5, 56))
    f.box(tip, (x + L * 0.8, y, z + h - 34), (L * 0.4, 5, 34))


def wheel(m, x, y, z, R, n=12, t=30.0, w=30.0, spokes=3, rim=CREAM, spoke=CREAM, hub=YEL):
    """A sheave wheel facing the road (its axle runs away from it): a rim of n timbers, whole-width spokes, a hub."""
    for k in range(n):
        a0, a1 = 2 * math.pi * (k - 0.04) / n, 2 * math.pi * (k + 1.04) / n
        tilt_box(m, rim, (x + R * math.cos(a0), y, z + R * math.sin(a0)), (x + R * math.cos(a1), y, z + R * math.sin(a1)), w, t)
    for k in range(spokes):
        a = math.pi * (k + 0.25) / spokes
        tilt_box(m, spoke, (x - R * math.cos(a), y, z - R * math.sin(a)), (x + R * math.cos(a), y, z + R * math.sin(a)), w * 0.6, t * 0.7)
    rod(m, hub, (x, y - w * 0.9, z), (x, y + w * 0.9, z), R * 0.17, steps=8)


def ring(r, rx, ry, n=9, rough=0.14, cx=0.0, cy=0.0):
    out = []
    for k in range(n):
        a = 2 * math.pi * k / n
        f = 1.0 + r.uniform(-rough, rough)
        out.append((cx + math.cos(a) * rx * f, cy + math.sin(a) * ry * f))
    return out


def column(g, seed, r_top=290.0, H=1700.0):
    """A column of the cut's own rock, 17 m, to lift a rim piece over the 15 m cut. Its flat top is at H."""
    r = random.Random(seed)
    beds = ((TALUS, -300.0, 260.0, 1.46, 1.24), (RK_D, 260.0, 800.0, 1.22, 1.12), (RK, 800.0, 1230.0, 1.10, 1.03),
            (RK_R, 1230.0, 1310.0, 1.07, 1.06), (RK_L, 1310.0, H - 60.0, 1.02, 0.98), (RK_D, H - 60.0, H, 1.05, 1.05))
    for (col, z0, z1, f0, f1) in beds:
        g.cyl(col, (r.uniform(-14, 14), r.uniform(-14, 14), z0), r_top * f0, z1 - z0, rot=(0, r.uniform(0, 50), 0), steps=7, top_r=r_top * f1)
    g.sphere(RK, (r_top * 1.3, -r_top * 0.6, 40), 110, scale=(1.2, 1, 0.6), steps=(4, 6))
    g.sphere(RK_D, (-r_top * 1.2, r_top * 0.5, 30), 90, scale=(1.1, 1, 0.6), steps=(4, 6))


def deck_on_rock(g, w, d, H, r_top, col=CREAM):
    """A timber platform on the column's top, a little wider than the rock, on raking struts."""
    bx(g, col, 0, 0, H, w, d, 18)
    for sx in (-1, 1):
        for sy in (-1, 1):
            beam(g, TEAL, (sx * r_top * 0.62, sy * r_top * 0.62, H - 190), (sx * (w / 2.0 - 16), sy * (d / 2.0 - 16), H), 16)


def bars(g, x, yf, z, w, n=3, col=NAVY):
    """A sign board's 'writing': n dark bars (never letters)."""
    for k in range(n):
        bx(g, col, x, yf - 5, z + 12 + k * 20, w * (0.8 if k % 2 else 0.6), 6, 9)


def washing(sc, x0, x1, y, z, cols, phase=0.0, n=None):
    """A line from x0 to x1 at height z with things on it that swing about the line."""
    g = sc.still()
    bx(g, CREAM, (x0 + x1) / 2.0, y, z, abs(x1 - x0), 5, 5)
    n = n or len(cols)
    for k in range(n):
        col = cols[k % len(cols)]
        x = x0 + (k + 0.5) * (x1 - x0) / n
        f = sc.part(P.motion(swing=11, pivot=(x, y, z), axis=0.0, rate=0.45, phase=phase + k * 0.21))
        if k % 3 == 0:          # long johns: a body and two legs
            f.box(col, (x, y, z - 30), (46, 5, 54))
            f.box(col, (x - 13, y, z - 80), (17, 5, 50))
            f.box(col, (x + 13, y, z - 80), (17, 5, 50))
        elif k % 3 == 1:        # a shirt, arms out
            f.box(col, (x, y, z - 30), (40, 5, 56))
            f.box(col, (x, y, z - 14), (80, 5, 18))
        else:                   # a blanket
            f.box(col, (x, y, z - 38), (66, 5, 72))


# ------------------------------------------------------------------------------------------ buildings
def ore_mill(sc):
    """BUILDING. The stepped ore mill: five lean-to roofs climbing away from the road, 7.6 m wide, 13.5 m deep, the
    head house, a cream ore bin and the yellow lamp room at the top (22 m), an ore gallery climbing its right side,
    an iron stack on its left. A vane turns on the lamp room."""
    g = sc.still()
    W, D, y00 = 760.0, 270.0, -680.0
    hw = W / 2.0
    for k in range(5):
        y0, y1 = y00 + k * D, y00 + (k + 1) * D
        zf = 330.0 + k * 330.0
        zb = zf + 140.0
        zlo = -FOOT if k == 0 else zf - 200.0
        bx(g, TEAL, 0, (y0 + y1) / 2.0, -FOOT, W, D, zf + FOOT)                       # the bay's walls up to its front eave
        beam(g, TEAL, (0, y0, zf - 48), (0, y1, zb - 48), W, 96)                     # ... and the wedge under the slope
        leanto(g, LILAC, 0, y0, y1, zf, zb, W + 90)
        for sx in (-1, 1):
            bx(g, CREAM, sx * hw, y0 - 2, max(zlo, 0.0), 20, 20, zf - max(zlo, 0.0))  # corner boards
            win_x(g, sx * hw, (y0 + y1) / 2.0, zf - 175, 80, 100, sx)
        if k:
            for x in (-230, 0, 230):
                bx(g, NAVY, x, y0 - 4, zf - 158, 92, 10, 104)                        # the clerestory strip over the roof in front
            bx(g, CREAM, 0, y0 - 5, zf - 172, W - 60, 12, 10)
    # the front of the lowest bay: the door, a window, the board
    bx(g, NAVY, -150, y00 - 4, 0, 210, 10, 240)
    bx(g, CREAM, -150, y00 - 6, 240, 250, 14, 16)
    win(g, 190, y00, 110, 110, 110)
    bx(g, CREAM, 190, y00 - 6, 250, 240, 8, 64)
    bars(g, 190, y00 - 6, 250, 200)
    g.sphere(ORE, (-150, y00 - 130, 20), 110, scale=(1.3, 1, 0.5), steps=(4, 6))     # ore at the door
    # the head of the mill, on the top bay
    zt = 330.0 + 4 * 330.0 + 140.0                                                   # 1790: the top roof's back edge
    yb = y00 + 5 * D                                                                 # 670
    bx(g, TEAL, -150, yb - 120, zt - 120, 320, 220, 330)                             # the head house
    leanto(g, LILAC, -150, yb - 230, yb - 10, zt + 210, zt + 290, 380)
    win(g, -150, yb - 230, zt + 60, 120, 90)
    bx(g, CREAM, 190, yb - 120, zt - 110, 260, 210, 250)                             # the ore bin
    for z in (zt - 40, zt + 60):
        bx(g, NAVY, 190, yb - 120, z, 268, 218, 10)
    beam(g, LILAC, (190, yb - 225, zt - 60), (190, yb - 420, zt - 200), 80, 20)      # its chute down to the next roof
    top = lamp_room(g, -150, yb - 120, zt + 300, 150, 150, 130)
    vane(sc, -150, yb - 120, top - 70, phase=0.2)
    # the ore gallery up the right side, on two bents
    xg = hw + 75.0
    beam(g, TEAL, (xg, -640, 120), (xg, 560, 1740), 110, 120)
    beam(g, LILAC, (xg, -650, 190), (xg, 566, 1818), 136, 14)
    for y in (-330, 20, 330):
        zc = 120.0 + (y + 640.0) / 1200.0 * 1620.0
        bx(g, NAVY, xg + 56, y, zc - 30, 8, 70, 60)
    for y in (-160, 260):
        zc = 120.0 + (y + 640.0) / 1200.0 * 1620.0
        for sx in (-1, 1):
            bx(g, TEAL, xg + sx * 40, y, -FOOT, 22, 22, zc - 60 + FOOT)
        beam(g, TEAL, (xg - 40, y, 40), (xg + 40, y, zc - 120), 12)
    # the iron stack on the left, strapped to the wall
    rod(g, NAVY, (-hw - 70, 250, -40), (-hw - 70, 250, 2060), 30, steps=7)
    rod(g, CREAM, (-hw - 70, 250, 1960), (-hw - 70, 250, 2000), 36, steps=7)
    bx(g, NAVY, -hw - 34, 250, 900, 60, 12, 12)
    bx(g, NAVY, -hw - 34, 250, 1500, 60, 12, 12)


def tipple(sc):
    """BUILDING. A tipple: a cream ore bin on teal trestle legs, a hopper under it, a chute to the road side, a head
    house with a lamp room, a stub of high track at the back with an ore cart on it. 6 x 8 m, 15 m. Nothing moves
    but a lantern under the bin."""
    g = sc.still()
    ZB = 700.0
    feet = ((-300, -260), (300, -260), (-300, 280), (300, 280))
    heads = ((-230, -190), (230, -190), (-230, 200), (230, 200))
    for (fx, fy), (hx, hy) in zip(feet, heads):
        beam(g, TEAL, (fx, fy, -FOOT), (hx, hy, ZB), 32)
    for z, f in ((330.0, 0.62), (ZB - 20, 1.0)):                                      # girts round the legs
        x = 300 - 70 * (z / ZB)
        yf, yb = -260 + 70 * (z / ZB), 280 - 80 * (z / ZB)
        bx(g, TEAL, 0, yf, z, 2 * x + 30, 22, 22)
        bx(g, TEAL, 0, yb, z, 2 * x + 30, 22, 22)
        for sx in (-1, 1):
            bx(g, TEAL, sx * x, (yf + yb) / 2.0, z, 22, yb - yf, 22)
    xbrace(g, TEAL, (-268, -228, 10), (268, -228, 320))                              # the front
    xbrace(g, TEAL, (-238, -196, 350), (238, -196, 670))
    for sx in (-1, 1):
        xbrace(g, TEAL, (sx * 272, -220, 10), (sx * 272, 240, 320))
    # the bin, banded, and its hopper
    bx(g, CREAM, 0, 5, ZB, 540, 450, 360)
    for z in (ZB + 60, ZB + 180, ZB + 300):
        bx(g, LILAC, 0, 5, z, 550, 460, 12)
    for sx in (-1, 1):
        bx(g, TEAL, sx * 268, -218, ZB, 20, 20, 360)
    g.cyl(CREAM, (0, 5, ZB - 150), 70 * 1.4142, 150, rot=(0, 45, 0), steps=4, top_r=215 * 1.4142)
    bx(g, NAVY, 0, 5, ZB - 176, 110, 110, 28)                                        # the gate
    beam(g, LILAC, (0, -40, ZB - 150), (0, -520, 230), 96, 18)                       # the chute
    for sx in (-1, 1):
        beam(g, LILAC, (sx * 52, -40, ZB - 130), (sx * 52, -520, 250), 8, 40)
    bx(g, TEAL, 0, -430, -FOOT, 22, 22, 300 + FOOT)
    bx(g, TEAL, 0, -430, 290, 130, 22, 18)
    g.sphere(ORE, (0, -560, 30), 120, scale=(1.2, 1, 0.55), steps=(4, 6))            # what came down it
    # the head house and the lamp room
    ZH = ZB + 360.0
    bx(g, TEAL, -60, 40, ZH, 320, 300, 230)
    leanto(g, LILAC, -60, -110, 190, ZH + 230, ZH + 320, 380)
    win(g, -60, -110, ZH + 70, 130, 100)
    lamp_room(g, 170, -120, ZH + 12, 130, 130, 120)
    bx(g, CREAM, -60, -116, ZH + 186, 200, 8, 40)
    bars(g, -60, -116, ZH + 176, 170, 2)
    # the high track coming in at the back, and a cart on it
    ZT = ZH - 30.0
    bx(g, LILAC, 0, 425, ZT, 150, 350, 14)
    for y in (330, 580):
        for sx in (-1, 1):
            beam(g, TEAL, (sx * 150, y, -FOOT), (sx * 60, y, ZT), 22)
        bx(g, TEAL, 0, y, ZT * 0.5, 230, 16, 16)
    for sx in (-1, 1):
        bx(g, NAVY, sx * 45, 425, ZT + 14, 8, 350, 8)
    bx(g, YEL, 0, 470, ZT + 46, 110, 150, 70)
    bx(g, NAVY, 0, 470, ZT + 30, 80, 120, 16)
    g.sphere(ORE, (0, 470, ZT + 116), 50, scale=(1, 1.3, 0.5), steps=(4, 6))
    f = sc.part(P.motion(swing=7, pivot=(-200, -236, ZB - 30), axis=0.0, rate=0.3, phase=0.4))
    lantern(f, -200, -236, ZB - 30, glow=0.6, s=1.3)


def assay_tower(sc):
    """BUILDING. The assay office: a 5 m square teal tower of five floors, banded in bleached boards, a stair
    zigzagging up its face, a railed gallery and the yellow lamp room at the top (22 m). A balance swings at the
    door, a vane turns on the roof."""
    g = sc.still()
    W, H = 420.0, 1700.0
    hw = W / 2.0
    bx(g, TEAL, 0, 0, -FOOT, W, W, H + FOOT)
    for k in range(5):
        z = 340.0 * k
        if k:
            bx(g, LILAC, 0, 0, z - 8, W + 14, W + 14, 16)
        win(g, 120 if k % 2 else -120, -hw, z + 150, 84, 110)
        win_x(g, hw, 0, z + 150, 84, 110, 1)
        win_x(g, -hw, 0, z + 150, 84, 110, -1)
        if k < 4:                                                                    # the stair: a flight a floor, to and fro
            x0, x1 = (-170, 170) if k % 2 == 0 else (170, -170)
            stair(g, x0, x1, -hw - 42, z + 4, z + 336, 72)
            bx(g, CREAM, x1, -hw - 42, z + 330, 80, 84, 12)
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, CREAM, sx * hw, sy * hw, 0, 22, 22, H)
    bx(g, NAVY, -120, -hw - 3, 0, 110, 10, 210)                                      # the door
    bx(g, CREAM, -120, -hw - 6, 210, 150, 14, 14)
    # the gallery and the lamp room
    bx(g, CREAM, 0, 0, H, W + 150, W + 150, 20)
    r = hw + 68
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, TEAL, sx * r, sy * r, H + 20, 14, 14, 96)
        bx(g, TEAL, sx * r, 0, H + 108, 10, 2 * r, 10)
        bx(g, TEAL, 0, sx * r, H + 108, 2 * r, 10, 10)
    top = lamp_room(g, 0, 0, H + 32, 260, 260, 230, glow=0.65)
    vane(sc, 0, 0, top - 70, phase=0.6, L=140.0)
    # the sign of the trade: a balance on a bracket by the door (no words)
    bx(g, NAVY, 150, -hw - 60, 300, 10, 120, 10)
    f = sc.part(P.motion(swing=9, pivot=(150, -hw - 110, 300), axis=90.0, rate=0.3, phase=0.1))
    f.box(CREAM, (150, -hw - 110, 270), (150, 6, 8))
    f.box(NAVY, (150, -hw - 110, 285), (6, 6, 34))
    for sx in (-1, 1):
        f.box(NAVY, (150 + sx * 68, -hw - 110, 244), (6, 6, 48))
        f.box(YEL, (150 + sx * 68, -hw - 110, 216), (44, 30, 8))


def powder_house(sc):
    """BUILDING (the open gaps only). The powder house: a small stone magazine under a teal barrel vault, stepped end
    walls, an iron door with teal straps, a lightning rod on a mast with its ball, a low blast wall in front, kegs.
    6.4 x 5 m, the rod 9 m. A lantern hangs still on its bracket (never near the door: this is a powder house)."""
    g = sc.still()
    L, D, H = 560.0, 420.0, 300.0
    bx(g, LILAC, 0, 0, -FOOT, L, D, H + FOOT)
    rod(g, TEAL, (-L / 2.0 - 16, 0, H), (L / 2.0 + 16, 0, H), D / 2.0 - 4, steps=12)      # the vault
    for sx in (-1, 1):                                                               # stepped end walls
        bx(g, LILAC, sx * (L / 2.0 + 10), 0, -FOOT, 44, D + 40, H + 130 + FOOT)
        bx(g, LILAC, sx * (L / 2.0 + 10), 0, H + 130, 44, D * 0.62, 90)
        bx(g, CREAM, sx * (L / 2.0 + 10), 0, H + 220, 50, D * 0.62 + 10, 14)
        for sy in (-1, 1):
            bx(g, CREAM, sx * (L / 2.0 + 10), sy * (D * 0.31 + 55), H + 130, 50, 110, 14)
    for k in range(4):                                                               # courses of stone on the front
        bx(g, ORE, 0, -D / 2.0 - 2, 40 + k * 70, L - 30, 6, 8)
    for sx in (-1, 1):                                                               # quoins
        for k in range(4):
            bx(g, CREAM, sx * (L / 2.0 - 30), -D / 2.0 - 4, 10 + k * 72, 60 if k % 2 else 40, 10, 40)
    bx(g, NAVY, 0, -D / 2.0 - 5, 0, 130, 12, 210)                                    # the iron door
    for z in (50, 150):
        bx(g, TEAL, 0, -D / 2.0 - 10, z, 140, 6, 14)
    bx(g, CREAM, 0, -D / 2.0 - 6, 210, 180, 16, 26)
    for x in (-190, 190):                                                            # slit vents
        bx(g, NAVY, x, -D / 2.0 - 4, 170, 16, 10, 70)
    # the lightning rod: a mast beside the house, a ball, the strap to earth
    xm = L / 2.0 + 110
    bx(g, TEAL, xm, 60, -FOOT, 22, 22, 640 + FOOT)
    bx(g, NAVY, xm, 60, 640, 8, 8, 230)
    g.sphere(CREAM, (xm, 60, 880), 20, steps=(4, 6))
    beam(g, NAVY, (xm, 60, 520), (L / 2.0 - 60, 0, H + D / 2.0 - 10), 6)
    # the blast wall, with its gap, and the kegs behind it
    for (x, w) in ((-250, 300), (230, 260)):
        bx(g, LILAC, x, -D / 2.0 - 230, -80, w, 70, 80 + 150)
        bx(g, CREAM, x, -D / 2.0 - 230, 150, w + 12, 82, 12)
    for (x, y) in ((-300, -330), (-240, -330), (-270, -330)):
        z = 76 if x == -270 else 0
        g.cyl(CREAM, (x, y - (0 if z else 0), z), 27, 74, steps=7)
        g.cyl(NAVY, (x, y, z + 30), 29, 10, steps=7)
    # a board with bars, and the lantern far from the door on the mast
    bx(g, YEL, 240, -D / 2.0 - 6, 150, 90, 8, 90)
    bx(g, NAVY, 240, -D / 2.0 - 9, 172, 60, 6, 12)
    bx(g, NAVY, 240, -D / 2.0 - 9, 200, 60, 6, 12)
    bx(g, NAVY, xm, 10, 420, 8, 100, 8)
    lantern(g, xm, -36, 420, glow=0.6, s=1.2)


def boarding_rim(sc):
    """BUILDING for the cuts. A boarding house on a 17 m column of the cut's rock: three floors of galleries, a
    ladder down the rock, a stove pipe, a lamp at the gable, washing swinging on the top gallery. 28 m."""
    g = sc.still()
    H = 1700.0
    column(g, 71, 290.0, H)
    deck_on_rock(g, 640, 500, H, 290.0)
    Z0 = H + 18.0
    W, D, FL = 500.0, 320.0, 255.0
    yc = 50.0
    yf = yc - D / 2.0
    bx(g, LILAC, 0, yc, Z0, W, D, 3 * FL)
    for sx in (-1, 1):
        bx(g, TEAL, sx * W / 2.0, yf - 2, Z0, 22, 22, 3 * FL)
        win_x(g, sx * W / 2.0, yc, Z0 + FL + 80, 70, 100, sx)
    for k in range(3):
        z = Z0 + k * FL
        if k:
            bx(g, CREAM, 0, yf - 60, z - 8, W + 60, 124, 12)                         # the gallery's floor
            bx(g, TEAL, 0, yf - 2, z - 16, W, 12, 12)
        bx(g, TEAL, 0, yf - 116, z + 84, W + 60, 8, 8)                               # its rail
        for x in (-W / 2.0 - 24, 0, W / 2.0 + 24):
            bx(g, TEAL, x, yf - 116, z, 12, 12, FL if k < 2 else 200)                # posts, floor to floor
        bx(g, NAVY, -150 if k % 2 == 0 else 150, yf - 3, z + 4, 76, 10, 170)         # a door
        bx(g, NAVY, 150 if k % 2 == 0 else -150, yf - 3, z + 70, 80, 10, 96)         # a window
        bx(g, NAVY, 0, yf - 3, z + 70, 70, 10, 96)
    ZR = Z0 + 3 * FL
    roof_eaves(g, LILAC, 0, yc - 40, W + 60, D + 130, ZR, 130, over=24, wall=LILAC)
    rod(g, NAVY, (160, yc + 60, ZR + 40), (160, yc + 60, ZR + 300), 16, steps=6)     # the stove pipe
    g.cyl(NAVY, (160, yc + 60, ZR + 300), 26, 20, steps=6, top_r=8)
    bx(g, NAVY, 0, yf - 60, ZR - 40, 8, 110, 8)                                      # the lamp, hung under the eave
    lantern(g, 0, yf - 110, ZR - 40, glow=0.65, s=1.3)
    ladder(g, -220, -262, H - 520, H + 30, rungs=6)                                  # down the rock to a ledge
    bx(g, RK_D, -220, -300, H - 560, 200, 130, 40)
    washing(sc, -W / 2.0 - 20, W / 2.0 + 20, yf - 124, Z0 + 2 * FL + 150, (CREAM, MAG, YEL, CREAM), phase=0.1, n=4)


def hoist_house(sc):
    """BUILDING. The hoist house: a teal engine house under a lilac roof with a glazed monitor on its ridge, the
    winding drum showing through its open front, a cream boiler in a cradle at its left, a 20 m iron chimney at its
    right with a pale smoke puff that bobs over it. 8 x 6.5 m."""
    g = sc.still()
    W, D, H = 700.0, 560.0, 480.0
    hw, hd = W / 2.0, D / 2.0
    bx(g, TEAL, 0, 0, -FOOT, W, D, H + FOOT)
    for k in range(6):                                                               # bleached boards up the front
        bx(g, LILAC, -hw + 40 + k * 26, -hd - 2, 300, 12, 6, 180)
    roof_eaves(g, LILAC, 0, 0, W, D, H, 220, over=40, wall=TEAL)
    bx(g, TEAL, 0, 0, H + 190, W * 0.5, 150, 110)                                    # the monitor
    for x in (-110, 0, 110):
        bx(g, YEL, x, -78, H + 220, 70, 8, 56, glow=0.55)
    roof_eaves(g, LILAC, 0, 0, W * 0.5, 150, H + 300, 60, over=24, t=12)
    vane(sc, 0, 0, H + 372, phase=0.8)
    # the open front: the drum, its flanges, the rope going up through the roof
    bx(g, NAVY, 90, -hd - 4, 0, 330, 12, 330)
    bx(g, CREAM, 90, -hd - 7, 330, 370, 16, 18)
    for sx in (-1, 1):
        bx(g, CREAM, 90 + sx * 176, -hd - 7, 0, 18, 16, 330)
    rod(g, CREAM, (-30, -hd - 30, 150), (210, -hd - 30, 150), 70, steps=10)
    for x in (-36, 216):
        rod(g, YEL, (x - 6, -hd - 30, 150), (x + 6, -hd - 30, 150), 96, steps=10)
    bx(g, TEAL, 90, -hd - 30, 0, 300, 60, 70)
    beam(g, CREAM, (90, -hd - 60, 215), (90, -hd - 330, 760), 8)                     # the rope, off towards the shaft
    # door, windows, the lamp over the door
    bx(g, NAVY, -230, -hd - 4, 0, 100, 10, 220)
    bx(g, CREAM, -230, -hd - 6, 220, 130, 14, 14)
    win_x(g, -hw, -120, 190, 100, 130, -1)
    win_x(g, -hw, 130, 190, 100, 130, -1)
    win_x(g, hw, -120, 190, 100, 130, 1)
    bx(g, NAVY, -230, -hd - 50, 330, 8, 90, 8)
    lantern(g, -230, -hd - 90, 330, glow=0.65, s=1.2)
    # the boiler in its cradle, strapped, with a dome and a whistle
    xb = -hw - 150.0
    rod(g, CREAM, (xb, -230, 170), (xb, 210, 170), 105, steps=10)
    for y in (-150, 0, 150):
        rod(g, NAVY, (xb, y - 6, 170), (xb, y + 6, 170), 110, steps=10)
    g.cyl(CREAM, (xb, 60, 260), 44, 70, steps=7, top_r=30)
    bx(g, YEL, xb, 60, 330, 12, 12, 46)
    for y in (-170, 150):
        bx(g, LILAC, xb, y, -80, 250, 50, 150)
    rod(g, NAVY, (xb, 150, 250), (-hw, 150, 360), 12, steps=6)                       # the steam pipe into the house
    # the chimney on a stone foot, banded, stayed back to the roof
    xc = hw + 130.0
    bx(g, LILAC, xc, 60, -FOOT, 170, 170, 150 + FOOT)
    bx(g, CREAM, xc, 60, 150, 186, 186, 16)
    g.cyl(NAVY, (xc, 60, 166), 50, 1834, steps=8, top_r=36)
    for z in (700.0, 1300.0, 1940.0):
        rr = 50 - 14 * (z - 166) / 1834.0
        g.cyl(CREAM, (xc, 60, z), rr + 6, 30, steps=8)
    beam(g, NAVY, (xc - 30, 60, 1250), (hw - 120, 0, H + 190), 6)
    # the smoke: three pale puffs that float over the stack
    for k, (dz, rr, ph) in enumerate(((120, 60, 0.0), (250, 84, 0.3), (420, 112, 0.62))):
        f = sc.part(P.motion(bob=-(16 + 10 * k), rate=0.22, phase=ph))
        f.sphere(CREAM if k != 1 else LILAC, (xc + 26 * k, 60, 2000 + dz), rr, scale=(1.15, 1, 0.8), steps=(4, 6))
    # the wood pile and a barrel
    for k in range(3):
        rod(g, LILAC, (hw + 40, -250 + k * 46, 22), (hw + 250, -250 + k * 46, 22), 22, steps=6)
    for k in range(2):
        rod(g, LILAC, (hw + 40, -227 + k * 46, 62), (hw + 250, -227 + k * 46, 62), 22, steps=6)


def rim_frame(sc):
    """BUILDING for the cuts. A prospect shaft's headframe on a 17 m column of the cut's rock: a teal A-frame with
    one sheave wheel winding to and fro, a lamp cabin at its waist, a cream bin, a shed at its foot, a pennant. 32 m."""
    g = sc.still()
    H = 1700.0
    column(g, 83, 280.0, H)
    deck_on_rock(g, 620, 520, H, 280.0)
    Z0 = H + 18.0
    ZT = Z0 + 1060.0
    fr = ((-150, -150), (150, -150))                                                 # two upright front posts
    for (x, y) in fr:
        bx(g, TEAL, x, y, Z0, 30, 30, ZT - Z0)
    for sx in (-1, 1):                                                               # two raking back legs
        beam(g, TEAL, (sx * 190, 220, Z0), (sx * 150, -100, ZT), 30)
    for z in (Z0 + 350, Z0 + 700, ZT - 20):
        bx(g, TEAL, 0, -150, z, 330, 22, 22)
    xbrace(g, TEAL, (-150, -150, Z0 + 20), (150, -150, Z0 + 340), 14)
    xbrace(g, TEAL, (-150, -150, Z0 + 372), (150, -150, Z0 + 690), 14)
    for sx in (-1, 1):
        beam(g, TEAL, (sx * 150, -150, Z0 + 350), (sx * 176, 110, Z0 + 350), 18)
        beam(g, TEAL, (sx * 150, -150, Z0 + 700), (sx * 162, -4, Z0 + 700), 18)
    bx(g, CREAM, 0, -110, ZT, 420, 220, 18)                                          # the wheel's platform
    for sx in (-1, 1):
        bx(g, TEAL, sx * 200, -212, ZT + 18, 10, 10, 80)
        bx(g, TEAL, sx * 200, -8, ZT + 18, 10, 10, 80)
    bx(g, TEAL, 0, -212, ZT + 92, 410, 8, 8)
    ZW = ZT + 200.0
    for y in (-150, -70):                                                            # the bearings: two small A-frames
        beam(g, TEAL, (-70, y, ZT + 18), (0, y, ZW), 16)
        beam(g, TEAL, (70, y, ZT + 18), (0, y, ZW), 16)
    w = sc.part(P.motion(swing=160, pivot=(0, -110, ZW), axis=90.0, rate=0.09, phase=0.2))
    wheel(w, 0, -110, ZW, 165.0, n=10, t=26, w=26, spokes=3)
    beam(g, CREAM, (165, -110, ZW), (165, -110, Z0 + 200), 7)                        # the rope down to the collar
    beam(g, CREAM, (-150, -110, ZW + 70), (-200, 200, Z0 + 190), 7)                  # ... and back to the winch shed
    # the lamp cabin and the bin at the waist, the shed and the collar at the foot
    lamp_room(g, -40, -200, Z0 + 720, 150, 110, 130, glow=0.65, finial=False)
    bx(g, CREAM, 110, -30, Z0 + 372, 150, 190, 160)
    bx(g, NAVY, 110, -30, Z0 + 440, 158, 198, 10)
    beam(g, LILAC, (110, -130, Z0 + 380), (110, -240, Z0 + 250), 60, 14)
    bx(g, LILAC, -170, 150, Z0, 220, 190, 180)
    leanto(g, LILAC, -170, 55, 245, Z0 + 180, Z0 + 240, 260, over=20)
    bx(g, NAVY, -170, 52, Z0, 70, 8, 150)
    bx(g, LILAC, 165, -110, Z0, 110, 110, 40)
    bx(g, NAVY, 165, -110, Z0 + 40, 80, 80, 6)
    ladder(g, 120, -168, Z0, ZT, rungs=9)
    pennant(sc, 0, 0, ZT + 18, h=520.0, phase=0.5)


def bunk_house(sc):
    """BUILDING. The boarding house of the camp: four narrow floors of bleached boards in a teal frame, galleries
    on three of them, a stair up its right end, a steep roof with its gable to the road, a lamp over the door, a
    water butt, a flower box, washing swinging on a line out to a post. 7 x 5.6 m, 16 m."""
    g = sc.still()
    W, D, FL = 640.0, 480.0, 285.0
    hw, hd = W / 2.0, D / 2.0
    yc = 60.0
    yf = yc - hd
    HT = 4 * FL
    bx(g, LILAC, 0, yc, -FOOT, W, D, HT + FOOT)
    for sx in (-1, 1):
        bx(g, TEAL, sx * hw, yf - 2, -FOOT, 26, 26, HT + FOOT)
        bx(g, TEAL, sx * hw, yc + hd, -FOOT, 26, 26, HT + FOOT)
    for k in range(4):
        z = k * FL
        bx(g, TEAL, 0, yf - 3, z + FL - 14, W, 14, 14)                               # the floor beam showing on the face
        if 1 <= k <= 3:
            bx(g, CREAM, 0, yf - 66, z - 8, W + 40, 132, 12)                         # gallery
            bx(g, TEAL, 0, yf - 128, z + 88, W + 40, 8, 8)
            bx(g, TEAL, 0, yf - 128, z + 44, W + 40, 6, 6)
        for x in (-hw - 14, -hw / 3.0, hw / 3.0, hw + 14):
            if k < 3:
                bx(g, TEAL, x, yf - 128, z, 13, 13, FL)
            elif abs(x) > hw:
                bx(g, TEAL, x, yf - 128, z, 13, 13, 96)
        d = -200 if k % 2 == 0 else 200
        bx(g, NAVY, d, yf - 3, z + 2, 84, 10, 190)                                   # a door
        bx(g, CREAM, d, yf - 6, z + 192, 104, 12, 12)
        win(g, -d, yf, z + 84, 84, 104)
        win(g, 0, yf, z + 84, 70, 104)
        win_x(g, -hw, yc, z + 84, 80, 104, -1)
    roof_gable(g, LILAC, 0, yc - 50, W, D + 140, HT, 260, over=36, wall=LILAC)
    win(g, 0, yc - 50 - (D + 140) / 2.0, HT + 40, 70, 90)                            # the attic
    rod(g, NAVY, (-180, yc + 120, HT + 60), (-180, yc + 120, HT + 380), 18, steps=6)
    g.cyl(NAVY, (-180, yc + 120, HT + 380), 30, 22, steps=6, top_r=8)
    vane(sc, 0, yc + 150, HT + 270, phase=0.45)
    # the stair up the right end, floor to floor, on the end wall
    xs = hw + 46.0
    for k in range(3):
        y0, y1 = (yc + hd - 20, yf - 60) if k % 2 == 0 else (yf - 60, yc + hd - 20)
        beam(g, LILAC, (xs, y0, k * FL + 2), (xs, y1, (k + 1) * FL - 6), 76, 14)
        beam(g, TEAL, (xs + 38, y0, k * FL + 92), (xs + 38, y1, (k + 1) * FL + 84), 8)
    for y in (yf - 60, yc + hd - 20):
        bx(g, TEAL, xs + 38, y, -FOOT, 16, 16, 3 * FL + 90 + FOOT)
    # small things: the lamp, the board, a water butt and its down pipe, a flower box, a bench
    bx(g, NAVY, -200, yf - 60, FL - 40, 8, 110, 8)
    lantern(g, -200, yf - 112, FL - 40, glow=0.65, s=1.2)
    bx(g, CREAM, 0, yf - 134, FL + 96, 260, 8, 62)
    bars(g, 0, yf - 134, FL + 92, 220)
    g.cyl(LILAC, (-hw - 70, yf + 40, 0), 46, 120, steps=8)
    g.cyl(NAVY, (-hw - 70, yf + 40, 50), 48, 10, steps=8)
    bx(g, NAVY, -hw - 20, yf + 40, 120, 12, 12, HT - 120)
    bx(g, TEAL, 200, yf - 12, 2 * FL + 66, 100, 22, 18)
    for x in (170, 200, 230):
        g.sphere(MAG, (x, yf - 14, 2 * FL + 96), 15, steps=(4, 6))
    bx(g, LILAC, 40, yf - 60, 40, 170, 40, 8)
    for x in (-30, 110):
        bx(g, LILAC, x, yf - 60, 0, 10, 36, 40)
    # the washing: from the first gallery out to a post
    bx(g, TEAL, -hw - 14, yf - 520, -FOOT, 18, 18, FL + 150 + FOOT)
    bx(g, TEAL, -hw - 14, yf - 520, FL + 130, 110, 12, 12)
    g2 = sc.still()
    beam(g2, CREAM, (-hw - 14, yf - 128, FL + 132), (-hw - 14, yf - 520, FL + 132), 5)
    for k, col in enumerate((CREAM, MAG, YEL, CREAM, TEAL)):
        y = yf - 180 - k * 70
        f = sc.part(P.motion(swing=11, pivot=(-hw - 14, y, FL + 132), axis=90.0, rate=0.45, phase=0.17 * k))
        if k % 2 == 0:
            f.box(col, (-hw - 14, y, FL + 100), (5, 44, 56))
            f.box(col, (-hw - 14, y - 12, FL + 52), (5, 16, 46))
            f.box(col, (-hw - 14, y + 12, FL + 52), (5, 16, 46))
        else:
            f.box(col, (-hw - 14, y, FL + 96), (5, 58, 68))


# ------------------------------------------------------------------------------------------ props
def ore_cart(sc):
    """PROP, LOW (1.5 m). Three ore carts coupled on 9 m of narrow track, a timber buffer stop at one end."""
    g = sc.still()
    for sy in (-1, 1):
        bx(g, NAVY, 0, sy * 34, 14, 900, 8, 10)
    for k in range(8):
        bx(g, LILAC, -400 + k * 114, 0, 0, 22, 130, 14)
    for k, x in enumerate((-250, -60, 130)):
        bx(g, NAVY, x, 0, 46, 120, 74, 14)                                           # the frame
        g.cyl(YEL, (x, 0, 60), 50 * 1.4142, 62, rot=(0, 45, 0), steps=4, top_r=74 * 1.4142)   # the tub, wider at the lip
        bx(g, NAVY, x, 0, 118, 152, 152, 6)
        g.sphere(ORE if k != 1 else CREAM, (x, 0, 126), 52, scale=(1.1, 1, 0.5), steps=(4, 6))
        for wx in (-38, 38):
            rod(g, NAVY, (x + wx, -44, 34), (x + wx, -36, 34), 22, steps=7)
            rod(g, NAVY, (x + wx, 36, 34), (x + wx, 44, 34), 22, steps=7)
        if k:
            bx(g, NAVY, x - 95, 0, 50, 70, 8, 8)
    for sy in (-1, 1):                                                               # the buffer stop
        beam(g, TEAL, (300, sy * 50, 0), (372, sy * 50, 130), 16)
        bx(g, TEAL, 380, sy * 50, 0, 16, 16, 150)
    bx(g, CREAM, 372, 0, 104, 22, 140, 30)
    g.sphere(ORE, (-390, 70, 14), 60, scale=(1.3, 1, 0.5), steps=(4, 6))


def adit_portal(sc):
    """PROP, tall (5 m). A timbered adit in a hump of red rock: teal posts and cap, lagging boards, a dark mouth,
    rails running out, a board with crossed pick and shovel (no words), a vent pipe, a lantern swinging on the cap."""
    r = random.Random(77)
    g = sc.still()
    for k, (z0, z1, col, f) in enumerate(((-150, 150, TALUS, 1.0), (150, 300, RK_D, 0.8), (300, 400, RK, 0.6), (400, 455, RK_L, 0.4))):
        pts = ring(r, 420 * f, 230 * f, 9, 0.12, cy=70 + 22 * k)
        g.plan(col, ccw(pts), z0, z1)
    yf = -160.0
    bx(g, NAVY, 0, yf - 4, 0, 190, 60, 230)                                          # the mouth
    for sx in (-1, 1):
        beam(g, TEAL, (sx * 122, yf - 30, -20), (sx * 104, yf - 30, 240), 30)
    bx(g, TEAL, 0, yf - 30, 240, 290, 36, 30)
    for k in range(4):
        bx(g, LILAC, -105 + k * 70, yf - 22, 272, 62, 14, 50 + (k % 2) * 10)         # lagging over the cap
    for sy in (-1, 1):
        bx(g, NAVY, sy * 30, yf - 150, 8, 8, 260, 8)                                 # rails
    for k in range(3):
        bx(g, LILAC, 0, yf - 60 - k * 80, 0, 110, 18, 10)
    bx(g, CREAM, 0, yf - 48, 330, 150, 8, 100)                                       # the board
    beam(g, NAVY, (-52, yf - 54, 346), (52, yf - 54, 414), 8)
    beam(g, NAVY, (52, yf - 54, 346), (-52, yf - 54, 414), 8)
    bx(g, NAVY, -50, yf - 54, 408, 44, 6, 10)
    bx(g, NAVY, 50, yf - 54, 404, 24, 6, 24)
    rod(g, NAVY, (200, 40, 380), (200, 40, 500), 12, steps=6)                        # the vent pipe
    g.cyl(NAVY, (200, 40, 500), 22, 16, steps=6, top_r=6)
    for k in range(3):                                                               # pit props stacked by the mouth
        rod(g, LILAC, (190, yf - 120 + k * 36, 16), (400, yf - 120 + k * 36, 16), 16, steps=6)
    rod(g, LILAC, (190, yf - 102, 46), (400, yf - 102, 46), 16, steps=6)
    bx(g, NAVY, -150, yf - 70, 268, 8, 70, 8)
    f = sc.part(P.motion(swing=8, pivot=(-150, yf - 100, 268), axis=0.0, rate=0.35, phase=0.2))
    lantern(f, -150, yf - 100, 268, glow=0.65, s=1.2)


def stamp_battery(sc):
    """PROP, tall (7 m). A five-stamp battery: a teal timber frame, five iron stems with cream shoes that rise and
    fall one after another, the mortar box, the cam shaft and its belt wheel, an apron table, a feed chute, a roof."""
    g = sc.still()
    for sx in (-1, 1):
        bx(g, TEAL, sx * 240, 0, -60, 36, 36, 700)
        beam(g, TEAL, (sx * 240, 150, -60), (sx * 240, 10, 420), 22)
    bx(g, TEAL, 0, 0, 600, 540, 42, 40)
    for z in (300, 460):
        bx(g, LILAC, 0, -6, z, 470, 26, 22)
    bx(g, LILAC, 0, 0, -60, 520, 130, 110)                                           # the mortar block
    bx(g, NAVY, 0, -10, 50, 470, 100, 100)                                           # the mortar box
    bx(g, CREAM, 0, -62, 60, 430, 6, 60)                                             # its screen
    rod(g, CREAM, (-250, 40, 540), (310, 40, 540), 14, steps=6)                      # the cam shaft
    rod(g, NAVY, (300, 40, 540), (324, 40, 540), 120, steps=10)                      # the belt wheel
    rod(g, YEL, (322, 40, 540), (330, 40, 540), 30, steps=6)
    beam(g, CREAM, (312, 150, 60), (312, 60, 440), 20, 6)                            # the belt
    for k in range(5):
        x = -160 + k * 80
        f = sc.part(P.motion(bob=-20, rate=0.7, phase=k * 0.4))
        f.box(NAVY, (x, -6, 400), (14, 14, 440))
        f.box(CREAM, (x, -6, 190), (46, 46, 70))
        f.box(YEL, (x, -6, 500), (30, 30, 22))
        rod(g, CREAM, (x - 8, 40, 540), (x + 8, 40, 540), 28, steps=6)               # its cam
    beam(g, CREAM, (0, -70, 70), (0, -250, 30), 430, 10)                             # the apron table
    for sx in (-1, 1):
        bx(g, TEAL, sx * 190, -240, -40, 14, 14, 66)
    beam(g, LILAC, (0, 250, 420), (0, 40, 170), 300, 14)                             # the feed chute
    for sx in (-1, 1):
        bx(g, TEAL, sx * 140, 240, -60, 18, 18, 470)
    g.sphere(ORE, (0, 250, 440), 90, scale=(1.4, 0.8, 0.5), steps=(4, 6))
    leanto(g, LILAC, 0, -90, 190, 660, 730, 620, over=30)


def flume(sc):
    """PROP, tall (6 m). 14 m of water flume on four raking trestle bents: a bleached board trough, a ribbon of
    water in it, a sluice gate with a yellow hand wheel, a spout at one end into a tub, a bucket swinging on a rope."""
    g = sc.still()
    Z = 480.0
    bx(g, LILAC, 0, 0, Z, 1400, 96, 10)
    for sy in (-1, 1):
        bx(g, LILAC, 0, sy * 48, Z, 1400, 10, 64)
    bx(g, WATER, 10, 0, Z + 38, 1380, 78, 6)
    for x in (-600, -200, 200, 600):
        for sy in (-1, 1):
            beam(g, TEAL, (x, sy * 120, -150), (x, sy * 46, Z - 4), 22)
        bx(g, TEAL, x, 0, Z - 26, 30, 150, 22)
        bx(g, TEAL, x, 0, 200, 16, 180, 16)
        for sy in (-1, 1):                                                           # the yokes round the trough
            bx(g, TEAL, x, sy * 58, Z - 10, 14, 10, 90)
        bx(g, TEAL, x, 0, Z + 78, 14, 126, 10)
    xbrace(g, TEAL, (-600, -84, 60), (-200, -84, 400), 12)
    xbrace(g, TEAL, (200, -84, 60), (600, -84, 400), 12)
    bx(g, LILAC, 0, -80, 205, 420, 10, 14)
    # the gate and its wheel
    bx(g, TEAL, 0, 0, Z + 60, 12, 110, 110)
    bx(g, NAVY, 0, 0, Z + 170, 8, 8, 50)
    g.cyl(YEL, (0, 0, Z + 216), 30, 8, steps=8)
    # the spout and the tub
    beam(g, LILAC, (700, 0, Z + 4), (770, 0, Z - 40), 80, 10)
    bx(g, WATER, 772, 0, 96, 40, 44, Z - 140)
    g.cyl(LILAC, (772, 0, -40), 78, 140, steps=8, top_r=86)
    g.cyl(WATER, (772, 0, 96), 74, 6, steps=8)
    g.cyl(NAVY, (772, 0, 30), 84, 10, steps=8)
    # a bucket on a rope
    f = sc.part(P.motion(swing=8, pivot=(-400, -60, Z - 10), axis=0.0, rate=0.4, phase=0.3))
    f.box(CREAM, (-400, -60, Z - 100), (5, 5, 180))
    f.cyl(CREAM, (-400, -60, Z - 240), 22, 50, steps=7, top_r=28)
    f.cyl(NAVY, (-400, -60, Z - 196), 30, 6, steps=7)


def sluice_box(sc):
    """PROP, LOW (1.6 m). A sluice box on low trestles: a 6 m riffled trough with a ribbon of water, three gold
    pans on a bench, a rocker cradle that rocks, a shovel, a bucket, a heap of tailings."""
    g = sc.still()
    beam(g, LILAC, (-300, 0, 128), (300, 0, 52), 76, 10)
    for sy in (-1, 1):
        beam(g, LILAC, (-300, sy * 38, 146), (300, sy * 38, 70), 8, 40)
    beam(g, WATER, (-290, 0, 138), (296, 0, 62), 58, 5)
    for k in range(6):
        x = -240 + k * 96
        bx(g, CREAM, x, 0, 134 - (x + 300) * 76 / 600.0, 8, 66, 12)
    for x, zt in ((-250, 122), (0, 90), (250, 58)):
        for sy in (-1, 1):
            beam(g, TEAL, (x, sy * 66, -30), (x, sy * 34, zt), 12)
        bx(g, TEAL, x, 0, zt - 12, 14, 84, 10)
    bx(g, LILAC, -360, 0, 138, 90, 110, 40)                                          # the head box
    for sy in (-1, 1):
        bx(g, TEAL, -360, sy * 50, -30, 12, 12, 168)
    g.sphere(ORE, (380, 10, 6), 86, scale=(1.3, 1, 0.45), steps=(4, 6))              # tailings at the tail
    # the bench and the pans
    bx(g, LILAC, 120, -150, 46, 250, 56, 8)
    for x in (10, 230):
        bx(g, LILAC, x, -150, -20, 10, 50, 66)
    for k, x in enumerate((40, 120, 200)):
        g.cyl(CREAM, (x, -150, 54), 20, 9, steps=8, top_r=31)
        g.cyl(YEL if k == 1 else ORE, (x, -150, 60), 14, 5, steps=6)
    # a bucket, a shovel against the trough
    g.cyl(CREAM, (-150, -120, 0), 20, 44, steps=7, top_r=26)
    g.cyl(NAVY, (-150, -120, 40), 27, 5, steps=7)
    beam(g, NAVY, (-60, -110, 4), (-74, -44, 150), 6)
    bx(g, CREAM, -58, -116, 0, 26, 8, 34)
    # the rocker
    f = sc.part(P.motion(swing=7, pivot=(-260, -150, 10), axis=0.0, rate=0.6, phase=0.2))
    f.box(TEAL, (-260, -150, 54), (120, 62, 46))
    f.box(NAVY, (-260, -150, 78), (100, 46, 6))
    f.box(LILAC, (-312, -150, 16), (14, 84, 18))
    f.box(LILAC, (-208, -150, 16), (14, 84, 18))
    f.box(TEAL, (-260, -150, 104), (8, 8, 56))


def tram_tower(sc):
    """PROP, tall (13 m). A tower of the aerial tramway: four raking teal legs, girts and crossed braces, a cross arm
    under a little roof, a length of rope, one cream bucket of ore hanging from it and swaying (it does not travel),
    a ladder, a signal flag."""
    g = sc.still()
    ZT = 1150.0
    feet = ((-170, -130), (170, -130), (-170, 130), (170, 130))
    for (fx, fy) in feet:
        beam(g, TEAL, (fx, fy, -200), (fx * 0.27, fy * 0.3, ZT), 26)
    for z in (300.0, 720.0):
        f = 1.0 - 0.73 * (z + 200) / (ZT + 200)
        f2 = 1.0 - 0.7 * (z + 200) / (ZT + 200)
        for sy in (-1, 1):
            bx(g, TEAL, 0, sy * 130 * f2, z, 340 * f + 24, 18, 18)
        for sx in (-1, 1):
            bx(g, TEAL, sx * 170 * f, 0, z, 18, 260 * f2, 18)
    xbrace(g, TEAL, (-148, -116, -80), (148, -116, 290), 12)
    xbrace(g, TEAL, (-108, -86, 330), (108, -86, 700), 12)
    bx(g, CREAM, 0, 0, ZT, 110, 110, 20)                                             # the cap
    bx(g, TEAL, 0, -60, ZT + 20, 26, 420, 26)                                        # the cross arm, out over the road side
    beam(g, TEAL, (0, -40, ZT - 220), (0, -240, ZT + 20), 14)
    pyr(g, LILAC, 0, 0, ZT + 90, 110, 70)
    for sx in (-1, 1):
        bx(g, TEAL, sx * 40, 0, ZT + 46, 10, 10, 44)
    rod(g, NAVY, (0, -252, ZT - 14), (0, -238, ZT - 14), 30, steps=8)                # the sheave
    bx(g, NAVY, 0, -245, ZT - 46, 760, 6, 6)                                         # the rope
    ladder(g, 0, -150, -200, 300, rungs=5)
    f = sc.part(P.motion(swing=9, pivot=(60, -245, ZT - 46), axis=90.0, rate=0.3, phase=0.1))
    f.box(NAVY, (60, -245, ZT - 40), (34, 10, 12))
    f.box(NAVY, (60, -245, ZT - 130), (6, 6, 170))
    f.box(NAVY, (60, -245, ZT - 218), (120, 6, 6))
    f.cyl(CREAM, (60, -245, ZT - 320), 38 * 1.4142, 100, rot=(0, 45, 0), steps=4, top_r=56 * 1.4142)
    f.box(NAVY, (60, -245, ZT - 222), (118, 118, 6))
    f.sphere(ORE, (60, -245, ZT - 212), 40, scale=(1, 1, 0.5), steps=(4, 6))
    pennant(sc, 0, 60, ZT + 150, h=170.0, col=CREAM, tip=YEL, L=120.0, phase=0.7)


def windlass(sc):
    """PROP, tall (3.6 m). A prospect shaft: a cribbed timber collar, a hand windlass on two A-frames, its bucket
    swaying on the rope, a ladder standing out of the shaft, a little roof on four posts, a lantern on a post."""
    g = sc.still()
    for k in range(2):                                                               # the cribbing, two courses
        z = k * 34
        for s in (-1, 1):
            if k == 0:
                bx(g, LILAC, 0, s * 105, z, 250, 30, 34)
            else:
                bx(g, LILAC, s * 105, 0, z, 30, 250, 34)
    bx(g, NAVY, 0, 0, 20, 180, 180, 6)
    for sx in (-1, 1):
        for sy in (-1, 1):
            beam(g, TEAL, (sx * 120, sy * 60, 0), (sx * 120, 0, 210), 16)
        bx(g, NAVY, sx * 150, 0, 196, 8, 70, 8)                                      # the cranks
        bx(g, NAVY, sx * 150, 34, 170, 8, 8, 60)
    rod(g, CREAM, (-124, 0, 200), (124, 0, 200), 22, steps=8)
    f = sc.part(P.motion(swing=7, pivot=(0, 0, 200), axis=90.0, rate=0.35, phase=0.5))
    f.box(CREAM, (0, 0, 135), (5, 5, 130))
    f.cyl(CREAM, (0, 0, 40), 26, 46, steps=7, top_r=32)
    f.cyl(NAVY, (0, 0, 82), 34, 6, steps=7)
    ladder(g, 60, 62, 10, 190, rungs=4)
    for sx in (-1, 1):                                                               # the roof on four posts
        for sy in (-1, 1):
            bx(g, TEAL, sx * 160, sy * 140, -30, 14, 14, 310)
    pyr(g, LILAC, 0, 0, 280, 200, 80)
    g.sphere(ORE, (-230, -120, 8), 70, scale=(1.2, 1, 0.5), steps=(4, 6))
    beam(g, NAVY, (-200, -60, 20), (-250, -150, 60), 6)                              # a pick on the heap
    bx(g, NAVY, -254, -156, 50, 60, 8, 8)
    bx(g, TEAL, 250, -120, -30, 14, 14, 270)                                         # the lantern post
    bx(g, NAVY, 250, -150, 232, 8, 70, 8)
    lantern(g, 250, -178, 232, glow=0.6)


def bell_frame(sc):
    """PROP, tall (5.6 m). The shift bell: a teal gallows with a little roof, a yellow bell that swings, a board of
    tally pegs (bars, no words), a bench with lunch pails, a lantern, a vane."""
    g = sc.still()
    for sx in (-1, 1):
        bx(g, TEAL, sx * 120, 0, -40, 26, 26, 460)
        beam(g, TEAL, (sx * 120, 0, 300), (sx * 40, 0, 410), 14)
        beam(g, TEAL, (sx * 120, 110, -40), (sx * 120, 6, 200), 14)
    bx(g, TEAL, 0, 0, 410, 330, 30, 28)
    roof_eaves(g, LILAC, 0, 0, 330, 110, 446, 50, over=20, t=10)
    vane(sc, 0, 0, 500, phase=0.35, L=100.0)
    f = sc.part(P.motion(swing=15, pivot=(0, 0, 408), axis=90.0, rate=0.45, phase=0.0))
    f.box(NAVY, (0, 0, 392), (10, 10, 34))
    f.cyl(YEL, (0, 0, 300), 52, 80, steps=8, top_r=24)
    f.cyl(YEL, (0, 0, 292), 58, 10, steps=8)
    f.box(NAVY, (0, 0, 276), (10, 10, 30))
    beam(g, CREAM, (60, 0, 400), (112, -20, 150), 5)                                 # the bell rope, tied off
    bx(g, CREAM, 0, -16, 150, 180, 8, 110)                                           # the tally board
    for k in range(3):
        for j in range(4):
            bx(g, YEL if (k + j) % 3 == 0 else NAVY, -60 + j * 40, -22, 166 + k * 30, 12, 6, 12)
    bx(g, LILAC, -230, -40, 44, 180, 44, 8)                                          # the bench and the pails
    for x in (-300, -160):
        bx(g, LILAC, x, -40, -20, 10, 40, 64)
    for x in (-270, -220):
        g.cyl(CREAM, (x, -40, 52), 13, 24, steps=6)
        bx(g, NAVY, x, -40, 76, 20, 6, 6)
    bx(g, NAVY, 120, -40, 330, 8, 80, 8)
    lantern(g, 120, -74, 330, glow=0.6)


def wash_line(sc):
    """PROP, tall (2.9 m). Wash day: two teal posts with cross arms, two lines of long johns, shirts and blankets
    that swing, a tin bath against a post, a tub and washboard on a stool, a basket."""
    g = sc.still()
    for sx in (-1, 1):
        bx(g, TEAL, sx * 260, 0, -40, 20, 20, 320)
        bx(g, TEAL, sx * 260, 0, 250, 14, 150, 14)
        beam(g, TEAL, (sx * 260, 0, 170), (sx * 260, 60, 250), 10)
    washing(sc, -258, 258, -60, 256, (CREAM, MAG, LILAC, CREAM, YEL), phase=0.0, n=5)
    washing(sc, -258, 258, 60, 256, (YEL, CREAM, WATER, CREAM), phase=0.4, n=4)
    rod(g, CREAM, (286, -8, 60), (300, 8, 64), 56, steps=9)                          # the tin bath on its side
    rod(g, NAVY, (284, -10, 60), (288, -6, 61), 60, steps=9)
    bx(g, LILAC, -120, -170, 40, 60, 60, 8)                                          # stool, tub, board
    for sx in (-1, 1):
        bx(g, LILAC, -120 + sx * 22, -170, -10, 8, 50, 50)
    g.cyl(CREAM, (-120, -170, 48), 34, 30, steps=8, top_r=42)
    g.cyl(WATER, (-120, -170, 72), 38, 6, steps=8)
    beam(g, LILAC, (-120, -150, 60), (-120, -190, 120), 34, 6)
    g.cyl(LILAC, (60, -170, 0), 40, 30, steps=7, top_r=48)                           # the basket
    g.sphere(CREAM, (60, -170, 36), 36, scale=(1, 1, 0.5), steps=(4, 6))


# ------------------------------------------------------------------------------------------ the hero
def twin_headframe(sc):
    """HERO. THE TWIN-WHEEL HEADFRAME: a 40 m teal timber headframe, four posts in six braced panels, two back
    stays raking down to the hoist house, a cream ore bin at its waist with a high track out to the dump, the yellow
    lamp room on its face, and at its head two 6 m sheave wheels that wind to and fro, one against the other, while
    the cage rides up and down the shaft. Lamps up the stays (steady), a long pennant on the roof. 21 x 9 m."""
    g = sc.still()
    X0 = -175.0                                                                      # the tower's middle; the stays run to +X
    ZD = 3300.0                                                                      # the wheel deck
    bw, tw = 330.0, 235.0                                                            # half width at the foot and at the deck
    bd, td = 300.0, 215.0

    def at(z, sx, sy):
        f = max(0.0, z) / ZD
        return (X0 + sx * (bw + (tw - bw) * f), sy * (bd + (td - bd) * f), z)

    for sx in (-1, 1):
        for sy in (-1, 1):
            beam(g, TEAL, (X0 + sx * bw, sy * bd, -FOOT), at(ZD, sx, sy), 62)
    levels = (0.0, 600.0, 1250.0, 1900.0, 2550.0, 3200.0)
    for i, z in enumerate(levels):
        if i:
            for sy in (-1, 1):
                a, b = at(z, -1, sy), at(z, 1, sy)
                bx(g, TEAL, X0, a[1], z - 20, b[0] - a[0] + 50, 40, 40)
            for sx in (-1, 1):
                a, b = at(z, sx, -1), at(z, sx, 1)
                bx(g, TEAL, a[0], 0, z - 20, 40, b[1] - a[1] + 40, 40)
        if i < 5:
            z1 = levels[i + 1]
            for sy in (-1, 1):                                                       # the faces to the road and away from it
                a, b = at(z + 30, -1, sy), at(z1 - 50, 1, sy)
                c, d = at(z + 30, 1, sy), at(z1 - 50, -1, sy)
                if (i + (sy > 0)) % 2 == 0:
                    beam(g, TEAL, a, b, 30)
                else:
                    beam(g, TEAL, c, d, 30)
                if sy < 0 and i in (0, 3, 4):
                    beam(g, TEAL, c if (i + (sy > 0)) % 2 == 0 else a, d if (i + (sy > 0)) % 2 == 0 else b, 30)
            for sx in (-1, 1):                                                       # the end faces
                a, b = at(z + 30, sx, -1), at(z1 - 50, sx, 1)
                c, d = at(z + 30, sx, 1), at(z1 - 50, sx, -1)
                beam(g, TEAL, a, b, 26) if i % 2 == 0 else beam(g, TEAL, c, d, 26)
    # the back stays, their ties to the tower, the lamps along them
    xs = X0 + 1630.0
    for sy in (-1, 1):
        a = (xs, sy * 250, -FOOT)
        b = (X0 + tw - 20, sy * 200, ZD - 60)
        beam(g, TEAL, a, b, 70)
        for z in (1250.0, 2550.0):
            f = (z + FOOT) / (ZD - 60 + FOOT)
            p = (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, z)
            q = at(z, 1, sy)
            beam(g, TEAL, (q[0], q[1], z - 20), (p[0], p[1], z - 20), 30)
        for k in range(5):
            f = 0.14 + 0.18 * k
            p = (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f - 40, a[2] + (b[2] - a[2]) * f)
            if sy < 0:
                bx(g, YEL, p[0], p[1] - 10, p[2] - 20, 34, 34, 44, glow=0.7)
                bx(g, NAVY, p[0], p[1] - 10, p[2] + 24, 44, 44, 8)
    for z in (700.0, 1900.0):
        f = (z + FOOT) / (ZD - 60 + FOOT)
        x = xs + (X0 + tw - 20 - xs) * f
        bx(g, TEAL, x, 0, z, 30, 2 * (250 - 50 * f), 30)
    # the ore bin at the waist, banded, its chute to the road side, the high track out to the dump
    ZBIN = 1250.0
    bx(g, CREAM, X0, -40, ZBIN, 2 * tw + 240, 520, 420)
    for z in (ZBIN + 70, ZBIN + 210, ZBIN + 350):
        bx(g, NAVY, X0, -40, z, 2 * tw + 250, 530, 12)
    g.cyl(CREAM, (X0, -40, ZBIN - 170), 80 * 1.4142, 170, rot=(0, 45, 0), steps=4, top_r=250 * 1.4142)
    beam(g, LILAC, (X0, -260, ZBIN - 20), (X0, -640, ZBIN - 330), 130, 24)
    for sx in (-1, 1):
        beam(g, LILAC, (X0 + sx * 70, -260, ZBIN + 10), (X0 + sx * 70, -640, ZBIN - 300), 10, 60)
    ZTR = ZBIN + 420.0
    xd = X0 - 1000.0
    xa, xb = xd - 60.0, X0 - bw + 20.0
    bx(g, LILAC, (xa + xb) / 2.0, 0, ZTR, xb - xa, 170, 18)
    for sy in (-1, 1):
        bx(g, NAVY, (xa + xb) / 2.0, sy * 46, ZTR + 18, xb - xa, 8, 8)
    for x in (X0 - 560.0, X0 - 880.0):
        for sy in (-1, 1):
            beam(g, TEAL, (x, sy * 190, -FOOT), (x, sy * 66, ZTR), 30)
        bx(g, TEAL, x, 0, ZTR * 0.45, 22, 250, 22)
        xbrace(g, TEAL, (x, -110, ZTR * 0.5), (x, 110, ZTR - 40), 14)
    xbrace(g, TEAL, (X0 - 880, -96, 300), (X0 - 560, -96, ZTR - 60), 16)
    bx(g, YEL, X0 - 720, 0, ZTR + 60, 180, 130, 90)                                  # an ore cart on the high track
    bx(g, NAVY, X0 - 720, 0, ZTR + 36, 140, 100, 24)
    g.sphere(ORE, (X0 - 720, 0, ZTR + 150), 70, scale=(1.2, 0.9, 0.5), steps=(4, 6))
    g.cyl(ORE, (xd - 40, 0, -FOOT), 480, 820 + FOOT, steps=9, top_r=60)             # the dump: a cone of tailings
    g.cyl(CREAM, (xd - 40, 0, 600), 200, 230, steps=9, top_r=50)
    # the lamp room on the tower's face, on brackets
    ZL = 2560.0
    yl = -(bd + (td - bd) * ZL / ZD) - 130
    lamp_room(g, X0, yl, ZL + 40, 320, 240, 270, glow=0.75)
    for sx in (-1, 1):
        beam(g, TEAL, (X0 + sx * 150, yl + 150, ZL - 220), (X0 + sx * 150, yl - 100, ZL + 26), 22)
    # the stair up the road face: a flight a panel, to and fro
    for i in range(4):
        z0, z1 = levels[i] + 10, levels[i + 1] - 20
        ya = -(bd + (td - bd) * z0 / ZD) - 60
        yb2 = -(bd + (td - bd) * z1 / ZD) - 60
        x0, x1 = (X0 - 190, X0 + 190) if i % 2 == 0 else (X0 + 190, X0 - 190)
        if i == 2:
            continue                                                                 # the bin stands in this panel
        beam(g, LILAC, (x0, ya, z0), (x1, yb2, z1), 80, 16)
        beam(g, CREAM, (x0, ya - 40, z0 + 100), (x1, yb2 - 40, z1 + 100), 10)
        bx(g, CREAM, x1, yb2, z1, 110, 100, 14)
    # the deck, railed, and the roof over the wheels
    bx(g, CREAM, X0, 0, ZD, 1180, 2 * td + 190, 26)
    ry = td + 86
    for x in (-560, -280, 0, 280, 560):
        for sy in (-1, 1):
            bx(g, TEAL, X0 + x, sy * ry, ZD + 26, 14, 14, 110)
    for sy in (-1, 1):
        bx(g, TEAL, X0, sy * ry, ZD + 130, 1140, 10, 10)
        bx(g, TEAL, X0, sy * ry, ZD + 78, 1140, 8, 8)
    R = 300.0
    ZW = ZD + 26 + R + 44
    wx = (X0 - 275.0, X0 + 275.0)
    wy = (-70.0, 70.0)
    for k in range(2):
        for dy in (-60, 60):                                                         # each wheel's bearings: an A-frame either side
            beam(g, TEAL, (wx[k] - 170, wy[k] + dy, ZD + 26), (wx[k], wy[k] + dy, ZW), 30)
            beam(g, TEAL, (wx[k] + 170, wy[k] + dy, ZD + 26), (wx[k], wy[k] + dy, ZW), 30)
        w = sc.part(P.motion(swing=170, pivot=(wx[k], wy[k], ZW), axis=90.0, rate=0.07, phase=0.5 * k))
        wheel(w, wx[k], wy[k], ZW, R, n=14, t=40, w=44, spokes=4)
    ZR = ZW + R + 60
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, TEAL, X0 + sx * 600, sy * 150, ZD + 26, 26, 26, ZR - ZD - 26)
    roof_gable(g, LILAC, X0, 0, 1260, 360, ZR, 150, over=40, t=18)
    pennant(sc, X0, 0, ZR + 160, h=330.0, col=CREAM, tip=TEAL, L=420.0, phase=0.3)
    for sx in (-1, 1):
        lantern(g, X0 + sx * 560, -ry - 4, ZD + 110, glow=0.75, s=2.2)
    # the ropes: one down the shaft to the cage, one down the stays to the hoist house
    xc = wx[0] + R                                                                   # the shaft rope leaves the first wheel's inner side
    bx(g, CREAM, xc, wy[0], 1700, 10, 10, ZW - 1700)
    beam(g, CREAM, (wx[1] + R * 0.7, wy[1], ZW + R * 0.7), (xs - 160, 0, 420), 10)
    c = sc.part(P.motion(bob=-170, rate=0.07, phase=0.0))
    c.box(YEL, (xc, wy[0], 2150), (190, 170, 250))
    c.box(NAVY, (xc, wy[0], 2280), (210, 190, 14))
    c.box(NAVY, (xc, wy[0], 2020), (210, 190, 14))
    c.box(NAVY, (xc, wy[0] - 88, 2150), (120, 6, 180))
    c.box(CREAM, (xc, wy[0], 2300), (10, 10, 120))
    # the collar house at the tower's foot and the hoist house at the stays' foot
    bx(g, LILAC, X0, 0, -FOOT, 2 * bw - 90, 2 * bd - 90, 330 + FOOT)
    bx(g, NAVY, X0 - 60, -bd + 40, 0, 150, 12, 230)
    bx(g, CREAM, X0 - 60, -bd + 36, 230, 190, 14, 18)
    win(g, X0 + 150, -bd + 45, 110, 100, 100)
    bx(g, CREAM, X0, -bd + 30, 330, 2 * bw - 60, 20, 20)
    xh = xs - 40.0
    bx(g, TEAL, xh, 0, -FOOT, 480, 620, 400 + FOOT)
    roof_eaves(g, LILAC, xh, 0, 480, 620, 400, 190, over=40, wall=TEAL)
    bx(g, NAVY, xh - 100, -314, 0, 170, 10, 260)
    for x in (xh + 60, xh + 160):
        win(g, x, -310, 150, 70, 100, glow=0.5, col=YEL)
    bx(g, CREAM, xh, -316, 300, 260, 8, 60)
    bars(g, xh, -316, 296, 220)
    g.cyl(NAVY, (xh + 150, 200, 380), 46, 1020, steps=8, top_r=34)
    g.cyl(CREAM, (xh + 150, 200, 1340), 42, 30, steps=8)
    for k, (dz, rr, ph) in enumerate(((120, 70, 0.0), (270, 100, 0.35))):
        f = sc.part(P.motion(bob=-(20 + 12 * k), rate=0.2, phase=ph))
        f.sphere(CREAM, (xh + 150 + 30 * k, 200, 1400 + dz), rr, scale=(1.15, 1, 0.8), steps=(4, 6))


JOBS = [
    ('SM_S1D7_OreMill', ore_mill, SMALL),
    ('SM_S1D7_Tipple', tipple, SMALL),
    ('SM_S1D7_AssayTower', assay_tower, SMALL),
    ('SM_S1D7_PowderHouse', powder_house, STILL),
    ('SM_S1D7_BoardingRim', boarding_rim, SMALL),
    ('SM_S1D7_HoistHouse', hoist_house, SMALL),
    ('SM_S1D7_RimFrame', rim_frame, SMALL),
    ('SM_S1D7_BunkHouse', bunk_house, SMALL),
    ('SM_S1D7_OreCart', ore_cart, STILL),
    ('SM_S1D7_AditPortal', adit_portal, SMALL),
    ('SM_S1D7_StampBattery', stamp_battery, SMALL),
    ('SM_S1D7_Flume', flume, SMALL),
    ('SM_S1D7_SluiceBox', sluice_box, LOWMOVE),
    ('SM_S1D7_TramTower', tram_tower, SMALL),
    ('SM_S1D7_Windlass', windlass, SMALL),
    ('SM_S1D7_BellFrame', bell_frame, SMALL),
    ('SM_S1D7_WashLine', wash_line, SMALL),
    ('SM_S1D7_TwinHeadframe', twin_headframe, dict(grow=(60, 60, 60), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
