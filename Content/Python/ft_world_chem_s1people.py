"""Chemical Works, Stage 1, builder s1people: the people pass 3 left out. /Game/World/ChemicalWorks/SM_S1PEOPLE_*.
Run: import ft_world_chem_s1people as K; K.run()

Seven districts, three pieces each: two raised terraces (the floor 3.2 m over the land: the land lies under the road and
the barrier is 1.6 m) and one tower (the floor 12.5 m up: the high side of a banked bend, a tank row or a loading bay
in front). Every piece is 6.8 m long at most and 3.4 m deep (it stands 41.6 to 45.0 m from the centre line, a hand or a hat
brim to 41.3 m, behind the watchers' 40 m line and before the animals' 46 m band), with 3 m of footing under z = 0. All TALL. Light is steady and
small (windows, lamps). Nothing is thrown, nothing circles, nothing bobs but steam and people; no green, no orange
box, no ring, coil or ball on a column. Tank Farm B (17.4 to 21.4 km) gets no piece: see the block's header.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ChemicalWorks'

# the kit's colours (ft_dress_chem.py)
BLUE, TEAL, TEAL_D, CYAN = '#2a3f8f', '#19b5c9', '#12849a', '#5ff0ff'
YEL, NAVY, MAG, VIOLET, WHITE = '#ffd23f', '#1c1f4a', '#e84aff', '#b44dff', '#eef6ff'
ORANGE, RED, CONC, STEAM = '#ff9e3d', '#ff3355', '#3c4a78', '#d8d0f0'
LIT, RUST, PAVE, PAVE_D, STEEL = '#ffe9a8', '#a8552f', '#7f8fc6', '#56659f', '#8f9bb3'
BLACK, GLASS, PINK, PALEPINK = '#1c1a26', '#233a63', '#ff7ad9', '#f3c4dc'
LAMP, LILAC, PURPLE, MIDBLUE = '#fff1c0', '#b9a8e0', '#6a5ab0', '#3a5ab8'
WALLP, WALLP_D = '#7560b8', '#4e45a6'
BRICK, CREAM = RUST, LIT

TOPS = (WHITE, CYAN, PINK, LILAC, LAMP, MIDBLUE, RED, TEAL, PALEPINK, STEAM)
BOTTOMS = (CONC, PAVE_D, STEEL, STEAM, BLUE, PAVE)
FOOT = 300.0
D = 320.0            # a terrace's floor
TIER = (240.0, 160.0, 80.0)


def fig(sc, r, x, y, z, yaw=0.0, **kw):
    d = P.look(r, tops=TOPS, bottoms=BOTTOMS)
    d.update(kw)
    d.setdefault('phase', 0.05 + 0.9 * r.random())
    d.setdefault('detail', 1)
    return P.figure(sc, x, y, z, yaw, **d)


def kid(r):
    return dict(build='kid', s=r.uniform(0.62, 0.7))


def wave(r, amp=18, lo=0.8, hi=1.2, **kw):
    d = dict(arm_r='wave', swing_r=amp, rate=r.uniform(lo, hi))
    d.update(kw)
    return d


def cheer(r, lo=0.9, hi=1.3, **kw):
    d = dict(arm_l='vee', arm_r='vee', bob=r.uniform(3, 6), rate=r.uniform(lo, hi))
    d.update(kw)
    return d


def slab(g, col, x0, x1, y0, y1, z0, z1, glow=0.0):
    g.box(col, ((x0 + x1) / 2.0, (y0 + y1) / 2.0, (z0 + z1) / 2.0), (x1 - x0, y1 - y0, z1 - z0), glow=glow)


def block(g, col, x0, x1, y0, y1, top):
    """A raised floor with its footing."""
    slab(g, col, x0, x1, y0, y1, -FOOT, top)


def tiers(g, cols, x0, x1, yb, run=30.0):
    """Three tiers stepping down towards the road from yb (tops 2.4, 1.6, 0.8 m)."""
    for k in range(3):
        block(g, cols[k % len(cols)], x0, x1, yb - (k + 1) * run, yb - k * run, TIER[k])


def stilts(g, col, xs, ys, top, w=22.0, lean=8.0):
    for i, x in enumerate(xs):
        for j, y in enumerate(ys):
            tilt_box(g, col, (x + (lean if (i + j) % 2 else -lean), y, -FOOT), (x, y, top), w, w)


def rail(g, col, x0, x1, y, z, h=95.0, n=3):
    g.box(col, ((x0 + x1) / 2.0, y, z + h), (x1 - x0, 8, 8))
    for k in range(n + 1):
        g.box(col, (x0 + (x1 - x0) * k / float(n), y, z + h / 2.0), (8, 8, h))


def rail_y(g, col, x, y0, y1, z, h=95.0, n=2):
    g.box(col, (x, (y0 + y1) / 2.0, z + h), (8, y1 - y0, 8))
    for k in range(n + 1):
        g.box(col, (x, y0 + (y1 - y0) * k / float(n), z + h / 2.0), (8, 8, h))


def ring_rail(g, col, rad, z, n=8, h=95.0):
    for k in range(n):
        a, b = math.radians(360.0 * k / n + 22.5), math.radians(360.0 * (k + 1) / n + 22.5)
        p, q = (rad * math.cos(a), rad * math.sin(a)), (rad * math.cos(b), rad * math.sin(b))
        g.box(col, (p[0], p[1], z + h / 2.0), (8, 8, h))
        tilt_box(g, col, (p[0], p[1], z + h), (q[0], q[1], z + h), 8, 8)


def disc(g, col, x, y, z, rad, glow=0.0, steps=8, t=6.0):
    """A round plate standing on a front wall whose face is at y (a porthole, a clock face, a round door)."""
    rod(g, col, (x, y - t, z), (x, y, z), rad, steps=steps, glow=glow)


def arch_win(g, x, y, z, w, h, col=LIT, glow=0.5):
    """A round-arched window on a front wall whose face is at y."""
    g.box(col, (x, y - 3, z + h / 2.0), (w, 6, h), glow=glow)
    disc(g, col, x, y, z + h, w / 2.0, glow=glow)


def capsule(hx, rad, n=4):
    pts = []
    for sx, a0 in ((1, -90.0), (-1, 90.0)):
        for k in range(n + 1):
            a = math.radians(a0 + 180.0 * k / n)
            pts.append((sx * hx + rad * math.cos(a), rad * math.sin(a)))
    return pts


def puff(sc, r, x, y, z, rad, col=STEAM):
    p = sc.part(P.motion(bob=-7, rate=r.uniform(0.12, 0.18), phase=r.random()))
    p.sphere(col, (x, y, z), rad, steps=(3, 6))


# ---- what a hand holds (nothing thinner than 4 cm, nothing orange, nothing green)
def h_tin(col=STEEL):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] - 16), (26, 14, 18))
        m.box(col, (h[0], h[1], h[2] - 5), (5, 5, 8))
    return f


def h_board(col=WHITE, clip=STEEL):
    def f(m, h, side):
        m.box(col, (h[0] - side * 8, h[1] - 6, h[2] + 10), (30, 5, 40))
        m.box(clip, (h[0] - side * 8, h[1] - 8, h[2] + 28), (16, 5, 6))
    return f


def h_stick(col, tip, length=40.0, head=(8, 20, 9)):
    """A tool held upright in the fist: a handle and a head (a brush, a hammer, a stamp, a spade)."""
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + length / 2.0), (5, 5, length))
        m.box(tip, (h[0], h[1] - head[1] / 2.0 + 3, h[2] + length + head[2] / 2.0), head)
    return f


def h_lamp():
    def f(m, h, side):
        m.box(STEEL, (h[0], h[1], h[2] - 7), (5, 5, 14))
        m.box(LAMP, (h[0], h[1], h[2] - 25), (16, 16, 22), glow=0.6)
    return f


def h_flask(col=VIOLET):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 2, h[2] + 4), 11, 24, steps=6, top_r=4)
    return f


def h_pole(col=STEAM, length=230.0):
    """A skimming pole, held low and dipped down in front of the feet (never raised at the road)."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 30, h[2] + 46), (h[0], h[1] - length * 0.1, h[2] - length * 0.8), 5, 5)
        m.box(col, (h[0], h[1] - length * 0.1, h[2] - length * 0.8), (40, 6, 18))
    return f


