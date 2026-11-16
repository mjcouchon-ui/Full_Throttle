"""FULL THROTTLE (Unreal) the population, level 0: Bayshore, FAR LIFE AND CREATURES (the extra worker "far").

Everything living from 150 m to the horizon on the bay side, and the level's big animals. Six meshes, all saved with
far=True (the always-moving master): they stand past the 180 m line where roadside pieces stop moving.
The road is raised and the beach falls away from it (2 m under the road at 110 m, 8 m at 220 m, the sea 340 m out),
so the whole beach and the sea are in the chase camera's view: what stands there is small. So everything here is
drawn BIG (people at 1.6 to 2.3 times life size, gulls 3 to 4.6 m across, dolphins 6 m long), massed, in hot colours
against sand and water, and every piece moves with a big, slow gesture.

  SM_FarFlock      a wheeling flock of 22 gulls in two rings that turn against each other      (Scatter, the beach)
  SM_FarPelicans   a skein of 9 pelicans in a V that travels 420 m along the shore and comes round (Afloat)
  SM_FarDolphins   a school of 6 dolphins leaping along the shore, foam where each comes down      (Afloat)
  SM_FarSurf       surfers riding, paddle-boarders, swimmers waving, a line of buoys             (Afloat, the surf line)
  SM_FarKites      four kite flyers on the sand, their kites 18 to 34 m up, two dogs running rings  (Scatter, the beach)
  SM_FarCrowd      a beach crowd as a band of colour: 24 people hopping out of step under big parasols, a ball
                   thrown up, three dogs running along the band                                (Scatter, the lower beach)

The placing rules: the block "far" of Source/FullThrottle/FTPeopleL0.inl.
Run in the editor:  import ft_people_bay_far as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/Bayshore'

WHITE, CREAM = '#fff6ea', '#f1e4c8'
HOTPINK, YEL, MINT, SKY, RED, ORANGE, VIOLET, NAVY, LIME = '#ff5fa2', '#ffc93c', '#7dffb0', '#2ee6ff', '#ff2d3c', '#ff7a3c', '#8f6bff', '#2b2350', '#b8ff5a'
HOT = (HOTPINK, YEL, RED, ORANGE, VIOLET, SKY, LIME, MINT)
GULL, GULL_TIP, BEAK = '#f4f9ff', '#3a4660', '#ffc93c'
DOLPHIN, DOLPHIN_PALE = '#4f6fa8', '#e6f0fa'
PEL_BODY, PEL_WING, PEL_BILL = '#fff6ea', '#5a4a3c', '#ff9a2e'


# ------------------------------------------------------------------------------------------ helpers
def gull(sc, x, y, z, heading, span, common, flap=34.0, body=GULL, tip=GULL_TIP, beak=BEAK, bill=0.0):
    """A cheap big bird (about 70 triangles): a body, a beak, two wings of two panels each (white, a dark tip) that
    beat about its line of flight. common: the motion values every part of it shares (phase, rate, orbit or travel).
    bill: a pelican's bill, as a share of the span."""
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    rx, ry = -ty, tx
    L = span * 0.4
    b = sc.part(P.motion(pivot=(x, y, z), **common))
    tilt_box(b, body, (x - tx * L * 0.55, y - ty * L * 0.55, z - L * 0.02), (x + tx * L * 0.5, y + ty * L * 0.5, z + L * 0.05), L * 0.3, L * 0.26)
    if bill:
        tilt_box(b, body, (x + tx * L * 0.45, y + ty * L * 0.45, z + L * 0.05), (x + tx * L * 0.75, y + ty * L * 0.75, z + L * 0.2), L * 0.2, L * 0.2)
        tilt_box(b, beak, (x + tx * L * 0.7, y + ty * L * 0.7, z + L * 0.18), (x + tx * (L * 0.7 + span * bill), y + ty * (L * 0.7 + span * bill), z - L * 0.02), L * 0.12, L * 0.16)
    else:
        tilt_box(b, beak, (x + tx * L * 0.5, y + ty * L * 0.5, z + L * 0.05), (x + tx * L * 0.78, y + ty * L * 0.78, z + L * 0.01), L * 0.1, L * 0.1)
    half = span * 0.5
    for side in (-1, 1):
        w = sc.part(P.motion(swing=side * flap, pivot=(x, y, z + L * 0.08), axis=heading, **common))
        a = (x + side * rx * L * 0.1, y + side * ry * L * 0.1, z + L * 0.08)
        mid = (x + side * rx * half * 0.55, y + side * ry * half * 0.55, z + L * 0.3)
        end = (x + side * rx * half - tx * L * 0.25, y + side * ry * half - ty * L * 0.25, z + L * 0.12)
        P._wing(w, body if not bill else tip, a, mid, tx, ty, L * (0.5 if not bill else 0.62))
        P._wing(w, tip, mid, end, tx, ty, L * (0.36 if not bill else 0.46))


