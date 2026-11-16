"""FULL THROTTLE (Unreal) the population, level 7: City Center, pass 3, builder p3d: HEIGHTS.
/Game/People/CityCenter/SM_P3D_*.

People up high, so that they clear the barrier: every piece here brings its own structure and stands ON the street
wall (a podium's roof, the brick building behind an alley) or over a plaza. Placed by C:/ftwork/p3/L7d.inl.

WHERE THEY STAND (FTDressL7.inl, ft_dress_metro.py; the main cast and the watchers have the kerb, the low shop roofs'
middles, the garages' decks, the middles of the dear doors' terraces, the glass infills, the side streets' mouths):
  a ROOF piece is drawn for a podium's roof edge: z = 0 is the roof (the set's "up"), y = 0 the podium's middle, the
  roof's edge y = -350. Everything stays between y = -420 and -120: a tower stands behind every podium, and nothing
  tall may come nearer than 24 m (the podium's middle is 29 m out; the mesh's bounds are grown 60 cm for movement).
  Free roofs used: the TALL shops' roofs (8.88 m: SM_Shops1's middle shop, SM_Shops2's two end shops, SM_Shops3's
  arcade), the picture houses without a party (11.82 m), the banks (10.38 m), the service yards' free halves
  (6.64 m), and the two ENDS of every dear door's terrace (9.6 m; 12.5 m from its middle).
    SM_P3D_Lookout_A/B/C   4.5 m: a scaffold platform 2.7 m over the roof (it clears the terrace's parasols):
                           A binoculars and a big flag, B a telescope and a bunch of balloons, C two drummers.
    SM_P3D_CrowsNest_A     4.2 m: a mast with a basket 4.7 m up, one in it waving a great flag, three at its foot.
    SM_P3D_Bleacher_A/B    8 m / 6.4 m: a little grandstand of three steps full of people; B under a striped canopy.
    SM_P3D_Billboard_A     8 m: a billboard on the roof being painted: three painters on its catwalk, two below.
    SM_P3D_Band_A          7.8 m: a band on a stage under a truss of lights: drums, two horns, a singer, a bass.
    SM_P3D_HighWire_A      7.8 m: a wire walker crossing between two masts with a long pole, a net under the wire.
    SM_P3D_Ticker_A        12 m: the bank's clerks at the parapet throwing paper; the paper floats down the front.
  SM_P3D_Abseil_A   stands at the brick building behind an alley (SM_Infill1's own origin: its face is y = -130, its
                    roof 34.4 m): two climbers on ropes letting a great banner down the wall, two more on the roof
                    paying the ropes out. It brings two steel masts and a head beam, so it stands on its own.
  SM_P3D_SkyWalk_A  a footbridge on its own legs across a monument's plaza (the gap between two podiums, 13 m), its
                    deck 6.4 m up under an arch hung with lanterns: people with lanterns on sticks.
It is night: every piece is TALL, stands 24 m or more from the road and carries steady light (figures a faint one,
cloth a little more, bulbs and lanterns): nothing blinks. Stripes, bars and discs only: never words, never a chevron.

Run in the editor:  import ft_people_city_p3d as K; K.run()        (or K.run(['SM_P3D_Band_A']))
"""
import math
import random
import unreal
import ft_people as P
import ft_people_city_watch as W
from ft_desert import rod, tilt_box

DIR = '/Game/People/CityCenter'
NAVY, DARK, GREY, STEEL, WHITE, CREAM = W.NAVY, W.DARK, W.GREY, W.STEEL, W.WHITE, W.CREAM
PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, BLUE = W.PINK, W.CYAN, W.YEL, W.MINT, W.RED, W.ORANGE, W.VIOLET, W.BLUE
WARM, WOOD, CONC = W.WARM, W.WOOD, W.CONC
G_BULB, G_CLOTH = W.G_BULB, W.G_CLOTH
G_LAMP = 1.2
SHIRTS = (WHITE, '#d7e3f0', '#57c7ff', PINK, CREAM)


def fig(sc, r, x, y, z=0.0, yaw=0.0, detail=1, **kw):
    """One citizen in a coat that reads at night (W.person), with the pose and movement given."""
    d = W.person(r)
    d.update(kw)
    d.setdefault('phase', r.random())
    return P.figure(sc, x, y, z=z, yaw=yaw, detail=detail, **d)


