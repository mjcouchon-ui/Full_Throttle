"""FULL THROTTLE (Unreal) the population of level 3, the Volcano: pass 3, builder p3b, "scenes".

Ten story groups that could only be on Mount Kaldera, one mesh a scene (/Game/People/Volcano/SM_P3B_<Name>):
  Wedding    a couple marrying under a flower arch at the lava's edge: guests throwing petals, a photographer, music
  Kitchen    the steam kitchen: cooks boiling eggs over a vent, a queue with bowls, two eating on a bench
  Shrine     an offering to the mountain: a cairn with streamers, drummers, a ring of dancers going round, an elder
  Rescue     the mountain rescue's drill (cyan jackets, no vehicle, nothing red): a volunteer carried on a litter chair,
             one hoisted on a winch over the back, the search dog, the coordinator's flag
  Sample     the heat-suit team taking samples: long ladles over the back, a quench trough steaming, a tracked rover
  School     the village school leaving in pairs behind the teacher's paddle; a water table; the school's dog
  HotSpring  a hot tub of spring water on the deck: bathers up to the shoulders, steam, an attendant, tea
  Tour       the volcano tour: a guide with a flag, tourists with cameras, two painters at easels, a souvenir parasol
  Band       the village band playing the leavers off: bass drum, tuba, trumpets, a conductor, two couples dancing
  Briefing   the survey's press briefing: a speaker at the hazard board, reporters, a camera on its tripod

How they stand (FTDressL3.inl, FTPeopleL3.inl): the barrier is 1.6 m, the land 0.75 m under the road, the main cast's
trestles 1.8 m up at 28 m. These scenes stand BEHIND those, on open stages 2.6 m up (thin legs: the lava channel
behind stays in sight), 32.5 m out on the left and 38.4 m on the right. All are TALL (24 m or more).
Every colour carries a little steady light (0.035, as the main cast's near pieces: the level is dark); the one real
glow is the lava in the samplers' ladles. Nothing blinks.

Run in the editor:  import ft_people_volcano_p3b as K; K.run()        K.run(['SM_P3B_Wedding'])
Placed by the block C:/ftwork/p3/L3b.inl (pasted by the integrator at the end of FTPeopleL3.inl).
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/Volcano'
DECK = 260.0
PLANK, PLANK_D, STEEL, STEEL_D = '#e8d8b8', '#c9a070', '#8f9bb3', '#5a5870'
WHITE, DARK, YEL, LIME, PINK, CYAN, ORANGE, VIOLET = '#fff6ea', '#22202a', '#ffc93c', '#b8ff5a', '#ff5fa2', '#2ee6ff', '#ff7a3c', '#8f6bff'
WOOD, BASALT, WATER, SILVER = '#c9a070', '#3a3848', '#9fe8ff', '#d7e3f0'
CLOTH = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a')
BRIGHT = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a', '#ff7a3c')


def tv(length, speed):
    """A travel for a part drawn by hand (figure() takes (length, cm/s) itself)."""
    return (float(length), float(speed) / float(length))


def face(x, y, tx, ty):
    """The yaw that turns a figure at (x, y) towards (tx, ty)."""
    return math.degrees(math.atan2(tx - x, -(ty - y)))


# ------------------------------------------------------------------------------------------ what they stand on
def stage(g, L, W=320.0, h=DECK, stripe=(PINK, WHITE), rail=WHITE, legs_every=360.0):
    """An OPEN deck on thin legs (rooted 2.5 m into the ground), a striped fascia along the road side and a two-bar
    rail behind it: people by the road stand BEHIND a rail."""
    g.box(PLANK, (0, 0, h - 8), (L, W, 16))
    n = max(int(L / 200.0), 2)
    for k in range(n):
        g.box(stripe[k % 2], (-L / 2.0 + (k + 0.5) * L / n, -W / 2.0 - 5, h - 20), (L / n, 10, 44))
    g.box(PLANK_D, (0, W / 2.0 + 4, h - 14), (L, 8, 28))
    n = max(int(round(L / legs_every)), 1)
    for k in range(n + 1):
        x = -L / 2.0 + 12 + k * (L - 24) / n
        for sy in (-1, 1):
            g.box(STEEL_D, (x, sy * (W / 2.0 - 16), (h - 16 - 250) / 2.0), (13, 13, h - 16 + 250))
        if k < n and k % 2 == 0:
            x1 = -L / 2.0 + 12 + (k + 1) * (L - 24) / n
            tilt_box(g, STEEL_D, (x, W / 2.0 - 16, 150), (x1, W / 2.0 - 16, h - 30), 9, 9)
        if rail:
            g.box(rail, (x, -W / 2.0 + 8, h + 52), (8, 8, 104))
    if rail:
        g.box(rail, (0, -W / 2.0 + 8, h + 100), (L, 7, 9))
        g.box(rail, (0, -W / 2.0 + 8, h + 56), (L, 6, 7))


def table(g, x, y, z, w=120.0, d=56.0, h=78.0, top=WHITE, leg=STEEL_D):
    g.box(top, (x, y, z + h - 3), (w, d, 6))
    for sx in (-1, 1):
        g.box(leg, (x + sx * (w / 2.0 - 8), y, z + (h - 6) / 2.0), (6, d - 8, h - 6))


def crate(g, x, y, z, w=110.0, d=44.0, h=46.0, col=WOOD):
    g.box(col, (x, y, z + h / 2.0), (w, d, h))


def tripod(g, x, y, z, top=120.0, spread=44.0, col=STEEL_D):
    for a in (90, 210, 330):
        tilt_box(g, col, (x, y, z + top), (x + math.cos(math.radians(a)) * spread, y + math.sin(math.radians(a)) * spread, z), 5, 5)


def steam(sc, x, y, z, n=3, r0=24.0, rate=0.32, col=WHITE):
    """Puffs of steam, one over the other, each floating up and down on its own clock."""
    for k in range(n):
        m = sc.part(P.motion(bob=-(16.0 + 9.0 * k), rate=rate + 0.06 * k, phase=k * 0.31, pivot=(x, y, z)))
        m.sphere(col, (x + (k - 1) * 15, y + (k % 2) * 14, z + 26 + k * 46), r0 + k * 8, steps=(3, 6))


def bunting(sc, g, x0, x1, y, z0, top, cols, n=8):
    """Two poles and a line of flags between them that swings together about its own line."""
    for x in (x0, x1):
        g.box(STEEL_D, (x, y, (z0 + top) / 2.0), (7, 7, top - z0))
    m = sc.part(P.motion(swing=7.0, pivot=((x0 + x1) / 2.0, y, top - 8), axis=0.0, rate=0.6))
    span = x1 - x0
    m.box(STEEL_D, ((x0 + x1) / 2.0, y, top - 8), (span, 4, 5))
    for i in range(n):
        m.box(cols[i % len(cols)], (x0 + (i + 0.5) * span / n, y, top - 30), (span / n * 0.7, 3, 40))


# ------------------------------------------------------------------------------------------ who they are
def folk(r, **fixed):
    d = P.look(r, tops=BRIGHT, wear=r.choice(('trousers', 'dress', 'skirt', 'coat', 'trousers', 'shorts')),
               hat=r.choice((None, None, 'sun', 'cap', 'beanie')), hat_col=r.choice(CLOTH), sleeves=r.choice(('short', 'long')))
    d.update(fixed)
    return d


def child(r, **fixed):
    d = folk(r)
    d.update(build='kid', s=r.uniform(0.62, 0.7))
    d.update(fixed)
    return d


def rescuer(r, **fixed):
    d = P.look(r, tops=(CYAN,), bottoms=('#2b2350', '#3a5fa8'), wear='trousers', sleeves='long', hat='helmet', hat_col=WHITE, band=YEL,
               shoes=DARK)
    d.update(fixed)
    return d


def suited(r, **fixed):
    """The samplers' heat suits: silver head to foot, a dark visor, a cyan band."""
    d = P.look(r, tops=(SILVER,), bottoms=(SILVER,), wear='coat', sleeves='long', hat='helmet', hat_col=SILVER, band=CYAN, shoes=DARK)
    d.update(fixed)
    return d


