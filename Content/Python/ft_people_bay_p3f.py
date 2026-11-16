"""FULL THROTTLE (Unreal) the population, pass 3, Bayshore, builder p3f: MORE OF THE FILM SHOOT.

Bayshore is a shoot (ft_people_crew.py has the first unit's kit, wardrobe Sun). This file adds the units a big
shoot brings with it, each ONE mesh, each RAISED on its own scaffold, rostrum or riser so that it clears the road's
barrier (the beach lies a metre under the road), each marked as crew by the pink chest band, the pink-and-white
tape and the pink-and-white canvas:

    SM_P3F_SecondUnit_A   a camera tower: the camera pans on a scaffold deck under a parasol, operator and assistant
    SM_P3F_SecondUnit_B   a long-lens position on a low rostrum: operator, the second-unit director on a stool
    SM_P3F_DroneTeam_A    a drone hovering over its landing pad (it floats, well off the road), pilot and spotter
    SM_P3F_DroneTeam_B    two drones circling a mast, two pilots, a tracking dish
    SM_P3F_StuntRig       a wire rig: a goalpost truss, a performer floating on the wire over the air bag, the rigger
    SM_P3F_Extras_A       the extras' holding area: a riser under a canopy, ten extras, the assistant director
    SM_P3F_Extras_B       extras queueing on a deck for the wardrobe rail
    SM_P3F_MakeUp         the make-up tent: actors in tall chairs, make-up, hair, the costume rail
    SM_P3F_PressPen       the press on a two-step riser: photographers, a television camera, one up a ladder
    SM_P3F_Reflector      grips with a great white bounce frame that tilts, a wind machine

Every piece is TALL (over 2 m): 24 m or more from the road's centre line. No glow anywhere, nothing blinks.
Movement by the figure builder's motion values only. No words, no marks.

Run in the editor:  import ft_people_bay_p3f as K; K.run()
"""
import math
import random
import ft_people as P
import ft_people_crew as C
from ft_desert import tilt_box

DIR = '/Game/People/Bayshore'
W = C.WARDROBES['Sun']
PINK, WHITE, DARK, GREY, YEL, STEEL, WOOD, CANVAS = C.PINK, C.WHITE, C.DARK, C.GREY, C.YEL, C.STEEL, C.WOOD, C.CANVAS
ORANGE, TEAL, BLUE, SKY, RED, LIME, VIOLET = '#ff7a3c', '#2ee6ff', '#2e6bff', '#57c7ff', '#ff2d3c', '#b8ff5a', '#8f6bff'
BRIGHT = (PINK, TEAL, YEL, '#7dffb0', ORANGE, VIOLET, RED, SKY, LIME, '#f4a6ff')


# ------------------------------------------------------------------------------------------ furniture
def ptape(m, length, y, x=0.0, z=0.0, posts=3):
    """The crew's pink-and-white tape, cheaper than the kit's: posts, an upper run in blocks, a lower pink run."""
    for k in range(posts):
        px = x - length / 2.0 + k * length / (posts - 1)
        m.box(WHITE, (px, y, z + 52), (9, 9, 104))
    n = max(int(round(length / 130.0)), 2)
    for k in range(n):
        m.box(PINK if k % 2 else WHITE, (x - length / 2.0 + (k + 0.5) * length / n, y, z + 92), (length / n, 4, 11))
    m.box(PINK, (x, y, z + 58), (length, 4, 7))


def stage(m, x, y, w, d, h, col=WOOD, brace=True):
    """A scaffold deck w by d, its top h over the ground, on four legs, a pink skirt along its road side."""
    m.box(col, (x, y, h - 6), (w, d, 12))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(STEEL, (x + sx * (w / 2.0 - 8), y + sy * (d / 2.0 - 8), (h - 12) / 2.0), (10, 10, h - 12))
    m.box(PINK, (x, y - d / 2.0 - 1, h - 24), (w, 4, 22))
    m.box(WHITE, (x, y - d / 2.0 - 1, h - 46), (w, 4, 12))
    if brace and h > 90:
        tilt_box(m, STEEL, (x - w / 2.0 + 8, y - d / 2.0 + 8, 12), (x + w / 2.0 - 8, y - d / 2.0 + 8, h - 60), 6, 6)


