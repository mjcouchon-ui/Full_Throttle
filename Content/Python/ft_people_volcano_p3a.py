"""FULL THROTTLE (Unreal) the population of level 3, the Volcano: pass 3, builder p3a, "gaps" (/Game/People/Volcano/SM_P3A_*).

The general cast of the mountain, for the stretches of road where nobody stood within 60 m: people at work (the survey
sampling the lava, a pipe gang, an ash gang, a sandbag chain), at rest (a tea break, a washing line and a card game) and
passing through (villagers with a handcart one way, porters with a mule the other).

Where they stand (C:/ftwork/p3/L3a.inl): on the FAR BANKS of the two lava channels, 54 m out on the left and 62.7 m on
the right, on the slots the basalt walls leave free, so the channel's glow lies in front of them; on the near bank of
the right-hand channel (43 m) beside the banked bends, where the land nearer the road is cut away; and, half as big
again, on basalt knolls 89 m out. The land lies 0.75 m under the road and a kerb of basalt stubs stands 21 m out, so
every bank piece brings a narrow trestle deck 1.4 m up (open underneath, 2.3 m deep: it fits between a channel and
what stands behind it). All of them are TALL: nothing here is written nearer than 42 m.

No vehicle and nothing red (the boss is ENGINE 13, a red fire engine); no orange crate and no green toolbox (the
pickups); no light that blinks. Every colour carries a little steady light of its own (V.lifted: the slopes are dark).

Run in the editor:  import ft_people_volcano_p3a as K; K.run()        K.run(['SM_P3A_Dike'])
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
import ft_people_volcano as V
from ft_desert import rod, tilt_box

DIR = '/Game/People/Volcano'
H = 140.0            # the bank deck
WB = 230.0           # its depth
PLANK, PLANK_D, STEEL, STEEL_D = '#e8d8b8', '#c9a070', '#8f9bb3', '#5a5870'
PALE, WHITE, DARK, YEL, LIME, PINK = '#b0a088', '#fff6ea', '#22202a', '#ffc93c', '#b8ff5a', '#ff5fa2'
ORANGE, CYAN, SKY, VIOLET, SILVER, WOOD = '#ff7a3c', '#2ee6ff', '#57c7ff', '#8f6bff', '#d7e3f0', '#c9a070'
SACK, SACK_D = '#f1e4c8', '#d9c8a0'
CLOTH = V.CLOTH
LIFT = 0.04


# ------------------------------------------------------------------------------------------ who
def worker(r, **fixed):
    """The mountain's works gangs: orange, yellow or blue, a hard hat, a white band (the film crew's band is pink, the
    fire crews' coats yellow with lime)."""
    d = P.look(r, tops=(ORANGE, YEL, SKY), bottoms=('#2b2350', '#3a5fa8', '#5a4a3c'), wear='trousers', sleeves=r.choice(('short', 'long')),
               hat='hard', hat_col=r.choice((YEL, WHITE, CYAN)), band=WHITE)
    d.update(fixed)
    return d


def stand(sc, r, x, y, who, z=H, **kw):
    d = dict(detail=1, phase=r.random())
    d.update(who)
    d.update(kw)
    return P.figure(sc, x, y, z, **d)


def walk(sc, r, x, y, L, v, who, z=H, **kw):
    """Someone walking the deck's length (v cm/s: negative walks towards -X) and coming round again."""
    d = dict(stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=abs(v) / 110.0, bob=2.0, detail=1, phase=r.random())
    d.update(who)
    d.update(kw)
    return P.figure(sc, x, y, z, yaw=90 if v > 0 else -90, travel=(L, v), **d)


# ------------------------------------------------------------------------------------------ what they hold
def hold_ladle(length=330.0):
    """A long sampling pole held out over the lava, a cup hanging at its end."""
    def f(m, h, side):
        if side < 0:
            return
        a = (h[0], h[1] + 40, h[2] + 10)
        b = (h[0], h[1] - length, h[2] - 10)
        tilt_box(m, STEEL, a, b, 5, 5)
        m.box(DARK, (b[0], b[1], b[2] - 28), (4, 4, 56))
        m.cyl(DARK, (b[0], b[1], b[2] - 74), 13, 20, steps=6)
        m.box(YEL, (b[0], b[1], b[2] - 52), (16, 16, 5))
    return f


