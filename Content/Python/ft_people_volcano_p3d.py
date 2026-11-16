"""FULL THROTTLE (Unreal) the population, Volcano (level index 3), pass 3, builder p3d: HEIGHTS
(/Game/People/Volcano/SM_P3D_...).

People UP HIGH, each piece bringing what it stands on. How the land lies here (FTDressL3.inl, FTPeopleL3.inl): the
barrier is 1.6 m tall and the land 0.75 m under the road; the main cast stands on trestles 28 m out (1.8 m up) and the
watchers on decks 36 / 42 m out (2.8 m up); two lava channels (38 to 52 m left, 46 to 61 m right) whose glow must stay
in sight; behind them basalt walls 6 to 11 m tall on many slots, the watchers' towers (11.5 m) and terraces. So:

  NEAR (33 m on the left, 37 m on the right: the strip between the trestles and the watch decks). Slim and OPEN: thin
  legs, nothing solid under the deck (the channel behind shows through), no more than 2.4 m deep. The people stand
  5 m up and more, over the heads of everyone already there, a fifth over life size.
    SM_P3D_Lookout_A  TALL  a slim scaffold lookout: villagers in a row at the rail, hopping and waving, one big flag,
                            a line of pennants overhead, a ladder up the back.
    SM_P3D_Lookout_B  TALL  the same lookout held by the survey: white coats and bright hard hats, a telescope turned
                            on the mountain, a radar bar turning, a windsock.
    SM_P3D_Crow       TALL  a crow's nest on a single braced pole, 6.4 m up: a lookout swinging a signal flag and
                            one pointing at the chase.

  BEHIND THE CHANNELS (69 m on the left, 70 m on the right), everyone 12.5 m up and more so that they top the basalt
  walls, and 1.8 times life size so that they read from the road.
    SM_P3D_Derrick_A  TALL  a survey derrick: scientists on its top deck (13.5 m) and its gallery, an anemometer
                            spinning, a windsock, a flag at the crown.
    SM_P3D_Derrick_B  TALL  the same derrick climbed by villagers: big flags, lines of pennants from the crown.
    SM_P3D_Crag_A     TALL  a stack of basalt columns with villagers on its heads, a rope ladder down the front.
    SM_P3D_Crag_B     TALL  the same crag with goats on its heads and ledges and their herder.
    SM_P3D_RopeBridge TALL  a footbridge 13 m up between two basalt pillars: villagers walk across it both ways
                            carrying their bundles (they travel), two wave from the pillars.
    SM_P3D_Headframe  TALL  a mine headframe: miners on its platform, the sheave wheel rocking, the cage with two
                            miners riding up and down between the legs. (No vehicle, nothing red: ENGINE 13.)
    SM_P3D_Arch       TALL  a basalt arch: villagers on its lintel, a banner under it, a climber on a rope going
                            up and down in the opening.
    SM_P3D_Signal     TALL  a signal mast: a keeper on its platform (14 m) swinging two flags, two striped signal
                            arms swinging at the head.

No glow, nothing that blinks, no words or marks, nothing red and no vehicle (the boss here is a red fire engine).
The placing rules are C:/ftwork/p3/L3d.inl (the integrator pastes them into FTPeopleL3.inl).

Run in the editor:  import ft_people_volcano_p3d as K; K.run()        K.run(['SM_P3D_Crow'])
"""
import math
import random
import ft_people as P
import ft_people_volcano as V
from ft_desert import rod, tilt_box

DIR = '/Game/People/Volcano'

PLANK, PLANK_D, STEEL, STEEL_D = '#e8d8b8', '#c9a070', '#8f9bb3', '#5a5870'
PALE, WHITE, DARK, YEL, LIME, PINK = '#b0a088', '#fff6ea', '#22202a', '#ffc93c', '#b8ff5a', '#ff5fa2'
ORANGE, CYAN, VIOLET, MINT, SKY = '#ff7a3c', '#2ee6ff', '#8f6bff', '#7dffb0', '#57c7ff'
BAS, BAS_L, ROPE = '#3e3244', '#5a4e5e', '#c9a070'
CLOTH = (PINK, CYAN, YEL, MINT, WHITE, VIOLET, SKY, '#f4a6ff', LIME)
VILLAGE_TOPS = CLOTH + ('#3a5fa8',)
NEAR = 500.0          # the near lookouts' deck
BIG = 1.8             # the people behind the channels


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


def miner(r, **fixed):
    d = P.look(r, tops=(ORANGE, CYAN, MINT, VIOLET), bottoms=('#2b2350', '#3a5fa8', '#5a4a3c'), wear='trousers', sleeves='long',
               hat='hard', hat_col=WHITE, band=LIME, shoes='#22202a')
    d.update(fixed)
    return d


