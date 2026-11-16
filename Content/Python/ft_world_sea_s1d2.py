"""Under the Sea, Stage 1, builder s1d2: district 2 "The kelp forest", 1.7 to 3.4 km: the kelp farmers' stilt village.
/Game/World/UnderTheSea/SM_S1D2_*. Run: import ft_world_sea_s1d2 as K; K.run()

Tall thin plank houses on long stilts with steep weed-thatched A-roofs (sun yellow: the district's signal), rope
bridges, float-glass lanterns, the farmers' gear, and THE REEL, the harvesting wheel. Every piece is one
ft_people.Scene (M_People, one section); the hero is saved with the always-moving master. Everything stands on
piles (a footing below z = 0): the sand falls away beside the banked bends.
Palette (the plan's): sun yellow, straw, dark timber, pale, deep green trim; the kit's kelp greens for cut weed only;
pale glass for the floats. No words, no marks, nothing of any other world.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

YEL, STRAW, TIM, PALE, GRN = '#ffd23f', '#c9b44a', '#6a4c36', '#f1fbe6', '#1f5a3a'
TIMD = '#4a3426'                 # timber in shadow: doors, ropes' ties, recesses
KELP, KELPD, KELPL = '#3f8a4a', '#2a6a3a', '#7fcf6a'      # the kit's weed colours (cut weed only)
GLASS = '#cfeeff'                # float glass (the kit's bubble colour): the only thing that glows, with the windows
WIN = '#fff1c0'
FOOT = 320.0                     # how far the piles go under z = 0


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def gable_roof(g, cx, cy, z0, w, d, h, over=45.0, col=YEL):
    """A steep A-roof, ridge along Y (the gable faces the road), thatch hanging past the eaves, green barge boards."""
    g.side(col, ccw([(-w / 2.0 - over, z0), (w / 2.0 + over, z0), (0.0, z0 + h)]), -d / 2.0 - over, d / 2.0 + over, loc=(cx, cy, 0.0))
    for s in (-1, 1):
        g.box(STRAW, (cx + s * (w / 2.0 + over - 8.0), cy, z0 - 22.0), (18.0, d + 2 * over - 10.0, 50.0))
        tilt_box(g, GRN, (cx + s * (w / 2.0 + over), cy - d / 2.0 - over - 4.0, z0), (cx, cy - d / 2.0 - over - 4.0, z0 + h), 8.0, 26.0)
    g.box(TIM, (cx, cy, z0 + h + 2.0), (18.0, d + 2 * over + 40.0, 18.0))


def long_roof(g, cx, cy, z0, L, d, h, over=45.0, col=YEL):
    """The same roof with its ridge along X (a long shed): two thatch slabs, open gables with green rafters."""
    for s in (-1, 1):
        tilt_box(g, col, (cx, cy + s * (d / 2.0 + over), z0), (cx, cy, z0 + h), L, 18.0)
        g.box(STRAW, (cx, cy + s * (d / 2.0 + over - 8.0), z0 - 22.0), (L - 10.0, 18.0, 50.0))
        for e in (-1, 1):
            tilt_box(g, GRN, (cx + e * (L / 2.0 + 3.0), cy + s * (d / 2.0 + over), z0), (cx + e * (L / 2.0 + 3.0), cy, z0 + h), 8.0, 26.0)
    g.box(TIM, (cx, cy, z0 + h + 2.0), (L + 40.0, 18.0, 18.0))


def strands(sc, x, y0, y1, z, n, seed, along_x=False, rate=0.22):
    """Loose weed hanging from an eave, swaying together. The eave runs along Y (or X): they swing across it."""
    r = random.Random(seed)
    f = sc.part(P.motion(swing=6.0, pivot=((y0 + y1) / 2.0 if along_x else x, x if along_x else (y0 + y1) / 2.0, z),
                         axis=0.0 if along_x else 90.0, rate=rate, phase=r.random()))
    for k in range(n):
        t = y0 + (y1 - y0) * (k + 0.5) / n
        ln = r.uniform(60.0, 150.0)
        c = (KELP, STRAW, KELPD)[k % 3]
        if along_x:
            f.box(c, (t, x, z - ln / 2.0), (26.0, 7.0, ln))
        else:
            f.box(c, (x, t, z - ln / 2.0), (7.0, 26.0, ln))


def window(g, x, y, z, w=70.0, h=90.0, lit=0.0):
    """A window on a front (-Y) wall at y: the pane, two green shutters, a sill."""
    g.box(WIN if lit else TIMD, (x, y - 3.0, z), (w, 8.0, h), glow=lit)
    for s in (-1, 1):
        g.box(GRN, (x + s * (w / 2.0 + 12.0), y - 4.0, z), (20.0, 8.0, h + 8.0))
    g.box(TIM, (x, y - 8.0, z - h / 2.0 - 6.0), (w + 50.0, 18.0, 10.0))


def side_window(g, x, y, z, w=80.0, h=100.0, lit=0.0):
    g.box(WIN if lit else TIMD, (x, y, z), (8.0, w, h), glow=lit)


def door(g, x, y, z, w=90.0, h=200.0, col=GRN):
    g.box(col, (x, y - 3.0, z + h / 2.0), (w, 8.0, h))
    g.box(TIM, (x, y - 5.0, z + h + 8.0), (w + 30.0, 12.0, 14.0))
    g.box(YEL, (x + w * 0.3, y - 8.0, z + h * 0.5), (9.0, 6.0, 9.0))


def ladder(g, a, b, w=56.0, rungs=8, col=TIM):
    """a: the foot, b: the top (the middle of the ladder); the rails stand w apart across X."""
    for s in (-1, 1):
        tilt_box(g, col, (a[0] + s * w / 2.0, a[1], a[2]), (b[0] + s * w / 2.0, b[1], b[2]), 8.0, 8.0)
    for k in range(rungs):
        t = (k + 0.5) / rungs
        g.box(col, (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t), (w, 7.0, 7.0))


def lantern(m, x, y, z, r=30.0, glow=0.6):
    """A float-glass lantern hanging from (x, y, z): a cord, a small yellow hat, the glass float in a net."""
    m.box(TIMD, (x, y, z - 14.0), (5.0, 5.0, 28.0))
    m.cyl(YEL, (x, y, z - 36.0), r * 0.95, 14.0, steps=6, top_r=r * 0.3)
    cz = z - 34.0 - r * 0.85
    m.sphere(GLASS, (x, y, cz), r, steps=(3, 6), glow=glow)
    m.box(GRN, (x, y, cz), (r * 2.1, 5.0, 5.0))
    m.box(GRN, (x, y, cz), (5.0, r * 2.1, 5.0))


def hung_lantern(sc, x, y, z, r=30.0, glow=0.6, seed=0, across_x=True):
    f = sc.part(P.motion(swing=5.0, pivot=(x, y, z), axis=90.0 if across_x else 0.0, rate=0.3, phase=(seed * 0.37) % 1.0))
    lantern(f, x, y, z, r, glow)


def vane(sc, x, y, z, seed=0):
    """A weather vane on a short mast: an arrow with a weed-leaf tail, turning to and fro."""
    g = sc.still()
    g.box(TIM, (x, y, z + 60.0), (8.0, 8.0, 120.0))
    f = sc.part(P.motion(swing=38.0, pivot=(x, y, z + 110.0), axis=P.UP, rate=0.11, phase=(seed * 0.29) % 1.0))
    f.box(GRN, (x, y, z + 112.0), (110.0, 5.0, 9.0))
    f.box(YEL, (x - 50.0, y, z + 112.0), (34.0, 5.0, 36.0))
    f.box(YEL, (x + 58.0, y, z + 112.0), (16.0, 6.0, 18.0))


def pennant(sc, x, y, z, cols, seg=150.0, h=46.0, n=3, seed=0):
    """A long weed pennant from a pole top at (x, y, z), streaming along +X, rippling (each length a beat behind)."""
    for k in range(n):
        f = sc.part(P.motion(swing=9.0 + 3.0 * k, pivot=(x, y, z), axis=P.UP, rate=0.35, phase=(seed * 0.21 - 0.13 * k) % 1.0))
        f.box(cols[k % len(cols)], (x + seg * (k + 0.5), y, z - 6.0 * k), (seg + 4.0, 5.0, h - 9.0 * k))


def hoist(sc, x, y, z, reach_y, drop, seed=0, load=STRAW):
    """A hoist beam sticking out of a wall towards -Y, its rope and a small bale turning slowly on it."""
    g = sc.still()
    g.box(TIM, (x, y - reach_y / 2.0, z), (16.0, reach_y, 16.0))
    px, py = x, y - reach_y + 12.0
    f = sc.part(P.motion(swing=4.0, pivot=(px, py, z), axis=0.0, rate=0.2, phase=(seed * 0.41) % 1.0))
    f.box(TIMD, (px, py, z - drop / 2.0), (5.0, 5.0, drop))
    f.box(load, (px, py, z - drop - 36.0), (80.0, 66.0, 72.0))
    f.box(TIMD, (px, py, z - drop - 36.0), (84.0, 12.0, 76.0))


# ------------------------------------------------------------------------------------------ the buildings
def stilt_house(sc):
    """The farmer's house: three storeys on four long stilts, each storey a little wider, a porch, the long ladder,
    weed sheets drying under the deck, a hoist in the gable, a steep yellow roof. 6.9 x 6.8 m, about 27.5 m tall."""
    g = sc.still()
    T = 1100.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, TIM, (sx * 330, sy * 300, -FOOT), (sx * 250, sy * 220, T), 30.0, 30.0)
    tilt_box(g, TIMD, (-305, -275, 250), (258, -228, 900), 12.0, 12.0)
    tilt_box(g, TIMD, (305, -275, 250), (-258, -228, 900), 12.0, 12.0)
    tilt_box(g, TIMD, (-305, 275, 250), (258, 228, 900), 12.0, 12.0)
    g.box(TIM, (0, -30, T + 12), (700, 680, 24))
    for x in (-340, -120, 120, 340):
        g.box(TIM, (x, -360, T + 70), (10, 10, 92))
    g.box(GRN, (0, -360, T + 118), (700, 12, 10))
    z0 = T + 24
    g.box(PALE, (0, 20, z0 + 160), (520, 460, 320))
    g.box(TIM, (0, 20, z0 + 328), (560, 500, 16))
    g.box(STRAW, (0, 10, z0 + 496), (560, 500, 320))
    g.box(TIM, (0, 10, z0 + 664), (600, 540, 16))
    g.box(PALE, (0, 0, z0 + 822), (600, 540, 300))
    for s in (-1, 1):
        g.box(GRN, (s * 256, -212, z0 + 160), (14, 8, 320))
        g.box(GRN, (s * 296, -272, z0 + 822), (14, 8, 300))
    door(g, -120, -210, z0)
    window(g, 120, -210, z0 + 180, lit=0.35)
    window(g, -140, -240, z0 + 506)
    window(g, 140, -240, z0 + 506, lit=0.35)
    window(g, 0, -270, z0 + 822, w=90, lit=0.35)
    side_window(g, 283, 10, z0 + 500)
    side_window(g, -283, 10, z0 + 500, lit=0.35)
    side_window(g, 303, 0, z0 + 822, lit=0.35)
    side_window(g, -303, 0, z0 + 822)
    zr = z0 + 972
    gable_roof(g, 0, 0, zr, 600, 540, 640)
    hoist(sc, 0, -270, zr + 380, 240, 300, seed=1)
    ladder(g, (-250, -430, -100), (-250, -372, T + 20), rungs=10)
    g.cyl(TIMD, (170, 120, zr + 250), 20, 420, steps=6)
    g.cyl(YEL, (170, 120, zr + 670), 36, 16, steps=6, top_r=10)
    g.box(TIM, (40, -250, z0 + 296), (10, 90, 10))
    hung_lantern(sc, 40, -288, z0 + 292, 26, seed=2)
    g.box(TIMD, (0, -236, 820), (480, 5, 5))
    f = sc.part(P.motion(swing=7.0, pivot=(0, -236, 820), axis=0.0, rate=0.25, phase=0.3))
    for k, c in enumerate((STRAW, KELP, STRAW, KELPD)):
        f.box(c, (-180 + k * 120, -236, 820 - 100 - 14 * (k % 2)), (92, 6, 200 + 28 * (k % 2)))
    strands(sc, -337, -300, 300, zr - 40, 4, 11)
    strands(sc, 337, -300, 300, zr - 40, 4, 12)
    vane(sc, 0, -290, zr + 650, seed=3)


def tall_house(sc):
    """The cutters' lodging: four dark plank storeys, each jettied out over the one below (top-heavy), pale bands,
    a lean-to on its own stilts, a balcony, a bucket hoist, a long weed pennant. About 7.6 x 6.1 m, 27.5 m tall."""
    g = sc.still()
    T = 950.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, TIM, (sx * 290, sy * 290, -FOOT), (sx * 200, sy * 200, T), 30.0, 30.0)
    g.box(TIMD, (0, -246, 480), (500, 12, 12))
    g.box(TIMD, (0, 246, 480), (500, 12, 12))
    g.box(TIMD, (-246, 0, 480), (12, 500, 12))
    g.box(TIM, (0, 0, T + 12), (520, 520, 24))
    z0 = T + 24
    ws = (400, 440, 480, 520)
    for k, w in enumerate(ws):
        zc = z0 + k * 310
        g.box(TIM if k != 2 else STRAW, (0, 0, zc + 150), (w, w, 300))
        g.box(PALE, (0, 0, zc + 305), (w + 44, w + 44, 12))
        fy = -w / 2.0
        if k == 0:
            door(g, -90, fy, zc, col=PALE)
            window(g, 100, fy, zc + 170, w=60, lit=0.35)
        elif k == 2:
            window(g, 0, fy, zc + 160, w=110, h=110, lit=0.35)
        else:
            window(g, -110, fy, zc + 160, w=64, lit=0.35 if k == 3 else 0.0)
            window(g, 110, fy, zc + 160, w=64, lit=0.35 if k == 1 else 0.0)
        side_window(g, -w / 2.0 - 3, 0, zc + 160, lit=0.35 if k % 2 else 0.0)
    # the balcony of the third storey
    zb = z0 + 2 * 310
    g.box(TIM, (0, -300, zb + 6), (420, 130, 12))
    for x in (-200, 0, 200):
        g.box(TIM, (x, -358, zb + 52), (10, 10, 84))
    g.box(GRN, (0, -358, zb + 96), (420, 12, 10))
    tilt_box(g, TIMD, (-190, -250, zb - 110), (-190, -350, zb), 10.0, 10.0)
    tilt_box(g, TIMD, (190, -250, zb - 110), (190, -350, zb), 10.0, 10.0)
    # the lean-to, on two stilts of its own
    for sy in (-1, 1):
        tilt_box(g, TIM, (430, sy * 190, -FOOT), (370, sy * 150, T + 30), 24.0, 24.0)
    g.box(TIM, (300, 0, T + 12), (200, 380, 24))
    g.box(PALE, (300, 0, z0 + 120), (190, 340, 240))
    tilt_box(g, YEL, (420, 0, z0 + 230), (205, 0, z0 + 390), 400.0, 16.0)
    g.box(STRAW, (410, 0, z0 + 212), (18, 390, 44))
    side_window(g, 397, 0, z0 + 130, lit=0.35)
    zr = z0 + 4 * 310
    gable_roof(g, 0, 0, zr, 560, 560, 600)
    # the bucket hoist on the left wall
    g.box(TIM, (-330, 60, zr - 60), (220, 16, 16))
    f = sc.part(P.motion(swing=4.0, pivot=(-425, 60, zr - 60), axis=90.0, rate=0.18, phase=0.6))
    f.box(TIMD, (-425, 60, zr - 60 - 330), (5, 5, 660))
    f.cyl(STRAW, (-425, 60, zr - 60 - 720), 34, 60, steps=6, top_r=40)
    ladder(g, (-110, -330, -100), (-110, -268, T + 20), rungs=9)
    # a crooked stove pipe
    g.cyl(TIMD, (-150, 150, zr + 280), 18, 260, steps=6)
    tilt_box(g, TIMD, (-150, 150, zr + 530), (-200, 150, zr + 640), 30.0, 30.0)
    g.cyl(YEL, (-205, 150, zr + 640), 32, 14, steps=6, top_r=8)
    # the pennant pole on the front of the ridge
    g.box(TIM, (0, -300, zr + 660), (10, 10, 220))
    pennant(sc, 0, -300, zr + 745, (KELP, YEL, KELP), seed=4)
    hung_lantern(sc, -30, -372, zb - 4, 26, seed=5)
    strands(sc, -317, -300, 300, zr - 40, 4, 13)
    strands(sc, 317, -300, 300, zr - 40, 4, 14)


def drying_barn(sc):
    """The drying barn: a long open shed on six stilts, slatted sides, the weed hung inside in rows that sway,
    one end boarded with a loading door and a hoist. 14.6 m along the road, about 22 m tall."""
    g = sc.still()
    T = 1350.0
    for x in (-560, 0, 560):
        for sy in (-1, 1):
            tilt_box(g, TIM, (x * 1.12, sy * 270, -FOOT), (x, sy * 210, T), 30.0, 30.0)
    tilt_box(g, TIMD, (-620, -262, 220), (-8, -214, 1180), 12.0, 12.0)
    tilt_box(g, TIMD, (620, -262, 220), (8, -214, 1180), 12.0, 12.0)
    tilt_box(g, TIMD, (-620, 262, 220), (-8, 214, 1180), 12.0, 12.0)
    tilt_box(g, TIMD, (620, 262, 220), (8, 214, 1180), 12.0, 12.0)
    g.box(TIM, (0, 0, T + 12), (1380, 560, 24))
    z0 = T + 24
    for x in (-640, 0, 640):
        for sy in (-1, 1):
            g.box(TIM, (x, sy * 243, z0 + 200), (18, 18, 400))
    for sy in (-1, 1):
        for k, z in enumerate((70, 200, 330)):
            g.box(STRAW if k != 1 else PALE, (0, sy * 250, z0 + z), (1300, 8, 36))
        g.box(TIM, (0, sy * 243, z0 + 406), (1320, 14, 14))
    g.box(PALE, (-648, 0, z0 + 200), (10, 480, 400))
    g.box(GRN, (-655, 0, z0 + 130), (8, 150, 240))
    for k in range(6):
        x = -470 + k * 200
        f = sc.part(P.motion(swing=5.0, pivot=(x, 0, z0 + 372), axis=90.0, rate=0.22, phase=(k * 0.17) % 1.0))
        f.box((KELP, STRAW, KELPD)[k % 3], (x, 0, z0 + 212 + 10 * (k % 2)), (10, 400, 300 - 20 * (k % 2)))
    long_roof(g, 0, 0, z0 + 406, 1400, 500, 400)
    # the loading hoist over the door
    g.box(TIM, (-760, 0, z0 + 480), (260, 16, 16))
    f = sc.part(P.motion(swing=4.0, pivot=(-870, 0, z0 + 480), axis=90.0, rate=0.2, phase=0.45))
    f.box(TIMD, (-870, 0, z0 + 480 - 220), (5, 5, 440))
    f.box(STRAW, (-870, 0, z0 + 480 - 480), (90, 80, 80))
    f.box(TIMD, (-870, 0, z0 + 480 - 480), (94, 12, 84))
    ladder(g, (-600, -345, -100), (-600, -288, T + 20), rungs=11)
    hung_lantern(sc, -330, -300, z0 + 384, 26, seed=6, across_x=False)
    hung_lantern(sc, 330, -300, z0 + 384, 26, seed=7, across_x=False)
    for k, x in enumerate((180, 300, 430)):
        g.box(STRAW, (x, -255, z0 + 30), (90, 50, 60))
        g.box(TIMD, (x, -255, z0 + 30), (12, 54, 64))
    strands(sc, -292, -660, 660, z0 + 366, 7, 15, along_x=True)
    vane(sc, 690, 0, z0 + 815, seed=8)


def mast_house(sc):
    """The look-out: a small two-storey house under a pyramid roof with a mast straight through it, a crow's nest
    30 m up, a ladder of pegs, stays, a pennant and a lantern at the top. About 6 x 6 m, 36 m tall."""
    g = sc.still()
    T = 1000.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, TIM, (sx * 280, sy * 280, -FOOT), (sx * 190, sy * 190, T), 28.0, 28.0)
    tilt_box(g, TIMD, (-262, -262, 200), (196, -196, 900), 12.0, 12.0)
    tilt_box(g, TIMD, (262, -262, 200), (-196, -196, 900), 12.0, 12.0)
    g.box(TIM, (0, -30, T + 12), (500, 560, 24))
    for x in (-240, 0, 240):
        g.box(TIM, (x, -300, T + 66), (10, 10, 84))
    g.box(GRN, (0, -300, T + 110), (500, 12, 10))
    z0 = T + 24
    g.box(PALE, (0, 20, z0 + 150), (400, 400, 300))
    g.box(TIM, (0, 20, z0 + 306), (440, 440, 12))
    g.box(STRAW, (0, 20, z0 + 452), (440, 440, 280))
    door(g, -80, -180, z0)
    window(g, 100, -180, z0 + 170, w=60, lit=0.35)
    window(g, 0, -200, z0 + 460, w=100, lit=0.35)
    side_window(g, 223, 20, z0 + 460, lit=0.35)
    side_window(g, -223, 20, z0 + 460)
    zr = z0 + 592
    g.cyl(YEL, (0, 20, zr), 400.0, 430.0, rot=(0, 45, 0), steps=4, top_r=30.0)
    for s in (-1, 1):
        g.box(STRAW, (s * 268, 20, zr - 20), (18, 560, 46))
        g.box(STRAW, (0, 20 + s * 268, zr - 20), (520, 18, 46))
    # the mast, its pegs and the crow's nest
    tilt_box(g, TIM, (0, 20, zr + 200), (0, 20, 3420), 30.0, 30.0)
    for k in range(8):
        g.box(TIMD, (38 if k % 2 else -38, 20, zr + 560 + k * 135), (60, 8, 8))
    zn = 3000.0
    g.cyl(TIM, (0, 20, zn - 12), 125, 12, steps=8)
    g.cyl(STRAW, (0, 20, zn), 112, 100, steps=8, top_r=122)
    g.cyl(GRN, (0, 20, zn + 100), 126, 12, steps=8)
    g.cyl(YEL, (0, 20, zn + 300), 150, 110, steps=6, top_r=14)
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, TIMD, (sx * 96, 20 + sy * 96, zn), (sx * 250, 20 + sy * 250, zr + 20), 5.0, 5.0)
    tilt_box(g, PALE, (60, -80, zn + 130), (130, -170, zn + 160), 14.0, 14.0)      # the spyglass on the rim
    hung_lantern(sc, 0, -96, zn + 300, 30, seed=9)
    pennant(sc, 0, 20, 3390, (YEL, KELP, YEL), seg=130, h=42, seed=10)
    ladder(g, (-170, -372, -100), (-170, -312, T + 20), rungs=9)
    g.box(TIM, (150, -214, z0 + 290), (10, 70, 10))
    hung_lantern(sc, 150, -244, z0 + 286, 24, seed=11)
    strands(sc, -268, -220, 260, zr - 40, 3, 16)
    strands(sc, 268, -220, 260, zr - 40, 3, 17)


def rope_walk(sc):
    """The rope walk: a very long open shed on eight stilts where the weed is twisted into rope. The strands run
    its whole length, a twisting wheel rocks at one end, coils wait at the other, finished hanks hang from the
    eave. 27.4 m along the road, about 19 m tall."""
    g = sc.still()
    T = 1300.0
    xs = (-1150, -380, 380, 1150)
    for x in xs:
        for sy in (-1, 1):
            tilt_box(g, TIM, (x * 1.04, sy * 215, -FOOT), (x, sy * 150, T), 28.0, 28.0)
    for x0, x1 in ((-1196, -380), (1196, 380)):
        tilt_box(g, TIMD, (x0, -205, 220), (x1, -152, 1160), 12.0, 12.0)
        tilt_box(g, TIMD, (x0, 205, 220), (x1, 152, 1160), 12.0, 12.0)
    g.box(TIM, (0, 0, T + 12), (2660, 420, 24))
    z0 = T + 24
    for x in (-1290, -430, 430, 1290):
        for sy in (-1, 1):
            g.box(TIM, (x, sy * 180, z0 + 140), (14, 14, 280))
    for sy in (-1, 1):
        g.box(TIM, (0, sy * 180, z0 + 286), (2620, 12, 12))
        g.box(GRN, (0, sy * 196, z0 + 60), (2620, 8, 14))
    long_roof(g, 0, 0, z0 + 292, 2700, 360, 260, over=40.0)
    for k, (y, z) in enumerate(((-90, 120), (-30, 160), (30, 120), (90, 160))):
        tilt_box(g, (STRAW, PALE, STRAW, KELPL)[k], (-1240, 0, z0 + 150), (1180, y, z0 + z), 9.0, 9.0)
    # the twisting wheel at the -X end
    g.box(TIM, (-1262, 0, z0 + 90), (20, 60, 180))
    f = sc.part(P.motion(swing=170.0, pivot=(-1248, 0, z0 + 150), axis=0.0, rate=0.05, phase=0.2))
    f.box(GRN, (-1248, 0, z0 + 150), (12, 230, 16))
    f.box(GRN, (-1248, 0, z0 + 150), (12, 16, 230))
    f.box(YEL, (-1244, 0, z0 + 150), (16, 40, 40))
    # the traveller in the middle, and the coils at the +X end
    g.box(TIM, (300, 0, z0 + 40), (130, 220, 60))
    g.box(TIM, (300, 0, z0 + 110), (16, 220, 90))
    for k in range(3):
        g.cyl(STRAW, (1130, -40, z0 + k * 24), 62 - 6 * k, 22, steps=8)
    g.cyl(PALE, (1010, 70, z0), 50, 24, steps=8)
    g.cyl(PALE, (1010, 70, z0 + 26), 42, 22, steps=8)
    f = sc.part(P.motion(swing=5.0, pivot=(0, -206, z0 + 262), axis=0.0, rate=0.24, phase=0.7))
    for k in range(6):
        f.box(STRAW if k % 2 else PALE, (-950 + k * 380, -206, z0 + 262 - 62), (18, 12, 120))
    ladder(g, (-800, -300, -100), (-800, -226, T + 20), rungs=10)
    for k, x in enumerate((-640, 0, 640)):
        hung_lantern(sc, x, -216, z0 + 250, 24, seed=12 + k, across_x=False)
    vane(sc, 1330, 0, z0 + 560, seed=14)


def press_house(sc):
    """The press house: a stout two-storey house on six thick stilts with a flat roof, and on the roof the great
    screw press under its own small yellow canopy: a vat of weed, the screw, capstan bars going round and round.
    About 7.2 x 6.4 m, 24 m tall."""
    g = sc.still()
    T = 1000.0
    for x in (-260, 0, 260):
        for sy in (-1, 1):
            tilt_box(g, TIM, (x * 1.2, sy * 290, -FOOT), (x, sy * 220, T), 36.0, 36.0)
    tilt_box(g, TIMD, (-306, -282, 200), (-4, -226, 900), 14.0, 14.0)
    tilt_box(g, TIMD, (306, -282, 200), (4, -226, 900), 14.0, 14.0)
    g.box(TIM, (0, -20, T + 12), (680, 640, 24))
    z0 = T + 24
    g.box(TIM, (0, 20, z0 + 150), (600, 520, 300))
    g.box(PALE, (0, 20, z0 + 308), (640, 560, 16))
    g.box(PALE, (0, 20, z0 + 456), (600, 520, 280))
    g.box(TIM, (0, 20, z0 + 606), (660, 580, 20))
    door(g, 0, -240, z0, w=110, col=PALE)
    window(g, -190, -240, z0 + 170, w=60, lit=0.35)
    window(g, 190, -240, z0 + 170, w=60)
    window(g, -150, -240, z0 + 456, lit=0.35)
    window(g, 150, -240, z0 + 456, lit=0.35)
    side_window(g, 303, 20, z0 + 456, lit=0.35)
    side_window(g, -303, 20, z0 + 160)
    # the awning over the door
    tilt_box(g, YEL, (0, -360, z0 + 215), (0, -242, z0 + 262), 240.0, 10.0)
    for s in (-1, 1):
        tilt_box(g, TIM, (s * 110, -352, z0 + 216), (s * 110, -244, z0 + 150), 7.0, 7.0)
    z1 = z0 + 616
    for x in (-320, 0, 320):
        g.box(TIM, (x, 300, z1 + 45), (10, 10, 90))
    g.box(GRN, (0, 300, z1 + 92), (660, 12, 10))
    for s in (-1, 1):
        g.box(GRN, (s * 322, 30, z1 + 92), (12, 540, 10))
        g.box(TIM, (s * 322, -240, z1 + 45), (10, 10, 90))
    # the press
    for s in (-1, 1):
        g.box(TIM, (s * 175, 20, z1 + 215), (30, 30, 430))
    g.box(TIM, (0, 20, z1 + 440), (420, 44, 44))
    g.cyl(TIM, (0, 20, z1), 122, 112, steps=10)
    g.cyl(GRN, (0, 20, z1 + 70), 126, 12, steps=10)
    g.sphere(KELP, (0, 20, z1 + 116), 104, scale=(1, 1, 0.45), steps=(3, 6))
    g.cyl(STRAW, (0, 20, z1 + 170), 104, 18, steps=10)
    g.cyl(PALE, (0, 20, z1 + 186), 22, 380, steps=6)
    for k in range(3):
        g.cyl(STRAW, (0, 20, z1 + 240 + k * 60), 34, 12, steps=6)
    f = sc.part(P.motion(travel=(-1.0, 0.05), base=(0.0, 20.0)))
    f.box(TIM, (0, 20, z1 + 540), (470, 16, 16))
    f.box(TIM, (0, 20, z1 + 540), (16, 470, 16))
    for s in (-1, 1):
        f.box(YEL, (s * 238, 20, z1 + 540), (22, 24, 24))
        f.box(YEL, (0, 20 + s * 238, z1 + 540), (24, 22, 24))
    # the canopy over it
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(TIM, (sx * 260, 20 + sy * 190, z1 + 320), (12, 12, 640))
    gable_roof(g, 0, 20, z1 + 640, 520, 380, 220, over=30.0)
    # the spout and the barrels of juice
    tilt_box(g, TIMD, (110, 20, z1 + 30), (262, -150, z1 + 120), 14.0, 14.0)
    for k, (x, y) in enumerate(((262, -170), (-250, -170))):
        g.cyl(TIM, (x, y, z1), 46, 96, steps=8)
        g.cyl(GRN, (x, y, z1 + 44), 49, 10, steps=8)
    ladder(g, (250, -400, -100), (250, -342, T + 20), rungs=9)
    hung_lantern(sc, -130, -352, z0 + 210, 24, seed=15)
    hung_lantern(sc, 130, -352, z0 + 210, 24, seed=16)
    strands(sc, -282, -150, 190, z1 + 600, 3, 18)
    strands(sc, 282, -150, 190, z1 + 600, 3, 19)
    g.cyl(TIMD, (-240, 230, z1), 18, 300, steps=6)
    g.cyl(YEL, (-240, 230, z1 + 300), 32, 14, steps=6, top_r=8)


def float_store(sc):
    """The float store: a platform on four stilts with a small hut, a crane, a shelf of glass floats, and under
    the platform the great net bag of floats, swinging gently. About 8 x 6 m, 19 m tall."""
    r = random.Random(33)
    g = sc.still()
    T = 1250.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, TIM, (sx * 340, sy * 290, -FOOT), (sx * 260, sy * 205, T), 30.0, 30.0)
    tilt_box(g, TIMD, (-322, 270, 220), (264, 208, 1120), 12.0, 12.0)
    tilt_box(g, TIMD, (322, 270, 220), (-264, 208, 1120), 12.0, 12.0)
    g.box(TIM, (0, 0, T + 12), (640, 520, 24))
    z0 = T + 24
    g.box(PALE, (40, 90, z0 + 140), (420, 300, 280))
    g.box(GRN, (40, 90, z0 + 286), (450, 330, 12))
    door(g, -60, -60, z0, w=80, h=190)
    window(g, 130, -60, z0 + 160, w=56, h=80, lit=0.35)
    side_window(g, 253, 90, z0 + 160, lit=0.35)
    gable_roof(g, 40, 90, z0 + 292, 420, 300, 330, over=40.0)
    for x in (-310, 0, 310):
        g.box(TIM, (x, -252, z0 + 45), (10, 10, 90))
    g.box(GRN, (0, -252, z0 + 92), (640, 12, 10))
    # the shelf of floats on the deck
    g.box(TIM, (-225, -150, z0 + 30), (150, 110, 10))
    for k, c in enumerate((GLASS, PALE, WIN)):
        g.sphere(c, (-270 + k * 46, -150 + 18 * (k % 2), z0 + 70), 34, steps=(3, 6))
    # the crane
    g.box(TIM, (-270, 190, z0 + 200), (26, 26, 400))
    tilt_box(g, TIM, (-270, 190, z0 + 380), (-560, 190, z0 + 470), 20.0, 20.0)
    tilt_box(g, TIMD, (-270, 190, z0 + 200), (-420, 190, z0 + 422), 10.0, 10.0)
    f = sc.part(P.motion(swing=5.0, pivot=(-545, 190, z0 + 462), axis=90.0, rate=0.2, phase=0.15))
    f.box(TIMD, (-545, 190, z0 + 462 - 190), (5, 5, 380))
    f.sphere(GLASS, (-545, 190, z0 + 462 - 430), 52, steps=(4, 6))
    f.box(GRN, (-545, 190, z0 + 462 - 430), (112, 6, 6))
    f.box(GRN, (-545, 190, z0 + 462 - 430), (6, 112, 6))
    # the net bag under the platform
    f = sc.part(P.motion(swing=2.5, pivot=(0, 0, T), axis=0.0, rate=0.15, phase=0.5))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(f, STRAW, (sx * 170, sy * 130, T), (sx * 120, sy * 100, T - 230), 7.0, 7.0)
            tilt_box(f, GRN, (sx * 150, sy * 125, T - 230), (sx * 60, sy * 50, T - 640), 7.0, 7.0)
    f.cyl(GRN, (0, 0, T - 236), 190, 10, steps=8)
    f.cyl(GRN, (0, 0, T - 440), 236, 8, steps=8)
    cols = (GLASS, PALE, WIN, GLASS, STRAW, PALE, GLASS, WIN, PALE, GLASS)
    spots = ((-95, -70, -330), (95, -60, -340), (0, 85, -335), (-110, 60, -450), (110, 70, -455), (0, -105, -460),
             (0, 0, -420), (-60, -20, -560), (70, 10, -565), (0, 40, -640))
    for k, (x, y, z) in enumerate(spots):
        f.sphere(cols[k], (x, y, T + z), 66 + r.uniform(-8, 8), steps=(4, 6))
    ladder(g, (220, -330, -100), (220, -268, T + 20), rungs=10)
    hung_lantern(sc, -30, -262, z0 + 80, 24, seed=17)
    strands(sc, -213, -60, 240, z0 + 250, 3, 20)
    strands(sc, 293, -60, 240, z0 + 250, 3, 21)
    vane(sc, 40, -70, z0 + 632, seed=18)


# ------------------------------------------------------------------------------------------ the props
def rope_bridge(sc):
    """A rope bridge between two pile towers (TALL: the deck hangs 10 to 12 m up): sagging planks, hand ropes,
    weed caught on it, three float lanterns under it. 20.6 m along the road, 17 m tall."""
    g = sc.still()
    X = 920.0
    for s in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, TIM, (s * (X + 50), sy * 130, -FOOT), (s * X, sy * 90, 1520), 24.0, 24.0)
        tilt_box(g, TIMD, (s * (X + 44), -124, 200), (s * (X + 4), 94, 1100), 10.0, 10.0)
        g.box(TIM, (s * X, 0, 1200), (230, 270, 20))
        g.box(GRN, (s * X, 0, 1420), (40, 240, 12))
        gable_roof(g, s * X, 0, 1520, 200, 220, 160, over=24.0)
    n = 9
    pts = []
    for k in range(n + 1):
        x = -810.0 + 1620.0 * k / n
        pts.append((x, 1210.0 - 180.0 * (1.0 - (x / 810.0) ** 2)))
    for k in range(n):
        (xa, za), (xb, zb) = pts[k], pts[k + 1]
        tilt_box(g, TIM if k % 2 else STRAW, (xa, 0, za), (xb, 0, zb), 110.0, 10.0)
        for sy in (-1, 1):
            tilt_box(g, STRAW, (xa, sy * 60, za + 96), (xb, sy * 60, zb + 96), 6.0, 6.0)
    for k in range(1, n):
        for sy in (-1, 1):
            g.box(TIMD, (pts[k][0], sy * 60, pts[k][1] + 48), (5, 5, 96))
    f = sc.part(P.motion(swing=7.0, pivot=(0, 0, 1040), axis=0.0, rate=0.22, phase=0.1))
    for k, i in enumerate((1, 3, 4, 6, 8)):
        f.box((KELP, KELPD, STRAW)[k % 3], (pts[i][0] + 40, -52 + 26 * k, pts[i][1] - 60 - 14 * (k % 2)), (26, 7, 120 + 28 * (k % 2)))
    for k, i in enumerate((2, 5, 7)):
        hung_lantern(sc, pts[i][0], 0, pts[i][1] - 6, 28, seed=20 + k, across_x=False)


def drying_rack(sc):
    """A drying rack (TALL, 4.7 m): two A-frames, three rails, sheets of weed swaying on them, a basket at its foot."""
    g = sc.still()
    for x in (-330, 330):
        tilt_box(g, TIM, (x, -130, -300), (x, 0, 446), 14.0, 14.0)
        tilt_box(g, TIM, (x, 130, -300), (x, 0, 446), 14.0, 14.0)
        g.box(YEL, (x, 0, 462), (44, 5, 26))
    g.box(TIM, (0, 0, 440), (720, 10, 10))
    g.box(TIM, (0, -50, 262), (700, 10, 10))
    g.box(TIM, (0, 50, 262), (700, 10, 10))
    for half in (0, 1):
        f = sc.part(P.motion(swing=8.0, pivot=(0, 0, 440), axis=0.0, rate=0.3, phase=0.2 + 0.18 * half))
        for k in range(half, 5, 2):
            f.box((STRAW, KELP, STRAW, KELPD, STRAW)[k], (-260 + k * 130, 0, 440 - 118 - 10 * (k % 2)), (112, 6, 230 + 20 * (k % 2)))
    f = sc.part(P.motion(swing=6.0, pivot=(0, -50, 262), axis=0.0, rate=0.27, phase=0.6))
    for k in range(3):
        f.box((KELPD, STRAW, KELP)[k], (-200 + k * 200, -50, 262 - 70), (90, 6, 136))
    g.cyl(STRAW, (250, -150, -60), 38, 110, steps=8, top_r=44)
    g.box(KELP, (250, -150, 62), (50, 40, 26))


def float_lantern(sc):
    """A float lantern on a leaning post (TALL, 5.4 m): two glass floats in nets on bracket arms, glowing steadily,
    a thatch hat on the post, a coil of rope at its foot."""
    g = sc.still()
    tilt_box(g, TIM, (0, 0, -300), (12, 0, 500), 22.0, 22.0)
    g.box(GRN, (5, 0, 180), (30, 30, 14))
    g.box(GRN, (9, 0, 360), (30, 30, 14))
    tilt_box(g, TIM, (11, 0, 468), (124, 0, 522), 10.0, 10.0)
    tilt_box(g, TIM, (8, 0, 372), (-84, 0, 410), 10.0, 10.0)
    g.cyl(YEL, (12, 0, 498), 46, 44, steps=6, top_r=5)
    hung_lantern(sc, 120, 0, 518, 34, glow=0.6, seed=30)
    hung_lantern(sc, -80, 0, 406, 24, glow=0.6, seed=31)
    g.cyl(STRAW, (-40, -50, -60), 44, 84, steps=8)
    g.cyl(STRAW, (-40, -50, 24), 36, 16, steps=8)


def bale_stack(sc):
    """Baled weed (TALL, 4.3 m): four straw-coloured 1.9 m bales tied with dark rope on a plank deck on piles, a
    yellow tarpaulin over the top one, a hay fork leaning on them."""
    g = sc.still()
    g.box(TIM, (0, 10, 40), (470, 430, 14))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(TIM, (sx * 205, 10 + sy * 185, -130), (20, 20, 340))
    B = 190.0
    for (x, y, z, yaw) in ((-100, -90, 47 + B / 2, 0), (100, -86, 47 + B / 2, 0), (10, 112, 47 + B / 2, 0), (-14, -30, 47 + B * 1.5, 14)):
        g.box(STRAW, (x, y, z), (B, B, B), rot=(0, yaw, 0))
        g.box(TIMD, (x, y, z), (B + 6, 12, B + 6), rot=(0, yaw, 0))
        g.box(TIMD, (x, y, z), (12, B + 6, B + 6), rot=(0, yaw, 0))
    zt = 47 + B * 2
    g.box(YEL, (-14, -30, zt + 5), (206, 206, 8), rot=(0, 14, 0))
    g.box(YEL, (-116, -40, zt - 40), (8, 190, 90), rot=(0, 14, 0))
    tilt_box(g, TIM, (236, -210, 40), (150, -118, 330), 8.0, 8.0)
    for s in (-1, 1):
        tilt_box(g, PALE, (150 + s * 12, -118, 330), (128 + s * 14, -92, 396), 5.0, 5.0)


def scythe_rack(sc):
    """The cutters' rack (TALL, 4.4 m): four long scythes leaning on a rail, a ladder beside them, a whetstone tub."""
    g = sc.still()
    for x in (-200, 200):
        g.box(TIM, (x, 0, -15), (16, 16, 570))
        g.cyl(YEL, (x, 0, 270), 20, 26, steps=6, top_r=4)
    g.box(TIM, (0, 0, 250), (480, 12, 12))
    g.box(TIM, (0, -72, 40), (480, 12, 12))
    for k, x in enumerate((-150, -50, 50, 150)):
        tilt_box(g, TIM, (x, -76, 20), (x, 22, 400), 8.0, 8.0)
        tilt_box(g, PALE, (x, 22, 400), (x + 70, 12, 426), 5.0, 18.0)
        tilt_box(g, PALE, (x + 70, 12, 426), (x + 124, 4, 408), 5.0, 12.0)
        g.box(GRN, (x, -30, 200), (12, 12, 10))
    ladder(g, (310, -100, -60), (310, 30, 436), w=50.0, rungs=6)
    g.cyl(STRAW, (-290, -40, -60), 30, 104, steps=8, top_r=34)
    g.box(PALE, (-290, -40, 48), (30, 14, 10))


