"""FULL THROTTLE (Unreal) the dressing, level 3: Nevada, Highway 93 to Las Vegas (3.2).

The kit that fills the desert: ranch fence and phone lines by the road, the dry wash's cut banks and its fallen
bridge, a biker roadhouse and its camp, diners, trailers, water tanks and scrapyards, a power corridor marching off,
canyon walls, arches and hoodoos for Red Rock Pass, the Syndicate's hangar dug into a rock with its radar post, the
casinos of Jackpot Junction, and a horizon of mesas, monument buttes, far ranges and a neon town.
Placed by Source/FullThrottle/FTDressL2.inl. Rebuild: import ft_dress; ft_dress.run(['nevada'])

Drawn like ft_desert's props (ft_art.Model, cm, the same sunset palette), with the dressing's own axes: the long side
along X (X is laid along the road), the FRONT towards -Y (the placement turns it to the road), the base on z = 0.
Near pieces (fence, sage, junk) are low and unlit; the wash banks step up from 21 m and top out past 24 m. Rock pieces carry the names the export's finish
pass treats as rock (SM_Rock*, SM_Ridge*, SM_Butte*, SM_Peak*). Horizon pieces are built from z = 0, 90 to 150 m tall.
"""
import math
import random
from ft_desert import (SAND, SANDST, SANDST_D, SANDST_L, SAGE, SAGE_D, BARK, BARK_D, WOOD, LEATHER, GREY, FLAME, RED, PURP, MAG,
                       TEAL, NAVY, rod, tilt_box)
from ft_car import BLACK, GLASS, CHROME, STEEL, GOLD, CYAN, YEL, WHITE
from ft_dress_chem import px, py, pz, buried as _buried, drum, QUERY, FAR_KEEP

LAND = '#a2507a'      # the desert's outer land and the floor under the level (FTTrack GroundBands, level 2)
MIDLAND = '#c9805a'   # the land between the roadside and the edge: the footing of anything that stands on the land
ROCK_R = '#c9603e'    # a redder band
DUSK = '#b0607a'      # rock gone pink with distance
DUSK_D = '#8a4a7a'
DUSK_L = '#c98a9a'
CREAM = '#f4efe6'
CONC = '#c9b8a0'
RUST = '#b8553a'
DRYG = '#d8b060'      # dry grass
WIRE = '#3a2a2a'
WARM = '#ffc86a'
SYN = '#1c1830'       # Syndicate black


def buried(fn, col=MIDLAND, keep=0.0):
    return _buried(fn, col, keep=keep)


def piled(fn, half=60.0):
    """For a piece too long to sit on one buried block (the power span runs 180 m, over ground that rises and falls):
    a narrow pile under its origin, flush with the base, does the same job of sending the finish pass's panel below it."""
    def draw(m):
        fn(m)
        top = 0.0
        for mesh in m.meshes.values():
            top = max(top, QUERY.get_mesh_bounding_box(mesh).max.z)
        d = max(top, 50.0)
        m.box(MIDLAND, (0, 0, -d / 2.0), (half * 2, half * 2, d))
    return draw


def car(m, x, y, yaw, col, roof=None, wheels=True):
    """A boxy eighties saloon at (x, y), turned by yaw."""
    a = math.radians(yaw)
    ca, sa = math.cos(a), math.sin(a)

    def at(dx, dy, dz):
        return (x + dx * ca - dy * sa, y + dx * sa + dy * ca, dz)
    m.box(col, at(0, 0, 62), (430, 172, 62), rot=(0, yaw, 0))
    m.box(roof or col, at(-20, 0, 120), (220, 150, 56), rot=(0, yaw, 0))
    m.box(GLASS, at(96, 0, 118), (10, 140, 44), rot=(0, yaw, 0))
    if wheels:
        for dx in (-130, 130):
            for dy in (-80, 80):
                rod(m, BLACK, at(dx, dy - 12, 32), at(dx, dy + 12, 32), 32, steps=8)


def bike(m, x, y, yaw):
    a = math.radians(yaw)
    ca, sa = math.cos(a), math.sin(a)

    def at(dx, dz):
        return (x + dx * ca, y + dx * sa, dz)
    for dx in (-70, 80):
        rod(m, BLACK, (at(dx, 34)[0] - sa * 7, at(dx, 34)[1] + ca * 7, 34), (at(dx, 34)[0] + sa * 7, at(dx, 34)[1] - ca * 7, 34), 34, steps=8)
    tilt_box(m, CHROME, at(-70, 40), at(40, 70), 14)
    tilt_box(m, CHROME, at(40, 70), at(96, 40), 10)
    tilt_box(m, CHROME, at(40, 70), at(70, 120), 8)
    m.box(LEATHER, at(-30, 78), (70, 26, 14), rot=(0, yaw, 0))
    m.box(PURP, at(10, 70), (50, 24, 22), rot=(0, yaw, 0))


def strata(m, outline, layers, taper=0.07, cx=0.0, cy=0.0):
    """Stack plan polygons into banded rock: outline [(x, y)], layers [(z0, z1, colour)]; each layer steps in a little."""
    for k, (z0, z1, col) in enumerate(layers):
        f = 1.0 - taper * k
        m.plan(col, [(cx + (x - cx) * f, cy + (y - cy) * f) for (x, y) in outline], z0, z1)


def ring(r, cx, cy, rx, ry, n=9, rough=0.22):
    """A lumpy outline for a rock."""
    pts = []
    for k in range(n):
        a = 2 * math.pi * k / n
        q = 1.0 + r.uniform(-rough, rough)
        pts.append((cx + math.cos(a) * rx * q, cy + math.sin(a) * ry * q))
    return pts


# ------------------------------------------------------------------------------------------ the near roadside
def wire_fence(m, L=1700):
    """Ranch fence: weathered posts, two strands, one post leaning and a strand down."""
    n = 5
    strand = '#8a7660'
    for k in range(n + 1):
        x = -L / 2.0 + L * k / n
        lean = 14 if k == 2 else 0
        m.box(BARK if k % 2 else BARK_D, (x, lean * 0.6, 68), (11, 11, 136), rot=(0, 0, lean))
    for z in (62, 114):
        m.box(strand, (0, 0, z), (L, 3, 3))
    tilt_box(m, strand, (L * 0.1, 0, 114), (L * 0.3, 30, 20), 3)
    for k in range(4):
        m.box(DRYG, (-L * 0.4 + k * L * 0.27, 12, 16), (8, 30, 34), rot=(0, 40 * k, 12))


