"""FULL THROTTLE (Unreal) the population, level 0, Bayshore: THE WATCHERS (/Game/People/Bayshore/SM_Watch...).

The chase is the event of the afternoon and the town has built itself places to watch it from. Every piece here
brings its own height (the chase camera rides 3.5 m over a road whose barrier is 1.6 m high, and the land lies a
metre under the road: a person on the ground is a head behind the barrier), a MASS of people in hot colours, and
big out-of-step movement: hopping, both arms up, flags and striped banners. All TALL: 24 m or more from the road.
Placed by the "watch" block of Source/FullThrottle/FTPeopleL0.inl.

    SM_WatchStand_A / _B    a scaffold grandstand of holidaymakers, 14 m long, four tiers, striped skirt and back
                            boards, pennants. The bay side, right behind the boardwalk.
    SM_WatchTower_A / _B    a scaffold platform 4.3 m up under a striped canopy: a knot of people at its rail, a big
                            flag. Small footprint: stands on slopes, beside the villas, between the beach clubs.
    SM_WatchDeck            a beach bar's deck on stilts, parasols on it, its customers all at the road-side rail.
    SM_WatchTerrace         a hotel's sun terrace across the mouth of a side street, behind the promenade: two
                            stepped floors of guests at the rails, each over a striped valance. The town side.

Half of every crowd livens up as the chase comes by (react), half never rests, so a stand is in motion from far off.
Run in the editor:  import ft_people_bay_watch as K; K.run()        (or K.run(['SM_WatchStand_A']))
"""
import random
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/Bayshore'

WOOD, WOOD_D, WHITE, CREAM = '#c9a06a', '#9a744a', '#fffaf2', '#fff1d6'
CORAL, TEAL, YEL, HOTPINK, MINT, SKY, NAVY, RED, LAV, PEACH = ('#ff7a8a', '#19c5c0', '#ffc93c', '#ff4fb8', '#7dffb0', '#57c7ff', '#2b2350',
                                                                 '#ff2d3c', '#b9a4ff', '#ffc9a0')
STEEL = '#c9c4d6'
HOT = ('#ff4fb8', '#ff2d3c', '#ffc93c', '#2ee6ff', '#ff7a3c', '#7dffb0', '#8f6bff', '#fffaf2', '#b8ff5a', '#ff5fa2')
SWIM = ('#ff2d3c', '#2ee6ff', '#ffc93c', '#ff4fb8', '#2b2350', '#7dffb0', '#ff7a3c', '#8f6bff')
FOOT = 220.0         # how far legs and skirts go down under z = 0 (a long piece stands level on land that follows the road)


def fan_look(r, **fixed):
    """A holidaymaker out to watch: beach clothes, a hot colour, often a hat."""
    d = P.look(r, tops=HOT, bottoms=SWIM, wear=r.choice(('shorts', 'shorts', 'swim', 'trousers')),
               sleeves=r.choice(('none', 'short', 'short')), hair=r.choice(('short', 'short', 'long', 'pony', 'bald')))
    hat = r.choice((None, None, None, None, 'cap'))
    if hat:
        d.update(hat=hat, hat_col=r.choice(HOT))
    d.update(fixed)
    return d


def _flag_hold(col, col2, w=110.0):
    """A big two-colour flag on a pole in the hand (it swings with the arm)."""
    def f(m, h, side):
        m.box(STEEL, (h[0], h[1], h[2] + 70), (5, 5, 170))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + 132), (w, 4, 34))
        m.box(col2, (h[0] + side * w / 2.0, h[1], h[2] + 100), (w, 4, 30))
    return f