def person(sc, x, y, z, d, scale=1.0, **kw):
    """One figure from a look/act dict, its size multiplied."""
    d = dict(d)
    d['s'] = d.get('s', 1.0) * scale
    d.update(kw)
    return P.figure(sc, x, y, z, **d)


def big_flag(col, col2=WHITE, w=110.0, h=64.0, pole=STEEL):
    """A flag big enough to read from the road: two bands of colour on a long pole."""
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + 70), (5, 5, 170))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + 140), (w, 4, h / 2.0))
        m.box(col2, (hd[0] + side * w / 2.0, hd[1], hd[2] + 140 - h / 2.0), (w, 4, h / 2.0))
    return f


def hold_bundle(col):
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1] - 4, hd[2] - 12), (34, 26, 30))
    return f


def cheers(k=1.0):
    """The ways a watcher watches: big gestures, each out of step with the next."""
    return (
        dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=10 * k, rate=1.5),
        dict(arm_r='wave', swing_r=30, rate=1.2, bob=4 * k),
        dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=9 * k, rate=1.3),
        dict(arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.7, bob=2 * k),
        dict(arm_l='wave', arm_r='wave', swing_l=-24, swing_r=24, rate=1.0, bob=6 * k),
        dict(arm_l='up', arm_r='hip', swing_l=22, rate=1.6, bob=8 * k),
    )


# ------------------------------------------------------------------------------------------ what they stand on
def pennants(sc, a, b, cols, n=8, drop=70.0, swing=5.0, rate=0.6):
    """A line of cloth from point a to point b: it swings together about its own middle."""
    mid = ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0)
    ax = math.degrees(math.atan2(b[1] - a[1], b[0] - a[0]))
    m = sc.part(P.motion(swing=swing, pivot=mid, axis=ax, rate=rate, phase=0.1 * n))
    tilt_box(m, STEEL_D, a, b, 5, 5)
    for i in range(n):
        f = (i + 0.5) / n
        p = (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f)
        m.box(cols[i % len(cols)], (p[0], p[1], p[2] - drop / 2.0), (max(abs(b[0] - a[0]), abs(b[1] - a[1])) / n * 0.8, 4, drop), rot=(0, ax, 0))


def windsock(sc, g, x, y, z0, top, length=220.0, cols=(ORANGE, WHITE), rate=0.4, swing=20.0, fat=44.0):
    g.box(STEEL, (x, y, (z0 + top) / 2.0), (9, 9, top - z0))
    m = sc.part(P.motion(swing=swing, pivot=(x, y, top - 30), axis=P.UP, rate=rate, phase=0.3))
    n = 5
    for k in range(n):
        f = fat * (1.0 - 0.5 * k / (n - 1))
        m.box(cols[k % 2], (x + 12 + (k + 0.5) * length / n, y, top - 30 - k * 5), (length / n, f * 0.6, f))


def ladder(g, x, y, z0, z1, w=46.0, col=PLANK_D, rungs=7):
    for sx in (-1, 1):
        g.box(col, (x + sx * w / 2.0, y, (z0 + z1) / 2.0), (6, 6, z1 - z0))
    for k in range(rungs):
        g.box(col, (x, y, z0 + (k + 0.5) * (z1 - z0) / rungs), (w, 5, 5))


def fascia(g, x0, x1, y, z, cols, n=6, h=44.0):
    for k in range(n):
        g.box(cols[k % 2], (x0 + (k + 0.5) * (x1 - x0) / n, y, z), ((x1 - x0) / n, 10, h))


def rail(g, x0, x1, y, z, posts=4, col=WHITE):
    for k in range(posts):
        g.box(col, (x0 + k * (x1 - x0) / (posts - 1), y, z + 52), (8, 8, 104))
    g.box(col, ((x0 + x1) / 2.0, y, z + 100), (x1 - x0, 7, 9))
    g.box(col, ((x0 + x1) / 2.0, y, z + 56), (x1 - x0, 6, 7))


def slim_scaffold(g, L, W, h, stripe):
    """The near lookouts' deck: OPEN, on four thin legs rooted 2.5 m into the ground, braced on the far side only and
    only above 1.6 m (the lava channel behind is seen under it)."""
    g.box(PLANK, (0, 0, h - 8), (L, W, 16))
    fascia(g, -L / 2.0, L / 2.0, -W / 2.0 - 5, h - 20, stripe, n=6)
    xs = (-L / 2.0 + 12, 0.0, L / 2.0 - 12)
    for x in xs:
        for sy in (-1, 1):
            g.box(STEEL_D, (x, sy * (W / 2.0 - 12), (h - 16 - 250) / 2.0), (12, 12, h - 16 + 250))
    tilt_box(g, STEEL_D, (xs[0], W / 2.0 - 12, 170), (xs[1], W / 2.0 - 12, h - 30), 8, 8)
    tilt_box(g, STEEL_D, (xs[2], W / 2.0 - 12, 170), (xs[1], W / 2.0 - 12, h - 30), 8, 8)
    for sx in (-1, 1):
        g.box(STEEL_D, (sx * (L / 2.0 - 12), 0, h - 150), (8, W - 24, 8))
    rail(g, -L / 2.0 + 6, L / 2.0 - 6, -W / 2.0 + 8, h, posts=4)


