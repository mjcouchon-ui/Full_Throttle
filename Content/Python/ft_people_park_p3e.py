"""FULL THROTTLE (Unreal) the THEME PARK, population pass 3, builder p3e: FAR AND SKY (level index 4).
Meshes /Game/People/ThemePark/SM_P3E_...; the placing rules are the block C:/ftwork/p3/L4e.inl.

Everything here stands 150 m and more from the road, or flies over it at that distance. The park is densely built and
a night level, so every piece is drawn BIG (figures 4 m and more: the table doubles them again), HIGH (on a mound, a
trestle, a beam, a spire, a cable, or in the air), in hot colour with a small steady glow, and every piece has a deep
foot because the lagoon's land falls away under a Far piece. All are saved with the always-moving master (far=True).

    SM_P3E_Kites_A     a kite hill: four flyers on a mound, two diamond kites, a box kite and a delta 25 to 40 m up,
                       each swaying on its line out of step with the others.
    SM_P3E_Kites_B     a dragon kite (a head and eleven discs rippling up the sky), a pole of three carp windsocks,
                       a diamond, children hopping under them.
    SM_P3E_Parrots     a flock of twelve great macaws (scarlet, blue-and-gold, green) wheeling round a point.
    SM_P3E_Flamingos   a skein of nine flamingos, necks out and legs trailing, flying four slots' length and round again.
    SM_P3E_Train       the park's little railway on a trestle 8 m up: an engine with puffs of steam and four open
                       coaches of waving passengers, running the trestle's 90 m and coming round again.
    SM_P3E_Monorail    a monorail on pylons 14 m up: a four-car train going the other way, heads at its lit windows,
                       a station deck of people waving it in.
    SM_P3E_Zipline     a sky glide between two striped towers: four riders hanging from the cable 20 m up and
                       travelling its length, people on both platforms waving them off and in.
    SM_P3E_ClimbSpire  a climbing rock 30 m high: six climbers on ropes working up its face, one on the summit with
                       a flag, belayers and friends hopping at its foot.
    SM_P3E_SkyTeam     a parachute display team: five jumpers under square striped canopies, spiralling slowly
                       round each other, each trailing a streamer.
    SM_P3E_PicnicHill  a lawn hill with three terraces of people out to watch the chase, hopping out of step;
                       flag wavers and a pavilion on top.
    SM_P3E_WaveStand   a far stand, fifty people in five rows doing a wave: the hop runs along the stand, column
                       after column; a banner waver at each end.

Rules kept: nothing nearer than 150 m in the table (every piece is TALL); a steady glow only, nothing blinks; no clown
(the boss is MR. CHUCKLES) and no balloons (he bombs the road with them); no words, no marks; movement only through
the figure builder's motion values.

Run in the editor:  import ft_people_park_p3e as K; K.run()      (or K.run(['SM_P3E_Parrots']))
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/ThemePark'
NAVY, PURPLE, PLUM = '#2b2350', '#3a2f6a', '#4a3a78'
MAG, CYAN, AMBER, CREAM = '#ff4fb8', '#2ee6ff', '#ffd23f', '#fff1d6'
RED, WHITE, LAV, MINT, TEAL = '#ff2d3c', '#fff6ea', '#b9a4ff', '#7fe3c4', '#2fc6c0'
ORANGE, GOLD, WOOD, GREY_L, DARK = '#ff8a2a', '#ffc93c', '#9a6a44', '#9a98a8', '#22202a'
LIME, VIOLET, PINE, BLUE = '#b8ff5a', '#8f6bff', '#2f8a5a', '#3a5fa8'
ROCK, ROCK2, PINK = '#c9a070', '#b87a50', '#ff8fb0'
BRIGHT = (MAG, CYAN, AMBER, RED, ORANGE, VIOLET, LIME, WHITE)
LAMP = '#ffe28a'
FOOT = 1500.0      # how far a piece's foot goes down under z = 0 (cm as built)


# ------------------------------------------------------------------------------------------ helpers
def lift(sc, g):
    """A night level: every colour of a piece gets a small steady glow (g), like a figure under stage light.
    Allowed because every piece of this kit stands far beyond 24 m; nothing blinks."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.09):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def tv_of(travel):
    return (float(travel[0]), float(travel[1]) / float(travel[0]))


def raw(sc, mo=None):
    """A part drawn in the mesh's own space."""
    m = sc.part(mo)
    m.base = None
    return m


def bulbs(m, a, b, n, size=30.0, glow=1.3, col=LAMP):
    for k in range(n):
        f = (k + 0.5) / n
        m.box(col, (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f), (size, size, size), glow=glow)


def mound(g, rad, h, top_r, col=PINE, steps=10):
    """A lawn mound with a deep foot. Returns its top's z."""
    g.cyl(col, (0, 0, 0), rad, h, steps=steps, top_r=top_r)
    g.cyl(col, (0, 0, -FOOT), rad, FOOT, steps=steps)
    return h


