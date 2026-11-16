"""FULL THROTTLE (Unreal) the population, level index 2: Nevada, pass 3, builder p3e: FAR AND SKY.

Everything here is for 150 m and beyond and for the air over Highway 93: saved with far=True (the always-moving
master), drawn BIG (people 3 to 3.4 times life size, balloons 21 to 26 m across, canopies 13 to 20 m) in hot colours
against orange land and a hazy sky, each with one big slow movement. Nothing lit except the balloons' burner flame
(steady, 160 m and more from the road).

  In the air (the height is built into the mesh: z = 0 is the land)
  SM_P3E_Balloons_A    three hot-air balloons, two passengers waving in each basket, rising, sinking and leaning
  SM_P3E_Balloons_B    three balloons of another cut and other colours
  SM_P3E_HangGliders   five hang gliders wheeling in two rings that turn against each other, pilots prone under the wings
  SM_P3E_ParaGliders   five paragliders wheeling under arched canopies, a pilot hanging under each
  SM_P3E_Skydivers     seven parachutists under round canopies, swinging like pendulums and drifting round
  SM_P3E_Geese         a skein of eleven snow geese in a V that travels 420 m along the road and comes round
  SM_P3E_Paramotors    five powered paragliders in line astern that travel 500 m along the road and come round
  On the land (each brings its own pale ground: a dark thing on orange land is lost)
  SM_P3E_Kites         a kite meet on a pale playa: eight fliers leaning on their lines, eight big kites swinging
  SM_P3E_DirtBikes     six dirt bike riders racing a pale track 160 m long, leaping, dust behind them
  SM_P3E_LandYachts    six land yachts under bright sails racing a white salt strip 200 m long, heeling
  SM_P3E_Climbers      a 50 m rock tower: six climbers on bright ropes up its face, a party waving on the summit
  SM_P3E_Rodeo         a far rodeo: a hopping crowd on bleachers, a bucking bronco, two riders crossing the ring

The placing rules: C:/ftwork/p3/L2e.inl (pasted by the integrator at the end of Source/FullThrottle/FTPeopleL2.inl).
The ground, the dust and the far animal are ft_people_nevada_far's (swell, bush, puff, beast): imported, not changed.
Run in the editor:  import ft_people_nevada_p3e as K; K.run()
"""
import math
import random
import ft_people as P
import ft_people_nevada_far as FAR
from ft_desert import rod, tilt_box

DIR = '/Game/People/Nevada'

SAND, SANDST, SANDST_D, SANDST_L, ROCK_R, TALUS = '#f2c58a', '#e0784a', '#b8553a', '#f59a62', '#c9603e', '#a04a34'
PALE, DUST, SALT = '#f7cf98', '#fff1dc', '#fff6ea'
WOOD, BARK_D = '#9a6a44', '#6a4c36'
INK, NAVY, TEAL, RED, MAG, CYAN = '#22202a', '#2b2350', '#2fc6c0', '#ff2d3c', '#ff4fb8', '#2ee6ff'
CREAM, WHITE, GOLD, YEL, DENIM, ORANGE, VIOLET, LIME = '#f4efe6', '#fff6ea', '#ffc93c', '#ffe14d', '#3a5fa8', '#ff7a3c', '#8f6bff', '#b8ff5a'
HOT = (RED, CYAN, GOLD, MAG, WHITE, TEAL, VIOLET, LIME, ORANGE)
WICKER, FLAME = '#c9a070', '#ffc93c'


# ------------------------------------------------------------------------------------------ helpers
def person(sc, mo, x, y, z=0.0, s=3.0, top=RED, bottom=NAVY, skin='#d9a070', hair=INK, arms='vee', yaw=0.0, legs=True, hat=None):
    """A person for 150 m and beyond (about 72 triangles), drawn into the part with motion mo (so it moves with
    whatever else is drawn into that part). legs=False: from the hips up (in a basket, in a seat).
    NOTE: this moves the part's drawing frame to the person: fetch the part again (sc.part(mo)) before drawing more."""
    m = sc.part(mo, (x, y, z), yaw, s)
    if legs:
        m.box(bottom, (0, 0, 34), (36, 24, 68))
    m.box(top, (0, 0, 95), (48, 28, 56))
    m.box(skin, (0, 0, 146), (40, 38, 42))
    if hat:
        m.box(hat, (0, 0, 169), (76, 76, 7))
        m.box(hat, (0, 0, 180), (36, 38, 16))
    else:
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
    return m