def hold_cloth(col, w=120.0, h2=70.0):
    """A big cloth on a stick, waved at the chase."""
    def f(m, h, side):
        m.box(STEEL, (h[0], h[1], h[2] + 70), (4, 4, 170))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + 120), (w, 3, h2))
    return f


def hold_shovel():
    def f(m, h, side):
        if side < 0:
            return
        tilt_box(m, WOOD, (h[0] - 16, h[1] + 30, h[2] + 44), (h[0] - 16, h[1] - 44, h[2] - 58), 5, 5)
        m.box(STEEL, (h[0] - 16, h[1] - 52, h[2] - 70), (24, 26, 5))
    return f


def hold_broom():
    def f(m, h, side):
        if side < 0:
            return
        tilt_box(m, WOOD, (h[0] - 16, h[1] + 20, h[2] + 50), (h[0] - 16, h[1] - 50, h[2] - 96), 5, 5)
        m.box(YEL, (h[0] - 16, h[1] - 54, h[2] - 104), (46, 12, 16))
    return f


# ------------------------------------------------------------------------------------------ furniture
def tripod(g, x, y, z, top=128.0, col=STEEL):
    for a in (90, 210, 330):
        rod(g, col, (x, y, z + top), (x + math.cos(math.radians(a)) * 46, y + math.sin(math.radians(a)) * 46, z), 3.5, steps=5)


def windsock(sc, g, x, y, z0, top, cols=(ORANGE, WHITE), length=170.0):
    g.box(STEEL, (x, y, (z0 + top) / 2.0), (6, 6, top - z0))
    m = sc.part(P.motion(swing=24.0, pivot=(x, y, top - 20), axis=P.UP, rate=0.4, phase=0.3))
    n = 4
    for k in range(n):
        w = 40.0 - k * 7.0
        m.box(cols[k % 2], (x + 12 + (k + 0.5) * length / n, y, top - 20), (length / n, w, w))


def anemometer(sc, g, x, y, z0, top):
    g.box(STEEL, (x, y, (z0 + top) / 2.0), (5, 5, top - z0))
    m = V.spin(sc, x, y, 0.9)
    for a in (0, 120, 240):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(m, STEEL, (x, y, top), (x + ca * 48, y + sa * 48, top), 4, 4)
        m.sphere(WHITE, (x + ca * 54, y + sa * 54, top), 11, steps=(3, 6))


def crate(g, x, y, z, w=50.0, d=44.0, h=46.0, col='#7a8fb8'):
    """A seat or a table: pale blue or wood (never the pickups' orange or green)."""
    g.box(col, (x, y, z + h / 2.0), (w, d, h))
    g.box(WHITE, (x, y, z + h * 0.72), (w + 2, d + 2, 4))


def sacks(g, x, y, z, nx=3, nz=2, w=60.0, d=36.0, h=22.0):
    for k in range(nz):
        for i in range(nx - (k % 2)):
            g.box(SACK if (i + k) % 2 else SACK_D, (x + (i - (nx - 1 - (k % 2)) / 2.0) * (w + 3), y, z + (k + 0.5) * h), (w, d, h - 2))


def wheelbarrow(sc, x, y, L, v, z=H):
    """A barrow of ash travelling with whoever pushes it (stand them 80 cm behind)."""
    m = sc.part(P.motion(travel=V.tv(L, v), base=(x, y), pivot=(x, y, z + 40)), (x, y, z), 0.0, 1.0)
    d = 1 if v > 0 else -1
    m.box(SKY, (0, 0, 52), (84, 58, 34))
    m.sphere(PALE, (0, 0, 72), 30, scale=(1.2, 0.9, 0.6), steps=(3, 6))
    m.cyl(DARK, (d * 48, 5, 22), 22, 10, rot=(0, 0, 90), steps=8)
    for sy in (-1, 1):
        m.box(STEEL, (-d * 66, sy * 24, 62), (60, 5, 5))
        m.box(STEEL, (-d * 30, sy * 24, 18), (5, 5, 36))


