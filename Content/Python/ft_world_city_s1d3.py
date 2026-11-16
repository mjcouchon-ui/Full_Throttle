"""City Center, Stage 1, builder s1d3: district 3 "The Garages" (2.10 to 3.13 km), the motor quarter.
/Game/World/CityCenter/SM_S1D3_*. Run: import ft_world_city_s1d3 as K; K.run()

Concrete parking decks and workshops: open decks stacked like plates with thick rounded slab edges, helical ramps, saw-tooth
north-light roofs, stair drums, painted bay stripes. Blue and white, silver, light concrete, pale yellow stripes, cyan for steady
lines of light. NO orange and NO green box anywhere (the pickups are an orange crate and a green toolbox), no wrench emblem, no
cone trio, no vehicle bigger than a small parked car, no words.

Where the pieces stand (the table block C:/ftwork/s1/L7_s1d3.inl): the street wall of this level is full (kerb, podiums, the level's
own towers), so the district is built as LOTS behind the first row of towers: on the left 54 to 70 m out (buildings) with a yard
behind them, on the right 100 to 116 m out with a yard in front. Buildings are tall and narrow so they show over and between the
towers and from the banked bends and the jump; the props stand in the yards. THE CORKSCREW STACK stands 148 to 190 m out.
Every piece is an ft_people.Scene: one material (M_People), one section, flat normals, no collision, no LODs.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
BLUE, WHITE, SILVER, CONC, YEL, CYAN = '#3a5fa8', '#fff6ea', '#c9c4d6', '#9a98a8', '#ffe28a', '#2ee6ff'
TYRE, RED, MAG, GLASS, GREY, WOOD = '#26232e', '#ff2d3c', '#ff4fb8', '#233a63', '#3f3d50', '#9a6a44'
CARS = (RED, YEL, CYAN, WHITE, MAG, BLUE)       # never orange, never green


def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def rslab(g, col, cx, cy, z0, z1, L, W, r=120.0, glow=0.0):
    """A slab with rounded corners (12 points), L along X, W along Y."""
    hx, hy = L / 2.0, W / 2.0
    r = min(r, hx - 1.0, hy - 1.0)
    pts = []
    for sx, sy, a0 in ((1, -1, -90), (1, 1, 0), (-1, 1, 90), (-1, -1, 180)):
        for k in range(3):
            a = math.radians(a0 + k * 45)
            pts.append((cx + sx * (hx - r) + r * math.cos(a), cy + sy * (hy - r) + r * math.sin(a)))
    g.plan(col, ccw(pts), z0, z1, glow=glow)


def deck(g, cx, cy, z, L, W, r=160.0, edge=BLUE):
    """One parking deck: a thick white plate, its rounded edge banded."""
    rslab(g, WHITE, cx, cy, z, z + 150, L, W, r)
    rslab(g, edge, cx, cy, z + 50, z + 100, L + 50, W + 50, r + 25)


def car(g, x, y, z, col, s=1.0, across=False):
    """A small parked car, two boxes. across: nose towards the road (-Y)."""
    L, W = 400.0 * s, 180.0 * s
    if across:
        g.box(col, (x, y, z + 45 * s), (W, L, 60 * s))
        g.box(GLASS, (x, y + 20 * s, z + 95 * s), (W * 0.86, L * 0.5, 44 * s))
    else:
        g.box(col, (x, y, z + 45 * s), (L, W, 60 * s))
        g.box(GLASS, (x - 20 * s, y, z + 95 * s), (L * 0.5, W * 0.86, 44 * s))


def car1(g, x, y, z, col, s=1.0):
    """A car seen far off on a deck: one box, nose out."""
    g.box(col, (x, y, z + 55 * s), (190 * s, 420 * s, 110 * s))


def disc(g, col, x, y, z, r, t=10.0, steps=10, glow=0.0):
    """A disc standing up, facing the road: its front face at y, t thick behind it."""
    rod(g, col, (x, y + t, z), (x, y, z), r, steps=steps, glow=glow)


def helix(g, turns, segs, r_in, r_out, pitch, z0, thick, edges, edge_h=110.0, a_start=0.0):
    """A helical ramp as flat steps (plates). edges: the fascia's colour turn by turn. Returns [(angle, z of the deck)]."""
    out = []
    for i in range(turns * segs):
        a0 = a_start + 2.0 * math.pi * i / segs
        a1 = a_start + 2.0 * math.pi * (i + 1) / segs
        z = z0 + pitch * i / float(segs)
        c0, s0, c1, s1 = math.cos(a0), math.sin(a0), math.cos(a1), math.sin(a1)
        ra, rb = r_in, r_out
        g.plan(WHITE, ccw([(ra * c0, ra * s0), (rb * c0, rb * s0), (rb * c1, rb * s1), (ra * c1, ra * s1)]), z, z + thick)
        ra, rb = r_out - 12.0, r_out + 34.0
        g.plan(edges[(i // segs) % len(edges)], ccw([(ra * c0, ra * s0), (rb * c0, rb * s0), (rb * c1, rb * s1), (ra * c1, ra * s1)]),
               z - 12.0, z + thick + edge_h)
        out.append(((a0 + a1) / 2.0, z + thick))
    return out


# ------------------------------------------------------------------------------------------ the hero
def corkscrew_stack(sc):
    """THE CORKSCREW STACK: a free-standing helical car ramp, twelve white-and-blue turns round an open core, small bright cars
    parked nose-out, and on top a giant wheel (silver hub, tyre lying flat) that turns slowly. 35 m across, about 114 m tall."""
    r = random.Random(73)
    g = sc.still()
    g.cyl(CONC, (0, 0, -300), 1760, 330, steps=12)                      # 3 m of footing
    g.cyl(WHITE, (0, 0, 30), 1790, 230, steps=12)                       # the ground drum
    tilt_box(g, WHITE, (0, -2500, 0), (0, -1500, 262), 520, 40)         # the way in, up from the street side
    for k in range(6):                                                  # the open core: six columns and a line of light
        a = math.radians(30 + k * 60)
        g.cyl(SILVER, (820 * math.cos(a), 820 * math.sin(a), 0), 105, 10450, steps=6)
    g.cyl(CYAN, (0, 0, 260), 70, 10200, steps=6, glow=1.2)
    spots = helix(g, 12, 9, 880, 1700, 850, 250, 130, (BLUE, WHITE))
    for i, (a, z) in enumerate(spots):
        if i % 5 in (1, 3):
            ca, sa = math.cos(a), math.sin(a)
            g.box(r.choice(CARS), (1270 * ca, 1270 * sa, z + 80), (560, 250, 150), rot=(0, math.degrees(a), 0))
    zt = 250 + 12 * 850                                                 # 10450: the top deck
    g.cyl(WHITE, (0, 0, zt), 1750, 160, steps=10)
    g.cyl(CYAN, (0, 0, zt + 40), 1790, 50, steps=10, glow=1.0)
    zt += 160
    w = sc.part(P.motion(travel=(-1.0, 0.03), base=(0.0, 0.0)))         # the wheel turns about the tower's axis, once in 33 s
    w.cyl(SILVER, (0, 0, zt), 210, 300, steps=8)
    z = zt + 150
    w.cyl(TYRE, (0, 0, z), 1250, 150, steps=10, top_r=1450)
    w.cyl(TYRE, (0, 0, z + 150), 1450, 300, steps=10)
    w.cyl(TYRE, (0, 0, z + 450), 1450, 150, steps=10, top_r=1250)
    w.cyl(WHITE, (0, 0, z + 600), 1160, 40, steps=10)                   # the white wall
    w.cyl(SILVER, (0, 0, z + 640), 720, 110, steps=10)                  # the hub
    for k in range(5):
        a = k * 72.0
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        w.box(SILVER, (560 * ca, 560 * sa, z + 670), (1040, 190, 70), rot=(0, a, 0))
        w.box(BLUE, (930 * ca, 930 * sa, z + 760), (150, 150, 70), rot=(0, a, 0))          # five wheel nuts
    for k in range(6):                                                  # pale lugs on the tread: the turning reads from the side
        a = 30.0 + k * 60.0
        w.box(SILVER, (1462 * math.cos(math.radians(a)), 1462 * math.sin(math.radians(a)), z + 300), (70, 300, 330), rot=(0, a, 0))
    w.cyl(BLUE, (0, 0, z + 750), 270, 190, steps=8, top_r=200)          # the hub cap
    w.box(YEL, (1020, 0, z + 660), (110, 110, 120))                     # the valve


# ------------------------------------------------------------------------------------------ buildings, left lots (54 to 70 m out)
def deck_garage(sc):
    """A deck garage: ten open decks stacked like plates, thick rounded slab edges, a line of light under each, cars nose-out,
    a blue stair drum at the corner. 18 x 15 m, 89 m to the top deck, the drum's cap at 99 m."""
    r = random.Random(11)
    L, W, PITCH = 1800.0, 1500.0, 860.0
    g = sc.still()
    g.box(CONC, (0, 0, -150), (L - 80, W - 80, 300))
    g.box(SILVER, (0, W / 2.0 - 40, 4300), (L - 300, 40, 8600))          # the back wall
    for x in (-700, 0, 700):
        for y in (-W / 2.0 + 90, W / 2.0 - 110):
            g.box(CONC, (x, y, 4300), (70, 70, 8600))
    for k in range(11):
        z = k * PITCH
        deck(g, 0, 0, z, L, W)
        if k < 10:
            g.box(CYAN, (0, -W / 2.0 + 60, z + PITCH - 14), (L - 400, 12, 18), glow=1.0)
            for x in (-520, 0, 520):
                if r.random() < 0.8:
                    car1(g, x + r.uniform(-50, 50), -W / 2.0 + 340, z + 150, r.choice(CARS), 1.2)
    zt = 10 * PITCH + 150                                               # the top deck: a parapet, bay lines, three cars, two lamps
    g.box(WHITE, (0, -W / 2.0 + 20, zt + 50), (L - 260, 24, 100))
    g.box(WHITE, (0, W / 2.0 - 20, zt + 50), (L - 260, 24, 100))
    g.box(WHITE, (L / 2.0 - 20, 0, zt + 50), (24, W - 260, 100))
    g.box(WHITE, (-L / 2.0 + 20, 100, zt + 50), (24, W - 460, 100))
    for k in range(5):
        g.box(YEL, (-640 + k * 320, 280, zt + 3), (16, 520, 6))
    for k, x in enumerate((-480, -160, 160, 480)):
        if k != 2:
            car(g, x, 300, zt, CARS[(k * 2 + 1) % 6], 1.15, across=True)
    for x in (-520, 520):
        g.cyl(SILVER, (x, -300, zt), 12, 560, steps=6)
        g.box(SILVER, (x, -300, zt + 560), (170, 12, 12))
        for s in (-1, 1):
            g.sphere(CYAN, (x + s * 85, -300, zt + 530), 36, steps=(4, 6), glow=1.2)
    dx, dy = -L / 2.0 - 40, -W / 2.0 + 220                               # the stair drum
    g.cyl(BLUE, (dx, dy, -300), 300, 9950, steps=10)
    for k in range(5):
        g.cyl(WHITE, (dx, dy, 700 + k * 2 * PITCH), 314, 80, steps=10)
    g.cyl(WHITE, (dx, dy, 9650), 345, 60, steps=10)
    g.cyl(WHITE, (dx, dy, 9710), 330, 230, steps=10, top_r=70)
    for k in range(10):
        g.box(YEL, (dx, dy - 300, 420 + k * PITCH), (70, 10, 230), glow=0.9)
    for s in (-1, 1):                                                   # the way in: a blue portal with a pale yellow bar
        g.box(BLUE, (450 + s * 190, -W / 2.0 - 10, 220), (40, 40, 440))
    g.box(BLUE, (450, -W / 2.0 - 10, 460), (460, 46, 60))
    g.box(YEL, (450, -W / 2.0 - 36, 460), (300, 8, 22), glow=0.8)


def body_shop(sc):
    """A body shop on four parking decks: a white hall with roller shutters under a saw-tooth north-light roof, a car raised on a
    lift over the roof, a blue stair tower with white flights. 20 x 14 m, about 58 m."""
    r = random.Random(23)
    L, W, PITCH = 1700.0, 1400.0, 900.0
    g = sc.still()
    g.box(CONC, (0, 0, -150), (L - 80, W - 80, 300))
    for x in (-650, 0, 650):
        for y in (-W / 2.0 + 90, W / 2.0 - 90):
            g.box(CONC, (x, y, 1800), (70, 70, 3600))
    for k in range(5):
        z = k * PITCH
        deck(g, 0, 0, z, L, W)
        if k < 4:
            for x in (-480, 0, 480):
                if r.random() < 0.85:
                    car1(g, x + r.uniform(-40, 40), -W / 2.0 + 330, z + 150, r.choice(CARS), 1.15)
    z0 = 4 * PITCH + 150                                                # the hall
    HL, HW = L - 100, W - 100
    g.box(WHITE, (0, 0, z0 + 450), (HL, HW, 900))
    g.box(BLUE, (0, 0, z0 + 70), (HL + 12, HW + 12, 140))
    for x in (-520, 0, 520):
        g.box(SILVER, (x, -HW / 2.0 - 4, z0 + 420), (380, 10, 560))
        g.box(YEL, (x, -HW / 2.0 - 8, z0 + 170), (380, 10, 44))
    disc(g, BLUE, 0, -HW / 2.0 - 12, z0 + 800, 85, t=12)               # a picture sign: a little white car on a blue disc
    g.box(WHITE, (0, -HW / 2.0 - 15, z0 + 785), (100, 6, 26))
    g.box(WHITE, (-6, -HW / 2.0 - 15, z0 + 810), (54, 6, 24))
    z1 = z0 + 900                                                       # the saw-tooth roof: glass on the upright of each tooth
    tw = HL / 4.0
    for k in range(4):
        x0 = -HL / 2.0 + k * tw
        g.side(SILVER, ccw([(x0, 0.0), (x0 + tw, 0.0), (x0, 320.0)]), -HW / 2.0, HW / 2.0, loc=(0, 0, z1))
        g.box(CYAN, (x0 - 4, 0, z1 + 170), (8, HW - 260, 220), glow=0.9)
    for s in (-1, 1):                                                   # the roof lift, a car up on it
        g.box(YEL, (s * 270, 0, z1 + 450), (44, 44, 900))
    rslab(g, BLUE, 0, 0, z1 + 900, z1 + 934, 660, 330, 60)
    car(g, 0, 0, z1 + 934, RED, 1.25)
    sx = L / 2.0 + 130                                                  # the stair tower
    g.box(BLUE, (sx, 120, (z1 + 200 - 300) / 2.0), (260, 520, z1 + 200 + 300))
    g.box(WHITE, (sx, 120, z1 + 230), (300, 560, 60))
    for k in range(8):
        a, b = (sx - 110, sx + 110) if k % 2 == 0 else (sx + 110, sx - 110)
        tilt_box(g, WHITE, (a, -150, 120 + k * 560), (b, -150, 120 + (k + 1) * 560), 16, 34)


def tyre_depot(sc):
    """A tyre depot: a tower of nine giant tyres, their treads painted white and blue turn about, a blue door in the bottom
    one, portholes, a lamp and a pennant on the hub cap. 13 m across, about 63 m."""
    R, PITCH = 640.0, 640.0
    g = sc.still()
    g.cyl(CONC, (0, 0, -300), 560, 300, steps=10)
    for k in range(9):
        z = k * PITCH
        g.cyl(TYRE, (0, 0, z), 500, 130, steps=10, top_r=R)
        g.cyl(WHITE if k % 2 == 0 else BLUE, (0, 0, z + 130), R, 300, steps=10)
        g.cyl(TYRE, (0, 0, z + 430), R, 130, steps=10, top_r=500)
        g.cyl(SILVER, (0, 0, z + 560), 380, 80, steps=10)
        if k % 2 == 1:
            disc(g, YEL, 0, -R - 8, z + 280, 64, t=40, steps=8, glow=0.9)
    zt = 9 * PITCH
    g.cyl(SILVER, (0, 0, zt), 380, 200, steps=10, top_r=120)
    g.sphere(YEL, (0, 0, zt + 270), 90, steps=(4, 6), glow=1.0)
    g.cyl(SILVER, (0, 0, zt + 340), 7, 240, steps=6)
    g.box(BLUE, (0, -R + 10, 150 + 150), (220, 70, 300))                # the door, its frame and canopy
    g.box(WHITE, (0, -R - 22, 150 + 310), (270, 10, 30))
    for s in (-1, 1):
        g.box(WHITE, (s * 122, -R - 22, 150 + 150), (26, 10, 300))
    g.box(YEL, (0, -R - 70, 150 + 350), (330, 150, 16))
    tilt_box(g, WHITE, (0, -R - 190, 0), (0, -R - 20, 150), 240, 20)     # steps up to it
    f = sc.part(P.motion(swing=14.0, pivot=(0.0, 0.0, zt + 520.0), axis=P.UP, rate=0.4, phase=0.2))
    f.box(BLUE, (90, 0, zt + 525), (170, 5, 70))


# ------------------------------------------------------------------------------------------ buildings, right lots (100 to 116 m out)
def car_wash(sc):
    """A car-wash hall: a white portal under a blue beam, two giant striped roller brushes turning slowly either side of a car,
    a third rocking over it, soap bubbles on the roof. 27 x 14 m, about 62 m."""
    g = sc.still()
    g.box(CONC, (0, 0, -100), (2400, 1300, 400))
    for s in (-1, 1):
        g.box(WHITE, (s * 1050, 0, 2500), (300, 1200, 4800))
        g.box(BLUE, (s * 1050, 0, 700), (312, 1212, 300))
        g.box(BLUE, (s * 1050, 0, 4400), (312, 1212, 300))
    rslab(g, BLUE, 0, 0, 4900, 5400, 2700, 1400, 200)
    rslab(g, WHITE, 0, 0, 5400, 5480, 2500, 1200, 180)
    g.box(CYAN, (0, -560, 4840), (1700, 30, 30), glow=1.0)
    for k in range(5):
        g.box(YEL, (-640 + k * 320, -560, 104), (120, 180, 8))
    car(g, 0, 60, 100, RED, 2.2, across=True)
    for x, y, rr, col, gl in ((-640, 0, 260, WHITE, 0.0), (-170, 110, 380, CYAN, 0.6), (430, -60, 230, WHITE, 0.0), (830, 90, 160, CYAN, 0.6),
                              (150, -260, 140, WHITE, 0.0)):
        g.sphere(col, (x, y, 5480 + rr * 0.8), rr, steps=(5, 8), glow=gl)
    for s in (-1, 1):                                                   # the upright brushes turn on the spot, opposite ways
        b = sc.part(P.motion(travel=(-1.0, s * 0.12), base=(s * 560.0, 0.0)))
        b.cyl(WHITE, (s * 560, 0, 120), 230, 4200, steps=8)
        b.box(BLUE, (s * 560, 0, 2220), (640, 90, 4000))
        b.box(BLUE, (s * 560, 0, 2220), (90, 640, 4000))
        b.cyl(YEL, (s * 560, 0, 4320), 265, 80, steps=8)
    t = sc.part(P.motion(swing=150.0, pivot=(0.0, 0.0, 4560.0), axis=0.0, rate=0.07))     # the top brush rocks about its own axle
    rod(t, WHITE, (-860, 0, 4560), (860, 0, 4560), 180, steps=8)
    t.box(BLUE, (0, 0, 4560), (1600, 80, 520))
    t.box(BLUE, (0, 0, 4560), (1600, 520, 80))


def elevator_tower(sc):
    """A car-elevator tower: an open white shaft, a blue floor every 7 m with a car parked on each third one, and a car riding
    up and down on a platform between two lines of light on the street face. 10 x 12 m, about 100 m."""
    g = sc.still()
    g.box(CONC, (0, 0, -150), (900, 900, 300))
    g.box(BLUE, (0, 0, 200), (860, 860, 400))
    g.box(WHITE, (0, -432, 170), (300, 8, 300))
    H, PITCH = 8600.0, 700.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WHITE, (sx * 380, sy * 380, 400 + H / 2.0), (110, 110, H))
    for k in range(12):
        z = 400 + (k + 1) * PITCH
        g.box(BLUE, (0, 0, z), (860, 860, 50))
        if k % 3 == 1:
            car(g, 0, 0, z + 25, CARS[(k // 3) % 6], 1.4)
    for k in range(4):                                                  # braces up the back
        z = 500 + k * 2100
        tilt_box(g, SILVER, (-380, 400, z), (380, 400, z + 2000), 24, 40)
        tilt_box(g, SILVER, (380, 400, z), (-380, 400, z + 2000), 24, 40)
    for s in (-1, 1):
        g.box(CYAN, (s * 300, -470, 400 + H / 2.0), (40, 40, H), glow=1.1)
    zt = 400 + H
    g.box(WHITE, (0, -60, zt + 250), (1000, 1120, 500))
    rslab(g, BLUE, 0, -60, zt + 500, zt + 560, 1100, 1220, 120)
    rod(g, SILVER, (0, -330, zt + 760), (0, 210, zt + 760), 200, steps=10)                 # the winding drum
    for y in (-380, 260):
        g.box(BLUE, (0, y, zt + 680), (120, 50, 260))
    g.sphere(YEL, (0, -60, zt + 1040), 80, steps=(4, 6), glow=1.0)
    lift = sc.part(P.motion(bob=-3600.0, rate=0.03))                    # it rides 36 m up and 36 m down, 33 s the round
    lift.box(YEL, (0, -600, 4780), (620, 300, 44))
    lift.box(BLUE, (0, -470, 4900), (700, 30, 280))
    car(lift, 0, -600, 4802, MAG, 1.3)


def paint_shop(sc):
    """A paint shop on two parking decks: a white hall with broad paint stripes and five colour swatches across its front, a giant
    roller that has just painted a blue band, five fat extractor cowls and a blue chimney with a slow white puff. 24 x 14 m, 65 m."""
    r = random.Random(41)
    L, W, PITCH = 2400.0, 1400.0, 860.0
    g = sc.still()
    g.box(CONC, (0, 0, -150), (L - 80, W - 80, 300))
    for x in (-1000, 0, 1000):
        for y in (-W / 2.0 + 90, W / 2.0 - 90):
            g.box(CONC, (x, y, 860), (70, 70, 1720))
    for k in range(3):
        deck(g, 0, 0, k * PITCH, L, W)
        if k < 2:
            for x in (-800, -270, 270, 800):
                if r.random() < 0.8:
                    car1(g, x, -W / 2.0 + 330, k * PITCH + 150, r.choice(CARS), 1.15)
    z0 = 2 * PITCH + 150
    HL, HW = L - 100, W - 100
    fy = -HW / 2.0
    g.box(WHITE, (0, 0, z0 + 500), (HL, HW, 1000))
    g.box(BLUE, (0, fy - 4, z0 + 150), (HL + 8, 10, 160))
    g.box(YEL, (0, fy - 4, z0 + 340), (HL + 8, 10, 100))
    g.box(CYAN, (0, fy - 4, z0 + 470), (HL + 8, 10, 70))
    for k, col in enumerate((RED, YEL, CYAN, MAG, BLUE)):
        g.box(col, (-460 + k * 330, fy - 5, z0 + 760), (240, 12, 240))
    g.box(BLUE, (-900, fy - 5, z0 + 700), (420, 12, 420))               # the band the roller has just painted
    rod(g, WHITE, (-1120, fy - 110, z0 + 930), (-680, fy - 110, z0 + 930), 110, steps=8)
    tilt_box(g, SILVER, (-900, fy - 110, z0 + 930), (-700, fy - 180, z0 + 1500), 30, 30)
    z1 = z0 + 1000
    g.box(BLUE, (0, 0, z1 + 30), (HL + 60, HW + 60, 60))
    for k in range(5):
        x = -900 + k * 340
        g.cyl(SILVER, (x, 200, z1 + 60), 110, 260, steps=8)
        g.cyl(WHITE, (x, 200, z1 + 320), 190, 90, steps=8)
        g.cyl(SILVER, (x, 200, z1 + 410), 190, 80, steps=8, top_r=60)
    cx, cy = 950.0, 150.0
    g.cyl(BLUE, (cx, cy, z1), 170, 6200 - z1, steps=10)
    for k in range(3):
        g.cyl(WHITE, (cx, cy, z1 + 700 + k * 1000), 182, 160, steps=10)
    g.cyl(SILVER, (cx, cy, 6200), 205, 90, steps=10)
    p = sc.part(P.motion(swing=2.5, pivot=(cx, cy, z1), axis=0.0, rate=0.1, phase=0.3))
    for dx, dz, rr in ((0, 190, 150), (90, 330, 110)):
        p.sphere(WHITE, (cx + dx, cy, 6140 + dz), rr, steps=(4, 6))


def ramp_drum(sc):
    """A ramp drum: a silver drum with a helical ramp wound six times round it, small cars on the turns, a saucer canopy and a
    lamp. 19 m across, about 60 m."""
    r = random.Random(57)
    g = sc.still()
    g.cyl(CONC, (0, 0, -300), 930, 320, steps=10)
    g.cyl(SILVER, (0, 0, 0), 520, 5450, steps=10)
    for k in range(3):
        g.cyl(YEL, (0, 0, 1500 + k * 1700), 532, 70, steps=10)
    spots = helix(g, 6, 8, 500, 900, 850, 300, 110, (BLUE, WHITE), edge_h=90.0)
    for i, (a, z) in enumerate(spots):
        if i % 4 == 1:
            g.box(r.choice(CARS), (700 * math.cos(a), 700 * math.sin(a), z + 60), (300, 170, 110), rot=(0, math.degrees(a), 0))
    g.cyl(WHITE, (0, 0, 5450), 980, 110, steps=10)
    g.cyl(BLUE, (0, 0, 5560), 980, 230, steps=10, top_r=210)
    g.sphere(YEL, (0, 0, 5860), 105, steps=(4, 6), glow=1.0)
    g.cyl(CYAN, (0, 0, 5400), 990, 40, steps=10, glow=1.0)


# ------------------------------------------------------------------------------------------ props (in the lots' yards, 75 m and more out)
def pay_booth(sc):
    """A pay booth on its island, the barrier arm lifted and staying up."""
    g = sc.still()
    rslab(g, CONC, 0, 0, -40, 18, 620, 300, 80)
    for s in (-1, 1):
        g.box(YEL, (s * 275, 0, 14), (50, 250, 12))
    g.box(WHITE, (-120, 0, 148), (200, 200, 260))
    g.box(BLUE, (-120, 0, 48), (206, 206, 60))
    g.box(YEL, (-120, -101, 196), (150, 6, 84), glow=0.9)
    g.box(YEL, (-19, 0, 196), (6, 130, 84), glow=0.9)
    rslab(g, BLUE, -120, 0, 278, 312, 320, 300, 70)
    g.cyl(CYAN, (-120, 0, 312), 22, 26, steps=6, glow=1.0)
    g.box(SILVER, (70, 0, 78), (40, 40, 120))
    g.box(BLUE, (56, -32, 132), (80, 26, 46))
    tilt_box(g, WHITE, (70, -32, 132), (230, -32, 560), 16, 16)          # the arm, up
    for k in range(3):
        f0, f1 = 0.18 + k * 0.28, 0.30 + k * 0.28
        tilt_box(g, RED, (70 + 160 * f0, -32, 132 + 428 * f0), (70 + 160 * f1, -32, 132 + 428 * f1), 20, 20)
    g.box(BLUE, (200, -70, 83), (44, 44, 130))                          # the ticket post
    g.box(YEL, (200, -93, 120), (30, 5, 26))


def tyre_stack(sc):
    """Stacks of tyres on a pallet, one leaning, and a jack trolley."""
    g = sc.still()
    g.box(WOOD, (-40, 0, 6), (250, 120, 12))
    for x, y, n in ((-120, 0, 5), (-30, 8, 4), (55, -6, 3)):
        for k in range(n):
            g.cyl(TYRE if k % 2 == 0 else GREY, (x, y, 12 + k * 22), 40, 22, steps=8)
        g.cyl(WHITE, (x, y, 12 + n * 22 - 1), 22, 4, steps=8)
    rod(g, TYRE, (150, 40, 40), (150, 18, 44), 40, steps=8)              # one tyre leaning on the pallet
    rod(g, WHITE, (150, 18, 44), (150, 14, 44.7), 22, steps=8)
    g.box(BLUE, (260, -20, 14), (130, 46, 16))                          # the jack trolley
    tilt_box(g, SILVER, (215, -20, 22), (300, -20, 48), 30, 10)
    g.cyl(SILVER, (300, -20, 46), 14, 6, steps=6)
    tilt_box(g, SILVER, (200, -20, 20), (150, -20, 100), 5, 5)
    g.box(YEL, (148, -20, 102), (8, 30, 8))


def bollard_row(sc):
    """Six white bollards with blue caps and a chain."""
    g = sc.still()
    g.box(CONC, (0, 0, 3), (700, 44, 6))
    xs = [-275 + k * 110 for k in range(6)]
    for x in xs:
        g.cyl(WHITE, (x, 0, 0), 13, 88, steps=6)
        g.cyl(BLUE, (x, 0, 88), 16, 14, steps=6)
    for a, b in zip(xs[:-1], xs[1:]):
        m = (a + b) / 2.0
        tilt_box(g, SILVER, (a, 0, 78), (m, 0, 60), 5, 5)
        tilt_box(g, SILVER, (m, 0, 60), (b, 0, 78), 5, 5)


def hoist_frame(sc):
    """A tow hoist on an A-frame, an engine block hanging from its chain (no vehicle)."""
    g = sc.still()
    rslab(g, CONC, 0, 0, -30, 10, 640, 420, 60)
    for x in (-250, 250):
        for s in (-1, 1):
            tilt_box(g, BLUE, (x, s * 170, 0), (x, 0, 520), 24, 24)
        g.box(SILVER, (x, 0, 250), (16, 190, 16))
    g.box(SILVER, (0, 0, 528), (580, 28, 30))
    g.box(YEL, (0, 0, 500), (64, 44, 28))
    g.box(SILVER, (0, 0, 400), (6, 6, 180))
    g.box(YEL, (0, 0, 304), (16, 10, 20))
    g.box(GREY, (0, 0, 250), (96, 62, 74))                              # the engine block
    g.box(SILVER, (0, 0, 296), (100, 50, 20))
    for k in range(4):
        g.cyl(SILVER, (-33 + k * 22, 0, 306), 8, 14, steps=6)
    disc(g, BLUE, -20, -36, 246, 22, t=6, steps=8)
    disc(g, YEL, 26, -36, 262, 12, t=6, steps=6)
    for s in (-1, 1):
        g.box(YEL, (s * 300, -150, 20), (30, 60, 22))
    g.box(BLUE, (150, 120, 40), (90, 50, 60))                           # a parts bin and an oil pan beside it
    g.cyl(SILVER, (-140, 110, 10), 34, 10, steps=8)


def oil_drums(sc):
    """Oil drums in blue and white on a drip tray, a hand pump in one, a funnel on another."""
    g = sc.still()
    g.box(CONC, (0, 0, 4), (300, 190, 8))
    spots = ((-100, -40), (-36, -46), (30, -40), (96, -44), (-70, 30), (-4, 36), (62, 30))
    for k, (x, y) in enumerate(spots):
        g.cyl(BLUE if k % 2 == 0 else WHITE, (x, y, 8), 29, 88, steps=8)
        g.cyl(SILVER, (x, y, 36), 31, 6, steps=8)
        g.cyl(SILVER, (x, y, 66), 31, 6, steps=8)
    x, y = spots[5]
    g.box(SILVER, (x, y, 126), (8, 8, 60))
    tilt_box(g, SILVER, (x, y, 150), (x + 34, y, 138), 6, 6)
    tilt_box(g, YEL, (x, y, 152), (x - 30, y, 172), 6, 6)
    x, y = spots[0]
    g.cyl(YEL, (x, y, 96), 6, 22, steps=6, top_r=22)
    g.cyl(WHITE, (140, 50, 8), 16, 34, steps=6)                         # a pail


def height_gantry(sc):
    """A height bar: blue posts, a pale yellow bar banded blue, striped flaps hanging from it that swing, a round sign on top."""
    g = sc.still()
    for s in (-1, 1):
        g.box(CONC, (s * 350, 0, 12), (90, 90, 24))
        g.box(BLUE, (s * 350, 0, 222), (30, 30, 420))
    g.box(YEL, (0, 0, 432), (760, 26, 36))
    for k in range(4):
        g.box(BLUE, (-270 + k * 180, 0, 432), (60, 30, 40))
    disc(g, BLUE, 0, -8, 510, 56, t=16, steps=10)
    g.box(YEL, (0, -11, 510), (76, 5, 14))
    for k in range(7):
        f = sc.part(P.motion(swing=9.0, pivot=(0.0, 0.0, 414.0), axis=0.0, rate=0.45, phase=0.0 if k % 2 == 0 else 0.35))
        f.box(WHITE if k % 2 == 0 else BLUE, (-270 + k * 90, 0, 370), (60, 5, 88))


def bay_sign(sc):
    """A blade sign on a post, pictures only: a little car and an arrow pointing down, a line of light on top."""
    g = sc.still()
    g.box(CONC, (0, 0, 14), (80, 80, 28))
    g.box(SILVER, (0, 0, 320), (24, 24, 600))
    g.box(BLUE, (92, 0, 470), (160, 22, 420))
    g.box(CYAN, (92, 0, 686), (164, 26, 12), glow=1.0)
    for s in (-1, 1):
        y = s * 13
        g.box(WHITE, (92, y, 590), (104, 5, 28))
        g.box(WHITE, (86, y, 616), (56, 5, 24))
        g.box(YEL, (92, y, 440), (18, 5, 110), glow=0.9)
        tilt_box(g, YEL, (56, y, 400), (92, y, 348), 5, 18, glow=0.9)
        tilt_box(g, YEL, (128, y, 400), (92, y, 348), 5, 18, glow=0.9)


def tool_cart(sc):
    """A blue drawer cart, a silver air tank with its hose reel, a creeper board."""
    g = sc.still()
    g.box(BLUE, (-60, 0, 62), (112, 52, 76))
    for k in range(3):
        g.box(WHITE, (-60, -28, 40 + k * 22), (96, 5, 5))
    g.box(SILVER, (-60, 0, 103), (120, 58, 6))
    g.box(YEL, (-90, 6, 114), (30, 20, 16))
    for s in (-1, 1):
        g.box(GREY, (-60 + s * 44, 0, 12), (14, 44, 24))
    rod(g, SILVER, (40, 10, 42), (150, 10, 42), 26, steps=8)             # the air tank
    for x in (58, 132):
        g.box(GREY, (x, 10, 9), (12, 46, 18))
    g.box(BLUE, (95, 10, 80), (54, 36, 26))
    disc(g, YEL, 95, -22, 50, 20, t=8, steps=8)
    g.cyl(RED, (188, -8, 0), 9, 46, steps=6)
    g.box(WOOD, (95, -80, 9), (110, 44, 6))                             # the creeper board
    g.box(GREY, (95, -80, 3), (90, 30, 6))


def arrow_sign(sc):
    """A way-in sign for the lots: a blue disc rimmed white on a white post, a pale yellow arrow lit on both faces. 15 m."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -100), 90, 140, steps=8)
    g.cyl(WHITE, (0, 0, 40), 40, 1010, steps=6)
    for k in range(2):
        g.cyl(BLUE, (0, 0, 300 + k * 400), 46, 60, steps=6)
    rod(g, WHITE, (0, 12, 1250), (0, -12, 1250), 290, steps=12)
    rod(g, BLUE, (0, 22, 1250), (0, -22, 1250), 262, steps=12)
    for y0, y1 in ((-30, -22), (22, 30)):
        g.box(YEL, (-50, (y0 + y1) / 2.0, 1250), (230, 8, 64), glow=1.0)
        g.side(YEL, ccw([(50.0, -112.0), (190.0, 0.0), (50.0, 112.0)]), y0, y1, glow=1.0, loc=(0, 0, 1250))


def deck_lamps(sc):
    """Three top-deck lamp masts on a striped kerb, two cyan globes each (steady)."""
    g = sc.still()
    rslab(g, CONC, 0, 0, -30, 30, 820, 150, 50)
    for k in range(4):
        g.box(YEL if k % 2 == 0 else BLUE, (-300 + k * 200, -76, 16), (100, 6, 22))
    for x in (-300, 0, 300):
        g.cyl(SILVER, (x, 0, 30), 13, 1070, steps=6)
        g.cyl(BLUE, (x, 0, 30), 22, 90, steps=6)
        g.box(SILVER, (x, 0, 1100), (180, 14, 14))
        for s in (-1, 1):
            g.box(SILVER, (x + s * 86, 0, 1082), (8, 8, 30))
            g.sphere(CYAN, (x + s * 86, 0, 1040), 36, steps=(4, 6), glow=1.2)


def car_stack(sc):
    """A car rack: three shelves of three small bright cars, nose-out, in a silver frame under a white roof. 9.3 m."""
    g = sc.still()
    g.box(CONC, (0, 0, -40), (960, 320, 120))
    for x in (-450, -150, 150, 450):
        for y in (-130, 130):
            g.box(SILVER, (x, y, 470), (22, 22, 900))
    k = 0
    for lv in range(3):
        z = 20 + lv * 290
        g.box(BLUE, (0, 0, z + 8), (930, 290, 16))
        for x in (-300, 0, 300):
            car(g, x, 0, z + 16, CARS[(k * 5 + lv) % 6], 0.64, across=True)
            k += 1
    rslab(g, WHITE, 0, 0, 900, 930, 1000, 360, 60)
    g.box(YEL, (0, -176, 912), (700, 8, 14), glow=0.9)
    g.box(BLUE, (505, 0, 300), (60, 200, 600))                          # the lift mast at its end
    g.box(YEL, (505, -104, 520), (40, 8, 60))


def car_vane(sc):
    """A weather vane on a tall mast: a little red car on an arrow, turning slowly to and fro; plain balls at the four points."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -60), 60, 100, steps=8)
    g.cyl(SILVER, (0, 0, 40), 14, 1460, steps=6)
    g.cyl(BLUE, (0, 0, 40), 26, 120, steps=6)
    g.box(SILVER, (0, 0, 1380), (240, 10, 10))
    g.box(SILVER, (0, 0, 1380), (10, 240, 10))
    for dx, dy in ((120, 0), (-120, 0), (0, 120), (0, -120)):
        g.sphere(WHITE, (dx, dy, 1380), 16, steps=(3, 5))
    g.sphere(YEL, (0, 0, 1440), 24, steps=(4, 6))
    v = sc.part(P.motion(swing=22.0, pivot=(0.0, 0.0, 1500.0), axis=P.UP, rate=0.07))
    v.box(BLUE, (0, 0, 1510), (320, 10, 12))
    v.box(WHITE, (-150, 0, 1530), (70, 6, 70))
    v.side(BLUE, ccw([(150.0, -28.0), (210.0, 0.0), (150.0, 28.0)]), -5, 5, loc=(0, 0, 1510))
    v.box(RED, (10, 0, 1548), (150, 44, 36))
    v.box(WHITE, (4, 0, 1580), (80, 40, 28))
    for x in (-35, 55):
        rod(v, TYRE, (x, 25, 1532), (x, -25, 1532), 16, steps=8)


JOBS = [
    ('SM_S1D3_CorkscrewStack', corkscrew_stack, dict(far=True)),
    ('SM_S1D3_DeckGarage', deck_garage, STILL),
    ('SM_S1D3_BodyShop', body_shop, STILL),
    ('SM_S1D3_TyreDepot', tyre_depot),
    ('SM_S1D3_CarWash', car_wash, dict(far=True)),
    ('SM_S1D3_ElevatorTower', elevator_tower, dict(far=True)),
    ('SM_S1D3_PaintShop', paint_shop),
    ('SM_S1D3_RampDrum', ramp_drum, STILL),
    ('SM_S1D3_PayBooth', pay_booth, STILL),
    ('SM_S1D3_TyreStack', tyre_stack, STILL),
    ('SM_S1D3_BollardRow', bollard_row, STILL),
    ('SM_S1D3_HoistFrame', hoist_frame, STILL),
    ('SM_S1D3_OilDrums', oil_drums, STILL),
    ('SM_S1D3_HeightGantry', height_gantry),
    ('SM_S1D3_BaySign', bay_sign, STILL),
    ('SM_S1D3_ToolCart', tool_cart, STILL),
    ('SM_S1D3_ArrowSign', arrow_sign, STILL),
    ('SM_S1D3_DeckLamps', deck_lamps, STILL),
    ('SM_S1D3_CarStack', car_stack, STILL),
    ('SM_S1D3_CarVane', car_vane),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
