"""FULL THROTTLE (Unreal) the population of Ice Mountain, pass 3, builder p3a: THE GAPS (15 Nov 2026).

The general cast of the mountain, for the stretches of road where nobody stood within 60 m: people at work (piste
crews, a rescue team, avalanche surveyors), at rest (families on blankets in the snow, a hot-drinks hut, a climbers'
camp, an igloo being built, sled dogs staked out) and passing through (a ski school in a snake, snowshoers and ski
tourers along a snow bank, children pulling sleds).

Three pieces are LOW (built under 1.15 m, and saved with a small bounds margin: this hill's sight line pushes
anything over about 1.4 m out to 24.7 m): sitting people, children at 0.6 of life size, dogs. They are written 23 m
out, behind the crash padding. (Fix round, 16 Nov: the three now stand on a 40 cm snow plinth footed into the
ground, see on_plinth: 1.5 m, still LOW, and the track may now push them out to about 24.7 m.) The ten others are TALL: people 1.3 times life size on a snow mound or a deck of
their own (so their feet clear the barrier), footed 1.2 m into the ground (the hill falls 7 in 100 under a level
piece); the table lays them 28 m out on the glaciers, 44 m on the ridges (1.2 times) and 57 m in the resorts (1.5).

Looks only: no collision, no glow, nothing that blinks, no words or marks; nothing big, white and furry on two legs
(the level's boss is THE ABOMINABLE), no snowmen and no flying snowballs.

Meshes: /Game/People/IceMountain/SM_P3A_*   (that prefix is this builder's alone)
Run in the editor:  import ft_people_ice_p3a as K; K.run()         (or K.run(['SM_P3A_Camp_A']))
The rules that place them: C:/ftwork/p3/L5a.inl (pasted by the integrator at the end of FTPeopleL5.inl).
"""
import random
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/IceMountain'

SNOW, SNOW_SH, ICE, ICE_D = '#f4f9ff', '#b9c6ea', '#bfe3ff', '#7fb0e0'
WOOD, WOOD_D, STEEL, STEEL_D = '#9a6a44', '#6a4c36', '#9aa4b8', '#5a6478'
RED, BLUE, ORANGE, PINK, TEAL, YEL, LIME, VIOLET = '#ff2d3c', '#2e6bff', '#ff8a2a', '#ff4fb8', '#19b5c9', '#ffd23f', '#b8ff5a', '#8f6bff'
DARK, WHITE = '#22202a', '#fff6ea'
SUITS = (RED, PINK, ORANGE, YEL, LIME, '#2ee6ff', BLUE, VIOLET, '#ff5fa2', '#7dffb0')
LEGS = ('#3a5fa8', WHITE, VIOLET, TEAL, DARK)
BIG = 1.3            # people on the tall pieces (the cel pass inks a life-size figure 30 m off to a scribble)
FOOT = 120.0         # how far a tall piece goes into the ground
LOW_SAVE = dict(grow=(60.0, 60.0, 10.0), grow_down=(60.0, 60.0, 0.0))


# ------------------------------------------------------------------------------------------ people
def suit(r, big=BIG, hats=('beanie', 'beanie', 'helmet', 'cap'), **fixed):
    """Somebody in a ski suit or a padded coat in one hot colour."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear=r.choice(('trousers', 'trousers', 'coat')), top=c, bottom=r.choice((c,) + LEGS),
               hat=r.choice(hats), hat_col=r.choice(SUITS), shoes=r.choice((c, WHITE, DARK)),
               hair=r.choice(('short', 'long', 'bun')), s=big * r.uniform(0.95, 1.05))
    d.update(fixed)
    return d


def worker(r, top=ORANGE, band=LIME, hat='hard', hat_col=YEL, **fixed):
    """Somebody at work on the mountain: one bright jacket, a band across the chest, dark trousers."""
    d = P.look(r, sleeves='long', wear='trousers', top=top, bottom=r.choice((DARK, '#2b2350', '#3a5fa8')), hat=hat, hat_col=hat_col,
               band=band, shoes=DARK, hair=r.choice(('short', 'long', 'bun')), s=BIG * r.uniform(0.95, 1.05))
    d.update(fixed)
    return d


def put(sc, r, x, y, z, d, **act):
    """Draw one figure: a look (a dict from suit / worker) and what it does."""
    d = dict(d)
    d.update(act)
    d.setdefault('phase', r.random())
    d.setdefault('detail', 1)
    return P.figure(sc, x, y, z, **d)


def carried(sc, bx, by, L, v, pz=20.0, dx=0.0):
    """A model for a thing that travels with the walker whose place is (bx, by): a sled on its rope, skis underfoot."""
    return sc.part(P.motion(pivot=(bx + dx, by, pz), base=(bx, by), travel=(float(L), float(v) / float(L))))


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_shovel(col=STEEL, shaft=WOOD):
    def f(m, h, side):
        tilt_box(m, shaft, (h[0], h[1] + 26, h[2] + 34), (h[0], h[1] - 46, h[2] - 52), 5, 5)
        tilt_box(m, col, (h[0], h[1] - 44, h[2] - 50), (h[0], h[1] - 66, h[2] - 80), 26, 5)
    return f


def hold_pole(col=DARK, length=100.0, grip=RED):
    """A ski or trekking pole held by its top, its point on the snow."""
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 12 - length / 2.0), (4.5, 4.5, length))
        m.box(grip, (h[0], h[1], h[2] + 10), (9, 9, 12))
    return f


def hold_staff(length=300.0):
    """A surveyor's staff: red and white lengths, held upright."""
    def f(m, h, side):
        n = 6
        for k in range(n):
            m.box(RED if k % 2 else WHITE, (h[0], h[1] - 6, h[2] - 70 + (k + 0.5) * length / n), (9, 9, length / n))
    return f