def far_person(sc, x, y, mo, s=1.6, top=HOTPINK, bottom=NAVY, skin='#d9a070', hair='#2a2230', arms='vee', yaw=0.0):
    """A person for 150 m and beyond (about 72 triangles): legs, body, head, hair, two arms. Drawn into the part with
    motion mo, so a group that shares mo hops together and the next group hops out of step."""
    m = sc.part(mo, (x, y, 0.0), yaw, s)
    m.box(bottom, (0, 0, 34), (36, 24, 68))
    m.box(top, (0, 0, 95), (48, 28, 56))
    m.box(skin, (0, 0, 146), (40, 38, 42))
    m.box(hair, (0, 4, 166), (44, 40, 14))
    for side in (-1, 1):
        if arms == 'vee':
            tilt_box(m, skin, (side * 26, 0, 116), (side * 58, 0, 176), 13, 13)
        elif arms == 'up':
            tilt_box(m, skin, (side * 26, 0, 116), (side * 34, 0, 184), 13, 13)
        else:
            tilt_box(m, skin, (side * 28, 0, 118), (side * 36, 0, 64), 13, 13)


def parasol(g, x, y, col, col2, r=320.0, h=380.0):
    g.box(WHITE, (x, y, h / 2.0), (9, 9, h))
    g.cyl(col, (x, y, h - 50), r, 60, steps=8, top_r=r * 0.45)
    g.cyl(col2, (x, y, h + 10), r * 0.45, 46, steps=8, top_r=8)


def dog_ring(sc, cx, cy, R, a_deg, rps, body, s=2.0, phase=0.0):
    """A dog running a ring round (cx, cy): rps rounds a second (negative: the other way)."""
    a = math.radians(a_deg)
    yaw = a_deg + (180.0 if rps > 0 else 0.0)
    P.quadruped(sc, cx + math.cos(a) * R, cy + math.sin(a) * R, yaw=yaw, s=s, length=70, height=48, girth=22, neck=16, neck_up=35, head=20, tail=24,
                wag=28, stride=34, rate=2.6, bob=7, phase=phase, orbit=(cx, cy, rps), body=body, ears='#5a4a3c', snout='#22202a')


def dog_run(sc, x, y, L, speed, body, s=2.0, phase=0.0):
    """A dog running the piece's X (speed cm/s, negative towards -X) and coming round."""
    P.quadruped(sc, x, y, yaw=90.0 if speed > 0 else -90.0, s=s, length=70, height=48, girth=22, neck=16, neck_up=35, head=20, tail=24,
                wag=28, stride=34, rate=2.6, bob=7, phase=phase, travel=(L, speed), body=body, ears='#5a4a3c', snout='#22202a')


# ------------------------------------------------------------------------------------------ the sky
def flock(seed=201):
    """A wheeling flock: 22 gulls, 3 to 4.6 m across, in two rings that turn against each other (an inner, lower ring
    of 12 and an outer, higher ring of 10), each bird on its own radius and height, wings beating out of step."""
    def build(sc):
        r = random.Random(seed)
        for ring, (n, r0, r1, z0, z1, sgn) in enumerate(((12, 500, 1400, 1100, 2400, 1), (10, 1500, 2400, 2000, 3800, -1))):
            for k in range(n):
                a_deg = k * 360.0 / n + r.uniform(-12, 12)
                a = math.radians(a_deg)
                R = r.uniform(r0, r1)
                rps = sgn * r.uniform(0.040, 0.062) * (1100.0 / R) ** 0.5
                common = dict(phase=r.random(), rate=r.uniform(1.1, 1.6), base=(0.0, 0.0), travel=(-1.0, rps), bob=-r.uniform(40, 110))
                gull(sc, math.cos(a) * R, math.sin(a) * R, r.uniform(z0, z1), a_deg + sgn * 90.0, r.uniform(330, 500), common)
    return build


