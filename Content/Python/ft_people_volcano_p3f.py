"""FULL THROTTLE (Unreal) the population of level 3, the Volcano, pass 3, builder p3f: MORE OF THE SHOOT.

The level is a film shoot on a mountain that is going up. The first round placed the crew kit (ft_people_crew.py, the
Heat wardrobe) on rostrums by the start and before the set pieces. This kit adds the unit types a shoot of this size
would have strung out along 25 km of road, every one on its own raised deck (the Volcano's rule: the barrier is 1.6 m
tall and the land lies under the road, so a person on the ground nearer than 33 m shows a head at most):

    SM_P3F_SecondUnit_A   a second camera position: the camera panning with its operator, the focus puller, the
                          unit director in a tall chair, the clapper loader, a runner crossing the deck with a case
    SM_P3F_SecondUnit_B   the long-lens tower: a scaffold platform 4 m up, the camera under a striped parasol, a
                          spotter with binoculars and a flag marshal on the deck below
    SM_P3F_DroneTeam_A    the drone team: three camera drones hovering 4 to 6 m over the deck (they float by bob, one
                          circles the pad), two pilots, a spotter, the landing pad, the windsock
    SM_P3F_DroneTeam_B    the heavy-lift drone: one big drone with a slung camera circling over its launch table, the
                          pilot seated at the ground station, a spare on its pole stand
    SM_P3F_StuntRig_A     the wire rig: a goalpost truss, a performer swinging on the wire, riggers on the rope,
                          the crash mats
    SM_P3F_StuntRig_B     the descender tower: a performer riding the wire up and down beside the tower, the air bag
                          breathing under it, the safety team in silver with (yellow) extinguishers
    SM_P3F_Extras_A       extras' holding: a striped canopy, benches of villagers in costume waiting, an assistant
                          director calling the next group with a megaphone
    SM_P3F_Extras_B       extras' rehearsal: the costume rail, a line running on the spot for the "flee" shot, the
                          second assistant counting them in
    SM_P3F_MakeUp         the make-up tent: two actors in tall chairs at the mirrors, the artists at work, a big fan
    SM_P3F_PressPen       the press pen: a two-step riser behind tape, photographers and a film crew of their own,
                          a reporter, one on a step-ladder
    SM_P3F_CoolTent       the Volcano's own unit: the cooling station. Crew in heat suits under a mist arch, water
                          barrels, cups, helmets off
    SM_P3F_Rostrum        a rostrum for the crew kit's own pieces (the table stands SM_Crew<Piece>_Heat on it, Up 1.8 m)

Every piece is TALL (a deck 1.8 m up): the table writes them 28 m and more from the centre line. Nothing glows beyond
the small steady lift the level's first round gives its near pieces (0.035; a steady light may stand from 24 m);
nothing blinks. Nothing red and no vehicle (the boss of this level is ENGINE 13, a red fire engine). The crew wear the
Heat wardrobe with the shoot's PINK chest band, behind pink-and-white tape.

Run in the editor:  import ft_people_volcano_p3f as K; K.run()        K.run(['SM_P3F_DroneTeam_A'])
Placed by the block C:/ftwork/p3/L3f.inl (pasted by the integrator into Source/FullThrottle/FTPeopleL3.inl).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/Volcano'
DECK = 180.0
PLANK, PLANK_D, STEEL, STEEL_D = '#e8d8b8', '#c9a070', '#8f9bb3', '#5a5870'
WHITE, DARK, GREY, YEL, LIME, PINK, CYAN = '#fff6ea', '#22202a', '#3a3848', '#ffc93c', '#b8ff5a', '#ff5fa2', '#2ee6ff'
ORANGE, VIOLET, BLUE, MINT, WOOD = '#ff7a3c', '#8f6bff', '#2e6bff', '#7dffb0', '#c9a070'
WATER, WATER_L = '#9fe8ff', '#e6fbff'
CLOTH = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a', '#3a5fa8')
HEAT = C.WARDROBES['Heat']
LIFT = 0.035


def crew(r, **fixed):
    """A crew member in the Heat wardrobe, with the shoot's pink chest band."""
    fixed.setdefault('band', PINK)
    return C.member(r, HEAT, **fixed)


def extra(r, **fixed):
    """An extra in costume: a villager of the mountain, in strong colour."""
    d = P.look(r, tops=CLOTH, wear=r.choice(('trousers', 'skirt', 'dress', 'trousers', 'shorts')),
               hat=r.choice((None, None, 'sun', 'cap', 'beanie')), hat_col=r.choice(CLOTH))
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ what they stand on
def deck(g, L, W=400.0, h=DECK, stripe=(PINK, WHITE), bays=300.0):
    """A raised deck on open legs (the lava channel behind stays in sight under it): planks, edge beams, a pink and
    white stripe along the road edge, legs rooted 2.5 m into the ground, braces on the far side only."""
    g.box(PLANK, (0, 0, h - 7), (L, W, 14))
    g.box(PLANK_D, (0, -W / 2.0 - 4, h - 12), (L, 8, 26))
    g.box(PLANK_D, (0, W / 2.0 + 4, h - 12), (L, 8, 26))
    n = max(int(L / 100.0), 2)
    for k in range(n):
        g.box(stripe[k % 2], (-L / 2.0 + (k + 0.5) * L / n, -W / 2.0 - 9, h - 12), (L / n, 3, 22))
    n = max(int(round(L / bays)), 1)
    for k in range(n + 1):
        x = -L / 2.0 + 10 + k * (L - 20) / n
        for sy in (-1, 1):
            g.box(STEEL_D, (x, sy * (W / 2.0 - 16), (h - 14 - 250) / 2.0), (12, 12, h - 14 + 250))
        if k < n and k % 2 == 0:
            x1 = -L / 2.0 + 10 + (k + 1) * (L - 20) / n
            tilt_box(g, STEEL_D, (x, W / 2.0 - 16, 10), (x1, W / 2.0 - 16, h - 30), 8, 8)


def ptape(g, L, y, z=DECK, posts=5):
    """The crew's barrier on the deck's road edge: posts and two runs of pink-and-white tape, a metre high."""
    for k in range(posts):
        x = -L / 2.0 + k * L / (posts - 1)
        g.box(WHITE, (x, y, z + 52), (9, 9, 104))
    n = max(int(L / 75.0), 2)
    for k in range(n):
        x = -L / 2.0 + (k + 0.5) * L / n
        g.box(PINK if k % 2 else WHITE, (x, y, z + 92), (L / n, 4, 11))
        g.box(WHITE if k % 2 else PINK, (x, y, z + 58), (L / n, 4, 7))


