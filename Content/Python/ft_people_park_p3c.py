"""FULL THROTTLE (Unreal) the THEME PARK's MOVERS (level index 4, population pass 3, builder p3c):
/Game/People/ThemePark/SM_P3C_...

Everything here TRAVELS: along the mesh's X (which the placement lays along the road) or round a point of its own,
never towards the road. The park's road has a metre of barrier and a hoarding behind it, so every piece brings its own
RAISED way (a promenade on posts, a viaduct, a trough on trestles, a rail on pylons, a rink on a drum): the people
show over the stalls. A little house stands over each end of a way, so nobody pops in or out in the open.

    SM_P3C_Walk_A        a raised promenade 3 m up: strollers both ways, a pushchair, a child and a dog, a tray of drinks.
    SM_P3C_Walk_B        the same promenade, the park's fun run: runners in chest bands on two lanes, a marshal waving them on.
    SM_P3C_Walk_C        the same, on wheels: cyclists, a pedicab with two passengers, a tandem, two roller skaters.
    SM_P3C_MiniRail      a miniature steam railway on a brick viaduct: the engine, its driver, four open carriages of
                         waving passengers, a tunnel mouth at each end.
    SM_P3C_HorseTram     a horse tram on the promenade (driver, passengers under its roof), a pony cart and a rider on
                         horseback coming the other way.
    SM_P3C_Skywalk_Lo    a high walk 8 m up for the middle distance: big walkers, runners and cyclists over the roofs.
    SM_P3C_Skywalk_Hi    the same on 27 m piers with a deep foot, for the lagoon (the road is a coaster 26 m over the valley).
    SM_P3C_LogRide_Lo    a log ride's trough on trestles 7 m up: four logs of riders, arms in the air, float along it.
    SM_P3C_LogRide_Hi    the same on 27 m trestles, for the lagoon.
    SM_P3C_SkyBikes      pedal cars on two high rails (one each way), a parasol over each, the riders pedalling.
    SM_P3C_Rink          a roller rink on a drum 6 m up: eight skaters circling its lamp post at their own speeds.
    SM_P3C_Parrots       a perch tower and a flock of great macaws wheeling round it (Pirate Cove's birds, loose in the park).
    SM_P3C_Flamingos     a skein of nine flamingos flying the length of two slots and round again (the lagoon's birds).

Rules kept: every piece is TALL and its table rule stands it 24 m or more from the centre line (28 m and beyond), so
the small steady glow that lifts the colours out of the park's night, and the rows of steady lamps, are allowed;
nothing blinks. No clown (the boss is MR. CHUCKLES), no balloons (he bombs the road with them). No words, no marks.
Movement only through ft_people's motion values (the vertex shader).

Run in the editor:  import ft_people_park_p3c; ft_people_park_p3c.run()      (or run(['SM_P3C_Rink']))
The placing rules are the block C:/ftwork/p3/L4c.inl (pasted into Source/FullThrottle/FTPeopleL4.inl by the integrator).
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/ThemePark'
NAVY, PURPLE, PLUM = '#2b2350', '#3a2f6a', '#4a3a78'
MAG, CYAN, AMBER, CREAM = '#ff4fb8', '#2ee6ff', '#ffd23f', '#fff1d6'
RED, WHITE, LAV, MINT, TEAL = '#ff2d3c', '#fff6ea', '#b9a4ff', '#7fe3c4', '#2fc6c0'
ORANGE, GOLD, WOOD, GREY_L, DARK = '#ff8a2a', '#ffc93c', '#9a6a44', '#9a98a8', '#22202a'
LIME, VIOLET, WATER, BRICK = '#b8ff5a', '#8f6bff', '#2a8fe0', '#c9604a'
PINK, HOTPINK = '#ff9ac8', '#ff2d7a'
BRIGHT = (MAG, CYAN, AMBER, RED, ORANGE, VIOLET, LIME, WHITE, MINT)
LAMP = '#ffe28a'
FOOT = 1500.0      # how far a lagoon piece's foot goes down under z = 0 (cm as built): the land falls away there


# ------------------------------------------------------------------------------------------ helpers
def lift(sc, g):
    """The park is a night level: a figure in plain colours away from the lamps is a dark shape. Every colour of a
    piece gets a small steady glow (g). Allowed because every piece of this kit stands beyond 24 m; nothing blinks."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.07):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def tv_of(L, V):
    return (float(L), float(V) / float(L))


def mover(sc, x, y, z, L, V, phase=0.0, rate=0.0, bob=0.0):
    """The model of a thing that travels the mesh's X with the people on it (a car, a cart, a log)."""
    return sc.part(P.motion(base=(float(x), float(y)), travel=tv_of(L, V), pivot=(float(x), float(y), float(z)), phase=phase, rate=rate, bob=bob))


def bulbs(m, a, b, n, size=14.0, glow=1.3, col=LAMP):
    for k in range(n):
        f = (k + 0.5) / n
        m.box(col, (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f), (size, size, size), glow=glow)


def wheel(m, x, y, z, r, w=8.0, col=DARK, steps=8):
    rod(m, col, (x, y - w / 2.0, z), (x, y + w / 2.0, z), r, steps=steps)


