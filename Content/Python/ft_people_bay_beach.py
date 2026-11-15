"""FULL THROTTLE (Unreal) the population, level 0: Bayshore, the beach and the sea (the second half of the kit).

ft_people_bay.py has the kit's header, its colours and helpers, the pieces by the road and the list of jobs; this
file has the lifeguard tower, the bandstand, the open beach, the pier's people and what lives in the sky and on the sea.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box
import ft_people_bay as B
from ft_people_bay import WOOD, WOOD_D, WHITE, CREAM, CORAL, TEAL, YEL, HOTPINK, MINT, NAVY, RED, HOT, SWIM, LIFT, CHEER, beach_look, walker, dog


# ------------------------------------------------------------------------------------------ by the road: raised
def lifeguard(seed=45):
    """The lifeguard tower: a hut on stilts, red and yellow. One guard sweeps the beach with binoculars, the other
    waves the chase on with a flag; rescue boards lean on the stilts; the flag on the roof swings in the wind."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        D = 235.0                                                            # the deck
        for sx in (-1, 1):
            for sy in (-1, 1):
                tilt_box(g, WOOD_D, (sx * 150, sy * 130, -30), (sx * 120, sy * 110, D), 14, 14)
        tilt_box(g, WOOD_D, (-150, -130, 40), (150, -130, D - 30), 8, 8)
        g.box(WOOD, (0, 0, D - 8), (330, 300, 16))
        g.box(WHITE, (0, 60, D + 95), (250, 170, 190))                            # the hut, a red band, a dark window
        g.box(RED, (0, 60, D + 150), (254, 174, 34))
        g.box(NAVY, (0, -27, D + 100), (120, 4, 70))
        g.box(YEL, (0, 40, D + 200), (330, 290, 14), rot=(0, 0, -8))             # the roof, tipped to the road
        for sx in (-1, 1):                                                   # the rail round the front
            g.box(WHITE, (sx * 160, -60, D + 50), (8, 8, 100))
            g.box(WHITE, (sx * 160, -100, D + 96), (8, 100, 8))
        g.box(WHITE, (0, -146, D + 96), (328, 8, 8))
        g.box(WHITE, (0, -146, D + 50), (328, 6, 6))
        tilt_box(g, WOOD, (230, 150, -20), (150, 40, D - 8), 80, 10)             # the ramp
        tilt_box(g, YEL, (-215, 20, 0), (-170, 20, 270), 52, 9)                  # two rescue boards against the stilts
        tilt_box(g, RED, (-250, -40, 0), (-185, -40, 250), 52, 9)
        g.box(WHITE, (0, 60, D + 300), (8, 8, 200))                              # the flag on the roof
        fl = sc.part(P.motion(swing=16.0, pivot=(0.0, 60.0, D + 370.0), axis=P.UP, rate=0.8))
        fl.box(RED, (52, 60, D + 384), (96, 5, 30))
        fl.box(YEL, (52, 60, D + 354), (96, 5, 30))
        guard = dict(top=YEL, bottom=RED, wear='shorts', sleeves='short', hat='cap', hat_col=RED)

        def binoculars(m, h, side):
            m.box(NAVY, (h[0], h[1] - 12, h[2] + 2), (26, 20, 13))
        P.figure(sc, -70, -90, z=D, yaw=-10, arm_l='face', arm_r='face', hold_r=binoculars, turn=38, rate=0.16, **P.look(r, **guard))
        P.figure(sc, 80, -92, z=D, yaw=15, arm_r='up', arm_l='hip', hold_r=P.hold_flag(RED, WHITE, 70), swing_r=24, rate=1.1, react=True,
                 **P.look(r, **guard))
        # down on the sand: a swimmer asking the way, a child hopping
        P.figure(sc, 260, -150, z=LIFT, yaw=-50, arm_r='point', swing_r=10, axis_r='x', rate=0.6, **beach_look(r, wear='swim'))
        P.figure(sc, 330, -90, z=LIFT, yaw=-30, bob=8, rate=1.5, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, **beach_look(r, build='kid', s=0.66))
    return build