def base(sc, L=1200.0, W=400.0, posts=5):
    g = sc.still()
    deck(g, L, W)
    ptape(g, L - 30, -W / 2.0 + 10, DECK, posts)
    return g


def tripod(m, x, y, z, top=128.0):
    for a in (90, 210, 330):
        rod(m, STEEL, (x, y, z + top), (x + math.cos(math.radians(a)) * 52, y + math.sin(math.radians(a)) * 52, z), 3.5, steps=5)
    m.cyl(DARK, (x, y, z + top - 6), 9, 12, steps=6)


def chair(m, x, y, z, yaw=0.0, seat=72.0):
    """A director's chair on the deck. Its sitter faces -Y at yaw 0."""
    old = m.base
    m.base = P.ft_art._xf((x, y, z), (0.0, yaw, 0.0), (1, 1, 1))
    for sx in (-1, 1):
        tilt_box(m, WOOD, (sx * 26, -22, 0), (sx * 26, 22, seat), 5, 5)
        tilt_box(m, WOOD, (sx * 26, 22, 0), (sx * 26, -22, seat), 5, 5)
        m.box(WOOD, (sx * 26, 24, seat + 30), (5, 5, 62))
    m.box(PINK, (0, 0, seat), (54, 46, 5))
    m.box(PINK, (0, 25, seat + 46), (54, 4, 24))
    m.base = old


def bench(m, x, y, z, L=260.0, col=WOOD):
    m.box(col, (x, y, z + 43), (L, 40, 6))
    for sx in (-1, 1):
        m.box(STEEL_D, (x + sx * (L / 2.0 - 16), y, z + 20), (8, 36, 40))


def canopy(m, x, y, z, w, d, h=250.0, cols=(PINK, WHITE), n=6):
    """A striped canopy on four poles (its roof is drawn in strips along X)."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(STEEL, (x + sx * (w / 2.0 - 6), y + sy * (d / 2.0 - 6), z + h / 2.0), (6, 6, h))
    for k in range(n):
        m.box(cols[k % 2], (x - w / 2.0 + (k + 0.5) * w / n, y, z + h + 5), (w / n, d + 30, 10))
        m.box(cols[(k + 1) % 2], (x - w / 2.0 + (k + 0.5) * w / n, y - d / 2.0 - 15, z + h - 12), (w / n, 3, 24))


def pennant(sc, g, x, y, z, h=330.0, cols=(PINK, WHITE), rate=0.7, phase=0.0, side=1):
    """A pole with a two-colour pennant that swings in the hot wind."""
    g.box(STEEL_D, (x, y, z + h / 2.0), (7, 7, h))
    f = sc.part(P.motion(swing=16.0, pivot=(x, y, z + h - 40), axis=P.UP, rate=rate, phase=phase))
    f.box(cols[0], (x + side * 50, y, z + h - 28), (100, 4, 44))
    f.box(cols[1], (x + side * 50, y, z + h - 60), (100, 4, 20))


def drone(m, x, y, z, s=1.0, body=GREY, trim=YEL, cam=True):
    """A camera drone: a flat body, four arms, four rotor discs, skids, the camera ball slung under it. No light."""
    m.box(body, (x, y, z), (46 * s, 60 * s, 16 * s))
    m.box(trim, (x, y - 31 * s, z), (30 * s, 4, 10 * s))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, DARK, (x, y, z + 2 * s), (x + sx * 62 * s, y + sy * 62 * s, z + 8 * s), 7 * s, 5 * s)
            m.cyl(WHITE, (x + sx * 62 * s, y + sy * 62 * s, z + 12 * s), 34 * s, 3, steps=8)
        m.box(DARK, (x + sx * 26 * s, y, z - 22 * s), (5, 70 * s, 5))
        m.box(DARK, (x + sx * 26 * s, y, z - 12 * s), (5, 5, 20 * s))
    if cam:
        m.sphere(DARK, (x, y - 6 * s, z - 20 * s), 13 * s, steps=(4, 6))
        m.cyl(STEEL, (x, y - 16 * s, z - 20 * s), 6 * s, 10 * s, rot=(0, 0, 90), steps=6)


def hold_controller(col=DARK, trim=YEL):
    """A drone pilot's controller, held in both hands at the chest: a slab with two sticks and an aerial."""
    def f(m, h, side):
        if side < 0:
            return
        m.box(col, (h[0] - 22, h[1] - 6, h[2] + 6), (40, 16, 10))
        m.box(trim, (h[0] - 22, h[1] - 6, h[2] + 12), (22, 10, 3))
        m.box(STEEL, (h[0] - 22, h[1] - 2, h[2] + 22), (4, 4, 28))
    return f


def hold_mic(col=DARK, head=ORANGE):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 3, h[2] + 10), (5, 5, 22))
        m.sphere(head, (h[0], h[1] - 3, h[2] + 26), 8, steps=(3, 6))
    return f


def hold_brush(col=PINK):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 8, h[2] + 4), (4, 22, 4))
        m.box(col, (h[0], h[1] - 22, h[2] + 4), (8, 10, 8))
    return f


def hold_binoculars():
    def f(m, h, side):
        m.box(DARK, (h[0] - side * 6, h[1] - 8, h[2] + 4), (24, 20, 10))
    return f


def hold_extinguisher(col=YEL):
    """Yellow, never red (the boss here is a red fire engine)."""
    def f(m, h, side):
        m.cyl(col, (h[0], h[1], h[2] - 44), 10, 40, steps=7)
        m.cyl(DARK, (h[0], h[1], h[2] - 4), 5, 8, steps=5)
    return f


def stack(g, x, y, z=DECK, cols=(GREY, YEL)):
    C.case(g, x, y, z=z, col=cols[0])
    C.case(g, x + 8, y - 4, z=z + 38, w=52, h=30, col=cols[1])


