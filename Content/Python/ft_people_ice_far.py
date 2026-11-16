"""FULL THROTTLE (Unreal) the population of level 5, Ice Mountain: FAR LIFE AND CREATURES (/Game/People/IceMountain/SM_Far*).

The extra worker's kit for the far band: what lives from the top of the slopes (230 m) to the first ridge (340 m) and
in the sky over it. Everything is drawn several times life size and in hot colours on white or dark rock, because it
is seen from a road at 120 mph, 250 to 400 m away; everything is saved with far=True (the always-moving master: the
track stops vertex movement past 180 m for other pieces). The main worker's kit (ft_people_ice.py) has the cast of
the middle distance (its own eagles, ibex, wolves and sleds at 30 to 240 m, life size to 2.4 times): these are the
same mountain's life one layer further out, 4 to 6 times life size, each on a piece of mountain it brings with it.

  SM_FarSkiFace     a snow face with a switchback piste: skiers and boarders come down the upper leg, and down the
                    lower leg the other way, between red and blue gates. 160 m long, 75 m tall.
  SM_FarEagles      five great eagles wheeling 60 to 110 m up, 14 m across the wings.
  SM_FarClimbWall   a dark rock wall with three roped parties on it, a hanging camp and a climber swinging on a rope.
  SM_FarIbexArete   a herd of ibex leaping along a rock arete, one after another.
  SM_FarWolfCrest   a wolf pack on a snow crest: three howling, the rest running the skyline.
  SM_FarSledRace    three dog teams racing along a snow crest, flags at the turn.

DOWNHILL (the trick is the main kit's, see ft_people_ice.py's header: written out again here because a worker never
imports another worker's file). `travel` moves a part along the mesh's X only and a swing is a sine; but with the
swing's rate equal to the travel's rounds a second, the swing's angle is a fixed function of the place on the line,
and a swing of a degree about a pivot 600 m under the ground is a straight slide. So a slider's height is
A sin(2 pi (f + c)) and its place (f - 0.5) L - S sin(2 pi (f + c)): with c = -0.15, S = 0.15 L it comes DOWN steadily
from f = 0.4 to 0.9, runs out over a small rise, wraps, and climbs back unseen inside the hill.

Rules kept: nothing glows or blinks; no words or marks; nothing big, white and furry on two legs (the boss is THE
ABOMINABLE): every person is in a hot-coloured suit, every creature is dark and four-legged or a bird.

Run in the editor:  import ft_people_ice_far as K; K.run()        (or K.run(['SM_FarEagles']))
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/IceMountain'

SNOW, SNOW_SH, SNOW_D, ICE, ICE_D = '#f4f9ff', '#cdd6f1', '#a9b8e0', '#bfe3ff', '#7fb0e0'
ROCK, ROCK_D, ROCK_DD = '#6a7488', '#4a5268', '#343a4e'
RED, BLUE, ORANGE, PINK, TEAL, YEL, LIME, VIOLET = '#ff2d3c', '#2e6bff', '#ff8a2a', '#ff4fb8', '#19b5c9', '#ffd23f', '#b8ff5a', '#8f6bff'
DARK, WHITE, WOOD = '#22202a', '#fff6ea', '#9a6a44'
SUITS = (RED, ORANGE, YEL, PINK, LIME, '#2ee6ff', BLUE, VIOLET)


def suit(r, s, **fixed):
    """Somebody in a ski suit of one hot colour, head to foot."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear='trousers', top=c, bottom=c, hat='beanie', hat_col=r.choice(SUITS), shoes=DARK, hair='short', s=s)
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ downhill
def run_point(f, L, S, A, c=-0.15, d=1):
    a = 2.0 * math.pi * (f + c * d)
    return ((f - 0.5) * L - S * math.sin(a), A * d * math.sin(a))


def run_curve(L, S, A, c=-0.15, d=1, n=12):
    """The run from its top to the end of its run-out, in the order a slider meets it: [(x, height over the middle)]."""
    fs = [0.4 + 0.6 * i / float(n) for i in range(n + 1)]
    return [run_point(f if d > 0 else 1.0 - f, L, S, A, c, d) for f in fs]


