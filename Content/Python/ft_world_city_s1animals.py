"""City Center, Stage 1, builder s1animals: the animals of the eight districts. /Game/World/CityCenter/SM_S1ANIMALS_*.
Run: import ft_world_city_s1animals as K; K.run()

Where they stand (the table block C:\\ftwork\\s1\\L7_s1animals.inl): this avenue is a canyon, its ground from 25.5 m out is
podiums and towers and its kerb pockets are full, so the small animals stand where a city's animals do: on the FRONT
EDGE of the podiums' roofs (a strip 90 cm deep behind the cornice: every roof piece here is drawn inside y -45..+75),
on the service yard's loading dock, on the water round the Plaza's monument and on the ground behind it. The doves fly
135 m over the road's level, 100 m and more out.

Every piece: ft_people.Scene, one section, M_People (the doves M_PeopleFar). Movement only by the builders' motion
values. Nothing here is orange or green and box-shaped, nothing hovers, nothing circles near the road.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'

# the kit's own palette (ft_dress_metro.py)
BRICK, BRICK_D, TAN, STONE, STONE_L = '#6a3f44', '#54343c', '#8a6f5c', '#706e80', '#9a98a8'
CYAN, WARM, WARM_L, AMBER, MAG, RED = '#2ee6ff', '#ffe28a', '#fff1c0', '#ffd23f', '#ff4fb8', '#ff2d3c'
CREAM, WHITE, TEAL, GREEN_D, PINE, BLUE, WOOD, WOOD_D = '#fff1d6', '#fff6ea', '#2fc6c0', '#24403a', '#2f6b52', '#3a5fa8', '#9a6a44', '#6a4c36'
SILVER, GOLD, BLACK, GREY_D, SAND = '#c9c4d6', '#ffc93c', '#1c1a26', '#3f3d50', '#b89a6a'
BLUSH, LILAC, VIOLET, AQUA, PEACH, STEEL_B, PINK, RUST = '#f3c4dc', '#c391d8', '#8662b8', '#9ff0ff', '#ffc89a', '#2a3a54', '#ff5fa2', '#c94f2e'
EYE = '#22202a'


class Fr(object):
    """A creature's own frame in the piece: at (x, y, z), turned by yaw (front = -Y at yaw 0), k times life size."""

    def __init__(self, sc, x, y, z=0.0, yaw=0.0, k=1.0):
        self.sc, self.at, self.yaw, self.k = sc, (x, y, z), yaw, k
        self.c, self.s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(self, p):
        x, y, z = self.at
        return (x + self.k * (p[0] * self.c - p[1] * self.s), y + self.k * (p[0] * self.s + p[1] * self.c), z + self.k * p[2])

    def still(self):
        return self.sc.still(self.at, self.yaw, self.k)

    def part(self, pivot=(0, 0, 0), axis=0.0, **mo):
        """pivot in the creature's frame; axis: degrees from the creature's own X in the ground plane, or P.UP."""
        return self.sc.part(P.motion(pivot=self.W(pivot), axis=(P.UP if axis == P.UP else self.yaw + axis), **mo), self.at, self.yaw, self.k)


# ------------------------------------------------------------------------------------------ small creatures, plain shapes
def perch(sc, x, y, z, yaw=0.0, k=1.0, body=TAN, wing=WOOD_D, beak=AMBER, bib=None, legs=False, phase=0.0, rate=1.8, bob=0.0, rock=0.0):
    """A small bird standing (25 cm at k 1): it hops (bob) or tips forward and back (rock)."""
    f = Fr(sc, x, y, z, yaw, k)
    m = f.part(pivot=(0, 0, 0), axis=0.0, phase=phase, rate=rate, bob=bob, swing=rock)
    m.sphere(body, (0, 2, 12), 10, scale=(1.0, 1.3, 1.0), steps=(3, 6))
    m.sphere(body, (0, -8, 23), 7, steps=(3, 5))
    m.box(beak, (0, -16, 22), (4, 7, 4))
    tilt_box(m, wing, (0, 10, 13), (0, 24, 18), 9, 4)
    for s in (-1, 1):
        m.box(wing, (s * 9, 4, 13), (4, 15, 10))
        m.box(EYE, (s * 5, -12, 25), (4, 4, 4))
    if bib:
        m.box(bib, (0, -8, 12), (10, 4, 10))
    if legs:
        for s in (-1, 1):
            m.box(beak, (s * 4, 2, 2), (4, 4, 6))


def cat(sc, x, y, z, yaw=0.0, k=1.8, col=WHITE, patch=WOOD, eye=AMBER, ear=None, muzzle=5.0, phase=0.0, rate=0.5, wag=22.0, rings=None):
    """A sitting cat (45 cm at k 1; muzzle 12: a sitting dog; rings: a raccoon's tail). Only its tail moves."""
    f = Fr(sc, x, y, z, yaw, k)
    g = f.still()
    g.box(col, (0, 8, 11), (24, 28, 22))
    g.cyl(col, (0, -4, 0), 11, 34, steps=6, top_r=8)
    g.box(patch, (0, -13, 17), (10, 5, 16))
    g.box(col, (0, -7, 42), (22, 18, 17))
    g.box(patch, (0, -16 - muzzle / 2.0, 39), (10, muzzle, 7))
    g.box(EYE, (0, -17 - muzzle, 41), (4, 4, 4))
    for s in (-1, 1):
        g.box(eye, (s * 6, -15, 46), (4, 4, 4))
        g.cyl(ear or col, (s * 7, -6, 50), 4.5, 9, steps=4, top_r=1.0)
        g.box(patch, (s * 6, -13, 3), (7, 8, 6))
    t = f.part(pivot=(0, 20, 5), axis=P.UP, phase=phase, rate=rate, swing=wag)
    tilt_box(t, col, (0, 20, 5), (20, 30, 6), 6, 6)
    tilt_box(t, rings or col, (20, 30, 6), (30, 22, 13), 6, 6)
    if rings:
        t.box(rings, (10, 25, 6), (5, 8, 8))
    return f