# ------------------------------------------------------------------------------------------ the second unit
def second_unit_a(sc):
    r = random.Random(301)
    g = base(sc, 1200.0, 400.0)
    cx, cy = -110.0, -50.0
    tripod(g, cx, cy, DECK)
    pan = dict(swing=24.0, pivot=(cx, cy, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=0.13, phase=0.1, **pan))
    C.film_camera(cam, cx, cy, DECK + 128)
    P.figure(sc, cx, cy + 82, z=DECK, arm_l='push', arm_r='push', rate=0.13, phase=0.1, sway=pan, detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, cx + 84, cy + 30, z=DECK, yaw=-35, arm_l='fwd', arm_r='hip', rate=0.3, bob=-1.2, phase=r.random(), detail=1, **crew(r))
    chair(g, -360, 40, DECK, yaw=14)
    P.figure(sc, -360, 40, z=DECK, yaw=14, legs='sit', seat=72, arm_r='face', hold_r=C.hold_megaphone(col=WHITE, trim=PINK), arm_l='point',
             swing_l=12, axis_l='x', rate=0.5, phase=r.random(), detail=1, **crew(r, hat='cap', hat_col=PINK))
    P.figure(sc, 150, -90, z=DECK, yaw=16, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), swing_r=10, axis_r='x', rate=0.4, phase=r.random(),
             detail=1, **crew(r))
    P.figure(sc, 330, -70, z=DECK, yaw=-20, arm_r='point', swing_r=12, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.6,
             phase=r.random(), detail=1, **crew(r))
    # the runner crosses the back of the deck with a case and comes round again
    P.figure(sc, 0, 140, z=DECK, yaw=90, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=YEL), stride=24, rate=1.5, bob=3,
             travel=(1080.0, 130.0), detail=1, **crew(r))
    stack(g, 480, 60)
    C.apple_box(g, -240, 90, z=DECK)
    pennant(sc, g, -570, 170, DECK, cols=(PINK, WHITE), phase=0.2)
    pennant(sc, g, 570, 170, DECK, cols=(WHITE, PINK), phase=0.6, side=-1)


def second_unit_b(sc):
    """The long-lens tower: a scaffold platform 4 m up at one end of the deck."""
    r = random.Random(302)
    g = base(sc, 1000.0, 400.0)
    tx, ty, th = 300.0, 40.0, 220.0       # the tower's middle, and its platform's height over the deck
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL, (tx + sx * 115, ty + sy * 115, DECK + th / 2.0), (9, 9, th))
        tilt_box(g, STEEL_D, (tx + sx * 115, ty - 115, DECK + 10), (tx + sx * 115, ty + 115, DECK + th - 20), 6, 6)
    tilt_box(g, STEEL_D, (tx - 115, ty + 115, DECK + 10), (tx + 115, ty + 115, DECK + th - 20), 6, 6)
    g.box(PLANK, (tx, ty, DECK + th), (260, 260, 12))
    for k in range(4):
        g.box(PINK if k % 2 else WHITE, (tx - 97 + k * 65, ty - 133, DECK + th - 2), (65, 4, 20))
    g.box(WHITE, (tx, ty - 124, DECK + th + 96), (250, 6, 8))
    for sx in (-1, 1):
        g.box(WHITE, (tx + sx * 122, ty - 124, DECK + th + 50), (7, 7, 100))
    for k in range(6):                                   # the ladder up the side
        g.box(YEL, (tx - 150, ty + 60, DECK + 20 + k * 36), (8, 50, 6))
    for sy in (-1, 1):
        g.box(YEL, (tx - 150, ty + 60 + sy * 26, DECK + th / 2.0), (6, 6, th))
    top = DECK + th + 6
    cx, cy = tx, ty - 40.0
    tripod(g, cx, cy, top, top=118.0)
    pan = dict(swing=30.0, pivot=(cx, cy, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=0.1, phase=0.4, **pan))
    C.film_camera(cam, cx, cy, top + 118)
    cam.cyl(STEEL, (cx, cy - 64, top + 135), 12, 70, rot=(0, 0, 90), steps=8)      # the long lens
    cam.cyl(DARK, (cx, cy - 134, top + 135), 16, 14, rot=(0, 0, 90), steps=8)
    P.figure(sc, cx, cy + 80, z=top, arm_l='push', arm_r='push', rate=0.1, phase=0.4, sway=pan, detail=1, **crew(r, hat='headset', hat_col=DARK))
    # the parasol over the camera, nodding in the wind
    g.box(STEEL, (tx + 100, ty + 100, top + 130), (6, 6, 260))
    par = sc.part(P.motion(swing=5.0, pivot=(tx + 100, ty + 100, top + 250), axis=0.0, rate=0.35))
    for k in range(6):
        par.box(PINK if k % 2 else WHITE, (tx - 110 + k * 55, ty + 20, top + 262), (55, 260, 8))
    # on the deck below: the spotter, the flag marshal, the grip with the cases
    P.figure(sc, -120, -70, z=DECK, yaw=-10, arm_l='face', arm_r='face', hold_r=hold_binoculars(), rock=5, rate=0.25, phase=r.random(), detail=1,
             **crew(r))
    P.figure(sc, -300, -80, z=DECK, yaw=8, arm_r='up', hold_r=P.hold_flag(col=PINK, pole=STEEL, w=80.0), swing_r=30, arm_l='hip', rate=0.8, bob=3,
             phase=r.random(), react=True, detail=1, **crew(r))
    P.figure(sc, -30, 80, z=DECK, yaw=150, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=GREY), rate=0.3, bob=-1.2, phase=r.random(), detail=1,
             **crew(r))
    P.figure(sc, -420, 60, z=DECK, yaw=-30, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.5,
             phase=r.random(), detail=1, **crew(r))
    stack(g, -200, 120)
    pennant(sc, g, -470, 170, DECK, cols=(YEL, PINK), phase=0.3)


