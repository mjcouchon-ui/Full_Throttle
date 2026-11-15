"""FULL THROTTLE (Unreal) dressing kit, level 5: Ice Mountain (Frostbite Peak). Layers of mountain: ice walls, seracs
and frozen falls, rock cuts under netting, cornices and crevasse fields; and the resort that works it: avalanche
fences in rows up the slopes, snow sheds, a rescue post, lift lines climbing out of base stations, a weather station,
snow guns, groomers, a valley lodge. On the horizon: faceted peaks in two depths, a glacier tongue, a far lift line
on a ridge, an observatory on a summit. Placed by Source/FullThrottle/FTDressL5.inl. See ft_dress.py for how a piece
is drawn (front faces -Y, base on z = 0, long axis along X; horizon pieces built from z = 0 up).

    import ft_dress; ft_dress.run(['ice'])

Names matter to the export's finishing pass (ft_visual_art): a name that starts SM_Rock, SM_SnowRock, SM_Ridge or
SM_Peak is finished as rock (a few facets inside its own envelope); SM_Pole... gets a small mounting block; anything
else gets two service panels on its -X end, a quarter of the way up. Compact buildings keep a wall there for them;
open frames and vehicles (the lift span, the weather station, the groomer) carry a thin root far enough underground
that the panels land out of sight (_root); and a rock piece whose facets would hang in the air takes a letter after
its name (the facets are seeded by the name). SkyScratch/SourceArt/Jam/unreal/dress_tools/kit_dry.py works all of that out
without the editor.
"""
import math
import random
import ft_art
from ft_desert import rod, tilt_box
from ft_car import WHITE, BLACK

SNOW, SNOW_SH, ICE, ICE_D, ICE_DEEP = '#f4f9ff', '#cdd6f1', '#bfe3ff', '#7fb0e0', '#4f8fd2'
ROCK, ROCK_D, ROCK_FAR = '#6a7488', '#5a6478', '#a3a6d0'
FAR_ROCK, FAR_ROCK_D = '#8b93b4', '#7a83a8'   # the far mountains: paler than the rock by the road (they stood too dark)
STEEL, STEEL_D = '#9aa4b8', '#5a6478'
PINE, PINE_D, WOOD, WOOD_D = '#2f6b52', '#234f40', '#9a6a44', '#6a4c36'
RED, BLUE, ORANGE, NET = '#ff2d3c', '#2e6bff', '#ff8a2a', '#3b3f52'
LIT, PANE = '#ffe28a', '#1d2740'
CONC = '#aab3c4'


def _root(m, depth, x=0.0, y=0.0):
    """A thin root straight down: it stretches the piece's envelope underground so the finishing pass's panels (sized
    and placed by the envelope) land there instead of hanging in an open frame."""
    m.box(STEEL_D, (x, y, -depth / 2.0), (8, 8, depth))


SLOPE = 6.8   # degrees: the snow slopes of this level rise 12 cm a metre from 80 m out (FTTrack.cpp GroundBands)


def _on_slope(m):
    """Lay the whole piece on the level's snow slope: everything drawn after this rises away from the road (+Y) at the
    slope's own angle. For wide flat things (drift, a crevasse field) that would otherwise stand out of the slope at
    their downhill edge and be buried at the other."""
    m.base = ft_art._xf((0, 0, 0), (0, 0, -SLOPE))


def _cap(m, x, y, z, rx, ry, h=40, col=SNOW):
    """A slab of snow lying on something."""
    m.sphere(col, (x, y, z), 100, scale=(rx / 100.0, ry / 100.0, h / 100.0), steps=(4, 7))


# ------------------------------------------------------------------------------------------ the verge (low, quiet)
def snow_fence(m):
    """Forty metres of slatted snow fence, leaning with the wind, a drift piled against it."""
    L = 4000
    for k in range(11):
        x = -L / 2.0 + k * 400
        m.box(WOOD_D, (x, 0, 60), (12, 12, 240), rot=(0, 0, 10))
    for z in (50, 100, 150):
        m.box(WOOD, (0, -4 - z * 0.17, z), (L, 6, 30))
    for k in range(20):
        m.box(WOOD, (-L / 2.0 + 100 + k * 200, -22, 100), (26, 6, 150), rot=(0, 0, 10))
    m.sphere(SNOW, (0, 90, -40), 1000, scale=(2.0, 0.14, 0.11), steps=(4, 9))
    m.sphere(SNOW_SH, (600, 120, -30), 500, scale=(1.5, 0.2, 0.16), steps=(4, 7))


def snow_bank(m, seed=1):
    """A ploughed bank of snow along the verge, cut square on the road side, marker poles standing in it."""
    r = random.Random(seed)
    m.sphere(SNOW, (0, 60, -30), 1000, scale=(1.3, 0.2, 0.15), steps=(4, 9))
    m.box(SNOW_SH, (0, -70, 40), (2300, 30, 150), rot=(0, 0, -12))
    for k in range(4):
        m.sphere(SNOW_SH if k % 2 else SNOW, (r.uniform(-1000, 1000), r.uniform(0, 120), 20), r.uniform(120, 220), scale=(1.4, 0.8, 0.5), steps=(4, 6))
    for x in (-800, 0, 800):
        m.cyl(BLACK, (x, 30, 60), 4, 190, steps=5)
        m.cyl(ORANGE, (x, 30, 210), 5, 40, steps=5)


def piste_marker(m, col=BLUE):
    """A piste marker: a padded pole and a coloured disc that faces along the road, a white middle on both faces."""
    m.cyl(STEEL_D, (0, 0, -40), 6, 280, steps=6)
    m.cyl(col, (0, 0, 0), 14, 110, steps=8)
    rod(m, col, (-6, 0, 230), (6, 0, 230), 32, steps=10)
    rod(m, WHITE, (-8, 0, 230), (8, 0, 230), 20, steps=10)