# ------------------------------------------------------------------------------------------ at work: the survey
def sample_a(sc):
    """The survey samples the lava: a heat suit with a long pole out over the channel, a thermal camera panning, one
    writing, one carrying samples along the deck, one waving the chase through."""
    r = random.Random(301)
    g = sc.still()
    L = 820.0
    V.trestle(g, L, WB, h=H, bays=410.0, stripe=(YEL, DARK))
    heat = P.look(r, tops=(SILVER,), bottoms=(SILVER,), sleeves='long', wear='trousers', hat='helmet', hat_col=SILVER, band=YEL, shoes=DARK)
    stand(sc, r, -250, 10, heat, legs='stride', arm_r='fwd', arm_l='hip', hold_r=hold_ladle(), swing_r=5, axis_r='x', rate=0.3)
    tx, ty = -30.0, -40.0
    tripod(g, tx, ty, H)
    pan = dict(swing=24.0, pivot=(tx, ty, H), axis=P.UP)
    cam = sc.part(P.motion(rate=0.12, phase=0.2, **pan))
    cam.box(DARK, (tx, ty - 4, H + 142), (30, 52, 28))
    cam.box(YEL, (tx, ty - 32, H + 142), (22, 8, 20))
    stand(sc, r, tx, ty + 54, V.sci_look(r), arm_l='push', arm_r='push', rate=0.12, phase=0.2, sway=pan)
    stand(sc, r, 130, 0, V.sci_look(r), yaw=-18, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.4, bob=-1.5)
    stand(sc, r, 320, -40, V.sci_look(r), yaw=14, arm_r='wave', swing_r=26, arm_l='hip', rate=1.2, bob=4, react=True)
    crate(g, 235, 22, H, w=110, d=50, h=50, col=WHITE)           # the sample rack (the walker's lane behind it stays clear)
    for k, col in enumerate((PINK, CYAN, YEL, VIOLET)):
        g.cyl(col, (195 + k * 27, 22, H + 50), 9, 22, steps=6)
    walk(sc, r, -120, 80, L - 80, 75.0, V.sci_look(r), arm_r='carry', arm_l='carry', swing_l=0, swing_r=0, hold_r=P.hold_box(WHITE, 40.0))


def sample_b(sc):
    """The survey's instruments: a seismograph table with two sitting at it, an anemometer spinning, a windsock, one
    scanning the mountain through binoculars, one jumping and pointing at the chase."""
    r = random.Random(302)
    g = sc.still()
    L = 860.0
    V.trestle(g, L, WB, h=H, bays=430.0)
    g.box(PLANK_D, (-70, 5, H + 72), (230, 70, 8))
    for sx in (-1, 1):
        g.box(STEEL_D, (-70 + sx * 100, 5, H + 34), (8, 60, 68))
    g.box(DARK, (-130, 5, H + 92), (64, 50, 32))
    g.box(WHITE, (-130, -21, H + 92), (50, 3, 20))
    drum = V.spin(sc, -30, 5, 0.25)
    drum.cyl(WHITE, (-30, 5, H + 76), 20, 30, steps=8)
    drum.box(PINK, (-30, -15, H + 92), (4, 4, 26))
    for x in (-130, -20):
        crate(g, x, 84, H)
        stand(sc, r, x, 84, V.sci_look(r), legs='sit', seat=46, arm_l='carry', arm_r='fwd', swing_r=10, axis_r='x', rate=0.5)
    anemometer(sc, g, -380, 70, H, H + 330)
    windsock(sc, g, 390, 70, H, H + 360)
    stand(sc, r, 150, -30, V.sci_look(r), yaw=10, arm_l='face', arm_r='face', turn=22, rate=0.2)
    stand(sc, r, 290, -20, V.sci_look(r), yaw=-12, arm_r='point', arm_l='up', swing_l=14, rate=1.4, bob=8, react=True)
    stand(sc, r, -300, -30, worker(r), yaw=16, arm_r='wave', swing_r=24, rate=1.1, bob=3, react=True)


