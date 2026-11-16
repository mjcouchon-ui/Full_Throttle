"""City Center, Stage 1, builder s1people: people RAISED on their own structures, for the stretches pass 3 left.
/Game/World/CityCenter/SM_S1PEOPLE_*. Run: import ft_world_city_s1people as K; K.run()

Every piece is an ft_people.Scene (M_People, one section), TALL (it stands 24 m or more out) and brings what its
people stand on, 1.2 to 3 m up, so that they clear the barrier (the land lies 0.85 m under the road, the barrier is
1.6 m). Placed by C:/ftwork/s1/L7_s1people.inl in slots s % 8 == 1.

  Syndicate Territory (the arena's right-hand pavement, the circuit): the CITY'S OWN people, held back along the black
  wall. Home-made stands of crates, ladders and planks; nobody in black and gold; unlit; slow (0.5 a second and under).
    PressRiser   6 x 0.9 m scaffold riser against the wall: photographers, a sketch artist, a reporter, a child.
    WallSitters  6 x 0.7 m wall of packing crates under the wall's words: five sitting with their legs over the edge.
    PlinthBarrel a woman on a barrel and a crate, shading her eyes.       (the wall's left end)
    UmpireChair  an old man with field glasses on a tall ladder chair under a parasol.   (the wall's right end)
    NewsStack    a paper boy on his stack of bundles, a blank sheet held up.
    StandCrates  8 x 2.6 m, three tiers of planks on barrels and crates, ten neighbours (between two banner poles).
    StandLadders 8 x 2.6 m, two step-ladders and a trestle with planks between: painters' kit turned grandstand.
    PassQueue    7 x 1.5 m boardwalk before a checkpoint: a queue with its luggage, a volunteer at a desk.
    GirderLunch  the yard hands of the circuit eating on a girder laid over two trestles, cable drums for steps.
  The Plaza (civic marble, teal and white; lamps steady):
    TaiChi       a class on a stepped marble terrace, moving as one, slowly.
    StoryTerrace a story told to children on a library terrace, a book cart, a reader on a bench.
    EaselPergola painters at their easels under a white pergola, a sitter with a parasol.
    ChoirSteps   8 m: a choir in teal and white on tall steps over a colonnade, its conductor on a ledge (40 m out).
  The Garages:   LiftLunch    mechanics in white overalls eating on a four-post car lift raised 2.1 m (an alley's mouth).
  Theatre Row, Penthouse Row:  TuningBalcony  a plum balcony for a glass infill's face: harp, cello, violin, flute.
  Wall Street:   ClerksLedge  a sandstone ledge on corbels for a glass infill's face: clerks, a bell, the tea trolley.
Pictures and stripes only: no words, no mark, nobody real. No box in orange or green, no pipe or tube pointed at the road.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'
SLOW = dict(grow=(12.0, 12.0, 12.0), grow_down=(12.0, 12.0, 0.0))     # nothing here moves further than a hand's reach

WOOD, WOOD_D, CREAM, WHITE, RED, TEAL, BLUE, SILVER = '#9a6a44', '#6a4c36', '#fff1d6', '#fff6ea', '#ff2d3c', '#2fc6c0', '#3a5fa8', '#c9c4d6'
STEEL, CYAN, VIOLET, PLUM, MAG, LILAC, BLUSH = '#9a98a8', '#2ee6ff', '#8662b8', '#4a3a78', '#ff4fb8', '#c391d8', '#f3c4dc'
WARM, WARM_L, SAND, TAN, COPPER, GREEN_D, PINE, AQUA = '#ffe28a', '#fff1c0', '#b89a6a', '#8a6f5c', '#2f6b52', '#24403a', '#2f6b52', '#9ff0ff'
STONE_L, TYRE, GREY_D, BLUE_G, DARK = '#9a98a8', '#26232e', '#3f3d50', '#7a8fb8', '#22202a'
G_LAMP, G_BULB = 1.5, 2.0

# the city's own people in the Chairman's streets: no gold, no black (his men wear both)
SYN_TOPS = ('#ff5fa2', '#2ee6ff', '#7dffb0', '#fff6ea', '#8f6bff', '#3a5fa8', '#ff2d3c', '#57c7ff', '#f4a6ff', '#b8ff5a')
SYN_BOTS = ('#3a5fa8', '#f1e4c8', '#7a8fb8', '#c94f6a', '#e8e0f0', '#5a4a3c')
PLAZA_TOPS = (WHITE, TEAL, AQUA, '#57c7ff', BLUSH, WARM)
PLAZA_BOTS = ('#f1e4c8', '#e8e0f0', '#7a8fb8', TEAL)


def fig(sc, r, x, y, z=0.0, yaw=0.0, detail=1, tops=SYN_TOPS, bots=SYN_BOTS, **kw):
    d = P.look(r, tops=tops, bottoms=bots)
    d.update(kw)
    d.setdefault('phase', r.random())
    return P.figure(sc, x, y, z=z, yaw=yaw, detail=detail, **d)


def kid(sc, r, x, y, z=0.0, yaw=0.0, detail=1, **kw):
    kw.setdefault('s', r.uniform(0.62, 0.7))
    return fig(sc, r, x, y, z, yaw, detail, build='kid', **kw)


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_sheet(col=CREAM, w=44.0, h=34.0, band=None, hand=1):
    """A sheet, a book, a board held before the chest (drawn once, with the hand given: 1 right, -1 left)."""
    def f(m, hd, side):
        if side == hand:
            m.box(col, (hd[0] - side * 22, hd[1] - 7, hd[2] + 8), (w, 4, h))
            if band:
                m.box(band, (hd[0] - side * 22, hd[1] - 10, hd[2] + 8), (w * 0.7, 4, h * 0.3))
    return f


def hold_glasses():
    def f(m, hd, side):
        if side > 0:
            m.box(GREY_D, (hd[0] - 16, hd[1] - 9, hd[2] + 4), (30, 16, 11))
    return f


def hold_stick(col=WOOD, L=40.0, tip=None):
    """A brush, a bow, a baton: a short stick held out ahead."""
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1] - L / 2.0, hd[2] + 4), (5, L, 5))
        if tip:
            m.box(tip, (hd[0], hd[1] - L, hd[2] + 4), (7, 8, 7))
    return f


def hold_case(col=WOOD_D, w=44.0, h=32.0, trim=CREAM):
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1], hd[2] - h / 2.0 - 8), (16, w, h))
        m.box(trim, (hd[0], hd[1], hd[2] - 8), (5, 14, 5))
    return f


def hold_pinwheel():
    def f(m, hd, side):
        m.box(WOOD, (hd[0], hd[1], hd[2] + 24), (4, 4, 52))
        for k, c in enumerate((CYAN, MAG)):
            m.box(c, (hd[0], hd[1] - 4, hd[2] + 50), (34, 4, 9), rot=(45 + 90 * k, 0, 0))
    return f


def hold_bell(col=COPPER):
    def f(m, hd, side):
        m.box(WOOD, (hd[0], hd[1], hd[2] + 8), (5, 5, 16))
        m.cyl(col, (hd[0], hd[1], hd[2] + 14), 12, 20, steps=6, top_r=5)
    return f


def hold_pennant(col=CYAN, pole=WOOD):
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + 50), (4, 4, 120))
        m.box(col, (hd[0] + side * 26, hd[1], hd[2] + 96), (48, 4, 26))
    return f


# ------------------------------------------------------------------------------------------ carpentry
def bunting(sc, r, x0, x1, y, z, cols=(CREAM, CYAN, RED, BLUE), swing=4.0):
    """A line of little flags between two points at one height; it rocks slowly. The poles are the caller's."""
    b = sc.part(P.motion(swing=swing, pivot=((x0 + x1) / 2.0, y, z), axis=0.0, rate=0.3, phase=r.random()))
    b.box(GREY_D, ((x0 + x1) / 2.0, y, z), (abs(x1 - x0), 4, 4))
    n = max(3, int(abs(x1 - x0) / 75.0))
    for k in range(n):
        b.box(cols[k % len(cols)], (x0 + (k + 0.5) * (x1 - x0) / n, y, z - 17), (30, 4, 28))