def blocky(m, r, x, y, z, s=1.0, yaw=0.0, legs=True, arms=False, top=None):
    """A far person as blocks of colour (legs, a bright top, a head; arms=True: both arms up as one bar over the
    head). 1 = 1.9 m. Drawn into m, which carries the movement."""
    top = top or r.choice(BRIGHT)
    rot = (0, yaw, 0)
    if legs:
        m.box(r.choice((NAVY, BLUE, CREAM, '#c94f6a')), (x, y, z + 40 * s), (44 * s, 36 * s, 80 * s), rot=rot)
    m.box(top, (x, y, z + 114 * s), (62 * s, 40 * s, 72 * s), rot=rot)
    m.box(r.choice(P.SKINS), (x, y, z + 172 * s), (42 * s, 40 * s, 46 * s), rot=rot)
    if arms:
        m.box(top, (x, y, z + 208 * s), (120 * s, 16 * s, 16 * s), rot=rot)


def hold_bigflag(col, col2=WHITE, pole=CREAM, h=230.0, w=120.0):
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + h / 2.0 - 30), (6, 6, h + 40))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + h - 28), (w, 5, 56))
        m.box(col2, (hd[0] + side * w / 2.0, hd[1], hd[2] + h - 76), (w, 5, 40))
    return f


# ------------------------------------------------------------------------------------------ kites
def _line(m, a, b, w=9.0):
    tilt_box(m, CREAM, a, b, w, w)


def kite_diamond(sc, hand, k, col, col2, phase, size=320.0, rate=0.24):
    """A diamond kite at k = (x, y, z), swaying on its line about the flyer's hand."""
    m = raw(sc, P.motion(phase=phase, rate=rate, bob=-50.0, swing=7.0, pivot=hand, axis=90.0))
    kx, ky, kz = k
    m.box(col, (kx, ky, kz), (size, 16, size), rot=(45, 0, 0))
    m.box(col2, (kx, ky - 10, kz), (22, 8, size * 1.36))
    m.box(col2, (kx, ky - 10, kz), (size * 1.36, 8, 22))
    for j in range(4):          # the tail: bows on a string
        m.box(col2 if j % 2 else col, (kx + (j % 2) * 40 - 20, ky, kz - size * 0.8 - j * 110), (90, 12, 46))
    _line(m, hand, (kx, ky, kz - size * 0.6))


def kite_box(sc, hand, k, col, col2, phase):
    m = raw(sc, P.motion(phase=phase, rate=0.2, bob=-60.0, swing=6.0, pivot=hand, axis=90.0))
    kx, ky, kz = k
    m.box(col, (kx, ky, kz + 170), (260, 260, 150))
    m.box(col2, (kx, ky, kz - 170), (260, 260, 150))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(CREAM, (kx + sx * 120, ky + sy * 120, kz), (14, 14, 500))
    _line(m, hand, (kx, ky, kz - 250))


def kite_delta(sc, hand, k, col, col2, phase):
    m = raw(sc, P.motion(phase=phase, rate=0.3, bob=-70.0, swing=9.0, pivot=hand, axis=90.0))
    kx, ky, kz = k
    for sx in (-1, 1):
        m.box(col, (kx + sx * 190, ky, kz + 40), (420, 16, 230), rot=(sx * 22, 0, 0))
        m.box(col2, (kx + sx * 330, ky - 6, kz + 100), (150, 10, 90), rot=(sx * 22, 0, 0))
    m.box(col2, (kx, ky - 8, kz - 10), (40, 12, 330))
    for sx in (-1, 1):          # two streamers
        m.box(col2, (kx + sx * 60, ky, kz - 330), (30, 10, 380))
    _line(m, hand, (kx, ky, kz - 170))


def kites_a(sc):
    """A kite hill: four flyers on a mound, two diamond kites, a box kite and a delta 25 to 40 m up, each swaying
    on its line out of step with the others; two friends pointing up."""
    r = random.Random(4501)
    g = sc.still()
    T = mound(g, 1100, 420, 760)
    g.cyl(CREAM, (0, 0, T - 4), 700, 10, steps=10)
    spots = ((-520, MAG, CYAN), (-170, AMBER, RED), (190, LIME, VIOLET), (540, CYAN, WHITE))
    kites = ((-1500, 500, 3300), (-520, 300, 4100), (700, 400, 2700), (1700, 600, 3700))
    S = 2.4
    for i, ((x, c1, c2), k) in enumerate(zip(spots, kites)):
        top = P.figure(sc, x, -80 + (i % 2) * 120, T, yaw=r.uniform(-20, 20), detail=0, arm_r='up', arm_l=('wave' if i % 2 else 'down'),
                       swing_r=10, axis_r='y', rate=0.5, bob=6.0, phase=r.random(), **P.look(r, top=c1, s=S))
        hand = (x + 40, -80 + (i % 2) * 120, top + 60)
        ph = r.random()
        if i == 1:
            kite_box(sc, hand, k, c1, c2, ph)
        elif i == 3:
            kite_delta(sc, hand, k, c1, c2, ph)
        else:
            kite_diamond(sc, hand, k, c1, c2, ph)
    for x in (-800, 820):
        P.figure(sc, x, -260, T - 60, yaw=(-30 if x < 0 else 30), detail=0, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.4, bob=26.0,
                 phase=r.random(), **P.look(r, top=r.choice(BRIGHT), build='kid', s=1.7))


