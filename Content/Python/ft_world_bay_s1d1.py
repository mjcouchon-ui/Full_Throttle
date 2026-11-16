"""Bayshore, Stage 1, builder s1d1: district 1 "The Strip" (0 to 3.2 km). /Game/World/Bayshore/SM_S1D1_*.
Streamline deco, 2 to 3 storeys: flat roofs with stepped parapets, rounded ends, eyebrow ledges over ribbon windows,
fins and pylons through the roof line, everything striped horizontally. Cream walls, hot pink bands and fins, yellow
crowns, teal roofs and ledges, navy glass. The hero is THE SUNBURST, a bandshell under a half-sun of rays.
Placed by the block C:/ftwork/s1/L0_s1d1.inl. Run: import ft_world_bay_s1d1 as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/Bayshore'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))   # a piece with no moving part: true bounds
SMALL = dict(grow=(20, 20, 20), grow_down=(20, 20, 0))   # a piece whose moving parts stay close to it
MOVE = dict(grow=(60, 60, 40), grow_down=(60, 60, 0))    # a building with pennants, washing, a rocking sign

CREAM, HOT, YEL, TEAL, NAVY = '#fff1d6', '#ff4fb8', '#ffe14d', '#2fc6c0', '#2b2350'     # the district's five
WHITE, SKY, STEEL, LEAF, WOOD, PINK = '#fffaf2', '#8fe3ff', '#c9c4d6', '#2fbf71', '#c9a06a', '#ff9ec7'
FOOT = 300.0                                    # how far a building's footing goes down (the Strip is hills)


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def side(g, col, pts, y0, y1, glow=0.0):
    """A profile [(x, z)] seen from the road, from y0 to y1 deep."""
    g.side(col, ccw(pts), y0, y1, glow=glow)


def disc_y(g, col, x, y, z, r, t, steps=12, glow=0.0):
    """A disc facing the road: its front face at y, t thick away from the road."""
    g.cyl(col, (x, y, z), r, t, rot=(0, 0, 90), steps=steps, glow=glow)


def disc_x(g, col, x, y, z, r, t, steps=12, glow=0.0):
    """A disc facing along the road, centred on x, t thick."""
    g.cyl(col, (x + t / 2.0, y, z), r, t, rot=(90, 0, 0), steps=steps, glow=glow)


def arch(g, col, x, zc, r0, r1, y0, y1, n=10, a0=0.0, a1=180.0, glow=0.0):
    """A flat arch (a piece of a ring) facing the road, centre (x, zc), from radius r0 to r1 (r0 = 0: a fan)."""
    an = [math.radians(a0 + (a1 - a0) * k / float(n)) for k in range(n + 1)]
    pts = [(x + r1 * math.cos(a), zc + r1 * math.sin(a)) for a in an]
    if r0 > 0.0:
        pts += [(x + r0 * math.cos(a), zc + r0 * math.sin(a)) for a in reversed(an)]
    elif a1 - a0 < 179.0:
        pts.append((x, zc))
    side(g, col, pts, y0, y1, glow)


def body(g, x, y, w, d, h, col=CREAM, foot=FOOT):
    g.box(col, (x, y, (h - foot) / 2.0), (w, d, h + foot))


def band(g, col, x, y, w, d, z, hh=24.0, proud=8.0):
    """A horizontal stripe right round a box body."""
    g.box(col, (x, y, z), (w + 2 * proud, d + 2 * proud, hh))


def ribbon(g, x0, x1, y, z, hh, n=0, ledge=TEAL, glass=NAVY):
    """A ribbon window on a wall at y that faces the road, its eyebrow ledge over it, n white mullions."""
    w, xc = x1 - x0, (x0 + x1) / 2.0
    g.box(glass, (xc, y - 4, z), (w, 12, hh))
    g.box(ledge, (xc, y - 26, z + hh / 2.0 + 10), (w + 50, 64, 14))
    g.box(WHITE, (xc, y - 8, z - hh / 2.0 - 5), (w + 20, 20, 10))
    for k in range(n):
        g.box(WHITE, (x0 + (k + 1) * w / (n + 1.0), y - 9, z), (10, 8, hh))


def awning(g, x0, x1, y, z, cols, depth=150.0, drop=55.0, n=None):
    """A striped awning hung on the wall at y, falling towards the road."""
    n = n or max(2, int(round((x1 - x0) / 110.0)))
    w = (x1 - x0) / float(n)
    roll = -math.degrees(math.atan2(drop, depth))
    for k in range(n):
        g.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y - depth / 2.0, z - drop / 2.0), (w, math.hypot(depth, drop), 10), rot=(0, 0, roll))


def porthole(g, x, y, z, r=45.0):
    disc_y(g, WHITE, x, y - 10, z, r + 12, 10, steps=8)
    disc_y(g, NAVY, x, y - 15, z, r, 6, steps=8)


def stepped(g, col, x, y, z, w, d, n, dw, hh):
    """A stepped parapet or crown: n slabs, each narrower than the one under it."""
    for k in range(n):
        g.box(col, (x, y, z + hh * (k + 0.5)), (w - k * dw, d, hh))
    return z + n * hh


def door(g, x, y, w=110.0, h=220.0, col=NAVY, trim=HOT, z=0.0):
    g.box(trim, (x, y - 3, z + h / 2.0 + 6), (w + 26, 8, h + 12))
    g.box(col, (x, y - 6, z + h / 2.0), (w, 8, h))
    g.box(YEL, (x + w * 0.3, y - 11, z + h * 0.48), (8, 5, 26))


def stool(g, x, y, col, z=0.0):
    g.cyl(STEEL, (x, y, z - 30), 7, 92, steps=5)
    g.cyl(col, (x, y, z + 62), 22, 10, steps=7)


def vent(g, x, y, z):
    g.box(STEEL, (x, y, z + 25), (70, 70, 50))
    g.box(TEAL, (x, y, z + 56), (88, 88, 12))


def cowl(sc, x, y, z, phase=0.0):
    """A roof vent whose cowl turns to and fro in the wind."""
    g = sc.still()
    g.cyl(STEEL, (x, y, z), 18, 60, steps=6)
    c = sc.part(P.motion(swing=55, pivot=(x, y, z + 60), axis=P.UP, rate=0.12, phase=phase))
    c.box(STEEL, (x + 12, y, z + 76), (70, 40, 34))
    c.box(HOT, (x - 30, y, z + 92), (34, 5, 30))


def pennant(sc, x, y, z, col, w=110.0, h=50.0, phase=0.0, rate=0.4, swing=16.0, mast=0.0):
    """A pennant that swings about its mast (mast: cm of mast drawn below it)."""
    if mast:
        sc.still().cyl(STEEL, (x, y, z - mast), 5, mast + h / 2.0 + 6, steps=5)
        sc.still().sphere(YEL, (x, y, z + h / 2.0 + 14), 9, steps=(3, 5))
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + w / 2.0 + 5, y, z), (w, 5, h))


def globe(g, x, y, z, r=40.0, glow=0.6):
    g.sphere(YEL, (x, y, z), r, steps=(4, 7), glow=glow)


def bowl(g, x, y, z, r=48.0, flowers=(HOT, YEL, WHITE)):
    """A deco planter bowl on a stem: a clipped mound and a few flowers."""
    g.cyl(CREAM, (x, y, z), r * 0.42, 12, steps=7)
    g.cyl(CREAM, (x, y, z + 12), r * 0.2, 22, steps=6)
    g.cyl(CREAM, (x, y, z + 34), r * 0.5, 30, steps=8, top_r=r)
    g.sphere(LEAF, (x, y, z + 70), r * 0.86, scale=(1, 1, 0.6), steps=(3, 7))
    for k, c in enumerate(flowers):
        a = 2.1 * k + 0.4
        g.sphere(c, (x + math.cos(a) * r * 0.45, y + math.sin(a) * r * 0.45, z + 92), r * 0.2, steps=(3, 5))


def fin(g, col, x, y0, y1, z0, z1, t=50.0, discs=0, disc_col=YEL, glow=0.0):
    """A fin through the roof line: thin across the road's way, broad to whoever drives up to it; a stepped top and
    round bosses on both faces."""
    yc, d = (y0 + y1) / 2.0, y1 - y0
    g.box(col, (x, yc, (z0 + z1) / 2.0), (t, d, z1 - z0))
    g.box(col, (x, yc - d * 0.12, z1 + 30), (t, d * 0.62, 60))
    g.box(col, (x, yc - d * 0.2, z1 + 80), (t, d * 0.3, 40))
    for k in range(discs):
        zz = z1 - 90 - k * (d * 0.5 + 30)
        disc_x(g, disc_col, x, yc, zz, d * 0.24, t + 24, steps=10, glow=glow)


def parasol(g, x, y, z, col, r=130.0, h=230.0):
    g.cyl(WHITE, (x, y, z - 30), 6, h + 30, steps=5)
    g.cyl(col, (x, y, z + h - 22), r, 44, steps=10, top_r=8)
    g.cyl(STEEL, (x, y, z + 66), 44, 8, steps=8)


def rail(g, x0, x1, y, z, n=5):
    """A ship's rail along a roof's edge."""
    w, xc = x1 - x0, (x0 + x1) / 2.0
    g.box(WHITE, (xc, y, z + 72), (w, 6, 8))
    g.box(WHITE, (xc, y, z + 40), (w, 6, 6))
    for k in range(n + 1):
        g.box(WHITE, (x0 + k * w / float(n), y, z + 36), (6, 6, 76))


def lamp(g, x, y, z, glow=0.6):
    """A wall lamp on a wall at y that faces the road."""
    g.box(TEAL, (x, y - 6, z), (14, 12, 36))
    g.sphere(YEL, (x, y - 19, z + 4), 11, steps=(3, 5), glow=glow)


def speed(g, x0, x1, y, z, col=TEAL, n=3):
    """Speed lines on a wall at y: n stripes, each shorter than the one under it."""
    w = x1 - x0
    for k in range(n):
        g.box(col, (x0 + w * (1.0 - 0.2 * k) / 2.0, y - 4, z + k * 28), (w * (1.0 - 0.2 * k), 8, 10))


def tub(g, x, y, z=0.0, r=34.0):
    g.box(CREAM, (x, y, z - 30), (r * 1.3, r * 1.3, 60))
    bowl(g, x, y, z, r)


# ------------------------------------------------------------------------------------------ the buildings
def diner(sc):
    """The drive-in diner: a round dining room, a kitchen wing with a hatch and stools, a round canopy on one mushroom
    column, a hot pink fin through the roof, a giant cup steaming on the wing. 13.3 x 8.8 m, 11 m to the fin's top."""
    g = sc.still()
    lamp(g, -120 - 100, 40 - 392, 250)
    lamp(g, -120 + 100, 40 - 392, 250)
    rail(g, 150, 630, 400, 376)
    speed(g, 500, 650, -200, 230, HOT)
    tub(g, 80, -330)
    tub(g, 650, -300)
    dx, dy = -120.0, 40.0
    g.cyl(CREAM, (dx, dy, -FOOT), 400, FOOT + 400, steps=14)
    g.cyl(HOT, (dx, dy, 48), 406, 26, steps=14)
    g.cyl(NAVY, (dx, dy, 150), 406, 130, steps=14)                 # the ribbon window, all the way round
    g.cyl(TEAL, (dx, dy, 290), 462, 16, steps=14)                  # its eyebrow
    for k in range(7):                                             # mullions on the road side
        a = math.radians(200 + k * 23.3)
        g.box(WHITE, (dx + math.cos(a) * 407, dy + math.sin(a) * 407, 215), (12, 12, 130), rot=(0, math.degrees(a), 0))
    g.cyl(TEAL, (dx, dy, 400), 426, 26, steps=14)
    g.cyl(CREAM, (dx, dy, 426), 300, 50, steps=12)                 # the stepped crown
    g.cyl(HOT, (dx, dy, 476), 210, 44, steps=12)
    g.cyl(YEL, (dx, dy, 520), 120, 40, steps=10)
    door(g, dx, dy - 402, 120, 226)
    fin(g, HOT, dx, dy - 150, dy + 150, 400, 960, discs=3, glow=0.4)
    # the kitchen wing
    wx, wy = 390.0, 110.0
    body(g, wx, wy, 540, 620, 360)
    band(g, HOT, wx, wy, 540, 620, 60, 26)
    band(g, TEAL, wx, wy, 540, 620, 368, 16, 30)
    stepped(g, CREAM, wx, wy - 300, 376, 420, 30, 3, 110, 32)
    g.box(NAVY, (wx, wy - 314, 195), (300, 10, 130))               # the hatch
    g.box(WHITE, (wx, wy - 340, 122), (340, 60, 12))
    awning(g, wx - 170, wx + 170, wy - 310, 330, [HOT, CREAM], n=5)
    for k in range(4):
        stool(g, wx - 135 + k * 90, wy - 420, HOT if k % 2 else TEAL)
    g.box(WHITE, (wx + 225, wy - 316, 200), (70, 8, 150))          # the menu: three bars, no words
    for k, c in enumerate((HOT, TEAL, YEL)):
        g.box(c, (wx + 225, wy - 322, 245 - k * 44), (46, 5, 22))
    porthole(g, wx + 225, wy - 310, 310, 26)
    # the canopy on its mushroom column
    cx, cy = -470.0, -270.0
    g.cyl(CREAM, (cx, cy, -150), 30, 450, steps=8, top_r=80)
    g.cyl(TEAL, (cx, cy, 300), 190, 30, steps=14)
    g.cyl(HOT, (cx, cy, 330), 150, 20, steps=12)
    g.cyl(YEL, (cx, cy, 350), 80, 20, steps=10)
    g.cyl(YEL, (cx, cy, 286), 150, 14, steps=12, glow=0.35)        # the lit soffit
    # the cup on the wing's roof, and its steam
    ux, uy = wx + 40, wy + 60
    g.cyl(TEAL, (ux, uy, 384), 150, 12, steps=10)
    g.cyl(CREAM, (ux, uy, 396), 86, 140, steps=10, top_r=118)
    g.cyl(HOT, (ux, uy, 450), 104, 22, steps=10)
    g.box(CREAM, (ux + 140, uy, 470), (60, 26, 70))
    for k in range(3):
        s = sc.part(P.motion(bob=-12, rate=0.22, phase=k / 3.0))
        s.sphere(WHITE, (ux - 30 + k * 30, uy, 590 + k * 62), 34 - k * 6, steps=(3, 6))
    vent(g, wx - 180, wy + 220, 384)
    cowl(sc, wx + 200, wy + 230, 384)


