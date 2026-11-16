"""FULL THROTTLE (Unreal) the population, level 7 City Center, pass 3, builder p3f: MORE OF THE SHOOT.
/Game/People/CityCenter/SM_P3F_*.

The last scene is being shot in Midtown at midnight, and the unit has taken the roofs: second units with their
cameras, a jib, drone teams, a wire rig, the extras before a green screen and in their holding area, the make-up
tent, the press pen, the playback monitors. Eleven meshes. Placed by C:/ftwork/p3/L7f.inl (the integrator pastes it
at the end of FTPeopleL7.inl), which also places more of the shared crew kit (/Game/People/Crew/SM_Crew*_City).

HOW A PIECE HERE IS DRAWN (the first pass's own roof convention, ft_people_city.roof_party): a roof piece stands ON a
podium's roof by a Site set whose "up" is the roof; y = 0 is the podium's middle and the roof's edge is at y = -350.
The crew's pink-and-white tape runs along the edge at y = -335 and everybody stands behind it, within 3 m of the
edge (from the street a roof's floor is not seen: what reads is what stands up over the edge, so every unit brings
something upright and steadily lit: a bounce frame, a green screen, a mirror, monitors). A piece is at most 7.4 m
long and reaches back to about y = +20, so it fits the low roofs of the shop rows (10.6 m), the pawn shop (9 m), the
service yards' strip (3.6 m deep) and leaves the first pass's parties their place.
Every roof is 25.5 m or more from the centre line, so these pieces may carry steady light (the light rule: 24 m);
nothing here blinks, chases or strobes. The drones hover (a float) or circle a mast slowly, over their own roof.
SM_P3F_LowUnit is the one LOW piece (under 2 m, no light at all): the table stands it on the ground inside the loops.
The crew wear the kit's pink chest band; their City wardrobe is given lighter coats here (the kit's own reads as
silhouettes at night). No words, no marks: stripes and plain panels only.

Run in the editor:  import ft_people_city_p3f as K; K.run()        (or K.run(['SM_P3F_PressPen']))
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/CityCenter'
PINK, WHITE, DARK, GREY, YEL, STEEL = C.PINK, C.WHITE, C.DARK, C.GREY, C.YEL, C.STEEL
WOOD, LAMP = C.WOOD, C.LAMP
CYAN, MINT, ORANGE, VIOLET, SKY, CREAM = '#2ee6ff', '#7dffb0', '#ff7a3c', '#8f6bff', '#57c7ff', '#fff1d6'
HOT = (PINK, CYAN, YEL, MINT, '#ff2d3c', ORANGE, VIOLET, WHITE, '#f4a6ff', '#b8ff5a', SKY)
G_FIG, G_PANEL, G_SCREEN, G_BULB = 0.09, 0.35, 0.6, 1.5
LEN, FRONT = 740.0, -335.0

# the kit's City wardrobe with coats that read at night (the pink band and the headsets stay)
W = dict(C.WARDROBES['City'])
W['tops'] = (WHITE, SKY, YEL, CREAM, VIOLET, GREY)
W['bottoms'] = (DARK, GREY, '#3a5fa8')
W['hats'] = (('headset', PINK), ('cap', WHITE), ('beanie', PINK), ('cap', DARK))


def lit(sc, g=G_FIG):
    """Night: every part that carries no light of its own gets a faint one (ft_people_city.lit, copied). ONLY for
    pieces the table stands 24 m or more from the road."""
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


def crew(r, **fixed):
    return C.member(r, W, **fixed)


def extra(r, **fixed):
    """One of the cast or the press: a citizen in a coat that reads at night, never over 1.72 m."""
    d = P.look(r, tops=HOT, **fixed)
    if 's' not in fixed:
        d['s'] = r.uniform(0.9, 1.0)
    return d


def fig(sc, x, y, look, **kw):
    d = dict(look)
    d.update(kw)
    d.setdefault('detail', 1)
    return P.figure(sc, x, y, **d)


def tape2(m, length=LEN, y=FRONT, posts=4):
    """The crew's pink-and-white tape, a metre high, drawn with few boxes (the kit's own is 430 triangles)."""
    for k in range(posts):
        m.box(WHITE, (-length / 2.0 + k * length / (posts - 1), y, 52), (9, 9, 104))
    n = max(int(length / 110.0), 2)
    for k in range(n):
        m.box(PINK if k % 2 else WHITE, (-length / 2.0 + (k + 0.5) * length / n, y, 90), (length / n, 4, 13))
    m.box(PINK, (0, y, 56), (length, 4, 7))


def tripod_at(m, x, y, z0=0.0, top=128.0):
    for a in (90, 210, 330):
        rod(m, STEEL, (x, y, z0 + top), (x + math.cos(math.radians(a)) * 52, y + math.sin(math.radians(a)) * 52, z0), 3.5, steps=5)
    m.cyl(DARK, (x, y, z0 + top - 6), 9, 12, steps=6)


def monitor(m, x, y, z, w=110.0, h=66.0, col=SKY, glow=G_SCREEN):
    """A monitor, its screen towards -Y (the road). Steady light."""
    m.box(DARK, (x, y, z), (w + 10, 8, h + 10))
    m.box(col, (x, y - 4.6, z), (w, 2, h), glow=glow)


def drone(m, x, y, z, s=1.0):
    """A camera drone, 1.9 m across (big, so that it reads): dark body, a pink top, four rotor discs, the camera under
    it, two small steady lamps. Never a pickup's shape or colours: no ring, no glow on the body."""
    m.box(DARK, (x, y, z), (64 * s, 64 * s, 20 * s))
    m.box(PINK, (x, y, z + 12 * s), (40 * s, 40 * s, 5 * s))
    for yaw in (45.0, -45.0):
        m.box(GREY, (x, y, z + 2 * s), (190 * s, 10 * s, 8 * s), rot=(0, yaw, 0))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.cyl(STEEL, (x + sx * 66 * s, y + sy * 66 * s, z + 7 * s), 34 * s, 5 * s, steps=8)
    m.sphere(DARK, (x, y - 8 * s, z - 20 * s), 14 * s, steps=(3, 6))
    for sx in (-1, 1):
        m.box(WHITE, (x + sx * 20 * s, y - 33 * s, z), (9 * s, 4 * s, 7 * s), glow=1.0)
        m.box(GREY, (x + sx * 30 * s, y, z - 20 * s), (6 * s, 70 * s, 6 * s))


