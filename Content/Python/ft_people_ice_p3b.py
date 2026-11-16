"""FULL THROTTLE (Unreal) the population of Ice Mountain, pass 3, builder p3b: SCENES (15 Nov 2026).

Story groups that could only be on this mountain, one mesh a scene, each on a raised terrace of packed snow with a
skirt of colour blocks (the road's barrier hides whoever stands on the ground): a mountain-rescue drill, an ice
sculpture contest, a wedding under an arch of skis, a curling match, alphorns and flag throwers with a ring dance,
a snowball battle between two forts, an expedition's base camp with an igloo going up, a medal ceremony, an ice
fishing derby, an ice-core drilling station and a game of pond hockey.

People are 1.5 times life size in plain hot colours (the cel pass inks a life-size figure 30 m off to a scribble).
Every piece is TALL (the table writes none nearer than 29 m), nothing glows, nothing blinks, no words or marks,
nothing big, white and furry on two legs (the level's boss is THE ABOMINABLE). The hill falls 7 in 100 along the road
and a piece stands level, so every terrace goes 1.2 m into the ground.

Meshes: /Game/People/IceMountain/SM_P3B_*   (that prefix is this builder's alone)
Run in the editor:  import ft_people_ice_p3b as K; K.run()        (or K.run(['SM_P3B_Curling_A']))
The rules that place them: C:/ftwork/p3/L5b.inl (pasted by the integrator at the end of FTPeopleL5.inl).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/IceMountain'

SNOW, SNOW_SH, ICE, ICE_D, ICE_DEEP = '#f4f9ff', '#b9c6ea', '#bfe3ff', '#7fb0e0', '#4f8fd2'
WOOD, WOOD_D, STEEL, STEEL_D = '#9a6a44', '#6a4c36', '#9aa4b8', '#5a6478'
RED, BLUE, ORANGE, PINK, TEAL, YEL, LIME, VIOLET = '#ff2d3c', '#2e6bff', '#ff8a2a', '#ff4fb8', '#19b5c9', '#ffd23f', '#b8ff5a', '#8f6bff'
DARK, WHITE, SILVER, BRONZE = '#22202a', '#fff6ea', '#cfcfd8', '#c9703a'
SUITS = (RED, PINK, ORANGE, YEL, LIME, '#2ee6ff', BLUE, VIOLET, '#ff5fa2', '#7dffb0')
LEGS = ('#3a5fa8', WHITE, VIOLET, TEAL)
BIG = 1.5            # the size of people in this kit
FOOT = 120.0         # how far every terrace goes into the ground
L, D, H = 1100.0, 440.0, 110.0     # a terrace: length along the road, depth, height of its top


def who(r, **fixed):
    """Somebody in a ski suit or a padded coat, a bobble hat, a helmet or a cap: one hot colour, big."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear=r.choice(('trousers', 'trousers', 'coat')), top=c, bottom=r.choice((c,) + LEGS),
               hat=r.choice(('beanie', 'beanie', 'helmet', 'cap')), hat_col=r.choice(SUITS), shoes=r.choice((c, WHITE, DARK)),
               hair=r.choice(('short', 'long', 'bun')), s=BIG * r.uniform(0.93, 1.07))
    d.update(fixed)
    return d


def fig(sc, r, x, y, z=H, yaw=0.0, detail=0, **kw):
    d = who(r)
    d.update(kw)
    d.setdefault('phase', r.random())
    return P.figure(sc, x, y, z, yaw=yaw, detail=detail, **d)


def kid(**kw):
    d = dict(build='kid', s=BIG * 0.7)
    d.update(kw)
    return d


def stage(sc, skirt, top=SNOW, length=L, depth=D, h=H):
    """The terrace every scene stands on: packed snow, a skirt of colour blocks along the front and round the ends."""
    g = sc.still()
    g.box(SNOW_SH, (0, 0, (h - FOOT) / 2.0 - 5), (length, depth, h + FOOT - 10))
    g.box(top, (0, 0, h - 5), (length + 24, depth + 24, 10))
    n = 8
    for k in range(n):
        g.box(skirt[k % len(skirt)], (-length / 2.0 + (k + 0.5) * length / n, -depth / 2.0 - 14, (h - 14 - FOOT) / 2.0), (length / n, 6, h - 14 + FOOT))
    for ex in (-1, 1):
        g.box(skirt[(1 if ex > 0 else 0) % len(skirt)], (ex * (length / 2.0 + 14), 0, (h - 14 - FOOT) / 2.0), (6, depth, h - 14 + FOOT))
    return g


def dog(sc, x, y, z=H, yaw=0.0, body='#c9703a', s=1.7, **kw):
    d = dict(length=75, height=52, girth=26, neck=18, neck_up=35, head=22, tail=24, wag=30, rate=2.0, belly=WHITE, snout=WHITE, ears='#6a4428')
    d.update(kw)
    P.quadruped(sc, x, y, z, yaw=yaw, s=s, body=body, **d)


def pennant(sc, x, y, z0, top, col, sq=None, w=130.0, h=80.0, phase=0.0, pole=STEEL_D):
    """A pole with a flag that swings in the wind (a field and a square of a second colour: nobody's flag)."""
    g = sc.still()
    g.box(pole, (x, y, (z0 + top) / 2.0), (7, 7, top - z0))
    m = sc.part(P.motion(phase=phase, rate=0.6, swing=20.0, pivot=(x, y, top - h / 2.0), axis=P.UP))
    m.box(col, (x + w / 2.0, y, top - h / 2.0), (w, 4, h))
    if sq:
        m.box(sq, (x + w / 2.0, y, top - h / 2.0), (h * 0.45, 5.6, h * 0.45))


def bunting(sc, a, b, n=7, cols=(RED, YEL, TEAL, PINK), size=(44.0, 54.0), phase=0.0):
    g = sc.still()
    tilt_box(g, DARK, a, b, 4, 4)
    for i in range(n):
        f = (i + 0.5) / n
        x, y, z = a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f
        m = sc.part(P.motion(phase=phase + i * 0.17, rate=0.9, swing=16.0, pivot=(x, y, z), axis=0.0))
        m.box(cols[i % len(cols)], (x, y, z - size[1] / 2.0), (size[0], 3, size[1]))


# ------------------------------------------------------------------------------------------ what a hand holds
def h_flag(col=RED, sq=WHITE, w=150.0, h=90.0, length=250.0, pole=WHITE):
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + length / 2.0 - 20), (5, 5, length))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + length - 20 - h / 2.0), (w, 3, h))
        if sq:
            m.box(sq, (hd[0] + side * w / 2.0, hd[1], hd[2] + length - 20 - h / 2.0), (h * 0.45, 4.4, h * 0.45))
    return f