def rail(m, x, y, w, z, h=100.0):
    m.box(WHITE, (x, y, z + h), (w, 6, 6))
    for sx in (-1, 0, 1):
        m.box(WHITE, (x + sx * (w / 2.0 - 4), y, z + h / 2.0), (6, 6, h))


def canopy(m, x, y, w, d, z0, h, n=6, cols=(PINK, WHITE)):
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(STEEL, (x + sx * (w / 2.0 - 6), y + sy * (d / 2.0 - 6), z0 + h / 2.0), (7, 7, h))
    for k in range(n):
        m.box(cols[k % 2], (x - w / 2.0 + (k + 0.5) * w / n, y, z0 + h + 5), (w / n, d + 30, 10))


def parasol(m, x, y, z0, h=250.0, r=150.0, col=PINK, tip=WHITE):
    m.box(STEEL, (x, y, z0 + h / 2.0), (6, 6, h))
    m.cyl(col, (x, y, z0 + h - 6), r, 34, steps=8, top_r=12)
    m.cyl(tip, (x, y, z0 + h + 28), 12, 12, steps=6)


def pennant(sc, x, y, z0, h, col=PINK, phase=0.0, w=110.0):
    """A mast with a pennant that swings in the breeze."""
    g = sc.still()
    g.box(STEEL, (x, y, z0 + h / 2.0), (6, 6, h))
    f = sc.part(P.motion(swing=16.0, pivot=(x, y, z0 + h - 30), axis=P.UP, rate=0.8, phase=phase))
    f.box(col, (x + w / 2.0 + 3, y, z0 + h - 30), (w, 4, 50))


def tripod_at(m, x, y, z0, top=128.0):
    for a in (90, 210, 330):
        tilt_box(m, STEEL, (x, y, z0 + top), (x + math.cos(math.radians(a)) * 50, y + math.sin(math.radians(a)) * 50, z0), 5, 5)
    m.box(DARK, (x, y, z0 + top - 4), (16, 16, 10))


def stool(m, x, y, z0, seat=72.0):
    for sx in (-1, 1):
        tilt_box(m, WOOD, (x + sx * 22, y - 18, z0), (x + sx * 22, y + 18, z0 + seat), 5, 5)
        tilt_box(m, WOOD, (x + sx * 22, y + 18, z0), (x + sx * 22, y - 18, z0 + seat), 5, 5)
    m.box(CANVAS, (x, y, z0 + seat), (52, 44, 5))


def drone(m, x, y, z, s=1.0):
    """A camera drone, drawn large to read: a dark body with a pink back, four arms, four rotor discs, the camera
    slung under it, skids. No light on it."""
    m.box(DARK, (x, y, z), (70 * s, 70 * s, 22 * s))
    m.box(PINK, (x, y, z + 12 * s), (42 * s, 42 * s, 4 * s))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, GREY, (x, y, z), (x + sx * 80 * s, y + sy * 80 * s, z + 8 * s), 9 * s, 9 * s)
            m.cyl(WHITE, (x + sx * 80 * s, y + sy * 80 * s, z + 12 * s), 34 * s, 4 * s, steps=8)
        m.box(GREY, (x + sx * 30 * s, y, z - 24 * s), (6 * s, 64 * s, 6 * s))
    m.box(GREY, (x, y - 10 * s, z - 20 * s), (22 * s, 26 * s, 20 * s))


def hold_controller():
    def f(m, h, side):
        if side < 0:
            return
        m.box(GREY, (h[0] - 22, h[1] - 8, h[2] + 4), (56, 20, 10))
        m.box(DARK, (h[0] - 44, h[1] - 6, h[2] + 22), (4, 4, 30))
    return f