def end_houses(g, L, H, deep, wall=CREAM, roof=MAG, h=270.0, w=240.0):
    """A little house over each end of a way: the travellers come and go inside it."""
    for sx in (-1, 1):
        x = sx * (L / 2.0 - w / 2.0)
        g.box(wall, (x, 0, H + h / 2.0), (w, deep + 16, h))
        g.box(roof, (x, 0, H + h + 22), (w + 50, deep + 60, 44))
        g.box(roof, (x, 0, H + h + 62), (w * 0.55, deep * 0.6, 40))
        g.box(DARK, (x - sx * (w / 2.0 + 1), 0, H + 105), (4, deep * 0.62, 210))      # the doorway the way runs through


def deck(sc, L, H, deep, foot=120.0, cols=(MAG, CREAM), posts=5, stripes=6, lamps=6, rail=True, post_w=46.0, k=1.0):
    """A raised way: a deck with its top at H, a striped fascia, posts that go `foot` under the ground, a railing along
    the front and the back (k: how much bigger the far pieces' railings and lamps are drawn). Returns the still model."""
    g = sc.still()
    g.box(PLUM, (0, 0, H - 20), (L, deep, 40))
    w = L / stripes
    for i in range(stripes):
        g.box(cols[i % 2], (-L / 2.0 + (i + 0.5) * w, -deep / 2.0 - 4, H - 34), (w, 10, 70 * k))
    for i in range(posts):
        x = -L / 2.0 + (i + 0.5) * L / posts
        g.box(NAVY, (x, 0, (H - 40 - foot) / 2.0), (post_w, deep * 0.5, H - 40 + foot))
    if rail:
        t = 8.0 * k
        for sy in (-1, 1):
            g.box(CREAM, (0, sy * (deep / 2.0 - 6), H + 100 * k), (L, t, t))
        n = 7
        for i in range(n):
            g.box(CREAM, (-L / 2.0 + i * L / (n - 1), -deep / 2.0 + 6, H + 50 * k), (t, t, 100 * k))
    if lamps:
        bulbs(g, (-L / 2.0, -deep / 2.0 - 11, H - 82 * k), (L / 2.0, -deep / 2.0 - 11, H - 82 * k), lamps, size=14.0 * k)
    return g


def walker(sc, r, x, y, z, L, V, fast=False, **kw):
    """One person travelling the way: facing the way they go, legs and arms swinging, out of step with the rest."""
    d = P.look(r, tops=BRIGHT)
    st = 34 if fast else 24
    d.update(yaw=(90 if V > 0 else -90), stride=st, swing_l=st * 0.8, swing_r=-st * 0.8, axis_l='x', axis_r='x',
             rate=(2.3 if fast else 1.5) * r.uniform(0.9, 1.1), bob=(7.0 if fast else 4.0), phase=r.random(), travel=(L, V), detail=1)
    d.update(kw)
    return P.figure(sc, x, y, z, **d)


def bike(sc, r, x, y, z, L, V, col, s=1.0, detail=1, **kw):
    """A bicycle and its rider, travelling."""
    d = 1.0 if V > 0 else -1.0
    m = mover(sc, x, y, z, L, V)
    R = 32.0 * s
    for wx in (-50, 50):
        wheel(m, x + wx * s, y, z + R, R, 8.0 * s)
    seat = (x - d * 14 * s, y, z + 84 * s)
    head = (x + d * 34 * s, y, z + 88 * s)
    tilt_box(m, col, (x - d * 50 * s, y, z + R), seat, 7 * s)
    tilt_box(m, col, seat, head, 7 * s)
    tilt_box(m, col, head, (x + d * 50 * s, y, z + R), 7 * s)
    tilt_box(m, col, seat, (x + d * 4 * s, y, z + R), 7 * s)
    m.box(DARK, (head[0], y, head[2] + 12 * s), (7 * s, 46 * s, 7 * s))
    m.box(DARK, (seat[0], y, seat[2] + 6 * s), (26 * s, 12 * s, 7 * s))
    look = P.look(r, tops=BRIGHT, hat='helmet', hat_col=r.choice(BRIGHT), s=s)
    look.update(yaw=(90 if V > 0 else -90), arm_l='push', arm_r='push', stride=30, rate=2.2, phase=r.random(), travel=(L, V), detail=detail,
                wear='shorts')
    look.update(kw)
    P.figure(sc, x - d * 12 * s, y, z + 26 * s, **look)


# ------------------------------------------------------------------------------------------ the promenade (near)
WALK_L, WALK_H, WALK_D = 4200.0, 300.0, 300.0


def promenade(sc, cols, roof):
    g = deck(sc, WALK_L, WALK_H, WALK_D, cols=cols)
    end_houses(g, WALK_L, WALK_H, WALK_D, roof=roof)
    return g


