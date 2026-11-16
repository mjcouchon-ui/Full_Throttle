"""FULL THROTTLE (Unreal) the population of Ice Mountain, pass 3, builder p3d: HEIGHTS (15 Nov 2026).

People UP HIGH, so that they clear the road's barrier and show over the pines and the ice: each piece brings its own
structure. Lookout towers, footbridges with walkers crossing (they travel along the piece's X, which is laid along the
road: nothing crosses the road), chalet fronts with two balconies, a rock crag and an ice crag with a summit party, a
judges' tower with score paddles (blocks of colour, never numbers), a lift pylon with mechanics on its service platform
and a chair swinging, an avalanche dam with a row of watchers, a rescue deck on stilts, an ice arch with a roped party
on top and one coming down a rope.

Looks only: no collision, no glow, nothing that blinks. Every piece is TALL (24 m or more from the road; the block
writes none nearer than 52 m). People are 1.9 times life size: the pieces stand 52 to 84 m out. The hill falls 7 in
100 along the road and a piece stands level, so every piece has a foot that goes 1.5 m into the ground.

Meshes: /Game/People/IceMountain/SM_P3D_*   (that prefix is this builder's alone)
Run in the editor:  import ft_people_ice_p3d as K; K.run()         (or K.run(['SM_P3D_Lookout_A']))
The rules that place them: C:/ftwork/p3/L5d.inl (pasted by the integrator at the end of FTPeopleL5.inl).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/IceMountain'

SNOW, SNOW_SH, ICE, ICE_L, ICE_D = '#f4f9ff', '#b9c6ea', '#7fb0e0', '#a8d4f4', '#5a8ccc'
WOOD, WOOD_D, STEEL, STEEL_D = '#9a6a44', '#6a4c36', '#9aa4b8', '#5a6478'
ROCK, ROCK_D, CONC, CONC_D, CREAM = '#8a86a6', '#6f6a8a', '#c9c4d6', '#a39cc2', '#f1e4c8'
RED, BLUE, ORANGE, PINK, TEAL, YEL, LIME, VIOLET = '#ff2d3c', '#2e6bff', '#ff8a2a', '#ff4fb8', '#19b5c9', '#ffd23f', '#b8ff5a', '#8f6bff'
DARK, WHITE = '#22202a', '#fff6ea'
SUITS = (RED, PINK, ORANGE, YEL, LIME, '#2ee6ff', BLUE, VIOLET, '#ff5fa2', '#7dffb0')
LEGS = ('#3a5fa8', WHITE, VIOLET, TEAL, DARK)
BIG = 1.9            # people here are 1.9 times life size: the pieces stand 52 m and more from the road
FOOT = 150.0         # how far every piece goes into the ground (the hill falls 7 in 100 under a level piece)
GROW = dict(grow=(140.0, 140.0, 100.0), grow_down=(140.0, 140.0, 0.0))


def person(r, big=BIG, **fixed):
    """Somebody in a ski suit or a padded coat, a bobble hat or a helmet: one hot colour, big."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear=r.choice(('trousers', 'trousers', 'coat')), top=c, bottom=r.choice((c,) + LEGS),
               hat=r.choice(('beanie', 'beanie', 'helmet', 'cap')), hat_col=r.choice(SUITS), shoes=r.choice((c, WHITE, DARK)),
               hair=r.choice(('short', 'long', 'bun')), s=big * r.uniform(0.93, 1.07))
    d.update(fixed)
    return d


def big_flag(col=RED, sq=WHITE, pole=WHITE, w=190.0, h=110.0, length=280.0):
    """A big flag on a pole in the hand, a field with a square of a second colour: it swings with the arm."""
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + length / 2.0 - 20), (5, 5, length))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + length - 20 - h / 2.0), (w, 3, h))
        m.box(sq, (hd[0] + side * w / 2.0, hd[1], hd[2] + length - 20 - h * 0.5), (h * 0.5, 4.2, h * 0.5))
    return f


def ski_pole(col=DARK, length=125.0, grip=RED):
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1], hd[2] + length / 2.0 - 18), (4.5, 4.5, length))
        m.box(grip, (hd[0], hd[1], hd[2] + length - 18), (11, 11, 5))
    return f


def skis(col=RED, tip=WHITE, length=200.0):
    """A pair of skis stood on end beside the body, held at the hip."""
    def f(m, hd, side):
        for k in (-1, 1):
            m.box(col, (hd[0] + side * 10 + k * 6, hd[1], hd[2] - 40 + length / 2.0), (9, 4, length))
        m.box(tip, (hd[0] + side * 10, hd[1], hd[2] - 40 + length - 12), (22, 5, 24))
    return f


