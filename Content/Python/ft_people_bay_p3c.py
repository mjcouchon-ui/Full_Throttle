"""FULL THROTTLE (Unreal) the population, level 0: Bayshore, pass 3, builder p3c: THE MOVERS.

Everything here TRAVELS: along the piece's X (which the placement lays along the road) and round again, or in a ring
about a point of the piece. Nothing travels across: a line is never turned towards the road by its rule (yaw jitter 0
by the road, a few degrees on the lower beach), and the longest line by the road is 38 m, inside its own 45 m slot.

  by the road, LOW (on the boardwalk's deck, 21.7 m, and on the town's pavement, 22.4 m)
    SM_P3C_Wheels_A / _B     cyclists with pennants, roller skaters, children on scooters, both ways        38 m
    SM_P3C_Wheels_C / _D     the same on the pavement: a cargo trike, a runner and a dog, skaters, scooters  36 m
  the beach path (40 m, bay side)
    SM_P3C_BeachPath_A       a four-wheeled surrey under a striped canopy, a family pedalling; joggers; a dog 44 m
    SM_P3C_BeachPath_B       beach cruisers with boards under the arm, a running club                        44 m
    SM_P3C_Circles           children on bikes riding rings round a pennant mast, dogs and a runner the other way
  the lower beach (98 to 136 m, bay side, level ground; the always-moving master)
    SM_P3C_Riders            four horses and their riders at a canter along the sand, a dog after them       90 m
    SM_P3C_Ponies            a string of four ponies, a child on each, led at a walk                         60 m
    SM_P3C_SandTrain         the beach train: a little engine and two open carriages of waving passengers   100 m
    SM_P3C_BeachRace         a fun run: twelve runners in hot singlets with white bibs, a marshal on a bike 110 m
  the water's edge (Afloat)
    SM_P3C_Paddlers          a dragon boat (eight paddlers in time, a drummer, a helm) and three kayaks      240 m

The placing rules: C:/ftwork/p3/L0c.inl (the block "p3c" of the level's table).
Run in the editor:  import ft_people_bay_p3c as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/Bayshore'

WHITE, CREAM, DARK, WOOD, WOOD_D, STEEL = '#fffaf2', '#fff1d6', '#22202a', '#c9a06a', '#9a744a', '#8f9bb3'
HOTPINK, YEL, MINT, SKY, RED, ORANGE, VIOLET, NAVY, LIME, TEAL = ('#ff4fb8', '#ffc93c', '#7dffb0', '#2ee6ff', '#ff2d3c', '#ff7a3c', '#8f6bff',
                                                                 '#2b2350', '#b8ff5a', '#19c5c0')
HOT = (HOTPINK, RED, YEL, SKY, ORANGE, MINT, VIOLET, LIME)
HORSES = (('#8e5a3a', '#3b2a22'), ('#5a4a3c', '#22202a'), ('#f1e4c8', '#fffaf2'), ('#c9a070', '#fff1d6'), ('#3b2a22', '#22202a'))
LIFT = 15.0          # the track sinks a piece 15 cm into the land: what stands on the ground is drawn this much up


# ------------------------------------------------------------------------------------------ helpers
def lk(r, low=False, **fixed):
    """A random person in hot colours. low: for a piece that must stay under 2 m as built (no tall hair, s under 0.93)."""
    d = P.look(r, tops=HOT, wear=r.choice(('shorts', 'trousers', 'skirt', 'shorts', 'dress')), sleeves=r.choice(('short', 'none', 'short')))
    if low:
        d['hair'] = r.choice(('short', 'long', 'pony', 'bald', 'short'))
        d['s'] = r.uniform(0.88, 0.93)
    d.update(fixed)
    return d


def fig(sc, x, y, look, **kw):
    """P.figure with a look and the pose merged into one set of values (a value given twice would be an error)."""
    v = dict(look)
    v.update(kw)
    return P.figure(sc, x, y, **v)


def prop(sc, x, y, L, v, z=0.0, **kw):
    """The model for a thing that travels the piece's X with whoever is at (x, y): v cm/s (negative: towards -X),
    round a line L long. It is drawn in the mesh's own space, where it stands at the start."""
    return sc.part(P.motion(pivot=(x, y, z), base=(x, y), travel=(float(L), float(v) / float(L)), **kw))


