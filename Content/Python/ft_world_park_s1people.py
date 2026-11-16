"""Theme Park, Stage 1, builder s1people: the people pass 3 left out. /Game/World/ThemePark/SM_S1PEOPLE_*.
Run: import ft_world_park_s1people as K; K.run()

Every piece brings its own raised floor (2 m and more over the land as built, 2.6 m and more as placed at 1.3: the
land lies under the road, the barrier is 1.6 m and the verge has a 1.8 m hoarding), so the figures clear the barrier.

  THE SHOW ROW (24 pieces, 2.8 x 1.75 m as built, placed at 1.3 in the parade lane 26 m out, seven to a slot, either
  side of the crowd before a stall row on the right of the Last Ride): six a quarter, each doing what its quarter does.
      Sweet Street     Sw_Ices  Sw_Toffee  Sw_Wagon  Sw_Window  Sw_Baker  Sw_Bench
      Crescent Keep    Kp_Heralds  Kp_Juggler  Kp_Dance  Kp_Painter  Kp_Bridge  Kp_Stargazer
      Snowglobe Square Sn_Rink  Sn_Sledge  Sn_Snowman  Sn_Mugs  Sn_Bell  Sn_Skis
      Giant's Garden   Gd_Ladder  Gd_TeaCup  Gd_Toadstools  Gd_Hose  Gd_Beans  Gd_Thimbles
  THE GAPS BEFORE THE LAST RIDE (5 x 3 m as built, placed at 1.3, 40 m out and more):
      JumpTerrace, JumpSteps   the big jump's valley side: a stepped terrace of guests, a photographers' riser
      CoveDeck_A, CoveDeck_B   Pirate Cove's bend and loop: crooked stilt decks (net menders; a look-out, a map, a barrel)
      BackLotDock              the back lot's bend: stage hands push a painted flat along a loading dock
  THE LAGOON (8 x 3.8 m, deck 23.5 m up on piles with a 6 m foot, placed at 1.15 by Far rules: deck at the rails):
      LagoonJetty_A (anglers under a pagoda lamp), LagoonJetty_B (tea under a jellyfish dome, a springboard)

Light: every colour carries a small steady glow (lift(): 0.08, a night level) and there are rows of steady lamps;
every rule of the block stands 26 m out or more; nothing blinks. No limb swings faster than 1.5 a second.
No clown, no balloon, no pie or cake, nothing thrown, nothing circling, no crate or chest, nothing on a rotor.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'

CREAM, WHITE, RED, MAG, AMBER, GOLD = '#fff1d6', '#fff6ea', '#ff2d3c', '#ff4fb8', '#ffd23f', '#ffc93c'
CYAN, TEAL, LAV, PURPLE, PLUM, NAVY = '#2ee6ff', '#2fc6c0', '#b9a4ff', '#3a2f6a', '#4a3a78', '#2b2350'
WOOD, WOOD_D, TIN, TIN_D, PEACH, KRAKEN = '#9a6a44', '#6a4c36', '#a8b4d0', '#7f8aa6', '#ffc59e', '#c2408f'
POOL, SPRAY, LAMP, LEAF, LEAF_D = '#3a5fa8', '#dff6ff', '#ffe28a', '#4f9a6a', '#2f6b52'

TOPS = (MAG, CYAN, AMBER, RED, WHITE, LAV, '#8f6bff', '#7dffb0', '#57c7ff')
BOTTOMS = ('#2b2350', '#3a5fa8', '#f1e4c8', '#7a8fb8', '#c94f6a', '#e8e0f0')
FOOT = 250.0
HX, HY = 140.0, 85.0          # a show-row piece: half its length and half its depth
GLOW = 0.08


def R(n):
    return random.Random(n)


def slab(g, col, x0, x1, y0, y1, z0, z1, glow=0.0):
    g.box(col, ((x0 + x1) / 2.0, (y0 + y1) / 2.0, (z0 + z1) / 2.0), (x1 - x0, y1 - y0, z1 - z0), glow=glow)


def lamps(g, a, b, n, size=11.0, col=LAMP, glow=1.3):
    """A row of steady lamps from a to b."""
    for k in range(n):
        f = (k + 0.5) / n
        g.box(col, (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f), (size, size, size), glow=glow)


def fig(sc, r, x, y, z, yaw=0.0, **kw):
    d = P.look(r, tops=TOPS, bottoms=BOTTOMS)
    d.update(kw)
    d.setdefault('phase', r.random())
    d.setdefault('detail', 2)
    return P.figure(sc, x, y, z, yaw, **d)


def kid(r, **kw):
    d = dict(build='kid', s=r.uniform(0.62, 0.7))
    d.update(kw)
    return d


def wave(r, amp=22, **kw):
    d = dict(arm_r='wave', swing_r=amp, rate=r.uniform(0.8, 1.3))
    d.update(kw)
    return d


def cheer(r, **kw):
    d = dict(arm_l='vee', arm_r='vee', bob=r.uniform(4, 7), rate=r.uniform(0.9, 1.3))
    d.update(kw)
    return d


def together(sc, base, fn):
    """Everything fn() draws that travels takes ONE place on the line: a walker and what he pushes come round as one."""
    before = set(sc.parts)
    out = fn()
    for k in sc.order:
        if k not in before:
            mo = sc.parts[k][1]
            if mo['travel'][0] > 0.5:
                mo['base'] = (float(base[0]), float(base[1]))
    return out


def lift(sc, g=GLOW):
    """A night level: every colour gets a small steady glow of its own (all these pieces stand 26 m out or more)."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn):
    def build(sc):
        fn(sc)
        lift(sc)
    build.__doc__ = fn.__doc__
    return build


def pennants(sc, r, x0, x1, y, z, cols, n=6):
    g = sc.still()
    g.box(WHITE, ((x0 + x1) / 2.0, y, z), (x1 - x0, 4, 4))
    for k in range(n):
        x = x0 + (k + 0.5) * (x1 - x0) / n
        f = sc.part(P.motion(swing=12, pivot=(x, y, z), axis=0.0, rate=r.uniform(0.5, 0.8), phase=r.random()))
        f.box(cols[k % len(cols)], (x, y, z - 17), (min(30.0, (x1 - x0) / n - 6), 4, 30))


def flag(sc, r, x, y, z0, h, col, pole=WHITE, w=44.0, drop=30.0, d=1):
    """A flag on a pole; d = -1 flies it towards -X (a pole at the +X end of a piece)."""
    g = sc.still()
    g.box(pole, (x, y, z0 + h / 2.0), (5, 5, h))
    f = sc.part(P.motion(swing=14, pivot=(x, y, z0 + h), axis=P.UP, rate=r.uniform(0.4, 0.6), phase=r.random()))
    f.box(col, (x + d * (w / 2.0 + 2), y, z0 + h - drop / 2.0 - 2), (w, 4, drop))


def vane(sc, r, x, y, z, shape='star', col=GOLD):
    """A weather vane: an arrow and a star, a moon or a fish, turning a little in the wind."""
    g = sc.still()
    g.box(TIN, (x, y, z + 22), (5, 5, 44))
    v = sc.part(P.motion(swing=28, pivot=(x, y, z + 44), axis=P.UP, rate=r.uniform(0.15, 0.25), phase=r.random()))
    v.box(col, (x, y, z + 46), (62, 5, 5))
    v.box(col, (x - 30, y, z + 46), (10, 5, 16))
    if shape == 'star':
        v.box(col, (x + 22, y, z + 62), (20, 5, 20))
        v.box(col, (x + 22, y, z + 62), (20, 5, 20), rot=(45, 0, 0))
    elif shape == 'moon':
        for a in (-60, -30, 0, 30, 60):
            v.box(col, (x + 22 - math.cos(math.radians(a)) * 12, y, z + 66 + math.sin(math.radians(a)) * 12), (9, 5, 9))
    else:
        v.box(col, (x + 18, y, z + 62), (30, 5, 14))
        v.box(col, (x + 37, y, z + 62), (8, 5, 22))


# ---------------------------------------------------------------- what a hand holds (nothing thinner than 4 cm)
def h_cone(scoop=MAG):
    def f(m, h, side):
        m.cyl(PEACH, (h[0], h[1] - 2, h[2] + 2), 3, 16, steps=6, top_r=7)
        m.sphere(scoop, (h[0], h[1] - 2, h[2] + 23), 8, steps=(3, 6))
    return f


def h_tray(col=WHITE, a=MAG, b=WHITE):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 10, h[2] + 9), (58, 34, 5))
            for k, c in enumerate((a, b, a)):
                m.cyl(PEACH, (h[0] - 42 + k * 18, h[1] - 10, h[2] + 11), 3, 12, steps=6, top_r=6)
                m.sphere(c, (h[0] - 42 + k * 18, h[1] - 10, h[2] + 27), 7, steps=(3, 6))
    return f


def h_horn(col=GOLD, banner=CYAN):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] - 4, h[2] + 4), (h[0], h[1] - 74, h[2] + 36), 6, 6)
        m.box(col, (h[0], h[1] - 78, h[2] + 38), (15, 10, 15))
        m.box(banner, (h[0], h[1] - 42, h[2] + 3), (5, 34, 30))
    return f


def h_club(col=CYAN, cap=GOLD):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 18), (7, 7, 34))
        m.box(cap, (h[0], h[1], h[2] + 40), (11, 11, 12))
    return f


def h_stick(col=WOOD, tip=MAG, length=34.0):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + length / 2.0), (5, 5, length))
        m.box(tip, (h[0], h[1], h[2] + length + 5), (8, 8, 12))
    return f


def h_pennant(col=MAG, pole=WHITE):
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + 42), (5, 5, 100))
        m.box(col, (h[0] + side * 26, h[1], h[2] + 78), (48, 4, 26))
    return f


def h_can(col=TEAL):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 6, h[2] - 10), (16, 24, 20))
        tilt_box(m, col, (h[0], h[1] - 16, h[2] - 8), (h[0], h[1] - 40, h[2] + 4), 6, 6)
    return f


def h_rod(col=WOOD, length=200.0):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 16, h[2] - 6), (h[0], h[1] - length * 0.8, h[2] + length * 0.55), 5, 5)
    return f


def h_fish(col=SPRAY):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 22), (10, 16, 44))
        m.box(col, (h[0], h[1], h[2] + 48), (5, 26, 12))
    return f


