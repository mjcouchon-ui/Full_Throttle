"""FULL THROTTLE (Unreal) the population, level 0, Bayshore, pass 3, builder p3d: HEIGHTS (/Game/People/Bayshore/SM_P3D_...).

People UP HIGH, so that they clear the road's barrier: every piece here brings its own structure and stands its people
4 to 13 m over the land (the land lies under the road's line and the chase camera rides low behind a metre-high
barrier: a person on the ground is a head, a person on a roof is a person). All of them are TALL: 24 m or more from the
road, and the table block (C:/ftwork/p3/L0d.inl) puts them 58 to 74 m out. No glow, nothing that blinks.

    SM_P3D_RoofParty_A / _B   a flat-roofed beach house and the party on its roof: a row at the rail, dancers on a
                              riser under a striped pergola, pennants.
    SM_P3D_FootBridge_A / _B  a footbridge laid ALONG the road, 20 m long, its deck 5.2 m up: walkers and a jogger
                              cross it and come round again (B: with a dog), others wave from the rail.
    SM_P3D_JudgeTower         the surf contest's judging tower: three judges at a table on the top deck holding up
                              coloured paddles, the contest's fans on the deck under them.
    SM_P3D_Lookout            a timber lookout on splayed legs: someone with binoculars turning to follow the chase,
                              someone pointing, a child waving, a flag, a gull on the roof.
    SM_P3D_SignPainters       a billboard being painted (a sun over the sea, no words): two painters on a cradle that
                              rides slowly up and down its ropes, a helper on the catwalk.
    SM_P3D_ScissorLift        decorators on a scissor lift and a stepladder stringing bunting between two poles.
    SM_P3D_ClimbWall          a climbing tower: three climbers working their way up and down the wall on ropes, two at
                              the top with their arms in the air.
    SM_P3D_SkyBikes           pedal cars on a rail 4.8 m up, 30 m of it along the road: three cars of two riders travel
                              it and come round again; a little station at each end.
    SM_P3D_BalconyBlock       the end of an apartment block: three balconies one over the other, everybody out on them,
                              washing on a line, two more on the roof under a parasol.

Run in the editor:  import ft_people_bay_p3d as K; K.run()        (or K.run(['SM_P3D_Lookout']))
"""
import random
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/Bayshore'

WOOD, WOOD_D, WHITE, CREAM = '#c9a06a', '#9a744a', '#fffaf2', '#fff1d6'
CORAL, TEAL, YEL, HOTPINK, MINT, SKY, NAVY, RED, LAV, PEACH = ('#ff7a8a', '#19c5c0', '#ffc93c', '#ff4fb8', '#7dffb0', '#57c7ff', '#2b2350',
                                                                 '#ff2d3c', '#b9a4ff', '#ffc9a0')
STEEL, DARK, ORANGE = '#c9c4d6', '#3a3848', '#ff7a3c'
HOT = ('#ff4fb8', '#ff2d3c', '#ffc93c', '#2ee6ff', '#ff7a3c', '#7dffb0', '#8f6bff', '#fffaf2', '#b8ff5a', '#ff5fa2')
SWIM = ('#ff2d3c', '#2ee6ff', '#ffc93c', '#ff4fb8', '#2b2350', '#7dffb0', '#ff7a3c', '#8f6bff')
FOOT = 220.0         # how far legs and walls go down under z = 0 (the land falls away from the road)


# ------------------------------------------------------------------------------------------ people
def fan_look(r, **fixed):
    """A holidaymaker: beach clothes, a hot colour, sometimes a cap."""
    d = P.look(r, tops=HOT, bottoms=SWIM, wear=r.choice(('shorts', 'shorts', 'swim', 'trousers', 'skirt')),
               sleeves=r.choice(('none', 'short', 'short')), hair=r.choice(('short', 'short', 'long', 'pony', 'bald', 'afro', 'bun')))
    if r.random() < 0.25:
        d.update(hat='cap', hat_col=r.choice(HOT))
    d.update(fixed)
    return d


def flag_hold(col, col2, w=110.0):
    """A big two-colour flag on a pole in the hand (it swings with the arm)."""
    def f(m, h, side):
        m.box(STEEL, (h[0], h[1], h[2] + 70), (5, 5, 170))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + 132), (w, 4, 34))
        m.box(col2, (h[0] + side * w / 2.0, h[1], h[2] + 100), (w, 4, 30))
    return f


def cheer(r, i):
    """What one of a crowd does: big gestures (they are read from 60 m and more). Every third never rests."""
    k = i % 6
    a = dict(react=(i % 3 != 1), rate=r.uniform(1.2, 1.7), phase=r.random())
    if k == 0:
        a.update(arm_l='up', arm_r='up', swing_l=22, swing_r=-22, bob=r.uniform(14, 22))
    elif k == 1:
        a.update(arm_l='vee', arm_r='vee', swing_l=38, swing_r=38, bob=r.uniform(6, 10), rate=r.uniform(0.9, 1.2))
    elif k == 2:
        a.update(arm_r='wave', swing_r=44, arm_l='hip', bob=r.uniform(10, 16))
    elif k == 3:
        a.update(arm_l='point', arm_r='up', swing_r=30, bob=r.uniform(8, 14))
    elif k == 4:
        a.update(arm_l='up', arm_r='up', hold_r=P.hold_phone(), bob=r.uniform(3, 6), rate=r.uniform(0.6, 0.9))
    else:
        a.update(arm_l='vee', arm_r='vee', rock=r.uniform(9, 13), rate=r.uniform(0.8, 1.1))
    return a