def bell(col=YEL):
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1] - 4, hd[2] - 16), (24, 16, 28))
    return f


def paddle(col=RED, mark=WHITE):
    """A judge's score paddle: a square of colour with a smaller square on it (never a number)."""
    def f(m, hd, side):
        m.box(WOOD_D, (hd[0], hd[1], hd[2] + 22), (5, 5, 60))
        m.box(col, (hd[0], hd[1] - 2, hd[2] + 78), (64, 5, 64))
        m.box(mark, (hd[0], hd[1] - 5, hd[2] + 78), (30, 2, 30))
    return f


def bat(col=ORANGE):
    """A marshal's bat: a plain board on a grip (no light)."""
    def f(m, hd, side):
        m.box(DARK, (hd[0], hd[1], hd[2] + 8), (5, 5, 26))
        m.box(col, (hd[0], hd[1], hd[2] + 40), (30, 5, 44))
    return f


# what a watcher does: big, slow, readable
ACTS = (
    dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=20, rate=1.4),
    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=24, rate=1.2),
    dict(arm_r='wave', swing_r=30, rate=1.1, bob=10),
    dict(arm_l='wave', swing_l=30, rate=0.9, bob=14),
    dict(arm_l='vee', arm_r='vee', swing_l=18, swing_r=-18, bob=12, rate=1.0, poles=True),
    dict(arm_r='fwd', arm_l='hip', swing_r=14, axis_r='x', rate=1.6, bob=6, ring=True),
)


def crowd(sc, r, spots, flags=(RED, YEL), flag_every=4, seed=0, kid_at=(), yaw=0.0, big=BIG):
    """One watcher on each spot (x, y, z), each with its own act, phase and colours."""
    for k, (x, y, z) in enumerate(spots):
        d = person(r, big=big)
        a = dict(ACTS[(k * 5 + seed) % len(ACTS)])
        if a.pop('poles', False):
            c = r.choice((DARK, RED, BLUE))
            a.update(hold_l=ski_pole(c), hold_r=ski_pole(c))
        if a.pop('ring', False):
            a.update(hold_r=bell())
        if flag_every and k % flag_every == 1:
            c = flags[(k // flag_every) % len(flags)]
            a = dict(arm_r='up', arm_l='hip', swing_r=22, rate=0.8, bob=6, hold_r=big_flag(c, WHITE if c != WHITE else RED))
        if k in kid_at:
            d.update(build='kid', s=big * 0.72)
            a.update(bob=26, rate=1.8)
        d.update(a)
        P.figure(sc, x, y, z, yaw=yaw + r.uniform(-14, 14), detail=0, phase=r.random(), react=True, **d)


def mast_flag(sc, x, y, z0, h=320.0, col=RED, sq=WHITE, w=220.0, hh=120.0, phase=0.0, pole=STEEL_D):
    """A mast standing on (x, y, z0) and its flag, which swings round the mast in the wind."""
    g = sc.still()
    g.box(pole, (x, y, z0 + h / 2.0), (9, 9, h))
    m = sc.part(P.motion(phase=phase, rate=0.6, swing=18.0, pivot=(x, y, z0 + h - hh / 2.0), axis=P.UP))
    m.box(col, (x + w / 2.0, y, z0 + h - hh / 2.0), (w, 4, hh))
    m.box(sq, (x + w / 2.0, y, z0 + h - hh / 2.0), (hh * 0.5, 5.4, hh * 0.5))


def rail(sc, x0, x1, y, z, cols, n=6, h=56.0, lift=16.0):
    """A rail along X on a deck at height z: panels of colour and a dark top bar."""
    g = sc.still()
    w = (x1 - x0) / float(n)
    for k in range(n):
        g.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y, z + lift + h / 2.0), (abs(w), 6, h))
    g.box(DARK, ((x0 + x1) / 2.0, y, z + lift + h + 4), (abs(x1 - x0) + 10, 9, 8))


def steps_x(sc, x0, y, top, n=6, col=WOOD_D, width=150.0, run=50.0):
    """Steps going down from a deck at height `top`, away along +X from x0."""
    g = sc.still()
    rise = top / float(n + 1)
    for i in range(n):
        z = top - (i + 1) * rise
        g.box(col, (x0 + (i + 0.5) * run, y, (z - FOOT) / 2.0), (run, width, z + FOOT))


