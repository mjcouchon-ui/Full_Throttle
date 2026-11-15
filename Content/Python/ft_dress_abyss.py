"""FULL THROTTLE (Unreal) dressing kit, level 6: Under the Sea (the road to Nereida). A layered seafloor: reefs and
anemone beds by the road, kelp forests, a research station strung along a pipeline, a salvage field of containers,
anchors and wrecks, seamounts and a broken tanker out in the blue. Placed by Source/FullThrottle/FTDressL6.inl.
The swimming things (schools, the whale, the ray), the shafts of light and the bubbles are ft_world.build_abyss().
See ft_dress.py for how a piece is drawn (front faces -Y, base on z = 0, long axis along X).

    import ft_dress; ft_dress.run(['abyss'])
"""
import math
import random
from ft_desert import rod, tilt_box
from ft_car import CYAN, YEL, WHITE

SAND, SAND_D = '#d9c89a', '#b9a77e'
# rock and wrecks were the water's own blue in the first run and vanished into it: rock is a dusty rose now, wrecks rust
ROCK, ROCK_D, ROCK_L = '#9a6a7e', '#6a4562', '#c79aa2'
KELP, KELP_D, KELP_L = '#3f8a4a', '#2a6a3a', '#7fcf6a'
CORAL, CORAL2, MAG, LAV = '#ff7a6a', '#ffb04f', '#ff4fb8', '#b9a4ff'
HULL, HULL_D, RUST, RUST_D = '#8f6450', '#5c3f3a', '#c8683a', '#7a3f2a'
STATION, STATION_D = '#cfe3ea', '#7fa2b4'
SUB = '#ffd23f'
WOOD_D = '#6a4c36'


def _corals(m, r, cx, cy, z, n, spread, h=260):
    """Little growths: branching rods with bright tips, and now and then a round brain coral."""
    for k in range(n):
        a = r.uniform(0, 6.2832)
        x, y = cx + math.cos(a) * r.uniform(0, spread), cy + math.sin(a) * r.uniform(0, spread)
        c = r.choice([CORAL, CORAL2, MAG, LAV, '#ffd23f'])
        if r.random() < 0.3:
            m.sphere(c, (x, y, z + 30), r.uniform(40, 80), scale=(1, 1, 0.7), steps=(4, 6))
            continue
        hh = r.uniform(0.5, 1.0) * h
        for j in range(3):
            b = a + j * 2.1
            tip = (x + math.cos(b) * hh * 0.35, y + math.sin(b) * hh * 0.35, z + hh)
            rod(m, c, (x, y, z), tip, 12, steps=5)
            m.sphere(WHITE if j == 0 else c, tip, 18, steps=(3, 5), glow=0.5)


# ------------------------------------------------------------------------------------------ near the road
def reef_low(m, seed=1):
    r = random.Random(seed)
    for k in range(5):
        x, y = r.uniform(-420, 420), r.uniform(-260, 260)
        s = r.uniform(120, 230)
        m.sphere(ROCK if k % 2 else ROCK_L, (x, y, s * 0.15), s, scale=(1.3, 1.0, 0.5), steps=(4, 7), rot=(0, r.uniform(0, 180), 0))
    _corals(m, r, 0, 0, 60, 9, 380)


def anemones(m, seed=2):
    """A bed of tube anemones and two sea fans: low, bright, right by the barrier."""
    r = random.Random(seed)
    for k in range(14):
        x, y = r.uniform(-330, 330), r.uniform(-200, 200)
        h = r.uniform(60, 150)
        c = r.choice([MAG, LAV, CORAL2, CYAN])
        m.cyl(c, (x, y, -10), 16, h, steps=6, top_r=22)
        m.sphere(WHITE, (x, y, h - 6), 20, scale=(1, 1, 0.5), steps=(3, 6), glow=0.6)
    for x in (-180, 220):
        m.side(LAV if x < 0 else CORAL, [(x - 20, 0), (x + 20, 0), (x + 170, 250), (x, 320), (x - 170, 250)], -6, 6, rot=(0, r.uniform(-30, 30), 0))
    m.sphere(SAND_D, (0, 0, -30), 380, scale=(1.2, 0.8, 0.14), steps=(4, 8))


def pipeline(m):
    """44 m of seabed pipeline on saddles, a valve wheel, a flange every span, a small marker light."""
    L = 4400
    rod(m, STATION_D, (-L / 2.0, 0, 110), (L / 2.0, 0, 110), 60, steps=10)
    for k in range(6):
        x = -L / 2.0 + 200 + k * 800
        m.box(HULL_D, (x, 0, 45), (110, 190, 90))
        rod(m, SUB, (x + 282, 0, 110), (x + 318, 0, 110), 74, steps=10)
    m.cyl(HULL, (0, -60, 110), 26, 150, steps=6)
    m.cyl(MAG, (0, -60, 262), 70, 14, steps=10)
    m.box(MAG, (0, -60, 262), (150, 14, 14))
    m.cyl(HULL_D, (1400, 80, 0), 8, 300, steps=6)
    m.sphere(CYAN, (1400, 80, 316), 22, steps=(4, 6), glow=1.6)


