"""FULL THROTTLE (Unreal) the population of level 3, the Volcano (Mount Kaldera): /Game/People/Volcano/.

Who lives here while the mountain goes up, and what the shoot's chase drives past:
  the villagers leaving, calmly, with what they can carry: on foot along raised boardwalks (the ground is hot), with
  handcarts, a mule, goats on a lead; and far up the slopes as long caravans. Others have stopped on a stand to watch.
  the fire crews (yellow coats, white helmets, lime bands; pumps and hose, NO vehicle and nothing red: the boss of this
  level is ENGINE 13, a red fire engine) cooling the lava's edge with water;
  the survey's scientists (white coats, coloured hard hats) with their instruments: a dish, an anemometer, a balloon;
  goats, on their crags; birds fleeing the ash (egrets, and the crater lake's flamingos).

What the road is like here (FTDressL3.inl): the barrier is 1.6 m tall, the land lies 0.75 m BELOW the road, and a kerb
of basalt stubs up to 2.2 m stands 21 m out. A person on the ground nearer than about 33 m shows a head at most. So
everyone near the road stands on a TRESTLE DECK 1.8 m up (open underneath: the lava channels behind stay in sight),
which makes those pieces TALL: they are written at 27 m and more. Past the channels people stand on the land, bigger.

Run in the editor:  import ft_people_volcano as K; K.run()        K.run(['SM_VolEvac_A'])
Placed by Source/FullThrottle/FTPeopleL3.inl.
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/Volcano'
DECK = 180.0
PLANK, PLANK_D, STEEL, STEEL_D = '#e8d8b8', '#c9a070', '#8f9bb3', '#5a5870'
PALE, WHITE, DARK, YEL, LIME, PINK = '#b0a088', '#fff6ea', '#22202a', '#ffc93c', '#b8ff5a', '#ff5fa2'
WATER, WATER_L = '#9fe8ff', '#e6fbff'
WOOD = '#c9a070'
CLOTH = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a')
VILLAGE_TOPS = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a', '#3a5fa8')


def tv(length, speed):
    """A travel for a part drawn by hand (figure() takes (length, cm/s) itself)."""
    return (float(length), float(speed) / float(length))


# ------------------------------------------------------------------------------------------ what they stand on
def trestle(g, L, W=300.0, h=DECK, plank=PLANK, edge=PLANK_D, leg=STEEL_D, rail=WHITE, bays=300.0, stripe=None):
    """A raised deck on open legs: planks, an edge beam, legs every `bays` cm rooted 2.5 m into the ground, braces on
    the far side only, and a two-bar rail along the road side (people by the road stand BEHIND a rail)."""
    g.box(plank, (0, 0, h - 7), (L, W, 14))
    g.box(edge, (0, -W / 2.0 - 4, h - 12), (L, 8, 26))
    g.box(edge, (0, W / 2.0 + 4, h - 12), (L, 8, 26))
    if stripe:
        n = max(int(L / 80.0), 2)
        for k in range(n):
            g.box(stripe[k % 2], (-L / 2.0 + (k + 0.5) * L / n, -W / 2.0 - 9, h - 12), (L / n, 3, 22))
    n = max(int(round(L / bays)), 1)
    for k in range(n + 1):
        x = -L / 2.0 + 10 + k * (L - 20) / n
        for sy in (-1, 1):
            g.box(leg, (x, sy * (W / 2.0 - 16), (h - 14 - 250) / 2.0), (12, 12, h - 14 + 250))
        if k < n and k % 2 == 0:
            x1 = -L / 2.0 + 10 + (k + 1) * (L - 20) / n
            tilt_box(g, leg, (x, W / 2.0 - 16, 10), (x1, W / 2.0 - 16, h - 30), 8, 8)
        if rail:
            g.box(rail, (x, -W / 2.0 + 6, h + 52), (7, 7, 104))
    if rail:
        g.box(rail, (0, -W / 2.0 + 6, h + 100), (L, 6, 8))
        g.box(rail, (0, -W / 2.0 + 6, h + 56), (L, 5, 6))


# ------------------------------------------------------------------------------------------ what a villager carries
def hold_bundle(col, tie='#fff6ea'):
    """A bundle carried on the head, one arm up to steady it."""
    def f(m, h, side):
        m.sphere(col, (0, 0, 196), 30, scale=(1.3, 1.1, 0.8), steps=(4, 7))
        m.box(tie, (0, 0, 196), (10, 68, 50))
    return f


def hold_staff(col='#8f6b4a'):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 3, h[2] - 4), (5, 5, 168))
    return f


def hold_roll(col='#ff5fa2', end='#fff6ea'):
    """A rolled rug or mattress on the shoulder."""
    def f(m, h, side):
        m.box(col, (side * 28, 0, 152), (28, 124, 28))
        m.box(end, (side * 28, 0, 152), (16, 130, 16))
    return f


def hold_cage(col='#ffc93c', bird='#ff5fa2'):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1], h[2] - 44), 16, 32, steps=6)
        m.cyl(col, (h[0], h[1], h[2] - 12), 16, 10, steps=6, top_r=3)
        m.box(bird, (h[0], h[1] - 17, h[2] - 28), (10, 3, 12))
    return f


def villager(r, **fixed):
    d = P.look(r, tops=VILLAGE_TOPS, wear=r.choice(('trousers', 'dress', 'skirt', 'coat', 'trousers', 'shorts')),
               hat=r.choice((None, None, 'sun', 'cap', 'beanie')), hat_col=r.choice(CLOTH), sleeves=r.choice(('short', 'long')))
    d.update(fixed)
    return d


def walker(sc, r, x, y, L, v, z=DECK, carry=None, detail=0, **kw):
    """A villager walking the deck's length (v cm/s: negative walks towards -X) and coming round again."""
    d = villager(r)
    pose = dict(stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=abs(v) / 110.0, bob=2.0)
    if carry == 'bundle':
        pose.update(arm_r='up', swing_r=0, hold_r=hold_bundle(r.choice(CLOTH)))
    elif carry == 'bag':
        pose.update(hold_r=P.hold_bag(r.choice(CLOTH)), swing_r=-6)
    elif carry == 'bags':
        pose.update(hold_r=P.hold_bag(r.choice(CLOTH)), hold_l=P.hold_bag(r.choice(CLOTH)), swing_r=-6, swing_l=6)
    elif carry == 'staff':
        pose.update(arm_r=((36, 70), (72, 84)), swing_r=0, hold_r=hold_staff())
    elif carry == 'roll':
        pose.update(arm_r='face', swing_r=0, hold_r=hold_roll(r.choice(CLOTH)))
    elif carry == 'cage':
        pose.update(arm_r=((30, 60), (40, 70)), swing_r=0, hold_r=hold_cage())
    elif carry == 'pack':
        d['pack'] = r.choice(CLOTH)
    elif carry == 'wave':
        pose.update(arm_l='wave', swing_l=24, axis_l='y')
    elif carry == 'push':
        pose.update(arm_l='push', arm_r='push', swing_l=0, swing_r=0)
    d.update(kw)
    P.figure(sc, x, y, z, yaw=90 if v > 0 else -90, detail=detail, phase=r.random(), travel=(L, v), **dict(pose, **d))