def ring_prop(sc, x, y, cx, cy, rps, **kw):
    """The same for a thing that circles (cx, cy): rps rounds a second."""
    return sc.part(P.motion(pivot=(x, y, 0.0), base=(cx, cy), travel=(-1.0, float(rps)), **kw))


def wheel(m, x, y, z, r, w=6.0, col=DARK, steps=8):
    rod(m, col, (x, y - w / 2.0, z), (x, y + w / 2.0, z), r, steps=steps)


def bike(m, x, y, d, col, z=0.0, saddle=78.0, bars=WHITE):
    """A bicycle heading d (+1: +X) with its saddle over (x - 8 d, y)."""
    for dx in (-50, 52):
        wheel(m, x + d * dx, y, z + 31, 31)
    tilt_box(m, col, (x - d * 50, y, z + 31), (x - d * 10, y, z + saddle - 4), 5, 5)
    tilt_box(m, col, (x - d * 10, y, z + saddle - 8), (x + d * 38, y, z + saddle + 6), 5, 5)
    tilt_box(m, col, (x + d * 36, y, z + saddle + 16), (x + d * 52, y, z + 31), 5, 5)
    m.box(bars, (x + d * 36, y, z + saddle + 18), (6, 46, 6))
    m.box(DARK, (x - d * 10, y, z + saddle - 2), (26, 12, 6))


def cyclist(sc, r, x, y, d, L, speed, look, col, z=0.0, flag=None, detail=1, saddle=74.0, **kw):
    """Someone riding a bicycle along the piece's X: the legs pedal, the hands are on the bars."""
    s = look.get('s', 0.9)
    m = prop(sc, x, y, L, speed * d)
    bike(m, x, y, d, col, z, saddle)
    if flag:                                                                   # a pennant on a whip behind the saddle
        m.box(WHITE, (x - d * 50, y, z + 103), (4, 4, 144))
        m.box(flag, (x - d * 69, y, z + 163), (34, 4, 22))
    v = dict(arm_l='push', arm_r='push', stride=32, rate=1.3, phase=r.random(), detail=detail)
    v.update(kw)
    fig(sc, x - d * 8, y, look, z=z + saddle - 66.0 * s, yaw=90.0 * d, travel=(L, speed * d), **v)


def roller(sc, r, x, y, d, L, look, speed=430.0, z=0.0, detail=1, **kw):
    """A roller skater: long strides, arms swinging wide."""
    v = dict(legs='stride', stride=30, swing_l=34, swing_r=-34, axis_l='x', axis_r='x', rate=1.0, bob=2, phase=r.random(), detail=detail)
    v.update(kw)
    fig(sc, x, y, look, z=z + 8, yaw=90.0 * d, travel=(L, speed * d), **v)
    m = prop(sc, x, y, L, speed * d)
    for sy in (-10, 10):
        m.box(DARK, (x, y + sy, z + 4), (30, 8, 8))


def scooter(sc, r, x, y, d, L, col, speed=300.0, z=0.0):
    """A child on a push scooter."""
    look = lk(r, build='kid', s=0.66, hat='helmet', hat_col=r.choice(HOT))
    fig(sc, x, y, look, z=z + 10, yaw=90.0 * d, legs='stride', arm_l='push', arm_r='push', stride=16, rate=1.2, bob=2, phase=r.random(),
        travel=(L, speed * d), detail=1)
    m = prop(sc, x, y, L, speed * d)
    m.box(col, (x, y, z + 8), (66, 13, 5))
    for dx in (-30, 32):
        m.box(DARK, (x + d * dx, y, z + 6), (13, 6, 12))
    tilt_box(m, STEEL, (x + d * 30, y, z + 8), (x + d * 24, y, z + 80), 4, 4)
    m.box(col, (x + d * 24, y, z + 80), (5, 38, 5))


def runner(sc, r, x, y, d, L, look, speed=340.0, z=0.0, detail=1, **kw):
    v = dict(legs='stride', stride=40, swing_l=36, swing_r=-36, axis_l='x', axis_r='x', rate=1.7, bob=7, phase=r.random(), detail=detail)
    v.update(kw)
    fig(sc, x, y, look, z=z, yaw=90.0 * d, travel=(L, speed * d), **v)


def dog(sc, x, y, d, L, speed, z=0.0, body='#e8c26a', s=1.0, phase=0.0):
    P.quadruped(sc, x, y, z=z, yaw=90.0 * d, s=s, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=30,
                rate=2.4, bob=4, phase=phase, travel=(L, speed * d), body=body, ears='#5a4a3c', snout=DARK)


