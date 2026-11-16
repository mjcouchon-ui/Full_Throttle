"""FULL THROTTLE (Unreal) the population of the THEME PARK, pass 3, builder p3b: SCENES (/Game/People/ThemePark/SM_P3B_...).

Ten story groups that could only be in Coaster Cove, one mesh a scene, 9 to 13 figures each doing something different,
with the props that tell the story. What the first rounds learned about this level decides how they are built: the
road has a metre of barrier and a hoarding behind it, and a row of stalls 3.2 m high in many slots, so a person on the
ground is not seen. Every scene here therefore stands on a STAGE OF ITS OWN (a striped rostrum 2.6 m high, a coloured
back cloth, a row of steady lamps), is built 1.2 times life size and is placed at 1.5 times its built size, 40 m out:
the deck is then 3.9 m up and the cast 3 m tall. It is a night level: every colour carries a steady glow (the park
kit's lift()); nothing blinks. No clown and no balloon anywhere (the boss is MR. CHUCKLES, who flies under balloons).

    SM_P3B_Wedding     a wedding under a heart arch: the couple, the hippo marrying them, guests throwing confetti
                       (it drifts), a flower child, the photographer, an usher with a ribbon, two doves circling.
    SM_P3B_Birthday    a birthday table: a three-tier cake with candles, children in party hats hopping in their
                       seats, the birthday child standing on a chair, a waiter going to and fro with a tray, a parent
                       filming, the frog bringing a present.
    SM_P3B_BigPrize    the high striker: a guest swings the mallet, the puck runs up to the bell and falls, the barker
                       with a megaphone, the winner holding a giant bear overhead, a rack of prizes, people clapping.
    SM_P3B_Dance       the giraffe's dance class: two rows of children and grown-ups doing the same move in step,
                       one child half a beat out, the music box, parents filming.
    SM_P3B_Magic       the magic show: the magician in a top hat and cape, the assistant floating over a draped
                       table, a rabbit popping out of a hat, two doves, great playing cards, children on a bench.
    SM_P3B_PieContest  the pie-eating contest: four eaters nodding into their pies at a long table, the judge with a
                       flag, last year's winner on a box, the tally of pies, the crowd behind.
    SM_P3B_RideDrill   the evacuation drill: a coaster car stopped on a piece of track, riders waving from it, staff
                       in orange bands on a ladder, with a megaphone, a clipboard and a held-out sheet, cones.
    SM_P3B_Backstage   behind the scenes: the hippo with its head off having a drink, somebody fanning them, the
                       giraffe stretching, a dancer warming up, the costume rail swaying, a mirror in lamps, a runner
                       with a costume head.
    SM_P3B_Treasure    the children's treasure hunt (Pirate Cove): a sand pit, an X, children digging, the chest's lid
                       lifting, the captain pointing with a spyglass, a parrot on a perch, a map, a palm.
    SM_P3B_Cadets      the space cadets' passing-out (Rocket Land): a toy rocket on its pad, a row of children in
                       helmets saluting, the commander with the medals, the frog in a helmet, planets going round.
    SM_P3B_Wedding_Sky / _Dance_Sky / _Magic_Sky
                       three of them again on a tower 18 m high (27 m as placed) for THE LAGOON, where the road is the
                       coaster 26 m over the valley: the scene comes up to the rails. The legs go 4 m under z = 0.

Run in the editor:  import ft_people_park_p3b as K; K.run()
The placing rules are C:/ftwork/p3/L4b.inl (the integrator pastes them into FTPeopleL4.inl).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
import ft_people_park as K
from ft_desert import rod, tilt_box

DIR = '/Game/People/ThemePark'
NAVY, PURPLE, PLUM = K.NAVY, K.PURPLE, K.PLUM
MAG, CYAN, AMBER, CREAM = K.MAG, K.CYAN, K.AMBER, K.CREAM
RED, WHITE, LAV, MINT, TEAL = K.RED, K.WHITE, K.LAV, K.MINT, K.TEAL
ORANGE, GOLD, WOOD, WOOD_D, GREY_L, DARK = K.ORANGE, K.GOLD, K.WOOD, K.WOOD_D, K.GREY_L, K.DARK
LIME, VIOLET, SILVER, SAND, LEAF, BEAR = '#b8ff5a', '#8f6bff', '#d7e3f0', '#f1d9a0', '#3fbf6a', '#c98a5a'
SZ = 1.2            # the cast is built this much bigger than life (and placed at 1.5 times: 3 m people on a 3.9 m deck)


# ------------------------------------------------------------------------------------------ helpers
def F(sc, x, y, d, z=0.0, yaw=0.0, **kw):
    """One figure of a scene: d is a look (K.guest, K.staff, K.kid), kw what this one does."""
    d = dict(d)
    d.update(kw)
    d['s'] = d.get('s', 1.0) * SZ
    d['detail'] = 1 if d.get('detail', 0) >= 2 else 0      # 36 m out and more: a scene's leads (asked for as 2) at detail 1, the rest at 0
    return P.figure(sc, x, y, z=z, yaw=yaw, **d)


def on_body(sc, x, y, z=0.0, yaw=0.0, s=1.0, phase=0.0, rate=0.0, bob=0.0, react=False):
    """The model a plain figure's body is drawn into (same movement), in the figure's own frame: for a hat or a cape
    the figure builder does not have. s: the figure's size before SZ."""
    s = s * SZ
    mo = P.motion(phase=phase, rate=rate, bob=bob, react=react, base=(x, y), swing=0.0, pivot=(x, y, z + s * 100.0), axis=yaw)
    return sc.part(mo, (x, y, z), yaw, s)


def cone_hat(sc, x, y, col, z=0.0, yaw=0.0, s=1.0, top=176.0, **mv):
    m = on_body(sc, x, y, z, yaw, s, **mv)
    m.cyl(col, (0, 0, top - 6.0), 17, 40, steps=6, top_r=2.0)
    m.sphere(WHITE, (0, 0, top + 36.0), 6, steps=(3, 5))


def raise_all(sc, dz):
    """Everything drawn so far goes up by dz (the pivots of its movements with it)."""
    xf = unreal.Transform(location=unreal.Vector(0.0, 0.0, float(dz)))
    for key in sc.order:
        model, mo = sc.parts[key]
        for pm in model.meshes.values():
            unreal.GeometryScript_MeshTransforms.transform_mesh(pm, xf)
        pv = mo['pivot']
        mo['pivot'] = (pv[0], pv[1], pv[2] + dz)


def _back(g, H, L, W, cols, back):
    """The back cloth, its valance and lamps, the two end posts."""
    g.box(back, (0, W / 2.0 - 12, H + 175), (L, 18, 350))
    K.stripes(g, -L / 2.0, L / 2.0, W / 2.0 - 30, H + 350, H + 410, (cols[0], CREAM, cols[1], CREAM), 8, depth=44)
    K.bulbs(g, (-L / 2.0 + 30, W / 2.0 - 56, H + 340), (L / 2.0 - 30, W / 2.0 - 56, H + 340), 6, size=16.0)
    for sx in (-1, 1):
        g.box(PLUM, (sx * (L / 2.0 - 14), W / 2.0 - 14, H + 215), (28, 28, 430))
        g.sphere(GOLD, (sx * (L / 2.0 - 14), W / 2.0 - 14, H + 446), 22, steps=(3, 6))