def handcart(sc, r, x, y, L, v, z=DECK, s=1.0):
    """A two-wheeled handcart piled with a household, travelling with whoever pushes it (stand them 95 cm behind)."""
    m = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, z + 50 * s)), (x, y, z), 0.0, s)
    m.box(WOOD, (0, 0, 52), (130, 74, 10))
    for sy in (-1, 1):
        m.box(WOOD, (0, sy * 37, 68), (130, 5, 26))
        m.cyl(DARK, (0, sy * 44 + (4 if sy < 0 else 12), 34), 34, 8, rot=(0, 0, 90), steps=10)
        m.cyl(WHITE, (0, sy * 44 + (5 if sy < 0 else 13), 34), 12, 10, rot=(0, 0, 90), steps=6)
    d = -1 if v > 0 else 1
    for sy in (-1, 1):
        m.box(WOOD, (d * 95, sy * 30, 76), (70, 5, 5))
    m.box(r.choice(CLOTH), (-28, 0, 86), (60, 62, 56))
    m.sphere(r.choice(CLOTH), (30, 4, 84), 30, scale=(1.1, 1.0, 0.9), steps=(4, 7))
    m.box(r.choice(CLOTH), (-22, 0, 128), (44, 50, 28))
    m.box(WOOD, (34, 0, 122), (6, 40, 44))           # a chair, upside down on the load
    m.box(WOOD, (34, 0, 144), (40, 40, 5))


def goat(sc, r, x, y, z, yaw=0.0, s=1.0, body=None, **kw):
    P.quadruped(sc, x, y, z, yaw=yaw, s=s, length=80, height=62, girth=27, neck=22, neck_up=42, head=24, horns='#f1e4c8',
                body=body or r.choice(('#fff6ea', '#fff6ea', '#e8c26a', '#c9a070', '#cfcfd8')), tail=12, tail_col='#5a4a3c',
                legs_col='#5a4a3c', snout='#f6d3b4', ears='#5a4a3c', **kw)


# ------------------------------------------------------------------------------------------ the villagers leaving
def evac(seed=21, v=95.0, mule=False):
    def build(sc):
        r = random.Random(seed)
        L = 1800.0
        g = sc.still()
        trestle(g, L, 300.0, bays=450.0)
        ya, yb = -40.0, 60.0
        slots = [-780, -610, -440, -230, -40, 150, 330, 520, 700]
        acts = ['bundle', 'wave', 'bags', 'roll', 'pack', 'cage', 'bundle', 'staff', 'bag']
        r.shuffle(acts)
        cart_at = 3 if not mule else 5
        for i, x in enumerate(slots):
            y = ya if i % 2 else yb
            if i == cart_at:
                handcart(sc, r, x, 10, L, v)
                walker(sc, r, x - (95 if v > 0 else -95), 10, L, v, carry='push')
                continue
            if i == cart_at + 1:
                continue                                        # room for the cart
            if mule and i == 1:
                P.quadruped(sc, x, 0, DECK, yaw=90 if v > 0 else -90, length=120, height=100, girth=38, neck=40, neck_up=48, head=34,
                            body='#a8703a', mane='#3b2a22', tail=34, stride=20, rate=abs(v) / 110.0, phase=r.random(), travel=(L, v))
                pk = sc.part(P.motion(travel=tv(L, v), base=(x, 0.0), pivot=(x, 0.0, DECK + 100)))
                for sy in (-1, 1):
                    pk.box(r.choice(CLOTH), (x, sy * 28, DECK + 92), (70, 22, 40))
                pk.sphere(r.choice(CLOTH), (x, 0, DECK + 124), 28, scale=(1.4, 1.0, 0.7), steps=(4, 7))
                continue
            if i % 4 == 2:                                       # a child beside a grown-up
                walker(sc, r, x + 40, y - 50, L, v, carry='wave', build='kid', s=r.uniform(0.6, 0.7))
            walker(sc, r, x, y, L, v, carry=acts[i])
        gx = slots[0] - 60 if not mule else slots[-1] + 60
        for k in range(2):
            goat(sc, r, gx + k * 95, ya + 10 + k * 70, DECK, yaw=90 if v > 0 else -90, stride=22, wag=30, rate=abs(v) / 60.0,
                 phase=r.random(), travel=(L, v), bob=2.0)
    return build