def trike(sc, r, x, y, d, L, speed, col, z=0.0):
    """A cargo trike: a striped box between two front wheels, its rider pedalling behind it. Under 2 m."""
    look = lk(r, low=True, hat='cap', hat_col=col, wear='shorts')
    s = look['s']
    m = prop(sc, x, y, L, speed * d)
    wheel(m, x - d * 60, y, z + 31, 31)
    for sy in (-36, 36):
        wheel(m, x + d * 62, y + sy, z + 27, 27)
    m.box(col, (x + d * 62, y, z + 62), (84, 58, 50))
    m.box(WHITE, (x + d * 62, y, z + 62), (86, 60, 14))
    tilt_box(m, STEEL, (x - d * 60, y, z + 31), (x - d * 16, y, z + 74), 5, 5)
    tilt_box(m, STEEL, (x - d * 16, y, z + 70), (x + d * 22, y, z + 60), 5, 5)
    m.box(STEEL, (x + d * 16, y, z + 96), (6, 50, 6))
    m.box(DARK, (x - d * 16, y, z + 76), (26, 12, 6))
    fig(sc, x - d * 14, y, look, z=z + 78.0 - 66.0 * s, yaw=90.0 * d, arm_l='push', arm_r='push', stride=30, rate=1.1, phase=r.random(),
        travel=(L, speed * d), detail=1)


def horse_rider(sc, r, x, y, d, L, speed, look, coat, sh=1.0, rate=1.5, bob=8.0, stride=30.0, detail=1, **kw):
    """A horse (or, with sh under 1, a pony) and its rider, hopping together along the piece's X."""
    ph = r.random()
    P.quadruped(sc, x, y, z=LIFT, yaw=90.0 * d, s=sh, body=coat[0], mane=coat[1], tail_col=coat[1], stride=stride, wag=14, rate=rate, bob=bob,
                phase=ph, travel=(L, speed * d))
    s = look.get('s', 1.0)
    v = dict(legs='wide', arm_l='hold', arm_r='hold', detail=detail)
    v.update(kw)
    fig(sc, x, y, look, z=LIFT + sh * 120.0 - 58.0 * s, yaw=90.0 * d, rate=rate, bob=bob, phase=ph, travel=(L, speed * d), **v)
    m = prop(sc, x, y, L, speed * d, rate=rate, bob=bob, phase=ph)               # the saddle cloth
    m.box(look.get('top', RED), (x, y, LIFT + sh * 121.0), (sh * 50, sh * 46, 5))


# ------------------------------------------------------------------------------------------ by the road: wheels
def wheels(seed, variant, L, z0):
    """38 m (the boardwalk) or 36 m (the pavement) of people on wheels, in two lanes 50 cm apart: one lane each way.
    Nothing is 2 m tall as built. From the road: helmets, pennants and shoulders going by, faster than the walkers."""
    def build(sc):
        r = random.Random(seed)
        ya, yb = -25.0, 25.0
        h = L / 2.0 - 150.0

        def rider(x, y, d, speed, col, flag):
            cyclist(sc, r, x, y, d, L, speed, lk(r, low=True, hat='helmet', hat_col=r.choice(HOT)), col, z=z0, flag=flag)

        def skate(x, y, d, speed=430.0):
            roller(sc, r, x, y, d, L, lk(r, low=True, hat='cap', hat_col=r.choice(HOT), wear='shorts'), speed=speed, z=z0)

        if variant == 0:        # three cyclists one way, two skaters and a scooter the other
            rider(-h, ya, 1, 560, RED, YEL)
            rider(-h * 0.25, ya, 1, 560, SKY, HOTPINK)
            rider(h * 0.6, ya, 1, 520, YEL, None)
            skate(h, yb, -1)
            skate(h * 0.3, yb, -1, 400)
            scooter(sc, r, -h * 0.5, yb, -1, L, LIME, z=z0)
        elif variant == 1:      # a line of four skaters, two scooters; two cyclists the other way
            for k in range(4):
                skate(-h + k * 130, ya, 1, 440)
            scooter(sc, r, h * 0.35, ya, 1, L, HOTPINK, z=z0)
            scooter(sc, r, h * 0.6, ya, 1, L, SKY, z=z0)
            rider(h, yb, -1, 600, HOTPINK, LIME)
            rider(-h * 0.2, yb, -1, 540, MINT, None)
        elif variant == 2:      # the pavement: a cargo trike, a runner and a dog; skaters the other way
            trike(sc, r, -h, ya, 1, L, 380, ORANGE, z=z0)
            runner(sc, r, h * 0.1, ya, 1, L, lk(r, low=True, wear='shorts', sleeves='none', hat='cap', hat_col=WHITE), z=z0)
            dog(sc, h * 0.1 + 80, ya, 1, L, 340, z=z0, body=WHITE, phase=0.3)
            skate(h, yb, -1)
            skate(h - 120, yb, -1)
            rider(-h * 0.3, yb, -1, 560, VIOLET, YEL)
        else:                   # two trikes and a scooter; a cyclist, a skater and a runner the other way
            trike(sc, r, -h * 0.9, ya, 1, L, 360, HOTPINK, z=z0)
            trike(sc, r, h * 0.2, ya, 1, L, 360, TEAL, z=z0)
            scooter(sc, r, h * 0.75, ya, 1, L, YEL, z=z0)
            rider(h, yb, -1, 580, LIME, RED)
            skate(h * 0.2, yb, -1, 420)
            runner(sc, r, -h * 0.6, yb, -1, L, lk(r, low=True, wear='shorts', hat='cap', hat_col=RED), z=z0)
    return build