def hold_probe(col=YEL, length=260.0):
    """An avalanche probe: a long thin pole pushed down into the snow ahead."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] - 4, h[2] + 70), (h[0], h[1] - 30, h[2] + 70 - length), 5, 5)
    return f


def hold_axe(col=STEEL, grip=ORANGE):
    def f(m, h, side):
        m.box(grip, (h[0], h[1], h[2] + 20), (5, 5, 70))
        m.box(col, (h[0], h[1] - 8, h[2] + 54), (6, 38, 7))
    return f


def hold_skis(col=RED, tip=WHITE, length=190.0):
    """A pair of skis carried upright against the shoulder."""
    def f(m, h, side):
        for k in (-1, 1):
            m.box(col, (h[0] + side * 8 + k * 5, h[1], h[2] - 30 + length / 2.0), (8, 4, length))
        m.box(tip, (h[0] + side * 8, h[1], h[2] - 30 + length - 10), (20, 5, 20))
    return f


def hold_block(col=ICE, w=52.0):
    """A cut block of snow, carried in both arms (drawn once, on the right hand)."""
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 26, h[1] - 8, h[2] + 14), (w, 36, 30))
            m.box(ICE_D, (h[0] - 26, h[1] - 8, h[2] - 2), (w, 36, 3))
    return f


# ------------------------------------------------------------------------------------------ furniture
def mound(g, length, depth, h, y=0.0, lumps=True):
    """A ploughed snow platform with a level top `h` cm up, footed FOOT into the ground: blue-grey sides, a white top."""
    g.box(SNOW_SH, (0, y, (h - FOOT) / 2.0 - 4), (length, depth, h + FOOT - 8))
    g.box(SNOW, (0, y, h - 4), (length + 24, depth + 24, 8))
    if lumps:
        for ex in (-1, 1):
            g.box(SNOW, (ex * (length / 2.0 + 45), y, (h * 0.55 - FOOT) / 2.0), (110, depth + 50, h * 0.55 + FOOT))


def low_net(g, length, y, h=62.0, col=ORANGE, posts=7):
    """A low safety net along the front of a low piece: posts and one band of colour (under 0.7 m)."""
    for i in range(posts):
        g.box(DARK, (-length / 2.0 + i * length / (posts - 1), y, h / 2.0 - 20), (7, 7, h + 40))
    g.box(col, (0, y, h - 16), (length, 4, 26))


def net_fence(g, x0, x1, y, z, col=ORANGE, h=120.0):
    """A run of piste net on posts, standing on z."""
    n = max(int(abs(x1 - x0) / 160.0), 1)
    for i in range(n + 1):
        g.box(DARK, (x0 + (x1 - x0) * i / n, y, z + h / 2.0), (8, 8, h))
    g.box(col, ((x0 + x1) / 2.0, y, z + h - 22), (abs(x1 - x0), 4, 30))
    g.box(col, ((x0 + x1) / 2.0, y, z + h - 66), (abs(x1 - x0), 4, 30))


def pennant(sc, x, y, z, height=330.0, col=ORANGE, pole=WHITE, w=110.0, phase=0.0):
    """A pole with a pennant that swings in the wind about the pole."""
    sc.still().box(pole, (x, y, z + height / 2.0), (7, 7, height))
    m = sc.part(P.motion(phase=phase, rate=0.8, swing=22.0, pivot=(x, y, z + height - 30), axis=P.UP))
    m.box(col, (x + w / 2.0, y, z + height - 34), (w, 3, 52))
    m.box(WHITE, (x + w * 0.3, y, z + height - 34), (w * 0.2, 4.2, 52))


def steam(sc, x, y, z, n=3, phase=0.0):
    """Puffs of steam over a pot: pale balls that float up and down, each on its own beat."""
    for k in range(n):
        m = sc.part(P.motion(phase=phase + k * 0.31, rate=0.35, bob=-16.0))
        m.sphere(WHITE, (x + (k - 1) * 14, y, z + 24 + k * 26), 13 + k * 4, steps=(3, 6))


def husky(sc, r, x, y, z=0.0, yaw=0.0, s=1.2, **kw):
    """A sled dog: grey or russet over a pale belly, tail wagging."""
    c = r.choice(('#8f9bb3', '#5a6478', '#b87a50', '#3a3848'))
    d = dict(length=70, height=48, girth=22, neck=16, neck_up=35, head=18, tail=22, body=c, belly=WHITE, snout=WHITE, ears=c,
             legs_col=WHITE, wag=30.0, rate=r.uniform(1.4, 2.2), phase=r.random())
    d.update(kw)
    return P.quadruped(sc, x, y, z, yaw=yaw, s=s, **d)


# ------------------------------------------------------------------------------------------ LOW: a family in the snow
def snow_seat(seed=101):
    """Families sitting on blankets in the snow behind a low net: four adults sitting, waving and drinking, two
    children hopping, a dog. 9 m long, built under 1.15 m: LOW."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        low_net(g, 900.0, -105.0)
        cols = (RED, TEAL, YEL, PINK, VIOLET)
        xs = (-350, -180, 0, 180, 350)
        for k, x in enumerate(xs):
            g.box(cols[k], (x, 20, 3), (150, 170, 6))
            g.box(WHITE, (x, 20, 4), (150, 30, 6.5))
        acts = (dict(arm_r='wave', swing_r=26, rate=1.1), dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.4),
                dict(arm_r='face', hold_r=P.hold_cup(), swing_r=8, axis_r='x', rate=0.4), dict(arm_l='wave', swing_l=24, rate=0.9))
        for k, x in enumerate((-350, -180, 180, 350)):
            d = suit(r, big=0.9, hats=('cap', 'cap', None), hair='short')
            put(sc, r, x, 40, 3, d, legs='sit', seat=8.0, yaw=r.uniform(-18, 18), react=True, **acts[(k + seed) % 4])
        for x in (-80, 70):
            d = suit(r, big=0.57, hats=('cap', None), build='kid', hair='short')
            put(sc, r, x, 10, 6, d, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=5, rate=1.6, react=True)
        husky(sc, r, 265, -40, yaw=20, s=1.1)
        g.box(STEEL, (-265, -30, 14), (36, 28, 28))                   # a flask box and a basket
        g.box(WOOD, (0, 70, 14), (60, 40, 28))
    return build


