"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea, pass 3, builder p3f: MORE OF THE SHOOT
(/Game/People/UnderTheSea/SM_P3F_*).

The level is a film shoot on the sea bed. The main table has the main unit (the shared crew kit in its Sea wardrobe and
the camera sled). This kit is the rest of a big shoot, each unit brought with the rostrum it stands on: the sand lies
0.9 m under the road behind a metre of barrier, so every deck here is 2.4 m or more over the sand, the figures are a
quarter larger than life, and what marks the crew out is big and flat: pink-and-white blocks along every deck's front
edge, the pink-and-white tape at its rail, the pink band across the chest.

    SM_P3F_SecondUnit_A   a second camera unit: the camera panning on its tripod with its operator, the assistant
                          pointing, the clapper loader, the unit's director on a box with a megaphone; a diver with a
                          camera housing hangs in the water over them.
    SM_P3F_SecondUnit_B   a bigger unit: two cameras panning against each other, no diver over it.
    SM_P3F_DroneTeam_A    the camera-drone team (under water a drone is a small yellow submersible): one hovering over
                          its pad, one circling the mast behind the deck (never towards the road by more than the
                          deck's own depth), the pilot at the controls, the spotter pointing.
    SM_P3F_DroneTeam_B    three drones: one high on a pole, one hovering, one circling; a technician at the spare.
    SM_P3F_StuntRig       stunt rigging: a 10 m gantry, a performer swinging on the line, a second waiting on the
                          tower, riggers on the deck, the air bag breathing under the swing.
    SM_P3F_Extras_A       the extras' holding area under a striped canopy: aquanauts of every colour on benches and
                          standing about, a wrangler with a megaphone.
    SM_P3F_Extras_B       the extras queuing at the costume rail, a wrangler with a clipboard.
    SM_P3F_MakeUp         the make-up tent: a performer in the chair, the artist at work, the mirror with its steady
                          lamps, helmets on the rack, one waiting with a cup.
    SM_P3F_PressPen_A     the press pen: photographers at the tape, cameras up, one on a step, a boom over the rail.
    SM_P3F_PressPen_B     the press in two rows, two more hanging in the water over the pen with camera housings.
    SM_P3F_CableCam       a cable camera: two masts, the wire, the camera carriage sliding 2.5 m each way along the road
                          with its diver, the focus puller and an assistant on the deck under it.
    SM_P3F_LightBalloon   the lighting unit: a balloon light (a steady glow) floating on its lines over the deck, the
                          sparks at the lines and the generator, a reflector.

    SM_P3F_Deck_A / _B    a bare rostrum, 15 m long, 2.5 m over the sand, a safety diver at one end and the unit's
                          flags: the block's Sets stand a scene of the shared crew kit on it (SM_Crew<Piece>_Sea).

All are TALL (24 m or more from the centre line; the block places their middles 29 m out or more). Light: only steady
glow (the balloon, the mirror's lamps); nothing blinks. Nothing is blue-grey with a fin, nothing is an orange carrier
or a green toolbox. The drones are yellow boxes with a lens, small, and stay over their own deck.

A piece is drawn like every prop: cm; z = 0 is the sand; the middle of the footprint is the origin; the FRONT faces -Y
(the road); long pieces run along X (the road).

Run in the editor:  import ft_people_sea_p3f as K; K.run()        (or K.run(['SM_P3F_StuntRig']))
The placing rules: C:/ftwork/p3/L6f.inl (pasted by the integrator at the end of Source/FullThrottle/FTPeopleL6.inl).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
import ft_people_sea as S
import ft_people_sea_watch as WT
from ft_desert import rod, tilt_box

DIR = '/Game/People/UnderTheSea'

PINK, WHITE, DARK, GREY, YEL, STEEL = C.PINK, C.WHITE, C.DARK, C.GREY, C.YEL, C.STEEL
DECK, DECK_D = '#cfe3ea', '#7fa2b4'              # the station's metal (ft_dress_abyss.py)
ORANGE, RED, LIME, MAG, LAMP = '#ff7a3c', '#ff2d3c', '#b8ff5a', '#ff4fb8', '#fff1c0'
HELM = '#f4f9ff'
W = C.WARDROBES['Sea']
FIG = 1.25                                       # the crew is drawn a quarter larger than life (see the header)


# ------------------------------------------------------------------------------------------ what every unit shares
def rostrum(sc, L, D, Z, y=0.0, off=0, tape=True, legs=3):
    """A unit's deck: L along the road, D deep, its top Z over the sand; pink-and-white blocks along its front edge and
    the crew's tape at the rail. Returns the still model (its frame the mesh's own)."""
    g = sc.still()
    g.box(DECK, (0, y, Z - 9), (L, D, 18))
    g.box(DECK_D, (0, y, Z - 28), (L - 50, D - 50, 20))
    for k in range(legs):
        x = -L / 2.0 + 40 + k * (L - 80) / float(legs - 1)
        for sy in (-1, 1):
            g.box(DECK_D, (x, y + sy * (D / 2.0 - 40), (Z - 38) / 2.0 - 15), (18, 18, Z - 38 + 30))
        if k < legs - 1:                          # a brace across each bay, on the road's side
            x2 = x + (L - 80) / float(legs - 1)
            tilt_box(g, DECK_D, (x, y - D / 2.0 + 40, 30), (x2, y - D / 2.0 + 40, Z - 50), 10, 10)
    n = max(int(L / 150.0), 4)
    for k in range(n):
        g.box(PINK if (k + off) % 2 else WHITE, (-L / 2.0 + (k + 0.5) * L / n, y - D / 2.0 - 3, Z - 34), (L / n - 6, 6, 60))
    if tape:
        ty = y - D / 2.0 + 16
        posts = max(int(L / 260.0) + 1, 3)
        for k in range(posts):
            g.box(WHITE, (-L / 2.0 + 10 + k * (L - 20) / float(posts - 1), ty, Z + 55), (9, 9, 110))
        m = max(int(L / 130.0), 3)
        for k in range(m):
            x = -L / 2.0 + (k + 0.5) * L / m
            g.box(PINK if k % 2 else WHITE, (x, ty, Z + 98), (L / m, 4, 14))
            g.box(WHITE if k % 2 else PINK, (x, ty, Z + 58), (L / m, 4, 9))
    return g


def crew(sc, r, x, y, z, yaw=0.0, s=FIG, detail=1, **kw):
    """One of the crew on a deck: the Sea wardrobe (a suit, the helmet, the pack), the pink band across the chest."""
    d = C.member(r, W, band=PINK)
    d['s'] = s * r.uniform(0.95, 1.03)
    d.update(kw)
    if not d.get('phase'):
        d['phase'] = r.uniform(0.05, 0.95)       # never the still part's own key: a figure may not move the furniture's frame
    return P.figure(sc, x, y, z=z, yaw=yaw, detail=detail, **d)


def fig(sc, x, y, z, d, **kw):
    """A figure from a ready look (a dict of figure() values), FIG times life size unless kw says otherwise."""
    d = dict(d)
    d['s'] = FIG
    d.update(kw)
    d.setdefault('detail', 1)
    return P.figure(sc, x, y, z=z, **d)


def extra(sc, r, x, y, z, yaw=0.0, s=FIG, **kw):
    """An aquanaut of Nereida hired for the day (about 120 triangles): a suit of one strong colour, the white helmet."""
    kw.setdefault('phase', r.uniform(0.05, 0.95))
    WT.watcher(sc, r, x, y, z, yaw=yaw, s=s * r.uniform(0.94, 1.04), **kw)


def box_seat(g, x, y, Z, w=60.0, h=58.0, col=None):
    g.box(col or C.WOOD, (x, y, Z + h / 2.0), (w, 44, h))


def pennant(sc, x, y, z0, h, phase=0.0, w=130.0, hang=220.0, col=PINK, stripe=WHITE):
    """The unit's flag: a tall pole, a flat pennant of the crew's colours that swings a little about it."""
    g = sc.still()
    g.box(DECK_D, (x, y, z0 + h / 2.0), (9, 9, h))
    f = sc.part(P.motion(phase=phase, rate=0.4, swing=10.0, pivot=(x, y, z0 + h), axis=P.UP))
    cx = x + w / 2.0 + (5 if w > 0 else -5)
    f.box(col, (cx, y, z0 + h - hang / 2.0), (abs(w), 5, hang))
    f.box(stripe, (cx, y, z0 + h - hang / 2.0), (abs(w) + 2, 7, hang * 0.28))


def camera_post(sc, x, y, Z, phase, swing=20.0, rate=0.11, r=None, detail=1):
    """A film camera on its tripod on the deck, panning, and its operator panning with it."""
    g = sc.still(at=(0.0, 0.0, Z))
    C.tripod(g, x, y, top=150.0)
    sc.still()
    pan = dict(swing=swing, pivot=(x, y, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=rate, phase=phase, **pan))
    C.film_camera(cam, x, y, Z + 150.0)
    crew(sc, r, x, y + 92, Z, arm_l='push', arm_r='push', rate=rate, phase=phase, sway=pan, hat='helmet', detail=detail)


def hold_pad(col=DARK, trim=YEL):
    """A drone pilot's control pad, held in both hands."""
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 22, h[1] - 8, h[2] + 4), (48, 22, 9))
            m.box(trim, (h[0] - 22, h[1] - 8, h[2] + 10), (30, 10, 4))
            m.box(STEEL, (h[0] - 2, h[1] + 2, h[2] + 22), (4, 4, 34))
    return f