# ------------------------------------------------------------------------------------------ the fire crews
def fire_look(r, **fixed):
    d = P.look(r, tops=(YEL,), bottoms=('#2b2350', '#3a3848'), wear='coat', sleeves='long', hat=r.choice(('hard', 'helmet')),
               hat_col=WHITE, band=LIME, shoes='#22202a', pack=r.choice((None, STEEL)), hair='short',
               hair_col=r.choice(('#2a2230', '#3b2a22', '#6a4428', '#cfcfd8')))
    d.update(fixed)
    return d


def hold_nozzle():
    def f(m, h, side):
        if side > 0:
            m.cyl(STEEL, (h[0] - 14, h[1] + 4, h[2] + 4), 6, 44, rot=(0, 0, 90), steps=6)
    return f


def jet(sc, x, y, z, reach=900.0, rise=260.0, sweep=14.0, rate=0.35, phase=0.0, yaw=180.0, w0=9.0, w1=30.0, glow=0.08):
    """A jet of water from a nozzle at (x, y, z), thrown `reach` cm along yaw (180 = away from the road), sweeping
    side to side about the nozzle. It carries a little steady light of its own (in the Volcano's dark the water read as
    navy): every piece with a jet stands 28 m and more from the road."""
    m = sc.part(P.motion(swing=sweep, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    dx, dy = math.sin(math.radians(yaw)), -math.cos(math.radians(yaw))
    n = 7
    prev = (x, y, z)
    for k in range(1, n + 1):
        t = k / float(n)
        q = (x + dx * reach * t, y + dy * reach * t, z + rise * 4.0 * t * (1.0 - t) - 60.0 * t)
        w = w0 + (w1 - w0) * t
        tilt_box(m, WATER if k % 2 else WATER_L, prev, q, w, w, glow=glow)
        prev = q
    m.sphere(WATER_L, prev, 52, scale=(1.3, 1.3, 0.7), steps=(3, 6), glow=glow)
    m.sphere(WHITE, (prev[0] + 30, prev[1] + dy * 30, prev[2] + 10), 34, steps=(3, 5), glow=glow)


def pump(g, x, y, z):
    """A portable pump: a yellow engine in a dark frame, a grey hose coil beside it. (No barrel, no red, no glow.)"""
    g.box(DARK, (x, y, z + 6), (124, 84, 12))
    g.box(YEL, (x - 10, y, z + 44), (84, 64, 64))
    g.cyl(STEEL, (x + 44, y, z + 12), 20, 50, steps=8)
    g.box(DARK, (x - 10, y, z + 80), (60, 40, 8))
    for sx in (-1, 1):
        g.box(DARK, (x + sx * 58, y, z + 44), (6, 84, 76))
    g.cyl(STEEL_D, (x - 110, y + 10, z), 36, 16, steps=10)
    g.cyl(WHITE, (x - 110, y + 10, z + 16), 26, 6, steps=8)


def hose_a(sc):
    """Three firefighters on a steel deck throwing water over the lava's edge; a fourth waves the chase through."""
    r = random.Random(31)
    g = sc.still()
    L = 1000.0
    trestle(g, L, 320.0, plank=STEEL, edge=STEEL_D, rail=YEL)
    pump(g, 330, 40, DECK)
    for i, x in enumerate((-330, -90, 150)):
        y = 70.0
        P.figure(sc, x, y, DECK, yaw=180 + (i - 1) * 8, legs='stride', arm_l='carry', arm_r='carry', hold_r=hold_nozzle(), rock=3.0, rate=0.35,
                 phase=i * 0.3, detail=1, **fire_look(r))
        jet(sc, x - 4, y + 60, DECK + 112, reach=880 + i * 60, rise=250 + i * 30, phase=i * 0.3)
        g.box(WHITE, ((x + 330) / 2.0, 20, DECK + 4), (abs(330 - x), 9, 8))          # the hose, flat on the deck
        g.box(WHITE, (x, 45, DECK + 4), (9, 50, 8))
    P.figure(sc, -440, -90, DECK, yaw=-20, arm_r='wave', swing_r=26, arm_l='hip', rate=1.2, bob=3, react=True, detail=1, phase=0.4, **fire_look(r, hat='hard'))
    P.figure(sc, 440, -70, DECK, yaw=25, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.5, bob=6, react=True, detail=1, **fire_look(r, pack=None))


# ------------------------------------------------------------------------------------------ the survey
def sci_look(r, **fixed):
    d = P.look(r, tops=(WHITE,), bottoms=('#3a5fa8', '#2b2350', '#5a4a3c', '#c94f6a'), wear='coat', sleeves='long',
               hat='hard', hat_col=r.choice((PINK, '#2ee6ff', '#8f6bff', '#7dffb0')), band='#2ee6ff')
    d.update(fixed)
    return d


def spin(sc, x, y, rate, phase=0.0):
    """A part that turns round and round about the upright through (x, y)."""
    m = sc.part(P.motion(base=(x, y), travel=(-1.0, rate), phase=phase))
    return m


def survey_a(sc):
    """The survey's deck: a dish turning, an anemometer spinning, a balloon on its line, and five scientists: one at
    the theodolite, one pointing at the mountain, one writing, one filming the chase, one jumping."""
    r = random.Random(41)
    g = sc.still()
    L = 1100.0
    trestle(g, L, 340.0, rail=WHITE, stripe=('#2ee6ff', WHITE))
    # the dish on its post
    g.box(STEEL_D, (-420, 90, DECK + 70), (14, 14, 140))
    d = spin(sc, -420, 90, 0.12)
    tilt_box(d, WHITE, (-420, 90, DECK + 150), (-420, 60, DECK + 196), 150, 12)
    tilt_box(d, PINK, (-420, 62, DECK + 186), (-420, 20, DECK + 214), 8, 8)
    d.box(PINK, (-420, 16, DECK + 216), (18, 18, 18))
    # the anemometer mast
    g.box(STEEL_D, (470, 110, DECK + 170), (8, 8, 340))
    a = spin(sc, 470, 110, 0.9)
    for k in range(3):
        ang = math.radians(k * 120)
        px, py = 470 + math.cos(ang) * 46, 110 + math.sin(ang) * 46
        tilt_box(a, DARK, (470, 110, DECK + 340), (px, py, DECK + 340), 5, 5)
        a.sphere(('#ff5fa2', '#2ee6ff', '#ffc93c')[k], (px, py, DECK + 340), 16, steps=(3, 6))
    # the balloon on its line
    b = sc.part(P.motion(swing=5.0, pivot=(250.0, 120.0, DECK), axis=0.0, rate=0.22, bob=-14.0))
    tilt_box(b, WHITE, (250, 120, DECK), (250, 120, DECK + 430), 4, 4)
    b.sphere(WHITE, (250, 120, DECK + 510), 92, steps=(5, 9))
    b.cyl(PINK, (250, 120, DECK + 494), 93, 30, steps=9)
    b.box(DARK, (250, 120, DECK + 404), (26, 26, 30))
    # the theodolite
    for a in (90, 210, 330):
        tilt_box(g, STEEL_D, (-150, -40, DECK + 118), (-150 + math.cos(math.radians(a)) * 44, -40 + math.sin(math.radians(a)) * 44, DECK), 5, 5)
    g.box(YEL, (-150, -40, DECK + 132), (26, 36, 26))
    g.cyl(DARK, (-150, -58, DECK + 134), 7, 16, rot=(0, 0, 90), steps=6)
    P.figure(sc, -150, 30, DECK, arm_l='hold', arm_r='face', rate=0.3, bob=-1.5, detail=1, **sci_look(r))
    P.figure(sc, -300, -60, DECK, yaw=150, arm_r='up', swing_r=10, axis_r='x', arm_l='hip', rate=0.5, detail=1, **sci_look(r))
    P.figure(sc, 40, 60, DECK, yaw=-20, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.2, phase=0.5, detail=1, **sci_look(r))
    P.figure(sc, 170, -70, DECK, yaw=15, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=5, rate=0.4, react=True, detail=1, **sci_look(r))
    P.figure(sc, 380, -60, DECK, yaw=-10, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=8, rate=1.5, react=True, detail=1, phase=0.3, **sci_look(r))
    # the recorder's table
    g.box(STEEL, (40, 130, DECK + 78), (120, 56, 6))
    for sx in (-1, 1):
        g.box(STEEL_D, (40 + sx * 52, 130, DECK + 38), (6, 50, 76))
    g.box(DARK, (20, 130, DECK + 96), (40, 30, 30))
    g.cyl(WHITE, (74, 146, DECK + 92), 11, 28, rot=(0, 0, 90), steps=7)


# ------------------------------------------------------------------------------------------ the film crew's rostrum
def rostrum(sc):
    """The shoot's own raised deck (the crew kit's pieces are stood ON it by the table: Up 1.8 m), with its pennants."""
    g = sc.still()
    L, W = 1500.0, 460.0
    trestle(g, L, W, rail=None, stripe=(PINK, WHITE))
    for k, x in enumerate((-L / 2.0 + 14, L / 2.0 - 14)):
        g.box(STEEL_D, (x, W / 2.0 - 14, DECK + 200), (7, 7, 400))
        f = sc.part(P.motion(swing=16.0, pivot=(x, W / 2.0 - 14, DECK + 360), axis=P.UP, rate=0.7, phase=k * 0.4))
        s = 1 if k == 0 else -1
        f.box(PINK, (x + s * 50, W / 2.0 - 14, DECK + 372), (100, 4, 44))
        f.box(WHITE, (x + s * 50, W / 2.0 - 14, DECK + 340), (100, 4, 20))
    for sx in (-1, 1):                                  # steps up the back
        for k in range(4):
            g.box(PLANK_D, (sx * 400, W / 2.0 + 30 + k * 34, DECK - 30 - k * 44), (120, 34, 10))


def hose_b(sc):
    """A second crew: two on one big jet, two walking a length of hose down the deck on their shoulders, one sitting on
    the hose reel with a cup."""
    r = random.Random(33)
    g = sc.still()
    L = 1100.0
    trestle(g, L, 320.0, plank=STEEL, edge=STEEL_D, rail=YEL, bays=366.0)
    pump(g, -400, 50, DECK)
    for i, x in enumerate((-150, -60)):
        P.figure(sc, x, 70 + i * 6, DECK, yaw=180, legs='stride', arm_l='carry', arm_r='carry', hold_r=hold_nozzle() if i else None, rock=3.0, rate=0.3,
                 phase=0.1, detail=1, **fire_look(r))
    jet(sc, -100, 135, DECK + 112, reach=1250, rise=420, sweep=18, rate=0.3, phase=0.1, w0=14, w1=46)
    g.box(WHITE, (-250, 40, DECK + 4), (300, 9, 8))
    # the hose carriers, walking the deck and coming round
    tvl = (L - 120.0, 80.0)
    for i, x in enumerate((180, 330)):
        P.figure(sc, x, -40, DECK, yaw=90, stride=22, arm_r='face', swing_l=16, axis_l='x', rate=0.75, bob=2, phase=i * 0.5, travel=tvl, detail=1, **fire_look(r, pack=None))
    hz = sc.part(P.motion(travel=tv(*tvl), base=(255.0, -40.0), pivot=(255.0, -40.0, DECK + 150)))
    hz.box(WHITE, (255, -14, DECK + 150), (250, 12, 12))
    # the reel and the one resting on it
    g.cyl(STEEL_D, (430, 90, DECK), 44, 46, steps=10)
    g.cyl(WHITE, (430, 90, DECK + 46), 34, 5, steps=8)
    P.figure(sc, 430, 84, DECK + 5, yaw=10, legs='sit', seat=46, arm_r='face', hold_r=P.hold_cup(), arm_l='down', rate=0.3, bob=-1.0, detail=1, **fire_look(r, hat='hard', pack=None))
    P.figure(sc, 60, -90, DECK, yaw=-15, arm_r='wave', swing_r=26, arm_l='hip', rate=1.2, bob=3, react=True, detail=1, phase=0.4, **fire_look(r, hat='hard'))


def survey_b(sc):
    """The gas team: a wind sock, a thermal camera on its tripod, one reaching a sampling pole out over the back rail,
    one at a laptop on a crate, three who have seen the chase."""
    r = random.Random(43)
    g = sc.still()
    L = 1000.0
    trestle(g, L, 340.0, rail=WHITE, stripe=('#2ee6ff', WHITE), bays=333.0)
    # the wind sock: striped lengths on a mast, swinging with the wind
    g.box(STEEL_D, (-430, 120, DECK + 220), (8, 8, 440))
    w = sc.part(P.motion(swing=22.0, pivot=(-430.0, 120.0, DECK + 420), axis=P.UP, rate=0.35, bob=-6.0))
    for k in range(4):
        w.box(PINK if k % 2 else WHITE, (-430 + 30 + k * 46, 120, DECK + 420), (46, 52 - k * 9, 52 - k * 9))
    # the radar bar, turning
    g.box(STEEL_D, (440, 110, DECK + 150), (10, 10, 300))
    a = spin(sc, 440, 110, 0.35)
    a.box(WHITE, (440, 110, DECK + 312), (170, 26, 22))
    a.box('#2ee6ff', (440, 110, DECK + 326), (170, 10, 8))
    # the thermal camera
    for ang in (90, 210, 330):
        tilt_box(g, STEEL_D, (120, -50, DECK + 120), (120 + math.cos(math.radians(ang)) * 44, -50 + math.sin(math.radians(ang)) * 44, DECK), 5, 5)
    g.box(DARK, (120, -50, DECK + 136), (40, 56, 32))
    g.box('#8f6bff', (120, -80, DECK + 136), (30, 6, 24))
    P.figure(sc, 120, 20, DECK, arm_l='hold', arm_r='hold', rate=0.3, bob=-1.5, detail=1, **sci_look(r))
    # the sampler, at the back rail
    P.figure(sc, -220, 100, DECK, yaw=180, arm_l='carry', arm_r='carry', hold_r=P.hold_rod(col=STEEL, length=300.0), swing_r=10, axis_r='x', rate=0.4, detail=1, **sci_look(r))
    # the laptop on a crate
    g.box(PLANK_D, (-40, 110, DECK + 30), (70, 50, 60))
    g.box(DARK, (-40, 106, DECK + 76), (40, 4, 28))
    P.figure(sc, -40, 40, DECK, yaw=180, arm_l='hold', arm_r='hold', rate=0.3, bob=-1.0, phase=0.4, detail=1, **sci_look(r))
    P.figure(sc, -340, -70, DECK, yaw=10, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.7, react=True, detail=1, **sci_look(r))
    P.figure(sc, 300, -70, DECK, yaw=-12, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.5, react=True, detail=1, phase=0.6, **sci_look(r))
    P.figure(sc, 400, -40, DECK, yaw=5, arm_r='wave', swing_r=26, bob=4, rate=1.3, react=True, detail=1, phase=0.2, **sci_look(r))


# ------------------------------------------------------------------------------------------ the villagers who stopped to watch
def watch(seed=61, banner=(PINK, WHITE, '#2ee6ff')):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L = 1000.0
        trestle(g, L, 380.0, bays=333.0)
        rows = ((-110.0, 0.0), (0.0, 46.0), (110.0, 92.0))
        for k, (y, up) in enumerate(rows):
            if up:
                g.box(PLANK_D, (0, y + 30, DECK + up / 2.0), (L - 60, 170 - k * 20, up))
        acts = (
            dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.5),
            dict(arm_r='wave', swing_r=26, rate=1.2, bob=3),
            dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=7, rate=1.3),
            dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7, bob=2),
            dict(arm_r='up', hold_r=P.hold_flag(col=r.choice(CLOTH), w=70.0), swing_r=18, rate=1.0, bob=4),
            dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4, bob=2),
        )
        for k, (y, up) in enumerate(rows):
            n = 5 if k != 1 else 4
            for i in range(n):
                x = -L / 2.0 + 100 + (i + (0.5 if k == 1 else 0.0)) * (L - 200) / 4.0 + r.uniform(-16, 16)
                d = villager(r)
                if k == 0 and i in (1, 3):
                    d.update(build='kid', s=r.uniform(0.6, 0.7))
                d.update(acts[(i * 5 + k * 2 + seed) % len(acts)])
                P.figure(sc, x, y + r.uniform(-8, 8), DECK + up, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=0, **d)
        # what they brought with them, and the cloth they have strung up
        for i in range(4):
            g.box(r.choice(CLOTH), (-420 + i * 270 + r.uniform(-20, 20), -160, DECK + 18), (44, 22, 36))
        for sx in (-1, 1):
            g.box(PLANK_D, (sx * (L / 2.0 - 12), 160, DECK + 230), (8, 8, 460))
        b = sc.part(P.motion(swing=4.0, pivot=(0.0, 160.0, DECK + 440), axis=0.0, rate=0.5))
        n = 9
        for i in range(n):
            b.box(banner[i % len(banner)], (-L / 2.0 + 30 + (i + 0.5) * (L - 60) / n, 160, DECK + 404), ((L - 60) / n, 4, 70))
    return build


