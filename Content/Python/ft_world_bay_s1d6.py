"""Bayshore, Stage 1, builder s1d6: district 6 "Downtown waterfront" (16.8 to 21.1 km). /Game/World/Bayshore/SM_S1D6_*.
Night-out downtown, the level's only DARK district: navy and deep violet masses, asphalt podiums, grids of lit yellow
windows, hot pink and pool turquoise neon drawn along every roof line, neon OUTLINES on roof frames (shapes, never
words), bright canopies, a skybridge over the side street. The hero is THE PICTURE PALACE: a stepped front like a pipe
organ, a 30 m neon fan marquee and a crown of five stars that turns slowly.
Placed by the block C:/ftwork/s1/L0_s1d6.inl. Run: import ft_world_bay_s1d6 as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/Bayshore'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))        # a piece with no moving part: true bounds
SMALL = dict(grow=(25, 25, 25), grow_down=(25, 25, 0))   # a piece whose moving parts stay close to it

NAVY, VIOLET, HOT, TURQ, YEL, ASPH = '#2b2350', '#3a2f6a', '#ff4fb8', '#49e2d5', '#ffe14d', '#4b4668'   # the district's six
WHITE, CREAM, STEEL, LAV, PINK, SKY, MINT, LEAF, CORAL = '#fffaf2', '#fff1d6', '#c9c4d6', '#b9a4ff', '#ff9ec7', '#8fe3ff', '#7fe3c4', '#2fbf71', '#ff7a8a'
PASTEL = (PINK, MINT, SKY, LAV, CREAM, CORAL, WHITE)     # parked cars: small and pale, never dark
FOOT = 300.0                                             # how far a building's footing goes down
GN, GW, GB = 0.5, 0.3, 0.6                               # glow: neon tubes, lit windows, bulbs (steady; every piece that glows stands 24 m out or more)


# ------------------------------------------------------------------------------------------ helpers
def B(g, col, x0, x1, y0, y1, z0, z1, glow=0.0):
    """A box by its corners."""
    g.box(col, ((x0 + x1) / 2.0, (y0 + y1) / 2.0, (z0 + z1) / 2.0), (abs(x1 - x0), abs(y1 - y0), abs(z1 - z0)), glow=glow)


def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def arc(cx, cz, r, a0, a1, n):
    return [(cx + r * math.cos(math.radians(a0 + (a1 - a0) * k / float(n))), cz + r * math.sin(math.radians(a0 + (a1 - a0) * k / float(n)))) for k in range(n + 1)]


def tube(g, col, pts, y, t=16.0, w=14.0, glow=GN, close=False):
    """A neon tube along the points [(x, z)], in the plane y (it faces the road). t: its thickness; w: its depth."""
    pts = list(pts) + ([pts[0]] if close else [])
    for (x0, z0), (x1, z1) in zip(pts, pts[1:]):
        L = math.hypot(x1 - x0, z1 - z0) or 1.0
        ex, ez = (x1 - x0) / L * t * 0.4, (z1 - z0) / L * t * 0.4
        tilt_box(g, col, (x0 - ex, y, z0 - ez), (x1 + ex, y, z1 + ez), w, t, glow=glow)


def fan(g, col, x, zc, r0, r1, y0, y1, n=10, a0=0.0, a1=180.0, glow=0.0):
    """A flat piece of a ring facing the road, centre (x, zc), from radius r0 to r1 (r0 = 0: a half disc)."""
    pts = arc(x, zc, r1, a0, a1, n)
    if r0 > 0.0:
        pts += list(reversed(arc(x, zc, r0, a0, a1, n)))
    g.side(col, ccw(pts), y0, y1, glow=glow)


def star_pts(r, a0=90.0):
    return [((r if k % 2 == 0 else r * 0.45) * math.cos(math.radians(a0 + k * 36.0)), (r if k % 2 == 0 else r * 0.45) * math.sin(math.radians(a0 + k * 36.0))) for k in range(10)]


def star(g, col, x, y, z, r, t=14.0, glow=0.0):
    """A five-pointed star facing the road."""
    g.side(col, ccw([(x + px, z + pz) for px, pz in star_pts(r)]), y - t / 2.0, y + t / 2.0, glow=glow)


def disc(g, col, x, y, z, r, t=8.0, steps=10, glow=0.0):
    """A disc facing the road: its front face at y, t thick away from the road."""
    g.cyl(col, (x, y, z), r, t, rot=(0, 0, 90), steps=steps, glow=glow)


def disc_x(g, col, x, y, z, r, t=8.0, steps=10, glow=0.0):
    """A disc facing along the road, centred on x, t thick."""
    g.cyl(col, (x + t / 2.0, y, z), r, t, rot=(90, 0, 0), steps=steps, glow=glow)


def wb(g, face, col, a0, a1, wall, proud, z0, z1, glow=0.0):
    """A slab on a wall. face 'f': the wall y = wall that faces the road (a = x); 'b': it faces away; 'l': the wall
    x = wall that faces -X (a = y); 'r': it faces +X. proud: how far it stands out of the wall."""
    s = -1.0 if face in 'fl' else 1.0
    lo, hi = sorted((wall, wall + s * proud))
    if face in 'fb':
        B(g, col, a0, a1, lo, hi, z0, z1, glow)
    else:
        B(g, col, lo, hi, a0, a1, z0, z1, glow)


def grid(g, face, a0, a1, wall, z0, z1, n, rows, lit=YEL, frame=NAVY, glow=GW, dark=0, seed=1, alt=(), mw=0.3, sh=0.36):
    """A grid of lit windows: one lit pane, the frame's piers and spandrels over it, `dark` windows unlit and
    alt = [(colour, how many, glow)] lit another colour. Separate windows, never stripes."""
    r = random.Random(seed)
    cw, ch = (a1 - a0) / float(n), (z1 - z0) / float(rows)
    m, s = cw * mw, ch * sh
    wb(g, face, lit, a0, a1, wall, 6, z0, z1, glow)
    for k in range(1, n):
        wb(g, face, frame, a0 + k * cw - m / 2.0, a0 + k * cw + m / 2.0, wall, 12, z0, z1)
    for k in range(1, rows):
        wb(g, face, frame, a0, a1, wall, 12, z0 + k * ch - s / 2.0, z0 + k * ch + s / 2.0)
    cells = [(i, j) for i in range(n) for j in range(rows)]
    r.shuffle(cells)
    k = 0
    for col, cnt, gl in [(VIOLET if frame != VIOLET else NAVY, dark, 0.0)] + list(alt):
        for i, j in cells[k:k + cnt]:
            wb(g, face, col, a0 + i * cw + (m / 2.0 if i > 0 else 0.0), a0 + (i + 1) * cw - (m / 2.0 if i < n - 1 else 0.0), wall, 9,
               z0 + j * ch + (s / 2.0 if j > 0 else 0.0), z0 + (j + 1) * ch - (s / 2.0 if j < rows - 1 else 0.0), gl)
        k += cnt


def roofline(g, col, w, d, z, front=True, sides=True, glow=GN, t=16.0):
    """A neon line along the edge of a roof w x d (centred), at height z."""
    if front:
        B(g, col, -w / 2.0 - 8, w / 2.0 + 8, -d / 2.0 - 10, -d / 2.0 + 6, z, z + t, glow)
    if sides:
        for sx in (-1, 1):
            B(g, col, sx * w / 2.0 - 8, sx * w / 2.0 + 8, -d / 2.0 - 10, d / 2.0, z, z + t, glow)


def awning(g, x0, x1, y, z, cols, depth=140.0, drop=50.0, n=None):
    """A striped awning hung on the wall at y, falling towards the road."""
    n = n or max(2, int(round((x1 - x0) / 110.0)))
    w = (x1 - x0) / float(n)
    roll = -math.degrees(math.atan2(drop, depth))
    for k in range(n):
        g.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y - depth / 2.0, z - drop / 2.0), (w, math.hypot(depth, drop), 10), rot=(0, 0, roll))


def frame(g, x0, x1, y, z0, z1, back=260.0):
    """A steel roof frame that carries a neon outline: two posts, two rails, a brace behind each post."""
    for x in (x0, x1):
        B(g, STEEL, x - 7, x + 7, y - 7, y + 7, z0, z1)
        tilt_box(g, STEEL, (x, y, z0 + (z1 - z0) * 0.7), (x, y + back, z0), 10, 10)
    for z in (z0 + 40, z1 - 8):
        B(g, STEEL, x0, x1, y - 6, y + 6, z - 6, z + 6)


def bulbs(g, a, b, n, cols=(HOT, TURQ, YEL), size=20.0):
    """A string of party lights from a to b (steady)."""
    tilt_box(g, STEEL, a, b, 5, 5)
    for k in range(n):
        t = (k + 0.5) / float(n)
        g.box(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t - 13), (size, size, size), glow=GB)


def parasol(g, x, y, z, col, r=120.0, h=230.0):
    g.cyl(STEEL, (x, y, z), 5, h, steps=5)
    g.cyl(col, (x, y, z + h - 44), r, 46, steps=8, top_r=8)
    g.cyl(WHITE, (x, y, z + 72), 42, 6, steps=8)


def tub(g, x, y, z, r=46.0, col=LEAF, pot=ASPH):
    """A clipped cone in a square tub (a cone, not a ball: no green ball the size of a pickup anywhere)."""
    B(g, pot, x - r * 0.8, x + r * 0.8, y - r * 0.8, y + r * 0.8, z, z + r * 0.9)
    g.cyl(col, (x, y, z + r * 0.9), r * 0.8, r * 2.8, steps=6, top_r=r * 0.12)


def vent(g, x, y, z):
    B(g, STEEL, x - 45, x + 45, y - 40, y + 40, z, z + 60)
    B(g, TURQ, x - 52, x + 52, y - 47, y + 47, z + 60, z + 70)


def hut(g, x0, x1, y0, y1, z, h=230.0, col=VIOLET):
    """The stair head on a roof, its lit door towards the road."""
    B(g, col, x0, x1, y0, y1, z, z + h)
    B(g, YEL, (x0 + x1) / 2.0 - 40, (x0 + x1) / 2.0 + 40, y0 - 6, y0, z, z + 190, 0.25)


def cowl(sc, x, y, z, phase=0.0):
    """A roof vent whose cowl turns to and fro in the wind."""
    sc.still().cyl(STEEL, (x, y, z), 18, 60, steps=6)
    c = sc.part(P.motion(swing=55, pivot=(x, y, z + 60), axis=P.UP, rate=0.12, phase=phase))
    c.box(STEEL, (x + 12, y, z + 76), (70, 40, 34))
    c.box(HOT, (x - 30, y, z + 92), (34, 5, 30))


def pennant(sc, x, y, z, col, w=120.0, h=54.0, phase=0.0, rate=0.4, swing=16.0, mast=0.0):
    """A pennant that swings about its mast (mast: cm of mast drawn below it)."""
    if mast:
        sc.still().cyl(STEEL, (x, y, z - mast), 5, mast + h / 2.0 + 6, steps=5)
        sc.still().sphere(YEL, (x, y, z + h / 2.0 + 14), 9, steps=(3, 5))
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + w / 2.0 + 5, y, z), (w, 5, h))


def car(g, x, y, z, col, across=False, band=None):
    """A small boxy parked car, 4.2 x 1.8 m, pale; across: it stands nose to the kerb."""
    def b(c, dx, z0, z1, sx, sy):
        if across:
            B(g, c, x - sy / 2.0, x + sy / 2.0, y + dx - sx / 2.0, y + dx + sx / 2.0, z + z0, z + z1)
        else:
            B(g, c, x + dx - sx / 2.0, x + dx + sx / 2.0, y - sy / 2.0, y + sy / 2.0, z + z0, z + z1)
    b(col, 0, 28, 92, 420, 176)
    b(NAVY, -15, 92, 136, 220, 160)
    b(col, -15, 136, 144, 200, 166)
    for dx in (-135, 135):
        b(ASPH, dx, 0, 56, 70, 184)
    if band:
        b(band, 0, 40, 58, 424, 180)


def doorway(g, x, y, w=220.0, h=290.0, frame_col=VIOLET):
    """A lit pair of doors in a wall at y that faces the road."""
    B(g, frame_col, x - w / 2.0 - 30, x + w / 2.0 + 30, y - 10, y, 0, h + 34)
    B(g, YEL, x - w / 2.0, x + w / 2.0, y - 16, y - 8, 0, h, 0.25)
    B(g, NAVY, x - 5, x + 5, y - 21, y - 14, 0, h)


# ------------------------------------------------------------------------------------------ buildings by the side street: the front row
def night_club(sc):
    """A night club, 15 x 11 m, 12.4 m to the roof and 19.6 to the top of its sign: an asphalt podium, a navy box with
    no windows but pink neon fins, a pink canopy, and a cocktail glass drawn in neon on a roof frame."""
    g = sc.still()
    B(g, ASPH, -750, 750, -550, 550, -FOOT, 420)
    B(g, NAVY, -720, 720, -520, 520, 420, 1200)
    B(g, VIOLET, -750, 750, -550, 550, 1200, 1236)
    roofline(g, TURQ, 1500, 1100, 1236)
    doorway(g, 0, -550, 380, 300)
    B(g, HOT, -380, 380, -860, -550, 344, 380)                                   # the canopy
    B(g, TURQ, -388, 388, -870, -856, 338, 386, GN)
    for sx in (-1, 1):
        g.cyl(STEEL, (sx * 340, -830, 0), 7, 344, steps=6)
        tub(g, sx * 470, -630, 0)
        for x in (420, 580):                                                     # portholes in the podium
            disc(g, WHITE, sx * x, -558, 240, 66, 8)
            disc(g, TURQ, sx * x, -564, 240, 50, 6, glow=GW)
        for k in range(3):                                                       # neon fins up the blind wall
            B(g, HOT, sx * (380 + k * 120) - 9, sx * (380 + k * 120) + 9, -536, -520, 520, 1120, GN)
        for y in (-700, -850):                                                   # rope posts by the carpet
            g.cyl(YEL, (sx * 150, y, 0), 6, 95, steps=6)
        tilt_box(g, TURQ, (sx * 150, -700, 86), (sx * 150, -850, 86), 6, 6)
        grid(g, 'l' if sx < 0 else 'r', -380, 380, sx * 720, 560, 1080, 3, 2, lit=HOT, glow=0.25, dark=2, seed=11 + sx, alt=[(TURQ, 1, GW)])
        B(g, YEL, sx * 750 - 6, sx * 750 + 6, 150, 260, 0, 230, 0.25)           # the side door
    B(g, HOT, -110, 110, -900, -550, 0, 6)                                       # the carpet
    tube(g, TURQ, arc(0, 800, 200, 0, 360, 12), -530)                            # a ring and a star over the door
    star(g, HOT, 0, -532, 800, 120, glow=GN)
    frame(g, -330, 330, -300, 1236, 1960)
    y = -318
    tube(g, HOT, [(-250, 1880), (0, 1560), (250, 1880)], y, t=20)                # the glass
    tube(g, TURQ, [(-275, 1880), (275, 1880)], y, t=20)
    tube(g, HOT, [(0, 1560), (0, 1350)], y, t=20)
    tube(g, HOT, [(-130, 1340), (130, 1340)], y, t=20)
    tube(g, TURQ, [(-150, 1750), (150, 1750)], y, t=12)
    tube(g, TURQ, [(70, 1740), (215, 1950)], y - 6, t=12)
    disc(g, YEL, 96, y - 16, 1784, 36, 8, glow=GN)
    for bx, bz, br in ((-70, 1690, 18), (-10, 1640, 14), (40, 1700, 12)):
        disc(g, YEL, bx, y - 10, bz, br, 6, steps=8, glow=GN)
    hut(g, 300, 620, 100, 420, 1236)
    vent(g, -520, 250, 1236)
    cowl(sc, -560, -150, 1236, 0.2)
    for x in (-700, 220):
        g.cyl(STEEL, (x, 420, 1236), 5, 230, steps=5)
    bulbs(g, (-700, 420, 1460), (220, 420, 1460), 8)
    pennant(sc, 330, -300, 2020, HOT, mast=70, phase=0.1)


def theatre(sc):
    """A theatre, 16 x 12 m, 13 m to its stepped parapet, a fly tower behind to 15 m: a deep canopy edged with steady
    bulbs, three lit doors, a fan window, a blade of turquoise discs, a fire escape, a star vane that turns."""
    g = sc.still()
    B(g, ASPH, -812, 812, -612, 612, -FOOT, 60)
    B(g, VIOLET, -800, 800, -600, 600, 60, 1000)
    roofline(g, HOT, 1600, 1200, 1000)
    z = 1016
    for w, h in ((1200, 90), (800, 90), (400, 110)):
        B(g, NAVY, -w / 2.0, w / 2.0, -600, -470, z, z + h)
        z += h
    g.cyl(STEEL, (0, -535, z), 6, 70, steps=5)
    star(g, YEL, 0, -535, z + 170, 110, glow=GN)
    B(g, NAVY, -600, 600, 150, 600, 1000, 1500)                                  # the fly tower
    B(g, TURQ, -608, 608, 140, 156, 1500, 1516, GN)
    B(g, NAVY, -560, 560, -1000, -600, 400, 470)                                 # the canopy
    B(g, HOT, -568, 568, -1010, -1000, 392, 478)
    B(g, CREAM, -540, 540, -990, -610, 393, 400, GW)
    for k in range(11):
        B(g, YEL, -530 + k * 106 - 14, -530 + k * 106 + 14, -1022, -1008, 380, 408, GB)
    for sx in (-1, 1):
        for k in range(3):
            B(g, YEL, sx * 574 - 7, sx * 574 + 7, -950 + k * 130 - 14, -950 + k * 130 + 14, 380, 408, GB)
        tilt_box(g, STEEL, (sx * 500, -960, 470), (sx * 500, -600, 760), 8, 8)
        B(g, WHITE, sx * 650 - 100, sx * 650 + 100, -610, -600, 90, 330)        # a poster case: a plain colour, no words
        B(g, PINK if sx < 0 else TURQ, sx * 650 - 84, sx * 650 + 84, -616, -608, 106, 314)
        grid(g, 'f', sx * 540 - 180, sx * 540 + 180, -600, 540, 920, 2, 2, frame=VIOLET, dark=1, seed=3 + sx)
        grid(g, 'l' if sx < 0 else 'r', -480, 60, sx * 800, 180, 900, 3, 2, frame=VIOLET, dark=2, seed=5 + sx)
        pennant(sc, sx * 700, -560, 1150, TURQ if sx < 0 else HOT, mast=130, phase=0.3 + sx * 0.2)
    for x in (-330, 0, 330):
        doorway(g, x, -600, 220, 270, NAVY)
    B(g, YEL, -260, 260, -606, -598, 520, 700, GW)                               # the fan window over the canopy
    fan(g, YEL, 0, 700, 0, 260, -606, -598, n=8, glow=GW)
    for a in (45, 90, 135):
        tube(g, NAVY, [(0, 700), (255 * math.cos(math.radians(a)), 700 + 255 * math.sin(math.radians(a)))], -606, t=14, w=10, glow=0.0)
    B(g, NAVY, -260, 260, -612, -604, 692, 708)
    B(g, HOT, 716, 776, -720, -600, 500, 1250)                                   # the blade
    for k in range(5):
        disc_x(g, TURQ, 746, -660, 590 + k * 145, 36, 76, steps=8, glow=GN)
    for z in (330, 640):                                                         # the fire escape on the left wall
        B(g, STEEL, -900, -800, -220, 220, z, z + 10)
        B(g, STEEL, -900, -892, -220, 220, z + 10, z + 100)
    tilt_box(g, STEEL, (-850, 200, 335), (-850, -200, 645), 60, 10)
    tilt_box(g, STEEL, (-850, -200, 0), (-850, 200, 335), 60, 10)
    vent(g, -300, -100, 1000)
    vent(g, 420, -200, 1000)
    cowl(sc, -620, -250, 1000, 0.6)
    g.cyl(STEEL, (300, 400, 1500), 6, 170, steps=5)
    v = sc.part(P.motion(swing=25, pivot=(300, 400, 1660), axis=P.UP, rate=0.08, phase=0.4))
    v.box(TURQ, (300, 400, 1672), (220, 8, 8))
    v.box(TURQ, (205, 400, 1672), (40, 8, 46))
    v.side(HOT, ccw([(390 + px, 1672 + pz) for px, pz in star_pts(46)]), 395, 405)


def dept_store(sc):
    """A department store, 16 x 12 m, 16.5 m: show windows under a long pink and cream awning, three floors of lit
    windows, a revolving door that turns, pennants, a shopping bag drawn in neon on the roof."""
    g = sc.still()
    B(g, ASPH, -800, 800, -600, 600, -FOOT, 420)
    B(g, NAVY, -780, 780, -580, 580, 420, 1650)
    B(g, VIOLET, -800, 800, -600, 600, 1650, 1690)
    roofline(g, HOT, 1600, 1200, 1690)
    B(g, TURQ, -790, 790, -592, -580, 420, 440, GN)
    for sx in (-1, 1):
        grid(g, 'f', sx * 450 - 270, sx * 450 + 270, -600, 60, 330, 2, 1, frame=ASPH, mw=0.12)
        B(g, HOT if sx < 0 else TURQ, sx * 560 - 50, sx * 560 + 50, -616, -606, 70, 190)       # what is in the windows: plain boxes and balls
        g.sphere(TURQ if sx < 0 else HOT, (sx * 560, -612, 226), 34, steps=(4, 6))
        B(g, CREAM, sx * 340 - 40, sx * 340 + 40, -616, -606, 70, 250)
        awning(g, min(sx * 190, sx * 760), max(sx * 190, sx * 760), -600, 410, [HOT, CREAM])
        B(g, HOT, sx * 772 - 8, sx * 772 + 8, -596, -580, 440, 1650, GN)
        grid(g, 'l' if sx < 0 else 'r', -520, 520, sx * 780, 500, 1570, 5, 3, dark=4, seed=21 + sx, alt=[(TURQ, 1, GW)])
        grid(g, 'l' if sx < 0 else 'r', -300, 300, sx * 800, 60, 330, 2, 1, frame=ASPH, mw=0.12)
        tub(g, sx * 250, -690, 0)
    grid(g, 'f', -740, 740, -580, 500, 1570, 8, 3, dark=5, seed=20, alt=[(TURQ, 2, GW), (HOT, 2, GW)])
    B(g, VIOLET, -150, 150, -640, -600, 0, 340)                                  # the revolving door's drum
    g.cyl(HOT, (0, -640, 340), 170, 24, steps=10)
    g.cyl(TURQ, (0, -640, 364), 150, 14, steps=10, glow=GN)
    d = sc.part(P.motion(travel=(-1.0, 0.1), base=(0.0, -700.0)))
    d.box(SKY, (0, -700, 150), (230, 8, 290))
    d.box(SKY, (0, -700, 150), (8, 230, 290))
    g.cyl(STEEL, (0, -700, 0), 6, 300, steps=5)
    frame(g, -300, 300, -330, 1690, 2330)
    y = -348
    tube(g, HOT, [(-200, 1790), (200, 1790), (200, 2140), (-200, 2140)], y, t=20, close=True)      # the bag
    tube(g, TURQ, arc(0, 2140, 110, 0, 180, 6), y, t=16)
    star(g, YEL, 0, y, 1960, 105, glow=GN)
    for k, c in enumerate((HOT, TURQ, HOT)):
        pennant(sc, -600 + k * 600, -570, 1880, c, mast=190, phase=0.2 + k * 0.3)
    hut(g, 380, 700, 150, 480, 1690)
    vent(g, -560, 250, 1690)
    vent(g, -360, 380, 1690)
    cowl(sc, -620, -150, 1690, 0.5)


def corner_tower(sc):
    """A round corner tower, 21 m to its last ring and 28 m to the mast's ball, on a three-storey wing (14.3 x 11.3 m in all): five bands
    of lit windows round the drum, rings of turquoise neon, a pink cone, a round canopy."""
    g = sc.still()
    cx, cy = -330.0, -180.0
    B(g, VIOLET, -300, 700, -450, 550, -FOOT, 1050)
    B(g, NAVY, -310, 712, -462, 562, 1050, 1086)
    B(g, HOT, 0, 712, -470, -456, 1086, 1102, GN)
    B(g, HOT, 704, 720, -470, 562, 1086, 1102, GN)
    grid(g, 'f', 110, 680, -450, 440, 980, 3, 2, frame=VIOLET, dark=1, seed=31)
    grid(g, 'r', -380, 480, 700, 120, 980, 4, 3, frame=VIOLET, dark=3, seed=32, alt=[(HOT, 1, GW)])
    grid(g, 'f', 110, 680, -450, 60, 300, 2, 1, frame=VIOLET, mw=0.14)          # the wing's shop
    awning(g, 100, 690, -450, 400, [TURQ, CREAM])
    g.cyl(NAVY, (cx, cy, -FOOT), 400, 2100 + FOOT, steps=12)
    for k in range(5):
        g.cyl(YEL, (cx, cy, 440 + k * 330), 405, 160, steps=12, glow=GW)
    for z in (382, 1262, 2060, 2100):
        g.cyl(TURQ, (cx, cy, z), 412, 18, steps=12, glow=GN)
    for a in range(12):
        an = math.radians(a * 30.0 + 15.0)
        g.box(NAVY, (cx + 404 * math.cos(an), cy + 404 * math.sin(an), 1250), (30, 70, 1660), rot=(0, a * 30.0 + 15.0, 0))
    g.cyl(HOT, (cx, cy, 330), 480, 26, steps=12)                                 # the round canopy
    g.cyl(TURQ, (cx, cy, 318), 470, 12, steps=12, glow=GN)
    B(g, YEL, cx - 90, cx + 90, cy - 412, cy - 398, 0, 280, 0.25)
    g.cyl(VIOLET, (cx, cy, 2118), 300, 250, steps=12)
    g.cyl(TURQ, (cx, cy, 2368), 310, 16, steps=12, glow=GN)
    g.cyl(HOT, (cx, cy, 2384), 300, 300, steps=12, top_r=20)
    pennant(sc, cx, cy, 2800, TURQ, mast=130, phase=0.7)
    hut(g, 380, 660, 200, 480, 1086)
    vent(g, 200, 300, 1086)
    cowl(sc, 560, -250, 1086, 0.1)
    tub(g, 60, -520, 0)
    tub(g, 720 - 60, -520, 0)


# ------------------------------------------------------------------------------------------ the back row: the towers
def glass_office(sc):
    """A glass office block, 14 x 11 m, 32.6 m and a neon moon on the roof to 39.8 m: a curtain wall in a grid of lit
    windows on three sides, sockets for the skybridge, a window cleaners' cradle that rides slowly up and down."""
    g = sc.still()
    B(g, ASPH, -700, 700, -550, 550, -FOOT, 500)
    B(g, NAVY, -650, 650, -500, 500, 500, 3200)
    grid(g, 'f', -600, 600, -550, 50, 400, 4, 1, frame=ASPH, mw=0.14, alt=[(TURQ, 1, GW)], seed=40)
    B(g, VIOLET, -300, 300, -750, -550, 410, 440)
    B(g, TURQ, -308, 308, -760, -746, 404, 446, GN)
    for sx in (-1, 1):
        g.cyl(STEEL, (sx * 270, -722, 0), 7, 410, steps=6)
        grid(g, 'l' if sx < 0 else 'r', -440, 440, sx * 650, 560, 3120, 4, 8, dark=6, seed=42 + sx, alt=[(TURQ, 2, GW)])
        B(g, TURQ, sx * 642 - 8, sx * 642 + 8, -516, -500, 500, 3200, GN)
        x0, x1 = sorted((sx * 650, sx * 800))                                    # a socket for the skybridge
        B(g, VIOLET, x0, x1, -150, 150, 1510, 1850)
        B(g, YEL, x0, x1, -156, -150, 1610, 1760, GW)
        B(g, TURQ, x0, x1, -160, -150, 1832, 1848, GN)
        B(g, STEEL, sx * 255 - 10, sx * 255 + 10, -570, -470, 3260, 3274)        # the cradle's davits
        tub(g, sx * 420, -640, 0)
    grid(g, 'f', -600, 600, -500, 560, 3120, 6, 8, dark=9, seed=41, alt=[(TURQ, 3, GW), (HOT, 2, GW)])
    B(g, YEL, -60, 60, 500, 506, 560, 3120, 0.2)                                 # the stair's strip on the back
    B(g, VIOLET, -670, 670, -520, 520, 3200, 3260)
    roofline(g, HOT, 1340, 1040, 3260)
    B(g, VIOLET, -350, 350, -150, 380, 3260, 3470)
    g.cyl(STEEL, (250, 250, 3470), 6, 320, steps=5)
    g.sphere(YEL, (250, 250, 3800), 14, steps=(3, 5), glow=0.4)
    frame(g, -280, 280, -400, 3260, 3980, back=220.0)
    y = -418
    tube(g, TURQ, arc(-20, 3640, 270, 55, 305, 10), y, t=22)                     # the moon
    tube(g, TURQ, arc(95, 3640, 230, 75, 285, 8), y, t=22)
    star(g, HOT, 215, y, 3800, 62, glow=GN)
    star(g, HOT, 250, y, 3520, 44, glow=GN)
    c = sc.part(P.motion(bob=-45.0, rate=0.05, phase=0.2))                       # the cradle (nobody in it)
    c.box(WHITE, (0, -530, 2300), (480, 46, 70))
    c.box(TURQ, (0, -530, 2340), (490, 56, 10))
    for sx in (-1, 1):
        c.box(STEEL, (sx * 255, -530, 2770), (6, 6, 860))


