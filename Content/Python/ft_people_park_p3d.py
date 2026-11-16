"""FULL THROTTLE (Unreal) the THEME PARK, population pass 3, builder p3d: HEIGHTS (/Game/People/ThemePark/SM_P3D_...).

People UP HIGH, so that they clear the road's barrier, the verge's hoarding and the stalls' roofs (a stall is 3.2 m to
its roof and 4.7 m to its sign). Every piece brings its own structure, and is TALL: it stands 39 m or more from the
road's centre line (the table block is C:/ftwork/p3/L4d.inl). What is new beside the main kit's stages and towers and the
watchers' bridges, terrace, station, look-out and bleachers:

    SM_P3D_Balcony_A    the park hotel's front: two long balconies (4.3 and 8.0 m), five guests on each, cheering, waving,
                        filming; lit windows behind them, a striped awning, lamps along the cornice, two pennants
    SM_P3D_Balcony_B    a narrower, taller house: three balconies (4.3, 7.9, 11.5 m), three guests on each, a striped
                        banner that sways under the cornice
    SM_P3D_ClockTower   a clock tower: four heralds with long horns on its balcony 7.6 m up, rocking as they play; the
                        clock's hands and its pendulum swing; guests on the terrace at its foot (3.3 m)
    SM_P3D_Castle       a castle's curtain wall between two turrets: six guests in the crenels 5.4 m up, a knight with
                        a great flag at each end; banners that sway on the wall, pennants on the turrets
    SM_P3D_TreeHouse    a tree house: six children and parents on the deck round the trunk (5.2 m), three in the
                        look-out above (9 m), a child on a swing under the deck, lanterns
    SM_P3D_RopeBridge   two wooden towers 16 m apart and a plank bridge between them 6.8 m up: four walkers cross it
                        (they travel along the piece and come round again), guests wave from both towers
    SM_P3D_Lighthouse   a striped lighthouse 29 m tall: seven guests on its low gallery (7 m), five on the lantern
                        gallery (24.6 m, 1.6 times life size), a steady lamp. With a deep foot: over the lagoon it
                        stands on the valley floor and its lantern gallery is at the height of the coaster's rails
    SM_P3D_CrowsNest    a ship's mast on a piece of deck: guests at the deck's rail (3 m), three sailors in the low
                        nest (8.2 m) and three in the top one (24.8 m: a telescope, a great flag); a striped sail sways.
                        A deep foot, like the lighthouse
    SM_P3D_Gantry       Rocket Land's launch gantry: three decks (4.6, 8.6, 12.6 m), guests on the lower two, the
                        rocket's crew in white suits on the top one; the rocket beside it on its pad (a ride, unlit)
    SM_P3D_RoofParty    a kiosk with a party on its roof 4.5 m up: six dancers at the parapet, two on a riser, the
                        disc jockey at the booth, strings of steady lamps, a mirror ball that turns to and fro, a giant
                        ice-cream cone on the corner

The park is a NIGHT level: like the other park kits every colour gets a steady glow of its own (lift()), the people
more than their structure. All of it stands 24 m or more out (in fact 39 m), where a steady light is allowed; nothing
blinks. No clown (the boss is MR. CHUCKLES), no balloons (his bombs), no words, no marks.

Run in the editor:  import ft_people_park_p3d; ft_people_park_p3d.run()      (or run(['SM_P3D_Castle']))
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
ORANGE, GOLD, GREY_L, DARK, BULB = '#ff8a2a', '#ffc93c', '#9a98a8', '#22202a', '#ffe28a'
LIME, VIOLET = '#b8ff5a', '#8f6bff'
WOOD, WOOD_D = '#9a6a44', '#6a4c36'
STONE, STONE_D = '#cfc4e6', '#9a8fc0'
GREEN, GREEN_L = '#3fae6a', '#6fd98a'
BRIGHT = (MAG, CYAN, AMBER, RED, ORANGE, VIOLET, LIME, WHITE)
BIG = 1.3      # figures a third over life size, as the watchers' kit draws them: at 40 m a life-size figure is a speck
LOW = (0, 1, 2, 4, 5)        # what a watcher does where there is a floor close overhead (no flags, no foam hands)


# ------------------------------------------------------------------------------------------ helpers
def lift(sc, g, gp):
    """A steady glow on every colour of the piece (the level is lit by night): gp for everything that moves (the
    people, what they hold, flags), g for the structure (a third of it on its dark colours)."""
    for key in sc.order:
        model, mo = sc.parts[key]
        moving = mo['rate'] != 0.0
        out = {}
        for (col, glow), pm in model.meshes.items():
            want = gp if moving else (g / 3.0 if col in (NAVY, PURPLE, PLUM, DARK, WOOD_D) else g)
            k2 = (col, max(glow, want))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.12, gp=0.34):
    def build(sc):
        fn(sc)
        lift(sc, g, gp)
    build.__doc__ = fn.__doc__
    return build


def stripes(m, x0, x1, y, z0, z1, cols, n, depth=10.0):
    w = (x1 - x0) / float(n)
    for k in range(n):
        m.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y, (z0 + z1) / 2.0), (w, depth, z1 - z0))


def bulbs(m, a, b, n, size=14.0, glow=1.3, cols=(BULB,)):
    """A row of steady lamps from a to b."""
    for k in range(n):
        f = (k + 0.5) / n
        m.box(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f), (size, size, size), glow=glow)


def guest(r, **fixed):
    d = P.look(r, tops=BRIGHT)
    d.update(fixed)
    return d


def kid(r, **fixed):
    d = P.look(r, tops=BRIGHT, build='kid', s=r.uniform(0.62, 0.7))
    d.update(fixed)
    return d


def staff(r, **fixed):
    """The park's staff: mint shirt, navy trousers, pink cap, gold sash (the main kit's uniform)."""
    d = P.look(r, top=MINT, bottom=NAVY, hat='cap', hat_col=MAG, band=GOLD, sleeves='short', wear='trousers', shoes=WHITE)
    d.update(fixed)
    return d


def pennant(sc, x, y, z0, h, col, phase, w=120.0):
    """A pole with a pennant that swings round it."""
    sc.still().box(CREAM, (x, y, z0 + h / 2.0), (8, 8, h))
    m = sc.part(P.motion(phase=phase, rate=0.8, swing=24.0, pivot=(x, y, z0 + h), axis=P.UP))
    m.box(col, (x + w / 2.0, y, z0 + h - 30), (w, 5, 56))


def hold_foam(col):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 2, h[2] + 24), (32, 12, 46))
        m.box(col, (h[0], h[1] - 2, h[2] + 60), (12, 12, 30))
    return f


def hold_film(screen='#9ff0ff'):
    """A phone held up to film the chase (a steady rim of light: these pieces stand 39 m out and more)."""
    def f(m, h, side):
        m.box(DARK, (h[0] - side * 10, h[1] - 6, h[2] + 12), (17, 5, 30))
        m.box(screen, (h[0] - side * 10, h[1] - 3, h[2] + 12), (21, 2, 34), glow=0.5)
    return f


def hold_horn(col=GOLD):
    """A herald's long horn, its bell towards the road."""
    def f(m, h, side):
        if side > 0:
            tilt_box(m, col, (h[0], h[1] - 2, h[2] + 4), (h[0], h[1] - 84, h[2] + 30), 7, 7)
            m.box(col, (h[0], h[1] - 96, h[2] + 34), (30, 22, 30))
            m.box(MAG, (h[0], h[1] - 44, h[2] - 12), (4, 40, 36))
    return f