def staged(fn, cols=(MAG, CREAM), back=PURPLE, H=260.0, L=1120.0, W=520.0, g=0.09):
    """fn draws the scene on z = 0; it is then stood on a rostrum H high (its skirt goes 1.6 m under z = 0, for
    falling ground) before a back cloth, and lit."""
    def build(sc):
        fn(sc)
        raise_all(sc, H)
        m = sc.still()
        foot = -160.0
        m.box(PLUM, (0, 0, (H - 12 + foot) / 2.0), (L - 30, W - 30, H - 12 - foot))
        K.stripes(m, -L / 2.0, L / 2.0, -W / 2.0, foot, H - 12, cols, 8)
        m.box(GOLD, (0, 0, H - 6), (L + 16, W + 16, 12))
        K.bulbs(m, (-L / 2.0 + 20, -W / 2.0 - 12, H - 6), (L / 2.0 - 20, -W / 2.0 - 12, H - 6), 8, size=16.0)
        _back(m, H, L, W, cols, back)
        K.lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def towered(fn, cols=(MAG, CREAM), back=PURPLE, H=1800.0, L=1120.0, W=520.0, g=0.1):
    """The same scene on a tower H high for the lagoon: four banded legs that go 4 m under z = 0, braces, a deck with a
    striped fascia and lamps, the back cloth."""
    def build(sc):
        fn(sc)
        raise_all(sc, H)
        m = sc.still()
        lx, ly = L / 2.0 - 70, W / 2.0 - 60
        for sx in (-1, 1):
            for sy in (-1, 1):
                m.box(PLUM, (sx * lx, sy * ly, (H - 400) / 2.0), (56, 56, H + 400))
                for k in range(3):
                    m.box(cols[0] if k % 2 == 0 else CREAM, (sx * lx, sy * ly, 350 + k * 520), (64, 64, 150))
        for k in range(3):
            z0, z1 = 120 + k * 540, 120 + (k + 1) * 540
            tilt_box(m, GREY_L, (-lx if k % 2 == 0 else lx, -ly, z0), (lx if k % 2 == 0 else -lx, -ly, z1), 18, 18)
        m.box(CREAM, (0, 0, H - 10), (L + 20, W + 20, 20))
        K.stripes(m, -L / 2.0 - 10, L / 2.0 + 10, -W / 2.0 - 12, H - 110, H - 20, (cols[0], CREAM, cols[1], CREAM), 8)
        m.box(GOLD, (0, -W / 2.0 - 12, H - 4), (L + 30, 8, 12))
        K.bulbs(m, (-L / 2.0 + 20, -W / 2.0 - 20, H - 62), (L / 2.0 - 20, -W / 2.0 - 20, H - 62), 8, size=18.0)
        _back(m, H, L, W, cols, back)
        K.lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=10, rate=1.5),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4, bob=2),      # clapping
    dict(arm_r='wave', swing_r=26, arm_l='hip', bob=4, rate=1.2),
    dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=12, rate=1.3),
    dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7),
)


def film(r):
    """Somebody filming it on a phone (no glow: a figure may be turned towards the road)."""
    return dict(arm_r='face', hold_r=P.hold_phone(), arm_l='hip', rate=0.3, bob=-1.5, phase=r.random())


def bench(m, x, y, L=300.0, col=WOOD, yaw=0.0):
    old = m.base
    m.base = P.ft_art._xf((x, y, 0), (0.0, yaw, 0.0), (1, 1, 1))
    m.box(col, (0, 0, 50), (L, 46, 8))
    for sx in (-1, 1):
        m.box(WOOD_D, (sx * (L / 2.0 - 20), 0, 23), (10, 40, 46))
    m.base = old


def table(m, x, y, L, W, H, cloth, trim):
    m.box(cloth, (x, y, H - 20), (L, W, 44))
    m.box(trim, (x, y, H + 3), (L + 8, W + 8, 6))
    for sx in (-1, 1):
        m.box(WOOD_D, (x + sx * (L / 2.0 - 16), y, (H - 40) / 2.0), (10, W - 20, H - 40))


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_bouquet():
    def f(m, h, side):
        m.box(LEAF, (h[0], h[1] - 4, h[2] + 2), (5, 5, 22))
        for dx, dz, c in ((-8, 16, MAG), (8, 16, WHITE), (0, 26, AMBER)):
            m.sphere(c, (h[0] + dx, h[1] - 6, h[2] + dz), 10, steps=(3, 5))
    return f


def hold_mallet():
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 46), (6, 6, 112))
        m.box(RED, (h[0], h[1], h[2] + 104), (28, 48, 28))
    return f


def hold_bear(col=BEAR):
    """A giant plush bear held up in both hands (drawn once, from the right hand)."""
    def f(m, h, side):
        if side < 0:
            return
        z = h[2] + 52
        m.sphere(col, (0, h[1], z), 40, scale=(1.0, 0.8, 1.05), steps=(4, 6))
        m.sphere(CREAM, (0, h[1] - 26, z - 2), 20, steps=(3, 5))
        m.sphere(col, (0, h[1], z + 58), 28, steps=(4, 6))
        m.box(DARK, (0, h[1] - 26, z + 56), (10, 6, 8))
        for sx in (-1, 1):
            m.sphere(col, (sx * 22, h[1], z + 84), 11, steps=(3, 5))
            m.box(col, (sx * 44, h[1], z + 14), (26, 20, 20))
            m.box(col, (sx * 22, h[1] - 8, z - 44), (22, 30, 20))
    return f


def hold_wand():
    def f(m, h, side):
        m.box(DARK, (h[0], h[1], h[2] + 30), (5, 5, 62))
        m.box(WHITE, (h[0], h[1], h[2] + 64), (6, 6, 12))
    return f


def hold_fan(col=AMBER):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 10), (5, 5, 30))
        m.box(col, (h[0], h[1] - 2, h[2] + 46), (62, 4, 50))
        m.box(WHITE, (h[0], h[1] - 4.5, h[2] + 46), (44, 1.5, 10))
    return f


def hold_spade(col=CYAN):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 14, h[2] - 16), (5, 5, 70), rot=(0, 0, 0))
        m.box(col, (h[0], h[1] - 16, h[2] - 58), (22, 5, 26))
    return f


def hold_map():
    def f(m, h, side):
        if side < 0:
            return
        cx = h[0] - 24
        m.box(CREAM, (cx, h[1] - 6, h[2] + 12), (56, 3, 40))
        tilt_box(m, RED, (cx - 10, h[1] - 8.2, h[2] + 4), (cx + 6, h[1] - 8.2, h[2] + 20), 1.5, 5)
        tilt_box(m, RED, (cx - 10, h[1] - 8.2, h[2] + 20), (cx + 6, h[1] - 8.2, h[2] + 4), 1.5, 5)
    return f