def record_shop(sc):
    """The record shop: a rounded end, a shop window under an awning, and a giant yellow disc stood on the roof with
    three notes drifting over it. 13 x 8 m, 15.4 m to the disc's top."""
    g = sc.still()
    lamp(g, -120, -400, 262)
    lamp(g, 70, -400, 262)
    rail(g, -180, 620, 380, 716, n=6)
    speed(g, 560, 640, -400, 420, HOT)
    tub(g, -220, -450)
    tub(g, 620, -450)
    ox = 80.0
    body(g, ox + 120, 0, 900, 800, 700)
    g.cyl(CREAM, (ox - 330, 0, -FOOT), 400, 700 + FOOT, steps=14)
    for z, c, hh in ((40, HOT, 30), (345, TEAL, 22), (664, HOT, 28)):
        band(g, c, ox + 120, 0, 900, 800, z, hh)
        g.cyl(c, (ox - 330, 0, z - hh / 2.0), 408, hh, steps=14)
    g.box(TEAL, (ox + 120, 0, 708), (930, 830, 16))
    g.cyl(TEAL, (ox - 330, 0, 700), 416, 16, steps=14)
    # ground floor: the window, the door, the awning
    g.box(NAVY, (ox + 250, -404, 180), (460, 12, 200))
    for k in range(3):                                             # sleeves in the window
        g.box((YEL, HOT, TEAL)[k], (ox + 110 + k * 140, -412, 170), (90, 6, 90))
        disc_y(g, NAVY, ox + 132 + k * 140, -417, 170, 34, 5, steps=8)
    awning(g, ox - 10, ox + 510, -400, 330, [HOT, CREAM], n=6)
    door(g, ox - 110, -400, 120, 230)
    ribbon(g, ox - 150, ox + 520, -400, 510, 110, n=5)
    for k in range(4):                                             # the upper windows run on round the end
        a = math.radians(195 + k * 22)
        g.box(NAVY, (ox - 330 + math.cos(a) * 402, math.sin(a) * 402, 510), (12, 110, 110), rot=(0, math.degrees(a), 0))
    # the disc on the roof
    rx, rz = ox + 130, 716 + 60 + 380
    g.box(CREAM, (rx, 60, 746), (500, 200, 60))
    g.box(TEAL, (rx, 60, 782), (360, 140, 14))
    g.box(STEEL, (rx, 80, rz - 120), (40, 40, 420))
    for r, c, pr in ((380, YEL, 0), (310, NAVY, 6), (292, YEL, 12), (222, NAVY, 18), (204, YEL, 24), (128, HOT, 30), (26, CREAM, 36)):
        disc_y(g, c, rx, 30 - pr, rz, r, 40, steps=16 if r > 200 else 10)
    for k in range(3):                                             # notes, drifting
        nx, nz = rx - 520 + k * 150, 1180 + k * 120
        n = sc.part(P.motion(bob=-14, rate=0.3, phase=k * 0.31))
        c = (HOT, TEAL, HOT)[k]
        n.sphere(c, (nx, 0, nz), 28, scale=(1.2, 1, 0.85), steps=(3, 6))
        n.box(c, (nx + 26, 0, nz + 56), (8, 8, 110))
        n.box(c, (nx + 46, 0, nz + 100), (40, 8, 24))
    fin(g, HOT, ox + 500, -330, -90, 560, 1040, discs=2, glow=0.4)
    vent(g, ox + 420, 250, 716)
    cowl(sc, ox - 330, 120, 716, phase=0.4)
    pennant(sc, ox - 330, -100, 900, YEL, mast=190, phase=0.2)


