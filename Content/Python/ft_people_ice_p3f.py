"""FULL THROTTLE (Unreal) the population of level 5, Ice Mountain: pass 3, builder p3f, THE FILM SHOOT
(/Game/People/IceMountain/SM_P3F_*).

More of the shoot than the shared crew kit has: the units a ski-chase film would bring up a mountain. Every piece is
built with ft_people_crew's helpers in the Snow wardrobe (padded coats, bobble hats, the pink band across the chest),
behind the crew's pink-and-white tape, with the kit's yellow-and-dark equipment.

    SM_P3F_SecondUnit_A   a scaffold camera tower: the camera panning on its deck, operator and spotter up top, the
                          director and the clapper loader below
    SM_P3F_SecondUnit_B   the long-lens position on a sledge under a parasol: operator, spotter, first assistant
    SM_P3F_DroneTeam_A    a drone hovering over its landing pad (5.6 m up), pilot, spotter, a technician at the monitor
    SM_P3F_DroneTeam_B    two drones circling a mast (6 and 6.8 m up), two pilots, the aerial technician
    SM_P3F_StuntRig       a truss goalpost: a stunt performer swinging on wires over the crash mats, two riggers on
                          the rope, the coordinator
    SM_P3F_Extras_A       the extras' holding area: a riser of skiers in bibs keeping warm, their wrangler
    SM_P3F_Extras_B       wardrobe: a rail of coats, a rack of skis, extras filing past and round again
    SM_P3F_MakeUp         the make-up tent: an actor in the chair, the make-up artist, the hairdresser, the next one
    SM_P3F_PressPen       the press pen: photographers on a riser, a reporter talking to a camera
    SM_P3F_CableCam       a cable camera: its trolley sliding to and fro between two masts, the winch, the desk
    SM_P3F_WindMachine    the snow unit: a wind machine blowing snow past an actor, two grips shovelling

Rules kept here: nothing glows and nothing blinks; no words or marks (stripes only); the drones are dark grey and pink,
never orange or green, and fly by bob and orbit only; nothing big, white and furry (the level's boss is the Abominable).
The table block is C:/ftwork/p3/L5f.inl (written by C:/ftwork/jobs/L5p3f/make_inl.py).

Run in the editor:  import ft_people_ice_p3f as K; K.run()        (or K.run(['SM_P3F_StuntRig']))
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/IceMountain'
W = C.WARDROBES['Snow']

PINK, WHITE, DARK, GREY, YEL, STEEL, WOOD = C.PINK, C.WHITE, C.DARK, C.GREY, C.YEL, C.STEEL, C.WOOD
SNOW, RED, BLUE, TEAL, ORANGE, VIOLET = '#f4f9ff', '#ff2d3c', '#2e6bff', '#19b5c9', '#ff8a2a', '#8f6bff'
SUITS = (RED, '#ff5fa2', ORANGE, '#ffd23f', '#b8ff5a', '#2ee6ff', BLUE, VIOLET, '#7dffb0')


def crew(r, **fixed):
    return C.member(r, W, **fixed)


def extra(r, **fixed):
    """An extra dressed as a skier: a hot suit, a helmet or a bobble hat, a white bib across the chest."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear='trousers', top=c, bottom=r.choice((c, DARK, '#3a5fa8')), hat=r.choice(('helmet', 'beanie', 'beanie')),
               hat_col=r.choice(SUITS), shoes=DARK, hair='short', band=WHITE, s=r.uniform(0.94, 1.04))
    d.update(fixed)
    return d


def deck(g, x, y, w, d, z, skirt=(PINK, WHITE), legs=True):
    """A scaffold deck w by d, its top at z, a striped skirt along its front."""
    g.box(WOOD, (x, y, z - 6), (w, d, 12))
    if legs:
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(STEEL, (x + sx * (w / 2.0 - 8), y + sy * (d / 2.0 - 8), (z - 12) / 2.0), (8, 8, z - 12))
    n = max(int(w / 90.0), 2)
    h = min(z - 12.0, 46.0)
    for k in range(n):
        g.box(skirt[k % len(skirt)], (x - w / 2.0 + (k + 0.5) * w / n, y - d / 2.0 - 2, z - 12 - h / 2.0), (w / n, 4, h))


def rail(g, x0, x1, y, z, h=100.0):
    """A guard rail from x0 to x1 at y, standing on z."""
    for x in (x0, x1):
        g.box(STEEL, (x, y, z + h / 2.0), (6, 6, h))
    g.box(YEL, ((x0 + x1) / 2.0, y, z + h), (abs(x1 - x0) + 6, 6, 6))


def hold_binoculars():
    def f(m, h, side):
        m.box(DARK, (h[0] - side * 6, h[1] - 8, h[2] + 4), (22, 16, 9))
    return f