def ice_blocks(m, seed=2):
    """Blocks of blue ice fallen by the road, snow on their tops."""
    r = random.Random(seed)
    for k in range(6):
        x, y = r.uniform(-360, 360), r.uniform(-160, 160)
        s = r.uniform(80, 170)
        c = [ICE, ICE_D, ICE_DEEP][k % 3]
        m.box(c, (x, y, s * 0.4), (s * 1.3, s, s), rot=(r.uniform(-14, 14), r.uniform(0, 90), r.uniform(-14, 14)))
        if k % 2 == 0:
            _cap(m, x, y, s * 0.9, s * 0.6, s * 0.5, 22)
    m.sphere(SNOW, (0, 0, -40), 420, scale=(1.1, 0.5, 0.14), steps=(4, 7))


def snow_cannon(m):
    """A snow gun on its trolley: the cabinet, the big fan barrel tipped up, a hose away over the snow."""
    m.box(ORANGE, (-70, 0, 70), (120, 150, 110))
    m.box(STEEL_D, (0, 0, 20), (260, 170, 24))
    for s in (-1, 1):
        rod(m, BLACK, (60, s * 90, 26), (60, s * 104, 26), 26, steps=8)
    rod(m, STEEL_D, (20, 0, 60), (40, 0, 170), 14, steps=6)
    rod(m, STEEL, (0, 0, 190), (130, 0, 250), 56, steps=10)
    rod(m, ORANGE, (124, 0, 247), (140, 0, 255), 62, steps=10)
    tilt_box(m, BLACK, (-130, 40, 30), (-420, 140, 8), 12, 12)
    m.sphere(SNOW, (-40, 0, -20), 220, scale=(1.2, 0.8, 0.14), steps=(4, 6))
    _root(m, 290)


def sled(m):
    """A rescue sled drawn up on the snow, a bundle of marker poles and a coil of rope by it."""
    m.box(ORANGE, (0, 0, 34), (230, 70, 26))
    m.box(ORANGE, (-110, 0, 70), (20, 70, 80), rot=(20, 0, 0))
    for s in (-1, 1):
        m.box(STEEL_D, (0, s * 34, 12), (270, 6, 8))
        rod(m, STEEL, (110, s * 30, 40), (220, s * 30, 110), 4, steps=4)
    for k in range(5):
        rod(m, RED if k % 2 else ORANGE, (-100, 110 + k * 9, 14 + (k % 2) * 8), (150, 120 + k * 9, 14 + (k % 2) * 8), 4, steps=4)
    m.cyl(WOOD, (160, -110, 0), 34, 26, steps=8)
    m.sphere(SNOW, (0, 0, -24), 240, scale=(1.3, 0.9, 0.12), steps=(4, 6))
    _root(m, 130)


# ------------------------------------------------------------------------------------------ the resort's works
def avalanche_fences(m, seed=3):
    """Avalanche fences: three staggered rows of steel rakes stepping up the slope, snow hung on their uphill sides."""
    r = random.Random(seed)
    for row in range(3):
        y, z0 = row * 620, row * 74
        x0 = -1500 + (row % 2) * 320
        for seg in range(3):
            xs = x0 + seg * 1000
            m.box(STEEL_D, (xs + 400, y, z0 + 300), (840, 12, 14))
            m.box(STEEL_D, (xs + 400, y - 60, z0 + 120), (840, 12, 14))
            for k in range(6):
                x = xs + 40 + k * 152
                tilt_box(m, STEEL, (x, y - 110, z0 - 40), (x, y + 30, z0 + 330), 18, 10)
            for x in (xs + 80, xs + 720):
                tilt_box(m, STEEL_D, (x, y + 30, z0 + 300), (x, y + 240, z0 - 60), 18, 18)
            m.sphere(SNOW, (xs + 400, y + 140, z0 + 60), 100, scale=(4.3, 1.5, r.uniform(0.6, 1.1)), steps=(3, 6))
    m.sphere(SNOW_SH, (0, 600, -60), 1700, scale=(1.1, 0.6, 0.08), steps=(4, 8))


def snow_shed(m):
    """An avalanche gallery beside the road: a concrete frame open towards the road through its arches, closed at both
    ends, its sloping roof deep in snow."""
    L, W, H = 3600, 760, 520
    m.box(CONC, (0, W / 2.0 - 40, H / 2.0), (L, 80, H))
    for s in (-1, 1):
        m.box(CONC, (s * (L / 2.0 - 40), 0, H / 2.0), (80, W, H))
        m.box(PANE, (s * (L / 2.0 + 2), 60, 130), (6, 150, 240))
    for k in range(9):
        x = -L / 2.0 + 240 + k * 390
        m.box(CONC, (x, -W / 2.0 + 40, H / 2.0), (90, 80, H))
        m.box(ROCK_D, (x, -W / 2.0 + 40, 40), (110, 100, 80))
    m.box(ROCK_D, (0, 0, H + 30), (L + 60, W + 60, 60), rot=(0, 0, -8))
    m.box(SNOW, (0, 40, H + 110), (L + 120, W + 40, 110), rot=(0, 0, -8))
    _cap(m, 300, 240, H + 190, 900, 260, 90)
    m.box(ORANGE, (0, -W / 2.0 - 4, H - 40), (L - 200, 6, 30))
    m.box(CONC, (0, 0, -150), (L, W, 300))