def h_pole(col=DARK, length=250.0, tip=RED):
    """A pole held upright, its foot near the ground: an avalanche probe, a wand, a broom held at rest."""
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1] - 6, length / 2.0 + 6), (4.5, 4.5, length))
        m.box(tip, (hd[0], hd[1] - 6, length + 6), (10, 10, 10))
    return f


def h_broom(col=YEL, head=RED, reach=70.0):
    """A broom or a stick from the hand down to the ice ahead: a curling broom, a hockey stick."""
    def f(m, hd, side):
        tilt_box(m, col, (hd[0], hd[1] + 8, hd[2] + 14), (hd[0], hd[1] - reach, 8), 4.5, 4.5)
        m.box(head, (hd[0], hd[1] - reach - 6, 6), (34, 14, 9))
    return f


def h_skis(col=RED, tip=WHITE, length=190.0):
    def f(m, hd, side):
        for k in (-1, 1):
            m.box(col, (hd[0] + side * 10 + k * 6, hd[1], hd[2] - 40 + length / 2.0), (9, 4, length))
        m.box(tip, (hd[0] + side * 10, hd[1], hd[2] - 40 + length - 12), (22, 5, 24))
    return f


def h_thing(col=YEL, size=(22.0, 22.0, 30.0), up=14.0):
    """A block of colour in the hand: a trophy, a bouquet, a bell, a radio, a fish, a core of ice."""
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1] - 4, hd[2] + up), size)
    return f


def h_rod(col=WOOD_D, length=150.0, fish=None):
    """A short rod held out ahead, a line down from its tip, and maybe a fish on it."""
    def f(m, hd, side):
        tip = (hd[0], hd[1] - length * 0.8, hd[2] + length * 0.5)
        tilt_box(m, col, (hd[0], hd[1] + 10, hd[2] - 6), tip, 4.5, 4.5)
        m.box(DARK, (tip[0], tip[1], tip[2] - 50), (4, 4, 100))
        if fish:
            m.box(fish, (tip[0], tip[1], tip[2] - 120), (16, 30, 60))
            m.box(WHITE, (tip[0], tip[1] + 2, tip[2] - 156), (10, 34, 16))
    return f


def h_horn(col='#e0b070', band=RED, reach=250.0):
    """An alphorn: from the mouth down to the ground ahead, its bell turned up."""
    def f(m, hd, side):
        if side < 0:
            return
        x = hd[0] - 28.0
        tilt_box(m, col, (x, hd[1] - 8, hd[2] + 22), (x, hd[1] - reach, 16), 9, 9)
        tilt_box(m, band, (x, hd[1] - reach * 0.5, hd[2] * 0.55 + 16), (x, hd[1] - reach * 0.56, hd[2] * 0.5 + 14), 12, 12)
        tilt_box(m, col, (x, hd[1] - reach, 14), (x, hd[1] - reach - 46, 50), 26, 26)
    return f


def h_stick(col=WOOD, reach=60.0):
    return h_broom(col, DARK, reach)