def hold_brush(col=PINK):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 10, h[2] + 4), (4, 26, 4))
        m.box(col, (h[0], h[1] - 26, h[2] + 4), (8, 10, 8))
    return f


def extra(r, **fixed):
    """An extra waiting for the call: a holidaymaker out of the town's own cast (no crew band)."""
    d = P.look(r, tops=BRIGHT, wear=r.choice(('shorts', 'shorts', 'skirt', 'dress', 'trousers', 'swim')),
               sleeves=r.choice(('short', 'short', 'none')), hat=r.choice((None, None, 'sun', 'cap')), hat_col=r.choice(BRIGHT))
    d.update(fixed)
    return d


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.5),
    dict(arm_r='wave', swing_r=26, rate=1.2, bob=3),
    dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=9, rate=1.3),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4),
    dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7),
    dict(arm_l='wave', arm_r='hip', swing_l=22, rate=1.0, bob=4),
)


# ------------------------------------------------------------------------------------------ the pieces
def second_unit_a(seed=11):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 230.0
        stage(g, 0, 60, 340, 260, H)
        rail(g, 0, 184, 340, H)
        for sy in (-1, 1):                                   # the ladder up its side
            g.box(STEEL, (186, 60 + sy * 24, H / 2.0), (6, 6, H))
        for k in range(5):
            g.box(STEEL, (186, 60, 28 + k * 46), (6, 54, 6))
        tripod_at(g, 0, 20, H)
        pan = dict(swing=24.0, pivot=(0.0, 20.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.12, phase=0.3, **pan))
        C.film_camera(cam, 0, 20, H + 128)
        P.figure(sc, 0, 104, z=H, arm_l='push', arm_r='push', rate=0.12, phase=0.3, sway=pan, **C.member(r, W, hat='headset', hat_col=DARK))
        P.figure(sc, -112, 70, z=H, yaw=-25, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), **C.member(r, W))
        g = sc.still()
        parasol(g, 96, 130, H, h=270, r=170)
        # on the ground: the clapper loader and a grip with a case
        P.figure(sc, -250, -80, yaw=20, detail=1, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.35, bob=-1.5, phase=r.random(), **C.member(r, W))
        P.figure(sc, 280, -40, yaw=-30, detail=1, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=YEL), rate=0.3, bob=-1.0, phase=r.random(), **C.member(r, W))
        g = sc.still()
        C.case(g, -250, 60)
        ptape(g, 680, -190)
        pennant(sc, -164, 184, H, 260, PINK, 0.1)
    return build


def second_unit_b(seed=13):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 150.0
        stage(g, 0, 40, 480, 230, H)
        rail(g, 0, 150, 480, H)
        g.box(WOOD, (-270, 40, 50), (60, 120, 100))            # a step up
        tripod_at(g, -80, 10, H)
        pan = dict(swing=18.0, pivot=(-80.0, 10.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.1, phase=0.6, **pan))
        C.film_camera(cam, -80, 10, H + 128)
        cam.cyl(STEEL, (-80, -52, H + 145), 14, 74, rot=(0, 0, 90), steps=8, top_r=22)     # the long lens
        P.figure(sc, -80, 92, z=H, arm_l='push', arm_r='push', rate=0.1, phase=0.6, sway=pan, **C.member(r, W, hat='cap', hat_col=DARK))
        P.figure(sc, -176, 20, z=H, yaw=-30, arm_l='fwd', rate=0.3, bob=-1.0, phase=r.random(), **C.member(r, W, hat='headset', hat_col=DARK))
        g = sc.still()
        stool(g, 120, 40, H)
        P.figure(sc, 120, 40, z=H + 6, yaw=10, legs='sit', seat=72, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12, axis_l='x',
                 rate=0.5, phase=r.random(), **C.member(r, W, hat='sun', hat_col=WHITE))
        P.figure(sc, 206, 70, z=H, yaw=-20, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.2, phase=r.random(), **C.member(r, W))
        g = sc.still()
        parasol(g, 30, 120, H, h=260, r=180, col=WHITE, tip=PINK)
        C.case(g, 300, 20)
        C.case(g, 306, 16, z=38, w=52, h=30, col=YEL)
        ptape(g, 700, -170)
        pennant(sc, 236, 150, H, 250, PINK, 0.5)
    return build