def slider(sc, r, bx, by, zb, L, v, S, A, s, c=-0.15, d=1, kind='ski', pz=60000.0):
    """A skier or a boarder who comes down the run and round again (bx: its place in the queue, -L/2 .. L/2)."""
    th = -S / pz
    px = -(A * d) / th
    sway = dict(swing=math.degrees(th), pivot=(bx - px, by, zb - pz), axis=90.0)
    rate = d * v / L
    ph = (bx / L + 0.5 + c * d) % 1.0
    board = kind == 'board'
    yaw = 90.0 * d - (90.0 if board else 0.0)
    arms = dict(arm_l='out', arm_r='out') if board else dict(arm_l=((35, -30), (60, 20)), arm_r=((35, -30), (60, 20)))
    P.figure(sc, bx, by, zb, yaw=yaw, legs='wide', detail=0, rate=rate, phase=ph, sway=sway, travel=(L, d * v), **dict(suit(r, s), **arms))
    m = sc.part(P.motion(phase=ph, rate=rate, base=(bx, by), travel=(L, rate), **sway), (bx, by, zb), yaw, s)
    col = r.choice(SUITS)
    if board:
        m.box(col, (0, -2, 3), (170, 34, 7))
    else:
        for sx in (-1, 1):
            m.box(col, (sx * 17, -14, 3), (12, 180, 6))


