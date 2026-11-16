"""FULL THROTTLE (Unreal) the Volcano's FAR LIFE AND CREATURES (level 3, Mount Kaldera): /Game/People/Volcano/SM_Far*.

The extra worker's kit for everything living from 150 m to the horizon (the main kit, ft_people_volcano.py, has the
street-level cast, the herd at 130 m, the caravan at 200 m, the camp at 232 m and the loose flocks over the road).
This kit stays out of those bands and adds what they do not have:

  SM_FarSkein_A / _B   birds fleeing the ash in LONG SKEINS, high and far: a strict V of cranes (A), a double string of
                       herons going the other way (B). Birds 14 to 17 m across so that a skein reads at 250 m and more.
  SM_FarKettle         a kettle of vultures wheeling over the slopes (they circle: the skeins travel).
  SM_FarLedgeGoats     a stepped basalt crag with goats five times life size on its ledges: some walk the ledges and come
                       round again, some leap on the column heads; the goatherd waves a cloth from the top.
  SM_FarRidgeLine      the villagers leaving along a ridge path: a causeway of basalt 8 m up (it stands on the skyline) and
                       along it ox wagons under bright tilts, people with bundles and banners, goats; five times life size.
  SM_FarRimSurvey      a survey team on a crater's rim: white coats and coloured hard hats, a theodolite, a striped
                       staff, a signal flag, an anemometer turning, a weather balloon on its line.

Every piece is saved with the always-moving master (far=True) and its colours carry a little steady light of their own
(the Volcano's slopes are dark: a crowd 200 m off is black on black without it; all of it stands 140 m and more out).

Run in the editor:  import ft_people_volcano_far as K; K.run()        K.run(['SM_FarSkein_A'])
Placed by the "far" block of Source/FullThrottle/FTPeopleL3.inl.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/Volcano'
BAS, BAS_L, BAS_D, ASHCAP = '#3e3244', '#5a4e5e', '#2c2433', '#b0a088'
WHITE, DARK, YEL, PINK, CYAN, LIME, VIOLET = '#fff6ea', '#22202a', '#ffc93c', '#ff5fa2', '#2ee6ff', '#b8ff5a', '#8f6bff'
WOOD = '#c9a070'
CLOTH = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a')


def tv(length, speed):
    """A travel for a part drawn by hand (figure() takes (length, cm/s) itself)."""
    return (float(length), float(speed) / float(length))


def lifted(fn, g=0.06):
    """A far piece's colours given a little steady light of their own (see the header)."""
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


# ------------------------------------------------------------------------------------------ birds
def far_bird(sc, x, y, z, heading, span, body, wing, tip, beak, common, flap=34.0, glow=0.3, neck=0.8, legs=None, dihedral=0.0):
    """A great bird at (x, y, z) flying along `heading` (degrees in the ground plane): body, a long neck, a beak, a
    tail, legs trailing, and two wings of two panels each (the outer one in the `tip` colour) that beat about its line
    of flight. common: the motion every part shares (phase, rate, bob, base, travel)."""
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    rx, ry = -ty, tx
    B = span * 0.42

    def at(f, s=0.0, u=0.0):
        return (x + tx * B * f + rx * s, y + ty * B * f + ry * s, z + B * u)

    b = sc.part(P.motion(pivot=(x, y, z), **common))
    b.base = None
    tilt_box(b, body, at(-0.5), at(0.5, 0, 0.06), B * 0.38, B * 0.36, glow=glow)
    tilt_box(b, body, at(0.4, 0, 0.05), at(0.5 + neck, 0, 0.2), B * 0.16, B * 0.16, glow=glow)
    tilt_box(b, beak, at(0.5 + neck, 0, 0.2), at(0.85 + neck, 0, 0.14), B * 0.11, B * 0.11, glow=glow)
    tilt_box(b, tip, at(-0.45), at(-0.95, 0, 0.03), B * 0.32, B * 0.07, glow=glow)
    if legs:
        tilt_box(b, legs, at(-0.5, 0, -0.05), at(-1.6, 0, -0.16), B * 0.08, B * 0.08, glow=glow)
    half = span * 0.5
    for side in (-1, 1):
        w = sc.part(P.motion(swing=side * flap, pivot=at(0, 0, 0.1), axis=heading, **common))
        w.base = None
        a = at(0.0, side * B * 0.12, 0.1)
        mid = at(0.0, side * half * 0.55, 0.24 + dihedral * 0.55)
        end = at(-0.2, side * half, 0.12 + dihedral)
        tilt_box(w, wing, a, mid, B * 0.5, max(B * 0.05, 5.0), glow=glow)
        tilt_box(w, tip, mid, end, B * 0.38, max(B * 0.05, 5.0), glow=glow)


