"""FULL THROTTLE (Unreal) the population of level 5, Ice Mountain (/Game/People/IceMountain/).

The cast of a ski resort on a film day: racers coming down their own big-air hills and down slalom courses on the
real slopes, snowboarders swinging from wall to wall of a small pipe, crowds on scaffold stands in ski suits of every
hot colour, an apres-ski deck, climbers on a blue ice face, snowmen and the children building them, a dog sled, ibex
on a crag, wolves along a ridge, a roped party crossing the glacier, eagles wheeling, and the film crew's snowmobile
camera. Placed by Source/FullThrottle/FTPeopleL5.inl.

What this kit found out about the figure builder's movement (ft_people.M_People), and uses everywhere:

  DOWNHILL. `travel` moves a part along the mesh's X only, and `bob` and `swing` are sines: nothing in the master
  goes steadily DOWN. But a part's swing and its travel run off the same clock, so with the swing's rate set to the
  travel's own rounds a second (rate = speed / length) the swing's angle is a fixed function of where the part is on
  its line. A swing of a degree or two about a pivot 120 to 600 m under the ground is a straight slide (the crew's
  dolly does that), and the pivot's place gives the slide's direction: so a slider's height is A sin(2 pi (f + c))
  and its place along the line (f - 0.5) L - S sin(2 pi (f + c)), f being its fraction of the round. With c = -0.15
  and S = 0.15 L it comes DOWN, steadily, over 80 % of the line's length (f 0.4 to 0.9), runs out over a small rise,
  wraps, and climbs back unseen through the last fifth, which lies under the top of the hill (the master shrinks a
  part to its pivot as it wraps, and the pivot is deep underground). `run_point` is that curve: the hill is drawn
  from it, so skis stay on the snow all the way down. Negative speed and rate give the mirror image (d = -1), for
  the left of the road, where the table turns a piece round.

  A HALF PIPE. A swing of 70 degrees about a pivot over the pipe's axis is a pendulum: the rider goes up one wall,
  tilting with it, hangs at the lip and comes back.

Rules kept here: nothing glows; nothing blinks; no words or marks; nothing big, white and furry on two legs (the
level's boss is THE ABOMINABLE: the snowmen are small, hatted, scarfed and carrot-nosed, and stand 40 m out and more).

Run in the editor:  import ft_people_ice; ft_people_ice.run()        (or run(['SM_SkiHill_A']))
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/IceMountain'

SNOW, SNOW_SH, ICE, ICE_D, ICE_DEEP = '#f4f9ff', '#cdd6f1', '#bfe3ff', '#7fb0e0', '#4f8fd2'
ROCK, ROCK_D = '#6a7488', '#5a6478'
WOOD, WOOD_D, STEEL, STEEL_D = '#9a6a44', '#6a4c36', '#9aa4b8', '#5a6478'
RED, BLUE, ORANGE, PINK, TEAL, YEL = '#ff2d3c', '#2e6bff', '#ff8a2a', '#ff4fb8', '#19b5c9', '#ffd23f'
DARK, WHITE = '#22202a', '#fff6ea'
PINE, PINE_D = '#2f6b52', '#234f40'
SUITS = (RED, '#ff5fa2', ORANGE, YEL, '#b8ff5a', '#2ee6ff', BLUE, '#8f6bff', PINK, '#7dffb0')
LEGS = ('#3a5fa8', WHITE, '#8f6bff', '#19b5c9')
BIG = 1.3           # the size of people in this kit (the cel pass inks a 1.7 m figure 30 m off to a black scribble)
PZ = 12000.0        # how far under a slider its swing's pivot lies (near pieces)


def suit(r, **fixed):
    """Somebody in a ski suit: a hot colour head to foot or over dark trousers, a helmet or a bobble hat."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear='trousers', top=c, bottom=r.choice((c, c) + LEGS), hat=r.choice(('cap', 'beanie', 'beanie')),
               hat_col=r.choice(SUITS), shoes=c, hair='short', s=BIG * r.uniform(0.94, 1.06))
    d.update(fixed)
    return d


def coat(r, **fixed):
    """Somebody watching: a padded coat, a bobble hat."""
    d = P.look(r, sleeves='long', wear='coat', tops=SUITS, bottoms=LEGS, hat=r.choice(('beanie', 'beanie', 'cap', None)), hat_col=r.choice(SUITS),
               shoes=WHITE, hair=r.choice(('short', 'long', 'bun')), s=BIG * r.uniform(0.92, 1.06))
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ downhill
def run_point(f, L, S, A, c=-0.15, d=1):
    """Where a slider is at fraction f of its round: (x along the mesh, height over the run's middle). See the header."""
    a = 2.0 * math.pi * (f + c * d)
    return ((f - 0.5) * L - S * math.sin(a), A * d * math.sin(a))


def run_curve(L, S, A, c=-0.15, d=1, n=14):
    """The run from its top to the end of its run-out, in the order a slider meets it."""
    fs = [0.4 + 0.6 * i / float(n) for i in range(n + 1)]
    return [run_point(f if d > 0 else 1.0 - f, L, S, A, c, d) for f in fs]