def hold_controller():
    """A drone pilot's controller in both hands: a dark slab, two sticks, a screen hood (no glow)."""
    def f(m, h, side):
        if side < 0:
            return
        cx = h[0] - 22
        m.box(DARK, (cx, h[1] - 6, h[2] + 6), (44, 16, 9))
        m.box(GREY, (cx, h[1] - 14, h[2] + 18), (30, 4, 20))
        m.box(PINK, (cx, h[1] - 6, h[2] + 11), (44, 4, 3))
    return f


def hold_mic(col=DARK, head=TEAL):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 3, h[2] + 12), (5, 5, 24))
        m.box(head, (h[0], h[1] - 3, h[2] + 28), (10, 10, 12))
    return f


def hold_brush():
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 10, h[2] + 4), (4, 22, 4))
        m.box(PINK, (h[0], h[1] - 24, h[2] + 4), (7, 8, 7))
    return f


def hold_shovel():
    def f(m, h, side):
        if side < 0:
            return
        tilt_box(m, WOOD, (h[0] - 20, h[1] + 10, h[2] + 20), (h[0] - 20, h[1] - 70, h[2] - 40), 5, 5)
        m.box(STEEL, (h[0] - 20, h[1] - 84, h[2] - 50), (30, 34, 5))
        m.box(SNOW, (h[0] - 20, h[1] - 84, h[2] - 42), (24, 26, 12))
    return f


def drone(m, x, y, z, s=1.0):
    """A camera drone, big enough to read: dark body, pink stripe, four arms and rotor discs, the camera slung under."""
    m.box(DARK, (x, y, z), (70 * s, 54 * s, 20 * s))
    m.box(PINK, (x, y, z + 11 * s), (72 * s, 14 * s, 4 * s))
    for sx in (-1, 1):
        for sy in (-1, 1):
            hx, hy = x + sx * 72 * s, y + sy * 66 * s
            tilt_box(m, GREY, (x + sx * 20 * s, y + sy * 16 * s, z + 2 * s), (hx, hy, z + 8 * s), 8 * s, 6 * s)
            m.cyl(STEEL, (hx, hy, z + 12 * s), 34 * s, 4.5 * s, steps=7)
            m.cyl(DARK, (hx, hy, z + 4 * s), 6 * s, 12 * s, steps=5)
    m.box(GREY, (x, y - 8 * s, z - 20 * s), (26 * s, 30 * s, 22 * s))
    m.box(STEEL, (x, y - 28 * s, z - 20 * s), (14 * s, 12 * s, 14 * s))
    for sx in (-1, 1):
        m.box(DARK, (x + sx * 30 * s, y, z - 24 * s), (5 * s, 50 * s, 5 * s))
        m.box(DARK, (x + sx * 30 * s, y, z - 14 * s), (5 * s, 5 * s, 18 * s))


def monitor(g, x, y, z=0.0):
    """A monitor on a stand under its hood, its screen away from the road (no glow)."""
    g.box(STEEL, (x, y, z + 55), (6, 6, 110))
    g.box(DARK, (x, y, z + 4), (44, 44, 8))
    g.box(DARK, (x, y, z + 132), (70, 10, 46))
    g.box('#3a5fa8', (x, y + 5.5, z + 132), (60, 1.5, 38))
    g.box(DARK, (x, y + 22, z + 158), (78, 44, 4))
    for sx in (-1, 1):
        g.box(DARK, (x + sx * 37, y + 22, z + 134), (3, 44, 48))