def h_line(col=STEAM):
    """A short stick, a string and a horseshoe magnet."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1], h[2]), (h[0], h[1] - 24, h[2] + 30), 7, 7)
        m.box(WHITE, (h[0], h[1] - 24, h[2] - 26), (6.5, 6.5, 112))
        m.box(RED, (h[0], h[1] - 24, h[2] - 88), (22, 8, 16))
    return f


def h_ladle(col=STEEL, load=PINK):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 30, h[2] + 6), (6, 70, 6))
            m.box(col, (h[0] - 24, h[1] - 70, h[2] + 2), (34, 30, 20))
            m.box(load, (h[0] - 24, h[1] - 70, h[2] + 13), (28, 24, 4))
    return f


def h_tray(col=WHITE, a=CYAN, b=PINK):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 12, h[2] + 10), (50, 34, 5))
            m.cyl(a, (h[0] - 36, h[1] - 14, h[2] + 12), 6, 14, steps=6)
            m.cyl(b, (h[0] - 14, h[1] - 10, h[2] + 12), 6, 16, steps=6)
    return f


def h_roll(col=WHITE, length=70.0):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 4, h[2] + 6), (length, 9, 9))
    return f


CUP = P.hold_cup(WHITE)
TOWELS = P.hold_box(WHITE, 44.0)
CRATE = P.hold_box(WHITE, 50.0)

# ---- the districts' dress
CAP = dict(hat='cap', hat_col=STEEL)                                           # the gate: flat caps
COAT = dict(top=WHITE, wear='coat', sleeves='long')                             # the reactor block: white coats
HARD = dict(hat='hard', hat_col=WHITE, band=WHITE)                              # the yards
SUIT = dict(top=PALEPINK, bottom=PALEPINK, wear='coat', sleeves='long', hat='helmet', hat_col=WHITE, pack=STEEL)   # the waste run
SAILOR = dict(top=WHITE, bottom=BLUE, band=MIDBLUE, hat='peak', hat_col=WHITE, sleeves='long')                    # the port
SMITH = dict(top=LIT, bottom=CONC, band=RUST, sleeves='none')                   # the copper quarter


def robe(r):                                                                    # the spa
    c = r.choice((WHITE, LILAC, STEAM, PALEPINK))
    return dict(top=c, bottom=c, wear='coat', sleeves='long')


# ============================================================ s1d1 Factory Gate: the company town
def doorstep(sc):
    """A family waving from the doorstep of a brick terraced house: raised pavement, three steps, lit arched windows."""
    r = random.Random(101)
    g = sc.still()
    block(g, BRICK, -335, 335, -80, 170, D)
    g.box(CREAM, (0, -82, D - 16), (674, 6, 32))
    tiers(g, (CREAM,), -110, 110, -80)
    slab(g, BRICK, -335, 335, 140, 170, D, D + 400)
    slab(g, CREAM, -340, 340, 132, 170, D + 400, D + 426)
    slab(g, CREAM, -335, 335, 136, 140, D + 232, D + 248)
    slab(g, YEL, -46, 46, 134, 140, D, D + 210)
    slab(g, WHITE, -58, 58, 132, 140, D + 210, D + 226)
    for x in (-200, 200):
        arch_win(g, x, 140, D + 90, 90, 110)
        slab(g, WHITE, x - 60, x + 60, 118, 140, D + 74, D + 90)
        for k in (-1, 1):
            g.sphere(PINK, (x + k * 28, 126, D + 98), 14, steps=(3, 5))
        slab(g, BRICK, x - 15, x + 95, 140, 170, D + 426, D + 520)
        for k in (0, 1):
            g.cyl(CREAM, (x + 12 + k * 56, 155, D + 520), 16, 36, steps=6)
    g.box(WHITE, (84, 128, D + 250), (10, 24, 10))
    g.box(LAMP, (84, 114, D + 236), (22, 22, 30), glow=0.7)
    g.box(STEEL, (-240, 82, D + 23), (52, 52, 46))                                # grandfather's chair
    g.box(STEEL, (-240, 110, D + 70), (52, 8, 60))
    fig(sc, r, -60, 60, D, 5, wear='dress', **wave(r))
    fig(sc, r, 50, 70, D, -5, detail=0, arm_l='hip', **wave(r, amp=14, **CAP))
    fig(sc, r, -240, 70, D, 10, legs='sit', detail=0, **wave(r, amp=10, lo=0.5, hi=0.7, **CAP))
    fig(sc, r, 235, 50, D, -12, **cheer(r))
    fig(sc, r, -40, -95, TIER[0], 0, **cheer(r, **kid(r)))
    fig(sc, r, 70, -95, TIER[0], -8, **wave(r, **kid(r)))


def clock_on(sc):
    """Clocking on under a sawtooth canopy: the time clock (its hand creeps), the card board, a queue with lunch tins, a bicycle."""
    r = random.Random(102)
    g = sc.still()
    block(g, BRICK, -335, 335, -110, 170, D)
    g.box(CREAM, (0, -112, D - 16), (674, 6, 32))
    rail(g, WHITE, -335, 335, -104, D, n=2)
    for x in (-322, 0, 322):
        for y in (-90, 150):
            g.box(WHITE, (x, y, D + 130), (12, 12, 260))
    for x0 in (-335, 0):
        tilt_box(g, CREAM, (x0, 30, D + 262), (x0 + 335, 30, D + 360), 270, 10)
        g.box(LIT, (x0 + 330, 30, D + 310), (8, 262, 92), glow=0.4)
    g.box(CREAM, (240, 135, D + 105), (96, 50, 210))
    disc(g, WHITE, 240, 110, D + 165, 32)
    hand = sc.part(P.motion(swing=40, pivot=(240, 102, D + 165), axis=90.0, rate=0.15, phase=0.3))
    hand.box(BLACK, (240, 101, D + 178), (6, 5, 28))
    g = sc.still()
    g.box(BLUE, (90, 162, D + 140), (150, 8, 110))
    for k in range(3):
        g.box(WHITE, (42 + k * 48, 156, D + 140), (30, 5, 60))
    for xw in (-245, -155):                                                       # the bicycle
        rod(g, STEEL, (xw, 116, D + 32), (xw, 124, D + 32), 32, steps=8)
    tilt_box(g, RED, (-245, 120, D + 32), (-205, 120, D + 80), 6, 6)
    tilt_box(g, RED, (-205, 120, D + 80), (-155, 120, D + 32), 6, 6)
    g.box(RED, (-195, 120, D + 82), (70, 6, 6))
    g.box(BLACK, (-225, 120, D + 88), (26, 12, 6))
    g.box(STEEL, (-162, 120, D + 92), (6, 44, 6))
    fig(sc, r, 240, 62, D, 180, detail=0, arm_r='fwd', swing_r=10, axis_r='x', rate=0.5, **CAP)
    fig(sc, r, 150, 50, D, 90, detail=0, hold_l=h_tin(), **CAP)
    fig(sc, r, 70, 40, D, 60, detail=0, arm_l='hip', arm_r='hip', hold_r=h_tin(WHITE))
    fig(sc, r, -40, -50, D, 0, **wave(r, **CAP))
    fig(sc, r, -130, -55, D, 8, **cheer(r))
    fig(sc, r, -120, 110, D, 0, detail=0, arm_l='hip', **CAP)
    fig(sc, r, -270, -50, D, 12, wear='dress', **wave(r))


def hose_tower(sc):
    """The works fire station's hose tower: a brick shaft, hoses hung to dry, the crew on the gallery 12.6 m up, a bell swinging."""
    r = random.Random(103)
    g = sc.still()
    slab(g, BRICK, -110, 110, -110, 110, -FOOT, 1240)
    for z in (300, 620, 940):
        g.box(CREAM, (0, 0, z), (232, 232, 26))
    slab(g, YEL, -50, 50, -116, -110, 0, 210)
    disc(g, YEL, 0, -110, 210, 50)
    for z in (430, 760, 1050):
        slab(g, LIT, -24, 24, -116, -110, z, z + 100, glow=0.5)
    for k in range(3):
        g.box(STEAM, (62 + k * 18, -114, 830 - k * 60), (10, 6, 700 - k * 120))    # hoses hung to dry
    g.box(WHITE, (0, 0, 1250), (540, 340, 20))
    for sx in (-1, 1):
        tilt_box(g, CREAM, (sx * 110, -60, 1090), (sx * 250, -60, 1240), 20, 20)
        tilt_box(g, CREAM, (sx * 110, 60, 1090), (sx * 250, 60, 1240), 20, 20)
        rail_y(g, WHITE, sx * 266, -166, 166, 1260)
    rail(g, WHITE, -266, 266, -166, 1260, n=4)
    slab(g, CREAM, -100, 100, -100, 100, 1260, 1540)
    g.cyl(BRICK, (0, 0, 1540), 170, 26, steps=8)
    g.cyl(CREAM, (0, 0, 1566), 160, 240, steps=8, top_r=12)
    g.box(WHITE, (0, -120, 1500), (10, 44, 10))
    bell = sc.part(P.motion(swing=14, pivot=(0, -138, 1496), axis=0.0, rate=0.6, phase=0.2))
    bell.cyl(STEEL, (0, -138, 1444), 26, 48, steps=8, top_r=12)
    crew = dict(hat='hard', hat_col=RED, top=MIDBLUE, band=WHITE)
    fig(sc, r, -190, -138, 1260, 10, **cheer(r, **crew))
    fig(sc, r, -70, -140, 1260, 0, **wave(r, **crew))
    fig(sc, r, 70, -140, 1260, 0, arm_r='up', swing_r=10, axis_r='x', rate=0.6, phase=0.2, **crew)
    fig(sc, r, 190, -138, 1260, -10, **wave(r, **crew))
    fig(sc, r, -190, 20, 1260, 70, **wave(r, **crew))
    fig(sc, r, 190, 20, 1260, -70, **cheer(r, **crew))