def drone_team_a(seed=17):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 150.0
        stage(g, -190, 60, 300, 230, H)
        rail(g, -190, 170, 300, H)
        P.figure(sc, -240, 50, z=H, yaw=-20, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.0, phase=r.random(),
                 **C.member(r, W, hat='cap', hat_col=DARK))
        P.figure(sc, -130, 70, z=H, yaw=-35, arm_r='up', swing_r=10, axis_r='x', arm_l='face', rate=0.5, phase=r.random(), **C.member(r, W, hat='sun', hat_col=WHITE))
        g = sc.still()
        # the landing pad, a spare drone on it
        g.box(ORANGE, (220, 50, 4), (260, 260, 8))
        g.box(WHITE, (220, 50, 9), (260, 44, 3))
        g.box(WHITE, (220, 50, 9), (44, 260, 3))
        drone(g, 150, 120, 36, 0.8)
        C.case(g, 40, 150, col=YEL)
        # the drone in the air over the pad: it floats. Fix round: drawn at 0.9 (it was 1.5, 3.4 m across: a dark rotorcraft
        # beside a road with gunships on it) and it no longer turns its nose from side to side (a bob with a yaw swing is
        # how a pickup idles)
        d = sc.part(P.motion(bob=-45.0, rate=0.22, phase=0.1))
        drone(d, 220, 50, 640, 0.9)
        P.figure(sc, 60, -20, yaw=-60, detail=1, arm_r='face', arm_l='hip', rate=0.3, bob=-1.0, phase=r.random(), **C.member(r, W))
        g = sc.still()
        ptape(g, 760, -160)
        pennant(sc, -336, 170, H, 250, PINK, 0.3)
    return build


def drone_team_b(seed=19):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 150.0
        stage(g, 0, 40, 400, 220, H)
        rail(g, 0, 146, 400, H)
        # the mast the drones circle, a pennant at its head
        g.box(STEEL, (0, 120, 400), (10, 10, 800))
        g.box(DARK, (0, 120, 12), (70, 70, 24))
        f = sc.part(P.motion(swing=16.0, pivot=(0.0, 120.0, 770.0), axis=P.UP, rate=0.8))
        f.box(PINK, (62, 120, 770), (116, 4, 54))
        for k, (dx, z) in enumerate(((270.0, 560.0), (-270.0, 700.0))):
            d = sc.part(P.motion(base=(0.0, 120.0), travel=(-1.0, 0.06), bob=-18.0, rate=0.3, phase=0.25 + 0.4 * k))
            drone(d, dx, 120, z, 0.9)      # fix round: 0.9, it was 1.4 (see drone_team_a)
        P.figure(sc, -110, 30, z=H, yaw=-25, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.0, phase=r.random(),
                 **C.member(r, W, hat='cap', hat_col=DARK))
        P.figure(sc, 20, 50, z=H, yaw=20, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.0, phase=r.random(),
                 **C.member(r, W, hat='headset', hat_col=DARK))
        P.figure(sc, 140, 30, z=H, yaw=30, arm_r='up', swing_r=10, axis_r='x', arm_l='hip', rate=0.5, phase=r.random(), **C.member(r, W))
        g = sc.still()
        # the tracking dish on its tripod, turning to and fro
        tripod_at(g, -260, 0, 0, top=150)
        dish = sc.part(P.motion(swing=40.0, pivot=(-260.0, 0.0, 0.0), axis=P.UP, rate=0.06))
        dish.box(WHITE, (-260, -6, 190), (80, 8, 80))
        dish.box(DARK, (-260, -22, 190), (8, 30, 8))
        g = sc.still()
        C.case(g, 270, 30, col=YEL)
        ptape(g, 720, -150)
    return build


