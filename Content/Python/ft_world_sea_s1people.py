"""Under the Sea, Stage 1, builder s1people: the people pass 3 left out. /Game/World/UnderTheSea/SM_S1PEOPLE_*.
Run: import ft_world_sea_s1people as K; K.run()

Pass 3's note: nobody on the research station's left, three scenes in the approach, the kelp forest's scenes half
hidden in the weed, Nereida's left hidden behind the travel tube to 8 m up, the level past 13 km thin, and the cut
banks of the banked bends where nothing can stand nearer than 40 m. So every piece here brings its own raised floor,
footed 3 m into the sand: P pieces (plinths, steps, terraces: floors 2.3 to 3.3 m as built), M pieces (two floors, the
upper one 5.8 m or more) and T pieces (towers: a floor 6.2 m up; the kelp cutters' mast has its nest 13.8 m up).
Placed at 1.4 the high floors stand 8.1 m and more over the sand: over the tube and over the weed's foot.
The people are the level's own aquanauts (a suit in a strong colour, never blue; a white helmet with a dark visor; a
pack) doing what their district does. Built 4.3 m long and 2.9 m deep at most.
Nothing here is a fish, a fin, a jaw, a car or a cart; no orange or green box, float or lantern; glow is steady, pale
cream, pink or rose, on still parts only (every rule of this builder stands 24.3 m out or more); nothing blinks,
nothing circles, nothing is thrown; no limb moves faster than 1.1 a second.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'
FOOT = 300.0
G = 0.08            # a steady glow (lamps, lit ports, a pearl)
SAVE = dict(grow=(30, 20, 60), grow_down=(30, 20, 0))

HELM, VISOR, DARK, STEEL = '#f4f9ff', '#22202a', '#22202a', '#8f9bb3'
ROCK, ROCK_D, ROCK_L = '#9a6a7e', '#6a4562', '#c79aa2'
CORAL, PEACH, MAG, LAV, SHELL = '#ff7a6a', '#ffb04f', '#ff4fb8', '#b9a4ff', '#fff1c0'
YEL, STRAW, WOOD_D, PALE = '#ffd23f', '#c9b44a', '#6a4c36', '#f1fbe6'
WHITE, STATION, STATION_D, NAVY, CYAN = '#e6eef2', '#cfe3ea', '#7fa2b4', '#3d4763', '#2ee6ff'
RUST, RUST_D, HULL, SAIL, TEAL, SOOT = '#c8683a', '#7a3f2a', '#8f6450', '#f4efe0', '#3f8a9a', '#3a3140'
MARBLE, PLUM, PINK, ROSE, GOLD_O = '#f4efe0', '#6a4562', '#ff9ec7', '#c79aa2', '#c9b44a'
COPPER, GLASS, INK = '#3fae7a', '#cfeeff', '#3a3140'
RED, INK2, CREAM = '#ff3355', '#16304f', '#fff1c0'
VIOLET, GOLD = '#8a5a9a', '#ffd23f'

S1 = (CORAL, PEACH, MAG, LAV, '#fff6ea')           # the reef's suits
S2 = (YEL, '#fff6ea', '#ff5fa2', YEL)              # the kelp farmers'
S3 = ('#fff6ea', '#ff5fa2', YEL, '#fff6ea')        # the station's
S4 = (RUST, '#ffc93c', '#fff6ea', '#ff5fa2')       # the wreckers'
S5 = (PINK, '#fff6ea', GOLD_O, MAG, LAV)           # the Pearl Quarter's
S6 = (MAG, YEL, '#fff6ea', '#f4a6ff')              # the Bubble Works'
S7 = (RED, '#fff6ea', '#ffc93c', RED)              # Lamp Row's
S8 = (LAV, GOLD, PINK, '#fff6ea', VIOLET)          # the Crown's
KID = dict(build='kid', s=0.86, detail=0)


# ------------------------------------------------------------------------------------------ helpers
def block(g, col, x, y, z0, z1, w, d, glow=0.0):
    g.box(col, (x, y, (z0 + z1) / 2.0), (w, d, z1 - z0), glow=glow)


def drum(g, col, x, y, z0, z1, r, steps=8, top_r=None, glow=0.0):
    g.cyl(col, (x, y, z0), r, z1 - z0, steps=steps, top_r=top_r, glow=glow)


def stilts(g, col, pts, h, t=18.0):
    for (x, y) in pts:
        block(g, col, x, y, -FOOT, h, t, t)


def rail(g, col, x0, x1, y, z, h=64.0, n=4, t=6.0):
    """A rail along X: n posts and a top bar."""
    for k in range(n):
        block(g, col, x0 + (x1 - x0) * k / float(n - 1), y, z, z + h, t, t)
    g.box(col, ((x0 + x1) / 2.0, y, z + h), (abs(x1 - x0) + t, t, t))


def xbrace(g, col, x0, x1, y, z0, z1, t=8.0):
    tilt_box(g, col, (x0, y, z0), (x1, y, z1), t, t)
    tilt_box(g, col, (x0, y, z1), (x1, y, z0), t, t)


def ladder(g, col, x, y, z0, z1, w=40.0, rungs=5):
    for sx in (-1, 1):
        block(g, col, x + sx * w / 2.0, y, z0, z1, 6, 6)
    for k in range(rungs):
        g.box(col, (x, y, z0 + (z1 - z0) * (k + 0.5) / rungs), (w, 5, 5))


def pennant(sc, col, x, y, z, h=200.0, w=70.0, fh=34.0, pole=STEEL, rate=0.4, swing=14.0, ph=0.3):
    """A pole with a long cloth that swings slowly about the pole (the water moves it)."""
    block(sc.still(), pole, x, y, z, z + h, 5, 5)
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z + h - fh / 2.0), axis=P.UP, rate=rate, phase=ph))
    f.box(col, (x + w / 2.0 + 2.0, y, z + h - fh / 2.0), (w, 4, fh))
    f.box(col, (x + w + 12.0, y, z + h - fh / 2.0), (22, 4, fh * 0.5))


def bubbles(sc, x, y, z, n=4, gap=46.0, r0=8.0, ph=0.1, rate=0.35):
    """A string of small pale bubbles, each floating up and down a hand's width out of step with the next."""
    for k in range(n):
        b = sc.part(P.motion(bob=-10.0, rate=rate, phase=(ph + k * 0.27) % 1.0 + 0.001))
        b.sphere(GLASS, (x + (k % 2) * 10 - 5, y, z + k * gap), r0 + k * 2.0, steps=(3, 5))


def lamp(g, x, y, z, h=150.0, col=CREAM, post=INK, r=17.0):
    block(g, post, x, y, z, z + h, 6, 6)
    g.sphere(col, (x, y, z + h + r * 0.8), r, steps=(3, 6), glow=G)


def who(sc, r, x, y, z, yaw=0.0, suits=S1, suit=None, **kw):
    """One of Nereida's people: a suit, a white helmet with a dark visor, a pack. Never blue."""
    c = suit or r.choice(suits)
    d = dict(s=1.2, skin=HELM, hair='bald', hat='helmet', hat_col=HELM, top=c, bottom=c, shoes=DARK, sleeves='long', detail=1,
             build=r.choice(('avg', 'avg', 'slim', 'stocky')), pack=r.choice((STEEL, '#ffc93c', DARK)), phase=0.05 + 0.9 * r.random())
    d.update(kw)
    return P.figure(sc, x, y, z, yaw=yaw, **d)


def plume(sc, col, x, y, top):
    sc.still().box(col, (x, y + 4, top + 10), (9, 30, 30))


def h_basket(col=SHELL, rim=PEACH):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 6, h[2] - 4), 17, 22, steps=6, top_r=21)
        m.cyl(rim, (h[0], h[1] - 6, h[2] + 18), 22, 5, steps=6)
    return f


def h_scythe(col=STEEL, pole=WOOD_D, length=190.0):
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + length / 2.0 - 40.0), (5, 5, length))
        m.box(col, (h[0] + side * 26, h[1], h[2] + length - 46.0), (56, 5, 12))
    return f


def h_clip(col=PALE):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 6, h[2] + 10), (26, 5, 34))
        m.box(NAVY, (h[0], h[1] - 6, h[2] + 26), (16, 6, 6))
    return f


def h_glass(col=GOLD_O):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 26, h[2] + 8), (8, 44, 8))
        m.box(DARK, (h[0], h[1] - 50, h[2] + 8), (11, 8, 11))
    return f


def h_pipe(length=150.0, col=STEEL, ball=GLASS, r=24.0):
    """A glassblower's pipe held ahead and up, a half-blown bubble on its end."""
    def f(m, h, side):
        tip = (h[0], h[1] - length * 0.86, h[2] + length * 0.42)
        tilt_box(m, col, (h[0], h[1] + 14, h[2] - 6), tip, 5, 5)
        m.sphere(ball, tip, r, steps=(4, 6))
    return f


def h_lance(col=PINK, pole=GOLD_O, length=210.0, w=54.0):
    """A staff with a pennant (a guard's, a herald's): no blade."""
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + length / 2.0 - 60.0), (5, 5, length))
        m.box(col, (h[0] + side * (w / 2.0 + 3), h[1], h[2] + length - 78.0), (w, 4, 26))
        m.sphere(pole, (h[0], h[1], h[2] + length - 56.0), 7, steps=(3, 5))
    return f


def h_flag(col=PINK, pole=GOLD_O, w=60.0):
    """A streamer on a stick (5 cm thick: the ink flickers on thinner cloth)."""
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + 45), (5, 5, 110))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + 84), (w, 5, 30))
    return f


def h_cloth(col=CREAM):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 4, h[2] + 4), (22, 6, 20))
    return f


def h_horn(col=GOLD):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 26, h[2] + 10), (7, 50, 7))
        m.box(col, (h[0], h[1] - 56, h[2] + 10), (22, 12, 22))
    return f


def h_tray(col=PLUM, pearl=SHELL):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 8, h[2] + 6), (56, 30, 5))
            for k in (-1, 0, 1):
                m.sphere(pearl, (h[0] - 24 + k * 16, h[1] - 8, h[2] + 15), 7, steps=(3, 5))
    return f