def poles(g, xs, y, z0, z1, col=WOOD, w=7.0):
    for x in xs:
        g.box(col, (x, y, (z0 + z1) / 2.0), (w, w, z1 - z0))


def ladder(g, x, y0, y1, z0, z1, col=WOOD, w=44.0, rungs=5):
    for sx in (-1, 1):
        tilt_box(g, col, (x + sx * w / 2.0, y0, z0), (x + sx * w / 2.0, y1, z1), 6, 6)
    for k in range(rungs):
        t = (k + 0.5) / rungs
        g.box(col, (x, y0 + (y1 - y0) * t, z0 + (z1 - z0) * t), (w, 5, 5))


def crate(g, x, y, z, s=75.0, d=None, col=WOOD, slat=WOOD_D):
    """A packing crate standing on z (never orange, never green)."""
    d = d or s
    g.box(col, (x, y, z + s / 2.0), (s - 2, d, s - 2))
    g.box(slat, (x, y - d / 2.0 - 1.5, z + s / 2.0), (s - 2, 4, 9))


def barrel(g, x, y, z, r=40.0, h=92.0, col=WOOD, hoop=SILVER):
    g.cyl(col, (x, y, z), r, h, steps=8)
    for t in (0.2, 0.78):
        g.cyl(hoop, (x, y, z + h * t), r + 2.5, 6, steps=8)


def parasol(g, x, y, z0, z1, r=78.0, a=CREAM, b=RED, pole=SILVER):
    g.box(pole, (x, y, (z0 + z1) / 2.0), (5, 5, z1 - z0))
    g.cyl(b, (x, y, z1 - 8), r + 3, 6, steps=8)
    g.cyl(a, (x, y, z1 - 2), r, 26, steps=8, top_r=6)


def globe_lamp(g, x, y, z0, z1, col=WHITE, r=20.0):
    """A steady globe on a post (only on pieces that stand 24 m and more out)."""
    g.box(col, (x, y, (z0 + z1) / 2.0), (8, 8, z1 - z0))
    g.sphere(WARM_L, (x, y, z1 + r * 0.8), r, steps=(4, 6), glow=G_LAMP)


# ========================================================================================== Syndicate Territory
def press_riser(sc):
    """Against the black wall: a scaffold riser 6 m long, its deck 1.55 m up; the held-back press."""
    r = random.Random(701)
    g = sc.still()
    L, H = 600.0, 155.0
    for i in range(4):
        x = -L / 2.0 + 8 + i * (L - 16) / 3.0
        for y in (-38.0, 38.0):
            g.box(SILVER, (x, y, (H - 100) / 2.0), (8, 8, H + 100))
        g.box(SILVER, (x, -38, H + 52), (6, 6, 104))
    for i in range(3):
        x0 = -L / 2.0 + 8 + i * (L - 16) / 3.0
        x1 = x0 + (L - 16) / 3.0
        a, b = ((x0, x1) if i % 2 == 0 else (x1, x0))
        tilt_box(g, STEEL, (a, -38, 16), (b, -38, H - 16), 5, 5)
    g.box(WOOD, (0, 0, H - 5), (L, 90, 10))
    g.box(CREAM, (0, -44, H - 24), (L, 4, 28))
    g.box(RED, (0, -45.5, H - 24), (L * 0.9, 4, 8))
    g.box(BLUE, (0, -38, H + 100), (L - 12, 5, 5))                        # the rope they are held behind
    ladder(g, 318, -30, 20, -40, H, col=STEEL)
    # a camera on a tripod, its operator panning slowly
    tx, ty = -225.0, -14.0
    for dx, dy in ((-18, -12), (18, -12), (0, 20)):
        tilt_box(g, GREY_D, (tx + dx, ty + dy, H), (tx, ty, H + 118), 4, 4)
    pan = dict(swing=5.0, pivot=(tx, ty, H), axis=P.UP)
    c = sc.part(P.motion(rate=0.25, phase=0.2, **pan))
    c.box(GREY_D, (tx, ty, H + 130), (24, 34, 22))
    c.box(SILVER, (tx, ty - 22, H + 130), (11, 12, 11))
    fig(sc, r, tx + 6, 24, H, yaw=4, arm_l='hold', arm_r='hold', hat='cap', hat_col=BLUE, rate=0.25, phase=0.2, sway=pan)
    # a sketch artist: the board on the arm, the pencil hand working
    fig(sc, r, -105, 12, H, yaw=-12, arm_l='carry', arm_r='hold', hold_l=hold_sheet(WHITE, 40, 50, band=BLUE_G, hand=-1), hold_r=hold_stick(WOOD, 14),
        hat='beanie', hat_col=RED, rate=0.5, swing_r=7, axis_r='x')
    # a reporter with a headset and a pad
    fig(sc, r, 10, 10, H, yaw=8, arm_l='hold', arm_r='hold', hold_r=hold_sheet(CREAM, 26, 30), hat='cap', hat_col=WHITE, rate=0.3, turn=5)
    # a photographer, the camera at the eye; a silver reflector on a stand beside
    fig(sc, r, 125, 12, H, yaw=-6, arm_l='face', arm_r='face', hold_r=P.hold_camera(), wear='coat', rate=0.3, turn=7)
    g.box(GREY_D, (196, 26, H + 80), (5, 5, 160))
    g.cyl(SILVER, (196, 22, H + 150), 30, 4, rot=(80, 90, 0), steps=8)
    # a child on a box, waving slowly
    crate(g, 252, 8, H, s=38.0, col=CREAM, slat=BLUE)
    kid(sc, r, 252, 8, H + 38, yaw=10, arm_r='wave', swing_r=12, rate=0.45)
    poles(g, (-292, 292), 38, H, H + 265, col=SILVER, w=6)
    bunting(sc, r, -292, 292, 38, H + 262)