def ladder(g, x, y0, y1, z1, col=STEEL, w=44.0, rungs=6):
    """A ladder from (x, y0, 0) up to (x, y1, z1)."""
    for sx in (-1, 1):
        tilt_box(g, col, (x + sx * w / 2.0, y0, 0), (x + sx * w / 2.0, y1, z1), 6, 6)
    for k in range(rungs):
        t = (k + 0.5) / rungs
        g.box(col, (x, y0 + (y1 - y0) * t, z1 * t), (w, 5, 5))


def hold_binoculars():
    def f(m, h, side):
        if side > 0:
            m.box(DARK, (h[0] - 16, h[1] - 9, h[2] + 4), (30, 16, 11))
    return f


def hold_stick(col=WOOD):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 14, h[2] + 4), (5, 34, 5))
    return f


def hold_roller(col=PINK):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 32), (5, 5, 70))
        m.box(col, (h[0], h[1] - 7, h[2] + 68), (28, 11, 11))
    return f


def hold_horn(col=YEL):
    def f(m, h, side):
        if side > 0:
            m.cyl(col, (h[0] - 14, h[1] - 6, h[2] + 6), 5, 38, rot=(0, 0, 90), steps=6, top_r=15)
    return f


def hold_lantern(col=WARM, stick=90.0):
    """A paper lantern on a stick (steady light: only on a piece 24 m or more from the road)."""
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + stick / 2.0), (5, 5, stick))
        m.box(col, (h[0], h[1], h[2] + stick + 16), (26, 26, 34), glow=G_LAMP)
        m.box(DARK, (h[0], h[1], h[2] + stick + 35), (14, 14, 5))
    return f


def hold_pole(col=WHITE, L=330.0):
    """The wire walker's balancing pole, across the body."""
    def f(m, h, side):
        if side > 0:
            m.box(col, (0, h[1] - 6, h[2] + 2), (L, 6, 6))
            for sx in (-1, 1):
                m.box(RED, (sx * (L / 2.0 - 14), h[1] - 6, h[2] + 2), (28, 8, 8))
    return f


