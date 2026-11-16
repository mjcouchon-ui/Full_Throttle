"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea, pass 3, builder e: FAR AND SKY
(/Game/People/UnderTheSea/SM_P3E_*).

What lives from 150 m out to the trench walls and in the water high over them. Under the sea the "sky" is the open
water over the sand, so the things of a sky are here in their sea form, and PEOPLE are in every one of them (the first
far kit, ft_people_sea_far.py, is mostly creatures; none of it is copied):

    SM_P3E_DiveLine_A / _B   a shot line from an anchor block up to a buoy 62 m over the sand, a team of aquanauts
                             three times life size going down it (A) or coming up it (B), kicking, out of step
    SM_P3E_Kites             a pylon with a platform, four people on it flying great diamond kites in the current
    SM_P3E_Balloons_A        three striped lift balloons with baskets, two people waving from each, wheeling slowly
    SM_P3E_Balloons_B        two bunches of small balloons carrying a bench each, two riders on it, wheeling the other way
    SM_P3E_Gliders           five delta-wing sea gliders wheeling at different heights, a pilot slung under each
    SM_P3E_StarRing          formation swimmers: a star of eight and a star of five, hands to the hub, turning to and fro
    SM_P3E_FishHerd          herders on scooters driving a shoal of yellow fish along the trench (it travels)
    SM_P3E_WaterBus          Nereida's open-deck water bus under a striped awning, full of waving passengers (it travels)
    SM_P3E_RidgeClimbers     a rock ridge 52 m tall for the horizon, two roped teams of giants on it, a flag on the top
    SM_P3E_DivingBirds       a flock of nine great sea birds "flying" under water, wheeling, wings beating

Everything is BIG (figures 2.4 to 4.5 times life), in the level's hot colours (yellow, coral, pink, white, lime: never
grey with a white belly and a tall back fin, never the blue of the boss's torpedo fish), carries a little steady light
of its own colour against the haze (lit(): a constant, nothing blinks), and is saved with far=True (the always-moving
master). Every piece is TALL and is placed 150 m and more from the road (C:/ftwork/p3/L6e.inl).

