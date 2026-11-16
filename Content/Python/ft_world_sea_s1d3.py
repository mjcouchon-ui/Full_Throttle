"""Under the Sea, Stage 1, builder s1d3: district 3 "The research station" (3.4 to 5 km), the science campus.
/Game/World/UnderTheSea/SM_S1D3_*. Run: import ft_world_sea_s1d3 as K; K.run()

Clean, modular, round: white drums stacked on ONE central column with daylight between them, a geodesic greenhouse, a
glass specimen tank, a ring dormitory, flat roofs with dishes; everything stands on three splayed dark legs. White
#e6eef2 and station white #cfe3ea on steel #7fa2b4 and dark #3d4763 (the sand is pale: every white piece carries dark
legs and trim), cyan stripes (paint, unlit), magenta lights and cream ports (steady; only on pieces placed 34 m and
more out). Hero: THE BIG EAR, a sonar dish 40 m across nodding from side to side on a ringed lattice tower.
Every piece is one ft_people.Scene (one material, one section). Placed by C:/ftwork/s1/L6_s1d3.inl.

Not re-made from the kit (ft_dress_abyss.py): its station hull, hub (a solid stack of drums), dome habitat, pressure
habitat, pressure vessel, pipeline, travel tube, marker post, submarine. No fish, no creature, no person here; no fin
shape, nothing orange or green at box size, nothing that bobs and turns at 1 to 2 m.

A dish: the primitives' faces look outwards only, so a hollow cone seen from its mouth would be invisible. A dish
is a capped cone (the cap is its face) with three flat discs on the face, darker towards the middle: the hollow is drawn.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # rod(m, col, a, b, r, steps=), tilt_box(m, col, a, b, width, depth)

DIR = '/Game/World/UnderTheSea'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
LOWMOVE = dict(grow=(25, 25, 0), grow_down=(25, 25, 0))      # a low prop with a turning part: bounds stay under 2.5 m

WHITE, STN, STEEL, DARK = '#e6eef2', '#cfe3ea', '#7fa2b4', '#3d4763'
CYAN, MAG, LAMP, GLASS = '#2ee6ff', '#ff4fb8', '#fff1c0', '#cfeeff'
PORT, LAV, WEED, WEED_L, SAND = '#0a1a2a', '#b9a4ff', '#3f8a4a', '#7fcf6a', '#d9c89a'
TAU = 2.0 * math.pi
THIRD = TAU / 3.0


# ------------------------------------------------------------------------------------------ helpers
def pol(r, a, z, c=(0.0, 0.0)):
    """A point r out from c at angle a (radians), z up."""
    return (c[0] + math.cos(a) * r, c[1] + math.sin(a) * r, z)


def unit(x, y, z):
    n = math.sqrt(x * x + y * y + z * z) or 1.0
    return (x / n, y / n, z / n)


def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def cone(m, col, a, b, r0, r1, steps=10, glow=0.0):
    """A capped cone from a (radius r0) to b (radius r1): ft_desert.rod with two radii."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def hoop(m, col, r, z, n, w, d=None, c=(0.0, 0.0), a0=0.0, glow=0.0):
    """A ring of n straight bars (a rail, a rim, a stripe along a crown)."""
    pts = [pol(r, a0 + TAU * k / n, z, c) for k in range(n)]
    for p, q in zip(pts, pts[1:] + pts[:1]):
        tilt_box(m, col, p, q, w, d or w, glow=glow)


def legs3(m, c, r_top, z_top, r_foot, w=44.0, a0=0.5, z_foot=-150.0, col=DARK, pad=WHITE):
    """The district's mark: three splayed legs, a pale pad where each meets the sand (the leg goes on under it)."""
    for k in range(3):
        a = a0 + k * THIRD
        tilt_box(m, col, pol(r_top, a, z_top, c), pol(r_foot, a, z_foot, c), w, w)
        t = (z_top - 14.0) / (z_top - z_foot)
        m.box(pad, pol(r_top + (r_foot - r_top) * t, a, 4.0, c), (w * 2.6, w * 2.6, 48.0), rot=(0, math.degrees(a), 0))


def ports(m, r, z, n, a0=0.0, w=110.0, h=80.0, c=(0.0, 0.0), glow=0.9, col=LAMP):
    """Lit panes round a drum."""
    for k in range(n):
        a = a0 + TAU * k / n
        m.box(col, pol(r + 3.0, a, z, c), (10.0, w, h), rot=(0, math.degrees(a), 0), glow=glow)


def railing(m, r, z, n=8, h=90.0, c=(0.0, 0.0), col=STEEL, a0=0.0, t=8.0):
    for k in range(n):
        m.box(col, pol(r, a0 + TAU * k / n, z + h / 2.0, c), (t, t, h))
    hoop(m, col, r, z + h, n, t, t, c, a0)


def drum(m, col, z, r, h, n=12, c=(0.0, 0.0), stripe=CYAN, lit=6, a0=0.0):
    """One storey: a drum, a cyan stripe under its roof line, a dark recess beneath it, lit panes."""
    m.cyl(col, (c[0], c[1], z), r, h, steps=n)
    if stripe:
        m.cyl(stripe, (c[0], c[1], z + h - 70.0), r + 7.0, 30.0, steps=n)
    m.cyl(DARK, (c[0], c[1], z - 26.0), r - 50.0, 26.0, steps=n)
    if lit:
        ports(m, r, z + h * 0.42, lit, a0=a0, c=c)


def dish(m, apex, d, R, depth, n=8, back=WHITE, feed=MAG, glow=0.0):
    """A dish with its apex at `apex`, opening along the unit vector d (see the header)."""
    def f(t):
        return (apex[0] + d[0] * t, apex[1] + d[1] * t, apex[2] + d[2] * t)
    cone(m, back, apex, f(depth), R * 0.16, R, steps=n)
    rod(m, STEEL, f(depth), f(depth + 5.0), R * 0.62, steps=n)
    rod(m, DARK, f(depth), f(depth + 9.0), R * 0.26, steps=6)
    rod(m, STEEL, f(depth), f(depth + R * 0.7), max(R * 0.035, 4.0), steps=5)
    m.sphere(feed, f(depth + R * 0.7), max(R * 0.1, 9.0), steps=(3, 6), glow=glow)