def h_sheet(col=CREAM, mark=GOLD, w=56.0, hh=38.0):
    """A map or a chart held in both hands: a pale sheet with a compass cross (no words)."""
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 6, h[2] + 12), (w, 4, hh))
            m.box(mark, (h[0] - 24, h[1] - 8.6, h[2] + 12), (w * 0.5, 4, 5))
            m.box(mark, (h[0] - 24, h[1] - 8.6, h[2] + 12), (5, 4, hh * 0.6))
    return f


def h_fiddle(col=WOOD):
    def f(m, h, side):
        m.box(col, (h[0] - side * 6, h[1] - 6, h[2] + 12), (20, 44, 9))
        m.box(NAVY, (h[0] - side * 6, h[1] - 34, h[2] + 14), (6, 22, 5))
    return f


def h_bow(col=WHITE):
    def f(m, h, side):
        m.box(col, (h[0] - side * 22, h[1], h[2] + 6), (58, 4, 4))
    return f


def h_ski(col=MAG, tip=WHITE, length=170.0):
    """A ski or a sledge board held upright beside the body."""
    def f(m, h, side):
        m.box(col, (h[0] + side * 9, h[1], h[2] + 30), (7, 40, length))
        m.box(tip, (h[0] + side * 9, h[1], h[2] + 30 + length / 2.0 + 5), (7, 40, 10))
    return f


def h_broom(col=WOOD, head=AMBER):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1], h[2] + 30), (h[0], h[1] - 30, h[2] - 80), 5, 5)
        m.box(head, (h[0], h[1] - 33, h[2] - 88), (34, 9, 16))
    return f


MUG = P.hold_cup(RED)
CUP = P.hold_cup(WHITE)
CAM = P.hold_camera(col=WHITE, lens=CYAN)


# ================================================================ SWEET STREET (9.30 to 11.94 km)
def choc_block(g, top, x0=-HX, x1=HX, y0=-HY, y1=HY, drips=7, seed=1):
    """A chocolate plinth, a wafer panel on its front, an icing cap that drips over the edge."""
    r = R(seed)
    slab(g, WOOD_D, x0 + 5, x1 - 5, y0 + 5, y1 - 5, -FOOT, top - 16)
    slab(g, PEACH, x0 + 14, x1 - 14, y0 + 1, y0 + 9, 34, top - 40)
    for k in range(1, 5):
        x = x0 + 14 + k * (x1 - x0 - 28) / 5.0
        slab(g, WOOD_D, x - 3, x + 3, y0 - 1, y0 + 6, 34, top - 40)
    zc = (34 + top - 40) / 2.0
    slab(g, WOOD_D, x0 + 14, x1 - 14, y0 - 1, y0 + 6, zc - 3, zc + 3)
    slab(g, WHITE, x0, x1, y0, y1, top - 16, top)
    for k in range(drips):
        x = x0 + (k + 0.5) * (x1 - x0) / drips + r.uniform(-8, 8)
        slab(g, WHITE, x - 9, x + 9, y0, y0 + 8, top - 16 - r.uniform(14, 40), top - 14)


def cane(g, x, y, z, h=150.0, col=MAG, hook=1):
    g.cyl(WHITE, (x, y, z), 7, h, steps=6)
    for k in range(3):
        g.cyl(col, (x, y, z + 16 + k * h / 3.1), 7.8, 12, steps=6)
    tilt_box(g, WHITE, (x, y, z + h - 4), (x + hook * 16, y, z + h + 12), 12, 12)
    tilt_box(g, col, (x + hook * 16, y, z + h + 12), (x + hook * 30, y, z + h), 12, 12)


def icing_roof(g, x0, x1, y0, y1, z, seed=3, th=26.0):
    r = R(seed)
    slab(g, WHITE, x0, x1, y0, y1, z, z + th)
    slab(g, WHITE, x0 + 16, x1 - 16, y0 + 12, y1 - 8, z + th, z + th + 14)
    n = max(3, int((x1 - x0) / 38))
    for k in range(n):
        x = x0 + (k + 0.5) * (x1 - x0) / n
        slab(g, WHITE, x - 8, x + 8, y0, y0 + 8, z - r.uniform(10, 30), z + 2)


def sw_ices(sc):
    """Three guests and a child licking ices on wafer steps under a line of pennants."""
    r = R(101)
    g = sc.still()
    choc_block(g, 200, seed=11)
    slab(g, PEACH, -HX + 4, HX - 4, 8, HY - 2, 200, 232)
    slab(g, WHITE, -HX, HX, 4, HY, 232, 246)
    lamps(g, (-HX + 12, 2, 222), (HX - 12, 2, 222), 7)
    cane(g, -126, 66, 246, 150, MAG, 1)
    cane(g, 126, 66, 246, 150, KRAKEN, -1)
    pennants(sc, r, -118, 118, 66, 392, (MAG, WHITE, KRAKEN, GOLD), 7)
    fig(sc, r, -86, -40, 200, 12, arm_r='face', hold_r=h_cone(MAG), swing_r=9, axis_r='x', rate=0.6)
    fig(sc, r, 74, -40, 200, -14, arm_l='face', hold_l=h_cone(WHITE), swing_l=9, axis_l='x', rate=0.55, arm_r='wave', swing_r=18)
    fig(sc, r, 0, -44, 200, 0, hold_r=h_cone(KRAKEN), **kid(r, **cheer(r)))
    fig(sc, r, 0, 46, 246, 0, hold_l=h_cone(MAG), **wave(r, 26, arm_l='hold'))


def sw_toffee(sc):
    """Two confectioners pull a rope of toffee from a hook; a child hops before them."""
    r = R(102)
    g = sc.still()
    choc_block(g, 205, seed=12)
    g.box(WOOD_D, (0, 44, 205 + 85), (12, 12, 170))
    tilt_box(g, GOLD, (0, 44, 370), (0, 30, 362), 7, 7)
    cane(g, -128, 72, 205, 190, MAG, 1)
    cane(g, 128, 72, 205, 190, MAG, -1)
    for k in range(8):                                             # a striped valance between the canes
        slab(g, (MAG, WHITE)[k % 2], -120 + k * 30, -90 + k * 30, 68, 76, 372, 400)
    slab(g, WHITE, -128, 128, 62, 82, 400, 414)
    ph = 0.2
    t = sc.part(P.motion(swing=4, pivot=(0, 30, 362), axis=0.0, rate=0.7, phase=ph))
    for sx, c in ((-1, KRAKEN), (1, MAG)):
        tilt_box(t, c, (0, 30, 360), (sx * 34, 6, 330), 11, 11)
        tilt_box(t, PEACH, (sx * 34, 6, 330), (sx * 60, -14, 312), 11, 11)
    for sx in (-1, 1):
        fig(sc, r, sx * 84, -4, 205, -sx * 55, top=WHITE, bottom=NAVY, hat='chef', hat_col=WHITE, band=MAG, build='stocky',
            arm_l='fwd', arm_r='fwd', rock=4, rate=0.7, phase=ph)
    fig(sc, r, 6, -50, 205, 0, **kid(r, **cheer(r)))


def sw_wagon(sc):
    """An ice wagon under a striped canopy: a baker with a tray of cones, a seller waving."""
    r = R(103)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            rod(g, KRAKEN, (sx * 92, sy * 62 + 8, 56), (sx * 92, sy * 62 - 8, 56), 56, steps=10)
            rod(g, WHITE, (sx * 92, sy * 62 + 10, 56), (sx * 92, sy * 62 - 10, 56), 14, steps=6)
    slab(g, WOOD_D, -134, 134, -52, 52, -FOOT, 92)                 # the chassis and its footing
    slab(g, PEACH, -HX, HX, -HY + 6, HY - 6, 92, 184)
    for k in range(7):
        slab(g, (MAG, WHITE)[k % 2], -HX + k * 40, -HX + 40 + k * 40, -HY + 2, -HY + 10, 100, 176)
    slab(g, WHITE, -HX, HX, -HY, HY, 184, 200)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(WHITE, (sx * 128, sy * 70, 200), 6, 190, steps=6)
    for k in range(8):
        slab(g, (WHITE, MAG)[k % 2], -HX + k * 35, -HX + 35 + k * 35, -HY, -HY + 8, 356, 390)
    slab(g, MAG, -HX, HX, -HY, HY, 390, 402)
    slab(g, WHITE, -110, 110, -60, 60, 402, 414)
    lamps(g, (-HX + 14, -HY - 2, 350), (HX - 14, -HY - 2, 350), 6)
    slab(g, WOOD_D, -30, 30, 44, 70, 200, 262)                     # a rack of cones at the back
    for k in range(3):
        g.cyl(PEACH, (-18 + k * 18, 56, 262), 4, 22, steps=6, top_r=8)
    fig(sc, r, -66, -6, 200, 8, top=WHITE, bottom=NAVY, hat='chef', hat_col=WHITE, band=KRAKEN, arm_l='carry', arm_r='carry',
        hold_r=h_tray(), rock=3, rate=0.5)
    fig(sc, r, 70, -10, 200, -10, top=MAG, hat='cap', hat_col=WHITE, **wave(r, 26))


def sw_window(sc):
    """Two children at a sweet shop's lit window, a parent waving beside them."""
    r = R(104)
    g = sc.still()
    choc_block(g, 200, seed=14)
    slab(g, PEACH, -124, 124, 36, HY - 2, 200, 400)
    slab(g, WOOD_D, -74, 74, 30, 38, 262, 372)
    slab(g, GOLD, -66, 66, 28, 36, 270, 364, glow=0.6)
    slab(g, WOOD_D, -3, 3, 26, 34, 270, 364)
    slab(g, WHITE, -80, 80, 22, 40, 250, 262)                      # the sill, iced
    icing_roof(g, -HX, HX, 16, HY, 400, seed=24)
    cane(g, -128, 28, 200, 190, KRAKEN, 1)
    cane(g, 128, 28, 200, 190, KRAKEN, -1)
    for k in range(4):                                             # jars in the window: tall, pale, square
        slab(g, (MAG, WHITE, KRAKEN, CREAM)[k], -52 + k * 30, -34 + k * 30, 22, 30, 274, 300 + (k % 2) * 12)
    fig(sc, r, -56, -8, 200, 170, arm_r='fwd', swing_r=8, axis_r='x', **kid(r, bob=5, rate=1.1))
    fig(sc, r, 8, -4, 200, 190, arm_l='point', **kid(r, bob=6, rate=1.2))
    fig(sc, r, 76, -40, 200, -12, hold_l=h_cone(WHITE), arm_l='hold', **wave(r, 24))


