"""FULL THROTTLE (Unreal) the population of the THEME PARK, pass 3, builder p3f: MORE OF THE FILM SHOOT.
/Game/People/ThemePark/SM_P3F_...

The level is a shoot, and the first two rounds gave it the main unit (ft_people_park.py: the shared crew kit's scenes
on rostrums, a camera tower). This kit adds the units a big shoot spreads along 25 km of road:

    SM_P3F_SecondUnit_A   the second unit on a rostrum: a camera panning on its tripod, its operator, the director
                          with a megaphone, the clapper loader, a hand-held camera, a spotter pointing.
    SM_P3F_SecondUnit_B   the long-lens unit on a scaffold: two cameras under a pink-and-white umbrella, their
                          operators, a spotter with binoculars; the data cart and its wrangler on the lower deck.
    SM_P3F_DroneTeam_A/B  the drone team on a rostrum: the pilot with a controller, a spotter pointing up, a
                          technician at the cases, the landing pad. One drone hovers 5 to 6 m up (a slow float), a
                          second circles the pad (A) or sits on its mast (B). 27 m from the road and more: never over it.
    SM_P3F_StuntRig       stunt rigging: a truss gantry 6 m high, a performer on a wire swinging under it, the air bag
                          breathing below, two riggers on the rope, the coordinator and a safety officer.
    SM_P3F_Extras_A/B     the extras' holding area: a striped canopy, benches of extras in costume, others stretching
                          and waving at the chase, the assistant director counting them, a costume rail.
    SM_P3F_MakeUp         the make-up tent: a mirror in a row of steady lamps, a performer in the chair, the make-up
                          artist and the hair stylist at work, a runner with a tray, the costume rail swaying.
    SM_P3F_PressPen_A/B   the press pen: photographers and reporters on a three-step riser behind the crew's tape,
                          tracking the chase with their cameras; a television camera (A) or a step-ladder (B).
    SM_P3F_Dolly          the shared kit's camera dolly (ft_people_crew.dolly, wardrobe Park) on a long rostrum, lit:
    SM_P3F_Applause_B     and its second applauding crew (seed 8). The park's own kit had neither.
    SM_P3F_LagoonTower_A  for the lagoon, where the road is a coaster 26 m over the valley: a camera tower 25 m high
                          (built to stand on the valley floor, its deck at the rails), a crane arm and crew on the deck;
    SM_P3F_LagoonTower_B  the same with the drone team on it: a big drone hovering over the deck, another circling it.

The crew is marked as on every level: the PINK band across the chest, headsets, yellow-and-dark equipment, pink-and-
white tape. It is a night level and the verge is a hoarding 1.8 m high: every piece is RAISED and carries the steady
glow the park's own kit uses (ft_people_park.lift, 0.09) and a row of steady lamps. Everything here is over 2 m tall
and stands 24 m or more from the road's centre line; nothing blinks. No clown and no balloons (the boss is MR. CHUCKLES).
No words, no marks. Movement only through M_People's motion values.

Run in the editor:  import ft_people_park_p3f as K; K.run()
The placing rules are the block C:/ftwork/p3/L4f.inl (pasted into FTPeopleL4.inl by the level's integrator).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
import ft_people_park as PK
from ft_desert import rod, tilt_box

DIR = '/Game/People/ThemePark'
CW = C.WARDROBES['Park']
PINK, WHITE, DARK, GREY, YEL, STEEL, WOOD = C.PINK, C.WHITE, C.DARK, C.GREY, C.YEL, C.STEEL, C.WOOD
CYAN, RED, ORANGE, LAV, CREAM, NAVY = '#2ee6ff', '#ff2d3c', '#ff8a2a', '#b9a4ff', '#fff1d6', '#2b2350'
DZ = 90.0            # a rostrum's height


# ------------------------------------------------------------------------------------------ helpers
def crew(r, **fixed):
    """One of the shoot's crew: wardrobe Park, the pink band across the chest."""
    fixed.setdefault('band', PINK)
    return C.member(r, CW, **fixed)


def extra(r, **fixed):
    """An extra in costume: bright, a hat as often as not."""
    hat = r.choice((None, 'sun', 'cowboy', 'cap', None, 'beanie'))
    d = P.look(r, tops=PK.BRIGHT, hat=hat, hat_col=r.choice(PK.BRIGHT), wear=r.choice(('trousers', 'dress', 'skirt', 'shorts', 'coat')))
    d.update(fixed)
    return d


def tape_on(sc, length, y, z, posts=4):
    """The crew's pink-and-white tape on a deck z high (posts and two runs; long strips: a piece is placed often)."""
    g = sc.still(at=(0.0, 0.0, z))
    for k in range(posts):
        x = -length / 2.0 + k * length / (posts - 1)
        g.box(WHITE, (x, y, 50), (9, 9, 100))
    n = max(int(length / 110.0), 3)
    for k in range(n):
        x = -length / 2.0 + (k + 0.5) * length / n
        g.box(PINK if k % 2 else WHITE, (x, y, 90), (length / n, 4, 11))
        g.box(WHITE if k % 2 else PINK, (x, y, 56), (length / n, 4, 8))
    return sc.still()


