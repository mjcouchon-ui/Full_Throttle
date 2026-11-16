"""FULL THROTTLE (Unreal) the population of the THEME PARK (level index 4, Coaster Cove): /Game/People/ThemePark/.

The park already has guests (ft_dress_park.py: _guest in the queue rails and on the benches, SM_Crowd before the
stalls). They stand on the ground behind the verge's hoarding, so from the road they are a band of heads. This kit
adds the park's own cast, and everything in it is RAISED, MASSED, COLOURED and MOVING, because that is what reads
from a road with a metre of barrier at 120 mph:

    the parade      floats and marchers in a lane 24.9 to 27.1 m out: a tiered cake float with dancers, the giraffe and a candle,
                    stilt walkers, flag bearers and ribbon dancers, the band's wagon with the hippo conducting. They
                    travel the piece's own length and come round again.
    the mascots     three original animals in costume (a giraffe in a pink waistcoat, a lilac hippo, a mint frog):
                    in the parade, and meeting guests on a podium under a rainbow arch (a queue up the ramp).
    the performers  a high wire 8 m up between two masts (two walkers, one under a parasol), a show stage (a kick line,
                    a trampolinist, the frog as compere), the dog show (dogs through hoops).
    the staff       mint shirt, navy trousers, pink cap, a gold sash: on a cherry picker hanging bunting, at the top
                    of the queue tower, leading the ponies, at the podium.
    the queues      a switchback ramp three decks high to a slide's station, guests shuffling up it.
    creatures       a pony ring (ponies with riders going round a raised ring), the dogs of the dog show, doves
                    wheeling over the stalls.
    far             an open-air theatre's stand full of people (a band of colour that hops), doves.

No clown anywhere in this kit, and no balloons: the level's boss is MR. CHUCKLES, a clown who flies under a bunch of
balloons and bombs the road with them (the one inflatable here is a whale the size of a bus). It is a night level: every piece carries a small steady glow
(lift(), and rows of steady lamps), all of it 24 m or more from the centre line; nothing blinks. No words, no marks.

Run in the editor:  import ft_people_park; ft_people_park.run()       (or run(['SM_ParkHighWire']))
The placing rules are Source/FullThrottle/FTPeopleL4.inl.
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/ThemePark'
NAVY, PURPLE, PLUM = '#2b2350', '#3a2f6a', '#4a3a78'
MAG, CYAN, AMBER, CREAM = '#ff4fb8', '#2ee6ff', '#ffd23f', '#fff1d6'
RED, WHITE, LAV, MINT, TEAL = '#ff2d3c', '#fff6ea', '#b9a4ff', '#7fe3c4', '#2fc6c0'
ORANGE, GOLD, WOOD, WOOD_D, GREY_L, DARK = '#ff8a2a', '#ffc93c', '#9a6a44', '#6a4c36', '#9a98a8', '#22202a'
FROG = '#7dffb0'
BRIGHT = (MAG, CYAN, AMBER, RED, ORANGE, '#8f6bff', '#b8ff5a', WHITE)


# ------------------------------------------------------------------------------------------ helpers
def staff(r, **fixed):
    """A member of the park's staff: mint shirt, navy trousers, pink cap, gold sash."""
    d = P.look(r, top=MINT, bottom=NAVY, hat='cap', hat_col=MAG, band=GOLD, sleeves='short', wear='trousers', shoes=WHITE)
    d.update(fixed)
    return d


def guest(r, **fixed):
    d = P.look(r, tops=BRIGHT)
    d.update(fixed)
    return d


def kid(r, **fixed):
    d = P.look(r, tops=BRIGHT, build='kid', s=r.uniform(0.6, 0.7))
    d.update(fixed)
    return d


def together(sc, base, fn):
    """Everything fn() draws that travels takes ONE place on the line, so a float and its riders (or a pony and its
    rider) come round as one thing instead of each wrapping where it stands."""
    before = set(sc.parts)
    out = fn()
    for k in sc.order:
        if k not in before:
            mo = sc.parts[k][1]
            if mo['travel'][0] > 0.5:
                mo['base'] = (float(base[0]), float(base[1]))
    return out


def lift(sc, g):
    """The park is a NIGHT level and everything round these pieces is lit: a figure in plain colours standing in a
    stall's shadow reads as a dark shape. Every colour of a piece gets a small steady glow of its own (g: 0.04 is about
    two fifths of the colour), like a figure under stage light. Every piece of this kit stands 24 m or more from the
    road's centre line, where a steady light is allowed; nothing blinks."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.04):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def bulbs(m, a, b, n, size=13.0, glow=1.3, cols=('#ffe28a',)):
    """A row of steady lamps from a to b (boxes: a lamp is 12 triangles)."""
    for k in range(n):
        f = (k + 0.5) / n
        m.box(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f), (size, size, size), glow=glow)


def tv_of(travel):
    return (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)


def bunting(m, a, b, n=8, cols=(MAG, CYAN, AMBER, WHITE), drop=34.0):
    """A line of pennants from a to b (a line 5 cm thick: the cel pass makes thinner ones flicker)."""
    tilt_box(m, CREAM, a, b, 5, 5)
    for k in range(n):
        f = (k + 0.5) / n
        p = (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f)
        m.box(cols[k % len(cols)], (p[0], p[1], p[2] - drop * 0.5), (drop * 0.8, 4, drop), rot=(0, 0, 0))


def stripes(m, x0, x1, y, z0, z1, cols, n, depth=6.0):
    w = (x1 - x0) / float(n)
    for k in range(n):
        m.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y, (z0 + z1) / 2.0), (w, depth, z1 - z0))


def hold_bigflag(col, pole=CREAM, h=330.0, w=130.0):
    def f(m, hd, side):
        m.box(pole, (hd[0], hd[1], hd[2] + h / 2.0 - 40), (5, 5, h))
        m.box(col, (hd[0], hd[1] + w / 2.0, hd[2] + h - 80), (4, w, 70))
        m.sphere(GOLD, (hd[0], hd[1], hd[2] + h - 36), 9, steps=(3, 5))
    return f


def hold_ribbon(col, length=150.0):
    """A wand with a long ribbon in three folds (it swings with the arm)."""
    def f(m, hd, side):
        m.box(CREAM, (hd[0], hd[1], hd[2] + 16), (4, 4, 40))
        p = (hd[0], hd[1], hd[2] + 36)
        for k in range(3):
            q = (p[0], p[1] + length / 3.0, p[2] + (22 if k % 2 == 0 else -22))
            tilt_box(m, col, p, q, 4, 13)
            p = q
    return f


def hold_pole(col=CREAM, length=430.0):
    """A wire walker's balancing pole, across both hands (drawn once, from the right hand)."""
    def f(m, hd, side):
        if side > 0:
            m.box(col, (0, hd[1], hd[2] + 4), (length, 6, 6))
            for sx in (-1, 1):
                m.box(MAG, (sx * length / 2.0, hd[1], hd[2] + 4), (24, 9, 9))
    return f