# ------------------------------------------------------------------------------------------ buildings
def drum_lab(sc):
    """BUILDING. The laboratory: four drums on one column, 7 m apart at the feet, 27 m to the beacon. A lift capsule
    rides its shaft on the front; the roof dish nods."""
    g = sc.still()
    legs3(g, (0, 0), 170, 560, 560, w=50, a0=0.5)
    g.cyl(STEEL, (0, 0, -120), 105, 2320, steps=8)                         # the one column (seen between the drums)
    for z, r, h, col, a0 in ((520, 430, 320, WHITE, 0.0), (970, 520, 330, STN, 0.26), (1430, 400, 320, WHITE, 0.52), (1880, 465, 300, STN, 0.1)):
        drum(g, col, z, r, h, a0=a0)
    g.cyl(MAG, (0, 0, 472), 300, 18, steps=12, glow=1.2)                  # the ring light under the lowest drum
    g.cyl(DARK, (0, 0, 954), 610, 16, steps=12)                           # a gallery round the widest drum
    railing(g, 596, 970, n=8, h=85)
    g.cyl(DARK, (0, 0, 2180), 480, 14, steps=12)                          # the roof deck
    railing(g, 450, 2194, n=6, h=85, a0=0.3)
    g.cyl(DARK, (-200, 120, 2194), 9, 520, steps=5)                       # aerial and its steady beacon
    g.sphere(MAG, (-200, 120, 2730), 26, steps=(3, 6), glow=1.8)
    g.cyl(STEEL, (190, 150, 2194), 60, 110, steps=6)                      # a vent cowl
    g.cyl(DARK, (190, 150, 2304), 85, 26, steps=6)
    for x in (-70, 70):                                                    # the lift's two rails, out in front
        g.box(DARK, (x, -680, 1060), (14, 14, 2260))
    for z in (680, 1130, 1590, 2030):                                      # a gangway from the shaft to every storey
        g.box(STEEL, (0, -560, z), (120, 260, 12))
    lift = sc.part(P.motion(bob=-640, rate=0.045, phase=0.2))             # the capsule floats 6.4 m up and down
    lift.box(GLASS, (0, -690, 1150), (150, 110, 190))
    lift.box(MAG, (0, -690, 1254), (160, 120, 20), glow=0.8)
    lift.box(DARK, (0, -690, 1050), (160, 120, 14))
    g.cyl(DARK, (60, -40, 2194), 16, 140, steps=5)
    d = sc.part(P.motion(swing=14, pivot=(60, -40, 2334), axis=0.0, rate=0.07, phase=0.4))
    dish(d, (60, -40, 2334), unit(0.25, -0.62, 0.74), 190, 70, n=8)


def greenhouse(sc):
    """BUILDING. The greenhouse: a geodesic frame 10 m across on a round deck on three legs, glass over its crown,
    open below so the beds of green rows show; they sway. A magenta grow light under the crown."""
    g = sc.still()
    legs3(g, (0, 0), 300, 300, 640, w=46, a0=1.1)
    g.cyl(STEEL, (0, 0, -120), 120, 430, steps=8)
    g.cyl(DARK, (0, 0, 300), 540, 40, steps=10)
    g.cyl(STN, (0, 0, 340), 560, 24, steps=10)
    R, Z = 500.0, 364.0
    r0 = [pol(R, TAU * j / 10, Z) for j in range(10)]
    r1 = [pol(R * math.cos(0.66), TAU * (j + 0.5) / 10, Z + R * math.sin(0.66)) for j in range(10)]
    r2 = [pol(R * math.cos(1.17), TAU * j / 5, Z + R * math.sin(1.17)) for j in range(5)]
    apex = (0.0, 0.0, Z + R)

    def strut(a, b):
        tilt_box(g, WHITE, a, b, 14, 14)
    for j in range(10):
        strut(r0[j], r1[j])
        strut(r0[(j + 1) % 10], r1[j])
        strut(r1[j], r1[(j + 1) % 10])
    for j in range(5):
        strut(r2[j], r1[2 * j])
        strut(r2[j], r1[(2 * j - 1) % 10])
        strut(r2[j], r2[(j + 1) % 5])
        strut(r2[j], apex)
    z1, z2 = Z + R * math.sin(0.66), Z + R * math.sin(1.17)
    g.cyl(GLASS, (0, 0, z1), R * math.cos(0.66) * 0.95, z2 - z1, steps=10, top_r=R * math.cos(1.17) * 0.95, rot=(0, 18, 0))
    g.cyl(GLASS, (0, 0, z2), R * math.cos(1.17) * 0.95, R * 0.985 + Z - z2, steps=10, top_r=40)
    g.cyl(MAG, (0, 0, z1 - 30), 300, 14, steps=10, glow=1.0)             # the grow light
    g.cyl(STEEL, (0, 0, Z + R - 24), 46, 60, steps=6)                     # the vent at the crown
    g.cyl(DARK, (0, 0, Z + R + 36), 66, 18, steps=6)
    r = random.Random(31)
    for i, y in enumerate((-250, 0, 250)):                                 # three beds, three green rows a bed
        half = math.sqrt(430.0 ** 2 - y * y) * 0.92
        g.box(DARK, (0, y, Z + 24), (half * 2, 110, 48))
        p = sc.part(P.motion(swing=3.5, pivot=(0, y, Z + 48), axis=0.0, rate=0.22 + 0.04 * i, phase=0.3 * i))
        for k in range(3):
            h = r.uniform(130, 250)
            x = -half * 0.66 + k * half * 0.66
            p.box(WEED_L if (k + i) % 2 else WEED, (x, y, Z + 48 + h / 2.0), (half * 0.56, 70, h))
            if (k + i) % 2 == 0:
                p.sphere(LAV if k else MAG, (x, y, Z + 48 + h + 16), 34, steps=(3, 5))
    rod(g, STN, (0, -440, Z + 112), (0, -640, Z + 112), 105, steps=8)     # the airlock, towards the road
    rod(g, MAG, (0, -640, Z + 112), (0, -652, Z + 112), 116, steps=8, glow=0.9)
    tilt_box(g, DARK, (0, -645, Z + 6), (0, -980, -40), 130, 14)           # its ramp down to the sand