# ------------------------------------------------------------------------------------------ the lookout tower
def lookout(seed=11, roof=RED, trim=YEL, panels=(PINK, WHITE), flag=(RED, WHITE)):
    """A timber lookout tower: four people at the rail of a deck 8 m up, a hut behind them with snow on its roof and
    a flag on a mast, one more climbing the ladder. 7 m square at the foot, 15 m to the flag: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, half = 820.0, 270.0
        for ex in (-1, 1):
            for ey in (-1, 1):
                tilt_box(g, WOOD_D, (ex * (half + 90), ey * (half + 90), -FOOT), (ex * (half - 30), ey * (half - 30), H), 26, 26)
            tilt_box(g, WOOD, (ex * (half + 70), -half - 60, 60), (ex * (half - 20), half - 20, H - 60), 14, 14)
        tilt_box(g, WOOD, (-half - 70, -half - 70, 60), (half - 20, -half + 20, H - 60), 14, 14)
        tilt_box(g, WOOD, (half + 70, -half - 70, 60), (-half + 20, -half + 20, H - 60), 14, 14)
        g.box(WOOD, (0, 0, H - 10), (2 * half + 80, 2 * half + 80, 20))
        for ex in (-1, 1):
            g.box(trim, (ex * (half + 40), 0, H + 48), (6, 2 * half + 80, 64))
        g.box(WOOD, (0, 130, H + 170), (360, 260, 340))                      # the hut
        g.box(DARK, (0, -3, H + 200), (200, 6, 110))
        g.box(roof, (0, 130, H + 355), (460, 360, 30))
        g.box(SNOW, (0, 130, H + 381), (420, 320, 22))
        lx = half + 60                                                       # the ladder up the right-hand side
        for ey in (-40, 30):
            tilt_box(g, WOOD_D, (lx + 110, ey, -FOOT), (lx - 10, ey, H), 8, 8)
        for i in range(7):
            f = (i + 1) / 8.0
            g.box(WOOD_D, (lx + 110 - 120 * (f * H + FOOT) / (H + FOOT), -5, f * H), (6, 76, 6))
        rail(sc, -half - 40, half + 40, -half - 40, H, panels, n=6)
        mast_flag(sc, 150, 130, H + 392, h=330, col=flag[0], sq=flag[1], phase=0.1 * seed)
        spots = [(-190, -190, H), (-60, -165, H), (75, -190, H), (205, -165, H)]
        crowd(sc, r, spots, flags=(YEL, RED), flag_every=4, seed=seed)
        d = person(r, big=BIG * 0.9)
        P.figure(sc, lx + 95, -5, 300, yaw=-90, detail=0, arm_l='up', arm_r='up', phase=r.random(), rate=0.35, bob=-35, **d)
    return build


# ------------------------------------------------------------------------------------------ the footbridge
def bridge(seed=21, ice=False, panels=(RED, WHITE), tower=RED):
    """A footbridge 17 m long and 6 m up, hung from two towers on piers of timber (or of blue ice): five people walk
    across it both ways (along the piece's X: along the road, never towards it) and two stand at the rail and wave."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L, H, W = 1700.0, 600.0, 300.0
        px = L / 2.0 - 150.0
        for ex in (-1, 1):
            if ice:
                g.box(ICE, (ex * px, 20, (H - 20 - FOOT) / 2.0), (360, 380, H - 20 + FOOT))
                g.box(ICE_L, (ex * (px + 70), 70, (H * 0.55 - FOOT) / 2.0), (470, 480, H * 0.55 + FOOT))
                g.box(SNOW, (ex * (px + 160), 70, H * 0.55 + 6), (200, 490, 12))
            else:
                for ey in (-1, 1):
                    tilt_box(g, WOOD_D, (ex * (px + 80), ey * 170, -FOOT), (ex * px, ey * 110, H - 20), 24, 24)
                    tilt_box(g, WOOD_D, (ex * (px - 80), ey * 170, -FOOT), (ex * px, ey * 110, H - 20), 24, 24)
                tilt_box(g, WOOD, (ex * (px - 70), -160, 80), (ex * (px + 70), -160, H - 100), 12, 12)
                g.box(WOOD, (ex * px, 0, H * 0.5), (220, 330, 14))
            for ey in (-1, 1):
                g.box(tower, (ex * px, ey * (W / 2.0 + 14), H + 200), (22, 22, 400))
                tilt_box(g, DARK, (ex * px, ey * (W / 2.0 + 14), H + 395), (0, ey * (W / 2.0 + 14), H + 105), 6, 6)
                tilt_box(g, DARK, (ex * px, ey * (W / 2.0 + 14), H + 395), (ex * (L / 2.0), ey * (W / 2.0 + 14), H + 90), 6, 6)
            g.box(tower, (ex * px, 0, H + 410), (30, W + 60, 24))
            for f in (0.33, 0.66):                                           # hangers on the road's side
                g.box(DARK, (ex * px * f, -W / 2.0 - 14, H + 40 + (105 + 290 * f) / 2.0), (5, 5, 105 + 290 * f - 80))
        g.box(WOOD, (0, 0, H - 10), (L, W, 20))
        g.box(DARK, (0, W / 2.0 + 4, H + 78), (L, 8, 8))
        rail(sc, -L / 2.0, L / 2.0, -W / 2.0 - 4, H, panels, n=10, h=50, lift=14)
        TL = L - 120.0
        for k in range(5):
            d = person(r)
            fwd = (k % 2 == 0)
            if k == 1:
                d.update(arm_l='hip', hold_l=skis(r.choice((RED, YEL, LIME, PINK))))
            if k == 2:
                d.update(pack=r.choice((ORANGE, TEAL, VIOLET)))
            P.figure(sc, -TL / 2.0 + (k + 0.5) * TL / 5.0, 20 if fwd else 100, H, yaw=90 if fwd else -90, detail=0, stride=24,
                     swing_l=0 if k == 1 else 20, swing_r=-20, axis_l='x', axis_r='x', rate=0.9 + 0.06 * k, bob=4, phase=r.random(),
                     travel=(TL, 95.0 if fwd else -80.0), **d)
        crowd(sc, r, [(-90, -100, H), (70, -100, H)], flags=(YEL, PINK), flag_every=2, seed=seed)
    return build


# ------------------------------------------------------------------------------------------ the chalet's balconies
def balcony(seed=31, wall=CREAM, timber=WOOD_D, shut=RED, panels=(RED, WHITE), flag=(YEL, RED)):
    """A chalet's front, three storeys and a gable under snow: four people on the first balcony and three on the
    second, with flags, bells and ski poles; shutters in one hot colour; a flag at the gable. 11 m wide, 15 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Wd, Dp, H = 1100.0, 320.0, 1080.0
        z1, z2 = 280.0, 680.0
        g.box(wall, (0, 0, (z1 - FOOT) / 2.0), (Wd, Dp, z1 + FOOT))
        g.box(timber, (0, 0, (z1 + H) / 2.0), (Wd, Dp, H - z1))
        g.side(timber, [(-Wd / 2.0, H), (Wd / 2.0, H), (0, H + 330)], -Dp / 2.0, Dp / 2.0)
        for ex in (-1, 1):
            tilt_box(g, SNOW, (ex * (Wd / 2.0 + 90), 0, H - 50), (0, 0, H + 350), Dp + 120, 26)
        g.box(wall, (270, 40, H + 250), (80, 80, 240))
        g.box(SNOW, (270, 40, H + 376), (96, 96, 14))
        g.box(DARK, (0, -Dp / 2.0 - 2, 110), (130, 5, 220))                   # the door
        yb = -Dp / 2.0 - 90.0
        for z, w, xs in ((z1, 1000.0, (-330, 0, 330)), (z2, 760.0, (-190, 190))):
            g.box(timber, (0, yb, z - 9), (w, 180, 18))
            for ex in (-1, 1):
                tilt_box(g, timber, (ex * (w / 2.0 - 40), -Dp / 2.0, z - 110), (ex * (w / 2.0 - 40), yb - 70, z - 14), 14, 14)
            for x in xs:
                g.box(DARK, (x, -Dp / 2.0 - 2, z + 160), (110, 5, 200))
                for ex in (-1, 1):
                    g.box(shut, (x + ex * 78, -Dp / 2.0 - 3, z + 160), (40, 6, 200))
        rail(sc, -500, 500, yb - 86, z1, panels, n=8)
        rail(sc, -380, 380, yb - 86, z2, panels, n=6)
        mast_flag(sc, 0, 0, H + 320, h=300, col=flag[0], sq=flag[1], phase=0.07 * seed)
        spots = [(-360, yb - 10, z1), (-120, yb + 10, z1), (120, yb - 10, z1), (360, yb + 10, z1),
                 (-250, yb, z2), (0, yb - 10, z2), (250, yb, z2)]
        crowd(sc, r, spots, flags=(flag[1], flag[0]), flag_every=4, seed=seed, kid_at=(2,))
    return build


# ------------------------------------------------------------------------------------------ the crag
def crag(seed=41, ice=False):
    """A crag of rock (or of blue ice) in blocks, snow on every top: two people on the first ledge, one on the second,
    one on a shoulder, and the summit party at 9.6 m, one of them with a big flag; a flag planted on the top."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        A, B = (ICE, ICE_D) if ice else (ROCK, ROCK_D)
        g.box(B, (0, 60, (420 - FOOT) / 2.0), (900, 700, 420 + FOOT), rot=(0, 8, 0))
        g.box(A, (60, 120, (700 - FOOT) / 2.0), (620, 520, 700 + FOOT), rot=(0, -12, 0))
        g.box(B, (20, 160, (960 - FOOT) / 2.0), (380, 380, 960 + FOOT), rot=(0, 20, 0))
        g.box(A, (-330, 200, (560 - FOOT) / 2.0), (300, 360, 560 + FOOT), rot=(0, 30, 0))
        g.box(B, (380, 220, (300 - FOOT) / 2.0), (320, 420, 300 + FOOT), rot=(0, -25, 0))
        g.box(SNOW, (0, -215, 424), (880, 160, 10), rot=(0, 8, 0))
        g.box(SNOW, (60, -85, 704), (600, 120, 10), rot=(0, -12, 0))
        g.box(SNOW, (20, 160, 965), (390, 390, 12), rot=(0, 20, 0))
        g.box(SNOW, (-330, 200, 565), (310, 370, 12), rot=(0, 30, 0))
        g.box(SNOW, (380, 220, 305), (330, 430, 12), rot=(0, -25, 0))
        tilt_box(g, ORANGE, (-300, -260, 520), (300, -260, 520), 5, 5)       # a fixed rope along the first ledge
        for x in (-300, 0, 300):
            g.box(DARK, (x, -260, 475), (6, 6, 100))
        mast_flag(sc, 150, 250, 970, h=300, col=PINK if ice else YEL, sq=WHITE, phase=0.05 * seed)
        spots = [(-250, -200, 428), (190, -205, 428), (240, -85, 708), (-330, 150, 570), (-70, 110, 970), (95, 140, 970)]
        crowd(sc, r, spots, flags=(RED, YEL), flag_every=4, seed=seed, kid_at=(0,))
    return build


# ------------------------------------------------------------------------------------------ the judges' tower
def judges(seed=51, body=TEAL, trim=YEL, skirt=(PINK, WHITE)):
    """A judges' tower on stilts: three judges behind the counter, each raising a score paddle on a beat of its own,
    and two people on the roof, one with a big flag. Striped skirt, steps at the side. 7 m wide, 15 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, Wd, Dp = 620.0, 700.0, 380.0
        for ex in (-1, 1):
            for ey in (-1, 1):
                g.box(STEEL_D, (ex * (Wd / 2.0 - 30), ey * (Dp / 2.0 - 30), (H - FOOT) / 2.0), (22, 22, H + FOOT))
            g.box(body, (ex * (Wd / 2.0 - 6), 40, H + 230), (12, Dp - 80, 460))
        tilt_box(g, STEEL, (-Wd / 2.0 + 30, -Dp / 2.0 + 30, 40), (Wd / 2.0 - 30, -Dp / 2.0 + 30, H - 230), 10, 10)
        tilt_box(g, STEEL, (Wd / 2.0 - 30, -Dp / 2.0 + 30, 40), (-Wd / 2.0 + 30, -Dp / 2.0 + 30, H - 230), 10, 10)
        for k in range(7):
            g.box(skirt[k % 2], (-Wd / 2.0 + (k + 0.5) * Wd / 7.0, -Dp / 2.0 - 4, H - 110), (Wd / 7.0, 6, 180))
        g.box(WOOD, (0, 0, H - 10), (Wd + 40, Dp + 40, 20))
        g.box(body, (0, Dp / 2.0 - 6, H + 230), (Wd, 12, 460))
        g.box(trim, (0, 0, H + 470), (Wd + 80, Dp + 80, 24))
        g.box(SNOW, (0, 0, H + 492), (Wd + 40, Dp + 40, 20))
        steps_x(sc, Wd / 2.0 + 20, 40, H - 20, n=6)
        rail(sc, -Wd / 2.0, Wd / 2.0, -Dp / 2.0 - 10, H, (body, WHITE), n=6, h=70, lift=10)
        cols = (RED, YEL, LIME)
        for k, x in enumerate((-210, 0, 210)):
            d = person(r, hat='headset', hat_col=DARK)
            P.figure(sc, x, -80, H, yaw=r.uniform(-10, 10), detail=0, arm_r='up', arm_l='hip', swing_r=24, rate=0.55 + 0.17 * k,
                     bob=5, phase=r.random(), react=True, hold_r=paddle(cols[k], WHITE if k != 1 else DARK), **d)
        top = H + 502
        d = person(r)
        P.figure(sc, -150, -40, top, detail=0, arm_r='up', arm_l='hip', swing_r=26, rate=0.7, bob=8, phase=r.random(), react=True,
                 hold_r=big_flag(RED, WHITE, w=230.0, h=130.0, length=330.0), **d)
        d = person(r)
        P.figure(sc, 140, -20, top, detail=0, arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, rate=1.2, bob=22, phase=r.random(),
                 react=True, **d)
    return build


# ------------------------------------------------------------------------------------------ the lift pylon
def pylon(seed=61, tube=STEEL, arm=BLUE):
    """A chair lift's pylon with its service platform 7.6 m up: two mechanics in orange on the platform (one hammers,
    one waves), a third on the ladder, and a chair with two riders swinging from the arm. 11.5 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        TOP, PZ = 1150.0, 760.0
        g.cyl(tube, (0, 60, -FOOT), 42, TOP + FOOT, steps=8)
        g.box(arm, (0, 60, TOP), (760, 44, 50))
        for ex in (-1, 1):
            g.box(DARK, (ex * 340, 60, TOP + 40), (160, 30, 30))
            g.box(YEL, (ex * 340, 60, TOP + 62), (120, 20, 14))
            g.box(DARK, (ex * 340, 60, TOP + 75), (6, 760, 6))                # the cable, running away from the road
        g.box(STEEL_D, (-150, -60, PZ - 8), (460, 300, 16))                   # the platform
        tilt_box(g, STEEL_D, (0, 30, PZ - 200), (-330, -60, PZ - 16), 12, 12)
        tilt_box(g, STEEL_D, (0, 30, PZ - 200), (-60, -190, PZ - 16), 12, 12)
        for ex in (-1, 1):
            g.box(STEEL_D, (ex * 22, 10, (PZ - FOOT) / 2.0), (6, 6, PZ + FOOT))
        for i in range(7):
            g.box(STEEL_D, (0, 10, 60 + i * 100), (50, 5, 5))
        g.box(ORANGE, (-380, -60, PZ + 46), (6, 300, 60))
        rail(sc, -380, 80, -210, PZ, (ORANGE, WHITE), n=4, h=50, lift=20)
        kit = dict(top=ORANGE, bottom=DARK, band=YEL, hat='hard', wear='trousers', shoes=DARK)
        d = person(r, hat_col=WHITE, **kit)
        P.figure(sc, -290, -100, PZ, yaw=20, detail=0, arm_r='fwd', arm_l='hip', swing_r=24, axis_r='x', rate=1.5, phase=r.random(),
                 hold_r=P.hold_tool(), **d)
        d = person(r, hat_col=YEL, **kit)
        P.figure(sc, -110, -120, PZ, detail=0, arm_l='wave', swing_l=30, rate=1.0, bob=6, phase=r.random(), react=True, **d)
        d = person(r, big=BIG * 0.9, hat_col=WHITE, **kit)
        P.figure(sc, 0, -34, 290, yaw=180, detail=0, arm_l='up', arm_r='up', rate=0.3, bob=-45, phase=r.random(), **d)
        cx, cy, cz = 340.0, 60.0, TOP + 70.0                                  # the chair
        sw = dict(swing=7.0, pivot=(cx, cy, cz), axis=90.0)
        ch = sc.part(P.motion(phase=0.4, rate=0.5, **sw))
        ch.box(STEEL_D, (cx, cy, cz - 170), (8, 8, 340))
        ch.box(RED, (cx, cy, cz - 340), (220, 60, 12))
        ch.box(RED, (cx, cy + 30, cz - 290), (220, 10, 90))
        for k, ox in enumerate((-52, 52)):
            d = person(r, s=1.5)
            d.update(legs='sit', arm_r='wave' if k else 'down', arm_l='down' if k else 'wave')
            P.figure(sc, cx + ox, cy, cz - 340 - 46 * 1.5, detail=0, phase=0.4, rate=0.5, sway=sw, **d)
    return build