# ------------------------------------------------------------------------------------------ 1 the rescue drill
def rescue(seed=101, skirt=(ORANGE, WHITE)):
    """Mountain rescue at practice: a sled with a wrapped casualty hauled to and fro by two rescuers, a line of
    probers, a dog and its handler, one on the radio, one waving the helicopter in, a windsock."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt)
        for (x, y, w, hh) in ((330, 150, 260, 60), (470, 110, 200, 90), (400, 40, 150, 40)):      # the avalanche's debris
            g.box(ICE, (x, y, H + hh / 2.0), (w, 130, hh))
        g.box(ORANGE, (110, -160, H + 22), (90, 50, 44))
        g.box(YEL, (110, -160, H + 48), (94, 54, 8))
        team = dict(top=RED, bottom=RED, band=YEL, hat='helmet', hat_col=WHITE, wear='trousers', shoes=DARK)
        sx, sy = -230.0, -80.0
        sw = dict(swing=1.3, pivot=(sx, sy, -6000.0), axis=90.0)
        m = sc.part(P.motion(phase=0.0, rate=0.22, **sw))
        m.box(ORANGE, (sx, sy, H + 24), (240, 66, 28))
        m.box(DARK, (sx, sy - 30, H + 6), (260, 6, 12))
        m.box(DARK, (sx, sy + 30, H + 6), (260, 6, 12))
        m.box(YEL, (sx - 14, sy, H + 50), (176, 50, 26))
        m.box(SILVER, (sx - 14, sy, H + 64), (60, 54, 6))
        m.sphere('#d9a070', (sx + 96, sy, H + 56), 20, steps=(3, 6))
        m.box(BLUE, (sx + 100, sy, H + 72), (34, 40, 14))
        tilt_box(m, STEEL_D, (sx + 120, sy, H + 30), (sx + 205, sy, H + 140), 5, 5)
        tilt_box(m, STEEL_D, (sx - 120, sy, H + 30), (sx - 190, sy, H + 140), 5, 5)
        fig(sc, r, sx + 235, sy, yaw=90, detail=1, arm_l='back', arm_r='back', legs='stride', phase=0.0, rate=0.22, sway=sw, **team)
        fig(sc, r, sx - 225, sy, yaw=90, arm_l='push', arm_r='push', legs='stride', phase=0.0, rate=0.22, sway=sw, **team)
        fig(sc, r, 500, -120, yaw=-20, detail=1, arm_r='up', arm_l='hip', swing_r=26, rate=0.8, bob=6, hold_r=h_flag(ORANGE, WHITE), **team)
        fig(sc, r, 250, -110, yaw=25, arm_r='face', arm_l='point', hold_r=h_thing(DARK, (12, 10, 30)), rate=0.5, turn=14, **team)
        fig(sc, r, 150, 60, yaw=-60, arm_r='fwd', arm_l='hip', rate=1.0, bob=5, **team)
        dog(sc, 250, 20, yaw=-90, graze=14, rate=1.6)
        for k, (x, y) in enumerate(((300, 60), (410, -30))):
            fig(sc, r, x, y, yaw=180 if k else 150, arm_r='fwd', arm_l='fwd', swing_r=9, swing_l=9, axis_r='x', axis_l='x', rate=1.1 + 0.2 * k,
                hold_r=h_pole(DARK, 270.0, ORANGE), **team)
        fig(sc, r, 20, 130, yaw=40, arm_r='carry', arm_l='carry', swing_r=22, swing_l=22, axis_r='x', axis_l='x', rate=0.9,
            hold_r=P.hold_tool(STEEL, ORANGE, 60.0), **team)
        g = sc.still()
        g.box(STEEL_D, (-500, 160, H + 230), (8, 8, 460))
        m = sc.part(P.motion(phase=0.3, rate=0.5, swing=22.0, pivot=(-500, 160, H + 440), axis=P.UP))
        for k in range(4):
            m.box(ORANGE if k % 2 == 0 else WHITE, (-500 + 20 + k * 36, 160, H + 440), (36, 30 - k * 4, 30 - k * 4))
    return build


# ------------------------------------------------------------------------------------------ 2 the ice sculptors
def sculpt(seed=102, skirt=(TEAL, WHITE)):
    """An ice sculpture contest: a swan, a star and a block still being cut; sculptors with chisels, a judge, onlookers."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt)
        # the swan
        x = -380.0
        g.box(ICE_D, (x, 60, H + 30), (230, 170, 60))
        g.box(ICE, (x, 60, H + 110), (190, 120, 100))
        tilt_box(g, ICE, (x + 60, 60, H + 150), (x + 110, 60, H + 300), 44, 44)
        tilt_box(g, ICE, (x + 110, 60, H + 300), (x + 170, 60, H + 262), 40, 40)
        g.box(ORANGE, (x + 190, 60, H + 248), (30, 20, 16))
        tilt_box(g, ICE_DEEP, (x - 20, 60, H + 150), (x - 110, 60, H + 240), 110, 20)
        g.box(PINK, (x, -30, H + 30), (60, 8, 40))
        # the star: three bars crossed on a plinth
        x = 0.0
        g.box(ICE_D, (x, 110, H + 50), (150, 150, 100))
        for a in (0.0, 60.0, 120.0):
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            tilt_box(g, ICE, (x - ca * 150, 110, H + 250 - sa * 150), (x + ca * 150, 110, H + 250 + sa * 150), 40, 46)
        g.box(ICE_DEEP, (x, 106, H + 250), (70, 50, 70))
        g.box(YEL, (x, 30, H + 50), (60, 8, 40))
        # the block still being cut, and its ladder
        x = 380.0
        g.box(ICE_D, (x, 80, H + 150), (190, 170, 300))
        g.box(ICE, (x - 30, 60, H + 320), (110, 120, 60))
        for k in range(3):
            g.box(WOOD, (x - 170, 60, H + 30 + k * 60), (70, 110, 60 - k * 0))
        for (cx, cy) in ((x - 60, -60), (x + 60, -90), (x + 130, -40)):
            g.box(ICE, (cx, cy, H + 14), (36, 30, 28))
        bunting(sc, (-540, 210, H + 380), (540, 210, H + 380), n=9, cols=(TEAL, YEL, PINK, RED))
        g = sc.still()
        for ex in (-1, 1):
            g.box(WOOD_D, (ex * 540, 210, H + 190), (10, 10, 380))
        fig(sc, r, 210, 60, z=H + 180, yaw=-90, detail=1, arm_r='fwd', arm_l='hold', swing_r=28, axis_r='x', rate=1.8, hold_r=P.hold_tool(STEEL, RED, 40.0))
        fig(sc, r, 500, -60, yaw=120, arm_r='fwd', arm_l='fwd', swing_r=10, swing_l=10, axis_r='x', axis_l='x', rate=1.2, hold_r=h_thing(ORANGE, (16, 90, 22), 0.0))
        fig(sc, r, -250, -20, yaw=100, arm_r='wave', arm_l='hip', swing_r=24, rate=1.4, hold_r=h_thing(YEL, (20, 10, 26)))
        fig(sc, r, -120, -110, yaw=170, detail=1, arm_l='hold', arm_r='point', hold_l=h_thing(WHITE, (30, 6, 40)), rate=0.6, turn=16, top=VIOLET, hat='cap', hat_col=YEL)
        fig(sc, r, 120, -60, yaw=200, arm_r='hold', arm_l='hold', hold_r=P.hold_camera(), rate=0.5, turn=12)
        fig(sc, r, -470, -120, yaw=150, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=14, rate=1.3)
        fig(sc, r, 30, -150, yaw=185, bob=22, rate=1.7, arm_l='vee', arm_r='vee', **kid())
        fig(sc, r, -40, -40, yaw=160, arm_r='fwd', arm_l='fwd', swing_r=20, swing_l=-20, axis_r='z', axis_l='z', rate=2.0, bob=4)
    return build