def sw_baker(sc):
    """A baker waving from his round-topped door, an apprentice sweeping sugar, a child with an ice."""
    r = R(105)
    g = sc.still()
    choc_block(g, 200, seed=15)
    slab(g, PEACH, -78, 78, 40, HY - 2, 200, 420)
    slab(g, GOLD, -36, 36, 34, 42, 200, 352, glow=0.5)
    rod(g, GOLD, (0, 42, 352), (0, 34, 352), 36, steps=10, glow=0.5)
    slab(g, KRAKEN, 36, 46, 2, 42, 200, 360)                       # the door leaf, open
    icing_roof(g, -94, 94, 26, HY, 420, seed=25)
    g.sphere(WHITE, (48, 62, 462), 26, steps=(3, 6))
    g.cyl(PEACH, (48, 62, 470), 22, 70, steps=6, top_r=5)          # a chimney like a cone upside down
    lamps(g, (-70, 30, 408), (70, 30, 408), 5)
    fig(sc, r, 0, 14, 200, 0, top=WHITE, bottom=NAVY, hat='chef', hat_col=WHITE, band=MAG, build='stocky', arm_l='hip', **wave(r, 28))
    fig(sc, r, -92, -20, 200, 30, top=WHITE, bottom=KRAKEN, hat='cap', hat_col=MAG, arm_l='hold', arm_r='hold', hold_r=h_broom(),
        swing_r=10, axis_r='x', rate=0.8)
    fig(sc, r, 104, -36, 200, -10, hold_r=h_cone(MAG), arm_r='face', **kid(r, bob=5, rate=1.0))


def sw_bench(sc):
    """Two guests with cups on a bench of sugar cubes behind a cane rail, a third cheering with a pennant."""
    r = R(106)
    g = sc.still()
    choc_block(g, 205, seed=16)
    for k, x in enumerate((-124, -42, 42, 124)):
        cane(g, x, 74, 205, 110, (MAG, KRAKEN)[k % 2], 1 if x < 0 else -1)
    slab(g, WHITE, -128, 128, 70, 78, 268, 276)
    for k in range(3):
        slab(g, WHITE, -112 + k * 50, -66 + k * 50, 8, 54, 205, 251)
        slab(g, CREAM, -108 + k * 50, -70 + k * 50, 12, 50, 251, 255)
    lamps(g, (-HX + 12, -HY - 1, 196), (HX - 12, -HY - 1, 196), 6)
    fig(sc, r, -88, 30, 205, 6, legs='sit', arm_r='face', hold_r=CUP, swing_r=7, axis_r='x', rate=0.5)
    fig(sc, r, -20, 30, 205, -6, legs='sit', arm_l='hold', hold_l=h_cone(MAG), **wave(r, 20))
    fig(sc, r, 76, -10, 205, -8, hold_l=h_pennant(MAG), **cheer(r))


# ================================================================ CRESCENT KEEP (11.94 to 16.10 km)
def keep_block(g, top, x0=-HX, x1=HX, y0=-HY, y1=HY, merlons=5):
    """A length of castle wall: lavender stone, a cream floor course, merlons along the front, two lit slits."""
    slab(g, LAV, x0 + 4, x1 - 4, y0 + 4, y1 - 4, -FOOT, top - 12)
    slab(g, PLUM, x0 + 4, x1 - 4, y0 + 1, y0 + 8, top * 0.45, top * 0.45 + 8)
    slab(g, CREAM, x0, x1, y0, y1, top - 12, top)
    w = (x1 - x0) / (merlons * 2 - 1.0)
    for k in range(merlons):
        slab(g, CREAM, x0 + k * 2 * w, x0 + k * 2 * w + w, y0, y0 + 10, top, top + 24)
    for sx in (-0.5, 0.5):
        xc = (x0 + x1) / 2.0 + sx * (x1 - x0) * 0.5
        slab(g, CREAM, xc - 12, xc + 12, y0, y0 + 8, top * 0.55, top * 0.55 + 54)
        slab(g, GOLD, xc - 6, xc + 6, y0 - 2, y0 + 6, top * 0.55 + 6, top * 0.55 + 46, glow=0.6)


def hat_post(g, x, y, z, h=120.0, rr=14.0):
    g.cyl(LAV, (x, y, z), rr, h, steps=6)
    g.cyl(CREAM, (x, y, z + h - 8), rr + 4, 8, steps=6)
    g.cyl(PURPLE, (x, y, z + h), rr + 8, 64, steps=6, top_r=3)
    g.box(GOLD, (x, y, z + h + 68), (9, 9, 9))


def moon(g, x, y, z, rr, col=GOLD, th=6.0, glow=0.0):
    """A crescent, its horns to +X, standing in the X-Z plane."""
    for a in (-70, -35, 0, 35, 70):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        g.box(col, (x - ca * rr, y, z + sa * rr), (rr * 0.5, th, rr * 0.62), rot=(-a, 0, 0), glow=glow)


def kp_heralds(sc):
    """Two heralds raise long trumpets hung with banners on a wall top; a drummer beats time below them."""
    r = R(201)
    g = sc.still()
    keep_block(g, 212)
    slab(g, LAV, -HX + 4, HX - 4, 22, HY - 4, 212, 240)
    slab(g, CREAM, -HX, HX, 18, HY, 240, 250)
    hat_post(g, -126, 70, 250, 150)
    hat_post(g, 126, 70, 250, 150)
    flag(sc, r, -126, 70, 468, 60, CYAN, GOLD)
    flag(sc, r, 126, 70, 468, 60, CYAN, GOLD, d=-1)
    for sx in (-1, 1):
        fig(sc, r, sx * 66, 50, 250, sx * 32, top=CYAN, bottom=PURPLE, band=GOLD, sleeves='long', hat='cap', hat_col=PURPLE,
            arm_r='face', hold_r=h_horn(), swing_r=10, axis_r='x', rate=0.4, phase=0.1 + 0.3 * (sx > 0), arm_l='hip')
    g.cyl(CREAM, (0, -52, 212 + 64), 20, 30, steps=8)
    g.cyl(CYAN, (0, -52, 212 + 74), 21, 10, steps=8)
    fig(sc, r, 0, -24, 212, 0, top=PURPLE, bottom=NAVY, band=GOLD, hat='cap', hat_col=CYAN, arm_l='carry', arm_r='carry',
        hold_l=h_stick(WOOD, CREAM, 18), hold_r=h_stick(WOOD, CREAM, 18), swing_l=14, swing_r=-14, axis_l='x', axis_r='x', rate=1.2)


def kp_juggler(sc):
    """A juggler with clubs on a round stone plinth; a child and a parent watch from the wall beside him."""
    r = R(202)
    g = sc.still()
    g.cyl(LAV, (-72, 0, -FOOT), 62, 236 + FOOT, steps=10)
    g.cyl(PLUM, (-72, 0, 110), 64, 9, steps=10)
    g.cyl(CREAM, (-72, 0, 236), 68, 12, steps=10)
    keep_block(g, 200, x0=4, x1=HX, merlons=3)
    hat_post(g, 124, 68, 200, 130)
    flag(sc, r, 124, 68, 398, 56, CYAN, GOLD, d=-1)
    lamps(g, (-128, -52, 226), (-16, -52, 226), 4)
    fig(sc, r, -72, 0, 248, 0, top=MAG, bottom=PURPLE, band=GOLD, hair='pony', arm_l='carry', arm_r='carry', hold_l=h_club(CYAN),
        hold_r=h_club(GOLD, CYAN), swing_l=24, swing_r=-24, axis_l='x', axis_r='x', rate=0.9, build='slim')
    fig(sc, r, 44, -30, 200, 28, **kid(r, arm_l='vee', arm_r='vee', bob=6, rate=1.1))
    fig(sc, r, 96, -8, 200, 30, arm_l='point', arm_r='hip')


def kp_dance(sc):
    """Four dancers turn in a ring on a round dais under a line of banners, a gold crescent in their middle."""
    r = R(203)
    g = sc.still()
    g.cyl(LAV, (0, 0, -FOOT), 80, 192 + FOOT, steps=12)
    g.cyl(PLUM, (0, 0, 96), 82, 9, steps=12)
    g.cyl(CREAM, (0, 0, 192), 84, 13, steps=12)
    for sx in (-1, 1):
        slab(g, LAV, sx * 112 - 22, sx * 112 + 22, 30, 78, -FOOT, 150)
        hat_post(g, sx * 112, 54, 150, 250)
    pennants(sc, r, -104, 104, 54, 440, (CYAN, GOLD, LAV, WHITE), 6)
    g.cyl(CREAM, (0, 0, 205), 15, 44, steps=6)
    moon(g, 4, 0, 284, 26, GOLD, 8, glow=0.5)
    for k, (dx, dy, yw) in enumerate(((-52, -8, 60), (52, -8, -60), (-22, 50, 160), (22, -54, 0))):
        fig(sc, r, dx, dy, 205, yw, wear=('dress', 'skirt', 'trousers', 'skirt')[k], top=(CYAN, LAV, GOLD, MAG)[k], arm_l='out', arm_r='out',
            turn=14, bob=5, rate=0.8, phase=k * 0.25, build='slim')


def kp_painter(sc):
    """A painter at an easel on a balustraded terrace, a model posing with a pennant, a child looking on."""
    r = R(204)
    g = sc.still()
    keep_block(g, 208, merlons=4)
    for k in range(7):
        g.cyl(LAV, (-126 + k * 42, 76, 208), 7, 60, steps=6)
    slab(g, CREAM, -HX, HX, 68, 84, 268, 278)
    for a, b in (((-44, 30, 208), (-26, 22, 380)), ((8, 30, 208), (-10, 22, 380)), ((-18, 62, 208), (-18, 26, 380))):
        tilt_box(g, WOOD, a, b, 6, 6)
    slab(g, CREAM, -54, 18, 14, 20, 286, 384)                      # the canvas, its picture to the road
    slab(g, PURPLE, -48, 12, 11, 17, 330, 378)
    slab(g, LAV, -48, 12, 11, 17, 292, 330)
    moon(g, -10, 9, 356, 12, GOLD, 5)
    fig(sc, r, -100, -6, 208, 62, top=CREAM, bottom=PLUM, hat='beanie', hat_col=PURPLE, sleeves='long', arm_r='fwd', hold_r=h_stick(WOOD, MAG, 26),
        swing_r=9, axis_r='z', rate=0.8, arm_l='hip')
    fig(sc, r, 84, 20, 208, -14, wear='dress', top=LAV, arm_r='hip', arm_l='wave', hold_l=h_pennant(CYAN, GOLD), rock=3, rate=0.4)
    fig(sc, r, 44, -44, 208, 150, arm_r='point', **kid(r, bob=4, rate=0.9))


