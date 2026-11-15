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


# ------------------------------------------------------------------------------------------ the third round (art pass 2)
# The seabed itself. From a glass tunnel's roof, a loop or a banked bend the camera looks down on the sand, and the
# sand between the reefs was bare. So: flat pieces, all under 1.2 m but the whale's ribs, which may lie right up to the
# verge: reef pavement (plates of rock, brain and plate corals, starfish), a service cable in a trench running out
# from the road, a landing apron for the station's submarines, a whale's bones.
# The track sinks a dressed piece 15 cm: the sand's surface is at z = LAND in a piece's own space.
LAND = 15.0
BONE = '#f4efe0'


def _ccw(pts):
    """The polygon counter-clockwise, whichever way it was written (an extrusion written clockwise is inside out)."""
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def _oval(rx, ry, n=10, cx=0.0, cy=0.0, a0=0.0):
    return [(cx + math.cos(a0 + 2 * math.pi * k / n) * rx, cy + math.sin(a0 + 2 * math.pi * k / n) * ry) for k in range(n)]


def _star(r0, r1, cx, cy, a0=0.0):
    return [(cx + math.cos(a0 + math.pi * k / 5.0) * (r0 if k % 2 == 0 else r1), cy + math.sin(a0 + math.pi * k / 5.0) * (r0 if k % 2 == 0 else r1))
            for k in range(10)]


def reef_floor(m, seed=41):
    """A patch of reef pavement 16 x 10 m, nothing in it over a metre: flat plates of rock, brain corals, plate corals
    on short stems, giant starfish, a few tube sponges. For the sand between the reefs: it is seen from above."""
    r = random.Random(seed)
    for k in range(5):
        x, y = r.uniform(-560, 560), r.uniform(-320, 320)
        m.plan(ROCK_L if k % 2 else ROCK, _oval(r.uniform(170, 300), r.uniform(120, 220), 8, x, y, r.uniform(0, 1)), -40, LAND + r.uniform(18, 40))
    for k in range(3):
        x, y = r.uniform(-600, 600), r.uniform(-360, 360)
        s = r.uniform(60, 100)
        m.sphere([CORAL2, LAV, CORAL][k], (x, y, LAND + s * 0.2), s, scale=(1.2, 1.0, 0.6), steps=(4, 7))
    for k in range(4):
        x, y = r.uniform(-640, 640), r.uniform(-380, 380)
        m.cyl(ROCK_D, (x, y, LAND - 10), 16, 60, steps=5)
        m.cyl([MAG, CORAL, CORAL2, LAV][k], (x, y, LAND + 46), r.uniform(70, 120), 14, steps=8)
    for k in range(3):
        x, y = r.uniform(-700, 700), r.uniform(-420, 420)
        m.plan([CORAL, MAG, CORAL2][k], _ccw(_star(110, 44, x, y, r.uniform(0, 1))), LAND - 4, LAND + 14)
    for k in range(3):
        x, y = r.uniform(-500, 500), r.uniform(-300, 300)
        m.cyl(SUB, (x, y, LAND - 10), 14, r.uniform(50, 90), steps=6, top_r=19)


def cable_trench(m, seed=43):
    """A service cable in a trench, 60 m of it, running out from the road (along +Y): the sand thrown up in a bank on
    each side, the cable on yellow saddles between, a junction box with a ring of light at the far end. Under a
    metre high, so it may start at the verge."""
    r = random.Random(seed)
    L = 6000.0
    for s in (-1, 1):
        for k in range(6):
            y = -L / 2.0 + 500 + k * 1000
            m.sphere(SAND_D, (s * 150 + r.uniform(-15, 15), y, LAND - 10), 520, scale=(0.22, 1.05, 0.11), steps=(3, 6))
    rod(m, STATION_D, (0, -L / 2.0, LAND + 14), (0, L / 2.0 - 250, LAND + 14), 22, steps=6)
    for k in range(6):
        m.box(SUB, (0, -L / 2.0 + 400 + k * 1000, LAND + 8), (90, 60, 34))
    m.box(STATION, (0, L / 2.0 - 150, LAND + 40), (220, 220, 80))
    m.cyl(STATION_D, (0, L / 2.0 - 150, LAND + 80), 60, 26, steps=8)
    m.cyl(CYAN, (0, L / 2.0 - 150, LAND + 106), 46, 12, steps=8, glow=1.2)


def sub_pad(m):
    """A landing apron for the station's submarines, 22 m across: an eight-sided plate, a painted ring and cross,
    short marker posts with small lamps round it, cargo waiting at the back. Flat: the posts are a metre."""
    R, top = 1100.0, LAND + 14.0
    m.cyl(STATION_D, (0, 0, -40), R, top + 40, steps=8)
    m.cyl(SUB, (0, 0, top), R - 170, 2, steps=16)
    m.cyl(STATION_D, (0, 0, top), R - 290, 3, steps=16)
    m.box(MAG, (0, 0, top + 4), (1100, 150, 2))
    m.box(MAG, (0, 0, top + 4), (150, 1100, 2))
    for k in range(8):
        a = k * math.pi / 4
        x, y = math.cos(a) * (R - 135), math.sin(a) * (R - 135)
        m.cyl(STATION, (x, y, top), 22, 96, steps=6)
        m.sphere(CYAN, (x, y, top + 104), 20, steps=(3, 5), glow=1.0)
    m.box(RUST, (-480, 700, top + 65), (300, 200, 130))
    m.box(SUB, (-170, 730, top + 50), (160, 160, 100))
    m.cyl(HULL, (260, 720, top), 60, 120, steps=8)


