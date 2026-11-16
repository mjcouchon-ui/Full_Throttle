"""FULL THROTTLE (Unreal) the population, level 2 (Chemical Works), pass 3, builder p3b: SCENES.

Ten story groups that could only be at the Volta works, one mesh a scene (/Game/People/ChemicalWorks/SM_P3B_*):

    SM_P3B_Decon       the decontamination line: a suit under the shower turning, two scrubbing it with long brushes,
                       the queue waiting its turn, one already out and drying off, the attendant ticking them off
    SM_P3B_FireDrill   the works fire brigade at drill: two on the hand pump's see-saw, the hose crew and its jet on a
                       burning practice drum, the captain with a megaphone, the bucket chain
    SM_P3B_Inspection  inspection day: three inspectors with clipboards, the manager all welcome, and behind his back
                       two holding a tarpaulin over the leaking drums, one mopping, one painting the rail
    SM_P3B_Meeting     the stop-work meeting: a speaker on a drum with a megaphone, placards, fists, the brazier
    SM_P3B_TugOfWar    the shifts' tug of war, orange against lime, the rope and both teams sliding to and fro, the
                       referee with a flag, the cup waiting on a drum
    SM_P3B_Capstan     the gang opening the great sluice valve: four walking the capstan round, the foreman beating
                       time, the gauge's needle swinging, steam at the gland
    SM_P3B_Briefing    the toolbox talk: the foreman at the board with a pointer, the shift on drums (one asleep, one
                       with a hand up, one eating, one waving at the chase), the late-comer running in along the back
    SM_P3B_SafeDays    the tally of days without an accident: the manager on the steps hanging the next green tile,
                       the shift clapping, the one with an arm in a sling, the photographer
    SM_P3B_CraneLift   the lift: a vessel swinging on the davit's chain, the banksman signalling, two steadying it, the
                       winch man at the crank, the cradle waiting
    SM_P3B_Band        the works brass band at practice: conductor, tuba, trombone, two trumpets, the bass drum, the
                       cymbals, the banner

How this road lies (ft_people_chem.py, FTDressL1.inl): a person on the ground is not seen from the road; pipe racks
(to 7.6 m), loading bays (7.2 m), compressor sheds (5.7 m) and tank rows (to 11.4 m) stand 24 to 38 m out in every
district. So every scene of this kit brings its own STAGE: a boarded steel deck 12.3 m up on four raking legs (it
clears all of those and can stand over them), a lit pale sheet behind the cast, two steady work lamps. Figures are
drawn 1.4 times life size. All pieces are TALL (24 m or more; the block places them 34 to 35 m out), carry only steady
light, and every one moves. No words, no marks: stripes and shapes only.

The block that places them is C:/ftwork/p3/L1b.inl (written by the scratch script C:/ftwork/jobs/L1p3b/mkblock.py).
Run in the editor:  import ft_people_chem_p3b; ft_people_chem_p3b.run()
"""
import math
import random
import unreal
import ft_people as P
import ft_people_chem as C
import ft_people_crew as CR
from ft_desert import rod, tilt_box

DIR = '/Game/People/ChemicalWorks'

STEELB, TEAL, HAZ, NAVY, MAG, VIOLET, WHITE = C.STEELB, C.TEAL, C.HAZ, C.NAVY, C.MAG, C.VIOLET, C.WHITE
PAVE, RUST, DARK, POLE, BOARD = C.PAVE, C.RUST, C.DARK, C.POLE, C.BOARD
ORANGE, LIME, YEL, RED, PINK, LAMP = C.ORANGE, C.LIME, C.YEL, C.RED, C.PINK, C.LAMP
CYAN, GOLD, CREAM, WATER = '#2ee6ff', '#ffc93c', '#fff6ea', '#9ff0ff'

H0 = 1230.0       # the deck: over the tank rows (11.4 m)
S = 1.4           # the cast, on raised steel 34 m out, is drawn larger than life: it must read at 120 mph
UP = P.UP


# ------------------------------------------------------------------------------------------ the stage and the cast
def stage(sc, L, D=340.0, H=H0, sheet=WHITE, stripe=ORANGE):
    """A boarded deck L long (X) and D deep (Y), H up, on four raking legs: toe board in hazard yellow, one rail at
    the front, a lit sheet 3.2 m high behind the cast, a steady work lamp over each end. Returns the still model."""
    g = sc.still()
    hx, hy = L / 2.0, D / 2.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, POLE, (sx * (hx + 40), sy * (hy + 40), 0), (sx * (hx - 30), sy * (hy - 20), H - 20), 22, 22)
        g.box(POLE, (sx * (hx + 6), -hy - 10, H + 50), (8, 8, 100))
    for sy in (-1, 1):
        g.box(POLE, (0, sy * (hy + 8), H * 0.5), (L + 10, 12, 12))
    tilt_box(g, POLE, (-hx, hy + 10, H * 0.5), (hx, hy + 10, H - 50), 12, 12)
    g.box(STEELB, (0, 0, H - 34), (L + 10, D + 10, 30))
    g.box(BOARD, (0, 0, H - 10), (L + 40, D + 30, 20))
    g.box(HAZ, (0, -hy - 17, H + 2), (L + 40, 4, 34))
    g.box(POLE, (0, -hy - 10, H + 98), (L + 30, 8, 8))
    g.box(sheet, (0, hy + 12, H + 160), (L, 5, 320), glow=0.1)
    g.box(stripe, (0, hy + 8.5, H + 296), (L, 2, 20))
    for sx in (-1, 1):
        g.box(POLE, (sx * (hx - 20), hy + 12, H + 380), (9, 9, 130))
        g.box(DARK, (sx * (hx - 20), hy - 4, H + 446), (60, 44, 24))
        g.box(LAMP, (sx * (hx - 20), hy - 14, H + 432), (52, 30, 5), glow=0.8)
    return g