def cheer(r, i):
    """What one of the crowd does. Big: the gestures have to carry 100 m. Every third never rests (no react)."""
    k = i % 6
    live = (i % 3 != 1)
    a = dict(react=live, rate=r.uniform(1.25, 1.7), phase=r.random())
    if k == 0:      # hopping, both arms up
        a.update(arm_l='up', arm_r='up', swing_l=22, swing_r=-22, bob=r.uniform(16, 24))
    elif k == 1:    # a big two-armed wave
        a.update(arm_l='vee', arm_r='vee', swing_l=38, swing_r=38, bob=r.uniform(6, 10), rate=r.uniform(0.9, 1.2))
    elif k == 2:    # one arm waving, hopping
        a.update(arm_r='wave', swing_r=44, arm_l='hip', bob=r.uniform(12, 18))
    elif k == 3:    # pointing at the chase, bouncing
        a.update(arm_l='point', arm_r='up', swing_r=30, bob=r.uniform(8, 14))
    elif k == 4:    # filming it, held up over the heads in front
        a.update(arm_l='up', arm_r='up', hold_r=P.hold_phone(), bob=r.uniform(3, 6), rate=r.uniform(0.6, 0.9))
    else:           # rocking from foot to foot, arms out
        a.update(arm_l='vee', arm_r='vee', rock=r.uniform(9, 13), rate=r.uniform(0.8, 1.1))
    return a


def crowd_row(sc, r, n, x0, x1, y, z, s=1.25, detail=0, start=0, flags=(), kid_every=6, cols=(HOTPINK, WHITE), yaw_j=14.0):
    for i in range(n):
        x = x0 + (i + 0.5) * (x1 - x0) / n + r.uniform(-14, 14)
        d = fan_look(r, s=s * r.uniform(0.93, 1.07))
        if kid_every and (i + start) % kid_every == kid_every - 2:
            d.update(build='kid', s=s * r.uniform(0.66, 0.74))
        a = cheer(r, i * 5 + start * 3 + int(r.random() * 6))
        if i in flags:
            a = dict(react=False, rate=r.uniform(0.7, 0.9), phase=r.random(), arm_l='hip', arm_r='up', swing_r=30,
                     hold_r=_flag_hold(cols[0], cols[1]), bob=4)
        d.update(a)
        P.figure(sc, x, y + r.uniform(-8, 8), z=z, yaw=r.uniform(-yaw_j, yaw_j), detail=detail, **d)


def pennant(sc, g, x, y, z0, z1, col, phase=0.0, w=120.0, h=64.0, pole=STEEL):
    """A pole from z0 to z1 and a pennant at its head that swings about the pole."""
    g.box(pole, (x, y, (z0 + z1) / 2.0), (7, 7, z1 - z0))
    fl = sc.part(P.motion(swing=22.0, pivot=(x, y, z1 - h / 2.0), axis=P.UP, rate=1.1, phase=phase))
    fl.box(col, (x + w / 2.0 + 4, y, z1 - h / 2.0), (w, 5, h))
    fl.box(WHITE, (x + w * 0.5 + 4, y, z1 - h / 2.0), (w * 0.34, 6, h * 0.42))