def hold_spyglass():
    def f(m, h, side):
        tilt_box(m, GOLD, (h[0], h[1] + 4, h[2] + 4), (h[0], h[1] - 44, h[2] + 10), 8, 8)
        tilt_box(m, DARK, (h[0], h[1] - 44, h[2] + 10), (h[0], h[1] - 62, h[2] + 12), 11, 11)
    return f


def hold_tray(cols=(CYAN, MAG, AMBER)):
    def f(m, h, side):
        if side < 0:
            return
        cx = h[0] - 24
        m.box(SILVER, (cx, h[1] - 8, h[2] + 10), (60, 38, 4))
        for k, c in enumerate(cols):
            m.cyl(c, (cx - 18 + k * 18, h[1] - 8, h[2] + 12), 6, 17, steps=6, top_r=7.5)
    return f


def hold_present(col=MAG, ribbon=WHITE, w=54.0):
    def f(m, h, side):
        if side < 0:
            return
        cx = h[0] - 24
        m.box(col, (cx, h[1] - 10, h[2] + 26), (w, w * 0.8, w * 0.8))
        m.box(ribbon, (cx, h[1] - 10, h[2] + 26), (9, w * 0.8 + 2, w * 0.8 + 2))
        m.box(ribbon, (cx, h[1] - 10, h[2] + 26 + w * 0.4 + 5), (26, 10, 10))
    return f


def hold_sheet(col=WHITE, w=230.0):
    """One end of a sheet held out level between two people (each draws half)."""
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0], h[1] - w / 2.0, h[2] + 2), (120, w, 5))
            m.box(RED, (h[0], h[1] - w / 2.0, h[2] + 5), (30, 30, 2))
    return f


def hippo_head(m, x, y, z):
    """The hippo's costume head, off, standing on something."""
    m.sphere(LAV, (x, y, z + 44), 46, scale=(1.12, 1.0, 0.95), steps=(4, 8))
    m.box(LAV, (x, y - 36, z + 26), (80, 48, 42))
    for sx in (-1, 1):
        m.sphere(LAV, (x + sx * 32, y + 6, z + 86), 10, steps=(3, 5))
        m.sphere(WHITE, (x + sx * 19, y - 34, z + 68), 9, steps=(3, 5))
        m.box(DARK, (x + sx * 19, y - 42, z + 68), (6, 3, 8))
        m.box(WHITE, (x + sx * 20, y - 56, z + 4), (10, 8, 12))


# ------------------------------------------------------------------------------------------ the scenes (drawn on z = 0)
def wedding(sc):
    """A wedding under the heart arch: the hippo marries them, confetti drifts, doves circle."""
    r = random.Random(301)
    g = sc.still()
    for sx in (-1, 1):
        g.box(WHITE, (sx * 170, 150, 195), (22, 22, 390))
        for k in range(4):
            g.box((MAG, CREAM, '#ff9ad0', AMBER)[k], (sx * 170, 136, 70 + k * 92), (38, 30, 38), rot=(45, 0, 0))
    g.box(WHITE, (0, 150, 400), (384, 22, 24))
    for sx in (-1, 1):
        g.sphere(RED, (sx * 27, 144, 462), 34, steps=(4, 6))
    g.box(RED, (0, 144, 430), (54, 44, 54), rot=(45, 0, 0))
    g.box(RED, (0, -60, 2), (130, 380, 4))                                              # the carpet
    F(sc, -58, 60, K.guest(r, wear='dress', top=WHITE, bottom=WHITE, hair='long', shoes=WHITE), yaw=38, arm_l='hold', arm_r='hold',
      hold_r=hold_bouquet(), rock=5, rate=0.5, detail=2)
    F(sc, 58, 60, K.guest(r, top=NAVY, bottom=NAVY, sleeves='long', wear='trousers', hair='short', band=WHITE), yaw=-38, arm_l='hold', arm_r='hip',
      rock=5, rate=0.5, phase=0.5, detail=2)
    K.mascot(sc, 'hippo', 0, 165, s=1.3, phase=0.2, rate=0.8, bob=4, arm_l='out', arm_r='wave', swing_r=18)
    spots = ((-330, 20, 55), (-450, 110, 60), (-420, -110, 45), (330, 20, -55), (450, 110, -60), (420, -110, -45))
    for k, (x, y, yaw) in enumerate(spots):
        d = K.guest(r) if k % 3 else K.guest(r, wear='dress', hat='sun', hat_col=(MAG, CYAN)[k % 2])
        F(sc, x, y, d, yaw=yaw, phase=r.random(), react=True, detail=0 if k % 2 else 1, **CHEER[(k * 2) % 4])
    F(sc, -150, -170, K.kid(r, wear='dress', top=AMBER), yaw=25, arm_r='hold', arm_l='down', hold_r=P.hold_bag(col=CREAM), bob=9, rate=1.1)
    F(sc, 170, -190, K.guest(r, top=DARK), yaw=165, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rate=0.3, bob=-2.0)
    F(sc, 250, 150, K.staff(r), yaw=-20, arm_r='wave', swing_r=40, hold_r=K.hold_ribbon(MAG), arm_l='out', swing_l=16, bob=6, rate=0.9)
    cols = (MAG, CYAN, AMBER, WHITE, LIME, RED)
    for k in range(2):
        c = sc.part(P.motion(bob=-38.0, rate=0.4, phase=0.5 * k))
        for j in range(7):
            c.box(cols[(j + k) % 6], (r.uniform(-190, 190), r.uniform(-20, 120), r.uniform(250, 370)), (10, 10, 5))
    P.bird(sc, 130, 150, 540, heading=90, span=96, orbit=(0, 150, 0.2), phase=0.1, rate=2.4)
    P.bird(sc, -130, 150, 560, heading=270, span=96, orbit=(0, 150, 0.2), phase=0.6, rate=2.4)