# ------------------------------------------------------------------------------------------ the units
def second_unit_a(seed=101):
    """A scaffold camera tower, its deck 2.2 m up. TALL."""
    def build(sc):
        r = random.Random(seed)
        Z = 220.0
        g = sc.still()
        C.tape(g, 640, -215, posts=4)
        deck(g, 0, 0, 380, 270, Z)
        for sx in (-1, 1):
            tilt_box(g, STEEL, (sx * 182, -127, 20), (sx * 182, 127, Z - 20), 5, 5)
        rail(g, -186, 186, 131, Z)
        for sx in (-1, 1):
            g.box(STEEL, (sx * 186, -131, Z + 50), (6, 6, 100))
            g.box(YEL, (sx * 186, 0, Z + 100), (6, 268, 6))
        for sy in (-1, 1):                       # the ladder up the right-hand end
            tilt_box(g, YEL, (250, 60 + sy * 24, 0), (196, 60 + sy * 24, Z), 6, 6)
        for k in range(4):
            g.box(YEL, (242 - k * 13.5, 60, 36 + k * 50), (6, 48, 6))
        gd = sc.still(at=(0, -40, Z))
        C.tripod(gd, 0, 0)
        g = sc.still()
        C.case(g, -120, 80, z=Z, w=60, h=34)
        C.case(g, -290, 90)
        C.case(g, -278, 86, z=38, w=52, h=30, col=YEL)
        C.apple_box(g, 300, -40)
        pan = dict(swing=26.0, pivot=(0.0, -40.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.12, phase=0.15, **pan))
        C.film_camera(cam, 0, -40, Z + 128)
        cam.box(GREY, (0, -112, Z + 145), (20, 54, 20))
        P.figure(sc, 0, 42, z=Z, arm_l='push', arm_r='push', rate=0.12, phase=0.15, sway=pan, **crew(r, hat='headset', hat_col=DARK))
        P.figure(sc, 120, -60, z=Z, yaw=-18, arm_r='face', hold_r=hold_binoculars(), arm_l='point', swing_l=10, axis_l='x', rate=0.5,
                 phase=r.random(), **crew(r))
        P.figure(sc, -230, -110, yaw=14, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12, axis_l='x', rate=0.55,
                 phase=r.random(), detail=1, **crew(r, hat='cap', hat_col=DARK))
        P.figure(sc, 250, -120, yaw=-16, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.4, bob=-1.5, phase=r.random(), detail=1, **crew(r))
        P.figure(sc, -70, -150, yaw=6, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.5, react=True, phase=r.random(), detail=1, **crew(r))
    return build


def second_unit_b(seed=102):
    """The long-lens position: a sledge with the camera under a parasol. TALL (the parasol)."""
    def build(sc):
        r = random.Random(seed)
        Z = 58.0
        g = sc.still()
        C.tape(g, 640, -200, posts=4)
        for sy in (-1, 1):                       # the sledge: two runners, a yellow bed
            g.box(DARK, (0, sy * 80, 8), (380, 12, 16))
            tilt_box(g, DARK, (190, sy * 80, 8), (230, sy * 80, 44), 12, 12)
        g.box(YEL, (0, 0, Z - 8), (340, 190, 16))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(STEEL, (sx * 140, sy * 80, 30), (8, 8, 36))
        gd = sc.still(at=(-40, -30, Z))
        C.tripod(gd, 0, 0, top=118.0)
        g = sc.still()
        g.box(STEEL, (110, 50, Z + 150), (7, 7, 300))                       # the parasol's pole
        C.case(g, 110, -50, z=Z, w=56, h=30)
        C.case(g, -280, 80)
        C.apple_box(g, 260, -70)
        shade = sc.part(P.motion(swing=3.0, pivot=(110.0, 50.0, Z + 290.0), axis=0.0, rate=0.3, phase=0.4))
        shade.cyl(PINK, (110, 50, Z + 250), 175, 50, steps=8, top_r=14)
        shade.cyl(WHITE, (110, 50, Z + 243), 181, 8, steps=8)
        pan = dict(swing=18.0, pivot=(-40.0, -30.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.1, phase=0.6, **pan))
        C.film_camera(cam, -40, -30, Z + 118)
        rod(cam, GREY, (-40, -90, Z + 135), (-40, -210, Z + 135), 13, steps=7)     # the long lens
        rod(cam, DARK, (-40, -210, Z + 135), (-40, -240, Z + 135), 18, steps=7)
        P.figure(sc, -40, 52, z=Z, arm_l='push', arm_r='push', rate=0.1, phase=0.6, sway=pan, **crew(r, hat='headset', hat_col=DARK))
        P.figure(sc, 260, -70, z=30, yaw=-14, arm_r='face', arm_l='face', hold_r=hold_binoculars(), rate=0.3, bob=-1.0, phase=r.random(), **crew(r))
        P.figure(sc, -230, -90, yaw=18, arm_r='point', swing_r=12, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.6,
                 phase=r.random(), detail=1, **crew(r))
        P.figure(sc, 120, -150, yaw=-8, arm_r='wave', swing_r=24, bob=4, rate=1.2, react=True, phase=r.random(), detail=1, **crew(r))
    return build