def pelicans(seed=211):
    """A skein of 9 pelicans, 4.6 m across, in a V low over the water: it travels 420 m along the shore (the piece's X)
    and comes round again. Slow, deep wing beats that run down the line one bird after the next."""
    def build(sc):
        r = random.Random(seed)
        L, sp = 42000.0, 1500.0
        for k in range(9):
            row = (k + 1) // 2
            side = -1 if k % 2 else 1
            x = 1200.0 - row * 620.0 + r.uniform(-60, 60)
            y = side * row * 430.0
            common = dict(phase=row * 0.13, rate=0.75, base=(x, y), travel=(L, sp / L), bob=-50.0)
            gull(sc, x, y, 1700.0 + row * 70.0 + r.uniform(-40, 40), 0.0, 540.0, common, flap=24.0, body=PEL_BODY, tip=PEL_WING, beak=PEL_BILL, bill=0.2)
    return build


# ------------------------------------------------------------------------------------------ the sea
def _dolphin(m, x, y, z, s, col=DOLPHIN, pale=DOLPHIN_PALE):
    """An arched dolphin heading +X, its back's top at z (cm), s times a 3 m animal."""
    def q(dx, dz):
        return (x + dx * s, y, z + dz * s)
    tilt_box(m, col, q(-150, -70), q(-60, -14), 46 * s, 44 * s)          # the tail stock, rising
    tilt_box(m, col, q(-70, -18), q(60, -8), 66 * s, 60 * s)             # the body, over the top
    tilt_box(m, col, q(50, -10), q(130, -52), 54 * s, 50 * s)            # the head, going down
    tilt_box(m, col, q(125, -50), q(172, -84), 20 * s, 18 * s)           # the beak
    tilt_box(m, pale, q(-50, -40), q(70, -38), 50 * s, 22 * s)           # the belly
    tilt_box(m, col, q(-12, 8), q(-48, 52), 12 * s, 36 * s)              # the fin
    m.box(col, q(-168, -84), (44 * s, 130 * s, 12 * s), rot=(40, 0, 0))  # the flukes


def dolphins(seed=221):
    """A school of 6 dolphins, 6 m long, leaping along the shore: each hops out of the water and goes under again as
    it travels the piece's X, one after another, with a ring of foam where it comes down and a streak behind it."""
    def build(sc):
        r = random.Random(seed)
        L = 22000.0
        for i in range(6):
            x = -2200.0 + i * 900.0 + r.uniform(-150, 150)
            y = r.uniform(-700, 700)
            sp = r.uniform(950, 1150)
            rate = r.uniform(0.36, 0.44)
            ph = r.random()
            s = r.uniform(2.6, 3.2)
            m = sc.part(P.motion(base=(x, y), travel=(L, sp / L), bob=640.0, rate=rate, phase=ph))
            _dolphin(m, x, y, -60.0, s)
            f = sc.part(P.motion(base=(x, y), travel=(L, sp / L), bob=-10.0, rate=rate, phase=ph))
            f.cyl(WHITE, (x + 60, y, -8), 300, 40, steps=8, top_r=180)
            tilt_box(f, WHITE, (x - 150, y, 2), (x - 1400, y, 4), 220, 16)
    return build


def _board(m, x, y, col, stripe, length=520.0, w=120.0):
    m.box(col, (x, y, 14), (length, w, 16))
    m.box(stripe, (x, y, 23), (length * 0.8, w * 0.26, 3))
    tilt_box(m, WHITE, (x - length * 0.45, y, 6), (x - length * 2.0, y, 50), 250, 40)      # the foam it throws
    tilt_box(m, WHITE, (x - length * 0.4, y + 110, 4), (x - length * 1.6, y + 300, 26), 150, 24)