def birthday(sc):
    """A birthday table: the cake and its candles, children in party hats, a waiter going to and fro."""
    r = random.Random(307)
    g = sc.still()
    table(g, 0, -20, 640, 110, 92, CREAM, MAG)
    g.cyl(CREAM, (0, -20, 96), 62, 40, steps=10)
    g.cyl(MAG, (0, -20, 136), 64, 8, steps=10)
    g.cyl(CYAN, (0, -20, 144), 44, 36, steps=10)
    g.cyl(AMBER, (0, -20, 180), 46, 7, steps=10)
    g.cyl(CREAM, (0, -20, 187), 28, 30, steps=8)
    for k in range(5):
        a = k * math.pi * 2.0 / 5.0
        g.box(RED, (math.cos(a) * 18, -20 + math.sin(a) * 18, 229), (5, 5, 24))
        g.box(AMBER, (math.cos(a) * 18, -20 + math.sin(a) * 18, 246), (7, 7, 10), glow=0.9)
    for k, px in enumerate((-250, -170, 170, 250)):
        g.cyl((CYAN, AMBER, LIME, MAG)[k], (px, -40, 96), 9, 20, steps=6, top_r=11)
        g.cyl(WHITE, (px + 30, -10, 96), 20, 4, steps=8)
    bench(g, -190, 70, 300, MAG)
    bench(g, 190, 70, 300, CYAN)
    hats = (CYAN, AMBER, MAG, LIME)
    for k, px in enumerate((-290, -190, -90, 110, 290)):
        mv = dict(phase=r.random(), rate=1.2 + 0.2 * (k % 3), bob=7 + 3 * (k % 2))
        d = K.kid(r)
        F(sc, px, 70, d, legs='sit', seat=50, react=True, arm_l=('up', 'vee', 'hold')[k % 3], arm_r=('up', 'wave', 'hold')[k % 3],
          swing_r=(-12, 22, -12)[k % 3], swing_l=(12, 0, 12)[k % 3], axis_l=('y', 'y', 'z')[k % 3], axis_r=('y', 'y', 'z')[k % 3], **mv)
        cone_hat(sc, px, 70, hats[k % 4], s=d['s'], top=184.0 - (66.0 - 50.0), react=True, **mv)
    # the birthday child on a chair at the table's end
    g.box(WOOD, (400, -20, 30), (60, 60, 60))
    mv = dict(phase=0.15, rate=1.4, bob=14)
    d = K.kid(r, top=AMBER, wear='dress')
    F(sc, 400, -20, d, z=60, yaw=-25, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, detail=2, **mv)
    cone_hat(sc, 400, -20, RED, z=60, yaw=-25, s=d['s'], top=184.0, **mv)
    # the waiter with a tray, behind the table, to and fro across the stage
    F(sc, 0, 160, K.staff(r), yaw=90, stride=22, rate=0.9, bob=3, arm_l='carry', arm_r='carry', hold_r=hold_tray(), travel=(900.0, 70.0))
    F(sc, -430, -120, K.guest(r), yaw=60, **film(r))
    F(sc, -380, 110, K.guest(r, wear='dress'), yaw=40, phase=r.random(), **CHEER[1])
    K.mascot(sc, 'frog', 470, 130, yaw=-30, s=1.25, phase=0.4, rate=1.0, bob=6, arm_l='hip', arm_r='wave', swing_r=24)
    pm = sc.part(P.motion(bob=6.0, rate=1.0, phase=0.4))
    pm.box(MAG, (400, 150, 34), (64, 54, 56))
    pm.box(WHITE, (400, 150, 34), (10, 56, 58))
    pm.box(WHITE, (400, 150, 68), (30, 12, 12))
    K.bunting(g, (-520, 200, 330), (520, 200, 330), n=8)


def big_prize(sc):
    """The high striker: the mallet comes down, the puck runs up to the bell, the winner holds a giant bear overhead."""
    r = random.Random(311)
    g = sc.still()
    TX = 250.0
    g.box(PLUM, (TX, 60, 20), (150, 110, 40))
    g.box(CREAM, (TX, 60, 250), (54, 20, 420))
    for k in range(6):
        g.box((LIME, AMBER, ORANGE, RED, MAG, VIOLET)[k], (TX, 48, 75 + k * 62), (40, 6, 52))
    g.box(PLUM, (TX, 60, 470), (110, 26, 24))
    g.sphere(GOLD, (TX, 56, 512), 34, steps=(4, 8), glow=0.3)
    g.box(RED, (TX - 120, -20, 16), (70, 60, 32))                                       # the pad the mallet hits
    puck = sc.part(P.motion(bob=380.0, rate=0.5, phase=0.0))
    puck.box(WHITE, (TX, 40, 58), (46, 10, 22))
    F(sc, TX - 205, -10, K.guest(r, build='stocky', top=RED, sleeves='none', hair='bald'), yaw=75, legs='wide', arm_r='up', arm_l='up',
      hold_r=hold_mallet(), swing_r=58, swing_l=58, axis_r='x', axis_l='x', rate=0.5, phase=0.25, detail=2)
    F(sc, TX + 150, -60, K.staff(r), yaw=-30, arm_r='face', hold_r=C.hold_megaphone(), arm_l='point', swing_l=12, axis_l='x', rate=0.7, bob=3, detail=2)
    # the prizes on their rack
    g.box(WOOD, (-330, 190, 150), (360, 50, 10))
    g.box(WOOD, (-330, 190, 20), (360, 50, 10))
    for sx in (-1, 1):
        g.box(WOOD_D, (-330 + sx * 175, 190, 110), (10, 50, 220))
    for k, c in enumerate((MAG, CYAN, LIME)):
        px = -440 + k * 110
        g.sphere(c, (px, 184, 196), 38, scale=(1.0, 0.8, 1.05), steps=(4, 6))
        g.sphere(c, (px, 184, 250), 25, steps=(3, 6))
        for sx in (-1, 1):
            g.sphere(c, (px + sx * 20, 184, 274), 10, steps=(3, 5))
        g.box(DARK, (px, 160, 250), (10, 5, 7))
    for k, c in enumerate((AMBER, VIOLET, ORANGE)):
        g.sphere(c, (-440 + k * 110, 184, 62), 34, scale=(1.0, 0.8, 1.0), steps=(3, 6))
    # the winner, the bear overhead
    F(sc, -160, -90, K.guest(r, wear='dress', top=CYAN, hair='afro'), yaw=10, arm_l='up', arm_r='up', hold_r=hold_bear(), bob=14, rate=1.3, phase=0.3,
      react=True, detail=2)
    F(sc, -300, -30, K.kid(r), yaw=35, phase=r.random(), **CHEER[3])
    F(sc, -430, -120, K.guest(r), yaw=45, phase=r.random(), **CHEER[1])
    F(sc, -470, 40, K.guest(r, hat='cap', hat_col=RED), yaw=50, phase=r.random(), detail=0, **CHEER[0])
    F(sc, 20, 120, K.guest(r), yaw=40, phase=r.random(), detail=0, **CHEER[4])
    F(sc, 440, 80, K.guest(r, build='stocky'), yaw=-55, arm_l='hip', arm_r='hip', rock=5, rate=0.4)          # next in line
    F(sc, 480, -90, K.kid(r), yaw=-60, phase=r.random(), **CHEER[2])
    F(sc, 110, -190, K.guest(r), yaw=150, **film(r))