def bell_post(sc):
    """A rope-bridge end post (TALL, 5.9 m): a thick banded post under its own tiny thatch, the farm's bell swinging
    on the cross beam, a pull rope, and the first planks and hand ropes of a bridge running down behind it."""
    g = sc.still()
    tilt_box(g, TIM, (0, 0, -300), (0, 0, 474), 34.0, 34.0)
    g.box(GRN, (0, 0, 120), (40, 40, 16))
    g.box(GRN, (0, 0, 330), (40, 40, 16))
    g.box(TIM, (0, 0, 430), (230, 20, 20))
    gable_roof(g, 0, 0, 474, 200, 120, 96, over=20.0)
    f = sc.part(P.motion(swing=14.0, pivot=(72, 0, 420), axis=90.0, rate=0.4, phase=0.3))
    f.box(TIMD, (72, 0, 406), (10, 10, 28))
    f.cyl(STRAW, (72, 0, 326), 46, 68, steps=8, top_r=16)
    f.cyl(YEL, (72, 0, 320), 50, 8, steps=8)
    f.box(TIMD, (72, 0, 318), (10, 10, 28))
    f = sc.part(P.motion(swing=3.0, pivot=(-76, 0, 420), axis=90.0, rate=0.4, phase=0.55))
    f.box(TIMD, (-76, 0, 285), (5, 5, 270))
    f.box(YEL, (-76, 0, 142), (12, 12, 20))
    a, b = (0.0, 30.0, 300.0), (0.0, 420.0, -40.0)
    tilt_box(g, STRAW, a, b, 120.0, 8.0)
    for k in range(4):
        t = (k + 0.5) / 4.0
        g.box(TIM, (0, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t + 8), (132, 12, 12))
    for s in (-1, 1):
        tilt_box(g, STRAW, (s * 62, 20, 396), (s * 62, 420, 64), 6.0, 6.0)
        g.box(TIM, (s * 62, 420, -115), (14, 14, 370))


