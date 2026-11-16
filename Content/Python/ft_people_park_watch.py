"""FULL THROTTLE (Unreal) the THEME PARK's WATCHERS (level index 4, Coaster Cove): /Game/People/ThemePark/SM_Watch...

The extra worker's kit for one focus: the guests who WATCH THE CHASE. The main kit (ft_people_park.py) has the park's
cast (parade, mascots, performers, staff, a stand under an awning, the theatre far off). This kit adds the places a
park's guests would crowd to when a car chase comes through, each one RAISED over the stalls' roofs (a stall is 3.2 m
to its roof and 4.7 m to its sign), massed, strongly coloured and moving:

    SM_WatchBridge_A     a queue footbridge 6.2 m up behind a row of stalls: eleven guests at a striped parapet (they
                         cheer, wave, film, point, shake flags and foam hands), seven more on a step behind them
    SM_WatchBridge_B     a shorter bridge with a striped pavilion roof: guests under it, two of them holding a striped
                         banner up between them, a child on a box
    SM_WatchTerrace      a food court's terrace on columns: guests with cups at the rail under parasols that sway, a
                         giant striped cup on the corner, a waiter going round with a tray, the kiosk under the deck
    SM_WatchPlatform     a coaster's station: the train held at the platform, its riders turned in their seats with
                         their arms up, the next riders at the platform's rail above them, filming
    SM_WatchTower        a look-out tower of three balconies (4.5, 7.6 and 10.7 m), four guests on each, a flag on top
    SM_WatchBleachers    the show bleachers, for 60 to 90 m: thirty people on five steep steps who hop in a wave that
                         runs along the stand, the two top rows lifting coloured cards (stripes of colour, no picture)

The park is a NIGHT level: like the main kit, every colour of a piece gets a steady glow of its own (lift()),
so a figure does not go black against the lit stalls. Every piece here is TALL and stands 24 m or more from the road's
centre line, where a steady light is allowed; nothing blinks. No clown (the boss is MR. CHUCKLES), no words, no marks.

Run in the editor:  import ft_people_park_watch; ft_people_park_watch.run()      (or run(['SM_WatchTower']))
The placing rules are the "watch" block of Source/FullThrottle/FTPeopleL4.inl.
"""
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
BRIGHT = (MAG, CYAN, AMBER, RED, ORANGE, VIOLET, LIME, WHITE)


# ------------------------------------------------------------------------------------------ helpers
def lift(sc, g, gp):
    """A steady glow on every colour of the piece, like a figure under stage light: the level is lit by night, and in
    the game's frames a figure in plain colours 40 m out was a dark speck beside the stalls' lamps. gp for everything
    that moves (the people, what they hold, the flags, the parasols), g for the structure they stand on (a third of
    it on its dark colours), so that the crowd is the brightest thing of its piece and not its parapet. Only for
    pieces that stand 24 m or more from the road (these stand 37 m out and more); steady, nothing blinks."""
    for key in sc.order:
        model, mo = sc.parts[key]
        moving = mo['rate'] != 0.0
        out = {}
        for (col, glow), pm in model.meshes.items():
            want = gp if moving else (g / 3.0 if col in (NAVY, PURPLE, PLUM, DARK) else g)
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


def guest(r, **fixed):
    d = P.look(r, tops=BRIGHT)
    d.update(fixed)
    return d


def kid(r, **fixed):
    d = P.look(r, tops=BRIGHT, build='kid', s=r.uniform(0.62, 0.7))
    d.update(fixed)
    return d


def pennant(sc, x, y, z0, h, col, phase, w=120.0):
    """A pole with a pennant that swings round it."""
    sc.still().box(CREAM, (x, y, z0 + h / 2.0), (8, 8, h))
    m = sc.part(P.motion(phase=phase, rate=0.8, swing=24.0, pivot=(x, y, z0 + h), axis=P.UP))
    m.box(col, (x + w / 2.0, y, z0 + h - 30), (w, 5, 56))


def hold_foam(col):
    """A big foam hand, one finger up."""
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 2, h[2] + 24), (32, 12, 46))
        m.box(col, (h[0], h[1] - 2, h[2] + 60), (12, 12, 30))
    return f