def wire_line(m):
    """A phone line: three poles with cross-arms and insulators, the wires hanging between them."""
    xs = (-1500, 0, 1500)
    for x in xs:
        pz(m, BARK, x, 0, 0, 900, 13, steps=8)
        for z, w in ((820, 240), (740, 180)):
            m.box(BARK_D, (x, 0, z), (10, w, 10))
            for s in (-1, 1):
                pz(m, WHITE, x, s * (w / 2.0 - 14), z + 5, z + 22, 6, steps=6)
        tilt_box(m, BARK_D, (x, 20, 700), (x, 80, 815), 6)
    for y, z in ((-106, 842), (106, 842), (-76, 762), (76, 762)):
        for i in range(2):
            x0, x1 = xs[i], xs[i + 1]
            xm = (x0 + x1) / 2.0
            tilt_box(m, WIRE, (x0, y, z), (xm, y, z - 46), 5)
            tilt_box(m, WIRE, (xm, y, z - 46), (x1, y, z), 5)
        tilt_box(m, WIRE, (xs[0] - 750, y, z - 46), (xs[0], y, z), 5)
        tilt_box(m, WIRE, (xs[2], y, z), (xs[2] + 750, y, z - 46), 5)


def sagebrush(m, seed=0):
    """Sage, stones and dry grass: the desert floor's own clutter."""
    r = random.Random(seed + 11)
    for k in range(5):
        x, y = r.uniform(-260, 260), r.uniform(-200, 200)
        rr = r.uniform(40, 80)
        m.sphere(SAGE if k % 2 else SAGE_D, (x, y, rr * 0.5), rr, scale=(1, 1, 0.7), steps=(5, 9))
    for k in range(3):
        x, y = r.uniform(-300, 300), r.uniform(-220, 220)
        rr = r.uniform(22, 48)
        m.sphere(SANDST if k % 2 else SANDST_D, (x, y, rr * 0.4), rr, scale=(1.2, 1, 0.6), steps=(3, 6), rot=(0, r.uniform(0, 180), 0))
    for k in range(6):
        x, y = r.uniform(-280, 280), r.uniform(-210, 210)
        m.box(DRYG, (x, y, 22), (7, 26, 46), rot=(r.uniform(-14, 14), r.uniform(0, 180), 0))


def junk_pile(m):
    """Tyres, drums, a car bonnet, a fridge: what collects behind a desert garage."""
    for k in range(5):
        pz(m, BLACK, -120 + (k % 2) * 8, 40, k * 24, k * 24 + 22, 42, steps=10)
    rod(m, BLACK, (-30, -40, 42), (-30, -64, 46), 42, steps=10)
    drum(m, 90, 70, 0, RUST)
    drum(m, 160, 20, 0, TEAL)
    drum(m, 60, -60, 0, RED, lying=True, yaw=70)
    m.box(RUST, (200, -90, 40), (150, 120, 8), rot=(-28, 20, 0))
    m.box(CREAM, (-230, -50, 80), (70, 66, 160), rot=(0, 12, 6))
    m.box(WOOD, (10, 150, 34), (90, 80, 68))
    m.box(BARK_D, (-200, 140, 8), (240, 16, 16), rot=(0, 30, 0))


def wreck_car(m):
    """A stripped saloon on blocks, bonnet up, one wheel left leaning on it."""
    for x in (-130, 130):
        for y in (-70, 70):
            m.box(CONC, (x, y, 16), (44, 30, 32))
    m.box(RUST, (0, 0, 70), (430, 172, 62))
    m.box(ROCK_R, (-20, 0, 128), (220, 150, 56))
    m.box(SYN, (96, 0, 126), (10, 138, 42))
    m.box(RUST, (190, 0, 134), (150, 160, 8), rot=(38, 0, 0))     # the bonnet, up: the whole wreck stays under 2 m
    rod(m, BLACK, (-150, -112, 34), (-150, -92, 40), 33, steps=8)
    m.box(SANDST_D, (-60, 110, 6), (200, 90, 12))


def rock_bank(m, seed=0, L=2600, D=760):
    """The wash's cut bank: banded earth stepping up from the road side, slumped boulders, sage on the lip."""
    r = random.Random(seed + 5)
    n = 7
    front = [(-L / 2.0 + L * k / (n - 1), -D / 2.0 + r.uniform(0, 150)) for k in range(n)]
    for k, (z0, z1, col) in enumerate(((0, 90, SANDST_D), (90, 170, SANDST), (170, 230, SANDST_L), (230, 262, SAND))):
        pts = [(x, y + k * 95) for (x, y) in front] + [(L / 2.0, D / 2.0), (-L / 2.0, D / 2.0)]
        m.plan(col, pts, z0, z1)
    for k in range(6):
        x = r.uniform(-L * 0.45, L * 0.45)
        rr = r.uniform(40, 95)
        m.sphere(SANDST if k % 2 else ROCK_R, (x, -D / 2.0 + r.uniform(-60, 60), rr * 0.5), rr, scale=(1.2, 1, 0.7), steps=(5, 8), rot=(0, r.uniform(0, 180), 0))
    for k in range(5):
        m.sphere(SAGE if k % 2 else SAGE_D, (r.uniform(-L * 0.45, L * 0.45), r.uniform(0, D * 0.4), 290), r.uniform(40, 62), scale=(1, 1, 0.7), steps=(5, 8))