def marker_post(m):
    """A survey marker: a post with a lit ring and a number plate, a cable drum beside it."""
    m.cyl(STATION_D, (0, 0, -30), 12, 420, steps=6)
    m.cyl(CYAN, (0, 0, 330), 46, 16, steps=10, glow=1.4)
    m.box(SUB, (0, -16, 240), (110, 8, 70))
    m.box(HULL_D, (0, -21, 240), (60, 4, 34))
    rod(m, WOOD_D, (150, -5, 60), (150, 85, 60), 70, steps=10)
    rod(m, HULL, (150, -15, 60), (150, -5, 60), 86, steps=10)
    rod(m, HULL, (150, 85, 60), (150, 95, 60), 86, steps=10)


def crates(m, seed=3):
    """Lost cargo half in the sand: crates, drums, a net over them."""
    r = random.Random(seed)
    for k in range(6):
        x, y = r.uniform(-260, 260), r.uniform(-180, 180)
        if k % 3 == 2:
            m.cyl(RUST if k % 2 else HULL, (x, y, 10), 46, 120, rot=(r.uniform(60, 100), r.uniform(0, 180), 0), steps=8)
        else:
            s = r.uniform(110, 170)
            m.box(WOOD_D if k % 2 else RUST_D, (x, y, s * 0.3), (s, s, s), rot=(r.uniform(-18, 18), r.uniform(0, 90), r.uniform(-18, 18)))
    m.sphere(KELP_D, (0, 0, 60), 300, scale=(1.1, 0.8, 0.3), steps=(3, 6))
    m.sphere(SAND_D, (0, 0, -40), 420, scale=(1.2, 0.9, 0.16), steps=(4, 8))


# ------------------------------------------------------------------------------------------ the middle distance
def kelp_forest(m, seed=4, n=14, h=2000, w=1400, d=900):
    """A stand of tall kelp: ribbons leaning with the current, floats along them, a few fronds near the top."""
    r = random.Random(seed)
    for s in range(n):
        x0, y0 = r.uniform(-w / 2.0, w / 2.0), r.uniform(-d / 2.0, d / 2.0)
        hh = h * r.uniform(0.55, 1.0)
        p = (x0, y0, -40)
        seg = 8
        for k in range(1, seg + 1):
            lean = (k / float(seg)) ** 1.5
            q = (x0 + 260 * lean + 60 * math.sin(k * 0.9 + s), y0 + 40 * math.cos(k * 0.7 + s), hh * k / seg)
            tilt_box(m, KELP if (k + s) % 2 else KELP_D, p, q, 46, 14)
            if k % 3 == 0:
                m.sphere('#c9b44a', q, 20, steps=(3, 5))
            if k > seg - 3:
                tilt_box(m, KELP_L, q, (q[0] + 150, q[1] + 60 * (1 if s % 2 else -1), q[2] + 60), 30, 8)
            p = q
    m.sphere(ROCK_D, (0, 0, -30), w * 0.55, scale=(1.0, d / float(w), 0.12), steps=(4, 8))


def reef_shelf(m, seed=5):
    """A terraced reef: three stepped shelves of rock, table corals on their lips, growths all over."""
    r = random.Random(seed)
    for k, (w, d, h, z) in enumerate(((3000, 1300, 320, 0), (2300, 1000, 300, 320), (1400, 700, 280, 620))):
        x = r.uniform(-200, 200)
        m.sphere(ROCK if k % 2 == 0 else ROCK_L, (x, 100 * k, z + h * 0.3), w / 2.0, scale=(1.0, d / float(w), h * 1.5 / w), steps=(5, 9))
        for j in range(4):
            tx = x + r.uniform(-w * 0.4, w * 0.4)
            m.cyl(r.choice([CORAL2, LAV, MAG]), (tx, -d * 0.36 + 100 * k, z + h * 0.55), r.uniform(120, 220), 22, steps=8)
            m.cyl(ROCK_D, (tx, -d * 0.36 + 100 * k, z + h * 0.2), 26, h * 0.4, steps=5)
        _corals(m, r, x, 100 * k, z + h * 0.75, 5, w * 0.3, h=300)
    for x, c, yaw in ((-260, MAG, 25), (240, CORAL, -30)):      # two sea fans on the top shelf (one big pale one read as a kite)
        m.side(c, [(x - 24, 880), (x + 24, 880), (x + 200, 1180), (x + 150, 1290), (x, 1340), (x - 150, 1290), (x - 200, 1180)], 180, 192, rot=(0, yaw, 0))