def fan(sc, r, x, y, z, i, s=1.2, detail=0, yaw=0.0, kid=False, **fixed):
    """One of the crowd at (x, y, z). Every figure made here moves (a figure at rest would share the still model)."""
    d = fan_look(r, s=s * r.uniform(0.94, 1.06))
    if kid:
        d.update(build='kid', s=s * r.uniform(0.66, 0.74))
    d.update(cheer(r, i))
    d.update(fixed)
    return P.figure(sc, x, y, z=z, yaw=yaw + r.uniform(-12, 12), detail=detail, **d)


def row(sc, r, n, x0, x1, y, z, s=1.2, detail=0, start=0, kid_at=(), flag_at=(), cols=(HOTPINK, WHITE)):
    for i in range(n):
        x = x0 + (i + 0.5) * (x1 - x0) / n + r.uniform(-12, 12)
        extra = {}
        if i in flag_at:
            extra = dict(react=False, rate=r.uniform(0.7, 0.9), arm_l='hip', arm_r='up', swing_r=30, swing_l=0, rock=0,
                         hold_r=flag_hold(cols[0], cols[1]), bob=4)
        fan(sc, r, x, y + r.uniform(-8, 8), z, i * 5 + start * 3 + int(r.random() * 6), s=s, detail=detail, kid=(i in kid_at), **extra)


# ------------------------------------------------------------------------------------------ furniture
def stripes(g, x0, x1, y, z0, z1, cols, n, t=6.0):
    """A board of upright stripes facing the road."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        g.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y, (z0 + z1) / 2.0), (w, t, z1 - z0))


def canopy(g, x0, x1, y0, y1, z, cols, n):
    """A flat striped awning."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        g.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, (y0 + y1) / 2.0, z), (w, y1 - y0, 10))


def post(g, x, y, top, col=STEEL, w=12.0, z0=-FOOT):
    g.box(col, (x, y, (top + z0) / 2.0), (w, w, top - z0))


def rail_x(g, x0, x1, y, z, col=WHITE, n=5, h=104.0):
    """A rail along X, its top h over z."""
    g.box(col, ((x0 + x1) / 2.0, y, z + h), (x1 - x0, 8, 8))
    for k in range(n):
        g.box(col, (x0 + 4 + k * (x1 - x0 - 8) / float(max(n - 1, 1)), y, z + h / 2.0), (7, 7, h))


def rail_y(g, x, y0, y1, z, col=WHITE, h=104.0):
    g.box(col, (x, (y0 + y1) / 2.0, z + h), (8, y1 - y0, 8))


def pennant(sc, g, x, y, z0, z1, col, phase=0.0, w=120.0, h=64.0, pole=STEEL):
    """A pole from z0 to z1 and a pennant at its head that swings about the pole."""
    g.box(pole, (x, y, (z0 + z1) / 2.0), (7, 7, z1 - z0))
    fl = sc.part(P.motion(swing=22.0, pivot=(x, y, z1 - h / 2.0), axis=P.UP, rate=1.1, phase=phase + 0.07))
    fl.box(col, (x + w / 2.0 + 4, y, z1 - h / 2.0), (w, 5, h))
    fl.box(WHITE, (x + w * 0.5 + 4, y, z1 - h / 2.0), (w * 0.34, 6, h * 0.42))


def parasol(g, x, y, z, col, rad=150.0, h=250.0):
    g.cyl(WHITE, (x, y, z), 5, h, steps=5)
    g.cyl(col, (x, y, z + h - 6), rad, 46, steps=8, top_r=10)