def tank_hall(sc):
    """BUILDING. The specimen tank: 21 m of glass on two tripods, weed swaying in it (no fish), a strip light under
    the roof, blank tags under each bay, a pump cabinet at one end and stairs at the other."""
    g = sc.still()
    legs3(g, (-680, 0), 150, 300, 400, w=44, a0=0.52)
    legs3(g, (680, 0), 150, 300, 400, w=44, a0=0.52 + math.pi)
    g.box(DARK, (0, 0, 325), (2200, 560, 50))
    g.box(GLASS, (0, 0, 650), (2140, 500, 600))
    g.box(WHITE, (0, 0, 985), (2260, 620, 70))
    g.box(CYAN, (0, -312, 985), (2260, 6, 22))
    g.box(LAMP, (0, -256, 936), (2100, 8, 22), glow=0.8)
    for k in range(6):
        x = -1070 + k * 428
        for y in (-262, 262):
            g.box(DARK, (x, y, 650), (36, 36, 600))
    g.box(SAND, (0, -253, 386), (2100, 8, 72))                              # the tank's bed, seen through the front
    r = random.Random(63)
    for bay in range(5):
        x0 = -856 + bay * 428
        g.box(WHITE, (x0, -284, 325), (120, 6, 34))                         # a blank tag under each bay
        for j in range(3):
            x = x0 + (j - 1) * 110 + r.uniform(-30, 30)
            h = r.uniform(240, 500)
            p = sc.part(P.motion(swing=r.uniform(4, 7), pivot=(x, -257, 422), axis=90.0, rate=r.choice((0.16, 0.2, 0.24)), phase=round(r.random(), 2)))
            tilt_box(p, (WEED, WEED_L, WEED)[j], (x, -257, 422), (x + r.uniform(-30, 30), -257, 422 + h), 8, 46)
        g.sphere(r.choice((LAV, MAG, LAMP)), (x0 + r.uniform(-120, 120), -256, 432), 36, scale=(1.5, 0.22, 1.0), steps=(3, 6))
    g.box(STN, (1185, 0, 560), (160, 420, 420))                             # the pump cabinet and its pipe to the roof
    g.box(MAG, (1185, -214, 700), (120, 8, 30), glow=0.8)
    rod(g, STEEL, (1185, 100, 770), (1185, 100, 1100), 28, steps=6)
    rod(g, STEEL, (1185, 100, 1100), (880, 100, 1100), 28, steps=6)
    g.cyl(STEEL, (880, 100, 1020), 34, 90, steps=6)
    for k in range(6):                                                       # the roof rail along the front
        g.box(STEEL, (-1070 + k * 428, -296, 1065), (8, 8, 90))
    g.box(STEEL, (0, -296, 1110), (2180, 8, 8))
    g.cyl(STEEL, (300, 0, 1020), 90, 40, steps=8)                            # the filling hatch and its wheel
    g.cyl(MAG, (300, 0, 1060), 60, 12, steps=8)
    g.cyl(DARK, (-700, 60, 1020), 14, 90, steps=5)
    dish(g, (-700, 60, 1110), unit(0.2, -0.5, 0.84), 150, 55, n=8)
    tilt_box(g, DARK, (-1010, -285, 330), (-1010, -700, -40), 110, 14)      # stairs to the sand
    tilt_box(g, STEEL, (-1066, -285, 420), (-1066, -700, 50), 8, 8)


def ring_dorm(sc):
    """BUILDING. The dormitory: a doughnut 15 m across on three legs, a hub with a glass bubble, a cyan line along
    its crown, lit ports all round."""
    g = sc.still()
    RM, RT, Z, n = 560.0, 185.0, 820.0, 12
    pts = [pol(RM, TAU * k / n, Z) for k in range(n)]
    ext = RT * math.tan(math.pi / n)
    for k in range(n):
        a, b = pts[k], pts[(k + 1) % n]
        L = math.hypot(b[0] - a[0], b[1] - a[1])
        u = ((b[0] - a[0]) / L, (b[1] - a[1]) / L)
        rod(g, WHITE if k % 2 else STN, (a[0] - u[0] * ext, a[1] - u[1] * ext, Z), (b[0] + u[0] * ext, b[1] + u[1] * ext, Z), RT, steps=8)
        am = TAU * (k + 0.5) / n
        g.box(LAMP, pol(RM * math.cos(math.pi / n) + RT * 0.96 + 3.0, am, Z + 30), (10, 120, 60), rot=(0, math.degrees(am), 0), glow=0.9)
    hoop(g, CYAN, RM, Z + RT + 1, n, 30, 10)
    legs3(g, (0, 0), RM, Z - RT + 20, 800, w=50, a0=0.5)
    g.cyl(STEEL, (0, 0, -120), 150, 1000, steps=8)                          # the hub
    g.cyl(WHITE, (0, 0, Z - 110), 240, 230, steps=10)
    g.cyl(CYAN, (0, 0, Z + 80), 246, 24, steps=10)
    g.sphere(GLASS, (0, 0, Z + 150), 170, scale=(1, 1, 0.7), steps=(4, 8))
    g.cyl(MAG, (0, 0, Z - 150), 210, 16, steps=10, glow=1.2)
    for k in range(3):
        a = 0.5 + math.pi / 3 + k * THIRD
        rod(g, STN, pol(200, a, Z), pol(RM - RT + 30, a, Z), 62, steps=6)
    g.cyl(DARK, (0, 0, Z + 250), 8, 300, steps=5)
    g.sphere(MAG, (0, 0, Z + 566), 24, steps=(3, 6), glow=1.8)
    tilt_box(g, DARK, (0, -RM, Z - RT + 10), (0, -RM - 200, -40), 60, 12)   # the ladder
    top = pol(RM, 2.2, Z + RT - 6)
    g.cyl(DARK, top, 12, 80, steps=5)
    d = sc.part(P.motion(swing=30, pivot=(top[0], top[1], top[2] + 80), axis=P.UP, rate=0.05, phase=0.7))
    dish(d, (top[0], top[1], top[2] + 80), unit(-0.3, -0.5, 0.81), 120, 44, n=8)