def hold_film(screen='#9ff0ff'):
    """A phone held up to film the chase: its lit screen faces the one who holds it, the road sees its dark back and a
    rim of light (a steady glow: these pieces stand 24 m out and more)."""
    def f(m, h, side):
        m.box(DARK, (h[0] - side * 10, h[1] - 6, h[2] + 12), (17, 5, 30))
        m.box(screen, (h[0] - side * 10, h[1] - 3, h[2] + 12), (21, 2, 34), glow=0.5)
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


BIG = 1.3     # the watchers are drawn a third over life size: at 37 m and more a life-size figure was a speck in the frames


def watcher(sc, r, x, y, z, k, yaw=0.0, child=False, detail=0, big=BIG, **kw):
    d = kid(r) if child else guest(r)
    d.update(act(r, k))
    d.update(kw)
    d['s'] = d.get('s', 1.0) * big
    return P.figure(sc, x, y, z=z, yaw=yaw + r.uniform(-12, 12), phase=r.random(), react=True, detail=detail, **d)


def deck(g, L, D, z, y=0.0, col=CREAM, edge=PLUM):
    g.box(col, (0, y, z - 12), (L, D, 24))
    g.box(edge, (0, y, z - 36), (L - 40, D - 70, 24))


def trestle(g, xs, D, z, y=0.0, col=PLUM, brace=LAV):
    for x in xs:
        for sy in (-1, 1):
            g.box(col, (x, y + sy * (D / 2.0 - 34), (z - 24) / 2.0), (26, 26, z - 24))
        tilt_box(g, brace, (x, y - (D / 2.0 - 34), 70), (x, y + D / 2.0 - 34, z - 90), 12, 12)


def parapet(g, x0, x1, y, z, cols, n, h=58.0):
    """A solid striped parapet: the piece's own banner, a band of colour under the watchers."""
    stripes(g, x0, x1, y, z, z + h, cols, n)
    g.box(CREAM, ((x0 + x1) / 2.0, y, z + h + 5), (x1 - x0 + 16, 18, 10))


def stair(g, x0, x1, y, z, w, col=CREAM, rail=MAG):
    tilt_box(g, col, (x0, y, z - 12), (x1, y, 0), w, 22)
    for sy in (-1, 1):
        tilt_box(g, rail, (x0, y + sy * (w / 2.0 - 6), z + 50), (x1, y + sy * (w / 2.0 - 6), 62), 8, 64)


# ------------------------------------------------------------------------------------------ the bridges
def bridge_a(sc):
    """A queue footbridge 6.2 m up, 15 m long, a stair at each end: eleven guests at a striped parapet cheer, wave,
    film, point, shake flags and foam hands; seven more on a step behind them hop with their arms up; pennants on
    four poles."""
    r = random.Random(401)
    g = sc.still()
    Z, L, D = 620.0, 1500.0, 280.0
    deck(g, L, D, Z)
    trestle(g, (-660, -220, 220, 660), D, Z)
    stair(g, L / 2.0, L / 2.0 + 430, 0, Z, D - 60)
    stair(g, -L / 2.0, -L / 2.0 - 430, 0, Z, D - 60)
    parapet(g, -L / 2.0, L / 2.0, -D / 2.0 + 6, Z, (MAG, CREAM), 10)
    g.box(PLUM, (0, 50, Z + 30), (L - 160, 90, 60))
    g.box(PLUM, (0, D / 2.0 - 6, Z + 92), (L, 8, 8))
    for k in range(4):
        x = -690 + k * 460
        g.box(PLUM, (x, D / 2.0 - 6, Z + 46), (8, 8, 92))
        pennant(sc, x, D / 2.0 - 6, Z + 92, 330, (CYAN, AMBER, MAG)[k % 3], k * 0.21)
    order = (0, 2, 1, 6, 3, 4, 2, 5, 0, 7, 1)
    for i, k in enumerate(order):
        x = -675 + i * 135 + r.uniform(-12, 12)
        if i == 5:
            g.box(TEAL, (x, -D / 2.0 + 56, Z + 22), (60, 50, 44))
            watcher(sc, r, x, -D / 2.0 + 56, Z + 44, k, child=True)
        else:
            watcher(sc, r, x, -D / 2.0 + 56, Z, k)
    for i in range(7):
        fan(sc, r, -570 + i * 190 + r.uniform(-20, 20), 50, Z + 60 + 83, r.random(), rate=0.7, bob=16.0, wave=(i % 3 == 1), legs=True, size=1.25)