def whale_bones(m, seed=44):
    """A whale's skeleton on the sand, 20 m of it: the skull with its jaws apart, the spine, eight pairs of ribs
    standing like an arcade (2.6 m at the tallest), the last bones of the tail."""
    r = random.Random(seed)
    m.sphere(SAND_D, (100, 0, -20), 1100, scale=(1.1, 0.45, 0.1), steps=(4, 8))
    rod(m, BONE, (-800, 0, LAND + 60), (540, 0, LAND + 90), 34, steps=6)
    for k in range(8):
        x = -420 + k * 125
        h = 260.0 * math.sin(math.pi * (k + 1.2) / 9.5)
        for s in (-1, 1):
            top = (x + 10, s * (90 + h * 0.5), LAND + 60 + h)
            tilt_box(m, BONE, (x, 0, LAND + 80), top, 30, 22)
            tilt_box(m, BONE, top, (x + 16, s * (140 + h * 0.35), LAND - 6), 26, 20)
    m.side(BONE, [(520, LAND + 10), (980, LAND + 30), (1230, LAND + 40), (980, LAND + 150), (560, LAND + 190)], -110, 110)
    for y0, y1 in ((-150, -118), (118, 150)):
        m.side(BONE, [(600, LAND - 4), (1210, LAND + 4), (1190, LAND + 26), (600, LAND + 22)], y0, y1)
    for k in range(5):
        m.sphere(BONE, (-860 - k * 110, r.uniform(-20, 20), LAND + 26), 40 - k * 4, scale=(1.3, 1, 0.8), steps=(3, 5))


def life():
    """What moves (art pass 2, and the zone pass). ft_life.animate() puts M_Life on a piece's slots and saves it in
    place; an export puts the finish pass's materials back, so run this again after every export of a piece named here:
        import ft_dress, ft_dress_abyss; ft_dress.run(['abyss']); ft_dress_abyss.life_props(); ft_dress_abyss.life()
    The water moves everything slowly: weed and grass sway, jellyfish rise and sink, lamps and rings pulse. Nothing on
    the verge blinks (a light that comes and goes beside the road reads as a pickup)."""
    import ft_life
    A = ft_life.animate
    D = '/Game/Maps/Abyss/Dress/'
    A(D + 'SM_KelpForest', 'Sway', SwayAmp=85.0, SwayHeight=2000.0, SwayPow=1.5, SwayRate=0.5, SwayLean=0.3, Wind=(1.0, 0.3, 0.0))
    A(D + 'SM_KelpForest2', 'Sway', SwayAmp=110.0, SwayHeight=2600.0, SwayPow=1.5, SwayRate=0.45, SwayLean=0.3, Wind=(1.0, 0.3, 0.0))
    for name in ('SM_SeaGrass', 'SM_SeaGrass2'):
        A(D + name, 'Sway', SwayAmp=12.0, SwayHeight=140.0, SwayPow=1.4, SwayRate=0.8, SwayLean=0.2, Wind=(1.0, 0.3, 0.0))
    for name in ('SM_Jellyfish', 'SM_Jellyfish2'):          # each bell on its own beat: the phase runs across the swarm
        A(D + name, 'Bob', BobAmp=55.0, BobRate=0.11, BobRand=1.0, BobPhase=(0.0009, 0.0006, 0.0), BobDrift=12.0)
    A(D + 'SM_StationHub', 'Light', [CYAN + '*', MAG + '*'], **ft_life._chase((0.0, 0.0, 1.0), 1300.0, rate=0.35, low=0.35, rand=1.0))
    A(D + 'SM_TubeRun', 'Light', [MAG + '*'], **ft_life._chase((1.0, 0.0, 0.0), 940.0, rate=0.5, low=0.4))
    A(D + 'SM_WatchTower', 'Light', ['#ff3355*'], **dict(ft_life.BLINK, LightRate=0.45, LightDuty=0.6))
    A(D + 'SM_WatchTower', 'Light', [MAG + '*'], **ft_life.PULSE)
    A(D + 'SM_VentField', 'Light', [EMBER + '*'], LightRate=2.4, LightDuty=0.7, LightSoft=0.45, LightMin=0.5, LightRand=1.0)
    A(D + 'SM_DomeHabitat', 'Light', ['#fff1c0*'], **dict(ft_life.PULSE, LightRate=0.2, LightMin=0.7))
    A(D + 'SM_SubPad', 'Light', [CYAN + '*'], **dict(ft_life.PULSE, LightRate=0.4, LightMin=0.35))
    # ---- the zone pass (the fourth round). What hangs in the water moves with it, slowly: the buoyancy spheres sway on
    # their chains, the weed raft wanders and its fronds swing, the far whales heave. The net of light on the sand swims
    # (every part of it wanders by where it is in the world) while soft bands of light cross it. Nothing here blinks.
    A(D + 'SM_Tethered', 'Sway', SwayAmp=75.0, SwayHeight=2200.0, SwayPow=1.3, SwayRate=0.35, SwayLean=0.25, Wind=(1.0, 0.3, 0.0))
    A(D + 'SM_KelpRaft', ('Bob', 'Sway'), BobAmp=45.0, BobRate=0.06, BobRand=1.0, BobDrift=90.0, SwayBase=RAFT_Z, SwayHeight=-700.0, SwayAmp=70.0,
      SwayPow=1.2, SwayRate=0.5, SwayLean=0.3, Wind=(1.0, 0.3, 0.0))
    A(D + 'SM_WhalePod', ('Bob', 'Spin'), BobAmp=260.0, BobRate=0.03, BobRand=1.0, BobPhase=(0.0001, 0.0, 0.0), SpinPivot=(0.0, 0.0, 4300.0),
      SpinAxis=(0.0, 1.0, 0.0), SpinSwing=2.5, SpinRate=0.22, SpinRand=1.0)
    A(D + 'SM_CausticNet', ('Sway', 'Light'), SwayBase=-73.0, SwayHeight=100.0, SwayPow=1.0, SwayAmp=30.0, SwayRate=1.1, SwayGust=5.0, SwayLean=0.0,
      Wind=(1.0, 0.3, 0.0), **dict(ft_life._chase((0.7, 0.7, 0.0), 1100.0, rate=0.22, duty=0.5, low=0.3), LightSoft=0.45, LightRand=1.0))


