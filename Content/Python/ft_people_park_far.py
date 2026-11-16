"""FULL THROTTLE (Unreal) the THEME PARK's far life and creatures (level index 4): /Game/People/ThemePark/SM_Far...

The extra worker's kit for the far distance (150 m to the horizon) and the park's animals. The main kit
(ft_people_park.py, SM_Park...) has the parade lane by the road, the stages, the stands, doves and a pony ring; this
one adds what lives beyond them, and nothing here is a copy of those:

    SM_FarMarch_A    a marching band crossing a parade viaduct 7 m over the lawns: shakos, flags, bass drums, horns,
                     a drum major; 4 m figures (far things are drawn big) that march the viaduct's length and come round.
    SM_FarMarch_B    the same viaduct, the lagoon's serpent as a carried parade puppet: a head and eleven hoops of
                     colour rising and falling in a wave over the bearers' poles, flag bearers before and behind.
    SM_FarDucks      a skein of nine great mallards in a V, flying the length of two slots and round again.
    SM_FarDuckPond   a duck pond on a lawn: a ring of lit water, ducks swimming round an island with a duck house,
                     a mother and her ducklings in a line, two ducks coming in to land.
    SM_FarQueue      a far ride's station: a switchback ramp four flights up to a platform, a queue shuffling up
                     it, the people on the platform waving and hopping, pigeons on the roof and wheeling round it.
    SM_FarBalloons   a balloon seller's cart on a mound, the bunch as big as a tree (floating, each balloon out of
                     step), children hopping for them, two balloons that got away.

Rules kept: every piece stands 24 m or more from the road (the table puts them at 60 m and beyond), so the small
steady glow that lifts their colours out of the park's night is allowed; nothing blinks. No clown (the boss is
MR. CHUCKLES). No words, no marks. The ducks, pigeons and the two far parades are saved with the always-moving
master (far=True).

Run in the editor:  import ft_people_park_far; ft_people_park_far.run()      (or run(['SM_FarDucks']))
The placing rules are the "far" block of Source/FullThrottle/FTPeopleL4.inl.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/ThemePark'
NAVY, PURPLE, PLUM = '#2b2350', '#3a2f6a', '#4a3a78'
MAG, CYAN, AMBER, CREAM = '#ff4fb8', '#2ee6ff', '#ffd23f', '#fff1d6'
RED, WHITE, LAV, MINT, TEAL = '#ff2d3c', '#fff6ea', '#b9a4ff', '#7fe3c4', '#2fc6c0'
ORANGE, GOLD, WOOD, GREY_L, DARK = '#ff8a2a', '#ffc93c', '#9a6a44', '#9a98a8', '#22202a'
LIME, VIOLET, WATER, PINE = '#b8ff5a', '#8f6bff', '#2a5fd0', '#2f8a5a'
DUCK_HEAD, DUCK_BODY, DUCK_WING, DUCK_HEN = '#22c27a', '#f1e4c8', '#57c7ff', '#d9a070'
BRIGHT = (MAG, CYAN, AMBER, RED, ORANGE, VIOLET, LIME, WHITE)
LAMP = '#ffe28a'


# ------------------------------------------------------------------------------------------ helpers
def lift(sc, g):
    """The park is a night level: a figure in plain colours far from the lamps is a dark shape. Every colour of a
    piece gets a small steady glow (g), like a figure under stage light. Allowed because every piece of this kit
    stands far beyond 24 m; nothing blinks."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.07):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def tv_of(travel):
    return (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)


def bulbs(m, a, b, n, size=26.0, glow=1.3, col=LAMP):
    for k in range(n):
        f = (k + 0.5) / n
        m.box(col, (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f), (size, size, size), glow=glow)


def hold_bigflag(col, col2=WHITE, pole=CREAM, h=230.0, w=120.0):
    """A parade flag twice a marcher's height (in the figure's own scale): a pole and a banner in two bands."""
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + h / 2.0 - 30), (6, 6, h + 40))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + h - 28), (w, 5, 56))
        m.box(col2, (hd[0] + side * w / 2.0, hd[1], hd[2] + h - 76), (w, 5, 40))
    return f


