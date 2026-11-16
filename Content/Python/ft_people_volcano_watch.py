"""FULL THROTTLE (Unreal) the population, Volcano (level index 3): THE WATCHERS (/Game/People/Volcano/SM_Watch...).

The people who watch the chase and the mountain: the survey's scientists on observation decks and survey towers,
villagers on terraces of basalt, a fire crew resting on its raised stand. An extra worker's kit beside the level's
main one (ft_people_volcano.py: the street-level cast, the shoot); the placing rules are the "watch" block of
Source/FullThrottle/FTPeopleL3.inl.

How the land lies for a watcher here (FTDressL3.inl): the barrier is 1.6 m tall, the land 0.75 m under the road, a
basalt kerb 21 m out, and two lava channels (38 to 52 m on the left, 46 to 61 m on the right) whose glow must stay in
sight. The main cast stands on trestles 28 m out. So the watchers stand
  - on OPEN scaffold decks 2.8 m up between the main cast and the channels (36 m on the left, 42 m on the right): thin
    legs, nothing solid under the deck, so the channel behind shows through (from the chase camera the channel is seen
    between 0.3 and 1.4 m above the land at those distances);
  - BEHIND the channels (61 m on the left, 69.5 m on the right), big: a survey tower whose deck (11.5 m) tops the
    basalt walls that stand behind the channels, and terraces of basalt in the walls' gaps; the people there are half
    as big again as life, so that they read from the road.

    SM_WatchDeck_A     TALL  the survey's observation deck: scientists in white coats and bright hard hats; some watch
                             the mountain through a telescope, the rest wave at the chase, point and film. A radar bar
                             turns, a striped windsock swings.
    SM_WatchDeck_B     TALL  the same deck taken over by villagers: three steps of them hopping and waving cloth, a
                             line of pennants overhead, their bundles at the rail.
    SM_WatchFire       TALL  a fire crew resting on its raised stand (no vehicle, nothing red: ENGINE 13 is a red fire
                             engine): four sit on the edge with their legs hanging, one waves a helmet, one drinks,
                             under a striped canopy beside a water tank.
    SM_WatchTower      TALL  a survey tower, its deck 11.5 m up: seven watchers nearly twice life size, big flags, a spinning
                             anemometer and a windsock. Stands behind a lava channel, over the basalt wall.
    SM_WatchTerrace_A  TALL  a terrace of basalt, four steps, twenty-four villagers hopping out of step under a long
    SM_WatchTerrace_B        striped banner. Behind a lava channel, in the gaps of the basalt walls.

Run in the editor:  import ft_people_volcano_watch as K; K.run()        K.run(['SM_WatchTower'])
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/Volcano'

PLANK, PLANK_D, STEEL, STEEL_D = '#e8d8b8', '#c9a070', '#8f9bb3', '#5a5870'
PALE, WHITE, DARK, YEL, LIME, PINK = '#b0a088', '#fff6ea', '#22202a', '#ffc93c', '#b8ff5a', '#ff5fa2'
ORANGE, CYAN, VIOLET, MINT = '#ff7a3c', '#2ee6ff', '#8f6bff', '#7dffb0'
BAS, BAS_L = '#3e3244', '#5a4e5e'
CLOTH = (PINK, CYAN, YEL, MINT, WHITE, VIOLET, '#57c7ff', '#f4a6ff', LIME)
VILLAGE_TOPS = CLOTH + ('#3a5fa8',)
DECK = 280.0          # the near decks: the lava channel behind is seen under them


# ------------------------------------------------------------------------------------------ the casts
def villager(r, **fixed):
    d = P.look(r, tops=VILLAGE_TOPS, wear=r.choice(('trousers', 'dress', 'skirt', 'coat', 'trousers', 'shorts')),
               hat=r.choice((None, None, 'sun', 'cap', 'beanie')), hat_col=r.choice(CLOTH), sleeves=r.choice(('short', 'long')))
    d.update(fixed)
    return d


def scientist(r, **fixed):
    d = P.look(r, tops=(WHITE,), bottoms=('#3a5fa8', '#2b2350', '#5a4a3c', '#c94f6a'), wear='coat', sleeves='long',
               hat='hard', hat_col=r.choice((PINK, CYAN, VIOLET, MINT, ORANGE)), band=CYAN)
    d.update(fixed)
    return d


def fire_look(r, **fixed):
    d = P.look(r, tops=(YEL,), bottoms=('#2b2350', '#3a3848'), wear='coat', sleeves='long', hat=r.choice(('hard', 'helmet')),
               hat_col=WHITE, band=LIME, shoes='#22202a', hair='short', hair_col=r.choice(('#2a2230', '#3b2a22', '#6a4428', '#cfcfd8')))
    d.update(fixed)
    return d


def big_flag(col, col2=WHITE, w=110.0, h=64.0, pole=STEEL):
    """A flag big enough to read from the road: two bands of colour on a long pole."""
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + 70), (5, 5, 170))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + 140 - h / 4.0 + h / 2.0 - 16), (w, 4, h / 2.0))
        m.box(col2, (hd[0] + side * w / 2.0, hd[1], hd[2] + 140 - h / 4.0 - 16), (w, 4, h / 2.0))
    return f


def hold_helmet(col=WHITE):
    def f(m, hd, side):
        m.cyl(col, (hd[0], hd[1], hd[2] + 6), 30, 5, steps=8)
        m.cyl(col, (hd[0], hd[1], hd[2] + 10), 25, 20, steps=8, top_r=16)
    return f


def hold_board(col=WHITE, clip=ORANGE):
    """A clipboard at the chest."""
    def f(m, hd, side):
        if side > 0:
            m.box(col, (hd[0] - 14, hd[1] - 8, hd[2] + 10), (34, 5, 44))
            m.box(clip, (hd[0] - 14, hd[1] - 11, hd[2] + 28), (18, 3, 7))
    return f


def cheers(r, lively=1.0):
    """The ways a watcher watches: big gestures, each out of step with the next."""
    k = lively
    return (
        dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=10 * k, rate=1.5),
        dict(arm_r='wave', swing_r=30, rate=1.2, bob=4 * k),
        dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=9 * k, rate=1.3),
        dict(arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.7, bob=2 * k),
        dict(arm_l='wave', arm_r='wave', swing_l=-24, swing_r=24, rate=1.0, bob=6 * k),
        dict(arm_l='up', arm_r='hip', swing_l=22, rate=1.6, bob=8 * k),
    )


# ------------------------------------------------------------------------------------------ what they stand on
def scaffold(g, L, W, h=DECK, stripe=(ORANGE, WHITE), rail=True, legs_every=300.0, leg=STEEL_D):
    """An OPEN deck on thin steel legs (rooted 2.5 m into the ground: the land falls away here and there), braced on
    the far side only and only above 1.5 m, a striped fascia along the road side, a two-bar rail behind it."""
    g.box(PLANK, (0, 0, h - 8), (L, W, 16))
    n = max(int(L / 90.0), 2)
    for k in range(n):
        g.box(stripe[k % 2], (-L / 2.0 + (k + 0.5) * L / n, -W / 2.0 - 5, h - 20), (L / n, 10, 44))
    g.box(PLANK_D, (0, W / 2.0 + 4, h - 14), (L, 8, 28))
    n = max(int(round(L / legs_every)), 1)
    for k in range(n + 1):
        x = -L / 2.0 + 12 + k * (L - 24) / n
        for sy in (-1, 1):
            g.box(leg, (x, sy * (W / 2.0 - 16), (h - 16 - 250) / 2.0), (13, 13, h - 16 + 250))
        if k < n:
            x1 = -L / 2.0 + 12 + (k + 1) * (L - 24) / n
            a, b = ((x, x1) if k % 2 == 0 else (x1, x))
            tilt_box(g, leg, (a, W / 2.0 - 16, 150), (b, W / 2.0 - 16, h - 30), 9, 9)
        if rail:
            g.box(WHITE, (x, -W / 2.0 + 8, h + 52), (8, 8, 104))
    if rail:
        g.box(WHITE, (0, -W / 2.0 + 8, h + 100), (L, 7, 9))
        g.box(WHITE, (0, -W / 2.0 + 8, h + 56), (L, 6, 7))


def windsock(sc, g, x, y, z0, top, length=220.0, cols=(ORANGE, WHITE), rate=0.4, swing=20.0, fat=44.0):
    """A mast from z0 to `top` and a striped sock that swings about it."""
    g.box(STEEL, (x, y, (z0 + top) / 2.0), (9, 9, top - z0))
    m = sc.part(P.motion(swing=swing, pivot=(x, y, top - 30), axis=P.UP, rate=rate, phase=0.3))
    n = 5
    for k in range(n):
        f = fat * (1.0 - 0.5 * k / (n - 1))
        m.box(cols[k % 2], (x + 12 + (k + 0.5) * length / n, y, top - 30 - k * 5), (length / n, f * 0.6, f))


def pennants(sc, x0, x1, y, z, cols, n=9, drop=70.0, w=None, swing=5.0, rate=0.6):
    """A line of cloth between two points at height z: it swings together about its own line."""
    m = sc.part(P.motion(swing=swing, pivot=((x0 + x1) / 2.0, y, z), axis=0.0, rate=rate))
    span = x1 - x0
    w = w or span / n
    m.box(STEEL_D, ((x0 + x1) / 2.0, y, z), (span, 4, 5))
    for i in range(n):
        m.box(cols[i % len(cols)], (x0 + (i + 0.5) * span / n, y, z - drop / 2.0), (w, 4, drop))


# ------------------------------------------------------------------------------------------ the near decks (36 / 42 m)
def deck_a(sc):
    """The survey's observation deck."""
    r = random.Random(301)
    g = sc.still()
    L, W = 960.0, 300.0
    scaffold(g, L, W, stripe=(ORANGE, WHITE))
    g.box(PLANK_D, (0, 85, DECK + 30), (L - 40, 120, 60))                    # the back row's riser
    acts = cheers(r)
    # the front row: at the rail, watching the chase
    xs = (-400, -255, -110, 35, 180, 325)
    for i, x in enumerate(xs):
        d = scientist(r)
        d.update(acts[(i * 5 + 1) % len(acts)])
        if i == 2:
            d.update(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), swing_l=0, swing_r=0, turn=16, rate=0.5, bob=0)
        if i == 4:
            d.update(arm_r='up', hold_r=big_flag(ORANGE, WHITE), swing_r=18, rate=1.0, bob=4, arm_l='hip', swing_l=0)
        P.figure(sc, x + r.uniform(-14, 14), -70 + r.uniform(-8, 8), DECK, yaw=r.uniform(-14, 14), phase=r.random(), react=(i % 2 == 0),
                 detail=1 if i in (2, 4) else 0, **d)
    # the back row, on the riser: two at the telescope with their backs to the road (the mountain is the other way)
    for i, x in enumerate((-330, -160, 60, 230)):
        d = scientist(r)
        d.update(acts[(i * 2) % len(acts)])
        P.figure(sc, x + r.uniform(-12, 12), 85 + r.uniform(-8, 8), DECK + 60, yaw=r.uniform(-14, 14), phase=r.random(), detail=0, **d)
    tx, ty, tz = 400.0, 60.0, DECK + 60.0
    g.box(PLANK_D, (tx, ty + 25, DECK + 30), (120, 170, 60))
    for a in (30, 150, 270):
        rod(g, STEEL, (tx, ty + 40, tz + 120), (tx + math.cos(math.radians(a)) * 45, ty + 40 + math.sin(math.radians(a)) * 45, tz), 4.5, steps=5)
    rod(g, WHITE, (tx, ty - 30, tz + 96), (tx, ty + 130, tz + 176), 15, steps=8)
    rod(g, ORANGE, (tx, ty + 112, tz + 167), (tx, ty + 136, tz + 179), 18, steps=8)
    P.figure(sc, tx - 6, ty - 62, DECK + 60, yaw=180, arm_l='face', arm_r='face', rock=3, rate=0.4, phase=0.2, detail=1, **scientist(r))
    P.figure(sc, tx - 95, ty - 10, DECK + 60, yaw=150, arm_r=((120, -60), (150, -70)), arm_l='hip', swing_r=10, rate=0.8, phase=0.6, detail=0,
             hold_l=hold_board(), **scientist(r))
    # the instruments: a radar bar turning on its post, the windsock
    px, py = -445.0, 95.0
    g.box(STEEL, (px, py, DECK + 60 + 110), (10, 10, 220))
    g.box(DARK, (px, py, DECK + 60 + 30), (60, 50, 60))
    m = sc.part(P.motion(base=(px, py), travel=(-1.0, 0.35)))
    m.box(WHITE, (px, py, DECK + 60 + 240), (170, 16, 34))
    m.box(ORANGE, (px + 60, py, DECK + 60 + 240), (40, 18, 36))
    windsock(sc, g, 150.0, 138.0, DECK, DECK + 520.0)


