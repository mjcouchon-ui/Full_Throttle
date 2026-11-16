"""Theme Park, Stage 1, builder s1d7: "The Last Ride: Snowglobe Square" (16.1 to 20.5 km), a winter fair.
/Game/World/ThemePark/SM_S1D7_*.   Run: import ft_world_park_s1d7 as K; K.run()

Low wide chalets under thick snow roofs that hang over like duvets, icicle fringes, round frosted windows, ice-block
walls and arcades. The palette is the kit's (ft_dress_park.py): WHITE snow, SPRAY ice, POOL blue timber and shadow,
CYAN lit windows and ice edges, TIN metalwork, RED for doors and scarves only; NAVY for coal and a dark hatch, TIN_D
for a clapper and a broom, PINE for the one fir inside the globe. All of it is our own: no word, no mark, no likeness.

Every piece is an ft_people.Scene (one section, M_People / M_PeopleFar). Front faces -Y, base on z = 0, cm.
Glow is used only on pieces the table stands 40 m and more from the centre line; the four LOW pieces have none.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

WHITE, SPRAY, POOL, CYAN, TIN, RED = '#fff6ea', '#dff6ff', '#3a5fa8', '#2ee6ff', '#a8b4d0', '#ff2d3c'
TIN_D, NAVY, PINE = '#7f8aa6', '#2b2350', '#2f6b52'
SG = 0.07      # the snow's own steady glow (a night level; the park's cast carries 0.05 to 0.1)
WG = 0.9       # a lit window
LG = 1.3       # a lamp
F = 150.0      # a building's footing under z = 0


# ------------------------------------------------------------------------------------------ shared details
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def fringe(g, x0, x1, y, z, drop=70.0, tooth=75.0, seed=1, col=SPRAY, t=12.0):
    """An icicle fringe hanging from z along X at depth y: one saw-toothed plate."""
    r = random.Random(seed)
    n = max(2, int((x1 - x0) / tooth))
    w = (x1 - x0) / float(n)
    pts = [(x0, z), (x0, z - 8.0)]
    for k in range(n):
        pts.append((x0 + (k + 0.5) * w, z - 8.0 - drop * r.uniform(0.35, 1.0)))
        pts.append((x0 + (k + 1) * w, z - 8.0))
    pts.append((x1, z))
    g.side(col, ccw(pts), y - t / 2.0, y + t / 2.0)


def icicle(g, x, y, z, length, rad=11.0):
    g.cyl(SPRAY, (x, y, z), rad, length, rot=(180, 0, 0), steps=4, top_r=3.0)


def flake(m, x, y, z, r, glow=0.8, col=CYAN):
    """A snowflake standing in the x-z plane: three crossed bars."""
    for k in range(3):
        m.box(col, (x, y, z), (2.0 * r, 6.0, max(r * 0.2, 6.0)), rot=(k * 60.0, 0, 0), glow=glow)


def porthole(g, x, y, z, r, glow=WG, frame=TIN):
    """A round frosted window on a wall whose face is at depth y (it looks to -Y), snow on its sill."""
    rod(g, frame, (x, y + 6, z), (x, y - 10, z), r + 12, steps=8)
    rod(g, CYAN, (x, y, z), (x, y - 14, z), r, steps=8, glow=glow)
    g.box(frame, (x, y - 16, z), (2 * r, 5, 7))
    g.box(frame, (x, y - 16, z), (7, 5, 2 * r))
    g.box(WHITE, (x, y - 14, z - r - 14), (2 * r + 34, 28, 16))


def door(g, x, y, z=0.0, w=130.0, h=240.0):
    """The district's red door in an ice frame, a snowed step before it."""
    g.box(SPRAY, (x, y - 4, z + h / 2.0 + 8), (w + 40, 10, h + 34))
    g.box(RED, (x, y - 9, z + h / 2.0), (w, 10, h))
    g.box(TIN, (x + w * 0.3, y - 16, z + h * 0.48), (10, 8, 24))
    g.box(WHITE, (x, y - 42, z + 4), (w + 80, 84, 26))


def lantern(g, x, y, z, s=1.0, glow=LG):
    g.box(TIN, (x, y, z - 3 * s), (32 * s, 32 * s, 6 * s))
    g.box(CYAN, (x, y, z + 15 * s), (25 * s, 25 * s, 30 * s), glow=glow)
    g.box(TIN, (x, y, z + 33 * s), (36 * s, 36 * s, 7 * s))
    g.box(WHITE, (x, y, z + 42 * s), (44 * s, 44 * s, 12 * s))


def eave_roof(g, x, y, z, w, d, rise, over=85.0, t=85.0, seed=1, tooth=75.0, drop=70.0):
    """A snow duvet on a roof whose ridge runs along X: two fat slabs, rolled eaves and ridge, icicles along the front."""
    hd = d / 2.0 + over
    low = z - over * rise / (d / 2.0)
    for sgn in (-1, 1):
        tilt_box(g, WHITE, (x, y + sgn * hd, low + t / 2.0), (x, y, z + rise + t / 2.0), w + 2 * over, t, glow=SG)
        rod(g, WHITE, (x - w / 2.0 - over, y + sgn * hd, low + t * 0.45), (x + w / 2.0 + over, y + sgn * hd, low + t * 0.45), t * 0.62, steps=6, glow=SG)
    rod(g, WHITE, (x - w / 2.0 - over, y, z + rise + t * 0.55), (x + w / 2.0 + over, y, z + rise + t * 0.55), t * 0.6, steps=6, glow=SG)
    g.box(POOL, (x, y, z + rise * 0.25), (w, d * 0.5, rise * 0.5))
    fringe(g, x - w / 2.0 - over, x + w / 2.0 + over, y - hd - 4.0, low + 8.0, drop=drop, tooth=tooth, seed=seed)


def gable_roof(g, x, y0, y1, z, w, rise, over=90.0, t=80.0):
    """A snow duvet on a roof whose ridge runs away from the road: the timber gable faces the road under it."""
    hw = w / 2.0
    s = rise / hw
    zb = z - over * s
    g.side(POOL, ccw([(x - hw, z), (x + hw, z), (x, z + rise)]), y0, y1)
    pts = [(x - hw - over, zb), (x, z + rise), (x + hw + over, zb), (x + hw + over + 16, zb + t * 0.7),
           (x + hw + over - 50, zb + t + 50 * s + 8), (x, z + rise + t + 16), (x - hw - over + 50, zb + t + 50 * s + 8),
           (x - hw - over - 16, zb + t * 0.7)]
    g.side(WHITE, ccw(pts), y0 - over, y1 + over * 0.5, glow=SG)
    for sgn in (-1, 1):                        # icicles down both verges of the gable
        for k, u in enumerate((0.08, 0.3, 0.52, 0.76)):
            px = x + sgn * (hw + over) * (1.0 - u)
            pz = zb + (z + rise - zb) * u
            icicle(g, px, y0 - over + 10.0, pz + 6.0, 38.0 + 22.0 * ((k * 3 + (1 if sgn > 0 else 0)) % 4), 12.0)


def chimney(g, x, y, z, h=280.0, w=86.0):
    g.box(POOL, (x, y, z + h / 2.0), (w, w, h))
    g.box(WHITE, (x, y, z + h + 10), (w + 28, w + 28, 26), glow=SG)
    g.box(TIN, (x, y, z + h + 34), (w * 0.5, w * 0.5, 26))


