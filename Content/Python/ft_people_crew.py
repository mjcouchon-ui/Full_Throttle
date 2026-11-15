"""FULL THROTTLE (Unreal) the population: the film crew, on every level (/Game/People/Crew/).

In the fiction every level is a film shoot, so every level has its crew: a camera position, a crane, a dolly on its
rails, lighting, sound, the director's village, the stunt team with its crash mats, crew applauding at the barrier,
catering. One kit, dressed for each level's weather (a WARDROBE): the meshes are SM_Crew<Piece>_<Wardrobe>, e.g.
/Game/People/Crew/SM_CrewCamera_Sun. What marks the crew out on any level: the pink band across the chest, the
headsets, the yellow-and-dark equipment, the pink-and-white tape of their barrier.

    level 0 Bayshore  Sun      1 Chemical Works  Work     2 Nevada  Desert     3 Volcano  Heat
    level 4 Theme Park  Park   5 Ice Mountain  Snow       6 Under the Sea  Sea  7 City Center  City

The pieces, and where a table may put them (the sight rule: over 2 m tall = 24 m or more from the centre line):
    SM_CrewCamera     LOW   a film camera on its tripod panning, its operator, a focus puller, the director in a chair
                            with a megaphone, the clapper loader, the first assistant pointing. Behind crew tape.
    SM_CrewApplauseA/B LOW  crew at the tape: clapping, cheering, pointing, filming on a phone. They liven up as the
                            chase comes by.
    SM_CrewStunt      LOW   the stunt team: crash mats, an air bag, two performers warming up, the coordinator.
    SM_CrewVillage    LOW   the director's village: tall chairs, the monitors under their hood, the script supervisor.
    SM_CrewDolly      LOW   a camera dolly sliding to and fro on its rails, the operator riding, a grip pushing.
    SM_CrewCrane      TALL  a camera crane, its arm rising and falling, a grip at the base.
    SM_CrewLights     TALL  two lighting stands (steady light: 24 m or more), a reflector, the generator, a spark.
    SM_CrewSound      TALL  the boom operator, boom overhead, and the recordist at the sound cart.
    SM_CrewCatering   TALL  the catering table under its canopy: a cook, a queue with cups, one sitting down to eat.

Run in the editor:  import ft_people_crew; ft_people_crew.run()          (every wardrobe)
                    ft_people_crew.run(['Sun'], ['SM_CrewCamera'])        (one wardrobe, one piece)
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/Crew'
PINK, WHITE, DARK, GREY, YEL, STEEL = '#ff5fa2', '#fff6ea', '#22202a', '#3a3848', '#ffc93c', '#8f9bb3'
WOOD, CANVAS, LAMP = '#c9a070', '#2b2350', '#fff1c0'

WARDROBES = {
    'Sun': dict(tops=(DARK, GREY, WHITE, '#2ee6ff'), bottoms=('#f1e4c8', '#3a5fa8', '#5a4a3c'), sleeves='short', wear=('shorts', 'trousers'),
                hats=(('cap', DARK), ('sun', '#f1e4c8'), (None, DARK), ('headset', DARK)), band=PINK),
    'Work': dict(tops=('#ff7a3c', YEL), bottoms=('#2b2350', DARK), sleeves='long', wear=('trousers',),
                 hats=(('hard', YEL), ('hard', WHITE)), band='#b8ff5a'),
    'Desert': dict(tops=('#f1e4c8', WOOD, WHITE, '#7a8fb8'), bottoms=('#5a4a3c', '#3a5fa8'), sleeves='short', wear=('trousers',),
                   hats=(('cowboy', '#8e5a3a'), ('sun', '#f1e4c8'), ('cap', '#c94f2e')), band=PINK),
    'Heat': dict(tops=('#c9c4d6', '#d7e3f0'), bottoms=('#c9c4d6', '#a39cc2'), sleeves='long', wear=('trousers',),
                 hats=(('hard', '#ff7a3c'), ('helmet', '#c9c4d6')), band='#ff7a3c'),
    'Park': dict(tops=(PINK, '#2ee6ff', YEL, '#7dffb0'), bottoms=('#2b2350', WHITE), sleeves='short', wear=('shorts', 'trousers'),
                 hats=(('cap', '#ff2d3c'), ('cap', '#2ee6ff'), ('headset', DARK), (None, DARK)), band=WHITE),
    'Snow': dict(tops=('#ff2d3c', '#2e6bff', YEL, '#7dffb0', '#8f6bff'), bottoms=(DARK, '#2b2350'), sleeves='long', wear=('coat',),
                 hats=(('beanie', '#ff2d3c'), ('beanie', '#2e6bff'), ('beanie', YEL)), band=PINK),
    'Sea': dict(tops=(YEL, '#ff7a3c'), bottoms=(YEL, '#ff7a3c'), sleeves='long', wear=('trousers',),
                hats=(('helmet', '#d7e3f0'),), band='#2ee6ff', pack=STEEL),
    'City': dict(tops=(DARK, '#2b2350', GREY), bottoms=(DARK, GREY), sleeves='long', wear=('trousers',),
                 hats=(('headset', PINK), ('cap', DARK), ('beanie', DARK)), band=PINK),
}
LEVEL_WARDROBE = ('Sun', 'Work', 'Desert', 'Heat', 'Park', 'Snow', 'Sea', 'City')


def member(r, w, **fixed):
    """A crew member's look out of a wardrobe (figure() values)."""
    hat, hat_col = r.choice(w['hats'])
    top = r.choice(w['tops'])
    d = P.look(r, tops=w['tops'], bottoms=w['bottoms'], sleeves=w['sleeves'], wear=r.choice(w['wear']), hat=hat, hat_col=hat_col,
               band=w['band'], pack=w.get('pack'))
    d['top'] = top
    if w.get('pack'):           # a suit: one colour head to foot
        d['bottom'] = top
    d['s'] = r.uniform(0.92, 1.0)
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ equipment
def tape(m, length, y, posts=4):
    """The crew's barrier: posts and two runs of pink-and-white tape, a metre high. People by the road stand BEHIND it."""
    for k in range(posts):
        x = -length / 2.0 + k * length / (posts - 1)
        m.box(WHITE, (x, y, 52), (9, 9, 104))
        m.box(DARK, (x, y, 4), (30, 30, 8))
    n = max(int(length / 60.0), 2)
    for k in range(n):
        x = -length / 2.0 + (k + 0.5) * length / n
        m.box(PINK if k % 2 else WHITE, (x, y, 92), (length / n, 3, 11))
        m.box(WHITE if k % 2 else PINK, (x, y, 58), (length / n, 3, 7))


