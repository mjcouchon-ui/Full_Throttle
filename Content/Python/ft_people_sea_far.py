"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea: FAR LIFE AND CREATURES (/Game/People/UnderTheSea/SM_Far*).

The extra worker's kit for this level (focus "far"): what lives from 150 m out to the trench walls and over them.
The main kit (ft_people_sea.py) has the aquanauts by the road, the turtles, the dolphins, the octopus, the giant
jellyfish, the seahorse patrol and the cargo convoy; the dressing (ft_dress_abyss.py) has the schools, the rays, the
sharks and the whales. None of those is copied here. This kit adds:

    SM_FarSquid        one giant squid, 34 m long, coral red, jetting mantle-first along the trench, arms writhing
    SM_FarDomeWorks    a dome of Nereida being built far out: a crane swinging a panel, aquanauts three times life
                       size welding on its ribs (steady light), a line of them going up to a buoy, scooters circling
    SM_FarNautilus     a shoal of giant nautiluses, striped shells 5 to 8 m across, rising and sinking out of step
    SM_FarCrabMarch    giant spider crabs on the march along the trench wall, legs working, claws raised
    SM_FarSeaDragon    an oarfish, the "sea dragon": a silver ribbon 60 m long with a red crest, a wave running down it
    SM_FarScooterWing  a far flight of aquanauts on scooters in a vee, streamers behind them, travelling

Everything is BIG (three times life and more) and in the level's hot colours (coral, yellow, pink, white, lime: never
grey with a white belly and a tall back fin, never the blue of the boss's torpedo fish), and every mesh is saved with
far=True (the always-moving master), because it stands past 150 m.

How a whole thing moves here (learned from the shader, ft_people.py): a part's hop / float and its swing use the
part's own rate and phase, so two parts that must stay together cannot float at different rates. What moves a whole
creature in one piece is travel and orbit (they depend only on the base and the round rate): so the creatures of this
kit travel or circle as a whole, and their limbs swing on top of that.

A piece is drawn like every prop: cm; z = 0 is the land it stands on; the middle of the footprint is the origin; the
FRONT faces -Y (the road); long pieces run along X (the road).

Run in the editor:  import ft_people_sea_far; ft_people_sea_far.run()     (or run(['SM_FarSquid']))
The placing rules: the "far" block of Source/FullThrottle/FTPeopleL6.inl.
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
ORANGE, PINK, LIME, RED = '#ff7a3c', '#ff5fa2', '#b8ff5a', '#ff2d3c'
GLASS, GLASS_D, DECK, DECK_D = '#bfeef2', '#7fc4d0', '#cfe3ea', '#7fa2b4'
BUBBLE = '#e6f6ff'
SPARK = '#fff1c0'


def cone(m, col, a, b, r0, r1, steps=8, glow=0.0):
    """A cylinder from a to b, r0 wide at a and r1 at b."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def _tv(travel):
    """(length cm, speed cm/s) -> the motion's (length, rounds a second)."""
    return (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)