def bridge_b(sc):
    """A bridge with a striped pavilion roof over its middle: guests under it and on both wings, two of them holding a
    striped banner up between them, a child on a box; one stair."""
    r = random.Random(409)
    g = sc.still()
    Z, L, D = 620.0, 1500.0, 300.0
    deck(g, L, D, Z, col=WHITE, edge=PURPLE)
    trestle(g, (-660, -220, 220, 660), D, Z, col=PURPLE, brace=MINT)
    stair(g, L / 2.0, L / 2.0 + 430, 0, Z, D - 60, col=WHITE, rail=CYAN)
    parapet(g, -L / 2.0, L / 2.0, -D / 2.0 + 6, Z, (CYAN, WHITE), 10)
    g.box(CYAN, (-L / 2.0 + 5, 0, Z + 29), (10, D, 58))
    # the pavilion: four posts, a roof of sloping stripes, a gold ball
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(PURPLE, (sx * 330, sy * (D / 2.0 - 10), Z + 180), (14, 14, 360))
    for k in range(6):
        x = -330 + (k + 0.5) * 110
        for sy in (-1, 1):
            tilt_box(g, MAG if k % 2 else CREAM, (x, 0, Z + 470), (x, sy * (D / 2.0 + 40), Z + 352), 110, 8)
    g.sphere(GOLD, (0, 0, Z + 492), 26, steps=(3, 6))
    pennant(sc, -L / 2.0 + 20, D / 2.0 - 10, Z, 420, AMBER, 0.1)
    pennant(sc, L / 2.0 - 20, D / 2.0 - 10, Z, 420, MAG, 0.6)
    y = -D / 2.0 + 58
    # the banner pair: both hop together, the banner with them
    ph = r.random()
    for sx in (-1, 1):
        P.figure(sc, sx * 195, y, z=Z, arm_l='up', arm_r='up', bob=10, rate=1.2, phase=ph, react=True, detail=0, **guest(r, s=BIG))
    bn = sc.part(P.motion(phase=ph, rate=1.2, bob=10.0, react=True))
    stripes(bn, -240, 240, y - 4, Z + 250, Z + 334, (RED, WHITE, AMBER, WHITE), 8, depth=5)
    order = (1, 6, 2, 0, 3, 4, 5, 2)
    xs = (-680, -540, -400, -280, 290, 420, 550, 680)
    for i, (x, k) in enumerate(zip(xs, order)):
        if i == 2:
            g.box(AMBER, (x, y, Z + 22), (60, 50, 44))
            watcher(sc, r, x, y, Z + 44, 0, child=True)
        else:
            watcher(sc, r, x + r.uniform(-10, 10), y, Z, k)
    watcher(sc, r, 0, y + 10, Z, 4)