def wk(r, **kw):
    kw.setdefault('s', S * r.uniform(0.95, 1.03))
    return C.worker(r, **kw)


def st(r, col=None, **kw):
    kw.setdefault('s', S * r.uniform(0.95, 1.02))
    return C.suit(r, col, **kw)


def plain(r, top, bottom=DARK, hat='hard', hat_col=CREAM, **kw):
    """Somebody who is not in hi-vis: an inspector, a manager, a bandsman."""
    d = P.look(r, sleeves='long', wear='trousers', hat=hat, hat_col=hat_col)
    d.update(top=top, bottom=bottom, shoes=DARK, s=S * r.uniform(0.95, 1.03))
    d.update(kw)
    return d


def put(sc, r, x, y, d, z=H0, **pose):
    """One of the cast on the deck. d: a look; pose: figure()'s pose and movement values (they win)."""
    q = dict(d)
    q.update(pose)
    # hands, feet and bent arms for whoever holds something, walks a path or turns on the spot; the rest are detail 0
    q.setdefault('detail', 1 if (q.get('hold_l') or q.get('hold_r') or q.get('orbit') or q.get('travel') or q.get('turn')) else 0)
    if q['detail'] == 0 and q.get('hat') in ('hard', 'helmet', 'peak'):
        q['hair'] = 'bald'                      # nothing of it shows under the hat at this size
    q.setdefault('phase', r.random())
    return P.figure(sc, x, y, z=z, **q)


def drum(g, x, y, z=H0, col=TEAL, band=HAZ, h=90.0, r=30.0):
    g.cyl(col, (x, y, z), r, h, steps=7)
    g.cyl(band, (x, y, z + h * 0.55), r + 2, 10, steps=7)


def seat(g, x, y, col=RUST):
    """What a sitter sits on: figure(legs='sit', seat=46) at scale S sits 64 cm up."""
    g.cyl(col, (x, y + 8 * S, H0), 27, 46 * S, steps=6)


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_brush(col=POLE, head=CYAN, length=95.0):
    """A long-handled brush held out ahead (scrubbing somebody down)."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 14, h[2]), (h[0], h[1] - length, h[2] + 6), 4, 4)
        m.box(head, (h[0], h[1] - length, h[2] + 6), (26, 10, 14))
    return f


def hold_mop(col=POLE, head=CREAM):
    """A mop or a broom: from the hand down to the floor ahead."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 10, h[2] + 20), (h[0], h[1] - 50, 8), 4, 4)
        m.box(head, (h[0], h[1] - 52, 6), (34, 16, 12))
    return f


def hold_bucket(col=RED):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1], h[2] - 34), 11, 24, steps=6, top_r=14)
        m.box(DARK, (h[0], h[1], h[2] - 6), (3, 3, 10))
    return f


def hold_nozzle(col=GOLD):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 4, h[2] + 4), 6, 34, rot=(0, 0, 90), steps=6, top_r=3.5)
    return f


def hold_tile(col=LIME, w=34.0):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 3, h[2] + 20), (w, 4, w))
    return f


def hold_towel(col=CREAM):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] - 16), (20, 6, 46))
    return f


def hold_pointer(col=CREAM, length=70.0):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1], h[2]), (h[0] + side * length * 0.5, h[1] - length * 0.6, h[2] + length * 0.5), 4, 4)
    return f


def hold_horn(col=GOLD, length=44.0, bell=13.0):
    """A trumpet at the lips: a tube ahead of the hand, a bell at its end."""
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] + 4, h[2] + 4), 3.5, length, rot=(0, 0, 90), steps=6)
        m.cyl(col, (h[0], h[1] - length + 6, h[2] + 4), 4, 12, rot=(0, 0, 90), steps=6, top_r=bell)
    return f


def hold_trombone(col=GOLD):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 30, h[2] + 4), (4, 96, 4))
        m.box(col, (h[0] + side * 7, h[1] - 44, h[2] + 4), (4, 70, 4))
        m.cyl(col, (h[0], h[1] - 70, h[2] + 4), 4, 14, rot=(0, 0, 90), steps=6, top_r=16)
    return f


