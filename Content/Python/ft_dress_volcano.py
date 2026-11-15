"""FULL THROTTLE (Unreal) dressing kit, level 3: the Volcano (Mount Kaldera). Geological depth and the people who
worked here: columnar basalt and fractured cliffs on the slopes, lava falls, heat-stained stacks and ash banks; a
survey camp, gantries and sluices over the lava channels, a drill site, cooling fins, abandoned haulers, a ruined
observatory; and on the horizon the caldera's rim, cinder cones, far lava falls and a ridge of pylons.
Placed by Source/FullThrottle/FTDressL3.inl. See ft_dress.py for how a piece is drawn (front faces -Y, base on
z = 0, long axis along X; horizon pieces built from z = 0 up).

    import ft_dress; ft_dress.run(['volcano'])

Names matter to the export's finishing pass (ft_visual_art): a name that starts SM_Crag, SM_Rock, SM_Ridge or SM_Peak
is finished as rock (a few facets inside its own envelope, half of them in the level's hot orange: right for
heat-stained rock, wrong for ash, so the ash pieces have plain names); SM_Pole... gets a small mounting block; anything else
gets two service panels on its -X end, sized and placed by the piece's whole envelope. So: buildings keep a wall just
inside their -X end for the panels; open frames and vehicles carry a thin root underground (_root) deep enough that
the panels land out of sight; and a rock piece whose facets would hang in the air takes a letter after its name (the
facets are seeded by the name). SkyScratch/SourceArt/Jam/unreal/dress_tools/kit_dry.py works all of that out without the editor.
"""
import math
import random
import ft_art
from ft_desert import rod, tilt_box
from ft_car import YEL, WHITE

LAVA, LAVA_Y, ORANGE, RED = '#ff6a1a', '#ffd23f', '#ff8a2a', '#ff2d3c'
BASALT, BASALT_L, ASH, CHAR = '#2a2230', '#3e3244', '#5a4e5e', '#1e1820'
ASH_L = '#6f6272'
CONC, CONC_D = '#9a98a8', '#6a6878'
RUST, RUST_D = '#8c4a2e', '#5c3324'
STAIN_R, STAIN_O, SULPH = '#7b3526', '#b9743b', '#c8b14b'
LIVERY, HUT, TRUCK, PANE = '#e2672b', '#cdc5c8', '#c7a13b', '#14101a'
LIT = '#ffe28a'
# second pass: the verge's ash, pale enough to read against the black ground without a light on it (the verge may
# carry none). Both are colours the game's palette already has an instance of.
PALE, PALE_L = '#87746d', '#b0a088'
DIR = '/Game/Maps/Volcano/Dress/'


def _root(m, depth, x=0.0, y=0.0):
    """A thin root straight down: it stretches the piece's envelope underground, so the finishing pass's panels (sized
    and placed by the envelope) land out of sight instead of hanging in an open frame."""
    m.box(CHAR, (x, y, -depth / 2.0), (8, 8, depth))


def _jag(r, x0, x1, teeth, lo, hi, floor=0.0):
    """A jagged skyline as a closed side profile [(x, z)]: along the bottom, then back over the teeth."""
    pts = [(x0, floor), (x1, floor)]
    n = teeth * 2
    step = (x1 - x0) / float(n)
    for k in range(n + 1):
        x = x1 - step * k
        if k in (0, n):
            z = lo * 0.12                                   # the wall runs down to nothing at its ends
        else:
            z = r.uniform(lo, hi) if k % 2 else r.uniform(lo * 0.55, lo)
            x += r.uniform(-0.3, 0.3) * step
            z *= 0.6 if k in (1, n - 1) else (0.85 if k in (2, n - 2) else 1.0)
        pts.append((x, z))
    return pts


def _ccw(pts):
    """The polygon counter-clockwise, whichever way it was written (an extrusion written clockwise is inside out)."""
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def _oval(rx, ry, n=14, cx=0.0, cy=0.0, a0=0.0):
    return [(cx + math.cos(a0 + 6.28318 * k / n) * rx, cy + math.sin(a0 + 6.28318 * k / n) * ry) for k in range(n)]


def _ribbon(m, col, a, b, y, width, thick=50, glow=0.0):
    """A flat length of lava facing the road, from a = (x, z) down to b = (x, z), `width` across."""
    dx, dz = a[0] - b[0], a[1] - b[1]
    m.box(col, ((a[0] + b[0]) / 2.0, y, (a[1] + b[1]) / 2.0), (width, thick, math.hypot(dx, dz) or 1.0),
          rot=(math.degrees(math.atan2(-dx, dz)), 0, 0), glow=glow)


def _cascade(m, r, x, y, z_top, z_bot, w, glow=1.4):
    """A lava fall as a broken cascade (second pass: drawn as one box it read as a straight yellow bar from every
    distance): five lengths, each stepping sideways and widening, a ledge of dark rock where it steps, a bright core
    on alternate lengths, and two streams running apart at the foot. ft_life runs light down it (animate() below)."""
    n = 5
    H = z_top - z_bot
    cx, cz = x, z_top
    for k in range(n):
        nz = z_top - H * (k + 1) / float(n)
        nx = cx + r.uniform(-1.0, 1.0) * w * 1.2
        ww = w * (0.7 + 0.2 * k)
        if k == n - 1:
            for sd in (-1, 1):
                _ribbon(m, LAVA, (cx, cz), (cx + sd * ww * 1.2, nz), y, ww * 0.55, 50, glow)
        else:
            _ribbon(m, LAVA, (cx, cz), (nx, nz), y, ww, 50, glow)
            if k % 2 == 0:
                _ribbon(m, LAVA_Y, (cx, cz - H * 0.02), (nx, nz + H * 0.05), y - 30, ww * 0.34, 40, glow + 0.4)
            m.box(BASALT, (nx + r.uniform(-0.4, 0.4) * ww, y - 40, nz), (ww * 2.6, 150, H * 0.03 + 40), rot=(r.uniform(-9, 9), 0, 0))
        cx, cz = nx, nz


def _columns(m, r, x0, x1, y0, rows, per_row, h0, h1, rad, rise=0.0, fan=0.0):
    """Hexagonal basalt columns in staggered rows; each row further back (+Y) stands rise higher."""
    cols = [BASALT, BASALT_L, '#352a3a']
    for j in range(rows):
        for k in range(per_row):
            x = x0 + (x1 - x0) * (k + 0.5 * (j % 2)) / float(per_row)
            h = r.uniform(h0, h1) + j * rise
            rr = rad * r.uniform(0.82, 1.12)
            lean = fan * ((k + 0.5) / per_row - 0.5) * 2.0
            m.cyl(cols[(k + j) % 3], (x + r.uniform(-0.2, 0.2) * rad, y0 + j * rad * 1.7 + r.uniform(-0.2, 0.2) * rad, -800), rr, h + 800,
                  rot=(lean, 0, 0), steps=6)


# ------------------------------------------------------------------------------------------ the verge (low, dull)
def ash_bank(m, seed=1):
    """A bank of ash along the verge with things half sunk in it: a drum, a post, a slab."""
    r = random.Random(seed)
    m.sphere(PALE, (0, 0, -40), 1000, scale=(1.25, 0.24, 0.13), steps=(4, 9))       # second pass: pale ash (was dark on dark)
    m.sphere(PALE_L, (-500, 60, -30), 520, scale=(1.3, 0.34, 0.2), steps=(4, 8))
    m.sphere(PALE_L, (640, -40, -40), 420, scale=(1.4, 0.36, 0.2), steps=(4, 8))
    rod(m, RUST, (250, 30, 20), (330, 70, 96), 44, steps=8)
    m.box(CHAR, (-820, 20, 70), (10, 10, 190), rot=(0, 0, 9))
    m.box(RUST_D, (-820, 12, 140), (6, 70, 46), rot=(0, 0, 9))
    for k in range(4):
        m.box(BASALT, (r.uniform(-900, 900), r.uniform(-90, 90), 26), (r.uniform(60, 130), r.uniform(50, 90), 30), rot=(r.uniform(-14, 14), r.uniform(0, 90), 0))