def deck_b(sc):
    """The same deck, taken over by the village: three steps of them and a line of pennants."""
    r = random.Random(302)
    g = sc.still()
    L, W = 960.0, 320.0
    scaffold(g, L, W, stripe=(PINK, YEL))
    rows = ((-90.0, 0.0), (15.0, 55.0), (115.0, 110.0))
    for k, (y, up) in enumerate(rows):
        if up:
            g.box(PLANK_D, (0, y + 25, DECK + up / 2.0), (L - 50, 150 - k * 20, up))
    acts = cheers(r)
    for k, (y, up) in enumerate(rows):
        n = 5 if k != 1 else 4
        for i in range(n):
            x = -L / 2.0 + 95 + (i + (0.5 if k == 1 else 0.0)) * (L - 190) / 4.0 + r.uniform(-18, 18)
            d = villager(r)
            if k == 0 and i in (1, 3):
                d.update(build='kid', s=r.uniform(0.62, 0.7))
            d.update(acts[(i * 5 + k * 2) % len(acts)])
            if (k, i) in ((1, 0), (2, 3)):
                d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag(r.choice(CLOTH[:4]), WHITE), swing_r=18, rate=1.0, bob=4)
            P.figure(sc, x, y + r.uniform(-8, 8), DECK + up, yaw=r.uniform(-14, 14), phase=r.random(), react=(i + k) % 2 == 0, detail=0, **d)
    for i in range(4):          # what they carried up here
        g.box(r.choice(CLOTH), (-400 + i * 265 + r.uniform(-20, 20), -128, DECK + 18), (46, 24, 36))
    for sx in (-1, 1):
        g.box(PLANK_D, (sx * (L / 2.0 - 14), 150, DECK + 240), (9, 9, 480))
    pennants(sc, -L / 2.0 + 14, L / 2.0 - 14, 150.0, DECK + 470.0, (PINK, WHITE, CYAN, YEL, VIOLET), n=10, drop=78.0)


