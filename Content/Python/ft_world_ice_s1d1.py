"""Ice Mountain, Stage 1, builder s1d1: district 1 "First tracks" (0 to 2.43 km). /Game/World/IceMountain/SM_S1D1_*.
Run: import ft_world_ice_s1d1 as K; K.run()

THE SUMMIT STATION. A modern station on the top of the mountain: round pods and capsules on navy stilts (the snow
blows under them), porthole and band windows, shallow domes and drum roofs under a snow cap, outside spiral stairs,
antennas. Nothing gabled, nothing timber. Teal walls, navy stilts and bases, cream bands, red flags, doors and rails,
dark glass, warm windows (steady; every lit piece stands 32 m or more out).

Eight buildings (the pod hostel, the drum cafe, the timing cabin, the ski school, the lift machine hall, the start
house, the weather pod, the board shop), eight props (ski rack, locker pods, ticket gate and snow stake: LOW, 1.3 m
and under; podium, lamp mast, windsock, pennant line: tall) and the hero, THE SUMMIT NEEDLE (56 m with its mast).

What the hill asks (FTDressL5.inl): the road falls 7 in 100 and the land with it, and beside a banked bend the land
is cut down: so every building stands on stilts or a base that goes 4 m into the ground, and the three masts are
footed 12 m down like the level's own race banners. Low props stay 1.3 m and under (the sight line of this hill).
No figures (the people are s1people's), no vehicle, nothing orange, nothing white and round in the air, no words.
What moves (all in the vertex shader): pennants and flags, the windsock, the lift hall's cable wheels (they rock
slowly), the weather pod's cups and vane, the cafe's steam, the shop's board sign, the Needle's long pennants.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
TEAL, NAVY, CREAM, RED, GLASS, WARM = '#19b5c9', '#3d4763', '#f4efe6', '#ff2d3c', '#1d2740', '#ffe28a'
SNOW, STEEL, SLATE, YEL, TEAL_L, WOOD = '#f4f9ff', '#9aa4b8', '#5a6478', '#ffd23f', '#2fc6c0', '#9a6a44'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))     # no moving part: true bounds
LIT = 0.5                                              # a warm window's steady glow
FOOT = -400.0                                          # how far a building's stilts and base go into the ground


# ------------------------------------------------------------------------------------------ helpers
def cone(m, col, a, b, r0, r1=None, steps=8, glow=0.0):
    """A cylinder or a cone from a to b (rod()'s own turn, with a second radius)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def pol(r, a):
    """(x, y) at radius r, angle a in degrees (0 = +X, -90 = the front)."""
    return (r * math.cos(math.radians(a)), r * math.sin(math.radians(a)))


def port(m, x, y, z, r, d=(0.0, -1.0), lit=True, steps=8):
    """A porthole on a wall at (x, y, z), looking along d (in the ground plane): a cream ring and a pane."""
    rod(m, CREAM, (x, y, z), (x + d[0] * 7.0, y + d[1] * 7.0, z), r, steps=steps)
    rod(m, WARM if lit else GLASS, (x, y, z), (x + d[0] * 10.0, y + d[1] * 10.0, z), r * 0.68, steps=6, glow=LIT if lit else 0.0)


def port_r(m, cx, cy, R, a, z, r, lit=True):
    """A porthole on a round wall of radius R about (cx, cy), at angle a."""
    dx, dy = pol(1.0, a)
    port(m, cx + dx * R, cy + dy * R, z, r, (dx, dy), lit)


def capsule(m, col, x0, x1, y, z, r, steps=12):
    """A capsule lying along X from x0 to x1 (the round ends reach 0.6 r further)."""
    rod(m, col, (x0, y, z), (x1, y, z), r, steps=steps)
    for x in (x0, x1):
        m.sphere(col, (x, y, z), r, scale=(0.6, 0.985, 0.985), steps=(4, 10))


def dome(m, col, x, y, z, r, h, steps=12, snow=True):
    """A shallow dome on (x, y, z): two rings of facets, the upper one under snow."""
    m.cyl(col, (x, y, z), r, h * 0.55, steps=steps, top_r=r * 0.68)
    m.cyl(SNOW if snow else col, (x, y, z + h * 0.55), r * 0.7, h * 0.45, steps=steps, top_r=r * 0.22)


def stilts(m, pts, top, r=13.0, col=NAVY, foot=FOOT):
    for (x, y) in pts:
        m.cyl(col, (x, y, foot), r, top - foot, steps=6)


def spiral(m, cx, cy, z0, z1, r, n, a0=0.0, turn=30.0):
    """An outside spiral stair: a navy pole, cream treads, a red rail from tread end to tread end."""
    m.cyl(NAVY, (cx, cy, FOOT), 16, z1 + 110 - FOOT, steps=6)
    last = None
    for i in range(n):
        a = a0 + i * turn
        z = z0 + (z1 - z0) * i / float(n - 1)
        dx, dy = pol(1.0, a)
        m.box(CREAM, (cx + dx * r * 0.55, cy + dy * r * 0.55, z), (r * 0.95, 34, 7), rot=(0, a, 0))
        tip = (cx + dx * r, cy + dy * r, z + 90)
        if last:
            tilt_box(m, RED, last, tip, 6, 6)
        last = tip


def antenna(m, x, y, z, h, col=STEEL):
    m.cyl(col, (x, y, z), 3.5, h, steps=5)
    m.box(col, (x, y, z + h * 0.62), (70, 5, 5))
    m.box(col, (x, y, z + h * 0.8), (44, 5, 5))
    m.sphere(RED, (x, y, z + h), 9, steps=(3, 5))


def pennant(sc, x, y, z, length, h, col, phase, rate=0.4, segs=2, swing=10.0, tip=0.45, col2=None):
    """A pennant streaming along +X from a mast at (x, y): each length of cloth swings a little more than the one
    before it, a little later: a wave down the cloth."""
    seg = length / float(segs)
    for i in range(segs):
        f = sc.part(P.motion(swing=swing + i * swing * 0.6, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase - i * 0.07))
        hh = h * (1.0 - (1.0 - tip) * (i + 0.5) / segs)
        f.box(col if (i % 2 == 0 or not col2) else col2, (x + 6 + seg * (i + 0.5), y, z - i * h * 0.05), (seg + 1.0, 5.0, hh))


def flag_pole(sc, g, x, y, z, h, col, phase, w=110.0, fh=64.0):
    g.cyl(STEEL, (x, y, z), 4.0, h, steps=5)
    pennant(sc, x, y, z + h - fh * 0.5, w, fh, col, phase, rate=0.45, segs=2, swing=12.0, tip=0.8)


def awning(m, x, y, z, w, out=90.0, drop=34.0, n=6):
    """A striped awning over a door or a window on the front wall at y: red and cream by turns."""
    s = w / float(n)
    for i in range(n):
        tilt_box(m, RED if i % 2 == 0 else CREAM, (x - w / 2.0 + s * (i + 0.5), y, z), (x - w / 2.0 + s * (i + 0.5), y - out, z - drop), s, 6)


def rail_ring(m, cx, cy, z, r, n, h=95.0, col=RED):
    """Posts and a top rail round a deck."""
    pts = [(cx + pol(r, k * 360.0 / n)[0], cy + pol(r, k * 360.0 / n)[1]) for k in range(n)]
    for k in range(n):
        m.box(col, (pts[k][0], pts[k][1], z + h / 2.0), (7, 7, h))
        a, b = pts[k], pts[(k + 1) % n]
        tilt_box(m, col, (a[0], a[1], z + h), (b[0], b[1], z + h), 6, 6)


def clock(m, x, y, z, r):
    """A plain dial on the front wall (no figures on it): a cream disc, a navy rim, two hands."""
    rod(m, NAVY, (x, y, z), (x, y - 6, z), r * 1.14, steps=10)
    rod(m, CREAM, (x, y, z), (x, y - 9, z), r, steps=10)
    tilt_box(m, NAVY, (x, y - 11, z), (x + r * 0.45, y - 11, z + r * 0.5), 5, 8)
    tilt_box(m, RED, (x, y - 11, z), (x - r * 0.25, y - 11, z + r * 0.78), 5, 6)


# ------------------------------------------------------------------------------------------ the buildings
def pod_hostel(sc):
    """BUILDING: three teal capsules stacked out of line on navy stilts, a spiral stair up one end. 14 x 3.4 m, 12.6 m."""
    g = sc.still()
    X0 = -110.0
    rows = ((X0 - 90, 420.0), (X0 + 110, 760.0), (X0 - 60, 1100.0))
    R, HL = 160.0, 380.0
    stilts(g, [(rows[0][0] + x, y) for x in (-380, 0, 380) for y in (-105, 105)], 312)
    for x in (-380, 380):
        tilt_box(g, NAVY, (rows[0][0] + x, -105, 40), (rows[0][0] + x, 105, 250), 9, 9)
    tilt_box(g, NAVY, (rows[0][0] - 380, -105, 60), (rows[0][0], -105, 270), 9, 9)
    tilt_box(g, NAVY, (rows[0][0] + 380, -105, 60), (rows[0][0], -105, 270), 9, 9)
    for k, (cx, cz) in enumerate(rows):
        capsule(g, TEAL, cx - HL, cx + HL, 0, cz, R)
        rod(g, CREAM, (cx - HL - 4, 0, cz), (cx - HL + 44, 0, cz), R + 4, steps=10)          # a cream belt at each shoulder
        rod(g, CREAM, (cx + HL - 44, 0, cz), (cx + HL + 4, 0, cz), R + 4, steps=10)
        for s in (-1, 1):                                                                    # the bubble window in each end
            ex = cx + s * (HL + R * 0.6 - 16)
            rod(g, GLASS, (ex, 0, cz), (ex + s * 12, 0, cz), 96, steps=8)
            rod(g, WARM, (ex, 0, cz), (ex + s * 15, 0, cz), 62, steps=8, glow=LIT)
        for px in (-250, 0, 250):
            if k == 0 and px == -250:
                continue                                                                    # the door is there
            port(g, cx + px, -R + 2, cz + 22, 44, lit=(px + k * 250) % 500 != 0)
        g.box(GLASS, (cx, R - 2, cz + 26), (520, 8, 54))                                     # a band window up the back
        g.sphere(SNOW, (cx, 0, cz + R - 12), 120, scale=(3.4, 0.95, 0.26), steps=(4, 8))     # snow lying along the top
    for (cx, cz), (nx, nz) in zip(rows[:-1], rows[1:]):                                      # saddles between the capsules
        for x in (-230, 230):
            g.box(NAVY, ((cx + nx) / 2.0 + x, 0, (cz + nz) / 2.0), (150, 210, 70))
    # the door of the lowest capsule: a red door, a little deck, a ladder, a striped awning
    cx, cz = rows[0]
    g.box(RED, (cx - 250, -R + 4, cz + 4), (78, 10, 160))
    g.box(CREAM, (cx - 250, -R - 2, cz + 44), (30, 6, 30))
    g.box(CREAM, (cx - 250, -R - 50, cz - 82), (190, 110, 10))
    for s in (-1, 1):
        tilt_box(g, NAVY, (cx - 250 + s * 34, -R - 100, cz - 84), (cx - 250 + s * 34, -R - 190, -30), 7, 7)
    for i in range(5):
        g.box(CREAM, (cx - 250, -R - 112 - i * 17, cz - 110 - i * 62), (62, 16, 6))
    awning(g, cx - 250, -R + 6, cz + 122, 190, out=80, drop=30, n=5)
    # the stair tower up the +X end, a gangway to each capsule
    sx = X0 + 700.0
    spiral(g, sx, 0, 30, 1060, 100, 11, a0=-90, turn=45)
    for (cx, cz) in rows:
        end = cx + HL + R * 0.6
        g.box(CREAM, ((end + sx) / 2.0, -70, cz - 62), (sx - end + 30, 46, 8))
        g.box(RED, ((end + sx) / 2.0, -92, cz - 14), (sx - end + 30, 5, 5))
    # the roof: an antenna, a small radar drum, two vents, a red pennant
    tx, tz = rows[2][0], rows[2][1] + R
    antenna(g, tx + 220, 0, tz - 14, 210)
    g.cyl(STEEL, (tx - 250, 0, tz - 16), 46, 40, steps=8)
    g.sphere(CREAM, (tx - 250, 0, tz + 36), 44, scale=(1, 1, 0.7), steps=(4, 8))
    for x in (-60, 40):
        g.cyl(SLATE, (tx + x, 60, tz - 30), 13, 62, steps=6)
        g.cyl(SLATE, (tx + x, 60, tz + 32), 21, 9, steps=6)
    flag_pole(sc, g, sx, 0, 1170, 250, RED, 0.2, w=150, fh=70)


def drum_cafe(sc):
    """BUILDING: a teal drum on a navy stem with a ring terrace 12.4 m across, parasols, a cup on a mast. 9.8 m."""
    g = sc.still()
    g.cyl(NAVY, (0, 0, FOOT), 150, 300 - FOOT, steps=10)
    for k in range(6):
        a = k * 60 + 30
        p0, p1 = pol(430, a), pol(520, a)
        rod(g, NAVY, (p0[0], p0[1], FOOT), (p1[0], p1[1], 372), 13, steps=6)
    g.cyl(NAVY, (0, 0, 290), 190, 90, steps=16, top_r=600)
    g.cyl(CREAM, (0, 0, 380), 620, 30, steps=16)
    rail_ring(g, 0, 0, 410, 604, 12)
    g.cyl(TEAL, (0, 0, 410), 400, 300, steps=16)
    g.cyl(CREAM, (0, 0, 410), 407, 26, steps=16)
    g.cyl(GLASS, (0, 0, 520), 404, 112, steps=16)
    for k in range(8):                                                   # lit panes round the band window
        a = k * 45 + 22.5
        p = pol(404, a)
        g.box(WARM, (p[0], p[1], 576), (8, 150, 84), rot=(0, a, 0), glow=LIT)
    g.cyl(CREAM, (0, 0, 640), 408, 24, steps=16)
    g.cyl(NAVY, (0, 0, 710), 452, 26, steps=16)                          # the eaves
    dome(g, TEAL, 0, 0, 736, 440, 150, steps=16)
    g.cyl(NAVY, (0, 0, 870), 96, 44, steps=8)
    g.sphere(RED, (0, 0, 930), 26, steps=(3, 6))
    g.box(RED, (0, -404, 512), (120, 10, 200))                           # the door, and its awning
    g.box(CREAM, (0, -411, 560), (44, 6, 44))
    awning(g, 0, -404, 636, 260, out=96, drop=34, n=6)
    spiral(g, pol(700, -150)[0], pol(700, -150)[1], 20, 400, 96, 10, a0=30, turn=36)
    for k, a in enumerate((-55, -125, 20, 160)):                         # round tables under furled parasols
        p = pol(515, a)
        g.cyl(CREAM, (p[0], p[1], 478), 44, 7, steps=8)
        g.cyl(NAVY, (p[0], p[1], 410), 5, 250, steps=5)
        g.cyl(RED if k % 2 == 0 else CREAM, (p[0], p[1], 540), 20, 120, steps=6, top_r=5)
        for s in (-1, 1):
            q = pol(515, a + s * 9)
            g.box(TEAL, (q[0], q[1], 440), (26, 26, 60))
    # a plain cup on a mast (the cafe's sign: a shape, no word), steam rising off it
    g.cyl(NAVY, (250, 120, 740), 8, 330, steps=6)
    g.cyl(CREAM, (250, 120, 1070), 46, 92, steps=10, top_r=62)
    g.box(CREAM, (322, 120, 1128), (34, 12, 10))
    g.box(CREAM, (336, 120, 1108), (10, 12, 44))
    g.box(CREAM, (322, 120, 1088), (34, 12, 10))
    g.cyl(RED, (250, 120, 1106), 58, 14, steps=10)
    r = random.Random(11)
    for i in range(3):
        st = sc.part(P.motion(bob=-14.0, rate=0.22, phase=i * 0.33, swing=4, pivot=(250, 120, 1070), axis=0.0))
        st.sphere(SNOW, (250 + r.uniform(-18, 18), 120, 1196 + i * 46), 22 - i * 4, scale=(1, 1, 1.3), steps=(3, 6))
    g.cyl(SLATE, (-200, 160, 760), 22, 150, steps=6)                     # the kitchen's flue, with a cowl
    g.cyl(SLATE, (-200, 160, 910), 34, 12, steps=6)
    g.cyl(SLATE, (-200, 160, 930), 30, 24, steps=6, top_r=6)


def timing_cabin(sc):
    """BUILDING: a glass box on a navy stem, a big results board of coloured blocks beside it (no words). 10 x 4.4 m, 9.6 m."""
    g = sc.still()
    X0 = -160.0
    g.box(NAVY, (X0, 0, (520 + FOOT) / 2.0), (120, 120, 520 - FOOT))
    g.box(NAVY, (X0, 0, 12), (320, 320, 36))
    for sx, sy in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
        tilt_box(g, NAVY, (X0 + sx * 150, sy * 150, 20), (X0 + sx * 50, sy * 50, 300), 12, 12)
        tilt_box(g, NAVY, (X0 + sx * 55, sy * 55, 360), (X0 + sx * 240, sy * 170, 520), 12, 12)
    g.box(CREAM, (X0, 0, 535), (540, 400, 30))
    g.box(GLASS, (X0, 0, 660), (480, 340, 190))
    g.box(TEAL, (X0, 0, 582), (500, 360, 64))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(TEAL, (X0 + sx * 240, sy * 170, 660), (26, 26, 196))
    for i in range(3):
        g.box(WARM, (X0 - 150 + i * 150, -172, 676), (120, 6, 92), glow=LIT)
    for sx in (-1, 1):
        g.box(WARM, (X0 + sx * 242, 0, 676), (6, 220, 92), glow=LIT)
    g.box(TEAL, (X0, 0, 770), (580, 440, 30))
    g.box(SNOW, (X0, 0, 796), (520, 380, 24))
    g.box(RED, (X0, 0, 752), (586, 446, 8))
    antenna(g, X0 + 200, 120, 800, 170)
    g.cyl(STEEL, (X0 - 170, 110, 800), 6, 60, steps=5)                   # a dish
    g.cyl(STEEL, (X0 - 170, 110, 860), 12, 26, steps=8, top_r=56, rot=(-35, 0, 0))
    clock(g, X0, -182, 596, 30)
    # the stair up the -X side
    for s in (-1, 1):
        tilt_box(g, NAVY, (X0 - 560, s * 40, 10), (X0 - 270, s * 40, 530), 8, 14)
    for i in range(8):
        g.box(CREAM, (X0 - 540 + i * 36, 0, 40 + i * 64), (34, 84, 7))
    tilt_box(g, RED, (X0 - 560, -44, 105), (X0 - 270, -44, 625), 6, 6)
    # the results board: a navy panel on two posts, a red header, five rows (a square and a bar: places and times)
    bx = X0 + 610.0
    for s in (-1, 1):
        g.box(NAVY, (bx + s * 190, 10, (780 + FOOT) / 2.0), (22, 22, 780 - FOOT))
    g.box(NAVY, (bx, 0, 520), (480, 30, 540))
    g.box(CREAM, (bx, 0, 794), (500, 36, 12))
    g.box(SNOW, (bx, 0, 808), (470, 44, 18))
    g.box(RED, (bx, -17, 740), (430, 8, 54))
    cols = (YEL, STEEL, WOOD, TEAL, TEAL)
    bars = (300, 270, 250, 210, 170)
    for i in range(5):
        z = 660 - i * 82
        g.box(cols[i], (bx - 180, -17, z), (56, 8, 56))
        g.box(WARM if i == 0 else CREAM, (bx - 130 + bars[i] / 2.0, -17, z), (bars[i], 8, 44), glow=LIT if i == 0 else 0.0)
        g.box(TEAL_L, (bx + 190, -17, z), (26, 8, 26))
    g.box(RED, (bx, 17, 520), (300, 6, 300))                             # the back: a plain red field in a cream frame
    g.box(CREAM, (bx, 16, 520), (360, 4, 360))
    flag_pole(sc, g, X0 - 262, -192, 808, 170, RED, 0.1, w=100, fh=56)
    flag_pole(sc, g, X0 + 262, -192, 808, 170, TEAL, 0.55, w=100, fh=56)


def ski_school(sc):
    """BUILDING: two low domes on stilts joined by a tube, a row of little gates out front. 14.4 x 7.6 m, 5.6 m."""
    g = sc.still()
    A, B = -430.0, 430.0
    for cx, R, H, zb in ((A, 280.0, 170.0, 90.0), (B, 230.0, 150.0, 90.0)):
        g.cyl(NAVY, (cx, 0, zb - 24), R + 12, 24, steps=14)
        stilts(g, [(cx + pol(R - 40, k * 72 + 18)[0], pol(R - 40, k * 72 + 18)[1]) for k in range(5)], zb - 20, r=12)
        g.cyl(TEAL, (cx, 0, zb), R, H, steps=14)
        g.cyl(CREAM if cx == A else RED, (cx, 0, zb + H - 26), R + 5, 22, steps=14)
        g.cyl(NAVY, (cx, 0, zb + H), R + 22, 16, steps=14)
        dome(g, TEAL, cx, 0, zb + H + 16, R + 10, 150 if cx == A else 120, steps=14)
        for a in ((-130, -50, 0, 180) if cx == A else (-140, -90, -40, 0)):
            if cx == A and a == -50:
                continue
            port_r(g, cx, 0, R - 2, a, zb + 86, 40, lit=a in (-130, -90, 0))
    rod(g, TEAL, (A + 250, 0, 180), (B - 200, 0, 180), 84, steps=10)       # the tube between them
    rod(g, CREAM, (A + 268, 0, 180), (A + 296, 0, 180), 90, steps=10)
    rod(g, CREAM, (B - 240, 0, 180), (B - 212, 0, 180), 90, steps=10)
    for x in (-60, 60):
        port(g, x, -82, 190, 30, lit=x > 0)
    g.box(SNOW, (0, 0, 262), (330, 90, 14))
    stilts(g, [(0, -40), (0, 40)], 110, r=10)
    # the door of the big dome: red, under a round canopy, a cream ramp down to the snow
    dp = pol(280, -50)
    dx, dy = pol(1.0, -50)
    g.box(RED, (A + dp[0], dp[1], 176), (100, 12, 164), rot=(0, 40, 0))
    g.box(TEAL_L, (A + dp[0] + dx * 8, dp[1] + dy * 8, 200), (34, 6, 34), rot=(0, 40, 0))
    rod(g, NAVY, (A + dp[0] - dx * 20, dp[1] - dy * 20, 270), (A + dp[0] + dx * 90, dp[1] + dy * 90, 262), 80, steps=8)
    tilt_box(g, CREAM, (A + dp[0] + dx * 10, dp[1] + dy * 10, 90), (A + dp[0] + dx * 200, dp[1] + dy * 200, -10), 110, 10)
    # the school's sign on a mast: a cream six-point star of three bars on a teal disc (a snow crystal: a shape)
    g.cyl(NAVY, (B, 0, 380), 8, 170, steps=6)
    rod(g, TEAL, (B, 6, 600), (B, -6, 600), 78, steps=12)
    rod(g, CREAM, (B, 8, 600), (B, -8, 600), 86, steps=12)
    for a in (0, 60, 120):
        g.box(CREAM, (B, -12, 600), (118, 6, 14), rot=(a, 0, 0))
    g.box(CREAM, (B, 12, 600), (118, 6, 14))
    # the practice gates out front: five little flags that wobble, teal and red by turns
    for i in range(5):
        x = -360 + i * 180
        g.cyl(STEEL, (x, -360, -30), 4, 100, steps=5)
        f = sc.part(P.motion(swing=9, pivot=(x, -360, 0), axis=0.0, rate=0.5, phase=i * 0.21))
        f.box(TEAL if i % 2 == 0 else RED, (x + 24, -360, 82), (44, 5, 34))
    # small skis leaning on the small dome, a flue on each roof, a pennant on the big one
    for i, c in enumerate((RED, YEL, CREAM, TEAL_L, RED)):
        px = B + 110 + i * 26
        tilt_box(g, c, (px, -265, 70), (px, -212 - i * 4, 190), 9, 4)
    g.cyl(SLATE, (A + 120, 90, 300), 14, 110, steps=6)
    g.cyl(SLATE, (A + 120, 90, 410), 22, 10, steps=6)
    flag_pole(sc, g, A, 0, 410, 190, RED, 0.4, w=120, fh=60)


def lift_hall(sc):
    """BUILDING: the lift's machine hall, a teal half cylinder on a navy base with a big cable wheel on each gable
    (they rock slowly) and three spare cabins hung on a rail along the front. 15.6 x 9.4 m, 7.9 m."""
    g = sc.still()
    g.box(NAVY, (0, 0, (260 + FOOT) / 2.0), (1400, 800, 260 - FOOT))
    rod(g, TEAL, (-700, 0, 260), (700, 0, 260), 400, steps=16)
    for x in (-684, -232, 232, 684):
        rod(g, CREAM, (x - 16, 0, 260), (x + 16, 0, 260), 408, steps=16)
    g.box(CREAM, (0, 0, 268), (1410, 812, 16))
    g.box(SNOW, (0, 0, 652), (1330, 250, 24))
    g.box(SNOW, (0, 0, 670), (1220, 140, 22))
    r = random.Random(5)
    for k, s in enumerate((-1, 1)):
        g.box(GLASS, (s * 702, 0, 392), (14, 440, 250))                  # the hall's mouth
        g.box(CREAM, (s * 704, 0, 524), (14, 470, 16))
        g.box(WARM, (s * 708, 0, 330), (6, 300, 60), glow=LIT)
        g.box(NAVY, (s * 730, 0, 600), (80, 60, 60))                     # the axle's bracket
        w = sc.part(P.motion(swing=170, pivot=(s * 770, 0, 600), axis=0.0, rate=0.04, phase=0.5 * k))
        rod(w, NAVY, (s * 752, 0, 600), (s * 792, 0, 600), 34, steps=8)
        pts = [(s * 770, 178 * math.cos(math.radians(a * 30)), 600 + 178 * math.sin(math.radians(a * 30))) for a in range(12)]
        for a in range(12):
            tilt_box(w, RED, pts[a], pts[(a + 1) % 12], 22, 16)
        for a in range(3):
            w.box(CREAM, (s * 770, 0, 600), (12, 340, 14), rot=(0, 0, a * 60))
    # the front: a red roller door with cream chevrons, lit portholes, louvres, three dormers in the vault
    g.box(RED, (-380, -402, 120), (260, 10, 230))
    for z in (70, 150):
        tilt_box(g, CREAM, (-480, -408, z), (-380, -408, z + 50), 6, 16)
        tilt_box(g, CREAM, (-380, -408, z + 50), (-280, -408, z), 6, 16)
    for x in (-130, 20, 170, 320, 470):
        port(g, x, -400, 190, 38, lit=x != 170)
    for x in (560, 620):
        g.box(STEEL, (x, -402, 80), (44, 8, 90))
        for z in (55, 80, 105):
            g.box(SLATE, (x, -407, z), (40, 4, 8))
    for x in (-360, 0, 360):
        g.box(TEAL, (x, -306, 500), (170, 90, 120))
        g.box(WARM, (x, -353, 500), (130, 6, 80), glow=LIT)
        g.box(SNOW, (x, -300, 566), (190, 110, 14))
    # the cabin rail: three spare cabins (teal, red, cream) hanging in a row
    g.box(NAVY, (150, -470, 372), (900, 12, 12))
    for x in (-290, 150, 590):
        tilt_box(g, NAVY, (x, -400, 330), (x, -470, 372), 10, 10)
    for x, c in ((-130, TEAL), (150, RED), (430, CREAM)):
        g.box(NAVY, (x, -470, 345), (10, 8, 50))
        g.box(c, (x, -470, 250), (150, 104, 140))
        g.box(GLASS, (x, -470, 272), (154, 108, 50))
        g.box(NAVY, (x, -470, 324), (120, 80, 10))
        g.box(SNOW, (x, -470, 333), (104, 66, 8))
    antenna(g, -560, 120, 600, 190)
    flag_pole(sc, g, 560, 0, 655, 210, RED, 0.7, w=130, fh=64)


def start_house(sc):
    """BUILDING: the start house, a tall teal drum with a gallery, a ramp door and a row of red flags on its roof. 8 m across, 13.5 m."""
    g = sc.still()
    g.cyl(NAVY, (0, 0, FOOT), 330, 120 - FOOT, steps=14)
    g.cyl(TEAL, (0, 0, 120), 300, 800, steps=14)
    g.cyl(CREAM, (0, 0, 120), 306, 26, steps=14)
    g.cyl(CREAM, (0, 0, 440), 306, 26, steps=14)
    g.cyl(GLASS, (0, 0, 760), 304, 112, steps=14)
    for k in range(7):
        a = k * 360.0 / 7 - 90
        p = pol(304, a)
        g.box(WARM, (p[0], p[1], 816), (8, 130, 82), rot=(0, a, 0), glow=LIT)
    g.cyl(CREAM, (0, 0, 716), 400, 20, steps=14)                          # the gallery
    g.cyl(NAVY, (0, 0, 690), 320, 26, steps=14, top_r=390)
    rail_ring(g, 0, 0, 736, 388, 10, h=90)
    g.cyl(NAVY, (0, 0, 920), 352, 56, steps=14)                           # the drum roof
    g.cyl(RED, (0, 0, 962), 356, 14, steps=14)
    g.cyl(SNOW, (0, 0, 976), 340, 64, steps=14, top_r=190)
    g.cyl(NAVY, (0, 0, 1040), 60, 60, steps=8)
    antenna(g, 0, 0, 1100, 250)
    for k in range(6):                                                     # the row of red flags round the roof
        p = pol(330, k * 60 + 30)
        flag_pole(sc, g, p[0], p[1], 976, 200, RED, k * 0.17, w=110, fh=58)
    # the ramp door: a red door in a cream frame half way up, a short cream ramp with teal rails, a start wand
    g.box(CREAM, (0, -300, 392), (230, 14, 250))
    g.box(RED, (0, -306, 386), (184, 10, 220))
    g.box(GLASS, (0, -312, 430), (120, 6, 60))
    tilt_box(g, CREAM, (0, -300, 276), (0, -556, 60), 200, 12)
    for s in (-1, 1):
        tilt_box(g, TEAL, (s * 104, -300, 330), (s * 104, -556, 114), 8, 60)
        g.box(NAVY, (s * 104, -556, 20), (14, 14, 200))
        g.box(NAVY, (s * 104, -430, 60), (12, 12, 250))
    g.box(RED, (0, -320, 340), (200, 6, 6))
    clock(g, 0, -304, 620, 62)
    bx = 190.0                                                             # the start lights: three in a column, steady
    g.box(NAVY, (bx, -246, 420), (50, 14, 190))
    for i in range(3):
        rod(g, WARM, (bx, -252, 360 + i * 58), (bx, -258, 360 + i * 58), 17, steps=8, glow=LIT)
    for a, z in ((-20, 250), (20, 380), (60, 520), (120, 300), (180, 600), (-150, 560), (-125, 240)):
        port_r(g, 0, 0, 298, a, z, 40, lit=a in (-20, 60, 180, -125))


def weather_pod(sc):
    """BUILDING: the weather pod, a teal ball on three navy legs, instruments on top (cups going round, a vane). 6 m, 9.6 m."""
    g = sc.still()
    for k in range(3):
        a = k * 120 + 90
        p0, p1 = pol(300, a), pol(110, a)
        rod(g, NAVY, (p0[0], p0[1], FOOT), (p1[0], p1[1], 360), 15, steps=6)
        g.cyl(NAVY, (pol(292, a)[0], pol(292, a)[1], -6), 34, 14, steps=6)
    g.cyl(STEEL, (0, 0, FOOT), 28, 330 - FOOT, steps=6)
    g.sphere(TEAL, (0, 0, 520), 230, steps=(6, 12))
    g.cyl(CREAM, (0, 0, 500), 236, 40, steps=12)
    g.cyl(NAVY, (0, 0, 300), 120, 40, steps=10)
    for a in (-90, -30, 30, 90, 150, 210):
        port_r(g, 0, 0, 226, a, 566, 36, lit=a in (-90, 30, 150))
    g.box(RED, (0, -238, 440), (70, 10, 100))                             # a hatch, and a ladder down the front
    for s in (-1, 1):
        tilt_box(g, STEEL, (s * 26, -244, 400), (s * 26, -330, -20), 6, 6)
    for i in range(7):
        g.box(STEEL, (0, -252 - i * 11, 360 - i * 54), (52, 6, 6))
    g.cyl(SNOW, (0, 0, 716), 150, 34, steps=10, top_r=70)                 # snow on the crown
    g.cyl(NAVY, (0, 0, 736), 60, 40, steps=8)
    g.sphere(CREAM, (-70, 40, 800), 60, scale=(1, 1, 0.8), steps=(4, 8))  # a small cream radar dome
    g.cyl(STEEL, (-70, 40, 730), 20, 40, steps=6)
    g.cyl(STEEL, (60, 0, 760), 4, 190, steps=5)                           # the cups' mast
    cups = sc.part(P.motion(base=(60, 0), travel=(-1.0, 0.4)))
    for k in range(3):
        p = pol(46, k * 120)
        cups.box(STEEL, (60 + p[0] / 2.0, p[1] / 2.0, 946), (46, 5, 5), rot=(0, k * 120, 0))
        cups.sphere(RED, (60 + p[0], p[1], 946), 13, steps=(3, 6))
    g.cyl(STEEL, (-10, -90, 740), 4, 150, steps=5)                        # the vane: a teal arrow that hunts the wind
    v = sc.part(P.motion(swing=24, pivot=(-10, -90, 890), axis=P.UP, rate=0.17, phase=0.3))
    v.box(TEAL, (-10, -90, 892), (120, 5, 8))
    v.box(TEAL, (-66, -90, 900), (36, 5, 40))
    v.box(RED, (56, -90, 892), (22, 9, 20))
    g.box(CREAM, (150, 110, 650), (70, 70, 70))                           # a louvred instrument screen on an arm
    for z in (630, 650, 670):
        g.box(SLATE, (150, 72, z), (62, 4, 8))
    tilt_box(g, NAVY, (60, 60, 600), (150, 110, 620), 8, 8)
    g.box(GLASS, (-150, -120, 690), (110, 6, 80), rot=(0, 35, 40))        # a solar panel
    antenna(g, 0, 60, 770, 160)


def board_shop(sc):
    """BUILDING: the board shop, one teal capsule on a deck with a shop window under a striped awning and a giant
    plain snowboard turning slowly on the roof (a shape, no word). 10.6 x 5.4 m, 9 m."""
    g = sc.still()
    R = 190.0
    stilts(g, [(x, y) for x in (-330, 0, 330) for y in (-120, 120)], 190)
    capsule(g, TEAL, -330, 330, 0, 340, R)
    rod(g, CREAM, (-336, 0, 340), (-296, 0, 340), R + 4, steps=12)
    rod(g, CREAM, (296, 0, 340), (336, 0, 340), R + 4, steps=12)
    g.sphere(SNOW, (0, 0, 340 + R - 14), 130, scale=(2.9, 0.95, 0.26), steps=(4, 8))
    for s in (-1, 1):
        ex = s * (330 + R * 0.6 - 16)
        rod(g, GLASS, (ex, 0, 340), (ex + s * 12, 0, 340), 110, steps=8)
        rod(g, WARM, (ex, 0, 340), (ex + s * 15, 0, 340), 70, steps=8, glow=LIT)
    g.box(GLASS, (-70, -R + 6, 350), (380, 14, 150))                      # the shop window, lit, boards on show in it
    for i, c in enumerate((RED, YEL, CREAM, TEAL_L)):
        g.box(WARM, (-205 + i * 90, -R - 2, 350), (78, 6, 130), glow=LIT)
        g.box(c, (-205 + i * 90, -R - 6, 344), (26, 5, 104))
    awning(g, -70, -R + 2, 452, 420, out=100, drop=36, n=7)
    g.box(RED, (220, -R + 8, 322), (84, 12, 170))
    g.box(CREAM, (220, -R + 0, 364), (30, 6, 30))
    g.box(CREAM, (0, -240, 228), (800, 150, 14))                          # the deck, its rail, three steps
    for x in (-390, -130, 130):
        g.box(RED, (x, -310, 280), (7, 7, 96))
    g.box(RED, (-130, -310, 328), (530, 6, 6))
    for i in range(3):
        g.box(CREAM, (300, -330 - i * 34, 190 - i * 62), (150, 40, 10))
    stilts(g, [(-380, -300), (0, -300), (380, -300)], 224, r=9)
    # the sign: a giant plain board on a post, turning slowly from side to side
    g.cyl(NAVY, (-170, 0, 500), 10, 90, steps=6)
    b = sc.part(P.motion(swing=35, pivot=(-170, 0, 590), axis=P.UP, rate=0.07, phase=0.2))
    b.box(RED, (-170, 0, 760), (112, 14, 250))
    rod(b, RED, (-170, -7, 885), (-170, 7, 885), 56, steps=10)
    rod(b, RED, (-170, -7, 635), (-170, 7, 635), 56, steps=10)
    for z, c in ((840, CREAM), (760, TEAL), (680, CREAM)):
        b.box(c, (-170, 0, z), (114, 18, 26))
    for z in (715, 805):
        b.box(NAVY, (-170, -11, z), (64, 8, 18))
    # crossed skis on the other end of the roof, a flue, a pennant
    tilt_box(g, YEL, (150, -10, 510), (290, -10, 700), 6, 16)
    tilt_box(g, CREAM, (290, -18, 510), (150, -18, 700), 6, 16)
    g.cyl(NAVY, (220, 0, 500), 8, 60, steps=6)
    g.cyl(SLATE, (60, 70, 500), 14, 80, steps=6)
    g.cyl(SLATE, (60, 70, 580), 22, 10, steps=6)
    flag_pole(sc, g, 380, 0, 470, 200, TEAL, 0.6, w=120, fh=60)


# ------------------------------------------------------------------------------------------ the props
def ski_rack(sc):
    """PROP, LOW (1.2 m): a rack of skis and boards in the station's colours. 5 x 0.9 m."""
    g = sc.still()
    for x in (-240, 0, 240):
        tilt_box(g, NAVY, (x, -30, -40), (x, 6, 104), 9, 9)
        tilt_box(g, NAVY, (x, 42, -40), (x, 6, 104), 9, 9)
    g.box(NAVY, (0, 6, 100), (500, 9, 9))
    g.box(NAVY, (0, -14, 30), (500, 7, 7))
    cols = (TEAL, RED, CREAM, YEL, TEAL_L, NAVY, RED, TEAL, CREAM, YEL, TEAL, RED)
    for i, c in enumerate(cols):
        x = -214 + i * 39 + (8 if i % 3 == 1 else 0)
        if i % 4 == 2:                                                    # a board
            tilt_box(g, c, (x, -34, 2), (x, 2, 118), 28, 5)
            g.box(NAVY, (x, -18, 62), (22, 8, 10))
        else:                                                             # a pair of skis
            for d in (-6, 6):
                tilt_box(g, c, (x + d, -34, 2), (x + d, 2, 118), 8, 4)
    g.sphere(SNOW, (-120, -8, 0), 60, scale=(2.6, 0.7, 0.2), steps=(3, 8))
    g.sphere(SNOW, (150, 0, 0), 50, scale=(2.2, 0.8, 0.2), steps=(3, 8))


def locker_pods(sc):
    """PROP, LOW (1.15 m): a row of five teal locker pods on a navy plinth, one of them red. 5.2 x 0.9 m."""
    g = sc.still()
    g.box(NAVY, (0, 0, -14), (520, 92, 58))
    g.box(CREAM, (0, 0, 17), (500, 80, 5))
    for i in range(5):
        x = -200 + i * 100
        c = RED if i == 3 else TEAL
        g.cyl(c, (x, 0, 18), 40, 68, steps=8)
        g.cyl(c, (x, 0, 86), 40, 22, steps=8, top_r=20)
        g.cyl(SNOW, (x, 0, 106), 22, 8, steps=8, top_r=8)
        g.box(CREAM, (x, -39, 56), (36, 5, 52))
        rod(g, GLASS, (x, -40, 68), (x, -45, 68), 9, steps=6)
        g.box(NAVY, (x + 10, -43, 44), (6, 5, 12))
    g.box(STEEL, (-252, -30, 40), (10, 10, 110))
    g.box(TEAL_L, (-252, -37, 86), (20, 6, 16))


def ticket_gate(sc):
    """PROP, LOW (1.15 m): the lift's ticket gate, three teal posts with red bars on a navy plate. 4.4 x 1.2 m."""
    g = sc.still()
    g.box(NAVY, (0, 0, -16), (440, 120, 48))
    for i in range(3):
        x = -150 + i * 150
        g.cyl(TEAL, (x, 0, 8), 27, 84, steps=8)
        g.cyl(TEAL, (x, 0, 92), 27, 16, steps=8, top_r=14)
        g.box(CREAM, (x, -26, 76), (30, 6, 22))
        g.box(TEAL_L, (x, -30, 80), (12, 4, 8))
        for a in (0, 120, 240):
            p = pol(1.0, a)
            tilt_box(g, RED, (x + 30, 0, 60), (x + 30 + 22, p[0] * 40, 60 + p[1] * 40), 7, 7)
    for s in (-1, 1):
        g.box(CREAM, (s * 205, 0, 44), (9, 9, 76))
        g.box(CREAM, (s * 205, 0, 84), (9, 110, 7))
    g.sphere(SNOW, (186, 34, 8), 36, scale=(1.4, 1, 0.3), steps=(3, 6))


def snow_stake(sc):
    """PROP, LOW (1.3 m): the snow-measuring stake, banded red and cream, a ring gauge on it, a round bench about it. 3 m across."""
    g = sc.still()
    for i in range(6):
        g.box(RED if i % 2 == 0 else CREAM, (0, 0, -29 + i * 26 + 13), (10, 10, 26))
    g.box(NAVY, (0, 0, 127), (16, 16, 6))
    for k in range(8):                                                    # the ring gauge
        a0, a1 = pol(30, k * 45), pol(30, k * 45 + 45)
        tilt_box(g, TEAL, (a0[0], a0[1], 66), (a1[0], a1[1], 66), 6, 8)
    for a in (0, 90):
        g.box(TEAL, (0, 0, 66), (60, 5, 5), rot=(0, a, 0))
    for k in range(8):                                                    # the bench: eight planks round
        a = k * 45
        p = pol(116, a)
        g.box(CREAM if k % 2 == 0 else TEAL, (p[0], p[1], 42), (38, 92, 8), rot=(0, a, 0))
        if k % 2 == 0:
            g.box(NAVY, (p[0], p[1], 6), (30, 12, 72), rot=(0, a, 0))
    g.sphere(SNOW, (0, 0, 2), 80, scale=(1, 1, 0.24), steps=(3, 8))


def podium(sc):
    """PROP (tall, 5.6 m): the winners' podium, three plain steps before a teal board, three pennants; nobody on it. 5.6 x 2 m."""
    g = sc.still()
    for x, h, c, medal in ((0, 110, CREAM, YEL), (-165, 78, TEAL, STEEL), (165, 52, TEAL, WOOD)):
        g.box(c, (x, 0, (h - 100) / 2.0), (160, 150, h + 100))
        g.box(RED if x == 0 else CREAM, (x, 0, h + 4), (168, 158, 8))
        rod(g, medal, (x, -76, h * 0.55), (x, -81, h * 0.55), 22, steps=8)
    g.box(NAVY, (0, 20, -20), (560, 200, 44))
    for s in (-1, 1):
        g.box(NAVY, (s * 262, 96, 170), (14, 14, 540))
    g.box(TEAL, (0, 96, 330), (530, 10, 190))
    g.box(CREAM, (0, 96, 432), (540, 14, 14))
    for i in range(8):
        g.box(RED if i % 2 == 0 else CREAM, (-231 + i * 66, 89, 252), (66, 6, 34))
    for i in range(3):                                                    # three plain blocks of colour on the board
        g.box((CREAM, TEAL_L, CREAM)[i], (-150 + i * 150, 89, 350), (96, 6, 84))
    g.box(SNOW, (0, 96, 446), (500, 30, 14))
    for i, (x, h, c) in enumerate(((0, 560, RED), (-262, 500, TEAL), (262, 500, CREAM))):
        g.cyl(STEEL, (x, 110, 420), 4, h - 420, steps=5)
        pennant(sc, x, 110, h - 36, 120, 62, c, i * 0.31, rate=0.45, segs=2, swing=12.0, tip=0.75)
    g.box(RED, (-300, -60, 30), (10, 10, 120))                            # the race bell on its post
    g.cyl(YEL, (-300, -60, 92), 18, 26, steps=8, top_r=8)


def lamp_mast(sc):
    """PROP (tall, 7.3 m): a porthole lamp mast, three round lamps on arms, steady. Footed 12 m down."""
    g = sc.still()
    g.cyl(NAVY, (0, 0, -1200), 10, 1850, steps=6, top_r=7)
    g.cyl(TEAL, (0, 0, 100), 15, 46, steps=8)
    g.cyl(CREAM, (0, 0, 146), 15, 10, steps=8)
    g.cyl(NAVY, (0, 0, -8), 26, 16, steps=8)
    for k in range(3):
        a = k * 120 - 90
        p, q = pol(78, a), pol(40, a)
        tilt_box(g, NAVY, (0, 0, 560), (p[0], p[1], 606), 7, 7)
        g.cyl(CREAM, (p[0], p[1], 586), 32, 26, steps=8)
        g.cyl(WARM, (p[0], p[1], 578), 25, 8, steps=8, glow=0.8)
        g.cyl(SNOW, (p[0], p[1], 612), 28, 10, steps=8, top_r=10)
        tilt_box(g, NAVY, (q[0], q[1], 520), (p[0], p[1], 590), 5, 5)
    g.cyl(TEAL, (0, 0, 650), 16, 34, steps=8, top_r=5)
    g.sphere(RED, (0, 0, 696), 11, steps=(3, 6))
    f = sc.part(P.motion(swing=8, pivot=(0, 0, 470), axis=0.0, rate=0.3, phase=0.4))   # a little pendant on the mast
    f.box(RED, (0, -12, 430), (40, 5, 46))
    f.box(CREAM, (0, -12, 396), (24, 5, 24))
    g.box(NAVY, (0, -8, 462), (50, 8, 6))


def windsock(sc):
    """PROP (tall, 8.3 m): a windsock, red and cream, swinging on its mast. Footed 12 m down."""
    g = sc.still()
    g.cyl(STEEL, (0, 0, -1200), 9, 2010, steps=6, top_r=6)
    g.cyl(RED, (0, 0, 60), 12, 50, steps=8)
    g.cyl(CREAM, (0, 0, 110), 12, 50, steps=8)
    g.cyl(RED, (0, 0, 160), 12, 50, steps=8)
    g.cyl(NAVY, (0, 0, -8), 24, 16, steps=8)
    g.sphere(RED, (0, 0, 818), 11, steps=(3, 6))
    for i in range(5):
        s = sc.part(P.motion(swing=12 + i * 5, pivot=(0, 0, 770), axis=P.UP, rate=0.33, phase=0.1 - i * 0.06))
        x0 = 14 + i * 52
        cone(s, RED if i % 2 == 0 else CREAM, (x0, 0, 770 - i * 5), (x0 + 54, 0, 765 - i * 5), 36 - i * 4.5, 31.5 - i * 4.5, steps=8)
    g.box(STEEL, (0, 0, 742), (5, 80, 5))                                  # the hoop's yoke
    g.box(STEEL, (0, 0, 800), (5, 80, 5))


def flag_line(sc):
    """PROP (tall, 6.4 m): a line of pennants between two navy masts, teal, red and cream. 12 m along the road. Footed 12 m down."""
    g = sc.still()
    for s in (-1, 1):
        g.cyl(NAVY, (s * 580, 0, -1200), 10, 1820, steps=6, top_r=7)
        g.cyl(TEAL, (s * 580, 0, 90), 15, 44, steps=8)
        g.cyl(NAVY, (s * 580, 0, -8), 26, 16, steps=8)
        g.sphere(RED, (s * 580, 0, 632), 13, steps=(3, 6))
        tilt_box(g, STEEL, (s * 580, 0, 600), (0, 0, 548), 5, 5)
    cols = (TEAL, RED, CREAM)
    for i in range(9):
        x = -464 + i * 116
        z = 548 + 52 * abs(x) / 580.0 - 4
        f = sc.part(P.motion(swing=20, pivot=(x, 0, z), axis=0.0, rate=0.55, phase=i * 0.13))
        f.box(cols[i % 3], (x, 0, z - 20), (56, 5, 40))
        f.box(cols[i % 3], (x, 0, z - 56), (30, 5, 32))


# ------------------------------------------------------------------------------------------ the hero
def summit_needle(sc):
    """HERO: THE SUMMIT NEEDLE. A navy needle carrying a teal saucer (a viewing deck 22 m across: a ring of dark glass
    with lit panes, a cream underside), a red mast on top with three long pennants streaming. 56 m to the mast's tip."""
    g = sc.still()
    g.cyl(NAVY, (0, 0, -600), 720, 650, steps=16)
    g.cyl(CREAM, (0, 0, 50), 600, 36, steps=16)
    g.cyl(NAVY, (0, 0, 86), 420, 60, steps=16, top_r=300)
    g.cyl(NAVY, (0, 0, 140), 300, 1300, steps=12, top_r=160)
    g.cyl(NAVY, (0, 0, 1440), 160, 2000, steps=12, top_r=100)
    for z, r in ((760, 236), (1440, 172), (2300, 140), (3000, 118)):
        g.cyl(CREAM, (0, 0, z), r, 44, steps=12)
    for k in range(3):                                                    # three buttress fins, a cream edge on each
        a = k * 120 + 30
        p0, p1 = pol(690, a), pol(150, a)
        tilt_box(g, NAVY, (p0[0], p0[1], 40), (p1[0], p1[1], 1560), 150, 44)
        q0, q1 = pol(720, a), pol(176, a)
        tilt_box(g, CREAM, (q0[0], q0[1], 70), (q1[0], q1[1], 1590), 20, 50)
    for k in range(3):                                                    # three entrance pods round the foot
        a = k * 120 - 90
        p = pol(560, a)
        d = pol(1.0, a)
        g.cyl(TEAL, (p[0], p[1], 50), 170, 170, steps=10)
        dome(g, TEAL, p[0], p[1], 220, 180, 110, steps=10)
        g.box(RED, (p[0] + d[0] * 168, p[1] + d[1] * 168, 140), (10, 90, 150), rot=(0, a, 0))
        g.box(WARM, (p[0] + d[0] * 172, p[1] + d[1] * 172, 176), (6, 50, 50), rot=(0, a, 0), glow=LIT)
    # lit slits winding up the shaft, and the lift's car on its rail up the front
    for i in range(10):
        a = -90 + i * 47
        z = 520 + i * 270
        r = 300 - (300 - 160) * min(1.0, (z - 140) / 1300.0) if z < 1440 else 160 - 60 * (z - 1440) / 2000.0
        p = pol(r + 1, a)
        g.box(WARM, (p[0], p[1], z), (8, 34, 110), rot=(0, a, 0), glow=LIT)
    tilt_box(g, CREAM, (0, -306, 150), (0, -112, 3300), 12, 30)
    g.box(TEAL, (0, -236, 1900), (120, 96, 200))
    g.box(GLASS, (0, -246, 1930), (124, 84, 80))
    g.box(RED, (0, -236, 2006), (126, 100, 12))
    # a crow's nest half way up
    g.cyl(NAVY, (0, 0, 2360), 150, 60, steps=12, top_r=270)
    g.cyl(CREAM, (0, 0, 2420), 280, 20, steps=12)
    rail_ring(g, 0, 0, 2440, 268, 8, h=90)
    # the saucer
    for k in range(6):
        a = k * 60
        p0, p1 = pol(112, a), pol(800, a)
        rod(g, NAVY, (p0[0], p0[1], 3000), (p1[0], p1[1], 3550), 15, steps=6)
    g.cyl(CREAM, (0, 0, 3350), 112, 270, steps=24, top_r=1100)
    g.cyl(TEAL, (0, 0, 3620), 1124, 60, steps=24)
    g.cyl(GLASS, (0, 0, 3680), 1070, 170, steps=24)
    for k in range(12):
        a = k * 30 + 15
        p = pol(1070, a)
        g.box(WARM, (p[0], p[1], 3765), (8, 300, 110), rot=(0, a, 0), glow=LIT)
    for k in range(12):                                                   # the deck's rail on the rim
        p = pol(1100, k * 30)
        g.box(RED, (p[0], p[1], 3725), (8, 8, 90))
    g.cyl(CREAM, (0, 0, 3850), 1130, 30, steps=24)
    g.cyl(TEAL, (0, 0, 3880), 1124, 250, steps=24, top_r=420)
    g.cyl(SNOW, (0, 0, 4050), 660, 84, steps=24, top_r=410)
    g.cyl(NAVY, (0, 0, 4120), 380, 150, steps=16)
    for k in range(8):
        port_r(g, 0, 0, 378, k * 45 + 22.5, 4196, 44, lit=k % 2 == 0)
    g.cyl(CREAM, (0, 0, 4270), 400, 22, steps=16)
    g.cyl(TEAL, (0, 0, 4292), 392, 90, steps=16, top_r=150)
    g.cyl(SNOW, (0, 0, 4362), 200, 40, steps=16, top_r=60)
    # the red mast, its yards, the pennants
    g.cyl(RED, (0, 0, 4380), 40, 1180, steps=8, top_r=12)
    for z, r in ((4700, 38), (5050, 30)):
        g.cyl(CREAM, (0, 0, z), r, 60, steps=8)
    g.box(STEEL, (0, 0, 4880), (8, 300, 8))
    g.box(STEEL, (0, 0, 5200), (200, 8, 8))
    g.sphere(CREAM, (0, 0, 5574), 26, steps=(4, 8))
    pennant(sc, 0, 0, 5430, 1040, 150, RED, 0.0, rate=0.22, segs=5, swing=7.0, tip=0.3)
    pennant(sc, 0, 0, 5120, 820, 120, TEAL, 0.35, rate=0.25, segs=4, swing=7.0, tip=0.3)
    pennant(sc, 0, 0, 4790, 620, 100, CREAM, 0.7, rate=0.28, segs=4, swing=7.0, tip=0.35, col2=RED)


JOBS = [
    ('SM_S1D1_PodHostel', pod_hostel),
    ('SM_S1D1_DrumCafe', drum_cafe),
    ('SM_S1D1_TimingCabin', timing_cabin),
    ('SM_S1D1_SkiSchool', ski_school),
    ('SM_S1D1_LiftHall', lift_hall),
    ('SM_S1D1_StartHouse', start_house),
    ('SM_S1D1_WeatherPod', weather_pod),
    ('SM_S1D1_BoardShop', board_shop, dict(grow=(120, 120, 60), grow_down=(120, 120, 0))),
    ('SM_S1D1_SkiRack', ski_rack, STILL),
    ('SM_S1D1_LockerPods', locker_pods, STILL),
    ('SM_S1D1_TicketGate', ticket_gate, STILL),
    ('SM_S1D1_SnowStake', snow_stake, STILL),
    ('SM_S1D1_Podium', podium),
    ('SM_S1D1_LampMast', lamp_mast, dict(grow=(20, 20, 0), grow_down=(20, 20, 0))),
    ('SM_S1D1_Windsock', windsock, dict(grow=(40, 220, 20), grow_down=(40, 220, 0))),
    ('SM_S1D1_FlagLine', flag_line, dict(grow=(20, 40, 0), grow_down=(20, 40, 0))),
    ('SM_S1D1_SummitNeedle', summit_needle, dict(far=True, grow=(100, 700, 80), grow_down=(100, 700, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
