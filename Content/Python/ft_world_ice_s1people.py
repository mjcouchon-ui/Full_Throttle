"""Ice Mountain, Stage 1, builder s1people: the people pass 3 left out. /Game/World/IceMountain/SM_S1PEOPLE_*.
Run: import ft_world_ice_s1people as K; K.run()

Every piece brings its own raised floor (1.9 to 3.2 m as built, footed 2.2 m into the ground: this hill falls 7 in
100) so its people clear the barrier and the cutting of a banked bend; three pieces a district, each doing what the
district does, and one scaffold tower for the half pipes. Built 4 m long and 2.6 m deep at most (the tower 3 m),
placed at 1.3. Nobody in white, nothing thrown, no snowman, no orange, nothing that glows on a moving part.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
FOOT = 220.0
G = 0.08            # a steady glow (lamps, lit windows): every rule of this builder stands 25 m out or more

SNOW = '#f4f9ff'
TEAL, NAVY, CREAM, RED, PANE, LIT = '#19b5c9', '#3d4763', '#f4efe6', '#ff2d3c', '#1d2740', '#ffe28a'
CHAR, YEL, CONC, STEEL, SLATE = '#3b3f52', '#ffd23f', '#aab3c4', '#9aa4b8', '#5a6478'
PINK, WOOD, WOOD_D = '#ff4fb8', '#9a6a44', '#6a4c36'
VIOLET, MINT, BLUE, TEAL_LIT = '#b9a2ff', '#8ff0cf', '#2e6bff', '#2fc6c0'

HAIR = ('#2a2230', '#3b2a22', '#6a4428', '#a8703a', '#e8c26a', '#c94f2e')
BOTS = ('#2b2350', '#3a5fa8', '#5a4a3c', '#22202a', '#7a8fb8', '#c94f6a')
T1 = (TEAL, RED, YEL, PINK, '#8f6bff', TEAL)
T2 = (PINK, TEAL, '#8f6bff', RED)
T3 = (PINK, WOOD, TEAL, YEL, RED, PINK)
T4 = (YEL, RED, '#3a5fa8', YEL, WOOD_D)
T5 = (VIOLET, MINT, '#8f6bff', PINK, YEL)
T6 = (BLUE, PINK, YEL, '#8f6bff', BLUE)
T7 = (WOOD, CHAR, '#3a5fa8', WOOD_D)
T8 = (RED, YEL, CHAR, RED, '#3a5fa8')
KID = dict(build='kid', s=0.68)


# ------------------------------------------------------------------------------------------ helpers
def block(g, col, x, y, z0, z1, w, d, glow=0.0):
    g.box(col, (x, y, (z0 + z1) / 2.0), (w, d, z1 - z0), glow=glow)


def legs(g, col, pts, h, t=16.0):
    for (x, y) in pts:
        block(g, col, x, y, -FOOT, h, t, t)


def rail(g, col, x0, x1, y, z, h=92.0, n=4, t=6.0):
    """A rail along X: n posts, a top bar and a mid bar."""
    for k in range(n):
        block(g, col, x0 + (x1 - x0) * k / float(n - 1), y, z, z + h, t, t)
    g.box(col, ((x0 + x1) / 2.0, y, z + h), (abs(x1 - x0) + t, t, t))
    g.box(col, ((x0 + x1) / 2.0, y, z + h * 0.5), (abs(x1 - x0), 5, 5))


def xbrace(g, col, x0, x1, y, z0, z1, t=8.0):
    tilt_box(g, col, (x0, y, z0), (x1, y, z1), t)
    tilt_box(g, col, (x0, y, z1), (x1, y, z0), t)


def ladder(g, col, x, y, z0, z1, w=44.0, rungs=5):
    for sx in (-1, 1):
        block(g, col, x + sx * w / 2.0, y, z0, z1, 6, 6)
    for k in range(rungs):
        g.box(col, (x, y, z0 + (z1 - z0) * (k + 0.5) / rungs), (w, 5, 5))


def flag(sc, col, x, y, z, h=200.0, w=64.0, fh=40.0, pole=STEEL, rate=0.5, swing=14.0, ph=0.0):
    """A pole with a cloth that swings about the pole."""
    block(sc.still(), pole, x, y, z, z + h, 5, 5)
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z + h - fh / 2.0), axis=P.UP, rate=rate, phase=ph))
    f.box(col, (x + w / 2.0 + 2.0, y, z + h - fh / 2.0), (w, 4, fh))


def bunting(sc, cols, a, b, n, size=26.0, rate=0.7, line=NAVY):
    """A line from a to b hung with n pennants, each rocking about the line."""
    tilt_box(sc.still(), line, a, b, 4)
    ax = math.degrees(math.atan2(b[1] - a[1], b[0] - a[0]))
    for k in range(n):
        t = (k + 0.5) / n
        p = (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t)
        f = sc.part(P.motion(swing=22, pivot=p, axis=ax, rate=rate, phase=(k * 0.37) % 1.0))
        f.box(cols[k % len(cols)], (p[0], p[1], p[2] - size / 2.0), (size * 0.8, 4, size), rot=(0, ax, 0))


def who(sc, r, x, y, z, yaw=0.0, tops=T1, **kw):
    """One of the cast: winter clothes in the district's colours, never white, dark shoes."""
    d = P.look(r, tops=tops, bottoms=BOTS)
    d.update(hair_col=r.choice(HAIR), shoes='#22202a', detail=1, sleeves='long', phase=r.random())
    d.update(kw)
    return P.figure(sc, x, y, z, yaw=yaw, **d)


def h_ski(col):
    def f(m, h, side):
        m.box(col, (h[0] + side * 7, h[1] - 3, h[2] + 62), (8, 5, 170))
    return f


def h_pole(col, length=200.0):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + length / 2.0 - 40.0), (4, 4, length))
    return f


def h_lamp():
    def f(m, h, side):
        m.box(CHAR, (h[0], h[1], h[2] - 12), (12, 12, 6))
        m.box(LIT, (h[0], h[1], h[2] - 24), (10, 10, 16), glow=G)
    return f


def h_bell(col=YEL):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1], h[2] - 24), 13, 20, steps=6, top_r=6)
    return f


def h_horn(col=YEL, length=58.0, r=20.0):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 4, h[2] + 4), 4, length, rot=(-90, -90, 0), steps=6, top_r=r)
    return f


def h_violin():
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 6, h[2] + 6), (12, 34, 8))
    return f


def h_board(col):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 6, h[2] + 10), (30, 5, 22))
    return f


# ------------------------------------------------------------------------------------------ s1d1, First tracks: competition day
def d1_startpod(sc):
    """The start pod on stilts: racers queue along its ring deck with skis and boards, the starter waves them on."""
    r = random.Random(511)
    g = sc.still()
    legs(g, NAVY, ((-180, -100), (180, -100), (-180, 100), (180, 100)), 230)
    xbrace(g, NAVY, -180, 180, 100, 20, 210)
    block(g, CREAM, 0, 0, 230, 244, 400, 250)
    block(g, TEAL, 0, -124, 222, 240, 400, 5)
    rail(g, RED, -196, 196, -122, 244, n=5)
    g.cyl(TEAL, (70, 40, 244), 84, 180, steps=10)                      # the pod
    g.cyl(CREAM, (70, 40, 300), 86, 22, steps=10)
    g.cyl(TEAL, (70, 40, 424), 90, 44, steps=10, top_r=36)
    g.cyl(SNOW, (70, 40, 468), 44, 18, steps=8, top_r=14)
    g.box(RED, (30, -44, 304), (44, 6, 120))                           # the ramp door
    g.box(PANE, (95, -44, 370), (34, 6, 34))
    g.box(LIT, (95, -47, 370), (22, 4, 22), glow=G)
    block(g, STEEL, 120, 70, 468, 590, 5, 5)                           # antenna
    g.box(STEEL, (120, 70, 560), (40, 4, 4))
    g.box(STEEL, (120, 70, 530), (26, 4, 4))
    flag(sc, RED, -186, 110, 244, h=230, ph=0.2)
    flag(sc, TEAL, -186, 60, 244, h=190, ph=0.6)
    who(sc, r, -160, -70, 244, yaw=70, tops=T1, hat='helmet', hat_col=TEAL, arm_l='hold', hold_r=h_ski(PINK), bob=3, rate=0.8)
    who(sc, r, -100, -76, 244, yaw=80, tops=T1, hat='helmet', hat_col=RED, hold_l=P.hold_board(YEL, TEAL, 150), rock=5, rate=0.6)
    who(sc, r, -42, -70, 244, yaw=75, tops=T1, hat='helmet', hat_col=YEL, arm_l='vee', arm_r='vee', bob=6, rate=1.2, react=True)
    who(sc, r, 128, -84, 244, yaw=-50, tops=(RED,), hat='peak', hat_col=NAVY, band=CREAM, arm_r='wave', hold_r=P.hold_flag(RED, STEEL, 50), swing_r=24, rate=0.8)