def hold_parasol(col=CYAN, trim=WHITE):
    def f(m, hd, side):
        m.box(CREAM, (hd[0], hd[1], hd[2] + 50), (4, 4, 110))
        m.cyl(col, (hd[0], hd[1], hd[2] + 96), 62, 22, steps=8, top_r=6)
        m.cyl(trim, (hd[0], hd[1], hd[2] + 92), 64, 5, steps=8)
    return f


def hold_horn(col=GOLD):
    def f(m, hd, side):
        m.cyl(col, (hd[0], hd[1] - 4, hd[2] + 2), 5, 52, rot=(0, 0, 90), steps=6, top_r=19)
    return f


def hold_baton(col=WHITE):
    def f(m, hd, side):
        m.box(col, (hd[0], hd[1], hd[2] + 34), (4.5, 4.5, 70))
        m.sphere(GOLD, (hd[0], hd[1], hd[2] + 70), 8, steps=(3, 5))
    return f


# ------------------------------------------------------------------------------------------ the mascots
MASCOTS = {'giraffe': (AMBER, MAG, CYAN), 'hippo': (LAV, CYAN, AMBER), 'frog': (FROG, RED, AMBER)}   # fur, waistcoat, bow tie


def mascot(sc, kind, x, y, z=0.0, yaw=0.0, s=1.2, phase=0.0, rate=1.1, bob=6.0, travel=None, stride=0.0, arm_l='hip', arm_r='wave',
           swing_r=26.0, swing_l=0.0, react=False):
    """One of the park's three costumed animals: a person in a fur suit and a waistcoat with a big costume head
    (a giraffe's long neck, a hippo's great muzzle, a frog's eyes). Original animals: none is anybody's character.
    Returns the top (z in the mesh's space)."""
    fur, coat, tie = MASCOTS[kind]
    P.figure(sc, x, y, z=z, yaw=yaw, s=s, skin=fur, top=coat, bottom=fur, shoes=fur, hair='bald', build='stocky', sleeves='none',
             arm_l=arm_l, arm_r=arm_r, swing_r=swing_r, swing_l=swing_l, axis_l='y', axis_r='y', stride=stride, detail=2,
             phase=phase, rate=rate, bob=bob, react=react, travel=travel)
    # the costume head nods from the neck, in step with the body's hop
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    mo = P.motion(phase=phase, rate=rate, bob=bob, react=react, base=(x, y), travel=tv_of(travel), swing=5.0, pivot=(x, y, z + s * 124.0), axis=yaw)
    h = sc.part(mo, (x, y, z), yaw, s)
    h.box(tie, (0, -19, 116), (44, 7, 15))
    h.box(WHITE, (0, -20, 116), (11, 8, 11))
    if kind == 'giraffe':
        h.cyl(fur, (0, 0, 118), 31, 60, steps=8, top_r=17)
        h.cyl(fur, (0, 0, 176), 16, 130, steps=6)
        for k, (sx, sz) in enumerate(((-9, 196), (8, 232), (-7, 266), (9, 208), (0, 286))):
            h.box(ORANGE, (sx, -15 if k % 2 == 0 else 15, sz), (15, 5, 18))
        h.box(fur, (0, -14, 322), (38, 64, 36))
        h.box(CREAM, (0, -52, 315), (32, 22, 27))
        for sx in (-1, 1):
            h.cyl(fur, (sx * 10, 6, 338), 4.5, 18, steps=5)
            h.sphere(ORANGE, (sx * 10, 6, 359), 7, steps=(3, 5))
            h.box(fur, (sx * 26, 8, 330), (16, 7, 11))
            h.box(DARK, (sx * 19.6, -22, 327), (2.5, 7, 9))
        top = 366.0
    elif kind == 'hippo':
        h.sphere(fur, (0, 0, 160), 52, scale=(1.12, 1.0, 0.95), steps=(4, 8))
        h.box(fur, (0, -40, 140), (88, 52, 46))
        for sx in (-1, 1):
            h.box(WHITE, (sx * 22, -60, 114), (11, 9, 13))
            h.box(DARK, (sx * 16, -66.5, 152), (8, 2, 6))
            h.sphere(fur, (sx * 36, 6, 206), 11, steps=(3, 5))
            h.sphere(WHITE, (sx * 21, -38, 186), 10, steps=(3, 5))
            h.box(DARK, (sx * 21, -47, 186), (6, 3, 8))
        top = 216.0
    else:
        h.sphere(fur, (0, 0, 158), 50, scale=(1.16, 1.0, 0.86), steps=(4, 8))
        for sx in (-1, 1):
            h.sphere(WHITE, (sx * 25, -8, 202), 18, steps=(3, 6))
            h.box(DARK, (sx * 25, -25, 204), (9, 4, 11))
        h.box(DARK, (0, -47, 146), (62, 4, 5))
        top = 222.0
    return z + s * top


# ------------------------------------------------------------------------------------------ the parade
def _float_body(m, L, W, H, cols):
    """A parade float's body: a striped skirt down to the ground, a gold-edged deck H up."""
    stripes(m, -L / 2.0, L / 2.0, -W / 2.0, 6, H - 8, cols, 8)
    stripes(m, -L / 2.0, L / 2.0, W / 2.0, 6, H - 8, cols, 8)
    m.box(cols[0], (0, 0, (H - 8) / 2.0 + 3), (L - 4, W - 6, H - 14))
    m.box(GOLD, (0, 0, H - 4), (L + 16, W + 16, 12))
    for sx in (-1, 1):
        m.cyl(DARK, (sx * L * 0.3, -W / 2.0 - 2, 24), 24, 8, rot=(0, 0, 90), steps=8)
    bulbs(m, (-L / 2.0, -W / 2.0 - 9, H - 4), (L / 2.0, -W / 2.0 - 9, H - 4), 10)