def rostrum(sc, L, W, dz=DZ, cy=0.0, lamps=6):
    """A deck dz high, pink and white along its front, a row of steady lamps under its edge."""
    g = sc.still()
    g.box(GREY, (0, cy, (dz - 10) / 2.0), (L - 20, W - 20, dz - 10))
    g.box(WOOD, (0, cy, dz - 5), (L, W, 10))
    PK.stripes(g, -L / 2.0, L / 2.0, cy - W / 2.0, 6, dz - 12, (PINK, WHITE), max(int(L / 110.0), 4))
    if lamps:
        PK.bulbs(g, (-L / 2.0 + 30, cy - W / 2.0 - 5, dz - 4), (L / 2.0 - 30, cy - W / 2.0 - 5, dz - 4), lamps, size=11.0, glow=1.2)
    return g


def tripod_at(sc, x, y, z, top=128.0):
    g = sc.still(at=(0.0, 0.0, z))
    C.tripod(g, x, y, top=top)
    return sc.still()


def big_camera(m, x, y, z, k=1.0):
    """A film camera drawn k times life size, its lens towards -Y (for the towers, whose people are 1.5 times life)."""
    m.box(DARK, (x, y, z + 17 * k), (30 * k, 62 * k, 34 * k))
    m.cyl(GREY, (x - 6 * k, y + 14 * k, z + 34 * k), 15 * k, 12 * k, steps=8)
    m.cyl(GREY, (x - 6 * k, y - 16 * k, z + 34 * k), 15 * k, 12 * k, steps=8)
    m.box(STEEL, (x, y - 42 * k, z + 17 * k), (16 * k, 24 * k, 16 * k))
    m.box(DARK, (x, y - 58 * k, z + 17 * k), (34 * k, 10 * k, 30 * k))
    m.box(YEL, (x + 16 * k, y + 4 * k, z + 20 * k), (3 * k, 36 * k, 12 * k))


def drone(sc, x, y, z, span=110.0, col=YEL, mo=None):
    """A camera drone: a body, four arms, four rotor discs, the camera slung under it. span: rotor tip to rotor tip."""
    m = sc.part(mo)
    a = span * 0.32
    m.box(col, (x, y, z), (span * 0.34, span * 0.34, span * 0.13))
    m.box(DARK, (x, y, z + span * 0.08), (span * 0.2, span * 0.2, span * 0.05))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, DARK, (x, y, z), (x + sx * a, y + sy * a, z + span * 0.04), span * 0.06, span * 0.05)
            m.cyl(WHITE, (x + sx * a, y + sy * a, z + span * 0.07), span * 0.2, 3.0, steps=8)
        m.box(DARK, (x + sx * a * 0.6, y, z - span * 0.14), (4, span * 0.5, 4))          # the skids
        m.box(DARK, (x + sx * a * 0.6, y, z - span * 0.08), (4, 4, span * 0.12))
    m.box(DARK, (x, y - span * 0.1, z - span * 0.13), (span * 0.14, span * 0.14, span * 0.13))
    m.box(STEEL, (x, y - span * 0.2, z - span * 0.13), (span * 0.09, span * 0.07, span * 0.09))


def hold_controller():
    def f(m, h, side):
        if side < 0:
            return
        cx = h[0] - 22
        m.box(DARK, (cx, h[1] - 6, h[2] + 6), (46, 22, 9))
        m.box(YEL, (cx, h[1] - 8, h[2] + 12), (20, 14, 3))
        for sx in (-1, 1):
            m.box(STEEL, (cx + sx * 16, h[1] - 6, h[2] + 15), (4, 4, 10))
        m.box(STEEL, (cx, h[1] + 6, h[2] + 22), (4, 4, 26))
    return f


def hold_mic(col=DARK, head=CYAN):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 3, h[2] + 10), (5, 5, 26))
        m.box(head, (h[0], h[1] - 3, h[2] + 27), (11, 11, 12))
    return f


def hold_brush(col=PINK):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 10, h[2] + 4), (4, 22, 4))
        m.box(col, (h[0], h[1] - 24, h[2] + 4), (7, 8, 7))
    return f


def hold_binoculars():
    def f(m, h, side):
        m.box(DARK, (h[0] - side * 6, h[1] - 8, h[2] + 6), (22, 18, 10))
    return f


def hold_tray():
    def f(m, h, side):
        if side < 0:
            return
        m.box(WHITE, (h[0] - 24, h[1] - 8, h[2] + 8), (50, 30, 4))
        for k in range(3):
            m.box((PINK, CYAN, YEL)[k], (h[0] - 38 + k * 14, h[1] - 8, h[2] + 17), (9, 9, 14))
    return f


def stool(g, x, y, z, h=46.0, col=NAVY):
    g.box(col, (x, y, z + h - 4), (48, 44, 8))
    g.box(STEEL, (x, y, z + (h - 8) / 2.0), (8, 8, h - 8))
    g.box(STEEL, (x, y, z + 3), (40, 36, 6))


def costume_rail(sc, x, y, z, L=200.0, seed=0, sway=4.0):
    """A rail of costumes on wheels: the frame stands still, the clothes sway on their hangers."""
    r = random.Random(900 + seed)
    g = sc.still()
    for sx in (-1, 1):
        g.box(STEEL, (x + sx * L / 2.0, y, z + 85), (6, 6, 170))
        g.box(DARK, (x + sx * L / 2.0, y, z + 4), (8, 50, 8))
    g.box(STEEL, (x, y, z + 170), (L + 8, 6, 6))
    n = int(L / 26.0)
    for k in range(n):
        px = x - L / 2.0 + (k + 0.5) * L / n
        m = sc.part(P.motion(swing=sway, pivot=(px, y, z + 168), axis=0.0, rate=0.35, phase=r.random()))
        m.box(r.choice(PK.BRIGHT), (px, y, z + 110), (20, 34, 100))