def hold_drum(col=WHITE, rim=RED):
    """A bass drum worn in front (drawn once, with the right hand)."""
    def f(m, hd, side):
        if side > 0:
            m.cyl(rim, (hd[0] - 30, hd[1] - 4, hd[2] - 2), 42, 34, rot=(0, 0, 90), steps=8)
            m.cyl(col, (hd[0] - 30, hd[1] - 7, hd[2] - 2), 36, 40, rot=(0, 0, 90), steps=8)
    return f


def hold_horn(col=GOLD):
    def f(m, hd, side):
        m.cyl(col, (hd[0], hd[1] - 4, hd[2] + 4), 6, 46, rot=(0, 0, 90), steps=6, top_r=24)
    return f


def hold_baton(col=WHITE, knob=GOLD):
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1], hd[2] + 46), (6, 6, 110))
        m.box(knob, (hd[0], hd[1], hd[2] + 104), (20, 20, 20))
    return f


def viaduct(sc, L=6400.0, top=700.0, deep=520.0, cols=(MAG, CREAM)):
    """The parade's viaduct: a deck 7 m up on arches, a striped fascia, a row of steady lamps under its edge.
    Returns the deck's z."""
    g = sc.still()
    g.box(PLUM, (0, 0, top - 30), (L, deep, 60))
    n = 16
    w = L / n
    for k in range(n):
        g.box(cols[k % 2], (-L / 2.0 + (k + 0.5) * w, -deep / 2.0 - 4, top - 70), (w, 10, 150))
    for k in range(5):
        x = -L / 2.0 + (k + 0.5) * L / 5.0
        g.box(NAVY, (x, 0, (top - 60) / 2.0), (260, deep - 80, top - 60))
        g.box(PURPLE, (x, -deep / 2.0 + 34, (top - 60) / 2.0), (300, 12, top - 60))
    bulbs(g, (-L / 2.0, -deep / 2.0 - 12, top - 160), (L / 2.0, -deep / 2.0 - 12, top - 160), 20)
    return top


def band_look(r, **kw):
    """The band's uniform: a red coat with a gold sash, white trousers, a tall white shako."""
    d = P.look(r, top=RED, bottom=WHITE, shoes=DARK, hat='chef', hat_col=WHITE, band=GOLD, sleeves='long', wear='trousers', hair='short')
    d.update(kw)
    return d


# ------------------------------------------------------------------------------------------ the far parades
def march_a(sc):
    """A marching band on the parade viaduct: eight bandsmen 4.6 m tall (far things are drawn big) marching its 64 m
    and coming round again: the drum major with her baton, three flag bearers, two bass drums, horns."""
    r = random.Random(401)
    L = 6400.0
    top = viaduct(sc, L)
    S, V = 2.7, 300.0
    acts = [
        dict(arm_r='up', swing_r=26, axis_r='x', hold_r=hold_baton(), arm_l='hip', hat_col=GOLD),                    # the drum major
        dict(arm_r='fwd', hold_r=hold_bigflag(MAG, CYAN), swing_l=24, axis_l='x'),
        dict(arm_l='carry', arm_r='carry', hold_r=hold_drum(), swing_l=20, axis_l='x'),
        dict(arm_l='face', arm_r='face', hold_r=hold_horn()),
        dict(arm_r='fwd', hold_r=hold_bigflag(AMBER, RED), swing_l=24, axis_l='x'),
        dict(arm_l='face', arm_r='face', hold_l=hold_horn()),
        dict(arm_l='carry', arm_r='carry', hold_r=hold_drum(CREAM, CYAN), swing_l=20, axis_l='x'),
        dict(arm_l='face', arm_r='face', hold_r=hold_horn()),
    ]
    n = len(acts)
    for k, act in enumerate(acts):
        d = band_look(r, s=S * r.uniform(0.96, 1.04))
        d.update(act)
        x = L / 2.0 - (k + 0.5) * L / n
        P.figure(sc, x, -120 + (k % 2) * 150, top, yaw=90, detail=0, stride=26, rate=1.1, bob=10.0, phase=(k % 2) * 0.5,
                 travel=(L, V), **d)


