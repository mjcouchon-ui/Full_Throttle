"""FULL THROTTLE (Unreal) the population, level 2: the Volta Chemical Works (/Game/People/ChemicalWorks/).

The night shift. The works' road runs in a trench of its own making: a metre of barrier, then the kerb and its
stores (17.3 to 23 m), then pipe racks 5 to 7 m tall or tank rows to 11 m at the sight line (24.7 m on). A person
on the ground is not seen from the road at all. So this cast is RAISED on its own steel (scaffolds, watch towers,
loading docks, bund walls), MASSED, dressed in what reads against navy concrete at night (hi-vis orange and lime,
white and yellow suits, the guards' red), and every piece carries its own work lamp where the rules let it (a
steady light, 24 m or more from the centre line).

Placed by Source/FullThrottle/FTPeopleL1.inl. Run in the editor:  import ft_people_chem; ft_people_chem.run()
A piece is drawn like every prop: cm; it stands on z = 0; its FRONT faces -Y (the road); long pieces run along X.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/ChemicalWorks'

# the works' own palette (ft_chem / ft_dress_chem), and what the shift wears
STEELB, STEELB_D, TEAL, HAZ, NAVY, MAG, VIOLET, WHITE = '#2a3f8f', '#1c2a66', '#19b5c9', '#ffd23f', '#1c1f4a', '#e84aff', '#b44dff', '#eef6ff'
PAVE, PAVE_D, RUST, DARK = '#7f8fc6', '#56659f', '#a8552f', '#22202a'
POLE = '#aab6e0'          # scaffold tube: pale, so that the steel a crew stands on reads against the night
BOARD = '#d9b98a'         # scaffold boards
ORANGE, LIME, YEL, RED, PINK = '#ff7a3c', '#b8ff5a', '#ffc93c', '#ff2d3c', '#ff5fa2'
LAMP = '#fff1c0'
HIVIS = (ORANGE, ORANGE, LIME, YEL, '#fff6ea')
HATS = (YEL, '#fff6ea', ORANGE, '#2ee6ff', RED)


def worker(r, **fixed):
    """One of the shift: long sleeves in hi-vis, dark trousers, a hard hat, a reflective band."""
    top = r.choice(HIVIS)
    d = P.look(r, tops=HIVIS, bottoms=('#2b2350', '#3a5fa8', DARK, '#5a4a3c'), sleeves='long', wear='trousers', hat='hard',
               hat_col=r.choice(HATS), band=('#fff6ea' if top != '#fff6ea' else ORANGE))
    d['top'] = top
    d['s'] = r.uniform(0.9, 1.0)
    d.update(fixed)
    return d


def suit(r, col=None, **fixed):
    """A chemical suit: one colour head to foot, a hood (a helmet with a dark visor), an air pack."""
    col = col or r.choice(('#fff6ea', YEL, '#fff6ea', LIME))
    d = P.look(r, sleeves='long', wear='trousers', hat='helmet', hat_col=col, band=ORANGE, pack=PAVE)
    d.update(top=col, bottom=col, shoes=DARK, s=r.uniform(0.92, 1.0))
    d.update(fixed)
    return d


def guard(r, **fixed):
    d = P.look(r, sleeves='long', wear='trousers', hat='peak', hat_col=RED, band='#fff6ea')
    d.update(top=RED, bottom=DARK, shoes=DARK, s=r.uniform(0.95, 1.02))
    d.update(fixed)
    return d


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=8, rate=1.5),
    dict(arm_r='wave', swing_r=28, rate=1.3, bob=4),
    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=8, rate=1.3),
    dict(arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.7),
    dict(arm_l='wave', arm_r='wave', swing_l=-24, swing_r=24, rate=1.1, bob=5),
    dict(arm_l='hold', arm_r='hold', swing_l=14, swing_r=-14, axis_l='z', axis_r='z', rate=2.4),     # clapping
)


# ------------------------------------------------------------------------------------------ steel to stand on
def scaffold(g, L, D, decks, x0=0.0, y0=0.0, bays=None, col=POLE, rail=True, lamp=0.0):
    """A tube scaffold L long (X) and D deep (Y) round (x0, y0), boarded at the heights in `decks`. Standards 9 cm
    thick (the cel pass inks every edge: nothing thin), a guard rail at the back and the ends of every deck and a
    single one at the front, toe boards in hazard yellow, a diagonal in every other bay. lamp: how far the work lamps
    over the top deck glow (0 = none: nearer than 24 m)."""
    bays = bays or max(int(round(L / 300.0)), 1)
    top = decks[-1] + 110.0
    for k in range(bays + 1):
        x = x0 - L / 2.0 + L * k / bays
        for sy in (-1, 1):
            g.box(col, (x, y0 + sy * D / 2.0, top / 2.0), (9, 9, top))
            g.box(NAVY, (x, y0 + sy * D / 2.0, 3), (34, 34, 6))
    for z in decks:
        g.box(BOARD, (x0, y0, z - 6), (L + 20, D + 10, 10))
        g.box(HAZ, (x0, y0 - D / 2.0 - 4, z + 8), (L + 20, 4, 18))
        if rail:
            g.box(col, (x0, y0 + D / 2.0, z + 104), (L, 8, 8))
            g.box(col, (x0, y0 + D / 2.0, z + 54), (L, 7, 7))
            g.box(col, (x0, y0 - D / 2.0, z + 100), (L, 8, 8))
            for sx in (-1, 1):
                g.box(col, (x0 + sx * L / 2.0, y0, z + 104), (8, D, 8))
    for k in range(0, bays, 2):                                         # a diagonal on the back face
        xa = x0 - L / 2.0 + L * k / bays
        xb = x0 - L / 2.0 + L * (k + 1) / bays
        tilt_box(g, col, (xa, y0 + D / 2.0 + 6, 20), (xb, y0 + D / 2.0 + 6, decks[0] - 20), 8, 8)
    if lamp:
        for sx in (-1, 1):
            x = x0 + sx * (L / 2.0 - 40)
            g.box(col, (x, y0 + D / 2.0, top + 60), (8, 8, 120))
            g.box(DARK, (x, y0 + D / 2.0 - 14, top + 122), (60, 40, 26))
            g.box(LAMP, (x, y0 + D / 2.0 - 30, top + 108), (52, 26, 5), glow=lamp)
    return top


def ladder(g, x, y, z0, z1, col=HAZ):
    for s in (-1, 1):
        g.box(col, (x + s * 22, y, (z0 + z1) / 2.0), (7, 7, z1 - z0))
    k = z0 + 40
    while k < z1:
        g.box(col, (x, y, k), (44, 6, 6))
        k += 80


def hold_torch(glow=0.0):
    """A marshal's wand: a lit baton (glow only on a piece 24 m or more from the road)."""
    def f(m, h, side):
        m.box(DARK, (h[0], h[1], h[2] + 4), (6, 6, 16))
        m.box(ORANGE, (h[0], h[1], h[2] + 32), (8, 8, 44), glow=glow)
    return f


