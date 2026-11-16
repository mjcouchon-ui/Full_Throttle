"""City Center, Stage 1, builder s1d4: district 4 "The Plaza" (3.13 to 4.62 km), the civic park.
/Game/World/CityCenter/SM_S1D4_*. Run: import ft_world_city_s1d4 as K; K.run()

Low, wide civic marble and glass: a bandstand, a tea pavilion, a carousel house, a fountain court, a palm house, a
library, a belvedere; the park's furniture (globe lamps, a sundial, benches round a tree, a boat-hire rack, a fountain,
a pergola walk, a balustrade with urns, a model-yacht pond); and THE ORRERY, a rotunda under a teal dome carrying an
armillary sphere 60 m across whose rings turn slowly.

Where they stand (Source/FullThrottle/FTPeopleL7.inl, block s1d4): every piece 41.9 m or more from the centre line (the
street's first 41 m are the level's own kerb, podiums, towers and monuments), in the pockets behind the monuments,
at the far ends of the side streets (behind the elevated line), in the lots behind the left-hand towers. So every
piece may be taller than 2 m and may carry steady light. Nothing blinks. One material, one section (ft_people.Scene).
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'
TEAL, WHITE, AQUA, PINE, STONE, LAMP = '#2fc6c0', '#fff6ea', '#9ff0ff', '#2f6b52', '#9a98a8', '#ffe28a'
G1, G2, G4 = 0.35, 0.9, 2.0                       # glow: soft (glass, water), a lit window, a lamp's globe
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SMALL = dict(grow=(30, 30, 30), grow_down=(30, 30, 0))


# ------------------------------------------------------------------------------------------ helpers
def pol(r, deg, z=0.0):
    a = math.radians(deg)
    return (r * math.cos(a), r * math.sin(a), z)


def drum(g, x, y, z, r, h, col=WHITE, cap=None, steps=6):
    """A fat column: a shaft, a foot and a head."""
    g.cyl(col, (x, y, z), r, h, steps=steps)
    g.cyl(cap or col, (x, y, z), r * 1.4, h * 0.07, steps=steps)
    g.cyl(cap or col, (x, y, z + h * 0.92), r * 1.4, h * 0.08, steps=steps)


def saucer(g, x, y, z, r, col=TEAL, rim=WHITE, steps=12):
    """A roof like an upturned saucer; returns its top."""
    g.cyl(rim, (x, y, z), r, r * 0.05, steps=steps)
    g.cyl(col, (x, y, z + r * 0.05), r * 0.97, r * 0.2, steps=steps, top_r=r * 0.3)
    g.cyl(rim, (x, y, z + r * 0.25), r * 0.3, r * 0.06, steps=steps, top_r=r * 0.2)
    return z + r * 0.31


def ring(m, cols, c, R, tilt, head, n=24, w=200.0, glow=0.0):
    """A ring of n straight lengths round c: its plane is tipped `tilt` degrees from level about the level line that
    points along `head` degrees. Returns the function that gives a point on it (degrees round the ring)."""
    t, h = math.radians(tilt), math.radians(head)
    u = (math.cos(h), math.sin(h), 0.0)
    v = (-math.sin(h) * math.cos(t), math.cos(h) * math.cos(t), math.sin(t))

    def at(deg):
        a = math.radians(deg)
        return tuple(c[i] + R * (math.cos(a) * u[i] + math.sin(a) * v[i]) for i in range(3))

    step = 360.0 / n
    for k in range(n):
        tilt_box(m, cols[k % len(cols)], at((k - 0.07) * step), at((k + 1.07) * step), w, w, glow=glow)
    return at


def vane(sc, x, y, z, col=TEAL, phase=0.0):
    """A weather vane: an arrow that swings in the wind. Its top is z + 1.1 m."""
    g = sc.still()
    g.cyl(WHITE, (x, y, z), 6, 92, steps=5)
    v = sc.part(P.motion(swing=24, pivot=(x, y, z + 80), axis=P.UP, rate=0.12, phase=phase))
    v.box(col, (x, y, z + 80), (130, 6, 8))
    v.box(col, (x - 60, y, z + 80), (36, 6, 34))
    v.box(LAMP, (x + 66, y, z + 80), (22, 8, 22), rot=(45, 0, 0))


def globe_post(g, x, y, z, h=150.0, r=24.0):
    """A small lamp: a white post and a warm globe."""
    g.cyl(WHITE, (x, y, z), 7, h, steps=5)
    g.sphere(LAMP, (x, y, z + h + r * 0.8), r, steps=(4, 6), glow=G4)


# ------------------------------------------------------------------------------------------ buildings
def bandstand(sc):
    """10.4 m across, 8 m tall: a saucer roof on eight drums, a lit ceiling, a drum and music stands, bunting."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -300), 520, 340, steps=12)
    g.cyl(WHITE, (0, 0, 40), 470, 40, steps=12)
    g.cyl(TEAL, (0, 0, 80), 440, 14, steps=12)
    g.box(WHITE, (0, -470, 10), (260, 90, 60))
    g.box(STONE, (0, -440, 45), (220, 60, 50))
    for k in range(8):
        a = 22.5 + k * 45
        x, y, _ = pol(390, a)
        drum(g, x, y, 94, 32, 330)
        if k != 5:                                              # the bay towards the road stays open
            tilt_box(g, WHITE, pol(390, a, 172), pol(390, a + 45, 172), 12, 12)
            tilt_box(g, TEAL, pol(390, a, 132), pol(390, a + 45, 132), 8, 40)
    g.cyl(LAMP, (0, 0, 414), 330, 10, steps=12, glow=G2)       # the lit ceiling
    top = saucer(g, 0, 0, 424, 540)
    g.cyl(WHITE, (0, 0, top), 60, 60, steps=8)
    g.sphere(LAMP, (0, 0, top + 95), 42, steps=(4, 6), glow=G4)
    cols = (TEAL, LAMP, WHITE, AQUA)
    for k in range(16):                                         # bunting under the eave
        a = k * 22.5
        p = pol(548, a, 396)
        g.box(cols[k % 4], p, (44, 6, 46), rot=(0, a + 90, 0))
    rod(g, WHITE, (-17, 60, 160), (17, 60, 160), 62, steps=10)   # a bass drum on its side
    rod(g, TEAL, (-21, 60, 160), (21, 60, 160), 40, steps=8)
    for k in range(5):                                          # music stands and stools in a half ring
        a = 30 + k * 30
        x, y, _ = pol(250, a)
        g.cyl(STONE, (x, y, 94), 5, 110, steps=5)
        g.box(WHITE, (x, y, 212), (46, 6, 34), rot=(0, a + 90, 0))
        x, y, _ = pol(310, a)
        g.cyl(TEAL, (x, y, 94), 20, 46, steps=6)
    vane(sc, 0, 0, top + 130)