def skate(g, x, y, z, s=1.0, boot=WHITE, glow=0.0):
    """A skate, its blade along X (the hire's wares and its sign: a picture, no word)."""
    g.box(TIN, (x, y, z + 4 * s), (74 * s, 6 * s, 8 * s))
    g.box(TIN, (x - 18 * s, y, z + 12 * s), (8 * s, 6 * s, 10 * s))
    g.box(TIN, (x + 20 * s, y, z + 12 * s), (8 * s, 6 * s, 10 * s))
    g.box(boot, (x + 6 * s, y, z + 24 * s), (58 * s, 18 * s, 16 * s), glow=glow)
    g.box(boot, (x - 12 * s, y, z + 46 * s), (24 * s, 18 * s, 34 * s), glow=glow)
    g.box(CYAN, (x - 12 * s, y, z + 62 * s), (27 * s, 20 * s, 7 * s))


# ------------------------------------------------------------------------------------------ BUILDINGS
def skate_hire(sc):
    """BUILDING 1: the skate-hire chalet. 15 m long, one storey, its eave to the road; a giant skate on the ridge."""
    g = sc.still()
    W, D, H = 1500.0, 620.0, 430.0
    fy = -D / 2.0
    g.box(POOL, (0, 0, (H - F) / 2.0), (W, D, H + F))
    for x in (-W / 2.0 + 14, W / 2.0 - 14):
        g.box(TIN, (x, fy - 6, H / 2.0), (40, 30, H))
    g.box(TIN, (0, fy - 4, 118), (W - 70, 10, 12))
    # the hire hatch: a lit opening, a counter of skates under its own snowed awning
    g.box(NAVY, (-150, fy - 4, 258), (640, 10, 196))
    g.box(CYAN, (-150, fy - 8, 268), (590, 6, 150), glow=WG * 0.6)
    g.box(TIN, (-150, fy - 36, 152), (700, 76, 14))
    for k in range(5):
        skate(g, -390 + k * 120, fy - 40, 159, 0.9, WHITE if k % 2 == 0 else SPRAY)
    for x in (-490, 190):
        g.box(TIN, (x, fy - 70, 76), (12, 12, 152))
    tilt_box(g, TIN, (-150, fy - 4, 384), (-150, fy - 150, 340), 720, 12)
    tilt_box(g, WHITE, (-150, fy - 2, 402), (-150, fy - 158, 356), 744, 26, glow=SG)
    fringe(g, -522, 222, fy - 160, 346, drop=44, tooth=62, seed=3)
    door(g, 430, fy, 0)
    porthole(g, 630, fy, 280, 50)
    porthole(g, -630, fy, 280, 50)
    lantern(g, 330, fy - 30, 300, 0.8)
    g.box(TIN, (330, fy - 16, 336), (8, 34, 8))
    # a bench under the left window, skis leant by the door
    g.box(POOL, (-630, fy - 52, 46), (190, 52, 10))
    for x in (-710, -550):
        g.box(POOL, (x, fy - 52, 20), (12, 44, 42))
    g.box(WHITE, (-630, fy - 52, 56), (170, 44, 10), glow=SG)
    for k, c in enumerate((CYAN, WHITE, SPRAY)):
        tilt_box(g, c, (540 + k * 26, fy - 64, 0), (548 + k * 26, fy - 8, 215), 13, 5)
    rod(g, WHITE, (-W / 2.0 - 50, fy - 24, 6), (300, fy - 24, 6), 44, steps=6, glow=SG)
    rod(g, WHITE, (520, fy - 24, 6), (W / 2.0 + 50, fy - 24, 6), 44, steps=6, glow=SG)
    eave_roof(g, 0, 0, H, W, D, 230, over=88, t=88, seed=2)
    # the sign on the ridge: one skate 3.7 m long
    top = H + 230 + 88 * 1.1
    for x in (-110, 150):
        g.box(TIN, (x, -30, top + 30), (14, 14, 90))
    skate(g, 0, -30, top + 60, 5.0, WHITE, glow=0.12)
    g.cyl(TIN, (560, 110, H + 120), 24, 250, steps=6)
    g.cyl(WHITE, (560, 110, H + 370), 38, 22, steps=6, glow=SG)


def ice_arcade(sc):
    """BUILDING 2: the ice-block arcade, five arches of clear blocks before a blue hall, lanterns in three of the arches."""
    g = sc.still()
    W, D, H = 1560.0, 560.0, 470.0
    fy = -D / 2.0
    py = fy + 75.0
    g.box(POOL, (0, 110, (H - F) / 2.0), (W, 340, H + F))
    for k in range(6):
        x = -700 + k * 280
        g.box(SPRAY, (x, py, (H - F) / 2.0), (150, 150, H + F), glow=0.06)
        g.box(CYAN, (x, py, 226), (156, 156, 7), glow=0.3)
    for k in range(5):
        cx = -560 + k * 280
        for j in range(5):
            a = math.radians(18 + j * 36)
            g.box(SPRAY, (cx + 95 * math.cos(a), py, 300 + 95 * math.sin(a)), (62, 150, 50), rot=(18 + j * 36 - 90, 0, 0), glow=0.06)
        porthole(g, cx, -60, 240, 46)
        if k % 2 == 0:
            lantern(g, cx, py, 286, 0.8)
            g.box(TIN, (cx, py, 345), (6, 6, 46))
    g.box(SPRAY, (0, py, 432), (W, 150, 76), glow=0.06)
    g.box(SPRAY, (0, fy - 30, 8), (W + 60, 130, 24))
    # the roof: a flat thick duvet, its front rolled, icicles all along; a stepped pediment of blocks with a snowflake
    g.box(WHITE, (0, 0, H + 45), (W + 120, D + 140, 90), glow=SG)
    rod(g, WHITE, (-W / 2.0 - 60, fy - 70, H + 50), (W / 2.0 + 60, fy - 70, H + 50), 62, steps=6, glow=SG)
    fringe(g, -W / 2.0 - 60, W / 2.0 + 60, fy - 76, H + 4, drop=72, tooth=78, seed=5)
    g.box(SPRAY, (0, fy + 40, H + 150), (380, 110, 120), glow=0.06)
    g.box(SPRAY, (0, fy + 40, H + 250), (210, 110, 90), glow=0.06)
    g.box(WHITE, (0, fy + 40, H + 305), (240, 130, 24), glow=SG)
    flake(g, 0, fy - 18, H + 160, 52)
    for sx in (-1, 1):
        g.sphere(WHITE, (sx * 420, 40, H + 90), 150, scale=(1.5, 1.0, 0.45), steps=(4, 8), glow=SG)
        g.cyl(SPRAY, (sx * (W / 2.0 - 30), fy + 40, H + 88), 46, 210, steps=6, top_r=5, glow=0.1)