def stripes(g, x0, x1, y, z0, z1, cols, n, t=6.0):
    """A board of upright stripes facing the road."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        g.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y, (z0 + z1) / 2.0), (w, t, z1 - z0))


# ------------------------------------------------------------------------------------------ the grandstand
def stand(seed=101, variant=0):
    """A scaffold grandstand, 14 m long: four tiers from 2.5 m up (over the boardwalk's canopy), a striped canvas skirt, striped back boards with a
    sun (or a wave) on them, pennants. 24 people, a quarter larger than life (they are read from 60 to 150 m)."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Lg, rows, z0, rise, depth = 1400.0, 4, 250.0, 86.0, 116.0
        c1, c2, c3 = ((HOTPINK, WHITE, TEAL) if variant == 0 else (YEL, WHITE, CORAL))
        y_front = -rows * depth / 2.0
        for k in range(rows):                                               # the tiers: a plank deck and a coloured riser
            top = z0 + k * rise
            y = y_front + (k + 0.5) * depth
            g.box(WOOD, (0, y, top - 7), (Lg, depth, 14))
            g.box((c3, c1)[k % 2], (0, y - depth / 2.0 - 2, top - 30), (Lg, 4, 60))
        stripes(g, -Lg / 2.0, Lg / 2.0, y_front - 6, -FOOT, z0 - 60, (c1, c2), 10)                 # the skirt, to the ground and under it
        top_z = z0 + (rows - 1) * rise
        y_back = y_front + rows * depth + 6
        stripes(g, -Lg / 2.0, Lg / 2.0, y_back, top_z, top_z + 250, (c3, c2), 7, t=8.0)            # the back boards
        g.box(c1, (0, y_back, top_z + 262), (Lg + 30, 12, 24))
        if variant == 0:                                                     # a sun on the back boards
            g.cyl(YEL, (0, y_back - 2, top_z + 150), 78, 10, steps=10, rot=(90, 90, 0))
        else:                                                                # a wave of three blocks
            for k in range(3):
                g.box(TEAL, (-150 + k * 150, y_back - 6, top_z + 120 + (40 if k == 1 else 0)), (150, 6, 60))
        for sx in (-1, 1):                                                   # end frames: legs, a rail up the side
            for k in (0, rows):
                yy = y_front + k * depth
                g.box(STEEL, (sx * (Lg / 2.0 + 5), yy, (top_z + 250 - FOOT) / 2.0 if k else (z0 + 100 - FOOT) / 2.0),
                      (10, 10, (top_z + 250 + FOOT) if k else (z0 + 100 + FOOT)))
            tilt_box(g, c1, (sx * (Lg / 2.0 + 5), y_front, z0 + 100), (sx * (Lg / 2.0 + 5), y_back, top_z + 110), 8, 30)
        for k in range(1, 4):                                                # legs under the middle
            g.box(STEEL, (-Lg / 2.0 + k * Lg / 4.0, y_back - 6, (top_z - FOOT) / 2.0), (9, 9, top_z + FOOT))
        for k, x in enumerate((-Lg / 2.0, 0.0, Lg / 2.0)):
            pennant(sc, g, x, y_back + 8, top_z + 250, top_z + 470, (c1, YEL if variant == 0 else HOTPINK, c3)[k], phase=0.3 * k)
        per = ((7, 6, 6, 5) if variant == 0 else (6, 7, 5, 6))
        for k in range(rows):
            top = z0 + k * rise
            y = y_front + (k + 0.55) * depth
            off = 0.0 if k % 2 == 0 else 60.0
            crowd_row(sc, r, per[k], -Lg / 2.0 + 50 + off, Lg / 2.0 - 50 + off - 60, y, top, detail=0, start=k * 2 + variant,
                      flags=((1, 4) if k == rows - 1 else ((3,) if k == 0 and variant else ())), cols=(c1, c2))
    return build


# ------------------------------------------------------------------------------------------ the scaffold platform
def tower(seed=201, variant=0):
    """A scaffold platform 4.3 m up, 4.6 m square, in front of a striped back board: eight people at its rail, a big flag, a
    striped banner hung from the deck."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, W = 430.0, 460.0
        c1, c2, c3 = ((TEAL, WHITE, HOTPINK) if variant == 0 else (CORAL, WHITE, YEL))
        hw = W / 2.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(STEEL, (sx * (hw - 12), sy * (hw - 12), (H + 330 - FOOT) / 2.0), (12, 12, H + 330 + FOOT))
        for sx in (-1, 1):                                                   # braces on the sides and the front
            tilt_box(g, STEEL, (sx * (hw - 12), -hw + 12, 20), (sx * (hw - 12), hw - 12, H - 30), 7, 7)
        tilt_box(g, STEEL, (-hw + 12, -hw + 12, 20), (hw - 12, -hw + 12, 210), 7, 7)
        tilt_box(g, STEEL, (hw - 12, -hw + 12, 210), (-hw + 12, -hw + 12, H - 30), 7, 7)
        g.box(WOOD, (0, 0, H - 9), (W + 30, W + 30, 18))                     # the deck, a step at the back of it
        g.box(WOOD_D, (0, hw - 110, H + 22), (W - 40, 180, 44))
        stripes(g, -hw - 15, hw + 15, -hw - 18, H - 150, H + 40, (c1, c2), 6)                      # the banner hung from the deck
        g.box(c3, (0, -hw - 21, H - 55), (W * 0.5, 4, 44))
        for sx in (-1, 1):
            g.box(c1, (sx * (hw + 17), 0, H + 12), (5, W + 30, 56))
        g.box(WHITE, (0, -hw - 10, H + 104), (W + 30, 8, 8))                 # the rail
        for sx in (-1, 1):
            g.box(WHITE, (sx * (hw + 10), 0, H + 104), (8, W + 30, 8))
        stripes(g, -hw - 15, hw + 15, hw + 2, H + 60, H + 330, (c1, c2), 6, t=8.0)                  # a striped back board: the people stand out against it
        g.box(c3, (0, hw + 2, H + 342), (W + 60, 12, 24))
        g.box(c3, (0, hw - 4, H + 200), (150, 6, 90))
        pennant(sc, g, hw - 12, hw - 12, H + 330, H + 560, c3, w=130.0, h=70.0)
        for k in range(6):                                                   # a ladder up the back
            g.box(STEEL, (0, hw + 16, 40 + k * 70), (60, 6, 6))
        for sx in (-1, 1):
            g.box(STEEL, (sx * 30, hw + 16, H / 2.0), (6, 6, H))
        crowd_row(sc, r, 4, -hw + 20, hw - 20, -hw + 66, H, s=1.25, detail=1, start=variant, flags=(3,) if variant == 0 else (0,), cols=(c3, c2), kid_every=0)
        crowd_row(sc, r, 3, -hw + 40, hw - 40, hw - 120, H + 44, s=1.25, detail=0, start=3 + variant, kid_every=0)
    return build


# ------------------------------------------------------------------------------------------ the beach bar's deck
def _parasol(g, x, y, z, col, r=150.0, h=250.0):
    g.cyl(WHITE, (x, y, z), 5, h, steps=5)
    g.cyl(col, (x, y, z + h - 6), r, 46, steps=8, top_r=10)


def deck(seed=301):
    """A beach bar's deck on stilts, 11 m long and 3 m up: the bar and its thatch at the back, three parasols, and
    every customer at the road-side rail at the rail. 13 people."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, Lg, D = 300.0, 1100.0, 520.0
        g.box(WOOD, (0, 0, H - 10), (Lg, D, 20))
        for k in range(5):
            for y in (-D / 2.0 + 20, D / 2.0 - 20):
                g.box(WOOD_D, (-Lg / 2.0 + 30 + k * (Lg - 60) / 4.0, y, (H - 20 - FOOT) / 2.0), (22, 22, H - 20 + FOOT))
        stripes(g, -Lg / 2.0, Lg / 2.0, -D / 2.0 - 4, H - 90, H + 30, (CORAL, WHITE, TEAL, WHITE), 12)      # a canvas band round the deck's edge
        g.box(WHITE, (0, -D / 2.0 - 4, H + 100), (Lg, 8, 8))
        g.box(WOOD_D, (0, D / 2.0 - 70, H + 55), (Lg * 0.5, 80, 110))                                    # the bar
        g.box(YEL, (0, D / 2.0 - 70, H + 300), (Lg * 0.58, 220, 14), rot=(0, 0, -12))                    # its thatch
        g.box('#e8b26a', (0, D / 2.0 - 60, H + 322), (Lg * 0.5, 150, 16), rot=(0, 0, -12))
        for sx in (-1, 1):
            g.box(WOOD_D, (sx * Lg * 0.27, D / 2.0 - 30, H + 150), (12, 12, 300))
            g.box(WOOD_D, (sx * Lg * 0.27, D / 2.0 - 150, H + 135), (12, 12, 270))
        for k, x in enumerate((-430, 0, 430)):
            _parasol(g, x, 30, H, (HOTPINK, YEL, TEAL)[k], r=170.0, h=290.0)
        tilt_box(g, WOOD, (Lg / 2.0 + 40, D / 2.0 - 60, H - 10), (Lg / 2.0 + 300, D / 2.0 - 60, -FOOT * 0.4), 110, 12)   # the steps down
        pennant(sc, g, -Lg / 2.0 + 20, -D / 2.0 + 20, H, H + 470, HOTPINK, phase=0.0)
        pennant(sc, g, Lg / 2.0 - 20, -D / 2.0 + 20, H, H + 470, TEAL, phase=0.4)
        crowd_row(sc, r, 7, -Lg / 2.0 + 30, Lg / 2.0 - 30, -D / 2.0 + 64, H, s=1.25, detail=1, start=1, flags=(3,), cols=(HOTPINK, YEL))
        g.box(WOOD_D, (0, -D / 2.0 + 190, H + 20), (Lg - 200, 110, 40))                                  # a bench the second row stands on
        crowd_row(sc, r, 6, -Lg / 2.0 + 120, Lg / 2.0 - 120, -D / 2.0 + 190, H + 40, s=1.25, detail=0, start=4)
    return build


# ------------------------------------------------------------------------------------------ the hotel's terrace
def terrace(seed=401):
    """A hotel's sun terrace bridging the mouth of a side street, 14 m long: it stands on four corner piers only (the
    street, 10 m with its kerbs, runs between them), the lower floor 6 m up (over the promenade's clipped trees, which
    stand between it and the road) and stepped 1.7 m out in front of the upper one, so that both rows of guests stand
    in the open at their rails; a striped valance under each floor, a striped canopy over the doors at the back."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Lg, D, F1, F2, TOP, OUT = 1400.0, 440.0, 600.0, 940.0, 1250.0, 170.0
        for sx in (-1, 1):                                                   # the piers: none in the street
            x = sx * (Lg / 2.0 - 40)
            g.box(WHITE, (x, -D / 2.0 + 30, (F1 - FOOT) / 2.0), (56, 56, F1 + FOOT))
            g.box(WHITE, (x, D / 2.0 - 30, (TOP - FOOT) / 2.0), (56, 56, TOP + FOOT))
            tilt_box(g, WHITE, (x, -D / 2.0 + 30, F1 - 190), (x, -D / 2.0 - OUT + 20, F1 - 30), 30, 30)   # a bracket under the lower balcony
        g.box(WHITE, (0, -D / 2.0 + 30, F1 - 62), (Lg, 50, 70))             # the beams that span the street
        g.box(WHITE, (0, D / 2.0 - 30, F1 - 62), (Lg, 50, 70))
        for z, out, cols in ((F1, OUT, (HOTPINK, WHITE)), (F2, 0.0, (TEAL, WHITE))):   # the floors, a valance under each, a rail on each
            yf = -D / 2.0 - out
            g.box(PEACH, (0, -out / 2.0, z - 14), (Lg, D + out, 28))
            stripes(g, -Lg / 2.0, Lg / 2.0, yf - 4, z - 96, z - 2, cols, 14)
            g.box(WHITE, (0, yf - 2, z + 100), (Lg, 8, 8))
            for k in range(8):
                g.box(WHITE, (-Lg / 2.0 + 10 + k * (Lg - 20) / 7.0, yf - 2, z + 50), (8, 8, 100))
        g.box(PEACH, (0, D / 2.0 - 12, (F1 + TOP) / 2.0), (Lg, 24, TOP - F1))    # the back wall, doors in it
        for z in (F1, F2):
            for k in range(4):
                g.box(NAVY, (-Lg / 2.0 + 200 + k * (Lg - 400) / 3.0, D / 2.0 - 26, z + 115), (130, 6, 230))
        for k in range(10):                                                  # the canopy, over the doors only
            w = Lg / 10.0
            g.box((YEL, WHITE)[k % 2], (-Lg / 2.0 + (k + 0.5) * w, D / 4.0 + 20, TOP + 6), (w, D / 2.0, 10), rot=(0, 0, -7))
        g.box(HOTPINK, (0, 18, TOP - 14), (Lg, 6, 30))
        for k, x in enumerate((-Lg / 2.0 + 20, 0.0, Lg / 2.0 - 20)):
            pennant(sc, g, x, D / 2.0 - 30, TOP, TOP + 260, (HOTPINK, TEAL, YEL)[k], phase=0.3 * k, w=140.0, h=76.0)
        crowd_row(sc, r, 8, -Lg / 2.0 + 40, Lg / 2.0 - 40, -D / 2.0 - OUT + 66, F1, s=1.25, detail=1, start=0, flags=(2,), cols=(HOTPINK, WHITE))
        crowd_row(sc, r, 8, -Lg / 2.0 + 40, Lg / 2.0 - 40, -D / 2.0 + 66, F2, s=1.25, detail=0, start=3, flags=(5,), cols=(TEAL, YEL))
    return build


JOBS = [
    ('SM_WatchStand_A', stand(101, 0)),
    ('SM_WatchStand_B', stand(102, 1)),
    ('SM_WatchTower_A', tower(201, 0)),
    ('SM_WatchTower_B', tower(202, 1)),
    ('SM_WatchDeck', deck(301)),
    ('SM_WatchTerrace', terrace(401)),
]
FIRST = ['SM_WatchStand_A', 'SM_WatchTower_A']


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