def tea_pavilion(sc):
    """10.4 x 8.2 m, 7.9 m tall: a boathouse-shaped tea room with a deep white veranda, a pine-green roof and, for a
    sign, a giant teapot on the ridge, steam drifting from its spout."""
    g = sc.still()
    g.box(STONE, (0, 0, -130), (1040, 820, 340))                # the plinth: top z 40
    g.box(WHITE, (0, 120, 200), (900, 460, 320))                # the tea room
    for k in range(5):
        if k != 2:
            g.box(AQUA, (-320 + k * 160, -112, 210), (110, 6, 200), glow=G1)
    g.box(LAMP, (0, -113, 190), (120, 8, 250), glow=G2)         # the door
    g.box(TEAL, (0, -255, 46), (980, 300, 12))                  # the veranda's deck
    for k in range(6):
        drum(g, -450 + k * 180, -380, 52, 16, 250)
    for k in range(5):
        if k != 2:
            g.box(WHITE, (-360 + k * 180, -380, 124), (150, 8, 10))
            g.box(TEAL, (-360 + k * 180, -380, 92), (150, 6, 34))
    tilt_box(g, WHITE, (0, -424, 292), (0, -100, 374), 1000, 12)   # the veranda's roof
    for k in range(10):
        g.box(TEAL if k % 2 else LAMP, (-450 + k * 100, -426, 276), (100, 6, 30))
    tilt_box(g, PINE, (0, -134, 356), (0, 125, 562), 960, 16)   # the main roof
    tilt_box(g, PINE, (0, 384, 356), (0, 115, 562), 960, 16)
    for sx in (-1, 1):
        g.box(WHITE, (sx * 446, 120, 410), (8, 360, 100))
        g.box(WHITE, (sx * 446, 120, 495), (8, 180, 70))
        g.box(AQUA, (sx * 451, 120, 420), (6, 90, 60), glow=G1)
    g.box(TEAL, (0, 120, 566), (1000, 30, 20))                  # the ridge
    # the sign: a giant teapot
    g.cyl(WHITE, (0, 120, 574), 52, 16, steps=8)
    g.sphere(WHITE, (0, 120, 668), 96, scale=(1.15, 1, 0.9), steps=(5, 8))
    g.cyl(TEAL, (0, 120, 660), 113, 16, steps=8)
    tilt_box(g, WHITE, (96, 120, 646), (196, 120, 726), 24, 24)
    tilt_box(g, WHITE, (-100, 120, 708), (-176, 120, 686), 16, 16)
    tilt_box(g, WHITE, (-176, 120, 686), (-108, 120, 616), 16, 16)
    g.cyl(TEAL, (0, 120, 750), 50, 16, steps=8, top_r=30)
    g.sphere(LAMP, (0, 120, 780), 14, steps=(3, 5))
    for k in range(3):                                          # steam: three puffs that drift
        s = sc.part(P.motion(bob=-10, rate=0.2, phase=k * 0.33, swing=4, pivot=(196, 120, 726), axis=0))
        s.sphere(WHITE, (212 + k * 24, 120, 776 + k * 62), 22 + k * 8, steps=(3, 6))
    for k in range(4):                                          # lanterns under the veranda
        x = -360 + k * 240
        g.box(WHITE, (x, -300, 300), (5, 5, 44))
        g.sphere(LAMP, (x, -300, 262), 17, steps=(3, 6), glow=G4)
    for x in (-300, 300):                                       # tables set for tea
        g.cyl(WHITE, (x, -260, 52), 5, 66, steps=5)
        g.cyl(WHITE, (x, -260, 118), 44, 6, steps=8)
        g.sphere(TEAL, (x, -260, 136), 13, steps=(3, 5))
        for e in (-1, 1):
            g.box(AQUA, (x + e * 74, -260, 74), (36, 36, 44))
    for sx in (-1, 1):                                          # clipped bays in tubs at the steps
        g.box(STONE, (sx * 110, -396, 70), (44, 44, 60))
        g.sphere(PINE, (sx * 110, -396, 132), 34, steps=(4, 6))