def choc_hut(sc):
    """BUILDING 3: the hot-chocolate hut: eight-sided, a mushroom cap of snow, a fat chimney that puffs steam."""
    g = sc.still()
    R, H = 330.0, 370.0
    ap = R * math.cos(math.radians(22.5))                 # the flat front wall stands at y = -ap
    g.cyl(POOL, (0, 0, -F), R, H + F, rot=(0, 22.5, 0), steps=8)
    g.cyl(TIN, (0, 0, 104), R + 8, 12, rot=(0, 22.5, 0), steps=8)
    # the serving window on the front, a shelf of mugs, shutters folded back
    g.box(NAVY, (0, -ap - 3, 250), (200, 8, 150))
    g.box(CYAN, (0, -ap - 6, 256), (176, 6, 120), glow=WG)
    g.box(TIN, (0, -ap - 30, 168), (250, 64, 12))
    for sx in (-1, 1):
        g.box(POOL, (sx * 122, -ap - 8, 250), (40, 10, 156))
        g.box(WHITE, (sx * 122, -ap - 12, 250), (16, 5, 16))
        g.cyl(WHITE, (sx * 80, -ap - 36, 174), 20, 40, steps=6)
        g.box(WHITE, (sx * 80 + 24, -ap - 36, 194), (10, 8, 22))
    g.cyl(CYAN, (0, -ap - 36, 174), 20, 40, steps=6)
    c45 = math.cos(math.radians(45.0))
    # the red door on the right-hand face, a square lit pane on the left-hand one
    g.box(SPRAY, ((ap + 3) * c45, -(ap + 3) * c45, 128), (160, 10, 270), rot=(0, 45, 0))
    g.box(RED, ((ap + 8) * c45, -(ap + 8) * c45, 120), (124, 10, 240), rot=(0, 45, 0))
    g.box(WHITE, ((ap + 40) * c45, -(ap + 40) * c45, 4), (200, 84, 26), rot=(0, 45, 0))
    g.box(TIN, (-(ap + 3) * c45, -(ap + 3) * c45, 250), (140, 8, 140), rot=(0, -45, 0))
    g.box(CYAN, (-(ap + 7) * c45, -(ap + 7) * c45, 250), (112, 8, 112), rot=(0, -45, 0), glow=WG)
    g.box(WHITE, (-(ap + 14) * c45, -(ap + 14) * c45, 174), (150, 26, 16), rot=(0, -45, 0))
    # the cap of snow and its icicles
    g.cyl(WHITE, (0, 0, H - 34), 440, 96, steps=10, glow=SG)
    g.cyl(WHITE, (0, 0, H + 62), 424, 200, steps=10, top_r=216, glow=SG)
    g.cyl(WHITE, (0, 0, H + 262), 216, 110, steps=10, top_r=56, glow=SG)
    for k in range(9):
        a = math.radians(-170 + k * 20)
        icicle(g, 432 * math.cos(a), 432 * math.sin(a), H - 30, 46 + 30 * ((k * 5) % 3), 13)
    # the fat chimney and its steam (three puffs that float)
    cx, cy = 170.0, 120.0
    g.cyl(POOL, (cx, cy, H), 104, 430, steps=8, top_r=90)
    g.cyl(TIN, (cx, cy, H + 300), 100, 14, steps=8)
    g.cyl(WHITE, (cx, cy, H + 430), 124, 46, steps=8, glow=SG)
    for k in range(3):
        st = sc.part(P.motion(bob=-48.0, rate=0.2, phase=k * 0.33))
        st.sphere(WHITE, (cx + 26 * (k - 1), cy, H + 560 + k * 118), 62 + k * 16, steps=(4, 6), glow=0.12)
    # the sign on the eave: one great mug (a picture, no word), its own wisp floating
    mz = H + 64.0
    g.cyl(WHITE, (-170, -330, mz), 86, 160, steps=8, glow=0.12)
    g.cyl(CYAN, (-170, -330, mz + 62), 89, 30, steps=8, glow=0.3)
    g.cyl(NAVY, (-170, -330, mz + 158), 72, 6, steps=8)
    g.box(WHITE, (-290, -330, mz + 120), (60, 22, 20), glow=0.12)
    g.box(WHITE, (-312, -330, mz + 82), (20, 22, 90), glow=0.12)
    g.box(WHITE, (-290, -330, mz + 44), (60, 22, 20), glow=0.12)
    wp = sc.part(P.motion(bob=-30.0, rate=0.25, phase=0.6))
    wp.sphere(WHITE, (-170, -330, mz + 236), 40, steps=(4, 6), glow=0.12)
    wp.sphere(WHITE, (-146, -330, mz + 310), 28, steps=(3, 6), glow=0.12)
    # two ice stools and a picture board on a post
    for x in (-300, -390):
        g.cyl(SPRAY, (x, -ap - 110, 0), 30, 46, steps=6)
        g.cyl(WHITE, (x, -ap - 110, 46), 34, 10, steps=6)
    g.box(TIN, (330, -ap - 96, 70), (10, 10, 140))
    g.box(TIN, (330, -ap - 100, 180), (96, 8, 110))
    for k in range(3):
        g.box(WHITE, (302 + k * 28, -ap - 105, 196), (18, 5, 22))
        g.box(CYAN, (302 + k * 28, -ap - 105, 160), (18, 5, 12))
    g.box(WHITE, (330, -ap - 100, 240), (112, 20, 14))


def toboggan_tower(sc):
    """BUILDING 4: the toboggan tower: a blue shaft, a snow-capped look-out, an ice chute down to a snow bank."""
    g = sc.still()
    tx, ty, TW, TH = -470.0, 40.0, 360.0, 980.0
    fy = ty - TW / 2.0
    g.box(POOL, (tx, ty, (TH - F) / 2.0), (TW, TW, TH + F))
    for z in (330, 660):
        g.box(WHITE, (tx, ty, z), (TW + 56, TW + 56, 24), glow=SG)
        fringe(g, tx - TW / 2.0 - 28, tx + TW / 2.0 + 28, fy - 26, z - 10, drop=36, tooth=60, seed=int(z))
    door(g, tx, fy, 0)
    porthole(g, tx, fy, 500, 54)
    porthole(g, tx, fy, 820, 44)
    # the deck, its rail, the canopy
    g.box(TIN, (tx, ty, TH), (TW + 220, TW + 220, 26))
    hr = TW / 2.0 + 100
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(POOL, (tx + sx * hr, ty + sy * hr, TH + 160), (22, 22, 320))
        g.box(POOL, (tx + sx * hr, ty, TH + 100), (14, 2 * hr, 14))
        g.box(WHITE, (tx + sx * hr, ty, TH + 112), (22, 2 * hr, 12), glow=SG)
    g.box(POOL, (tx, ty - hr, TH + 100), (2 * hr, 14, 14))
    g.box(WHITE, (tx, ty - hr, TH + 112), (2 * hr, 22, 12), glow=SG)
    g.box(WHITE, (tx, ty, TH + 330), (2 * hr + 110, 2 * hr + 110, 50), glow=SG)
    fringe(g, tx - hr - 55, tx + hr + 55, ty - hr - 58, TH + 306, drop=58, tooth=78, seed=8)
    g.cyl(WHITE, (tx, ty, TH + 350), 440, 380, rot=(0, 45, 0), steps=4, top_r=18, glow=SG)
    g.cyl(TIN, (tx, ty, TH + 720), 7, 170, steps=6)
    lantern(g, tx, ty - hr + 30, TH + 250, 1.0)
    fl = sc.part(P.motion(swing=14.0, pivot=(tx, ty, TH + 850), axis=P.UP, rate=0.32, phase=0.1))
    fl.box(CYAN, (tx + 74, ty, TH + 850), (136, 5, 58), glow=0.3)
    # the chute: an ice trough between snowed walls, on three trestles, a sledge waiting half way
    a = (tx + TW / 2.0 + 90, -70.0, TH + 20)
    b = (560.0, -70.0, 60.0)
    tilt_box(g, SPRAY, a, b, 170, 26, glow=0.1)
    for sy in (-1, 1):
        tilt_box(g, WHITE, (a[0], a[1] + sy * 92, a[2] + 34), (b[0], b[1] + sy * 92, b[2] + 34), 24, 70, glow=SG)
    for x in (-60.0, 180.0, 400.0):
        u = (x - a[0]) / (b[0] - a[0])
        z = a[2] + (b[2] - a[2]) * u - 26
        g.box(POOL, (x, -70, (z - F) / 2.0), (34, 150, z + F))
        g.box(TIN, (x, -70, z * 0.55), (40, 190, 12))
    um = 0.45
    mx, mz = a[0] + (b[0] - a[0]) * um, a[2] + (b[2] - a[2]) * um
    pitch = math.degrees(math.atan2(b[2] - a[2], b[0] - a[0]))
    g.box(POOL, (mx, -70, mz + 34), (120, 74, 12), rot=(pitch, 0, 0))
    g.box(WHITE, (mx - 10, -70, mz + 58), (60, 56, 30), rot=(pitch, 0, 0))
    g.box(SPRAY, (690, -70, 30), (280, 170, 26), glow=0.1)
    g.sphere(WHITE, (850, -70, 14), 130, scale=(1.0, 1.1, 0.6), steps=(4, 8), glow=SG)
    # a queue rail at the tower's foot
    for x in (tx + 240, tx + 400):
        g.box(POOL, (x, fy - 60, 50), (16, 16, 100))
    g.box(POOL, (tx + 320, fy - 60, 96), (190, 12, 12))
    g.box(WHITE, (tx + 320, fy - 60, 106), (190, 18, 9), glow=SG)