def walk_a(sc):
    """Strollers on the raised promenade: a front lane going one way (a couple, a pushchair, a child running with a
    dog, a waiter with a tray), a back lane coming the other way."""
    r = random.Random(3101)
    promenade(sc, (MAG, CREAM), MAG)
    L, H, S = WALK_L, WALK_H, 1.12
    V = 130.0
    y = -70.0
    # the couple
    walker(sc, r, 1500, y - 20, H, L, V, hat='sun', hat_col=AMBER, wear='dress', hair='long', s=S, arm_l='wave', swing_l=20, axis_l='y', react=True)
    walker(sc, r, 1500, y + 45, H, L, V, hat='cap', hat_col=CYAN, s=S * 1.04, hold_r=P.hold_cup())
    # the pushchair and its pusher
    px = 620.0
    walker(sc, r, px, y, H, L, V, arm_l='push', arm_r='push', swing_l=0, swing_r=0, s=S, wear='skirt', hair='bun')
    m = mover(sc, px + 78, y, H, L, V)
    m.box(CYAN, (px + 78, y, H + 62), (62, 50, 44))
    m.box(AMBER, (px + 62, y, H + 100), (40, 54, 34))
    m.box(GREY_L, (px + 40, y, H + 100), (6, 46, 6))
    for wx in (56, 100):
        wheel(m, px + wx, y, H + 16, 16, 44, steps=6)
    # a child running with a dog
    walker(sc, r, -260, y - 10, H, L, V, fast=True, build='kid', s=0.74, arm_r='up', hat='cap', hat_col=RED)
    P.quadruped(sc, -150, y + 40, H, yaw=90, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=24,
                rate=2.6, bob=5.0, travel=(L, V), body='#e8c26a', s=1.2)
    # the waiter with a tray of drinks
    def tray(m2, h, side):
        m2.box(WHITE, (h[0], h[1] - 8, h[2] + 9), (48, 34, 4))
        for dx in (-12, 10):
            m2.cyl(r.choice((MAG, CYAN, LIME)), (h[0] + dx, h[1] - 8, h[2] + 11), 6, 18, steps=6)
    walker(sc, r, -1250, y, H, L, V, top=WHITE, bottom=NAVY, band=MAG, hat='peak', hat_col=MAG, arm_r='hold', swing_r=0, hold_r=tray, s=S)
    walker(sc, r, -1820, y + 20, H, L, V, hold_l=P.hold_bag(LIME), s=S, wear='coat', hair='afro')
    # the back lane, the other way
    for k, x in enumerate((-1500, -700, 250, 1050, 1750)):
        kw = dict(detail=0, s=S)
        if k == 1:
            kw.update(build='kid', s=0.76, fast=True)
        if k == 3:
            kw.update(arm_r='wave', swing_r=22, axis_r='y', react=True)
        fast = kw.pop('fast', False)
        walker(sc, r, x, 78 + (k % 2) * 30, H, L, -110.0 if not fast else -110.0, fast=fast, **kw)


def walk_b(sc):
    """The park's fun run on the promenade: runners in chest bands on two lanes (the back lane slower), a marshal
    with a flag waving them on from a box."""
    r = random.Random(3102)
    g = promenade(sc, (CYAN, CREAM), CYAN)
    L, H, S = WALK_L, WALK_H, 1.12
    bands = (WHITE, AMBER, MAG, LIME)
    for k in range(7):
        x = -1800 + k * 560 + r.uniform(-90, 90)
        walker(sc, r, x, -85 + (k % 2) * 50, H, L, 420.0, fast=True, wear='shorts', sleeves='none', band=bands[k % 4], s=S * r.uniform(0.95, 1.05),
               hat=('cap' if k % 3 == 0 else None), hat_col=r.choice(BRIGHT), detail=(1 if k % 2 == 0 else 0))
    for k in range(5):
        x = -1700 + k * 800 + r.uniform(-120, 120)
        walker(sc, r, x, 80, H, L, 300.0, fast=True, wear='shorts', band=bands[(k + 1) % 4], s=S * r.uniform(0.92, 1.04), detail=0,
               arm_l=('up' if k == 2 else 'down'), arm_r=('up' if k == 2 else 'down'))
    # the marshal on a box by the front rail
    g.box(AMBER, (0, -118, H + 20), (70, 46, 40))
    P.figure(sc, 0, -118, H + 40, yaw=20, arm_r='up', swing_r=28, axis_r='y', hold_r=P.hold_flag(MAG, CREAM, 70.0), arm_l='hip', rate=1.4, bob=4.0,
             react=True, **P.look(r, top=LIME, bottom=NAVY, band=WHITE, hat='cap', hat_col=WHITE, s=S))


def walk_c(sc):
    """Wheels on the promenade: two cyclists and a pedicab with its two passengers one way; a tandem and two roller
    skaters the other."""
    r = random.Random(3103)
    promenade(sc, (AMBER, CREAM), ORANGE)
    L, H, S = WALK_L, WALK_H, 1.12
    V = 470.0
    y = -72.0
    bike(sc, r, 1500, y, H, L, V, MAG, s=S)
    bike(sc, r, 300, y + 30, H, L, V, CYAN, s=S)
    # the pedicab: its rider in front, a bench for two under a canopy behind
    x = -1000.0
    bike(sc, r, x + 150, y, H, L, V, AMBER, s=S, top=WHITE, band=MAG)
    m = mover(sc, x, y, H, L, V)
    m.box(MAG, (x - 20, y, H + 62), (150, 110, 46))
    m.box(CREAM, (x - 86, y, H + 120), (18, 110, 80))
    for sy in (-1, 1):
        wheel(m, x - 30, y + sy * 60, H + 34, 34)
        m.box(CREAM, (x - 86, y + sy * 50, H + 190), (8, 8, 90))
    m.box(LIME, (x - 30, y, H + 240), (170, 130, 12))
    m.box(CREAM, (x + 56, y, H + 236), (10, 130, 26))
    for sy, top in ((-1, CYAN), (1, AMBER)):
        P.figure(sc, x - 30, y + sy * 26, H + 52, yaw=90, detail=1, arm_r=('wave' if sy < 0 else 'down'), swing_r=(22 if sy < 0 else 0), axis_r='y',
                 rate=1.3, phase=r.random(), travel=(L, V), react=True, **P.look(r, top=top, s=0.95))
    # the other way: a tandem, two skaters
    yb = 84.0
    xt = 900.0
    bike(sc, r, xt, yb, H, L, -380.0, LIME, s=S, detail=0)
    bike(sc, r, xt + 110, yb, H, L, -380.0, LIME, s=S, detail=0)
    for k, xs in enumerate((-600, -1150)):
        walker(sc, r, xs, yb + (k * 2 - 1) * 18, H + 7, L, -380.0, stride=20, rate=1.1, bob=0.0, arm_l='out', arm_r='out', swing_l=14, swing_r=14,
               axis_l='y', axis_r='y', hat='helmet', hat_col=(MAG, CYAN)[k], wear='shorts', s=S, detail=0)
        m2 = mover(sc, xs, yb + (k * 2 - 1) * 18, H, L, -380.0)
        m2.box(DARK, (xs, yb + (k * 2 - 1) * 18, H + 4), (34, 40, 8))