def reef_arch(m, seed=6):
    """A rock arch the current has cut, 18 m across, growths along its top."""
    r = random.Random(seed)
    for s in (-1, 1):
        m.sphere(ROCK, (s * 900, 0, 380), 420, scale=(0.8, 1.0, 1.5), steps=(5, 8))
        m.sphere(ROCK_L, (s * 760, 60, 900), 330, scale=(1.0, 0.9, 1.1), steps=(5, 8))
    m.sphere(ROCK, (0, 0, 1180), 760, scale=(1.5, 0.6, 0.32), steps=(5, 9))
    m.sphere(ROCK_D, (0, 40, 1060), 520, scale=(1.4, 0.5, 0.22), steps=(4, 8))
    _corals(m, r, 0, 0, 1380, 7, 700, h=280)
    for k in range(4):
        x = -500 + k * 330
        tilt_box(m, KELP if k % 2 else KELP_D, (x, 0, 1050), (x + 60, 30, 1050 - r.uniform(300, 600)), 36, 10)


def station(m):
    """A research station module: a hull on legs with lit portholes, an airlock dome, a docking collar, an aerial."""
    L, R = 2400, 430
    rod(m, STATION, (-L / 2.0, 0, 560), (L / 2.0, 0, 560), R, steps=14)
    for s in (-1, 1):
        m.sphere(STATION, (s * L / 2.0, 0, 560), R, scale=(0.5, 1, 1), steps=(6, 12))
        rod(m, MAG, (s * (L / 2.0 - 160) - 20, 0, 560), (s * (L / 2.0 - 160) + 20, 0, 560), R + 14, steps=14, glow=0.9)
    for k in range(5):
        x = -L / 2.0 + 300 + k * 450
        rod(m, STATION_D, (x, -R + 30, 560), (x, -R - 30, 560), 96, steps=10)
        rod(m, '#fff1c0' if k % 2 else CYAN, (x, -R - 30, 560), (x, -R - 42, 560), 70, steps=10, glow=1.3)
    for x in (-800, 0, 800):
        for y in (-260, 260):
            rod(m, STATION_D, (x, y * 0.6, 300), (x, y, -40), 30, steps=6)
        m.box(STATION_D, (x, 0, -10), (160, 640, 40))
    m.sphere(CYAN, (500, 0, 960), 300, scale=(1, 1, 0.8), steps=(6, 10), glow=0.3)
    m.cyl(STATION_D, (500, 0, 900), 320, 60, steps=12)
    m.cyl(STATION_D, (-700, 0, 980), 150, 200, steps=10)
    m.cyl(SUB, (-700, 0, 1180), 170, 30, steps=10)
    m.cyl(STATION_D, (-200, 100, 980), 10, 800, steps=6)
    m.sphere(MAG, (-200, 100, 1800), 28, steps=(4, 6), glow=1.8)


def station_hub(m):
    """The station's tower: stacked drums ringed with light, a dish, a beacon. A mark for the whole district."""
    z = -40
    for k, (r, h, c) in enumerate(((520, 700, STATION), (400, 600, STATION_D), (460, 500, STATION), (300, 520, STATION_D), (200, 420, STATION))):
        m.cyl(c, (0, 0, z), r, h, steps=14)
        m.cyl(CYAN if k % 2 == 0 else MAG, (0, 0, z + h - 40), r + 16, 34, steps=14, glow=1.2)
        if k < 3:
            for j in range(6):
                a = j * 1.0472
                m.box('#fff1c0', (math.cos(a) * (r + 2), math.sin(a) * (r + 2), z + h * 0.45), (10, 110, 90), rot=(0, math.degrees(a), 0), glow=0.9)
        z += h
    m.cyl(STATION_D, (0, 0, z), 14, 600, steps=6)
    m.sphere(MAG, (0, 0, z + 620), 40, steps=(4, 6), glow=2.2)
    m.cyl(STATION, (260, 0, z - 120), 240, 40, rot=(60, 0, 0), steps=12, top_r=20)
    for a in (0.6, 2.7, 4.6):
        rod(m, STATION_D, (math.cos(a) * 480, math.sin(a) * 480, 500), (math.cos(a) * 900, math.sin(a) * 900, -40), 34, steps=6)