def film_camera(m, x, y, z, yaw=0.0):
    """A film camera, its lens towards -Y at yaw 0: the body, the twin magazine on top, the lens and its matte box.
    (x, y, z) is the middle of the camera's base. Draw it into a model whose frame is the mesh's own."""
    old = m.base
    m.base = P.ft_art._xf((x, y, z), (0.0, yaw, 0.0), (1, 1, 1))
    m.box(DARK, (0, 0, 17), (30, 62, 34))
    m.cyl(GREY, (0, 14, 50), 17, 12, rot=(0, 0, 90), steps=10)
    m.cyl(GREY, (0, -16, 50), 17, 12, rot=(0, 0, 90), steps=10)
    m.cyl(STEEL, (0, -31, 17), 9, 22, rot=(0, 0, 90), steps=8)
    m.box(DARK, (0, -58, 17), (34, 10, 30))
    m.box(YEL, (16, 4, 20), (3, 36, 12))
    m.box(DARK, (-20, 20, 26), (10, 26, 10))
    m.base = old


def tripod(m, x, y, top=128.0):
    for a in (90, 210, 330):
        rod(m, STEEL, (x, y, top), (x + math.cos(math.radians(a)) * 52, y + math.sin(math.radians(a)) * 52, 0), 3.5, steps=5)
    m.cyl(DARK, (x, y, top - 6), 9, 12, steps=6)


def case(m, x, y, z=0.0, w=70.0, d=44.0, h=38.0, col=GREY):
    m.box(col, (x, y, z + h / 2.0), (w, d, h))
    m.box(STEEL, (x, y, z + h * 0.72), (w + 2, d + 2, 4))


def apple_box(m, x, y, z=0.0):
    m.box(WOOD, (x, y, z + 15), (50, 34, 30))