def kites_b(sc):
    """A dragon kite: a head and eleven discs that ripple up the sky from the flyer's hands; beside it a pole of
    three carp windsocks swinging in the wind, a diamond kite, and three children hopping under them."""
    r = random.Random(4507)
    g = sc.still()
    T = mound(g, 1100, 420, 760)
    g.cyl(CREAM, (0, 0, T - 4), 700, 10, steps=10)
    S = 2.4
    top = P.figure(sc, -420, 0, T, yaw=-15, detail=0, arm_l='up', arm_r='up', swing_l=8, swing_r=-8, rate=0.5, bob=6.0,
                   **P.look(r, top=RED, bottom=NAVY, hat='cap', hat_col=AMBER, s=S))
    hand = (-420, 0, top + 60)
    cols = (RED, AMBER, ORANGE, AMBER)
    n = 12
    x0, z0, dx, dz = -150.0, 1500.0, 185.0, 215.0
    _line(g, hand, (x0, 300, z0))
    for k in range(n):
        x, z = x0 + k * dx, z0 + k * dz
        m = raw(sc, P.motion(phase=1.0 - k * 0.09, rate=0.5, bob=-95.0, pivot=(x, 300, z)))
        if k == n - 1:      # the head, highest: a great disc with eyes, horns and a beard
            m.cyl(LIME, (x, 300, z), 230, 30, rot=(0, 0, 90), steps=8)
            for sx in (-1, 1):
                m.box(WHITE, (x + sx * 90, 280, z + 60), (80, 12, 80))
                m.box(DARK, (x + sx * 90, 272, z + 60), (36, 8, 40))
                tilt_box(m, MAG, (x + sx * 120, 300, z + 180), (x + sx * 220, 300, z + 380), 40, 20)
            m.box(MAG, (x, 280, z - 110), (150, 12, 60))
        else:
            rad = 110.0 + k * 7.0
            m.cyl(cols[k % 4], (x, 300, z), rad, 20, rot=(0, 0, 90), steps=8)
            for sx in (-1, 1):  # a feather each side
                m.box(MAG if k % 2 else CYAN, (x + sx * (rad + 50), 300, z), (100, 10, 28))
    # the carp pole
    px = 620.0
    g.box(CREAM, (px, 80, T + 800), (24, 24, 1600))
    g.sphere(GOLD, (px, 80, T + 1620), 40, steps=(4, 6))
    for k, (c1, c2) in enumerate(((DARK, WHITE), (RED, WHITE), (CYAN, CREAM))):
        z = T + 1480 - k * 330
        ln = 560.0 - k * 90.0
        m = raw(sc, P.motion(phase=k * 0.23, rate=0.45, swing=16.0, pivot=(px, 80, z), axis=P.UP))
        m.cyl(c1, (px + 20, 80, z), 105 - k * 12, ln, rot=(-90, 0, 0), steps=6, top_r=46)
        m.box(c2, (px + 60, 80, z), (70, 220 - k * 24, 220 - k * 24))
        m.box(c1, (px + ln + 40, 80, z), (110, 14, 190))
    P.figure(sc, px - 140, -60, T, yaw=10, detail=0, arm_r='up', swing_r=12, rate=0.7, bob=6.0, **P.look(r, top=CYAN, s=S))
    hand2 = (px - 100, -60, T + S * 230)
    kite_diamond(sc, hand2, (1750, 500, 3000), VIOLET, AMBER, 0.4)
    for k, x in enumerate((-820, -80, 300)):
        P.figure(sc, x, -280, T - 70, yaw=r.uniform(-25, 25), detail=0, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.5, bob=28.0,
                 phase=r.random(), **P.look(r, top=BRIGHT[k + 2], build='kid', s=1.7))


# ------------------------------------------------------------------------------------------ birds
def macaw(sc, x, y, z, heading, span, cols, phase, rate=2.2, orbit=None, flap=30.0, glow=0.4):
    """A great macaw in fat boxes: body, head, a hooked beak, a long tail, two wings that beat."""
    body, wing, tail, face = cols
    base, tv = ((orbit[0], orbit[1]), (-1.0, float(orbit[2]))) if orbit else ((x, y), (0.0, 0.0))
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    L = span * 0.5
    rot = (0, heading, 0)
    m = raw(sc, P.motion(pivot=(x, y, z), phase=phase, rate=rate, base=base, travel=tv))
    m.box(body, (x, y, z), (L, L * 0.5, L * 0.52), rot=rot, glow=glow)
    m.box(body, (x + tx * L * 0.66, y + ty * L * 0.66, z + L * 0.14), (L * 0.44, L * 0.44, L * 0.46), rot=rot, glow=glow)
    m.box(face, (x + tx * L * 0.8, y + ty * L * 0.8, z + L * 0.1), (L * 0.2, L * 0.46, L * 0.24), rot=rot, glow=glow)
    m.box(DARK, (x + tx * L * 0.98, y + ty * L * 0.98, z + L * 0.02), (L * 0.22, L * 0.22, L * 0.3), rot=rot)
    m.box(tail, (x - tx * L * 1.05, y - ty * L * 1.05, z - L * 0.04), (L * 1.2, L * 0.22, L * 0.14), rot=rot, glow=glow)
    for side in (-1, 1):
        w = raw(sc, P.motion(swing=side * flap, pivot=(x, y, z + L * 0.1), axis=heading, phase=phase, rate=rate, base=base, travel=tv))
        w.box(wing, (x - ty * side * span * 0.3, y + tx * side * span * 0.3, z + L * 0.12), (L * 0.62, span * 0.46, L * 0.2), rot=rot, glow=glow)