def submarine(m):
    """A yellow research submarine parked on a cradle, its lamps on."""
    rod(m, SUB, (-700, 0, 330), (700, 0, 330), 220, steps=12)
    m.sphere(SUB, (700, 0, 330), 220, scale=(0.9, 1, 1), steps=(6, 12))
    m.sphere(SUB, (-700, 0, 330), 220, scale=(1.4, 1, 1), steps=(6, 12))
    m.sphere(CYAN, (780, 0, 350), 150, steps=(5, 9), glow=0.5)
    m.box(SUB, (-100, 0, 620), (420, 200, 280))
    m.cyl(HULL_D, (-60, 0, 760), 12, 300, steps=6)
    m.box(HULL_D, (-1050, 0, 330), (30, 520, 30))
    m.box(HULL_D, (-1050, 0, 330), (30, 30, 420))
    rod(m, HULL_D, (-1120, 0, 330), (-1090, 0, 330), 120, steps=10)
    for s in (-1, 1):
        m.sphere(WHITE, (640, s * 190, 240), 44, steps=(4, 6), glow=2.0)
        m.box(HULL, (0, s * 250, 240), (500, 50, 30))
    for x in (-450, 350):
        m.box(HULL_D, (x, 0, 50), (120, 520, 100))
        m.box(HULL_D, (x, 0, 110), (120, 260, 40))
    m.box(MAG, (0, -222, 400), (700, 6, 40), glow=0.8)