def stunt_rig(seed=23):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        # the goalpost: two yellow towers banded dark, braced, a beam across their heads
        for sx in (-1, 1):
            g.box(YEL, (sx * 340, 60, 380), (26, 26, 760))
            for k in range(3):
                g.box(DARK, (sx * 340, 60, 150 + k * 220), (29, 29, 36))
            tilt_box(g, STEEL, (sx * 340, 220, 0), (sx * 340, 60, 520), 8, 8)
            g.box(DARK, (sx * 340, 60, 8), (90, 90, 16))
        g.box(YEL, (0, 60, 772), (720, 26, 26))
        g.box(DARK, (0, 60, 800), (44, 44, 90))
        # the performer on the wire, floating up and down over the air bag
        fl = dict(bob=-45.0, rate=0.3, phase=0.15)
        wire = sc.part(P.motion(**fl))
        wire.box(DARK, (0, 74, 630), (5, 5, 320))
        P.figure(sc, 0, 60, z=330, arm_l='out', arm_r='out', legs='wide', swing_l=12, swing_r=-12,
                 **dict(C.member(r, W, top=ORANGE, bottom=ORANGE, hat='helmet', hat_col=WHITE, sleeves='long', wear='trousers', band=None), **fl))
        bag = sc.part(P.motion(bob=-5.0, rate=0.3))
        bag.box(BLUE, (0, 60, 70), (430, 300, 140))
        bag.box(SKY, (0, 60, 144), (350, 230, 8))
        bag.box(WHITE, (0, 60, 149), (120, 120, 3))
        # the rigger and the coordinator on their own deck, the rope up to the tower's head
        g = sc.still()
        H = 150.0
        stage(g, -540, 60, 240, 210, H)
        tilt_box(g, DARK, (-520, 40, H + 112), (-340, 60, 770), 5, 5)
        P.figure(sc, -520, 70, z=H, yaw=-90, arm_l='push', arm_r='push', rock=5, rate=0.3, phase=0.15, **C.member(r, W, hat='cap', hat_col=DARK))
        P.figure(sc, -600, 20, z=H, yaw=-25, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=10, axis_l='x', rate=0.5, phase=r.random(),
                 **C.member(r, W, hat='headset', hat_col=DARK))
        g = sc.still()
        for k, col in enumerate((RED, '#3a5fa8', RED)):       # crash mats
            g.box(col, (500 + (k % 2) * 8, 70, 14 + k * 28), (220, 150, 26))
        P.figure(sc, 500, -70, yaw=20, detail=1, arm_l='hip', arm_r='down', rate=0.3, bob=-1.0, phase=r.random(), **C.member(r, W))
        g = sc.still()
        ptape(g, 1240, -150, posts=5)
    return build


def extras_a(seed=29):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        # the riser: three steps, a pink skirt, the canopy over it
        for k in range(3):
            g.box(WOOD, (0, 30 + k * 84, 30 + k * 30), (660, 84, 60 + k * 60))
        g.box(PINK, (0, -13, 40), (660, 4, 22))
        canopy(g, 0, 120, 680, 280, 0, 440, n=6)
        n = 0
        for k in range(3):
            for i in range(4 if k != 1 else 3):
                x = -250 + i * (166 if k != 1 else 250) + r.uniform(-20, 20)
                d = extra(r)
                if n % 4 == 2:
                    d.update(build='kid', s=r.uniform(0.62, 0.7))
                d.update(CHEER[(n * 5 + seed) % len(CHEER)])
                if n == 3:
                    d.update(arm_r='up', hold_r=P.hold_flag(col=PINK), swing_r=14, arm_l='hip', swing_l=0, bob=3, rate=0.9)
                P.figure(sc, x, 30 + k * 84, z=60 + k * 60, yaw=r.uniform(-16, 16), detail=0, phase=r.random(), react=True, **d)
                n += 1
        # the assistant director on a box, megaphone up, turned to the extras
        g = sc.still()
        g.box(YEL, (-420, -40, 50), (90, 90, 100))
        P.figure(sc, -420, -40, z=100, yaw=140, arm_r='face', hold_r=C.hold_megaphone(), arm_l='up', swing_l=16, rate=0.7, phase=r.random(),
                 **C.member(r, W, hat='cap', hat_col=DARK))
        g = sc.still()
        ptape(g, 900, -130, posts=4)
    return build