def parrots(sc):
    """A flock of twelve great macaws (each 3.6 m across so that it reads from the road): scarlet, blue-and-gold
    and green, wheeling round a point at 15 to 28 m, out of step, some one way and some the other."""
    r = random.Random(4513)
    kinds = ((RED, BLUE, RED, WHITE), (BLUE, BLUE, CYAN, AMBER), (LIME, RED, LIME, WHITE), (AMBER, BLUE, AMBER, WHITE))
    for k in range(12):
        a = k * 2.0 * math.pi / 12.0 + r.uniform(-0.2, 0.2)
        ring = k % 3
        rad = 700.0 + ring * 600.0 + r.uniform(-80, 80)
        way = 1.0 if ring != 1 else -1.0
        rps = way * (0.085 - ring * 0.015)
        macaw(sc, math.cos(a) * rad, math.sin(a) * rad, 1500.0 + ring * 450.0 + r.uniform(-150, 250), math.degrees(a) + 90.0 * way,
              r.uniform(330, 390), kinds[k % 4], r.random(), rate=r.uniform(2.0, 2.6), orbit=(0, 0, rps))


def flamingo(sc, x, y, z, span, phase, rate, tv, glow=0.4):
    """A flamingo flying along +X: neck stretched out ahead, legs trailing, black-tipped wings."""
    L = span * 0.5
    m = raw(sc, P.motion(pivot=(x, y, z), phase=phase, rate=rate, base=(x, y), travel=tv))
    m.box(PINK, (x, y, z), (L * 0.9, L * 0.44, L * 0.46), glow=glow)
    tilt_box(m, PINK, (x + L * 0.4, y, z + L * 0.05), (x + L * 1.35, y, z + L * 0.2), L * 0.16, L * 0.16)
    m.box(MAG, (x + L * 1.46, y, z + L * 0.2), (L * 0.28, L * 0.24, L * 0.24), glow=glow)
    m.box(DARK, (x + L * 1.66, y, z + L * 0.12), (L * 0.2, L * 0.14, L * 0.16))
    tilt_box(m, MAG, (x - L * 0.4, y, z - L * 0.05), (x - L * 1.5, y, z - L * 0.12), L * 0.1, L * 0.2)
    for side in (-1, 1):
        w = raw(sc, P.motion(swing=side * 30.0, pivot=(x, y, z + L * 0.1), axis=0.0, phase=phase, rate=rate, base=(x, y), travel=tv))
        w.box(PINK, (x, y + side * span * 0.2, z + L * 0.1), (L * 0.56, span * 0.3, L * 0.16), glow=glow)
        w.box(DARK, (x - L * 0.06, y + side * span * 0.42, z + L * 0.1), (L * 0.44, span * 0.16, L * 0.14))


def flamingos(sc):
    """A skein of nine flamingos (4 m across), necks out and legs trailing, in a slanting line 19 to 23 m up,
    flying four slots' length along the road and round again."""
    r = random.Random(4519)
    L, V = 18000.0, 1100.0
    tv = tv_of((L, V))
    for k in range(9):
        x = 1500.0 - k * 380.0
        y = (k - 4) * 260.0 + r.uniform(-60, 60)
        z = 2100.0 - abs(k - 3) * 55.0 + r.uniform(-40, 40)
        flamingo(sc, x, y, z, r.uniform(380, 430), r.random(), r.uniform(1.7, 2.1), tv)


# ------------------------------------------------------------------------------------------ far traffic
def trestle(g, L, top, cols=(MAG, CREAM), deep=260.0, piers=7, lamps=14):
    """A deck on piers with a striped fascia and a row of steady lamps; the piers go FOOT down."""
    g.box(PLUM, (0, 0, top - 30), (L, deep, 60))
    n = 12
    w = L / n
    for k in range(n):
        g.box(cols[k % 2], (-L / 2.0 + (k + 0.5) * w, -deep / 2.0 - 4, top - 60), (w, 10, 120))
    for k in range(piers):
        x = -L / 2.0 + (k + 0.5) * L / piers
        g.box(NAVY, (x, 0, (top - 60 - FOOT) / 2.0), (150, deep - 60, top - 60 + FOOT))
    bulbs(g, (-L / 2.0, -deep / 2.0 - 14, top - 140), (L / 2.0, -deep / 2.0 - 14, top - 140), lamps)