# ------------------------------------------------------------------------------------------ the miniature railway
def mini_rail(sc):
    """A miniature steam railway on a brick viaduct: the engine and its driver, four open carriages of passengers
    waving, a tunnel mouth at each end."""
    r = random.Random(3104)
    L, H, D = 4200.0, 260.0, 230.0
    g = sc.still()
    g.box(BRICK, (0, 0, (H - 120.0) / 2.0), (L, D, H + 120.0))
    g.box(CREAM, (0, 0, H - 8), (L, D + 24, 16))
    n = 7
    for k in range(n):
        x = -L / 2.0 + (k + 0.5) * L / n
        g.box(NAVY, (x, -D / 2.0 - 2, H * 0.36), (L / n * 0.56, 6, H * 0.72))
        g.box(NAVY, (x, -D / 2.0 - 2, H * 0.74), (L / n * 0.38, 6, H * 0.12))
    g.box(GREY_L, (0, -46, H + 4), (L, 8, 8))
    g.box(GREY_L, (0, 46, H + 4), (L, 8, 8))
    bulbs(g, (-L / 2.0, -D / 2.0 - 14, H - 30), (L / 2.0, -D / 2.0 - 14, H - 30), 7)
    for sx in (-1, 1):      # the tunnel mouths
        x = sx * (L / 2.0 - 130)
        g.box(PURPLE, (x, 0, H + 150), (260, D + 40, 300))
        g.box(MINT, (x, 0, H + 320), (300, D + 70, 40))
        g.box(DARK, (x - sx * 131, 0, H + 110), (4, 150, 220))
    V = 360.0
    x0 = 900.0
    m = mover(sc, x0, 0, H, L, V)
    rod(m, RED, (x0 - 95, 0, H + 96), (x0 + 110, 0, H + 96), 50, steps=8)
    m.box(DARK, (x0, 0, H + 40), (250, 96, 26))
    m.box(RED, (x0 - 130, 0, H + 130), (80, 112, 150))
    m.box(GOLD, (x0 - 130, 0, H + 214), (100, 126, 14))
    m.cyl(DARK, (x0 + 78, 0, H + 140), 16, 60, steps=6, top_r=24)
    m.cyl(GOLD, (x0 + 10, 0, H + 140), 18, 26, steps=6)
    tilt_box(m, AMBER, (x0 + 110, 0, H + 52), (x0 + 158, 0, H + 14), 90, 10)
    for wx in (-70, 0, 70):
        for sy in (-1, 1):
            wheel(m, x0 + wx, sy * 50, H + 30, 30, 8, col=GOLD, steps=6)
    # the steam: two puffs that hop over the funnel, with the train
    for k, (dx, sz) in enumerate(((60, 34), (20, 46))):
        st = sc.part(P.motion(base=(x0, 0.0), travel=tv_of(L, V), pivot=(x0 + dx, 0.0, H + 230.0), phase=k * 0.5, rate=1.1, bob=46.0))
        st.sphere(WHITE, (x0 + dx, 0, H + 236 + k * 40), sz, steps=(3, 6))
    P.figure(sc, x0 - 132, 0, H + 62, yaw=90, detail=0, arm_l='wave', swing_l=24, axis_l='y', rate=1.2, travel=(L, V), react=True,
             **P.look(r, top=NAVY, bottom=NAVY, hat='peak', hat_col=RED, band=GOLD, s=0.84))
    cols = (CYAN, AMBER, MAG, LIME)
    for k in range(4):
        x = x0 - 390 - k * 320
        c = mover(sc, x, 0, H, L, V)
        c.box(cols[k], (x, 0, H + 62), (280, 116, 56))
        c.box(CREAM, (x, 0, H + 94), (290, 124, 10))
        c.box(DARK, (x + 150, 0, H + 44), (30, 10, 10))
        for wx in (-90, 90):
            for sy in (-1, 1):
                wheel(c, x + wx, sy * 52, H + 24, 24, 8, steps=6)
        for j, dx in enumerate((-62, 62)):
            kid = (k + j) % 3 == 0
            d = P.look(r, tops=BRIGHT, s=(0.8 if kid else 1.0), build=('kid' if kid else r.choice(('avg', 'slim', 'stocky'))))
            act = (dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, axis_l='y', axis_r='y'), dict(arm_l='wave', swing_l=24, axis_l='y'),
                   dict(arm_r='wave', swing_r=24, axis_r='y'), dict(arm_r='point'))[(k * 2 + j) % 4]
            d.update(act)
            P.figure(sc, x + dx, (j * 2 - 1) * 14, H + 44, yaw=(0 if j else 90), detail=(1 if j == 0 else 0), rate=1.3, bob=3.0, phase=r.random(), travel=(L, V),
                     react=True, **d)