def extras_b(seed=31):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 140.0
        stage(g, 0, 60, 740, 260, H)
        rail(g, 0, 186, 740, H)
        # the wardrobe rail at one end, costumes swinging on it
        for sx in (-1, 1):
            g.box(STEEL, (270 + sx * 90, 150, H + 90), (6, 6, 180))
        g.box(STEEL, (270, 150, H + 180), (190, 6, 6))
        sw = sc.part(P.motion(swing=6.0, pivot=(270.0, 150.0, H + 180.0), axis=0.0, rate=0.4))
        for k in range(6):
            sw.box(BRIGHT[(k * 3 + seed) % len(BRIGHT)], (195 + k * 30, 150, H + 118), (22, 8, 110))
        P.figure(sc, 240, 70, z=H, yaw=70, arm_r='fwd', swing_r=10, axis_r='x', arm_l='hold', rate=0.6, phase=r.random(), **C.member(r, W))
        # the queue, and three who sit it out on the bench
        for i in range(4):
            d = extra(r)
            d.update(CHEER[(i * 5 + seed) % len(CHEER)])
            P.figure(sc, 130 - i * 96 + r.uniform(-10, 10), 40 + (i % 2) * 30, z=H, yaw=r.uniform(-30, 10), detail=1 if i < 2 else 0, phase=r.random(),
                     react=True, **d)
        g = sc.still()
        g.box(TEAL, (-250, 130, H + 44), (230, 44, 8))
        for sx in (-1, 1):
            g.box(TEAL, (-250 + sx * 100, 130, H + 20), (8, 40, 40))
        for i in range(3):
            d = extra(r)
            d.update((dict(arm_r='wave', swing_r=24, rate=1.2), dict(arm_r='face', hold_r=P.hold_cup(), rate=0.3, bob=-0.8), dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=1.4))[i])
            P.figure(sc, -326 + i * 76, 130, z=H, yaw=r.uniform(-12, 12), legs='sit', seat=46, detail=0, phase=r.random(), react=True, **d)
        g = sc.still()
        parasol(g, -60, 150, H, h=270, r=170, col=WHITE, tip=PINK)
        ptape(g, 860, -150, posts=4)
        pennant(sc, -366, 186, H, 250, PINK, 0.7)
    return build