def hold_canister(col=VIOLET, glow=0.0):
    def f(m, h, side):
        m.cyl('#fff6ea', (h[0], h[1], h[2] - 30), 11, 30, steps=6)
        m.cyl(col, (h[0], h[1], h[2] - 26), 12, 14, steps=6, glow=glow)
        m.box(DARK, (h[0], h[1], h[2] - 2), (4, 4, 8))
    return f


# ------------------------------------------------------------------------------------------ the pieces
BIG = 1.35    # people on raised steel 24 m and more from the road are drawn a third over life size: they must read at 120 mph


def sheet(g, x, y, z, L, H, col=WHITE, stripe=ORANGE, glow=0.18):
    """Scaffold sheeting behind a deck, lit by the work lamps: the pale ground a crew's colours read against."""
    g.box(col, (x, y, z + H / 2.0), (L, 5, H), glow=glow)
    g.box(stripe, (x, y - 3.5, z + H - 22), (L, 2, 16))


def gantry(seed=1, L=900.0, decks=(430.0, 760.0), n=(4, 3), lamp=0.8, weld=False):
    """The shift on a sheeted scaffold: workers in hi-vis who stop work to watch the chase: waving, pointing,
    cheering, clapping, one still at it with a wrench (or a welder, his arc a steady point of light). Its own work
    lamps over the top deck. TALL: 24 m and more from the centre line."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        D = 200.0
        scaffold(g, L, D, decks, lamp=lamp)
        ladder(g, L / 2.0 - 60, -D / 2.0 - 6, 0, decks[0])
        if len(decks) > 1:
            ladder(g, -L / 2.0 + 60, -D / 2.0 - 6, decks[0], decks[-1])
        for z in decks:
            sheet(g, 0, D / 2.0 + 8, z + 4, L - 20, 236)
        for di, z in enumerate(decks):
            m = n[di]
            for i in range(m):
                x = -L / 2.0 + 120 + (L - 240) * (i + 0.5) / m + r.uniform(-20, 20)
                d = worker(r)
                d.update(CHEER[(i * 2 + di * 3 + seed) % len(CHEER)])
                d['s'] = BIG * r.uniform(0.95, 1.03)
                if di == 0 and i == m - 1:
                    if weld:
                        d.update(arm_r='fwd', arm_l='fwd', swing_r=0, swing_l=0, rate=0.4, bob=-1.5, hat='helmet', hat_col=DARK)
                        g.box('#9ff0ff', (x, -88, z + 150), (12, 12, 12), glow=1.0)
                    else:
                        d.update(arm_r='fwd', swing_r=18, axis_r='x', arm_l='hip', swing_l=0, hold_r=P.hold_tool(), rate=1.4, bob=0)
                P.figure(sc, x, -10 + r.uniform(-20, 20), z=z, yaw=r.uniform(-20, 20), phase=r.random(), react=True, detail=1, **d)
        g.cyl(TEAL, (-L / 2.0 + 50, 40, decks[0]), 28, 86, steps=8)
    return build


def watch_tower(seed=5, H=1150.0):
    """A watch tower over the tank rows: four raking legs, a railed deck between two lamps, and
    on it a guard waving the chase through with a lit wand, a worker with a pink flag, another cheering. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        W, Dp = 420.0, 230.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                tilt_box(g, POLE, (sx * (W / 2.0 + 60), sy * (Dp / 2.0 + 10), 0), (sx * W / 2.0, sy * Dp / 2.0, H), 16, 16)
        for k in range(3):
            z0, z1 = H * k / 3.0, H * (k + 1) / 3.0
            sgn = 1 if k % 2 else -1
            tilt_box(g, POLE, (-sgn * W / 2.0, -Dp / 2.0, z0 + 20), (sgn * W / 2.0, -Dp / 2.0, z1 - 20), 9, 9)
            g.box(POLE, (0, -Dp / 2.0, z1), (W + 40, 9, 9))
            g.box(POLE, (0, Dp / 2.0, z1), (W + 40, 9, 9))
        g.box(BOARD, (0, 0, H - 6), (W + 40, Dp + 30, 12))
        g.box(HAZ, (0, -Dp / 2.0 - 16, H + 8), (W + 40, 4, 20))
        g.box(POLE, (0, -Dp / 2.0 - 10, H + 100), (W + 30, 8, 8))
        sheet(g, 0, Dp / 2.0 + 10, H, W + 30, 250)
        for sx in (-1, 1):
            g.box(POLE, (sx * (W / 2.0 + 14), Dp / 2.0 + 6, H + 160), (9, 9, 320))
            g.box(POLE, (sx * (W / 2.0 + 14), -Dp / 2.0 - 10, H + 160), (9, 9, 320))
        for sx in (-1, 1):                                              # no roof (it would shade them): a lamp on each back post
            g.box(DARK, (sx * (W / 2.0 + 14), Dp / 2.0 - 20, H + 326), (60, 70, 22))
            g.box(LAMP, (sx * (W / 2.0 + 14), Dp / 2.0 - 30, H + 312), (50, 50, 5), glow=0.9)
        ladder(g, W / 2.0 + 40, 0, 0, H, col=POLE)
        s = 1.4
        P.figure(sc, -130, -20, z=H, yaw=10, arm_r='wave', swing_r=30, hold_r=hold_torch(glow=1.0), arm_l='hip', rate=1.1, phase=r.random(),
                 react=True, detail=1, **guard(r, s=s))
        P.figure(sc, 0, 10, z=H, yaw=-8, arm_l='up', swing_l=20, hold_l=P.hold_flag(col=PINK, w=90), arm_r='point', rate=0.9, phase=r.random(),
                 react=True, detail=1, **worker(r, s=s))
        P.figure(sc, 135, -20, z=H, yaw=-14, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=9, rate=1.4, phase=r.random(), react=True,
                 detail=1, **worker(r, s=s * 0.95))
    return build