def drone_team_a(seed=103):
    """A drone hovering over its pad, 5.6 m up; the pilot, the spotter, a technician. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.tape(g, 640, -230, posts=4)
        g.box(PINK, (0, -40, 3), (270, 270, 6))                            # the landing pad: a white cross on pink
        g.box(WHITE, (0, -40, 6.5), (210, 40, 2))
        g.box(WHITE, (0, -40, 6.5), (40, 210, 2))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.cyl(YEL, (sx * 150, -40 + sy * 150, 0), 9, 34, steps=6, top_r=4)
        C.case(g, 250, 70, w=90, d=60, h=44)
        drone(g, 250, 70, 76, s=0.8)                                       # the spare, on its case
        monitor(g, -250, 90)
        C.apple_box(g, -250, 150)
        hov = sc.part(P.motion(bob=-38.0, rate=0.3, phase=0.2, swing=7.0, pivot=(0.0, -40.0, 560.0), axis=0.0))
        drone(hov, 0, -40, 560)
        P.figure(sc, -90, 120, yaw=-8, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.0, phase=r.random(),
                 **crew(r, hat='headset', hat_col=DARK))
        P.figure(sc, 90, 110, yaw=10, arm_r='up', swing_r=9, arm_l='face', rate=0.5, phase=r.random(), **crew(r))
        P.figure(sc, -250, 150, z=30, yaw=180, legs='sit', seat=46, arm_l='hold', arm_r='hold', rate=0.3, bob=-0.8, phase=r.random(), detail=1, **crew(r))
        P.figure(sc, 230, -120, yaw=-14, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=7, rate=1.4, react=True, phase=r.random(),
                 detail=1, **crew(r))
    return build


def drone_team_b(seed=104):
    """Two drones circling a mast, 6 and 6.8 m up; two pilots, the aerial technician at the dish. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.tape(g, 640, -230, posts=4)
        g.box(DARK, (0, 0, 6), (70, 70, 12))                               # the mast: pink and white bands, a ball on top
        for k in range(5):
            g.box((PINK, WHITE)[k % 2], (0, 0, 12 + 50 + k * 100), (12, 12, 100))
        g.sphere(YEL, (0, 0, 530), 16, steps=(4, 6))
        for a in (30, 150, 270):
            tilt_box(g, STEEL, (0, 0, 300), (math.cos(math.radians(a)) * 120, math.sin(math.radians(a)) * 120, 0), 4, 4)
        g.box(GREY, (230, 120, 44), (150, 70, 88))                         # the work table
        g.box(YEL, (230, 84, 60), (150, 2, 14))
        C.case(g, 200, 120, z=88, w=60, d=40, h=26, col=YEL)
        g.box(STEEL, (-260, 130, 80), (7, 7, 160))                         # the dish on its post
        g.cyl(WHITE, (-260, 118, 170), 50, 8, rot=(0, 0, 90), steps=8, top_r=30)
        g.box(DARK, (-260, 130, 4), (50, 50, 8))
        for k, (dx, dz) in enumerate(((290.0, 600.0), (-290.0, 680.0))):
            fl = sc.part(P.motion(bob=-22.0, rate=0.35, phase=0.5 * k, pivot=(dx, 0.0, dz), base=(0.0, 0.0), travel=(-1.0, 0.11)))
            drone(fl, dx, 0, dz, s=0.9)
        P.figure(sc, -90, -110, yaw=-12, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.0, phase=r.random(),
                 **crew(r, hat='headset', hat_col=DARK))
        P.figure(sc, 80, -120, yaw=14, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rock=4, rate=0.35, phase=r.random(),
                 **crew(r, hat='headset', hat_col=DARK))
        P.figure(sc, 230, 30, yaw=180, arm_l='fwd', arm_r='carry', swing_l=8, axis_l='x', rate=0.6, phase=r.random(), detail=1, **crew(r))
        P.figure(sc, -200, 40, yaw=-20, arm_r='up', swing_r=10, arm_l='hip', rate=0.5, phase=r.random(), detail=1, **crew(r))
    return build