def hold_brush():
    def f(m, h, side):
        m.box(PINK, (h[0], h[1] - 14, h[2] + 4), (6, 34, 6))
        m.box(WHITE, (h[0], h[1] - 34, h[2] + 4), (10, 10, 10))
    return f


def drone(m, x, y, z, s=1.0, yaw=0.0):
    """A camera drone of the sea: a small yellow submersible box, a dark float on top, four thrusters, the lens to the
    front (-Y at yaw 0), two skids. About 110 triangles. (x, y, z) is its middle."""
    old = m.base
    m.base = P.ft_art._xf((x, y, z), (0.0, yaw, 0.0), (s, s, s))
    m.box(YEL, (0, 0, 0), (84, 64, 32))
    m.box(DARK, (0, 4, 22), (70, 50, 14))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(DARK, (sx * 50, sy * 30, 4), (18, 18, 26))
        m.box(STEEL, (sx * 34, 0, -26), (8, 76, 8))
        m.box(STEEL, (sx * 34, -20, -14), (6, 6, 20))
    m.box(DARK, (0, -36, 0), (30, 12, 26))
    m.box(STEEL, (0, -44, 0), (18, 6, 16))
    m.box(PINK, (0, 0, 31), (84, 12, 5))
    m.base = old


# ------------------------------------------------------------------------------------------ the pieces
def second_unit(seed=1, two=False):
    def build(sc):
        r = random.Random(seed)
        L, D, Z = (1000.0 if two else 900.0), 420.0, 260.0
        g = rostrum(sc, L, D, Z, off=seed)
        box_seat(g, -L / 2.0 + 110, 90, Z, w=70)
        C.case(g, L / 2.0 - 90, 120, z=Z)
        C.case(g, L / 2.0 - 84, 116, z=Z + 38, w=52, h=30, col=YEL)
        g.box(DARK, (0, 150, Z + 2), (L * 0.6, 8, 4))
        if two:
            camera_post(sc, -170, -40, Z, 0.2, r=r, detail=0)
            camera_post(sc, 190, -30, Z, 0.7, swing=26.0, rate=0.09, r=r, detail=0)
        else:
            camera_post(sc, 0, -40, Z, 0.2, r=r)
        # the unit's director on a box, megaphone at the face, pointing
        crew(sc, r, -L / 2.0 + 110, 90, Z, yaw=14, legs='sit', seat=46, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12,
             axis_l='x', rate=0.5, hat='helmet', top=WHITE)
        crew(sc, r, L / 2.0 - 230, -90, Z, yaw=16, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.35, bob=-3.0, detail=0)
        crew(sc, r, (-340 if two else -170), -110, Z, yaw=-22, arm_r='point', swing_r=14, axis_r='x', arm_l='hip', rate=0.6, detail=0)
        if not two:
            # one hangs in the water over the unit with a camera housing, the lens on the road
            S.swimmer(sc, 260, 110, Z + 400, heading=270, pitch=-18, s=1.3, suit=W['tops'][seed % 2], fins=DARK, arms='tow', kick=9.0,
                      rate=0.5, phase=0.4, bob=-18.0, band=PINK, bubbles=False, hold=S.hold_camera_housing())
        pennant(sc, -L / 2.0 + 20, 180, Z, 420, phase=0.1 * seed, w=-150.0)
        pennant(sc, L / 2.0 - 20, 180, Z, 360, phase=0.5, w=150.0, col=WHITE, stripe=PINK)
    return build