# ------------------------------------------------------------------------------------------ the avalanche dam
def dam(seed=71, net=ORANGE, flag=(RED, WHITE)):
    """An avalanche dam in three concrete steps with steel ribs down its face, snow on the steps: eight watchers in a
    row on the walkway 6 m up, behind a rail of orange and white, two of them with flags. 15 m long: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L = 1500.0
        g.box(CONC, (0, -200, (220 - FOOT) / 2.0), (L, 200, 220 + FOOT))
        g.box(CONC_D, (0, -40, (420 - FOOT) / 2.0), (L - 80, 200, 420 + FOOT))
        g.box(CONC, (0, 160, (600 - FOOT) / 2.0), (L - 160, 300, 600 + FOOT))
        g.box(SNOW, (0, -220, 225), (L, 160, 10))
        g.box(SNOW, (0, -65, 425), (L - 80, 150, 10))
        for i in range(5):
            x = -L / 2.0 + 190 + i * (L - 380) / 4.0
            tilt_box(g, STEEL_D, (x, -310, 0), (x, 6, 590), 50, 30)
        H = 600.0
        rail(sc, -L / 2.0 + 110, L / 2.0 - 110, 22, H, (net, WHITE), n=9, h=50, lift=20)
        mast_flag(sc, L / 2.0 - 150, 250, H, h=380, col=flag[0], sq=flag[1], phase=0.03 * seed)
        n = 8
        span = L - 420.0
        spots = [(-span / 2.0 + (i + 0.5) * span / n + r.uniform(-15, 15), 100 + (i % 2) * 60, H) for i in range(n)]
        crowd(sc, r, spots, flags=(YEL, RED), flag_every=4, seed=seed, kid_at=(3,))
    return build


# ------------------------------------------------------------------------------------------ the rescue deck
def helideck(seed=81):
    """The mountain rescue's landing deck on stilts, 5 m up, no aircraft on it: a marshal waving two bats, two
    carrying a sled stretcher, a lookout pointing, one waving; a windsock on a mast (no lights). TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, R = 520.0, 430.0
        g.cyl(STEEL_D, (0, 0, H - 24), R, 24, steps=8)
        g.cyl(ORANGE, (0, 0, H), R * 0.74, 3, steps=8)
        g.cyl(STEEL, (0, 0, H + 3), R * 0.6, 3, steps=8)
        g.cyl(YEL, (0, 0, H + 6), R * 0.2, 3, steps=8)
        g.cyl(STEEL, (0, 0, -FOOT), 30, H - 24 + FOOT, steps=6)
        for a in (45, 135, 225, 315):
            c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
            tilt_box(g, STEEL, (c * R * 0.95, s * R * 0.95, -FOOT), (c * R * 0.6, s * R * 0.6, H - 24), 22, 22)
        for k in range(8):                                                   # the safety net round the rim
            a = k * 45.0 + 22.5
            c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
            g.box(ORANGE if k % 2 else WHITE, (c * (R * 0.924 + 28), s * (R * 0.924 + 28), H - 12), (R * 0.78, 56, 6), rot=(0, a + 90.0, 0))
        steps_x(sc, R * 0.9, 0, H - 30, n=6, col=STEEL_D)
        g.box(STEEL_D, (-290, 290, H + 170), (8, 8, 340))                      # the windsock
        m = sc.part(P.motion(phase=0.2, rate=0.5, swing=22.0, pivot=(-290, 290, H + 330), axis=P.UP))
        rod(m, ORANGE, (-290, 290, H + 330), (-200, 290, H + 326), 30, steps=6)
        rod(m, WHITE, (-200, 290, H + 326), (-120, 290, H + 320), 25, steps=6)
        rod(m, ORANGE, (-120, 290, H + 320), (-50, 290, H + 312), 19, steps=6)
        g = sc.still()
        g.box(ORANGE, (0, 90, H + 9 + BIG * 86), (230, 70, 20))                # the sled stretcher between its bearers
        g.box(WHITE, (0, 90, H + 9 + BIG * 86 + 22), (180, 56, 26))
        Z = H + 9
        kit = dict(top=RED, bottom=RED, band=YEL, hat='helmet', hat_col=WHITE, wear='trousers', shoes=DARK)
        d = person(r, **kit)
        P.figure(sc, 0, -250, Z, detail=0, arm_l='up', arm_r='up', swing_l=22, swing_r=-22, rate=1.0, bob=6, phase=r.random(), react=True,
                 hold_l=bat(), hold_r=bat(), **d)
        for ex in (-1, 1):
            d = person(r, **kit)
            P.figure(sc, ex * 185, 90, Z, yaw=-90 * ex, detail=0, arm_l='carry', arm_r='carry', rate=1.0, bob=4, phase=0.3, **d)
        d = person(r, **kit)
        P.figure(sc, -260, -90, Z, yaw=-25, detail=0, arm_r='face', arm_l='point', swing_l=12, rate=0.6, phase=r.random(), react=True, **d)
        d = person(r, **kit)
        P.figure(sc, 260, -110, Z, yaw=20, detail=0, arm_r='wave', swing_r=30, rate=1.1, bob=10, phase=r.random(), react=True, **d)
    return build