def hold_scope(col=GOLD):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] - 4, h[2] + 6), (h[0], h[1] - 70, h[2] + 12), 10, 10)
    return f


def hold_bigflag(col, h=260.0, w=150.0):
    def f(m, hd, side):
        m.box(CREAM, (hd[0], hd[1], hd[2] + h / 2.0 - 40), (6, 6, h))
        m.box(col, (hd[0] + side * w / 2.0, hd[1], hd[2] + h - 80), (w, 4, 84))
    return f


FILM = ((60, 70), (120, 86))      # both hands up before the face


def act(r, k):
    """What a watcher does: one of eight things, k picks it."""
    k %= 8
    c = r.choice((MAG, CYAN, AMBER, RED, LIME))
    if k == 0:
        return dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=12, rate=1.6)
    if k == 1:
        return dict(arm_r='wave', swing_r=28, arm_l='hip', bob=4, rate=1.3)
    if k == 2:
        return dict(arm_l=FILM, arm_r=FILM, hold_r=hold_film(), turn=24, rate=0.3)
    if k == 3:
        return dict(arm_r=((120, 0), (160, 0)), swing_r=22, hold_r=P.hold_flag(c, w=76), arm_l='hip', bob=6, rate=1.2)
    if k == 4:
        return dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=14, rate=1.4)
    if k == 5:
        return dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', bob=3, rate=0.7)
    if k == 6:
        return dict(arm_r='up', swing_r=24, hold_r=hold_foam(c), arm_l='down', bob=8, rate=1.5)
    return dict(arm_l='up', arm_r='up', hold_r=P.hold_sign(WHITE, c, w=92, h2=58), bob=8, rate=1.1)


DANCE = (
    dict(arm_l='up', arm_r='up', turn=30, bob=10, rate=1.1),
    dict(arm_l='vee', arm_r='vee', rock=11, bob=6, rate=0.9),
    dict(arm_l='out', arm_r='out', turn=42, bob=8, rate=0.8),
    dict(arm_r='up', arm_l='hip', rock=13, bob=5, rate=1.2),
)


def watcher(sc, r, x, y, z, k, yaw=0.0, child=False, big=BIG, look=None, **kw):
    d = dict(look) if look else (kid(r) if child else guest(r))
    d.update(act(r, k))
    d.update(kw)
    d['s'] = d.get('s', 1.0) * big
    return P.figure(sc, x, y, z=z, yaw=yaw + r.uniform(-12, 12), phase=r.uniform(0.05, 0.95), react=True, detail=0, **d)


def row(sc, r, xs, y, z, kinds, kid_at=(), box_col=TEAL, big=BIG):
    """Watchers at a parapet, one at each x; kid_at: which of them are children (on a box, to see over)."""
    for i, x in enumerate(xs):
        k = kinds[i % len(kinds)]
        if i in kid_at:
            sc.still().box(box_col, (x, y, z + 22), (60, 50, 44))
            watcher(sc, r, x, y, z + 44, k, child=True, big=big)
        else:
            watcher(sc, r, x + r.uniform(-8, 8), y, z, k, big=big)


def parapet(g, x0, x1, y, z, cols, n, h=58.0):
    """A solid striped parapet: a band of colour under the watchers."""
    stripes(g, x0, x1, y, z, z + h, cols, n)
    g.box(CREAM, ((x0 + x1) / 2.0, y, z + h + 5), (x1 - x0 + 16, 18, 10))


def ring(g, cx, cy, z, R, h, cols, a0, a1, n):
    """A parapet round part of a circle: n flat panels from angle a0 to a1 (degrees; 0 = towards the road, -Y)."""
    step = (a1 - a0) / float(n)
    w = 2.0 * R * math.tan(math.radians(step / 2.0)) + 6.0
    for k in range(n):
        a = math.radians(a0 + (k + 0.5) * step)
        g.box(cols[k % len(cols)], (cx + R * math.sin(a), cy - R * math.cos(a), z + h / 2.0), (w, 10, h), rot=(0, math.degrees(a), 0))


def round_row(sc, r, cx, cy, z, R, angles, kinds, big=BIG, look=None, kw=None):
    """Watchers round a gallery, each facing out."""
    for i, a in enumerate(angles):
        ar = math.radians(a)
        extra = dict(kw[i]) if kw and kw[i] else {}
        d = dict(look(r)) if look else guest(r)
        if extra:
            d.update(extra)
            d['s'] = d.get('s', 1.0) * big
            P.figure(sc, cx + R * math.sin(ar), cy - R * math.cos(ar), z=z, yaw=a, phase=r.uniform(0.05, 0.95), react=True, detail=0, **d)
        else:
            watcher(sc, r, cx + R * math.sin(ar), cy - R * math.cos(ar), z, kinds[i % len(kinds)], yaw=a, big=big, look=d)