def trestles(g, L, y=-36, n=4):
    for k in range(n):
        x = -L / 2.0 + (k + 0.5) * L / n
        w = L / n - 60
        for sx in (-1, 1):
            g.box(PAVE, (x + sx * (w / 2.0 - 10), y + 2, 45), (8, 30, 90))
        for j in range(4):
            g.box(RED if j % 2 else '#fff6ea', (x - w / 2.0 + (j + 0.5) * w / 4.0, y, 78), (w / 4.0, 6, 20))


def kerb_crew(seed=2, n=8, L=1000.0, lunch=False):
    """The shift at the works' kerb, behind its own red-and-white trestles: a line of workers cheering the chase on
    (lunch=True: half of them sitting on crates with their cups, on their break). LOW: nothing over 2 m. The trestles stand in front; the people (who carry the little light every figure of this kit
    has) stand behind the piece's own middle, which the table puts 24.1 m out."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        trestles(g, L, y=-50)
        for i in range(n):
            x = -L / 2.0 + (i + 0.5) * L / n + r.uniform(-18, 18)
            d = worker(r, s=r.uniform(0.86, 0.94))
            if lunch and i % 2 == 0:
                g.box(RUST if i % 4 else TEAL, (x, 34, 23), (64, 54, 46))
                P.figure(sc, x, 30, legs='sit', seat=46, arm_r='wave', swing_r=24, arm_l='hold', hold_l=P.hold_cup(), rate=1.0, phase=r.random(),
                         react=True, **d)
                continue
            d.update(CHEER[(i * 5 + seed) % len(CHEER)])
            d['bob'] = min(d.get('bob', 0), 7)
            P.figure(sc, x, 22 + (16 if i % 2 else -6), yaw=r.uniform(-16, 16), phase=r.random(), react=True, **d)
    return build


def dock_crowd(seed=3, L=1200.0, n=14, suits=False):
    """Shift change on a loading platform: a concrete deck on piers, 4.3 m up (from this road nothing under 3 m shows
    at the sight line), with a hazard-striped lip, the shift massed along it two deep, hopping and waving out of
    step, two sitting on the edge with their legs dangling, a lamp post at each end (suits=True: half of them the
    waste crews in their chemical suits, against dark sheeting). TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, D = 430.0, 230.0
        g.box(PAVE, (0, 0, H - 25), (L, D, 50))
        for k in range(4):
            x = -L / 2.0 + 90 + k * (L - 180) / 3.0
            g.box(PAVE_D, (x, 20, (H - 50) / 2.0), (70, 110, H - 50))
        tilt_box(g, PAVE_D, (L / 2.0 + 60, -60, 0), (L / 2.0 + 60, 90, H - 20), 90, 12)            # the stair up the end
        nb = 8
        for k in range(nb):
            g.box(HAZ if k % 2 else NAVY, (-L / 2.0 + (k + 0.5) * L / nb, -D / 2.0 - 2, H - 16), (L / nb, 5, 32))
        for sx in (-1, 1):                                              # the lamp posts (steady: 24 m and more)
            g.box(POLE, (sx * (L / 2.0 - 20), D / 2.0 - 14, H + 200), (10, 10, 400))
            g.box(LAMP, (sx * (L / 2.0 - 20), D / 2.0 - 50, H + 392), (40, 70, 8), glow=0.9)
        sheet(g, 0, D / 2.0 - 4, H, L - 60, 270, col=(WHITE if not suits else '#3a2a7a'), stripe=(ORANGE if not suits else VIOLET),
              glow=(0.18 if not suits else 0.0))
        for i in range(n):
            x = -L / 2.0 + 130 + (i + 0.5) * (L - 260) / n + r.uniform(-12, 12)
            back = i % 2
            d = suit(r) if (suits and i % 3 != 1) else worker(r)
            d.update(CHEER[(i * 5 + seed) % len(CHEER)])
            d['s'] = 1.25 * r.uniform(0.95, 1.04)
            P.figure(sc, x, (50 if back else -50) + r.uniform(-10, 10), z=H, yaw=r.uniform(-18, 18), phase=r.random(), react=True,
                     detail=(0 if back else 1), **d)
        for sx in (-1, 1):                                              # two on the edge, legs over the lip
            P.figure(sc, sx * (L / 2.0 - 60), -D / 2.0 + 20, z=H - 46, legs='sit', seat=46, arm_r='wave', swing_r=22, rate=1.0,
                     phase=r.random(), detail=1, **worker(r))
    return build