# ------------------------------------------------------------------------------------------ a far aquanaut
def diver(sc, x, y, z, heading=0.0, pitch=0.0, s=3.0, suit=SUITS[0], fins=FINS[0], phase=0.0, rate=0.5, bob=-30.0, kick=16.0,
          base=None, travel=(0.0, 0.0), scooter=None, streamer=None, spark=False, arms='fwd', chunk=1.0, ribbons=4):
    """An aquanaut of Nereida for the far water, belly down, the hips at (x, y, z), the head towards `heading` (degrees,
    0 = +X) and raised by `pitch` (90: upright in the water). s = 3: three times life (5 m), which is what reads at
    200 m. travel: the motion's own (length, rounds a second); base: the group's base (so that a flight keeps its
    shape) or an orbit's centre. scooter: a colour (a scooter in the hands); streamer: a colour (a ribbon behind the
    fins); spark: a steady welding light at the hands. chunk: how much deeper than life the body, the helmet, the
    limbs and the scooter are drawn (seen side-on from the road a swimmer of true proportions is a thin line: the review);
    ribbons: the streamer's pieces. About 120 triangles."""
    h, p = math.radians(heading), math.radians(pitch)
    f = (math.cos(h) * math.cos(p), math.sin(h) * math.cos(p), math.sin(p))
    rt = (-math.sin(h), math.cos(h), 0.0)
    u = (-math.cos(h) * math.sin(p), -math.sin(h) * math.sin(p), math.cos(p))

    def W(a, b=0.0, c=0.0):
        a, b, c = a * s, b * s, c * s
        return (x + f[0] * a + rt[0] * b + u[0] * c, y + f[1] * a + rt[1] * b + u[1] * c, z + f[2] * a + rt[2] * b + u[2] * c)

    common = dict(phase=phase, rate=rate, bob=bob, base=base or (x, y), travel=travel)
    piv = (x, y, z)
    b = sc.part(P.motion(pivot=piv, **common))
    c = chunk
    tilt_box(b, suit, W(-6), W(62), 48 * s, 28 * s * c)
    tilt_box(b, STEEL if c == 1.0 else YEL, W(4, 0, 14 + 8 * c), W(52, 0, 14 + 8 * c), 22 * s, 18 * s * c)
    cone(b, HELM, W(62, 0, 4), W(112, 0, 4), 27 * s * c, 22 * s * c, steps=6)
    tilt_box(b, VISOR, W(98, 0, -12 * c), W(116, 0, -4 * c), 36 * s * c, 18 * s * c)
    for sd in (-1, 1):
        hand = W(120, sd * 20, -4) if arms == 'fwd' else W(96, sd * 26, -52)
        if c != 1.0:
            hand = W(120, sd * 20, -30 * c)
        tilt_box(b, suit, W(52, sd * 28, 0), hand, 14 * s, 14 * s * c)
    if scooter:
        cone(b, scooter, W(104, 0, -34 * c), W(196, 0, -34 * c), 20 * s * c, 14 * s * c, steps=6)
        cone(b, DARK, W(88, 0, -34 * c), W(104, 0, -34 * c), 27 * s * c, 27 * s * c, steps=6)
    if spark:
        b.sphere(SPARK, W(104, 0, -64), 20 * s, steps=(3, 5), glow=1.0)
    for sd in (-1, 1):
        hip = W(-4, sd * 12, 0)
        m = sc.part(P.motion(swing=sd * kick, pivot=hip, axis=heading + 90.0, **common)) if kick else b
        tilt_box(m, suit, hip, W(-80, sd * 13, 0), 18 * s, 18 * s * c)
        tilt_box(m, fins, W(-76, sd * 14, 0), W(-136, sd * 16, -4), 34 * s, 7 * s * c * c)
    if streamer:
        for k in range(ribbons):
            st = sc.part(P.motion(swing=10.0 + 4.0 * k, pivot=W(-140), axis=(P.UP if c == 1.0 else heading + 90.0), **common))
            tilt_box(st, streamer if k % 2 == 0 else WHITE, W(-140 - k * 90, 0, 6), W(-232 - k * 90, 0, 6), 6 * s, 30 * s * c)
    return W