# ------------------------------------------------------------------------------------------ LOW: children and sleds
def sled_kids(seed=102):
    """Children pulling sleds along a trodden track, two one way and two the other, a dog running with them, two
    more hopping by a low snow wall. 12 m long, built under 1.15 m: LOW."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L = 1200.0
        low_net(g, L, -110.0, col=PINK, posts=9)
        g.box(SNOW_SH, (0, 0, 2), (L, 150, 4))                          # the trodden track
        for d1, y, xs, v in ((1, -30, (-380, 160), 120.0), (-1, 40, (-250, 300), 100.0)):
            for x in xs:
                d = suit(r, big=0.58, hats=('cap', None), build='kid', hair='short')
                put(sc, r, x, y, 4, d, yaw=90 * d1, stride=26, arm_l='back', arm_r='down', swing_r=18, axis_r='x', bob=3, rate=1.5,
                    travel=(L, d1 * v))
                m = carried(sc, x, y, L, d1 * v, dx=-d1 * 95)
                c = r.choice((RED, YEL, TEAL, ORANGE))
                m.box(c, (x - d1 * 95, y, 16), (70, 34, 8))
                for sy in (-1, 1):
                    m.box(WOOD_D, (x - d1 * 95, y + sy * 14, 6), (80, 5, 8))
                m.box(DARK, (x - d1 * 45, y, 22), (40, 3, 3))
                m.box(r.choice(SUITS), (x - d1 * 95, y, 30), (40, 26, 20))
        husky(sc, r, 120, -70, 4, yaw=90, s=1.0, stride=24, travel=(L, 120.0), wag=20.0)
        g.box(SNOW, (430, 95, 26), (260, 46, 52))                       # a low snow wall, and two children behind it
        g.box(ICE_D, (430, 71, 26), (260, 3, 6))
        for x in (380, 480):
            d = suit(r, big=0.58, hats=('cap', None), build='kid', hair='short')
            put(sc, r, x, 135, 4, d, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=5, rate=1.7, react=True)
    return build


# ------------------------------------------------------------------------------------------ LOW: the dog yard
def huskies(seed=103):
    """Sled dogs staked out on straw along a line, wagging, nodding at their bowls, two hopping; the musher and a
    child sitting among them, the loaded sled. 10 m long, built under 1.15 m: LOW."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        low_net(g, 1000.0, -105.0, col=TEAL, posts=8)
        g.box(DARK, (0, 30, 6), (860, 4, 4))                            # the stake-out line
        for k in range(6):
            x = -400 + k * 130
            g.box(YEL, (x, 10, 3), (96, 80, 6))
            g.box(RED, (x + 34, -36, 6), (20, 20, 10))
            husky(sc, r, x, 10, 6, yaw=r.uniform(-40, 40), s=r.uniform(1.15, 1.3), graze=(16.0 if k % 3 == 0 else 0.0),
                  bob=(6.0 if k % 3 == 1 else 0.0))
        d = suit(r, big=0.9, hats=('cap',), hair='short')
        put(sc, r, 420, 30, 3, d, legs='sit', seat=8.0, yaw=35, arm_r='fwd', swing_r=16, axis_r='x', arm_l='wave', swing_l=22, rate=0.9, react=True)
        d = suit(r, big=0.57, hats=('cap', None), build='kid', hair='short')
        put(sc, r, -470, 20, 6, d, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=5, rate=1.5, react=True)
        for sy in (-1, 1):                                              # the sled, loaded
            g.box(WOOD_D, (330, 110 + sy * 22, 6), (220, 6, 10))
        g.box(WOOD, (330, 110, 20), (200, 50, 8))
        for k, c in enumerate((RED, BLUE, YEL)):
            g.box(c, (270 + k * 56, 110, 42), (50, 44, 36))
    return build


