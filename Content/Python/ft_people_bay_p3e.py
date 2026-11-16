"""FULL THROTTLE (Unreal) the population, level 0: Bayshore, pass 3, builder p3e: FAR AND SKY.

Everything here is for 150 m and beyond and for the air over the bay and the town: saved with far=True (the
always-moving master), drawn BIG (people at 1.6 to 2.2 times life size, balloons 17 to 22 m across, canopies 9 to
12 m) in hot colours against sand, water and sky, each with one big slow movement.

  SM_P3E_Balloons_A   three hot-air balloons, two passengers in each basket, drifting and rising and sinking   (Afloat, far out over the bay)
  SM_P3E_Balloons_B   two balloons of another cut and colour                                                  (Far, over the town)
  SM_P3E_Gliders_A    five paragliders wheeling in two rings, a pilot hanging under each arched canopy         (Far, over the beach)
  SM_P3E_Gliders_B    four hang gliders wheeling, the pilots prone under the delta wings                       (Far, over the town)
  SM_P3E_Chutes_A     six parachutists under square canopies, each turning its own slow circle, one with a streamer (Far, over the surf)
  SM_P3E_Chutes_B     five parachutists under round canopies swinging like pendulums                           (Far, over the town)
  SM_P3E_Flamingos    a skein of eleven flamingos that travels 300 m along the shore and comes round           (Afloat)
  SM_P3E_KiteSurf     four kite surfers riding along the shore under big arched kites                          (Afloat)
  SM_P3E_Rowers       three racing shells of four rowers each, bodies rocking, oars sweeping                    (Afloat, inshore)
  SM_P3E_JetSkis      four riders on jet skis chasing each other round a ring, throwing foam                   (Afloat)
  SM_P3E_Riders       five riders on horseback along the lower beach, the leader with a pennant                 (Scatter, 240 m and out)
  SM_P3E_BeachGame    a beach football game: eight players running, a ball in the air, goals, a hopping crowd   (Scatter, 185 m and out)

The placing rules: C:/ftwork/p3/L0e.inl (pasted by the integrator at the end of Source/FullThrottle/FTPeopleL0.inl).
Run in the editor:  import ft_people_bay_p3e as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/Bayshore'

WHITE, CREAM, DARK = '#fff6ea', '#f1e4c8', '#22202a'
HOTPINK, YEL, MINT, SKY, RED, ORANGE, VIOLET, NAVY, LIME = '#ff5fa2', '#ffc93c', '#7dffb0', '#2ee6ff', '#ff2d3c', '#ff7a3c', '#8f6bff', '#2b2350', '#b8ff5a'
HOT = (HOTPINK, YEL, RED, ORANGE, VIOLET, SKY, LIME, MINT)
WICKER, FLAME = '#c9a070', '#ffc93c'
FLAM, FLAM_PALE, FLAM_LEG = '#ff5fa2', '#f4a6ff', '#ff2d3c'


# ------------------------------------------------------------------------------------------ helpers
def person(sc, mo, x, y, z=0.0, s=1.6, top=HOTPINK, bottom=NAVY, skin='#d9a070', hair='#2a2230', arms='vee', yaw=0.0, legs=True):
    """A person for 150 m and beyond (about 72 triangles), drawn into the part with motion mo (so it moves with
    whatever else is drawn into that part). legs=False: from the hips up (in a basket, in a boat)."""
    m = sc.part(mo, (x, y, z), yaw, s)
    if legs:
        m.box(bottom, (0, 0, 34), (36, 24, 68))
    m.box(top, (0, 0, 95), (48, 28, 56))
    m.box(skin, (0, 0, 146), (40, 38, 42))
    m.box(hair, (0, 4, 166), (44, 40, 14))
    for side in (-1, 1):
        if arms == 'vee':
            tilt_box(m, skin, (side * 26, 0, 116), (side * 58, 0, 176), 13, 13)
        elif arms == 'up':
            tilt_box(m, skin, (side * 26, 0, 116), (side * 34, 0, 184), 13, 13)
        elif arms == 'fwd':
            tilt_box(m, skin, (side * 26, 0, 112), (side * 26, -56, 104), 13, 13)
        else:
            tilt_box(m, skin, (side * 28, 0, 118), (side * 36, 0, 64), 13, 13)


def panel(m, col, a, b, chord, thick):
    """A slab from a to b: `chord` deep across (level, square to a-b) and `thick` thick."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    yaw = math.degrees(math.atan2(dy, dx))
    pitch = math.degrees(math.atan2(dz, math.hypot(dx, dy)))
    m.box(col, ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0), (L, chord, thick), rot=(pitch, yaw, 0))


