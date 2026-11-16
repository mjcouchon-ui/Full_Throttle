"""FULL THROTTLE (Unreal) the population, level 2: the Volta Chemical Works, MORE OF THE SHOOT (pass 3, builder p3f).

The level is a film shoot, and pass 3 gives it the units a big night shoot has beyond the first kit: a second unit on
its scaffold and on its camera truck, the drone team, the stunt riggers with a performer on the wire, the extras'
holding area, the make-up tent, the press pen, a camera tower and a lighting balloon. All /Game/People/ChemicalWorks/
SM_P3F_*; placed by the block C:/ftwork/p3/L1f.inl (pasted by the level's integrator at the end of FTPeopleL1.inl).

How this road lies (FTDressL1.inl, third pass): the barrier hides the ground out to 33 m, a pipe rack (to 7.6 m) or a
tank row (to 11 m) stands at the sight line in most districts. So EVERY piece here carries its people on a deck 2.4 m
or more up, a fifth over life size, and four of them reach over the tank rows (the drones 9 to 15 m up, the camera
tower's head 12 m, the rig's truss 12 m, the lighting balloon 13 m). All are TALL: 24 m or more from the centre line
(the block keeps them 30 m and more out). What marks the film crew: the pink band across the chest, pink-and-white tape
round every deck. The only lights are steady (the balloon, the mirror's bulbs); nothing blinks.

    SM_P3F_SecondUnit_A   a scaffold deck: the second unit's camera panning with its operator, the assistant pointing
                          the shot, the clapper loader, a spotter; a runner to and fro on the ground
    SM_P3F_SecondUnit_B   the camera truck: a jib rising and falling over the flatbed, its operator, a director with a megaphone
    SM_P3F_DroneTeam_A    the launch deck: the pilot at the controller, a spotter pointing; three camera drones circling
                          and hovering 9 to 15 m up
    SM_P3F_DroneTeam_B    the drone van: two pilots on the roof, a dish turning on its mast, two drones circling, one docked
    SM_P3F_StuntRig_A     a truss goalpost 12 m high: a performer swinging on the wire over the air bag, riggers hauling
    SM_P3F_StuntRig_B     a descender tower: a performer hoisted up and down on the wire, the winch crew, a hand-held camera
    SM_P3F_Extras_A / _B  the extras' holding area: a stepped riser of costumed extras (laboratory coats; evacuees in
                          their own clothes) waiting, stretching, waving; the wrangler with a megaphone
    SM_P3F_MakeUp         the make-up tent on its deck: a performer in the chair, the artist's brush, the lit mirror
    SM_P3F_PressPen       the press riser: photographers, a long lens, a video camera panning, a reporter
    SM_P3F_CamTower       a camera tower: the head camera panning 11 m up, a second operator half way
    SM_P3F_LightBalloon   a lighting balloon (steady light) 13 m up on its tethers, the sparks on their deck

Run in the editor:  import ft_people_chem_p3f; ft_people_chem_p3f.run()
A piece is drawn like every prop: cm; it stands on z = 0; its FRONT faces -Y (the road); long pieces run along X.
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/ChemicalWorks'
W = C.WARDROBES['Work']

PINK, WHITE, DARK, GREY, YEL, STEEL = C.PINK, C.WHITE, C.DARK, C.GREY, C.YEL, C.STEEL
POLE, BOARD, LAMP = '#aab6e0', '#d9b98a', '#fff1c0'
ORANGE, LIME, RED, CYAN, TEAL, SILVER = '#ff7a3c', '#b8ff5a', '#ff2d3c', '#2ee6ff', '#19b5c9', '#d7e3f0'
BIG = (1.14, 1.24)          # a fifth over life size: they are read from 30 m, through a rack or a fence


def crew(r, **fixed):
    """One of the film crew in the works' wardrobe (hi-vis, hard hat), the crew's pink band across the chest."""
    d = C.member(r, W)
    d['band'] = PINK
    d['s'] = r.uniform(*BIG)
    d.update(fixed)
    return d


def person(r, **fixed):
    """Somebody who is not crew (an extra, the press): no pink band."""
    d = P.look(r)
    d['s'] = r.uniform(*BIG)
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ furniture
def skirt(sc, x, y, z, length, h=26.0):
    """The crew's mark on every deck: a run of pink-and-white along its front edge."""
    g = sc.still()
    n = max(int(length / 110.0), 2)
    for k in range(n):
        g.box(PINK if k % 2 else WHITE, (x - length / 2.0 + (k + 0.5) * length / n, y, z), (length / n, 5, h))