def bell_chalet(sc):
    """BUILDING 5: the bell chalet: two storeys, a balcony with a washing line of scarves, a snow-capped belfry
    whose bell swings."""
    g = sc.still()
    W, D, H1, H2 = 1040.0, 620.0, 330.0, 300.0
    fy = -D / 2.0
    g.box(WHITE, (0, 0, (H1 - F) / 2.0), (W, D, H1 + F), glow=0.03)
    g.box(POOL, (0, 0, H1 + H2 / 2.0), (W + 30, D + 30, H2))
    for x in (-W / 2.0, W / 2.0):
        g.box(POOL, (x, fy - 4, H1 / 2.0), (46, 22, H1))
    g.box(POOL, (0, fy - 4, 20), (W, 14, 40))
    # the balcony
    by = fy - 170.0
    g.box(TIN, (0, fy - 88, H1 + 6), (W + 60, 176, 16))
    g.box(POOL, (0, by, H1 + 88), (W + 60, 16, 14))
    g.box(WHITE, (0, by, H1 + 104), (W + 70, 30, 22), glow=SG)
    for k in range(7):
        g.box(POOL, (-W / 2.0 - 20 + k * (W + 40) / 6.0, by, H1 + 48), (16, 16, 82))
    for x in (-380, 0, 380):
        tilt_box(g, POOL, (x, fy, H1 - 120), (x, fy - 150, H1 - 4), 18, 18)
    fringe(g, -W / 2.0 - 30, W / 2.0 + 30, by - 2, H1 - 2, drop=50, tooth=70, seed=5)
    # ground floor: the door between two shuttered windows with snowed sills
    door(g, 0, fy, 0)
    for x in (-330, 330):
        g.box(TIN, (x, fy - 3, 190), (150, 8, 150))
        g.box(CYAN, (x, fy - 6, 190), (120, 8, 120), glow=WG)
        g.box(TIN, (x, fy - 9, 190), (7, 6, 120))
        for sx in (-1, 1):
            g.box(POOL, (x + sx * 98, fy - 7, 190), (44, 10, 150))
            g.box(WHITE, (x + sx * 98, fy - 11, 196), (14, 5, 14))
        g.box(WHITE, (x, fy - 16, 104), (200, 34, 18), glow=SG)
    # upper floor: a lit balcony door, two round windows
    g.box(TIN, (0, fy - 16, H1 + 122), (130, 8, 226))
    g.box(CYAN, (0, fy - 19, H1 + 122), (104, 8, 200), glow=WG)
    for x in (-330, 330):
        porthole(g, x, fy - 15, H1 + 160, 46)
    zt = H1 + H2
    gable_roof(g, 0, fy - 15, D / 2.0 + 15, zt, W + 30, 330, over=90, t=80)
    porthole(g, 0, fy - 15, zt + 118, 40)
    chimney(g, 310, 150, zt + 60, 330)
    # the belfry on the ridge
    rz = zt + 330 + 70.0
    cy = fy + 150.0
    g.box(POOL, (0, cy, rz + 6), (210, 210, 50))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(POOL, (sx * 76, cy + sy * 76, rz + 130), (22, 22, 220))
    g.box(WHITE, (0, cy, rz + 250), (300, 300, 34), glow=SG)
    fringe(g, -150, 150, cy - 152, rz + 236, drop=34, tooth=60, seed=12)
    g.cyl(WHITE, (0, cy, rz + 262), 200, 230, rot=(0, 45, 0), steps=4, top_r=14, glow=SG)
    g.cyl(TIN, (0, cy, rz + 486), 6, 70, steps=6)
    flake(g, 0, cy, rz + 590, 36)
    bell = sc.part(P.motion(swing=20.0, pivot=(0, cy, rz + 226), axis=90.0, rate=0.45))
    bell.box(TIN, (0, cy, rz + 205), (12, 12, 44))
    bell.cyl(TIN, (0, cy, rz + 96), 64, 96, steps=8, top_r=30)
    bell.cyl(TIN, (0, cy, rz + 84), 72, 14, steps=8)
    bell.box(TIN_D, (0, cy, rz + 68), (16, 16, 32))
    # the washing line along the balcony: scarves and mittens that stir
    for x in (-W / 2.0 - 20, W / 2.0 + 20):
        g.box(POOL, (x, by, H1 + 170), (14, 14, 240))
    g.box(TIN, (0, by, H1 + 262), (W + 40, 5, 5))
    wl = sc.part(P.motion(swing=7.0, pivot=(0, by, H1 + 262), axis=0.0, rate=0.5, phase=0.3))
    for k, (c, h) in enumerate(((RED, 96), (WHITE, 50), (CYAN, 60), (RED, 110), (SPRAY, 46), (WHITE, 84), (CYAN, 52))):
        wl.box(c, (-390 + k * 130, by, H1 + 258 - h / 2.0), (36, 5, h))


def palace_gate(sc):
    """BUILDING 6: the ice palace gate: two round towers of clear blocks under snow spires, an arch of blocks between,
    a portcullis of icicles, the floor lit from below."""
    g = sc.still()
    for sx in (-1, 1):
        x = sx * 540.0
        g.cyl(SPRAY, (x, 0, -F), 190, 820 + F, steps=8, top_r=170, glow=0.08)
        for z in (200, 420, 640):
            g.cyl(CYAN, (x, 0, z), 190 - 20.0 * (z + F) / 970.0 + 5, 9, steps=8, glow=0.5)
        g.cyl(SPRAY, (x, 0, 820), 216, 70, steps=8, glow=0.08)
        for k in range(8):
            a = math.radians(k * 45 + 22.5)
            g.box(SPRAY, (x + 196 * math.cos(a), 196 * math.sin(a), 915), (60, 60, 50), rot=(0, k * 45 + 22.5, 0), glow=0.08)
        g.cyl(WHITE, (x, 0, 890), 180, 430, steps=8, top_r=10, glow=SG)
        g.cyl(TIN, (x, 0, 1310), 6, 150, steps=6)
        f = sc.part(P.motion(swing=16.0, pivot=(x, 0, 1420), axis=P.UP, rate=0.3, phase=0.2 + 0.2 * sx))
        f.box(CYAN, (x + 76, 0, 1420), (140, 5, 56), glow=0.3)
        porthole(g, x, -182, 540, 42)
        g.sphere(WHITE, (x + sx * 120, -150, 8), 150, scale=(1.3, 0.9, 0.45), steps=(4, 8), glow=SG)
    g.box(SPRAY, (0, 0, 640), (760, 220, 150), glow=0.08)
    for x in (-250, 0, 250):
        g.box(CYAN, (x, -111, 640), (8, 6, 150), glow=0.5)
    for j in range(7):
        ad = (j + 0.5) * 180.0 / 7.0
        a = math.radians(ad)
        g.box(SPRAY, (330 * math.cos(a), 0, 300 + 330 * math.sin(a)), (152, 200, 70), rot=(ad - 90, 0, 0), glow=0.08)
    for k in range(5):
        g.box(SPRAY, (-300 + k * 150, 0, 750), (90, 200, 70), glow=0.08)
        g.box(WHITE, (-300 + k * 150, 0, 792), (102, 212, 18), glow=SG)
    fringe(g, -200, 200, -50, 566, drop=170, tooth=74, seed=9, t=16)
    flake(g, 0, -116, 650, 58)
    g.box(CYAN, (0, 0, 6), (640, 300, 12), glow=0.6)
    g.box(WHITE, (0, -230, 2), (520, 180, 22), glow=SG)