def fire_stand(sc):
    """A fire crew resting on its raised stand. No vehicle and nothing red (the boss here is a red fire engine)."""
    r = random.Random(303)
    g = sc.still()
    L, W = 900.0, 300.0
    scaffold(g, L, W, stripe=(LIME, YEL), rail=False, leg=PLANK_D)
    # a rail along the back and the ends, none in front: they sit on the edge
    for k in range(4):
        g.box(WHITE, (-L / 2.0 + 10 + k * (L - 20) / 3.0, W / 2.0 - 8, DECK + 52), (8, 8, 104))
    g.box(WHITE, (0, W / 2.0 - 8, DECK + 100), (L, 7, 9))
    # the water tank at one end, the canopy over the middle, hose coils and cans
    g.cyl('#9fd8f0', (-345, 40, DECK), 78, 170, steps=10)
    g.cyl(WHITE, (-345, 40, DECK + 170), 82, 12, steps=10)
    g.box(STEEL, (-345, -44, DECK + 40), (16, 20, 16))
    for sx, sy in ((-190, -120), (-190, 130), (330, -120), (330, 130)):
        g.box(STEEL, (sx, sy, DECK + 150), (8, 8, 300))
    n = 7
    for k in range(n):
        g.box((LIME, WHITE)[k % 2], (-190 + (k + 0.5) * 520.0 / n, 5, DECK + 306), (520.0 / n, 290, 10))
    cv = sc.part(P.motion(swing=5.0, pivot=(70.0, -140.0, DECK + 300.0), axis=0.0, rate=0.7))      # the canopy's valance stirs
    for k in range(n):
        cv.box((WHITE, LIME)[k % 2], (-190 + (k + 0.5) * 520.0 / n, -142, DECK + 280), (520.0 / n, 4, 42))
    for i, x in enumerate((240, 300)):
        g.cyl(LIME, (x, 95, DECK + i * 0), 34, 26, steps=8)
        g.cyl(DARK, (x, 95, DECK + 26), 16, 3, steps=6)
    for i in range(3):
        g.box(YEL, (395, 60 + i * 0 - 40 * i, DECK + 24), (30, 34, 48))
    # four sit on the road edge, legs hanging, each out of step: one waves a helmet, one drinks, two wave
    seats = ((-150, dict(arm_r='up', hold_r=hold_helmet(), swing_r=24, rate=1.1, hat=None, arm_l='down')),
             (-30, dict(arm_r='face', hold_r=P.hold_cup(), swing_r=8, axis_r='x', rate=0.4)),
             (95, dict(arm_l='wave', arm_r='wave', swing_l=-22, swing_r=22, rate=1.0)),
             (215, dict(arm_r='point', swing_r=10, axis_r='x', rate=0.7, arm_l='down')))
    for i, (x, act) in enumerate(seats):
        d = fire_look(r)
        d.update(act)
        P.figure(sc, x, -W / 2.0 + 14, DECK - 46.0, legs='sit', yaw=r.uniform(-10, 10), phase=r.random(), react=(i % 2 == 0), detail=1, **d)
    # four stand behind them: cheering, one with a striped flag, one leaning on the tank with a cup
    acts = cheers(r)
    for i, x in enumerate((-250, -80, 60, 180, 310)):
        d = fire_look(r)
        d.update(acts[(i * 2 + 1) % len(acts)])
        if i == 2:
            d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag(LIME, YEL), swing_r=18, rate=0.9, bob=3)
        if i == 0:
            d.update(arm_r='face', hold_r=P.hold_cup(), swing_r=8, axis_r='x', rate=0.4, arm_l='hip', swing_l=0, bob=0)
        P.figure(sc, x + r.uniform(-12, 12), 30 + r.uniform(-12, 12), DECK, yaw=r.uniform(-16, 16), phase=r.random(), react=(i % 2 == 1), detail=0, **d)


