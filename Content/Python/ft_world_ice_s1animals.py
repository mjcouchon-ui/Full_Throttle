"""Ice Mountain, Stage 1, builder s1animals: the animals of the eight districts. /Game/World/IceMountain/SM_S1ANIMALS_*.
Run: import ft_world_ice_s1animals as K; K.run()

Eighteen pieces, each one small scene that tells which district it stands in:
  ground  Hares (the summit, the high ridge), Kennel (the summit's rescue dogs), Ptarmigan (the avalanche works),
          Goats and SleighPink (the valley town), SleighBlue (the spa), Marmots (the ridges), Penguins and Seals (the
          glacier and the mine, each with its own melt pool), Foxes (the mine), Chamois (a crag: the works, the high
          ridge), YakTrain (the white ridge, with its drover), Reindeer (the expedition, with their keeper and sledge),
          Swans (a pond: the valley town, the spa).
  sky     SkyBuntings (the summit), SkyOwl (the works: one pale owl, gliding), SkyGulls (the glacier, the mine),
          SkySwans (the ridges: a V). All pale, all on STRAIGHT lines along the road, never a circle.

Rules kept here: nothing white or pale blue stands upright (the boss is a pale yeti); nothing orange, no green box;
nothing dark in the air; no glow at all; every colour is one of ft_dress_ice.py; movement only by the builders' motion
parameters. What walks, walks along the piece's X (the road's own direction): no path can cross the road.
This hill falls 7 in 100 along the road: every ground piece brings its own level pad of packed snow (50 cm of footing).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
SNOW, SNOW_SH, ICE, ICE_D, ICE_DEEP = '#f4f9ff', '#cdd6f1', '#bfe3ff', '#7fb0e0', '#4f8fd2'
ROCK, ROCK_D, CUT = '#6a7488', '#5a6478', '#8b93b4'
STEEL, WOOD, WOOD_D, PINE, PINE_D = '#9aa4b8', '#9a6a44', '#6a4c36', '#2f6b52', '#234f40'
RED, BLUE, CHAR, PANE, CONC = '#ff2d3c', '#2e6bff', '#3b3f52', '#1d2740', '#aab3c4'
PINK, TEAL, YEL, ASPH, CREAM = '#ff4fb8', '#19b5c9', '#ffd23f', '#3d4763', '#f4efe6'
MINT, VIOLET = '#8ff0cf', '#b9a2ff'


# ------------------------------------------------------------------------------------------ helpers
def mo_at(x, y, z=0.0, **kw):
    """A motion whose pivot and base are the creature's own place."""
    d = dict(pivot=(x, y, z), base=(x, y))
    d.update(kw)
    return P.motion(**d)


def pad(g, w, d, top=6.0, depth=50.0, col=SNOW_SH):
    """The piece's own level ground: packed snow, `depth` cm of footing."""
    g.box(col, (0, 0, (top - depth) / 2.0), (w, d, top + depth))


def mound(g, x, y, rx, ry, h, col=SNOW):
    g.sphere(col, (x, y, 0), 100, scale=(rx / 100.0, ry / 100.0, h / 100.0), steps=(4, 6))


def boulder(g, x, y, z, r, col=ROCK, sq=0.75, yaw=0.0):
    g.sphere(col, (x, y, z), r, scale=(1.0, 0.85, sq), steps=(3, 6), rot=(0, yaw, 0))


def pool(g, x, y, r, water=ICE_D, deep=None, rim=SNOW, lumps=8, seed=1):
    """A melt pool brought with the piece: a flat disc, a paler shelf round it, snow lumps on its rim."""
    r2 = random.Random(seed)
    g.cyl(ICE, (x, y, 1.0), r + 22, 6.0, steps=10)
    g.cyl(water, (x, y, 3.0), r, 5.5, steps=10)
    if deep:
        g.cyl(deep, (x, y, 5.0), r * 0.55, 4.5, steps=8)
    for k in range(lumps):
        a = math.radians(k * 360.0 / lumps + r2.uniform(-12, 12))
        if math.sin(a) < -0.35:
            continue                                   # the side towards the road stays open: the water shows
        mound(g, x + math.cos(a) * (r + 30), y + math.sin(a) * (r + 30), r2.uniform(40, 62), r2.uniform(34, 50), r2.uniform(18, 30), rim)


def _frame(x, y, z, yaw, s):
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    return lambda p: (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])


def animal(sc, x, y, z=0.0, yaw=0.0, s=1.0, extra=None, head_extra=None, **q):
    """P.quadruped, and then more drawn onto it: extra(m) into its body, head_extra(m, n1) into its neck and head
    (n1 = the top of the neck, in the animal's own frame). The two models are fetched with the very motion values
    quadruped() gave those parts, so what is drawn moves with them."""
    P.quadruped(sc, x, y, z, yaw, s, **q)
    length, height, girth = q.get('length', 150.0), q.get('height', 120.0), q.get('girth', 42.0)
    neck, neck_up, graze = q.get('neck', 50.0), q.get('neck_up', 55.0), q.get('graze', 0.0)
    W = _frame(x, y, z, yaw, s)
    tv, base = (0.0, 0.0), (x, y)
    if q.get('travel'):
        tv = (float(q['travel'][0]), float(q['travel'][1]) / float(q['travel'][0]))
    elif q.get('orbit'):
        base, tv = (q['orbit'][0], q['orbit'][1]), (-1.0, float(q['orbit'][2]))
    common = dict(phase=q.get('phase', 0.0), rate=q.get('rate', 0.0), bob=q.get('bob', 0.0), base=base, travel=tv)

    def part(swing, pivot):
        return sc.part(P.motion(swing=swing, pivot=W(pivot), axis=yaw + 0.0, **common), (x, y, z), yaw, s)

    hl = length / 2.0
    if extra:
        extra(part(0.0, (0, 0, height)))
    if head_extra:
        a = math.radians(neck_up)
        n0 = (0, -hl + girth * 0.25, height - girth * 0.35)
        n1 = (0, n0[1] - math.cos(a) * neck, n0[2] + math.sin(a) * neck)
        m = part(graze, (0, -hl + girth * 0.2, height - girth * 0.3)) if graze else part(0.0, (0, 0, height))
        head_extra(m, n1)


def sky_bird(sc, x, y, z, span, body, wing, beak, phase, rate, flap, length, speed, bob=0.0, neck=0.0, head=0.0):
    """P.bird flying +X on a straight line `length` cm long (it comes round again), with a long neck or a round
    head drawn onto its body when asked."""
    P.bird(sc, x, y, z, heading=0.0, span=span, body=body, wing=wing, beak=beak, phase=phase, rate=rate, flap=flap,
           travel=(length, speed), bob=bob)
    if neck or head:
        m = sc.part(P.motion(pivot=(x, y, z), phase=phase, rate=rate, bob=bob, base=(x, y), travel=(float(length), float(speed) / float(length))))
        m.base = None
        L = span * 0.42
        if neck:
            tilt_box(m, body, (x + L * 0.5, y, z + L * 0.08), (x + L * 0.5 + neck, y, z + L * 0.16), L * 0.13, L * 0.13)
            m.box(body, (x + L * 0.5 + neck + L * 0.08, y, z + L * 0.17), (L * 0.24, L * 0.16, L * 0.16))
            m.box(beak, (x + L * 0.5 + neck + L * 0.27, y, z + L * 0.16), (L * 0.16, L * 0.1, L * 0.08))
        if head:
            m.sphere(body, (x + L * 0.5, y, z + L * 0.12), head, steps=(3, 6))
            for sd in (-1, 1):
                m.box(YEL, (x + L * 0.5 + head * 0.82, y + sd * head * 0.4, z + L * 0.16), (head * 0.3, head * 0.34, head * 0.34))