def chair(m, x, y, yaw=0.0, seat=72.0):
    """A director's chair: crossed legs, a canvas seat and back. Its sitter faces -Y at yaw 0."""
    old = m.base
    m.base = P.ft_art._xf((x, y, 0), (0.0, yaw, 0.0), (1, 1, 1))
    for sx in (-1, 1):
        tilt_box(m, WOOD, (sx * 26, -22, 0), (sx * 26, 22, seat), 5, 5)
        tilt_box(m, WOOD, (sx * 26, 22, 0), (sx * 26, -22, seat), 5, 5)
        m.box(WOOD, (sx * 26, 24, seat + 30), (5, 5, 62))
        m.box(WOOD, (sx * 26, 0, seat + 22), (6, 50, 5))
    m.box(CANVAS, (0, 0, seat), (54, 46, 5))
    m.box(CANVAS, (0, 25, seat + 46), (54, 4, 24))
    m.base = old


def hold_megaphone(col=WHITE, trim='#ff2d3c'):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 6, h[2] + 6), 6, 30, rot=(0, 0, 90), steps=8, top_r=15)
        m.cyl(trim, (h[0], h[1] - 34, h[2] + 6), 15, 4, rot=(0, 0, 90), steps=8)
    return f


def hold_clapper():
    """The clapperboard: a dark slate, the striped sticks across its top (never words)."""
    def f(m, h, side):
        if side < 0:
            return
        cx = h[0] - 22
        m.box(DARK, (cx, h[1] - 5, h[2] + 14), (50, 4, 36))
        for k in range(5):
            m.box(WHITE if k % 2 else DARK, (cx - 20 + k * 10, h[1] - 5, h[2] + 36), (10, 5, 8))
        for k in range(3):
            m.box(WHITE, (cx, h[1] - 7.4, h[2] + 4 + k * 9), (40, 1, 2))
    return f


def hold_clipboard():
    def f(m, h, side):
        m.box(WOOD, (h[0] - side * 6, h[1] - 6, h[2] + 12), (24, 3, 32))
        m.box(WHITE, (h[0] - side * 6, h[1] - 7.8, h[2] + 10), (20, 1, 25))
    return f


def hold_boom(length=330.0):
    """A sound boom held overhead: the pole out over the scene, the furry microphone at its tip."""
    def f(m, h, side):
        if side < 0:
            return
        a = (h[0] - 26, h[1] + 70, h[2] + 4)
        b = (h[0] - 26, h[1] - length, h[2] + 26)
        tilt_box(m, DARK, a, b, 4, 4)
        m.cyl(STEEL, (b[0], b[1] + 16, b[2] - 9), 9, 34, rot=(0, 0, 90), steps=7)
    return f


def light_stand(m, x, y, height=330.0, yaw=0.0, glow=0.8):
    tripod(m, x, y, top=90.0)
    m.box(STEEL, (x, y, height / 2.0 + 30), (6, 6, height - 60))
    old = m.base
    m.base = P.ft_art._xf((x, y, height), (0.0, yaw, 0.0), (1, 1, 1))
    m.box(DARK, (0, 4, 0), (66, 40, 60))
    m.box(LAMP, (0, -17, 0), (56, 3, 50), glow=glow)
    for sx in (-1, 1):
        tilt_box(m, DARK, (sx * 33, -18, 0), (sx * 52, -44, 0), 3, 56)
    tilt_box(m, DARK, (0, -18, 30), (0, -42, 46), 62, 3)
    m.base = old


# ------------------------------------------------------------------------------------------ the pieces
def camera(w, seed=1):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        tape(g, 620, -150, posts=5)
        tripod(g, 0, -40)
        # the camera pans, and its operator with it, round the tripod
        pan = dict(swing=20.0, pivot=(0.0, -40.0, 0.0), axis=P.UP)
        cam = sc.part(P.motion(rate=0.11, phase=0.2, **pan))
        film_camera(cam, 0, -40, 128)
        P.figure(sc, 0, 42, arm_l='push', arm_r='push', rate=0.11, phase=0.2, sway=pan, **member(r, w, hat='headset', hat_col=DARK))
        P.figure(sc, 78, -6, yaw=-38, arm_l='fwd', **member(r, w))                                   # the focus puller
        chair(g, -200, 30, yaw=12)
        P.figure(sc, -200, 30, z=6, yaw=12, legs='sit', seat=72, arm_r='face', hold_r=hold_megaphone(), arm_l='point', swing_l=10, axis_l='x',
                 rate=0.5, phase=r.random(), **member(r, w, hat='cap', hat_col=DARK))                # the director
        P.figure(sc, 190, -84, yaw=18, arm_l='hold', arm_r='hold', hold_r=hold_clapper(), rate=0.35, bob=-1.5, phase=r.random(), **member(r, w))
        P.figure(sc, -92, -78, yaw=-22, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), **member(r, w))
        case(g, 250, 40)
        case(g, 262, 36, z=38, w=52, h=30, col=YEL)
        apple_box(g, -120, 50)
        g.box(DARK, (60, 60, 2), (240, 6, 4))
    return build