def drone_team(seed=1, three=False):
    def build(sc):
        r = random.Random(seed)
        L, D, Z = 820.0, 400.0, 260.0
        g = rostrum(sc, L, D, Z, off=seed)
        # the pad: a dark disc, a yellow one on it
        px = -L / 2.0 + 170
        g.cyl(DARK, (px, 0, Z), 100, 6, steps=10)
        g.cyl(YEL, (px, 0, Z + 6), 62, 4, steps=10)
        # the mast behind the deck, a ball on top (unlit), the circling drone's tether ring
        mx, my, mh = 170.0, 60.0, Z + 560.0
        g.box(STEEL, (mx, my, (Z + mh) / 2.0), (12, 12, mh - Z))
        g.sphere(PINK, (mx, my, mh + 16), 22, steps=(4, 6))
        g.cyl(DARK, (mx, my, Z), 34, 14, steps=8)
        C.case(g, L / 2.0 - 80, 110, z=Z, col=YEL)
        C.case(g, L / 2.0 - 190, 130, z=Z)
        # one hovers over its pad, rising and settling; one circles the mast, 2.2 m from it
        hov = sc.part(P.motion(phase=0.15, rate=0.3, bob=-34.0))
        drone(hov, px, 0, Z + 250, s=1.25, yaw=10)
        orb = sc.part(P.motion(phase=0.5, rate=0.35, bob=-14.0, base=(mx, my), travel=(-1.0, 0.06)))
        drone(orb, mx + 220, my, Z + 470, s=1.25, yaw=90)
        if three:
            # a third up on a pole at the deck's end: the camera on a stick
            g.box(STEEL, (-L / 2.0 + 30, 150, Z + 190), (10, 10, 380))
            tilt = sc.part(P.motion(phase=0.3, rate=0.12, swing=24.0, pivot=(-L / 2.0 + 30, 150, 0.0), axis=P.UP))
            drone(tilt, -L / 2.0 + 30, 150, Z + 400, s=1.15, yaw=0)
            drone(sc.still(), L / 2.0 - 190, 130, Z + 68, s=1.0, yaw=30)      # the spare, on its case
            sc.still()
        # the pilot, pad in both hands, looking up; the spotter pointing at the drone; a technician
        crew(sc, r, px + 230, 40, Z, yaw=40, arm_l='hold', arm_r='hold', hold_r=hold_pad(), rock=3, rate=0.3, hat='helmet', top=YEL)
        crew(sc, r, px + 110, 150, Z, yaw=70, arm_r='up', swing_r=12, arm_l='face', rate=0.5, bob=-3.0)
        crew(sc, r, 20, -100, Z, yaw=-15, arm_r='wave', swing_r=22, arm_l='hip', rate=0.7, react=True)
        if three:
            crew(sc, r, L / 2.0 - 250, 40, Z, yaw=-120, arm_l='carry', arm_r='carry', hold_r=P.hold_tool(), rate=0.4, bob=-2.0, detail=0)
        pennant(sc, L / 2.0 - 20, 170, Z, 380, phase=0.2 * seed, w=140.0)
    return build