# ------------------------------------------------------------------------------------------ small creatures
def hare(sc, x, y, z=0.0, yaw=0.0, s=1.0, up=False, **mo):
    m = sc.part(mo_at(x, y, z, **mo), (x, y, z), yaw, s)
    if up:                                             # sitting up, ears high
        m.cyl(WOOD, (0, 4, 0), 14, 34, steps=6, top_r=10)
        m.box(CREAM, (0, -8, 18), (14, 6, 20))
        m.sphere(WOOD, (0, -2, 42), 11, steps=(3, 6))
        for sd in (-1, 1):
            m.box(WOOD, (sd * 5, 0, 62), (5, 5, 26))
            m.box(WOOD_D, (sd * 5, 0, 77), (5.4, 5.4, 6))
            m.box(WOOD_D, (sd * 8, -10, 22), (5, 9, 5))
        m.box(WOOD_D, (0, 14, 4), (22, 22, 8))
        m.sphere(CREAM, (0, 20, 12), 6, steps=(3, 5))
        return
    m.box(WOOD, (0, 2, 20), (20, 40, 22))
    m.sphere(WOOD, (0, 14, 25), 15, steps=(3, 6))
    m.sphere(WOOD, (0, -22, 36), 11, steps=(3, 6))
    m.box(CREAM, (0, 0, 9), (16, 30, 5))
    m.sphere(CREAM, (0, 27, 26), 6, steps=(3, 5))
    for sd in (-1, 1):
        m.box(WOOD, (sd * 5, -18, 56), (5, 5, 24), rot=(0, 0, 0))
        m.box(WOOD_D, (sd * 5, -18, 70), (5.4, 5.4, 6))
        m.box(WOOD_D, (sd * 7, -14, 6), (6, 8, 12))
        m.box(WOOD_D, (sd * 9, 12, 4), (7, 22, 8))


def ptarmigan(sc, x, y, z=0.0, yaw=0.0, s=1.0, **mo):
    m = sc.part(mo_at(x, y, z + 10.0 * s, **mo), (x, y, z), yaw, s)
    m.sphere(WOOD, (0, 0, 20), 16, scale=(1.0, 1.35, 1.0), steps=(4, 6))
    m.box(WOOD_D, (0, 6, 32), (20, 18, 5))
    m.box(WOOD_D, (0, -6, 31), (16, 6, 5))
    m.box(CREAM, (0, -2, 7), (18, 24, 5))
    m.sphere(WOOD, (0, -20, 36), 8, steps=(3, 5))
    m.box(RED, (0, -21, 44), (8, 6, 4))
    m.box(CHAR, (0, -29, 35), (4.5, 6, 4.5))
    m.box(WOOD_D, (0, 23, 24), (16, 12, 5))
    for sd in (-1, 1):
        m.box(CREAM, (sd * 6, -2, 2), (6, 10, 4))


def marmot(sc, x, y, z=0.0, yaw=0.0, s=1.0, up=True, **mo):
    m = sc.part(mo_at(x, y, z, **mo), (x, y, z), yaw, s)
    if up:
        m.cyl(WOOD, (0, 0, 0), 15, 34, steps=6, top_r=11)
        m.box(CREAM, (0, -10, 17), (14, 6, 20))
        m.sphere(WOOD, (0, -2, 42), 11, steps=(3, 6))
        m.box(CREAM, (0, -12, 40), (9, 6, 7))
        m.box(CHAR, (0, -15, 43), (5, 4, 4))
        for sd in (-1, 1):
            m.box(WOOD_D, (sd * 8, 0, 52), (5, 5, 5))
            m.box(WOOD_D, (sd * 9, -12, 24), (5, 9, 5))
        m.box(WOOD_D, (0, 19, 5), (9, 24, 7))
    else:                                              # on all fours, low and long
        m.box(WOOD, (0, 0, 14), (22, 44, 20))
        m.sphere(WOOD, (0, -26, 20), 11, steps=(3, 6))
        m.box(CREAM, (0, -36, 18), (9, 6, 7))
        m.box(CHAR, (0, -39, 21), (5, 4, 4))
        m.box(WOOD_D, (0, 30, 12), (9, 24, 7))
        for sd in (-1, 1):
            m.box(WOOD_D, (sd * 8, -24, 30), (5, 5, 5))
            m.box(WOOD_D, (sd * 9, -14, 4), (6, 8, 8))
            m.box(WOOD_D, (sd * 9, 14, 4), (6, 8, 8))


def penguin(sc, x, y, z=0.0, yaw=0.0, s=1.0, chick=False, **mo):
    """Navy and cream, small. Upright, it waddles (give swing and axis)."""
    col = ROCK_D if chick else PANE
    m = sc.part(mo_at(x, y, z, **mo), (x, y, z), yaw, s)
    m.cyl(col, (0, 0, 5), 15, 44, steps=6, top_r=10)
    m.box(CONC if chick else CREAM, (0, -10.5, 26), (15, 8, 28))
    m.sphere(col, (0, 0, 57), 10, steps=(3, 6))
    m.box(YEL, (0, -11, 56), (5, 9, 4.5))
    for sd in (-1, 1):
        m.box(YEL, (sd * 6, -5, 3), (8, 15, 6))
        m.box(col, (sd * 17, 1, 30), (5, 11, 26), rot=(0, 0, sd * 10))


def penguin_slide(sc, x, y, z=0.0, s=1.0, **mo):
    """On its belly, head towards +X: the one that slides to the pool."""
    m = sc.part(mo_at(x, y, z, **mo), (x, y, z), 0.0, s)
    m.box(PANE, (0, 0, 14), (48, 26, 18))
    m.box(CREAM, (0, 0, 5), (44, 22, 6))
    m.sphere(PANE, (30, 0, 18), 10, steps=(3, 6))
    m.box(YEL, (42, 0, 17), (9, 5, 4.5))
    for sd in (-1, 1):
        m.box(PANE, (-2, sd * 19, 12), (26, 11, 5))
        m.box(YEL, (-28, sd * 6, 8), (12, 8, 5))