def workshop(sc):
    """BUILDING. The workshop: a white hall with a roller door, a lit window under a cyan awning, air bottles at the
    gable, and on the roof a gantry arm that swings a specimen cage out over the yard."""
    g = sc.still()
    g.box(DARK, (0, 0, -40), (1240, 840, 220))
    g.box(WHITE, (0, 0, 330), (1200, 800, 520))
    g.box(STN, (0, 0, 610), (1260, 860, 40))
    g.box(CYAN, (0, -432, 610), (1264, 6, 16))
    g.box(DARK, (-250, -402, 270), (480, 10, 400))                           # the roller door
    for k in range(5):
        g.box(STEEL, (-250, -408, 112 + k * 76), (460, 8, 52))
    g.box(MAG, (-250, -406, 492), (500, 8, 22), glow=0.8)
    g.box(PORT, (330, -402, 380), (420, 8, 130))                             # the window
    for k in range(3):
        g.box(LAMP, (210 + k * 120, -407, 380), (96, 6, 100), glow=0.8)
    tilt_box(g, CYAN, (330, -400, 494), (330, -548, 452), 460, 10)           # its awning
    g.box(WHITE, (-540, -404, 470), (96, 6, 96))                             # a picture sign: a gear (no words)
    rod(g, DARK, (-540, -407, 470), (-540, -413, 470), 34, steps=8)
    rod(g, WHITE, (-540, -413, 470), (-540, -417, 470), 13, steps=6)
    g.box(DARK, (690, -50, 80), (150, 520, 20))                              # air bottles in their rack
    for k in range(3):
        g.cyl(WHITE, (690, -200 + k * 150, 90), 55, 320, steps=8)
        g.cyl(CYAN, (690, -200 + k * 150, 410), 38, 40, steps=8)
    for x, y in ((-380, 180), (60, 220)):                                    # roof vents and a skylight
        g.cyl(STEEL, (x, y, 630), 70, 90, steps=6)
        g.cyl(DARK, (x, y, 720), 100, 24, steps=6)
    g.box(GLASS, (-180, -80, 650), (380, 260, 60))
    for k in range(3):                                                       # the gantry: a tripod, a post, the arm
        tilt_box(g, DARK, (420, -250, 800), pol(170, 0.5 + k * THIRD, 630, (420, -250)), 16, 16)
    g.cyl(DARK, (420, -250, 630), 34, 530, steps=6)
    j = sc.part(P.motion(swing=24, pivot=(420, -250, 1100), axis=P.UP, rate=0.05, phase=0.3))
    tilt_box(j, STEEL, (420, -250, 1110), (420, -1000, 1180), 40, 50)
    tilt_box(j, STEEL, (420, -250, 1110), (420, 30, 1095), 40, 50)
    j.box(DARK, (420, 60, 1085), (110, 110, 90))
    tilt_box(j, DARK, (420, -250, 1250), (420, -820, 1180), 8, 8)
    j.box(DARK, (420, -250, 1190), (30, 30, 130))
    j.box(DARK, (420, -960, 950), (8, 8, 420))
    j.box(WHITE, (420, -960, 730), (70, 50, 60))
    j.box(STEEL, (420, -960, 640), (130, 130, 110))                          # the cage (steel, 1.3 m, 6 m up)
    j.sphere(LAV, (420, -960, 640), 40, steps=(3, 6))


def current_tower(sc):
    """BUILDING. The weather-and-current tower: a lattice tripod to a 30 m mast, a cabin drum half-way, three sets of
    cups turning at three speeds, a lavender sock, and the vane at the top swinging to and fro."""
    g = sc.still()
    A0 = 0.5

    def rr(z):
        return 400.0 - 330.0 * (z + 150.0) / 1650.0
    for k in range(3):
        tilt_box(g, DARK, pol(400, A0 + k * THIRD, -150), pol(70, A0 + k * THIRD, 1500), 40, 40)
        g.box(WHITE, pol(rr(10), A0 + k * THIRD, 4), (110, 110, 48), rot=(0, math.degrees(A0 + k * THIRD), 0))
    lv = (350, 800, 1200)
    for z in lv:
        hoop(g, STEEL, rr(z), z, 3, 18, 18, a0=A0)
    for z_a, z_b in zip(lv, lv[1:]):
        for k in range(3):
            tilt_box(g, STEEL, pol(rr(z_a), A0 + k * THIRD, z_a), pol(rr(z_b), A0 + (k + 1) * THIRD, z_b), 14, 14)
    g.cyl(STEEL, (0, 0, -100), 26, 3060, steps=6)                           # the mast, to 29.6 m
    g.cyl(WHITE, (0, 0, 800), 300, 30, steps=10)
    railing(g, 280, 830, n=6, h=85)
    drum(g, WHITE, 1500, 230, 300, n=10, lit=5, a0=0.3)
    g.cyl(DARK, (0, 0, 1800), 270, 20, steps=10)
    railing(g, 250, 1820, n=6, h=85, a0=0.5)
    g.cyl(MAG, (0, 0, 1462), 150, 14, steps=10, glow=1.2)
    tilt_box(g, DARK, (0, -290, 815), (0, -430, -40), 50, 10)
    for i, (z, L, rate) in enumerate(((2100, 230, 0.15), (2450, 190, -0.2), (2800, 150, 0.25))):
        g.cyl(DARK, (0, 0, z - 14), 40, 28, steps=6)
        c = sc.part(P.motion(travel=(-1.0, rate), base=(0.0, 0.0), phase=0.1 * i))
        for k in range(3):
            a = k * THIRD + i
            tilt_box(c, STEEL, (0, 0, z), pol(L, a, z), 12, 12)
            c.sphere(MAG if k == 0 else WHITE, pol(L, a, z), 46, steps=(3, 6))
    v = sc.part(P.motion(swing=50, pivot=(0, 0, 2960), axis=P.UP, rate=0.06, phase=0.1))
    tilt_box(v, DARK, (-260, 0, 2960), (300, 0, 2960), 16, 16)
    v.box(CYAN, (-225, 0, 2962), (110, 8, 90))
    cone(v, MAG, (300, 0, 2960), (400, 0, 2960), 34, 6, steps=6)
    s = sc.part(P.motion(swing=18, pivot=(0, 0, 1960), axis=P.UP, rate=0.11, phase=0.6))
    cone(s, LAV, (30, 0, 1960), (330, 0, 1930), 52, 20, steps=6)