def d1_coaches(sc):
    """A stepped coaches' terrace: three tiers of coaches and families, bunting over their heads."""
    r = random.Random(512)
    g = sc.still()
    for k, (y, h) in enumerate(((-86, 190), (0, 250), (86, 310))):
        block(g, NAVY, 0, y, -FOOT, h, 400, 88)
        block(g, CREAM, 0, y - 45, h - 30, h - 8, 400, 4)
        block(g, TEAL, 0, y, h, h + 5, 392, 78)
    for sx in (-1, 1):
        block(g, STEEL, sx * 194, 120, 310, 560, 6, 6)
    bunting(sc, (RED, TEAL, CREAM, YEL), (-194, 120, 550), (194, 120, 550), 6)
    flag(sc, RED, -190, -86, 195, h=170, w=54, ph=0.1)
    who(sc, r, -120, -86, 195, tops=T1, hat='beanie', hat_col=RED, arm_l='hold', arm_r='hold', hold_r=P.hold_camera())
    who(sc, r, -40, -86, 195, tops=T1, hat='beanie', hat_col=TEAL, arm_r='wave', swing_r=26, rate=1.1, react=True, **KID)
    who(sc, r, 70, -86, 195, tops=T1, arm_l='up', arm_r='up', bob=5, rate=1.3, react=True)
    who(sc, r, -80, 0, 255, tops=(TEAL,), hat='cap', hat_col=RED, band=CREAM, arm_r='wave', hold_r=P.hold_flag(YEL, STEEL, 56), swing_r=28, rate=0.7)
    who(sc, r, 60, 0, 255, tops=(TEAL,), hat='headset', hat_col=NAVY, band=CREAM, arm_l='face', arm_r='point')
    who(sc, r, -10, 86, 315, tops=T1, arm_r='wave', swing_r=22, rate=1.0, hold_l=P.hold_cup(CREAM), arm_l='hold')


def d1_cafe(sc):
    """The cafe terrace on two drum stems: a hatch under a striped awning, a family at a table, a wave from the rail."""
    r = random.Random(513)
    g = sc.still()
    for x in (-110, 110):
        g.cyl(NAVY, (x, 0, -FOOT), 46, FOOT + 226, steps=8)
    block(g, CREAM, 0, 0, 226, 240, 400, 250)
    block(g, TEAL, 0, -124, 218, 238, 400, 5)
    rail(g, RED, -196, 196, -122, 240, n=5)
    block(g, TEAL, 90, 92, 240, 400, 200, 62)                           # the kiosk
    block(g, CREAM, 90, 92, 300, 322, 204, 66)
    g.box(PANE, (90, 60, 356), (120, 4, 56))
    g.box(LIT, (90, 58, 356), (100, 3, 40), glow=G)
    g.cyl(TEAL, (90, 92, 400), 104, 34, steps=8, top_r=40)
    g.cyl(SNOW, (90, 92, 434), 44, 14, steps=8, top_r=12)
    for k in range(5):                                                  # the awning, red and cream
        tilt_box(g, RED if k % 2 == 0 else CREAM, (10 + k * 40, 62, 400), (10 + k * 40, 6, 376), 40, 5)
    g.cyl(STEEL, (-120, 20, 240), 4, 78, steps=6)                       # a table and its parasol
    g.cyl(CREAM, (-120, 20, 318), 42, 6, steps=8)
    g.cyl(STEEL, (-120, 20, 324), 3, 120, steps=6)
    g.cyl(TEAL, (-120, 20, 430), 84, 30, steps=8, top_r=6)
    who(sc, r, -170, 20, 240, yaw=-60, tops=T1, hat='beanie', hat_col=PINK, arm_r='face', hold_r=P.hold_cup(RED))
    who(sc, r, -76, 30, 240, yaw=50, tops=T1, hat='beanie', hat_col=YEL, arm_l='hold', hold_l=P.hold_cup(TEAL), bob=4, rate=1.0, **KID)
    who(sc, r, 90, 24, 240, yaw=0, tops=(RED,), hat='chef', hat_col=TEAL, arm_l='carry', arm_r='carry', hold_r=P.hold_box(CREAM, 40))
    who(sc, r, -20, -78, 240, tops=T1, arm_r='wave', swing_r=24, rate=1.0, react=True)
    who(sc, r, 160, -78, 240, tops=T1, hat='cap', hat_col=TEAL, arm_l='hold', arm_r='hold', hold_r=P.hold_phone())


# ------------------------------------------------------------------------------------------ s1d2, Black diamond: the avalanche works
def diamond_plate(g, x, y, z):
    block(g, STEEL, x, y, z - 150, z, 6, 6)
    g.box(YEL, (x, y - 4, z + 30), (62, 5, 62), rot=(45, 0, 0))
    g.box(CHAR, (x, y - 8, z + 30), (34, 4, 34), rot=(45, 0, 0))


def d2_bunker(sc):
    """A blunt blast shelter: the avalanche patrol works from its roof with probes and flags, onlookers behind."""
    r = random.Random(521)
    g = sc.still()
    block(g, CONC, 0, 0, -FOOT, 250, 384, 236)
    block(g, CHAR, 0, 0, 250, 268, 400, 252)
    for k in range(8):
        block(g, YEL if k % 2 == 0 else CHAR, -175 + k * 50, -127, 228, 250, 50, 4)
    for x in (-60, 40, 140):
        g.box(PANE, (x, -119, 170), (64, 4, 14))
    g.box(YEL, (-150, -119, 66), (58, 4, 132))
    rail(g, STEEL, -196, 196, -120, 268, n=5)
    diamond_plate(g, 170, 100, 460)
    flag(sc, YEL, -186, 100, 268, h=210, ph=0.4)
    P.figure(sc, -140, -60, 268, yaw=20, top=YEL, band=CHAR, bottom=CHAR, sleeves='long', hat='helmet', hat_col=YEL, detail=1,
             skin=P.SKINS[2], arm_r='hold', arm_l='hold', hold_r=h_pole(STEEL, 230), swing_r=6, axis_r='x', rate=0.5, phase=0.1)
    P.figure(sc, -50, -70, 268, yaw=-10, top=YEL, band=CHAR, bottom=NAVY, sleeves='long', hat='helmet', hat_col=RED, detail=1,
             skin=P.SKINS[4], arm_r='wave', hold_r=P.hold_flag(RED, STEEL, 54), swing_r=26, rate=0.7, phase=0.5)
    P.figure(sc, 50, -66, 268, yaw=-30, top=YEL, band=CHAR, bottom=CHAR, sleeves='long', hat='helmet', hat_col=YEL, detail=1,
             skin=P.SKINS[0], arm_l='face', arm_r='point', pack=CHAR)
    who(sc, r, 120, 30, 268, tops=T2, hat='beanie', hat_col=PINK, arm_l='hold', arm_r='hold', hold_r=P.hold_camera())
    who(sc, r, -20, 40, 268, tops=T2, hat='beanie', hat_col=TEAL, arm_r='wave', swing_r=24, rate=1.1, react=True, **KID)


def d2_gantry(sc):
    """A snow-depth gantry: surveyors on its deck under the hanging gauge, a ladder up the side."""
    r = random.Random(522)
    g = sc.still()
    legs(g, STEEL, ((-180, -90), (180, -90), (-180, 90), (180, 90)), 300, 18)
    xbrace(g, SLATE, -180, 0, -90, 30, 280)
    xbrace(g, SLATE, 0, 180, -90, 30, 280)
    block(g, SLATE, 0, 0, 300, 314, 400, 220)
    for k in range(8):
        block(g, YEL if k % 2 == 0 else CHAR, -175 + k * 50, -111, 292, 314, 50, 4)
    rail(g, STEEL, -196, 196, -106, 314, n=5)
    ladder(g, STEEL, 120, 114, -60, 314, rungs=6)
    for sx in (-1, 1):
        block(g, STEEL, sx * 186, 80, 314, 600, 10, 10)
    g.box(STEEL, (0, 80, 600), (384, 12, 12))
    sw = sc.part(P.motion(swing=5, pivot=(0, 80, 594), axis=0.0, rate=0.3, phase=0.2))   # the gauge hangs and stirs
    sw.box(STEEL, (0, 80, 540), (5, 5, 110))
    sw.box(YEL, (0, 80, 462), (50, 6, 50))
    sw.box(CHAR, (0, 76, 462), (50, 3, 10))
    P.figure(sc, -130, -40, 314, yaw=15, top=YEL, band=CHAR, bottom=CHAR, sleeves='long', hat='hard', hat_col=YEL, detail=1,
             skin=P.SKINS[1], arm_l='face', arm_r='face', phase=0.3)
    P.figure(sc, -40, -30, 314, yaw=-20, top=YEL, band=CHAR, bottom=NAVY, sleeves='long', hat='hard', hat_col=YEL, detail=1,
             skin=P.SKINS[3], arm_l='hold', arm_r='hold', hold_r=h_board(CREAM), hair='bun', hair_col=HAIR[1])
    P.figure(sc, 60, -44, 314, yaw=-35, top=YEL, band=CHAR, bottom=CHAR, sleeves='long', hat='hard', hat_col=RED, detail=1,
             skin=P.SKINS[5], arm_r='point', swing_r=8, rate=0.5, phase=0.7)
    who(sc, r, 122, -40, 314, tops=(YEL,), band=CHAR, hat='hard', hat_col=YEL, arm_r='wave', hold_r=P.hold_flag(YEL, STEEL, 50), swing_r=24, rate=0.8)


