"""Chemical Works, Stage 1, builder s1plants: the night planting of the eight districts. /Game/World/ChemicalWorks/SM_S1PLANTS_*.
Run: import ft_world_chem_s1plants as K; K.run()

22 plant pieces, every one an ft_people.Scene (one section, M_People), every one stirring in the vertex shader only:
a crown rocks about the point where it joins its trunk (so nothing tears), a row nods about its own foot line, a
curtain or a tendril swings about the point it hangs from. No glow anywhere, nothing bobs, nothing turns.
Colours are the kit's (ft_dress_chem.py): foliage is teal, blue or purple; there is NO green (the repair pickup's
colour) and no orange. LOW pieces are 0.8 to 1 m deep strips, top under 1.9 m, bounds under 2.5 m (no shadow pass).
The placing rules: C:/ftwork/s1/L1_s1plants.inl (written by C:/ftwork/jobs/L1s1plants/make_inl.py).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, from, to, width, depth)   rod(m, col, from, to, r, steps)

DIR = '/Game/World/ChemicalWorks'
LOW = dict(grow=(25, 25, 12), grow_down=(25, 25, 0))      # a low piece: bounds stay under 2.5 m

TEAL, TEAL_D, CYAN = '#19b5c9', '#12849a', '#5ff0ff'
BLUE, BLUE_D, BLUE_L = '#3a5ab8', '#33489e', '#4060c0'
PURP, PURP_L, PURP_D = '#6a5ab0', '#7560b8', '#4e45a6'
LILAC, PALE, WHITE = '#b9a8e0', '#d8d0f0', '#eef6ff'
CREAM, LAMP, YEL = '#ffe9a8', '#fff1c0', '#ffd23f'
PINK, PINK_P, MAG, VIO = '#ff7ad9', '#f3c4dc', '#e84aff', '#b44dff'
ORCH, PLUM = '#c391d8', '#8662b8'
RUST, STEEL, NAVY, BLACK = '#a8552f', '#8f9bb3', '#1c1f4a', '#1c1a26'
CONC, CONC_D, PAVE, PAVE_D, GLASS = '#3c4a78', '#2a3560', '#7f8fc6', '#56659f', '#233a63'


def sway(deg, pivot=(0.0, 0.0, 0.0), axis=0.0, rate=0.3, phase=0.0):
    return P.motion(swing=deg, pivot=pivot, axis=axis, rate=rate, phase=phase)


def spike(m, col, a, b, r0, r1, steps=4):
    """A tapering stem, leaf or thorn from a to b (rod's own turning, with a narrow end; never under 2 cm of radius)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, top_r=max(r1, 2.0))


def plate(m, col, c, r, yaw, tilt, h=8.0, steps=6):
    """A round leaf, a cap or a flower: a flat disc whose face looks `tilt` degrees off straight up, towards `yaw`."""
    m.cyl(col, c, r, h, rot=(-tilt, yaw, 0), steps=steps)


def blob(m, col, c, r, sq=(1.0, 1.0, 1.0), steps=(3, 6)):
    m.sphere(col, c, r, scale=sq, steps=steps)


def frond(m, col, hub, yaw, L, rise, droop, w, col2=None, t=9.0):
    """A fern frond from its hub: it rises, arches and hangs."""
    c, s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    pts = [hub,
           (hub[0] + c * L * 0.42, hub[1] + s * L * 0.42, hub[2] + rise),
           (hub[0] + c * L * 0.80, hub[1] + s * L * 0.80, hub[2] + rise - droop * 0.42),
           (hub[0] + c * L, hub[1] + s * L, hub[2] + rise - droop)]
    ws = (w, w * 0.9, w * 0.5)
    for k in range(3):
        tilt_box(m, col2 if (k == 1 and col2) else col, pts[k], pts[k + 1], ws[k], t)


def ring(n, r, a0=0.0):
    return [(math.cos(math.radians(a0 + k * 360.0 / n)) * r, math.sin(math.radians(a0 + k * 360.0 / n)) * r, a0 + k * 360.0 / n) for k in range(n)]


# =============================================================================================== LOW strips (from 17 m)
def cobble_weeds(sc):
    """Weeds through broken paving, 8 m x 0.9 m, top 1.05 m: tufts, dandelion clocks and three yellow flowers."""
    r = random.Random(11)
    g = sc.still()
    for k in range(5):
        g.box(PAVE if k % 2 else PAVE_D, (-330 + k * 165 + r.uniform(-6, 6), 0, -13 + r.uniform(0, 5)), (150, 76, 36), rot=(0, r.uniform(-4, 4), 0))
    g.box(CONC, (-248, 4, 8), (16, 70, 6)); g.box(CONC, (84, -3, 9), (14, 66, 6))        # the cracks, tarred
    parts = [sc.part(sway(5.0, rate=0.32 + 0.05 * k, phase=k / 3.0)) for k in range(3)]
    for k in range(10):                                                                  # tufts in the joints
        x, y = -360 + k * 80 + r.uniform(-14, 14), r.uniform(-24, 24)
        m = parts[k % 3]
        for j in range(4):
            a = r.uniform(0, 6.28)
            hgt = r.uniform(26, 58)
            spike(m, (TEAL_D, BLUE, TEAL, BLUE_D)[(k + j) % 4], (x, y, 4), (x + math.cos(a) * hgt * 0.45, y + math.sin(a) * hgt * 0.2, hgt), 5.0, 2.0, steps=3)
    for k in range(6):                                                                   # dandelion clocks
        x, y, hgt = -340 + k * 132 + r.uniform(-20, 20), r.uniform(-18, 18), r.uniform(62, 92)
        m = parts[(k + 1) % 3]
        rod(m, TEAL_D, (x, y, 2), (x, y, hgt), 2.4, steps=4)
        blob(m, PALE if k % 2 else WHITE, (x, y, hgt + 9), 13)
    for k in range(3):                                                                   # the ones still in flower
        x, y = -270 + k * 250, 20 - k * 16
        m = parts[k]
        rod(m, TEAL_D, (x, y, 2), (x, y, 34), 2.4, steps=4)
        plate(m, YEL, (x, y, 34), 11, 0, 0, h=6)
        plate(m, CREAM, (x, y, 39), 5, 0, 0, h=4)


def privet_hedge(sc):
    """A clipped hedge 9 m long, 1.4 m high, with a cream flowering top, two clipped balls and a white wicket gate."""
    r = random.Random(12)
    g = sc.still()
    g.box(PAVE_D, (0, 0, -9), (900, 78, 42))                                             # the stone kerb it grows from
    g.box(PAVE, (0, -36, 6), (900, 8, 14))
    for x in (130, 228):                                                                 # the wicket gate, shut
        g.box(WHITE, (x, 0, 62), (10, 12, 104)); g.box(PALE, (x, 0, 117), (16, 16, 8))
    for k in range(5):
        g.box(WHITE, (146 + k * 16.5, 0, 56 + (8 if k == 2 else 0)), (8, 5, 80 + (16 if k == 2 else 0)))
    g.box(PALE, (179, 0, 36), (88, 6, 8)); g.box(PALE, (179, 0, 82), (88, 6, 8))
    for k, (x0, x1, top) in enumerate(((-448, -160, 132), (-156, 122, 126), (236, 448, 132))):
        m = sc.part(sway(1.3, rate=0.24 + 0.03 * k, phase=k * 0.37))
        cx, L = (x0 + x1) / 2.0, x1 - x0
        m.box(TEAL_D, (cx, 0, (12 + top) / 2.0), (L, 66, top - 12))
        m.box(BLUE_D, (cx, 31, (12 + top) / 2.0 - 8), (L - 8, 8, top - 34))              # the shade side
        m.box(CREAM, (cx, 0, top + 3), (L - 6, 58, 7))                                   # in flower along the top
        for j in range(int(L / 62)):                                                     # blossom on the road face
            m.box(LAMP, (x0 + 30 + j * 62 + r.uniform(-12, 12), -34, r.uniform(40, top - 16)), (10, 5, 10))
        for j in range(2):                                                               # shoots the shears missed
            sx = x0 + L * (0.3 + 0.4 * j) + r.uniform(-20, 20)
            spike(m, TEAL, (sx, r.uniform(-14, 14), top), (sx + r.uniform(-8, 8), 0, top + r.uniform(22, 34)), 4.0, 2.0, steps=3)
    m = sc.part(sway(1.3, rate=0.24, phase=0.0))
    blob(m, TEAL, (-420, 0, 154), 27, steps=(4, 6))                                      # a clipped ball at each end
    m = sc.part(sway(1.3, rate=0.3, phase=0.74))
    blob(m, TEAL, (420, 0, 154), 27, steps=(4, 6))


def topiary_tubs(sc):
    """Four clipped shapes in white tubs on a paved strip: a ball, a cone, a two-tier poodle, a cube. Top 1.8 m."""
    g = sc.still()
    g.box(PAVE, (0, 0, -11), (780, 84, 34))
    g.box(LILAC, (0, -40, 9), (780, 6, 8))
    for k, x in enumerate((-292, -98, 98, 292)):
        g.cyl(WHITE, (x, 0, 4), 28, 40, steps=8, top_r=33)
        g.cyl(PALE, (x, 0, 40), 35, 6, steps=8)
        g.cyl(CONC_D, (x, 0, 43), 29, 5, steps=8)                                         # the soil
        m = sc.part(sway(2.0, pivot=(0, 0, 46), rate=0.26 + 0.04 * k, phase=k * 0.29))
        rod(m, PLUM, (x, 0, 44), (x, 0, 86), 4.5, steps=5)
        if k == 0:
            blob(m, BLUE, (x, 0, 122), 40, steps=(4, 8)); blob(m, BLUE_L, (x - 10, -12, 136), 24, steps=(3, 6))
        elif k == 1:
            m.cyl(PURP, (x, 0, 62), 37, 116, steps=8, top_r=5)
            m.cyl(PURP_L, (x, 0, 96), 27, 8, steps=8, top_r=24); m.cyl(PURP_L, (x, 0, 132), 16, 7, steps=8, top_r=13)
        elif k == 2:
            blob(m, TEAL_D, (x, 0, 100), 33, steps=(4, 8))
            rod(m, PLUM, (x, 0, 126), (x, 0, 146), 4.5, steps=5)
            blob(m, TEAL, (x, 0, 158), 22, steps=(4, 6))
        else:
            m.box(BLUE_D, (x, 0, 112), (60, 60, 60)); m.box(BLUE, (x, -4, 116), (54, 56, 56))
            blob(m, PALE, (x, 0, 154), 13)


def thistles(sc):
    """Giant thistles and rosebay spires in a strip, 6 m x 0.9 m, top 1.88 m: pink heads on spiny stems."""
    r = random.Random(14)
    g = sc.still()
    for k in range(4):
        g.box(PAVE_D if k % 2 else CONC, (-225 + k * 150, 0, -12), (146, 70, 34), rot=(0, r.uniform(-5, 5), 0))
    parts = [sc.part(sway(3.5, rate=0.27 + 0.04 * k, phase=k * 0.31)) for k in range(3)]
    for k in range(6):                                                                   # the thistles
        x, y, hgt = -270 + k * 108 + r.uniform(-10, 10), r.uniform(-14, 14), r.uniform(118, 160)
        m = parts[k % 3]
        rod(m, TEAL_D, (x, y, 2), (x, y, hgt), 4.2, steps=4)
        for j in range(3):                                                               # spiny leaves, along the strip
            z = hgt * (0.22 + 0.24 * j)
            sg = 1 if (j + k) % 2 else -1
            spike(m, PAVE if j % 2 else BLUE, (x, y, z), (x + sg * (44 - j * 8), y + r.uniform(-8, 8), z + 16), 7.0, 2.0, steps=3)
        blob(m, PLUM, (x, y, hgt + 8), 13)
        m.cyl(PINK, (x, y, hgt + 14), 9, 13, steps=6, top_r=17)                           # the tuft, flaring
        if k % 2 == 0:                                                                   # a side head
            spike(m, TEAL_D, (x, y, hgt * 0.6), (x + 22, y, hgt * 0.6 + 26), 3.2, 2.2, steps=3)
            blob(m, PLUM, (x + 22, y, hgt * 0.6 + 32), 9)
            m.cyl(PINK_P, (x + 22, y, hgt * 0.6 + 37), 6, 9, steps=5, top_r=12)
    for k in range(4):                                                                   # rosebay: magenta spires
        x, y, hgt = -220 + k * 150 + r.uniform(-12, 12), r.uniform(-20, 20), r.uniform(96, 112)
        m = parts[(k + 1) % 3]
        rod(m, BLUE_D, (x, y, 2), (x, y, hgt), 3.4, steps=4)
        m.cyl(MAG, (x, y, hgt - 6), 14, 72, steps=5, top_r=3)
        m.cyl(PINK, (x, y, hgt - 22), 17, 16, steps=5, top_r=14)
        spike(m, TEAL_D, (x, y, hgt * 0.4), (x - 30, y, hgt * 0.4 + 12), 5.0, 2.0, steps=3)


def fungi(sc):
    """Giant fungi of the sludge works, 5 m x 0.95 m, top 1.5 m: a pale stump with brackets, puffballs, nodding toadstools."""
    g = sc.still()
    g.box(CONC_D, (0, 0, -14), (500, 80, 30))
    g.cyl(PALE, (-150, 6, -30), 36, 118, steps=7, top_r=28)                              # the stump
    g.cyl(LILAC, (-150, 6, 88), 26, 4, steps=7)
    for k, (dx, z, rr, col) in enumerate(((-30, 26, 34, ORCH), (26, 46, 30, LILAC), (-22, 66, 24, PINK_P))):
        g.cyl(col, (-150 + dx, -8, z), rr, 9, steps=6)                                   # bracket shelves
        g.cyl(WHITE, (-150 + dx, -8, z - 3), rr - 5, 4, steps=6)
    for (x, y, rr, col) in ((-40, 0, 43, PINK_P), (28, 14, 27, PALE), (206, -8, 36, PINK_P), (-232, -12, 22, WHITE)):
        blob(g, col, (x, y, rr * 0.66), rr, sq=(1, 0.86, 0.8), steps=(4, 7))             # puffballs (they do not move)
        g.box(PLUM, (x, y, rr * 1.44), (9, 9, 5))                                         # the pore
    for k, (x, y, hgt, rr, col) in enumerate(((84, -6, 104, 36, ORCH), (128, 14, 62, 26, VIO), (158, -14, 122, 30, LILAC),
                                               (-208, 16, 54, 20, ORCH), (236, 16, 44, 18, VIO))):
        m = sc.part(sway(2.6, pivot=(x, y, 0), axis=0.0 if k % 2 else 90.0, rate=0.22 + 0.03 * k, phase=k * 0.23))
        m.cyl(PALE, (x, y, 0), rr * 0.26 + 3, hgt, steps=5, top_r=rr * 0.2 + 2)
        m.cyl(WHITE, (x, y, hgt - 5), rr - 5, 5, steps=7)                                # the gills
        m.cyl(col, (x, y, hgt), rr, rr * 0.72, steps=7, top_r=rr * 0.22)
        for j in range(3):
            a = math.radians(40 + j * 120 + k * 30)
            m.box(WHITE, (x + math.cos(a) * rr * 0.52, y + math.sin(a) * rr * 0.52, hgt + rr * 0.36), (7, 7, 6))


def _reeds(sc, n, L, depth, h0, h1, plume, seed, groups, deg):
    r = random.Random(seed)
    parts = [sc.part(sway(deg, rate=0.24 + 0.035 * k, phase=k / float(groups))) for k in range(groups)]
    for k in range(n):
        x = -L / 2.0 + L * (k + 0.5) / n + r.uniform(-8, 8)
        y = r.uniform(-depth / 2.0, depth / 2.0)
        hgt = r.uniform(h0, h1)
        lean = r.uniform(-14, 14)
        m = parts[int((x + L / 2.0) / L * groups * 0.999 + r.choice((0, 0, 1))) % groups]
        tilt_box(m, (TEAL_D, BLUE, TEAL, BLUE_D)[k % 4], (x, y, -4), (x + lean, y, hgt), 6.5, 5.0)
        if k % 2 == 0:                                                                   # a magenta plume
            spike(m, MAG if k % 4 else PINK, (x + lean, y, hgt - 6), (x + lean * 1.5 + 5, y, hgt + plume), plume * 0.3, 2.2, steps=4)
        elif k % 6 == 1:                                                                 # a bulrush head
            m.cyl(PLUM, (x + lean * 0.94, y, hgt - 34), 5.5, 30, steps=5)


def reed_bed(sc):
    """A reed bed 8 m x 0.9 m beside a strip of dark water, top 1.88 m: blades in four waves, magenta plumes."""
    g = sc.still()
    g.box(PAVE_D, (0, 0, -14), (800, 84, 30))
    g.box(GLASS, (0, -22, 2), (770, 30, 5))                                              # still water, unlit
    for k in range(5):
        g.box(PAVE, (-320 + k * 160, -40, 3), (96, 10, 12))                              # edging stones
    _reeds(sc, 30, 760, 36, 108, 156, 30, 21, 4, 4.0)


def reeds_tall(sc):
    """The same bed grown tall, 8 m x 2.2 m, top 2.95 m: a pool with two lily pads, reeds in five waves. TALL."""
    g = sc.still()
    g.box(PAVE_D, (0, 0, -52), (820, 220, 110))
    g.box(GLASS, (0, -52, 5), (760, 96, 5))
    for k in range(6):
        g.box(PAVE, (-340 + k * 136, -106, 6), (100, 12, 16))
    for (x, y, rr) in ((-180, -60, 30), (120, -44, 24), (260, -70, 20)):
        g.cyl(PALE, (x, y, 7), rr, 4, steps=7)
        g.box(GLASS, (x + rr * 0.6, y, 9), (rr * 0.7, 6, 4))
    g.cyl(PINK, (120, -44, 11), 8, 9, steps=5, top_r=12)                                  # one lily in flower
    _reeds(sc, 44, 780, 110, 176, 252, 42, 22, 5, 3.6)


def allotment(sc):
    """A tank man's bed, 8 m x 0.9 m, top 1.89 m: beans on canes, four pale cabbages, three sunflowers."""
    g = sc.still()
    g.box(CONC_D, (0, 0, -12), (800, 80, 32))                                            # dug soil
    for y in (-42, 42):
        g.box(PAVE_D, (0, y, -4), (808, 6, 28))                                          # the boards
    g.box(PALE, (-52, -30, 16), (5, 5, 30)); g.box(WHITE, (-52, -31, 30), (16, 4, 10))   # a label on a stake (blank)
    m = sc.part(sway(1.4, rate=0.25, phase=0.1))                                         # the bean row
    for k in range(4):
        x = -350 + k * 86
        tilt_box(m, PALE, (x, -30, 0), (x, 0, 176), 4.5, 4.5); tilt_box(m, PALE, (x, 30, 0), (x, 0, 176), 4.5, 4.5)
        m.box(TEAL_D, (x, 0, 78), (54, 34, 120)); m.box(TEAL, (x + 10, -14, 112), (36, 22, 60))
        m.box(VIO, (x - 14, -19, 60 + k * 14), (9, 5, 9)); m.box(VIO, (x + 16, -19, 120 - k * 12), (9, 5, 9))
    m.box(PALE, (-221, 0, 174), (300, 5, 5))                                             # the ridge pole
    for k in range(4):                                                                   # cabbages
        x = -30 + k * 66
        blob(g, PALE, (x, 0, 22), 24, sq=(1, 1, 0.8), steps=(4, 6))
        for j in range(3):
            plate(g, LILAC, (x + math.cos(j * 2.1 + k) * 22, math.sin(j * 2.1 + k) * 16, 12), 20, math.degrees(j * 2.1 + k), 38, h=6, steps=5)
    g.cyl(TEAL, (212, 8, 4), 13, 24, steps=7)                                            # a watering can
    spike(g, TEAL, (222, 8, 14), (246, 8, 30), 4.0, 2.4, steps=4)
    g.box(TEAL_D, (212, 8, 34), (22, 5, 5))
    for k, (x, hgt) in enumerate(((272, 150), (322, 163), (372, 142))):                  # sunflowers, facing the road
        m = sc.part(sway(3.4, rate=0.22 + 0.04 * k, phase=k * 0.31))
        rod(m, TEAL_D, (x, 0, 0), (x, 0, hgt), 4.2, steps=5)
        spike(m, TEAL, (x, 0, hgt * 0.45), (x + 30, -6, hgt * 0.45 + 16), 9.0, 2.0, steps=3)
        spike(m, TEAL, (x, 0, hgt * 0.66), (x - 28, -6, hgt * 0.66 + 14), 8.0, 2.0, steps=3)
        plate(m, YEL, (x, -3, hgt), 24, -90, 76, h=5, steps=8)
        plate(m, PLUM, (x, -7, hgt - 1), 12, -90, 76, h=5, steps=6)


def agaves(sc):
    """Agaves and a yucca in three square navy tubs on raked gravel, with two tufts of pale grass. Top 1.85 m."""
    r = random.Random(17)
    g = sc.still()
    g.box(PAVE_D, (0, 0, -12), (760, 88, 32))
    for k in range(6):
        g.box(PAVE, (-316 + k * 126, 6, 5), (60, 70, 4))                                  # the rake's lines
    for k, x in enumerate((-250, 0, 250)):
        g.box(NAVY, (x, 0, 22), (70, 70, 46))
        g.box(PALE, (x, 0, 46), (76, 76, 6))
        for sx in (-1, 1):
            g.box(STEEL, (x + sx * 27, -36, 20), (6, 4, 34))                             # rivet straps on the road face
        if k != 1:                                                                       # an agave: a stiff rosette
            m = sc.part(sway(1.4, pivot=(x, 0, 48), rate=0.2 + 0.05 * k, phase=k * 0.3))
            for (px, py, a) in ring(8, 1.0, 12 * k):
                spike(m, PAVE if (int(a / 45) % 2) else LILAC, (x + px * 8, py * 8, 48), (x + px * 50, py * 38, 92 + 14 * (int(a / 45) % 3)), 9.0, 2.0, steps=4)
            for (px, py, a) in ring(4, 1.0, 40 * k):
                spike(m, BLUE_L, (x + px * 4, py * 4, 50), (x + px * 16, py * 14, 128), 8.0, 2.0, steps=4)
            spike(m, WHITE, (x, 0, 56), (x, 0, 142), 6.0, 2.0, steps=4)                    # the spear at the heart
        else:                                                                            # the yucca, in flower
            m = sc.part(sway(1.8, pivot=(x, 0, 48), rate=0.24, phase=0.55))
            m.cyl(PLUM, (x, 0, 46), 10, 46, steps=6, top_r=8)
            for (px, py, a) in ring(9, 1.0, 5):
                spike(m, TEAL_D if int(a / 40) % 2 else BLUE, (x, 0, 90), (x + px * 44, py * 36, 96 + 12 * (int(a / 40) % 3)), 6.5, 2.0, steps=3)
            m2 = sc.part(sway(3.0, pivot=(x, 0, 92), rate=0.3, phase=0.2))
            rod(m2, PALE, (x, 0, 92), (x, 0, 150), 3.4, steps=5)
            for j in range(5):                                                           # cream bells
                m2.cyl(CREAM, (x + (9 if j % 2 else -9), 0, 118 + j * 13), 8, 12, steps=5, top_r=4)
    for k, x in enumerate((-125, 125)):                                                  # ornamental grass
        m = sc.part(sway(5.0, rate=0.3 + 0.06 * k, phase=0.4 + k * 0.33))
        for j in range(8):
            a = j * 0.785 + k
            spike(m, LILAC if j % 2 else PALE, (x, 0, 6), (x + math.cos(a) * 34, math.sin(a) * 26, r.uniform(60, 96)), 4.5, 2.0, steps=3)


# =============================================================================================== TALL pieces (24 m and out)
def plane_tree(sc):
    """The old plane tree of the yard, 14 m: a mottled pale trunk, a round seat about it, a broad blue-teal crown in
    three masses that rock about the fork, seed balls hanging under it."""
    r = random.Random(3)
    g = sc.still()
    g.cyl(PALE, (0, 0, -150), 50, 760, steps=8, top_r=34)
    for k in range(7):                                                                   # the bark's patches
        a = math.radians(k * 97 + 20)
        z = 60 + k * 74
        rr = 47 - z * 0.021
        g.box(LILAC if k % 2 else STEEL, (math.cos(a) * rr, math.sin(a) * rr, z), (26, 26, 58 + (k % 3) * 14), rot=(0, math.degrees(a), 0))
    g.cyl(PAVE_D, (0, 0, -30), 96, 72, steps=8)                                           # the round seat
    g.cyl(PAVE, (0, 0, 42), 122, 9, steps=8)
    g.cyl(PALE, (0, 0, 51), 70, 30, steps=8, top_r=62)                                    # its back rest
    limbs = (((-250, 60, 860), 26), ((240, -110, 900), 26), ((30, 230, 930), 22), ((-40, -20, 1060), 24))
    for (e, w) in limbs:
        tilt_box(g, PALE, (0, 0, 590), e, w, w)
    lobes = (((-290, 70, 900), 300, BLUE_D, 0), ((-120, -210, 990), 250, TEAL_D, 0), ((-330, -120, 1060), 190, BLUE, 0),
             ((270, -120, 930), 290, TEAL_D, 1), ((330, 120, 1010), 220, BLUE_D, 1), ((150, -260, 1080), 180, BLUE, 1),
             ((20, 240, 980), 280, BLUE_D, 2), ((-30, 0, 1150), 320, TEAL_D, 2), ((40, 60, 1290), 170, BLUE, 2))
    parts = [sc.part(sway(2.2, pivot=(0, 0, 600), axis=(20.0, 85.0, 150.0)[k], rate=0.2 + 0.035 * k, phase=k * 0.31)) for k in range(3)]
    for (c, rr, col, k) in lobes:
        blob(parts[k], col, c, rr, sq=(1, 1, 0.72), steps=(4, 7))
    for k in range(6):                                                                   # seed balls on their strings
        a = math.radians(k * 60 + 15)
        x, y, z = math.cos(a) * 330, math.sin(a) * 300, 760 + (k % 3) * 22
        m = parts[k % 3]
        m.box(PALE, (x, y, z + 44), (4.5, 4.5, 80))
        blob(m, CREAM if k % 2 else PALE, (x, y, z), 15)


def poplars(sc):
    """A row of three slim poplars, 18 m, each swaying from its foot; a white rail between them."""
    g = sc.still()
    g.box(PAVE_D, (0, 0, -60), (1040, 110, 124))
    for k in range(7):
        g.box(WHITE, (-450 + k * 150, -44, 34), (10, 10, 66))
    g.box(PALE, (0, -44, 52), (910, 6, 8))
    for k, (x, hgt) in enumerate(((-420, 1700), (0, 1800), (420, 1620))):
        m = sc.part(sway(2.4, pivot=(x, 0, 0), axis=(0.0, 60.0, 120.0)[k], rate=0.19 + 0.03 * k, phase=k * 0.37))
        m.cyl(PALE, (x, 0, -100), 21, 300, steps=6, top_r=16)
        h1, h2 = hgt * 0.3, hgt * 0.36
        m.cyl(TEAL_D, (x, 0, 150), 100, h1, steps=7, top_r=150)
        m.cyl(TEAL_D, (x, 0, 150 + h1), 150, h2, steps=7, top_r=112)
        m.cyl(TEAL_D, (x, 0, 150 + h1 + h2), 112, hgt - 150 - h1 - h2, steps=7, top_r=10)
        m.cyl(BLUE_D, (x + 34, 34, 260), 96, h1 + h2 - 200, steps=6, top_r=104)           # the shade flank
        m.cyl(TEAL, (x - 40, -40, 330), 80, h1 + h2 - 330, steps=6, top_r=92)             # the lit flank
        m.cyl(TEAL, (x - 26, -26, h1 + h2), 86, hgt * 0.24, steps=6, top_r=22)
        for j in range(3):                                                               # shoots at the foot
            a = j * 2.1 + k
            spike(m, TEAL, (x + math.cos(a) * 20, math.sin(a) * 20, 30), (x + math.cos(a) * 60, math.sin(a) * 46, 150), 7.0, 2.0, steps=3)


def buddleia(sc):
    """Buddleia sprung out of the ballast, 3 m: arching stems with violet and magenta flower spikes, two old sleepers."""
    r = random.Random(6)
    g = sc.still()
    g.cyl(PAVE_D, (0, 0, -100), 190, 112, steps=9, top_r=140)                             # the ballast heap
    g.box(RUST, (-70, -60, 16), (230, 26, 16), rot=(0, 14, 0)); g.box(RUST, (60, 70, 15), (220, 26, 16), rot=(0, -20, 0))
    g.box(STEEL, (0, -110, 16), (300, 9, 14), rot=(0, 4, 0))                              # a length of rail
    for k in range(5):
        g.box(PAVE, (r.uniform(-120, 120), r.uniform(-110, 110), 14), (22, 18, 12))      # loose stones
    parts = [sc.part(sway(3.4, pivot=(0, 0, 10), axis=(0.0, 60.0, 120.0)[k], rate=0.25 + 0.04 * k, phase=k * 0.3)) for k in range(3)]
    for k in range(9):
        a = math.radians(k * 40 + r.uniform(-12, 12))
        reach, top = r.uniform(120, 190), r.uniform(190, 262)
        m = parts[k % 3]
        mid = (math.cos(a) * reach * 0.5, math.sin(a) * reach * 0.5, top)
        end = (math.cos(a) * reach, math.sin(a) * reach, top - r.uniform(20, 50))
        tilt_box(m, TEAL_D, (math.cos(a) * 12, math.sin(a) * 12, 8), mid, 8, 8)
        tilt_box(m, BLUE_D, mid, end, 7, 7)
        tip = (math.cos(a) * (reach + 58), math.sin(a) * (reach + 58), end[2] - 30)
        spike(m, VIO if k % 2 else MAG, end, tip, 15.0, 3.0, steps=5)                     # the flower spike, drooping
        for j in (0.45, 0.8):                                                            # long leaves in pairs
            p = (mid[0] * j * 1.1, mid[1] * j * 1.1, 8 + (top - 8) * j)
            spike(m, TEAL if k % 2 else BLUE, p, (p[0] - math.sin(a) * 44, p[1] + math.cos(a) * 44, p[2] + 10), 7.0, 2.0, steps=3)
        if k % 3 == 0:                                                                   # one standing spike
            spike(m, PINK, mid, (mid[0] * 0.9, mid[1] * 0.9, top + 46), 12.0, 3.0, steps=5)


def dead_tree(sc):
    """A bare white tree of the waste run, 9 m: a leaning trunk, a hollow, bracket fungi, two long branches that creak."""
    g = sc.still()
    spike(g, WHITE, (0, 0, -150), (16, 0, 330), 52, 37, steps=7)
    spike(g, PALE, (16, 0, 325), (40, 10, 570), 37, 24, steps=7)
    spike(g, WHITE, (40, 10, 565), (18, 0, 800), 23, 9, steps=6)
    spike(g, PALE, (18, 0, 795), (64, -6, 905), 9, 3, steps=5)
    for k, (px, py, a) in enumerate(ring(4, 1.0, 30)):                                   # roots
        spike(g, PALE if k % 2 else WHITE, (px * 30, py * 30, 30), (px * 120, py * 120, -30), 18, 6, steps=5)
    g.box(BLACK, (8, -44, 150), (22, 10, 46))                                            # the hollow
    spike(g, PALE, (30, 5, 470), (-70, -30, 540), 12, 7, steps=5)                        # a broken stub
    for k, (z, rr, col) in enumerate(((210, 34, ORCH), (250, 28, LILAC), (300, 22, PINK_P))):
        g.cyl(col, (44 + k * 2, 24 - k * 6, z), rr, 9, steps=6)
    m = sc.part(sway(2.6, pivot=(16, 0, 330), axis=0.0, rate=0.13, phase=0.2))           # the left branch creaks
    spike(m, WHITE, (16, 0, 325), (-190, 24, 560), 21, 12, steps=6)
    spike(m, PALE, (-190, 24, 555), (-268, 0, 770), 12, 4, steps=5)
    spike(m, WHITE, (-150, 20, 520), (-300, 60, 590), 8, 3, steps=4)
    spike(m, WHITE, (-240, 10, 690), (-330, -30, 730), 6, 2.5, steps=4)
    m = sc.part(sway(3.0, pivot=(40, 10, 565), axis=20.0, rate=0.17, phase=0.7))         # the right branch creaks
    spike(m, WHITE, (40, 10, 560), (205, -12, 722), 17, 9, steps=6)
    spike(m, PALE, (205, -12, 718), (256, 0, 884), 9, 3.5, steps=5)
    spike(m, WHITE, (150, -6, 668), (300, 30, 700), 7, 2.5, steps=4)
    spike(m, PALE, (232, -6, 800), (316, -36, 850), 5, 2.2, steps=4)


def willow(sc):
    """A weeping willow, 12 m: a thick leaning trunk, a low dome, twelve long curtains that swing from the crown's rim."""
    r = random.Random(8)
    g = sc.still()
    spike(g, PLUM, (-20, 0, -150), (30, 0, 420), 58, 40, steps=8)
    spike(g, PLUM, (30, 0, 410), (0, 0, 760), 40, 26, steps=7)
    for (e, w) in (((-260, 80, 800), 22), ((250, -60, 820), 22), ((40, 250, 810), 20), ((-30, -250, 800), 20)):
        tilt_box(g, PLUM, (22, 0, 520), e, w, w)
    for k, (px, py, a) in enumerate(ring(3, 1.0, 50)):
        spike(g, PLUM, (px * 30 - 10, py * 30, 30), (px * 150 - 10, py * 150, -30), 20, 7, steps=5)
    top = sc.part(sway(1.6, pivot=(0, 0, 700), axis=40.0, rate=0.18, phase=0.1))
    blob(top, TEAL_D, (0, 0, 900), 430, sq=(1, 1, 0.52), steps=(4, 9))
    blob(top, BLUE_D, (-60, 40, 1030), 300, sq=(1, 1, 0.5), steps=(4, 8))
    blob(top, TEAL, (40, -30, 1110), 170, sq=(1, 1, 0.5), steps=(3, 7))
    for k, (px, py, a) in enumerate(ring(12, 1.0, 9)):
        rad = 400 + (k % 3) * 22
        x, y = px * rad, py * rad
        hang = r.uniform(430, 600)
        m = sc.part(sway(5.5 + (k % 3), pivot=(x, y, 860), axis=a + 90.0 if k % 2 else 90.0, rate=0.2 + 0.02 * (k % 4), phase=k * 0.083))
        m.box(TEAL_D, (x, y, 860 - hang * 0.25), (64, 22, hang * 0.5), rot=(0, a + 90, 0))
        m.box(LILAC if k % 3 == 0 else TEAL, (x, y, 860 - hang * 0.72), (48, 14, hang * 0.56), rot=(0, a + 90, 0))
        m.box(PALE if k % 3 == 0 else CYAN, (x, y, 860 - hang - 8), (22, 8, 30), rot=(0, a + 90, 0))


def tree_ferns(sc):
    """Two tree ferns and a clump of gunnera in the warm mist, 5 m: big fronds that nod, pale croziers, leaves 2 m across."""
    g = sc.still()
    g.cyl(CONC_D, (0, 30, -100), 300, 108, steps=9, top_r=250)
    for n, (fx, fy, hgt, L, nf) in enumerate(((-140, 30, 360, 240, 8), (170, -30, 230, 200, 7))):
        g.cyl(PLUM, (fx, fy, -60), 30, hgt + 60, steps=7, top_r=20)
        for j in range(4):
            g.cyl(PURP_D, (fx, fy, 30 + j * hgt / 4.6), 29 - j * 2, 14, steps=7)         # the old frond bases, in rings
        hub = (fx, fy, hgt)
        ps = [sc.part(sway(4.0, pivot=hub, axis=(0.0, 90.0)[k], rate=0.2 + 0.05 * k + 0.03 * n, phase=0.2 + k * 0.4 + n * 0.3)) for k in range(2)]
        for k in range(nf):
            frond(ps[k % 2], TEAL if k % 2 else TEAL_D, hub, k * 360.0 / nf + n * 20, L, 70, 150, 62, col2=BLUE if k % 2 else TEAL)
        for k in range(3):                                                               # croziers, still curled
            a = math.radians(k * 120 + 40)
            tip = (fx + math.cos(a) * 34, fy + math.sin(a) * 34, hgt + 92)
            spike(ps[0], PALE, hub, tip, 6, 3.5, steps=4)
            blob(ps[0], PALE, tip, 11)
    for k, (lx, ly, hgt, rr, yaw) in enumerate(((60, 190, 150, 100, 80), (-60, 240, 110, 84, 130), (150, 250, 190, 92, 40), (40, 300, 80, 70, 100))):
        m = sc.part(sway(3.0, pivot=(50, 200, 0), axis=0.0, rate=0.24 + 0.03 * k, phase=k * 0.27))
        rod(m, TEAL_D, (50, 200, -10), (lx, ly, hgt), 6.5, steps=5)
        plate(m, PURP_L if k % 2 else PURP, (lx, ly, hgt), rr, yaw, 30, h=9, steps=7)
        plate(m, LILAC, (lx, ly, hgt + 6), rr * 0.3, yaw, 30, h=5, steps=5)
    spike(g, PINK_P, (50, 200, 0), (58, 205, 70), 16, 5, steps=5)                         # the gunnera's flower cone


def cypress(sc):
    """Two columnar cypresses in square navy tubs, 10 m and 8.6 m, deep teal, with a trough of pale grass between."""
    g = sc.still()
    for n, (x, hgt) in enumerate(((-190, 990), (190, 850))):
        g.box(NAVY, (x, 0, -22), (116, 116, 156))
        g.box(PALE, (x, 0, 58), (124, 124, 8))
        for sx in (-1, 1):
            g.box(STEEL, (x + sx * 42, -59, 10), (8, 4, 86))
        m = sc.part(sway(1.6, pivot=(x, 0, 60), axis=(0.0, 70.0)[n], rate=0.17 + 0.04 * n, phase=n * 0.45))
        m.cyl(PLUM, (x, 0, 56), 12, 40, steps=6)
        h1, h2 = hgt * 0.27, hgt * 0.36
        m.cyl(TEAL_D, (x, 0, 90), 56, h1, steps=7, top_r=80)
        m.cyl(TEAL_D, (x, 0, 90 + h1), 80, h2, steps=7, top_r=62)
        m.cyl(TEAL_D, (x, 0, 90 + h1 + h2), 62, hgt - 90 - h1 - h2, steps=7, top_r=7)
        m.cyl(BLUE_D, (x + 22, 22, 150), 52, h1 + h2 - 110, steps=6, top_r=56)
        m.cyl(TEAL, (x - 24, -24, 200), 44, h1 + h2 - 170, steps=6, top_r=50)
        m.cyl(TEAL, (x - 16, -16, h1 + h2 + 20), 48, hgt * 0.2, steps=6, top_r=14)
    g.box(NAVY, (0, -10, 4), (180, 56, 60)); g.box(PALE, (0, -10, 35), (186, 62, 6))    # the trough
    m = sc.part(sway(4.5, pivot=(0, -10, 36), rate=0.3, phase=0.6))
    for j in range(9):
        a = j * 0.7
        spike(m, LILAC if j % 2 else PALE, (-60 + j * 15, -10, 36), (-60 + j * 15 + math.cos(a) * 26, -10 + math.sin(a) * 22, 96 + (j % 3) * 16), 4.5, 2.0, steps=3)


def mop_trees(sc):
    """Two round-headed standard trees in half-drum planters with the tank farm's yellow band, 5 m, staked and tied."""
    g = sc.still()
    for n, (x, hgt) in enumerate(((-180, 400), (180, 360))):
        g.cyl(TEAL, (x, 0, -100), 60, 156, steps=10)
        g.cyl(YEL, (x, 0, 22), 62, 12, steps=10)
        g.cyl(WHITE, (x, 0, 54), 63, 5, steps=10)
        g.cyl(CONC_D, (x, 0, 56), 54, 5, steps=10)
        g.box(PALE, (x + 22, 0, 150), (6, 6, 190))                                        # the stake
        m = sc.part(sway(2.2, pivot=(x, 0, 58), axis=(10.0, 100.0)[n], rate=0.22 + 0.05 * n, phase=n * 0.4))
        rod(m, PLUM, (x, 0, 56), (x, 0, hgt - 60), 7.5, steps=6)
        m.box(WHITE, (x + 10, 0, 228), (30, 8, 6))                                        # the tie
        blob(m, BLUE, (x, 0, hgt), 118, sq=(1, 1, 0.9), steps=(5, 8))
        for k, (px, py, a) in enumerate(ring(4, 78, 30 + n * 45)):
            blob(m, TEAL_D if k % 2 else BLUE_L, (x + px, py, hgt + 18 - 26 * (k % 2)), 62, steps=(3, 6))
        blob(m, TEAL_D, (x, 0, hgt + 82), 64, sq=(1, 1, 0.7), steps=(3, 6))
        for k, (px, py, a) in enumerate(ring(5, 104, 10 + n * 30)):                      # blossom
            m.box(CREAM, (x + px, py, hgt + 30 - k * 14), (13, 13, 13))
    g.box(PAVE_D, (0, 0, -6), (150, 34, 22)); g.box(PALE, (0, 0, 7), (120, 26, 5))       # a plank seat between the drums


def tamarisk(sc):
    """A windblown tamarisk in a cut-drum planter, 3 m, its pink plumes all swept one way; sea holly and thrift beside it."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -100), 62, 152, steps=10)
    g.cyl(TEAL_D, (0, 0, 18), 64, 12, steps=10)
    g.cyl(PALE, (0, 0, 50), 65, 5, steps=10)
    tilt_box(g, PLUM, (0, 0, 46), (56, 0, 150), 15, 15)
    boughs = ((56, 0, 150), (132, 30, 222), 0), ((56, 0, 150), (120, -44, 206), 1), ((56, 0, 150), (70, 6, 246), 2)
    parts = [sc.part(sway(4.0, pivot=(56, 0, 150), axis=90.0, rate=0.24 + 0.05 * k, phase=k * 0.3)) for k in range(3)]
    for (a, b, k) in boughs:
        m = parts[k]
        tilt_box(m, PLUM, a, b, 9, 9)
        for j in range(4):                                                               # feathery plumes, leaning down wind
            t = 0.35 + j * 0.22
            p = (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t)
            q = (p[0] + 84 + j * 8, p[1] + (j - 1.5) * 16, p[2] + 40 - j * 5)
            spike(m, (PINK, PINK_P, ORCH)[(j + k) % 3], p, q, 19.0, 3.0, steps=5)
        spike(m, TEAL_D, a, (a[0] + 60, a[1] + (k - 1) * 30, a[2] + 20), 10.0, 2.5, steps=4)
    g.cyl(PAVE_D, (-150, -10, -30), 46, 56, steps=8)                                      # a second, lower drum end
    g.cyl(PALE, (-150, -10, 26), 48, 4, steps=8)
    for k, (px, py, a) in enumerate(ring(5, 26, 20)):                                    # sea holly
        m = sc.part(sway(3.2, pivot=(-150, -10, 28), axis=0.0 if k % 2 else 90.0, rate=0.3 + 0.02 * k, phase=k * 0.2))
        x, y, z = -150 + px, -10 + py, 78 + (k % 3) * 14
        rod(m, STEEL, (-150 + px * 0.3, -10 + py * 0.3, 28), (x, y, z), 3.0, steps=4)
        blob(m, PAVE, (x, y, z + 8), 11)
        for j in range(4):
            b2 = j * 1.571 + k
            spike(m, LILAC, (x, y, z), (x + math.cos(b2) * 26, y + math.sin(b2) * 26, z + 6), 5.0, 2.0, steps=3)
    blob(g, TEAL_D, (150, -60, 8), 38, sq=(1, 1, 0.5), steps=(3, 7))                      # a cushion of thrift
    m = sc.part(sway(4.0, pivot=(150, -60, 20), rate=0.34, phase=0.5))
    for k, (px, py, a) in enumerate(ring(5, 20, 0)):
        rod(m, TEAL_D, (150 + px * 0.5, -60 + py * 0.5, 20), (150 + px, -60 + py, 46), 2.4, steps=4)
        blob(m, PINK, (150 + px, -60 + py, 51), 8)


def wind_pine(sc):
    """A windblown pine on the mole's rocks, 8 m: a leaning rust trunk, flat pads of needles swept to one side."""
    g = sc.still()
    for (x, y, rr, col) in ((-60, 40, 120, PAVE_D), (90, -50, 90, CONC), (-10, -110, 70, PAVE_D), (160, 70, 60, PAVE)):
        blob(g, col, (x, y, 0), rr, sq=(1, 0.9, 0.6), steps=(3, 6))
    g.cyl(PAVE_D, (0, 0, -100), 150, 100, steps=8)
    spike(g, RUST, (0, 0, -40), (44, 0, 300), 30, 22, steps=6)
    spike(g, RUST, (44, 0, 294), (150, 0, 524), 22, 16, steps=6)
    spike(g, RUST, (20, 0, 200), (-90, 30, 300), 9, 4, steps=4)                           # a dead snag
    m = sc.part(sway(2.0, pivot=(150, 0, 524), axis=90.0, rate=0.2, phase=0.15))
    spike(m, RUST, (150, 0, 518), (270, 0, 650), 16, 9, steps=6)
    spike(m, RUST, (150, 0, 518), (120, 40, 700), 10, 5, steps=5)
    spike(m, RUST, (230, 0, 606), (420, -20, 610), 9, 4, steps=5)
    pads = (((300, 0, 648), 190, TEAL_D), ((150, 40, 722), 150, TEAL_D), ((440, -24, 606), 130, BLUE_D), ((250, -10, 762), 112, TEAL))
    for (c, rr, col) in pads:
        m.cyl(BLUE_D, (c[0], c[1], c[2] - 14), rr * 0.86, 16, steps=7, top_r=rr)          # the shade under a pad
        m.cyl(col, c, rr, 30, steps=7, top_r=rr * 0.74)
        m.cyl(TEAL, (c[0] - rr * 0.2, c[1], c[2] + 30), rr * 0.5, 14, steps=6, top_r=rr * 0.3)
    m2 = sc.part(sway(3.0, pivot=(44, 0, 300), axis=90.0, rate=0.27, phase=0.6))          # a low bough
    spike(m2, RUST, (44, 0, 296), (-120, -30, 380), 10, 5, steps=5)
    m2.cyl(BLUE_D, (-130, -30, 370), 82, 14, steps=7, top_r=92)
    m2.cyl(TEAL_D, (-130, -30, 384), 92, 24, steps=7, top_r=64)
    for k in range(3):                                                                   # cones
        g.box(PLUM, (60 + k * 46, 30 - k * 34, 10), (12, 12, 18))


def pergola(sc):
    """A white pergola 6.4 m long with pale night-flowering climbers: tendrils hang and swing, a bench stands under it."""
    r = random.Random(20)
    g = sc.still()
    g.box(PAVE, (0, 0, -52), (680, 260, 108))
    for x in (-280, 0, 280):
        for y in (-100, 100):
            g.box(WHITE, (x, y, 140), (16, 16, 276))
            g.box(PALE, (x, y, 10), (24, 24, 20))
    for y in (-100, 100):
        g.box(PALE, (0, y, 284), (640, 14, 16))
    for k in range(7):
        g.box(WHITE, (-300 + k * 100, 0, 298), (12, 270, 12))
    g.box(PURP, (0, 60, 44), (200, 44, 8)); g.box(PURP_D, (-80, 60, 20), (10, 40, 40)); g.box(PURP_D, (80, 60, 20), (10, 40, 40))
    g.box(PURP, (0, 82, 76), (200, 8, 50))                                               # the bench and its back
    for (x, y) in ((-280, -100), (280, 100), (0, -100)):                                 # stems twisting up the posts
        tilt_box(g, PLUM, (x + 12, y, 4), (x - 10, y + 8, 140), 7, 7)
        tilt_box(g, PLUM, (x - 10, y + 8, 140), (x + 10, y, 280), 7, 7)
    top = sc.part(sway(1.2, pivot=(0, 0, 290), rate=0.2, phase=0.3))
    for k in range(6):                                                                   # the climber's masses on the roof
        blob(top, TEAL_D if k % 2 else BLUE_D, (-260 + k * 104 + r.uniform(-16, 16), r.uniform(-70, 70), 312), r.uniform(58, 82), sq=(1, 1, 0.34), steps=(3, 6))
    for k in range(10):                                                                  # hanging tendrils with a pale trumpet each
        x, y = -290 + k * 64 + r.uniform(-8, 8), (-106 if k % 2 else 106) + r.uniform(-4, 4)
        hang = r.uniform(80, 150)
        m = sc.part(sway(6.0, pivot=(x, y, 286), axis=0.0, rate=0.26 + 0.02 * (k % 5), phase=k * 0.1))
        m.box(TEAL if k % 3 else TEAL_D, (x, y, 286 - hang / 2.0), (8, 8, hang))
        m.box(TEAL_D, (x + 8, y, 286 - hang * 0.4), (16, 5, 14))
        m.cyl(WHITE if k % 2 else CREAM, (x, y, 286 - hang - 15), 13, 16, steps=6, top_r=5)
        m.box(PALE, (x, y, 286 - hang - 18), (5, 5, 6))


def pollard_limes(sc):
    """A line of three pollarded limes, 6 m: whitewashed feet, a knuckle on each trunk, a fan of straight shoots in leaf."""
    r = random.Random(23)
    g = sc.still()
    g.box(PAVE_D, (0, 0, -52), (980, 120, 108))
    for k in range(6):
        g.box(WHITE if k % 2 else PAVE, (-400 + k * 160, -56, 7), (80, 8, 10))            # a painted kerb
    for n, x in enumerate((-370, 0, 370)):
        g.cyl(PAVE_D, (x, 0, -60), 30, 380, steps=7, top_r=24)
        g.cyl(WHITE, (x, 0, 2), 32, 86, steps=7, top_r=30)                                # the whitewash
        g.box(CONC, (x + 6, -26, 190), (14, 8, 30))                                       # a scar
        hub = (x, 0, 326)
        m = sc.part(sway(2.6, pivot=hub, axis=(0.0, 60.0, 120.0)[n], rate=0.2 + 0.035 * n, phase=n * 0.3))
        blob(m, PAVE_D, hub, 50, sq=(1, 1, 0.8), steps=(4, 6))
        for k in range(9):                                                               # the year's shoots
            a = math.radians(k * 40 + n * 13)
            lean = 0.34 + 0.22 * (k % 3)
            L = r.uniform(210, 280)
            spike(m, TEAL_D if k % 2 else BLUE_D, hub, (x + math.cos(a) * L * lean, math.sin(a) * L * lean, 326 + L * (1.0 - lean * 0.45)), 8.0, 2.4, steps=4)
        for k, (px, py, a) in enumerate(ring(4, 84, n * 30)):                            # the leaf
            blob(m, (TEAL, BLUE, TEAL_D, BLUE_L)[(k + n) % 4], (x + px, py, 486 + (k % 2) * 34), 78, sq=(1, 1, 0.78), steps=(3, 6))
        blob(m, TEAL, (x, 0, 566), 62, sq=(1, 1, 0.8), steps=(3, 6))


def sunflowers(sc):
    """Giant sunflowers and a bean wigwam of the tank men's gardens, 3.2 m: five heavy yellow heads that nod."""
    g = sc.still()
    g.box(CONC_D, (0, 0, -52), (620, 170, 108))
    for y in (-88, 88):
        g.box(PAVE_D, (0, y, -6), (628, 6, 30))
    wig = sc.part(sway(1.2, pivot=(-210, 0, 0), rate=0.2, phase=0.2))                    # the wigwam
    for k, (px, py, a) in enumerate(ring(6, 62, 0)):
        tilt_box(wig, PALE, (-210 + px, py, 0), (-210, 0, 262), 5, 5)
    wig.cyl(TEAL_D, (-210, 0, 20), 60, 190, steps=6, top_r=16)
    wig.cyl(TEAL, (-222, -14, 70), 36, 110, steps=5, top_r=12)
    for k in range(5):
        a = k * 1.3
        wig.box(VIO, (-210 + math.cos(a) * 44, math.sin(a) * 44 - 4, 60 + k * 26), (10, 10, 10))
    wig.box(WHITE, (-210, 0, 258), (16, 16, 8))                                           # the tie at the top
    for k, (x, y, hgt, rr) in enumerate(((-70, 20, 286, 40), (20, -30, 250, 36), (110, 30, 300, 42), (196, -20, 232, 34), (270, 26, 268, 38))):
        m = sc.part(sway(3.0, pivot=(x, y, 0), axis=0.0, rate=0.18 + 0.03 * k, phase=k * 0.21))
        rod(m, TEAL_D, (x, y, -4), (x, y, hgt - 8), 5.5, steps=5)
        for j in range(3):
            z = hgt * (0.3 + 0.2 * j)
            sg = 1 if (j + k) % 2 else -1
            plate(m, TEAL if j % 2 else BLUE, (x + sg * 30, y - 6, z), 26, 0 if sg > 0 else 180, 30, h=6, steps=5)
        plate(m, YEL, (x, y - 6, hgt), rr, -90, 72, h=6, steps=9)
        plate(m, PLUM, (x, y - 12, hgt - 2), rr * 0.5, -90, 72, h=6, steps=7)
    for k in range(4):                                                                   # pale cabbages along the front
        x = -40 + k * 96
        blob(g, PALE, (x, -62, 22), 24, sq=(1, 1, 0.8), steps=(4, 6))
        for j in range(3):
            plate(g, LILAC, (x + math.cos(j * 2.1 + k) * 22, -62 + math.sin(j * 2.1 + k) * 16, 12), 20, math.degrees(j * 2.1 + k), 38, h=6, steps=5)


JOBS = [('SM_S1PLANTS_CobbleWeeds', cobble_weeds, LOW), ('SM_S1PLANTS_PrivetHedge', privet_hedge, LOW),
        ('SM_S1PLANTS_TopiaryTubs', topiary_tubs, LOW), ('SM_S1PLANTS_Thistles', thistles, LOW),
        ('SM_S1PLANTS_Fungi', fungi, LOW), ('SM_S1PLANTS_ReedBed', reed_bed, LOW),
        ('SM_S1PLANTS_Allotment', allotment, LOW), ('SM_S1PLANTS_Agaves', agaves, LOW),
        ('SM_S1PLANTS_PlaneTree', plane_tree), ('SM_S1PLANTS_Poplars', poplars), ('SM_S1PLANTS_Buddleia', buddleia),
        ('SM_S1PLANTS_DeadTree', dead_tree), ('SM_S1PLANTS_ReedsTall', reeds_tall), ('SM_S1PLANTS_Willow', willow),
        ('SM_S1PLANTS_TreeFerns', tree_ferns), ('SM_S1PLANTS_Cypress', cypress), ('SM_S1PLANTS_MopTrees', mop_trees),
        ('SM_S1PLANTS_Tamarisk', tamarisk), ('SM_S1PLANTS_WindPine', wind_pine), ('SM_S1PLANTS_Pergola', pergola),
        ('SM_S1PLANTS_PollardLimes', pollard_limes), ('SM_S1PLANTS_Sunflowers', sunflowers)]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