def hold_mic():
    def f(m, h, side):
        m.box(DARK, (h[0], h[1] - 3, h[2] + 10), (5, 5, 22))
        m.sphere(PINK, (h[0], h[1] - 3, h[2] + 26), 8, steps=(3, 5))
    return f


def hold_controller():
    def f(m, h, side):
        if side > 0:
            m.box(GREY, (h[0] - 20, h[1] - 6, h[2] + 6), (40, 16, 9))
            m.box(WHITE, (h[0] - 20, h[1] - 6, h[2] + 20), (4, 4, 20))
    return f


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.6),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.0),
    dict(arm_r='wave', swing_r=24, rate=1.3, bob=3),
    dict(arm_r='point', arm_l='hip', swing_r=9, axis_r='x', rate=0.7),
    dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.4),
    dict(arm_l='wave', arm_r='hip', swing_l=22, rate=1.1, bob=2),
)


# ------------------------------------------------------------------------------------------ the pieces
def second_unit_a(sc):
    """A second unit: the A camera panning on a rostrum, a long-lens B camera, the director on a box with a megaphone,
    the clapper loader, an assistant waving the chase through; a lit bounce frame behind."""
    r = random.Random(31)
    g = sc.still()
    tape2(g)
    g.box(GREY, (-170, -190, 30), (290, 230, 60))
    g.box(YEL, (-170, -306, 55), (290, 4, 8))
    tripod_at(g, -170, -235, 60)
    pan = dict(swing=22.0, pivot=(-170.0, -235.0, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=0.1, phase=0.3, **pan))
    C.film_camera(cam, -170, -235, 188)
    fig(sc, -170, -150, crew(r, hat='headset', hat_col=PINK), z=60, arm_l='push', arm_r='push', rate=0.1, phase=0.3, sway=pan)
    # the bounce frame
    for sx in (-1, 1):
        g.box(STEEL, (230 + sx * 156, 14, 165), (7, 7, 330))
    g.box(WHITE, (230, 12, 205), (305, 5, 230), glow=G_PANEL)
    g.box(DARK, (230, 16, 205), (316, 4, 242))
    # the B camera
    tripod_at(g, 250, -240, 0)
    pan2 = dict(swing=15.0, pivot=(250.0, -240.0, 0.0), axis=P.UP)
    cam2 = sc.part(P.motion(rate=0.14, phase=0.7, **pan2))
    C.film_camera(cam2, 250, -240, 128)
    cam2.cyl(STEEL, (250, -310, 145), 11, 36, rot=(0, 0, 90), steps=8)
    fig(sc, 250, -158, crew(r), arm_l='push', arm_r='push', rate=0.14, phase=0.7, sway=pan2)
    C.apple_box(g, 60, -130)
    fig(sc, 60, -130, crew(r, hat='cap', hat_col=WHITE), z=30, yaw=-8, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12, axis_l='x',
        rate=0.5, phase=r.random())
    fig(sc, -330, -262, crew(r), yaw=24, arm_l='hold', arm_r='hold', hold_r=C.hold_clapper(), rate=0.35, bob=-1.5, phase=r.random())
    fig(sc, 30, -278, crew(r), yaw=10, arm_r='wave', swing_r=24, arm_l='hip', rate=1.2, bob=3, react=True, phase=r.random())
    C.case(g, 110, -40, w=64, d=40, h=36, col=YEL)
    lit(sc)