# ------------------------------------------------------------------------------------------ 3 the wedding
def wedding(seed=103, skirt=(PINK, WHITE)):
    """A wedding on the snow: the couple under an arch of skis held by a guard of honour, guests throwing confetti,
    a photographer, a fiddler, a child hopping, a cake."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt)
        g.box(RED, (0, -40, H + 2), (640, 150, 4))                         # the carpet
        cols = (RED, YEL, BLUE, LIME, PINK, TEAL)
        for k in range(6):                                                 # the guard of honour: skis raised over the couple
            x = -300 + k * 120
            c = cols[k]
            d = who(r, top=c, bottom=c, hat='beanie', hat_col=WHITE)
            d['phase'] = r.random()
            P.figure(sc, x, 110, H, yaw=0, detail=0, arm_r=((150, 60), (150, 70)), arm_l='hip', swing_r=6, axis_r='x', rate=0.7, **d)
            g = sc.still()
            for j in (-1, 1):
                tilt_box(g, c, (x + 40 + j * 7, 80, H + 215), (x + 40 + j * 7, -120, H + 400), 4, 11)
        fig(sc, r, -42, -40, yaw=10, detail=1, top=WHITE, bottom=WHITE, wear='dress', hair='long', hat=None, band=PINK, shoes=WHITE,
            arm_l='up', arm_r='down', swing_l=18, rate=0.9, bob=6, hold_l=h_thing(RED, (30, 30, 30)), s=BIG)
        fig(sc, r, 46, -40, yaw=-10, detail=1, top=BLUE, bottom=BLUE, wear='trousers', hat=None, hair='short', band=WHITE, shoes=DARK,
            arm_r='wave', arm_l='down', swing_r=24, rate=0.9, bob=6, s=BIG * 1.05)
        g = sc.still()
        g.box(WHITE, (-42, 6, H + 150), (60, 8, 170))                      # her veil
        acts = (dict(arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=16, rate=1.3), dict(arm_r='wave', swing_r=30, bob=10, rate=1.1),
                dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=20, rate=1.5))
        for k, (x, y, yw) in enumerate(((-460, -90, -60), (-380, 30, -70), (440, -100, 60), (380, 20, 70))):
            fig(sc, r, x, y, yaw=yw, **acts[k % 3])
        fig(sc, r, -250, -150, yaw=120, bob=24, rate=1.8, arm_l='up', arm_r='up', top=YEL, **kid())
        fig(sc, r, 250, -150, yaw=215, detail=1, arm_r='hold', arm_l='hold', hold_r=P.hold_camera(), rate=0.4, turn=10, top=ORANGE)
        fig(sc, r, 500, 120, yaw=20, arm_l='face', arm_r='fwd', swing_r=26, axis_r='z', rate=1.6, hold_l=h_thing(WOOD, (14, 40, 14)), top=LIME)
        g = sc.still()
        g.box(WOOD, (-500, 130, H + 45), (110, 110, 90))                   # the cake on its table
        g.cyl(PINK, (-500, 130, H + 90), 46, 34, steps=8)
        g.cyl(YEL, (-500, 130, H + 124), 32, 30, steps=8)
        g.cyl(PINK, (-500, 130, H + 154), 18, 26, steps=8)
        for k in range(12):                                                # confetti, drifting
            m = sc.part(P.motion(phase=k * 0.083, rate=0.35 + 0.03 * (k % 4), bob=-45.0, pivot=(0, 0, H + 300)))
            m.box(cols[k % 6], (-260 + k * 47, -70 + (k * 53) % 90, H + 330 + (k * 37) % 110), (14, 4, 14), rot=(0, k * 31.0, 20))
    return build


# ------------------------------------------------------------------------------------------ 4 the curling match
def curling(seed=104, skirt=(BLUE, YEL)):
    """A curling match on a sheet of blue ice: a stone sliding to the house with two sweepers ahead of it, the thrower
    in the hack, the skip calling, the other rink waiting, two fans behind the boards with a bell and a flag."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt, top=ICE)
        hx, y0 = 400.0, -60.0
        g.cyl(RED, (hx, y0, H), 130, 3, steps=12)
        g.cyl(WHITE, (hx, y0, H + 3), 88, 2, steps=12)
        g.cyl(BLUE, (hx, y0, H + 5), 44, 2, steps=10)
        g.box(DARK, (0, y0, H + 1), (1000, 5, 2))
        g.box(DARK, (-470, y0, H + 8), (30, 60, 16))                       # the hack
        g.box(WOOD, (0, 150, H + 30), (1080, 10, 60))                      # the boards behind
        g.box(RED, (0, 144, H + 50), (1080, 4, 14))

        def stone(m, x, y, col):
            m.cyl(STEEL_D, (x, y, H + 1), 24, 16, steps=8)
            m.cyl(col, (x, y, H + 17), 17, 7, steps=8)
            m.box(col, (x, y, H + 30), (26, 7, 7))
        for (x, y, c) in ((hx - 30, y0 + 40, RED), (hx + 50, y0 - 20, YEL), (hx + 90, y0 + 70, YEL), (hx - 110, y0 - 70, RED), (-430, 90, YEL), (-380, 90, RED)):
            stone(g, x, y, c)
        Lt, sp = 700.0, 110.0
        m = sc.part(P.motion(travel=(Lt, sp / Lt), base=(0.0, y0), pivot=(0.0, y0, H + 10)))
        stone(m, 0.0, y0, RED)
        red = dict(top=RED, bottom=WHITE, hat='beanie', hat_col=RED, wear='trousers')
        yel = dict(top=YEL, bottom=BLUE, hat='beanie', hat_col=YEL, wear='trousers')
        for k, oy in enumerate((-60, 56)):
            fig(sc, r, 110 + k * 40, y0 + oy, yaw=180 if oy < 0 else 0, arm_r='push', arm_l='push', swing_r=22, swing_l=22, axis_r='z', axis_l='z', rate=2.4,
                hold_r=h_broom(DARK, PINK, 74.0), travel=(Lt, sp), phase=0.5 * k, **red)
        fig(sc, r, -420, y0, yaw=90, detail=1, legs='stride', arm_r='fwd', arm_l='out', rate=0.5, rock=5, **red)
        fig(sc, r, hx + 70, y0 + 10, yaw=-90, detail=1, arm_r='up', arm_l='point', swing_r=14, rate=1.2, bob=4, hold_r=h_pole(DARK, 200.0, PINK), **red)
        fig(sc, r, -250, 80, yaw=0, arm_l='hip', arm_r='down', hold_r=h_pole(DARK, 190.0, TEAL), rate=0.4, turn=10, **yel)
        fig(sc, r, -150, 100, yaw=0, arm_l='down', arm_r='face', hold_l=h_pole(DARK, 190.0, TEAL), rate=0.5, rock=4, **yel)
        fig(sc, r, 60, 195, yaw=0, arm_r='fwd', arm_l='hip', swing_r=16, axis_r='x', rate=1.7, bob=8, hold_r=h_thing(YEL, (24, 16, 28), -16.0))
        fig(sc, r, 200, 195, yaw=0, arm_r='up', arm_l='up', swing_r=24, swing_l=-14, rate=0.9, bob=12, hold_r=h_flag(PINK, YEL))
        fig(sc, r, 320, 195, yaw=0, bob=22, rate=1.6, arm_l='vee', arm_r='vee', **kid())
        g = sc.still()
        g.box(WOOD_D, (-420, 200, H + 150), (10, 10, 300))                 # the scoreboard: discs of the rinks' colours
        g.box(DARK, (-420, 196, H + 250), (220, 8, 130))
        for k in range(4):
            g.box(RED if k % 2 else YEL, (-495 + k * 50, 190, H + 280), (30, 6, 30))
            g.box(YEL if k < 2 else RED, (-495 + k * 50, 190, H + 225), (30, 6, 30))
    return build


