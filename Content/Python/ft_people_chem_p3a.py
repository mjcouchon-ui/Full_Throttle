"""FULL THROTTLE (Unreal) the population, pass 3, builder p3a ("gaps"): the Volta Chemical Works (/Game/People/ChemicalWorks/SM_P3A_*).

The general cast of the works, for the stretches the first rounds left empty: people at work, at rest and passing
through, in pieces that fit any district.

  LOW pieces (top under 2 m as built; the table puts them 24.3 m out, in the 1.4 m lane between the kerb's stores and the
  racks, in the slots the first round's kerb line leaves free). Furniture and barriers stand in front (y < 0, unlit);
  every figure stands at y >= 45 (24.75 m), so the little light the shift carries (ft_people_chem.lit) is never nearer
  than 24 m:
    SM_P3A_Break_A     the tea break: three on drums with their cups, one at the urn, two up and cheering
    SM_P3A_Break_B     the card school round a cable reel, two kibitzers, one who has seen the chase
    SM_P3A_Sweepers    the yard gang passing through (they travel +X): two brooms, a bin cart, a litter picker
    SM_P3A_Porters     porters passing the other way (-X): a box, a length of pipe between two, a sack truck
    SM_P3A_Fitters     fitters at a valve manifold: a wrench, a handwheel turning, a clipboard, one on his toolbox
    SM_P3A_Marshals    road marshals behind chevron boards, waving the chase through with batons and a small flag
    SM_P3A_Hose        the wash-down team in chemical suits: a standpipe, a hose, spray on a pallet of drums
  TALL pieces (the table puts them 44 m and more out; each brings its own steel and a steady light, and carries its
  people 10 to 17 m up, over the racks and tank rows at the sight line):
    SM_P3A_StairTower  a stair tower, the shift on every landing and the top deck, one climbing
    SM_P3A_Painters    painters in a cradle that rides up and down a half-painted hoarding; two on a side scaffold
    SM_P3A_Mess        the mess cabin on a stack of four containers: the break taken at the rail
    SM_P3A_Steeplejacks a stack with a ring platform: two on the ring, one on the ladder, one in a bosun's chair

Placed by the block C:/ftwork/p3/L1a.inl. Run in the editor:  import ft_people_chem_p3a; ft_people_chem_p3a.run()
A piece is drawn like every prop: cm; it stands on z = 0; its FRONT faces -Y (the road); long pieces run along X.
"""
import math
import random
import unreal
import ft_people as P
import ft_people_chem as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/ChemicalWorks'
FY = 45.0            # the line every figure of a LOW piece stands on (the piece's middle is 24.3 m out)
WOOD = '#8f6b4a'
SPRAY = '#9ff0ff'
CREAM = '#fff6ea'


def low(r, **fixed):
    """One of the shift for a LOW piece: small enough that hat, raised hands and hop stay under 2 m."""
    fixed.setdefault('s', r.uniform(0.86, 0.93))
    return C.worker(r, **fixed)


def big(r, **fixed):
    """One of the shift for a TALL piece: a third over life size, middle detail."""
    fixed.setdefault('s', C.BIG * r.uniform(0.95, 1.03))
    return C.worker(r, **fixed)


def marshal(r, **fixed):
    d = P.look(r, sleeves='long', wear='trousers', hat='cap', hat_col=C.ORANGE, band=CREAM)
    d.update(top=C.LIME, bottom='#2b2350', shoes=C.DARK, s=r.uniform(0.86, 0.92))
    d.update(fixed)
    return d


def drum(g, x, y, z=0.0, col=C.TEAL, r=24.0, h=42.0):
    g.cyl(col, (x, y, z), r, h, steps=8)
    g.cyl(C.NAVY, (x, y, z + h * 0.45), r + 1.5, 4, steps=8)


def rail(g, L, y=-60.0, col=C.HAZ):
    """A single low rail on posts: what the walkers of a piece pass behind."""
    n = max(int(L / 300.0), 2)
    for k in range(n + 1):
        g.box(C.PAVE, (-L / 2.0 + L * k / n, y, 46), (9, 9, 92))
    g.box(col, (0, y, 88), (L, 7, 9))