def wash_bed(m, L=6400, W=1500):
    """The wash itself: a pale gravel bed between low cut banks, dried mud, boulders and drift wood. The long side lies
    along X; the table turns it across the road, so the bed runs away from the highway on both sides of it. Low: it
    starts 20 m from the centre line, so nothing in it stands over 2 m."""
    r = random.Random(77)
    m.box('#ecd2a8', (0, 0, 16), (L, W, 32))
    for k in range(7):
        x = -L / 2.0 + L * (k + 0.5) / 7
        m.box('#c9a078', (x + r.uniform(-200, 200), r.uniform(-W * 0.25, W * 0.25), 33), (r.uniform(320, 620), r.uniform(200, 420), 6), rot=(0, r.uniform(0, 90), 0))
    h = W / 2.0
    for (y0, y1, y2, y3) in ((h, h, h + 260, h + 340), (-h - 340, -h - 260, -h, -h)):
        m.plan(SANDST_D, [(-L / 2.0, y0), (L / 2.0, y1), (L / 2.0, y2), (-L / 2.0, y3)], 0, 90)
    for (y0, y1, y2, y3) in ((h + 130, h + 100, h + 260, h + 340), (-h - 340, -h - 260, -h - 100, -h - 130)):
        m.plan(SANDST, [(-L / 2.0, y0), (L / 2.0, y1), (L / 2.0, y2), (-L / 2.0, y3)], 90, 160)
    for k in range(9):
        rr = r.uniform(45, 120)
        m.sphere(SANDST if k % 2 else ROCK_R, (r.uniform(-L * 0.46, L * 0.46), r.choice((-1, 1)) * r.uniform(h * 0.55, h * 0.95), 32 + rr * 0.35), rr,
                 scale=(1.2, 1, 0.7), steps=(4, 7), rot=(0, r.uniform(0, 180), 0))
    for k in range(3):
        x, y = r.uniform(-L * 0.4, L * 0.4), r.uniform(-h * 0.5, h * 0.5)
        a = r.uniform(0, math.pi)
        rod(m, BARK_D, (x, y, 48), (x + math.cos(a) * 260, y + math.sin(a) * 260, 62), 15, steps=6)
    for k in range(6):
        m.sphere(SAGE if k % 2 else SAGE_D, (r.uniform(-L * 0.45, L * 0.45), r.choice((-1, 1)) * (h + 230), 150), r.uniform(42, 58), scale=(1, 1, 0.7), steps=(4, 6))


def sign_frame(m):
    """An old roadside hoarding: a bleached board with panels missing, on timber legs with back struts. Unlit."""
    for x in (-360, 360):
        m.box(BARK, (x, 0, 300), (18, 18, 600))
        tilt_box(m, BARK_D, (x, 150, 0), (x, 0, 420), 12)
    m.box(BARK_D, (0, 0, 380), (900, 12, 14))
    m.box(BARK_D, (0, 0, 840), (900, 12, 14))
    for k in range(6):
        if k == 4:
            continue
        col = ('#e8d8b8', '#e0c8a0', '#e8d8b8', '#d8c8a8', '', '#e8d8b8')[k]
        m.box(col, (-375 + k * 150, -8, 610 - (14 if k == 2 else 0)), (146, 8, 440), rot=(0, 0, 3 if k == 2 else 0))
    m.box(TEAL, (-150, -14, 700), (430, 4, 60))
    m.box(MAG, (-150, -14, 560), (300, 4, 40))
    m.box('#e8d8b8', (300, 60, 30), (146, 440, 8), rot=(0, 25, 0))      # the panel that fell off


def mile_sign(m):
    """A highway shield on a post and two reflector posts."""
    m.box(STEEL, (0, 0, 130), (8, 8, 260))
    m.box(WHITE, (0, -6, 250), (90, 6, 96))
    m.box(BLACK, (0, -10, 250), (74, 4, 80))
    m.box(WHITE, (0, -13, 244), (60, 4, 56))
    m.box(TEAL, (0, -6, 170), (110, 6, 44))
    for x in (-260, 260):
        m.box(WHITE, (x, 0, 50), (10, 10, 100))
        m.box(RED, (x, -6, 88), (10, 4, 16))


# ------------------------------------------------------------------------------------------ the middle distance
def diner(m):
    """A streamline diner: rounded ends, a chrome waist, a band of warm windows, a neon roofline and a star sign."""
    m.box(CONC, (0, 0, 20), (1900, 760, 40))
    m.box(CREAM, (0, 0, 180), (1400, 520, 300))
    for x in (-700, 700):
        pz(m, CREAM, x, 0, 30, 330, 260, steps=14)
    m.box(GREY, (0, -263, 130), (1400, 8, 44))
    m.box(WARM, (0, -263, 228), (1300, 8, 96), glow=0.55)
    for k in range(8):
        m.box(GREY, (-650 + k * 186, -268, 228), (12, 6, 96))
    m.box(TEAL, (0, 0, 345), (1960, 600, 30))
    m.box(MAG, (0, -302, 345), (1960, 6, 18), glow=0.9)
    m.box(RED, (60, -266, 120), (110, 8, 190))
    m.box(NAVY, (-300, -60, 470), (620, 40, 190))
    m.box(MAG, (-300, -82, 548), (590, 4, 16), glow=1.0)
    m.box(MAG, (-300, -82, 392), (590, 4, 16), glow=1.0)
    m.box(CYAN, (-300, -82, 470), (460, 4, 70), glow=0.9)
    for k in range(5):
        a = k * 2 * math.pi / 5
        tilt_box(m, YEL, (320, -70, 520), (320 + math.sin(a) * 120, -70, 520 + math.cos(a) * 120), 22, 12, glow=1.0)
    car(m, -500, -520, 8, '#7fe3c4', CREAM)
    car(m, 250, -540, -6, '#ffc1dc')
    pz(m, STEEL, 880, -420, 0, 620, 12, steps=6)
    m.box(TEAL, (880, -420, 640), (200, 16, 90))


