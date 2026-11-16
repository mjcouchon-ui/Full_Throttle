"""FULL THROTTLE (Unreal) the population of level 3, Nevada (level index 2): THE WATCHERS. /Game/People/Nevada/SM_Watch*.

The people who came out to watch the chase. Highway 93 is level and straight and the land lies a metre under it behind
a metre of barrier, so every watcher here is RAISED and MASSED, and stands 36 m out or more (the main cast and the film
crew have the band nearer the road): a crowd of 14 to 24 in hot colours on something tall, hopping out of step, with
big things in its hands (hats, striped cards, flags, foam fingers), livelier as the car comes by (react).

    SM_WatchBleachers_A   rodeo bleachers on a timber frame: four tiers of ranch families, hats in the air, a striped
                          backdrop behind them, two big flags swinging at the ends
    SM_WatchBleachers_B   the same in the strip's colours: the front row holds striped cards up in a wave, the top row
                          a long banner that bobs
    SM_WatchTrucks        the roadhouse crowd on wheels: a flatbed with hay bales and a lifted pickup, people standing on
                          the beds, the bales and the cab roofs, a flag as big as the truck
    SM_WatchPorch         a roadhouse front brought with its crowd: bikers on the balcony over the porch and along the
                          roof's edge, a flag on the ridge
    SM_WatchBalcony       the strip: a casino's terraces, gowns and dinner jackets on two balconies, valets on the
                          carpeted steps, chips thrown in the air
    SM_WatchBluff         FAR (far=True): a tailgate on a red rock bluff, giants so that they read at 200 m: a crowd
                          against the sky, a 20 m banner, two campers

Everything moves through M_People (ft_people.py). No words, no marks, no light on any piece. The placing rules are the
block "watch" in Source/FullThrottle/FTPeopleL2.inl, written by write_table() below:
    in the editor:   import ft_people_nevada_watch as K; K.run()
    plain Python:    python Content/Python/ft_people_nevada_watch.py          (prints the table block)
"""
import math
import random

try:
    import unreal
    import ft_people as P
    from ft_desert import rod, tilt_box
except ImportError:      # plain Python: only the table writer below is used
    unreal = None
    P = None

DIR = '/Game/People/Nevada'

SAND, SANDST, SANDST_D, SANDST_L, ROCK_R, TALUS = '#f2c58a', '#e0784a', '#b8553a', '#f59a62', '#c9603e', '#a04a34'
BARK, BARK_D, WOOD, WOOD_L = '#8a6a4a', '#6a4c36', '#9a6a44', '#c9955f'
LEATHER, NAVY, PURP, MAG, TEAL, RED, CYAN = '#22202a', '#2b2350', '#7b3bd6', '#ff4fb8', '#2fc6c0', '#ff2d3c', '#2ee6ff'
CREAM, WHITE, GOLD, YEL, STEEL, CHROME = '#f4efe6', '#fff6ea', '#ffc93c', '#ffe14d', '#8f9bb3', '#d7e3f0'
DENIM, RUST, MINT, PINK, LEMON, GLASS = '#3a5fa8', '#b8553a', '#7fe3c4', '#ff9ec4', '#ffe9a8', '#3a4a6a'
ROCK_TONES = (TALUS, SANDST_D, ROCK_R, SANDST, SANDST_L, SANDST, ROCK_R, SANDST_L)
HOT = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a', '#ff2d3c')
S = 1.25            # a watcher's size as built (the table makes the piece bigger still)


# ------------------------------------------------------------------------------------------ what a watcher holds
def hold_hat(col=CREAM):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 10), (84, 84, 6))
        m.box(col, (h[0], h[1], h[2] + 24), (40, 40, 24))
    return f


def hold_card(col, stripe):
    """A big card held overhead (drawn from the right hand, centred over the head)."""
    def f(m, h, side):
        m.box(col, (h[0] - 30, h[1], h[2] + 36), (92, 5, 64))
        m.box(stripe, (h[0] - 30, h[1] - 3.5, h[2] + 36), (92, 2, 20))
    return f


def hold_finger(col):
    """A foam finger: a fat block with one finger up."""
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 22), (34, 12, 40))
        m.box(col, (h[0], h[1], h[2] + 62), (14, 12, 44))
    return f


def hold_flag(col, stripe, w=120.0, hgt=64.0):
    def f(m, h, side):
        m.box(CREAM, (h[0], h[1], h[2] + 70), (5, 5, 170))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + 122), (w, 4, hgt))
        m.box(stripe, (h[0] + side * w / 2.0, h[1] - 3, h[2] + 122), (w, 2, hgt * 0.34))
    return f


def acts_for(r, hats=(CREAM, WHITE, GOLD, RED)):
    """The ways a watcher cheers: all of them big enough to read at 40 m and more."""
    c1, c2 = r.sample(HOT, 2)
    return [
        dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=12, rate=1.5),
        dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=14, rate=1.25),
        dict(arm_r='wave', hold_r=hold_hat(r.choice(hats)), swing_r=30, rate=1.2, bob=6),
        dict(arm_r='up', hold_r=hold_flag(c1, c2), swing_r=24, rate=0.9, bob=5),
        dict(arm_l='hip', arm_r='up', hold_r=hold_finger(r.choice(HOT)), swing_r=20, rate=1.4, bob=10),
        dict(arm_l='wave', arm_r='wave', swing_l=24, swing_r=-24, rate=1.1, bob=8),
    ]