def carousel_house(sc):
    """10.8 m across, 8.6 m tall: a round glass house with a pleated roof; the carousel inside turns slowly (plain
    striped poles and benches under a lit canopy)."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -300), 540, 340, steps=12)
    g.cyl(WHITE, (0, 0, 40), 500, 24, steps=12)
    for k in range(12):
        a = 15 + k * 30
        x, y, _ = pol(480, a)
        g.cyl(WHITE, (x, y, 64), 14, 330, steps=5)
        tilt_box(g, AQUA, pol(480, a, 112), pol(480, a + 30, 112), 6, 90, glow=G1)       # the glass skirt
        tilt_box(g, AQUA, pol(480, a, 360), pol(480, a + 30, 360), 6, 66, glow=G1)       # the clerestory
        tilt_box(g, LAMP if k % 2 else WHITE, pol(546, a, 386), pol(546, a + 30, 386), 6, 36)   # the valance
        tilt_box(g, WHITE, pol(536, a, 424), pol(74, a, 652), 14, 14)                    # a pleat's rib
    g.cyl(WHITE, (0, 0, 396), 540, 22, steps=12)
    g.cyl(TEAL, (0, 0, 418), 534, 230, steps=12, top_r=70)
    g.cyl(WHITE, (0, 0, 648), 70, 50, steps=8)
    g.cyl(TEAL, (0, 0, 698), 90, 90, steps=8, top_r=8)
    g.sphere(LAMP, (0, 0, 800), 24, steps=(4, 6), glow=G4)
    f = sc.part(P.motion(swing=16, pivot=(0, 0, 840), axis=P.UP, rate=0.3))
    f.box(WHITE, (0, 0, 800), (5, 5, 90))
    f.box(TEAL, (34, 0, 836), (64, 5, 30))
    turn = dict(travel=(-1.0, 0.05), base=(0.0, 0.0))
    c = sc.part(P.motion(**turn))                               # the carousel: one round in 20 s
    c.cyl(TEAL, (0, 0, 64), 400, 16, steps=12)
    c.cyl(WHITE, (0, 0, 80), 80, 280, steps=8)
    c.cyl(TEAL, (0, 0, 150), 85, 40, steps=8)
    c.cyl(LAMP, (0, 0, 250), 85, 30, steps=8, glow=G2)
    c.cyl(LAMP, (0, 0, 348), 400, 12, steps=12, glow=G1)        # the lit canopy
    for k in range(8):
        a = k * 45
        x, y, _ = pol(300, a)
        c.cyl(TEAL if k % 2 else WHITE, (x, y, 80), 7, 268, steps=5)
        for z in (130, 200, 270):
            c.cyl(WHITE if k % 2 else TEAL, (x, y, z), 10, 26, steps=5)
        b = sc.part(P.motion(bob=-12, rate=0.25, phase=0.5 * (k % 2), **turn))            # the benches rise and fall
        col = AQUA if k % 2 else LAMP
        b.box(col, (x, y, 150), (96, 44, 12), rot=(0, a + 90, 0))
        p = pol(324, a, 186)
        b.box(col, p, (96, 8, 64), rot=(0, a + 90, 0))


def fountain_court(sc):
    """10.8 m across, 5.3 m tall: a ring of eight white arches round a stepped basin; the jets sway."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -300), 540, 330, steps=16)             # top z 30
    g.cyl(WHITE, (0, 0, 30), 330, 46, steps=12)
    g.cyl(AQUA, (0, 0, 70), 300, 10, steps=12, glow=G1)
    g.cyl(WHITE, (0, 0, 60), 44, 190, steps=8)
    g.cyl(WHITE, (0, 0, 250), 50, 46, steps=10, top_r=170)
    g.cyl(AQUA, (0, 0, 290), 160, 10, steps=10, glow=G1)
    g.cyl(WHITE, (0, 0, 296), 26, 120, steps=6)
    g.cyl(WHITE, (0, 0, 416), 30, 30, steps=8, top_r=90)
    g.cyl(AQUA, (0, 0, 442), 84, 8, steps=8, glow=G1)
    j = sc.part(P.motion(swing=3, pivot=(0, 0, 446), axis=0, rate=0.35))
    j.cyl(AQUA, (0, 0, 446), 12, 80, steps=5, top_r=5, glow=G1)
    for k in range(6):
        a = k * 60
        p = sc.part(P.motion(swing=4, pivot=pol(280, a, 76), axis=a + 90, rate=0.3, phase=k / 6.0))
        tilt_box(p, AQUA, pol(280, a, 76), pol(215, a, 250), 9, 9, glow=G1)
        tilt_box(p, AQUA, pol(215, a, 250), pol(150, a, 296), 9, 9, glow=G1)
    for k in range(8):
        a0 = 22.5 + k * 45
        a1 = a0 + 45
        x, y, _ = pol(500, a0)
        g.box(WHITE, (x, y, 195), (56, 56, 330), rot=(0, a0, 0))
        g.box(STONE, (x, y, 50), (70, 70, 40), rot=(0, a0, 0))
        p0, p3 = pol(500, a0, 360), pol(500, a1, 360)
        p1 = (p0[0] * 0.67 + p3[0] * 0.33, p0[1] * 0.67 + p3[1] * 0.33, 418)
        p2 = (p0[0] * 0.33 + p3[0] * 0.67, p0[1] * 0.33 + p3[1] * 0.67, 418)
        tilt_box(g, WHITE, p0, p1, 50, 40)
        tilt_box(g, WHITE, p1, p2, 50, 40)
        tilt_box(g, WHITE, p2, p3, 50, 40)
        tilt_box(g, TEAL, pol(503, a0, 446), pol(503, a1, 446), 58, 16)
        tilt_box(g, WHITE, pol(506, a0, 468), pol(506, a1, 468), 66, 28)
        g.cyl(STONE, (x, y, 482), 22, 36, steps=6, top_r=34)
        if k % 2:
            g.sphere(LAMP, (x, y, 546), 27, steps=(4, 6), glow=G4)
        else:
            g.sphere(PINE, (x, y, 548), 31, steps=(4, 6))