# ------------------------------------------------------------------------------------------ the second unit
def second_unit_a(sc):
    """The second unit on its rostrum: camera, operator, director, clapper loader, a hand-held camera, a spotter."""
    r = random.Random(301)
    L, W = 640.0, 300.0
    rostrum(sc, L, W)
    tape_on(sc, L - 30, -W / 2.0 + 10, DZ, posts=4)
    tripod_at(sc, -60, -30, DZ)
    pan = dict(swing=24.0, pivot=(-60.0, -30.0, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=0.12, phase=0.3, **pan))
    C.film_camera(cam, -60, -30, DZ + 128)
    P.figure(sc, -60, 52, z=DZ, arm_l='push', arm_r='push', rate=0.12, phase=0.3, sway=pan, detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, -230, -40, z=DZ, yaw=14, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12, axis_l='x', rate=0.6, phase=r.random(),
             detail=1, **crew(r, hat='cap', hat_col=DARK))
    P.figure(sc, 90, -70, z=DZ, yaw=16, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.4, bob=-1.5, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, 235, -50, z=DZ, yaw=-12, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=18, rate=0.3, phase=r.random(), react=True,
             detail=1, **crew(r))
    P.figure(sc, 160, 70, z=DZ, yaw=-25, arm_r='point', swing_r=14, axis_r='x', arm_l='hip', rate=0.7, bob=4, phase=r.random(), react=True,
             detail=1, **crew(r))
    g = sc.still()
    C.case(g, -250, 90, z=DZ)
    C.case(g, -240, 88, z=DZ + 38, w=52, h=30, col=YEL)
    C.apple_box(g, 20, 100, z=DZ)


def second_unit_b(sc):
    """The long-lens unit: a scaffold 2.6 m high, two cameras under a striped umbrella, a spotter; the data cart below."""
    r = random.Random(307)
    H = 260.0
    rostrum(sc, 660, 300)
    tape_on(sc, 630, -140, DZ, posts=4)
    g = sc.still()
    cx = -130.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL, (cx + sx * 170, sy * 110, (H + DZ) / 2.0), (9, 9, H - DZ))
        tilt_box(g, STEEL, (cx + sx * 170, -110, DZ + 10), (cx - sx * 170, -110, H - 10), 6, 6)
    g.box(WOOD, (cx, 0, H), (370, 250, 12))
    g.box(PINK, (cx, -124, H + 40), (370, 4, 46))
    for k in range(4):
        g.box(WHITE, (cx - 140 + k * 93, -126.5, H + 40), (46, 2, 46))
    g.box(YEL, (cx, -122, H + 96), (370, 6, 8))
    g.box(YEL, (cx, 122, H + 96), (370, 6, 8))
    for sx in (-1, 1):
        g.box(YEL, (cx + sx * 182, 0, H + 96), (6, 240, 8))
        for sy in (-1, 1):
            g.box(YEL, (cx + sx * 182, sy * 120, H + 50), (6, 6, 100))
    # the ladder up the side
    for sy in (-1, 1):
        tilt_box(g, STEEL, (cx + 200, sy * 22 + 60, DZ), (cx + 186, sy * 22 + 60, H + 6), 6, 6)
    for k in range(4):
        g.box(STEEL, (cx + 197 - k * 3.4, 60, DZ + 30 + k * 42), (6, 44, 5))
    # the umbrella over the cameras
    g.box(STEEL, (cx, 30, H + 150), (6, 6, 290))
    for k in range(6):
        a0, a1 = math.radians(k * 60.0), math.radians(k * 60.0 + 60.0)
        am = (a0 + a1) / 2.0
        tilt_box(g, PINK if k % 2 else WHITE, (cx, 30, H + 300), (cx + math.cos(am) * 150, 30 + math.sin(am) * 150, H + 262), 150, 5)
    top = H + 6
    for i, (x, ph, sw) in enumerate(((cx - 80, 0.1, 22.0), (cx + 70, 0.6, -18.0))):
        tripod_at(sc, x, -40, top)
        pan = dict(swing=sw, pivot=(x, -40.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.1, phase=ph, **pan))
        C.film_camera(cam, x, -40, top + 128)
        cam.box(STEEL, (x, -112, top + 145), (20, 70, 20))                        # the long lens
        P.figure(sc, x, 44, z=top, arm_l='push', arm_r='push', rate=0.1, phase=ph, sway=pan, detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, cx + 150, -70, z=top, yaw=-20, arm_l='face', arm_r='face', hold_r=hold_binoculars(), turn=20, rate=0.25, phase=0.4, detail=1, **crew(r))
    # the lower deck: the data cart, its wrangler on a stool, a runner cheering
    g = sc.still()
    g.box(GREY, (210, 40, DZ + 50), (110, 60, 100))
    g.box(DARK, (210, 8, DZ + 122), (96, 8, 44))
    g.box('#3a5fa8', (210, 3.5, DZ + 122), (84, 1.5, 34))
    stool(g, 210, 105, DZ)
    P.figure(sc, 210, 105, z=DZ, yaw=0, legs='sit', seat=46, arm_l='hold', arm_r='hold', rate=0.3, bob=-0.8, detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, 280, -80, z=DZ, yaw=10, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.6, phase=0.2, react=True, detail=1, **crew(r))