def garage(m):
    """A tin garage and scrapyard: two dark bays, a canopy, crushed cars stacked, a tyre wall, a wheel on a pole."""
    m.box(GREY, (0, 150, 220), (1300, 800, 440))
    m.side(TEAL, [(-680, 440), (680, 440), (680, 470), (0, 560), (-680, 470)], -260, 560)
    for x in (-330, 330):
        m.box(SYN, (x, -252, 180), (480, 8, 340))
        m.box(RUST, (x, -256, 362), (500, 6, 22))
    m.box(TEAL, (0, -420, 400), (1300, 340, 14), rot=(0, 0, 8))
    for x in (-600, 0, 600):
        m.box(STEEL, (x, -560, 190), (12, 12, 380))
    for k, col in enumerate((RUST, '#5a7a9a', YEL)):
        m.box(col, (980, 100, 38 + k * 66), (410, 170, 58), rot=(0, 6 * k - 6, 0))
        m.box(SYN, (980, 100, 70 + k * 66), (200, 150, 8), rot=(0, 6 * k - 6, 0))
    for k in range(7):
        pz(m, BLACK, -900, -200 + k * 90, 0, 60, 44, steps=8)
        if k % 2 == 0:
            pz(m, BLACK, -900, -200 + k * 90, 60, 120, 44, steps=8)
    pz(m, STEEL, -1050, -520, 0, 760, 14, steps=6)
    rod(m, RED, (-1050, -540, 860), (-1050, -520, 860), 110, steps=14)
    rod(m, WHITE, (-1050, -546, 860), (-1050, -540, 860), 76, steps=14)
    rod(m, BLACK, (-1050, -552, 860), (-1050, -546, 860), 40, steps=10)
    m.box(RED, (300, -600, 90), (60, 60, 180))
    wreck_car_at(m, 620, -640, 20)


def wreck_car_at(m, x, y, yaw):
    car(m, x, y, yaw, RUST, ROCK_R, wheels=False)
    m.box(CONC, (x, y, 16), (300, 140, 32), rot=(0, yaw, 0))


def water_tank(m):
    """A water tower: a banded tank under a conical roof on four braced legs."""
    for s in (-1, 1):
        for t in (-1, 1):
            tilt_box(m, STEEL, (s * 270, t * 270, 0), (s * 190, t * 190, 1080), 22)
    for z0, z1 in ((120, 560), (560, 1000)):
        for s in (-1, 1):
            tilt_box(m, STEEL, (s * 262, -262 + (z0 - 120) * 0.02, z0), (s * 200, 200, z1), 10)
            tilt_box(m, STEEL, (-262, s * 262, z0), (200, s * 200, z1), 10)
    m.box(STEEL, (0, 0, 1080), (460, 460, 20))
    pz(m, CREAM, 0, 0, 1090, 1500, 330, steps=16)
    pz(m, TEAL, 0, 0, 1250, 1330, 334, steps=16)
    pz(m, RUST, 0, 0, 1500, 1680, 350, steps=16, top_r=20)
    pz(m, STEEL, 0, 0, 0, 1090, 26, steps=8)
    m.box(STEEL, (0, -340, 1300), (40, 6, 420))


def trailer(m):
    """A trailer home: striped awning, an aerial, a dish, a gas bottle, a lit window."""
    m.box(CONC, (-330, 0, 18), (40, 200, 36))
    m.box(CONC, (330, 0, 18), (40, 200, 36))
    m.box(CREAM, (0, 0, 190), (940, 290, 270))
    m.box(TEAL, (0, -147, 150), (940, 4, 46))
    m.box(GREY, (0, 0, 332), (960, 310, 14))
    m.box(WARM, (-220, -147, 230), (150, 4, 80), glow=0.45)
    m.box(RUST, (120, -147, 170), (80, 4, 190))
    for k in range(6):
        m.box(MAG if k % 2 else CREAM, (-190 + k * 76, -270, 300 - 0), (74, 250, 8), rot=(0, 0, 12))
    for x in (-380, 190):
        m.box(STEEL, (x, -390, 135), (8, 8, 270))
    pz(m, STEEL, 400, 60, 339, 560, 5, steps=6)
    for z in (470, 520):
        m.box(STEEL, (400, 60, z), (6, 110, 6))
    m.sphere(GREY, (-380, 60, 400), 64, scale=(0.35, 1, 1), steps=(5, 10), rot=(25, 30, 0))
    rod(m, CREAM, (500, -40, 46), (600, -40, 46), 36, steps=10)
    for x in (-460, -380):
        m.box(TEAL, (x, -330, 40), (50, 50, 8))
        m.box(TEAL, (x, -306, 70), (50, 8, 60), rot=(0, 0, -14))


def roadhouse(m):
    """The Snake Eyes' bar: a low brick box behind a porch, the dice on the roof, the bikes in a row outside."""
    m.box(SANDST_D, (0, 200, 200), (1500, 800, 400))
    m.box(NAVY, (0, 200, 408), (1560, 860, 24))
    m.box(BARK_D, (0, -330, 340), (1500, 300, 14), rot=(0, 0, 9))
    for x in (-700, -230, 230, 700):
        m.box(BARK, (x, -460, 160), (16, 16, 320))
    m.box(BARK_D, (0, -330, 14), (1500, 300, 28))
    m.box(SYN, (0, -203, 150), (130, 8, 250))
    for x in (-470, 470):
        m.box(WARM, (x, -203, 220), (260, 8, 110), glow=0.5)
        m.box(BARK_D, (x, -208, 220), (10, 4, 110))
    # the sign: two dice and the word
    m.box(SYN, (0, 120, 520), (900, 30, 200))
    m.box(MAG, (150, 100, 520), (520, 8, 96), glow=1.0)
    for s, rot in ((-1, (14, 18, 8)), (1, (-10, -22, 30))):
        cx = -330 + s * 95
        m.box(RED, (cx, 100, 660 + 30 * s), (150, 150, 150), rot=rot)
        m.sphere(WHITE, (cx, 20, 660 + 30 * s), 22, scale=(1, 0.3, 1), steps=(4, 8), glow=0.6)
    for k in range(5):
        bike(m, -520 + k * 150, -640, 78 + 5 * (k % 3))
    drum(m, 720, -560, 28, RUST)
    drum(m, 650, -600, 28, TEAL)
    car(m, 600, -780, 172, '#5a7a9a')