def stunt_rig(seed=1):
    def build(sc):
        r = random.Random(seed)
        L, D, Z = 960.0, 420.0, 240.0
        g = rostrum(sc, L, D, Z, off=seed)
        TOP, TX, TY = 1000.0, 400.0, 60.0
        for sx in (-1, 1):
            for dx in (-45, 45):
                g.box(STEEL, (sx * TX + dx, TY, (Z + TOP) / 2.0), (16, 16, TOP - Z))
            for k in range(3):                    # the tower's lacing, yellow and dark in turn
                z0 = Z + 20 + k * (TOP - Z - 40) / 3.0
                z1 = z0 + (TOP - Z - 40) / 3.0
                tilt_box(g, YEL if k % 2 else DARK, (sx * TX - 45, TY, z0), (sx * TX + 45, TY, z1), 10, 10)
        g.box(YEL, (0, TY, TOP + 12), (2 * TX + 160, 30, 30))
        for k in range(7):
            g.box(DARK, (-TX + 60 + k * (2 * TX - 120) / 6.0, TY, TOP + 12), (22, 34, 34))
        g.box(DECK, (-TX, TY - 20, 620), (190, 170, 12))                    # the waiting platform on the left tower
        g.box(PINK, (-TX, TY - 108, 640), (190, 6, 40))
        # the air bag on the deck under the swing, breathing on its fan
        bag = sc.part(P.motion(phase=0.2, rate=0.3, bob=-6.0))
        bag.box('#2e6bff', (40, TY - 10, Z + 70), (380, 250, 140))
        bag.box('#57c7ff', (40, TY - 10, Z + 144), (310, 190, 8))
        bag.box(WHITE, (40, TY - 10, Z + 150), (110, 110, 4))
        g.box(GREY, (L / 2.0 - 70, TY + 90, Z + 30), (60, 60, 60))
        # the performer on the line: the whole figure and the line swing about the beam, along the road
        swing = dict(swing=20.0, pivot=(40.0, TY, TOP), axis=90.0)
        rope = sc.part(P.motion(phase=0.3, rate=0.2, **swing))
        rope.box(WHITE, (40, TY, TOP - 170), (6, 6, 340))
        rope.box(DARK, (40, TY, TOP - 345), (40, 8, 10))
        fig(sc, 40, TY, TOP - 345 - FIG * 176, C.member(r, W, top=ORANGE, bottom=ORANGE, band=None, hat='helmet', hat_col=WHITE),
            arm_l='up', arm_r='up', legs='wide', phase=0.3, rate=0.2, sway=swing)
        # the second performer waits on the tower; riggers on the deck; the coordinator
        fig(sc, -TX, TY - 30, 626, C.member(r, W, top=LIME, bottom=LIME, band=None, hat='helmet', hat_col=RED),
            arm_l='vee', arm_r='vee', bob=10, rate=0.5, phase=0.6, react=True)
        crew(sc, r, TX - 90, -60, Z, yaw=-100, arm_l='push', arm_r='push', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rate=0.4)
        crew(sc, r, -210, -130, Z, yaw=-10, arm_r='up', swing_r=14, arm_l='hold', hold_l=C.hold_clipboard(), rate=0.5)
        crew(sc, r, 300, -140, Z, yaw=20, arm_l='hip', arm_r='point', swing_r=10, axis_r='x', rate=0.6, bob=-2.0)
        pennant(sc, TX + 60, TY, TOP, 240, phase=0.4, w=150.0, hang=180.0)
    return build