def arch(m, c, ux, uy, span, sag, chord, thick, cols, n=5):
    """An arched canopy: n slabs across `span` along the unit direction (ux, uy) through c, the ends `sag` lower."""
    pts = []
    for i in range(n + 1):
        u = -0.5 + float(i) / n
        pts.append((c[0] + ux * u * span, c[1] + uy * u * span, c[2] - sag * (2.0 * u) ** 2))
    for i in range(n):
        panel(m, cols[i % len(cols)], pts[i], pts[i + 1], chord, thick)
    return pts[0], pts[-1]


def ring_mo(r, sgn, R, cx=0.0, cy=0.0, k=0.05, **kw):
    """The motion of a thing that circles (cx, cy) at radius R: slower the wider the ring."""
    rps = sgn * r.uniform(0.8, 1.2) * k * (1500.0 / max(R, 300.0)) ** 0.5
    d = dict(phase=r.random(), base=(cx, cy), travel=(-1.0, rps))
    d.update(kw)
    return d


# ------------------------------------------------------------------------------------------ the sky
def _balloon(sc, r, x, y, z, R, c1, c2, c3, cut):
    """A hot-air balloon, its envelope's middle at z: it floats up and down and leans slowly about the ground under it."""
    mo = P.motion(phase=r.random(), rate=r.uniform(0.035, 0.055), bob=-r.uniform(250, 450), swing=r.uniform(1.5, 2.6),
                  pivot=(x, y, 0.0), axis=r.uniform(0, 180))
    m = sc.part(mo)
    m.sphere(c1, (x, y, z), R, steps=(6, 10))
    m.cyl(c2, (x, y, z - 1.55 * R), 0.22 * R, 1.0 * R, steps=10, top_r=0.85 * R)            # the throat
    if cut == 0:                                                                            # one broad band and a cap
        m.cyl(c3, (x, y, z - 0.16 * R), 1.03 * R, 0.32 * R, steps=10)
        m.cyl(c2, (x, y, z + 0.74 * R), 0.7 * R, 0.3 * R, steps=10, top_r=0.2 * R)
    else:                                                                                   # two bands, a dark crown
        m.cyl(c3, (x, y, z + 0.2 * R), 0.99 * R, 0.2 * R, steps=10)
        m.cyl(c2, (x, y, z - 0.36 * R), 0.96 * R, 0.2 * R, steps=10)
        m.cyl(c3, (x, y, z + 0.78 * R), 0.64 * R, 0.26 * R, steps=10, top_r=0.15 * R)
    bz = z - 1.55 * R - 420.0                                                               # the basket's floor
    m.box(WICKER, (x, y, bz + 90), (330, 330, 180))
    m.box(c3, (x, y, bz + 172), (346, 346, 26))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, DARK, (x + sx * 150, y + sy * 150, bz + 180), (x + sx * 0.2 * R, y + sy * 0.2 * R, z - 1.5 * R), 12, 12)
    m.box(FLAME, (x, y, z - 1.55 * R - 60), (70, 70, 150), glow=0.5)                        # the burner's flame (steady; 300 m and more out)
    for k2, sx in enumerate((-1, 1)):
        person(sc, mo, x + sx * 80, y - 60 + k2 * 90, bz - 30, s=1.7, top=r.choice(HOT), skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS),
               arms=('vee', 'up')[k2], yaw=r.uniform(-40, 40), legs=False)


def balloons(seed, lay, cut):
    """lay: [(x, y, z of the envelope's middle, radius, three colours)]."""
    def build(sc):
        r = random.Random(seed)
        for (x, y, z, R, cols) in lay:
            _balloon(sc, r, x, y, z, R, cols[0], cols[1], cols[2], cut)
    return build


def _hang(sc, mo, c, th, s, top, r):
    """The pilot hanging in a harness under c, facing along th (degrees)."""
    person(sc, mo, c[0], c[1], c[2], s=s, top=top, bottom=NAVY, skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS), arms='up', yaw=th + 90.0)