# ============================================================ s1d2 Reactor Block: the research quarter
def blackboard(sc):
    """Two white coats arguing at a blackboard, a class on a bench: a purple lozenge deck overhanging its base."""
    r = random.Random(201)
    g = sc.still()
    lz = [(-340, 0), (-250, -170), (250, -170), (340, 0), (250, 170), (-250, 170)]
    g.plan(WALLP_D, [(x * 0.7, y * 0.7) for x, y in lz], -FOOT, D - 50)
    g.plan(WALLP, lz, D - 50, D)
    g.plan(WHITE, [(x * 0.99, y * 0.99) for x, y in lz], D - 62, D - 50)
    for x in (-110, 0, 110):
        disc(g, VIOLET, x, -119, 150, 26, glow=0.5)
    g.box(WHITE, (130, 150, D + 175), (250, 8, 150))
    g.box(NAVY, (130, 145, D + 175), (230, 6, 130))
    for k, (x, z, w, h) in enumerate(((60, 210, 50, 6), (60, 180, 6, 50), (130, 190, 60, 6), (190, 170, 6, 60), (170, 215, 40, 6))):
        g.box(WHITE if k % 2 else CYAN, (x, 141, D + z), (w, 4, h))                  # strokes and figures, never words
    for x in (30, 230):
        tilt_box(g, WALLP_D, (x, 160, D), (x, 150, D + 110), 10, 10)
    for sx in (-1, 1):                                                              # fins
        g.box(WALLP_D, (sx * 262, 110, D + 160), (14, 60, 320))
        g.box(CYAN, (sx * 270, 110, D + 160), (4, 20, 280), glow=0.35)
    g.box(WALLP_D, (-170, 80, D + 23), (230, 44, 46))                               # the bench
    fig(sc, r, 60, 80, D, 90, detail=0, arm_r='point', swing_r=14, rate=0.7, **COAT)
    fig(sc, r, 215, 70, D, -90, arm_l='point', arm_r='hip', swing_l=12, rate=0.9, hat='cap', hat_col=WHITE, **COAT)
    fig(sc, r, -245, 70, D, 30, legs='sit', detail=0, arm_l='hold', arm_r='hold', hold_r=h_board())
    fig(sc, r, -170, 70, D, 25, legs='sit', detail=0, arm_r='up', swing_r=8, rate=0.6)
    fig(sc, r, -95, 70, D, 20, legs='sit', detail=0, arm_l='hold', arm_r='hold', hold_r=h_board(CYAN))
    fig(sc, r, 0, -110, D, 0, **wave(r, **COAT))
    fig(sc, r, -160, -90, D, 10, **cheer(r))


def flask_walk(sc):
    """A trolley of giant flasks pushed to and fro on a round-ended deck, an instrument on a tripod, clipboards."""
    r = random.Random(202)
    g = sc.still()
    g.plan(WALLP_D, capsule(120, 120), -FOOT, D - 40)
    g.plan(VIOLET, capsule(123, 123), 120, 180, glow=0.45)
    g.plan(WALLP, capsule(170, 170), D - 40, D)
    g.plan(WHITE, capsule(167, 167), D - 52, D - 40)
    for sx in (-1, 1):
        g.box(WALLP_D, (sx * 236, 112, D + 165), (14, 60, 330))
        g.box(CYAN, (sx * 244, 112, D + 165), (4, 20, 290), glow=0.35)
    for a, b in (((170, 20), (200, 30)), ((230, 20), (200, 30)), ((200, 70), (200, 30))):
        tilt_box(g, WHITE, (a[0], a[1], D), (b[0], b[1], D + 128), 6, 6)             # the tripod
    g.box(CYAN, (200, 26, D + 138), (30, 44, 22))
    piv = (-70, -40, D - 3000.0)                                                    # 1 degree about a point 30 m down: half a metre each way
    t = sc.part(P.motion(swing=1.0, pivot=piv, axis=90.0, rate=0.25, phase=0.4))
    t.box(WHITE, (20, -40, D + 72), (124, 60, 8))
    t.box(WHITE, (20, -40, D + 26), (124, 60, 8))
    for x in (-38, 78):
        t.box(STEEL, (x, -40, D + 38), (6, 56, 76))
    for k, c in enumerate((VIOLET, WHITE, CYAN)):
        t.cyl(c, (-12 + k * 34, -40, D + 76), 15, 22, steps=6)
        t.cyl(c, (-12 + k * 34, -40, D + 98), 15, 30, steps=6, top_r=6)
    fig(sc, r, -70, -40, D, 90, arm_l='push', arm_r='push', sway=dict(swing=1.0, pivot=piv, axis=90.0), rate=0.25, phase=0.4, **COAT)
    fig(sc, r, 200, 86, D, 0, detail=0, arm_r='face', hair='bun', **COAT)
    fig(sc, r, -215, 50, D, 30, detail=0, arm_l='hold', arm_r='hold', hold_l=h_board(), swing_r=8, axis_r='z', rate=1.0, **COAT)
    fig(sc, r, 90, 100, D, -20, detail=0, arm_l='hold', arm_r='hold', hold_r=h_board(CYAN), **COAT)
    fig(sc, r, -150, -100, D, 8, **wave(r, **COAT))
    fig(sc, r, 150, -90, D, -8, **cheer(r))