def deck(sc, x, y, w, d, h, rail=True, brace=True):
    """A scaffold deck h cm up: four legs, the boards, the pink-and-white along the front, a rail along the back."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(POLE, (x + sx * (w / 2.0 - 6), y + sy * (d / 2.0 - 6), (h - 12) / 2.0), (9, 9, h - 12))
    g.box(BOARD, (x, y, h - 6), (w, d, 12))
    if brace:
        tilt_box(g, POLE, (x - w / 2.0 + 6, y + d / 2.0 - 6, 10), (x + w / 2.0 - 6, y + d / 2.0 - 6, h - 16), 7, 7)
    if rail:
        g.box(POLE, (x, y + d / 2.0 - 5, h + 108), (w, 6, 6))
        for sx in (-1, 1):
            g.box(POLE, (x + sx * (w / 2.0 - 6), y + d / 2.0 - 5, h + 54), (6, 6, 108))
    skirt(sc, x, y - d / 2.0 - 3, h - 19, w)


def riser(sc, w, heights, depth=112.0):
    """A stepped stand: one board a step, a face under each, pink-and-white under the first. Returns each step's y."""
    g = sc.still()
    n = len(heights)
    ys = []
    for k, h in enumerate(heights):
        y = (k - (n - 1) / 2.0) * depth
        ys.append(y)
        g.box(BOARD, (0, y, h - 6), (w, depth, 12))
        if k:
            g.box(GREY, (0, y - depth / 2.0 + 3, (h + heights[k - 1]) / 2.0 - 6), (w, 6, h - heights[k - 1]))
    for sx in (-1, 1):
        g.box(POLE, (sx * (w / 2.0 - 8), ys[0], (heights[0] - 12) / 2.0), (9, 9, heights[0] - 12))
        g.box(POLE, (sx * (w / 2.0 - 8), ys[-1], (heights[-1] - 12) / 2.0), (9, 9, heights[-1] - 12))
    tilt_box(g, POLE, (-w / 2.0 + 8, ys[-1], 10), (w / 2.0 - 8, ys[-1], heights[-1] - 16), 7, 7)
    skirt(sc, 0, ys[0] - depth / 2.0 - 3, heights[0] - 19, w)
    return ys