def surf(seed=231):
    """The surf line: five surfers riding (2.2 times life size, arms out, rocking on bright boards that throw foam),
    two paddle-boarders standing tall with long paddles, six swimmers who wave and bob, and a line of orange buoys."""
    def build(sc):
        r = random.Random(seed)
        L = 16000.0
        suits = ('#22202a', RED, NAVY, HOTPINK, VIOLET)
        for i in range(5):
            x, y = -3600.0 + i * 1800.0 + r.uniform(-300, 300), r.uniform(-900, 300)
            sp = r.uniform(600, 820) * (1 if i % 2 == 0 else -1)
            ph, col = r.random(), HOT[i]
            lk = P.look(r, tops=suits, bottoms=('#22202a',), wear='swim', sleeves='long', s=2.2)
            P.figure(sc, x, y, z=22, yaw=(90 + 55) if sp > 0 else (-90 - 55), legs='wide', arm_l='out', arm_r='out', rock=10, rate=0.4, phase=ph,
                     travel=(L, sp), detail=1, **lk)
            b = sc.part(P.motion(base=(x, y), travel=(L, sp / L), bob=-8, rate=0.4, phase=ph))
            if sp > 0:
                _board(b, x, y, col, WHITE)
            else:
                b.box(col, (x, y, 14), (520, 120, 16))
                b.box(WHITE, (x, y, 23), (416, 31, 3))
                tilt_box(b, WHITE, (x + 234, y, 6), (x + 1040, y, 50), 250, 40)
                tilt_box(b, WHITE, (x + 208, y + 110, 4), (x + 832, y + 300, 26), 150, 24)
        for k in range(2):                                                   # paddle-boarders, further out
            x, y, sp = -1800.0 + k * 3300.0, 1500.0 + k * 300.0, 260.0 * (1 if k else -1)
            ph = 0.3 * k

            def paddle(m, h, side, c=(YEL, MINT)[k]):
                tilt_box(m, c, (h[0], h[1], h[2] + 70), (h[0], h[1] - 30, h[2] - 150), 6, 6)
                m.box(c, (h[0], h[1] - 34, h[2] - 165), (22, 5, 46))
            P.figure(sc, x, y, z=22, yaw=90 if sp > 0 else -90, legs='stride', arm_l='hold', arm_r='hold', hold_r=paddle, swing_r=26, axis_r='x',
                     rate=0.5, phase=ph, bob=-8, travel=(L, sp), detail=1, **P.look(r, tops=(HOTPINK, YEL), wear='shorts', hat='sun', hat_col=WHITE, s=2.3))
            b = sc.part(P.motion(base=(x, y), travel=(L, sp / L), bob=-8, rate=0.5, phase=ph))
            b.box((MINT, ORANGE)[k], (x, y, 14), (640, 150, 16))
            b.box(WHITE, (x, y, 23), (500, 40, 3))
        for k in range(6):                                                   # swimmers: head, shoulders and a waving arm
            x, y = -3900.0 + k * 1500.0 + r.uniform(-300, 300), r.uniform(-1700, -700)
            s = 2.0
            P.figure(sc, x, y, z=-104 * s, yaw=r.uniform(-30, 30), arm_l='up' if k % 2 else 'down', arm_r='wave', swing_r=30, swing_l=20 if k % 2 else 0,
                     rate=r.uniform(0.7, 1.0), phase=r.random(), bob=-22, detail=0, wear='swim', sleeves='none',
                     **P.look(r, tops=(RED, YEL, HOTPINK, SKY), s=s, hat=('cap' if k % 3 == 0 else None), hat_col=(YEL, RED)[k % 2]))
            g = sc.part(P.motion(bob=-22, rate=0.8, phase=r.random()))
            g.cyl(WHITE, (x, y, -6), 150, 12, steps=8, top_r=100)
        for k in range(7):                                                   # the buoy line beyond them
            g = sc.part(P.motion(bob=-18, rate=0.3, phase=k * 0.17))
            g.sphere(ORANGE, (-4800.0 + k * 1600.0, 2400.0, 30), 70, steps=(4, 6))
    return build