# ------------------------------------------------------------------------------------------ the roof party
def roof_party(seed=11, variant=0):
    """A flat-roofed beach house, 9 m by 6 m, the roof 4.2 m up: five at the rail, three dancing on a riser under a
    striped pergola (B: one in a deck chair with a drink, a water tank), an outside stair, two pennants."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Wd, D, H = 900.0, 600.0, 400.0
        wall, c1, c2, c3 = ((PEACH, HOTPINK, TEAL, YEL) if variant == 0 else (SKY, YEL, CORAL, MINT))
        g.box(wall, (0, 0, (H - FOOT) / 2.0), (Wd, D, H + FOOT))
        yf = -D / 2.0 - 3
        g.box(NAVY, (-250, yf, 110), (110, 6, 220))                            # a door, two windows
        g.box(NAVY, (90, yf, 220), (150, 6, 120))
        g.box(NAVY, (300, yf, 220), (150, 6, 120))
        stripes(g, -Wd / 2.0, Wd / 2.0, yf - 2, H - 70, H, (c1, WHITE), 9)     # a painted frieze under the roof
        Z = H + 16.0
        g.box(WHITE, (0, 0, H + 8), (Wd + 40, D + 40, 16))
        rail_x(g, -Wd / 2.0 - 16, Wd / 2.0 + 16, -D / 2.0 - 14, Z, n=6)
        for sx in (-1, 1):
            rail_y(g, sx * (Wd / 2.0 + 14), -D / 2.0 - 14, D / 2.0, Z)
        for sx in (-1, 1):                                                     # the pergola over the back half
            post(g, sx * (Wd / 2.0 - 30), D / 2.0 - 30, Z + 300, WHITE, 10, Z)
            post(g, sx * (Wd / 2.0 - 30), 30, Z + 300, WHITE, 10, Z)
        canopy(g, -Wd / 2.0, Wd / 2.0, 10, D / 2.0, Z + 304, (c2, WHITE), 8)
        g.box(c1, (0, 8, Z + 290), (Wd, 6, 28))
        g.box(WOOD, (0, 150, Z + 20), (520, 200, 40))                          # the riser the dancers are on
        for sx in (-1, 1):                                                     # the loudspeakers
            g.box(DARK, (sx * 350, 180, Z + 60), (80, 70, 120))
            g.box(c3, (sx * 350, 143, Z + 70), (44, 4, 44))
        tilt_box(g, WHITE, (Wd / 2.0 + 70, D / 2.0 - 40, Z - 8), (Wd / 2.0 + 70, -D / 2.0 + 40, -FOOT * 0.3), 100, 12)   # the outside stair
        pennant(sc, g, -Wd / 2.0 - 10, -D / 2.0 - 10, Z, Z + 430, c1, phase=0.0)
        pennant(sc, g, Wd / 2.0 + 10, -D / 2.0 - 10, Z, Z + 430, c2, phase=0.4)
        if variant == 0:
            parasol(g, -340, -60, Z, c3)
        else:
            g.cyl(STEEL, (-330, 200, Z + 40), 80, 150, steps=8)                # a water tank on its legs
            g.cyl(c2, (-330, 200, Z + 190), 84, 40, steps=8, top_r=10)
            g.box(STEEL, (-330, 200, Z + 20), (120, 120, 40))
            g.box(c3, (330, -60, Z + 30), (60, 110, 14))                       # a deck chair, somebody in it with a drink
            P.figure(sc, 330, -30, z=Z, seat=37.0, legs='sit', arm_r='face', hold_r=P.hold_cup(), arm_l='wave', swing_l=30, rate=0.8,
                     phase=r.random(), detail=1, **fan_look(r, s=1.2, hat='sun', hat_col=CREAM))
        row(sc, r, 5 if variant == 0 else 4, -Wd / 2.0 + 60, (Wd / 2.0 - 60) if variant == 0 else 220, -D / 2.0 + 56, Z, detail=1 if variant == 0 else 0,
            start=variant, kid_at=(3,), flag_at=(1,), cols=(c1, c3))
        for k, x in enumerate((-170, 0, 170)):                                 # the dancers
            P.figure(sc, x, 150 + (20 if k == 1 else 0), z=Z + 40, yaw=r.uniform(-30, 30), arm_l='vee', arm_r='up', turn=r.uniform(24, 34),
                     bob=r.uniform(8, 14), rate=r.uniform(1.2, 1.5), phase=r.random(), detail=0, **fan_look(r, s=1.2))
    return build


# ------------------------------------------------------------------------------------------ the footbridge
def foot_bridge(seed=21, variant=0):
    """A footbridge laid along the road: a deck 20 m long and 5.2 m up between two portal frames, stairs down at each
    end (30 m over all). Two walkers and a jogger cross and come round again (B: a dog runs with the jogger, and a
    second couple the other way), four at the road-side rail."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Lg, Dk, H = 2000.0, 280.0, 520.0
        c1, c2, c3 = ((TEAL, HOTPINK, YEL) if variant == 0 else (CORAL, SKY, MINT))
        g.box(CREAM, (0, 0, H - 12), (Lg, Dk, 24))
        stripes(g, -Lg / 2.0, Lg / 2.0, -Dk / 2.0 - 4, H - 96, H, (c1, WHITE), 16)
        g.box(c2, (0, -Dk / 2.0 - 6, H - 104), (Lg, 8, 16))
        rail_x(g, -Lg / 2.0, Lg / 2.0, -Dk / 2.0 + 4, H, n=9)
        rail_x(g, -Lg / 2.0, Lg / 2.0, Dk / 2.0 - 4, H, n=5)
        for k, sx in enumerate((-1, 1)):                                       # the portal frames
            x = sx * (Lg / 2.0 - 60)
            for sy in (-1, 1):
                post(g, x, sy * (Dk / 2.0 + 14), H + 330, WHITE, 26)
            g.box(c2, (x, 0, H + 330), (30, Dk + 60, 26))
            g.box(c3, (x, -Dk / 2.0 - 16, H + 270), (60, 6, 60))
            pennant(sc, g, x, 0, H + 340, H + 600, (c2, c3)[k], phase=0.35 * k, w=140.0, h=72.0)
            tilt_box(g, CREAM, (sx * Lg / 2.0, 0, H - 12), (sx * (Lg / 2.0 + 520), 0, -FOOT * 0.4), Dk - 40, 14)     # the stairs
        for sy in (-1, 1):
            post(g, 0, sy * (Dk / 2.0 - 20), H - 24, WHITE, 22)
        Lt = Lg - 120.0
        # along +X
        P.figure(sc, -500, 62, z=H, yaw=90, stride=24, swing_l=20, swing_r=-20, axis_l='x', axis_r='x', rate=1.0, bob=3, phase=r.random(),
                 travel=(Lt, 115), detail=1, hold_l=P.hold_bag(c2), **fan_look(r, s=1.2, wear='dress', hair='long'))
        P.figure(sc, -430, 26, z=H, yaw=90, stride=24, swing_l=20, swing_r=-20, axis_l='x', axis_r='x', rate=1.0, bob=3, phase=r.random(),
                 travel=(Lt, 115), detail=1, **fan_look(r, s=1.25, hat='sun', hat_col=CREAM))
        # along -X: the jogger
        P.figure(sc, 300, 100, z=H, yaw=-90, legs='stride', stride=36, swing_l=34, swing_r=-34, axis_l='x', axis_r='x', rate=1.9, bob=6,
                 phase=r.random(), travel=(Lt, -270), detail=1, **fan_look(r, s=1.2, wear='shorts', sleeves='none', band=WHITE))
        if variant == 1:
            P.quadruped(sc, 180, 104, z=H, yaw=-90, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=24,
                        rate=2.4, phase=r.random(), travel=(Lt, -270), body='#e8c26a', s=1.3)
            P.figure(sc, 700, 40, z=H, yaw=-90, stride=22, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=0.9, bob=3, phase=r.random(),
                     travel=(Lt, -100), detail=0, hold_r=P.hold_board(YEL, RED, 170.0), **fan_look(r, s=1.2, wear='swim', sleeves='none'))
        else:
            P.figure(sc, 650, 40, z=H, yaw=-90, stride=22, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=0.9, bob=3, phase=r.random(),
                     travel=(Lt, -100), detail=0, hold_r=flag_hold(c2, WHITE, 70.0), arm_r='up', **fan_look(r, s=0.85, build='kid'))
        row(sc, r, 4, -720, 720, -Dk / 2.0 + 52, H, detail=0, start=1 + variant, kid_at=(2,), flag_at=(0,) if variant else (3,), cols=(c2, c3))
    return build