def lattice(g, h, bx, by, tx, ty, col=STEEL_D, brace=STEEL, rings=(0.3, 0.62), leg=20.0):
    """A four-legged tower from the ground (rooted 2.5 m) to height h, tapering from (bx, by) to (tx, ty) half-widths:
    ring beams, cross braces on the back and the two ends (the road side stays open)."""
    def at(f):
        return bx + (tx - bx) * f, by + (ty - by) * f
    for sx in (-1, 1):
        for sy in (-1, 1):
            f0 = -250.0 / h
            x0, y0 = at(f0)
            tilt_box(g, col, (sx * x0, sy * y0, -250), (sx * tx, sy * ty, h), leg, leg)
    levels = [0.04] + list(rings) + [0.97]
    for f in rings:
        ex, ey = at(f)
        for sy in (-1, 1):
            g.box(col, (0, sy * ey, h * f), (ex * 2, 11, 11))
        for sx in (-1, 1):
            g.box(col, (sx * ex, 0, h * f), (11, ey * 2, 11))
    for k in range(len(levels) - 1):
        f0, f1 = levels[k], levels[k + 1]
        (x0, y0), (x1, y1) = at(f0), at(f1)
        s = 1 if k % 2 == 0 else -1
        tilt_box(g, brace, (-s * x0, y0, h * f0), (s * x1, y1, h * f1), 9, 9)              # the back
        for sx in (-1, 1):
            tilt_box(g, brace, (sx * x0, -s * y0, h * f0), (sx * x1, s * y1, h * f1), 9, 9)    # the ends


def hexcol(g, x, y, r, h, col=BAS, cap=PALE):
    """A basalt column, rooted 2.5 m, ash on its head."""
    g.cyl(col, (x, y, -250), r, h + 250 - 10, steps=6)
    g.cyl(cap, (x, y, h - 10), r * 0.92, 10, steps=6)


# ------------------------------------------------------------------------------------------ NEAR: the lookouts
def lookout(seed, science):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L, W, S = 660.0, 220.0, 1.2
        slim_scaffold(g, L, W, NEAR, (ORANGE, WHITE) if science else (PINK, YEL))
        ladder(g, -L / 2.0 + 60, W / 2.0 + 6, -60, NEAR + 20)
        acts = cheers()
        xs = (-262, -158, -52, 52, 158, 262)
        for i, x in enumerate(xs):
            d = scientist(r) if science else villager(r)
            if not science and i == 1:
                d.update(build='kid', s=r.uniform(0.62, 0.7))
            d.update(acts[(i * 5 + seed) % len(acts)])
            if i == 3:
                d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag(ORANGE if science else PINK, WHITE), swing_r=20, rate=1.0, bob=4)
            if science and i == 0:
                d.update(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), swing_l=0, swing_r=0, turn=16, rate=0.5, bob=0)
            person(sc, x + r.uniform(-8, 8), -30 + r.uniform(-8, 8), NEAR, d, S, yaw=r.uniform(-14, 14), phase=r.random(), react=(i % 2 == 0), detail=1)
        if science:
            # a telescope on its tripod at the far end, turned on the mountain; a radar bar turning; the windsock
            tx, ty, tz = 290.0, 60.0, NEAR
            for a in (30, 150, 270):
                rod(g, STEEL, (tx, ty, tz + 120), (tx + math.cos(math.radians(a)) * 40, ty + math.sin(math.radians(a)) * 40, tz), 4.5, steps=5)
            rod(g, WHITE, (tx, ty - 60, tz + 100), (tx, ty + 60, tz + 170), 13, steps=8)
            px, py = -300.0, 80.0
            g.box(STEEL, (px, py, NEAR + 130), (10, 10, 260))
            m = sc.part(P.motion(base=(px, py), travel=(-1.0, 0.35)))
            m.box(WHITE, (px, py, NEAR + 270), (150, 14, 30))
            m.box(ORANGE, (px + 52, py, NEAR + 270), (36, 16, 32))
            windsock(sc, g, 120.0, 96.0, NEAR, NEAR + 430.0, length=180.0)
        else:
            for sx in (-1, 1):
                g.box(PLANK_D, (sx * (L / 2.0 - 10), W / 2.0 - 10, NEAR + 200), (9, 9, 400))
            pennants(sc, (-L / 2.0 + 10, W / 2.0 - 10, NEAR + 395), (L / 2.0 - 10, W / 2.0 - 10, NEAR + 395), (PINK, WHITE, CYAN, YEL, VIOLET), n=8, drop=74.0)
            for i in range(3):
                g.box(r.choice(CLOTH), (-200 + i * 190 + r.uniform(-20, 20), 78, NEAR + 18), (44, 24, 36))
    return build