def rescue_hut(m):
    """The mountain rescue's hut: stone below, timber above, a steep roof under snow, an orange door, skis racked outside."""
    m.box(ROCK, (0, 0, 110), (760, 560, 320))
    m.box(WOOD, (0, 0, 380), (760, 560, 260))
    m.side(SNOW, [(-440, 500), (440, 500), (0, 880)], -330, 330)
    m.side(WOOD_D, [(-400, 505), (400, 505), (0, 840)], -290, 290)
    m.box(ORANGE, (-140, -283, 150), (110, 8, 220))
    m.box(PANE, (160, -283, 300), (220, 8, 120))
    m.box(LIT, (160, -287, 300), (190, 4, 96), glow=1.4)
    m.box(ORANGE, (0, -286, 450), (640, 6, 44))
    m.box(WHITE, (0, -288, 450), (160, 4, 26))
    for k in range(4):
        m.box([RED, BLUE, ORANGE, WHITE][k], (300 + k * 14, -300, 150), (8, 10, 300), rot=(0, 0, -8))
    m.cyl(ROCK_D, (200, 120, 600), 44, 340, steps=6)
    _cap(m, 200, 120, 950, 60, 60, 40)
    m.cyl(STEEL_D, (-300, 180, 600), 6, 520, steps=5)
    m.box(ORANGE, (-240, 180, 1070), (110, 6, 70))
    m.box(ROCK, (0, 0, -300), (800, 600, 600))


def helipad(m):
    """The rescue's landing pad cleared out of the snow: a deck on a timber crib, the ring and the H, a wind sock, a
    store along its near edge with the fuel sledges."""
    S = 1300
    m.box(STEEL_D, (0, 0, 70), (S, S, 36))
    m.box(ORANGE, (0, 0, 90), (S - 160, 50, 6))
    m.box(ORANGE, (0, 0, 90), (50, S - 160, 6))
    for s in (-1, 1):
        m.box(WHITE, (s * 240, 0, 92), (56, 560, 6))
        m.box(ORANGE, (0, s * (S / 2.0 - 50), 90), (S - 100, 36, 6))
        m.box(ORANGE, (s * (S / 2.0 - 50), 0, 90), (36, S - 100, 6))
    m.box(WHITE, (0, 0, 92), (480, 56, 6))
    m.box(WOOD_D, (0, 0, -320), (S - 60, S - 60, 740))
    m.box(WOOD, (-S / 2.0 + 60, 0, 220), (120, S - 160, 280))             # the store (and where the service panels sit)
    m.box(SNOW, (-S / 2.0 + 60, 0, 372), (150, S - 120, 40))
    m.cyl(STEEL_D, (S / 2.0 - 70, -S / 2.0 + 70, 90), 7, 520, steps=6)
    tilt_box(m, ORANGE, (S / 2.0 - 70, -S / 2.0 + 70, 590), (S / 2.0 - 280, -S / 2.0 + 130, 548), 50, 50)
    for sx, sy in ((1, 1), (1, -1), (-1, 1)):
        m.sphere(RED, (sx * (S / 2.0 - 30), sy * (S / 2.0 - 30), 110), 16, steps=(4, 6), glow=1.6)
    m.sphere(SNOW, (0, 0, -40), 1100, scale=(0.55, 0.55, 0.1), steps=(4, 8))


def snowcat(m):
    """A piste groomer: tracks, an orange cab, the blade in front and the tiller behind."""
    for s in (-1, 1):
        m.box(BLACK, (0, s * 150, 60), (560, 90, 120))
        rod(m, STEEL_D, (-260, s * 150, 60), (-260, s * 200, 60), 60, steps=8)
    m.box(ORANGE, (-20, 0, 160), (460, 250, 110))
    m.box(ORANGE, (60, 0, 290), (240, 230, 170))
    m.box(PANE, (184, 0, 300), (6, 200, 110))
    m.box(PANE, (60, -117, 300), (190, 6, 100))
    m.box(STEEL, (330, 0, 80), (30, 420, 150), rot=(-12, 0, 0))
    rod(m, STEEL_D, (-330, -190, 70), (-330, 190, 70), 50, steps=8)
    m.sphere(RED, (60, 0, 388), 14, steps=(4, 6), glow=1.6)
    m.sphere(SNOW, (0, 0, -30), 420, scale=(1.4, 0.9, 0.12), steps=(4, 7))
    _root(m, 350)


def lift_station(m):
    """A chair lift's bottom station: the bullwheel under a canopy, the operator's cabin, the queue's rails, two chairs
    standing in the station."""
    m.box(CONC, (-360, 0, 260), (240, 700, 520))                         # the anchor block on the approach side
    m.box(RED, (0, 0, 560), (1100, 760, 70))
    m.box(SNOW, (0, 0, 620), (1140, 800, 60))
    rod(m, STEEL_D, (60, 0, 440), (60, 0, 520), 250, steps=12)
    m.cyl(STEEL, (60, 0, 0), 40, 520, steps=8)
    for s in (-1, 1):
        m.box(STEEL_D, (360, s * 330, 280), (40, 40, 560))
        rod(m, BLACK, (60, s * 250, 470), (700, s * 250, 520), 5, steps=4)
        rod(m, STEEL_D, (420, s * 250, 500), (420, s * 250, 240), 5, steps=4)
        m.box(RED if s > 0 else BLUE, (420, s * 250, 220), (60, 130, 14))
        m.box(RED if s > 0 else BLUE, (446, s * 250, 262), (8, 130, 76))
    m.box(WOOD, (-380, -470, 150), (200, 200, 300))
    m.box(PANE, (-380, -572, 190), (150, 6, 110))
    m.box(LIT, (-380, -576, 190), (126, 4, 86), glow=1.4)
    m.side(SNOW, [(-500, 300), (-260, 300), (-380, 400)], -590, -350)
    for k in range(4):
        m.box(STEEL_D, (140 + k * 150, -430, 50), (8, 8, 100))
    m.box(STEEL_D, (365, -430, 96), (460, 6, 6))
    for s in (-1, 1):                                                    # the cable on up to the line's first pylon (the LiftLine set: 15 m out, 67 cm higher)
        rod(m, BLACK, (700, s * 250, 520), (1500, s * 330, 1527), 5, steps=4)
    m.box(CONC, (0, 0, -350), (1000, 700, 700))