def picture_house(sc):
    """The picture house: a stepped fin 18 m tall, a blank marquee with a row of bulbs, a ticket booth, poster frames
    with shapes in them. 13 x 9.4 m."""
    g = sc.still()
    for x in (-345, -115, 115, 345):
        lamp(g, x, -350, 270)
    rail(g, -620, 620, 440, 836, n=8)
    rail(g, -620, -470, -320, 836, n=2)
    rail(g, 470, 620, -320, 836, n=2)
    speed(g, -640, -380, -350, 600, HOT)
    speed(g, 380, 640, -350, 600, HOT)
    tub(g, -620, -410)
    tub(g, 620, -410)
    body(g, 0, 60, 1300, 820, 820)
    for z, c in ((40, HOT), (560, TEAL), (640, TEAL), (720, TEAL)):
        band(g, c, 0, 60, 1300, 820, z, 22)
    g.box(TEAL, (0, 60, 828), (1330, 850, 16))
    stepped(g, CREAM, 0, -330, 836, 900, 40, 3, 240, 46)
    stepped(g, YEL, 0, -330, 974, 180, 40, 2, 80, 30)
    # the marquee: a blank board, bars for the programme, bulbs under its edge
    g.box(CREAM, (0, -440, 400), (760, 300, 120))
    g.box(HOT, (0, -440, 468), (790, 330, 18))
    g.box(HOT, (0, -440, 334), (790, 330, 14))
    g.box(WHITE, (0, -594, 400), (640, 8, 84))
    for k in range(3):
        g.box((TEAL, HOT, TEAL)[k], (-150 + k * 150, -600, 400), (110, 5, 20 + 12 * (k % 2)))
    for k in range(9):
        g.box(YEL, (-320 + k * 80, -590, 318), (22, 22, 18), glow=0.6)
    for k in range(3):
        door(g, -230 + k * 230, -352, 130, 236)
    g.cyl(CREAM, (0, -470, -60), 70, 250, steps=8)                 # the ticket booth
    g.cyl(NAVY, (0, -470, 110), 73, 56, steps=8)
    g.cyl(TEAL, (0, -470, 190), 92, 14, steps=8)
    g.cyl(HOT, (0, -470, 204), 50, 22, steps=8, top_r=14)
    for s in (-1, 1):                                              # poster frames: a sun and waves, no faces, no words
        px = s * 520
        g.box(YEL, (px, -354, 230), (170, 8, 250))
        g.box(NAVY if s < 0 else TEAL, (px, -360, 230), (140, 6, 220))
        disc_y(g, HOT if s < 0 else YEL, px, -367, 270, 42, 5, steps=8)
        g.box(WHITE, (px, -366, 170), (110, 5, 16))
        g.box(WHITE, (px, -366, 140), (80, 5, 12))
        porthole(g, s * 430, -350, 480, 48)
        porthole(g, s * 560, -350, 480, 30)
    # the fin: stepped, a line of bulbs up its edge
    g.box(HOT, (0, -230, 1150), (60, 440, 1300))                   # z 500 .. 1800
    g.box(CREAM, (0, -230, 1500), (76, 300, 24))
    g.box(CREAM, (0, -230, 1300), (76, 300, 24))
    g.box(CREAM, (0, -230, 1100), (76, 300, 24))
    g.box(HOT, (0, -290, 1540), (60, 250, 620))                    # steps on the road side
    g.box(HOT, (0, -360, 1380), (60, 150, 420))
    g.box(YEL, (0, -230, 1830), (60, 260, 60))
    g.box(YEL, (0, -230, 1880), (60, 120, 40))
    for k in range(8):
        g.box(YEL, (0, -456, 640 + k * 130), (74, 20, 34), glow=0.6)
    vent(g, 420, 300, 836)
    vent(g, -380, 280, 836)
    cowl(sc, 250, 330, 836, phase=0.7)
    pennant(sc, 0, -230, 2010, TEAL, mast=110, w=140, h=60)


def bowling(sc):
    """The bowling alley: a long low box with a row of round windows, a stepped entrance, and a giant pin rocking on
    a pylon beside a teal ball. 22 x 8.6 m, 15.6 m to the pin's top."""
    g = sc.still()
    lamp(g, -140, -392, 280)
    lamp(g, 140, -392, 280)
    rail(g, -1080, -260, -300, 536, n=4)
    rail(g, 260, 1080, -300, 536, n=4)
    rail(g, -1080, 1080, 420, 536, n=8)
    speed(g, -1090, -940, -320, 200, TEAL)
    speed(g, 940, 1090, -320, 200, TEAL)
    tub(g, -300, -400)
    body(g, 0, 60, 2200, 760, 520)
    for z, c, hh in ((40, HOT, 30), (150, HOT, 14), (430, TEAL, 20), (490, HOT, 24)):
        band(g, c, 0, 60, 2200, 760, z, hh)
    g.box(TEAL, (0, 60, 528), (2230, 790, 16))
    for k in range(7):
        if k != 3:
            porthole(g, -840 + k * 280, -320, 300, 62)
    # the entrance: a stepped front and a round canopy
    g.box(CREAM, (0, -350, 320), (420, 80, 640))
    stepped(g, CREAM, 0, -350, 640, 420, 80, 3, 110, 44)
    stepped(g, YEL, 0, -350, 772, 90, 80, 2, 40, 30)
    door(g, 0, -392, 180, 240)
    arch(g, TEAL, 0, 290, 0, 250, -520, -390, n=8)
    arch(g, HOT, 0, 304, 0, 200, -500, -390, n=8)
    for s in (-1, 1):
        g.cyl(CREAM, (s * 215, -500, -60), 14, 350, steps=6)
    ribbon(g, -160, 160, -392, 470, 90, n=3)
    # the pin on its pylon, and the ball
    px, py = -900.0, -40.0
    g.box(CREAM, (px, py, 560), (150, 150, 1000))                  # z 60 .. 1060
    for k in range(4):
        g.box(HOT if k % 2 else TEAL, (px, py, 640 + k * 110), (166, 166, 26))
    g.box(TEAL, (px, py, 1068), (220, 220, 16))
    p = sc.part(P.motion(swing=5, pivot=(px, py, 1076), axis=0.0, rate=0.25))
    p.cyl(WHITE, (px, py, 1076), 62, 130, steps=10, top_r=98)
    p.cyl(WHITE, (px, py, 1206), 98, 130, steps=10, top_r=66)
    p.cyl(WHITE, (px, py, 1336), 66, 90, steps=10, top_r=36)
    p.cyl(HOT, (px, py, 1396), 46, 16, steps=10)
    p.cyl(HOT, (px, py, 1426), 40, 14, steps=10)
    p.cyl(WHITE, (px, py, 1426), 36, 70, steps=10, top_r=50)
    p.sphere(WHITE, (px, py, 1506), 52, steps=(4, 8))
    bx = 780.0
    g.cyl(CREAM, (bx, 0, 536), 120, 40, steps=10)
    g.sphere(TEAL, (bx, 0, 700), 135, steps=(6, 10))
    for k in range(3):
        disc_y(g, NAVY, bx - 30 + k * 34, -128 + abs(k - 1) * 4, 740 + (k % 2) * 34, 15, 8, steps=6)
    for k in range(3):                                             # three small pins set up on the roof
        qx = 300 + k * 90
        g.cyl(WHITE, (qx, -120 + (k % 2) * 90, 536), 20, 70, steps=6, top_r=30)
        g.cyl(WHITE, (qx, -120 + (k % 2) * 90, 606), 30, 70, steps=6, top_r=12)
        g.sphere(WHITE, (qx, -120 + (k % 2) * 90, 690), 18, steps=(3, 5))
    vent(g, -300, 300, 536)
    vent(g, 60, 320, 536)
    cowl(sc, -560, 300, 536, phase=0.2)