def step_tower(sc):
    """A stepped tower, 14 x 12 m at the foot, four setbacks to 33.5 m and a mast to 39 m: every setback drawn in pink
    neon, turquoise tubes up the crown, tubs on the terraces, a pennant."""
    g = sc.still()
    tiers = ((1400, 1200, -FOOT, 1400, VIOLET), (1100, 950, 1400, 2400, NAVY), (800, 700, 2400, 3050, VIOLET), (500, 450, 3050, 3350, NAVY))
    for w, d, z0, z1, col in tiers:
        B(g, col, -w / 2.0, w / 2.0, -d / 2.0, d / 2.0, z0, z1)
        roofline(g, HOT, w, d, z1 - 10)
    grid(g, 'f', -620, 620, -600, 480, 1300, 6, 3, frame=VIOLET, dark=4, seed=50, alt=[(TURQ, 2, GW)])
    grid(g, 'f', -480, 480, -475, 1480, 2320, 4, 3, dark=3, seed=51, alt=[(HOT, 1, GW)])
    grid(g, 'f', -340, 340, -350, 2470, 2970, 3, 2, frame=VIOLET, dark=1, seed=52)
    for sx in (-1, 1):
        f = 'l' if sx < 0 else 'r'
        grid(g, f, -520, 520, sx * 700, 480, 1300, 5, 3, frame=VIOLET, dark=4, seed=53 + sx)
        grid(g, f, -400, 400, sx * 550, 1480, 2320, 3, 3, dark=2, seed=55 + sx)
        grid(g, f, -280, 280, sx * 400, 2470, 2970, 2, 2, frame=VIOLET, dark=1, seed=57 + sx)
        for sy in (-1, 1):
            B(g, TURQ, sx * 250 - 9, sx * 250 + 9, sy * 225 - 9, sy * 225 + 9, 3050, 3350, GN)
        tub(g, sx * 630, -540, 1400, r=40)
        tub(g, sx * 480, -420, 2400, r=36)
        tub(g, sx * 300, -700, 0)
    B(g, NAVY, -220, 220, -612, -600, 0, 430)                                    # the portal
    B(g, YEL, -150, 150, -618, -610, 0, 380, 0.25)
    B(g, NAVY, -5, 5, -623, -616, 0, 380)
    tube(g, HOT, [(-200, 0), (-200, 430), (200, 430), (200, 0)], -620, t=18)
    grid(g, 'f', -620, -280, -600, 60, 380, 2, 1, frame=VIOLET, mw=0.14)
    grid(g, 'f', 280, 620, -600, 60, 380, 2, 1, frame=VIOLET, mw=0.14)
    g.cyl(STEEL, (0, 0, 3350), 14, 520, steps=6, top_r=5)
    g.sphere(YEL, (0, 0, 3885), 22, steps=(4, 6), glow=0.4)
    pennant(sc, 0, 0, 3780, TURQ, w=150, h=60, phase=0.5)
    parasol(g, -560, 430, 1400, PINK)
    parasol(g, 560, 430, 1400, TURQ)