# ------------------------------------------------------------------------------------------ the surf contest's judges
def judge_tower(seed=31):
    """A scaffold tower, two decks (3 m and 5.7 m) under a striped roof: three judges sit at a table on the top deck
    and hold up coloured paddles; four of the contest's fans on the lower deck; surfboards lashed to its side."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Wd, D, Z1, Z2, RF = 520.0, 380.0, 300.0, 570.0, 860.0
        hw, hd = Wd / 2.0, D / 2.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                post(g, sx * (hw - 10), sy * (hd - 10), RF, STEEL, 13)
            tilt_box(g, STEEL, (sx * (hw - 10), -hd + 10, 10), (sx * (hw - 10), hd - 10, Z1 - 30), 7, 7)
        tilt_box(g, STEEL, (-hw + 10, -hd + 10, 10), (hw - 10, -hd + 10, Z1 - 160), 7, 7)
        g.box(WOOD, (0, 0, Z1 - 9), (Wd + 50, D + 50, 18))
        g.box(WOOD, (0, 0, Z2 - 9), (Wd + 10, D + 10, 18))
        stripes(g, -hw - 25, hw + 25, -hd - 28, Z1 - 150, Z1 + 30, (HOTPINK, WHITE, TEAL, WHITE), 8)      # the banner hung from the lower deck
        g.cyl(YEL, (0, -hd - 32, Z1 - 60), 56, 8, steps=10, rot=(90, 90, 0))                              # a sun on it
        rail_x(g, -hw - 25, hw + 25, -hd - 22, Z1, n=4)
        stripes(g, -hw - 5, hw + 5, -hd - 8, Z2 - 70, Z2, (YEL, WHITE), 8)
        g.box(WHITE, (0, -hd + 40, Z2 + 42), (Wd - 70, 56, 84))                                           # the judges' table
        g.box(TEAL, (0, -hd + 10, Z2 + 44), (Wd - 110, 4, 50))
        canopy(g, -hw - 30, hw + 30, -hd - 60, hd + 20, RF + 5, (CORAL, WHITE), 6)
        g.box(CORAL, (0, -hd - 60, RF - 12), (Wd + 60, 6, 30))
        for k in range(3):                                                                                # boards on its side
            g.box((YEL, MINT, HOTPINK)[k], (hw + 12, -110 + k * 110, Z1 + 126), (8, 46, 220))
            g.box(WHITE, (hw + 17, -110 + k * 110, Z1 + 126), (2, 9, 190))
        for k in range(7):                                                                                # the ladder up the back
            g.box(STEEL, (0, hd + 14, 40 + k * 80), (60, 6, 6))
        for sx in (-1, 1):
            g.box(STEEL, (sx * 30, hd + 14, Z2 / 2.0), (6, 6, Z2))
        pennant(sc, g, -hw + 10, hd - 10, RF, RF + 250, TEAL, phase=0.0)
        pennant(sc, g, hw - 10, hd - 10, RF, RF + 250, HOTPINK, phase=0.45)
        for k, x in enumerate((-150, 0, 150)):                                                            # the judges
            g = sc.still()
            g.box(DARK, (x, -hd + 112, Z2 + 22), (44, 44, 44))
            col = (YEL, HOTPINK, MINT)[k]
            P.figure(sc, x, -hd + 112, z=Z2, legs='sit', arm_r='up', arm_l='hip', swing_r=24, rate=r.uniform(0.7, 1.0), phase=r.random(),
                     hold_r=P.hold_sign(col, WHITE, w=56.0, h2=56.0, pole=STEEL), detail=1,
                     **fan_look(r, s=1.2, top=WHITE, hat='sun' if k != 1 else 'cap', hat_col=CREAM if k != 1 else RED))
        row(sc, r, 4, -hw + 10, hw - 10, -hd + 50, Z1, detail=0, start=2, flag_at=(3,), cols=(HOTPINK, YEL))
    return build


# ------------------------------------------------------------------------------------------ the lookout
def binoculars(m, h, side):
    m.box(DARK, (h[0] - side * 10, h[1] - 8, h[2] + 4), (30, 18, 12))


def lookout(seed=41):
    """A timber lookout on four splayed legs, its deck 6.4 m up under a pointed roof: one with binoculars turning to
    follow the chase, one pointing and hopping, a child waving, one with a flag; a telescope; a gull on the roof."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, Wd = 640.0, 360.0
        hw = Wd / 2.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                tilt_box(g, WOOD_D, (sx * 290, sy * 290, -FOOT), (sx * (hw - 10), sy * (hw - 10), H), 24, 24)
                post(g, sx * (hw - 6), sy * (hw - 6), H + 250, WOOD_D, 14, H)
        for z, e in ((230, 233), (440, 200)):                                  # the ring beams
            for s2 in (-1, 1):
                g.box(WOOD, (0, s2 * e, z), (2 * e, 14, 14))
                g.box(WOOD, (s2 * e, 0, z), (14, 2 * e, 14))
        tilt_box(g, WOOD, (-233, -233, 230), (200, -200, 440), 10, 10)
        g.box(WOOD, (0, 0, H - 10), (Wd + 70, Wd + 70, 20))
        stripes(g, -hw - 35, hw + 35, -hw - 38, H - 70, H + 2, (TEAL, WHITE), 6)
        for s2 in (-1, 1):
            g.box(WHITE, (0, s2 * (hw + 26), H + 104), (Wd + 60, 8, 8))
            g.box(WHITE, (s2 * (hw + 26), 0, H + 104), (8, Wd + 60, 8))
        g.cyl(CORAL, (0, 0, H + 250), 300, 150, steps=8, top_r=12)             # the roof
        g.box(WHITE, (0, 0, H + 246), (Wd + 90, Wd + 90, 10))
        for k in range(8):                                                     # the ladder up the back
            g.box(WOOD, (0, hw + 60, 30 + k * 80), (60, 6, 6))
        tilt_box(g, WOOD_D, (-30, hw + 110, -40), (-30, hw + 30, H), 7, 7)
        tilt_box(g, WOOD_D, (30, hw + 110, -40), (30, hw + 30, H), 7, 7)
        g.cyl(STEEL, (110, -hw + 30, H), 5, 112, steps=5)                      # the telescope
        tilt_box(g, DARK, (110, -hw + 62, H + 112), (110, -hw - 14, H + 134), 15, 15)
        pennant(sc, g, 0, 0, H + 400, H + 640, HOTPINK, w=150.0, h=80.0)
        P.bird(sc, 150, -150, H + 310, heading=-60, span=95, flap=0.0, bob=3.0, rate=0.8, phase=0.3)
        # the watchers
        P.figure(sc, -110, -hw + 44, z=H, arm_l='face', arm_r='face', hold_r=binoculars, turn=26, rate=0.3, phase=r.random(), detail=1,
                 **fan_look(r, s=1.25, hat='sun', hat_col=CREAM, top=RED))
        fan(sc, r, 20, -hw + 50, H, 3, s=1.25, detail=1)
        fan(sc, r, 150, -hw + 90, H, 2, s=1.25, detail=1, kid=True)
        fan(sc, r, -60, 60, H, 0, s=1.25, detail=1, react=False, rate=0.8, arm_l='hip', arm_r='up', swing_r=30, swing_l=0,
            hold_r=flag_hold(YEL, TEAL), bob=4)
    return build