def parade_a(sc):
    """The cake float: three tiers, dancers on the deck, the giraffe on the top tier, a great candle at the back.
    The whole float goes along the lane and comes round again."""
    r = random.Random(41)
    TR = (1700.0, 85.0)
    mo = P.motion(travel=tv_of(TR), base=(0.0, 0.0))

    def draw():
        f = sc.part(mo)
        _float_body(f, 760, 200, 170, (MAG, CREAM))
        f.cyl(CREAM, (-170, 0, 170), 105, 80, steps=10)
        f.cyl(MAG, (-170, 0, 240), 109, 14, steps=10)
        f.cyl(CYAN, (-170, 0, 254), 70, 60, steps=10)
        f.cyl(AMBER, (-170, 0, 310), 74, 12, steps=10)
        for k in range(5):
            a = k * math.pi * 2.0 / 5.0
            f.sphere(RED, (-170 + math.cos(a) * 105, math.sin(a) * 105, 262), 13, steps=(3, 5))
        mascot(sc, 'giraffe', -170, 0, z=322, s=1.0, phase=0.2, rate=0.9, bob=5, arm_l='wave', swing_l=-22, travel=TR)
        acts = (dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=16, rate=1.5),
                dict(arm_l='out', arm_r='wave', swing_r=28, turn=30, bob=10, rate=1.1),
                dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=18, rate=1.4))
        for k, px in enumerate((40, 170, 300)):
            d = guest(r, wear='dress' if k % 2 == 0 else 'shorts', top=(CYAN, AMBER, '#b8ff5a')[k], bottom=(MAG, RED, NAVY)[k], hair=('pony', 'afro', 'bun')[k])
            d.update(acts[k])
            P.figure(sc, px, (-30, 36, -20)[k], z=176, yaw=(-12, 8, 20)[k], phase=r.random(), travel=TR, detail=1, **d)
        # the candle at the back, its flame swaying
        for k in range(5):
            f.cyl(CREAM if k % 2 == 0 else MAG, (-335, 0, 176 + k * 52), 24, 52, steps=8)
        b = sc.part(P.motion(travel=tv_of(TR), base=(0.0, 0.0), swing=7.0, pivot=(-335.0, 0.0, 436.0), axis=0.0, rate=0.7, phase=0.3))
        b.sphere(AMBER, (-335, 0, 472), 30, scale=(0.75, 0.75, 1.4), steps=(4, 6), glow=1.2)
        bunting(f, (-60, -96, 250), (370, -96, 250), n=7)
        for px in (-60, 370):
            f.box(GOLD, (px, -96, 214), (6, 6, 78))
    together(sc, (0.0, 0.0), draw)


def _stilt(sc, r, x, y, TR, phase, col_a, col_b, yaw=90.0, **kw):
    """A stilt walker: a figure 1.45 m up on stilts in long striped trousers that stride with the legs."""
    up, stride, rate = 145.0, 15.0, 0.75
    d = guest(r, top=col_a, bottom=col_b, wear='trousers', sleeves='long', hat='sun', hat_col=col_b, s=1.0, build='slim')
    d.update(kw)
    P.figure(sc, x, y, z=up, yaw=yaw, stride=stride, rate=rate, phase=phase, travel=TR, detail=1, **d)
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    for side in (-1, 1):
        hx = side * 7.0                      # avg / slim hips: HW / 2 - 8
        piv = (x + hx * cy, y + hx * sy, up + 66.0)
        m = sc.part(P.motion(phase=phase, rate=rate, base=(x, y), travel=tv_of(TR), swing=side * stride, pivot=piv, axis=yaw), (x, y, up), yaw, 1.0)
        for k in range(2):
            m.box(col_b if k % 2 else WHITE, (side * 9.0, 0, -36 - k * 72), (17, 19, 72))
        m.box(DARK, (side * 9.0, -4, -150 + 8), (19, 30, 12))


def parade_b(sc):
    """The walking parade: two flag bearers, three stilt walkers, two ribbon dancers, the frog out in front. Every
    one of them walks the lane (16 m) and comes round again."""
    r = random.Random(43)
    TR = (1600.0, 85.0)
    walk = dict(yaw=90, stride=22, rate=0.9, bob=3, travel=TR, detail=1)
    mascot(sc, 'frog', 700, 10, yaw=90, s=1.25, phase=0.1, rate=0.9, bob=5, stride=18, arm_l='wave', swing_l=-24, travel=TR)
    for k, px in enumerate((480, 380)):
        P.figure(sc, px, (-50, 50)[k], arm_r=((40, 60), (95, 95)), hold_r=hold_bigflag((CYAN, AMBER)[k]), swing_l=16, axis_l='x', phase=0.5 * k,
                 **dict(walk, **guest(r, top=RED, bottom=WHITE, hat='peak', hat_col=RED, band=GOLD, sleeves='long', detail=0)))
    _stilt(sc, r, 140, -30, TR, 0.0, MAG, CYAN, arm_l='wave', arm_r='out', swing_l=-24)
    _stilt(sc, r, -120, 40, TR, 0.4, AMBER, RED, arm_l='up', arm_r='up', swing_l=14, swing_r=-14)
    _stilt(sc, r, -380, -20, TR, 0.7, '#b8ff5a', '#8f6bff', arm_l='out', arm_r='wave', swing_r=24, hold_r=hold_ribbon(AMBER))
    for k, px in enumerate((-590, -720)):
        P.figure(sc, px, (40, -40)[k], arm_r='wave', swing_r=40, hold_r=hold_ribbon((MAG, CYAN)[k]), arm_l='out', swing_l=20, phase=0.3 + 0.4 * k,
                 **dict(walk, bob=9, **guest(r, wear='dress', top=(AMBER, WHITE)[k], hair='pony')))