def march_b(sc):
    """The lagoon's serpent as a parade puppet, carried over the viaduct: a head with a crest and eleven hoops of
    colour that rise and fall in a wave on the bearers' poles (they travel the viaduct's 64 m and come round), a
    flag bearer before and behind."""
    r = random.Random(409)
    L = 6400.0
    top = viaduct(sc, L, cols=(CYAN, CREAM))
    S, V = 2.2, 240.0
    tv = tv_of((L, V))
    n = 12
    gap = 330.0
    x0 = 1900.0
    cols = (LIME, MINT, CYAN, MINT)
    for k in range(n):
        x = x0 - k * gap
        mo = P.motion(phase=k * 0.11, rate=0.55, bob=-150.0, pivot=(x, 0, top + 620), base=(x, 0.0), travel=tv)
        m = sc.part(mo)
        if k == 0:      # the head: jaws, a crest, an eye each side
            m.box(LIME, (x + 120, 0, top + 640), (420, 250, 240))
            m.box(MINT, (x + 330, 0, top + 590), (220, 210, 110))
            m.box(RED, (x + 330, 0, top + 520), (190, 170, 30))
            for sy in (-1, 1):
                m.box(WHITE, (x + 200, sy * 128, top + 700), (70, 12, 70))
                m.box(DARK, (x + 210, sy * 134, top + 700), (30, 8, 34))
            for j in range(3):
                tilt_box(m, MAG, (x + 60 - j * 110, 0, top + 740), (x - 20 - j * 110, 0, top + 900), 26, 90)
        else:
            sz = 230.0 - (k > 8) * (k - 8) * 40.0
            m.cyl(cols[k % 4], (x - gap * 0.46, 0, top + 620), sz / 2.0, gap * 0.92, rot=(-90, 0, 0), steps=6)
            m.box(MAG if k % 2 else AMBER, (x, 0, top + 620 + sz / 2.0 + 30), (gap * 0.5, 22, 70))
        if k % 2 == 0 and k < 11:     # a bearer under every other hoop: the pole goes up into the wave
            P.figure(sc, x, 0, top, yaw=90, detail=0, stride=24, rate=1.0, bob=8.0, phase=k * 0.13, arm_l='up', arm_r='up', travel=(L, V),
                     **P.look(r, top=TEAL, bottom=NAVY, hat='cap', hat_col=AMBER, s=S))
            g = sc.part(P.motion(base=(x, 0.0), travel=tv, pivot=(x, 0, top)))
            g.box(CREAM, (x, 0, top + 500), (14, 14, 330))
    for x, c1, c2 in ((x0 + 700, MAG, AMBER), (x0 - n * gap - 300, AMBER, MAG)):
        P.figure(sc, x, -60, top, yaw=90, detail=0, stride=26, rate=1.1, bob=10.0, arm_r='fwd', hold_r=hold_bigflag(c1, c2), swing_l=24, axis_l='x',
                 travel=(L, V), **P.look(r, top=WHITE, bottom=MAG, hat='chef', hat_col=c1, s=S))