def mitten_shop(sc):
    """BUILDING 7: the mitten shop: a small gabled chalet, a lit bay of woollens, a giant mitten swaying on a bracket,
    a snowflake vane on the ridge."""
    g = sc.still()
    W, D, H = 920.0, 620.0, 400.0
    fy = -D / 2.0
    g.box(POOL, (0, 0, (H - F) / 2.0), (W, D, H + F))
    for x in (-W / 2.0 + 12, W / 2.0 - 12):
        g.box(TIN, (x, fy - 5, H / 2.0), (36, 22, H))
    # the bay window and its wares
    g.box(TIN, (-170, fy - 40, 215), (400, 80, 250))
    g.box(CYAN, (-170, fy - 82, 220), (350, 6, 200), glow=WG)
    for x in (-286, -170, -54):
        g.box(TIN, (x, fy - 86, 220), (8, 6, 200))
    g.box(RED, (-240, fy - 87, 200), (22, 5, 110))
    g.box(WHITE, (-140, fy - 87, 176), (56, 5, 50))
    g.box(SPRAY, (-84, fy - 87, 250), (40, 5, 56))
    g.box(WHITE, (-170, fy - 46, 354), (430, 104, 30), glow=SG)
    fringe(g, -385, 45, fy - 98, 342, drop=40, tooth=58, seed=11)
    g.box(WHITE, (-170, fy - 44, 84), (420, 96, 18), glow=SG)
    door(g, 290, fy, 0)
    tilt_box(g, WHITE, (290, fy, 318), (290, fy - 120, 286), 230, 26, glow=SG)
    fringe(g, 176, 404, fy - 122, 276, drop=30, tooth=56, seed=13)
    lantern(g, 410, fy - 30, 190, 0.8)
    g.box(TIN, (410, fy - 16, 226), (8, 34, 8))
    gable_roof(g, 0, fy - 10, D / 2.0 + 10, H, W + 20, 380, over=95, t=85)
    porthole(g, 0, fy - 10, H + 150, 52)
    chimney(g, -260, 130, H + 150, 330)
    # the sign: a mitten 2.4 m tall on a bracket at the corner (a picture, no word)
    sx0 = -W / 2.0 - 150.0
    g.box(TIN, (-W / 2.0 - 110, fy - 30, H + 44), (270, 10, 10))
    tilt_box(g, TIN, (-W / 2.0 - 6, fy - 30, H - 60), (-W / 2.0 - 150, fy - 30, H + 40), 8, 8)
    mt = sc.part(P.motion(swing=6.0, pivot=(sx0, fy - 30, H + 40), axis=90.0, rate=0.3))
    mt.box(TIN, (sx0, fy - 30, H + 22), (8, 8, 36))
    mt.box(CYAN, (sx0, fy - 30, H - 20), (166, 66, 48), glow=0.3)
    mt.box(WHITE, (sx0, fy - 30, H - 136), (150, 58, 190), glow=0.12)
    mt.box(WHITE, (sx0 - 98, fy - 30, H - 110), (56, 52, 92), glow=0.12)
    flake(mt, sx0, fy - 62, H - 140, 46, glow=0.5, col=POOL)
    # the vane on the ridge: an arrow that turns to and fro under a snowflake
    zv = H + 380 + 85 + 150.0
    g.cyl(TIN, (0, 60, H + 380 + 60), 7, 250, steps=6)
    flake(g, 0, 60, zv + 96, 40)
    v = sc.part(P.motion(swing=35.0, pivot=(0, 60, zv), axis=P.UP, rate=0.12, phase=0.4))
    v.box(TIN, (0, 60, zv), (200, 6, 10))
    v.box(CYAN, (-86, 60, zv), (50, 6, 46), glow=0.3)
    v.box(CYAN, (96, 60, zv), (30, 6, 28), glow=0.3)
    # a sledge leant by the wall, a bench with a folded rug
    tilt_box(g, POOL, (120, fy - 60, 0), (128, fy - 10, 150), 60, 8)
    for dx in (-24, 24):
        tilt_box(g, TIN, (120 + dx, fy - 70, 0), (128 + dx, fy - 18, 156), 6, 8)
    g.box(WHITE, (0, fy - 24, 6), (W + 80, 60, 30), glow=SG)


# ------------------------------------------------------------------------------------------ PROPS (four LOW, four TALL)
def sleigh(sc):
    """PROP 1, LOW (top 1.76 m), unlit: a blue sleigh on curled tin runners, a sack and a scarf left in it."""
    g = sc.still()
    g.box(WHITE, (0, 0, -24), (340, 150, 50))
    for y in (-48, 48):
        g.box(TIN, (0, y, 7), (300, 8, 10))
        tilt_box(g, TIN, (150, y, 7), (196, y, 42), 8, 10)
        tilt_box(g, TIN, (196, y, 42), (184, y, 88), 8, 10)
        for x in (-100, 60):
            g.box(TIN, (x, y, 26), (8, 8, 34))
    g.box(POOL, (-10, 0, 62), (250, 112, 40))
    g.box(WHITE, (-10, -57, 62), (200, 5, 9))
    tilt_box(g, POOL, (112, 0, 50), (160, 0, 118), 112, 14)
    g.box(POOL, (-128, 0, 112), (14, 112, 100))
    g.box(CYAN, (-60, 0, 92), (110, 100, 22))
    g.box(WHITE, (-128, 0, 168), (30, 124, 16))
    g.box(WHITE, (40, 0, 96), (70, 80, 30))
    g.sphere(WHITE, (40, 0, 120), 34, steps=(4, 6))
    g.box(RED, (-128, -30, 138), (18, 26, 62))
    g.box(TIN, (176, 0, 112), (8, 8, 60))
    g.box(TIN, (176, 0, 152), (24, 24, 26))
    g.box(SPRAY, (176, 0, 152), (17, 26, 17))
    g.sphere(WHITE, (0, 0, 2), 190, scale=(1.1, 0.5, 0.1), steps=(4, 8))