# ------------------------------------------------------------------------------------------ the terrace
def terrace(sc):
    """A food court's terrace on six columns, its kiosk under the deck: nine guests at the rail (cups in their hands,
    waving, filming) under three parasols that sway; a giant striped cup on the corner; a waiter goes round."""
    r = random.Random(419)
    g = sc.still()
    Z, L, D = 600.0, 1500.0, 520.0
    deck(g, L, D, Z, col=CREAM, edge=TEAL)
    for x in (-680, 0, 680):
        for sy in (-1, 1):
            g.cyl(CREAM, (x, sy * (D / 2.0 - 50), 0), 30, Z - 24, steps=8)
            g.cyl(AMBER, (x, sy * (D / 2.0 - 50), Z - 110), 36, 50, steps=8)
    g.box(TEAL, (0, 70, 150), (900, 300, 300))
    g.box(BULB, (0, -84, 190), (760, 6, 110), glow=0.9)
    stripes(g, -470, 470, -110, 300, 330, (RED, CREAM), 8, depth=120)
    parapet(g, -L / 2.0, L / 2.0, -D / 2.0 + 6, Z, (AMBER, WHITE), 10)
    for sx in (-1, 1):
        g.box(AMBER, (sx * (L / 2.0 - 5), 0, Z + 29), (10, D, 58))
    stair(g, -L / 2.0, -L / 2.0 - 420, 120, Z, 200, rail=TEAL)
    for k, x in enumerate((-470, 0, 470)):
        m = sc.part(P.motion(phase=k * 0.31, rate=0.3, swing=3.5, pivot=(x, 60.0, Z), axis=0.0))
        m.box(CREAM, (x, 60, Z + 135), (9, 9, 270))
        m.cyl((MAG, CYAN, MAG)[k], (x, 60, Z + 262), 185, 74, steps=8, top_r=10)
        m.cyl(CREAM, (x, 60, Z + 254), 190, 9, steps=8)
        g.cyl(WHITE, (x, 60, Z + 70), 56, 7, steps=8)
        g.box(PLUM, (x, 60, Z + 35), (10, 10, 70))
    # the giant cup: red and white bands, a straw
    cx, cy = L / 2.0 - 110, D / 2.0 - 110
    for j in range(4):
        g.cyl(RED if j % 2 else WHITE, (cx, cy, Z + j * 62), 66 + j * 7, 62, steps=8, top_r=73 + j * 7)
    g.cyl(CREAM, (cx, cy, Z + 248), 98, 12, steps=8)
    tilt_box(g, CYAN, (cx + 10, cy, Z + 250), (cx + 70, cy, Z + 400), 14, 14)
    drink = dict(arm_r='face', hold_r=P.hold_cup(RED), arm_l='wave', swing_l=24, bob=4, rate=1.2)
    order = (0, 'd', 2, 3, 'd', 4, 6, 2, 1)
    for i, k in enumerate(order):
        x = -640 + i * 160 + r.uniform(-12, 12)
        if k == 'd':
            P.figure(sc, x, -D / 2.0 + 58, z=Z, yaw=r.uniform(-12, 12), phase=r.random(), react=True, detail=0, **dict(guest(r, s=BIG), **drink))
        else:
            watcher(sc, r, x, -D / 2.0 + 58, Z, k)

    def tray(m, h, side):
        if side > 0:
            m.cyl(GREY_L, (h[0] - 24, h[1] - 8, h[2] + 8), 30, 5, steps=8)
            m.cyl(RED, (h[0] - 30, h[1] - 8, h[2] + 13), 7, 18, steps=6)
            m.cyl(AMBER, (h[0] - 14, h[1] - 8, h[2] + 13), 7, 18, steps=6)
    P.figure(sc, -300, 170, z=Z, yaw=90, stride=22, arm_l='carry', arm_r='carry', hold_r=tray, rate=1.4, bob=2, phase=0.2,
             travel=(L - 300, 90), detail=0, **P.look(r, top=MINT, bottom=NAVY, hat='cap', hat_col=MAG, band=GOLD, shoes=WHITE))