# ------------------------------------------------------------------------------------------ the sign painters
def roller(col):
    def f(m, h, side):
        m.box(STEEL, (h[0], h[1], h[2] + 30), (4, 4, 70))
        m.box(col, (h[0], h[1] - 6, h[2] + 68), (34, 12, 14))
    return f


def sign_painters(seed=51):
    """A billboard, 11 m by 5.2 m, its foot 3.8 m up, being painted: a sun over the sea, half done (no words). Two
    painters on a cradle that rides 70 cm up and down its ropes, one of them turned round to wave; a helper on the
    catwalk under the board."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z0, BH, BW = 380.0, 520.0, 1100.0
        top = Z0 + BH
        for sx in (-1, 1):
            post(g, sx * 380, 30, top, DARK, 30)
        g.box(CREAM, (0, 0, Z0 + BH / 2.0), (BW, 24, BH))
        g.box(TEAL, (0, 0, top + 10), (BW + 30, 34, 20))
        g.box(TEAL, (0, 0, Z0 - 10), (BW + 30, 34, 20))
        g.box(SKY, (-270, -14, Z0 + 330), (540, 6, 330))                        # the painted half: sky, sea, sun
        g.box(TEAL, (-270, -15, Z0 + 110), (540, 6, 110))
        g.box(NAVY, (-270, -15, Z0 + 30), (540, 6, 56))
        g.cyl(YEL, (-300, -18, Z0 + 310), 110, 8, steps=10, rot=(90, 90, 0))
        g.box(HOTPINK, (-270, -16, Z0 + 180), (540, 6, 26))
        g.box(SKY, (120, -14, Z0 + 400), (220, 6, 190))                         # where the rollers have got to
        g.box(TEAL, (70, -15, Z0 + 110), (130, 6, 110))
        g.box(WOOD, (0, -80, Z0 - 6), (BW, 130, 12))                            # the catwalk
        g.box(DARK, (-520, -80, Z0 - 60), (10, 130, 100))
        g.box(DARK, (520, -80, Z0 - 60), (10, 130, 100))
        for k, c in enumerate((HOTPINK, YEL, TEAL)):                           # paint tins
            g.cyl(c, (-330 + k * 50, -90, Z0), 16, 30, steps=6)
        cx, cy, cz = 260.0, -100.0, Z0 + 180.0
        for sx in (-1, 1):                                                     # the ropes and their outriggers
            g.box(DARK, (cx + sx * 200, cy, (cz - 80 + top + 20) / 2.0), (5, 5, top + 100 - cz))
            g.box(DARK, (cx + sx * 200, -40, top + 24), (8, 150, 8))
        ph = r.random()
        mo = P.motion(phase=ph, rate=0.25, bob=-70.0)
        c = sc.part(mo)
        c.box(WOOD_D, (cx, cy, cz - 5), (440, 96, 10))
        c.box(ORANGE, (cx, cy - 50, cz + 100), (440, 6, 6))
        c.box(ORANGE, (cx, cy - 50, cz + 50), (440, 6, 6))
        for sx in (-1, 1):
            c.box(ORANGE, (cx + sx * 218, cy, cz + 52), (6, 96, 108))
        c.cyl(HOTPINK, (cx + 20, cy + 20, cz), 15, 28, steps=6)
        P.figure(sc, cx - 110, cy, z=cz, yaw=180, arm_r='up', arm_l='hip', swing_r=26, axis_r='y', hold_r=roller(SKY), rate=0.25, phase=ph,
                 bob=-70.0, detail=1, **fan_look(r, s=1.2, top=WHITE, bottom='#fff6ea', wear='trousers', hat='cap', hat_col=WHITE))
        P.figure(sc, cx + 120, cy - 6, z=cz, yaw=10, arm_r='wave', arm_l='down', swing_r=40, axis_r='y', hold_l=roller(TEAL), rate=0.25, phase=ph,
                 bob=-70.0, detail=1, **fan_look(r, s=1.2, top=WHITE, bottom='#fff6ea', wear='trousers', hat='cap', hat_col=HOTPINK))
        fan(sc, r, -440, -84, Z0, 1, s=1.2, detail=1, top=WHITE)
        g = sc.still()
        pennant(sc, g, -BW / 2.0 - 4, 0, top, top + 240, HOTPINK, phase=0.2)
    return build


# ------------------------------------------------------------------------------------------ the decorators
def scissor_lift(seed=61):
    """Bunting going up for the shoot: a scissor lift, its platform 4.3 m up, one decorator reaching up to the line and
    one holding a box of flags; a third on a stepladder at the far pole. The bunting stirs."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        PZ = 434.0
        g.box(DARK, (0, 0, (70 - FOOT) / 2.0), (300, 170, 70 + FOOT))          # the chassis (down into falling land)
        g.box(TEAL, (0, 0, 80), (320, 184, 24))
        for k in range(3):                                                     # the scissors
            z0 = 96 + k * 110
            for sy in (-1, 1):
                tilt_box(g, ORANGE, (-135, sy * 72, z0), (135, sy * 72, z0 + 108), 12, 12)
                tilt_box(g, ORANGE, (135, sy * 72, z0), (-135, sy * 72, z0 + 108), 12, 12)
        g.box(TEAL, (0, 0, PZ - 7), (350, 190, 14))
        g.box(TEAL, (0, -95, PZ + 12), (350, 6, 24))
        rail_x(g, -175, 175, -95, PZ, ORANGE, n=3)
        rail_x(g, -175, 175, 95, PZ, ORANGE, n=3)
        for sx in (-1, 1):
            rail_y(g, sx * 175, -95, 95, PZ, ORANGE)
        PX, PT = 780.0, 780.0
        for k, sx in enumerate((-1, 1)):                                       # the two poles
            post(g, sx * PX, 0, PT, WHITE, 14)
            g.box((HOTPINK, TEAL)[k], (sx * PX, 0, PT + 12), (26, 26, 24))
        n = 10
        pts = []
        for k in range(n + 1):
            x = -PX + k * 2.0 * PX / n
            pts.append((x, 0.0, PT - 20 - 95.0 * (1.0 - (x / PX) ** 2)))
        fl = sc.part(P.motion(swing=13.0, pivot=(0.0, 0.0, PT - 80.0), axis=0.0, rate=1.2, phase=0.21))
        g = sc.still()
        cols = (HOTPINK, YEL, TEAL, WHITE, CORAL)
        for k in range(n):
            a, b = pts[k], pts[k + 1]
            tilt_box(g, DARK, a, b, 5, 5)
            mx, mz = (a[0] + b[0]) / 2.0, (a[2] + b[2]) / 2.0
            fl.box(cols[k % 5], (mx, 0, mz - 34), (86, 4, 60))
        tilt_box(g, STEEL, (PX - 90, -50, -30), (PX - 90, 30, 300), 70, 6)      # the stepladder at the far pole
        tilt_box(g, STEEL, (PX - 90, 110, -30), (PX - 90, 30, 300), 70, 6)
        g.box(STEEL, (PX - 90, 30, 262), (76, 70, 8))
        P.figure(sc, -30, -20, z=PZ, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=0.7, phase=r.random(), bob=2, detail=1,
                 **fan_look(r, s=1.22, top=YEL, hat='cap', hat_col=RED))
        P.figure(sc, 100, 10, z=PZ, arm_l='carry', arm_r='carry', hold_r=P.hold_box(HOTPINK), rock=7, rate=0.6, phase=r.random(), detail=1,
                 **fan_look(r, s=1.2, top=YEL))
        P.figure(sc, PX - 90, 24, z=266, yaw=-70, arm_l='up', arm_r='wave', swing_r=40, rate=1.1, phase=r.random(), detail=1,
                 **fan_look(r, s=1.2, top=YEL, hat='cap', hat_col=TEAL))
    return build