# ------------------------------------------------------------------------------------------ the balconies
def _house(sc, seed, W, H, levels, wall, trim, pcols, awn, per, kids):
    r = random.Random(seed)
    g = sc.still()
    g.box(wall, (0, 90, H / 2.0), (W, 300, H))                      # the house: its front wall at y = -60
    g.box(trim, (0, 90, H + 20), (W + 60, 340, 40))
    g.box(trim, (0, -64, 150), (W * 0.24, 10, 300))
    g.box(BULB, (0, -70, 160), (W * 0.17, 4, 230), glow=0.9)
    aw = W * 0.36
    for k in range(6):
        x = -aw / 2.0 + (k + 0.5) * aw / 6.0
        tilt_box(g, awn if k % 2 else CREAM, (x, -60, 345), (x, -215, 300), aw / 6.0, 8)
    n = 0
    for j, Z in enumerate(levels):
        pc = pcols[j % len(pcols)]
        g.box(CREAM, (0, -150, Z - 12), (W, 190, 24))               # the balcony: y -245 .. -55
        for sx in (-1, 1):
            tilt_box(g, trim, (sx * (W / 2.0 - 50), -62, Z - 120), (sx * (W / 2.0 - 50), -225, Z - 26), 14, 14)
            g.box(pc[0], (sx * (W / 2.0 - 5), -150, Z + 29), (10, 190, 58))
        parapet(g, -W / 2.0, W / 2.0, -239, Z, pc, per * 2)
        xs = [-W / 2.0 + (i + 0.5) * W / per for i in range(per)]
        for x in xs:
            g.box(BULB, (x, -62, Z + 150), (W / per * 0.42, 6, 180), glow=0.8)       # the lit window behind each
        top = (j == len(levels) - 1)
        kinds = (0, 3, 2, 6, 1, 7, 4) if top else LOW
        row(sc, r, xs, -185, Z, kinds[n % 3:] + kinds[:n % 3], kid_at=[i for i in range(per) if (j, i) in kids], box_col=pc[0])
        n += per
    g = sc.still()
    bulbs(g, (-W / 2.0, -84, H + 6), (W / 2.0, -84, H + 6), 10, cols=(BULB, MAG, BULB, CYAN))
    return r


def balcony_a(sc):
    """The park hotel's front, 14 m wide: two long balconies (4.3 and 8.0 m) with striped parapets, five guests on
    each (they cheer, wave, film, shake flags and foam hands), a lit window behind each, a striped awning over the
    door, lamps along the cornice, a pediment with a gold disc, two pennants."""
    W, H = 1400.0, 1240.0
    _house(sc, 601, W, H, (430.0, 800.0), CREAM, MAG, ((MAG, CREAM), (CYAN, WHITE)), MAG, 5, {(0, 2), (1, 4)})
    g = sc.still()
    tilt_box(g, MAG, (-360, 90, H + 40), (0, 90, H + 230), 300, 30)
    tilt_box(g, MAG, (360, 90, H + 40), (0, 90, H + 230), 300, 30)
    g.box(GOLD, (0, -62, H + 110), (110, 8, 110), glow=0.3)
    pennant(sc, -W / 2.0 + 30, 90, H + 40, 300, AMBER, 0.15, w=150)
    pennant(sc, W / 2.0 - 30, 90, H + 40, 300, CYAN, 0.65, w=150)


def balcony_b(sc):
    """A narrower, taller house, 10 m wide: three balconies one over the other (4.3, 7.9 and 11.5 m), three guests on
    each, lit windows, a striped banner that sways under the cornice, a pointed roof with a pennant."""
    W, H = 1000.0, 1620.0
    _house(sc, 607, W, H, (430.0, 790.0, 1150.0), LAV, PURPLE, ((AMBER, WHITE), (MAG, CREAM), (CYAN, WHITE)), CYAN, 3, {(1, 0)})
    g = sc.still()
    g.cyl(PURPLE, (0, 90, H + 40), 330, 260, steps=4, top_r=14)
    pennant(sc, 0, 90, H + 290, 220, MAG, 0.4, w=150)
    m = sc.part(P.motion(phase=0.2, rate=0.35, swing=5.0, pivot=(0.0, -66.0, H - 10.0), axis=0.0))
    stripes(m, -260, 260, -68, H - 180, H - 10, (RED, WHITE, AMBER, WHITE), 8, depth=5)