def second_unit_b(sc):
    """A second unit with a jib: its arm rises and falls, the camera at its tip; a grip at the weights, the operator at
    a monitor, a spark at a lamp, the script supervisor."""
    r = random.Random(37)
    g = sc.still()
    tape2(g)
    g.box(GREY, (-110, -180, 20), (150, 150, 40))
    g.box(YEL, (-110, -180, 132), (22, 22, 184))
    g.box(DARK, (-110, -180, 230), (40, 40, 16))
    swing = dict(swing=10.0, pivot=(-110.0, -180.0, 236.0), axis=90.0)
    arm = sc.part(P.motion(rate=0.08, phase=0.1, **swing))
    tilt_box(arm, YEL, (-270, -180, 204), (210, -180, 296), 14, 18)
    arm.box(DARK, (-276, -180, 196), (56, 46, 56))
    arm.box(DARK, (212, -180, 280), (26, 26, 36))
    C.film_camera(arm, 212, -186, 196)
    fig(sc, -345, -165, crew(r), yaw=90, arm_l='push', arm_r='push', rock=3, rate=0.08, phase=0.1)
    # the operator's monitor on a stand, and the operator
    g.box(STEEL, (40, -40, 60), (6, 6, 120))
    monitor(g, 40, -44, 140, w=70, h=44)
    fig(sc, 20, -250, crew(r, hat='headset', hat_col=PINK), yaw=176, arm_l='hold', arm_r='hold', rate=0.3, bob=-1.0, phase=r.random())
    C.light_stand(g, 300, -30, height=300, yaw=-18, glow=0.8)
    fig(sc, 250, -120, crew(r), yaw=120, arm_r='up', arm_l='up', swing_r=6, swing_l=-4, axis_r='x', axis_l='x', rate=0.5, phase=r.random())
    fig(sc, 150, -268, crew(r), yaw=-20, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.2, phase=r.random())
    fig(sc, -230, -272, crew(r), yaw=14, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.6, react=True, phase=r.random())
    C.case(g, -20, -90)
    lit(sc)