# ------------------------------------------------------------------------------------------ at rest
def rest_a(sc):
    """A tea break under a parasol: three on a bench with cups, a kettle steaming, one stretching, a child running
    rings round the deck with a dog after it."""
    r = random.Random(303)
    g = sc.still()
    L = 820.0
    V.trestle(g, L, WB, h=H, bays=410.0)
    g.box(PLANK_D, (-190, 62, H + 44), (290, 40, 8))
    for x in (-310, -70):
        g.box(STEEL_D, (x, 62, H + 20), (10, 36, 40))
    looks = [worker(r), V.villager(r), worker(r)]
    for k, x in enumerate((-290, -190, -90)):
        if k == 1:
            stand(sc, r, x, 62, looks[k], legs='sit', seat=48, arm_r='wave', swing_r=24, arm_l='down', rate=1.1, react=True)
        else:
            stand(sc, r, x, 62, looks[k], legs='sit', seat=48, arm_r='face', hold_r=P.hold_cup(), swing_r=9, axis_r='x', arm_l='down', rate=0.3)
    px, py = -240.0, 90.0                                       # the parasol's pole, behind the bench between two sitters
    g.box(STEEL, (px, py, H + 130), (6, 6, 260))
    par = sc.part(P.motion(swing=3.0, pivot=(px, py, H), axis=0.0, rate=0.25))
    par.cyl(PINK, (px, py - 20, H + 236), 112, 40, steps=8, top_r=10)
    par.cyl(WHITE, (px, py - 20, H + 230), 116, 7, steps=8)
    g.box(DARK, (40, 70, H + 28), (50, 40, 56))
    g.cyl(STEEL, (40, 70, H + 56), 16, 22, steps=6, top_r=11)
    for k in range(2):
        st = sc.part(P.motion(bob=-16.0, rate=0.45, phase=k * 0.5))
        st.sphere(WHITE, (40 + k * 12, 70, H + 112 + k * 26), 12 + k * 3, steps=(3, 5))
    stand(sc, r, 130, -10, worker(r), yaw=-8, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=0.4)
    cx, cy, rad = 285.0, 10.0, 70.0
    P.figure(sc, cx + rad, cy, H, yaw=180, orbit=(cx, cy, 0.22), stride=26, swing_l=20, swing_r=-20, axis_l='x', axis_r='x', rate=2.2, bob=3, detail=1,
             **V.villager(r, build='kid', s=0.68, top=YEL))
    th = -55.0
    P.quadruped(sc, cx + rad * math.cos(math.radians(th)), cy + rad * math.sin(math.radians(th)), H, yaw=180 + th, length=70, height=48, girth=20,
                neck=16, neck_up=35, head=18, tail=22, wag=30, stride=22, rate=2.4, orbit=(cx, cy, 0.22), body='#e8c26a', ears='#8e5a3a')