# ------------------------------------------------------------------------------------------ the cut banks (3.4)
# Inside a banked bend the track cuts the seabed down to the road's low edge, and that cutting was one pale wall of
# sand a third of the frame high ("the big pale wall at 20 s"). A Bank rule has the track lay these on it
# (ft_dress_bank.py has the drawing rules: flat, x along the road, y up the slope, z out of the bank). The road is cut
# through the reef's own rock: beds of the mauve reef rock and of packed sand, one over another, each its own ledge,
# coral and weed grown on the ledges of every other panel. Unlit and still: nothing here can read as a pickup.
import ft_dress_bank as B   # noqa: E402

# a deep cut's beds, foot to top: from y, to y, colour, how far the bed stands out
REEF_BEDS = ((-450.0, -290.0, ROCK, 20.0), (-290.0, -120.0, SAND_D, 10.0), (-120.0, 40.0, ROCK_L, 16.0), (40.0, 200.0, ROCK, 8.0),
             (200.0, 340.0, SAND_D, 14.0), (340.0, 450.0, SAND, 22.0))


def cut_reef(m, seed=0, growth=False):
    """A deep cut through the reef (11 x 9 m as drawn): six beds, the dark rock in the seams between them;
    growth=True: coral in three colours and weed on the ledges."""
    r = random.Random(seed + 600)
    B.body(m, ROCK_D, B.TALL)
    last = len(REEF_BEDS) - 1
    for k, (y0, y1, col, z) in enumerate(REEF_BEDS):
        B.band(m, col, y0 if k == 0 else y0 + 6.0, y1 if k == last else y1 + 14.0, z,
               wob=(0.0 if k == 0 else 22.0, 0.0 if k == last else 22.0), n=5, seed=seed * 16 + k)
    if growth:
        # Crusts of coral lying on two ledges and weed on a third: flat patches in the reef's own pink and lavender.
        # (The first try had single bright heads in four colours, a metre across: beside the road they read as
        # gems to pick up, and this level's verge may carry nothing that does.)
        for x, y, col in ((-300.0, -276.0, CORAL), (150.0, 52.0, LAV)):
            for k in range(3):
                q = r.uniform(58.0, 84.0)
                B.shape(m, col, [(x + k * 96.0 + math.cos(a) * q * r.uniform(0.8, 1.15), y + math.sin(a) * q * 0.5) for a in [6.2832 * i / 7.0 for i in range(7)]],
                        10.0 + 6.0 * (k % 2), B.FACE + 8.0)
        for k in range(3):
            B.shape(m, KELP if k % 2 else KELP_D, [(-240.0 + k * 70.0 - 28.0, -108.0), (-240.0 + k * 70.0 + 28.0, -108.0), (-240.0 + k * 70.0 + r.uniform(-14.0, 14.0), -108.0 + r.uniform(90.0, 140.0))],
                    6.0, B.FACE + 16.0)