def fan(sc, mo, x, y, z, s, top, skin, arms_up=False):
    """Somebody in a far crowd: a body, a head, now and then both arms up (24 or 36 triangles)."""
    m = sc.part(mo, (x, y, z), 0.0, s)
    m.box(top, (0, 0, 52), (50, 30, 104))
    m.box(skin, (0, 0, 126), (40, 36, 42))
    if arms_up:
        m.box(skin, (0, 0, 150), (112, 14, 16))


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


def ring(r, sgn, R, k=0.06):
    """Rounds a second for a thing that circles at radius R: slower the wider the ring."""
    return sgn * r.uniform(0.8, 1.2) * k * (1500.0 / max(R, 300.0)) ** 0.5


def pad(g, L, D, H, top=PALE, skirt=SANDST_D, cut=0.22):
    """The ground a far scene brings: a pale eight-sided flat on a darker skirt that goes 3 m into the land."""
    def octa(l, d):
        c = cut * min(l, d)
        return [(-l / 2 + c, -d / 2), (l / 2 - c, -d / 2), (l / 2, -d / 2 + c), (l / 2, d / 2 - c),
                (l / 2 - c, d / 2), (-l / 2 + c, d / 2), (-l / 2, d / 2 - c), (-l / 2, -d / 2 + c)]
    g.plan(skirt, octa(L + 600.0, D + 600.0), -300.0, H * 0.55)
    g.plan(top, octa(L, D), H * 0.55, H)


def pennant(sc, r, x, y, z, h, col, w=260.0):
    """A pole and a pennant that swings round it."""
    g = sc.still()
    g.box(CREAM, (x, y, z + h / 2.0), (22, 22, h))
    m = sc.part(P.motion(phase=r.random(), rate=r.uniform(0.7, 1.1), swing=24.0, pivot=(x, y, z + h), axis=P.UP))
    m.box(col, (x + w / 2.0, y, z + h - 60), (w, 10, 110))
    m.box(WHITE, (x + w / 2.0, y, z + h - 60), (w * 0.5, 12, 36))


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
    elif cut == 1:                                                                          # two bands, a crown
        m.cyl(c3, (x, y, z + 0.2 * R), 0.99 * R, 0.2 * R, steps=10)
        m.cyl(c2, (x, y, z - 0.36 * R), 1.0 * R, 0.2 * R, steps=10)
        m.cyl(c3, (x, y, z + 0.78 * R), 0.64 * R, 0.26 * R, steps=10, top_r=0.15 * R)
    else:                                                                                   # three narrow bands
        for k2, dz in enumerate((-0.42, -0.07, 0.3)):
            m.cyl((c3, c2, c3)[k2], (x, y, z + dz * R), (0.95, 1.03, 0.97)[k2] * R, 0.14 * R, steps=10)
    bz = z - 1.55 * R - 420.0                                                               # the basket's floor
    m.box(WICKER, (x, y, bz + 90), (330, 330, 180))
    m.box(c3, (x, y, bz + 172), (346, 346, 26))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, INK, (x + sx * 150, y + sy * 150, bz + 180), (x + sx * 0.2 * R, y + sy * 0.2 * R, z - 1.5 * R), 24, 24)
    m.box(FLAME, (x, y, z - 1.55 * R - 60), (70, 70, 150), glow=0.5)                        # the burner (steady; 160 m and more out)
    for k2, sx in enumerate((-1, 1)):
        person(sc, mo, x + sx * 80, y - 60 + k2 * 90, bz - 30, s=1.9, top=r.choice(HOT), skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS),
               arms=('vee', 'up')[k2], yaw=r.uniform(-40, 40), legs=False, hat=(CREAM if k2 else None))


def balloons(seed, lay):
    """lay: [(x, y, z of the envelope's middle, radius, three colours, cut)]."""
    def build(sc):
        r = random.Random(seed)
        for (x, y, z, R, cols, cut) in lay:
            _balloon(sc, r, x, y, z, R, cols[0], cols[1], cols[2], cut)
    return build


