"""FULL THROTTLE (Unreal) the population of Ice Mountain: THE WATCHERS (an extra worker's kit, 15 Nov 2026).

The people who watch the chase from 30 to 56 m: a race-finish stand under a chequered gantry, crowds on ploughed
snow banks behind an orange net, a scaffold tower of flag wavers, a snow groomer with people on its roof, a lift
platform with a queue of skiers. Raised, massed, in hot ski-suit colours, with big flags (stripes and blocks,
never words). Every figure hops or waves out of step and livens up as the chase comes by (react).

Looks only: no collision, no glow, nothing that blinks. Every piece is TALL (24 m or more from the road; the
table writes none nearer than 30 m). The hill falls 7 in 100 along the road and a piece stands level, so every
piece has a foot that goes 1.2 m into the ground.

Meshes: /Game/People/IceMountain/SM_Watch*   (that prefix is this kit's alone on this level)
Run in the editor:  import ft_people_ice_watch as K; K.run()         (or K.run(['SM_WatchBank_A']))
The rules that place them: the "watch" block of Source/FullThrottle/FTPeopleL5.inl.
"""
import random
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/IceMountain'

SNOW, SNOW_SH, ICE = '#f4f9ff', '#b9c6ea', '#7fb0e0'
WOOD, WOOD_D, STEEL, STEEL_D = '#9a6a44', '#6a4c36', '#9aa4b8', '#5a6478'
RED, BLUE, ORANGE, PINK, TEAL, YEL, LIME, VIOLET = '#ff2d3c', '#2e6bff', '#ff8a2a', '#ff4fb8', '#19b5c9', '#ffd23f', '#b8ff5a', '#8f6bff'
DARK, WHITE = '#22202a', '#fff6ea'
SUITS = (RED, PINK, ORANGE, YEL, LIME, '#2ee6ff', BLUE, VIOLET, '#ff5fa2', '#7dffb0')
LEGS = ('#3a5fa8', WHITE, VIOLET, TEAL, DARK)
BIG = 1.6            # people here are 1.6 times life size: at 40 m a life-size figure is a speck behind the barrier
FOOT = 120.0         # how far every piece goes into the ground (the hill falls 7 in 100 under a level piece)