# ------------------------------------------------------------------------------------------ the lookout
def lookout(seed=1, kind=0, trim=YEL):
    """TALL, a roof piece 4.5 m long: a scaffold platform 2.7 m over the roof (over the terrace's parasols), a rail, a
    cloth hung from its front, a string of lights. kind 0: one with binoculars turning, one with a big flag, a child
    hopping, one pointing, three behind. 1: a telescope on its tripod and its astronomer, a bunch of balloons.
    2: two drummers at two big drums, a flag."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z = 270.0
        F = Z + 16.0
        for sx in (-1, 1):
            for y in (-336.0, -128.0):
                g.box(GREY, (sx * 205, y, Z / 2.0), (14, 14, Z))
            tilt_box(g, GREY, (sx * 205, -336, 20), (sx * 205, -128, Z - 20), 8, 8)
            g.box(STEEL, (sx * 218, -343, F + 170), (7, 7, 340))
        tilt_box(g, GREY, (-205, -128, 20), (205, -128, Z - 20), 8, 8)
        g.box(CONC, (0, -232, Z + 8), (450, 236, 16))
        g.box(trim, (0, -351, Z + 2), (450, 6, 34), glow=G_CLOTH)
        W.rail(g, -218, 218, -343, F, h=96.0, n=3)
        ladder(g, -240, -60, -118, F)
        g.box(DARK, (0, -343, F + 338), (436, 4, 4))
        W.bulbs(g, (-218, -343, F + 326), (218, -343, F + 326), 5, sag=0)
        bars, base = W.SETS[(seed + kind) % len(W.SETS)]
        W.drop(sc, r, 0.0, -356, Z - 16, 380.0, 220.0, base, bars, (0, 2, 3)[kind % 3])
        if kind == 0:
            fig(sc, r, -150, -292, F, yaw=r.uniform(-8, 8), arm_l='face', arm_r='face', hold_r=hold_binoculars(), turn=20, rate=0.3)
            fig(sc, r, -48, -292, F, arm_r='wave', swing_r=22, rate=0.9, bob=4, react=True, hold_r=W.big_flag(col=PINK, bar=WHITE))
            fig(sc, r, 56, -296, F, build='kid', s=0.66, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.7, react=True)
            fig(sc, r, 150, -292, F, yaw=-10, arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.8, bob=2)
            for x in (-120.0, 10.0, 140.0):
                W.mini(sc, r, x + r.uniform(-8, 8), -190, z=F, yaw=r.uniform(-12, 12), legs=True)
        elif kind == 1:
            for a in (90, 210, 330):
                rod(g, STEEL, (150, -300, F + 120), (150 + math.cos(math.radians(a)) * 42, -300 + math.sin(math.radians(a)) * 42, F), 3.5, steps=5)
            tilt_box(g, WHITE, (150, -268, F + 108), (150, -350, F + 168), 17, 17)
            g.box(CYAN, (150, -352, F + 170), (20, 6, 20))
            fig(sc, r, 150, -232, F, arm_l='face', arm_r='hold', rock=3, rate=0.35)
            fig(sc, r, 30, -292, F, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.5, react=True)
            fig(sc, r, -70, -292, F, arm_r='wave', swing_r=26, rate=1.2, bob=4, react=True)
            fig(sc, r, -160, -292, F, build='kid', s=0.66, arm_r='up', swing_r=18, bob=8, rate=1.8, react=True)
            for x in (-100.0, 40.0):
                W.mini(sc, r, x + r.uniform(-8, 8), -190, z=F, yaw=r.uniform(-12, 12), legs=True)
            # the balloons, tied to the rail's end post: they float and lean together
            b = sc.part(P.motion(bob=-6.0, swing=6.0, pivot=(-218.0, -343.0, F + 96.0), axis=0.0, rate=0.35, phase=r.random()))
            for k, c in enumerate((PINK, CYAN, YEL, WHITE, VIOLET)):
                top = (-218 + (k - 2) * 34, -343 - (k % 2) * 16, F + 250 + (k % 3) * 34)
                tilt_box(b, DARK, (-218, -343, F + 96), top, 4, 4)
                b.sphere(c, (top[0], top[1], top[2] + 24), 27, steps=(4, 6), glow=G_CLOTH)
        else:
            for x, c in ((-120.0, RED), (40.0, CYAN)):
                for sx in (-1, 1):
                    tilt_box(g, WOOD, (x + sx * 30, -322, F), (x - sx * 10, -322, F + 78), 6, 6)
                g.cyl(c, (x, -322, F + 76), 40, 34, steps=10, glow=G_CLOTH)
                g.cyl(CREAM, (x, -322, F + 110), 38, 3, steps=10, glow=G_CLOTH)
                fig(sc, r, x, -262, F, sleeves='none', arm_l='carry', arm_r='carry', swing_l=26, swing_r=-26, axis_l='x', axis_r='x',
                    rate=2.0, bob=3, hold_l=hold_stick(), hold_r=hold_stick(), hat='beanie', hat_col=c)
            fig(sc, r, 160, -292, F, arm_r='wave', swing_r=22, rate=0.9, bob=4, react=True, hold_r=W.big_flag(col=YEL, bar=RED))
            for x in (-180.0, -40.0, 110.0):
                W.mini(sc, r, x + r.uniform(-8, 8), -186, z=F, yaw=r.uniform(-12, 12), legs=True)
        W.lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the crow's nest
def crows_nest(seed=5):
    """TALL, a roof piece 4.2 m long: a mast 6.3 m tall, a basket 4.7 m up with one in it waving a great flag, lines of
    pennants down to the roof, a ladder up the mast; three at its foot (one hauls a line, one points up, one cheers)."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        MY = -300.0
        g.box(RED, (0, MY, 315), (18, 18, 630))
        for z in (110.0, 250.0, 390.0):
            g.box(WHITE, (0, MY, z), (20, 20, 44))
        for k in range(9):
            g.box(STEEL, (0, MY - 12, 40 + k * 48), (44, 5, 5))
        g.box(WOOD, (0, MY, 466), (130, 120, 8))
        g.box(NAVY, (0, MY, 516), (124, 114, 92), glow=G_CLOTH)
        g.box(YEL, (0, MY - 58, 516), (124, 3, 24), glow=G_CLOTH)
        for k, (ex, ey) in enumerate(((-208.0, -340.0), (208.0, -340.0), (-208.0, -300.0), (208.0, -300.0))):
            tilt_box(g, DARK, (0, MY, 612), (ex, ey, 0), 5, 5)
            if k < 2:
                p = sc.part(P.motion(swing=3.0, pivot=(0.0, MY, 612.0), axis=90.0, rate=0.45, phase=r.random()))
                for j in range(4):
                    t = 0.2 + j * 0.19
                    p.box((PINK, CYAN, YEL, WHITE)[(j + k) % 4], (ex * t, MY + (ey - MY) * t - 3, 612 * (1 - t) - 20), (30, 3, 34), glow=G_CLOTH)
        W.mini(sc, r, 0, MY + 6, z=498, s=1.05, act=('down', 'wave', 0, 22, 3, 0.8), top=PINK,
               hold=W.big_flag(col=CYAN, bar=WHITE, L=300.0, w=160.0, h=96.0))
        fig(sc, r, -130, -318, 0, yaw=20, arm_l='up', arm_r='up', swing_l=16, swing_r=16, axis_l='x', axis_r='x', rate=0.8, bob=2)
        fig(sc, r, 118, -322, 0, yaw=-25, arm_r='up', arm_l='hip', swing_r=10, rate=0.7, bob=2)
        fig(sc, r, 176, -250, 0, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=8, rate=1.5, react=True)
        W.lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the bleacher