def path(g, L, y=FY, w=100.0):
    """The strip a piece's walkers travel: it also gives the mesh its full length (they travel the whole of it)."""
    g.box(C.PAVE_D, (0, y, 2), (L, w, 4))
    for k in range(6):
        g.box(C.HAZ, (-L / 2.0 + (k + 0.5) * L / 6.0, y - w / 2.0 + 3, 5), (L / 12.0, 6, 3))


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_broom(head=C.RED):
    def f(m, h, side):
        if side < 0:
            return
        tilt_box(m, WOOD, (h[0], h[1] + 24, h[2] + 44), (h[0], h[1] - 66, 8), 5, 5)
        m.box(head, (h[0], h[1] - 68, 6), (44, 12, 11))
    return f


def hold_baton(col=C.ORANGE):
    def f(m, h, side):
        m.box(C.DARK, (h[0], h[1], h[2] + 4), (6, 6, 14))
        m.box(col, (h[0], h[1], h[2] + 22), (8, 8, 28))
    return f


def hold_pennant(col=C.PINK):
    """A small flag on a short stick (a low piece: nothing may reach 2 m)."""
    def f(m, h, side):
        m.box(C.POLE, (h[0], h[1], h[2] + 20), (4, 4, 44))
        m.box(col, (h[0] + side * 19, h[1], h[2] + 31), (36, 3, 20))
    return f


def hold_roller(col=C.MAG):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 34), (4, 4, 76))
        m.box(col, (h[0], h[1] - 4, h[2] + 74), (36, 11, 11))
    return f


def hold_board():
    def f(m, h, side):
        m.box(WOOD, (h[0] - side * 6, h[1] - 6, h[2] + 12), (24, 3, 32))
        m.box(CREAM, (h[0] - side * 6, h[1] - 7.8, h[2] + 10), (20, 1, 25))
    return f


def hold_nozzle():
    def f(m, h, side):
        if side < 0:
            return
        m.box(C.ORANGE, (h[0] - 20, h[1] - 4, h[2] + 4), (9, 60, 9))
        m.box(C.POLE, (h[0] - 20, h[1] - 40, h[2] + 4), (7, 22, 7))
    return f


# ------------------------------------------------------------------------------------------ the LOW pieces
def break_a(seed=31, L=900.0):
    """The tea break behind the trestles: three on drums with their cups, one filling his at the urn, two on their feet."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.trestles(g, L, y=-60)
        g.box(C.RUST, (-380, 40, 30), (84, 60, 60))
        g.cyl(C.POLE, (-380, 40, 60), 21, 44, steps=8)
        g.cyl(C.DARK, (-380, 40, 104), 23, 6, steps=8)
        sit = (
            dict(arm_r='wave', swing_r=24, arm_l='hold', hold_l=P.hold_cup(), rate=1.0),
            dict(arm_l='face', hold_l=P.hold_cup(), arm_r='point', swing_r=10, axis_r='x', rate=0.6),
            dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.4),
        )
        for i, x in enumerate((-190, -70, 150)):
            drum(g, x, FY + 4, col=(C.TEAL, C.HAZ, C.MAG)[i], h=40.0)
            P.figure(sc, x + r.uniform(-4, 4), FY, legs='sit', seat=46, phase=r.random(), react=True, **dict(low(r), **sit[i]))
        P.figure(sc, -300, FY, yaw=-90, arm_r='fwd', hold_r=P.hold_cup(), swing_r=7, axis_r='x', arm_l='hip', rate=0.5, phase=r.random(), **low(r))
        P.figure(sc, 30, FY + 4, yaw=r.uniform(-12, 12), phase=r.random(), react=True, **dict(low(r), arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=7, rate=1.5))
        P.figure(sc, 300, FY, yaw=-14, phase=r.random(), react=True, **dict(low(r), arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=6, rate=1.3))
        g.box(C.STEELB, (400, 40, 20), (70, 50, 40))
    return build


def break_b(seed=32, L=900.0):
    """The card school: two players across a cable reel, slapping their cards down, two looking on, one who has seen
    the chase and is telling the rest."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.trestles(g, L, y=-60)
        g.cyl(WOOD, (0, FY, 0), 52, 6, steps=10)
        g.cyl(C.POLE, (0, FY, 6), 22, 58, steps=8)
        g.cyl(WOOD, (0, FY, 64), 56, 7, steps=10)
        for k, col in enumerate((CREAM, C.RED, CREAM)):
            g.box(col, (-16 + k * 16, FY - 8 + k * 7, 72.5), (10, 14, 2))
        for sx in (-1, 1):
            g.box(C.RUST if sx < 0 else C.TEAL, (sx * 104, FY + 4, 20), (58, 52, 40))
            P.figure(sc, sx * 104, FY, yaw=-sx * 90, legs='sit', seat=46, arm_r='fwd', swing_r=16, axis_r='x', arm_l='hold', rate=0.9,
                     phase=0.2 + 0.5 * (sx > 0), **low(r))
        P.figure(sc, -230, FY + 4, yaw=70, arm_l='hip', arm_r='face', hold_r=P.hold_cup(), rate=0.3, bob=-1.0, phase=r.random(), **low(r))
        P.figure(sc, 220, FY + 2, yaw=-40, phase=r.random(), react=True, **dict(low(r), arm_r='point', swing_r=12, axis_r='x', arm_l='wave', swing_l=-22, rate=0.9, bob=4))
        P.figure(sc, 350, FY, yaw=10, phase=r.random(), react=True, **dict(low(r), arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=7, rate=1.5))
        P.figure(sc, -360, FY, yaw=-10, phase=r.random(), react=True, **dict(low(r), arm_r='wave', swing_r=28, rate=1.3, bob=4))
        drum(g, -420, FY - 30, col=C.HAZ)
    return build