def parade_c(sc):
    """The band's wagon: a stepped wagon with the band on it (a bass drum, three horns, a pair of cymbals), the hippo
    conducting from the front step. It goes along the lane and comes round again."""
    r = random.Random(47)
    TR = (1700.0, 85.0)
    mo = P.motion(travel=tv_of(TR), base=(0.0, 0.0))

    def draw():
        f = sc.part(mo)
        _float_body(f, 720, 200, 150, (CYAN, CREAM))
        f.box(PURPLE, (-150, 20, 190), (380, 150, 70))
        f.box(GOLD, (-150, 20, 228), (392, 162, 8))
        band = dict(top=RED, bottom=WHITE, hat='peak', hat_col=RED, band=GOLD, sleeves='long', wear='trousers')
        # the back row, on the step: the bass drum and two horns
        P.figure(sc, -290, 30, z=232, arm_r=((70, 80), (100, 80)), swing_r=30, axis_r='x', arm_l='carry', rate=1.5, phase=0.0, travel=TR, detail=1, **guest(r, **band))
        d = sc.part(P.motion(travel=tv_of(TR), base=(0.0, 0.0)))
        d.cyl(WHITE, (-290, -22, 340), 52, 34, rot=(0, 0, 90), steps=10)
        d.cyl(RED, (-290, -20, 340), 56, 6, rot=(0, 0, 90), steps=10)
        d.cyl(RED, (-290, -54, 340), 56, 6, rot=(0, 0, 90), steps=10)
        for k, px in enumerate((-150, -20)):
            P.figure(sc, px, 30, z=232, arm_l='face', arm_r='face', hold_r=hold_horn(), rock=5, rate=1.5, phase=0.25 + 0.5 * k, travel=TR, detail=0, **guest(r, **band))
        # the front row, on the deck: a horn and the cymbals
        P.figure(sc, 90, -10, z=156, arm_l='face', arm_r='face', hold_r=hold_horn(), rock=5, rate=1.5, phase=0.5, travel=TR, detail=1, **guest(r, **band))

        def cym(m, hd, side):
            m.cyl(GOLD, (hd[0], hd[1] - 6, hd[2] + 6), 24, 4, rot=(0, 90, 90), steps=8)
        P.figure(sc, 200, 10, z=156, arm_l='hold', arm_r='hold', hold_l=cym, hold_r=cym, swing_l=22, swing_r=-22, axis_l='z', axis_r='z', bob=6, rate=1.5, phase=0.1,
                 travel=TR, detail=1, **guest(r, **band))
        mascot(sc, 'hippo', 310, -20, z=156, yaw=-8, s=1.15, phase=0.3, rate=1.5, bob=8, arm_r='wave', swing_r=30, arm_l='out', swing_l=14, travel=TR)
        for px in (-340, 350):
            f.box(GOLD, (px, 90, 250), (6, 6, 200))
        bunting(f, (-340, 90, 346), (350, 90, 346), n=9)
    together(sc, (0.0, 0.0), draw)


# ------------------------------------------------------------------------------------------ the performers
def high_wire(sc):
    """The high wire: two masts, a wire 8.2 m up, a walker with a balancing pole who rocks at the middle, another
    under a parasol who goes to and fro along the wire, a third waving from the mast's platform; a net under them and
    a knot of guests looking up."""
    r = random.Random(53)
    g = sc.still()
    H = 820.0
    for sx in (-1, 1):
        g.box(PLUM, (sx * 620, 0, 460), (30, 30, 920))
        g.box(CREAM, (sx * 570, 0, H - 6), (150, 110, 12))
        g.box(GOLD, (sx * 620, 0, 928), (44, 44, 16))
        tilt_box(g, CREAM, (sx * 620, 0, 900), (sx * 860, 0, 0), 6, 6)
        for k in range(4):
            g.box(MAG if k % 2 == 0 else CREAM, (sx * 620, 0, 60 + k * 120), (34, 34, 60))
        g.sphere('#ffe28a', (sx * 620, 0, 956), 24, steps=(3, 6), glow=1.3)
    g.box(DARK, (0, 0, H - 4), (1140, 7, 7))
    bunting(g, (-620, 0, 920), (620, 0, 920), n=14, drop=52)
    # the net
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(PLUM, (sx * 430, sy * 140, 110), (12, 12, 220))
    g.box(CYAN, (0, 0, 222), (900, 300, 6))
    g.box(WHITE, (0, 0, 226), (900, 10, 4))
    g.box(WHITE, (0, 0, 226), (10, 300, 4))
    leo = dict(wear='swim', sleeves='long', hair='bun', build='slim', s=1.0)
    P.figure(sc, 0, 0, z=H, yaw=90, legs='stride', arm_l=((80, 0), (80, 0)), arm_r=((80, 0), (80, 0)), hold_r=hold_pole(), rock=9, rate=0.45,
             **guest(r, top=MAG, bottom=GOLD, **leo))
    slide = dict(swing=2.2, pivot=(330.0, 0.0, H - 6000.0), axis=90.0)
    P.figure(sc, 330, 0, z=H, yaw=-90, legs='stride', arm_r=((100, 0), (150, 0)), hold_r=hold_parasol(), arm_l='out', sway=slide, rate=0.09, phase=0.2,
             **guest(r, top=AMBER, bottom=RED, **leo))
    P.figure(sc, -570, -10, z=H, yaw=-10, arm_r='wave', swing_r=26, arm_l='hip', rate=1.1, bob=4, **guest(r, top=CYAN, bottom=NAVY, **leo))
    for k in range(6):
        d = guest(r) if k != 2 else kid(r)
        d.update((dict(arm_r='point', arm_l='down'), dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=8, rate=1.5), dict(arm_r='face', arm_l='hip'))[k % 3])
        P.figure(sc, -330 + k * 130 + r.uniform(-20, 20), -250 + (k % 2) * 50, yaw=180 + r.uniform(-25, 25), phase=r.random(), detail=0, **d)


def stage_a(sc):
    """The show stage: a deck 1.6 m up under a sunburst arch, a kick line of four, a trampolinist going up 2.6 m, and
    the frog as compere."""
    r = random.Random(59)
    g = sc.still()
    g.box(CREAM, (0, 0, 78), (1040, 520, 156))
    stripes(g, -520, 520, -262, 8, 148, (MAG, CREAM), 10)
    g.box(GOLD, (0, -262, 152), (1050, 8, 10))
    bulbs(g, (-500, -268, 164), (500, -268, 164), 12)
    g.box(PURPLE, (0, 245, 430), (1040, 30, 560))
    g.cyl(AMBER, (0, 228, 400), 190, 8, rot=(0, 0, 90), steps=14, glow=0.35)
    for k in range(9):
        a = math.radians(-90 + (k - 4) * 22)
        tilt_box(g, AMBER, (math.cos(a + math.pi / 2.0) * 215, 226, 400 + math.sin(a + math.pi / 2.0) * 215),
                 (math.cos(a + math.pi / 2.0) * 300, 226, 400 + math.sin(a + math.pi / 2.0) * 300), 8, 26)
    stripes(g, -520, 520, 226, 650, 720, (MAG, CREAM, CYAN, CREAM), 12, depth=40)
    for sx in (-1, 1):
        g.box(PLUM, (sx * 500, 100, 430), (40, 320, 560))
        g.box(DARK, (sx * 440, -200, 196), (70, 60, 80))
        g.sphere(GOLD, (sx * 500, 100, 740), 26, steps=(3, 6))
    for k in range(4):
        d = guest(r, wear='skirt', top=(CYAN, MAG)[k % 2], bottom=(MAG, CYAN)[k % 2], sleeves='none', hair=('pony', 'bun', 'long', 'afro')[k], s=1.0)
        P.figure(sc, -330 + k * 130, -120, z=156, stride=38, arm_l='out', arm_r='out', bob=7, rate=1.4, phase=0.5 * (k % 2), detail=1, **d)
    g.cyl(DARK, (330, 60, 156), 120, 46, steps=10)
    g.cyl(CYAN, (330, 60, 200), 104, 5, steps=10)
    P.figure(sc, 330, 60, z=204, arm_l='vee', arm_r='vee', swing_l=20, swing_r=-20, bob=260, rate=0.36, phase=0.1,
             **guest(r, top=AMBER, bottom=RED, wear='swim', sleeves='long', build='slim', hair='short'))
    mascot(sc, 'frog', -440, -170, z=156, yaw=20, s=1.1, phase=0.6, rate=1.2, bob=7)