# ------------------------------------------------------------------------------------------ the horse tram
def horse_tram(sc):
    """A horse tram on the promenade: the horse, the driver with the reins, passengers under its roof. A pony cart
    with two children and a rider on horseback come the other way on the back lane."""
    r = random.Random(3105)
    promenade(sc, (LIME, CREAM), TEAL)
    L, H = WALK_L, WALK_H
    V = 230.0
    y = -62.0
    x = 300.0
    P.quadruped(sc, x + 400, y, H, yaw=90, stride=22, rate=1.5, bob=4.0, wag=14, travel=(L, V), body='#f4f1ea', mane='#cfcfd8', s=1.05)
    m = mover(sc, x, y, H, L, V)
    m.box(RED, (x, y, H + 62), (430, 150, 40))
    m.box(GOLD, (x, y - 76, H + 62), (430, 4, 14))
    for wx in (-140, 140):
        for sy in (-1, 1):
            wheel(m, x + wx, y + sy * 70, H + 30, 30, 8, col=GOLD, steps=6)
    for cx in (-205, 0, 205):
        for sy in (-1, 1):
            m.box(CREAM, (x + cx, y + sy * 68, H + 170), (8, 8, 180))
    m.box(CREAM, (x, y, H + 266), (470, 170, 14))
    m.box(RED, (x, y - 85, H + 250), (470, 6, 24))
    m.box(DARK, (x + 290, y, H + 74), (150, 8, 8))            # the shaft to the horse
    P.figure(sc, x + 180, y, H + 82, yaw=90, detail=1, arm_l='fwd', arm_r='fwd', rate=1.2, bob=2.0, travel=(L, V),
             **P.look(r, top=NAVY, bottom=CREAM, hat='peak', hat_col=RED, band=GOLD, s=0.92))
    acts = (dict(arm_l='wave', swing_l=24, axis_l='y'), dict(arm_r='up', swing_r=18, axis_r='y'), dict(arm_r='point'), dict(arm_l='up', arm_r='up'),
            dict(arm_r='wave', swing_r=24, axis_r='y'))
    for k in range(5):
        d = P.look(r, tops=BRIGHT, s=(0.7 if k == 2 else 0.9), build=('kid' if k == 2 else 'avg'))
        d.update(acts[k])
        P.figure(sc, x + 100 - k * 72, y + (k % 2) * 40 - 30, H + 82, yaw=(0 if k % 2 == 0 else 90), detail=(1 if k % 2 == 0 else 0), rate=1.3, bob=2.0,
                 phase=r.random(), travel=(L, V), react=True, **d)
    # the back lane: a pony cart, and a rider
    yb, Vb = 88.0, -200.0
    xc = -900.0
    P.quadruped(sc, xc - 190, yb, H, yaw=-90, stride=26, rate=2.0, bob=4.0, wag=20, travel=(L, Vb), body='#8e5a3a', mane='#3b2a22', s=0.68)
    c = mover(sc, xc, yb, H, L, Vb)
    c.box(CYAN, (xc, yb, H + 60), (150, 100, 44))
    c.box(DARK, (xc - 110, yb, H + 56), (90, 8, 8))
    for sy in (-1, 1):
        wheel(c, xc, yb + sy * 54, H + 34, 34, 8, col=AMBER)
    for j in (0, 1):
        P.figure(sc, xc - 30 + j * 60, yb, H + 50, yaw=-90, detail=0, arm_r=('up' if j else 'fwd'), arm_l='fwd', swing_r=(16 if j else 0), axis_r='y', rate=1.4,
                 phase=r.random(), travel=(L, Vb), **P.look(r, tops=BRIGHT, build='kid', s=0.78))
    xr = 1100.0
    top = P.quadruped(sc, xr, yb - 10, H, yaw=-90, stride=24, rate=1.6, bob=5.0, wag=14, travel=(L, Vb), body='#5e3b28', mane=DARK)
    P.figure(sc, xr + 10, yb - 10, H + 52, yaw=-90, detail=1, legs='wide', arm_l='fwd', arm_r='wave', swing_r=22, axis_r='y', rate=1.6, bob=5.0,
             travel=(L, Vb), react=True, **P.look(r, top=MAG, bottom=CREAM, hat='cowboy', hat_col=CREAM, s=1.0))