def watcher(r, **fixed):
    d = P.look(r, tops=HOT)
    d['s'] = S * r.uniform(0.93, 1.07)
    q = r.random()
    if q < 0.14:
        d.update(build='kid', s=S * 0.72)
    if r.random() < 0.3:
        d.update(wear=r.choice(('dress', 'shorts', 'skirt')))
    d.update(fixed)
    return d


def row(sc, r, n, x0, x1, y, z, k=0, detail=0, wave=None, hat=None, hat_p=0.35, react=True, acts=None, fixed=None, yaw=0.0):
    """n watchers along X from x0 to x1 at (y, z), each out of step with the next. wave: (card colours) the row holds
    cards up and a wave runs along it. hat: the row's kind of hat (some of them wear it)."""
    for i in range(n):
        x = x0 + (i + 0.5) * (x1 - x0) / n + r.uniform(-14, 14)
        d = watcher(r, **(fixed or {}))
        if hat and r.random() < hat_p:
            d.update(hat=hat, hat_col=r.choice((CREAM, WHITE, GOLD, RED, BARK)))
        if wave:
            a = dict(arm_l='up', arm_r='up', bob=18, rate=0.7, hold_r=hold_card(wave[i % len(wave)], wave[(i + 2) % len(wave)]))
            ph = i / float(n) * 0.9
        else:
            al = acts or acts_for(r)
            a = dict(al[(i * 5 + k * 3) % len(al)])
            ph = r.random()
        d.update(a)
        P.figure(sc, x, y + r.uniform(-8, 8), z=z, yaw=yaw + r.uniform(-16, 16), phase=ph, react=react, detail=detail, **d)


def big_flag(sc, x, y, z0, height, col, stripe, w=260.0, h=150.0, phase=0.0, pole=CREAM):
    """A flag on a pole: the cloth swings round the pole."""
    g = sc.still()
    g.box(pole, (x, y, z0 + height / 2.0), (9, 9, height))
    f = sc.part(P.motion(phase=phase, rate=0.55, swing=24.0, pivot=(x, y, z0 + height), axis=P.UP))
    f.box(col, (x + w / 2.0, y, z0 + height - h / 2.0), (abs(w), 5, h))
    f.box(stripe, (x + w / 2.0, y - 3.5, z0 + height - h / 2.0), (abs(w), 2, h * 0.3))
    f.box(stripe, (x + w / 2.0, y + 3.5, z0 + height - h / 2.0), (abs(w), 2, h * 0.3))


def bunting(g, x0, x1, y, z, n=12, drop=64.0):
    g.box(CREAM, ((x0 + x1) / 2.0, y, z), (x1 - x0, 5, 5))
    for k in range(n):
        px = x0 + (k + 0.5) * (x1 - x0) / n
        w = (x1 - x0) / n * 0.42
        g.side(HOT[(k * 3) % len(HOT)], [(px - w, z), (px + w, z), (px, z - drop)], y - 3, y + 3)


def wheel(g, x, y, z, rad, w, col=LEATHER, hub=CHROME):
    pts = [(x + math.cos(a * math.pi / 4.0) * rad, z + math.sin(a * math.pi / 4.0) * rad) for a in range(8)]
    g.side(col, pts, y - w / 2.0, y + w / 2.0)
    pts = [(x + math.cos(a * math.pi / 3.0) * rad * 0.45, z + math.sin(a * math.pi / 3.0) * rad * 0.45) for a in range(6)]
    g.side(hub, pts, y - w / 2.0 - 2, y + w / 2.0 + 2)


