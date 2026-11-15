"""FULL THROTTLE (Unreal) dressing kit, level 0: Bayshore. The resort strip in blocks: a boardwalk and beach life on
the bay side, kiosks and street furniture by the road, shop rows, motels, pools, car parks and service yards behind,
more hotels (ft_city's own, with real glass and neon), a marina afloat on the bay, and things that stand on the skyline.
Placed by Source/FullThrottle/FTDressL0.inl. See ft_dress.py for how a piece is drawn (front faces -Y, base on z = 0).

    import ft_dress; ft_dress.run(['bayshore'])
"""
import math
import random
import ft_city
from ft_desert import tilt_box
from ft_city import PINK, HOTPINK, MINT, LAV, PEACH, CREAM, CORAL, TEAL, SKY, WHITE, YEL, NAVY, SAND, RED, LEAF, LEAF_D, TRUNK_D

WOOD, WOOD_D = '#c9a06a', '#9a744a'
GREY, GREY_D = '#b8b4c8', '#8a86a0'
POOL = '#49e2d5'
GLOW = 0.8


# ------------------------------------------------------------------------------------------ the bay side, near
def boardwalk(m):
    """44 m of plank walk on posts, railed on the sea side (+Y is away from the road)."""
    L, W = 4400, 420
    m.box(WOOD, (0, 0, 40), (L, W, 16))
    for k in range(11):
        m.box(WOOD_D, (-L / 2 + 200 + k * 400, 0, 49), (14, W, 3))
    for k in range(8):
        x = -L / 2 + 300 + k * 543
        for y in (-W / 2 + 30, W / 2 - 30):
            m.box(WOOD_D, (x, y, -130), (26, 26, 330))
        m.box(WHITE, (x, W / 2 - 14, 100), (14, 14, 110))
    m.box(WHITE, (0, W / 2 - 14, 152), (L, 12, 12))
    m.box(WHITE, (0, W / 2 - 14, 104), (L, 8, 8))
    for x in (-1500, 1500):   # a bench and a lamp globe now and then
        m.box(TEAL, (x, 110, 82), (220, 50, 10))
        m.box(TEAL, (x, 134, 112), (220, 10, 60))
        m.cyl(WHITE, (x + 300, W / 2 - 30, 48), 6, 250, steps=6)
        m.sphere(YEL, (x + 300, W / 2 - 30, 318), 26, steps=(5, 8), glow=0.6)


def beach_path(m):
    """A plank path from the walk down the beach to the water, under an arch, with a shower."""
    for k in range(10):
        m.box(WOOD if k % 2 else WOOD_D, (0, 320 + k * 330, 10 - k * 22), (260, 300, 12))
    for s in (-1, 1):
        m.box(WHITE, (s * 160, 150, 150), (20, 20, 320))
    m.box(TEAL, (0, 150, 330), (380, 26, 80))
    m.box(YEL, (0, 134, 330), (310, 6, 44), glow=0.5)
    m.cyl(SKY, (240, 470, -20), 7, 250, steps=6)
    m.box(SKY, (240, 440, 226), (10, 70, 10))
    for k in range(3):     # surfboards stuck in the sand beside it
        m.box([CORAL, YEL, MINT][k], (-260 - k * 70, 420, 110), (50, 12, 260), rot=(0, 0, 8 - k * 7))


def cabanas(m):
    """A row of five striped beach huts with pitched roofs."""
    cols = [PINK, MINT, YEL, SKY, CORAL]
    for k in range(5):
        x = -720 + k * 360
        m.box(cols[k], (x, 0, 110), (300, 260, 220))
        for j in range(3):
            m.box(WHITE, (x - 100 + j * 100, -131, 110), (34, 4, 220))
        m.side(WHITE, [(x - 170, 220), (x + 170, 220), (x, 330)], -150, 150)
        m.box(NAVY, (x, -133, 86), (90, 6, 170))
        m.box(WOOD_D, (x, 0, -60), (280, 240, 120))


def sunbeds(m):
    """Sunbeds under two parasols."""
    for k, (x, y) in enumerate(((-260, 0), (0, 120), (260, -40), (-120, -230), (180, -260))):
        m.box(WHITE, (x, y, 26), (70, 190, 10))
        m.box([CORAL, TEAL, YEL, PINK, MINT][k], (x, y + 60, 44), (64, 60, 10), rot=(0, 0, 28))
        for s in (-1, 1):
            m.box(WOOD_D, (x, y + s * 70, 8), (60, 8, 30))
    for x, y, c in ((-130, 20, HOTPINK), (200, -150, TEAL)):
        m.cyl(WHITE, (x, y, -40), 5, 290, steps=6)
        m.cyl(c, (x, y, 236), 150, 46, steps=10, top_r=8)
        m.cyl(WHITE, (x, y, 232), 152, 6, steps=10)


def surf_rack(m):
    """A rack of surfboards standing on end and a hire sign."""
    m.box(WOOD_D, (0, 0, 120), (620, 14, 14))
    m.box(WOOD_D, (0, 0, 30), (620, 14, 14))
    for s in (-1, 1):
        m.box(WOOD_D, (s * 310, 0, 70), (16, 16, 300))
    cols = [CORAL, YEL, MINT, SKY, HOTPINK, WHITE, TEAL]
    for k in range(7):
        x = -270 + k * 90
        m.box(cols[k], (x, -22, 150), (60, 12, 300), rot=(0, 0, -6))
        m.box(WHITE, (x, -29, 150), (10, 4, 280), rot=(0, 0, -6))
    m.box(NAVY, (0, 0, 330), (420, 16, 84))
    m.box(YEL, (0, -10, 330), (360, 4, 44), glow=0.5)


def volleyball(m):
    m.box(WHITE, (0, 0, 2), (900, 1800, 4))
    m.box(SAND, (0, 0, 4), (860, 1760, 4))
    for s in (-1, 1):
        m.cyl(WOOD_D, (s * 470, 0, -40), 9, 300, steps=6)
    m.box(WHITE, (0, 0, 205), (940, 4, 90))
    m.box(NAVY, (0, 0, 250), (940, 6, 8))
    m.sphere(YEL, (160, 260, 330), 22, steps=(5, 8))
    m.box(CORAL, (-520, -200, 150), (60, 60, 300))      # the referee's chair
    m.box(WHITE, (-520, -200, 310), (90, 90, 10))