# ------------------------------------------------------------------------------------------ behind the channels (61 / 69.5 m)
def tower(sc):
    """A survey tower: the deck 11.5 m up tops the basalt walls; the watchers are nearly twice life size."""
    r = random.Random(304)
    g = sc.still()
    H, DX, DY, S = 1150.0, 300.0, 210.0, 1.9
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, STEEL_D, (sx * (DX + 70), sy * (DY + 60), -250), (sx * (DX - 20), sy * (DY - 16), H - 10), 20, 20)
    for z, f in ((330.0, 0.74), (700.0, 0.42)):
        ex, ey = DX - 20 + 90 * f, DY - 16 + 76 * f
        for sy in (-1, 1):
            g.box(STEEL_D, (0, sy * ey, z), (ex * 2, 12, 12))
        for sx in (-1, 1):
            g.box(STEEL_D, (sx * ex, 0, z), (12, ey * 2, 12))
    for sx in (-1, 1):          # the cross braces, on the two ends
        tilt_box(g, STEEL, (sx * (DX + 30), -(DY + 30), 330), (sx * (DX - 6), DY - 4, 1100), 10, 10)
        tilt_box(g, STEEL, (sx * (DX + 30), DY + 30, 330), (sx * (DX - 6), -(DY - 4), 1100), 10, 10)
    tilt_box(g, STEEL, (-(DX + 40), DY + 34, 20), (DX + 10, DY + 10, 700), 10, 10)       # and the back
    tilt_box(g, STEEL, (DX + 40, DY + 34, 20), (-(DX + 10), DY + 10, 700), 10, 10)
    g.box(PLANK, (0, 0, H - 9), (DX * 2 + 60, DY * 2 + 60, 18))
    n = 8
    for k in range(n):          # a fascia of hot colour all round the deck's edge on the road side
        g.box((ORANGE, WHITE)[k % 2], (-(DX + 30) + (k + 0.5) * (DX * 2 + 60) / n, -(DY + 34), H - 34), ((DX * 2 + 60) / n, 10, 70))
    for k in range(5):
        x = -(DX + 20) + k * (DX * 2 + 40) / 4.0
        g.box(WHITE, (x, -(DY + 20), H + 70), (10, 10, 140))
        g.box(WHITE, (x, DY + 20, H + 70), (10, 10, 140))
    for y in (-(DY + 20), DY + 20):
        g.box(WHITE, (0, y, H + 136), (DX * 2 + 50, 9, 11))
        g.box(WHITE, (0, y, H + 72), (DX * 2 + 50, 8, 9))
    for sx in (-1, 1):
        g.box(WHITE, (sx * (DX + 20), 0, H + 136), (9, DY * 2 + 40, 11))
    g.box(PLANK_D, (0, 110, H + 45), (DX * 2 - 40, 150, 90))                 # the back row's riser
    for k in range(13):         # the ladder, up the back
        g.box(STEEL, (-(DX - 60), DY + 60 - k * 3.2, 40 + k * 88), (56, 8, 8))
    for sx in (-28, 28):
        tilt_box(g, STEEL, (-(DX - 60) + sx, DY + 64, 0), (-(DX - 60) + sx, DY + 22, H), 8, 8)
    acts = cheers(r)
    front = (-225, -75, 75, 225)
    for i, x in enumerate(front):
        d = scientist(r) if i != 1 else villager(r)
        d.update(acts[(i * 5) % len(acts)])
        if i == 0:
            d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag(ORANGE, WHITE), swing_r=20, rate=0.9, bob=4)
        if i == 3:
            d.update(arm_l='up', arm_r='hip', swing_r=0, hold_l=big_flag(CYAN, WHITE), swing_l=20, rate=1.0, bob=4)
        d['s'] = S * r.uniform(0.95, 1.04)
        P.figure(sc, x, -95 + r.uniform(-10, 10), H, yaw=r.uniform(-12, 12), phase=r.random(), detail=0, **d)
    for i, x in enumerate((-150, 10, 165)):
        d = scientist(r) if i != 2 else villager(r)
        d.update(acts[(i * 2 + 1) % len(acts)])
        d['s'] = S * r.uniform(0.95, 1.04)
        P.figure(sc, x, 110 + r.uniform(-10, 10), H + 90, yaw=r.uniform(-12, 12), phase=r.random(), detail=0, **d)
    # the mast: a big windsock and an anemometer turning on top
    mx, my = DX - 10.0, DY - 10.0
    windsock(sc, g, mx, my, H, H + 760.0, length=380.0, fat=80.0, swing=24.0, rate=0.35)
    m = sc.part(P.motion(base=(mx, my), travel=(-1.0, 0.6)))
    for a in (0, 120, 240):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(m, STEEL, (mx, my, H + 770), (mx + ca * 90, my + sa * 90, H + 770), 7, 7)
        m.sphere(WHITE if a else ORANGE, (mx + ca * 100, my + sa * 100, H + 770), 24, steps=(3, 6))