def hazmat(seed=6, L=1400.0):
    """The waste crew: a file of figures in chemical suits crossing a pipe bridge, 4.4 m up, with canisters of the
    violet stuff (it glows: the piece stands 24 m and more out), one at the end waving them on with a lit wand, a
    drum of it on the deck. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H, D = 440.0, 180.0
        for k in range(4):
            x = -L / 2.0 + 60 + k * (L - 120) / 3.0
            for sy in (-1, 1):
                g.box(PAVE_D, (x, sy * D / 2.0, H / 2.0), (22, 22, H))
            g.box(PAVE_D, (x, 0, H - 60), (22, D, 22))
        g.box(PAVE, (0, 0, H - 8), (L + 16, D + 16, 16))
        g.cyl(VIOLET, (-L / 2.0, 0, H - 60), 26, L, rot=(-90, 0, 0), steps=8, glow=0.35)             # the line under the deck
        for k in range(10):
            g.box(HAZ if k % 2 else VIOLET, (-L / 2.0 + (k + 0.5) * L / 10.0, -D / 2.0 - 10, H + 10), (L / 10.0, 5, 24))
        g.box(POLE, (0, D / 2.0 + 4, H + 105), (L, 8, 8))
        sheet(g, 0, D / 2.0 + 8, H + 4, L - 20, 250, col='#3a2a7a', stripe=VIOLET, glow=0.0)
        for i in range(5):
            x = -L / 2.0 + (i + 0.5) * L / 5.0
            P.figure(sc, x, 10, z=H, yaw=90, stride=24, swing_l=16, axis_l='x', arm_r='down', hold_r=hold_canister(glow=0.9), rate=0.9, bob=3,
                     phase=i * 0.37, travel=(L, 95), detail=1, **suit(r, s=BIG))
        g.cyl('#fff6ea', (L / 2.0 - 70, 40, H), 34, 96, steps=8)
        g.cyl(VIOLET, (L / 2.0 - 70, 40, H + 34), 36, 30, steps=8, glow=0.8)
        P.figure(sc, -L / 2.0 + 70, -40, z=H, yaw=20, arm_r='wave', swing_r=30, hold_r=hold_torch(glow=1.0), arm_l='point', rate=1.0,
                 phase=0.3, react=True, detail=1, **suit(r, col=YEL, s=BIG))
    return build


def guards(seed=7):
    """The gate post: a red cabin with a lit window on a concrete plinth, 4.2 m up, its balcony towards the road; a
    red-and-white signal arm that rises and falls; on the balcony a guard waving the chase through with a lit wand,
    another with a hand up, and the dog. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 420.0
        g.box(PAVE_D, (0, 30, (H - 30) / 2.0), (300, 200, H - 30))
        g.box(PAVE, (0, 0, H - 15), (760, 250, 30))
        for k in range(8):
            g.box(RED if k % 2 else '#fff6ea', (-380 + (k + 0.5) * 95, -127, H - 14), (95, 5, 28))
        g.box(POLE, (0, -118, H + 100), (750, 8, 8))
        for k in range(5):
            g.box(POLE, (-371 + k * 185.5, -118, H + 50), (8, 8, 100))
        g.box(RED, (-250, 50, H + 130), (200, 140, 260))
        for k in range(3):
            g.box('#fff6ea', (-250, -21, H + 40 + k * 90), (202, 2, 30))
        g.box(LAMP, (-250, -22, H + 170), (100, 3, 60), glow=0.6)
        g.box(NAVY, (-250, 40, H + 268), (250, 200, 16))
        g.box(LAMP, (-250, -50, H + 258), (60, 30, 5), glow=0.9)
        sheet(g, 130, 122, H, 480, 250)
        g.box(PAVE, (360, 90, H + 150), (20, 20, 300))
        arm = sc.part(P.motion(swing=30.0, pivot=(360.0, 78.0, H + 290.0), axis=90.0, rate=0.14, phase=0.25))
        for k in range(6):
            arm.box(RED if k % 2 else '#fff6ea', (360 - 45 - k * 80, 78, H + 290), (80, 10, 22))
        s = BIG
        P.figure(sc, -30, -30, z=H, yaw=-10, arm_r='wave', swing_r=32, hold_r=hold_torch(glow=1.0), arm_l='hip', rate=1.2, phase=r.random(),
                 react=True, **guard(r, s=s))
        P.figure(sc, 140, 0, z=H, yaw=12, arm_l='out', arm_r='point', swing_r=12, axis_r='x', rate=0.6, phase=r.random(), **guard(r, s=s))
        P.quadruped(sc, 280, -30, z=H, yaw=20, s=1.3, length=80, height=56, girth=24, neck=18, neck_up=40, head=22, tail=26, wag=34, rate=2.6,
                    bob=3, body='#8e5a3a', snout=DARK, ears=DARK)
    return build