def gliders_a(seed=301):
    """Five paragliders, canopies 11 to 13 m across, wheeling in two rings that turn against each other, each rolling
    a little and rising and sinking; the pilot (2 times life size) hangs 7 m under the canopy."""
    def build(sc):
        r = random.Random(seed)
        lay = ((0, 1300, 3000, 1), (130, 1700, 4300, 1), (245, 1500, 5500, 1), (60, 2900, 3900, -1), (200, 3200, 5100, -1))
        for i, (a_deg, R, z, sgn) in enumerate(lay):
            a = math.radians(a_deg)
            ux, uy = math.cos(a), math.sin(a)
            c = (ux * R, uy * R, float(z))
            th = a_deg + sgn * 90.0
            mo = P.motion(rate=r.uniform(0.08, 0.13), bob=-r.uniform(150, 320), swing=r.uniform(6, 10), pivot=c, axis=th, **ring_mo(r, sgn, R, k=0.045))
            m = sc.part(mo)
            c1, c2 = HOT[(i * 3) % 8], (WHITE, YEL, WHITE, NAVY, WHITE)[i]
            span = r.uniform(1100, 1300)
            e0, e1 = arch(m, c, ux, uy, span, 190.0, 330.0, 46.0, (c1, c2, c1, c2, c1))
            seat = (c[0], c[1], c[2] - 700.0)
            for e in (e0, e1):
                tilt_box(m, WHITE, e, (seat[0], seat[1], seat[2] + 330), 9, 9)
            m.box(DARK, (seat[0], seat[1], seat[2] + 150), (80, 80, 150))                    # the harness
            _hang(sc, mo, seat, th, 2.0, c2 if c2 != WHITE else c1, r)
    return build


def gliders_b(seed=311):
    """Four hang gliders, delta wings 11 m across with a bright leading edge, wheeling one way at four heights; the
    pilots lie prone in their bags under the keel."""
    def build(sc):
        r = random.Random(seed)
        lay = ((20, 1200, 3200), (115, 2300, 4200), (205, 1500, 5200), (300, 2500, 6100))
        for i, (a_deg, R, z) in enumerate(lay):
            a = math.radians(a_deg)
            ux, uy = math.cos(a), math.sin(a)
            th = a_deg + 90.0
            tx, ty = math.cos(math.radians(th)), math.sin(math.radians(th))
            c = (ux * R, uy * R, float(z))
            mo = P.motion(rate=r.uniform(0.08, 0.12), bob=-r.uniform(150, 300), swing=r.uniform(8, 13), pivot=c, axis=th, **ring_mo(r, 1, R, k=0.06))
            m = sc.part(mo)
            c1, c2 = HOT[(i * 3 + 1) % 8], (WHITE, NAVY, WHITE, YEL)[i]
            nose = (c[0] + tx * 260, c[1] + ty * 260, c[2] + 30)
            for side in (-1, 1):
                tip = (c[0] + side * ux * 560 - tx * 230, c[1] + side * uy * 560 - ty * 230, c[2] + 60)
                mid = ((nose[0] + tip[0]) / 2.0 - tx * 130, (nose[1] + tip[1]) / 2.0 - ty * 130, c[2] + 20)
                panel(m, c2, nose, tip, 110.0, 34.0)                                         # the leading edge
                panel(m, c1, (c[0] - tx * 40, c[1] - ty * 40, c[2]), mid, 330.0, 26.0)       # the sail
                panel(m, c1, mid, (tip[0] - tx * 60, tip[1] - ty * 60, tip[2] - 10), 200.0, 24.0)
            tilt_box(m, DARK, (c[0] - tx * 300, c[1] - ty * 300, c[2] + 10), nose, 14, 14)   # the keel
            hz = c[2] - 190.0
            tilt_box(m, (NAVY, RED, VIOLET, ORANGE)[i], (c[0] - tx * 330, c[1] - ty * 330, hz - 20), (c[0] + tx * 60, c[1] + ty * 60, hz), 90, 70)   # the pilot in the bag
            m.box(r.choice(P.SKINS), (c[0] + tx * 110, c[1] + ty * 110, hz + 14), (74, 74, 74))
            m.box(WHITE, (c[0] + tx * 110, c[1] + ty * 110, hz + 46), (84, 84, 30))          # the helmet
            tilt_box(m, DARK, (c[0], c[1], hz + 30), (c[0], c[1], c[2]), 12, 12)             # the hang strap
            tilt_box(m, DARK, (c[0] + tx * 170 - ux * 150, c[1] + ty * 170 - uy * 150, hz - 60), (c[0] + tx * 170 + ux * 150, c[1] + ty * 170 + uy * 150, hz - 60), 12, 12)   # the bar
            for side in (-1, 1):
                tilt_box(m, DARK, (c[0] + tx * 170 + side * ux * 150, c[1] + ty * 170 + side * uy * 150, hz - 60), (c[0] + tx * 60, c[1] + ty * 60, c[2]), 10, 10)
    return build