def biker_camp(m):
    """A camp off the road: three tents, a fire in a drum, the colours on a pole, bikes, a log to sit on."""
    for (x, y, yaw, col) in ((-360, 120, 12, '#6a7a52'), (40, 260, -8, PURP), (420, 60, 24, '#c9a26a')):
        a = math.radians(yaw)
        for s in (-1, 1):
            tilt_box(m, col, (x + math.cos(a) * s * 110, y + math.sin(a) * s * 110, 0), (x, y, 175), 300, 8)
        m.box(SYN, (x - math.sin(a) * -150, y + math.cos(a) * -150, 60), (60, 4, 120), rot=(0, yaw, 0))
    pz(m, RUST, 0, -160, 0, 86, 30, steps=8)
    pz(m, FLAME, 0, -160, 86, 92, 26, steps=8, glow=1.2)
    pz(m, YEL, 0, -160, 92, 150, 16, steps=6, glow=1.3, top_r=2)
    pz(m, STEEL, -560, -120, 0, 640, 8, steps=6)
    m.box(PURP, (-466, -120, 560), (180, 4, 120))
    m.box(WHITE, (-466, -124, 560), (44, 4, 44))
    bike(m, 260, -260, 40)
    bike(m, 380, -300, 55)
    bike(m, -230, -330, 110)
    rod(m, BARK, (-150, -260, 22), (110, -300, 22), 22, steps=8)
    m.box(WOOD, (520, -160, 34), (90, 80, 68))
    drum(m, 600, -80, 0, TEAL)


def power_span(m, span=18000):
    """A transmission tower and the wires to the next one, 180 m on along +X: three in a row make a corridor."""
    H = 3000
    for s in (-1, 1):
        for t in (-1, 1):
            tilt_box(m, STEEL, (s * 320, t * 320, 0), (s * 70, t * 70, H), 30)
    for (z0, z1) in ((0, 1000), (1000, 2000), (2000, 3000)):
        w0, w1 = 320 - 250 * z0 / H, 320 - 250 * z1 / H
        for s in (-1, 1):
            tilt_box(m, STEEL, (s * w0, -w0, z0), (s * w1, w1, z1), 13)
            tilt_box(m, STEEL, (-w0, s * w0, z0), (w1, s * w1, z1), 13)
        m.box(STEEL, (0, 0, z1), (2 * w1 + 30, 2 * w1 + 30, 16))
    m.box(STEEL, (0, 0, H + 650), (110, 110, 1300))
    ends = []
    for (z, w) in ((H + 150, 1700), (H + 650, 2100), (H + 1150, 1300)):
        m.box(STEEL, (0, 0, z), (60, w, 40))
        for s in (-1, 1):
            tilt_box(m, STEEL, (0, s * 60, z + 220), (0, s * w / 2.0, z + 20), 12)
            pz(m, WHITE, 0, s * (w / 2.0 - 30), z - 170, z - 20, 16, steps=6)
            ends.append((s * (w / 2.0 - 30), z - 170))
    m.sphere(RED, (0, 0, H + 1330), 30, steps=(4, 6), glow=1.2)
    n = 5
    for (y, z) in ends:
        for k in range(n):
            u0, u1 = k / float(n), (k + 1) / float(n)
            s0, s1 = 520 * 4 * u0 * (1 - u0), 520 * 4 * u1 * (1 - u1)
            tilt_box(m, WIRE, (span * u0, y, z - s0), (span * u1, y, z - s1), 12)


def hangar(m):
    """The Syndicate's hide: a hangar door let into a rock outcrop, red-lit, under a camouflage net, with its mast,
    a watchtower, two black trucks and a landing pad."""
    r = random.Random(17)
    strata(m, ring(r, 0, 500, 1700, 1100, 10, 0.16), [(0, 300, SANDST_D), (300, 620, SANDST), (620, 900, ROCK_R), (900, 1120, SANDST_L)], 0.06, 0, 500)
    m.box(CONC, (-200, -700, 340), (1500, 420, 680))             # the portal, standing proud of the rock
    m.box(SYN, (-200, -716, 300), (1300, 400, 600))
    m.box(STEEL, (-640, -918, 290), (400, 10, 560))
    m.box(STEEL, (120, -918, 290), (300, 10, 560))
    m.box(RED, (-200, -922, 630), (1300, 6, 26), glow=1.1)
    for k in range(6):
        m.box(YEL if k % 2 else BLACK, (-740 + k * 216, -922, 28), (200, 6, 40))
    # the net over the dump
    m.box(SAGE_D, (1150, -560, 330), (900, 700, 8), rot=(0, 0, 10))
    for (x, y) in ((760, -860), (1540, -860), (760, -280), (1540, -280)):
        m.box(BARK_D, (x, y, 160), (10, 10, 330))
    for k in range(4):
        m.box(('#3a4a3a', SYN, '#4a3a2a', '#3a4a3a')[k], (900 + k * 150, -560 + (k % 2) * 90, 50 + (k % 2) * 30), (130, 110, 100))
    # the mast
    pz(m, STEEL, 600, 500, 1100, 2500, 20, steps=6)
    for z, w in ((1700, 300), (2050, 220), (2350, 150)):
        m.box(STEEL, (600, 500, z), (w, 10, 10))
        m.box(STEEL, (600, 500, z), (10, w, 10))
    m.sphere(RED, (600, 500, 2530), 34, steps=(4, 6), glow=1.4)
    # the watchtower
    for s in (-1, 1):
        for t in (-1, 1):
            tilt_box(m, BARK_D, (-1500 + s * 120, -700 + t * 120, 0), (-1500 + s * 90, -700 + t * 90, 700), 14)
    m.box(SYN, (-1500, -700, 790), (260, 260, 180))
    m.box(RED, (-1500, -832, 800), (200, 4, 16), glow=0.9)
    m.box(SYN, (-1500, -700, 900), (320, 320, 16))
    # trucks and the pad
    for (x, yaw) in ((-600, 4), (-1000, -8)):
        m.box(SYN, (x, -1000, 110), (460, 190, 150), rot=(0, yaw, 0))
        m.box(SYN, (x + 150, -1000, 220), (150, 180, 90), rot=(0, yaw, 0))
        m.box(RED, (x - 40, -1000 - 97, 120), (320, 4, 20), rot=(0, yaw, 0))
    pz(m, SYN, 1700, -1300, 0, 40, 380, steps=18)
    pz(m, YEL, 1700, -1300, 40, 44, 320, steps=18)
    pz(m, SYN, 1700, -1300, 44, 46, 270, steps=18)