# ------------------------------------------------------------------------------------------ the clock tower
def clock_tower(sc):
    """A clock tower 18 m tall on a terrace: four heralds with long horns on its balcony 7.6 m up, rocking as they
    play; the clock's hands and the pendulum under it swing; four guests on the terrace at its foot (3.3 m); a
    pointed roof, a pennant, lamps round the terrace."""
    r = random.Random(613)
    g = sc.still()
    TZ, Z = 330.0, 760.0
    g.box(CREAM, (0, 60, TZ / 2.0), (1040, 480, TZ))                     # the terrace block: y -180 .. 300
    stripes(g, -520, 520, -184, 30, TZ - 40, (LAV, CREAM), 8, depth=8)
    parapet(g, -520, 520, -174, TZ, (AMBER, WHITE), 8)
    for sx in (-1, 1):
        g.box(AMBER, (sx * 515, 60, TZ + 29), (10, 480, 58))
    bulbs(g, (-500, -190, TZ - 16), (500, -190, TZ - 16), 10)
    g.box(PLUM, (0, 60, 900), (400, 400, 1140))                          # the shaft: 3.3 .. 14.7 m, its front at y = -140
    for sx in (-1, 1):
        g.box(LAV, (sx * 190, -130, 900), (24, 24, 1140))
    g.box(CREAM, (0, -210, Z - 12), (720, 200, 24))                      # the heralds' balcony: y -310 .. -110
    for sx in (-1, 1):
        tilt_box(g, LAV, (sx * 300, -142, Z - 130), (sx * 300, -290, Z - 26), 14, 14)
        g.box(MAG, (sx * 355, -210, Z + 29), (10, 200, 58))
    parapet(g, -360, 360, -304, Z, (MAG, CREAM), 6)
    for i, x in enumerate((-250, -84, 84, 250)):
        d = P.look(r, top=RED, bottom=WHITE, hat='peak', hat_col=RED, band=GOLD, sleeves='long', shoes=DARK, s=BIG)
        P.figure(sc, x, -250, z=Z, yaw=(-14, -5, 5, 14)[i], arm_r=((96, 70), (104, 88)), hold_r=hold_horn(), arm_l='hip',
                 rock=(7 if i % 2 else -7), rate=0.6, phase=0.1 + 0.07 * i, react=True, detail=0, **d)
    # the clock: a square face, a round one on it, four marks; the hands swing about its middle, a pendulum under it
    CZ = 1240.0
    g = sc.still()
    g.box(CREAM, (0, -146, CZ), (270, 12, 270))
    g.cyl(AMBER, (0, -150, CZ), 122, 8, rot=(0, 0, 90), steps=10, glow=0.3)
    for a in range(4):
        ca, sa = math.cos(math.radians(a * 90)), math.sin(math.radians(a * 90))
        g.box(DARK, (ca * 108, -160, CZ + sa * 108), (20, 8, 20))
    h = sc.part(P.motion(phase=0.0, rate=0.22, swing=40.0, pivot=(0.0, -164.0, CZ), axis=90.0))
    h.box(DARK, (0, -165, CZ + 44), (16, 6, 100))
    h.box(RED, (36, -166, CZ), (80, 6, 14))
    p = sc.part(P.motion(phase=0.3, rate=0.5, swing=17.0, pivot=(0.0, -150.0, CZ - 150.0), axis=90.0))
    p.box(GOLD, (0, -150, CZ - 225), (9, 6, 150))
    p.box(GOLD, (0, -150, CZ - 310), (54, 10, 54), glow=0.3)
    g = sc.still()
    g.box(CREAM, (0, 60, 1485), (460, 460, 30))
    g.cyl(MAG, (0, 60, 1500), 300, 300, steps=8, top_r=12)
    g.sphere(GOLD, (0, 60, 1812), 22, steps=(3, 6))
    pennant(sc, 0, 60, 1800, 200, AMBER, 0.5, w=140)
    row(sc, r, (-440, -300, 300, 440), -120, TZ, (1, 2, 0, 6), kid_at=(1,), box_col=AMBER)


# ------------------------------------------------------------------------------------------ the castle wall
def castle(sc):
    """A castle's curtain wall 15 m long between two round turrets: six guests in the crenels 5.4 m up (cheering,
    waving, filming, a flag, a foam hand), a knight with a great flag at each end; a gate with a gold grille, four
    banners that sway on the wall, pennants on the turrets' pointed roofs. Its stone goes 2 m under z = 0."""
    r = random.Random(619)
    g = sc.still()
    Z, L, T = 540.0, 1500.0, 260.0
    g.box(STONE, (0, 0, (Z - 200) / 2.0), (L, T, Z + 200))
    g.box(STONE_D, (0, -T / 2.0 + 8, Z + 29), (L, 16, 58))
    n = 13
    w = L / n
    gaps = []
    for k in range(n):
        x = -L / 2.0 + (k + 0.5) * w
        if k % 2 == 0:
            g.box(STONE, (x, -T / 2.0 + 8, Z + 80), (w, 20, 44))
        else:
            gaps.append(x)
    g.box(STONE_D, (0, T / 2.0 - 8, Z + 40), (L, 16, 80))
    g.box(NAVY, (0, -T / 2.0 - 2, 190), (300, 8, 380))
    for k in range(5):
        g.box(GOLD, (-120 + k * 60, -T / 2.0 - 7, 190), (10, 6, 380))
    g.box(STONE_D, (0, -T / 2.0 - 6, 395), (360, 14, 30))
    bulbs(g, (-240, -T / 2.0 - 12, 330), (240, -T / 2.0 - 12, 330), 2, size=22.0)
    for sx in (-1, 1):
        tx = sx * (L / 2.0 + 150)
        g.cyl(STONE, (tx, 0, -200), 190, Z + 560, steps=8)
        g.cyl(STONE_D, (tx, 0, Z + 300), 214, 60, steps=8)
        g.cyl(MAG if sx < 0 else CYAN, (tx, 0, Z + 360), 230, 300, steps=8, top_r=10)
        g.box(BULB, (tx, -190, Z + 170), (50, 8, 90), glow=0.8)
        pennant(sc, tx, 0, Z + 650, 200, AMBER if sx < 0 else MAG, 0.2 + 0.3 * sx, w=140)
    for i, x in enumerate((-520, -260, 260, 520)):
        m = sc.part(P.motion(phase=0.23 * i, rate=0.4, swing=4.0, pivot=(float(x), -T / 2.0 - 4.0, 470.0), axis=0.0))
        c = (MAG, CYAN, AMBER, RED)[i]
        m.box(c, (x, -T / 2.0 - 5, 340), (130, 5, 260))
        m.box(WHITE, (x, -T / 2.0 - 8, 360), (60, 3, 60))
    for i, x in enumerate(gaps):
        watcher(sc, r, x, -T / 2.0 + 62, Z, (0, 2, 3, 1, 6, 4)[i], child=(i == 3))
    for sx in (-1, 1):
        d = P.look(r, top=GREY_L, bottom=GREY_L, hat='helmet', hat_col=GREY_L, band=(RED if sx < 0 else CYAN), sleeves='long', shoes=DARK, s=BIG)
        P.figure(sc, sx * (L / 2.0 - 56), 30, z=Z + 40, yaw=sx * 8, arm_r=((120, 0), (160, 0)), swing_r=20,
                 hold_r=hold_bigflag(RED if sx < 0 else CYAN), arm_l='hip', bob=4, rate=0.8, phase=0.3 + 0.2 * sx, react=True, detail=0, **d)
    sc.still().box(STONE_D, (0, 30, Z + 20), (L, 120, 40))            # the step the knights stand on