def car_deck(sc):
    """A multi-storey car deck, 16 x 12 m, five open floors to 13.8 m and a stair tower to 17 m: small pale cars nose
    to the parapets, a lit strip on every floor, a little car drawn in neon over the top deck."""
    g = sc.still()
    r = random.Random(4)
    B(g, ASPH, -800, 800, -600, 600, -FOOT, 20)
    B(g, VIOLET, -500, 500, 150, 600, 20, 1280)                                  # the ramps' core
    for k in range(5):
        zt = 320.0 * k
        if k:
            B(g, ASPH, -800, 800, -600, 600, zt - 40, zt)
            B(g, VIOLET, -800, 800, -600, -580, zt, zt + 95)
            for sx in (-1, 1):
                B(g, VIOLET, sx * 790 - 10, sx * 790 + 10, -580, 600, zt, zt + 95)
        if k < 4:
            B(g, YEL, -480, 480, 142, 150, zt + 190, zt + 250, 0.25)
        for x in (-560, -190, 190, 560):
            if r.random() < 0.75:
                car(g, x, -350, zt + (20 if k == 0 else 0), r.choice(PASTEL), across=True)
    for x in (-780, -260, 260, 780):
        B(g, NAVY, x - 20, x + 20, -600, -560, 20, 1280)
    for sx in (-1, 1):
        for y in (0, 580):
            B(g, NAVY, sx * 780 - 20, sx * 780 + 20, y - 20, y + 20, 20, 1280)
        g.cyl(STEEL, (sx * 700, 60, 1280), 6, 300, steps=5)                      # a lamp on the top deck
        g.sphere(YEL, (sx * 700, 60, 1596), 22, steps=(4, 6), glow=GB)
    roofline(g, HOT, 1600, 1200, 1375)
    B(g, TURQ, -808, 808, -610, -596, 700, 716, GN)
    B(g, NAVY, 480, 800, 300, 600, 1280, 1700)                                   # the stair tower
    B(g, YEL, 620, 660, 292, 300, 1330, 1650, GW)
    B(g, TURQ, 472, 808, 290, 306, 1700, 1716, GN)
    B(g, YEL, -808, -800, -300, 100, 250, 290, GW)                               # the way in, off the side street
    B(g, YEL, 800, 808, -300, 100, 250, 290, GW)
    frame(g, -330, 330, -560, 1375, 1900, back=200.0)
    y = -578
    tube(g, HOT, [(-270, 1560), (-270, 1650), (-150, 1665), (-90, 1770), (110, 1770), (180, 1665), (270, 1650), (270, 1560)], y, t=18, close=True)
    for x in (-150, 150):
        tube(g, TURQ, arc(x, 1550, 52, 0, 360, 8), y - 4, t=14)


