"""City Center, Stage 1, builder s1plants: what grows on the avenue. /Game/World/CityCenter/SM_S1PLANTS_*.
Run: import ft_world_city_s1plants as K; K.run()

Twenty-four plant pieces, every one an ft_people.Scene (one section, M_People), each with parts that sway or stir in
the vertex shader. Placed by C:\\ftwork\\s1\\L7_s1plants.inl (made by C:\\ftwork\\jobs\\L7s1plants\\place.py).

The avenue is a canyon, so there are three kinds of piece and each is drawn for its place:
  KERB   low (top under 2 m, 7 m by 1.5 m at the most): stands on the sidewalk BEHIND the parked cars, 20.8 to 22.4 m out
  ROOF   tall, in its own trough or tub (the piece brings it): stands on a podium's roof at the front edge, z = 0 the roof
  OPEN   tall, with a footing: the Plaza's open blocks only (the willow and the magnolia)
Colours are the kit's (ft_dress_metro.py) and each district's own of the zone plan (L7_plan.md). Nothing here is lit.
No green or orange box, drum or ball between 0.6 and 2 m (the pickups), no cone trio, nothing that bobs like a pickup.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'

PINE, GREEN_D, LEAF, LIME = '#2f6b52', '#24403a', '#3fae6a', '#b8ff5a'
CREAM, WHITE, RED, AMBER, TEAL, AQUA = '#fff1d6', '#fff6ea', '#ff2d3c', '#ffd23f', '#2fc6c0', '#9ff0ff'
MAG, PLUM, LILAC, BLUSH, SILVER, VIOLET = '#ff4fb8', '#4a3a78', '#c391d8', '#f3c4dc', '#c9c4d6', '#8662b8'
WOOD, WOOD_D, BRICK, OXBLOOD, TAN, SAND = '#9a6a44', '#6a4c36', '#6a3f44', '#54343c', '#8a6f5c', '#b89a6a'
STONE_L, CONC, IRON, BLACK, GOLD, STEEL, WARM = '#9a98a8', '#5a5870', '#3f3d50', '#1c1a26', '#ffc93c', '#2a3a54', '#ffe28a'

LOW = dict(grow=(25, 25, 20), grow_down=(25, 25, 0))       # a kerb piece: its bounds stay under 2.5 m (no shadow, by the track's rule)
TALL = dict(grow=(40, 40, 30), grow_down=(40, 40, 0))


def sway(sc, pivot, swing=3.0, axis=20.0, rate=0.35, phase=0.0):
    """The model for everything that rocks about `pivot` (the foot of a stem, the top of a hanging trail)."""
    return sc.part(P.motion(swing=swing, pivot=pivot, axis=axis, rate=rate, phase=phase))


def blob(m, col, c, r, sq=(1, 1, 1), steps=(4, 6)):
    m.sphere(col, c, r, scale=sq, steps=steps)


# ================================================================================================ KERB: low pieces
def geranium_run(sc):
    """Fifth and Main: three striped window boxes on a trestle, red geraniums, ivy trailing from the ends."""
    r = random.Random(11)
    g = sc.still()
    for x in (-300, 0, 300):                                    # the trestle
        tilt_box(g, WOOD, (x - 30, -34, -30), (x, 0, 86), 7, 7)
        tilt_box(g, WOOD, (x + 30, 34, -30), (x, 0, 86), 7, 7)
    g.box(WOOD_D, (0, 0, 84), (680, 44, 8))
    g.box(WOOD_D, (0, 0, 30), (600, 6, 6))
    for k, x in enumerate((-220, 0, 220)):
        g.box(CREAM, (x, 0, 106), (196, 46, 36))
        for j in range(4):
            g.box(RED, (x - 72 + j * 48, -24, 106), (22, 3, 36))
        g.box(GREEN_D, (x, 0, 126), (184, 38, 6))
        for j in range(3):                                      # three heads a box, each nodding on its own
            cx = x - 60 + j * 60
            f = sway(sc, (cx, 0, 126), swing=5, axis=r.uniform(0, 180), rate=r.uniform(0.4, 0.6), phase=r.random())
            f.box(PINE, (cx, 0, 140), (5, 5, 28))
            blob(f, LEAF, (cx, 4, 146), 22, (1.2, 1, 0.5), (3, 5))
            blob(f, RED, (cx, -6, 162), 15, (1.2, 1, 0.8), (3, 6))
            blob(f, MAG if (j + k) % 3 == 0 else RED, (cx + 14, 6, 156), 10, steps=(3, 5))
    for s in (-1, 1):                                           # ivy trails off both ends and swings
        for j in range(3):
            x = s * (322 + j * 4)
            t = sway(sc, (x, 0, 122), swing=7, axis=90, rate=0.45 + 0.07 * j, phase=r.random())
            t.box((PINE, LEAF, GREEN_D)[j], (x, -14 + j * 14, 122 - (38 + j * 14) / 2.0), (12, 6, 38 + j * 14))
    g.cyl(TEAL, (-330, 30, 0), 14, 30, steps=6)                 # a watering can under the trestle
    tilt_box(g, TEAL, (-330, 30, 20), (-300, 30, 38), 5, 5)


def camellia_tubs(sc):
    """Theatre Row: three camellias in plum tubs by a stage door, blush and magenta flowers, petals on the ground."""
    r = random.Random(12)
    g = sc.still()
    for k, x in enumerate((-240, 0, 240)):
        g.cyl(PLUM, (x, 0, -30), 44, 92, steps=8, top_r=52)
        g.cyl(LILAC, (x, 0, 44), 52, 9, steps=8)
        g.cyl(SILVER, (x, 0, 0), 47, 6, steps=8)
        b = sway(sc, (x, 0, 62), swing=3, axis=r.uniform(0, 180), rate=r.uniform(0.3, 0.45), phase=r.random())
        b.cyl(WOOD_D, (x, 0, 60), 7, 34, steps=5)
        blob(b, GREEN_D, (x, 0, 132), 54, (1, 1, 0.95), (5, 8))
        blob(b, PINE, (x + 10, -8, 150), 36, (1, 1, 0.8), (4, 6))
        for j in range(7):
            a = j * 0.9 + k
            col = (BLUSH, MAG, BLUSH, WHITE)[(j + k) % 4]
            blob(b, col, (x + math.cos(a) * 46, math.sin(a) * 40 - 8, 118 + (j % 4) * 16), 10, (1, 1, 0.7), (3, 5))
        for j in range(4):
            g.box(BLUSH, (x + r.uniform(-90, 90), r.uniform(-60, 60), 1.5), (12, 9, 3), rot=(0, r.uniform(0, 90), 0))
    g.box(PLUM, (0, 62, 4), (700, 8, 8))                        # a plum kerb rail behind them


def sumac_low(sc):
    """The Garages: buddleia and a young sumac out of cracked concrete."""
    r = random.Random(13)
    g = sc.still()
    for k in range(5):                                          # the slab, broken and heaved
        x = -280 + k * 140
        g.box(STONE_L, (x, r.uniform(-12, 12), 4 + r.uniform(0, 8)), (128, 120, 16), rot=(r.uniform(-7, 7), r.uniform(-8, 8), r.uniform(-5, 5)))
    g.box(CONC, (0, 0, -18), (700, 110, 30))
    for k, x in enumerate((-250, -90, 120, 270)):               # buddleia: arching stems, lilac spikes nodding at the tips
        for j in range(3):
            a = math.radians(r.uniform(0, 360))
            tip = (x + math.cos(a) * 58, math.sin(a) * 40, r.uniform(116, 158))
            s = sway(sc, (x, 0, 8), swing=5, axis=math.degrees(a) + 90, rate=r.uniform(0.35, 0.6), phase=r.random())
            tilt_box(s, PINE, (x, 0, 6), tip, 6, 6)
            s.box(LEAF, ((x + tip[0]) / 2.0, tip[1] / 2.0, tip[2] * 0.55), (34, 8, 5), rot=(20, math.degrees(a), 0))
            blob(s, LILAC if (j + k) % 2 else VIOLET, (tip[0], tip[1], tip[2] + 4), 9, (1, 1, 2.3), (3, 5))
    for x in (-170, 30, 200):                                   # sumac: a stem, leaf fans, a dark red candle
        s = sway(sc, (x, 20, 8), swing=4, axis=r.uniform(0, 180), rate=0.4, phase=r.random())
        s.box(WOOD_D, (x, 20, 60), (7, 7, 110))
        for j in range(4):
            s.box(PINE if j % 2 else LEAF, (x, 20, 84 + j * 12), (86 - j * 8, 10, 4), rot=(0, j * 47, 14))
        blob(s, OXBLOOD, (x, 20, 134), 10, (1, 1, 1.9), (3, 5))


def tulip_bed(sc):
    """The Plaza: three teal-edged squares of tulips, a colour a square."""
    r = random.Random(14)
    g = sc.still()
    for k, (x, cols) in enumerate(((-250, (RED, RED, AMBER)), (0, (AMBER, WHITE, AMBER)), (250, (MAG, WHITE, MAG)))):
        g.box(TEAL, (x, 0, -4), (176, 150, 52))
        g.box(GREEN_D, (x, 0, 23), (156, 130, 4))
        g.box(WHITE, (x, -76, 10), (150, 3, 10))
        for row in range(3):                                    # a row of three sways together, each row out of step
            y = -42 + row * 42
            f = sway(sc, (x, y, 24), swing=6, axis=0, rate=0.5 + 0.06 * row, phase=r.random())
            for j in range(3):
                tx = x - 50 + j * 50 + r.uniform(-8, 8)
                h = r.uniform(44, 58)
                f.box(PINE, (tx, y, 24 + h / 2.0), (5, 5, h))
                f.box(LEAF, (tx + 7, y, 40), (6, 14, 26), rot=(0, 0, 18))
                f.cyl(cols[(row + j) % 3], (tx, y, 22 + h), 7, 17, steps=5, top_r=11)


def box_hedge(sc):
    """The Plaza and Wall Street: a clipped box hedge in a stone kerb, a ball at each end."""
    r = random.Random(15)
    g = sc.still()
    g.box(STONE_L, (0, 0, -8), (700, 110, 44))
    g.box(GREEN_D, (0, 0, 54), (520, 78, 82))
    g.box(PINE, (0, 0, 100), (500, 62, 12))
    for x in (-160, 0, 160):
        g.box(PINE, (x, 0, 110), (120, 56, 16))
    for s in (-1, 1):
        b = sway(sc, (s * 300, 0, 14), swing=2.5, axis=90, rate=0.4, phase=r.random())
        b.cyl(WOOD_D, (s * 300, 0, 14), 7, 40, steps=5)
        blob(b, GREEN_D, (s * 300, 0, 96), 45, steps=(5, 8))
        blob(b, PINE, (s * 300, 0, 124), 24, (1, 1, 0.7), (3, 6))
    for k in range(10):                                         # shoots the shears missed, stirring
        x = -230 + k * 51
        f = sway(sc, (x, 0, 110), swing=9, axis=r.uniform(0, 180), rate=r.uniform(0.6, 0.9), phase=r.random())
        f.box(LEAF, (x, r.uniform(-18, 18), 124), (5, 5, 20))
        f.box(LEAF, (x + 5, r.uniform(-18, 18), 131), (12, 5, 5))


def rose_bed(sc):
    """Penthouse Row and the Syndicate's quarter: red roses behind an iron rail."""
    r = random.Random(16)
    g = sc.still()
    g.box(OXBLOOD, (0, 10, -8), (680, 100, 40))
    g.box(GREEN_D, (0, 10, 13), (660, 84, 4))
    for k in range(7):                                          # the rail, towards the road
        x = -330 + k * 110
        g.box(BLACK, (x, -48, 34), (7, 7, 100))
        g.sphere(IRON, (x, -48, 90), 8, steps=(3, 5))
    for z in (36, 72):
        g.box(BLACK, (0, -48, z), (664, 5, 5))
    for k in range(9):
        x = -300 + k * 75
        y = 14 + (k % 2) * 18
        b = sway(sc, (x, y, 14), swing=1.5, axis=r.uniform(0, 180), rate=r.uniform(0.25, 0.4), phase=r.random())   # fix round: 3 > 1.5 (the arena is still)
        b.box(PINE, (x, y, 36), (6, 6, 44))
        blob(b, GREEN_D, (x, y, 66), 30, (1.1, 1, 0.9), (4, 6))
        for j in range(3):
            a = j * 2.1 + k
            blob(b, RED if (j + k) % 4 else WHITE, (x + math.cos(a) * 24, y + math.sin(a) * 18 - 8, 78 + j * 9), 10, (1, 1, 0.8), (3, 5))


