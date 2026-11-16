"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea, pass 3, builder d: HEIGHTS
(/Game/People/UnderTheSea/SM_P3D_*).

People UP HIGH, each piece brought with the thing they stand on, so that they clear the barrier and the reef and are
seen against the plain blue water. Every deck here is 2.6 m or more over the sand, most are 6 to 10 m:

    SM_P3D_Gantry_A     a gantry bridge between two towers, 7.2 m up: a crowd at the rail, two walking the bridge,
                        one hanging in the water over it with a flag.
    SM_P3D_Gantry_B     the same bridge with a second deck over its middle and a crowd on that.
    SM_P3D_Mast         a lookout mast: a tub 9.2 m up with four in it, a half deck at 4.7 m, a climber on the ladder.
    SM_P3D_Buoy         a round deck hung under a great orange float, 6.4 m up; two more hang in the water with flags.
    SM_P3D_Spire        a rock spire with three ledges (3.8, 6.8, 9.8 m): standing, sitting on the edge, one on top.
    SM_P3D_WreckMast    a wreck's mast still standing: divers sitting along its two yards, one in the crow's nest.
    SM_P3D_Crane        a salvage crane: riggers on its platform and its cab roof, two riding the load up and down.
    SM_P3D_Terraces     a stepped tower block of Nereida: a crowd on each of its three roofs (4, 7.2, 10.2 m).
    SM_P3D_RopeBridge   a plank bridge between two rock stacks, 6.6 m up: three crossing it, others waving.
    SM_P3D_Lift         an open lift between two landings (2.6 and 8.6 m): the cage rides up and down with three in it.

The aquanaut is the watchers' kit's (ft_people_sea_watch.py: a suit of one strong colour, never blue, the white helmet
with a dark visor, a pack): this file calls that kit's helpers, so the kits meet without a seam.

All TALL: 24 m or more from the centre line (the block places them 46 m out and more). Steady light only (a few lamps
and windows, glow 0.5), nothing blinks. Nothing here is blue-grey with a fin.

A piece is drawn like every prop: cm; z = 0 is the sand; the middle of the footprint is the origin; the FRONT faces -Y
(the road); long pieces run along X (the road).

Run in the editor:  import ft_people_sea_p3d as K; K.run()        (or K.run(['SM_P3D_Mast']))
The placing rules: C:/ftwork/p3/L6d.inl (the integrator pastes it at the end of FTPeopleL6.inl).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_sea_watch as W
from ft_desert import rod, tilt_box

DIR = '/Game/People/UnderTheSea'

SUITS = W.SUITS
HELM, STEEL, DARK, WHITE, YEL = W.HELM, W.STEEL, W.DARK, W.WHITE, W.YEL
DECK, DECK_D, HULL_D, PORT, LAMP = W.DECK, W.DECK_D, W.HULL_D, W.PORT, W.LAMP
MAG, RED, ORANGE, LIME, PINK = W.MAG, W.RED, W.ORANGE, W.LIME, W.PINK
PANELS = W.PANELS
ROCK, ROCK_L, WOOD, RUST = '#6c6a8c', '#9d94b8', '#8f6b4a', '#a8553a'


def deck(g, x0, x1, y0, y1, z, seed=0, n=4, posts=3):
    """A deck with its top at z: the plate, a front parapet of colour blocks, a rail over it."""
    cx = (x0 + x1) / 2.0
    g.box(DECK, (cx, (y0 + y1) / 2.0, z - 10), (x1 - x0, y1 - y0, 20))
    W.fascia(g, x0, x1, y0 - 3, z + 34, 64, n, seed)
    g.box(DECK_D, (cx, y0, z + 112), (x1 - x0, 7, 7))
    for k in range(posts):
        g.box(DECK_D, (x0 + k * (x1 - x0) / float(max(posts - 1, 1)), y0, z + 56), (8, 8, 112))


def walker(sc, r, x, y, z, length, speed, s=1.4):
    """An aquanaut who walks the piece's X and comes round again (the helmet is a hat here, so the visor travels)."""
    suit = r.choice(SUITS)
    P.figure(sc, x, y, z=z, yaw=(90.0 if speed > 0 else -90.0), s=s, skin=HELM, hair='bald', hat='helmet', hat_col=HELM, top=suit, bottom=suit,
             shoes=DARK, sleeves='long', detail=0, pack=r.choice((STEEL, YEL, DARK)), stride=24, swing_l=20, swing_r=-20, axis_l='x', axis_r='x',
             rate=0.8, bob=4, phase=r.random(), travel=(length, speed))