def slider(sc, r, bx, by, zb, L, v, S, A, c=-0.15, d=1, kind='ski', pz=PZ, detail=1, s=None, **lookfix):
    """A skier or a snowboarder who comes down the run and round again. bx: its place in the queue (anywhere in
    -L/2 .. L/2: spread a run's sliders evenly); by: its lane; zb: the height of the run's middle in the mesh;
    v: cm a second along the line; d: +1 down towards +X, -1 towards -X."""
    th = -S / pz                                   # radians: the slide along the line
    px = -(A * d) / th
    piv = (bx - px, by, zb - pz)
    sway = dict(swing=math.degrees(th), pivot=piv, axis=90.0)
    rate = d * v / L
    ph = (bx / L + 0.5 + c * d) % 1.0
    look = suit(r, **lookfix)
    if s is not None:
        look['s'] = s
    s = look['s']
    board = kind == 'board'
    yaw = 90.0 * d - (90.0 if board else 0.0)
    arms = dict(arm_l='out', arm_r='out') if board else dict(arm_l=((35, -30), (60, 20)), arm_r=((35, -30), (60, 20)))
    P.figure(sc, bx, by, zb, yaw=yaw, legs='wide', detail=detail, rate=rate, phase=ph, sway=sway, travel=(L, d * v), **dict(look, **arms))
    m = sc.part(P.motion(phase=ph, rate=rate, base=(bx, by), travel=(L, rate), **sway), (bx, by, zb), yaw, s)
    col = r.choice(SUITS)
    if board:
        m.box(col, (0, -2, 3), (158, 30, 6))
        m.box(DARK, (0, -2, 7), (60, 10, 3))
    else:
        for sx in (-1, 1):
            m.box(col, (sx * 17, -14, 3), (10, 170, 5))
            tilt_box(m, DARK, (sx * 40, 14, 86), (sx * 44, 78, 30), 4, 4)
    return m


def ski_hill(d=1, seed=1):
    """A big-air hill of the resort's own: racers and boarders burst out of the start hut on its top, come down 8 m
    and run out. 34 m long, 7 m deep, 11 m to the hut's roof: TALL. d = 1 for the right of the road (down towards
    +X, the way the mountain falls), d = -1 its twin for the left."""
    def build(sc):
        r = random.Random(seed)
        L, S, A, v = 3200.0, 480.0, 400.0, 620.0
        g = sc.still()
        pts = [(x, z + A) for x, z in run_curve(L, S, A, d=d, n=14)]
        top = pts[0]
        back = top[0] - d * 520.0
        poly = [(back, -160.0), (back, top[1])] + pts + [(pts[-1][0] + d * 70.0, -160.0)]
        g.side(SNOW, poly, -370, 370)
        # the piste's edges: a run of netting in the resort's colours down both sides
        for k in range(len(pts) - 1):
            a, b = pts[k], pts[k + 1]
            for sy in (-1, 1):
                tilt_box(g, (ORANGE, PINK)[k % 2], (a[0], sy * 362, a[1] + 22), (b[0], sy * 362, b[1] + 22), 8, 44)
        # banners on the hill's road face: stripes and blocks, no words
        for k, (dx, w, hh, col) in enumerate(((-260, 420, 300, TEAL), (160, 300, 240, YEL), (520, 300, 170, PINK))):
            g.box(col, (top[0] + d * dx, -373, top[1] - 80 - hh / 2.0 - k * 110), (w, 8, hh))
            g.box(WHITE, (top[0] + d * dx, -378, top[1] - 80 - hh / 2.0 - k * 110), (w * 0.7, 4, hh * 0.22))
        # the start hut: everything that climbs back up inside the hill comes out of the snow under it
        for k in range(1, len(pts) - 1, 2):            # groomed bands across the piste
            a, b = pts[k], pts[k + 1]
            tilt_box(g, SNOW_SH, (a[0], 0, a[1] + 1), (b[0], 0, b[1] + 1), 700, 6)
        hx = top[0] - d * 250.0
        g.box(WOOD, (hx, 0, top[1] + 175), (440, 600, 350))
        g.box(DARK, (hx + d * 221, 0, top[1] + 150), (4, 420, 290))
        g.box(SNOW, (hx, 0, top[1] + 372), (500, 660, 44))
        for k in range(6):
            g.box(RED if k % 2 else WHITE, (hx + d * 250, -250 + k * 100, top[1] + 340), (70, 100, 10), rot=(-16 * d, 0, 0))
        for sy in (-1, 1):
            g.box(STEEL_D, (hx, sy * 280, top[1] + 480), (8, 8, 280))
            fl = sc.part(P.motion(swing=16.0, pivot=(hx, sy * 280, top[1] + 560), axis=P.UP, rate=0.9, phase=0.3 * sy))
            fl.box((PINK, TEAL)[sy > 0], (hx + d * 70, sy * 280, top[1] + 570), (140, 5, 80))
        n = 6
        lanes = [-240, 120, -60, 240, -150, 30]
        for i in range(n):
            bx = -L / 2.0 + (i + 0.5) * L / n
            slider(sc, r, bx, lanes[i], A, L, v, S, A, d=d, kind='board' if i % 3 == 1 else 'ski', detail=0, s=1.5)
    return build