# ------------------------------------------------------------------------------------------ the bleachers
def bleachers(seed, kind):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L, rows, step, rise, H0, per = 1300.0, 4, 88.0, 64.0, 250.0, 6
        cA, cB, cC = ((RED, CREAM, DENIM), (PURP, LEMON, TEAL))[kind]
        y0 = -rows * step / 2.0
        top = H0 + (rows - 1) * rise
        yb = -y0 + 6
        # the tiers: a plank floor and a riser board each; the front riser is the skirt, striped
        for k in range(rows):
            h = H0 + k * rise
            g.box(WOOD_L, (0, y0 + (k + 0.5) * step, h - 6), (L, step, 12))
            if k:
                g.box(cA if k % 2 else cC, (0, y0 + k * step, h - rise / 2.0 - 6), (L, 6, rise))
        g.box(cB, (0, y0 - 2, H0 - 62), (L + 10, 6, 112))
        for k in range(9):
            if k % 2 == 0:
                g.box(cA, (-L / 2.0 + (k + 0.5) * L / 9.0, y0 - 5.5, H0 - 62), (L / 9.0, 2, 112))
        g.box(cC, (0, y0 - 6, H0 - 10), (L + 14, 4, 20))
        # the frame: posts from under the ground (the land may fall away), braces across the front, the sides closed
        n = 6
        for k in range(n):
            x = -L / 2.0 + 12 + k * (L - 24) / (n - 1.0)
            g.box(BARK_D, (x, y0 + 12, (H0 - 110) / 2.0 - 60), (16, 16, H0 - 110 + 120))
            g.box(BARK_D, (x, yb - 14, top / 2.0 - 60), (16, 16, top + 120))
            if k < n - 1:
                x2 = -L / 2.0 + 12 + (k + 1) * (L - 24) / (n - 1.0)
                a, b = ((x, y0 + 12, 10), (x2, y0 + 12, H0 - 124)) if k % 2 == 0 else ((x, y0 + 12, H0 - 124), (x2, y0 + 12, 10))
                tilt_box(g, BARK, a, b, 9, 9)
        for sx in (-1, 1):
            for k in range(rows):
                h = H0 + k * rise
                g.box(BARK, (sx * (L / 2.0 + 4), y0 + (k + 0.5) * step, (h + H0 - 118) / 2.0), (8, step, h - H0 + 118))
        # the backdrop: a board behind the top row, a band of chevrons over the heads
        g.box(cB, (0, yb, top + 170), (L + 30, 10, 340))
        g.box(cC, (0, yb - 6, top + 318), (L + 30, 3, 44))
        for k in range(10):
            px = -L / 2.0 + (k + 0.5) * (L / 10.0)
            g.box(cA if k % 2 == 0 else cC, (px, yb - 7, top + 252), (L / 10.0 * 0.62, 6, 76))
        for sx in (-1, 0, 1):
            g.box(BARK_D, (sx * (L / 2.0 - 4), yb + 9, (top + 340) / 2.0 - 60), (14, 10, top + 340 + 120))
        bunting(g, -L / 2.0, L / 2.0, yb - 8, top + 372, n=14)
        g.box(CREAM, (0, y0 + 4, H0 + 54), (L, 6, 6))            # the front rail (one thin bar: the front row shows)
        big_flag(sc, -L / 2.0 - 30, yb, 0.0, top + 640, cA, cB, w=-250.0, phase=0.2)
        big_flag(sc, L / 2.0 + 30, yb, 0.0, top + 640, cC if kind else cA, cB, w=250.0, phase=0.6)
        # the crowd
        hat = 'cowboy' if kind == 0 else 'sun'
        for k in range(rows):
            off = (0.3 if k % 2 else -0.3) * L / per * 0.5
            wave = None
            if kind == 1 and k == 0:
                wave = (MAG, CYAN, GOLD, MINT, WHITE)
            row(sc, r, per, -L / 2.0 + 30 + off, L / 2.0 - 30 + off, y0 + (k + 0.5) * step + 4, H0 + k * rise, k=k + seed,
                detail=(1 if k == 0 else 0), wave=wave, hat=hat, hat_p=(0.34 if kind == 0 else 0.2), react=not wave)
        if kind == 1:
            # the long banner over the top row: it bobs, a stripe along it
            b = sc.part(P.motion(phase=0.1, rate=0.8, bob=-16.0))
            zb = top + S * 172 + 96
            b.box(WHITE, (0, y0 + (rows - 0.5) * step - 24, zb), (L * 0.8, 5, 78))
            b.box(MAG, (0, y0 + (rows - 0.5) * step - 27.5, zb), (L * 0.8, 2, 26))
            for sx in (-1, 1):
                b.box(CREAM, (sx * L * 0.4, y0 + (rows - 0.5) * step - 24, zb - 80), (6, 6, 240))
    return build


# ------------------------------------------------------------------------------------------ the roadhouse crowd on wheels
def trucks(sc):
    r = random.Random(207)
    g = sc.still()
    # ---- the flatbed: cab at the -X end, the deck 160 up, hay bales along its back half as a second tier
    D = 270.0
    for wx in (-690, -420, 30, 110):
        for sy in (-1, 1):
            wheel(g, wx, sy * (D / 2.0 - 22), 56 - 4, 56, 40)
    g.box(LEATHER, (-300, 0, 104), (940, D - 70, 28))
    g.box(TEAL, (-650, 0, 215), (220, D, 200))                    # the cab
    g.box(TEAL, (-650, 0, 330), (190, D - 14, 40))
    g.box(GLASS, (-650, -D / 2.0 - 1, 262), (150, 3, 70))
    g.box(GLASS, (-762, 0, 262), (3, D - 50, 70))
    g.box(CHROME, (-768, 0, 150), (10, D - 20, 50))
    g.box(CHROME, (-560, D / 2.0 - 20, 330), (14, 14, 220))       # the stack
    g.box(WOOD_L, (-190, 0, 151), (700, D, 18))                   # the deck
    g.box(RED, (-190, -D / 2.0 - 2, 134), (700, 5, 22))
    g.box(WHITE, (-190, -D / 2.0 - 5, 134), (700, 2, 7))
    for k in range(5):
        bx = -470 + k * 140
        g.box(YEL if k % 2 else LEMON, (bx, 62, 160 + 32), (132, 120, 64))
        g.box(BARK, (bx, 1, 160 + 32), (136, 3, 8))
    # ---- the lifted pickup: cab towards -X, the bed 130 up
    PX = 520.0
    for wx in (PX - 170, PX + 175):
        for sy in (-1, 1):
            wheel(g, wx, sy * 102, 64 - 4, 64, 46)
    g.box(LEATHER, (PX, 0, 100), (520, 150, 24))
    g.box(MAG, (PX, 0, 150), (560, 232, 76))                       # the body
    g.box(MAG, (PX - 110, 0, 222), (190, 220, 80))                 # the cab
    g.box(GLASS, (PX - 110, -111.5, 226), (150, 3, 52))
    g.box(GLASS, (PX - 206, 0, 226), (3, 190, 52))
    g.box(CREAM, (PX, -117.5, 150), (560, 2, 16))
    g.box(CHROME, (PX - 284, 0, 128), (10, 236, 30))
    g.box(CHROME, (PX + 284, 0, 128), (10, 236, 30))
    g.box(LEATHER, (PX + 130, 0, 190), (260, 200, 6))              # the bed's floor (its walls stand round it)
    g.box(CHROME, (PX - 10, 0, 300), (10, 210, 10))                # the roll bar
    for sy in (-1, 1):
        g.box(CHROME, (PX - 10, sy * 104, 245), (10, 10, 116))
    # ---- the crowd
    acts = acts_for(r)
    row(sc, r, 5, -520, 150, -74, 160, k=1, detail=1, acts=acts, hat='cap', hat_p=0.4)
    row(sc, r, 5, -530, 140, 62, 224, k=2, detail=0, acts=acts, hat='cowboy', hat_p=0.3)
    row(sc, r, 3, PX + 20, PX + 250, 0, 193, k=3, detail=1, acts=acts, hat='cap', hat_p=0.3)
    # on the cab roofs: the flag as big as the truck, and one with both arms up
    d = watcher(r, hair='mohawk', hair_col=MAG, top=LEATHER, band=GOLD, build='stocky', s=S * 1.05)
    P.figure(sc, -650, 0, z=350, arm_l='hip', arm_r='up', swing_r=20, rate=0.7, bob=4, phase=0.3, react=True, detail=1,
             hold_r=hold_flag(WHITE, RED, w=250.0, hgt=140.0), **d)
    d = watcher(r, hat='cowboy', hat_col=WHITE, s=S)
    P.figure(sc, PX - 110, 0, z=262, arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=14, rate=1.5, phase=0.7, react=True, detail=1, **d)
    big_flag(sc, 190, 120, 0.0, 720, GOLD, LEATHER, w=280.0, h=160.0, phase=0.4, pole=CHROME)