def cut_shoal(m, seed=0):
    """A shallow cut (11 x 4 m as drawn): two beds of the reef rock, a seam of packed sand, a second ledge of rock, a
    pale cap. (Zone pass: it was rock, sand, cap, and in the real game it still read as one pale wall with a mauve
    foot. More of it is rock now, and a crust of coral lies along the upper ledge, flat and unlit.)"""
    r = random.Random(seed + 700)
    B.body(m, ROCK_D, B.LOW)
    B.band(m, ROCK, -200.0, -96.0, 22.0, wob=(0.0, 14.0), seed=seed * 16 + 1)
    B.band(m, ROCK_L, -104.0, -22.0, 12.0, wob=14.0, seed=seed * 16 + 2)
    B.band(m, SAND_D, -30.0, 62.0, 8.0, wob=14.0, seed=seed * 16 + 3)
    B.band(m, ROCK, 54.0, 128.0, 18.0, wob=14.0, seed=seed * 16 + 4)
    B.band(m, SAND, 120.0, 200.0, 12.0, wob=(12.0, 0.0), seed=seed * 16 + 5)
    for x in (-330.0, 90.0):
        for k in range(3):
            q = r.uniform(46.0, 70.0)
            B.shape(m, CORAL if x < 0 else LAV, [(x + k * 84.0 + math.cos(a) * q * r.uniform(0.8, 1.15), 92.0 + math.sin(a) * q * 0.42)
                                               for a in [6.2832 * i / 7.0 for i in range(7)]], 8.0 + 5.0 * (k % 2), B.FACE + 18.0)


# ------------------------------------------------------------------------------------------ the fourth round (the zone pass, 15 Nov 2026)
# The owner's words for this zone: "Nearby pressure structures, marine silhouettes, and suspended scenery with schools of
# fish, rising bubbles, drifting vegetation, and moving caustics."
# What the real game's frames showed: the bottom was dressed and the water over it was empty. From the chase camera the
# top half of the frame is the bright net of the surface, and nothing stood against it but shafts of light. So this
# round fills the water: silhouettes high up and far out (whales, the hulls of ships on the surface, rays in formation,
# sharks on their rounds), things that hang in it (buoyancy spheres on their chains, rafts of weed with fronds trailing),
# things that rise through it (bubbles from vents), schools that turn and stream, and pressure vessels by the road.
# This level's rule stands: nothing near the road may read as a pickup or as the boss's torpedo fish. What is new on the
# verge is big, dark and unlit; schools keep 30 m from the road and are not blue; nothing blinks.
import ft_art   # noqa: E402

DEEP1, DEEP2, DEEP3 = '#16304f', '#27507a', '#4a7fa6'      # silhouettes, nearest to farthest: paler and bluer with distance
BELLY = '#9fb8c8'
PORT = '#0a1a2a'
FISH_A, FISH_B, FISH_C = '#e6eef2', '#b9a4ff', '#ff9ec7'     # school colours: silver, lavender, pink (the boss's torpedo fish are blue)
BUBBLE = '#cfeeff'
CAUSTIC = '#f1fbe6'


def pod_low(m):
    """A pressure vessel on its cradles by the road, 10 m of it: a ribbed hull lying along the road, domed ends, a
    hatch, dark ports, a manifold with two valve wheels, ballast blocks. Under 2.6 m, dark and unlit: it may stand
    right behind the barrier, and nothing on it can read as a pickup."""
    R, L = 105.0, 760.0
    rod(m, STEEL, (-L / 2.0, 0, 125), (L / 2.0, 0, 125), R, steps=10)
    for s in (-1, 1):
        m.sphere(STEEL, (s * L / 2.0, 0, 125), R, scale=(0.55, 1, 1), steps=(5, 10))
    for k in range(4):
        x = -L / 2.0 + 95 + k * 190
        rod(m, STATION_D, (x - 16, 0, 125), (x + 16, 0, 125), R + 9, steps=10)
    for x in (-230, 230):
        m.box(STEEL_D, (x, 0, 0), (76, 260, 120))
    m.cyl(STATION_D, (-95, 0, 212), 54, 34, steps=8)
    m.cyl(STEEL_D, (-95, 0, 246), 40, 10, steps=8)
    for x in (95, 285):                                                                  # dark ports on the road side
        rod(m, STATION_D, (x, -R + 12, 135), (x, -R - 8, 135), 34, steps=8)
        rod(m, PORT, (x, -R - 8, 135), (x, -R - 12, 135), 24, steps=8)
    m.box(STEEL_D, (560, -30, 40), (150, 170, 200))                                      # the manifold
    for y in (-70, 20):
        rod(m, STATION_D, (560, y, 150), (560, y, 176), 46, steps=8)
    rod(m, STEEL, (L / 2.0 + 40, -30, 100), (486, -30, 100), 30, steps=6)
    for x, y, s in ((-560, -120, 90), (-600, 60, 70), (-470, 130, 60)):
        m.box(STEEL_D, (x, y, s * 0.3), (s * 1.3, s, s))