# ------------------------------------------------------------------------------------------ two to three storeys by the road (behind the shopfronts)
def walk_up(sc):
    """A three-storey walk-up, 15 x 5.2 m, 11.4 m and a roof sign to 17 m: lit and dark windows with flower boxes, a
    fire escape, washing on a line that swings, an aerial, a coffee cup drawn in neon."""
    g = sc.still()
    r = random.Random(8)
    B(g, VIOLET, -750, 750, -260, 260, -FOOT, 1100)
    B(g, ASPH, -765, 765, -275, 275, 1100, 1140)
    roofline(g, TURQ, 1530, 550, 1140, sides=False)
    B(g, YEL, -600, 600, -266, -260, 60, 300, 0.2)
    for fl in range(2):
        z0 = 480 + fl * 300
        for k in range(5):
            x = -600 + k * 300
            lit = r.random() < 0.7
            B(g, CREAM, x - 62, x + 62, -268, -260, z0 - 8, z0 + 178)
            B(g, YEL if lit else NAVY, x - 50, x + 50, -273, -266, z0, z0 + 170, GW if lit else 0.0)
            if (k + fl) % 2 == 0:
                B(g, ASPH, x - 60, x + 60, -292, -262, z0 - 32, z0 - 8)
                B(g, HOT if k % 2 else PINK, x - 52, x + 52, -288, -266, z0 - 8, z0 + 12)
    for z in (450, 750):                                                         # the fire escape
        B(g, STEEL, 150, 650, -345, -262, z, z + 10)
        B(g, STEEL, 150, 650, -345, -337, z + 10, z + 95)
    tilt_box(g, STEEL, (220, -304, 755), (560, -304, 455), 50, 10)
    for sx in (-1, 1):
        grid(g, 'l' if sx < 0 else 'r', -180, 180, sx * 750, 440, 1040, 2, 2, frame=VIOLET, dark=1, seed=60 + sx)
    for x in (-650, -50):                                                        # the washing line
        g.cyl(STEEL, (x, -100, 1140), 5, 225, steps=5)
    tilt_box(g, STEEL, (-650, -100, 1360), (-50, -100, 1360), 5, 5)
    for k, (c, w, h) in enumerate(((WHITE, 90, 120), (PINK, 60, 90), (TURQ, 110, 70), (YEL, 60, 100), (WHITE, 80, 130))):
        x = -570 + k * 115
        p = sc.part(P.motion(swing=9, pivot=(x, -100, 1358), axis=0.0, rate=0.45, phase=k * 0.19))
        p.box(c, (x, -100, 1358 - h / 2.0), (w, 5, h))
    frame(g, 200, 600, -200, 1140, 1700, back=200.0)
    y = -218
    tube(g, TURQ, [(260, 1440), (290, 1270), (490, 1270), (520, 1440)], y, t=18, close=True)       # the cup
    tube(g, TURQ, arc(530, 1355, 58, -70, 70, 5), y, t=14)
    tube(g, HOT, [(220, 1245), (560, 1245)], y, t=18)
    for x in (340, 440):
        tube(g, YEL, [(x, 1480), (x + 30, 1535), (x, 1590), (x + 30, 1645)], y, t=12)
    hut(g, -330, -80, 20, 240, 1140, h=210)
    g.cyl(STEEL, (-420, 150, 1140), 5, 260, steps=5)                             # the aerial
    B(g, STEEL, -500, -340, 147, 153, 1330, 1336)
    B(g, STEEL, -470, -370, 147, 153, 1380, 1386)
    cowl(sc, 80, 120, 1140, 0.3)
    vent(g, 660, 120, 1140)