def skein_v(sc):
    """Cranes in a strict V, the V stood on edge so that it reads as a V from the road (one arm climbs behind the
    leader, the other falls). Flies along +X and comes round."""
    r = random.Random(301)
    L, v = 36000.0, 2400.0
    z0 = 8200.0
    spots = [(0, 0)]
    for i in range(1, 7):
        spots.append((i, 1))
        spots.append((i, -1))
    for i, arm in spots:
        x = 5000.0 - i * 1700.0
        y = arm * i * 420.0
        z = z0 + arm * i * 700.0
        common = dict(phase=(i * 0.13) % 1.0, rate=r.uniform(0.95, 1.1), bob=-60.0, base=(x, y), travel=tv(L, v))
        far_bird(sc, x, y, z, 0.0, r.uniform(1500.0, 1750.0), WHITE, '#f1e4c8', DARK, '#ff5fa2', common, flap=36.0, legs=DARK)


def skein_string(sc):
    """Herons in a long double string that rises and falls along its length, going the other way (-X)."""
    r = random.Random(302)
    L, v = 36000.0, -2000.0
    n = 13
    for i in range(n):
        x = -9600.0 + i * 1600.0 + r.uniform(-150, 150)
        row = i % 2
        y = (-1 if row else 1) * 420.0
        z = 6600.0 + 1100.0 * math.sin(i * 0.62) + (900.0 if row else 0.0)
        common = dict(phase=(i * 0.09) % 1.0, rate=r.uniform(0.8, 0.95), bob=-90.0, base=(x, y), travel=tv(L, v))
        far_bird(sc, x, y, z, 180.0, r.uniform(1400.0, 1650.0), '#e6fbff', '#57c7ff', '#3a5fa8', '#ffc93c', common, flap=30.0, neck=0.55, legs='#ffc93c')


def kettle(sc):
    """Vultures wheeling over the slopes: nine great birds on circles of their own round one thermal, wings held up
    in a shallow V and barely beating."""
    r = random.Random(303)
    for i in range(9):
        R = 1500.0 + (i % 3) * 1300.0 + r.uniform(-200, 200)
        a0 = math.radians(i * 137.0)
        z = 3600.0 + i * 620.0
        x, y = R * math.cos(a0), R * math.sin(a0)
        rate = (0.05 + 0.012 * (i % 4)) * (1 if i % 2 else -1)
        heading = math.degrees(a0) + (90.0 if rate > 0 else -90.0)
        common = dict(phase=r.random(), rate=0.45, bob=-40.0, base=(0.0, 0.0), travel=(-1.0, rate))
        far_bird(sc, x, y, z, heading, r.uniform(1400.0, 1700.0), '#e8c26a', '#fff6ea', '#5a4a3c', '#ff5fa2', common, flap=9.0, neck=0.3, dihedral=0.16)


# ------------------------------------------------------------------------------------------ goats on ledges
def goat(sc, r, x, y, z, yaw=0.0, s=4.0, body=None, **kw):
    P.quadruped(sc, x, y, z, yaw=yaw, s=s, length=80, height=62, girth=30, neck=22, neck_up=42, head=26, horns='#ffc93c',
                body=body or r.choice((WHITE, WHITE, '#f1e4c8', '#cfcfd8')), tail=12, tail_col='#5a4a3c',
                legs_col='#5a4a3c', snout='#f6d3b4', ears='#5a4a3c', **kw)


def column(g, x, y, rad, top, k=0, root=-400.0, cap=True):
    g.cyl((BAS, BAS_L, BAS_D)[k % 3], (x, y, root), rad, top - root, steps=6)
    if cap:
        g.cyl(ASHCAP, (x, y, top), rad * 0.92, 10, steps=6)