def stage_b(sc):
    """The dog show: the same kind of stage, lower and open. Three dogs go through hoops one after another (each hops
    as it reaches its hoop), their trainer points, a staff member claps, a child at the front hops with them."""
    r = random.Random(61)
    g = sc.still()
    g.box(CREAM, (0, 0, 70), (1000, 420, 140))
    stripes(g, -500, 500, -212, 8, 132, (CYAN, CREAM), 10)
    g.box(GOLD, (0, -212, 136), (1010, 8, 10))
    bulbs(g, (-480, -218, 148), (480, -218, 148), 12)
    for sx in (-1, 1):
        g.box(PLUM, (sx * 480, 190, 330), (26, 26, 380))
    g.box(MAG, (0, 190, 500), (1000, 30, 60))
    bunting(g, (-480, 190, 460), (480, 190, 460), n=12, drop=48)
    for k, px in enumerate((-250, 20, 290)):
        g.box(WHITE, (px, 0, 190), (8, 8, 100))
        for a in range(10):
            a0, a1 = a * math.pi / 5.0, (a + 1) * math.pi / 5.0
            tilt_box(g, (RED, AMBER, MAG)[k], (px, math.cos(a0) * 62, 300 + math.sin(a0) * 62), (px, math.cos(a1) * 62, 300 + math.sin(a1) * 62), 9, 9)
        P.quadruped(sc, px, 0, z=140, yaw=90, length=74, height=52, girth=22, neck=16, neck_up=35, head=20, tail=24, wag=34, stride=26,
                    rate=0.8, phase=0.33 * k, bob=150, body=('#e8c26a', WHITE, '#8e5a3a')[k], ears=('#8e5a3a', '#22202a', '#5e3b28')[k])
    P.figure(sc, -420, -120, z=140, yaw=-50, arm_r='point', swing_r=14, axis_r='x', arm_l='hip', rate=0.8,
             **guest(r, top=RED, bottom=NAVY, hat='cowboy', hat_col=AMBER, sleeves='long'))
    P.figure(sc, 430, -130, z=140, yaw=40, arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4, **staff(r))
    P.figure(sc, 150, -160, z=140, yaw=170, arm_l='up', arm_r='up', bob=10, rate=1.6, phase=0.4, **kid(r))


def _meet(kind, seed):
    def build(sc):
        """A mascot meets its public: on a striped podium under a rainbow arch, two children hopping beside it,
        a family coming up the ramp, a staff member taking the picture."""
        r = random.Random(seed)
        g = sc.still()
        g.cyl(CREAM, (0, 0, 0), 180, 120, steps=12)
        for k in range(6):
            a = k * math.pi / 3.0
            g.box(MAG if kind != 'hippo' else CYAN, (math.cos(a) * 176, math.sin(a) * 176, 60), (10, 80, 120), rot=(0, math.degrees(a), 0))
        g.cyl(GOLD, (0, 0, 118), 186, 8, steps=12)
        for k in range(7):
            a = math.pi * (1.0 + (k + 0.5) / 7.0)
            g.box('#ffe28a', (math.cos(a) * 190, math.sin(a) * 190, 110), (13, 13, 13), glow=1.3)
        tilt_box(g, GREY_L, (-640, 0, 0), (-170, 0, 112), 150, 12)        # the ramp
        g.box(PLUM, (-400, -78, 60), (480, 6, 6))
        for j, col in enumerate((MAG, AMBER, CYAN)):                      # the rainbow arch, behind the podium
            R, n = 250.0 + j * 36.0, 10
            for k in range(n):
                a0, a1 = math.pi * k / float(n), math.pi * (k + 1) / float(n)
                tilt_box(g, col, (math.cos(a0) * R, 150, 40 + math.sin(a0) * R), (math.cos(a1) * R, 150, 40 + math.sin(a1) * R), 22, 36)
        mascot(sc, kind, 0, 10, z=122, s=1.2 if kind != 'giraffe' else 1.05, phase=0.1, rate=1.1, bob=7, arm_l='wave', swing_l=-24, react=True)
        for k, px in enumerate((-105, 110)):
            P.figure(sc, px, -60, z=122, yaw=(20, -20)[k], arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=12, rate=1.7, phase=r.random(), detail=1, **kid(r))
        for k in range(3):
            f = (k + 0.5) / 3.0
            d = guest(r) if k != 1 else kid(r)
            P.figure(sc, -620 + f * 420, 0, z=f * 100, yaw=90, arm_r=('wave', 'up', 'down')[k], swing_r=(20, 10, 0)[k], bob=(0, 9, 0)[k], rate=1.3,
                     phase=r.random(), hold_l=P.hold_flag(CYAN) if k == 2 else None, detail=0, **d)
        P.figure(sc, 260, -150, yaw=60, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rate=0.3, bob=-1.5, detail=1, **staff(r))
    return build