def deck_house(sc):
    """BUILDING. The deck house: a wide drum with a dark window band on three legs, a roof deck with a rail, a
    penthouse drum, two dishes (one nods), a telescope, a planter, a cyan canopy."""
    g = sc.still()
    legs3(g, (0, 0), 300, 300, 560, w=46, a0=0.2)
    g.cyl(STEEL, (0, 0, -120), 130, 440, steps=8)
    drum(g, WHITE, 300, 520, 300, n=12, lit=0)
    g.cyl(PORT, (0, 0, 400), 526, 110, steps=12)
    ports(g, 525, 455, 8, a0=0.2, w=150, h=70)
    g.cyl(MAG, (0, 0, 258), 260, 14, steps=12, glow=1.1)
    g.cyl(DARK, (0, 0, 600), 560, 16, steps=12)
    railing(g, 540, 616, n=8, h=85)
    drum(g, STN, 616, 300, 240, n=10, c=(-120, 90), lit=4, a0=0.6)
    g.cyl(DARK, (-120, 90, 856), 330, 14, steps=10)
    g.cyl(DARK, (-200, 60, 870), 14, 120, steps=5)
    d = sc.part(P.motion(swing=12, pivot=(-200, 60, 990), axis=0.0, rate=0.08, phase=0.2))
    dish(d, (-200, 60, 990), unit(0.1, -0.6, 0.8), 150, 55, n=8)
    g.cyl(DARK, (-30, 190, 870), 12, 70, steps=5)
    dish(g, (-30, 190, 940), unit(-0.4, -0.3, 0.86), 100, 40, n=8)
    g.cyl(DARK, (-290, 200, 870), 7, 330, steps=5)
    g.sphere(MAG, (-290, 200, 1214), 22, steps=(3, 6), glow=1.8)
    g.cyl(WHITE, (300, -250, 616), 60, 50, steps=6)                          # a planter on the deck
    g.sphere(LAV, (300, -250, 694), 50, steps=(3, 6))
    g.sphere(MAG, (336, -226, 726), 28, steps=(3, 5))
    g.cyl(DARK, (330, 180, 616), 8, 110, steps=5)                            # a telescope
    tilt_box(g, WHITE, (300, 200, 716), (384, 150, 772), 26, 26)
    for x, y in ((120, -360), (400, -360), (120, -120), (400, -120)):        # a canopy over the deck's front corner
        g.box(STEEL, (x, y, 716), (8, 8, 200))
    g.box(CYAN, (260, -240, 820), (330, 290, 10))


# ------------------------------------------------------------------------------------------ props
def tripod(sc):
    """PROP, low (1.9 m). An instrument tripod whose head pans, a banded staff, an open case, two pins with blank tags."""
    g = sc.still()
    for k in range(3):
        tilt_box(g, DARK, (0, 0, 128), pol(62, 0.5 + k * THIRD, -25), 9, 9)
    g.cyl(STEEL, (0, 0, 120), 14, 22, steps=6)
    h = sc.part(P.motion(swing=22, pivot=(0, 0, 150), axis=P.UP, rate=0.08, phase=0.3))
    h.box(WHITE, (0, 0, 158), (34, 46, 32))
    rod(h, DARK, (0, -23, 160), (0, -52, 160), 11, steps=6)
    h.box(CYAN, (0, -54, 160), (14, 5, 14))
    h.box(MAG, (0, 8, 179), (22, 22, 10))
    for i in range(6):
        g.box(MAG if i % 2 else WHITE, (170, 20, -3.5 + i * 33), (9, 7, 33))
    g.box(DARK, (170, 20, 182), (16, 12, 8))
    g.box(WHITE, (-150, 10, 10), (70, 46, 34))
    g.box(DARK, (-150, 10, 29), (72, 48, 5))
    g.box(CYAN, (-150, -15, 12), (40, 5, 8))
    for x in (-60, 95):
        g.box(DARK, (x, -40, 10), (5, 5, 60))
        g.box(WHITE, (x, -41, 34), (22, 5, 14))


def sample_rack(sc):
    """PROP, low (1.7 m). Three shelves of sample jars under a little roof: pale glass, lids in magenta, lavender,
    cream, cyan; a blank tag; a jar of weed on the sand beside it."""
    g = sc.still()
    r = random.Random(7)
    L = 300.0
    for x in (-L / 2.0, 0.0, L / 2.0):
        g.box(DARK, (x, 0, 72), (8, 50, 185))
    for z in (30, 78, 126):
        g.box(WHITE, (0, 0, z), (L + 10, 54, 6))
    g.box(STN, (0, 0, 168), (L + 30, 62, 8))
    g.box(CYAN, (0, -32, 168), (L + 30, 5, 8))
    for z in (33, 81, 129):
        for k in range(6):
            x = -125 + k * 50
            g.cyl(GLASS, (x, -4, z), 14, 30, steps=5)
            g.box(r.choice((MAG, LAV, LAMP, CYAN, WHITE)), (x, -4, z + 33), (22, 22, 7))
    g.box(WHITE, (75, -30, 152), (60, 5, 14))
    g.cyl(GLASS, (205, 0, -12), 30, 84, steps=6)
    p = sc.part(P.motion(swing=8, pivot=(205, 0, 70), axis=90.0, rate=0.25, phase=0.4))
    p.box(WEED_L, (205, 0, 104), (8, 26, 64))
    p.box(WEED, (214, 6, 96), (6, 18, 48))