def hang_gliders(sc):
    """Five hang gliders, wings 17 m across, wheeling in two rings that turn against each other, each banking a
    little and rising and sinking; the pilot lies prone under the wing."""
    r = random.Random(601)
    lay = ((10, 1400, 3600, 1), (135, 1800, 5000, 1), (250, 1600, 6300, 1), (70, 3000, 4400, -1), (215, 3300, 5700, -1))
    for i, (a_deg, R, z, sgn) in enumerate(lay):
        a = math.radians(a_deg)
        c = (math.cos(a) * R, math.sin(a) * R, float(z))
        th = math.radians(a_deg + sgn * 90.0)
        tx, ty = math.cos(th), math.sin(th)
        nx, ny = -ty, tx
        mo = P.motion(phase=r.random(), rate=r.uniform(0.08, 0.13), bob=-r.uniform(150, 320), swing=r.uniform(7, 12), pivot=c,
                      axis=a_deg + sgn * 90.0, base=(0.0, 0.0), travel=(-1.0, ring(r, sgn, R)))
        m = sc.part(mo)
        c1, c2 = HOT[(i * 2) % len(HOT)], (WHITE, INK, YEL, WHITE, NAVY)[i]
        nose = (c[0] + tx * 420, c[1] + ty * 420, c[2] + 30)
        for side in (-1, 1):
            tip = (c[0] - tx * 330 + side * nx * 850, c[1] - ty * 330 + side * ny * 850, c[2] + 60)
            mid = ((nose[0] + tip[0]) / 2.0 - tx * 150, (nose[1] + tip[1]) / 2.0 - ty * 150, c[2] + 40)
            panel(m, c1, nose, tip, 330, 22)
            panel(m, c2, (c[0] - tx * 60, c[1] - ty * 60, c[2] + 20), mid, 300, 20)
        tilt_box(m, INK, (c[0] - tx * 420, c[1] - ty * 420, c[2] + 26), nose, 26, 26)                      # the keel
        tilt_box(m, INK, (c[0], c[1], c[2] + 20), (c[0] + tx * 60, c[1] + ty * 60, c[2] - 250), 24, 24)       # the control frame
        # the pilot, prone: legs, body, head, in a bright harness
        top = HOT[(i * 2 + 3) % len(HOT)]
        tilt_box(m, NAVY, (c[0] - tx * 330, c[1] - ty * 330, c[2] - 170), (c[0] - tx * 110, c[1] - ty * 110, c[2] - 190), 80, 62)
        tilt_box(m, top, (c[0] - tx * 110, c[1] - ty * 110, c[2] - 190), (c[0] + tx * 100, c[1] + ty * 100, c[2] - 210), 120, 80)
        m.box(r.choice(P.SKINS), (c[0] + tx * 150, c[1] + ty * 150, c[2] - 200), (90, 90, 90))
        m.box(WHITE, (c[0] + tx * 150, c[1] + ty * 150, c[2] - 160), (100, 100, 40))                          # the helmet


def _wing_pilot(sc, r, mo, c, tx, ty, span, cols, top, drop=760.0, s=3.0, motor=False):
    """An arched canopy at c, its span across the line of flight (tx, ty), and the pilot hanging `drop` under it."""
    nx, ny = -ty, tx
    m = sc.part(mo)
    e0, e1 = arch(m, c, nx, ny, span, span * 0.2, span * 0.3, 34, cols)
    seat = (c[0], c[1], c[2] - drop)
    for e in (e0, e1):
        tilt_box(m, INK, (e[0], e[1], e[2]), (seat[0], seat[1], seat[2] + 150.0 * s), 30, 30)
    if motor:                                                                                # the cage behind the pilot
        back = (seat[0] - tx * 40.0 * s, seat[1] - ty * 40.0 * s, seat[2] + 100.0 * s)
        rod(m, INK, back, (back[0] - tx * 26.0, back[1] - ty * 26.0, back[2]), 62.0 * s, steps=8)
        rod(m, top, (back[0] - tx * 26.0, back[1] - ty * 26.0, back[2]), (back[0] - tx * 50.0, back[1] - ty * 50.0, back[2]), 20.0 * s, steps=6)
    person(sc, mo, seat[0], seat[1], seat[2], s=s, top=top, bottom=NAVY, skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS), arms='up',
           yaw=math.degrees(math.atan2(ty, tx)) + 90.0, hat=WHITE)


def para_gliders(sc):
    """Five paragliders, canopies 14 to 16 m across, wheeling in two rings; each rolls a little and rises and sinks."""
    r = random.Random(611)
    lay = ((0, 1300, 3200, 1), (120, 1700, 4500, 1), (240, 1500, 5700, 1), (55, 2800, 4000, -1), (195, 3100, 5200, -1))
    for i, (a_deg, R, z, sgn) in enumerate(lay):
        a = math.radians(a_deg)
        c = (math.cos(a) * R, math.sin(a) * R, float(z))
        th = a_deg + sgn * 90.0
        mo = P.motion(phase=r.random(), rate=r.uniform(0.08, 0.13), bob=-r.uniform(150, 320), swing=r.uniform(6, 10), pivot=c, axis=th,
                      base=(0.0, 0.0), travel=(-1.0, ring(r, sgn, R, k=0.05)))
        c1, c2 = HOT[(i * 4 + 1) % len(HOT)], (WHITE, YEL, WHITE, INK, WHITE)[i]
        _wing_pilot(sc, r, mo, c, math.cos(math.radians(th)), math.sin(math.radians(th)), r.uniform(1400, 1600), (c1, c2, c1, c2, c1),
                    HOT[(i * 4 + 4) % len(HOT)])