# ------------------------------------------------------------------------------------------ the drone teams
def drone_team_a(sc):
    r = random.Random(303)
    g = base(sc, 1200.0, 400.0)
    # the landing pad: a yellow square, a dark ring and a cross (no letter)
    g.box(YEL, (0, 60, DECK + 2), (230, 230, 4))
    g.cyl(DARK, (0, 60, DECK + 3), 86, 3, steps=10)
    g.cyl(YEL, (0, 60, DECK + 4), 66, 3, steps=10)
    g.box(DARK, (0, 60, DECK + 7), (110, 16, 3))
    g.box(DARK, (0, 60, DECK + 7), (16, 110, 3))
    # three drones: two float where they are, the third circles the pad
    d1 = sc.part(P.motion(bob=-36.0, rate=0.23, phase=0.1))
    drone(d1, -330, 40, DECK + 470, s=1.2, trim=PINK)
    d2 = sc.part(P.motion(bob=-28.0, rate=0.31, phase=0.6))
    drone(d2, 360, 70, DECK + 600, s=1.0, trim=YEL)
    d3 = sc.part(P.motion(bob=-18.0, rate=0.4, phase=0.3, base=(0.0, 60.0), travel=(-1.0, 0.085)))
    drone(d3, 120, 60, DECK + 380, s=1.3, trim=CYAN)
    P.figure(sc, -250, -60, z=DECK, yaw=10, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rock=4, rate=0.3, phase=r.random(), detail=1,
             **crew(r, hat='cap', hat_col=PINK))
    P.figure(sc, 250, -70, z=DECK, yaw=-14, arm_l='hold', arm_r='hold', hold_r=hold_controller(trim=CYAN), rock=4, rate=0.27, phase=r.random(),
             detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, -470, -40, z=DECK, yaw=20, arm_r='up', swing_r=14, arm_l='face', rate=0.6, bob=2, phase=r.random(), react=True, detail=1, **crew(r))
    P.figure(sc, 480, 110, z=DECK, yaw=170, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=YEL), rate=0.3, bob=-1.0, phase=r.random(),
             detail=1, **crew(r))
    # the ground station: a table, a hooded monitor (dark, unlit), battery cases
    g.box(STEEL, (-60, -110, DECK + 78), (150, 60, 6))
    for sx in (-1, 1):
        g.box(STEEL_D, (-60 + sx * 66, -110, DECK + 38), (6, 50, 76))
    g.box(DARK, (-60, -104, DECK + 102), (70, 34, 44))
    g.box(BLUE, (-60, -122, DECK + 102), (58, 2, 34))
    stack(g, 520, -40, cols=(YEL, GREY))
    C.case(g, -540, 110, z=DECK, col=YEL)
    # the windsock
    g.box(STEEL_D, (575, 170, DECK + 190), (7, 7, 380))
    w = sc.part(P.motion(swing=20.0, pivot=(575.0, 170.0, DECK + 360), axis=P.UP, rate=0.4))
    for k in range(4):
        w.cyl(PINK if k % 2 else WHITE, (575 - 40 - k * 48, 170, DECK + 360), 22 - k * 3.5, 48, rot=(90, 0, 0), steps=6, top_r=22 - (k + 1) * 3.5)


def drone_team_b(sc):
    r = random.Random(304)
    g = base(sc, 1100.0, 400.0)
    # the heavy-lift drone circles over its launch table, a camera slung under it on two lines
    big = sc.part(P.motion(bob=-30.0, rate=0.2, phase=0.2, base=(-80.0, 40.0), travel=(-1.0, 0.06)))
    bx, by, bz = 40.0, 40.0, DECK + 560.0
    drone(big, bx, by, bz, s=1.7, body=GREY, trim=PINK, cam=False)
    for sx in (-1, 1):
        big.box(DARK, (bx + sx * 26, by, bz - 80), (4, 4, 110))
    big.box(DARK, (bx, by, bz - 150), (70, 50, 40))
    big.cyl(STEEL, (bx, by - 25, bz - 150), 12, 26, rot=(0, 0, 90), steps=7)
    big.box(YEL, (bx, by + 26, bz - 150), (40, 4, 22))
    # the launch table
    g.box(YEL, (-80, 40, DECK + 60), (220, 180, 8))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL_D, (-80 + sx * 96, 40 + sy * 76, DECK + 28), (8, 8, 56))
    g.cyl(DARK, (-80, 40, DECK + 64), 60, 3, steps=10)
    g.cyl(YEL, (-80, 40, DECK + 65), 44, 3, steps=10)
    # a spare on its pole stand, breathing a little in the wind
    g.box(STEEL_D, (400, 120, DECK + 130), (8, 8, 260))
    sp = sc.part(P.motion(swing=5.0, pivot=(400.0, 120.0, DECK + 260), axis=0.0, rate=0.5))
    drone(sp, 400, 120, DECK + 284, s=1.1, trim=CYAN)
    # the pilot at the ground station, the camera operator beside, the spotter, the safety marshal
    C.apple_box(g, -380, 60, z=DECK)
    g.box(STEEL, (-380, -40, DECK + 78), (130, 54, 6))
    for sx in (-1, 1):
        g.box(STEEL_D, (-380 + sx * 58, -40, DECK + 38), (6, 46, 76))
    g.box(DARK, (-380, -36, DECK + 100), (64, 30, 40))
    P.figure(sc, -380, 60, z=DECK - 14, legs='sit', seat=46, arm_l='hold', arm_r='hold', hold_r=hold_controller(trim=PINK), rate=0.3, bob=-0.8,
             phase=r.random(), detail=1, **crew(r, hat='headset', hat_col=DARK))
    P.figure(sc, -260, -70, z=DECK, yaw=-12, arm_l='hold', arm_r='hold', hold_r=hold_controller(trim=CYAN), rock=4, rate=0.28, phase=r.random(),
             detail=1, **crew(r, hat='cap', hat_col=DARK))
    P.figure(sc, 200, -80, z=DECK, yaw=10, arm_r='up', swing_r=16, arm_l='hip', rate=0.55, bob=2, phase=r.random(), react=True, detail=1, **crew(r))
    P.figure(sc, 430, -70, z=DECK, yaw=-18, arm_l='out', arm_r='out', swing_l=8, swing_r=-8, rate=0.5, phase=r.random(), detail=1,
             **crew(r, top=YEL, hat='hard', hat_col=WHITE))
    stack(g, 500, 60, cols=(GREY, YEL))
    pennant(sc, g, -520, 170, DECK, cols=(CYAN, WHITE), phase=0.5)


# ------------------------------------------------------------------------------------------ stunt rigging
def truss(g, a, b, w=22.0, col=STEEL, cross=STEEL_D):
    """A square lattice truss from a to b (both on one vertical or one level line): four chords and a zig-zag."""
    vertical = abs(b[2] - a[2]) > abs(b[0] - a[0])
    L = (b[2] - a[2]) if vertical else (b[0] - a[0])
    n = max(int(abs(L) / 110.0), 2)
    for s1 in (-1, 1):
        for s2 in (-1, 1):
            if vertical:
                g.box(col, (a[0] + s1 * w, a[1] + s2 * w, a[2] + L / 2.0), (6, 6, abs(L)))
            else:
                g.box(col, (a[0] + L / 2.0, a[1] + s1 * w, a[2] + s2 * w), (abs(L), 6, 6))
    for k in range(n):
        t0, t1 = k / float(n), (k + 1) / float(n)
        s = 1 if k % 2 else -1
        if vertical:
            tilt_box(g, cross, (a[0] - s * w, a[1] - w, a[2] + L * t0), (a[0] + s * w, a[1] - w, a[2] + L * t1), 5, 5)
        else:
            tilt_box(g, cross, (a[0] + L * t0, a[1] - w, a[2] - s * w), (a[0] + L * t1, a[1] - w, a[2] + s * w), 5, 5)