# ------------------------------------------------------------------------------------------ the station
def platform(sc):
    """A coaster's station: the train is held at the platform, its six riders turned in their seats with their arms
    up; the next riders stand at the platform's rail above them, filming and cheering; a striped roof."""
    r = random.Random(431)
    g = sc.still()
    Z, L = 540.0, 1500.0
    TY, PY, PZ = -150.0, 110.0, Z + 96.0
    # the track on its girder, the platform behind and above it
    g.box(PLUM, (0, TY, Z - 62), (L + 500, 70, 44))
    for sy in (-1, 1):
        g.box(CYAN, (0, TY + sy * 42, Z - 34), (L + 500, 12, 12))
    for x in (-900, -300, 300, 900):
        g.box(PURPLE, (x, TY, (Z - 84) / 2.0), (30, 44, Z - 84))
    deck(g, L, 300.0, PZ, y=PY, col=CREAM, edge=PURPLE)
    trestle(g, (-660, 0, 660), 300.0, PZ, y=PY, col=PURPLE, brace=TEAL)
    parapet(g, -L / 2.0, L / 2.0, PY - 144, PZ, (TEAL, CREAM), 10)
    for sx in (-1, 1):
        for x in (sx * 700, sx * 240):
            g.box(PURPLE, (x, PY + 135, PZ + 170), (14, 14, 340))
    for k in range(10):
        x = -L / 2.0 + (k + 0.5) * L / 10.0
        tilt_box(g, ORANGE if k % 2 else CREAM, (x, PY + 150, PZ + 372), (x, PY - 170, PZ + 318), L / 10.0, 8)
    stair(g, L / 2.0, L / 2.0 + 430, PY, PZ, 220, rail=ORANGE)
    # the train: three cars, two riders each
    for j, x in enumerate((-340, 0, 340)):
        g.box(RED, (x, TY, Z - 2), (300, 150, 64))
        g.box(AMBER, (x, TY - 77, Z - 2), (250, 6, 30))
        g.box(DARK, (x + 150, TY, Z - 14), (40, 60, 20))
        g.box(RED, (x - 130, TY, Z + 52), (30, 140, 60))
        for sx in (-1, 1):
            d = guest(r) if (j + sx) % 3 else kid(r, s=0.8)
            d.update(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, rate=1.5 + 0.2 * j)
            P.figure(sc, x + sx * 62 + 20, TY, z=Z - 30, yaw=r.uniform(-30, 30), legs='sit', seat=30, phase=r.random(), react=True, detail=0, **d)
    tilt_box(g, AMBER, (490, TY, Z - 10), (590, TY, Z - 34), 150, 30)
    order = (2, 0, 3, 2, 6, 1)
    for i, k in enumerate(order):
        watcher(sc, r, -600 + i * 240 + r.uniform(-16, 16), PY - 92, PZ, k)


# ------------------------------------------------------------------------------------------ the tower
def tower(sc):
    """A look-out tower: three balconies one over the other (4.5, 7.6 and 10.7 m), each with its own striped parapet
    and four guests at it; a pointed roof and a flag that swings."""
    r = random.Random(443)
    g = sc.still()
    W, D = 580.0, 380.0
    g.box(NAVY, (0, 60, 680), (170, 170, 1360))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(PLUM, (sx * (W / 2.0 - 14), -20 + sy * (D / 2.0 - 14), 690), (20, 20, 1380))
    cols = ((MAG, CREAM), (CYAN, WHITE), (AMBER, WHITE))
    k = 0
    for j, Z in enumerate((450.0, 760.0, 1070.0)):
        g.box(CREAM, (0, -20, Z - 12), (W, D, 24))
        parapet(g, -W / 2.0, W / 2.0, -20 - D / 2.0 + 6, Z, cols[j], 6)
        for sx in (-1, 1):
            g.box(cols[j][0], (sx * (W / 2.0 - 5), -20, Z + 29), (10, D, 58))
        for i in range(4):
            x = -204 + i * 136 + r.uniform(-10, 10)
            watcher(sc, r, x, -20 - D / 2.0 + 58, Z, (0, 1, 2, 4, 4, 1, 5, 0, 2, 5, 0, 1)[k], big=1.2)   # no flags or foam hands here: the next balcony is 2.9 m overhead
            k += 1
    g.cyl(MAG, (0, -20, 1380), 400, 190, steps=8, top_r=12)
    g.cyl(CREAM, (0, -20, 1370), 410, 12, steps=8)
    g.box(CREAM, (0, -20, 1680), (8, 8, 240))
    m = sc.part(P.motion(phase=0.3, rate=0.7, swing=26.0, pivot=(0.0, -20.0, 1700.0), axis=P.UP))
    m.box(AMBER, (80, -20, 1760), (160, 5, 70))
    # the stair up the side, as two flights with a landing each and rails in the balconies' colours
    for sx, y0, y1, z0, z1, c in ((1, 150, -190, 0.0, 438.0, MAG), (-1, -190, 150, 450.0, 748.0, CYAN)):
        x = sx * (W / 2.0 + 60)
        tilt_box(g, CREAM, (x, y0, z0), (x, y1, z1), 100, 16)
        for e in (-1, 1):
            tilt_box(g, c, (x + e * 46, y0, z0 + 62), (x + e * 46, y1, z1 + 62), 8, 60)
        g.box(CREAM, (x, y1 + (-40 if y1 < y0 else 40), z1 + 4), (100, 90, 16))