def seal(sc, x, y, z=0.0, yaw=0.0, s=1.0, col=ROCK_D, **mo):
    """A seal lying, head towards -Y."""
    m = sc.part(mo_at(x, y, z, **mo), (x, y, z), yaw, s)
    m.sphere(col, (0, 0, 20), 20, scale=(1.0, 2.6, 0.95), steps=(4, 6))
    m.box(STEEL, (0, -4, 5), (26, 70, 6))
    for sd in (-1, 1):
        m.box(CHAR, (sd * 23, -18, 6), (18, 13, 5), rot=(0, sd * 25, 0))
        m.box(CHAR, (sd * 8, 56, 12), (11, 22, 5), rot=(0, sd * 18, 0))


def seal_head(sc, x, y, z=0.0, yaw=0.0, s=1.0, col=ROCK_D, **mo):
    """Its neck and head, a part of their own (they lift, or they are all that shows above the water)."""
    mo.setdefault('pivot', _frame(x, y, z, yaw, s)((0, -36, 22)))
    m = sc.part(mo_at(x, y, z, **mo), (x, y, z), yaw, s)
    tilt_box(m, col, (0, -36, 22), (0, -52, 40), 20, 20)
    m.sphere(col, (0, -56, 44), 13, steps=(3, 6))
    m.box(STEEL, (0, -68, 41), (11, 8, 9))
    m.box(CHAR, (0, -72, 44), (5, 4, 4))


def swan(sc, x, y, z=0.0, yaw=0.0, s=1.0, col=CREAM, dip=None, **mo):
    """Cream, head towards -Y. dip: a motion for the neck and head alone (it feeds)."""
    m = sc.part(mo_at(x, y, z, **mo), (x, y, z), yaw, s)
    m.sphere(col, (0, 0, 16), 20, scale=(0.9, 1.6, 0.72), steps=(4, 6))
    for sd in (-1, 1):
        m.box(col, (sd * 13, 8, 25), (8, 44, 18), rot=(0, 0, sd * 14))
    tilt_box(m, col, (0, 28, 22), (0, 46, 36), 12, 6)
    n = sc.part(P.motion(**dip), (x, y, z), yaw, s) if dip else m
    tilt_box(n, col, (0, -24, 20), (0, -36, 52), 9, 9)
    tilt_box(n, col, (0, -36, 50), (0, -30, 66), 8, 8)
    n.box(col, (0, -35, 68), (9, 16, 9))
    n.box(YEL, (0, -47, 66), (6, 10, 5))
    n.box(CHAR, (0, -42, 72), (5, 5, 4))


def tuft(g, x, y, z=0.0, col=PINE_D, n=5, r=18.0, h=26.0, seed=0):
    """A tuft of dwarf pine or heather through the snow: a few leaning blades."""
    rr = random.Random(seed)
    for k in range(n):
        a = math.radians(k * 360.0 / n + rr.uniform(-20, 20))
        tilt_box(g, col, (x, y, z), (x + math.cos(a) * r, y + math.sin(a) * r, z + h * rr.uniform(0.7, 1.1)), 6, 6)


# ------------------------------------------------------------------------------------------ the ground pieces
def hares(sc):
    """The summit and the high ridge: mountain hares in their brown coat round a burrow under a snow hummock. Two hop
    along the front, one nibbles, one sits up on the hummock and watches, ears turning."""
    g = sc.still()
    pad(g, 600, 420)
    mound(g, 60, 90, 190, 120, 62)
    mound(g, -200, 120, 110, 80, 40)
    g.cyl(CHAR, (40, 22, 14), 26, 20, steps=7, rot=(60, 90, 0))        # the burrow's mouth
    g.box(SNOW_SH, (40, -30, 7), (70, 60, 4))                           # the scrape before it
    tuft(g, -230, 10, 6, PINE_D, 6, 30, 34, 1)
    tuft(g, 240, 120, 6, PINE, 5, 26, 30, 2)
    tuft(g, 250, -120, 6, PINE_D, 5, 22, 26, 3)
    for k, (tx, ty) in enumerate(((-120, -150), (-60, -160), (0, -148), (60, -158), (120, -150))):
        g.box(SNOW_SH, (tx, ty, 7), (16, 10, 4))                        # tracks
    hare(sc, 60, 92, 60, yaw=20, s=1.55, up=True, swing=9, axis=P.UP, rate=0.35, phase=0.2)
    hare(sc, -150, 30, 6, yaw=-40, s=1.5, swing=7, axis=-40.0, rate=1.1, phase=0.5)
    hare(sc, -200, -120, 6, yaw=90, s=1.5, bob=20, rate=1.4, phase=0.1, travel=(520.0, 0.21))
    hare(sc, 80, -165, 6, yaw=90, s=1.35, bob=17, rate=1.5, phase=0.6, travel=(520.0, 0.21))
    hare(sc, 215, 20, 6, yaw=150, s=1.4, bob=5, rate=0.6, phase=0.3)


def kennel(sc):
    """The summit: the mountain rescue's dogs at their kennel. A timber kennel under a teal roof and a snow cap, a
    rescue sled on its end, bowls, a coiled rope; three dogs (tails wag, one sniffs the bowl, the pup jumps)."""
    g = sc.still()
    pad(g, 700, 460)
    g.box(WOOD, (-150, 110, 80), (250, 190, 150))                       # the kennel
    g.box(WOOD_D, (-150, 110, 10), (262, 202, 10))
    for k in range(4):
        g.box(WOOD_D, (-250 + k * 66, 13, 80), (6, 5, 150))             # plank joints
    g.box(PANE, (-150, 13, 55), (84, 6, 100))                           # the door
    g.cyl(PANE, (-150, 15, 105), 42, 6, steps=8, rot=(-90, -90, 0))
    for sd in (-1, 1):
        tilt_box(g, TEAL, (-150 + sd * 150, 110, 150), (-150, 110, 222), 216, 14)
    g.box(SNOW, (-150, 110, 228), (150, 210, 16))
    g.box(SNOW, (-218, 110, 196), (90, 214, 12), rot=(-26, 0, 0))
    g.box(SNOW, (-82, 110, 196), (90, 214, 12), rot=(26, 0, 0))
    g.box(CREAM, (-150, 10, 178), (70, 5, 26))                          # a board of plain blocks over the door
    for k, c in enumerate((RED, TEAL, YEL)):
        g.box(c, (-170 + k * 20, 7, 178), (14, 5, 16))
    g.cyl(STEEL, (-150, 110, 222), 4, 110, steps=6)                     # the mast
    f = sc.part(P.motion(swing=14, pivot=(-150, 110, 315), axis=P.UP, rate=0.5, phase=0.1))
    f.box(TEAL, (-112, 110, 315), (76, 4, 30))
    f.box(RED, (-66, 110, 315), (18, 4.4, 30))
    g = sc.still()
    tilt_box(g, RED, (40, 190, 6), (58, 150, 170), 12, 46)              # the rescue sled, stood on its end
    tilt_box(g, WOOD_D, (40, 176, 6), (52, 150, 120), 6, 60)
    g.cyl(WOOD_D, (130, 170, 0), 9, 96, steps=6)                        # a post with a coiled rope
    g.cyl(YEL, (130, 170, 60), 17, 18, steps=8)
    for bx in (150, 215):
        g.cyl(RED, (bx, -150, 6), 18, 10, steps=8, top_r=22)            # bowls
    g.box(WOOD, (300, 150, 26), (60, 44, 40))                           # a crate of blankets
    g.box(TEAL, (300, 150, 50), (56, 40, 10))
    mound(g, 290, -170, 90, 60, 26)
    dog = dict(length=70, height=48, girth=22, neck=16, neck_up=35, head=22, body=WOOD, belly=CREAM, ears=WOOD_D, snout=CREAM,
               tail=30, tail_col=CREAM, legs_col=WOOD)

    def cask(m):                                                        # the little cask under the chin
        m.cyl(WOOD_D, (-8, -34, 30), 7, 16, steps=6, rot=(-90, 0, 0))
        m.box(RED, (0, -30, 40), (24, 6, 5))
    animal(sc, 60, -60, 6, yaw=15, s=1.75, wag=32, rate=1.8, phase=0.1, extra=cask, **dog)
    d2 = dict(dog); d2.update(neck_up=-25, body=WOOD_D, legs_col=WOOD_D)
    animal(sc, 190, -70, 6, yaw=-100, s=1.6, graze=12, wag=24, rate=1.2, phase=0.5, **d2)
    animal(sc, -40, -150, 6, yaw=60, s=1.1, bob=16, wag=30, rate=1.7, phase=0.8, **dog)