# ------------------------------------------------------------------------------------------ the tree house
def tree_house(sc):
    """A tree house in a great tree: six children and parents at the rail of the deck round the trunk (5.2 m), three
    in the look-out above (9 m), a child on a swing under the deck, a red hut with a lit window, lanterns, a rope
    ladder. The trunk goes 2 m under z = 0."""
    r = random.Random(631)
    g = sc.still()
    Z, Z2 = 520.0, 900.0
    g.cyl(WOOD, (0, 0, -200), 135, 1500, steps=8, top_r=80)
    g.sphere(GREEN, (0, 40, 1520), 430, steps=(4, 8))
    g.sphere(GREEN_L, (-350, 20, 1360), 300, steps=(4, 8))
    g.sphere(GREEN_L, (350, 60, 1380), 310, steps=(4, 8))
    g.box(WOOD, (0, -40, Z - 12), (840, 620, 24))                        # the deck: y -350 .. 270
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 90, -70, 190), (sx * 380, -320, Z - 24), 18, 18)
        g.box(AMBER, (sx * 415, -40, Z + 29), (10, 620, 58))
    parapet(g, -420, 420, -344, Z, (WOOD_D, AMBER), 8)
    g.box(RED, (0, 160, Z + 130), (400, 200, 260))
    g.box(BULB, (120, 58, Z + 150), (90, 6, 90), glow=0.9)
    for sx in (-1, 1):
        tilt_box(g, CREAM, (sx * 230, 160, Z + 250), (0, 160, Z + 340), 240, 14)
    g.box(WOOD, (0, -150, Z2 - 12), (480, 260, 24))                      # the look-out: y -280 .. -20
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 70, -40, Z2 - 190), (sx * 220, -260, Z2 - 24), 14, 14)
        g.box(MAG, (sx * 235, -150, Z2 + 29), (10, 260, 58))
    parapet(g, -240, 240, -274, Z2, (MAG, CREAM), 4)
    bulbs(g, (-400, -352, Z - 34), (400, -352, Z - 34), 6, cols=(BULB, MAG, BULB, CYAN))
    for sx in (-1, 1):                                                     # the rope ladder
        tilt_box(g, CREAM, (-330 + sx * 26, -356, Z - 20), (-330 + sx * 26, -470, 0), 6, 6)
    for k in range(5):
        f = (k + 0.5) / 5.0
        g.box(WOOD, (-330, -356 - 114 * f, (Z - 20) * (1.0 - f)), (60, 8, 8))
    # the swing: a child on a plank, hung from a beam at the deck's side
    PV = (640.0, -100.0, Z - 40.0)
    g.box(WOOD_D, (540, -100, Z - 32), (300, 18, 18))
    sw = dict(swing=20.0, pivot=PV, axis=90.0)
    m = sc.part(P.motion(phase=0.1, rate=0.42, **sw))
    for sy in (-1, 1):
        m.box(CREAM, (640, -100 + sy * 24, (Z - 40 + 168) / 2.0), (6, 6, Z - 40 - 168))
    m.box(WOOD, (640, -100, 165), (40, 76, 10))
    P.figure(sc, 640, -100, z=135, yaw=0, legs='sit', seat=46, arm_l='vee', arm_r='vee', rate=0.42, phase=0.1, sway=sw, detail=0, **kid(r))
    row(sc, r, (-350, -210, -70, 70, 210, 350), -290, Z, (3, 0, 2, 1, 4, 6), kid_at=(1, 4), box_col=WOOD_D)
    row(sc, r, (-150, 0, 150), -225, Z2, (0, 5, 1), kid_at=(0,), box_col=MAG, big=1.2)