def thistle_pampas(sc):
    """The circuit's waste ground: pampas plumes and thistles in a heap of clinker."""
    r = random.Random(17)
    g = sc.still()
    g.box(STEEL, (0, 0, -12), (640, 110, 32))
    for k in range(6):
        blob(g, STONE_L, (-270 + k * 108 + r.uniform(-20, 20), r.uniform(-30, 30), 4), r.uniform(20, 34), (1.3, 1, 0.5), (3, 5))
    for x in (-210, 40, 250):                                   # pampas: a fountain of blades, cream plumes over it
        for j in range(7):
            a = math.radians(j * 51 + x)
            tilt_box(g, PINE if j % 2 else LEAF, (x, 0, 4), (x + math.cos(a) * 62, math.sin(a) * 44, 84 + (j % 3) * 10), 6, 4)
        for j in range(4):
            a = math.radians(j * 90 + 30 + x)
            tip = (x + math.cos(a) * 30, math.sin(a) * 22, 128 + j * 8)
            f = sway(sc, (x, 0, 6), swing=6, axis=math.degrees(a) + 90, rate=r.uniform(0.3, 0.5), phase=r.random())
            tilt_box(f, TAN, (x, 0, 6), tip, 5, 5)
            blob(f, CREAM, (tip[0], tip[1], tip[2] + 4), 11, (1, 1, 2.5), (3, 6))
    for x in (-320, -90, 150, 320):                             # thistles: violet heads on silver stalks
        f = sway(sc, (x, 20, 4), swing=5, axis=r.uniform(0, 180), rate=0.55, phase=r.random())
        f.box(SILVER, (x, 20, 52), (5, 5, 96))
        f.box(SILVER, (x, 20, 60), (40, 5, 5), rot=(24, 30, 0))
        f.box(SILVER, (x, 20, 82), (34, 5, 5), rot=(-24, 100, 0))
        blob(f, SILVER, (x, 20, 102), 11, steps=(3, 5))
        blob(f, VIOLET, (x, 20, 114), 10, (1.2, 1.2, 0.8), (3, 6))