def hold_tuba(col=GOLD):
    def f(m, h, side):
        if side > 0:
            return
        m.box(col, (h[0] + 14, h[1] - 4, h[2] - 6), (30, 22, 44))
        m.cyl(col, (h[0] + 14, h[1] - 2, h[2] + 14), 9, 46, steps=6, top_r=27)
    return f


def hold_cymbal(col=GOLD):
    def f(m, h, side):
        m.cyl(col, (h[0] - side * 5, h[1] - 6, h[2] + 6), 21, 3, rot=(0, 90, 90), steps=8)
    return f


# ------------------------------------------------------------------------------------------ the scenes
def decon(seed=31, L=1200.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet='#cfe9ff', stripe=CYAN)
        H = H0
        sx = -120.0
        # the shower: a frame, a rose, a tub; the water shimmers
        for k in (-1, 1):
            g.box(POLE, (sx + k * 95, 60, H + 170), (9, 9, 340))
        g.box(POLE, (sx, 60, H + 340), (200, 9, 9))
        g.box(POLE, (sx, 10, H + 336), (9, 100, 9))
        g.cyl(CYAN, (sx, -30, H + 312), 30, 16, steps=7, top_r=12)
        g.cyl(CYAN, (sx, -30, H), 92, 22, steps=8)
        for k in range(3):
            w = sc.part(P.motion(phase=k / 3.0, rate=1.7, bob=-16.0))
            w.box(WATER, (sx - 26 + k * 26, -30 - 8 * (k % 2), H + 170), (7, 7, 250))
        put(sc, r, sx, -30, st(r, YEL), arm_l='out', arm_r='out', turn=38, rate=0.45)
        put(sc, r, sx - 150, -50, st(r, CREAM), yaw=90, arm_l='fwd', arm_r='fwd', hold_r=hold_brush(), swing_l=14, swing_r=14,
            axis_l='y', axis_r='y', rate=1.5)
        put(sc, r, sx + 150, -40, st(r, CREAM), yaw=-90, arm_l='fwd', arm_r='fwd', hold_l=hold_brush(head=PINK), swing_l=14, swing_r=14,
            axis_l='y', axis_r='y', rate=1.3)
        # the queue: three more waiting, each in its own way
        put(sc, r, 250, -20, st(r, LIME), yaw=-70, arm_l='hip', arm_r='hip', rock=4, rate=0.5)
        put(sc, r, 370, -60, st(r, YEL), yaw=-80, arm_r='face', bob=5, rate=1.6)
        put(sc, r, 495, -10, st(r, CREAM), yaw=-20, arm_r='wave', swing_r=28, arm_l='down', rate=1.2, react=True)
        drum(g, 560, 110, col=CREAM, band=VIOLET)
        g.cyl(VIOLET, (560, 110, H + 90), 26, 6, steps=7, glow=0.5)
        # out the other side: hood off, towel up, and the attendant ticks one more off
        put(sc, r, -400, -50, wk(r, hat=None, top=CREAM), arm_l='up', arm_r='up', hold_r=hold_towel(), swing_l=14, swing_r=-14, bob=7, rate=1.4,
            react=True)
        put(sc, r, -530, -10, wk(r, top=ORANGE), yaw=40, arm_l='hold', arm_r='hold', hold_l=CR.hold_clipboard(), swing_r=10, axis_r='z', rate=1.1)
        g.box(CYAN, (-470, 120, H + 45), (120, 50, 90))
        g.box(CREAM, (-470, 120, H + 96), (124, 54, 12))
    return build