def lockers(sc):
    """PROP, low (1.93 m). Five white equipment lockers in a row on a dark plinth, a cyan stripe, vents and handles; a
    bench in front with a helmet and a folded towel."""
    g = sc.still()
    g.box(DARK, (0, 0, -5), (470, 66, 34))
    for k in range(5):
        x = -184 + k * 92
        g.box(STN if k % 2 else WHITE, (x, 0, 100), (88, 58, 176))
        g.box(CYAN, (x, -30, 160), (88, 5, 10))
        g.box(DARK, (x, -30, 60), (50, 5, 26))
        g.box(DARK, (x + 30, -31, 110), (6, 6, 22))
    g.box(STN, (0, -4, 190), (480, 74, 6))
    g.box(DARK, (-60, -75, 42), (220, 34, 6))
    for x in (-150, 30):
        g.box(DARK, (x, -75, 12), (8, 30, 60))
    g.sphere(WHITE, (-100, -75, 62), 17, steps=(3, 6))
    g.box(PORT, (-100, -90, 62), (22, 6, 12))
    g.box(LAV, (0, -75, 49), (40, 26, 8))


def cable_drum(sc):
    """PROP, low (1.82 m). A cable drum on an A-frame: white flanges, turns of cyan cable, its end run along the sand
    to a small junction box."""
    g = sc.still()
    rod(g, DARK, (-46, 0, 98), (46, 0, 98), 52, steps=10)
    for k in range(4):
        rod(g, CYAN, (-41 + k * 24, 0, 98), (-31 + k * 24, 0, 98), 55, steps=10)
    for s in (-1, 1):
        rod(g, WHITE, (s * 46, 0, 98), (s * 56, 0, 98), 84, steps=10)
        rod(g, MAG, (s * 56, 0, 98), (s * 61, 0, 98), 22, steps=6)
        for y in (-1, 1):
            tilt_box(g, DARK, (s * 68, 0, 100), (s * 68, y * 70, -25), 9, 9)
    for y in (-70, 70):
        g.box(DARK, (0, y, 4), (150, 9, 9))
    tilt_box(g, CYAN, (30, -50, 62), (120, -60, 6), 8, 8)
    tilt_box(g, CYAN, (120, -60, 6), (232, -20, 6), 8, 8)
    g.box(WHITE, (250, -10, 14), (44, 34, 40))
    g.box(MAG, (250, -28, 22), (16, 5, 10))


def manifold(sc):
    """PROP, low (1.76 m). A valve manifold: a white pipe on dark saddles, three risers with magenta wheels (the
    middle one turns to and fro), two gauges, a small tank, an elbow into the sand."""
    g = sc.still()
    rod(g, WHITE, (-170, 0, 72), (170, 0, 72), 30, steps=8)
    for x in (-120, 120):
        g.box(DARK, (x, 0, 18), (34, 90, 76))
    for s in (-1, 1):
        rod(g, STEEL, (s * 170, 0, 72), (s * 182, 0, 72), 40, steps=8)
    for i, x in enumerate((-70, 0, 70)):
        g.cyl(STEEL, (x, 0, 72), 12, 60 + i * 14, steps=6)
        z = 134 + i * 14
        w = sc.part(P.motion(swing=80, pivot=(x, 0, z), axis=P.UP, rate=0.06, phase=0.3)) if i == 1 else g
        w.cyl(MAG, (x, 0, z), 30, 7, steps=8)
        w.box(MAG, (x, 0, z + 4), (64, 7, 7))
        w.box(MAG, (x, 0, z + 4), (7, 64, 7))
    for x in (-35, 35):
        g.cyl(STEEL, (x, -10, 72), 7, 62, steps=5)
        rod(g, WHITE, (x, -14, 146), (x, -22, 146), 18, steps=8)
        g.box(MAG, (x, -23, 150), (5, 4, 14))
    g.cyl(STN, (240, 20, -20), 46, 150, steps=8)
    g.sphere(STN, (240, 20, 130), 46, scale=(1, 1, 0.6), steps=(3, 8))
    g.cyl(CYAN, (240, 20, 90), 48, 12, steps=8)
    rod(g, WHITE, (-182, 0, 72), (-232, 0, 72), 30, steps=8)
    g.cyl(WHITE, (-232, 0, -25), 30, 97, steps=8)


def current_vane(sc):
    """PROP, tall (5 m). A current vane on a pole: the arrow swings to and fro, three cups turn under it; an
    instrument box with a blank tag."""
    g = sc.still()
    for k in range(3):
        tilt_box(g, DARK, (0, 0, 120), pol(90, 0.5 + k * THIRD, -30), 12, 12)
    g.cyl(STEEL, (0, 0, -30), 9, 500, steps=6)
    g.box(WHITE, (0, -16, 150), (44, 26, 60))
    g.box(DARK, (0, -30, 150), (30, 5, 40))
    g.box(CYAN, (0, -30, 176), (44, 5, 6))
    g.box(WHITE, (0, -12, 95), (30, 5, 18))
    g.cyl(DARK, (0, 0, 390), 14, 22, steps=6)
    c = sc.part(P.motion(travel=(-1.0, 0.2), base=(0.0, 0.0)))
    for k in range(3):
        tilt_box(c, STEEL, (0, 0, 400), pol(70, k * THIRD, 400), 8, 8)
        c.sphere(MAG if k == 0 else WHITE, pol(70, k * THIRD, 400), 22, steps=(3, 6))
    v = sc.part(P.motion(swing=40, pivot=(0, 0, 480), axis=P.UP, rate=0.07, phase=0.2))
    tilt_box(v, DARK, (-120, 0, 480), (140, 0, 480), 10, 10)
    v.box(CYAN, (-100, 0, 482), (70, 6, 56))
    cone(v, MAG, (140, 0, 480), (200, 0, 480), 20, 6, steps=6)
    v.cyl(DARK, (0, 0, 466), 14, 30, steps=6)