# ------------------------------------------------------------------------------------------ TALL: the piste crew
def piste_crew(seed=111, top=ORANGE, band=LIME, net=ORANGE, gun=False):
    """A piste crew on a ploughed mound: two shovelling, one driving a post, one on the radio, a marshal waving a
    flag; a run of net half built behind them. gun: a snow gun on its stand, one of them at its controls.
    11 m long, about 4.5 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 110.0
        mound(g, 1100.0, 400.0, H)
        net_fence(g, -520, 120, 160, H, col=net)
        g.box(WOOD_D, (260, 150, H + 75), (12, 12, 150))                # the post being driven
        g.cyl(net, (430, 120, H), 34, 60, steps=8)                      # a roll of net, a stack of posts
        for k in range(3):
            g.box(WOOD, (-380 + k * 16, -120, H + 8 + k * 14), (220, 14, 12))
        put(sc, r, -330, -40, H, worker(r, top, band), yaw=-20, arm_l='carry', arm_r='carry', hold_r=hold_shovel(), swing_l=22, swing_r=22,
            axis_l='x', axis_r='x', rate=0.8)
        put(sc, r, -120, 20, H, worker(r, top, band), yaw=25, arm_l='carry', arm_r='carry', hold_r=hold_shovel(), swing_l=24, swing_r=24,
            axis_l='x', axis_r='x', rate=0.7)
        put(sc, r, 250, 95, H, worker(r, top, band), yaw=180, arm_r='up', hold_r=P.hold_tool(), swing_r=38, axis_r='x', arm_l='hip', rate=1.1)
        put(sc, r, 430, -30, H, worker(r, top, band, hat='beanie', hat_col=top), yaw=-10, arm_r='wave', hold_r=P.hold_flag(col=YEL, w=100.0),
            swing_r=26, arm_l='point', rate=1.0, bob=4, react=True, detail=2)
        if gun:
            g.box(STEEL_D, (70, -90, H + 60), (16, 16, 120))
            C.tripod(g, 70, -90, top=H + 70)
            m = sc.part(P.motion(rate=0.12, swing=24.0, pivot=(70.0, -90.0, H + 130.0), axis=P.UP))
            m.box(YEL, (70, -100, H + 140), (60, 130, 60))
            m.box(DARK, (70, -170, H + 140), (66, 14, 66))
            put(sc, r, 150, -60, H, worker(r, top, band), yaw=-60, arm_l='fwd', arm_r='hip', swing_l=10, axis_l='x', rate=0.5)
        else:
            put(sc, r, 80, -70, H, worker(r, top, band), yaw=10, arm_r='face', hold_r=P.hold_phone(), arm_l='point', swing_l=12, axis_l='x', rate=0.5)
            g.box(YEL, (-10, -130, H + 55), (90, 8, 70))               # a board of stripes on two legs
            g.box(DARK, (-10, -134.5, H + 55), (90, 1.5, 16))
            for sx in (-1, 1):
                g.box(DARK, (-10 + sx * 38, -128, H + 12), (6, 6, 24))
    return build


# ------------------------------------------------------------------------------------------ TALL: the ski school
def ski_school(seed=121, bib=YEL):
    """A ski school in a snake along a groomed bank: the instructor and six children in bibs, skis underfoot, sliding
    the bank's 14 m and coming round; two who have sat down in the snow, a parent filming. 14 m long, about 3.5 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, L = 90.0, 1400.0
        mound(g, L, 380.0, H, lumps=False)
        for k in range(5):                                              # gate markers along the back
            x = -560 + k * 280
            g.box(RED if k % 2 else BLUE, (x, 150, H + 55), (8, 8, 110))
            g.box(RED if k % 2 else BLUE, (x, 150, H + 96), (50, 4, 34))
        v = 130.0
        for k in range(7):
            x = 470 - k * 150
            kid = k > 0
            d = suit(r, big=(0.8 if kid else BIG), hats=('helmet',), band=(bib if kid else None), wear='trousers')
            if kid:
                d.update(build='kid')
            act = dict(yaw=90, legs='wide', arm_l='out', arm_r='out', swing_l=14, swing_r=-14, rock=7, rate=r.uniform(0.7, 1.1), travel=(L, v), detail=0)
            if not kid:
                act.update(arm_r='wave', swing_r=24, rock=0, detail=1)
            put(sc, r, x, -40 + (25 if k % 2 else -25), H + 5, d, **act)
            m = carried(sc, x, -40 + (25 if k % 2 else -25), L, v, pz=H + 3)
            c = r.choice(SUITS)
            for sy in (-1, 1):
                m.box(c, (x + 8, -40 + (25 if k % 2 else -25) + sy * 13, H + 3), (150 if not kid else 100, 9, 5))
        for x, yaw in ((-250, 15), (120, -20)):                         # two sitting in the snow
            d = suit(r, big=0.8, hats=('helmet',), band=bib, build='kid')
            put(sc, r, x, 80, H, d, legs='sit', seat=8.0, yaw=yaw, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, rate=1.3, react=True)
        put(sc, r, 520, 100, H, suit(r), yaw=-25, arm_l='hold', arm_r='hold', hold_r=P.hold_phone(), bob=-1.5, rate=0.3)
    return build