def drone_team_a(sc):
    """A drone team: the drone hovers over its pad (a float, and a slow drift along the roof), the pilot at the
    sticks, a spotter pointing up at it, a technician on a case with a tablet, a runner with a crate; a wind sock."""
    r = random.Random(41)
    g = sc.still()
    tape2(g)
    g.box(GREY, (170, -165, 6), (250, 250, 12))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(PINK, (170 + sx * 110, -165 + sy * 110, 13), (30, 30, 4), glow=0.5)
    d = sc.part(P.motion(bob=-22.0, rate=0.3, phase=0.1, swing=4.0, pivot=(170.0, -165.0, -500.0), axis=90.0))
    drone(d, 170, -165, 345)
    fig(sc, -30, -240, crew(r, hat='headset', hat_col=PINK), yaw=-40, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.0, phase=r.random())
    fig(sc, -150, -262, crew(r), yaw=-30, arm_r=((150, 30), (158, 20)), swing_r=8, axis_r='x', arm_l='hip', rate=0.5, phase=r.random())
    C.case(g, -265, -120, w=70, d=44, h=40, col=YEL)
    fig(sc, -265, -120, crew(r), z=-6, yaw=20, legs='sit', seat=40, arm_l='hold', arm_r='hold', hold_r=P.hold_phone(glow=0.6), rate=0.25, bob=-0.8,
        phase=r.random())
    fig(sc, 20, -90, crew(r), yaw=-150, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=GREY), rate=0.3, bob=-1.0, phase=r.random())
    # the wind sock on its mast
    g.box(STEEL, (-335, -40, 170), (7, 7, 340))
    sock = sc.part(P.motion(swing=18.0, pivot=(-335.0, -40.0, 330.0), axis=P.UP, rate=0.4, phase=0.2))
    tilt_box(sock, ORANGE, (-335, -40, 330), (-262, -40, 320), 24, 24)
    tilt_box(sock, WHITE, (-262, -40, 320), (-232, -40, 316), 18, 18)
    C.case(g, 320, -30, col=GREY)
    lit(sc)


def drone_team_b(sc):
    """Two drones hovering either side of a mast, one above the other (they bob; they no longer circle: review round); the two
    pilots, a monitor cart with its screens to the street, a crew member waving the chase on."""
    r = random.Random(43)
    g = sc.still()
    tape2(g)
    cx, cy = 60.0, -150.0
    g.box(STEEL, (cx, cy, 185), (8, 8, 370))
    g.box(DARK, (cx, cy, 4), (60, 60, 8))
    g.box(LAMP, (cx, cy, 376), (16, 16, 12), glow=1.0)
    for k, sx in enumerate((1, -1)):
        d = sc.part(P.motion(bob=-14.0, rate=0.3, phase=0.5 * k))
        drone(d, cx + sx * 140, cy, 300 + k * 70, s=0.9)
    # the monitor cart
    g.box(GREY, (-250, -40, 45), (150, 60, 90))
    monitor(g, -288, -46, 130, w=64, h=44)
    monitor(g, -212, -46, 130, w=64, h=44, col=MINT)
    fig(sc, -250, -150, crew(r, hat='headset', hat_col=PINK), yaw=180, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rate=0.3, bob=-1.0)
    fig(sc, -130, -250, crew(r), yaw=60, arm_l='hold', arm_r='hold', hold_r=hold_controller(), rock=4, rate=0.25, phase=r.random())
    fig(sc, 250, -260, crew(r), yaw=-70, arm_r=((150, 30), (158, 20)), swing_r=8, axis_r='x', arm_l='hip', rate=0.5, phase=r.random())
    fig(sc, 320, -150, crew(r), yaw=10, arm_r='wave', swing_r=24, arm_l='down', rate=1.2, bob=3, react=True, phase=r.random())
    C.case(g, 300, -30, col=YEL)
    lit(sc)