# ------------------------------------------------------------------------------------------ 5 alphorns and a ring dance
def alphorns(seed=105, skirt=(RED, YEL)):
    """A mountain festival: three alphorn players, two flag throwers, four dancers going round in a ring, an
    accordion player on a stool, and a cow in her bell and flowers."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt)
        folk = dict(bottom=DARK, hat='cap', hat_col=DARK, wear='trousers', shoes=DARK)
        for k, x in enumerate((-110, 0, 110)):
            fig(sc, r, x, 120, yaw=0, detail=1 if k == 1 else 0, arm_r='hold', arm_l='hold', hold_r=h_horn(), rate=0.4, rock=3, top=(RED, WHITE, RED)[k], band=YEL, **folk)
        fig(sc, r, -330, 130, yaw=10, arm_r='up', arm_l='hip', swing_r=38, rate=0.6, bob=5, hold_r=h_flag(PINK, YEL, 190.0, 120.0, 290.0), top=WHITE, band=RED, **folk)
        fig(sc, r, 330, 130, yaw=-10, arm_l='up', arm_r='hip', swing_l=38, rate=0.5, bob=5, hold_l=h_flag(TEAL, WHITE, 190.0, 120.0, 290.0), top=WHITE, band=RED, **folk)
        cx, cy = -330.0, -70.0
        for k in range(4):                                                 # the ring dance
            a = math.radians(k * 90.0)
            c = (PINK, LIME, YEL, TEAL)[k]
            fig(sc, r, cx + math.cos(a) * 100, cy + math.sin(a) * 100, yaw=k * 90.0 + 180.0, arm_l='out', arm_r='out', bob=10, rate=1.5,
                orbit=(cx, cy, 0.14), top=c, bottom=RED if k % 2 else BLUE, wear='skirt' if k % 2 else 'trousers', hat=None, hair='long' if k % 2 else 'short')
        g = sc.still()
        g.box(WOOD, (130, -110, H + 34), (60, 60, 68))
        fig(sc, r, 130, -110, yaw=0, legs='sit', seat=46, z=H + 0, arm_r='hold', arm_l='hold', swing_r=16, swing_l=-16, axis_r='z', axis_l='z', rate=1.5,
            hold_r=h_thing(RED, (60, 26, 44), 0.0), top=LIME, **folk)
        fig(sc, r, 260, -130, yaw=15, bob=20, rate=1.5, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, **kid())
        P.quadruped(sc, 440, -60, H, yaw=60, s=1.25, length=190, height=125, girth=62, neck=34, neck_up=10, head=40, body=WHITE, belly='#f1e4c8',
                    legs_col=BRONZE, ears=BRONZE, snout=PINK, horns='#e0b070', tail=50, tail_col=BRONZE, graze=10, wag=18, rate=0.5, phase=0.2)
        g = sc.still()
        g.box(BRONZE, (440, -40, H + 120), (84, 110, 70))                  # her patches, her bell, her flowers
        g.box(YEL, (372, -100, H + 86), (30, 22, 34))
        g.box(RED, (410, -76, H + 168), (60, 30, 14))
        bunting(sc, (-540, 205, H + 400), (540, 205, H + 400), n=9, cols=(RED, WHITE, YEL, RED))
        g = sc.still()
        for ex in (-1, 1):
            g.box(WOOD_D, (ex * 540, 205, H + 200), (10, 10, 400))
    return build


# ------------------------------------------------------------------------------------------ 6 the snowball battle
def snowfort(seed=106, skirt=(BLUE, RED)):
    """A snowball battle: two forts of ice blocks under their pennants, three defenders each with their arms going,
    a stock of snowballs on the ground (none in the air: see below), a referee between them and a jumping dog."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt)
        for ex, team in ((-1, RED), (1, BLUE)):
            fx = ex * 400.0
            for k in range(5):                                             # the wall, crenellated, facing the middle
                y = -160 + k * 80
                g.box(ICE if k % 2 else ICE_D, (fx - ex * 110, y, H + 60), (60, 80, 120))
                if k % 2 == 0:
                    g.box(ICE, (fx - ex * 110, y, H + 150), (60, 60, 60))
            for k in range(3):                                             # the wall towards the road, low
                g.box(ICE_D if k % 2 else ICE, (fx - ex * 70 + ex * k * 80, -190, H + 35), (80, 50, 70))
            pennant(sc, fx + ex * 110, 150, H, H + 430, team, WHITE, phase=0.2 if ex > 0 else 0.6)
            g = sc.still()
            g.box(ICE, (fx + ex * 110, 150, H + 30), (80, 80, 60))
            tm = dict(top=team, hat='beanie', hat_col=team)
            fig(sc, r, fx - ex * 30, 60, yaw=ex * 90.0, detail=1, arm_r='up', arm_l='fwd', swing_r=50, axis_r='x', rate=1.1, bob=4, **tm)
            fig(sc, r, fx + ex * 20, -70, yaw=ex * 80.0, arm_r='wave', arm_l='hold', swing_r=40, axis_r='x', rate=1.4, bob=10, **tm)
            fig(sc, r, fx + ex * 60, -150, yaw=ex * 60.0, bob=24, rate=1.9, arm_l='up', arm_r='up', **kid(**tm))
            # (fix round, 16 Nov: no snowballs in the air. The Abominable's attack is thrown snowballs and this piece
            # stands beside his road: the throwing arms and the stock on the ground tell the story.)
        g = sc.still()
        for k in range(3):                                                 # a stock of snowballs
            g.sphere(ICE, (-20 + k * 30, 170, H + 14), 15, steps=(3, 5))
        fig(sc, r, 0, 110, yaw=0, detail=1, arm_l='out', arm_r='face', rate=0.8, turn=22, top=YEL, bottom=DARK, hat='cap', hat_col=DARK)
        fig(sc, r, -60, -160, yaw=160, arm_r='hold', arm_l='hold', hold_r=P.hold_phone(), rate=0.5, turn=18)
        dog(sc, 60, -60, yaw=70, bob=26, rate=1.3, body='#e8c26a')
    return build