# ------------------------------------------------------------------------------------------ the roadhouse front
def porch(sc):
    r = random.Random(411)
    g = sc.still()
    L, Dp, H1, H2 = 1200.0, 420.0, 330.0, 660.0       # the balcony's floor and the roof's edge
    yf = -40.0                                         # the house's front wall
    g.box(RUST, (0, yf + Dp / 2.0, H2 / 2.0 - 60), (L, Dp, H2 + 120))
    for k in range(7):                                 # boards
        g.box(SANDST_D, (-L / 2.0 + (k + 0.5) * L / 7.0, yf - 1.5, H2 / 2.0), (6, 3, H2))
    # the porch: a roof on posts, its top the balcony
    g.box(WOOD_L, (0, yf - 130, H1 - 8), (L + 40, 260, 16))
    g.box(CREAM, (0, yf - 262, H1 - 34), (L + 44, 6, 52))
    for k in range(6):
        if k % 2 == 0:
            g.box(RED, (-L / 2.0 + (k + 0.5) * L / 6.0, yf - 265.5, H1 - 34), (L / 6.0, 2, 52))
    for k in range(5):
        g.box(CREAM, (-L / 2.0 + 10 + k * (L - 20) / 4.0, yf - 250, (H1 - 16) / 2.0 - 60), (16, 16, H1 - 16 + 120))
    g.box(CREAM, (0, yf - 254, H1 + 58), (L + 30, 6, 6))            # the balcony's rail: one thin bar and its posts
    for k in range(5):
        g.box(CREAM, (-L / 2.0 + 10 + k * (L - 20) / 4.0, yf - 254, H1 + 30), (8, 8, 60))
    # doors and windows
    for wx in (-420, 420):
        g.box(GLASS, (wx, yf - 2, 190), (190, 4, 130))
        g.box(GLASS, (wx, yf - 2, H1 + 150), (150, 4, 150))
    g.box(BARK_D, (0, yf - 2, 130), (170, 4, 250))
    g.box(BARK_D, (0, yf - 2, H1 + 120), (130, 4, 220))
    # the false front over the roof's edge: low at the sides (the roof crowd shows over it), a tall board in the middle
    g.box(SANDST_D, (0, yf - 4, H2 + 14), (L + 20, 12, 28))
    g.box(CREAM, (0, yf + 150, H2 + 330), (430, 12, 250))
    g.box(RED, (0, yf + 143, H2 + 330), (380, 3, 200))
    g.box(GOLD, (0, yf + 139, H2 + 330), (120, 5, 120), rot=(45, 0, 0))     # a diamond
    for sx in (-1, 1):
        g.box(BARK_D, (sx * 190, yf + 160, H2 + 100), (14, 14, 210))
    big_flag(sc, -L / 2.0 + 60, yf + 200, H2, 520, GOLD, LEATHER, w=-240.0, h=140.0, phase=0.15)
    big_flag(sc, L / 2.0 - 60, yf + 200, H2, 520, RED, WHITE, w=240.0, h=140.0, phase=0.55)
    bunting(g, -L / 2.0, L / 2.0, yf - 256, H1 + 190, n=12, drop=56.0)
    for sx in (-1, 1):
        g.box(CREAM, (sx * L / 2.0, yf - 256, H1 + 95), (7, 7, 190))
    # the crowd: the balcony (bikers: leather and hot bandanas), and the roof's edge
    acts = acts_for(r, hats=(LEATHER, RED, WHITE))
    for i in range(7):
        x = -L / 2.0 + 70 + (i + 0.5) * (L - 140) / 7.0 + r.uniform(-14, 14)
        d = watcher(r)
        if i % 2 == 0:
            d.update(top=LEATHER, band=r.choice(HOT), sleeves='none', build=r.choice(('stocky', 'avg')), s=S * r.uniform(1.0, 1.08),
                     hair=r.choice(('mohawk', 'bald', 'long', 'pony')), hair_col=r.choice((MAG, CYAN, GOLD, '#c94f2e')))
        if i % 3 == 1:
            d.update(hat='helmet', hat_col=r.choice(HOT))
        d.update(acts[(i * 5 + 1) % len(acts)])
        P.figure(sc, x, yf - 150 + r.uniform(-20, 20), z=H1, yaw=r.uniform(-16, 16), phase=r.random(), react=True, detail=1, **d)
    row(sc, r, 4, -L / 2.0 + 40, -230, yf + 60, H2, k=2, detail=0, acts=acts, hat='cowboy', hat_p=0.5)
    row(sc, r, 4, 230, L / 2.0 - 40, yf + 60, H2, k=4, detail=0, acts=acts, hat='cap', hat_p=0.5)