# ------------------------------------------------------------------------------------------ the giant squid
def squid(seed=1, body='#ff5a4a', pale='#ffd2c4', spot='#ffc93c', sign=1, length=6000.0, speed=300.0, z0=2400.0):
    """One giant squid, 34 m from the tip of the mantle to the clubs of the long arms, jetting mantle-first along the
    trench (it travels 60 m of the mesh's X and comes round again: short, so that at
    twice its size it stays well clear of the road), eight arms and two long tentacles writhing behind it.
    One great animal: coral red with a pale belly, yellow spots, an eye a metre and a half across."""
    def build(sc):
        r = random.Random(seed)
        tv = _tv((length, sign * speed))
        base = (0.0, 0.0)
        X = lambda v: v * sign          # the mantle leads
        b = sc.part(P.motion(pivot=(0.0, 0.0, z0), base=base, travel=tv))
        cone(b, body, (X(-40), 0, z0), (X(1500), 0, z0 + 60), 270, 80, steps=8)
        b.sphere(body, (X(1500), 0, z0 + 60), 82, steps=(3, 6))
        tilt_box(b, pale, (X(60), 0, z0 - 190), (X(1250), 0, z0 - 60), 300, 70)
        b.box(body, (X(1230), 0, z0 + 50), (640, 640, 30), rot=(0, 45, 0))          # the fin: a diamond across the tip
        b.box(pale, (X(1230), 0, z0 + 68), (360, 360, 12), rot=(0, 45, 0))
        for k in range(3):                                                          # bands round the mantle
            xx = 250 + k * 380
            rr = 270 - (xx + 40) * 190.0 / 1540.0
            cone(b, pale, (X(xx), 0, z0 + xx * 0.04), (X(xx + 70), 0, z0 + xx * 0.04), rr + 8, rr, steps=8)
        cone(b, body, (X(-330), 0, z0 - 10), (X(-20), 0, z0), 190, 240, steps=8)    # the head
        for sd in (-1, 1):
            b.sphere(WHITE, (X(-170), sd * 190, z0 + 20), 120, scale=(1, 0.6, 1), steps=(4, 8))
            b.sphere(DARK, (X(-170), sd * 250, z0 + 20), 62, scale=(1, 0.5, 1), steps=(3, 6))
        for k in range(7):
            b.sphere(spot, (X(200 + k * 170), -(250 - k * 24) * 0.86, z0 + 90 + (k % 2) * 70), 44, scale=(1, 0.4, 1), steps=(3, 5))
            b.sphere(spot, (X(280 + k * 170), (250 - k * 24) * 0.86, z0 + 90 + (k % 2) * 70), 44, scale=(1, 0.4, 1), steps=(3, 5))
        # eight arms trailing from the head
        for k in range(8):
            a = math.radians(22.5 + k * 45.0)
            cy_, cz_ = math.cos(a), math.sin(a)
            root = (X(-320), cy_ * 120, z0 - 10 + cz_ * 120)
            updown = abs(cz_) > abs(cy_)
            ph, rate = r.random(), r.uniform(0.32, 0.5)
            reach = r.uniform(900, 1150)
            pts = [root]
            for j in range(1, 5):
                t = j / 4.0
                pts.append((root[0] - X(reach * t), root[1] + cy_ * 330 * t * t + cy_ * 60 * t, root[2] + cz_ * 330 * t * t + cz_ * 60 * t))
            for j in range(4):
                m = sc.part(P.motion(phase=ph - j * 0.09, rate=rate, swing=(7.0 + 4.0 * j) * (1 if k % 2 else -1), pivot=root,
                                     axis=(90.0 if updown else P.UP), base=base, travel=tv))
                w = 84 - j * 16
                tilt_box(m, body if j < 3 else pale, pts[j], pts[j + 1], w, w)
        # the two long tentacles and their clubs
        for sd in (-1, 1):
            root = (X(-320), sd * 60, z0 - 60)
            ph = r.random()
            pts = [root] + [(root[0] - X(380 * j), root[1] + sd * (40 * j + 14 * j * j), root[2] - 30 * j + 50 * math.sin(j * 1.3)) for j in range(1, 6)]
            for j in range(5):
                m = sc.part(P.motion(phase=ph - j * 0.1, rate=0.3, swing=(5.0 + 2.5 * j) * sd, pivot=root, axis=(P.UP if j % 2 else 90.0), base=base, travel=tv))
                tilt_box(m, body, pts[j], pts[j + 1], 46, 46)
                if j == 4:
                    tilt_box(m, pale, pts[5], (pts[5][0] - X(380), pts[5][1] + sd * 30, pts[5][2]), 190, 60)
                    tilt_box(m, body, (pts[5][0] - X(60), pts[5][1], pts[5][2] + 34), (pts[5][0] - X(330), pts[5][1] + sd * 26, pts[5][2] + 34), 110, 14)
        # the jet: bubbles left behind the head
        for k in range(5):
            bb = sc.part(P.motion(phase=k * 0.21, rate=0.35, bob=-70.0, pivot=(X(-700 - k * 420), 0, z0 + 200), base=base, travel=tv))
            bb.sphere(BUBBLE, (X(-700 - k * 420), (k % 2) * 160 - 80, z0 + 260 + k * 90), 46 + 12 * k, steps=(3, 6))
    return build