def sitter(sc, r, x, y, z_seat, yaw, i=0, s=1.25):
    """One sitting on an edge at z_seat, the legs over the side."""
    d = dict(W.SIT[i % len(W.SIT)])
    W.watcher(sc, r, x, y, z_seat - 46.0 * s, yaw=yaw, s=s, legs='sit', phase=r.random(), react=True, **d)


# ------------------------------------------------------------------------------------------ the pieces
def gantry(seed=1, upper=False):
    """A gantry bridge, 18 m long, its deck 7.2 m over the sand."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z = 720.0
        for sx in (-1, 1):
            g.box(HULL_D, (sx * 830, 40, (Z - 40) / 2.0), (130, 130, Z + 40))
            g.box(YEL if sx < 0 else ORANGE, (sx * 830, 40, Z * 0.45), (138, 138, 60))
            g.box(HULL_D, (sx * 830, 40, 0), (240, 240, 50))
            rod(g, DECK_D, (sx * 780, 40, Z - 300), (sx * 420, 0, Z - 24), 14, steps=5)
        deck(g, -900, 900, -130, 130, Z, seed, n=6, posts=4)
        g.box(DECK_D, (0, 0, Z - 40), (1700, 60, 40))
        n = 4 if upper else 6
        spots = [(-650 + k * 1300.0 / (n - 1), -75 + (k % 2) * 50, Z, 0.0) for k in range(n)]
        W.crowd(sc, r, spots, seed)
        walker(sc, r, -300, 80, Z, 1500, 85)
        if upper:
            Z2 = Z + 360.0
            for sx in (-1, 1):
                g.box(DECK_D, (sx * 350, 110, Z + 170), (24, 24, 340))
            deck(g, -400, 400, -60, 150, Z2, seed + 2, n=3, posts=3)
            W.crowd(sc, r, [(-260, 10, Z2, -8), (0, 40, Z2, 0), (260, 10, Z2, 8)], seed + 2, kid_every=0)
            W.pennant(sc, 0, 140, Z2, 420, RED, WHITE, 0.3, w=160.0, hang=240.0)
        else:
            walker(sc, r, 400, 70, Z, 1500, -65)
            W.floater(sc, r, 60, -40, Z + 520, RED, yaw=6, phase=0.3)
        W.pennant(sc, -830, 40, Z, 600, YEL, RED, 0.1, w=-150.0, hang=230.0)
        W.pennant(sc, 830, 40, Z, 540, WHITE, MAG, 0.6, w=150.0, hang=230.0)
    return build


def mast(seed=1):
    """A lookout mast: a tub 9.2 m up, a half deck at 4.7 m on the road's side, a ladder between."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z, Z1 = 920.0, 470.0
        g.cyl(HULL_D, (0, 0, -40), 46, Z + 20, steps=6)
        for a in (30, 150, 270):
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            rod(g, DECK_D, (ca * 30, sa * 30, Z - 80), (ca * 520, sa * 520, -20), 6, steps=4)
            g.box(HULL_D, (ca * 520, sa * 520, 0), (100, 100, 50))
        g.cyl(DECK, (0, 0, Z - 18), 250, 18, steps=10)
        g.cyl(MAG, (0, 0, Z), 256, 66, steps=10)
        g.cyl(WHITE, (0, 0, Z + 66), 260, 10, steps=10)
        W.crowd(sc, r, [(math.sin(math.radians(a)) * 165, -math.cos(math.radians(a)) * 165, Z + 20, a * 0.6) for a in (-78, -26, 26, 78)], seed, kid_every=0)
        deck(g, -270, 270, -340, -40, Z1, seed, n=3, posts=2)
        for sx in (-1, 1):
            rod(g, DECK_D, (sx * 20, -20, Z1 - 240), (sx * 230, -300, Z1 - 24), 12, steps=5)
            g.box(DECK_D, (sx * 26, -54, (Z1 + Z) / 2.0), (6, 6, Z - Z1))
        for k in range(6):
            g.box(DECK_D, (0, -54, Z1 + 50 + k * 66), (52, 6, 6))
        W.crowd(sc, r, [(-180, -250, Z1, -8), (180, -250, Z1, 8)], seed + 1, kid_every=0)
        # on the ladder: going up and coming down
        W.watcher(sc, r, 0, -96, Z1 + 170, yaw=180, s=1.2, arm_l='up', arm_r='up', bob=-70, rate=0.14, phase=0.1)
        W.pennant(sc, 0, 40, Z, 560, YEL, RED, 0.2, w=170.0, hang=260.0)
    return build