# ------------------------------------------------------------------------------------------ the strip's terraces
def balcony(sc):
    r = random.Random(523)
    g = sc.still()
    L1, D1, H1 = 1300.0, 560.0, 340.0
    L2, D2, H2 = 1040.0, 300.0, 720.0
    y1 = -D1 / 2.0
    g.box(CREAM, (0, 0, H1 / 2.0 - 60), (L1, D1, H1 + 120))
    g.box(GOLD, (0, y1 - 3, H1 - 14), (L1 + 10, 6, 28))
    g.box(PURP, (0, y1 - 2, H1 - 110), (L1 + 6, 4, 70))
    for k in range(9):                                             # arches along the front: dark doorways
        ax = -L1 / 2.0 + (k + 0.5) * L1 / 9.0
        if k != 4:
            g.box(NAVY, (ax, y1 - 2, 110), (86, 4, 190))
    y2 = D1 / 2.0 - D2
    g.box(PINK, (0, y2 + D2 / 2.0, (H1 + H2) / 2.0), (L2, D2, H2 - H1))
    g.box(GOLD, (0, y2 - 3, H2 - 14), (L2 + 10, 6, 28))
    for k in range(7):
        g.box(NAVY, (-L2 / 2.0 + (k + 0.5) * L2 / 7.0, y2 - 2, H1 + 150), (80, 4, 210))
    # the crown: a fan of gold and magenta rays behind the top balcony (shapes only)
    yc = D1 / 2.0 - 30
    g.box(PURP, (0, yc, H2 + 150), (620, 40, 300))
    for k in range(7):
        a = math.radians(20 + k * 140 / 6.0)
        tilt_box(g, GOLD if k % 2 == 0 else MAG, (0, yc - 24, H2 + 40), (math.cos(a) * 330, yc - 24, H2 + 40 + math.sin(a) * 330), 8, 40)
    g.box(GOLD, (0, yc - 26, H2 + 60), (120, 10, 120))
    # balustrades: a gold bar on short posts (one thin bar: the people show)
    for (Lb, yb, zb) in ((L1, y1 + 8, H1), (L2, y2 + 8, H2)):
        g.box(GOLD, (0, yb, zb + 56), (Lb, 6, 6))
        for k in range(7):
            g.box(CREAM, (-Lb / 2.0 + 6 + k * (Lb - 12) / 6.0, yb, zb + 28), (10, 10, 56))
    # the carpet: steps up the middle of the front, a red runner
    for k in range(5):
        hs = H1 * (4 - k) / 5.0
        g.box(CREAM, (0, y1 - 22 - k * 44, hs / 2.0 - 30), (300, 44, hs + 60))
        g.box(RED, (0, y1 - 22 - k * 44, hs + 2), (170, 44, 5))
    for sx in (-1, 1):
        big_flag(sc, sx * (L1 / 2.0 - 30), y2 + 40, H1, 760, (MAG, CYAN)[sx > 0], WHITE, w=sx * 250.0, h=150.0, phase=0.3 + 0.2 * sx, pole=GOLD)
    # the crowd: gowns and dinner jackets
    acts = acts_for(r, hats=(WHITE, LEATHER))

    def guest(i):
        d = watcher(r)
        if d.get('build') == 'kid':
            d.update(build='avg', s=S)
        if i % 2 == 0:
            d.update(wear='dress', top=r.choice((MAG, RED, CYAN, GOLD, MINT, '#8f6bff')), hair=r.choice(('long', 'bun', 'afro')))
        else:
            d.update(wear='trousers', top=r.choice((WHITE, CREAM, PINK, LEMON)), bottom=LEATHER, sleeves='long', band=r.choice((RED, LEATHER, PURP)))
        return d

    n1 = 8
    for i in range(n1):
        x = -L1 / 2.0 + 50 + (i + 0.5) * (L1 - 100) / n1 + r.uniform(-14, 14)
        if abs(x) < 120:
            x += 150 if x >= 0 else -150
        d = guest(i)
        d.update(acts[(i * 5 + 2) % len(acts)])
        P.figure(sc, x, y1 + 70 + r.uniform(-10, 10), z=H1, yaw=r.uniform(-16, 16), phase=r.random(), react=True, detail=1, **d)
    n2 = 6
    for i in range(n2):
        x = -L2 / 2.0 + 50 + (i + 0.5) * (L2 - 100) / n2 + r.uniform(-14, 14)
        d = guest(i + 1)
        d.update(acts[(i * 5 + 4) % len(acts)])
        P.figure(sc, x, y2 + 70 + r.uniform(-10, 10), z=H2, yaw=r.uniform(-16, 16), phase=r.random(), react=True, detail=0, **d)
    # two valets at the head of the carpet, caps in the air
    for sx in (-1, 1):
        d = watcher(r, top=RED, bottom=LEATHER, sleeves='long', band=GOLD, wear='trousers', hat=None, build='avg', s=S)
        P.figure(sc, sx * 60, y1 + 20, z=H1, arm_l=('wave' if sx < 0 else 'hip'), arm_r=('wave' if sx > 0 else 'hip'),
                 swing_l=(28 if sx < 0 else 0), swing_r=(28 if sx > 0 else 0), hold_l=(hold_hat(RED) if sx < 0 else None),
                 hold_r=(hold_hat(RED) if sx > 0 else None), bob=6, rate=1.2, phase=0.25 + 0.25 * sx, react=True, detail=1, **d)
    # chips thrown in the air over both balconies: they hop high, out of step
    for i in range(12):
        zc = H1 + 250 if i % 2 == 0 else H2 + 250
        Lc = L1 if i % 2 == 0 else L2
        c = sc.part(P.motion(phase=r.random(), rate=r.uniform(0.7, 1.1), bob=r.uniform(90, 150)))
        cx = r.uniform(-Lc / 2.0 + 60, Lc / 2.0 - 60)
        c.box(r.choice((GOLD, MAG, CYAN, WHITE, RED)), (cx, (y1 if i % 2 == 0 else y2) + 40, zc), (26, 8, 26))