def neon_bar(sc):
    """A two-storey music bar, 14 x 5.2 m, 8.7 m and its notes in neon to 15 m: four tall windows lit pink, a blade of
    discs, a roof terrace with parasols and a string of lights, pennants."""
    g = sc.still()
    B(g, NAVY, -700, 700, -260, 260, -FOOT, 850)
    B(g, VIOLET, -712, 712, -272, 272, 850, 872)
    roofline(g, HOT, 1424, 544, 872)
    B(g, YEL, -560, 560, -266, -260, 60, 300, 0.2)
    for x in (-480, -160, 160, 480):
        B(g, TURQ, x - 92, x + 92, -268, -260, 420, 792)
        B(g, PINK, x - 74, x + 74, -274, -266, 436, 776, GW)
        B(g, TURQ, x - 92, x + 92, -280, -262, 404, 420)
    B(g, HOT, -770, -712, -360, -260, 350, 1150)                                 # the blade
    for k in range(4):
        disc_x(g, TURQ, -741, -310, 470 + k * 190, 30, 74, steps=8, glow=GN)
    for sx in (-1, 1):
        grid(g, 'l' if sx < 0 else 'r', -180, 180, sx * 700, 430, 780, 2, 1, lit=PINK, dark=0, seed=70)
    B(g, STEEL, -700, 100, -262, -256, 955, 963)                                 # the terrace's rail
    for x in (-690, -430, -170, 90):
        B(g, STEEL, x - 4, x + 4, -263, -255, 872, 960)
    parasol(g, -520, 20, 872, PINK)
    parasol(g, -200, 60, 872, TURQ)
    for x in (-690, 120):
        g.cyl(STEEL, (x, 200, 872), 5, 250, steps=5)
    bulbs(g, (-690, 200, 1118), (120, 200, 1118), 7)
    frame(g, 200, 640, -200, 872, 1560, back=200.0)
    y = -218
    disc(g, YEL, 290, y - 16, 1080, 56, 10, glow=GN)                            # two notes on one beam
    disc(g, YEL, 500, y - 16, 1130, 56, 10, glow=GN)
    tube(g, HOT, [(340, 1080), (340, 1420)], y, t=18)
    tube(g, HOT, [(550, 1130), (550, 1470)], y, t=18)
    tube(g, HOT, [(340, 1410), (550, 1460)], y, t=42)
    star(g, TURQ, 240, y, 1420, 52, glow=GN)
    hut(g, 380, 640, 20, 240, 872, h=200)
    pennant(sc, -690, 200, 1190, HOT, mast=60, phase=0.15)
    pennant(sc, 120, 200, 1190, TURQ, mast=60, phase=0.55)


# ------------------------------------------------------------------------------------------ the bay side: downtown comes down to the sand
def pavilion(sc):
    """A night pavilion on piles over the sand, 18 x 7.5 m, 6 m to the roof and its sign to 10.4 m: a lit glass hall,
    a drum with a turquoise ring, parasols on the deck, waves and a half sun drawn in neon."""
    g = sc.still()
    B(g, ASPH, -900, 900, -375, 375, 60, 100)
    for x in (-820, -410, 0, 410, 820):
        for y in (-320, 320):
            B(g, NAVY, x - 18, x + 18, y - 18, y + 18, -FOOT, 60)
    B(g, NAVY, -700, 700, -100, 340, 100, 560)
    grid(g, 'f', -660, 660, -100, 130, 520, 7, 1, mw=0.14, seed=80, alt=[(TURQ, 2, GW), (HOT, 1, GW)])
    B(g, VIOLET, -800, 800, -250, 375, 560, 600)
    B(g, TURQ, -808, 808, -260, -244, 600, 616, GN)
    for sx in (-1, 1):
        grid(g, 'l' if sx < 0 else 'r', -60, 300, sx * 700, 130, 520, 2, 1, mw=0.14)
        B(g, HOT, sx * 800 - 8, sx * 800 + 8, -250, 375, 600, 616, GN)
        B(g, STEEL, min(sx * 220, sx * 900), max(sx * 220, sx * 900), -375, -367, 190, 198)        # the deck's rail
        for x in (230, 560, 890):
            B(g, STEEL, sx * x - 4, sx * x + 4, -375, -367, 100, 194)
        for k, x in enumerate((420, 680)):
            parasol(g, sx * x, -240, 100, PINK if (k + (sx > 0)) % 2 else TURQ)
        pennant(sc, sx * 780, 340, 800, HOT if sx < 0 else TURQ, mast=200, phase=0.3 + sx * 0.2)
    B(g, CREAM, -200, 200, -475, -375, -FOOT, 62)                                # the steps down to the sand
    B(g, CREAM, -200, 200, -575, -475, -FOOT, 24)
    g.cyl(VIOLET, (0, 80, 600), 260, 170, steps=10)
    g.cyl(TURQ, (0, 80, 770), 268, 16, steps=10, glow=GN)
    g.cyl(HOT, (0, 80, 786), 260, 90, steps=10, top_r=30)
    frame(g, -520, 520, -200, 600, 1010, back=180.0)
    y = -218
    tube(g, TURQ, [(-480 + k * 80, 700 + 42 * math.sin(k * math.pi / 2.0)) for k in range(13)], y, t=18)
    tube(g, HOT, [(-480 + k * 80, 630 + 42 * math.sin(k * math.pi / 2.0 + math.pi)) for k in range(13)], y, t=18)
    tube(g, YEL, arc(0, 790, 150, 0, 180, 7), y, t=18)
    for a in (30, 90, 150):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        tube(g, YEL, [(185 * ca, 790 + 185 * sa), (245 * ca, 790 + 245 * sa)], y, t=14)