# ------------------------------------------------------------------------------------------ ducks
def duck(sc, x, y, z, heading=0.0, span=320.0, phase=0.0, rate=2.6, travel=None, orbit=None, hen=False, flap=30.0, bob=0.0):
    """A mallard in the air: P.bird with a long neck, a green head (a hen: brown) and a white collar."""
    P.bird(sc, x, y, z, heading=heading, span=span, body=(DUCK_HEN if hen else DUCK_BODY), wing=(CREAM if hen else DUCK_WING), beak=ORANGE,
           phase=phase, rate=rate, flap=flap, travel=travel, orbit=orbit, bob=bob)
    tv, base = (0.0, 0.0), (x, y)
    if travel:
        tv = tv_of(travel)
    elif orbit:
        base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
    m = sc.part(P.motion(pivot=(x, y, z), phase=phase, rate=rate, bob=bob, base=base, travel=tv))
    m.base = None
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    Lb = span * 0.42
    a = (x + tx * Lb * 0.45, y + ty * Lb * 0.45, z + Lb * 0.05)
    b = (x + tx * Lb * 1.0, y + ty * Lb * 1.0, z + Lb * 0.16)
    tilt_box(m, WHITE, a, b, Lb * 0.17, Lb * 0.17)
    m.box(WOOD if hen else DUCK_HEAD, (x + tx * Lb * 1.08, y + ty * Lb * 1.08, z + Lb * 0.2), (Lb * 0.36, Lb * 0.3, Lb * 0.3), rot=(0, heading, 0))
    m.box(ORANGE, (x + tx * Lb * 1.36, y + ty * Lb * 1.36, z + Lb * 0.16), (Lb * 0.26, Lb * 0.2, Lb * 0.09), rot=(0, heading, 0))


def ducks(sc):
    """A skein of nine great mallards in a V (each 3.4 m across, so that they read from the road), 15 to 19 m up,
    flying the length of four slots along the road and round again. The always-moving master."""
    r = random.Random(419)
    L, V = 18000.0, 1300.0
    for k in range(9):
        rank = (k + 1) // 2
        sy = -1 if k % 2 else 1
        x = 1300.0 - rank * 360.0
        y = sy * rank * 300.0
        z = 1700.0 - rank * 40.0 + r.uniform(-30, 30)
        duck(sc, x, y, z, heading=0.0, span=r.uniform(320, 360), phase=r.random(), rate=r.uniform(2.3, 2.8), travel=(L, V), hen=(k % 3 == 2))


def swim_duck(sc, cx, cy, rad, ang, z, rps, size=1.0, hen=False, phase=0.0, chick=False):
    """A duck on the water, swimming round (cx, cy): `ang` degrees round the circle of radius `rad`, heading along
    it (the orbit turns it as it goes). size 1 = 2.2 m long: a far duck is a big duck. chick: a duckling (a yellow
    ball with a head)."""
    a = math.radians(ang)
    x, y = cx + math.cos(a) * rad, cy + math.sin(a) * rad
    hdg = ang + (90.0 if rps >= 0 else -90.0)
    h = math.radians(hdg)
    tx, ty = math.cos(h), math.sin(h)
    m = sc.part(P.motion(phase=phase, rate=0.7, bob=-7.0 * size, pivot=(x, y, z), base=(cx, cy), travel=(-1.0, rps)))
    m.base = None
    L = 220.0 * size

    def at(f, up):
        return (x + tx * L * f, y + ty * L * f, z + L * up)
    if chick:
        m.box(AMBER, at(0, 0.16), (L * 0.5, L * 0.36, L * 0.32), rot=(0, hdg, 0))
        m.box(AMBER, at(0.26, 0.4), (L * 0.28, L * 0.26, L * 0.26), rot=(0, hdg, 0))
        m.box(ORANGE, at(0.46, 0.38), (L * 0.14, L * 0.12, L * 0.07), rot=(0, hdg, 0))
        return
    body = DUCK_HEN if hen else DUCK_BODY
    m.box(body, at(0, 0.17), (L, L * 0.48, L * 0.34), rot=(0, hdg, 0))
    m.box(CREAM if hen else DUCK_WING, at(-0.08, 0.3), (L * 0.56, L * 0.52, L * 0.16), rot=(0, hdg, 0))
    tilt_box(m, WOOD if hen else DARK, at(-0.42, 0.3), at(-0.66, 0.5), L * 0.26, L * 0.1)
    tilt_box(m, WHITE, at(0.36, 0.26), at(0.5, 0.62), L * 0.16, L * 0.16)
    m.box(WOOD if hen else DUCK_HEAD, at(0.54, 0.7), (L * 0.3, L * 0.26, L * 0.24), rot=(0, hdg, 0))
    m.box(ORANGE, at(0.77, 0.66), (L * 0.2, L * 0.17, L * 0.07), rot=(0, hdg, 0))