def bleacher(seed=3, L=800.0, rows=(8, 7, 8), canopy=False):
    """TALL, a roof piece: a little grandstand of three steps along the roof's edge, every place taken, everyone
    hopping out of step. Plain: a banner on two poles behind and two big flags in the top row. canopy: a striped
    awning on four posts over it instead."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h = L / 2.0
        bars, base = W.SETS[seed % len(W.SETS)]
        for k in range(3):
            g.box((GREY, CONC)[k % 2], (0, -312 + 62 * k, (40 + 50 * k) / 2.0), (L, 62, 40 + 50 * k))
        n = 8
        for k in range(n):
            g.box((bars[0], base)[k % 2], (-h + (k + 0.5) * L / n, -345, 20), (L / n, 4, 40), glow=G_CLOTH)
        for sx in (-1, 1):
            g.box(STEEL, (sx * (h + 4), -250, 150), (6, 190, 6))
            g.box(STEEL, (sx * (h + 4), -343, 75), (6, 6, 150))
            g.box(STEEL, (sx * (h + 4), -157, 120), (6, 6, 240))
        if canopy:
            for sx in (-1, 1):
                for y in (-340.0, -150.0):
                    g.box(WHITE, (sx * (h + 4), y, 215), (8, 8, 430))
            m = 6
            for k in range(m):
                g.box((bars[1], WHITE)[k % 2], (-h + (k + 0.5) * (L + 8) / m - 4, -245, 434), ((L + 8) / m, 240, 8), glow=G_CLOTH)
                g.box((bars[1], WHITE)[k % 2], (-h + (k + 0.5) * (L + 8) / m - 4, -366, 418), ((L + 8) / m, 4, 34), glow=G_CLOTH)
        else:
            for sx in (-1, 1):
                g.box(WOOD, (sx * (h - 40), -150, 235), (9, 9, 470))
            ban = sc.part(P.motion(swing=4.0, pivot=(0.0, -150.0, 466.0), axis=0.0, rate=0.4, phase=r.random()))
            W.cloth(ban, -(h - 40), h - 40, -150, 336, 466, base=base, bars=bars, kind=seed % 4)
        sp = (L - 90.0) / (max(rows) - 1)
        for k, cnt in enumerate(rows):
            for i in range(cnt):
                x = -(cnt - 1) * sp / 2.0 + i * sp + r.uniform(-8, 8)
                hold, act = None, None
                if k == 2 and not canopy and i in (1, cnt - 2):
                    hold, act = W.big_flag(col=(PINK, CYAN)[i % 2], bar=WHITE), ('down', 'wave', 0, 22, 4, 0.9)
                elif k == 1 and i == cnt // 2:
                    hold, act = W.placard(col=(CREAM, YEL)[seed % 2], bar=(RED, VIOLET)[seed % 2]), ('down', 'wave', 0, 10, 8, 1.3)
                W.mini(sc, r, x, -312 + 62 * k, z=40 + 50 * k, yaw=r.uniform(-12, 12), legs=(k == 0), hold=hold, act=act)
        W.lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the billboard
def billboard(seed=7):
    """TALL, a roof piece 8 m long: a billboard on two posts (stripes and a disc: never words), half painted; three
    painters on the catwalk before it rolling the paint on, a foreman on the roof waving at the chase, a mate pointing
    up; lamps over the board, a ladder, cans."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 1):
            g.box(GREY, (sx * 340, -190, 320), (16, 16, 640))
            tilt_box(g, GREY, (sx * 340, -125, 0), (sx * 340, -190, 420), 10, 10)
            tilt_box(g, GREY, (sx * 340, -200, 150), (sx * 340, -300, 176), 8, 8)
        g.box(CREAM, (0, -204, 400), (780, 10, 420), glow=0.14)
        g.box(NAVY, (0, -203, 400), (800, 6, 440))
        g.box(PINK, (-160, -210, 400), (440, 3, 400), glow=0.2)
        g.box(CYAN, (-160, -212, 250), (440, 3, 70), glow=0.2)
        g.cyl(YEL, (-200, -211, 440), 110, 3, rot=(0, 0, 90), steps=10, glow=0.22)
        g.box(WHITE, (66, -212, 400), (12, 3, 400), glow=0.2)
        g.box(PINK, (150, -210, 520), (150, 3, 150), glow=0.2)
        g.box(WOOD, (0, -262, 176), (800, 84, 10))
        W.rail(g, -400, 400, -302, 181, h=90.0, n=4)
        for x in (-250.0, 0.0, 250.0):
            g.box(DARK, (x, -238, 626), (8, 76, 8))
            g.box(WARM, (x, -276, 618), (34, 22, 12), glow=G_BULB)
        ladder(g, -372, -340, -300, 176)
        for k, c in enumerate((PINK, CYAN, YEL, WHITE)):
            g.cyl(c, (250 + k * 34, -330 + (k % 2) * 30, 0), 14, 30, steps=6)
        over = dict(top=WHITE, bottom='#d7e3f0', sleeves='long', hat='cap', hat_col=WHITE)
        fig(sc, r, 130, -256, 181, yaw=180, arm_r='up', arm_l='hip', swing_r=20, axis_r='x', rate=0.8, hold_r=hold_roller(PINK), **over)
        fig(sc, r, 270, -256, 181, yaw=180, arm_r='up', arm_l='up', swing_r=18, swing_l=-18, axis_r='x', axis_l='x', rate=0.7,
            hold_r=hold_roller(PINK), **over)
        fig(sc, r, -240, -256, 181, yaw=180, arm_r='fwd', arm_l='down', swing_r=16, rate=1.0, hold_r=hold_roller(CYAN), **over)
        fig(sc, r, -130, -326, 0, arm_l='wave', arm_r='wave', swing_l=-22, swing_r=22, rate=1.1, bob=6, react=True, hat='cap', hat_col=RED)
        fig(sc, r, 60, -326, 0, yaw=150, arm_r='up', arm_l='hip', swing_r=10, rate=0.6, bob=2)
        W.lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the band