def stunt_rig_a(sc):
    """The wire rig: a goalpost truss over the deck, a performer swinging along the road on the wire."""
    r = random.Random(305)
    g = base(sc, 1200.0, 400.0)
    H = 560.0
    for sx in (-1, 1):
        truss(g, (sx * 420, 60, DECK), (sx * 420, 60, DECK + H))
        g.box(YEL, (sx * 420, 60, DECK + 6), (90, 90, 12))
    truss(g, (-442, 60, DECK + H), (442, 60, DECK + H))
    # the performer on the wire: a pendulum from the cross beam, in the plane of the deck
    swing = dict(swing=24.0, pivot=(0.0, 60.0, DECK + H), axis=90.0)
    wire = sc.part(P.motion(rate=0.22, phase=0.0, **swing))
    wire.box(DARK, (0, 60, DECK + H - 150), (4, 4, 300))
    P.figure(sc, 0, 60, z=DECK + 90, arm_l='out', arm_r='out', legs='stride', rate=0.22, phase=0.0, sway=swing, detail=1,
             **crew(r, top=ORANGE, bottom=ORANGE, hat='helmet', hat_col=WHITE, band=None))
    # the riggers on the rope at one post, the coordinator, the mats
    rope = sc.part(P.motion(swing=3.0, pivot=(420.0, 60.0, DECK + H), axis=0.0, rate=0.22))
    tilt_box(rope, WHITE, (420, 40, DECK + H), (300, -40, DECK + 110), 4, 4)
    P.figure(sc, 290, -50, z=DECK, yaw=-60, arm_l='up', arm_r='up', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rock=0, rate=0.22, phase=0.5,
             detail=1, **crew(r))
    P.figure(sc, 220, -90, z=DECK, yaw=-70, arm_l='fwd', arm_r='fwd', swing_l=10, swing_r=10, axis_l='x', axis_r='x', rate=0.22, phase=0.5, detail=1,
             **crew(r, build='stocky'))
    P.figure(sc, -300, -80, z=DECK, yaw=22, arm_r='point', swing_r=12, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.6, phase=r.random(),
             detail=1, **crew(r, hat='cap', hat_col=PINK))
    for k, col in enumerate((BLUE, CYAN, BLUE)):
        g.box(col, (-60 + (k % 2) * 8, 70, DECK + 14 + k * 28), (330, 200, 26))
    g.box(WHITE, (-60, 70, DECK + 86), (120, 90, 3))
    P.figure(sc, -520, 60, z=DECK, yaw=40, arm_l='vee', arm_r='vee', bob=9, rate=1.2, phase=r.random(), detail=1,
             **crew(r, top=LIME, bottom=DARK, hat='helmet', hat_col=YEL, band=None))                       # the next one up, warming up
    C.case(g, 530, 130, z=DECK, col=YEL)


def stunt_rig_b(sc):
    """The descender: a tower with a jib, a performer riding the wire up and down, the air bag breathing below."""
    r = random.Random(306)
    g = base(sc, 1200.0, 400.0)
    H = 640.0
    truss(g, (-380, 90, DECK), (-380, 90, DECK + H), w=30.0, col=YEL)
    g.box(GREY, (-380, 90, DECK + 6), (120, 120, 12))
    truss(g, (-410, 90, DECK + H), (-60, 90, DECK + H), w=18.0, col=YEL)
    g.box(DARK, (-90, 90, DECK + H - 24), (30, 30, 30))
    g.box(DARK, (-90, 90, DECK + H - 190), (4, 4, 300))
    ride = dict(bob=-140.0, rate=0.16, phase=0.0)
    w = sc.part(P.motion(**ride))
    w.box(DARK, (-90, 90, DECK + 480), (4, 4, 120))
    P.figure(sc, -90, 90, z=DECK + 300, arm_l='vee', arm_r='vee', legs='wide', detail=1,
             **dict(crew(r, top=PINK, bottom=PINK, hat='helmet', hat_col=WHITE, band=None), **ride))
    # the air bag, breathing on its fan
    bag = sc.part(P.motion(bob=-5.0, rate=0.3))
    bag.box(BLUE, (-60, 70, DECK + 60), (360, 240, 120))
    bag.box(CYAN, (-60, 70, DECK + 123), (300, 190, 8))
    bag.box(WHITE, (-60, 70, DECK + 128), (110, 110, 3))
    g.box(GREY, (160, 150, DECK + 26), (56, 56, 52))
    # the safety team in silver, a coordinator with the megaphone, a performer waiting her turn
    P.figure(sc, 260, -70, z=DECK, yaw=-28, arm_r='down', hold_r=hold_extinguisher(), arm_l='hip', rate=0.3, bob=-1.0, phase=r.random(), detail=1,
             **crew(r, top='#d7e3f0', bottom='#c9c4d6', hat='helmet', hat_col='#c9c4d6'))
    P.figure(sc, 370, -90, z=DECK, yaw=-20, arm_l='down', hold_l=hold_extinguisher(), arm_r='point', swing_r=10, axis_r='x', rate=0.5,
             phase=r.random(), detail=1, **crew(r, top='#d7e3f0', bottom='#c9c4d6', hat='helmet', hat_col='#c9c4d6'))
    P.figure(sc, 120, -90, z=DECK, yaw=-40, arm_r='face', hold_r=C.hold_megaphone(col=WHITE, trim=PINK), arm_l='up', swing_l=14, rate=0.6,
             phase=r.random(), detail=1, **crew(r, hat='cap', hat_col=PINK))
    P.figure(sc, 500, 40, z=DECK, yaw=30, arm_l='hip', arm_r='up', rock=10, rate=0.5, phase=r.random(), detail=1,
             **crew(r, top=LIME, bottom=DARK, hat='helmet', hat_col=YEL, band=None))
    P.figure(sc, -540, -60, z=DECK, yaw=24, arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.0, bob=3,
             phase=r.random(), react=True, detail=1, **crew(r))
    stack(g, 520, 150, cols=(YEL, GREY))