def duck_pond(sc):
    """A duck pond on a lawn, 21 m across: a ring of lit water in a pale rim, an island with a duck house on a
    post, six great ducks swimming round it, a hen with five ducklings in a line behind her, three ducks circling
    low over the water as if coming in to land."""
    r = random.Random(431)
    g = sc.still()
    g.cyl(CREAM, (0, 0, -260), 1050, 290, steps=14)
    g.cyl(WATER, (0, 0, -250), 980, 292, steps=14, glow=0.5)
    g.cyl('#57c7ff', (0, 0, 40), 640, 3, steps=12, glow=0.6)
    g.cyl(PINE, (0, 0, 0), 230, 90, steps=8, top_r=180)
    g.box(WOOD, (0, 0, 220), (26, 26, 280))
    g.box(CREAM, (0, 0, 420), (200, 170, 150))
    g.box(DARK, (0, -86, 410), (60, 6, 70))
    tilt_box(g, RED, (-130, 0, 480), (0, 0, 590), 210, 14)
    tilt_box(g, RED, (0, 0, 590), (130, 0, 480), 210, 14)
    for k in range(6):
        g.cyl(LIME, (math.cos(k * 1.05 + 0.3) * 1000, math.sin(k * 1.05 + 0.3) * 1000, 30), 60, 240, steps=5, top_r=8)
    Z = 42.0
    for k in range(6):
        swim_duck(sc, 0, 0, r.uniform(430, 520) if k % 2 else r.uniform(760, 860), k * 60.0 + r.uniform(-12, 12), Z, 0.035 if k % 2 else -0.022,
                  size=r.uniform(0.95, 1.15), hen=(k % 3 == 1), phase=r.random())
    swim_duck(sc, 0, 0, 640, 200.0, Z, 0.028, size=1.05, hen=True)
    for k in range(5):
        swim_duck(sc, 0, 0, 640, 200.0 - 15.0 - k * 9.5, Z, 0.028, size=0.42, chick=True, phase=k * 0.2)
    for k in range(3):
        a = k * 2.1 + 0.5
        rad = 1250.0 + k * 130.0
        duck(sc, math.cos(a) * rad, math.sin(a) * rad, 520.0 + k * 190.0, heading=math.degrees(a) + 90.0, span=300, phase=r.random(), rate=2.6,
             orbit=(0, 0, 0.07 + k * 0.012), hen=(k == 1))


# ------------------------------------------------------------------------------------------ the far queue
def pigeon(sc, x, y, z, heading, span, phase, orbit=None, flap=40.0, rate=3.4):
    """A pigeon as three boxes (a far flock is many small things): a lilac body, pale wings that beat."""
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    base, tv = ((orbit[0], orbit[1]), (-1.0, float(orbit[2]))) if orbit else ((x, y), (0.0, 0.0))
    b = sc.part(P.motion(pivot=(x, y, z), phase=phase, rate=rate, base=base, travel=tv))
    b.base = None
    b.box(LAV, (x, y, z), (span * 0.5, span * 0.2, span * 0.16), rot=(0, heading, 0))
    b.box(MAG, (x + tx * span * 0.22, y + ty * span * 0.22, z + span * 0.02), (span * 0.14, span * 0.21, span * 0.17), rot=(0, heading, 0))
    for side in (-1, 1):
        w = sc.part(P.motion(swing=side * flap, pivot=(x, y, z), axis=heading, phase=phase, rate=rate, base=base, travel=tv))
        w.base = None
        w.box(WHITE, (x - ty * side * span * 0.28, y + tx * side * span * 0.28, z + span * 0.03), (span * 0.26, span * 0.5, span * 0.04), rot=(0, heading, 0))


