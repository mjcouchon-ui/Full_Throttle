"""FULL THROTTLE (Unreal) the population of Nevada, pass 3, builder p3f: MORE OF THE SHOOT (/Game/People/Nevada/SM_P3F_*).

In the fiction the level is a film shoot. The first round drew the main unit up at the start, the loops and the jumps
(ft_people_crew.py, wardrobe Desert). This kit adds the rest of a shoot's circus, strung along Highway 93:

    SM_P3F_SecondUnit_A/_B   a second-unit camera position on a scaffold riser: the camera panning with its operator, a
                             long-lens camera beside it, the clapper loader, the assistant director with a megaphone
                             (_B: mirrored, under a pink-and-white parasol)
    SM_P3F_CamTruck          the tracking truck parked up: a jib arm with the camera rising and falling over the cab
                             side, the crew on the scaffold platform over its bed
    SM_P3F_DroneTeam_A       the drone team: a heavy camera drone standing on its landing pad (fix round: it hovered;
                             no drone flies beside the road), the pilot, the spotter pointing up, a technician, a runner filming
    SM_P3F_DroneTeam_B       a drone docked on its mast, the pilot in a chair,
                             an antenna hand turning with it
    SM_P3F_StuntRig_A        stunt rigging: a truss gantry, a performer swinging on the wire, two riggers hauling, the
                             coordinator, a crash mat
    SM_P3F_StuntRig_B        the high fall: a seven-metre scaffold tower with two performers on top, the air bag
                             breathing below, the coordinator and the safety hand on a riser
    SM_P3F_Extras_A/_B       the extras' holding area: western costume under a shade sail, a bench, two rehearsing a
                             dance, the wrangler with a clipboard, the costume rail swinging in the wind
    SM_P3F_MakeUp            make-up and wardrobe under a canopy: an actor in the chair, the artist and the hairdresser
                             at work, a fitting, a runner with cups
    SM_P3F_PressPen_A/_B     the press pen behind crew tape: photographers panning with the chase, a television camera,
                             a reporter with a microphone, one on a step ladder
    SM_P3F_CamTower          a camera tower: the camera and its operator panning eight metres up under a parasol

This road is level, the land lies a metre under it behind a metre of barrier, so everybody stands RAISED (a riser 2.2 m
up, a truck's platform, a tower): every piece here is TALL and belongs 24 m or more from the centre line. No light on
any piece, no words, no marks. The crew wears the Desert wardrobe with the pink chest band; every riser carries the
pink-and-white tape and a pink pennant. Everything moves through M_People (ft_people.py).

The placing rules are the block C:/ftwork/p3/L2f.inl (pasted into Source/FullThrottle/FTPeopleL2.inl by the integrator).
Run in the editor:  import ft_people_nevada_p3f; ft_people_nevada_p3f.run()
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/Nevada'
W = C.WARDROBES['Desert']
PINK, WHITE, DARK, GREY, YEL, STEEL, WOOD = C.PINK, C.WHITE, C.DARK, C.GREY, C.YEL, C.STEEL, C.WOOD
CREAM, BLUE, SKY, ORANGE, LIME, GLASS, SAND = '#f4efe6', '#2e6bff', '#57c7ff', '#ff7a3c', '#b8ff5a', '#d7e3f0', '#f1e4c8'
HOT = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a', '#ff2d3c')
H = 220.0            # a riser's floor


def crew(r, **fixed):
    return C.member(r, W, **fixed)


def extra(r, **fixed):
    """An extra in western costume."""
    hat, hat_col = r.choice((('cowboy', '#8e5a3a'), ('cowboy', '#fff6ea'), ('cowboy', '#22202a'), ('sun', '#ffc93c'), (None, DARK)))
    d = P.look(r, tops=HOT, bottoms=('#5a4a3c', '#3a5fa8', '#f1e4c8', '#c94f6a'), sleeves='long',
               wear=r.choice(('trousers', 'dress', 'skirt', 'trousers')), hat=hat, hat_col=hat_col)
    d.update(fixed)
    return d


def visitor(r, **fixed):
    """Press: ordinary clothes, a cap or a sun hat, a white band (their pass)."""
    hat, hat_col = r.choice((('cap', '#3a5fa8'), ('cap', '#ff2d3c'), ('sun', SAND), (None, DARK), (None, DARK)))
    d = P.look(r, hat=hat, hat_col=hat_col, band=WHITE)
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ what the shoot stands on
def crew_tape(g, x0, length, y, z):
    """The crew's barrier on a riser's front edge: four posts and a run of pink-and-white tape."""
    for k in range(4):
        g.box(WHITE, (x0 - length / 2.0 + k * length / 3.0, y, z + 50), (8, 8, 100))
    n = max(2, int(round(length / 130.0)))
    for k in range(n):
        g.box(PINK if k % 2 else WHITE, (x0 - length / 2.0 + (k + 0.5) * length / n, y, z + 90), (length / n, 4, 12))


def scaf(sc, x0=0.0, L=1040.0, D=420.0, h=H, tape=True, rail=True):
    """A scaffold riser, its floor at h: posts, yellow braces, a pink-and-white skirt, a back rail, the crew's tape."""
    g = sc.still()
    g.box(DARK, (x0, 0, h - 7), (L, D, 14))
    g.box(YEL, (x0, -D / 2.0 - 2, h - 7), (L + 6, 5, 16))
    n = max(2, int(round(L / 340.0)) + 1)
    xs = [x0 - L / 2.0 + 10 + k * (L - 20) / (n - 1.0) for k in range(n)]
    for k, x in enumerate(xs):
        for sy in (-1, 1):
            g.box(STEEL, (x, sy * (D / 2.0 - 8), (h - 14) / 2.0), (8, 8, h - 14))
        if k < n - 1:
            tilt_box(g, YEL, (x, -D / 2.0 + 8, 16), (xs[k + 1], -D / 2.0 + 8, h - 30), 6, 6)
    m = max(2, int(round(L / 260.0)))
    for k in range(m):
        g.box(PINK if k % 2 else WHITE, (x0 - L / 2.0 + (k + 0.5) * L / m, -D / 2.0 - 5, h - 40), (L / m, 4, 26))
    if rail:
        for x in xs:
            g.box(STEEL, (x, D / 2.0 - 6, h + 52), (7, 7, 104))
        g.box(YEL, (x0, D / 2.0 - 6, h + 102), (L, 7, 7))
    if tape:
        crew_tape(g, x0, L - 40, -D / 2.0 + 16, h)