def hang_trough(sc):
    """Penthouse Row: a hanging garden in a white trough on silver legs, green spilling over its rim, white roses."""
    r = random.Random(18)
    g = sc.still()
    for x in (-260, 260):
        g.box(SILVER, (x, 0, 40), (10, 60, 140))
        g.box(SILVER, (x, 0, -24), (30, 90, 12))
    g.box(WHITE, (0, 0, 126), (620, 70, 44))
    g.box(BLUSH, (0, -36, 140), (620, 3, 8))
    g.box(GREEN_D, (0, 0, 150), (600, 58, 6))
    for k in range(8):                                          # mounds on top, rocking
        x = -262 + k * 75
        b = sway(sc, (x, 0, 150), swing=3, axis=r.uniform(0, 180), rate=0.4, phase=r.random())
        blob(b, PINE if k % 2 else LEAF, (x, 0, 162), 30, (1.2, 1, 0.6), (3, 6))
        blob(b, WHITE, (x + 10, -12, 176), 9, steps=(3, 5))
        if k % 3 == 0:
            blob(b, BLUSH, (x - 14, 8, 174), 9, steps=(3, 5))
    for k in range(11):                                         # the trails over the front rim, each swinging from its top
        x = -280 + k * 56
        L = r.uniform(44, 96)
        t = sway(sc, (x, -38, 148), swing=8, axis=0, rate=r.uniform(0.35, 0.6), phase=r.random())
        t.box((PINE, LEAF, GREEN_D)[k % 3], (x, -40, 148 - L / 2.0), (16, 6, L))
        t.box(LEAF, (x + 9, -42, 148 - L), (10, 5, 14))


def laurel_urns(sc):
    """Wall Street: four clipped laurel balls in sandstone urns on one long step."""
    r = random.Random(19)
    g = sc.still()
    g.box(SAND, (0, 0, -8), (680, 100, 44))
    g.box(CREAM, (0, 0, 16), (660, 84, 5))
    for k, x in enumerate((-255, -85, 85, 255)):
        g.cyl(TAN, (x, 0, 18), 26, 10, steps=8)
        g.cyl(SAND, (x, 0, 28), 16, 16, steps=8, top_r=34)
        g.cyl(SAND, (x, 0, 44), 34, 34, steps=8, top_r=40)
        g.cyl(CREAM, (x, 0, 78), 43, 7, steps=8)
        b = sway(sc, (x, 0, 84), swing=2.5, axis=r.uniform(0, 180), rate=0.35, phase=r.random())
        b.cyl(WOOD_D, (x, 0, 84), 6, 26, steps=5)
        blob(b, GREEN_D, (x, 0, 146), 42, steps=(5, 8))
        blob(b, PINE, (x - 8, -10, 160), 26, (1, 1, 0.8), (3, 6))