def breakwater(m, seed=4):
    """A run of big pale rocks for the point."""
    r = random.Random(seed)
    for k in range(11):
        x = -1500 + k * 300 + r.uniform(-60, 60)
        s = r.uniform(120, 240)
        m.sphere(GREY if k % 3 else GREY_D, (x, r.uniform(-140, 140), s * 0.2), s, scale=(1.2, 1.0, r.uniform(0.6, 0.9)), steps=(4, 6),
                 rot=(r.uniform(-15, 15), r.uniform(0, 180), 0))
    m.cyl(RED, (1500, 0, 0), 22, 420, steps=8)           # a marker post at its end
    m.sphere(YEL, (1500, 0, 440), 34, steps=(5, 8), glow=0.8)


# ------------------------------------------------------------------------------------------ the town side, near
def kiosk(m, col=PINK, trim=MINT, what='cone'):
    """A drinks or ice-cream kiosk: a striped awning over the hatch, and its ware, huge, on the roof."""
    m.box(col, (0, 0, 135), (320, 280, 270))
    m.box(NAVY, (0, -141, 160), (230, 6, 120))
    m.box(WHITE, (0, -165, 96), (270, 50, 10))
    for k in range(6):
        m.box(trim if k % 2 else WHITE, (-125 + k * 50, -200, 250), (50, 130, 12), rot=(0, 0, -16))
    m.box(trim, (0, 0, 280), (350, 310, 22))
    if what == 'cone':
        m.cyl('#e8b26a', (0, 0, 292), 10, 190, steps=10, top_r=78)
        m.sphere(PINK, (0, 0, 520), 92, steps=(6, 10), glow=0.25)
        m.sphere(CREAM, (30, -20, 620), 70, steps=(6, 10), glow=0.25)
        m.sphere(RED, (40, -30, 700), 22, steps=(4, 6), glow=0.6)
    else:
        m.cyl(WHITE, (0, 0, 292), 70, 220, steps=12)      # a paper cup, a straw, a slice of fruit on the rim
        m.cyl(CORAL, (0, 0, 420), 72, 60, steps=12)
        m.cyl(YEL, (30, 0, 500), 8, 170, steps=6, rot=(0, 0, 14))
        m.cyl(YEL, (-66, 0, 500), 56, 14, steps=12, rot=(90, 0, 0))
    m.box(HOTPINK, (0, -158, 300), (300, 8, 10), glow=GLOW)


def bus_stop(m):
    m.box(NAVY, (0, 60, 130), (420, 10, 250))
    m.box(SKY, (0, 54, 150), (380, 4, 180), glow=0.15)
    m.box(WHITE, (0, 0, 262), (460, 170, 12))
    for s in (-1, 1):
        m.box(WHITE, (s * 215, -70, 130), (10, 10, 262))
        m.box(WHITE, (s * 215, 60, 130), (10, 10, 262))
    m.box(TEAL, (0, 30, 50), (300, 44, 10))
    m.box(YEL, (270, -60, 250), (70, 8, 90), glow=0.5)   # the route sign
    m.cyl(WHITE, (270, -60, 0), 5, 210, steps=6)


def planters(m, seed=2):
    """A low white wall with a clipped hedge, planters with flowers, a bench: the town's edge along the road."""
    r = random.Random(seed)
    m.box(WHITE, (0, 40, 40), (2200, 50, 80))
    m.box(LEAF_D, (0, 40, 112), (2140, 66, 70))
    for k in range(5):
        x = -900 + k * 450
        m.box(CREAM, (x, -70, 30), (120, 120, 60))
        m.sphere(LEAF, (x, -70, 92), 62, scale=(1, 1, 0.8), steps=(5, 8))
        for j in range(4):
            m.sphere([HOTPINK, YEL, CORAL, WHITE][j], (x + r.uniform(-40, 40), -70 + r.uniform(-40, 40), 120 + r.uniform(0, 30)), 14, steps=(3, 5))
    m.box(TEAL, (600, -180, 44), (200, 50, 10))
    m.box(TEAL, (600, -156, 80), (200, 10, 60))


def phone_booths(m):
    for k in range(2):
        x = -70 + k * 140
        m.box(CORAL if k else TEAL, (x, 0, 125), (110, 110, 250))
        m.box(SKY, (x, -56, 140), (80, 4, 160), glow=0.2)
        m.box(WHITE, (x, 0, 258), (126, 126, 16))
    m.box(NAVY, (160, 20, 60), (60, 60, 120))            # a post box and a litter bin
    m.cyl(GREY, (-180, 10, 0), 28, 90, steps=8)


def flags(m):
    """A row of five flagpoles (the flags are still: the living-world pass may set them fluttering)."""
    cols = [HOTPINK, YEL, TEAL, CORAL, LAV]
    for k in range(5):
        x = -600 + k * 300
        m.cyl(WHITE, (x, 0, -40), 7, 760, steps=6)
        m.sphere(YEL, (x, 0, 730), 14, steps=(4, 6))
        m.box(cols[k], (x + 90, 0, 640), (170, 6, 110))
        m.box(WHITE, (x + 90, -1, 640), (170, 6, 24))