def guard_fence(m):
    """Forty metres of battered guard rail: posts, two rails, one bay knocked flat, a plate that once warned of lava."""
    L = 4000
    for k in range(11):
        x = -L / 2.0 + k * 400
        lean = 26 if k in (6, 7) else (4 if k % 4 == 0 else 0)
        m.box(CHAR, (x, 0, 40), (14, 14, 210), rot=(0, 0, lean))
    for z in (60, 104):                                                  # second pass: ochre rails (rust on black did not read)
        m.box(STAIN_O, (-900, 0, z), (2200, 9, 18))
        m.box(STAIN_O, (1450, 0, z), (1100, 9, 18))
    tilt_box(m, RUST_D, (220, 0, 96), (760, 60, 22), 18, 9)
    tilt_box(m, RUST_D, (300, 0, 50), (860, 80, 14), 18, 9)
    m.box(CHAR, (-1500, -8, 110), (150, 6, 110))
    m.box(STAIN_O, (-1500, -12, 110), (120, 4, 80))
    m.sphere(PALE, (500, 40, -30), 330, scale=(1.5, 0.5, 0.2), steps=(4, 7))
    _root(m, 160, -L / 2.0)


def pipe_run(m):
    """Forty-four metres of twin geothermal pipe on saddles, an expansion loop in the middle, a valve wheel."""
    L = 4400
    for y, c in ((-50, CONC), (50, STAIN_O)):                            # second pass: paler pipe
        rod(m, c, (-L / 2.0, y, 96), (-360, y, 96), 36, steps=8)
        rod(m, c, (360, y, 96), (L / 2.0, y, 96), 36, steps=8)
        rod(m, c, (-360, y, 96), (-360, y, 250), 36, steps=8)
        rod(m, c, (360, y, 96), (360, y, 250), 36, steps=8)
        rod(m, c, (-360, y, 250), (360, y, 250), 36, steps=8)
    for k in range(8):
        x = -L / 2.0 + 240 + k * 560
        if abs(x) < 420:
            continue
        m.box(CONC_D, (x, 0, 28), (60, 190, 76))
    for x in (-1500, 1300):
        rod(m, STAIN_O, (x - 14, 0, 96), (x + 14, 0, 96), 62, steps=8)
    m.cyl(CHAR, (1000, -70, 96), 10, 120, steps=6)
    m.cyl(RUST_D, (1000, -70, 210), 44, 10, steps=8)


def rubble(m, seed=2):
    """Heat-cracked slabs and stubs of column by the road."""
    r = random.Random(seed)
    for k in range(7):
        x, y = r.uniform(-360, 360), r.uniform(-170, 170)
        if k % 3 == 0:
            m.cyl(BASALT_L, (x, y, -40), r.uniform(60, 90), r.uniform(70, 150), rot=(r.uniform(-12, 12), 0, r.uniform(-12, 12)), steps=6)
        else:
            m.box([BASALT, STAIN_R, '#352a3a'][k % 3], (x, y, 20), (r.uniform(120, 240), r.uniform(90, 170), r.uniform(24, 50)),
                  rot=(r.uniform(-16, 16), r.uniform(0, 90), r.uniform(-10, 10)))
    m.sphere(PALE, (0, 0, -40), 420, scale=(1.1, 0.5, 0.14), steps=(4, 7))


def cable_reels(m):
    """What a crew left: crates stacked at one end, two cable reels, drums, a tarp over something."""
    for k, (y, z, s) in enumerate(((-90, 60, 120), (40, 60, 120), (-30, 178, 110), (130, 55, 110))):
        m.box(RUST_D if k % 2 else '#4c3a30', (-250, y, z), (120, s, s))
    for x, y in ((40, -60), (210, 70)):
        rod(m, '#4c3a30', (x, y - 46, 96), (x, y + 46, 96), 60, steps=8)
        for s in (-1, 1):
            rod(m, RUST, (x, y + s * 46, 96), (x, y + s * 58, 96), 96, steps=10)
    for k in range(3):
        m.cyl(RUST if k % 2 else CONC_D, (330 + (k % 2) * 70, -100 + k * 60, 0), 30, 88, steps=8)
    m.sphere(ASH_L, (120, 150, 40), 110, scale=(1.5, 1.0, 0.7), steps=(4, 6))


def marker_post(m):
    """A survey stake: a striped post, an instrument box on a tripod beside it."""
    m.cyl(CHAR, (0, 0, -40), 6, 210, steps=6)
    for k in range(3):
        m.cyl(STAIN_O if k % 2 else HUT, (0, 0, 50 + k * 40), 7, 30, steps=6)
    for a in (0.3, 2.4, 4.5):
        rod(m, CHAR, (90 + math.cos(a) * 46, math.sin(a) * 46, -20), (90, 0, 120), 4, steps=4)
    m.box(LIVERY, (90, 0, 136), (34, 26, 30))


# ------------------------------------------------------------------------------------------ the people's works
def survey_tower(m):
    """The survey's watch tower: a concrete shaft, a narrower one on it, a glazed cabin, an aerial and a beacon."""
    m.box(CONC_D, (0, 0, 620), (640, 640, 1640))
    m.box(LIVERY, (0, 0, 1380), (654, 654, 90))
    m.box(HUT, (0, 0, 1290), (654, 654, 60))
    m.box(CONC, (0, 0, 1900), (400, 400, 920))
    m.box(CONC_D, (0, 0, 1675), (600, 600, 90))
    m.box(CHAR, (0, 0, 2390), (700, 700, 60))
    m.box(HUT, (0, 0, 2570), (640, 640, 300))
    m.box(PANE, (0, -322, 2590), (560, 6, 170))
    m.box(LIT, (0, -326, 2590), (520, 4, 130), glow=1.4)
    m.box(PANE, (-322, 0, 2590), (6, 560, 170))
    m.box(LIT, (-326, 0, 2590), (4, 520, 130), glow=1.4)
    m.box(LIVERY, (0, 0, 2740), (700, 700, 44))
    m.cyl(CHAR, (120, 120, 2760), 8, 620, steps=6)
    m.sphere(RED, (120, 120, 3400), 26, steps=(4, 6), glow=1.6)
    m.cyl(HUT, (-150, 100, 2760), 110, 30, rot=(40, 30, 0), steps=10, top_r=20)
    for k in range(7):                                             # the stair, flight by flight, up the back
        z0 = 20 + k * 230
        tilt_box(m, CHAR, (-240 if k % 2 else 240, 350, z0), (240 if k % 2 else -240, 350, z0 + 230), 70, 12)
    m.box(CHAR, (0, 380, 800), (560, 8, 1640))
    m.box(CONC_D, (0, 0, -350), (760, 760, 700))


def survey_hut(m):
    """A prefab hut on skids: the survey's livery stripe, ash on the roof, a lit window, steps."""
    L, W, H = 800, 480, 300
    m.box(HUT, (0, 0, 60 + H / 2.0), (L, W, H))
    m.box(LIVERY, (0, 0, 60 + H * 0.3), (L + 6, W + 6, 44))
    m.box(ASH, (0, 0, 60 + H + 16), (L + 50, W + 50, 32))
    m.sphere(ASH_L, (120, 40, 60 + H + 30), 240, scale=(1.3, 0.8, 0.16), steps=(3, 6))
    m.box(PANE, (-150, -W / 2.0 - 3, 190), (90, 6, 200))
    m.box(PANE, (170, -W / 2.0 - 3, 240), (240, 6, 100))
    m.box(LIT, (170, -W / 2.0 - 7, 240), (210, 4, 76), glow=1.4)
    m.box(CONC_D, (330, 0, 60 + H + 70), (120, 120, 80))
    m.cyl(CHAR, (-300, 150, 60 + H), 6, 300, steps=6)
    for s in (-1, 1):
        m.box(CHAR, (0, s * 170, 10), (L - 40, 30, 110))
    m.box(CONC_D, (-150, -W / 2.0 - 10, 30), (120, 50, 50))
    m.box(CONC_D, (0, 0, -220), (L - 140, W - 140, 420))