def _guitar(col=RED):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 30, h[1] - 10, h[2] - 4), (44, 10, 34))
            tilt_box(m, WOOD_D, (h[0] - 30, h[1] - 10, h[2]), (h[0] - 92, h[1] - 10, h[2] + 26), 7, 7)
    return f


def bandstand(seed=47):
    """A beach band on a small round stand under a canopy, and the people dancing on the deck before it. The
    drummer's arms go, the guitarists rock, the dancers twist and hop out of step."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        D = 125.0
        g.cyl(WOOD, (0, 90, -30), 250, D + 30, steps=8)
        g.cyl(HOTPINK, (0, 90, D - 26), 256, 14, steps=8)
        g.box(WOOD, (0, -230, D / 2.0 - 40), (760, 240, D - 30 + 50))                # the dance deck, a step lower
        g.box(TEAL, (0, -351, D - 46), (764, 3, 30))
        for k in range(6):
            a = math.radians(k * 60 + 30)
            g.box(WHITE, (math.cos(a) * 225, 90 + math.sin(a) * 225, D + 150), (10, 10, 300))
        g.cyl(WHITE, (0, 90, D + 296), 286, 12, steps=8)                          # the canopy: a white rim, a coral cone, a gold knob
        g.cyl(CORAL, (0, 90, D + 306), 276, 96, steps=8, top_r=20)
        g.cyl(YEL, (0, 90, D + 400), 28, 26, steps=6)
        band = dict(top=WHITE, bottom=NAVY, wear='trousers', sleeves='short', hat='sun', hat_col=YEL)
        P.figure(sc, -110, 20, z=D, yaw=14, arm_l='hold', arm_r='hold', hold_r=_guitar(RED), rock=6, rate=1.0, **P.look(r, **band))
        P.figure(sc, 110, 20, z=D, yaw=-14, arm_l='hold', arm_r='hold', hold_r=_guitar(TEAL), rock=6, rate=1.0, phase=0.5, **P.look(r, **band))
        P.figure(sc, 0, -40, z=D, arm_r='face', arm_l='up', swing_l=18, rate=1.0, bob=5, **P.look(r, top=HOTPINK, bottom=WHITE, wear='dress', hair='afro'))
        g.cyl(RED, (0, 150, D), 34, 40, steps=8)                                  # the drums
        g.cyl(WHITE, (0, 150, D + 40), 35, 3, steps=8)
        g.cyl(YEL, (-60, 130, D + 84), 26, 3, steps=6)
        g.box(NAVY, (-60, 130, D + 42), (4, 4, 84))
        P.figure(sc, 0, 205, z=D - 6, legs='sit', seat=46, arm_l='fwd', arm_r='fwd', swing_l=24, swing_r=-24, axis_l='x', axis_r='x', rate=2.0,
                 **P.look(r, **band))
        for i in range(7):
            x = -320 + i * 107 + r.uniform(-14, 14)
            y = -200 - (i % 2) * 80 + r.uniform(-10, 10)
            d = beach_look(r)
            if i % 2:
                P.figure(sc, x, y, z=D - 28, yaw=r.uniform(-40, 40), arm_l='vee', arm_r='vee', turn=32, rate=1.0, bob=7, phase=r.random(), detail=1, **d)
            else:
                P.figure(sc, x, y, z=D - 28, yaw=r.uniform(-40, 40), arm_l='wave', arm_r='hip', swing_l=-22, rate=1.0, bob=10, phase=r.random(), detail=1, **d)
    return build


# ------------------------------------------------------------------------------------------ middle distance: the beach
def _parasol(g, x, y, z, col, r=165.0, h=235.0):
    g.cyl(WHITE, (x, y, z - 30), 5, h + 30, steps=6)
    g.cyl(col, (x, y, z + h - 20), r, 48, steps=8, top_r=8)
    g.cyl(WHITE, (x, y, z + h - 24), r + 2, 5, steps=8)


def _lying(sc, r, x, y, z, yaw, towel):
    """Someone lying on a towel (drawn here: the figure builder stands and sits)."""
    m = sc.part(P.motion(bob=-1.2, rate=0.22, phase=r.random()), (x, y, z), yaw)
    skin, sw = r.choice(P.SKINS), r.choice(SWIM)
    m.box(towel, (0, 0, 3), (96, 210, 6))
    m.box(WHITE, (0, 0, 3.5), (96, 30, 6))
    m.box(skin, (0, 52, 18), (40, 84, 20))            # legs
    m.box(sw, (0, 2, 19), (44, 26, 23))
    m.box(skin if r.random() < 0.5 else r.choice(HOT), (0, -32, 20), (46, 52, 24))
    m.cyl(skin, (0, -80, 8), 20, 30, steps=6)
    m.box(r.choice(P.HAIRS), (0, -92, 22), (40, 22, 26))
    tilt_box(m, skin, (26, -36, 22), (40, -92, 14), 11, 11)


def beach_day(seed=61, variant=0):
    """A stretch of the open beach in use: parasols, people on towels, children and a ball; variant 0 has a kite up and
    a dog running rings, variant 1 an ice-cream cart with its queue and two people throwing a disc."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        cols = [HOTPINK, TEAL, YEL, CORAL, '#8f6bff', RED]
        for k, (x, y) in enumerate(((-520, 120), (120, 260), (560, -60))):
            _parasol(g, x, y, z, cols[(k + variant * 3) % 6])
            _lying(sc, r, x - 90, y + 10, z, r.uniform(-30, 30), cols[(k + 2) % 6])
            if k != 1:
                _lying(sc, r, x + 100, y - 20, z, r.uniform(150, 210), cols[(k + 4) % 6])
            g.box(cols[(k + 1) % 6], (x + 10, y + 120, z + 20), (44, 34, 40))        # a bag, a cool box
        # children round a beach ball that hops between them
        cx, cy = -120, -260
        for k in range(3):
            a = math.radians(k * 120 + 30)
            P.figure(sc, cx + math.cos(a) * 150, cy + math.sin(a) * 150, z=z, yaw=math.degrees(a) + 90, arm_l='up', arm_r='up', swing_l=14, swing_r=-14,
                     bob=14, rate=1.2, phase=k / 3.0, **beach_look(r, build='kid', s=r.uniform(0.62, 0.72)))
        ball = sc.part(P.motion(bob=170.0, rate=0.6))
        ball.sphere(RED, (cx, cy, z + 60), 34, steps=(5, 8))
        ball.sphere(WHITE, (cx, cy, z + 60), 35, scale=(1.0, 0.32, 1.0), steps=(4, 8))
        if variant == 0:
            # the kite: high over the group, on a line from the flyer's hand, swinging in the wind
            fx, fy = 380, 260
            P.figure(sc, fx, fy, z=z, yaw=-20, arm_r='up', arm_l='vee', rock=4, rate=0.3, **beach_look(r, top=WHITE))
            hand = (fx + 30.0, fy, z + 185.0)
            kite = sc.part(P.motion(swing=13.0, pivot=hand, axis=90.0, rate=0.3))
            kx, ky, kz = fx - 180.0, fy + 120.0, z + 900.0
            tilt_box(kite, WHITE, hand, (kx, ky, kz - 70), 4, 4)
            kite.box(HOTPINK, (kx, ky, kz), (150, 6, 150), rot=(45, 0, 0))
            kite.box(YEL, (kx, ky - 4, kz), (74, 4, 74), rot=(45, 0, 0))
            for j in range(3):
                kite.box((TEAL, YEL, HOTPINK)[j], (kx + 16 * (j % 2), ky, kz - 140 - j * 46), (30, 4, 22))
            # the dog runs rings round the children
            P.quadruped(sc, cx + 330, cy, z=z, yaw=180, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=30,
                        rate=2.6, orbit=(cx, cy, 0.16), body='#e8c26a', ears='#5a4a3c', snout='#22202a')
            P.figure(sc, 620, 180, z=z, yaw=200, arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.6, **beach_look(r))
        else:
            # the ice-cream cart: a striped box on wheels under a parasol, the seller, a queue of four
            ox, oy = 330, 280
            g.box(WHITE, (ox, oy, z + 62), (150, 84, 76))
            g.box(HOTPINK, (ox, oy - 43, z + 62), (150, 3, 30))
            for sx in (-1, 1):
                g.cyl(NAVY, (ox + sx * 58, oy - 46, z + 22), 22, 8, rot=(0, 0, 90), steps=8)
            _parasol(g, ox, oy + 30, z + 60, YEL, r=150, h=210)
            g.cyl('#e8b26a', (ox - 40, oy, z + 100), 6, 34, steps=6, top_r=16)
            g.sphere(HOTPINK, (ox - 40, oy, z + 146), 19, steps=(4, 6))
            P.figure(sc, ox + 10, oy + 80, z=z, arm_r='fwd', arm_l='hold', swing_r=10, axis_r='x', rate=0.7,
                     **P.look(r, top=WHITE, bottom=NAVY, hat='cap', hat_col=HOTPINK))
            for k in range(4):
                d = beach_look(r)
                if k == 1:
                    d.update(build='kid', s=0.66)
                P.figure(sc, ox - 20 + k * 30 + r.uniform(-10, 10), oy - 120 - k * 72, z=z, yaw=180 + r.uniform(-25, 25), arm_r='hold' if k % 2 else 'down',
                         arm_l='hip' if k == 2 else 'down', rate=0.35, bob=-1.5, rock=3 if k == 3 else 0, phase=r.random(), detail=1, **d)
            # two throwing a disc to and fro (it slides between their hands)
            for sx in (-1, 1):
                P.figure(sc, sx * 330 - 180, -420, z=z, yaw=-90.0 * sx, arm_r='fwd', arm_l='down', swing_r=30, axis_r='x', rate=0.4, phase=0.0 if sx > 0 else 0.5,
                         bob=6, detail=1, **beach_look(r))
            disc = sc.part(P.motion(swing=5.6, pivot=(-180.0, -420.0, -3000.0), axis=90.0, rate=0.4))
            disc.cyl(YEL, (-180, -420, z + 150), 22, 5, steps=8)
    return build