def train(sc):
    """The park's little railway on a trestle 8 m up: a red engine with a gold funnel and three puffs of steam, and
    four open coaches under striped roofs, two passengers in each waving at the chase; it runs the trestle's 90 m
    and comes round again."""
    r = random.Random(4523)
    L, top, V = 9000.0, 800.0, 900.0
    g = sc.still()
    trestle(g, L, top)
    tv = tv_of((L, V))

    def car(x):
        return raw(sc, P.motion(pivot=(x, 0, top), base=(x, 0.0), travel=tv))
    ex = 1500.0
    m = car(ex)
    m.box(DARK, (ex, 0, top + 40), (620, 190, 80))
    m.cyl(RED, (ex - 60, 0, top + 190), 115, 420, rot=(-90, 0, 0), steps=8)
    m.box(RED, (ex - 200, 0, top + 250), (240, 210, 340))
    m.box(GOLD, (ex - 200, 0, top + 440), (290, 250, 40))
    m.box(CYAN, (ex - 200, -108, top + 310), (130, 8, 110), glow=0.5)
    m.cyl(GOLD, (ex + 230, 0, top + 290), 46, 190, steps=6, top_r=70)
    m.box(GOLD, (ex + 60, 0, top + 310), (70, 70, 60))
    m.box(AMBER, (ex + 390, 0, top + 70), (60, 200, 110))
    m.sphere(LAMP, (ex + 370, 0, top + 200), 44, steps=(4, 6), glow=1.4)
    for k in range(3):          # steam: each puff rises and falls on its own, trailing behind the funnel
        px = ex + 230 - k * 190.0
        s = raw(sc, P.motion(phase=k * 0.3, rate=0.7, bob=-50.0, pivot=(px, 0, top + 600 + k * 110), base=(px, 0.0), travel=tv))
        s.sphere(WHITE, (px, 0, top + 600 + k * 110), 90 + k * 30, steps=(4, 6))
    roofs = (CYAN, AMBER, MAG, LIME)
    for c in range(4):
        cx = ex - 760.0 - c * 640.0
        m = car(cx)
        m.box(DARK, (cx, 0, top + 30), (560, 180, 60))
        m.box(roofs[c], (cx, -96, top + 130), (560, 12, 150))
        m.box(CREAM, (cx, -103, top + 130), (560, 6, 40))
        for sx in (-1, 1):
            m.box(CREAM, (cx + sx * 260, 0, top + 300), (20, 180, 420))
        m.box(roofs[(c + 2) % 4], (cx, 0, top + 520), (600, 220, 30))
        for j in (-1, 1):
            P.figure(sc, cx + j * 130, 0, top + 60, yaw=r.uniform(-20, 20), detail=0, arm_r='wave', arm_l=('up' if (c + j) % 3 == 0 else 'down'),
                     swing_r=26, rate=1.3, bob=10.0, phase=r.random(), travel=(L, V), **P.look(r, top=r.choice(BRIGHT), s=2.0))


def monorail(sc):
    """A monorail on pylons 14 m up: a four-car train in cream and magenta running towards -X and coming round
    again, heads at its lit windows; a station deck in the middle with five people waving it in."""
    r = random.Random(4529)
    L, top, V = 9000.0, 1400.0, -1500.0
    g = sc.still()
    g.box(CREAM, (0, 0, top - 40), (L, 70, 80))
    g.box(CYAN, (0, -38, top - 40), (L, 6, 24), glow=0.4)
    for k in range(7):
        x = -L / 2.0 + (k + 0.5) * L / 7.0
        g.box(PURPLE, (x, 0, (top - 80 - FOOT) / 2.0), (100, 100, top - 80 + FOOT))
        g.box(PURPLE, (x, 0, top - 110), (140, 260, 60))
    tv = tv_of((L, V))
    for c in range(4):
        cx = -900.0 + c * 600.0
        m = raw(sc, P.motion(pivot=(cx, 0, top), base=(cx, 0.0), travel=tv))
        m.box(CREAM, (cx, 0, top + 110), (560, 170, 220))
        m.box(MAG, (cx, -88, top + 40), (560, 8, 60))
        m.box(CYAN, (cx, -88, top + 150), (480, 8, 90), glow=0.5)
        m.box(NAVY, (cx, 0, top - 40), (420, 130, 100))
        if c == 0:
            m.box(MAG, (cx - 330, 0, top + 80), (110, 150, 150))
            m.sphere(LAMP, (cx - 390, 0, top + 80), 36, steps=(4, 6), glow=1.4)
        if c == 3:
            m.box(MAG, (cx + 330, 0, top + 80), (110, 150, 150))
        for j in range(3):
            hx = cx - 160 + j * 160
            m.box(r.choice(P.SKINS), (hx, -92, top + 150), (50, 10, 54))
            m.box(r.choice(P.HAIRS), (hx, -93, top + 182), (54, 10, 20))
    # the station
    dz = top - 70
    g.box(PLUM, (0, -330, dz - 20), (1500, 420, 40))
    g.box(AMBER, (0, -544, dz + 50), (1500, 12, 110))
    g.box(NAVY, (0, -330, (dz - 40 - FOOT) / 2.0), (260, 260, dz - 40 + FOOT))
    for sx in (-1, 1):
        g.box(CREAM, (sx * 700, -160, dz + 330), (24, 24, 660))
    g.box(MAG, (0, -250, dz + 680), (1600, 520, 36))
    bulbs(g, (-760, -520, dz + 640), (760, -520, dz + 640), 8)
    for k in range(5):
        P.figure(sc, -560 + k * 280, -400, dz, yaw=r.uniform(-15, 15), detail=0, arm_r='wave', arm_l=('up' if k % 2 else 'down'), swing_r=24,
                 swing_l=(18 if k % 2 else 0), rate=1.2, bob=14.0, phase=r.random(), **P.look(r, top=BRIGHT[k], s=2.2))