def palm_house(sc):
    """12.4 x 9.4 m, 17 m tall: a glass barrel vault with a glass dome on an eight-sided drum, palms inside."""
    g = sc.still()
    g.box(STONE, (0, 0, -125), (1240, 940, 350))                # top z 50
    g.box(WHITE, (0, 0, 80), (1180, 760, 60))
    g.box(AQUA, (0, 0, 300), (1160, 740, 380), glow=G1)         # the glass walls
    for k in range(9):
        g.box(WHITE, (-560 + k * 140, -372, 300), (14, 10, 380))
    for sx in (-1, 1):
        for k in range(4):
            g.box(WHITE, (sx * 582, -280 + k * 187, 300), (10, 14, 380))
    g.box(WHITE, (0, 0, 496), (1190, 770, 22))
    rod(g, AQUA, (-580, 0, 500), (580, 0, 500), 350, steps=12, glow=G1)   # the vault (its lower half is inside)
    for x in (-575, -290, 290, 575):                            # the vault's hoops
        for k in range(5):
            b0, b1 = math.radians(k * 36), math.radians((k + 1) * 36)
            tilt_box(g, WHITE, (x, 356 * math.cos(b0), 500 + 356 * math.sin(b0)), (x, 356 * math.cos(b1), 500 + 356 * math.sin(b1)), 14, 12)
    g.box(TEAL, (0, 0, 860), (1100, 20, 16))
    for x in (-430, -150, 150, 430):                            # palms seen through the glass
        g.box(PINE, (x, -375, 250), (16, 6, 270))
        for dx, dz in ((-90, 20), (-60, 80), (0, 105), (60, 80), (90, 20)):
            tilt_box(g, PINE, (x, -375, 384), (x + dx, -375, 384 + dz), 6, 28)
    g.cyl(WHITE, (0, 0, 496), 300, 560, steps=8)                # the drum, to 1056
    for k in range(8):
        a = 22.5 + k * 45
        g.box(AQUA, pol(303, a, 790), (130, 6, 400), rot=(0, a + 90, 0), glow=G1)
    g.cyl(WHITE, (0, 0, 1044), 318, 26, steps=12)
    g.sphere(AQUA, (0, 0, 1060), 310, scale=(1, 1, 1.15), steps=(6, 12), glow=G1)
    g.cyl(WHITE, (0, 0, 1396), 70, 150, steps=8)
    g.cyl(LAMP, (0, 0, 1440), 74, 60, steps=8, glow=G2)
    g.cyl(TEAL, (0, 0, 1546), 92, 110, steps=8, top_r=8)
    g.sphere(LAMP, (0, 0, 1672), 20, steps=(3, 6), glow=G4)
    g.box(WHITE, (0, -420, 250), (260, 100, 400))               # the porch
    g.box(LAMP, (0, -472, 230), (160, 6, 300), glow=G2)
    g.box(TEAL, (0, -430, 462), (300, 130, 24))
    g.box(STONE, (0, -452, 20), (300, 36, 60))
    for sx in (-1, 1):
        globe_post(g, sx * 190, -440, 50)


def library(sc):
    """12 x 9.4 m, 15 m tall: a colonnade of six fat drums under a shallow teal dome on a lit drum; an open book in
    stone either side of the steps (its sign)."""
    g = sc.still()
    g.box(STONE, (0, 70, -110), (1200, 800, 380))               # the plinth: y -330..470, top z 80
    for k in range(3):
        g.box(WHITE if k % 2 == 0 else STONE, (0, -447 + 46 * k, 13.5 * (k + 1) - 20), (700, 46, 27 * (k + 1) + 40))
    g.box(WHITE, (0, 120, 360), (1000, 560, 560))               # the hall: z 80..640
    for k in range(6):
        drum(g, -475 + k * 190, -250, 80, 44, 520)
    g.box(WHITE, (0, 60, 650), (1160, 760, 100))                # the entablature
    g.box(TEAL, (0, -322, 650), (1100, 6, 36))
    g.box(STONE, (0, 60, 712), (1200, 800, 24))
    g.box(LAMP, (0, -163, 300), (150, 8, 380), glow=G2)         # the door
    for x in (-380, -190, 190, 380):
        g.box(AQUA, (x, -163, 340), (90, 8, 300), glow=G1)
    for sx in (-1, 1):                                          # the open books
        x = sx * 440
        g.box(STONE, (x, -400, 60), (110, 110, 180))
        g.box(WHITE, (x - 27, -400, 162), (58, 96, 10), rot=(20, 0, 0))
        g.box(WHITE, (x + 27, -400, 162), (58, 96, 10), rot=(-20, 0, 0))
        g.box(TEAL, (x, -400, 160), (8, 100, 8))
        globe_post(g, sx * 300, -300, 80, h=170)
        g.cyl(STONE, (sx * 560, -290, 724), 24, 40, steps=6, top_r=38)       # urns on the corners
        g.sphere(PINE, (sx * 560, -290, 792), 34, steps=(4, 6))
    g.cyl(WHITE, (0, 120, 724), 330, 320, steps=12)             # the dome's drum, to 1044
    for k in range(8):
        a = 22.5 + k * 45
        p = pol(334, a, 890)
        g.box(LAMP, (p[0], p[1] + 120, p[2]), (110, 8, 190), rot=(0, a + 90, 0), glow=G2)
    g.cyl(TEAL, (0, 120, 1030), 342, 22, steps=12)
    g.sphere(TEAL, (0, 120, 1048), 345, scale=(1, 1, 0.62), steps=(6, 12))
    g.cyl(WHITE, (0, 120, 1250), 60, 90, steps=8)
    g.cyl(TEAL, (0, 120, 1340), 76, 40, steps=8, top_r=10)
    g.sphere(LAMP, (0, 120, 1420), 44, steps=(4, 6), glow=G4)