# ------------------------------------------------------------------------------------------ the rope bridge
def rope_bridge(sc):
    """Two wooden towers 16 m apart, each with a deck 6.8 m up under a pointed roof, and a plank bridge between them
    that sags a little: four walkers cross it (three one way, one the other; they travel the bridge and come round
    again), two guests wave from each tower; a striped cloth along the bridge's rail, pennants, lanterns."""
    r = random.Random(641)
    g = sc.still()
    Z, TX = 680.0, 800.0
    for sx in (-1, 1):
        cx = sx * TX
        for ax in (-1, 1):
            for ay in (-1, 1):
                g.box(WOOD_D, (cx + ax * 172, ay * 172, (Z + 250) / 2.0), (26, 26, Z + 250))
            tilt_box(g, WOOD, (cx + ax * 172, -176, 60), (cx - ax * 172, -176, Z - 60), 14, 14)
        g.box(WOOD, (cx, 0, Z - 12), (400, 400, 24))
        pc = (MAG, CREAM) if sx < 0 else (CYAN, WHITE)
        parapet(g, cx - 200, cx + 200, -194, Z, pc, 4)
        g.box(pc[0], (cx + sx * 195, 0, Z + 29), (10, 400, 58))
        g.cyl(pc[0], (cx, 0, Z + 250), 310, 190, steps=8, top_r=10)
        g.cyl(CREAM, (cx, 0, Z + 240), 318, 12, steps=8)
        bulbs(g, (cx - 180, -204, Z - 30), (cx + 180, -204, Z - 30), 3, cols=(BULB,))
        pennant(sc, cx, 0, Z + 430, 200, AMBER if sx < 0 else MAG, 0.25 + 0.25 * sx, w=140)
        tilt_box(g, CREAM, (cx + sx * 150, 180, Z - 20), (cx + sx * 150, 330, 0), 90, 12)        # the stair behind
    def sag(x):
        return Z - 8.0 - 28.0 * (1.0 - (x / 600.0) ** 2)
    cols = (AMBER, RED, WHITE, AMBER, RED, WHITE)
    for k in range(6):
        x0, x1 = -600.0 + k * 200.0, -400.0 + k * 200.0
        tilt_box(g, WOOD, (x0, 0, sag(x0) - 7), (x1, 0, sag(x1) - 7), 150, 14)
        tilt_box(g, cols[k], (x0, -78, sag(x0) + 30), (x1, -78, sag(x1) + 30), 8, 54)
        tilt_box(g, CREAM, (x0, 78, sag(x0) + 92), (x1, 78, sag(x1) + 92), 6, 6)
        tilt_box(g, CREAM, (x0, -78, sag(x0) + 92), (x1, -78, sag(x1) + 92), 6, 6)
    walk = dict(stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=1.0, bob=3, detail=0)
    WZ = Z - 28.0
    P.figure(sc, -430, -22, z=WZ, yaw=90, phase=0.1, travel=(1200, 62), **dict(guest(r, s=BIG), **walk))
    P.figure(sc, -40, -22, z=WZ, yaw=90, phase=0.5, travel=(1200, 62), arm_r=((120, 0), (160, 0)), hold_r=P.hold_flag(MAG, w=76),
             stride=24, swing_l=18, axis_l='x', rate=1.0, bob=3, detail=0, **kid(r, s=0.9))
    P.figure(sc, 330, -22, z=WZ, yaw=90, phase=0.8, travel=(1200, 62), **dict(guest(r, s=BIG), **walk))
    P.figure(sc, 120, 34, z=WZ, yaw=-90, phase=0.3, travel=(1200, -55), **dict(staff(r, s=BIG), **walk))
    row(sc, r, (-TX - 90, -TX + 90), -140, Z, (1, 0), box_col=MAG)
    row(sc, r, (TX - 90, TX + 90), -140, Z, (4, 2), kid_at=(0,), box_col=CYAN)


# ------------------------------------------------------------------------------------------ the lighthouse
def lighthouse(sc):
    """A red and white lighthouse 29 m tall on a rock: seven guests round its low gallery (7 m), five on the lantern
    gallery (24.6 m), drawn 1.6 times life size; a steady lamp under a pointed cap. Its foot goes 6 m under z = 0:
    over the lagoon it stands on the valley floor, 1.12 times its size, with the lantern gallery level with the rails."""
    r = random.Random(643)
    g = sc.still()
    g.cyl(PLUM, (0, 0, -600), 300, 600, steps=8)
    g.cyl(GREY_L, (0, 0, 0), 430, 120, steps=8, top_r=330)
    Z, Z2 = 700.0, 2460.0

    def rad(z):
        return 270.0 - (z - 120.0) / 2340.0 * 100.0
    for k in range(6):
        z0 = 120.0 + k * 390.0
        g.cyl(RED if k % 2 == 0 else WHITE, (0, 0, z0), rad(z0), 390, steps=10, top_r=rad(z0 + 390.0))
    g.box(NAVY, (0, -266, 230), (96, 12, 210))
    for z in (1160.0, 1780.0):
        g.box(BULB, (0, -rad(z) - 1, z), (60, 10, 90), glow=0.8)
    g.cyl(CREAM, (0, 0, Z - 24), 440, 24, steps=10)
    ring(g, 0, 0, Z, 432, 58, (RED, WHITE), -112, 112, 8)
    bulbs(g, (-300, -330, Z - 40), (300, -330, Z - 40), 4)
    round_row(sc, r, 0, 0, Z, 350, (-84, -56, -28, 0, 28, 56, 84), (1, 0, 3, 2, 6, 4, 5))
    g = sc.still()
    g.cyl(CREAM, (0, 0, Z2 - 24), 345, 24, steps=10)
    ring(g, 0, 0, Z2, 337, 62, (WHITE, RED), -112, 112, 8)
    g.cyl(BULB, (0, 0, Z2), 150, 250, steps=8, glow=1.0)                 # the lamp: steady
    for a in (45, 135, 225, 315):
        g.box(NAVY, (math.cos(math.radians(a)) * 150, math.sin(math.radians(a)) * 150, Z2 + 125), (16, 16, 250))
    g.cyl(RED, (0, 0, Z2 + 250), 235, 150, steps=8, top_r=12)
    g.sphere(GOLD, (0, 0, Z2 + 412), 22, steps=(3, 6))
    round_row(sc, r, 0, 0, Z2, 248, (-70, -35, 0, 35, 70), (0, 3, 1, 6, 4), big=1.6)


# ------------------------------------------------------------------------------------------ the crow's nest
def _sailor(r):
    return P.look(r, top=WHITE, bottom=NAVY, hat='cap', hat_col=RED, band=RED, sleeves='short', wear='trousers', shoes=DARK)