def makeup(seed=37):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 120.0
        stage(g, 0, 70, 540, 310, H)
        canopy(g, 0, 70, 540, 310, H, 290, n=6, cols=(WHITE, PINK))
        # the mirror with its bulbs (unlit: paint), the table under it
        g.box(WHITE, (-130, 196, H + 45), (210, 40, 90))
        g.box(DARK, (-130, 210, H + 160), (180, 8, 130))
        g.box(SKY, (-130, 205, H + 160), (156, 4, 106))
        for k in range(5):
            g.box(YEL, (-202 + k * 36, 204, H + 222), (12, 6, 12))
        # two actors in tall chairs, make-up and hair at work on them
        stool(g, -120, 70, H)
        P.figure(sc, -120, 70, z=H + 6, yaw=90, legs='sit', seat=72, arm_l='down', arm_r='down', rate=0.25, bob=-0.8, phase=r.random(),
                 **P.look(r, top=LIME, bottom='#2b2350', wear='trousers', sleeves='long', hair='long'))
        P.figure(sc, -30, 64, z=H, yaw=-90, arm_r='fwd', swing_r=12, axis_r='y', hold_r=hold_brush(), arm_l='hold', rate=1.1, phase=r.random(), **C.member(r, W))
        stool(g, 150, 110, H)
        P.figure(sc, 150, 110, z=H + 6, yaw=-90, legs='sit', seat=72, arm_l='hold', arm_r='hold', hold_r=P.hold_cup(), rate=0.25, bob=-0.8, phase=r.random(),
                 **P.look(r, top=RED, bottom=WHITE, wear='dress', sleeves='none', hair='afro'))
        P.figure(sc, 150, 190, z=H, yaw=0, arm_l='fwd', arm_r='fwd', swing_l=10, swing_r=-10, axis_l='y', axis_r='y', rate=1.3, phase=r.random(), **C.member(r, W))
        # the stunt double, suited and waiting at the front, warming up
        P.figure(sc, 30, -40, z=H, yaw=-10, arm_l='vee', arm_r='vee', bob=6, rock=8, rate=0.6, phase=r.random(),
                 **C.member(r, W, top=ORANGE, bottom=ORANGE, hat='helmet', hat_col=WHITE, sleeves='long', wear='trousers', band=None))
        # the costume rail
        g = sc.still()
        for sx in (-1, 1):
            g.box(STEEL, (-210 + sx * 50, -30, H + 85), (6, 6, 170))
        g.box(STEEL, (-210, -30, H + 170), (110, 6, 6))
        sw = sc.part(P.motion(swing=6.0, pivot=(-210.0, -30.0, H + 170.0), axis=0.0, rate=0.4))
        for k in range(4):
            sw.box(BRIGHT[(k * 3 + 1) % len(BRIGHT)], (-250 + k * 27, -30, H + 112), (20, 8, 104))
        g = sc.still()
        g.box(WOOD, (300, 70, 40), (60, 120, 80))
        ptape(g, 700, -150)
    return build


def press_pen(seed=41):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(GREY, (0, 30, 55), (620, 104, 110))
        g.box(GREY, (0, 134, 85), (620, 104, 170))
        g.box(WHITE, (0, -23, 96), (620, 4, 14))
        g.box(PINK, (0, -23, 70), (620, 4, 14))
        press = (
            dict(arm_l='face', arm_r='face', hold_r=P.hold_camera(), rate=0.3, bob=-1.0),
            dict(arm_l='hold', arm_r='up', hold_r=P.hold_camera(), swing_r=8, rate=0.8, bob=4),
            dict(arm_l='face', arm_r='face', hold_r=P.hold_camera(lens=WHITE), rock=5, rate=0.4),
            dict(arm_r='face', hold_r=P.hold_phone(), arm_l='wave', swing_l=20, rate=1.1, bob=3),
        )
        for k in range(2):
            for i in range(3):
                d = P.look(r, tops=(DARK, '#3a5fa8', WHITE, TEAL, YEL), wear='trousers', sleeves=r.choice(('short', 'long')), hat=r.choice((None, 'cap', 'sun')),
                           hat_col=r.choice((RED, WHITE, DARK)), pack=r.choice((None, GREY, ORANGE)))
                d.update(press[(i + k * 2 + seed) % len(press)])
                P.figure(sc, -210 + i * 150 + k * 50 + r.uniform(-12, 12), 30 + k * 104, z=110 + k * 60, yaw=r.uniform(-18, 18), detail=1 - k,
                         phase=r.random(), react=True, **d)
        # the television camera at the back's end, its operator panning with the chase
        g = sc.still()
        tripod_at(g, 250, 120, 170)
        pan = dict(swing=26.0, pivot=(250.0, 120.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.14, phase=0.4, **pan))
        cam.box(DARK, (250, 110, 170 + 148), (30, 70, 34))
        cam.cyl(STEEL, (250, 75, 170 + 148), 11, 26, rot=(0, 0, 90), steps=8, top_r=16)
        cam.box(TEAL, (250, 120, 170 + 170), (20, 30, 10))
        # one up a step ladder at the side
        g = sc.still()
        for sx in (-1, 1):
            tilt_box(g, YEL, (-400 + sx * 26, -30, 0), (-400 + sx * 26, 30, 200), 6, 6)
            tilt_box(g, YEL, (-400 + sx * 26, 90, 0), (-400 + sx * 26, 30, 200), 6, 6)
        g.box(YEL, (-400, 30, 200), (64, 44, 8))
        P.figure(sc, -400, 30, z=204, yaw=15, detail=1, arm_l='face', arm_r='face', hold_r=P.hold_camera(lens=WHITE), rate=0.4, rock=4, phase=r.random(),
                 **P.look(r, top=YEL, wear='shorts', hat='cap', hat_col=RED))
        g = sc.still()
        ptape(g, 940, -120, posts=4)
        pennant(sc, 306, 180, 170, 260, WHITE, 0.2)
    return build