def queue_tower(sc):
    """A far ride's station, a helter-skelter 25 m high: a striped tower, three galleries round it with the queue
    walking round and round on its way up (blocks of colour 4 m tall: far things are drawn big), four people at
    the top rail waving, a lamp under the roof, and a flock of pigeons wheeling round the roof."""
    r = random.Random(443)
    g = sc.still()
    H = 1900.0
    n = 6
    for k in range(n):      # the tower: bands of two colours, narrowing
        r0 = 520.0 - k * 36.0
        g.cyl(MAG if k % 2 else CREAM, (0, 0, k * H / n), r0, H / n, steps=8, top_r=r0 - 36.0)
    g.cyl(NAVY, (0, 0, -200), 560, 200, steps=10)
    levels = (260.0, 800.0, 1340.0)
    for i, z in enumerate(levels):
        rr = 520.0 - (z / H) * 220.0 + 330.0
        g.cyl(PLUM, (0, 0, z - 40), rr, 40, steps=10)
        g.cyl(CYAN if i % 2 else AMBER, (0, 0, z - 70), rr + 6, 34, steps=10)
        cnt = 6
        for k in range(cnt):
            a = (k + r.uniform(-0.2, 0.2)) * 2 * math.pi / cnt
            rad = rr - 130.0
            x, y = math.cos(a) * rad, math.sin(a) * rad
            m = sc.part(P.motion(phase=r.random(), rate=1.3, bob=14.0, pivot=(x, y, z), base=(0.0, 0.0), travel=(-1.0, 0.028 if i % 2 else -0.028)))
            m.base = None
            yaw = math.degrees(a)
            sz = r.uniform(0.9, 1.08)
            top = r.choice(BRIGHT)
            m.box(r.choice((NAVY, '#3a5fa8', CREAM, '#c94f6a')), (x, y, z + 80 * sz), (84, 70, 160 * sz), rot=(0, yaw, 0))
            m.box(top, (x, y, z + 225 * sz), (120, 76, 140 * sz), rot=(0, yaw, 0))
            m.box(r.choice(P.SKINS), (x, y, z + 345 * sz), (84, 80, 100 * sz), rot=(0, yaw, 0))
            if k % 3 == 0:
                m.box(top, (x, y, z + 360 * sz), (30, 150, 26), rot=(0, yaw, 0))      # arms out: a wave at this size
    # the top platform, its rail and roof
    g.cyl(PLUM, (0, 0, H - 30), 640, 40, steps=12)
    g.cyl(AMBER, (0, 0, H - 64), 650, 34, steps=12)
    for k in range(6):
        a = k * math.pi / 3.0
        g.box(CREAM, (math.cos(a) * 600, math.sin(a) * 600, H + 300), (26, 26, 600))
    g.cyl(RED, (0, 0, H + 600), 760, 380, steps=10, top_r=30)
    g.cyl(CREAM, (0, 0, H + 560), 780, 46, steps=10)
    g.sphere(LAMP, (0, 0, H + 1010), 60, steps=(4, 6), glow=1.5)
    bulbs(g, (-560, -560, H + 540), (560, -560, H + 540), 6, size=40)
    for k in range(4):
        x = -420 + k * 280
        P.figure(sc, x, -470, H, yaw=r.uniform(-15, 15), detail=0, arm_l=('up' if k % 2 else 'down'), arm_r='wave', swing_r=26, swing_l=(20 if k % 2 else 0),
                 rate=1.3, bob=16.0, phase=r.random(), react=True, **P.look(r, top=BRIGHT[k], s=2.2))
    # pigeons: a ring of them round the roof, out of step
    for k in range(7):
        a = r.uniform(0, 2 * math.pi)
        rad = r.uniform(900, 1500)
        pigeon(sc, math.cos(a) * rad, math.sin(a) * rad, H + r.uniform(300, 1100), math.degrees(a) + 90.0, r.uniform(210, 260), r.random(),
               orbit=(0, 0, r.uniform(0.09, 0.13)))