def parade_d(sc):
    """The balloon giant: a whale the size of a bus, six metres up, on four ropes; four of the staff walk it along
    and two children run beside them."""
    r = random.Random(73)
    TR = (1500.0, 85.0)

    def draw():
        b = sc.part(P.motion(travel=tv_of(TR), base=(0.0, 0.0), bob=-16.0, rate=0.22, phase=0.1))
        Z = 560.0
        b.sphere(CYAN, (0, 0, Z), 120, scale=(2.5, 1.0, 1.0), steps=(5, 10))
        b.sphere(WHITE, (30, 0, Z - 34), 104, scale=(2.3, 0.94, 0.8), steps=(4, 8))
        tilt_box(b, CYAN, (-260, 0, Z + 10), (-380, 0, Z + 70), 150, 26)
        tilt_box(b, CYAN, (-260, 0, Z + 10), (-380, 0, Z - 30), 150, 26)
        for sy in (-1, 1):
            b.box(DARK, (210, sy * 84, Z + 26), (18, 6, 18))
            tilt_box(b, CYAN, (60, sy * 90, Z - 30), (0, sy * 135, Z - 70), 26, 70)
        b.box(WHITE, (150, 0, Z + 150), (8, 8, 70))
        b.sphere(WHITE, (150, 0, Z + 190), 30, scale=(1.6, 1.6, 0.6), steps=(3, 6))
        hands = ((-240, -50), (240, -50), (-240, 50), (240, 50))
        for k, (hx, hy) in enumerate(hands):
            tilt_box(b, WHITE, (hx, hy, 190), (hx * 0.6, hy * 0.6, Z - 100), 4.5, 4.5)
            P.figure(sc, hx, hy, yaw=90, stride=22, rate=0.9, bob=3, phase=0.25 * k, arm_r='up', arm_l='down', swing_l=16, axis_l='x',
                     travel=TR, detail=1 if hy < 0 else 0, **staff(r))
        for k, px in enumerate((420, 520)):
            P.figure(sc, px, (-40, 30)[k], yaw=90, stride=30, rate=1.5, bob=10, phase=0.5 * k, arm_l='up', arm_r='up', swing_l=14, swing_r=-14,
                     travel=TR, detail=1, **kid(r))
    together(sc, (0.0, 0.0), draw)


# ------------------------------------------------------------------------------------------ the queues, the stands
def queue_tower(sc):
    """The queue for the big slide: two ramps, one behind and above the other, a station on top under a striped
    roof, the slide's chute down the far end. Guests shuffle up the ramps; a member of staff waves them on; a child
    goes down the chute."""
    r = random.Random(79)
    g = sc.still()
    R1 = ((-520.0, -100.0, 10.0), (520.0, -100.0, 215.0))
    R2 = ((520.0, 100.0, 235.0), (-380.0, 100.0, 440.0))
    for a, b in (R1, R2):
        tilt_box(g, CREAM, a, b, 170, 12)
        tilt_box(g, MAG, (a[0], a[1] - 86, a[2] + 96), (b[0], b[1] - 86, b[2] + 96), 6, 7)
        tilt_box(g, MAG, (a[0], a[1] - 86, a[2] + 50), (b[0], b[1] - 86, b[2] + 50), 5, 5)
    g.box(CREAM, (585, 0, 222), (130, 380, 12))
    g.box(CREAM, (-520, 100, 446), (290, 190, 12))
    for px, py, pz in ((640, -180, 228), (640, 180, 228), (0, -180, 110), (0, 190, 330), (-390, 190, 440), (-660, 190, 440), (-660, 10, 440), (-390, 10, 440)):
        g.box(PLUM, (px, py, pz / 2.0), (16, 16, pz))
    for px in (-660, -390):
        for py in (10, 190):
            g.box(PLUM, (px, py, 560), (10, 10, 230))
    stripes(g, -690, -360, 100, 672, 700, (CYAN, CREAM), 6, depth=240)
    g.cyl(GOLD, (-525, 100, 700), 16, 50, steps=6, top_r=2)
    bulbs(g, (-690, -24, 668), (-360, -24, 668), 6)
    tilt_box(g, CYAN, (-660, 100, 452), (-1180, 100, 20), 120, 14)                 # the chute
    for sy in (-1, 1):
        tilt_box(g, AMBER, (-660, 100 + sy * 62, 478), (-1180, 100 + sy * 62, 46), 8, 40)
    g.box(CYAN, (-1240, 100, 12), (140, 150, 24))
    acts = (dict(arm_l='down', arm_r='down'), dict(arm_r='wave', swing_r=22, rate=1.2), dict(arm_l='up', arm_r='up', swing_l=10, swing_r=-10, bob=7, rate=1.5),
            dict(arm_r='point', arm_l='hip'), dict(arm_r='face', arm_l='down', hold_r=P.hold_cup()))
    for j, (a, b) in enumerate((R1, R2)):
        n = 5 if j == 0 else 4
        for k in range(n):
            f = (k + 0.6) / (n + 0.4)
            d = guest(r) if (k + j) % 4 != 2 else kid(r)
            d.update(acts[(k * 2 + j) % len(acts)])
            d.setdefault('rate', 0.5)
            d.setdefault('bob', 3)
            P.figure(sc, a[0] + (b[0] - a[0]) * f, a[1] + 10, z=a[2] + (b[2] - a[2]) * f + 8, yaw=(90 if j == 0 else -90) + (-80 if k % 3 == 1 else 0),
                     phase=r.random(), react=(k % 3 == 1), detail=0, **d)
    P.figure(sc, -450, 60, z=452, yaw=-20, arm_r='wave', swing_r=28, arm_l='hip', rate=1.2, bob=4, detail=1, **staff(r))
    # down the chute: a slide along its slope (a small swing about a pivot far off, square to the slope)
    P.figure(sc, -920, 100, z=250, yaw=-90, legs='sit', seat=20, arm_l='up', arm_r='up', rate=0.22, phase=0.3,
             sway=dict(swing=2.2, pivot=(-920.0 + 3829.0, 100.0, 250.0 - 4619.0), axis=90.0), detail=1, **kid(r))


def grandstand(sc):
    """A stand of guests watching the chase: four rows on steps under a striped awning, sixteen people who hop and
    wave out of step and liven up as the cars come by; two flags."""
    r = random.Random(83)
    g = sc.still()
    rows = 4
    for k in range(rows):
        g.box(PURPLE if k % 2 else PLUM, (0, -120 + k * 80, (110 + k * 60) / 2.0), (920, 80, 110 + k * 60))
    stripes(g, -460, 460, -162, 6, 104, (AMBER, CREAM), 8)
    for sx in (-1, 1):
        g.box(PLUM, (sx * 470, 140, 290), (16, 16, 580))
        g.box(PLUM, (sx * 470, -150, 250), (12, 12, 500))
    for k in range(8):
        tilt_box(g, MAG if k % 2 else CREAM, (-470 + (k + 0.5) * 117.5, 160, 590), (-470 + (k + 0.5) * 117.5, -170, 500), 117.5, 8)
    bulbs(g, (-460, -168, 488), (460, -168, 488), 10)
    acts = (dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=12, rate=1.6), dict(arm_r='wave', swing_r=26, bob=4, rate=1.3),
            dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=14, rate=1.4), dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7),
            dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4, bob=3))
    for k in range(rows):
        for i in range(4):
            d = guest(r) if (i + k) % 5 != 3 else kid(r)
            d.update(acts[(i * 3 + k * 2) % len(acts)])
            if (k, i) in ((3, 0), (2, 3)):
                d.update(arm_r=((120, 0), (160, 0)), swing_r=20, hold_r=P.hold_flag((CYAN, AMBER)[i % 2]), bob=6, rate=1.2)
            P.figure(sc, -345 + i * 230 + (k % 2) * 90 + r.uniform(-16, 16), -120 + k * 80, z=110 + k * 60, yaw=r.uniform(-14, 14), phase=r.random(),
                     react=True, detail=0, **d)