def pressure_hab(m):
    """A pressure habitat, 11 m: a sphere 7 m across on three legs, a band of dark ports with a steady light behind
    them, an access tube down to the sand; a standing tank beside it with a domed head, joined by a tube; a dish."""
    m.sphere(STATION, (0, 0, 640), 360, steps=(6, 12))
    m.cyl(STATION_D, (0, 0, 612), 366, 56, steps=12)
    for k in range(6):
        a = k * math.pi / 3 + 0.5
        m.box(PORT, (math.cos(a) * 356, math.sin(a) * 356, 640), (12, 120, 64), rot=(0, math.degrees(a), 0))
        m.box('#fff1c0', (math.cos(a) * 362, math.sin(a) * 362, 640), (6, 84, 34), rot=(0, math.degrees(a), 0), glow=0.5)
    for k in range(3):
        a = k * 2 * math.pi / 3 + 0.3
        rod(m, STEEL, (math.cos(a) * 250, math.sin(a) * 250, 360), (math.cos(a) * 540, math.sin(a) * 540, -60), 36, steps=6)
        m.box(STEEL_D, (math.cos(a) * 540, math.sin(a) * 540, 0), (170, 170, 60))
    m.cyl(STEEL_D, (0, 0, -40), 84, 340, steps=8)
    m.cyl(STEEL, (640, 140, -40), 175, 880, steps=10)                                    # the standing tank
    m.sphere(STEEL, (640, 140, 840), 175, scale=(1, 1, 0.7), steps=(4, 10))
    for z in (160, 460, 760):
        m.cyl(STATION_D, (640, 140, z), 184, 34, steps=10)
    rod(m, STATION_D, (320, 70, 640), (480, 105, 640), 72, steps=8)
    m.cyl(STATION_D, (0, 0, 990), 12, 150, steps=5)
    m.cyl(STATION, (0, 0, 1120), 110, 30, steps=8, top_r=30)


def tethered(m):
    """Suspended scenery: three buoyancy spheres on their chains, 9, 15 and 21 m up, each over its anchor block; one
    carries an instrument cage. life() lets them sway on their chains. Unlit."""
    for k, (x, y, h, r, col) in enumerate(((-430, 70, 800, 150, STATION), (110, -90, 1380, 190, STATION_D), (560, 130, 1960, 160, STATION))):
        m.box(STEEL_D, (x, y, 10), (170, 170, 130))
        m.box(HULL_D, (x, y, (h + 60) / 2.0), (20, 20, h - 60))
        m.sphere(col, (x, y, h + r), r, steps=(5, 9))
        m.cyl(RUST if k % 2 else STEEL, (x, y, h + r - 18), r + 5, 36, steps=9)
        m.cyl(STEEL_D, (x, y, h + 2 * r - 6), 40, 30, steps=6)
    m.box(STEEL_D, (110, -90, 1180), (150, 150, 170))                                    # the cage under the middle one
    m.box(STATION_D, (110, -90, 1180), (120, 164, 60))


RAFT_Z = 1500.0


def kelp_raft(m, seed=52):
    """Drifting vegetation: a raft of weed 14 m long hanging in the water 15 m up, floats along it, fronds trailing
    under it. life() lets it wander and its fronds swing."""
    r = random.Random(seed)
    for k in range(4):
        x = -480 + k * 320 + r.uniform(-40, 40)
        m.sphere(KELP_D if k % 2 else KELP, (x, r.uniform(-120, 120), RAFT_Z + r.uniform(-20, 20)), r.uniform(240, 330), scale=(1.2, 0.9, 0.16), steps=(4, 7))
    for k in range(6):
        m.sphere('#c9b44a', (r.uniform(-600, 600), r.uniform(-220, 220), RAFT_Z + 34), r.uniform(26, 40), steps=(3, 5))
    for k in range(9):
        x, y = r.uniform(-620, 620), r.uniform(-230, 230)
        n = r.choice((2, 3, 3))
        p = (x, y, RAFT_Z - 10)
        for j in range(n):
            q = (p[0] + r.uniform(-50, 90), p[1] + r.uniform(-40, 40), p[2] - r.uniform(170, 260))
            tilt_box(m, [KELP, KELP_D, KELP_L][(k + j) % 3], p, q, 40, 12)
            p = q


def _whale(m, x0, y0, z0, s, col, belly):
    """One whale as a silhouette with some body to it, nose towards +x, s = its size (1 = 30 m)."""
    def P(pts):
        return [(x0 + x * s, z0 + z * s) for x, z in pts]
    body = [(1500, 60), (1380, 300), (900, 470), (200, 520), (-600, 400), (-1200, 200), (-1500, 120), (-1500, 20), (-1100, -60), (-300, -330),
            (500, -380), (1100, -300), (1420, -120)]
    m.side(col, _ccw(P(body)), y0 - 260 * s, y0 + 260 * s)
    m.side(belly, _ccw(P([(1380, -150), (1050, -330), (300, -400), (-250, -350), (300, -290), (1000, -230)])), y0 - 268 * s, y0 + 268 * s)
    m.plan(col, _ccw([(x0 + x * s, y0 + y * s) for x, y in ((-1420, 0), (-1750, 520), (-1950, 480), (-1720, 0), (-1950, -480), (-1750, -520))]),
           z0 + 30 * s, z0 + 110 * s)                                                    # the flukes
    for sy in (-1, 1):
        tilt_box(m, col, (x0 + 620 * s, y0 + sy * 240 * s, z0 - 220 * s), (x0 + 260 * s, y0 + sy * 620 * s, z0 - 420 * s), 260 * s, 40 * s)