def pennant(sc, x, y, z0, height=330.0, phase=0.0, w=130.0, hh=76.0):
    """The unit's pink pennant on a pole, swinging in the wind."""
    g = sc.still()
    g.box(STEEL, (x, y, z0 + height / 2.0), (7, 7, height))
    f = sc.part(P.motion(swing=13.0, pivot=(x, y, z0), axis=P.UP, rate=0.7, phase=phase))
    f.box(PINK, (x + w / 2.0 + 4, y, z0 + height - hh / 2.0 - 4), (w, 5, hh))
    f.box(WHITE, (x + w / 2.0 + 4, y, z0 + height - hh / 2.0 - 4), (w * 0.3, 7, hh * 0.4))


def parasol(sc, x, y, z0, r=150.0, h=250.0):
    g = sc.still()
    g.box(STEEL, (x, y, z0 + h / 2.0), (6, 6, h))
    for k in range(8):
        a0, a1 = 2 * math.pi * k / 8.0, 2 * math.pi * (k + 1) / 8.0
        g.plan(PINK if k % 2 else WHITE, [(x, y), (x + math.cos(a0) * r, y + math.sin(a0) * r), (x + math.cos(a1) * r, y + math.sin(a1) * r)], z0 + h - 12, z0 + h)


def canopy(sc, x0, x1, z0, h=250.0, y0=-150.0, y1=150.0, cols=(PINK, WHITE), n=4):
    """A shade on four poles, striped."""
    g = sc.still()
    for x in (x0, x1):
        for y in (y0, y1):
            g.box(STEEL, (x, y, z0 + h / 2.0), (6, 6, h))
    for k in range(n):
        g.box(cols[k % 2], (x0 + (k + 0.5) * (x1 - x0) / n, (y0 + y1) / 2.0, z0 + h + 4), ((x1 - x0) / n + 0.5, (y1 - y0) + 30, 8))