def wader(sc, x, y, z, yaw=0.0, k=1.0, body=PINK, neck=PINK, beak=CREAM, tip=EYE, leg=PINK, crest=None, one_leg=False,
          phase=0.0, rate=0.3, dip=20.0, rock=0.0):
    """A tall bird on long legs (135 cm at k 1): a flamingo, a heron. Its neck dips; or the whole bird rocks."""
    f = Fr(sc, x, y, z, yaw, k)
    g = f.part(pivot=(0, 0, 0), axis=90.0, phase=phase, rate=rate, swing=rock) if rock else f.still()
    g.box(leg, (-5, 0, 28), (5, 5, 58))
    if one_leg:
        tilt_box(g, leg, (5, 0, 56), (5, 12, 40), 5, 5)
    else:
        g.box(leg, (5, 2, 28), (5, 5, 58))
    g.sphere(body, (0, 2, 70), 17, scale=(1.0, 1.55, 0.9), steps=(4, 6))
    tilt_box(g, crest or body, (0, 22, 70), (0, 38, 62), 12, 6)
    n = f.part(pivot=(0, -18, 78), axis=0.0, phase=phase, rate=rate, swing=(0.0 if rock else dip))
    if rock:
        n = g
    tilt_box(n, neck, (0, -16, 76), (0, -30, 104), 6, 6)
    tilt_box(n, neck, (0, -30, 104), (0, -22, 126), 6, 6)
    n.box(neck, (0, -25, 130), (9, 13, 9))
    if crest:
        tilt_box(n, crest, (0, -22, 134), (0, -6, 130), 5, 4)
    tilt_box(n, beak, (0, -31, 130), (0, -42, 124), 5, 5)
    tilt_box(n, tip, (0, -42, 124), (0, -48, 119), 5, 5)
    for s in (-1, 1):
        n.box(EYE, (s * 4, -28, 132), (4, 4, 4))


def duck(sc, mo, x, y, z, yaw, k=1.0, body=TAN, head=PINE, beak=AMBER, neck=0.0, wing=None):
    """A water bird afloat (35 cm long at k 1), one part: it goes where mo takes it. neck: a swan's, cm."""
    m = sc.part(mo, (x, y, z), yaw, k)
    m.sphere(body, (0, 2, 7), 12, scale=(1.0, 1.55, 0.75), steps=(3, 6))
    tilt_box(m, wing or body, (0, 14, 9), (0, 26, 16), 10, 5)
    if neck:
        tilt_box(m, body, (0, -12, 10), (0, -20, 10 + neck * 0.6), 6, 6)
        tilt_box(m, body, (0, -20, 10 + neck * 0.6), (0, -14, 10 + neck), 6, 6)
        hz = 12 + neck
        for s in (-1, 1):
            tilt_box(m, wing or body, (s * 9, 0, 12), (s * 11, 20, 24), 5, 12)
    else:
        hz = 21
    m.sphere(head, (0, -15, hz), 7, steps=(3, 5))
    m.box(beak, (0, -24, hz - 1), (5, 8, 4))
    for s in (-1, 1):
        m.box(EYE, (s * 5, -18, hz + 2), (4, 4, 4))


def crate(g, x, y, z, s=60.0, col=WOOD, slat=WOOD_D):
    """A slatted wooden crate (never over 60 cm, never orange or green: a pickup is an orange crate, a green box)."""
    g.box(col, (x, y, z + s / 2.0), (s, s, s))
    for k in (-1, 1):
        g.box(slat, (x, y - s / 2.0 - 1, z + s / 2.0 + k * s * 0.3), (s + 2, 4, s * 0.14))
        g.box(slat, (x + k * (s / 2.0 - 3), y - s / 2.0 - 2, z + s / 2.0), (6, 4, s))


# ------------------------------------------------------------------------------------------ 1 sparrows on a wire (Fifth and Main, Theatre Row)
def sparrow_wire(sc):
    r = random.Random(11)
    g = sc.still()
    for s in (-1, 1):
        g.cyl(CREAM, (s * 205, 0, 0), 5, 150, steps=6)
        g.box(RED, (s * 205, 0, 40), (13, 13, 14))
        g.cyl(TEAL, (s * 205, 0, 150), 8, 10, steps=6, top_r=3)
        g.box(BRICK, (s * 205, 0, 4), (22, 22, 8))
    g.box(GREY_D, (0, 0, 138), (410, 4, 4))                       # the top wire: the sparrows' own
    g.box(GREY_D, (0, 8, 96), (410, 4, 4))                        # the washing line under it
    wash = sc.part(P.motion(swing=7, pivot=(0, 8, 96), axis=0.0, rate=0.45, phase=0.2))
    for k, (c, w, h) in enumerate(((CREAM, 46, 50), (RED, 30, 36), (TEAL, 40, 44), (WARM, 26, 30), (MAG, 44, 40))):
        x = -150 + k * 75
        wash.box(c, (x, 8, 94 - h / 2.0), (w, 4, h))
        wash.box(WOOD, (x - w / 2.0 + 5, 8, 97), (4, 6, 8))
        wash.box(WOOD, (x + w / 2.0 - 5, 8, 97), (4, 6, 8))
    g = sc.still()
    g.box(WOOD, (-150, 6, 5), (44, 30, 10))                       # a seed tray at the post's foot
    g.box(WARM, (-150, 6, 11), (36, 22, 4))
    cols = ((TAN, WOOD_D), (WOOD, WOOD_D), (TAN, BRICK_D))
    for k in range(7):
        b, w = cols[k % 3]
        perch(sc, -170 + k * 56 + r.uniform(-8, 8), 0, 140, yaw=(180 if k in (2, 5) else 0) + r.uniform(-25, 25), k=1.25, body=b, wing=w,
              bib=CREAM, phase=r.random(), rate=r.uniform(1.4, 2.3), bob=r.uniform(4, 7))
    perch(sc, -150, -4, 13, yaw=40, k=1.25, body=TAN, wing=WOOD_D, bib=CREAM, phase=0.4, rate=2.6, rock=22)   # one pecks at the seed


# ------------------------------------------------------------------------------------------ 2 the shop cat on a crate
def shop_cat(sc):
    g = sc.still()
    crate(g, 0, 10, 0, 58)
    crate(g, 52, 18, 0, 36, col=WOOD_D, slat=WOOD)
    g.box(RED, (0, 10, 61), (50, 44, 6))                          # its cushion
    g.cyl(WHITE, (-48, -8, 0), 13, 6, steps=8)                     # a saucer of milk
    g.cyl(CREAM, (-48, -8, 4), 10, 4, steps=8)
    g.box(SILVER, (58, -22, 3), (26, 8, 6))                       # a fish, picked clean
    g.box(SILVER, (72, -22, 5), (6, 12, 10))
    cat(sc, 0, 12, 64, yaw=-18, k=1.75, col=WHITE, patch=WOOD, ear=WOOD, phase=0.1, rate=0.45, wag=26)
    cat(sc, 52, 16, 36, yaw=25, k=1.05, col=STONE_L, patch=WHITE, phase=0.6, rate=0.9, wag=34)   # the kitten
    f = Fr(sc, -40, 26, 0, 200, 1.0)                              # a mouse looks out from behind the crate and hops back
    m = f.part(pivot=(0, 0, 0), axis=0.0, phase=0.3, rate=0.7, bob=5)
    m.sphere(STONE_L, (0, 0, 7), 7, scale=(1, 1.4, 1), steps=(3, 5))
    m.box(BLUSH, (-5, -5, 13), (5, 4, 6))
    m.box(BLUSH, (5, -5, 13), (5, 4, 6))
    tilt_box(m, BLUSH, (0, 9, 5), (0, 24, 4), 4, 4)