def d2_experts(sc):
    """Above the drop: experts strap in on a concrete buttress, boards racked behind, one stretches, one waits."""
    r = random.Random(523)
    g = sc.still()
    block(g, CONC, 0, -40, -FOOT, 210, 400, 180)
    block(g, CONC, 0, 85, -FOOT, 300, 400, 90)
    block(g, YEL, 0, -129, 190, 210, 400, 4)
    block(g, YEL, 0, 39, 282, 300, 400, 4)
    for x in (-190, -95, 0, 95, 190):                                   # marker bollards, yellow and black
        block(g, YEL, x, -116, 210, 262, 14, 14)
        block(g, CHAR, x, -116, 232, 246, 16, 16)
    for k, c in enumerate((PINK, TEAL, YEL, RED)):                      # boards in a rack
        g.box(c, (90 + k * 26, 110, 372), (20, 6, 145))
    g.box(STEEL, (128, 118, 350), (120, 6, 6))
    diamond_plate(g, -170, 110, 480)
    g.box(PINK, (-110, -70, 216), (150, 30, 8))                         # a board at the sitter's feet
    who(sc, r, -110, 50, 210, tops=T2, hat='helmet', hat_col=PINK, legs='sit', seat=90, arm_l=((30, 60), (20, 80)), arm_r=((30, 60), (20, 80)),
        swing_r=10, axis_r='x', rate=0.9)
    who(sc, r, -10, -50, 210, yaw=-15, tops=T2, hat='helmet', hat_col=TEAL, hold_r=P.hold_board(TEAL, YEL, 150))
    who(sc, r, 80, -60, 210, yaw=10, tops=T2, hat='helmet', hat_col=YEL, arm_l='out', arm_r='out', rock=9, rate=0.6)
    who(sc, r, 160, -50, 210, yaw=-25, tops=T2, hat='helmet', hat_col=RED, arm_l='face', arm_r='hip')
    who(sc, r, -30, 86, 300, tops=T2, hat='beanie', hat_col=YEL, arm_l='up', arm_r='up', bob=5, rate=1.2, react=True)


# ------------------------------------------------------------------------------------------ s1d3, The valley run: market day
def d3_bandstand(sc):
    """The town band on its bandstand: tuba, drum, horn and conductor under a pink shingle cap; children on the steps."""
    r = random.Random(531)
    g = sc.still()
    g.cyl(CREAM, (-60, 0, -FOOT), 124, FOOT + 200, steps=8)
    g.cyl(WOOD, (-60, 0, 200), 130, 10, steps=8)
    for k in range(6):
        a = math.radians(30 + k * 60)
        block(g, WOOD_D, -60 + 116 * math.cos(a), 116 * math.sin(a), 210, 430, 10, 10)
    g.cyl(WOOD_D, (-60, 0, 426), 128, 10, steps=8)
    g.cyl(PINK, (-60, 0, 436), 138, 96, steps=8, top_r=22)
    g.cyl(SNOW, (-60, 0, 500), 62, 44, steps=8, top_r=8)
    g.cyl(YEL, (-60, 0, 544), 5, 34, steps=6)
    for x in (-118, -60, -2):
        g.box(PINK, (x, -118, 250), (46, 6, 44))                        # the balustrade's panels
        g.box(CREAM, (x, -122, 250), (18, 4, 18), rot=(45, 0, 0))
    block(g, CREAM, 135, 0, -FOOT, 110, 130, 200)                       # the steps
    block(g, CREAM, 135, 40, 110, 170, 130, 120)
    g.box(PINK, (135, 30, 180), (90, 34, 10))                           # a sled
    g.box(WOOD_D, (135, 30, 173), (96, 6, 6))
    who(sc, r, -120, -30, 210, yaw=25, tops=(PINK,), hat='peak', hat_col=WOOD_D, s=0.95, arm_l='hold', arm_r='hold', hold_r=h_horn(YEL, 70, 30), rock=3, rate=0.8)
    who(sc, r, -20, 40, 210, yaw=-10, tops=(PINK,), hat='peak', hat_col=WOOD_D, s=0.95, arm_l='hold', arm_r='hold', hold_r=h_horn(YEL, 46, 15), rock=3, rate=0.8)
    d = who(sc, r, -90, 50, 210, yaw=10, tops=(PINK,), hat='peak', hat_col=WOOD_D, s=0.95, arm_l='carry', arm_r='carry', swing_r=22, axis_r='x', swing_l=-22, axis_l='x', rate=1.4)
    g = sc.still()
    g.cyl(CREAM, (-90, 14, 300), 26, 22, steps=8)                       # the drum on its stand
    g.cyl(PINK, (-90, 14, 296), 28, 6, steps=8)
    block(g, WOOD_D, -90, 14, 210, 296, 6, 6)
    who(sc, r, -30, -70, 210, yaw=170, tops=(WOOD_D,), band=PINK, s=0.95, arm_r='wave', hold_r=h_pole(CREAM, 60), swing_r=30, rate=1.4, arm_l='out', swing_l=14)
    who(sc, r, 112, 30, 166, tops=T3, hat='beanie', hat_col=PINK, legs='sit', seat=20, arm_r='wave', swing_r=26, rate=1.1, react=True, detail=0, **KID)
    who(sc, r, 160, 30, 166, tops=T3, hat='beanie', hat_col=TEAL, legs='sit', seat=20, arm_l='up', arm_r='up', detail=0, **KID)


def judge_box(g, x, col):
    legs(g, WOOD_D, ((x - 62, -80), (x + 62, -80), (x - 62, 80), (x + 62, 80)), 240, 12)
    xbrace(g, WOOD_D, x - 62, x + 62, -80, 30, 220, 7)
    block(g, WOOD, x, 0, 240, 252, 150, 184)
    block(g, WOOD, x, -88, 252, 318, 150, 8)
    g.box(col, (x, -93, 285), (60, 4, 44))
    g.box(CREAM, (x, -96, 285), (20, 3, 20), rot=(45, 0, 0))
    for sx in (-1, 1):
        block(g, WOOD_D, x + sx * 70, -84, 252, 450, 9, 9)
        block(g, WOOD_D, x + sx * 70, 84, 252, 450, 9, 9)
        tilt_box(g, col, (x + sx * 84, 0, 446), (x, 0, 500), 200, 8)   # the gable roof, ridge front to back
        tilt_box(g, SNOW, (x + sx * 50, 0, 478), (x, 0, 510), 150, 8)


def d3_judges(sc):
    """Gate judges in two roofed boxes on legs, a plank bridge between them where the bell ringer stands."""
    r = random.Random(532)
    g = sc.still()
    judge_box(g, -124, PINK)
    judge_box(g, 124, PINK)
    block(g, WOOD, 0, 0, 240, 250, 100, 70)
    rail(g, WOOD_D, -48, 48, -34, 250, h=80, n=3, t=6)
    bunting(sc, (PINK, CREAM), (-54, -84, 440), (54, -84, 440), 5, size=22)
    who(sc, r, -124, -30, 252, tops=(CREAM, WOOD), band=PINK, hat='peak', hat_col=PINK, arm_r='up', hold_r=P.hold_flag(RED, WOOD_D, 46), swing_r=18, rate=0.6)
    who(sc, r, 124, -30, 252, tops=(CREAM, WOOD), band=PINK, hat='peak', hat_col=PINK, arm_l='face', arm_r='out', hold_r=P.hold_flag(YEL, WOOD_D, 46), swing_r=14, rate=0.5)
    who(sc, r, -18, 0, 250, tops=T3, hat='cowboy', hat_col=WOOD_D, arm_r='wave', hold_r=h_bell(YEL), swing_r=34, rate=1.2, arm_l='hip')
    who(sc, r, 26, 6, 250, tops=T3, hat='beanie', hat_col=PINK, arm_l='up', arm_r='up', bob=5, rate=1.3, react=True, **KID)