def sweepers(seed=33, L=1600.0, speed=62.0):
    """The yard gang passing through, slowly, towards +X: two with brooms, one pushing the bin cart, a litter picker."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        path(g, L)
        rail(g, L)
        walk = dict(yaw=90, stride=20, rate=0.8, bob=2, travel=(L, speed))
        broom = dict(arm_l='carry', arm_r='carry', swing_l=14, swing_r=14, axis_l='x', axis_r='x')
        P.figure(sc, -300, FY + 6, phase=0.0, **dict(low(r), hold_r=hold_broom(), **dict(walk, **broom)))
        P.figure(sc, -120, FY - 4, phase=0.4, **dict(low(r), hold_r=hold_broom(C.TEAL), **dict(walk, **broom)))
        P.figure(sc, 80, FY, phase=0.7, **dict(low(r), arm_l='push', arm_r='push', **walk))
        cx = 80 + 78.0
        cart = sc.part(P.motion(phase=0.7, rate=0.8, bob=2, base=(cx, FY), pivot=(cx, FY, 40.0), travel=(L, speed / L)))
        cart.box(C.LIME, (cx, FY, 58), (74, 56, 72))
        cart.box(C.NAVY, (cx, FY, 96), (80, 60, 6))
        cart.box(C.POLE, (cx - 44, FY, 92), (6, 50, 6))
        for sy in (-1, 1):
            cart.cyl(C.DARK, (cx + 10, FY + sy * 30 + (4 if sy < 0 else 0), 13), 13, 6, rot=(0, 0, 90), steps=8)
        P.figure(sc, 330, FY + 4, phase=0.2, **dict(low(r), arm_r='down', hold_r=P.hold_bag(C.ORANGE), arm_l='point', swing_l=14, axis_l='x',
                                                    **walk))
    return build


def porters(seed=34, L=1800.0, speed=-105.0):
    """Porters passing the other way (towards -X): one with a box, two with a length of pipe between them, one
    behind a sack truck, the charge hand with his board."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        path(g, L)
        rail(g, L)
        walk = dict(yaw=-90, stride=26, rate=1.0, bob=3, travel=(L, speed))
        tv = (L, speed / L)
        P.figure(sc, -420, FY, phase=0.1, **dict(low(r), arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=C.BOARD), **walk))
        ph = 0.45
        for x in (-150, 110):
            P.figure(sc, x, FY - 10, phase=ph, **dict(low(r, s=0.9), arm_l='down', arm_r='down', swing_r=18, axis_r='x', **walk))
        px = -20.0
        pipe = sc.part(P.motion(phase=ph, rate=1.0, bob=3, base=(px, FY), pivot=(px, FY, 50.0), travel=tv))
        rod(pipe, C.TEAL, (px - 220, FY + 24, 50), (px + 220, FY + 24, 50), 8, steps=6)
        for sx in (-1, 1):
            rod(pipe, C.HAZ, (px + sx * 205, FY + 24, 50), (px + sx * 222, FY + 24, 50), 10, steps=6)
        P.figure(sc, 420, FY, phase=0.8, **dict(low(r), arm_l='push', arm_r='push', **walk))
        tx = 420 - 76.0
        truck = sc.part(P.motion(phase=0.8, rate=1.0, bob=3, base=(tx, FY), pivot=(tx, FY, 40.0), travel=tv))
        truck.box(C.RED, (tx + 22, FY, 70), (6, 44, 120))
        truck.box(C.RED, (tx - 4, FY, 12), (50, 44, 6))
        truck.cyl(C.HAZ, (tx - 6, FY, 15), 21, 42, steps=8)
        truck.cyl(C.MAG, (tx - 6, FY, 59), 21, 42, steps=8)
        for sy in (-1, 1):
            truck.cyl(C.DARK, (tx + 22, FY + sy * 26 + (4 if sy < 0 else 0), 12), 12, 5, rot=(0, 0, 90), steps=8)
        P.figure(sc, 650, FY + 4, phase=0.3, **dict(low(r), arm_l='hold', hold_l=hold_board(), arm_r='point', swing_r=10, axis_r='x', **walk))
    return build


