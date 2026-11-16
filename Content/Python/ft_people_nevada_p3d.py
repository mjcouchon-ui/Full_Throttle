"""FULL THROTTLE (Unreal) the population of level 3, Nevada (level index 2), pass 3, builder p3d: HEIGHTS.
/Game/People/Nevada/SM_P3D_*.

People UP HIGH, so that they clear the road's barrier and the land that lies under the road: every piece brings its
own structure and stands 44 m out or more (all of them are TALL). The figures are built big (S = 1.4) and the table
makes the pieces bigger still, so that a person on a tower reads from the road.

    SM_P3D_Scaffold_A    a spectators' scaffold tower, three decks of ranch folk, a flag as big as a deck
    SM_P3D_Scaffold_B    a wider one of two decks under a striped awning, in the strip's colours
    SM_P3D_WaterTower    a desert water tower: people round the catwalk, one sitting with legs over the edge, one on
                         the ladder going up and down, a wind vane turning on the roof
    SM_P3D_Billboard     a billboard being repainted (bands and a sun, no words): two painters with rollers on the
                         catwalk, onlookers beside them, one sitting on the top edge with a flag
    SM_P3D_RockSpire_A   a red rock pinnacle, 13 m: climbers on the summit with a flag, two on a ledge, one on the rope
    SM_P3D_RockSpire_B   the same on a 21 m needle (for the walled stretches: the rock cuts and the pass)
    SM_P3D_MesaLedge     a low mesa with a tailgate on top: a row at the edge, a striped canopy, a telescope, a dog
    SM_P3D_RoofParty     a flat-roofed motel block: dancers on the roof, one on the parapet, two on the stair house
    SM_P3D_SignRig       the strip's riggers: a sunburst sign in its frame (unlit), two in a cradle that rises and falls
                         before it, a gang on the scaffold deck, one sitting on the top beam
    SM_P3D_Trestle       a timber trestle along the road: two on a pump trolley travelling its length, three waving
                         from the water stop's platform
    SM_P3D_Derrick       a wildcat drilling derrick: hands on the working floor and the crown, the walking beam nodding
    SM_P3D_Lookout       a fire lookout on stilts: watchers along the gallery with field glasses, a flag on the roof
    SM_P3D_RVRoof        a camper van with a roof deck: tourists with cameras under a parasol, one sitting on the edge

Everything moves through M_People (ft_people.py). No words, no marks, no light on any piece.
The placing rules are C:/ftwork/p3/L2d.inl (the integrator pastes them into FTPeopleL2.inl).
    in the editor:   import ft_people_nevada_p3d as K; K.run()
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/Nevada'

SAND, SANDST, SANDST_D, SANDST_L, ROCK_R, TALUS = '#f2c58a', '#e0784a', '#b8553a', '#f59a62', '#c9603e', '#a04a34'
BARK, BARK_D, WOOD, WOOD_L = '#8a6a4a', '#6a4c36', '#9a6a44', '#c9955f'
LEATHER, NAVY, PURP, MAG, TEAL, RED, CYAN = '#22202a', '#2b2350', '#7b3bd6', '#ff4fb8', '#2fc6c0', '#ff2d3c', '#2ee6ff'
CREAM, WHITE, GOLD, YEL, STEEL, CHROME = '#f4efe6', '#fff6ea', '#ffc93c', '#ffe14d', '#8f9bb3', '#d7e3f0'
DENIM, RUST, MINT, PINK, LEMON, GLASS = '#3a5fa8', '#b8553a', '#7fe3c4', '#ff9ec4', '#ffe9a8', '#3a4a6a'
HOT = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a', '#ff2d3c')
HATS = (CREAM, WHITE, GOLD, RED, BARK)
S = 1.4             # a figure's size as built (the table makes the piece bigger still)
GROW = dict(grow=(120.0, 80.0, 140.0), grow_down=(120.0, 80.0, 0.0))


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_hat(col=CREAM):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 10), (84, 84, 6))
        m.box(col, (h[0], h[1], h[2] + 24), (40, 40, 24))
    return f


def hold_flag(col, stripe, w=120.0, hgt=64.0):
    def f(m, h, side):
        m.box(CREAM, (h[0], h[1], h[2] + 70), (5, 5, 170))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + 122), (w, 4, hgt))
        m.box(stripe, (h[0] + side * w / 2.0, h[1] - 3, h[2] + 122), (w, 2, hgt * 0.34))
    return f


def hold_glasses(col=LEATHER):
    """Field glasses at the face."""
    def f(m, h, side):
        m.box(col, (h[0] - side * 10, h[1] - 8, h[2] + 4), (30, 22, 14))
    return f


def hold_roller(col):
    """A paint roller on a long pole."""
    def f(m, h, side):
        m.box(WOOD_L, (h[0], h[1], h[2] + 50), (5, 5, 110))
        m.box(col, (h[0], h[1] - 4, h[2] + 110), (44, 14, 14))
    return f


# ------------------------------------------------------------------------------------------ the cast
def acts_for(r):
    """The ways somebody up high greets the chase: all big enough to read at 50 m."""
    c1, c2 = r.sample(HOT, 2)
    return [
        dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=12, rate=1.5),
        dict(arm_r='wave', hold_r=hold_hat(r.choice(HATS)), swing_r=30, rate=1.2, bob=6),
        dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=14, rate=1.25),
        dict(arm_r='up', hold_r=hold_flag(c1, c2), swing_r=24, rate=0.9, bob=5),
        dict(arm_l='wave', arm_r='wave', swing_l=24, swing_r=-24, rate=1.1, bob=8),
        dict(arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.7, bob=3),
        dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.4),
    ]


def who(r, **fixed):
    d = P.look(r, tops=HOT)
    d['s'] = S * r.uniform(0.94, 1.06)
    if r.random() < 0.3:
        d.update(wear=r.choice(('dress', 'shorts', 'skirt')))
    d.update(fixed)
    return d


def put(sc, r, x, y, z, act=None, detail=1, yaw=0.0, **fixed):
    """One of the cast at (x, y, z) doing `act` (a dict of figure() values), out of step with everybody else."""
    kw = dict(phase=0.03 + 0.94 * r.random(), rate=0.5, react=True, detail=detail)
    kw.update(who(r, **fixed))
    if kw.get('build') == 'kid':
        kw['s'] = S * 0.7
    if act:
        kw.update(act)
    return P.figure(sc, x, y, z=z, yaw=yaw + r.uniform(-12, 12), **kw)


def sitter(sc, r, x, y_edge, z_top, act=None, detail=1, **fixed):
    """Somebody sitting on an edge z_top high whose front is y_edge, legs dangling over it."""
    kw = dict(who(r, **fixed))
    s = kw['s']
    a = dict(arm_l='wave', arm_r='wave', swing_l=22, swing_r=-22, rate=1.0)
    if act:
        a = dict(act)
    a.pop('bob', None)
    a.pop('rock', None)
    kw.update(a)
    kw.update(legs='sit', seat=46.0, phase=0.03 + 0.94 * r.random(), react=True, detail=detail)
    return P.figure(sc, x, y_edge + 16.0 * s, z=z_top - 46.0 * s, yaw=r.uniform(-8, 8), **kw)


def hatted(r, kinds=('cowboy', 'sun', 'cap'), p=0.6):
    if r.random() < p:
        return dict(hat=r.choice(kinds), hat_col=r.choice(HATS))
    return {}


# ------------------------------------------------------------------------------------------ furniture
def big_flag(sc, x, y, z0, height, col, stripe, w=260.0, h=150.0, phase=0.0, pole=CREAM):
    """A flag on a pole: the cloth swings round the pole."""
    g = sc.still()
    g.box(pole, (x, y, z0 + height / 2.0), (9, 9, height))
    f = sc.part(P.motion(phase=phase, rate=0.55, swing=24.0, pivot=(x, y, z0 + height), axis=P.UP))
    f.box(col, (x + w / 2.0, y, z0 + height - h / 2.0), (abs(w), 5, h))
    f.box(stripe, (x + w / 2.0, y - 3.5, z0 + height - h / 2.0), (abs(w), 2, h * 0.3))
    f.box(stripe, (x + w / 2.0, y + 3.5, z0 + height - h / 2.0), (abs(w), 2, h * 0.3))


def bunting(g, x0, x1, y, z, n=8, drop=60.0):
    g.box(CREAM, ((x0 + x1) / 2.0, y, z), (abs(x1 - x0), 5, 5))
    for k in range(n):
        px = x0 + (k + 0.5) * (x1 - x0) / n
        w = abs(x1 - x0) / n * 0.42
        g.side(HOT[(k * 3) % len(HOT)], [(px - w, z), (px + w, z), (px, z - drop)], y - 3, y + 3)


def ladder(g, x, y, z0, z1, col=BARK_D, step=90.0):
    for sx in (-1, 1):
        g.box(col, (x + sx * 22, y, (z0 + z1) / 2.0), (6, 6, z1 - z0))
    n = int((z1 - z0) / step)
    for k in range(n):
        g.box(col, (x, y, z0 + (k + 0.5) * (z1 - z0) / n), (44, 5, 5))


def wheel(g, x, y, z, rad, w, col=LEATHER, hub=CHROME):
    pts = [(x + math.cos(a * math.pi / 4.0) * rad, z + math.sin(a * math.pi / 4.0) * rad) for a in range(8)]
    g.side(col, pts, y - w / 2.0, y + w / 2.0)
    pts = [(x + math.cos(a * math.pi / 3.0) * rad * 0.45, z + math.sin(a * math.pi / 3.0) * rad * 0.45) for a in range(6)]
    g.side(hub, pts, y - w / 2.0 - 2, y + w / 2.0 + 2)


def parasol(g, x, y, z, col, col2, rad=150.0, height=250.0):
    g.box(CREAM, (x, y, z + height / 2.0), (6, 6, height))
    g.cyl(col, (x, y, z + height - 40), rad, 40, steps=8, top_r=8.0)
    g.cyl(col2, (x, y, z + height - 46), rad * 1.02, 7, steps=8)


# ------------------------------------------------------------------------------------------ the scaffold towers
def scaffold(seed, kind):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        W, D = (560.0, 240.0) if kind == 0 else (780.0, 240.0)
        levels = (290.0, 600.0, 910.0) if kind == 0 else (300.0, 620.0)
        cols = (RED, CREAM, DENIM) if kind == 0 else (PURP, LEMON, TEAL)
        top = levels[-1] + (360.0 if kind else 150.0)
        xs = (-W / 2.0, W / 2.0) if kind == 0 else (-W / 2.0, 0.0, W / 2.0)
        for x in xs:
            for y in (-D / 2.0, D / 2.0):
                g.box(STEEL, (x, y, (top - 300.0) / 2.0), (12, 12, top + 300.0))
        for k, z in enumerate(levels):
            g.box(WOOD_L, (0, 0, z - 6), (W + 40, D + 20, 12))
            g.box(cols[k % 3], (0, -D / 2.0 - 12, z - 28), (W + 44, 6, 44))
            g.box(CREAM, (0, -D / 2.0 - 8, z + 122), (W + 20, 6, 6))
            z0 = levels[k - 1] if k else -40.0
            for sx in (-1, 1):
                a, b = ((sx * W / 2.0, -D / 2.0, z0), (sx * W / 2.0, D / 2.0, z - 14))
                tilt_box(g, STEEL, a if k % 2 else (a[0], b[1], a[2]), b if k % 2 else (b[0], a[1], b[2]), 7, 7)
        tilt_box(g, STEEL, (-W / 2.0, -D / 2.0, 0), (xs[1], -D / 2.0, levels[0] - 60), 7, 7)
        tilt_box(g, STEEL, (xs[1], -D / 2.0, 0), (-W / 2.0, -D / 2.0, levels[0] - 60), 7, 7)
        if kind:
            zt = levels[-1] + 340.0
            for k in range(6):
                g.box(cols[0] if k % 2 else cols[1], (-W / 2.0 - 20 + (k + 0.5) * (W + 40) / 6.0, -10, zt), ((W + 40) / 6.0, D + 80, 9))
            g.box(cols[2], (0, -D / 2.0 - 50, zt - 22), (W + 40, 5, 36))
        else:
            big_flag(sc, W / 2.0, D / 2.0, levels[-1], 470.0, cols[0], cols[1], w=-270.0, phase=0.3)
        acts = acts_for(r)
        per = (3, 2, 2) if kind == 0 else (4, 3)
        i = 0
        for k, z in enumerate(levels):
            n = per[k]
            for j in range(n):
                x = -W / 2.0 + 70 + (j + 0.5) * (W - 140) / n + r.uniform(-12, 12)
                fixed = hatted(r, ('cowboy', 'cap') if kind == 0 else ('sun', 'cap'))
                if i == 4:
                    fixed.update(build='kid')
                put(sc, r, x, -D / 2.0 + 44, z, acts[(i * 3 + kind) % len(acts)], detail=1 if (k == 0 or j == 0) and i < 4 else 0, **fixed)
                i += 1
    return build


# ------------------------------------------------------------------------------------------ the water tower
def water_tower(sc):
    r = random.Random(411)
    g = sc.still()
    Z = 600.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, BARK_D, (sx * 300, sy * 300, -300), (sx * 245, sy * 245, Z - 10), 20, 20)
    tilt_box(g, BARK, (-290, -292, 0), (255, -252, Z - 40), 10, 10)
    tilt_box(g, BARK, (290, -292, 0), (-255, -252, Z - 40), 10, 10)
    for sx in (-1, 1):
        tilt_box(g, BARK, (sx * 292, -290, 0), (sx * 252, 255, Z - 40), 10, 10)
    g.box(WOOD_L, (0, 0, Z - 6), (640, 640, 12))
    g.cyl(CREAM, (0, 0, Z), 212, 290, steps=10)
    g.cyl(TEAL, (0, 0, Z + 110), 216, 70, steps=10)
    g.cyl(RUST, (0, 0, Z + 290), 232, 90, steps=10, top_r=30.0)
    g.box(CREAM, (0, -314, Z + 122), (640, 6, 6))
    for sx in (-1, 1):
        g.box(CREAM, (sx * 314, 0, Z + 122), (6, 640, 6))
        g.box(BARK_D, (sx * 314, -314, Z + 62), (8, 8, 124))
    ladder(g, 285, -330, -40, Z + 120)
    # the wind vane turns on the roof
    g.box(STEEL, (0, 0, Z + 420), (6, 6, 90))
    v = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, 0.12)))
    v.box(RED, (10, 0, Z + 462), (190, 6, 10))
    v.box(RED, (-80, 0, Z + 462), (50, 6, 60))
    acts = acts_for(r)
    for j, x in enumerate((-235, -95, 45)):
        put(sc, r, x, -262, Z, acts[(j * 2 + 1) % len(acts)], detail=1, **hatted(r))
    sitter(sc, r, 185, -320, Z, None, detail=1, **hatted(r))
    # one on the ladder, going up and down
    put(sc, r, 285, -362, 240, dict(arm_l='up', arm_r='up', bob=-70, rate=0.22, swing_l=8, swing_r=-8, axis_l='x', axis_r='x'),
        detail=0, yaw=180, hat='cowboy', hat_col=BARK)


# ------------------------------------------------------------------------------------------ the billboard
def billboard(sc):
    r = random.Random(423)
    g = sc.still()
    Z0, Z1, W = 440.0, 880.0, 980.0
    for sx in (-1, 1):
        g.box(BARK_D, (sx * 320, 20, (Z1 - 300) / 2.0), (28, 28, Z1 + 300))
        tilt_box(g, BARK, (sx * 320, 260, -40), (sx * 320, 30, Z0 + 120), 14, 14)
    g.box(CREAM, (0, 0, (Z0 + Z1) / 2.0), (W, 14, Z1 - Z0))
    # the picture: a desert sunset in bands, a sun, a mesa (no words); the left third already in its new colours
    for k, col in enumerate(('#ff7a3c', PINK, LEMON, CYAN)):
        g.box(col, (120, -8, Z0 + 55 + k * 110), (W - 280, 3, 110))
    g.cyl(GOLD, (160, -9, Z0 + 235), 120, 5, rot=(0, 0, 90), steps=10)
    g.side(PURP, [(-140, Z0), (-60, Z0 + 150), (120, Z0 + 150), (170, Z0 + 90), (330, Z0 + 90), (400, Z0)], -16, -10)
    g.box(MINT, (-360, -8, (Z0 + Z1) / 2.0), (220, 3, Z1 - Z0 - 20))
    g.box(MAG, (-360, -10, Z0 + 330), (220, 3, 70))
    g.box(BARK_D, (0, -4, Z1 + 8), (W + 30, 22, 16))
    g.box(BARK_D, (0, -4, Z0 - 8), (W + 30, 22, 16))
    # the catwalk
    ZC = Z0 - 20.0
    g.box(WOOD_L, (0, -80, ZC - 6), (W + 60, 150, 12))
    for x in (-420, 0, 420):
        tilt_box(g, BARK, (x, -150, ZC - 12), (x, 6, ZC - 150), 9, 9)
    g.box(CREAM, (0, -152, ZC + 122), (W + 60, 6, 6))
    for x in (-510, 510):
        g.box(CREAM, (x, -152, ZC + 60), (6, 6, 124))
    g.cyl(MINT, (-250, -60, ZC), 16, 30, steps=6)
    g.cyl(MAG, (-215, -40, ZC), 16, 30, steps=6)
    paint = dict(arm_r='up', arm_l='hip', swing_r=16, axis_r='x', rate=0.8, react=False)
    put(sc, r, -400, -70, ZC, dict(paint, hold_r=hold_roller(MINT)), detail=1, yaw=180, hat='cap', hat_col=WHITE, top=WHITE, bottom=CREAM)
    put(sc, r, -130, -70, ZC, dict(paint, hold_r=hold_roller(MAG)), detail=1, yaw=180, hat='cap', hat_col=WHITE, top=WHITE, bottom=CREAM)
    acts = acts_for(r)
    put(sc, r, 90, -100, ZC, acts[1], detail=1, **hatted(r))
    put(sc, r, 230, -100, ZC, acts[0], detail=0, build='kid')
    sitter(sc, r, 390, -155, ZC, None, detail=1, **hatted(r))
    # one sitting on the board's top edge with a flag
    sitter(sc, r, 300, -16, Z1 + 16, dict(arm_r='up', arm_l='down', hold_r=hold_flag(RED, CREAM), swing_r=22, rate=0.9), detail=0)


# ------------------------------------------------------------------------------------------ the rock pinnacle
def rock_spire(seed, tall):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        U = 800.0 if tall else 0.0
        if tall:
            g.cyl(TALUS, (0, 0, -300), 430, 300 + U, steps=7, top_r=330.0)
            g.cyl(SANDST_D, (-10, 10, U - 60), 340, 70, steps=7, top_r=335.0)
        g.cyl(SANDST_D if tall else TALUS, (0, 0, U - 300 if not tall else U), 360 if not tall else 325, 800 if not tall else 500, steps=7, top_r=300.0)
        g.cyl(SANDST, (20, 10, U + 500), 290, 420, steps=7, top_r=232.0)
        g.cyl(ROCK_R, (20, 10, U + 920), 242, 60, steps=7, top_r=236.0)
        g.cyl(SANDST_L, (30, 0, U + 980), 226, 320, steps=7, top_r=192.0)
        g.cyl(SANDST_D, (30, 0, U + 1300), 206, 40, steps=7)
        ZS = U + 1340.0
        # a ledge on the face, and the rope from the cap
        ZL = U + 920.0
        g.box(SANDST_D, (-70, -290, ZL - 20), (300, 190, 40))
        g.box(TALUS, (-70, -270, ZL - 70), (220, 150, 60))
        g.box(BARK_D, (190, -215, ZS + 6), (10, 110, 10))
        g.box(CREAM, (190, -262, (ZS + U + 470) / 2.0), (5, 5, ZS - U - 470))
        big_flag(sc, 60, 60, ZS, 420.0, RED if not tall else MAG, CREAM, w=250.0, phase=0.2)
        acts = acts_for(r)
        helm = dict(hat='helmet')
        put(sc, r, -80, -110, ZS, acts[0], detail=1, pack=r.choice(HOT), **helm)
        put(sc, r, 50, -135, ZS, acts[3], detail=1, hat_col=YEL, **helm)
        put(sc, r, 150, -70, ZS, acts[4], detail=0, pack=r.choice(HOT))
        put(sc, r, -150, -300, ZL, acts[6], detail=1, **hatted(r, ('cap', 'sun')))
        sitter(sc, r, -20, -386, ZL, None, detail=0, pack=r.choice(HOT))
        # the climber on the rope, going up and down
        put(sc, r, 190, -290, U + 640, dict(arm_l='up', arm_r='up', bob=-80, rate=0.2, swing_l=8, swing_r=-8, axis_l='x', axis_r='x'),
            detail=0, yaw=180, hat='helmet', hat_col=RED, top=CYAN, pack=GOLD)
    return build


# ------------------------------------------------------------------------------------------ the mesa and its tailgate
def mesa_ledge(sc):
    r = random.Random(437)
    g = sc.still()
    low = [(-880, -300), (-520, -350), (-60, -320), (420, -360), (860, -290), (900, 120), (500, 330), (-200, 300), (-760, 340), (-930, 60)]
    mid = [(-800, -250), (-460, -290), (-40, -265), (400, -300), (790, -240), (820, 90), (460, 270), (-190, 250), (-700, 280), (-850, 40)]
    cap = [(-770, -230), (-440, -268), (-30, -245), (390, -278), (760, -220), (790, 80), (440, 250), (-180, 232), (-680, 260), (-820, 30)]
    g.plan(TALUS, low, -300, 300)
    g.plan(SANDST, mid, 300, 470)
    g.plan(SANDST_L, cap, 470, 500)
    Z = 500.0
    # the canopy and two camp seats under it
    for x in (330, 650):
        for y in (-40, 180):
            g.box(STEEL, (x, y, Z + 150), (7, 7, 300))
    for k in range(5):
        g.box(MAG if k % 2 else CREAM, (298 + (k + 0.5) * 77, 70, Z + 304), (77, 270, 9))
    g.box(CYAN, (430, 80, Z + 32), (70, 60, 64))
    g.box(RED, (560, 90, Z + 32), (70, 60, 64))
    # the telescope on its tripod
    for a in (90, 210, 330):
        rod(g, STEEL, (-425, -150, Z + 150), (-425 + math.cos(math.radians(a)) * 60, -150 + math.sin(math.radians(a)) * 60, Z), 4, steps=5)
    tilt_box(g, WHITE, (-425, -90, Z + 150), (-425, -230, Z + 210), 20, 20)
    big_flag(sc, -740, 120, Z, 520.0, GOLD, RED, w=260.0, phase=0.5)
    acts = acts_for(r)
    xs = (-670, -550, -425, -290, -140, 10, 150)
    for j, x in enumerate(xs):
        fixed = hatted(r)
        if j == 4:
            fixed.update(build='kid')
        act = acts[(j * 3 + 2) % len(acts)]
        y = -170 + r.uniform(-14, 14)
        if j == 2:
            act, y = dict(arm_r='face', arm_l='hip', rate=0.3, bob=-2), -40.0      # at the telescope's eyepiece
        put(sc, r, x, y, Z, act, detail=1 if j in (0, 3, 5) else 0, **fixed)
    put(sc, r, 430, 80, Z, dict(arm_r='hold', hold_r=P.hold_cup(), arm_l='wave', swing_l=20, rate=0.9, legs='sit', seat=46.0),
        detail=0, s=S, **hatted(r, ('sun',), 1.0))
    P.quadruped(sc, 250, -170, Z, yaw=20, s=1.5, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=34, bob=5,
                rate=2.2, phase=0.4, body='#e8c26a', ears=BARK)


# ------------------------------------------------------------------------------------------ the roof party
def roof_party(sc):
    r = random.Random(449)
    g = sc.still()
    W, D, Z = 1040.0, 560.0, 420.0
    g.box(PINK, (0, 30, (Z - 300) / 2.0), (W, D, Z + 300))
    g.box(TEAL, (0, 30, Z - 16), (W + 16, D + 16, 30))
    g.box(CREAM, (0, -D / 2.0 + 24, Z + 14), (W + 16, 14, 28))
    g.box(NAVY, (-330, -D / 2.0 + 28, 120), (120, 6, 240))
    for x in (-110, 110, 330):
        g.box(GLASS, (x, -D / 2.0 + 28, 215), (130, 6, 110))
        g.box(CREAM, (x, -D / 2.0 + 26, 150), (150, 10, 12))
    for k in range(4):
        tilt_box(g, MAG if k % 2 else CREAM, (-420 + k * 60 + 30, -D / 2.0 - 70, 262), (-420 + k * 60 + 30, -D / 2.0 + 30, 300), 60, 7)
    # the stair house on the roof, two up on it
    g.box(LEMON, (330, 130, Z + 125), (300, 240, 250))
    g.box(TEAL, (330, 130, Z + 256), (320, 260, 14))
    g.box(NAVY, (240, 8, Z + 105), (90, 6, 200))
    parasol(g, -360, 120, Z, MAG, CREAM, height=330.0)
    g.box(CREAM, (-500, 250, Z + 180), (7, 7, 360))
    g.box(CREAM, (185, 245, Z + 310), (7, 7, 100))
    bunting(g, -500, 185, 250, Z + 350, n=9)
    acts = acts_for(r)
    dance = (dict(arm_l='up', arm_r='up', turn=26, rate=0.9, bob=0), dict(arm_l='vee', arm_r='hip', rock=9, rate=1.0), dict(arm_l='out', arm_r='out', turn=30, rate=0.7))
    for j, x in enumerate((-420, -270, -110, 50)):
        act = dance[j % 3] if j % 2 == 0 else acts[(j * 2) % len(acts)]
        fixed = dict(wear='dress') if j == 0 else hatted(r, ('sun', 'cap'), 0.4)
        put(sc, r, x, -150 + (30 if j % 2 else -20), Z, act, detail=1 if j < 2 else 0, **fixed)
    sitter(sc, r, 160, -D / 2.0 + 16, Z + 28, None, detail=1, **hatted(r, ('sun',), 0.5))
    put(sc, r, 270, 50, Z + 263, acts[3], detail=0)
    put(sc, r, 400, 60, Z + 263, acts[0], detail=0, **hatted(r))


# ------------------------------------------------------------------------------------------ the sign riggers
def sign_rig(sc):
    r = random.Random(461)
    g = sc.still()
    ZB, ZC = 1180.0, 760.0
    for sx in (-1, 1):
        g.box(NAVY, (sx * 360, 40, (ZB - 300) / 2.0), (30, 30, ZB + 300))
        tilt_box(g, NAVY, (sx * 360, 300, -40), (sx * 360, 50, 560), 14, 14)
    g.box(NAVY, (0, 40, ZB), (780, 34, 34))
    g.box(NAVY, (0, 40, 330), (720, 20, 20))
    # the sign: a sunburst (a disc, a ring, eight rays), unlit
    for k in range(4):
        g.box(MAG if k % 2 else CYAN, (0, 30, ZC), (720, 10, 56), rot=(k * 45.0 + 22.5, 0, 0))
    g.cyl(MAG, (0, 26, ZC), 280, 8, rot=(0, 0, 90), steps=12)
    g.cyl(GOLD, (0, 18, ZC), 236, 8, rot=(0, 0, 90), steps=12)
    g.cyl(CREAM, (0, 10, ZC), 120, 6, rot=(0, 0, 90), steps=10)
    # the scaffold deck at the foot
    ZD = 300.0
    for sx in (-1, 1):
        for y in (-240, -60):
            g.box(STEEL, (sx * 400, y, (ZD - 300) / 2.0), (10, 10, ZD + 300))
    g.box(WOOD_L, (0, -150, ZD - 6), (860, 220, 12))
    g.box(GOLD, (0, -262, ZD - 26), (864, 6, 40))
    g.box(CREAM, (0, -258, ZD + 122), (860, 6, 6))
    tilt_box(g, STEEL, (-400, -240, 0), (400, -240, ZD - 50), 7, 7)
    # the cradle rises and falls before the sign, two riggers in it
    lift = dict(phase=0.3, rate=0.11, bob=-90.0)
    c = sc.part(P.motion(**lift))
    Z0 = 640.0
    c.box(STEEL, (40, -90, Z0 - 5), (330, 100, 10))
    c.box(YEL, (40, -140, Z0 + 30), (334, 6, 60))
    c.box(YEL, (40, -40, Z0 + 110), (334, 5, 6))
    for x in (-120, 200):
        c.box(STEEL, (x, -40, Z0 + 55), (6, 6, 110))
        c.box(LEATHER, (x, -40, (Z0 + 110 + ZB) / 2.0), (5, 5, ZB - Z0 - 110))
        g.box(NAVY, (x, -2, ZB + 22), (9, 110, 9))
    rig = dict(sleeves='long', hat='hard', hat_col=YEL, band='#b8ff5a', s=S)
    work = dict(arm_r='up', arm_l='fwd', swing_r=14, axis_r='x', hold_r=P.hold_tool(), react=False)
    for x, yaw in ((-40, 180), (120, 150)):
        kw = dict(P.look(r, tops=('#ff7a3c', WHITE, DENIM)))
        kw.update(rig)
        kw.update(work)
        kw.update(lift)
        P.figure(sc, x, -90, z=Z0, yaw=yaw, detail=1, **kw)
    acts = acts_for(r)
    put(sc, r, -290, -190, ZD, dict(arm_r=((150, 20), (165, 10)), arm_l='hip', swing_r=10, axis_r='x', rate=0.6), detail=1, **rig)
    put(sc, r, -120, -200, ZD, acts[3], detail=0, **hatted(r, ('cap',)))
    put(sc, r, 250, -200, ZD, acts[0], detail=0, wear='dress')
    sitter(sc, r, -250, 22, ZB + 17, None, detail=0, **rig)


# ------------------------------------------------------------------------------------------ the trestle
def trestle(sc):
    r = random.Random(473)
    g = sc.still()
    L, Z = 1640.0, 520.0
    for sy in (-1, 1):
        g.box(BARK_D, (0, sy * 60, Z - 22), (L, 18, 32))
        g.box(STEEL, (0, sy * 60, Z + 12), (L, 7, 8))
    for k in range(10):
        g.box(BARK, (-L / 2.0 + (k + 0.5) * L / 10.0, 0, Z + 2), (24, 210, 12))
    bents = (-720.0, -240.0, 240.0, 720.0)
    for x in bents:
        g.box(BARK_D, (x, 0, Z - 48), (22, 250, 22))
        for sy in (-1, 1):
            tilt_box(g, BARK_D, (x, sy * 190, -300), (x, sy * 78, Z - 56), 20, 20)
        tilt_box(g, BARK, (x, -150, 60), (x, 110, Z - 120), 9, 9)
    for k in range(3):
        a, b = bents[k], bents[k + 1]
        tilt_box(g, BARK, (a, -128, 150) if k % 2 else (a, -92, Z - 90), (b, -92, Z - 90) if k % 2 else (b, -128, 150), 10, 10)
    # the water stop's platform, hung on the road's side of the first bay
    g.box(WOOD_L, (-480, -190, Z - 6), (430, 150, 12))
    g.box(CREAM, (-480, -262, Z + 122), (430, 6, 6))
    for x in (-690, -270):
        g.box(BARK_D, (x, -262, Z + 60), (7, 7, 124))
        tilt_box(g, BARK, (x, -260, Z - 12), (x, -110, Z - 170), 9, 9)
    g.cyl(TEAL, (-650, -200, Z), 30, 70, steps=7)
    acts = acts_for(r)
    put(sc, r, -560, -195, Z, acts[1], detail=1, **hatted(r))
    put(sc, r, -440, -200, Z, acts[0], detail=0, build='kid')
    put(sc, r, -320, -195, Z, acts[3], detail=0, **hatted(r))
    # the pump trolley travels the trestle and comes round again
    TL, SP = 1440.0, 170.0
    cx, zc = 0.0, Z + 16.0
    tv = (TL, SP / TL)
    car = sc.part(P.motion(base=(cx, 0.0), travel=tv, pivot=(cx, 0.0, zc + 30)))
    car.box(RED, (cx, 0, zc + 24), (230, 150, 16))
    for sx in (-1, 1):
        for sy in (-1, 1):
            car.cyl(LEATHER, (cx + sx * 80, sy * 60 + 6, zc + 14), 18, 12, rot=(0, 0, 90), steps=7)
    car.box(STEEL, (cx, 0, zc + 100), (16, 16, 140))
    beam = sc.part(P.motion(base=(cx, 0.0), travel=tv, pivot=(cx, 0.0, zc + 170), swing=16.0, axis=90.0, rate=0.9))
    beam.box(GOLD, (cx, 0, zc + 170), (210, 10, 10))
    beam.box(GOLD, (cx - 100, 0, zc + 170), (8, 90, 8))
    beam.box(GOLD, (cx + 100, 0, zc + 170), (8, 90, 8))
    for sx, yaw, ph in ((-1, 90, 0.0), (1, -90, 0.5)):
        kw = dict(P.look(r, tops=HOT))
        kw.update(s=S * 0.95, hat='cowboy', hat_col=r.choice(HATS))
        P.figure(sc, cx + sx * 82, 0, z=zc + 32, yaw=yaw, arm_l='push', arm_r='push', swing_l=18, swing_r=18, axis_l='x', axis_r='x',
                 rate=0.9, phase=ph, detail=1, travel=(TL, SP), **kw)


# ------------------------------------------------------------------------------------------ the derrick
def derrick(sc):
    r = random.Random(487)
    g = sc.still()
    ZT = 1150.0

    def half(z):
        return 270.0 - 205.0 * (z + 300.0) / (ZT + 300.0)

    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, BARK_D, (sx * 270, sy * 270, -300), (sx * 65, sy * 65, ZT), 18, 18)
    ZF, ZM = 420.0, 800.0
    for z in (ZF, ZM):
        h = half(z)
        for s2 in (-1, 1):
            g.box(BARK, (0, s2 * h, z - 14), (2 * h + 18, 12, 14))
            g.box(BARK, (s2 * h, 0, z - 14), (12, 2 * h + 18, 14))
    h0, h1, h2 = half(0), half(ZF - 30), half(ZM - 30)
    tilt_box(g, BARK, (-h0, -h0, 0), (h1, -h1, ZF - 30), 9, 9)
    tilt_box(g, BARK, (h0, -h0, 0), (-h1, -h1, ZF - 30), 9, 9)
    tilt_box(g, BARK, (-half(ZF), -half(ZF) + 4, ZF + 130), (h2, -h2, ZM - 30), 8, 8)
    hf = half(ZF)
    g.box(WOOD_L, (0, -30, ZF - 4), (2 * hf + 70, 2 * hf + 110, 10))
    g.box(CREAM, (0, -hf - 82, ZF + 122), (2 * hf + 70, 6, 6))
    g.box(WOOD_L, (0, 0, ZT + 4), (250, 250, 12))
    g.box(RUST, (0, 0, ZT + 40), (90, 60, 60))
    g.box(STEEL, (0, 0, (ZT + ZF) / 2.0), (9, 9, ZT - ZF))
    ladder(g, hf + 40, -hf - 60, -40, ZF + 110)
    big_flag(sc, 100, 100, ZT + 10, 330.0, GOLD, RED, w=220.0, h=120.0, phase=0.7)
    # the engine house, the walking beam nodding over it
    g.box(RUST, (620, 60, 0), (280, 240, 420))
    g.box(CREAM, (620, 60, 216), (300, 260, 14))
    g.box(BARK_D, (400, 0, 60), (26, 26, 520))
    b = sc.part(P.motion(swing=11.0, pivot=(400.0, 0.0, 330.0), axis=90.0, rate=0.3, phase=0.1))
    b.box(GOLD, (400, 0, 330), (520, 22, 26))
    b.box(GOLD, (148, 0, 310), (30, 30, 90))
    b.box(STEEL, (650, 0, 290), (10, 10, 90))
    hand = dict(hat='hard', sleeves='long', band='#b8ff5a')
    acts = acts_for(r)
    put(sc, r, -110, -hf - 30, ZF, acts[0], detail=1, hat_col=YEL, **hand)
    put(sc, r, 40, -hf - 34, ZF, dict(arm_r='up', hold_r=P.hold_tool(), swing_r=18, axis_r='x', arm_l='hip', rate=0.8), detail=1, hat_col=WHITE, **hand)
    put(sc, r, -20, -95, ZT + 10, acts[4], detail=1, hat_col=RED, **hand)
    put(sc, r, 620, -20, 223, acts[5], detail=0, hat_col=YEL, **hand)


# ------------------------------------------------------------------------------------------ the fire lookout
def lookout(sc):
    r = random.Random(499)
    g = sc.still()
    Z = 700.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, BARK_D, (sx * 250, sy * 250, -300), (sx * 200, sy * 200, Z - 10), 18, 18)
    tilt_box(g, BARK, (-240, -243, 0), (205, -206, Z - 60), 9, 9)
    tilt_box(g, BARK, (240, -243, 0), (-205, -206, Z - 60), 9, 9)
    tilt_box(g, BARK, (243, -240, 0), (206, 205, Z - 60), 9, 9)
    g.box(WOOD_L, (0, 0, Z - 6), (600, 600, 12))
    g.box(RED, (0, -302, Z - 28), (604, 6, 40))
    g.box(CREAM, (0, 50, Z + 115), (310, 310, 230))
    g.box(GLASS, (0, 50, Z + 150), (316, 316, 80))
    g.cyl(RUST, (0, 50, Z + 230), 270, 130, rot=(0, 45, 0), steps=4, top_r=24.0)
    g.box(CREAM, (0, -294, Z + 122), (600, 6, 6))
    for sx in (-1, 1):
        g.box(CREAM, (sx * 294, 0, Z + 122), (6, 600, 6))
        g.box(BARK_D, (sx * 294, -294, Z + 62), (8, 8, 124))
    # the stairs: two flights and a landing
    tilt_box(g, WOOD, (-345, -230, 0), (-345, 230, 350), 70, 12)
    tilt_box(g, WOOD, (-345, 230, 350), (-345, -230, Z - 10), 70, 12)
    g.box(WOOD_L, (-345, 250, 344), (90, 90, 12))
    big_flag(sc, 0, 50, Z + 360, 330.0, RED, CREAM, w=230.0, h=120.0, phase=0.4)
    acts = acts_for(r)
    put(sc, r, -200, -235, Z, dict(arm_l='face', arm_r='face', hold_r=hold_glasses(), rock=5, rate=0.3), detail=1, hat='peak', hat_col=DENIM, top='#7dffb0')
    put(sc, r, -60, -240, Z, acts[5], detail=1, **hatted(r))
    put(sc, r, 80, -238, Z, acts[1], detail=1, **hatted(r, ('cowboy',), 0.0))
    put(sc, r, 205, -240, Z, acts[0], detail=0, build='kid')
    put(sc, r, -345, 250, 350, acts[4], detail=0, yaw=-40, pack=r.choice(HOT))


# ------------------------------------------------------------------------------------------ the camper's roof deck
def rv_roof(sc):
    r = random.Random(509)
    g = sc.still()
    L, Wd, Z = 840.0, 250.0, 335.0
    g.box(CREAM, (0, 0, 210), (L, Wd, 250))
    g.box(TEAL, (0, -Wd / 2.0 - 2, 190), (L + 4, 4, 44))
    g.box(MAG, (0, -Wd / 2.0 - 2, 150), (L + 4, 4, 14))
    g.box(LEATHER, (0, 0, 70), (L - 60, Wd - 30, 40))
    for x in (-260, -60, 140):
        g.box(GLASS, (x, -Wd / 2.0 - 3, 270), (130, 4, 70))
    g.box(GLASS, (350, -Wd / 2.0 - 3, 262), (90, 4, 88))
    g.box(TEAL, (270, -Wd / 2.0 - 3, 200), (64, 4, 200))
    for x in (-280, 290):
        wheel(g, x, -Wd / 2.0 + 14, 48, 50, 30)
    g.box(WOOD_L, (-40, 0, Z + 4), (L - 120, Wd - 10, 8))
    g.box(CREAM, (-40, -Wd / 2.0 + 4, Z + 118), (L - 120, 6, 6))
    for x in (-400, -40, 320):
        g.box(STEEL, (x, -Wd / 2.0 + 4, Z + 62), (6, 6, 116))
    parasol(g, -300, 50, Z + 8, RED, CREAM, rad=140.0, height=330.0)
    g.box(CYAN, (-210, 60, Z + 30), (70, 50, 44))
    g.box(STEEL, (360, 80, Z + 190), (6, 6, 380))
    bunting(g, -300, 360, 80, Z + 360, n=8)
    ladder(g, 446, 0, 40, Z + 100, col=STEEL, step=70.0)
    acts = acts_for(r)
    put(sc, r, -330, -60, Z + 8, acts[6], detail=1, **hatted(r, ('sun', 'cap'), 1.0))
    put(sc, r, -190, -66, Z + 8, acts[1], detail=1, wear='shorts')
    put(sc, r, -50, -60, Z + 8, acts[0], detail=0, build='kid')
    put(sc, r, 90, -64, Z + 8, acts[3], detail=1, **hatted(r))
    sitter(sc, r, 230, -Wd / 2.0, Z + 8, None, detail=0, wear='shorts', **hatted(r, ('cap',), 1.0))


JOBS = [
    ('SM_P3D_Scaffold_A', scaffold(401, 0), GROW),
    ('SM_P3D_Scaffold_B', scaffold(402, 1), GROW),
    ('SM_P3D_WaterTower', water_tower, GROW),
    ('SM_P3D_Billboard', billboard, GROW),
    ('SM_P3D_RockSpire_A', rock_spire(431, False), GROW),
    ('SM_P3D_RockSpire_B', rock_spire(432, True), GROW),
    ('SM_P3D_MesaLedge', mesa_ledge, GROW),
    ('SM_P3D_RoofParty', roof_party, GROW),
    ('SM_P3D_SignRig', sign_rig, GROW),
    ('SM_P3D_Trestle', trestle, GROW),
    ('SM_P3D_Derrick', derrick, GROW),
    ('SM_P3D_Lookout', lookout, GROW),
    ('SM_P3D_RVRoof', rv_roof, GROW),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