def wall_sitters(sc):
    """A wall of packing crates 6 m long and 1.5 m high under the wall's own words: five sitting on its edge."""
    r = random.Random(702)
    g = sc.still()
    cols = (WOOD, CREAM, WOOD_D, BLUE_G)
    for i in range(8):
        x = -262.5 + i * 75.0
        g.box(cols[i % 4], (x, 10, 37.5 - 50), (73, 70, 75 + 100))
        g.box(cols[(i + 2) % 4], (x, 10, 112.5), (73, 70, 73))
        g.box(WOOD_D if cols[(i + 2) % 4] != WOOD_D else WOOD, (x, -26.5, 112.5), (73, 4, 9))
    T = 150.0
    ladder(g, -322, -24, 10, -40, T)
    # a reader behind an open paper
    fig(sc, r, -235, 12, T - 46, legs='sit', arm_l='hold', arm_r='hold', hold_r=hold_sheet(WHITE, 58, 40, band=STEEL), hat='cap', hat_col=RED,
        rate=0.25, swing_r=3, axis_r='x')
    # knitting
    fig(sc, r, -118, 12, T - 46, legs='sit', arm_l='hold', arm_r='hold', hair='bun', hair_col='#cfcfd8', wear='skirt', hold_r=hold_stick(SILVER, 20),
        hold_l=hold_stick(SILVER, 20), rate=0.5, swing_l=6, swing_r=-6, axis_l='x', axis_r='x')
    g.sphere(MAG, (-84, 14, T + 14), 14, steps=(3, 6))                     # the ball of wool
    # a child with a pinwheel
    kid(sc, r, 0, 12, T - 46 * 0.66, legs='sit', arm_r='wave', hold_r=hold_pinwheel(), swing_r=10, rate=0.4)
    # a mug at the lips; a flask and a lunch tin beside
    fig(sc, r, 118, 12, T - 46, legs='sit', arm_r='face', hold_r=P.hold_cup(WHITE), arm_l='down', rate=0.3, swing_r=5, axis_r='x')
    g.cyl(SILVER, (62, 18, T), 7, 30, steps=6)
    g.box(CREAM, (176, 20, T + 9), (26, 16, 18))
    g.box(BLUE, (176, 20, T + 19.5), (10, 5, 4))
    # leaning back, a hat over the eyes
    fig(sc, r, 236, 12, T - 46, legs='sit', arm_l='back', arm_r='back', hat='sun', hat_col=CREAM, rate=0.2, swing_l=2)
    poles(g, (-292, 292), 40, T, T + 250)
    bunting(sc, r, -292, 292, 40, T + 247, cols=(CREAM, MAG, CYAN, WHITE))


def plinth_barrel(sc):
    """A woman on a barrel and a crate (1.55 m), a hand shading her eyes, looking up and down the road."""
    r = random.Random(703)
    g = sc.still()
    barrel(g, 0, 4, -60, r=40, h=152)
    crate(g, 0, 4, 92, s=62.0, col=CREAM, slat=BLUE)
    g.box(WOOD, (38, 30, 100), (5, 5, 330))
    b = sc.part(P.motion(swing=5.0, pivot=(38, 30, 265), axis=P.UP, rate=0.3, phase=0.4))
    b.box(CYAN, (62, 30, 250), (46, 4, 28))
    fig(sc, r, 0, 4, 154, detail=2, arm_r='face', arm_l='hip', wear='coat', hair='long', top='#ff5fa2', rate=0.22, turn=9)
    crate(g, -52, 14, -40, s=46.0, d=40.0)                                 # the step she climbed by
    g.box(CREAM, (-52, 14, 12), (24, 18, 12))


def umpire_chair(sc):
    """An old man with field glasses on a tall ladder chair (seat 1.6 m) under a striped parasol."""
    r = random.Random(704)
    g = sc.still()
    S = 160.0
    for sx in (-1, 1):
        tilt_box(g, WHITE, (sx * 44, -40, -60), (sx * 34, -18, S), 7, 7)
        tilt_box(g, WHITE, (sx * 44, 40, -60), (sx * 34, 22, S + 70), 7, 7)
        g.box(WHITE, (sx * 34, 2, S + 26), (6, 44, 6))
    for k in range(4):
        t = (k + 0.6) / 4.6
        g.box(BLUE, (0, -40 + 22 * t, -60 + (S + 60) * t), (88 - 20 * t, 6, 6))
    g.box(BLUE, (0, 2, S - 4), (76, 46, 8))
    g.box(BLUE, (0, 23, S + 46), (70, 6, 36))
    parasol(g, 40, 30, S, S + 205, r=74, a=CREAM, b=BLUE)
    fig(sc, r, 0, 4, S - 46, detail=2, legs='sit', arm_l='face', arm_r='face', hold_r=hold_glasses(), hat='sun', hat_col=WHITE, hair='short',
        hair_col='#cfcfd8', top='#7dffb0', rate=0.2, turn=10)
    g.box(CREAM, (-46, 6, S - 40), (10, 26, 30))                           # his bag on the rail
    g.box(WOOD, (-46, 6, S - 22), (5, 5, 8))


def news_stack(sc):
    """A paper boy on his stack of bundles (1.5 m), a blank sheet held up."""
    r = random.Random(705)
    g = sc.still()
    crate(g, 0, 6, -60, s=84.0, d=70.0)
    z = 24.0
    for k in range(5):
        w = 78 - 4 * k
        g.box((CREAM, WHITE)[k % 2], (0, 6, z + 12.5), (w, 58, 25))
        g.box(BLUE, (0, 6, z + 12.5), (7, 60, 26.5))
        z += 25.5
    fig(sc, r, 0, 6, z, detail=2, s=0.88, arm_r='up', arm_l='carry', hold_r=hold_sheet(WHITE, 40, 52, band=STEEL), hold_l=P.hold_bag(CREAM),
        hat='cap', hat_col=RED, top='#57c7ff', bottom='#5a4a3c', rate=0.4, swing_r=9)
    g.box(CREAM, (-64, 6, -5.5), (40, 54, 109))                           # the next bundles, waiting
    g.box(BLUE, (-64, 6, 30), (7, 56, 40))