def fire_drill(seed=32, L=1300.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet=WHITE, stripe=RED)
        H = H0
        px = -330.0
        # the hand pump: a red tank on wheels, the see-saw beam and a man at each end of it
        g.box(RED, (px, 20, H + 62), (170, 86, 70))
        g.box(GOLD, (px, 20, H + 101), (176, 90, 8))
        for k in (-1, 1):
            rod(g, DARK, (px + k * 60, -34, H + 34), (px + k * 60, 74, H + 34), 34, steps=8)
        g.box(DARK, (px, 20, H + 150), (14, 14, 100))
        beam = sc.part(P.motion(rate=0.8, swing=13.0, pivot=(px, 20.0, H + 196.0), axis=90.0))
        beam.box(GOLD, (px, 20, H + 196), (430, 8, 8))
        for k in (-1, 1):
            beam.box(GOLD, (px + k * 212, -10, H + 196), (8, 70, 8))
        fire = dict(hat='helmet', hat_col=RED, top=YEL, bottom=DARK, band=CREAM)
        put(sc, r, px - 212, -62, wk(r, **fire), arm_l=((118, 70), (128, 80)), arm_r=((118, 70), (128, 80)), swing_l=20, swing_r=20,
            axis_l='x', axis_r='x', rate=0.8, phase=0.0, bob=6)
        put(sc, r, px + 212, -62, wk(r, **fire), arm_l=((118, 70), (128, 80)), arm_r=((118, 70), (128, 80)), swing_l=20, swing_r=20,
            axis_l='x', axis_r='x', rate=0.8, phase=0.5, bob=6)
        # the captain
        put(sc, r, -40, -70, wk(r, hat='helmet', hat_col=CREAM, top=RED, bottom=DARK, band=GOLD), yaw=-30, arm_r='face', hold_r=CR.hold_megaphone(),
            arm_l='point', swing_l=14, axis_l='x', rate=0.9)
        # the hose crew, facing +X, and the jet (it sweeps a little)
        put(sc, r, 150, -40, wk(r, **fire), yaw=90, legs='stride', arm_l='carry', arm_r='carry', rock=3, rate=0.6, phase=0.1)
        put(sc, r, 260, -40, wk(r, **fire), yaw=90, legs='stride', arm_l='fwd', arm_r='fwd', hold_r=hold_nozzle(), rock=3, rate=0.6, phase=0.1)
        tilt_box(g, RUST, (px + 85, 20, H + 50), (150, -20, H + 150), 9, 9)
        jet = sc.part(P.motion(rate=0.6, phase=0.1, swing=5.0, pivot=(300.0, -40.0, H + 170.0), axis=UP))
        pts = ((330, 175), (400, 222), (470, 232), (540, 205), (590, 150))
        for k in range(len(pts) - 1):
            a, b = pts[k], pts[k + 1]
            tilt_box(jet, WATER, (a[0], -40, H + a[1]), (b[0], -40, H + b[1]), 12 + k * 5, 12 + k * 5)
        # what they are putting out: the practice drums and their (painted) flames, steady light
        drum(g, 585, -30, col=NAVY, band=HAZ, h=110, r=36)
        drum(g, 560, 60, col=RUST, band=HAZ, h=100, r=34)
        fl = sc.part(P.motion(rate=0.7, bob=-7.0))
        fl.cyl(ORANGE, (585, -30, H + 110), 28, 70, steps=6, top_r=4, glow=0.5)
        fl.cyl(GOLD, (578, -34, H + 110), 15, 44, steps=5, top_r=2, glow=0.6)
        # the bucket chain
        put(sc, r, -570, 40, wk(r), yaw=70, arm_l='carry', arm_r='down', hold_r=hold_bucket(), swing_r=24, axis_r='x', rate=0.9)
        put(sc, r, 20, 90, wk(r), yaw=110, arm_r='carry', arm_l='down', hold_l=hold_bucket(CYAN), swing_l=24, axis_l='x', rate=0.9)
    return build


def inspection(seed=33, L=1200.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet=WHITE, stripe=TEAL)
        H = H0
        # the visitors, in a file, facing the manager
        for k, x in enumerate((-200, -80, 40)):
            d = plain(r, ('#3a5fa8', '#2b2350', '#5a4a3c')[k], hat_col=CREAM, band=None)
            if k == 2:
                put(sc, r, x, -70, d, yaw=90, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=CR.hold_clipboard(), rate=0.6)
            else:
                put(sc, r, x, -60 + k * 20, d, yaw=80, arm_l='hold', arm_r='hold', hold_l=CR.hold_clipboard(), swing_r=12, axis_r='z', rate=1.3 - k * 0.3)
        put(sc, r, 190, -70, plain(r, MAG, hat_col=GOLD, build='stocky', band=CREAM), yaw=-90, arm_l='out', arm_r='out', swing_l=12, swing_r=-12,
            bob=4, rate=1.0)
        # behind his back: the leak, and the tarpaulin going up over it
        drum(g, 400, 100, col=RUST, band=HAZ)
        drum(g, 470, 110, col=TEAL, band=HAZ)
        drum(g, 435, 105, z=H + 90, col=NAVY, band=VIOLET)
        g.cyl(VIOLET, (430, 30, H), 70, 4, steps=7, glow=0.4)
        tarp = sc.part(P.motion(rate=0.8, bob=-9.0, phase=0.2))
        tarp.box(ORANGE, (435, 40, H + 150), (250, 6, 200))
        tarp.box(CREAM, (435, 36, H + 200), (250, 2, 22))
        put(sc, r, 300, 30, wk(r), yaw=180, arm_l='up', arm_r='up', rock=4, rate=0.8, phase=0.2)
        put(sc, r, 570, 30, wk(r), yaw=180, arm_l='up', arm_r='up', rock=4, rate=0.8, phase=0.7)
        # and the rest of the tidying
        put(sc, r, -400, -30, wk(r), yaw=-60, arm_l='fwd', arm_r='fwd', hold_r=hold_mop(), swing_l=16, swing_r=16, axis_l='x', axis_r='x', rate=1.9)
        g.cyl(HAZ, (-330, -110, H), 20, 56, steps=6, top_r=5)
        put(sc, r, -540, -90, wk(r), yaw=20, arm_r='fwd', hold_r=hold_brush(head=HAZ, length=40), swing_r=20, axis_r='y', arm_l='down',
            hold_l=hold_bucket(HAZ), rate=1.7)
    return build