def observatory(sc):
    """A small white dome on a purple stalk: astronomers on the ring gallery 12.6 m up, the telescope out of its slot."""
    r = random.Random(203)
    g = sc.still()
    g.cyl(WALLP_D, (0, 0, -FOOT), 78, FOOT + 1170, steps=10, top_r=62)
    g.cyl(WALLP, (0, 0, 1170), 70, 70, steps=10, top_r=165)
    g.cyl(WHITE, (0, 0, 1240), 170, 18, steps=10)
    g.cyl(WALLP, (0, 0, 1258), 98, 170, steps=10)
    g.cyl(VIOLET, (0, 0, 1330), 101, 44, steps=10, glow=0.5)
    g.sphere(WHITE, (0, 0, 1428), 108, scale=(1, 1, 0.85), steps=(5, 10))
    g.box(GLASS, (40, -30, 1470), (150, 40, 110))
    rod(g, WHITE, (30, -30, 1440), (140, -44, 1560), 15, steps=6)
    for z in (300, 620, 940):
        disc(g, CYAN, 0, -68, z, 18, glow=0.4, steps=6)
    slab(g, WHITE, -30, 30, -84, -74, 0, 190)
    ring_rail(g, WHITE, 166, 1258)
    fig(sc, r, -100, -88, 1258, 35, **wave(r, **COAT))
    fig(sc, r, 0, -134, 1258, 0, **cheer(r, **COAT))
    fig(sc, r, 100, -88, 1258, -35, arm_r='face', hold_r=CUP, arm_l='hip', **COAT)
    fig(sc, r, 134, 20, 1258, -80, arm_r='point', swing_r=8, rate=0.5, **COAT)
    fig(sc, r, -134, 20, 1258, 80, **wave(r))


# ============================================================ s1d3 Loading Yards: the freight town
def cafe_hatch(sc):
    """The drivers' cafe: a blue container with a lit hatch and a striped awning on a yellow dock; drivers on stools, the cook."""
    r = random.Random(301)
    g = sc.still()
    block(g, YEL, -335, 335, -110, 170, D)
    for k in range(6):
        tilt_box(g, BLACK, (-300 + k * 110, -112, D - 80), (-250 + k * 110, -112, D - 10), 5, 16)
    slab(g, MIDBLUE, -250, 250, 60, 170, D, D + 250)
    for k in range(6):
        g.box(BLUE, (-225 + k * 90, 57, D + 125), (14, 6, 240))
    slab(g, LIT, -150, 90, 55, 60, D + 105, D + 200, glow=0.5)
    slab(g, WHITE, -170, 110, 20, 60, D + 95, D + 105)
    for k in range(6):
        tilt_box(g, RED if k % 2 else WHITE, (-155 + k * 50, 60, D + 255), (-155 + k * 50, -30, D + 215), 50, 8)
    g.box(STEEL, (-170, 120, D + 270), (70, 60, 40))                                # a roof vent
    g.box(STEEL, (190, 150, D + 310), (8, 8, 120))                                  # a sign: an arrow, no words
    g.box(RED, (190, 144, D + 390), (110, 6, 60))
    g.box(WHITE, (180, 140, D + 390), (60, 4, 12))
    tilt_box(g, WHITE, (228, 140, D + 390), (208, 140, D + 408), 4, 10)
    tilt_box(g, WHITE, (228, 140, D + 390), (208, 140, D + 372), 4, 10)
    for x in (-120, -30, 60):
        g.cyl(STEEL, (x, 0, D), 17, 55, steps=6)
    fig(sc, r, -120, -5, D, 0, legs='sit', seat=55, hold_l=CUP, **wave(r, **CAP))
    fig(sc, r, -30, -5, D, 5, legs='sit', seat=55, arm_r='face', hold_r=CUP, swing_r=5, axis_r='x', rate=0.4, **HARD)
    fig(sc, r, 60, -5, D, -10, legs='sit', seat=55, detail=0, arm_l='vee', arm_r='vee', swing_l=8, swing_r=8, rate=1.0)
    fig(sc, r, 175, 30, D, -10, detail=0, top=WHITE, hat='chef', hat_col=WHITE, arm_l='hold', arm_r='hold', hold_r=h_tray(), rock=3, rate=0.5)
    fig(sc, r, -275, -50, D, 12, **wave(r, **HARD))
    fig(sc, r, 275, -60, D, -12, **cheer(r, **CAP))


def tally_dock(sc):
    """Checkers with tally boards, a loader at a drum, a customs man at an open crate: a paved dock before a red sliding door."""
    r = random.Random(302)
    g = sc.still()
    block(g, PAVE, -335, 335, -110, 170, D)
    g.box(YEL, (0, -112, D - 14), (674, 6, 28))
    slab(g, RED, 40, 335, 150, 170, D, D + 330)
    for x in (90, 185, 280):
        g.box(WHITE, (x, 147, D + 165), (10, 6, 320))
    g.box(WHITE, (140, 146, D + 290), (70, 5, 18))                                  # a painted 7
    tilt_box(g, WHITE, (170, 146, D + 284), (130, 146, D + 180), 5, 18)
    slab(g, WHITE, -300, -190, 60, 160, D, D + 90)
    slab(g, MIDBLUE, -180, -80, 70, 160, D, D + 90)
    slab(g, WHITE, -250, -140, 70, 150, D + 90, D + 170)
    slab(g, BLUE, -256, -134, 64, 156, D + 170, D + 178)
    rod(g, MIDBLUE, (20, -35, D + 42), (20, 45, D + 42), 42, steps=10)              # a drum on its side
    for y in (-18, 22):
        rod(g, WHITE, (20, y, D + 42), (20, y + 8, D + 42), 44, steps=10)
    slab(g, WHITE, 205, 300, 30, 110, D, D + 70)                                   # the opened crate
    tilt_box(g, WHITE, (205, 70, D + 70), (170, 70, D + 130), 80, 8)
    g.box(TEAL, (252, 70, D + 74), (70, 60, 10))
    fig(sc, r, -50, 5, D, 90, arm_l='push', arm_r='push', swing_l=5, swing_r=5, axis_l='x', axis_r='x', rate=0.5, **HARD)
    fig(sc, r, -195, 110, D + 178, 0, detail=0, arm_l='hold', arm_r='hold', hold_l=h_board(), swing_r=8, axis_r='z', rate=1.1, **CAP)
    fig(sc, r, 110, 80, D, 20, detail=0, arm_l='hold', arm_r='hold', hold_r=h_board(CYAN), **HARD)
    fig(sc, r, 165, 20, D, 70, detail=0, arm_r='fwd', hold_r=h_stick(STEEL, STEEL, 46.0, (8, 16, 8)), swing_r=10, axis_r='x', rate=0.6, hat='peak', hat_col=MIDBLUE, top=MIDBLUE)
    fig(sc, r, -150, -60, D, 8, **cheer(r, **HARD))
    fig(sc, r, 110, -70, D, -6, **wave(r, **CAP))
    fig(sc, r, 290, -65, D, -12, **wave(r, **kid(r)))


def signal_cabin(sc):
    """A glazed signal cabin on a yellow steel tower: the signalman at his levers, shunters with steady lamps, a semaphore arm."""
    r = random.Random(303)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(YEL, (sx * 140, sy * 110, (1240 - FOOT) / 2.0), (24, 24, 1240 + FOOT))
        for z0 in (0, 620):
            tilt_box(g, YEL, (-140, sx * 110, z0), (140, sx * 110, z0 + 600), 14, 14)
            tilt_box(g, YEL, (sx * 140, -110, z0 + 600), (sx * 140, 110, z0), 14, 14)
    tilt_box(g, STEEL, (-275, 150, -100), (-150, 150, 1244), 40, 10)                # the stair
    g.box(STEEL, (0, 0, 1250), (560, 340, 20))
    slab(g, YEL, -190, 190, -10, 160, 1260, 1330)
    slab(g, LIT, -186, 186, -6, 156, 1330, 1430, glow=0.4)
    for x in (-190, 0, 190):
        g.box(YEL, (x, -10, 1380), (10, 10, 100))
    slab(g, RED, -215, 215, -40, 170, 1430, 1452)
    rail(g, WHITE, -276, 276, -166, 1260, n=4)
    for sx in (-1, 1):
        rail_y(g, WHITE, sx * 276, -166, -10, 1260, n=1)
    for k, c in enumerate((RED, WHITE, MIDBLUE)):
        g.box(c, (-36 + k * 36, -40, 1305), (7, 7, 90))
    g.box(STEEL, (232, 120, 1450), (10, 10, 380))
    arm = sc.part(P.motion(swing=22, pivot=(232, 112, 1600), axis=90.0, rate=0.2, phase=0.1))
    arm.box(RED, (285, 112, 1600), (106, 6, 22))
    arm.box(WHITE, (318, 108, 1600), (16, 5, 22))
    fig(sc, r, 0, -78, 1260, 160, arm_r='fwd', swing_r=12, axis_r='x', rate=0.5, hat='peak', hat_col=MIDBLUE, top=MIDBLUE)
    fig(sc, r, -200, -128, 1260, 10, arm_r='wave', hold_r=h_lamp(), swing_r=14, rate=0.8, **HARD)
    fig(sc, r, 200, -128, 1260, -10, arm_l='wave', hold_l=h_lamp(), swing_l=14, rate=0.7, **HARD)
    fig(sc, r, 100, -130, 1260, 0, **cheer(r, **CAP))
    fig(sc, r, -100, -130, 1260, 0, **wave(r))