def kp_bridge(sc):
    """Guests wave from a little arched bridge between two hat-roofed piers, banners hanging from its parapet."""
    r = R(205)
    g = sc.still()
    for sx in (-1, 1):
        slab(g, LAV, sx * 114 - 26, sx * 114 + 26, -HY + 4, HY - 4, -FOOT, 240)
        slab(g, LAV, sx * 70 - 20, sx * 70 + 20, -HY + 8, HY - 8, 150, 240)
        hat_post(g, sx * 124, 66, 256, 110, 13)
        slab(g, GOLD, sx * 114 - 6, sx * 114 + 6, -HY + 2, -HY + 10, 120, 164, glow=0.6)
    slab(g, LAV, -52, 52, -HY + 8, HY - 8, 204, 240)
    slab(g, PLUM, -52, 52, -HY + 5, -HY + 12, 196, 204)
    slab(g, CREAM, -HX, HX, -HY, HY, 240, 256)
    for k in range(5):
        slab(g, CREAM, -HX + k * 62, -HX + 32 + k * 62, -HY, -HY + 10, 256, 282)
    for k, x in enumerate((-60, 60)):
        b = sc.part(P.motion(swing=6, pivot=(x, -HY - 2, 240), axis=0.0, rate=0.5, phase=0.3 * k))
        b.box(CYAN, (x, -HY - 2, 200), (36, 4, 76))
        b.box(GOLD, (x, -HY - 4.5, 206), (16, 4, 16), rot=(45, 0, 0))
    fig(sc, r, -84, 6, 256, 8, **wave(r, 26))
    fig(sc, r, 0, 0, 256, 0, arm_l='hold', arm_r='hold', hold_r=CAM, rock=2, rate=0.4)
    fig(sc, r, 86, 8, 256, -8, **cheer(r))


def kp_stargazer(sc):
    """An astronomer at a brass telescope on castle steps, a child pointing at the sky, a guest with a star chart."""
    r = R(206)
    g = sc.still()
    keep_block(g, 198, merlons=4)
    slab(g, LAV, -HX + 4, HX - 4, 12, HY - 4, 198, 238)
    slab(g, CREAM, -HX, HX, 8, HY, 238, 250)
    for a in ((16, 34), (56, 34), (36, 70)):
        tilt_box(g, WOOD, (a[0], a[1], 250), (36, 48, 362), 6, 6)
    rod(g, GOLD, (-2, 48, 352), (96, 60, 452), 10, steps=8)        # the tube points up and along the road, never at it
    rod(g, CYAN, (96, 60, 452), (104, 61, 460), 12, steps=8)
    hat_post(g, -104, 68, 250, 120)
    vane(sc, r, -104, 68, 442, 'moon')
    fig(sc, r, -40, 48, 250, 78, wear='coat', top=PURPLE, bottom=NAVY, band=GOLD, hat='sun', hat_col=PURPLE, sleeves='long', arm_r='face',
        arm_l='hip', rock=2, rate=0.3)
    fig(sc, r, -92, -38, 198, 10, arm_r='up', swing_r=8, **kid(r, bob=5, rate=1.0))
    fig(sc, r, 74, -38, 198, -8, arm_l='hold', arm_r='hold', hold_r=h_sheet(NAVY, GOLD), rock=3, rate=0.5)


# ================================================================ SNOWGLOBE SQUARE (16.10 to 20.50 km)
def snow_block(g, top, x0=-HX, x1=HX, y0=-HY, y1=HY, seed=1):
    """Blue timber under a thick fall of snow, an ice edge, icicles along the front."""
    r = R(seed)
    slab(g, POOL, x0 + 6, x1 - 6, y0 + 6, y1 - 6, -FOOT, top - 24)
    for k in range(1, 4):
        x = x0 + k * (x1 - x0) / 4.0
        slab(g, NAVY, x - 3, x + 3, y0 + 3, y0 + 9, 20, top - 40)
    slab(g, CYAN, x0 + 6, x1 - 6, y0 + 2, y0 + 9, top - 34, top - 26)
    slab(g, WHITE, x0, x1, y0, y1, top - 24, top)
    for k in range(8):
        x = x0 + (k + 0.5) * (x1 - x0) / 8.0 + r.uniform(-8, 8)
        L = r.uniform(12, 34)
        slab(g, SPRAY, x - 3.5, x + 3.5, y0 + 1, y0 + 8, top - 24 - L, top - 22)


def snowy(r, **kw):
    d = dict(sleeves='long', hat='beanie', hat_col=r.choice((RED, CYAN, MAG, WHITE)), band=r.choice((RED, WHITE, CYAN)))
    d.update(kw)
    return d


def frost_lamp(g, x, y, z, h=190.0):
    g.cyl(TIN, (x, y, z), 6, h, steps=6)
    g.box(TIN_D, (x, y, z + h + 4), (26, 26, 8))
    g.box(CYAN, (x, y, z + h + 20), (18, 18, 24), glow=1.1)
    g.box(WHITE, (x, y, z + h + 38), (30, 30, 12))


def sn_rink(sc):
    """Two skaters glide to and fro on a small rink behind a rail; a child holds on at the front."""
    r = R(301)
    g = sc.still()
    snow_block(g, 198, seed=31)
    slab(g, SPRAY, -132, 132, -64, 74, 198, 205)
    slab(g, CYAN, -132, 132, -70, -64, 198, 207)
    for k in range(5):
        g.box(TIN, (-128 + k * 64, -78, 205 + 34), (6, 6, 68))
    slab(g, TIN, -131, 131, -81, -75, 268, 275)
    frost_lamp(g, -126, 76, 198, 170)
    frost_lamp(g, 126, 76, 198, 170)
    fig(sc, r, 0, -14, 205, 90, arm_l='out', arm_r='out', stride=18, rate=1.0, travel=(236, 105), **snowy(r, build='slim'))
    fig(sc, r, 0, 42, 205, -90, arm_l='back', arm_r='fwd', stride=16, rate=0.9, travel=(236, -85), **snowy(r, wear='skirt'))
    fig(sc, r, 84, -56, 205, 0, arm_l='fwd', arm_r='fwd', **kid(r, bob=3, rate=0.8, **snowy(r)))


def sn_sledge(sc):
    """A parent pulls a child on a sledge along a snow bank; a skier waves from behind a snowy fence."""
    r = R(302)
    g = sc.still()
    snow_block(g, 200, seed=32)
    slab(g, WHITE, -HX + 6, HX - 6, 26, HY - 4, 200, 226)
    for k in range(4):
        g.box(POOL, (-120 + k * 80, 78, 226 + 40), (9, 9, 80))
        g.box(WHITE, (-120 + k * 80, 78, 226 + 86), (15, 15, 12))
    slab(g, POOL, -124, 124, 74, 82, 276, 286)
    ph = r.random()

    def pull():
        fig(sc, r, 50, -22, 200, 90, arm_l='back', arm_r='down', stride=15, rate=1.0, phase=ph, travel=(130, 50), **snowy(r, wear='coat'))
        s = sc.part(P.motion(travel=(130.0, 50.0 / 130.0), base=(0.0, 0.0), rate=1.0, phase=ph))
        s.box(RED, (-30, -22, 222), (74, 34, 7))
        for sy in (-1, 1):
            s.box(TIN, (-30, -22 + sy * 14, 206), (82, 5, 6))
            s.box(TIN, (-6, -22 + sy * 14, 214), (5, 5, 14))
            s.box(TIN, (-54, -22 + sy * 14, 214), (5, 5, 14))
        tilt_box(s, WHITE, (6, -22, 226), (34, -22, 290), 4, 4)
        fig(sc, r, -34, -22, 226 - 46 * 0.66, 90, legs='sit', arm_l='vee', arm_r='vee', rate=1.0, phase=ph, travel=(130, 50),
            **kid(r, s=0.66, **snowy(r)))

    together(sc, (0.0, 0.0), pull)
    fig(sc, r, 76, 52, 226, -12, hold_l=h_ski(CYAN, WHITE, 165), **wave(r, 26, **snowy(r)))


def sn_snowman(sc):
    """Two children finish a snowman in a hat and scarf; a grown-up watches with a mug."""
    r = R(303)
    g = sc.still()
    snow_block(g, 200, seed=33)
    g.sphere(WHITE, (0, 30, 204), 60, scale=(1.3, 0.8, 0.4), steps=(3, 8))
    g.sphere(WHITE, (18, 24, 240), 44, steps=(4, 8))
    g.sphere(WHITE, (18, 24, 302), 32, steps=(4, 8))
    g.sphere(WHITE, (18, 24, 350), 23, steps=(4, 8))
    g.box(RED, (18, 24, 328), (50, 50, 9))
    g.box(RED, (36, 2, 310), (10, 8, 34))
    g.cyl(NAVY, (18, 24, 366), 26, 5, steps=8)
    g.cyl(NAVY, (18, 24, 370), 17, 26, steps=8)
    for z in (292, 308):
        g.box(NAVY, (18, -6, z), (6, 6, 6))
    for sx in (-1, 1):
        g.box(NAVY, (18 + sx * 8, 3, 356), (5, 5, 5))
        tilt_box(g, WOOD_D, (18 + sx * 28, 24, 308), (18 + sx * 66, 20, 334), 5, 5)
    fig(sc, r, -66, -14, 200, 68, arm_l='fwd', arm_r='fwd', swing_r=10, axis_r='x', **kid(r, rate=1.1, **snowy(r)))
    fig(sc, r, 104, -26, 200, -50, **kid(r, **cheer(r, **snowy(r))))
    fig(sc, r, -102, 46, 200, 20, arm_r='face', hold_r=MUG, swing_r=6, axis_r='x', rate=0.4, **snowy(r, wear='coat'))