def stunt_rig(seed=105):
    """A truss goalpost 6 m high: a performer swinging on wires over the crash mats. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 580.0
        C.tape(g, 820, -200, posts=5)
        for sx in (-1, 1):
            x = sx * 380
            g.box(YEL, (x, 0, H / 2.0), (22, 22, H))
            g.box(DARK, (x, 0, 8), (110, 150, 16))
            for sy in (-1, 1):
                tilt_box(g, STEEL, (x, sy * 70, 14), (x, 0, 260), 6, 6)
            for k in range(4):
                g.box(DARK, (x, 0, 100 + k * 130), (28, 28, 10))
        g.box(YEL, (0, 0, H + 14), (800, 26, 28))
        for k in range(5):
            g.box(DARK, (-320 + k * 160, 0, H + 14), (10, 30, 32))
        for k, col in enumerate(('#3a5fa8', RED, '#3a5fa8')):                  # the crash mats
            g.box(col, (k * 10 - 10, 20, 15 + k * 30), (330, 220, 28))
        g.cyl(DARK, (-380, 80, 16), 30, 40, rot=(0, 0, 90), steps=8)       # the winch at the left tower
        tilt_box(g, STEEL, (-380, 0, H), (-300, -90, 110), 4, 4)           # the rope down to the riggers
        swing = dict(swing=27.0, pivot=(0.0, 0.0, H), axis=90.0)
        wires = sc.part(P.motion(rate=0.28, phase=0.0, **swing))
        for sx in (-1, 1):
            tilt_box(wires, DARK, (sx * 6, 0, H), (sx * 22, 0, 362), 4, 4)
        P.figure(sc, 0, 0, z=235, arm_l='out', arm_r='out', legs='stride', rate=0.28, phase=0.0, sway=swing,
                 **P.look(r, sleeves='long', wear='trousers', top=ORANGE, bottom=ORANGE, hat='helmet', hat_col=WHITE, shoes=DARK, band=DARK, s=1.0))
        for k, x in enumerate((-300, -230)):                                 # two riggers hauling
            P.figure(sc, x, -100 + k * 30, yaw=-70, arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='x', axis_r='x', rock=0,
                     rate=0.56, phase=0.1 * k, detail=1, **crew(r))
        P.figure(sc, 250, -120, yaw=-20, arm_r='face', hold_r=C.hold_megaphone(), arm_l='up', swing_l=10, rate=0.5, phase=r.random(),
                 **crew(r, hat='cap', hat_col=DARK))
        P.figure(sc, 120, -150, yaw=10, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.2, phase=r.random(), detail=1, **crew(r))
    return build


def extras_a(seed=106):
    """The extras' holding area: nine skiers in bibs on a two-step riser, keeping warm; the wrangler. LOW or just over."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.tape(g, 820, -200, posts=5)
        deck(g, 0, -20, 760, 110, 36, skirt=(TEAL, WHITE), legs=False)
        deck(g, 0, 90, 760, 110, 92, skirt=(WHITE, TEAL), legs=False)
        g.box(GREY, (0, 148, 46), (760, 6, 92))
        warm = (
            dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=10, rate=1.5),              # star jumps
            dict(arm_l='hold', arm_r='hold', swing_l=14, swing_r=-14, axis_l='z', axis_r='z', rate=2.2, bob=2),   # beating the arms
            dict(arm_l='hip', arm_r='hip', rock=9, rate=0.6),
            dict(arm_r='wave', swing_r=24, bob=4, rate=1.1),
            dict(arm_l='out', arm_r='out', turn=26, rate=0.7),
            dict(arm_r='down', arm_l='down', hold_r=P.hold_board(col='#ffd23f', stripe=RED, length=170.0), bob=-1.5, rate=0.4),
        )
        k = 0
        for t, (y, z, n) in enumerate(((-20, 36, 5), (90, 92, 4))):
            for i in range(n):
                x = -380 + (i + 0.5) * 760.0 / n + r.uniform(-16, 16)
                d = extra(r)
                d.update(warm[(k * 5 + seed) % len(warm)])
                P.figure(sc, x, y + r.uniform(-8, 8), z, yaw=r.uniform(-14, 14), detail=0, phase=r.random(), react=(k % 2 == 0), **d)
                k += 1
        P.figure(sc, -300, -140, yaw=160, arm_r='face', hold_r=C.hold_megaphone(), arm_l='hold', hold_l=C.hold_clipboard(), rate=0.4, bob=-1.2,
                 phase=r.random(), detail=1, **crew(r, hat='headset', hat_col=DARK))
        P.figure(sc, 320, -140, yaw=-10, arm_r='up', hold_r=P.hold_sign(col=WHITE, stripe=PINK), arm_l='hip', rock=4, rate=0.4, phase=r.random(),
                 detail=1, **crew(r))
    return build