def flood_mast(sc):
    """PROP, tall (9 m). A floodlight mast on a tripod: four lamp heads under hoods (steady), a cyan band, a small
    magenta ring, rungs, a control box."""
    g = sc.still()
    for k in range(3):
        tilt_box(g, DARK, (0, 0, 260), pol(120, 0.5 + k * THIRD, -40), 14, 14)
    g.cyl(DARK, (0, 0, -40), 15, 900, steps=6, top_r=10)
    g.cyl(CYAN, (0, 0, 300), 18, 26, steps=6)
    g.cyl(MAG, (0, 0, 560), 16, 20, steps=6, glow=1.0)
    g.box(STEEL, (0, 0, 852), (270, 16, 16))
    for x in (-110, -37, 37, 110):
        g.box(WHITE, (x, -6, 884), (56, 44, 46))
        g.box(LAMP, (x, -30, 880), (44, 5, 34), glow=1.6)
        g.box(DARK, (x, -4, 910), (62, 56, 6))
    for k in range(6):
        g.box(STEEL, (0, -16, 330 + k * 70), (40, 6, 6))
    g.box(WHITE, (0, -24, 130), (40, 24, 56))
    g.box(MAG, (0, -37, 140), (12, 5, 8))
    tilt_box(g, DARK, (0, -12, 100), (60, -90, -20), 6, 6)


def sign_post(sc):
    """PROP, tall (3.6 m). A way post with three picture boards (a dome, a dish, a flask: no words) and a blank tag."""
    g = sc.still()
    g.cyl(DARK, (0, 0, -30), 10, 370, steps=6)
    g.cyl(WHITE, (0, 0, -30), 26, 44, steps=6)
    g.sphere(MAG, (0, 0, 350), 15, steps=(3, 6))
    g.side(WHITE, _ccw([(8, 268), (150, 268), (192, 292), (150, 316), (8, 316)]), -5, 5)        # to the domes
    rod(g, CYAN, (70, -5, 286), (70, -10, 286), 19, steps=8)
    g.box(DARK, (70, -8, 274), (56, 6, 7))
    g.side(STN, _ccw([(-8, 212), (-150, 212), (-192, 236), (-150, 260), (-8, 260)]), -5, 5)     # to the dish
    rod(g, MAG, (-84, -5, 240), (-84, -10, 240), 15, steps=8)
    g.box(DARK, (-84, -8, 222), (6, 6, 18))
    g.side(WHITE, _ccw([(8, 156), (150, 156), (192, 180), (150, 204), (8, 204)]), -5, 5)        # to the laboratory
    g.side(LAV, _ccw([(54, 164), (96, 164), (75, 198)]), -10, -5)
    g.box(WHITE, (0, -13, 110), (60, 5, 40))
    g.box(CYAN, (0, -15, 124), (60, 5, 6))


def antenna_farm(sc):
    """PROP, tall (6.7 m). Three small dishes on poles of three heights on a dark skid: one nods, one pans; a whip
    aerial with a steady magenta bead, a white cabinet."""
    g = sc.still()
    g.box(DARK, (0, 0, -5), (300, 110, 30))
    for x in (-150, 150):
        g.box(DARK, (x, 0, -12), (16, 150, 22))
    for x, h, R, d, mv in ((-100, 380, 90, (0.2, -0.55, 0.81), 'nod'), (20, 560, 120, (-0.3, -0.5, 0.81), 'pan'), (115, 250, 70, (0.5, -0.4, 0.77), None)):
        g.cyl(STEEL, (x, 0, 10), 9, h - 10, steps=6)
        if mv == 'nod':
            m = sc.part(P.motion(swing=12, pivot=(x, 0, h), axis=0.0, rate=0.09, phase=0.2))
        elif mv == 'pan':
            m = sc.part(P.motion(swing=35, pivot=(x, 0, h), axis=P.UP, rate=0.05, phase=0.6))
        else:
            m = g
        dish(m, (x, 0, h), unit(*d), R, R * 0.35, n=8)
    g.box(WHITE, (30, -40, 40), (60, 30, 60))
    g.box(CYAN, (30, -56, 58), (60, 5, 6))
    g.cyl(DARK, (140, 30, 10), 5, 420, steps=5)
    g.sphere(MAG, (140, 30, 440), 13, steps=(3, 6), glow=1.4)
    tilt_box(g, DARK, (20, 0, 330), (-130, 40, 8), 5, 5)