def crow(sc):
    """A crow's nest on a single braced pole."""
    r = random.Random(331)
    g = sc.still()
    H, S = 640.0, 1.2
    g.box(PLANK_D, (0, 0, (H - 250) / 2.0), (22, 22, H + 250))
    for a in (90, 210, 330):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(g, STEEL_D, (ca * 110, sa * 110, -200), (ca * 8, sa * 8, 330), 9, 9)
    for k in range(7):
        g.box(PLANK, (0, 14, 60 + k * 82), (46, 5, 5))
    g.cyl(PLANK, (0, 0, H - 16), 112, 16, steps=8)
    g.cyl(ORANGE, (0, 0, H), 116, 30, steps=8)
    g.cyl(WHITE, (0, 0, H + 30), 116, 30, steps=8)
    g.box(PLANK_D, (0, 70, H + 190), (12, 12, 380))
    fl = sc.part(P.motion(swing=14.0, pivot=(0.0, 70.0, H + 350.0), axis=P.UP, rate=0.5, phase=0.4))
    fl.box(PINK, (62, 70, H + 350), (110, 4, 34))
    fl.box(WHITE, (62, 70, H + 316), (110, 4, 34))
    d = villager(r, hat='cap', hat_col=ORANGE)
    d.update(arm_r='up', arm_l='hip', hold_r=big_flag(YEL, PINK), swing_r=26, rate=1.0, bob=3)
    person(sc, -46, -24, H, d, S, yaw=-10, phase=0.1, react=True, detail=1)
    d = villager(r)
    d.update(arm_r='point', arm_l='face', swing_r=14, axis_r='x', rate=0.8, bob=2)
    person(sc, 48, -20, H, d, S, yaw=12, phase=0.6, detail=1)


# ------------------------------------------------------------------------------------------ BEHIND THE CHANNELS
def derrick(seed, science):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, TX, TY = 1350.0, 230.0, 170.0
        lattice(g, H, 300.0, 230.0, TX, TY)
        # the top deck, its fascia and rail; the gallery half way up, hung on the road side
        g.box(PLANK, (0, 0, H - 9), (TX * 2 + 90, TY * 2 + 80, 18))
        cols = (ORANGE, WHITE) if science else (PINK, YEL)
        fascia(g, -(TX + 45), TX + 45, -(TY + 45), H - 36, cols, n=8, h=72.0)
        rail(g, -(TX + 38), TX + 38, -(TY + 30), H, posts=5)
        GZ = 760.0
        g.box(PLANK, (0, -235, GZ - 8), (470, 130, 16))
        fascia(g, -235, 235, -305, GZ - 26, cols[::-1], n=6, h=52.0)
        for sx in (-1, 1):
            tilt_box(g, STEEL, (sx * 225, -290, GZ - 16), (sx * 262, -205, GZ - 190), 9, 9)
        ladder(g, 0, TY + 62, GZ, H, w=60.0, rungs=6)
        # the crown: the derrick goes on up to its head, a flag there
        CR = H + 620.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                tilt_box(g, STEEL_D, (sx * (TX - 10), sy * (TY - 10) + 40, H), (sx * 26, sy * 26 + 60, CR), 14, 14)
        g.box(STEEL_D, (0, 60, H + 330), (2 * 118, 11, 11))
        g.box(DARK, (0, 60, CR + 12), (90, 90, 24))
        g.box(STEEL, (0, 60, CR + 150), (9, 9, 260))
        fl = sc.part(P.motion(swing=16.0, pivot=(0.0, 60.0, CR + 240.0), axis=P.UP, rate=0.45, phase=0.2))
        fl.box(cols[0], (95, 60, CR + 252), (180, 5, 48))
        fl.box(WHITE, (95, 60, CR + 204), (180, 5, 48))
        acts = cheers()
        # four on the top deck at the rail
        for i, x in enumerate((-198, -66, 66, 198)):
            d = scientist(r) if science else villager(r)
            d.update(acts[(i * 5 + seed) % len(acts)])
            if i == 1 or (not science and i == 3):
                d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag(cols[0], WHITE, w=120.0), swing_r=20, rate=1.0, bob=4)
            if science and i == 2:
                d.update(arm_l='face', arm_r='face', swing_l=0, swing_r=0, rock=3, rate=0.4, bob=0)
            person(sc, x + r.uniform(-8, 8), -(TY - 40) + r.uniform(-8, 8), H, d, BIG, yaw=r.uniform(-14, 14), phase=r.random(), react=(i % 2 == 0),
                   detail=1 if i in (1, 2) else 0)
        # three on the gallery
        for i, x in enumerate((-150, 0, 150)):
            d = scientist(r) if science else villager(r)
            d.update(acts[(i * 2 + 1 + seed) % len(acts)])
            person(sc, x + r.uniform(-8, 8), -232, GZ, d, 1.5, yaw=r.uniform(-14, 14), phase=r.random(), react=(i == 1), detail=0)
        if science:
            # an anemometer spinning on one corner, the windsock on the other
            ax_, ay_ = -(TX + 20), TY + 20
            g.box(STEEL, (ax_, ay_, H + 150), (9, 9, 300))
            m = sc.part(P.motion(base=(ax_, ay_), travel=(-1.0, 0.8)))
            for a in (0, 120, 240):
                ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
                tilt_box(m, STEEL, (ax_, ay_, H + 300), (ax_ + ca * 90, ay_ + sa * 90, H + 300), 6, 6)
                m.sphere(ORANGE if a == 0 else WHITE, (ax_ + ca * 100, ay_ + sa * 100, H + 300), 26, steps=(3, 6))
            windsock(sc, g, TX + 20, TY + 20, H, H + 430.0, length=300.0, fat=64.0)
        else:
            for sx in (-1, 1):
                pennants(sc, (0, 60, CR), (sx * (TX + 40), -(TY + 30), H + 110), (PINK, WHITE, CYAN, YEL, VIOLET), n=7, drop=110.0, swing=4.0, rate=0.5)
    return build