# ------------------------------------------------------------------------------------------ goats
BAS, BAS_L, ASHCAP = '#3e3244', '#5a4e5e', '#b0a088'


def crag(g, r, cols):
    """Basalt columns with ash on their heads: cols = [(x, y, radius, height)]."""
    for k, (x, y, rad, h) in enumerate(cols):
        g.cyl(BAS if k % 2 else BAS_L, (x, y, -60), rad, h + 60, steps=6)
        g.cyl(ASHCAP, (x, y, h), rad * 0.9, 8, steps=6)


def goats_a(sc):
    """Goats on a basalt crag, and the child who minds them, waving from the top."""
    r = random.Random(71)
    g = sc.still()
    cols = [(-330, 20, 120, 150), (-140, -10, 130, 215), (60, 30, 140, 290), (270, 0, 130, 230), (440, 40, 110, 160), (-30, 190, 130, 250), (190, 200, 120, 190)]
    crag(g, r, cols)
    spots = [(-330, 20, 150, 60), (-140, -10, 215, -70), (270, 0, 230, 100), (440, 40, 160, -40), (110, 70, 290, 80)]
    for i, (x, y, h, yaw) in enumerate(spots):
        kw = dict(bob=10.0, rate=0.9) if i % 2 else dict(graze=18.0, rate=0.5)
        goat(sc, r, x, y, h + 8, yaw=yaw, s=1.25, wag=30, phase=r.random(), **kw)
    P.figure(sc, 10, -20, 298, yaw=0, arm_r='wave', swing_r=26, arm_l='up', hold_l=hold_staff(), rate=1.2, bob=4, react=True, detail=1, phase=0.3,
             **villager(r, build='kid', s=0.72, hat='sun', hat_col=YEL, top=PINK))