def paramotors(sc):
    """Five powered paragliders in line astern, canopies 18 m across, flying 500 m along the road and coming round:
    each rolls from side to side and rises and sinks out of step with the next."""
    r = random.Random(621)
    L, sp = 50000.0, 1300.0
    for i in range(5):
        x = 9000.0 - i * 4400.0 + r.uniform(-500, 500)
        y = ((i * 2) % 5 - 2) * 420.0
        c = (x, y, 3000.0 + ((i * 3) % 5) * 520.0)
        mo = P.motion(phase=r.random(), rate=r.uniform(0.09, 0.14), bob=-r.uniform(180, 320), swing=r.uniform(6, 10), pivot=c, axis=0.0,
                      base=(x, y), travel=(L, sp / L))
        c1, c2 = HOT[(i * 2 + 3) % len(HOT)], (WHITE, INK, WHITE, YEL, WHITE)[i]
        _wing_pilot(sc, r, mo, c, 1.0, 0.0, 1800.0, (c1, c2, c1, c2, c1), HOT[(i * 2 + 6) % len(HOT)], drop=820.0, s=3.2, motor=True)


def skydivers(sc):
    """Seven parachutists under round canopies 13 m across, at every height from 25 to 80 m: each swings like a
    pendulum under its canopy, sinks and lifts, and the whole stick drifts slowly round."""
    r = random.Random(631)
    lay = ((0, 600, 7600), (70, 1500, 6600), (150, 900, 5600), (205, 2100, 4900), (265, 1300, 4000), (320, 2400, 3300), (30, 2700, 2600))
    for i, (a_deg, R, z) in enumerate(lay):
        a = math.radians(a_deg)
        x, y = math.cos(a) * R, math.sin(a) * R
        Rc = 650.0
        mo = P.motion(phase=r.random(), rate=r.uniform(0.1, 0.16), bob=-r.uniform(350, 650), swing=r.uniform(8, 13), pivot=(x, y, z + 0.45 * Rc),
                      axis=r.uniform(0, 180), base=(0.0, 0.0), travel=(-1.0, r.choice((-1, 1)) * r.uniform(0.008, 0.016)))
        m = sc.part(mo)
        c1, c2 = HOT[(i * 2) % len(HOT)], (WHITE, INK, WHITE, YEL, WHITE, NAVY, WHITE)[i]
        m.cyl(c1, (x, y, z), Rc, 0.45 * Rc, steps=8, top_r=0.38 * Rc)                         # the canopy
        m.cyl(c2, (x, y, z - 0.14 * Rc), 1.03 * Rc, 0.16 * Rc, steps=8)                       # its skirt
        m.cyl(c2, (x, y, z + 0.44 * Rc), 0.4 * Rc, 0.08 * Rc, steps=8, top_r=0.12 * Rc)       # the cap
        jz = z - 900.0
        for (lx, ly) in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            tilt_box(m, INK, (x + lx * 0.9 * Rc, y + ly * 0.9 * Rc, z - 0.1 * Rc), (x + lx * 60, y + ly * 60, jz + 520), 28, 28)
        person(sc, mo, x, y, jz, s=3.0, top=HOT[(i * 2 + 5) % len(HOT)], bottom=(NAVY, INK, DENIM)[i % 3], skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS),
               arms='up', yaw=r.uniform(-60, 60), hat=None)


def geese(sc):
    """A skein of eleven snow geese in a V, 5 m from wing tip to wing tip, white with black wing ends: they fly 420 m
    along the road, beating slowly, and come round again."""
    r = random.Random(641)
    L, sp = 42000.0, 1500.0
    for i in range(11):
        k = (i + 1) // 2
        side = -1 if i % 2 else 1
        x = 2600.0 - k * 520.0 + r.uniform(-60, 60)
        y = side * k * 400.0 + r.uniform(-40, 40)
        z = 3200.0 + k * 40.0 + r.uniform(-80, 80)
        P.bird(sc, x, y, z, heading=0.0, span=r.uniform(480, 560), body=WHITE, wing=INK, beak=ORANGE, phase=r.random(), rate=r.uniform(1.0, 1.25),
               flap=30.0, travel=(L, sp), bob=-40.0)