def dance(sc):
    """The giraffe's dance class: two rows doing the same move in step, one child half a beat out."""
    r = random.Random(313)
    g = sc.still()
    g.cyl(MAG, (-400, 90, 0), 90, 46, steps=10)
    g.cyl(CREAM, (-400, 90, 46), 94, 6, steps=10)
    K.mascot(sc, 'giraffe', -400, 90, z=52, yaw=40, s=1.15, phase=0.0, rate=1.0, bob=8, arm_l='wave', arm_r='wave', swing_r=30, swing_l=-30)
    step = dict(arm_l='out', arm_r='out', swing_l=34, swing_r=34, rate=1.0, bob=8)
    twist = dict(arm_l='vee', arm_r='hip', turn=28, rate=1.0, bob=6)
    k = 0
    for row, (y, xs) in enumerate(((-110, (-190, -40, 110, 260, 410)), (70, (-110, 40, 190, 340)))):
        for x in xs:
            d = K.kid(r) if k % 3 != 1 else K.guest(r, wear=('dress', 'shorts')[k % 2])
            mv = dict(step if row == 0 else twist)
            mv['phase'] = 0.5 if k == 4 else 0.0                    # the child at the end of the front row is out of step
            F(sc, x, y, d, yaw=22, detail=1 if row == 0 else 0, **mv)
            k += 1
    # the music box and whoever minds it
    g.box(DARK, (480, 170, 55), (120, 70, 110))
    for sx in (-1, 1):
        g.cyl(SILVER, (480 + sx * 30, 134, 60), 22, 4, rot=(0, 0, 90), steps=8)
    g.box(MAG, (480, 134, 100), (90, 4, 10))
    box = sc.part(P.motion(bob=4.0, rate=2.0))
    box.box(CYAN, (480, 170, 114), (60, 30, 8))
    F(sc, 360, 180, K.staff(r), yaw=-20, phase=0.0, **CHEER[1])
    F(sc, -250, 190, K.guest(r), yaw=10, **film(r))
    F(sc, -120, 200, K.guest(r, wear='dress'), yaw=0, phase=r.random(), detail=0, **CHEER[2])
    K.bunting(g, (-520, 205, 320), (520, 205, 320), n=8)


def magic(sc):
    """The magic show: the assistant floats over the table, a rabbit pops out of the hat, doves circle."""
    r = random.Random(317)
    g = sc.still()
    # great playing cards on the back cloth (shapes only)
    for k, (cx, col) in enumerate(((-380, RED), (-250, DARK), (250, DARK), (380, RED))):
        g.box(WHITE, (cx, 222, 230), (104, 6, 160), rot=(0, 0, 0))
        g.box(col, (cx, 217, 230), (40, 3, 40), rot=(45, 0, 0))
        for sx, sz in ((-1, 1), (1, -1)):
            g.box(col, (cx + sx * 36, 217, 230 + sz * 60), (12, 3, 12), rot=(45, 0, 0))
    # the magician: top hat, cape, wand
    mv = dict(phase=0.0, rate=0.6, bob=3)
    d = K.guest(r, top=DARK, bottom=DARK, sleeves='long', wear='trousers', hair='short', band=RED, build='slim')
    F(sc, -150, 20, d, yaw=35, arm_r='up', hold_r=hold_wand(), swing_r=22, arm_l='point', swing_l=10, axis_l='x', detail=2, **mv)
    hm = on_body(sc, -150, 20, 0, 35, d['s'], **mv)
    hm.cyl(DARK, (0, 0, 168), 34, 5, steps=8)
    hm.cyl(DARK, (0, 0, 172), 22, 44, steps=8)
    hm.cyl(RED, (0, 0, 174), 23, 9, steps=8)
    hm.box(RED, (0, 20, 78), (62, 6, 96))
    hm.box(DARK, (0, 17, 78), (56, 4, 90))
    # the draped table and the assistant floating over it
    table(g, 90, 40, 250, 80, 70, VIOLET, GOLD)
    F(sc, 90, 40, K.guest(r, wear='dress', top=CYAN, bottom=CYAN, hair='long', shoes=WHITE), z=118, yaw=0, arm_l='out', arm_r='out',
      swing_l=8, swing_r=-8, bob=-34, rate=0.25, phase=0.2, detail=2)
    ring = sc.part(P.motion(bob=-60.0, rate=0.5, phase=0.6))
    for a in range(8):
        a0, a1 = a * math.pi / 4.0, (a + 1) * math.pi / 4.0
        tilt_box(ring, GOLD, (90 + math.cos(a0) * 96, 40 + math.sin(a0) * 96, 210), (90 + math.cos(a1) * 96, 40 + math.sin(a1) * 96, 210), 7, 7)
    # the rabbit in the hat, on its own stand
    g.box(PLUM, (-330, 60, 45), (60, 60, 90))
    g.cyl(DARK, (-330, 60, 90), 44, 5, steps=8)
    g.cyl(DARK, (-330, 60, 94), 30, 46, steps=8)
    rab = sc.part(P.motion(bob=46.0, rate=0.45, phase=0.5))
    rab.sphere(WHITE, (-330, 60, 118), 24, steps=(4, 6))
    for sx in (-1, 1):
        rab.box(WHITE, (-330 + sx * 10, 60, 152), (9, 6, 44))
        rab.box(MAG, (-330 + sx * 10, 56, 152), (4, 2, 30))
        rab.box(DARK, (-330 + sx * 9, 38, 124), (5, 4, 6))
    P.bird(sc, 60, 100, 470, heading=90, span=100, orbit=(-60, 100, 0.22), phase=0.2, rate=2.4)
    P.bird(sc, -180, 100, 500, heading=270, span=100, orbit=(-60, 100, 0.22), phase=0.7, rate=2.4)
    # the audience: children on a bench at the front, their backs to the road, two standing at the sides
    bench(g, 300, -170, 420, CYAN)
    for k, px in enumerate((130, 230, 330, 430)):
        F(sc, px, -170, K.kid(r), yaw=180 + (15, 5, -10, -20)[k], legs='sit', seat=50, phase=r.random(), react=True, **CHEER[(0, 1, 3, 1)[k]])
    F(sc, 500, 40, K.guest(r), yaw=-70, phase=r.random(), **CHEER[4])
    F(sc, -470, -100, K.guest(r, wear='dress'), yaw=60, phase=r.random(), **CHEER[1])
    F(sc, -420, 150, K.staff(r), yaw=30, arm_l='hip', arm_r='wave', swing_r=20, rate=0.8, bob=3, detail=0)