def buoy(seed=1):
    """A round deck hung under a great float, 6.4 m over the sand, held down by three anchor lines."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z, R = 640.0, 340.0
        for a in (30, 150, 270):
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            g.box(HULL_D, (ca * 560, sa * 560, 10), (120, 120, 70))
            rod(g, DECK_D, (ca * 560, sa * 560, 40), (ca * (R - 20), sa * (R - 20), Z - 30), 6, steps=4)
        g.cyl(DECK, (0, 0, Z - 18), R, 18, steps=12)
        g.cyl(MAG, (0, 0, Z - 44), R - 16, 26, steps=12)
        ZF = Z + 760.0
        g.sphere(ORANGE, (0, 0, ZF), 290, scale=(1, 1, 0.75), steps=(5, 10))
        g.cyl(WHITE, (0, 0, ZF - 22), 294, 44, steps=10)
        for a in (45, 135, 225, 315):
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            rod(g, DECK_D, (ca * 230, sa * 230, ZF - 110), (ca * (R - 14), sa * (R - 14), Z), 7, steps=4)
        n = 12
        for k in range(n):                         # the rail's panels on the road's side
            am = math.radians((k + 0.5) * 360.0 / n)
            if math.cos(am) > 0.1:
                chord = 2 * (R - 10) * math.sin(math.pi / n)
                g.box(PANELS[(k + seed) % len(PANELS)], (math.sin(am) * (R - 14), -math.cos(am) * (R - 14), Z + 40), (chord - 10, 5, 60), rot=(0, math.degrees(am), 0))
        spots = [(math.sin(math.radians(a)) * 245, -math.cos(math.radians(a)) * 245, Z, a * 0.55) for a in (-95, -57, -19, 19, 57, 95)]
        W.crowd(sc, r, spots, seed)
        W.floater(sc, r, -470, -60, Z + 380, RED, yaw=10, phase=0.2)
        W.floater(sc, r, 480, -40, Z + 470, YEL, yaw=-12, phase=0.7)
    return build


def spire(seed=1):
    """A rock spire, 9.8 m, three ledges on the road's side."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(ROCK, (0, 0, -40), 430, 420, steps=9, top_r=340)
        g.cyl(ROCK_L, (0, 60, 380), 300, 300, steps=9, top_r=235)
        g.cyl(ROCK, (0, 110, 680), 200, 300, steps=8, top_r=150)
        for (x, y, z, rr, col) in ((-360, -120, 140, 70, PINK), (330, -180, 90, 60, ORANGE), (240, 20, 520, 50, YEL), (-200, 60, 800, 44, PINK)):
            g.sphere(col, (x, y, z), rr, steps=(3, 6))
        W.crowd(sc, r, [(-135, -272, 380, -10), (0, -292, 380, 0), (135, -272, 380, 10)], seed, kid_every=0)
        for i, a in enumerate((-47.0, 47.0)):
            sitter(sc, r, math.sin(math.radians(a)) * 338, -math.cos(math.radians(a)) * 338, 380, a, i + seed)
        W.crowd(sc, r, [(-75, -128, 680, -6), (75, -128, 680, 6)], seed + 2, kid_every=0)
        W.crowd(sc, r, [(-62, 80, 980, -8), (62, 64, 980, 8)], seed + 4, kid_every=0)
        W.pennant(sc, 0, 190, 980, 440, RED, WHITE, 0.3, w=170.0, hang=260.0)
        W.floater(sc, r, 330, -30, 1040, LIME, yaw=-10, phase=0.5)
    return build