def applause(w, seed=3):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        tape(g, 760, -70, posts=6)
        acts = (
            dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.6),
            dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.6),          # clapping
            dict(arm_r='point', arm_l='hip', swing_r=9, axis_r='x', rate=0.7),
            dict(arm_r='face', hold_r=P.hold_phone(), arm_l='down', rate=0.3, bob=-1.0),
            dict(arm_r='wave', swing_r=24, rate=1.3, bob=3),
            dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.4),
        )
        n = 7
        for i in range(n):
            x = -330 + i * 110 + r.uniform(-14, 14)
            y = (10 if i % 2 else 55) + r.uniform(-8, 8)
            d = member(r, w)
            d.update(acts[(i * 5 + seed) % len(acts)])
            P.figure(sc, x, y, yaw=r.uniform(-16, 16), phase=r.random(), react=True, **d)
        apple_box(g, 300, 110)
    return build


def stunt(w, seed=5):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        tape(g, 760, -190, posts=6)
        # the air bag, breathing on its fan; the stack of crash mats
        bag = sc.part(P.motion(bob=-5.0, rate=0.3))
        bag.box('#2e6bff', (-170, 30, 72), (400, 300, 144))
        bag.box('#57c7ff', (-170, 30, 147), (330, 240, 8))
        bag.box(WHITE, (-170, 30, 152), (120, 120, 3))
        g.box(GREY, (-400, 150, 30), (60, 60, 60))
        for k, col in enumerate(('#3a5fa8', '#ff2d3c', '#3a5fa8', '#ff2d3c')):
            g.box(col, (210 + (k % 2) * 8, 80, 14 + k * 28), (230, 150, 26))
        look = dict(hat='helmet', sleeves='long', wear='trousers', band=None)
        P.figure(sc, 170, -70, yaw=-12, bob=11, rate=1.2, arm_l='vee', arm_r='vee', phase=0.1, **member(r, w, top='#ff7a3c', bottom='#ff7a3c', hat_col=WHITE, **look))
        P.figure(sc, 290, -90, yaw=20, rock=11, rate=0.5, arm_l='hip', arm_r='up', phase=0.6, **member(r, w, top='#b8ff5a', bottom=DARK, hat_col='#ff2d3c', **look))
        P.figure(sc, 40, -110, yaw=-30, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=hold_clipboard(), rate=0.6, **member(r, w))
        # a safety officer with an extinguisher at their feet
        P.figure(sc, 380, 30, yaw=30, arm_l='hip', arm_r='down', rate=0.3, bob=-1.0, **member(r, w))
        g.cyl('#ff2d3c', (350, 6, 0), 11, 46, steps=8)
        g.cyl(DARK, (350, 6, 46), 5, 10, steps=6)
    return build


def village(w, seed=7):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        tape(g, 560, -170, posts=5)
        # the monitors under their hood, their backs to the road
        for sx in (-1, 1):
            g.box(STEEL, (sx * 52, -70, 52), (5, 5, 104))
        g.box(DARK, (0, -70, 108), (124, 44, 8))
        for sx in (-1, 1):
            g.box(DARK, (sx * 30, -66, 132), (52, 8, 40))
            g.box('#3a5fa8', (sx * 30, -61.5, 132), (44, 1.5, 32))
        g.box(DARK, (0, -72, 156), (130, 54, 4))
        for sx in (-1, 1):
            g.box(DARK, (sx * 64, -72, 134), (3, 54, 44))
        chair(g, -70, 40)
        chair(g, 60, 46, yaw=-10)
        P.figure(sc, -70, 40, z=6, legs='sit', seat=72, arm_r='point', swing_r=9, axis_r='x', arm_l='down', rate=0.45, phase=0.3,
                 **member(r, w, hat='cap', hat_col=DARK))
        P.figure(sc, 60, 46, z=6, yaw=-10, legs='sit', seat=72, arm_l='hold', arm_r='hold', hold_r=P.hold_cup(), rate=0.25, bob=-0.8, **member(r, w))
        P.figure(sc, 170, -20, yaw=-35, arm_l='hold', arm_r='hold', hold_l=hold_clipboard(), rate=0.3, bob=-1.2, phase=0.5, **member(r, w))
        P.figure(sc, -200, 10, yaw=40, arm_r='face', hold_r=P.hold_phone(), arm_l='hip', rock=4, rate=0.35, **member(r, w))
        case(g, 220, 70)
        apple_box(g, -150, 90)
    return build