def pie_contest(sc):
    """The pie-eating contest: four eaters nodding into their pies, the judge's flag, the tally."""
    r = random.Random(331)
    g = sc.still()
    table(g, -60, -90, 620, 90, 88, WHITE, RED)
    bench(g, -60, 0, 600, WOOD)
    tops = (RED, CYAN, AMBER, LIME)
    for k, px in enumerate((-290, -140, 10, 160)):
        d = K.guest(r, top=tops[k], build=('stocky', 'avg', 'slim', 'stocky')[k]) if k != 2 else K.kid(r, top=tops[k])
        F(sc, px, 0, d, legs='sit', seat=50, arm_l='hold', arm_r='hold', rate=1.1 + 0.25 * k, phase=r.random(),
          sway=dict(swing=17.0, pivot=(px, 0.0, 56.0), axis=0.0), detail=2)
        g.cyl(SAND, (px, -92, 91), 30, 9, steps=8, top_r=34)
        g.cyl(CREAM, (px, -92, 100), 28, 9, steps=8, top_r=12)
        for j in range(k + 1):                                           # the empty tins beside each
            g.cyl(SILVER, (px + 56, -100, 91 + j * 6), 24, 5, steps=6, top_r=28)
    # the judge and the flag
    F(sc, 340, -70, K.staff(r), yaw=-50, arm_r='up', hold_r=P.hold_flag(col=RED), swing_r=30, arm_l='hip', rate=1.0, bob=3, detail=2)
    # last year's winner on a box, a rosette on a pole
    g.box(GOLD, (470, 70, 35), (90, 90, 70))
    F(sc, 470, 70, K.guest(r, build='stocky', top=VIOLET), z=70, yaw=-35, phase=0.3, **CHEER[3])
    # the tally: a board of pies, a column each
    g.box(NAVY, (-420, 205, 230), (260, 10, 230))
    for k in range(4):
        for j in range(k + 1 + (k % 2)):
            g.box(tops[k], (-510 + k * 60, 198, 140 + j * 44), (40, 5, 32))
    g.box(GOLD, (-420, 198, 338), (250, 5, 8))
    # the crowd behind the eaters
    for k, (x, y, yaw) in enumerate(((-360, 130, 15), (-220, 150, 5), (-80, 135, 0), (70, 155, -5), (220, 135, -15))):
        d = K.guest(r) if k % 2 else K.guest(r, wear='dress')
        F(sc, x, y, d, yaw=yaw, phase=r.random(), react=True, detail=0, **CHEER[k % 4])
    F(sc, -470, -110, K.guest(r), yaw=70, **film(r))
    K.bunting(g, (-520, 205, 380), (520, 205, 380), n=8, cols=(RED, WHITE, AMBER, WHITE))


def ride_drill(sc):
    """The evacuation drill: a coaster car stopped on its track, riders waving, staff on a ladder and with a sheet."""
    r = random.Random(337)
    g = sc.still()
    TH = 210.0
    for sy in (-1, 1):
        tilt_box(g, RED, (-420, 90 + sy * 34, TH - 10), (280, 90 + sy * 34, TH + 30), 10, 10)
    for px in (-360, -120, 120, 250):
        zt = TH - 10 + (px + 420) * 40.0 / 700.0
        g.box(WHITE, (px, 90, zt / 2.0), (16, 16, zt))
        g.box(WHITE, (px, 90, zt - 8), (14, 96, 10))
        tilt_box(g, GREY_L, (px, 90, 20), (px + 110, 90, zt - 30), 8, 8)
    car = sc.part(P.motion(swing=1.5, pivot=(-60.0, 90.0, TH), axis=0.0, rate=0.5))
    car.box(MAG, (-60, 90, TH + 52), (330, 104, 56))
    car.box(CYAN, (120, 90, TH + 44), (40, 96, 40))
    car.box(GOLD, (-60, 36, TH + 62), (300, 5, 10))
    for px in (-170, -60, 50):
        car.box(NAVY, (px + 34, 90, TH + 98), (10, 90, 40))
    ok = dict(sway=dict(swing=1.5, pivot=(-60.0, 90.0, TH), axis=0.0), rate=0.5)
    F(sc, -170, 90, K.guest(r), z=TH + 34, legs='sit', seat=50, arm_l='up', arm_r='up', **ok)
    F(sc, -60, 90, K.kid(r), z=TH + 34, legs='sit', seat=50, arm_l='vee', arm_r='vee', phase=0.3, **ok)
    F(sc, 50, 90, K.guest(r, wear='dress'), z=TH + 34, legs='sit', seat=50, arm_r='wave', swing_r=26, bob=2, rate=1.2, phase=0.6)
    # the ladder and the one on it
    for sx in (-1, 1):
        tilt_box(g, SILVER, (-290 + sx * 22, -90, 0), (-250 + sx * 22, 30, TH + 60), 7, 7)
    for k in range(6):
        f = (k + 0.7) / 6.5
        g.box(SILVER, (-290 + 40 * f, -90 + 120 * f, (TH + 60) * f), (48, 6, 6))
    vest = dict(band=ORANGE, hat='hard', hat_col=ORANGE)
    F(sc, -272, -40, K.staff(r, **vest), z=118, yaw=160, arm_l='up', arm_r='up', swing_r=8, axis_r='x', rate=0.6, detail=2)
    F(sc, -400, -150, K.staff(r, **vest), yaw=40, arm_l='push', arm_r='push', rock=3, rate=0.6)                    # footing the ladder
    F(sc, 330, -120, K.staff(r, **vest), yaw=-40, arm_r='face', hold_r=C.hold_megaphone(col=ORANGE, trim=WHITE), arm_l='point', swing_l=12, axis_l='x',
      rate=0.7, bob=2, detail=2)
    F(sc, 460, 10, K.staff(r, **vest), yaw=-60, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rate=0.3, bob=-1.5)
    # two with a sheet held out under the car's end
    F(sc, 30, -150, K.staff(r, **vest), yaw=0, arm_l='carry', arm_r='carry', hold_r=hold_sheet(), rate=0.8, bob=3, phase=0.0)
    F(sc, 180, -150, K.staff(r, **vest), yaw=0, arm_l='carry', arm_r='carry', hold_r=hold_sheet(), rate=0.8, bob=3, phase=0.0)
    for px, py in ((-480, -200), (-120, -210), (260, -215), (500, -190)):
        g.cyl(ORANGE, (px, py, 0), 20, 50, steps=6, top_r=5)
        g.box(WHITE, (px, py, 24), (24, 24, 7))
    # one already down, wrapped in a blanket, with a drink; a child clapping
    bench(g, 430, 170, 220, WOOD)
    F(sc, 400, 170, K.guest(r, top=SILVER, sleeves='long'), yaw=-20, legs='sit', seat=50, arm_r='face', hold_r=P.hold_cup(), arm_l='hold', rate=0.3, bob=-1.0)
    F(sc, 500, 150, K.kid(r), yaw=-30, phase=r.random(), **CHEER[1])