def sn_mugs(sc):
    """Three with mugs under a snow-laden veranda hung with icicles and a steady blue lantern."""
    r = R(304)
    g = sc.still()
    snow_block(g, 200, seed=34)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(POOL, (sx * 126, sy * 68, 200 + 95), (11, 11, 190))
    slab(g, POOL, -HX, HX, -HY + 4, HY - 4, 390, 402)
    slab(g, WHITE, -HX, HX, -HY, HY, 402, 432)
    slab(g, WHITE, -112, 112, -62, 62, 432, 450)
    rr = R(44)
    for k in range(9):
        x = -HX + (k + 0.5) * 2 * HX / 9.0
        L = rr.uniform(14, 40)
        slab(g, SPRAY, x - 3.5, x + 3.5, -HY + 1, -HY + 8, 402 - L, 404)
    g.box(TIN, (0, -30, 372), (5, 5, 36))
    g.box(CYAN, (0, -30, 344), (20, 20, 24), glow=1.1)
    g.cyl(POOL, (-30, 30, 200), 26, 96, steps=8)                   # a tall round table, a jug on it
    g.cyl(WHITE, (-30, 30, 296), 34, 7, steps=8)
    g.cyl(RED, (-30, 30, 303), 9, 22, steps=6)
    fig(sc, r, -84, 26, 200, 40, arm_r='face', hold_r=MUG, swing_r=7, axis_r='x', rate=0.45, **snowy(r))
    fig(sc, r, 24, 34, 200, -40, arm_l='hold', hold_l=MUG, arm_r='hip', rock=2, rate=0.4, **snowy(r, wear='coat'))
    fig(sc, r, 76, -36, 200, -8, hold_l=MUG, arm_l='hold', **wave(r, 24, **snowy(r)))


def sn_bell(sc):
    """A ringer hauls the rope of a bell in a snow-capped frame; the bell swings; a child covers its ears."""
    r = R(305)
    g = sc.still()
    snow_block(g, 200, seed=35)
    for x in (-44, 64):
        g.box(POOL, (x, 30, 200 + 135), (13, 13, 270))
    slab(g, POOL, -60, 80, 22, 38, 462, 476)
    slab(g, WHITE, -74, 94, 12, 48, 476, 500)
    slab(g, WHITE, -50, 70, 18, 42, 500, 514)
    ph = 0.15
    b = sc.part(P.motion(swing=16, pivot=(10, 30, 462), axis=90.0, rate=0.5, phase=ph))
    b.box(TIN, (10, 30, 448), (7, 7, 30))
    b.cyl(GOLD, (10, 30, 390), 30, 44, steps=8, top_r=14)
    b.box(TIN_D, (10, 30, 384), (9, 9, 14))
    tilt_box(g, WHITE, (-40, 30, 462), (-84, 8, 318), 5, 5)        # the rope
    fig(sc, r, -100, 2, 200, 55, arm_l='up', arm_r='up', swing_l=12, swing_r=12, axis_l='x', axis_r='x', rate=0.5, phase=ph, **snowy(r, build='stocky'))
    fig(sc, r, 110, -32, 200, -30, arm_l='face', arm_r='face', **kid(r, bob=5, rate=1.0, **snowy(r)))
    frost_lamp(g, 124, 70, 200, 150)


def sn_skis(sc):
    """Skiers on ice-block steps: one waving with her skis, a child with a sledge board, one cheering with a pennant."""
    r = R(306)
    g = sc.still()
    slab(g, SPRAY, -HX + 4, HX - 4, -HY + 4, HY - 4, -FOOT, 183)
    for k in range(1, 5):
        slab(g, CYAN, -HX + k * 56 - 3, -HX + k * 56 + 3, -HY + 1, -HY + 8, 20, 180)
    slab(g, CYAN, -HX + 4, HX - 4, -HY + 1, -HY + 8, 96, 102)
    slab(g, WHITE, -HX, HX, -HY, HY, 183, 196)
    slab(g, SPRAY, -HX + 4, HX - 4, 10, HY - 4, 196, 234)
    slab(g, WHITE, -HX, HX, 6, HY, 234, 246)
    for k in range(3):
        tilt_box(g, (RED, CYAN, MAG)[k], (96 + k * 10, 80, 246), (104 + k * 10, 62, 420), 9, 5)
    frost_lamp(g, -126, 70, 246, 150)
    fig(sc, r, -84, -38, 196, 10, hold_l=h_ski(MAG, WHITE, 170), **wave(r, 24, **snowy(r)))
    fig(sc, r, 80, -40, 196, -10, hold_r=h_ski(RED, WHITE, 96), **kid(r, arm_l='vee', bob=6, rate=1.1, **snowy(r)))
    fig(sc, r, -6, 48, 246, 0, hold_l=h_pennant(CYAN, TIN), **cheer(r, **snowy(r, wear='coat')))


# ================================================================ GIANT'S GARDEN (20.50 km to the end)
def trough(g, top, x0=-HX, x1=HX, y0=-HY, y1=HY, band=TEAL):
    """A giant's planter: planks, an enamel band, a cream label with a flower on it, a dark rim of soil."""
    slab(g, WOOD, x0 + 5, x1 - 5, y0 + 5, y1 - 5, -FOOT, top - 12)
    for k in range(1, 5):
        x = x0 + k * (x1 - x0) / 5.0
        slab(g, WOOD_D, x - 3, x + 3, y0 + 2, y0 + 8, 10, top - 12)
    slab(g, band, x0 + 3, x1 - 3, y0 + 1, y0 + 9, top - 64, top - 40)
    xc = (x0 + x1) / 2.0
    slab(g, CREAM, xc - 30, xc + 30, y0 - 1, y0 + 7, top - 150, top - 84)
    slab(g, LEAF, xc - 3, xc + 3, y0 - 3, y0 + 5, top - 144, top - 112)
    g.box(RED, (xc, y0 - 2, top - 102), (22, 6, 22), rot=(45, 0, 0))
    g.box(GOLD, (xc, y0 - 4, top - 102), (9, 6, 9))
    slab(g, WOOD_D, x0, x1, y0, y1, top - 12, top)


def gardener(r, **kw):
    d = dict(top=TEAL, bottom=WOOD_D, hat='sun', hat_col=GOLD, band=CREAM)
    d.update(kw)
    return d


def toadstool(g, x, y, z, h, rr, cap=RED):
    g.cyl(CREAM, (x, y, z), rr * 0.34, h, steps=6, top_r=rr * 0.26)
    g.sphere(cap, (x, y, z + h + rr * 0.1), rr, scale=(1, 1, 0.5), steps=(4, 8))
    for a in (20, 140, 260):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        g.box(GOLD, (x + ca * rr * 0.55, y + sa * rr * 0.55, z + h + rr * 0.44), (rr * 0.24, rr * 0.24, 6))
    return z + h + rr * 0.58


def gd_ladder(sc):
    """A gardener up a ladder waters a nodding giant sunflower; one steadies the ladder, a child points."""
    r = R(401)
    g = sc.still()
    trough(g, 195)
    f = sc.part(P.motion(swing=2, pivot=(66, 40, 195), axis=0.0, rate=0.3, phase=0.2))
    f.cyl(LEAF, (66, 40, 195), 9, 300, steps=6)
    tilt_box(f, LEAF, (66, 40, 300), (112, 30, 330), 30, 6)
    tilt_box(f, LEAF, (66, 40, 380), (26, 52, 404), 26, 6)
    rod(f, GOLD, (66, 34, 510), (66, 22, 510), 50, steps=10)
    rod(f, WOOD_D, (66, 26, 510), (66, 16, 510), 26, steps=8)
    for sy in (-1, 1):
        tilt_box(g, WOOD, (-70, 40 + sy * 17, 195), (50, 40 + sy * 17, 470), 6, 6)
    for k in range(6):
        t = (k + 0.7) / 6.6
        g.box(WOOD, (-70 + 120 * t, 40, 195 + 275 * t), (6, 38, 6))
    fig(sc, r, -18, 30, 195 + 126, 60, arm_r='fwd', hold_r=h_can(), swing_r=10, axis_r='x', rate=0.5, arm_l='hold', **gardener(r))
    fig(sc, r, -106, 26, 195, 80, arm_l='fwd', arm_r='fwd', **gardener(r, hat='cap', hat_col=RED, build='stocky'))
    fig(sc, r, 104, -42, 195, -20, arm_r='up', swing_r=8, **kid(r, bob=5, rate=1.0))


def gd_teacup(sc):
    """A tea party in a giant's teacup: three guests up to their waists in it, cups in hand; toadstools at its foot."""
    r = R(402)
    g = sc.still()
    trough(g, 96)
    g.cyl(GOLD, (0, 0, 96), 84, 10, steps=12)
    g.cyl(CREAM, (0, 0, 106), 50, 112, steps=12, top_r=84)
    g.cyl(TEAL, (0, 0, 178), 78, 14, steps=12)
    g.cyl(GOLD, (0, 0, 212), 86, 9, steps=12)
    g.cyl(WOOD, (0, 0, 218), 79, 6, steps=12)                      # the tea
    for a, b in (((80, 0, 196), (118, 0, 186)), ((118, 0, 186), (124, 0, 150)), ((124, 0, 150), (96, 0, 124)), ((96, 0, 124), (62, 0, 128))):
        tilt_box(g, TEAL, a, b, 13, 13)
    toadstool(g, -118, -56, 96, 44, 26)
    toadstool(g, 112, 58, 96, 60, 26)
    toadstool(g, -122, 52, 96, 34, 22)
    fig(sc, r, -34, -14, 150, 14, arm_r='face', hold_r=CUP, swing_r=7, axis_r='x', rate=0.5, hat='sun', hat_col=MAG)
    fig(sc, r, 36, -12, 150, -14, hold_l=CUP, arm_l='hold', **wave(r, 24))
    fig(sc, r, 2, 36, 150, 0, hold_r=CUP, arm_r='up', arm_l='vee', bob=4, rate=1.0, hat='sun', hat_col=GOLD)


def gd_toadstools(sc):
    """Children on toadstool seats: one waving, one cheering, one hopping on the big cap; a parent below."""
    r = R(403)
    g = sc.still()
    trough(g, 150, band=RED)
    t1 = toadstool(g, -92, 14, 150, 62, 46)
    t2 = toadstool(g, 6, 30, 150, 118, 60)
    t3 = toadstool(g, 100, 10, 150, 80, 46)
    s1, s3 = 0.66, 0.68
    fig(sc, r, -92, 14, t1 - 46 * s1, 10, legs='sit', **kid(r, s=s1, **wave(r, 24)))
    fig(sc, r, 6, 30, t2 - 4, 0, **kid(r, **cheer(r)))
    fig(sc, r, 100, 10, t3 - 46 * s3, -10, legs='sit', arm_l='vee', arm_r='vee', **kid(r, s=s3, bob=3, rate=0.9))
    fig(sc, r, 46, -52, 150, -6, arm_l='hip', **wave(r, 20, **gardener(r)))
    toadstool(g, -128, -58, 150, 26, 20)