def surf_walk(seed=71):
    """Surfers carrying their boards along the beach to the break, in a line; two more stood by their boards."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        L = 2400.0
        bcols = (YEL, HOTPINK, MINT, '#2ee6ff', CORAL, WHITE)
        for i in range(5):
            x = -1000 + i * 420 + r.uniform(-50, 50)
            d = beach_look(r, wear='swim' if i % 2 else 'shorts', top=('#22202a', '#2b2350', RED, '#22202a', '#3a5fa8')[i], sleeves=('long', 'none')[i % 2], hat=None)
            walker(sc, r, x, (i % 2) * 70 - 20, 1, L, speed=120, z=z, look=d, arm_r='down', swing_r=0,
                   hold_r=P.hold_board(bcols[i], bcols[(i + 3) % 6], 200))
        dog(sc, 300, 130, 1, L, speed=120, z=z, body=WHITE)
        for k, x in enumerate((-300, 260)):
            g.box(bcols[k + 4], (x + 60, 300, z + 115), (50, 8, 230), rot=(0, 0, 8))
            g.box(bcols[k + 1], (x + 60, 296, z + 115), (10, 4, 200), rot=(0, 0, 8))
            P.figure(sc, x, 290, z=z, yaw=40 - 80 * k, arm_r='point' if k else 'hip', arm_l='hip', swing_r=9 if k else 0, axis_r='x', rate=0.5, bob=-1.5,
                     **beach_look(r, wear='swim', hat=None))
    return build


PIER_DECK = 630.0       # ft_dress_bay.pier: the deck over the piece's own origin; its rails are at x = +-290


def anglers(seed=81):
    """The people of a pier, drawn in the dressing pier's own space (a Set stands it on the pier: same place, no
    offsets): anglers along both rails over the first 90 m, their rods dipping, people who have turned to watch the
    chase, gulls on the rail and over the deck. The pier's land end is at y = -15000."""
    def build(sc):
        r = random.Random(seed)
        z = PIER_DECK
        y0 = -14300.0
        for i in range(9):
            sx = 1 if i % 2 else -1
            y = y0 + 500 + i * 950 + r.uniform(-150, 150)
            d = P.look(r, tops=HOT, wear=r.choice(('shorts', 'trousers')), hat=r.choice(('sun', 'cap', 'cowboy')), hat_col=r.choice((CREAM, YEL, RED, WHITE)))
            if i in (2, 7):                                                  # turned round to watch the road
                d.update(CHEER[(i * 3) % len(CHEER)])
                P.figure(sc, sx * 150, y, z=z, yaw=r.uniform(-20, 20), phase=r.random(), detail=1, **d)
                continue
            P.figure(sc, sx * 245, y, z=z, yaw=90.0 * sx, arm_l='hold', arm_r='fwd', hold_r=P.hold_rod('#5a4a3c', 330), swing_r=5, axis_r='x',
                     rate=r.uniform(0.25, 0.4), phase=r.random(), detail=1, **d)
            if i % 3 == 0:
                sc.still().box((RED, TEAL, YEL)[i % 3 - 1], (sx * 200, y + 70, z + 18), (40, 30, 36))    # a bucket, a tackle box
        for k in range(3):                                                   # gulls stood on the rail
            P.bird(sc, (290, -290, 290)[k], y0 + 2000 + k * 2600, z + 118, heading=(180, 0, 200)[k], span=110, flap=0.0, bob=4, rate=0.9, phase=k / 3.0)
        for k in range(4):                                                   # and wheeling over the deck
            a = math.radians(k * 90 + 20)
            R = 500 + k * 160
            cy = y0 + 4200
            P.bird(sc, math.cos(a) * R, cy + math.sin(a) * R, z + 500 + k * 170, heading=math.degrees(a) + 90, span=250, flap=30, rate=1.5,
                   phase=k / 4.0, orbit=(0, cy, 0.07 + 0.01 * k))
    return build