# ------------------------------------------------------------------------------------------ far: the tailgate on the bluff
def bluff(sc):
    """FAR. A red rock bluff 34 m long and 10 m tall, and on it giants: a crowd 4.5 m tall (it stands 150 to 280 m out),
    two campers, a banner 20 m long that bobs, flags."""
    r = random.Random(631)
    g = sc.still()
    L, D, H = 3400.0, 1300.0, 1000.0
    beds = 6
    for k in range(beds):
        q = 1.0 - 0.07 * k
        z0 = -200.0 if k == 0 else H * k / beds
        z1 = H * (k + 1) / beds
        pts = []
        for a in range(10):
            an = 2 * math.pi * a / 10.0
            w = 1.0 + r.uniform(-0.1, 0.1)
            pts.append((math.cos(an) * L / 2.0 * q * w, math.sin(an) * D / 2.0 * q * w))
        g.plan(ROCK_TONES[(k + 1) % len(ROCK_TONES)], pts, z0, z1)
    Lt = L * (1.0 - 0.07 * (beds - 1)) * 0.8
    # two campers at the back
    for cx, col in ((-700, CREAM), (650, MINT)):
        g.box(col, (cx, 220, H + 190), (760, 300, 300))
        g.box((TEAL, MAG)[cx > 0], (cx, 68, H + 200), (760, 4, 60))
        g.box(GLASS, (cx - 180, 67, H + 270), (220, 4, 90))
        g.box(LEATHER, (cx, 220, H + 30), (600, 280, 50))
        g.box(col, (cx, 220, H + 362), (420, 200, 44))
    # the banner between two poles: it bobs
    b = sc.part(P.motion(phase=0.2, rate=0.6, bob=-40.0))
    zb = H + 900
    b.box(WHITE, (0, 40, zb), (2000, 12, 240))
    b.box(RED, (0, 32, zb), (2000, 4, 80))
    for k in range(5):
        b.box(GOLD, (-800 + k * 400, 30, zb), (120, 4, 240))
    for sx in (-1, 1):
        g.box(CREAM, (sx * 1010, 40, H + 520), (22, 22, 1040))
    big_flag(sc, -Lt / 2.0 + 60, 120, H, 1500, MAG, WHITE, w=-620.0, h=380.0, phase=0.1)
    big_flag(sc, Lt / 2.0 - 60, 120, H, 1500, CYAN, WHITE, w=620.0, h=380.0, phase=0.6)
    # the crowd on the rim: giants in the hottest colours, hopping out of step
    n = 13
    for i in range(n):
        x = -Lt / 2.0 + 120 + (i + 0.5) * (Lt - 240) / n + r.uniform(-30, 30)
        d = P.look(r, tops=HOT)
        d['s'] = 2.6 * r.uniform(0.92, 1.08)
        if i % 4 == 1:
            d.update(hat='cowboy', hat_col=r.choice((CREAM, WHITE, RED)))
        a = (dict(arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=34, rate=1.2),
             dict(arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=40, rate=1.0),
             dict(arm_l='wave', arm_r='wave', swing_l=28, swing_r=-28, bob=20, rate=0.9))[i % 3]
        d.update(a)
        P.figure(sc, x, -D / 2.0 * 0.62 + r.uniform(-40, 40), z=H, yaw=r.uniform(-14, 14), phase=r.random(), detail=0, **d)