# ------------------------------------------------------------------------------------------ the extras
def extras_a(sc):
    """Extras' holding: villagers in costume waiting under a canopy, the assistant director calling the next group."""
    r = random.Random(307)
    g = base(sc, 1200.0, 400.0)
    canopy(g, -180, 60, DECK, 700.0, 250.0, cols=(PINK, WHITE), n=7)
    for row, by in enumerate((30.0, 130.0)):
        bench(g, -330, by, DECK)
        bench(g, -30, by, DECK)
        for k in range(5):
            x = -420 + k * 110 + r.uniform(-12, 12)
            acts = (dict(arm_l='hold', arm_r='hold', hold_r=P.hold_cup(), bob=-0.8, rate=0.3),
                    dict(arm_r='face', hold_r=P.hold_phone(), bob=-0.6, rate=0.25),
                    dict(arm_r='wave', swing_r=22, rate=1.1, react=True),
                    dict(arm_l='down', arm_r='hip', rock=4, rate=0.3),
                    dict(arm_l='hold', arm_r='hold', hold_r=P.hold_bag(col=r.choice(CLOTH)), bob=-0.8, rate=0.3))
            d = extra(r)
            d.update(acts[(k + row * 2) % len(acts)])
            if row == 1 and k == 2:
                d.update(build='kid', s=0.66)
            P.figure(sc, x, by, z=DECK, legs='sit', seat=46, yaw=r.uniform(-12, 12), phase=r.random(), detail=1 if row == 0 else 0, **d)
    # the assistant director with the megaphone and the list, and the group that has been called: on their feet
    P.figure(sc, 300, -80, z=DECK, yaw=120, arm_r='face', hold_r=C.hold_megaphone(col=WHITE, trim=PINK), arm_l='hold', hold_l=C.hold_clipboard(),
             rock=5, rate=0.5, phase=r.random(), detail=1, **crew(r, hat='headset', hat_col=DARK))
    for k in range(3):
        d = extra(r, hat=None)
        P.figure(sc, 400 + k * 70, 30 + (k % 2) * 60, z=DECK, yaw=-20 + k * 10, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=6, rate=1.5,
                 phase=r.random(), react=True, detail=1, **d)
    # the urn and the cups
    g.box(WHITE, (540, 150, DECK + 40), (110, 60, 80))
    g.cyl(STEEL, (540, 150, DECK + 80), 18, 40, steps=8)
    pennant(sc, g, 575, -150, DECK, h=300.0, cols=(YEL, PINK), phase=0.2, side=-1)


def extras_b(sc):
    """Extras' rehearsal: a line running on the spot for the "flee" shot, the costume rail, the second assistant."""
    r = random.Random(308)
    g = base(sc, 1200.0, 400.0)
    # the costume rail: a row of coloured garments swaying on their hangers
    g.box(STEEL, (-380, 140, DECK + 170), (360, 6, 6))
    for sx in (-1, 1):
        g.box(STEEL, (-380 + sx * 178, 140, DECK + 85), (6, 6, 170))
        g.box(STEEL_D, (-380 + sx * 178, 140, DECK + 4), (10, 70, 8))
    for k in range(9):
        cl = sc.part(P.motion(swing=6.0, pivot=(-540.0 + k * 40, 140.0, DECK + 170), axis=0.0, rate=0.5, phase=k * 0.13))
        cl.box(CLOTH[k % len(CLOTH)], (-540 + k * 40, 140, DECK + 112), (30, 12, 104))
    P.figure(sc, -380, 60, z=DECK, yaw=180, arm_l='up', arm_r='hold', swing_l=8, axis_l='x', rate=0.4, phase=r.random(), detail=1, **crew(r))
    # the runners: seven villagers jogging on the spot, out of step, bundles in their arms
    for k in range(7):
        d = extra(r)
        if k == 3:
            d.update(build='kid', s=0.66)
        hold = (P.hold_bag(col=r.choice(CLOTH)) if k % 3 == 0 else (P.hold_box(col=r.choice((WOOD, YEL, PINK))) if k % 3 == 1 else None))
        P.figure(sc, -120 + k * 95 + r.uniform(-10, 10), 20 + (k % 2) * 90, z=DECK, yaw=90 + r.uniform(-10, 10),
                 arm_l='carry' if hold else 'hold', arm_r='carry' if hold else 'hold', hold_r=hold, stride=30, bob=7,
                 swing_l=0 if hold else 26, swing_r=0 if hold else -26, axis_l='x', axis_r='x', rate=1.7 + r.uniform(-0.15, 0.15), phase=r.random(),
                 detail=1 if k % 2 == 0 else 0, **d)
    # the second assistant counts them in: an arm chopping the beat, the clapper beside
    P.figure(sc, 560, -70, z=DECK, yaw=75, arm_r='up', swing_r=30, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.85,
             phase=r.random(), detail=1, **crew(r, hat='cap', hat_col=PINK))
    P.figure(sc, -220, -90, z=DECK, yaw=-10, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.4, bob=-1.2, phase=r.random(), detail=1,
             **crew(r))
    C.apple_box(g, 470, 150, z=DECK)
    pennant(sc, g, -575, -150, DECK, h=300.0, cols=(CYAN, PINK), phase=0.6)


# ------------------------------------------------------------------------------------------ make-up
def make_up(sc):
    r = random.Random(309)
    g = base(sc, 1100.0, 400.0)
    canopy(g, -120, 70, DECK, 640.0, 260.0, h=260.0, cols=(WHITE, PINK), n=8)
    # two mirror stations, their backs to the road: a frame, a pale mirror, a row of bulbs (unlit)
    for k, mx in enumerate((-280.0, 20.0)):
        g.box(WHITE, (mx, 170, DECK + 150), (150, 8, 130))
        g.box('#d7e3f0', (mx, 165, DECK + 150), (126, 3, 106))
        for j in range(5):
            g.box(YEL, (mx - 60 + j * 30, 164, DECK + 210), (12, 4, 12))
        g.box(WOOD, (mx, 150, DECK + 78), (160, 50, 6))
        for sx in (-1, 1):
            g.box(STEEL_D, (mx + sx * 70, 150, DECK + 38), (6, 40, 76))
        chair(g, mx, 70, DECK, yaw=180)
        # the actor in the chair, facing the mirror (in costume: the hero's bright jacket, the villain's dark coat)
        P.figure(sc, mx, 70, z=DECK, yaw=180, legs='sit', seat=72, arm_l='down', arm_r='hold', hold_r=P.hold_cup(), rate=0.25, bob=-0.6,
                 phase=r.random(), detail=1,
                 **P.look(r, top=(CYAN, VIOLET)[k], bottom=(WHITE, DARK)[k], sleeves='long', hair=('pony', 'short')[k], hat=None))
        # the artist beside, working with a brush
        P.figure(sc, mx + 62, 40, z=DECK, yaw=150, arm_r='face', hold_r=hold_brush(col=(PINK, YEL)[k]), swing_r=9, axis_r='x', arm_l='hip', rate=1.1,
                 phase=r.random(), detail=1, **crew(r, hat=None, top=WHITE))
    # the hairdresser with the dryer, and the runner bringing iced water
    P.figure(sc, -350, 30, z=DECK, yaw=-150, arm_l='face', hold_l=hold_mic(col=GREY, head=MINT), swing_l=12, axis_l='x', arm_r='hip', rate=0.9,
             phase=r.random(), detail=1, **crew(r, hat=None, top=PINK, band=None))
    P.figure(sc, 0, -90, z=DECK, yaw=90, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=WATER, w=40.0), stride=22, rate=1.3, bob=3,
             travel=(980.0, 110.0), detail=1, **crew(r))
    # the big fan on its stand: the blades rock (the head turns to and fro)
    g.box(STEEL_D, (340, 60, DECK + 70), (8, 8, 140))
    g.box(STEEL_D, (340, 60, DECK + 4), (80, 80, 8))
    fan = sc.part(P.motion(swing=40.0, pivot=(340.0, 60.0, DECK + 150), axis=P.UP, rate=0.25))
    fan.cyl(STEEL, (340, 30, DECK + 150), 56, 14, rot=(0, 0, 90), steps=10)
    fan.cyl(CYAN, (340, 22, DECK + 150), 46, 4, rot=(0, 0, 90), steps=10)
    fan.cyl(DARK, (340, 18, DECK + 150), 12, 6, rot=(0, 0, 90), steps=6)
    # the wardrobe trunk and the next one waiting, fanning herself
    C.case(g, 470, 140, z=DECK, w=110.0, d=60.0, h=70.0, col=PINK)
    P.figure(sc, 470, 20, z=DECK, yaw=-20, arm_r='face', swing_r=16, axis_r='z', arm_l='hip', rate=1.6, phase=r.random(), detail=1,
             **P.look(r, top=YEL, wear='dress', hair='bun', hat=None))
    pennant(sc, g, 525, -150, DECK, h=300.0, cols=(PINK, WHITE), phase=0.4, side=-1)