def whale_pod(m):
    """Marine silhouettes for the middle distance and the horizon: three whales, 30, 24 and 15 m, in three receding
    tones, 30 to 55 m over the piece's base. A Far piece: z = 0 stands 25 m under the road. life() lets them heave."""
    _whale(m, 0, 0, 4300, 1.0, DEEP1, DEEP3)
    _whale(m, -2600, 700, 5300, 0.8, DEEP2, DEEP3)
    _whale(m, 1500, -500, 3500, 0.5, DEEP2, BELLY)


def hull_above(m):
    """Suspended scenery for the horizon: a ship on the surface seen from below, 70 m of her: the hull's shape, a keel,
    a rudder and a screw, her anchor cable slanting down to the bottom. A Far piece (z = 0 stands 25 m under the
    road): the surface is 107 to 113 m over that, so the hull is drawn from 99 m up and its top is above the water."""
    Z = 9900.0
    m.side(DEEP1, _ccw([(-3500, Z + 900), (-3300, Z + 150), (-2600, Z), (2300, Z), (3100, Z + 500), (3500, Z + 1500), (3500, Z + 2000), (-3500, Z + 2000)]), -600, 600)
    m.side(RUST_D, _ccw([(-3350, Z + 420), (-3250, Z + 160), (-2600, Z + 10), (2300, Z + 10), (3080, Z + 500), (3200, Z + 760)]), -612, 612)
    m.box(DEEP1, (-200, 0, Z - 60), (4200, 80, 160))                                     # the keel
    m.box(DEEP1, (-3380, 0, Z + 260), (40, 60, 560))                                     # rudder, and the screw as a disc
    rod(m, DEEP2, (-3180, 0, Z + 230), (-3140, 0, Z + 230), 200, steps=8)
    tilt_box(m, DEEP1, (2500, 0, Z + 100), (5200, 400, 700), 120, 120)                   # the anchor cable, thick as a mast


CAUSTIC_Z = 27.0     # over the piece's own zero: the track sinks a piece 15 cm, so the net lies 12 cm over the sand
CAUSTIC_GLOW = 0.07  # M_Life multiplies a glow by 10.5: this is light on sand, not a lamp


def caustic_net(m, seed=53):
    """Moving caustics for the sand: a net of pale light 30 x 20 m, lying 12 cm over the sand: five ribbons along the
    road and four across it, each a wandering line. life() lets every part of it wander by where it is in the world
    (so the net swims) and runs soft bands of light across it. Flat: it casts nothing and stands nowhere near the road."""
    r = random.Random(seed)
    for k in range(5):
        y0 = -820 + k * 410 + r.uniform(-60, 60)
        ph, amp, wid = r.uniform(0, 6.28), r.uniform(90, 160), r.uniform(34, 52)
        mid = [(-1500 + 3000 * i / 10.0, y0 + amp * math.sin(i * 0.9 + ph) + 0.4 * amp * math.sin(i * 2.1 + ph * 2)) for i in range(11)]
        m.plan(CAUSTIC, _ccw([(x, y + wid / 2.0) for x, y in mid] + [(x, y - wid / 2.0) for x, y in reversed(mid)]), CAUSTIC_Z, CAUSTIC_Z + 3, glow=CAUSTIC_GLOW)
    for k in range(4):
        x0 = -1150 + k * 760 + r.uniform(-80, 80)
        ph, amp, wid = r.uniform(0, 6.28), r.uniform(100, 170), r.uniform(30, 46)
        mid = [(x0 + amp * math.sin(i * 1.0 + ph) + 0.4 * amp * math.sin(i * 2.3 + ph), -1000 + 2000 * i / 8.0) for i in range(9)]
        m.plan(CAUSTIC, _ccw([(x + wid / 2.0, y) for x, y in mid] + [(x - wid / 2.0, y) for x, y in reversed(mid)]), CAUSTIC_Z + 1, CAUSTIC_Z + 4, glow=CAUSTIC_GLOW)


# ---- what moves by itself: props built with ft_life.Kit (parts made about the origin and sent round by the material)
def _fish(m, x, y, z, yaw, s, body, tail):
    """One chunky fish, nose along its yaw: for schools seen from 30 m and more."""
    a = math.radians(yaw)
    m.sphere(body, (x, y, z), 56 * s, scale=(1.0, 0.36, 0.5), steps=(3, 6), rot=(0, yaw, 0), glow=0.05)      # as bright as the level's own schools, no more
    m.box(tail, (x - math.cos(a) * 62 * s, y - math.sin(a) * 62 * s, z), (40 * s, 8 * s, 62 * s), rot=(0, yaw, 0))