def winter_garden(sc):
    """A winter garden, 20 x 13 m (15.2 with its porch), 12 m: a navy hall of lit windows under a barrel vault ribbed
    in turquoise neon, a lit fan window in each end, a porch under a pink neon arch."""
    g = sc.still()
    B(g, NAVY, -1000, 1000, -650, 650, -FOOT, 600)
    grid(g, 'f', -940, 940, -650, 90, 520, 9, 1, mw=0.2, dark=2, seed=90, alt=[(TURQ, 2, GW)])
    B(g, VIOLET, -1015, 1015, -665, 665, 600, 640)
    roofline(g, HOT, 2030, 1330, 640)
    g.cyl(VIOLET, (950, 0, 640), 520, 1900, rot=(90, 0, 0), steps=14)            # the vault
    for x in (-900, -450, 0, 450, 900):
        disc_x(g, TURQ, x, 0, 640, 532, 16, steps=14, glow=GN)
    B(g, YEL, -800, 800, -40, 40, 1150, 1176, GW)                                # the ridge light
    for sx in (-1, 1):
        grid(g, 'l' if sx < 0 else 'r', -560, 560, sx * 1000, 90, 520, 5, 1, mw=0.2, dark=1, seed=91 + sx)
        disc_x(g, YEL, sx * 955, 0, 640, 470, 12, steps=14, glow=GW)             # the fan window in the end
        for a in (45, 90, 135):
            tilt_box(g, NAVY, (sx * 962, 0, 640), (sx * 962, 465 * math.cos(math.radians(a)), 640 + 465 * math.sin(math.radians(a))), 12, 24)
        g.sphere(YEL, (sx * 820, 0, 1200), 24, steps=(4, 6), glow=0.4)
        tub(g, sx * 380, -740, 0)
        tub(g, sx * 700, -720, 0)
        pennant(sc, sx * 960, -600, 960, TURQ if sx < 0 else HOT, mast=310, phase=0.4 + sx * 0.25)
    B(g, VIOLET, -260, 260, -800, -650, -FOOT, 420)                              # the porch
    B(g, YEL, -190, 190, -808, -798, 30, 350, 0.25)
    B(g, NAVY, -5, 5, -813, -806, 30, 350)
    tube(g, HOT, [(-300, 30)] + list(reversed(arc(0, 420, 300, 0, 180, 8))) + [(300, 30)], -812, t=20)
    B(g, CREAM, -330, 330, -900, -800, -FOOT, 30)


def sky_bridge(sc):
    """The skybridge, 26 m long, 15.1 to 18.7 m up: it joins the two towers of the back row over the side street. A
    band of lit windows each side, turquoise neon along it, a round sign hung under it that swings a little."""
    g = sc.still()
    B(g, NAVY, -1300, 1300, -150, 150, 1510, 1850)
    B(g, VIOLET, -1300, 1300, -170, 170, 1850, 1870)
    for face, wall, seed in (('f', -150, 100), ('b', 150, 101)):
        grid(g, face, -1240, 1240, wall, 1610, 1760, 10, 1, mw=0.2, dark=2, seed=seed, alt=[(TURQ, 1, GW)])
        wb(g, face, TURQ, -1300, 1300, wall, 10, 1516, 1532, GN)
        wb(g, face, TURQ, -1300, 1300, wall, 10, 1832, 1848, GN)
    s = sc.part(P.motion(swing=5, pivot=(0, 0, 1510), axis=0.0, rate=0.3, phase=0.3))
    for sx in (-1, 1):
        s.box(STEEL, (sx * 60, 0, 1440), (6, 6, 140))
    s.cyl(HOT, (0, -10, 1280), 112, 20, rot=(0, 0, 90), steps=10)
    s.cyl(TURQ, (0, -16, 1280), 84, 6, rot=(0, 0, 90), steps=10, glow=GN)
    s.side(YEL, ccw([(px, 1280 + pz) for px, pz in star_pts(50)]), -24, -16, glow=GN)


# ------------------------------------------------------------------------------------------ props
def neon_palm(sc):
    """A palm drawn in neon on a post, 7.9 m (TALL): a pink trunk, six turquoise fronds, two yellow nuts."""
    g = sc.still()
    B(g, ASPH, -45, 45, -45, 45, -200, 40)
    g.cyl(STEEL, (0, 0, 40), 9, 280, steps=6)
    tube(g, HOT, [(0, 300), (-28, 430), (-12, 570), (30, 690)], 0, t=24, w=22)
    for a in (160, 120, 75, 35, -5, 195):
        a1 = a - 55 if a <= 90 else a + 55
        p1 = (30 + 150 * math.cos(math.radians(a)), 690 + 150 * math.sin(math.radians(a)))
        p2 = (p1[0] + 140 * math.cos(math.radians(a1)), p1[1] + 140 * math.sin(math.radians(a1)))
        tube(g, TURQ, [(30, 690), p1, p2], 0, t=20, w=18)
    for x in (4, 58):
        disc(g, YEL, x, -14, 668, 22, 28, steps=8, glow=GN)


def street_clock(sc):
    """A street clock with four faces, 6.9 m (TALL): a navy post, a violet head, a pink cap, two flower baskets."""
    g = sc.still()
    g.cyl(ASPH, (0, 0, -200), 60, 240, steps=8)
    g.cyl(VIOLET, (0, 0, 40), 38, 70, steps=8, top_r=22)
    g.cyl(NAVY, (0, 0, 110), 16, 306, steps=8)
    g.cyl(HOT, (0, 0, 400), 30, 16, steps=8)
    B(g, VIOLET, -62, 62, -62, 62, 416, 540)
    for s in (-1, 1):
        disc(g, WHITE, 0, -70 if s < 0 else 62, 478, 50, 8)
        B(g, NAVY, -4, 4, s * 66 - 4, s * 66 + 4, 478, 518)
        B(g, NAVY, 0, 30, s * 66 - 4, s * 66 + 4, 474, 482)
        disc_x(g, WHITE, s * 66, 0, 478, 50, 8)
        B(g, NAVY, s * 66 - 4, s * 66 + 4, -4, 4, 478, 518)
        B(g, NAVY, s * 66 - 4, s * 66 + 4, -30, 0, 474, 482)
        g.sphere(PINK, (s * 120, 0, 296), 24, steps=(4, 6))                      # a flower basket
        g.sphere(HOT, (s * 120, 0, 316), 13, steps=(3, 5))
        B(g, NAVY, s * 120 - 3, s * 120 + 3, -3, 3, 318, 336)
    B(g, NAVY, -124, 124, -5, 5, 334, 344)
    g.cyl(HOT, (0, 0, 540), 84, 60, steps=4, top_r=6)
    g.cyl(STEEL, (0, 0, 600), 5, 62, steps=5)
    g.sphere(YEL, (0, 0, 674), 14, steps=(3, 5))