def meeting(seed=34, L=1200.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet='#ffe1ee', stripe=PINK)
        H = H0
        # the speaker, up on a drum
        g.cyl(NAVY, (-420, -40, H), 44, 70, steps=8)
        g.cyl(HAZ, (-420, -40, H + 60), 46, 10, steps=8)
        put(sc, r, -420, -40, wk(r, top=RED, hat_col=CREAM), z=H + 70, yaw=55, arm_r='face', hold_r=CR.hold_megaphone(), arm_l='up', swing_l=26,
            axis_l='x', rate=1.1, detail=2)
        # the brazier
        g.cyl(RUST, (-560, 60, H), 34, 80, steps=7)
        fl = sc.part(P.motion(rate=0.6, bob=-6.0))
        fl.cyl(ORANGE, (-560, 60, H + 80), 26, 60, steps=6, top_r=4, glow=0.5)
        fl.cyl(GOLD, (-556, 56, H + 80), 14, 38, steps=5, top_r=2, glow=0.6)
        put(sc, r, -520, -90, wk(r), yaw=30, arm_l='fwd', arm_r='fwd', bob=3, rate=0.9, detail=0)
        # the placards (stripes, never words)
        signs = ((CREAM, RED), (YEL, DARK), (CYAN, MAG))
        for k, x in enumerate((-230, 60, 380)):
            put(sc, r, x, 50, wk(r), yaw=-25 + r.uniform(-10, 10), arm_r='up', hold_r=P.hold_sign(signs[k][0], signs[k][1], w=84, h2=56),
                swing_r=10, arm_l='hip', bob=6, rate=1.0 + 0.15 * k, react=True)
        # the floor of the meeting: fists up, out of step
        acts = (dict(arm_r='up', swing_r=18, axis_r='x', bob=7, rate=1.3), dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=9, rate=1.5),
                dict(arm_l='up', swing_l=18, axis_l='x', arm_r='hip', bob=5, rate=1.1), dict(arm_l='hold', arm_r='hold', swing_l=14, swing_r=-14,
                                                                                         axis_l='z', axis_r='z', rate=2.3))
        for k, x in enumerate((-310, -120, 190, 480)):
            put(sc, r, x + r.uniform(-15, 15), -80 + (k % 2) * 40, wk(r, band=None), yaw=-35 + r.uniform(-12, 12), react=True, detail=0,
                **acts[k % len(acts)])
    return build


def tug_of_war(seed=35, L=1300.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet=WHITE, stripe=LIME)
        H = H0
        rate, ph, deg, deep = 0.32, 0.0, 1.4, 3000.0          # everything on the rope slides 73 cm each way, together
        yr, zr = -60.0, H + 118.0 * S

        def slide(x):
            return dict(swing=deg, pivot=(x, yr, H - deep), axis=90.0)

        for k, (xa, xb) in enumerate(((-500.0, -170.0), (-170.0, 170.0), (170.0, 500.0))):
            rp = sc.part(P.motion(rate=rate, phase=ph, **slide((xa + xb) / 2.0)))
            rp.box(CREAM, ((xa + xb) / 2.0, yr - 30, zr), (xb - xa, 7, 7))
            if k == 1:
                rp.box(PINK, (0, yr - 30, zr - 22), (10, 4, 44))
        g.box(CREAM, (0, yr, H + 1), (10, 200, 3))
        for side, top in ((-1, ORANGE), (1, LIME)):
            for k in range(3):
                x = side * (210 + k * 130)
                d = wk(r, top=top, band=CREAM, hat_col=(YEL if side < 0 else CYAN))
                put(sc, r, x, yr + 6, d, yaw=-90 * side, legs='stride', arm_l=((60, 80), (80, 88)), arm_r=((60, 80), (80, 88)),
                    rate=rate, phase=ph, sway=slide(x))
        # the referee, the cup, and somebody for each side
        put(sc, r, 0, 60, wk(r, top=CREAM, hat_col=CREAM, band=RED), arm_r='up', hold_r=P.hold_flag(col=PINK, w=70), swing_r=22, arm_l='hip',
            rate=0.9)
        drum(g, -90, 120, col=NAVY)
        g.cyl(GOLD, (-90, 120, H + 90), 8, 24, steps=6, top_r=22)
        g.cyl(GOLD, (-90, 120, H + 114), 22, 18, steps=6, top_r=26)
        put(sc, r, -400, 70, wk(r, top=ORANGE), arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=9, rate=1.5, react=True, detail=0)
        put(sc, r, 410, 70, wk(r, top=LIME), arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=9, rate=1.3, react=True, detail=0)
        put(sc, r, 560, -20, wk(r), yaw=-50, arm_r='wave', swing_r=26, rate=1.2, react=True, detail=0)
    return build