# ============================================================ s1d4 Waste Run: the sludge works
def skim_jetty(sc):
    """Suited skimmers with poles on a duckboard jetty over pink liquor, a ladle man, two children fishing with a magnet."""
    r = random.Random(401)
    g = sc.still()
    for k in range(8):
        g.box(PALEPINK if k % 2 else STEEL, (-294 + k * 84, 0, D - 7), (82, 324, 14))
    stilts(g, BLACK, (-300, 0, 300), (-140, 140), D - 14)
    tilt_box(g, BLACK, (-300, -140, 40), (0, -140, D - 30), 12, 12)
    tilt_box(g, BLACK, (300, -140, 40), (0, -140, D - 30), 12, 12)
    slab(g, MAG, -330, 330, -165, 165, -FOOT, 30)
    g.box(STEEL, (250, 115, D + 25), (150, 70, 50))
    g.box(PINK, (250, 115, D + 52), (138, 58, 6))
    tilt_box(g, BLACK, (-312, 140, D), (-292, 140, D + 330), 10, 10)
    for k, c in enumerate((PINK, WHITE, MAG)):
        g.box(c, (-270 + k * 6, 134, D + 300 - k * 52), (92, 6, 40), rot=((-5, 4, -3)[k], 0, 0))
    fig(sc, r, -200, -84, D, 0, arm_r='fwd', hold_r=h_pole(), swing_r=8, axis_r='x', rate=0.5, **SUIT)
    fig(sc, r, 60, -84, D, -8, arm_l='fwd', hold_l=h_pole(), swing_l=8, axis_l='x', rate=0.45, **SUIT)
    fig(sc, r, 235, 45, D, 150, detail=0, arm_l='carry', arm_r='carry', hold_r=h_ladle(), swing_l=6, swing_r=6, axis_l='x', axis_r='x', rate=0.4, **SUIT)
    fig(sc, r, 140, 30, D, -5, detail=0, **cheer(r, **SUIT))
    fig(sc, r, -85, -146, D - 30, 0, legs='sit', seat=30, arm_r='fwd', hold_r=h_line(), swing_r=6, axis_r='x', rate=0.4, **kid(r))
    fig(sc, r, -25, -146, D - 30, 0, legs='sit', seat=30, arm_l='vee', arm_r='vee', swing_l=8, swing_r=8, rate=1.1, **kid(r))


def drum_wash(sc):
    """The drum wash: a crooked lean-to on stilts, a crew scrubbing upturned drums, a tub of pink liquor, a bent stove pipe."""
    r = random.Random(402)
    g = sc.still()
    slab(g, STEEL, -335, 335, -120, 170, D - 16, D)
    stilts(g, BLACK, (-310, 0, 310), (-100, 150), D - 16)
    slab(g, PALEPINK, -320, 320, 156, 170, D, D + 300)
    g.box(STEEL, (-200, 153, D + 190), (90, 6, 70))
    g.box(STEEL, (120, 153, D + 110), (70, 6, 90))
    for x, h in ((-80, 130), (40, 90), (250, 150)):
        g.box(MAG, (x, 153, D + 300 - h / 2.0), (30, 5, h))
    tilt_box(g, BLACK, (-335, 40, D + 310), (335, 40, D + 400), 270, 12)
    g.box(BLACK, (-322, -88, D + 152), (14, 14, 304))
    g.box(BLACK, (322, -88, D + 197), (14, 14, 394))
    rod(g, BLACK, (200, 120, D + 380), (200, 120, D + 500), 12, steps=6)
    rod(g, BLACK, (200, 120, D + 496), (245, 120, D + 560), 12, steps=6)
    g.box(STEEL, (-140, 120, D + 70), (360, 10, 10))
    for x in (-300, 20):
        g.box(STEEL, (x, 120, D + 35), (10, 10, 70))
    for k, c in enumerate((WHITE, TEAL, WHITE)):
        g.cyl(c, (-250 + k * 110, 120, D + 75), 38, 78, steps=8)
    g.box(PINK, (190, 60, D + 30), (170, 100, 60))
    g.box(MAG, (190, 60, D + 62), (158, 88, 6))
    rod(g, TEAL, (70, -70, D + 40), (70, 10, D + 40), 40, steps=8)
    brush = h_stick(STEEL, WHITE, 30.0, (22, 14, 10))
    fig(sc, r, -195, 60, D, 200, detail=0, arm_r='fwd', hold_r=brush, swing_r=16, axis_r='x', rate=0.9, **SUIT)
    fig(sc, r, -85, 60, D, 160, detail=0, arm_l='fwd', hold_l=brush, swing_l=16, axis_l='x', rate=0.8, **SUIT)
    fig(sc, r, 0, -30, D, 90, arm_l='push', arm_r='push', swing_l=5, swing_r=5, axis_l='x', axis_r='x', rate=0.5, **SUIT)
    fig(sc, r, -270, -70, D, 10, **cheer(r, **SUIT))
    fig(sc, r, 160, -70, D, -5, wear='dress', **wave(r))
    fig(sc, r, 275, -80, D, -12, **cheer(r, **kid(r)))


def assay_tower(sc):
    """The filter house: a thin tin tower with three funnel hoppers and a crooked hut; the assayer holds a flask to the lamp."""
    r = random.Random(403)
    g = sc.still()
    slab(g, PALEPINK, -100, 100, -100, 100, -FOOT, 1240)
    tilt_box(g, BLACK, (-230, -60, -100), (-100, -60, 700), 16, 16)
    tilt_box(g, BLACK, (240, 40, -100), (100, 40, 560), 16, 16)
    for x, z, w, h in ((-40, 300, 90, 120), (50, 880, 110, 80), (-30, 1090, 70, 110)):
        g.box(STEEL, (x, -103, z), (w, 6, h))
    for x, h in ((-70, 260), (20, 180), (75, 330)):
        g.box(MAG, (x, -103, 1240 - h / 2.0), (26, 5, h))
    for x in (-66, 0, 66):
        g.cyl(STEEL, (x, -130, 560), 12, 110, steps=6, top_r=36)
        g.box(BLACK, (x, -114, 600), (8, 32, 8))
    g.box(STEEL, (0, 0, 1249), (500, 340, 18))
    for sx in (-1, 1):
        tilt_box(g, BLACK, (sx * 100, 0, 1080), (sx * 240, 0, 1240), 14, 14)
        rail_y(g, PINK, sx * 246, -166, 166, 1258)
    rail(g, PINK, -246, 246, -166, 1258, n=4)
    slab(g, PALEPINK, -90, 90, 10, 160, 1258, 1470)
    slab(g, LIT, -36, 36, 4, 10, 1340, 1420, glow=0.5)
    tilt_box(g, BLACK, (-130, 85, 1480), (130, 85, 1560), 190, 10)
    rod(g, BLACK, (40, 120, 1520), (40, 120, 1640), 11, steps=6)
    rod(g, BLACK, (40, 120, 1636), (78, 120, 1690), 11, steps=6)
    g.box(PINK, (-246, -166, 1400), (8, 8, 190))
    g.box(LAMP, (-232, -152, 1500), (26, 26, 32), glow=0.6)
    fig(sc, r, -185, -108, 1258, 30, arm_l='face', hold_l=h_flask(), arm_r='hip', rock=2, rate=0.3, **SUIT)
    fig(sc, r, -60, -132, 1258, 0, **cheer(r, **SUIT))
    fig(sc, r, 60, -132, 1258, 0, **wave(r, **SUIT))
    fig(sc, r, 180, -125, 1258, -20, **wave(r, **kid(r)))
    fig(sc, r, 190, 40, 1258, -80, **cheer(r, **SUIT))