def lift_span(m, rise=717.0, span=6000.0, h=1500.0):
    """A lift pylon and the cable from it to the next one up the slope (the span runs along +Y, away from the road, and
    climbs `rise`): cross arm, sheaves, three chairs out on the up line and three coming down."""
    m.cyl(STEEL, (0, 0, -700), 46, h + 700, steps=8, top_r=28)
    m.box(STEEL_D, (0, 0, h), (720, 40, 40))
    m.sphere(RED, (0, 0, h + 50), 20, steps=(4, 6), glow=1.6)
    for s in (-1, 1):
        rod(m, BLACK, (s * 330, -30, h - 30), (s * 330, 30, h - 30), 30, steps=8)
        rod(m, NET, (s * 330, 0, h - 40), (s * 330, span, h - 40 + rise), 5, steps=4)
        for k in range(3):
            u = (k + 0.5 + (0.25 if s > 0 else 0.0)) / 3.2
            y, z = span * u, h - 40 + rise * u
            rod(m, STEEL_D, (s * 330, y, z), (s * 330, y, z - 250), 5, steps=4)
            m.box(RED if s > 0 else BLUE, (s * 330, y, z - 265), (120, 60, 14))
            m.box(RED if s > 0 else BLUE, (s * 330, y + 26, z - 225), (120, 8, 70))
    _cap(m, 0, 0, 0, 150, 150, 40)
    _root(m, 1700.0)


def lift_top(m, up=1000.0):
    """The lift's top station, where the last span comes in 14 m over the slope: a stone tower with the drive house on
    it (a big lit window down the hill), the return wheel under a hood out over the cable, a mast."""
    m.box(ROCK, (0, 120, up / 2.0 - 100), (760, 600, up + 200))
    m.box(ROCK_D, (0, -190, up * 0.4), (200, 30, up * 0.8))
    m.box(WOOD, (0, 120, up + 240), (700, 520, 480))
    m.side(SNOW, [(-400, up + 470), (400, up + 470), (0, up + 760)], -160, 400)
    m.side(WOOD_D, [(-370, up + 474), (370, up + 474), (0, up + 720)], -130, 370)
    m.box(PANE, (0, -142, up + 280), (460, 6, 200))
    m.box(LIT, (0, -146, up + 280), (420, 4, 160), glow=1.4)
    m.box(RED, (0, -420, up + 520), (760, 460, 60))
    m.box(SNOW, (0, -420, up + 574), (790, 490, 50))
    rod(m, STEEL_D, (0, -420, up + 410), (0, -420, up + 490), 230, steps=12)
    for s in (-1, 1):
        tilt_box(m, STEEL_D, (s * 340, -180, up + 60), (s * 340, -600, up + 500), 40, 40)
    m.cyl(STEEL_D, (260, 300, up + 740), 6, 520, steps=5)
    m.sphere(RED, (260, 300, up + 1270), 18, steps=(4, 6), glow=1.6)
    m.box(ROCK, (0, 120, -500), (840, 680, 1000))
    _root(m, 1750, 0, 120)


def weather_station(m):
    """A weather station: the observers' hut, a radar ball on a tower, a louvred screen box on legs, an instrument mast."""
    m.box(CONC, (-200, 0, 150), (420, 520, 300))                          # the hut, on the approach side
    m.box(ROCK_D, (-200, 0, -200), (440, 540, 400))
    m.box(ORANGE, (-200, 0, 316), (460, 560, 30))
    m.box(SNOW, (-200, 0, 350), (480, 580, 44))
    m.box(PANE, (-200, -263, 180), (200, 6, 110))
    m.box(LIT, (-200, -267, 180), (170, 4, 86), glow=1.4)
    m.cyl(STEEL, (260, 80, -300), 110, 920, steps=8, top_r=70)
    m.sphere(WHITE, (260, 80, 800), 230, steps=(6, 10))
    m.cyl(STEEL_D, (260, 80, 600), 150, 30, steps=8)
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(STEEL_D, (240 + sx * 36, -330 + sy * 36, 30), (8, 8, 180))
    m.box(WHITE, (240, -330, 160), (110, 110, 110))
    m.cyl(STEEL_D, (480, -160, -40), 8, 900, steps=5)
    m.box(STEEL_D, (480, -160, 800), (8, 260, 8))
    for s in (-1, 1):
        m.sphere(ORANGE, (480, -160 + s * 130, 816), 18, steps=(4, 6))
    m.sphere(RED, (480, -160, 880), 14, steps=(4, 6), glow=1.6)
    m.sphere(SNOW, (100, 100, -40), 700, scale=(1.2, 0.9, 0.1), steps=(4, 8))
    _root(m, 820, -200)