def gd_hose(sc):
    """Someone waters a giant bell flower in its pot with a hose (the stream runs along the road); a child hops."""
    r = R(404)
    g = sc.still()
    trough(g, 195)
    rod(g, TEAL, (-112, 40, 240), (-112, 18, 240), 34, steps=10)   # the hose reel
    g.box(WOOD_D, (-112, 29, 218), (12, 30, 46))
    tilt_box(g, TEAL, (-112, 6, 200), (-64, -2, 200), 8, 8)
    tilt_box(g, TEAL, (-64, -2, 200), (-40, 2, 300), 8, 8)
    g.cyl(WOOD, (104, 20, 195), 30, 62, steps=10, top_r=38)
    g.cyl(WOOD_D, (104, 20, 257), 40, 8, steps=10)
    b = sc.part(P.motion(swing=4, pivot=(104, 20, 265), axis=0.0, rate=0.35, phase=0.4))
    b.cyl(LEAF, (104, 20, 265), 7, 150, steps=6)
    tilt_box(b, LEAF, (104, 20, 330), (70, 30, 352), 24, 6)
    b.cyl(LAV, (104, 20, 404), 28, 40, steps=8, top_r=12)
    b.cyl(PURPLE, (104, 20, 396), 30, 8, steps=8)
    w = sc.part(P.motion(swing=5, pivot=(-2, 0, 322), axis=90.0, rate=0.6, phase=0.1))
    tilt_box(w, SPRAY, (-2, 0, 322), (36, 4, 352), 9, 9)
    tilt_box(w, SPRAY, (36, 4, 352), (72, 10, 350), 11, 11)
    tilt_box(w, SPRAY, (72, 10, 350), (100, 16, 322), 13, 13)
    fig(sc, r, -44, 0, 195, 82, arm_r='fwd', arm_l='hip', hold_r=h_stick(TEAL, GOLD, 10), rock=2, rate=0.6, **gardener(r))
    fig(sc, r, 30, -48, 195, 20, **kid(r, **cheer(r)))


def gd_beans(sc):
    """A gardener on a stool ties bean poles, another brings a basket; a scarecrow in a straw hat sways."""
    r = R(405)
    g = sc.still()
    trough(g, 195, band=GOLD)
    for a in (0, 120, 240):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        rod(g, WOOD, (62 + ca * 44, 30 + sa * 34, 195), (62, 30, 500), 5, steps=6)
        for k in range(4):
            t = 0.15 + k * 0.2
            g.box((LEAF, LEAF_D)[k % 2], (62 + ca * 44 * (1 - t), 30 + sa * 34 * (1 - t), 195 + 305 * t), (26, 22, 18))
            if k % 2:
                g.box(RED, (62 + ca * 44 * (1 - t), 26 + sa * 34 * (1 - t), 207 + 305 * t), (9, 9, 9))
    g.box(GOLD, (62, 30, 496), (14, 14, 10))
    slab(g, WOOD, -22, 22, 4, 44, 195, 243)                        # a wooden stool
    fig(sc, r, 0, 24, 243, 70, arm_l='up', arm_r='up', swing_r=7, axis_r='x', rate=0.6, **gardener(r))

    def basket(m, h, side):
        if side > 0:
            m.box(CREAM, (h[0] - 24, h[1] - 8, h[2] + 4), (44, 28, 20))
            m.box(LEAF, (h[0] - 24, h[1] - 8, h[2] + 17), (36, 20, 8))
    fig(sc, r, 112, -40, 195, -16, arm_l='carry', arm_r='carry', hold_r=basket, rock=2, rate=0.5, **gardener(r, hat='cap', hat_col=TEAL))
    s = sc.part(P.motion(swing=3, pivot=(-80, 40, 195), axis=0.0, rate=0.35, phase=0.6))   # the scarecrow
    s.box(WOOD_D, (-80, 40, 195 + 95), (8, 8, 190))
    s.box(WOOD_D, (-80, 40, 350), (104, 7, 7))
    s.box(LAV, (-80, 40, 330), (50, 22, 72))
    s.box(MAG, (-66, 28, 316), (16, 5, 16))
    s.box(CREAM, (-80, 40, 392), (36, 32, 40))
    s.box(RED, (-80, 40, 370), (40, 34, 9))
    s.cyl(GOLD, (-80, 40, 408), 44, 5, steps=8)
    s.cyl(GOLD, (-80, 40, 412), 22, 22, steps=8, top_r=16)
    for sx in (-1, 1):
        s.box(GOLD, (-80 + sx * 54, 40, 344), (12, 9, 20))


def gd_thimbles(sc):
    """A fiddler and a dancer on two giant thimbles, a child on a giant cotton reel between them."""
    r = R(406)
    g = sc.still()
    trough(g, 110, band=RED)
    for x, top in ((-82, 222), (86, 196)):
        g.cyl(TIN, (x, 6, 110), 54, top - 110, steps=10, top_r=46)
        g.cyl(TIN_D, (x, 6, 110), 57, 14, steps=10)
        for k in range(3):
            g.cyl(TIN_D, (x, 6, 140 + k * 22), 54.5 - k * 1.6, 5, steps=10)
        g.cyl(TIN_D, (x, 6, top), 44, 5, steps=10)
    fig(sc, r, -82, 6, 227, 10, arm_l='hold', arm_r='fwd', hold_l=h_fiddle(), hold_r=h_bow(), swing_r=15, axis_r='z', rate=1.2,
        top=RED, bottom=WOOD_D, hat='sun', hat_col=GOLD)
    fig(sc, r, 86, 6, 201, -10, wear='skirt', top=GOLD, bottom=RED, arm_l='out', arm_r='vee', turn=16, bob=5, rate=0.9, build='slim')
    g.cyl(WOOD, (2, -28, 110), 30, 8, steps=10)                    # the reel
    g.cyl(RED, (2, -28, 118), 22, 50, steps=10)
    g.cyl(WOOD, (2, -28, 168), 30, 8, steps=10)
    s = 0.66
    fig(sc, r, 2, -22, 176 - 46 * s, 0, legs='sit', arm_l='vee', arm_r='vee', **kid(r, s=s, bob=3, rate=0.9))


# ================================================================ THE GAPS BEFORE THE LAST RIDE (5 x 3 m)
GX, GY = 250.0, 150.0


def stripes(g, x0, x1, y, z0, z1, cols, n, depth=8.0):
    w = (x1 - x0) / float(n)
    for k in range(n):
        slab(g, cols[k % len(cols)], x0 + k * w, x0 + (k + 1) * w, y - depth / 2.0, y + depth / 2.0, z0, z1)


def jump_terrace(sc):
    """The big jump's valley side: three steps of guests cheering, filming and waving flags under pennant poles."""
    r = R(501)
    g = sc.still()
    tops = (200, 250, 300)
    for k, top in enumerate(tops):
        y0, y1 = -GY + k * 100, -GY + (k + 1) * 100
        slab(g, PLUM, -GX + 4, GX - 4, y0 + 4, GY - 4 if k == 2 else y1 + 4, -FOOT if k == 0 else tops[k - 1] - 10, top - 10)
        slab(g, AMBER, -GX, GX, y0, y1 if k < 2 else GY, top - 10, top)
        if k:
            lamps(g, (-GX + 16, y0 - 2, top - 26), (GX - 16, y0 - 2, top - 26), 6)
    stripes(g, -GX + 6, GX - 6, -GY + 3, 40, 186, (RED, CREAM), 10)
    slab(g, NAVY, -GX + 4, GX - 4, -GY + 1, -GY + 8, 20, 40)
    for sx in (-1, 1):
        g.cyl(WHITE, (sx * 238, 138, 300), 6, 260, steps=6)
        g.box(AMBER, (sx * 238, 138, 566), (12, 12, 12), glow=1.2)
    pennants(sc, r, -232, 232, 138, 540, (MAG, AMBER, CYAN, RED, WHITE), 8)
    acts = [lambda: cheer(r), lambda: wave(r, 26), lambda: dict(arm_l='hold', arm_r='hold', hold_r=CAM, rock=2, rate=0.4),
            lambda: cheer(r, hold_l=h_pennant(r.choice((MAG, CYAN, RED)))), lambda: dict(arm_r='point', arm_l='hip')]
    spots = [(-180, -100, 0, 1), (-60, -104, 0, 0), (60, -100, 0, 3), (180, -104, 0, 1),
             (-150, 0, 1, 2), (0, 0, 1, 4), (150, 0, 1, 3), (-90, 100, 2, 3), (90, 100, 2, 0)]
    for i, (x, y, k, a) in enumerate(spots):
        d = acts[a]()
        if i in (1, 2, 5):
            d = kid(r, **d)
        fig(sc, r, x, y, tops[k], r.uniform(-12, 12), detail=1, **d)


def jump_steps(sc):
    """A photographers' riser under a striped awning: three with cameras, a family pointing at the jump, a star vane."""
    r = R(502)
    g = sc.still()
    slab(g, PLUM, -GX + 4, GX - 4, -GY + 4, GY - 4, -FOOT, 190)
    slab(g, CREAM, -GX, GX, -GY, GY, 190, 200)
    stripes(g, -GX + 6, GX - 6, -GY + 3, 40, 180, (MAG, CREAM), 10)
    slab(g, NAVY, -GX + 4, GX - 4, -GY + 1, -GY + 8, 20, 40)
    slab(g, PLUM, -GX + 4, GX - 4, 4, GY - 4, 200, 252)
    slab(g, CREAM, -GX, GX, 0, GY, 252, 262)
    lamps(g, (-GX + 16, -2, 236), (GX - 16, -2, 236), 7)
    for sx in (-1, 1):
        for y in (14, 136):
            g.cyl(WHITE, (sx * 236, y, 262), 6, 230, steps=6)
    stripes(g, -GX, GX, 2, 470, 500, (RED, CREAM), 12)
    slab(g, RED, -GX, GX, -2, GY, 500, 512)
    slab(g, CREAM, -GX + 30, GX - 30, 30, GY - 30, 512, 526)
    vane(sc, r, 0, 76, 526, 'star')
    for k in range(4):                                             # planters along the front edge
        x = -186 + k * 124
        slab(g, WOOD, x - 22, x + 22, -GY + 6, -GY + 34, 200, 232)
        g.sphere(LEAF, (x, -GY + 20, 244), 22, scale=(1, 0.7, 0.8), steps=(3, 6))
    for k, x in enumerate((-150, 30)):
        fig(sc, r, x, 70, 262, r.uniform(-10, 10), detail=1, arm_l='hold', arm_r='hold', hold_r=CAM, rock=2, rate=0.4, hat=('cap', 'sun')[k], hat_col=MAG)
    fig(sc, r, 186, 70, 262, -8, detail=1, **cheer(r))
    fig(sc, r, -120, -70, 200, 8, detail=1, arm_r='point', arm_l='hip')
    fig(sc, r, -40, -76, 200, 0, detail=1, **kid(r, **cheer(r)))
    fig(sc, r, 44, -70, 200, -6, detail=1, **wave(r, 26))
    fig(sc, r, 150, -74, 200, -12, detail=1, **kid(r, hold_l=h_pennant(CYAN), **cheer(r)))