def extras(seed=1, queue=False):
    def build(sc):
        r = random.Random(seed)
        L, D, Z = 1040.0, 440.0, 250.0
        g = rostrum(sc, L, D, Z, off=seed)
        # the canopy over the back of the deck: pink and white, on four poles
        CZ = Z + 330.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(STEEL, (sx * (L / 2.0 - 30), 70 + sy * 130, (Z + CZ) / 2.0), (8, 8, CZ - Z))
        n = 8
        for k in range(n):
            g.box(PINK if k % 2 else WHITE, (-L / 2.0 + (k + 0.5) * L / n, 70, CZ + 6), (L / n, 300, 12))
        if queue:
            # the costume rail: suits of every colour on a bar
            rx = L / 2.0 - 190
            for sx in (-1, 1):
                g.box(STEEL, (rx + sx * 130, 120, Z + 100), (8, 8, 200))
            g.box(STEEL, (rx, 120, Z + 200), (270, 8, 8))
            for k, col in enumerate((YEL, ORANGE, PINK, LIME, RED, WHITE)):
                g.box(col, (rx - 105 + k * 42, 120, Z + 130), (30, 14, 120))
            acts = (dict(arm_l='down', arm_r='hip', bob=-5, rate=0.4), dict(arm_r='wave', swing_r=26, arm_l='down', bob=8, rate=0.7, react=True),
                    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=-10, rate=0.5, react=True), dict(arm_l='hold', arm_r='hold', bob=-4, rate=0.3),
                    dict(arm_r='point', swing_r=12, axis_r='x', arm_l='down', bob=-6, rate=0.6, react=True))
            for i in range(9):
                x = -L / 2.0 + 90 + i * 84 + r.uniform(-10, 10)
                extra(sc, r, x, -40 + (i % 2) * 70 + r.uniform(-8, 8), Z, yaw=(60 if i % 3 else -10) + r.uniform(-14, 14), **acts[(i * 3 + seed) % len(acts)])
            crew(sc, r, rx - 150, -20, Z, yaw=-70, arm_l='hold', arm_r='point', hold_l=C.hold_clipboard(), swing_r=10, axis_r='x', rate=0.5, top=WHITE)
        else:
            sit = (dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, rate=0.6, react=True), dict(arm_r='wave', swing_r=28, arm_l='down', rate=0.8, react=True),
                   dict(arm_l='hold', arm_r='hold', rate=0.3, bob=-2), dict(arm_l='down', arm_r='face', rate=0.3, bob=-2))
            for row, by in enumerate((150.0, 20.0)):
                g = sc.still()
                g.box(C.WOOD, (-120, by, Z + 29), (L - 420, 44, 58))
                for i in range(4):
                    x = -L / 2.0 + 160 + i * 160 + r.uniform(-12, 12) + row * 30
                    extra(sc, r, x, by, Z, yaw=r.uniform(-12, 12), legs='sit', seat=46, **sit[(i * 3 + row + seed) % len(sit)])
            stand = (dict(arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=14, rate=0.55, react=True),
                     dict(arm_r='wave', swing_r=28, arm_l='hip', bob=10, rate=0.75, react=True),
                     dict(arm_r='up', hold_r=P.hold_sign(col=WHITE, stripe=PINK), arm_l='down', swing_r=8, bob=-6, rate=0.5))
            for i, x in enumerate((L / 2.0 - 300, L / 2.0 - 190, L / 2.0 - 90)):
                extra(sc, r, x, -90 + (i % 2) * 90, Z, yaw=r.uniform(-20, 10), **stand[i])
            crew(sc, r, -L / 2.0 + 80, -120, Z, yaw=30, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=10, axis_l='x', rate=0.5, top=WHITE)
        pennant(sc, -L / 2.0 + 20, 200, CZ - 40, 300, phase=0.3, w=-140.0, hang=180.0)
    return build