# ------------------------------------------------------------------------------------------ the town side, middle
def shops(m, seed=1):
    """A row of four two-storey shopfronts: lit windows, awnings, sign boards with a neon line, clutter on the roof."""
    r = random.Random(seed)
    cols = [PEACH, MINT, PINK, CREAM]
    trim = [TEAL, CORAL, WHITE, HOTPINK]
    for k in range(4):
        x = -1350 + k * 900
        h = 620 + r.choice((0, 80, 140))
        m.box(cols[k], (x, 150, h / 2.0), (880, 900, h))
        m.box(NAVY, (x, -304, 150), (700, 8, 240))
        m.box(YEL if k % 2 else SKY, (x, -308, 150), (640, 4, 190), glow=0.45)          # the shop window, lit
        m.box(NAVY, (x - 250, -310, 120), (110, 6, 220))                                # its door
        for j in range(7):
            m.box(trim[k] if j % 2 else WHITE, (x - 360 + j * 120, -372, 300), (120, 150, 12), rot=(0, 0, -14))
        m.box(NAVY, (x, -306, 392), (760, 10, 96))                                      # the sign board
        m.box(trim[k], (x, -313, 392), (r.uniform(380, 640), 4, 44), glow=0.6)
        for j in range(3):                                                              # upstairs windows
            m.box(NAVY, (x - 270 + j * 270, -302, 520), (150, 6, 120))
            if r.random() < 0.6:
                m.box(YEL, (x - 270 + j * 270, -305, 520), (130, 4, 100), glow=0.35)
        m.box(WHITE, (x, 150, h + 12), (900, 920, 24))
        m.box(GREY, (x + r.uniform(-250, 250), 250, h + 60), (160, 120, 90))            # an AC unit
        if k == 1:
            m.cyl(GREY_D, (x + 200, 380, h + 24), 60, 150, steps=8)                    # a water tank
    m.box(HOTPINK, (0, -300, 615), (3560, 8, 8), glow=GLOW)