def stand_crates(sc):
    """Three tiers of planks on barrels and crates, 8 x 2.6 m: ten of the neighbours, between two banner poles."""
    r = random.Random(706)
    g = sc.still()
    L = 800.0
    tiers = ((-85.0, 120.0), (5.0, 190.0), (95.0, 260.0))                 # (middle y, deck height)
    for k, (y, z) in enumerate(tiers):
        g.box((WOOD, CREAM, WOOD)[k], (0, y, z - 5), (L, 88, 10))
        g.box((CREAM, BLUE_G, CREAM)[k], (0, y - 46, z - 24), (L, 4, 30))
        for i in range(5):
            x = -340 + i * 170.0
            if k == 0 and i % 2 == 0:
                barrel(g, x, y, -60, r=34, h=z + 50)
            elif k == 0:
                g.box(CREAM, (x, y, (z - 70) / 2.0), (66, 70, z + 50))
            else:
                g.box((WOOD_D, BLUE_G)[(i + k) % 2], (x, y, (z - 110) / 2.0), (70, 80, z + 90))
    g.box(BLUE, (0, -131, 97), (L * 0.86, 4, 9))
    for k in range(3):                                                    # the steps at the left end
        g.box(WOOD, (-430, -85 + k * 45, (40 + 45 * k) / 2.0 - 30), (58, 45, 40 + 45 * k + 60))
    poles(g, (-392, 392), 134, 0, 520, w=8)
    bunting(sc, r, -392, 392, 134, 516)
    parasol(g, 300, 108, 260, 505, r=92, a=CREAM, b=BLUE)
    z = tiers[0][1]
    top = fig(sc, r, -300, -80, z, arm_l='up', arm_r='up', build='stocky', rate=0.3, turn=4)
    kid(sc, r, -300, -72, top - 62, legs='sit', arm_r='wave', swing_r=12, rate=0.45, s=0.6)
    fig(sc, r, -120, -82, z, arm_r='wave', hold_r=hold_sheet(WHITE, 20, 20), swing_r=12, rate=0.4, wear='dress')
    fig(sc, r, 70, -80, z, arm_l='hold', arm_r='hold', hold_r=P.hold_phone())
    fig(sc, r, 250, -82, z, arm_l='hip', arm_r='hip', hat='cap', hat_col=CYAN, rate=0.25, turn=8)
    z = tiers[2][1] - 46.0
    fig(sc, r, -210, 62, z, legs='sit', arm_r='face', hold_r=P.hold_cup(WHITE), rate=0.3, swing_r=5, axis_r='x')
    fig(sc, r, -20, 62, z, legs='sit', arm_l='hold', arm_r='hold', hair='bun', wear='skirt', hold_r=hold_sheet(CREAM, 30, 22))
    kid(sc, r, 150, 62, tiers[2][1] - 46 * 0.66, legs='sit', arm_l='up', arm_r='up', rate=0.45, swing_l=8, swing_r=-8)
    z = tiers[2][1]
    fig(sc, r, -310, 104, z, detail=0, arm_r='wave', hold_r=hold_pennant(CYAN), swing_r=10, rate=0.4)
    fig(sc, r, -100, 104, z, arm_l='face', arm_r='face', hold_r=hold_glasses(), rate=0.2, turn=10)
    fig(sc, r, 110, 104, z, detail=0, arm_r='point', hat='sun', hat_col=WHITE, hair_col='#cfcfd8', rate=0.3, swing_r=6)


def stand_ladders(sc):
    """Painters' kit turned grandstand, 8 x 2.6 m: two step-ladders and a trestle, planks between at 1.3 and 2.1 m."""
    r = random.Random(707)
    g = sc.still()
    for x in (-300.0, 300.0):
        for sx in (-1, 1):
            tilt_box(g, WOOD, (x + sx * 36, -120, -40), (x + sx * 24, -4, 300), 7, 7)
            tilt_box(g, WOOD, (x + sx * 36, 120, -40), (x + sx * 24, 4, 300), 7, 7)
        for k in range(6):
            t = (k + 0.7) / 6.6
            g.box(WOOD, (x, -120 + 116 * t, -40 + 340 * t), (70 - 22 * t, 6, 6))
        g.box(WOOD, (x, 0, 302), (60, 30, 8))
    for sy in (-1, 1):
        tilt_box(g, WOOD_D, (0, sy * 105, -40), (0, sy * 30, 204), 8, 8)
    g.box(WOOD_D, (0, 0, 204), (70, 90, 8))
    lo, hi = 130.0, 212.0
    g.box(CREAM, (0, -74, lo - 4), (640, 36, 8))
    g.box(CREAM, (0, 18, hi - 4), (640, 36, 8))
    g.box(WHITE, (-180, -74, lo + 1), (150, 40, 4))                       # a dust sheet over the plank
    g.box(WHITE, (-180, -93, lo - 22), (150, 4, 46))
    for x, c in ((60, CYAN), (96, MAG), (-250, WHITE)):
        g.cyl(SILVER, (x, 22, hi), 13, 20, steps=6)
        g.cyl(c, (x, 22, hi + 18), 11, 4, steps=6)
    g.cyl(SILVER, (322, -62, 150), 15, 24, steps=6)                       # a bucket on the ladder's hook
    g.box(GREY_D, (322, -62, 186), (4, 4, 30))
    bunting(sc, r, -300, 300, 0, 380, cols=(CREAM, MAG, CYAN, WHITE))
    poles(g, (-300, 300), 0, 300, 384, w=6)
    fig(sc, r, -200, -66, lo - 46, legs='sit', arm_l='hold', arm_r='hold', hold_r=hold_sheet(CREAM, 30, 20), hat='cap', hat_col=WHITE)
    fig(sc, r, -70, -66, lo - 46, legs='sit', arm_r='wave', swing_r=11, rate=0.4, wear='dress')
    kid(sc, r, 60, -66, lo - 46 * 0.66, legs='sit', arm_r='point', rate=0.4, swing_r=6)
    fig(sc, r, 190, -66, lo - 46, legs='sit', arm_r='face', hold_r=P.hold_cup(CREAM), rate=0.3, swing_r=5, axis_r='x', build='stocky')
    fig(sc, r, -120, 26, hi - 46, legs='sit', arm_l='face', arm_r='face', hold_r=hold_glasses(), rate=0.2, turn=9)
    fig(sc, r, 170, 22, hi, arm_l='hip', arm_r='face', hat='beanie', hat_col=CYAN, rate=0.25, turn=7)
    fig(sc, r, -300, -52, 150, arm_l='hold', arm_r='wave', swing_r=10, rate=0.4, top=WHITE, bottom='#f1e4c8', hat='cap', hat_col=WHITE)