def signal_mast(m):
    """A signal mast banded red and white, dishes on it, a beacon at its head, the equipment cabin at its foot."""
    m.box(CONC, (0, 0, 110), (260, 220, 220))
    m.box(CONC, (0, 0, -250), (280, 240, 500))
    _cap(m, 0, 0, 230, 150, 130, 34)
    H = 2600
    for k in range(10):
        m.cyl(RED if k % 2 else WHITE, (0, 0, 220 + k * (H - 220) / 10.0), 20 - k, (H - 220) / 10.0, steps=6)
    for z, yaw in ((1500, 20), (1900, -50), (2250, 70)):
        m.cyl(WHITE, (0, 0, z), 90, 26, rot=(80, yaw, 0), steps=10, top_r=16)
    m.sphere(RED, (0, 0, H + 20), 26, steps=(4, 6), glow=1.6)


def pipe_stands(m):
    """Beside a half pipe: a judges' tower with a striped awning, a bank of benches on scaffolding, flood lamps on a mast."""
    m.box(WOOD, (-300, 0, 330), (360, 420, 660))                          # the tower, on the approach side
    m.box(PANE, (-300, -212, 520), (300, 6, 150))
    m.box(LIT, (-300, -216, 520), (270, 4, 120), glow=1.4)
    for k in range(5):
        m.box(RED if k % 2 else WHITE, (-440 + k * 70, -290, 640), (70, 190, 10), rot=(0, 0, -16))
    m.box(SNOW, (-300, 0, 690), (400, 460, 44))
    for k in range(5):
        m.box(STEEL, (240, -260 + k * 110, 60 + k * 70), (760, 100, 14))
        m.box(BLUE if k % 2 else ORANGE, (240, -230 + k * 110, 90 + k * 70), (700, 26, 30))
    for x in (-110, 240, 590):
        tilt_box(m, STEEL_D, (x, -300, -40), (x, 260, 380), 14, 14)
        m.box(STEEL_D, (x, 250, 170), (14, 14, 420))
    m.cyl(STEEL_D, (640, 300, -40), 12, 1150, steps=6)
    m.box(STEEL_D, (640, 300, 1090), (40, 300, 90))
    for s in (-1, 0, 1):
        m.box('#fff1c0', (620, 300 + s * 100, 1090), (8, 70, 60), glow=0.8)


def lodge(m):
    """The valley lodge: three floors of timber on a stone ground floor, a wide roof under snow, lit windows, a clock
    in the gable, a balcony the length of the front."""
    L, W = 1500, 900
    m.box(ROCK, (0, 0, 150), (L, W, 300))
    m.box(WOOD, (0, 0, 620), (L, W, 660))
    m.side(SNOW, [(-L / 2.0 - 120, 940), (L / 2.0 + 120, 940), (0, 1500)], -W / 2.0 - 90, W / 2.0 + 90)
    m.side(WOOD_D, [(-L / 2.0 - 60, 946), (L / 2.0 + 60, 946), (0, 1440)], -W / 2.0 - 96, W / 2.0 + 96)
    for f in range(3):
        z = 420 + f * 210
        for j in range(6):
            x = -L / 2.0 + 180 + j * 230
            m.box(PANE, (x, -W / 2.0 - 3, z), (130, 6, 110))
            if (j + f) % 3:
                m.box(LIT, (x, -W / 2.0 - 7, z), (110, 4, 90), glow=1.4)
    m.box(WOOD_D, (0, -W / 2.0 - 70, 520), (L, 140, 14))
    m.box(WOOD_D, (0, -W / 2.0 - 136, 570), (L, 8, 90))
    m.box(RED, (0, -W / 2.0 - 4, 150), (220, 8, 240))
    rod(m, WHITE, (0, -W / 2.0 - 96, 1180), (0, -W / 2.0 - 108, 1180), 90, steps=12)
    m.box(PANE, (0, -W / 2.0 - 110, 1200), (8, 4, 70))
    m.box(PANE, (26, -W / 2.0 - 110, 1180), (50, 4, 8))
    m.cyl(ROCK_D, (400, 200, 1100), 60, 520, steps=6)
    _cap(m, 400, 200, 1630, 80, 80, 44)
    m.box(ROCK, (0, 0, -400), (L + 40, W + 40, 800))


# ------------------------------------------------------------------------------------------ the mountain itself
def ice_wall(m, seed=4, length=3600, h=1500):
    """A wall of blue glacier ice: fluted buttresses in three blues, snow on every ledge and along the top."""
    r = random.Random(seed)
    n = 9
    w = length / float(n)
    for k in range(n):
        x = -length / 2.0 + (k + 0.5) * w
        hh = h * r.uniform(0.62, 1.0)
        d = r.uniform(360, 620)
        m.box([ICE, ICE_D, ICE_DEEP][k % 3], (x, d / 2.0 - 300, hh / 2.0 - 400), (w * 1.04, d, hh + 800), rot=(r.uniform(-4, 4), r.uniform(-5, 5), 0))
        _cap(m, x, d / 2.0 - 300, hh, w * 0.62, d * 0.6, r.uniform(50, 90))
        if k % 2:
            m.box(ICE, (x, -310 - r.uniform(0, 60), hh * 0.3 - 360), (w * 0.5, 140, hh * 0.6 + 870))
            _cap(m, x, -330, hh * 0.6, w * 0.3, 90, 30)
    m.box(ICE_DEEP, (0, 520, h * 0.36 - 325), (length, 500, h * 0.72 + 950))
    m.sphere(SNOW, (0, -380, -60), length * 0.5, scale=(1.0, 0.13, 0.07), steps=(4, 8))