def patrol(seed=8, L=1800.0):
    """Security on its round: two guards walking the kerb line with the dog trotting ahead of them. LOW, unlit."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(PAVE_D, (0, 0, 2), (L, 110, 4))
        for k in range(6):
            g.box(HAZ, (-L / 2.0 + (k + 0.5) * L / 6.0, -52, 5), (L / 12.0, 6, 3))
        walk = dict(yaw=90, stride=26, swing_l=20, swing_r=-20, axis_l='x', axis_r='x', rate=1.0, bob=3, travel=(L, 120))
        P.figure(sc, -200, 36, phase=0.0, **dict(walk, **guard(r, s=0.95)))
        P.figure(sc, -90, 6, phase=0.5, **dict(walk, **guard(r, s=0.9)))
        P.quadruped(sc, 110, 20, yaw=90, length=80, height=56, girth=24, neck=18, neck_up=40, head=22, tail=26, wag=30, stride=24, rate=2.4,
                    bob=3, body='#5a4a3c', snout=DARK, ears=DARK, travel=(L, 120))
    return build


def boffins(seed=9, L=640.0, Z=440.0):
    """The Alchemist's laboratory staff on an observation deck: white coats against dark sheeting, a dish on a
    tripod that pans, one holding up a flask of the violet stuff (it glows), one with a clipboard, one filming. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        D = 200.0
        scaffold(g, L, D, (Z,), lamp=0.8, col=PAVE)
        sheet(g, 0, D / 2.0 + 8, Z + 4, L - 20, 236, col='#3a2a7a', stripe=VIOLET, glow=0.0)
        ladder(g, L / 2.0 - 50, -D / 2.0 - 6, 0, Z)

        def coat(**kw):
            d = P.look(r, sleeves='long', wear='coat', hair=r.choice(('short', 'long', 'bun', 'bald', 'afro')))
            d.update(top='#fff6ea', bottom=DARK, s=BIG * r.uniform(0.95, 1.03), band=None)
            d.update(kw)
            return d
        P.figure(sc, -220, -10, z=Z, yaw=8, arm_r='up', swing_r=10, hold_r=hold_canister(glow=1.0), arm_l='hip', rate=0.7, phase=r.random(),
                 detail=1, **coat())
        P.figure(sc, -90, 10, z=Z, yaw=-10, arm_l='hold', arm_r='point', swing_r=12, axis_r='x', hold_l=P.hold_bag(col=BOARD), rate=0.6,
                 phase=r.random(), react=True, detail=1, **coat(hat='hard', hat_col=WHITE))
        P.figure(sc, 40, -10, z=Z, yaw=6, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rate=0.3, bob=-1.5, phase=r.random(), detail=1, **coat())
        P.figure(sc, 240, 20, z=Z, yaw=-20, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=8, rate=1.4, phase=r.random(), react=True,
                 detail=1, **coat())
        # the dish on its tripod, panning
        for a in (90, 210, 330):
            rod(g, PAVE, (150, -10, Z + 120), (150 + math.cos(math.radians(a)) * 40, -10 + math.sin(math.radians(a)) * 40, Z), 4, steps=5)
        dish = sc.part(P.motion(swing=40.0, pivot=(150.0, -10.0, Z + 120.0), axis=P.UP, rate=0.1))
        dish.cyl(WHITE, (150, -30, Z + 140), 44, 10, rot=(0, 0, 90), steps=8, top_r=20)
        dish.box(VIOLET, (150, -44, Z + 140), (8, 20, 8), glow=0.8)
    return build