def scientist(r, **fixed):
    d = P.look(r, tops=(WHITE,), bottoms=('#3a5fa8', '#2b2350', '#5a4a3c'), wear='coat', sleeves='long', hat='hard',
               hat_col=r.choice((PINK, CYAN, VIOLET, '#7dffb0')), band=CYAN)
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_bouquet(cols=(PINK, YEL, WHITE)):
    def f(m, h, side):
        if side > 0:
            m.sphere(cols[0], (h[0] - 20, h[1] - 12, h[2] + 12), 15, steps=(3, 6))
            m.box(cols[1], (h[0] - 20, h[1] - 12, h[2] + 28), (12, 12, 10))
            m.box('#7dffb0', (h[0] - 20, h[1] - 8, h[2] - 6), (5, 5, 24))
    return f


def hold_basket(col=WOOD, fill=PINK):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 4, h[2] - 22), 15, 18, steps=6, top_r=19)
        m.box(fill, (h[0], h[1] - 4, h[2] - 1), (22, 22, 6))
    return f


def hold_bowl(col=WHITE):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 3, h[2] + 4), 8, 9, steps=6, top_r=13)
    return f


def hold_stick(col=WOOD, length=44.0):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 4, h[2] + length / 2.0), (5, 5, length))
    return f


def hold_horn(col=YEL, length=44.0):
    """A trumpet at the mouth, its bell to the front."""
    def f(m, h, side):
        m.cyl(col, (h[0] - side * 10, h[1] - 6, h[2] + 4), 4, length, rot=(0, 0, 90), steps=6, top_r=14)
    return f


def hold_squeeze(col='#ff2d6a', key=WHITE):
    """A squeeze box between the hands."""
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 6, h[2] + 2), (44, 22, 34))
            m.box(key, (h[0] - 24, h[1] - 18, h[2] + 2), (36, 3, 26))
    return f


def hold_paddle(col=LIME, bar=WHITE):
    """A crossing paddle: a round sign on a stick, a plain bar across it (no word)."""
    def f(m, h, side):
        m.box(STEEL, (h[0], h[1], h[2] + 30), (5, 5, 70))
        m.cyl(col, (h[0], h[1] + 4, h[2] + 84), 26, 8, rot=(0, 0, 90), steps=8)
        m.box(bar, (h[0], h[1] - 5, h[2] + 84), (34, 3, 9))
    return f


def hold_ladle(length=210.0, glow=0.5):
    """The samplers' pole: out ahead and a little down, a dark ladle at its tip and a spoonful of lava in it."""
    def f(m, h, side):
        if side < 0:
            return
        a = (h[0] - 24, h[1] + 40, h[2] + 8)
        b = (h[0] - 24, h[1] - length, h[2] - 26)
        tilt_box(m, STEEL, a, b, 5, 5)
        m.cyl(DARK, (b[0], b[1] - 8, b[2] - 12), 15, 14, steps=6)
        m.box(ORANGE, (b[0], b[1] - 8, b[2] + 3), (16, 16, 5), glow=glow)
    return f


def hold_fan(col=PINK):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 4, h[2] + 16), (26, 3, 26))
    return f


def hold_towel(col=WHITE):
    """A towel over the shoulder."""
    def f(m, h, side):
        m.box(col, (side * 26, 0, 118), (22, 40, 8))
        m.box(col, (side * 26, -20, 98), (22, 5, 44))
    return f


def hold_bucket(col=WOOD):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 6, h[2] - 26), 13, 24, steps=6, top_r=16)
    return f


def hold_pointer(col=DARK, length=70.0):
    """A pointer held straight out to the side."""
    def f(m, h, side):
        m.box(col, (h[0] + side * length / 2.0, h[1], h[2]), (length, 4, 4))
    return f


def hold_tuba(col=YEL):
    """A tuba worn round the body, its bell over the shoulder."""
    def f(m, h, side):
        if side > 0:
            m.cyl(col, (-26, 8, 96), 9, 62, steps=7, top_r=34)
            m.box(col, (-8, -16, 92), (44, 12, 44))
    return f