def reflector(seed=43):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 150.0
        stage(g, 0, 50, 460, 230, H)
        # the bounce: a great white frame edged in yellow, tilting on its two stands
        tilt = dict(swing=7.0, pivot=(0.0, 110.0, H + 60.0), axis=0.0)
        fr = sc.part(P.motion(rate=0.22, phase=0.2, **tilt))
        fr.box(WHITE, (0, 110, H + 250), (380, 6, 310))
        fr.box(YEL, (0, 108, H + 410), (396, 10, 12))
        fr.box(YEL, (0, 108, H + 90), (396, 10, 12))
        for sx in (-1, 1):
            fr.box(YEL, (sx * 194, 108, H + 250), (10, 10, 330))
            g.box(STEEL, (sx * 206, 110, H + 130), (7, 7, 260))
            g.box(DARK, (sx * 206, 110, H + 6), (50, 50, 12))
        P.figure(sc, -150, 40, z=H, yaw=170, arm_l='up', arm_r='up', rock=3, rate=0.22, phase=0.2, **C.member(r, W))
        P.figure(sc, 150, 40, z=H, yaw=-170, arm_l='up', arm_r='up', rock=3, rate=0.22, phase=0.2, **C.member(r, W, hat='cap', hat_col=DARK))
        P.figure(sc, 0, -30, z=H, yaw=0, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), **C.member(r, W, hat='headset', hat_col=DARK))
        # the wind machine: a drum on a stand, a streamer of ribbons blowing from its cage
        g = sc.still()
        g.box(STEEL, (360, 20, 80), (10, 10, 160))
        g.box(DARK, (360, 20, 8), (90, 90, 16))
        g.cyl(DARK, (360, 60, 210), 70, 60, rot=(0, 0, 90), steps=10)
        g.cyl(GREY, (360, -2, 210), 58, 6, rot=(0, 0, 90), steps=10)
        for k in range(3):
            rib = sc.part(P.motion(swing=20.0, pivot=(360.0, -10.0, 170.0 + k * 40), axis=P.UP, rate=1.6, phase=k * 0.3))
            rib.box((PINK, WHITE, PINK)[k], (360, -70, 170 + k * 40), (5, 120, 10))
        g = sc.still()
        for k in range(3):
            g.box(('#5a4a3c', ORANGE, '#5a4a3c')[k], (-300 + k * 14, 10, 10 + k * 18), (70, 40, 18))
        ptape(g, 860, -150, posts=4)
        pennant(sc, -222, 160, H, 250, PINK, 0.6)
    return build


JOBS = [
    ('SM_P3F_SecondUnit_A', second_unit_a(11)),
    ('SM_P3F_SecondUnit_B', second_unit_b(13)),
    ('SM_P3F_DroneTeam_A', drone_team_a(17)),
    ('SM_P3F_DroneTeam_B', drone_team_b(19), dict(grow=(320.0, 320.0, 80.0), grow_down=(320.0, 320.0, 0.0))),
    ('SM_P3F_StuntRig', stunt_rig(23), dict(grow=(60.0, 60.0, 80.0), grow_down=(60.0, 60.0, 0.0))),
    ('SM_P3F_Extras_A', extras_a(29)),
    ('SM_P3F_Extras_B', extras_b(31)),
    ('SM_P3F_MakeUp', makeup(37)),
    ('SM_P3F_PressPen', press_pen(41)),
    ('SM_P3F_Reflector', reflector(43)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