# ------------------------------------------------------------------------------------------ TALL: walkers on a bank
def hikers(seed=131, d1=1, tour=False):
    """Six walkers along the crest of a snow bank, packs on their backs, poles swinging: they walk its 16 m and come
    round again. tour: ski tourers, skis on their shoulders, a dog trotting with them. One has stopped to take a
    picture. d1: +1 towards +X, -1 towards -X. 16 m long, about 4 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, L = 120.0, 1600.0
        mound(g, L, 300.0, H)
        v = 95.0 * d1
        for k in range(6):
            x = -660 + k * 250 + r.uniform(-30, 30)
            d = suit(r, hats=('beanie', 'beanie', 'cap'), pack=r.choice(SUITS))
            if tour and k % 2 == 0:
                put(sc, r, x, -30, H, d, yaw=90 * d1, stride=22, arm_r='hold', hold_r=hold_skis(col=r.choice((RED, YEL, BLUE))), arm_l='down',
                    swing_l=18, axis_l='x', bob=3, rate=1.2, travel=(L, v))
            else:
                c = r.choice((DARK, RED, BLUE))
                put(sc, r, x, -30, H, d, yaw=90 * d1, stride=22, arm_l='carry', arm_r='carry', hold_l=hold_pole(c), hold_r=hold_pole(c),
                    swing_l=18, swing_r=-18, axis_l='x', axis_r='x', bob=3, rate=1.2, travel=(L, v))
        if tour:
            husky(sc, r, 80, -95, H, yaw=90 * d1, s=1.3, stride=24, travel=(L, v), wag=18.0)
        put(sc, r, 300 * d1, 95, H, suit(r), yaw=10, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), bob=-1.5, rate=0.3, detail=2)
        pennant(sc, -L / 2.0 + 60, 110, H, col=(TEAL if tour else PINK), phase=0.2)       # way marks at the ends
        pennant(sc, L / 2.0 - 60, 110, H, col=(YEL if tour else ORANGE), phase=0.6)
    return build


# ------------------------------------------------------------------------------------------ TALL: mountain rescue
def rescue(seed=141):
    """A mountain rescue team at drill on a mound: two hauling a loaded sled to and fro, a handler and a big dog,
    one on the radio pointing up the hill, one sounding the snow with a probe; their pennant. 11 m long, 4.5 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 110.0
        mound(g, 1150.0, 400.0, H)
        look = dict(top=RED, band=YEL, hat='helmet', hat_col=WHITE)
        slide = dict(swing=1.6, pivot=(-200.0, -60.0, -6000.0), axis=90.0)
        m = sc.part(P.motion(rate=0.1, phase=0.3, **slide))             # the sled: an orange tub on runners, its load strapped down
        m.box(ORANGE, (-200, -60, H + 26), (210, 62, 30))
        for sy in (-1, 1):
            m.box(DARK, (-200, -60 + sy * 24, H + 6), (230, 6, 10))
        m.box(BLUE, (-230, -60, H + 52), (90, 50, 26))
        m.box(YEL, (-140, -60, H + 50), (60, 46, 22))
        m.box(DARK, (-185, -60, H + 66), (8, 56, 3))
        put(sc, r, -50, -60, H, worker(r, **look), yaw=-90, arm_l='back', arm_r='back', rate=0.1, phase=0.3, sway=slide)
        put(sc, r, -350, -60, H, worker(r, **look), yaw=-90, arm_l='push', arm_r='push', rate=0.1, phase=0.3, sway=slide)
        put(sc, r, 200, 60, H, worker(r, **look), yaw=-15, arm_r='fwd', swing_r=10, axis_r='x', arm_l='hip', rate=0.6)
        P.quadruped(sc, 290, 10, H, yaw=-30, s=1.3, length=84, height=58, girth=30, neck=18, neck_up=30, head=24, tail=26, body='#b87a50',
                    belly=WHITE, snout=WHITE, legs_col=WHITE, wag=26.0, graze=10.0, rate=1.6, phase=r.random())
        put(sc, r, 440, -40, H, worker(r, **look), yaw=20, arm_r='face', hold_r=P.hold_phone(col=YEL, screen=DARK), arm_l='point', swing_l=12,
            axis_l='x', rate=0.5, detail=2)
        put(sc, r, -470, 90, H, worker(r, **look), yaw=30, arm_l='fwd', arm_r='fwd', hold_r=hold_probe(), swing_l=14, swing_r=14, axis_l='x',
            axis_r='x', rate=0.7)
        pennant(sc, 60, 150, H, height=380.0, col=ORANGE, phase=0.1)
        g.box(RED, (520, 110, H + 22), (70, 50, 44))
        g.box(YEL, (520, 110, H + 30), (72, 52, 8))
    return build