# ------------------------------------------------------------------------------------------ the beach path
def surrey(sc, r, x, y, d, L, speed, cols):
    """A four-wheeled pedal surrey: a bench front and back, a striped canopy on four posts. 2.3 m tall."""
    z = LIFT
    m = prop(sc, x, y, L, speed * d)
    for dx in (-80, 80):
        for sy in (-62, 62):
            wheel(m, x + dx, y + sy, z + 28, 28, steps=6)
    m.box(STEEL, (x, y, z + 44), (230, 112, 8))
    for dx in (-50, 55):
        m.box(cols[0], (x + d * dx, y, z + 78), (34, 104, 8))
        m.box(cols[0], (x + d * (dx - 18), y, z + 100), (6, 104, 44))
    for dx in (-104, 104):
        for sy in (-52, 52):
            m.box(WHITE, (x + dx, y + sy, z + 130), (5, 5, 170))
    for k in range(4):
        m.box(cols[k % 2], (x - 96 + k * 64, y, z + 220), (64, 128, 10))
    m.box(cols[1], (x, y - 64, z + 208), (256, 4, 16))
    m.box(cols[1], (x, y + 64, z + 208), (256, 4, 16))
    m.box(DARK, (x + d * 100, y, z + 100), (6, 60, 6))
    for k, (dx, sy) in enumerate(((-50, -28), (-50, 28), (55, -28), (55, 28))):
        kid = k >= 2
        look = lk(r, build='kid', s=0.66) if kid else lk(r, s=0.95, hat='sun', hat_col=r.choice(HOT))
        fig(sc, x + d * dx, y + sy, look, z=z + (56 if kid else 40), yaw=90.0 * d, legs='sit', arm_l='wave' if sy < 0 else 'push',
            arm_r='push' if sy < 0 else 'wave', swing_l=26 if sy < 0 else 0, swing_r=0 if sy < 0 else 26, rate=1.3, phase=r.random(),
            travel=(L, speed * d), detail=1)


def beach_path(seed, variant):
    """44 m of the path along the top of the beach, 40 m from the road, where the whole figure is in view."""
    def build(sc):
        r = random.Random(seed)
        L = 4400.0
        if variant == 0:
            surrey(sc, r, -1500.0, -40.0, 1, L, 300, (RED, WHITE))
            surrey(sc, r, 1300.0, 60.0, -1, L, 280, (TEAL, YEL))
            for k in range(2):
                runner(sc, r, 300.0 + k * 110, -50.0 + k * 40, 1, L, lk(r, wear='shorts', sleeves='none'), z=LIFT, speed=350)
            dog(sc, 560.0, -40.0, 1, L, 350, z=LIFT, body='#5a4a3c', phase=0.4)
        else:
            def board(col):
                return P.hold_board(col, WHITE, 170.0)
            for k in range(3):                                                  # cruisers, a board under the arm
                x = -1900.0 + k * 520
                cyclist(sc, r, x, -45.0, 1, L, 470, lk(r, wear='swim', sleeves='none', s=0.98), HOT[k * 2], z=LIFT, saddle=84.0, flag=HOT[k * 2 + 1],
                        arm_r='down', hold_r=board(HOT[(k * 3 + 2) % 8]))
            for k in range(5):                                                  # the running club, in step two by two
                runner(sc, r, 1900.0 - k * 120, 40.0 + (k % 2) * 60, -1, L, lk(r, wear='shorts', top=HOT[k % 8], hat='cap', hat_col=WHITE), z=LIFT,
                       speed=330)
            dog(sc, 1250.0, 130.0, -1, L, 330, z=LIFT, body=WHITE, phase=0.7)
    return build