def rest_b(sc):
    """A washing line the length of the deck, bright cloths swinging on it, one hanging more; three at cards round a
    crate; a goat; one at the rail shading the eyes and pointing at the chase."""
    r = random.Random(304)
    g = sc.still()
    L = 860.0
    V.trestle(g, L, WB, h=H, bays=430.0)
    ly, lz = 96.0, H + 200.0
    for x in (-390, 390):
        g.box(WOOD, (x, ly, H + 104), (7, 7, 208))
    g.box(WHITE, (0, ly, lz), (780, 4, 4))
    cols = list(CLOTH)
    r.shuffle(cols)
    for k in range(8):
        x = -330 + k * 94
        w, hh = r.uniform(52, 76), r.uniform(60, 96)
        m = sc.part(P.motion(swing=13.0, pivot=(x, ly, lz), axis=0.0, rate=0.5, phase=r.random()))
        m.box(cols[k % len(cols)], (x, ly, lz - hh / 2.0), (w, 3, hh))
    crate(g, -170, 0, H, w=70, d=70, h=52, col=WOOD)
    seats = ((-262, 0, 90), (-78, 0, -90), (-170, 86, 0))
    for k, (x, y, yaw) in enumerate(seats):
        crate(g, x, y, H, w=44, d=44, h=40)
        stand(sc, r, x, y, V.villager(r) if k else worker(r), yaw=yaw, legs='sit', seat=40, arm_r='fwd', swing_r=14, axis_r='x', arm_l='carry', rate=0.4)
    stand(sc, r, 130, 52, V.villager(r, wear='dress'), yaw=180, arm_l='up', arm_r='up', swing_l=8, swing_r=-8, rate=0.5)
    stand(sc, r, 30, -62, V.villager(r), yaw=6, arm_r='point', arm_l='face', rate=1.0, bob=3, react=True)
    V.goat(sc, r, 300, -20, H, yaw=90, graze=18, wag=30, rate=0.5, phase=r.random())


# ------------------------------------------------------------------------------------------ passing through
def passing(seed, v, porters=False):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L = 1000.0
        TL = L - 70.0
        V.trestle(g, L, WB, h=H, bays=500.0)
        xs = [-400, -240, -80, 80, 240, 400]
        ya, yb = -52.0, 78.0
        who = (lambda: worker(r, pack=r.choice(CLOTH))) if porters else (lambda: V.villager(r))
        carries = [dict(arm_r='up', swing_r=0, hold_r=V.hold_bundle(r.choice(CLOTH))),
                   dict(hold_r=P.hold_bag(r.choice(CLOTH)), hold_l=P.hold_bag(r.choice(CLOTH)), swing_r=-6, swing_l=6),
                   dict(arm_r='face', swing_r=0, hold_r=V.hold_roll(r.choice(CLOTH))),
                   dict(arm_l='wave', swing_l=24, axis_l='y'),
                   dict(arm_r=((36, 70), (72, 84)), swing_r=0, hold_r=V.hold_staff())]
        r.shuffle(carries)
        d = 1 if v > 0 else -1
        for i, x in enumerate(xs):
            if i == 2:
                if porters:      # the mule and its packs
                    P.quadruped(sc, x, 14, H, yaw=90 * d, length=120, height=100, girth=38, neck=40, neck_up=48, head=34, body='#a8703a',
                                mane='#3b2a22', tail=34, stride=20, rate=abs(v) / 110.0, phase=r.random(), travel=(TL, v))
                    pk = sc.part(P.motion(travel=V.tv(TL, v), base=(x, 14.0), pivot=(x, 14.0, H + 100)))
                    for sy in (-1, 1):
                        pk.box(r.choice(CLOTH), (x, 14 + sy * 28, H + 92), (70, 22, 40))
                    pk.sphere(r.choice(CLOTH), (x, 14, H + 124), 28, scale=(1.4, 1.0, 0.7), steps=(4, 7))
                else:            # the handcart and whoever pushes it
                    V.handcart(sc, r, x, 14, TL, v, z=H)
                    walk(sc, r, x - 95 * d, 14, TL, v, who(), arm_l='push', arm_r='push', swing_l=0, swing_r=0)
                continue
            if i == 3 and not porters:
                continue                                        # room for the cart
            y = ya if i % 2 else yb
            walk(sc, r, x, y, TL, v, who(), detail=1 if i % 2 else 0, **carries[i % len(carries)])
            if i == 4:           # a child beside a grown-up
                walk(sc, r, x + 46, y + (40 if y < 0 else -40), TL, v, V.villager(r, build='kid', s=r.uniform(0.6, 0.7)), arm_l='wave', swing_l=24, axis_l='y',
                     detail=0)
        if porters:
            for k in range(2):
                V.goat(sc, r, xs[0] - 10 + k * 90, ya + 20 + k * 90, H, yaw=90 * d, stride=22, wag=30, rate=abs(v) / 60.0, phase=r.random(),
                       travel=(TL, v), bob=2.0)
    return build