# ------------------------------------------------------------------------------------------ 7 the base camp
def basecamp(seed=107, skirt=(ORANGE, TEAL)):
    """An expedition's camp: an igloo going up block by block (two carriers walk the blocks over from the quarry),
    dome tents, a cook at the pot, one resting on a crate, the radio mast, a husky."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt)
        g.sphere(ICE, (400, 60, H), 170, steps=(5, 8))                     # the igloo and its porch
        g.box(ICE_D, (400, -110, H + 45), (110, 120, 90))
        g.box(DARK, (400, -172, H + 36), (64, 6, 72))
        for k in range(4):                                                 # the quarry: cut blocks
            g.box(ICE_D if k % 2 else ICE, (-470 + (k % 2) * 80, -130 + (k // 2) * 70, H + 30), (70, 56, 60))
        g.sphere(ORANGE, (-330, 130, H), 120, scale=(1.25, 1.0, 0.9), steps=(4, 8))       # tents
        g.sphere(YEL, (-70, 150, H), 100, scale=(1.25, 1.0, 0.9), steps=(4, 8))
        g.box(DARK, (-330, 22, H + 40), (60, 6, 80))
        exp = dict(wear='coat', hat='beanie')
        for k in range(2):                                                 # the carriers: along the front, quarry to igloo
            fig(sc, r, -120 + k * 260, -150, yaw=90, detail=1 if k == 0 else 0, arm_r='carry', arm_l='carry', hold_r=P.hold_box(ICE, 50.0),
                stride=20, rate=1.2, bob=3, travel=(620.0, 55.0), **exp)
        fig(sc, r, -400, -40, yaw=200, arm_r='fwd', arm_l='fwd', swing_r=24, swing_l=24, axis_r='x', axis_l='x', rate=1.3, hold_r=h_thing(STEEL, (8, 90, 16), -10.0), **exp)
        fig(sc, r, 300, 50, yaw=-60, z=H + 0, arm_l='up', arm_r='up', swing_l=10, swing_r=-10, rate=0.6, bob=4, hold_r=h_thing(ICE_D, (50, 40, 34), 20.0), **exp)
        g = sc.still()
        g.box(STEEL_D, (80, 20, H + 30), (50, 50, 60))                     # the stove, the pot, its steam
        g.cyl(RED, (80, 20, H + 60), 30, 34, steps=8)
        for k in range(3):
            m = sc.part(P.motion(phase=k * 0.33, rate=0.4, bob=-30.0, pivot=(80, 20, H + 140)))
            m.box(WHITE, (72 + k * 10, 20, H + 130 + k * 36), (26 + k * 8, 20, 20))
        fig(sc, r, 150, 60, yaw=-70, detail=1, arm_r='fwd', arm_l='hip', swing_r=18, axis_r='z', rate=1.0, hold_r=h_thing(WOOD, (8, 8, 60), -30.0), top=RED, **exp)
        g = sc.still()
        g.box(WOOD, (-190, -20, H + 34), (70, 60, 68))
        fig(sc, r, -190, -20, yaw=-20, legs='sit', arm_r='face', arm_l='down', hold_r=P.hold_cup(YEL), rate=0.5, rock=4, **exp)
        fig(sc, r, 180, 180, yaw=0, arm_r='face', arm_l='hip', hold_r=h_thing(DARK, (12, 10, 30)), rate=0.5, turn=14, **exp)
        g = sc.still()
        g.box(STEEL_D, (240, 190, H + 260), (7, 7, 520))                   # the radio mast and its stays
        tilt_box(g, DARK, (240, 190, H + 500), (120, 200, H), 4, 4)
        tilt_box(g, DARK, (240, 190, H + 500), (330, 205, H), 4, 4)
        pennant(sc, 240, 190, H + 500, H + 600, RED, YEL, w=110.0, h=64.0, phase=0.4)
        g = sc.still()
        for k, c in enumerate((RED, YEL, BLUE, PINK)):                     # skis stood in the snow
            g.box(c, (-560 + 14 + k * 18, 60 + k * 14, H + 100), (9, 4, 200))
        dog(sc, 10, -70, yaw=120, body=SILVER, ears=DARK, s=1.8)
        bunting(sc, (-330, 130, H + 250), (240, 190, H + 420), n=8, cols=(BLUE, WHITE, RED, LIME, YEL))
        g = sc.still()
        g.box(WOOD_D, (-330, 130, H + 180), (8, 8, 150))
    return build


# ------------------------------------------------------------------------------------------ 8 the medal ceremony
def podium(seed=108, skirt=(YEL, RED)):
    """The medal ceremony: three winners on the steps, a backdrop of colour blocks, the presenter, two flag bearers,
    a drummer, photographers at the corners."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt)
        for (x, hh, c) in ((0, 130, YEL), (-140, 90, SILVER), (140, 60, BRONZE)):
            g.box(c, (x, 20, H + hh / 2.0), (140, 130, hh))
            g.box(WHITE, (x, -47, H + hh / 2.0), (50, 4, min(hh - 20, 50)))
        by = 190.0
        for ex in (-1, 1):
            g.box(STEEL_D, (ex * 330, by, H + 240), (12, 12, 480))
        for i in range(8):                                                 # the backdrop: blocks of colour, a chequer along its top
            g.box((TEAL, PINK, VIOLET, BLUE)[i % 4], (-330 + (i + 0.5) * 82.5, by, H + 270), (82.5, 8, 260))
            g.box(DARK if i % 2 else WHITE, (-330 + (i + 0.5) * 82.5, by, H + 430), (82.5, 9, 60))
        fig(sc, r, 0, 20, z=H + 130, detail=1, arm_l='vee', arm_r='up', swing_l=14, swing_r=-12, bob=14, rate=1.2, hold_r=h_thing(YEL, (26, 26, 44), 16.0), hat=None)
        fig(sc, r, -140, 20, z=H + 90, arm_r='up', arm_l='hip', swing_r=18, rate=0.9, bob=8, hold_r=h_skis(PINK))
        fig(sc, r, 140, 20, z=H + 60, arm_l='wave', arm_r='down', swing_l=30, rate=1.1, bob=8, hold_l=h_thing(RED, (30, 30, 30)))
        fig(sc, r, 250, -40, yaw=-60, detail=1, arm_r='carry', arm_l='carry', hold_r=h_thing(VIOLET, (60, 40, 10), 6.0), rate=0.5, rock=3, top=WHITE, bottom=BLUE, hat='peak', hat_col=BLUE)
        for ex, c in ((-1, RED), (1, BLUE)):
            fig(sc, r, ex * 470, 130, yaw=-ex * 15.0, arm_r='up', arm_l='hip', swing_r=24, rate=0.6 + 0.1 * ex, bob=5, hold_r=h_flag(c, YEL, 200.0, 120.0, 300.0))
        g = sc.still()
        g.cyl(RED, (-290, -120, H + 90), 42, 40, steps=8)                  # the drum
        g.cyl(WHITE, (-290, -120, H + 130), 43, 3, steps=8)
        fig(sc, r, -290, -60, yaw=0, arm_r='fwd', arm_l='fwd', swing_r=18, swing_l=-18, axis_r='x', axis_l='x', rate=2.2, top=LIME)
        for ex in (-1, 1):
            fig(sc, r, ex * 420, -140, yaw=180 - ex * 55.0, arm_r='hold', arm_l='hold', hold_r=P.hold_camera(), rate=0.6, turn=12, bob=3)
        fig(sc, r, -60, -150, yaw=180, bob=22, rate=1.7, arm_l='up', arm_r='up', **kid())
        bunting(sc, (-330, by - 10, H + 470), (-540, -200, H + 250), n=6, phase=0.2)
        bunting(sc, (330, by - 10, H + 470), (540, -200, H + 250), n=6, cols=(YEL, PINK, TEAL, RED), phase=0.6)
    return build