def stunt_rig(sc):
    """Stunt rigging: a truss goalpost, a performer in a harness swinging on the wire over a crash mat, the rigger on
    the line, the coordinator, a second performer warming up, a work lamp."""
    r = random.Random(47)
    g = sc.still()
    tape2(g)
    for sx in (-1, 1):
        g.box(YEL, (sx * 250, -130, 212), (16, 16, 424))
        g.box(DARK, (sx * 250, -130, 5), (70, 110, 10))
        tilt_box(g, YEL, (sx * 250, -130, 300), (sx * 150, -130, 420), 9, 9)
    g.box(YEL, (0, -130, 424), (516, 16, 16))
    g.box('#2e6bff', (0, -130, 22), (300, 190, 44))
    g.box(SKY, (0, -130, 46), (240, 140, 5))
    sw = dict(swing=13.0, pivot=(0.0, -130.0, 420.0), axis=90.0)
    wire = sc.part(P.motion(rate=0.3, phase=0.0, **sw))
    wire.box(STEEL, (0, -118, 345), (5, 5, 150))
    look = extra(r, top=ORANGE, bottom=ORANGE, hat='helmet', hat_col=WHITE, sleeves='long', wear='trousers', s=0.95)
    fig(sc, 0, -130, look, z=150, legs='wide', arm_l='out', arm_r='out', rate=0.3, phase=0.0, sway=sw)
    fig(sc, 330, -225, crew(r), yaw=-70, arm_l='up', arm_r='up', swing_l=8, swing_r=-8, axis_l='x', axis_r='x', rate=0.3, phase=0.0)
    fig(sc, -330, -262, crew(r, hat='cap', hat_col=WHITE), yaw=30, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(),
        rate=0.6, phase=r.random())
    fig(sc, -165, -275, extra(r, top='#b8ff5a', bottom=DARK, hat='helmet', hat_col='#ff2d3c', sleeves='long', wear='trousers'), yaw=-10,
        arm_l='vee', arm_r='vee', bob=9, rate=1.1, phase=0.4)
    fig(sc, 170, -280, crew(r), yaw=12, arm_r='wave', swing_r=22, arm_l='hip', rate=1.2, bob=3, react=True, phase=r.random())
    C.light_stand(g, -335, -20, height=320, yaw=24, glow=0.8)
    lit(sc)


def extras_a(sc):
    """The extras before the green screen: eight of the cast in two rows (the back row on a riser) cheering the
    chase on, an assistant director with a megaphone keeping them in their place."""
    r = random.Random(53)
    g = sc.still()
    tape2(g)
    g.box(MINT, (-30, 16, 190), (600, 6, 300), glow=0.3)
    for sx in (-1, 1):
        g.box(STEEL, (-30 + sx * 306, 16, 176), (8, 8, 352))
        tilt_box(g, STEEL, (-30 + sx * 306, 16, 250), (-30 + sx * 306, 110, 0), 6, 6)
    g.box(STEEL, (-30, 16, 346), (620, 8, 8))
    g.box(GREY, (-30, -125, 20), (590, 90, 40))
    wears = ('dress', 'coat', 'skirt', 'trousers')
    hats = (None, 'sun', None, 'cowboy', None, 'cap')
    for i in range(4):
        d = extra(r, wear=wears[i % 4], hat=hats[(i * 2) % 6], hat_col=r.choice(HOT))
        d.update(CHEER[(i * 2 + 1) % len(CHEER)])
        fig(sc, -255 + i * 150 + r.uniform(-10, 10), -238 + r.uniform(-8, 8), d, yaw=r.uniform(-14, 14), phase=r.random(), react=True)
    for i in range(4):
        d = extra(r, wear=wears[(i + 2) % 4], hat=hats[(i * 2 + 3) % 6], hat_col=r.choice(HOT))
        d.update(CHEER[(i * 2) % len(CHEER)])
        fig(sc, -235 + i * 140 + r.uniform(-10, 10), -125, d, z=40, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=0)
    fig(sc, 330, -262, crew(r, hat='headset', hat_col=PINK), yaw=-28, arm_r='face', hold_r=C.hold_megaphone(), arm_l='up', swing_l=12, rate=0.6,
        phase=r.random())
    lit(sc)