def piste_run(seed=2, L=12000.0, slope=0.12, n=7):
    """A slalom course on the mountain's own slope, for 150 to 230 m out: racers come straight down the fall line
    between red and blue gates, out of a start gate, and stop by a little crowd at the finish. Laid with its +X DOWN
    the slope (the table places it by a set, turned -90), and always moving (far=True).
    It is seen from 110 to 230 m, so everything is drawn for that distance (second round: nothing of the first
    version could be made out from the road): racers 5.8 m tall in one hot colour head to foot, gate panels 4 m
    wide, orange netting down both edges of the piste (a line 100 m long on the white slope is what says "a piste"),
    a striped start house and a striped finish band 14 m wide."""
    def build(sc):
        r = random.Random(seed)
        S, A = 0.15 * L, slope * 0.4 * L
        zb = -slope * 0.15 * L
        v = 1700.0
        g = sc.still()

        def land(x):
            return -slope * x

        xt, xe = -0.25 * L, 0.55 * L
        # the piste's edges: netting, orange and pink by turns, 1.6 m high
        for k in range(8):
            xa, xb = xt + k * (xe - xt) / 8.0, xt + (k + 1) * (xe - xt) / 8.0
            for sy in (-1, 1):
                tilt_box(g, (ORANGE, PINK)[k % 2], (xa, sy * 700, land(xa) + 80), (xb, sy * 700, land(xb) + 80), 14, 160)
        # the gates: a flag panel 4 m wide on two poles, red and blue by turns
        for k in range(7):
            x = xt + 700 + k * (xe - xt - 1400) / 6.0
            y = (-1 if k % 2 else 1) * 330
            col = RED if k % 2 else BLUE
            for dy in (-200, 200):
                g.box(col, (x, y + dy, land(x) + 200), (16, 16, 420))
            g.box(col, (x, y, land(x) + 330), (16, 400, 180))
        # the start: a house in stripes on a snow mound, an arch over the gate
        g.sphere(SNOW, (xt - 520, 0, land(xt) + 60), 520, scale=(1.0, 1.5, 0.75), steps=(4, 8))
        for k in range(6):
            g.box((TEAL, WHITE)[k % 2], (xt - 300, -625 + k * 250, land(xt) + 420), (300, 250, 520))
        g.box(PINK, (xt - 300, 0, land(xt) + 720), (360, 1560, 90))
        # the finish: a striped band 14 m wide on two posts, and the people waiting under it
        for sy in (-1, 1):
            g.box(YEL, (xe + 160, sy * 720, land(xe) + 300), (70, 70, 700))
        for k in range(7):
            g.box((RED, WHITE)[k % 2], (xe + 160, -600 + k * 200, land(xe) + 580), (60, 200, 200))
        for i in range(6):
            sy = -1 if i % 2 else 1
            P.figure(sc, xe - 300 + (i // 2) * 260, sy * (860 + 90 * (i % 3)), land(xe) - 10, yaw=90 if sy < 0 else -90, detail=0, arm_l='up', arm_r='up',
                     swing_l=14, swing_r=-14, bob=40, rate=1.3, phase=r.random(), **dict(coat(r), s=3.2))
        for i in range(n):
            bx = -L / 2.0 + (i + 0.5) * L / n
            c = SUITS[(i * 3 + 1) % len(SUITS)]
            slider(sc, r, bx, (-220, 180, 0, -90, 250, 60, -280)[i % 7], zb, L, v, S, A, d=1, pz=60000.0, detail=0, s=3.4,
                   kind='board' if i % 4 == 2 else 'ski', top=c, bottom=c, shoes=c, hat_col=c)
    return build


# ------------------------------------------------------------------------------------------ the pipe
def board_pipe(seed=3):
    """A small half pipe end-on to the road's own direction: three boarders swing from wall to wall, friends wait on
    the lips. 10 m long, 12.4 m across, 4.4 m to the lip: TALL."""
    def build(sc):
        r = random.Random(seed)
        R, z0, half = 450.0, 60.0, 500.0
        top = z0 + R - R * math.cos(math.radians(80))
        g = sc.still(yaw=90.0)          # this model's x runs across the pipe (the mesh's +Y), its y back along the mesh's X
        arc = [(R * math.sin(math.radians(a)), z0 + R - R * math.cos(math.radians(a))) for a in range(-80, 81, 16)]
        for k in range(len(arc) - 1):          # the snow under the curve, a slice a segment (each one convex)
            (x1, z1), (x2, z2) = arc[k], arc[k + 1]
            g.side(SNOW, [(x1, -120), (x2, -120), (x2, z2), (x1, z1)], -half, half)
        for sx in (-1, 1):
            g.box(SNOW, (sx * (arc[-1][0] + 640) / 2.0, 0, (top - 120) / 2.0), (640 - arc[-1][0], 2 * half, top + 120))
        g = sc.still()
        for sy in (-1, 1):
            g.box(TEAL if sy < 0 else PINK, (0, sy * (arc[-1][0] + 8), top + 6), (2 * half, 26, 14))       # the coping
            for k in range(5):
                g.box((YEL, PINK, TEAL, WHITE, ORANGE)[k], (-half + 100 + k * 200, sy * 646, top - 150), (200, 8, 200))
            for ex in (-1, 1):
                g.box(STEEL_D, (ex * (half - 30), sy * 600, top + 170), (8, 8, 340))
                fl = sc.part(P.motion(swing=18.0, pivot=(ex * (half - 30), sy * 600, top + 300), axis=P.UP, rate=0.8, phase=0.25 * ex + 0.1 * sy))
                fl.box((RED, BLUE)[ex > 0], (ex * (half - 30) + 60, sy * 600, top + 300), (120, 5, 70))
        # the riders: pendulums about the pipe's axis
        for i, x in enumerate((-300, 0, 300)):
            look = suit(r, s=1.4)
            sway = dict(swing=72.0, pivot=(x, 0.0, z0 + R), axis=0.0)
            ph, rate = i / 3.0 + 0.1, 0.3
            P.figure(sc, x, 0, z0 + 8, yaw=90.0, legs='wide', arm_l='out', arm_r='vee', rate=rate, phase=ph, sway=sway, **look)
            m = sc.part(P.motion(phase=ph, rate=rate, base=(x, 0), **sway), (x, 0, z0 + 8), 90.0, look['s'])
            m.box(r.choice(SUITS), (0, -2, -3), (158, 30, 6))
        # friends on the lips, boards under their arms
        for i, (x, sy) in enumerate(((-380, -1), (-120, -1), (260, -1), (-250, 1), (120, 1), (400, 1))):
            act = (dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=9, rate=1.4), dict(arm_r='wave', swing_r=24, rate=1.2, bob=4),
                   dict(arm_l='down', arm_r='down', hold_r=P.hold_board(col=r.choice(SUITS), stripe=WHITE, length=150.0), rate=0.4, bob=-1.5))[i % 3]
            P.figure(sc, x, sy * 560, top, yaw=0 if sy > 0 else 180, detail=0, phase=r.random(), **dict(suit(r), **act))
    return build