# ------------------------------------------------------------------------------------------ the beach
def _kite(m, kind, x, y, z, col, col2, hand, k=1.8):
    """A big kite flat to the road (in the XZ plane), its string down to the flyer's hand, its tail of bows.
    k: its size (1 = a 3.3 m diamond)."""
    if kind == 'diamond':
        m.box(col, (x, y, z), (330 * k, 10, 330 * k), rot=(45, 0, 0))
        m.box(col2, (x, y - 8, z), (150 * k, 6, 150 * k), rot=(45, 0, 0))
        tail = (x, y, z - 240 * k)
    elif kind == 'delta':
        m.side(col, [(x - 380 * k, z - 150 * k), (x + 380 * k, z - 150 * k), (x, z + 250 * k)], y - 6, y + 6)
        m.side(col2, [(x - 150 * k, z - 150 * k), (x + 150 * k, z - 150 * k), (x, z + 80 * k)], y - 11, y - 5)
        tail = (x, y, z - 150 * k)
    else:                                                                    # a box kite: two bands on four spars
        for dz in (-130, 130):
            m.box(col, (x, y, z + dz * k), (260 * k, 150 * k, 130 * k))
        for sx in (-1, 1):
            m.box(col2, (x + sx * 125 * k, y, z), (12, 150 * k, 400 * k))
        tail = (x, y, z - 200 * k)
    tilt_box(m, WHITE, hand, (x, y, z - 60), 8, 8)
    for j in range(5):                                                       # the tail, bow after bow
        tz = tail[2] - (90 + j * 120) * k
        tx = tail[0] - (30 + j * 46) * k
        m.box((col2, col)[j % 2], (tx, y, tz), (110 * k, 8, 44 * k), rot=(20 if j % 2 else -20, 0, 0))
        if j:
            tilt_box(m, WHITE, (tx + 46 * k, y, tz + 120 * k), (tx, y, tz), 8, 8)
        else:
            tilt_box(m, WHITE, tail, (tx, y, tz), 8, 8)


def kites(seed=241):
    """Kite flyers: four people on the sand (1.7 times life size), arms up at the strings, their kites 18 to 34 m up
    and each swaying across the sky about its flyer's hands (a diamond, a delta, a box kite, a diamond); two children
    hopping; two dogs running rings round them."""
    def build(sc):
        r = random.Random(seed)
        kinds = ('diamond', 'delta', 'box', 'diamond')
        cols = ((HOTPINK, YEL), (RED, WHITE), (VIOLET, LIME), (ORANGE, SKY))
        for i in range(4):
            x, y = -1500.0 + i * 1000.0 + r.uniform(-120, 120), r.uniform(-250, 250)
            s = 1.7
            ph, rate = r.random(), r.uniform(0.16, 0.24)
            top = P.figure(sc, x, y, yaw=r.uniform(-25, 25), arm_l='up', arm_r='vee', swing_l=10, swing_r=10, rate=rate * 4, phase=ph, detail=1,
                           **P.look(r, tops=HOT, s=s, hat=('cap', 'sun', None, 'cap')[i], hat_col=cols[i][1]))
            hand = (x, y, top + 20.0)
            kx, kz = x + r.uniform(600, 1600) * (1 if i % 2 else -1), 2600.0 + ((i * 2) % 4) * 800.0 + r.uniform(-200, 200)
            k = sc.part(P.motion(swing=r.uniform(9, 14), pivot=hand, axis=90.0, rate=rate, phase=ph, bob=-r.uniform(60, 160)))
            _kite(k, kinds[i], kx, y + 40.0, kz, cols[i][0], cols[i][1], hand)
        for k in range(2):
            P.figure(sc, -400.0 + k * 1300.0, -330.0, arm_l='vee', arm_r='vee', bob=22, rate=1.3, phase=k * 0.4, detail=1,
                     **P.look(r, tops=HOT, s=1.15, build='kid'))
        dog_ring(sc, -900.0, 0.0, 620.0, 40.0, 0.13, '#e8c26a', phase=0.2)
        dog_ring(sc, 700.0, 60.0, 520.0, 200.0, -0.16, '#5a4a3c', phase=0.6)
    return build