def extras_b(sc):
    """The extras' holding area: a striped wind break, three of the cast on a bench (a cup, a telephone, a wave), two
    rehearsing a dance step, the costume rail, the wrangler with a clipboard."""
    r = random.Random(59)
    g = sc.still()
    tape2(g)
    for k in range(6):
        g.box(PINK if k % 2 else WHITE, (-250 + k * 100, 14, 112), (100, 5, 200), glow=0.15)
    for k in range(3):
        g.box(STEEL, (-300 + k * 300, 20, 112), (7, 7, 224))
    # the bench and its three
    g.box(WOOD, (-40, -70, 46), (330, 44, 8))
    for sx in (-1, 1):
        g.box(WOOD, (-40 + sx * 150, -70, 21), (8, 40, 42))
    sit = (dict(arm_l='hold', arm_r='face', hold_r=P.hold_cup(), rate=0.3, bob=-0.8),
           dict(arm_l='hold', arm_r='hold', hold_r=P.hold_phone(glow=0.6), rate=0.25, bob=-0.8),
           dict(arm_r='wave', swing_r=22, arm_l='down', rate=1.2, react=True))
    for i in range(3):
        d = extra(r, wear=('dress', 'trousers', 'coat')[i], hat=(None, 'sun', None)[i], hat_col=YEL)
        d.update(sit[i])
        fig(sc, -150 + i * 110, -70, d, legs='sit', seat=46, yaw=r.uniform(-10, 10), phase=r.random())
    # the costume rail
    for sx in (-1, 1):
        g.box(STEEL, (280 + sx * 70, -60, 88), (6, 6, 176))
    g.box(STEEL, (280, -60, 176), (150, 6, 6))
    for k in range(5):
        g.box(HOT[(k * 3 + 1) % len(HOT)], (224 + k * 28, -60, 126), (22, 34, 92))
    # two rehearsing a step, turning opposite ways
    fig(sc, -300, -235, extra(r, wear='dress', hair='long'), yaw=60, arm_l='out', arm_r='hip', turn=28, rate=0.5, phase=0.0)
    fig(sc, -215, -250, extra(r, wear='coat', hat='cowboy', hat_col=CREAM), yaw=-110, arm_l='hip', arm_r='out', turn=28, rate=0.5, phase=0.5)
    fig(sc, 130, -262, crew(r), yaw=-15, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=C.hold_clipboard(), rate=0.6, phase=r.random())
    fig(sc, 310, -258, extra(r, wear='skirt', hat='cap', hat_col=PINK), yaw=18, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.5,
        react=True, phase=r.random())
    lit(sc)


def make_up(sc):
    """The make-up tent: a pink-and-white canopy, the mirror with its bulbs (its lit side to the street), an actor in
    the chair, the make-up artist and the hairdresser at work, an actor waiting with a cup, a runner with a crate."""
    r = random.Random(61)
    g = sc.still()
    tape2(g)
    for sx in (-1, 1):
        for y in (-285, -25):
            g.box(STEEL, (sx * 175, y, 125), (7, 7, 250))
    for k in range(5):
        g.box(PINK if k % 2 else WHITE, (-140 + k * 70, -155, 255), (70, 280, 9))
    g.box(WHITE, (0, 0, 40), (210, 40, 80))
    g.box(DARK, (0, 4, 160), (170, 6, 124))
    g.box('#bfe9ff', (0, 0, 160), (140, 3, 96), glow=0.3)
    for k in range(4):
        for z in (104, 216):
            g.box(LAMP, (-60 + k * 40, -1, z), (12, 6, 12), glow=G_BULB)
    C.chair(g, 0, -120, yaw=180)
    fig(sc, 0, -120, extra(r, wear='dress', hair='long', top=PINK), z=6, yaw=180, legs='sit', seat=72, arm_l='hold', arm_r='hold', rate=0.25, bob=-0.8)
    fig(sc, 78, -105, crew(r), yaw=105, arm_r='fwd', swing_r=10, axis_r='x', arm_l='hold', rate=1.2, phase=r.random())
    fig(sc, -78, -100, crew(r, hat='beanie', hat_col=PINK), yaw=-105, arm_l='face', arm_r='fwd', swing_l=9, axis_l='x', rate=0.9, phase=r.random())
    fig(sc, -270, -255, extra(r, wear='coat', hat='cowboy', hat_col=CREAM, top=YEL), yaw=12, arm_r='face', hold_r=P.hold_cup(), arm_l='hip', rock=4,
        rate=0.35, phase=r.random())
    fig(sc, 285, -250, crew(r), yaw=-25, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=YEL), rate=0.3, bob=-1.0, phase=r.random())
    C.case(g, 300, -90, col=GREY)
    g.box(WOOD, (-290, -80, 23), (40, 40, 46))
    lit(sc)