# ------------------------------------------------------------------------------------------ TALL: hot drinks
def hot_drinks(seed=151):
    """A hot-drinks hut on a timber deck: striped awning, the urn steaming, the server pouring, a queue of four with
    cups, two more at a barrel. 9.5 m long, about 5 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 110.0
        g.box(SNOW_SH, (0, 0, (H - 12 - FOOT) / 2.0), (960, 420, H - 12 + FOOT))
        g.box(WOOD, (0, 0, H - 6), (990, 450, 12))
        g.box(SNOW, (0, -231, H * 0.3 - FOOT / 2.0), (1040, 40, H * 0.6 + FOOT))
        # the hut at the -X end, its counter towards +X
        g.box(WOOD_D, (-400, 20, H + 110), (130, 300, 220))
        g.box(WOOD, (-250, 20, H + 100), (30, 260, 10))
        g.box(WOOD_D, (-250, 20, H + 48), (22, 250, 96))
        for k in range(6):
            g.box(PINK if k % 2 else WHITE, (-330, -130 + (k + 0.5) * 50, H + 236), (300, 50, 12))
        g.box(SNOW, (-370, 20, H + 246), (200, 310, 10))
        g.cyl(STEEL, (-250, 90, H + 105), 20, 44, steps=8)
        steam(sc, -250, 90, H + 150, phase=0.2)
        for k, c in enumerate((YEL, ORANGE, TEAL)):
            g.cyl(c, (-250, -60 + k * 40, H + 105), 9, 16, steps=6)
        put(sc, r, -306, 0, H, suit(r, hats=('beanie',), top=WHITE, band=PINK), yaw=90, arm_r='fwd', swing_r=12, axis_r='x', arm_l='hold', rate=0.7)
        for k in range(4):                                              # the queue, facing the hut
            waving = (k == 2)
            act = dict(yaw=10, arm_r='wave', swing_r=26, rate=1.1, bob=4, react=True) if waving else \
                dict(yaw=-90 + r.uniform(-15, 15), arm_r='hold', hold_r=P.hold_cup(), arm_l='down', bob=-1.5, rate=0.4)
            put(sc, r, -150 + k * 115, -20 + (k % 2) * 40, H, suit(r), **act)
        g.cyl(WOOD_D, (360, 60, H), 34, 100, steps=8)                   # a barrel for a table
        g.cyl(WOOD, (360, 60, H + 100), 46, 6, steps=8)
        put(sc, r, 290, 60, H, suit(r), yaw=90, arm_r='face', hold_r=P.hold_cup(), swing_r=12, axis_r='x', rate=0.3, detail=2)
        put(sc, r, 430, 60, H, suit(r), yaw=-90, arm_l='face', hold_l=P.hold_cup(), arm_r='hip', rock=4, rate=0.4)
        for sx in (150, 470):                                           # skis stood in a rack at the back
            g.box(STEEL_D, (sx, 200, H + 60), (8, 8, 120))
        g.box(STEEL_D, (310, 200, H + 110), (330, 8, 8))
        for k, c in enumerate((RED, YEL, BLUE, LIME, PINK, TEAL)):
            g.box(c, (180 + k * 52, 192, H + 95), (12, 5, 190))
    return build


# ------------------------------------------------------------------------------------------ TALL: the climbers' camp
def camp(seed=161):
    """A climbers' camp on a levelled platform: two dome tents, one climber coiling a rope, one at the stove, one
    pointing the route with an axe, one jumping to keep warm; pennants on a line, skis stuck in the snow.
    10 m long, about 4.5 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 100.0
        mound(g, 1050.0, 440.0, H)
        for x, y, rad, c, c2 in ((-330, 90, 115, ORANGE, YEL), (310, 100, 100, TEAL, WHITE)):
            g.sphere(c, (x, y, H), rad, steps=(5, 8))
            g.cyl(c2, (x, y, H + rad * 0.22), rad * 0.99, rad * 0.2, steps=8)
            g.box(DARK, (x, y - rad * 0.86, H + rad * 0.3), (rad * 0.6, 14, rad * 0.6))
        a, b = (-480, 190, H + 260), (470, 190, H + 260)
        for p in (a, b):
            g.box(WOOD_D, (p[0], p[1], H + 130), (9, 9, 260))
        tilt_box(g, DARK, a, b, 4, 4)
        for i in range(8):
            x = a[0] + (i + 0.5) * (b[0] - a[0]) / 8.0
            m = sc.part(P.motion(phase=i * 0.17, rate=0.9, swing=16.0, pivot=(x, 190.0, H + 258.0), axis=0.0))
            m.box((RED, YEL, TEAL, PINK)[i % 4], (x, 190, H + 230), (50, 3, 56))
        put(sc, r, -110, -60, H, suit(r, hats=('beanie', 'helmet')), yaw=-10, arm_l='hold', arm_r='hold', swing_l=16, swing_r=-16, axis_l='z',
            axis_r='z', rate=1.0)
        g.cyl(RED, (-110, -110, H), 26, 12, steps=8)                    # the coil of rope at their feet
        g.box(STEEL, (60, 20, H + 20), (46, 40, 40))                    # the stove on a crate, and the cook on another
        g.cyl(DARK, (60, 20, H + 40), 16, 18, steps=8)
        steam(sc, 60, 20, H + 62, phase=0.5)
        g.box(WOOD, (140, 40, H + 23 * BIG), (50, 44, 46 * BIG))
        put(sc, r, 140, 40, H, suit(r, hats=('beanie',)), yaw=-70, legs='sit', seat=46.0, arm_l='fwd', arm_r='hold', hold_r=P.hold_cup(),
            swing_l=10, axis_l='x', rate=0.5)
        put(sc, r, 420, -70, H, suit(r, hats=('helmet',)), yaw=15, arm_r='point', hold_r=hold_axe(), swing_r=12, axis_r='x', arm_l='hip', rate=0.6,
            detail=2)
        put(sc, r, -440, -70, H, suit(r, hats=('beanie',)), yaw=-15, arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=14, rate=1.3, react=True)
        for k, c in enumerate((RED, BLUE, YEL)):                        # skis and poles stuck in the snow, packs
            g.box(c, (130 + k * 28, 170, H + 85), (10, 5, 190))
        for k, c in enumerate((VIOLET, LIME)):
            g.box(c, (-60 + k * 70, 130, H + 26), (44, 30, 52))
    return build