def stilt_deck(g, r, top=236.0, lean=14.0):
    """A crooked plank deck on leaning piles, a rope rail along its back."""
    for x in (-226, -76, 76, 226):
        for y in (-120, 120):
            l = r.uniform(-lean, lean)
            rod(g, WOOD_D, (x + l, y, -FOOT), (x - l * 0.4, y, top - 10), 13, steps=6)
    for x in (-150, 0, 150):
        tilt_box(g, WOOD_D, (x - 70, -124, 60), (x + 70, -124, 170), 10, 8)
    slab(g, WOOD, -GX, GX, -GY, GY, top - 16, top)
    for k in range(1, 6):
        slab(g, WOOD_D, -GX + k * 83 - 3, -GX + k * 83 + 3, -GY - 1, -GY + 5, top - 16, top + 1)
    for k in range(5):
        g.cyl(WOOD_D, (-240 + k * 120, GY - 10, top), 8, 96, steps=6)
    slab(g, CREAM, -240, 240, GY - 13, GY - 7, top + 78, top + 84)
    return top


def cove_deck_a(sc):
    """Pirate Cove: net menders on a stilt deck, a net swaying on its rack, one hauling a line, a lantern, a red flag."""
    r = R(601)
    g = sc.still()
    T = stilt_deck(g, r)
    for x in (-150, 60):
        g.cyl(WOOD_D, (x, 96, T), 8, 250, steps=6)
    slab(g, WOOD_D, -160, 70, 92, 100, T + 244, T + 254)
    n = sc.part(P.motion(swing=3, pivot=(-45, 96, T + 248), axis=0.0, rate=0.3, phase=0.2))
    for k in range(6):
        n.box(TEAL, (-140 + k * 38, 94, T + 150), (4, 4, 190))
    for k in range(4):
        n.box(TEAL, (-45, 94, T + 80 + k * 46), (196, 4, 4))
    for k in range(5):
        n.box((CREAM, RED)[k % 2], (-140 + k * 46, 90, T + 244), (14, 10, 14))
    for x in (-110, -20):
        g.cyl(WOOD_D, (x, 30, T), 22, 46, steps=8)                 # low kegs to sit on
    fig(sc, r, -110, 30, T, 14, detail=1, legs='sit', arm_l='hold', arm_r='hold', swing_r=8, axis_r='x', rate=1.0, hat='cap', hat_col=RED, top=CREAM, bottom=TEAL)
    fig(sc, r, -20, 30, T, -12, detail=1, legs='sit', arm_l='hold', arm_r='hold', swing_l=8, axis_l='x', rate=0.9, top=TEAL, bottom=WOOD_D, hair='long')
    tilt_box(g, CREAM, (-70, 20, T + 60), (-64, 60, T + 120), 60, 5)   # the net across their knees
    fig(sc, r, 150, -40, T, 60, detail=1, arm_l='up', arm_r='up', swing_l=12, swing_r=12, axis_l='x', axis_r='x', rate=0.6, top=RED, bottom=NAVY, build='stocky')
    tilt_box(g, CREAM, (176, -24, T + 180), (226, 60, T + 400), 5, 5)
    g.cyl(WOOD_D, (226, 60, T), 9, 430, steps=6)                   # a mast with a yard and a flag
    slab(g, WOOD_D, 160, 250, 56, 64, T + 396, T + 404)
    flag(sc, r, 226, 60, T + 430, 40, RED, WOOD_D)
    fig(sc, r, 40, -90, T, 0, detail=1, **wave(r, 26, top=CREAM, bottom=RED))
    fig(sc, r, -200, -80, T, 10, detail=1, **kid(r, **cheer(r)))
    g.box(WOOD_D, (-236, -130, T + 70), (7, 7, 140))
    g.box(GOLD, (-236, -130, T + 152), (20, 20, 24), glow=1.0)
    g.box(WOOD_D, (-236, -130, T + 168), (26, 26, 8))


def cove_deck_b(sc):
    """Pirate Cove: a look-out with a brass glass on a cupola, a map read by lantern, a barrel rolled along the deck."""
    r = R(602)
    g = sc.still()
    T = stilt_deck(g, r, lean=18.0)
    slab(g, WOOD, 130, 240, 30, 130, T, T + 150)                   # the cupola: a planked box, a rail, a shingle cap on posts
    for k in range(3):
        slab(g, TEAL, 150 + k * 30, 164 + k * 30, 26, 32, T + 60, T + 110)
    slab(g, CREAM, 126, 244, 26, 134, T + 150, T + 160)
    for x, y in ((132, 32), (238, 32), (132, 128), (238, 128)):
        g.cyl(WOOD_D, (x, y, T + 160), 6, 200, steps=6)
    g.cyl(TEAL, (185, 80, T + 360), 92, 60, steps=4, top_r=14)
    vane(sc, r, 185, 80, T + 420, 'fish')
    tilt_box(g, GOLD, (196, 40, T + 300), (246, 44, T + 322), 9, 9)   # the glass points along the road, never at it
    fig(sc, r, 176, 60, T + 160, 80, detail=1, arm_r='face', arm_l='hip', rock=2, rate=0.3, top=RED, bottom=NAVY, hat='cap', hat_col=NAVY, band=GOLD)
    g.cyl(WOOD_D, (-150, 60, T), 30, 80, steps=8)                  # a barrel for a table
    g.box(GOLD, (-150, 60, T + 94), (18, 18, 26), glow=1.0)
    fig(sc, r, -200, 40, T, 30, detail=1, arm_l='hold', arm_r='hold', hold_r=h_sheet(CREAM, GOLD, 70, 46), rock=2, rate=0.4, top=TEAL, bottom=WOOD_D, hat='sun', hat_col=CREAM)
    fig(sc, r, -100, 30, T, -30, detail=1, arm_r='point', **kid(r, bob=4, rate=0.9))
    ph = r.random()

    def roll():
        fig(sc, r, -40, -92, T, 90, detail=1, arm_l='push', arm_r='push', stride=16, rate=1.0, phase=ph, travel=(300, 60), top=CREAM, bottom=RED, build='stocky')
        b = sc.part(P.motion(travel=(300.0, 0.2), base=(0.0, 0.0), rate=1.0, phase=ph))
        rod(b, WOOD_D, (30, -58, T + 32), (30, -126, T + 32), 32, steps=8)
        rod(b, TIN_D, (30, -70, T + 32), (30, -78, T + 32), 33.5, steps=8)
        rod(b, TIN_D, (30, -106, T + 32), (30, -114, T + 32), 33.5, steps=8)

    together(sc, (0.0, 0.0), roll)
    flag(sc, r, -240, 130, T + 96, 150, RED, WOOD_D)


def back_lot_dock(sc):
    """The back lot: a stage hand pushes a painted flat along a loading dock, a manager waves him on, a painter
    touches up a tin star, a work lamp burns."""
    r = R(701)
    g = sc.still()
    slab(g, TIN_D, -GX + 4, GX - 4, -GY + 4, GY - 4, -FOOT, 188)
    slab(g, TIN, -GX, GX, -GY, GY, 188, 200)
    slab(g, NAVY, -GX + 4, GX - 4, -GY + 1, -GY + 8, 150, 180)
    stripes(g, -GX + 6, GX - 6, -GY + 2, 110, 150, (AMBER, NAVY), 14, 8)
    for k in range(6):                                             # a plain plank fence along the back
        slab(g, WOOD, -GX + 6 + k * 82, -GX + 80 + k * 82, GY - 12, GY - 4, 200, 380 - (k % 2) * 14)
    g.box(GOLD, (-170, 118, 300), (110, 8, 110), rot=(45, 0, 0))   # a tin star leaning on the fence
    g.box(GOLD, (-170, 116, 300), (110, 8, 110))
    fig(sc, r, -96, 96, 200, 150, detail=1, arm_r='fwd', hold_r=h_stick(WOOD, AMBER, 26), swing_r=9, axis_r='x', rate=0.8, top=CREAM, bottom=NAVY, hat='cap', hat_col=MAG)
    ph = r.random()

    def push():
        fig(sc, r, -128, 20, 200, 90, detail=1, arm_l='push', arm_r='push', stride=15, rate=1.0, phase=ph, travel=(232, 50), top=NAVY, bottom=NAVY, band=AMBER, build='stocky')
        f = sc.part(P.motion(travel=(232.0, 50.0 / 232.0), base=(0.0, 0.0), rate=1.0, phase=ph))
        slab(f, CREAM, -84, 100, 12, 20, 216, 440)
        slab(f, LAV, -84, 100, 9, 17, 216, 300)
        slab(f, PURPLE, -84, 100, 9, 17, 300, 330)
        f.box(AMBER, (40, 8, 386), (50, 6, 50), rot=(45, 0, 0))
        f.box(AMBER, (40, 6, 386), (50, 6, 50))
        tilt_box(f, WOOD_D, (-60, 22, 216), (-60, 60, 380), 8, 8)
        tilt_box(f, WOOD_D, (76, 22, 216), (76, 60, 380), 8, 8)
        slab(f, WOOD_D, -90, 106, 10, 64, 206, 216)

    together(sc, (0.0, 0.0), push)
    fig(sc, r, 186, -96, 200, -30, detail=1, arm_r='hold', hold_r=h_sheet(WHITE, NAVY, 26, 34), arm_l='wave', swing_l=24, rate=1.0, top=MAG, bottom=NAVY, hat='cap', hat_col=NAVY)
    fig(sc, r, -60, -100, 200, 6, detail=1, **wave(r, 20, top=NAVY, bottom=NAVY, band=AMBER, hat='cap', hat_col=AMBER))
    g.cyl(TIN_D, (226, 100, 200), 6, 210, steps=6)                 # the work lamp
    g.box(TIN_D, (226, 94, 418), (40, 16, 30))
    g.box(LAMP, (226, 86, 418), (32, 6, 22), glow=1.3)


# ================================================================ THE LAGOON (the deck at the rails, 23.5 m up)
JX, JY, JZ = 400.0, 165.0, 2350.0