# ------------------------------------------------------------------------------------------ the high walk (middle distance)
def _skywalk(sc, H, foot, seed, cols, roof):
    r = random.Random(seed)
    L, D = 4200.0, 340.0
    S = 1.5
    hi = foot > 500.0
    g = deck(sc, L, H, D, foot=foot, cols=cols, posts=(3 if hi else 4), stripes=8, lamps=8, post_w=(150.0 if hi else 70.0), k=1.6)
    end_houses(g, L, H, D, roof=roof, h=400.0, w=320.0)
    if hi:      # masts over the piers, a lamp on each
        for i in range(3):
            x = -L / 2.0 + (i + 0.5) * L / 3.0
            g.box(NAVY, (x, D / 2.0 - 30, H + 330), (60, 60, 660))
            g.box(LAMP, (x, D / 2.0 - 30, H + 690), (90, 90, 60), glow=1.3)
    y = -80.0
    # the front lane: walkers, a child on shoulders' height waving, a runner
    walker(sc, r, 1500, y, H, L, 200.0, detail=0, s=S, arm_l='wave', swing_l=24, axis_l='y', react=True, hat='sun', hat_col=AMBER)
    walker(sc, r, 1380, y + 70, H, L, 200.0, detail=0, s=S * 1.03)
    walker(sc, r, 500, y, H, L, 200.0, detail=0, s=S, hold_r=P.hold_flag(MAG, CREAM, 80.0), arm_r='fwd', swing_r=0)
    walker(sc, r, 380, y + 60, H, L, 200.0, detail=0, s=S * 0.72, build='kid', fast=True, arm_r='up')
    walker(sc, r, -520, y + 20, H, L, 200.0, detail=0, s=S, wear='dress', hair='long', arm_r='vee', arm_l='vee', swing_l=12, swing_r=-12,
           axis_l='y', axis_r='y', react=True)
    walker(sc, r, -1350, y, H, L, 200.0, detail=0, s=S, hold_l=P.hold_bag(CYAN))
    walker(sc, r, -1480, y + 70, H, L, 200.0, detail=0, s=S, hat='cap', hat_col=RED)
    # the back lane: two cyclists and three runners, the other way
    bike(sc, r, 1100, 96, H, L, -620.0, CYAN, s=S, detail=0)
    bike(sc, r, -300, 96, H, L, -620.0, MAG, s=S, detail=0)
    for k, x in enumerate((1750, 300, -1100)):
        walker(sc, r, x, 40, H, L, -430.0, fast=True, detail=0, s=S, wear='shorts', band=(WHITE, AMBER, LIME)[k])


def skywalk_lo(sc):
    """A high walk 8 m up, over the stalls' roofs: big walkers on the front lane (a flag, a child running, a wave for
    the chase), cyclists and runners coming back on the other."""
    _skywalk(sc, 800.0, 150.0, 3106, (MAG, CREAM), VIOLET)


def skywalk_hi(sc):
    """The lagoon's high walk: the same people on a deck 27 m over the valley floor (about the coaster's own height),
    on three piers with a deep foot, a lamp mast on each."""
    _skywalk(sc, 2700.0, FOOT, 3107, (CYAN, CREAM), MAG)


# ------------------------------------------------------------------------------------------ the log ride
def _log_ride(sc, H, foot, seed):
    r = random.Random(seed)
    L, D = 4000.0, 260.0
    S = 1.45
    hi = foot > 500.0
    g = sc.still()
    g.box(WATER, (0, 0, H - 12), (L, D, 24))
    for sy in (-1, 1):
        g.box(WOOD, (0, sy * (D / 2.0 + 8), H + 12), (L, 18, 76))
    g.box(CREAM, (0, -D / 2.0 - 18, H + 46), (L, 6, 12))
    n = 3 if hi else 5
    for i in range(n):
        x = -L / 2.0 + (i + 0.5) * L / n
        w = 130.0 if hi else 60.0
        for sy in (-1, 1):
            g.box(WOOD, (x, sy * D * 0.36, (H - 24 - foot) / 2.0), (w, w * 0.6, H - 24 + foot))
        g.box('#7a5234', (x, 0, H - 90), (w * 0.7, D * 0.8, 30))
    bulbs(g, (-L / 2.0, -D / 2.0 - 22, H - 20), (L / 2.0, -D / 2.0 - 22, H - 20), 8, size=22.0)
    for sx in (-1, 1):      # a mill house over each end, a pitched roof
        x = sx * (L / 2.0 - 170)
        g.box('#7a5234', (x, 0, H + 190), (340, D + 60, 380))
        g.box(RED, (x, 0, H + 400), (400, D + 120, 44))
        g.box(RED, (x, 0, H + 446), (240, D * 0.7, 48))
        g.box(DARK, (x - sx * 171, 0, H + 130), (4, D * 0.7, 230))
    V = 330.0
    acts = (dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, axis_l='y', axis_r='y'), dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12,
            axis_l='y', axis_r='y'), dict(arm_r='wave', swing_r=26, axis_r='y'), dict(arm_l='up', arm_r='point'))
    for k in range(4):
        x = 1500.0 - k * 1000.0
        ph = k * 0.27
        m = mover(sc, x, 0, H, L, V, phase=ph, rate=0.6, bob=-9.0)
        m.box('#8e5a3a', (x, 0, H + 44), (420, 170, 88))
        m.box('#6a4428', (x, 0, H + 92), (340, 120, 10))
        m.box('#b87a50', (x + 222, 0, H + 44), (26, 150, 76))
        m.box('#b87a50', (x - 222, 0, H + 44), (26, 150, 76))
        m.box(WHITE, (x - 250, 0, H + 6), (70, 200, 12))          # the wash behind it
        for j in range(2 + (k % 2)):
            d = P.look(r, tops=BRIGHT, s=S * (0.74 if (k + j) % 4 == 3 else 1.0), build=('kid' if (k + j) % 4 == 3 else 'avg'))
            d.update(acts[(k + j) % 4])
            P.figure(sc, x + 110 - j * 130, 0, H + 40, yaw=90, detail=0, rate=0.6, bob=-9.0, phase=ph, travel=(L, V), react=True, **d)