def ledge_goats(sc):
    """A stepped basalt crag, 48 m long and 34 m tall: two ledges that goats walk (opposite ways, coming round again)
    and column heads behind them where goats leap; the goatherd waves a cloth from the highest head."""
    r = random.Random(311)
    g = sc.still()
    W = 4800.0
    T1, T2 = 800.0, 1800.0
    n = 8
    for k in range(n):                                    # the two ledges: a row of columns each, their heads level
        x = -W / 2.0 + (k + 0.5) * W / n
        column(g, x, -820.0, 330.0, T1, k, cap=False)
        column(g, x + 150.0, -300.0, 330.0, T2, k + 1, cap=False)
    g.box(ASHCAP, (0, -820.0, T1 + 6), (W, 520.0, 12))
    g.box(ASHCAP, (0, -300.0, T2 + 6), (W, 520.0, 12))
    heads = []
    for k in range(7):                                    # the heads behind: ragged, the tallest in the middle
        x = -W / 2.0 + 400.0 + k * (W - 800.0) / 6.0
        top = 2500.0 + 900.0 * math.sin(math.pi * (k + 0.5) / 7.0) + r.uniform(-150, 150)
        column(g, x, 300.0, 400.0, top, k + 2)
        heads.append((x, top))
    for k in range(5):                                    # stubs in front of the foot
        column(g, -W / 2.0 + 500.0 + k * 950.0 + r.uniform(-120, 120), -1300.0, 260.0, r.uniform(250.0, 520.0), k, cap=False)
    Lw = W - 300.0
    for i in range(3):                                    # walkers on the low ledge go +X
        goat(sc, r, -W / 2.0 + 500 + i * 1500, -860.0, T1 + 12, yaw=90, s=5.0, stride=26, wag=30, rate=1.5, bob=18.0, phase=r.random(), travel=(Lw, 330.0))
    for i in range(2):                                    # and on the high ledge -X
        goat(sc, r, -W / 2.0 + 1300 + i * 2100, -340.0, T2 + 12, yaw=-90, s=5.0, stride=26, wag=30, rate=1.4, bob=18.0, phase=r.random(), travel=(Lw, -280.0))
    for i, k in enumerate((0, 2, 4, 6)):                  # leapers on the heads
        x, top = heads[k]
        goat(sc, r, x, 260.0, top + 10, yaw=(-70, 60, -40, 110)[i], s=5.2, bob=320.0, rate=0.55 + 0.07 * i, phase=r.random(), wag=40)
    x, top = heads[3]
    P.figure(sc, x, 240.0, top + 10, yaw=0, s=5.0, arm_r='up', swing_r=30, arm_l='hip', rate=0.9, bob=20, detail=1, phase=0.3,
             hold_r=P.hold_flag(col=PINK, w=120.0), top=CYAN, bottom='#2b2350', hat='sun', hat_col=YEL, skin=P.SKINS[3], hair='long', wear='coat')


# ------------------------------------------------------------------------------------------ the ridge path
def hold_bundle(col, tie=WHITE):
    def f(m, h, side):
        m.sphere(col, (0, 0, 196), 30, scale=(1.3, 1.1, 0.8), steps=(4, 7))
        m.box(tie, (0, 0, 196), (10, 68, 50))
    return f


def villager(r, **fixed):
    d = P.look(r, tops=CLOTH, wear=r.choice(('trousers', 'dress', 'skirt', 'coat', 'trousers')),
               hat=r.choice((None, 'sun', 'cap', 'beanie')), hat_col=r.choice(CLOTH), sleeves=r.choice(('short', 'long')))
    d.update(fixed)
    return d


def wagon(sc, r, x, y, z, L, v, S, tilt_col, stripe):
    """An ox wagon under a bright tilt, travelling along +X behind its ox (the ox is drawn here too)."""
    m = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, z + 100 * S)), (x, y, z), 0.0, S)
    m.box(WOOD, (0, 0, 78), (250, 110, 26))
    for sx in (-80, 80):
        for sy in (-1, 1):
            m.cyl(DARK, (sx, sy * 58 - 5, 46), 46, 10, rot=(0, 0, 90), steps=6)
    m.cyl(tilt_col, (-125, 0, 150), 74, 250, rot=(-90, 0, 0), steps=8)       # the tilt: a barrel lying along X
    for k in (-1, 1):
        m.cyl(stripe, (k * 62 - 20, 0, 150), 77, 40, rot=(-90, 0, 0), steps=8)
    m.box(WOOD, (170, 0, 70), (100, 8, 8))                                     # the pole to the ox
    P.quadruped(sc, x + 260 * S, y, z, yaw=90, s=S, length=190, height=125, girth=62, neck=34, neck_up=10, head=40, horns=WHITE,
                body=r.choice(('#c9a070', '#f1e4c8', '#a8703a')), tail=50, stride=16, rate=0.8, phase=r.random(), bob=4.0, travel=(L, v))