def watcher(r, **fixed):
    """Somebody in a ski suit or a padded coat, a bobble hat or a helmet: one hot colour, big."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear=r.choice(('trousers', 'trousers', 'coat')), top=c, bottom=r.choice((c,) + LEGS),
               hat=r.choice(('beanie', 'beanie', 'helmet', 'cap')), hat_col=r.choice(SUITS), shoes=r.choice((c, WHITE, DARK)),
               hair=r.choice(('short', 'long', 'bun')), s=BIG * r.uniform(0.92, 1.08))
    d.update(fixed)
    return d


def big_flag(col=RED, stripe=WHITE, pole=WHITE, w=170.0, h=100.0, length=270.0):
    """A big flag on a pole in the hand, a field with a square of a second colour: it swings with the arm."""
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + length / 2.0 - 20), (5, 5, length))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + length - 20 - h / 2.0), (w, 3, h))
        m.box(stripe, (hd[0] + side * w / 2.0, hd[1], hd[2] + length - 20 - h * 0.5), (h * 0.5, 4.2, h * 0.5))     # a square in the field: no country's flag
    return f


def ski_pole(col=DARK, length=125.0, grip=RED):
    """A ski pole held up by its middle."""
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1], hd[2] + length / 2.0 - 18), (4.5, 4.5, length))
        m.box(grip, (hd[0], hd[1], hd[2] + length - 18), (11, 11, 5))
    return f


def skis(col=RED, tip=WHITE, length=200.0):
    """A pair of skis stood on end beside the body, held at the hip."""
    def f(m, hd, side):
        for k in (-1, 1):
            m.box(col, (hd[0] + side * 10 + k * 6, hd[1], hd[2] - 40 + length / 2.0), (9, 4, length))
        m.box(tip, (hd[0] + side * 10, hd[1], hd[2] - 40 + length - 12), (22, 5, 24))
    return f


def bell(col=YEL):
    """A cow bell, rung."""
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1] - 4, hd[2] - 16), (24, 16, 28))
    return f


# what a watcher does: big, slow, readable
ACTS = (
    dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=20, rate=1.4),
    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=24, rate=1.2),
    dict(arm_r='wave', swing_r=30, rate=1.1, bob=10),
    dict(arm_l='wave', swing_l=30, rate=0.9, bob=14),
    dict(arm_l='vee', arm_r='vee', swing_l=18, swing_r=-18, bob=12, rate=1.0, poles=True),
    dict(arm_r='fwd', arm_l='hip', swing_r=14, axis_r='x', rate=1.6, bob=6, ring=True),
)


def crowd(sc, r, spots, flags=(RED, YEL), flag_every=4, seed=0, kid_at=(), yaw=0.0):
    """One watcher on each spot (x, y, z), each with its own act, phase and colours."""
    for k, (x, y, z) in enumerate(spots):
        d = watcher(r)
        a = dict(ACTS[(k * 5 + seed) % len(ACTS)])
        if a.pop('poles', False):
            c = r.choice((DARK, RED, BLUE))
            a.update(hold_l=ski_pole(c), hold_r=ski_pole(c))
        if a.pop('ring', False):
            a.update(hold_r=bell())
        if flag_every and k % flag_every == 1:
            c = flags[(k // flag_every) % len(flags)]
            a = dict(arm_r='up', arm_l='hip', swing_r=22, rate=0.8, bob=6,
                     hold_r=big_flag(c, WHITE if c != WHITE else RED))
        if k in kid_at:
            d.update(build='kid', s=BIG * 0.72)
            a.update(bob=26, rate=1.8)
        d.update(a)
        P.figure(sc, x, y, z, yaw=yaw + r.uniform(-14, 14), detail=0, phase=r.random(), react=True, **d)


def bunting(sc, a, b, n=8, cols=(RED, YEL, TEAL, PINK), drop=40.0, size=(46.0, 56.0), phase=0.0):
    """A line of pennants from a to b that lifts and falls in the wind (blocks of colour, each on its own beat)."""
    g = sc.still()
    tilt_box(g, DARK, a, b, 4, 4)
    for i in range(n):
        f = (i + 0.5) / n
        sag = drop * 4.0 * f * (1.0 - f)
        x, y, z = a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f - sag * 0.2
        m = sc.part(P.motion(phase=phase + i * 0.17, rate=0.9, swing=16.0, pivot=(x, y, z), axis=0.0 if abs(b[0] - a[0]) > abs(b[1] - a[1]) else 90.0))
        m.box(cols[i % len(cols)], (x, y, z - size[1] / 2.0), (size[0], 3, size[1]))


# ------------------------------------------------------------------------------------------ the race finish
def finish(seed=21, skirt=(PINK, WHITE, TEAL, YEL), flags=(RED, YEL)):
    """A race-finish stand: three steps of benches, fourteen people, under a chequered gantry with pennants.
    12 m long, the gantry 10 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        length, depth, rise, first = 1200.0, 150.0, 100.0, 140.0
        n = 8
        for k in range(n):       # the skirt: blocks of colour along the front
            g.box(skirt[k % len(skirt)], (-length / 2.0 + (k + 0.5) * length / n, -depth / 2.0 - 4, (first - FOOT) / 2.0), (length / n, 8, first + FOOT))
        for t in range(3):
            z = first + t * rise
            g.box(WOOD, (0, t * depth, z - 7), (length, depth, 14))
            g.box(STEEL_D, (0, t * depth + depth / 2.0 - 4, (z - FOOT) / 2.0), (length, 8, z + FOOT))
            for ex in (-1, 1):   # the ends step up in colour: the road sees a stand end-on first
                g.box(skirt[(t + 1) % len(skirt)], (ex * length / 2.0, t * depth, (z - FOOT) / 2.0), (10, depth, z + FOOT))
        # the gantry behind: two posts, a chequered beam
        top = 1000.0
        py = 2 * depth + depth / 2.0 + 30
        for ex in (-1, 1):
            g.box(RED, (ex * (length / 2.0 + 40), py, (top - FOOT) / 2.0), (36, 36, top + FOOT))
            g.box(WHITE, (ex * (length / 2.0 + 40), py, 520), (40, 40, 90))
        cells = 13
        cw = (length + 116.0) / cells
        for i in range(cells):
            for j in range(2):
                g.box(DARK if (i + j) % 2 else WHITE, (-(length + 116.0) / 2.0 + (i + 0.5) * cw, py, top - 160 + j * 80 + 40), (cw, 22, 80))
        bunting(sc, (-length / 2.0 - 40, py - 20, top - 180), (0, -depth / 2.0, 520), n=7, phase=0.1)
        bunting(sc, (length / 2.0 + 40, py - 20, top - 180), (0, -depth / 2.0, 520), n=7, cols=(YEL, PINK, TEAL, RED), phase=0.5)
        spots = []
        for t, cnt in enumerate((5, 4, 5)):
            for i in range(cnt):
                spots.append((-length / 2.0 + (i + 0.5) * length / cnt + r.uniform(-18, 18), t * depth + r.uniform(-12, 12), first + t * rise))
        crowd(sc, r, spots, flags=flags, flag_every=5, seed=seed, kid_at=(2, 11))
    return build