def radar_post(m):
    """A listening post: a dish tilted at the sky on a lattice mast, a black cabin, a fence."""
    H = 1300
    for s in (-1, 1):
        for t in (-1, 1):
            tilt_box(m, STEEL, (s * 170, t * 170, 0), (s * 80, t * 80, H), 16)
    for z in (430, 860, H):
        w = 170 - 90 * z / H
        m.box(STEEL, (0, 0, z), (2 * w + 16, 2 * w + 16, 12))
    m.box(SYN, (0, 0, H + 40), (240, 240, 60))
    m.sphere(GREY, (0, -40, H + 330), 330, scale=(1, 0.3, 1), steps=(6, 14), rot=(0, 0, 32))
    tilt_box(m, STEEL, (0, -40, H + 330), (0, -250, H + 470), 12)
    m.sphere(RED, (0, -260, H + 476), 26, steps=(4, 6), glow=1.2)
    m.box(SYN, (430, 60, 130), (420, 300, 260))
    m.box(RED, (430, -92, 200), (380, 4, 18), glow=0.9)
    m.box(STEEL, (430, 60, 266), (440, 320, 12))
    for (x, y, sx, sy) in ((0, -420, 1500, 4), (-750, 0, 4, 840), (750, 0, 4, 840)):
        m.box(STEEL, (x, y, 110), (sx, sy, 6))
        m.box(STEEL, (x, y, 180), (sx, sy, 6))
    for k in range(6):
        m.box(STEEL, (-750 + k * 300, -420, 100), (8, 8, 200))


def casino(m):
    """Jackpot Junction's casino: a purple block stepped back at the top, a lit marquee, a blade sign, neon rings."""
    m.box('#3a2a6a', (0, 200, 400), (2200, 1300, 800))
    m.box('#4a3a8a', (0, 300, 950), (1700, 1000, 300))
    m.box(GOLD, (0, -456, 1100), (1700, 8, 30), glow=1.0)
    for f in range(3):
        for k in range(9):
            if (k + f) % 4 != 3:
                m.box(WARM if (k + f) % 3 else '#9ff0ff', (-960 + k * 240, -454, 330 + f * 190), (150, 8, 90), glow=0.6)
    m.box(SYN, (0, -640, 300), (1000, 400, 50))
    m.box(GOLD, (0, -842, 300), (1000, 6, 50), glow=1.1)
    for k in range(5):
        m.box('#fff3a8', (-440 + k * 220, -640, 268), (36, 36, 14), glow=1.4)
    for x in (-460, 460):
        m.box(GOLD, (x, -800, 138), (30, 30, 276))
    m.box(SYN, (1150, -480, 700), (130, 60, 1400))
    for k in range(5):
        m.box((MAG, CYAN, GOLD, MAG, CYAN)[k], (1150, -514, 320 + k * 210), (90, 6, 150), glow=1.2)
    for k in range(5):
        a = k * 2 * math.pi / 5
        tilt_box(m, YEL, (1150, -500, 1500), (1150 + math.sin(a) * 130, -500, 1500 + math.cos(a) * 130), 24, 12, glow=1.2)
    for k, col in enumerate((MAG, CYAN, GOLD)):
        rod(m, col, (-500 + k * 500, 300, 1250 + 60 * k), (-500 + k * 500, 320, 1250 + 60 * k), 190, steps=12, glow=1.1)
        rod(m, '#3a2a6a', (-500 + k * 500, 296, 1250 + 60 * k), (-500 + k * 500, 300, 1250 + 60 * k), 150, steps=12)
    car(m, -700, -1000, 90, '#ffd23f')
    car(m, -420, -1010, 88, CREAM)
    car(m, 620, -1000, 94, '#ff7ad9')


def chapel(m):
    """A neon chapel-of-chance: a white A-frame, a lit doorway, a tall arrow sign pointing in."""
    m.side(CREAM, [(-330, 0), (330, 0), (0, 560)], -380, 380)
    m.side(MAG, [(-350, 0), (-330, 0), (0, 560), (0, 590)], -388, -380, glow=0.9)
    m.side(MAG, [(350, 0), (330, 0), (0, 560), (0, 590)], -388, -380, glow=0.9)
    m.box(WARM, (0, -384, 150), (150, 8, 300), glow=0.6)
    m.box(CYAN, (0, -384, 380), (120, 8, 60), glow=0.9)
    pz(m, STEEL, 620, -300, 0, 900, 14, steps=6)
    m.box(SYN, (620, -306, 780), (420, 30, 240))
    m.side(YEL, [(420, 700), (700, 700), (700, 660), (820, 780), (700, 900), (700, 860), (420, 860)], -330, -322, glow=1.1)
    for k in range(6):
        m.sphere('#fff3a8', (450 + k * 64, -326, 930), 12, steps=(4, 6), glow=1.4)
    car(m, -620, -520, 15, '#7fe3c4', CREAM)


def bridge_ruin(m):
    """The old road's bridge over the wash, down: two piers, a span hanging off one, a slab in the bed, the barricade."""
    for x in (-900, 700):
        m.box(CONC, (x, 0, 330), (220, 700, 660))
        m.box('#b0a088', (x, 0, 680), (300, 760, 40))
    m.box(CONC, (-1650, 0, 660), (1300, 660, 70))
    m.box(CONC, (-260, 0, 500), (1200, 660, 70), rot=(-17, 0, 0))
    m.box(CONC, (1500, 60, 60), (1100, 640, 70), rot=(4, 9, 3))
    for k in range(5):
        tilt_box(m, RUST, (310, -260 + k * 130, 330), (430 + 20 * k, -260 + k * 130, 250 - 30 * (k % 2)), 5)
    for s in (-1, 1):
        m.box(WHITE, (-1650, s * 320, 760), (1300, 14, 14))
        for k in range(5):
            m.box(WHITE, (-2250 + k * 300, s * 320, 725), (12, 12, 60))
    # the barricade and its barrels
    for x in (-2500, -2380):
        tilt_box(m, WHITE, (x, -260, 0), (x, -160, 130), 10)
        tilt_box(m, WHITE, (x, -60, 0), (x, -160, 130), 10)
    for k in range(6):
        m.box(FLAME if k % 2 else WHITE, (-2440, -260 + k * 40 + 20, 110), (150, 40, 22))
    for (x, y) in ((-2300, 140), (-2440, 230), (-2200, 300)):
        drum(m, x, y, 0, FLAME)