def dolly(w, seed=9):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L = 1100.0
        for sy in (-1, 1):
            g.box(STEEL, (0, sy * 36, 5), (L, 6, 10))
        for k in range(12):
            g.box(WOOD, (-L / 2.0 + (k + 0.5) * L / 12.0, 0, 2), (14, 96, 4))
        tape(g, L + 100, -150, posts=7)
        # everything on the dolly slides 3 m each way: a swing of 3 degrees about a pivot 60 m under the rails
        slide = dict(swing=3.0, pivot=(0.0, 0.0, -6000.0), axis=90.0)
        d = sc.part(P.motion(rate=0.07, **slide))
        d.box(GREY, (0, 0, 22), (150, 92, 14))
        for sx in (-1, 1):
            for sy in (-1, 1):
                d.cyl(DARK, (sx * 56, sy * 36 - 4, 10), 10, 8, rot=(0, 0, 90), steps=8)
        d.box(STEEL, (-30, 0, 62), (10, 10, 70))
        film_camera(d, -30, -6, 96)
        d.box(DARK, (40, 0, 52), (34, 34, 46))
        d.box(STEEL, (70, 0, 76), (5, 60, 5))
        d.box(STEEL, (70, 0, 52), (5, 5, 52))
        P.figure(sc, 36, 6, z=29, legs='sit', seat=46, arm_l='push', arm_r='push', rate=0.07, sway=slide, **member(r, w, hat='headset', hat_col=DARK))
        P.figure(sc, 126, 0, yaw=90, arm_l='push', arm_r='push', rate=0.07, sway=slide, **member(r, w))
        P.figure(sc, -420, 70, yaw=-25, arm_r='point', swing_r=9, axis_r='x', arm_l='hip', rate=0.5, **member(r, w))
        case(g, 430, 80)
    return build


def crane(w, seed=11):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(GREY, (0, 60, 24), (150, 220, 30))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.cyl(DARK, (sx * 70, 60 + sy * 80 - 6, 18), 18, 12, rot=(0, 0, 90), steps=8)
        g.box(YEL, (0, 60, 150), (26, 26, 240))
        g.box(DARK, (0, 60, 272), (40, 40, 16))
        # the arm rises and falls about the top of the post
        arm = sc.part(P.motion(swing=9.0, pivot=(0.0, 60.0, 280.0), axis=0.0, rate=0.09))
        tilt_box(arm, YEL, (0, 240, 240), (0, -500, 420), 14, 20)
        tilt_box(arm, STEEL, (0, 60, 330), (0, -480, 422), 4, 4)
        arm.box(DARK, (0, 250, 232), (60, 50, 60))
        arm.box(DARK, (0, -500, 400), (30, 30, 40))
        film_camera(arm, 0, -506, 352)
        P.figure(sc, 90, 190, yaw=160, arm_l='push', arm_r='push', rock=3, rate=0.09, **member(r, w))
        P.figure(sc, -130, -40, yaw=-20, arm_r='up', swing_r=8, arm_l='hip', rate=0.4, **member(r, w, hat='headset', hat_col=DARK))
        case(g, 170, 20)
    return build