def crag(seed, goats):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        # the stack: tall heads in the middle and at the back, steps down to the ends and the front
        cols = (  # x, y, radius, height
            (-110, 60, 150, 1390), (150, 70, 150, 1320), (20, -120, 140, 1270), (-330, 30, 140, 1180), (400, 40, 140, 1250),
            (-250, -140, 120, 980), (270, -150, 125, 1060), (-520, -40, 120, 860), (590, -50, 115, 900), (20, 230, 150, 1100),
            (-300, 210, 130, 900), (330, 220, 130, 940), (-90, -255, 100, 640), (150, -265, 95, 520),
        )
        for i, (x, y, rad, h) in enumerate(cols):
            hexcol(g, x, y, rad, h, col=(BAS, BAS_L)[i % 2])
        acts = cheers(0.7)
        if not goats:
            # a rope ladder down the front, and villagers on the heads
            for sx in (-1, 1):
                g.box(ROPE, (20 + sx * 30, -264, 1270 - 380), (6, 6, 760))
            for k in range(8):
                g.box(PLANK, (20, -264, 560 + k * 92), (60, 6, 6))
            spots = ((-110, 30, 1390, 0), (150, 40, 1320, 1), (20, -130, 1270, 1), (-330, 10, 1180, 0), (400, 20, 1250, 0),
                     (270, -150, 1060, 0), (-250, -140, 980, 0))
            for i, (x, y, z, det) in enumerate(spots):
                d = villager(r)
                d.update(acts[(i * 5 + seed) % len(acts)])
                if i in (0, 4):
                    d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag((PINK, CYAN)[i % 2], WHITE, w=130.0), swing_r=20, rate=1.0, bob=4)
                if i == 6:
                    d.update(build='kid', s=0.66)
                person(sc, x, y, z, d, BIG, yaw=r.uniform(-16, 16), phase=r.random(), react=(i % 2 == 0), detail=det)
            pennants(sc, (-330, 30, 1180 + 560), (400, 40, 1250 + 560), (YEL, WHITE, PINK, CYAN), n=8, drop=120.0, swing=4.0, rate=0.5)
            for x, z in ((-330, 1180), (400, 1250)):
                g.box(PLANK_D, (x, 110 if x < 0 else 120, z + 280), (10, 10, 560))
        else:
            # goats on the heads and ledges (twice life size), their herder with his staff, a child
            herd = ((150, 60, 1320, 90, '#fff6ea'), (20, -120, 1270, -60, '#c9a070'), (400, 40, 1250, 100, '#e8d8b8'),
                    (-250, -140, 980, -80, '#fff6ea'), (270, -150, 1060, 70, '#8e5a3a'), (590, -50, 900, 110, '#fff6ea'))
            for i, (x, y, z, yaw, col) in enumerate(herd):
                P.quadruped(sc, x, y, z, yaw=yaw, s=2.0, body=col, length=80, height=62, girth=26, neck=18, neck_up=40 if i % 2 else -15,
                            head=22, horns=PALE, tail=12, ears=col, phase=r.random(), rate=0.5 + 0.2 * (i % 3), bob=4 if i % 2 else 0,
                            graze=14 if i % 2 == 0 else 6, wag=24)
            d = villager(r, hat='sun', hat_col=YEL, wear='coat', top=CYAN)
            d.update(arm_r='up', arm_l='hip', hold_r=big_flag(ORANGE, WHITE, w=120.0), swing_r=20, rate=0.9, bob=3)
            person(sc, -110, 30, 1390, d, BIG, yaw=-8, phase=0.2, react=True, detail=1)
            d = villager(r, build='kid', s=0.66)
            d.update(acts[0])
            person(sc, -330, 10, 1180, d, BIG, yaw=10, phase=0.7, detail=0)
            d = villager(r)
            d.update(acts[3])
            person(sc, -520, -40, 860, d, BIG, yaw=-20, phase=0.4, detail=0)
    return build