def ski_face(seed=1):
    """A snow face with a switchback piste, for 300 m and further: a line of skiers and boarders, five times life
    size, comes down the upper leg towards +X and down the lower leg towards -X, between red and blue gates."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L, S, s = 13000.0, 1950.0, 5.0
        A1, A2 = 1500.0, 1700.0
        base = 500.0
        zb1 = base + A1                    # the lower leg's middle (front, y -900 .. 500)
        zb2 = base + 2.0 * A1 - 250.0 + A2    # the upper leg's (behind it, y 500 .. 1900)
        legs = ((zb1, A1, -1, -900.0, 500.0, SNOW), (zb2, A2, 1, 500.0, 1900.0, '#e9f0ff'))
        for zb, A, d, y0, y1, col in legs:
            pts = [(x, z + zb) for x, z in run_curve(L, S, A, d=d)]
            top = pts[0]
            back = top[0] - d * 2600.0
            poly = [(back - d * 1500.0, -300.0), (back, top[1])] + pts + [(pts[-1][0] + d * 600.0, -300.0)]
            g.side(col, poly, y0, y1)
            # the piste's lower edge: netting in two colours, so that the line of the run reads from far off
            for k in range(len(pts) - 1):
                a, b = pts[k], pts[k + 1]
                tilt_box(g, (ORANGE, PINK)[k % 2], (a[0], y0 - 14, a[1] - 60), (b[0], y0 - 14, b[1] - 60), 30, 150)
            # gates: tall panels, red and blue by turns, on the uphill side of the lanes
            for k in range(2, len(pts) - 2, 2):
                x, z = pts[k]
                col2 = RED if (k // 2) % 2 else BLUE
                for dx in (-90, 90):
                    g.box(col2, (x + dx, y1 - 220, z + 260), (36, 36, 560))
                g.box(col2, (x, y1 - 220, z + 430), (216, 30, 220))
            # the start arch at the top of the leg
            for sy in (y0 + 160, y1 - 160):
                g.box(TEAL, (top[0], sy, top[1] + 420), (110, 110, 900))
            g.box(YEL, (top[0], (y0 + y1) / 2.0, top[1] + 900), (110, y1 - y0 - 200, 200))
            n = 6
            lanes = (0.2, 0.6, 0.35, 0.75, 0.25, 0.55)
            for i in range(n):
                bx = -L / 2.0 + (i + 0.5) * L / n
                slider(sc, r, bx, y0 + (y1 - y0) * lanes[i], zb, L, 1700.0, S, A, s, d=d, kind='board' if i % 3 == 1 else 'ski')
        # the mountain behind the piste: a shaded shoulder and a peak, rock showing through
        g.side(SNOW_SH, [(-9800, -300), (-5200, 4300), (-2400, 5600), (600, 7600), (2600, 6500), (5600, 4700), (9800, -300)], 1900, 2600)
        g.side(ROCK, [(-1700, 4200), (-300, 5900), (500, 7000), (1500, 5800), (2600, 4300)], 1880, 1900)
        g.side(ROCK_D, [(-6600, 900), (-5200, 3300), (-4300, 2400), (-3600, 900)], 1880, 1900)
        g.side(SNOW, [(-200, 6500), (600, 7600), (1500, 6600), (700, 6900)], 1860, 1900)
    return build


def eagles(seed=2):
    """Five great eagles wheeling, for 250 m and further: 14 m across the wings, 60 to 110 m up, circles 30 to 75 m in
    radius, two of them the other way round. Slow wing beats; white heads and tails (eagles, not crows)."""
    def build(sc):
        for i, (R, z, w) in enumerate(((3400, 7000, 0.045), (5600, 9800, -0.034), (2200, 8600, 0.06), (7400, 11000, 0.028), (4600, 6200, -0.04))):
            a = i * 1.9
            x, y = R * math.cos(a), R * math.sin(a)
            hdeg = math.degrees(a) + (90 if w > 0 else -90)
            P.bird(sc, x, y, z, heading=hdeg, span=1400, body='#4a3a30', wing='#2a2230', beak=YEL, phase=i * 0.27, rate=0.55, flap=22, orbit=(0, 0, w))
            h = sc.part(P.motion(phase=i * 0.27, rate=0.55, pivot=(x, y, z), base=(0, 0), travel=(-1.0, w)))
            hd = math.radians(hdeg)
            h.sphere(WHITE, (x + math.cos(hd) * 350, y + math.sin(hd) * 350, z + 80), 125, steps=(3, 5))
            h.box(WHITE, (x - math.cos(hd) * 540, y - math.sin(hd) * 540, z + 10), (190, 190, 20), rot=(0, hdeg, 0))
    return build


# ------------------------------------------------------------------------------------------ the wall
def climb_wall(seed=3):
    """A dark rock wall, 46 m wide and 62 m tall, with three roped parties on it in hot colours (five times life
    size), a hanging camp, a climber swinging across on a rope, and two on the summit waving a flag."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        s = 5.0
        F = -520.0                                     # the face the climbers are on
        g.box(ROCK_D, (0, 0, 2900), (3000, 1040, 6400))
        g.box(ROCK, (-1900, 140, 2200), (1500, 900, 5000))
        g.box(ROCK_DD, (1800, 180, 1800), (1700, 860, 4200))
        g.box(ROCK, (900, -40, 4900), (900, 1000, 1300))
        g.box(SNOW, (0, 0, 6150), (3100, 1100, 130))
        g.box(SNOW, (-1900, 140, 4760), (1600, 960, 130))
        g.box(SNOW, (1800, 180, 3960), (1800, 920, 130))
        g.box(ICE, (-760, F - 6, 2600), (260, 30, 4300))       # a frozen fall down the face
        g.box(SNOW, (500, F - 60, 3300), (1300, 130, 90))       # ledges
        g.box(SNOW, (-1500, F + 60, 1500), (900, 130, 90))

        def climber(x, z, ph, **kw):
            d = suit(r, s, pack=r.choice(SUITS))
            mo = dict(rate=0.22, phase=ph, bob=-150.0)
            P.figure(sc, x, F - 75, z, yaw=180, detail=0, arm_l=r.choice(('up', 'vee')), arm_r=r.choice(('up', 'wave')), legs='wide', **dict(d, **dict(mo, **kw)))

        # three parties: a rope in the party's colour from the top one's anchor down through all of them
        for px, z0, n, rope in ((-250, 1300, 3, YEL), (900, 500, 2, PINK), (-1650, 400, 2, LIME)):
            top_z = z0 + (n - 1) * 1450 + 1700
            g.box(rope, (px + 150, F - 18, (z0 + top_z) / 2.0 + 300), (26, 26, top_z - z0))
            for k in range(n):
                climber(px + (k % 2) * 170, z0 + k * 1450, r.random())
        # the one who swings across on a rope
        ax, az = 1500.0, 5600.0
        sw = dict(swing=20.0, pivot=(ax, F - 75, az), axis=90.0)
        P.figure(sc, ax, F - 75, az - 3300, yaw=180, detail=0, arm_l='up', arm_r='up', legs='stride', rate=0.24, phase=0.1, sway=sw, **suit(r, s, top=RED, bottom=RED))
        rp = sc.part(P.motion(rate=0.24, phase=0.1, **sw))
        rp.box(ORANGE, (ax, F - 40, az - 1250), (26, 26, 2500))
        # the hanging camp: a ledge tent on two ropes
        cx, cz = -1750.0, 3300.0
        cm = sc.part(P.motion(rate=0.3, phase=0.4, swing=3.0, pivot=(cx, F, cz + 1200), axis=90.0))
        cm.box(DARK, (cx, F - 110, cz), (520, 220, 30))
        cm.box(ORANGE, (cx, F - 110, cz + 150), (480, 210, 270))
        cm.box(PINK, (cx, F - 110, cz + 310), (360, 190, 60))
        for dx in (-230, 230):
            cm.box(YEL, (cx + dx, F - 20, cz + 760), (22, 22, 900))
        # the summit: two with a flag
        P.figure(sc, -500, -200, 6210, detail=0, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=60, rate=1.0, phase=0.2, **suit(r, s))
        P.figure(sc, 300, -200, 6210, detail=0, arm_l='hip', arm_r='up', bob=40, rate=0.9, phase=0.6, **suit(r, s))
        g.box(DARK, (680, -200, 7000), (40, 40, 1600))
        fl = sc.part(P.motion(swing=14.0, pivot=(680, -200, 7500), axis=P.UP, rate=0.8))
        fl.box(PINK, (1080, -200, 7500), (800, 24, 480))            # no country's flag: pink, a teal block at the fly
        fl.box(TEAL, (1330, -214, 7500), (300, 12, 480))
    return build