def survey_hut_round(m):
    """A half-barrel hut, corrugated, half drifted over with ash."""
    L, R = 900, 270
    rod(m, CONC, (-L / 2.0, 0, 20), (L / 2.0, 0, 20), R, steps=12)
    for k in range(5):
        x = -L / 2.0 + 90 + k * 180
        rod(m, CONC_D, (x - 8, 0, 20), (x + 8, 0, 20), R + 8, steps=12)
    for s in (-1, 1):
        m.box(HUT, (s * L / 2.0, 0, 130), (14, R * 2 - 30, 260))
    m.box(PANE, (-L / 2.0 - 8, 0, 110), (6, 110, 200))
    m.box(LIVERY, (-L / 2.0 - 8, 0, 250), (6, 300, 36))
    m.box(LIT, (-L / 2.0 - 9, 150, 150), (4, 90, 70), glow=1.4)
    m.sphere(ASH, (200, 220, -30), 620, scale=(1.2, 0.6, 0.32), steps=(4, 8))
    m.box(CONC_D, (0, 0, -300), (L, R * 2 - 40, 600))


def instrument_mast(m):
    """An instrument mast: wind cups on a cross arm, a dish, a solar panel, the recorder's box at its foot."""
    m.cyl(CONC_D, (0, 0, -300), 60, 350, steps=8)
    m.cyl(CHAR, (0, 0, 0), 10, 1400, steps=6)
    m.box(CHAR, (0, 0, 1280), (10, 300, 10))
    for s in (-1, 1):
        m.sphere(HUT, (0, s * 150, 1300), 22, steps=(4, 6))
    m.box(CHAR, (0, 0, 1380), (240, 8, 8))
    m.sphere(LIVERY, (120, 0, 1390), 18, steps=(4, 6))
    m.cyl(HUT, (0, -16, 900), 80, 24, rot=(0, 0, 80), steps=10, top_r=14)
    m.box(PANE, (0, -40, 560), (120, 8, 170), rot=(0, 0, 30))
    m.box(LIVERY, (0, 40, 130), (70, 60, 120))
    m.sphere(RED, (0, 0, 1420), 14, steps=(4, 6), glow=1.6)


def helipad(m):
    """A landing pad on legs for the survey's helicopter: the ring and the H, a wind sock, the fuel cage along one edge."""
    S = 1400
    m.box(CONC_D, (0, 0, 110), (S, S, 40))
    m.box(HUT, (0, 0, 132), (S - 140, 60, 6))
    m.box(HUT, (0, 0, 132), (60, S - 140, 6))
    for s in (-1, 1):
        m.box(LIVERY, (s * 260, 0, 134), (60, 620, 6))
        m.box(HUT, (0, s * (S / 2.0 - 60), 132), (S - 140, 40, 6))
        m.box(HUT, (s * (S / 2.0 - 60), 0, 132), (40, S - 140, 6))
    m.box(LIVERY, (0, 0, 134), (520, 60, 6))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(CHAR, (sx * (S / 2.0 - 80), sy * (S / 2.0 - 80), -340), (70, 70, 900))
            m.sphere(RED, (sx * (S / 2.0 - 30), sy * (S / 2.0 - 30), 150), 16, steps=(4, 6), glow=1.6)
    m.box(RUST_D, (-S / 2.0 + 50, 0, 250), (100, S - 200, 240))          # the fuel cage
    m.box(RUST_D, (-S / 2.0 + 45, 0, -10), (90, S - 200, 200))           # the store under the deck's edge (the service panels sit on it)
    for k in range(5):
        m.cyl(RUST if k % 2 else STAIN_O, (-S / 2.0 + 170, -400 + k * 200, 130), 34, 90, steps=8)
    m.cyl(CHAR, (S / 2.0 - 60, -S / 2.0 + 60, 130), 7, 520, steps=6)
    tilt_box(m, LIVERY, (S / 2.0 - 60, -S / 2.0 + 60, 630), (S / 2.0 - 260, -S / 2.0 + 120, 590), 50, 50)


def gantry(m):
    """A maintenance gantry over a lava channel (it spans along Y): two towers, a truss, a walkway, a hoist with its hook
    down, pipes carried across, and a plate screen low on the upstream side."""
    half = 1150
    for s in (-1, 1):
        m.box(RUST_D, (0, s * half, 420), (150, 150, 1040))
        m.box(CONC_D, (0, s * half, -110), (260, 260, 340))
        tilt_box(m, RUST, (0, s * (half - 60), 60), (0, s * (half - 420), 560), 30, 30)
    m.box(RUST, (0, 0, 920), (130, half * 2 + 200, 100))
    m.box(RUST_D, (0, 0, 800), (110, half * 2, 26))
    for k in range(8):
        y0 = -half + k * (half * 2 / 8.0)
        tilt_box(m, RUST_D, (0, y0, 810), (0, y0 + half * 2 / 8.0, 900), 16, 16)
    m.box(CHAR, (-150, 0, 560), (120, half * 2 - 160, 14))              # the walkway and its rail
    m.box(CHAR, (-206, 0, 620), (8, half * 2 - 160, 8))
    m.box(CHAR, (-206, 0, 680), (8, half * 2 - 160, 8))
    m.box(LIVERY, (0, 240, 840), (190, 220, 130))                        # the hoist
    rod(m, CHAR, (0, 240, 770), (0, 240, 380), 5, steps=4)
    m.box(CHAR, (0, 240, 350), (60, 60, 60))
    for x in (60, 130):
        rod(m, CONC_D, (x, -half, 300), (x, half, 300), 30, steps=8)
    m.box(RUST, (-180, 0, 170), (14, half * 2 - 320, 540))               # the screen, down into the channel
    for k in range(6):
        m.box(STAIN_O, (-190, -half + 330 + k * 330, 300), (6, 150, 60), rot=(0, 0, 0))


def pipe_bridge(m):
    """Pipes carried over a lava channel on three trestles (it spans along Y); one has burst and stands venting."""
    half = 1300
    for y in (-half + 150, 0, half - 150):
        for s in (-1, 1):
            tilt_box(m, RUST_D, (s * 150, y, -60), (s * 40, y, 420), 30, 30)
        m.box(RUST_D, (0, y, 430), (300, 60, 30))
        m.box(CONC_D, (0, y, -165), (420, 160, 330))
    for x, c in ((-90, CONC_D), (0, RUST), (90, CONC_D)):
        rod(m, c, (x, -half, 480), (x, half, 480), 34, steps=8)
    m.box(CHAR, (0, 0, 120), (340, half * 2 - 500, 20))                  # a service deck slung underneath
    for k in range(3):
        m.sphere(ASH_L if k % 2 else HUT, (0, 420 + k * 30, 600 + k * 170), 90 + k * 50, steps=(4, 6))
    m.box(CHAR, (0, 400, 500), (80, 90, 60))


def sluice_gate(m):
    """A sluice across a lava channel (it spans along Y): a concrete wall, a gate in its middle with the lava showing
    through, two lifting towers, a walkway along the top."""
    half = 900
    for s in (-1, 1):
        m.box(CONC_D, (0, s * (half + 150) / 2.0, 250), (300, half - 150, 620))
        m.box(CONC, (-205, s * (half + 150) / 2.0, 60), (110, half - 150, 380))    # the apron upstream
        m.box(CONC, (0, s * 230, 640), (220, 160, 420))
    m.box(CONC_D, (0, 0, 520), (300, 300, 120))
    m.box(RUST_D, (0, 0, 320), (60, 300, 300))
    m.box(LAVA, (10, 0, 80), (120, 290, 150), glow=1.6)
    m.box(LAVA_Y, (-150, 0, 55), (240, 240, 30), glow=2.0)
    m.box(CHAR, (0, 0, 870), (180, 700, 60))
    m.box(CHAR, (-150, 0, 590), (60, half * 2, 12))
    m.box(CHAR, (-176, 0, 650), (8, half * 2, 8))
    for k in range(6):
        m.box(STAIN_O, (-152, -half + 200 + k * 290, 420), (4, 150, 70))
    m.box(CONC_D, (0, 0, -150), (340, half * 2, 320))