def press_pen(sc):
    """The press pen: a television camera panning on its tripod, a reporter at the microphone, three photographers
    following the chase with their cameras, one more on a step ladder, a dish on a mast. No flashes."""
    r = random.Random(67)
    g = sc.still()
    tape2(g)
    g.box(GREY, (0, -190, 15), (720, 230, 30))
    g.box(CYAN, (0, -306, 27), (720, 4, 6))
    def press(**kw):
        d = dict(band=CYAN, sleeves='long', wear='trousers')
        d.update(kw)
        return extra(r, **d)
    tripod_at(g, -235, -262, 30)
    pan = dict(swing=25.0, pivot=(-235.0, -262.0, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=0.12, phase=0.4, **pan))
    C.film_camera(cam, -235, -262, 158)
    fig(sc, -235, -180, press(hat='cap', hat_col=CYAN), z=30, arm_l='push', arm_r='push', rate=0.12, phase=0.4, sway=pan)
    fig(sc, -110, -270, press(wear='coat', hair='long'), z=30, yaw=30, arm_r='face', hold_r=hold_mic(), arm_l='point', swing_l=12, axis_l='x', rate=0.7,
        phase=r.random())
    for i, (x, y) in enumerate(((20, -268), (115, -250), (210, -270))):
        fig(sc, x, y, press(hat=(None, 'cap', 'beanie')[i], hat_col=HOT[i * 3]), z=30, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=28,
            rate=0.25 + 0.07 * i, phase=r.random(), react=True)
    # the step ladder and the one who climbed it
    for sy in (-1, 1):
        tilt_box(g, STEEL, (320, -170 + sy * 40, 30), (320, -170, 150), 46, 6)
    g.box(STEEL, (320, -170, 122), (50, 40, 6))
    fig(sc, 320, -170, press(hat='cap', hat_col=YEL), z=125, yaw=-12, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=22, rate=0.3,
        phase=r.random())
    # the dish
    g.box(STEEL, (-335, -40, 150), (8, 8, 300))
    g.sphere(WHITE, (-335, -52, 310), 62, scale=(1.0, 0.3, 1.0), steps=(4, 8))
    g.box(CYAN, (-335, -74, 310), (8, 8, 8), glow=1.0)
    C.case(g, 60, -110, col=GREY)
    lit(sc)