def tower(sc, x, y, w, levels, top):
    """A scaffold tower: four posts to `top`, a deck at every level, a diagonal a storey, pink-and-white at every deck."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(POLE, (x + sx * (w / 2.0 - 5), y + sy * (w / 2.0 - 5), top / 2.0), (10, 10, top))
    z0 = 0.0
    for k, h in enumerate(levels):
        g.box(BOARD, (x, y, h - 6), (w, w, 12))
        sx = -1 if k % 2 else 1
        tilt_box(g, POLE, (x - sx * (w / 2.0 - 5), y + w / 2.0 - 5, z0 + 8), (x + sx * (w / 2.0 - 5), y + w / 2.0 - 5, h - 14), 7, 7)
        g.box(POLE, (x, y + w / 2.0 - 5, h + 106), (w, 6, 6))
        z0 = h
    for h in levels:
        skirt(sc, x, y - w / 2.0 - 3, h - 19, w)


def ground_tape(sc, length, y):
    g = sc.still()
    C.tape(g, length, y, posts=max(int(length / 180.0), 3))


def camera_on_deck(sc, r, x, y, h, swing=24.0, rate=0.11, phase=0.2, **look):
    """A film camera on its tripod on a deck h up, panning; its operator pans with it."""
    g = sc.still(at=(0.0, 0.0, h))
    C.tripod(g, x, y)
    pan = dict(swing=swing, pivot=(x, y, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=rate, phase=phase, **pan))
    C.film_camera(cam, x, y, h + 128)
    d = crew(r, hat='headset', hat_col=DARK)
    d.update(look)
    P.figure(sc, x, y + 88, z=h, arm_l='push', arm_r='push', rate=rate, phase=phase, sway=pan, detail=1, **d)


def hold_controller():
    def f(m, h, side):
        if side > 0:
            m.box(DARK, (h[0] - 26, h[1] - 6, h[2] + 6), (48, 22, 10))
            m.box(PINK, (h[0] - 26, h[1] - 6, h[2] + 13), (12, 8, 6))
            m.box(STEEL, (h[0] - 8, h[1] - 6, h[2] + 26), (4, 4, 34))
    return f


def hold_mic():
    def f(m, h, side):
        m.box(DARK, (h[0], h[1] - 4, h[2] + 12), (5, 5, 28))
        m.box(YEL, (h[0], h[1] - 4, h[2] + 30), (12, 12, 14))
    return f


def hold_brush():
    def f(m, h, side):
        m.box(PINK, (h[0], h[1] - 14, h[2] + 4), (5, 30, 5))
    return f


def hold_panel(col=WHITE, size=110.0):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 26, h[1] - 6, h[2] + size / 2.0), (size, 5, size))
            m.box(PINK, (h[0] - 26, h[1] - 9, h[2] + size / 2.0), (size * 0.7, 2, 12))
    return f


def drone(sc, x, y, z, s=1.6, centre=None, rounds=0.05, phase=0.0, hover=35.0):
    """A camera drone, exaggerated (2.5 m across at s 1.6): it floats on its own beat and, with centre, circles it."""
    mo = dict(phase=phase, rate=0.28, bob=-hover)
    if centre:
        mo.update(base=(centre[0], centre[1]), travel=(-1.0, rounds))
    m = sc.part(P.motion(**mo), at=(x, y, z), s=s)
    m.box(DARK, (0, 0, 0), (72, 52, 22))
    m.box(PINK, (0, 0, 13), (74, 16, 6))
    for a in (45, -45):
        m.box(GREY, (0, 0, 4), (200, 9, 7), rot=(0, a, 0))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.cyl(STEEL, (sx * 70, sy * 70, 9), 36, 3, steps=8)
    m.box(YEL, (0, -8, -20), (22, 22, 20))
    m.cyl(DARK, (0, -19, -20), 7, 12, rot=(0, 0, 90), steps=6)
    for sx in (-1, 1):
        m.box(GREY, (sx * 30, 0, -30), (5, 60, 5))


# ------------------------------------------------------------------------------------------ the pieces
def second_unit_a(sc):
    r = random.Random(301)
    H = 300.0
    deck(sc, 0, 0, 660, 270, H)
    ground_tape(sc, 760, -200)
    g = sc.still()
    C.case(g, 250, 60, z=H)
    C.case(g, 262, 56, z=H + 38, w=52, h=30, col=YEL)
    C.apple_box(g, -270, 70, z=H)
    camera_on_deck(sc, r, -60, -40, H)
    P.figure(sc, 70, -70, z=H, yaw=-30, arm_r='point', swing_r=13, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, 190, -80, z=H, yaw=16, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.35, bob=-2.0, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, -230, -60, z=H, yaw=-14, arm_l='face', arm_r='face', hold_r=P.hold_camera(col=GREY), turn=22, rate=0.14, phase=r.random(), detail=1,
             **crew(r, hat='hard', hat_col=WHITE))
    # the runner, to and fro in front of the deck with a case
    P.figure(sc, 0, -140, yaw=90, stride=26, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=YEL), rate=1.0, bob=3, phase=0.3,
             travel=(640, 150), detail=1, **crew(r))


def second_unit_b(sc):
    r = random.Random(302)
    g = sc.still()
    BED = 126.0
    g.box(DARK, (0, 0, 78), (640, 190, 30))                       # the chassis
    for sx in (-210, 200):
        for sy in (-1, 1):
            g.cyl(DARK, (sx, sy * 100 + 16, 46), 46, 32, rot=(0, 0, 90), steps=10)
    g.box(YEL, (245, 0, 170), (150, 210, 156))                    # the cab
    g.box('#3a5fa8', (245, -106, 196), (110, 3, 66))
    g.box(DARK, (322, 0, 110), (10, 214, 40))
    g.box(BOARD, (-80, 0, BED - 6), (480, 214, 12))               # the flatbed
    g.box(YEL, (-100, 20, BED + 125), (26, 26, 250))              # the jib's post
    g.box(DARK, (-100, 20, BED + 256), (40, 40, 14))
    skirt(sc, -80, -110, BED - 19, 480)
    ground_tape(sc, 760, -200)
    # the jib rises and falls about the top of its post; the camera hangs at its tip
    piv = (-100.0, 20.0, BED + 262.0)
    arm = sc.part(P.motion(swing=11.0, pivot=piv, axis=90.0, rate=0.1))
    tilt_box(arm, YEL, (90, 20, piv[2] - 70), (-420, 20, piv[2] + 118), 16, 22)
    arm.box(DARK, (100, 20, piv[2] - 78), (64, 54, 64))
    arm.box(DARK, (-420, 20, piv[2] + 96), (30, 30, 44))
    C.film_camera(arm, -420, 14, piv[2] + 42, yaw=0.0)
    P.figure(sc, 30, 30, z=BED, yaw=150, arm_l='push', arm_r='push', rock=3, rate=0.1, detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, -230, -40, z=BED, yaw=-12, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12, axis_l='x', rate=0.5,
             phase=r.random(), detail=1, **crew(r, hat='hard', hat_col=WHITE))
    P.figure(sc, 120, -50, z=BED, yaw=20, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=6, rate=1.4, react=True, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, -290, 50, z=BED, yaw=-40, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.5, phase=r.random(), detail=1, **crew(r))


def drone_team_a(sc):
    r = random.Random(303)
    H = 260.0
    deck(sc, 0, 0, 520, 250, H)
    ground_tape(sc, 640, -190)
    g = sc.still()
    g.cyl(WHITE, (150, -10, H), 70, 4, steps=10)                  # the landing pad
    g.cyl(PINK, (150, -10, H + 4), 44, 3, steps=10)
    C.case(g, -200, 70, z=H, col=YEL)
    C.case(g, -196, 66, z=H + 38, w=52, h=30)
    P.figure(sc, -40, -50, z=H, yaw=-8, arm_l='hold', arm_r='hold', hold_r=hold_controller(), turn=10, rate=0.12, phase=r.random(), detail=1,
             **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, -150, -60, z=H, yaw=18, arm_r='up', swing_r=12, axis_r='y', arm_l='hip', rate=0.5, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, 60, 40, z=H, yaw=-30, arm_l='face', arm_r='face', rate=0.3, bob=-1.5, phase=r.random(), detail=1, **crew(r, hat='hard', hat_col=WHITE))
    drone(sc, 230, 0, 930, phase=0.1)                              # hovering by bob only (review round: no orbit beside the road)
    drone(sc, -260, 60, 1230, phase=0.5)
    drone(sc, 120, 150, 1500, s=1.8, phase=0.8)


def drone_team_b(sc):
    r = random.Random(304)
    g = sc.still()
    ROOF = 262.0
    g.box(WHITE, (0, 0, 150), (560, 210, 210))                    # the van
    g.box(PINK, (0, -106, 150), (560, 3, 34))
    g.box('#3a5fa8', (200, -106, 205), (110, 3, 56))
    g.box(DARK, (0, 0, 40), (540, 200, 20))
    for sx in (-190, 190):
        for sy in (-1, 1):
            g.cyl(DARK, (sx, sy * 104 + 14, 40), 40, 28, rot=(0, 0, 90), steps=10)
    g.box(BOARD, (0, 0, ROOF - 3), (560, 214, 8))
    g.box(POLE, (0, 100, ROOF + 104), (560, 6, 6))
    for sx in (-1, 1):
        g.box(POLE, (sx * 274, 100, ROOF + 52), (6, 6, 104))
    g.box(POLE, (-250, 60, 560), (11, 11, 600))                   # the mast
    g.box(POLE, (250, 70, 470), (10, 10, 420))                    # the docking pole
    g.box(DARK, (250, 70, 684), (60, 60, 8))
    skirt(sc, 0, -110, ROOF - 16, 560)
    ground_tape(sc, 680, -200)
    dish = sc.part(P.motion(swing=40.0, pivot=(-250.0, 60.0, 0.0), axis=P.UP, rate=0.08))
    dish.cyl(WHITE, (-250, 44, 880), 50, 16, rot=(0, 0, 90), steps=10, top_r=14)
    dish.box(DARK, (-250, 60, 870), (20, 20, 30))
    P.figure(sc, -90, -40, z=ROOF, yaw=10, arm_l='hold', arm_r='hold', hold_r=hold_controller(), turn=12, rate=0.13, phase=r.random(), detail=1,
             **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, 60, -40, z=ROOF, yaw=-16, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.5, phase=r.random(), detail=1,
             **crew(r, hat='headset', hat_col=PINK))
    P.figure(sc, 180, -30, z=ROOF, yaw=24, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), detail=1, **crew(r))
    drone(sc, 250, 70, 716, s=1.3, hover=0.0)                     # docked on its pole
    drone(sc, 240, -20, 1080, phase=0.3)                           # hovering by bob only (review round)
    drone(sc, -220, 120, 1400, s=1.8, phase=0.7)


def stunt_rig_a(sc):
    r = random.Random(305)
    TOP = 1200.0
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(YEL, (sx * 450, 40 + sy * 40, TOP / 2.0), (14, 14, TOP))
        for k in range(6):
            g.box(YEL, (sx * 450, 40, 180 + k * 190), (10, 86, 10))
        tilt_box(g, DARK, (sx * 450, 40, TOP), (sx * 700, 60, 0), 6, 6)        # the guy line
    g.box(YEL, (0, 40, TOP), (930, 34, 44))
    g.box(DARK, (0, 40, TOP - 30), (60, 50, 30))
    ground_tape(sc, 900, -260)
    deck(sc, 330, 60, 300, 230, 260, brace=False)
    deck(sc, -330, 60, 300, 230, 260, brace=False)
    g = sc.still()
    tilt_box(g, DARK, (300, 20, 400), (40, 40, TOP - 40), 6, 6)                # the hauling line
    bag = sc.part(P.motion(bob=-6.0, rate=0.3))
    bag.box('#2e6bff', (0, -40, 75), (330, 280, 150))
    bag.box('#57c7ff', (0, -40, 153), (260, 220, 8))
    # the performer swings on the wire: a pendulum about the beam
    piv = (0.0, 40.0, TOP - 30.0)
    swing = dict(swing=20.0, pivot=piv, axis=90.0)
    wire = sc.part(P.motion(rate=0.22, phase=0.0, **swing))
    wire.box(DARK, (0, 40, 880), (6, 6, 580))
    P.figure(sc, 0, 40, z=390, arm_l='up', arm_r='up', legs='wide', rate=0.22, phase=0.0, sway=swing, detail=1,
             **person(r, top=SILVER, bottom=SILVER, sleeves='long', hat='helmet', hat_col=RED, band=PINK, s=1.25))
    P.figure(sc, 300, 20, z=260, yaw=70, arm_l='push', arm_r='push', swing_l=16, swing_r=16, axis_l='x', axis_r='x', rate=0.22, phase=0.1, detail=1, **crew(r))
    P.figure(sc, 380, 70, z=260, yaw=60, arm_l='push', arm_r='push', swing_l=14, swing_r=14, axis_l='x', axis_r='x', rate=0.22, phase=0.6, detail=1, **crew(r))
    P.figure(sc, -330, 10, z=260, yaw=-10, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12, axis_l='x', rate=0.5, phase=r.random(), detail=1,
             **crew(r, hat='hard', hat_col=WHITE))
    P.figure(sc, -400, 80, z=260, yaw=30, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=7, rate=1.3, react=True, phase=r.random(), detail=1, **crew(r))


def stunt_rig_b(sc):
    r = random.Random(306)
    TOP = 1250.0
    tower(sc, -200, 40, 280, (400.0, 800.0, 1140.0), TOP)
    g = sc.still()
    g.box(YEL, (60, 40, TOP), (800, 26, 34))                      # the jib
    tilt_box(g, YEL, (-200, 40, 1000), (300, 40, TOP - 12), 12, 12)
    g.box(DARK, (380, 40, 640), (6, 6, 1200))                     # the wire
    g.box(DARK, (-200, 60, 1170), (70, 50, 50))                   # the winch
    g.cyl(STEEL, (-200, 40, 1170), 22, 50, rot=(0, 0, 90), steps=8)
    for k, col in enumerate(('#3a5fa8', RED, '#3a5fa8')):
        g.box(col, (380 + (k % 2) * 8, 30, 14 + k * 28), (240, 220, 26))
    ground_tape(sc, 860, -230)
    # the performer on the wire, hoisted and lowered 3 m each way
    P.figure(sc, 380, 40, z=520, arm_l='out', arm_r='out', legs='wide', bob=-300, rate=0.09, phase=0.0, detail=1,
             **person(r, top=SILVER, bottom=SILVER, sleeves='long', hat='helmet', hat_col=RED, band=PINK, s=1.25))
    P.figure(sc, -240, -20, z=1140, yaw=40, arm_l='push', arm_r='push', swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=0.6, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, -150, -60, z=800, yaw=-20, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), detail=1,
             **crew(r, hat='hard', hat_col=WHITE))
    P.figure(sc, -230, -50, z=400, yaw=-30, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(col=GREY), turn=24, rate=0.12, phase=r.random(), detail=1,
             **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, 120, -110, yaw=40, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=7, rate=1.2, react=True, phase=r.random(), detail=1, **crew(r))


EXTRA_ACTS = (
    dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=0.4),                             # stretching
    dict(arm_r='wave', swing_r=26, rate=1.2, bob=4, react=True),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.2, react=True),     # clapping
    dict(arm_r='face', hold_r=P.hold_cup(), arm_l='hip', rate=0.3, bob=-1.5),
    dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=7, rate=1.3, react=True),
    dict(arm_r='point', swing_r=10, axis_r='x', arm_l='down', rate=0.6),
)


def extras(seed, variant):
    def build(sc):
        r = random.Random(seed)
        heights = (200.0, 272.0, 344.0)
        ys = riser(sc, 780, heights)
        g = sc.still()
        g.box(WHITE, (0, ys[-1] + 58, heights[-1] + 130), (780, 6, 250))          # the sheeting behind the top row
        g.box(PINK, (0, ys[-1] + 54, heights[-1] + 236), (780, 3, 22))
        ground_tape(sc, 860, -230)
        if variant == 'A':      # the urn and the cups
            g = sc.still()
            g.box(WHITE, (330, ys[0], heights[0] + 44), (100, 60, 88))
            g.cyl(STEEL, (330, ys[0], heights[0] + 88), 18, 36, steps=8)
        else:                   # the costume rail, swaying on its hangers
            g = sc.still()
            for sx in (-1, 1):
                g.box(POLE, (300 + sx * 70, ys[-1], heights[-1] + 95), (6, 6, 190))
            g.box(POLE, (300, ys[-1], heights[-1] + 190), (150, 6, 6))
            rail = sc.part(P.motion(swing=5.0, pivot=(300.0, ys[-1], heights[-1] + 190.0), axis=0.0, rate=0.3))
            for k, col in enumerate((RED, CYAN, YEL, '#8f6bff', LIME)):
                rail.box(col, (245 + k * 27, ys[-1], heights[-1] + 130), (20, 34, 110))
        n = 0
        for k in range(3):
            for i in range(3):
                x = -300 + i * 190 + (k % 2) * 80 + r.uniform(-14, 14)
                if variant == 'A':       # the laboratory's extras: white coats, coloured hard hats
                    d = person(r, top=WHITE, wear='coat', sleeves='long', hat=r.choice(('hard', 'hard', None)), hat_col=r.choice((CYAN, YEL, ORANGE, RED)))
                else:                    # the evacuees: everybody's own clothes
                    d = person(r, wear=r.choice(('trousers', 'dress', 'skirt', 'shorts')), hat=r.choice((None, None, 'cap', 'beanie', 'sun')),
                               hat_col=r.choice((RED, CYAN, YEL, PINK)))
                    if (k + i) % 4 == 3:
                        d.update(build='kid', s=0.78)
                act = dict(EXTRA_ACTS[(n * 5 + seed) % len(EXTRA_ACTS)])
                n += 1
                if k < 2 and i == 1 and d.get('build') != 'kid':      # one a row sits on the step behind
                    P.figure(sc, x, ys[k] + 46, z=heights[k], yaw=r.uniform(-12, 12), legs='sit', seat=56, arm_l='hold', arm_r='hold',
                             hold_r=P.hold_cup(), rate=0.3, bob=-1.2, phase=r.random(), detail=1, **d)
                else:
                    P.figure(sc, x, ys[k] - 14, z=heights[k], yaw=r.uniform(-14, 14), phase=r.random(), detail=1 if k == 0 else 0, **dict(d, **act))
        # the wrangler: crew, with the megaphone
        P.figure(sc, -350, ys[0] - 20, z=heights[0], yaw=-20, arm_r='face', hold_r=C.hold_megaphone(), arm_l='hold', hold_l=C.hold_clipboard(),
                 rock=4, rate=0.4, phase=r.random(), detail=1, **crew(r, hat='hard', hat_col=WHITE))
    return build


def make_up(sc):
    r = random.Random(309)
    H = 240.0
    deck(sc, 0, 0, 640, 300, H, rail=False)
    ground_tape(sc, 740, -220)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL, (sx * 300, sy * 132, H + 130), (7, 7, 260))
    for k in range(6):
        g.box(PINK if k % 2 else WHITE, (-266 + k * 106.4, 0, H + 266), (106.4, 300, 10))
    # the mirror and its bulbs (a steady light: this piece stands 24 m and more out)
    g.box(DARK, (-110, 120, H + 150), (190, 8, 150))
    g.box('#9ff0ff', (-110, 115, H + 150), (160, 2, 120))
    for k in range(5):
        g.box(LAMP, (-180 + k * 35, 113, H + 216), (14, 3, 14), glow=0.6)
    g.box(WHITE, (-110, 96, H + 70), (190, 44, 8))
    g.box(BOARD, (-110, -10, H + 34), (54, 50, 68))                # the chair
    g.box(DARK, (-110, 16, H + 96), (54, 6, 60))
    for sx in (-1, 1):
        g.box(POLE, (200 + sx * 80, 90, H + 95), (6, 6, 190))
    g.box(POLE, (200, 90, H + 190), (170, 6, 6))
    rail = sc.part(P.motion(swing=5.0, pivot=(200.0, 90.0, H + 190.0), axis=0.0, rate=0.3))
    for k, col in enumerate((SILVER, RED, ORANGE, CYAN, YEL, '#8f6bff')):
        rail.box(col, (135 + k * 26, 90, H + 128), (19, 34, 112))
    # the performer in the chair, the artist and the hairdresser at work
    P.figure(sc, -110, -10, z=H, legs='sit', seat=62, arm_l='down', arm_r='down', rate=0.3, bob=-1.0, detail=1,
             **person(r, top=SILVER, bottom=SILVER, sleeves='long', hair='long', hair_col='#c94f2e', band=PINK, s=1.2))
    P.figure(sc, -200, -20, z=H, yaw=-80, arm_r='fwd', hold_r=hold_brush(), swing_r=12, axis_r='z', arm_l='hip', rate=1.1, phase=r.random(), detail=1,
             **crew(r, hat=None, hair='bun', top=PINK, band=WHITE))
    P.figure(sc, -40, 50, z=H, yaw=50, arm_l='fwd', arm_r='fwd', swing_l=10, swing_r=-10, axis_l='x', axis_r='x', rate=1.4, phase=r.random(), detail=1,
             **crew(r, hat=None, hair='afro'))
    P.figure(sc, 120, -70, z=H, yaw=14, arm_r='face', hold_r=P.hold_cup(), arm_l='hip', rock=4, rate=0.35, phase=r.random(), detail=1,
             **person(r, top=SILVER, bottom=SILVER, sleeves='long', hat='helmet', hat_col=RED, band=PINK, s=1.2))
    P.figure(sc, 240, -60, z=H, yaw=-24, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.5, phase=r.random(), detail=1, **crew(r))


def press_pen(sc):
    r = random.Random(310)
    heights = (210.0, 292.0)
    ys = riser(sc, 720, heights, depth=130.0)
    ground_tape(sc, 820, -220)
    g = sc.still()
    g.box(POLE, (0, ys[-1] + 60, heights[-1] + 108), (720, 6, 6))
    for sx in (-1, 1):
        g.box(POLE, (sx * 354, ys[-1] + 60, heights[-1] + 54), (6, 6, 108))
    # the video camera on the top step, panning with its operator
    camera_on_deck(sc, r, 220, ys[1] - 30, heights[1], swing=30.0, rate=0.13, phase=0.6, band=WHITE, hat='cap', hat_col=DARK, top='#3a5fa8')
    # the long lens on its stand
    g = sc.still(at=(0.0, 0.0, heights[0]))
    C.tripod(g, -250, ys[0] - 30, top=132.0)
    pan = dict(swing=18.0, pivot=(-250.0, ys[0] - 30.0, 0.0), axis=P.UP)
    lens = sc.part(P.motion(rate=0.16, phase=0.1, **pan))
    lens.box(DARK, (-250, ys[0] - 20, heights[0] + 146), (26, 30, 24))
    lens.cyl(WHITE, (-250, ys[0] - 34, heights[0] + 146), 12, 70, rot=(0, 0, 90), steps=8, top_r=17)
    P.figure(sc, -250, ys[0] + 34, z=heights[0], arm_l='hold', arm_r='hold', rate=0.16, phase=0.1, sway=pan, detail=1,
             **person(r, band=WHITE, hat='cap', hat_col=RED))
    acts = (
        dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=18, rate=0.2),
        dict(arm_l='face', arm_r='face', hold_r=P.hold_camera(col=GREY), turn=14, rate=0.16),
        dict(arm_r='face', hold_r=hold_mic(), arm_l='point', swing_l=12, axis_l='x', rate=0.6),
        dict(arm_r='up', hold_r=P.hold_camera(), swing_r=8, arm_l='hip', rate=0.5, bob=4, react=True),
        dict(arm_r='wave', swing_r=24, rate=1.2, bob=4, react=True, arm_l='hold', hold_l=C.hold_clipboard()),
    )
    spots = ((-120, 0), (10, 0), (140, 0), (-180, 1), (-40, 1))
    for i, (x, k) in enumerate(spots):
        d = person(r, band=WHITE, wear=r.choice(('trousers', 'trousers', 'skirt', 'coat')), hat=r.choice((None, None, 'cap', 'beanie')),
                   hat_col=r.choice((RED, CYAN, DARK)))
        d.update(acts[i % len(acts)])
        P.figure(sc, x + r.uniform(-10, 10), ys[k] - 24, z=heights[k], yaw=r.uniform(-14, 14), phase=r.random(), detail=1, **d)
    # the publicist: crew, keeping the pen
    P.figure(sc, 320, ys[0] - 30, z=heights[0], yaw=30, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.5,
             phase=r.random(), detail=1, **crew(r, hat='hard', hat_col=WHITE))


def cam_tower(sc):
    r = random.Random(311)
    TOP = 1210.0
    tower(sc, 0, 40, 290, (550.0, 1100.0), TOP)
    ground_tape(sc, 620, -210)
    g = sc.still()
    g.box(WHITE, (0, 188, 830), (290, 5, 420))                    # sheeting on the back of the upper storey
    g.box(PINK, (0, 184, 1000), (290, 3, 40))
    C.case(g, 230, 20)
    C.case(g, 240, 16, z=38, w=52, h=30, col=YEL)
    camera_on_deck(sc, r, -20, 0, 1100.0, swing=32.0, rate=0.1, phase=0.4)
    P.figure(sc, 90, -40, z=1100, yaw=-20, arm_r='up', hold_r=P.hold_flag(col=PINK), swing_r=20, arm_l='hip', rate=0.7, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, -40, -50, z=550, yaw=14, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(col=GREY), turn=26, rate=0.14, phase=r.random(), detail=1,
             **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, 70, 20, z=550, yaw=-30, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), detail=1, **crew(r, hat='hard', hat_col=WHITE))
    P.figure(sc, -230, -80, yaw=30, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=6, rate=1.3, react=True, phase=r.random(), detail=1, **crew(r))


def light_balloon(sc):
    r = random.Random(312)
    H = 250.0
    deck(sc, -160, 30, 340, 240, H)
    ground_tape(sc, 760, -210)
    g = sc.still()
    g.box(YEL, (230, 60, 62), (230, 130, 110))                    # the generator
    g.box(DARK, (230, 60, 122), (200, 100, 10))
    g.box(DARK, (310, 60, 160), (12, 12, 70))
    for sx in (150, 310):
        g.cyl(DARK, (sx, 0, 20), 20, 12, rot=(0, 0, 90), steps=8)
    g.cyl(DARK, (40, -60, 0), 34, 30, steps=8)                    # the cable drum
    for a, b in (((-160, 30, H + 90), (0, 60, 1180)), ((300, 100, 120), (40, 60, 1180)), ((-330, 150, 0), (-30, 70, 1180))):
        tilt_box(g, DARK, a, b, 5, 5)
    # the balloon, breathing on its tethers: a steady, soft light
    bal = sc.part(P.motion(bob=-22.0, rate=0.14))
    bal.sphere(LAMP, (0, 60, 1330), 170, scale=(1.5, 1.5, 0.9), steps=(6, 10), glow=0.45)
    bal.cyl(PINK, (0, 60, 1318), 258, 24, steps=10)
    bal.box(DARK, (0, 60, 1180), (40, 40, 26))
    P.figure(sc, -160, 0, z=H, yaw=40, arm_l='up', arm_r='up', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rate=0.3, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, -260, -30, z=H, yaw=-20, arm_l='up', arm_r='up', hold_r=hold_panel(), rock=5, rate=0.25, phase=r.random(), detail=1, **crew(r))
    P.figure(sc, -60, -50, z=H, yaw=10, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), detail=1,
             **crew(r, hat='hard', hat_col=WHITE))
    P.figure(sc, 90, -110, yaw=120, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=GREY), rate=0.3, bob=-1.5, phase=r.random(), detail=1, **crew(r))


WIDE = dict(grow=(340.0, 340.0, 120.0), grow_down=(340.0, 340.0, 0.0))
JOBS = [
    ('SM_P3F_SecondUnit_A', second_unit_a, dict(grow=(120.0, 80.0, 60.0), grow_down=(120.0, 80.0, 0.0))),
    ('SM_P3F_SecondUnit_B', second_unit_b, dict(grow=(80.0, 60.0, 140.0))),
    ('SM_P3F_DroneTeam_A', drone_team_a, dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3F_DroneTeam_B', drone_team_b, dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3F_StuntRig_A', stunt_rig_a, WIDE),
    ('SM_P3F_StuntRig_B', stunt_rig_b, dict(grow=(80.0, 80.0, 340.0))),
    ('SM_P3F_Extras_A', extras(307, 'A')),
    ('SM_P3F_Extras_B', extras(308, 'B')),
    ('SM_P3F_MakeUp', make_up),
    ('SM_P3F_PressPen', press_pen),
    ('SM_P3F_CamTower', cam_tower),
    ('SM_P3F_LightBalloon', light_balloon),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