def seracs(m, seed=5):
    """Seracs: leaning towers and blocks of ice where the glacier breaks, snow caught on them."""
    r = random.Random(seed)
    for k in range(9):
        x, y = r.uniform(-900, 900), r.uniform(-500, 500)
        hh = r.uniform(400, 1500)
        w = r.uniform(220, 420)
        m.box([ICE, ICE_D, ICE_DEEP][k % 3], (x, y, hh / 2.0 - 350), (w, w * r.uniform(0.7, 1.1), hh + 700),
              rot=(r.uniform(-14, 14), r.uniform(0, 90), r.uniform(-14, 14)))
        _cap(m, x, y, hh, w * 0.5, w * 0.45, 40)
    m.sphere(SNOW_SH, (0, 0, -110), 1300, scale=(1.1, 0.7, 0.13), steps=(4, 8))


def ice_shelf(m, seed=6):
    """An ice shelf overhanging a rock step: grey rock, a thick blue slab pushed out over it, a cornice of snow on its lip."""
    r = random.Random(seed)
    L = 4000
    for k in range(5):
        x = -L / 2.0 + (k + 0.5) * L / 5.0
        dd, hh = r.uniform(700, 900), r.uniform(560, 760)
        m.box(ROCK if k % 2 else ROCK_D, (x, 250, (hh / 2.0 - 500) / 2.0), (L / 5.0 * 1.05, dd, hh / 2.0 + 1100), rot=(0, r.uniform(-5, 5), 0))
    m.box(ICE_D, (0, 60, 800), (L + 100, 1300, 220), rot=(0, 0, -4))
    m.box(ICE, (0, -40, 930), (L + 60, 1380, 90), rot=(0, 0, -4))
    m.sphere(SNOW, (0, -520, 1010), 100, scale=(L / 200.0 + 0.6, 2.6, 1.3), steps=(4, 10))
    m.box(SNOW, (0, 240, 1010), (L + 40, 1100, 90), rot=(0, 0, -4))
    for k in range(7):
        x = -L / 2.0 + 300 + k * 560 + r.uniform(-100, 100)
        m.cyl(ICE, (x, -640, 690 - r.uniform(0, 80)), 26, r.uniform(120, 300), rot=(180, 0, 0), steps=5, top_r=3)
    m.sphere(SNOW_SH, (0, -500, -110), L * 0.5, scale=(1.0, 0.16, 0.1), steps=(4, 8))


def rock_cut(m, seed=7):
    """A rock cut beside the road under safety netting: benches of grey rock, a mesh pinned over them, snow on the ledges."""
    r = random.Random(seed)
    L = 3600
    for k in range(6):
        x = -L / 2.0 + (k + 0.5) * L / 6.0
        hh = r.uniform(520, 900)
        m.box(ROCK if k % 2 else ROCK_D, (x, 200, hh / 2.0 - 150), (L / 6.0 * 1.05, r.uniform(520, 700), hh + 300), rot=(r.uniform(-3, 3), r.uniform(-4, 4), 0))
        _cap(m, x, 200, hh, L / 12.0, 240, 50)
    m.box(ROCK_D, (0, 520, 500), (L, 300, 1300))
    for k in range(10):                                                   # the netting: two sets of diagonals
        x = -L / 2.0 + k * 400
        tilt_box(m, NET, (x, -150, 20), (x + 520, -120, 560), 10, 6)
        tilt_box(m, NET, (x + 520, -150, 20), (x, -120, 560), 10, 6)
    for z in (30, 560):
        m.box(NET, (0, -140, z), (L, 10, 12))
    for k in range(7):
        m.box(ORANGE, (-L / 2.0 + 300 + k * 500, -146, 560), (30, 12, 30))
    m.box(ORANGE, (-1200, -170, 250), (220, 8, 150))
    m.box(BLACK, (-1200, -176, 250), (150, 4, 80))
    m.sphere(SNOW, (0, -260, -50), L * 0.5, scale=(1.0, 0.12, 0.07), steps=(4, 8))


def frozen_fall(m, seed=8):
    """A frozen waterfall: a cleft in a rock face hung with columns of ice, a frozen pool at its foot."""
    r = random.Random(seed)
    H = 1900
    for s in (-1, 1):
        m.box(ROCK if s < 0 else ROCK_D, (s * 760, 80, H / 2.0 - 400), (1200, 700, H + 800), rot=(0, s * 6, s * 3))
        _cap(m, s * 760, 80, H, 560, 320, 80)
    m.box(ROCK_D, (0, 320, H * 0.5 - 400), (460, 420, H + 800))
    for k in range(9):
        x = -200 + k * 50 + r.uniform(-14, 14)
        hh = H * r.uniform(0.5, 0.98)
        m.cyl([ICE, ICE_D, WHITE][k % 3], (x, -40 - r.uniform(0, 90), H - hh), r.uniform(26, 60), hh, steps=5, top_r=r.uniform(30, 70))
    m.cyl(ROCK_D, (0, -360, -800), 480, 782, steps=10)
    m.cyl(ICE, (0, -360, -20), 430, 40, steps=10)
    m.cyl(WHITE, (0, -300, 4), 200, 30, steps=8)
    for k in range(5):
        m.box(ICE_D, (r.uniform(-360, 360), -360 + r.uniform(-200, 200), 40), (r.uniform(70, 150), r.uniform(60, 110), r.uniform(40, 90)),
              rot=(r.uniform(-12, 12), r.uniform(0, 90), 0))
    _cap(m, 0, -560, -70, 620, 300, 60, SNOW_SH)