def poster_drum(sc):
    """A poster column, 4.4 m (TALL): a cream drum, five plain posters (colours, no words), a violet brim, a pink cone."""
    g = sc.still()
    g.cyl(ASPH, (0, 0, -200), 86, 230, steps=10)
    g.cyl(CREAM, (0, 0, 30), 72, 290, steps=10)
    g.cyl(VIOLET, (0, 0, 320), 92, 24, steps=10)
    g.cyl(HOT, (0, 0, 344), 84, 70, steps=10, top_r=14)
    g.sphere(YEL, (0, 0, 426), 14, steps=(3, 5))
    for k, c in enumerate((PINK, TURQ, YEL, LAV, WHITE)):
        a = k * 72.0 + 18.0
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        g.box(NAVY, (72 * ca, 72 * sa, 175), (8, 76, 214), rot=(0, a, 0))
        g.box(c, (75 * ca, 75 * sa, 175), (8, 62, 196), rot=(0, a, 0))
        g.box(HOT if c != PINK else TURQ, (78 * ca, 78 * sa, 120), (6, 44, 44), rot=(0, a, 0))


def news_stand(sc):
    """A news stand, 4.9 x 2.0 m, 3.0 m (TALL): racks of plain magazines, an awning, open shutters with plain posters,
    a lit blank sign, bundles of papers, a card rack that turns."""
    g = sc.still()
    cols = (PINK, TURQ, YEL, WHITE, LAV)
    B(g, VIOLET, -150, 150, -60, 90, -200, 230)
    B(g, CREAM, -160, 160, -100, -60, 90, 104)
    B(g, NAVY, -130, 130, -66, -58, 110, 205)
    for row in range(2):
        for k in range(6):
            B(g, cols[(k + row * 2) % 5], -138 + k * 46, -138 + k * 46 + 38, -67, -60, 12 + row * 38, 42 + row * 38)
    for k in range(6):
        B(g, cols[(k + 1) % 5], -120 + k * 42, -120 + k * 42 + 30, -70, -64, 160, 198)
    for sx in (-1, 1):
        g.box(VIOLET, (sx * 200, -95, 120), (8, 110, 200), rot=(0, -sx * 25.0, 0))
        g.box(PINK if sx < 0 else TURQ, (sx * 200 - sx * 2, -100, 130), (8, 84, 130), rot=(0, -sx * 25.0, 0))
    awning(g, -170, 170, -60, 240, [HOT, CREAM], depth=110.0, drop=40.0, n=6)
    B(g, NAVY, -165, 165, -75, 100, 230, 246)
    B(g, YEL, -110, 110, -30, -20, 246, 300, GW)
    for x in (-60, 0, 60):
        disc(g, HOT, x, -36, 273, 16, 6, steps=8)
    B(g, WHITE, 180, 240, -50, 10, 0, 40)
    B(g, TURQ, 178, 242, -24, -16, 0, 42)
    g.cyl(STEEL, (-235, -40, -100), 5, 260, steps=5)                             # the card rack
    s = sc.part(P.motion(travel=(-1.0, 0.08), base=(-235.0, -40.0)))
    for k in range(4):
        a = k * 90.0
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        for j in range(2):
            s.box(cols[(k + j) % 5], (-235 + 16 * ca, -40 + 16 * sa, 70 + j * 46), (5, 26, 36), rot=(0, a, 0))


def taxi_rank(sc):
    """A taxi rank, 6.9 x 6.0 m, 2.9 m (TALL): a turquoise shelter with a lit blank fascia and a bench, a round sign,
    two yellow and white cabs nose to the kerb."""
    g = sc.still()
    for x in (-250, 250):
        B(g, NAVY, x - 7, x + 7, 193, 207, -200, 250)
    B(g, SKY, -250, 250, 198, 204, 60, 220)
    B(g, TURQ, -300, 300, 60, 240, 250, 264)
    B(g, YEL, -300, 300, 54, 60, 244, 286, GW)
    for k in range(8):
        B(g, WHITE, -280 + k * 70, -250 + k * 70, 49, 54, 250 + (k % 2) * 16, 266 + (k % 2) * 16)
    B(g, HOT, -180, 180, 150, 190, 45, 55)
    for x in (-160, 160):
        B(g, NAVY, x - 5, x + 5, 160, 180, -100, 45)
    g.cyl(STEEL, (330, 120, -200), 6, 470, steps=5)
    disc(g, TURQ, 330, 108, 250, 30, 8, steps=8)
    disc(g, WHITE, 330, 104, 250, 16, 4, steps=8)
    for x in (-150, 150):
        car(g, x, -150, 0, YEL, across=True, band=WHITE)
        B(g, WHITE, x - 25, x + 25, -185, -145, 144, 156)


def rope_carpet(sc):
    """LOW (top 1.14 m): a pink carpet 5 m long between two rows of yellow posts and turquoise ropes, a tub of flowers at each end."""
    g = sc.still()
    B(g, HOT, -250, 250, -55, 55, -100, 7)
    for y in (-68, 68):
        xs = (-240, -80, 80, 240)
        for x in xs:
            g.cyl(YEL, (x, y, -100), 6, 195, steps=6)
            g.sphere(YEL, (x, y, 104), 10, steps=(3, 5))
        for xa, xb in zip(xs, xs[1:]):
            xm = (xa + xb) / 2.0
            tilt_box(g, TURQ, (xa, y, 88), (xm, y, 70), 6, 6)
            tilt_box(g, TURQ, (xm, y, 70), (xb, y, 88), 6, 6)
    for sx in (-1, 1):                                                           # a tub of pink and white flowers (no ball)
        B(g, ASPH, sx * 310 - 30, sx * 310 + 30, -30, 30, -100, 44)
        B(g, PINK, sx * 310 - 24, sx * 310 + 24, -24, 24, 44, 58)
        B(g, WHITE, sx * 310 - 10, sx * 310 + 10, -10, 10, 58, 70)


def news_racks(sc):
    """LOW (top 1.29 m): five paper boxes in pale colours on legs, a violet post box, a pink hydrant, a trough of pink flowers."""
    g = sc.still()
    for k, c in enumerate((PINK, TURQ, YEL, WHITE, LAV)):
        x = -200 + k * 62
        B(g, ASPH, x - 5, x + 5, -5, 5, -100, 50)
        B(g, c, x - 25, x + 25, -22, 22, 50, 112)
        B(g, NAVY, x - 18, x + 18, -27, -22, 66, 100)
        B(g, WHITE, x - 27, x + 27, -24, 24, 112, 118)
    g.cyl(VIOLET, (150, 0, -100), 24, 205, steps=8)
    g.sphere(VIOLET, (150, 0, 105), 24, steps=(4, 8))
    g.cyl(HOT, (150, 0, 66), 27, 10, steps=8)
    g.cyl(HOT, (235, 0, -100), 12, 160, steps=6)
    g.sphere(HOT, (235, 0, 62), 13, steps=(3, 6))
    B(g, HOT, 219, 251, -5, 5, 28, 40)
    B(g, ASPH, -330, -250, -25, 25, -100, 45)
    B(g, PINK, -326, -254, -21, 21, 45, 62)
    for x in (-310, -290, -268):
        B(g, HOT if x != -290 else YEL, x - 7, x + 7, -7, 7, 62, 74)


def premiere_wall(sc):
    """LOW (top 1.72 m): a premiere's backdrop 4.3 m long, navy with eight plain stars, a low step and a pink mat in front."""
    g = sc.still()
    B(g, NAVY, -210, 210, -8, 8, 25, 166)
    B(g, WHITE, -214, 214, -10, 10, 166, 172)
    B(g, WHITE, -214, 214, -10, 10, 19, 25)
    for x in (-190, 190):
        B(g, ASPH, x - 8, x + 8, -40, 40, -70, 19)
    for j in range(2):
        for k in range(4):
            star(g, (HOT, TURQ, YEL)[(k + j) % 3], -150 + k * 100, -10, 62 + j * 68, 27, t=6)
    B(g, ASPH, -150, 150, -75, -12, -70, 14)
    B(g, HOT, -140, 140, -72, -15, 14, 19)


