"""Nevada, Stage 1, builder s1d8: district 8 "Vegas Approach" (22.0 to 25.6 km).
/Game/World/Nevada/SM_S1D8_*.   Run: import ft_world_nevada_s1d8 as K; K.run()

A palace boulevard: white colonnaded carriage porches, tiered wedding-cake towers, glass domes, grand stairs, urns and
balustrades. Marble white, gilding, crimson carpets and roofs, turquoise water and glass, navy window bands.
Every mesh is an ft_people.Scene (one section, M_People). The hero: THE CROWN TOWER, a white fluted tower of five
set-backs carrying a giant gold crown on a crimson cushion, its turquoise jewels glowing steadily.
Nothing here carries a word, a numeral, a dice, a chip, a vehicle or a column of square lamps. Light is steady
(glow only: M_People cannot blink) and only on pieces that the table stands 24 m or more out.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))          # no moving part: true bounds
SMALL = dict(grow=(40, 40, 20), grow_down=(40, 40, 0))     # a pennant, a bell, a ribbon: parts that stay close

W, GOLD, CRIM, TURQ, NAVY = '#fff6ea', '#ffc93c', '#6a2440', '#3fd6d0', '#2b2350'     # the district's five
SHADE, LEAF, LEAF_L, TRUNK = '#c9c4d6', '#5f8f4a', '#8fb86a', '#8a6a4a'              # marble in shade, clipped green
FOOT = 200.0                                               # a building's footing under z = 0


# ------------------------------------------------------------------------------------------ helpers
def bx(g, col, x, y, z0, w, d, h, glow=0.0, rot=(0, 0, 0)):
    """A box standing on z0."""
    g.box(col, (x, y, z0 + h / 2.0), (w, d, h), rot=rot, glow=glow)


def ngon(r, n, cx=0.0, cy=0.0, a0=0.0):
    """A regular outline, counter-clockwise, its first corner at angle a0 (radians)."""
    return [(cx + r * math.cos(a0 + 2.0 * math.pi * k / n), cy + r * math.sin(a0 + 2.0 * math.pi * k / n)) for k in range(n)]


def band(g, x, y, z, w, d, h=110.0, col=NAVY, out=5.0, glow=0.0):
    """A window band right round a block: one box, proud of the wall by `out`. z = its middle."""
    g.box(col, (x, y, z), (w + 2 * out, d + 2 * out, h), glow=glow)


def piers(g, x, y, z0, w, d, h, n, pw=34.0, col=W, out=11.0, sides=2):
    """Marble piers over a band: n across the front (each one box right through the block, so the back has them
    too) and `sides` across each end."""
    for k in range(n):
        px = x - w / 2.0 + pw / 2.0 + k * (w - pw) / (n - 1.0)
        bx(g, col, px, y, z0, pw, d + 2 * out, h)
    for k in range(sides):
        py = y - d / 2.0 + (k + 1) * d / (sides + 1.0)
        bx(g, col, x, py, z0, w + 2 * out, pw, h)


def cornice(g, x, y, z, w, d, col=GOLD, h=26.0, out=22.0):
    bx(g, col, x, y, z, w + 2 * out, d + 2 * out, h)


def tier(g, x, y, z0, w, d, h, n=5, floors=1, corn=GOLD):
    """One storey block of the palace style: marble, navy window bands, piers, a gilded cornice. Returns its top."""
    bx(g, W, x, y, z0, w, d, h)
    fh = h / float(floors)
    for f in range(floors):
        band(g, x, y, z0 + fh * (f + 0.52), w, d, fh * 0.46)
    piers(g, x, y, z0, w, d, h, n, sides=max(1, n // 3))
    cornice(g, x, y, z0 + h, w, d, corn)
    return z0 + h + 26.0


def stepped(g, col, x, y, z, w, d, n=3, h=50.0, shrink=0.74, cap=GOLD):
    """A wedding-cake roof: n slabs, each smaller. Returns its top."""
    f = 1.0
    for k in range(n):
        f = shrink ** k
        bx(g, col, x, y, z + k * h, w * f, d * f, h)
    z += n * h
    if cap:
        bx(g, cap, x, y, z, max(30.0, w * f * 0.5), max(30.0, d * f * 0.5), 16.0)
        z += 16.0
    return z


def column(g, x, y, z0, h, r=24.0, steps=6):
    g.cyl(W, (x, y, z0), r, h - 26.0, steps=steps, top_r=r * 0.86)
    bx(g, GOLD, x, y, z0 + h - 26.0, r * 2.6, r * 2.6, 26.0)


def urn(g, x, y, z, s=1.0, col=GOLD, steps=6):
    """A gilded urn, 96 cm tall at s = 1."""
    g.cyl(col, (x, y, z), 16 * s, 22 * s, steps=steps, top_r=9 * s)
    g.cyl(col, (x, y, z + 22 * s), 12 * s, 62 * s, steps=steps, top_r=38 * s)
    g.cyl(col, (x, y, z + 84 * s), 42 * s, 12 * s, steps=steps)


def finial(g, x, y, z, s=1.0):
    g.cyl(GOLD, (x, y, z), 7 * s, 90 * s, steps=5)
    g.sphere(GOLD, (x, y, z + 100 * s), 17 * s, steps=(3, 5))


def dome(g, x, y, z, r, col=TURQ, squash=0.85, steps=(5, 10), glow=0.0, top=True):
    """A glass dome: z = its springing."""
    g.sphere(col, (x, y, z), r, scale=(1, 1, squash), steps=steps, glow=glow)
    if top:
        g.cyl(GOLD, (x, y, z + r * squash - 8), r * 0.13, r * 0.3, steps=5, top_r=max(5.0, r * 0.03))
        g.sphere(GOLD, (x, y, z + r * squash + r * 0.36), max(12.0, r * 0.09), steps=(3, 5))


def ribs(g, x, y, z, r, squash, n, col=GOLD, w=14.0, a0=0.0):
    """Gilded ribs up a dome: n meridians, two straight pieces each."""
    for k in range(n):
        a = a0 + 2.0 * math.pi * k / n
        ca, sa = math.cos(a), math.sin(a)
        pts = []
        for phi in (86.0, 48.0, 14.0):
            q = math.radians(phi)
            rr = (r + 5.0) * math.sin(q)
            pts.append((x + ca * rr, y + sa * rr, z + (r + 5.0) * squash * math.cos(q)))
        tilt_box(g, col, pts[0], pts[1], w, 10.0)
        tilt_box(g, col, pts[1], pts[2], w, 10.0)


def balustrade(g, x0, x1, y, z, n=None, h=84.0, col=W):
    """A rail on fat balusters, along the road at depth y."""
    L = x1 - x0
    n = n or max(2, int(L / 90.0))
    g.box(col, ((x0 + x1) / 2.0, y, z + h - 7.0), (L, 24.0, 14.0))
    g.box(col, ((x0 + x1) / 2.0, y, z + 6.0), (L, 24.0, 12.0))
    for k in range(n):
        g.box(col, (x0 + (k + 0.5) * L / n, y, z + h / 2.0), (22.0, 16.0, h - 24.0))


def stair(g, x, y0, z1, w, n, run=38.0, carpet=0.0, z0=0.0):
    """Steps down towards the road from the edge (y0, z1) to z0, and a crimson runner `carpet` wide over them."""
    rise = (z1 - z0) / float(n + 1)
    for k in range(n):
        d = run * (n - k)
        lo = 70.0 if k == 0 else 0.0
        bx(g, SHADE if k % 2 else W, x, y0 - d / 2.0, z0 + rise * k - lo, w, d, rise + lo)
    if carpet:
        tilt_box(g, CRIM, (x, y0 + 4.0, z1 + 4.0), (x, y0 - run * n, z0 + rise + 4.0), carpet, 8.0)


def doors(g, x, yf, z0, w=300.0, h=330.0):
    """A navy doorway with two gilded leaves in the wall whose face is at yf."""
    bx(g, NAVY, x, yf - 2.0, z0, w + 60.0, 12.0, h + 30.0)
    for s in (-1, 1):
        bx(g, GOLD, x + s * w * 0.26, yf - 6.0, z0, w * 0.46, 10.0, h)


def pennant(sc, g, x, y, z0, h, L=230.0, fh=74.0, phase=0.0, swing=14.0, col=CRIM):
    """A gilded mast on z0 with a long pennant that swings round it."""
    g.cyl(GOLD, (x, y, z0), 8.0, h, steps=5)
    g.sphere(GOLD, (x, y, z0 + h + 12.0), 15.0, steps=(3, 5))
    zt = z0 + h - 6.0
    f = sc.part(P.motion(swing=swing, pivot=(x, y, zt), axis=P.UP, rate=0.4, phase=phase))
    f.side(col, [(x + 6.0, zt - fh), (x + L, zt - fh * 0.5), (x + 6.0, zt)], y - 3.0, y + 3.0)
    f.box(GOLD, (x + L * 0.42, y, zt - fh * 0.5), (L * 0.5, 8.0, 10.0))


# ------------------------------------------------------------------------------------------ the buildings
def carriage_porch(sc):
    """The carriage porch: a square pavilion on paired columns under a gold-ribbed crimson canopy, in front of an
    entrance hall with a glass dome. 23 x 18 m, the dome 14.6 m, the pennant masts 15.7 m."""
    g = sc.still()
    bx(g, SHADE, 0, 380, -FOOT, 2300, 720, FOOT + 60)
    z = tier(g, 0, 400, 60, 2200, 600, 500, n=8)
    for s in (-1, 1):
        balustrade(g, s * 760 if s > 0 else -1090, s * 1090 if s > 0 else -760, 115, z, n=4)
        urn(g, s * 1060, 150, z + 84, 1.4)
    z = tier(g, 0, 420, z, 1500, 480, 360, n=5)
    zr = stepped(g, CRIM, 0, 420, z, 1420, 420, n=2, h=60, shrink=0.8, cap=None)
    g.cyl(W, (0, 420, zr), 190, 90, steps=10)
    g.cyl(GOLD, (0, 420, zr + 90), 202, 18, steps=10)
    dome(g, 0, 420, zr + 108, 185, glow=0.15)
    ribs(g, 0, 420, zr + 108, 185, 0.85, 6)
    for s in (-1, 1):
        pennant(sc, g, s * 590, 420, z, 570, phase=0.5 + 0.3 * s)
    doors(g, 0, 100, 60, 320, 340)
    for s in (-1, 1):
        bx(g, GOLD, s * 260, 84, 380, 14, 34, 14)
        g.sphere(W, (s * 260, 66, 372), 30, steps=(4, 6), glow=0.8)
    # the porch
    bx(g, SHADE, 0, -330, -FOOT, 1160, 900, FOOT + 30)
    bx(g, CRIM, 0, -330, 30, 300, 900, 8)
    for sx in (-1, 1):
        for cx in (430, 525):
            for cy in (-700, 40):
                column(g, sx * cx, cy, 30, 470, r=24, steps=6)
    bx(g, W, 0, -330, 500, 1160, 900, 80)
    cornice(g, 0, -330, 580, 1160, 900)
    stepped(g, CRIM, 0, -330, 606, 1100, 840, n=3, h=55, shrink=0.74, cap=None)
    for xe in (-480, -240, 0, 240, 480):
        for s in (-1, 1):
            tilt_box(g, GOLD, (xe, -330 + s * 762, 652), (xe * 0.55, -330 + s * 215, 780), 18, 12)
    urn(g, 0, -330, 771, 1.6)
    for s in (-1, 1):
        bx(g, W, s * 640, -760, -60, 90, 90, 170)
        urn(g, s * 640, -760, 110, 1.1)


def hotel_tower(sc):
    """The tiered hotel: a wedding cake of six storeys' blocks with five set-backs, gilded cornices and corner
    blocks, a crimson stepped roof and a spire. 21 x 18 m, 29.8 m."""
    g = sc.still()
    bx(g, SHADE, 0, 0, -FOOT, 2100, 1500, FOOT + 70)
    dims = ((1900, 1300, 440, 7, 1), (1560, 1060, 480, 6, 2), (1240, 840, 440, 5, 2), (940, 640, 400, 4, 2), (660, 460, 350, 3, 2), (420, 320, 300, 2, 1))
    z = 70.0
    for k, (w, d, h, n, fl) in enumerate(dims):
        z2 = tier(g, 0, 0, z, w, d, h, n=n, floors=fl)
        if k < 5:
            for sx in (-1, 1):
                for sy in (-1, 1):
                    bx(g, GOLD, sx * (w / 2.0 - 40), sy * (d / 2.0 - 40), z2, 56, 56, 78)
        z = z2
    z = stepped(g, CRIM, 0, 0, z, 400, 300, n=3, h=46, shrink=0.7)
    g.cyl(GOLD, (0, 0, z), 9, 150, steps=5)
    g.sphere(GOLD, (0, 0, z + 165), 22, steps=(3, 6))
    z1 = 70 + 440 + 26
    bx(g, TURQ, 0, -590, z1, 520, 84, 10)                                  # the terrace pool
    for s in (-1, 1):
        g.cyl(W, (s * 800, -560, z1), 78, 150, steps=8)
        g.cyl(GOLD, (s * 800, -560, z1 + 150), 86, 12, steps=8)
        dome(g, s * 800, -560, z1 + 162, 80, steps=(4, 8))
    # the door, its canopy on gilded posts, the landing and the runner
    doors(g, 0, -650, 70, 330, 300)
    bx(g, SHADE, 0, -880, -FOOT, 780, 300, FOOT + 40)
    bx(g, CRIM, 0, -850, 40, 290, 380, 8)
    tilt_box(g, CRIM, (0, -650, 468), (0, -1010, 400), 660, 16)
    bx(g, GOLD, 0, -1012, 372, 680, 14, 30)
    for s in (-1, 1):
        g.cyl(GOLD, (s * 300, -990, 40), 9, 345, steps=5)
        urn(g, s * 450, -820, 40, 1.2)


def winter_garden(sc):
    """The winter garden: a glass hall in a marble colonnade under a great turquoise dome with gilded ribs, a
    half-size dome at each end. 25 x 14.5 m, 16.5 m."""
    g = sc.still()
    bx(g, SHADE, 0, 0, -FOOT, 2500, 1300, FOOT + 60)
    bx(g, W, 0, 0, 60, 2400, 1200, 60)
    stair(g, 0, -650, 120, 720, 4, carpet=270)
    bx(g, TURQ, 0, 30, 120, 2200, 1000, 560, glow=0.15)
    band(g, 0, 30, 404, 2200, 1000, 16, col=GOLD, out=4)
    for k in range(9):
        column(g, -1100 + k * 275, -490, 120, 560, r=22, steps=5)
    for s in (-1, 1):
        for y in (-170, 190, 540):
            column(g, s * 1112, y, 120, 560, r=22, steps=5)
    bx(g, W, 0, 30, 680, 2290, 1090, 70)
    cornice(g, 0, 30, 750, 2290, 1090)
    doors(g, 0, -472, 120, 300, 380)
    bx(g, W, 0, 30, 776, 1500, 900, 90)
    g.cyl(W, (0, 30, 866), 520, 110, steps=12)
    g.cyl(GOLD, (0, 30, 976), 540, 22, steps=12)
    dome(g, 0, 30, 998, 500, squash=0.8, steps=(6, 12), glow=0.15)
    ribs(g, 0, 30, 998, 500, 0.8, 8, w=20)
    for s in (-1, 1):
        g.cyl(W, (s * 860, 30, 776), 262, 70, steps=8)
        g.cyl(GOLD, (s * 860, 30, 846), 274, 16, steps=8)
        dome(g, s * 860, 30, 862, 240, steps=(4, 8), glow=0.15)
        urn(g, s * 1130, -560, 120, 1.5)
        urn(g, s * 450, -600, 120, 1.1)


def arcade(sc):
    """The colonnade arcade: seven bays between eight piers, a gilded urn on the parapet over each pier, crimson
    awnings, a domed attic over the middle bay. 29 x 9 m, 15.8 m."""
    g = sc.still()
    bx(g, SHADE, 0, 0, -FOOT, 2900, 900, FOOT + 50)
    bx(g, W, 0, 290, 50, 2800, 300, 640)
    band(g, 0, 290, 560, 2800, 300, 90)
    for k in range(8):
        px = -1330 + k * 380
        bx(g, W, px, -360, 50, 92, 112, 560)
        bx(g, GOLD, px, -360, 610, 122, 142, 30)
        urn(g, px, -400, 830, 1.3, steps=5)
    for k in range(7):
        cx = -1140 + k * 380
        bx(g, NAVY, cx, 136, 50, 230, 12, 400)
        bx(g, GOLD, cx, 132, 450, 250, 12, 16)
        if k % 2 == 0:
            tilt_box(g, CRIM, (cx, 130, 520), (cx, -70, 440), 270, 12)
        else:
            bx(g, GOLD, cx, -412, 560, 60, 14, 60)                       # a gilded keystone
    bx(g, CRIM, 0, -130, 50, 2700, 150, 8)                                # the runner under the colonnade
    bx(g, W, 0, -30, 640, 2880, 880, 80)
    cornice(g, 0, -30, 720, 2880, 880)
    balustrade(g, -1420, 1420, -440, 746, n=15)
    bx(g, W, 0, 60, 746, 780, 600, 330)
    band(g, 0, 60, 920, 780, 600, 150)
    piers(g, 0, 60, 746, 780, 600, 330, 4, sides=1)
    cornice(g, 0, 60, 1076, 780, 600)
    g.cyl(W, (0, 60, 1102), 290, 70, steps=10)
    dome(g, 0, 60, 1172, 280, squash=0.9, steps=(5, 10), glow=0.15)
    ribs(g, 0, 60, 1172, 280, 0.9, 6)
    for s in (-1, 1):
        bx(g, W, s * 1240, 200, 746, 320, 420, 200)
        cornice(g, s * 1240, 200, 946, 320, 420)
        stepped(g, CRIM, s * 1240, 200, 972, 300, 400, n=2, h=50, shrink=0.66)


def theatre(sc):
    """The theatre front: a fan of gilded rays in a great half-round window over a fan-shaped crimson marquee with
    a row of steady lamps, a stepped parapet, two stair towers under turquoise caps, a fly tower. 25 x 15 m, 17 m."""
    g = sc.still()
    F = -450.0
    bx(g, SHADE, 0, -30, -FOOT, 2300, 1000, FOOT + 60)
    stair(g, 0, -530, 60, 1300, 2, run=40, carpet=320)
    bx(g, W, 0, 0, 60, 2000, 900, 1100)
    band(g, 0, 0, 1040, 2000, 900, 90)
    cornice(g, 0, 0, 1160, 2000, 900)
    bx(g, W, 0, F + 60, 1186, 1400, 110, 150)
    bx(g, GOLD, 0, F + 60, 1336, 1440, 130, 18)
    bx(g, W, 0, F + 60, 1354, 800, 110, 130)
    bx(g, GOLD, 0, F + 60, 1484, 840, 130, 18)
    g.sphere(GOLD, (0, F + 60, 1550), 52, steps=(4, 6))
    bx(g, W, 0, 220, 1186, 1100, 420, 380)
    cornice(g, 0, 220, 1566, 1100, 420)
    stepped(g, CRIM, 0, 220, 1592, 1060, 380, n=2, h=50, shrink=0.7)
    for s in (-1, 1):
        bx(g, W, s * 1080, F + 100, 60, 320, 320, 1240)
        band(g, s * 1080, F + 100, 1150, 320, 320, 130)
        cornice(g, s * 1080, F + 100, 1300, 320, 320)
        dome(g, s * 1080, F + 100, 1326, 170, steps=(4, 8), glow=0.15)
    # the fan window
    zc = 610.0
    g.side(NAVY, [(430.0 * math.cos(math.pi * k / 10.0), zc + 430.0 * math.sin(math.pi * k / 10.0)) for k in range(11)], F - 12, F)
    for k in range(7):
        a = math.radians(15 + 25 * k)
        tilt_box(g, GOLD, (math.cos(a) * 70, F - 16, zc + math.sin(a) * 70), (math.cos(a) * 405, F - 16, zc + math.sin(a) * 405), 10, 24, glow=0.5)
    g.side(GOLD, [(95.0 * math.cos(math.pi * k / 6.0), zc + 95.0 * math.sin(math.pi * k / 6.0)) for k in range(7)], F - 22, F - 10)
    # the fan marquee
    def fan(r, n=9):
        return [(r * math.cos(math.pi + math.pi * k / (n - 1.0)), F + r * math.sin(math.pi + math.pi * k / (n - 1.0))) for k in range(n)]
    g.plan(GOLD, fan(650), 470, 488)
    g.plan(CRIM, fan(620), 488, 550)
    for k in range(9):
        a = math.pi + math.pi * (k + 0.5) / 9.0
        g.box(W, (590 * math.cos(a), F + 590 * math.sin(a), 568), (36, 36, 36), glow=0.75)
    for s in (-1, 1):
        tilt_box(g, GOLD, (s * 330, F - 4, 840), (s * 330, F - 480, 556), 10, 10)
    for x in (-360, 0, 360):
        doors(g, x, F, 60, 230, 300)
    for s in (-1, 1):                                                     # the bills: bars of paint, never letters
        bx(g, GOLD, s * 770, F - 4, 250, 230, 10, 330)
        bx(g, CRIM, s * 770, F - 8, 268, 194, 8, 294)
        for j, ww in enumerate((150, 110, 130)):
            bx(g, W, s * 770, F - 12, 470 - j * 70, ww, 6, 30)


def clock_pavilion(sc):
    """The clock pavilion: a slender marble tower on an arch, four dials with ticks and no numerals, three gilded
    bells swinging in an open belfry, a crimson stepped roof and a vane. 7.6 m square, 26 m."""
    g = sc.still()
    bx(g, SHADE, 0, 0, -FOOT, 760, 760, FOOT + 50)
    stair(g, 0, -380, 50, 300, 2, run=30, carpet=140)
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, W, sx * 265, sy * 265, 50, 170, 170, 470)
    bx(g, NAVY, 0, 0, 50, 400, 400, 420)
    for x in (-110, 0, 110):
        bx(g, GOLD, x, -204, 50, 10, 10, 330)
    bx(g, W, 0, 0, 520, 720, 720, 90)
    cornice(g, 0, 0, 610, 720, 720)
    bx(g, W, 0, 0, 636, 560, 560, 520)
    band(g, 0, 0, 900, 560, 560, 220)
    piers(g, 0, 0, 636, 560, 560, 520, 3, sides=1)
    cornice(g, 0, 0, 1156, 560, 560)
    zc = 1182.0
    bx(g, W, 0, 0, zc, 620, 620, 620)
    zm = zc + 310.0
    for rot in ((0, 0, 90), (0, 0, -90), (90, 0, 0), (-90, 0, 0)):          # a dial on each face, whichever way a turn goes
        g.cyl(GOLD, (0, 0, zm), 264, 316, rot=rot, steps=12)
        g.cyl(NAVY, (0, 0, zm), 236, 324, rot=rot, steps=12)
    for k in range(12):
        a = math.radians(30 * k)
        t = 34.0 if k % 3 == 0 else 20.0
        g.box(W, (math.sin(a) * 196, -328, zm + math.cos(a) * 196), (t, 8, t))
        if k % 3 == 0:
            for s in (-1, 1):
                g.box(W, (s * 328, math.sin(a) * 196, zm + math.cos(a) * 196), (8, t, t))
    tilt_box(g, GOLD, (0, -332, zm), (96, -332, zm + 70), 10, 22)
    tilt_box(g, GOLD, (0, -332, zm), (-60, -332, zm + 172), 10, 16)
    for s in (-1, 1):
        tilt_box(g, GOLD, (s * 332, 0, zm), (s * 332, 60, zm + 172), 16, 10)
        tilt_box(g, GOLD, (s * 332, 0, zm), (s * 332, -96, zm + 70), 22, 10)
    cornice(g, 0, 0, zc + 620, 620, 620)
    zb = zc + 646.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            bx(g, W, sx * 240, sy * 240, zb, 70, 70, 300)
    bx(g, W, 0, 0, zb + 300, 600, 600, 50)
    cornice(g, 0, 0, zb + 350, 600, 600)
    bx(g, GOLD, 0, 0, zb + 270, 420, 14, 14)                               # the bell beam
    for x, r, h, ph, sw, rate in ((0, 110, 170, 0.0, 15, 0.45), (-150, 62, 100, 0.35, 21, 0.7), (150, 62, 100, 0.7, 21, 0.62)):
        b = sc.part(P.motion(swing=sw, pivot=(x, 0, zb + 268), axis=0.0, rate=rate, phase=ph))
        b.cyl(GOLD, (x, 0, zb + 262 - h), r, h, steps=8, top_r=r * 0.38)
        b.sphere(CRIM, (x, 0, zb + 256 - h), r * 0.3, steps=(3, 5))
    z = stepped(g, CRIM, 0, 0, zb + 376, 620, 620, n=3, h=80, shrink=0.66)
    g.cyl(GOLD, (0, 0, z), 9, 150, steps=5)
    v = sc.part(P.motion(swing=24, pivot=(0, 0, z + 120), axis=P.UP, rate=0.12, phase=0.3))
    v.box(GOLD, (20, 0, z + 120), (190, 8, 12))
    v.side(GOLD, [(-150, z + 92), (-75, z + 120), (-150, z + 148)], -4, 4)
    g.sphere(GOLD, (0, 0, z + 165), 18, steps=(3, 5))


def belvedere(sc):
    """The belvedere: a round temple of eight columns round a glass core, urns on its ring, an upper drum of
    windows and a ribbed turquoise dome with a lantern. 8 m across, 16 m."""
    g = sc.still()
    g.plan(SHADE, ngon(400, 12), -FOOT, 40)
    g.plan(W, ngon(372, 12), 40, 90)
    bx(g, CRIM, 0, -330, 90, 150, 200, 8)
    bx(g, W, 0, -420, -60, 260, 90, 105)
    g.cyl(TURQ, (0, 0, 90), 215, 620, steps=8, glow=0.15)
    for z in (300, 520):
        g.cyl(GOLD, (0, 0, z), 221, 14, steps=8)
    for k in range(8):
        a = math.radians(22.5 + 45 * k)
        column(g, math.cos(a) * 312, math.sin(a) * 312, 90, 620, r=26, steps=5)
        urn(g, math.cos(a) * 330, math.sin(a) * 330, 804, 1.1, steps=5)
        b = math.radians(45 * k)
        g.box(W, (math.cos(b) * 272, math.sin(b) * 272, 934), (26, 36, 130), rot=(0, 45 * k, 0))
    g.plan(W, ngon(388, 12), 710, 780)
    g.plan(GOLD, ngon(408, 12), 780, 804)
    g.cyl(W, (0, 0, 804), 262, 260, steps=10)
    g.cyl(NAVY, (0, 0, 874), 268, 120, steps=10)
    g.cyl(GOLD, (0, 0, 1064), 286, 22, steps=10)
    dome(g, 0, 0, 1086, 276, squash=0.95, steps=(6, 10), glow=0.15, top=False)
    ribs(g, 0, 0, 1086, 276, 0.95, 8)
    g.cyl(W, (0, 0, 1340), 52, 100, steps=6)
    dome(g, 0, 0, 1440, 60, squash=1.0, steps=(3, 6))


# ------------------------------------------------------------------------------------------ the props
def lamp_standard(sc):
    """TALL prop: a gilded lamp standard with five cream globes, steady. 7.6 m."""
    g = sc.still()
    bx(g, W, 0, 0, -60, 110, 110, 100)
    bx(g, GOLD, 0, 0, 40, 70, 70, 50)
    g.cyl(GOLD, (0, 0, 90), 11, 480, steps=6, top_r=8)
    g.cyl(GOLD, (0, 0, 330), 20, 26, steps=6)
    for k in range(4):
        a = math.radians(45 + 90 * k)
        cx, cy = math.cos(a), math.sin(a)
        tilt_box(g, GOLD, (cx * 6, cy * 6, 470), (cx * 112, cy * 112, 520), 10, 10)
        tilt_box(g, GOLD, (cx * 108, cy * 108, 516), (cx * 130, cy * 130, 592), 10, 10)
        g.sphere(W, (cx * 130, cy * 130, 628), 40, steps=(4, 6), glow=0.85)
    g.cyl(GOLD, (0, 0, 560), 9, 112, steps=5)
    g.sphere(W, (0, 0, 716), 46, steps=(4, 6), glow=0.85)
    for s in (-1, 1):                                                     # a crimson banner each side of the shaft
        bx(g, GOLD, s * 46, 0, 420, 78, 8, 8)
        bx(g, CRIM, s * 52, 0, 270, 56, 6, 150)
        bx(g, GOLD, s * 52, 0, 258, 56, 8, 12)


def topiary(sc):
    """TALL prop: a pair of spiral topiaries in gilded tubs on a marble kerb, a crimson mat between. 5 m."""
    g = sc.still()
    bx(g, W, 0, 0, -50, 520, 160, 62)
    bx(g, CRIM, 0, 0, 12, 190, 120, 8)
    for s in (-1, 1):
        x = s * 172
        g.cyl(GOLD, (x, 0, 12), 52, 80, steps=6, top_r=68)
        g.cyl(GOLD, (x, 0, 92), 74, 12, steps=6)
        g.cyl(TRUNK, (x, 0, 100), 9, 360, steps=5)
        for k in range(6):
            a = math.radians(k * 70 * s)
            r = 80.0 - k * 10.0
            g.sphere(LEAF if k % 2 else LEAF_L, (x + math.cos(a) * r * 0.4, math.sin(a) * r * 0.4, 150 + k * 54), r, scale=(1, 1, 0.55), steps=(3, 6))
        g.sphere(LEAF_L, (x, 0, 476), 30, steps=(3, 6))


def urn_balustrade(sc):
    """TALL prop: a balustrade's corner pier carrying a giant gilded urn of clipped green and blossom. 4.6 m."""
    g = sc.still()
    bx(g, SHADE, 20, 20, -50, 560, 200, 62)
    bx(g, W, -200, 0, 12, 112, 112, 150)
    bx(g, GOLD, -200, 0, 162, 132, 132, 16)
    balustrade(g, -144, 222, 0, 12, n=4)
    bx(g, W, 250, 0, 12, 60, 60, 112)
    g.sphere(GOLD, (250, 0, 140), 22, steps=(3, 5))
    g.box(W, (-200, 62, 89), (24, 110, 14))
    g.box(W, (-200, 62, 18), (24, 110, 12))
    urn(g, -200, 0, 178, 2.2, steps=8)
    for s in (-1, 1):
        tilt_box(g, GOLD, (-200 + s * 60, 0, 300), (-200 + s * 118, 0, 352), 12, 12)
        tilt_box(g, GOLD, (-200 + s * 118, 0, 348), (-200 + s * 84, 0, 392), 12, 12)
    g.sphere(LEAF, (-200, 0, 404), 78, scale=(1, 1, 0.62), steps=(4, 6))
    for k in range(4):
        a = math.radians(40 + 90 * k)
        g.sphere(W if k % 2 else CRIM, (-200 + math.cos(a) * 46, math.sin(a) * 46, 436), 18, steps=(3, 5))