def make_up(seed=1):
    def build(sc):
        r = random.Random(seed)
        L, D, Z = 800.0, 420.0, 250.0
        g = rostrum(sc, L, D, Z, off=seed)
        CZ = Z + 320.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(STEEL, (sx * (L / 2.0 - 60), 50 + sy * 140, (Z + CZ) / 2.0), (8, 8, CZ - Z))
        n = 6
        for k in range(n):                        # the tent's roof, a peak down its middle
            g.box(PINK if k % 2 else WHITE, (-L / 2.0 + 60 + (k + 0.5) * (L - 120) / n, 50, CZ + 8), ((L - 120) / n, 320, 14))
        g.box(WHITE, (0, 50, CZ + 34), (L - 180, 120, 40))
        g.box(PINK, (0, 50, CZ + 62), (L - 260, 40, 18))
        g.box(WHITE, (0, 196, Z + 170), (L - 130, 6, 300))                  # the back wall
        g.box(PINK, (0, 192, Z + 250), (L - 130, 4, 40))
        # the mirror, its lamps steady (this piece stands 24 m out or more)
        mxx = -150.0
        g.box(DARK, (mxx, 186, Z + 170), (190, 8, 150))
        g.box('#9ff0ff', (mxx, 181, Z + 170), (160, 3, 120))
        for k in range(5):
            g.box(LAMP, (mxx - 76 + k * 38, 179, Z + 242), (16, 4, 12), glow=0.5)
        g.box(WHITE, (mxx, 150, Z + 44), (210, 60, 88))                     # the table under it
        for k, col in enumerate((PINK, YEL, LIME, ORANGE)):
            g.box(col, (mxx - 70 + k * 46, 146, Z + 94), (22, 22, 12))
        box_seat(g, mxx, 60, Z, w=64, col=DARK)
        # the helmet rack
        hx = L / 2.0 - 150
        g.box(STEEL, (hx, 170, Z + 80), (200, 40, 8))
        g.box(STEEL, (hx, 170, Z + 160), (200, 40, 8))
        for sx in (-1, 1):
            g.box(STEEL, (hx + sx * 96, 170, Z + 90), (8, 40, 180))
        for k in range(3):
            if k != 1:
                g.sphere(HELM, (hx - 62 + k * 62, 168, Z + 110), 26, steps=(4, 6))
            else:
                g.sphere(YEL, (hx, 168, Z + 190), 26, steps=(4, 6))
                g.sphere(HELM, (hx - 62, 168, Z + 190), 26, steps=(4, 6))
        # the performer in the chair, helmet off; the artist at work; one holds the mirror light; one waits with a cup
        fig(sc, mxx, 60, Z, P.look(r, top=ORANGE, bottom=ORANGE, sleeves='long', wear='trousers', hair='afro'),
            yaw=180, legs='sit', seat=46, arm_l='down', arm_r='down', rate=0.25, bob=-1.5, phase=0.2)
        crew(sc, r, mxx + 96, 40, Z, yaw=110, arm_r='fwd', swing_r=12, axis_r='x', hold_r=hold_brush(), arm_l='hip', rate=0.9, top=WHITE, hat='headset', hat_col=DARK,
             pack=None)
        crew(sc, r, mxx - 110, -40, Z, yaw=-150, arm_l='up', arm_r='hold', rock=3, rate=0.3, pack=None)
        fig(sc, 150, -90, Z, C.member(r, W, top=LIME, bottom=LIME, band=None, hat='helmet', hat_col=WHITE),
            yaw=-10, arm_r='hold', hold_r=P.hold_cup(), arm_l='wave', swing_l=24, rate=0.7, bob=4, phase=0.5, react=True)
        crew(sc, r, 290, 20, Z, yaw=-30, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=PINK), rate=0.3, bob=-2.0)
        pennant(sc, L / 2.0 - 30, 200, CZ, 260, phase=0.6, w=140.0, hang=170.0)
    return build


def press_pen(seed=1, tiers=False):
    def build(sc):
        r = random.Random(seed)
        L, D, Z = 900.0, 400.0, 250.0
        g = rostrum(sc, L, D, Z, off=seed)
        if tiers:
            g.box(DECK, (0, 110, Z + 35), (L - 60, 150, 70))
            g.box(DECK_D, (0, 34, Z + 35), (L - 60, 4, 70))
        else:
            g.box(C.WOOD, (L / 2.0 - 150, 60, Z + 30), (70, 60, 60))        # the step
            g.box(C.WOOD, (L / 2.0 - 150, 100, Z + 75), (70, 40, 30))
        C.case(g, -L / 2.0 + 80, 140, z=Z)
        acts = (dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), bob=10, rate=0.6),
                dict(arm_r='face', hold_r=P.hold_camera(), arm_l='hip', bob=6, rate=0.5),
                dict(arm_l='hold', arm_r='up', hold_r=P.hold_camera(), swing_r=8, bob=14, rate=0.7),
                dict(arm_r='wave', swing_r=26, arm_l='hold', hold_l=P.hold_phone(), bob=8, rate=0.8),
                dict(arm_l='face', hold_l=P.hold_camera(), arm_r='point', swing_r=10, axis_r='x', bob=-4, rate=0.6))
        n_front = 5 if tiers else 7
        for i in range(n_front):
            x = -L / 2.0 + 90 + i * (L - 180) / float(n_front - 1) + r.uniform(-12, 12)
            extra(sc, r, x, -110 + (i % 2) * 50, Z, yaw=r.uniform(-16, 16), react=True, **acts[(i * 2 + seed) % len(acts)])
        if tiers:
            for i in range(3):
                x = -L / 2.0 + 170 + i * (L - 340) / 2.0 + r.uniform(-12, 12)
                extra(sc, r, x, 110, Z + 70, yaw=r.uniform(-12, 12), **(dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=16, rate=0.55, react=True) if i % 2
                                                                          else dict(arm_r='wave', swing_r=28, arm_l='hip', bob=10, rate=0.8, react=True)))
            for k, (x, z, ph) in enumerate(((-250, Z + 470, 0.2), (240, Z + 540, 0.7))):
                S.swimmer(sc, x, 60, z, heading=270 + (12 if k else -12), pitch=-22, s=1.3, suit=S.SUITS[(seed + 2 * k + 2) % len(S.SUITS)], fins=S.FINS[k], arms='tow',
                          kick=8.0, rate=0.45, phase=ph, bob=-22.0, bubbles=False, hold=S.hold_camera_housing())
        else:
            extra(sc, r, L / 2.0 - 150, 62, Z + 60, yaw=-14, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), bob=5, rate=0.5, react=True)
            extra(sc, r, -L / 2.0 + 190, 60, Z, yaw=6, arm_l='up', arm_r='up', hold_r=C.hold_boom(length=220.0), rock=3, rate=0.25)
        crew(sc, r, 0, 150, Z, yaw=0, arm_l='out', arm_r='out', rock=4, rate=0.4, top=WHITE)       # the unit's publicist holds them back
        pennant(sc, -L / 2.0 + 20, 180, Z + (70 if tiers else 0), 380, phase=0.15, w=-140.0, col=WHITE, stripe=PINK)
    return build