# ============================================================ s1d5 Export Terminal: the port (slow, steady: the boss's arena)
def mission_steps(sc):
    """Sailors on leave on the white steps of the seamen's mission: portholes, one red band, kitbags. Slow waves."""
    r = random.Random(501)
    g = sc.still()
    block(g, WHITE, -335, 335, -80, 170, D)
    tiers(g, (WHITE, TEAL_D, WHITE), -335, 335, -80)
    slab(g, WHITE, -335, 335, 140, 170, D, D + 300)
    g.box(RED, (0, 138, D + 262), (670, 5, 24))
    slab(g, TEAL_D, -45, 45, 134, 140, D, D + 200)
    for x in (-210, 210):
        disc(g, LAMP, x, 140, D + 170, 30, glow=0.5, t=8.0)
        disc(g, TEAL_D, x, 140, D + 170, 37, t=4.0)
    slab(g, BLUE, -340, 340, 130, 170, D + 300, D + 322)
    g.side(BLUE, [(-90, D + 322), (90, D + 322), (0, D + 388)], 130, 170)
    for sx in (-1, 1):
        rail_y(g, TEAL, sx * 330, -76, 136, D, n=2)
        g.box(TEAL, (sx * 330, 30, D + 50), (6, 212, 6))
    for x, y in ((150, 100), (195, 110)):
        g.cyl(WHITE, (x, y, D), 22, 70, steps=6, top_r=16)
    fig(sc, r, -220, -60, TIER[0], 0, legs='sit', seat=80, **wave(r, amp=12, lo=0.5, hi=0.7, **SAILOR))
    fig(sc, r, -130, -60, TIER[0], 5, legs='sit', seat=80, arm_r='face', hold_r=CUP, **SAILOR)
    fig(sc, r, 0, 50, D, 0, detail=0, **wave(r, amp=14, lo=0.5, hi=0.8, **SAILOR))
    fig(sc, r, 105, 40, D, -15, detail=0, arm_l='hip', arm_r='point', **SAILOR)
    fig(sc, r, 235, -95, TIER[0], -8, **cheer(r, lo=0.5, hi=0.7, **SAILOR))
    fig(sc, r, -40, -95, TIER[0], 0, wear='dress', top=LAMP, **wave(r, amp=12, lo=0.5, hi=0.8))


def customs_desk(sc):
    """Officials stamping papers at a teal desk under a white colonnade; a docker and a sailor wait their turn."""
    r = random.Random(502)
    g = sc.still()
    block(g, WHITE, -335, 335, -130, 170, D)
    g.box(TEAL_D, (0, -132, 30), (674, 6, 60))
    for x in (-310, -105, 105, 310):
        g.cyl(WHITE, (x, -105, D), 20, 280, steps=8)
    slab(g, WHITE, -338, 338, -130, -80, D + 280, D + 320)
    g.box(RED, (0, -132, D + 300), (676, 5, 14))
    slab(g, BLUE, -340, 340, -135, 170, D + 320, D + 340)
    slab(g, WHITE, -335, 335, 150, 170, D, D + 280)
    for x in (-200, 200):
        g.box(LAMP, (x, 147, D + 170), (80, 6, 120), glow=0.45)
    g.box(TEAL_D, (-85, 60, D + 40), (230, 70, 80))
    for k in range(3):
        g.box(WHITE, (-150 + k * 60, 55, D + 83), (30, 22, 5))
    g.box(WHITE, (270, 100, D + 40), (100, 90, 80))
    g.box(TEAL, (265, 100, D + 115), (80, 80, 70))
    stamp = h_stick(STEEL, RED, 16.0, (14, 14, 10))
    official = dict(top=BLUE, bottom=BLUE, hat='peak', hat_col=WHITE, sleeves='long', legs='sit', seat=50)
    fig(sc, r, -140, 108, D, 0, arm_r='fwd', hold_r=stamp, swing_r=10, axis_r='x', rate=0.6, **official)
    fig(sc, r, -30, 108, D, 0, arm_l='hold', arm_r='hold', swing_r=6, axis_r='z', rate=0.7, **official)
    fig(sc, r, 90, 20, D, 120, arm_l='carry', arm_r='carry', hold_r=CRATE, top=TEAL, **CAP)
    fig(sc, r, 185, 30, D, 110, detail=0, arm_l='hold', hold_l=h_board(), **SAILOR)
    fig(sc, r, -210, -60, D, 8, **wave(r, amp=12, lo=0.5, hi=0.8, **SAILOR))
    fig(sc, r, 210, -62, D, -8, **cheer(r, lo=0.5, hi=0.7, **kid(r)))


def hoist_loft(sc):
    """A slice of gabled quay warehouse: red doors one above another, dockers on the top loading stage 12.5 m up working the hoist."""
    r = random.Random(503)
    g = sc.still()
    slab(g, WHITE, -160, 160, -60, 170, -FOOT, 1500)
    slab(g, TEAL_D, -164, 164, -64, 170, -FOOT, 110)
    g.box(RED, (0, -62, 640), (324, 5, 40))
    for z in (130, 700):
        slab(g, RED, -55, 55, -66, -60, z, z + 190)
    for x in (-110, 110):
        for z in (330, 900):
            g.box(LAMP, (x, -62, z), (44, 5, 70), glow=0.4)
    g.side(BLUE, [(-180, 1500), (180, 1500), (0, 1680)], -70, 170)
    g.box(STEEL, (0, -115, 1240), (440, 110, 16))
    for sx in (-1, 1):
        tilt_box(g, TEAL_D, (sx * 150, -62, 1080), (sx * 150, -160, 1232), 14, 14)
        rail_y(g, TEAL, sx * 216, -166, -64, 1248, n=1)
    rail(g, TEAL, -216, 216, -166, 1248, n=4)
    slab(g, GLASS, -70, 70, -64, -60, 1250, 1450)
    g.box(WHITE, (0, -105, 1560), (14, 130, 14))
    sack = sc.part(P.motion(swing=5, pivot=(0, -150, 1553), axis=90.0, rate=0.3, phase=0.2))
    sack.box(STEAM, (0, -150, 1525), (5, 5, 56))
    sack.box(STEAM, (0, -150, 1472), (46, 36, 54))
    docker = dict(top=TEAL, **CAP)
    fig(sc, r, -160, -118, 1248, 12, **wave(r, amp=12, lo=0.5, hi=0.8, **docker))
    fig(sc, r, -75, -118, 1248, 0, arm_l='up', arm_r='up', swing_l=6, swing_r=6, axis_l='x', axis_r='x', rate=0.3, phase=0.2, **docker)
    fig(sc, r, 75, -118, 1248, 0, arm_r='face', arm_l='hip', **SAILOR)
    fig(sc, r, 160, -118, 1248, -12, **cheer(r, lo=0.5, hi=0.7, **docker))