def crows_nest(sc):
    """A ship's mast 30 m tall on a piece of her deck: four guests at the deck's rail (3 m), three sailors in the low
    nest (8.2 m), three in the top one (24.8 m, 1.6 times life size: one with a telescope, one with a great flag, one
    waving); a striped sail between the yards sways, a pennant at the truck. A foot 6 m under z = 0, like the
    lighthouse: over the lagoon it stands on the valley floor."""
    r = random.Random(647)
    g = sc.still()
    DZ, Z, Z2 = 300.0, 820.0, 2480.0
    g.cyl(PLUM, (0, 0, -600), 220, 600, steps=8)
    g.box(WOOD, (0, 0, DZ / 2.0), (920, 380, DZ))                        # the deck: a block of hull
    g.box(RED, (0, -192, 236), (920, 6, 60))
    g.box(AMBER, (0, -192, 150), (920, 6, 22))
    parapet(g, -460, 460, -184, DZ, (WOOD_D, CREAM), 8)
    for sx in (-1, 1):
        g.box(WOOD_D, (sx * 455, 0, DZ + 29), (10, 380, 58))
    bulbs(g, (-420, -200, DZ - 30), (420, -200, DZ - 30), 6)
    g.cyl(WOOD_D, (0, 0, DZ), 42, 2700, steps=8, top_r=24)               # the mast
    for z, w in ((1250.0, 780.0), (2050.0, 620.0)):
        g.box(WOOD_D, (0, 0, z), (w, 18, 18))
        g.box(CREAM, (0, 0, z + 24), (w - 80, 36, 30))
    m = sc.part(P.motion(phase=0.4, rate=0.3, swing=4.0, pivot=(0.0, -14.0, 2040.0), axis=0.0))
    stripes(m, -280, 280, -14, 1330, 2036, (RED, CREAM), 4, depth=6)
    g = sc.still()
    g.cyl(WOOD, (0, 0, Z - 20), 195, 20, steps=10)
    ring(g, 0, 0, Z, 188, 72, (WOOD_D, RED), -180, 180, 10)
    g.cyl(WOOD, (0, 0, Z2 - 20), 215, 20, steps=10)
    ring(g, 0, 0, Z2, 208, 78, (RED, WOOD_D), -180, 180, 10)
    bulbs(g, (-150, -200, Z2 - 30), (150, -200, Z2 - 30), 3)
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 190, 0, Z - 20), (sx * 440, 0, DZ), 7, 7)
        tilt_box(g, WOOD_D, (sx * 210, 0, Z2 - 20), (sx * 300, 0, 2060), 7, 7)
    pennant(sc, 0, 0, 3000, 190, RED, 0.2, w=190)
    row(sc, r, (-350, -200, 200, 350), -128, DZ, (1, 0, 2, 4), kid_at=(1,), box_col=RED)
    round_row(sc, r, 0, 0, Z, 108, (-62, 0, 62), (1, 5, 0), look=_sailor)
    scope = dict(arm_r=((84, 80), (150, 96)), hold_r=hold_scope(), arm_l='hip', turn=30, rate=0.25)
    flag = dict(arm_r=((120, 0), (160, 0)), swing_r=22, hold_r=hold_bigflag(AMBER, h=230, w=130), arm_l='hip', bob=5, rate=0.9)
    wave = dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=10, rate=1.4)
    round_row(sc, r, 0, 0, Z2, 122, (-64, 0, 64), (0,), big=1.6, look=_sailor, kw=(flag, scope, wave))


# ------------------------------------------------------------------------------------------ the gantry
def gantry(sc):
    """Rocket Land's launch gantry: a steel tower with three decks (4.6, 8.6 and 12.6 m). Four guests on the lowest,
    three on the middle one, and on the top one the rocket's crew in white suits and helmets, waving, with one of the
    staff shaking a flag. Beside it the rocket on its pad (a ride: white, with red fins and nose, unlit but for a
    window), an arm across to it; lamps along the decks. Posts and pad go 3 m under z = 0."""
    r = random.Random(653)
    g = sc.still()
    levels = (460.0, 860.0, 1260.0)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(GREY_L, (sx * 230, 120 + sy * 190, 640), (30, 30, 1880))
    pcols = ((ORANGE, WHITE), (CYAN, WHITE), (ORANGE, WHITE))
    zp = 0.0
    for j, Z in enumerate(levels):
        g.box(CREAM, (0, -130, Z - 12), (640, 300, 24))                  # the deck: y -280 .. 20
        g.box(GREY_L, (0, 120, Z - 10), (490, 410, 16))
        parapet(g, -320, 320, -274, Z, pcols[j], 6)
        for sx in (-1, 1):
            g.box(pcols[j][0], (sx * 315, -130, Z + 29), (10, 300, 58))
        a, b = ((-230, 230) if j % 2 == 0 else (230, -230))
        tilt_box(g, ORANGE, (a, -74, zp + 20), (b, -74, Z - 30), 14, 14)
        bulbs(g, (-290, -284, Z - 30), (290, -284, Z - 30), 4)
        zp = Z
    g.box(ORANGE, (0, 120, 1570), (520, 440, 30))
    g.box(GREY_L, (0, 120, 1700), (12, 12, 240))
    g.box(BULB, (0, 120, 1830), (30, 30, 30), glow=1.2)
    # the rocket on its pad, an arm across to it from the middle deck
    RX, RY = 660.0, 60.0
    g.cyl(GREY_L, (RX, RY, -300), 240, 330, steps=8)
    for a in range(4):
        ca, sa = math.cos(math.radians(a * 90 + 45)), math.sin(math.radians(a * 90 + 45))
        g.box(RED, (RX + ca * 150, RY + sa * 150, 190), (70, 70, 300), rot=(0, a * 90 + 45, 0))
    g.cyl(WHITE, (RX, RY, 120), 125, 900, steps=10)
    g.cyl(CYAN, (RX, RY, 640), 129, 110, steps=10)
    g.cyl(RED, (RX, RY, 1020), 125, 300, steps=10, top_r=8)
    g.box(BULB, (RX, RY - 124, 840), (64, 8, 64), glow=0.8)
    g.box(ORANGE, (430, -130, 880), (230, 70, 16))
    g.box(ORANGE, (430, -162, 925), (230, 8, 8))
    row(sc, r, (-240, -80, 80, 240), -220, levels[0], LOW, kid_at=(2,), box_col=ORANGE)
    row(sc, r, (-200, 0, 200), -220, levels[1], (2, 4, 1))
    for i, x in enumerate((-190, 0)):
        d = P.look(r, top=WHITE, bottom=WHITE, hat='helmet', hat_col=WHITE, band=ORANGE, sleeves='long', shoes=ORANGE, pack=GREY_L, s=BIG)
        P.figure(sc, x, -220, z=levels[2], yaw=(-8, 6)[i], arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=(8, 4)[i], rate=(1.0, 1.3)[i],
                 phase=0.2 + 0.4 * i, react=True, detail=0, **d)
    watcher(sc, r, 200, -220, levels[2], 3, look=staff(r))