def d3_market(sc):
    """A market stall on an arcaded terrace: the shopkeeper under a pink awning, a customer, washing on the line."""
    r = random.Random(533)
    g = sc.still()
    block(g, CREAM, 0, 0, -FOOT, 226, 400, 250)
    block(g, WOOD_D, 0, 0, 226, 240, 400, 256)
    for x in (-130, 0, 130):                                            # three arches
        g.box(PANE, (x, -124, 84), (78, 5, 168))
        g.cyl(PANE, (x, -122, 168), 39, 5, rot=(-90, -90, 0), steps=8)
        g.box(PINK, (x, -127, 214), (16, 4, 22))
    block(g, WOOD, -100, 70, 240, 330, 170, 46)                         # the stall
    for k, c in enumerate((WOOD_D, PINK, YEL, WOOD_D)):
        g.box(c, (-160 + k * 40, 66, 340), (28, 30, 16))
    for sx in (-1, 1):
        block(g, WOOD_D, -100 + sx * 88, 100, 240, 480, 8, 8)
    for k in range(6):
        tilt_box(g, PINK if k % 2 == 0 else CREAM, (-175 + k * 30, 104, 482), (-175 + k * 30, 30, 446), 30, 5)
    for x in (30, 190):                                                 # the washing line
        block(g, WOOD_D, x, 100, 240, 450, 7, 7)
    bunting(sc, (PINK, CREAM, TEAL, YEL), (30, 100, 444), (190, 100, 444), 4, size=44, rate=0.5, line=WOOD_D)
    rail(g, WOOD_D, 20, 196, -120, 240, h=84, n=3)
    who(sc, r, -100, 110, 240, tops=(PINK,), band=CREAM, hat='chef', hat_col=PINK, arm_l='carry', arm_r='wave', swing_r=20, rate=0.9)
    who(sc, r, -110, -20, 240, yaw=175, tops=T3, hat='beanie', hat_col=TEAL, arm_r='point', hold_l=P.hold_bag(PINK))
    who(sc, r, 60, -70, 240, yaw=-15, tops=T3, hat='beanie', hat_col=YEL, hold_r=P.hold_board(PINK, CREAM, 110), arm_l='wave', swing_l=24, rate=1.1, react=True, **KID)
    who(sc, r, 120, 60, 240, yaw=180, tops=T3, hair='bun', arm_l='up', arm_r='up', swing_r=8, axis_r='x', rate=0.5)
    who(sc, r, 160, -70, 240, tops=T3, hat='sun', hat_col=PINK, arm_l='hold', hold_l=P.hold_cup(CREAM), arm_r='hip')


# ------------------------------------------------------------------------------------------ s1d4, The Abominable's glacier: the expedition
def crate(g, col, x, y, z0, z1, w, d, strap=NAVY):
    block(g, col, x, y, z0, z1, w, d)
    block(g, strap, x, y - d / 2.0 - 1.5, z0 + (z1 - z0) * 0.45, z0 + (z1 - z0) * 0.55, w + 2, 3)


def route_flag(sc, x, y, z, h=150.0, ph=0.0):
    flag(sc, RED, x, y, z, h=h, w=36, fh=26, pole=WOOD, rate=0.7, swing=18, ph=ph)


def d4_haul(sc):
    """A depot of crates stacked as a stage: two explorers heave a loaded sledge along it, the leader points the way."""
    r = random.Random(541)
    g = sc.still()
    crate(g, WOOD, -134, -30, -FOOT, 200, 130, 200)
    crate(g, NAVY, 0, -30, -FOOT, 200, 134, 200, strap=YEL)
    crate(g, WOOD_D, 134, -30, -FOOT, 200, 130, 200)
    crate(g, WOOD_D, -100, 98, -FOOT, 300, 198, 60, strap=YEL)
    crate(g, WOOD, 100, 98, -FOOT, 300, 198, 60)
    for sy in (-1, 1):                                                  # the sledge
        g.box(STEEL, (-130, -60 + sy * 22, 206), (120, 6, 8))
    g.box(WOOD_D, (-130, -60, 216), (104, 50, 10))
    g.box(YEL, (-134, -60, 244), (84, 44, 46))
    g.box(NAVY, (-134, -83, 244), (8, 3, 48))
    g.box(NAVY, (-110, -83, 244), (8, 3, 48))
    route_flag(sc, -192, 104, 300, ph=0.1)
    route_flag(sc, 4, 104, 300, h=130, ph=0.5)
    route_flag(sc, -190, -120, 200, h=120, ph=0.8)
    heave = dict(rock=5, rate=0.7)
    who(sc, r, 0, -60, 200, yaw=90, tops=T4, hat='helmet', hat_col=YEL, arm_l='back', arm_r='back', pack=NAVY, phase=0.2, **heave)
    who(sc, r, 90, -60, 200, yaw=90, tops=T4, hat='helmet', hat_col=RED, arm_l='back', arm_r='back', phase=0.2, **heave)
    tilt_box(sc.still(), WOOD, (-88, -60, 250), (80, -60, 290), 4)       # the trace
    who(sc, r, 60, 96, 300, yaw=-40, tops=(YEL,), hat='peak', hat_col=NAVY, band=NAVY, arm_r='point', arm_l='hip')
    who(sc, r, 122, 96, 300, tops=T4, hat='cap', hat_col=YEL, arm_r='wave', hold_r=P.hold_flag(RED, WOOD, 44), swing_r=22, rate=0.8)


def d4_mess(sc):
    """The mess tent on a timber deck: the cook ladles from the pot, two explorers sit on crates with their mugs."""
    r = random.Random(542)
    g = sc.still()
    legs(g, WOOD_D, ((-180, -100), (0, -100), (180, -100), (-180, 100), (0, 100), (180, 100)), 196, 18)
    g.box(STEEL, (0, -100, 20), (396, 10, 14))                          # sled runners
    g.box(STEEL, (0, 100, 20), (396, 10, 14))
    block(g, WOOD, 0, 0, 196, 210, 400, 250)
    g.side(YEL, [(-100, 210), (100, 210), (0, 350)], -10, 122, loc=(-90, 0, 0))     # the ridge tent, its end to the road
    g.side(NAVY, [(-34, 210), (34, 210), (0, 300)], -13, -10, loc=(-90, 0, 0))
    block(g, STEEL, -30, 60, 300, 420, 9, 9)                            # stove pipe
    g.box(STEEL, (-30, 60, 424), (22, 22, 6))
    g.cyl(STEEL, (70, 30, 210), 22, 56, steps=8)                        # the stove and the pot
    g.cyl(NAVY, (70, 30, 266), 26, 24, steps=8)
    crate(g, WOOD_D, 150, -60, 210, 256, 50, 44, strap=YEL)
    crate(g, NAVY, 30, -70, 210, 256, 50, 44, strap=YEL)
    block(g, STEEL, 186, 110, 210, 520, 5, 5)                           # the radio mast and its stays
    tilt_box(g, STEEL, (186, 110, 500), (120, 110, 214), 4)
    tilt_box(g, STEEL, (186, 110, 500), (186, 20, 214), 4)
    route_flag(sc, 186, 110, 420, h=110, ph=0.3)
    who(sc, r, 80, 76, 210, yaw=0, tops=(YEL,), hat='chef', hat_col=YEL, band=NAVY, arm_r='fwd', hold_r=P.hold_tool(STEEL, WOOD_D, 30), swing_r=14, axis_r='x', rate=0.6, arm_l='hip')
    who(sc, r, 150, -52, 210, yaw=-20, tops=T4, hat='cap', hat_col=RED, legs='sit', seat=46, arm_r='face', hold_r=P.hold_cup(YEL))
    who(sc, r, 30, -62, 210, yaw=15, tops=T4, hat='cap', hat_col=NAVY, legs='sit', seat=46, arm_l='hold', hold_l=P.hold_cup(RED), arm_r='wave', swing_r=20, rate=0.9, react=True)
    who(sc, r, -150, -70, 210, yaw=10, tops=T4, hat='helmet', hat_col=YEL, arm_l='hold', arm_r='hold', hold_r=P.hold_cup(NAVY), pack=WOOD_D)


def d4_photo(sc):
    """The expedition's photographer on a timber scaffold: the big camera on its tripod, the cloth, a pose, a signal."""
    r = random.Random(543)
    g = sc.still()
    legs(g, WOOD_D, ((-180, -100), (180, -100), (-180, 100), (180, 100), (0, -100), (0, 100)), 270, 16)
    xbrace(g, WOOD, -180, 0, -100, 30, 250)
    xbrace(g, WOOD, 0, 180, -100, 30, 250)
    block(g, WOOD, 0, 0, 270, 284, 400, 240)
    ladder(g, WOOD_D, -150, 124, -60, 284, rungs=6)
    for a, b in (((-110, -50), (-80, -20)), ((-50, -50), (-80, -20)), ((-80, 10), (-80, -20))):     # the tripod
        tilt_box(g, WOOD_D, (a[0], a[1], 284), (b[0], b[1], 400), 5)
    g.box(NAVY, (-80, -24, 420), (46, 60, 42))
    g.cyl(STEEL, (-80, -54, 420), 12, 18, rot=(-90, -90, 0), steps=6)
    g.cyl(YEL, (-80, 10, 284), 3, 250, steps=6)                         # the yellow umbrella over the camera
    g.cyl(YEL, (-80, 10, 520), 90, 26, steps=8, top_r=8)
    crate(g, WOOD_D, 110, -40, 284, 334, 70, 60, strap=YEL)
    route_flag(sc, 190, 110, 284, h=170, ph=0.2)
    route_flag(sc, 190, -110, 284, h=130, ph=0.7)
    who(sc, r, -80, 40, 284, yaw=0, tops=(NAVY,), hat='cap', hat_col=NAVY, arm_l='fwd', arm_r='fwd')
    g = sc.still()
    g.box(NAVY, (-80, 22, 434), (56, 60, 40))                           # the cloth over head and camera back
    who(sc, r, 110, -40, 334, yaw=-20, tops=T4, hat='helmet', hat_col=YEL, arm_l='hip', arm_r='hip', pack=NAVY)
    who(sc, r, 40, -60, 284, yaw=-30, tops=T4, hat='cap', hat_col=RED, arm_r='up', hold_r=P.hold_flag(RED, WOOD, 44), swing_r=16, rate=0.6)
    who(sc, r, 160, 40, 284, tops=T4, hat='helmet', hat_col=RED, arm_r='wave', swing_r=24, rate=1.0, react=True)