def h_yoke(col=WOOD_D, ball=GLASS):
    """A carrier's yoke: a bar across the hands, a net of pale glass floats at each end (never orange)."""
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 26, h[1], h[2] + 8), (150, 6, 6))
            for sx in (-1, 1):
                m.box(col, (h[0] - 26 + sx * 70, h[1], h[2] - 8), (4, 4, 28))
                m.sphere(ball, (h[0] - 26 + sx * 70, h[1], h[2] - 36), 20, steps=(3, 6))
    return f


# ------------------------------------------------------------------------------------------ s1d1, the reef
def d1_gardeners(sc):
    """P. Reef gardeners on a rock mound: one up a ladder trimming a coral tree, one with a basket, a child waving."""
    r = random.Random(611)
    g = sc.still()
    drum(g, ROCK, -50, 0, -FOOT, 150, 142, steps=8)
    drum(g, ROCK_L, -50, 0, 150, 232, 132, steps=8)
    drum(g, SHELL, -50, 0, 222, 236, 138, steps=8)                       # a shell rim round the top
    drum(g, CORAL, 150, 40, -FOOT, 330, 26, steps=6, top_r=18)           # the coral tree
    for k, (dx, dy, zz, c) in enumerate(((70, -20, 520, MAG), (-60, 30, 560, CORAL), (20, 60, 620, PEACH), (-30, -40, 470, LAV))):
        rod(g, CORAL if k % 2 else PEACH, (150, 40, 300 + k * 10), (150 + dx, 40 + dy, zz), 11, steps=5)
        g.sphere(c, (150 + dx, 40 + dy, zz + 10), 26, steps=(3, 6))
    g.box(LAV, (40, 96, 330), (120, 8, 190))                             # a sea fan behind
    g.box(MAG, (40, 92, 330), (70, 6, 120))
    block(g, PEACH, 150, -50, 236, 250, 96, 80)                          # the ledge the trimmer stands on
    ladder(g, WOOD_D, 150, -96, -120, 250, rungs=6)
    for k in range(3):                                                   # the baskets of cuttings
        drum(g, SHELL, -150 + k * 34, 70, 236, 262, 16, steps=6, top_r=19)
        g.sphere((MAG, PEACH, LAV)[k], (-150 + k * 34, 70, 268), 12, steps=(3, 5))
    pennant(sc, MAG, -150, 20, 236, h=230, w=80, ph=0.2)
    who(sc, r, 150, -52, 250, yaw=170, suits=S1, arm_r='up', hold_r=P.hold_tool(STEEL, MAG, 34), swing_r=14, axis_r='x', rate=0.7)
    who(sc, r, -96, -44, 236, yaw=-12, suits=S1, arm_l='carry', arm_r='carry', hold_r=h_basket())
    who(sc, r, -20, -74, 236, yaw=8, suits=S1, arm_r='wave', swing_r=24, bob=5, rate=0.9, react=True, **KID)
    who(sc, r, 20, 10, 236, yaw=60, suits=S1, arm_r='point')


def d1_pearlstair(sc):
    """P. The pearl divers' stair: three shell tiers under a scallop awning; a diver holds up the day's pearl."""
    r = random.Random(612)
    g = sc.still()
    block(g, PEACH, 0, 0, -FOOT, 120, 420, 280)
    block(g, CORAL, 0, 46, 120, 230, 420, 188)
    block(g, PEACH, 30, 96, 230, 330, 300, 88)
    for (y, z, w, x) in ((-140, 110, 424, 0), (-48, 220, 424, 0), (52, 320, 304, 30)):
        g.box(SHELL, (x, y, z + 6), (w, 8, 20))                          # cream rims
    for k in range(5):                                                   # scallops along the foot
        drum(g, SHELL, -168 + k * 84, -120, 20, 80, 24, steps=6, top_r=8)
    for sx in (-1, 1):
        block(g, LAV, 30 + sx * 140, 122, 330, 560, 8, 8)
    for k in range(5):                                                   # the awning: a fan of ribs
        a = math.radians(-60 + k * 30)
        tilt_box(g, MAG if k % 2 == 0 else LAV, (30, 112, 556), (30 + math.sin(a) * 170, 112 - math.cos(a) * 120, 596 + math.cos(a) * 30), 56, 6)
    block(g, ROCK_L, -170, 96, 230, 290, 70, 80)                         # a bench of rock
    for k in range(2):
        drum(g, SHELL, 150 + k * 36, -20, 230, 258, 16, steps=6, top_r=20)
    lamp(g, -196, -120, 120, h=120, col=SHELL, post=LAV)
    t = who(sc, r, 20, 96, 330, yaw=0, suits=S1, arm_r='up')
    g = sc.still()
    g.sphere(SHELL, (20 + 42, 96, t - 6), 15, steps=(4, 6), glow=G)        # the pearl, over the head
    who(sc, r, -80, -64, 120, yaw=-20, suits=S1, arm_l='carry', arm_r='carry', hold_r=h_basket())
    who(sc, r, 96, -10, 230, yaw=14, suits=S1, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)
    who(sc, r, -170, 50, 230, yaw=10, suits=S1, legs='sit', seat=50, arm_r='wave', swing_r=20, rate=0.8)
    who(sc, r, 130, -96, 120, yaw=30, suits=S1, arm_l='hip', arm_r='point')


def d1_conch(sc):
    """P. A conch hut on rock stilts, its balcony full: children wave, a pennant of weed on the tip."""
    r = random.Random(613)
    g = sc.still()
    for (x, y) in ((-170, -100), (170, -100), (-170, 100), (170, 100)):
        drum(g, ROCK, x, y, -FOOT, 300, 22, steps=6, top_r=16)
    block(g, SHELL, 0, 0, 300, 316, 410, 280)
    block(g, PEACH, 0, -138, 292, 312, 410, 6)
    rail(g, LAV, -200, 200, -134, 316, h=60, n=5)
    drum(g, CORAL, 40, 40, 316, 520, 86, steps=8, top_r=74)              # the shell
    zz, rr = 520.0, 96.0
    for k in range(5):                                                   # the spiral, turn on turn
        drum(g, MAG if k % 2 == 0 else LAV, 40 + k * 6, 40, zz, zz + 52 - k * 4, rr, steps=8, top_r=rr * 0.74)
        drum(g, SHELL, 40 + k * 6, 40, zz - 4, zz + 6, rr + 6, steps=8)
        zz += 52 - k * 4
        rr *= 0.72
    g.box(ROCK_D, (40, -46, 376), (56, 6, 110))                          # the door mouth
    g.box(SHELL, (100, -36, 430), (30, 6, 30), glow=G)                   # a lit round window
    ladder(g, WOOD_D, -150, 136, -100, 316, rungs=7)
    pennant(sc, '#7fcf6a', 64, 40, zz, h=150, w=90, fh=22, pole=SHELL, ph=0.5)
    who(sc, r, -150, -76, 316, yaw=-10, suits=S1, arm_r='wave', swing_r=26, bob=5, rate=1.0, react=True, **KID)
    who(sc, r, -96, -80, 316, yaw=6, suits=S1, arm_l='vee', arm_r='vee', bob=7, rate=1.1, react=True, **KID)
    who(sc, r, -30, -70, 316, yaw=0, suits=S1, arm_l='wave', swing_l=18, rate=0.7)
    who(sc, r, 150, -74, 316, yaw=20, suits=S1, arm_l='hold', arm_r='hold', hold_r=h_basket(SHELL, MAG))


# ------------------------------------------------------------------------------------------ s1d2, the kelp forest
def d2_stiltdeck(sc):
    """P. A kelp farmers' deck on stilts under a yellow thatch: two haul at the winch, weed sheets dry on a line."""
    r = random.Random(621)
    g = sc.still()
    stilts(g, WOOD_D, ((-190, -120), (190, -120), (-190, 120), (190, 120)), 320, 16)
    xbrace(g, WOOD_D, -190, 190, 120, -40, 280)
    block(g, '#9a744a', 0, 0, 320, 336, 420, 280)
    rail(g, WOOD_D, -204, 204, -134, 336, h=60, n=5)
    for sx in (-1, 1):
        block(g, WOOD_D, sx * 190, 110, 336, 600, 10, 10)
    tilt_box(g, YEL, (0, 30, 610), (0, 142, 540), 430, 12)               # the thatch: the district's yellow roof
    tilt_box(g, YEL, (0, 30, 610), (0, -40, 566), 430, 12)
    for k in range(6):
        g.box(STRAW, (-180 + k * 72, -44, 540), (40, 6, 44))             # the fringe past the eaves
    g.box(WOOD_D, (0, 104, 520), (380, 5, 5))                            # the drying line
    for k in range(4):
        f = sc.part(P.motion(swing=7, pivot=(-140 + k * 92, 104, 520), axis=0.0, rate=0.3, phase=0.1 + k * 0.21))
        f.box(STRAW if k % 2 else '#a8b04a', (-140 + k * 92, 104, 452), (70, 5, 130))
    g = sc.still()
    drum(g, WOOD_D, 120, -20, 336, 400, 30, steps=6)                     # the winch
    block(g, STEEL, 120, -20, 400, 412, 90, 8)
    tilt_box(g, SHELL, (120, -50, 380), (70, -140, 300), 5, 5)           # its rope, over the side
    for k in range(2):
        block(g, STRAW, -150 + k * 66, 60, 336, 396, 60, 60)             # baled weed, straw colour
        g.box(WOOD_D, (-150 + k * 66, 60, 366), (62, 62, 5))
    block(g, WOOD_D, -196, -124, 336, 470, 6, 6)                         # a float-glass lantern
    g.sphere(GLASS, (-196, -124, 486), 18, steps=(3, 6), glow=G)
    pennant(sc, YEL, 190, 110, 600, h=150, w=70, ph=0.7)
    who(sc, r, 60, -30, 336, yaw=-70, suits=S2, arm_l='push', arm_r='push', rock=4, rate=0.6)
    who(sc, r, 170, -40, 336, yaw=80, suits=S2, arm_l='push', arm_r='push', rock=4, rate=0.6)
    who(sc, r, -60, -84, 336, yaw=0, suits=S2, arm_r='wave', swing_r=22, rate=0.8, react=True)
    who(sc, r, -130, -80, 336, yaw=10, suits=S2, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)