def far_crowd(sc):
    """The open-air theatre, for the far distance (it keeps moving out there): a stand 24 m wide and 9 m high packed
    with people drawn as blocks of colour, hopping in a wave that runs along the stand; a striped roof, a row of
    steady lamps along its edge. Stand it at 1.4 to 2 times its size."""
    r = random.Random(89)
    g = sc.still()
    rows, cols, W = 5, 14, 2400.0
    for k in range(rows):
        g.box(PURPLE if k % 2 else PLUM, (0, -200 + k * 110, (200 + k * 130) / 2.0), (W, 110, 200 + k * 130))
    g.box(NAVY, (0, 300, 450), (W + 80, 40, 900))
    for sx in (-1, 1):
        g.box(NAVY, (sx * (W / 2.0 + 30), 40, 400), (40, 560, 800))
    for k in range(10):
        tilt_box(g, MAG if k % 2 else CREAM, (-W / 2.0 + (k + 0.5) * W / 10.0, 320, 1010), (-W / 2.0 + (k + 0.5) * W / 10.0, -260, 900), W / 10.0, 12)
    for k in range(12):
        g.sphere('#ffe28a', (-W / 2.0 + (k + 0.5) * W / 12.0, -262, 880), 20, steps=(3, 5), glow=1.4)
    tops = (MAG, CYAN, AMBER, RED, WHITE, '#b8ff5a', ORANGE, '#8f6bff')
    for c in range(cols):
        m = sc.part(P.motion(phase=c / float(cols), rate=0.55, bob=42.0))
        for k in range(rows):
            x = -W / 2.0 + (c + 0.5) * W / cols + r.uniform(-18, 18)
            y, z = -200 + k * 110, 200 + k * 130
            m.box(r.choice(tops), (x, y, z + 52), (86, 50, 104), glow=0.05)
            m.box(r.choice(P.SKINS), (x, y, z + 128), (48, 44, 46), glow=0.04)
            if (c + k) % 3 == 0:
                m.box(r.choice(tops), (x + 34, y, z + 150), (16, 16, 90), glow=0.05)


# ------------------------------------------------------------------------------------------ the staff
def cherry_picker(sc):
    """Two of the staff up a cherry picker hanging a line of bunting from a lamp post, a third on the ground waving
    it up; the boom and its basket rise and fall a little."""
    r = random.Random(97)
    g = sc.still()
    g.box(MINT, (0, 0, 70), (300, 150, 70))
    g.box(CREAM, (-90, 0, 130), (110, 140, 60))
    g.box(DARK, (-90, -72, 136), (80, 4, 34))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(DARK, (sx * 100, sy * 76 - 5, 34), 34, 10, rot=(0, 0, 90), steps=8)
    g.cyl(AMBER, (70, 0, 105), 30, 30, steps=8)
    for k, (cx, cy) in enumerate(((-260, -120), (300, -130), (360, 60))):
        g.cyl(ORANGE, (cx, cy, 0), 20, 56, steps=6, top_r=5)
        g.cyl(WHITE, (cx, cy, 24), 15, 10, steps=6, top_r=12)
    sway = dict(swing=4.0, pivot=(70.0, 0.0, 120.0), axis=90.0)
    b = sc.part(P.motion(rate=0.1, **sway))
    tilt_box(b, AMBER, (70, 0, 120), (330, 0, 520), 26, 26)
    b.box(CREAM, (380, 0, 530), (150, 120, 12))
    for sx in (-1, 1):
        for sy in (-1, 1):
            b.box(MAG, (380 + sx * 72, sy * 57, 580), (7, 7, 100))
    for sy in (-1, 1):
        b.box(MAG, (380, sy * 57, 628), (150, 7, 8))
    for sx in (-1, 1):
        b.box(MAG, (380 + sx * 72, 0, 628), (7, 120, 8))
    P.figure(sc, 345, 0, z=536, yaw=-80, arm_l='up', arm_r='up', swing_r=8, axis_r='x', rate=0.1, sway=sway, **staff(r))
    P.figure(sc, 420, 10, z=536, yaw=10, arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=MAG, w=40), rate=0.1, sway=sway, **staff(r))
    # the lamp post they are dressing, and the line already up
    g.box(PLUM, (640, 0, 400), (20, 20, 800))
    g.sphere(CREAM, (640, 0, 820), 30, steps=(3, 6))
    bunting(g, (640, 0, 760), (1000, 0, 600), n=6, drop=50)
    g.box(PLUM, (1000, 0, 300), (14, 14, 600))
    tilt_box(g, CREAM, (640, 0, 760), (470, 0, 720), 5, 5)
    P.figure(sc, 170, -130, yaw=-30, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=0.8, bob=3, **staff(r, hat='hard', hat_col=AMBER, band='#b8ff5a'))