# ------------------------------------------------------------------------------------------ the drone team
def _drone_team(seed, col, second):
    def build(sc):
        r = random.Random(seed)
        L, W = 600.0, 300.0
        rostrum(sc, L, W)
        tape_on(sc, L - 30, -W / 2.0 + 10, DZ, posts=4)
        g = sc.still()
        px, py = 150.0, 20.0
        g.box(YEL, (px, py, DZ + 2), (200, 200, 4))                       # the landing pad
        g.box(DARK, (px, py, DZ + 4.5), (150, 150, 2))
        g.box(YEL, (px, py, DZ + 6), (110, 110, 2))
        g.box(DARK, (px, py, DZ + 7.5), (40, 40, 2))
        # the pilot looks up at the drone; the spotter points at it; the technician sits at the cases
        P.figure(sc, -110, -50, z=DZ, yaw=-28, arm_l='hold', arm_r='hold', hold_r=hold_controller(), turn=10, rate=0.22, phase=0.1, detail=1,
                 **crew(r, hat='cap', hat_col=RED))
        P.figure(sc, -10, -80, z=DZ, yaw=-20, arm_r='up', swing_r=12, arm_l='hip', rate=0.6, bob=3, phase=r.random(), react=True, detail=1,
                 **crew(r, hat='headset', hat_col=DARK))
        C.case(g, -230, 70, z=DZ, w=90, d=54, h=44)
        C.case(g, -150, 96, z=DZ, w=60, d=40, h=34, col=YEL)
        P.figure(sc, -230, 70, z=DZ, yaw=24, legs='sit', seat=44, arm_l='hold', arm_r='hold', hold_r=P.hold_tool(), swing_r=8, axis_r='x',
                 rate=0.8, phase=r.random(), detail=1, **crew(r))
        P.figure(sc, 20, 80, z=DZ, yaw=-40, arm_r='wave', swing_r=24, arm_l='down', rate=1.2, bob=5, phase=r.random(), react=True, detail=1, **crew(r))
        # a mast with a steady lamp and a pennant, so the pad reads from the road
        g.box(STEEL, (270, 110, DZ + 150), (6, 6, 300))
        g.box(PINK, (245, 110, DZ + 276), (50, 4, 30))
        g.box('#ffe28a', (270, 110, DZ + 306), (14, 14, 14), glow=1.2)
        # the drone over the pad. Review of pass 3: it is a prop on a steel pole now and only rocks (it floated free,
        # 7 m up and 25 m from the road, and the level's enemies fly on rotors); the 'orbit' variant's second drone,
        # which circled the pad, is gone.
        g.box(STEEL, (px, py, (DZ + 8.0 + 554.0) / 2.0), (8, 8, 554.0 - DZ - 8.0))
        drone(sc, px, py, 560.0, span=120.0, col=col,
              mo=P.motion(rate=0.22, phase=r.random(), swing=5.0, pivot=(px, py, 554.0), axis=0.0))
        if second == 'mast':                         # a second one on its launch mast, rocking as its rotors run up
            g.box(STEEL, (-20, 120, DZ + 110), (8, 8, 220))
            g.box(DARK, (-20, 120, DZ + 222), (60, 60, 6))
            drone(sc, -20.0, 120.0, DZ + 246.0, span=96.0, col=WHITE,
                  mo=P.motion(swing=4.0, pivot=(-20.0, 120.0, DZ + 226.0), axis=90.0, rate=0.9, phase=r.random()))
    return build


# ------------------------------------------------------------------------------------------ stunt rigging
def stunt_rig(sc):
    """A truss gantry 6 m high: a performer swings on a wire under it, over the air bag; riggers on the rope."""
    r = random.Random(331)
    L, W = 780.0, 300.0
    rostrum(sc, L, W)
    tape_on(sc, L - 30, -W / 2.0 + 10, DZ, posts=5)
    g = sc.still()
    TOP = 620.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(YEL, (sx * 330, sy * 40 + 30, (TOP + DZ) / 2.0), (10, 10, TOP - DZ))
        for k in range(5):
            z0 = DZ + k * (TOP - DZ) / 5.0
            z1 = DZ + (k + 1) * (TOP - DZ) / 5.0
            tilt_box(g, DARK, (sx * 330, -10 if k % 2 else 70, z0), (sx * 330, 70 if k % 2 else -10, z1), 6, 6)
        g.box(DARK, (sx * 330, 30, DZ + 5), (70, 150, 10))
    for sy in (-1, 1):
        g.box(YEL, (0, sy * 40 + 30, TOP), (680, 10, 10))
    for k in range(8):
        x0 = -330 + k * 82.5
        tilt_box(g, DARK, (x0, -10 if k % 2 else 70, TOP), (x0 + 82.5, 70 if k % 2 else -10, TOP), 6, 6)
    PK.bulbs(g, (-300, -16, TOP - 12), (300, -16, TOP - 12), 6, size=12.0, glow=1.2)
    # the performer on the wire: everything swings about the beam
    swing = dict(swing=15.0, pivot=(0.0, 30.0, TOP), axis=90.0)
    wire = sc.part(P.motion(rate=0.21, phase=0.0, **swing))
    wire.box(WHITE, (0, 30, (TOP + 400) / 2.0), (5, 5, TOP - 400))
    wire.box(DARK, (0, 30, 392), (40, 30, 16))
    P.figure(sc, 0, 30, z=265, yaw=0, legs='stride', arm_l='out', arm_r='vee', rate=0.21, phase=0.0, sway=swing, detail=1,
             **crew(r, top=ORANGE, bottom=ORANGE, hat='helmet', hat_col=WHITE, sleeves='long', wear='trousers', band=None))
    # the air bag, breathing on its fan
    bag = sc.part(P.motion(bob=-5.0, rate=0.3))
    bag.box('#2e6bff', (0, 30, DZ + 65), (380, 220, 130))
    bag.box('#57c7ff', (0, 30, DZ + 133), (320, 170, 8))
    bag.box(WHITE, (0, 30, DZ + 138), (110, 110, 3))
    # the riggers on the rope, by the left leg
    rope = sc.still()
    tilt_box(rope, WHITE, (-322, 30, TOP - 8), (-285, -70, DZ + 120), 5, 5)
    P.figure(sc, -280, -86, z=DZ, yaw=160, arm_l='push', arm_r='push', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rate=0.21, phase=0.25,
             detail=1, **crew(r))
    P.figure(sc, -235, -40, z=DZ, yaw=170, arm_l='up', arm_r='up', swing_l=8, swing_r=8, axis_l='x', axis_r='x', rate=0.21, phase=0.3,
             detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, 265, -80, z=DZ, yaw=24, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.6, detail=1,
             **crew(r, hat='cap', hat_col=DARK))
    P.figure(sc, 320, 60, z=DZ, yaw=30, arm_l='hip', arm_r='down', rate=0.3, bob=-1.0, detail=1, **crew(r))
    g = sc.still()
    g.cyl(RED, (285, 20, DZ), 11, 46, steps=8)
    g.cyl(DARK, (285, 20, DZ + 46), 5, 10, steps=6)