# ------------------------------------------------------------------------------------------ 9 the ice fishing derby
def icefishing(seed=109, skirt=(TEAL, YEL)):
    """An ice fishing derby: anglers on stools jigging at their holes, one hauling a great fish out, a fish leaping
    from a hole, the red hut, the weigh-in rack, a child, a dog at the bucket, one drilling a new hole."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt, top=ICE)
        holes = ((-330, -90), (-120, -130), (90, -80), (300, -140), (-220, 60))
        for (x, y) in holes:
            g.cyl(ICE_DEEP, (x, y, H), 44, 2, steps=8)
            g.cyl('#2b4a7a', (x, y, H + 2), 32, 1.5, steps=8)
        # the hut
        hx = 430.0
        g.box(RED, (hx, 110, H + 110), (190, 170, 220))
        g.box(WHITE, (hx, 110, H + 232), (220, 200, 24))
        g.box(DARK, (hx - 40, 22, H + 80), (60, 6, 150))
        g.box(YEL, (hx + 45, 22, H + 140), (50, 6, 50))
        g.box(STEEL_D, (hx + 60, 150, H + 290), (16, 16, 100))
        for k, (x, y) in enumerate(holes[:3]):                             # the anglers on their stools
            g = sc.still()
            g.box((ORANGE, YEL, PINK)[k], (x, y + 96, H + 34), (44, 44, 68))
            fig(sc, r, x, y + 96, yaw=0, legs='sit', detail=1 if k == 1 else 0, arm_r='fwd', arm_l='hip', swing_r=9, axis_r='x', rate=1.0 + 0.2 * k, hold_r=h_rod())
        x, y = holes[3]
        fig(sc, r, x, y + 100, yaw=0, detail=1, arm_r='up', arm_l='up', swing_r=8, swing_l=-8, bob=12, rate=1.2, hold_r=h_rod(WOOD_D, 120.0, ORANGE))
        x, y = holes[4]
        m = sc.part(P.motion(phase=0.1, rate=0.35, bob=150.0, pivot=(x, y, H)))      # a fish leaping out of the fifth hole
        m.box(PINK, (x, y, H - 40), (20, 34, 70))
        m.box(WHITE, (x, y + 2, H - 82), (12, 40, 18))
        fig(sc, r, x + 90, y + 40, yaw=70, bob=22, rate=1.6, arm_l='vee', arm_r='vee', **kid())
        g = sc.still()
        for ex in (-1, 1):                                                 # the weigh-in rack
            g.box(WOOD_D, (-440 + ex * 90, 160, H + 130), (10, 10, 260))
        g.box(WOOD_D, (-440, 160, H + 256), (200, 10, 10))
        for k, c in enumerate((ORANGE, PINK, SILVER, YEL)):
            m = sc.part(P.motion(phase=k * 0.2, rate=0.6, swing=8.0, pivot=(-440, 160, H + 256), axis=90.0))
            m.box(c, (-506 + k * 44, 160, H + 200 - (k % 2) * 20), (16, 30, 70 + (k % 2) * 30))
        fig(sc, r, -330, 150, yaw=-20, detail=1, arm_l='hold', arm_r='point', hold_l=h_thing(WHITE, (30, 6, 40)), rate=0.5, turn=12, top=VIOLET, hat='cap', hat_col=WHITE)
        g = sc.still()
        g.cyl(BLUE, (160, 60, H), 26, 44, steps=8, top_r=30)               # the bucket, the dog at it
        dog(sc, 215, 80, yaw=-100, graze=16, rate=1.2, body='#6a4428')
        ax, ay = -10.0, 110.0                                              # drilling a new hole: the auger turns
        m = sc.part(P.motion(travel=(-1.0, 0.5), base=(ax, ay - 46)))
        m.box(STEEL, (ax, ay - 46, H + 80), (8, 8, 160))
        for k in range(3):
            m.box(RED, (ax, ay - 46, H + 20 + k * 30), (50, 10, 8), rot=(0, k * 60.0, 0))
        m.box(STEEL_D, (ax, ay - 46, H + 160), (70, 8, 8))
        fig(sc, r, ax, ay, yaw=0, arm_r='hold', arm_l='hold', rate=1.0, rock=4)
        fig(sc, r, 300, 150, yaw=-10, arm_r='wave', arm_l='hip', swing_r=28, rate=1.0, bob=8, hold_l=h_thing(YEL, (24, 16, 28), -16.0))
    return build


# ------------------------------------------------------------------------------------------ 10 the ice-core station
def icecore(seed=110, skirt=(BLUE, ORANGE)):
    """Glaciologists at work: two walk the drill's capstan round under its tripod, the core barrel rising and falling;
    cores laid out on a bench and measured, one held up to the light, a digger in a snow pit, a weather mast turning,
    a balloon on its line."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt)
        sci = dict(wear='coat', hat='beanie', band=WHITE)
        cx, cy = -260.0, 0.0
        for a in (90, 210, 330):                                           # the tripod
            rod(g, STEEL_D, (cx + math.cos(math.radians(a)) * 190, cy + math.sin(math.radians(a)) * 190, H), (cx, cy, H + 520), 5, steps=5)
        g.cyl(DARK, (cx, cy, H + 500), 22, 30, steps=6)
        g.cyl(ORANGE, (cx, cy, H), 40, 50, steps=8)
        m = sc.part(P.motion(phase=0.0, rate=0.25, bob=-70.0, pivot=(cx, cy, H + 250)))
        m.cyl(STEEL, (cx, cy, H + 130), 13, 300, steps=6)
        m.cyl(ICE_DEEP, (cx, cy, H + 100), 9, 30, steps=6)
        m = sc.part(P.motion(travel=(-1.0, 0.09), base=(cx, cy)))          # the capstan's bar, and the two who walk it round
        m.box(YEL, (cx, cy, H + 150), (330, 9, 9))
        for k, ex in enumerate((-1, 1)):
            fig(sc, r, cx + ex * 150, cy - ex * 34, yaw=180 if ex > 0 else 0, detail=1 if k else 0, arm_r='push', arm_l='push', stride=18, rate=1.2,
                orbit=(cx, cy, 0.09), top=ORANGE if k else TEAL, **sci)
        g = sc.still()
        g.box(WOOD, (200, -120, H + 80), (360, 70, 10))                    # the bench of cores
        for ex in (-1, 1):
            g.box(WOOD_D, (200 + ex * 160, -120, H + 38), (10, 60, 76))
        for k in range(4):
            tilt_box(g, ICE if k % 2 else ICE_DEEP, (60 + k * 76, -120 + (k % 2) * 16, H + 96), (130 + k * 76, -120 + (k % 2) * 16, H + 96), 16, 16)
        fig(sc, r, 130, -50, yaw=0, detail=1, arm_r='fwd', arm_l='hold', swing_r=12, axis_r='z', rate=0.9, hold_r=h_thing(YEL, (6, 50, 8), 0.0), top=RED, **sci)
        fig(sc, r, 290, -50, yaw=0, arm_l='hold', arm_r='hold', hold_l=h_thing(WHITE, (30, 6, 40)), rate=0.5, rock=3, top=VIOLET, **sci)
        fig(sc, r, 440, -120, yaw=-30, arm_r='up', arm_l='point', swing_r=10, rate=0.7, bob=4, hold_r=h_thing(ICE_DEEP, (16, 16, 80), 30.0), top=LIME, **sci)
        g = sc.still()
        g.box(ICE_D, (30, 140, H + 20), (190, 130, 40))                    # the snow pit's spoil, and the digger waist deep in it
        fig(sc, r, 30, 120, z=H - 60, yaw=20, arm_r='carry', arm_l='carry', swing_r=26, swing_l=26, axis_r='x', axis_l='x', rate=0.9, hold_r=P.hold_tool(STEEL, ORANGE, 60.0), top=PINK, **sci)
        g = sc.still()
        g.sphere(ORANGE, (300, 150, H), 110, scale=(1.3, 1.0, 0.9), steps=(4, 8))       # the tent, the cases
        g.box(BLUE, (160, 170, H + 22), (70, 46, 44))
        g.box(YEL, (160, 170, H + 58), (56, 40, 28))
        mx, my = 500.0, 150.0                                              # the weather mast: cups going round, a vane
        g.box(STEEL_D, (mx, my, H + 250), (8, 8, 500))
        g.box(BLUE, (mx, my - 30, H + 190), (90, 6, 60), rot=(0, 0, 25))
        m = sc.part(P.motion(travel=(-1.0, 0.7), base=(mx, my)))
        m.box(STEEL, (mx, my, H + 505), (150, 6, 6))
        m.box(STEEL, (mx, my, H + 505), (6, 150, 6))
        for (ox, oy) in ((75, 0), (-75, 0), (0, 75), (0, -75)):
            m.box(RED, (mx + ox, my + oy, H + 505), (26, 26, 26))
        m = sc.part(P.motion(phase=0.3, rate=0.3, swing=30.0, pivot=(mx, my, H + 440), axis=P.UP))
        m.box(YEL, (mx + 50, my, H + 440), (100, 5, 30))
        fig(sc, r, -520, -140, yaw=10, arm_r='up', arm_l='hip', rate=0.6, bob=4, top=YEL, **sci)                # the balloon and who holds it
        m = sc.part(P.motion(phase=0.5, rate=0.3, bob=-22.0, swing=5.0, pivot=(-520, -140, H + 240), axis=0.0))
        m.box(DARK, (-490, -140, H + 400), (4, 4, 260))
        m.sphere(VIOLET, (-490, -140, H + 590), 70, steps=(4, 8))          # (fix round: was orange, the missile pickup's colour and size)
        m.box(WHITE, (-490, -140, H + 590), (146, 146, 16))
        for k in range(5):                                                 # the line of wands along the back
            pennant(sc, -100 + k * 60, 205, H, H + 150, RED if k % 2 else ORANGE, None, w=40.0, h=30.0, phase=k * 0.2, pole=WOOD_D)
    return build