# ------------------------------------------------------------------------------------------ the snow bank
def bank(seed=31, net=ORANGE, flags=(RED, YEL), banner=None):
    """A ploughed snow bank cut in three steps behind a safety net, thirteen people on it in ski suits, waving poles
    and flags, ringing bells. banner: two colours for a striped banner on two posts over the top row. 16 m long."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        length, depth, rise, first = 1600.0, 170.0, 100.0, 130.0
        for t in range(3):
            z = first + t * rise
            L2 = length - t * 140.0
            g.box(SNOW_SH, (0, t * depth, (z - FOOT) / 2.0 - 5), (L2, depth, z + FOOT - 10))
            g.box(SNOW, (0, t * depth + 6, z - 5), (L2 + 30, depth + 12, 10))
            for ex in (-1, 1):   # ploughed lumps at the ends
                g.box(SNOW, (ex * (L2 / 2.0 + 40), t * depth, (z - 60 - FOOT) / 2.0), (110, depth + 40, z - 60 + FOOT))
        g.box(ICE, (0, 3 * depth - depth / 2.0 + 40, (first + 2 * rise - FOOT) / 2.0), (length - 280, 80, first + 2 * rise + FOOT))
        # the net along the foot: posts and two bands of colour
        ny = -depth / 2.0 - 46
        for i in range(9):
            g.box(DARK, (-length / 2.0 + i * length / 8.0, ny, 55 - FOOT / 2.0), (9, 9, 110 + FOOT))
        g.box(net, (0, ny, 88), (length, 4, 34))
        g.box(net, (0, ny, 40), (length, 4, 34))
        spots = []
        for t, cnt in enumerate((5, 4, 4)):
            L2 = length - t * 140.0 - 160.0
            for i in range(cnt):
                spots.append((-L2 / 2.0 + (i + 0.5) * L2 / cnt + r.uniform(-25, 25), t * depth + r.uniform(-20, 20), first + t * rise))
        if banner:
            z = first + 2 * rise
            by = 2 * depth + 70
            for ex in (-1, 1):
                g.box(WOOD_D, (ex * 420, by, z + 260 - FOOT / 2.0), (14, 14, 520 + FOOT))
            m = sc.part(P.motion(phase=0.3, rate=0.7, bob=-10.0))
            for i in range(6):
                m.box(banner[i % 2], (-420 + (i + 0.5) * 140, by - 6, z + 430), (140, 5, 150))
        crowd(sc, r, spots, flags=flags, flag_every=0 if banner else 4, seed=seed, kid_at=(3, 8))
    return build


# ------------------------------------------------------------------------------------------ the flag tower
def tower(seed=41, drape=(RED, WHITE, RED, WHITE), flags=(RED, YEL, TEAL)):
    """A scaffold tower: a drape of stripes, three people at the rail of the first deck, three on top swinging big
    flags. 4 m square, 13 m to the flags' tips: TALL, and meant to show over pines and ice."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h1, h2, half = 300.0, 660.0, 200.0
        for ex in (-1, 1):
            for ey in (-1, 1):
                g.box(STEEL_D, (ex * half, ey * half, (h2 + 110 - FOOT) / 2.0), (14, 14, h2 + 110 + FOOT))
            tilt_box(g, STEEL, (ex * half, -half, 0), (ex * half, half, h1), 9, 9)
            tilt_box(g, STEEL, (ex * half, half, h1), (ex * half, -half, h2), 9, 9)
        tilt_box(g, STEEL, (-half, half, 0), (half, half, h1), 9, 9)
        tilt_box(g, STEEL, (half, half, h1), (-half, half, h2), 9, 9)
        for z in (h1, h2):
            g.box(WOOD, (0, 0, z - 8), (2 * half + 40, 2 * half + 40, 16))
        n = len(drape)
        for k in range(n):       # the drape down the front, ground to first deck, and a band at each rail
            g.box(drape[k], (-half + (k + 0.5) * 2 * half / n, -half - 10, (h1 - FOOT) / 2.0), (2 * half / n, 6, h1 + FOOT))
        g.box(YEL, (0, -half - 10, h1 + 70), (2 * half + 30, 6, 40))
        g.box(PINK, (0, -half - 10, h2 + 70), (2 * half + 30, 6, 40))
        for ex in (-1, 1):
            g.box(TEAL, (ex * (half + 10), 0, h1 + 70), (6, 2 * half + 30, 40))
            g.box(TEAL, (ex * (half + 10), 0, h2 + 70), (6, 2 * half + 30, 40))
        low = [(-130, -120, h1), (0, -90, h1), (130, -120, h1)]
        crowd(sc, r, low, flag_every=0, seed=seed)
        for k, x in enumerate((-125, 10, 140)):
            d = watcher(r)
            c = flags[k % len(flags)]
            d.update(arm_r='up', arm_l='hip' if k != 1 else 'up', swing_r=26, rate=0.7 + 0.12 * k, bob=8,
                     hold_r=big_flag(c, WHITE, w=230.0, h=130.0, length=330.0))
            if k == 1:
                d.update(swing_l=-20)
            P.figure(sc, x, -100 + 60 * (k % 2), h2, yaw=r.uniform(-12, 12), detail=0, phase=r.random(), react=True, **d)
    return build