def flag_court(sc):
    """TALL prop: three gilded masts on a round marble dais, crimson and gold pennants swinging. 10.4 m."""
    g = sc.still()
    g.plan(W, ngon(240, 10), -50, 14)
    g.plan(CRIM, ngon(176, 10), 14, 22)
    g.plan(GOLD, ngon(70, 8), 22, 40)
    for k, (x, h) in enumerate(((-150, 800), (0, 1000), (150, 800))):
        bx(g, GOLD, x, 0, 22, 44, 44, 40)
        pennant(sc, g, x, 0, 22, h, L=250 if k == 1 else 210, fh=84 if k == 1 else 70, phase=0.31 * k, swing=15,
                col=GOLD if k == 1 else CRIM)


def gate_arch(sc):
    """TALL prop: a gilded garden gate between two marble piers with urns, a scrolled arch and an orb over it. 8.6 m."""
    g = sc.still()
    bx(g, SHADE, 0, 0, -50, 760, 150, 62)
    for s in (-1, 1):
        bx(g, W, s * 300, 0, 12, 120, 120, 508)
        bx(g, GOLD, s * 300, 0, 520, 146, 146, 22)
        urn(g, s * 300, 0, 542, 1.25)
        for k in range(5):
            bx(g, GOLD, s * (34 + 46 * k), 0, 30, 8, 8, 400 + 16 * (4 - k))
    for z in (64, 408):
        g.box(GOLD, (0, 0, z), (480, 10, 12))
    for r, n, w in ((240.0, 6, 18.0), (150.0, 4, 12.0)):
        for k in range(n):
            a0, a1 = math.pi * k / n, math.pi * (k + 1) / n
            tilt_box(g, GOLD, (math.cos(a0) * r, 0, 520 + math.sin(a0) * r), (math.cos(a1) * r, 0, 520 + math.sin(a1) * r), w, w)
    for s in (-1, 1):
        tilt_box(g, GOLD, (s * 150, 0, 520), (s * 240, 0, 520), 12, 12)
    bx(g, CRIM, 0, 0, 676, 90, 14, 80)
    g.sphere(GOLD, (0, 0, 810), 44, steps=(4, 6))
    g.cyl(GOLD, (0, 0, 756), 9, 20, steps=5)