# ------------------------------------------------------------------------------------------ crests and their creatures
def crest(sc, L, H, w=900.0, rock=False):
    """A crest to run along: a level top H up, L long, and a shaded step in front of it (the road's side)."""
    g = sc.still()
    top = ROCK_D if rock else SNOW
    g.side(top, [(-L / 2.0 - 900, -300), (-L / 2.0 - 250, H * 0.72), (-L / 2.0, H), (L / 2.0, H), (L / 2.0 + 250, H * 0.72), (L / 2.0 + 900, -300)], -w / 2.0, w / 2.0)
    g.side(ROCK if rock else SNOW_SH, [(-L / 2.0 - 1500, -300), (-L / 2.0 - 500, H * 0.5), (L / 2.0 + 500, H * 0.5), (L / 2.0 + 1500, -300)], -w / 2.0 - 420, -w / 2.0)
    return g


def ibex_arete(seed=4):
    """A herd of ibex leaping along a rock arete, one after another: nine of them, five times life size, tan with
    cream bellies and great horns, on dark rock with spires behind. 100 m long, 24 m to the spires."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 9000.0, 900.0, 950.0
        g = crest(sc, L, H, w=800.0, rock=True)
        for k in range(7):                                   # spires behind the ledge, snow on their tops
            x = -L / 2.0 + (k + 0.5) * L / 7.0 + r.uniform(-300, 300)
            h = r.uniform(600, 1500)
            g.cyl(ROCK_DD if k % 2 else ROCK, (x, 700, 0), 620, H + h, steps=5, top_r=200)
            g.cyl(SNOW, (x, 700, H + h - 10), 215, 90, steps=5, top_r=120)
        for k in range(5):                                   # snow lying on the lip of the ledge
            g.box(SNOW, (-L / 2.0 + (k + 0.5) * L / 5.0 + r.uniform(-400, 400), -330, H - 40), (r.uniform(600, 1100), 160, 110))
        coats = ('#d9924a', '#b87a50', '#e8a85a')
        for i in range(9):
            bx = -L / 2.0 + (i + 0.5) * L / 9.0 + r.uniform(-150, 150)
            P.quadruped(sc, bx, r.uniform(-140, 60), H, yaw=90, s=5.0, length=84, height=64, girth=28, neck=26, neck_up=48, head=24, tail=12, stride=30,
                        body=coats[i % 3], belly='#f1e4c8', horns='#f1e4c8', ears=coats[i % 3], rate=0.9, bob=230, phase=r.random(), travel=(L, v))
    return build


def wolf_crest(seed=5):
    """A wolf pack on a snow crest, black on white: three on a rock, howling (heads thrown back, nodding), and five
    running the skyline. Five to six times life size. 80 m long."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 7600.0, 800.0, 1100.0
        g = crest(sc, L, H, w=1000.0)
        g.cyl(ROCK_D, (0, 250, H - 200), 900, 760, steps=6, top_r=560)
        g.cyl(ROCK, (-950, 300, H - 200), 520, 480, steps=5, top_r=300)
        darks = ('#2a2230', '#3b3f52', '#22202a')
        for i, (x, y, z, yaw) in enumerate(((0, 180, H + 560, 70), (-950, 260, H + 280, 110), (520, 380, H + 560, 60))):
            P.quadruped(sc, x, y, z, yaw=yaw, s=6.0, length=92, height=58, girth=24, neck=24, neck_up=74, head=28, tail=40, graze=14, wag=18,
                        body=darks[i], belly='#7a8fb8', ears=DARK, rate=0.35, phase=i * 0.3)
        for i in range(5):
            bx = -L / 2.0 + (i + 0.5) * 760 + r.uniform(-80, 80)
            P.quadruped(sc, bx, r.uniform(-380, -180), H, yaw=90, s=5.0, length=92, height=58, girth=24, neck=22, neck_up=18, head=26, tail=40, stride=36,
                        body=darks[i % 3], belly='#7a8fb8', ears=DARK, rate=2.0, bob=40, phase=r.random(), travel=(L, v))
    return build