def jetty(g):
    for x in (-340, -114, 114, 340):
        for y in (-125, 125):
            g.cyl(PLUM, (x, y, -600), 20, JZ + 590, steps=6)
    for k, z in enumerate((500, 1250, 2000)):
        for b in range(3):
            x0, x1 = -340 + b * 227, -113 + b * 227
            a, c = ((x0, -125, z - 240), (x1, -125, z + 240)) if (b + k) % 2 else ((x1, -125, z - 240), (x0, -125, z + 240))
            tilt_box(g, POOL, a, c, 12, 12)
        for sx in (-1, 1):
            tilt_box(g, POOL, (sx * 340, -125, z - 200), (sx * 340, 125, z + 200), 12, 12)
    slab(g, POOL, -JX + 6, JX - 6, -JY + 6, JY - 6, JZ - 36, JZ - 12)
    slab(g, WHITE, -JX, JX, -JY, JY, JZ - 12, JZ + 8)
    lamps(g, (-JX + 20, -JY - 2, JZ - 24), (JX - 20, -JY - 2, JZ - 24), 8, 13, CYAN, 1.2)
    for k in range(9):
        g.box(WHITE, (-JX + 8 + k * 98, JY - 8, JZ + 8 + 46), (8, 8, 92))
    slab(g, CYAN, -JX + 4, JX - 4, JY - 12, JY - 4, JZ + 92, JZ + 100)
    for sx in (-1, 1):
        slab(g, WHITE, sx * (JX - 8) - 4, sx * (JX - 8) + 4, -JY + 20, JY - 4, JZ + 92, JZ + 100)
        g.box(WHITE, (sx * (JX - 8), -JY + 24, JZ + 54), (8, 8, 92))
    return JZ + 8


def lagoon_jetty_a(sc):
    """The lagoon: anglers on a jetty at the rails' height, a pagoda lamp, a child with its catch, oars in a rack."""
    r = R(801)
    g = sc.still()
    T = jetty(g)
    for x in (-90, 90):
        for y in (20, 130):
            g.cyl(WHITE, (x, y, T), 7, 230, steps=6)
    g.cyl(LAV, (0, 75, T + 230), 124, 80, steps=4, top_r=40, rot=(0, 45, 0))
    g.cyl(LAV, (0, 75, T + 310), 60, 40, steps=4, top_r=8, rot=(0, 45, 0))
    g.box(CYAN, (0, 75, T + 200), (30, 30, 36), glow=1.2)
    vane(sc, r, 0, 75, T + 350, 'fish', CYAN)
    slab(g, POOL, -70, 70, 96, 128, T, T + 46)                     # a bench under the roof
    fig(sc, r, -34, 110, T, 6, detail=1, legs='sit', arm_r='face', hold_r=CUP, swing_r=6, axis_r='x', rate=0.4)
    fig(sc, r, 34, 110, T, -6, detail=1, legs='sit', **wave(r, 22))
    fig(sc, r, -320, -60, T, -62, detail=1, arm_l='hold', arm_r='fwd', hold_r=h_rod(WOOD, 220), swing_r=6, axis_r='x', rate=0.3, hat='sun', hat_col=WHITE, top=CYAN)
    fig(sc, r, 320, -60, T, 62, detail=1, arm_l='fwd', arm_r='hold', hold_l=h_rod(WOOD, 220), swing_l=6, axis_l='x', rate=0.35, hat='cap', hat_col=LAV, top=WHITE)
    fig(sc, r, -170, -100, T, 8, detail=1, hold_r=h_fish(), arm_r='up', **kid(r, arm_l='vee', bob=6, rate=1.1))
    fig(sc, r, 150, -104, T, -6, detail=1, **cheer(r))
    for k in range(4):                                             # oars leaning on the back rail
        tilt_box(g, WOOD, (220 + k * 34, 130, T), (228 + k * 34, 150, T + 220), 6, 6)
        g.box(CYAN, (229 + k * 34, 152, T + 236), (20, 6, 50))
    for x in (-250, 250):                                          # mooring posts with a coil of rope
        g.cyl(PLUM, (x, -JY + 26, T), 12, 60, steps=6)
        g.cyl(CREAM, (x, -JY + 26, T + 20), 16, 14, steps=6)


def lagoon_jetty_b(sc):
    """The lagoon: tea under a jellyfish dome of ribs and swaying streamers, a bather bouncing on a springboard."""
    r = R(802)
    g = sc.still()
    T = jetty(g)
    cx, cy, rr = -170.0, 10.0, 140.0
    for k in range(6):                                             # the bell: six ribs to a cap
        a = math.radians(k * 60 + 30)
        ca, sa = math.cos(a), math.sin(a)
        p = [(cx + ca * rr * f, cy + sa * rr * f, T + h) for f, h in ((1.0, 0), (0.96, 120), (0.7, 230), (0.2, 286))]
        for i in range(3):
            tilt_box(g, LAV, p[i], p[i + 1], 9, 9)
    g.cyl(CYAN, (cx, cy, T + 280), 46, 14, steps=8, glow=0.6)
    g.sphere(LAV, (cx, cy, T + 296), 30, scale=(1, 1, 0.6), steps=(3, 8))
    for k in range(6):                                             # streamers under the rim, swaying
        a = math.radians(k * 60)
        x, y = cx + math.cos(a) * rr * 0.86, cy + math.sin(a) * rr * 0.86
        s = sc.part(P.motion(swing=8, pivot=(x, y, T + 228), axis=k * 60.0 + 90.0, rate=r.uniform(0.3, 0.5), phase=r.random()))
        s.box((CYAN, WHITE)[k % 2], (x, y, T + 196), (8, 8, 64))
    g.cyl(WHITE, (cx, cy, T), 10, 70, steps=6)
    g.cyl(WHITE, (cx, cy, T + 70), 44, 7, steps=8)
    g.cyl(TEAL, (cx, cy, T + 77), 13, 18, steps=6, top_r=9)        # the pot
    tilt_box(g, TEAL, (cx + 10, cy, T + 86), (cx + 26, cy, T + 98), 5, 5)
    for k, a in enumerate((200, 340)):
        x, y = cx + math.cos(math.radians(a)) * 84, cy + math.sin(math.radians(a)) * 84
        g.cyl(POOL, (x, y, T), 18, 46, steps=6)
        fig(sc, r, x, y, T, (60, -60)[k], detail=1, legs='sit', arm_r='face', hold_r=CUP, swing_r=6, axis_r='x', rate=0.4 + 0.1 * k,
            hat=('sun', None)[k], hat_col=WHITE)
    fig(sc, r, -40, -110, T, 0, detail=1, top=WHITE, bottom=NAVY, band=CYAN, arm_l='carry', arm_r='carry', hold_r=h_tray(WHITE, CYAN, WHITE), rock=2, rate=0.5)
    slab(g, POOL, 180, 262, -40, 60, T, T + 46)                    # the springboard's stand: two steps
    slab(g, POOL, 222, 262, -40, 60, T + 46, T + 92)
    slab(g, WHITE, 222, 392, -14, 34, T + 92, T + 100)
    fig(sc, r, 366, 10, T + 100, 0, detail=1, wear='shorts', sleeves='none', top=POOL, bottom=POOL, arm_l='up', arm_r='up', bob=9, rate=0.9)
    fig(sc, r, 200, 10, T + 46, -10, detail=1, **kid(r, wear='shorts', band=WHITE, **cheer(r)))


ROW = dict(grow=(8, 6, 30), grow_down=(8, 6, 0))                   # true bounds: the lane is 2.2 m deep
GAP = dict(grow=(15, 8, 40), grow_down=(15, 8, 0))
JOBS = [
    ('SM_S1PEOPLE_Sw_Ices', lit(sw_ices), ROW), ('SM_S1PEOPLE_Sw_Toffee', lit(sw_toffee), ROW), ('SM_S1PEOPLE_Sw_Wagon', lit(sw_wagon), ROW),
    ('SM_S1PEOPLE_Sw_Window', lit(sw_window), ROW), ('SM_S1PEOPLE_Sw_Baker', lit(sw_baker), ROW), ('SM_S1PEOPLE_Sw_Bench', lit(sw_bench), ROW),
    ('SM_S1PEOPLE_Kp_Heralds', lit(kp_heralds), ROW), ('SM_S1PEOPLE_Kp_Juggler', lit(kp_juggler), ROW), ('SM_S1PEOPLE_Kp_Dance', lit(kp_dance), ROW),
    ('SM_S1PEOPLE_Kp_Painter', lit(kp_painter), ROW), ('SM_S1PEOPLE_Kp_Bridge', lit(kp_bridge), ROW), ('SM_S1PEOPLE_Kp_Stargazer', lit(kp_stargazer), ROW),
    ('SM_S1PEOPLE_Sn_Rink', lit(sn_rink), ROW), ('SM_S1PEOPLE_Sn_Sledge', lit(sn_sledge), ROW), ('SM_S1PEOPLE_Sn_Snowman', lit(sn_snowman), ROW),
    ('SM_S1PEOPLE_Sn_Mugs', lit(sn_mugs), ROW), ('SM_S1PEOPLE_Sn_Bell', lit(sn_bell), ROW), ('SM_S1PEOPLE_Sn_Skis', lit(sn_skis), ROW),
    ('SM_S1PEOPLE_Gd_Ladder', lit(gd_ladder), ROW), ('SM_S1PEOPLE_Gd_TeaCup', lit(gd_teacup), ROW), ('SM_S1PEOPLE_Gd_Toadstools', lit(gd_toadstools), ROW),
    ('SM_S1PEOPLE_Gd_Hose', lit(gd_hose), ROW), ('SM_S1PEOPLE_Gd_Beans', lit(gd_beans), ROW), ('SM_S1PEOPLE_Gd_Thimbles', lit(gd_thimbles), ROW),
    ('SM_S1PEOPLE_JumpTerrace', lit(jump_terrace), GAP), ('SM_S1PEOPLE_JumpSteps', lit(jump_steps), GAP),
    ('SM_S1PEOPLE_CoveDeck_A', lit(cove_deck_a), GAP), ('SM_S1PEOPLE_CoveDeck_B', lit(cove_deck_b), GAP),
    ('SM_S1PEOPLE_BackLotDock', lit(back_lot_dock), GAP),
    ('SM_S1PEOPLE_LagoonJetty_A', lit(lagoon_jetty_a), GAP), ('SM_S1PEOPLE_LagoonJetty_B', lit(lagoon_jetty_b), GAP),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