# ============================================================ s1d6 Cooling Field: the spa in the steam
def chess_terrace(sc):
    """Bathers in robes on a stepped stone terrace: two at a chess table, a nurse with a wheeled chair, urns, a lamp of three globes."""
    r = random.Random(601)
    g = sc.still()
    block(g, STEAM, -335, 335, -80, 170, D)
    tiers(g, (LILAC, STEAM, LILAC), -335, 335, -80)
    g.box(PAVE, (0, 162, D + 90), (670, 12, 12))
    for k in range(5):
        g.box(PAVE, (-324 + k * 162, 162, D + 42), (12, 12, 84))
    for sx in (-1, 1):
        g.box(STEAM, (sx * 296, 118, D + 35), (44, 44, 70))
        g.cyl(LILAC, (sx * 296, 118, D + 70), 14, 40, steps=6, top_r=26)
        g.sphere(TEAL_D, (sx * 296, 118, D + 130), 26, steps=(3, 6))
    g.cyl(PAVE, (-170, 60, D), 40, 66, steps=8)
    g.box(WHITE, (-170, 60, D + 68), (44, 44, 5))
    for k, (dx, dy) in enumerate(((-12, -10), (10, 8), (-8, 12), (12, -12))):
        g.box(NAVY if k % 2 else LILAC, (-170 + dx, 60 + dy, D + 78), (7, 7, 16))
    for x in (-245, -95):
        g.cyl(LILAC, (x, 60, D), 18, 46, steps=6)
    g.box(WHITE, (110, 14, D + 50), (50, 46, 8))                                    # the wheeled chair
    g.box(WHITE, (110, 36, D + 82), (50, 8, 64))
    for x in (80, 140):
        rod(g, STEEL, (x - 4, 14, D + 30), (x + 4, 14, D + 30), 30, steps=8)
    g.box(STEAM, (0, 150, D + 130), (10, 10, 260))
    g.box(STEAM, (0, 150, D + 262), (90, 8, 8))
    for x in (-40, 0, 40):
        g.sphere(LAMP, (x, 150, D + 284), 16, steps=(3, 6), glow=0.6)
    fig(sc, r, -245, 60, D, 90, legs='sit', detail=0, arm_r='fwd', swing_r=8, axis_r='x', rate=0.3, **robe(r))
    fig(sc, r, -95, 60, D, -90, legs='sit', arm_l='face', detail=0, **robe(r))
    fig(sc, r, 110, 8, D, 0, legs='sit', seat=54, **wave(r, amp=12, lo=0.6, hi=0.9, **robe(r)))
    fig(sc, r, 110, 78, D, 0, detail=0, arm_l='push', arm_r='push', top=WHITE, bottom=WHITE, wear='dress', hat='cap', hat_col=WHITE)
    fig(sc, r, -40, -95, TIER[0], 0, arm_l='carry', arm_r='carry', hold_r=TOWELS, top=WHITE, bottom=PURPLE)
    fig(sc, r, 255, 20, D, -10, detail=0, **cheer(r, **robe(r)))
    fig(sc, r, 200, -95, TIER[0], -6, **wave(r, **robe(r)))


def plunge_rim(sc):
    """Taking the waters: bathers in robes on the rim of a raised plunge pool, one in it, steam drifting, a deck chair."""
    r = random.Random(602)
    g = sc.still()
    block(g, PAVE, -335, 335, -130, 170, D)
    g.box(STEAM, (0, -132, D - 14), (674, 6, 28))
    slab(g, STEAM, -250, 250, 10, 36, D, D + 40)
    slab(g, STEAM, -250, 250, 140, 166, D, D + 40)
    for sx in (-1, 1):
        slab(g, STEAM, sx * 237 - 13, sx * 237 + 13, 36, 140, D, D + 40)
    slab(g, LILAC, -224, 224, 36, 140, D, D + 24)
    for x, y, z, rad in ((-120, 100, 170, 34), (40, 80, 230, 26), (160, 110, 150, 30)):
        puff(sc, r, x, y, D + z, rad)
    g = sc.still()
    tilt_box(g, LILAC, (-70, -70, D + 16), (-30, -70, D + 16), 60, 6)                # a deck chair
    tilt_box(g, WHITE, (-30, -70, D + 16), (20, -70, D + 80), 60, 6)
    tilt_box(g, PURPLE, (-74, -70, D), (10, -70, D + 50), 6, 6)
    g.cyl(PURPLE, (300, 120, D), 9, 190, steps=6, top_r=5)                          # a folded parasol
    g.cyl(STEAM, (300, -80, D), 30, 40, steps=8)
    g.sphere(TEAL_D, (300, -80, D + 72), 36, steps=(4, 6))
    fig(sc, r, -150, 22, D, 0, legs='sit', seat=40, **wave(r, amp=12, lo=0.6, hi=0.9, **robe(r)))
    fig(sc, r, -50, 22, D, 6, legs='sit', seat=40, arm_r='face', hold_r=CUP, **robe(r))
    fig(sc, r, 120, 92, D - 80, 0, arm_l='vee', arm_r='vee', swing_l=10, swing_r=10, rate=0.9, top=LILAC, sleeves='none', hat='cap', hat_col=WHITE)
    fig(sc, r, -280, -60, D, 12, arm_l='carry', arm_r='carry', hold_r=TOWELS, top=WHITE, bottom=PURPLE)
    fig(sc, r, 210, -70, D, -10, **cheer(r, **robe(r)))
    fig(sc, r, 90, -80, D, 0, **wave(r, **robe(r)))


def belvedere(sc):
    """A tea belvedere on a stepped lavender tower: six columns under a pagoda-stepped roof, bathers in robes at the rail 12.7 m up."""
    r = random.Random(603)
    g = sc.still()
    slab(g, LILAC, -170, 170, -165, 165, -FOOT, 420)
    slab(g, STEAM, -140, 140, -140, 140, 420, 860)
    slab(g, LILAC, -110, 110, -110, 110, 860, 1250)
    for x in (-80, 80):
        arch_win(g, x, -165, 130, 60, 150, LAMP, 0.4)
    arch_win(g, 0, -140, 560, 70, 150, LAMP, 0.4)
    g.box(PAVE, (0, 0, 428), (350, 340, 16))
    g.box(PAVE, (0, 0, 868), (290, 290, 16))
    g.cyl(STEAM, (0, 0, 1250), 170, 22, steps=8)
    for k in range(6):
        a = math.radians(30 + k * 60)
        g.cyl(STEAM, (150 * math.cos(a), 150 * math.sin(a), 1272), 12, 220, steps=6)
    g.cyl(PURPLE, (0, 0, 1492), 168, 40, steps=8, top_r=110)
    g.cyl(LILAC, (0, 0, 1532), 110, 26, steps=8)
    g.cyl(PURPLE, (0, 0, 1558), 125, 36, steps=8, top_r=60)
    g.cyl(PURPLE, (0, 0, 1594), 60, 60, steps=8, top_r=8)
    ring_rail(g, PAVE, 164, 1272)
    fig(sc, r, -95, -92, 1272, 30, **wave(r, lo=0.6, hi=0.9, **robe(r)))
    fig(sc, r, 0, -128, 1272, 0, arm_r='face', hold_r=CUP, arm_l='hip', **robe(r))
    fig(sc, r, 95, -92, 1272, -30, **cheer(r, **robe(r)))
    fig(sc, r, -128, 30, 1272, 80, **wave(r, **robe(r)))
    fig(sc, r, 128, 30, 1272, -80, arm_l='hold', arm_r='hold', hold_r=h_tray(WHITE, LILAC, PINK), top=WHITE, bottom=PURPLE)


# ============================================================ s1d8 Reactor Two: the copper quarter (no small orange thing, no glow on orange)
def battered(g, top, y0=-120.0, y1=170.0):
    """A copper plinth with sloping ends and four rust buttress fins, a cream floor on it."""
    g.side(ORANGE, [(-340, -FOOT), (340, -FOOT), (315, top), (-315, top)], y0, y1)
    for x in (-250, -85, 85, 250):
        tilt_box(g, RUST, (x, y0 - 26, 0), (x, y0 + 6, top - 30), 36, 40)
    slab(g, LIT, -310, 310, y0 + 5, y1 - 30, top, top + 6)