def pass_queue(sc):
    """Before a checkpoint: a boardwalk 7 x 1.5 m, 1.3 m up; a queue with its luggage, a volunteer stamping at a desk."""
    r = random.Random(708)
    g = sc.still()
    L, H = 700.0, 130.0
    g.box(WOOD, (0, 0, H - 6), (L, 150, 12))
    for i in range(4):
        g.box((WOOD_D, BLUE_G)[i % 2], (-300 + i * 200, 0, (H - 112) / 2.0), (64, 132, H + 88))
    g.box(CREAM, (0, -76.5, H - 26), (L, 4, 28))
    for k in range(3):                                                    # the steps up, at the tail
        g.box(WOOD, (372 + k * 40, 0, (H - 34 * (k + 1)) / 2.0 - 30), (40, 110, H - 34 * (k + 1) + 60))
    for x in (-340, -170, 0, 170, 340):
        g.box(WHITE, (x, -68, H + 45), (6, 6, 90))
    g.box(BLUE, (0, -68, H + 86), (680, 5, 5))
    # the desk at the head of the queue (left), its volunteer facing down the line
    g.box(CREAM, (-262, 0, H + 76), (70, 56, 6))
    for sx in (-1, 1):
        g.box(WOOD_D, (-262 + sx * 28, 0, H + 37), (6, 50, 74))
    g.box(WHITE, (-250, -8, H + 82), (24, 30, 5))
    fig(sc, r, -318, 0, H, yaw=90, arm_r='fwd', arm_l='down', hold_r=hold_stick(WOOD_D, 10, tip=BLUE), band=CYAN, top=WHITE, hat='cap', hat_col=CYAN,
        rate=0.5, swing_r=10, axis_r='x')
    # the queue, everyone facing the desk
    fig(sc, r, -196, 4, H, yaw=-90, arm_r='fwd', hold_r=hold_sheet(CREAM, 18, 24), wear='coat', rate=0.3, swing_r=4)
    fig(sc, r, -98, -4, H, yaw=-84, arm_l='down', arm_r='down', hold_r=hold_case(WOOD_D), hat='sun', hat_col=CREAM, wear='dress')
    g.box(BLUE_G, (-2, 6, H + 18), (58, 38, 36))                           # a trunk, a child sitting on it
    g.box(CREAM, (-2, 6, H + 18), (60, 6, 38))
    kid(sc, r, -2, 6, H + 36 - 46 * 0.66, yaw=-60, legs='sit', arm_r='wave', swing_r=8, rate=0.4)
    fig(sc, r, 92, 0, H, yaw=-96, arm_l='hold', arm_r='hold', hold_r=hold_sheet(WHITE, 50, 36, band=STEEL), hat='cap', hat_col=RED)
    top = fig(sc, r, 190, 6, H, yaw=-88, arm_l='down', arm_r='carry', hair='bun', hair_col='#cfcfd8', wear='skirt', rate=0.25, turn=4)
    g.cyl(SILVER, (166, -4, H + 84), 15, 26, steps=6)                      # her birdcage, empty
    g.cyl(SILVER, (166, -4, H + 110), 15, 9, steps=6, top_r=3)
    fig(sc, r, 284, -2, H, yaw=-92, arm_l='down', arm_r='face', hold_l=hold_case(BLUE_G, 40, 30), rate=0.3, swing_r=5, axis_r='x', build='stocky')
    for k, c in enumerate((CREAM, WOOD_D)):                               # hat boxes at the tail
        g.cyl(c, (330, 40, H + k * 17), 19 - 2 * k, 16, steps=8)


def girder_lunch(sc):
    """The circuit's yard hands eating on a steel girder laid over two riveted trestles, 2.2 m up; cable drums for steps."""
    r = random.Random(709)
    g = sc.still()
    T = 220.0
    for x in (-280.0, 280.0):
        for sy in (-1, 1):
            tilt_box(g, STEEL, (x - 46, sy * 78, -60), (x - 14, sy * 14, T - 36), 10, 10)
            tilt_box(g, STEEL, (x + 46, sy * 78, -60), (x + 14, sy * 14, T - 36), 10, 10)
        g.box(STEEL, (x, 0, 70), (96, 8, 8))
        g.box(VIOLET, (x, 0, T - 40), (64, 44, 10))
    g.box(SILVER, (0, 0, T - 3), (760, 34, 6))
    g.box(SILVER, (0, 0, T - 32), (760, 34, 6))
    g.box(STEEL, (0, 0, T - 17.5), (760, 7, 24))
    for k in range(9):
        g.box(CYAN if k == 4 else STEEL, (-340 + k * 85, -17.5, T - 17.5), (6, 4, 24))
    # two cable drums on their sides and a plank up from them
    for x, rr in ((-130.0, 58.0), (120.0, 46.0)):
        for sy in (-1, 1):
            rod(g, SILVER, (x, 60 + sy * 34, rr), (x, 60 + sy * 40, rr), rr, steps=10)
        rod(g, VIOLET, (x, 28, rr), (x, 92, rr), rr * 0.55, steps=8)
    tilt_box(g, WOOD, (-130, 60, 118), (-30, 20, T - 34), 30, 6)
    over = dict(tops=(BLUE_G, '#57c7ff', '#7a8fb8', WHITE), bots=(BLUE_G, '#7a8fb8'))
    hats = (WHITE, CYAN, WHITE, VIOLET, WHITE, CYAN)
    acts = (dict(arm_r='face', hold_r=P.hold_cup(WHITE), swing_r=5, axis_r='x', rate=0.3),
            dict(arm_l='hold', arm_r='hold', hold_r=hold_sheet(WHITE, 26, 16)),
            dict(arm_r='wave', swing_r=11, rate=0.4),
            dict(arm_l='hold', arm_r='hold', hold_r=hold_sheet(CREAM, 46, 34, band=STEEL)),
            dict(arm_r='face', hold_r=P.hold_cup(CREAM), swing_r=4, axis_r='x', rate=0.25))
    for k in range(5):
        fig(sc, r, -250 + k * 110, 10, T - 46, legs='sit', hat='hard', hat_col=hats[k], band=(CYAN if k % 2 else None), **dict(over, **acts[k]))
    g.cyl(SILVER, (300, 0, T), 15, 34, steps=7)                           # the tea urn
    g.box(GREY_D, (300, -17, T + 10), (5, 8, 5))
    g.box(CREAM, (-320, 0, T + 9), (28, 18, 18))
    fig(sc, r, 345, 0, T, arm_r='wave', arm_l='hip', hat='hard', hat_col=hats[5], swing_r=10, rate=0.35, **over)
    fig(sc, r, 120, 60, 92, arm_l='hip', arm_r='point', hat='hard', hat_col=WHITE, rate=0.25, turn=6, **over)


# ========================================================================================== The Plaza
def _terrace(g, L, top, step, col=WHITE, trim=TEAL, stone=STONE_L):
    """A marble terrace L long, its deck `top` up from y -100 to +150, one broad step before it; 1.5 m of footing."""
    g.box(col, (0, 25, (top - 150) / 2.0), (L, 250, top + 150))
    g.box(stone, (0, -125, (step - 150) / 2.0), (L, 50, step + 150))
    g.box(trim, (0, -101.5, top - 14), (L - 40, 4, 12))
    g.box(col, (0, 25, top + 3), (L + 16, 262, 6))


def _balusters(g, L, y, z, n=8, col=WHITE, rail=TEAL, h=70.0):
    for k in range(n):
        g.cyl(col, (-L / 2.0 + 20 + k * (L - 40) / (n - 1), y, z), 9, h, steps=5, top_r=6)
    g.box(rail, (0, y, z + h + 4), (L, 14, 8))