def parlour(sc):
    """The ice-cream parlour: a drum under a wide eyebrow, a striped drum roof, two giant ice lollies leaning on the
    crown, parasol tables and stools. 10.4 m across, 9.6 m tall. For the beach."""
    g = sc.still()
    lamp(g, -95, -398, 250)
    lamp(g, 95, -398, 250)
    tub(g, -240, -440)
    tub(g, 180, -470)
    g.cyl(CREAM, (0, 0, -FOOT), 400, FOOT + 340, steps=14)
    g.cyl(HOT, (0, 0, 40), 406, 28, steps=14)
    g.cyl(TEAL, (0, 0, 104), 405, 12, steps=14)
    g.cyl(NAVY, (0, 0, 150), 406, 120, steps=14)
    for k in range(6):
        a = math.radians(205 + k * 26)
        g.box(WHITE, (math.cos(a) * 407, math.sin(a) * 407, 210), (12, 12, 120), rot=(0, math.degrees(a), 0))
    g.cyl(TEAL, (0, 0, 290), 520, 18, steps=16)                    # the eyebrow canopy
    g.cyl(HOT, (0, 0, 276), 470, 14, steps=16)
    g.cyl(YEL, (0, 0, 268), 410, 8, steps=14, glow=0.3)
    z = 340.0
    for k, (r, c) in enumerate(((380, HOT), (330, CREAM), (280, HOT), (230, CREAM))):   # the striped drum roof
        g.cyl(c, (0, 0, z), r, 62, steps=14)
        z += 62
    g.sphere(YEL, (0, 0, z), 200, scale=(1, 1, 0.5), steps=(4, 10))
    g.cyl(STEEL, (0, 0, z + 80), 6, 110, steps=5)
    door(g, 0, -402, 120, 226)
    g.box(WHITE, (-200, -372, 140), (150, 60, 12))                 # the hatch's shelf
    for s, (c1, c2, c3) in ((-1, (HOT, CREAM, YEL)), (1, (TEAL, YEL, HOT))):   # the lollies
        lx = s * 190.0
        L = sc.part(P.motion(swing=3, pivot=(lx, -60, 600), axis=90.0, rate=0.2, phase=0.25 + 0.5 * s))
        L.box(WOOD, (lx, -60, 660), (34, 20, 150))
        L.box(c1, (lx, -60, 800), (150, 64, 130))
        L.box(c2, (lx, -60, 905), (150, 64, 80))
        L.box(c3, (lx, -60, 990), (150, 64, 90))
        L.cyl(c3, (lx, -92, 1035), 75, 64, rot=(0, 0, 90), steps=10)
    for s in (-1, 1):                                              # tables under parasols, a stool each side
        tx = s * 470
        parasol(g, tx, -300, 0, YEL if s < 0 else HOT)
        stool(g, tx - 60, -330, TEAL)
        stool(g, tx + 60, -270, HOT)
    g.box(WHITE, (300, -330, 110), (70, 8, 150))                   # a board with three lollies painted on it
    for k, c in enumerate((HOT, YEL, TEAL)):
        g.box(c, (282 + k * 18, -336, 130 - (k % 2) * 30), (12, 5, 40))
    pennant(sc, 0, 0, z + 170, HOT, w=120, h=50, phase=0.6)


def milk_bar(sc):
    """The milk bar: two storeys of ribbon windows and a round glass-block corner tower with a stepped cap and a
    ringed spire, a terrace with stools under an awning, three yellow crowns on the parapet. 12.6 x 8.6 m, 14.6 m."""
    g = sc.still()
    lamp(g, -205, -320, 262)
    lamp(g, -35, -320, 262)
    rail(g, -430, 620, 430, 696, n=7)
    speed(g, 470, 625, -320, 560, HOT)
    tub(g, -180, -470)
    tub(g, 610, -470)
    body(g, 90, 60, 1080, 760, 680)
    for z, c, hh in ((40, HOT, 30), (340, TEAL, 20), (646, HOT, 26)):
        band(g, c, 90, 60, 1080, 760, z, hh)
    g.box(TEAL, (90, 60, 688), (1110, 790, 16))
    # the tower: rings of glass block
    tx, ty = -450.0, -230.0
    g.cyl(CREAM, (tx, ty, -FOOT), 236, FOOT + 60, steps=12)
    z = 60.0
    for k in range(8):
        g.cyl(SKY if k % 2 == 0 else WHITE, (tx, ty, z), 230 if k % 2 == 0 else 238, 96 if k % 2 == 0 else 28, steps=12, glow=0.25 if k % 2 == 0 else 0.0)
        z += 96 if k % 2 == 0 else 28
    g.cyl(SKY, (tx, ty, z), 230, 300, steps=12, glow=0.25)
    z += 300
    for r, c, hh in ((262, TEAL, 22), (200, CREAM, 50), (150, HOT, 44), (96, YEL, 40)):
        g.cyl(c, (tx, ty, z), r, hh, steps=12)
        z += hh
    g.cyl(STEEL, (tx, ty, z), 9, 330, steps=5)
    for k in range(3):
        g.cyl(YEL, (tx, ty, z + 70 + k * 80), 44 - k * 10, 12, steps=8, glow=0.4)
    g.sphere(HOT, (tx, ty, z + 340), 18, steps=(3, 6))
    # the front
    ribbon(g, -190, 600, -320, 500, 110, n=5)
    g.box(NAVY, (240, -324, 190), (520, 12, 190))
    for k in range(3):
        g.box(WHITE, (70 + k * 170, -330, 190), (10, 8, 190))
    awning(g, -40, 600, -320, 330, [HOT, CREAM], n=7)
    door(g, -120, -320, 110, 230)
    g.box(CREAM, (250, -430, 52), (560, 60, 104))                  # the bar on the terrace
    g.box(TEAL, (250, -430, 110), (590, 86, 12))
    for k in range(5):
        stool(g, 30 + k * 110, -500, (HOT, YEL, TEAL)[k % 3])
    for k in range(3):                                             # the crowns
        cx = 150 + k * 200
        stepped(g, YEL, cx, -300, 696, 130, 40, 3, 40, 26)
    stepped(g, CREAM, 350, -300, 696, 660, 30, 1, 0, 20)
    g.box(WHITE, (480, -350, 250), (10, 60, 10))                   # a hanging board, blank but for a glass and a straw
    g.box(WHITE, (480, -390, 210), (8, 90, 100))
    for sx in (-1, 1):
        g.box(HOT, (480 + sx * 6, -390, 200), (5, 36, 54))
        g.box(TEAL, (480 + sx * 6, -378, 240), (5, 8, 50))
    vent(g, 420, 300, 696)
    cowl(sc, 120, 320, 696, phase=0.5)
    # the washing behind the parapet: cloths on a line, stirring
    for s in (-1, 1):
        g.cyl(STEEL, (290 + s * 250, 180, 696), 6, 150, steps=5)
    g.box(STEEL, (290, 180, 840), (500, 5, 5))
    for k in range(4):
        w = sc.part(P.motion(swing=10, pivot=(0, 180, 838), axis=0.0, rate=0.35, phase=k * 0.23))
        w.box((WHITE, HOT, TEAL, YEL)[k], (110 + k * 120, 180, 792), (90, 5, 90))