def d2_cuttersmast(sc):
    """T. The cutters' mast: a look-out in the nest 13.8 m up under a yellow cone, cutters with long scythes below."""
    r = random.Random(622)
    g = sc.still()
    block(g, WOOD_D, 0, 20, -FOOT, 1560, 26, 26)
    for (x, y) in ((-190, -110), (190, -110), (-190, 120), (190, 120)):
        tilt_box(g, WOOD_D, (x, y, -FOOT), (x * 0.74, y * 0.7, 620), 16, 16)
    block(g, '#9a744a', 0, 0, 620, 636, 320, 250)
    rail(g, WOOD_D, -156, 156, -120, 636, h=60, n=4)
    for (x, y) in ((-150, -110), (150, -110), (0, 136)):                 # stays up to the nest
        tilt_box(g, SHELL, (x, y, 636), (0, 20, 1360), 5, 5)
    drum(g, '#9a744a', 0, 20, 1380, 1396, 96, steps=8)                   # the nest
    drum(g, WOOD_D, 0, 20, 1396, 1450, 98, steps=8, top_r=104)
    for k in range(4):
        a = math.radians(45 + k * 90)
        block(g, WOOD_D, math.cos(a) * 88, 20 + math.sin(a) * 88, 1450, 1590, 6, 6)
    drum(g, YEL, 0, 20, 1590, 1700, 124, steps=8, top_r=10)              # the yellow cone
    drum(g, STRAW, 0, 20, 1576, 1592, 128, steps=8)
    ladder(g, WOOD_D, 30, 40, 636, 1380, w=34, rungs=12)
    g.box(YEL, (-120, 116, 720), (70, 6, 150))                           # a banner on the back rail, no words
    g.box(STRAW, (-120, 113, 720), (40, 4, 40))
    for k in range(3):                                                   # cut weed hung to dry under the deck
        f = sc.part(P.motion(swing=6, pivot=(-100 + k * 100, -100, 620), axis=0.0, rate=0.3, phase=0.2 + k * 0.3))
        f.box('#a8b04a' if k % 2 else STRAW, (-100 + k * 100, -100, 520), (50, 5, 200))
    pennant(sc, YEL, 0, 20, 1700, h=110, w=90, ph=0.4)
    who(sc, r, 0, -30, 1396, yaw=0, suits=S2, arm_l='face', arm_r='face', hold_r=h_glass(), turn=16, rate=0.25)
    who(sc, r, -100, -60, 636, yaw=-16, suits=S2, arm_r='up', hold_r=h_scythe(), swing_r=10, axis_r='y', rate=0.5)
    who(sc, r, 100, -60, 636, yaw=14, suits=S2, arm_l='up', hold_l=h_scythe(), swing_l=10, axis_l='y', rate=0.5)
    who(sc, r, 10, -90, 636, yaw=0, suits=S2, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True)


# ------------------------------------------------------------------------------------------ s1d3, the research station
def d3_roofdeck(sc):
    """T. A white drum lab on three splayed legs, scientists on its roof deck 6.2 m up, a current vane turning."""
    r = random.Random(631)
    g = sc.still()
    for k in range(3):
        a = math.radians(90 + k * 120)
        rod(g, STATION_D, (math.cos(a) * 200, math.sin(a) * 130, -FOOT), (math.cos(a) * 90, math.sin(a) * 80, 340), 16, steps=6)
    drum(g, NAVY, 0, 0, -FOOT, 330, 26, steps=6)
    drum(g, WHITE, 0, 0, 330, 600, 132, steps=10)
    drum(g, CYAN, 0, 0, 430, 452, 135, steps=10)                         # the station's cyan stripe (paint)
    for k in (-1, 0, 1):
        g.box(NAVY, (k * 70, -128 + abs(k) * 14, 520), (40, 8, 40))
        g.box(CREAM, (k * 70, -131 + abs(k) * 14, 520), (26, 5, 26), glow=G)
    drum(g, STATION, 0, 0, 600, 620, 142, steps=10)                      # the roof deck
    for k in range(8):
        a = math.radians(22.5 + k * 45)
        block(g, STATION_D, math.cos(a) * 134, math.sin(a) * 134, 620, 684, 6, 6)
    drum(g, STATION_D, 0, 0, 682, 689, 139, steps=10)
    drum(g, STATION, 0, 0, 686, 691, 131, steps=10)
    block(g, NAVY, 70, 70, 620, 800, 7, 7)                               # the dish
    drum(g, WHITE, 70, 70, 800, 830, 16, steps=8, top_r=62)
    block(g, NAVY, -80, 80, 620, 860, 6, 6)                              # the vane's mast, a steady lamp on it
    g.sphere(MAG, (-80, 80, 870), 12, steps=(3, 5), glow=G)
    v = sc.part(P.motion(swing=50, pivot=(-80, 80, 820), axis=P.UP, rate=0.15, phase=0.3))
    v.box(STATION_D, (-80, 80, 820), (120, 5, 5))
    v.box(MAG, (-128, 80, 820), (30, 5, 34))
    g = sc.still()
    for k in range(3):                                                   # an instrument tripod
        a = math.radians(k * 120)
        tilt_box(g, NAVY, (60 + math.cos(a) * 26, -50 + math.sin(a) * 26, 620), (60, -50, 720), 5, 5)
    g.box(WHITE, (60, -52, 730), (30, 30, 22))
    who(sc, r, -70, -84, 620, yaw=-14, suits=S3, arm_l='hold', arm_r='hold', hold_r=h_clip())
    who(sc, r, 6, -80, 620, yaw=0, suits=S3, arm_r='point', arm_l='hip')
    who(sc, r, 96, -40, 620, yaw=50, suits=S3, arm_r='wave', swing_r=20, rate=0.8, react=True)
    who(sc, r, -20, 20, 620, yaw=20, suits=S3, arm_l='face', arm_r='face', hold_r=h_glass(STATION_D), detail=0)