# ------------------------------------------------------------------------------------------ a dome being built
def dome_works(seed=1):
    """Far out, a new dome of Nereida going up: the glass half closed, its ribs bare on the road's side, a crane
    swinging a panel over it, aquanauts three times life size at work on the ribs (two of them welding: a steady
    light), a line of them on a rope going up to a buoy 45 m over the sand, and scooters circling the works."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        R = 1700.0
        g.cyl(DECK_D, (0, 0, -900), R + 90, 1000, steps=12)                         # the footing (the land may fall away under it)
        g.cyl(DECK, (0, 0, 100), R + 40, 70, steps=12)
        g.sphere(GLASS, (0, 260, 100), R - 40, scale=(1, 0.82, 0.92), steps=(6, 12))   # the half that is closed, set back
        g.sphere(GLASS_D, (0, 0, 100), R - 260, scale=(1, 1, 0.9), steps=(5, 10))      # the dark inside behind the bare ribs
        for k in range(7):                                                          # the bare ribs on the road's side
            a = math.radians(-160 + k * 23.3)
            pts = []
            for j in range(6):
                e = math.radians(j * 18.0)
                pts.append((math.cos(a) * math.cos(e) * R, math.sin(a) * math.cos(e) * R * 1.0, 100 + math.sin(e) * R * 0.95))
            for j in range(5):
                tilt_box(g, ORANGE if k % 2 == 0 else YEL, pts[j], pts[j + 1], 70, 70)
        for e_deg in (30.0, 62.0):                                                  # two rings
            e = math.radians(e_deg)
            for k in range(8):
                a0, a1 = math.radians(-170 + k * 20), math.radians(-150 + k * 20)
                tilt_box(g, WHITE, (math.cos(a0) * math.cos(e) * R, math.sin(a0) * math.cos(e) * R, 100 + math.sin(e) * R * 0.95),
                         (math.cos(a1) * math.cos(e) * R, math.sin(a1) * math.cos(e) * R, 100 + math.sin(e) * R * 0.95), 50, 50)
        # the crane: a mast beside the dome, its jib swinging a panel over the ribs
        mx = -R - 500.0
        g.box(YEL, (mx, 0, 1700), (150, 150, 3400))
        for k in range(6):
            g.box(DARK, (mx, 0, 300 + k * 560), (170, 170, 70))
        g.box(DARK, (mx, 0, -200), (500, 500, 500))
        piv = (mx, 0.0, 3300.0)
        jib = sc.part(P.motion(rate=0.06, swing=22.0, pivot=piv, axis=P.UP))
        tilt_box(jib, YEL, (mx - 700, 0, 3340), (mx + 2700, 0, 3340), 110, 150)
        jib.box(DARK, (mx - 640, 0, 3260), (360, 260, 300))
        jib.box(STEEL, (mx + 2300, 0, 2890), (14, 14, 800))
        jib.box(PINK, (mx + 2300, 0, 2360), (760, 40, 520))
        jib.box(WHITE, (mx + 2300, -24, 2360), (520, 10, 150))
        # the hands on the ribs: upright in the water, floating out of step
        spots = ((-0.30, 20.0, True), (-0.12, 44.0, False), (0.02, 28.0, True), (0.18, 52.0, False), (0.33, 22.0, False))
        for k, (fa, e_deg, weld) in enumerate(spots):
            a = math.radians(-90 + fa * 180.0)
            e = math.radians(e_deg)
            px, py, pz = math.cos(a) * math.cos(e) * (R + 150), math.sin(a) * math.cos(e) * (R + 150), 100 + math.sin(e) * R * 0.95 + 60
            diver(sc, px, py, pz, heading=math.degrees(a) + 180.0, pitch=58.0, s=3.0, suit=SUITS[(k * 2 + seed) % len(SUITS)], fins=FINS[(k + seed) % len(FINS)],
                  phase=r.random(), rate=r.uniform(0.3, 0.45), bob=-36.0, kick=10.0, spark=weld, arms='down')
        # the rope to the buoy, and the line of aquanauts on it
        top = (0.0, -200.0, 100 + R * 0.95)
        buoy = (1500.0, -700.0, 4400.0)
        tilt_box(g, WHITE, top, buoy, 14, 14)
        fl = sc.part(P.motion(rate=0.2, bob=-40.0, pivot=buoy))
        fl.sphere(ORANGE, buoy, 230, scale=(1, 1, 0.8), steps=(4, 8))
        fl.cyl(WHITE, (buoy[0], buoy[1], buoy[2] - 40), 240, 80, steps=8)
        for k in range(4):
            t = 0.2 + k * 0.2
            px, py, pz = (top[0] + (buoy[0] - top[0]) * t, top[1] + (buoy[1] - top[1]) * t - 110, top[2] + (buoy[2] - top[2]) * t)
            diver(sc, px, py, pz, heading=-18.0, pitch=58.0, s=3.0, suit=SUITS[(k * 3 + 1 + seed) % len(SUITS)], fins=FINS[(k + 2) % len(FINS)],
                  phase=k * 0.22, rate=0.4, bob=-60.0, kick=14.0)
        # scooters circling the works
        for k, (rad, zz, a_deg) in enumerate(((2700.0, 1500.0, 200.0), (3000.0, 2500.0, 320.0), (2500.0, 3300.0, 80.0))):
            a = math.radians(a_deg)
            diver(sc, math.cos(a) * rad, math.sin(a) * rad, zz, heading=a_deg + 90.0, pitch=0.0, s=3.0, suit=SUITS[(k * 3 + seed) % len(SUITS)],
                  fins=FINS[(k + 1) % len(FINS)], phase=r.random(), rate=0.8, bob=0.0, kick=0.0, base=(0.0, 0.0), travel=(-1.0, 0.035),
                  scooter=YEL, streamer=(PINK, LIME, ORANGE)[k], chunk=1.8, ribbons=2)
        # bubbles from the works
        for k in range(6):
            bb = sc.part(P.motion(phase=k * 0.17, rate=0.25, bob=-160.0, pivot=(-400 + k * 260, -900, 2000)))
            bb.sphere(BUBBLE, (-600 + k * 300 + r.uniform(-80, 80), -1100 + r.uniform(-200, 200), 1900 + k * 330), 50 + 14 * k, steps=(3, 6))
    return build

# ------------------------------------------------------------------------------------------ the nautilus shoal
def _nautilus(sc, x, y, z, R, sign, shell, stripe, flesh, phase, rate, bob):
    """One giant nautilus seen from the side: the striped disc of the shell (its axis across the road), the hood, the
    eye, the bunch of arms. It floats up and down and rocks about the middle of its shell."""
    c = (x, y, z)
    mo = dict(phase=phase, rate=rate, bob=bob, pivot=c, axis=90.0)
    b = sc.part(P.motion(swing=9.0, **mo))
    T = R * 0.5
    rod(b, shell, (x, y - T / 2.0, z), (x, y + T / 2.0, z), R, steps=10)
    # the coil: one thick band in the stripe's colour, wound a turn and a half from the middle out to the mouth of the
    # shell, standing proud of both faces (the review: a plain disc with a dot in it read as a fried egg from the road)
    n = 11
    pts = []
    for k in range(n + 1):
        t = k / float(n)
        rr = R * 0.15 * math.exp(1.74 * t)
        a = math.radians(-25.0 + (1.0 - t) * 540.0)
        pts.append((x + sign * math.cos(a) * rr, y, z + math.sin(a) * rr, rr))
    for k in range(n):
        tilt_box(b, stripe, pts[k][:3], pts[k + 1][:3], T * 1.16, max(R * 0.09, (pts[k][3] + pts[k + 1][3]) * 0.16))
    hx = x + sign * R * 0.86
    b.box(stripe, (hx, y, z - R * 0.12), (R * 0.62, T * 1.06, R * 0.36), rot=(20 * sign, 0, 0))         # the hood
    b.box(flesh, (hx + sign * R * 0.1, y, z - R * 0.46), (R * 0.56, T * 0.9, R * 0.4))
    for sd in (-1, 1):
        b.sphere(WHITE, (hx + sign * R * 0.05, y + sd * T * 0.5, z - R * 0.42), R * 0.17, steps=(3, 6))
        b.sphere(DARK, (hx + sign * R * 0.07, y + sd * T * 0.6, z - R * 0.42), R * 0.09, steps=(3, 5))
    t = sc.part(P.motion(swing=17.0, **mo))
    for k in range(5):
        a = math.radians(4 - k * 17)
        tilt_box(t, flesh if k % 2 else WHITE, (hx + sign * R * 0.3, y + (k - 2.0) * T * 0.2, z - R * 0.5),
                 (hx + sign * (R * 0.3 + math.cos(a) * R * 1.15), y + (k - 2.0) * T * 0.3, z - R * 0.5 + math.sin(a) * R * 1.15), R * 0.13, R * 0.13)


def nautilus_shoal(seed=1):
    """A shoal of five giant nautiluses over the far sand: coiled shells 6 to 9 m across, 9 to 34 m up, each rising and
    sinking and rocking in its own time."""
    def build(sc):
        r = random.Random(seed)
        cols = (('#fff1dc', '#ff7a3c', '#ffb48a'), ('#fff6ea', '#ff2d3c', '#ff9aa0'), ('#ffe9b0', '#c8683a', '#ffc93c'), ('#fff1dc', '#ff5fa2', '#ffc0dc'))
        for k in range(5):
            shell, stripe, flesh = cols[(k + seed) % len(cols)]
            R = r.uniform(300, 430)
            _nautilus(sc, (k - 2.0) * 1250 + r.uniform(-250, 250), r.uniform(-500, 500), 1100 + ((k * 3) % 5) * 480 + r.uniform(-120, 120), R,
                      1 if (k + seed) % 3 else -1, shell, stripe, flesh, r.random(), r.uniform(0.09, 0.15), -r.uniform(220, 340))
    return build


# ------------------------------------------------------------------------------------------ the spider crabs' march
def _spider_crab(sc, x, y, s, shell, leg, pale, phase, rate, tv):
    """One giant spider crab facing the road and walking sideways along X: a domed shell 4 m across carried 4 m up on
    eight long legs that work in turn, eyes on stalks, both claws raised and waving."""
    base = (x, y)
    H = 380.0 * s
    bm = dict(phase=phase, rate=rate, base=base, travel=tv)
    b = sc.part(P.motion(bob=24.0 * s, pivot=(x, y, H), **bm))
    b.sphere(shell, (x, y, H), 210 * s, scale=(1.0, 0.85, 0.62), steps=(4, 8))
    b.box(pale, (x, y - 150 * s, H - 30 * s), (240 * s, 60 * s, 90 * s))
    for k in range(5):
        a = k * 1.26 + 0.3
        b.sphere(pale, (x + math.cos(a) * 110 * s, y - 60 * s + math.sin(a) * 50 * s, H + 96 * s), 34 * s, scale=(1, 1, 0.5), steps=(3, 5))
    for sd in (-1, 1):
        b.box(leg, (x + sd * 60 * s, y - 170 * s, H + 110 * s), (16 * s, 16 * s, 150 * s))
        b.sphere(DARK, (x + sd * 60 * s, y - 176 * s, H + 200 * s), 34 * s, steps=(3, 6))
    # the legs: four a side, out along X and down, working one after another
    for sd in (-1, 1):
        for k in range(4):
            hip = (x + sd * 170 * s, y + (k - 1.5) * 70 * s, H - 10 * s)
            knee = (x + sd * (420 + k * 60) * s, y + (k - 1.5) * 150 * s, H + (260 - k * 30) * s)
            foot = (x + sd * (640 + k * 70) * s, y + (k - 1.5) * 230 * s, 0.0)
            m = sc.part(P.motion(phase=phase + k * 0.25 + (0.5 if sd > 0 else 0.0), rate=rate, swing=11.0, pivot=hip, axis=90.0, base=base, travel=tv))
            tilt_box(m, leg, hip, knee, 44 * s, 44 * s)
            tilt_box(m, leg, knee, foot, 34 * s, 34 * s)
            m.box(WHITE, (foot[0], foot[1], 30 * s), (38 * s, 38 * s, 60 * s))
    # the claws, raised to the road
    for sd in (-1, 1):
        sh = (x + sd * 150 * s, y - 120 * s, H + 10 * s)
        el = (x + sd * 330 * s, y - 300 * s, H + 150 * s)
        hand = (x + sd * 300 * s, y - 380 * s, H + 420 * s)
        m = sc.part(P.motion(phase=phase + (0.3 if sd > 0 else 0.0), rate=rate, swing=sd * 16.0, pivot=sh, axis=0.0, base=base, travel=tv))
        tilt_box(m, leg, sh, el, 54 * s, 54 * s)
        tilt_box(m, shell, el, hand, 80 * s, 70 * s)
        tilt_box(m, shell, hand, (hand[0] + sd * 40 * s, hand[1] - 20 * s, hand[2] + 200 * s), 70 * s, 50 * s)
        tilt_box(m, WHITE, hand, (hand[0] - sd * 70 * s, hand[1] - 20 * s, hand[2] + 170 * s), 40 * s, 40 * s)


def crab_march(seed=1, sign=1, length=8400.0, speed=170.0):
    """Giant spider crabs on the march along the trench wall: four of them, one behind the other, walking sideways
    along the road with their claws up. They travel the mesh's X and come round again."""
    def build(sc):
        r = random.Random(seed)
        tv = _tv((length, sign * speed))
        cols = (('#ff5a3c', '#ff8a4a', '#ffe0c0'), ('#ff2d3c', '#ff7a3c', '#ffd2c4'), ('#ff7a3c', '#ffc93c', '#fff1c0'), ('#ff5fa2', '#ff8a6a', '#ffe0ec'))
        for k in range(4):
            shell, leg, pale = cols[(k + seed) % 4]
            _spider_crab(sc, (k - 1.5) * 2100 + r.uniform(-200, 200), r.uniform(-300, 300), r.uniform(1.0, 1.35), shell, leg, pale, r.random(), r.uniform(0.5, 0.7), tv)
    return build