def dance_hall(sc):
    """The dance hall: a hot pink arch of light over the doors, a yellow arch round it, two ringed pylons, a small
    sunrise of rays on the stepped parapet, pennants. 13 x 9 m, 13 m."""
    g = sc.still()
    for x in (-560, 560):
        lamp(g, x, -420, 250)
    rail(g, -640, -400, -320, 776, n=3)
    rail(g, 400, 640, -320, 776, n=3)
    rail(g, -640, 640, 440, 776, n=8)
    speed(g, -640, -500, -330, 520, HOT)
    speed(g, 500, 640, -330, 520, HOT)
    tub(g, -420, -470)
    tub(g, 420, -470)
    body(g, 0, 60, 1300, 780, 760)
    for z, c, hh in ((40, HOT, 30), (610, TEAL, 20), (680, TEAL, 20), (730, HOT, 22)):
        band(g, c, 0, 60, 1300, 780, z, hh)
    g.box(TEAL, (0, 60, 768), (1330, 810, 16))
    top = stepped(g, CREAM, 0, -310, 776, 760, 40, 3, 200, 44)
    for k in range(5):                                             # the sunrise on the parapet
        a = math.radians(30 + k * 30)
        tilt_box(g, YEL if k % 2 == 0 else HOT, (math.cos(a) * 60, -310, top + math.sin(a) * 60), (math.cos(a) * 250, -310, top + math.sin(a) * 250), 30, 36)
    arch(g, YEL, 0, top, 0, 70, -332, -288, n=6)
    # the arches of light over the doors
    g.box(NAVY, (0, -334, 200), (480, 10, 400))
    arch(g, NAVY, 0, 400, 0, 240, -339, -329, n=8)
    arch(g, HOT, 0, 400, 250, 310, -380, -330, n=10, glow=0.5)
    arch(g, YEL, 0, 400, 330, 372, -362, -330, n=10, glow=0.35)
    for s in (-1, 1):
        g.box(HOT, (s * 280, -355, 200), (60, 50, 400), glow=0.5)
        g.box(YEL, (s * 351, -346, 200), (42, 32, 400), glow=0.35)
        door(g, s * 100, -338, 150, 250, trim=TEAL)
        # the pylons
        px = s * 560
        g.box(CREAM, (px, -360, 400), (120, 120, 1100))            # z -150 .. 950
        for k in range(5):
            g.box(TEAL if k % 2 == 0 else HOT, (px, -360, 420 + k * 110), (136, 136, 22))
        stepped(g, CREAM, px, -360, 950, 90, 90, 2, 30, 40)
        globe(g, px, -360, 1066, 40, 0.6)
        ribbon(g, s * 440 - 50, s * 440 + 50, -330, 300, 200, n=1)
        pennant(sc, s * 300, 200, 960, TEAL if s < 0 else YEL, mast=190, phase=0.3 + 0.2 * s)
    g.box(CREAM, (0, -440, 12), (700, 160, 24))                    # the step, a carpet on it
    g.box(HOT, (0, -440, 26), (300, 150, 6))
    vent(g, -420, 300, 776)
    vent(g, 380, 280, 776)
    cowl(sc, 0, 320, 776, phase=0.1)


def flats(sc):
    """A streamline apartment house, three storeys: round-ended balconies, ribbon windows, a glass-block stair tower
    with a fin, washing on the roof and a weather vane. 11.6 x 8.6 m, 16.8 m to the vane."""
    g = sc.still()
    lamp(g, 120, -320, 262)
    lamp(g, 280, -320, 262)
    rail(g, -520, 400, 430, 1066, n=6)
    tub(g, 110, -400)
    tub(g, 300, -400)
    body(g, -60, 60, 940, 760, 1050)
    for k in range(3):
        z0 = 20 + k * 345
        band(g, HOT, -60, 60, 940, 760, z0 + 20, 26)
        ribbon(g, -480, 300, -320, z0 + 215, 110, n=4)
        g.box(TEAL, (-150, -395, z0 + 12 + (0 if k else 14)), (560, 150, 14))           # the balcony, round at one end
        g.cyl(TEAL, (-430, -395, z0 + 5 + (0 if k else 14)), 75, 14, steps=8)
        g.box(WHITE, (-150, -466, z0 + 80), (560, 8, 12))
        g.box(WHITE, (-150, -466, z0 + 50), (560, 8, 8))
        for j in range(4):
            g.box(WHITE, (-420 + j * 180, -466, z0 + 50), (8, 8, 76))
        porthole(g, 340, -320, z0 + 215, 34)
    band(g, TEAL, -60, 60, 940, 760, 1040, 22, 20)
    g.box(TEAL, (-60, 60, 1058), (970, 790, 16))
    stepped(g, CREAM, -100, -310, 1066, 620, 30, 2, 200, 40)
    door(g, 200, -320, 110, 230)
    # the stair tower
    tx, ty = 480.0, -150.0
    g.cyl(CREAM, (tx, ty, -FOOT), 196, FOOT + 80, steps=10)
    z = 80.0
    for k in range(7):
        g.cyl(SKY if k % 2 == 0 else WHITE, (tx, ty, z), 190 if k % 2 == 0 else 198, 300 if k % 2 == 0 else 30, steps=10, glow=0.25 if k % 2 == 0 else 0.0)
        z += 300 if k % 2 == 0 else 30
    for r, c, hh in ((220, TEAL, 20), (160, CREAM, 40), (110, HOT, 40)):
        g.cyl(c, (tx, ty, z), r, hh, steps=10)
        z += hh
    g.box(HOT, (tx, ty, z + 50), (40, 240, 190))
    g.box(HOT, (tx, ty, z + 170), (40, 130, 60))
    g.cyl(STEEL, (tx, ty, z + 190), 6, 150, steps=5)
    v = sc.part(P.motion(swing=50, pivot=(tx, ty, z + 320), axis=P.UP, rate=0.08, phase=0.3))   # the vane: a sun and its tail
    v.box(YEL, (tx, ty, z + 320), (150, 6, 10))
    disc_y(v, YEL, tx + 76, ty - 4, z + 320, 30, 8, steps=8)
    v.box(HOT, (tx - 80, ty, z + 320), (40, 6, 50))
    # the roof: a washing line, a deckchair, a tank of flowers
    for s in (-1, 1):
        g.cyl(STEEL, (-160 + s * 280, 200, 1066), 6, 160, steps=5)
    g.box(STEEL, (-160, 200, 1220), (560, 5, 5))
    for k in range(5):
        w = sc.part(P.motion(swing=11, pivot=(0, 200, 1218), axis=0.0, rate=0.33, phase=k * 0.19))
        w.box((WHITE, HOT, YEL, TEAL, WHITE)[k], (-370 + k * 105, 200, 1168), (80, 5, 96))
    g.box(CREAM, (-380, -120, 1096), (200, 70, 60))
    for k in range(3):
        g.sphere((HOT, YEL, HOT)[k], (-440 + k * 60, -120, 1136), 22, steps=(3, 5))
    vent(g, 150, 250, 1066)
    cowl(sc, -420, 80, 1066, phase=0.8)