def lights(w, seed=13):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        light_stand(g, -230, 0, height=340, yaw=14)
        light_stand(g, 240, 30, height=300, yaw=-18)
        # the reflector: a white square on a stand
        g.box(STEEL, (20, 70, 100), (5, 5, 200))
        tilt_box(g, WHITE, (20, 60, 120), (20, 30, 300), 190, 4)
        g.box(YEL, (-60, 150, 34), (110, 60, 68))
        g.box(DARK, (-60, 150, 72), (90, 44, 8))
        for k in range(3):
            g.cyl(DARK, (120 + k * 34, 150, 0), 15, 10, steps=8)
        g.box(DARK, (0, 110, 2), (480, 6, 4))
        P.figure(sc, -170, 40, yaw=60, arm_r='up', arm_l='up', swing_r=6, swing_l=-4, axis_r='x', axis_l='x', rate=0.5, **member(r, w))
        P.figure(sc, 110, 120, yaw=-150, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=GREY), rate=0.3, bob=-1.0, **member(r, w))
    return build


def sound(w, seed=15):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(GREY, (130, 40, 62), (90, 56, 46))
        g.box(DARK, (130, 34, 90), (80, 44, 10))
        for sx in (-1, 1):
            g.cyl(DARK, (130 + sx * 36, 36, 16), 16, 8, rot=(0, 0, 90), steps=8)
            g.box(STEEL, (130 + sx * 36, 40, 34), (5, 5, 30))
        g.box(STEEL, (176, 40, 150), (4, 4, 130))
        apple_box(g, 130, 110)
        P.figure(sc, 130, 110, z=30, yaw=0, legs='sit', seat=46, arm_l='hold', arm_r='hold', rate=0.3, bob=-0.8, **member(r, w, hat='headset', hat_col=DARK))
        P.figure(sc, -80, 10, arm_l='up', arm_r='up', hold_r=hold_boom(), rock=3, rate=0.25, **member(r, w, hat='headset', hat_col=DARK))
        case(g, -220, 80, col=YEL)
    return build


def catering(w, seed=17):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        # the canopy: pink and white, on four poles
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(STEEL, (sx * 190, 60 + sy * 110, 118), (6, 6, 236))
        for k in range(6):
            g.box(PINK if k % 2 else WHITE, (-160 + k * 64, 60, 242), (64, 240, 10))
        g.box(WHITE, (0, 60, 44), (300, 80, 88))
        g.box(PINK, (0, 19, 60), (300, 2, 12))
        g.cyl(STEEL, (-100, 60, 88), 16, 34, steps=8)
        g.cyl(STEEL, (-60, 60, 88), 13, 26, steps=8)
        for k in range(4):
            g.box(('#ffc93c', '#ff7a3c', '#7dffb0', '#fff6ea')[k], (10 + k * 36, 60, 92), (30, 44, 8))
        P.figure(sc, -20, 130, arm_r='fwd', swing_r=9, axis_r='x', arm_l='hold', rate=0.6, **member(r, w, hat='chef', hat_col=WHITE, top=WHITE, band=None))
        for k in range(3):
            P.figure(sc, -60 + k * 84, -50 - (k % 2) * 16, yaw=180 + r.uniform(-20, 20), arm_r='hold', arm_l='down', hold_r=P.hold_cup(), rate=0.3, bob=-1.2,
                     phase=r.random(), **member(r, w))
        apple_box(g, 290, -20)
        P.figure(sc, 290, -20, z=30, yaw=-30, legs='sit', seat=46, arm_l='hold', arm_r='face', hold_r=P.hold_cup(), rate=0.3, bob=-0.8, **member(r, w))
    return build


PIECES = (
    ('SM_CrewCamera', camera, {}),
    ('SM_CrewApplauseA', applause, {}),
    ('SM_CrewApplauseB', lambda w: applause(w, seed=8), {}),
    ('SM_CrewStunt', stunt, {}),
    ('SM_CrewVillage', village, {}),
    ('SM_CrewDolly', dolly, dict(grow=(340.0, 60.0, 60.0), grow_down=(340.0, 60.0, 0.0))),
    ('SM_CrewCrane', crane, dict(grow=(60.0, 60.0, 120.0))),
    ('SM_CrewLights', lights, {}),
    ('SM_CrewSound', sound, {}),
    ('SM_CrewCatering', catering, {}),
)


def run(wardrobes=None, only=None):
    P.build_core()
    for name in (wardrobes or LEVEL_WARDROBE):
        w = WARDROBES[name]
        jobs = [('%s_%s' % (piece, name), fn(w), opts) for piece, fn, opts in PIECES if not only or piece in only]
        P.export(DIR, jobs)


def paths(wardrobe):
    return ['%s/%s_%s' % (DIR, piece, wardrobe) for piece, _, _ in PIECES]