def band(seed=9):
    """TALL, a roof piece 7.8 m long: a band on a low stage under a truss hung with lights, speakers at both ends, a
    striped cloth behind: a drummer, a trumpet, a second horn, a singer, a double bass, two dancers."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z = 50.0
        bars, base = W.SETS[seed % len(W.SETS)]
        g.box(NAVY, (0, -245, Z / 2.0), (780, 200, Z))
        g.box(PINK, (0, -346, Z - 8), (780, 4, 12), glow=0.6)
        for sx in (-1, 1):
            g.box(STEEL, (sx * 384, -160, 235), (12, 12, 470))
            g.box(DARK, (sx * 336, -196, Z + 82), (84, 70, 164))
            g.cyl(GREY, (sx * 336, -233, Z + 118), 26, 4, rot=(0, 0, 90), steps=8)
            g.cyl(GREY, (sx * 336, -233, Z + 50), 18, 4, rot=(0, 0, 90), steps=8)
        g.box(STEEL, (0, -160, 464), (780, 12, 12))
        W.bulbs(g, (-384, -160, 446), (384, -160, 446), 8, sag=0)
        back = sc.part(P.motion(swing=3.0, pivot=(0.0, -152.0, 452.0), axis=0.0, rate=0.35, phase=r.random()))
        W.cloth(back, -280, 280, -152, 250, 452, base=base, bars=bars, kind=1)
        # the drums
        g.box(DARK, (0, -186, Z + 23), (36, 36, 46))
        g.cyl(WHITE, (0, -246, Z + 40), 38, 30, rot=(0, 0, 90), steps=10, glow=G_CLOTH)
        g.cyl(RED, (-56, -226, Z + 62), 21, 14, steps=8)
        g.box(STEEL, (58, -226, Z + 52), (5, 5, 104))
        g.cyl(YEL, (58, -226, Z + 104), 27, 3, steps=8)
        fig(sc, r, 0, -186, Z, detail=0, legs='sit', arm_l='carry', arm_r='carry', swing_l=24, swing_r=-24, axis_l='x', axis_r='x',
            rate=2.0, hold_l=hold_stick(), hold_r=hold_stick(), hat='cap', hat_col=DARK)
        fig(sc, r, -236, -292, Z, yaw=8, arm_l='face', arm_r='face', hold_r=hold_horn(YEL), rock=5, rate=1.2)
        fig(sc, r, -128, -300, Z, yaw=-6, arm_l='hold', arm_r='face', hold_r=hold_horn('#ffe28a'), rock=6, rate=1.2, phase=0.5)
        fig(sc, r, 10, -306, Z, arm_r='face', arm_l='up', swing_l=18, bob=5, rate=1.2, react=True, wear='dress', top=RED, hair='long')
        # the double bass and its player
        tilt_box(g, WOOD, (232, -300, Z), (232, -300, Z + 118), 46, 24)
        tilt_box(g, DARK, (232, -300, Z + 118), (232, -300, Z + 196), 8, 8)
        fig(sc, r, 262, -262, Z, detail=0, yaw=-10, arm_l='face', arm_r='carry', swing_r=14, rate=1.2, hat='peak', hat_col=DARK)
        fig(sc, r, 122, -300, Z, arm_l='vee', arm_r='vee', turn=28, bob=8, rate=1.2, react=True, wear='skirt')
        fig(sc, r, -330, -300, Z, detail=0, arm_l='wave', arm_r='wave', swing_l=-22, swing_r=22, bob=7, rate=1.2, react=True)
        W.lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the high wire
def high_wire(seed=11):
    """TALL, a roof piece 7.8 m long: a wire 4.3 m over the roof between two red-and-white masts, a walker crossing it
    to and fro with a long pole, a helper on each mast's little platform, a striped net under the wire, six looking up."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Zw, Y = 430.0, -250.0
        for sx in (-1, 1):
            g.box(RED, (sx * 380, Y, 265), (16, 16, 530))
            for z in (120.0, 260.0, 400.0):
                g.box(WHITE, (sx * 380, Y, z), (18, 18, 40))
            g.box(WOOD, (sx * 336, Y, Zw - 6), (104, 90, 8))
            tilt_box(g, STEEL, (sx * 380, Y, 500), (sx * 380, -345, 0), 5, 5)
            tilt_box(g, STEEL, (sx * 380, Y, 500), (sx * 380, -130, 0), 5, 5)
            p = sc.part(P.motion(swing=12.0, pivot=(sx * 380.0, Y, 520.0), axis=P.UP, rate=0.6, phase=r.random()))
            p.box((PINK, CYAN)[sx > 0], (sx * 380 + 34, Y, 512), (64, 3, 32), glow=G_CLOTH)
            for y in (-322.0, -178.0):
                g.box(STEEL, (sx * 296, y, 106), (8, 8, 212))
        g.box(DARK, (0, Y, Zw - 3), (760, 5, 5))
        g.box(WHITE, (0, Y, 213), (600, 150, 5), glow=G_CLOTH)
        for k in range(5):
            g.box(RED, (-240 + k * 120, Y, 214), (44, 152, 5), glow=G_CLOTH)
        fig(sc, r, 0, Y, Zw, yaw=90, detail=2, top=PINK, bottom=WHITE, sleeves='long', shoes=WHITE, build='slim', hair='bun', s=1.0,
            arm_l='out', arm_r='out', hold_r=hold_pole(), rate=0.11,
            sway=dict(swing=2.4, pivot=(0.0, Y, Zw - 6000.0), axis=90.0))
        fig(sc, r, -340, Y, Zw - 2, yaw=-20, detail=0, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=4, rate=1.3, top=CYAN, s=0.95)
        fig(sc, r, 340, Y, Zw - 2, yaw=20, detail=0, arm_r='wave', swing_r=24, rate=1.0, bob=3, top=YEL, s=0.95)
        for i, x in enumerate((-320.0, -200.0, -70.0, 60.0, 190.0, 320.0)):
            W.mini(sc, r, x + r.uniform(-10, 10), -334, z=0, yaw=r.uniform(-14, 14), legs=True,
                   act=(('up', 'up', 14, -14, 6, 1.4), ('down', 'up', 0, -20, 5, 1.2), ('wave', 'down', -24, 0, 4, 1.1))[i % 3])
        W.lit(sc)
    return build