def playback(sc):
    """Playback: three monitors under a hood with their screens to the street (steady light), the director and the
    producer in tall chairs before them, the script supervisor, the technician at the cart, a runner going to and
    fro along the tape."""
    r = random.Random(71)
    g = sc.still()
    tape2(g)
    for sx in (-1, 1):
        g.box(STEEL, (sx * 215, -6, 105), (7, 7, 210))
    g.box(DARK, (0, -10, 214), (450, 70, 6))
    g.box(DARK, (0, -2, 108), (440, 5, 20))
    for k, col in enumerate((SKY, LAMP, MINT)):
        monitor(g, -140 + k * 140, -10, 160, w=118, h=72, col=col)
    C.chair(g, -75, -150, yaw=180)
    C.chair(g, 75, -150, yaw=180)
    fig(sc, -75, -150, crew(r, hat='cap', hat_col=WHITE), z=6, yaw=180, legs='sit', seat=72, arm_r='point', swing_r=9, axis_r='x', arm_l='down',
        rate=0.45, phase=0.3)
    fig(sc, 75, -150, crew(r), z=6, yaw=170, legs='sit', seat=72, arm_l='hold', arm_r='hold', hold_r=P.hold_cup(), rate=0.25, bob=-0.8)
    fig(sc, -250, -150, crew(r), yaw=150, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.2, phase=0.5)
    g.box(GREY, (305, -60, 45), (100, 60, 90))
    g.box(DARK, (305, -70, 104), (50, 4, 30))
    g.box(SKY, (305, -73, 104), (44, 2, 24), glow=0.5)
    fig(sc, 305, -150, crew(r, hat='headset', hat_col=PINK), yaw=180, arm_l='hold', arm_r='hold', rate=0.3, bob=-1.0, phase=r.random())
    fig(sc, 0, -278, crew(r, pack=PINK), yaw=90, stride=22, swing_l=16, swing_r=-16, axis_l='x', axis_r='x', rate=1.0, bob=2, phase=r.random(),
        travel=(640, 70))
    lit(sc)


def low_unit(sc):
    """LOW (under 2 m, no light): a ground unit behind its tape. A camera on a low tripod, its operator sitting on a
    box behind it, a photographer on a case with a long lens, another standing, the assistant pointing the chase
    out. Stood on the ground inside the loops by the table, where nothing lit or tall may be."""
    r = random.Random(73)
    g = sc.still()
    tape2(g, 640.0, -150.0)
    tripod_at(g, -60, -70, 0, top=88)
    pan = dict(swing=20.0, pivot=(-60.0, -70.0, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=0.11, phase=0.2, **pan))
    C.film_camera(cam, -60, -70, 88)
    C.apple_box(g, -60, 20)
    fig(sc, -60, 20, crew(r, hat='headset', hat_col=PINK), z=30, legs='sit', seat=46, arm_l='push', arm_r='push', rate=0.11, phase=0.2, sway=pan, detail=2)
    C.case(g, 110, -40, w=70, d=44, h=40, col=YEL)
    fig(sc, 110, -40, crew(r, hat='cap', hat_col=WHITE), z=-6, yaw=-14, legs='sit', seat=40, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=20,
        rate=0.3, phase=r.random(), detail=2)
    fig(sc, 235, -60, crew(r, hat='cap', hat_col=DARK), yaw=16, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=24, rate=0.35, phase=r.random(),
        react=True, detail=2)
    fig(sc, -230, -60, crew(r, hat='cap', hat_col=WHITE), yaw=-20, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.6, react=True,
        phase=r.random(), detail=2)
    C.case(g, -290, 40, col=GREY)


JOBS = [
    ('SM_P3F_SecondUnit_A', second_unit_a),
    ('SM_P3F_SecondUnit_B', second_unit_b, dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3F_DroneTeam_A', drone_team_a, dict(grow=(90.0, 60.0, 80.0), grow_down=(90.0, 60.0, 0.0))),
    ('SM_P3F_DroneTeam_B', drone_team_b, dict(grow=(180.0, 180.0, 80.0), grow_down=(180.0, 180.0, 0.0))),
    ('SM_P3F_StuntRig', stunt_rig, dict(grow=(100.0, 60.0, 60.0), grow_down=(100.0, 60.0, 0.0))),
    ('SM_P3F_Extras_A', extras_a),
    ('SM_P3F_Extras_B', extras_b),
    ('SM_P3F_MakeUp', make_up),
    ('SM_P3F_PressPen', press_pen),
    ('SM_P3F_Playback', playback),
    ('SM_P3F_LowUnit', low_unit),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