# ------------------------------------------------------------------------------------------ the hero
def picture_palace(sc):
    """THE PICTURE PALACE, 35 m wide, 21.6 m deep, 46.1 m: nine navy and violet pylons stepped up to the middle like
    organ pipes, each capped in pink neon with a turquoise tube up its face; a 30 m fan marquee of pink and turquoise
    tubes over a canopy of bulbs; a blade of discs; five lit doors up carpeted steps between rope posts; and on the
    middle pylon a crown of five yellow stars that turns once in 33 seconds (the crown turns about the mesh's origin)."""
    g = sc.still()
    B(g, ASPH, -1750, 1750, -700, 1000, -FOOT, 60)
    for k in range(4):                                                           # the steps
        B(g, CREAM, -1000 - (3 - k) * 60, 1000 + (3 - k) * 60, -1140 + k * 110, -700, -FOOT if k == 0 else 15 * k, 15 * (k + 1))
        B(g, HOT, -150, 150, -1140 + k * 110, -1030 + k * 110 if k < 3 else -650, 15 * (k + 1), 15 * (k + 1) + 5)
    for sx in (-1, 1):
        for k in range(4):
            g.cyl(YEL, (sx * 230, -1085 + k * 130, 15 * min(k + 1, 4)), 7, 100, steps=6)
            g.sphere(YEL, (sx * 230, -1085 + k * 130, 15 * min(k + 1, 4) + 110), 11, steps=(3, 5))
            if k:
                tilt_box(g, TURQ, (sx * 230, -1085 + (k - 1) * 130, 15 * k + 92), (sx * 230, -1085 + k * 130, 15 * (k + 1) + 92), 6, 6)
    B(g, NAVY, -1500, 1500, 150, 1000, 60, 1800)                                 # the hall and the fly tower
    B(g, VIOLET, -900, 900, 500, 1000, 1800, 2600)
    B(g, HOT, -908, 908, 490, 506, 2600, 2618, GN)
    for sx in (-1, 1):
        f = 'l' if sx < 0 else 'r'
        B(g, HOT, sx * 900 - 8, sx * 900 + 8, 490, 1000, 2600, 2618, GN)
        B(g, TURQ, sx * 1500 - 8, sx * 1500 + 8, 150, 1000, 1800, 1816, GN)
        grid(g, f, 660, 960, sx * 1500, 1200, 1700, 2, 1, dark=0, seed=200 + sx)
        x0, x1 = sorted((sx * 1260, sx * 1750))                                  # a wing
        B(g, VIOLET, x0, x1, -450, 660, 60, 1100)
        B(g, NAVY, x0 - 10, x1 + 10, -460, 670, 1100, 1130)
        B(g, TURQ, x0 - 10, x1 + 10, -470, -454, 1130, 1146, GN)
        B(g, TURQ, sx * 1756 - 8, sx * 1756 + 8, -470, 670, 1130, 1146, GN)
        grid(g, 'f', x0 + 40, x1 - 40, -450, 560, 1000, 2, 2, frame=VIOLET, dark=1, seed=202 + sx, alt=[(HOT, 1, GW)])
        grid(g, f, -330, 540, sx * 1750, 200, 1000, 4, 3, frame=VIOLET, dark=4, seed=204 + sx, alt=[(TURQ, 1, GW)])
        for k, x in enumerate((1390, 1620)):                                     # poster cases: plain colours, no words
            B(g, WHITE, sx * x - 84, sx * x + 84, -458, -450, 150, 440)
            B(g, (PINK, TURQ, LAV, YEL)[k + (sx > 0) * 2], sx * x - 68, sx * x + 68, -464, -456, 166, 424)
        pennant(sc, sx * 1500, 100, 1420, HOT if sx < 0 else TURQ, w=200, h=80, mast=290, phase=0.3 + sx * 0.2)
        g.cyl(NAVY, (sx * 1420, -1000, 60), 14, 370, steps=6)                    # the canopy's posts
        g.cyl(CREAM, (sx * 600, -860, 60), 90, 200, steps=8)                     # a ticket booth
        g.cyl(HOT, (sx * 600, -860, 260), 112, 20, steps=8)
        g.cyl(TURQ, (sx * 600, -860, 280), 100, 60, steps=8, top_r=12)
        B(g, NAVY, sx * 600 - 40, sx * 600 + 40, -958, -946, 150, 230)
        tub(g, sx * 1100, -900, 60, r=60)
    for i in range(-4, 5):                                                       # the organ front
        k = abs(i)
        x, H = i * 280.0, (4150.0, 3500.0, 2950.0, 2450.0, 2000.0)[k]
        yf = -450.0 - (4 - k) * 45.0
        B(g, NAVY if k % 2 == 0 else VIOLET, x - 140, x + 140, yf, 300, 60, H)
        B(g, HOT, x - 146, x + 146, yf - 10, yf + 12, H - 16, H + 8, GN)
        zf = 520.0 + math.sqrt(max(0.0, 1500.0 ** 2 - x * x)) + 60.0
        if k and H - 60 > zf + 80:
            B(g, TURQ, x - 10, x + 10, yf - 12, yf, zf, H - 50, GN)
        if k:
            sx = 1 if i > 0 else -1                                              # the step's own edge, on its outer side
            B(g, HOT, min(sx * 124, sx * 140) + x, max(sx * 124, sx * 140) + x, yf, 300, H - 16, H + 8, GN)
    B(g, VIOLET, -1100, 1100, -650, -600, 60, 430)                               # the doors
    for k in range(-2, 3):
        B(g, YEL, k * 400 - 130, k * 400 + 130, -658, -650, 60, 360, 0.25)
        B(g, NAVY, k * 400 - 6, k * 400 + 6, -663, -656, 60, 360)
    B(g, NAVY, -1500, 1500, -1050, -640, 430, 520)                               # the canopy and its bulbs
    B(g, HOT, -1508, 1508, -1060, -1050, 424, 526, 0.4)
    B(g, CREAM, -1460, 1460, -1030, -660, 423, 430, GW)
    for k in range(15):
        B(g, YEL, -1400 + k * 200 - 16, -1400 + k * 200 + 16, -1074, -1058, 408, 440, GB)
    fan(g, VIOLET, 0, 520, 300, 1500, -690, -660, n=14)                          # the fan marquee, 30 m
    for k in range(11):
        a = math.radians(k * 18.0)
        tube(g, HOT if k % 2 == 0 else TURQ, [(340 * math.cos(a), 520 + 340 * math.sin(a)), (1440 * math.cos(a), 520 + 1440 * math.sin(a))], -702, t=26, w=20)
    tube(g, HOT, arc(0, 520, 1482, 0, 180, 16), -704, t=30, w=22)
    tube(g, TURQ, arc(0, 520, 318, 0, 180, 8), -704, t=24, w=22)
    fan(g, YEL, 0, 520, 0, 270, -716, -690, n=8, glow=0.4)
    B(g, HOT, -70, 70, -740, -630, 2090, 3900)                                   # the blade: discs, no words
    B(g, TURQ, -78, 78, -748, -736, 2090, 2106, GN)
    for k in range(6):
        disc(g, TURQ, 0, -748, 2260 + k * 290, 46, 8, glow=GN)
    star(g, YEL, 0, -748, 3990, 100, t=16, glow=GN)
    g.cyl(STEEL, (0, 0, 4150), 16, 110, steps=6)                                 # the crown
    c = sc.part(P.motion(travel=(-1.0, 0.03), base=(0.0, 0.0)))
    R = 520.0
    c.cyl(HOT, (0, 0, 4200), 60, 60, steps=8)
    for k in range(10):
        a = k * 36.0 + 18.0
        c.box(TURQ, (R * math.cos(math.radians(a)), R * math.sin(math.radians(a)), 4230), (26, 2 * R * math.tan(math.radians(18.0)) + 10, 26), rot=(0, a, 0), glow=GN)
    for k in range(5):
        a = k * 72.0
        b = math.radians(a + 90.0)
        c.box(STEEL, (R / 2.0 * math.cos(b), R / 2.0 * math.sin(b), 4230), (R, 14, 14), rot=(0, a + 90.0, 0))
        c.box(STEEL, (R * math.cos(b), R * math.sin(b), 4290), (14, 14, 120))
        c.side(YEL, ccw(star_pts(180)), -12, 12, glow=GN, rot=(0, a, 0), loc=(0, R, 4430))


# ------------------------------------------------------------------------------------------ the kit
JOBS = [
    ('SM_S1D6_NightClub', night_club),
    ('SM_S1D6_Theatre', theatre),
    ('SM_S1D6_DeptStore', dept_store),
    ('SM_S1D6_CornerTower', corner_tower),
    ('SM_S1D6_GlassOffice', glass_office),
    ('SM_S1D6_StepTower', step_tower),
    ('SM_S1D6_CarDeck', car_deck, STILL),
    ('SM_S1D6_WalkUp', walk_up),
    ('SM_S1D6_NeonBar', neon_bar),
    ('SM_S1D6_Pavilion', pavilion),
    ('SM_S1D6_WinterGarden', winter_garden),
    ('SM_S1D6_SkyBridge', sky_bridge, SMALL),
    ('SM_S1D6_NeonPalm', neon_palm, STILL),
    ('SM_S1D6_StreetClock', street_clock, STILL),
    ('SM_S1D6_PosterDrum', poster_drum, STILL),
    ('SM_S1D6_NewsStand', news_stand, SMALL),
    ('SM_S1D6_TaxiRank', taxi_rank, STILL),
    ('SM_S1D6_RopeCarpet', rope_carpet, STILL),
    ('SM_S1D6_NewsRacks', news_racks, STILL),
    ('SM_S1D6_PremiereWall', premiere_wall, STILL),
    ('SM_S1D6_PicturePalace', picture_palace, dict(far=True, grow=(80, 80, 20), grow_down=(80, 80, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilds them
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