def tower(sc, x, y, w=220.0, top=600.0):
    """A scaffold tower: four posts, three rings, zig-zag braces on its face, a railed platform with the crew's
    pink-and-white band, a ladder up its side. Returns the platform's floor."""
    g = sc.still()
    hw = w / 2.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL, (x + sx * hw, y + sy * hw, top / 2.0), (9, 9, top))
    levels = 3
    for k in range(1, levels + 1):
        z = top * k / (levels + 1.0)
        g.box(STEEL, (x, y - hw, z), (w, 6, 6))
        g.box(STEEL, (x, y + hw, z), (w, 6, 6))
        g.box(STEEL, (x - hw, y, z), (6, w, 6))
        g.box(STEEL, (x + hw, y, z), (6, w, 6))
    for k in range(levels + 1):
        z0, z1 = top * k / (levels + 1.0), top * (k + 1) / (levels + 1.0)
        a, b = (-hw, hw) if k % 2 else (hw, -hw)
        tilt_box(g, YEL, (x + a, y - hw, z0 + 6), (x + b, y - hw, z1 - 6), 6, 6)
    pw = w + 40.0
    g.box(DARK, (x, y, top + 5), (pw, pw, 10))
    for k in range(4):
        g.box(PINK if k % 2 else WHITE, (x - pw / 2.0 + (k + 0.5) * pw / 4.0, y - pw / 2.0 - 2, top - 16), (pw / 4.0, 4, 28))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL, (x + sx * (pw / 2.0 - 5), y + sy * (pw / 2.0 - 5), top + 60), (6, 6, 100))
        g.box(YEL, (x + sx * (pw / 2.0 - 5), y, top + 108), (6, pw, 6))
    g.box(YEL, (x, y + pw / 2.0 - 5, top + 108), (pw, 6, 6))
    g.box(YEL, (x, y - pw / 2.0 + 5, top + 108), (pw, 6, 6))
    for sy in (-1, 1):
        g.box(STEEL, (x + hw + 16, y + sy * 20, top / 2.0), (5, 5, top))
    for k in range(7):
        g.box(STEEL, (x + hw + 16, y, (k + 0.5) * top / 7.0), (5, 44, 5))
    return top + 10.0