def sled_race(seed=6):
    """Three dog teams racing along a snow crest: four dogs a team in pairs, a bright sled, a musher leaning on the
    handle, a pennant on the sled. Four to five times life size. 110 m long, flags at both ends."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 10000.0, 700.0, 1300.0
        g = crest(sc, L, H, w=1100.0)
        tv = (L, v / L)
        for sx in (-1, 1):                                    # the turn flags
            x = sx * (L / 2.0 - 200)
            g.box(DARK, (x, 350, H + 700), (40, 40, 1400))
            fl = sc.part(P.motion(swing=16.0, pivot=(x, 350, H + 1100), axis=P.UP, rate=0.9, phase=0.25 + 0.25 * sx))
            fl.box((PINK, TEAL)[sx > 0], (x + 330, 350, H + 1100), (660, 24, 440))
        dogs = ('#3b3f52', '#5a6478', '#8e5a3a', '#2a2230')
        for t, (x0, y0, col) in enumerate(((-3300, -300, RED), (0, 60, TEAL), (3300, -180, YEL))):
            for k in range(4):
                P.quadruped(sc, x0 + 620 + (k // 2) * 520, y0 + (-90 if k % 2 else 90), H, yaw=90, s=4.0, length=70, height=48, girth=20, neck=16, neck_up=35,
                            head=18, tail=22, wag=30, stride=36, body=dogs[(k + t) % 4], belly='#cdd6f1', ears=DARK, rate=2.4, bob=24, phase=r.random(), travel=(L, v))
            sl = sc.part(P.motion(base=(x0, y0), travel=tv, pivot=(x0, y0, H)))
            sl.box(col, (x0 + 760, y0, H + 150), (1100, 30, 30))                  # the gang line
            sl.box(col, (x0 - 60, y0, H + 110), (520, 260, 120))                    # the sled
            sl.box(DARK, (x0 - 60, y0, H + 30), (640, 300, 40))
            sl.box(WOOD, (x0 - 330, y0, H + 330), (30, 260, 420))
            sl.box(DARK, (x0 - 60, y0 + 100, H + 640), (30, 30, 900))
            pn = sc.part(P.motion(base=(x0, y0), travel=tv, swing=14.0, pivot=(x0 - 60, y0 + 100, H + 950), axis=P.UP, rate=1.1))
            pn.box(col, (x0 - 300, y0 + 100, H + 950), (460, 20, 260))
            P.figure(sc, x0 - 470, y0, H, yaw=90, detail=0, arm_l='push', arm_r='push', rate=1.0, bob=12, phase=0.2 * t, travel=(L, v), **suit(r, 4.5))
    return build


BIGGROW = dict(far=True, grow=(600.0, 300.0, 300.0), grow_down=(600.0, 300.0, 300.0))
JOBS = [
    ('SM_FarSkiFace', ski_face(1), BIGGROW),
    ('SM_FarEagles', eagles(2), dict(far=True, grow=(7600.0, 7600.0, 400.0), grow_down=(7600.0, 7600.0, 400.0))),
    ('SM_FarClimbWall', climb_wall(3), BIGGROW),
    ('SM_FarIbexArete', ibex_arete(4), BIGGROW),
    ('SM_FarWolfCrest', wolf_crest(5), BIGGROW),
    ('SM_FarSledRace', sled_race(6), BIGGROW),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, [(j[0], j[1], j[2]) for j in JOBS], only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