def extras_b(seed=107):
    """Wardrobe: a rail of coats, a rack of skis, five extras filing past it and round again; the costumer, the
    assistant waving them on. LOW or just over."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.tape(g, 880, -200, posts=5)
        for x in (-250, 150):                                               # the rail
            g.box(STEEL, (x, 130, 95), (7, 7, 190))
            g.box(DARK, (x, 130, 4), (10, 70, 8))
        g.box(STEEL, (-50, 130, 188), (410, 6, 6))
        for k in range(8):
            g.box(SUITS[(k * 4 + 1) % len(SUITS)], (-225 + k * 50, 130, 132), (38, 14, 96))
        g.box(WOOD, (330, 140, 60), (150, 10, 120))                         # the ski rack
        for k in range(5):
            tilt_box(g, SUITS[(k * 2) % len(SUITS)], (275 + k * 28, 128, 0), (275 + k * 28, 138, 172), 9, 4)
        g.box(GREY, (-380, 130, 30), (90, 60, 60))
        g.box(YEL, (-380, 130, 62), (94, 64, 5))
        for i in range(5):
            x = -400 + i * 160 + r.uniform(-12, 12)
            d = extra(r)
            if i % 2:
                d.update(hold_r=P.hold_board(col=SUITS[(i * 3) % len(SUITS)], stripe=WHITE, length=165.0))
            P.figure(sc, x, -60 + r.uniform(-14, 14), yaw=90, stride=24, swing_l=18, swing_r=(0 if i % 2 else -18), axis_l='x', axis_r='x',
                     rate=0.9, bob=3, phase=r.random(), travel=(880, 55), detail=1, **d)
        P.figure(sc, -110, 60, yaw=0, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=RED, w=40.0), rock=4, rate=0.4, phase=r.random(),
                 detail=1, **crew(r))
        P.figure(sc, 390, -130, yaw=-60, arm_r='wave', swing_r=26, arm_l='hip', rate=1.0, bob=3, phase=r.random(), detail=1,
                 **crew(r, hat='headset', hat_col=DARK))
    return build


def make_up(seed=108):
    """The make-up tent: a striped canopy, the mirror table, an actor in the chair. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.tape(g, 640, -210, posts=4)
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(STEEL, (sx * 230, 20 + sy * 150, 124), (7, 7, 248))
        for k in range(6):
            g.box((PINK, WHITE)[k % 2], (-192 + k * 77, 20, 254), (77, 330, 10))
        for k in range(6):
            g.box((WHITE, PINK)[k % 2], (-192 + k * 77, -147, 236), (77, 4, 28))
        g.box(C.CANVAS, (0, 172, 130), (466, 5, 236))                       # the back wall
        g.box(WHITE, (40, 140, 40), (300, 50, 80))                          # the mirror table
        g.box(YEL, (40, 150, 150), (240, 6, 130))
        g.box('#bfe3ff', (40, 146, 150), (216, 3, 106))
        for k in range(4):
            g.box((PINK, TEAL, YEL, RED)[k], (-60 + k * 60, 132, 86), (16, 16, 12))
        C.chair(g, -70, 20)
        C.chair(g, 130, 40, yaw=-12)
        g.box(STEEL, (-215, 60, 80), (6, 6, 160))                           # a costume on a stand
        g.box(RED, (-215, 60, 128), (48, 22, 72))
        g.box(WHITE, (-215, 48, 128), (48, 2, 10))
        hero = dict(sleeves='long', wear='trousers', hat=None, shoes=DARK, band=None)
        P.figure(sc, -70, 20, z=6, legs='sit', seat=72, arm_l='down', arm_r='down', rate=0.25, bob=-0.8, phase=0.2,
                 **P.look(r, top=RED, bottom=RED, hair='long', s=0.98, **hero))
        P.figure(sc, -150, -30, yaw=-70, arm_r='face', hold_r=hold_brush(), swing_r=9, axis_r='x', arm_l='hold', rate=1.1, phase=r.random(), **crew(r))
        P.figure(sc, -40, 90, yaw=-150, arm_l='fwd', arm_r='fwd', swing_l=8, swing_r=-8, axis_l='x', axis_r='x', rate=0.9, phase=r.random(),
                 detail=1, **crew(r))
        P.figure(sc, 130, 40, z=6, yaw=-12, legs='sit', seat=72, arm_r='face', hold_r=P.hold_cup(), arm_l='down', rate=0.3, bob=-0.8, phase=0.6,
                 detail=1, **P.look(r, top=BLUE, bottom=BLUE, hair='short', s=0.98, **hero))
        P.figure(sc, 250, -110, yaw=-25, arm_l='hold', arm_r='point', hold_l=C.hold_clipboard(), swing_r=10, axis_r='x', rate=0.5, phase=r.random(),
                 detail=1, **crew(r, hat='headset', hat_col=DARK))
    return build