# ------------------------------------------------------------------------------------------ TALL: the surveyors
def surveyors(seed=171):
    """An avalanche survey on a mound: one at the instrument on its tripod, one holding the red-and-white staff, one
    sounding the snow, one writing it down; a weather mast with its cups turning. 9 m long, about 6 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 120.0
        mound(g, 950.0, 380.0, H)
        look = dict(top=YEL, band=BLUE, hat='beanie', hat_col=BLUE)
        g.box(STEEL_D, (-150, -40, H + 60), (10, 10, 120))
        C.tripod(g, -150, -40, top=H + 150)
        g.box(YEL, (-150, -40, H + 166), (30, 44, 30))
        g.cyl(DARK, (-150, -62, H + 166), 8, 12, steps=6)
        put(sc, r, -225, -40, H, worker(r, **look), yaw=90, arm_l='hold', arm_r='hold', rock=3, rate=0.3, detail=2)
        put(sc, r, 330, -30, H, worker(r, **look), yaw=-60, arm_r='fwd', hold_r=hold_staff(), arm_l='hip', rock=4, rate=0.4)
        put(sc, r, 90, 80, H, worker(r, **look), yaw=20, arm_l='fwd', arm_r='fwd', hold_r=hold_probe(col=ORANGE), swing_l=16, swing_r=16,
            axis_l='x', axis_r='x', rate=0.7)
        put(sc, r, -20, -90, H, worker(r, **look), yaw=-25, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), bob=-1.5, rate=0.3)
        mx, my = -380.0, 120.0                                          # the weather mast
        g.box(STEEL, (mx, my, H + 230), (9, 9, 460))
        g.box(ORANGE, (mx, my, H + 300), (50, 40, 60))
        m = sc.part(P.motion(base=(mx, my), travel=(-1.0, 0.45)))
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            m.box(DARK, (mx + dx * 30, my + dy * 30, H + 462), (64 if dx else 5, 64 if dy else 5, 5))
            m.box(RED, (mx + dx * 62, my + dy * 62, H + 462), (24, 24, 24))
        pennant(sc, 420, 130, H, height=300.0, col=RED, phase=0.4)
        g.box(BLUE, (200, -130, H + 20), (80, 50, 40))
    return build


# ------------------------------------------------------------------------------------------ TALL: the igloo builders
def igloo(seed=181):
    """An igloo going up on a mound, in blocks of blue-shadowed snow: one builder lifting a block over their head,
    one carrying one, a child waving from the door, one sawing blocks out of the bank; flags on the dome.
    9.5 m long, about 4.5 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 90.0
        mound(g, 980.0, 440.0, H)
        cx, cy, R = -200.0, 70.0, 170.0
        g.sphere(ICE, (cx, cy, H), R, steps=(5, 10))
        for k in range(3):                                              # courses of blocks
            g.cyl(ICE_D, (cx, cy, H + 40 + k * 44), R * (0.99 - k * k * 0.07), 4, steps=10)
        g.box(ICE, (cx, cy - R - 10, H + 45), (110, 90, 90))            # the porch and its dark door
        g.box(DARK, (cx, cy - R - 56, H + 36), (66, 4, 72))
        pennant(sc, cx, cy, H + R - 6, height=150.0, col=RED, phase=0.3)
        d = suit(r, big=0.8, hats=('beanie',), build='kid')
        put(sc, r, cx + 95, cy - R - 40, H, d, yaw=-10, arm_r='wave', swing_r=28, bob=8, rate=1.4, react=True)
        for k in range(4):                                              # cut blocks waiting
            g.box(ICE, (260 + (k % 2) * 62, 150 - (k // 2) * 44, H + 16), (56, 38, 32))
        put(sc, r, 60, -40, H, suit(r), yaw=70, arm_l='carry', arm_r='carry', hold_r=hold_block(), bob=3, rate=0.8)
        put(sc, r, 240, 40, H, suit(r), yaw=110, arm_l='up', arm_r='up', hold_r=hold_block(), swing_l=8, swing_r=8, axis_l='x', axis_r='x',
            bob=5, rate=0.6)
        put(sc, r, 420, -60, H, suit(r), yaw=20, arm_r='fwd', hold_r=P.hold_tool(col=STEEL, grip=RED, length=60.0), swing_r=24, axis_r='x',
            arm_l='hip', rate=1.3)
        put(sc, r, -430, -70, H, suit(r), yaw=-20, arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=10, rate=1.2, react=True, detail=2)
        g.box(RED, (410, 150, H + 10), (120, 40, 8))                    # a sledge for the blocks
    return build


PLINTH = 40.0        # fix round, 16 Nov: the snow plinth under the three low pieces


def on_plinth(build, length, depth, y=0.0):
    """A low piece as built, lifted PLINTH cm onto a level plinth of packed snow that goes FOOT into the ground.
    (Fix round: the hill falls 7 in 100 along the road and these pieces, 9 to 12 m long, stood on z = 0 with no
    foot: in the game the uphill end was buried to the shoulders. Still LOW: 1.11 + 0.4 m.)"""
    import unreal

    def f(sc):
        build(sc)
        up = unreal.Vector(0.0, 0.0, PLINTH)
        for key in sc.order:
            model, mo = sc.parts[key]
            for pm in model.meshes.values():
                unreal.GeometryScript_MeshTransforms.translate_mesh(pm, up)
            px, py, pz = mo['pivot']
            mo['pivot'] = (px, py, pz + PLINTH)
        mound(sc.still(), length, depth, PLINTH, y=y, lumps=False)
    return f


JOBS = [
    ('SM_P3A_SnowSeat_A', on_plinth(snow_seat(101), 960.0, 290.0, 10.0), LOW_SAVE),
    ('SM_P3A_SledKids_A', on_plinth(sled_kids(102), 1260.0, 320.0, 15.0), LOW_SAVE),
    ('SM_P3A_Huskies_A', on_plinth(huskies(103), 1060.0, 300.0, 10.0), LOW_SAVE),
    ('SM_P3A_PisteCrew_A', piste_crew(111)),
    ('SM_P3A_PisteCrew_B', piste_crew(112, top=YEL, band=ORANGE, net=PINK, gun=True)),
    ('SM_P3A_SkiSchool_A', ski_school(121)),
    ('SM_P3A_Hikers_A', hikers(131, 1, False)),
    ('SM_P3A_Hikers_B', hikers(132, -1, True)),
    ('SM_P3A_Rescue_A', rescue(141), dict(grow=(220.0, 60.0, 60.0), grow_down=(220.0, 60.0, 0.0))),
    ('SM_P3A_HotDrinks_A', hot_drinks(151)),
    ('SM_P3A_Camp_A', camp(161)),
    ('SM_P3A_Surveyors_A', surveyors(171)),
    ('SM_P3A_Igloo_A', igloo(181)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