def bath_house(sc):
    """The bathing pavilion on the sand: a long low house with round ends, a row of cabin doors in three colours,
    a stepped pylon with a lamp, towels drying on a line, a shower. 16 x 6.6 m, 9.3 m to the pylon's lamp."""
    g = sc.still()
    lamp(g, -470, -210, 280, 0.5)
    lamp(g, 470, -210, 280, 0.5)
    rail(g, -440, -160, -200, 348, n=3)
    rail(g, 160, 440, -200, 348, n=3)
    tub(g, -520, -330)
    tub(g, 500, -360)
    body(g, 0, 60, 900, 540, 330)
    for s in (-1, 1):
        g.cyl(CREAM, (s * 450, 60, -FOOT), 270, FOOT + 330, steps=12)
        g.cyl(HOT, (s * 450, 60, 34), 276, 26, steps=12)
        g.cyl(TEAL, (s * 450, 60, 330), 330, 18, steps=12)
        g.cyl(NAVY, (s * 450, 60, 170), 275, 70, steps=12)
        g.cyl(TEAL, (s * 450, 60, 250), 300, 12, steps=12)
    band(g, HOT, 0, 60, 900, 540, 47, 26)
    g.box(TEAL, (0, 60, 339), (900, 660, 18))
    for k in range(8):                                             # the cabins
        x = -385 + k * 110
        g.box((HOT, YEL, TEAL)[k % 3], (x, -214, 120), (84, 10, 200))
        g.box(WHITE, (x, -220, 180), (50, 5, 10))
        g.box(WHITE, (x, -220, 160), (50, 5, 10))
        disc_y(g, NAVY, x, -216, 272, 20, 8, steps=6)
    g.box(TEAL, (0, -250, 300), (920, 90, 12))                     # the eyebrow over them
    g.box(CREAM, (0, -300, 8), (980, 180, 30))                     # the step
    # the pylon in the middle
    stepped(g, CREAM, 0, 60, 348, 300, 220, 3, 70, 60)
    g.box(CREAM, (0, 60, 690), (110, 110, 330))
    for k in range(3):
        g.box(HOT if k % 2 == 0 else TEAL, (0, 60, 600 + k * 90), (126, 126, 20))
    stepped(g, YEL, 0, 60, 855, 90, 90, 2, 30, 24)
    globe(g, 0, 60, 936, 34, 0.5)
    pennant(sc, -300, 60, 520, HOT, mast=170, phase=0.1)
    pennant(sc, 300, 60, 520, YEL, mast=170, phase=0.6)
    # towels on a line at one end, a shower at the other, a ring on the wall
    for x in (560, 780):
        g.cyl(WHITE, (x, -330, -80), 7, 270, steps=5)
    g.box(WHITE, (670, -330, 186), (226, 5, 5))
    for k in range(3):
        w = sc.part(P.motion(swing=12, pivot=(0, -330, 184), axis=0.0, rate=0.36, phase=k * 0.3))
        w.box((HOT, YEL, TEAL)[k], (604 + k * 66, -330, 132), (50, 5, 100))
        w.box(WHITE, (604 + k * 66, -333, 132), (50, 5, 22))
    g.cyl(STEEL, (-700, -330, -80), 8, 320, steps=5)
    g.box(STEEL, (-700, -350, 236), (8, 44, 8))
    g.cyl(TEAL, (-700, -350, -20), 60, 30, steps=8)
    for x in (-440, 440):
        disc_y(g, WHITE, x, -212, 250, 34, 10, steps=8)
        disc_y(g, HOT, x, -218, 250, 22, 8, steps=8)


# ------------------------------------------------------------------------------------------ the props
def bollards(sc):
    """LOW. A row of five bollards with gum-ball heads, a chain hanging between them. 6.4 m, 1.05 m tall."""
    g = sc.still()
    cols = (HOT, YEL, TEAL, HOT, YEL)
    for k in range(5):
        x = -300 + k * 150
        g.cyl(WHITE, (x, 0, -60), 15, 122, steps=6)
        g.cyl(TEAL, (x, 0, 22), 19, 10, steps=6)
        g.sphere(cols[k], (x, 0, 82), 23, steps=(4, 6))
        if k < 4:
            tilt_box(g, STEEL, (x + 12, 0, 54), (x + 75, 0, 38), 6, 6)
            tilt_box(g, STEEL, (x + 75, 0, 38), (x + 138, 0, 54), 6, 6)


def bench(sc):
    """LOW. A deco bench: stepped ends, a striped back, a planter bowl at each end. 4.3 m, 1.2 m tall."""
    g = sc.still()
    g.box(TEAL, (0, 0, 44), (230, 54, 10))
    for k, c in enumerate((CREAM, HOT, CREAM)):
        g.box(c, (0, 30, 66 + k * 18), (230, 8, 14))
    for s in (-1, 1):
        g.box(CREAM, (s * 122, 0, -10), (14, 70, 100))             # the ends: stepped up to the back
        g.box(CREAM, (s * 122, 12, 54), (14, 46, 30))
        g.box(CREAM, (s * 122, 24, 88), (14, 22, 38))
        g.box(CREAM, (s * 176, 4, -30), (44, 44, 60))
        bowl(g, s * 176, 4, 0, 34)


def pylon_a(sc):
    """TALL. A round striped lamp pylon: stepped foot, five bands, three fins under a yellow globe. 6.6 m."""
    g = sc.still()
    g.cyl(CREAM, (0, 0, -120), 62, 150, steps=8)
    g.cyl(TEAL, (0, 0, 30), 50, 16, steps=8)
    g.cyl(CREAM, (0, 0, 46), 36, 500, steps=8)
    for k in range(5):
        g.cyl(HOT if k % 2 == 0 else TEAL, (0, 0, 110 + k * 90), 41, 24, steps=8)
    for k in range(3):
        a = math.radians(90 + k * 120)
        g.box(TEAL, (math.cos(a) * 46, math.sin(a) * 46, 500), (70, 8, 130), rot=(0, 90 + k * 120, 0))
    g.cyl(CREAM, (0, 0, 546), 52, 14, steps=8)
    g.cyl(HOT, (0, 0, 560), 30, 24, steps=8)
    globe(g, 0, 0, 622, 42, 0.6)


def pylon_b(sc):
    """TALL. A fin totem: a stepped hot pink blade through four teal eyebrows, three lit yellow bosses, a ball on
    top. 8.9 m. Broad to whoever drives up to it, thin from the side."""
    g = sc.still()
    stepped(g, CREAM, 0, 0, -120, 150, 260, 1, 0, 170)
    stepped(g, CREAM, 0, 0, 50, 110, 220, 2, 30, 30)
    g.box(HOT, (0, 0, 420), (40, 170, 620))                        # z 110 .. 730
    g.box(HOT, (0, -20, 760), (40, 110, 60))
    g.box(HOT, (0, -34, 810), (40, 60, 40))
    for k in range(4):
        g.box(TEAL, (0, 0, 170 + k * 170), (90, 230, 12))
    for k in range(3):
        disc_x(g, YEL, 0, 0, 255 + k * 170, 52, 66, steps=10, glow=0.4)
    g.cyl(STEEL, (0, -34, 830), 5, 30, steps=5)
    g.sphere(YEL, (0, -34, 874), 20, steps=(4, 6), glow=0.5)


def clock(sc):
    """TALL. The finned street clock: two faces at ten past ten, a pendulum swinging under them. 4.9 m."""
    g = sc.still()
    stepped(g, CREAM, 0, 0, -100, 96, 96, 1, 0, 130)
    stepped(g, TEAL, 0, 0, 30, 76, 76, 2, 20, 16)
    g.box(CREAM, (0, 0, 180), (36, 36, 240))
    for k in range(3):
        g.box(HOT, (0, 0, 110 + k * 60), (44, 44, 12))
    hz = 392.0
    g.cyl(TEAL, (0, -30, hz), 86, 60, rot=(0, 0, 90), steps=14)
    for s in (-1, 1):
        y = s * 30
        disc_y(g, WHITE, 0, y - 5 if s < 0 else y - 1, hz, 72, 6, steps=14)
        fy = y - 9 if s < 0 else y + 5
        tilt_box(g, NAVY, (0, fy, hz), (-34, fy, hz + 26), 7, 9)                      # the hour hand
        tilt_box(g, NAVY, (0, fy, hz), (26, fy, hz + 52), 7, 7)                       # the minute hand
        for k in range(4):
            a = math.radians(k * 90)
            g.box(HOT, (math.cos(a) * 58, fy, hz + math.sin(a) * 58), (10, 5, 10))
        g.box(HOT, (s * 110, 0, hz), (14, 70, 150))                                   # the fins, stepped
        g.box(HOT, (s * 132, 0, hz), (14, 70, 96))
        g.box(HOT, (s * 154, 0, hz), (14, 70, 46))
    top = stepped(g, CREAM, 0, 0, hz + 84, 90, 60, 2, 34, 18)
    g.sphere(YEL, (0, 0, top + 16), 18, steps=(4, 6))
    p = sc.part(P.motion(swing=15, pivot=(0, 0, 306), axis=90.0, rate=0.5))
    p.box(NAVY, (0, -26, 268), (7, 6, 76))
    disc_y(p, YEL, 0, -30, 224, 20, 8, steps=8)