# ------------------------------------------------------------------------------------------ ticker tape
def ticker(seed=13, L=1200.0):
    """TALL, a roof piece 12 m long (a bank's roof: the step lifts them over the pediment): the clerks at the parapet
    in shirtsleeves, throwing paper; thirty-two sheets float down the front in eight flights, out of step; streamers
    off the rail; a string of lights."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h = L / 2.0
        Z = 80.0
        g.box(GREY, (0, -290, Z / 2.0), (L - 20, 110, Z))
        W.rail(g, -h, h, -345, Z, h=100.0, n=6)
        for sx in (-1, 1):
            g.box(STEEL, (sx * h, -345, Z + 170), (7, 7, 340))
        g.box(DARK, (0, -345, Z + 338), (L, 4, 4))
        W.bulbs(g, (-h, -345, Z + 326), (h, -345, Z + 326), 10, sag=0)
        for i, x in enumerate((-450.0, -150.0, 150.0, 450.0)):
            fig(sc, r, x + r.uniform(-10, 10), -298, Z, yaw=r.uniform(-10, 10), top=SHIRTS[i % len(SHIRTS)], bottom=(NAVY, DARK)[i % 2], sleeves='long',
                arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=5, rate=1.1 + 0.1 * i, react=True, band=(RED, None, CYAN, None)[i])
        for i, x in enumerate((-540.0, -300.0, -75.0, 0.0, 75.0, 300.0, 540.0)):
            W.mini(sc, r, x + r.uniform(-8, 8), -292, z=Z, yaw=r.uniform(-12, 12), legs=True, top=SHIRTS[(i + 2) % len(SHIRTS)])
        for j in range(8):
            px = -h + (j + 0.5) * L / 8.0
            p = sc.part(P.motion(bob=-r.uniform(35.0, 65.0), rate=r.uniform(0.25, 0.45), phase=r.random(), swing=r.uniform(4.0, 7.0),
                                 pivot=(px, -385.0, 760.0), axis=90.0))
            for i in range(4):
                p.box((WHITE, CREAM, PINK, CYAN, YEL)[(i + j) % 5], (px + r.uniform(-L / 5.0, L / 5.0), r.uniform(-405, -362), r.uniform(70, 400)),
                      (24, 3, 17), rot=(r.uniform(-40, 40), r.uniform(0, 90), r.uniform(-40, 40)), glow=G_CLOTH)
        for k in range(6):
            x = -h + (k + 0.5) * L / 6.0
            s = sc.part(P.motion(swing=7.0, pivot=(x, -350.0, Z + 100.0), axis=0.0, rate=r.uniform(0.4, 0.7), phase=r.random()))
            s.box((PINK, WHITE, CYAN, YEL)[k % 4], (x, -352, Z + 100 - 80), (9, 4, 160), glow=G_CLOTH)
        W.lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the banner let down a wall
def abseil(seed=15):
    """TALL: stands at the brick building behind an alley (SM_Infill1's own origin: its face is y = -130, its roof
    34.4 m up). Two steel masts up the face and a head beam (the piece stands on its own), two ropes, two climbers one
    and a half times life size going up and down them, a banner 6 m wide and 10 m long let down between them (it
    stirs), two more on the roof paying out the ropes."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 3440.0
        S = 1.5
        for sx in (-1, 1):
            g.box(STEEL, (sx * 600, -200, H / 2.0 + 40), (20, 20, H + 80))
            for z in (600.0, 1500.0, 2400.0, 3300.0):
                g.box(STEEL, (sx * 600, -165, z), (12, 70, 12))
        g.box(YEL, (0, -200, H + 70), (1220, 22, 22))
        bars, base = W.SETS[seed % len(W.SETS)]
        ban = sc.part(P.motion(swing=1.6, pivot=(0.0, -252.0, 3300.0), axis=0.0, rate=0.3, phase=r.random()))
        W.cloth(ban, -300, 300, -252, 2300, 3300, base=base, bars=bars, kind=2, glow=0.2)
        ban.box(WOOD, (0, -252, 2296), (620, 9, 9))
        for sx, z0, ph, col in ((-1, 2560.0, 0.0, PINK), (1, 2040.0, 0.45, CYAN)):
            x = sx * 392.0
            g.box(WHITE, (x, -192, (H + 70 + z0 - 160) / 2.0), (6, 6, H + 70 - z0 + 160))
            fig(sc, r, x, -196, z0, yaw=180, s=S, detail=2, legs='sit', seat=40, top=col, bottom=NAVY, sleeves='long', hat='helmet', hat_col=YEL,
                arm_l='up', arm_r='out' if sx < 0 else 'up', bob=-170.0, rate=0.1, phase=ph, pack=RED)
        for x, sw in ((0.0, 14), (112.0, -14)):
            fig(sc, r, x, -96, H, s=1.2, arm_l='fwd', arm_r='fwd', swing_l=sw, swing_r=-sw, axis_l='x', axis_r='x', rate=0.6, bob=2,
                hat='hard', hat_col=YEL)
        W.lit(sc, 0.13)
    return build