# ------------------------------------------------------------------------------------------ the sea dragon
def _oarfish(sc, x0, y, z, n, seg, tall, sign, body, crest, phase, rate, amp, tv):
    """A ribbon of n slabs with a wave running down it (each slab floats a little behind the one ahead), head first
    along sign * X: a red crest all along the back, a plume on the head, two long oars trailing under the chin."""
    base = (0.0, 0.0)
    for k in range(n):
        t = k / float(n - 1)
        h = tall * (1.0 - 0.75 * t * t)
        cx = x0 - sign * (k + 0.5) * seg
        m = sc.part(P.motion(phase=phase - k * 0.045, rate=rate, bob=-amp * (0.45 + 0.55 * t), pivot=(cx, y, z), base=base, travel=tv))
        m.box(body if k % 4 else '#ffd0dc', (cx, y, z), (seg * 1.12, 50, h))
        m.box(crest, (cx, y, z + h / 2.0 + 40), (seg * 0.8, 16, 90))
        if k == 0:
            hx = x0 + sign * 150
            m.box(body, (hx, y, z - 10), (380, 70, tall * 1.1))
            m.box(crest, (hx + sign * 150, y, z - tall * 0.34), (120, 60, 60))
            for sd in (-1, 1):
                m.sphere(WHITE, (hx + sign * 60, y + sd * 36, z + tall * 0.16), 62, steps=(3, 6))
                m.sphere(DARK, (hx + sign * 70, y + sd * 58, z + tall * 0.16), 32, steps=(3, 5))
            for j in range(4):
                tilt_box(m, crest, (hx - sign * j * 70, y, z + tall * 0.5), (hx - sign * (260 + j * 150), y, z + tall * 0.5 + 520 - j * 70), 22, 22)
            for sd in (-1, 1):
                tilt_box(m, crest, (hx - sign * 60, y + sd * 30, z - tall * 0.5), (hx - sign * 900, y + sd * 160, z - tall * 0.5 - 520), 20, 20)
                m.box(crest, (hx - sign * 940, y + sd * 166, z - tall * 0.5 - 540), (150, 20, 80))
    cx = x0 - sign * (n + 0.4) * seg
    m = sc.part(P.motion(phase=phase - n * 0.045, rate=rate, bob=-amp, pivot=(cx, y, z), base=base, travel=tv))
    m.box(crest, (cx, y, z), (seg * 0.9, 14, tall * 0.6))