# ------------------------------------------------------------------------------------------ 3 the carriage horse (behind the Plaza's monument)
def dray_horse(sc):
    g = sc.still()
    for k in range(3):                                            # a stone mounting block of three steps
        g.box(STONE_L, (-250 + k * 22, 60, 15 + k * 30 - 50), (110 - k * 44, 80, 30 + 100))
    g.box(TEAL, (-250, 19, 62), (112, 4, 8))
    for s in (-1, 1):                                             # the hitching rail, a ring at each end
        g.cyl(WHITE, (s * 150 + 40, 95, -100), 7, 210, steps=6)
        g.sphere(TEAL, (s * 150 + 40, 95, 116), 11, steps=(3, 6))
    g.box(WHITE, (40, 95, 92), (300, 8, 8))
    g.box(WOOD, (215, 30, 22), (120, 50, 44))                     # a water trough
    g.box(AQUA, (215, 30, 45), (108, 40, 4))
    g.box(WOOD_D, (215, 4, 22), (124, 4, 10))
    # the hand cart of hay behind it
    g.box(WOOD, (-60, 190, 62), (170, 86, 10))
    for s in (-1, 1):
        rod(g, WOOD_D, (-60, 190 + s * 50 - 4, 34), (-60, 190 + s * 50 + 4, 34), 34, steps=10)
        g.box(WOOD, (-60, 190 + s * 46, 92), (170, 6, 50))
        tilt_box(g, WOOD, (30, 190 + s * 36, 62), (130, 190 + s * 36, 30), 6, 6)
    for k, (x, z) in enumerate(((-100, 68), (-30, 68), (-66, 112))):
        g.box(WARM, (x, 190, z + 22), (64, 74, 44))
        g.box(WOOD_D, (x, 190, z + 22), (8, 76, 46))
    horse = dict(length=165, height=128, girth=54, neck=54, neck_up=42, head=38, body=CREAM, belly=WHITE, mane=TAN, ears=CREAM,
                 legs_col=WHITE, tail=60, snout=BLUSH)
    P.quadruped(sc, 40, 20, yaw=90, s=1.22, graze=9, wag=22, rate=0.42, phase=0.15, **horse)
    g = sc.still()
    g.box(TEAL, (40, 20, 158), (86, 70, 6))                       # its blanket, the Plaza's teal, a white edge
    g.box(WHITE, (40, 20 - 36, 148), (86, 4, 16))
    g.box(WHITE, (40, 20 + 36, 148), (86, 4, 16))
    for k in range(3):                                            # sparrows at the spilled oats
        perch(sc, 150 + k * 34, -40 + (k % 2) * 22, 0, yaw=60 * k, k=1.3, body=TAN, wing=WOOD_D, bib=CREAM, phase=k * 0.31, rate=2.0, bob=6)
    g = sc.still()
    g.box(WARM, (168, -30, 3), (70, 40, 5))
    P.quadruped(sc, -150, -30, yaw=60, s=1.15, length=70, height=46, girth=22, neck=16, neck_up=35, head=20, body=WHITE, ears=WOOD,
                snout=WOOD, tail=26, tail_col=WHITE, wag=34, rate=1.3, phase=0.5)                                # the stable dog


# ------------------------------------------------------------------------------------------ 4 greyhounds and a poodle at a lead post
def greyhounds(sc):
    g = sc.still()
    g.cyl(WHITE, (0, 20, 0), 7, 104, steps=8)
    g.cyl(SILVER, (0, 20, 104), 11, 6, steps=8)
    g.sphere(BLUSH, (0, 20, 118), 10, steps=(3, 6))
    g.cyl(WHITE, (0, 20, 0), 20, 8, steps=8)
    g.box(BLUSH, (0, 14, 84), (30, 4, 12))                        # a bow on the post
    g.cyl(WHITE, (118, -12, 0), 16, 9, steps=8)                    # the water bowl
    g.cyl(CYAN, (118, -12, 6), 12, 4, steps=8)
    hound = dict(length=96, height=70, girth=20, neck=28, neck_up=48, head=26, tail=44)
    P.quadruped(sc, -92, 0, yaw=62, s=1.3, body=SILVER, belly=WHITE, ears=SILVER, snout=WHITE, wag=24, graze=6, rate=0.7, phase=0.0, **hound)
    P.quadruped(sc, 70, 24, yaw=-70, s=1.3, body=WHITE, belly=WHITE, ears=PEACH, snout=PEACH, wag=30, rate=0.9, phase=0.4, **hound)
    g = sc.still()
    g.box(BLUSH, (-92, 0, 94), (30, 70, 5), rot=(0, 62, 0))       # their coats
    g.box(VIOLET, (70, 24, 94), (30, 70, 5), rot=(0, -70, 0))
    tilt_box(g, VIOLET, (0, 20, 78), (-43, -26, 88), 4, 4)        # the leads
    tilt_box(g, BLUSH, (0, 20, 78), (17, 5, 88), 4, 4)
    # the poodle: pompoms, hopping on the spot
    P.quadruped(sc, 150, 30, yaw=20, s=1.0, length=52, height=44, girth=20, neck=14, neck_up=60, head=17, body=WHITE, ears=WHITE,
                snout=BLUSH, tail=0, rate=1.5, bob=7, phase=0.2)
    f = Fr(sc, 150, 30, 0, 20, 1.0)
    m = f.part(pivot=(0, 0, 0), axis=0.0, rate=1.5, bob=7, phase=0.2)
    m.sphere(WHITE, (0, -32, 61), 10, steps=(3, 6))
    m.sphere(WHITE, (0, 30, 48), 9, steps=(3, 5))
    for s in (-1, 1):
        m.sphere(WHITE, (s * 7, -20, 8), 7, steps=(3, 5))
        m.sphere(WHITE, (s * 7, 21, 8), 7, steps=(3, 5))
    m.box(BLUSH, (0, -30, 72), (12, 6, 5))