# ------------------------------------------------------------------------------------------ the hero
def big_ear(sc):
    """HERO. THE BIG EAR: a sonar dish 40 m across on a ringed lattice tower, 61 m in all, on three great splayed
    legs. The dish nods slowly from side to side about its trunnion; a lift car rides the tower; five ring galleries
    (cyan stripes and steady magenta rings in turn), the listening room under the yoke, eight magenta lights on the rim."""
    g = sc.still()
    A0 = math.radians(30)                                                    # legs at 30, 150 and 270 degrees: one towards the road
    for k in range(3):
        a = A0 + k * THIRD
        tilt_box(g, DARK, pol(520, a, 1500), pol(1800, a, -300), 200, 200)
        tilt_box(g, STEEL, pol(1250, a, 480), pol(560, a, 60), 70, 70)       # a tie from each leg to the tower's foot
        g.cyl(WHITE, pol(1590, a, -150), 320, 300, steps=10)                 # the foot drums
        g.cyl(CYAN, pol(1590, a, 92), 328, 34, steps=10)
    r0, r1, z0, z1 = 640.0, 380.0, -200.0, 3600.0

    def rr(z):
        return r0 + (r1 - r0) * (z - z0) / (z1 - z0)
    for k in range(6):                                                       # six corner posts leaning in
        tilt_box(g, STEEL, pol(r0, k * TAU / 6, z0), pol(r1, k * TAU / 6, z1), 50, 50)
    levels = (500, 1200, 1900, 2600, 3300)
    for i, z in enumerate(levels):                                           # the rings
        g.cyl(WHITE if i % 2 == 0 else STN, (0, 0, z), rr(z) + 110, 70, steps=12)
        g.cyl(CYAN if i % 2 == 0 else MAG, (0, 0, z - 30), rr(z) + 118, 30, steps=12, glow=(0.0 if i % 2 == 0 else 1.0))
    for i in (1, 3):
        railing(g, rr(levels[i]) + 96, levels[i] + 70, n=6, h=100, t=10)
    for i in range(len(levels) - 1):                                         # the lattice between them
        za, zb = levels[i] + 70.0, levels[i + 1] - 30.0
        for k in range(6):
            a, b = k * TAU / 6, (k + 1) * TAU / 6
            if (k + i) % 2 == 0:
                tilt_box(g, STEEL, pol(rr(za), a, za), pol(rr(zb), b, zb), 28, 28)
            else:
                tilt_box(g, STEEL, pol(rr(za), b, za), pol(rr(zb), a, zb), 28, 28)
    g.cyl(STN, (0, 0, -250), 150, 3900, steps=8)                             # the lift column
    lift = sc.part(P.motion(bob=-1250, rate=0.02, phase=0.1))
    lift.box(GLASS, (0, -200, 1900), (170, 110, 230))
    lift.box(MAG, (0, -200, 2025), (180, 120, 20), glow=0.8)
    for z, a in ((1270, 2.4), (2670, 0.7)):                                  # a cabin on two of the rings
        c = pol(rr(z) + 20, a, z + 90)
        g.box(WHITE, c, (200, 200, 180), rot=(0, math.degrees(a), 0))
        g.box(LAMP, pol(rr(z) + 122, a, z + 100), (8, 120, 70), rot=(0, math.degrees(a), 0), glow=0.9)
    drum(g, WHITE, 3600, 520, 380, n=12, lit=8)                              # the listening room
    g.cyl(MAG, (0, 0, 3556), 400, 22, steps=12, glow=1.2)
    g.cyl(DARK, (0, 0, 3980), 560, 30, steps=12)
    railing(g, 536, 4010, n=8, h=110, t=12)
    PZ = 4350.0
    for s in (-1, 1):                                                        # the yoke and its trunnion
        tilt_box(g, DARK, (s * 330, 60, 4010), (s * 430, 60, PZ + 50), 150, 110)
    rod(g, STEEL, (-480, 60, PZ), (480, 60, PZ), 60, steps=8)
    g.cyl(DARK, (-380, -300, 4010), 12, 150, steps=5)                        # a small fixed dish and an aerial on the roof
    dish(g, (-380, -300, 4160), unit(-0.5, -0.5, 0.7), 130, 46, n=8)
    g.cyl(DARK, (380, 330, 4010), 9, 420, steps=5)
    g.sphere(MAG, (380, 330, 4444), 26, steps=(3, 6), glow=1.8)
    # ---- the dish: its axis leans 38 degrees from straight up, towards the road
    dsh = sc.part(P.motion(swing=8, pivot=(0, 60, PZ), axis=90.0, rate=0.04, phase=0.0))
    t = math.radians(38)
    d = (0.0, -math.sin(t), math.cos(t))
    v = (0.0, math.cos(t), math.sin(t))
    R, depth = 2000.0, 620.0

    def at(along, a=0.0, rad=0.0):
        return (math.cos(a) * rad, 60.0 + d[1] * along + math.sin(a) * rad * v[1], PZ + d[2] * along + math.sin(a) * rad * v[2])
    cone(dsh, WHITE, at(-60), at(depth), 260, R, steps=16)                   # the shell
    rod(dsh, STN, at(depth), at(depth + 14), R * 0.86, steps=16)             # the face: three discs, darker inwards
    rod(dsh, STEEL, at(depth), at(depth + 26), R * 0.6, steps=16)
    rod(dsh, DARK, at(depth), at(depth + 38), R * 0.3, steps=12)
    for k in range(16):                                                      # the rim
        tilt_box(dsh, DARK, at(depth + 10, k * TAU / 16, R), at(depth + 10, (k + 1) * TAU / 16, R), 90, 90)
    for k in range(8):
        dsh.box(MAG, at(depth + 66, (k + 0.5) * TAU / 8, R), (70, 70, 70), glow=1.6)
        tilt_box(dsh, STEEL, at(-10, k * TAU / 8, 330), at(depth - 14, k * TAU / 8, R + 26), 60, 40)    # ribs on the back
    for k in range(3):                                                       # the feed on its three struts
        tilt_box(dsh, STEEL, at(depth, 0.5 + k * THIRD, R * 0.95), at(depth + 1500), 30, 30)
    cone(dsh, DARK, at(depth + 1520), at(depth + 1330), 60, 130, steps=8)
    dsh.sphere(MAG, at(depth + 1540), 80, steps=(4, 8), glow=2.0)
    dsh.box(DARK, at(-250), (520, 300, 300))                                 # the counterweight


JOBS = [
    ('SM_S1D3_DrumLab', drum_lab, dict(grow=(40, 40, 0), grow_down=(40, 40, 0))),
    ('SM_S1D3_Greenhouse', greenhouse, STILL),
    ('SM_S1D3_TankHall', tank_hall, STILL),
    ('SM_S1D3_RingDorm', ring_dorm, dict(grow=(40, 40, 20), grow_down=(40, 40, 0))),
    ('SM_S1D3_Workshop', workshop, dict(grow=(420, 120, 20), grow_down=(420, 60, 0))),
    ('SM_S1D3_CurrentTower', current_tower, dict(grow=(60, 60, 30), grow_down=(60, 60, 0))),
    ('SM_S1D3_DeckHouse', deck_house, dict(grow=(30, 30, 30), grow_down=(30, 30, 0))),
    ('SM_S1D3_Tripod', tripod, LOWMOVE),
    ('SM_S1D3_SampleRack', sample_rack, LOWMOVE),
    ('SM_S1D3_Lockers', lockers, STILL),
    ('SM_S1D3_CableDrum', cable_drum, STILL),
    ('SM_S1D3_Manifold', manifold, LOWMOVE),
    ('SM_S1D3_CurrentVane', current_vane, dict(grow=(120, 220, 10), grow_down=(120, 220, 0))),
    ('SM_S1D3_FloodMast', flood_mast, STILL),
    ('SM_S1D3_SignPost', sign_post, STILL),
    ('SM_S1D3_AntennaFarm', antenna_farm, dict(grow=(60, 60, 30), grow_down=(60, 60, 0))),
    ('SM_S1D3_BigEar', big_ear, dict(far=True, grow=(220, 80, 140), grow_down=(220, 80, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters (never rebuilt here)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