# ------------------------------------------------------------------------------------------ the crowd
def _scaffold(g, length, tiers, depth=110.0, rise=75.0, first=115.0, skirt=(PINK, WHITE, TEAL, YEL)):
    """A scaffold stand: `tiers` decks stepping up and back, a striped skirt along the front, rails at the ends."""
    n = max(int(length / 150.0), 2)
    for k in range(n):
        g.box(skirt[k % len(skirt)], (-length / 2.0 + (k + 0.5) * length / n, -depth / 2.0 - 4, first / 2.0 - 20), (length / n, 8, first + 40))
    for t in range(tiers):
        z = first + t * rise
        g.box(WOOD, (0, t * depth, z - 6), (length, depth, 12))
        if t:
            g.box(STEEL_D, (0, t * depth - depth / 2.0, z - rise / 2.0), (length, 6, rise))
    for ex in (-1, 1):                     # the ends: steps of colour, never a wall (the road sees a stand end-on first)
        for t in range(tiers):
            g.box(skirt[(t + 1) % len(skirt)], (ex * length / 2.0, t * depth, (first + t * rise) / 2.0 - 20), (10, depth, first + t * rise + 40))
    g.box(skirt[0], (0, (tiers - 1) * depth + depth / 2.0, (first + (tiers - 1) * rise) / 2.0), (length, 6, first + (tiers - 1) * rise + 100))


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=12, rate=1.5),
    dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=14, rate=1.3),
    dict(arm_r='wave', swing_r=26, rate=1.2, bob=5),
    dict(arm_l='wave', swing_l=26, rate=1.0, bob=8),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4, bob=3),
    dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7, bob=2),
)


def stand(seed=4, flags=(RED, YEL), skirt=(PINK, WHITE, TEAL, YEL)):
    """A scaffold stand of people in ski suits and padded coats, cheering the chase: three rows, eleven people, 1.45
    times life size and well apart (a packed stand inks to one black mass), flags. 10 m long, 6 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        length, tiers = 1000.0, 3
        _scaffold(g, length, tiers, depth=130.0, rise=95.0, skirt=skirt)
        k = 0
        for t in range(tiers):
            n = 4 if t != 1 else 3
            for i in range(n):
                x = -length / 2.0 + (i + 0.5) * length / n + r.uniform(-14, 14)
                d = (suit if (i + t) % 3 else coat)(r)
                d['s'] *= 1.12
                d.update(CHEER[(k * 5 + seed) % len(CHEER)])
                if k % 4 == 2:
                    d.update(arm_r='wave', swing_r=22, hold_r=P.hold_flag(col=flags[k % 2], w=110.0), rate=1.0, bob=4)
                if i == 2 and t == 0:
                    d.update(build='kid', s=1.0)
                P.figure(sc, x, t * 130.0 + r.uniform(-10, 10), 115.0 + t * 95.0, yaw=r.uniform(-12, 12), detail=0, phase=r.random(), react=True, **d)
                k += 1
    return build


# ------------------------------------------------------------------------------------------ the resort
def lodge_deck(seed=5):
    """An apres-ski deck on posts: parasols, a bar, eight people dancing in ski boots. 9 m long, 5.6 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z = 150.0
        g.box(WOOD, (0, 0, Z - 10), (900, 460, 20))
        g.box(WOOD_D, (0, -232, Z / 2.0 - 30), (900, 8, Z + 60))
        for k in range(6):
            g.box((TEAL, WHITE, PINK, WHITE, YEL, WHITE)[k], (-375 + k * 150, -238, Z - 40), (150, 6, 90))
        for ex in (-1, 1):
            g.box(WOOD_D, (ex * 446, 0, Z / 2.0 - 30), (8, 460, Z + 60))
        g.box(WOOD_D, (0, 232, Z / 2.0 - 30), (900, 8, Z + 60))
        g.box(WOOD_D, (300, 150, Z + 55), (220, 80, 110))                    # the bar
        g.box(RED, (300, 108, Z + 80), (220, 6, 40))
        for k, x in enumerate((-300, 0, 300)):
            y = 60 if k != 1 else -40
            g.box(STEEL_D, (x, y, Z + 150), (8, 8, 300))
            sh = sc.part(P.motion(swing=3.0, pivot=(x, y, Z + 290), axis=0.0, rate=0.35, phase=0.3 * k))
            sh.cyl((RED, YEL, TEAL)[k], (x, y, Z + 290), 190, 70, steps=8, top_r=12)
            sh.cyl(WHITE, (x, y, Z + 284), 196, 8, steps=8)
        dance = (dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=10, rate=1.4), dict(arm_l='out', arm_r='out', turn=28, bob=6, rate=0.9),
                 dict(arm_l='vee', arm_r='hip', rock=9, bob=5, rate=1.1), dict(arm_r='wave', swing_r=26, bob=6, rate=1.2),
                 dict(arm_r='face', arm_l='hip', hold_r=P.hold_cup(col=YEL), rock=5, rate=0.6))
        for i, (x, y) in enumerate(((-380, -150), (-230, -40), (-120, -160), (20, -150), (150, -60), (250, -170), (390, -120), (-330, 110))):
            d = (coat if i % 2 else suit)(r)
            d.update(dance[i % len(dance)])
            P.figure(sc, x, y, Z, yaw=r.uniform(-25, 25), detail=0, phase=r.random(), react=(i % 2 == 0), **d)
    return build