def belvedere(sc):
    """6.4 x 6.4 m, 17.6 m tall: a look-out of three storeys under three saucer roofs: an open arcade round a glass
    globe, a lit room, a lantern with a lamp; a stair up its side; a weather vane."""
    g = sc.still()
    g.box(STONE, (0, 0, -100), (640, 640, 400))                 # top z 100
    for sx in (-1, 1):
        for sy in (-1, 1):
            drum(g, sx * 250, sy * 250, 100, 40, 420)
            g.cyl(WHITE, (sx * 130, sy * 130, 1170), 16, 262, steps=5)
        tilt_box(g, WHITE, (-250, sx * 250, 470), (0, sx * 250, 524), 30, 30)
        tilt_box(g, WHITE, (250, sx * 250, 470), (0, sx * 250, 524), 30, 30)
        tilt_box(g, WHITE, (sx * 250, -250, 470), (sx * 250, 0, 524), 30, 30)
        tilt_box(g, WHITE, (sx * 250, 250, 470), (sx * 250, 0, 524), 30, 30)
    g.box(WHITE, (0, 0, 545), (600, 600, 50))
    g.cyl(STONE, (0, 0, 100), 60, 90, steps=8)
    g.sphere(AQUA, (0, 0, 256), 70, steps=(5, 8), glow=G1)      # the glass globe in the arcade
    saucer(g, 0, 0, 570, 470)
    g.box(WHITE, (0, 0, 880), (400, 400, 400))                  # the room: z 680..1080
    for k in range(4):
        a = k * 90
        g.box(AQUA, pol(202, a, 890), (6, 200, 240), rot=(0, a, 0), glow=G2)
        g.box(TEAL, pol(232, a, 760), (10, 470, 12), rot=(0, a, 0))                      # the balcony's rail
    saucer(g, 0, 0, 1080, 360)
    g.cyl(STONE, (0, 0, 1170), 50, 60, steps=6)
    g.sphere(LAMP, (0, 0, 1310), 80, steps=(5, 8), glow=G4)     # the lamp in the lantern
    saucer(g, 0, 0, 1432, 250)
    g.cyl(TEAL, (0, 0, 1500), 28, 150, steps=6, top_r=7)
    tilt_box(g, STONE, (340, -250, 100), (340, 250, 420), 60, 14)       # the stair up its side
    g.box(STONE, (340, 280, 250), (60, 60, 340))
    for k in range(4):
        g.box(WHITE, (366, -200 + k * 130, 220 + k * 83), (8, 8, 90))
    tilt_box(g, WHITE, (366, -250, 190), (366, 250, 510), 8, 8)
    vane(sc, 0, 0, 1640)