CROW, CROW_W = '#2a2230', '#4a3a8a'


def crows_perch(seed=10, L=800.0, Z=480.0):
    """Crows on a lamp-lit pipe rail: a row of them shuffling and hopping, three more circling over it. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 0, 1):
            g.box(POLE, (sx * (L / 2.0 - 20), 0, Z / 2.0), (12, 12, Z))
        g.cyl(WHITE, (-L / 2.0, 0, Z), 12, L, rot=(-90, 0, 0), steps=6, glow=0.15)
        sheet(g, 0, 30, Z - 150, L - 80, 130, glow=0.25)
        g.box(LAMP, (0, -6, Z + 190), (70, 30, 6), glow=0.9)
        g.box(POLE, (0, 0, Z + 100), (8, 8, 200))
        for i in range(8):
            x = -L / 2.0 + 60 + i * (L - 120) / 7.0 + r.uniform(-15, 15)
            P.bird(sc, x, 0, Z + 26, heading=r.choice((-90, -70, -110, 90)), span=120, body=CROW, wing=CROW_W, beak=YEL, phase=r.random(),
                   rate=r.uniform(0.8, 1.6), flap=10, bob=r.choice((5, 9, 14)), s=1.5)
        for i in range(3):
            a = i * 2.1
            P.bird(sc, math.cos(a) * 150, 90 + math.sin(a) * 150, Z + 260 + i * 70, heading=math.degrees(a) + 90, span=230, body=CROW, wing=CROW_W,
                   beak=YEL, phase=r.random(), rate=2.0, flap=34, orbit=(0.0, 90.0, 0.12))
    return build


def crow_flock(seed=4, n=10, span=700.0, z0=3000.0):
    """Crows wheeling over the far works against the violet of the horizon: huge (a far thing must be big to read:
    wings 7 m across), and always moving."""
    def build(sc):
        r = random.Random(seed)
        for i in range(n):
            a = r.uniform(0, 6.2832)
            rad = r.uniform(1500, 5000)
            P.bird(sc, math.cos(a) * rad, math.sin(a) * rad, z0 + r.uniform(0, 3500), heading=math.degrees(a) + 90.0,
                   span=span * r.uniform(0.75, 1.2), body=CROW, wing=CROW, beak=CROW, phase=r.random(), rate=r.uniform(0.9, 1.4), flap=34,
                   orbit=(0.0, 0.0, 0.02 + 0.006 * (i % 3)))
        sc.still().box(NAVY, (0, 0, 2), (40, 40, 4))
    return build


def far_catwalk(seed=11, L=4200.0, H=1500.0, n=30, s=2.4):
    """The far shift: a footbridge between two towers, 15 m up, its lit sheeting 40 m long, and the whole shift along
    it as a band of colour that hops out of step (each figure a block of hi-vis, a head and a hat, two and a half
    times life size: this is seen from 100 to 280 m). Always moving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 1):
            g.box(PAVE_D, (sx * (L / 2.0 - 150), 0, (H + 300) / 2.0), (300, 300, H + 300))
            g.box(LAMP, (sx * (L / 2.0 - 150), -152, H + 200), (160, 4, 90), glow=0.7)
        g.box(PAVE, (0, 0, H - 20), (L - 300, 260, 40))
        g.box(HAZ, (0, -132, H + 14), (L - 300, 5, 28))
        g.box(WHITE, (0, 120, H + 300), (L - 320, 6, 600), glow=0.25)
        g.box(ORANGE, (0, 116, H + 570), (L - 320, 3, 40), glow=0.3)
        for i in range(n):
            x = -L / 2.0 + 260 + (i + 0.5) * (L - 520) / n + r.uniform(-20, 20)
            y = -40 if i % 2 else 40
            m = sc.part(P.motion(phase=r.random(), rate=r.uniform(1.1, 1.7), bob=r.uniform(18, 34)), (x, y, H), 0.0, s)
            top = r.choice((ORANGE, ORANGE, LIME, YEL, '#fff6ea', PINK, '#2ee6ff'))
            m.box('#2b2350', (0, 0, 33), (34, 24, 66))
            m.box(top, (0, 0, 95), (48, 28, 58))
            m.box(r.choice(P.SKINS), (0, 0, 142), (36, 34, 36))
            m.box(r.choice(HATS), (0, 0, 166), (42, 40, 14))
            if i % 3 == 0:
                for sx in (-1, 1):
                    m.box(top, (sx * 32, 0, 150), (12, 14, 70))          # arms up over the head
    return build