def seat(sc, x, y, z, h=72.0, col=C.CANVAS):
    """A tall chair, its sitter facing -Y: sit a figure on it with z = floor + 6, legs='sit', seat=h."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WOOD, (x + sx * 24, y + sy * 20, z + h / 2.0), (5, 5, h))
    g.box(col, (x, y, z + h), (54, 46, 6))
    g.box(col, (x, y + 25, z + h + 44), (54, 5, 26))
    for sx in (-1, 1):
        g.box(WOOD, (x + sx * 24, y + 25, z + h + 30), (5, 5, 60))


def cam_position(sc, x, y, z, r, rate=0.12, phase=0.3, swing=22.0, long_lens=False, det=2):
    """A film camera on its tripod at (x, y) on a floor at z, panning, its operator panning with it."""
    gh = sc.still(at=(0, 0, z))
    C.tripod(gh, x, y)
    pan = dict(swing=swing, pivot=(x, y, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=rate, phase=phase, **pan))
    C.film_camera(cam, x, y, z + 128)
    if long_lens:
        cam.cyl(GREY, (x, y - 63, z + 145), 12, 64, rot=(0, 0, 90), steps=8)
    P.figure(sc, x, y + 82, z=z, arm_l='push', arm_r='push', rate=rate, phase=phase, sway=pan, detail=det, **crew(r, hat='headset', hat_col=DARK))


def drone(sc, x, y, z, bob=-30.0, rate=0.25, phase=0.0, spin=1.3, s=1.0, orbit=None):
    """A heavy camera drone, 2.5 m across at s = 1: it floats by its bob; its blades turn (spin: rounds a second) when
    it holds its place, or the whole drone circles orbit = (centre x, centre y, rounds a second). No light on it."""
    base, tv = (x, y), (0.0, 0.0)
    if orbit:
        base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
    b = sc.part(P.motion(bob=bob, rate=rate, phase=phase, base=base, travel=tv))
    b.box(DARK, (x, y, z), (64 * s, 64 * s, 24 * s))
    b.box(PINK, (x, y, z + 14 * s), (42 * s, 42 * s, 6 * s))
    b.sphere(GREY, (x, y - 8 * s, z - 24 * s), 16 * s, steps=(4, 6))
    for sx in (-1, 1):
        b.box(GREY, (x + sx * 28 * s, y, z - 34 * s), (5 * s, 70 * s, 5 * s))
        b.box(GREY, (x + sx * 28 * s, y, z - 22 * s), (5 * s, 5 * s, 22 * s))
    for sx, sy in ((-1, -1), (1, -1), (1, 1), (-1, 1)):
        rx, ry = x + sx * 82 * s, y + sy * 82 * s
        tilt_box(b, GREY, (x + sx * 24 * s, y + sy * 24 * s, z), (rx, ry, z + 6 * s), 9 * s, 9 * s)
        b.cyl(DARK, (rx, ry, z + 2 * s), 9 * s, 16 * s, steps=6)
        b.cyl('#c9c4d6', (rx, ry, z + 19 * s), 44 * s, 3 * s, steps=8)
        if spin and not orbit:
            bl = sc.part(P.motion(bob=bob, rate=rate, phase=phase, base=(rx, ry), travel=(-1.0, spin)))
            bl.box(WHITE, (rx, ry, z + 24 * s), (84 * s, 9 * s, 4 * s))


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_controller():
    def f(m, h, side):
        if side < 0:
            return
        cx = h[0] - 22
        m.box(DARK, (cx, h[1] - 6, h[2] + 6), (46, 16, 10))
        m.box(YEL, (cx - 14, h[1] - 8, h[2] + 15), (4, 4, 10))
        m.box(YEL, (cx + 14, h[1] - 8, h[2] + 15), (4, 4, 10))
        m.box(STEEL, (cx, h[1] - 1, h[2] + 26), (4, 4, 34))
    return f


def hold_mic():
    def f(m, h, side):
        m.box(DARK, (h[0], h[1] - 4, h[2] + 12), (5, 5, 24))
        m.sphere(YEL, (h[0], h[1] - 4, h[2] + 29), 8, steps=(3, 5))
    return f


def hold_fan(col=CREAM):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 3, h[2] + 18), (28, 4, 28))
    return f


def hold_antenna():
    def f(m, h, side):
        m.box(STEEL, (h[0], h[1], h[2] + 60), (5, 5, 150))
        m.box(WHITE, (h[0], h[1] - 4, h[2] + 150), (46, 6, 46))
        m.box(PINK, (h[0], h[1] - 7.5, h[2] + 150), (30, 2, 8))
    return f


def hold_dryer():
    def f(m, h, side):
        m.box(PINK, (h[0], h[1] - 10, h[2] + 10), (10, 30, 12))
        m.box(DARK, (h[0], h[1], h[2] + 2), (7, 7, 16))
    return f


def hold_tray():
    def f(m, h, side):
        if side < 0:
            return
        cx = h[0] - 22
        m.box(WOOD, (cx, h[1] - 8, h[2] + 8), (50, 30, 4))
        for k in range(3):
            m.cyl(WHITE, (cx - 15 + k * 15, h[1] - 8, h[2] + 10), 5.5, 14, steps=6)
    return f


# ------------------------------------------------------------------------------------------ the pieces
def second_unit(seed, flip=1, shade=False):
    def build(sc):
        r = random.Random(seed)
        f = flip
        scaf(sc)
        cam_position(sc, -150 * f, -40, H, r, rate=0.12, phase=0.3)
        cam_position(sc, 150 * f, -60, H, r, rate=0.09, phase=0.7, swing=14.0, long_lens=True, det=1)
        if shade:
            parasol(sc, -150 * f, 110, H, r=170.0, h=262.0)
        # the clapper loader between the cameras; the assistant director with the megaphone
        P.figure(sc, 0, -120, z=H, yaw=10 * f, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.35, bob=-1.5, phase=r.random(), detail=1, **crew(r))
        P.figure(sc, -370 * f, -100, z=H, yaw=-18 * f, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12, axis_l='x', rate=0.6,
                 phase=r.random(), react=True, **crew(r, hat='cap', hat_col=DARK))
        gh = sc.still(at=(0, 0, H))
        C.case(gh, 400 * f, 90)
        C.case(gh, 410 * f, 86, z=38, w=52, h=30, col=YEL)
        C.apple_box(gh, 330 * f, -40)
        pennant(sc, -505 * f, 196, H, phase=0.2)
    return build


def cam_truck(sc):
    r = random.Random(61)
    g = sc.still()
    # the truck, its cab towards +X
    g.box(DARK, (0, 0, 62), (720, 190, 24))
    for x in (-270, -180, 245):
        g.cyl(DARK, (x, -78, 46), 46, 26, rot=(0, 0, 90), steps=10)
        g.cyl(DARK, (x, 104, 46), 46, 26, rot=(0, 0, 90), steps=10)
    g.box(CREAM, (275, 0, 150), (170, 210, 150))
    g.box('#3a5fa8', (361, 0, 182), (4, 180, 60))
    g.box('#3a5fa8', (275, -106, 182), (110, 4, 56))
    g.box(PINK, (275, -106, 122), (170, 4, 14))
    g.box(CREAM, (385, 0, 105), (60, 200, 60))
    g.box(GREY, (-90, 0, 84), (540, 210, 20))
    g.box(PINK, (-90, -106, 84), (540, 4, 10))
    # the scaffold platform over the bed, its floor at 250
    PZ = 250.0
    for x in (-340, 160):
        for sy in (-1, 1):
            g.box(STEEL, (x, sy * 96, (94 + PZ) / 2.0), (8, 8, PZ - 94))
    tilt_box(g, YEL, (-340, -96, 100), (160, -96, PZ - 20), 6, 6)
    g.box(DARK, (-90, 0, PZ - 7), (540, 224, 14))
    g.box(YEL, (-90, -114, PZ - 7), (546, 5, 16))
    for k in range(4):
        g.box(PINK if k % 2 else WHITE, (-360 + (k + 0.5) * 135, -117, PZ - 36), (135, 4, 24))
    for x in (-355, -90, 175):
        g.box(STEEL, (x, 106, PZ + 52), (7, 7, 104))
    g.box(YEL, (-90, 106, PZ + 102), (540, 7, 7))
    # the jib: a post, and an arm that rises and falls with the camera slung under its tip
    g.box(YEL, (-210, 30, PZ + 75), (24, 24, 150))
    g.box(DARK, (-210, 30, PZ + 156), (36, 36, 14))
    arm = sc.part(P.motion(swing=8.0, pivot=(-210.0, 30.0, PZ + 160.0), axis=0.0, rate=0.1, phase=0.4))
    tilt_box(arm, YEL, (-210, 104, PZ + 142), (-210, -216, PZ + 216), 14, 18)
    arm.box(DARK, (-210, 100, PZ + 136), (50, 40, 46))
    arm.box(DARK, (-210, -212, PZ + 196), (26, 26, 34))
    C.film_camera(arm, -210, -216, PZ + 118)
    P.figure(sc, -120, 50, z=PZ, yaw=-25, arm_l='push', arm_r='push', rock=3, rate=0.1, phase=0.4, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, 20, -50, z=PZ, yaw=12, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=11, axis_l='x', rate=0.55, phase=r.random(),
             react=True, **crew(r, hat='cap', hat_col=DARK))
    P.figure(sc, 125, 30, z=PZ, yaw=-8, arm_r='wave', swing_r=22, arm_l='hip', rate=1.1, bob=3, phase=r.random(), react=True, detail=1, **crew(r))
    pennant(sc, 300, 70, 225, height=210, phase=0.6)


def drone_team_a(sc):
    r = random.Random(71)
    scaf(sc)
    gh = sc.still(at=(0, 0, H))
    gh.cyl(ORANGE, (300, 10, 0), 112, 6, steps=8)
    gh.box(WHITE, (300, 10, 8), (150, 24, 4))
    gh.box(WHITE, (300, 10, 8), (24, 150, 4))
    gh.box(GREY, (95, 84, 23), (70, 44, 46))
    C.case(gh, 440, 120, w=60, d=40)
    drone(sc, 300, 10, H + 47, bob=0.0, rate=0.22, phase=0.1, spin=0)     # fix round: it stands ON its pad, blades still (was hovering 4.3 m up, 27 m from the road)
    P.figure(sc, -60, -70, z=H, yaw=14, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.5, phase=r.random(), **crew(r, hat='cap', hat_col=DARK))
    P.figure(sc, -250, -90, z=H, yaw=20, arm_r='up', swing_r=10, arm_l='face', rate=0.5, phase=r.random(), react=True, detail=1, **crew(r))
    P.figure(sc, 95, 84, z=H, yaw=-10, legs='sit', seat=46, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=DARK, w=40.0), rate=0.3, bob=-0.8,
             phase=r.random(), detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, -410, -60, z=H, yaw=30, arm_r='face', hold_r=P.hold_phone(), arm_l='hip', rock=4, rate=0.35, phase=r.random(), detail=1, **crew(r))
    pennant(sc, -505, 196, H, phase=0.5)


def drone_team_b(sc):
    r = random.Random(73)
    scaf(sc)
    g = sc.still()
    g.box(STEEL, (-380, 70, H + 190), (10, 10, 380))
    g.box(DARK, (-380, 70, H + 384), (120, 120, 8))
    g.box(PINK, (-380, 8, H + 384), (120, 4, 8))
    drone(sc, -380, 70, H + 425, bob=-3.0, rate=0.5, phase=0.3, spin=1.1, s=0.8)
    # fix round: the second drone, which circled free over the riser (to 24.1 m from the road), is gone; the docked one on its mast stays
    seat(sc, 120, 50, H)
    P.figure(sc, 120, 50, z=H + 6, legs='sit', seat=72, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-0.8, phase=r.random(),
             **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, -110, -80, z=H, arm_r='hold', hold_r=hold_antenna(), arm_l='hip', turn=28, rate=0.09, phase=0.1, detail=1, **crew(r))
    P.figure(sc, 340, -80, z=H, yaw=-12, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, rate=0.8, bob=3, phase=r.random(), react=True, detail=1, **crew(r))
    P.figure(sc, -250, -100, z=H, yaw=25, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.6, phase=r.random(), detail=1, **crew(r))
    gh = sc.still(at=(0, 0, H))
    C.case(gh, 440, 110, w=60, d=40, col=YEL)
    pennant(sc, 505, 196, H, phase=0.8)


def stunt_rig_a(sc):
    r = random.Random(81)
    scaf(sc)
    g = sc.still()
    TOP = H + 520.0
    for x in (-430, 430):
        for y in (10, 90):
            g.box(YEL, (x, y, (H + TOP) / 2.0), (12, 12, TOP - H))
        for k in range(4):
            g.box(STEEL, (x, 50, H + 60 + k * 140), (8, 80, 8))
    g.box(YEL, (0, 50, TOP + 8), (890, 96, 16))
    g.box(DARK, (0, 50, TOP - 6), (40, 40, 14))
    # the performer on the wire, swinging along the gantry
    swing = dict(swing=15.0, pivot=(0.0, 50.0, TOP), axis=90.0)
    wire = sc.part(P.motion(rate=0.28, phase=0.1, **swing))
    wire.box(DARK, (0, 50, TOP - 90), (5, 5, 180))
    P.figure(sc, 0, 50, z=TOP - 350, arm_l='out', arm_r='out', legs='stride', rate=0.28, phase=0.1, sway=swing,
             **crew(r, top=ORANGE, bottom=ORANGE, sleeves='long', hat='helmet', hat_col=WHITE, band=None))
    g = sc.still()
    g.box(BLUE, (0, 50, H + 18), (320, 200, 36))
    g.box(SKY, (0, 50, H + 38), (260, 150, 5))
    tilt_box(g, DARK, (-300, -20, H + 110), (-430, 50, TOP), 5, 5)
    # two riggers hauling on the line, the coordinator, the safety hand
    P.figure(sc, -300, -30, z=H, yaw=-75, arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='x', axis_r='x', rate=0.56, phase=0.1, detail=1, **crew(r))
    P.figure(sc, -215, 30, z=H, yaw=-80, arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='x', axis_r='x', rate=0.56, phase=0.35, detail=1, **crew(r))
    P.figure(sc, 255, -90, z=H, yaw=-20, arm_r='face', hold_r=C.hold_megaphone(), arm_l='up', swing_l=9, rate=0.5, phase=r.random(), react=True,
             **crew(r, hat='cap', hat_col=DARK))
    P.figure(sc, 380, -30, z=H, yaw=-30, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.2, phase=r.random(), detail=1, **crew(r))
    pennant(sc, 505, 196, H, height=300, phase=0.3)


def stunt_rig_b(sc):
    r = random.Random(83)
    tz = tower(sc, -430, 0, w=220.0, top=690.0)
    suit = dict(sleeves='long', hat='helmet', band=None)
    P.figure(sc, -470, -40, z=tz, yaw=8, arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=5, rate=1.3, phase=0.2, react=True,
             **crew(r, top=ORANGE, bottom=ORANGE, hat_col=WHITE, **suit))
    P.figure(sc, -385, 40, z=tz, yaw=-20, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.6, phase=0.7, detail=1,
             **crew(r, top=LIME, bottom=DARK, hat_col='#ff2d3c', **suit))
    # the air bag breathing on its fan
    bag = sc.part(P.motion(bob=-6.0, rate=0.3))
    bag.box(BLUE, (0, 0, 126), (470, 372, 252))
    bag.box(SKY, (0, 0, 255), (400, 310, 8))
    bag.box(WHITE, (0, 0, 261), (130, 130, 4))
    g = sc.still()
    g.box(GREY, (-262, 150, 32), (56, 56, 64))
    scaf(sc, x0=420.0, L=300.0)
    P.figure(sc, 370, -80, z=H, yaw=22, arm_r='face', hold_r=C.hold_megaphone(), arm_l='up', swing_l=9, rate=0.5, phase=r.random(), react=True,
             **crew(r, hat='cap', hat_col=DARK))
    P.figure(sc, 480, -40, z=H, yaw=28, arm_r='up', hold_r=P.hold_flag(col=PINK, w=56.0), swing_r=16, arm_l='hip', rate=0.9, phase=r.random(), detail=1, **crew(r))
    g = sc.still()
    g.cyl('#ff2d3c', (535, 80, H), 11, 46, steps=8)
    g.cyl(DARK, (535, 80, H + 46), 5, 10, steps=6)


def extras(seed, flip=1):
    def build(sc):
        r = random.Random(seed)
        f = flip
        scaf(sc)
        xa, xb = sorted((-470 * f, 20 * f))
        canopy(sc, xa, xb, H, h=255.0, cols=(SAND, PINK), n=4)
        g = sc.still()
        g.box(WOOD, (-225 * f, 86, H + 42), (430, 44, 8))
        for x in (-420 * f, -30 * f):
            g.box(WOOD, (x, 86, H + 20), (8, 40, 40))
        sits = (dict(arm_r='face', hold_r=P.hold_cup(), arm_l='down', rate=0.3, bob=-0.8),
                dict(arm_r='face', hold_r=hold_fan(), swing_r=14, axis_r='x', arm_l='down', rate=1.5),
                dict(arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.25, bob=-0.6))
        for k, x in enumerate((-370, -225, -80)):
            P.figure(sc, x * f, 80, z=H, yaw=r.uniform(-10, 10), legs='sit', seat=46, phase=r.random(), detail=1, **dict(extra(r), **sits[(k + seed) % 3]))
        # two rehearsing a dance step, a third clapping time
        P.figure(sc, 120 * f, -70, z=H, yaw=60 * f, arm_l='hip', arm_r='out', turn=30, rate=0.5, phase=0.0, detail=1, **extra(r, wear='dress'))
        P.figure(sc, 215 * f, -50, z=H, yaw=-120 * f, arm_l='out', arm_r='hip', turn=30, rate=0.5, phase=0.5, detail=1, **extra(r, wear='trousers', hat='cowboy'))
        P.figure(sc, -110 * f, -110, z=H, yaw=20 * f, arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.0, bob=3,
                 phase=r.random(), react=True, detail=1, **extra(r))
        # the wrangler
        P.figure(sc, 400 * f, -100, z=H, yaw=-25 * f, arm_r='point', swing_r=11, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.6, phase=r.random(),
                 **crew(r, hat='headset', hat_col=DARK))
        # the costume rail, its clothes swinging in the wind
        g = sc.still()
        for x in (270 * f, 450 * f):
            g.box(STEEL, (x, 150, H + 88), (6, 6, 176))
        g.box(STEEL, (360 * f, 150, H + 176), (190, 6, 6))
        rail = sc.part(P.motion(swing=7.0, pivot=(360.0 * f, 150.0, H + 176.0), axis=0.0, rate=0.4, phase=0.2))
        for k in range(5):
            rail.box(HOT[(k * 3 + seed) % len(HOT)], (360 * f - 68 + k * 34, 150, H + 118), (28, 9, 100))
        pennant(sc, 505 * f, 196, H, phase=0.4)
    return build


def make_up(sc):
    r = random.Random(91)
    scaf(sc)
    canopy(sc, -400, 60, H, h=255.0, n=4)
    # the chair, the mirror with its (unlit) bulbs, the table of pots
    seat(sc, -200, 20, H)
    g = sc.still()
    g.box(DARK, (-200, 150, H + 150), (170, 8, 130))
    g.box(GLASS, (-200, 145, H + 150), (140, 3, 100))
    for k in range(5):
        g.box(CREAM, (-264 + k * 32, 144, H + 208), (12, 4, 12))
    g.box(WHITE, (-200, 120, H + 80), (190, 60, 8))
    for sx in (-1, 1):
        g.box(STEEL, (-200 + sx * 88, 120, H + 38), (6, 50, 76))
    for k, col in enumerate((PINK, '#2ee6ff', YEL, '#8f6bff')):
        g.cyl(col, (-260 + k * 40, 118, H + 84), 8, 16, steps=6)
    P.figure(sc, -200, 20, z=H + 6, legs='sit', seat=72, arm_l='down', arm_r='hold', rate=0.25, bob=-0.6, phase=r.random(), **extra(r, hat=None, band=WHITE))
    P.figure(sc, -115, 0, z=H, yaw=-90, arm_r='fwd', swing_r=10, axis_r='x', hold_r=P.hold_tool(col=WOOD, grip=PINK, length=18.0), arm_l='hip', rate=1.1,
             phase=r.random(), **crew(r, hat=None))
    P.figure(sc, -290, 30, z=H, yaw=90, arm_l='fwd', swing_l=8, axis_l='x', hold_l=hold_dryer(), arm_r='down', rate=0.8, phase=r.random(), detail=1, **crew(r, hat=None))
    # a fitting: arms out, the dresser at the sleeve; a runner with cups
    P.figure(sc, 200, -50, z=H, yaw=6, arm_l='out', arm_r='out', turn=7, rate=0.3, phase=r.random(), detail=1, **extra(r, hat='cowboy', hat_col='#fff6ea'))
    P.figure(sc, 290, -30, z=H, yaw=-90, arm_l='fwd', arm_r='fwd', swing_l=9, swing_r=-9, axis_l='x', axis_r='x', rate=0.9, phase=r.random(), detail=1, **crew(r, hat=None))
    P.figure(sc, 420, 40, z=H, yaw=-150, arm_l='carry', arm_r='carry', hold_r=hold_tray(), rate=0.4, bob=-1.5, phase=r.random(), detail=1, **crew(r))
    g = sc.still()
    for x in (130, 330):
        g.box(STEEL, (x, 150, H + 88), (6, 6, 176))
    g.box(STEEL, (230, 150, H + 176), (210, 6, 6))
    rail = sc.part(P.motion(swing=7.0, pivot=(230.0, 150.0, H + 176.0), axis=0.0, rate=0.4, phase=0.6))
    for k in range(5):
        rail.box(HOT[(k * 2 + 1) % len(HOT)], (230 - 76 + k * 38, 150, H + 118), (30, 9, 100))
    pennant(sc, 505, 196, H, phase=0.1)


def press_pen(seed, flip=1):
    def build(sc):
        r = random.Random(seed)
        f = flip
        scaf(sc)
        # the television camera and the reporter talking to it
        gh = sc.still(at=(0, 0, H))
        C.tripod(gh, -360 * f, 40)
        pan = dict(swing=12.0, pivot=(-360.0 * f, 40.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.15, phase=0.2, **pan))
        cam.box(DARK, (-360 * f, 30, H + 150), (30, 70, 34))
        cam.cyl(GREY, (-360 * f, -5, H + 150), 11, 30, rot=(0, 0, 90), steps=8)
        cam.box(PINK, (-360 * f, 40, H + 169), (32, 30, 5))
        P.figure(sc, -360 * f, 122, z=H, arm_l='push', arm_r='push', rate=0.15, phase=0.2, sway=pan, detail=1, **visitor(r, hat='headset', hat_col=DARK))
        P.figure(sc, -350 * f, -95, z=H, yaw=165, arm_r='face', hold_r=hold_mic(), arm_l='point', swing_l=12, axis_l='x', rock=3, rate=0.5, phase=r.random(),
                 **visitor(r, hat=None, top='#ff2d3c'))
        # photographers at the tape, panning with the chase
        for k, x in enumerate((-170, -40, 100)):
            P.figure(sc, x * f + r.uniform(-12, 12), -105 + (k % 2) * 50, z=H, yaw=r.uniform(-12, 12), arm_l='hold', arm_r='hold', hold_r=P.hold_camera(),
                     turn=22, rate=0.35, phase=0.15 * k, react=True, detail=(2 if k == 1 else 1), **visitor(r))
        P.figure(sc, 230 * f, -90, z=H, yaw=-10 * f, arm_r='wave', swing_r=24, arm_l='face', hold_l=P.hold_phone(), rate=1.2, bob=4, phase=r.random(), react=True,
                 detail=1, **visitor(r))
        # one on a step ladder
        g = sc.still()
        lx = 390 * f
        tilt_box(g, YEL, (lx, -10, H), (lx, 40, H + 110), 60, 6)
        tilt_box(g, YEL, (lx, 110, H), (lx, 50, H + 110), 60, 6)
        g.box(YEL, (lx, 45, H + 112), (64, 44, 8))
        P.figure(sc, lx, 45, z=H + 116, yaw=6 * f, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=20, rate=0.3, phase=0.5, react=True, detail=1, **visitor(r))
        # the unit's marshal keeps the pen
        P.figure(sc, 20 * f, 90, z=H, yaw=10 * f, arm_l='hip', arm_r='point', swing_r=9, axis_r='x', rate=0.5, phase=r.random(), detail=1, **crew(r, hat='headset', hat_col=DARK))
        pennant(sc, 505 * f, 196, H, phase=0.7)
    return build


def cam_tower(sc):
    r = random.Random(97)
    tz = tower(sc, 0, 0, w=240.0, top=600.0)
    cam_position(sc, -35, -50, tz, r, rate=0.1, phase=0.5, swing=26.0, long_lens=True)
    parasol(sc, -35, 100, tz, r=150.0, h=250.0)
    P.figure(sc, 85, -70, z=tz, yaw=-14, arm_l='face', arm_r='face', turn=22, rate=0.2, phase=r.random(), detail=1, **crew(r))
    pennant(sc, 132, 132, tz, height=330, phase=0.9)
    # the generator at the tower's foot
    g = sc.still()
    g.box(YEL, (-230, 60, 34), (110, 60, 68))
    g.box(DARK, (-230, 60, 72), (90, 44, 8))


JOBS = [
    ('SM_P3F_SecondUnit_A', second_unit(301, 1, False)),
    ('SM_P3F_SecondUnit_B', second_unit(302, -1, True)),
    ('SM_P3F_CamTruck', cam_truck),
    ('SM_P3F_DroneTeam_A', drone_team_a, dict(grow=(60.0, 60.0, 90.0))),
    ('SM_P3F_DroneTeam_B', drone_team_b, dict(grow=(60.0, 60.0, 90.0))),
    ('SM_P3F_StuntRig_A', stunt_rig_a, dict(grow=(120.0, 60.0, 60.0), grow_down=(120.0, 60.0, 0.0))),
    ('SM_P3F_StuntRig_B', stunt_rig_b),
    ('SM_P3F_Extras_A', extras(311, 1)),
    ('SM_P3F_Extras_B', extras(312, -1)),
    ('SM_P3F_MakeUp', make_up),
    ('SM_P3F_PressPen_A', press_pen(321, 1)),
    ('SM_P3F_PressPen_B', press_pen(322, -1)),
    ('SM_P3F_CamTower', cam_tower),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