def fitters(seed=35, L=800.0):
    """Fitters at a valve manifold: a pipe on stools, two handwheels (one turning under a fitter's hands), a wrench at
    work, the checker with his board, one on his toolbox, one who has downed tools to wave."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        rod(g, C.TEAL, (-L / 2.0 + 30, -8, 62), (L / 2.0 - 30, -8, 62), 15, steps=8)
        for x in (-320, -110, 110, 320):
            g.box(C.PAVE, (x, -8, 24), (26, 40, 48))
        for x in (-L / 2.0 + 30, L / 2.0 - 30):
            g.cyl(C.HAZ, (x, 10, 62), 22, 8, rot=(0, 0, 90), steps=8)
        for i, x in enumerate((-200, 60)):
            g.box(C.STEELB, (x, -8, 84), (30, 30, 30))
            g.box(C.POLE, (x, -8, 106), (7, 7, 20))
            wheel = sc.part(P.motion(phase=0.3 * i, rate=0.5, swing=50.0, pivot=(x, -8.0, 118.0), axis=P.UP)) if i == 0 else g
            wheel.box(C.RED, (x, -8, 118), (40, 8, 6))
            wheel.box(C.RED, (x, -8, 118), (8, 40, 6))
        P.figure(sc, -200, FY, arm_l='fwd', arm_r='fwd', swing_l=12, swing_r=12, axis_l='z', axis_r='z', rate=0.5, phase=0.0, **low(r))
        P.figure(sc, 60, FY, yaw=8, arm_r='fwd', swing_r=18, axis_r='x', hold_r=P.hold_tool(), arm_l='hip', rate=1.4, phase=r.random(), **low(r))
        P.figure(sc, -70, FY + 4, yaw=-20, arm_l='hold', arm_r='hold', hold_l=hold_board(), rate=0.3, bob=-1.2, phase=r.random(), **low(r, hat_col=CREAM))
        g.box(C.RED, (220, FY + 4, 20), (70, 44, 40))
        P.figure(sc, 220, FY, legs='sit', seat=46, phase=r.random(), react=True, **dict(low(r), arm_r='wave', swing_r=26, arm_l='hold', hold_l=P.hold_cup(), rate=1.1))
        P.figure(sc, 340, FY, yaw=-12, phase=r.random(), react=True, **dict(low(r), arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=7, rate=1.5))
        P.figure(sc, -340, FY, yaw=14, phase=r.random(), react=True, **dict(low(r), arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.7))
    return build


def marshals(seed=36, L=900.0):
    """Road marshals behind three chevron boards: batons waved overhead, an arm windmilling the chase through, a small
    pink flag, one pointing down the road."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for bx in (-300, 0, 300):
            for sx in (-1, 1):
                g.box(C.PAVE, (bx + sx * 100, -58, 30), (9, 30, 60))
            g.box(CREAM, (bx, -60, 68), (250, 6, 76))
            for k in range(4):
                tilt_box(g, C.ORANGE, (bx - 96 + k * 60, -64, 34), (bx - 66 + k * 60, -64, 102), 3, 26)
        acts = (
            dict(arm_r='wave', swing_r=30, hold_r=hold_baton(), arm_l='hip', rate=1.2, bob=3),
            dict(arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, hold_l=hold_baton(), hold_r=hold_baton(), rate=1.1, bob=6),
            dict(arm_r='out', swing_r=38, axis_r='y', arm_l='face', rate=1.0),
            dict(arm_l='wave', swing_l=-26, hold_l=hold_pennant(), arm_r='point', swing_r=8, axis_r='x', rate=1.0, bob=3),
            dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, hold_r=hold_baton(C.YEL), rate=1.4, bob=7),
        )
        for i in range(5):
            x = -360 + i * 180 + r.uniform(-14, 14)
            P.figure(sc, x, FY + (6 if i % 2 else 0), yaw=r.uniform(-12, 12), phase=r.random(), react=True, **dict(marshal(r), **acts[i]))
        for x in (-440, 430):
            g.cyl(C.ORANGE, (x, FY - 20, 0), 20, 62, steps=8, top_r=6)
            g.box(C.ORANGE, (x, FY - 20, 2), (46, 46, 4))
    return build