def herd(sc):
    """A herd on the move across the slope: goats twice life size (it is seen from 100 m and more), their herder and a
    dog. Saved with the always-moving master."""
    r = random.Random(73)
    L, v = 5000.0, 260.0
    for i in range(13):
        x = -L / 2.0 + 300 + i * 290 + r.uniform(-90, 90)
        y = r.uniform(-260, 260)
        goat(sc, r, x, y, 0.0, yaw=90, s=2.3, stride=24, wag=30, rate=r.uniform(1.6, 2.1), phase=r.random(), bob=14.0, travel=(L, v))
    P.figure(sc, -L / 2.0 + 120, 0, 0, yaw=90, stride=24, swing_l=18, axis_l='x', arm_r=((36, 70), (72, 84)), hold_r=hold_staff(), rate=1.0, bob=4,
             travel=(L, v), detail=0, **villager(r, hat='sun', hat_col=YEL, top='#2ee6ff', wear='coat', s=2.2))
    P.quadruped(sc, -L / 2.0 + 420, 300, 0, yaw=90, s=2.2, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=24,
                rate=2.4, bob=6.0, travel=(L, v), body='#e8c26a')


# ------------------------------------------------------------------------------------------ far: the caravan, the camp, the fire line
LAMP = '#ffe28a'