def snowmen(seed=6):
    """Children building snowmen on a mound: two snowmen (small, hatted, scarfed, carrot-nosed), four children
    hopping round them, a red sledge. 9 m across, 3.6 m: TALL, and 40 m out or more (nothing white on two legs near
    the road)."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.sphere(SNOW, (0, 0, -30), 100, scale=(4.6, 3.2, 1.4), steps=(4, 8))
        Z = 100.0

        def man(x, y, s, hat, scarf):
            g.sphere(SNOW, (x, y, Z + 55 * s), 68 * s, steps=(4, 7))
            g.sphere(SNOW_SH, (x, y, Z + 140 * s), 50 * s, steps=(4, 7))
            g.sphere(SNOW, (x, y, Z + 205 * s), 36 * s, steps=(4, 7))
            g.cyl(hat, (x, y, Z + 232 * s), 30 * s, 34 * s, steps=7)
            g.cyl(hat, (x, y, Z + 228 * s), 46 * s, 6 * s, steps=7)
            g.cyl(scarf, (x, y, Z + 172 * s), 44 * s, 14 * s, steps=7)
            g.box(scarf, (x + 30 * s, y - 36 * s, Z + 150 * s), (16 * s, 8 * s, 60 * s))
            g.cyl(ORANGE, (x, y - 34 * s, Z + 205 * s), 7 * s, 40 * s, rot=(0, 0, 90), steps=5, top_r=1)
            for sx in (-1, 1):
                tilt_box(g, WOOD_D, (x + sx * 44 * s, y, Z + 150 * s), (x + sx * 110 * s, y - 10, Z + 200 * s), 6, 6)
        man(-150, 20, 1.15, DARK, RED)
        man(170, 60, 0.9, BLUE, YEL)
        kids = (dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=16, rate=1.5), dict(arm_r='up', arm_l='hip', swing_r=12, axis_r='x', bob=6, rate=0.8),
                dict(arm_l='vee', arm_r='vee', bob=20, rate=1.3, turn=20), dict(arm_r='wave', swing_r=26, bob=8, rate=1.1))
        for i, (x, y, yaw) in enumerate(((-300, -70, -50), (-20, -90, 30), (300, -40, 40), (60, 150, 170))):
            d = suit(r, build='kid', s=r.uniform(0.82, 0.95), hat='beanie')
            d.update(kids[i])
            P.figure(sc, x, y, Z - 12, yaw=yaw, detail=1, phase=r.random(), **d)
        g.box(RED, (360, -150, Z - 30), (120, 46, 10))
        for sy in (-1, 1):
            g.box(WOOD_D, (360, -150 + sy * 18, Z - 42), (130, 6, 12))
    return build


def ice_climb(seed=7):
    """A blue ice face with three climbers on it in hot colours, each on a red rope, hammering and inching up, and
    one holding the ropes at the foot. 8 m wide, 11 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(ICE, (0, 120, 480), (760, 420, 1160))
        g.box(ICE_D, (-240, 60, 380), (300, 420, 960), rot=(0, 8, 0))
        g.box(ICE_DEEP, (250, 100, 300), (280, 440, 800), rot=(0, -10, 0))
        g.box(ICE_D, (60, -60, 160), (360, 200, 520))
        g.sphere(SNOW, (0, 120, 1060), 100, scale=(4.2, 2.4, 0.7), steps=(4, 8))
        g.sphere(SNOW, (0, -120, -10), 100, scale=(5.0, 2.2, 0.6), steps=(4, 8))
        for i, (x, y, z) in enumerate(((-230, -190, 520), (60, -200, 300), (250, -160, 640))):
            g.box(RED, (x + 24, y + 22, (z + 1060) / 2.0 + 60), (7, 7, 1060 - z))
            d = suit(r, s=1.45, pack=r.choice(SUITS))
            P.figure(sc, x, y, z, yaw=180, detail=1, arm_l='up', arm_r='vee', swing_r=18, swing_l=-10, axis_r='x', axis_l='x', legs='wide',
                     bob=-34, rate=0.22 + 0.04 * i, phase=r.random(), hold_r=P.hold_tool(col=STEEL, grip=YEL, length=30.0), **d)
        P.figure(sc, -40, -330, 30, yaw=170, detail=1, arm_l='hold', arm_r='up', swing_r=8, axis_r='x', rate=0.4, bob=-2, **suit(r))
        P.figure(sc, 200, -350, 30, yaw=20, detail=1, arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.6, react=True, **coat(r))
    return build