# ------------------------------------------------------------------------------------------ the land
def kites(sc):
    """A kite meet on a pale playa: eight fliers lean on their lines, and eight big kites (diamonds with tails, deltas,
    box kites) swing slowly from side to side 18 to 46 m up, each the way its flier leans; three watch and jump."""
    r = random.Random(651)
    g = sc.still()
    S, Z = 3.0, 260.0
    pad(g, 6800.0, 3000.0, Z)
    for i in range(8):
        fx = -2700.0 + i * 770.0 + r.uniform(-150, 150)
        fy = r.uniform(-700, 100)
        ph, rate = r.random(), r.uniform(0.13, 0.22)
        sw = r.uniform(11, 18) * (1 if i % 2 else -1)
        hand = (fx, fy, Z + 184.0 * S)
        person(sc, P.motion(phase=ph, rate=rate, swing=sw * 0.3, pivot=(fx, fy, Z), axis=90.0), fx, fy, Z, s=S, top=HOT[(i * 2 + 1) % len(HOT)],
               bottom=r.choice((NAVY, DENIM, CREAM)), skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS), arms='up', yaw=180.0,
               hat=(CREAM if i % 3 == 0 else None))
        kx, ky, kz = fx + r.uniform(-300, 300), fy + r.uniform(500, 1300), Z + 1800.0 + ((i * 3) % 8) * 400.0
        W = r.uniform(420, 560)
        c1, c2 = HOT[(i * 4) % len(HOT)], HOT[(i * 4 + 2) % len(HOT)]
        m = sc.part(P.motion(phase=ph, rate=rate, swing=sw, pivot=hand, axis=90.0))
        tilt_box(m, WHITE, hand, (kx, ky, kz), 22, 22)                                         # the line
        kind = i % 3
        if kind == 0:                                                                          # a diamond and its tail
            m.box(c1, (kx, ky, kz), (W, 14, W), rot=(45, 0, 0))
            m.box(c2, (kx, ky - 2, kz), (W * 1.36, 16, W * 0.14))
            for j in range(3):
                m.box((c2, c1, c2)[j], (kx + (j % 2) * 40 - 20, ky, kz - W * 0.92 - j * W * 0.34), (W * 0.2, 12, W * 0.2))
        elif kind == 1:                                                                        # a delta and two streamers
            m.side(c1, [(kx - W, kz - W * 0.4), (kx + W, kz - W * 0.4), (kx, kz + W * 0.7)], ky - 8, ky + 8)
            m.box(c2, (kx, ky - 2, kz - W * 0.22), (W * 1.2, 18, W * 0.16))
            for sx in (-1, 1):
                tilt_box(m, c2, (kx + sx * W * 0.7, ky, kz - W * 0.4), (kx + sx * W * 0.9, ky, kz - W * 1.3), 40, 12)
        else:                                                                                  # a box kite
            for dz in (-0.5, 0.5):
                m.box((c1, c2)[dz > 0], (kx, ky, kz + dz * W * 0.9), (W * 0.6, W * 0.6, W * 0.5))
            m.box(INK, (kx, ky, kz), (30, 30, W * 1.4))
    for i in range(3):
        wx = -1900.0 + i * 1900.0 + r.uniform(-200, 200)
        person(sc, P.motion(phase=r.random(), rate=r.uniform(1.0, 1.4), bob=r.uniform(40, 90)), wx, -1000.0 + r.uniform(-150, 150), Z, s=S * (0.7 if i == 1 else 1.0),
               top=HOT[(i * 3 + 2) % len(HOT)], bottom=r.choice((NAVY, DENIM)), skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS), arms='vee')
    for sx in (-1, 1):
        pennant(sc, r, sx * 3150.0, -1100.0, Z, 900.0, (MAG, CYAN)[sx > 0])