def tai_chi(sc):
    """A class on a stepped marble terrace 8.6 m long, 1.5 m up: seven moving as one, slowly."""
    r = random.Random(710)
    g = sc.still()
    L, T = 860.0, 150.0
    _terrace(g, L, T, 75.0)
    _balusters(g, L, 140, T + 6)
    for sx in (-1, 1):
        g.cyl(STONE_L, (sx * 400, -70, T + 6), 24, 46, steps=7, top_r=32)
        globe_lamp(g, sx * 400, 140, T + 80, T + 250)
    g.box(TEAL, (0, 20, T + 8), (620, 170, 4))                          # the mat they stand on
    spots = [(0, -60)] + [(-270, 10), (-135, 10), (135, 10), (270, 10), (-200, 95), (200, 95)]
    for k, (x, y) in enumerate(spots):
        fig(sc, r, x, y, T + 10, legs='wide', tops=((TEAL,) if k == 0 else (WHITE, WHITE, AQUA)), bots=((WHITE,) if k == 0 else ('#e8e0f0', TEAL, WHITE)),
            sleeves='long', arm_l='fwd', arm_r='out', swing_l=24, swing_r=24, axis_l='z', axis_r='z', rate=0.22, phase=0.1, hair=('bun' if k % 3 == 0 else 'short'))


def story_terrace(sc):
    """A library terrace 8.2 m long, 1.4 m up: a story told to children, a book cart, a reader on a bench."""
    r = random.Random(711)
    g = sc.still()
    L, T = 820.0, 140.0
    _terrace(g, L, T, 70.0)
    _balusters(g, L, 140, T + 6, n=7)
    Z = T + 6
    for sx in (-1, 1):
        globe_lamp(g, sx * 395, -80, Z, Z + 210)
    # the teller on a tall stool, the open book on a knee
    g.cyl(TEAL, (-290, 20, Z), 17, 62, steps=6, top_r=20)
    fig(sc, r, -290, 20, Z + 62 - 46, yaw=78, legs='sit', arm_l='hold', arm_r='wave', hold_l=hold_sheet(WHITE, 46, 30, band=TEAL, hand=-1), wear='dress',
        tops=(TEAL,), hair='bun', rate=0.45, swing_r=12, detail=2)
    for k, (x, y) in enumerate(((-170, -30), (-150, 60), (-80, 10), (-50, 95), (10, -45))):
        g.box((TEAL, AQUA, BLUSH)[k % 3], (x, y, Z + 5), (44, 44, 10))
        kid(sc, r, x, y, Z + 10 - 12 * 0.66, yaw=-90 + r.uniform(-25, 25), legs='sit', seat=12.0, tops=PLAZA_TOPS, bots=PLAZA_BOTS,
            arm_r=('up' if k == 2 else 'down'), rate=0.5, swing_r=(8 if k == 2 else 0))
    # the book cart and its keeper
    g.box(TEAL, (150, 70, Z + 46), (96, 44, 70))
    g.box(WHITE, (150, 47, Z + 46), (84, 4, 54))
    for k in range(7):
        g.box((RED, WARM, BLUE, BLUSH, PINE, AQUA, MAG)[k], (114 + k * 12, 70, Z + 94), (9, 34, 26 + (k % 3) * 4))
    for sx in (-1, 1):
        g.cyl(GREY_D, (150 + sx * 34, 70, Z), 9, 10, steps=6)
    fig(sc, r, 226, 72, Z, yaw=-90, arm_l='push', arm_r='push', tops=PLAZA_TOPS, bots=PLAZA_BOTS, wear='skirt', rate=0.3, turn=4)
    # a bench and a reader
    g.box(WHITE, (320, -20, Z + 42), (120, 44, 8))
    for sx in (-1, 1):
        g.box(STONE_L, (320 + sx * 48, -20, Z + 19), (12, 40, 38))
    fig(sc, r, 320, -18, Z + 46 - 46, legs='sit', arm_l='hold', arm_r='hold', hold_r=hold_sheet(BLUSH, 34, 24, band=WHITE), tops=PLAZA_TOPS, bots=PLAZA_BOTS,
        hat='sun', hat_col=WHITE)


def easel_pergola(sc):
    """Painters at their easels under a white pergola hung with lamps, on a terrace 8.2 m long; a sitter with a parasol."""
    r = random.Random(712)
    g = sc.still()
    L, T = 820.0, 130.0
    _terrace(g, L, T, 65.0)
    Z = T + 6
    for sx in (-1, 1):
        for y in (-80.0, 130.0):
            g.box(WHITE, (sx * 385, y, Z + 165), (13, 13, 330))
        g.box(WHITE, (sx * 385, 25, Z + 334), (16, 250, 12))
    for y in (-80.0, 130.0):
        g.box(WHITE, (0, y, Z + 345), (L, 12, 10))
    for k in range(8):
        g.box(TEAL if k % 2 else WHITE, (-350 + k * 100, 25, Z + 354), (10, 262, 8))
    for x in (-250.0, 0.0, 250.0):
        g.box(GREY_D, (x, -80, Z + 318), (4, 4, 44))
        g.sphere(WARM_L, (x, -80, Z + 284), 17, steps=(4, 6), glow=G_LAMP)
    pics = ((AQUA, PINE, WARM), (BLUSH, MAG, WHITE), (WARM, RED, TEAL))
    for k, x in enumerate((-250.0, 20.0, 270.0)):
        ex, ey = x - 46, 20.0
        for dx, dy in ((-20, -10), (20, -10), (0, 26)):
            tilt_box(g, WOOD, (ex + dx, ey + dy, Z), (ex, ey + 4, Z + 150), 5, 5)
        g.box(WHITE, (ex, ey - 4, Z + 118), (6, 62, 78), rot=(0, -40, 0))
        g.box(pics[k][0], (ex + 2.7, ey - 6.3, Z + 132), (5, 54, 40), rot=(0, -40, 0))
        g.box(pics[k][1], (ex + 2.7, ey - 6.3, Z + 98), (5, 54, 26), rot=(0, -40, 0))
        g.box(pics[k][2], (ex + 4.2, ey - 7.6, Z + 136), (5, 20, 20), rot=(0, -40, 0))
        fig(sc, r, x, ey + r.uniform(-6, 6), Z, yaw=-72, arm_r='fwd', arm_l='hip', hold_r=hold_stick(WOOD, 26, tip=pics[k][1]), tops=(CREAM, WHITE, AQUA),
            bots=PLAZA_BOTS, hat=('beanie' if k != 1 else 'sun'), hat_col=(TEAL, WHITE, BLUSH)[k], rate=0.6, swing_r=9, axis_r='x')
    # the sitter on a marble drum, a parasol on her shoulder; a child watching a painter work
    g.cyl(WHITE, (150, -62, Z), 30, 46, steps=8)
    fig(sc, r, 150, -58, Z, legs='sit', arm_l='down', arm_r='hold', wear='dress', tops=(BLUSH, AQUA), hair='long', detail=2)
    parasol(g, 176, -40, Z + 60, Z + 215, r=58, a=WHITE, b=TEAL, pole=WOOD)
    kid(sc, r, -160, 70, Z, yaw=-130, arm_r='point', tops=PLAZA_TOPS, bots=PLAZA_BOTS, rate=0.4, swing_r=6)