def zipline(sc):
    """A sky glide: two striped towers 66 m apart, a cable 20 m up, four riders hanging from it with their legs
    kicking, travelling its length and coming round again; two people on each platform waving them off and in."""
    r = random.Random(4531)
    L, H, V = 6600.0, 2050.0, 800.0
    g = sc.still()
    for sx in (-1, 1):
        x = sx * (L / 2.0 + 200)
        for k in range(5):
            g.box((MAG, CREAM)[k % 2] if sx < 0 else (CYAN, CREAM)[k % 2], (x, 0, k * 420 + 210), (340, 340, 420))
        g.box(NAVY, (x, 0, -FOOT / 2.0), (360, 360, FOOT))
        g.box(PLUM, (x, -60, 1680), (760, 620, 40))
        g.box(AMBER, (x, -374, 1750), (760, 12, 110))
        g.cyl(RED, (x, 0, 2100), 420, 300, steps=8, top_r=30)
        g.sphere(LAMP, (x, 0, 2430), 44, steps=(4, 6), glow=1.4)
        bulbs(g, (x - 340, -384, 1640), (x + 340, -384, 1640), 4)
        for j in (-1, 1):
            P.figure(sc, x + j * 200, -250, 1700, yaw=r.uniform(-15, 15), detail=0, arm_r='wave', arm_l=('up' if j > 0 else 'down'), swing_r=24,
                     rate=1.2, bob=12.0, phase=r.random(), **P.look(r, top=r.choice(BRIGHT), s=2.0))
    g.box(CREAM, (0, -420, H), (L + 400, 12, 12))
    S = 2.3
    tv = tv_of((L, V))
    for k in range(4):
        x = -L / 2.0 + (k + 0.5) * L / 4.0
        fz = H - 150 - S * 175
        P.figure(sc, x, -420, fz, yaw=90, detail=0, arm_l='up', arm_r='up', legs='stride', stride=22, rate=0.9, phase=r.random(),
                 travel=(L, V), **P.look(r, top=BRIGHT[k * 2 % 8], hat='helmet', hat_col=(AMBER, RED, CYAN, LIME)[k], s=S))
        m = raw(sc, P.motion(pivot=(x, -420, H), base=(x, -420.0), travel=tv))
        m.box(DARK, (x, -420, H - 20), (90, 30, 60))
        m.box(AMBER, (x, -420, H - 100), (16, 16, 130))
        m.box(AMBER, (x, -420, H - 160), (150, 16, 16))


def climb_spire(sc):
    """A climbing rock 30 m high, lit holds all over its face: six climbers on ropes working up it out of step,
    one on the summit waving a flag, two belayers and three friends hopping at its foot."""
    r = random.Random(4537)
    g = sc.still()
    radii = (700.0, 600.0, 480.0, 380.0, 280.0)
    SH = 600.0
    for k, rad in enumerate(radii):
        g.cyl(ROCK if k % 2 == 0 else ROCK2, (0, 0, k * SH), rad, SH, steps=7, top_r=rad - 70)
    g.cyl(ROCK2, (0, 0, -FOOT), 720, FOOT, steps=7)
    TOP = len(radii) * SH
    g.cyl(CREAM, (0, 0, TOP), 230, 20, steps=7)
    for k in range(16):         # the holds
        st = k % 5
        a = math.radians(-90 + r.uniform(-75, 75))
        rad = radii[st] - 30
        g.box(BRIGHT[k % 8], (math.cos(a) * rad, math.sin(a) * rad, st * SH + r.uniform(80, 520)), (56, 56, 56), glow=0.5)
    S = 2.3
    spots = ((0, -128, 260), (0, -62, 420), (1, -100, 200), (2, -70, 120), (2, -112, 420), (3, -86, 230))
    for i, (st, deg, dz) in enumerate(spots):
        a = math.radians(deg)
        rad = radii[st] - (dz / SH) * 70.0 + 40.0
        x, y, z = math.cos(a) * rad, math.sin(a) * rad, st * SH + dz
        P.figure(sc, x, y, z, yaw=deg - 90.0, detail=0, arm_l='up', arm_r='vee', swing_l=18, swing_r=-18, axis_l='y', axis_r='y', legs='wide',
                 rate=0.35, bob=-45.0, phase=i * 0.17, **P.look(r, top=BRIGHT[i], bottom=NAVY, hat='helmet', hat_col=BRIGHT[(i + 3) % 8], pack=AMBER, s=S))
        tilt_box(g, CREAM, (x * 1.04, y * 1.04, z + S * 150), (math.cos(a) * 240, math.sin(a) * 240, TOP), 8, 8)
    P.figure(sc, 0, -40, TOP + 20, detail=0, arm_r='up', arm_l='hip', swing_r=24, axis_r='y', rate=0.8, bob=8.0,
             hold_r=hold_bigflag(MAG, AMBER, h=260.0, w=150.0), **P.look(r, top=WHITE, bottom=RED, hat='helmet', hat_col=RED, s=S))
    for k, x in enumerate((-520, 480)):
        P.figure(sc, x, -900, 0, yaw=(160 if x < 0 else -160), detail=0, arm_l='hold', arm_r='hold', rate=0.5, bob=4.0, phase=k * 0.4,
                 **P.look(r, top=ORANGE, bottom=NAVY, hat='helmet', hat_col=WHITE, s=S))
    g.cyl(CREAM, (0, -900, -6), 900, 12, steps=8)
    for k, x in enumerate((-180, 60, 250)):
        P.figure(sc, x, -1120, 0, yaw=180 + r.uniform(-20, 20), detail=0, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.4, bob=24.0,
                 phase=r.random(), **P.look(r, top=BRIGHT[k + 1], s=2.0))