def cable_cam(seed=1):
    def build(sc):
        r = random.Random(seed)
        L, D, Z = 620.0, 400.0, 250.0
        g = rostrum(sc, L, D, Z, off=seed)
        MX, MH = 640.0, 700.0
        for sx in (-1, 1):
            g.box(GREY, (sx * MX, 40, 20), (130, 130, 40))
            g.box(YEL, (sx * MX, 40, MH / 2.0 + 20), (24, 24, MH))
            for k in range(3):
                g.box(DARK, (sx * MX, 40, 150 + k * 220), (30, 30, 36))
            tilt_box(g, STEEL, (sx * MX, 40, MH - 40), (sx * (MX - 190), 40, Z - 20), 8, 8)
            g.box(PINK, (sx * MX, 40, MH + 30), (60, 60, 30))
        g.box(STEEL, (0, 40, MH - 6), (2 * MX, 6, 6))
        g.box(STEEL, (0, 40, MH - 46), (2 * MX, 5, 5))
        # the carriage and its diver slide about 2.5 m each way: a small swing about a pivot 60 m under the sand
        slide = dict(swing=2.2, pivot=(0.0, 40.0, -6000.0), axis=90.0)
        car = sc.part(P.motion(phase=0.1, rate=0.07, **slide))
        car.box(DARK, (0, 40, MH - 26), (110, 40, 52))
        car.box(YEL, (0, 40, MH - 62), (70, 30, 20))
        car.box(STEEL, (0, 40, MH - 96), (10, 10, 50))
        C.film_camera(car, 0, 24, MH - 160)
        S.swimmer(sc, -150, 40, MH - 150, heading=0, pitch=0, s=1.3, suit=W['tops'][seed % 2], fins=DARK, arms='tow', kick=0.0, rate=0.07, phase=0.1,
                  band=PINK, bubbles=False, sway=slide)
        # on the deck: the focus puller at the remote, an assistant following the carriage with an arm, a loader
        g = sc.still()
        g.box(GREY, (-60, 100, Z + 45), (90, 56, 90))
        g.box(DARK, (-60, 70, Z + 100), (70, 8, 44))
        C.case(g, L / 2.0 - 80, 130, z=Z, col=YEL)
        crew(sc, r, -60, 10, Z, yaw=180, arm_l='hold', arm_r='hold', hold_r=hold_pad(), rate=0.3, bob=-2.0, hat='helmet')
        crew(sc, r, 120, -90, Z, yaw=-20, arm_r='up', swing_r=18, arm_l='hip', rate=0.14)
        crew(sc, r, -210, -110, Z, yaw=24, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.35, bob=-3.0)
    return build