def choir_steps(sc):
    """8 m tall, for 40 m and more out: a choir in teal and white on tall steps over a colonnade, swaying row by row;
    its conductor on a ledge before it. Lamps at the corners (steady)."""
    r = random.Random(713)
    g = sc.still()
    L = 700.0
    g.box(STONE_L, (0, 30, -65), (L + 60, 330, 170))
    g.box(WHITE, (0, 70, 150), (L - 40, 200, 300))
    for k in range(6):
        g.cyl(WHITE, (-300 + k * 120, -80, 20), 26, 262, steps=6)
    g.box(WHITE, (0, -40, 291), (L, 150, 18))
    g.box(TEAL, (0, -116.5, 291), (L - 30, 4, 10))
    rows = (5, 4, 5)
    for k, n in enumerate(rows):
        y, z = -60.0 + 58 * k, 300.0 + 55 * k
        g.box((WHITE, STONE_L)[k % 2], (0, y, (z + 290) / 2.0), (L - 40, 58, z - 290 + 0.1))
        for i in range(n):
            x = -(n - 1) * 120 / 2.0 + i * 120 + r.uniform(-8, 8)
            d = P.look(r)
            P.figure(sc, x, y, z=z, detail=0, skin=d['skin'], hair=d['hair'], hair_col=d['hair_col'], top=(TEAL, WHITE)[k % 2], bottom=(TEAL, WHITE)[k % 2],
                     wear='dress', sleeves='long', band=(WHITE, TEAL)[k % 2], arm_l='hold', arm_r='hold', rock=4.0, rate=0.35, phase=0.25 * k)
    g.box(TEAL, (0, 120, 560), (L - 40, 14, 280))
    g.box(WHITE, (0, 120, 707), (L - 20, 20, 14))
    g.cyl(WHITE, (0, 120, 714), 120, 36, steps=8, top_r=30)
    for sx in (-1, 1):
        globe_lamp(g, sx * 335, -100, 300, 470, r=24)
        globe_lamp(g, sx * 335, 120, 714, 770, r=24)
    # the conductor's ledge, and a lyre for a music stand
    g.box(WHITE, (0, -150, 293), (130, 80, 14))
    tilt_box(g, STONE_L, (0, -120, 200), (0, -170, 286), 40, 12)
    g.box(TEAL, (0, -122, 340), (5, 5, 80))
    g.box(WHITE, (0, -124, 384), (40, 5, 28), rot=(20, 0, 0))
    fig(sc, r, 0, -160, 300, yaw=180, detail=1, sleeves='long', wear='coat', tops=(TEAL,), bots=(WHITE,), arm_l='vee', arm_r='vee', hold_r=hold_stick(WHITE, 24),
        swing_l=14, swing_r=-14, rate=0.7)


# ========================================================================================== The Garages
def lift_lunch(sc):
    """In an alley's mouth: a four-post car lift raised 2.1 m, no car on it: the mechanics' lunch, in white overalls."""
    r = random.Random(714)
    g = sc.still()
    T = 216.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(BLUE, (sx * 244, sy * 112, 120), (18, 18, 440))
            g.box(WHITE, (sx * 244, sy * 112, 345), (22, 22, 14))
        g.box(SILVER, (sx * 244, 0, T - 24), (14, 224, 16))
    for sy in (-1, 1):
        g.box(WHITE, (0, sy * 72, T - 6), (500, 64, 12))
        g.box('#ffe28a', (0, sy * 72 - 30, T - 6), (500, 5, 13))
        g.box('#ffe28a', (0, sy * 72 + 30, T - 6), (500, 5, 13))
    for x in (-150.0, 0.0, 150.0):
        g.box(SILVER, (x, 0, T - 18), (12, 150, 10))
    for k in range(3):
        g.cyl(TYRE, (186, 72, T + k * 21), 33, 21, steps=8)
        g.cyl(SILVER, (186, 72, T + k * 21 + 18), 15, 4, steps=6)
    g.cyl(BLUE, (-196, 118, -60), 30, 150, steps=8)                        # an oil drum, blue with a white band
    g.cyl(WHITE, (-196, 118, 40), 31.5, 16, steps=8)
    g.box(CREAM, (-60, 76, T + 10), (32, 14, 20))                          # the wireless
    g.box(SILVER, (-48, 76, T + 36), (4, 4, 34))
    ov = dict(tops=(WHITE,), bots=(WHITE,), sleeves='long')
    fig(sc, r, -150, -78, T - 46, legs='sit', arm_l='hold', arm_r='hold', hold_r=hold_sheet(CREAM, 22, 14), hat='cap', hat_col=BLUE, **ov)
    g.box(WHITE, (-98, -72, T + 8), (26, 18, 16))
    g.box(BLUE, (-98, -72, T + 17.5), (10, 5, 4))
    fig(sc, r, -30, -78, T - 46, legs='sit', arm_r='face', hold_r=P.hold_cup(WHITE), hat='cap', hat_col=BLUE, rate=0.4, swing_r=6, axis_r='x', react=True, **ov)
    fig(sc, r, 90, -78, T - 46, legs='sit', arm_r='wave', swing_r=16, rate=0.9, react=True, build='stocky', band=BLUE, **ov)
    fig(sc, r, 60, 70, T, arm_l='hold', arm_r='hold', hold_r=hold_sheet(CREAM, 24, 18), hat='cap', hat_col=BLUE, rate=0.7, swing_r=8, axis_r='x', **ov)
    fig(sc, r, -160, 72, T, arm_l='hip', arm_r='up', tops=(BLUE,), bots=(WHITE,), band=WHITE, swing_r=10, rate=0.8, react=True)   # the valet, the keys held up
    ladder(g, 290, -150 + 40, -112, -40, T, col=SILVER)


# ========================================================================================== Theatre Row, Penthouse Row
def _music_stand(g, x, y, z):
    g.box(SILVER, (x, y, z + 55), (4, 4, 110))
    g.box(WHITE, (x, y - 3, z + 118), (34, 4, 26))