def rope_bridge(sc):
    """A footbridge between two basalt pillars; villagers cross it both ways."""
    r = random.Random(341)
    g = sc.still()
    Z, HALF = 1300.0, 1000.0
    for sx in (-1, 1):
        cx = sx * (HALF + 170)
        hexcol(g, cx, 0, 190, Z)
        hexcol(g, cx + sx * 150, 150, 150, Z - 260, col=BAS_L)
        hexcol(g, cx + sx * 40, -190, 120, Z - 520)
        hexcol(g, cx - sx * 110, 200, 120, Z - 700, col=BAS_L)
        for sy in (-1, 1):
            g.box(PLANK_D, (sx * (HALF + 30), sy * 95, Z + 210), (16, 16, 420))
        g.box(PLANK_D, (sx * (HALF + 30), 0, Z + 412), (16, 206, 16))
    # the deck (planks in two colours), hand ropes, the two carrying ropes sagging from the posts, hangers
    n = 10
    for k in range(n):
        g.box((PLANK, PLANK_D)[k % 2], (-HALF + (k + 0.5) * 2 * HALF / n, 0, Z - 9), (2 * HALF / n, 170, 14))
    seg = 8
    for sy in (-1, 1):
        g.box(ROPE, (0, sy * 88, Z + 96), (2 * HALF, 5, 6))
        pts = []
        for k in range(seg + 1):
            f = -1.0 + 2.0 * k / seg
            pts.append((f * (HALF + 30), sy * 95, Z + 120 + 290 * f * f))
        for k in range(seg):
            tilt_box(g, ROPE, pts[k], pts[k + 1], 7, 7)
        for k in range(1, seg):
            g.box(ROPE, (pts[k][0], sy * 92, (Z + pts[k][2]) / 2.0), (5, 5, pts[k][2] - Z))
    fascia(g, -HALF, HALF, -90, Z - 30, (PINK, WHITE), n=14, h=30.0)
    pennants(sc, (-HALF - 30, 0, Z + 412), (HALF + 30, 0, Z + 412), (YEL, PINK, CYAN, WHITE, VIOLET), n=12, drop=90.0, swing=4.0, rate=0.5)
    # the walkers: four go one way and three the other, in two files that pass each other
    span = 2.0 * HALF
    for i in range(7):
        fwd = i % 2 == 0
        d = villager(r)
        if i == 3:
            d.update(build='kid', s=0.66)
        d.update(arm_l='down', arm_r='carry' if i % 3 == 0 else 'down', hold_r=hold_bundle(r.choice(CLOTH)) if i % 3 == 0 else None,
                 stride=24, swing_l=18, swing_r=0 if i % 3 == 0 else -18, axis_l='x', axis_r='x', rate=1.1, bob=3)
        if i % 3 == 1:
            d.update(pack=r.choice(CLOTH))
        x = -HALF + (i + 0.5) * span / 7.0
        person(sc, x, 34 if fwd else -34, Z, d, 1.6, yaw=90 if fwd else -90, phase=r.random(), detail=0,
               travel=(span, 95.0 if fwd else -80.0))
    # one on each pillar, waving the walkers on and waving at the chase
    acts = cheers()
    for sx in (-1, 1):
        d = villager(r)
        d.update(acts[1 if sx < 0 else 4])
        if sx > 0:
            d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag(CYAN, WHITE, w=120.0), swing_r=20, rate=1.0, bob=4)
        person(sc, sx * (HALF + 230), -40, Z, d, BIG, yaw=-sx * 14, phase=r.random(), react=True, detail=1)