# ------------------------------------------------------------------------------------------ the press
def press_pen(sc):
    """The press pen: a two-step riser, photographers, a film crew of their own, a reporter. No flash: nothing blinks."""
    r = random.Random(310)
    g = base(sc, 1200.0, 400.0, posts=7)
    step = 50.0
    g.box(PLANK_D, (-100, 70, DECK + step / 2.0), (800, 130, step))
    g.box(PLANK_D, (-100, 150, DECK + step), (800, 90, step * 2.0))
    for sx in (-1, 1):                                   # the pen's side tape
        g.box(WHITE, (-100 + sx * 420, 60, DECK + 52), (9, 9, 104))
        g.box(PINK, (-100 + sx * 420, -60, DECK + 92), (4, 240, 11))
    snappers = (
        dict(arm_l='face', arm_r='face', hold_r=P.hold_camera(), rock=5, rate=0.4),
        dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(lens=YEL), turn=14, rate=0.3),
        dict(arm_r='up', hold_r=P.hold_phone(), arm_l='hip', swing_r=8, rate=0.5, bob=2),
        dict(arm_l='face', arm_r='face', hold_r=P.hold_camera(col=GREY), rock=6, rate=0.33),
        dict(arm_r='wave', swing_r=24, arm_l='hold', hold_l=C.hold_clipboard(), rate=1.2, bob=4, react=True),
    )
    press_tops = ('#3a5fa8', '#fff6ea', '#ffc93c', '#8f6bff', '#2ee6ff', '#f4a6ff', '#7dffb0')
    for row, (py, pz, n) in enumerate(((-70.0, DECK, 6), (70.0, DECK + step, 5), (150.0, DECK + step * 2.0, 4))):
        for k in range(n):
            x = -100 - 340 + (k + 0.5) * 680.0 / n + r.uniform(-14, 14)
            d = P.look(r, tops=press_tops, hat=r.choice((None, None, 'cap', 'sun')), hat_col=r.choice(press_tops), band=WHITE)
            d.update(snappers[(k + row * 2) % len(snappers)])
            P.figure(sc, x, py, z=pz, yaw=r.uniform(-12, 12), phase=r.random(), detail=1 if row == 0 else 0, **d)
    # the television crew: the reporter facing her own camera, the operator, the sound recordist with a boom
    P.figure(sc, 420, -60, z=DECK, yaw=90, arm_r='face', hold_r=hold_mic(), arm_l='point', swing_l=14, axis_l='x', rate=0.7, phase=r.random(),
             detail=1, **P.look(r, top=PINK, bottom=WHITE, wear='skirt', hair='long', hat=None))
    tripod(g, 540, -60, DECK, top=118.0)
    C.film_camera(g, 540, -60, DECK + 118, yaw=90)
    P.figure(sc, 560, 20, z=DECK, yaw=120, arm_l='push', arm_r='push', rate=0.3, bob=-1.0, phase=r.random(), detail=1,
             **P.look(r, top=DARK, hat='cap', hat_col=CYAN, band=WHITE))
    P.figure(sc, 440, 60, z=DECK, yaw=150, arm_l='up', arm_r='up', hold_r=C.hold_boom(length=210.0), rock=3, rate=0.25, phase=r.random(), detail=1,
             **P.look(r, top=MINT, hat='headset', hat_col=DARK, band=WHITE))
    # the step-ladder photographer at the end
    for k in range(3):
        g.box(YEL, (-560, 60, DECK + 30 + k * 34), (70, 40 - k * 6, 6))
    for sx in (-1, 1):
        tilt_box(g, YEL, (-560 + sx * 34, 30, DECK), (-560 + sx * 30, 70, DECK + 104), 5, 5)
        tilt_box(g, YEL, (-560 + sx * 34, 110, DECK), (-560 + sx * 30, 70, DECK + 104), 5, 5)
    P.figure(sc, -560, 60, z=DECK + 100, yaw=8, arm_l='face', arm_r='face', hold_r=P.hold_camera(lens=CYAN), rock=4, rate=0.35, phase=r.random(),
             detail=1, **P.look(r, top=ORANGE, hat='cap', hat_col=WHITE, band=WHITE))