def wreck_mast(seed=1):
    """A wreck's mast still standing over a stub of hull: divers sitting along its two yards, one in the nest."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(HULL_D, (0, 60, 70), (1000, 320, 220))
        g.box(RUST, (0, 60, 172), (1006, 326, 26))
        g.cyl(WOOD, (0, 0, -40), 38, 1200, steps=6)
        Y1, Y2, ZN = 640.0, 940.0, 1130.0
        g.box(WOOD, (0, 0, Y1), (1320, 44, 44))
        g.box(WOOD, (0, 0, Y2), (900, 40, 40))
        g.cyl(DECK, (0, 0, ZN - 12), 124, 12, steps=8)
        g.cyl(YEL, (0, 0, ZN), 128, 62, steps=8)
        for sx in (-1, 1):
            rod(g, DECK_D, (sx * 650, 0, Y1 + 22), (0, 0, ZN - 20), 5, steps=4)
            rod(g, DECK_D, (sx * 660, 0, Y1), (sx * 900, 60, 180), 5, steps=4)
        # what is left of a sail: blocks of colour under the lower yard, swinging a little
        b = sc.part(P.motion(rate=0.3, swing=5.0, pivot=(0, 10, Y1 - 22), axis=0.0, phase=0.2))
        for k in range(6):
            b.box(PANELS[(k + seed) % len(PANELS)], (-460 + (k + 0.5) * 920.0 / 6.0, 12, Y1 - 22 - 80), (920.0 / 6.0 - 6, 6, 160))
        for i, x in enumerate((-560, -320, 300, 540)):
            sitter(sc, r, x, 0, Y1 + 22, r.uniform(-8, 8), i + seed)
        for i, x in enumerate((-310, 300)):
            sitter(sc, r, x, 0, Y2 + 20, r.uniform(-8, 8), i + seed + 1)
        W.crowd(sc, r, [(0, -56, ZN + 16, 0)], seed + 1, kid_every=0, s=1.3)
        W.crowd(sc, r, [(-330, -40, 185, -6), (340, -30, 185, 6)], seed + 3, kid_every=0)
        W.floater(sc, r, 560, -50, Y2 + 60, RED, yaw=-10, phase=0.6)
    return build


def crane(seed=1):
    """A salvage crane: a platform 9 m up with its cab, a jib over the find, and a load going up and down with two
    riggers riding it."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        X0, Z = -480.0, 900.0
        g.box(HULL_D, (X0, 60, (Z - 40) / 2.0), (170, 170, Z + 40))
        g.box(HULL_D, (X0, 60, 0), (300, 300, 50))
        for z in (300, 600):
            g.box(YEL, (X0, 60, z), (178, 178, 44))
        deck(g, X0 - 380, X0 + 380, -300, 190, Z, seed, n=4, posts=3)
        g.box(YEL, (X0, 70, Z + 115), (280, 240, 230))
        g.box(PORT, (X0, -52, Z + 140), (200, 10, 90))
        g.box(LAMP, (X0, -54, Z + 208), (120, 6, 26), glow=0.5)
        ZJ = Z + 270.0
        g.box(YEL, (X0 + 650, 70, ZJ), (1700, 60, 60))
        g.box(HULL_D, (X0 - 320, 70, ZJ - 10), (240, 160, 140))
        g.box(HULL_D, (X0, 70, Z + 370), (44, 44, 280))
        rod(g, DECK_D, (X0, 70, Z + 500), (X0 + 1420, 70, ZJ + 30), 7, steps=4)
        rod(g, DECK_D, (X0, 70, Z + 500), (X0 - 400, 70, ZJ + 60), 7, steps=4)
        W.crowd(sc, r, [(X0 - 270, -230, Z, -8), (X0, -240, Z, 0), (X0 + 270, -230, Z, 8)], seed, kid_every=0)
        W.crowd(sc, r, [(X0 - 60, 30, Z + 230, -6), (X0 + 70, 60, Z + 230, 6)], seed + 2, kid_every=0, s=1.3)
        # the load: it rides 1.7 m up and down the line, the two on it with it
        XL, YL, ZL = X0 + 1300.0, 70.0, 560.0
        g.box(DECK_D, (XL, YL, (300 + ZJ - 30) / 2.0), (7, 7, ZJ - 30 - 300))
        mo = dict(phase=0.2, rate=0.12, bob=-170.0)
        ld = sc.part(P.motion(**mo))
        ld.box(ORANGE, (XL, YL, ZL - 9), (340, 270, 18))
        ld.box(WHITE, (XL, YL - 136, ZL + 16), (340, 6, 50))
        for sx in (-1, 1):
            for sy in (-1, 1):
                tilt_box(ld, DECK_D, (XL + sx * 160, YL + sy * 125, ZL), (XL, YL, ZL + 330), 6, 6)
        W.watcher(sc, r, XL - 85, YL - 40, ZL, yaw=-8, s=1.3, arm_l='up', arm_r='vee', **mo)
        W.watcher(sc, r, XL + 85, YL - 40, ZL, yaw=8, s=1.3, arm_l='wave', arm_r='up', **mo)
        W.pennant(sc, X0 - 370, 180, Z, 420, RED, WHITE, 0.4, w=-150.0, hang=230.0)
    return build