def neon_arrow(sc):
    """TALL. The neon arrow on its post: a cream board with a sun on top and a lit arrow pointing DOWN at the door
    under it (never along the road), bulbs down both edges. 6.8 m."""
    g = sc.still()
    stepped(g, CREAM, 0, 0, -100, 90, 90, 1, 0, 130)
    stepped(g, TEAL, 0, 0, 30, 70, 70, 2, 20, 16)
    g.box(TEAL, (0, 14, 210), (24, 24, 300))
    for k in range(2):
        g.box(HOT, (0, 14, 130 + k * 90), (32, 32, 12))
    side(g, CREAM, [(-110, 360), (0, 300), (110, 360), (110, 600), (-110, 600)], -6, 10)   # the board, pointed below
    side(g, HOT, [(-80, 430), (-34, 430), (-34, 580), (34, 580), (34, 430), (80, 430), (0, 340)], -16, -6, glow=0.5)   # the arrow
    for s in (-1, 1):
        for k in range(4):
            g.box(YEL, (s * 98, -10, 400 + k * 56), (14, 10, 14), glow=0.6)
    arch(g, YEL, 0, 600, 0, 54, -10, 10, n=6, glow=0.4)            # the sun on top, five short rays
    for k in range(5):
        a = math.radians(20 + k * 35)
        tilt_box(g, HOT, (math.cos(a) * 62, 0, 600 + math.sin(a) * 62), (math.cos(a) * 96, 0, 600 + math.sin(a) * 96), 12, 14)
    g.box(TEAL, (0, 2, 606), (240, 22, 12))


def juke_kiosk(sc):
    """TALL. The kiosk shaped like a jukebox: an arched top in three bands of lit colour, a grille, a hatch under a
    small awning, bubbles rising up its sides. 2.9 x 1.9 m, 3.4 m tall."""
    g = sc.still()
    g.box(CREAM, (0, 0, 60), (250, 170, 320))                      # z -100 .. 220
    g.cyl(CREAM, (0, -85, 220), 125, 170, rot=(0, 0, 90), steps=14)
    arch(g, HOT, 0, 220, 100, 128, -100, -85, n=8, glow=0.4)
    arch(g, YEL, 0, 220, 70, 96, -98, -85, n=8, glow=0.4)
    arch(g, TEAL, 0, 220, 0, 66, -96, -85, n=8)
    for s in (-1, 1):
        g.box(HOT, (s * 114, -92, 110), (28, 15, 220), glow=0.4)
        g.box(YEL, (s * 83, -91, 150), (26, 13, 140), glow=0.4)
        g.box(TEAL, (s * 136, 0, 120), (22, 120, 240))                               # the side fins
        g.box(TEAL, (s * 152, 0, 100), (14, 90, 160))
        for k in range(3):                                                           # bubbles
            b = sc.part(P.motion(bob=-7, rate=0.5, phase=k * 0.33 + (0.15 if s > 0 else 0.0)))
            b.box(WHITE, (s * 114, -102, 40 + k * 70), (10, 6, 10))
    g.box(NAVY, (0, -88, 150), (130, 8, 100))                      # the hatch
    g.box(WHITE, (0, -106, 96), (150, 44, 10))
    awning(g, -70, 70, -86, 216, [HOT, CREAM], depth=50, drop=22, n=4)
    g.box(TEAL, (0, -90, 44), (190, 10, 80))                       # the grille
    for k in range(3):
        g.box(CREAM, (0, -96, 22 + k * 22), (170, 5, 8))
    for k in range(3):
        disc_y(g, YEL, -44 + k * 44, -97, 250, 15, 6, steps=8)
    g.box(TEAL, (0, 0, 349), (60, 150, 10))
    g.sphere(YEL, (0, 0, 362), 14, steps=(3, 6))


def deck_hut(sc):
    """TALL. Deckchairs for hire: a striped hut with a hatch and a flat teal roof, a stack of folded chairs leaning on
    it, two set out, a pennant. 5.6 x 2.6 m, 3.7 m to the pennant."""
    g = sc.still()
    for k in range(6):
        g.box(CREAM if k % 2 == 0 else HOT, (-90, 30, -100 + 27 + k * 54), (220, 190, 54))    # z -100 .. 224
    g.box(TEAL, (-90, 20, 232), (270, 250, 16))
    g.box(YEL, (-90, -102, 246), (150, 14, 28))
    g.box(YEL, (-90, -102, 268), (80, 14, 18))
    g.box(NAVY, (-90, -68, 150), (120, 8, 80))
    g.box(WHITE, (-90, -84, 106), (140, 40, 10))
    g.box(WHITE, (-180, -68, 150), (36, 6, 70))                    # a board: three stripes, no words
    for k, c in enumerate((HOT, TEAL, YEL)):
        g.box(c, (-180, -72, 172 - k * 22), (26, 5, 12))
    for k in range(6):                                             # the stack, leaning on the hut
        x = 40 + k * 18
        tilt_box(g, WOOD, (x + 26, 30, 0), (x, 30, 130), 110, 8)
        tilt_box(g, (HOT, YEL, TEAL)[k % 3], (x + 30, 30, 26), (x + 8, 30, 118), 96, 5)
    for x, c in ((230, YEL), (330, TEAL)):                          # two chairs set out, facing the road
        for s in (-1, 1):
            tilt_box(g, WOOD, (x + s * 30, -104, 0), (x + s * 30, -20, 96), 8, 8)
            tilt_box(g, WOOD, (x + s * 30, -30, 0), (x + s * 30, -90, 50), 8, 8)
        tilt_box(g, c, (x, -94, 22), (x, -24, 90), 54, 6)
    pennant(sc, -90, 60, 330, HOT, mast=90, w=90, h=44, phase=0.4)
    g.box(CREAM, (150, -10, -45), (460, 270, 100))                 # a mat of boards on the sand


def fountain(sc):
    """TALL. The sunrise fountain: a stepped round basin, a bowl on a stem, a small half-sun of rays on top, seven
    plumes of water that rise and fall. 4.6 m across, 3.3 m."""
    g = sc.still()
    g.cyl(CREAM, (0, 0, -150), 230, 200, steps=14)
    g.cyl(HOT, (0, 0, 22), 236, 14, steps=14)
    g.cyl(TEAL, (0, 0, 50), 205, 6, steps=14)                      # water
    g.cyl(CREAM, (0, 0, 40), 70, 70, steps=10)
    g.cyl(CREAM, (0, 0, 110), 36, 50, steps=8)
    g.cyl(CREAM, (0, 0, 160), 44, 34, steps=10, top_r=120)
    g.cyl(TEAL, (0, 0, 194), 112, 6, steps=10)
    g.cyl(HOT, (0, 0, 182), 124, 10, steps=10)
    g.cyl(CREAM, (0, 0, 194), 22, 50, steps=6)
    arch(g, YEL, 0, 244, 0, 34, -8, 8, n=6)
    for k in range(5):
        a = math.radians(20 + k * 35)
        tilt_box(g, YEL if k % 2 == 0 else HOT, (math.cos(a) * 40, 0, 244 + math.sin(a) * 40), (math.cos(a) * 78, 0, 244 + math.sin(a) * 78), 10, 12)
    for k in range(6):                                             # plumes round the basin, out of step
        a = math.radians(k * 60 + 30)
        w = sc.part(P.motion(bob=-12, rate=0.4, phase=k / 6.0))
        w.cyl(WHITE, (math.cos(a) * 150, math.sin(a) * 150, 44), 12, 86, steps=5, top_r=5)
        w.sphere(SKY, (math.cos(a) * 150, math.sin(a) * 150, 132), 13, steps=(3, 5))
    w = sc.part(P.motion(bob=-8, rate=0.3, phase=0.5))
    for k in range(4):
        a = math.radians(k * 90 + 45)
        w.sphere(WHITE, (math.cos(a) * 96, math.sin(a) * 96, 204), 13, steps=(3, 5))