def headframe(sc):
    """A mine headframe: miners on the platform, the sheave rocking, the cage riding up and down."""
    r = random.Random(351)
    g = sc.still()
    H, TX, TY = 1300.0, 240.0, 160.0
    lattice(g, H, 270.0, 190.0, TX, TY, rings=(0.34, 0.66))
    for sx in (-1, 1):          # the raking back legs
        tilt_box(g, STEEL_D, (sx * 150, 520, -250), (sx * 110, TY - 20, H + 250), 22, 22)
    g.box(STEEL_D, (0, 330, H * 0.5), (280, 11, 11))
    g.box(PLANK, (0, 0, H - 9), (TX * 2 + 90, TY * 2 + 80, 18))
    fascia(g, -(TX + 45), TX + 45, -(TY + 45), H - 36, (YEL, DARK), n=10, h=72.0)
    rail(g, -(TX + 38), TX + 38, -(TY + 30), H, posts=5, col=YEL)
    # the sheave house over the platform and the wheel in it, rocking as the rope runs
    WZ = H + 420.0
    for sx in (-1, 1):
        tilt_box(g, STEEL_D, (sx * 170, TY - 10, H), (sx * 60, 60, WZ), 16, 16)
        tilt_box(g, STEEL_D, (sx * 170, -40, H), (sx * 60, 60, WZ), 16, 16)
    g.box(STEEL_D, (0, 60, WZ), (150, 16, 16))
    w = sc.part(P.motion(swing=55.0, pivot=(0.0, 60.0, WZ), axis=90.0, rate=0.22, phase=0.2))
    rod(w, DARK, (0, 48, WZ), (0, 72, WZ), 150, steps=12)
    w.box(YEL, (0, 40, WZ), (44, 10, 44))
    for a in (0, 60, 120):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(w, YEL, (-ca * 142, 40, WZ - sa * 142), (ca * 142, 40, WZ + sa * 142), 10, 18)
        tilt_box(w, YEL, (-ca * 142, 80, WZ - sa * 142), (ca * 142, 80, WZ + sa * 142), 10, 18)
    # the cage between the legs, on its rope: it floats 2.6 m up and down, its two riders with it
    CZ = 620.0
    ride = dict(bob=-260.0, rate=0.11, phase=0.3)
    c = sc.part(P.motion(**ride))
    c.box(STEEL, (0, 0, CZ - 6), (230, 190, 12))
    c.box(STEEL, (0, 0, CZ + 330), (230, 190, 12))
    for sx in (-1, 1):
        for sy in (-1, 1):
            c.box(YEL, (sx * 109, sy * 89, CZ + 162), (10, 10, 330))
    c.box(YEL, (0, -92, CZ + 110), (230, 6, 8))
    c.box(DARK, (0, 0, CZ + 336 + 240), (7, 7, 480))
    for i, x in enumerate((-52, 52)):
        d = miner(r)
        d.update(arm_r='wave' if i else 'down', arm_l='down' if i else 'hip', swing_r=18 if i else 0)
        person(sc, x, 0, CZ, d, 1.5, yaw=(-1) ** i * 10, detail=0, **ride)
    # four on the platform: one with the flag, the rest cheer and point at the chase
    acts = cheers()
    for i, x in enumerate((-204, -68, 68, 204)):
        d = miner(r)
        d.update(acts[(i * 5 + 2) % len(acts)])
        if i == 2:
            d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag(YEL, DARK, w=120.0), swing_r=20, rate=1.0, bob=4)
        person(sc, x + r.uniform(-8, 8), -(TY - 40) + r.uniform(-8, 8), H, d, BIG, yaw=r.uniform(-14, 14), phase=r.random(), react=(i % 2 == 0),
               detail=1 if i == 2 else 0)
    ladder(g, TX + 30, TY + 50, 0, H, w=60.0, col=YEL, rungs=10)


def arch(sc):
    """A basalt arch: villagers on its lintel, a banner under it, a climber on a rope in the opening."""
    r = random.Random(361)
    g = sc.still()
    Z, HALF = 1300.0, 520.0
    for sx in (-1, 1):
        cx = sx * (HALF + 150)
        hexcol(g, cx, 0, 180, Z - 150, cap=BAS_L)
        hexcol(g, cx + sx * 170, 110, 150, Z - 420)
        hexcol(g, cx + sx * 60, -180, 120, Z - 700, col=BAS_L)
        hexcol(g, cx + sx * 250, -70, 110, Z - 880, col=BAS_L)
    # the lintel: three slabs, the middle one thinner (an arch, not a beam), ash on top
    g.box(BAS, (0, 0, Z - 85), (2 * HALF + 660, 330, 150))
    g.box(BAS_L, (-(HALF - 60), 0, Z - 215), (300, 300, 130))
    g.box(BAS_L, (HALF - 60, 0, Z - 215), (300, 300, 130))
    g.box(PALE, (0, 0, Z - 5), (2 * HALF + 640, 310, 10))
    acts = cheers(0.7)
    for i, x in enumerate((-560, -340, -120, 120, 340, 560)):
        d = villager(r)
        d.update(acts[(i * 5 + 3) % len(acts)])
        if i in (1, 4):
            d.update(arm_r='up', arm_l='hip', swing_l=0, hold_r=big_flag((VIOLET, YEL)[i % 2], WHITE, w=130.0), swing_r=20, rate=1.0, bob=4)
        if i == 3:
            d.update(build='kid', s=0.66)
        person(sc, x + r.uniform(-10, 10), -60 + r.uniform(-10, 10), Z, d, BIG, yaw=r.uniform(-14, 14), phase=r.random(), react=(i % 2 == 1),
               detail=1 if i in (1, 4) else 0)
    # the banner under the lintel, stirring
    b = sc.part(P.motion(swing=6.0, pivot=(-200.0, -150.0, Z - 160.0), axis=0.0, rate=0.5))
    for k, col in enumerate((PINK, WHITE, CYAN, WHITE)):
        b.box(col, (-380 + k * 120 + 60, -152, Z - 330), (120, 5, 340))
    # the climber: a rope from the lintel, and a figure on it that goes up and down in the opening
    ride = dict(bob=-190.0, rate=0.14, phase=0.6)
    c = sc.part(P.motion(**ride))
    cx, cz = 250.0, 620.0
    c.box(ROPE, (cx, -150, cz + 420), (7, 7, 700))
    c.box(ORANGE, (cx, -150, cz + 60), (60, 40, 14))
    d = villager(r, top=ORANGE, hat='helmet', hat_col=LIME, wear='trousers')
    d.update(arm_l='up', arm_r='up', swing_l=8, swing_r=-8)
    person(sc, cx, -150, cz - 10, d, 1.6, legs='sit', yaw=0, detail=1, **ride)