def press_pen(seed=109):
    """The press pen: photographers on a riser, a video camera, a reporter talking to her own cameraman. LOW or just over."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.tape(g, 820, -200, posts=5)
        for sx in (-1, 1):                                                   # the pen's sides: the same tape, end-on
            for k in range(3):
                g.box((PINK, WHITE)[k % 2], (sx * 410, -200 + (k + 0.5) * 110, 92), (3, 110, 11))
            g.box(WHITE, (sx * 410, 130, 52), (9, 9, 104))
        deck(g, -60, 80, 560, 130, 56, skirt=(TEAL, WHITE), legs=False)
        gd = sc.still(at=(-230, 60, 56))
        C.tripod(gd, 0, 0, top=120.0)
        g = sc.still()
        C.case(g, 330, 100, col=YEL)
        pan = dict(swing=24.0, pivot=(-230.0, 60.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.13, phase=0.3, **pan))
        cam.box(DARK, (-230, 50, 56 + 138), (26, 60, 30))
        rod(cam, GREY, (-230, 20, 56 + 138), (-230, -20, 56 + 138), 9, steps=6)
        cam.box(TEAL, (-216, 56, 56 + 142), (3, 30, 10))

        def press(**fixed):
            d = P.look(r, sleeves='long', wear='coat', tops=('#3a5fa8', '#5a4a3c', DARK, '#7a8fb8', VIOLET), bottoms=(DARK, '#2b2350'),
                       hat=r.choice(('beanie', 'cap', None)), hat_col=r.choice((TEAL, DARK, RED)), band='#2ee6ff', s=r.uniform(0.94, 1.04))
            d.update(fixed)
            return d

        P.figure(sc, -230, 130, z=56, arm_l='push', arm_r='push', rate=0.13, phase=0.3, sway=pan, detail=1, **press())
        snaps = (dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=5, rate=0.5), dict(arm_l='face', arm_r='face', hold_r=P.hold_camera(), bob=-1.5, rate=0.4),
                 dict(arm_r='up', hold_r=P.hold_camera(), arm_l='hip', swing_r=7, bob=5, rate=1.2), dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=16, rate=0.35))
        for i, x in enumerate((-110, 0, 110, 200)):
            d = press()
            d.update(snaps[i])
            P.figure(sc, x + r.uniform(-10, 10), 80, z=56, yaw=r.uniform(-12, 12), phase=r.random(), react=(i % 2 == 0), detail=1, **d)
        for i, x in enumerate((-330, -160, 40)):
            d = press()
            d.update(snaps[(i + 1) % 4])
            P.figure(sc, x, -70 + r.uniform(-10, 10), yaw=r.uniform(-12, 12), phase=r.random(), react=(i % 2 == 1), detail=1, **d)
        # the reporter, her back to the road, and her cameraman
        P.figure(sc, 250, -130, yaw=180, arm_r='face', hold_r=hold_mic(), arm_l='point', swing_l=10, axis_l='x', rate=0.6, phase=r.random(),
                 detail=1, **press(top=RED, hat=None, hair='long'))
        P.figure(sc, 260, 10, yaw=0, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(col=GREY), rate=0.3, bob=-1.0, phase=r.random(), detail=1, **press())
    return build


def cable_cam(seed=110):
    """A cable camera: its trolley slides 4.5 m each way between two masts 6.4 m high. TALL. 12 m long."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 600.0
        C.tape(g, 1160, -200, posts=6)
        for sx in (-1, 1):
            x = sx * 560
            g.box(STEEL, (x, 0, (H + 40) / 2.0), (14, 14, H + 40))
            g.box(YEL, (x, 0, 30), (60, 60, 60))
            for k in range(3):
                g.box((PINK, WHITE)[k % 2], (x, 0, 200 + k * 130), (18, 18, 60))
            tilt_box(g, STEEL, (x, 0, H - 40), (x + sx * 20, 170, 0), 4, 4)
            tilt_box(g, STEEL, (x, 0, H - 40), (x - sx * 150, 60, 0), 4, 4)
            g.cyl(DARK, (x, -8, H - 14), 14, 16, rot=(0, 0, 90), steps=7)
        g.box(DARK, (0, 0, H), (1120, 5, 5))                                # the cable
        g.cyl(YEL, (-470, 110, 0), 34, 46, steps=8)                         # the winch drum
        g.cyl(DARK, (-470, 110, 46), 40, 6, steps=8)
        monitor(g, 200, 110)
        g.box(GREY, (200, 60, 44), (130, 60, 88))
        C.apple_box(g, 200, 150)
        C.case(g, 420, 110)
        slide = dict(swing=3.7, pivot=(0.0, 0.0, H + 7000.0), axis=90.0)
        tr = sc.part(P.motion(rate=0.08, phase=0.1, **slide))
        tr.box(DARK, (0, 0, H - 4), (70, 22, 20))
        for sx in (-1, 1):
            tr.cyl(YEL, (sx * 24, -6, H + 2), 9, 12, rot=(0, 0, 90), steps=6)
        tr.box(STEEL, (0, 0, H - 40), (7, 7, 56))
        tr.box(PINK, (0, 0, H - 70), (56, 30, 8))
        tr.box(DARK, (0, -4, H - 94), (36, 54, 34))
        rod(tr, GREY, (0, -31, H - 96), (0, -58, H - 104), 11, steps=6)
        P.figure(sc, 200, 150, z=30, yaw=180, legs='sit', seat=46, arm_l='hold', arm_r='hold', rate=0.3, bob=-0.8, phase=r.random(),
                 **crew(r, hat='headset', hat_col=DARK))
        P.figure(sc, -400, 60, yaw=-60, arm_l='push', arm_r='push', swing_l=8, swing_r=8, axis_l='x', axis_r='x', rate=0.16, phase=0.1, detail=1, **crew(r))
        P.figure(sc, 20, -110, yaw=-10, arm_r='up', swing_r=10, arm_l='hold', hold_l=C.hold_clipboard(), rate=0.5, phase=r.random(), detail=1, **crew(r))
        P.figure(sc, 430, -120, yaw=16, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.5, react=True, phase=r.random(), detail=1, **crew(r))
    return build