def ski_stack(sc):
    """PROP 2, LOW (top under 1.9 m), unlit: skis and poles leant on a rack, two sledges stacked, a pair of boots."""
    g = sc.still()
    g.box(WHITE, (0, -20, -24), (380, 220, 50))
    for x in (-150, 150):
        tilt_box(g, POOL, (x, -40, 0), (x, 0, 150), 12, 12)
        tilt_box(g, POOL, (x, 40, 0), (x, 0, 150), 12, 12)
    g.box(POOL, (0, 0, 150), (330, 14, 14))
    g.box(WHITE, (0, 0, 162), (330, 22, 10))
    for k, c in enumerate((CYAN, WHITE, SPRAY, TIN, CYAN, POOL, WHITE)):
        x = -120 + k * 40
        tilt_box(g, c, (x, -62, 4), (x, -9, 172), 13, 5)
        tilt_box(g, c, (x, -9, 172), (x, -16, 181), 13, 5)
    for x in (-166, 166):
        tilt_box(g, TIN, (x, -50, 0), (x, -8, 140), 5, 5)
        g.box(TIN, (x, -48, 14), (16, 6, 16))
    for k in range(2):
        z = 22 + k * 20
        g.box(POOL, (60 - k * 8, -108, z + 6), (124, 48, 8))
        for y in (-128, -88):
            g.box(TIN, (60 - k * 8, y, z - 3), (136, 5, 6))
            g.box(TIN, (104 - k * 8, y, z + 1), (6, 5, 10))
    for x in (-96, -66):
        g.box(WHITE, (x, -104, 16), (22, 34, 30))
        g.box(CYAN, (x, -104, 34), (24, 26, 8))
        g.box(WHITE, (x, -118, 8), (22, 22, 14))


def snow_fence(sc):
    """PROP 3, LOW (top 1.5 m), unlit: a snow-capped fence run, 8 m, a drift against it, a scarf tied on a post."""
    g = sc.still()
    g.box(WHITE, (0, 10, -22), (840, 110, 50))
    for k in range(5):
        x = -400 + k * 200
        g.box(POOL, (x, 0, 55), (22, 22, 120))
        g.box(WHITE, (x, 0, 122), (34, 34, 16))
        g.sphere(WHITE, (x, 0, 134), 16, steps=(3, 6))
    for z in (50, 92):
        g.box(POOL, (0, 0, z), (800, 10, 14))
    g.box(WHITE, (0, 0, 103), (780, 16, 9))
    fringe(g, -390, 390, -2, 86, drop=22, tooth=46, seed=4, t=8)
    for x, s in ((-250, 1.0), (60, 1.3), (310, 0.8)):
        g.sphere(WHITE, (x, -34, 6), 100 * s, scale=(1.4, 0.6, 0.36), steps=(4, 8))
    g.box(RED, (200, -13, 100), (28, 6, 16))
    g.box(RED, (208, -13, 76), (9, 6, 40))
    g.box(CYAN, (-200, -13, 104), (20, 8, 26))
    g.box(CYAN, (-212, -13, 100), (8, 8, 12))


def ice_seats(sc):
    """PROP 4, LOW (top 1.88 m), unlit: three ice-block benches with fur throws between two ice posts, a line of
    pennons, snowball pyramids."""
    g = sc.still()
    g.box(WHITE, (0, 6, -22), (720, 160, 50))
    for x in (-230, 0, 230):
        g.box(SPRAY, (x, 10, 28), (170, 70, 50))
        g.box(CYAN, (x, 10, 55), (172, 72, 5))
        g.box(SPRAY, (x, 48, 80), (170, 22, 50))
        g.box(WHITE, (x - 30, 4, 62), (80, 62, 10))
        g.box(WHITE, (x, 48, 108), (160, 26, 8))
    for x in (-115, 115):
        for dx, dy in ((-16, -8), (16, -8), (0, 16)):
            g.sphere(WHITE, (x + dx, -20 + dy, 14), 17, steps=(3, 6))
        g.sphere(WHITE, (x, -18, 40), 17, steps=(3, 6))
    for sx in (-1, 1):
        g.cyl(SPRAY, (sx * 338, 10, 0), 22, 152, steps=6, top_r=16)
        g.sphere(WHITE, (sx * 338, 10, 162), 26, steps=(4, 6))
    g.box(TIN, (0, 10, 146), (660, 5, 5))
    for k in range(9):
        g.box((WHITE, CYAN, SPRAY)[k % 3], (-280 + k * 70, 10, 128), (26, 5, 30))


def _snowman(sc, g, x, y, s, hat, phase):
    g.sphere(WHITE, (x, y, 62 * s), 78 * s, scale=(1, 1, 0.88), steps=(5, 8), glow=SG)
    g.sphere(WHITE, (x, y, 168 * s), 58 * s, steps=(5, 8), glow=SG)
    g.sphere(WHITE, (x, y, 250 * s), 42 * s, steps=(4, 8), glow=SG)
    for dx in (-14, 14):
        g.box(NAVY, (x + dx * s, y - 38 * s, 258 * s), (9 * s, 8 * s, 9 * s))
    g.cyl(SPRAY, (x, y - 38 * s, 246 * s), 8 * s, 34 * s, rot=(-90, -90, 0), steps=5, top_r=2 * s)
    for k in range(3):
        g.box(NAVY, (x, y - 56 * s, (140 + k * 24) * s), (9 * s, 8 * s, 9 * s))
    for sx in (-1, 1):
        tilt_box(g, POOL, (x + sx * 50 * s, y, 180 * s), (x + sx * 112 * s, y - 10 * s, 216 * s), 8 * s, 8 * s)
    if hat == 'top':
        g.cyl(TIN, (x, y, 285 * s), 48 * s, 7 * s, steps=8)
        g.cyl(POOL, (x, y, 290 * s), 30 * s, 46 * s, steps=8)
        g.cyl(CYAN, (x, y, 292 * s), 32 * s, 9 * s, steps=8)
    elif hat == 'bobble':
        g.cyl(CYAN, (x, y, 272 * s), 42 * s, 34 * s, steps=8, top_r=24 * s)
        g.sphere(WHITE, (x, y, 316 * s), 15 * s, steps=(3, 6), glow=SG)
    else:
        g.cyl(TIN, (x, y, 280 * s), 34 * s, 38 * s, steps=8, top_r=40 * s)
        g.box(TIN, (x + 48 * s, y, 306 * s), (30 * s, 8 * s, 8 * s))
    g.cyl(RED, (x, y, 208 * s), 45 * s, 17 * s, steps=8)
    t = sc.part(P.motion(swing=12.0, pivot=(x + 32 * s, y - 34 * s, 216 * s), axis=90.0, rate=0.6, phase=phase))
    t.box(RED, (x + 34 * s, y - 42 * s, 186 * s), (18 * s, 9 * s, 62 * s))


def snowman_family(sc):
    """PROP 5, TALL (3.4 m): a snowman family, statues: hats, red scarves whose tails stir, coal buttons, ice noses."""
    g = sc.still()
    g.box(WHITE, (30, 0, -60), (520, 240, 120), glow=SG)
    g.sphere(WHITE, (30, 0, 0), 250, scale=(1.1, 0.5, 0.1), steps=(4, 8), glow=SG)
    _snowman(sc, g, -110, 10, 1.0, 'top', 0.0)
    _snowman(sc, g, 70, 0, 0.82, 'bobble', 0.35)
    _snowman(sc, g, 196, -40, 0.52, 'pot', 0.7)
    # the broom the tallest holds
    tilt_box(g, POOL, (-232, -4, 6), (-218, -2, 262), 8, 8)
    g.box(TIN_D, (-234, -4, 26), (40, 14, 52))