# ------------------------------------------------------------------------------------------ 5 raccoons at the bins, the guard dog asleep (the Garages' dock)
def raccoon_dog(sc):
    g = sc.still()
    g.cyl(SILVER, (-120, 10, 0), 30, 78, steps=10)
    g.cyl(GREY_D, (-120, 10, 75), 27, 4, steps=10)
    g.cyl(GREY_D, (-52, 30, 75), 27, 4, steps=10)
    for z in (22, 52):
        g.cyl(STONE_L, (-120, 10, z), 31.5, 5, steps=10)
    g.cyl(SILVER, (-52, 30, 0), 30, 78, steps=10, rot=(0, 0, 0))
    g.cyl(STONE_L, (-40, -28, 0), 33, 6, steps=10)                # its lid, on the floor
    g.box(WHITE, (-52, 6, 80), (26, 20, 8))                       # what was in it: paper, a bone
    g.box(CREAM, (-24, -34, 10), (30, 8, 6))
    cat(sc, -52, -34, 0, yaw=180, k=1.5, col=STONE_L, patch=WHITE, eye=WHITE, ear=GREY_D, muzzle=8, phase=0.2, rate=0.6, wag=24, rings=GREY_D)
    g = sc.still()
    g.box(GREY_D, (-52, -8, 68), (36, 5, 9))                      # the mask (it stands at the open bin, its back to the street)
    f = Fr(sc, -120, 10, 84, -30, 1.4)                            # the second one, head and paws out of the other bin
    m = f.part(pivot=(0, 0, 0), axis=0.0, phase=0.6, rate=0.5, bob=6)
    m.box(STONE_L, (0, 0, 8), (22, 18, 17))
    m.box(GREY_D, (0, -10, 10), (24, 4, 7))
    m.box(WHITE, (0, -12, 4), (10, 6, 6))
    for s in (-1, 1):
        m.cyl(GREY_D, (s * 8, 2, 16), 4.5, 8, steps=4, top_r=1.0)
        m.box(WHITE, (s * 6, -11, 11), (4, 4, 4))
    # the dog, asleep on its blue blanket: an ear twitches, the tail thumps
    g = sc.still()
    g.box(BLUE, (95, 6, 3), (150, 96, 6))
    g.box(WHITE, (95, -40, 3), (150, 6, 7))
    g.box(TAN, (95, 14, 26), (96, 42, 34))
    g.box(CREAM, (95, 14, 12), (80, 36, 10))
    g.box(TAN, (36, -6, 16), (30, 26, 22))
    g.box(CREAM, (20, -10, 10), (18, 16, 12))
    g.box(EYE, (10, -10, 12), (5, 8, 6))
    for s in (-1, 1):
        g.box(TAN, (52, -26 + s * 4, 6), (40, 9, 8))
    e = sc.part(P.motion(swing=24, pivot=(44, 6, 28), axis=0.0, rate=0.5, phase=0.3))
    e.box(WOOD_D, (40, 8, 22), (12, 5, 16))
    t = sc.part(P.motion(swing=20, pivot=(143, 20, 14), axis=P.UP, rate=0.8, phase=0.1))
    tilt_box(t, TAN, (143, 20, 14), (176, 2, 8), 7, 7)
    g = sc.still()
    g.cyl(WHITE, (150, -36, 0), 15, 9, steps=8)
    g.cyl(BLUE, (150, -36, 5), 11, 5, steps=8)


# ------------------------------------------------------------------------------------------ 6 gulls on the top deck's lamp posts
def gulls(sc):
    g = sc.still()
    for k, x in enumerate((-150, 0, 150)):
        g.cyl(WHITE, (x, 0, 0), 6, 250, steps=6)
        g.cyl(BLUE, (x, 0, 0), 12, 26, steps=6)
        g.cyl(BLUE, (x, 0, 150), 7.5, 12, steps=6)
        g.box(WHITE, (x, 0, 252), (60, 8, 6))
        for s in (-1, 1):
            g.sphere(WARM, (x + s * 30, 0, 240), 12, steps=(3, 6), glow=0.9)
    g.box(WARM, (0, -30, 2), (380, 10, 4))                        # a painted bay line
    for x in (-150, 150):
        g.box(WARM, (x, -8, 2), (10, 54, 4))
    kw = dict(k=2.3, body=WHITE, wing=SILVER, beak=AMBER, legs=True)
    perch(sc, -150, 0, 255, yaw=20, phase=0.1, rate=0.4, rock=7, **kw)
    perch(sc, 150 + 22, 0, 255, yaw=-35, phase=0.6, rate=0.3, rock=6, **kw)
    perch(sc, -60, -20, 0, yaw=150, phase=0.3, rate=1.1, rock=16, **kw)      # one on the deck, at a dropped chip
    g = sc.still()
    g.box(WARM_L, (-84, -40, 3), (12, 5, 5))
    # the one that lifts off the middle post and settles again (a hop of 55 cm, slow wings)
    P.bird(sc, 0, 0, 262, heading=-90, span=150, body=WHITE, wing=SILVER, beak=AMBER, rate=0.4, flap=30, bob=55, phase=0.2)