def dirt_bikes(sc):
    """Six dirt bike riders racing a pale track 160 m long: each leaps and rears out of step with the next, dust
    rolls behind every other one, and pennants mark the course. They cross it and come round again."""
    r = random.Random(661)
    g = sc.still()
    L, S, Z = 16000.0, 3.2, 280.0
    FAR.swell(g, r, L + 1400, 2600, Z, top=0.55)
    for k in range(4):
        FAR.bush(g, r, -L / 2.0 + 1500 + k * 4300 + r.uniform(-300, 300), r.choice((-1, 1)) * r.uniform(620, 700), Z - 120, 1.8)
    sp = 1500.0
    tv = (L, sp / L)
    for i in range(6):
        x = 4200.0 - i * 1500.0 + r.uniform(-250, 250)
        y = ((i * 3) % 5 - 2) * 120.0
        ph, rate = r.random(), r.uniform(0.5, 0.8)
        mo = P.motion(phase=ph, rate=rate, bob=r.uniform(130, 320), swing=r.uniform(10, 18), pivot=(x, y, Z + 60.0 * S), axis=90.0, base=(x, y), travel=tv)
        m = sc.part(mo, (x, y, Z), 0.0, S)
        c1, c2 = HOT[(i * 2) % len(HOT)], HOT[(i * 2 + 4) % len(HOT)]
        for wx in (-52, 52):
            rod(m, INK, (wx, -8, 30), (wx, 8, 30), 30, steps=7)                                # the wheels
        m.box(c1, (0, 0, 58), (72, 18, 26))                                                    # tank and frame
        tilt_box(m, c1, (-62, 0, 70), (-18, 0, 64), 16, 8)                                     # the rear fender
        tilt_box(m, CREAM, (38, 0, 60), (54, 0, 32), 9, 9)                                     # the fork
        m.box(INK, (40, 0, 86), (10, 46, 8))                                                   # the bars
        tilt_box(m, c2, (-8, 0, 72), (16, 0, 118), 34, 28)                                     # the rider's body, forward
        tilt_box(m, c2, (10, 0, 108), (40, 0, 90), 40, 11)                                     # both arms to the bars
        tilt_box(m, NAVY, (-8, 0, 74), (16, 0, 44), 36, 14)                                    # the legs
        m.box(WHITE if i % 2 else YEL, (24, 0, 134), (32, 30, 30))                             # the helmet
        if i % 2 == 0:
            FAR.puff(sc, x - 900.0, y, Z, 200, ph, rate, tv)
    for k in range(4):
        pennant(sc, r, -6000.0 + k * 4000.0, 560.0, Z - 40, 700.0, (RED, CYAN, MAG, GOLD)[k], w=220.0)


def land_yachts(sc):
    """Six land yachts racing a white salt strip 200 m long under bright sails, 13 m to the masthead: each heels to
    the wind out of step with the next; white dust behind three of them. They cross it and come round again."""
    r = random.Random(671)
    g = sc.still()
    L, S, Z = 20000.0, 3.0, 220.0
    FAR.swell(g, r, L + 1600, 3200, Z, tones=(SANDST_D, PALE, SALT), top=0.6)
    sp = 1700.0
    tv = (L, sp / L)
    for i in range(6):
        x = 5000.0 - i * 1750.0 + r.uniform(-300, 300)
        y = ((i * 2) % 5 - 2) * 190.0
        ph = r.random()
        mo = P.motion(phase=ph, rate=r.uniform(0.25, 0.4), swing=r.uniform(5, 9), pivot=(x, y, Z), axis=0.0, base=(x, y), travel=tv)
        m = sc.part(mo, (x, y, Z), 0.0, S)
        c1, c2 = HOT[(i * 3) % len(HOT)], HOT[(i * 3 + 1) % len(HOT)]
        tilt_box(m, c2, (-110, 0, 36), (150, 0, 30), 40, 22)                                   # the hull
        m.box(INK, (-90, 0, 30), (12, 196, 10))                                                # the rear axle
        for wy in (-98, 98):
            rod(m, INK, (-90, wy - 7, 24), (-90, wy + 7, 24), 24, steps=7)
        rod(m, INK, (150, -7, 22), (150, 7, 22), 22, steps=7)
        m.box(CREAM, (20, 0, 230), (10, 10, 400))                                              # the mast
        m.side(c1, [(12, 62), (-170, 74), (16, 426)], -5, 5)                                   # the sail
        tilt_box(m, WHITE, (10, 0, 170), (-112, 0, 176), 13, 44)                               # a band across it
        m.box(c2, (-44, 0, 62), (54, 32, 36))                                                  # the pilot, low in the hull
        m.box(WHITE, (-70, 0, 88), (28, 28, 28))
        if i % 2 == 1:
            FAR.puff(sc, x - 1000.0, y, Z, 190, ph, 0.5, tv, col=SALT)
    for k in range(3):
        pennant(sc, r, -6500.0 + k * 6500.0, 700.0, Z - 40, 700.0, (RED, TEAL, MAG)[k], w=220.0)