def bramble(sc):
    """The circuit: a bramble bank grown over a length of yard fence, canes arching out of it."""
    r = random.Random(20)
    g = sc.still()
    for k in range(5):
        x = -320 + k * 160
        g.box(STONE_L, (x, 30, 50), (8, 8, 160))
    for z in (40, 86, 126):
        g.box(SILVER, (0, 30, z), (648, 5, 5))
    for k in range(7):
        x = -270 + k * 90
        blob(g, GREEN_D if k % 2 else PINE, (x, 6 + r.uniform(-8, 8), r.uniform(34, 50)), r.uniform(50, 64), (1.3, 0.8, 0.9), (4, 6))
    for k in range(9):                                          # the canes: thorny arches that nod, a blossom or berries on each
        x = -300 + k * 75
        a = math.radians(r.uniform(200, 340))
        tip = (x + math.cos(a) * 56, 6 + math.sin(a) * 50, r.uniform(96, 150))
        c = sway(sc, (x, 6, 50), swing=6, axis=math.degrees(a) + 90, rate=r.uniform(0.35, 0.6), phase=r.random())
        tilt_box(c, OXBLOOD, (x, 6, 50), tip, 5, 5)
        c.box(LEAF, ((x + tip[0]) / 2.0, (6 + tip[1]) / 2.0, (50 + tip[2]) / 2.0 + 6), (26, 10, 5), rot=(0, math.degrees(a), 20))
        blob(c, WHITE if k % 3 else VIOLET, tip, 8, steps=(3, 5))
        if k % 3 == 0:
            blob(c, PLUM, (tip[0] + 8, tip[1], tip[2] - 8), 6, steps=(3, 5))


def yew_drums(sc):
    """The Syndicate's quarter: clipped yew, a drum and a cube turn about, in gold-banded black tubs. It hardly stirs."""
    r = random.Random(21)
    g = sc.still()
    g.box(IRON, (0, 0, -12), (660, 96, 30))
    for k, x in enumerate((-255, -85, 85, 255)):
        g.cyl(BLACK, (x, 0, 0), 38, 50, steps=8, top_r=44)
        g.cyl(GOLD, (x, 0, 34), 44, 7, steps=8)
        b = sway(sc, (x, 0, 50), swing=1.5, axis=r.uniform(0, 180), rate=0.25, phase=r.random())
        # fix round: no clipped cube or drum (a green box of pickup size by the barrier): four cones, tall and short in turn
        b.cyl(GREEN_D, (x, 0, 56), 36, 92 if k % 2 else 74, steps=8, top_r=9)
        b.sphere(PINE, (x, 0, 152 if k % 2 else 134), 9, steps=(3, 5))
        b.cyl(WOOD_D, (x, 0, 48), 7, 12, steps=5)


# ================================================================================================ ROOF: tall pieces in their own tubs
def plane_box(sc):
    """Fifth and Main, Wall Street: a London plane in a cast-iron planter, its trunk mottled cream and olive."""
    r = random.Random(31)
    g = sc.still()
    g.box(IRON, (0, 0, 28), (220, 120, 64))
    g.box(BLACK, (0, 0, 62), (236, 136, 8))
    for k in range(7):                                          # the grate
        g.box(SILVER, (-90 + k * 30, 0, 67), (6, 110, 4))
    g.box(RED, (0, -61, 30), (150, 3, 14))
    c = sway(sc, (0, 0, 60), swing=2.5, axis=20, rate=0.3, phase=0.2)
    c.cyl(CREAM, (0, 0, 56), 17, 250, steps=7, top_r=11)
    for k in range(7):                                          # the mottle: bark coming off in plates
        a = k * 1.3
        c.box(TAN if k % 2 else PINE, (math.cos(a) * 14, math.sin(a) * 14, 86 + k * 30), (14, 14, 24), rot=(0, math.degrees(a), 0))
    for a, lift in ((30, 60), (150, 50), (265, 65)):
        d = math.radians(a)
        tilt_box(c, CREAM, (0, 0, 290), (math.cos(d) * 80, math.sin(d) * 60, 290 + lift), 12, 12)
    for k, (x, y, z, rad) in enumerate(((0, 0, 430, 120), (-110, 20, 380, 92), (110, -20, 372, 96), (20, 60, 356, 80), (-30, -70, 366, 84),
                                        (60, 10, 478, 74))):
        blob(c, (LEAF, PINE, LEAF, PINE, GREEN_D, LEAF)[k], (x, y, z), rad, (1.15, 1, 0.8), (4, 7))
    for k in range(5):                                          # seed balls on strings, swinging under the crown
        x, y = r.uniform(-120, 120), r.uniform(-60, 60)
        s = sway(sc, (x, y, 330), swing=10, axis=r.uniform(0, 180), rate=r.uniform(0.5, 0.8), phase=r.random())
        s.box(TAN, (x, y, 312), (4, 4, 36))
        blob(s, TAN, (x, y, 290), 9, steps=(3, 5))