def backstage(sc):
    """Behind the scenes: the hippo with its head off, a fan, the giraffe stretching, the costume rail."""
    r = random.Random(347)
    g = sc.still()
    bench(g, -60, 40, 300, WOOD)
    # the hippo's player: fur to the neck, a person's head, a cup
    F(sc, -120, 40, dict(K.guest(r, hair='pony'), top=LAV, bottom=LAV, shoes=LAV, sleeves='long', wear='trousers', band=CYAN, build='stocky'),
      yaw=10, legs='sit', seat=50, arm_r='face', hold_r=P.hold_cup(col=CYAN), arm_l='hip', rate=0.3, bob=-1.5, detail=2)
    hippo_head(g, 20, 40, 54)
    F(sc, -260, 20, K.staff(r), yaw=65, arm_r='fwd', hold_r=hold_fan(), swing_r=24, axis_r='z', arm_l='hip', rate=1.6, detail=2)
    K.mascot(sc, 'giraffe', 230, -60, yaw=-20, s=1.1, phase=0.3, rate=0.35, bob=3, arm_l='vee', arm_r='vee', swing_r=-14, swing_l=14)
    F(sc, 60, -150, K.guest(r, wear='skirt', top=MAG, bottom=CREAM, sleeves='none', hair='bun', build='slim'), yaw=-10, legs='wide', arm_l='up', arm_r='out',
      rock=14, rate=0.4, phase=0.2, detail=2)
    # the costume rail, its costumes swaying
    for sx in (-1, 1):
        g.box(SILVER, (-390 + sx * 130, 170, 115), (7, 7, 230))
        g.box(DARK, (-390 + sx * 130, 170, 4), (10, 70, 8))
    g.box(SILVER, (-390, 170, 230), (270, 7, 7))
    for k, c in enumerate((MAG, K.FROG, AMBER, CYAN, RED)):
        px = -490 + k * 50
        cm = sc.part(P.motion(swing=6.0, pivot=(px, 170.0, 230.0), axis=90.0, rate=0.5, phase=0.17 * k))
        cm.box(c, (px, 170, 150), (38, 24, 130))
        cm.box(GOLD if k % 2 else WHITE, (px, 157, 190), (30, 3, 14))
    # the mirror in its lamps, and the one making up at it
    g.box(PLUM, (420, 215, 190), (200, 12, 250))
    g.box(SILVER, (420, 207, 200), (150, 5, 180))
    K.bulbs(g, (330, 204, 300), (510, 204, 300), 5, size=14.0)
    for sx in (-1, 1):
        K.bulbs(g, (420 + sx * 88, 204, 115), (420 + sx * 88, 204, 285), 4, size=14.0)
    g.box(WOOD, (420, 170, 78), (200, 60, 8))
    F(sc, 420, 110, K.guest(r, top=K.FROG, bottom=K.FROG, sleeves='long', wear='trousers', hair='afro', shoes=K.FROG), yaw=180, arm_r='face', arm_l='hip',
      swing_r=8, axis_r='x', rate=0.8)
    # a runner with a costume head on a tray, to and fro along the back
    F(sc, 0, 120, K.staff(r), yaw=90, stride=24, rate=1.0, bob=3, arm_l='carry', arm_r='carry', hold_r=hold_present(col=K.FROG, ribbon=WHITE, w=60.0),
      travel=(640.0, 80.0), detail=0)
    # a pair of stilts against the cloth, a trunk, a stage manager with a clipboard
    for sx in (-1, 1):
        tilt_box(g, WOOD, (170 + sx * 20, 150, 0), (190 + sx * 20, 225, 300), 8, 8)
    C.case(g, -480, -150, col=PLUM)
    C.case(g, -470, -150, z=38, w=52, h=30, col=AMBER)
    F(sc, 470, -150, K.staff(r, hat='headset', hat_col=DARK), yaw=-50, arm_l='hold', arm_r='point', hold_l=C.hold_clipboard(), swing_r=12, axis_r='x', rate=0.7)
    F(sc, -400, -100, K.guest(r, wear='shorts', top=AMBER, sleeves='none'), yaw=50, legs='stride', arm_l='up', arm_r='up', bob=12, rate=1.4, phase=0.5)   # skipping to warm up


def treasure(sc):
    """The children's treasure hunt: a sand pit, an X, the chest's lid lifting, the captain and a parrot."""
    r = random.Random(349)
    g = sc.still()
    g.box(WOOD_D, (-40, -30, 10), (720, 360, 20))
    g.box(SAND, (-40, -30, 14), (690, 330, 20))
    for sx, sy in ((-1, 1), (1, -1)):
        g.sphere(SAND, (-40 + sx * 190, -30 + sy * 70, 22), 60, scale=(1.0, 1.0, 0.4), steps=(3, 6))
    tilt_box(g, RED, (-150, -150, 26), (-50, -70, 26), 16, 5)
    tilt_box(g, RED, (-150, -70, 26), (-50, -150, 26), 16, 5)
    # the chest, half dug out, its lid lifting
    g.box(WOOD, (120, -40, 44), (130, 84, 60))
    for sx in (-1, 1):
        g.box(DARK, (120 + sx * 48, -40, 44), (10, 88, 64))
    g.box(GOLD, (120, -40, 72), (112, 68, 8), glow=0.25)
    lid = sc.part(P.motion(swing=24.0, pivot=(120.0, 2.0, 76.0), axis=0.0, rate=0.35, phase=0.75))
    tilt_box(lid, WOOD, (120, 2, 78), (120, -66, 124), 130, 12)
    tilt_box(lid, DARK, (120, 0, 80), (120, -68, 128), 22, 14)
    # the diggers
    bandana = (RED, CYAN, AMBER, MAG)
    for k, (x, y, yaw) in enumerate(((-230, -60, 60), (-100, 40, 10), (10, -150, -30), (260, -110, -60))):
        F(sc, x, y, K.kid(r, hat='cap', hat_col=bandana[k]), yaw=yaw, legs='stride', arm_l='carry', arm_r='carry', hold_r=hold_spade((CYAN, MAG, LIME, AMBER)[k]),
          swing_r=26, swing_l=26, axis_r='x', axis_l='x', rate=1.0 + 0.15 * k, phase=r.random(), bob=3, detail=2 if k < 2 else 1)
    F(sc, 250, 60, K.kid(r, hat='cap', hat_col=LIME), yaw=-40, phase=0.2, react=True, **CHEER[0])                 # it is the chest!
    F(sc, -330, 110, K.guest(r, wear='dress'), yaw=40, arm_l='hold', arm_r='hold', hold_r=hold_map(), rate=0.3, bob=-1.5)
    # the captain: a red coat, a three-cornered hat with gold braid, a spyglass
    mv = dict(phase=0.0, rate=0.6, bob=2)
    d = K.staff(r, top=RED, bottom=NAVY, wear='coat', sleeves='long', hat=None, band=GOLD, build='stocky')
    F(sc, 420, 100, d, yaw=-45, arm_r='face', hold_r=hold_spyglass(), arm_l='point', swing_l=12, axis_l='x', detail=2, **mv)
    hm = on_body(sc, 420, 100, 0, -45, d['s'], **mv)
    hm.box(NAVY, (0, 0, 168), (78, 62, 9))
    hm.box(NAVY, (0, 0, 180), (44, 40, 20))
    hm.box(GOLD, (0, -31, 171), (70, 4, 6))
    hm.box(RED, (22, 4, 198), (5, 30, 26))
    # the parrot on its perch, the palm, barrels
    g.box(WOOD, (-470, 150, 110), (8, 8, 220))
    g.box(WOOD, (-470, 150, 222), (90, 8, 8))
    P.bird(sc, -470, 150, 244, heading=-90, span=84, body=RED, wing=CYAN, beak=AMBER, flap=10, rate=1.4, bob=3)
    g.cyl(WOOD, (480, -150, 0), 20, 330, steps=6, top_r=13)
    for k in range(6):
        a = k * math.pi / 3.0
        pm = sc.part(P.motion(swing=4.0, pivot=(480.0, -150.0, 330.0), axis=math.degrees(a) + 90.0, rate=0.3, phase=0.15 * k))
        tilt_box(pm, LEAF, (480, -150, 330), (480 + math.cos(a) * 150, -150 + math.sin(a) * 150, 290), 40, 6)
    for px, py in ((-480, -170), (-420, -200)):
        g.cyl(WOOD, (px, py, 0), 28, 70, steps=8)
        g.cyl(DARK, (px, py, 30), 29.5, 7, steps=8)
    F(sc, -450, -40, K.guest(r), yaw=70, **film(r))
    F(sc, 70, 150, K.guest(r, hat='sun', hat_col=CREAM), yaw=0, phase=r.random(), detail=0, **CHEER[1])