# ------------------------------------------------------------------------------------------ s1d5, The white ridge: the aurora watch
def mint_lamp(g, x, y, z):
    for k, (w, h) in enumerate(((46, 22), (36, 20), (26, 18))):
        block(g, SLATE, x, y, z + k * 20, z + k * 20 + h, w, w)
    block(g, MINT, x, y, z + 60, z + 80, 16, 16, glow=G)


def d5_skywatch(sc):
    """Sky-watchers on a stone drum: one at the telescope as it pans, one points, one reads the chart, one sits."""
    r = random.Random(551)
    g = sc.still()
    g.cyl(SLATE, (-60, 0, -FOOT), 128, FOOT + 230, steps=8)
    g.cyl(VIOLET, (-60, 0, 230), 134, 12, steps=8)
    block(g, SLATE, -60, 10, 242, 330, 30, 30)                          # the pier
    t = sc.part(P.motion(swing=10, pivot=(-60, 10, 340), axis=P.UP, rate=0.12, phase=0.1))
    rod(t, STEEL, (-60, 46, 318), (-60, -70, 420), 13, steps=6)
    rod(t, MINT, (-60, -60, 411), (-60, -84, 432), 16, steps=6)
    block(g, SLATE, 135, 10, -FOOT, 170, 130, 220)                      # the step and its wind screen
    for sx in (-1, 1):
        block(g, NAVY, 135 + sx * 60, 110, 170, 380, 7, 7)
    tilt_box(g, VIOLET, (135, 112, 384), (135, 60, 352), 130, 5)
    g.box(VIOLET, (135, 114, 290), (124, 4, 150))
    g.box(MINT, (135, 111, 290), (124, 3, 18))
    mint_lamp(g, 180, -80, 170)
    block(g, SLATE, 130, 46, 170, 190, 60, 40)                          # a stone seat
    who(sc, r, -60, 74, 242, yaw=0, tops=T5, hat='cap', hat_col=VIOLET, arm_l='face', arm_r='face')
    who(sc, r, -140, -30, 242, yaw=20, tops=T5, hat='cap', hat_col=MINT, arm_r='up', swing_r=8, rate=0.4, arm_l='hip')
    who(sc, r, 20, -50, 242, yaw=-25, tops=T5, hair='long', arm_l='hold', arm_r='hold', hold_r=h_board(MINT))
    who(sc, r, 130, 40, 170, tops=T5, hat='cap', hat_col=VIOLET, legs='sit', seat=20, arm_r='face', hold_r=P.hold_cup(VIOLET))
    who(sc, r, 110, -60, 170, tops=T5, arm_l='point', arm_r='wave', swing_r=22, rate=1.0, react=True, **KID)


def d5_kites(sc):
    """The kite deck on its one leg: flyers sort rolled kites and reel a spool under a line of streamers."""
    r = random.Random(552)
    g = sc.still()
    g.cyl(SLATE, (0, 0, -FOOT), 44, FOOT + 250, steps=8)
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, NAVY, (sx * 20, sy * 20, 90), (sx * 170, sy * 96, 250), 10)
    block(g, NAVY, 0, 0, 250, 264, 400, 240)
    rail(g, MINT, -196, 196, -116, 264, n=5)
    ladder(g, NAVY, -150, 124, -60, 264, rungs=6)
    g.box(SLATE, (140, 96, 300), (110, 8, 8))                           # the rack of rolled kites
    g.box(SLATE, (140, 96, 380), (110, 8, 8))
    for k, c in enumerate((VIOLET, MINT, PINK, VIOLET)):
        rod(g, c, (100 + k * 27, 90, 268), (100 + k * 27, 104, 430), 9, steps=6)
    for sx in (-1, 1):
        block(g, STEEL, sx * 190, 112, 264, 560, 6, 6)
    bunting(sc, (VIOLET, MINT), (-190, 112, 552), (190, 112, 552), 8, size=40, rate=0.9)
    who(sc, r, -130, -30, 264, yaw=15, tops=T5, hat='cap', hat_col=VIOLET, arm_l='hold', arm_r='hold')
    g = sc.still()
    g.box(VIOLET, (-130, -62, 372), (56, 4, 56), rot=(45, 0, 0))        # a kite held at the chest
    g.box(MINT, (-130, -65, 372), (56, 3, 8))
    who(sc, r, -30, -40, 264, yaw=-10, tops=T5, hat='helmet', hat_col=MINT, arm_l='hold', arm_r='hold', swing_r=18, axis_r='x', rate=1.3, hold_l=P.hold_box(VIOLET, 20))
    who(sc, r, 60, -50, 264, yaw=-30, tops=T5, hair='pony', arm_r='up', arm_l='face')
    who(sc, r, 140, -30, 264, tops=T5, hat='cap', hat_col=PINK, arm_l='up', arm_r='up', bob=5, rate=1.2, react=True, **KID)


def d5_cairn(sc):
    """Hikers on a great stepped cairn: one plants the violet flag on top, one rests, one photographs, a mint lamp."""
    r = random.Random(553)
    g = sc.still()
    block(g, SLATE, 0, 0, -FOOT, 160, 400, 250)
    block(g, '#6a7488', -40, 30, 160, 250, 270, 180)
    block(g, SLATE, -60, 60, 250, 330, 140, 110)
    for (x, y, z, w) in ((150, -90, 160, 50), (-180, -100, 160, 34), (70, 100, 250, 40), (-150, 80, 250, 30)):
        block(g, '#6a7488', x, y, z, z + w * 0.6, w, w * 0.8)
    for (x, y, z) in ((120, 40, 160), (-190, 20, 160), (60, -40, 250)):   # heather
        block(g, VIOLET, x, y, z, z + 12, 30, 24)
    mint_lamp(g, 170, 90, 160)
    who(sc, r, -60, 60, 330, yaw=-10, tops=T5, hat='cap', hat_col=VIOLET, pack=NAVY, arm_r='up', hold_r=P.hold_flag(VIOLET, STEEL, 60), swing_r=10, rate=0.5, arm_l='hip')
    who(sc, r, -130, -20, 250, yaw=20, tops=T5, hat='helmet', hat_col=MINT, pack=VIOLET, arm_r='point', hold_l=h_pole(STEEL, 130))
    who(sc, r, 40, -48, 160, tops=T5, hair='bun', legs='sit', seat=90, arm_l='hold', hold_l=P.hold_cup(MINT), arm_r='wave', swing_r=20, rate=0.9, react=True)
    who(sc, r, 150, -40, 160, yaw=-20, tops=T5, hat='cap', hat_col=PINK, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), pack=MINT)
    who(sc, r, -150, -80, 160, yaw=10, tops=T5, hat='cap', hat_col=MINT, pack=PINK, arm_l='up', arm_r='up', bob=4, rate=1.1, react=True)


# ------------------------------------------------------------------------------------------ s1d6, The resort: the grand spa
def d6_quartet(sc):
    """A string quartet on a colonnaded terrace: four seated players bow in time under a blue cornice."""
    r = random.Random(561)
    g = sc.still()
    block(g, WOOD, 0, 0, -FOOT, 70, 400, 250)
    block(g, CREAM, 0, 0, 70, 220, 392, 244)
    block(g, BLUE, 0, -124, 196, 220, 400, 4)
    block(g, CREAM, 0, 122, 220, 470, 392, 6)                           # the back wall and its lit windows
    for x in (-110, 0, 110):
        g.box(PANE, (x, 117, 340), (56, 4, 150))
        g.box(LIT, (x, 114, 340), (40, 3, 130), glow=G)
    for x in (-180, -60, 60, 180):
        g.cyl(CREAM, (x, 90, 220), 12, 250, steps=6)
    block(g, BLUE, 0, 96, 470, 500, 400, 70)
    block(g, BLUE, 0, 96, 500, 530, 340, 50)
    for x in (-130, 0, 130):                                            # balustrade and flower boxes
        block(g, CREAM, x, -118, 220, 262, 110, 10)
        block(g, WOOD, x, -118, 262, 274, 70, 14)
        block(g, PINK, x, -118, 274, 284, 62, 12)
    suit = dict(tops=(NAVY, CHAR), band=BLUE, legs='sit', seat=46, s=0.98)
    for k, x in enumerate((-135, -45, 45)):
        who(sc, r, x, 10 + (12 if k == 1 else 0), 220, yaw=(-12, 0, 10)[k], arm_l='face', hold_l=h_violin(), arm_r='hold',
            swing_r=14, axis_r='z', rate=1.0, phase=0.25 * k, **suit)
        g = sc.still()
        block(g, WOOD_D, x, 16 + (12 if k == 1 else 0), 220, 264, 34, 34)       # a chair
        block(g, STEEL, x + 30, -40, 220, 300, 4, 4)                    # music stands
        g.box(NAVY, (x + 30, -42, 312), (30, 4, 24))
    who(sc, r, 135, 10, 220, yaw=14, arm_l=((40, 40), (100, 80)), arm_r='carry', swing_r=16, axis_r='z', rate=0.8, phase=0.6, **suit)
    g = sc.still()
    block(g, WOOD_D, 135, 16, 220, 264, 34, 34)
    g.box(WOOD_D, (135, -34, 280), (34, 16, 100))                       # the cello
    block(g, WOOD_D, 135, -34, 330, 372, 6, 6)