def hold_lantern(glow=0.5):
    """A lantern on a staff (a steady light: the pieces that carry it stand 60 m and more from the road)."""
    def f(m, h, side):
        m.box('#8f6b4a', (h[0], h[1] - 3, h[2] + 20), (5, 5, 190))
        m.box(LAMP, (h[0], h[1] - 3, h[2] + 122), (20, 20, 24), glow=glow)
    return f


def caravan(sc):
    """The villagers leaving, seen from far off: a long line on the move with carts, mules and lanterns, twice life
    size. Saved with the always-moving master."""
    r = random.Random(81)
    L, v, S = 5600.0, 170.0, 2.0
    kinds = ['w', 'cart', 'w', 'lamp', 'mule', 'w', 'w', 'lamp', 'cart', 'w', 'mule', 'w', 'lamp']
    step = (L - 500.0) / len(kinds)
    for i, kind in enumerate(kinds):
        x = -L / 2.0 + 250 + i * step + r.uniform(-40, 40)
        y = r.uniform(-90, 90)
        if kind == 'cart':
            handcart(sc, r, x, y, L, v, z=0.0, s=S)
            walker(sc, r, x - 95 * S, y, L, v, z=0.0, carry='push', s=S)
        elif kind == 'mule':
            P.quadruped(sc, x, y, 0, yaw=90, s=S, length=120, height=100, girth=38, neck=40, neck_up=48, head=34, body='#a8703a', mane='#3b2a22', tail=34,
                        stride=20, rate=0.9, phase=r.random(), travel=(L, v))
            pk = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, 200.0)))
            for sy in (-1, 1):
                pk.box(r.choice(CLOTH), (x, y + sy * 56, 184), (140, 44, 80))
            pk.sphere(r.choice(CLOTH), (x, y, 250), 56, scale=(1.4, 1.0, 0.7), steps=(4, 7))
        elif kind == 'lamp':
            d = villager(r, s=S)
            P.figure(sc, x, y, 0, yaw=90, stride=24, swing_l=18, axis_l='x', arm_r=((36, 70), (72, 84)), hold_r=hold_lantern(), rate=0.8, bob=3,
                     phase=r.random(), travel=(L, v), detail=0, **d)
        else:
            walker(sc, r, x, y, L, v, z=0.0, carry=r.choice(('bundle', 'bags', 'pack', 'roll', 'wave', 'bundle')), s=S)
            if i % 5 == 3:
                walker(sc, r, x + 70 * S, y + 60, L, v, z=0.0, carry='wave', build='kid', s=S * 0.66)