def circles(seed):
    """Children on bikes riding rings round a pennant mast on the sand; two dogs and a runner go round the other way;
    someone in the middle turns to watch them. Everything circles: rings of 5 to 9 m."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(WHITE, (0, 0, 260), (8, 8, 520))
        g.cyl(YEL, (0, 0, 0), 60, 14, steps=8)
        for k in range(3):
            f = sc.part(P.motion(swing=10.0, pivot=(0.0, 0.0, 470.0 - k * 70), axis=P.UP, rate=0.5, phase=k * 0.2))
            f.box(HOT[k * 2], (60, 0, 470 - k * 70), (120, 5, 50))
        fig(sc, 90.0, 60.0, lk(r, hat='sun', hat_col=HOTPINK), z=LIFT, arm_r='wave', swing_r=26, turn=50, rate=0.25, detail=1)
        for k, (R, rps, col) in enumerate(((520.0, 0.115, RED), (700.0, -0.085, SKY), (880.0, 0.07, LIME))):
            side = 1 if k % 2 == 0 else -1                                      # which end of the ring it starts at (+Y or -Y)
            d = -side if rps > 0 else side                                      # counter-clockwise at +Y heads towards -X
            x, y = 0.0, side * R
            look = lk(r, build='kid', s=0.74, hat='helmet', hat_col=HOT[(k * 3 + 1) % 8])
            m = ring_prop(sc, x, y, 0.0, 0.0, rps)
            bike(m, x, y, d, col, LIFT, 62.0)
            m.box(WHITE, (x - d * 50, y, LIFT + 95), (4, 4, 130))
            m.box(HOT[(k + 4) % 8], (x - d * 68, y, LIFT + 150), (34, 4, 22))
            fig(sc, x - d * 8, y, look, z=LIFT + 62.0 - 66.0 * 0.74, yaw=90.0 * d, arm_l='push', arm_r='push', stride=30, rate=1.5, phase=r.random(),
                orbit=(0.0, 0.0, rps), detail=1)
        for k, (R, a_deg, rps, col) in enumerate(((420.0, 30.0, -0.16, '#e8c26a'), (610.0, 200.0, -0.13, WHITE))):
            a = math.radians(a_deg)
            P.quadruped(sc, math.cos(a) * R, math.sin(a) * R, z=LIFT, yaw=a_deg + (180.0 if rps > 0 else 0.0), length=70, height=48, girth=20, neck=16,
                        neck_up=35, head=18, tail=22, wag=30, stride=32, rate=2.5, bob=6, phase=k * 0.4, orbit=(0.0, 0.0, rps), body=col, ears='#5a4a3c',
                        snout=DARK)
        a_deg, R, rps = 120.0, 790.0, -0.06
        a = math.radians(a_deg)
        fig(sc, math.cos(a) * R, math.sin(a) * R, lk(r, build='kid', s=0.7, top=HOTPINK), z=LIFT, yaw=a_deg, legs='stride', stride=40, swing_l=36,
            swing_r=-36, axis_l='x', axis_r='x', rate=1.8, bob=8, phase=0.2, orbit=(0.0, 0.0, rps), detail=1)
    return build


# ------------------------------------------------------------------------------------------ the lower beach
def riders(seed):
    """Four horses at a canter along the sand, 90 m and round again: two and two, a dog running after them."""
    def build(sc):
        r = random.Random(seed)
        L = 9000.0
        for k in range(4):
            x = -3600.0 + k * 2300.0 + r.uniform(-200, 200)
            y = (-90.0, 80.0, -40.0, 110.0)[k]
            look = lk(r, wear='trousers', bottom=(CREAM, NAVY, WHITE, '#5a4a3c')[k], top=HOT[(k * 3) % 8], hat=('helmet', 'cowboy', 'helmet', 'sun')[k],
                      hat_col=(DARK, CREAM, RED, WHITE)[k], s=1.0)
            horse_rider(sc, r, x, y, 1, L, 620.0 + k * 25, look, HORSES[k], rate=1.5, bob=9, stride=32,
                        arm_r='wave' if k == 1 else 'hold', swing_r=24 if k == 1 else 0)
        dog(sc, -4100.0, 20.0, 1, L, 640, z=LIFT, body='#e8c26a', s=1.3, phase=0.2)
    return build


def ponies(seed):
    """A pony string: four ponies nose to tail at a walk, a child on each (one waving), led by a handler in a sun hat,
    another walking at the back. 60 m and round again."""
    def build(sc):
        r = random.Random(seed)
        L, sp = 6000.0, 130.0
        runner(sc, r, 760.0, -50.0, 1, L, lk(r, wear='trousers', hat='sun', hat_col=CREAM, top=RED), z=LIFT, speed=sp, stride=24, swing_l=0,
               swing_r=18, arm_l='back', rate=0.95, bob=3)
        for k in range(4):
            x = 540.0 - k * 300.0
            look = lk(r, build='kid', s=0.66, hat='helmet', hat_col=HOT[(k * 2 + 1) % 8], top=HOT[(k * 3) % 8])
            horse_rider(sc, r, x, 0.0, 1, L, sp, look, HORSES[(k + 2) % 5], sh=0.7, rate=0.95, bob=3, stride=20,
                        arm_r='wave' if k % 2 else 'hold', swing_r=26 if k % 2 else 0)
        runner(sc, r, -700.0, 40.0, 1, L, lk(r, wear='shorts', hat='cap', hat_col=YEL), z=LIFT, speed=sp, stride=24, swing_l=18, swing_r=-18, rate=0.95,
               bob=3)
    return build


def sand_train(seed):
    """The beach train: a little engine with a funnel and two open carriages under striped canopies, eight passengers
    waving, the driver waving too. 100 m along the sand and round again. Pastel and hot colours, 2.3 m tall."""
    def build(sc):
        r = random.Random(seed)
        L, sp, d = 10000.0, 420.0, 1
        z = LIFT
        x = 500.0
        m = prop(sc, x, 0.0, L, sp * d)
        for dx in (-70, 70):
            for sy in (-58, 58):
                wheel(m, x + dx, sy, z + 30, 30, w=14)
        m.box(RED, (x + 30, 0, z + 78), (170, 104, 70))
        m.box(YEL, (x + 30, 0, z + 50), (176, 110, 10))
        m.cyl(DARK, (x + 86, 0, z + 113), 15, 46, steps=6, top_r=24)
        m.cyl(YEL, (x + 30, 0, z + 113), 20, 22, steps=6)
        m.box(RED, (x - 86, 0, z + 80), (60, 104, 74))
        for dx in (-112, -60):
            for sy in (-48, 48):
                m.box(WHITE, (x + dx, sy, z + 165), (5, 5, 100))
        m.box(YEL, (x - 86, 0, z + 220), (86, 124, 10))
        m.box(DARK, (x + 122, 0, z + 60), (20, 90, 30))
        fig(sc, x - 86, 0.0, lk(r, hat='peak', hat_col=NAVY, top=NAVY, s=0.9), z=z + 50, yaw=90.0 * d, arm_l='wave', arm_r='push', swing_l=26,
            rate=1.2, phase=0.1, travel=(L, sp * d), detail=1)
        for c in range(2):
            cx = x - 330.0 - c * 320.0
            cols = ((MINT, WHITE), (HOTPINK, WHITE))[c]
            m = prop(sc, cx, 0.0, L, sp * d)
            for dx in (-85, 85):
                for sy in (-60, 60):
                    wheel(m, cx + dx, sy, z + 24, 24, w=10, steps=6)
            m.box(cols[0], (cx, 0, z + 44), (270, 116, 10))
            for sy in (-58, 58):
                m.box(cols[0], (cx, sy, z + 72), (270, 5, 46))
                m.box(WHITE, (cx, sy, z + 98), (270, 7, 7))
            for dx in (-128, 128):
                for sy in (-56, 56):
                    m.box(WHITE, (cx + dx, sy, z + 135), (5, 5, 172))
            for k in range(4):
                m.box(cols[k % 2], (cx - 105 + k * 70, 0, z + 226), (70, 134, 10))
            m.box(STEEL, (cx + 150, 0, z + 44), (50, 8, 8))
            for row in (-55, 60):
                m.box(WOOD, (cx + row - 16, 0, z + 82), (30, 104, 8))
                for sy in (-28, 28):
                    fig(sc, cx + row, sy, lk(r, s=0.92), z=z + 48, yaw=90.0 * d, legs='sit', arm_l='up' if sy < 0 else 'down',
                        arm_r='down' if sy < 0 else 'up', swing_l=24 if sy < 0 else 0, swing_r=0 if sy < 0 else 24, rate=r.uniform(1.1, 1.6),
                        phase=r.random(), travel=(L, sp * d), detail=0)
    return build


def beach_race(seed):
    """A fun run along the sand: twelve runners in hot singlets with a white bib, each at a pace of their own (they
    string out and pass one another), a marshal on a bike with a pennant ahead of them and one behind. 110 m."""
    def build(sc):
        r = random.Random(seed)
        L = 11000.0
        cyclist(sc, r, 4600.0, 0.0, 1, L, 470, lk(r, top=YEL, hat='helmet', hat_col=WHITE, s=1.0), YEL, z=LIFT, saddle=84.0, flag=RED, detail=0)
        for k in range(12):
            x = 3600.0 - k * 640.0 + r.uniform(-150, 150)
            y = r.uniform(-160, 160)
            look = lk(r, wear='shorts', sleeves='none', top=HOT[(k * 3 + 1) % 8], band=WHITE, s=r.uniform(0.95, 1.06))
            runner(sc, r, x, y, 1, L, look, speed=r.uniform(400, 500), z=LIFT, detail=0, rate=r.uniform(1.6, 1.9), bob=8)
        cyclist(sc, r, -4700.0, 40.0, 1, L, 430, lk(r, top=ORANGE, hat='helmet', hat_col=WHITE, s=1.0), ORANGE, z=LIFT, saddle=84.0, flag=YEL,
                detail=0)
    return build


# ------------------------------------------------------------------------------------------ the water's edge
def paddlers(seed):
    """Afloat (the waterline is z = 0): a dragon boat, eight paddlers in time, a drummer at the bow beating the stroke,
    a helm standing at the stern with the long oar; three kayaks the other way, their paddles dipping left and right.
    They travel 240 m along the shore and come round again."""
    def build(sc):
        r = random.Random(seed)
        L, sp = 24000.0, 430.0
        x, y = 0.0, 0.0
        m = prop(sc, x, y, L, sp, bob=-5.0, rate=0.5)
        m.box(RED, (x, y, 14), (1000, 110, 52))
        m.box(YEL, (x, y, 36), (1004, 114, 10))
        tilt_box(m, RED, (x + 480, y, 20), (x + 640, y, 80), 80, 40)
        m.box(YEL, (x + 660, y, 100), (70, 60, 50))                             # the head: a block and two horns
        for sy in (-22, 22):
            tilt_box(m, WHITE, (x + 650, y + sy, 120), (x + 610, y + sy, 170), 10, 10)
        tilt_box(m, RED, (x - 480, y, 20), (x - 620, y, 110), 60, 30)
        m.box(YEL, (x - 640, y, 130), (30, 8, 60))
        tilt_box(m, WHITE, (x - 520, y, 4), (x - 1500, y, 6), 160, 10)             # the wake
        m.cyl(HOTPINK, (x + 400, y, 40), 30, 44, steps=6)                        # the drum

        def paddle(col):
            def f(mm, h, side):
                tilt_box(mm, WOOD, (h[0], h[1], h[2] + 26), (h[0] + side * 26, h[1] - 16, h[2] - 96), 6, 6)
                mm.box(col, (h[0] + side * 30, h[1] - 18, h[2] - 112), (8, 22, 44))
            return f

        for k in range(8):
            row, side = k // 2, (-1 if k % 2 else 1)
            look = lk(r, wear='shorts', sleeves='none', top=(SKY, YEL)[row % 2], hat='cap', hat_col=WHITE, s=1.0)
            fig(sc, x + 250.0 - row * 180.0, y + side * 30.0, look, z=-30.0, yaw=90.0, legs='sit', arm_l='fwd', arm_r='fwd',
                hold_l=paddle(YEL) if side < 0 else None, hold_r=paddle(YEL) if side > 0 else None, swing_l=30, swing_r=30, axis_l='x', axis_r='x',
                rate=1.0, phase=0.0, bob=-5, travel=(L, sp), detail=0)
        fig(sc, x + 455.0, y, lk(r, top=HOTPINK, hat='sun', hat_col=YEL, s=1.0), z=-30.0, yaw=-90.0, legs='sit', arm_l='up', arm_r='up', swing_l=30,
            swing_r=-30, axis_l='x', axis_r='x', rate=2.0, phase=0.0, travel=(L, sp), detail=0)

        def oar(mm, h, side):
            tilt_box(mm, WOOD, (h[0], h[1], h[2] + 30), (h[0] - 10, h[1] + 230, h[2] - 150), 7, 7)
        fig(sc, x - 430.0, y, lk(r, top=WHITE, hat='cap', hat_col=RED, s=1.0), z=34.0, yaw=90.0, legs='wide', arm_l='hip', arm_r='hold', hold_r=oar,
            rock=4, rate=0.5, travel=(L, sp), detail=0)
        for k in range(3):                                                      # the kayaks, coming the other way
            kx, ky, kv = 1500.0 + k * 900.0, 600.0 + k * 260.0, -(300.0 + k * 30.0)
            col = (LIME, ORANGE, VIOLET)[k]
            ph = k * 0.3
            m = prop(sc, kx, ky, L, kv, bob=-5.0, rate=0.6, phase=ph)
            m.box(col, (kx, ky, 12), (360, 64, 30))
            tilt_box(m, col, (kx - 170, ky, 12), (kx - 260, ky, 26), 36, 20)
            tilt_box(m, col, (kx + 170, ky, 12), (kx + 250, ky, 22), 36, 20)
            tilt_box(m, WHITE, (kx + 200, ky, 3), (kx + 800, ky, 5), 90, 8)
            fig(sc, kx, ky, lk(r, top=HOT[(k * 3 + 2) % 8], wear='shorts', sleeves='none', hat='cap', hat_col=HOT[k], s=1.0), z=-26.0, yaw=-90.0,
                legs='sit', arm_l='fwd', arm_r='fwd', rate=0.6, phase=ph, bob=-5, travel=(L, kv), detail=0)
            p = sc.part(P.motion(swing=26.0, pivot=(kx - 34.0, ky, 104.0), axis=0.0, rate=0.8, phase=ph, base=(kx, ky), travel=(L, kv / L)))
            p.box(WOOD, (kx - 34, ky, 104), (7, 250, 7))
            for sy in (-1, 1):
                p.box(WHITE, (kx - 34, ky + sy * 125, 104), (9, 40, 26))
    return build


# ------------------------------------------------------------------------------------------ the kit
LOWB = dict(grow=(120.0, 40.0, 12.0), grow_down=(120.0, 40.0, 0.0))            # by the road: the bounds stay tight (a hop of 7 cm at most)
PATH = dict(grow=(200.0, 60.0, 30.0), grow_down=(200.0, 60.0, 0.0))
RING = dict(grow=(950.0, 120.0, 40.0), grow_down=(950.0, 120.0, 0.0))          # the rings are built at their two ends: 9 m either way
FARX = dict(far=True, grow=(1200.0, 80.0, 60.0), grow_down=(1200.0, 80.0, 0.0))
SEA = dict(far=True, grow=(12000.0, 100.0, 60.0), grow_down=(12000.0, 100.0, 40.0))

JOBS = [
    ('SM_P3C_Wheels_A', wheels(301, 0, 3800.0, 0.0), LOWB),
    ('SM_P3C_Wheels_B', wheels(302, 1, 3800.0, 0.0), LOWB),
    ('SM_P3C_Wheels_C', wheels(303, 2, 3600.0, LIFT), LOWB),
    ('SM_P3C_Wheels_D', wheels(304, 3, 3600.0, LIFT), LOWB),
    ('SM_P3C_BeachPath_A', beach_path(311, 0), PATH),
    ('SM_P3C_BeachPath_B', beach_path(312, 1), PATH),
    ('SM_P3C_Circles', circles(321), RING),
    ('SM_P3C_Riders', riders(331), FARX),
    ('SM_P3C_Ponies', ponies(341), FARX),
    ('SM_P3C_SandTrain', sand_train(351), FARX),
    ('SM_P3C_BeachRace', beach_race(361), FARX),
    ('SM_P3C_Paddlers', paddlers(371), SEA),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