# ------------------------------------------------------------------------------------------ the extras
def _extras(seed, cols):
    def build(sc):
        r = random.Random(seed)
        L, W = 720.0, 310.0
        rostrum(sc, L, W)
        tape_on(sc, L - 30, -W / 2.0 + 10, DZ, posts=5)
        g = sc.still()
        # the canopy over the benches at the back
        for sx in (-1, 1):
            g.box(STEEL, (sx * 330, 135, DZ + 160), (7, 7, 320))
            g.box(STEEL, (sx * 330, -10, DZ + 140), (7, 7, 280))
        n = 8
        for k in range(n):
            x = -340 + (k + 0.5) * 680.0 / n
            tilt_box(g, cols[k % 2], (x, 150, DZ + 322), (x, -30, DZ + 278), 680.0 / n, 6)
        PK.bulbs(g, (-320, -34, DZ + 268), (320, -34, DZ + 268), 8, size=11.0, glow=1.2)
        # two benches of extras sitting, in costume
        for row, (y, n2) in enumerate(((95, 5), )):
            g.box(WOOD, (-70, y, DZ + 42), (470, 40, 8))
            for sx in (-1, 0, 1):
                g.box(STEEL, (-70 + sx * 220, y, DZ + 19), (8, 36, 38))
            for k in range(n2):
                x = -260 + k * 95 + r.uniform(-8, 8)
                act = (dict(arm_r='wave', swing_r=24, rate=1.2), dict(arm_r='face', hold_r=P.hold_cup(), rate=0.3, bob=-0.8),
                       dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=1.5), dict(arm_l='hold', arm_r='hold', rate=0.3, bob=-0.8))[(k + seed) % 4]
                P.figure(sc, x, y, z=DZ, yaw=r.uniform(-12, 12), legs='sit', seat=46, phase=r.random(), react=True, detail=0, **dict(extra(r), **act))
        # others on their feet: warming up, waving, pointing at the chase
        acts = (dict(rock=10, rate=0.5, arm_l='hip', arm_r='up'), dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.4),
                dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7), dict(arm_r='wave', swing_r=26, rate=1.3, bob=4),
                dict(turn=24, rate=0.6, arm_l='out', arm_r='out'))
        for k, x in enumerate((-300, -190, -70, 60, 170)):
            d = extra(r)
            d.update(acts[(k * 3 + seed) % len(acts)])
            if k == 2:
                d.update(build='kid', s=0.66)
            P.figure(sc, x + r.uniform(-10, 10), -60 + (k % 2) * 40, z=DZ, yaw=r.uniform(-16, 16), phase=r.random(), react=True, detail=1, **d)
        # the assistant director counts them
        P.figure(sc, 290, -70, z=DZ, yaw=60, arm_r='point', swing_r=14, axis_r='z', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.5, detail=1,
                 **crew(r, hat='headset', hat_col=DARK))
        costume_rail(sc, 270, 100, DZ, L=150.0, seed=seed)
    return build