def rope_sledge(sc):
    """LOW (1.4 m): a hauling sledge with a great coil of weed rope and a small bale on it, its haul rope on the
    sand, a marker stake with a yellow ribbon."""
    g = sc.still()
    for y in (-52, 52):
        g.box(TIM, (-10, y, 10), (300, 14, 18))
        tilt_box(g, TIM, (138, y, 10), (200, y, 62), 14.0, 18.0)
    for x in (-100, 80):
        g.box(TIMD, (x, 0, -50), (20, 150, 110))
    g.box(STRAW, (-20, 0, 24), (250, 122, 8))
    for k in range(3):
        g.cyl(STRAW if k != 1 else PALE, (-70, 0, 28 + k * 21), 58 - 6 * k, 20, steps=8)
    g.cyl(TIMD, (-70, 0, 91), 22, 4, steps=8)
    g.box(STRAW, (62, 6, 60), (72, 62, 62))
    g.box(TIMD, (62, 6, 60), (76, 10, 66))
    g.box(TIM, (198, 0, 58), (12, 120, 12))
    tilt_box(g, STRAW, (200, 0, 56), (330, -36, -24), 7.0, 7.0)
    tilt_box(g, STRAW, (330, -36, -24), (430, 14, -50), 7.0, 7.0)
    g.box(TIM, (-178, 64, 20), (10, 10, 240))
    f = sc.part(P.motion(swing=14.0, pivot=(-178, 64, 124), axis=P.UP, rate=0.3, phase=0.4))
    f.box(YEL, (-152, 64, 124), (46, 5, 22))