def capstan(seed=36, L=1150.0, D=480.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, D=D, sheet='#d6f4ff', stripe=TEAL)
        H = H0
        cx, cy, rad, rps = -150.0, 0.0, 168.0, 0.075
        # the valve: its main along the back, the spindle up through the deck, the capstan head
        rod(g, TEAL, (-L / 2.0 + 30, 190, H + 60), (L / 2.0 - 30, 190, H + 60), 48, steps=8)
        g.cyl(HAZ, (cx, cy, H), 40, 26, steps=8)
        g.cyl(STEELB, (cx, cy, H + 26), 20, 150, steps=8)
        turn = dict(base=(cx, cy), travel=(-1.0, rps))
        head = sc.part(P.motion(**turn))
        head.cyl(MAG, (cx, cy, H + 150), 34, 40, steps=8)
        for k in range(4):
            a = math.radians(90.0 * k + 20.0)
            tilt_box(head, CREAM, (cx, cy, H + 172), (cx + math.cos(a) * (rad + 40), cy + math.sin(a) * (rad + 40), H + 172), 9, 9)
        for k in range(4):
            th = 90.0 * k
            a = math.radians(th)
            put(sc, r, cx + math.cos(a) * rad, cy + math.sin(a) * rad, wk(r), yaw=th + 180.0, arm_l='push', arm_r='push', stride=20, rate=1.1,
                orbit=(cx, cy, rps), phase=k * 0.25)
        # the foreman beats time on a drum; the reader at the gauge; the oiler
        drum(g, 250, -120, col=NAVY)
        put(sc, r, 250, -60, wk(r, top=CREAM, hat_col=CREAM, band=ORANGE), yaw=160, arm_r='fwd', hold_r=P.hold_tool(col=RUST, grip=DARK), swing_r=26,
            axis_r='x', arm_l='hip', rate=1.1)
        g.box(POLE, (430, 150, H + 110), (10, 10, 220))
        g.cyl(CREAM, (430, 143, H + 250), 62, 8, rot=(0, 0, 90), steps=10)
        g.box(RED, (476, 134, H + 268), (26, 3, 34))
        nd = sc.part(P.motion(rate=0.5, swing=38.0, pivot=(430.0, 132.0, H + 250.0), axis=90.0))
        nd.box(DARK, (430, 132, H + 276), (7, 3, 58))
        put(sc, r, 480, -20, wk(r), yaw=150, arm_r='point', swing_r=10, axis_r='x', arm_l='hold', hold_l=CR.hold_clipboard(), rate=0.5)
        put(sc, r, 80, 110, wk(r), yaw=200, arm_r='fwd', swing_r=14, axis_r='x', arm_l='down', hold_l=hold_bucket(GOLD), rate=1.4, detail=0)
        # steam at the gland
        for k in range(3):
            p = sc.part(P.motion(rate=0.5, phase=k / 3.0, bob=-26.0))
            p.sphere(CREAM, (cx - 20 + k * 22, cy + 150 + 8 * k, H + 150 + k * 42), 24 + k * 9, steps=(3, 6))
    return build


def briefing(seed=37, L=1200.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet=WHITE, stripe=HAZ)
        H = H0
        # the board (shapes only): a red triangle, a green disc, a row of bars
        for k in (-1, 1):
            tilt_box(g, POLE, (-470 + k * 70, 60, H), (-470 + k * 40, 100, H + 260), 8, 8)
        g.box(NAVY, (-470, 96, H + 190), (200, 8, 170))
        g.cyl(RED, (-520, 91, H + 200), 36, 4, rot=(0, 0, 90), steps=3)
        g.cyl(LIME, (-430, 91, H + 225), 26, 4, rot=(0, 0, 90), steps=8)
        for k in range(3):
            g.box(CREAM, (-430, 91, H + 170 - k * 20), (70 - k * 14, 3, 9))
        put(sc, r, -340, 10, wk(r, top=CREAM, hat_col=CREAM, band=ORANGE), yaw=40, arm_l='up', hold_l=hold_pointer(), swing_l=12, arm_r='hip',
            rate=0.7, detail=2)
        # the shift on its drums, half turned to the board
        acts = (dict(rock=7, rate=0.25, arm_l='down', arm_r='down'),                       # asleep, nodding
                dict(arm_r='up', swing_r=14, rate=1.2),                                    # a hand up
                dict(arm_r='face', hold_r=P.hold_cup(), swing_r=8, axis_r='x', rate=0.6),  # at his tea
                dict(arm_l='wave', swing_l=28, rate=1.3, react=True),                      # waving at the chase
                dict(arm_l='vee', arm_r='vee', swing_l=8, swing_r=-8, rate=0.3),           # a long stretch
                dict(arm_l='hold', arm_r='hold', hold_r=P.hold_phone(), rate=0.4, bob=-1.5))
        for k, (x, y) in enumerate(((-150, -80), (0, -20), (140, -90), (290, -30), (430, -90), (540, -10))):
            seat(g, x, y, col=(RUST, TEAL, NAVY)[k % 3])
            put(sc, r, x, y, wk(r), yaw=(0 if k == 3 else -42 + r.uniform(-8, 8)), legs='sit', **acts[k])
        # the late-comer, at a run along the back
        put(sc, r, 0, 120, wk(r), yaw=-90, stride=34, swing_l=30, swing_r=-30, axis_l='x', axis_r='x', rate=2.2, bob=6,
            travel=(L - 260.0, -230.0))
    return build