def log_ride_lo(sc):
    """A log ride's trough on trestles 7 m up: four logs of riders with their arms in the air float along it, from
    one mill house to the other."""
    _log_ride(sc, 700.0, 150.0, 3108)


def log_ride_hi(sc):
    """The lagoon's log ride: the same trough 27 m over the valley floor on three tall trestles with a deep foot."""
    _log_ride(sc, 2700.0, FOOT, 3109)


# ------------------------------------------------------------------------------------------ pedal cars on rails
def sky_bikes(sc):
    """Pedal cars on two high rails, one each way: a rider (or two) pedalling under a parasol in each."""
    r = random.Random(3110)
    L, H = 4000.0, 760.0
    S = 1.4
    g = sc.still()
    for sy in (-1, 1):
        g.box(CREAM, (0, sy * 120, H - 12), (L, 24, 24))
    for i in range(4):
        x = -L / 2.0 + (i + 0.5) * L / 4.0
        g.box(VIOLET, (x, 0, (H - 60 - 150) / 2.0), (70, 70, H - 60 + 150))
        g.box(VIOLET, (x, 0, H - 44), (60, 330, 40))
        g.box(LAMP, (x, 0, H + 6), (40, 40, 40), glow=1.3)
    for sx in (-1, 1):      # a station at each end
        x = sx * (L / 2.0 - 160)
        g.box(MINT, (x, 0, H + 180), (320, 420, 400))
        g.box(MAG, (x, 0, H + 400), (380, 480, 40))
        g.box(NAVY, (x, 0, (H - 20 - 150) / 2.0), (200, 200, H - 20 + 150))
        g.box(DARK, (x - sx * 161, 0, H + 150), (4, 340, 280))
    cols = (MAG, CYAN, AMBER, LIME, ORANGE, RED)
    k = 0
    for sy, V in ((-1, 300.0), (1, -260.0)):
        y = sy * 120.0
        dd = 1.0 if V > 0 else -1.0
        for i in range(3):
            x = -1300.0 + i * 1300.0 + sy * 250.0
            col = cols[k % 6]
            m = mover(sc, x, y, H, L, V)
            m.box(col, (x, y, H + 40), (230, 120, 70))
            tilt_box(m, col, (x + dd * 115, y, H + 40), (x + dd * 190, y, H + 70), 100, 30)
            m.box(DARK, (x - dd * 130, y, H + 60), (12, 110, 110))          # the paddle at the back
            m.box(CREAM, (x - dd * 60, y, H + 190), (10, 10, 240))
            m.cyl(cols[(k + 3) % 6], (x - dd * 60, y, H + 300), 150, 50, steps=8, top_r=12)
            two = (k % 3 == 1)
            for j in range(2 if two else 1):
                d = P.look(r, tops=BRIGHT, s=S * (0.76 if (two and j) else 1.0), build=('kid' if (two and j) else 'avg'))
                d.update(arm_l=('up' if j else 'push'), arm_r=('wave' if (k % 2 == 0) else 'push'), swing_r=(24 if k % 2 == 0 else 0), axis_r='y')
                P.figure(sc, x + dd * (30 - j * 90), y, H + 36, yaw=(90 if V > 0 else -90), detail=0, stride=30, rate=1.8, phase=r.random(),
                         travel=(L, V), react=True, **d)
            k += 1


# ------------------------------------------------------------------------------------------ the roller rink
def rink(sc):
    """A roller rink on a drum 6 m up: eight skaters circle its lamp post, each ring at its own speed, arms out, a
    pair hand in hand, one skating backwards with her arms in the air."""
    r = random.Random(3111)
    H, R = 600.0, 720.0
    S = 1.45
    g = sc.still()
    g.cyl(PURPLE, (0, 0, -150), R * 0.82, H - 40 + 150, steps=12)
    g.cyl(MINT, (0, 0, H - 40), R, 40, steps=14)
    g.cyl(MAG, (0, 0, H - 90), R + 14, 50, steps=14)
    for k in range(12):
        a = math.radians(k * 30 + 15)
        g.box(CREAM, (math.cos(a) * (R - 14), math.sin(a) * (R - 14), H + 55), (12, 12, 110))
        g.box(LAMP, (math.cos(a) * (R - 14), math.sin(a) * (R - 14), H + 124), (26, 26, 26), glow=1.3)
    g.cyl(CREAM, (0, 0, H), 34, 260, steps=6)
    g.cyl(MAG, (0, 0, H + 260), 34, 200, steps=6)
    g.cyl(CYAN, (0, 0, H + 460), 150, 90, steps=8, top_r=20)
    g.box(LAMP, (0, 0, H + 420), (120, 120, 50), glow=1.3)
    rings = ((250.0, 0.20), (430.0, 0.15), (600.0, 0.11))
    spec = ((0, 20, dict(arm_l='out', arm_r='out')), (0, 200, dict(arm_l='up', arm_r='up', back=True)),
            (1, 80, dict(arm_l='out', arm_r='out', hand=True)), (1, 92, dict(arm_l='out', arm_r='out', hand=True)), (1, 250, dict(arm_r='wave', swing_r=24)),
            (2, 0, dict(arm_l='out', arm_r='out')), (2, 130, dict(arm_l='vee', arm_r='vee')), (2, 235, dict(arm_r='point', kid=True)))
    for ring, deg, act in spec:
        rad, rate = rings[ring]
        act = dict(act)
        back = act.pop('back', False)
        act.pop('hand', None)
        kid = act.pop('kid', False)
        a = math.radians(deg)
        d = P.look(r, tops=BRIGHT, s=S * (0.74 if kid else 1.0), build=('kid' if kid else r.choice(('avg', 'slim'))), wear=r.choice(('shorts', 'skirt', 'trousers')),
                   hat=r.choice((None, 'helmet', 'cap')), hat_col=r.choice(BRIGHT))
        d.update(act)
        d.setdefault('swing_l', 12)
        d.setdefault('swing_r', -12)
        P.figure(sc, math.cos(a) * rad, math.sin(a) * rad, H, yaw=deg + 180.0 + (180.0 if back else 0.0), detail=0, stride=18, axis_l='y', axis_r='y',
                 rate=1.1, bob=3.0, phase=r.random(), orbit=(0.0, 0.0, rate), react=True, **d)