def plank_walk(sc):
    """LOW (1.6 m): a length of the farm's plank walk on short piles, a rope rail along its back, a mooring post
    with a coil, a basket of cut weed."""
    g = sc.still()
    for x in (-400, -133, 133, 400):
        for y in (-60, 60):
            g.box(TIMD, (x, y, -15), (16, 16, 150))
    for k, y in enumerate((-50, 0, 50)):
        g.box(TIM if k != 1 else STRAW, (0, y, 62), (900 - 30 * (k % 2), 47, 10))
    xs = (-420, -140, 140, 420)
    for x in xs:
        g.box(TIM, (x, 68, 108), (12, 12, 84))
        g.box(GRN, (x, 68, 153), (16, 16, 8))
    for k in range(3):
        xm = (xs[k] + xs[k + 1]) / 2.0
        tilt_box(g, STRAW, (xs[k], 68, 140), (xm, 68, 112), 6.0, 6.0)
        tilt_box(g, STRAW, (xm, 68, 112), (xs[k + 1], 68, 140), 6.0, 6.0)
    g.cyl(TIM, (-300, -40, 67), 16, 62, steps=6)
    g.cyl(STRAW, (-300, -40, 67), 36, 14, steps=8)
    g.cyl(STRAW, (250, -16, 67), 32, 46, steps=8, top_r=38)
    f = sc.part(P.motion(swing=6.0, pivot=(250, -16, 110), axis=0.0, rate=0.3, phase=0.2))
    for k in range(3):
        f.box((KELP, KELPD, KELP)[k], (232 + k * 18, -16 + 8 * (k % 2), 134), (9, 24, 46))