def crevasse_field(m, seed=9):
    """A crevasse field: a flat of old snow split by deep blue cracks, flagged poles round its edge. For the high views."""
    r = random.Random(seed)
    _on_slope(m)
    m.box(SNOW_SH, (0, 0, -60), (3600, 2400, 160))
    for k in range(7):
        x = -1500 + k * 500 + r.uniform(-120, 120)
        y0, y1 = r.uniform(-1100, -300), r.uniform(300, 1100)
        tilt_box(m, ICE_DEEP, (x, y0, 22), (x + r.uniform(-300, 300), y1, 22), r.uniform(40, 110), 8)
        tilt_box(m, PANE, (x + 6, y0 + 60, 24), (x + r.uniform(-240, 240), y1 - 60, 24), r.uniform(14, 40), 8)
    for k in range(6):
        a = k * 1.047
        x, y = math.cos(a) * 1700, math.sin(a) * 1100
        m.cyl(BLACK, (x, y, -20), 4, 230, steps=5)
        m.box(RED, (x + 36, y, 180), (70, 4, 44))
    for k in range(4):
        _cap(m, r.uniform(-1500, 1500), r.uniform(-1000, 1000), 20, r.uniform(200, 420), r.uniform(120, 260), 50, SNOW)


def snow_drift(m, seed=10):
    """Wind-carved snow: long ridges of drift all leaning one way, hard blue shadow under their lips."""
    r = random.Random(seed)
    _on_slope(m)
    for k in range(6):
        x, y = r.uniform(-1300, 1300), -700 + k * 280 + r.uniform(-60, 60)
        L = r.uniform(900, 1900)
        m.sphere(SNOW, (x, y, 0), 100, scale=(L / 100.0, 1.5, r.uniform(0.7, 1.3)), steps=(4, 8), rot=(0, 18, 0))
        m.box(ICE_D, (x + 60, y - 110, 30), (L * 1.2, 26, 36), rot=(0, 18, 0))
    m.sphere(SNOW_SH, (0, 0, -60), 1700, scale=(1.1, 0.7, 0.07), steps=(4, 8))


# ------------------------------------------------------------------------------------------ the horizon (from z = 0 up)
def _mount(m, r, x, y, R, H, rock, snow_from=0.42, steps=6):
    """One faceted mountain: rock below, snow from part-way up, a blue shadow slab down the side away from the sun."""
    rock = {ROCK: FAR_ROCK, ROCK_D: FAR_ROCK_D}.get(rock, rock)
    m.cyl(rock, (x, y, -3000), R * 1.25, 3000 + H * snow_from, steps=steps, top_r=R * (1.0 - snow_from * 0.8))
    m.cyl(SNOW, (x, y, H * snow_from), R * (1.0 - snow_from * 0.8), H * (1.0 - snow_from), steps=steps, top_r=R * 0.03)
    a = r.uniform(2.4, 3.6)
    tilt_box(m, SNOW_SH, (x + math.cos(a) * R * 0.56, y + math.sin(a) * R * 0.2, H * snow_from * 1.05), (x + math.cos(a) * R * 0.14, y, H * 0.82), R * 0.16, R * 0.2)
    if steps % 2 == 0:                                                   # the blue facet on some of them only
        tilt_box(m, ICE_D, (x - math.cos(a) * R * 0.5, y - R * 0.3, H * snow_from * 1.02), (x - math.cos(a) * R * 0.1, y - R * 0.06, H * 0.8), R * 0.1, R * 0.2)


def far_peak(m, seed=11, H=24000):
    """A great peak with its shoulders: the level's skyline, faceted, snow from half-way up."""
    r = random.Random(seed)
    _mount(m, r, 0, 2000, H * 0.62, H, ROCK_D, 0.4, 7)
    _mount(m, r, -H * 0.5, 600, H * 0.4, H * 0.62, ROCK, 0.46, 6)
    _mount(m, r, H * 0.46, 0, H * 0.36, H * 0.52, ROCK, 0.5, 6)
    _mount(m, r, H * 0.1, -2600, H * 0.26, H * 0.3, ROCK_D, 0.6, 5)


def far_range(m, seed=12, length=60000, h=13000):
    """A long range of lesser peaks, paler with distance: the second depth of the skyline."""
    r = random.Random(seed)
    n = 8
    for k in range(n):
        x = -length / 2.0 + (k + 0.5) * length / n + r.uniform(-1500, 1500)
        hh = h * r.uniform(0.55, 1.0)
        _mount(m, r, x, r.uniform(-1200, 1200), length / n * r.uniform(0.62, 0.86), hh, ROCK_FAR, r.uniform(0.34, 0.5), 5 + k % 2)


def glacier_tongue(m, seed=13, W=22000, H=11000):
    """A glacier tongue coming down between two rock walls towards the road: stepped blue ice, crevasses across it,
    an ice cliff where it ends."""
    r = random.Random(seed)
    for s in (-1, 1):
        _mount(m, r, s * W * 0.62, 9000, W * 0.34, H * r.uniform(1.1, 1.5), ROCK_D, 0.45, 6)
        _mount(m, r, s * W * 0.5, 1500, W * 0.22, H * 0.7, ROCK, 0.5, 5)
    steps = 6
    for k in range(steps):
        y = -3000 + k * 3600
        z = H * (0.16 + 0.14 * k)
        m.box(ICE if k % 2 else ICE_D, (r.uniform(-600, 600), y, z / 2.0 - 1500), (W * (0.7 - 0.04 * k), 3900, z + 3000), rot=(0, r.uniform(-4, 4), 0))
        for j in range(3):
            m.box(ICE_DEEP, (r.uniform(-W * 0.2, W * 0.2), y - 1700 + j * 1100, z + 12), (W * r.uniform(0.2, 0.5), 150, 30), rot=(0, r.uniform(-10, 10), 0))
        m.box(SNOW, (0, y + 600, z + 30), (W * (0.62 - 0.04 * k), 2200, 60))
    m.box(ICE_DEEP, (0, -4950, H * 0.07 - 1500), (W * 0.68, 300, H * 0.14 + 3000))
    for k in range(6):
        m.box(ICE, (-W * 0.3 + k * W * 0.12, -5200, r.uniform(200, 900)), (r.uniform(500, 1100), r.uniform(400, 800), r.uniform(600, 1800)),
              rot=(r.uniform(-12, 12), r.uniform(0, 90), 0))