def ptarmigans(sc):
    """The avalanche works: a covey of ptarmigan in brown summer feather on a lichen rock and the snow round it.
    They peck; two shuffle along the front."""
    g = sc.still()
    pad(g, 520, 380)
    boulder(g, 30, 70, 20, 110, ROCK, 0.5, 20)
    boulder(g, -130, 110, 10, 70, ROCK_D, 0.6, -30)
    g.box(YEL, (10, 20, 62), (46, 30, 5), rot=(0, 20, 0))               # lichen
    g.box(MINT, (80, 60, 66), (30, 24, 5), rot=(0, -15, 0))
    g.box(YEL, (-130, 80, 44), (26, 20, 5))
    g.box(SNOW, (40, 100, 72), (120, 80, 8))
    tuft(g, 200, 130, 6, PINE_D, 5, 24, 28, 4)
    tuft(g, -220, -40, 6, PINE_D, 4, 20, 22, 5)
    g.box(CONC, (200, -120, 9), (90, 30, 8), rot=(0, 12, 0))            # a broken kerb of the works, half buried
    g.box(YEL, (200, -120, 14), (30, 31, 5), rot=(0, 12, 0))
    r = random.Random(11)
    for k, (px, py, pz, yw) in enumerate(((30, 30, 70, 10), (-40, -70, 6, -30), (120, -40, 6, 40), (-150, 10, 6, 80))):
        ptarmigan(sc, px, py, pz, yaw=yw, s=1.6, swing=16, axis=yw + 0.0, rate=r.uniform(0.9, 1.5), phase=r.random())
    ptarmigan(sc, -120, -140, 6, yaw=90, s=1.5, bob=4, rate=1.6, phase=0.2, travel=(360.0, 0.11))
    ptarmigan(sc, 60, -155, 6, yaw=90, s=1.4, bob=4, rate=1.6, phase=0.7, travel=(360.0, 0.11))


def goats(sc):
    """The valley town: a goat pen. Timber rails, a shelter under pink shingles and a deep snow cap, a hay rack, a
    trough; one goat stands on the roof, one walks the pen, a kid hops, the others feed."""
    g = sc.still()
    pad(g, 800, 580)
    for sx in (-380, -190, 0, 190, 380):
        for sy in (-270, 270):
            g.box(WOOD_D, (sx, sy, 50), (12, 12, 110))
    for sy in (-90, 90):
        for sx in (-380, 380):
            g.box(WOOD_D, (sx, sy, 50), (12, 12, 110))
    for z in (45, 88):
        g.box(WOOD, (0, -270, z), (770, 7, 12))
        g.box(WOOD, (0, 270, z), (770, 7, 12))
        g.box(WOOD, (-380, 0, z), (7, 540, 12))
        g.box(WOOD, (380, 0, z), (7, 540, 12))
    g.box(WOOD_D, (-200, 180, 90), (300, 170, 170))                     # the shelter
    g.box(PANE, (-200, 94, 70), (110, 6, 130))
    g.box(CREAM, (-200, 92, 150), (300, 5, 26))
    tilt_box(g, PINK, (-200, 80, 168), (-200, 280, 214), 330, 12)       # the shingle roof, falling to the front
    for k in range(4):
        g.box(WOOD_D, (-200, 100 + k * 50, 180 + k * 11.5), (334, 6, 6))
    tilt_box(g, SNOW, (-200, 110, 190), (-200, 276, 228), 300, 16)
    g.box(WOOD, (200, 200, 60), (160, 30, 8))                           # the hay rack
    for sx in (130, 270):
        g.box(WOOD_D, (sx, 200, 40), (8, 34, 80))
    g.box(YEL, (200, 200, 84), (150, 40, 40))
    g.box(WOOD_D, (250, -120, 22), (150, 44, 30))                       # the trough
    g.box(ICE_D, (250, -120, 38), (136, 32, 4))
    g.box(PINK, (0, -274, 100), (60, 5, 34))                            # a plate on the gate: a plain bell shape
    g.cyl(YEL, (0, -276, 92), 11, 5, steps=8, rot=(-90, -90, 0))
    goat = dict(length=80, height=62, girth=26, neck=22, neck_up=40, head=23, body=CREAM, belly=WOOD, ears=WOOD_D, horns=WOOD_D,
                tail=14, legs_col=WOOD_D, snout=WOOD)
    brown = dict(goat); brown.update(body=WOOD, belly=CREAM, snout=CREAM)
    animal(sc, -230, 190, 214, yaw=25, s=1.5, graze=6, wag=20, rate=0.5, phase=0.2, **goat)          # on the roof
    g2 = dict(brown); g2.update(neck_up=-20)
    animal(sc, 190, 120, 6, yaw=180, s=1.6, graze=10, wag=22, rate=0.7, phase=0.6, **g2)             # at the hay
    animal(sc, 240, -40, 6, yaw=10, s=1.5, graze=12, rate=0.5, phase=0.9, **dict(goat, neck_up=-25))  # at the trough
    animal(sc, -100, -170, 6, yaw=90, s=1.5, stride=16, rate=1.1, phase=0.3, travel=(520.0, 60.0), **brown)
    animal(sc, 40, 40, 6, yaw=-60, s=0.95, bob=14, wag=30, rate=1.5, phase=0.4, **goat)              # the kid