def cadets(sc):
    """The space cadets' passing-out: a toy rocket, a row of children in helmets saluting, medals, planets going round."""
    r = random.Random(353)
    g = sc.still()
    RX, RY = 400.0, 90.0
    g.cyl(GREY_L, (RX, RY, 0), 110, 24, steps=10)
    g.cyl(WHITE, (RX, RY, 60), 52, 250, steps=10)
    g.cyl(RED, (RX, RY, 310), 52, 110, steps=10, top_r=4)
    g.cyl(RED, (RX, RY, 150), 54, 22, steps=10)
    g.cyl(CYAN, (RX, RY - 46, 236), 20, 10, rot=(0, 0, 90), steps=8, glow=0.5)
    for k in range(3):
        a = math.radians(90 + k * 120)
        tilt_box(g, CYAN, (RX + math.cos(a) * 50, RY + math.sin(a) * 50, 150), (RX + math.cos(a) * 110, RY + math.sin(a) * 110, 24), 10, 60)
    puff = sc.part(P.motion(bob=-8.0, rate=0.4))
    for k in range(3):
        a = k * math.pi * 2.0 / 3.0
        puff.sphere(WHITE, (RX + math.cos(a) * 70, RY + math.sin(a) * 60, 44), 30, steps=(3, 5))
    # the planets going round on their arms
    g.box(PLUM, (-380, 150, 200), (12, 12, 400))
    for k, (c, rad, sz) in enumerate(((AMBER, 110, 30), (CYAN, 150, 22), (MAG, 80, 18))):
        a = k * 2.1
        pm = sc.part(P.motion(base=(-380.0, 150.0), travel=(-1.0, 0.12 + 0.05 * k)))
        px, py, pz = -380 + math.cos(a) * rad, 150 + math.sin(a) * rad, 400 - k * 50
        pm.sphere(c, (px, py, pz), sz, steps=(4, 6))
        tilt_box(pm, SILVER, (-380, 150, pz), (px, py, pz), 5, 5)
        if k == 0:
            pm.cyl(CREAM, (px, py, pz - 3), sz * 1.6, 5, steps=10)
    # the cadets, in a row, in helmets and silver
    for k, px in enumerate((-250, -140, -30, 80, 190)):
        d = K.kid(r, top=SILVER, bottom=SILVER, sleeves='long', wear='trousers', hat='helmet', hat_col=(WHITE, CYAN, WHITE, AMBER, WHITE)[k], band=(RED, MAG, CYAN, RED, LIME)[k])
        if k == 3:
            F(sc, px, -60, d, yaw=5, phase=0.3, detail=2, **CHEER[3])                    # cannot keep still
        else:
            F(sc, px, -60, d, yaw=5, arm_r='face', arm_l='down', rate=0.4, bob=-1.5, phase=0.2 * k, detail=2 if k < 2 else 1)
    # the commander with the medals
    F(sc, -330, -90, K.staff(r, top=SILVER, bottom=SILVER, sleeves='long', hat='peak', hat_col=NAVY, band=GOLD), yaw=70, arm_r='fwd', swing_r=12, axis_r='x',
      arm_l='carry', hold_l=hold_tray(cols=(GOLD, GOLD, GOLD)), rate=0.6, detail=2)
    # the frog in a helmet
    mv = dict(phase=0.5, rate=1.0, bob=7)
    K.mascot(sc, 'frog', 200, 130, yaw=-15, s=1.2, arm_l='hip', arm_r='wave', swing_r=24, **mv)
    dome = sc.part(P.motion(base=(200.0, 130.0), **mv), (200, 130, 0), -15, 1.2)
    dome.cyl(SILVER, (0, 0, 108), 60, 10, steps=8)
    dome.box(WHITE, (0, 36, 150), (44, 20, 70))
    # the families
    F(sc, -480, 60, K.guest(r), yaw=55, **film(r))
    F(sc, -180, 150, K.guest(r, wear='dress'), yaw=10, phase=r.random(), detail=0, **CHEER[1])
    F(sc, -40, 170, K.guest(r, build='stocky'), yaw=0, phase=r.random(), detail=0, **CHEER[0])
    F(sc, 520, -150, K.guest(r), yaw=-50, phase=r.random(), **CHEER[2])
    g.box(NAVY, (-30, -60, 2), (600, 110, 4))
    for k in range(5):
        g.box(AMBER, (-250 + k * 110, -110, 5), (30, 8, 3))


# ------------------------------------------------------------------------------------------ the kit
UP = dict(grow=(60.0, 60.0, 80.0))
JOBS = [
    ('SM_P3B_Wedding', staged(wedding, (MAG, CREAM), PURPLE), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3B_Birthday', staged(birthday, (CYAN, CREAM), PLUM), UP),
    ('SM_P3B_BigPrize', staged(big_prize, (RED, CREAM), NAVY), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3B_Dance', staged(dance, (AMBER, MAG), PURPLE), UP),
    ('SM_P3B_Magic', staged(magic, (VIOLET, CREAM), NAVY), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3B_PieContest', staged(pie_contest, (RED, WHITE), PLUM), UP),
    ('SM_P3B_RideDrill', staged(ride_drill, (ORANGE, CREAM), NAVY), UP),
    ('SM_P3B_Backstage', staged(backstage, (PLUM, GREY_L), PURPLE), UP),
    ('SM_P3B_Treasure', staged(treasure, (WOOD, SAND), TEAL), UP),
    ('SM_P3B_Cadets', staged(cadets, (CYAN, SILVER), NAVY), UP),
    ('SM_P3B_Wedding_Sky', towered(wedding, (MAG, CREAM), PURPLE), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3B_Dance_Sky', towered(dance, (AMBER, MAG), PURPLE), UP),
    ('SM_P3B_Magic_Sky', towered(magic, (VIOLET, CREAM), NAVY), dict(grow=(60.0, 60.0, 120.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