def lit(fn, glow=0.06):
    """Wrap a piece: everything in it that moves (its people, their props, its creatures) is given a little light of
    its own, as hi-vis under the work lamps has. The works' night light leaves a figure on the shadow side of the
    road nearly black. A steady light: only for pieces that stand 24 m and more from the centre line."""
    def build(sc):
        fn(sc)
        for key in sc.order:
            model, mo = sc.parts[key]
            if not mo['rate']:
                continue
            out = {}
            for (col, gl), mesh in model.meshes.items():
                k2 = (col, max(gl, glow))
                if k2 in out:
                    unreal.GeometryScript_MeshEdits.append_mesh(out[k2], mesh, unreal.Transform())
                else:
                    out[k2] = mesh
            model.meshes = out
    return build


JOBS = [
    ('SM_Gantry_A', lit(gantry(1))),
    ('SM_Gantry_B', lit(gantry(12, L=1100.0, decks=(700.0,), n=(6,), weld=True))),
    ('SM_Gantry_C', lit(gantry(13, L=800.0, decks=(470.0, 800.0), n=(3, 4)))),
    ('SM_WatchTower', lit(watch_tower(5))),
    ('SM_KerbCrew_A', lit(kerb_crew(2))),
    ('SM_KerbCrew_B', lit(kerb_crew(14, n=8, lunch=True))),
    ('SM_DockCrowd_A', lit(dock_crowd(3))),
    ('SM_DockCrowd_B', lit(dock_crowd(15, suits=True))),
    ('SM_Hazmat', lit(hazmat(6))),
    ('SM_Guards', lit(guards(7))),
    ('SM_Patrol', lit(patrol(8))),
    ('SM_Boffins', lit(boffins(9))),
    ('SM_CrowsPerch', lit(crows_perch(10), 0.03), dict(grow=(160.0, 60.0, 120.0), grow_down=(160.0, 60.0, 0.0))),
    ('SM_CrowFlock', crow_flock(4), dict(far=True, grow=(5500.0, 5500.0, 400.0), grow_down=(5500.0, 5500.0, 0.0))),
    ('SM_FarCatwalk', far_catwalk(11), dict(far=True, grow=(60.0, 60.0, 140.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