def wind_machine(seed=111):
    """The snow unit: a wind machine on its stand blowing snow along the piece past an actor; two grips shovelling
    snow into the wind, the generator. TALL (the fan's cage tops 3.2 m)."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.tape(g, 820, -200, posts=5)
        FX, FZ = -300.0, 200.0
        g.box(DARK, (FX - 20, 20, 8), (150, 170, 16))                       # the stand
        for sy in (-1, 1):
            tilt_box(g, YEL, (FX - 20, 20 + sy * 70, 14), (FX - 10, 20 + sy * 118, FZ), 10, 10)
        g.box(YEL, (FX - 10, 20, FZ), (12, 244, 10))
        rod(g, DARK, (FX - 40, 20, FZ), (FX + 30, 20, FZ), 116, steps=10)   # the cage
        rod(g, GREY, (FX + 30, 20, FZ), (FX + 34, 20, FZ), 104, steps=10)
        rod(g, YEL, (FX - 70, 20, FZ), (FX - 40, 20, FZ), 40, steps=7)      # the motor
        blades = sc.part(P.motion(swing=170.0, pivot=(FX + 40.0, 20.0, FZ), axis=0.0, rate=0.9))
        for k in range(3):
            a = math.radians(k * 60.0)
            tilt_box(blades, YEL, (FX + 40, 20 - math.cos(a) * 96, FZ - math.sin(a) * 96), (FX + 40, 20 + math.cos(a) * 96, FZ + math.sin(a) * 96), 6, 22)
        rod(blades, DARK, (FX + 34, 20, FZ), (FX + 48, 20, FZ), 18, steps=6)
        g = sc.still()
        g.box(YEL, (-330, 190, 40), (130, 70, 80))                          # the generator
        g.box(DARK, (-330, 190, 84), (110, 54, 8))
        g.sphere(SNOW, (-130, 110, 10), 70, scale=(1.3, 1.0, 0.6), steps=(4, 7))     # the heap they shovel from
        for k in range(7):                                                  # snow streaming down the wind
            bx, by, bz = -260.0 + k * 100.0, 20.0 + ((k * 37) % 50) - 25.0, FZ - 40.0 + ((k * 53) % 90)
            puff = sc.part(P.motion(bob=-14.0, rate=0.5, phase=0.14 * k, pivot=(bx, by, bz), base=(bx, by), travel=(700.0, 0.3)))
            puff.sphere((SNOW, '#bfe3ff')[k % 2], (bx, by, bz), 17.0 + (k % 3) * 7.0, steps=(3, 5))
        for k, (x, y) in enumerate(((-170, 60), (-80, 150))):
            P.figure(sc, x, y, yaw=120 + k * 40, arm_l='carry', arm_r='carry', hold_r=hold_shovel(), swing_l=16, swing_r=16, axis_l='x', axis_r='x',
                     rate=0.5, phase=0.5 * k, detail=1, **crew(r))
        P.figure(sc, 230, 20, yaw=90, legs='stride', arm_l='face', arm_r='push', rock=7, rate=0.5, phase=r.random(),
                 **P.look(r, sleeves='long', wear='trousers', top=RED, bottom=RED, hat='helmet', hat_col=WHITE, shoes=DARK, s=1.0))
        P.figure(sc, 330, -130, yaw=-30, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=C.hold_megaphone(), rate=0.5, phase=r.random(),
                 detail=1, **crew(r, hat='cap', hat_col=DARK))
    return build


JOBS = [
    ('SM_P3F_SecondUnit_A', second_unit_a()),
    ('SM_P3F_SecondUnit_B', second_unit_b()),
    ('SM_P3F_DroneTeam_A', drone_team_a(), dict(grow=(60.0, 60.0, 100.0))),
    ('SM_P3F_DroneTeam_B', drone_team_b(), dict(grow=(120.0, 420.0, 100.0), grow_down=(120.0, 420.0, 0.0))),
    ('SM_P3F_StuntRig', stunt_rig()),
    ('SM_P3F_Extras_A', extras_a()),
    ('SM_P3F_Extras_B', extras_b()),
    ('SM_P3F_MakeUp', make_up()),
    ('SM_P3F_PressPen', press_pen()),
    ('SM_P3F_CableCam', cable_cam()),
    ('SM_P3F_WindMachine', wind_machine()),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