def victory_column(sc):
    """TALL prop: a marble column on a stepped base carrying a ringed gilded orb, two crimson ribbons stirring. 11.4 m."""
    g = sc.still()
    bx(g, SHADE, 0, 0, -50, 300, 300, 80)
    bx(g, W, 0, 0, 30, 220, 220, 92)
    bx(g, GOLD, 0, 0, 122, 232, 232, 14)
    g.plan(W, ngon(62, 8, a0=math.pi / 8.0), 136, 900)
    for z in (300, 520, 740):
        g.plan(GOLD, ngon(68, 8, a0=math.pi / 8.0), z, z + 14)
    bx(g, GOLD, 0, 0, 900, 176, 176, 40)
    g.cyl(GOLD, (0, 0, 940), 34, 30, steps=6)
    g.sphere(GOLD, (0, 0, 1046), 82, steps=(5, 8))
    g.plan(GOLD, ngon(138, 10), 1040, 1052)
    for s in (-1, 1):
        f = sc.part(P.motion(swing=6, pivot=(0, s * 92, 900), axis=0.0, rate=0.3, phase=0.25 + 0.25 * s))
        f.box(CRIM, (0, s * 92, 760), (130, 6, 280))
        f.box(GOLD, (0, s * 92, 612), (130, 8, 18))