def terraces(seed=1):
    """A stepped tower block of Nereida: three roofs (4, 7.2 and 10.2 m), a crowd on each, lit windows under them."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z1, Z2, Z3 = 400.0, 720.0, 1020.0
        g.box(DECK, (0, 0, (Z1 - 40) / 2.0), (1000, 620, Z1 + 40))
        g.box(DECK_D, (0, 75, (Z1 + Z2) / 2.0), (700, 470, Z2 - Z1))
        g.box(DECK, (0, 145, (Z2 + Z3) / 2.0), (400, 330, Z3 - Z2))
        for k in range(4):
            g.box(LAMP, (-360 + k * 240, -312, 210), (130, 6, 76), glow=0.5)
        for k in range(3):
            g.box(LAMP, (-220 + k * 220, -162, Z1 + 190), (120, 6, 70), glow=0.5)
        for k in range(2):
            g.box(LAMP, (-90 + k * 180, -22, Z2 + 180), (100, 6, 66), glow=0.5)
        W.fascia(g, -500, 500, -313, Z1 + 34, 64, 6, seed)
        W.fascia(g, -350, 350, -163, Z2 + 34, 64, 4, seed + 2)
        W.fascia(g, -200, 200, -23, Z3 + 34, 64, 2, seed + 4)
        for (L, y, z) in ((1000, -310, Z1), (700, -160, Z2), (400, -20, Z3)):
            g.box(DECK_D, (0, y, z + 112), (L, 7, 7))
            for sx in (-1, 1):
                g.box(DECK_D, (sx * (L / 2.0 - 4), y, z + 56), (8, 8, 112))
        W.crowd(sc, r, [(-400 + k * 200, -255 + (k % 2) * 40, Z1, 0.0) for k in range(5)], seed)
        W.crowd(sc, r, [(-240, -100, Z2, -6), (0, -92, Z2, 0), (240, -100, Z2, 6)], seed + 3, kid_every=0)
        W.crowd(sc, r, [(-95, 50, Z3, -6), (95, 50, Z3, 6)], seed + 5, kid_every=0)
        W.pennant(sc, 0, 270, Z3, 440, YEL, RED, 0.3, w=170.0, hang=260.0)
        W.pennant(sc, -490, 280, Z1, 460, WHITE, MAG, 0.7, w=-140.0, hang=230.0)
    return build


def rope_bridge(seed=1):
    """A plank bridge between two rock stacks, 6.6 m over the sand: three crossing it, others waving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z = 660.0
        for sx in (-1, 1):
            g.cyl(ROCK if sx < 0 else ROCK_L, (sx * 820, 20, -40), 250, Z + 40, steps=8, top_r=190)
            g.sphere(PINK if sx < 0 else ORANGE, (sx * 1010, -90, 160), 64, steps=(3, 6))
        n = 6
        for k in range(n):
            g.box(WOOD if k % 2 else DECK, (-640 + (k + 0.5) * 1280.0 / n, 20, Z - 8), (1280.0 / n - 6, 200, 16))
        for y in (-80, 120):
            g.box(DECK_D, (0, y, Z + 106), (1300, 6, 6))
            for sx in (-1, 1):
                g.box(DECK_D, (sx * 646, y, Z + 53), (8, 8, 106))
        for k in range(8):                         # bunting on the road's side rail
            g.box(PANELS[(k + seed) % len(PANELS)], (-560 + k * 160, -82, Z + 74), (70, 4, 56))
        walker(sc, r, -500, 60, Z, 1560, 80)
        walker(sc, r, 0, 40, Z, 1560, 100)
        walker(sc, r, 420, 70, Z, 1560, -65)
        W.crowd(sc, r, [(-220, -35, Z, -4), (190, -35, Z, 4)], seed, kid_every=0)
        W.crowd(sc, r, [(-890, -90, Z, -12), (-760, -110, Z, -4), (760, -110, Z, 4), (890, -90, Z, 12)], seed + 2, kid_every=0)
        W.pennant(sc, -820, 150, Z, 440, RED, WHITE, 0.1, w=-150.0, hang=240.0)
        W.pennant(sc, 820, 150, Z, 400, YEL, MAG, 0.6, w=150.0, hang=240.0)
    return build