def sleigh(lead, trim):
    def build(sc):
        """Two sleigh horses under blankets at a hitching rail, their sleigh behind them: a hay net, a bucket, a
        lamp post without a light. One horse nods at the hay, the other paws; tails swing."""
        g = sc.still()
        pad(g, 800, 500)
        for sx in (-300, 0, 300):
            g.box(WOOD_D, (sx, -190, 60), (14, 14, 130))
        g.box(WOOD, (0, -190, 118), (640, 12, 12))
        g.box(YEL, (-150, -176, 84), (60, 22, 52))                      # the hay net
        g.cyl(STEEL, (180, -150, 6), 20, 30, steps=8, top_r=24)          # the bucket
        g.cyl(WOOD_D, (360, 150, 0), 8, 250, steps=6)                    # the lamp post
        g.box(WOOD_D, (360, 150, 252), (40, 40, 6))
        g.box(PANE, (360, 150, 232), (24, 24, 34))
        g.box(SNOW, (360, 150, 260), (46, 46, 10))
        # the sleigh, along X behind the horses
        for sy in (110, 210):
            g.box(WOOD_D, (0, sy, 12), (360, 9, 9))
            tilt_box(g, WOOD_D, (-180, sy, 12), (-250, sy, 80), 9, 9)
            for sx in (-110, 110):
                g.box(WOOD_D, (sx, sy, 34), (9, 9, 44))
        g.box(WOOD, (10, 160, 62), (300, 120, 14))
        g.box(lead, (10, 104, 92), (300, 8, 56))
        g.box(lead, (10, 216, 92), (300, 8, 56))
        g.box(lead, (160, 160, 110), (8, 120, 92))
        g.box(trim, (10, 100, 92), (240, 5, 10))
        g.box(trim, (10, 220, 92), (240, 5, 10))
        tilt_box(g, lead, (-140, 160, 70), (-190, 160, 130), 120, 8)
        g.box(trim, (100, 160, 88), (90, 100, 22))                       # the seat
        g.box(WOOD_D, (20, 160, 84), (60, 90, 30))                       # a trunk
        g.box(trim, (20, 160, 101), (62, 14, 5))
        mound(g, -330, 160, 80, 70, 28)
        horse = dict(length=150, height=120, girth=42, neck=50, neck_up=55, head=34, tail=46)

        def rug(m):                                                      # the blanket and the bell strap
            m.box(lead, (0, 10, 104), (48, 100, 40))
            m.box(trim, (0, 10, 86), (49.5, 100, 6))
            m.box(trim, (0, -66, 104), (46, 8, 30))
            for sd in (-1, 1):
                m.box(YEL, (sd * 24, -66, 98), (5, 7, 7))
        animal(sc, -150, -60, 6, yaw=0, s=1.15, graze=10, wag=18, rate=0.35, phase=0.2, extra=rug, body=WOOD_D, mane=CHAR,
               ears=WOOD_D, legs_col=CHAR, **dict(horse, neck_up=20))
        animal(sc, 110, -50, 6, yaw=8, s=1.15, stride=5, wag=22, rate=0.7, phase=0.6, extra=rug, body=WOOD, mane=WOOD_D,
               ears=WOOD, legs_col=WOOD_D, **horse)
    return build


def marmots(sc):
    """The ridges: marmots on a pile of lichen boulders among heather. One stands up from behind the top stone and
    drops again, one whistles and turns its head, one scurries along the front, one lies in the sun."""
    g = sc.still()
    pad(g, 500, 400)
    boulder(g, 0, 60, 26, 105, ROCK, 0.62, 10)
    boulder(g, -140, 90, 14, 75, ROCK_D, 0.6, 40)
    boulder(g, 130, 100, 12, 70, CUT, 0.6, -20)
    boulder(g, 20, 130, 70, 46, ROCK_D, 0.7, 0)                         # the top stone the look-out hides behind
    g.box(YEL, (-30, 0, 72), (40, 26, 5), rot=(0, 15, 0))
    g.box(MINT, (60, 20, 70), (28, 22, 5))
    g.box(SNOW, (-140, 100, 56), (90, 70, 8))
    g.box(SNOW, (130, 110, 50), (80, 60, 8))
    for k, (hx, hy) in enumerate(((-210, -60), (200, -20), (-60, -150), (210, 150))):
        tuft(g, hx, hy, 6, VIOLET if k % 2 == 0 else PINE_D, 5, 20, 22, 20 + k)
    g.cyl(CHAR, (-60, -18, 10), 22, 16, steps=7, rot=(60, 90, 0))       # a burrow
    marmot(sc, 20, 96, 58, yaw=10, s=1.5, bob=34, rate=0.22, phase=0.0)                 # up and down behind the stone
    marmot(sc, -110, 30, 60, yaw=-25, s=1.55, swing=22, axis=P.UP, rate=0.4, phase=0.3)  # the whistler
    marmot(sc, 110, 60, 56, yaw=70, s=1.5, up=False, swing=4, axis=70.0, rate=0.5, phase=0.6)
    marmot(sc, -100, -150, 6, yaw=90, s=1.45, up=False, bob=6, rate=1.8, phase=0.2, travel=(380.0, 0.16))
    marmot(sc, 180, -110, 6, yaw=30, s=1.4, swing=6, axis=30.0, rate=0.9, phase=0.8)


def penguins(sc):
    """The glacier and the mine: a huddle of penguins by a melt pool. They waddle on the spot, flippers out; a grey
    chick in the middle; one after another slides to the pool on its belly."""
    g = sc.still()
    pad(g, 800, 500)
    pool(g, 170, -20, 170, ICE_D, ICE_DEEP, SNOW, 7, 3)
    g.box(ICE, (150, 10, 9), (90, 60, 8), rot=(0, 25, 0))               # a floe
    g.box(SNOW_SH, (-120, -150, 7), (360, 40, 4))                       # the slide, polished
    mound(g, -330, 150, 90, 70, 30)
    g.box(WOOD_D, (-330, -190, 30), (6, 6, 70))                         # a route flag of the expedition, bamboo and red
    f = sc.part(P.motion(swing=12, pivot=(-330, -190, 60), axis=P.UP, rate=0.6, phase=0.4))
    f.box(RED, (-308, -190, 56), (40, 4, 22))
    r = random.Random(7)
    spots = ((-250, 60), (-200, 115), (-145, 60), (-235, 0), (-140, 0))
    for k, (px, py) in enumerate(spots):
        yw = r.uniform(-50, 50) + (180 if k == 1 else 0)
        penguin(sc, px, py, 6, yaw=yw, s=r.uniform(1.3, 1.5), swing=r.uniform(5, 8), axis=yw + 90.0, rate=r.uniform(0.7, 1.1), phase=r.random())
    penguin(sc, -195, 62, 6, yaw=0, s=0.95, chick=True, swing=8, axis=90.0, rate=1.3, phase=0.5)
    penguin(sc, 20, 110, 6, yaw=-70, s=1.4, swing=7, axis=20.0, rate=0.8, phase=0.1)          # one at the water's edge
    penguin_slide(sc, -200, -150, 7, s=1.4, travel=(460.0, 0.17), phase=0.0)
    penguin_slide(sc, 0, -150, 7, s=1.3, travel=(460.0, 0.17), phase=0.0)