# ------------------------------------------------------------------------------------------ the make-up tent
def make_up(sc):
    """The make-up tent, open to the road: the lit mirror, a performer in the chair, make-up and hair at work."""
    r = random.Random(347)
    L, W = 620.0, 310.0
    rostrum(sc, L, W)
    tape_on(sc, L - 30, -W / 2.0 + 10, DZ, posts=4)
    g = sc.still()
    # the tent: a striped back wall, two side walls, a pitched roof
    PK.stripes(g, -300, 300, 148, DZ, DZ + 250, (PINK, WHITE), 8, depth=6.0)
    for sx in (-1, 1):
        g.box(WHITE, (sx * 300, 60, DZ + 125), (6, 170, 250))
        g.box(STEEL, (sx * 300, -26, DZ + 125), (7, 7, 250))
    for k in range(8):
        x = -300 + (k + 0.5) * 75.0
        tilt_box(g, WHITE if k % 2 else PINK, (x, 152, DZ + 330), (x, -40, DZ + 252), 75.0, 6)
    # the mirror table against the back wall: the mirror in its row of steady lamps
    g.box(WHITE, (-60, 110, DZ + 40), (300, 50, 80))
    g.box(CREAM, (-60, 138, DZ + 150), (250, 8, 130))
    g.box('#bfeaff', (-60, 133, DZ + 150), (214, 3, 96))
    PK.bulbs(g, (-175, 130, DZ + 208), (55, 130, DZ + 208), 6, size=10.0, glow=1.1)
    PK.bulbs(g, (-175, 130, DZ + 92), (55, 130, DZ + 92), 6, size=10.0, glow=1.1)
    for k in range(4):
        g.box((PINK, CYAN, YEL, LAV)[k], (-150 + k * 34, 100, DZ + 87), (12, 12, 14))
    # the performer in the chair, back to the road; the make-up artist and the hair stylist either side
    stool(g, -110, 40, DZ, h=60.0, col=PINK)
    P.figure(sc, -110, 40, z=DZ, yaw=180, legs='sit', seat=60, arm_l='down', arm_r='down', rate=0.3, bob=-0.8, detail=1,
             **extra(r, top='#ffd23f', hat=None, hair='long'))
    P.figure(sc, -185, 36, z=DZ, yaw=-80, arm_r='fwd', hold_r=hold_brush(), swing_r=10, axis_r='x', arm_l='hold', rate=1.1, phase=0.2, detail=1,
             **crew(r, hat=None))
    P.figure(sc, -40, 70, z=DZ, yaw=110, arm_l='up', arm_r='up', swing_l=9, swing_r=-9, axis_l='x', axis_r='x', rate=0.9, phase=0.6, detail=1,
             **crew(r, hat=None))
    # the next one waits, in half a costume, and waves at the chase; a runner brings the tray
    stool(g, 80, 30, DZ, h=60.0, col=CYAN)
    P.figure(sc, 80, 30, z=DZ, yaw=10, legs='sit', seat=60, arm_r='wave', swing_r=26, arm_l='down', rate=1.2, phase=r.random(), react=True, detail=1,
             **extra(r, top=LAV, hat='sun', hat_col=PINK))
    P.figure(sc, 200, -70, z=DZ, yaw=-30, arm_l='carry', arm_r='carry', hold_r=hold_tray(), rate=0.4, bob=-1.5, phase=r.random(), detail=1,
             **crew(r, hat='headset', hat_col=DARK))
    costume_rail(sc, 215, 90, DZ, L=150.0, seed=47)


# ------------------------------------------------------------------------------------------ the press pen
def _press(seed, extra_piece):
    def build(sc):
        r = random.Random(seed)
        L = 700.0
        g = sc.still()
        steps = ((-95.0, 60.0), (-10.0, 120.0), (75.0, 180.0))            # y of the step's middle, its height
        for k, (y, h) in enumerate(steps):
            g.box(GREY, (0, y, h / 2.0 - 3), (L, 86, h - 6))
            g.box(WOOD, (0, y, h - 3), (L, 86, 6))
        PK.stripes(g, -L / 2.0, L / 2.0, -139, 6, 54, (PINK, WHITE), 6)
        PK.bulbs(g, (-L / 2.0 + 30, -144, 56), (L / 2.0 - 30, -144, 56), 6, size=11.0, glow=1.2)
        for sx in (-1, 1):                                               # the pen's side rails
            g.box(STEEL, (sx * (L / 2.0 - 4), 110, 240), (7, 7, 130))
            tilt_box(g, YEL, (sx * (L / 2.0 - 4), 110, 300), (sx * (L / 2.0 - 4), -120, 170), 6, 6)
        g.box(YEL, (0, 116, 300), (L, 6, 8))
        tape_on(sc, L - 30, -128, 60.0, posts=4)
        acts = (
            dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=20, rate=0.3),                       # tracking the chase
            dict(arm_l='face', arm_r='face', hold_r=P.hold_camera(lens=CYAN), turn=16, rate=0.26),
            dict(arm_r='face', hold_r=hold_mic(), arm_l='point', swing_l=10, axis_l='x', rate=0.6),            # a reporter
            dict(arm_r='up', hold_r=P.hold_phone(), arm_l='hip', rate=0.3, bob=-1.0),
            dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(col=GREY), turn=24, rate=0.34),
            dict(arm_r='wave', swing_r=24, rate=1.3, bob=4),
        )
        k = 0
        for row, (y, h) in enumerate(steps):
            xs = ((-290, -175, -60, 60, 290), (-250, -120, 10, 250), (-300, -170, 250))[row]
            for x in xs:
                if extra_piece == 'ladder' and row == 0 and x == 60:
                    continue
                d = P.look(r, tops=PK.BRIGHT, band=CYAN, wear=r.choice(('trousers', 'trousers', 'coat', 'skirt')),
                           hat=r.choice((None, None, 'cap', 'beanie')), hat_col=r.choice((NAVY, WHITE, RED)))
                d.update(acts[(k * 5 + seed) % len(acts)])
                P.figure(sc, x + r.uniform(-12, 12), y - 8, z=h, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=1 if row == 0 else 0, **d)
                k += 1
        top = steps[2][1]
        if extra_piece == 'tv':       # a television camera on the top step, panning with its operator
            tripod_at(sc, 60, 60, top)
            pan = dict(swing=26.0, pivot=(60.0, 60.0, 0.0), axis=P.UP)
            cam = sc.part(P.motion(rate=0.14, phase=0.5, **pan))
            C.film_camera(cam, 60, 60, top + 128)
            cam.box(CYAN, (60, 66, top + 190), (34, 40, 8))
            P.figure(sc, 60, 130, z=top, arm_l='push', arm_r='push', rate=0.14, phase=0.5, sway=pan, detail=0,
                     **P.look(r, tops=(NAVY,), band=CYAN, hat='headset', hat_col=DARK))
            P.figure(sc, 150, 70, z=top, yaw=-20, arm_r='face', hold_r=hold_mic(head=RED), arm_l='hip', rock=4, rate=0.4, detail=0,
                     **P.look(r, tops=(RED, WHITE), band=CYAN, wear='coat'))
        else:                         # a photographer up a step-ladder on the bottom step, over everyone's heads
            hb = steps[0][1]
            for sy in (-1, 1):
                tilt_box(g, STEEL, (60, -95 + sy * 34, hb), (60, -95 + sy * 6, hb + 150), 6, 6)
            for kk in range(3):
                g.box(STEEL, (60, -95, hb + 40 + kk * 45), (56, 30 - kk * 6, 5))
            g.box(YEL, (60, -95, hb + 152), (60, 36, 6))
            P.figure(sc, 60, -95, z=hb + 155, yaw=0, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(lens=YEL), turn=22, rate=0.3, react=True,
                     detail=1, **P.look(r, tops=(ORANGE,), band=CYAN, hat='cap', hat_col=NAVY))
            P.figure(sc, 90, 70, z=top, yaw=-10, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.4, react=True, detail=0,
                     **P.look(r, tops=PK.BRIGHT, band=CYAN))
    return build