def salvage_rig(m):
    """A salvage frame over a find: four legs, a gantry, a hoist with a cargo net coming up, floodlights."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            rod(m, RUST, (sx * 800, sy * 500, -40), (sx * 650, sy * 400, 1500), 40, steps=6)
    m.box(RUST_D, (0, 0, 1520), (1500, 1000, 60))
    m.box(RUST, (0, 0, 1580), (2200, 120, 120))
    m.box(SUB, (300, 0, 1500), (260, 200, 160))
    rod(m, HULL_D, (300, 0, 1420), (300, 0, 700), 8, steps=5)
    m.sphere(KELP_D, (300, 0, 560), 190, scale=(1, 1, 0.8), steps=(4, 7))
    m.box(RUST_D, (300, 0, 520), (180, 180, 150), rot=(10, 30, 8))
    for sx in (-1, 1):
        m.box(HULL_D, (sx * 720, -480, 1430), (120, 90, 90))
        m.sphere(WHITE, (sx * 720, -530, 1420), 50, steps=(4, 6), glow=2.2)
    m.box(SUB, (0, -506, 1520), (700, 8, 50))
    rod(m, HULL_D, (-1100, 0, 1580), (-1500, 0, 3400), 10, steps=5)     # a line going up to the surface


def containers(m, seed=7):
    """Shipping containers where they fell: a stack gone over, doors open, weed on them."""
    r = random.Random(seed)
    cols = [RUST, '#3f8a9a', SUB, RUST_D, '#8a5a9a']
    for k, (x, y, z, yaw, pitch) in enumerate(((0, 0, 130, 10, 0), (80, 30, 390, -8, 4), (-700, 250, 130, 70, 0), (500, -350, 200, 30, 24), (-250, -480, 110, -40, -6))):
        m.box(cols[k], (x, y, z), (610, 244, 260), rot=(pitch, yaw, 0))
        m.box(HULL_D, (x, y, z + 132), (600, 234, 8), rot=(pitch, yaw, 0))
    m.sphere(KELP_D, (60, 20, 540), 200, scale=(1.6, 0.9, 0.3), steps=(3, 6))
    m.sphere(SAND_D, (0, 0, -50), 900, scale=(1.2, 0.9, 0.13), steps=(4, 8))
    _corals(m, r, -300, 0, 20, 4, 500, h=200)


def anchor(m):
    """A ship's anchor twice a man's height, its chain running off into the sand."""
    m.box(HULL_D, (0, 0, 560), (90, 70, 1000), rot=(0, 0, 18))
    m.box(HULL_D, (-150, 0, 1030), (520, 70, 80), rot=(0, 0, 18))
    rod(m, HULL, (-170, -15, 1120), (-170, 15, 1120), 110, steps=10)
    for s in (-1, 1):
        tilt_box(m, HULL_D, (160, 0, 110), (160 + s * 380, 0, 420), 110, 70)
        m.side(HULL_D, [(160 + s * 380 - 80, 420), (160 + s * 380 + 80, 420), (160 + s * 470, 560)], -36, 36)
    for k in range(9):
        m.cyl(RUST_D, (-260 - k * 130, 60 + 30 * math.sin(k), 20 + (30 if k % 2 else 0)), 50, 26, rot=(90 if k % 2 else 0, 20, 0), steps=8)


def wreck_bow(m):
    """The bow of a freighter standing out of the sand at a slant, thirty metres of it, holed and weeded."""
    pts = [(-1500, 0), (1300, 0), (2300, 2900), (1700, 3100), (-1200, 1500)]
    m.side(HULL, pts, -520, 520, rot=(0, 0, 0))
    m.side(RUST, [(-1300, 0), (1200, 0), (1650, 1000), (-1250, 760)], -530, 530)
    m.side(HULL_D, [(1500, 2500), (2260, 2880), (1720, 3080), (1300, 2760)], -536, 536)
    m.box(HULL_D, (300, 0, 1900), (700, 1080, 60), rot=(52, 0, 0))
    for k in range(3):
        rod(m, '#0a1a2a', (-400 + k * 500, -528, 1100 + k * 330), (-400 + k * 500, -542, 1100 + k * 330), 90, steps=10)
    m.cyl(HULL_D, (2050, 0, 2950), 16, 700, rot=(35, 0, 0), steps=6)
    for k in range(5):
        x = -900 + k * 420
        tilt_box(m, KELP if k % 2 else KELP_D, (x, -540, 700 + k * 260), (x + 90, -600, 300 + k * 200), 44, 10)
    m.sphere(SAND_D, (0, 0, -60), 2100, scale=(1.2, 0.7, 0.1), steps=(4, 8))


def plane_tail(m):
    """A lost aeroplane: the tail and a length of fuselage, a wing lying by it."""
    rod(m, STATION, (-900, 0, 260), (790, 0, 440), 260, steps=12)
    m.sphere(STATION, (800, 0, 430), 260, scale=(1.5, 1, 1), steps=(5, 10))
    m.side(MAG, [(-1050, 420), (-500, 480), (-760, 1500), (-1150, 1450)], -30, 30)
    m.box(STATION, (-900, 0, 520), (360, 1300, 36))
    m.box(STATION_D, (300, -900, 60), (700, 1900, 50), rot=(4, 24, 6))
    for k in range(5):
        rod(m, '#0a1a2a', (-300 + k * 240, -258, 330 + k * 18), (-300 + k * 240, -270, 330 + k * 18), 44, steps=8)
    m.sphere(SAND_D, (0, 0, -60), 1500, scale=(1.3, 1.0, 0.1), steps=(4, 8))


# ------------------------------------------------------------------------------------------ the horizon (built from z = 0 up)
def sea_mount(m, seed=8, h=13000):
    """A pinnacle of rock standing out of the deep: a silhouette in the blue."""
    r = random.Random(seed)
    z, rad = 0, h * 0.42
    for k in range(6):
        hh = h / 6.0 * r.uniform(0.8, 1.25)
        nr = rad * r.uniform(0.6, 0.8)
        m.cyl(ROCK_D if k % 2 else ROCK, (r.uniform(-0.1, 0.1) * rad, r.uniform(-0.1, 0.1) * rad, z), rad, hh, steps=7, top_r=nr)
        if k in (1, 3):     # a shoulder of rock, off to one side (rings round it read as stripes in the first run)
            m.sphere(ROCK_L, (rad * 0.7, r.uniform(-0.4, 0.4) * rad, z + hh * 0.6), rad * 0.55, scale=(1, 1, 0.7), steps=(4, 6))
        z += hh
        rad = nr
    m.cyl(ROCK_D, (0, 0, z), rad, h * 0.1, steps=6, top_r=rad * 0.15)


def far_wreck(m):
    """A tanker broken in two on the bottom, two hundred metres of her, leaning."""
    for s, x0, pitch in ((-1, -5200, 0), (1, 5600, 7)):
        L = 9000
        m.side(HULL_D, [(x0 - L / 2.0, 0), (x0 + L / 2.0, 0), (x0 + L / 2.0 + (900 if s > 0 else 0), 2200), (x0 - L / 2.0 - (700 if s < 0 else 0), 2200)], -1300, 1300,
               rot=(0, 0, 9 * s))
        m.side(RUST_D, [(x0 - L / 2.0, 0), (x0 + L / 2.0, 0), (x0 + L / 2.0, 800), (x0 - L / 2.0, 800)], -1320, 1320, rot=(0, 0, 9 * s))
    m.box(HULL, (-7600, 0, 3600), (1600, 2000, 2400), rot=(0, 0, -9))
    m.box(HULL_D, (-7600, 0, 5000), (900, 1200, 500), rot=(0, 0, -9))
    m.cyl(RUST_D, (-7900, 0, 5200), 260, 1500, rot=(-9, 0, 0), steps=8)
    for x in (-3000, 3600, 6400):
        m.cyl(HULL, (x, 0, 2300), 60, 2200, steps=6)
        m.box(HULL, (x, 0, 4400), (1500, 80, 80))


def far_kelp(m, seed=9):
    """A wall of giant kelp, ninety metres tall: a dark curtain far off."""
    r = random.Random(seed)
    for s in range(22):
        x0 = -6000 + s * 570 + r.uniform(-200, 200)
        hh = r.uniform(5000, 9500)
        p = (x0, r.uniform(-600, 600), 0)
        for k in range(1, 6):
            q = (x0 + 900 * (k / 5.0) ** 1.5 + 200 * math.sin(k + s), p[1] + 100 * math.cos(k * 0.8 + s), hh * k / 5.0)
            tilt_box(m, KELP_D if (k + s) % 2 else '#1f5a3a', p, q, 260, 60)
            p = q


# ------------------------------------------------------------------------------------------ the second round
# What the first run's frames asked for: colour (the rock and the wrecks came out the water's own blue: see the
# palette above), a verge that is never bare (sea grass, clams), taller and brighter coral, things in the water as
# well as on the bottom (jellyfish), a station that reads as a place (domes joined by a lit tube), smoking vents on
# the approach, Nereida's own watch towers, an older wreck, and a reef wall for the horizon.
GRASS, GRASS_L = '#3fae7a', '#8fe0a0'
VENT, VENT_D, EMBER = '#3a3140', '#241d2a', '#ff7a2f'
STEEL, STEEL_D = '#3d4763', '#262d44'


def sea_grass(m, seed=21):
    """A meadow of sea grass 12 m long, shells in it. Under 1.5 m: it may stand right behind the barrier."""
    r = random.Random(seed)
    for k in range(44):
        x, y = r.uniform(-600, 600), r.uniform(-260, 260)
        h = r.uniform(60, 140)
        m.box(GRASS if k % 3 else GRASS_L, (x, y, h / 2.0 - 10), (14, 5, h), rot=(r.uniform(-22, 22), r.uniform(0, 180), r.uniform(-22, 22)))
    for k in range(5):
        m.sphere([CORAL2, LAV, WHITE][k % 3], (r.uniform(-500, 500), r.uniform(-220, 220), 6), r.uniform(16, 30), scale=(1, 1, 0.5), steps=(3, 5))


def giant_clam(m, seed=22):
    """Two giant clams, their lips lit. Low: verge."""
    r = random.Random(seed)
    for x, s, c in ((-150, 1.0, MAG), (190, 0.7, CYAN)):
        for side in (-1, 1):
            m.sphere(SAND_D, (x, side * 26 * s, 40 * s), 120 * s, scale=(1.3, 0.34, 0.8), steps=(4, 8), rot=(0, 0, side * 24))
            m.sphere(c, (x, side * 12 * s, 96 * s), 104 * s, scale=(1.25, 0.12, 0.3), steps=(3, 8), glow=0.9)
    m.sphere(SAND_D, (0, 0, -30), 330, scale=(1.2, 0.8, 0.14), steps=(4, 8))


def coral_garden(m, seed=23):
    """A stand of coral three metres tall: fans, branching horns with lit tips, tube sponges, a table."""
    r = random.Random(seed)
    m.sphere(ROCK, (0, 0, 10), 420, scale=(1.3, 0.9, 0.3), steps=(4, 8))
    for k in range(3):                                           # sea fans
        x = -330 + k * 330
        c = [MAG, CORAL, LAV][k]
        m.side(c, [(x - 16, 60), (x + 16, 60), (x + 150, 250), (x + 120, 330), (x, 380), (x - 120, 330), (x - 150, 250)], -6, 6,
               rot=(0, r.uniform(-35, 35), 0))
        m.box(ROCK_D, (x, 0, 60), (22, 14, 120))
    for k in range(4):                                           # tube sponges
        x, y = r.uniform(-380, 380), r.uniform(60, 240)
        for j in range(3):
            h = r.uniform(90, 230)
            m.cyl([SUB, LAV, CORAL2][k % 3], (x + j * 44, y + (j % 2) * 30, 20), 20, h, steps=6, top_r=27)
    _corals(m, r, 0, -120, 60, 8, 420, h=300)
    m.cyl(ROCK_D, (300, -150, 40), 22, 150, steps=5)
    m.cyl(CORAL2, (300, -150, 190), 170, 20, steps=8)


def jellyfish(m, seed=24):
    """A swarm of seven jellyfish hanging in the water, 4 to 14 m over the sand: lit bells, trailing arms."""
    r = random.Random(seed)
    for k in range(7):
        x, y, z = r.uniform(-700, 700), r.uniform(-400, 400), r.uniform(400, 1400)
        s = r.uniform(0.7, 1.5)
        c = [MAG, CYAN, LAV][k % 3]
        m.sphere(c, (x, y, z), 70 * s, scale=(1, 1, 0.7), steps=(4, 8), glow=1.1)
        m.cyl(WHITE, (x, y, z - 30 * s), 62 * s, 12 * s, steps=8, glow=0.6)
        for j in range(4):
            a = j * 1.571 + k
            m.box(c, (x + math.cos(a) * 30 * s, y + math.sin(a) * 30 * s, z - 150 * s), (8 * s, 8 * s, 240 * s),
                  rot=(r.uniform(-8, 8), 0, r.uniform(-8, 8)), glow=0.5)


def vent_field(m, seed=25):
    """Black smokers: five chimneys of dark rock, their cracks glowing, smoke standing over the tallest in flat puffs."""
    r = random.Random(seed)
    m.sphere(VENT_D, (0, 0, -20), 700, scale=(1.2, 0.9, 0.2), steps=(4, 8))
    tops = []
    for k in range(5):
        x, y = r.uniform(-480, 480), r.uniform(-300, 300)
        h = r.uniform(300, 900)
        m.cyl(VENT if k % 2 else VENT_D, (x, y, -20), r.uniform(90, 150), h, steps=6, top_r=r.uniform(36, 60))
        m.cyl(EMBER, (x, y, h - 30), 44, 26, steps=6, glow=1.6)
        for j in range(2):
            m.box(EMBER, (x + r.uniform(-50, 50), y - 70, h * r.uniform(0.25, 0.7)), (10, 60, r.uniform(60, 150)), glow=1.2)
        tops.append((h, x, y))
    tops.sort(reverse=True)
    for h, x, y in tops[:2]:
        for j in range(4):
            m.sphere(STEEL_D if j % 2 else VENT, (x + j * 50, y + 20 * j, h + 120 + j * 190), 90 + j * 46, scale=(1, 1, 0.8), steps=(4, 6))


def dome_habitat(m):
    """A habitat: a glass dome 14 m across on a ringed base, a lit room inside, an airlock tube, an aerial."""
    m.cyl(STATION_D, (0, 0, -40), 760, 160, steps=12)
    m.cyl(SUB, (0, 0, 120), 772, 26, steps=12)
    m.sphere(CYAN, (0, 0, 130), 700, scale=(1, 1, 0.82), steps=(6, 12), glow=0.25)
    m.box('#fff1c0', (0, 0, 260), (520, 420, 240), glow=0.7)
    m.box(STATION_D, (0, 0, 392), (560, 460, 24))
    rod(m, STATION, (0, -640, 120), (0, -1050, 120), 130, steps=10)
    rod(m, MAG, (0, -1050, 120), (0, -1070, 120), 144, steps=10, glow=0.9)
    m.cyl(STATION_D, (300, 300, 600), 9, 520, steps=5)
    m.sphere(MAG, (300, 300, 1130), 26, steps=(4, 6), glow=1.8)
    for a in (0.5, 2.6, 4.7):
        m.box(STATION_D, (math.cos(a) * 800, math.sin(a) * 800, 20), (120, 120, 160))


def tube_run(m):
    """47 m of travel tube on stilts, lit along its length, a capsule in it: one to a slot makes a line from module
    to module."""
    L = 4700
    rod(m, CYAN, (-L / 2.0, 0, 520), (L / 2.0, 0, 520), 120, steps=10, glow=0.35)
    for k in range(5):
        x = -L / 2.0 + 470 + k * 940
        rod(m, STATION, (x - 30, 0, 520), (x + 30, 0, 520), 138, steps=10)
        m.box(STATION_D, (x, 0, 190), (60, 60, 460))
        m.box(STATION_D, (x, 0, -20), (200, 200, 50))
    rod(m, SUB, (-500, 0, 520), (-180, 0, 520), 84, steps=8, glow=0.4)
    m.box(MAG, (0, -124, 520), (L, 6, 14), glow=1.0)


def watch_tower(m):
    """One of Nereida's watch towers: a dark steel spire 40 m tall, finned, ringed with magenta light, a red lamp
    turning at the top (the lamp is still: the living-world pass may turn it)."""
    z = -40
    for k, (r0, r1, h) in enumerate(((330, 260, 900), (260, 210, 900), (210, 170, 900), (170, 110, 800))):
        m.cyl(STEEL if k % 2 == 0 else STEEL_D, (0, 0, z), r0, h, steps=8, top_r=r1)
        m.cyl(MAG, (0, 0, z + h - 30), r1 + 14, 30, steps=8, glow=1.3)
        z += h
    for a in (0.0, 2.094, 4.189):
        m.side(STEEL_D, [(330, -40), (700, -40), (330, 900)], -16, 16, rot=(0, math.degrees(a), 0))
    m.cyl(STEEL_D, (0, 0, z), 150, 220, steps=8)
    m.cyl('#ff3355', (0, 0, z + 60), 158, 90, steps=8, glow=1.8)
    m.cyl(STEEL_D, (0, 0, z + 220), 10, 500, steps=5)
    m.box(STEEL_D, (0, -340, 420), (160, 60, 220))
    m.box('#ff3355', (0, -372, 420), (110, 6, 150), glow=0.9)


def galleon(m):
    """An older wreck: the hull of a sailing ship on its side in the sand, ribs showing, two masts leaning with rags
    of sail."""
    m.side(WOOD_D, [(-1500, 0), (1300, 0), (1700, 620), (1300, 760), (-1300, 760), (-1650, 1000), (-1750, 520)], -330, 330, rot=(0, 0, 12))
    m.side(RUST_D, [(-1300, 760), (1300, 760), (1200, 830), (-1250, 830)], -350, 350, rot=(0, 0, 12))
    for k in range(7):
        x = -1100 + k * 350
        m.box(WOOD_D, (x, -360, 420), (50, 50, 760), rot=(0, 0, 30))
    for x, h, lean in ((-500, 2300, 14), (600, 2700, -9)):
        rod(m, WOOD_D, (x, 0, 700), (x + lean * 30, 120, 700 + h), 46, steps=6)
        rod(m, WOOD_D, (x + lean * 20 - 600, 110, 700 + h * 0.7), (x + lean * 20 + 600, 110, 700 + h * 0.7), 26, steps=5)
        m.side(LAV, [(x + lean * 20 - 520, 700 + h * 0.68), (x + lean * 20 + 480, 700 + h * 0.68), (x + lean * 14 + 360, 700 + h * 0.3),
                     (x + lean * 14 - 150, 700 + h * 0.42), (x + lean * 14 - 420, 700 + h * 0.28)], 96, 104)
    for k in range(4):
        tilt_box(m, KELP if k % 2 else KELP_D, (-900 + k * 560, -350, 700), (-860 + k * 560, -420, 250), 40, 10)
    m.sphere(SAND_D, (0, 0, -60), 1900, scale=(1.2, 0.6, 0.1), steps=(4, 8))


def far_reef(m, seed=26):
    """A reef wall for the horizon, 160 m long and 45 m high: stacked terraces of rock, coral trees along its crest."""
    r = random.Random(seed)
    for k in range(9):
        x = -7200 + k * 1800 + r.uniform(-300, 300)
        h = r.uniform(1800, 4200)
        m.sphere(ROCK if k % 2 else ROCK_D, (x, r.uniform(-400, 400), h * 0.35), 1500, scale=(1.0, 0.7, h / 1500.0 * 0.8), steps=(4, 7))
        if k % 2 == 0:
            c = [MAG, CORAL2, LAV][(k // 2) % 3]
            m.cyl(ROCK_D, (x, 0, h * 0.9), 90, 700, steps=5)
            m.sphere(c, (x, 0, h * 0.9 + 900), 520, scale=(1.2, 0.9, 0.55), steps=(4, 7), glow=0.3)
    m.sphere(ROCK_D, (0, 0, 0), 8200, scale=(1.0, 0.16, 0.14), steps=(4, 8))


JOBS = [
    ('SM_ReefLow', reef_low), ('SM_ReefLow2', lambda m: reef_low(m, seed=12)), ('SM_Anemones', anemones), ('SM_Pipeline', pipeline),
    ('SM_MarkerPost', marker_post), ('SM_Crates', crates),
    ('SM_KelpForest', kelp_forest), ('SM_KelpForest2', lambda m: kelp_forest(m, seed=14, n=20, h=2600, w=2000, d=1200)),
    ('SM_ReefShelf', reef_shelf), ('SM_ReefShelf2', lambda m: reef_shelf(m, seed=15)), ('SM_ReefArch', reef_arch),
    ('SM_Station', station), ('SM_StationHub', station_hub), ('SM_Submarine', submarine),
    ('SM_SalvageRig', salvage_rig), ('SM_Containers', containers), ('SM_Anchor', anchor), ('SM_WreckBow', wreck_bow), ('SM_PlaneTail', plane_tail),
    ('SM_SeaMount', sea_mount), ('SM_SeaMount2', lambda m: sea_mount(m, seed=18, h=9000)), ('SM_FarWreck', far_wreck), ('SM_FarKelp', far_kelp),
    ('SM_SeaGrass', sea_grass), ('SM_SeaGrass2', lambda m: sea_grass(m, seed=31)), ('SM_GiantClam', giant_clam),
    ('SM_CoralGarden', coral_garden), ('SM_CoralGarden2', lambda m: coral_garden(m, seed=33)), ('SM_Jellyfish', jellyfish),
    ('SM_Jellyfish2', lambda m: jellyfish(m, seed=34)), ('SM_VentField', vent_field), ('SM_DomeHabitat', dome_habitat),
    ('SM_TubeRun', tube_run), ('SM_WatchTower', watch_tower), ('SM_Galleon', galleon), ('SM_FarReef', far_reef),
]