# ------------------------------------------------------------------------------------------ the hero
def orrery(sc):
    """THE ORRERY. A white rotunda (30 m across) on a stepped base under a teal dome; out of the dome a stem and a
    cradle carry an armillary sphere 60 m across: a fixed white meridian, and inside it three teal-and-white rings
    turning slowly about the upright (0.03, 0.045 and 0.07 rounds a second, two ways) round a warm steady globe; a
    planet rides the second ring and a moon the third. 120 m to the top."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -300), 1500, 450, steps=16)            # top z 150
    g.cyl(WHITE, (0, 0, 150), 1420, 100, steps=16)
    g.cyl(STONE, (0, 0, 250), 1340, 100, steps=16)             # top z 350
    g.box(WHITE, (0, -1400, 150), (700, 260, 300))             # the stair to the door
    g.cyl(WHITE, (0, 0, 350), 1050, 1500, steps=16)            # the wall, to 1850
    for k in range(12):
        x, y, _ = pol(1230, 15 + k * 30)
        drum(g, x, y, 350, 105, 1250)
    g.cyl(WHITE, (0, 0, 1600), 1350, 170, steps=16)            # the entablature
    g.cyl(TEAL, (0, 0, 1770), 1365, 50, steps=16)
    for k in range(6):                                          # lamps on the cornice
        p = pol(1290, 30 + k * 60)
        g.cyl(STONE, (p[0], p[1], 1820), 30, 50, steps=5)
        g.sphere(LAMP, (p[0], p[1], 1910), 46, steps=(3, 6), glow=G4)
    g.cyl(WHITE, (0, 0, 1820), 1000, 480, steps=16)            # the attic, to 2300
    for k in range(12):
        a = 15 + k * 30
        g.box(LAMP, pol(1002, a, 2060), (150, 10, 300), rot=(0, a + 90, 0), glow=G2)
    g.box(LAMP, (0, -1052, 900), (300, 14, 900), glow=G2)       # the great door
    g.box(WHITE, (0, -1062, 1390), (430, 30, 70))
    g.cyl(WHITE, (0, 0, 2290), 1030, 40, steps=16)
    g.sphere(TEAL, (0, 0, 2300), 1000, scale=(1, 1, 0.92), steps=(8, 16), glow=G1)       # the dome, to 3220
    for k in range(8):                                          # its ribs
        a = math.radians(k * 45)
        q = [(1008 * math.cos(f) * math.cos(a), 1008 * math.cos(f) * math.sin(a), 2300 + 928 * math.sin(f)) for f in (0.0, 0.75, 1.4)]
        tilt_box(g, WHITE, q[0], q[1], 50, 50)
        tilt_box(g, WHITE, q[1], q[2], 50, 50)
    g.cyl(AQUA, (0, 0, 3150), 330, 90, steps=12, glow=G2)       # the collar round the stem
    g.cyl(WHITE, (0, 0, 3200), 300, 2500, steps=8, top_r=170)   # the stem, to 5700
    g.cyl(TEAL, (0, 0, 4300), 262, 90, steps=8)
    g.cyl(TEAL, (0, 0, 5560), 200, 140, steps=8)
    C = (0.0, 0.0, 8800.0)
    for sx in (-1, 1):                                          # the cradle
        tilt_box(g, WHITE, (0, 0, 5600), (sx * 1545, 0, 6228), 240, 240)
    ring(g, (WHITE,), C, 3000, 90, 0, w=240, glow=0.12)        # the fixed meridian: it faces the road
    g.cyl(WHITE, (0, 0, 11900), 30, 60, steps=6)
    g.sphere(LAMP, (0, 0, 11985), 80, steps=(4, 6), glow=G4)
    g.sphere(LAMP, C, 700, steps=(6, 12), glow=1.2)             # the globe
    r1 = sc.part(P.motion(travel=(-1.0, 0.03), base=(0.0, 0.0)))
    ring(r1, (TEAL, TEAL, WHITE), C, 2600, 23, 0, w=200, glow=0.25)
    t = math.radians(23)
    n = (0.0, -math.sin(t), math.cos(t))
    tilt_box(r1, TEAL, tuple(C[i] - 1050 * n[i] for i in range(3)), tuple(C[i] + 1050 * n[i] for i in range(3)), 70, 70, glow=0.25)
    r2 = sc.part(P.motion(travel=(-1.0, -0.045), base=(0.0, 0.0)))
    at2 = ring(r2, (WHITE, AQUA), C, 2300, 55, 60, w=180, glow=0.25)
    r2.sphere(AQUA, at2(40), 230, steps=(4, 7), glow=G1)        # a planet
    r3 = sc.part(P.motion(travel=(-1.0, 0.07), base=(0.0, 0.0)))
    at3 = ring(r3, (TEAL, WHITE), C, 2000, 80, 120, w=160, glow=0.25)
    r3.sphere(WHITE, at3(200), 170, steps=(4, 7), glow=0.2)     # a moon


# ------------------------------------------------------------------------------------------ props
def globe_lamps(sc):
    """A cluster of five warm globes on a white post, 7 m: steady light. A flower basket either side."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -100), 60, 150, steps=8)
    g.cyl(WHITE, (0, 0, 50), 34, 60, steps=8, top_r=16)
    g.cyl(WHITE, (0, 0, 110), 11, 430, steps=6)
    g.cyl(TEAL, (0, 0, 300), 17, 24, steps=6)
    g.cyl(TEAL, (0, 0, 520), 20, 24, steps=6)
    for k in range(4):
        e = pol(95, 45 + k * 90, 560)
        tilt_box(g, WHITE, (0, 0, 522), e, 9, 9)
        g.cyl(TEAL, (e[0], e[1], 556), 14, 10, steps=6)
        g.sphere(LAMP, (e[0], e[1], 602), 36, steps=(4, 7), glow=G4)
    g.cyl(WHITE, (0, 0, 540), 9, 70, steps=5)
    g.sphere(LAMP, (0, 0, 655), 46, steps=(4, 7), glow=G4)
    for sx in (-1, 1):
        g.box(WHITE, (sx * 34, 0, 340), (60, 6, 6))
        g.cyl(TEAL, (sx * 60, 0, 290), 12, 26, steps=6, top_r=26)
        g.sphere(WHITE if sx > 0 else AQUA, (sx * 60, 0, 326), 24, scale=(1, 1, 0.7), steps=(3, 6))
        g.sphere(PINE, (sx * 74, -8, 318), 15, steps=(3, 5))