# ------------------------------------------------------------------------------------------ the ice arch
def icearch(seed=91):
    """An arch of blue ice 9.5 m high, snow on top: a roped party of four on the top (ice axes, a big flag), and a
    fifth coming down a rope in front of the arch, slowly up and down. 16 m wide: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 760.0
        for ex in (-1, 1):
            g.box(ICE, (ex * 470, 40, (H - FOOT) / 2.0), (330, 420, H + FOOT), rot=(0, ex * 10.0, 0))
            g.box(ICE_L, (ex * 650, 80, (H * 0.5 - FOOT) / 2.0), (300, 380, H * 0.5 + FOOT), rot=(0, -ex * 14.0, 0))
            g.box(SNOW, (ex * 650, 80, H * 0.5 + 6), (310, 390, 12), rot=(0, -ex * 14.0, 0))
            g.box(ICE_D, (ex * 300, 40, H - 60), (200, 340, 120))
        g.box(ICE_L, (0, 40, H + 90), (1300, 380, 180))
        g.box(SNOW, (0, 40, H + 187), (1320, 400, 14))
        for x, ln in ((-180, 120), (-90, 70), (10, 150), (110, 90), (200, 130)):
            g.box(ICE_L, (x, -120, H - ln / 2.0), (16, 16, ln))
        top = H + 194.0
        xs = (-430, -160, 120, 400)
        for k in range(3):                                                   # the party's rope
            tilt_box(g, ORANGE, (xs[k], -60, top + BIG * 72), (xs[k + 1], -60, top + BIG * 72), 5, 5)
        for k, x in enumerate(xs):
            d = person(r, pack=r.choice((ORANGE, TEAL, VIOLET, YEL)), hat='helmet')
            if k == 1:
                a = dict(arm_r='up', arm_l='hip', swing_r=24, rate=0.75, bob=6, hold_r=big_flag(RED, WHITE, w=220.0, h=125.0, length=320.0))
            elif k == 3:
                a = dict(arm_l='vee', arm_r='vee', swing_l=16, swing_r=-16, rate=1.2, bob=18)
            else:
                a = dict(arm_r='wave', arm_l='down', swing_r=26, rate=0.9 + 0.1 * k, bob=8, hold_r=P.hold_tool(col=STEEL, grip=RED, length=50.0))
            d.update(a)
            P.figure(sc, x, -70, top, yaw=r.uniform(-12, 12), detail=0, phase=r.random(), react=True, **d)
        ph = r.random()
        m = sc.part(P.motion(phase=ph, rate=0.22, bob=-60.0))
        m.box(ORANGE, (60, -170, (H + 60 + 480) / 2.0), (5, 5, H + 60 - 480))
        d = person(r, s=1.6, hat='helmet', pack=YEL)
        P.figure(sc, 60, -185, 200, yaw=180, detail=0, arm_l='up', arm_r='up', phase=ph, rate=0.22, bob=-60, **d)
    return build


JOBS = [
    ('SM_P3D_Lookout_A', lookout(), GROW),
    ('SM_P3D_Lookout_B', lookout(seed=13, roof=TEAL, trim=PINK, panels=(YEL, TEAL), flag=(PINK, WHITE)), GROW),
    ('SM_P3D_Bridge_A', bridge(), GROW),
    ('SM_P3D_Bridge_B', bridge(seed=23, ice=True, panels=(ORANGE, WHITE), tower=ORANGE), GROW),
    ('SM_P3D_Balcony_A', balcony(), GROW),
    ('SM_P3D_Balcony_B', balcony(seed=37, wall=WHITE, timber=WOOD, shut=TEAL, panels=(TEAL, YEL), flag=(PINK, WHITE)), GROW),
    ('SM_P3D_Crag_A', crag(), GROW),
    ('SM_P3D_Crag_B', crag(seed=43, ice=True), GROW),
    ('SM_P3D_Judges_A', judges(), GROW),
    ('SM_P3D_Pylon_A', pylon(), GROW),
    ('SM_P3D_Dam_A', dam(), GROW),
    ('SM_P3D_HeliDeck_A', helideck(), GROW),
    ('SM_P3D_IceArch_A', icearch(), GROW),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