def ice_fish(sc):
    """PROP 6, TALL (4.9 m): an ice sculpture of a leaping fish on a plinth of blocks, a crown of ice for its splash."""
    g = sc.still()
    g.box(SPRAY, (0, 0, -10), (240, 240, 280), glow=0.06)
    for z in (40, 86):
        g.box(CYAN, (0, 0, z), (246, 246, 6), glow=0.4)
    g.box(WHITE, (0, 0, 140), (272, 272, 26), glow=SG)
    flake(g, 0, -124, 62, 34)
    for k in range(8):
        a = math.radians(k * 45 + 10)
        g.cyl(SPRAY, (66 * math.cos(a), 66 * math.sin(a), 150), 26, 80 + 34 * (k % 3), steps=4, top_r=3, glow=0.1)
    pts = ((-70, 175), (-96, 275), (-60, 372), (22, 440), (124, 456))
    for k, d in enumerate((62, 96, 104, 78)):
        tilt_box(g, SPRAY, (pts[k][0], 0, pts[k][1]), (pts[k + 1][0], 0, pts[k + 1][1]), 30 + 8 * min(k, 2), d, glow=0.14)
    g.sphere(SPRAY, (150, 0, 452), 44, scale=(1.2, 0.7, 0.9), steps=(4, 8), glow=0.14)
    for sy in (-1, 1):
        g.box(NAVY, (160, sy * 26, 464), (10, 6, 10))
        g.box(CYAN, (20, sy * 34, 404), (60, 30, 8), rot=(20, sy * 30, 0), glow=0.5)
    tilt_box(g, CYAN, (-70, 0, 180), (-134, 0, 124), 14, 54, glow=0.5)
    tilt_box(g, CYAN, (-70, 0, 180), (-18, 0, 118), 14, 54, glow=0.5)
    tilt_box(g, CYAN, (-44, 0, 410), (-86, 0, 474), 12, 50, glow=0.5)


def lamp_pair(sc):
    """PROP 7, TALL (4.8 m): a pair of frosted lamp posts, steady cyan lanterns, a swag of snow-laden pennons between."""
    g = sc.still()
    for sx in (-1, 1):
        x = sx * 170.0
        lx = x - sx * 60.0
        g.cyl(POOL, (x, 0, -120), 30, 190, steps=8, top_r=20)
        g.cyl(WHITE, (x, 0, 62), 34, 16, steps=8, glow=SG)
        g.cyl(TIN, (x, 0, 70), 10, 380, steps=6)
        g.cyl(TIN, (x, 0, 250), 15, 12, steps=6)
        tilt_box(g, TIN, (x, 0, 440), (lx, 0, 480), 8, 8)
        g.box(TIN, (lx, 0, 466), (6, 6, 30))
        lantern(g, lx, 0, 410, 1.3)
        icicle(g, lx - 14, 0, 404, 34, 6)
        icicle(g, lx + 14, 0, 404, 22, 6)
        g.sphere(WHITE, (x, 0, 458), 18, steps=(3, 6), glow=SG)
        g.sphere(WHITE, (x + sx * 40, 10, 4), 70, scale=(1.2, 1.0, 0.4), steps=(4, 6), glow=SG)
    prev = None
    for k in range(8):
        u = k / 7.0
        p = (-92 + 184 * u, 0, 400 - 46 * math.sin(math.pi * u))
        if prev:
            tilt_box(g, TIN, prev, p, 5, 5)
            mid = ((prev[0] + p[0]) / 2.0, 0, (prev[2] + p[2]) / 2.0 - 17)
            g.box(CYAN if k % 2 else WHITE, mid, (20, 5, 28), glow=0.15)
        prev = p


def vane_post(sc):
    """PROP 8, TALL (6.6 m): a weather mast: four ice pointers (no letters), a thermometer board, a bird house with
    a red door, and a snowflake vane that turns slowly to and fro."""
    g = sc.still()
    g.cyl(POOL, (0, 0, -120), 34, 200, steps=8, top_r=22)
    g.cyl(WHITE, (0, 0, 74), 40, 18, steps=8, glow=SG)
    g.cyl(POOL, (0, 0, 80), 13, 470, steps=6)
    for a in (0, 90, 180, 270):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        g.box(SPRAY, (86 * ca, 86 * sa, 430), (150, 10, 16), rot=(0, a, 0), glow=0.1)
        g.box(SPRAY, (160 * ca, 160 * sa, 430), (30, 10, 46), rot=(0, a, 0), glow=0.1)
    g.sphere(WHITE, (0, 0, 446), 24, steps=(3, 6), glow=SG)
    g.box(TIN, (0, -18, 250), (60, 8, 150))
    g.box(WHITE, (0, -23, 256), (16, 5, 112))
    g.box(CYAN, (0, -25, 226), (12, 5, 52), glow=0.6)
    g.box(CYAN, (0, -25, 192), (24, 6, 22), glow=0.6)
    g.box(WHITE, (0, -18, 330), (72, 18, 12), glow=SG)
    g.box(TIN, (30, 0, 330), (60, 8, 8))
    g.box(POOL, (62, 0, 350), (70, 60, 60))
    g.box(RED, (62, -32, 348), (22, 5, 26))
    g.box(TIN, (62, -34, 328), (30, 14, 5))
    g.box(WHITE, (62, 0, 388), (96, 84, 18), glow=SG)
    g.cyl(TIN, (0, 0, 550), 6, 46, steps=6)
    v = sc.part(P.motion(swing=40.0, pivot=(0, 0, 600), axis=P.UP, rate=0.1, phase=0.15))
    v.box(TIN, (0, 0, 600), (240, 6, 8))
    v.box(CYAN, (-105, 0, 600), (50, 6, 60), glow=0.4)
    v.box(CYAN, (124, 0, 600), (30, 6, 30), glow=0.4)
    flake(v, 0, 0, 634, 30)