def ice_climb_b(seed=27):
    """The climb's second face (second round: the one face came by four times in 25 s): a frozen fall in three steps
    with a free-standing ice pillar beside it, a pair of climbers on the pillar on orange ropes, one abseiling the
    fall, a party of three at the foot round a flag. 9 m wide, 12 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for k, (w, h, y, col) in enumerate(((560, 1180, 200, ICE), (500, 860, 110, ICE_D), (440, 520, 20, ICE))):      # the fall, stepping down to the road
            g.box(col, (160, y, h / 2.0 - 20), (w, 300, h + 40), rot=(0, (4, -6, 5)[k], 0))
            g.sphere(SNOW, (160, y - 20, h), 100, scale=(w / 190.0, 1.9, 0.5), steps=(4, 8))
        for k in range(5):                                                                                     # icicles down the top step
            g.cyl(ICE_DEEP, (-40 + k * 100, 40, 860), 26, 300, steps=5, top_r=44)
        g.cyl(ICE_DEEP, (-300, 40, -30), 150, 1080, steps=6, top_r=95)                                           # the pillar
        g.cyl(ICE, (-300, 40, 1040), 120, 60, steps=6, top_r=60)
        g.sphere(SNOW, (0, -140, -10), 100, scale=(5.4, 2.4, 0.6), steps=(4, 8))
        for i, (x, y, z, rope) in enumerate(((-300, -170, 560, ORANGE), (-290, -150, 230, ORANGE), (190, -150, 600, YEL))):
            g.box(rope, (x + 22, y + 24, (z + 1100) / 2.0 + 60), (7, 7, 1100 - z))
            d = suit(r, s=1.45, pack=r.choice(SUITS))
            if i < 2:
                P.figure(sc, x, y, z, yaw=180, detail=1, arm_l='vee', arm_r='up', swing_r=-16, swing_l=12, axis_r='x', axis_l='x', legs='wide',
                         bob=-30, rate=0.2 + 0.05 * i, phase=r.random(), hold_r=P.hold_tool(col=STEEL, grip=PINK, length=30.0), **d)
            else:                                                                                              # abseiling: a long slow float down and up
                P.figure(sc, x, y, z, yaw=180, detail=1, arm_l='hold', arm_r='hold', legs='wide', bob=-150, rate=0.11, phase=r.random(), **d)
        fx = 380.0
        g.box(STEEL_D, (fx, -300, 190), (8, 8, 380))
        fl = sc.part(P.motion(swing=16.0, pivot=(fx, -300, 330), axis=P.UP, rate=0.9, phase=0.4))
        fl.box(PINK, (fx + 70, -300, 330), (140, 5, 80))
        P.figure(sc, 250, -340, 30, yaw=150, detail=1, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=10, rate=1.4, react=True, phase=r.random(), **coat(r))
        P.figure(sc, 60, -350, 30, yaw=175, detail=1, arm_l='hold', arm_r='up', swing_r=8, axis_r='x', rate=0.4, bob=-2, **suit(r))
        P.figure(sc, -120, -330, 30, yaw=-10, detail=1, arm_r='wave', swing_r=24, rate=1.1, bob=5, react=True, phase=r.random(), **coat(r))
    return build


# ------------------------------------------------------------------------------------------ creatures and far life
def _berm(sc, length, h, w=520.0, col=SNOW):
    """A snow ridge along X with a level crest, footed 3 m into the ground (the hill falls 7 in 100 along the road)."""
    g = sc.still(yaw=90.0)
    g.side(col, [(-w / 2.0, -300), (w / 2.0, -300), (w / 4.0, h), (-w / 4.0, h)], -length / 2.0, length / 2.0)
    return sc.still()


def dog_sled(seed=8):
    """A dog sled along a snow bank: six huskies in pairs, the sled, its driver. They run the bank's 34 m and come
    round again. 3.7 m to the driver's head: TALL."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 3400.0, 130.0, 520.0
        g = _berm(sc, L, H, w=560.0)
        for k in range(9):
            g.box((RED, BLUE)[k % 2], (-L / 2.0 + 200 + k * (L - 400) / 8.0, 150, H + 45), (10, 10, 110))
        tv = (L, v / L)
        for i in range(6):
            bx, by = 420 - (i // 2) * 210, (-1 if i % 2 else 1) * 42
            P.quadruped(sc, bx, by, H, yaw=90, s=1.7, length=74, height=50, girth=22, neck=16, neck_up=35, head=20, tail=24, stride=30,
                        body=('#6a7488', '#3b3f52', '#9aa4b8')[i % 3], belly=WHITE, snout=WHITE, ears=DARK, rate=2.4, bob=6, phase=r.random(), travel=(L, v))
        ln = sc.part(P.motion(pivot=(110, 0, H + 60), base=(110, 0), travel=tv))
        ln.box(RED, (110, 0, H + 62), (640, 6, 6))
        bx = -330.0
        sl = sc.part(P.motion(pivot=(bx, 0, H), base=(bx, 0), travel=tv))
        for sy in (-1, 1):
            sl.box(WOOD_D, (bx, sy * 36, H + 6), (260, 8, 12))
            sl.box(WOOD_D, (bx - 110, sy * 36, H + 70), (8, 8, 130))
        sl.box(YEL, (bx + 20, 0, H + 34), (200, 70, 44))
        sl.box(RED, (bx + 30, 0, H + 62), (150, 60, 16))
        sl.box(WOOD_D, (bx - 110, 0, H + 134), (10, 84, 10))
        P.figure(sc, bx - 150, 0, H + 10, yaw=90, arm_l='push', arm_r='push', detail=1, rate=1.0, bob=3, phase=0.2, travel=(L, v), **coat(r, s=1.4, hat='beanie'))
    return build


def ibex(seed=9):
    """Ibex on a crag: five of them, great horns, one hopping from ledge to ledge, two grazing. 9 m across, 8 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for x, y, h, r0, r1, col, tw in ((-170, 40, 500, 290, 150, ROCK, 12), (190, 20, 330, 270, 160, ROCK_D, 40), (40, 160, 670, 250, 120, ROCK, 70)):
            g.cyl(col, (x, y, -60), r0, h + 60, rot=(0, tw, 0), steps=5, top_r=r1)        # three leaning stacks of rock
            g.cyl(SNOW, (x, y, h - 2), r1 + 14, 26, rot=(0, tw, 0), steps=5, top_r=r1 * 0.7)
        g.cyl(ROCK_D, (-330, 150, -40), 200, 240, rot=(0, 25, 0), steps=5, top_r=90)
        g.sphere(SNOW, (0, 0, -20), 100, scale=(5.0, 3.6, 0.7), steps=(4, 8))
        coat_ = ('#8e5a3a', '#b87a50', '#6a4c36')
        for i, (x, y, z, yaw, kw) in enumerate(((-170, 30, 500, 60, dict(bob=26, rate=0.55)), (190, 10, 330, -70, dict(graze=22, rate=0.4)),
                                                 (40, 150, 670, 20, dict(graze=10, rate=0.3, wag=20)), (-380, -150, 20, 100, dict(graze=24, rate=0.5)),
                                                 (330, -170, 20, -110, dict(bob=14, rate=0.8)))):
            P.quadruped(sc, x, y, z, yaw=yaw, s=2.0 if i < 3 else 1.6, length=84, height=64, girth=28, neck=26, neck_up=48 if 'graze' not in kw else 10, head=24,
                        tail=12, body=coat_[i % 3], belly='#f1e4c8', horns='#f1e4c8', ears=coat_[i % 3], phase=r.random(), **kw)
    return build


def wolf_ridge(seed=10):
    """A wolf pack running a snow ridge, dark on white: six wolves, 2.4 times life size (it is for 100 m and
    further), always moving (far=True). 38 m long, 5.6 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 3800.0, 260.0, 760.0
        _berm(sc, L, H, w=700.0)
        for i in range(6):
            bx = -L / 2.0 + (i + 0.5) * 330 + r.uniform(-40, 40) + 600
            P.quadruped(sc, bx, r.uniform(-90, 90), H, yaw=90, s=2.4, length=92, height=58, girth=24, neck=22, neck_up=18, head=26, tail=40, stride=34,
                        body=('#3b3f52', '#5a6478', '#2a2230')[i % 3], belly='#9aa4b8', ears=DARK, rate=2.2, bob=12, phase=r.random(), travel=(L, v))
    return build


def rope_team(seed=11):
    """A roped party crossing a snow ridge: five mountaineers with packs and axes, a red rope between them. Twice
    life size, always moving (far=True). 32 m long, 6 m: TALL."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 3200.0, 240.0, 170.0
        _berm(sc, L, H, w=700.0)
        tv = (L, v / L)
        for i in range(5):
            bx = 560 - i * 280
            d = suit(r, s=2.0, pack=r.choice(SUITS), hat='beanie')
            if i % 2 == 0:
                d.update(hold_r=P.hold_tool(col=STEEL, grip=YEL, length=40.0))
            P.figure(sc, bx, 0, H, yaw=90, detail=0, stride=22, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=0.8, bob=5, phase=r.random(),
                     travel=(L, v), **d)
            if i:
                rp = sc.part(P.motion(pivot=(bx + 140, 0, H + 170), base=(bx + 140, 0), travel=tv))
                rp.box(RED, (bx + 140, 0, H + 170), (280, 7, 7))
    return build


def eagles(seed=12):
    """Three eagles wheeling: 5 m across the wings, 24 to 32 m up, circles 7 to 16 m in radius. Always moving."""
    def build(sc):
        for i, (R, z, w) in enumerate(((1100, 2400, 0.07), (1600, 3200, -0.05), (700, 2800, 0.09))):
            a = i * 2.1
            x, y = R * math.cos(a), R * math.sin(a)
            hdeg = math.degrees(a) + (90 if w > 0 else -90)
            P.bird(sc, x, y, z, heading=hdeg, span=520, body='#5a4a3c', wing='#3b2a22', beak=YEL, phase=i * 0.3, rate=0.8, flap=20, orbit=(0, 0, w))
            # a white head and tail: an eagle, not a crow
            h = sc.part(P.motion(phase=i * 0.3, rate=0.8, pivot=(x, y, z), base=(0, 0), travel=(-1.0, w)))
            hd = math.radians(hdeg)
            h.sphere(WHITE, (x + math.cos(hd) * 130, y + math.sin(hd) * 130, z + 30), 46, steps=(3, 5))
            h.box(WHITE, (x - math.cos(hd) * 200, y - math.sin(hd) * 200, z + 4), (70, 70, 8), rot=(0, hdeg, 0))
    return build


def paragliders(seed=14):
    """Three paragliders circling over the slopes: a pilot in a hot ski suit, 1.7 times life size, sitting in the lines
    under a striped canopy 10 m across, 26 to 38 m up, floating up and down as they wheel. Always moving (far=True).
    Second round: they are what shows over a banked bend's high side and over a half pipe's wall, where the land and
    everybody on it are below the road's edge. Placed 70 m out and more (the widest circle is 15 m, a canopy half 5)."""
    def build(sc):
        r = random.Random(seed)
        cols = ((RED, YEL), (TEAL, PINK), (ORANGE, WHITE))
        for i, (R, z, w) in enumerate(((1000, 2600, 0.035), (1500, 3800, -0.028), (600, 3200, 0.045))):
            a = i * 2.3
            x, y = R * math.cos(a), R * math.sin(a)
            hdeg = math.degrees(a) + (90 if w > 0 else -90)
            ph, rate, bob = i * 0.31, 0.12, -110.0
            mo = P.motion(phase=ph, rate=rate, bob=bob, base=(0, 0), travel=(-1.0, w))
            m = sc.part(mo, (x, y, z), hdeg + 90.0, 1.0)
            pts = []
            for k in range(-4, 5):                    # the canopy: an arc of eight panels
                th = math.radians(k * 13.0)
                pts.append((math.sin(th) * 620.0, 760.0 - (1.0 - math.cos(th)) * 620.0))
            for k in range(8):
                (x1, z1), (x2, z2) = pts[k], pts[k + 1]
                tilt_box(m, cols[i][k % 2], (x1, 0, z1), (x2, 0, z2), 300, 36)
            for k in (0, 2, 6, 8):                    # the lines
                tilt_box(m, DARK, (pts[k][0] * 0.96, 0, pts[k][1] - 10), ((-1 if k < 4 else 1) * 26, 0, 230), 6, 6)
            m.box(cols[i][0], (0, 20, 120), (70, 70, 110))        # the harness
            c = SUITS[(i * 4 + seed) % len(SUITS)]
            P.figure(sc, x, y, z, yaw=hdeg + 90.0, legs='sit', seat=46, arm_l='up', arm_r='up', detail=0, phase=ph, rate=rate, bob=bob,
                     orbit=(0, 0, w), **suit(r, s=1.7, top=c, bottom=c, hat='helmet', hat_col=WHITE))
    return build


def crew_skidoo(seed=13):
    """The shoot's snow camera: a snowmobile behind the crew's tape, its driver, the camera on a post at its back and
    the operator panning it after the chase, the assistant pointing. LOW."""
    def build(sc):
        r = random.Random(seed)
        w = C.WARDROBES['Snow']
        g = sc.still()
        C.tape(g, 620, -150, posts=5)
        g.box(C.YEL, (0, 0, 46), (230, 84, 44))
        g.box(C.DARK, (20, 0, 76), (120, 60, 16))
        g.box(C.DARK, (-20, 0, 14), (200, 60, 28))
        tilt_box(g, C.STEEL, (96, 0, 70), (120, 0, 118), 70, 5)
        for sy in (-1, 1):
            g.box(C.DARK, (120, sy * 40, 6), (120, 12, 8))
        g.box(C.STEEL, (-100, 0, 86), (8, 8, 60))
        pan = dict(swing=22.0, pivot=(-100.0, 0.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.12, phase=0.2, **pan))
        C.film_camera(cam, -100, 0, 116, yaw=0)
        P.figure(sc, -100, 70, arm_l='push', arm_r='push', rate=0.12, phase=0.2, sway=pan, **C.member(r, w, hat='beanie', hat_col=RED))
        P.figure(sc, 30, 0, z=38, yaw=-90, legs='sit', seat=46, arm_l='push', arm_r='push', rate=0.3, bob=-1.0, **C.member(r, w))
        P.figure(sc, 210, -70, yaw=-25, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), **C.member(r, w))
        P.figure(sc, -240, -60, yaw=20, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.5, react=True, phase=r.random(), **C.member(r, w))
        C.case(g, 250, 60)
    return build