def chutes_a(seed=321):
    """Six parachutists (2.1 times life size) under square canopies 10 m across, each turning its own slow circle,
    rolling into the turn and sinking and rising; one trails a long pink streamer, one a yellow one."""
    def build(sc):
        r = random.Random(seed)
        for i in range(6):
            ox, oy = -4000.0 + i * 1600.0 + r.uniform(-200, 200), r.uniform(-900, 900)
            R = r.uniform(600, 1100)
            sgn = 1 if i % 2 else -1
            a_deg = r.uniform(0, 360)
            a = math.radians(a_deg)
            ux, uy = math.cos(a), math.sin(a)
            z = 3200.0 + ((i * 5) % 6) * 950.0 + r.uniform(-200, 200)
            c = (ox + ux * R, oy + uy * R, z)
            th = a_deg + sgn * 90.0
            tx, ty = math.cos(math.radians(th)), math.sin(math.radians(th))
            mo = P.motion(rate=r.uniform(0.05, 0.08), bob=-r.uniform(300, 600), swing=r.uniform(7, 11), pivot=c, axis=th, **ring_mo(r, sgn, R, ox, oy, k=0.05))
            m = sc.part(mo)
            c1, c2 = HOT[(i * 5 + 2) % 8], (WHITE, NAVY)[i % 2]
            e0, e1 = arch(m, c, ux, uy, 1000.0, 110.0, 420.0, 70.0, (c1, c2, c1, c2, c1))
            seat = (c[0], c[1], c[2] - 760.0)
            for e in (e0, e1, c):
                tilt_box(m, WHITE, e, (seat[0], seat[1], seat[2] + 350), 9, 9)
            if i in (1, 4):                                                                  # a streamer behind
                sc2 = (HOTPINK, YEL)[i // 4]
                p0 = (seat[0] - tx * 60, seat[1] - ty * 60, seat[2] + 60)
                p1 = (seat[0] - tx * 700, seat[1] - ty * 700, seat[2] + 260)
                p2 = (seat[0] - tx * 1500, seat[1] - ty * 1500, seat[2] + 180)
                panel(m, sc2, p0, p1, 26.0, 90.0)
                panel(m, sc2, p1, p2, 26.0, 90.0)
            _hang(sc, mo, seat, th, 2.1, r.choice((RED, NAVY, VIOLET, ORANGE, DARK)), r)
    return build


def chutes_b(seed=331):
    """Five parachutists under round canopies 11 m across (two colours, a white skirt), each swinging like a slow
    pendulum under its canopy and drifting up and down, out of step with the next."""
    def build(sc):
        r = random.Random(seed)
        for i in range(5):
            x, y = -3600.0 + i * 1800.0 + r.uniform(-250, 250), r.uniform(-1000, 1000)
            z = 3400.0 + ((i * 3) % 5) * 1050.0 + r.uniform(-200, 200)
            mo = P.motion(phase=r.random(), rate=r.uniform(0.07, 0.11), bob=-r.uniform(300, 550), swing=r.uniform(8, 13), pivot=(x, y, z + 260.0),
                          axis=r.uniform(0, 180))
            m = sc.part(mo)
            c1, c2 = HOT[(i * 3) % 8], HOT[(i * 3 + 4) % 8]
            m.cyl(WHITE, (x, y, z - 60), 560, 60, steps=10)
            m.cyl(c1, (x, y, z), 550, 190, steps=10, top_r=360)
            m.cyl(c2, (x, y, z + 190), 360, 120, steps=10, top_r=90)
            seat = (x, y, z - 900.0)
            for k in range(4):
                a = math.radians(45 + k * 90)
                tilt_box(m, WHITE, (x + math.cos(a) * 520, y + math.sin(a) * 520, z - 50), (x, y, seat[2] + 350), 9, 9)
            _hang(sc, mo, seat, -90.0 + r.uniform(-50, 50), 2.1, r.choice((RED, NAVY, VIOLET, ORANGE, YEL)), r)
    return build


def _flamingo(sc, x, y, z, span, common, flap):
    """A flamingo in flight along +X: neck and legs stretched out straight, pink wings with black ends."""
    L = span * 0.4
    b = sc.part(P.motion(pivot=(x, y, z), **common))
    tilt_box(b, FLAM, (x - L * 0.5, y, z), (x + L * 0.45, y, z + L * 0.04), L * 0.32, L * 0.3)
    tilt_box(b, FLAM_PALE, (x + L * 0.4, y, z + L * 0.04), (x + L * 1.5, y, z + L * 0.1), L * 0.13, L * 0.13)      # the neck
    b.box(FLAM, (x + L * 1.6, y, z + L * 0.1), (L * 0.3, L * 0.2, L * 0.2))
    tilt_box(b, DARK, (x + L * 1.72, y, z + L * 0.1), (x + L * 1.95, y, z - L * 0.02), L * 0.12, L * 0.12)         # the bill
    tilt_box(b, FLAM_LEG, (x - L * 0.45, y, z - L * 0.04), (x - L * 1.7, y, z - L * 0.1), L * 0.12, L * 0.09)      # the legs
    half = span * 0.5
    for side in (-1, 1):
        w = sc.part(P.motion(swing=side * flap, pivot=(x, y, z + L * 0.08), axis=0.0, **common))
        a = (x, y + side * L * 0.1, z + L * 0.08)
        mid = (x, y + side * half * 0.55, z + L * 0.28)
        end = (x - L * 0.2, y + side * half, z + L * 0.12)
        panel(w, FLAM, a, mid, L * 0.56, max(L * 0.05, 5.0))
        panel(w, DARK, mid, end, L * 0.42, max(L * 0.05, 5.0))


def flamingos(seed=341):
    """A skein of eleven flamingos, 5.6 m across, in a long slanting line with a short second arm: it travels 300 m
    along the shore (the piece's X) and comes round again; the wing beats run down the line bird after bird."""
    def build(sc):
        r = random.Random(seed)
        L, sp = 30000.0, 1300.0
        for k in range(11):
            row = k if k < 8 else k - 7
            side = 1 if k < 8 else -1
            x = 2400.0 - row * 640.0 + r.uniform(-60, 60)
            y = side * row * 330.0
            common = dict(phase=row * 0.11, rate=0.8, base=(x, y), travel=(L, sp / L), bob=-60.0)
            _flamingo(sc, x, y, 1900.0 + row * 60.0 + r.uniform(-50, 50), r.uniform(520, 600), common, 26.0)
    return build


# ------------------------------------------------------------------------------------------ the sea
def kitesurf(seed=351):
    """Four kite surfers (2.2 times life size) riding along the shore, leaning back against the lines, on bright
    boards that throw foam; each one's arched kite, 10 m across, flies 24 to 30 m up ahead of it and sways."""
    def build(sc):
        r = random.Random(seed)
        L = 24000.0
        for i in range(4):
            x, y = -3900.0 + i * 2600.0 + r.uniform(-300, 300), -900.0 + i * 600.0 + r.uniform(-150, 150)
            d = 1 if i % 2 == 0 else -1
            sp = d * r.uniform(800, 1050)
            ph, rate = r.random(), r.uniform(0.3, 0.42)
            c1, c2 = HOT[(i * 3 + 1) % 8], HOT[(i * 3 + 5) % 8]
            P.figure(sc, x, y, z=24, yaw=d * 90 - d * 40, legs='wide', arm_l='fwd', arm_r='fwd', rock=9, rate=rate, phase=ph, travel=(L, sp), detail=1,
                     **P.look(r, tops=(DARK, RED, NAVY, VIOLET), bottoms=(DARK,), wear='swim', sleeves='long', s=2.2))
            tv = dict(base=(x, y), travel=(L, sp / L))
            b = sc.part(P.motion(pivot=(x, y, 0.0), bob=-10, rate=rate, phase=ph, **tv))
            b.box(c1, (x, y, 14), (420, 120, 18))
            b.box(WHITE, (x, y, 24), (330, 34, 4))
            tilt_box(b, WHITE, (x - d * 180, y, 6), (x - d * 1000, y, 60), 240, 44)                # the foam it throws
            tilt_box(b, WHITE, (x - d * 160, y + 110, 4), (x - d * 760, y + 320, 30), 150, 26)
            hands = (x + d * 60, y - 40, 250.0)
            kc = (x + d * r.uniform(900, 1500), y + 150.0, r.uniform(2400, 3000))
            k = sc.part(P.motion(pivot=hands, swing=r.uniform(6, 9), axis=90.0, rate=rate * 0.6, phase=ph, **tv))
            e0, e1 = arch(k, kc, 1.0, 0.0, 1000.0, 330.0, 330.0, 50.0, (c1, c2, WHITE, c2, c1))
            for e in (e0, e1):
                tilt_box(k, WHITE, hands, e, 8, 8)
    return build


def rowers(seed=361):
    """Three racing shells, 19 m long, each with four rowers (2 times life size) who rock forward and back together
    while their oars sweep, a cox in the stern, and a white wake: the boats travel the shore at three speeds."""
    def build(sc):
        r = random.Random(seed)
        L = 24000.0
        for i in range(3):
            x0, y0 = -3000.0 + i * 3000.0 + r.uniform(-300, 300), -700.0 + i * 700.0
            d = 1 if i != 1 else -1
            sp = d * r.uniform(480, 640)
            rate, ph = r.uniform(0.5, 0.62), r.random()
            hull, c2 = (HOTPINK, YEL, SKY)[i], (WHITE, NAVY, RED)[i]
            tv = dict(base=(x0, y0), travel=(L, sp / L))
            h = sc.part(P.motion(pivot=(x0, y0, 0.0), **tv))
            h.box(hull, (x0, y0, 22), (1300, 100, 56))
            h.box(c2, (x0, y0, 52), (1300, 60, 6))
            for e in (-1, 1):
                tilt_box(h, hull, (x0 + e * 650, y0, 22), (x0 + e * 950, y0, 34), 60, 44)
            tilt_box(h, WHITE, (x0 - d * 900, y0, 4), (x0 - d * 2300, y0, 8), 200, 10)          # the wake
            h.box(c2, (x0 - d * 560, y0, 100), (70, 80, 90))                                    # the cox
            h.box(r.choice(P.SKINS), (x0 - d * 560, y0, 172), (60, 60, 56))
            team = (RED, WHITE, VIOLET)[i]
            for k in range(4):
                xk = x0 - d * 300 + d * k * 250.0
                m = sc.part(P.motion(swing=15.0, pivot=(xk, y0, 50.0), axis=90.0, rate=rate, phase=ph, **tv))
                m.box(team, (xk, y0, 110), (70, 96, 116))
                m.box(r.choice(P.SKINS), (xk, y0, 200), (64, 64, 64))
                m.box(r.choice(P.HAIRS), (xk, y0, 238), (70, 70, 18))
                side = 1 if k % 2 else -1
                tilt_box(m, r.choice(P.SKINS), (xk, y0 + side * 40, 150), (xk - d * 70, y0 + side * 50, 96), 20, 20)   # arms to the oar
                o = sc.part(P.motion(swing=side * 24.0, pivot=(xk - d * 70, y0 + side * 70, 80.0), axis=P.UP, rate=rate, phase=ph + 0.25, **tv))
                tilt_box(o, WHITE, (xk - d * 70, y0 + side * 30, 96), (xk - d * 70, y0 + side * 520, 40), 12, 12)
                o.box(hull, (xk - d * 70, y0 + side * 560, 34), (26, 110, 50))
    return build


def jetskis(seed=371):
    """Four riders (2 times life size) on jet skis chasing each other round a ring 26 to 50 m across, hopping over
    the chop, each throwing a rooster tail of foam."""
    def build(sc):
        r = random.Random(seed)
        lay = ((10, 1300, 1), (190, 1500, 1), (95, 2300, -1), (280, 2500, -1))
        for i, (a_deg, R, sgn) in enumerate(lay):
            a = math.radians(a_deg)
            px, py = math.cos(a) * R, math.sin(a) * R
            th = a_deg + sgn * 90.0
            tx, ty = math.cos(math.radians(th)), math.sin(math.radians(th))
            rm = ring_mo(r, sgn, R, k=0.085)
            rps, ph, rate = rm['travel'][1], rm['phase'], r.uniform(0.8, 1.05)
            c1 = HOT[(i * 3 + 2) % 8]
            P.figure(sc, px, py, z=-10, yaw=th + 90.0, legs='sit', arm_l='fwd', arm_r='fwd', bob=45, rate=rate, phase=ph, orbit=(0.0, 0.0, rps), detail=1,
                     **P.look(r, tops=HOT, bottoms=(DARK, NAVY), wear='shorts', s=2.0, hat='cap', hat_col=WHITE))
            m = sc.part(P.motion(pivot=(px, py, 0.0), bob=45, rate=rate, phase=ph, base=(0.0, 0.0), travel=(-1.0, rps)))
            tilt_box(m, c1, (px - tx * 260, py - ty * 260, 30), (px + tx * 200, py + ty * 200, 46), 150, 80)
            tilt_box(m, WHITE, (px + tx * 180, py + ty * 180, 44), (px + tx * 400, py + ty * 400, 76), 110, 50)        # the bow
            tilt_box(m, DARK, (px + tx * 110, py + ty * 110, 70), (px + tx * 150, py + ty * 150, 190), 40, 40)         # the steering column
            tilt_box(m, WHITE, (px - tx * 260, py - ty * 260, 30), (px - tx * 900, py - ty * 900, 330), 150, 50)       # the rooster tail
            tilt_box(m, WHITE, (px - tx * 300, py - ty * 300, 4), (px - tx * 1500, py - ty * 1500, 8), 260, 10)        # the wake
    return build


# ------------------------------------------------------------------------------------------ the far beach
def riders(seed=381):
    """Five riders on horseback (1.9 times life size) along the lower beach, three one way and two the other, the
    horses' legs striding, the riders rising with them; the leader carries a long pennant, the last one waves."""
    def build(sc):
        r = random.Random(seed)
        L = 12000.0
        coats = ('#8e5a3a', '#5a4a3c', '#fff6ea', '#3b2a22', '#c9a070')
        for i in range(5):
            d = 1 if i < 3 else -1
            x = (-2600.0 + i * 1300.0) if d > 0 else (-1500.0 + (i - 3) * 1900.0)
            y = (-150.0 + r.uniform(-80, 80)) if d > 0 else (330.0 + r.uniform(-60, 60))
            sp = d * r.uniform(520, 640)
            ph, rate = r.random(), r.uniform(1.2, 1.5)
            hs = 1.9
            P.quadruped(sc, x, y, yaw=d * 90.0, s=hs, body=coats[i], mane=(DARK, DARK, '#cfcfd8', DARK, WHITE)[i], stride=30, wag=14, rate=rate,
                        bob=20, phase=ph, travel=(L, sp))
            kw = dict(arm_l='fwd', arm_r='fwd')
            if i == 0:
                kw = dict(arm_l='fwd', arm_r='up', hold_r=P.hold_flag(HOTPINK, WHITE, 130.0))
            elif i == 4:
                kw = dict(arm_l='fwd', arm_r='wave', swing_r=24)
            P.figure(sc, x, y, z=120.0 * hs - 46.0 * 1.8 - 6.0, yaw=d * 90.0, legs='sit', rate=rate, bob=20, phase=ph, travel=(L, sp), detail=0,
                     **dict(P.look(r, tops=HOT, s=1.8, hat=('cowboy', 'sun', 'cap', 'cowboy', None)[i], hat_col=(WHITE, YEL, RED, CREAM, WHITE)[i]), **kw))
            m = sc.part(P.motion(pivot=(x, y, 0.0), rate=rate, bob=20, phase=ph, base=(x, y), travel=(L, sp / L)))
            m.box(HOT[(i * 3) % 8], (x, y, 120.0 * hs + 4), (90, 110, 12))                      # the saddle cloth
    return build


def beachgame(seed=391):
    """A beach football game for 185 m and beyond: eight players in two colours (1.8 times life size) running to and
    fro and hopping, a big ball bouncing across the pitch, two goals with flags, a referee, and behind the far
    touchline a crowd of eight hopping out of step between a striped tent and four feather banners."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for e in (-1, 1):                                                                       # the goals
            gx = e * 2500.0
            for sy in (-1, 1):
                g.box(WHITE, (gx, sy * 380, 170), (24, 24, 340))
            g.box(WHITE, (gx, 0, 340), (24, 784, 24))
            g.box((RED, SKY)[e > 0], (gx + e * 90, 0, 150), (10, 760, 300))                    # the net as a slab of the team's colour
        g.box(WHITE, (0, -640, 3), (5000, 24, 6))                                               # touchlines on the sand
        g.box(WHITE, (0, 640, 3), (5000, 24, 6))
        g.box(YEL, (1700, 1050, 150), (700, 420, 300))                                          # the tent
        g.box(WHITE, (1700, 1050 - 214, 150), (240, 8, 300))
        g.cyl(HOTPINK, (1700, 1050, 300), 520, 190, steps=4, top_r=30, rot=(0, 45, 0))
        for i in range(8):                                                                      # the players
            team = i % 2
            x, y = -1900.0 + i * 540.0 + r.uniform(-120, 120), r.uniform(-480, 480)
            along = (i % 3 != 2)
            mo = P.motion(phase=r.random(), rate=r.uniform(0.22, 0.36), bob=r.uniform(18, 34), swing=r.uniform(5, 8), pivot=(x, y, -5000.0),
                          axis=90.0 if along else 0.0)
            person(sc, mo, x, y, s=1.8, top=(RED, SKY)[team], bottom=(WHITE, NAVY)[team], skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS),
                   arms=('down', 'vee', 'down', 'up')[i % 4], yaw=(90.0 if team else -90.0) + r.uniform(-30, 30))
        mo = P.motion(phase=0.3, rate=0.3, bob=24.0, swing=6.0, pivot=(-300.0, -560.0, -5000.0), axis=90.0)
        person(sc, mo, -300.0, -560.0, s=1.8, top=YEL, bottom=DARK, skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS), arms='up')     # the referee
        b = sc.part(P.motion(phase=0.1, rate=0.27, bob=640.0, swing=15.0, pivot=(0.0, 0.0, -5000.0), axis=90.0))                    # the ball
        b.sphere(WHITE, (0.0, 0.0, 120.0), 110, steps=(4, 6))
        b.box(HOTPINK, (0.0, 0.0, 120.0), (226, 226, 50))
        groups = [P.motion(phase=j / 4.0 + r.uniform(-0.05, 0.05), rate=r.uniform(0.9, 1.25), bob=r.uniform(26, 44)) for j in range(4)]
        for i in range(8):                                                                      # the crowd behind the far line
            person(sc, groups[(i * 3) % 4], -2100.0 + i * 420.0 + r.uniform(-50, 50), 900.0 + r.uniform(-60, 60), s=r.uniform(1.5, 1.75),
                   top=HOT[(i * 5 + 1) % 8], bottom=r.choice((NAVY, WHITE, RED, YEL)), skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS),
                   arms=('vee', 'up')[i % 2], yaw=r.uniform(-20, 20))
        for k in range(4):                                                                      # feather banners in the teams' colours
            fx, fy = -2300.0 + k * 1200.0, 1150.0
            f = sc.part(P.motion(swing=7.0, pivot=(fx, fy, 0.0), axis=90.0, rate=r.uniform(0.3, 0.45), phase=r.random()))
            f.box(WHITE, (fx, fy, 400), (10, 10, 800))
            f.box((RED, SKY)[k % 2], (fx + 55, fy, 520), (100, 8, 520))
    return build


JOBS = [
    ('SM_P3E_Balloons_A', balloons(401, ((-3300.0, 300.0, 5200.0, 1000.0, (HOTPINK, YEL, WHITE)), (300.0, -500.0, 7600.0, 1100.0, (SKY, WHITE, RED)),
                                         (3400.0, 500.0, 6100.0, 900.0, (ORANGE, VIOLET, YEL))), 0),
     dict(far=True, grow=(400.0, 400.0, 500.0), grow_down=(400.0, 400.0, 500.0))),
    ('SM_P3E_Balloons_B', balloons(402, ((-1900.0, 0.0, 5000.0, 1050.0, (YEL, RED, NAVY)), (2100.0, 300.0, 7200.0, 900.0, (MINT, HOTPINK, VIOLET))), 1),
     dict(far=True, grow=(400.0, 400.0, 500.0), grow_down=(400.0, 400.0, 500.0))),
    ('SM_P3E_Gliders_A', gliders_a(301), dict(far=True, grow=(400.0, 400.0, 400.0), grow_down=(400.0, 400.0, 400.0))),
    ('SM_P3E_Gliders_B', gliders_b(311), dict(far=True, grow=(400.0, 400.0, 400.0), grow_down=(400.0, 400.0, 400.0))),
    ('SM_P3E_Chutes_A', chutes_a(321), dict(far=True, grow=(2200.0, 2200.0, 700.0), grow_down=(2200.0, 2200.0, 700.0))),
    ('SM_P3E_Chutes_B', chutes_b(331), dict(far=True, grow=(500.0, 500.0, 600.0), grow_down=(500.0, 500.0, 600.0))),
    ('SM_P3E_Flamingos', flamingos(341), dict(far=True, grow=(15000.0, 100.0, 200.0), grow_down=(15000.0, 100.0, 100.0))),
    ('SM_P3E_KiteSurf', kitesurf(351), dict(far=True, grow=(12000.0, 100.0, 300.0), grow_down=(12000.0, 100.0, 30.0))),
    ('SM_P3E_Rowers', rowers(361), dict(far=True, grow=(12000.0, 100.0, 60.0), grow_down=(12000.0, 100.0, 10.0))),
    ('SM_P3E_JetSkis', jetskis(371), dict(far=True, grow=(300.0, 300.0, 100.0), grow_down=(300.0, 300.0, 10.0))),
    ('SM_P3E_Riders', riders(381), dict(far=True, grow=(6000.0, 60.0, 60.0), grow_down=(6000.0, 60.0, 0.0))),
    ('SM_P3E_BeachGame', beachgame(391), dict(far=True, grow=(1400.0, 800.0, 700.0), grow_down=(1400.0, 800.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