How a whole thing moves (ft_people.py's shader): hop / float / swing use a part's own rate and phase; what moves a
whole group in one piece is travel and orbit (they depend only on the base and the round rate). So the groups here
travel or circle as a whole about the mesh's middle (base (0, 0)), and limbs swing on top of that. Rig hands figure()
a scene whose every part takes the group's base and travel, so that a figure rides with what it stands on.

A piece is drawn like every prop: cm; z = 0 is what it stands on; the middle of the footprint is the origin; the FRONT
faces -Y (the road); long pieces run along X (the road).

Run in the editor:  import ft_people_sea_p3e; ft_people_sea_p3e.run()     (or run(['SM_P3E_Kites']))
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/UnderTheSea'

SUITS = ('#ffc93c', '#ff7a3c', '#ff5fa2', '#fff6ea', '#b8ff5a', '#ff2d3c', '#f4a6ff', '#7dffb0')
FINS = ('#ff2d3c', '#ffc93c', '#22202a', '#ff5fa2', '#fff6ea', '#b8ff5a')
HELM, VISOR, STEEL, DARK, WHITE, YEL = '#f4f9ff', '#22202a', '#8f9bb3', '#22202a', '#fff6ea', '#ffc93c'
ORANGE, PINK, LIME, RED, MINT, LILAC = '#ff7a3c', '#ff5fa2', '#b8ff5a', '#ff2d3c', '#7dffb0', '#f4a6ff'
DECK, DECK_D = '#cfe3ea', '#7fa2b4'
ROCK, ROCK_L = '#5d6fa3', '#7f8fc0'
BASKET = '#c9a070'
BUBBLE = '#e6f6ff'
SPARK = '#fff1c0'


def _tv(length, speed):
    """(length cm, speed cm/s) -> the motion's (length, rounds a second)."""
    return (float(length), float(speed) / float(length))


class Rig(object):
    """A scene for figure() whose every part takes the group's own values (base and travel: the group's line or
    circle), so that a figure stays on the thing it rides."""

    def __init__(self, sc, **over):
        self.sc = sc
        self.over = over

    def part(self, mo=None, at=(0.0, 0.0, 0.0), yaw=0.0, s=1.0):
        mo = dict(mo or P.STILL)
        mo.update(self.over)
        return self.sc.part(mo, at, yaw, s)

    def still(self, at=(0.0, 0.0, 0.0), yaw=0.0, s=1.0):
        return self.part(None, at, yaw, s)


# ------------------------------------------------------------------------------------------ a far aquanaut
def swimmer(sc, x, y, z, heading=0.0, pitch=0.0, s=3.0, suit=SUITS[0], fins=FINS[0], phase=0.0, rate=0.4, bob=-30.0, kick=14.0,
            base=None, travel=(0.0, 0.0), scooter=None, streamer=None, lamp=False, mo=None, frame=None):
    """An aquanaut for the far water, belly down, the hips at (x, y, z), the head towards `heading` (degrees, 0 = +X)
    and raised by `pitch`. s = 3: three times life (5 m). Chunky on purpose (seen side-on a true swimmer is a line).
    base / travel: the group's (so that a team keeps its shape). mo: one motion for the whole swimmer (a rigid
    formation: no kick). frame: (forward, right, up) unit vectors instead of heading and pitch. About 110 triangles."""
    if frame:
        f, rt, u = frame
    else:
        h, p = math.radians(heading), math.radians(pitch)
        f = (math.cos(h) * math.cos(p), math.sin(h) * math.cos(p), math.sin(p))
        rt = (-math.sin(h), math.cos(h), 0.0)
        u = (-math.cos(h) * math.sin(p), -math.sin(h) * math.sin(p), math.cos(p))

    def W(a, b=0.0, c=0.0):
        a, b, c = a * s, b * s, c * s
        return (x + f[0] * a + rt[0] * b + u[0] * c, y + f[1] * a + rt[1] * b + u[1] * c, z + f[2] * a + rt[2] * b + u[2] * c)

    common = dict(phase=phase, rate=rate, bob=bob, base=base or (x, y), travel=travel)
    b = sc.part(mo) if mo else sc.part(P.motion(pivot=(x, y, z), **common))
    tilt_box(b, suit, W(-6), W(62), 50 * s, 36 * s)
    tilt_box(b, YEL if suit != YEL else WHITE, W(4, 0, 26), W(52, 0, 26), 24 * s, 22 * s)       # the tank on the back
    tilt_box(b, HELM, W(62, 0, 4), W(112, 0, 4), 50 * s, 46 * s)
    tilt_box(b, VISOR, W(98, 0, -16), W(116, 0, -10), 38 * s, 22 * s)
    for sd in (-1, 1):
        tilt_box(b, suit, W(52, sd * 30, 0), W(120, sd * 20, -18), 16 * s, 16 * s)
    if scooter:
        tilt_box(b, scooter, W(100, 0, -44), W(205, 0, -44), 46 * s, 46 * s)
        tilt_box(b, DARK, W(84, 0, -44), W(100, 0, -44), 60 * s, 60 * s)
    if lamp:
        b.sphere(SPARK, W(134, 0, -22), 18 * s, steps=(3, 5), glow=1.0)
    for sd in (-1, 1):
        hip = W(-4, sd * 13, 0)
        m = b if (mo or not kick) else sc.part(P.motion(swing=sd * kick, pivot=hip, axis=heading + 90.0, **common))
        tilt_box(m, suit, hip, W(-80, sd * 14, 0), 20 * s, 20 * s)
        tilt_box(m, fins, W(-76, sd * 15, 0), W(-142, sd * 17, -4), 38 * s, 10 * s)
    if streamer:
        for k in range(2):
            st = b if mo else sc.part(P.motion(swing=10.0 + 5.0 * k, pivot=W(-142), axis=heading + 90.0, **common))
            tilt_box(st, streamer if k == 0 else WHITE, W(-146 - k * 100, 0, 6), W(-246 - k * 100, 0, 6), 8 * s, 34 * s)
    return W


def bubbles(sc, pts, r, rate=0.3, rise=120.0, size=50.0, **over):
    """A string of bubbles, each floating in its own time. over: the group's base / travel."""
    for k, p in enumerate(pts):
        mo = P.motion(phase=k * 0.19, rate=rate, bob=-rise, pivot=p)
        mo.update(over)
        sc.part(mo).sphere(BUBBLE, p, size + 12.0 * (k % 4) + r.uniform(0, 14), steps=(3, 6))


# ------------------------------------------------------------------------------------------ the dive line
def dive_line(seed=1, sign=1, n=8, up=False):
    """A shot line from an anchor block on the bottom to a buoy 62 m up, and a team on it: n aquanauts three times
    life size going down head first (up=False) or coming up (up=True), each kicking and drifting in its own time; the
    leader of a descent carries a lamp (a steady light)."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        fx = -sign * 1900.0
        top = (sign * 300.0, 0.0, 6200.0)
        g.box(DARK, (fx, 0, -150), (540, 540, 900))                       # the block (the land may fall away under it)
        g.box(YEL, (fx, 0, 330), (600, 600, 70))
        g.box(ORANGE, (fx, 0, 420), (160, 160, 120))
        tilt_box(g, WHITE, (fx, 0, 400), top, 18, 18)
        fl = sc.part(P.motion(rate=0.18, bob=-50.0, pivot=top))
        fl.sphere(ORANGE, top, 290, scale=(1, 1, 0.8), steps=(4, 8))
        fl.cyl(WHITE, (top[0], top[1], top[2] - 50), 304, 100, steps=8)
        fl.box(WHITE, (top[0], 0, top[2] + 420), (18, 18, 520))
        fl.box(RED if not up else PINK, (top[0] + sign * 190, 0, top[2] + 560), (360, 16, 220))
        dx, dz = top[0] - fx, top[2] - 400.0
        ang = math.degrees(math.atan2(dz, abs(dx)))
        fwd = 0.0 if sign > 0 else 180.0                                  # the way the line climbs
        for k in range(n):
            t = 0.1 + 0.78 * k / float(n - 1)
            px, pz = fx + dx * t, 400.0 + dz * t
            swimmer(sc, px, -170.0 - (k % 2) * 110.0, pz, heading=(fwd if up else fwd + 180.0), pitch=(ang if up else -ang) * 0.8, s=3.0,
                    suit=SUITS[(k * 3 + seed) % len(SUITS)], fins=FINS[(k + seed) % len(FINS)], phase=r.random(), rate=r.uniform(0.3, 0.45),
                    bob=-45.0, kick=15.0, lamp=(k == 0 and not up), streamer=((PINK, LIME, ORANGE)[k % 3] if k % 3 == 0 else None))
        bubbles(sc, [(fx + dx * (0.15 + 0.14 * k) + r.uniform(-80, 80), 160 + r.uniform(-60, 60), 900 + dz * (0.15 + 0.14 * k)) for k in range(6)], r)
    return build


# ------------------------------------------------------------------------------------------ the kite flyers
def kites(seed=1):
    """A pylon with a round platform 15 m up, four people three times life size on it, each flying a great diamond
    kite on a line in the current: the kites lean to and fro, each in its own time, 40 to 58 m up."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(DECK_D, (0, 0, 400), (360, 360, 2000))
        for k in range(4):
            g.box(DARK, (0, 0, 150 + k * 330), (390, 390, 60))
        g.cyl(DECK, (0, 0, 1400), 680, 80, steps=10)
        g.cyl(ORANGE, (0, 0, 1380), 700, 30, steps=10)
        for k in range(8):
            a = math.radians(k * 45.0 + 22.5)
            g.box(WHITE, (math.cos(a) * 640, math.sin(a) * 640, 1570), (26, 26, 190))
        spec = ((-420, -2300, -260, 4300, PINK, WHITE), (-140, -800, 240, 5700, YEL, RED), (140, 900, -120, 4900, LIME, PINK), (420, 2400, 200, 4100, ORANGE, WHITE))
        for k, (px, ox, oy, kz, col, col2) in enumerate(spec):
            side = 1 if ox > 0 else -1
            P.figure(sc, px, -160, 1480, yaw=-side * 20.0, detail=1, arm_r=('up' if side > 0 else 'hip'), arm_l=('up' if side < 0 else 'hip'),
                     bob=10.0, rate=0.5, phase=r.random(), **P.look(r, s=3.0, top=SUITS[(k * 2 + seed) % len(SUITS)], hat=('cap', 'sun', None, 'beanie')[k], hat_col=col))
            hand = (px + side * 100.0, -160.0, 1480 + 3.0 * 196.0)
            K = (px + ox, -160.0 + oy, float(kz))
            m = sc.part(P.motion(phase=r.random(), rate=r.uniform(0.11, 0.16), swing=r.uniform(6.0, 9.0), pivot=hand, axis=90.0))
            tilt_box(m, WHITE, hand, K, 16, 16)
            m.box(col, K, (800, 30, 800), rot=(45, 0, 0))
            m.box(col2, (K[0], K[1] - 22, K[2]), (1060, 12, 70))
            m.box(col2, (K[0], K[1] - 22, K[2]), (70, 12, 1060))
            for j in range(4):
                m.box(col2 if j % 2 else col, (K[0] + (60 if j % 2 else -60), K[1], K[2] - 700 - j * 270), (90, 14, 230))
    return build


# ------------------------------------------------------------------------------------------ balloons
def balloons(seed=1, rate=0.011):
    """Three striped lift balloons, 11 to 12 m across, a basket under each and two people waving from it. They wheel
    slowly about the middle of the piece, 26 to 43 m up."""
    def build(sc):
        r = random.Random(seed)
        tv = (-1.0, rate)
        rig = Rig(sc, base=(0.0, 0.0), travel=tv)
        spec = ((1500.0, 20.0, 2600.0, 560.0, PINK, WHITE), (1950.0, 150.0, 4300.0, 620.0, YEL, RED), (1700.0, 265.0, 3400.0, 520.0, MINT, ORANGE))
        for k, (rad, a_deg, z, R, col, band) in enumerate(spec):
            a = math.radians(a_deg + seed * 40.0)
            cx, cy = math.cos(a) * rad, math.sin(a) * rad
            b = sc.part(P.motion(pivot=(cx, cy, z), base=(0.0, 0.0), travel=tv))
            b.sphere(col, (cx, cy, z), R, scale=(1, 1, 1.12), steps=(5, 10))
            b.cyl(band, (cx, cy, z - R * 0.14), R * 1.02, R * 0.28, steps=10)
            b.cyl(band, (cx, cy, z + R * 0.62), R * 0.8, R * 0.16, steps=10)
            b.cyl(col, (cx, cy, z - R * 1.5), R * 0.24, R * 0.56, steps=8, top_r=R * 0.62)
            bz = z - R * 2.15
            for sx in (-1, 1):
                for sy in (-1, 1):
                    tilt_box(b, WHITE, (cx + sx * 150, cy + sy * 150, bz + 110), (cx + sx * R * 0.5, cy + sy * R * 0.5, z - R * 0.86), 16, 16)
            b.box(BASKET, (cx, cy, bz), (360, 360, 220))
            b.box(band, (cx, cy, bz + 120), (390, 390, 34))
            for j in (-1, 1):
                P.figure(rig, cx + j * 85.0, cy, bz - 150.0, yaw=r.uniform(0, 360), detail=0, arm_r='wave', arm_l=('up' if j > 0 else 'down'), swing_r=26.0,
                         rate=r.uniform(0.7, 1.0), phase=r.random(), **P.look(r, s=2.4, top=SUITS[(k * 2 + j + 1 + seed) % len(SUITS)]))
    return build


def bunches(seed=1, rate=-0.013):
    """Two bunches of seven small balloons, each carrying a bench with two riders who wave. They wheel slowly the other
    way about the middle of the piece, 30 and 46 m up."""
    def build(sc):
        r = random.Random(seed)
        tv = (-1.0, rate)
        rig = Rig(sc, base=(0.0, 0.0), travel=tv)
        cols = (PINK, YEL, LIME, ORANGE, WHITE, RED, LILAC, MINT)
        for k, (rad, a_deg, z) in enumerate(((1500.0, 70.0, 3000.0), (2000.0, 250.0, 4600.0))):
            a = math.radians(a_deg + seed * 55.0)
            cx, cy = math.cos(a) * rad, math.sin(a) * rad
            b = sc.part(P.motion(pivot=(cx, cy, z), base=(0.0, 0.0), travel=tv))
            seat = z - 1250.0
            for j in range(7):
                if j == 0:
                    ox, oy, oz = 0.0, 0.0, 330.0
                else:
                    aa = math.radians(j * 60.0)
                    ox, oy, oz = math.cos(aa) * 400.0, math.sin(aa) * 400.0, (j % 2) * 150.0 - 60.0
                b.sphere(cols[(j + k * 3 + seed) % len(cols)], (cx + ox, cy + oy, z + oz), 285, scale=(1, 1, 1.15), steps=(4, 8))
                tilt_box(b, WHITE, (cx + ox * 0.9, cy + oy * 0.9, z + oz - 300), (cx + (120 if ox > 0 else -120), cy, seat + 60), 14, 14)
            b.box(BASKET, (cx, cy, seat), (460, 150, 50))
            b.box(ORANGE, (cx, cy + 80, seat + 130), (460, 30, 240))
            for j in (-1, 1):
                P.figure(rig, cx + j * 110.0, cy, seat + 25.0 - 46.0 * 2.4, yaw=0.0, legs='sit', detail=0, arm_r='wave', arm_l=('wave' if j < 0 else 'down'),
                         swing_r=28.0, swing_l=(24.0 if j < 0 else 0.0), rate=r.uniform(0.7, 1.0), phase=r.random(),
                         **P.look(r, s=2.4, top=SUITS[(k * 3 + j + 1 + seed) % len(SUITS)]))
    return build


# ------------------------------------------------------------------------------------------ the gliders
def gliders(seed=1):
    """Five delta-wing sea gliders, 14 m from tip to tip, wheeling about the middle of the piece at different heights
    (26 to 54 m) and rates, a pilot 2.6 times life size slung under each, kicking, a streamer at each wing tip."""
    def build(sc):
        r = random.Random(seed)
        wings = ((PINK, WHITE), (YEL, RED), (LIME, PINK), (ORANGE, WHITE), (WHITE, PINK))
        spec = ((1300.0, 10.0, 2600.0, 0.020), (2300.0, 95.0, 3400.0, 0.016), (1700.0, 170.0, 4200.0, 0.020), (2500.0, 250.0, 4800.0, 0.016), (1100.0, 310.0, 5400.0, 0.024))
        for k, (rad, a_deg, z, rate) in enumerate(spec):
            a = math.radians(a_deg + seed * 30.0)
            cx, cy = math.cos(a) * rad, math.sin(a) * rad
            head = math.degrees(a) + 90.0
            tx, ty = -math.sin(a), math.cos(a)                  # the way it flies (anticlockwise)
            nx, ny = -ty, tx                                    # its left
            tv = (-1.0, rate)
            col, col2 = wings[(k + seed) % len(wings)]
            m = sc.part(P.motion(pivot=(cx, cy, z), base=(0.0, 0.0), travel=tv))
            pt = lambda f, l: (cx + tx * f + nx * l, cy + ty * f + ny * l)
            m.plan(col, [pt(520, 0), pt(-300, 700), pt(-120, 0), pt(-300, -700)], z, z + 26.0)
            m.plan(col2, [pt(330, 0), pt(-190, 330), pt(-90, 0), pt(-190, -330)], z + 26.0, z + 36.0)
            tilt_box(m, DARK, (cx + tx * 500, cy + ty * 500, z - 12), (cx - tx * 260, cy - ty * 260, z - 12), 22, 22)
            for sd in (-1, 1):
                tilt_box(m, DARK, (cx + tx * 120, cy + ty * 120, z - 12), (cx + nx * sd * 90, cy + ny * sd * 90, z - 210), 16, 16)
                st = sc.part(P.motion(phase=r.random(), rate=0.6, swing=14.0, pivot=(cx - tx * 300 + nx * sd * 700, cy - ty * 300 + ny * sd * 700, z), axis=P.UP,
                                      base=(0.0, 0.0), travel=tv))
                tilt_box(st, col2 if col2 != WHITE else RED, (cx - tx * 300 + nx * sd * 700, cy - ty * 300 + ny * sd * 700, z + 6),
                         (cx - tx * 760 + nx * sd * 700, cy - ty * 760 + ny * sd * 700, z + 6), 12, 70)
            swimmer(sc, cx - tx * 40, cy - ty * 40, z - 250.0, heading=head, pitch=0.0, s=2.6, suit=SUITS[(k * 3 + seed) % len(SUITS)], fins=FINS[(k + seed) % len(FINS)],
                    phase=r.random(), rate=0.5, bob=0.0, kick=12.0, base=(0.0, 0.0), travel=tv)
    return build


# ------------------------------------------------------------------------------------------ the formation
def star_ring(seed=1):
    """Formation swimmers: a star of eight aquanauts three times life size, upright in the water like a wheel, hands
    to the hub and fins out (19 m across, its hub 38 m up), turning to and fro; a star of five higher up beside it
    turning the other way. A steady lamp at each hub."""
    def build(sc):
        def star(hx, hy, hz, n, s, swing, rate, phase, k0):
            hub = (hx, hy, hz)
            mo = P.motion(phase=phase, rate=rate, swing=swing, pivot=hub, axis=90.0)
            b = sc.part(mo)
            b.sphere(SPARK, hub, 46 * s, steps=(4, 8), glow=1.0)
            rad = 46 * s + 120 * s
            for k in range(n):
                a = math.radians(k * 360.0 / n + 12.0)
                e = (math.cos(a), 0.0, math.sin(a))
                frame = ((-e[0], 0.0, -e[2]), (-e[2], 0.0, e[0]), (0.0, -1.0, 0.0))
                swimmer(sc, hx + e[0] * rad, hy, hz + e[2] * rad, s=s, suit=SUITS[(k + k0) % len(SUITS)], fins=FINS[(k * 2 + k0) % len(FINS)],
                        mo=mo, frame=frame, streamer=(PINK, YEL, LIME, ORANGE)[k % 4])
        star(-500.0, 0.0, 3800.0, 8, 3.0, 40.0, 0.07, 0.0, seed)
        star(1750.0, 300.0, 5700.0, 5, 2.6, -55.0, 0.09, 0.3, seed + 3)
    return build


# ------------------------------------------------------------------------------------------ the fish herd
def fish_herd(seed=1, sign=1, length=9000.0, speed=340.0):
    """Herders at work: three aquanauts on scooters, 3.2 times life size, drive a shoal of forty yellow and orange fish
    (2.2 to 3.2 m each) along the trench. The whole of it travels the mesh's X and comes round again; the shoal
    shimmers (six groups floating out of step)."""
    def build(sc):
        r = random.Random(seed)
        tv = _tv(length, sign * speed)
        over = dict(base=(0.0, 0.0), travel=tv)
        cx, cz = sign * 700.0, 2700.0
        groups = [sc.part(P.motion(phase=k / 6.0, rate=0.5, bob=-70.0, pivot=(cx, 0.0, cz), **over)) for k in range(6)]
        cols = ((YEL, ORANGE), (ORANGE, YEL), (YEL, PINK), ('#ffe27a', ORANGE))
        for k in range(40):
            while True:
                ux, uy, uz = r.uniform(-1, 1), r.uniform(-1, 1), r.uniform(-1, 1)
                if ux * ux + uy * uy + uz * uz <= 1.0:
                    break
            x, y, z = cx + ux * 1500.0, uy * 520.0, cz + uz * 850.0
            L = r.uniform(220, 320)
            body, tail = cols[k % len(cols)]
            m = groups[k % 6]
            m.box(body, (x, y, z), (L, L * 0.24, L * 0.5))
            m.box(tail, (x - sign * L * 0.62, y, z), (L * 0.34, L * 0.14, L * 0.34), rot=(45, 0, 0))
        head = 0.0 if sign > 0 else 180.0
        for k, (bx, by, bz) in enumerate(((-2500.0, -150.0, 2700.0), (-1900.0, 250.0, 3900.0), (-1700.0, -300.0, 1500.0))):
            swimmer(sc, sign * bx, by, bz, heading=head, pitch=(0.0, -8.0, 8.0)[k], s=3.2, suit=(PINK, WHITE, LIME)[(k + seed) % 3], fins=FINS[(k + seed) % len(FINS)],
                    phase=r.random(), rate=0.45, bob=-40.0, kick=10.0, scooter=(RED, ORANGE, PINK)[k], streamer=(WHITE, PINK, ORANGE)[k], **over)
        bubbles(sc, [(sign * (-3300.0 - k * 330.0), r.uniform(-200, 200), 2700.0 + r.uniform(-700, 900)) for k in range(4)], r, **over)
    return build


# ------------------------------------------------------------------------------------------ the water bus
def water_bus(seed=1, sign=1, length=9000.0, speed=430.0):
    """Nereida's water bus: an open barge 26 m long on two floats, 22 m up, a striped awning on posts over its deck,
    nine passengers 2.5 times life size at the rail on the road's side, waving, and the driver at the bow. It travels
    the mesh's X and comes round again."""
    def build(sc):
        r = random.Random(seed)
        tv = _tv(length, sign * speed)
        over = dict(base=(0.0, 0.0), travel=tv)
        rig = Rig(sc, **over)
        z0 = 2200.0
        deck = z0 + 150.0
        b = sc.part(P.motion(pivot=(0.0, 0.0, z0), **over))
        b.box(YEL, (0, 0, z0), (2600, 600, 300))
        b.box(YEL, (sign * 1420, 0, z0 + 30), (300, 480, 240))
        b.box(WHITE, (0, -306, z0 + 40), (2500, 14, 90))
        for sd in (-1, 1):
            b.box(ORANGE, (0, sd * 240, z0 - 210), (2300, 150, 130))
        b.box(DARK, (-sign * 1380, 0, z0 - 40), (180, 420, 200))
        for k in range(6):                                                # the rail on the road's side
            b.box(WHITE, (-1250 + k * 500, -290, deck + 110), (26, 26, 220))
        b.box(WHITE, (0, -290, deck + 220), (2540, 26, 26))
        for k in range(4):                                                # the awning
            for sd in (-1, 1):
                b.box(WHITE, (-1100 + k * 733, sd * 270, deck + 300), (30, 30, 600))
        b.box(PINK, (0, 0, deck + 630), (2500, 680, 60))
        for k in range(5):
            b.box(WHITE, (-1000 + k * 500, 0, deck + 646), (250, 690, 36))
        for k in range(9):
            P.figure(rig, -1040 + k * 250 + r.uniform(-30, 30), -170 + (k % 2) * 110, deck, yaw=r.uniform(-20, 20), detail=0,
                     arm_r=('wave', 'up', 'vee')[k % 3], arm_l=('down', 'up', 'vee')[k % 3], swing_r=26.0, swing_l=(20.0 if k % 3 else 0.0),
                     rate=r.uniform(0.7, 1.1), phase=r.random(), **P.look(r, s=r.uniform(2.3, 2.6), top=SUITS[(k + seed) % len(SUITS)]))
        P.figure(rig, sign * 1200.0, 60.0, deck, yaw=(90.0 if sign > 0 else -90.0), detail=0, arm_r='fwd', arm_l='fwd',
                 **P.look(r, s=2.5, top=WHITE, hat='peak', hat_col=RED))
        bubbles(sc, [(-sign * (1700.0 + k * 360.0), r.uniform(-150, 150), z0 + r.uniform(-100, 500)) for k in range(4)], r, **over)
    return build


# ------------------------------------------------------------------------------------------ the ridge
def ridge_climbers(seed=1):
    """For the horizon: a rock ridge 60 m wide and 52 m tall (it goes 15 m into the ground: the trench wall's top hides
    its foot), two roped teams of giants (4 times life size) on its two peaks, each on a ledge, hopping out of step with
    their arms up, two more on each top, and a flag on the summit that swings."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.side(ROCK, [(-3000, -1500), (-2500, 1500), (-1700, 2900), (-1100, 2500), (-200, 5200), (500, 3900), (1300, 4500), (2000, 2600), (2900, -1500)], 200, 800)
        g.side(ROCK_L, [(-3200, -1500), (-2400, 900), (-1500, 1900), (-600, 1500), (400, 2700), (1400, 1700), (2300, 1100), (3100, -1500)], -200, 200)
        y = 120.0
        looks = lambda k, s: P.look(r, s=s, top=SUITS[(k + seed) % len(SUITS)], bottom=('#2b2350', '#ff7a3c', '#fff6ea')[k % 3],
                                    hat=('helmet', 'beanie', 'helmet', 'cap')[k % 4], hat_col=(RED, YEL, WHITE, PINK)[k % 4])

        def team(a, b, n, k0):
            tilt_box(g, WHITE, (a[0], y - 40, a[1] + 350), (b[0], y - 40, b[1] + 350), 18, 18)
            for k in range(n):
                t = (k + 0.6) / float(n + 0.4)
                px, pz = a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t
                g.box(ORANGE, (px, y, pz - 20), (340, 260, 40))
                P.figure(sc, px, y, pz, yaw=r.uniform(-25, 25), detail=0, arm_r=('up', 'wave', 'vee')[k % 3], arm_l=('up', 'hip', 'vee')[k % 3],
                         swing_r=22.0, bob=26.0, rate=r.uniform(0.4, 0.6), phase=r.random(), **looks(k + k0, 4.0))

        def top(px, pz, k0):
            g.box(ROCK_L, (px, y + 60, pz - 60), (760, 420, 160))
            for j in (-1, 1):
                P.figure(sc, px + j * 190.0, y, pz + 20.0, yaw=j * 15.0, detail=0, arm_r='up', arm_l=('up' if j > 0 else 'wave'), swing_r=24.0, swing_l=24.0,
                         bob=30.0, rate=r.uniform(0.5, 0.7), phase=r.random(), **looks(k0 + j + 1, 4.5))

        team((-1100, 2500), (-200, 5200), 4, 0)
        top(-200.0, 5200.0, 4)
        team((500, 3900), (1300, 4500), 2, 6)
        top(1300.0, 4500.0, 8)
        pole = (-200.0, y + 150.0, 5220.0)
        g.box(WHITE, (pole[0], pole[1], pole[2] + 600), (24, 24, 1200))
        fl = sc.part(P.motion(rate=0.45, swing=14.0, pivot=(pole[0], pole[1], pole[2] + 1000), axis=P.UP))
        fl.box(PINK, (pole[0] + 300, pole[1], pole[2] + 1020), (600, 18, 340))
        fl.box(WHITE, (pole[0] + 300, pole[1] - 12, pole[2] + 1020), (600, 8, 90))
    return build


# ------------------------------------------------------------------------------------------ the diving birds
def diving_birds(seed=1):
    """A flock of nine great sea birds "flying" under water after the shoals: 4.5 to 6 m from wing tip to wing tip,
    white and pink with coral and yellow wings, wheeling about the middle of the piece at three rates, 28 to 56 m up,
    wings beating."""
    def build(sc):
        r = random.Random(seed)
        cols = (('#fff6ea', ORANGE, YEL), ('#ffd0dc', RED, YEL), ('#fff6ea', PINK, ORANGE), ('#fff1c0', ORANGE, RED))
        for k in range(9):
            rad = 900.0 + (k % 3) * 750.0 + r.uniform(-150, 150)
            a = math.radians(k * 40.0 + r.uniform(-12, 12))
            body, wing, beak = cols[(k + seed) % len(cols)]
            P.bird(sc, math.cos(a) * rad, math.sin(a) * rad, 2800.0 + ((k * 5) % 9) * 350.0, heading=math.degrees(a) + 90.0, span=r.uniform(450, 600),
                   body=body, wing=wing, beak=beak, phase=r.random(), rate=r.uniform(0.9, 1.3), flap=30.0, orbit=(0.0, 0.0, (0.022, 0.018, 0.026)[k % 3]))
    return build


# ------------------------------------------------------------------------------------------ against the haze
NO_GLOW = (DARK, VISOR, STEEL, DECK, DECK_D, BUBBLE, ROCK, ROCK_L)


def lit(build, glow=0.15):
    """The level's haze takes the colour out of everything past 150 m (the first far kit learned it in frames). Every
    coloured part of a far piece therefore carries a little steady light of its own colour (a constant: nothing
    blinks); the dark, the steel and the rock stay as they are."""
    def f(sc):
        build(sc)
        for key in sc.order:
            model = sc.parts[key][0]
            out = {}
            for (col, g), pm in model.meshes.items():
                k2 = (col, g if (col in NO_GLOW or g >= glow) else glow)
                if k2 in out:
                    k2 = (col, g)
                out[k2] = pm
            model.meshes = out
    return f


WHEEL = dict(far=True, grow=(2900.0, 2900.0, 150.0), grow_down=(2900.0, 2900.0, 100.0))
RUN = dict(far=True, grow=(4900.0, 150.0, 250.0), grow_down=(4900.0, 150.0, 250.0))

JOBS = [
    ('SM_P3E_DiveLine_A', lit(dive_line(1, 1, 8, False)), dict(far=True, grow=(150.0, 150.0, 150.0), grow_down=(150.0, 150.0, 0.0))),
    ('SM_P3E_DiveLine_B', lit(dive_line(2, -1, 7, True)), dict(far=True, grow=(150.0, 150.0, 150.0), grow_down=(150.0, 150.0, 0.0))),
    ('SM_P3E_Kites', lit(kites(1)), dict(far=True, grow=(1000.0, 200.0, 300.0), grow_down=(1000.0, 200.0, 0.0))),
    ('SM_P3E_Balloons_A', lit(balloons(1)), WHEEL),
    ('SM_P3E_Balloons_B', lit(bunches(1)), WHEEL),
    ('SM_P3E_Gliders', lit(gliders(1)), WHEEL),
    ('SM_P3E_StarRing', lit(star_ring(1)), dict(far=True, grow=(900.0, 100.0, 900.0), grow_down=(900.0, 100.0, 900.0))),
    ('SM_P3E_FishHerd', lit(fish_herd(1)), RUN),
    ('SM_P3E_WaterBus', lit(water_bus(1)), RUN),
    ('SM_P3E_RidgeClimbers', lit(ridge_climbers(1)), dict(far=True, grow=(200.0, 200.0, 200.0), grow_down=(200.0, 200.0, 0.0))),
    ('SM_P3E_DivingBirds', lit(diving_birds(1)), WHEEL),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