# ------------------------------------------------------------------------------------------ the climbing tower
def climb_wall(seed=71):
    """A climbing tower 9 m high, its face in big pastel panels dotted with holds: three climbers in helmets working up
    and down their ropes, two at the top with their arms in the air, one with a flag."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Wd, T, H = 560.0, 200.0, 900.0
        g.box(LAV, (0, 40, (H - FOOT) / 2.0), (Wd, T, H + FOOT))
        yf = -60.0
        cols = (PEACH, MINT, SKY, CREAM, CORAL)
        for i in range(4):
            for k in range(3):
                g.box(cols[(i * 2 + k) % 5], (-Wd / 2.0 + (i + 0.5) * Wd / 4.0, yf - 2, (k + 0.5) * H / 3.0), (Wd / 4.0 - 8, 6, H / 3.0 - 8))
        for k in range(16):                                                    # the holds
            g.box(r.choice(HOT), (r.uniform(-Wd / 2.0 + 30, Wd / 2.0 - 30), yf - 9, r.uniform(60, H - 40)), (16, 10, 16))
        TZ = H + 30.0
        g.box(WHITE, (0, 0, H + 15), (Wd + 70, T + 160, 30))                   # the ledge at the top
        stripes(g, -Wd / 2.0 - 35, Wd / 2.0 + 35, -T / 2.0 - 84, H - 60, H + 2, (HOTPINK, WHITE), 8)
        g.box(WHITE, (0, 130, TZ + 104), (Wd + 60, 8, 8))
        for sx in (-1, 1):
            rail_y(g, sx * (Wd / 2.0 + 30), -150, 130, TZ)
            post(g, sx * (Wd / 2.0 + 30), 130, TZ + 104, WHITE, 8, TZ)
        pennant(sc, g, -Wd / 2.0 - 20, 120, TZ, TZ + 360, TEAL, phase=0.0, w=150.0, h=80.0)
        pennant(sc, g, Wd / 2.0 + 20, 120, TZ, TZ + 360, YEL, phase=0.5, w=150.0, h=80.0)
        for k, (x, z0) in enumerate(((-180, 150), (10, 560), (195, 350))):     # the climbers and their ropes
            g = sc.still()
            g.box(DARK, (x, yf - 44, (z0 + 60 + H) / 2.0), (4, 4, H - z0 - 60))
            P.figure(sc, x, yf - 26, z=z0, yaw=180, s=1.1, legs='wide', arm_l='up', arm_r='vee', swing_l=16, swing_r=-20, axis_l='y', axis_r='y',
                     bob=-60.0, rate=(0.22, 0.17, 0.27)[k], phase=r.random(), detail=1, hat='helmet', hat_col=(RED, YEL, TEAL)[k],
                     band=DARK, sleeves='short', wear='shorts', skin=r.choice(P.SKINS), top=r.choice(HOT), bottom=r.choice(SWIM))
        fan(sc, r, -90, -90, TZ, 0, s=1.2, detail=1)
        fan(sc, r, 110, -80, TZ, 2, s=1.2, detail=1, react=False, rate=0.8, arm_l='hip', arm_r='up', swing_r=30, swing_l=0,
            hold_r=flag_hold(HOTPINK, WHITE), bob=4)
    return build


# ------------------------------------------------------------------------------------------ the sky bikes
def sky_bikes(seed=81):
    """Pedal cars on a rail 4.8 m up, 30 m of it laid along the road, a little station at each end: three cars of two
    riders each travel it and come round again (they never leave the rail's line); a striped skirt hangs under the rail."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Lg, RZ = 3000.0, 485.0
        g.box(WHITE, (0, 0, RZ - 15), (Lg, 26, 30))
        stripes(g, -Lg / 2.0, Lg / 2.0, 0, RZ - 90, RZ - 30, (HOTPINK, WHITE, TEAL, WHITE), 12, t=5.0)
        for x in (-1200, -400, 400, 1200):
            post(g, x, 0, RZ - 30, WHITE, 30)
        for k, sx in enumerate((-1, 1)):                                       # the stations
            x = sx * (Lg / 2.0 + 150)
            g.box(WOOD, (x, 30, RZ - 40), (300, 280, 16))
            for ax in (-1, 1):
                post(g, x + ax * 135, 160, RZ + 270, WHITE, 12)
                post(g, x + ax * 135, -100, RZ + 270, WHITE, 12, RZ - 32)
                post(g, x + ax * 135, -100, RZ - 40, WHITE, 12)
            canopy(g, x - 160, x + 160, -120, 180, RZ + 274, ((YEL, WHITE), (CORAL, WHITE))[k], 4)
            rail_x(g, x - 150, x + 150, -108, RZ - 32, n=2)
            tilt_box(g, WOOD, (x, 170, RZ - 40), (x, 620, -FOOT * 0.4), 120, 12)
            pennant(sc, g, x, 160, RZ + 280, RZ + 520, (HOTPINK, TEAL)[k], phase=0.4 * k)
            fan(sc, r, x + sx * 40, -50, RZ - 32, k + 1, s=1.2, detail=0)
            g = sc.still()
        Lt, V = 2800.0, 300.0
        for k, x0 in enumerate((-930, 0, 930)):                                # the cars
            col, col2 = ((HOTPINK, YEL), (TEAL, WHITE), (YEL, CORAL))[k]
            car = sc.part(P.motion(travel=(Lt, V / Lt), base=(x0, 0.0), pivot=(x0, 0.0, RZ + 30)))
            car.box(col, (x0, 0, RZ + 24), (190, 76, 46))
            car.box(col2, (x0 + 100, 0, RZ + 20), (30, 60, 34))
            car.box(DARK, (x0, 0, RZ + 2), (150, 40, 8))
            for ax in (-80, 80):
                car.box(WHITE, (x0 + ax, 0, RZ + 150), (6, 6, 210))
            car.box(col2, (x0, 0, RZ + 258), (220, 110, 10))
            car.box(col, (x0, -55, RZ + 244), (220, 4, 22))
            P.figure(sc, x0 + 34, 0, z=RZ, yaw=90, legs='sit', arm_l='fwd', arm_r='fwd', swing_l=8, swing_r=-8, axis_l='x', axis_r='x',
                     rate=1.6, phase=r.random(), travel=(Lt, V), detail=0, **fan_look(r, s=1.15))
            P.figure(sc, x0 - 42, 0, z=RZ, yaw=90, legs='sit', arm_l='hip', arm_r='wave', swing_r=42, rate=1.3, phase=r.random(),
                     travel=(Lt, V), detail=1, **fan_look(r, s=1.15))
    return build


# ------------------------------------------------------------------------------------------ the balconies
def balcony_block(seed=91):
    """The end of an apartment block, 7.2 m wide and 12.6 m high: three balconies one over the other and everybody out
    on them (washing stirs on a line on the second), two more on the roof beside a parasol and a water tank."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Wd, D, H = 720.0, 440.0, 1260.0
        g.box(CREAM, (0, 0, (H - FOOT) / 2.0), (Wd, D, H + FOOT))
        yf = -D / 2.0
        g.box(CORAL, (-Wd / 2.0 + 36, yf - 3, H / 2.0), (56, 6, H))            # a painted band up each corner
        g.box(CORAL, (Wd / 2.0 - 36, yf - 3, H / 2.0), (56, 6, H))
        g.box(NAVY, (-150, yf - 3, 110), (120, 6, 220))                        # the street door
        g.box(WHITE, (0, 0, H + 15), (Wd + 30, D + 30, 30))
        floors = (330.0, 640.0, 950.0)
        vcols = ((HOTPINK, WHITE), (TEAL, WHITE), (YEL, WHITE))
        for k, z in enumerate(floors):
            g.box(WHITE, (0, yf - 85, z - 8), (600, 170, 16))
            stripes(g, -300, 300, yf - 172, z - 78, z - 2, vcols[k], 6)
            rail_x(g, -300, 300, yf - 166, z, n=4)
            g.box(NAVY, (-140, yf - 3, z + 112), (130, 6, 224))
            g.box(NAVY, (130, yf - 3, z + 140), (150, 6, 110))
        Z = H + 30.0
        parasol(g, -200, -60, Z, HOTPINK, rad=160.0, h=260.0)
        g.cyl(STEEL, (210, 100, Z + 40), 90, 150, steps=8)
        g.cyl(TEAL, (210, 100, Z + 190), 94, 40, steps=8, top_r=10)
        g.box(STEEL, (210, 100, Z + 20), (130, 130, 40))
        pennant(sc, g, Wd / 2.0 - 10, yf + 10, Z, Z + 330, TEAL, phase=0.3)
        z2 = floors[1]
        g.box(DARK, (160, yf - 150, z2 + 200), (250, 4, 4))                    # the washing line
        g.box(WHITE, (36, yf - 150, z2 + 150), (5, 5, 110))
        g.box(WHITE, (284, yf - 150, z2 + 150), (5, 5, 110))
        w = sc.part(P.motion(swing=14.0, pivot=(160.0, yf - 150.0, z2 + 200.0), axis=0.0, rate=0.9, phase=0.13))
        for k, c in enumerate((RED, WHITE, SKY, YEL)):
            w.box(c, (70 + k * 60, yf - 150, z2 + 200 - (36 if k % 2 else 46)), (50, 4, 72 if k % 2 else 92))
        row(sc, r, 3, -280, 280, yf - 100, floors[0], detail=1, start=0, kid_at=(1,), flag_at=(2,), cols=(HOTPINK, YEL))
        row(sc, r, 2, -280, 20, yf - 100, floors[1], detail=0, start=2)
        row(sc, r, 3, -280, 280, yf - 100, floors[2], detail=0, start=4, flag_at=(0,), cols=(YEL, TEAL))
        row(sc, r, 2, -40, 300, yf + 50, Z, detail=0, start=1)
    return build


JOBS = [
    ('SM_P3D_RoofParty_A', roof_party(11, 0)),
    ('SM_P3D_RoofParty_B', roof_party(12, 1)),
    ('SM_P3D_FootBridge_A', foot_bridge(21, 0)),
    ('SM_P3D_FootBridge_B', foot_bridge(22, 1)),
    ('SM_P3D_JudgeTower', judge_tower(31)),
    ('SM_P3D_Lookout', lookout(41)),
    ('SM_P3D_SignPainters', sign_painters(51)),
    ('SM_P3D_ScissorLift', scissor_lift(61)),
    ('SM_P3D_ClimbWall', climb_wall(71)),
    ('SM_P3D_SkyBikes', sky_bikes(81)),
    ('SM_P3D_BalconyBlock', balcony_block(91)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