# ------------------------------------------------------------------------------------------ the bleachers
def fan(sc, r, x, y, z, phase, rate=0.5, bob=26.0, card=None, wave=False, legs=False, size=1.0):
    """A light member of a big crowd, seen from the waist up behind the row in front (about 70 triangles): a shirt, a
    head and its hair, both arms up; card: a coloured card held over the head; wave: the right arm swings."""
    top, skin, hair = r.choice(BRIGHT), r.choice(P.SKINS), r.choice(P.HAIRS)
    s = r.uniform(0.92, 1.06) * size
    mo = dict(phase=phase, rate=rate, bob=bob, react=True)
    m = sc.part(P.motion(**mo), (x, y, z), r.uniform(-10, 10), s)
    m.box(top, (0, 0, 36), (48, 28, 72))
    if legs:
        m.box(r.choice(P.BOTTOMS), (0, 0, -33), (36, 26, 66))
    m.box(skin, (0, 0, 94), (38, 34, 42))
    m.box(hair, (0, 3, 120), (42, 38, 13))
    for side in (-1, 1):
        if wave and side > 0:
            a = sc.part(P.motion(swing=26.0, pivot=(x + 28 * s, y, z + 64 * s), axis=90.0, **mo), (x, y, z), 0.0, s)
            tilt_box(a, top, (28, 0, 64), (44, -4, 124), 12, 12)
        else:
            tilt_box(m, top, (side * 28, 0, 64), (side * (34 if card else 46), -6, 124), 12, 12)
    if card:
        m.box(card, (0, -9, 148), (78, 5, 50))


def bleachers(sc):
    """The show bleachers: five steep steps 12 m wide, the lowest 1.7 m up, thirty people who hop in a wave that runs
    along the stand; the two top rows lift coloured cards; a back board with three big discs, lamps along its top,
    two pennants. For 60 to 90 m from the road, at 1.2 to 1.5 times its size."""
    r = random.Random(457)
    g = sc.still()
    rows, cols, W = 5, 6, 1200.0
    for k in range(rows):
        h = 170 + k * 75
        g.box(PURPLE if k % 2 else PLUM, (0, -180 + k * 90, h / 2.0), (W, 90, h))
    stripes(g, -W / 2.0, W / 2.0, -228, 8, 166, (CYAN, WHITE), 8)
    for sx in (-1, 1):
        g.box(NAVY, (sx * (W / 2.0 + 12), 20, 260), (24, 500, 520))
    g.box(NAVY, (0, 252, 450), (W + 48, 24, 900))
    for j, c in enumerate((CYAN, AMBER, MAG)):
        g.cyl(c, (-380 + j * 380, 240, 800), 74, 8, rot=(0, 0, 90), steps=10, glow=0.25)
    for j in range(8):
        g.box(BULB, (-W / 2.0 + (j + 0.5) * W / 8.0, 236, 892), (26, 14, 22), glow=1.3)
    pennant(sc, -W / 2.0 - 12, 250, 520, 560, MAG, 0.0, w=150)
    pennant(sc, W / 2.0 + 12, 250, 520, 560, AMBER, 0.5, w=150)
    cards = (MAG, AMBER, CYAN)
    for k in range(rows):
        for c in range(cols):
            x = -W / 2.0 + (c + 0.5) * W / cols + ((k % 2) * 50 - 25) + r.uniform(-14, 14)
            fan(sc, r, x, -180 + k * 90, 170 + k * 75, c / float(cols) + r.uniform(-0.05, 0.05),
                card=(cards[(c + k) % 3] if k >= 3 else None), wave=(k < 3 and (c + k) % 3 == 0))


JOBS = [
    ('SM_WatchBridge_A', lit(bridge_a)),
    ('SM_WatchBridge_B', lit(bridge_b)),
    ('SM_WatchTerrace', lit(terrace)),
    ('SM_WatchPlatform', lit(platform)),
    ('SM_WatchTower', lit(tower)),
    ('SM_WatchBleachers', lit(bleachers)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