def light_balloon(seed=1):
    def build(sc):
        r = random.Random(seed)
        L, D, Z = 760.0, 400.0, 250.0
        g = rostrum(sc, L, D, Z, off=seed)
        g.box(YEL, (L / 2.0 - 110, 120, Z + 36), (120, 64, 72))               # the generator
        g.box(DARK, (L / 2.0 - 110, 120, Z + 76), (100, 48, 8))
        for sx in (-1, 1):
            g.box(GREY, (sx * 200, 60, Z + 16), (50, 50, 32))                # the ballast the lines are made fast to
        g.box(GREY, (0, -120, Z + 16), (50, 50, 32))
        # the balloon light floats on its lines, rising and settling together with them; its glow is steady
        BZ = Z + 560.0
        b = sc.part(P.motion(phase=0.2, rate=0.16, bob=-16.0))
        b.sphere(LAMP, (0, 20, BZ), 170, scale=(1.0, 1.0, 0.72), steps=(6, 10), glow=0.6)
        b.cyl(WHITE, (0, 20, BZ - 140), 60, 26, steps=8, top_r=110)
        b.box(PINK, (0, 20, BZ - 150), (70, 70, 12))
        for (ax, ay) in ((-200, 60), (200, 60), (0, -120)):
            tilt_box(b, WHITE, (ax, ay, Z + 30), (ax * 0.25, 20 + (ay - 20) * 0.25, BZ - 140), 5, 5)
        # the reflector on its stand, turning a little
        g.box(STEEL, (-L / 2.0 + 110, 60, Z + 110), (6, 6, 220))
        ref = sc.part(P.motion(phase=0.4, rate=0.1, swing=14.0, pivot=(-L / 2.0 + 110, 60, 0.0), axis=P.UP))
        tilt_box(ref, WHITE, (-L / 2.0 + 110, 56, Z + 130), (-L / 2.0 + 110, 20, Z + 330), 200, 5)
        # the sparks: one hauls on a line, one at the generator, the gaffer points
        crew(sc, r, 150, 20, Z, yaw=-60, arm_l='up', arm_r='up', swing_l=8, swing_r=8, axis_l='x', axis_r='x', rate=0.32)
        crew(sc, r, L / 2.0 - 200, 60, Z, yaw=-100, arm_l='carry', arm_r='carry', hold_r=P.hold_tool(), rate=0.4, bob=-2.0)
        crew(sc, r, -110, -110, Z, yaw=-14, arm_r='up', swing_r=12, arm_l='hip', rate=0.5, top=WHITE)
        crew(sc, r, 270, -120, Z, yaw=12, arm_r='wave', swing_r=24, arm_l='down', rate=0.8, bob=4, react=True)
    return build


def kit_deck(seed=1):
    """A bare rostrum for a scene of the SHARED crew kit (/Game/People/Crew/SM_Crew<Piece>_Sea): the block's Sets stand
    a kit piece on it, 2.5 m up, so that the main unit's people clear the barrier too. 15 m by 5.4 m, nothing on it
    between the ends (the kit's dolly rails are 13 m at the size it is placed). A safety diver hangs in the water at
    one end; the unit's flags at the back corners."""
    def build(sc):
        L, D, Z = 1500.0, 540.0, 250.0
        g = rostrum(sc, L, D, Z, off=seed, tape=False, legs=4)
        sx = -1 if seed % 2 else 1
        for k in range(4):                        # the ladder at one end
            g.box(DECK_D, (sx * (L / 2.0 + 22), 120, 40 + k * 60), (40, 60, 8))
        for sy in (-1, 1):
            g.box(DECK_D, (sx * (L / 2.0 + 22), 120 + sy * 32, Z / 2.0 + 30), (8, 8, Z + 60))
        g.box(YEL, (-sx * (L / 2.0 - 40), 230, Z + 22), (60, 50, 44))
        S.swimmer(sc, sx * (L / 2.0 + 150), 60, Z + 300, heading=270 - sx * 20, pitch=24, s=1.3, suit=W['tops'][seed % 2], fins=YEL if seed % 2 else DARK,
                  arms='wave', wave=18.0, kick=8.0, rate=0.5, phase=0.3 * seed, bob=-16.0, band=PINK, bubbles=False)
        pennant(sc, -L / 2.0 + 20, D / 2.0 - 16, Z, 440, phase=0.2 * seed, w=-150.0)
        pennant(sc, L / 2.0 - 20, D / 2.0 - 16, Z, 380, phase=0.6, w=150.0, col=WHITE, stripe=PINK)
        if seed % 2 == 0:
            pennant(sc, 0, D / 2.0 - 16, Z, 500, phase=0.4, w=150.0, col=YEL, stripe=PINK)
    return build


TALLER = dict(grow=(60.0, 60.0, 80.0))
JOBS = [
    ('SM_P3F_SecondUnit_A', second_unit(1), dict(TALLER)),
    ('SM_P3F_SecondUnit_B', second_unit(2, two=True), dict(TALLER)),
    ('SM_P3F_DroneTeam_A', drone_team(1), dict(grow=(80.0, 150.0, 90.0), grow_down=(80.0, 60.0, 0.0))),
    ('SM_P3F_DroneTeam_B', drone_team(2, three=True), dict(grow=(80.0, 150.0, 90.0), grow_down=(80.0, 60.0, 0.0))),
    ('SM_P3F_StuntRig', stunt_rig(1), dict(TALLER)),
    ('SM_P3F_Extras_A', extras(1), dict(TALLER)),
    ('SM_P3F_Extras_B', extras(2, queue=True), dict(TALLER)),
    ('SM_P3F_MakeUp', make_up(1), dict(TALLER)),
    ('SM_P3F_PressPen_A', press_pen(1), dict(TALLER)),
    ('SM_P3F_PressPen_B', press_pen(2, tiers=True), dict(TALLER)),
    ('SM_P3F_CableCam', cable_cam(1), dict(grow=(300.0, 60.0, 80.0), grow_down=(300.0, 60.0, 0.0))),
    ('SM_P3F_LightBalloon', light_balloon(1), dict(TALLER)),
    ('SM_P3F_Deck_A', kit_deck(1), dict(TALLER)),
    ('SM_P3F_Deck_B', kit_deck(2), dict(TALLER)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