def safe_days(seed=38, L=1200.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet=WHITE, stripe=LIME)
        H = H0
        # the tally on the sheet: green days, one red a long time back, the next square still empty
        bx, bz, cell = 150.0, H + 70.0, 62.0
        g.box(NAVY, (bx, 164, H + 172), (6 * cell + 40, 8, 3 * cell + 36))
        for row in range(3):
            for col in range(6):
                k = row * 6 + col
                if k == 17:
                    continue
                g.box(RED if k == 2 else LIME, (bx + (col - 2.5) * cell, 158, bz + (2 - row) * cell + 40), (cell - 12, 4, cell - 12),
                      glow=(0.0 if k == 2 else 0.25))
        # the steps, and the manager hanging the tile
        lx = bx + 2.5 * cell + 30
        for k in (-1, 1):
            tilt_box(g, HAZ, (lx + k * 30, 20, H), (lx + k * 30, 110, H + 150), 8, 8)
            tilt_box(g, HAZ, (lx + k * 30, 150, H), (lx + k * 30, 110, H + 150), 8, 8)
        g.box(HAZ, (lx, 100, H + 76), (60, 44, 8))
        put(sc, r, lx, 96, plain(r, MAG, hat_col=GOLD, band=CREAM), z=H + 80, yaw=150, arm_l='up', hold_l=hold_tile(), swing_l=10, axis_l='x',
            arm_r='hip', rate=0.6, detail=2)
        # the one whose arm is in a sling, and the photographer
        put(sc, r, -30, -40, wk(r, band=None), yaw=-15, arm_l='hold', arm_r='down', rock=3, rate=0.5)
        g.box(CREAM, (-30 - 14 * S, -40 - 16 * S, H + 96 * S), (34 * S, 10 * S, 22 * S))
        put(sc, r, -480, -90, wk(r), yaw=95, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), bob=-2, rate=0.5)
        # the shift, clapping it in
        for k, x in enumerate((-330, -200, 110, 250, 480)):
            d = wk(r)
            q = dict(C.CHEER[(k * 5 + 5) % len(C.CHEER)])
            put(sc, r, x + r.uniform(-12, 12), -80 + (k % 2) * 50, d, yaw=(150.0 if k == 3 else 0.0) + r.uniform(-22, 22), react=True, **q)
        # bunting (pennants, no words) along the top of the sheet
        for k in range(9):
            fl = sc.part(P.motion(rate=0.9, phase=k / 9.0, swing=14.0, pivot=(-520.0 + k * 130.0, 160.0, H + 300.0), axis=0.0))
            fl.cyl((PINK, CYAN, GOLD)[k % 3], (-520 + k * 130.0, 160, H + 252), 3, 46, steps=3, top_r=24)
    return build


def crane_lift(seed=39, L=1200.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet='#ffe9c9', stripe=ORANGE)
        H = H0
        tx, ty, tz = -60.0, -50.0, H + 520.0
        # the davit: a post at the back, its jib out over the deck, a hazard-striped hook block
        g.box(HAZ, (tx, 130, H + 265), (26, 26, 530))
        tilt_box(g, HAZ, (tx, 130, H + 530), (tx, ty - 20, tz + 14), 20, 20)
        tilt_box(g, POLE, (tx, 130, H + 330), (tx, ty + 50, tz), 9, 9)
        g.box(DARK, (tx + 40, 130, H + 120), (50, 40, 50))
        # the load: a vessel on its chain, swinging along the deck
        sw = dict(rate=0.3, phase=0.0, swing=6.0, pivot=(tx, ty, tz), axis=90.0)
        ld = sc.part(P.motion(**sw))
        ld.box(DARK, (tx, ty, tz - 100), (7, 7, 200))
        ld.box(HAZ, (tx, ty, tz - 206), (26, 20, 22))
        ld.cyl(WHITE, (tx, ty, H + 110), 62, 190, steps=8)
        ld.sphere(WHITE, (tx, ty, H + 300), 62, scale=(1, 1, 0.45), steps=(3, 8))
        ld.cyl(MAG, (tx, ty, H + 190), 65, 26, steps=8)
        # the cradle it is going to
        for k in (-1, 1):
            g.box(RUST, (tx + k * 50, ty, H + 14), (24, 150, 28))
        # the banksman, the two steadying it, the winch man, the one who cannot look
        put(sc, r, 300, -80, wk(r, top=CREAM, hat_col=CREAM, band=RED), yaw=-80, arm_l='up', arm_r='up', swing_l=22, swing_r=22, rate=0.6, detail=2)
        put(sc, r, tx - 190, ty - 10, wk(r), yaw=90, legs='stride', arm_l=((95, 80), (120, 85)), arm_r=((95, 80), (120, 85)), rock=4, rate=0.3,
            phase=0.0)
        put(sc, r, tx + 190, ty - 10, wk(r), yaw=-90, legs='stride', arm_l=((95, 80), (120, 85)), arm_r=((95, 80), (120, 85)), rock=4, rate=0.3,
            phase=0.0)
        put(sc, r, tx + 110, 110, wk(r), yaw=120, arm_r='fwd', swing_r=30, axis_r='x', arm_l='hip', rate=1.3)
        put(sc, r, 470, 20, wk(r), yaw=-40, arm_l='face', arm_r='face', rock=3, rate=0.4)
        put(sc, r, -420, -70, wk(r), yaw=60, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.7)
        put(sc, r, -540, 30, wk(r), yaw=30, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=8, rate=1.4, react=True, detail=0)
        for k, x in enumerate((420, 520)):
            g.cyl((ORANGE, CREAM)[k], (x, -130, H), 18, 50, steps=6, top_r=5)
    return build