def hose(seed=37, L=1000.0):
    """The wash-down team in chemical suits: one at the standpipe's wheel, two on the hose, its spray falling on a
    pallet of drums, one scrubbing with a long brush, the supervisor waving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        C.trestles(g, L, y=-60)
        g.cyl(C.RED, (-440, FY, 0), 11, 92, steps=8)
        wheel = sc.part(P.motion(rate=0.5, swing=45.0, pivot=(-440.0, FY, 98.0), axis=P.UP))
        wheel.box(C.HAZ, (-440, FY, 98), (46, 8, 6))
        wheel.box(C.HAZ, (-440, FY, 98), (8, 46, 6))
        g.box(C.ORANGE, (-250, FY - 26, 5), (380, 9, 9))
        s = dict(s=0.93)
        P.figure(sc, -372, FY, yaw=-90, arm_l='fwd', arm_r='fwd', swing_l=12, swing_r=12, axis_l='z', axis_r='z', rate=0.5, **C.suit(r, **s))
        for k, x in enumerate((-70, 20)):
            P.figure(sc, x, FY, yaw=90, legs='stride', arm_l='carry', arm_r='carry', hold_r=hold_nozzle() if k else None, rock=4, rate=0.4,
                     phase=0.1 * k, **C.suit(r, col=C.YEL if k else None, **s))
        for k in range(6):
            p = sc.part(P.motion(phase=k / 6.0, rate=1.5, bob=-9.0))
            p.box(SPRAY, (86 + k * 36, FY + 14, 104 - k * k * 2.2), (22, 10, 10))
        g.box(C.RUST, (330, FY, 8), (180, 110, 16))
        for k, col in enumerate((C.TEAL, C.MAG, C.HAZ)):
            drum(g, 276 + k * 54, FY + (12 if k % 2 else -8), 16, col, h=86.0)
        P.figure(sc, 456, FY, yaw=-90, arm_l='carry', arm_r='carry', swing_l=13, swing_r=13, axis_l='x', axis_r='x', hold_r=hold_broom(CREAM), rate=0.9,
                 phase=r.random(), **C.suit(r, **s))
        P.figure(sc, -210, FY + 4, yaw=-8, phase=r.random(), react=True, **dict(C.suit(r, col=CREAM, **s), arm_r='wave', swing_r=28, arm_l='hip', rate=1.2, bob=4))
    return build


# ------------------------------------------------------------------------------------------ the TALL pieces
def lamps(g, pts, glow=0.9):
    for (x, y, z) in pts:
        g.box(C.DARK, (x, y, z + 12), (60, 60, 22))
        g.box(C.LAMP, (x, y - 6, z - 2), (50, 48, 5), glow=glow)


def stair_tower(seed=41, W=700.0, D=280.0, levels=(400.0, 800.0, 1200.0, 1600.0)):
    """A stair tower: four flights zig-zagging up a sheeted frame, the shift on every landing and the top deck (17 m),
    one on his way up. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        top = levels[-1]
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(C.POLE, (sx * W / 2.0, sy * D / 2.0, (top + 220) / 2.0), (14, 14, top + 220))
                g.box(C.NAVY, (sx * W / 2.0, sy * D / 2.0, 3), (40, 40, 6))
        ends = []
        for i, z1 in enumerate(levels):
            z0 = levels[i - 1] if i else 0.0
            xa = (W / 2.0 - 180) * (1 if i % 2 == 0 else -1)
            xb = -xa
            fy = -D / 2.0 + 60
            tilt_box(g, C.BOARD, (xa, fy, z0), (xb, fy, z1), 100, 12)
            tilt_box(g, C.HAZ, (xa, -D / 2.0 + 8, z0 + 105), (xb, -D / 2.0 + 8, z1 + 105), 8, 8)
            sgn = 1 if xb > 0 else -1
            if z1 < top:
                lx = xb + sgn * 90
                g.box(C.BOARD, (lx, 0, z1 - 6), (190, D, 10))
                g.box(C.HAZ, (lx, -D / 2.0 - 4, z1 + 8), (190, 4, 18))
                g.box(C.POLE, (lx, -D / 2.0, z1 + 102), (190, 8, 8))
                ends.append((lx, z1))
            C.sheet(g, 0, D / 2.0 + 8, z1 + 4, W, 236)
            tilt_box(g, C.POLE, (-W / 2.0, D / 2.0 + 16, z0 + 20), (W / 2.0, D / 2.0 + 16, z1 - 20), 9, 9) if i % 2 else None
        g.box(C.BOARD, (0, 0, top - 6), (W + 30, D + 20, 12))
        g.box(C.HAZ, (0, -D / 2.0 - 12, top + 8), (W + 30, 4, 20))
        g.box(C.POLE, (0, -D / 2.0 - 6, top + 102), (W + 20, 8, 8))
        for sx in (-1, 1):
            g.box(C.POLE, (sx * (W / 2.0 + 6), 0, top + 102), (8, D, 8))
        lamps(g, [(-W / 2.0, D / 2.0 - 30, top + 226), (W / 2.0, D / 2.0 - 30, top + 226)])
        for k, (lx, z) in enumerate(ends):
            d = big(r)
            d.update(C.CHEER[(k * 2 + seed) % len(C.CHEER)])
            P.figure(sc, lx, -30, z=z, yaw=r.uniform(-16, 16), phase=r.random(), react=True, detail=1, **d)
        for k, x in enumerate((-225, 0, 230)):
            d = big(r)
            d.update(C.CHEER[(k * 3 + 1) % len(C.CHEER)])
            if k == 1:
                d.update(arm_l='up', swing_l=20, hold_l=P.hold_flag(col=C.PINK, w=90), arm_r='point', swing_r=0, rate=0.9, bob=0)
            P.figure(sc, x, -40 + r.uniform(-14, 14), z=top, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=1, **d)
        # the one on his way up the third flight: stepping where he stands
        P.figure(sc, 0, -D / 2.0 + 60, z=levels[1] + (levels[2] - levels[1]) / 2.0 + 6, yaw=-90, stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x',
                 rate=1.2, bob=5, phase=r.random(), detail=1, **big(r))
    return build