def d6_baths(sc):
    """Guests in robes hurry up the grand stair to the baths; a porter with a trunk waits under the blue awning."""
    r = random.Random(562)
    g = sc.still()
    block(g, WOOD, 0, 0, -FOOT, 90, 400, 250)
    for k in range(6):                                                  # the stair climbs to the right
        block(g, CREAM, -175 + k * 50, 0, 90, 150 + k * 30, 50, 244)
        block(g, BLUE, -175 + k * 50, -123, 140 + k * 30, 150 + k * 30, 50, 3)
    block(g, CREAM, 150, 0, 90, 300, 100, 244)
    tilt_box(g, CHAR, (-198, -118, 240), (100, -118, 390), 6)           # the iron handrail
    for k in range(4):
        block(g, CHAR, -175 + k * 90, -118, 150 + k * 54, 236 + k * 46, 5, 5)
    g.box(CREAM, (150, 119, 400), (96, 6, 200))                         # the doorway
    g.box(PANE, (150, 115, 380), (56, 4, 160))
    g.box(LIT, (150, 112, 440), (56, 3, 30), glow=G)
    for sx in (-1, 1):
        block(g, CHAR, 150 + sx * 46, -100, 300, 480, 5, 5)
    for k in range(5):
        tilt_box(g, BLUE if k % 2 == 0 else CREAM, (110 + k * 20, 116, 510), (110 + k * 20, -104, 476), 20, 5)
    robe = dict(wear='coat', hair='bun', arm_l='hold', stride=18, rate=1.3, bob=3)
    who(sc, r, -150, -10, 180, yaw=70, tops=(PINK,), hold_r=P.hold_bag(YEL), **robe)
    who(sc, r, -50, 30, 240, yaw=75, tops=(YEL,), band=BLUE, **robe)
    who(sc, r, 40, -30, 270, yaw=60, tops=('#8f6bff',), hold_r=P.hold_bag(PINK), **robe)
    who(sc, r, 130, -20, 300, yaw=-20, tops=(BLUE,), hat='peak', hat_col=BLUE, band=YEL, arm_l='carry', arm_r='carry', hold_r=P.hold_box(WOOD_D, 50))
    who(sc, r, 176, 50, 300, yaw=-10, tops=(MINT,), wear='coat', arm_r='wave', swing_r=22, rate=0.9, react=True, **KID)


def d6_gallery(sc):
    """An iron balcony under a striped awning: spectators lean on the rail over the flower boxes, opera glasses out."""
    r = random.Random(563)
    g = sc.still()
    block(g, WOOD, 0, 66, -FOOT, 60, 400, 120)
    block(g, CREAM, 0, 66, 60, 500, 392, 114)
    for x in (-130, 0, 130):
        g.box(PANE, (x, 7, 340), (64, 4, 170))
        g.box(LIT, (x, 4, 372), (50, 3, 90), glow=G)
        g.box(PINK, (x, -116, 358), (70, 14, 12))                       # flower boxes on the rail
        g.box(WOOD, (x, -116, 346), (76, 16, 12))
    block(g, CHAR, 0, -56, 238, 250, 400, 130)                          # the balcony and its brackets
    for x in (-180, -60, 60, 180):
        tilt_box(g, CHAR, (x, 6, 120), (x, -110, 238), 8)
    rail(g, CHAR, -196, 196, -116, 250, h=90, n=7, t=5)
    block(g, BLUE, 0, 66, 500, 540, 400, 124)
    for k in range(8):
        tilt_box(g, BLUE if k % 2 == 0 else CREAM, (-175 + k * 50, 10, 470), (-175 + k * 50, -122, 432), 50, 5)
    lean = ((42, 70), (70, 88))
    who(sc, r, -150, -70, 250, tops=T6, hat='sun', hat_col=PINK, wear='coat', arm_l=lean, arm_r=lean)
    who(sc, r, -70, -66, 250, tops=T6, arm_l='face', arm_r='face', hat='cowboy', hat_col=CHAR)
    who(sc, r, 10, -70, 250, tops=T6, arm_l='hold', arm_r='hold', swing_r=10, axis_r='z', swing_l=-10, axis_l='z', rate=1.5, react=True)
    who(sc, r, 90, -62, 250, tops=T6, arm_r='wave', hold_r=P.hold_flag(BLUE, CHAR, 40), swing_r=24, rate=1.0, **KID)
    who(sc, r, 160, -70, 250, tops=T6, hair='long', wear='coat', arm_l=lean, arm_r='wave', swing_r=18, rate=0.8)


# ------------------------------------------------------------------------------------------ s1d7, The ice again: the crystal mine
def crystal(g, x, y, z, h, r, lean=(0, 0)):
    g.cyl(TEAL_LIT, (x, y, z), r, h * 0.7, steps=5, rot=(lean[0], lean[1], 0), glow=G)
    g.cyl(TEAL_LIT, (x - lean[0] * h * 0.012, y, z + h * 0.68), r, h * 0.3, steps=5, top_r=2, rot=(lean[0], lean[1], 0), glow=G)


def miner(sc, r, x, y, z, yaw=0.0, **kw):
    kw.setdefault('hat_col', YEL)
    top = who(sc, r, x, y, z, yaw=yaw, tops=T7, hat='hard', band=TEAL_LIT, **kw)
    return top


def d7_staging(sc):
    """Miners on a braced timber staging at a seam of teal crystal: a pick swings, a hammer taps, a lantern is held up."""
    r = random.Random(571)
    g = sc.still()
    legs(g, WOOD_D, ((-180, -100), (0, -100), (180, -100), (-180, 100), (0, 100), (180, 100)), 256, 16)
    xbrace(g, WOOD_D, -180, 0, -100, 20, 240)
    xbrace(g, WOOD_D, 0, 180, -100, 20, 240)
    block(g, WOOD, 0, 0, 256, 270, 400, 244)
    block(g, CHAR, 20, 100, 270, 470, 270, 50)                          # the rock and its seam
    block(g, '#5a6478', -60, 96, 270, 400, 120, 56)
    crystal(g, -20, 70, 330, 150, 20, (-14, 0))
    crystal(g, 60, 72, 300, 190, 24, (10, 0))
    crystal(g, 120, 74, 310, 120, 16, (24, 0))
    block(g, WOOD_D, -186, 110, 270, 470, 9, 9)                         # a timber lamp post
    g.box(WOOD_D, (-170, 110, 462), (40, 7, 7))
    g.box(LIT, (-154, 110, 440), (14, 14, 22), glow=G)
    miner(sc, r, -60, 10, 270, yaw=180, arm_r='up', hold_r=P.hold_tool(STEEL, WOOD, 46), swing_r=32, axis_r='x', rate=0.8)
    miner(sc, r, 60, 16, 270, yaw=170, arm_l='hold', arm_r='hold', hold_r=P.hold_tool(STEEL, WOOD, 24), swing_r=14, axis_r='x', rate=1.5, hat_col=RED)
    miner(sc, r, 150, -60, 270, yaw=-20, arm_r='face', hold_r=h_lamp(), arm_l='hip')
    miner(sc, r, -140, -60, 270, yaw=15, arm_l='carry', arm_r='carry', hold_r=P.hold_box(WOOD_D, 70))