WALK = dict(stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', bob=2.0)


def walker(sc, r, x, y, L, v, d, z=DECK, detail=0, **pose):
    """A figure walking the stage's length (v cm/s; negative walks towards -X) and coming round again."""
    p = dict(WALK, rate=abs(v) / 110.0)
    p.update(pose)
    p.update(d)
    P.figure(sc, x, y, z, yaw=90 if v > 0 else -90, detail=detail, phase=r.random(), travel=(L, v), **p)


# ------------------------------------------------------------------------------------------ 1 the wedding
def wedding(sc):
    r = random.Random(701)
    g = sc.still()
    L, W = 1100.0, 320.0
    stage(g, L, W, stripe=(PINK, WHITE))
    ax, ay = 0.0, 80.0
    for sx in (-1, 1):
        g.box(WHITE, (ax + sx * 100, ay, DECK + 128), (10, 10, 256))
    g.box(WHITE, (ax, ay, DECK + 260), (222, 12, 12))
    cols = (PINK, YEL, WHITE, '#f4a6ff', ORANGE)
    fl = sc.part(P.motion(swing=3.0, pivot=(ax, ay, DECK + 260), axis=0.0, rate=0.5))
    for k in range(5):
        a = math.pi * k / 4.0
        fl.sphere(cols[k % 5], (ax - math.cos(a) * 96, ay - 8, DECK + 262 + math.sin(a) * 36), 24, steps=(3, 6))
    for sx in (-1, 1):
        for k in range(3):
            fl.box(cols[(k + sx + 5) % 5], (ax + sx * 100, ay - 8, DECK + 60 + k * 66), (26, 12, 26))
    # the couple, turned to each other under the arch; the one who marries them behind
    P.figure(sc, -36, 60, DECK, yaw=90, arm_l='hold', arm_r='hold', hold_r=hold_bouquet(), rock=3, rate=0.4, detail=1,
             **folk(r, wear='dress', top=WHITE, sleeves='short', hair='bun', hat=None))
    P.figure(sc, 36, 60, DECK, yaw=-90, arm_l='hold', arm_r='hold', rock=3, rate=0.4, phase=0.5, detail=1,
             **folk(r, wear='trousers', top=CYAN, bottom='#2b2350', sleeves='long', hat=None))
    P.figure(sc, 0, 132, DECK, arm_l='vee', arm_r='vee', swing_l=8, swing_r=-8, rate=0.5, detail=1,
             **folk(r, wear='coat', top=VIOLET, sleeves='long', hat=None))
    # petals in the air over them
    for k in range(2):
        pt = sc.part(P.motion(bob=-26.0, rate=0.45, phase=k * 0.5, pivot=(0.0, 60.0, DECK + 230)))
        for j in range(6):
            pt.box((PINK, WHITE, YEL)[(j + k) % 3], (r.uniform(-90, 90), 60 + r.uniform(-50, 50), DECK + 215 + r.uniform(-20, 40)), (10, 10, 3),
                   rot=(r.uniform(0, 40), r.uniform(0, 90), 0))
    acts = (
        dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=6, rate=1.4),                          # throwing petals
        dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4),     # clapping
        dict(arm_r='wave', swing_r=24, rate=1.2, bob=3),
        dict(arm_r='face', hold_r=P.hold_phone(), rate=0.3, bob=-1.0),
    )
    for i, (x, y) in enumerate(((-460, -50), (-370, 50), (-280, -70), (270, -70), (360, 50), (450, -40))):
        d = folk(r)
        d.update(acts[(i * 3) % 4])
        P.figure(sc, x, y, DECK, yaw=face(x, y, 0, 60) + r.uniform(-10, 10), phase=r.random(), detail=0, **d)
    P.figure(sc, -150, -100, DECK, yaw=face(-150, -100, 0, 60), arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=5, rate=0.4, detail=1,
             **folk(r, hat='cap'))
    P.figure(sc, 150, -95, DECK, yaw=face(150, -95, 0, 60), arm_l='carry', arm_r='carry', hold_r=hold_basket(), bob=7, rate=1.3, phase=0.3,
             detail=0, **child(r, wear='dress'))
    P.figure(sc, 430, 110, DECK, yaw=face(430, 110, 0, 60), arm_l='hold', arm_r='hold', hold_r=hold_squeeze(), swing_l=9, swing_r=-9,
             axis_l='z', axis_r='z', rate=1.2, detail=0, **folk(r, hat='sun'))