# ------------------------------------------------------------------------------------------ the roof party
def roof_party(sc):
    """A kiosk with a party on its flat roof 4.5 m up: six dancers and cheerers at the striped parapet, two more on a
    riser, the disc jockey at the booth between two loudspeakers; strings of steady coloured lamps on four poles, a
    mirror ball that turns to and fro, a giant ice-cream cone on the corner; the kiosk's lit counter and awning below."""
    r = random.Random(659)
    g = sc.still()
    Z, L, D = 450.0, 1300.0, 640.0
    g.box(TEAL, (0, 40, (Z - 24) / 2.0), (L - 120, D - 140, Z - 24))     # the kiosk: its front at y = -210
    g.box(BULB, (0, -214, 200), (L - 340, 6, 130), glow=0.9)
    g.box(CREAM, (0, -240, 118), (L - 300, 60, 16))
    aw = L - 220
    for k in range(8):
        x = -aw / 2.0 + (k + 0.5) * aw / 8.0
        tilt_box(g, RED if k % 2 else CREAM, (x, -208, 345), (x, -340, 296), aw / 8.0, 8)
    g.box(CREAM, (0, 0, Z - 12), (L, D, 24))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(CREAM, (sx * (L / 2.0 - 40), sy * (D / 2.0 - 40), 0), 24, Z - 24, steps=6)
            g.box(PLUM, (sx * (L / 2.0 - 16), sy * (D / 2.0 - 16), Z + 195), (10, 10, 390))
        g.box(MAG, (sx * (L / 2.0 - 5), 0, Z + 29), (10, D, 58))
        bulbs(g, (sx * (L / 2.0 - 16), -D / 2.0 + 16, Z + 380), (sx * (L / 2.0 - 16), D / 2.0 - 16, Z + 380), 5, cols=(BULB, CYAN, BULB, MAG))
    parapet(g, -L / 2.0, L / 2.0, -D / 2.0 + 6, Z, (MAG, CREAM), 10)
    bulbs(g, (-L / 2.0 + 16, -D / 2.0 + 16, Z + 380), (L / 2.0 - 16, -D / 2.0 + 16, Z + 380), 12, cols=(BULB, MAG, BULB, CYAN, BULB, AMBER))
    RZ = Z + 60.0
    g.box(PLUM, (0, 160, Z + 30), (L - 300, 240, 60))                     # the riser
    g.box(NAVY, (0, 120, RZ + 50), (300, 90, 100))                        # the booth
    for sx in (-1, 1):
        g.cyl(AMBER, (sx * 80, 120, RZ + 100), 44, 6, steps=8)
        g.box(DARK, (sx * 250, 190, RZ + 90), (110, 100, 180))
        g.box(CYAN, (sx * 250, 138, RZ + 110), (60, 6, 60), glow=0.4)
    P.figure(sc, 0, 215, z=RZ, arm_r='up', swing_r=20, arm_l='fwd', bob=6, rate=1.1, phase=0.3, detail=0,
             **staff(r, s=BIG, hat='headset', hat_col=DARK))
    # the mirror ball on its arm: it turns to and fro, four coloured tiles on it show the turning
    g.box(PLUM, (0, 300, Z + 280), (10, 10, 560))
    g.box(PLUM, (0, 145, RZ + 496), (10, 320, 10))
    BZ = RZ + 430.0
    g.box(PLUM, (0, -10, BZ + 34), (6, 6, 60))
    m = sc.part(P.motion(phase=0.0, rate=0.12, swing=170.0, pivot=(0.0, -10.0, BZ), axis=P.UP))
    m.sphere(GREY_L, (0, -10, BZ - 30), 52, steps=(4, 8), glow=0.5)
    for a in range(4):
        ca, sa = math.cos(math.radians(a * 90)), math.sin(math.radians(a * 90))
        m.box((CYAN, MAG, AMBER, WHITE)[a], (ca * 50, -10 + sa * 50, BZ - 30), (16, 16, 30), glow=0.9)
    # the giant ice-cream cone
    g = sc.still()
    cx, cy = L / 2.0 - 130, D / 2.0 - 120
    g.cyl(AMBER, (cx, cy, Z), 22, 240, steps=8, top_r=98)
    g.sphere(MAG, (cx, cy, Z + 300), 108, steps=(4, 8))
    g.sphere(MINT, (cx - 16, cy, Z + 430), 88, steps=(4, 8))
    g.sphere(CREAM, (cx + 6, cy, Z + 530), 64, steps=(4, 8))
    g.sphere(RED, (cx + 6, cy, Z + 600), 24, steps=(3, 6))
    y = -D / 2.0 + 60
    for i in range(6):
        x = -520 + i * 208 + r.uniform(-12, 12)
        if i % 2 == 0:
            d = guest(r, s=BIG)
            d.update(DANCE[(i // 2) % len(DANCE)])
            P.figure(sc, x, y, z=Z, yaw=r.uniform(-14, 14), phase=r.uniform(0.05, 0.95), react=True, detail=0, **d)
        else:
            watcher(sc, r, x, y, Z, (0, 6, 1)[i // 2])
    for sx in (-1, 1):
        d = guest(r, s=BIG)
        d.update(DANCE[2 if sx < 0 else 3])
        P.figure(sc, sx * 400, 90, z=RZ, yaw=-sx * 12, phase=r.uniform(0.05, 0.95), react=True, detail=0, **d)


JOBS = [
    ('SM_P3D_Balcony_A', lit(balcony_a)),
    ('SM_P3D_Balcony_B', lit(balcony_b)),
    ('SM_P3D_ClockTower', lit(clock_tower)),
    ('SM_P3D_Castle', lit(castle)),
    ('SM_P3D_TreeHouse', lit(tree_house)),
    ('SM_P3D_RopeBridge', lit(rope_bridge)),
    ('SM_P3D_Lighthouse', lit(lighthouse)),
    ('SM_P3D_CrowsNest', lit(crows_nest)),
    ('SM_P3D_Gantry', lit(gantry)),
    ('SM_P3D_RoofParty', lit(roof_party)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