def sea_dragon(seed=1, sign=1, length=9000.0, speed=420.0):
    """The oarfish, the sailors' sea dragon: a silver ribbon 60 m long with a red crest, a wave running down it, and a
    younger one 35 m long higher up behind it. They travel the mesh's X and come round again."""
    def build(sc):
        tv = _tv((length, sign * speed))
        _oarfish(sc, sign * 2600.0, 0.0, 2300.0, 26, 230.0, 330.0, sign, '#f4f9ff', '#ff2d3c', 0.0, 0.3, 260.0, tv)
        _oarfish(sc, -sign * 1500.0, 500.0, 3500.0, 18, 190.0, 240.0, sign, '#fff1dc', '#ff5fa2', 0.4, 0.36, 200.0, tv)
    return build


# ------------------------------------------------------------------------------------------ the scooter wing
def scooter_wing(seed=1, sign=1, length=9000.0, speed=520.0):
    """A far flight of Nereida's aquanauts on scooters: seven in a vee, four times life size, a striped streamer
    behind each, travelling the mesh's X and coming round again."""
    def build(sc):
        r = random.Random(seed)
        tv = _tv((length, sign * speed))
        head = 0.0 if sign > 0 else 180.0
        for k in range(7):
            rank = (k + 1) // 2
            side = 1 if k % 2 else -1
            x = -sign * rank * 1500.0
            y = side * rank * 420.0 if k else 0.0
            z = 1600.0 + rank * 300.0 * (1 if k % 2 else -0.6)     # under the squid's height (the review: a wing flew through a squid)
            diver(sc, x, y, z, heading=head, pitch=4.0, s=4.0, suit=SUITS[(k + seed) % len(SUITS)], fins=FINS[(k * 2 + seed) % len(FINS)], phase=r.random(),
                  rate=0.45, bob=-50.0, kick=12.0, base=(0.0, 0.0), travel=tv, scooter=(YEL, ORANGE, WHITE)[k % 3], streamer=(PINK, RED, LIME, ORANGE)[k % 4],
                  chunk=2.0, ribbons=2)
    return build