# ------------------------------------------------------------------------------------------ the land itself
def rock_wall(m, seed=0, L=6200, D=1700, H=3600):
    """A length of canyon wall: banded cliff stepping back as it climbs, buttresses, talus at its foot."""
    r = random.Random(seed + 31)
    n = 9
    front = [(-L / 2.0 + L * k / (n - 1), -D / 2.0 + r.uniform(0, 420)) for k in range(n)]
    cols = (SANDST_D, ROCK_R, SANDST, SANDST_L, SANDST, ROCK_R, SANDST_L)
    z = 0.0
    for k, col in enumerate(cols):
        h = H * (0.2, 0.1, 0.17, 0.12, 0.16, 0.1, 0.15)[k]
        back = D / 2.0
        pts = [(x * (1.0 - 0.012 * k), y + k * 70 + r.uniform(-40, 40)) for (x, y) in front] + [(L / 2.0 * (1.0 - 0.012 * k), back), (-L / 2.0 * (1.0 - 0.012 * k), back)]
        m.plan(col, pts, z, z + h)
        z += h
    for x in (-L * 0.3, L * 0.18):
        m.plan(SANDST, [(x - 300, -D / 2.0 + 300), (x, -D / 2.0 - 380), (x + 320, -D / 2.0 + 300)], 0, H * r.uniform(0.45, 0.7))
    for k in range(8):
        rr = r.uniform(90, 220)
        m.sphere(SANDST_D if k % 2 else SANDST, (r.uniform(-L * 0.46, L * 0.46), -D / 2.0 - r.uniform(40, 360), rr * 0.4), rr, scale=(1.2, 1, 0.65), steps=(5, 8), rot=(0, r.uniform(0, 180), 0))


def rock_arch(m):
    """A natural arch: two banded piers and the span between them."""
    for s, hh in ((-1, 1500), (1, 1350)):
        x = s * 950
        strata(m, [(x - 330, -300), (x + 300, -340), (x + 360, 300), (x - 280, 330)],
               [(0, hh * 0.3, SANDST_D), (hh * 0.3, hh * 0.62, SANDST), (hh * 0.62, hh * 0.85, ROCK_R), (hh * 0.85, hh, SANDST_L)], 0.08, x, 0)
    m.box(SANDST, (0, 0, 1560), (2500, 560, 330), rot=(0, 0, 0))
    m.box(SANDST_L, (-60, 0, 1760), (1900, 470, 150), rot=(-2, 0, 0))
    m.box(ROCK_R, (0, 0, 1380), (1500, 500, 90))
    m.sphere(SANDST_D, (1400, -200, 110), 240, scale=(1.2, 1, 0.6), steps=(5, 8))
    m.sphere(SANDST, (-1500, 160, 90), 190, scale=(1, 1.2, 0.6), steps=(5, 8))


def rock_spires(m, seed=0):
    """Hoodoos: thin banded pillars under cap stones, a handful at different heights."""
    r = random.Random(seed + 23)
    for k in range(6):
        x, y = r.uniform(-700, 700), r.uniform(-420, 420)
        h = r.uniform(700, 2300)
        rr = r.uniform(110, 190)
        z = 0.0
        for j, col in enumerate((SANDST_D, SANDST, ROCK_R, SANDST_L)):
            hh = h * (0.34, 0.26, 0.22, 0.18)[j]
            pz(m, col, x, y, z, z + hh, rr * (1.0 - 0.17 * j), steps=8, top_r=rr * (1.0 - 0.17 * (j + 1)))
            z += hh
        m.sphere(SANDST, (x, y, z + rr * 0.3), rr * 0.75, scale=(1.1, 1, 0.6), steps=(5, 8))


def ridge_dune(m, seed=0, L=5200, D=1500):
    """A long low swell of banded ground with sage along its back: it breaks the flat between the road and the mesas."""
    r = random.Random(seed + 3)
    out = [(-L / 2.0, 0), (-L * 0.3, -D * 0.5), (L * 0.1, -D * 0.42), (L / 2.0, -D * 0.1), (L * 0.38, D * 0.45), (-L * 0.2, D * 0.5)]
    strata(m, out, [(0, 150, SANDST_D), (150, 300, SANDST), (300, 420, SANDST_L), (420, 480, SAND)], 0.13)
    for k in range(7):
        m.sphere(SAGE if k % 2 else SAGE_D, (r.uniform(-L * 0.3, L * 0.3), r.uniform(-D * 0.2, D * 0.2), 500), r.uniform(50, 90), scale=(1, 1, 0.7), steps=(5, 8))


# ------------------------------------------------------------------------------------------ the horizon
TALUS = '#a04a34'     # the scree at a mesa's foot
NEAR_TONES = (TALUS, SANDST_D, ROCK_R, SANDST, SANDST_L, SANDST)
# the same rock a few miles further off: each tone a third of the way to the haze (pink read as a missing texture)
FAR_TONES = ('#b0705c', '#c9785e', '#d47f60', '#e38e68', '#f0a578', '#e38e68')


def butte_far(m, seed=0, W=24000, D=11000, H=9000, tones=NEAR_TONES):
    """A great mesa: a talus skirt, banded cliffs, a flat top, a pinnacle standing off one end. Built from z = 0."""
    r = random.Random(seed + 41)
    skirt, t0, t1, t2, t3, t4 = tones
    out = ring(r, 0, 0, W / 2.0, D / 2.0, 11, 0.14)
    strata(m, [(x * 1.16, y * 1.2) for (x, y) in out], [(0, H * 0.2, skirt)], 0.0)
    strata(m, out, [(H * 0.0, H * 0.36, t0), (H * 0.36, H * 0.55, t1), (H * 0.55, H * 0.74, t2), (H * 0.74, H * 0.9, t3), (H * 0.9, H, t4)], 0.045)
    px_, hh = W * 0.62, H * 0.82
    strata(m, ring(r, px_, 600, 1500, 1300, 7, 0.12), [(0, hh * 0.3, skirt), (0, hh * 0.6, t0), (hh * 0.6, hh * 0.86, t2), (hh * 0.86, hh, t3)], 0.12, px_, 600)