def lift(seed=1):
    """An open lift between two landings (2.6 m on the right, 8.6 m on the left): the cage rides 3 m up and down
    with three in it."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        ZT = 1200.0
        for sx in (-1, 1):
            g.box(HULL_D, (sx * 190, 90, (ZT - 40) / 2.0), (44, 44, ZT + 40))
            g.box(HULL_D, (sx * 190, 90, 0), (130, 130, 50))
        g.box(DECK, (0, 20, ZT + 15), (520, 360, 30))
        g.box(YEL, (0, 70, ZT + 90), (260, 200, 120))
        g.box(LAMP, (0, -32, ZT + 90), (100, 6, 30), glow=0.5)
        ZL, ZH = 260.0, 860.0
        deck(g, 230, 900, -200, 160, ZL, seed, n=3, posts=2)
        deck(g, -900, -230, -200, 160, ZH, seed + 3, n=3, posts=2)
        for x in (320, 820):
            g.box(DECK_D, (x, 100, (ZL - 40) / 2.0 - 10), (30, 30, ZL + 20))
        for x in (-820, -320):
            g.box(DECK_D, (x, 100, (ZH - 40) / 2.0 - 10), (36, 36, ZH + 20))
        rod(g, DECK_D, (-800, 100, 200), (-340, 100, ZH - 30), 10, steps=4)
        W.crowd(sc, r, [(340, -110, ZL, -6), (560, -130, ZL, 0), (780, -110, ZL, 6)], seed, kid_every=0)
        W.crowd(sc, r, [(-780, -110, ZH, -6), (-560, -130, ZH, 0), (-340, -110, ZH, 6)], seed + 2, kid_every=0)
        # the cage
        ZC = 560.0
        mo = dict(phase=0.0, rate=0.1, bob=-300.0)
        c = sc.part(P.motion(**mo))
        c.box(ORANGE, (0, 0, ZC - 8), (300, 260, 16))
        c.box(ORANGE, (0, 0, ZC + 276), (300, 260, 12))
        c.box(MAG, (0, 128, ZC + 135), (300, 8, 270))
        c.box(WHITE, (0, -128, ZC + 30), (300, 6, 56))
        for sx in (-1, 1):
            c.box(DECK_D, (sx * 146, -126, ZC + 135), (8, 8, 270))
        W.watcher(sc, r, -95, -40, ZC, yaw=-8, s=1.2, arm_l='vee', arm_r='vee', **mo)
        W.watcher(sc, r, 0, 30, ZC, yaw=0, s=1.2, arm_l='wave', arm_r='hip', **mo)
        W.watcher(sc, r, 95, -40, ZC, yaw=8, s=1.2, arm_l='hip', arm_r='wave', **mo)
        W.pennant(sc, -890, 150, ZH, 420, RED, WHITE, 0.2, w=-150.0, hang=230.0)
    return build


UP = dict(grow=(80.0, 80.0, 200.0))
JOBS = [
    ('SM_P3D_Gantry_A', gantry(1), UP),
    ('SM_P3D_Gantry_B', gantry(2, upper=True), UP),
    ('SM_P3D_Mast', mast(1), UP),
    ('SM_P3D_Buoy', buoy(1), UP),
    ('SM_P3D_Spire', spire(1), UP),
    ('SM_P3D_WreckMast', wreck_mast(1), UP),
    ('SM_P3D_Crane', crane(1), UP),
    ('SM_P3D_Terraces', terraces(1), UP),
    ('SM_P3D_RopeBridge', rope_bridge(1), UP),
    ('SM_P3D_Lift', lift(1), UP),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