# ------------------------------------------------------------------------------------------ 2 the steam kitchen
def kitchen(sc):
    r = random.Random(702)
    g = sc.still()
    L, W = 1000.0, 320.0
    stage(g, L, W, stripe=(YEL, ORANGE))
    ox, oy = -130.0, 70.0
    g.cyl(STEEL_D, (ox, oy, DECK), 72, 46, steps=8)                       # the vent's oven: a ring of basalt, a grate
    g.cyl(DARK, (ox, oy, DECK + 46), 62, 5, steps=8)
    g.cyl(STEEL, (ox - 24, oy, DECK + 51), 22, 28, steps=8)               # the pot
    g.cyl(WOOD, (ox + 28, oy + 4, DECK + 51), 20, 14, steps=8)            # a basket of eggs
    for k in range(4):
        g.sphere(WHITE, (ox + 20 + (k % 2) * 15, oy - 4 + (k // 2) * 15, DECK + 68), 8, steps=(3, 5))
    steam(sc, ox - 20, oy, DECK + 86)
    for sx in (-1, 1):                                                     # the awning
        for sy in (-1, 1):
            g.box(STEEL_D, (ox + sx * 190, oy + sy * 66, DECK + 125), (6, 6, 250))
    for k in range(4):
        g.box((YEL, WHITE)[k % 2], (ox - 143 + k * 95, oy, DECK + 254), (95, 150, 8))
    val = sc.part(P.motion(swing=6.0, pivot=(ox, oy - 76, DECK + 250), axis=0.0, rate=0.7))
    for k in range(4):
        val.box((WHITE, YEL)[k % 2], (ox - 143 + k * 95, oy - 76, DECK + 232), (95, 3, 36))
    P.figure(sc, ox - 20, oy + 96, DECK, arm_r='fwd', hold_r=P.hold_tool(col=STEEL, grip=WOOD, length=30), swing_r=12, axis_r='z', arm_l='hip',
             rate=0.9, detail=1, **folk(r, hat='chef', hat_col=WHITE, top=WHITE, wear='trousers'))
    P.figure(sc, ox + 130, oy + 30, DECK, yaw=-70, arm_l='carry', arm_r='carry', hold_r=hold_basket(fill=WHITE), bob=-1.5, rate=0.4, detail=1,
             **folk(r, hat='chef', hat_col=WHITE, top=WHITE, wear='trousers'))
    # the counter and whoever serves at it
    table(g, 190, 40, DECK, w=180, d=60, h=86, top=WHITE)
    g.box(ORANGE, (190, 9, DECK + 66), (180, 2, 14))
    for k in range(4):
        g.cyl((CYAN, PINK, LIME, YEL)[k], (130 + k * 40, 40, DECK + 86), 11, 9, steps=6, top_r=15)
    P.figure(sc, 200, 104, DECK, arm_l='hold', arm_r='fwd', hold_r=hold_bowl(), swing_r=8, axis_r='x', rate=0.6, detail=0, **folk(r, hat='cap'))
    # the queue along the rail, bowls in hand
    for i, x in enumerate((240, 320, 400, 470)):
        d = folk(r) if i != 2 else child(r)
        P.figure(sc, x, -70 + (i % 2) * 22, DECK, yaw=-90 + r.uniform(-12, 12), arm_l='hold', arm_r='hold', hold_r=hold_bowl((WHITE, CYAN, PINK)[i % 3]),
                 rock=2.5, rate=0.3 + i * 0.05, phase=r.random(), detail=0, **d)
    # two eating on a bench; a child pointing at the chase
    crate(g, -385, 60, DECK, w=170)
    for i, x in enumerate((-430, -345)):
        P.figure(sc, x, 60, DECK, legs='sit', seat=46, arm_r='face', hold_r=hold_bowl(), swing_r=9, axis_r='x', arm_l='down', rate=0.5,
                 phase=i * 0.4, detail=1 if i == 0 else 0, **folk(r))
    P.figure(sc, -260, -95, DECK, yaw=-15, arm_r='point', swing_r=12, axis_r='x', arm_l='up', swing_l=14, bob=8, rate=1.4, react=True, detail=0,
             **child(r))


# ------------------------------------------------------------------------------------------ 3 the offering
def shrine(sc):
    r = random.Random(703)
    g = sc.still()
    L, W = 1100.0, 320.0
    stage(g, L, W, stripe=(VIOLET, YEL))
    sx0, sy0 = 400.0, 80.0
    g.box(BASALT, (sx0, sy0, DECK + 35), (160, 110, 70))                 # the cairn, a gold plate on its face
    g.box(STEEL_D, (sx0, sy0, DECK + 95), (116, 84, 50))
    g.box(BASALT, (sx0, sy0, DECK + 150), (74, 60, 60))
    g.box(YEL, (sx0, sy0 - 32, DECK + 150), (32, 6, 32))
    g.box(WOOD, (sx0, sy0, DECK + 262), (7, 7, 164))
    for k in range(4):                                                   # its streamers
        f = sc.part(P.motion(swing=15.0, pivot=(sx0, sy0, DECK + 300), axis=P.UP, rate=0.55, phase=k * 0.17))
        f.box((PINK, YEL, CYAN, WHITE)[k], (sx0 - 62, sy0, DECK + 334 - k * 30), (116, 3, 22))
    table(g, 262, 50, DECK, w=90, d=60, h=44, top=WOOD, leg=WOOD)        # the offering: fruit and flowers
    for k in range(3):
        g.sphere((ORANGE, YEL, PINK)[k], (240 + k * 22, 50, DECK + 56), 12, steps=(3, 6))
    P.figure(sc, 170, 40, DECK, yaw=90, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, hold_r=hold_stick(length=120), rate=0.4, detail=1,
             **folk(r, wear='coat', top=VIOLET, sleeves='long', hat=None, hair='long', hair_col='#cfcfd8'))
    # the ring of dancers going round a bowl of flowers
    cx, cy, R = -130.0, 20.0, 120.0
    g.cyl(STEEL_D, (cx, cy, DECK), 34, 30, steps=8)
    g.sphere(PINK, (cx, cy, DECK + 40), 26, scale=(1.0, 1.0, 0.6), steps=(3, 6))
    for k in range(4):
        a = math.radians(k * 90 + 20)
        P.figure(sc, cx + math.cos(a) * R, cy + math.sin(a) * R, DECK, yaw=math.degrees(a) + 180.0, arm_l='out', arm_r='up', swing_l=14, swing_r=-14,
                 bob=5, rate=1.0, phase=k * 0.25, orbit=(cx, cy, 0.07), detail=1 if k % 2 == 0 else 0,
                 **folk(r, wear=('dress', 'skirt')[k % 2], top=BRIGHT[k * 2], hat=None))
    # the drummers
    for i, (x, y) in enumerate(((-450, 60), (-360, 100))):
        g.cyl(WOOD, (x, y - 46, DECK), 26, 64, steps=8, top_r=30)
        g.cyl(WHITE, (x, y - 46, DECK + 64), 30, 4, steps=8)
        P.figure(sc, x, y, DECK, arm_l='carry', arm_r='carry', swing_l=22, swing_r=-22, axis_l='x', axis_r='x', rate=2.0, phase=i * 0.25, detail=0,
                 **folk(r, sleeves='none', wear='trousers', hat=None))
    for i, (x, y) in enumerate(((60, -95), (140, -105))):                # two sitting with their bowls
        P.figure(sc, x, y, DECK, yaw=face(x, y, sx0, sy0), legs='sit', seat=24, arm_l='hold', arm_r='hold', hold_r=hold_bowl(YEL), rock=4, rate=0.3,
                 phase=i * 0.4, detail=0, **folk(r))
    P.figure(sc, 300, -90, DECK, yaw=face(300, -90, sx0, sy0), arm_l='carry', arm_r='carry', hold_r=hold_basket(fill=YEL), bob=6, rate=1.1, detail=0,
             **child(r))


# ------------------------------------------------------------------------------------------ 4 the rescue drill
def rescue(sc):
    r = random.Random(704)
    g = sc.still()
    L, W = 1200.0, 320.0
    stage(g, L, W, stripe=(CYAN, WHITE))
    # a volunteer carried along the rail on a litter chair
    Lt, v, x0, y0 = 1000.0, 70.0, 0.0, -70.0
    lit = sc.part(P.motion(travel=tv(Lt, v), base=(x0, y0), pivot=(x0, y0, DECK + 80)))
    for sy in (-1, 1):
        lit.box(STEEL, (x0, y0 + sy * 24, DECK + 80), (230, 5, 5))
    lit.box(ORANGE, (x0, y0, DECK + 84), (56, 52, 8))
    lit.box(ORANGE, (x0 - 26, y0, DECK + 116), (6, 52, 60))
    for dx in (-88, 88):
        walker(sc, r, x0 + dx, y0, Lt, v, rescuer(r), arm_l='carry', arm_r='carry', swing_l=0, swing_r=0, detail=1 if dx > 0 else 0)
    P.figure(sc, x0, y0, DECK + 38, yaw=0, legs='sit', seat=46, arm_r='wave', swing_r=24, arm_l='down', rate=1.1, travel=(Lt, v), detail=1,
             **folk(r, hat='beanie'))
    # the winch: an A-frame leaning out over the back, one of the team going up and down on its rope
    wx = 430.0
    for sx in (-1, 1):
        tilt_box(g, STEEL_D, (wx + sx * 56, 140, DECK), (wx, 236, DECK + 310), 9, 9)
    tilt_box(g, STEEL_D, (wx, 30, DECK), (wx, 236, DECK + 310), 6, 6)
    g.box(WHITE, (wx, 236, DECK + 80), (5, 5, 460))
    g.box(YEL, (wx, 44, DECK + 26), (60, 44, 52))
    P.figure(sc, wx, 236, DECK - 90, yaw=180, arm_l='up', arm_r='up', bob=-50.0, rate=0.2, detail=0, **rescuer(r))
    P.figure(sc, wx - 20, 96, DECK, yaw=180, arm_l='push', arm_r='push', swing_l=16, swing_r=16, axis_l='x', axis_r='x', rate=1.0, detail=0,
             **rescuer(r))
    # the search dog and its handler; the coordinator's flag; one in a foil blanket with tea, the medic beside
    P.figure(sc, -470, 70, DECK, yaw=-30, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.6, detail=0, **rescuer(r))
    P.quadruped(sc, -380, 40, DECK, yaw=10, length=74, height=50, girth=22, neck=17, neck_up=38, head=20, tail=24, wag=32, rate=2.0,
                body='#e8c26a', ears='#8e5a3a', bob=2.0)
    g.box(CYAN, (-380, 46, DECK + 46), (26, 30, 8))                      # its jacket
    P.figure(sc, -250, -100, DECK, yaw=10, arm_r='wave', hold_r=P.hold_flag(col=CYAN, w=70), swing_r=20, arm_l='hip', rate=1.0, bob=3, react=True,
             detail=1, **rescuer(r))
    crate(g, 170, 96, DECK, w=90)
    P.figure(sc, 170, 96, DECK, yaw=-10, legs='sit', seat=46, arm_r='face', hold_r=P.hold_cup(), arm_l='hold', rate=0.3, bob=-1.0, detail=0,
             **folk(r, wear='coat', top=SILVER, sleeves='long', hat='beanie'))
    P.figure(sc, 262, 70, DECK, yaw=-80, arm_l='hold', arm_r='hold', hold_r=hold_bowl(WHITE), rock=3, rate=0.4, detail=0, **rescuer(r, hat='cap', hat_col=WHITE))


# ------------------------------------------------------------------------------------------ 5 the samplers
def sample(sc):
    r = random.Random(705)
    g = sc.still()
    L, W = 1000.0, 320.0
    stage(g, L, W, stripe=(SILVER, CYAN))
    for i, x in enumerate((-330, -210)):                                 # two with their ladles out over the back
        P.figure(sc, x, 120, DECK, yaw=180 + (i * 2 - 1) * 6, legs='stride', arm_l='carry', arm_r='carry', hold_r=hold_ladle(), rock=2.5, rate=0.3,
                 phase=i * 0.4, detail=1, **suited(r))
    g.box(STEEL, (-60, 110, DECK + 26), (120, 56, 52))                   # the quench trough
    g.box(WATER, (-60, 110, DECK + 53), (108, 46, 3))
    steam(sc, -60, 110, DECK + 70, n=3, r0=20)
    P.figure(sc, -60, 44, DECK, yaw=180, arm_r='fwd', hold_r=P.hold_tool(col=STEEL, grip=DARK, length=46), swing_r=10, axis_r='x', arm_l='hip',
             rate=0.5, detail=0, **suited(r))
    table(g, 170, 100, DECK, w=150, d=60, h=80, top=STEEL)               # the samples, weighed and written down
    for k in range(4):
        g.box((BASALT, ORANGE, YEL, STEEL_D)[k], (116 + k * 36, 100, DECK + 90), (22, 22, 16 + (k % 2) * 8))
    P.figure(sc, 170, 150, DECK, arm_l='hold', arm_r='hold', rate=0.35, bob=-1.5, detail=0, **scientist(r))
    # a tracked rover going to and fro along the rail, and whoever steers it
    Lr, vr, rx, ry = 760.0, -45.0, 0.0, -30.0
    rv = sc.part(P.motion(travel=tv(Lr, vr), base=(rx, ry), pivot=(rx, ry, DECK + 20)))
    rv.box(YEL, (rx, ry, DECK + 26), (76, 50, 22))
    for sy in (-1, 1):
        rv.box(DARK, (rx, ry + sy * 30, DECK + 12), (90, 12, 24))
    rv.box(STEEL, (rx + 14, ry, DECK + 66), (5, 5, 58))
    rv.box(CYAN, (rx + 14, ry, DECK + 98), (26, 20, 14))
    P.figure(sc, 400, -70, DECK, yaw=-90, arm_l='hold', arm_r='hold', rock=4, rate=0.35, detail=1, **scientist(r))
    # one walking the rail with the gas wand; the thermal camera and its operator; one with the helmet off, fanning
    walker(sc, r, 120, -105, 820.0, 55.0, suited(r), arm_r='fwd', swing_r=0, hold_r=P.hold_rod(col=STEEL, length=90))
    tripod(g, -430, -30, DECK)
    g.box(DARK, (-430, -30, DECK + 134), (30, 44, 28))
    g.cyl(CYAN, (-430, -8, DECK + 134), 9, 14, rot=(0, 0, -90), steps=6)
    P.figure(sc, -430, -96, DECK, yaw=180, arm_l='hold', arm_r='hold', rate=0.3, bob=-1.2, detail=0, **suited(r))
    crate(g, 330, 96, DECK, w=70, col=STEEL_D)
    P.figure(sc, 330, 96, DECK, yaw=10, legs='sit', seat=46, arm_r='face', hold_r=hold_fan(), swing_r=14, axis_r='x', arm_l='down', rate=2.0,
             detail=1, **suited(r, hat=None, hair='afro'))
    g.sphere(SILVER, (384, 100, DECK + 22), 26, steps=(3, 6))            # the helmet, beside the crate


# ------------------------------------------------------------------------------------------ 6 the school leaving
def school(sc):
    r = random.Random(706)
    g = sc.still()
    L, W = 1400.0, 320.0
    stage(g, L, W, stripe=(LIME, WHITE))
    v = 75.0
    walker(sc, r, 520, -40, L, v, folk(r, wear='coat', top=LIME, sleeves='long', hat='cap', hat_col=WHITE), arm_r='up', swing_r=0,
           hold_r=hold_paddle(), detail=1)
    k = 0
    for x in (400, 300, 200):
        for y in (-80, -5):
            pose = dict(arm_l='wave', swing_l=24, axis_l='y') if k % 3 == 1 else {}
            walker(sc, r, x + r.uniform(-12, 12), y, L, v, child(r, pack=CLOTH[k % len(CLOTH)], hat=('cap', None, 'sun')[k % 3]), **pose)
            k += 1
    walker(sc, r, 80, -40, L, v, folk(r, hat='sun'), hold_r=P.hold_bag(PINK), hold_l=P.hold_bag(CYAN), swing_l=6, swing_r=-6, detail=1)
    P.quadruped(sc, -30, 10, DECK, yaw=90, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=22, rate=1.6,
                body=WHITE, ears='#5a4a3c', travel=(L, v), bob=2.0)
    # the water table at the back, a child drinking, the crossing warden waving them on
    table(g, -360, 110, DECK, w=170, d=56, h=80)
    g.cyl(CYAN, (-410, 110, DECK + 80), 20, 44, steps=8)
    for i in range(4):
        g.cyl(WHITE, (-350 + i * 20, 104, DECK + 80), 6, 11, steps=5)
    P.figure(sc, -380, 150, DECK, arm_r='fwd', hold_r=P.hold_cup(), swing_r=8, axis_r='x', arm_l='hip', rate=0.6, detail=0, **folk(r, hat='cap'))
    P.figure(sc, -230, 106, DECK, yaw=-90, arm_r='face', hold_r=P.hold_cup(), swing_r=6, axis_r='x', rate=0.5, detail=0, **child(r, pack=YEL))
    P.figure(sc, 620, 120, DECK, yaw=-60, arm_r='wave', swing_r=22, arm_l='point', rate=1.1, bob=3, react=True, detail=1,
             **folk(r, wear='coat', top=LIME, sleeves='long', hat='peak', hat_col=WHITE))
    bunting(sc, g, -640, -80, 150, DECK, DECK + 300, (LIME, WHITE, CYAN, YEL), n=8)


# ------------------------------------------------------------------------------------------ 7 the hot spring
def hot_spring(sc):
    r = random.Random(707)
    g = sc.still()
    L, W = 1000.0, 320.0
    stage(g, L, W, stripe=(CYAN, PLANK_D))
    tx, ty, TL, TW, TH = -70.0, 45.0, 620.0, 220.0, 70.0
    for sy in (-1, 1):
        g.box(WOOD, (tx, ty + sy * (TW / 2.0 - 7), DECK + TH / 2.0), (TL, 14, TH))
    for sx in (-1, 1):
        g.box(WOOD, (tx + sx * (TL / 2.0 - 7), ty, DECK + TH / 2.0), (14, TW, TH))
    g.box(WATER, (tx, ty, DECK + 55), (TL - 28, TW - 28, 6))
    for k in range(3):
        steam(sc, tx - 190 + k * 190, ty + 30 - (k % 2) * 50, DECK + 80, n=2, r0=22, rate=0.28 + k * 0.05)
    acts = (
        dict(arm_l='up', arm_r='up', swing_l=8, swing_r=-8, rate=0.4),                         # a long stretch
        dict(arm_r='face', hold_r=P.hold_cup(), swing_r=7, axis_r='x', rate=0.4),
        dict(arm_r='wave', swing_r=24, rate=1.2, react=True),
        dict(arm_l='out', arm_r='out', rock=3, rate=0.3),
    )
    for i, x in enumerate((-290, -150, -10, 130)):
        d = folk(r, wear='swim', sleeves='short', hat=(None, 'sun', None, 'beanie')[i], top=BRIGHT[(i * 3 + 1) % len(BRIGHT)])
        d.update(acts[i])
        P.figure(sc, tx + x + 70, ty + (20 if i % 2 else -20), DECK - 10, yaw=r.uniform(-25, 25), legs='sit', seat=20, phase=r.random(), bob=-2.0,
                 detail=1 if i in (1, 2) else 0, **d)
    # one on the rim with their feet in; the attendant with a bucket; a child about to jump; tea
    P.figure(sc, -230, ty - TW / 2.0 + 8, DECK, yaw=180, legs='sit', seat=TH, arm_l='back', arm_r='back', rock=3, rate=0.3, detail=0,
             **folk(r, wear='shorts', hat=None))
    P.figure(sc, 290, 60, DECK, yaw=-90, arm_r='fwd', hold_r=hold_bucket(), swing_r=16, axis_r='x', arm_l='hip', rate=0.5, detail=1,
             **folk(r, wear='coat', top=WHITE, sleeves='short', hat=None))
    P.figure(sc, 300, -80, DECK, yaw=-70, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=8, rate=1.5, detail=0,
             **child(r, wear='shorts', hat=None))
    table(g, 420, 100, DECK, w=90, d=50, h=60, top=WOOD, leg=WOOD)
    g.cyl(WHITE, (400, 100, DECK + 60), 12, 18, steps=6)
    for i in range(2):
        g.cyl(CYAN, (430 + i * 18, 96, DECK + 60), 6, 8, steps=5)
    P.figure(sc, 430, 146, DECK, arm_l='hold', arm_r='hold', hold_r=P.hold_cup(), bob=-1.5, rate=0.35, detail=0,
             **folk(r, wear='coat', top=PINK, sleeves='long', hat=None, hair='bun'))
    walker(sc, r, -100, -110, 860.0, 55.0, folk(r, wear='coat', top=WHITE, sleeves='long', hat=None), hold_l=hold_towel(CYAN))
    # the screen at the back, towels on it
    g.box(PLANK_D, (tx - 60, 152, DECK + 90), (480, 6, 180))
    tw = sc.part(P.motion(swing=7.0, pivot=(tx - 60, 148, DECK + 178), axis=0.0, rate=0.6))
    for k in range(3):
        tw.box((PINK, WHITE, YEL)[k], (tx - 200 + k * 140, 146, DECK + 140), (70, 4, 76))


# ------------------------------------------------------------------------------------------ 8 the volcano tour
def tour(sc):
    r = random.Random(708)
    g = sc.still()
    L, W = 1100.0, 320.0
    stage(g, L, W, stripe=(ORANGE, WHITE))
    P.figure(sc, -420, -20, DECK, yaw=75, arm_r='wave', hold_r=P.hold_flag(col=YEL, w=64), swing_r=8, arm_l='out', swing_l=8, rate=0.6, detail=1,
             **folk(r, wear='shorts', top=ORANGE, hat='sun', hat_col=WHITE))
    acts = (
        dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=5, rate=0.4),
        dict(arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.6),
        dict(arm_r='face', hold_r=P.hold_phone(), rate=0.3, bob=-1.0),
        dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=14, rate=0.35),
    )
    for i, (x, y) in enumerate(((-320, 60), (-250, -50), (-170, 40), (-100, -70))):
        d = folk(r, hat='sun', hat_col=CLOTH[(i * 2) % len(CLOTH)], pack=CLOTH[(i * 2 + 3) % len(CLOTH)] if i % 2 else None)
        d.update(acts[i])
        P.figure(sc, x, y, DECK, yaw=face(x, y, -420, -20) + r.uniform(-25, 25), phase=r.random(), detail=0, **d)
    P.figure(sc, -200, -105, DECK, yaw=-10, arm_r='point', swing_r=14, axis_r='x', arm_l='up', swing_l=12, bob=8, rate=1.4, react=True, detail=0,
             **child(r, hat='cap'))
    # two taking their own picture, the chase behind them
    P.figure(sc, 10, -70, DECK, yaw=170, arm_r='fwd', hold_r=P.hold_rod(col=DARK, length=70), arm_l='hip', bob=3, rate=0.8, detail=1, **folk(r, hat='cap'))
    P.figure(sc, 62, -78, DECK, yaw=190, arm_l='wave', swing_l=20, bob=3, rate=0.8, phase=0.4, detail=0, **folk(r, hat=None))
    # the painters at their easels, the mountain on the canvas
    for i, x in enumerate((200, 330)):
        tripod(g, x, 132, DECK, top=150, spread=36, col=WOOD)
        g.box(WHITE, (x, 116, DECK + 140), (76, 4, 60))
        g.box('#57c7ff', (x, 113, DECK + 156), (70, 2, 24))
        g.box(STEEL_D, (x, 112, DECK + 128), (44, 2, 30))
        g.box(ORANGE, (x, 111, DECK + 148), (14, 2, 12))
        if i == 0:
            P.figure(sc, x, 50, DECK, yaw=180, arm_r='fwd', hold_r=hold_stick(col=DARK, length=16), swing_r=7, axis_r='z', arm_l='hip', rate=1.2,
                     detail=1, **folk(r, hat='sun', wear='dress'))
        else:
            crate(g, x, 44, DECK, w=44, d=44, h=44, col=STEEL_D)
            P.figure(sc, x, 44, DECK, yaw=180, legs='sit', seat=44, arm_r='fwd', hold_r=hold_stick(col=DARK, length=16), swing_r=7, axis_r='z',
                     rate=1.0, phase=0.5, detail=0, **folk(r, hat='beanie'))
    # the souvenir stand under its turning parasol
    px, py = 460.0, 60.0
    g.box(STEEL_D, (px, py, DECK + 120), (7, 7, 240))
    par = sc.part(P.motion(base=(px, py), travel=(-1.0, 0.1)))
    par.cyl(PINK, (px, py, DECK + 226), 120, 34, steps=8, top_r=8)
    for k in range(4):
        par.box(WHITE, (px, py, DECK + 232), (236, 22, 6), rot=(0, k * 45.0, 0))
    table(g, px - 10, py - 60, DECK, w=120, d=46, h=70, top=WOOD, leg=WOOD)
    for k in range(4):                                                   # little cones of the mountain, for sale
        g.cyl((BASALT, STEEL_D)[k % 2], (px - 52 + k * 28, py - 60, DECK + 70), 11, 18, steps=6, top_r=4)
        g.box(ORANGE, (px - 52 + k * 28, py - 60, DECK + 90), (6, 6, 4))
    P.figure(sc, px, py + 30, DECK, arm_r='wave', swing_r=20, arm_l='hip', rate=1.0, bob=2, detail=0, **folk(r, hat='cap', hat_col=PINK))


# ------------------------------------------------------------------------------------------ 9 the village band
def band(sc):
    r = random.Random(709)
    g = sc.still()
    L, W = 1100.0, 320.0
    stage(g, L, W, stripe=(PINK, YEL))
    bunting(sc, g, -530, 530, 150, DECK, DECK + 310, (PINK, YEL, WHITE, CYAN), n=12)
    coat = dict(wear='coat', sleeves='long', top=PINK, hat='peak', hat_col=WHITE)
    P.figure(sc, -330, 90, DECK, arm_l='hold', arm_r='hold', hold_r=hold_tuba(), rock=5, rate=1.0, detail=1, **folk(r, build='stocky', **coat))
    g.cyl(WHITE, (-190, 84, DECK + 100), 48, 34, rot=(0, 0, 90), steps=10)       # the bass drum on its stand
    g.cyl(PINK, (-190, 86, DECK + 100), 52, 6, rot=(0, 0, 90), steps=10)
    g.box(STEEL_D, (-190, 66, DECK + 26), (60, 30, 52))
    P.figure(sc, -190, 150, DECK, arm_r='fwd', hold_r=hold_stick(length=30), swing_r=26, axis_r='x', arm_l='fwd', swing_l=-26, axis_l='x', rate=2.0,
             detail=0, **folk(r, **coat))
    for i, x in enumerate((-60, 40)):
        P.figure(sc, x, 100, DECK, yaw=(-8, 8)[i], arm_r='face', arm_l='face', hold_r=hold_horn(), bob=3, rock=0, rate=1.0, phase=i * 0.5,
                 detail=1 if i == 0 else 0, **folk(r, **coat))
    P.figure(sc, 150, 96, DECK, yaw=12, arm_l='hold', arm_r='hold', hold_r=hold_squeeze(col=CYAN), swing_l=10, swing_r=-10, axis_l='z', axis_r='z',
             rate=1.0, detail=0, **folk(r, **coat))
    P.figure(sc, -80, -70, DECK, yaw=180, arm_l='vee', arm_r='vee', hold_r=hold_stick(col=WHITE, length=36), swing_l=16, swing_r=-16, rate=1.0,
             detail=1, **folk(r, wear='coat', sleeves='long', top=WHITE, hat='peak', hat_col=PINK))
    # two couples turning
    for i, (cx, cy, rate) in enumerate(((320.0, -30.0, 0.2), (440.0, 70.0, -0.17))):
        for sx in (-1, 1):
            P.figure(sc, cx + sx * 36, cy, DECK, yaw=-90 * sx, arm_l='out', arm_r='hold', bob=4, rate=1.0, phase=i * 0.3, orbit=(cx, cy, rate),
                     detail=0, **folk(r, wear=('dress', 'trousers')[(sx + 1) // 2], hat=None))
    P.figure(sc, -440, -80, DECK, yaw=20, arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', bob=6, rate=2.0, react=True,
             detail=0, **child(r))


# ------------------------------------------------------------------------------------------ 10 the press briefing
def briefing(sc):
    r = random.Random(710)
    g = sc.still()
    L, W = 1000.0, 320.0
    stage(g, L, W, stripe=(WHITE, VIOLET))
    # the hazard board: the mountain, its plume, three coloured bands (shapes only)
    bx, by = 360.0, 140.0
    for sx in (-1, 1):
        g.box(STEEL_D, (bx + sx * 96, by, DECK + 100), (7, 7, 200))
    g.box(WHITE, (bx, by, DECK + 150), (210, 6, 130))
    g.box(BASALT, (bx - 30, by - 4, DECK + 112), (110, 2, 34))
    g.box(BASALT, (bx - 30, by - 4, DECK + 142), (66, 2, 28))
    g.box(BASALT, (bx - 30, by - 4, DECK + 166), (30, 2, 22))
    g.box(ORANGE, (bx - 30, by - 5, DECK + 184), (16, 2, 16))
    for k in range(3):
        g.box((LIME, YEL, PINK)[k], (bx + 66, by - 4, DECK + 110 + k * 34), (44, 2, 24))
    P.figure(sc, 330, 10, DECK, yaw=-90, arm_l='out', hold_l=hold_pointer(length=40.0), swing_l=8, arm_r='hip', rate=0.5, detail=1, **scientist(r))
    g.box(WHITE, (250, 30, DECK + 55), (56, 46, 110))                    # the lectern, its microphones
    g.box(VIOLET, (221, 30, DECK + 80), (2, 46, 16))
    for k in range(2):
        g.box(DARK, (236, 18 + k * 22, DECK + 122), (5, 5, 24))
    P.figure(sc, 440, -50, DECK, yaw=-80, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=WHITE, w=36), bob=-1.5, rate=0.4, detail=0, **scientist(r))
    # the press: three standing along the rail, three on chairs, a camera on its tripod
    acts = (
        dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.4),
        dict(arm_r='fwd', hold_r=P.hold_rod(col=DARK, length=190), arm_l='hip', rock=2, rate=0.3),
        dict(arm_r='up', swing_r=12, arm_l='down', bob=3, rate=1.1),
    )
    for i, x in enumerate((-330, -190, -50)):
        d = folk(r, hat=(None, 'cap', 'headset')[i], hat_col=DARK)
        d.update(acts[i])
        P.figure(sc, x, -80 + (i % 2) * 20, DECK, yaw=90 + r.uniform(-10, 10), phase=r.random(), detail=1 if i == 0 else 0, **d)
    for i, x in enumerate((-290, -160, -30)):
        crate(g, x, 60, DECK, w=42, d=42, h=46, col=STEEL)
        g.box(STEEL, (x - 20, 60, DECK + 70), (4, 42, 48))
        pose = dict(arm_r='up', swing_r=10, rate=0.9) if i == 1 else dict(arm_l='hold', arm_r='hold', swing_r=6, axis_r='z', rate=1.6)
        P.figure(sc, x, 60, DECK, yaw=90, legs='sit', seat=46, phase=r.random(), detail=0, **dict(folk(r), **pose))
    tripod(g, 100, -70, DECK)
    cam = sc.part(P.motion(swing=12.0, pivot=(100.0, -70.0, DECK + 130), axis=P.UP, rate=0.12))
    cam.box(DARK, (104, -70, DECK + 136), (56, 26, 30))
    cam.box(CYAN, (134, -70, DECK + 136), (6, 20, 20))
    P.figure(sc, 40, -70, DECK, yaw=90, arm_l='push', arm_r='push', rate=0.3, bob=-1.2, detail=0, **folk(r, hat='headset', hat_col=DARK))


# ------------------------------------------------------------------------------------------ the kit
def lifted(fn, g=0.035):
    """Every colour of a piece given a little steady light of its own, as the main cast's near pieces have (the Volcano
    is dark, and an unlit figure is a dark shape on a pale deck). These pieces stand 32 m and more from the road."""
    def build(sc):
        fn(sc)
        for key in sc.order:
            model = sc.parts[key][0]
            new = {}
            for (col, glow), pm in model.meshes.items():
                k2 = (col, glow if glow > 0.0 else g)
                if k2 in new:
                    unreal.GeometryScript_MeshEdits.append_mesh(new[k2], pm, unreal.Transform())
                else:
                    new[k2] = pm
            model.meshes = new
    return build


JOBS = [
    ('SM_P3B_Wedding', lifted(wedding)),
    ('SM_P3B_Kitchen', lifted(kitchen)),
    ('SM_P3B_Shrine', lifted(shrine)),
    ('SM_P3B_Rescue', lifted(rescue)),
    ('SM_P3B_Sample', lifted(sample)),
    ('SM_P3B_School', lifted(school)),
    ('SM_P3B_HotSpring', lifted(hot_spring)),
    ('SM_P3B_Tour', lifted(tour)),
    ('SM_P3B_Band', lifted(band)),
    ('SM_P3B_Briefing', lifted(briefing)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