# ------------------------------------------------------------------------------------------ 7 squirrels chasing round a trunk (the Plaza)
def squirrels(sc):
    r = random.Random(7)
    g = sc.still()
    g.cyl(WOOD_D, (0, 0, -150), 62, 240, steps=9, top_r=50)
    g.cyl(WOOD_D, (0, 0, 90), 50, 200, steps=9, top_r=40)
    g.cyl(WOOD, (0, 0, 290), 40, 8, steps=9, top_r=30)            # the snapped top
    tilt_box(g, WOOD_D, (30, 0, 215), (110, 20, 262), 22, 22)     # a stub of a branch, a few leaves left
    g.sphere(PINE, (128, 24, 280), 34, scale=(1, 1, 0.7), steps=(3, 6))
    tilt_box(g, WOOD_D, (-28, 10, 205), (-90, 30, 228), 18, 18)
    g.box(BLACK, (0, -49, 150), (26, 6, 34))                      # the hole they live in
    g.cyl(STONE_L, (0, 0, -40), 150, 52, steps=12)                # a white stone ring round the foot, grass in it
    g.cyl(PINE, (0, 0, 12), 138, 5, steps=12)
    g.cyl(TEAL, (0, 0, 2), 152, 6, steps=12)
    for k in range(6):                                            # acorns and toadstools
        a = r.uniform(0, 6.283)
        x, y = math.cos(a) * 105, math.sin(a) * 105
        g.cyl(CREAM, (x, y, 16), 4, 10, steps=5)
        g.cyl(TEAL if k % 2 else WHITE, (x, y, 26), 11, 7, steps=6, top_r=3)
    for k, (ang, z, rate) in enumerate(((0, 30, 0.22), (150, 38, 0.22), (40, 112, -0.3))):
        a = math.radians(ang)
        rad = 62.0 if z < 90 else 59.0
        f = Fr(sc, math.cos(a) * rad, math.sin(a) * rad, z, ang + (180 if rate > 0 else 0), 1.5)
        m = sc.part(P.motion(base=(0, 0), travel=(-1.0, rate), phase=k * 0.25, rate=3.0, bob=3), f.at, f.yaw, f.k)
        m.sphere(RUST, (0, 0, 8), 9, scale=(1, 1.9, 1), steps=(3, 6))
        m.sphere(RUST, (0, -20, 12), 7, steps=(3, 5))
        m.box(CREAM, (0, -3, 3), (8, 22, 4))
        for s in (-1, 1):
            m.cyl(RUST, (s * 4, -20, 17), 3, 6, steps=4, top_r=1.0)
            m.box(EYE, (s * 5, -24, 14), (4, 4, 4))
        tilt_box(m, RUST, (0, 14, 10), (0, 26, 24), 9, 9)
        m.sphere(RUST, (0, 26, 30), 11, scale=(1, 1, 1.4), steps=(3, 6))
    f = Fr(sc, 0, 0, 298, 15, 1.6)                                # the one on top, at a nut
    m = f.part(pivot=(0, 0, 0), axis=0.0, phase=0.2, rate=1.2, swing=10)
    m.sphere(RUST, (0, 2, 12), 10, scale=(1, 1, 1.3), steps=(3, 6))
    m.sphere(RUST, (0, -6, 28), 7, steps=(3, 5))
    m.box(WOOD, (0, -14, 22), (6, 6, 6))
    m.sphere(RUST, (0, 16, 22), 10, scale=(1, 1, 1.7), steps=(3, 6))
    for s in (-1, 1):
        m.cyl(RUST, (s * 4, -6, 33), 3, 6, steps=4, top_r=1.0)
        m.box(EYE, (s * 5, -11, 30), (4, 4, 4))


# ------------------------------------------------------------------------------------------ 8 ducks and two swans (on the monument's own ring of water)
def ducks_swans(sc):
    """z = 0 is the water (the table stands it 0.5 m up, on SM_Monument's ring: plinth to 1.7 m, water to 4.6 m)."""
    g = sc.still()
    r = random.Random(4)
    for k in range(7):                                            # lily pads, a flower on some
        a = k * 0.9 + 0.3
        rad = (420, 428, 416, 424, 430, 418, 426)[k]
        x, y = math.cos(a) * rad, math.sin(a) * rad
        g.cyl(PINE, (x, y, 0), 20, 4, steps=7)
        if k % 2 == 0:
            g.cyl(BLUSH, (x + 4, y, 4), 8, 7, steps=5, top_r=3)
    for k, ang in enumerate((0, 180)):                            # the two swans go round one way, half a ring apart
        a = math.radians(ang)
        duck(sc, P.motion(base=(0, 0), travel=(-1.0, 0.045)), math.cos(a) * 335, math.sin(a) * 335, 0, ang + 180, k=2.6, body=WHITE,
             head=WHITE, beak=PEACH, neck=26, wing=WHITE)
    for k in range(4):                                            # a duck and three ducklings in a line, the other way
        a = math.radians(25 + (0, 19, 32, 45)[k])
        duck(sc, P.motion(base=(0, 0), travel=(-1.0, -0.07)), math.cos(a) * 260, math.sin(a) * 260, 0, math.degrees(a), k=(1.5 if k == 0 else 0.85),
             body=(TAN if k == 0 else WARM), head=(PINE if k == 0 else WARM), beak=AMBER)
    for k, ang in enumerate((215, 300)):                          # two that stay and bob
        a = math.radians(ang)
        duck(sc, P.motion(rate=0.5, bob=-4, phase=k * 0.4, swing=10, pivot=(math.cos(a) * 205, math.sin(a) * 205, 0), axis=ang),
             math.cos(a) * 205, math.sin(a) * 205, 0, ang + 90, k=1.5, body=WHITE if k else STONE_L, head=PINE, beak=AMBER)


# ------------------------------------------------------------------------------------------ 9 a koi trough (penthouse terraces)
def koi_trough(sc):
    g = sc.still()
    g.box(WHITE, (0, 12, 20), (320, 114, 40))
    g.box(TEAL, (0, 12, 34), (324, 118, 8))
    g.box(AQUA, (0, 12, 41), (296, 90, 4))
    for s in (-1, 1):
        g.box(WHITE, (s * 152, 12, 46), (16, 114, 12))
    g.box(WHITE, (0, 62, 46), (320, 14, 12))
    g.cyl(PINE, (-100, 30, 43), 13, 4, steps=7)                    # lily pads
    g.cyl(PINE, (84, -6, 43), 11, 4, steps=7)
    g.cyl(BLUSH, (84, -6, 46), 6, 6, steps=5, top_r=2)
    tilt_box(g, TEAL, (-150, 62, 52), (-150, 62, 96), 8, 8)        # a spout
    tilt_box(g, TEAL, (-150, 62, 92), (-128, 40, 86), 8, 8)
    tilt_box(g, AQUA, (-128, 40, 84), (-128, 38, 46), 5, 5)
    fish = ((-90, -14, RED, WHITE, 0.07), (20, 6, WHITE, RED, 0.055), (100, 28, AMBER, WHITE, 0.08), (-30, 40, WHITE, AMBER, -0.06), (70, -18, RED, AMBER, -0.075))
    for k, (x, y, c1, c2, rt) in enumerate(fish):
        d = 1.0 if rt > 0 else -1.0
        mo = dict(base=(x, y), travel=(220.0, rt), rate=1.6, phase=k * 0.21)
        m = sc.part(P.motion(pivot=(x, y, 44), **mo))
        m.sphere(c1, (x, y, 44), 11, scale=(2.1, 0.75, 0.6), steps=(3, 6))
        m.box(c2, (x + d * 4, y, 50), (16, 10, 4))
        m.box(c2, (x - d * 12, y, 49), (8, 8, 4))
        t = sc.part(P.motion(swing=28, pivot=(x - d * 20, y, 44), axis=P.UP, **mo))
        tilt_box(t, c1, (x - d * 20, y, 44), (x - d * 36, y, 46), 4, 15)
    f = Fr(sc, 130, 66, 52, 150, 1.0)                             # a frog on the rim
    m = f.part(pivot=(0, 0, 0), axis=0.0, phase=0.5, rate=0.6, bob=6)
    m.sphere(PINE, (0, 0, 6), 8, scale=(1.2, 1.3, 0.8), steps=(3, 6))
    for s in (-1, 1):
        m.box(WARM, (s * 5, -7, 11), (5, 5, 5))