def fish_ring(k):
    """A school turning on itself, 14 m across, 4 to 9 m over the sand: thirty fish nose to tail round a ring. Two or
    three stood one over another, larger as they go up, make the fish tornado."""
    r = random.Random(61)
    g = k.group('Spin', SpinPivot=(0.0, 0.0, 0.0), SpinAxis=(0.0, 0.0, 1.0), SpinRate=0.55, SpinRand=1.0)
    for i in range(30):
        a = 2 * math.pi * i / 30.0 + r.uniform(-0.06, 0.06)
        rad = r.uniform(520, 720)
        _fish(g, math.cos(a) * rad, math.sin(a) * rad, r.uniform(430, 900), math.degrees(a) + 90.0, r.uniform(0.8, 1.25),
              [FISH_A, FISH_B, FISH_A, FISH_C][i % 4], STATION_D)
    return dict(grow=(60, 60, 0), grow_down=(60, 60, 0))


def fish_stream(k):
    """A school on the move, 60 m of its road: seven knots of four fish that travel a line along the road, 5 m up,
    and come round again."""
    r = random.Random(62)
    g = k.group('Conveyor', ConvStart=(-3000.0, 0.0, 460.0), ConvDir=(1.0, 0.0, 0.0), ConvLen=6000.0, ConvRate=0.07, ConvRand=1.0, ConvArc=170.0)
    for i in range(7):
        def knot(m, i=i):
            for j in range(4):
                _fish(m, r.uniform(-160, 160), r.uniform(-130, 130), r.uniform(-90, 90), r.uniform(-8, 8), r.uniform(0.8, 1.2),
                      [FISH_A, FISH_C, FISH_A, FISH_B][(i + j) % 4], STATION_D)
        g.piece(knot, a=i / 7.0)
    return dict(grow=(3200, 200, 800), grow_down=(3200, 200, 0))


def _shark(g, rad, z, s=1.0):
    """A shark 6 m long swimming a round: drawn at `rad` out along +x, nose towards +y (the group's base turns it into
    its place on the round)."""
    g.sphere(DEEP1, (rad, 0, z), 300 * s, scale=(0.3, 1.0, 0.34), steps=(4, 8))
    g.sphere(BELLY, (rad, 20 * s, z - 34 * s), 250 * s, scale=(0.26, 1.0, 0.24), steps=(3, 6))
    tilt_box(g, DEEP1, (rad, 20 * s, z + 60 * s), (rad, -90 * s, z + 210 * s), 20 * s, 110 * s)             # the dorsal fin
    tilt_box(g, DEEP1, (rad, -270 * s, z), (rad, -400 * s, z + 150 * s), 18 * s, 80 * s)                    # the tail: two lobes
    tilt_box(g, DEEP1, (rad, -270 * s, z), (rad, -370 * s, z - 90 * s), 18 * s, 60 * s)
    for sx in (-1, 1):
        tilt_box(g, DEEP1, (rad + sx * 60 * s, 70 * s, z - 40 * s), (rad + sx * 230 * s, -40 * s, z - 110 * s), 90 * s, 16 * s)


def shark_ring(k):
    """Marine silhouettes: three sharks on their rounds, 15 to 20 m out from the middle and 12 to 21 m up, dark from
    below. For 80 m and more from the road."""
    g = k.group('Spin', SpinPivot=(0.0, 0.0, 0.0), SpinAxis=(0.0, 0.0, 1.0), SpinRate=0.16, SpinRand=1.0)
    for a, rad, z, s in ((0.0, 1500.0, 1250.0, 1.0), (125.0, 1900.0, 1700.0, 1.2), (250.0, 1650.0, 2050.0, 0.9)):
        g.base = ft_art._xf((0, 0, 0), (0, a, 0))
        _shark(g, rad, z, s)
    g.base = None
    return dict(grow=(450, 450, 0), grow_down=(450, 450, 0))


def manta_flight(k):
    """Marine silhouettes near the road: three rays in formation, 9 m across, that glide a 90 m line along the road
    13 m up and come round again. Dark above, pale below."""
    g = k.group('Conveyor', ConvStart=(-4500.0, 0.0, 1300.0), ConvDir=(1.0, 0.0, 0.0), ConvLen=9000.0, ConvRate=0.045, ConvRand=1.0, ConvArc=220.0)
    for i, (dy, dz, s) in enumerate(((0.0, 0.0, 1.0), (-520.0, 140.0, 0.8), (480.0, -120.0, 0.7))):
        def ray(m, dy=dy, dz=dz, s=s):
            wing = [(330 * s, 0), (60 * s, 450 * s), (-200 * s, 300 * s), (-260 * s, 0), (-200 * s, -300 * s), (60 * s, -450 * s)]
            m.plan(DEEP1, _ccw([(x, y + dy) for x, y in wing]), dz, dz + 36 * s)
            m.plan(BELLY, _ccw([(x * 0.9, y * 0.9 + dy) for x, y in wing]), dz - 12 * s, dz)
            m.box(DEEP1, (-470 * s, dy, dz + 16 * s), (460 * s, 20 * s, 20 * s))
            for sy in (-1, 1):
                m.box(DEEP1, (350 * s, dy + sy * 70 * s, dz + 14 * s), (130 * s, 36 * s, 30 * s))
        g.piece(ray, a=0.06 * i)
    return dict(grow=(4900, 300, 1700), grow_down=(4900, 300, 0))