def pool_edge(sc):
    """LOW prop: the end of a reflecting pool: a marble kerb round turquoise water, three small jets, an urn at each end. 1.7 m."""
    g = sc.still()
    bx(g, W, 0, 0, -40, 900, 220, 80)
    bx(g, TURQ, 0, 0, 34, 760, 150, 10)
    for x in (-250, 0, 250):
        g.cyl(W, (x, 0, 44), 14, 66, steps=5, top_r=5)
        g.cyl(GOLD, (x, 0, 40), 24, 10, steps=5)
    for s in (-1, 1):
        bx(g, W, s * 415, 0, 40, 70, 70, 36)
        urn(g, s * 415, 0, 76, 1.0)


def rope_line(sc):
    """LOW prop: a crimson runner between two rows of gilded posts and ropes, a doorman's desk at its end. 1.2 m."""
    g = sc.still()
    bx(g, CRIM, -40, 0, -30, 800, 130, 38)
    for sy in (-1, 1):
        for k in range(5):
            x = -400 + k * 180
            g.cyl(GOLD, (x, sy * 82, -30), 6, 126, steps=5)
            g.cyl(GOLD, (x, sy * 82, 0), 14, 8, steps=5)
            g.sphere(GOLD, (x, sy * 82, 104), 12, steps=(3, 5))
            if k < 4:
                tilt_box(g, CRIM, (x, sy * 82, 90), (x + 90, sy * 82, 70), 7, 7)
                tilt_box(g, CRIM, (x + 90, sy * 82, 70), (x + 180, sy * 82, 90), 7, 7)
    bx(g, W, 440, 0, -30, 76, 64, 140)
    bx(g, GOLD, 440, 0, 110, 90, 78, 10)
    bx(g, CRIM, 440, -34, 24, 52, 6, 62)