def camp(sc):
    """The assembly point up the slope: striped tents, lanterns on a line, and everyone who is waiting there, on their
    feet for the chase. Twice life size; saved with the always-moving master."""
    r = random.Random(91)
    g = sc.still()
    S = 2.0
    for k, (x, y, c1, c2) in enumerate(((-1500, 500, PINK, WHITE), (-300, 640, '#2ee6ff', WHITE), (1100, 520, YEL, WHITE))):
        g.cyl(c1, (x, y, 0), 420, 300, steps=8)
        g.cyl(c2, (x, y, 300), 440, 330, steps=8, top_r=20)
        g.box(DARK, (x, y - 400, 130), (150, 30, 260))
    for sx in (-1, 0, 1):
        g.box('#8f6b4a', (sx * 1800, 150, 420), (14, 14, 840))
    for k in range(12):
        g.box(LAMP, (-1650 + k * 300, 150, 760 - 46 * math.sin(math.pi * ((k % 6) + 0.5) / 6.0)), (44, 44, 52), glow=0.5)
    g.box(DARK, (0, 150, 800), (3600, 6, 6))
    acts = (
        dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=14, rate=1.4),
        dict(arm_r='wave', swing_r=26, rate=1.2, bob=6),
        dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=12, rate=1.3),
        dict(arm_r='up', hold_r=P.hold_flag(col=PINK, w=70.0), swing_r=18, rate=1.0, bob=6),
    )
    n = 20
    for i in range(n):
        x = -1900 + (i + 0.5) * 3800.0 / n + r.uniform(-30, 30)
        y = (-260 if i % 2 else -120) + r.uniform(-30, 30)
        d = villager(r, s=S * r.uniform(0.92, 1.06))
        if i % 6 == 4:
            d.update(build='kid', s=S * 0.68)
        d.update(acts[(i * 3) % len(acts)])
        P.figure(sc, x, y, 0, yaw=r.uniform(-14, 14), phase=r.random(), detail=0, **d)


def fire_line(sc):
    """A fire crew on the ground out by the fissures, seen from 100 m: five firefighters half as big again, their
    jets thrown high. Saved with the always-moving master."""
    r = random.Random(95)
    g = sc.still()
    S = 1.7
    pump(g, 900, -40, 0)
    g.box(YEL, (-900, -40, 60), (200, 120, 120))
    g.box(DARK, (-900, -40, 126), (160, 90, 12))
    for i, x in enumerate((-640, -320, 0, 320, 640)):
        P.figure(sc, x, 0, 0, yaw=180 + (i - 2) * 6, legs='stride', arm_l='carry', arm_r='carry', hold_r=hold_nozzle(), rock=3.0, rate=0.3, phase=i * 0.21,
                 detail=0, **fire_look(r, s=S))
        jet(sc, x - 6, 100, 112 * S, reach=1500 + (i % 2) * 300, rise=520 + (i % 3) * 90, sweep=12, rate=0.3, phase=i * 0.21, w0=16, w1=56)
        g.box(WHITE, (x, -30, 5), (12, 60, 10))
    g.box(WHITE, (0, -60, 5), (1800, 12, 10))