def seals(sc):
    """The glacier and the mine: seals at a melt pool. One hauled out lifts its head and looks round, one sleeps and
    slaps a flipper, one swims: only its head shows, bobbing in the open water."""
    g = sc.still()
    pad(g, 800, 640)
    pool(g, 0, 40, 230, ICE_D, ICE_DEEP, SNOW, 10, 5)
    g.box(ICE, (-110, 120, 9), (110, 80, 9), rot=(0, 30, 0))
    g.box(ICE, (120, -40, 9), (80, 60, 9), rot=(0, -20, 0))
    mound(g, -340, -200, 80, 60, 26)
    g.box(STEEL, (300, -230, 10), (40, 14, 8), rot=(0, 30, 0))          # a fish on the ice
    g.box(STEEL, (322, -217, 10), (12, 20, 8), rot=(0, 30, 0))
    seal(sc, -250, -160, 6, yaw=70, s=1.55)
    seal_head(sc, -250, -160, 6, yaw=70, s=1.55, swing=16, axis=P.UP, rate=0.3, phase=0.2)
    seal(sc, 250, -150, 6, yaw=-120, s=1.4, col=ROCK)
    seal_head(sc, 250, -150, 6, yaw=-120, s=1.4, col=ROCK)
    fl = sc.part(P.motion(swing=24, pivot=(270, -120, 16), axis=60.0, rate=0.8, phase=0.5))
    fl.box(CHAR, (300, -100, 22), (16, 34, 6), rot=(0, 60, 0))
    seal(sc, 110, 190, 6, yaw=200, s=1.25, col=ROCK_D)
    seal_head(sc, 110, 190, 6, yaw=200, s=1.25, swing=8, axis=200.0, rate=0.5, phase=0.7)
    seal_head(sc, 0, 60, -36, yaw=-20, s=1.5, bob=-8, rate=0.3, phase=0.1)                     # the swimmer
    seal_head(sc, -70, 20, -44, yaw=140, s=1.3, col=ROCK, bob=-7, rate=0.25, phase=0.6)


def foxes(sc):
    """The mine: arctic foxes in their slate summer coat by a den under the rocks. Two trot the length of the piece
    one behind the other, one stands at the den and swings its brush, a cub pounces."""
    g = sc.still()
    pad(g, 1150, 420)
    boulder(g, -40, 130, 16, 110, ROCK, 0.55, 10)
    boulder(g, 100, 150, 10, 70, ROCK_D, 0.6, 30)
    g.box(SNOW, (-30, 140, 70), (150, 90, 9))
    g.cyl(CHAR, (-40, 40, 12), 30, 20, steps=7, rot=(60, 90, 0))
    g.box(WOOD_D, (330, 150, 30), (120, 16, 16))                        # two pit props left lying
    g.box(WOOD, (340, 132, 44), (120, 16, 16), rot=(0, 8, 0))
    mound(g, -420, 140, 110, 70, 30)
    mound(g, 460, 150, 90, 60, 26)
    tuft(g, 230, 60, 6, PINE_D, 4, 18, 20, 9)
    fox = dict(length=70, height=46, girth=20, neck=16, neck_up=32, head=23, body=ROCK_D, belly=STEEL, ears=CHAR, snout=STEEL,
               tail=0, legs_col=CHAR)

    def brush(m):                                                       # the bushy tail, pale at the tip
        tilt_box(m, ROCK_D, (0, 34, 40), (0, 70, 30), 13, 13)
        tilt_box(m, STEEL, (0, 66, 31), (0, 82, 27), 11, 11)
    animal(sc, -300, -120, 6, yaw=90, s=1.55, stride=22, rate=1.9, phase=0.1, travel=(1000.0, 140.0), extra=brush, **fox)
    animal(sc, -90, -150, 6, yaw=90, s=1.45, stride=22, rate=1.9, phase=0.6, travel=(1000.0, 140.0), extra=brush, **fox)
    fl = sc.part(P.motion(swing=26, pivot=_frame(-150, 20, 6, -30.0, 1.5)((0, 34, 40)), axis=P.UP, rate=0.9, phase=0.3), (-150, 20, 6), -30.0, 1.5)
    brush(fl)
    animal(sc, -150, 20, 6, yaw=-30, s=1.5, graze=8, rate=0.9, phase=0.3, **fox)
    animal(sc, 150, 10, 6, yaw=40, s=1.0, bob=22, rate=1.3, phase=0.7, extra=brush, **fox)


def chamois(sc):
    """The avalanche works and the high ridge: chamois on a crag. One stands look-out on the top, one grazes on the
    middle ledge, one walks the long ledge to and fro."""
    g = sc.still()
    g.box(ROCK_D, (0, 60, -20), (860, 520, 560), rot=(0, 6, 0))         # the crag: three steps, 3 m of footing
    g.box(ROCK, (-60, 120, 300), (640, 400, 240), rot=(0, -8, 0))
    g.box(CUT, (120, 170, 470), (360, 280, 200), rot=(0, 14, 0))
    g.box(ROCK, (-330, -120, 60), (300, 240, 320), rot=(0, 20, 0))
    g.box(CUT, (340, -150, 40), (260, 220, 260), rot=(0, -25, 0))
    g.box(ROCK_D, (0, -230, 244), (640, 110, 32))                       # the long ledge
    g.box(SNOW, (120, 170, 578), (330, 250, 18), rot=(0, 14, 0))
    g.box(SNOW, (-250, 140, 428), (220, 300, 16), rot=(0, -8, 0))
    g.box(SNOW, (-330, -120, 228), (270, 210, 16), rot=(0, 20, 0))
    g.box(SNOW, (340, -150, 178), (230, 190, 16), rot=(0, -25, 0))
    g.box(YEL, (-100, -82, 330), (80, 5, 50))                           # lichen on the face
    g.box(MINT, (150, 28, 500), (60, 5, 40), rot=(0, 14, 0))
    tuft(g, -400, -180, 236, PINE_D, 5, 26, 30, 31)
    tuft(g, 250, 60, 586, VIOLET, 5, 20, 22, 32)
    cham = dict(length=95, height=76, girth=28, neck=26, neck_up=58, head=24, body=WOOD, belly=CREAM, ears=WOOD_D, horns=CHAR,
                tail=10, legs_col=CHAR, snout=CREAM)
    animal(sc, 110, 130, 587, yaw=20, s=1.5, graze=5, rate=0.25, phase=0.2, **cham)
    animal(sc, -300, -120, 236, yaw=-50, s=1.4, graze=12, wag=20, rate=0.6, phase=0.5, **dict(cham, neck_up=-30))
    animal(sc, -150, -230, 260, yaw=90, s=1.4, stride=17, rate=1.0, phase=0.1, travel=(460.0, 45.0), **cham)