def terrace(seed, banner, per_step=6):
    def build(sc):
        """A terrace of basalt: four steps of villagers half as big again as life, a long banner over them."""
        r = random.Random(seed)
        g = sc.still()
        L, S, D = 1700.0, 1.4, 170.0
        tops = (250.0, 400.0, 550.0, 700.0)
        for k, h in enumerate(tops):
            y = -1.5 * D + k * D
            g.box(BAS if k % 2 == 0 else BAS_L, (0, y, (h - 250) / 2.0), (L, D, h + 250))
            g.box(PALE, (0, y, h - 5), (L - 30, D - 20, 12))                          # ash on the step: the people stand out on it
        for sx in (-1, 1):       # columns of basalt at the ends, and the two that carry the banner
            for k in range(5):
                h = tops[min(k, 3)] + r.uniform(60, 240)
                g.cyl(BAS_L if k % 2 else BAS, (sx * (L / 2.0 + 55 + r.uniform(0, 50)), -1.8 * D + k * D * 0.95, -250), r.uniform(60, 85), h + 250, steps=6)
        acts = cheers(r, lively=1.3)
        for k, h in enumerate(tops):
            y = -1.5 * D + k * D
            n = per_step - (k % 2)
            for i in range(n):
                x = -L / 2.0 + 110 + (i + (0.5 if k % 2 else 0.0)) * (L - 220) / (per_step - 1) + r.uniform(-30, 30)
                d = villager(r)
                if k == 0 and i % 3 == 1:
                    d.update(build='kid')
                    d['s'] = r.uniform(0.62, 0.7)
                d.update(acts[(i * 5 + k * 2 + seed) % len(acts)])
                if (i + 2 * k + seed) % 7 == 0:
                    d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag(r.choice(CLOTH[:4]), WHITE), swing_r=20, rate=1.0, bob=5)
                d['s'] = d['s'] * S
                P.figure(sc, x, y + r.uniform(-14, 14), h, yaw=r.uniform(-14, 14), phase=r.random(), detail=0, **d)
        top = tops[-1]
        for sx in (-1, 1):
            g.box(PLANK_D, (sx * (L / 2.0 - 30), 1.5 * D + 60, top / 2.0 + 300), (14, 14, top + 620))
        pennants(sc, -L / 2.0 + 30, L / 2.0 - 30, 1.5 * D + 60, top + 600.0, banner, n=12, drop=130.0, swing=4.0, rate=0.5)
    return build


JOBS = [
    ('SM_WatchDeck_A', deck_a),
    ('SM_WatchDeck_B', deck_b),
    ('SM_WatchFire', fire_stand),
    ('SM_WatchTower', tower),
    ('SM_WatchTerrace_A', terrace(311, (PINK, WHITE, CYAN, WHITE))),
    ('SM_WatchTerrace_B', terrace(312, (YEL, VIOLET, YEL, WHITE), per_step=5)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