# ------------------------------------------------------------------------------------------ the cooling station
def cool_tent(sc):
    """The Volcano's own unit: crew in heat suits cooling off under a mist arch, water barrels, helmets off."""
    r = random.Random(311)
    g = base(sc, 1100.0, 400.0)
    # the mist arch: a hoop of pipe, the mist falling in bands that drift down and come back
    for sx in (-1, 1):
        g.box(STEEL, (-300 + sx * 110, 40, DECK + 125), (8, 8, 250))
    g.box(STEEL, (-300, 40, DECK + 252), (228, 8, 8))
    for k in range(5):
        mist = sc.part(P.motion(bob=-34.0, rate=0.5, phase=k * 0.2))
        mist.box(WATER_L if k % 2 else WATER, (-300 - 88 + k * 44, 40, DECK + 150), (20, 6, 150))
    # a silver heat suit under the arch, arms out, turning; another shaking the water off
    silver = dict(top='#d7e3f0', bottom='#c9c4d6', hat='helmet', hat_col='#c9c4d6')
    P.figure(sc, -300, 40, z=DECK, arm_l='out', arm_r='out', turn=40, rate=0.3, phase=r.random(), detail=1, **crew(r, **silver))
    P.figure(sc, -140, -60, z=DECK, yaw=-20, arm_l='vee', arm_r='vee', swing_l=20, swing_r=-20, bob=5, rate=1.3, phase=r.random(), detail=1,
             **crew(r, top='#d7e3f0', bottom='#c9c4d6', hat=None))
    # the shade and the benches: helmets off, cups in hand
    canopy(g, 200, 80, DECK, 520.0, 220.0, h=240.0, cols=(CYAN, WHITE), n=6)
    bench(g, 200, 120, DECK, L=420.0)
    for k in range(4):
        d = crew(r, hat=None)
        act = (dict(arm_l='hold', arm_r='face', hold_r=P.hold_cup(col=CYAN), swing_r=6, axis_r='x', rate=0.4),
               dict(arm_l='down', arm_r='hip', rock=3, rate=0.3),
               dict(arm_r='face', swing_r=16, axis_r='z', arm_l='down', rate=1.5),
               dict(arm_l='hold', arm_r='hold', hold_r=P.hold_cup(col=WHITE), bob=-0.8, rate=0.3))[k]
        d.update(act)
        P.figure(sc, 50 + k * 100, 120, z=DECK, legs='sit', seat=46, yaw=r.uniform(-10, 10), phase=r.random(), detail=1, **d)
        if k % 2 == 0:
            g.sphere(ORANGE, (50 + k * 100 + 50, 120, DECK + 58), 18, steps=(3, 6))          # a helmet on the bench
    # the water point: barrels on a stand, the medic handing out cups
    for k in range(3):
        g.cyl(BLUE, (330 + k * 62, -40, DECK + 60), 26, 70, steps=8)
        g.cyl(WHITE, (330 + k * 62, -40, DECK + 130), 20, 6, steps=8)
    g.box(STEEL_D, (392, -40, DECK + 30), (210, 60, 60))
    P.figure(sc, 250, -70, z=DECK, yaw=40, arm_r='fwd', hold_r=P.hold_cup(col=CYAN), swing_r=10, axis_r='x', arm_l='hold', rate=0.6,
             phase=r.random(), detail=1, **crew(r, top=WHITE, hat='cap', hat_col=MINT, band=MINT))
    # the fan
    g.box(STEEL_D, (-480, 100, DECK + 70), (8, 8, 140))
    fan = sc.part(P.motion(swing=35.0, pivot=(-480.0, 100.0, DECK + 150), axis=P.UP, rate=0.22))
    fan.cyl(STEEL, (-480, 70, DECK + 150), 52, 14, rot=(0, 0, 90), steps=10)
    fan.cyl(CYAN, (-480, 62, DECK + 150), 42, 4, rot=(0, 0, 90), steps=10)
    P.figure(sc, -480, -70, z=DECK, yaw=10, arm_r='wave', swing_r=22, arm_l='hip', rate=1.1, bob=3, phase=r.random(), react=True, detail=1,
             **crew(r))
    pennant(sc, g, 525, 170, DECK, cols=(CYAN, WHITE), phase=0.1, side=-1)


# ------------------------------------------------------------------------------------------ a rostrum for the kit
def rostrum(sc):
    """A raised deck for the crew kit's own pieces (the table stands them on it, Up 1.8 m): 15 m by 4.6 m, a parasol
    at one end, pennants, steps up the back."""
    g = sc.still()
    L, W = 1500.0, 460.0
    deck(g, L, W, stripe=(WHITE, PINK))
    pennant(sc, g, -L / 2.0 + 14, W / 2.0 - 14, DECK, h=400.0, cols=(PINK, YEL), phase=0.0)
    pennant(sc, g, L / 2.0 - 14, W / 2.0 - 14, DECK, h=400.0, cols=(YEL, PINK), phase=0.4, side=-1)
    g.box(STEEL, (0, W / 2.0 - 14, DECK + 170), (6, 6, 340))
    par = sc.part(P.motion(swing=4.0, pivot=(0.0, W / 2.0 - 14, DECK + 330), axis=0.0, rate=0.3))
    for k in range(6):
        par.box(PINK if k % 2 else WHITE, (-125 + k * 50, W / 2.0 - 60, DECK + 342), (50, 180, 8))
    for sx in (-1, 1):                                  # steps up the back
        for k in range(4):
            g.box(PLANK_D, (sx * 520, W / 2.0 + 30 + k * 34, DECK - 30 - k * 44), (120, 34, 10))


def lifted(fn, g=LIFT):
    """The piece's colours given a little steady light of their own, as the level's first round does for its near
    pieces: against the black works an unlit figure is a dark shape. (Every piece here stands 28 m out and more.)"""
    def build(sc):
        fn(sc)
        for key in sc.order:
            model = sc.parts[key][0]
            new = {}
            for (col, glow), pm in model.meshes.items():
                k2 = (col, glow if glow > 0.0 else g)
                if k2 in new:
                    unreal.GeometryScript_MeshEdits.append_mesh(new[k2], pm, unreal.Transform())
                else:
                    new[k2] = pm
            model.meshes = new
    return build


TALLER = dict(grow=(60.0, 60.0, 80.0))
JOBS = [
    ('SM_P3F_SecondUnit_A', lifted(second_unit_a)),
    ('SM_P3F_SecondUnit_B', lifted(second_unit_b)),
    ('SM_P3F_DroneTeam_A', lifted(drone_team_a), TALLER),
    ('SM_P3F_DroneTeam_B', lifted(drone_team_b), TALLER),
    ('SM_P3F_StuntRig_A', lifted(stunt_rig_a)),
    ('SM_P3F_StuntRig_B', lifted(stunt_rig_b), TALLER),
    ('SM_P3F_Extras_A', lifted(extras_a)),
    ('SM_P3F_Extras_B', lifted(extras_b)),
    ('SM_P3F_MakeUp', lifted(make_up)),
    ('SM_P3F_PressPen', lifted(press_pen)),
    ('SM_P3F_CoolTent', lifted(cool_tent)),
    ('SM_P3F_Rostrum', lifted(rostrum)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