def far_lift_line(m, seed=14, length=36000, h=7000):
    """A ridge with a lift line along its crest: six pylons, the cable between them, a top station at the high end."""
    r = random.Random(seed)
    for k in range(6):
        x = -length / 2.0 + (k + 0.5) * length / 6.0
        _mount(m, r, x, r.uniform(-800, 800), length / 6.0 * 0.8, h * (0.62 + 0.07 * k), ROCK, 0.4, 5 + k % 2)
    tops = []
    for k in range(6):
        x = -length * 0.42 + k * length * 0.168
        z = h * (0.6 + 0.07 * k) + 200
        m.cyl(STEEL_D, (x, -600, z - 700), 110, 2700, steps=6, top_r=70)
        m.box(STEEL_D, (x, -600, z + 2000), (140, 1300, 140))
        m.sphere(RED, (x, -600, z + 2150), 110, steps=(4, 6), glow=1.6)
        tops.append((x, z + 1950))
    for k in range(5):
        (x0, z0), (x1, z1) = tops[k], tops[k + 1]
        for s in (-1, 1):
            tilt_box(m, NET, (x0, -600 + s * 560, z0), (x1, -600 + s * 560, z1), 36, 36)
        m.box(RED, ((x0 + x1) / 2.0, -1160, (z0 + z1) / 2.0 - 420), (300, 220, 300))
        tilt_box(m, NET, ((x0 + x1) / 2.0, -1160, (z0 + z1) / 2.0), ((x0 + x1) / 2.0, -1160, (z0 + z1) / 2.0 - 300), 24, 24)
    m.box(WOOD, (tops[-1][0] + 1500, -400, tops[-1][1] - 1500), (2400, 1800, 1500))
    m.box(SNOW, (tops[-1][0] + 1500, -400, tops[-1][1] - 700), (2700, 2100, 220))
    m.box(LIT, (tops[-1][0] + 1500, -1310, tops[-1][1] - 1500), (1500, 30, 500), glow=1.4)


def summit_observatory(m, seed=15, H=17000):
    """An observatory on a far summit: two white domes and a mast on a crag."""
    r = random.Random(seed)
    _mount(m, r, 0, 1500, H * 0.6, H * 0.9, ROCK_D, 0.5, 7)
    _mount(m, r, -H * 0.42, 0, H * 0.34, H * 0.5, ROCK, 0.55, 5)
    m.cyl(ROCK, (H * 0.14, -1200, H * 0.55), H * 0.16, H * 0.2, steps=6, top_r=H * 0.11)
    z = H * 0.75
    m.cyl(WHITE, (H * 0.14, -1200, z), 1300, 900, steps=10)
    m.sphere(WHITE, (H * 0.14, -1200, z + 900), 1300, steps=(6, 10))
    m.box(PANE, (H * 0.14, -2300, z + 1300), (360, 700, 1300))
    m.cyl(WHITE, (H * 0.14 + 2100, -700, z), 760, 520, steps=8)
    m.sphere(WHITE, (H * 0.14 + 2100, -700, z + 520), 760, steps=(5, 8))
    m.cyl(STEEL_D, (H * 0.14 - 1400, -1000, z), 50, 2200, steps=5)
    m.sphere(RED, (H * 0.14 - 1400, -1000, z + 2260), 110, steps=(4, 6), glow=1.6)
    m.box(LIT, (H * 0.14, -2510, z + 380), (900, 30, 260), glow=1.4)


JOBS = [
    ('SM_SnowFence', snow_fence), ('SM_SnowRockBank', snow_bank), ('SM_SnowRockBank2', lambda m: snow_bank(m, seed=21)),
    ('SM_PoleMarker', piste_marker), ('SM_PoleMarker2', lambda m: piste_marker(m, RED)), ('SM_SnowRockIce', ice_blocks),
    ('SM_SnowCannon', snow_cannon), ('SM_Sled', sled),
    ('SM_RidgeFences', avalanche_fences), ('SM_SnowShed', snow_shed), ('SM_HutRescue', rescue_hut), ('SM_Helipad', helipad),
    ('SM_Snowcat', snowcat), ('SM_LiftStation', lift_station), ('SM_LiftSpan', lift_span), ('SM_LiftTop', lift_top),
    ('SM_WeatherStation', weather_station), ('SM_PoleSignal', signal_mast), ('SM_PipeStands', pipe_stands), ('SM_Lodge', lodge),
    ('SM_RockIceWall', ice_wall), ('SM_RockIceWall2', lambda m: ice_wall(m, seed=24, length=3000, h=2300)),
    ('SM_RockSeracB', seracs), ('SM_RockSeracH', lambda m: seracs(m, seed=25)), ('SM_RockIceShelf', ice_shelf), ('SM_RockCutting', rock_cut),
    ('SM_RockIceFalls', frozen_fall), ('SM_SnowRockCrevasse', crevasse_field), ('SM_SnowRockDrift', snow_drift),
    ('SM_PeakGreat', far_peak), ('SM_PeakLesser', lambda m: far_peak(m, seed=31, H=18000)), ('SM_RidgeRange', far_range),
    ('SM_RidgeGlacier', glacier_tongue), ('SM_RidgeLiftLine', far_lift_line), ('SM_PeakObservatory', summit_observatory),
]