# ------------------------------------------------------------------------------------------ 10 flamingos in a shallow roof pool
def flamingos(sc):
    g = sc.still()
    g.box(WHITE, (0, 14, 10), (330, 118, 20))
    g.box(BLUSH, (0, -44, 12), (330, 4, 8))
    g.box(CYAN, (0, 14, 21), (306, 94, 4), glow=0.35)
    for s in (-1, 1):
        g.cyl(WHITE, (s * 150, 62, 20), 12, 26, steps=8, top_r=9)   # a tub of reeds at each back corner
        for k in range(3):
            g.box(PINE, (s * 150 + (k - 1) * 7, 62, 66), (4, 4, 46))
    kw = dict(beak=CREAM, tip=EYE)
    wader(sc, -105, 10, 14, yaw=20, k=1.08, body=PINK, neck=PINK, leg=PINK, phase=0.0, rate=0.25, dip=34, **kw)
    wader(sc, -30, 30, 14, yaw=-30, k=1.0, body=BLUSH, neck=BLUSH, leg=PINK, one_leg=True, phase=0.4, rate=0.2, rock=3, **kw)
    wader(sc, 50, 6, 14, yaw=8, k=1.12, body=PINK, neck=PINK, leg=PINK, one_leg=True, phase=0.7, rate=0.3, dip=16, **kw)
    wader(sc, 118, 28, 14, yaw=160, k=0.98, body=BLUSH, neck=PINK, leg=PINK, phase=0.2, rate=0.22, dip=40, **kw)


# ------------------------------------------------------------------------------------------ 11 the peacock on a parapet block
def peacock(sc):
    g = sc.still()
    g.box(WHITE, (0, 14, 27), (150, 46, 54))
    g.box(BLUSH, (0, -10, 46), (152, 4, 8))
    g.box(WHITE, (0, 14, 57), (160, 54, 6))
    g.sphere(BLUE, (-20, 14, 78), 15, scale=(1.0, 1.3, 1.1), steps=(4, 6))       # the cock, facing the street
    for s in (-1, 1):
        g.box(AMBER, (-20 + s * 6, 12, 62), (4, 4, 8))
    fan = sc.part(P.motion(swing=5, pivot=(-20, 24, 80), axis=90.0, rate=0.3, phase=0.1))
    for k in range(9):
        a = math.radians(-80 + k * 20)
        tip = (-20 + math.sin(a) * 84, 28, 80 + math.cos(a) * 84)
        tilt_box(fan, PINE if k % 2 else TEAL, (-20, 28, 80), tip, 4, 15)
        fan.box(VIOLET, (tip[0], 26, tip[2]), (13, 4, 13))
        fan.box(CYAN, (tip[0], 24, tip[2]), (6, 4, 6))
    head = sc.part(P.motion(swing=9, pivot=(-20, 6, 88), axis=0.0, rate=0.6, phase=0.4))
    tilt_box(head, BLUE, (-20, 6, 86), (-20, -2, 112), 7, 7)
    head.box(BLUE, (-20, -5, 116), (9, 12, 9))
    head.box(AMBER, (-20, -14, 115), (4, 7, 4))
    for k in (-1, 0, 1):
        head.box(TEAL, (-20 + k * 5, -2, 126), (4, 4, 10))
    for s in (-1, 1):
        head.box(WHITE, (-20 + s * 4, -9, 118), (4, 4, 4))
    # the white hen beside him, her train down the block's end, pecking
    g = sc.still()
    g.sphere(WHITE, (48, 12, 74), 12, scale=(1.0, 1.3, 1.0), steps=(4, 6))
    tilt_box(g, WHITE, (52, 20, 72), (92, 26, 30), 12, 6)
    for s in (-1, 1):
        g.box(AMBER, (48 + s * 5, 10, 62), (4, 4, 6))
    hen = sc.part(P.motion(swing=22, pivot=(48, 4, 80), axis=0.0, rate=0.9, phase=0.7))
    tilt_box(hen, WHITE, (48, 4, 80), (48, -6, 98), 6, 6)
    hen.box(WHITE, (48, -9, 101), (8, 10, 8))
    hen.box(AMBER, (48, -17, 100), (4, 6, 4))
    hen.box(BLUSH, (48, -6, 108), (4, 4, 8))


# ------------------------------------------------------------------------------------------ 12 the Syndicate's dogs and the black cat (tails only)
def syn_dogs(sc):
    g = sc.still()
    for k, (x, z) in enumerate(((58, 0), (112, 0), (84, 34))):    # three strongboxes, each under 60 cm
        g.box(BLACK, (x, 18, z + 16), (50, 38, 32))
        g.box(GOLD, (x, -2, z + 16), (12, 4, 12))
        for s in (-1, 1):
            g.box(GOLD, (x + s * 23, -2, z + 28), (6, 4, 8))
            g.box(GOLD, (x + s * 23, -2, z + 4), (6, 4, 8))
    g.box(GREY_D, (-150, 26, 30), (8, 8, 60))                     # an iron post, a red glass lantern unlit, their chains
    g.box(GREY_D, (-150, 26, 62), (20, 20, 6))
    g.box(BRICK_D, (-150, 26, 76), (14, 14, 22))
    g.cyl(GREY_D, (-150, 26, 87), 12, 8, steps=4, top_r=2)
    dog = dict(k=2.3, col=GREY_D, patch=SILVER, eye=WARM, ear=GREY_D, muzzle=12, wag=10)
    cat(sc, -100, 6, 0, yaw=-10, phase=0.0, rate=0.4, **dog)
    cat(sc, -18, 10, 0, yaw=12, phase=0.5, rate=0.33, **dog)
    g = sc.still()
    for x, y in ((-100, -3), (-18, 1)):
        g.box(RED, (x, y, 76), (44, 44, 6))                        # their collars
    tilt_box(g, SILVER, (-146, 26, 40), (-108, 16, 70), 4, 4)
    tilt_box(g, SILVER, (-146, 26, 40), (-30, 22, 70), 4, 4)
    cat(sc, 84, 16, 66, yaw=20, k=1.5, col=BLACK, patch=GREY_D, eye=GOLD, phase=0.3, rate=0.35, wag=16)