def sundial(sc):
    """A sundial on a marble drum, 3.1 m: a teal dial with twelve gold hours and a white gnomon."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -100), 190, 125, steps=12)
    g.cyl(WHITE, (0, 0, 25), 150, 22, steps=12)
    g.cyl(WHITE, (0, 0, 47), 105, 150, steps=10)
    g.cyl(STONE, (0, 0, 60), 109, 14, steps=10)
    g.cyl(STONE, (0, 0, 170), 109, 14, steps=10)
    g.cyl(TEAL, (0, 0, 197), 122, 9, steps=12)
    for k in range(12):
        p = pol(100, k * 30, 208.5)
        g.box(LAMP, p, (18, 7, 5), rot=(0, k * 30, 0))
    tilt_box(g, WHITE, (0, -76, 208), (0, 62, 296), 9, 12)      # the gnomon
    g.box(WHITE, (0, 58, 250), (9, 12, 88))
    g.box(WHITE, (0, 22, 228), (9, 70, 44))
    g.sphere(LAMP, (0, 62, 306), 12, steps=(3, 5))


def bench_ring(sc):
    """Six teal benches back to back round a tree in a stone ring, 5.4 m: the crown rocks."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -100), 150, 150, steps=12)
    g.cyl(PINE, (0, 0, 50), 132, 8, steps=12)
    g.cyl(STONE, (0, 0, 50), 20, 300, steps=6, top_r=13)
    for k in range(6):
        a = k * 60
        t = math.radians(a)
        c = pol(205, a, 46)
        g.box(TEAL, c, (130, 44, 9), rot=(0, a + 90, 0))
        g.box(TEAL, pol(182, a, 78), (130, 8, 44), rot=(0, a + 90, 0))
        for e in (-1, 1):
            g.box(WHITE, (c[0] - e * 52 * math.sin(t), c[1] + e * 52 * math.cos(t), 22), (8, 40, 44), rot=(0, a + 90, 0))
    crown = sc.part(P.motion(swing=2.5, pivot=(0, 0, 50), axis=30, rate=0.3))
    crown.sphere(PINE, (0, 0, 420), 150, scale=(1, 1, 0.8), steps=(5, 8))
    crown.sphere(PINE, (90, 40, 356), 100, steps=(4, 7))
    crown.sphere(PINE, (-80, -50, 366), 105, steps=(4, 7))


def boat_rack(sc):
    """The boat hire's rack, 4.2 m: four little rowing boats stood on end against a white frame, oars, a life ring,
    a pennant that swings."""
    g = sc.still()
    g.box(STONE, (0, 0, -45), (330, 150, 110))                  # top z 10
    for sx in (-1, 1):
        g.box(WHITE, (sx * 150, 40, 215), (12, 12, 410))
        tilt_box(g, WHITE, (sx * 150, -55, 10), (sx * 150, 36, 300), 10, 10)
    g.box(WHITE, (0, 40, 400), (320, 12, 12))
    g.box(WHITE, (0, 40, 200), (320, 12, 10))
    cols = (TEAL, WHITE, AQUA, TEAL)
    for k in range(4):
        x = -105 + k * 70
        tilt_box(g, cols[k], (x, -30, 14), (x, 30, 330), 56, 24)          # the hull
        tilt_box(g, cols[k], (x, 30, 328), (x, 40, 392), 32, 20)          # its bow
        tilt_box(g, WHITE if cols[k] != WHITE else TEAL, (x, -46, 20), (x, 14, 330), 14, 6)   # the keel's stripe
        g.box(LAMP, (x, -2, 150), (50, 8, 10))
    for k in range(3):                                          # oars
        tilt_box(g, WHITE, (132 + k * 9, -52, 10), (138 + k * 9, 30, 360), 6, 6)
        g.box(LAMP, (138 + k * 9, 34, 384), (8, 20, 56))
    rod(g, WHITE, (-150, 30, 330), (-150, 18, 330), 30, steps=8)          # a life ring
    rod(g, TEAL, (-150, 18, 330), (-150, 12, 330), 15, steps=6)
    f = sc.part(P.motion(swing=14, pivot=(-150, 40, 420), axis=P.UP, rate=0.3))
    f.box(TEAL, (-116, 40, 404), (68, 6, 30))