def drill_rig(m):
    """An abandoned geothermal drill: the derrick leaning, its engine house, the drill pipe racked beside it."""
    lean = 5
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, RUST, (sx * 260 + 150, sy * 260, 0), (sx * 60 + 150 + lean * 30, sy * 60, 2200), 34, 34)
    for k in range(5):
        z = 260 + k * 400
        w = 520 - k * 86
        m.box(RUST_D, (150 + lean * (z / 73.0), 0, z), (w, w, 26))
    rod(m, CHAR, (150, 0, 0), (150 + lean * 28, 0, 2100), 14, steps=6)
    m.box(RUST_D, (150 + lean * 30, 0, 2230), (200, 200, 110))
    m.sphere(RED, (150 + lean * 30, 0, 2310), 22, steps=(4, 6), glow=1.6)    # the one lamp nobody turned off
    m.box(TRUCK, (-360, 0, 190), (420, 620, 380))                        # the engine house, on the approach side
    m.box(CHAR, (-360, 0, 392), (460, 660, 26))
    m.box(PANE, (-574, 120, 220), (6, 200, 130))
    m.cyl(CHAR, (-300, -200, 400), 24, 320, steps=6)
    for k in range(6):
        rod(m, CONC_D, (60 + (k % 3) * 34, 380 + (k // 3) * 30, 60 + (k // 3) * 30), (960 + (k % 3) * 34, 380 + (k // 3) * 30, 60 + (k // 3) * 30), 14, steps=5)
    m.cyl(CONC_D, (640, -360, 0), 150, 260, steps=10)
    m.sphere(ASH, (300, 0, -40), 760, scale=(0.9, 0.9, 0.16), steps=(4, 8))
    m.box(CONC_D, (250, 0, -420), (1500, 1100, 800))
    _root(m, 1850, 150)


def hauler(m):
    """A quarry hauler left where it stopped, its bed up, ash drifted against its wheels."""
    m.box(TRUCK, (60, 0, 260), (620, 330, 120))
    m.box(TRUCK, (-260, 0, 410), (190, 300, 220))
    m.box(PANE, (-360, 0, 440), (8, 250, 110))
    m.side(RUST, [(-140, 330), (440, 330), (520, 560), (-140, 470)], -190, 190, rot=(0, 0, 0))
    m.box(CHAR, (430, 0, 300), (60, 300, 60))
    for x in (-250, 180, 330):
        for s in (-1, 1):
            rod(m, CHAR, (x, s * 150, 110), (x, s * 215, 110), 110, steps=10)
    m.box(RUST_D, (-380, 0, 250), (40, 300, 90))
    m.sphere(ASH, (120, 180, -30), 420, scale=(1.5, 0.6, 0.3), steps=(4, 7))
    m.sphere(ASH_L, (-300, -140, -30), 300, scale=(1.2, 0.7, 0.3), steps=(4, 6))
    m.box(ASH, (60, 0, -430), (900, 420, 800))
    _root(m, 480)


def cooling_fins(m):
    """A bank of geothermal cooling fins on a manifold: tall slabs in a row, a header pipe over them, heat at the root."""
    L = 1600
    m.box(CONC_D, (-L / 2.0 - 60, 0, 300), (120, 520, 640))             # the end wall (and where the service panels sit)
    rod(m, RUST, (-L / 2.0, 0, 70), (L / 2.0, 0, 70), 60, steps=8)
    for k in range(9):
        x = -L / 2.0 + 120 + k * 170
        m.box(CONC if k % 2 else CONC_D, (x, 0, 370), (26, 440, 540))
    rod(m, CONC_D, (-L / 2.0, 0, 690), (L / 2.0 + 120, 0, 690), 40, steps=8)
    rod(m, CONC_D, (L / 2.0 + 120, 0, 690), (L / 2.0 + 120, 0, 0), 40, steps=8)
    m.box(LAVA, (0, 0, 18), (L - 100, 300, 16), glow=1.2)
    for k in range(3):
        m.sphere(ASH_L, (L / 2.0 - 100 + k * 40, 60, 800 + k * 190), 110 + k * 50, steps=(4, 6))
    m.box(CONC_D, (0, 0, -400), (L + 200, 560, 800))


def observatory(m):
    """The volcano observatory, ruined: a drum on a plinth, the dome split, a piece of it lying below, the telescope's stump."""
    m.cyl(CONC_D, (0, 0, -800), 820, 1160, steps=10)
    m.cyl(CONC, (0, 0, 360), 620, 420, steps=12)
    m.sphere(HUT, (0, 0, 780), 640, scale=(1, 1, 0.9), steps=(6, 12))
    m.box(PANE, (-120, -420, 1050), (130, 520, 620), rot=(0, 0, 0))
    m.sphere(HUT, (-980, -520, 60), 420, scale=(1.0, 0.8, 0.5), steps=(4, 8), rot=(20, 40, 30))
    rod(m, CHAR, (-100, -200, 900), (-380, -720, 1420), 70, steps=8)
    m.box(LIVERY, (0, 0, 420), (1250, 1250, 40))
    m.box(PANE, (-624, 200, 560), (6, 200, 160))
    for k in range(3):
        m.box(PANE, (-820 * math.cos(k * 0.5 - 0.5), -820 * math.sin(k * 0.5 - 0.5) - 40, 160), (8, 130, 180), rot=(0, math.degrees(k * 0.5 - 0.5), 0))
    m.sphere(ASH, (300, 500, -60), 900, scale=(1.2, 0.8, 0.22), steps=(4, 8))
    _root(m, 1150)


def ash_dune(m, seed=3):
    """A big dune of ash on the slope with a roof and the top of a pylon showing out of it."""
    r = random.Random(seed)
    m.sphere(ASH, (0, 0, -120), 1500, scale=(1.3, 0.62, 0.3), steps=(5, 10))
    m.sphere(ASH_L, (-500, 220, -60), 900, scale=(1.3, 0.6, 0.34), steps=(4, 8))
    m.sphere(ASH_L, (700, -200, -100), 800, scale=(1.4, 0.6, 0.3), steps=(4, 8))
    m.box(ASH, (0, 0, -520), (2600, 900, 800))
    m.side(RUST, [(200, 250), (760, 250), (480, 430)], -180, 180, rot=(0, 0, 8))
    tilt_box(m, RUST_D, (-700, 60, 200), (-560, 100, 760), 26, 26)
    tilt_box(m, RUST_D, (-420, 60, 240), (-560, 100, 760), 26, 26)
    m.box(RUST_D, (-560, 100, 700), (260, 14, 14))
    for k in range(5):
        m.box(BASALT, (r.uniform(-1300, 1300), r.uniform(-500, 500), r.uniform(100, 260)), (r.uniform(90, 170), r.uniform(70, 130), 40),
              rot=(r.uniform(-14, 14), r.uniform(0, 90), 0))


# ------------------------------------------------------------------------------------------ the rock
def basalt_columns(m, seed=4, length=3000, rows=3, per_row=13, h0=500, h1=1100, rad=120, rise=260, fan=0.0):
    """A wall of columnar basalt: hexagonal columns in staggered rows, stepping up to the back, talus at its foot."""
    r = random.Random(seed)
    _columns(m, r, -length / 2.0, length / 2.0, -rad * 1.2, rows, per_row, h0, h1, rad, rise, fan)
    for k in range(6):
        x = r.uniform(-length / 2.0, length / 2.0)
        m.cyl(BASALT_L, (x, -rad * 3.2 + r.uniform(-60, 60), -40), rad * 0.8, r.uniform(60, 220), rot=(r.uniform(-30, 30), 0, r.uniform(-20, 20)), steps=6)
    m.sphere(ASH, (0, -rad * 2.6, -60), length * 0.5, scale=(1.0, 0.12, 0.07), steps=(4, 8))
    top = h0 + (rows - 1) * rise - 20.0                   # the wall behind: lower than every column of the back row
    m.box(BASALT, (0, rad * 1.7 * rows, top / 2.0 - 400), (length, rad * 2.5, top + 800))


def fractured_cliff(m, seed=5, length=4000, h=2000):
    """A cliff split into great leaning slabs, fire showing in the joints, ash on the ledges."""
    r = random.Random(seed)
    n = 6
    w = length / float(n)
    for k in range(n):
        x = -length / 2.0 + (k + 0.5) * w
        hh = h * r.uniform(0.6, 1.0)
        m.box(BASALT if k % 2 else BASALT_L, (x, r.uniform(-60, 60), hh / 2.0 - 400), (w * 0.9, r.uniform(420, 640), hh + 800),
              rot=(r.uniform(-7, 7), r.uniform(-6, 6), r.uniform(-4, 4)))
        if k < n - 1:
            lean = r.uniform(-6, 6)
            if k in (1, 3):                                          # fire deep in two of the joints: a crack that wanders, not a bar
                xj = x + w / 2.0
                pts = [(xj + lean * 4, hh * 0.44), (xj + 50, hh * 0.3), (xj - 40, hh * 0.17), (xj + 15, hh * 0.05)]
                for a, b in zip(pts, pts[1:]):
                    _ribbon(m, LAVA, a, b, -62, 26, 40, glow=1.0)
        if k % 2 == 0:
            m.box(ASH, (x, -300, hh * r.uniform(0.3, 0.6)), (w * 0.7, 160, 26))
    m.box('#352a3a', (0, 420, h * 0.35 - 325), (length * 1.02, 520, h * 0.7 + 950))
    for k in range(5):
        m.box(BASALT, (r.uniform(-length / 2.0, length / 2.0), -420 + r.uniform(-80, 80), 30), (r.uniform(120, 260), r.uniform(100, 200), r.uniform(60, 140)),
              rot=(r.uniform(-20, 20), r.uniform(0, 90), 0))


def lava_fall(m, seed=6):
    """A cliff with lava coming over its lip in a fall, a pool crusting over below it."""
    r = random.Random(seed)
    H = 1900
    for s in (-1, 1):
        m.box(BASALT if s < 0 else BASALT_L, (s * 760, 0, H / 2.0 - 400), (1300, 620, H + 800), rot=(0, s * 5, s * 3))
        m.box('#352a3a', (s * 1500, 60, H * 0.36 - 350), (600, 520, H * 0.72 + 900), rot=(0, 0, -s * 6))
    m.box(CHAR, (0, 200, H * 0.45 - 350), (460, 420, H * 0.9 + 900))
    for k in range(7):                                                # broken tops, so the wings are no straight edge
        x = -1350 + k * 450 + r.uniform(-80, 80)
        if abs(x) < 260:
            continue
        hh = r.uniform(160, 520)
        m.box(BASALT_L if k % 2 else BASALT, (x, r.uniform(-40, 80), H + hh / 2.0 - 60), (r.uniform(240, 400), r.uniform(380, 520), hh + 120),
              rot=(r.uniform(-8, 8), r.uniform(-8, 8), r.uniform(-5, 5)))
    zt, zb = H - 60.0, 60.0                                           # second pass: the fall winds down the cleft in three lengths
    pts = [(0.0, zt), (-50.0, zt - (zt - zb) * 0.36), (45.0, zt - (zt - zb) * 0.7), (0.0, zb)]
    for k, (a, b) in enumerate(zip(pts, pts[1:])):
        _ribbon(m, LAVA, a, b, -150 - k * 45, 150 + k * 20, 40, glow=1.5)
        m.box(BASALT, (b[0] + 40, -200 - k * 45, b[1]), (380, 110, 64), rot=(r.uniform(-8, 8), 0, 0))
    _ribbon(m, LAVA_Y, (0.0, zt), (-40.0, zt - (zt - zb) * 0.3), -175, 60, 30, glow=2.0)
    m.box(LAVA, (0, 0, H - 40), (240, 420, 40), glow=1.5)
    m.cyl(LAVA, (0, -520, 8), 430, 20, steps=10, glow=1.4)
    m.cyl(LAVA_Y, (0, -420, 14), 150, 20, steps=8, glow=1.8)
    for k in range(6):
        a = r.uniform(0, 6.28)
        m.box(CHAR, (math.cos(a) * r.uniform(150, 360), -520 + math.sin(a) * r.uniform(150, 360), 30), (r.uniform(70, 150), r.uniform(60, 110), 18),
              rot=(0, r.uniform(0, 90), 0))
    m.cyl(BASALT, (0, -520, -800), 560, 810, steps=10)


def rock_stack(m, seed=7, tall=1.0):
    """Heat-stained stacks: discs of rock piled up, basalt below, burnt red, ochre and sulphur yellow above."""
    r = random.Random(seed)
    bands = [BASALT, BASALT_L, STAIN_R, STAIN_O, STAIN_R, SULPH, BASALT_L]
    for j, (x, y, hk) in enumerate(((0, 0, 1.0), (430, 150, 0.62), (-380, 220, 0.78))):
        z = -160.0
        rad = r.uniform(190, 250) * (0.8 + 0.2 * hk)
        n = int(7 * hk * tall) + 2
        m.cyl(BASALT, (x, y, -800), rad, 660, steps=7)
        for k in range(n):
            hh = r.uniform(110, 210) * tall
            m.cyl(bands[(k + j) % len(bands)], (x + r.uniform(-24, 24), y + r.uniform(-24, 24), z), rad * r.uniform(0.82, 1.1), hh, steps=7,
                  top_r=rad * r.uniform(0.7, 0.98))
            z += hh
            rad *= 0.93
        m.cyl(BASALT, (x, y, z), rad * 1.25, 70, steps=7, top_r=rad * 0.6)
    m.sphere(ASH, (0, 80, -60), 620, scale=(1.2, 0.9, 0.16), steps=(4, 8))


def terrace(m, seed=8):
    """Ash terraces stepping up the slope behind dark retaining banks, survey stakes along them."""
    r = random.Random(seed)
    for k in range(3):
        y = k * 620
        z = k * 150
        m.box(BASALT if k % 2 else '#352a3a', (r.uniform(-150, 150), y - 300, z / 2.0 - 325), (4200 - k * 500, 60, z + 950))
        m.box(ASH if k % 2 else ASH_L, (0, y, (z + 140) / 2.0 - 400), (4200 - k * 500, 620, z + 940))
        for j in range(3):
            m.cyl(STAIN_O, (-1500 + j * 1500 + r.uniform(-200, 200), y - 250, z + 140), 6, 150, steps=5)
    m.box(BASALT, (0, 1560, -150), (3000, 60, 1300))


# ------------------------------------------------------------------------------------------ the horizon (from z = 0 up)
def rim_wall(m, seed=9, length=26000, h=15000, notch=False):
    """A length of the caldera's rim: three jagged walls one behind another, lava in the seams of the nearest."""
    r = random.Random(seed)
    m.side(BASALT, _jag(r, -length / 2.0, length / 2.0, 7, h * 0.62, h, -3000), 1200, 2600)
    m.side(BASALT_L, _jag(r, -length * 0.46, length * 0.46, 9, h * 0.38, h * 0.68, -3000), -200, 1200)
    m.side(CHAR, _jag(r, -length * 0.42, length * 0.42, 11, h * 0.16, h * 0.4, -3000), -1500, -200)
    for k in range(4):
        x = r.uniform(-length * 0.36, length * 0.36)
        top, dx = h * r.uniform(0.16, 0.24), r.uniform(-900, 900)
        if k % 2 == 0:                                               # two thin seams, low: embers, not beacons; they wander
            pts = [(x, top * 0.7), (x + dx * 0.5 + 300, top * 0.45), (x + dx * 0.4 - 200, top * 0.22), (x + dx, -200.0)]
            for a, b in zip(pts, pts[1:]):
                _ribbon(m, LAVA, a, b, -1520, 70, 30, glow=1.0)
    if notch:                                                        # lava over the back wall's lip, down behind the middle one
        _cascade(m, r, r.uniform(-3000, 3000), 1150, h * 0.8, h * 0.36, 520, glow=1.2)


def cinder_cone(m, seed=10, R=9000, H=10500):
    """A cinder cone with fire in its throat, three runnels down the side that faces the road, ash standing over it."""
    r = random.Random(seed)
    m.cyl(BASALT, (0, 0, -2500), R * 1.25, 2500 + H * 0.62, steps=10, top_r=R * 0.45)
    m.cyl(BASALT_L, (0, 0, H * 0.62), R * 0.45, H * 0.38, steps=10, top_r=R * 0.2)
    m.cyl(LAVA, (0, 0, H - 30), R * 0.21, 60, steps=10, glow=2.0)
    m.cyl(YEL, (0, 0, H), R * 0.12, 60, steps=10, glow=2.6)
    for k in range(3):
        a = math.radians(-125 + k * 35 + r.uniform(-8, 8))
        top = (R * 0.2 * math.cos(a), R * 0.2 * math.sin(a), H * 0.98)
        mid = (R * 0.46 * math.cos(a), R * 0.46 * math.sin(a), H * 0.62)
        bot = (R * 1.0 * math.cos(a + 0.12), R * 1.0 * math.sin(a + 0.12), H * 0.08)
        tilt_box(m, LAVA, top, mid, 110, 60, glow=1.4)
        tilt_box(m, LAVA, mid, bot, 140, 60, glow=1.4)
    for k in range(4):
        m.sphere(ASH if k % 2 else '#43384a', (k * 500, k * 300, H + 1600 + k * 1500), 1900 + k * 600, steps=(5, 8))


def far_lava_falls(m, seed=11, length=24000, h=14000):
    """A far wall of cliff with three lava falls down its face into burning pools."""
    r = random.Random(seed)
    top = [(-length / 2.0, -3000), (length / 2.0, -3000), (length / 2.0, h * 0.7), (length * 0.34, h * 0.95), (length * 0.2, h * 0.9),
           (length * 0.08, h), (-length * 0.1, h * 0.92), (-length * 0.24, h * 0.98), (-length * 0.38, h * 0.8), (-length / 2.0, h * 0.6)]
    m.side(BASALT, top, 0, 1800)
    m.side(BASALT_L, _jag(r, -length * 0.5, length * 0.5, 8, h * 0.25, h * 0.5, -3000), -1400, 0)
    for k, x in enumerate((-length * 0.26, length * 0.04, length * 0.3)):
        hh = h * (0.86, 0.94, 0.8)[k]
        _cascade(m, r, x, -60, hh * 0.99, h * 0.16, 400, glow=1.4)
        m.box(LAVA, (x - 300, -1500, h * 0.12), (900, 200, 300), glow=1.4)     # the pool it burns in: two unequal lobes
        m.box(LAVA, (x + 420, -1560, h * 0.105), (600, 200, 220), glow=1.4)
        m.sphere('#43384a', (x + 400, -1700, h * 0.2 + 900), 1300, steps=(4, 7))


def pylon_ridge(m, seed=12, length=30000, h=6500):
    """A dark ridge with a power line marching along its crest: five pylons, the cables sagging between, beacons."""
    r = random.Random(seed)
    m.side(BASALT, _jag(r, -length / 2.0, length / 2.0, 6, h * 0.7, h, -3000), 0, 1600)
    m.side(CHAR, _jag(r, -length * 0.45, length * 0.45, 8, h * 0.3, h * 0.6, -3000), -1300, 0)
    tops = []
    for k in range(5):
        x = -length * 0.4 + k * length * 0.2
        z = h * 0.86
        ph = 4200
        for s in (-1, 1):
            tilt_box(m, CONC_D, (x + s * 520, 800, z), (x + s * 90, 800, z + ph), 110, 110)
        m.box(CONC_D, (x, 800, z + ph * 0.72), (1500, 90, 90))
        m.box(CONC_D, (x, 800, z + ph * 0.96), (1000, 90, 90))
        m.sphere(RED, (x, 800, z + ph + 120), 120, steps=(4, 6), glow=1.6)
        tops.append((x, z + ph * 0.72))
    for k in range(4):
        (x0, z0), (x1, z1) = tops[k], tops[k + 1]
        xm, zm = (x0 + x1) / 2.0, (z0 + z1) / 2.0 - 500
        for s in (-1, 1):
            tilt_box(m, CHAR, (x0 + s * 700, 800, z0), (xm + s * 700, 800, zm), 40, 40)
            tilt_box(m, CHAR, (xm + s * 700, 800, zm), (x1 + s * 700, 800, z1), 40, 40)


# ------------------------------------------------------------------------------------------ second pass (15 Nov 2026)
# The mine: what the level's shot asks for (conveyors, crushers, tips, a rail spur, pump houses), the ground between
# the road and the lava, things that stand in the lava, and stacks tall enough to show over a banked bend's cutting.
def _crescent(cx, cy, R, squash=0.7, n=9):
    out = [(cx + math.cos(math.radians(a)) * R, cy + math.sin(math.radians(a)) * R * squash) for a in [-115 + 230.0 * k / (n - 1) for k in range(n)]]
    inn = [(cx - R * 0.45 + math.cos(math.radians(a)) * R * 0.86, cy + math.sin(math.radians(a)) * R * 0.8 * squash)
           for a in [100 - 200.0 * k / (n - 3) for k in range(n - 2)]]
    return out + inn


def ash_ripples(m, seed=31):
    """Ash blown into crescents on the flat between the road and the lava: five pale dunes, a paler crest on each, all
    of it under a metre. The ground is black, so this is what gives the verge a floor."""
    r = random.Random(seed)
    for k in range(5):
        cx, cy = -1750 + k * 880 + r.uniform(-150, 150), r.uniform(-420, 420)
        R = r.uniform(430, 620)
        m.plan(PALE, _ccw(_crescent(cx, cy, R)), -40, 50)
        m.plan(PALE_L, _ccw(_crescent(cx + R * 0.16, cy, R * 0.72)), 50, 92)


def ash_tip(m):
    """Spoil tips: cones of pale ash, fresh spoil pale on the biggest, the tipping ramp up its side."""
    m.cyl(PALE, (0, 0, -300), 1500, 1400, steps=9, top_r=260)
    m.cyl(PALE_L, (0, 0, 780), 560, 330, steps=9, top_r=250)
    m.cyl(PALE, (1500, 500, -300), 1000, 920, steps=8, top_r=180)
    m.cyl(ASH_L, (-1300, -300, -300), 800, 640, steps=7, top_r=160)
    tilt_box(m, RUST_D, (-900, -700, 60), (-150, -120, 1120), 90, 60)
    m.box(RUST, (-90, -70, 1160), (300, 220, 90))


def conveyor(m):
    """An inclined ore conveyor on three trestles: the covered belt climbing from its loading hopper to the head
    house, a row of lamps along the gallery (they run up it: animate), a red lamp on the head house."""
    L, z0, z1 = 5600.0, 300.0, 1700.0
    tilt_box(m, CONC, (-L / 2.0, 0, z0), (L / 2.0, 0, z1), 300, 260)
    tilt_box(m, RUST, (-L / 2.0, -156, z0 - 60), (L / 2.0, -156, z1 - 60), 14, 70)
    for k in range(7):
        t = (k + 0.5) / 7.0
        m.box(LIT, (-L / 2.0 + L * t, -164, z0 + (z1 - z0) * t + 50), (150, 6, 70), glow=1.4)
    for t in (0.2, 0.5, 0.8):
        x, z = -L / 2.0 + L * t, z0 + (z1 - z0) * t - 130
        for sd in (-1, 1):
            tilt_box(m, RUST_D, (x + sd * (120 + z * 0.16), 0, -300), (x, 0, z), 70, 70)
        m.box(RUST_D, (x, 0, z * 0.5), (z * 0.2 + 160, 60, 50))
    m.box(RUST, (-L / 2.0 - 100, 0, z0 - 40), (460, 420, 420))
    m.box(CONC_D, (-L / 2.0 - 100, 0, -150), (380, 340, 600))
    m.box(CONC_D, (L / 2.0 + 240, 0, z1 + 40), (640, 460, 520))
    m.box(LIVERY, (L / 2.0 + 240, -234, z1 + 120), (560, 8, 120))
    m.box(CONC_D, (L / 2.0 + 240, 0, (z1 - 220) / 2.0 - 150), (300, 300, z1 - 220 + 300))
    m.sphere(RED, (L / 2.0 + 240, 0, z1 + 330), 22, steps=(4, 6), glow=1.6)


def crusher(m):
    """The crusher house: a tall shed stepped in three, the feed hopper high on one side, a band of lit windows under
    the company's stripe, the dust stack with its lamp, fines spilt at its foot."""
    m.box(CONC_D, (0, 0, 300), (1300, 1000, 1400))
    m.box(RUST_D, (-150, 0, 1300), (900, 900, 600))
    m.box(CONC_D, (-300, 0, 1800), (520, 700, 400))
    m.box(RUST, (520, 0, 1100), (420, 700, 220))
    m.box(LIVERY, (540, 0, 1250), (460, 760, 60), rot=(18, 0, 0))
    m.box(PANE, (0, -503, 620), (1000, 6, 150))
    for k in range(4):
        if k != 2:
            m.box(LIT, (-360 + k * 240, -507, 620), (190, 5, 110), glow=1.4)
    m.box(LIVERY, (0, -503, 880), (1300, 6, 70))
    m.cyl(CONC, (-520, 300, 1000), 70, 1500, steps=7, top_r=50)
    for k in range(2):
        m.cyl(RUST, (-520, 300, 1700 + k * 420), 76, 110, steps=7)
    m.sphere(RED, (-520, 300, 2530), 24, steps=(4, 6), glow=1.6)
    m.sphere(PALE, (300, -650, -40), 520, scale=(1.3, 0.6, 0.3), steps=(4, 7))


def rail_spur(m):
    """Forty-six metres of the mine's railway: pale ballast, the sleepers as one dark strip, two ochre rails."""
    L = 4640.0
    m.box(PALE, (0, 0, 0), (L, 420, 110))
    m.box('#4c3a30', (0, 0, 58), (L, 250, 14))
    for sd in (-1, 1):
        m.box(STAIN_O, (0, sd * 72, 74), (L, 14, 18))


def ore_train(m):
    """A slag train standing on the spur: a small diesel in the company's orange and four tubs of slag still glowing
    (it swells and dims: animate)."""
    m.box(CHAR, (-1300, 0, 110), (540, 200, 70))
    m.box(LIVERY, (-1300, 0, 240), (520, 230, 190))
    m.box(LIVERY, (-1460, 0, 400), (200, 230, 150))
    m.box(PANE, (-1460, -117, 410), (150, 6, 80))
    m.cyl(CHAR, (-1160, 0, 335), 30, 110, steps=6)
    for k in range(4):
        x = -700.0 + k * 440.0
        m.box(CHAR, (x, 0, 110), (360, 190, 60))
        m.side(RUST, _ccw([(x - 170, 140), (x + 170, 140), (x + 205, 330), (x - 205, 330)]), -120, 120)
        m.sphere(LAVA, (x, 0, 330), 100, scale=(1.5, 0.95, 0.5), steps=(4, 7), glow=1.0)


def pump_house(m):
    """A pump house on a lava channel's bank: a block under a rust roof, a lit window, the flywheel on its end wall,
    and a pipe out of each side (one down into the lava, one away over the ground: it stands on either bank)."""
    m.box(CONC_D, (0, 0, 50), (700, 520, 900))
    m.box(RUST, (0, 0, 530), (760, 580, 60))
    m.box(PANE, (0, -263, 290), (300, 6, 130))
    m.box(LIT, (0, -267, 290), (260, 5, 96), glow=1.4)
    rod(m, RUST, (355, 0, 260), (378, 0, 260), 170, steps=10)
    for sd in (-1, 1):
        rod(m, STAIN_O, (-120, sd * 250, 200), (-120, sd * 720, 200), 46, steps=8)
        rod(m, STAIN_O, (-120, sd * 720, 200), (-120, sd * 720, -120), 46, steps=8)
        m.box(CONC_D, (-120, sd * 500, 80), (90, 60, 190))


def lava_stones(m, seed=34):
    """Basalt stepping stones standing in the lava lake under a jump: a field of squat hexagonal columns, z = 0 at the
    lava (the table stands it there). They go 9 m down: the lake's bed falls steeply along the road."""
    r = random.Random(seed)
    cols = [BASALT, BASALT_L, '#352a3a']
    for k in range(18):
        m.cyl(cols[k % 3], (r.uniform(-1900, 1900), r.uniform(-1300, 1300), -900), r.uniform(150, 300), 900 + r.uniform(60, 420), steps=6)


def vent_stacks(m):
    """Three vent stacks of the geothermal field, banded and capped, footed 12 m down (they stand along the banked
    bends, over the cutting): tall enough to show against the sky from down in the bend. Nothing lit on them."""
    for k in range(3):
        x = (k - 1) * 1300.0
        H = 1500.0 + (k % 2) * 300
        m.cyl(PALE, (x, 0, -1200), 95, H + 1200, steps=7, top_r=70)       # ash-grey, dark bands (pale concrete with rust bands read as white candles)
        for j in range(3):
            m.cyl(CHAR, (x, 0, H * (0.42 + 0.2 * j)), 100 - j * 8, 110, steps=7)
        m.cyl(CHAR, (x, 0, H), 80, 70, steps=7, top_r=110)


def lava_rafts(m, seed=35, left=False):
    """What makes a lava channel read as flowing: rafts of dark crust riding on it, and bright chevrons of flow between
    them that light runs along (animate). The table lays it on a channel's middle; the lava stands 40 cm over z = 0.
    The chevrons point down the road (+x). left: the twin for the left-hand channel (SM_LavaRaftsL): the table turns a
    left-hand piece round, so it is drawn turned round here and both channels flow the same way."""
    if left:
        m.base = ft_art._xf((0, 0, 0), (0, 180, 0))
    r = random.Random(seed)
    for k in range(6):
        cx, cy = -1900 + k * 760 + r.uniform(-120, 120), r.uniform(-330, 330)
        m.plan(CHAR if k % 2 else BASALT, _ccw(_oval(r.uniform(200, 330), r.uniform(130, 210), 6, cx, cy, r.uniform(0, 1))), 20, 66)
    for k in range(5):
        x, y = -1600 + k * 800 + r.uniform(-100, 100), r.uniform(-300, 300)
        for sd in (-1, 1):
            tilt_box(m, LAVA_Y, (x, y + sd * 240, 48), (x + 320, y, 48), 60, 8, glow=1.8)


def animate():
    """What moves (ft_life.animate puts M_Life on the named material slots and saves the mesh in place). Run it after
    every export of one of these pieces: the export puts the plain palette back.
      the falls      light runs DOWN every lava fall and seam on the far walls, the rim and the fall cliffs
      the channels   light runs ALONG the chevrons of SM_LavaRafts and its rafts of crust drift a couple of metres
                     back and forth, slowly, so the channel reads as flowing
      the mine       the conveyor's lamps run up its gallery; the slag in the tubs swells and dims
      beacons        the red lamps on masts, towers, rigs, pads, pylons and the mine blink, each on its own clock"""
    import ft_life
    fall = dict(LightAxis=(0.0, 0.0, -1.0), LightRate=0.3, LightDuty=0.6, LightSoft=0.3, LightMin=0.5)
    n = ft_life.animate(DIR + 'SM_RidgeFalls', 'Light', [LAVA + '*', LAVA_Y + '*'], LightLen=3200.0, **fall)
    n += ft_life.animate(DIR + 'SM_RidgeRim2', 'Light', [LAVA + '*', LAVA_Y + '*'], LightLen=3200.0, **fall)
    n += ft_life.animate(DIR + 'SM_RidgeCaldera', 'Light', [LAVA + '*'], LightLen=2400.0, **fall)
    n += ft_life.animate(DIR + 'SM_CragFallA', 'Light', [LAVA + '*', LAVA_Y + '*'], LightLen=700.0, **fall)
    for name, ax in (('SM_LavaRafts', 1.0), ('SM_LavaRaftsL', -1.0)):
        n += ft_life.animate(DIR + name, 'Light', [LAVA_Y + '*'], LightAxis=(ax, 0.0, 0.0), LightLen=800.0, LightRate=0.55, LightDuty=0.5,
                             LightSoft=0.3, LightMin=0.3)
        n += ft_life.animate(DIR + name, 'Bob', [CHAR, BASALT], BobDir=(ax, 0.0, 0.0), BobAmp=220.0, BobRate=0.06, BobRand=1.0)
    n += ft_life.animate(DIR + 'SM_Conveyor', 'Light', [LIT + '*'], LightAxis=(1.0, 0.0, 0.0), LightLen=1600.0, LightRate=0.6, LightDuty=0.5,
                         LightSoft=0.15, LightMin=0.25)
    n += ft_life.animate(DIR + 'SM_OreTrain', 'Light', [LAVA + '*'], **ft_life.PULSE)
    for name in ('SM_PoleMast', 'SM_TowerSurvey', 'SM_DrillRig', 'SM_Helipad', 'SM_RidgePylons', 'SM_Conveyor', 'SM_Crusher'):
        n += ft_life.animate(DIR + name, 'Light', [RED + '*'], **ft_life.BLINK)
    return n


# ------------------------------------------------------------------------------------------ the cut banks (3.4)
# Inside a banked bend the track cuts the land down to the road's low edge, and that cutting was a black slope a third
# of the frame high. A Bank rule has the track lay these on it (ft_dress_bank.py has the drawing rules: flat, x along
# the road, y up the slope, z out of the bank). The road is cut through old lava: basalt columns stand up the bank,
# their heads at ragged heights, pale ash drifted over the top, sulphur run down one. Unlit (the verge carries no
# light): it reads by its greys against the black ground, like the ash crescents.
import ft_dress_bank as B   # noqa: E402


def cut_basalt(m, seed=0):
    """A deep cutting in the lava field (11 x 9 m as drawn): eight columns in three greys, ash on their heads, a rib
    of sulphur."""
    r = random.Random(seed + 400)
    B.body(m, BASALT_L, B.TALL)
    n = 8
    w = B.L / n
    for i in range(n):
        x = -B.L / 2.0 + w * (i + 0.5)
        head = r.uniform(170.0, 330.0)
        col = (ASH, PALE, ASH_L, ASH, ASH_L, PALE, ASH, ASH_L)[(i + seed) % 8]
        B.block(m, col, x, (head - 436.0) / 2.0, w - 16.0, head + 436.0, (10.0, 22.0, 16.0, 28.0)[(i * 3 + seed) % 4])
    B.band(m, PALE_L, 352.0, 450.0, 30.0, wob=(34.0, 0.0), n=6, seed=seed + 7)
    # what has run down between two columns: ochre on one design, sulphur on the other (the first try's sulphur rib,
    # 80 cm wide on every panel, read as a row of lit yellow bars)
    x = -B.L / 2.0 + w * (3.0 if seed % 2 == 0 else 5.0)
    B.shape(m, STAIN_O if seed % 2 == 0 else SULPH, [(x - 22.0, 120.0), (x + 22.0, 120.0), (x + 9.0, -300.0), (x - 9.0, -300.0)], 32.0)


def cut_ash(m, seed=0):
    """A shallow cutting (11 x 4 m as drawn): ash slumped down the bank, a paler crest under the land's edge, clinker
    at its foot. A step darker than the verge's crescents: it stands as a wall in the frame, not as a floor."""
    r = random.Random(seed + 410)
    B.body(m, ASH_L, B.LOW)
    B.band(m, PALE, -60.0, 200.0, 12.0, wob=(40.0, 0.0), n=6, seed=seed + 3)
    B.band(m, PALE_L, 124.0, 200.0, 20.0, wob=(22.0, 0.0), n=6, seed=seed + 5)
    for k in range(3):
        x = -380.0 + k * 370.0 + r.uniform(-60.0, 60.0)
        m.sphere(BASALT_L if k % 2 else CHAR, (x, -150.0 + r.uniform(-20.0, 20.0), B.FACE + 8.0), 60.0, scale=(1.3, 0.8, 0.4), steps=(3, 6))


JOBS = [
    ('SM_CutBasalt', cut_basalt), ('SM_CutBasalt2', lambda m: cut_basalt(m, seed=3)), ('SM_CutAsh', cut_ash),
    ('SM_AshRipples', ash_ripples), ('SM_AshTip', ash_tip), ('SM_Conveyor', conveyor), ('SM_Crusher', crusher), ('SM_RailSpur', rail_spur),
    ('SM_OreTrain', ore_train), ('SM_PumpHouse', pump_house), ('SM_LavaStones', lava_stones), ('SM_VentStacks', vent_stacks),
    ('SM_LavaRafts', lava_rafts), ('SM_LavaRaftsL', lambda m: lava_rafts(m, left=True)),
    ('SM_AshBank', ash_bank), ('SM_AshBank2', lambda m: ash_bank(m, seed=21)), ('SM_GuardFence', guard_fence), ('SM_PipeRun', pipe_run),
    ('SM_RockRubble', rubble), ('SM_CableReels', cable_reels), ('SM_PoleMarker', marker_post),
    ('SM_TowerSurvey', survey_tower), ('SM_HutSurvey', survey_hut), ('SM_HutSurvey2', survey_hut_round), ('SM_PoleMast', instrument_mast),
    ('SM_Helipad', helipad), ('SM_Gantry', gantry), ('SM_PolePipeBridge', pipe_bridge), ('SM_SluiceGate', sluice_gate),
    ('SM_DrillRig', drill_rig), ('SM_Hauler', hauler), ('SM_CoolingFins', cooling_fins), ('SM_Observatory', observatory),
    ('SM_AshDune', ash_dune), ('SM_AshDune2', lambda m: ash_dune(m, seed=23)),
    ('SM_CragColumns', basalt_columns),
    ('SM_CragColumns2', lambda m: basalt_columns(m, seed=24, length=2600, rows=4, per_row=11, h0=1100, h1=2100, rad=130, rise=340, fan=7.0)),
    ('SM_CragCliff', fractured_cliff), ('SM_CragCliff2', lambda m: fractured_cliff(m, seed=25, length=3400, h=2700)),
    ('SM_CragFallA', lava_fall), ('SM_RockStack', rock_stack), ('SM_RockStack2Z', lambda m: rock_stack(m, seed=27, tall=1.5)),
    ('SM_CragTerrace', terrace),
    ('SM_RidgeCaldera', rim_wall), ('SM_RidgeRim2', lambda m: rim_wall(m, seed=29, length=22000, h=18000, notch=True)),
    ('SM_PeakCinder', cinder_cone), ('SM_PeakCone2AD', lambda m: cinder_cone(m, seed=30, R=6500, H=7600)),
    ('SM_RidgeFalls', far_lava_falls), ('SM_RidgePylons', pylon_ridge),
]