# ------------------------------------------------------------------------------------------ the groomer
def groomer(seed=51, body=TEAL, trim=ORANGE):
    """A snow groomer parked side-on, used as a stand: two people on the cab's roof, three on the deck behind it,
    three on the snow heaped at its blade; a flag on its mast. 9 m long, 6 m to the roof people's heads: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(SNOW_SH, (0, 20, (40 - FOOT) / 2.0), (980, 420, 40 + FOOT))          # the ground it stands on, levelled
        for ey in (-1, 1):                                                        # tracks
            g.box(DARK, (40, ey * 135, 70), (640, 70, 100))
            g.box(STEEL_D, (40, ey * 135, 70), (560, 76, 40))
        g.box(body, (40, 0, 165), (600, 250, 110))                                # hull
        g.box(trim, (40, -127, 150), (600, 6, 30))
        g.box(body, (-150, 0, 290), (240, 240, 150))                              # cab
        g.box(DARK, (-150, -122, 300), (200, 6, 80))                              # its windows
        g.box(DARK, (-272, 0, 300), (6, 200, 80))
        g.box(WHITE, (-150, 0, 370), (260, 260, 12))                              # roof
        tilt_box(g, trim, (-330, 0, 40), (-400, 0, 190), 300, 16)                 # the blade
        g.box(SNOW, (-470, 0, (120 - FOOT) / 2.0), (170, 520, 120 + FOOT))        # snow heaped at the blade
        g.box(SNOW, (-520, 60, (70 - FOOT) / 2.0), (200, 600, 70 + FOOT))
        g.box(WOOD_D, (190, 0, 226), (300, 250, 12))                              # the deck behind the cab
        g.box(trim, (190, -127, 262), (300, 6, 60))
        g.box(STEEL_D, (330, 110, 420), (10, 10, 400))                            # the mast
        m = sc.part(P.motion(phase=0.2, rate=0.6, swing=18.0, pivot=(330, 110, 560), axis=P.UP))
        m.box(PINK, (330 + 110, 110, 560), (220, 4, 120))
        m.box(YEL, (330 + 110, 110, 560), (60, 5.4, 60))
        spots = [(-200, -40, 376), (-95, 30, 376), (90, 0, 232), (200, -40, 232), (290, 20, 232),
                 (-470, -160, 120), (-460, 20, 120), (-480, 190, 120)]
        crowd(sc, r, spots, flags=(YEL, RED), flag_every=4, seed=seed, kid_at=(6,))
    return build


# ------------------------------------------------------------------------------------------ the lift platform
def lift(seed=61, rail=PINK):
    """A chair lift's loading platform on stilts: the bull wheel on its gantry, a chair swinging with two riders
    who wave, and the queue at the rail with their skis stood up, watching the chase. 11 m long, 9 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        length, deep, dz = 1100.0, 320.0, 240.0
        g.box(WOOD, (0, 0, dz - 9), (length, deep, 18))
        for i in range(5):
            for ey in (-1, 1):
                g.box(WOOD_D, (-length / 2.0 + 40 + i * (length - 80) / 4.0, ey * (deep / 2.0 - 20), (dz - FOOT) / 2.0), (18, 18, dz + FOOT))
        for k in range(8):       # the rail's panels along the front
            g.box(rail if k % 2 == 0 else WHITE, (-length / 2.0 + (k + 0.5) * length / 8.0, -deep / 2.0 - 4, dz + 45), (length / 8.0, 6, 70))
        for i in range(6):       # steps at the downhill end
            g.box(WOOD_D, (length / 2.0 + 25 + i * 45, 0, (dz - 40 - i * 40 - FOOT) / 2.0), (45, deep * 0.6, dz - 40 - i * 40 + FOOT))
        # the gantry and the bull wheel
        gx, top = -330.0, 840.0
        for ey in (-1, 1):
            g.box(BLUE, (gx, ey * 120, (top + dz) / 2.0), (26, 26, top - dz))
        g.box(BLUE, (gx, 0, top), (40, 300, 30))
        g.cyl(YEL, (gx, 0, top + 16), 190, 16, steps=10)
        g.cyl(DARK, (gx, 0, top + 30), 46, 10, steps=8)
        tilt_box(g, DARK, (gx, -190, top + 22), (length / 2.0 + 300, -190, top + 60), 5, 5)       # the cable going away
        # a chair hanging from the cable, swinging, two riders
        cx, cz = 150.0, top + 40.0
        sw = dict(swing=7.0, pivot=(cx, -190, cz), axis=0.0)
        ch = sc.part(P.motion(phase=0.4, rate=0.5, **sw))
        ch.box(STEEL_D, (cx, -190, cz - 150), (8, 8, 300))
        ch.box(RED, (cx, -190, cz - 300), (200, 60, 12))
        ch.box(RED, (cx, -160, cz - 250), (200, 10, 90))
        for k, ox in enumerate((-48, 48)):
            d = watcher(r, s=1.35)
            d.update(legs='sit', arm_r='wave' if k else 'down', arm_l='down' if k else 'wave')
            P.figure(sc, cx + ox, -190, cz - 300 - 46 * 1.35, detail=0, phase=0.4, rate=0.5, sway=sw, **d)
        spots = [(-470 + i * 150, -90 + (i % 2) * 70, dz) for i in range(7)]
        for k, (x, y, z) in enumerate(spots):
            d = watcher(r)
            a = dict(ACTS[(k * 5 + seed) % 4])
            if k % 2 == 0:
                a = dict(arm_r='wave', swing_r=30, rate=1.0 + 0.1 * k, bob=8, arm_l='hip', hold_l=skis(r.choice((RED, YEL, LIME, PINK))))
            d.update(a)
            P.figure(sc, x, y, z, yaw=r.uniform(-14, 14), detail=0, phase=r.random(), react=True, **d)
    return build


JOBS = [
    ('SM_WatchFinish_A', finish()),
    ('SM_WatchBank_A', bank()),
    ('SM_WatchBank_B', bank(seed=37, net=PINK, banner=(RED, WHITE))),
    ('SM_WatchTower_A', tower()),
    ('SM_WatchGroomer_A', groomer()),
    ('SM_WatchLift_A', lift()),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