# ------------------------------------------------------------------------------------------ 13 a fox at a pipe mouth (the circuit's yards)
def fox_pipe(sc):
    g = sc.still()
    for k, (y, z) in enumerate(((-4, 40), (58, 40), (28, 98))):   # three riveted pipes on two cradles
        rod(g, SILVER, (-150, y, z), (150, y, z), 34, steps=10)
        for x in (-150, 0, 146):
            rod(g, STONE_L, (x - 3, y, z), (x + 3, y, z), 36.5, steps=10)
        rod(g, STEEL_B, (150, y, z), (151, y, z), 28, steps=10)   # the dark of its mouth
        rod(g, STEEL_B, (-151, y, z), (-150, y, z), 28, steps=10)
    rod(g, VIOLET, (60, 28, 98), (72, 28, 98), 35.5, steps=10)
    for x in (-90, 90):
        g.box(STEEL_B, (x, 27, 5), (16, 140, 10))
        for s in (-1, 1):
            tilt_box(g, STEEL_B, (x, 27 + s * 66, 4), (x, 27 + s * 82, 40), 10, 10)
    # the fox: head and forepaws out of the top pipe's mouth, looking up and down the road
    f = Fr(sc, 152, 28, 82, 90, 1.5)
    g = f.still()
    for s in (-1, 1):
        g.box(RUST, (s * 6, -8, 2), (6, 14, 6))
        g.box(BLACK, (s * 6, -15, 2), (6, 5, 6))
    m = f.part(pivot=(0, 4, 10), axis=P.UP, phase=0.2, rate=0.3, swing=24)
    m.box(RUST, (0, -4, 14), (20, 16, 15))
    m.box(WHITE, (0, -13, 11), (16, 5, 9))
    tilt_box(m, RUST, (0, -10, 13), (0, -24, 10), 8, 7)
    m.box(BLACK, (0, -26, 10), (5, 5, 5))
    for s in (-1, 1):
        m.cyl(RUST, (s * 7, -1, 21), 4.5, 10, steps=4, top_r=1.0)
        m.box(WARM, (s * 5, -12, 17), (4, 4, 4))
    # the vixen, curled on top, her brush swishing
    g = sc.still()
    g.sphere(RUST, (-60, 28, 142), 24, scale=(1.5, 1.0, 0.55), steps=(4, 6))
    g.box(RUST, (-94, 20, 144), (18, 16, 14))
    g.box(WHITE, (-105, 20, 141), (8, 10, 8))
    for s in (-1, 1):
        g.cyl(RUST, (-92, 20 + s * 6, 150), 4, 9, steps=4, top_r=1.0)
    t = sc.part(P.motion(swing=14, pivot=(-28, 28, 140), axis=P.UP, rate=0.35, phase=0.6))
    tilt_box(t, RUST, (-28, 28, 140), (4, 10, 140), 13, 11)
    tilt_box(t, WHITE, (4, 10, 140), (16, 2, 140), 11, 9)
    g = sc.still()
    for k in range(3):                                            # thistles at the cradle's foot
        g.box(PINE, (-128 + k * 9, -36, 20), (4, 4, 40))
        g.sphere(VIOLET, (-128 + k * 9, -36, 44), 7, steps=(3, 5))


# ------------------------------------------------------------------------------------------ 14 a heron on a pipe cradle
def heron(sc):
    g = sc.still()
    rod(g, SILVER, (-140, 20, 30), (140, 20, 30), 26, steps=10)
    for x in (-140, -40, 60, 138):
        rod(g, STONE_L, (x - 3, 20, 30), (x + 3, 20, 30), 28.5, steps=10)
    rod(g, CYAN, (-96, 20, 30), (-88, 20, 30), 27.5, steps=10)
    rod(g, STEEL_B, (140, 20, 30), (141, 20, 30), 21, steps=10)
    for x in (-100, 100):
        g.box(STEEL_B, (x, 20, 4), (14, 90, 8))
        for s in (-1, 1):
            tilt_box(g, STEEL_B, (x, 20 + s * 44, 4), (x, 20 + s * 60, 30), 9, 9)
    g.cyl(AQUA, (175, 0, 0), 36, 4, steps=9)                       # a puddle under the pipe's mouth, a drip
    g.box(AQUA, (146, 20, 10), (4, 4, 14))
    wader(sc, -30, 18, 56, yaw=-12, k=0.88, body=SILVER, neck=WHITE, beak=WARM, tip=WARM, leg=WARM, crest=STEEL_B, phase=0.1, rate=0.16, dip=20)
    f = Fr(sc, 176, -2, 4, 100, 1.0)                              # the frog it has its eye on
    m = f.part(pivot=(0, 0, 0), axis=0.0, phase=0.3, rate=0.45, bob=5)
    m.sphere(PINE, (0, 0, 6), 8, scale=(1.2, 1.3, 0.8), steps=(3, 6))
    for s in (-1, 1):
        m.box(WARM, (s * 5, -7, 11), (5, 5, 5))