# ------------------------------------------------------------------------------------------ against the haze
NO_GLOW = (DARK, VISOR, STEEL, GLASS, GLASS_D, DECK, DECK_D, BUBBLE)


def lit(build, glow=0.15):
    """The level's haze takes the colour out of everything past 150 m (learned in frames: a yellow crane at 200 m was
    dark green). Every coloured part of a far piece therefore carries a little steady light of its own colour (no
    blinking: it is the part's own colour, a constant); the dark and the glass parts stay as they are."""
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


JOBS = [
    ('SM_FarSquid', lit(squid(1), 0.1), dict(far=True, grow=(3400.0, 400.0, 300.0), grow_down=(3400.0, 400.0, 0.0))),
    ('SM_FarDomeWorks', lit(dome_works(1)), dict(far=True, grow=(1500.0, 3200.0, 300.0), grow_down=(1500.0, 3200.0, 0.0))),
]
REST = [
    ('SM_FarNautilus', lit(nautilus_shoal(1)), dict(far=True, grow=(150.0, 150.0, 400.0), grow_down=(150.0, 150.0, 400.0))),
    ('SM_FarCrabMarch', lit(crab_march(1)), dict(far=True, grow=(4700.0, 150.0, 150.0), grow_down=(4700.0, 150.0, 0.0))),
    ('SM_FarSeaDragon', lit(sea_dragon(1)), dict(far=True, grow=(4900.0, 100.0, 400.0), grow_down=(4900.0, 100.0, 400.0))),
    ('SM_FarScooterWing', lit(scooter_wing(1)), dict(far=True, grow=(4900.0, 100.0, 200.0), grow_down=(4900.0, 100.0, 200.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS + REST, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS + REST]