def cherry(sc):
    """Theatre Row, Penthouse Row: a cherry in blossom in a white tub, petals drifting down."""
    r = random.Random(32)
    g = sc.still()
    g.cyl(WHITE, (0, 0, 0), 70, 66, steps=8, top_r=82)
    g.cyl(PLUM, (0, 0, 46), 80, 8, steps=8)
    g.cyl(BLUSH, (0, 0, 66), 76, 5, steps=8)
    c = sway(sc, (0, 0, 66), swing=3, axis=30, rate=0.32, phase=0.6)
    c.cyl(OXBLOOD, (0, 0, 60), 14, 130, steps=6, top_r=11)
    for a, reach, top in ((20, 120, 330), (140, 110, 310), (255, 100, 345), (320, 60, 380)):
        d = math.radians(a)
        tilt_box(c, OXBLOOD, (0, 0, 184), (math.cos(d) * reach, math.sin(d) * reach * 0.6, top - 50), 10, 10)
    for k, (x, y, z, rad) in enumerate(((0, 0, 370, 96), (-120, 10, 330, 80), (118, -16, 326, 82), (10, 66, 318, 70), (-20, -66, 320, 72),
                                        (64, 20, 410, 62), (-70, -10, 398, 58))):
        blob(c, (BLUSH, BLUSH, WHITE, BLUSH, BLUSH, WHITE, BLUSH)[k], (x, y, z), rad, (1.15, 1, 0.78), (4, 7))
    for k in range(6):
        a = k * 1.05
        blob(c, MAG, (math.cos(a) * 110, math.sin(a) * 66, 300 + (k % 3) * 44), 13, steps=(3, 5))
    for k in range(7):                                          # petals: flat flakes that float and turn under the crown
        x, y = r.uniform(-150, 150), r.uniform(-70, 70)
        z = r.uniform(110, 250)
        p = sc.part(P.motion(bob=-16, swing=30, pivot=(x, y, z), axis=r.uniform(0, 180), rate=r.uniform(0.25, 0.45), phase=r.random()))
        p.box(BLUSH, (x, y, z), (14, 10, 4))
    for k in range(6):
        g.box(BLUSH, (r.uniform(-140, 140), r.uniform(-50, 50), 1.5), (13, 10, 3), rot=(0, r.uniform(0, 90), 0))


def sumac_tall(sc):
    """The Garages, the circuit: a staghorn sumac and a buddleia grown up through a broken concrete planter."""
    r = random.Random(33)
    g = sc.still()
    g.box(STONE_L, (-60, 0, 26), (230, 110, 60))
    g.box(STONE_L, (120, 6, 20), (120, 104, 48), rot=(0, 6, 9))
    g.box(CONC, (-60, 0, 58), (214, 94, 5))
    g.box('#ffe28a', (-60, -56, 30), (200, 3, 12))             # a faded parking stripe on its side
    for k, (x, lean, h) in enumerate(((-110, -14, 300), (-30, 6, 350), (50, 18, 280))):   # sumac: forked stems, flat leaf fans, red candles
        s = sway(sc, (x, 0, 58), swing=3.5, axis=r.uniform(0, 180), rate=r.uniform(0.3, 0.45), phase=r.random())
        top = (x + lean * 3, 0, h)
        tilt_box(s, WOOD_D, (x, 0, 50), top, 11, 11)
        for j in range(5):
            z = h - 20 - j * 34
            s.box(LEAF if (j + k) % 2 else PINE, (top[0] * (1 - j * 0.12) + x * j * 0.12, 0, z), (150 - j * 12, 16, 5), rot=(0, j * 53 + k * 20, 12))
        for j in range(2):
            blob(s, OXBLOOD, (top[0] + (j * 2 - 1) * 14, 0, h + 14), 11, (1, 1, 2.0), (3, 6))
    for j in range(4):                                          # buddleia arching over the broken end
        a = math.radians(-60 + j * 40)
        tip = (150 + math.cos(a) * 110, math.sin(a) * 60, r.uniform(190, 260))
        s = sway(sc, (130, 0, 44), swing=5, axis=math.degrees(a) + 90, rate=r.uniform(0.4, 0.6), phase=r.random())
        tilt_box(s, PINE, (130, 0, 40), tip, 7, 7)
        s.box(LEAF, ((130 + tip[0]) / 2.0, tip[1] / 2.0, tip[2] * 0.6), (46, 10, 5), rot=(24, math.degrees(a), 0))
        blob(s, LILAC if j % 2 else VIOLET, (tip[0], tip[1], tip[2] + 6), 11, (1, 1, 2.6), (3, 6))