def crowd(seed=251):
    """A beach crowd for 150 m and beyond, as a band of colour 44 m long: 24 people (1.6 times life size, hot swimwear
    and shirts) in eight groups that hop out of step, arms up; six big striped parasols; a striped ball thrown up and
    caught; three dogs running the length of the band."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        pc = ((RED, WHITE), (YEL, HOTPINK), (SKY, WHITE), (ORANGE, YEL), (VIOLET, MINT), (HOTPINK, WHITE))
        for k in range(6):
            parasol(g, -1900.0 + k * 760.0 + r.uniform(-80, 80), 250.0 + r.uniform(-60, 120), pc[k][0], pc[k][1])
        for k in range(5):                                                   # towels: slabs of colour on the sand
            g.box(HOT[(k * 3) % 8], (-1700.0 + k * 850.0, -330.0, 5), (300, 150, 10))
        groups = [P.motion(phase=j / 8.0 + r.uniform(-0.04, 0.04), rate=r.uniform(0.9, 1.25), bob=r.uniform(26, 44)) for j in range(8)]
        order = list(range(24))
        r.shuffle(order)
        for i in range(24):
            x = -2100.0 + i * 182.0 + r.uniform(-40, 40)
            y = (-130.0 if i % 2 else 70.0) + r.uniform(-50, 50)
            far_person(sc, x, y, groups[order[i] % 8], s=r.uniform(1.45, 1.75) if i % 6 != 4 else 1.0, top=HOT[(i * 5 + order[i]) % 8],
                       bottom=r.choice((NAVY, WHITE, RED, '#3a5fa8', YEL)), skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS),
                       arms=('vee', 'up', 'vee', 'down')[order[i] % 4], yaw=r.uniform(-20, 20))
        b = sc.part(P.motion(bob=820.0, rate=0.42, phase=0.1))                # the ball
        b.sphere(WHITE, (300.0, -60.0, 360.0), 130, steps=(4, 6))
        b.box(RED, (300.0, -60.0, 360.0), (268, 268, 60))
        for k in range(5):                                                   # feather banners: tall blades of colour that lean in the wind
            fx, fy = -2000.0 + k * 1000.0 + r.uniform(-100, 100), 420.0
            f = sc.part(P.motion(swing=7.0, pivot=(fx, fy, 0.0), axis=90.0, rate=r.uniform(0.3, 0.45), phase=r.random()))
            f.box(WHITE, (fx, fy, 400), (10, 10, 800))
            f.box(HOT[(k * 3 + 1) % 8], (fx + 55, fy, 520), (100, 8, 520))
            f.box(HOT[(k * 3 + 4) % 8], (fx + 55, fy, 300), (100, 10, 90))
        dog_run(sc, -800.0, -420.0, 5200.0, 620.0, '#e8c26a', phase=0.1)
        dog_run(sc, -1300.0, -470.0, 5200.0, 560.0, '#fff6ea', s=1.7, phase=0.5)
        dog_run(sc, 900.0, 430.0, 5200.0, -500.0, '#5a4a3c', phase=0.8)
    return build


JOBS = [
    ('SM_FarFlock', flock(201), dict(far=True, grow=(600.0, 600.0, 200.0), grow_down=(600.0, 600.0, 200.0))),
    ('SM_FarPelicans', pelicans(211), dict(far=True, grow=(21000.0, 100.0, 200.0), grow_down=(21000.0, 100.0, 100.0))),
    ('SM_FarDolphins', dolphins(221), dict(far=True, grow=(11000.0, 100.0, 500.0), grow_down=(11000.0, 100.0, 50.0))),
    ('SM_FarSurf', surf(231), dict(far=True, grow=(8000.0, 100.0, 100.0), grow_down=(8000.0, 100.0, 50.0))),
    ('SM_FarKites', kites(241), dict(far=True, grow=(700.0, 100.0, 300.0), grow_down=(700.0, 100.0, 0.0))),
    ('SM_FarCrowd', crowd(251), dict(far=True, grow=(2700.0, 60.0, 900.0), grow_down=(2700.0, 60.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