# ------------------------------------------------------------------------------------------ far: the sky and the sea
def gulls(seed=91):
    """A flock of gulls wheeling over the beach: big (2.5 to 3.4 m across) and always moving, for 60 m out and beyond."""
    def build(sc):
        r = random.Random(seed)
        for k in range(9):
            a = math.radians(k * 40 + r.uniform(-15, 15))
            R = r.uniform(350, 1500)
            P.bird(sc, math.cos(a) * R, math.sin(a) * R, r.uniform(900, 2300), heading=math.degrees(a) + 90, span=r.uniform(250, 340), flap=30,
                   rate=r.uniform(1.2, 1.7), phase=r.random(), orbit=(0, 0, r.uniform(0.05, 0.085)))
    return build


def surf_riders(seed=95):
    """Afloat: surfers riding along the bar, each on a board with a wedge of foam behind, two sat on their boards
    waiting, and a kite-surfer under a big arc of kite. Drawn at life size and placed large (the rule scales it)."""
    def build(sc):
        r = random.Random(seed)
        L = 7000.0
        bcols = (YEL, HOTPINK, MINT, WHITE)
        for i in range(4):
            x, y = -2600 + i * 1500 + r.uniform(-200, 200), r.uniform(-500, 500)
            sp = r.uniform(520, 700)
            lk = P.look(r, tops=('#22202a', RED, '#2b2350', HOTPINK), bottoms=('#22202a',), wear='swim', sleeves='long', hair='short', s=1.0)
            P.figure(sc, x, y, z=14, yaw=90 + 60, legs='wide', arm_l='out', arm_r='out', rock=9, rate=0.35, phase=r.random(), travel=(L, sp), detail=1, **lk)
            b = sc.part(P.motion(base=(x, y), travel=(L, sp / L), bob=-6, rate=0.35))
            b.box(bcols[i], (x, y, 8), (230, 52, 10))
            tilt_box(b, WHITE, (x - 110, y, 4), (x - 420, y, 30), 120, 26)       # the foam it leaves
            tilt_box(b, WHITE, (x - 110, y + 60, 2), (x - 360, y + 140, 16), 70, 16)
        for k in range(2):
            x, y = 600 + k * 500, 900 + k * 200
            b = sc.part(P.motion(bob=-14, rate=0.2, phase=k * 0.4))
            b.box(bcols[k + 1], (x, y, 6), (230, 52, 10))
            P.figure(sc, x, y, z=10 - 46, yaw=90, legs='sit', seat=46, arm_l='down', arm_r='hip', bob=-14, rate=0.2, phase=k * 0.4, detail=1,
                     **P.look(r, tops=('#22202a',), wear='swim', sleeves='long'))
        # the kite-surfer
        x, y, sp = -400, -1100, 900.0
        P.figure(sc, x, y, z=14, yaw=90 + 50, legs='wide', arm_l='up', arm_r='up', rock=6, rate=0.3, travel=(L, sp), detail=1,
                 **P.look(r, tops=(HOTPINK,), wear='swim', sleeves='long', hat='helmet', hat_col=YEL))
        k = sc.part(P.motion(base=(x, y), travel=(L, sp / L), swing=7.0, pivot=(x, y, 190.0), axis=0.0, rate=0.3))
        k.box(WHITE, (x, y, 8), (170, 50, 10))
        tilt_box(k, WHITE, (x + 20, y, 190), (x + 700, y - 100, 1500), 5, 5)
        for j in range(5):
            a = math.radians(-50 + j * 25)
            k.box((HOTPINK, YEL)[j % 2], (x + 700, y - 100 + math.sin(a) * 420, 1500 + math.cos(a) * 260 - 260), (150, 190, 14), rot=(0, 0, math.degrees(a)))
    return build