def d3_diverclass(sc):
    """P. A diver class on a stepped white platform: the teacher points at a picture board, three sit and one asks."""
    r = random.Random(632)
    g = sc.still()
    stilts(g, STATION_D, ((-190, -120), (190, -120), (-190, 120), (190, 120), (0, 0)), 240, 20)
    xbrace(g, STATION_D, -190, 190, 124, -60, 220)
    block(g, WHITE, 0, 0, 240, 258, 420, 280)
    block(g, CYAN, 0, -138, 236, 254, 420, 6)
    block(g, STATION, 60, 96, 258, 322, 290, 80)                         # the bench
    block(g, STATION_D, 60, 54, 258, 264, 290, 6)
    for sx in (-1, 1):                                                   # the board: pictures only
        block(g, NAVY, -150 + sx * 44, 110, 258, 480, 6, 6)
    g.box(PALE, (-150, 106, 430), (120, 6, 110))
    g.box(MAG, (-176, 102, 452), (34, 4, 34))
    g.box(YEL, (-132, 102, 452), (34, 4, 20))
    g.box(NAVY, (-150, 102, 408), (84, 4, 8))
    for k in range(4):                                                   # sample jars, pale glass
        drum(g, GLASS, 150 + (k % 2) * 30, -100 + (k // 2) * 30, 258, 300, 11, steps=6)
        drum(g, NAVY, 150 + (k % 2) * 30, -100 + (k // 2) * 30, 300, 306, 12, steps=6)
    lamp(g, 196, 126, 258, h=170, col=CREAM, post=NAVY)
    who(sc, r, -80, 40, 258, yaw=-50, suit='#fff6ea', arm_r='point', arm_l='hip', band=MAG, rock=3, rate=0.4)
    for k in range(3):
        who(sc, r, -30 + k * 76, 92, 258, yaw=-8 + k * 8, suits=S3, legs='sit', seat=53, pack=YEL,
            **(dict(arm_r='up', swing_r=10, rate=0.7) if k == 1 else dict()))
    who(sc, r, 120, -60, 258, yaw=24, suits=S3, arm_l='wave', swing_l=22, rate=0.9, react=True, pack=YEL)


# ------------------------------------------------------------------------------------------ s1d4, the salvage field and the approach
def d4_hulldeck(sc):
    """P. A wreckers' deck on a ship's steel box: two haul at the winch, a net of plates swings from the jib."""
    r = random.Random(641)
    g = sc.still()
    block(g, TEAL, 0, 10, -FOOT, 250, 420, 260)
    for k in range(6):
        block(g, '#327482', -175 + k * 70, -121, -100, 240, 14, 6)       # the box's ribs
    block(g, SAIL, 0, 0, 250, 264, 424, 286)
    for k in range(3):
        g.box(SOOT, (-120 + k * 120, -122, 130), (44, 6, 44))            # portholes
        g.box(CREAM, (-120 + k * 120, -125, 130), (26, 4, 26), glow=G)
    rail(g, SOOT, -206, 206, -138, 264, h=60, n=5)
    drum(g, SOOT, -120, 40, 264, 330, 34, steps=6)                       # the winch
    block(g, RUST_D, -120, 40, 330, 344, 100, 10)
    tilt_box(g, RUST_D, (-150, 90, 264), (60, 60, 640), 14, 14)          # the jib
    tilt_box(g, RUST_D, (-90, 120, 264), (60, 60, 640), 10, 10)
    tilt_box(g, SOOT, (-120, 40, 330), (60, 60, 640), 5, 5)
    n = sc.part(P.motion(swing=5, pivot=(60, 60, 640), axis=90.0, rate=0.3, phase=0.4))
    n.box(SOOT, (60, 60, 560), (5, 5, 160))
    n.box(HULL, (60, 60, 450), (110, 70, 8))
    n.box(RUST_D, (50, 56, 462), (80, 50, 8))
    n.box(STEEL, (66, 62, 474), (96, 10, 10))
    n.box(SOOT, (60, 60, 500), (120, 80, 5))
    g = sc.still()
    for sx in (-1, 1):                                                   # a patched sail for an awning
        block(g, HULL, 130 + sx * 70, 130, 264, 520, 8, 8)
    tilt_box(g, SAIL, (130, 134, 520), (130, 20, 470), 160, 6)
    g.box(TEAL, (100, 70, 498), (36, 30, 5))
    g.box(RUST_D, (160, 96, 510), (30, 24, 5))
    drum(g, HULL, 150, 90, 264, 300, 26, steps=6)                        # a capstan
    drum(g, SOOT, 150, 90, 300, 316, 34, steps=6)
    who(sc, r, -170, -40, 264, yaw=-60, suits=S4, arm_l='push', arm_r='push', rock=4, rate=0.5)
    who(sc, r, -60, -50, 264, yaw=60, suits=S4, arm_l='push', arm_r='push', rock=4, rate=0.5)
    who(sc, r, 40, -90, 264, yaw=0, suits=S4, arm_r='up', arm_l='hip', swing_r=12, rate=0.6, band=CREAM)
    who(sc, r, 150, -84, 264, yaw=14, suits=S4, arm_l='wave', swing_l=24, rate=0.9, react=True)


def d4_forge(sc):
    """P. The chimney-rock forge on an iron floor: a smith at the anvil, a helper, the funnel smoking bubbles."""
    r = random.Random(642)
    g = sc.still()
    drum(g, ROCK_D, 0, 0, -FOOT, 200, 142, steps=8, top_r=132)
    block(g, SOOT, 0, 0, 200, 240, 420, 280)
    block(g, RUST_D, 0, -138, 196, 236, 420, 6)
    stilts(g, SOOT, ((-196, -126), (196, -126), (-196, 126), (196, 126)), 200, 14)
    drum(g, SOOT, 120, 70, 240, 400, 62, steps=8, top_r=50)              # the funnel for a chimney
    drum(g, SAIL, 120, 70, 400, 440, 51, steps=8)
    drum(g, RUST_D, 120, 70, 440, 600, 50, steps=8, top_r=44)
    drum(g, SOOT, 120, 70, 600, 624, 50, steps=8)
    g.box(SOOT, (120, 6, 300), (70, 10, 70))                             # the fire mouth: a steady rose light
    g.box('#ff6a7a', (120, 2, 296), (48, 6, 44), glow=G)
    bubbles(sc, 120, 70, 660, n=4, gap=50, r0=10, ph=0.2)
    g = sc.still()
    block(g, SOOT, -20, -40, 240, 290, 40, 30)                           # the anvil
    block(g, STEEL, -20, -40, 290, 312, 84, 34)
    for k in range(2):                                                   # ship's wheels on a rack
        g.box(HULL, (-160, 110, 320 + k * 90), (76, 8, 8))
        g.box(HULL, (-160, 110, 320 + k * 90), (8, 8, 76))
        g.box(RUST_D, (-160, 112, 320 + k * 90), (54, 6, 54))
    block(g, SOOT, -160, 124, 240, 440, 8, 8)
    block(g, TEAL, 30, 100, 240, 300, 70, 50)                            # the quench tub
    block(g, GLASS, 30, 100, 300, 304, 60, 40)
    lamp(g, -196, -126, 240, h=150, col=CREAM, post=SOOT)
    who(sc, r, -76, -44, 240, yaw=-70, suits=S4, arm_r='up', hold_r=P.hold_tool(STEEL, WOOD_D, 30), swing_r=26, axis_r='x', rate=0.8, build='stocky')
    who(sc, r, 44, -40, 240, yaw=70, suits=S4, arm_l='fwd', arm_r='fwd', hold_r=P.hold_tool(SOOT, SOOT, 40))
    who(sc, r, -150, -70, 240, yaw=-10, suits=S4, arm_r='wave', swing_r=24, bob=5, rate=0.9, react=True, **KID)
    who(sc, r, 150, -90, 240, yaw=10, suits=S4, arm_l='hip', arm_r='hip', band=CREAM)


def d4_bridgehouse(sc):
    """T. A wheelhouse set up on crane legs, 6.2 m up: look-outs along its rail with a spyglass, lit ports behind."""
    r = random.Random(643)
    g = sc.still()
    stilts(g, RUST_D, ((-180, -110), (180, -110), (-180, 110), (180, 110)), 620, 18)
    for (z0, z1) in ((-40, 280), (300, 600)):
        xbrace(g, RUST_D, -180, 180, 110, z0, z1)
        xbrace(g, RUST_D, -180, 180, -110, z0, z1, 6)
    block(g, HULL, 0, 0, 290, 300, 380, 240)
    block(g, SAIL, 0, 0, 620, 636, 420, 284)
    block(g, RUST, 0, -140, 612, 632, 420, 6)
    rail(g, SOOT, -204, 204, -136, 636, h=62, n=5)
    block(g, SAIL, 20, 84, 636, 860, 300, 110)                           # the wheelhouse
    block(g, TEAL, 20, 80, 860, 880, 330, 140)
    g.box(SOOT, (20, 27, 780), (260, 6, 70))                             # its wide window
    for k in range(3):
        g.box(CREAM, (-64 + k * 84, 24, 780), (60, 4, 50), glow=G)
    drum(g, SOOT, 130, 90, 880, 990, 30, steps=6, top_r=26)              # the funnel
    drum(g, SAIL, 130, 90, 950, 968, 29, steps=6)
    for k in range(2):                                                   # life rings, cream and teal
        g.box(SAIL if k == 0 else TEAL, (-150 + k * 300, 26, 700), (44, 6, 44))
        g.box(SOOT, (-150 + k * 300, 24, 700), (20, 6, 20))
    ladder(g, SOOT, -150, 126, 300, 636, rungs=7)
    pennant(sc, TEAL, -110, 90, 880, h=170, w=80, ph=0.6)
    who(sc, r, -130, -44, 636, yaw=-16, suits=S4, arm_l='face', arm_r='face', hold_r=h_glass(), turn=14, rate=0.25)
    who(sc, r, -40, -84, 636, yaw=0, suits=S4, arm_r='point', arm_l='hip')
    who(sc, r, 56, -80, 636, yaw=8, suits=S4, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True)
    who(sc, r, 150, -78, 636, yaw=20, suits=S4, arm_r='wave', swing_r=22, rate=0.8, react=True, **KID)


# ------------------------------------------------------------------------------------------ s1d5, Nereida: the Pearl Quarter
def d5_templesteps(sc):
    """P. Families on a temple's steps: four marble steps to a porch of columns, a pearl on the gable."""
    r = random.Random(651)
    g = sc.still()
    for k in range(4):
        block(g, MARBLE if k % 2 == 0 else ROSE, 0, k * 35.0, -FOOT, 70 + k * 62, 424, 280 - k * 70)
    for (x, y) in ((-180, 124), (-60, 124), (60, 124), (180, 124)):
        drum(g, MARBLE, x, y, 256, 520, 15, steps=6)
        block(g, PLUM, x, y, 256, 270, 40, 36)
        block(g, GOLD_O, x, y, 512, 524, 40, 36)
    block(g, MARBLE, 0, 124, 524, 560, 424, 44)
    tilt_box(g, PINK, (-212, 124, 566), (0, 124, 640), 44, 14)
    tilt_box(g, PINK, (212, 124, 566), (0, 124, 640), 44, 14)
    g.sphere(SHELL, (0, 124, 668), 22, steps=(4, 6), glow=G)
    g.box(PLUM, (0, 144, 400), (300, 5, 240))                            # the door's shadow
    for sx in (-1, 1):                                                   # urns with trailing weed
        drum(g, ROSE, sx * 190, -100, 70, 130, 20, steps=6, top_r=26)
        g.sphere(LAV, (sx * 190, -100, 140), 22, steps=(3, 6))
    who(sc, r, -110, -40, 132, yaw=-10, suits=S5, arm_r='wave', swing_r=22, rate=0.8, react=True)
    who(sc, r, -50, -44, 132, yaw=0, suits=S5, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)
    who(sc, r, 60, 30, 194, yaw=6, suits=S5, wear='dress', arm_l='wave', swing_l=20, rate=0.7)
    who(sc, r, 130, 34, 194, yaw=14, suits=S5, arm_r='up', hold_r=h_flag(PINK, GOLD_O, 50), swing_r=18, rate=0.7, **KID)
    who(sc, r, -20, 96, 256, yaw=0, suits=S5, arm_l='hold', arm_r='hold', hold_r=h_tray(), detail=0)


def d5_guards(sc):
    """M. Plumed guards on plinths: two on a stepped plinth by a banner mast, one 6 m up on a great fluted drum."""
    r = random.Random(652)
    g = sc.still()
    block(g, ROSE, -100, 0, -FOOT, 240, 220, 270)
    block(g, MARBLE, -100, 10, 240, 300, 190, 220)
    block(g, GOLD_O, -100, -101, 280, 296, 190, 5)
    drum(g, MARBLE, 120, 20, -FOOT, 560, 74, steps=10)
    for k in range(5):
        drum(g, ROSE, 120, 20, 40 + k * 104, 52 + k * 104, 77, steps=10)   # the drum's bands
    drum(g, PLUM, 120, 20, 560, 580, 88, steps=10)
    block(g, MARBLE, 120, 20, 580, 600, 190, 190)
    block(g, GOLD_O, 120, -76, 584, 598, 190, 5)
    g.box(PINK, (120, -57, 330), (60, 6, 300))                           # a long banner down the drum, no words
    g.box(GOLD_O, (120, -60, 440), (36, 4, 36))
    block(g, GOLD_O, -100, 96, 300, 640, 7, 7)
    f = sc.part(P.motion(swing=10, pivot=(-100, 96, 560), axis=P.UP, rate=0.35, phase=0.3))
    f.box(PINK, (-52, 96, 560), (90, 4, 150))
    f.box(SHELL, (-52, 93, 580), (40, 4, 40))
    g = sc.still()
    g.sphere(SHELL, (-100, 96, 652), 14, steps=(3, 6), glow=G)
    for (x, y, z, yw, c) in ((-160, -50, 300, -10, PINK), (-40, -50, 300, 10, PINK), (120, -20, 600, 0, MAG)):
        t = who(sc, r, x, y, z, yaw=yw, suit='#fff6ea', band=c, pack=GOLD_O, build='avg', arm_r='up', hold_r=h_lance(c), **(dict(detail=1)))
        plume(sc, c, x, y, t)
    who(sc, r, -100, 20, 300, yaw=0, suits=S5, arm_l='face', arm_r='face', hold_r=h_horn(GOLD_O), rock=3, rate=0.5, detail=0)
    who(sc, r, 170, 60, 600, yaw=20, suits=S5, arm_l='wave', swing_l=24, bob=5, rate=0.9, react=True, **KID)


def d5_arcade(sc):
    """T. A two-storey arcade under a pink dome: pearl merchants below, the crowd of the fight 6.2 m up."""
    r = random.Random(653)
    g = sc.still()
    block(g, ROSE, 0, 0, -FOOT, 30, 424, 284)
    for z0 in (30, 320):
        for x in (-190, -64, 64, 190):
            drum(g, MARBLE, x, -122, z0, z0 + 260, 14, steps=6)
            block(g, GOLD_O, x, -122, z0 + 250, z0 + 262, 36, 30)
        for k in range(3):                                               # round arches: a lintel and a keystone
            g.box(MARBLE, (-127 + k * 127, -122, z0 + 246), (100, 22, 30))
            g.box(PINK, (-127 + k * 127, -125, z0 + 220), (70, 8, 26))
        block(g, MARBLE, 0, 110, z0, z0 + 290, 424, 60)
        g.box(PLUM, (0, 78, z0 + 150), (360, 6, 200))
    block(g, MARBLE, 0, 0, 290, 320, 424, 284)
    block(g, MARBLE, 0, 0, 600, 620, 424, 284)
    rail(g, GOLD_O, -204, 204, -136, 620, h=58, n=6)
    for (x, y) in ((-150, -90), (150, -90), (-150, 110), (150, 110)):
        block(g, MARBLE, x, y, 620, 880, 12, 12)
    block(g, MARBLE, 0, 0, 880, 900, 400, 280)
    g.sphere(PINK, (0, 10, 900), 150, scale=(1, 0.86, 0.5), steps=(4, 10))
    g.sphere(SHELL, (0, 10, 990), 20, steps=(4, 6), glow=G)
    lamp(g, -196, -128, 320, h=120, col=SHELL, post=GOLD_O, r=14)
    who(sc, r, -110, -60, 320, yaw=-8, suits=S5, arm_l='hold', arm_r='hold', hold_r=h_tray(), detail=0)
    who(sc, r, -130, -84, 620, yaw=-10, suits=S5, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True)
    who(sc, r, -40, -86, 620, yaw=0, suits=S5, wear='dress', arm_r='wave', swing_r=22, rate=0.8, react=True)
    who(sc, r, 50, -84, 620, yaw=4, suits=S5, arm_r='up', hold_r=h_flag(PINK, GOLD_O, 56), swing_r=18, rate=0.7, **KID)
    who(sc, r, 136, -84, 620, yaw=12, suits=S5, arm_l='wave', swing_l=20, rate=0.9, react=True)


def d5_grandstair(sc):
    """M. A grand open stair: families climb two marble flights to a landing 6 m up under a pink shell canopy."""
    r = random.Random(654)
    g = sc.still()
    block(g, ROSE, 0, 0, -FOOT, 30, 424, 284)
    for k in range(4):                                                   # the first flight, along the front
        block(g, MARBLE if k % 2 == 0 else ROSE, -180 + k * 54, -80, 30, 97 + k * 67, 54, 120)
    block(g, MARBLE, 110, 0, 284, 300, 204, 284)                         # the half landing
    for (x, y) in ((200, -130), (200, 130), (20, 130)):
        drum(g, MARBLE, x, y, 30, 284, 13, steps=6)
    for k in range(4):                                                   # the second flight, along the back
        block(g, MARBLE if k % 2 else ROSE, 150 - k * 54, 80, 355 + k * 75, 375 + k * 75, 56, 120)
    tilt_box(g, PLUM, (180, 80, 290), (-30, 80, 590), 100, 14)
    block(g, MARBLE, -116, 0, 584, 600, 196, 284)                        # the top landing
    block(g, GOLD_O, -116, -143, 580, 598, 196, 4)
    for (x, y) in ((-204, -130), (-204, 130), (-30, -130)):
        drum(g, MARBLE, x, y, 30, 584, 14, steps=6)
        block(g, GOLD_O, x, y, 572, 584, 38, 34)
    rail(g, GOLD_O, -208, -24, -134, 600, h=58, n=3)
    rail(g, GOLD_O, 12, 208, -134, 300, h=58, n=3)
    for sx in (-1, 1):
        block(g, GOLD_O, -116 + sx * 86, 120, 600, 850, 7, 7)
    for k in range(5):                                                   # the canopy: a shell of five ribs
        a = math.radians(-60 + k * 30)
        tilt_box(g, PINK if k % 2 == 0 else SHELL, (-116, 124, 846), (-116 + math.sin(a) * 120, 124 - math.cos(a) * 150, 880 + math.cos(a) * 30), 50, 6)
    g.sphere(SHELL, (-116, 124, 880), 18, steps=(4, 6), glow=G)
    drum(g, ROSE, 180, -110, 30, 86, 20, steps=6, top_r=26)              # an urn, trailing weed
    g.sphere(LAV, (180, -110, 96), 22, steps=(3, 6))
    who(sc, r, -72, -80, 231, yaw=70, suits=S5, legs='stride', arm_l='hold', arm_r='hold', hold_r=h_basket(SHELL, PINK), detail=0)
    who(sc, r, 70, -70, 300, yaw=-6, suits=S5, wear='dress', arm_r='wave', swing_r=20, rate=0.7)
    who(sc, r, -170, -80, 600, yaw=-10, suits=S5, arm_r='up', hold_r=h_flag(PINK, GOLD_O, 56), swing_r=18, rate=0.8, react=True)
    who(sc, r, -108, -84, 600, yaw=0, suits=S5, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)
    who(sc, r, -50, -78, 600, yaw=8, suits=S5, wear='dress', arm_l='wave', swing_l=22, rate=0.9, react=True)


# ------------------------------------------------------------------------------------------ s1d6, Nereida: the Bubble Works
def d6_blowdeck(sc):
    """P. Glassblowers on a copper deck: two turn half-blown bubbles on long pipes, a carrier brings floats."""
    r = random.Random(661)
    g = sc.still()
    drum(g, COPPER, -120, 0, -FOOT, 220, 40, steps=8)
    drum(g, COPPER, 120, 0, -FOOT, 220, 40, steps=8)
    block(g, INK, 0, 0, 220, 236, 424, 284)
    block(g, COPPER, 0, 0, 236, 250, 416, 276)
    for k in range(5):
        g.sphere(GLASS, (-180 + k * 90, -128, 216), 18, steps=(3, 6))    # a hem of glass beads
    drum(g, COPPER, 140, 76, 250, 420, 60, steps=8)                      # the furnace
    drum(g, INK, 140, 76, 420, 440, 64, steps=8)
    drum(g, COPPER, 140, 76, 440, 520, 50, steps=8, top_r=16)
    drum(g, COPPER, 140, 76, 520, 640, 15, steps=6)
    g.box(INK, (140, 14, 330), (64, 8, 60))
    g.box(MAG, (140, 10, 326), (44, 6, 40), glow=G)                      # its mouth: a steady magenta light
    bubbles(sc, 140, 76, 680, n=4, gap=48, r0=9, ph=0.5)
    g = sc.still()
    block(g, INK, -150, 100, 250, 310, 110, 50)                          # the mould bench: three bottle shapes
    for k in range(3):
        drum(g, GLASS, -184 + k * 34, 100, 310, 350, 12, steps=6, top_r=5)
    tilt_box(g, COPPER, (-204, 120, 250), (-204, 120, 520), 10, 10)      # a pipe looping over the back
    tilt_box(g, COPPER, (-204, 120, 520), (80, 120, 560), 10, 10)
    who(sc, r, -160, -64, 250, yaw=80, suits=S6, arm_l='hold', arm_r='fwd', hold_r=h_pipe(130), turn=5, rate=0.3)
    who(sc, r, 60, -30, 250, yaw=-110, suits=S6, arm_l='hold', arm_r='fwd', hold_r=h_pipe(96, r=30), turn=5, rate=0.3)
    who(sc, r, -30, 70, 250, yaw=0, suits=S6, arm_l='carry', arm_r='carry', hold_r=h_yoke(), rock=3, rate=0.5, detail=0)
    who(sc, r, 150, -84, 250, yaw=10, suits=S6, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)


def d6_floatstairs(sc):
    """M. Two bubble houses on a copper stem, a round deck at each (3 m and 6 m): carriers and the house's own wave."""
    r = random.Random(662)
    g = sc.still()
    drum(g, COPPER, 0, 30, -FOOT, 860, 30, steps=8)
    drum(g, COPPER, -80, 0, 286, 300, 136, steps=10)                     # the lower deck
    drum(g, INK, -80, 0, 276, 288, 130, steps=10)
    drum(g, COPPER, 80, 0, 586, 600, 132, steps=10)                      # the upper deck
    drum(g, INK, 80, 0, 576, 588, 126, steps=10)
    tilt_box(g, COPPER, (-150, 30, -FOOT), (-150, 30, 280), 12, 12)
    tilt_box(g, COPPER, (0, 30, 420), (160, 30, 580), 12, 12)
    g.sphere(GLASS, (-60, 60, 400), 78, steps=(5, 8))                    # the houses
    g.sphere(GLASS, (90, 60, 690), 70, steps=(5, 8))
    g.sphere(GLASS, (0, 30, 900), 60, steps=(5, 8))
    for (x, y, z, rr) in ((-60, 60, 400, 80), (90, 60, 690, 72), (0, 30, 900, 62)):
        drum(g, COPPER, x, y, z - 8, z + 8, rr, steps=8)                 # their collars
    g.box(MAG, (-60, -18, 410), (30, 6, 30), glow=G)
    g.box(MAG, (90, -10, 700), (26, 6, 26), glow=G)
    drum(g, COPPER, 0, 30, 950, 990, 22, steps=6, top_r=8)
    bubbles(sc, 0, 30, 1020, n=3, gap=44, r0=8, ph=0.3)
    g = sc.still()
    for (cx, z) in ((-80, 300), (80, 600)):
        for k in range(5):
            a = math.radians(200 + k * 35)
            block(g, INK, cx + math.cos(a) * 126, math.sin(a) * 126, z, z + 60, 6, 6)
    who(sc, r, -150, -50, 300, yaw=-20, suits=S6, arm_l='carry', arm_r='carry', hold_r=h_yoke(), rock=3, rate=0.5)
    who(sc, r, -50, -90, 300, yaw=0, suits=S6, arm_r='wave', swing_r=22, rate=0.8, react=True)
    who(sc, r, 30, -70, 600, yaw=-10, suits=S6, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True)
    who(sc, r, 120, -84, 600, yaw=10, suits=S6, arm_l='wave', swing_l=24, bob=5, rate=0.9, react=True, **KID)


def d6_bubbleterrace(sc):
    """T. A terrace 6.2 m up a copper trunk: children chase bubbles under a glass canopy, a parent looks on."""
    r = random.Random(663)
    g = sc.still()
    drum(g, COPPER, 0, 20, -FOOT, 600, 56, steps=8, top_r=44)
    for (x, y) in ((-180, -100), (180, -100), (0, 130)):
        tilt_box(g, COPPER, (x, y, -FOOT), (x * 0.5, y * 0.5 + 10, 590), 12, 12)
    for z in (140, 380):
        drum(g, INK, 0, 20, z, z + 16, 62, steps=8)
    block(g, INK, 0, 0, 596, 610, 424, 284)
    block(g, COPPER, 0, 0, 610, 620, 416, 276)
    for k in range(6):                                                   # a balustrade of pale glass
        block(g, GLASS, -175 + k * 70, -134, 620, 672, 56, 6)
    block(g, COPPER, 0, -134, 672, 680, 420, 8)
    for (x, y) in ((-170, 110), (170, 110), (-170, -100), (170, -100)):
        block(g, COPPER, x, y, 620, 900, 7, 7)
    g.sphere(GLASS, (0, 5, 900), 190, scale=(1, 0.7, 0.34), steps=(4, 10))
    block(g, COPPER, 0, 5, 890, 902, 396, 276)
    g.sphere(MAG, (0, 5, 880), 16, steps=(3, 6), glow=G)
    for k, (x, y, z) in enumerate(((-150, -60, 800), (-60, 10, 850), (20, -80, 790), (110, -20, 840), (160, -90, 770))):
        b = sc.part(P.motion(bob=-12.0, rate=0.3, phase=0.11 + k * 0.19))
        b.sphere(GLASS, (x, y, z), 12 + (k % 3) * 4, steps=(3, 6))
    who(sc, r, -130, -70, 620, yaw=-20, suits=S6, arm_l='up', arm_r='up', bob=8, rate=1.0, react=True, **KID)
    who(sc, r, -30, -60, 620, yaw=10, suits=S6, arm_r='up', arm_l='out', bob=7, rate=1.1, react=True, **KID)
    who(sc, r, 60, -84, 620, yaw=0, suits=S6, arm_l='vee', arm_r='vee', bob=6, rate=0.9, react=True, **KID)
    who(sc, r, 150, -64, 620, yaw=24, suits=S6, arm_l='hip', arm_r='wave', swing_r=18, rate=0.7)


def d6_blowpipe(sc):
    """M. The blowpipe tower: the master works a great pipe 6.2 m up, a half-blown bubble swelling over the roof."""
    r = random.Random(664)
    g = sc.still()
    for (x, y) in ((-170, -100), (170, -100), (0, 126)):
        tilt_box(g, COPPER, (x, y, -FOOT), (x * 0.82, y * 0.82, 620), 16, 16)
    for z in (150, 450):
        tilt_box(g, INK, (-160, -94, z), (160, -94, z), 8, 8)
        tilt_box(g, INK, (-160, -94, z), (0, 118, z), 8, 8)
        tilt_box(g, INK, (160, -94, z), (0, 118, z), 8, 8)
    block(g, COPPER, 0, 0, 290, 304, 380, 260)                           # the float store, 3 m up
    block(g, INK, 0, 60, 304, 420, 260, 10)                              # a rack of pale glass floats (never orange)
    for k in range(6):
        g.sphere(GLASS, (-100 + (k % 3) * 100, 46, 340 + (k // 3) * 56), 22, steps=(3, 6))
    block(g, INK, 0, 0, 606, 620, 420, 284)
    block(g, COPPER, 0, 0, 620, 628, 412, 276)
    rail(g, COPPER, -204, 204, -136, 628, h=60, n=5)
    drum(g, COPPER, 40, 40, 628, 700, 34, steps=8, top_r=22)             # the pipe's foot
    drum(g, STEEL, 40, 40, 700, 960, 7, steps=6)
    b = sc.part(P.motion(bob=-7.0, rate=0.2, phase=0.3))
    b.sphere(GLASS, (40, 40, 1030), 76, scale=(1, 1, 1.1), steps=(5, 8))
    g = sc.still()
    drum(g, COPPER, 40, 40, 950, 966, 20, steps=6)
    g.box(MAG, (-150, 110, 700), (60, 6, 110), glow=G)                   # a lit glass panel behind, magenta
    block(g, INK, -150, 114, 628, 770, 70, 6)
    who(sc, r, -20, -10, 628, yaw=60, suits=S6, arm_l='up', arm_r='up', rock=3, rate=0.3, build='stocky')
    who(sc, r, 160, -50, 628, yaw=-84, suits=S6, arm_l='hold', arm_r='fwd', hold_r=h_pipe(80, r=20), turn=5, rate=0.3)
    who(sc, r, -130, -84, 628, yaw=-8, suits=S6, arm_r='wave', swing_r=22, rate=0.8, react=True)
    who(sc, r, -60, -90, 628, yaw=6, suits=S6, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)
    who(sc, r, 110, -70, 304, yaw=10, suits=S6, arm_l='carry', arm_r='carry', hold_r=h_yoke(), rock=3, rate=0.5, detail=0)


# ------------------------------------------------------------------------------------------ s1d7, Nereida: Lamp Row
def d7_keepers(sc):
    """T. A banded lamp tower's gallery 6.2 m up: keepers polish the lamp's glass, one waves from the rail."""
    r = random.Random(671)
    g = sc.still()
    drum(g, WHITE, 0, 0, -FOOT, 600, 104, steps=10, top_r=92)
    for z in (60, 280, 480):
        drum(g, RED, 0, 0, z, z + 100, 106 - z * 0.019, steps=10, top_r=104 - z * 0.019)   # red paint, in broad bands
    g.box(INK2, (0, -100, 20), (50, 8, 110))                             # the door
    g.box(CREAM, (0, -98, 420), (26, 8, 34), glow=G)
    for k in range(4):
        a = math.radians(45 + k * 90)
        tilt_box(g, INK2, (math.cos(a) * 92, math.sin(a) * 92, 520), (math.cos(a) * 132, math.sin(a) * 132, 600), 8, 8)
    drum(g, INK2, 0, 0, 600, 618, 142, steps=10)                         # the gallery
    for k in range(10):
        a = math.radians(k * 36)
        block(g, INK2, math.cos(a) * 136, math.sin(a) * 136, 618, 680, 5, 5)
    drum(g, INK2, 0, 0, 678, 684, 140, steps=10)
    drum(g, WHITE, 0, 0, 682, 687, 132, steps=10)
    drum(g, CREAM, 0, 0, 618, 800, 60, steps=8, glow=G)                  # the lamp room: steady, behind its bars
    for k in range(4):
        a = math.radians(k * 90)
        block(g, INK2, math.cos(a) * 60, math.sin(a) * 60, 618, 800, 7, 7)
    drum(g, INK2, 0, 0, 800, 900, 96, steps=8, top_r=8)
    g.sphere(RED, (0, 0, 912), 13, steps=(3, 6))
    who(sc, r, -100, -30, 618, yaw=-80, suits=S7, wear='coat', arm_r='up', hold_r=h_cloth(), swing_r=16, axis_r='x', rate=0.7)
    who(sc, r, 100, -30, 618, yaw=80, suits=S7, wear='coat', arm_l='up', hold_l=h_cloth(), swing_l=16, axis_l='x', rate=0.6)
    who(sc, r, -40, -104, 618, yaw=-6, suits=S7, arm_r='wave', swing_r=22, rate=0.8, react=True)
    who(sc, r, 46, -102, 618, yaw=8, suits=S7, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)


def d7_pilots(sc):
    """P. Pilots on a stone pier: two with spyglasses, a signal mast with balls and cones hoisted, a child on a bollard."""
    r = random.Random(672)
    g = sc.still()
    block(g, WHITE, 0, 0, -FOOT, 230, 424, 284)
    block(g, RED, 0, -143, 130, 210, 424, 4)                             # a red band along the pier's face
    block(g, WHITE, 60, 80, 230, 310, 300, 120)
    block(g, TEAL, 60, 18, 296, 310, 300, 6)
    for k in range(4):                                                   # bollards and their chain
        drum(g, INK2, -190 + k * 100, -124, 230, 276, 12, steps=6, top_r=15)
        if k:
            tilt_box(g, INK2, (-290 + k * 100, -124, 262), (-190 + k * 100, -124, 262), 5, 5)
    block(g, INK2, -150, 90, 230, 700, 9, 9)                             # the signal mast
    g.box(INK2, (-150, 90, 600), (170, 6, 6))
    for (dx, z, c, cone) in ((-70, 540, WHITE, False), (-70, 470, TEAL, True), (70, 540, TEAL, False), (70, 460, WHITE, True)):
        if cone:
            drum(g, c, -150 + dx, 90, z - 22, z + 26, 22, steps=6, top_r=3)
        else:
            g.sphere(c, (-150 + dx, 90, z), 21, steps=(4, 6))
        g.box(INK2, (-150 + dx, 90, z + 44), (4, 4, 40))
    for k in range(3):                                                   # a rack of oil jars
        drum(g, CREAM, 150 + k * 28, 124, 310, 344, 11, steps=6, top_r=7)
    for k in range(3):                                                   # the great spyglass on its tripod
        a = math.radians(30 + k * 120)
        tilt_box(g, INK2, (120 + math.cos(a) * 30, 60 + math.sin(a) * 30, 310), (120, 60, 410), 5, 5)
    tilt_box(g, '#c9b44a', (120, 90, 400), (120, 10, 440), 14, 14)
    lamp(g, 200, -124, 230, h=150, col=CREAM, post=INK2)
    who(sc, r, -110, -30, 230, yaw=-16, suits=S7, wear='coat', arm_l='face', arm_r='face', hold_r=h_glass(), turn=14, rate=0.25)
    who(sc, r, 10, 70, 310, yaw=4, suits=S7, wear='coat', arm_l='face', arm_r='face', hold_r=h_glass(), turn=12, rate=0.2, hat_col=HELM, band=RED)
    who(sc, r, -20, -70, 230, yaw=10, suits=S7, arm_r='point', arm_l='hip')
    who(sc, r, 110, -118, 276, yaw=6, suits=S7, arm_l='vee', arm_r='vee', rock=5, rate=0.9, react=True, **KID)


def d7_belltower(sc):
    """M. A bell frame: the ringer hauls the rope 3 m up, the great bell swings slowly, a family watches from 6 m."""
    r = random.Random(673)
    g = sc.still()
    block(g, WHITE, 0, 0, -FOOT, 120, 424, 284)
    block(g, RED, 0, -143, 20, 100, 424, 4)
    stilts(g, INK2, ((-196, -126), (196, -126), (-196, 126), (196, 126), (0, 126), (0, -126)), 880, 14)
    block(g, WHITE, 0, 0, 290, 304, 410, 270)
    block(g, WHITE, -100, 0, 590, 604, 212, 270)
    rail(g, INK2, -196, 196, -130, 304, h=58, n=5)
    rail(g, INK2, -196, 0, -130, 604, h=58, n=3)
    xbrace(g, INK2, -196, 0, 126, 320, 580, 6)
    block(g, INK2, 100, 0, 760, 774, 200, 14)                            # the beam the bell hangs from
    b = sc.part(P.motion(swing=13, pivot=(100, 0, 760), axis=0.0, rate=0.35, phase=0.2))
    b.cyl('#c9b44a', (100, 0, 610), 66, 110, steps=8, top_r=34)
    b.cyl('#c9b44a', (100, 0, 720), 34, 30, steps=8, top_r=12)
    b.box(INK2, (100, 0, 600), (12, 12, 40))
    g = sc.still()
    tilt_box(g, SHELL, (150, -40, 740), (150, -60, 420), 5, 5)           # the rope
    for sy in (-1, 1):                                                   # a steep roof
        tilt_box(g, INK2, (0, sy * 140, 876), (0, 0, 990), 436, 10)
    g.sphere(RED, (0, 0, 1000), 13, steps=(3, 6))
    for k in range(3):                                                   # oil jars under the family's floor
        drum(g, CREAM, -170 + k * 30, 90, 304, 344, 12, steps=6, top_r=8)
    lamp(g, -196, -126, 120, h=120, col=CREAM, post=INK2, r=14)
    who(sc, r, 150, -64, 304, yaw=14, suits=S7, wear='coat', arm_l='up', arm_r='up', bob=5, rate=0.35, build='stocky')
    who(sc, r, 40, -70, 304, yaw=-10, suits=S7, arm_l='hip', arm_r='point', detail=0)
    who(sc, r, -160, -80, 604, yaw=-10, suits=S7, arm_r='wave', swing_r=22, rate=0.8, react=True)
    who(sc, r, -96, -84, 604, yaw=0, suits=S7, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)
    who(sc, r, -36, -78, 604, yaw=10, suits=S7, wear='dress', arm_l='wave', swing_l=18, rate=0.7)


def d7_pilothouse(sc):
    """M. A pilot house on bracket legs, its gallery 6 m up: pilots at the rail, signal shapes hoisted, a striped buoy below."""
    r = random.Random(674)
    g = sc.still()
    stilts(g, INK2, ((-170, -100), (170, -100), (-170, 100), (170, 100)), 600, 16)
    xbrace(g, INK2, -170, 170, 100, 0, 560, 7)
    for sx in (-1, 1):
        tilt_box(g, INK2, (sx * 170, -100, 480), (sx * 204, -136, 596), 8, 8)
    block(g, WHITE, 0, 0, 600, 616, 424, 284)
    block(g, RED, 0, -143, 598, 614, 424, 4)
    rail(g, INK2, -206, 206, -136, 616, h=60, n=5)
    block(g, WHITE, 30, 66, 616, 800, 250, 140)                          # the house
    block(g, RED, 30, 66, 690, 730, 254, 144)                            # a red band of paint
    tilt_box(g, INK2, (30, -20, 800), (30, 66, 880), 270, 10)            # its roof
    tilt_box(g, INK2, (30, 144, 800), (30, 66, 880), 270, 10)
    drum(g, WHITE, 30, 66, 800, 870, 60, steps=6, top_r=4)               # the round gable, from the front
    g.box(INK2, (-20, -6, 720), (44, 6, 60))
    g.box(CREAM, (70, -6, 740), (50, 6, 40), glow=G)                     # a lit window
    block(g, INK2, -150, 90, 616, 980, 8, 8)                             # the signal mast
    g.box(INK2, (-150, 90, 920), (110, 6, 6))
    for (dx, z, c, cone) in ((-46, 870, TEAL, False), (-46, 810, WHITE, True), (46, 870, WHITE, False)):
        if cone:
            drum(g, c, -150 + dx, 90, z - 20, z + 24, 20, steps=6, top_r=3)
        else:
            g.sphere(c, (-150 + dx, 90, z), 19, steps=(4, 6))
        g.box(INK2, (-150 + dx, 90, z + 34), (4, 4, 30))
    for k, c in enumerate((RED, WHITE, RED, WHITE)):                     # a marker buoy stood on end between the legs
        drum(g, c, 60, 20, -40 + k * 70, 30 + k * 70, 58 - k * 4, steps=8, top_r=54 - k * 4)
    drum(g, INK2, 60, 20, 240, 330, 10, steps=6)
    g.sphere(WHITE, (60, 20, 340), 16, steps=(3, 6))
    lamp(g, 200, -128, 616, h=120, col=CREAM, post=INK2, r=14)
    who(sc, r, -150, -38, 616, yaw=-16, suits=S7, wear='coat', arm_l='face', arm_r='face', hold_r=h_glass(), turn=14, rate=0.25)
    who(sc, r, -60, -84, 616, yaw=-4, suits=S7, arm_r='point', arm_l='hip')
    who(sc, r, 30, -84, 616, yaw=6, suits=S7, arm_r='wave', swing_r=22, rate=0.8, react=True)
    who(sc, r, 120, -80, 616, yaw=14, suits=S7, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)


# ------------------------------------------------------------------------------------------ s1d8, Nereida: the Crown
def d8_rotunda(sc):
    """M. The music rotunda: a band of three under gold columns 3 m up, children with streamers on its roof at 5.9 m."""
    r = random.Random(681)
    g = sc.still()
    drum(g, VIOLET, 0, 0, -FOOT, 290, 140, steps=10)
    for k in range(5):
        a = math.radians(198 + k * 36)
        g.box(LAV, (math.cos(a) * 139, math.sin(a) * 139, 150), (50, 8, 170), rot=(0, math.degrees(a) + 90, 0))   # arches round the foot
    drum(g, MARBLE, 0, 0, 290, 304, 144, steps=10)
    for k in range(6):
        a = math.radians(30 + k * 60)
        drum(g, GOLD, math.cos(a) * 128, math.sin(a) * 128, 304, 570, 8, steps=6)
    drum(g, GOLD, 0, 0, 570, 590, 144, steps=10)
    drum(g, MARBLE, 0, 0, 590, 598, 136, steps=10)
    for k in range(6):
        a = math.radians(180 + k * 36)
        block(g, GOLD, math.cos(a) * 136, math.sin(a) * 136, 598, 656, 5, 5)
    drum(g, GOLD, 0, 0, 654, 660, 139, steps=10)
    drum(g, MARBLE, 0, 0, 658, 663, 132, steps=10)
    block(g, GOLD, 0, 60, 598, 800, 8, 8)                                # the finial: an onion and a pennant
    g.sphere(GOLD, (0, 60, 820), 34, scale=(1, 1, 1.25), steps=(4, 8))
    drum(g, GOLD, 0, 60, 850, 900, 8, steps=6, top_r=3)
    pennant(sc, PINK, 0, 60, 900, h=90, w=80, fh=24, pole=GOLD, ph=0.2)
    who(sc, r, -76, -20, 304, yaw=-14, suits=S8, arm_l='face', arm_r='face', hold_r=h_horn(), rock=4, rate=0.6)
    who(sc, r, 0, -64, 304, yaw=0, suits=S8, arm_l='carry', arm_r='up', swing_r=20, axis_r='x', rate=1.0)
    g = sc.still()
    drum(g, PINK, 0, -112, 404, 436, 26, steps=8)                        # the drum he beats
    drum(g, MARBLE, 0, -112, 436, 441, 27, steps=8)
    block(g, GOLD, 0, -112, 304, 404, 6, 6)
    who(sc, r, 80, -56, 304, yaw=16, suits=S8, wear='dress', arm_l='hold', arm_r='hold', hold_r=h_basket(VIOLET, GOLD), rock=4, rate=0.5, detail=0)
    who(sc, r, -60, -76, 598, yaw=-10, suits=S8, arm_r='up', hold_r=h_flag(PINK, GOLD, 60), swing_r=22, rate=0.8, react=True, **KID)
    who(sc, r, 50, -80, 598, yaw=10, suits=S8, arm_l='up', hold_l=h_flag(LAV, GOLD, 60), swing_l=22, rate=0.9, react=True, **KID)


def d8_terrace(sc):
    """P. A garden terrace: courtiers in gowns, a gardener up a ladder clipping a topiary seahorse of gold wire."""
    r = random.Random(682)
    g = sc.still()
    block(g, VIOLET, 0, 0, -FOOT, 230, 424, 284)
    for k in range(4):
        g.box(LAV, (-150 + k * 100, -143, 100), (56, 5, 150))            # arches along the arcade's face
        g.box(MARBLE, (-150 + k * 100, -145, 184), (64, 5, 14))
    block(g, MARBLE, 0, 0, 230, 246, 430, 290)
    block(g, VIOLET, 50, 96, 246, 320, 320, 96)
    block(g, MARBLE, 50, 96, 320, 330, 326, 102)
    for k in range(5):                                                   # balusters
        drum(g, MARBLE, -196 + k * 98, -134, 246, 296, 8, steps=6)
    block(g, GOLD, 0, -134, 296, 304, 424, 8)
    block(g, PINK, -150, 100, 246, 300, 100, 70)                         # a clipped hedge in flower (never green)
    g.sphere(PINK, (-150, 100, 320), 44, scale=(1, 0.8, 0.6), steps=(3, 6))
    pts = ((120, 96, 330), (120, 96, 420), (150, 96, 480), (120, 96, 540), (96, 96, 600), (130, 96, 640))   # the seahorse: a frame of gold wire
    for a, b in zip(pts[:-1], pts[1:]):
        tilt_box(g, GOLD, a, b, 14, 14)
    tilt_box(g, GOLD, (130, 96, 640), (176, 96, 620), 12, 12)
    tilt_box(g, GOLD, (120, 96, 420), (96, 96, 370), 10, 10)
    g.sphere(LAV, (150, 96, 480), 26, steps=(3, 6))
    ladder(g, MARBLE, 40, 80, 330, 520, w=36, rungs=5)
    block(g, MARBLE, 40, 60, 420, 430, 50, 40)
    drum(g, MARBLE, 196, -110, 246, 300, 16, steps=6, top_r=24)          # an urn, trailing weed
    g.sphere(LAV, (196, -110, 312), 22, steps=(3, 6))
    who(sc, r, 40, 56, 430, yaw=120, suits=S8, arm_r='fwd', hold_r=P.hold_tool(STEEL, GOLD, 30), swing_r=10, axis_r='x', rate=0.7, s=1.0)
    who(sc, r, -110, -60, 246, yaw=-12, suit=VIOLET, wear='dress', band=GOLD, arm_l='hip', arm_r='wave', swing_r=16, rate=0.6)
    who(sc, r, -20, -70, 246, yaw=8, suit=PINK, wear='dress', band=GOLD, arm_l='hold', arm_r='hold', hold_r=h_cloth(LAV), rock=3, rate=0.5)
    who(sc, r, 96, -76, 246, yaw=14, suits=S8, arm_r='up', hold_r=h_flag(GOLD, MARBLE, 56), swing_r=22, rate=0.9, react=True, **KID)


def d8_turret(sc):
    """T. A gate lodge's violet turret: guards with pennant staffs and children with streamers on its balcony 6.2 m up."""
    r = random.Random(683)
    g = sc.still()
    drum(g, VIOLET, 0, 0, -FOOT, 600, 100, steps=10, top_r=92)
    for z in (120, 360):
        drum(g, MARBLE, 0, 0, z, z + 22, 102 - z * 0.012, steps=10)
    g.box(LAV, (0, -97, 30), (56, 8, 130))                               # the door
    g.box(CREAM, (0, -94, 470), (28, 8, 44), glow=G)
    for k in range(4):
        a = math.radians(45 + k * 90)
        tilt_box(g, VIOLET, (math.cos(a) * 90, math.sin(a) * 90, 520), (math.cos(a) * 130, math.sin(a) * 130, 600), 9, 9)
    drum(g, GOLD, 0, 0, 600, 618, 143, steps=10)
    drum(g, MARBLE, 0, 0, 618, 622, 136, steps=10)
    for k in range(10):
        a = math.radians(k * 36)
        block(g, MARBLE, math.cos(a) * 136, math.sin(a) * 136, 622, 680, 5, 5)
    drum(g, GOLD, 0, 0, 678, 684, 140, steps=10)
    drum(g, MARBLE, 0, 0, 682, 687, 132, steps=10)
    drum(g, VIOLET, 0, 20, 622, 840, 58, steps=8)
    drum(g, MARBLE, 0, 20, 840, 856, 70, steps=8)
    g.sphere(GOLD, (0, 20, 890), 64, scale=(1, 1, 1.2), steps=(5, 8))    # the onion dome
    drum(g, GOLD, 0, 20, 950, 1030, 12, steps=6, top_r=3)
    g.box(CREAM, (0, -40, 740), (24, 6, 50), glow=G)
    pennant(sc, LAV, 0, 20, 1030, h=80, w=90, fh=24, pole=GOLD, ph=0.6)
    for (x, yw, c) in ((-110, -30, LAV), (110, 30, PINK)):
        t = who(sc, r, x, -56, 622, yaw=yw, suit=GOLD, band=VIOLET, pack=VIOLET, build='avg', arm_r='up', hold_r=h_lance(c, MARBLE))
        plume(sc, c, x, -56, t)
    who(sc, r, -40, -106, 622, yaw=-6, suits=S8, arm_r='up', hold_r=h_flag(PINK, GOLD, 60), swing_r=22, rate=0.9, react=True, **KID)
    who(sc, r, 40, -104, 622, yaw=8, suits=S8, arm_l='vee', arm_r='vee', bob=6, rate=1.0, react=True, **KID)


def d8_archbridge(sc):
    """M. A garden bridge on a tall round arch, standing along the road: courtiers promenade 6 m up between gold lamp trees."""
    r = random.Random(684)
    g = sc.still()
    for sx in (-1, 1):
        block(g, VIOLET, sx * 168, 0, -FOOT, 560, 88, 230)
        block(g, MARBLE, sx * 168, 0, 300, 322, 94, 236)
        drum(g, LAV, sx * 190, -100, 616, 760, 20, steps=6, top_r=3)     # crystal pinnacles
    for k in range(6):                                                   # the arch's stones
        a0, a1 = math.radians(k * 30), math.radians(k * 30 + 30)
        tilt_box(g, MARBLE if k % 2 == 0 else LAV, (math.cos(a0) * 132, 0, 420 + math.sin(a0) * 132), (math.cos(a1) * 132, 0, 420 + math.sin(a1) * 132), 232, 22)
    block(g, VIOLET, 0, 0, 560, 600, 424, 236)
    block(g, MARBLE, 0, 0, 600, 616, 430, 284)
    block(g, GOLD, 0, -143, 596, 612, 430, 4)
    for k in range(6):
        drum(g, MARBLE, -200 + k * 80, -134, 616, 666, 8, steps=6)
    block(g, GOLD, 0, -134, 666, 674, 424, 8)
    for sx in (-1, 1):                                                   # gold lamp trees, three globes each
        block(g, GOLD, sx * 190, 110, 616, 800, 7, 7)
        g.box(GOLD, (sx * 190, 110, 780), (70, 6, 6))
        for dx in (-34, 0, 34):
            g.sphere(CREAM, (sx * 190 + dx, 110, 806 + (16 if dx == 0 else 0)), 13, steps=(3, 6), glow=G)
    block(g, PINK, 0, 110, 616, 664, 220, 50)                            # a flowering hedge along the back (never green)
    pennant(sc, PINK, 0, 110, 664, h=170, w=84, fh=26, pole=GOLD, ph=0.4)
    who(sc, r, -130, -70, 616, yaw=-60, suit=VIOLET, wear='dress', band=GOLD, legs='stride', arm_l='hip', arm_r='hold', hold_r=h_cloth(PINK), rock=3, rate=0.4)
    who(sc, r, -40, -80, 616, yaw=-4, suit=LAV, wear='dress', band=GOLD, arm_r='wave', swing_r=18, rate=0.7, react=True)
    who(sc, r, 40, -84, 616, yaw=6, suits=S8, arm_r='up', hold_r=h_flag(GOLD, MARBLE, 56), swing_r=22, rate=0.9, react=True, **KID)
    t = who(sc, r, 130, -70, 616, yaw=14, suit=GOLD, band=VIOLET, pack=VIOLET, build='avg', arm_r='up', hold_r=h_lance(LAV, MARBLE))
    plume(sc, LAV, 130, -70, t)


JOBS = [
    ('SM_S1PEOPLE_D1_Gardeners', d1_gardeners, SAVE), ('SM_S1PEOPLE_D1_PearlStair', d1_pearlstair, SAVE), ('SM_S1PEOPLE_D1_Conch', d1_conch, SAVE),
    ('SM_S1PEOPLE_D2_StiltDeck', d2_stiltdeck, SAVE), ('SM_S1PEOPLE_D2_CuttersMast', d2_cuttersmast, SAVE),
    ('SM_S1PEOPLE_D3_RoofDeck', d3_roofdeck, SAVE), ('SM_S1PEOPLE_D3_DiverClass', d3_diverclass, SAVE),
    ('SM_S1PEOPLE_D4_HullDeck', d4_hulldeck, SAVE), ('SM_S1PEOPLE_D4_Forge', d4_forge, SAVE), ('SM_S1PEOPLE_D4_BridgeHouse', d4_bridgehouse, SAVE),
    ('SM_S1PEOPLE_D5_TempleSteps', d5_templesteps, SAVE), ('SM_S1PEOPLE_D5_Guards', d5_guards, SAVE), ('SM_S1PEOPLE_D5_Arcade', d5_arcade, SAVE), ('SM_S1PEOPLE_D5_GrandStair', d5_grandstair, SAVE),
    ('SM_S1PEOPLE_D6_BlowDeck', d6_blowdeck, SAVE), ('SM_S1PEOPLE_D6_FloatStairs', d6_floatstairs, SAVE), ('SM_S1PEOPLE_D6_BubbleTerrace', d6_bubbleterrace, SAVE),
    ('SM_S1PEOPLE_D6_Blowpipe', d6_blowpipe, SAVE),
    ('SM_S1PEOPLE_D7_Keepers', d7_keepers, SAVE), ('SM_S1PEOPLE_D7_Pilots', d7_pilots, SAVE), ('SM_S1PEOPLE_D7_BellTower', d7_belltower, SAVE), ('SM_S1PEOPLE_D7_PilotHouse', d7_pilothouse, SAVE),
    ('SM_S1PEOPLE_D8_Rotunda', d8_rotunda, SAVE), ('SM_S1PEOPLE_D8_Terrace', d8_terrace, SAVE), ('SM_S1PEOPLE_D8_Turret', d8_turret, SAVE), ('SM_S1PEOPLE_D8_ArchBridge', d8_archbridge, SAVE),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