def yak_train(sc):
    """The white ridge: a yak train on its trodden trail, violet packs under mint straps, the drover ahead with a
    staff; wind streamers on two poles behind. Four yaks and the drover walk the piece's X and come round again."""
    g = sc.still()
    pad(g, 1300, 300, top=10, depth=60)
    g.box(SNOW, (0, -30, 12), (1240, 150, 5))                            # the trodden trail
    for k in range(4):
        mound(g, -420 + k * 280, 120, 130, 50, 30 + (k % 3) * 8)
    for px in (-560, 560):
        g.cyl(WOOD_D, (px, 120, -40), 7, 330, steps=6)
        boulder(g, px, 120, 14, 40, ROCK_D, 0.7)
    for k, px in enumerate((-560, 560)):
        f = sc.part(P.motion(swing=10, pivot=(px, 120, 250), axis=P.UP, rate=0.45, phase=0.2 + k * 0.4))
        for j, c in enumerate((VIOLET, MINT, VIOLET)):
            f.box(c, (px + 60 + j * 14, 120, 272 - j * 34), (110 + j * 28, 4.5, 24))
    yak = dict(length=170, height=118, girth=62, neck=30, neck_up=-8, head=36, body=WOOD_D, ears=WOOD_D, horns=CREAM, tail=50,
               tail_col=CHAR, legs_col=CHAR, snout=WOOD)

    def load(m):                                                        # the shaggy coat, the hump, the packs
        for sd in (-1, 1):
            m.box(CHAR, (sd * 31, 4, 62), (9, 150, 46))
            m.box(VIOLET, (sd * 44, 14, 100), (24, 74, 46))
        m.box(WOOD_D, (0, -52, 124), (42, 54, 20))
        m.box(MINT, (0, 14, 122), (92, 14, 6))
        m.cyl(VIOLET, (-34, 14, 136), 13, 68, steps=6, rot=(-90, 0, 0))
    for k, px in enumerate((-470, -230, 10, 250)):
        animal(sc, px, -30, 14, yaw=90, s=1.1, stride=13, wag=14, rate=0.85, phase=0.25 * k, travel=(1160.0, 52.0), extra=load, **yak)
    P.figure(sc, 470, -30, 14, yaw=90, s=1.18, skin=P.SKINS[3], top=VIOLET, bottom=ASPH, shoes='#22202a', hair='long', hair_col='#2a2230',
             wear='coat', sleeves='long', hat='beanie', hat_col=MINT, band=YEL, legs='stride', stride=18, swing_l=16, swing_r=10,
             axis_l='x', axis_r='x', arm_r='hold', hold_r=P.hold_rod(WOOD_D, 170.0), rate=0.85, phase=0.1, travel=(1160.0, 52.0), detail=1)


def reindeer(sc):
    """The expedition: three reindeer resting in harness by a loaded sledge, their keeper in a yellow parka holding
    out feed. Heads nod, ears and tails flick; the sledge's red route flag swings."""
    g = sc.still()
    pad(g, 900, 560)
    # the sledge, along X at the back
    for sy in (150, 230):
        g.box(WOOD_D, (40, sy, 12), (420, 10, 10))
        tilt_box(g, WOOD_D, (-170, sy, 12), (-250, sy, 90), 10, 10)
        for sx in (-90, 60, 210):
            g.box(WOOD_D, (sx, sy, 34), (10, 10, 44))
    g.box(WOOD, (50, 190, 60), (400, 100, 12))
    g.box(YEL, (-60, 190, 96), (120, 86, 60))                           # crates lashed under a navy sheet
    g.box(YEL, (70, 190, 90), (110, 86, 48))
    g.box(PANE, (180, 190, 98), (110, 92, 64))
    g.box(WOOD_D, (0, 190, 128), (250, 8, 5))
    g.box(WOOD_D, (0, 146, 96), (5, 5, 62))
    g.box(WOOD_D, (120, 146, 96), (5, 5, 62))
    g.cyl(STEEL, (240, 232, 60), 16, 50, steps=7)                       # a fuel drum, steel
    g.cyl(WOOD_D, (-140, 232, 60), 5, 240, steps=6)                     # bamboo and the route flag
    f = sc.part(P.motion(swing=14, pivot=(-140, 232, 280), axis=P.UP, rate=0.55, phase=0.3))
    f.box(RED, (-104, 232, 280), (72, 4.5, 40))
    g = sc.still()
    g.box(RED, (-150, -40, 9), (5, 380, 5), rot=(0, 62, 0))             # the traces lying in the snow
    g.box(RED, (40, -40, 9), (5, 330, 5), rot=(0, 80, 0))
    mound(g, 380, 180, 90, 70, 30)
    g.box(WOOD, (330, -150, 24), (70, 50, 36))                          # the feed box
    g.box(YEL, (330, -150, 44), (60, 40, 6))
    deer = dict(length=130, height=104, girth=40, neck=36, neck_up=42, head=30, body=WOOD, belly=WOOD_D, ears=WOOD_D, tail=12,
                tail_col=CONC, legs_col=WOOD_D, snout=WOOD_D)

    def harness(m):
        m.box(RED, (0, -40, 86), (44, 9, 40))
        m.box(RED, (0, 0, 106), (44, 80, 5))
        for sd in (-1, 1):
            m.box(YEL, (sd * 23, -40, 76), (5, 7, 7))

    def antlers(m, n1):
        for sd in (-1, 1):
            a, b = (sd * 6, n1[1] + 4, n1[2] + 12), (sd * 24, n1[1] + 12, n1[2] + 46)
            tilt_box(m, WOOD_D, a, b, 5.5, 5.5)
            tilt_box(m, WOOD_D, b, (sd * 14, n1[1] + 20, n1[2] + 72), 5, 5)
            tilt_box(m, WOOD_D, b, (sd * 36, n1[1] - 2, n1[2] + 62), 5, 5)
    animal(sc, -260, -60, 6, yaw=-28, s=1.3, graze=7, wag=20, rate=0.4, phase=0.1, extra=harness, head_extra=antlers, **deer)
    animal(sc, -60, -30, 6, yaw=-10, s=1.25, graze=12, wag=16, rate=0.3, phase=0.5, extra=harness, head_extra=antlers, **dict(deer, neck_up=-15))
    animal(sc, 140, -70, 6, yaw=40, s=1.3, graze=9, wag=22, rate=0.5, phase=0.8, extra=harness, head_extra=antlers, **dict(deer, neck_up=20))
    P.figure(sc, 262, -112, 6, yaw=105, s=1.2, skin=P.SKINS[2], top=YEL, bottom=PANE, shoes='#22202a', hair='short', hair_col='#3b2a22',
             wear='coat', sleeves='long', hat='beanie', hat_col=RED, arm_r='fwd', arm_l='down', swing_r=8, axis_r='x', rate=0.4, phase=0.2,
             hold_r=P.hold_cup(YEL), detail=1)