def butte_spires(m, seed=0):
    """Monument country: three buttes of different build, one a needle. Built from z = 0, to 150 m."""
    r = random.Random(seed + 57)
    for (x, y, w, d, h) in ((-9500, 500, 5200, 4200, 12500), (300, -300, 7600, 5200, 8600), (9000, 900, 3400, 3000, 15000)):
        strata(m, ring(r, x, y, w * 0.86, d * 0.86, 9, 0.12), [(0, h * 0.24, TALUS)], 0.0, x, y)
        strata(m, ring(r, x, y, w / 2.0, d / 2.0, 8, 0.13), [(0, h * 0.42, SANDST_D), (h * 0.42, h * 0.63, ROCK_R), (h * 0.63, h * 0.84, SANDST),
                                                              (h * 0.84, h, SANDST_L)], 0.07, x, y)
    strata(m, ring(r, 4300, 1300, 700, 650, 6, 0.1), [(0, 5200, SANDST_D), (5200, 8200, SANDST), (8200, 9400, SANDST_L)], 0.2, 4300, 1300)


def peak_range(m, seed=0, W=64000):
    """A far range: two ridgelines one behind the other, the back one paler. Built from z = 0, to 120 m."""
    r = random.Random(seed + 71)
    for layer, (y0, y1, hmax, col, top) in enumerate(((0, 2600, 8200, DUSK_D, DUSK), (2600, 5600, 12000, DUSK, DUSK_L))):
        n = 13
        pts = [(-W / 2.0, 0)]
        for k in range(n):
            x = -W / 2.0 + W * (k + 0.5) / n
            edge = math.sin(math.pi * (k + 0.5) / n) ** 0.6
            pts.append((x + r.uniform(-900, 900), hmax * edge * r.uniform(0.55, 1.0)))
        pts.append((W / 2.0, 0))
        m.side(col, pts, y0, y1)
        # the light catches the crest: a thin paler cap along the same line
        m.side(top, [(x, z - 420) for (x, z) in pts[1:-1]] + [(x, z) for (x, z) in reversed(pts[1:-1])], y0 - 40, y0)


def far_town(m, seed=0):
    """A neon town a few miles off: dark slabs with lit floors and neon crowns, a needle with a saucer. To 110 m."""
    r = random.Random(seed + 29)
    neon = [MAG, CYAN, GOLD, '#ff7a3d', '#b44dff']
    x = -9800
    while x < 8800:
        w, h = r.choice([1500, 2000, 2600]), r.choice([2600, 3800, 5200, 6800, 8600])
        y = r.choice([0, 900, 1900])
        col = r.choice(['#2b2350', '#3a2a6a', '#24204a', '#4a2a6a'])
        nc = r.choice(neon)
        m.box(col, (x + w / 2.0, y + 800, h / 2.0), (w, 1600, h))
        z = 600
        while z < h - 500:
            m.box(r.choice(['#ffe9a8', '#9ff0ff', '#ffc0e8']), (x + w / 2.0, y - 4, z), (w * 0.74, 8, 150), glow=0.7)
            z += 620
        m.box(nc, (x + w / 2.0, y - 6, h - 150), (w + 30, 10, 200), glow=1.4)
        x += w + r.choice([300, 700, 1200])
    pz(m, '#3a2a6a', 9600, 600, 0, 9500, 260, steps=10, top_r=120)
    m.sphere(CYAN, (9600, 600, 9300), 800, scale=(1, 1, 0.3), steps=(6, 12), glow=0.9)
    pz(m, STEEL, 9600, 600, 9500, 11200, 30, steps=6)
    m.sphere(RED, (9600, 600, 11260), 110, steps=(4, 6), glow=1.4)


JOBS = [
    ('SM_WireFence', buried(wire_fence)), ('SM_WireLine', buried(wire_line)), ('SM_Sagebrush', buried(sagebrush)),
    ('SM_Sagebrush2', buried(lambda m: sagebrush(m, 9))), ('SM_JunkPile', buried(junk_pile)), ('SM_WreckCar', buried(wreck_car)),
    ('SM_RockBank', buried(rock_bank)), ('SM_RockBank2', buried(lambda m: rock_bank(m, 8, 2100, 700))), ('SM_SignFrame', buried(sign_frame)),
    ('SM_MileSign', buried(mile_sign)), ('SM_WashBed', buried(wash_bed)),
    ('SM_Diner', buried(diner)), ('SM_Garage', buried(garage)), ('SM_WaterTank', buried(water_tank)), ('SM_Trailer', buried(trailer)),
    ('SM_Roadhouse', buried(roadhouse)), ('SM_BikerCamp', buried(biker_camp)), ('SM_PowerSpan', piled(power_span)),
    ('SM_Hangar', buried(hangar)), ('SM_RadarPost', buried(radar_post)), ('SM_Casino', buried(casino)), ('SM_Chapel', buried(chapel)),
    ('SM_BridgeRuin', buried(bridge_ruin)),
    ('SM_RockWall', buried(rock_wall)), ('SM_RockWall2', buried(lambda m: rock_wall(m, 5, 5200, 1500, 2700))), ('SM_RockArch', buried(rock_arch)),
    ('SM_RockSpires', buried(rock_spires)), ('SM_RockSpires2', buried(lambda m: rock_spires(m, 6))), ('SM_RidgeDune', buried(ridge_dune)),
    # the horizon stands past the land's edge, below it: its footing shows from a high camera, so it is the rock's own
    # colour (a bench under the mesa), and 30 m wider than the piece all round (see ft_dress_chem.buried, keep)
    ('SM_ButteFar', buried(butte_far, SANDST_D, FAR_KEEP)),
    ('SM_ButteFar2', buried(lambda m: butte_far(m, 9, 17000, 9000, 11500, FAR_TONES), FAR_TONES[1], FAR_KEEP)),
    ('SM_ButteSpires', buried(butte_spires, SANDST_D, FAR_KEEP)), ('SM_PeakRange', buried(peak_range, DUSK_D, FAR_KEEP)),
    ('SM_FarTown', buried(far_town, '#24204a', FAR_KEEP)),
]