# ------------------------------------------------------------------------------------------ the hero
def sunburst(sc):
    """THE SUNBURST: a round cream bandshell under a half-sun of eleven tapering rays, yellow and hot pink by turns,
    60 m wide and 45 m tall; teal steps, benches, two ringed pylons with pennants, lamps round the shell's mouth.
    Stands on the sand: the stage's body goes 3 m down."""
    g = sc.still()
    zc = 160.0                                                     # the stage, and the sun's centre
    g.box(CREAM, (0, 0, (zc - FOOT) / 2.0), (2900, 1500, zc + FOOT))
    g.box(HOT, (0, -4, zc - 40), (2916, 1516, 24))
    for k in range(4):                                             # the steps
        g.box(TEAL, (0, -790 - k * 60, (zc - 40 * (k + 1) - FOOT) / 2.0), (1500 + k * 120, 120, zc - 40 * (k + 1) + FOOT))
    # the shell: five arches, each smaller and further back, and the wall behind them
    for k in range(5):
        r1 = 1240 - k * 140
        y0 = -640 + k * 170
        arch(g, CREAM if k % 2 == 0 else WHITE, 0, zc, r1 - 150, r1, y0, y0 + 190, n=12)
        arch(g, HOT if k == 0 else TEAL, 0, zc, r1 - 26, r1 + 4, y0 - 12, y0, n=12)
    arch(g, CREAM, 0, zc, 0, 560, 190, 230, n=12)
    for k in range(3):                                             # the back wall's stripes
        arch(g, (HOT, YEL, TEAL)[k], 0, zc, 250 + k * 110, 290 + k * 110, 180, 190, n=10)
    for k in range(13):                                            # lamps round the mouth
        a = math.radians(6 + k * 14)
        g.box(YEL, (math.cos(a) * 1180, -656, zc + math.sin(a) * 1180), (30, 16, 30), glow=0.6)
    # the sun: a core behind the shell, and the rays
    arch(g, YEL, 0, zc, 0, 1420, 250, 330, n=14)
    arch(g, HOT, 0, zc, 1250, 1330, 236, 250, n=14)
    for k in range(11):
        a = math.radians(15 + k * 15)
        ca, sa = math.cos(a), math.sin(a)
        r_tip = 1.0 / math.sqrt((ca / 3200.0) ** 2 + (sa / 4340.0) ** 2)
        long_ray = (k % 2 == 1)
        if not long_ray:
            r_tip *= 0.8
        col = YEL if long_ray else HOT
        y0 = 250 if long_ray else 330
        r0 = 1380.0

        def ray(col2, r_a, r_b, w_a, w_b, ya, yb, glow=0.0):
            pts = [(r_a * ca + w_a * sa, zc + r_a * sa - w_a * ca), (r_b * ca + w_b * sa, zc + r_b * sa - w_b * ca),
                   (r_b * ca - w_b * sa, zc + r_b * sa + w_b * ca), (r_a * ca - w_a * sa, zc + r_a * sa + w_a * ca)]
            side(g, col2, pts, ya, yb, glow)

        ray(col, r0, r_tip, 170 if long_ray else 150, 34, y0, y0 + 80)
        ray(CREAM if long_ray else PINK, r0 + 120, r0 + (r_tip - r0) * 0.72, 86 if long_ray else 70, 30, y0 - 10, y0)   # a paler heart
        g.box(YEL, (r_tip * ca, y0 + 40, zc + r_tip * sa), (60, 100, 60), rot=(math.degrees(a), 0, 0), glow=0.6)        # a lamp at the tip
        if long_ray:                                               # a brace behind each long ray
            tilt_box(g, STEEL, (ca * 1300, 420, zc + sa * 1300), (ca * r_tip * 0.6, 420, zc + sa * r_tip * 0.6), 30, 30)
    for s in (-1, 1):                                              # props behind the outer rays, down to the sand
        tilt_box(g, STEEL, (s * 1900, 430, -FOOT), (s * 1900, 430, zc + 700), 40, 40)
        tilt_box(g, STEEL, (s * 1000, 430, -FOOT), (s * 1000, 430, zc + 1500), 40, 40)
    tilt_box(g, STEEL, (0, 430, -FOOT), (0, 430, zc + 2600), 50, 50)
    # the pylons at the stage's corners
    for s in (-1, 1):
        px = s * 1340
        g.box(CREAM, (px, -640, 560), (150, 150, 800))             # z 160 .. 960
        for k in range(6):
            g.box(HOT if k % 2 == 0 else TEAL, (px, -640, 260 + k * 120), (168, 168, 26))
        top = stepped(g, CREAM, px, -640, 960, 120, 120, 3, 30, 50)
        globe(g, px, -640, top + 44, 50, 0.6)
        pennant(sc, px, -640, top + 190, YEL if s < 0 else HOT, w=260, h=110, mast=110, phase=0.2 + 0.25 * s, rate=0.3, swing=14)
        # benches for the audience, in two blocks
        for k in range(4):
            g.box(TEAL, (s * 520, -1260 - k * 150, 20), (760, 50, 100))
            g.box(CREAM, (s * 520, -1238 - k * 150, 80), (760, 12, 36))
        # a bowl of flowers each side of the steps
        g.cyl(CREAM, (s * 1000, -900, -FOOT), 90, FOOT + 60, steps=8)
        bowl(g, s * 1000, -900, 60, 110)
        # a horn loudspeaker on a post
        g.cyl(STEEL, (s * 1120, -1500, -150), 10, 560, steps=5)
        g.cyl(YEL, (s * 1120, -1500, 400), 16, 90, rot=(0, 0, -80), steps=8, top_r=60)
    # on the stage: a white piano, music stands
    g.box(WHITE, (-260, -60, zc + 70), (300, 190, 40))
    g.box(WHITE, (-260, -160, zc + 96), (300, 20, 50))
    for x, y in ((-390, 20), (-130, 20), (-260, -140)):
        g.box(WHITE, (x, y, zc + 26), (14, 14, 52))
    for k in range(4):
        x = 150 + k * 150
        g.box(STEEL, (x, -200 + (k % 2) * 120, zc + 55), (6, 6, 110))
        g.box(HOT if k % 2 else YEL, (x, -204 + (k % 2) * 120, zc + 118), (50, 6, 36))
    pennant(sc, 0, 290, zc + 4440, HOT, w=260, h=110, mast=110, phase=0.5, rate=0.25, swing=12)


JOBS = [
    ('SM_S1D1_Diner', diner, MOVE), ('SM_S1D1_RecordShop', record_shop, MOVE), ('SM_S1D1_PictureHouse', picture_house, MOVE),
    ('SM_S1D1_Bowling', bowling, MOVE), ('SM_S1D1_Parlour', parlour, MOVE), ('SM_S1D1_MilkBar', milk_bar, MOVE),
    ('SM_S1D1_DanceHall', dance_hall, MOVE), ('SM_S1D1_Flats', flats, MOVE), ('SM_S1D1_BathHouse', bath_house, MOVE),
    ('SM_S1D1_Bollards', bollards, STILL), ('SM_S1D1_Bench', bench, STILL), ('SM_S1D1_PylonA', pylon_a, STILL), ('SM_S1D1_PylonB', pylon_b, STILL),
    ('SM_S1D1_Clock', clock, SMALL), ('SM_S1D1_NeonArrow', neon_arrow, STILL), ('SM_S1D1_JukeKiosk', juke_kiosk, SMALL),
    ('SM_S1D1_DeckHut', deck_hut, SMALL), ('SM_S1D1_Fountain', fountain, SMALL),
    ('SM_S1D1_Sunburst', sunburst, dict(grow=(60, 60, 60), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)            # (name, build, Scene.save options)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