# ------------------------------------------------------------------------------------------ the lantern bridge
def sky_walk(seed=17):
    """TALL: a footbridge on its own legs across a monument's plaza, 12.4 m long, its deck 6.4 m up, under an arch hung
    with seven paper lanterns (steady, they swing); six people at the rail with lanterns on sticks, cameras and a
    child, seven behind on a step, two banners hung under the deck."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z = 640.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(GREY, (sx * 585, sy * 95, (Z - 30) / 2.0), (26, 26, Z - 30))
            tilt_box(g, GREY, (sx * 585, -95, 60), (sx * 585, 95, Z - 60), 9, 9)
            g.box(VIOLET, (sx * 612, -127, Z + 115), (12, 12, 230))
        g.box(CONC, (0, 0, Z - 15), (1240, 250, 30))
        for sy in (-1, 1):
            g.box(VIOLET, (0, sy * 127, Z - 25), (1240, 8, 90))
        W.rail(g, -612, 612, -118, Z, n=6)
        g.box(GREY, (0, 50, Z + 17), (1160, 90, 34))
        tilt_box(g, GREY, (-560, 190, 0), (300, 190, Z - 12), 110, 14)
        g.box(GREY, (430, 190, Z - 15), (270, 110, 30))
        n = 8
        node = [(-612 + j * 1224.0 / n, Z + 230 + 150 * math.sin(math.pi * j / n)) for j in range(n + 1)]
        for j in range(n):
            tilt_box(g, VIOLET, (node[j][0], -127, node[j][1]), (node[j + 1][0], -127, node[j + 1][1]), 10, 10)
        for j in range(1, n):
            x, z = node[j]
            lan = sc.part(P.motion(swing=7.0, pivot=(x, -127.0, z), axis=0.0 if j % 2 else 90.0, rate=r.uniform(0.35, 0.55), phase=r.random()))
            lan.box(DARK, (x, -127, z - 14), (4, 4, 28))
            lan.box((WARM, PINK, WARM, CYAN)[j % 4], (x, -127, z - 46), (30, 30, 38), glow=G_LAMP)
        for k, x in enumerate((-330.0, 330.0)):
            bars, base = W.SETS[(seed + k) % len(W.SETS)]
            W.drop(sc, r, x, -131, Z - 72, 300.0, 250.0, base, bars, (2, 0)[k])
        acts = (
            dict(arm_r='wave', swing_r=14, rate=0.6, bob=3, hold_r=hold_lantern(WARM)),
            dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=3, rate=0.5),
            dict(arm_r='wave', swing_r=12, rate=0.5, bob=3, hold_r=hold_lantern(PINK)),
            dict(build='kid', s=0.66, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.7, react=True),
            dict(arm_l='wave', swing_l=-14, rate=0.7, bob=3, hold_l=hold_lantern(CYAN)),
            dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=8, rate=1.4, react=True),
        )
        for i, a in enumerate(acts):
            fig(sc, r, -500 + i * 200.0 + r.uniform(-8, 8), -78, Z, yaw=r.uniform(-12, 12), detail=0, **a)
        for i in range(7):
            W.mini(sc, r, -430 + i * 143 + r.uniform(-10, 10), 50, z=Z + 34, yaw=r.uniform(-12, 12), legs=True,
                   hold=hold_lantern((WARM, PINK, CYAN)[i % 3], stick=110.0) if i in (1, 3, 5) else None,
                   act=('down', 'wave', 0, 10, 5, 0.7) if i in (1, 3, 5) else None)
        W.lit(sc)
    return build


JOBS = [
    ('SM_P3D_Lookout_A', lookout(1, 0, YEL)),
    ('SM_P3D_Lookout_B', lookout(2, 1, CYAN)),
    ('SM_P3D_Lookout_C', lookout(4, 2, PINK)),
    ('SM_P3D_CrowsNest_A', crows_nest(5)),
    ('SM_P3D_Bleacher_A', bleacher(3)),
    ('SM_P3D_Bleacher_B', bleacher(6, L=640.0, rows=(6, 5, 6), canopy=True)),
    ('SM_P3D_Billboard_A', billboard(7)),
    ('SM_P3D_Band_A', band(9)),
    ('SM_P3D_HighWire_A', high_wire(11)),
    ('SM_P3D_Ticker_A', ticker(13)),
    ('SM_P3D_Abseil_A', abseil(15)),
    ('SM_P3D_SkyWalk_A', sky_walk(17)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