# ------------------------------------------------------------------------------------------ birds
def parrots(sc):
    """A perch tower with three great macaws on its arms, and nine more wheeling round it in three rings."""
    r = random.Random(3112)
    g = sc.still()
    T = 1000.0
    g.box(WOOD, (0, 0, (T - 200) / 2.0), (70, 70, T + 200))
    g.box(GOLD, (0, 0, T + 20), (120, 120, 40))
    perches = ((0.0, 620.0), (90.0, 800.0), (0.0, 960.0))
    pal = ((RED, CYAN), (CYAN, AMBER), (LIME, RED), (AMBER, CYAN), (RED, AMBER), (VIOLET, LIME))
    for k, (yaw, z) in enumerate(perches):
        a = math.radians(yaw)
        g.box(WOOD, (0, 0, z), (abs(math.cos(a)) * 560 + 30, abs(math.sin(a)) * 560 + 30, 26))
        sx = 1 if k % 2 == 0 else -1
        px, py = math.cos(a) * 230 * sx, math.sin(a) * 230 * sx
        body, wing = pal[k]
        P.bird(sc, px, py, z + 70, heading=-90.0, span=250.0, body=body, wing=wing, beak=AMBER, phase=k * 0.3, rate=0.8, flap=6.0, bob=10.0)
    rings = ((620.0, 900.0, 0.17), (1000.0, 1250.0, 0.13), (1400.0, 1600.0, 0.10))
    for k in range(9):
        rad, z, rate = rings[k % 3]
        deg = k * 40.0 + r.uniform(-12, 12)
        a = math.radians(deg)
        body, wing = pal[(k + 3) % 6]
        P.bird(sc, math.cos(a) * rad, math.sin(a) * rad, z + r.uniform(-80, 80), heading=deg + 90.0, span=330.0, body=body, wing=wing, beak=AMBER,
               phase=r.random(), rate=r.uniform(1.6, 2.2), flap=30.0, orbit=(0.0, 0.0, rate))


def flamingos(sc):
    """A skein of nine flamingos in a V, long necks out ahead and legs trailing, flying 80 m along the road and round
    again."""
    r = random.Random(3113)
    L, V = 8000.0, 1000.0
    tv = tv_of(L, V)
    for k in range(9):
        row = (k + 1) // 2
        side = -1 if k % 2 else 1
        x = 800.0 - row * 360.0
        y = side * row * 150.0
        z = 520.0 + row * 34.0 + r.uniform(-25, 25)
        ph = r.random()
        P.bird(sc, x, y, z, heading=0.0, span=340.0, body=PINK, wing=HOTPINK, beak=DARK, phase=ph, rate=1.9, flap=26.0, travel=(L, V))
        m = sc.part(P.motion(pivot=(x, y, z), base=(x, y), travel=tv))
        tilt_box(m, PINK, (x + 60, y, z + 6), (x + 250, y, z + 40), 20, 20)       # the neck
        m.box(PINK, (x + 268, y, z + 42), (44, 30, 30))
        tilt_box(m, HOTPINK, (x - 70, y, z - 6), (x - 300, y, z - 26), 12, 12)    # the legs, trailing


JOBS = [
    ('SM_P3C_Walk_A', lit(walk_a)),
    ('SM_P3C_Walk_B', lit(walk_b)),
    ('SM_P3C_Walk_C', lit(walk_c)),
    ('SM_P3C_MiniRail', lit(mini_rail), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3C_HorseTram', lit(horse_tram)),
    ('SM_P3C_Skywalk_Lo', lit(skywalk_lo, 0.08), dict(far=True)),
    ('SM_P3C_Skywalk_Hi', lit(skywalk_hi, 0.09), dict(far=True)),
    ('SM_P3C_LogRide_Lo', lit(log_ride_lo, 0.08), dict(far=True)),
    ('SM_P3C_LogRide_Hi', lit(log_ride_hi, 0.09), dict(far=True)),
    ('SM_P3C_SkyBikes', lit(sky_bikes, 0.08), dict(far=True)),
    ('SM_P3C_Rink', lit(rink, 0.08), dict(far=True, grow=(120.0, 120.0, 80.0), grow_down=(120.0, 120.0, 0.0))),
    ('SM_P3C_Parrots', lit(parrots, 0.3), dict(far=True, grow=(1700.0, 1700.0, 200.0), grow_down=(1700.0, 1700.0, 0.0))),
    ('SM_P3C_Flamingos', lit(flamingos, 0.3), dict(far=True, grow=(4300.0, 200.0, 200.0), grow_down=(4300.0, 200.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