def tuning_balcony(sc):
    """A plum balcony 7 m long on brackets, for a glass infill's face 6 m up (z = 0 is its deck): harp, cello, violin
    and flute tuning; a row of warm bulbs under the rail (steady)."""
    r = random.Random(715)
    g = sc.still()
    L = 700.0
    g.box(PLUM, (0, 0, -9), (L, 130, 18))
    g.box(LILAC, (0, -66.5, -9), (L - 20, 4, 8))
    for k in range(4):
        x = -300 + k * 200.0
        tilt_box(g, PLUM, (x, -40, -18), (x, 62, -110), 14, 12)
        g.box(PLUM, (x, 58, -64), (14, 12, 92))
    for k in range(11):
        x = -L / 2.0 + 20 + k * (L - 40) / 10.0
        g.cyl(BLUSH, (x, -56, 0), 8, 74, steps=5, top_r=5)
        g.box(WARM_L, (x, -68, -24), (11, 6, 11), glow=G_BULB)
    g.box(SILVER, (0, -56, 78), (L, 12, 8))
    for sx in (-1, 1):
        g.sphere(MAG, (sx * (L / 2.0 - 12), -56, 96), 13, steps=(3, 6))
    mus = dict(tops=(MAG, LILAC, WARM_L, BLUSH), bots=(PLUM, '#e8e0f0'))
    # the harp: a tall frame, the player seated, hands going over the strings
    hx = -250.0
    tilt_box(g, LILAC, (hx - 30, 10, 6), (hx - 44, 10, 170), 9, 9)
    tilt_box(g, LILAC, (hx - 44, 10, 170), (hx + 36, 10, 132), 9, 9)
    tilt_box(g, SILVER, (hx + 36, 10, 132), (hx - 22, 10, 10), 5, 5)
    g.box(PLUM, (hx - 16, 10, 8), (50, 22, 12))
    g.box(PLUM, (hx + 40, 24, 22), (36, 36, 44))
    fig(sc, r, hx + 40, 24, 44 - 46, yaw=-20, legs='sit', arm_l='fwd', arm_r='fwd', wear='dress', hair='long', rate=0.5, swing_l=9, swing_r=-9, axis_l='x', axis_r='x', **mus)
    # the cello
    cx = -70.0
    g.box(PLUM, (cx, 26, 22), (36, 36, 44))
    g.box(WOOD, (cx - 6, -8, 62), (36, 16, 70))
    g.box(WOOD_D, (cx - 6, -8, 124), (7, 7, 60))
    g.box(SILVER, (cx - 6, -8, 16), (4, 4, 30))
    fig(sc, r, cx, 26, 44 - 46, legs='sit', arm_l='face', arm_r='carry', hold_r=hold_stick(WHITE, 46), rate=0.6, swing_r=11, axis_r='z', **mus)
    _music_stand(g, cx + 70, -20, 0)
    # the violin under the chin, the bow arm working
    fig(sc, r, 110, 14, 0, yaw=14, arm_l='face', arm_r=((70, 60), (118, 96)), hold_l=hold_sheet(WOOD, 16, 12, hand=-1), hold_r=hold_stick(WHITE, 40), rate=0.7,
        swing_r=10, axis_r='z', wear='skirt', **mus)
    # the flute held across
    fig(sc, r, 250, 14, 0, yaw=-10, arm_l='face', arm_r='face', hat='beanie', hat_col=LILAC, **mus)
    g.box(SILVER, (268, -6, 147), (56, 5, 5))
    _music_stand(g, 190, -22, 0)


# ========================================================================================== Wall Street
def clerks_ledge(sc):
    """A sandstone ledge 7 m long on fat corbels, for a glass infill's face 6 m up (z = 0 is its deck): a bell rung
    for the close, two clerks over a ledger, a runner mid-stride, the tea trolley."""
    r = random.Random(716)
    g = sc.still()
    L = 700.0
    g.box(SAND, (0, 0, -11), (L, 130, 22))
    g.box(CREAM, (0, -66.5, -11), (L - 20, 4, 9))
    for k in range(4):
        x = -300 + k * 200.0
        g.box(TAN, (x, 20, -44), (40, 90, 44))
        g.box(TAN, (x, 44, -84), (34, 42, 40))
    for k in range(8):
        g.box(COPPER, (-L / 2.0 + 10 + k * (L - 20) / 7.0, -58, 40), (7, 7, 80))
    g.box(COPPER, (0, -58, 82), (L, 9, 7))
    g.box(COPPER, (0, -58, 40), (L, 5, 5))
    for sx in (-1, 1):
        g.sphere(SAND, (sx * (L / 2.0 - 14), -58, 104), 17, scale=(1, 1, 1.25), steps=(4, 6))
        g.cyl(COPPER, (sx * (L / 2.0 - 14), -58, 122), 9, 16, steps=5, top_r=2)
    st = dict(tops=(CREAM, WHITE, '#f1e4c8'), bots=(TAN, '#5a4a3c', '#7a8fb8'), sleeves='long')
    fig(sc, r, -280, 10, 0, yaw=8, arm_r='wave', arm_l='hip', hold_r=hold_bell(), band=GREEN_D, rate=1.0, swing_r=14, hat='peak', hat_col=COPPER, **st)
    fig(sc, r, -160, 14, 0, yaw=50, arm_l='hold', arm_r='hold', hold_r=hold_sheet(GREEN_D, 44, 30, band=CREAM), band=TAN, **st)
    fig(sc, r, -92, 10, 0, yaw=-50, arm_r='point', arm_l='down', rate=0.5, swing_r=6, band=GREEN_D, hair='bald', **st)
    fig(sc, r, 40, 6, 0, yaw=-90, legs='stride', arm_l='back', arm_r='fwd', hold_r=hold_sheet(WHITE, 10, 26), band=TAN, hat='cap', hat_col=COPPER, **st)
    # the tea trolley
    tx = 210.0
    g.box(CREAM, (tx, 16, 62), (90, 46, 6))
    g.box(CREAM, (tx, 16, 24), (90, 46, 6))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(COPPER, (tx + sx * 41, 16 + sy * 19, 36), (5, 5, 62))
        g.cyl(GREY_D, (tx + sx * 36, 16, 0), 8, 9, steps=6)
    g.cyl(SILVER, (tx - 20, 16, 65), 14, 32, steps=7)
    g.cyl(SILVER, (tx - 20, 16, 97), 9, 9, steps=6, top_r=3)
    for k in range(3):
        g.cyl(WHITE, (tx + 12 + k * 13, 10, 65), 5, 8, steps=5)
    fig(sc, r, tx + 78, 16, 0, yaw=-90, arm_l='push', arm_r='push', wear='skirt', hair='bun', tops=(WHITE,), bots=(COPPER,), band=CREAM, rate=0.3, turn=4)


JOBS = [('SM_S1PEOPLE_PressRiser', press_riser, SLOW), ('SM_S1PEOPLE_WallSitters', wall_sitters, SLOW),
        ('SM_S1PEOPLE_PlinthBarrel', plinth_barrel, SLOW), ('SM_S1PEOPLE_UmpireChair', umpire_chair, SLOW),
        ('SM_S1PEOPLE_NewsStack', news_stack, SLOW), ('SM_S1PEOPLE_StandCrates', stand_crates, SLOW),
        ('SM_S1PEOPLE_StandLadders', stand_ladders, SLOW), ('SM_S1PEOPLE_PassQueue', pass_queue, SLOW),
        ('SM_S1PEOPLE_GirderLunch', girder_lunch, SLOW), ('SM_S1PEOPLE_TaiChi', tai_chi, SLOW),
        ('SM_S1PEOPLE_StoryTerrace', story_terrace, SLOW), ('SM_S1PEOPLE_EaselPergola', easel_pergola, SLOW),
        ('SM_S1PEOPLE_ChoirSteps', choir_steps, SLOW), ('SM_S1PEOPLE_LiftLunch', lift_lunch, SLOW),
        ('SM_S1PEOPLE_TuningBalcony', tuning_balcony, SLOW), ('SM_S1PEOPLE_ClerksLedge', clerks_ledge, SLOW)]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