# ------------------------------------------------------------------------------------------ birds fleeing the ash
def big_bird(sc, x, y, z, span, body, wing, beak, phase, rate, flap, L, v, glow=0.2, bob=-30.0, legs=None, neck=0.5):
    """A bird big enough to read against the sky from the road, flying along +X and coming round. Its colours carry a
    little light of their own (the Volcano is dark: a white bird in shade reads black)."""
    common = dict(phase=phase, rate=rate, bob=bob, base=(x, y), travel=tv(L, v))
    B = span * 0.42
    b = sc.part(P.motion(pivot=(x, y, z), **common))
    b.base = None
    tilt_box(b, body, (x - B * 0.5, y, z), (x + B * 0.5, y, z + B * 0.06), B * 0.36, B * 0.34, glow=glow)
    tilt_box(b, body, (x + B * 0.4, y, z + B * 0.05), (x + B * (0.5 + neck), y, z + B * 0.22), B * 0.15, B * 0.15, glow=glow)
    tilt_box(b, beak, (x + B * (0.5 + neck), y, z + B * 0.22), (x + B * (0.78 + neck), y, z + B * 0.16), B * 0.1, B * 0.1, glow=glow)
    tilt_box(b, wing, (x - B * 0.45, y, z), (x - B * 0.9, y, z + B * 0.03), B * 0.3, B * 0.06, glow=glow)
    if legs:
        tilt_box(b, legs, (x - B * 0.5, y, z - B * 0.05), (x - B * 1.5, y, z - B * 0.16), B * 0.07, B * 0.07, glow=glow)
    for side in (-1, 1):
        w = sc.part(P.motion(swing=side * flap, pivot=(x, y, z + B * 0.1), axis=0.0, **common))
        w.base = None
        half = span * 0.5
        a = (x, y + side * B * 0.12, z + B * 0.1)
        mid = (x, y + side * half * 0.55, z + B * 0.24)
        tip = (x - B * 0.2, y + side * half, z + B * 0.12)
        tilt_box(w, body, a, mid, B * 0.46, max(B * 0.05, 4.0), glow=glow)
        tilt_box(w, wing, mid, tip, B * 0.34, max(B * 0.05, 4.0), glow=glow)


def flock(seed=51, n=11, body='#fff6ea', wing='#d7e3f0', beak='#ffc93c', span=(330.0, 450.0), L=11000.0, v=1500.0, lo=1200.0, hi=3200.0, depth=1800.0,
          legs=None, neck=0.5):
    def build(sc):
        r = random.Random(seed)
        for i in range(n):
            x = -L / 2.0 + (i + 0.5) * L / n * 0.5 + r.uniform(-250, 250)      # a loose skein, bunched over half the length
            y = r.uniform(-depth / 2.0, depth / 2.0)
            z = lo + (hi - lo) * r.random()
            big_bird(sc, x, y, z, r.uniform(*span), body, wing, beak, r.random(), r.uniform(1.2, 1.7), 34.0, L, v, legs=legs, neck=neck)
    return build


def lifted(fn, g=0.05):
    """A far piece's colours given a little steady light of their own: on the Volcano's dark slopes a crowd 150 m off is
    black on black without it. (Every such piece is placed 70 m and more from the road.)"""
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


# The near pieces too, a little: they stand 28 m out and more (a steady light may stand from 24 m), and against the
# black works of the finale an unlit figure is a dark shape on a pale deck.
NEAR_LIFT = 0.035

JOBS = [
    ('SM_VolEvac_A', lifted(evac(21, 95.0), NEAR_LIFT)),
    ('SM_VolEvac_B', lifted(evac(22, -95.0, mule=True), NEAR_LIFT)),
    ('SM_VolHose_A', lifted(hose_a, NEAR_LIFT)),
    ('SM_VolHose_B', lifted(hose_b, NEAR_LIFT)),
    ('SM_VolSurvey_A', lifted(survey_a, NEAR_LIFT)),
    ('SM_VolSurvey_B', lifted(survey_b, NEAR_LIFT)),
    ('SM_VolWatch_A', lifted(watch(61), NEAR_LIFT)),
    ('SM_VolWatch_B', lifted(watch(62, banner=(YEL, '#8f6bff', WHITE)), NEAR_LIFT)),
    ('SM_VolRostrum', rostrum),
    ('SM_VolGoats', lifted(goats_a, NEAR_LIFT)),
    ('SM_VolHerd', lifted(herd), dict(far=True)),
    ('SM_VolCaravan', lifted(caravan), dict(far=True)),
    ('SM_VolCamp', lifted(camp), dict(far=True)),
    ('SM_VolFireLine', lifted(fire_line, 0.06), dict(far=True)),
    ('SM_VolFlock_A', flock(51), dict(far=True)),
    ('SM_VolFlock_B', flock(52, n=9, body='#ff8fb8', wing='#ff5fa2', beak='#22202a', span=(360.0, 470.0), lo=900.0, hi=2400.0, legs='#ff5fa2', neck=0.9),
     dict(far=True)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