def band(seed=40, L=1200.0):
    def build(sc):
        r = random.Random(seed)
        g = stage(sc, L, sheet='#ffe1ee', stripe=GOLD)
        H = H0
        kit = dict(hat='peak', hat_col=PINK, bottom=DARK, band=GOLD)

        def man(**kw):
            d = plain(r, PINK, **kit)
            d.update(kw)
            return d

        step = dict(bob=4, rate=1.6)
        # the conductor, his back to the road, on a crate
        g.box(RUST, (0, -110, H + 22), (80, 60, 44))
        put(sc, r, 0, -110, man(top=CREAM, hat_col=CREAM), z=H + 44, yaw=180, arm_l='out', arm_r='out', swing_l=24, swing_r=24, rate=0.8,
            hold_r=hold_pointer(length=40), detail=2)
        # the front rank
        put(sc, r, -330, -40, man(), yaw=12, arm_l='face', arm_r='face', hold_r=hold_horn(), **step)
        put(sc, r, -180, -10, man(), yaw=6, arm_l='face', arm_r='face', hold_r=hold_horn(), phase=0.5, **step)
        put(sc, r, 190, -10, man(), yaw=-8, arm_l='hold', arm_r='fwd', hold_r=hold_trombone(), swing_r=12, axis_r='x', rate=0.8)
        put(sc, r, 350, -40, man(build='stocky'), yaw=-14, arm_l='hold', arm_r='hold', hold_l=hold_tuba(), **step)
        # the back rank: the bass drum, the cymbals, the banner
        rod(g, CREAM, (-470, 20, H + 150), (-430, 20, H + 150), 62, steps=10)
        rod(g, PINK, (-474, 20, H + 150), (-470, 20, H + 150), 66, steps=10)
        rod(g, PINK, (-430, 20, H + 150), (-426, 20, H + 150), 66, steps=10)
        g.box(DARK, (-450, 20, H + 44), (30, 60, 88))
        put(sc, r, -540, 20, man(), yaw=75, arm_r='fwd', hold_r=P.hold_tool(col=CREAM, grip=DARK, length=24), swing_r=24, axis_r='y', arm_l='hip',
            rate=1.6)
        put(sc, r, 60, 90, man(), arm_l='hold', arm_r='hold', hold_l=hold_cymbal(), hold_r=hold_cymbal(), swing_l=16, swing_r=-16, axis_l='z',
            axis_r='z', rate=0.8)
        put(sc, r, 500, 60, man(), yaw=-20, arm_r='up', hold_r=P.hold_flag(col=CYAN, w=96), swing_r=14, arm_l='hip', rate=0.8)
    return build


BIGGER = dict(grow=(120.0, 120.0, 120.0), grow_down=(120.0, 120.0, 0.0))
JOBS = [
    ('SM_P3B_Decon', C.lit(decon()), BIGGER),
    ('SM_P3B_FireDrill', C.lit(fire_drill()), BIGGER),
    ('SM_P3B_Inspection', C.lit(inspection()), BIGGER),
    ('SM_P3B_Meeting', C.lit(meeting()), BIGGER),
    ('SM_P3B_TugOfWar', C.lit(tug_of_war()), BIGGER),
    ('SM_P3B_Capstan', C.lit(capstan()), BIGGER),
    ('SM_P3B_Briefing', C.lit(briefing()), BIGGER),
    ('SM_P3B_SafeDays', C.lit(safe_days()), BIGGER),
    ('SM_P3B_CraneLift', C.lit(crane_lift()), BIGGER),
    ('SM_P3B_Band', C.lit(band()), BIGGER),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