def dolphins(seed=97):
    """Afloat: a school of dolphins leaping. Each swings about a point under the water, so it rises out of the sea
    in an arc and goes under again, one after another."""
    def build(sc):
        r = random.Random(seed)
        GREY, PALE = '#7a8fb8', '#d7e3f0'
        for i in range(6):
            x, y = -1500 + i * 600 + r.uniform(-150, 150), r.uniform(-500, 500)
            m = sc.part(P.motion(swing=78.0, pivot=(x, y, -170.0), axis=90.0, rate=r.uniform(0.22, 0.3), phase=r.random()))
            zt = 150.0                                                       # the top of its leap, over the water
            tilt_box(m, GREY, (x - 110, y, zt - 14), (x + 70, y, zt), 62, 58)
            tilt_box(m, GREY, (x + 70, y, zt), (x + 150, y, zt - 20), 40, 38)       # the head, the beak
            tilt_box(m, GREY, (x + 150, y, zt - 20), (x + 196, y, zt - 34), 16, 14)
            m.box(PALE, (x - 10, y, zt - 34), (190, 44, 16))
            tilt_box(m, GREY, (x - 20, y, zt + 26), (x - 62, y, zt + 78), 12, 34)   # the fin
            tilt_box(m, GREY, (x - 110, y, zt - 14), (x - 200, y, zt - 40), 34, 30)
            m.box(GREY, (x - 222, y, zt - 44), (40, 120, 10))                    # the flukes
            sp = sc.part(P.motion(bob=26.0, rate=0.6, phase=r.random()))
            sp.cyl(WHITE, (x + 320, y, -4), 60, 10, steps=6, top_r=40)
    return build


JOBS = [
    ('SM_GuardTower', lifeguard(45)),
    ('SM_Bandstand', bandstand(47)),
    ('SM_BeachDay_A', beach_day(61, 0), dict(grow=(120.0, 120.0, 120.0))),
    ('SM_BeachDay_B', beach_day(62, 1), dict(grow=(320.0, 60.0, 60.0), grow_down=(320.0, 60.0, 0.0))),
    ('SM_SurfWalk', surf_walk(71)),
    ('SM_Anglers', anglers(81), dict(far=True, grow=(400.0, 400.0, 100.0), grow_down=(400.0, 400.0, 0.0))),
    ('SM_Gulls', gulls(91), dict(far=True, grow=(900.0, 900.0, 100.0), grow_down=(900.0, 900.0, 0.0))),
    ('SM_SurfRiders', surf_riders(95), dict(far=True, grow=(200.0, 200.0, 200.0), grow_down=(200.0, 200.0, 20.0))),
    ('SM_Dolphins', dolphins(97), dict(far=True, grow=(400.0, 100.0, 100.0), grow_down=(400.0, 100.0, 200.0))),
]