def d7_cart(sc):
    """A cart of teal crystal on a trestle: one miner leans into it, a signaller flags the road, one sits and eats."""
    r = random.Random(572)
    g = sc.still()
    for x in (-170, 0, 170):                                            # three A-frame trestles
        for sy in (-1, 1):
            tilt_box(g, WOOD_D, (x, sy * 120, -FOOT), (x, sy * 56, 236), 14)
        g.box(WOOD_D, (x, 0, 236), (16, 250, 14))
        g.box(WOOD_D, (x, 0, 60), (10, 200, 10))
    for y in (10, 70):
        g.box(STEEL, (0, y, 252), (400, 7, 8))                          # the rails
    for k in range(6):
        g.box(WOOD, (-170 + k * 68, 40, 245), (16, 100, 6))
    block(g, WOOD, 0, -78, 240, 252, 400, 96)                           # the walkway
    rail(g, WOOD_D, -196, 196, -122, 252, h=84, n=5)
    g.side(STEEL, [(-58, 284), (58, 284), (46, 350), (-46, 350)], 6, 74, loc=(40, 0, 0))    # the cart
    for sx in (-1, 1):
        for y in (8, 72):
            g.cyl(CHAR, (40 + sx * 34, y - 3, 270), 14, 6, rot=(-90, -90, 0), steps=8)
    for (x, y, h, rr) in ((20, 30, 70, 14), (48, 46, 90, 16), (66, 28, 56, 12), (30, 56, 60, 12)):
        crystal(g, x, y, 344, h, rr)
    block(g, WOOD_D, 160, -60, 252, 296, 50, 44)
    miner(sc, r, -50, 40, 256, yaw=90, arm_l='push', arm_r='push', rock=3, rate=0.6)
    miner(sc, r, -150, -70, 252, yaw=10, arm_r='wave', hold_r=P.hold_flag(YEL, WOOD, 46), swing_r=22, rate=0.8, hat_col=RED)
    miner(sc, r, 160, -52, 252, legs='sit', seat=44, arm_r='face', hold_r=P.hold_cup(STEEL), arm_l='hip')
    miner(sc, r, 60, -76, 252, yaw=-20, arm_l='point', arm_r='hip')


def d7_trader(sc):
    """The crystal trader on a cribbing platform: the balance tips slowly, a miner with a sack waits, a clerk writes."""
    r = random.Random(573)
    g = sc.still()
    for k in range(5):                                                  # cribbing: crossed logs
        z = -40 + k * 56
        if k % 2 == 0:
            for y in (-104, 104):
                g.box(WOOD_D, (0, y, z), (400, 34, 56))
        else:
            for x in (-170, 0, 170):
                g.box(WOOD, (x, 0, z), (34, 250, 56))
    block(g, WOOD_D, 0, 0, -FOOT, -60, 380, 230)
    block(g, WOOD, 0, 0, 212, 226, 400, 250)
    for x in (-190, 190):                                               # a plank lean-to
        block(g, WOOD_D, x, 114, 226, 470, 9, 9)
        block(g, WOOD_D, x, -30, 226, 430, 9, 9)
    tilt_box(g, WOOD, (0, 122, 476), (0, -40, 430), 400, 8)
    block(g, WOOD_D, 0, 0, 226, 380, 10, 10)                            # the balance
    beam = sc.part(P.motion(swing=5, pivot=(0, 0, 380), axis=90.0, rate=0.2, phase=0.4))
    beam.box(STEEL, (0, 0, 380), (150, 6, 6))
    for sx in (-1, 1):
        beam.box(STEEL, (sx * 70, 0, 348), (4, 4, 60))
        beam.cyl(STEEL, (sx * 70, 0, 314), 26, 5, steps=8)
    beam.cyl(TEAL_LIT, (-70, 0, 319), 12, 36, steps=5, top_r=3)
    beam.box(CHAR, (70, 0, 327), (20, 20, 16))
    block(g, WOOD_D, 140, 30, 226, 296, 70, 44)                         # the clerk's desk and the lamp on it
    g.box(CREAM, (140, 24, 299), (30, 22, 4))
    block(g, WOOD, 140, 82, 226, 270, 36, 34)                           # his stool
    g.box(LIT, (166, 40, 310), (10, 10, 18), glow=G)
    crystal(g, -170, 90, 226, 110, 18)
    crystal(g, -140, 100, 226, 70, 13, (12, 0))
    who(sc, r, 0, 60, 226, tops=(CHAR,), band=TEAL_LIT, hat='peak', hat_col=WOOD_D, arm_l='point', arm_r='hip', build='stocky')
    miner(sc, r, -100, -40, 226, yaw=30, arm_l='hip', hold_r=P.hold_bag(WOOD))
    miner(sc, r, 140, 76, 226, yaw=0, legs='sit', seat=46, arm_r='fwd', swing_r=6, axis_r='z', rate=1.2, hat_col=RED)
    miner(sc, r, 60, -70, 226, yaw=-20, arm_r='wave', swing_r=22, rate=0.9, react=True)


# ------------------------------------------------------------------------------------------ s1d8, The high ridge: the beacons
def d8_gallery(sc):
    """Keepers on the gallery of a banded beacon: one with a spyglass, one waves, one trims the steady yellow lamp."""
    r = random.Random(581)
    g = sc.still()
    g.cyl(CHAR, (0, 0, -FOOT), 132, FOOT + 110, steps=8)
    for k in range(4):
        g.cyl(RED if k % 2 == 0 else CREAM, (0, 0, 110 + k * 46), 100 - k * 4, 46, steps=8)
    g.cyl(CHAR, (0, 0, 294), 134, 12, steps=8)
    for k in range(7):                                                  # the gallery rail, round the front
        a = math.radians(200 + k * 23.3)
        x, y = 128 * math.cos(a), 128 * math.sin(a)
        block(g, RED, x, y, 306, 396, 6, 6)
        if k:
            tilt_box(g, RED, (px, py, 396), (x, y, 396), 6)
        px, py = x, y
    g.cyl(PANE, (0, 10, 306), 58, 120, steps=8)                         # the lamp room
    g.cyl(YEL, (0, 10, 330), 60, 60, steps=8, glow=G)
    g.cyl(RED, (0, 10, 426), 76, 60, steps=8, top_r=8)
    block(g, CHAR, 0, 10, 486, 530, 5, 5)
    g.box(RED, (-56, -100, 60), (6, 50, 100))
    g.box(PANE, (0, -100, 200), (26, 6, 40))
    block(g, CHAR, 170, 0, -FOOT, 150, 60, 200)                         # the keeper's store, a step lower
    block(g, RED, 170, 0, 150, 164, 70, 210)
    flag(sc, RED, 146, 96, 164, h=220, w=50, ph=0.5)
    who(sc, r, -92, -62, 306, yaw=40, tops=T8, hat='peak', hat_col=CHAR, band=CREAM, arm_l='face', arm_r='face', hold_r=h_horn(CHAR, 40, 7))
    who(sc, r, 0, -100, 306, tops=T8, hat='peak', hat_col=RED, band=CREAM, arm_r='wave', swing_r=24, rate=0.9, react=True)
    who(sc, r, 92, -62, 306, yaw=-40, tops=T8, hat='peak', hat_col=CHAR, arm_l='hold', arm_r='hold', hold_r=h_lamp())
    who(sc, r, 170, -40, 164, tops=T8, hat='cap', hat_col=YEL, arm_l='up', arm_r='up', bob=4, rate=1.1, react=True, **KID)


def d8_signal(sc):
    """Signalmen on a stone station: two work semaphore flags, the mast's arms move slowly, one keeps the log."""
    r = random.Random(582)
    g = sc.still()
    block(g, CHAR, 0, 0, -FOOT, 210, 392, 244)
    block(g, SLATE, 0, 0, 210, 226, 400, 250)
    for k in range(8):
        block(g, RED if k % 2 == 0 else CREAM, -175 + k * 50, -124, 180, 210, 50, 4)
    for k in range(6):                                                  # the banded mast
        block(g, RED if k % 2 == 0 else CREAM, 0, 90, 226 + k * 64, 290 + k * 64, 14, 14)
    for (z, ph, sg) in ((560, 0.0, 1), (480, 0.5, -1)):
        a = sc.part(P.motion(swing=28, pivot=(0, 82, z), axis=90.0, rate=0.18, phase=ph))
        a.box(RED, (sg * 54, 82, z), (100, 5, 22))
        a.box(CREAM, (sg * 96, 80, z), (16, 6, 22))
    block(g, RED, 150, 80, 226, 280, 80, 50)                            # the flag locker
    for k, c in enumerate((YEL, RED, CREAM, YEL)):
        g.cyl(c, (118 + k * 20, 80, 280), 8, 30, steps=6)
    sem = dict(arm_l='out', arm_r='out', swing_l=32, swing_r=32, rate=0.5, hat='peak', hat_col=CHAR)
    who(sc, r, -92, -50, 226, tops=(RED,), band=CREAM, hold_l=P.hold_flag(YEL, CHAR, 32), hold_r=P.hold_flag(RED, CHAR, 32), **sem)
    who(sc, r, 66, -50, 226, tops=(RED,), band=CREAM, hold_l=P.hold_flag(RED, CHAR, 32), hold_r=P.hold_flag(YEL, CHAR, 32), **sem)
    who(sc, r, -14, 20, 226, yaw=20, tops=T8, hat='peak', hat_col=RED, arm_l='face', arm_r='face', hold_r=h_horn(CHAR, 40, 7))
    who(sc, r, 150, 10, 226, yaw=-15, tops=T8, hat='cap', hat_col=YEL, arm_l='hold', arm_r='hold', hold_r=h_board(CREAM), swing_r=6, axis_r='z', rate=1.1)