def bubble_vent(k):
    """Rising bubbles: a vent in a mound of dark rock and the column of bubbles over it, 26 m high. They leave the
    crack small, swell as they climb, lean with the current and are gone near the top."""
    s = k.group()
    s.sphere(VENT_D, (0, 0, -20), 300, scale=(1.2, 1.0, 0.4), steps=(4, 8))
    s.cyl(VENT, (20, 0, 40), 90, 150, steps=6, top_r=46)
    s.cyl(VENT_D, (-110, 60, 20), 60, 90, steps=6, top_r=30)
    p = k.group('Plume', rough=0.3, PlumeBase=(20.0, 0.0, 190.0), PlumeRise=2500.0, PlumeEase=1.0, PlumeDrift=260.0, PlumeSpread=90.0,
                PlumeSize0=0.4, PlumeSize1=1.5, PlumeRate=0.11)
    for i in range(14):
        p.piece(lambda m: m.sphere(BUBBLE, (0, 0, 0), 34, steps=(4, 6), glow=0.04), a=i / 14.0, b=0.6 + 0.8 * ((i * 5) % 7) / 6.0, c=i * 0.37)
    return dict(grow=(420, 420, 2800), grow_down=(420, 420, 0))


def whale_pass(k):
    """The whales that cross over the road at Nereida's gate: a cow 34 m long and her calf, 38 m up, swimming a line
    400 m long across the road and coming round again. Built with their noses along +y (the way they go)."""
    g = k.group('Conveyor', ConvStart=(0.0, -20000.0, 0.0), ConvDir=(0.0, 1.0, 0.0), ConvLen=40000.0, ConvRate=0.011, ConvRand=1.0)
    for n, (a, s, dx, dz) in enumerate(((0.0, 1.13, 0.0, 0.0), (0.035, 0.5, 900.0, -500.0), (0.5, 1.0, 300.0, 300.0))):
        def one(m, s=s, dx=dx, dz=dz):
            m.base = ft_art._xf((dx, 0, dz), (0, 90, 0))
            try:
                _whale(m, 0, 0, 0, s, DEEP1, DEEP3)
            finally:
                m.base = None
        g.piece(one, a=a)
    return dict(grow=(1200, 20500, 700), grow_down=(1200, 20500, 700))


LIFE_JOBS = [('SM_FishRing', fish_ring), ('SM_FishStream', fish_stream), ('SM_SharkRing', shark_ring), ('SM_MantaFlight', manta_flight),
             ('SM_BubbleVent', bubble_vent), ('SM_WhalePass', whale_pass)]


def life_props(only=None):
    """Build the props that move by themselves (ft_life.Kit: saved with their M_Life instances on them; they do not go
    through ft_dress.export). only: names to build. Returns the names built."""
    import unreal
    import ft_life
    done = []
    for name, fn in LIFE_JOBS:
        if only and name not in only:
            continue
        try:
            k = ft_life.Kit()
            ext = fn(k) or {}
            sm = k.save('/Game/Maps/Abyss/Dress/' + name, grow=ext.get('grow', (0.0, 0.0, 0.0)), grow_down=ext.get('grow_down', (0.0, 0.0, 0.0)))
            # no shadow from these: the track keeps a moving prop's shadow where it was first drawn (a cached page), and a
            # shadow standing still under a school that turns or a chair that travels is worse than none
            sub = unreal.get_editor_subsystem(unreal.StaticMeshEditorSubsystem)
            for i in range(len(sm.get_editor_property('static_materials'))):
                sub.enable_section_cast_shadow(sm, False, 0, i)
            unreal.EditorAssetLibrary.save_loaded_asset(sm)
            done.append(name)
        except Exception as e:  # noqa
            import traceback
            unreal.log_error('FTDRESS abyss/%s FAILED: %r %s' % (name, e, traceback.format_exc()[-900:]))
            print('JAM ERR life prop abyss', name, repr(e)[:300], flush=True)
    print('JAM life props abyss built', len(done), ' '.join(done), flush=True)
    return done


JOBS = [
    ('SM_CutReef', cut_reef), ('SM_CutReef2', lambda m: cut_reef(m, seed=4, growth=True)), ('SM_CutShoal', cut_shoal),
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
    # art pass 2: the seabed itself (see "the third round")
    ('SM_ReefFloor', reef_floor), ('SM_ReefFloor2', lambda m: reef_floor(m, seed=47)), ('SM_CableTrench', cable_trench),
    ('SM_SubPad', sub_pad), ('SM_WhaleBones', whale_bones),
    # the zone pass (see "the fourth round"): pressure vessels, what hangs in the water, silhouettes, the net of light
    ('SM_PodLow', pod_low), ('SM_PressureHab', pressure_hab), ('SM_Tethered', tethered), ('SM_KelpRaft', kelp_raft),
    ('SM_WhalePod', whale_pod), ('SM_HullAbove', hull_above), ('SM_CausticNet', caustic_net),
]