# ------------------------------------------------------------------------------------------ at work: the gangs
def mend_a(sc):
    """A pipe gang: a fat pipe along the front of the deck, one hammering at a flange, one at the valve (its wheel
    turns), the foreman pointing with a clipboard, one waving, one walking a length of pipe along the back."""
    r = random.Random(307)
    g = sc.still()
    L = 860.0
    V.trestle(g, L, WB, h=H, bays=430.0, stripe=(ORANGE, WHITE))
    py, pz = -52.0, H + 36.0
    rod(g, STEEL, (-400, py, pz), (400, py, pz), 26, steps=8)
    for x in (-250, 0, 250):
        rod(g, YEL, (x - 8, py, pz), (x + 8, py, pz), 34, steps=8)
        g.box(STEEL_D, (x + 60, py, H + 6), (16, 60, 12))
    g.box(STEEL, (110, py, pz + 46), (6, 6, 44))
    wh = V.spin(sc, 110, py, 0.3)
    wh.cyl(YEL, (110, py, pz + 66), 28, 6, steps=8)
    wh.box(DARK, (110, py, pz + 73), (56, 5, 3))
    stand(sc, r, -250, 22, worker(r), arm_r='fwd', hold_r=P.hold_tool(), swing_r=28, axis_r='x', arm_l='hip', rate=1.6)
    stand(sc, r, 110, 24, worker(r), arm_l='fwd', arm_r='fwd', rock=4, rate=0.8)
    stand(sc, r, -70, 40, worker(r, hat_col=WHITE), yaw=-30, arm_r='point', swing_r=8, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.5)
    stand(sc, r, 320, 20, worker(r), yaw=12, arm_r='wave', swing_r=26, arm_l='hip', rate=1.2, bob=4, react=True)
    walk(sc, r, 200, 86, L - 80, -70.0, worker(r), arm_r='face', swing_r=0, hold_r=V.hold_roll(STEEL, STEEL_D))


def mend_b(sc):
    """An ash gang clearing the fall: two shovelling at a heap, one sweeping, a barrow of ash wheeled along the back,
    sacks stacked, one drinking, one waving."""
    r = random.Random(308)
    g = sc.still()
    L = 860.0
    V.trestle(g, L, WB, h=H, bays=430.0)
    g.sphere(PALE, (-250, 10, H + 8), 92, scale=(1.3, 0.6, 0.5), steps=(4, 7))
    stand(sc, r, -350, 30, worker(r), yaw=-50, arm_l='carry', arm_r='carry', hold_r=hold_shovel(), rock=7, rate=0.7)
    stand(sc, r, -140, 30, worker(r), yaw=50, arm_l='carry', arm_r='carry', hold_r=hold_shovel(), rock=7, rate=0.7)
    stand(sc, r, 20, -30, worker(r), yaw=20, arm_l='carry', arm_r='carry', hold_r=hold_broom(), turn=16, rate=0.6)
    sacks(g, 330, 14, H, nx=3, nz=3)
    stand(sc, r, 200, 10, worker(r), yaw=-10, arm_r='face', hold_r=P.hold_cup(), swing_r=8, axis_r='x', arm_l='hip', rate=0.3)
    stand(sc, r, 340, -40, V.villager(r), yaw=14, arm_r='wave', swing_r=26, arm_l='up', swing_l=-12, rate=1.3, bob=5, react=True)
    TL = L - 90.0
    wheelbarrow(sc, -60, 84, TL, 80.0)
    walk(sc, r, -140, 84, TL, 80.0, worker(r), arm_l='push', arm_r='push', swing_l=0, swing_r=0)