# ------------------------------------------------------------------------------------------ the balloon seller
def balloons(sc):
    """A balloon seller on a mound: a striped cart, the seller holding the strings, a bunch of fifteen balloons as
    big as a tree (each floating out of step), five children hopping for them with their arms up, and two balloons
    that got away, drifting up and down high over the cart."""
    r = random.Random(457)
    g = sc.still()
    g.cyl(PINE, (0, 0, -300), 760, 560, steps=10, top_r=520)
    T = 260.0
    g.cyl(CREAM, (0, 0, T - 6), 520, 10, steps=10)
    for k in range(6):      # the cart
        g.box(MAG if k % 2 else CREAM, (-150 + (k + 0.5) * 50, 40, T + 70), (50, 150, 110))
    for sx in (-1, 1):
        g.cyl(AMBER, (sx * 150, -40, T + 40), 40, 12, rot=(0, 0, 90), steps=8)
    g.box(CYAN, (0, 40, T + 132), (330, 180, 14))
    cols = (RED, AMBER, CYAN, MAG, LIME, VIOLET, ORANGE, WHITE)
    knot = (0.0, 30.0, T + 330.0)
    g.box(CREAM, (0, 30, T + 230), (12, 12, 200))
    for k in range(15):
        a = k * 2.399
        rad = 60.0 + 54.0 * math.sqrt(k)
        bx, by = math.cos(a) * rad * 1.25, 30 + math.sin(a) * rad * 0.8
        bz = T + 620.0 + (k % 5) * 95.0 + r.uniform(-30, 30)
        m = sc.part(P.motion(phase=r.random(), rate=r.uniform(0.35, 0.6), bob=-22.0, swing=5.0, pivot=knot, axis=90.0))
        m.sphere(cols[k % len(cols)], (bx, by, bz), 96, scale=(1, 1, 1.18), steps=(4, 6))
        tilt_box(m, CREAM, knot, (bx, by, bz - 104), 5, 5)
    for k, (bx, bz) in enumerate(((-520, T + 1500), (430, T + 1900))):
        m = sc.part(P.motion(phase=k * 0.4, rate=0.09, bob=-260.0, swing=4.0, pivot=(bx, 0, T), axis=90.0))
        m.sphere(cols[k * 3], (bx, 0, bz), 96, scale=(1, 1, 1.18), steps=(4, 6))
        m.box(CREAM, (bx, 0, bz - 190), (5, 5, 170))
    P.figure(sc, 0, -70, T, detail=1, arm_r='up', arm_l='wave', swing_l=22, rate=1.0, bob=6.0, react=True,
             **P.look(r, top=MINT, bottom=NAVY, hat='sun', hat_col=AMBER, band=MAG, s=1.7))
    for k in range(5):
        x = -400 + k * 200 + (60 if k >= 2 else -40)
        P.figure(sc, x, -230 + (k % 2) * 110, T, yaw=(-25 if x < 0 else 25), detail=0, arm_l='up', arm_r='up', swing_l=14, swing_r=-14,
                 rate=1.5, bob=30.0, phase=r.random(), react=True, **P.look(r, top=BRIGHT[k], s=1.25, build='kid'))


JOBS = [
    ('SM_FarMarch_A', lit(march_a), dict(far=True, grow=(200.0, 60.0, 80.0), grow_down=(200.0, 60.0, 0.0))),
    ('SM_FarMarch_B', lit(march_b), dict(far=True, grow=(200.0, 60.0, 220.0), grow_down=(200.0, 60.0, 0.0))),
    ('SM_FarDucks', lit(ducks, 0.12), dict(far=True, grow=(9000.0, 200.0, 200.0), grow_down=(9000.0, 200.0, 0.0))),
    ('SM_FarDuckPond', lit(duck_pond, 0.09), dict(far=True, grow=(600.0, 600.0, 100.0), grow_down=(600.0, 600.0, 0.0))),
    ('SM_FarQueue', lit(queue_tower, 0.08), dict(far=True, grow=(900.0, 900.0, 100.0), grow_down=(900.0, 900.0, 0.0))),
    ('SM_FarBalloons', lit(balloons, 0.08), dict(far=True, grow=(100.0, 100.0, 320.0), grow_down=(100.0, 100.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