# ------------------------------------------------------------------------------------------ THE HERO
def snow_globe(sc):
    """THE SNOW GLOBE: a glass dome 50 m across (a cage of ice ribs: the master is opaque) on a carved drum; inside,
    a chalet village, a chapel and a fir on a hill of snow, lit softly from below, and snow drifting round. 35 m tall."""
    r = random.Random(17)
    g = sc.still()
    RD, HD, RG = 2700.0, 900.0, 2500.0
    ZC = 1030.0                                           # the dome's floor and the centre of its sphere
    g.cyl(TIN, (0, 0, -300), RD + 160, 450, steps=20)
    g.cyl(POOL, (0, 0, 150), RD, HD - 150, steps=20)
    g.cyl(TIN, (0, 0, 560), RD + 14, 30, steps=20)
    g.cyl(WHITE, (0, 0, HD), RD + 90, 70, steps=20, glow=SG)
    g.cyl(TIN, (0, 0, 970), RG + 60, 60, steps=20)
    g.cyl(CYAN, (0, 0, ZC), RG - 20, 14, steps=20, glow=0.7)
    for k in range(12):                                   # the drum's carving: lit round windows and snowflakes, turn about
        ad = k * 30 + 15.0
        ca, sa = math.cos(math.radians(ad)), math.sin(math.radians(ad))
        if k % 2 == 0:
            rod(g, TIN, ((RD - 6) * ca, (RD - 6) * sa, 350), ((RD + 16) * ca, (RD + 16) * sa, 350), 124, steps=8)
            rod(g, CYAN, ((RD - 4) * ca, (RD - 4) * sa, 350), ((RD + 24) * ca, (RD + 24) * sa, 350), 98, steps=8, glow=WG)
        else:
            for j in range(3):
                g.box(SPRAY, ((RD + 6) * ca, (RD + 6) * sa, 350), (236, 16, 36), rot=(j * 60.0, ad + 90.0, 0), glow=0.2)
    g.box(SPRAY, (0, -RD - 6, 400), (470, 40, 570), glow=0.1)
    g.box(RED, (0, -RD - 24, 366), (340, 20, 470))
    for k in range(3):
        g.box(WHITE, (0, -RD - 110 - k * 120, 110 - k * 110), (620, 130, 320), glow=SG)
    # the hill of snow and what stands on it
    RH, HH = RG - 150.0, 0.26 * (RG - 150.0)
    g.sphere(WHITE, (0, 0, ZC), RH, scale=(1, 1, 0.26), steps=(6, 12), glow=0.22)

    def hill(x, y):
        return ZC + HH * math.sqrt(max(0.0, 1.0 - (x * x + y * y) / (RH * RH)))

    for x, y, w in ((-1150, -520, 640), (-380, -1200, 600), (720, -960, 660), (1300, -160, 600), (-950, 620, 620)):
        z = hill(x, y) - 70
        d, h = 520.0, 400.0
        g.box(POOL, (x, y, z + h / 2.0), (w, d, h))
        rs = 300.0 / (w / 2.0)
        g.side(WHITE, ccw([(x - w / 2.0 - 80, z + h - 80 * rs), (x, z + h + 300), (x + w / 2.0 + 80, z + h - 80 * rs),
                           (x + w / 2.0 + 80, z + h - 80 * rs + 90), (x, z + h + 410), (x - w / 2.0 - 80, z + h - 80 * rs + 90)]),
               y - d / 2.0 - 70, y + d / 2.0 + 40, glow=0.14)
        g.box(CYAN, (x - w * 0.2, y - d / 2.0 - 6, z + 250), (150, 12, 150), glow=WG)
        g.box(RED, (x + w * 0.22, y - d / 2.0 - 6, z + 190), (120, 12, 230))
    # the chapel in the middle
    cz = hill(0, -150) - 60
    g.box(POOL, (120, -150, cz + 220), (760, 520, 440))
    g.side(WHITE, ccw([(-340, cz + 380), (120, cz + 760), (580, cz + 380), (580, cz + 470), (120, cz + 870), (-340, cz + 470)]),
           -480, 150, glow=0.14)
    g.box(POOL, (-330, -150, cz + 470), (360, 360, 940))
    g.cyl(WHITE, (-330, -150, cz + 940), 290, 560, rot=(0, 45, 0), steps=4, top_r=14, glow=0.14)
    g.box(WHITE, (-330, -150, cz + 940), (430, 430, 40), glow=0.14)
    g.box(CYAN, (-330, -336, cz + 700), (130, 12, 220), glow=WG)
    g.box(CYAN, (160, -416, cz + 240), (150, 12, 200), glow=WG)
    g.box(RED, (420, -416, cz + 130), (130, 12, 250))
    # the fir, snow on every tier, a star of ice on top
    fx, fyy = 620.0, 420.0
    fz = hill(fx, fyy) - 40
    g.cyl(POOL, (fx, fyy, fz), 70, 300, steps=6)
    for k in range(4):
        rr = 520.0 - k * 110.0
        g.cyl(PINE, (fx, fyy, fz + 250 + k * 330), rr, 460, steps=6, top_r=40)
        g.cyl(WHITE, (fx, fyy, fz + 400 + k * 330), rr * 0.72, 330, steps=6, top_r=30, glow=0.15)
    flake(g, fx, fyy, fz + 1760, 90, glow=1.0)
    # the glass: ten ribs, two hoops, a boss on top, three streaks of shine on the road side
    for m in range(10):
        a = math.radians(m * 36 + 18.0)
        ca, sa = math.cos(a), math.sin(a)
        for j in range(5):
            e0, e1 = math.radians(j * 18.0), math.radians((j + 1) * 18.0)
            tilt_box(g, SPRAY, (RG * math.cos(e0) * ca, RG * math.cos(e0) * sa, ZC + RG * math.sin(e0)),
                     (RG * math.cos(e1) * ca, RG * math.cos(e1) * sa, ZC + RG * math.sin(e1)), 50, 50, glow=0.3)
    for ed, n in ((30.0, 20), (60.0, 12)):
        e = math.radians(ed)
        rr, zz = RG * math.cos(e), ZC + RG * math.sin(e)
        for k in range(n):
            a0, a1 = math.radians(k * 360.0 / n), math.radians((k + 1) * 360.0 / n)
            tilt_box(g, SPRAY, (rr * math.cos(a0), rr * math.sin(a0), zz), (rr * math.cos(a1), rr * math.sin(a1), zz), 44, 44, glow=0.3)
    g.sphere(TIN, (0, 0, ZC + RG), 150, steps=(4, 8))
    g.cyl(CYAN, (0, 0, ZC + RG + 100), 40, 300, steps=6, top_r=6, glow=0.8)
    a = math.radians(250.0)
    for j in range(3):
        e0, e1 = math.radians(34.0 + j * 11), math.radians(43.0 + j * 11)
        q = RG + 30
        tilt_box(g, WHITE, (q * math.cos(e0) * math.cos(a), q * math.cos(e0) * math.sin(a), ZC + q * math.sin(e0)),
                 (q * math.cos(e1) * math.cos(a), q * math.cos(e1) * math.sin(a), ZC + q * math.sin(e1)), 170, 20, glow=0.5)
    # the snow: four drifts of flakes going round the middle at their own speeds (one against the others), floating
    for gi, rate in enumerate((0.035, -0.028, 0.05, 0.022)):
        fl = sc.part(P.motion(travel=(-1.0, rate), base=(0.0, 0.0), bob=-(110.0 + 30.0 * gi), rate=0.1 + 0.03 * gi, phase=gi * 0.23))
        n = 0
        while n < 11:
            rho = r.uniform(450.0, 2150.0)
            a = r.uniform(0.0, 2.0 * math.pi)
            x, y = rho * math.cos(a), rho * math.sin(a)
            z0 = hill(x, y) + 320.0
            z1 = ZC + math.sqrt(RG * RG - rho * rho) - 300.0
            if z1 <= z0:
                continue
            sz = r.uniform(60.0, 95.0)
            fl.box(WHITE, (x, y, r.uniform(z0, z1)), (sz, sz, sz), rot=(r.uniform(0, 45), r.uniform(0, 90), 0), glow=0.6)
            n += 1


JOBS = [
    ('SM_S1D7_SkateHire', skate_hire, STILL),
    ('SM_S1D7_IceArcade', ice_arcade, STILL),
    ('SM_S1D7_ChocHut', choc_hut),
    ('SM_S1D7_TobogganTower', toboggan_tower),
    ('SM_S1D7_BellChalet', bell_chalet),
    ('SM_S1D7_PalaceGate', palace_gate),
    ('SM_S1D7_MittenShop', mitten_shop),
    ('SM_S1D7_Sleigh', sleigh, STILL),
    ('SM_S1D7_SkiStack', ski_stack, STILL),
    ('SM_S1D7_SnowFence', snow_fence, STILL),
    ('SM_S1D7_IceSeats', ice_seats, STILL),
    ('SM_S1D7_SnowmanFamily', snowman_family),
    ('SM_S1D7_IceFish', ice_fish, STILL),
    ('SM_S1D7_LampPair', lamp_pair, STILL),
    ('SM_S1D7_VanePost', vane_post),
    ('SM_S1D7_SnowGlobe', snow_globe, dict(far=True, grow=(150, 150, 150), grow_down=(150, 150, 0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