def signal(sc):
    """A signal mast: a keeper swinging two flags, two striped signal arms swinging at the head."""
    r = random.Random(371)
    g = sc.still()
    H = 1400.0
    lattice(g, H, 120.0, 120.0, 46.0, 46.0, rings=(0.25, 0.5, 0.75), leg=14.0)
    for a in (45, 165, 285):          # guys
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(g, STEEL, (ca * 330, sa * 200, -100), (ca * 40, sa * 40, H * 0.72), 6, 6)
    g.cyl(PLANK, (0, 0, H - 16), 190, 16, steps=8)
    g.cyl(ORANGE, (0, 0, H - 50), 196, 34, steps=8)
    for k in range(8):
        a, b = math.radians(k * 45 + 22.5), math.radians(k * 45 + 67.5)
        pa, pb = (math.cos(a) * 178, math.sin(a) * 178, H + 100), (math.cos(b) * 178, math.sin(b) * 178, H + 100)
        g.box(WHITE, (pa[0], pa[1], H + 50), (8, 8, 100))
        tilt_box(g, WHITE, pa, pb, 7, 7)
    # the mast above the platform and its two signal arms
    MZ = H + 620.0
    g.box(STEEL_D, (0, 90, H + 330), (14, 14, 660))
    g.sphere(YEL, (0, 90, MZ + 60), 30, steps=(4, 8))
    for sx, ph in ((-1, 0.0), (1, 0.5)):
        m = sc.part(P.motion(swing=38.0, pivot=(0.0, 90.0, MZ - 60.0), axis=90.0, rate=0.35, phase=ph))
        for k in range(4):
            m.box((ORANGE, WHITE)[k % 2], (sx * (30 + (k + 0.5) * 55), 82, MZ - 60), (55, 8, 46))
    d = P.look(r, tops=(CYAN,), wear='coat', sleeves='long', hat='peak', hat_col=ORANGE, band=YEL)
    d.update(arm_l='out', arm_r='out', hold_l=big_flag(YEL, PINK, w=120.0), hold_r=big_flag(PINK, YEL, w=120.0), swing_l=34, swing_r=34, rate=0.8, bob=2)
    person(sc, 0, -70, H, d, 1.9, yaw=0, phase=0.1, react=True, detail=1)
    d = villager(r)
    d.update(arm_r='wave', swing_r=28, rate=1.2, arm_l='down')
    person(sc, 96, -150, H - 46.0 * 1.5, d, 1.5, legs='sit', yaw=10, phase=0.5, detail=0)
    ladder(g, 0, 122, 0, H, w=54.0, rungs=12)


JOBS = [
    ('SM_P3D_Lookout_A', V.lifted(lookout(321, False), 0.035)),    # the near scaffolds read black against the sky: the level's small steady lift (33 m out)
    ('SM_P3D_Lookout_B', V.lifted(lookout(322, True), 0.035)),
    ('SM_P3D_Crow', V.lifted(crow, 0.035)),
    ('SM_P3D_Derrick_A', derrick(323, True)),
    ('SM_P3D_Derrick_B', derrick(324, False)),
    ('SM_P3D_Crag_A', crag(325, False)),
    ('SM_P3D_Crag_B', crag(326, True)),
    ('SM_P3D_RopeBridge', rope_bridge),
    ('SM_P3D_Headframe', headframe),
    ('SM_P3D_Arch', arch),
    ('SM_P3D_Signal', signal),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