def fountain_jets(sc):
    """A fountain of three bowls, 5.3 m: a tall centre jet and six arcing jets that sway."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -100), 240, 160, steps=12)             # top z 60
    g.cyl(AQUA, (0, 0, 56), 214, 10, steps=12, glow=G1)
    g.cyl(WHITE, (0, 0, 60), 40, 120, steps=8)
    g.cyl(WHITE, (0, 0, 180), 46, 40, steps=10, top_r=130)
    g.cyl(AQUA, (0, 0, 214), 122, 10, steps=10, glow=G1)
    g.cyl(WHITE, (0, 0, 220), 24, 90, steps=6)
    g.cyl(WHITE, (0, 0, 310), 28, 26, steps=8, top_r=74)
    g.cyl(AQUA, (0, 0, 332), 68, 8, steps=8, glow=G1)
    j = sc.part(P.motion(swing=3, pivot=(0, 0, 336), axis=0, rate=0.35))
    j.cyl(AQUA, (0, 0, 336), 13, 185, steps=5, top_r=5, glow=G1)
    j.sphere(WHITE, (0, 0, 524), 13, steps=(3, 5))
    for k in range(6):
        a = k * 60
        p = sc.part(P.motion(swing=5, pivot=pol(200, a, 66), axis=a + 90, rate=0.3, phase=k / 6.0))
        tilt_box(p, AQUA, pol(200, a, 66), pol(150, a, 250), 9, 9, glow=G1)
        tilt_box(p, AQUA, pol(150, a, 250), pol(95, a, 290), 9, 9, glow=G1)
        tilt_box(p, AQUA, pol(95, a, 290), pol(62, a, 236), 8, 8, glow=G1)


def pergola(sc):
    """A pergola walk, 12 x 2.8 m, 3.5 m tall: ten white drums, rafters, climbers, four hanging baskets that swing."""
    g = sc.still()
    g.box(STONE, (0, 0, -45), (1220, 280, 110))                 # top z 10
    for k in range(5):
        x = -560 + k * 280
        for sy in (-1, 1):
            drum(g, x, sy * 110, 10, 17, 290)
        if k % 2 == 0:                                          # a climber up the back drum
            for z in (150, 205, 262):
                g.sphere(PINE, (x, 112, z), 30, steps=(3, 6))
    for sy in (-1, 1):
        g.box(WHITE, (0, sy * 110, 312), (1200, 16, 22))
    for k in range(11):
        g.box(WHITE, (-575 + k * 115, 0, 332), (12, 300, 16))
    for x in (-420, 140):
        g.box(PINE, (x, 30, 346), (250, 190, 14))
    cols = (WHITE, LAMP, AQUA, WHITE)
    for k in range(4):
        x = -420 + k * 280
        b = sc.part(P.motion(swing=5, pivot=(x, -110, 300), axis=0, rate=0.3, phase=k * 0.27))
        b.box(WHITE, (x, -110, 272), (5, 5, 56))
        b.cyl(TEAL, (x, -110, 214), 13, 30, steps=6, top_r=30)
        b.sphere(cols[k], (x, -110, 256), 30, scale=(1, 1, 0.6), steps=(3, 6))
        b.sphere(PINE, (x + 16, -118, 246), 18, steps=(3, 5))
    for x in (-280, 280):                                       # a bench under it
        g.box(TEAL, (x, 40, 50), (180, 44, 9))
        g.box(TEAL, (x, 64, 84), (180, 8, 46))
        for e in (-1, 1):
            g.box(WHITE, (x + e * 76, 44, 28), (8, 40, 40))


def urn_balustrade(sc):
    """LOW (top 1.9 m): 10 m of white balustrade between three piers with clipped balls in urns, tulips at its foot."""
    g = sc.still()
    g.box(STONE, (0, 0, -40), (1000, 50, 110))                  # top z 15
    for k in range(14):
        x = -455 + k * 70
        if abs(x) > 40 and abs(abs(x) - 470) > 40:
            g.cyl(WHITE, (x, 0, 15), 9, 62, steps=5, top_r=7)
    g.box(WHITE, (0, 0, 82), (1000, 30, 14))
    for x in (-470, 0, 470):
        g.box(WHITE, (x, 0, 60), (60, 60, 100))
        g.cyl(STONE, (x, 0, 110), 14, 22, steps=6, top_r=30)
        g.cyl(STONE, (x, 0, 132), 30, 18, steps=6, top_r=22)
        g.sphere(PINE, (x, 0, 168), 20, steps=(4, 6))
    cols = (LAMP, WHITE, TEAL)
    for k in range(12):
        x = -412 + k * 75
        g.box(PINE, (x, -42, 22), (8, 8, 30))
        g.box(cols[k % 3], (x, -42, 44), (20, 20, 20))


def yacht_pond(sc):
    """LOW (top 1.7 m): a round stone pond 9.6 m across, lily pads, a little lighthouse with a lamp, five model yachts
    sailing round it at their own slow speeds."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -100), 480, 140, steps=16)             # the rim: top z 40
    g.cyl(AQUA, (0, 0, 34), 440, 10, steps=16, glow=G1)
    g.cyl(WHITE, (0, 0, 44), 26, 60, steps=8)
    g.cyl(TEAL, (0, 0, 104), 20, 34, steps=8, top_r=14)
    g.sphere(LAMP, (0, 0, 152), 15, steps=(3, 6), glow=G4)
    for k in range(5):
        p = pol(120 + (k * 97) % 260, k * 77 + 20, 44)
        g.cyl(PINE, p, 24, 5, steps=6)
        if k % 2 == 0:
            g.sphere(WHITE, (p[0], p[1], 53), 9, steps=(3, 5))
    boats = ((150, 0.03, TEAL), (225, -0.022, WHITE), (300, 0.018, LAMP), (375, -0.014, TEAL), (300, 0.018, AQUA))
    for k, (r, rate, col) in enumerate(boats):
        a = k * 72 + (180 if k == 4 else 0)
        t = math.radians(a)
        y = sc.part(P.motion(travel=(-1.0, rate), base=(0.0, 0.0), bob=-2, rate=0.4, phase=k * 0.2))
        p = pol(r, a, 0)
        y.box(col, (p[0], p[1], 52), (72, 22, 16), rot=(0, a + 90, 0))
        y.box(WHITE, (p[0], p[1], 106), (5, 5, 96))
        y.box(WHITE, (p[0] - 18 * math.sin(t), p[1] + 18 * math.cos(t), 104), (34, 5, 76), rot=(0, a + 90, 0))
        y.box(TEAL if col != TEAL else LAMP, (p[0], p[1], 158), (14, 5, 8), rot=(0, a + 90, 0))


JOBS = [
    ('SM_S1D4_Bandstand', bandstand),
    ('SM_S1D4_TeaPavilion', tea_pavilion),
    ('SM_S1D4_CarouselHouse', carousel_house, SMALL),
    ('SM_S1D4_FountainCourt', fountain_court, SMALL),
    ('SM_S1D4_PalmHouse', palm_house, STILL),
    ('SM_S1D4_Library', library, STILL),
    ('SM_S1D4_Belvedere', belvedere),
    ('SM_S1D4_Orrery', orrery, dict(far=True, grow=(200, 1500, 100), grow_down=(200, 1500, 0))),
    ('SM_S1D4_GlobeLamps', globe_lamps, STILL),
    ('SM_S1D4_Sundial', sundial, STILL),
    ('SM_S1D4_BenchRing', bench_ring, SMALL),
    ('SM_S1D4_BoatRack', boat_rack, SMALL),
    ('SM_S1D4_FountainJets', fountain_jets, SMALL),
    ('SM_S1D4_Pergola', pergola, SMALL),
    ('SM_S1D4_UrnBalustrade', urn_balustrade, STILL),
    ('SM_S1D4_YachtPond', yacht_pond, dict(grow=(10, 10, 10), grow_down=(10, 10, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