def painters(seed=42, W=1000.0, H=1500.0):
    """Painters in a cradle that rides slowly up and down a hoarding, half of it primer and half fresh stripes (no
    words, no marks); two more on a scaffold at its end, who have stopped to watch. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(C.WHITE, (0, 70, 100 + H / 2.0), (W, 10, H), glow=0.12)
        for k in range(5):
            g.box(C.TEAL if k % 2 == 0 else C.PINK, (-W / 2.0 + (k + 0.5) * W / 5.0, 63, 100 + 360), (W / 5.0, 4, 720), glow=0.12)
        g.box(C.MAG, (-W * 0.3, 63, 100 + 720 + 130), (W * 0.4, 4, 260), glow=0.12)
        for x in (-W / 2.0 + 20, 0, W / 2.0 - 20):
            g.box(C.POLE, (x, 86, (H + 160) / 2.0), (14, 14, H + 160))
            tilt_box(g, C.POLE, (x, 330, 0), (x, 92, 900), 12, 12)
        g.box(C.POLE, (0, 20, H + 150), (W + 40, 150, 14))
        cz, ph, rate, ride = 930.0, 0.1, 0.15, 90.0
        cr = sc.part(P.motion(phase=ph, rate=rate, bob=-ride))
        cr.box(C.BOARD, (0, -40, cz - 6), (600, 110, 10))
        cr.box(C.HAZ, (0, -97, cz + 8), (600, 4, 18))
        cr.box(C.POLE, (0, -95, cz + 100), (600, 8, 8))
        for sx in (-1, 1):
            cr.box(C.POLE, (sx * 296, -40, cz + 52), (8, 110, 104))
            cr.box(C.DARK, (sx * 270, -40, (cz + H + 150 - ride) / 2.0), (6, 6, H + 150 - ride - cz))
            g.box(C.DARK, (sx * 270, -40, H + 45), (6, 6, 210))
        for k, col in enumerate((C.MAG, C.TEAL)):
            cr.cyl(col, (-40 + k * 80, -70, cz), 15, 30, steps=6)
        for k, x in enumerate((-200, 30, 215)):
            d = big(r)
            if k == 1:
                d.update(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12)
                yaw = r.uniform(-10, 10)
            else:
                d.update(arm_r='up', swing_r=20, axis_r='y', hold_r=hold_roller(C.MAG if k else C.TEAL), arm_l='hip')
                yaw = 180 + r.uniform(-10, 10)
            P.figure(sc, x, -34, z=cz, yaw=yaw, phase=ph, rate=rate, bob=-ride, detail=1, **d)
        sx0 = W / 2.0 + 190
        C.scaffold(g, 300.0, 200.0, (1100.0,), x0=sx0, y0=0.0, bays=1, lamp=0.8)
        C.ladder(g, sx0 + 100, -106, 0, 1100)
        C.sheet(g, sx0, 108, 1104, 280, 236)
        for k, x in enumerate((sx0 - 70, sx0 + 70)):
            d = big(r)
            d.update(C.CHEER[(k * 4 + seed) % len(C.CHEER)])
            P.figure(sc, x, -10, z=1100, yaw=r.uniform(-18, 18), phase=r.random(), react=True, detail=1, **d)
    return build


def mess(seed=43, n=4):
    """The mess cabin on a stack of four containers: the break taken at the rail, 10.4 m up: two on the bench with
    their cups, three on their feet for the chase. Its windows are lit (steady). TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        cols = (C.TEAL, C.RUST, C.STEELB, C.PAVE)
        for k in range(n):
            g.box(cols[k % 4], (0, 40, k * 260 + 130), (620, 250, 254))
            for x in (-220, -73, 73, 220):
                g.box(C.NAVY, (x, -86, k * 260 + 130), (8, 4, 226))
        Z = n * 260.0
        g.box(C.BOARD, (0, -20, Z - 6), (780, 410, 12))
        g.box(C.HAZ, (0, -227, Z + 8), (780, 4, 20))
        g.box(C.POLE, (0, -222, Z + 102), (780, 8, 8))
        for k in range(5):
            g.box(C.POLE, (-386 + k * 193, -222, Z + 52), (8, 8, 104))
        for sx in (-1, 1):
            g.box(C.POLE, (sx * 386, -20, Z + 102), (8, 400, 8))
            tilt_box(g, C.POLE, (sx * 360, -200, Z - 200), (sx * 310, -90, Z - 12), 10, 10)
        g.box(C.WHITE, (0, 110, Z + 125), (600, 140, 250))
        g.box(C.NAVY, (0, 110, Z + 256), (640, 190, 12))
        for x in (-190, 0):
            g.box(C.LAMP, (x, 38, Z + 150), (120, 4, 84), glow=0.6)
        g.box(C.TEAL, (200, 38, Z + 105), (90, 4, 206))
        C.ladder(g, 340, -234, 0, Z)
        g.box(C.RUST, (-230, -104, Z + 31), (220, 54, 62))
        for k, x in enumerate((-280, -180)):
            d = big(r)
            d.update((dict(arm_r='wave', swing_r=24, arm_l='hold', hold_l=P.hold_cup(), rate=1.0),
                      dict(arm_l='face', hold_l=P.hold_cup(), arm_r='point', swing_r=10, axis_r='x', rate=0.6))[k])
            P.figure(sc, x, -112, z=Z, legs='sit', seat=46, phase=r.random(), react=True, detail=1, **d)
        for k, x in enumerate((-20, 130, 280)):
            d = big(r)
            d.update(C.CHEER[(k * 2 + seed) % len(C.CHEER)])
            P.figure(sc, x, -140 + r.uniform(-16, 16), z=Z, yaw=r.uniform(-16, 16), phase=r.random(), react=True, detail=1, **d)
    return build