# ------------------------------------------------------------------------------------------ the lagoon's towers
def _tower_frame(sc, H, DL, DW):
    """A tower on four banded legs, built to stand on the lagoon's valley floor: its deck H up, the legs 6 m under
    z = 0 (it stands in a hollow as well as on a hump). The crew's colours; a railing; a row of steady lamps."""
    g = sc.still()
    lx, ly = DL / 2.0 - 110.0, DW / 2.0 - 70.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(GREY, (sx * lx, sy * ly, (H - 600) / 2.0), (52, 52, H + 600))
            for k in range(4):
                g.box(PINK if k % 2 == 0 else WHITE, (sx * lx, sy * ly, 480 + k * 560), (60, 60, 150))
    for k in range(4):
        z0, z1 = 150 + k * 580, 150 + (k + 1) * 580
        tilt_box(g, STEEL, (-lx if k % 2 == 0 else lx, -ly, z0), (lx if k % 2 == 0 else -lx, -ly, z1), 18, 18)
    g.box(WOOD, (0, 0, H - 10), (DL, DW, 20))
    PK.stripes(g, -DL / 2.0, DL / 2.0, -DW / 2.0 - 2, H - 110, H - 20, (PINK, WHITE), 10)
    g.box(YEL, (0, -DW / 2.0 - 2, H - 4), (DL + 10, 8, 12))
    PK.bulbs(g, (-DL / 2.0 + 40, -DW / 2.0 - 10, H - 60), (DL / 2.0 - 40, -DW / 2.0 - 10, H - 60), 10, size=16.0)
    for sx in (-1, 1):
        g.box(YEL, (sx * DL / 2.0, 0, H + 150), (8, DW, 10))
        for sy in (-1, 1):
            g.box(YEL, (sx * DL / 2.0, sy * DW / 2.0, H + 78), (8, 8, 156))
    g.box(YEL, (0, DW / 2.0, H + 150), (DL, 8, 10))
    return g


def lagoon_tower_a(sc):
    """The lagoon's camera tower: a crane arm rising and falling over the rails' side, a camera panning, crew cheering."""
    r = random.Random(359)
    H, DL, DW = 2500.0, 1100.0, 480.0
    g = _tower_frame(sc, H, DL, DW)
    S = 1.5
    # the crane: a post on the deck, the arm reaching out towards the road
    g.box(YEL, (330, 60, H + 150), (40, 40, 300))
    g.box(DARK, (330, 60, H + 310), (60, 60, 24))
    arm = sc.part(P.motion(swing=8.0, pivot=(330.0, 60.0, H + 320.0), axis=0.0, rate=0.09))
    tilt_box(arm, YEL, (330, 300, H + 270), (330, -420, H + 480), 22, 30)
    arm.box(DARK, (330, 310, H + 262), (90, 76, 90))
    arm.box(DARK, (330, -420, H + 452), (46, 46, 56))
    big_camera(arm, 330, -400, H + 378, 1.5)
    P.figure(sc, 430, 190, z=H, yaw=150, arm_l='push', arm_r='push', rock=3, rate=0.09, detail=0, **crew(r, s=S))
    # the camera on its tripod, panning with its operator
    for a in (90, 210, 330):
        rod(g, STEEL, (-250, -80, H + 190), (-250 + math.cos(math.radians(a)) * 78, -80 + math.sin(math.radians(a)) * 78, H), 5, steps=5)
    pan = dict(swing=24.0, pivot=(-250.0, -80.0, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=0.12, phase=0.2, **pan))
    big_camera(cam, -250, -80, H + 190, 1.5)
    P.figure(sc, -250, 50, z=H, arm_l='push', arm_r='push', rate=0.12, phase=0.2, sway=pan, detail=0, **crew(r, s=S, hat='headset', hat_col=DARK))
    acts = (dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=14, rate=1.6), dict(arm_r='wave', swing_r=28, bob=5, rate=1.3),
            dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7), dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=16, rate=1.4))
    for k, x in enumerate((-470, -80, 80, 200)):
        d = crew(r, s=S)
        d.update(acts[k])
        P.figure(sc, x, -150, z=H, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=0, **d)
    # two flags on the back corners
    for sx in (-1, 1):
        g.box(STEEL, (sx * (DL / 2.0 - 10), DW / 2.0 - 10, H + 420), (10, 10, 540))
        fl = sc.part(P.motion(swing=10.0, pivot=(sx * (DL / 2.0 - 10), DW / 2.0 - 10, H + 620), axis=P.UP, rate=0.5, phase=0.3 * sx))
        fl.box(PINK if sx < 0 else WHITE, (sx * (DL / 2.0 - 10) + 75, DW / 2.0 - 10, H + 630), (150, 5, 90))