JOBS = [
    ('SM_SkiHill_A', ski_hill(-1, 1)),      # for the right of the road: the slope faces the car coming down the road
    ('SM_SkiHillL_A', ski_hill(1, 11)),     # its twin for the left
    ('SM_PisteRun_A', piste_run(2), dict(far=True, grow=(6500.0, 400.0, 1400.0), grow_down=(6500.0, 400.0, 1400.0))),
    ('SM_BoardPipe_A', board_pipe(3), dict(grow=(60.0, 60.0, 250.0))),
    ('SM_SnowStand_A', stand(4)),
    ('SM_SnowStand_B', stand(24, flags=(TEAL, PINK), skirt=(YEL, RED, WHITE, BLUE))),
    ('SM_LodgeDeck_A', lodge_deck(5)),
    ('SM_Snowmen_A', snowmen(6)),
    ('SM_IceClimb_A', ice_climb(7)),
    ('SM_IceClimb_B', ice_climb_b(27)),
    ('SM_DogSled_A', dog_sled(8)),
    ('SM_Ibex_A', ibex(9)),
    ('SM_WolfRidge_A', wolf_ridge(10), dict(far=True)),
    ('SM_RopeTeam_A', rope_team(11), dict(far=True)),
    ('SM_Eagles_A', eagles(12), dict(far=True, grow=(1800.0, 1800.0, 200.0), grow_down=(1800.0, 1800.0, 200.0))),
    ('SM_Paragliders_A', paragliders(14), dict(far=True, grow=(2100.0, 2100.0, 300.0), grow_down=(2100.0, 2100.0, 300.0))),
    ('SM_CrewSkidoo', crew_skidoo(13)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