def climbers(sc):
    """A tower of the canyon's own rock, 50 m to its flat top, stepped back bed by bed: six climbers on bright ropes
    up the face that looks at the road, each working up and down and swaying on the rope; four on the summit jump and
    wave under a pennant; a camp of two tents and three more at the foot, looking up."""
    r = random.Random(681)
    g = sc.still()
    S = 3.4
    beds = ((2700.0, 2300.0, -300.0, 1000.0, TALUS), (2250.0, 1950.0, 1000.0, 2200.0, SANDST_D), (1900.0, 1650.0, 2200.0, 3400.0, ROCK_R),
            (1600.0, 1400.0, 3400.0, 4400.0, SANDST), (1300.0, 1200.0, 4400.0, 5000.0, SANDST_L))
    TOP = 5000.0
    for (W, D, z0, z1, col) in beds:
        g.box(col, (0, 0, (z0 + z1) / 2.0), (W, D, z1 - z0))
    g.box(SANDST_D, (-1350, 500, 1200), (700, 1100, 3000))                                    # two buttresses
    g.box(ROCK_R, (1150, 600, 1900), (600, 900, 4400))
    g.box(SAND, (0, 0, TOP + 12), (1220, 1120, 24))                                           # the flat top, pale
    spots = ((0, -620.0, 180.0), (1, 520.0, 1100.0), (1, -180.0, 1250.0), (2, 380.0, 2350.0), (3, -330.0, 3480.0), (3, 420.0, 3650.0))
    for i, (b, cx, cz) in enumerate(spots):
        W, D = beds[b][0], beds[b][1]
        cy = -D / 2.0 - 16.0 * S
        rope = HOT[(i * 2) % len(HOT)]
        g.box(rope, (cx, cy + 10, (cz + 170 * S + TOP) / 2.0), (24, 24, TOP - cz - 170 * S))  # the rope, up to the rim
        mo = P.motion(phase=r.random(), rate=r.uniform(0.12, 0.2), bob=-r.uniform(90, 170), swing=r.uniform(1.5, 3.0), pivot=(cx, cy, TOP), axis=90.0)
        person(sc, mo, cx, cy, cz, s=S, top=HOT[(i * 2 + 3) % len(HOT)], bottom=(NAVY, DENIM, INK)[i % 3], skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS),
               arms='up', yaw=180.0, hat=(WHITE, YEL, RED)[i % 3])
    for i in range(4):
        px = -420.0 + i * 280.0
        person(sc, P.motion(phase=r.random(), rate=r.uniform(0.9, 1.3), bob=r.uniform(40, 90)), px, -330.0 + (i % 2) * 180.0, TOP + 24, s=S,
               top=HOT[(i * 3 + 1) % len(HOT)], bottom=r.choice((NAVY, DENIM)), skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS), arms=('vee', 'up')[i % 2],
               hat=(CREAM if i == 2 else None))
    pennant(sc, r, 480.0, 300.0, TOP + 24, 900.0, RED, w=360.0)
    for k, (tx, col) in enumerate(((-900.0, CYAN), (700.0, GOLD))):                            # the camp
        g.cyl(col, (tx, -1750.0, 0.0), 300.0, 330.0, steps=4, top_r=20.0)
    for i in range(3):
        person(sc, P.motion(phase=r.random(), rate=r.uniform(0.8, 1.1), bob=r.uniform(20, 50)), -200.0 + i * 420.0, -1700.0 - (i % 2) * 160.0, 0.0, s=S,
               top=HOT[(i * 4 + 2) % len(HOT)], bottom=NAVY, skin=r.choice(P.SKINS), hair=r.choice(P.HAIRS), arms=('up', 'vee', 'down')[i], yaw=180.0)