def ridge_line(sc):
    """The villagers leaving along a ridge path, seen on the skyline: five times life size on a causeway 8 m up and
    144 m long. Ox wagons under bright tilts, people with bundles and banners, children waving, goats."""
    r = random.Random(321)
    S = 5.0
    kinds = ['flag', 'w', 'wagon', 'w', 'goats', 'flag', 'wagon', 'w', 'goats', 'w']
    room = dict(flag=600.0, w=1000.0, wagon=2800.0, goats=1700.0)
    L, v = sum(room[k] for k in kinds) + 200.0, 320.0
    W, TOP = L - 200.0, 800.0
    g = sc.still()
    g.box(BAS, (0, 0, (TOP - 700.0) / 2.0), (W, 620.0, TOP + 700.0))
    g.box(ASHCAP, (0, 0, TOP + 5), (W + 200, 660, 14))
    for k in range(7):
        column(g, -W / 2.0 + (k + 0.5) * W / 7.0, -380.0 + 60.0 * math.sin(k * 1.7), 380.0, TOP - r.uniform(120, 420), k, root=-700.0, cap=False)
    z = TOP + 12
    tilts = ((PINK, WHITE), (CYAN, YEL))
    wk = 0
    x = -L / 2.0 + 100.0
    for kind in kinds:
        pose = dict(stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=0.8, bob=10.0, yaw=90, detail=0, phase=r.random(), travel=(L, v))
        if kind == 'wagon':
            wagon(sc, r, x + 700.0, 0.0, z, L, v, S, *tilts[wk % 2])
            wk += 1
        elif kind == 'goats':
            for j in range(2):
                goat(sc, r, x + 350 + j * 700, (-60, 70)[j], z, yaw=90, s=S, stride=26, wag=30, rate=1.6, bob=16.0, phase=r.random(), travel=(L, v))
        elif kind == 'flag':
            pose.update(arm_r='up', swing_r=0, hold_r=P.hold_flag(col=r.choice((PINK, YEL, CYAN)), w=120.0))
            P.figure(sc, x + 300, 0.0, z, **dict(pose, **villager(r, s=S)))
        else:
            pose.update(arm_r='up', swing_r=0, hold_r=hold_bundle(r.choice(CLOTH)))
            P.figure(sc, x + 250, 0.0, z, **dict(pose, **villager(r, s=S)))
            if wk == 1:
                pose2 = dict(pose, phase=r.random(), arm_r='wave', swing_r=24, axis_r='y', hold_r=None)
                P.figure(sc, x + 720, 40.0, z, **dict(pose2, **villager(r, build='kid', s=S * 0.68)))
        x += room[kind]


# ------------------------------------------------------------------------------------------ the survey on a crater's rim
def sci(r, S, hat_col):
    return P.look(r, top=WHITE, bottom='#3a5fa8', wear='coat', sleeves='long', hat='hard', hat_col=hat_col, s=S)