JOBS = [
    ('SM_WatchBleachers_A', bleachers(301, 0), dict(grow=(120.0, 60.0, 120.0))),
    ('SM_WatchBleachers_B', bleachers(302, 1), dict(grow=(120.0, 60.0, 120.0))),
    ('SM_WatchTrucks', trucks, dict(grow=(120.0, 60.0, 120.0))),
    ('SM_WatchPorch', porch, dict(grow=(120.0, 60.0, 120.0))),
    ('SM_WatchBalcony', balcony, dict(grow=(120.0, 60.0, 200.0))),
    ('SM_WatchBluff', bluff, dict(far=True, grow=(300.0, 120.0, 200.0), grow_down=(300.0, 120.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]


# ------------------------------------------------------------------------------------------ the placing table
# Plain Python. Written SLOT BY SLOT like the main cast's table (a slot is 45.243 m of road; slot s lies s * 45.243 m
# along it): every row is one Site rule in a zone of its own, so a piece stands exactly on its slot, on the side and at
# the distance given here. The watchers keep to 44 to 56 m (the main cast and the film crew stand 24.6 to 34 m out and
# on some slots 42 to 44 m: those slots are left to them), clear of what the dressing has in that band (FTDressL2.inl:
# truck stops 40 m out, road stops and motel courts 60 to 66, casinos 74 to 80, chapels from 52, sign frames and sign
# towers 30 to 48, hoodoos 40 to 58, rock cuts and rock banks). The rock districts (the dry wash, Red Rock Pass, Red
# rock return) have no room at the road: there the watchers are the far piece only, where the land is open.
SLOT = 0.045243
NV = '/Game/People/Nevada/'
SETS = [('PplWatchBleachA', 'SM_WatchBleachers_A', 0.0), ('PplWatchBleachB', 'SM_WatchBleachers_B', 0.0), ('PplWatchTrucks', 'SM_WatchTrucks', 0.0),
        ('PplWatchPorch', 'SM_WatchPorch', 0.0), ('PplWatchBalcony', 'SM_WatchBalcony', 0.0), ('PplWatchBluff', 'SM_WatchBluff', 0.0),
        ('PplWatchTrucksQ', 'SM_WatchTrucks', 18.0)]       # Q: 18 m along its slot (beside the roadhouse deck, not behind it)
DIST = [('state', 0.0, 1.17), ('wash', 1.17, 2.43), ('snake', 2.43, 3.83), ('joshua', 3.83, 5.14), ('redrock', 5.14, 6.31), ('gas', 6.31, 7.75),
        ('jackpot', 7.75, 9.05), ('city', 9.05, 10.69), ('salt', 10.69, 14.5), ('range', 14.5, 18.5), ('return', 18.5, 22.0), ('vegas', 22.0, 25.6)]
CUT_ZONES = [(1.154, 1.335), (1.606, 1.742), (1.787, 2.059)]
LAST = int(25.55 / SLOT)
NEAR_SCALE, FAR_SCALE = 1.3, 1.5


def district(s):
    km = s * SLOT
    for n, a, b in DIST:
        if a <= km < b:
            return n
    return 'vegas'


def site_side(s, every):
    """The side a dressing Site rule with side 0 takes on slot s."""
    return 1 if (s // every) % 2 else -1


def rows():
    out = []
    idx = dict((n, i) for i, (n, _, _) in enumerate(SETS))
    cut = set()
    for a, b in CUT_ZONES:
        cut.update(range(int(math.ceil(a / SLOT - 1e-6)), int(math.floor(b / SLOT + 1e-6)) + 1))

    def put(s, side, name, x, scale=NEAR_SCALE):
        if 1 <= s <= LAST:
            out.append((s, side, idx[name], x, scale))

    r = random.Random(2093)
    for s in range(4, LAST + 1):
        d = district(s)
        a = -1 if (s // 2) % 2 == 0 else 1          # the side the main cast's own piece takes on most slots
        stop = (s + 3) % 8 == 0                      # a truck stop 40 m out (one side or the other)
        if d == 'state':
            if s == 4:
                put(s, -1, 'PplWatchBleachA', 48.0)
            elif s == 5:
                put(s, 1, 'PplWatchTrucks', 46.0)
            elif s % 4 == 0:
                put(s, -1 if (s // 4) % 2 else 1, ('PplWatchBleachA', 'PplWatchTrucks')[(s // 4) % 2], 46.0)
            if s % 8 == 6:
                put(s, -a, 'PplWatchBluff', r.uniform(190, 240), FAR_SCALE)
        elif d == 'wash':
            if s not in cut and s % 3 == 1:
                put(s, a, 'PplWatchBluff', r.uniform(200, 250), FAR_SCALE)
        elif d == 'snake':
            if s % 4 == 0:
                put(s, site_side(s, 4), 'PplWatchTrucksQ', 48.0)           # on the roadhouse's own lot
            elif s % 4 == 2:
                put(s, -site_side(s, 4), 'PplWatchPorch', 52.0)            # across the road from the scrapyard
            elif s % 8 == 1:
                put(s, -1, 'PplWatchBleachA', 46.0)
            elif s % 8 == 3:
                put(s, 1, 'PplWatchBleachA', 46.0)
        elif d == 'joshua':
            if s % 6 == 0:
                put(s, -a, 'PplWatchBleachA', 46.0)
            elif s % 6 == 3:
                put(s, -a, 'PplWatchTrucks', 46.0)
            if s % 6 == 2:
                put(s, a, 'PplWatchBluff', r.uniform(200, 240), FAR_SCALE)
        elif d == 'gas':
            if s % 8 == 1:
                put(s, a, 'PplWatchTrucks', 48.0)
                put(s, -a, 'PplWatchPorch', 52.0)
            elif s % 4 == 0:
                put(s, -site_side(s, 4), 'PplWatchBleachA', 46.0)          # across the road from the road stop
        elif d == 'jackpot':
            if s % 3 == 1 and s % 4 in (0, 1) and not stop:
                put(s, a, 'PplWatchBalcony', 52.0)
                put(s, -a, 'PplWatchBleachB', 50.0)
            elif s % 6 == 0 and s % 4 != 3:
                put(s, -site_side(s, 3), 'PplWatchBleachB', 46.0)          # across the road from the casino
        elif d == 'city':
            if s % 5 == 1:
                put(s, a, 'PplWatchBalcony', 56.0)
                put(s, -a, 'PplWatchBleachB', 54.0)
            elif s % 5 == 4:
                if s % 10 == 4:                                            # a motel court on the left, 60 m out
                    put(s, 1, 'PplWatchBalcony', 56.0)
                else:
                    put(s, -a, 'PplWatchBalcony', 56.0)
                    put(s, a, 'PplWatchBleachB', 54.0)
        elif d == 'salt':
            if s % 7 == 4:
                pass                                                       # a wreck, 30 to 70 m out
            elif s % 5 == 0:
                put(s, -a, ('PplWatchBleachA', 'PplWatchTrucks')[(s // 5) % 2], 46.0)
            elif s % 10 == 2:
                put(s, -a, ('PplWatchTrucks', 'PplWatchBleachA')[(s // 10) % 2], 46.0)
            elif s % 20 == 7:
                put(s, a, 'PplWatchPorch', 52.0)
            if s % 8 == 6:
                put(s, 1 if s * SLOT < 11.7 else -a, 'PplWatchBluff', r.uniform(200, 250), FAR_SCALE)
        elif d == 'range':
            if s % 6 == 5:
                put(s, a, ('PplWatchBleachA', 'PplWatchTrucks')[(s // 6) % 2], 50.0)
            if s % 8 == 3:
                put(s, -a, 'PplWatchBluff', r.uniform(215, 250), FAR_SCALE)
        elif d == 'vegas':
            if s % 2 == 1 and s % 5 not in (0, 2) and not stop:
                if s % 4 == 3:
                    put(s, a, 'PplWatchBalcony', 52.0)
                    put(s, -a, 'PplWatchBleachB', 50.0)
                else:                                                      # the main cast's second grandstand: side a, 44 m
                    put(s, -a, ('PplWatchBalcony', 'PplWatchBleachB')[(s // 4) % 2], 52.0)
    out.sort()
    return out


def table_text():
    rr = rows()
    L = ['// ---- BEGIN watch (ft_people_nevada_watch.py) ----',
         '// THE WATCHERS: crowds that came out to see the chase, raised and massed, 44 to 56 m out (behind the main cast and the',
         "// film crew, clear of the dressing's own pieces), and a far crowd on a bluff 190 to 250 m out. WRITTEN BY",
         '// Content/Python/ft_people_nevada_watch.py (python ft_people_nevada_watch.py prints this block): change that file.',
         '// Slot by slot, like the main cast: one Site rule a row, in a zone of its own. %d rows.' % len(rr)]
    for n, m, along in SETS:
        L.append('Set(TEXT("%s"), { { TEXT("%s%s"), %s, 0, 0, 0, 1.f } });' % (n, NV, m, ('%.1ff' % along) if along else '0'))
    L.append('{')
    L.append('\tstatic const TCHAR* const PplWatchSets[] = { ' + ', '.join('TEXT("%s")' % n for n, _, _ in SETS) + ' };')
    L.append('\tstruct FPplWatchRow { int16 Slot; int8 Side; uint8 Set; float X, Scale; };')
    L.append('\tstatic const FPplWatchRow PplWatchRows[] = {')
    line = '\t'
    for (s, side, k, x, sc) in rr:
        item = '{%d,%d,%d,%.1ff,%.2ff}, ' % (s, side, k, x, sc)
        if len(line) + len(item) > 150:
            L.append(line.rstrip())
            line = '\t'
        line += item
    L.append(line.rstrip())
    L.append('\t};')
    L.append('\tfor (const FPplWatchRow& R : PplWatchRows)')
    L.append('\t{')
    L.append('\t\tZone(TEXT("People watch"), (R.Slot - 0.4f) * 0.045243f, (R.Slot + 0.4f) * 0.045243f);')
    L.append('\t\tSite(PplWatchSets[R.Set], R.Side, R.X, 1, 0, 1.f, R.Scale);')
    L.append('\t}')
    L.append('}')
    L.append('// ---- END watch ----')
    return '\n'.join(L) + '\n'


def write_table(path=None):
    """Replaces the block "watch" in FTPeopleL2.inl (or appends it after what is there)."""
    import os
    path = path or os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'Source', 'FullThrottle', 'FTPeopleL2.inl')
    s = open(path, 'rb').read().decode('utf-8')
    crlf = '\r\n' in s
    s = s.replace('\r\n', '\n')
    a, b = s.find('// ---- BEGIN watch'), s.find('// ---- END watch ----')
    if a >= 0 and b > a:
        s = s[:a] + s[b + len('// ---- END watch ----'):].lstrip('\n')
    if not s.endswith('\n'):
        s += '\n'
    s += table_text()
    open(path, 'wb').write((s.replace('\n', '\r\n') if crlf else s).encode('utf-8'))
    rr = rows()
    cnt = {}
    for (_, _, k, _, _) in rr:
        cnt[SETS[k][1]] = cnt.get(SETS[k][1], 0) + 1
    print('rows', len(rr), cnt)


if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1 and sys.argv[1] == 'write':
        write_table()
    else:
        sys.stdout.write(table_text())