def motel(m):
    """A two-storey motel: a long block of doors along a gallery, an office at one end under a tall arrow sign."""
    L, D, H = 4200, 900, 600
    m.box(CREAM, (0, 0, H / 2.0), (L, D, H))
    m.box(TEAL, (0, 0, H + 14), (L + 80, D + 120, 28))
    m.box(WHITE, (0, -D / 2 - 90, 300), (L, 180, 14))                                   # the gallery
    m.box(TEAL, (0, -D / 2 - 176, 350), (L, 8, 90))
    for k in range(10):
        x = -L / 2 + 250 + k * 410
        for z in (0, 300):
            m.box([CORAL, TEAL, YEL][k % 3], (x, -D / 2 - 4, z + 115), (100, 8, 210))    # a door, a window by it
            m.box(NAVY, (x + 150, -D / 2 - 4, z + 150), (130, 8, 110))
            if (k + z // 300) % 3:
                m.box(YEL, (x + 150, -D / 2 - 8, z + 150), (112, 4, 92), glow=0.35)
        m.box(WHITE, (x - 110, -D / 2 - 170, 150), (14, 14, 300))
    m.box(PINK, (L / 2 + 320, -40, 220), (560, 700, 440))                               # the office
    m.box(SKY, (L / 2 + 320, -394, 190), (420, 6, 240), glow=0.3)
    m.box(TEAL, (L / 2 + 320, -40, 452), (620, 760, 24))
    m.cyl(WHITE, (L / 2 + 760, -420, 0), 14, 1250, steps=8)                             # the sign: MOTEL down a mast, an arrow
    m.box(NAVY, (L / 2 + 760, -420, 980), (300, 30, 620))
    for k in range(5):
        m.box(HOTPINK if k % 2 else YEL, (L / 2 + 760, -438, 1230 - k * 120), (200, 6, 84), glow=GLOW)
    m.side(YEL, [(L / 2 + 560, 640), (L / 2 + 960, 640), (L / 2 + 760, 520)], -440, -430, glow=GLOW)


def pool(m):
    """A motel pool: seen best from above, where loops and jumps look down on the town."""
    m.box(CREAM, (0, 0, 6), (1700, 1200, 12))
    m.box(POOL, (0, 0, 10), (1300, 760, 12), glow=0.2)
    m.box(WHITE, (0, 0, 12), (1340, 800, 6))
    m.box(POOL, (0, 0, 15), (1280, 740, 6), glow=0.2)
    for k in range(4):
        m.box(WHITE, (-480 + k * 320, 0, 19), (10, 740, 2))
    m.box(SKY, (520, -470, 60), (60, 200, 120))                                        # a slide
    m.box(WHITE, (520, -400, 130), (70, 260, 12), rot=(0, 0, 24))
    for x, y, c in ((-700, -480, HOTPINK), (-250, 500, TEAL), (700, 480, YEL)):
        m.cyl(WHITE, (x, y, 0), 5, 260, steps=6)
        m.cyl(c, (x, y, 210), 140, 44, steps=10, top_r=8)
        m.box(WHITE, (x + 120, y, 26), (70, 180, 10))
    m.box(LEAF_D, (0, 590, 50), (1700, 40, 100))                                       # a hedge behind


def car_park(m, seed=6):
    """A car park: painted bays, a few parked cars (well back from the road), two lamp masts."""
    r = random.Random(seed)
    m.box('#6f6a88', (0, 0, 4), (3000, 1700, 8))
    for k in range(11):
        for y in (-520, 520):
            m.box(WHITE, (-1350 + k * 270, y, 9), (10, 520, 2))
    cols = [CORAL, MINT, YEL, WHITE, SKY, PINK, LAV, TEAL]
    for k in range(10):
        if r.random() < 0.6:
            x, y = -1215 + k * 270, r.choice((-520, 520))
            c = r.choice(cols)
            m.box(c, (x, y, 52), (190, 440, 70))
            m.box(NAVY, (x, y + 20, 104), (170, 250, 50))
            m.box(c, (x, y + 20, 132), (160, 230, 10))
    for x in (-800, 800):
        m.cyl(GREY, (x, 0, 0), 9, 620, steps=6)
        m.box(GREY, (x, 0, 620), (220, 30, 16))
        for s in (-1, 1):
            m.box(YEL, (x + s * 90, 0, 608), (60, 26, 8), glow=0.7)


def service_yard(m, seed=3):
    """What is behind a hotel: a loading dock, dumpsters, crates, a fenced generator, vents, a stack of deck chairs."""
    r = random.Random(seed)
    m.box(GREY, (0, 300, 60), (900, 500, 120))
    m.box(GREY_D, (0, 300, 300), (900, 500, 360))
    m.box(NAVY, (-150, 46, 250), (300, 8, 240))
    for j in range(5):
        m.box(GREY, (-150, 42, 150 + j * 48), (290, 4, 6))
    m.box(YEL, (260, 44, 400), (120, 6, 40), glow=0.5)
    for k in range(3):
        x = -700 + k * 190
        m.box([TEAL, CORAL, MINT][k], (x, -120, 70), (170, 110, 140))
        m.box(NAVY, (x, -120, 146), (176, 116, 12))
    for k in range(6):
        m.box(WOOD if k % 2 else WOOD_D, (520 + (k % 2) * 110, -150 + (k // 2) * 20, 50 + (k // 2) * 100), (100, 100, 100),
              rot=(0, r.uniform(-12, 12), 0))
    m.box(GREY_D, (820, 260, 110), (260, 200, 220))
    for x in (700, 940):
        for y in (150, 370):
            m.box(GREY, (x, y, 130), (8, 8, 260))
    m.box(GREY, (820, 150, 250), (250, 6, 6))
    m.cyl(GREY, (-380, 480, 480), 40, 260, steps=8)
    m.cyl(GREY_D, (-380, 480, 740), 56, 30, steps=8)
    for k in range(6):
        m.box(WHITE, (-820, 220, 12 + k * 14), (70, 190, 10))


def villa(m, col=WHITE, roof=CORAL):
    """A low white villa with a tiled roof, a terrace and a garden wall."""
    m.box(col, (0, 100, 170), (1100, 700, 340))
    m.side(roof, [(-600, 340), (600, 340), (350, 520), (-350, 520)], -260, 460)
    m.box(col, (-380, -330, 120), (520, 420, 240))
    m.box(roof, (-380, -330, 250), (560, 460, 20))
    for x in (-200, 150, 420):
        m.box(NAVY, (x, -252, 190), (150, 6, 170))
        m.box(YEL, (x, -256, 190), (130, 4, 150), glow=0.3)
    m.box(col, (0, -620, 50), (1500, 30, 100))
    m.box(LEAF_D, (0, -590, 110), (1440, 60, 60))
    ft_city.palm(m, h=620, lean=8, seed=5, x=560, y=-380)


def water_tower(m):
    """A town water tower on legs, the town's name band round it: a skyline mark."""
    for k in range(6):
        a = k * math.pi / 3
        x, y = math.cos(a) * 330, math.sin(a) * 330
        m.cyl(GREY, (x, y, -60), 20, 1960, steps=6)
    for z in (500, 1100, 1650):
        m.cyl(GREY_D, (0, 0, z), 350, 22, steps=6)
    m.cyl(MINT, (0, 0, 1900), 520, 520, steps=14)
    m.cyl(MINT, (0, 0, 2420), 520, 230, steps=14, top_r=40)
    m.cyl(WHITE, (0, 0, 1820), 540, 80, steps=14)
    m.cyl(HOTPINK, (0, 0, 2130), 526, 110, steps=14, glow=0.35)
    m.sphere(RED, (0, 0, 2680), 30, steps=(4, 6), glow=1.2)
    m.cyl(GREY_D, (0, 0, -60), 40, 1960, steps=8)


def radio_mast(m):
    """A red and white lattice mast with a beacon, on a little transmitter house."""
    m.box(CREAM, (0, 0, 150), (500, 420, 300))
    m.box(CORAL, (0, 0, 312), (540, 460, 24))
    H = 5200
    for k in range(13):
        z = 300 + k * (H - 300) / 13.0
        w = 170 - k * 9
        m.box(RED if k % 2 else WHITE, (0, 0, z + (H - 300) / 26.0), (w, w, (H - 300) / 13.0 - 30))
        m.box(GREY_D, (0, 0, z), (w + 60, w + 60, 14))
    m.cyl(GREY, (0, 0, H), 8, 500, steps=6)
    m.sphere(RED, (0, 0, H + 520), 34, steps=(4, 6), glow=1.4)
    for z, r in ((3600, 150), (4300, 110)):
        m.cyl(WHITE, (120, 0, z), r, 30, steps=10, rot=(0, 0, 80))


def rooftop_sign(m, word='OCEAN', col=HOTPINK):
    """A block of flats with a huge lit word on its roof, on a scaffold: read from far down the road."""
    w, d, h = 2000, 1100, 2400
    m.box(LAV, (0, 0, h / 2.0), (w, d, h))
    m.box(WHITE, (0, 0, h + 20), (w + 60, d + 60, 40))
    for f in range(7):
        z = 240 + f * 300
        m.box(NAVY, (0, -d / 2 - 3, z + 60), (w - 240, 6, 150))
        for j in range(8):
            if (j * 7 + f * 3) % 5 > 1:
                m.box(YEL if (j + f) % 3 else SKY, (-w / 2 + 240 + j * 217, -d / 2 - 7, z + 60), (150, 4, 120), glow=0.4)
        m.box(WHITE, (0, -d / 2 - 40, z - 40), (w - 160, 80, 12))
    n = len(word)
    lw = min(330.0, (w - 100) / n)
    for s in (-1, 1):
        m.box(GREY_D, (s * (w / 2 - 200), -d / 2 + 120, h + 330), (20, 20, 620))
    m.box(GREY_D, (0, -d / 2 + 120, h + 60), (w - 200, 16, 16))
    wall = ft_city.Wall(m, None, (w / 2.0, -d / 2.0 + 100, 0), (-1, 0, 0), (0, -1, 0), w)   # seen from the road: u runs to the viewer's right
    ft_city.text(wall, word, w / 2.0, h + 120, lw * 0.95, col, 0, 14, glow=GLOW)


# ------------------------------------------------------------------------------------------ afloat on the bay, and the horizon
def sailboat(m, hull=WHITE, sail=CREAM, stripe=CORAL):
    """A yacht at anchor: stands with its waterline at z = 0."""
    m.side(hull, [(-520, 0), (520, 0), (640, 150), (-560, 150)], -150, 150)
    m.box(stripe, (30, 0, 120), (1160, 304, 26))
    m.box(hull, (-120, 0, 200), (420, 220, 110))
    m.box(NAVY, (-120, -112, 210), (330, 4, 50))
    m.cyl(GREY, (60, 0, 150), 10, 1500, steps=6)
    m.side(sail, [(80, 260), (80, 1600), (-560, 260)], -4, 4)
    m.side(sail, [(40, 300), (40, 1350), (600, 220)], -4, 4)
    m.box(stripe, (-240, 0, 262), (640, 10, 10))


def motor_yacht(m):
    m.side(WHITE, [(-900, 0), (860, 0), (1100, 230), (-940, 230)], -260, 260)
    m.box(TEAL, (40, 0, 196), (2000, 524, 30))
    m.box(WHITE, (-100, 0, 340), (1100, 400, 220))
    m.box(NAVY, (-100, -202, 350), (980, 4, 90))
    m.box(SKY, (-100, -206, 350), (940, 4, 60), glow=0.3)
    m.box(WHITE, (-200, 0, 520), (620, 320, 150))
    m.box(NAVY, (-200, -162, 530), (540, 4, 70))
    m.cyl(GREY, (-420, 0, 590), 8, 260, steps=6)
    m.sphere(RED, (-420, 0, 860), 16, steps=(4, 6), glow=1.0)
    m.box(CORAL, (-760, 0, 250), (160, 400, 14))


def marina_dock(m, seed=8):
    """Floating jetties with boats tied up and a harbour office on stilts: the marina, waterline at z = 0."""
    r = random.Random(seed)
    m.box(WOOD, (0, 0, 30), (5200, 260, 30))
    for k in range(10):
        m.cyl(WOOD_D, (-2400 + k * 533, 140, -300), 16, 520, steps=6)
    for k in range(6):
        x = -2100 + k * 840
        m.box(WOOD, (x, 600, 30), (160, 1000, 30))
        if r.random() < 0.85:
            c = r.choice([WHITE, CREAM, SKY, MINT])
            s = r.choice((-1, 1))
            m.box(c, (x + s * 240, 620, 70), (190, 620, 110))
            m.box(r.choice([CORAL, TEAL, YEL]), (x + s * 240, 620, 122), (196, 626, 14))
            m.box(NAVY, (x + s * 240, 560, 170), (150, 260, 80))
            if r.random() < 0.6:
                m.cyl(GREY, (x + s * 240, 700, 120), 6, 900, steps=6)
                m.side(CREAM, [(x + s * 240 + 10, 220), (x + s * 240 + 10, 1000), (x + s * 240 - 300, 220)], 690, 698)
    m.box(TEAL, (-2700, -40, 330), (700, 520, 360))                                     # the harbour office
    m.box(WHITE, (-2700, -40, 520), (760, 580, 22))
    m.box(SKY, (-2700, -302, 340), (520, 4, 160), glow=0.3)
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.cyl(WOOD_D, (-2700 + sx * 300, -40 + sy * 220, -300), 18, 460, steps=6)
    m.cyl(WHITE, (-2700, -40, 530), 8, 620, steps=6)
    m.box(HOTPINK, (-2630, -40, 1090), (140, 6, 90))
    m.cyl(RED, (2700, 0, -100), 50, 320, steps=8)                                       # a channel buoy
    m.sphere(YEL, (2700, 0, 260), 30, steps=(4, 6), glow=1.0)


def island(m, seed=11):
    """A low island far out in the bay with palms and a white hotel: waterline at z = 0."""
    r = random.Random(seed)
    m.sphere(SAND, (0, 0, -160), 5200, scale=(1.5, 1.0, 0.085), steps=(6, 14))
    m.sphere(LEAF_D, (-800, 300, 60), 2600, scale=(1.3, 0.9, 0.14), steps=(5, 10))
    m.box(WHITE, (1500, 0, 620), (1500, 900, 900))
    m.box(CORAL, (1500, 0, 1090), (1560, 960, 40))
    for j in range(4):
        m.box(YEL, (1100 + j * 270, -453, 500 + (j % 2) * 260), (150, 6, 110), glow=0.5)
    for k in range(9):
        x, y = r.uniform(-4200, 600), r.uniform(-1500, 1500)
        h = r.uniform(900, 1500)
        m.cyl(TRUNK_D, (x, y, 100), 36, h, steps=5)
        m.sphere(LEAF, (x, y, 100 + h), 330, scale=(1.3, 1.3, 0.4), steps=(4, 7))


# ------------------------------------------------------------------------------------------ the second round
# What the first run's frames asked for: the town read as buildings on blank ground and the beach as blank sand, from
# the road and worse from above. So: streets (a service street behind the shops and side streets back from the road,
# which make the town blocks), cars at the kerb, a cafe and a market among the palms, a bar on the beach, towels and
# dune grass on the sand, a pleasure pier, the funfair's coaster and carousel, and big ships for the bay's horizon.
ASPHALT = '#4b4668'
STRAW, STRAW_D = '#e2b45c', '#b98a3e'
CAR_COLS = [CORAL, MINT, YEL, WHITE, SKY, PINK, LAV, TEAL, RED]


def _car(m, x, y, col, across=False):
    """A boxy parked car, nose along +X (or along +Y with across)."""
    def b(c, dx, dy, z, sx, sy, sz, glow=0.0):
        if across:
            m.box(c, (x + dy, y + dx, z), (sy, sx, sz), glow=glow)
        else:
            m.box(c, (x + dx, y + dy, z), (sx, sy, sz), glow=glow)
    b(col, 0, 0, 62, 440, 180, 64)
    b(NAVY, -20, 0, 118, 230, 164, 52)
    b(col, -20, 0, 146, 210, 170, 10)
    for sx in (-140, 140):
        for sy in (-92, 92):
            b('#1c1a26', sx, sy, 30, 70, 14, 60)
    b(YEL, 221, 0, 66, 4, 150, 16, 0.4)
    b(RED, -221, 0, 66, 4, 150, 16, 0.5)


def parked_cars(m, seed=1):
    """Four cars parked nose to tail at the kerb, a kerb line and two meters: all under 1.6 m, so it may stand right
    behind the barrier."""
    r = random.Random(seed)
    for k in range(4):
        if r.random() < 0.85:
            _car(m, -850 + k * 566, 0, r.choice(CAR_COLS))
    m.box(WHITE, (0, -112, 6), (2300, 14, 12))
    for x in (-560, 560):
        m.box(GREY, (x, -135, 60), (8, 8, 120))
        m.box(GREY_D, (x, -135, 130), (16, 12, 26))


def tiki_bar(m):
    """A round beach bar under two tiers of thatch, stools round it, two torches and a surfboard for a sign."""
    m.cyl(WOOD_D, (0, 0, -40), 190, 145, steps=10)
    m.cyl(WOOD, (0, 0, 105), 215, 12, steps=10)
    for k in range(6):
        a = k * math.pi / 3
        m.box(WOOD_D, (math.cos(a) * 200, math.sin(a) * 200, 130), (16, 16, 340))
        b = a + math.pi / 6
        m.box(WHITE, (math.cos(b) * 290, math.sin(b) * 290, 30), (10, 10, 80))
        m.box([CORAL, TEAL, YEL][k % 3], (math.cos(b) * 290, math.sin(b) * 290, 74), (44, 44, 10))
    m.cyl(STRAW_D, (0, 0, 276), 372, 16, steps=10)
    m.cyl(STRAW, (0, 0, 290), 360, 120, steps=10, top_r=150)
    m.cyl(STRAW_D, (0, 0, 400), 190, 130, steps=10, top_r=14)
    for s in (-1, 1):
        m.box(WOOD_D, (s * 430, -260, 100), (12, 12, 280))
        m.sphere(YEL, (s * 430, -260, 258), 26, steps=(4, 6), glow=0.9)
    m.box(HOTPINK, (460, 120, 170), (40, 12, 300), rot=(0, 0, 10))
    m.box(WHITE, (460, 112, 170), (12, 6, 300), rot=(0, 0, 10))
    m.box(YEL, (0, -196, 150), (150, 6, 8), glow=0.7)


def cafe_terrace(m, seed=3):
    """A pavement cafe: a deck, five tables under striped parasols, a serving hut, a string of coloured lights."""
    cols = [CORAL, TEAL, YEL, HOTPINK, MINT]
    m.box(WOOD, (0, 0, -6), (1700, 1100, 68))
    m.box(WHITE, (-250, -548, 60), (1200, 8, 64))
    for k, (x, y) in enumerate(((-600, -300), (-600, 200), (-150, -40), (300, -300), (300, 200))):
        c = cols[k]
        m.cyl(WHITE, (x, y, 28), 44, 70, steps=6)
        m.box(WHITE, (x, y, 150), (8, 8, 180))
        m.cyl(c, (x, y, 230), 150, 46, steps=8, top_r=10)
        for sx in (-1, 1):
            m.box(c, (x + sx * 80, y, 52), (36, 36, 48))
    m.box(PINK, (640, 250, 170), (320, 420, 284))
    m.box(NAVY, (478, 250, 190), (6, 300, 120))
    m.box(TEAL, (640, 250, 320), (380, 480, 20))
    m.box(YEL, (640, 6, 350), (300, 10, 60), glow=0.6)
    for x in (-820, 820):
        m.box(WHITE, (x, -520, 180), (10, 10, 300))
    for j in range(9):
        m.sphere([YEL, HOTPINK, SKY][j % 3], (-820 + j * 205, -520, 316 - 30 * math.sin(j / 8.0 * math.pi)), 12, steps=(3, 5), glow=0.9)


def market_stalls(m, seed=7):
    """A row of five market stalls: striped awnings over crates of fruit, a neon line along the front."""
    trims = [CORAL, TEAL, YEL, HOTPINK, MINT]
    for k in range(5):
        x = -900 + k * 450
        for sx in (-190, 190):
            m.box(WHITE, (x + sx, -150, 120), (10, 10, 280))
            m.box(WHITE, (x + sx, 150, 140), (10, 10, 320))
        for j in range(4):
            m.box(trims[k] if j % 2 else WHITE, (x - 150 + j * 100, 0, 285), (100, 360, 10), rot=(0, 0, -8))
        m.box(WOOD, (x, -60, 80), (380, 170, 14))
        for j in range(4):
            m.box([RED, YEL, LEAF, CORAL][(j + k) % 4], (x - 135 + j * 90, -60, 108), (74, 130, 40))
        m.box(WOOD_D, (x, 120, 40), (380, 60, 160))
    m.box(HOTPINK, (0, -186, 268), (2250, 6, 8), glow=GLOW)


def towels(m, seed=1):
    """A stretch of beach as people leave it: towels, bags, a cooler, a ball. Low and flat: it is for the views from
    above (the loops and jumps look straight down on the sand). Long along the road, so it lies along the slope."""
    r = random.Random(seed)
    cols = [HOTPINK, TEAL, YEL, CORAL, MINT, LAV, SKY, WHITE]
    for k in range(14):
        x, y = r.uniform(-1400, 1400), r.uniform(-330, 330)
        c = r.choice(cols)
        yaw = r.uniform(0, 180)
        m.box(c, (x, y, 1), (200, 100, 14), rot=(0, yaw, 0))
        m.box(WHITE, (x, y, 2), (200, 30, 14), rot=(0, yaw, 0))
        if r.random() < 0.4:
            m.box(r.choice(cols), (x + 60, y + 90, 16), (46, 34, 44))
    m.sphere(RED, (r.uniform(-600, 600), r.uniform(-250, 250), 22), 24, steps=(4, 6))


def dune_grass(m, seed=2):
    """Low sand hummocks with tufts of grass."""
    r = random.Random(seed)
    for k in range(5):
        x, y = r.uniform(-700, 700), r.uniform(-250, 250)
        s = r.uniform(120, 220)
        m.sphere('#f0d9a4', (x, y, -s * 0.2), s, scale=(1.4, 1.0, 0.45), steps=(4, 6))
        for j in range(5):
            m.box(LEAF if j % 2 else LEAF_D, (x + r.uniform(-s, s) * 0.7, y + r.uniform(-s, s) * 0.5, s * 0.3 + 40), (10, 6, 110),
                  rot=(r.uniform(-25, 25), r.uniform(0, 180), r.uniform(-25, 25)))


def back_street(m):
    """The service street between the shops and the hotels: 47 m of it, one to a slot, end to end. A van, bins,
    a lamp."""
    m.box(ASPHALT, (0, 0, 2), (4700, 800, 40))
    for k in range(8):
        m.box(WHITE, (-2050 + k * 590, 0, 23), (300, 14, 4))
    for y in (-430, 430):
        m.box(CREAM, (0, y, 8), (4700, 60, 52))
    m.box(WHITE, (-900, 250, 110), (480, 190, 170))            # a delivery van
    m.box(SKY, (-1110, 250, 150), (60, 170, 70))
    m.box(CORAL, (-900, 153, 110), (300, 4, 60))
    for k in range(3):
        m.box([TEAL, CORAL, MINT][k], (1200 + k * 150, 330, 76), (120, 100, 110))
    m.box(GREY, (400, 400, 300), (10, 10, 560))                # a lamp
    m.box(GREY, (400, 330, 574), (16, 150, 12))
    m.box(YEL, (400, 270, 566), (40, 60, 8), glow=0.7)


def cross_street(m, seed=4):
    """A side street running back from the road between the blocks, 144 m of it: kerbs, a centre line, a crossing at
    the road end, cars along one kerb. Flat; placed with its middle 90 m from the road's centre."""
    r = random.Random(seed)
    Lh = 7200
    m.box(ASPHALT, (0, 0, 2), (900, 2 * Lh, 40))
    for sx in (-1, 1):
        m.box(CREAM, (sx * 480, 0, 8), (60, 2 * Lh, 52))
    for k in range(24):
        m.box(WHITE, (0, -Lh + 620 + k * 580, 23), (14, 300, 4))
    for k in range(5):
        m.box(WHITE, (-320 + k * 160, -Lh + 200, 23), (90, 300, 4))
    for k in range(5):
        if r.random() < 0.8:
            _car(m, 340, -4200 + k * 2300, r.choice(CAR_COLS), across=True)


def pier(m):
    """A pleasure pier, 300 m of it: from the boardwalk level out over the falling beach to the water, on piles that
    grow as the sand drops away, with lamps, a gateway at the land end and a round pavilion at the sea end.
    Built for a place 190 m from the road on the bay side: local z = 0 is the sand there, the deck is level with
    the boardwalk, and the piles are long enough for the beach's profile (ft_cel.LEVELS[0]['ground'])."""
    Lh, deck = 15000, 630
    m.box(WOOD, (0, 0, deck - 15), (600, 2 * Lh, 30))
    for sx in (-1, 1):
        m.box(WHITE, (sx * 290, 0, deck + 96), (10, 2 * Lh, 10))
        for k in range(11):
            m.box(WHITE, (sx * 290, -Lh + k * 3000, deck + 50), (10, 10, 100))
    for k in range(16):                                         # the piles: the sand is 5.6 m over local zero by the
        y = -Lh + 500 + k * 1930                                # road and 7 m under it at the water
        ground = 560 - (y + Lh) / (2.0 * Lh) * 1300
        for sx in (-1, 1):
            m.box(WOOD_D, (sx * 230, y, (deck + ground - 250) / 2.0), (34, 34, deck - ground + 250))
        m.box(WOOD_D, (0, y, deck - 60), (520, 20, 30))
    for k in range(6):
        y = -Lh + 1500 + k * 5000
        m.box(WHITE, (280, y, deck + 200), (10, 10, 400))
        m.sphere(YEL, (280, y, deck + 420), 30, steps=(4, 6), glow=0.9)
    for sx in (-1, 1):                                          # the gateway
        m.box(WHITE, (sx * 330, -Lh + 60, deck + 250), (44, 44, 500))
    m.box(TEAL, (0, -Lh + 60, deck + 560), (760, 40, 150))
    m.box(HOTPINK, (0, -Lh + 36, deck + 560), (520, 6, 60), glow=GLOW)
    y = Lh - 900                                                # the pavilion
    m.cyl(WOOD, (0, y, deck - 30), 900, 30, steps=8)
    m.cyl(CREAM, (0, y, deck), 700, 420, steps=8)
    m.cyl(YEL, (0, y, deck + 150), 706, 120, steps=8, glow=0.5)
    m.cyl(CORAL, (0, y, deck + 420), 820, 360, steps=8, top_r=90)
    m.sphere(WHITE, (0, y, deck + 800), 60, steps=(4, 6))
    m.box(WHITE, (0, y, deck + 960), (8, 8, 300))
    m.box(HOTPINK, (70, y, deck + 1060), (130, 6, 80))


def coaster(m, seed=5):
    """The funfair's wooden coaster, 150 m long and 34 m high: a pink track over white trestles, a train on the
    first drop, a station at one end."""
    pts = []
    for k in range(31):
        x = -7500 + k * 500
        z = 700 + 1500 * (0.5 + 0.5 * math.sin(x / 1100.0)) + 900 * (0.5 + 0.5 * math.sin(x / 2300.0 + 1.0))
        pts.append((x, 0, z))
    for k in range(30):
        tilt_box(m, HOTPINK, pts[k], pts[k + 1], 170, 46)
    for k in range(31):
        x, _, z = pts[k]
        m.box(WHITE, (x, 0, (z - 60) / 2.0 - 30), (40, 150, z + 60))
        if k < 30:
            tilt_box(m, WHITE, (x, 0, 0), (pts[k + 1][0], 0, pts[k + 1][2] - 120), 26)
    for j in range(3):                                          # the train
        a, b = pts[6 + j], pts[7 + j]
        tilt_box(m, [YEL, TEAL, CORAL][j], (a[0], 0, a[2] + 70), (b[0] - 60, 0, b[2] + 70), 200, 90)
    m.box(CREAM, (-7300, -200, 200), (900, 600, 400))           # the station
    m.box(TEAL, (-7300, -200, 420), (1000, 700, 30))
    m.box(YEL, (-7300, -506, 300), (600, 6, 90), glow=0.6)
    for k in range(8):
        m.sphere([YEL, HOTPINK][k % 2], (pts[3 + k * 3][0], -90, pts[3 + k * 3][2] + 60), 22, steps=(3, 5), glow=0.9)


def carousel(m):
    """A carousel under a coral cone, a ring of bulbs round the rim, horses on poles."""
    m.cyl(CREAM, (0, 0, -40), 520, 80, steps=12)
    m.cyl(WHITE, (0, 0, 40), 60, 420, steps=8)
    m.cyl(YEL, (0, 0, 440), 650, 26, steps=12, glow=0.7)
    m.cyl(CORAL, (0, 0, 460), 640, 230, steps=12, top_r=120)
    m.cyl(WHITE, (0, 0, 690), 120, 120, steps=12, top_r=10)
    m.sphere(YEL, (0, 0, 830), 30, steps=(4, 6), glow=0.9)
    for k in range(8):
        a = k * math.pi / 4
        x, y = math.cos(a) * 400, math.sin(a) * 400
        m.box(WHITE, (x, y, 250), (8, 8, 420))
        m.box([PINK, SKY, YEL, MINT][k % 4], (x, y, 150 + (k % 2) * 70), (110, 40, 70), rot=(0, math.degrees(a) + 90, 0))


def cruise_ship(m):
    """A cruise liner for the bay's horizon: 230 m of white ship, four stepped decks of lit windows, two funnels,
    a neon line along the hull. Afloat: z = 0 is the waterline."""
    L = 23000.0
    m.side(WHITE, [(-L / 2, 0), (-L / 2 + 600, 1500), (L / 2 - 2600, 1500), (L / 2, 1900), (L / 2 - 1500, 0)], -1500, 1500)
    m.box(NAVY, (-300, 0, 210), (L - 2400, 3030, 420))
    for d in range(4):
        x0, x1 = -L / 2 + 2200 + d * 1100, L / 2 - 4600 - d * 1500
        z = 1500 + d * 520
        m.box(WHITE, ((x0 + x1) / 2.0, 0, z + 260), (x1 - x0, 2600 - d * 260, 520))
        m.box(YEL, ((x0 + x1) / 2.0, 0, z + 300), (x1 - x0 - 300, 2620 - d * 260, 150), glow=0.5)
    for k, c in enumerate((CORAL, TEAL)):
        x = -1800 + k * 3400
        m.cyl(c, (x, 0, 3560), 520, 1300, steps=10, top_r=430)
        m.cyl(NAVY, (x, 0, 4860), 440, 160, steps=10)
    for k in range(8):
        for y in (-1340, 1340):
            m.box('#ff9a4d', (-6200 + k * 1500, y, 1640), (900, 220, 240))
    m.box(WHITE, (5200, 0, 4280), (50, 50, 1400))
    for y in (-1524, 1524):
        m.box(HOTPINK, (0, y, 1100), (9000, 6, 90), glow=0.6)


def speedboat(m):
    """A speedboat throwing a long white wake. Afloat."""
    m.side(WHITE, [(-420, 0), (-420, 150), (260, 150), (520, 60), (380, 0)], -130, 130)
    m.box(CORAL, (-40, 0, 156), (560, 264, 12))
    m.box(NAVY, (60, 0, 205), (130, 220, 90), rot=(20, 0, 0))
    m.box(WHITE, (-330, 0, 190), (120, 200, 80))
    for s in (-1, 1):
        tilt_box(m, WHITE, (-380, s * 120, 6), (-2200, s * 760, 6), 90, 12)
    m.box(WHITE, (-900, 0, 6), (900, 200, 12))


JOBS = [
    ('SM_Boardwalk', boardwalk), ('SM_BeachPath', beach_path), ('SM_Cabanas', cabanas), ('SM_Sunbeds', sunbeds),
    ('SM_SurfRack', surf_rack), ('SM_Volleyball', volleyball), ('SM_Breakwater', breakwater),
    ('SM_Kiosk', kiosk), ('SM_KioskDrinks', lambda m: kiosk(m, MINT, CORAL, 'cup')), ('SM_BusStop', bus_stop),
    ('SM_Planters', planters), ('SM_PhoneBooths', phone_booths), ('SM_Flags', flags),
    ('SM_Shops', shops), ('SM_Shops2', lambda m: shops(m, seed=5)), ('SM_Motel', motel), ('SM_Pool', pool),
    ('SM_CarPark', car_park), ('SM_ServiceYard', service_yard), ('SM_Villa', villa),
    ('SM_Villa2', lambda m: villa(m, CREAM, TEAL)), ('SM_WaterTower', water_tower), ('SM_RadioMast', radio_mast),
    ('SM_BlockOcean', lambda m: rooftop_sign(m, 'OCEAN', HOTPINK)), ('SM_BlockVista', lambda m: rooftop_sign(m, 'VISTA', YEL)),
    ('SM_Sailboat', sailboat), ('SM_Sailboat2', lambda m: sailboat(m, CREAM, WHITE, TEAL)), ('SM_MotorYacht', motor_yacht),
    ('SM_MarinaDock', marina_dock), ('SM_Island', island),
    ('SM_ParkedCars', parked_cars), ('SM_ParkedCars2', lambda m: parked_cars(m, seed=9)), ('SM_TikiBar', tiki_bar),
    ('SM_CafeTerrace', cafe_terrace), ('SM_MarketStalls', market_stalls), ('SM_Towels', towels),
    ('SM_Towels2', lambda m: towels(m, seed=8)), ('SM_DuneGrass', dune_grass), ('SM_BackStreet', back_street),
    ('SM_CrossStreet', cross_street), ('SM_Pier', pier), ('SM_Coaster', coaster), ('SM_Carousel', carousel),
    ('SM_CruiseShip', cruise_ship), ('SM_Speedboat', speedboat),
    # more of the strip's hotels: ft_city's own, with real glass, lit rooms and neon (names are our own)
    ('SM_HotelGrand', lambda m: ft_city.hotel(m, 2400, 1400, 13, CREAM, CORAL, HOTPINK, False, YEL, name='GRAND', blade='GRAND')),
    ('SM_HotelSky', lambda m: ft_city.hotel(m, 1600, 1000, 6, SKY, WHITE, CORAL, True, YEL, name='MARINA', blade='MOTEL')),
    ('SM_HotelCoral', lambda m: ft_city.hotel(m, 2000, 1100, 10, CORAL, CREAM, TEAL, False, SKY, name='SUNSET', blade='HOTEL', balcony=True)),
    ('SM_LinerPink', lambda m: ft_city.liner(m, 1700, 1000, 4, PINK, CREAM, TEAL, YEL, name='OCEAN')),
]