def rim_survey(sc):
    """A small crater's cone, 15 m tall, and a survey team on its rim at 4.2 times life size: a theodolite panned to
    and fro, a striped staff held up, a signal flag waved, an anemometer turning, a weather balloon on its line."""
    r = random.Random(331)
    S = 4.2
    g = sc.still()
    H = 1500.0
    g.cyl(BAS, (0, 0, -500), 3000.0, H + 500, steps=10, top_r=2300.0)
    g.cyl(ASHCAP, (0, 0, H), 2300.0, 12, steps=10)
    g.cyl(BAS_D, (0, 500, H + 12), 1400.0, 6, steps=10)                       # the crater's mouth
    for k in range(6):
        a = math.radians(205 + k * 26)
        g.cyl(BAS_L, (2800 * math.cos(a), 2800 * math.sin(a), -500), 380, 500 + r.uniform(300, 800), steps=6)
    z = H + 12
    y = -1500.0
    # the theodolite on its tripod and its operator panning it
    tx = -1050.0
    for a in (90, 210, 330):
        tilt_box(g, YEL, (tx + 220 * math.cos(math.radians(a)), y + 220 * math.sin(math.radians(a)), z), (tx, y, z + 520), 30, 30)
    th = sc.part(P.motion(swing=24, pivot=(tx, y, z + 560), axis=P.UP, rate=0.25, phase=0.1))
    th.box(DARK, (tx, y, z + 580), (200, 120, 120))
    th.box(CYAN, (tx, y - 80, z + 580), (70, 50, 70))
    P.figure(sc, tx + 60, y + 330, z, yaw=0, arm_l='hold', arm_r='face', detail=1, rock=3, rate=0.25, phase=0.1, **sci(r, S, YEL))

    def staff(m, h, side):
        for k in range(6):
            m.box(PINK if k % 2 else WHITE, (h[0], h[1] - 4, h[2] - 60 + k * 40), (10, 10, 40))
    P.figure(sc, -300.0, y + 60, z, yaw=20, arm_r=((36, 70), (72, 84)), hold_r=staff, detail=1, rock=2, rate=0.4, phase=0.5, **sci(r, S, PINK))
    P.figure(sc, 420.0, y, z, yaw=-10, arm_r='up', swing_r=34, arm_l='hip', hold_r=P.hold_flag(col=PINK, w=130.0), rate=0.9, bob=16, detail=1,
             phase=0.2, **sci(r, S, CYAN))
    P.figure(sc, 1050.0, y + 40, z, yaw=-25, arm_r='point', arm_l='face', swing_r=10, rate=0.7, bob=12, detail=1, phase=0.7, **sci(r, S, LIME))
    # the anemometer: a mast and three cups that circle it
    ax, ay = 1700.0, y + 250.0
    g.box('#8f9bb3', (ax, ay, z + 500), (30, 30, 1000))
    for k in range(3):
        a = math.radians(k * 120)
        cup = sc.part(P.motion(base=(ax, ay), travel=(-1.0, 0.5), pivot=(ax, ay, z + 1000)))
        tilt_box(cup, '#8f9bb3', (ax, ay, z + 990), (ax + 260 * math.cos(a), ay + 260 * math.sin(a), z + 990), 18, 18)
        cup.sphere(YEL, (ax + 290 * math.cos(a), ay + 290 * math.sin(a), z + 990), 80, steps=(3, 6))
    # the balloon on its line
    bx, by = -1850.0, y + 300.0
    g.box('#8f9bb3', (bx, by, z + 50), (150, 150, 100))
    bl = sc.part(P.motion(swing=5, pivot=(bx, by, z + 60), axis=90.0, rate=0.22, phase=0.4))
    tilt_box(bl, WHITE, (bx, by, z + 100), (bx, by, z + 1900), 10, 10)
    bl.sphere(WHITE, (bx, by, z + 2250), 380, steps=(5, 8))
    bl.cyl(PINK, (bx, by, z + 2190), 388, 120, steps=8)
    bl.box(CYAN, (bx, by, z + 1830), (90, 90, 90))


JOBS = [
    ('SM_FarSkein_A', lifted(skein_v, 0.3), dict(far=True, grow=(15000.0, 200.0, 300.0), grow_down=(15000.0, 200.0, 300.0))),
    ('SM_FarSkein_B', lifted(skein_string, 0.3), dict(far=True, grow=(11000.0, 200.0, 300.0), grow_down=(11000.0, 200.0, 300.0))),
    ('SM_FarKettle', lifted(kettle, 0.3), dict(far=True, grow=(5000.0, 5000.0, 300.0), grow_down=(5000.0, 5000.0, 300.0))),
    ('SM_FarLedgeGoats', lifted(ledge_goats, 0.1), dict(far=True, grow=(400.0, 100.0, 400.0), grow_down=(400.0, 100.0, 0.0))),
    ('SM_FarRidgeLine', lifted(ridge_line), dict(far=True, grow=(400.0, 100.0, 100.0), grow_down=(400.0, 100.0, 0.0))),
    ('SM_FarRimSurvey', lifted(rim_survey), dict(far=True, grow=(300.0, 300.0, 300.0), grow_down=(300.0, 300.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