# ------------------------------------------------------------------------------------------ the display team
def sky_team(sc):
    """A parachute display team: five jumpers under square striped canopies at 12 to 40 m, spiralling slowly round
    each other and rising and falling a little on the air, a long streamer from each one's heels."""
    r = random.Random(4541)
    S = 2.5
    pairs = ((MAG, WHITE), (CYAN, AMBER), (AMBER, RED), (LIME, VIOLET), (RED, WHITE))
    for k in range(5):
        a = k * 2.0 * math.pi / 5.0 + r.uniform(-0.2, 0.2)
        rad = 500.0 + (k % 3) * 520.0
        rps = 0.03 + (k % 3) * 0.006
        x, y = math.cos(a) * rad, math.sin(a) * rad
        z = 1200.0 + k * 620.0
        yaw = math.degrees(a) + 180.0     # facing along the circle (-Y turned by yaw = the tangent)
        ph = r.random()
        mo = dict(phase=ph, rate=0.1, bob=-140.0)
        top = P.figure(sc, x, y, z, yaw=yaw, detail=0, arm_l='vee', arm_r='vee', legs='wide', orbit=(0, 0, rps),
                       **dict(P.look(r, top=pairs[k][0], bottom=NAVY, hat='helmet', hat_col=pairs[k][1]), s=S, **mo))
        m = sc.part(P.motion(pivot=(x, y, z), base=(0.0, 0.0), travel=(-1.0, rps), **mo), (x, y, z), yaw, 1.0)
        cz = (top - z) + 620.0
        c1, c2 = pairs[k]
        for j in range(5):      # the canopy: five cells across, two colours
            m.box(c1 if j % 2 == 0 else c2, (-360 + j * 180, 0, cz - abs(j - 2) * 26), (180, 330, 56))
        for sx in (-1, 1):
            for sy in (-1, 1):
                tilt_box(m, CREAM, (sx * 60, 0, (top - z) - 60), (sx * 400, sy * 140, cz - 60), 8, 8)
        m.box(c1, (0, 330, 40), (26, 560, 14))      # the streamer, trailing behind the heels


# ------------------------------------------------------------------------------------------ far crowds
def picnic_hill(sc):
    """A lawn hill 9 m high with three terraces of people out to watch the chase (blocks of colour 4 m tall, hopping
    out of step, some with their arms up), four flag wavers and a striped pavilion on top, steady lamps on each terrace."""
    r = random.Random(4547)
    g = sc.still()
    R0, R1, H = 2000.0, 900.0, 900.0
    mound(g, R0, H, R1, steps=12)

    def rad_at(z):
        return R0 - (R0 - R1) * z / H
    S = 2.1
    for i, (z, n) in enumerate(((230.0, 8), (470.0, 7), (710.0, 5))):
        rr = rad_at(z) + 170.0
        g.cyl(CREAM if i % 2 == 0 else LAV, (0, 0, z - 30), rr, 30, steps=12)
        for k in range(n):
            deg = -90.0 + (k - (n - 1) / 2.0) * (118.0 / n) + r.uniform(-3, 3)
            a = math.radians(deg)
            x, y = math.cos(a) * (rr - 90), math.sin(a) * (rr - 90)
            m = raw(sc, P.motion(phase=round(r.random(), 2), rate=r.choice((1.1, 1.3, 1.5)), bob=r.choice((22.0, 30.0, 40.0)), pivot=(x, y, z)))
            blocky(m, r, x, y, z, s=S * r.uniform(0.9, 1.08), yaw=deg + 90.0, arms=(k % 3 == 0))
        for k in range(5):
            a = math.radians(-90.0 + (k - 2) * 26.0)
            g.box(LAMP, (math.cos(a) * (rr + 8), math.sin(a) * (rr + 8), z - 14), (36, 36, 36), glow=1.3)
    # the top: a pavilion and the flag wavers
    g.cyl(CREAM, (0, 0, H - 4), R1 - 40, 10, steps=12)
    for k in range(6):
        a = k * math.pi / 3.0
        g.box(CREAM, (math.cos(a) * 330, 200 + math.sin(a) * 330, H + 260), (26, 26, 520))
    g.cyl(MAG, (0, 200, H + 520), 460, 300, steps=8, top_r=30)
    g.cyl(AMBER, (0, 200, H + 490), 480, 40, steps=8)
    g.sphere(LAMP, (0, 200, H + 850), 46, steps=(4, 6), glow=1.4)
    flags = ((MAG, WHITE), (CYAN, AMBER), (AMBER, RED), (LIME, WHITE))
    for k, x in enumerate((-640, -280, 300, 660)):
        P.figure(sc, x, -520 + abs(x) * 0.25, H, yaw=(-15 if x < 0 else 15), detail=0, arm_r='up', arm_l='hip', swing_r=26, axis_r='y', rate=0.9,
                 bob=12.0, phase=r.random(), hold_r=hold_bigflag(flags[k][0], flags[k][1], h=300.0, w=170.0), **P.look(r, top=BRIGHT[k * 2], s=2.4))