# ------------------------------------------------------------------------------------------ creatures
def pony_ring(sc):
    """The pony ring: a raised round ring under a striped top on a post, four ponies with children up going round
    it, one of the staff in the middle turning with them."""
    r = random.Random(101)
    g = sc.still()
    g.cyl(CREAM, (0, 0, 0), 540, 100, steps=16)
    for k in range(16):
        a = (k + 0.5) * math.pi / 8.0
        if k % 2 == 0:
            g.box(MINT, (math.cos(a) * 536, math.sin(a) * 536, 52), (10, 206, 96), rot=(0, math.degrees(a), 0))
    g.cyl(GOLD, (0, 0, 98), 548, 8, steps=16)
    g.cyl(PLUM, (0, 0, 100), 20, 420, steps=8)
    g.cyl(MAG, (0, 0, 500), 330, 110, steps=12, top_r=20)
    g.cyl(CREAM, (0, 0, 486), 336, 16, steps=12)
    g.sphere(GOLD, (0, 0, 624), 24, steps=(3, 6))
    for k in range(12):
        a = k * math.pi / 6.0
        g.box('#ffe28a', (math.cos(a) * 338, math.sin(a) * 338, 482), (13, 13, 13), glow=1.3)
    g.cyl(MINT, (0, 0, 106), 130, 30, steps=10)
    RAD, RATE = 400.0, 0.05
    coats = (('#8e5a3a', '#3b2a22'), (WHITE, '#cfcfd8'), ('#e8c26a', WHITE), ('#5a4a3c', '#22202a'))
    for k in range(4):
        a = k * math.pi / 2.0 + 0.3
        px, py = math.cos(a) * RAD, math.sin(a) * RAD
        yaw = math.degrees(a) + 180.0
        ph = 0.25 * k
        P.quadruped(sc, px, py, z=106, yaw=yaw, s=0.82, body=coats[k][0], mane=coats[k][1], stride=20, rate=1.2, phase=ph, bob=4, wag=14, orbit=(0, 0, RATE))
        P.figure(sc, px, py, z=106 + 0.82 * 120 - 46 * 0.68, yaw=yaw, legs='sit', seat=46, arm_l=('up', 'hold', 'wave', 'hold')[k], arm_r=('up', 'hold', 'hold', 'wave')[k],
                 rate=1.2, phase=ph, bob=4, orbit=(0, 0, RATE), detail=0, **kid(r, s=0.68, hat='cowboy' if k % 2 == 0 else None, hat_col=RED))
    P.figure(sc, 110, -70, z=136, yaw=30, arm_r='out', arm_l='hip', rate=RATE, orbit=(0, 0, RATE), detail=1, **staff(r))
    for k in range(2):
        P.figure(sc, (-90, 80)[k], -470, z=106, yaw=180, arm_r='wave', swing_r=22, arm_l='down', bob=6, rate=1.3, phase=0.5 * k, detail=0, **guest(r))


def doves(sc):
    """A flock of white doves wheeling over the park: ten birds, each 2.6 m across so that they read from the road,
    9 to 16 m up, going round at their own speeds. Saved with the always-moving master."""
    r = random.Random(103)
    for k in range(10):
        a = r.uniform(0, math.pi * 2.0)
        rad = r.uniform(500, 1300)
        z = r.uniform(900, 1600)
        P.bird(sc, math.cos(a) * rad, math.sin(a) * rad, z, heading=math.degrees(a) + 90.0, span=r.uniform(230, 290), body='#fff6ea', wing='#e8e0f0',
               beak=AMBER, phase=r.random(), rate=r.uniform(1.6, 2.2), flap=34, orbit=(0, 0, r.uniform(0.05, 0.08)))


# ------------------------------------------------------------------------------------------ the film crew's own piece here
def crew_tower(sc):
    """The shoot's camera tower (the park's verge is a hoarding, so the camera goes up): a scaffold 3 m high, the film
    camera panning on its tripod with its operator, the first assistant pointing, two of the crew cheering from the
    lower deck; the crew's pink-and-white tape round its foot. The crew kit's own people, wardrobe 'Park'."""
    r = random.Random(107)
    w = C.WARDROBES['Park']
    g = sc.still()
    C.tape(g, 700, -170, posts=5)
    H = 300.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(C.STEEL, (sx * 150 - 100, sy * 110, H / 2.0), (9, 9, H))
        tilt_box(g, C.STEEL, (sx * 150 - 100, -110, 10), (-sx * 150 - 100, -110, H - 10), 6, 6)
    g.box(C.WOOD, (-100, 0, H), (330, 250, 12))
    for sy in (-1, 1):
        g.box(C.YEL, (-100, sy * 120, H + 96), (330, 6, 8))
    for sx in (-1, 1):
        g.box(C.YEL, (sx * 162 - 100, 0, H + 96), (6, 240, 8))
        for sy in (-1, 1):
            g.box(C.YEL, (sx * 162 - 100, sy * 120, H + 50), (6, 6, 100))
    g.box(C.PINK, (-100, -124, H + 40), (330, 4, 46))
    # the lower deck, beside it
    g.box(C.WOOD, (210, 0, 150), (260, 220, 12))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(C.STEEL, (210 + sx * 120, sy * 100, 75), (9, 9, 150))
    g.box(C.YEL, (210, -106, 240), (260, 6, 8))
    C.tripod(g, -130, -30, top=H + 128)
    pan = dict(swing=24.0, pivot=(-130.0, -30.0, 0.0), axis=P.UP)
    cam = sc.part(P.motion(rate=0.12, phase=0.2, **pan))
    C.film_camera(cam, -130, -30, H + 134)
    P.figure(sc, -130, 50, z=H + 6, arm_l='push', arm_r='push', rate=0.12, phase=0.2, sway=pan, **C.member(r, w, hat='headset', hat_col=C.DARK))
    P.figure(sc, -10, -40, z=H + 6, yaw=-30, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.6, **C.member(r, w))
    P.figure(sc, 160, -30, z=156, yaw=-10, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.6, phase=0.2, react=True, **C.member(r, w))
    P.figure(sc, 265, -20, z=156, yaw=12, arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.6, phase=0.6, react=True, **C.member(r, w))
    C.case(g, 330, 150)


PAR = dict(grow=(520.0, 20.0, 60.0), grow_down=(520.0, 20.0, 0.0))
JOBS = [
    ('SM_ParkParade_A', lit(parade_a), PAR),
    ('SM_ParkParade_B', lit(parade_b), dict(grow=(60.0, 20.0, 60.0), grow_down=(60.0, 20.0, 0.0))),
    ('SM_ParkParade_C', lit(parade_c), PAR),
    ('SM_ParkParade_D', lit(parade_d), PAR),
    ('SM_ParkHighWire', lit(high_wire), dict(grow=(340.0, 20.0, 60.0), grow_down=(340.0, 20.0, 0.0))),
    ('SM_ParkStage_A', lit(stage_a), dict(grow=(60.0, 60.0, 300.0))),
    ('SM_ParkStage_B', lit(stage_b), dict(grow=(60.0, 60.0, 190.0))),
    ('SM_ParkMeet_A', lit(_meet('giraffe', 67))),
    ('SM_ParkMeet_B', lit(_meet('hippo', 71))),
    ('SM_ParkQueueTower', lit(queue_tower), dict(grow=(300.0, 60.0, 60.0), grow_down=(300.0, 60.0, 0.0))),
    ('SM_ParkGrandstand', lit(grandstand, 0.05)),
    ('SM_ParkCherryPicker', lit(cherry_picker)),
    ('SM_ParkPonyRing', lit(pony_ring)),
    ('SM_ParkFarCrowd', lit(far_crowd, 0.03), dict(far=True)),
    ('SM_ParkDoves', lit(doves, 0.06), dict(far=True, grow=(400.0, 400.0, 100.0), grow_down=(400.0, 400.0, 0.0))),
    ('SM_ParkCrewTower', lit(crew_tower)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