def sheaf_row(sc):
    """LOW (1.4 m): seven sheaves of weed stood to dry against a rail, tied, each with a tuft; two sheets over the rail."""
    g = sc.still()
    for x in (-380, 0, 380):
        g.box(TIM, (x, 0, 12), (14, 14, 204))
    g.box(TIM, (0, 0, 110), (800, 10, 10))
    for k in range(7):
        x = -330 + k * 110
        g.cyl(STRAW, (x, -34, -40), 31, 150, steps=6, top_r=12)
        g.box(TIMD, (x, -34, 58), (32, 32, 8))
        g.cyl((KELP, STRAW, KELPD)[k % 3], (x, -34, 106), 15, 30, steps=6, top_r=26)
    f = sc.part(P.motion(swing=6.0, pivot=(0, 0, 112), axis=0.0, rate=0.28, phase=0.5))
    f.box(KELPD, (-270, 4, 80), (84, 6, 66))
    f.box(STRAW, (160, 4, 76), (84, 6, 74))


# ------------------------------------------------------------------------------------------ the hero
def the_reel(sc):
    """THE REEL: the harvesting wheel, 55 m across, on a timber A-frame, taller than every stalk. Its plane stands
    across the road (the driver sees its face from far down the road). The wheel winds in and pays out: it turns
    most of a round one way, slowly, and back (the builders' swing: 170 degrees each way, a round in about a minute).
    Long ropes of weed run up on to its drum from the sand; two cabins hang at the axle; two masts carry lanterns."""
    H, R, RX, FX = 3350.0, 2750.0, 330.0, 560.0
    g = sc.still()
    for sx in (-1, 1):
        x = sx * FX
        for sy in (-1, 1):
            tilt_box(g, TIM, (x, sy * 2100, -450), (x, sy * 60, H + 120), 110.0, 130.0)
        g.box(TIM, (x, 0, 1300), (90, 2380, 80))
        g.box(TIM, (x, 0, 2400), (90, 1240, 80))
        tilt_box(g, TIM, (sx * 1500, 0, -450), (x, 0, H - 300), 90.0, 90.0)
        g.box(GRN, (x, 0, H), (150, 270, 270))
        # the cabin at the axle
        cx = sx * (FX + 200)
        g.box(TIM, (cx, 0, H - 222), (270, 460, 24))
        g.box(PALE, (cx, 0, H - 60), (240, 420, 300))
        g.box(WIN, (cx, -213, H - 40), (120, 8, 110), glow=0.4)
        g.box(WIN, (cx + sx * 123, 0, H - 40), (8, 200, 110), glow=0.4)
        gable_roof(g, cx, 0, H + 90, 240, 420, 180, over=30.0)
        # the mast over the frame
        tilt_box(g, TIM, (x, 0, H + 120), (x, 0, 6500), 50.0, 50.0)
        g.cyl(YEL, (x, 0, 6480), 110, 90, steps=6, top_r=10)
        lantern(g, x + sx * 90, 0, 6440, r=60, glow=0.6)
        g.box(TIM, (x + sx * 45, 0, 6440), (110, 12, 12))
        pennant(sc, x, 0, 6300, (KELP, YEL, KELP), seg=420.0, h=150.0, seed=40 + sx)
    for sy in (-1, 1):
        g.box(TIM, (0, sy * 1850, 30), (2 * FX + 240, 90, 90))
    rod(g, TIMD, (-FX - 90, 0, H), (FX + 90, 0, H), 70.0, steps=8)
    ladder(g, (FX + 80, -2050, -300), (FX + 80, -170, H - 215), w=70.0, rungs=14)
    # the ropes of weed running up on to the drum
    for k, (x, sy) in enumerate(((-170, -1), (0, -1), (170, -1), (-90, 1), (90, 1))):
        tilt_box(g, (KELP, KELPD, KELPL, KELPD, KELP)[k], (x * 2.4, sy * (3300 + 110 * k), -300), (x, sy * 1367, H + 617), 44.0, 44.0)   # tangent to the drum
    f = sc.part(P.motion(swing=5.0, pivot=(0, -2000, 2200), axis=0.0, rate=0.15, phase=0.3))
    for k in range(6):
        t = 0.2 + 0.11 * k
        f.box((KELPL, KELP, STRAW)[k % 3], (-170 * (2.4 - 1.4 * t) + 60 * (k % 3), -3300 + 1933 * t, -300 + (H + 917) * t - 170), (40, 9, 300))
    # ---- the wheel
    w = sc.part(P.motion(swing=170.0, pivot=(0.0, 0.0, H), axis=0.0, rate=0.012, phase=0.0))
    N = 16

    def pt(x, rad, k, off=0.0):
        a = 2.0 * math.pi * (k + off) / N
        return (x, rad * math.cos(a), H + rad * math.sin(a))

    for sx in (-1, 1):
        x = sx * RX
        for k in range(N):
            # fix round: the rim and the spokes were dark timber, 60 to 90 cm thick: a thin dark wheel on dark water that no frame picked
            # out. Yellow and straw now (the district's roofs), about twice as thick.
            tilt_box(w, YEL if k % 2 == 0 else STRAW, pt(x, R, k), pt(x, R, k + 1), 120.0, 150.0)
            tilt_box(w, STRAW, pt(x, 2000.0, k), pt(x, 2000.0, k + 1), 70.0, 90.0)
        for k in range(4):
            tilt_box(w, STRAW if k % 2 else YEL, pt(x, R, k * 2), pt(x, R, k * 2 + 8), 110.0, 110.0)
        for k in range(8):
            w.box(YEL, pt(sx * (RX + 90), R - 60.0, k * 2), (120, 150, 150))
            w.sphere(GLASS, pt(sx * (RX + 75), R - 40.0, k * 2 + 1), 70, steps=(3, 6), glow=0.5)
    for k in range(12):
        a0, a1 = 2.0 * math.pi * k / 12.0, 2.0 * math.pi * (k + 1) / 12.0
        tilt_box(w, (KELP, KELPD, STRAW, KELPL)[k % 4], (0, 1500 * math.cos(a0), H + 1500 * math.sin(a0)),
                 (0, 1500 * math.cos(a1), H + 1500 * math.sin(a1)), 600.0, 70.0)
    for k in range(8):
        p, q = pt(0, 1530.0, k * 2, 0.5), pt(0, 1990.0, k * 2, 0.5)
        tilt_box(w, KELP if k % 2 else KELPL, p, q, 60.0, 30.0)
    rod(w, GRN, (-RX - 40, 0, H), (RX + 40, 0, H), 190.0, steps=8)