def wave_stand(sc):
    """A far stand: five rows of ten people (blocks of colour 4 m tall) doing a wave: the hop runs along the stand
    column after column and comes round; a banner waver at each end; a striped fascia and a roof with steady lamps."""
    r = random.Random(4549)
    g = sc.still()
    W, rows, cols = 5400.0, 5, 10
    for j in range(rows):
        y = -500.0 + j * 230.0
        z = 320.0 + j * 250.0
        g.box(PLUM if j % 2 else PURPLE, (0, y + 115, (z - FOOT) / 2.0), (W, 230, z + FOOT))
        g.box(CYAN if j % 2 else MAG, (0, y - 2, z - 40), (W, 8, 80))
    S = 2.1
    for c in range(cols):
        x0 = -W / 2.0 + 300.0 + (c + 0.5) * (W - 600.0) / cols
        m = raw(sc, P.motion(phase=1.0 - c / float(cols), rate=0.4, bob=110.0, pivot=(x0, 0, 600)))
        for j in range(rows):
            x = x0 + r.uniform(-50, 50)
            blocky(m, r, x, -500.0 + j * 230.0 + 110.0, 320.0 + j * 250.0, s=S * r.uniform(0.9, 1.06), legs=(j == 0), arms=((c + j) % 2 == 0),
                   top=BRIGHT[(c * 3 + j * 5) % 8])
    zt = 320.0 + rows * 250.0
    for sx in (-1, 1):
        g.box(CREAM, (sx * (W / 2.0 - 40), 600, (zt + 700) / 2.0), (40, 40, zt + 700))
        P.figure(sc, sx * (W / 2.0 - 150), -500 + 110, 320, yaw=-sx * 15, detail=0, arm_r='up', arm_l='hip', swing_r=28, axis_r='y', rate=0.8, bob=10.0,
                 phase=0.5 * (sx > 0), hold_r=hold_bigflag((MAG, CYAN)[sx > 0], AMBER, h=320.0, w=190.0), **P.look(r, top=WHITE, bottom=RED, s=2.4))
    g.box(RED, (0, 420, zt + 700), (W + 200, 700, 40))
    for k in range(9):
        g.box(AMBER if k % 2 else CREAM, (-W / 2.0 + (k + 0.5) * W / 9.0, 66, zt + 650), (W / 9.0, 10, 90))
    bulbs(g, (-W / 2.0, 56, zt + 590), (W / 2.0, 56, zt + 590), 12, size=36)


AIR = dict(far=True)
JOBS = [
    ('SM_P3E_Kites_A', lit(kites_a, 0.12), dict(far=True, grow=(500.0, 100.0, 120.0), grow_down=(500.0, 100.0, 0.0))),
    ('SM_P3E_Kites_B', lit(kites_b, 0.12), dict(far=True, grow=(400.0, 300.0, 160.0), grow_down=(400.0, 300.0, 0.0))),
    ('SM_P3E_Parrots', lit(parrots, 0.12), dict(far=True, grow=(2400.0, 2400.0, 200.0), grow_down=(2400.0, 2400.0, 200.0))),
    ('SM_P3E_Flamingos', lit(flamingos, 0.12), dict(far=True, grow=(9000.0, 200.0, 250.0), grow_down=(9000.0, 200.0, 250.0))),
    ('SM_P3E_Train', lit(train), dict(far=True, grow=(200.0, 60.0, 120.0), grow_down=(200.0, 60.0, 0.0))),
    ('SM_P3E_Monorail', lit(monorail), dict(far=True, grow=(200.0, 60.0, 80.0), grow_down=(200.0, 60.0, 0.0))),
    ('SM_P3E_Zipline', lit(zipline), dict(far=True, grow=(200.0, 60.0, 80.0), grow_down=(200.0, 60.0, 0.0))),
    ('SM_P3E_ClimbSpire', lit(climb_spire), dict(far=True, grow=(60.0, 60.0, 120.0), grow_down=(60.0, 60.0, 0.0))),
    ('SM_P3E_SkyTeam', lit(sky_team, 0.12), dict(far=True, grow=(2200.0, 2200.0, 250.0), grow_down=(2200.0, 2200.0, 250.0))),
    ('SM_P3E_PicnicHill', lit(picnic_hill), dict(far=True, grow=(60.0, 60.0, 120.0), grow_down=(60.0, 60.0, 0.0))),
    ('SM_P3E_WaveStand', lit(wave_stand), dict(far=True, grow=(60.0, 60.0, 200.0), grow_down=(60.0, 60.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