def lagoon_tower_b(sc):
    """The lagoon's drone tower: pilots and a spotter on the deck, a big drone hovering over it, another circling."""
    r = random.Random(367)
    H, DL, DW = 2500.0, 1000.0, 480.0
    g = _tower_frame(sc, H, DL, DW)
    S = 1.5
    g.box(YEL, (250, 30, H + 3), (360, 360, 6))                             # the landing pad
    g.box(DARK, (250, 30, H + 7), (270, 270, 3))
    g.box(YEL, (250, 30, H + 10), (190, 190, 3))
    P.figure(sc, -330, -130, z=H, yaw=-25, arm_l='hold', arm_r='hold', hold_r=hold_controller(), turn=10, rate=0.2, detail=0,
             **crew(r, s=S, hat='cap', hat_col=RED))
    P.figure(sc, -160, -150, z=H, yaw=-10, arm_l='hold', arm_r='hold', hold_r=hold_controller(), turn=12, rate=0.17, phase=0.4, detail=0,
             **crew(r, s=S, hat='headset', hat_col=DARK))
    P.figure(sc, 0, -150, z=H, yaw=-20, arm_r='up', swing_r=12, arm_l='hip', rate=0.6, bob=5, phase=r.random(), react=True, detail=0, **crew(r, s=S))
    P.figure(sc, -420, 60, z=H, yaw=20, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=14, rate=1.6, phase=r.random(), react=True, detail=0,
             **crew(r, s=S))
    C.case(g, -150, 150, z=H, w=130, d=80, h=66)
    C.case(g, -20, 160, z=H, w=90, d=60, h=50, col=YEL)
    g.box(STEEL, (-460, 200, H + 300), (10, 10, 600))
    g.box('#ffe28a', (-460, 200, H + 610), (22, 22, 22), glow=1.2)
    drone(sc, 250.0, 30.0, H + 520.0, span=300.0, col=YEL,
          mo=P.motion(bob=-80.0, rate=0.2, phase=0.1, swing=5.0, pivot=(250.0, 30.0, H + 520.0), axis=0.0))
    # the second circles the tower's own middle, 5.6 m out and 9 m over the deck (the tower stands 56 m from the road)
    drone(sc, 560.0, 0.0, H + 900.0, span=260.0, col=WHITE,
          mo=P.motion(bob=-50.0, rate=0.25, phase=0.6, base=(0.0, 0.0), travel=(-1.0, 0.045)))


# ------------------------------------------------------------------------------------------ the kit
JOBS = [
    ('SM_P3F_SecondUnit_A', PK.lit(second_unit_a)),
    ('SM_P3F_SecondUnit_B', PK.lit(second_unit_b)),
    ('SM_P3F_DroneTeam_A', PK.lit(_drone_team(311, YEL, 'orbit')), dict(grow=(60.0, 60.0, 90.0))),
    ('SM_P3F_DroneTeam_B', PK.lit(_drone_team(313, CYAN, 'mast')), dict(grow=(60.0, 60.0, 90.0))),
    ('SM_P3F_StuntRig', PK.lit(stunt_rig), dict(grow=(140.0, 60.0, 60.0), grow_down=(140.0, 60.0, 0.0))),
    ('SM_P3F_Extras_A', PK.lit(_extras(337, (PINK, WHITE)))),
    ('SM_P3F_Extras_B', PK.lit(_extras(341, (CYAN, CREAM)))),
    ('SM_P3F_MakeUp', PK.lit(make_up)),
    ('SM_P3F_PressPen_A', PK.lit(_press(349, 'tv'))),
    ('SM_P3F_PressPen_B', PK.lit(_press(353, 'ladder'))),
    # the shared crew kit's scenes the park's own kit left out, on a rostrum and lit like its others
    ('SM_P3F_Dolly', PK.lit(PK.raised(C.dolly(CW), 90.0, 1260.0, 330.0, -25.0)), dict(grow=(340.0, 60.0, 60.0), grow_down=(340.0, 60.0, 0.0))),
    ('SM_P3F_Applause_B', PK.lit(PK.raised(C.applause(CW, seed=8), 90.0, 840.0, 260.0, 10.0))),
    ('SM_P3F_LagoonTower_A', PK.lit(lagoon_tower_a, 0.1), dict(grow=(60.0, 60.0, 140.0))),
    ('SM_P3F_LagoonTower_B', PK.lit(lagoon_tower_b, 0.1), dict(grow=(60.0, 460.0, 140.0), grow_down=(1260.0, 460.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