# ------------------------------------------------------------------------------------------ 11 pond hockey
def hockey(seed=111, skirt=(PINK, LIME)):
    """Pond hockey on a swept sheet: four skaters wheeling round after the puck, a goalie rocking in each goal,
    boards behind, the coach waving on the bench, two fans."""
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, skirt, top=ICE)
        g.box(WOOD, (0, 150, H + 35), (1080, 10, 70))
        for k in range(6):
            g.box(PINK if k % 2 else LIME, (-450 + k * 180, 144, H + 40), (120, 4, 30))
        g.box(RED, (0, -20, H + 1), (6, 330, 2))
        g.cyl(BLUE, (0, -20, H), 70, 2, steps=10)
        g.cyl(ICE, (0, -20, H + 1), 60, 2.5, steps=10)
        for ex in (-1, 1):                                                 # the goals
            gx = ex * 480.0
            for y in (-110, 70):
                g.box(RED, (gx, y, H + 60), (8, 8, 120))
            g.box(RED, (gx, -20, H + 120), (8, 188, 8))
            g.box(WHITE, (gx + ex * 30, -20, H + 60), (6, 180, 116))
            fig(sc, r, gx - ex * 50, -20, yaw=ex * 90.0, detail=1, legs='wide', arm_l='out', arm_r='out', rate=0.9, rock=9,
                top=(PINK if ex < 0 else LIME), bottom=WHITE, hat='helmet', hat_col=(PINK if ex < 0 else LIME), hold_r=h_stick(WOOD, 60.0))
            g = sc.still()
        for k, (cx, cy, rad, rps) in enumerate(((-230, -40, 120, 0.2), (-120, 10, 150, -0.16), (200, -30, 130, 0.18), (120, -60, 100, -0.22))):
            a = math.radians(k * 100.0)
            team = PINK if k % 2 else LIME
            fig(sc, r, cx + math.cos(a) * rad, cy + math.sin(a) * rad, yaw=math.degrees(a) + (180.0 if rps > 0 else 0.0), legs='stride',
                arm_r='fwd', arm_l='hip', orbit=(cx, cy, rps), top=team, bottom=BLUE if k % 2 else VIOLET, hat='helmet', hat_col=team, hold_r=h_stick(WOOD, 66.0))
        m = sc.part(P.motion(travel=(-1.0, 0.2), base=(-230.0, -40.0)))    # the puck, with the first of them
        m.cyl(DARK, (-230 + 150, -40, H + 1), 14, 8, steps=6)
        g = sc.still()
        g.box(WOOD_D, (-250, 195, H + 30), (300, 50, 60))                  # the bench
        fig(sc, r, -330, 195, z=H + 60, yaw=0, detail=1, arm_r='up', arm_l='point', swing_r=26, rate=1.2, bob=6, top=YEL, hat='cap', hat_col=RED)
        fig(sc, r, -190, 195, z=H + 14, yaw=0, legs='sit', arm_l='down', arm_r='face', hold_r=P.hold_cup(RED), rate=0.5, rock=3)
        fig(sc, r, 160, 195, yaw=0, arm_r='up', arm_l='up', swing_r=18, swing_l=-18, bob=18, rate=1.3, hold_r=h_flag(LIME, WHITE, 120.0, 70.0, 200.0))
        fig(sc, r, 290, 195, yaw=0, bob=24, rate=1.7, arm_l='vee', arm_r='vee', **kid())
    return build


JOBS = [
    ('SM_P3B_Rescue_A', rescue()),
    ('SM_P3B_IceSculpt_A', sculpt()),
    ('SM_P3B_Wedding_A', wedding()),
    ('SM_P3B_Curling_A', curling()),
    ('SM_P3B_Alphorns_A', alphorns()),
    ('SM_P3B_SnowFort_A', snowfort()),
    ('SM_P3B_BaseCamp_A', basecamp()),
    ('SM_P3B_Podium_A', podium()),
    ('SM_P3B_IceFishing_A', icefishing()),
    ('SM_P3B_IceCore_A', icecore()),
    ('SM_P3B_PondHockey_A', hockey()),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