def club_terrace(sc):
    """The engineers' club terrace: a drawing on an easel, the tea lady and her urn, the last shift waving at the finish."""
    r = random.Random(801)
    g = sc.still()
    battered(g, D)
    slab(g, ORANGE, -315, 315, 140, 170, D, D + 330)
    for x in (-255, 0, 255):
        g.box(RUST, (x, 155, D + 350), (120, 30, 40))
    g.box(RUST, (0, 155, D + 384), (60, 30, 28))
    g.box(VIOLET, (0, 137, D + 296), (600, 6, 8), glow=0.4)
    for x in (-60, 0, 60):
        g.box(LIT, (x, 137, D + 180), (20, 6, 150), glow=0.5)
    g.box(MIDBLUE, (-190, 112, D + 156), (150, 8, 100))
    for k in range(3):
        g.box(WHITE, (-190, 107, D + 130 + k * 26), (120 - k * 30, 4, 5))
    for x in (-250, -130):
        tilt_box(g, STEEL, (x, 128, D), (x, 114, D + 110), 8, 8)
    g.box(WHITE, (160, 70, D + 76), (90, 50, 8))
    for x in (120, 200):
        g.box(STEEL, (x, 70, D + 40), (6, 46, 70))
    g.cyl(STEEL, (145, 70, D + 80), 22, 56, steps=8)
    for x in (180, 196):
        g.cyl(WHITE, (x, 62 + (x - 180), D + 80), 6, 12, steps=6)
    eng = dict(top=WHITE, bottom=CONC, sleeves='long')
    fig(sc, r, -272, 60, D + 6, 70, detail=0, arm_r='point', swing_r=8, rate=0.6, **eng)
    fig(sc, r, -105, 60, D + 6, -70, detail=0, arm_l='hip', arm_r='hold', hold_r=h_roll(), **eng)
    fig(sc, r, 238, 62, D + 6, -15, top=WHITE, bottom=PAVE, wear='dress', hat='cap', hat_col=WHITE, arm_r='hold', hold_r=CUP, rock=2, rate=0.4)
    fig(sc, r, 60, -40, D + 6, 0, hold_l=CUP, **wave(r, **eng))
    fig(sc, r, -30, -62, D + 6, 5, **cheer(r, top=STEAM, bottom=CONC))
    fig(sc, r, 272, -60, D + 6, -10, hold_l=h_tin(), **wave(r, **SMITH))


def forge_step(sc):
    """Smiths at the anvil (a slow hammer) before a hooded forge with a steady hearth; the last shift walks home waving."""
    r = random.Random(802)
    g = sc.still()
    battered(g, D)
    g.box(RUST, (30, 80, D + 45), (170, 100, 90))
    g.box(LIT, (30, 28, D + 62), (120, 5, 44), glow=0.9)
    for x in (-45, 105):
        g.box(CONC, (x, 80, D + 155), (10, 10, 130))
    g.cyl(CONC, (30, 80, D + 220), 88, 120, steps=6, top_r=30)
    g.box(CONC, (30, 80, D + 450), (44, 44, 220))
    g.box(CONC, (30, 80, D + 570), (70, 70, 20))
    g.box(STEEL, (-170, 0, D + 35), (50, 50, 70))                                   # the anvil
    g.box(STEEL, (-170, 0, D + 80), (112, 40, 20))
    g.cyl(STEEL, (235, 95, D), 40, 60, steps=8)                                     # the quench tub and its steam
    puff(sc, r, 235, 95, D + 130, 28)
    g = sc.still()
    for k in range(3):
        g.box(RUST, (-290 + k * 44, 125, D + 26 + k * 20), (40, 30, 40 + k * 40))   # a stepped parapet
    hammer = h_stick(LAMP, STEEL, 34.0, (12, 26, 14))
    fig(sc, r, -248, 0, D + 6, 90, arm_r='fwd', hold_r=hammer, swing_r=26, axis_r='x', rate=0.7, hat='cap', hat_col=STEEL, **SMITH)
    fig(sc, r, -92, 0, D + 6, -90, arm_l='fwd', hold_l=h_stick(STEEL, STEEL, 30.0, (8, 30, 8)), arm_r='hip', **SMITH)
    fig(sc, r, 150, 20, D + 6, 160, detail=0, arm_l='push', arm_r='push', swing_l=8, swing_r=8, axis_l='x', axis_r='x', rate=0.5, **SMITH)
    fig(sc, r, 250, -62, D + 6, -10, hold_l=h_tin(), **wave(r, top=WHITE, bottom=CONC, **CAP))
    fig(sc, r, 130, -72, D + 6, -4, **cheer(r, top=STEAM, bottom=PAVE_D))
    fig(sc, r, -20, -70, D + 6, 6, wear='dress', top=LILAC, **wave(r))


def choir_bastion(sc):
    """A battered copper bastion with buttress fins and lit slits: the works choir in a row on its parapet 12.6 m up, a conductor."""
    r = random.Random(803)
    g = sc.still()
    g.side(ORANGE, [(-175, -FOOT), (175, -FOOT), (140, 1240), (-140, 1240)], -130, 130)
    for sx in (-1, 1):
        tilt_box(g, RUST, (sx * 95, -146, 0), (sx * 95, -128, 760), 40, 44)
    for z in (380, 800):
        g.box(RUST, (0, -132, z), (310, 6, 22))
    for x in (-50, 0, 50):
        g.box(LIT, (x, -132, 990), (22, 6, 180), glow=0.5)
    g.box(VIOLET, (0, -132, 1190), (270, 6, 8), glow=0.4)
    g.box(RUST, (0, 0, 1250), (480, 340, 20))
    g.box(RUST, (0, -162, 1285), (480, 16, 50))
    for sx in (-1, 1):
        g.box(RUST, (sx * 232, 0, 1285), (16, 340, 50))
        for sy in (-1, 1):
            g.box(RUST, (sx * 215, sy * 145, 1315), (50, 50, 110))
    for x in (-90, 0, 90):
        g.box(RUST, (x, 120, 1300), (50, 50, 80))                                   # a row of short square vents
    g.box(RUST, (188, -90, 1280), (60, 60, 40))
    singer = dict(top=WHITE, bottom=STEAM, band=VIOLET, sleeves='long', arm_l='hold', arm_r='hold', rate=0.5, phase=0.1, rock=3)
    for k in range(5):
        fig(sc, r, -160 + k * 68, -100, 1260, 0, hold_r=h_board(LAMP, LAMP), **singer)
    fig(sc, r, 188, -90, 1300, 100, top=LAMP, bottom=CONC, arm_l='out', arm_r='up', swing_l=12, swing_r=16, rate=0.7)


TALL = dict(grow=(60, 60, 60), grow_down=(60, 60, 0))
JOBS = [
    ('SM_S1PEOPLE_Doorstep', doorstep, TALL), ('SM_S1PEOPLE_ClockOn', clock_on, TALL), ('SM_S1PEOPLE_HoseTower', hose_tower, TALL),
    ('SM_S1PEOPLE_Blackboard', blackboard, TALL), ('SM_S1PEOPLE_FlaskWalk', flask_walk, TALL), ('SM_S1PEOPLE_Observatory', observatory, TALL),
    ('SM_S1PEOPLE_CafeHatch', cafe_hatch, TALL), ('SM_S1PEOPLE_TallyDock', tally_dock, TALL), ('SM_S1PEOPLE_SignalCabin', signal_cabin, TALL),
    ('SM_S1PEOPLE_SkimJetty', skim_jetty, TALL), ('SM_S1PEOPLE_DrumWash', drum_wash, TALL), ('SM_S1PEOPLE_AssayTower', assay_tower, TALL),
    ('SM_S1PEOPLE_MissionSteps', mission_steps, TALL), ('SM_S1PEOPLE_CustomsDesk', customs_desk, TALL), ('SM_S1PEOPLE_HoistLoft', hoist_loft, TALL),
    ('SM_S1PEOPLE_ChessTerrace', chess_terrace, TALL), ('SM_S1PEOPLE_PlungeRim', plunge_rim, TALL), ('SM_S1PEOPLE_Belvedere', belvedere, TALL),
    ('SM_S1PEOPLE_ClubTerrace', club_terrace, TALL), ('SM_S1PEOPLE_ForgeStep', forge_step, TALL), ('SM_S1PEOPLE_ChoirBastion', choir_bastion, TALL),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