def d8_lift(sc):
    """The last lift's platform under its banded roof: two still on their chair, three on the deck, all cheering."""
    r = random.Random(583)
    g = sc.still()
    legs(g, CHAR, ((-180, -100), (180, -100), (-180, 100), (180, 100)), 240, 18)
    xbrace(g, CHAR, -180, 180, 100, 20, 220)
    block(g, SLATE, 0, 0, 240, 254, 400, 250)
    block(g, YEL, 0, -124, 232, 252, 400, 4)
    rail(g, RED, -196, 40, -120, 254, n=4)
    for (x, y) in ((-186, 110), (186, 110), (-186, -10), (186, -10)):
        block(g, CHAR, x, y, 254, 520, 8, 8)
    for k, c in enumerate((RED, CREAM, RED, CREAM, RED)):
        tilt_box(g, c, (-160 + k * 80, 122, 548), (-160 + k * 80, -24, 520), 80, 8)
    g.box(STEEL, (120, 40, 520), (6, 6, 8))                             # a chair hangs from the roof
    block(g, STEEL, 120, 40, 356, 520, 5, 5)
    g.box(RED, (120, 30, 300), (120, 40, 8))
    g.box(RED, (120, 52, 330), (120, 6, 60))
    for k, c in enumerate((YEL, RED, TEAL, PINK)):                      # skis in a rack
        g.box(c, (-170 + k * 16, 112, 340), (8, 5, 170))
    cheer = dict(arm_l='up', arm_r='up', rate=1.2, react=True)
    who(sc, r, 92, 44, 254, tops=T8, hat='helmet', hat_col=RED, legs='sit', seat=50, swing_r=14, swing_l=-14, **cheer)
    who(sc, r, 148, 44, 254, tops=T8, hat='helmet', hat_col=YEL, legs='sit', seat=50, arm_l='down', arm_r='wave', swing_r=24, rate=1.0, react=True)
    who(sc, r, -140, -50, 254, tops=T8, hat='helmet', hat_col=CHAR, bob=6, **cheer)
    who(sc, r, -60, -66, 254, tops=T8, hair='pony', bob=5, hold_r=P.hold_flag(RED, STEEL, 46), **cheer)
    who(sc, r, 10, -40, 254, tops=T8, hat='cap', hat_col=RED, bob=6, **dict(cheer, **KID))


def d8_hoist(sc):
    """Keepers hoist a string of signal flags up the halyard of a banded mast; the flag locker stands open."""
    r = random.Random(584)
    g = sc.still()
    g.cyl(CHAR, (0, 0, -FOOT), 130, FOOT + 190, steps=8)
    block(g, CHAR, 150, 0, -FOOT, 190, 100, 240)
    block(g, CHAR, -150, 0, -FOOT, 190, 100, 240)
    block(g, SLATE, 0, 0, 190, 204, 400, 250)
    for k in range(8):
        block(g, RED if k % 2 == 0 else CREAM, -175 + k * 50, -124, 162, 190, 50, 4)
    for k in range(7):                                                  # the banded mast
        block(g, RED if k % 2 == 0 else CREAM, -150, 70, 204 + k * 64, 268 + k * 64, 14, 14)
    block(g, YEL, -150, 70, 652, 672, 22, 22, glow=G)
    bunting(sc, (RED, YEL, CREAM, RED, YEL), (-146, 66, 640), (150, 40, 240), 7, size=38, rate=0.8, line=CHAR)
    block(g, RED, 40, 96, 204, 264, 100, 46)                            # the locker, lid up
    tilt_box(g, RED, (40, 122, 264), (40, 132, 320), 100, 5)
    for k, c in enumerate((YEL, CREAM, RED, YEL)):
        g.cyl(c, (4 + k * 24, 96, 264), 9, 22, steps=6)
    who(sc, r, 150, 30, 204, yaw=70, tops=T8, hat='peak', hat_col=CHAR, band=CREAM, arm_l='up', arm_r='vee', swing_l=16, axis_l='x', swing_r=-16, axis_r='x', rate=0.7)
    who(sc, r, 110, -30, 204, yaw=60, tops=T8, hat='peak', hat_col=RED, arm_l='vee', arm_r='up', swing_l=-16, axis_l='x', swing_r=16, axis_r='x', rate=0.7, phase=0.5)
    who(sc, r, 20, 40, 204, yaw=180, tops=T8, hat='cap', hat_col=YEL, arm_l='carry', arm_r='carry', hold_r=P.hold_box(YEL, 30))
    who(sc, r, -60, -60, 204, tops=T8, hat='peak', hat_col=CHAR, band=CREAM, arm_r='face', arm_l='hip')
    who(sc, r, -120, -70, 204, tops=T8, hat='cap', hat_col=RED, arm_r='wave', swing_r=24, rate=1.0, react=True, **KID)


# ------------------------------------------------------------------------------------------ the half pipes: over the wall
def pipe_tower(sc):
    """A scaffold tower for the half pipes: two decks of fans 5.2 and 8 m up, race banners down its front."""
    r = random.Random(590)
    g = sc.still()
    legs(g, SLATE, ((-140, -110), (140, -110), (-140, 110), (140, 110)), 1040, 14)
    for (z0, z1) in ((20, 500), (540, 780)):
        xbrace(g, STEEL, -140, 140, -110, z0, z1, 7)
        xbrace(g, STEEL, -140, 140, 110, z0, z1, 7)
    for z in (520, 800):
        block(g, NAVY, 0, 0, z - 14, z, 300, 240)
        rail(g, STEEL, -146, 146, -116, z, n=4)
    for k, c in enumerate((TEAL, PINK, YEL)):                           # banners hang under the decks
        g.box(c, (-90 + k * 90, -119, 380), (64, 4, 240))
        g.box(c, (-90 + k * 90, -119, 700), (64, 4, 150))
    block(g, RED, 0, 0, 1040, 1052, 320, 260)
    block(g, CREAM, 0, 0, 1052, 1060, 240, 200)
    flag(sc, PINK, -140, 0, 1060, h=170, ph=0.1)
    flag(sc, TEAL, 120, 0, 1060, h=140, ph=0.6)
    ladder(g, STEEL, 100, 122, -60, 800, w=40, rungs=12)
    for z in (520, 800):
        who(sc, r, -100, -60, z, tops=T1, hat='beanie', hat_col=PINK, arm_l='up', arm_r='up', bob=6, rate=1.2, react=True)
        who(sc, r, -10, -66, z, tops=T1, hat='beanie', hat_col=TEAL, arm_r='wave', hold_r=P.hold_flag(YEL if z < 600 else PINK, STEEL, 56), swing_r=26, rate=0.9)
        who(sc, r, 90, -60, z, tops=T1, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), hat='cap', hat_col=RED)
    who(sc, r, 40, 30, 800, tops=T1, hat='beanie', hat_col=YEL, arm_l='vee', arm_r='vee', rock=6, rate=0.9, **KID)


JOBS = [
    ('SM_S1PEOPLE_D1_StartPod', d1_startpod), ('SM_S1PEOPLE_D1_Coaches', d1_coaches), ('SM_S1PEOPLE_D1_Cafe', d1_cafe),
    ('SM_S1PEOPLE_D2_Bunker', d2_bunker), ('SM_S1PEOPLE_D2_Gantry', d2_gantry), ('SM_S1PEOPLE_D2_Experts', d2_experts),
    ('SM_S1PEOPLE_D3_Bandstand', d3_bandstand), ('SM_S1PEOPLE_D3_Judges', d3_judges), ('SM_S1PEOPLE_D3_Market', d3_market),
    ('SM_S1PEOPLE_D4_Haul', d4_haul), ('SM_S1PEOPLE_D4_Mess', d4_mess), ('SM_S1PEOPLE_D4_Photo', d4_photo),
    ('SM_S1PEOPLE_D5_SkyWatch', d5_skywatch), ('SM_S1PEOPLE_D5_Kites', d5_kites), ('SM_S1PEOPLE_D5_Cairn', d5_cairn),
    ('SM_S1PEOPLE_D6_Quartet', d6_quartet), ('SM_S1PEOPLE_D6_Baths', d6_baths), ('SM_S1PEOPLE_D6_Gallery', d6_gallery),
    ('SM_S1PEOPLE_D7_Staging', d7_staging), ('SM_S1PEOPLE_D7_Cart', d7_cart), ('SM_S1PEOPLE_D7_Trader', d7_trader),
    ('SM_S1PEOPLE_D8_Gallery', d8_gallery), ('SM_S1PEOPLE_D8_Signal', d8_signal), ('SM_S1PEOPLE_D8_Lift', d8_lift),
    ('SM_S1PEOPLE_D8_Hoist', d8_hoist), ('SM_S1PEOPLE_PipeTower', pipe_tower),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