# ------------------------------------------------------------------------------------------ the kit
JOBS = [
    ('SM_S1D2_StiltHouse', stilt_house),
    ('SM_S1D2_TallHouse', tall_house),
    ('SM_S1D2_DryingBarn', drying_barn),
    ('SM_S1D2_MastHouse', mast_house),
    ('SM_S1D2_RopeWalk', rope_walk),
    ('SM_S1D2_PressHouse', press_house, dict(grow=(260, 260, 60), grow_down=(260, 260, 0))),
    ('SM_S1D2_FloatStore', float_store),
    ('SM_S1D2_RopeBridge', rope_bridge),
    ('SM_S1D2_DryingRack', drying_rack),
    ('SM_S1D2_FloatLantern', float_lantern),
    ('SM_S1D2_BaleStack', bale_stack, STILL),
    ('SM_S1D2_ScytheRack', scythe_rack, STILL),
    ('SM_S1D2_BellPost', bell_post),
    ('SM_S1D2_RopeSledge', rope_sledge, dict(grow=(30, 30, 0), grow_down=(30, 30, 0))),
    ('SM_S1D2_PlankWalk', plank_walk, dict(grow=(10, 10, 0), grow_down=(10, 10, 0))),
    ('SM_S1D2_SheafRow', sheaf_row, dict(grow=(10, 20, 0), grow_down=(10, 20, 0))),
    ('SM_S1D2_TheReel', the_reel, dict(far=True, grow=(450, 250, 250), grow_down=(450, 250, 0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