def rodeo(sc):
    """A far rodeo on a pale ring: twenty on the bleachers jumping out of step, arms up; a bronco bucking in the
    middle with its rider waving a hat; two riders crossing the ring at a canter; a clown by the barrels; pennants."""
    r = random.Random(691)
    g = sc.still()
    S, Z = 3.0, 240.0
    pad(g, 9000.0, 5000.0, Z)
    for k in range(7):                                                                        # the ring's rail, on the road's side
        g.box(WHITE, (-3600.0 + k * 1200.0, -1700.0, Z + 150), (44, 44, 300))
    for rz in (130.0, 250.0):
        g.box(WHITE, (0, -1700.0, Z + rz), (7250, 26, 44))
    for k in range(3):                                                                        # three steps of bleachers at the back
        h = 320.0 + k * 300.0
        g.box((WOOD, BARK_D, WOOD)[k], (0, 1250.0 + k * 400.0, Z + h / 2.0), (7000, 400, h))
    for i in range(20):
        row = i % 3
        fx = -3150.0 + (i // 3) * 1000.0 + row * 330.0 + r.uniform(-80, 80)
        fan(sc, P.motion(phase=r.random(), rate=r.uniform(0.9, 1.5), bob=r.uniform(40, 110)), fx, 1250.0 + row * 400.0, Z + 320.0 + row * 300.0, S,
            HOT[(i * 5 + row) % len(HOT)], r.choice(P.SKINS), arms_up=(i % 3 != 1))
    FAR.beast(sc, 0.0, -300.0, Z, s=S * 1.1, coat=INK, mane=WHITE, tail=66, tail_w=18, neck_up=20, phase=0.0, rate=1.3, bob=110, stride=30, pitch=20,
              rider=FAR.hand_look(r, 0, 'hat', hat=CREAM, top=RED, swing=40.0))
    for k in range(2):
        FAR.beast(sc, -1800.0 + k * 3300.0, (-1000.0, 500.0)[k], Z, s=S, coat=(GOLD, WHITE)[k], mane=(WHITE, INK)[k], tail=62, tail_w=16, neck_up=46,
                  phase=0.3 + k * 0.4, rate=1.8, bob=26, stride=30, pitch=5, travel=(6600.0, 620.0), rider=FAR.hand_look(r, k + 1, ('flag', 'hat')[k], flag=MAG, stripe=WHITE, swing=14.0))
    for k in range(3):
        g.cyl((RED, CYAN, GOLD)[k], (-2500.0 + k * 2500.0, 150.0 - (k % 2) * 700.0, Z), 110.0, 280.0, steps=6)
    person(sc, P.motion(phase=r.random(), rate=1.6, bob=120.0), 1500.0, -900.0, Z, s=S, top=MAG, bottom=YEL, skin=r.choice(P.SKINS), hair=RED, arms='vee')
    for k in range(4):
        pennant(sc, r, -3600.0 + k * 2400.0, 2400.0, Z, 1750.0, (RED, GOLD, CYAN, MAG)[k], w=240.0)


BAL_A = ((-2600.0, 300.0, 6200.0, 1150.0, (RED, WHITE, GOLD), 0), (400.0, -200.0, 8200.0, 1300.0, (CYAN, NAVY, WHITE), 1),
         (3000.0, 500.0, 5200.0, 1050.0, (MAG, YEL, WHITE), 2))
BAL_B = ((-1900.0, 0.0, 7000.0, 1250.0, (GOLD, RED, NAVY), 1), (1700.0, 300.0, 5400.0, 1100.0, (WHITE, TEAL, MAG), 2),
         (200.0, 1500.0, 9000.0, 950.0, (VIOLET, WHITE, YEL), 0))

JOBS = [
    ('SM_P3E_Balloons_A', balloons(701, BAL_A), dict(far=True, grow=(300.0, 300.0, 500.0), grow_down=(300.0, 300.0, 500.0))),
    ('SM_P3E_Balloons_B', balloons(702, BAL_B), dict(far=True, grow=(300.0, 300.0, 500.0), grow_down=(300.0, 300.0, 500.0))),
    ('SM_P3E_HangGliders', hang_gliders, dict(far=True, grow=(4200.0, 4200.0, 400.0), grow_down=(4200.0, 4200.0, 400.0))),
    ('SM_P3E_ParaGliders', para_gliders, dict(far=True, grow=(4200.0, 4200.0, 400.0), grow_down=(4200.0, 4200.0, 400.0))),
    ('SM_P3E_Skydivers', skydivers, dict(far=True, grow=(3600.0, 3600.0, 700.0), grow_down=(3600.0, 3600.0, 700.0))),
    ('SM_P3E_Geese', geese, dict(far=True, grow=(21000.0, 200.0, 200.0), grow_down=(21000.0, 200.0, 200.0))),
    ('SM_P3E_Paramotors', paramotors, dict(far=True, grow=(25000.0, 300.0, 400.0), grow_down=(25000.0, 300.0, 400.0))),
    ('SM_P3E_Kites', kites, dict(far=True, grow=(1600.0, 200.0, 200.0), grow_down=(1600.0, 200.0, 0.0))),
    ('SM_P3E_DirtBikes', dirt_bikes, dict(far=True, grow=(300.0, 200.0, 500.0), grow_down=(300.0, 200.0, 0.0))),
    ('SM_P3E_LandYachts', land_yachts, dict(far=True, grow=(300.0, 300.0, 200.0), grow_down=(300.0, 300.0, 0.0))),
    ('SM_P3E_Climbers', climbers, dict(far=True, grow=(300.0, 200.0, 300.0), grow_down=(300.0, 200.0, 0.0))),
    ('SM_P3E_Rodeo', rodeo, dict(far=True, grow=(300.0, 200.0, 400.0), grow_down=(300.0, 200.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