def dike(sc):
    """A sandbag chain: seven hand sacks down the line, each a beat behind the last, from the pile to the wall going up
    at the far end; the foreman on the wall waves a yellow flag at the chase."""
    r = random.Random(309)
    g = sc.still()
    L = 1100.0
    V.trestle(g, L, WB, h=H, bays=550.0, stripe=(SACK, PLANK_D))
    sacks(g, -470, 40, H, nx=2, nz=3)
    for row in range(3):
        n = 6 - row
        for i in range(n):
            g.box(SACK if (i + row) % 2 else SACK_D, (260 + (i + 0.5 * row) * 46, -60, H + 11 + row * 22), (44, 34, 20))
    for i in range(7):
        x = -390 + i * 88
        who = worker(r) if i % 3 else V.villager(r)
        stand(sc, r, x, 16 + (10 if i % 2 else -10), who, yaw=(24 if i % 2 else -24), arm_l='carry', arm_r='carry', hold_r=P.hold_box(SACK, 44.0),
              rock=7, rate=0.6, phase=i * 0.14)
    stand(sc, r, 300, 20, worker(r), yaw=0, arm_l='fwd', arm_r='fwd', swing_l=16, swing_r=16, axis_l='x', axis_r='x', rate=0.6, phase=0.98)
    stand(sc, r, 470, 30, worker(r, hat_col=WHITE), yaw=18, arm_r='up', hold_r=hold_cloth(YEL, 90.0, 56.0), swing_r=18, arm_l='hip', rate=0.9, bob=3,
          react=True, detail=1)


# ------------------------------------------------------------------------------------------ the knolls, 89 m out
KNOLL_A = [(-420, 10, 120, 170), (-215, -20, 135, 265), (0, 20, 150, 365), (230, -10, 135, 285), (430, 30, 118, 185), (-110, 205, 130, 305),
           (135, 215, 125, 245)]
KNOLL_B = [(-400, 20, 118, 200), (-190, -10, 140, 330), (40, 30, 135, 250), (250, -15, 145, 390), (450, 25, 115, 215), (-60, 210, 130, 290),
           (190, 220, 125, 330)]


def knoll_a(sc):
    """Villagers camped on a knoll of basalt columns, their bundles beside them, waving big cloths at the chase; goats."""
    r = random.Random(310)
    g = sc.still()
    V.crag(g, r, KNOLL_A)
    top = dict((k, c[3] + 8.0) for k, c in enumerate(KNOLL_A))
    c = KNOLL_A
    stand(sc, r, c[0][0], c[0][1], V.villager(r, build='kid', s=0.7, top=YEL), z=top[0], arm_r='wave', swing_r=26, arm_l='wave', swing_l=-26, rate=1.3,
          bob=6, react=True, detail=0)
    stand(sc, r, c[1][0] - 50, c[1][1], V.villager(r), z=top[1], arm_r='up', hold_r=hold_cloth(PINK), swing_r=18, rate=0.8, bob=3, react=True)
    stand(sc, r, c[1][0] + 52, c[1][1] + 10, V.villager(r), z=top[1], arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, rate=1.5, bob=8, react=True, detail=0)
    stand(sc, r, c[2][0] - 55, c[2][1], V.villager(r), z=top[2], arm_l='up', hold_l=hold_cloth(CYAN, 140.0, 80.0), swing_l=20, rate=0.7, bob=3, react=True)
    stand(sc, r, c[2][0] + 58, c[2][1] - 10, V.villager(r), z=top[2], arm_r='point', arm_l='face', rate=1.0, bob=4, react=True, detail=0)
    g.sphere(r.choice(CLOTH), (c[2][0], c[2][1] + 70, top[2] + 24), 30, scale=(1.3, 1.0, 0.8), steps=(3, 6))
    stand(sc, r, c[3][0] - 48, c[3][1], V.villager(r), z=top[3], arm_r='up', swing_r=0, hold_r=V.hold_bundle(r.choice(CLOTH)), arm_l='wave', swing_l=24,
          rate=1.1, bob=3, react=True, detail=0)
    stand(sc, r, c[3][0] + 54, c[3][1] + 6, V.villager(r), z=top[3], arm_r='up', hold_r=hold_cloth(YEL), swing_r=18, rate=0.9, bob=3, react=True, detail=0)
    g.box(r.choice(CLOTH), (c[3][0], c[3][1] + 74, top[3] + 20), (60, 40, 40))
    stand(sc, r, c[4][0] - 30, c[4][1], V.villager(r, hat='sun', hat_col=YEL), z=top[4], arm_l='up', hold_l=V.hold_staff(), arm_r='wave', swing_r=24, rate=1.0,
          bob=3, react=True, detail=0)
    V.goat(sc, r, c[5][0], c[5][1], top[5], yaw=60, s=1.25, wag=30, bob=10.0, rate=0.9, phase=r.random())
    V.goat(sc, r, c[6][0], c[6][1], top[6], yaw=-70, s=1.25, wag=30, graze=18.0, rate=0.5, phase=r.random())