# ------------------------------------------------------------------------------------------ 15 the yard cats on a riveted tank
def yard_cats(sc):
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL_B, (sx * 40, 22 + sy * 26, 14), (8, 8, 28))
    g.cyl(SILVER, (0, 22, 26), 56, 72, steps=10)
    g.cyl(VIOLET, (0, 22, 56), 57.5, 10, steps=10)
    g.cyl(STONE_L, (0, 22, 98), 58, 6, steps=10)
    for k in range(8):
        a = k * math.pi / 4.0
        g.box(STONE_L, (math.cos(a) * 57, 22 + math.sin(a) * 57, 34), (5, 5, 5))
    tilt_box(g, STEEL_B, (60, 22, 70), (96, 22, 70), 8, 8)         # the tap, a drip pan
    tilt_box(g, STEEL_B, (96, 22, 72), (96, 22, 56), 8, 8)
    g.cyl(STONE_L, (96, 22, 0), 18, 8, steps=8)
    g.cyl(AQUA, (96, 22, 5), 14, 4, steps=8)
    cat(sc, -8, 20, 104, yaw=-15, k=1.45, col=STONE_L, patch=WHITE, eye=CYAN, phase=0.1, rate=0.4, wag=22)
    cat(sc, -96, 0, 0, yaw=30, k=1.5, col=GREY_D, patch=SILVER, eye=WARM, phase=0.5, rate=0.3, wag=16)
    # the third laps at the drip pan: its head goes down and up
    g = sc.still()
    g.box(WHITE, (128, 6, 18), (40, 22, 20))
    for sx in (-1, 1):
        g.box(WHITE, (128 + sx * 15, 6, 5), (7, 8, 10))
    h = sc.part(P.motion(swing=12, pivot=(110, 6, 22), axis=90.0, rate=0.7, phase=0.2))
    h.box(WHITE, (102, 6, 20), (18, 18, 15))
    h.box(BLUSH, (92, 6, 16), (5, 6, 5))
    for s in (-1, 1):
        h.cyl(STONE_L, (104, 6 + s * 6, 27), 4, 8, steps=4, top_r=1.0)
    t = sc.part(P.motion(swing=18, pivot=(148, 6, 24), axis=P.UP, rate=0.5, phase=0.6))
    tilt_box(t, WHITE, (148, 6, 24), (170, 20, 34), 7, 7)


# ------------------------------------------------------------------------------------------ 16 owls on a cornice (Wall Street)
def owls(sc):
    g = sc.still()
    g.box(SAND, (0, 6, 12), (290, 40, 24))
    g.box(CREAM, (0, 6, 27), (300, 46, 6))
    g.box(PINE, (0, -18, 20), (292, 4, 6))
    for s in (-1, 1):                                             # a stone pineapple at each end
        g.cyl(SAND, (s * 128, 6, 30), 10, 10, steps=6)
        g.sphere(SAND, (s * 128, 6, 56), 17, scale=(1, 1, 1.3), steps=(4, 6))
        g.cyl(PINE, (s * 128, 6, 74), 9, 16, steps=5, top_r=1.5)
    spec = ((-70, 1.5, TAN, CREAM, 0.16, 0.0, 62), (4, 1.75, SAND, WHITE, 0.12, 0.4, 70), (72, 1.4, TAN, CREAM, 0.2, 0.7, 55))
    for x, k, body, face, rate, ph, turn in spec:
        f = Fr(sc, x, 8, 30, 0, k)
        b = f.still()
        b.sphere(body, (0, 0, 16), 13, scale=(1.0, 0.95, 1.3), steps=(4, 6))
        b.box(face, (0, -10, 14), (14, 5, 16))
        for s in (-1, 1):
            b.box(WOOD_D, (s * 12, 2, 16), (5, 14, 20))
            b.box(AMBER, (s * 5, -6, 2), (5, 8, 4))
        h = f.part(pivot=(0, 0, 34), axis=P.UP, phase=ph, rate=rate, swing=turn)     # the head turns, slowly, a long way
        h.sphere(body, (0, 0, 40), 12, scale=(1.1, 1.0, 0.9), steps=(4, 6))
        h.box(face, (0, -9, 40), (20, 5, 13))
        h.box(AMBER, (0, -13, 37), (4, 5, 5))
        for s in (-1, 1):
            h.box(WARM, (s * 5.5, -11, 42), (7, 4, 7))
            h.box(EYE, (s * 5.5, -12.5, 42), (4, 4, 4))
            h.cyl(body, (s * 8, 0, 48), 3.5, 8, steps=4, top_r=1.0)
    perch(sc, 112, 2, 30, yaw=-20, k=1.3, body=CREAM, wing=TAN, beak=AMBER, phase=0.3, rate=0.8, bob=4)      # the owlet


# ------------------------------------------------------------------------------------------ 17, 18 white doves, wide and slow, far out and high
def _doves(sc, n, R, seed, rate):
    r = random.Random(seed)
    for k in range(n):
        ang = (k - (n - 1) / 2.0) * (300.0 / R) * (180.0 / math.pi) * 1.15 + r.uniform(-4, 4)       # about 3.5 m apart along the ring
        a = math.radians(ang)
        rad = R + r.uniform(-260, 220)
        P.bird(sc, math.cos(a) * rad, math.sin(a) * rad, r.uniform(0, 420), heading=ang + 90.0, span=300, body=WHITE, wing='#efe6ff', beak=BLUSH,
               rate=r.uniform(1.5, 1.9), flap=30, phase=r.random(), orbit=(0.0, 0.0, rate))


def doves_a(sc):
    _doves(sc, 7, 1600.0, 3, 0.034)


def doves_b(sc):
    _doves(sc, 5, 1250.0, 8, 0.04)


LOW = dict(grow=(30, 30, 30), grow_down=(30, 30, 0))
JOBS = [
    ('SM_S1ANIMALS_SparrowWire', sparrow_wire, LOW),
    ('SM_S1ANIMALS_ShopCat', shop_cat, LOW),
    ('SM_S1ANIMALS_DrayHorse', dray_horse, LOW),
    ('SM_S1ANIMALS_Greyhounds', greyhounds, LOW),
    ('SM_S1ANIMALS_RaccoonDog', raccoon_dog, LOW),
    ('SM_S1ANIMALS_Gulls', gulls, dict(grow=(40, 80, 80), grow_down=(40, 80, 0))),
    ('SM_S1ANIMALS_Squirrels', squirrels, dict(grow=(60, 60, 20), grow_down=(60, 60, 0))),
    ('SM_S1ANIMALS_DucksSwans', ducks_swans, dict(grow=(40, 40, 20), grow_down=(40, 40, 0))),
    ('SM_S1ANIMALS_KoiTrough', koi_trough, LOW),
    ('SM_S1ANIMALS_Flamingos', flamingos, LOW),
    ('SM_S1ANIMALS_Peacock', peacock, LOW),
    ('SM_S1ANIMALS_SynDogs', syn_dogs, LOW),
    ('SM_S1ANIMALS_FoxPipe', fox_pipe, LOW),
    ('SM_S1ANIMALS_Heron', heron, LOW),
    ('SM_S1ANIMALS_YardCats', yard_cats, LOW),
    ('SM_S1ANIMALS_Owls', owls, LOW),
    # the flock is drawn at one side of its ring: the bounds are grown to the whole ring
    ('SM_S1ANIMALS_Doves_A', doves_a, dict(far=True, grow=(50, 900, 150), grow_down=(3000, 900, 100))),
    ('SM_S1ANIMALS_Doves_B', doves_b, dict(far=True, grow=(50, 750, 150), grow_down=(2500, 750, 100))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