def ivy_trellis(sc):
    """The Garages, Wall Street: a sheet of ivy on its own trellis, in a trough; the loose ends flutter."""
    r = random.Random(34)
    g = sc.still()
    g.box(STONE_L, (0, 0, 24), (520, 80, 52))
    g.box(GREEN_D, (0, 0, 51), (500, 62, 4))
    for k in range(5):
        g.box(SILVER, (-240 + k * 120, 16, 190), (7, 7, 290))
    for k in range(4):
        g.box(SILVER, (0, 16, 110 + k * 72), (500, 5, 5))
    g.box(SILVER, (0, 16, 336), (500, 7, 7))
    cols = (PINE, GREEN_D, LEAF)
    for k in range(13):                                         # the sheet: overlapping slabs of leaf, thick enough to ink
        x = -210 + (k % 5) * 105 + r.uniform(-20, 20)
        z = 96 + (k // 5) * 92 + r.uniform(-14, 14)
        g.box(cols[k % 3], (x, 8 - (k % 2) * 5, z), (r.uniform(110, 160), 7, r.uniform(84, 118)))
    g.box(PINE, (0, 6, 318), (430, 8, 40))
    for k in range(12):                                         # loose ends: off the top and the sides, each on its own beat
        if k < 8:
            x, z, L = -220 + k * 63, 342, r.uniform(34, 70)
            t = sway(sc, (x, 10, z), swing=9, axis=0, rate=r.uniform(0.4, 0.7), phase=r.random())
            t.box(cols[k % 3], (x, 0, z - L / 2.0 + 14), (22, 6, L))
            t.box(LEAF, (x + 10, -3, z - L + 14), (14, 5, 14))
        else:
            s = -1 if k % 2 else 1
            z = 150 + (k - 8) * 46
            t = sway(sc, (s * 250, 10, z), swing=7, axis=90, rate=r.uniform(0.4, 0.6), phase=r.random())
            t.box(cols[k % 3], (s * 276, 6, z - 16), (52, 6, 30))


def tomato_roof(sc):
    """Fifth and Main: a roof tomato garden: a plank bed, cane wigwams heavy with fruit, a striped windbreak."""
    r = random.Random(35)
    g = sc.still()
    g.box(WOOD, (0, 0, 22), (460, 96, 44))
    for x in (-232, 232):
        g.box(WOOD_D, (x, 0, 24), (8, 104, 52))
    g.box(GREEN_D, (0, 0, 45), (444, 82, 4))
    for k, x in enumerate((-160, 0, 160)):
        w = sway(sc, (x, 0, 46), swing=2.5, axis=r.uniform(0, 180), rate=0.4, phase=r.random())
        for j in range(3):                                      # the wigwam
            a = math.radians(j * 120 + k * 30)
            tilt_box(w, TAN, (x + math.cos(a) * 44, math.sin(a) * 30, 46), (x, 0, 226), 5, 5)
        w.box(CREAM, (x, 0, 226), (10, 10, 8))
        for j in range(5):                                      # the vine, fruit hung down it
            z = 70 + j * 32
            blob(w, PINE if j % 2 else LEAF, (x + (j % 2) * 12 - 6, 0, z), 30 - j * 3, (1.1, 0.9, 0.8), (3, 6))
            a = j * 2.2 + k
            blob(w, RED if j < 4 else AMBER, (x + math.cos(a) * 26, math.sin(a) * 20 - 10, z - 6), 8, steps=(3, 5))
    for s in (-1, 1):                                           # the windbreak: a striped cloth between two poles, bellying
        g.box(WOOD_D, (s * 80, 44, 120), (6, 6, 240))
    c = sway(sc, (0, 44, 236), swing=6, axis=0, rate=0.5, phase=0.3)
    for j in range(5):
        c.box(RED if j % 2 == 0 else CREAM, (-64 + j * 32, 44, 186), (32, 4, 96))
    g.cyl(TEAL, (212, -20, 46), 12, 24, steps=6)


def cypress_urns(sc):
    """Wall Street: two columnar cypresses in sandstone urns on plinths, a copper-green chain between them."""
    r = random.Random(36)
    g = sc.still()
    for s in (-1, 1):
        x = s * 150
        g.box(SAND, (x, 0, 22), (96, 96, 44))
        g.box(CREAM, (x, 0, 47), (108, 108, 6))
        g.cyl(SAND, (x, 0, 50), 22, 16, steps=8, top_r=40)
        g.cyl(SAND, (x, 0, 66), 40, 30, steps=8, top_r=46)
        g.cyl(CREAM, (x, 0, 96), 49, 7, steps=8)
        c = sway(sc, (x, 0, 100), swing=2, axis=r.uniform(0, 180), rate=0.3, phase=r.random())
        c.cyl(WOOD_D, (x, 0, 98), 8, 26, steps=5)
        blob(c, GREEN_D, (x, 0, 270), 38, (1, 1, 4.1), (6, 8))        # the flame: a long ellipsoid, not a cone
        blob(c, PINE, (x - 10, -12, 250), 26, (1, 1, 3.6), (4, 6))
        blob(c, PINE, (x + 6, 10, 330), 20, (1, 1, 3.4), (4, 6))
    for k in range(5):                                          # the chain, swinging
        x = -100 + k * 50
        ch = sway(sc, (x, 0, 62), swing=8, axis=0, rate=0.6, phase=0.15 * k)
        ch.box(PINE, (x, 0, 62 - 8 - 14 * math.sin(math.pi * k / 4.0)), (40, 6, 6))


def palm_tub(sc):
    """Penthouse Row: a palm in a white tub with a blush band, fronds lifting and falling."""
    r = random.Random(37)
    g = sc.still()
    g.cyl(WHITE, (0, 0, 0), 62, 70, steps=8, top_r=76)
    g.cyl(BLUSH, (0, 0, 50), 75, 8, steps=8)
    g.cyl(SILVER, (0, 0, 70), 70, 5, steps=8)
    t = sway(sc, (0, 0, 70), swing=2.5, axis=40, rate=0.3, phase=0.4)
    pts = [(0, 0, 66), (10, 0, 160), (26, 0, 250), (36, 0, 340), (40, 0, 420)]
    for k in range(4):
        tilt_box(t, TAN if k % 2 else WOOD, pts[k], pts[k + 1], 26 - k * 3, 26 - k * 3)
        t.cyl(WOOD_D, (pts[k + 1][0], 0, pts[k + 1][2] - 5), 16 - k * 1.5, 7, steps=6)
    blob(t, WOOD, (40, 0, 426), 24, steps=(3, 6))
    for k in range(9):                                          # fronds: each two lengths, each rising and falling at its root
        a = math.radians(k * 40 + 10)
        root = (40, 0, 430)
        mid = (40 + math.cos(a) * 100, math.sin(a) * 100, 470 + (k % 3) * 14)
        tip = (40 + math.cos(a) * 185, math.sin(a) * 185, 400 + (k % 2) * 30)
        f = sc.part(P.motion(swing=5, pivot=root, axis=math.degrees(a) + 90, rate=r.uniform(0.3, 0.5), phase=r.random()))
        tilt_box(f, LEAF if k % 2 else PINE, root, mid, 34, 6)
        tilt_box(f, PINE if k % 2 else LEAF, mid, tip, 26, 6)


def bamboo_screen(sc):
    """Penthouse Row: a screen of bamboo in a long white trough."""
    r = random.Random(38)
    g = sc.still()
    g.box(WHITE, (0, 0, 28), (520, 76, 56))
    g.box(SILVER, (0, 0, 2), (500, 84, 6))
    g.box(BLUSH, (0, -39, 40), (520, 3, 8))
    g.box(GREEN_D, (0, 0, 55), (500, 60, 4))
    for grp in range(5):                                        # five clumps, each leaning on its own beat
        gx = -200 + grp * 100
        c = sway(sc, (gx, 0, 56), swing=3.5, axis=r.uniform(-20, 20), rate=r.uniform(0.3, 0.5), phase=r.random())
        for j in range(3):
            x = gx + (j - 1) * 30 + r.uniform(-6, 6)
            y = r.uniform(-18, 18)
            h = r.uniform(250, 330)
            c.cyl(LIME if (grp + j) % 3 == 0 else LEAF, (x, y, 54), 6, h, steps=5)
            c.box(PINE, (x, y, 54 + h * 0.5), (14, 14, 5))
            for q in range(3):
                c.box(LEAF if q % 2 else PINE, (x + (q - 1) * 20, y, 54 + h - 20 - q * 34), (54, 6, 14), rot=(r.uniform(-30, 30), r.uniform(0, 180), 0))


def yew_obelisk(sc):
    """The Syndicate's quarter: two yew obelisks and a ball between them, in gold-banded black tubs. It hardly stirs."""
    r = random.Random(39)
    g = sc.still()
    g.box(IRON, (0, 0, 6), (520, 110, 12))
    for k, x in enumerate((-180, 0, 180)):
        g.cyl(BLACK, (x, 0, 10), 44, 56, steps=8, top_r=52)
        g.cyl(GOLD, (x, 0, 48), 52, 8, steps=8)
        b = sway(sc, (x, 0, 66), swing=1.5, axis=r.uniform(0, 180), rate=0.22, phase=r.random())
        b.cyl(WOOD_D, (x, 0, 62), 8, 16, steps=5)
        if k == 1:
            blob(b, GREEN_D, (x, 0, 122), 50, steps=(5, 8))
        else:
            b.cyl(GREEN_D, (x, 0, 74), 48, 230, steps=4, top_r=15, rot=(0, 45, 0))     # four-sided, cut flat at the top
            b.cyl(PINE, (x, 0, 304), 15, 8, steps=4, rot=(0, 45, 0))
            blob(b, GREEN_D, (x, 0, 326), 17, steps=(4, 6))


def lime_row(sc):
    """The Syndicate's quarter: three pollarded limes in a strict row, knuckled heads, this year's shoots."""
    r = random.Random(40)
    g = sc.still()
    g.box(OXBLOOD, (0, 0, 22), (540, 90, 44))
    g.box(IRON, (0, 0, 46), (556, 104, 6))
    for k, x in enumerate((-190, 0, 190)):
        b = sway(sc, (x, 0, 48), swing=1.5, axis=r.uniform(0, 180), rate=0.25, phase=r.random())
        b.cyl(TAN, (x, 0, 44), 13, 190, steps=6, top_r=11)
        blob(b, TAN, (x, 0, 244), 30, (1.2, 1.1, 0.8), (4, 6))
        for j in range(6):
            a = math.radians(j * 60 + k * 20)
            tip = (x + math.cos(a) * 52, math.sin(a) * 40, 318 + (j % 2) * 20)
            tilt_box(b, WOOD_D, (x + math.cos(a) * 20, math.sin(a) * 16, 252), tip, 6, 6)
            blob(b, PINE if j % 2 else GREEN_D, tip, 22, (1, 1, 1.2), (3, 6))
    for z in (120, 200):                                        # the training wires that keep the row strict
        g.box(SILVER, (0, 0, z), (400, 4, 4))


def birch_pair(sc):
    """The circuit: two silver birches in a riveted steel planter, the thin twigs hanging and swinging."""
    r = random.Random(41)
    g = sc.still()
    g.box(STONE_L, (0, 0, 26), (300, 110, 52))
    g.box(SILVER, (0, 0, 52), (312, 122, 6))
    g.box(VIOLET, (0, -56, 30), (300, 3, 10))
    for k in range(6):
        g.sphere(SILVER, (-125 + k * 50, -56, 44), 5, steps=(3, 5))
    for k, (x, lean, h) in enumerate(((-70, -10, 470), (70, 16, 420))):
        c = sway(sc, (x, 0, 54), swing=2.5, axis=r.uniform(0, 180), rate=0.3, phase=r.random())
        top = (x + lean * 3, 0, h)
        tilt_box(c, WHITE, (x, 0, 50), top, 15, 15)
        for j in range(5):                                      # the bark's black dashes
            f = 0.15 + j * 0.16
            c.box(BLACK, (x + (top[0] - x) * f, -8, 50 + (h - 50) * f), (17, 5, 7))
        for j in range(5):
            a = math.radians(j * 72 + k * 36)
            cx, cy, cz = top[0] + math.cos(a) * 70, math.sin(a) * 50, h - 40 - (j % 3) * 46
            tilt_box(c, WHITE, (top[0], 0, cz - 30), (cx, cy, cz), 6, 6)
            blob(c, LIME if j % 2 else LEAF, (cx, cy, cz + 8), 44, (1.2, 1, 0.7), (3, 6))
        blob(c, LEAF, (top[0], 0, h + 24), 40, (1, 1, 0.9), (3, 6))
        for j in range(5):                                      # hanging twigs with a catkin, each swinging from its top
            a = math.radians(j * 72 + 20)
            tx, ty, tz = top[0] + math.cos(a) * 84, math.sin(a) * 58, h - 70 - (j % 2) * 40
            t = sway(sc, (tx, ty, tz), swing=11, axis=r.uniform(0, 180), rate=r.uniform(0.45, 0.7), phase=r.random())
            t.box(LEAF, (tx, ty, tz - 42), (6, 6, 84))
            t.box(WARM, (tx, ty, tz - 90), (7, 7, 20))


# ================================================================================================ OPEN: the Plaza's two trees
def willow(sc):
    """The Plaza's own: a weeping willow on a ring of pale stone, every frond swinging from the crown's rim."""
    r = random.Random(51)
    g = sc.still()
    g.cyl(STONE_L, (0, 0, -150), 150, 180, steps=10)
    g.cyl(TEAL, (0, 0, 30), 156, 8, steps=10)
    g.cyl(GREEN_D, (0, 0, 38), 128, 4, steps=10)
    c = sway(sc, (0, 0, 40), swing=1.5, axis=30, rate=0.25, phase=0.1)
    tilt_box(c, WOOD_D, (0, 0, 30), (26, 0, 250), 46, 40)
    for a, reach in ((15, 170), (130, 160), (250, 170)):
        d = math.radians(a)
        tilt_box(c, WOOD_D, (26, 0, 240), (26 + math.cos(d) * reach, math.sin(d) * reach, 440), 24, 22)
    blob(c, LEAF, (26, 0, 520), 270, (1, 0.9, 0.52), (5, 10))
    blob(c, PINE, (26, 0, 596), 170, (1, 0.9, 0.5), (4, 8))
    for k in range(16):                                         # the curtain: fronds hung round the rim
        a = math.radians(k * 22.5 + 6)
        rx = 26 + math.cos(a) * 250
        ry = math.sin(a) * 222
        L = r.uniform(250, 360)
        f = sc.part(P.motion(swing=7, pivot=(rx, ry, 500), axis=math.degrees(a), rate=r.uniform(0.25, 0.4), phase=r.random()))
        f.box(LEAF if k % 2 else LIME, (rx, ry, 500 - L / 2.0), (52, 9, L), rot=(0, math.degrees(a) + 90, 0))
        f.box(PINE, (rx, ry, 500 - L * 0.42), (30, 12, L * 0.6), rot=(0, math.degrees(a) + 90, 0))


def magnolia(sc):
    """The Plaza: a magnolia in a teal-edged round bed, white cups held up on bare dark branches."""
    r = random.Random(52)
    g = sc.still()
    g.cyl(STONE_L, (0, 0, -150), 120, 176, steps=10)
    g.cyl(TEAL, (0, 0, 26), 126, 8, steps=10)
    g.cyl(GREEN_D, (0, 0, 34), 104, 4, steps=10)
    c = sway(sc, (0, 0, 36), swing=2, axis=60, rate=0.28, phase=0.5)
    c.cyl(OXBLOOD, (0, 0, 30), 20, 120, steps=6, top_r=16)
    tips = []
    for k in range(6):
        a = math.radians(k * 60 + 10)
        reach = 150 + (k % 2) * 40
        tip = (math.cos(a) * reach, math.sin(a) * reach * 0.8, 330 + (k % 3) * 50)
        tilt_box(c, OXBLOOD, (0, 0, 140), tip, 12, 12)
        tips.append(tip)
        mid = (tip[0] * 0.55, tip[1] * 0.55, 140 + (tip[2] - 140) * 0.6)
        tip2 = (mid[0] * 0.9, mid[1] * 0.9 + 30, mid[2] + 130)
        tilt_box(c, OXBLOOD, mid, tip2, 8, 8)
        tips.append(tip2)
    tips.append((0, 0, 470))
    tilt_box(c, OXBLOOD, (0, 0, 140), (0, 0, 470), 10, 10)
    for k, tip in enumerate(tips):                              # a cup on every tip, a leaf pad under it
        blob(c, PINE if k % 2 else GREEN_D, (tip[0], tip[1], tip[2] - 6), 34, (1.2, 1, 0.5), (3, 6))
        c.cyl(WHITE, (tip[0], tip[1], tip[2] + 4), 12, 30, steps=6, top_r=24)
        c.cyl(BLUSH, (tip[0], tip[1], tip[2]), 13, 8, steps=6)
    for k in range(5):                                          # fallen cups on the bed, and two petals coming down
        a = k * 1.3
        g.cyl(WHITE, (math.cos(a) * 70, math.sin(a) * 60, 38), 9, 5, steps=5)
    for k in range(3):
        x, y, z = r.uniform(-140, 140), r.uniform(-90, 90), r.uniform(120, 260)
        p = sc.part(P.motion(bob=-18, swing=30, pivot=(x, y, z), axis=r.uniform(0, 180), rate=r.uniform(0.25, 0.4), phase=r.random()))
        p.box(WHITE, (x, y, z), (16, 11, 4))


JOBS = [
    ('SM_S1PLANTS_GeraniumRun', geranium_run, LOW), ('SM_S1PLANTS_CamelliaTubs', camellia_tubs, LOW),
    ('SM_S1PLANTS_SumacLow', sumac_low, LOW), ('SM_S1PLANTS_TulipBed', tulip_bed, LOW), ('SM_S1PLANTS_BoxHedge', box_hedge, LOW),
    ('SM_S1PLANTS_RoseBed', rose_bed, LOW), ('SM_S1PLANTS_ThistlePampas', thistle_pampas, LOW),
    ('SM_S1PLANTS_HangTrough', hang_trough, LOW), ('SM_S1PLANTS_LaurelUrns', laurel_urns, LOW), ('SM_S1PLANTS_Bramble', bramble, LOW),
    ('SM_S1PLANTS_YewDrums', yew_drums, LOW),
    ('SM_S1PLANTS_PlaneBox', plane_box, TALL), ('SM_S1PLANTS_Cherry', cherry, TALL), ('SM_S1PLANTS_SumacTall', sumac_tall, TALL),
    ('SM_S1PLANTS_IvyTrellis', ivy_trellis, TALL), ('SM_S1PLANTS_TomatoRoof', tomato_roof, TALL),
    ('SM_S1PLANTS_CypressUrns', cypress_urns, TALL), ('SM_S1PLANTS_PalmTub', palm_tub, TALL),
    ('SM_S1PLANTS_BambooScreen', bamboo_screen, TALL), ('SM_S1PLANTS_YewObelisk', yew_obelisk, TALL),
    ('SM_S1PLANTS_LimeRow', lime_row, TALL), ('SM_S1PLANTS_BirchPair', birch_pair, TALL),
    ('SM_S1PLANTS_Willow', willow, TALL), ('SM_S1PLANTS_Magnolia', magnolia, TALL),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