def knoll_b(sc):
    """The survey's lookout on a knoll: a windsock, a telescope panning with its watcher, scientists pointing and
    waving, one of the works gang with a flag."""
    r = random.Random(311)
    g = sc.still()
    V.crag(g, r, KNOLL_B)
    c = KNOLL_B
    top = dict((k, q[3] + 8.0) for k, q in enumerate(KNOLL_B))
    windsock(sc, g, c[6][0], c[6][1], top[6], top[6] + 330, length=230.0)
    tx, ty, tz = c[3][0] - 30.0, c[3][1] - 40.0, top[3]
    tripod(g, tx, ty, tz)
    pan = dict(swing=26.0, pivot=(tx, ty, tz), axis=P.UP)
    sco = sc.part(P.motion(rate=0.1, phase=0.4, **pan))
    rod(sco, WHITE, (tx, ty + 40, tz + 132), (tx, ty - 60, tz + 150), 8, steps=6)
    stand(sc, r, tx, ty + 62, V.sci_look(r), z=tz, arm_l='face', arm_r='face', rate=0.1, phase=0.4, sway=pan)
    stand(sc, r, c[3][0] + 70, c[3][1] + 10, V.sci_look(r), z=top[3], arm_r='point', arm_l='hip', rate=1.0, bob=4, react=True, detail=0)
    stand(sc, r, c[1][0] - 52, c[1][1], V.sci_look(r), z=top[1], arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, rate=1.5, bob=8, react=True, detail=0)
    stand(sc, r, c[1][0] + 54, c[1][1] + 8, worker(r), z=top[1], arm_r='up', hold_r=hold_cloth(ORANGE, 140.0, 80.0), swing_r=20, rate=0.7, bob=3, react=True)
    stand(sc, r, c[2][0], c[2][1], V.sci_look(r), z=top[2], arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.4, bob=-1.5, detail=0)
    stand(sc, r, c[0][0], c[0][1], V.sci_look(r), z=top[0], arm_r='wave', swing_r=26, arm_l='hip', rate=1.2, bob=5, react=True, detail=0)
    stand(sc, r, c[4][0], c[4][1], worker(r), z=top[4], arm_l='wave', swing_l=24, arm_r='face', rate=1.1, bob=4, react=True, detail=0)
    stand(sc, r, c[5][0], c[5][1], V.sci_look(r), z=top[5], arm_l='face', arm_r='face', turn=24, rate=0.2, detail=0)


JOBS = [
    ('SM_P3A_Sample_A', V.lifted(sample_a, LIFT)),
    ('SM_P3A_Sample_B', V.lifted(sample_b, LIFT)),
    ('SM_P3A_Rest_A', V.lifted(rest_a, LIFT)),
    ('SM_P3A_Rest_B', V.lifted(rest_b, LIFT)),
    ('SM_P3A_Pass_A', V.lifted(passing(305, 90.0), LIFT)),
    ('SM_P3A_Pass_B', V.lifted(passing(306, -85.0, porters=True), LIFT)),
    ('SM_P3A_Mend_A', V.lifted(mend_a, LIFT)),
    ('SM_P3A_Mend_B', V.lifted(mend_b, LIFT)),
    ('SM_P3A_Dike', V.lifted(dike, LIFT)),
    ('SM_P3A_Knoll_A', V.lifted(knoll_a, 0.05)),
    ('SM_P3A_Knoll_B', V.lifted(knoll_b, 0.05)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