# ------------------------------------------------------------------------------------------ the hero
TIER_R = (700.0, 600.0, 505.0, 415.0, 330.0)
TIER_H = (980.0, 860.0, 750.0, 650.0, 560.0)


def crown_tower(sc):
    """THE CROWN TOWER: a white fluted tower of five set-backs on a square podium with four domed turrets, carrying
    a giant gold crown: twelve points, six arches, a crimson cushion, turquoise jewels that glow steadily."""
    g = sc.still()
    bx(g, SHADE, 0, 0, -300, 1900, 1900, 340)
    bx(g, W, 0, 0, 40, 1700, 1700, 380)
    band(g, 0, 0, 250, 1700, 1700, 130)
    piers(g, 0, 0, 40, 1700, 1700, 380, 6, pw=60, sides=4)
    cornice(g, 0, 0, 420, 1700, 1700, h=36, out=30)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.plan(W, ngon(150, 8, sx * 760, sy * 760), 456, 620)
            dome(g, sx * 760, sy * 760, 620, 150, steps=(3, 6), top=False)
    doors(g, 0, -850, 40, 340, 300)
    bx(g, CRIM, 0, -900, 40, 300, 110, 8)
    z = 456.0
    for k in range(5):
        r, h = TIER_R[k], TIER_H[k]
        g.plan(W, ngon(r, 12), z, z + h)
        for j in range(12):
            a = 30.0 * j
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            g.box(W, (ca * (r - 4), sa * (r - 4), z + h / 2.0), (44, 64, h), rot=(0, a, 0))          # a flute's rib on each corner
            b = a + 15.0
            cb, sb = math.cos(math.radians(b)), math.sin(math.radians(b))
            rc = r * 0.9659 - 6.0
            g.box(NAVY, (cb * rc, sb * rc, z + h * 0.52), (24, r * 0.2, h * 0.66), rot=(0, b, 0))     # a tall window between
        g.plan(GOLD, ngon(r + 46, 12), z + h, z + h + 34)
        z += h + 34.0
    g.cyl(W, (0, 0, z), 330, 170, steps=12, top_r=500)
    z += 170.0
    g.plan(W, ngon(548, 12), z, z + 70)
    z += 70.0
    g.plan(GOLD, ngon(520, 12), z, z + 210)
    for j in range(12):
        b = 30.0 * j + 15.0
        cb, sb = math.cos(math.radians(b)), math.sin(math.radians(b))
        if j % 2 == 0:
            g.box(TURQ, (cb * 502, sb * 502, z + 105), (36, 96, 120), rot=(0, b, 0), glow=1.0)
        else:
            g.box(CRIM, (cb * 502, sb * 502, z + 105), (30, 80, 80), rot=(0, b, 0))
    zb = z + 210.0
    for j in range(12):
        a = math.radians(30.0 * j)
        ca, sa = math.cos(a), math.sin(a)
        if j % 2 == 0:
            g.cyl(GOLD, (ca * 485, sa * 485, zb), 80, 360, steps=4, top_r=14)
            g.sphere(TURQ, (ca * 485, sa * 485, zb + 396), 52, steps=(4, 6), glow=1.0)
            pts = ((470, 40), (420, 420), (250, 640), (40, 700))
            for i in range(3):
                tilt_box(g, GOLD, (ca * pts[i][0], sa * pts[i][0], zb + pts[i][1]), (ca * pts[i + 1][0], sa * pts[i + 1][0], zb + pts[i + 1][1]), 56, 44)
        else:
            g.cyl(GOLD, (ca * 485, sa * 485, zb), 60, 190, steps=4, top_r=12)
            g.sphere(GOLD, (ca * 485, sa * 485, zb + 214), 34, steps=(3, 5))
    g.sphere(CRIM, (0, 0, zb + 170), 430, scale=(1, 1, 0.8), steps=(5, 10))
    g.sphere(GOLD, (0, 0, zb + 740), 90, steps=(4, 8))
    g.cyl(GOLD, (0, 0, zb + 820), 10, 40, steps=5)
    g.sphere(TURQ, (0, 0, zb + 900), 60, steps=(4, 6), glow=1.0)


JOBS = [
    ('SM_S1D8_CarriagePorch', carriage_porch, SMALL),
    ('SM_S1D8_HotelTower', hotel_tower, STILL),
    ('SM_S1D8_WinterGarden', winter_garden, STILL),
    ('SM_S1D8_Arcade', arcade, STILL),
    ('SM_S1D8_Theatre', theatre, STILL),
    ('SM_S1D8_ClockPavilion', clock_pavilion, SMALL),
    ('SM_S1D8_Belvedere', belvedere, STILL),
    ('SM_S1D8_LampStandard', lamp_standard, STILL),
    ('SM_S1D8_Topiary', topiary, STILL),
    ('SM_S1D8_UrnBalustrade', urn_balustrade, STILL),
    ('SM_S1D8_FlagCourt', flag_court, dict(grow=(260, 260, 20), grow_down=(260, 260, 0))),
    ('SM_S1D8_GateArch', gate_arch, STILL),
    ('SM_S1D8_VictoryColumn', victory_column, SMALL),
    ('SM_S1D8_PoolEdge', pool_edge, STILL),
    ('SM_S1D8_RopeLine', rope_line, STILL),
    ('SM_S1D8_CrownTower', crown_tower, STILL),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