def steeplejacks(seed=44, H=2000.0, ring=1300.0):
    """A stack and its steeplejacks: two on the ring platform 13 m up, one on the ladder, one in a bosun's chair that
    swings gently from a jib at the top. TALL."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        cy = 60.0

        def rad(z):
            return 120.0 - 35.0 * z / H

        g.cyl(C.PAVE, (0, cy, 0), 120, H, steps=10, top_r=85)
        g.cyl(C.NAVY, (0, cy, 0), 150, 40, steps=10)
        for k, z in enumerate((1500, 1680, 1860)):
            g.cyl(C.ORANGE if k % 2 == 0 else CREAM, (0, cy, z), rad(z) + 4, 110, steps=10, top_r=rad(z + 110) + 4)
        g.cyl(C.BOARD, (0, cy, ring - 12), 260, 12, steps=10)
        g.cyl(C.HAZ, (0, cy, ring - 24), 268, 12, steps=10)
        for k in range(10):
            a0, a1 = math.radians(k * 36.0), math.radians(k * 36.0 + 36.0)
            p0 = (math.cos(a0) * 256, cy + math.sin(a0) * 256)
            p1 = (math.cos(a1) * 256, cy + math.sin(a1) * 256)
            g.box(C.POLE, (p0[0], p0[1], ring + 60), (8, 8, 110))
            tilt_box(g, C.POLE, (p0[0], p0[1], ring + 112), (p1[0], p1[1], ring + 112), 8, 8)
        for k in range(4):
            a = math.radians(45 + k * 90)
            tilt_box(g, C.POLE, (math.cos(a) * 110, cy + math.sin(a) * 110, ring - 260), (math.cos(a) * 240, cy + math.sin(a) * 240, ring - 14), 10, 10)
        C.ladder(g, 0, cy - 128, 0, ring)
        lamps(g, [(-215, cy - 60, ring + 150), (215, cy - 60, ring + 150)], glow=0.8)
        for sx in (-1, 1):
            g.box(C.POLE, (sx * 215, cy - 40, ring + 80), (8, 8, 150))
        for k, (x, y) in enumerate(((-120, cy - 215), (115, cy - 220))):
            d = big(r)
            d.update(C.CHEER[(k * 3 + seed) % len(C.CHEER)])
            P.figure(sc, x, y, z=ring, yaw=r.uniform(-16, 16), phase=r.random(), react=True, detail=1, **d)
        P.figure(sc, 0, cy - 160, z=640, yaw=180, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, axis_l='x', axis_r='x', stride=18, rate=1.1, bob=5,
                 phase=r.random(), detail=1, **big(r))
        # the jib at the top and the bosun's chair under it
        jx, jy, jz = 330.0, cy - 100.0, H + 20.0
        tilt_box(g, C.POLE, (-60, jy, jz), (jx + 30, jy, jz), 12, 12)
        tilt_box(g, C.POLE, (0, jy, H - 300), (jx - 40, jy, jz), 9, 9)
        g.box(C.POLE, (0, jy + 50, jz), (12, 110, 12))
        sz = ring + 330.0
        swing = dict(swing=6.0, pivot=(jx, jy, jz), axis=90.0)
        ch = sc.part(P.motion(rate=0.2, phase=0.3, **swing))
        ch.box(C.DARK, (jx, jy + 16, (sz + 60 + jz) / 2.0), (6, 6, jz - sz - 60))
        ch.box(C.BOARD, (jx, jy, sz + 56), (84, 46, 8))
        ch.cyl(C.MAG, (jx + 52, jy, sz + 20), 13, 30, steps=6)
        P.figure(sc, jx, jy - 4, z=sz, legs='sit', seat=46, arm_r='wave', arm_l='up', rate=0.2, phase=0.3, sway=swing, detail=1, **big(r))
    return build


JOBS = [
    ('SM_P3A_Break_A', C.lit(break_a())),
    ('SM_P3A_Break_B', C.lit(break_b())),
    ('SM_P3A_Sweepers', C.lit(sweepers())),
    ('SM_P3A_Porters', C.lit(porters())),
    ('SM_P3A_Fitters', C.lit(fitters())),
    ('SM_P3A_Marshals', C.lit(marshals())),
    ('SM_P3A_Hose', C.lit(hose())),
    ('SM_P3A_StairTower', C.lit(stair_tower())),
    ('SM_P3A_Painters', C.lit(painters()), dict(grow=(60.0, 60.0, 130.0), grow_down=(60.0, 60.0, 0.0))),
    ('SM_P3A_Mess', C.lit(mess())),
    ('SM_P3A_Steeplejacks', C.lit(steeplejacks()), dict(grow=(120.0, 60.0, 60.0), grow_down=(120.0, 60.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