def swans(sc):
    """The valley town and the spa: swans on a pond the frost has left open. Three sail round it, a grey cygnet
    behind the last; one feeds by the reeds, its neck dipping. A little timber duck house on stilts, frozen reeds."""
    g = sc.still()
    pad(g, 700, 620)
    g.cyl(ICE, (0, 10, 1.0), 290, 6.0, steps=12)
    g.cyl(ICE_DEEP, (0, 10, 3.0), 262, 5.5, steps=12)
    r = random.Random(9)
    for k in range(8):
        a = math.radians(k * 360.0 / 8 + r.uniform(-10, 10))
        if math.sin(a) < -0.3:
            continue
        mound(g, math.cos(a) * 300, 10 + math.sin(a) * 292, r.uniform(44, 66), r.uniform(36, 50), r.uniform(18, 30))
    for sx, sy in ((-236, 236), (-176, 236), (-236, 284), (-176, 284)):
        g.box(WOOD_D, (sx, sy, 24), (7, 7, 60))                          # the duck house
    g.box(WOOD, (-206, 260, 72), (90, 76, 46))
    g.box(PANE, (-206, 221, 68), (26, 5, 30))
    g.box(YEL, (-206, 220, 88), (30, 5, 6))
    tilt_box(g, WOOD_D, (-206, 216, 96), (-206, 306, 110), 104, 8)
    tilt_box(g, SNOW, (-206, 222, 104), (-206, 304, 117), 96, 10)
    tilt_box(g, WOOD, (-206, 222, 50), (-206, 170, 8), 30, 5)            # its ramp to the water
    for k, (rx, ry) in enumerate(((210, 190), (238, 150), (-262, 96))):
        rd = sc.part(P.motion(swing=5, pivot=(rx, ry, 6), axis=30.0, rate=0.4, phase=0.2 * k))
        for j in range(3):
            rd.box(WOOD_D, (rx + j * 11 - 16, ry + (j % 2) * 9, 44 + j * 5), (5, 5, 80 + j * 10))
            rd.box(WOOD, (rx + j * 11 - 16, ry + (j % 2) * 9, 92 + j * 10), (8, 8, 20))
    for k, (rad, th, sz) in enumerate(((170, 0.0, 1.55), (170, 55.0, 1.45), (178, 205.0, 1.5))):
        a = math.radians(th)
        swan(sc, math.cos(a) * rad, 10 + math.sin(a) * rad, 6, yaw=th + 180.0, s=sz, base=(0.0, 10.0), travel=(-1.0, 0.028), bob=-2.5, rate=0.3, phase=0.3 * k)
    a = math.radians(232.0)
    swan(sc, math.cos(a) * 180, 10 + math.sin(a) * 180, 6, yaw=232.0 + 180.0, s=0.8, col=CONC, base=(0.0, 10.0), travel=(-1.0, 0.028), bob=-2.5, rate=0.5, phase=0.7)
    fx, fy = 120.0, 150.0
    swan(sc, fx, fy, 6, yaw=-60.0, s=1.5, bob=-2.0, rate=0.35, phase=0.2,
         dip=dict(swing=26, pivot=(fx + 1.5 * 24 * math.sin(math.radians(-60.0)), fy - 1.5 * 24 * math.cos(math.radians(-60.0)), 6 + 1.5 * 20), axis=-60.0, rate=0.35, phase=0.2, bob=-2.0))


# ------------------------------------------------------------------------------------------ the sky pieces
SKY_LEN = 4400.0     # every sky piece flies a straight 44 m along the road and comes round again


def sky_swans(sc):
    """The ridges: a V of seven cream swans, necks out, flying a straight line along the road."""
    for k in range(7):
        row = (k + 1) // 2
        sd = 1 if k % 2 else -1
        sky_bird(sc, 390 - row * 260, sd * row * 190 if k else 0, 150 + row * 22, 270, CREAM, SNOW_SH, YEL, phase=0.13 * k, rate=1.0, flap=24,
                 length=SKY_LEN, speed=620.0, neck=62)


def sky_owl(sc):
    """The avalanche works: one pale owl, gliding a straight line far out, wings almost still."""
    sky_bird(sc, 0, 0, 120, 330, CREAM, SNOW_SH, YEL, phase=0.3, rate=0.35, flap=7, length=SKY_LEN, speed=430.0, bob=-45.0, head=30)


def sky_gulls(sc):
    """The glacier and the mine: five pale gulls strung out along the road, lifting and falling on the wind."""
    r = random.Random(21)
    for k, (gx, gy, gz) in enumerate(((430, -160, 60), (200, 120, 190), (-40, -40, 120), (-250, 200, 240), (-440, -180, 20))):
        sky_bird(sc, gx, gy, gz, 190, CREAM, SNOW_SH, YEL, phase=r.random(), rate=1.3 + 0.1 * (k % 3), flap=20, length=SKY_LEN, speed=560.0, bob=-50.0)


def sky_buntings(sc):
    """The summit: a flock of nine snow buntings, cream with pale grey wings (fix round: brown wings read dark against the sky), dipping along the road."""
    r = random.Random(33)
    for k in range(9):
        sky_bird(sc, r.uniform(-420, 420), r.uniform(-260, 260), r.uniform(20, 260), 120, CREAM, SNOW_SH, YEL, phase=r.random(), rate=1.9, flap=38,
                 length=SKY_LEN, speed=760.0, bob=-30.0)


LOW = dict(grow=(40, 40, 24), grow_down=(40, 40, 0))


def _run(gx, up=24):
    return dict(grow=(gx, 40, up), grow_down=(gx, 40, 0))


def _sky(built, gy, up):
    """built: how far the piece as drawn already reaches along X (cm); the bounds are grown to the line's ends."""
    gx = SKY_LEN / 2.0 + 150.0 - built
    return dict(far=True, grow=(gx, gy, up), grow_down=(gx, gy, 80))


JOBS = [
    ('SM_S1ANIMALS_Hares', hares, _run(300)),
    ('SM_S1ANIMALS_Kennel', kennel, LOW),
    ('SM_S1ANIMALS_Ptarmigan', ptarmigans, _run(220, 12)),
    ('SM_S1ANIMALS_Goats', goats, _run(300)),
    ('SM_S1ANIMALS_SleighPink', sleigh(PINK, CREAM), LOW),
    ('SM_S1ANIMALS_SleighBlue', sleigh(BLUE, YEL), LOW),
    ('SM_S1ANIMALS_Marmots', marmots, _run(230, 50)),
    ('SM_S1ANIMALS_Penguins', penguins, _run(280, 12)),
    ('SM_S1ANIMALS_Seals', seals, dict(grow=(30, 30, 14), grow_down=(30, 30, 0))),
    ('SM_S1ANIMALS_Foxes', foxes, _run(560)),
    ('SM_S1ANIMALS_Chamois', chamois, _run(280)),
    ('SM_S1ANIMALS_YakTrain', yak_train, _run(640)),
    ('SM_S1ANIMALS_Reindeer', reindeer, LOW),
    ('SM_S1ANIMALS_Swans', swans, dict(grow=(120, 120, 10), grow_down=(120, 120, 0))),
    ('SM_S1ANIMALS_SkySwans', sky_swans, _sky(450, 60, 60)),
    ('SM_S1ANIMALS_SkyOwl', sky_owl, _sky(80, 60, 70)),
    ('SM_S1ANIMALS_SkyGulls', sky_gulls, _sky(450, 60, 80)),
    ('SM_S1ANIMALS_SkyBuntings', sky_buntings, _sky(420, 50, 60)),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
