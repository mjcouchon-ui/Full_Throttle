"""Chemical Works, Stage 1, builder s1d5: district 5 "Export Terminal" (9.46 to 13.4 km), the port and the boss's arena.
/Game/World/ChemicalWorks/SM_S1D5_*.  Run: import ft_world_chem_s1d5 as K; K.run()

A sea port's waterfront: tall, narrow, gabled quay warehouses with hoist beams, a colonnaded customs house, capped
silos, the harbour master's octagon, the seamen's mission, a chandlery; the quay's own furniture; and THE ANCHOR.
Palette: white (lead), red (one band a building, doors, caps), teal (plinths, shutters), blue steel (roofs), lamp cream
(steady windows; every lit piece is placed 27 m or more out). Because the boss is a truck with a coil, a ring and two
ball electrodes: nothing here has a ring, a ball or a disc on its top, no collars up a shaft, no light at a top, and
the only moving parts are slow swings (a hoisted crate, a weather vane, signal shapes, nets, the anchor's chain).
Every piece: one ft_people.Scene, one section, M_People; cm; z = 0 the land; front = -Y; X along the road."""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ChemicalWorks'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))   # a piece with no moving part: true bounds

W, RED, TEAL, BLUE, CREAM = '#eef6ff', '#ff3355', '#12849a', '#2a3f8f', '#fff1c0'      # the district's five
PAVE, STEEL, GLASS, TEAL_L, DEEP = '#7f8fc6', '#8f9bb3', '#233a63', '#19b5c9', '#1c2a66'  # the kit's: paving, steel, unlit glass
LIT = 0.4                                           # a steady lit window


def gable(g, x, y0, y1, w, he, hr, wall=W, roof=BLUE, over=30.0):
    """A gable end facing the road and its pitched roof: the ridge runs along Y from y0 to y1."""
    g.side(wall, [(x - w / 2.0, he), (x + w / 2.0, he), (x, hr)], y0, y1)
    ym, k = (y0 + y1) / 2.0, (hr - he) / (w / 2.0)
    for s in (-1, 1):
        tilt_box(g, roof, (x + s * (w / 2.0 + over), ym, he - over * k + 12), (x, ym, hr + 12), (y1 - y0) + 2 * over, 22)


def win_x(g, x, y, z, d, h, col=GLASS, glow=0.0, s=1):
    """A window on a wall that looks along X (s: which way)."""
    g.box(col, (x + s * 3.0, y, z), (8, d, h), glow=glow)


# ------------------------------------------------------------------------------------------ buildings
def quay_warehouse(sc):
    """BUILDING. A quay warehouse: 7.6 x 9 m, six storeys, 27 m to the ridge. A column of red loading doors under a
    hoist beam; a crate in a sling hangs from the beam and swings slowly along the wall."""
    g = sc.still()
    w, d, he, hr = 760.0, 900.0, 2200.0, 2720.0
    fy = -d / 2.0
    g.box(TEAL, (0, 0, -65), (w + 60, d + 60, 370))                    # plinth, 2.5 m of footing
    g.box(W, (0, 0, (120 + he) / 2.0), (w, d, he - 120))
    g.box(RED, (0, 0, 205), (w + 14, d + 14, 90))                      # the waterline band
    gable(g, 0, fy, -fy, w, he, hr)
    g.box(RED, (0, fy - 3, 300), (230, 10, 250))                       # the cart door
    g.box(W, (0, fy - 7, 300), (8, 6, 250))
    for k in range(5):
        z = 640 + k * 330
        g.box(RED, (0, fy - 3, z), (150, 10, 210))                     # loading doors, one above another
        g.box(BLUE, (0, fy - 15, z - 112), (190, 34, 12))              # each with its landing
        lit = (k % 2 == 0)
        for s in (-1, 1):
            g.box(CREAM if lit else GLASS, (s * 250, fy - 3, z + 10), (100, 8, 130), glow=LIT if lit else 0.0)
            g.box(TEAL, (s * 322, fy - 3, z + 10), (36, 8, 130))       # a shutter folded back
    rod(g, TEAL, (0, fy - 8, 2400), (0, fy + 2, 2400), 60, steps=8)    # the gable's round vent
    g.box(BLUE, (0, fy - 110, 2600), (28, 260, 34))                    # the hoist beam
    tilt_box(g, BLUE, (0, fy - 10, 2470), (0, fy - 200, 2585), 14, 14)
    for s in (-1, 1):
        for row, z in enumerate((700, 1250, 1800)):
            for y in (-200, 200):
                win_x(g, s * w / 2.0, y, z, 110, 150, CREAM if row == 1 else GLASS, LIT if row == 1 else 0.0, s)
    g.box(TEAL, (w / 2.0 - 34, fy - 7, 1160), (12, 12, 2080))          # a rain pipe
    g.box(BLUE, (0, 150, hr + 30), (60, 130, 50))                      # the ridge vent
    h = sc.part(P.motion(swing=3.0, pivot=(0, fy - 215, 2585), axis=90.0, rate=0.11, phase=0.2))
    h.box(CREAM, (0, fy - 215, 2265), (7, 7, 640))                     # the fall
    h.box(STEEL, (0, fy - 215, 1925), (26, 20, 44))                    # the hook block
    h.box(TEAL, (0, fy - 215, 1850), (120, 90, 100))                   # a crate in its sling
    h.box(CREAM, (0, fy - 215, 1850), (126, 14, 106))


def stepped_row(sc):
    """BUILDING, UNLIT. Three joined sheds of the old quay, 16.9 x 7.6 m: a plain gable (16 m), a crow-stepped gable
    (21 m) and a taller plain one (17.6 m); shuttered, no lit window: it may stand in the flare avenue."""
    g = sc.still()
    d = 760.0
    fy = -d / 2.0
    g.box(TEAL, (5, 0, -75), (1700, d + 60, 350))
    units = ((-560, 500, 1250, 1600, 0), (0, 600, 1650, 2080, 1), (570, 520, 1420, 1760, 0))
    for i, (x, w, he, hr, stepped) in enumerate(units):
        g.box(W, (x, 0, (100 + he) / 2.0), (w, d, he - 100))
        if stepped:
            gable(g, x, fy + 40, -fy, w, he, hr - 70)
            n = 4
            sh = (hr - he) / n
            for j in range(n):
                g.box(W, (x, fy + 20, he + (j + 0.5) * sh), (w * (1.0 - j / float(n)) + (0 if j else 20), 40, sh))
            g.box(RED, (x, fy + 20, hr + 12), (w / n + 30, 48, 24))
            g.box(RED, (x, 0, 185), (w + 14, d + 14, 80))               # the row's one band
        else:
            gable(g, x, fy, -fy, w, he, hr)
            g.box(BLUE, (x, fy - 90, hr - 130), (24, 200, 30))          # hoist beams on the outer two
            g.box(CREAM, (x, fy - 170, hr - 230), (6, 6, 200))
            g.box(STEEL, (x, fy - 170, hr - 345), (20, 14, 30))
        g.box(RED, (x, fy - 3, 235), (w * 0.42, 10, 250))
        z = 560
        while z + 110 < he - 40:
            g.box(RED if stepped else TEAL, (x, fy - 3, z), (130, 10, 200))     # loading doors / closed shutters
            for s in (-1, 1):
                g.box(GLASS, (x + s * (w * 0.5 - 85), fy - 3, z + 10), (80, 8, 110))
            z += 330
    for s, x in ((-1, -810), (1, 830)):
        for y in (-170, 170):
            win_x(g, x, y, 700, 100, 140, GLASS, 0.0, s)


def customs_house(sc):
    """BUILDING. The customs house: 20 x 10 m, a colonnade of six under a pediment, a square turret with a pyramid
    roof and a flagless mast with a yard (23 m). Warm windows behind the columns."""
    g = sc.still()
    g.box(TEAL, (0, 0, -100), (2040, 1020, 300))                       # the stylobate, 2.5 m of footing
    g.box(TEAL, (0, 30, 75), (1960, 920, 50))
    g.box(W, (0, 120, 530), (1800, 740, 860))                          # the hall: front face y = -250
    g.box(RED, (0, 120, 150), (1812, 752, 70))
    for k in range(6):
        x = -750 + k * 300
        g.box(W, (x, -390, 125), (110, 110, 50))
        g.cyl(W, (x, -390, 150), 40, 560, steps=8, top_r=34)
        g.box(W, (x, -390, 725), (104, 104, 30))
    g.box(W, (0, -340, 790), (1840, 230, 100))                         # the entablature
    g.box(BLUE, (0, -340, 852), (1880, 260, 24))
    g.side(W, [(-920, 864), (920, 864), (0, 1090)], -460, -260)        # the pediment
    g.side(RED, [(-700, 884), (700, 884), (0, 1056)], -467, -460)
    g.box(W, (0, -470, 945), (90, 8, 90), rot=(45, 0, 0))              # a plain white lozenge, no seal
    g.box(BLUE, (0, 130, 985), (1840, 760, 50))
    for x in (-600, -300, 300, 600):
        g.box(CREAM, (x, -253, 440), (120, 8, 330), glow=LIT)
    g.box(RED, (0, -254, 290), (170, 10, 380))
    for s in (-1, 1):
        for y in (-60, 150, 360):
            win_x(g, s * 900, y, 480, 110, 300, CREAM, LIT, s)
    g.box(W, (0, 200, 1270), (440, 440, 520))                          # the turret
    g.box(BLUE, (0, 200, 1548), (500, 500, 36))
    g.cyl(BLUE, (0, 200, 1566), 340, 300, rot=(0, 45, 0), steps=4, top_r=14)
    rod(g, TEAL, (0, -26, 1330), (0, -19, 1330), 88, steps=8)          # its porthole
    rod(g, GLASS, (0, -31, 1330), (0, -20, 1330), 68, steps=8)
    g.cyl(STEEL, (0, 200, 1866), 8, 420, steps=6)                      # the mast: a yard, no flag, a flat truck
    g.box(W, (0, 200, 2120), (280, 8, 8))
    g.box(BLUE, (0, 200, 2291), (22, 22, 10))


def harbour_office(sc):
    """BUILDING. The harbour master's office: an octagon 8 m across with a railed gallery 11 m up, a look-out cabin
    under a pointed roof (unlit: nothing glows at its top) and a weather vane that turns slowly. 19.9 m."""
    g = sc.still()
    A, R8 = 0.9239, (0, 22.5, 0)
    g.cyl(TEAL, (0, 0, -250), 500, 370, rot=R8, steps=8)
    g.cyl(W, (0, 0, 120), 410, 1000, rot=R8, steps=8, top_r=370)
    g.cyl(RED, (0, 0, 200), 415, 80, rot=R8, steps=8, top_r=412)
    g.cyl(BLUE, (0, 0, 1060), 395, 60, rot=R8, steps=8, top_r=540)     # the corbel
    g.cyl(BLUE, (0, 0, 1120), 560, 36, rot=R8, steps=8)                # the gallery
    v = [(545 * math.cos(math.radians(22.5 + 45 * k)), 545 * math.sin(math.radians(22.5 + 45 * k))) for k in range(8)]
    for k in range(8):
        a, b = v[k], v[(k + 1) % 8]
        g.box(W, (a[0], a[1], 1208), (9, 9, 105))
        for z in (1205, 1256):
            tilt_box(g, W, (a[0], a[1], z), (b[0], b[1], z), 8, 8)     # a ship's rail, two bars
    g.cyl(W, (0, 0, 1156), 290, 300, rot=R8, steps=8)                  # the look-out
    g.cyl(GLASS, (0, 0, 1270), 295, 110, rot=R8, steps=8)
    g.cyl(BLUE, (0, 0, 1456), 370, 330, rot=R8, steps=8, top_r=12)     # a pointed roof
    g.cyl(STEEL, (0, 0, 1786), 7, 190, steps=6)
    g.box(STEEL, (0, 0, 1860), (150, 7, 7))
    g.box(STEEL, (0, 0, 1860), (7, 150, 7))
    for z in (380, 640, 900):
        r = 410 - 40 * (z - 120) / 1000.0
        g.box(CREAM, (0, -A * r - 2, z), (110, 10, 150), glow=LIT)
        if z != 640:
            for s in (-1, 1):
                g.box(GLASS, (s * (A * r + 2), 0, z), (10, 110, 150))
    g.box(RED, (0, -A * 410 - 1, 235), (130, 12, 230))
    g.box(BLUE, (0, -A * 410 - 30, 362), (190, 70, 12))
    vn = sc.part(P.motion(swing=28.0, pivot=(0, 0, 1930), axis=P.UP, rate=0.07, phase=0.4))
    vn.box(RED, (10, 0, 1930), (260, 8, 12))
    vn.side(RED, [(120, 1902), (195, 1930), (120, 1958)], -5, 5)
    vn.side(W, [(-155, 1896), (-80, 1930), (-155, 1964)], -5, 5)


def twin_silos(sc):
    """BUILDING, UNLIT. Twin silos, 16.2 x 7.6 m, 24.4 m: plain white drums, one red band, pointed red caps with a
    square vent, a closed bridge between them and a weigh hut at their foot. No ribs, no rings, no light."""
    g = sc.still()
    g.box(TEAL, (0, 0, -70), (1620, 760, 360))
    for s in (-1, 1):
        x = s * 430
        g.cyl(W, (x, 0, 110), 330, 1900, steps=12)
        g.cyl(RED, (x, 0, 300), 336, 90, steps=12)
        g.cyl(RED, (x, 0, 2010), 365, 380, steps=12, top_r=46)
        g.box(BLUE, (x, 0, 2415), (70, 70, 50))
        g.box(STEEL, (x + s * 120, -312, 1100), (40, 14, 1700))        # the ladder's strip
        g.box(RED, (x - s * 60, -318, 215), (100, 30, 190))
    g.box(W, (0, 0, 1840), (240, 120, 170))                            # the bridge
    g.box(BLUE, (0, 0, 1933), (260, 140, 16))
    g.box(GLASS, (0, -62, 1850), (130, 8, 60))
    g.box(W, (0, -200, 250), (230, 300, 280))                          # the weigh hut
    tilt_box(g, BLUE, (0, -372, 345), (0, -40, 450), 260, 14)
    g.box(RED, (0, -352, 205), (90, 10, 180))


def seamens_mission(sc):
    """BUILDING. The seamen's mission: a hall 10.4 x 8.6 m with a lean-to, gable to the road, porthole windows lit
    warm, a ship's wheel SHAPE flat on the gable (static, unlit) and a bell cote with no bell. 15.2 m."""
    g = sc.still()
    w, d, he, hr = 1040.0, 860.0, 760.0, 1240.0
    hx, fy = -140.0, -430.0
    g.box(TEAL, (0, 0, -80), (1400, 940, 340))
    g.box(W, (hx, fy - 15, 110), (360, 50, 40))                        # the door step
    g.box(W, (hx, 0, (90 + he) / 2.0), (w, d, he - 90))
    g.box(RED, (hx, 0, 170), (w + 14, d + 14, 70))
    gable(g, hx, fy, -fy, w, he, hr)
    g.box(RED, (hx, fy - 3, 235), (190, 10, 290))
    rod(g, BLUE, (hx, fy - 10, 930), (hx, fy + 2, 930), 130, steps=10)  # the wheel: rim, face, spokes, hub
    rod(g, W, (hx, fy - 14, 930), (hx, fy, 930), 100, steps=10)
    for a in (0, 45, 90, 135):
        g.box(BLUE, (hx, fy - 17, 930), (340, 8, 16), rot=(a, 0, 0))
    rod(g, BLUE, (hx, fy - 24, 930), (hx, fy - 12, 930), 28, steps=8)
    for s in (-1, 1):
        rod(g, TEAL, (hx + s * 330, fy - 8, 420), (hx + s * 330, fy, 420), 80, steps=8)
        rod(g, CREAM, (hx + s * 330, fy - 12, 420), (hx + s * 330, fy - 1, 420), 58, steps=8, glow=LIT)
    for y in (-240, 0, 240):
        for (x, z, s) in ((hx - w / 2.0, 450, -1), (hx + w / 2.0, 560, 1)):
            rod(g, TEAL, (x + s * 8, y, z), (x, y, z), 80, steps=8)
            rod(g, CREAM, (x + s * 12, y, z), (x + s * 1, y, z), 58, steps=8, glow=LIT)
    g.box(W, (hx, fy + 90, hr + 55), (130, 130, 190))                  # the bell cote
    g.box(GLASS, (hx, fy + 23, hr + 75), (70, 8, 90))
    g.cyl(BLUE, (hx, fy + 90, hr + 150), 115, 130, rot=(0, 45, 0), steps=4, top_r=8)
    g.box(W, (530, -100, 235), (300, 620, 290))                        # the lean-to
    tilt_box(g, BLUE, (372, -100, 484), (704, -100, 372), 660, 18)
    g.box(RED, (530, -413, 200), (90, 10, 200))
    win_x(g, 680, -100, 250, 130, 110, GLASS, 0.0, 1)


def chandlery(sc):
    """BUILDING. The chandlery: a narrow shop 5.6 x 7 m, four storeys, 15.8 m: a lit shop window under a striped
    awning, coils of rope and fenders hung on its front, a blank lozenge sign, a hoist."""
    g = sc.still()
    w, d, he, hr = 560.0, 700.0, 1250.0, 1570.0
    fy = -d / 2.0
    g.box(TEAL, (0, 0, -80), (w + 50, d + 50, 340))
    g.box(W, (0, 0, (90 + he) / 2.0), (w, d, he - 90))
    g.box(RED, (0, 0, 470), (w + 12, d + 12, 50))
    gable(g, 0, fy, -fy, w, he, hr)
    g.box(CREAM, (-90, fy - 3, 270), (280, 8, 220), glow=LIT)          # the shop window
    g.box(BLUE, (-90, fy - 6, 270), (8, 8, 220))
    g.box(BLUE, (-90, fy - 6, 270), (280, 8, 8))
    g.box(RED, (180, fy - 3, 245), (110, 10, 270))
    for k in range(5):                                                 # the awning, red and white
        x = -224 + k * 112
        tilt_box(g, RED if k % 2 == 0 else W, (x, fy - 4, 440), (x, fy - 134, 385), 112, 10)
    g.box(BLUE, (20, fy - 22, 652), (520, 8, 8))                       # the rail the goods hang from
    for x in (-180, -40):
        rod(g, CREAM, (x, fy - 16, 590), (x, fy - 2, 590), 50, steps=8)
        rod(g, DEEP, (x, fy - 20, 590), (x, fy - 8, 590), 22, steps=6)
    for x in (100, 165, 230):
        g.cyl(W, (x, fy - 22, 520), 20, 105, steps=6)
        g.cyl(TEAL, (x, fy - 22, 625), 11, 24, steps=6)
    for row, z in enumerate((830, 1090)):
        for s in (-1, 1):
            g.box(CREAM if row == 0 else GLASS, (s * 150, fy - 3, z), (110, 8, 150), glow=LIT if row == 0 else 0.0)
            g.box(TEAL, (s * 226, fy - 3, z), (36, 8, 150))
    g.box(BLUE, (0, fy - 75, 1440), (22, 170, 28))                     # the hoist
    g.box(CREAM, (0, fy - 140, 1330), (6, 6, 200))
    g.box(STEEL, (0, fy - 140, 1218), (20, 14, 30))
    g.box(BLUE, (-248, fy - 60, 700), (8, 120, 8))                     # the sign: a blank white lozenge on red
    g.box(RED, (-248, fy - 72, 640), (10, 90, 90))
    g.box(W, (-248, fy - 72, 640), (14, 46, 46), rot=(0, 0, 45))
    g.box(W, (150, 150, 1480), (70, 70, 200))
    g.box(RED, (150, 150, 1590), (86, 86, 24))
    for s in (-1, 1):
        for y in (-150, 150):
            win_x(g, s * w / 2.0, y, 830, 100, 140, GLASS, 0.0, s)


# ------------------------------------------------------------------------------------------ props
def bollard_row(sc):
    """PROP, LOW (1.02 m), 5.2 x 0.7 m: four mooring bollards on a length of quay edge, a hawser sagging between."""
    g = sc.still()
    g.box(TEAL, (0, 0, -22), (520, 70, 104))
    g.box(W, (0, -31, 24), (520, 10, 14))
    xs = (-195, -65, 65, 195)
    for x in xs:
        g.cyl(BLUE, (x, 0, 30), 17, 58, steps=8, top_r=14)
        g.cyl(W, (x, 0, 88), 25, 14, steps=8)
    for a, b in zip(xs[:-1], xs[1:]):
        m = (a + b) / 2.0
        tilt_box(g, CREAM, (a, 0, 74), (m, 0, 46), 8, 8)
        tilt_box(g, CREAM, (m, 0, 46), (b, 0, 74), 8, 8)
    g.cyl(CREAM, (236, 0, 30), 20, 14, steps=8)


def fender_stack(sc):
    """PROP, LOW (1.2 m), 3.4 x 1.1 m: three white ship's fenders on a pallet and a stack of rope coils. Static, unlit."""
    g = sc.still()
    g.box(TEAL, (0, 0, -20), (340, 110, 68))
    g.box(BLUE, (0, 0, 17), (330, 100, 6))
    for (y, z) in ((-27, 48), (27, 48), (0, 94)):
        rod(g, W, (-150, y, z), (40, y, z), 26, steps=8)
        rod(g, RED, (-65, y, z), (-45, y, z), 27.5, steps=8)
        rod(g, BLUE, (-158, y, z), (-150, y, z), 10, steps=6)
        rod(g, BLUE, (40, y, z), (48, y, z), 10, steps=6)
    g.cyl(CREAM, (110, 0, 20), 44, 16, steps=8)
    g.cyl(CREAM, (110, 0, 36), 38, 16, steps=8)
    g.cyl(CREAM, (110, 0, 52), 32, 14, steps=8)


def boat_trestles(sc):
    """PROP, LOW (1.53 m), 4.3 x 1.3 m: a ship's boat upturned on two trestles, red with a white strake, the paint
    pot beside it."""
    g = sc.still()
    g.box(PAVE, (0, 0, -24), (430, 130, 60))
    hull = [(-190, -45), (-150, -60), (60, -62), (190, 0), (60, 62), (-150, 60), (-190, 45)]
    g.plan(W, hull, 62, 82)
    g.plan(RED, [(x, y * 0.86) for x, y in hull], 82, 120)
    g.plan(RED, [(x * 0.93, y * 0.5) for x, y in hull], 120, 142)
    g.box(TEAL, (0, 0, 147), (330, 10, 12))
    for x in (-110, 110):
        g.box(STEEL, (x, 0, 57), (10, 128, 10))
        for s in (-1, 1):
            tilt_box(g, STEEL, (x - 12, s * 56, 6), (x, s * 40, 54), 8, 8)
            tilt_box(g, STEEL, (x + 12, s * 56, 6), (x, s * 40, 54), 8, 8)
    g.cyl(W, (175, -42, 6), 11, 18, steps=6)
    g.box(TEAL, (175, -42, 34), (6, 6, 22))


def crab_pots(sc):
    """PROP, LOW (1.87 m), 3 x 1.2 m: five creels stacked, a line of small red and white floats, a marker pole."""
    g = sc.still()
    g.box(PAVE, (0, 0, -24), (300, 120, 60))
    for i, (x, z) in enumerate(((-95, 6), (0, 6), (95, 6), (-48, 54), (48, 54))):
        g.box(TEAL_L if i % 2 else TEAL, (x, 4, z + 23), (88, 70, 46))
        g.box(DEEP, (x, -32, z + 23), (60, 6, 26))
        g.box(CREAM, (x, 4, z + 48), (92, 8, 6))
    g.box(CREAM, (-56, -50, 9), (150, 6, 6))
    for k in range(5):
        g.sphere(RED if k % 2 == 0 else W, (-120 + k * 32, -50, 17), 11, steps=(3, 5))
    g.cyl(W, (136, 30, 6), 4.5, 166, steps=6)
    g.cyl(RED, (136, 30, 30), 20, 40, steps=6, top_r=8)
    g.box(W, (136, 30, 172), (30, 6, 30))


def kedge_anchor(sc):
    """PROP, LOW (1.88 m), 2.3 x 1.1 m: a spare kedge anchor stood upright on a block, red with a white stock, a
    heap of chain: the hero's little brother. A square shackle, no ring."""
    g = sc.still()
    g.box(PAVE, (0, 0, -22), (230, 110, 64))
    g.box(W, (0, 0, 22), (70, 70, 24))
    g.box(RED, (0, 0, 95), (16, 16, 130))
    for s in (-1, 1):
        tilt_box(g, RED, (0, 0, 40), (s * 52, 0, 62), 16, 16)
        tilt_box(g, RED, (s * 52, 0, 62), (s * 74, 0, 104), 16, 16)
        g.side(W, [(s * 74 - 18, 96), (s * 74 + 18, 96), (s * 78, 134)], -9, 9)
    g.box(W, (0, 0, 142), (120, 12, 12))
    for s in (-1, 1):
        g.box(RED, (s * 66, 0, 142), (12, 18, 18))
        g.box(STEEL, (s * 12, 0, 170), (7, 10, 24))
    g.box(STEEL, (0, 0, 184), (31, 10, 7))
    r = random.Random(5)
    for k in range(7):
        g.box(STEEL, (-84 + (k % 3) * 9 + r.uniform(-10, 10), r.uniform(-28, 28), 15 + (k % 2) * 8), (26, 12, 10), rot=(0, r.uniform(0, 180), 0))
    for k in range(3):
        g.box(STEEL, (70 + k * 12, -20 + k * 18, 15), (26, 12, 10), rot=(0, 40.0 * k, 0))


def quay_lamp(sc):
    """PROP, TALL (9.1 m), 1.8 m deep: a quay lamp on a curved davit. The lamp hangs UNDER a dark shade below the
    davit's arm (the top of the piece is unlit steel); steady cream light. Only placed 27 m or more out."""
    g = sc.still()
    py = 55.0
    g.box(TEAL, (0, py, -40), (66, 66, 220))
    g.box(W, (0, py, 76), (50, 50, 12))
    g.cyl(BLUE, (0, py, 82), 10, 700, steps=6, top_r=7)
    pts = [(py, 782), (py - 22, 848), (py - 62, 892), (py - 112, 900), (py - 140, 886)]
    for a, b in zip(pts[:-1], pts[1:]):
        tilt_box(g, BLUE, (0, a[0], a[1]), (0, b[0], b[1]), 12, 12)
    g.cyl(BLUE, (0, py - 140, 848), 34, 30, steps=6, top_r=12)          # the shade
    g.box(CREAM, (0, py - 140, 836), (30, 30, 24), glow=0.6)            # the lamp under it
    g.box(BLUE, (0, py, 560), (120, 8, 8))                              # a cross tree and a coil of line on it
    g.box(CREAM, (-48, py, 532), (24, 10, 50))
    g.box(RED, (0, py - 13, 300), (36, 10, 50))


def signal_mast(sc):
    """PROP, TALL (11.8 m), UNLIT: a signal mast with a yard and three plain shapes hung from it (a red cone, a teal
    cylinder, a white lozenge: no flag of any code, no ball) that swing a little. A flat truck on top."""
    g = sc.still()
    g.box(TEAL, (0, 0, -45), (90, 90, 210))
    g.box(W, (0, 0, 66), (70, 70, 12))
    g.cyl(W, (0, 0, 72), 12, 700, steps=6, top_r=9)
    g.box(BLUE, (0, 0, 780), (34, 34, 22))
    g.cyl(RED, (0, 0, 790), 8, 380, steps=6, top_r=6)
    g.box(BLUE, (0, 0, 1174), (22, 22, 10))
    g.box(W, (0, 0, 930), (440, 10, 10))
    for s in (-1, 1):
        tilt_box(g, STEEL, (s * 215, 0, 934), (0, 0, 1100), 6, 6)
    g.box(STEEL, (40, 0, 500), (6, 6, 860))                             # the halyard, made fast at a cleat
    g.box(BLUE, (30, 0, 100), (30, 8, 8))
    r = random.Random(9)
    for i, x in enumerate((-170, 90, 185)):
        m = sc.part(P.motion(swing=7.0, pivot=(x, 0, 925), axis=90.0, rate=0.13, phase=r.random()))
        m.box(CREAM, (x, 0, 895), (6, 6, 60))
        if i == 0:
            m.cyl(RED, (x, 0, 790), 40, 76, steps=6, top_r=4)
        elif i == 1:
            m.cyl(TEAL_L, (x, 0, 790), 30, 76, steps=6)
        else:
            m.cyl(W, (x, 0, 790), 5, 38, steps=6, top_r=34)
            m.cyl(W, (x, 0, 828), 34, 38, steps=6, top_r=5)


def gangway(sc):
    """PROP, TALL (3.5 m), 7.2 x 1.5 m, UNLIT: a ship's gangway on its wheeled tower, laid ALONG the road (never a
    ramp towards it): a brow with red canvas dodgers and a white stripe, its outer end on a trestle."""
    g = sc.still()
    g.box(PAVE, (10, 0, -24), (720, 150, 60))
    for x in (-300, -120):
        for s in (-1, 1):
            rod(g, DEEP, (x, s * 62, 34), (x, s * 74, 34), 28, steps=8)
    g.box(BLUE, (-210, 0, 56), (250, 120, 14))
    for x in (-320, -100):
        for s in (-1, 1):
            g.box(STEEL, (x, s * 52, 156), (8, 8, 190))
            g.box(W, (x, s * 58, 306), (8, 8, 100))
    for s in (-1, 1):
        tilt_box(g, STEEL, (-320, s * 52, 66), (-100, s * 52, 246), 8, 8)
        g.box(W, (-210, s * 58, 352), (228, 8, 8))
        tilt_box(g, RED, (-90, s * 56, 306), (350, s * 56, 226), 6, 70)
        tilt_box(g, W, (-90, s * 60, 306), (350, s * 60, 226), 6, 16)
        tilt_box(g, W, (-90, s * 56, 350), (350, s * 56, 270), 8, 8)
        g.box(STEEL, (338, s * 45, 86), (8, 8, 160))
    g.box(W, (-210, 0, 256), (250, 130, 12))
    g.box(W, (-328, 0, 352), (8, 124, 8))
    tilt_box(g, W, (-90, 0, 256), (350, 0, 176), 110, 12)
    g.box(STEEL, (338, 0, 120), (8, 98, 8))


def net_rack(sc):
    """PROP, TALL (3.7 m), 5.6 x 1.2 m, UNLIT: nets drying on a pole between two trestles, floats along their feet;
    the three nets sway slowly. A basket and an oar beside."""
    g = sc.still()
    g.box(PAVE, (0, 0, -24), (560, 120, 60))
    for x in (-250, 250):
        for s in (-1, 1):
            tilt_box(g, STEEL, (x, s * 50, 6), (x, 0, 338), 9, 9)
    g.box(W, (0, 0, 338), (540, 10, 10))
    r = random.Random(11)
    for i, x in enumerate((-165, 0, 165)):
        n = sc.part(P.motion(swing=5.0, pivot=(x, 0, 333), axis=0.0, rate=0.16, phase=r.random()))
        n.box(TEAL_L if i != 1 else TEAL, (x, 0, 208), (150, 6, 250))
        n.box(W, (x, 0, 322), (150, 8, 10))
        for k in range(4):
            n.box(RED if (k + i) % 2 else W, (x - 56 + k * 37, 0, 86), (16, 14, 14))
    g.cyl(CREAM, (222, -30, 6), 22, 40, steps=8, top_r=27)
    tilt_box(g, W, (-222, -42, 6), (-238, 8, 300), 8, 8)
    g.box(RED, (-239, 11, 332), (8, 22, 70))


# ------------------------------------------------------------------------------------------ the hero
def anchor(sc):
    """THE ANCHOR (hero), 60.0 m: a ship's anchor upright on a stepped plinth 22 m square, 38.6 m across the flukes.
    Red shank, arms and flukes (white bills), a white stock, a SQUARE shackle (no ring, nothing round on top), no
    light anywhere; a length of giant chain hangs from the stock's end and swings slowly (one swing part)."""
    g = sc.still()
    g.box(W, (0, 0, -25), (2200, 2200, 550))                           # three steps, 3 m of footing
    g.box(W, (0, 0, 375), (1700, 1700, 250))
    g.box(TEAL, (0, 0, 625), (1250, 1250, 250))
    g.box(W, (0, 0, 742), (1270, 1270, 24))
    g.box(W, (0, 0, 900), (820, 700, 300))                             # the pedestal
    g.box(RED, (0, 0, 772), (840, 720, 40))
    g.box(BLUE, (0, -352, 900), (400, 8, 200))                         # a blank tablet
    g.box(CREAM, (0, -356, 900), (360, 8, 160))
    for j in range(3):                                                 # steps up the front
        top = 250 + (3 - j) * 62.5
        g.box(W, (0, -905 - j * 110, (250 + top) / 2.0), (420, 110, top - 250))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(W, (sx * 1000, sy * 1000, 400), (110, 110, 300))     # corner posts with red caps
            g.box(RED, (sx * 1000, sy * 1000, 570), (130, 130, 40))
            g.cyl(BLUE, (sx * 760, sy * 760, 500), 40, 110, steps=8, top_r=32)   # bollards on the second step
            g.box(W, (sx * 760, sy * 760, 622), (96, 96, 24))
    sag = ((-1000, 520), (-500, 432), (0, 404), (500, 432), (1000, 520))
    for a, b in zip(sag[:-1], sag[1:]):                                # chains swagged between the posts
        if abs(a[0] + b[0]) > 600:                                     # the front is left open at the steps
            tilt_box(g, STEEL, (a[0], -1000, a[1]), (b[0], -1000, b[1]), 26, 26)
        for sx in (-1, 1):
            tilt_box(g, STEEL, (sx * 1000, a[0], a[1]), (sx * 1000, b[0], b[1]), 26, 26)
    g.box(RED, (0, 0, 1250), (560, 300, 400))                          # the crown
    g.box(RED, (0, 0, 2000), (360, 270, 1200))                         # the shank, plain: no collars
    g.box(RED, (0, 0, 3950), (300, 250, 2900))
    for s in (-1, 1):
        arm = [(0, 1250), (s * 520, 1300), (s * 1000, 1520), (s * 1380, 1900), (s * 1600, 2400)]
        for a, b in zip(arm[:-1], arm[1:]):
            tilt_box(g, RED, (a[0], 0, a[1]), (b[0], 0, b[1]), 260, 260)
        if s > 0:
            g.side(RED, [(1380, 2150), (1930, 2330), (1640, 3050)], -150, 150)        # the fluke
            g.side(W, [(1523, 2645), (1770, 2726), (1640, 3050)], -158, 158)          # its white bill
        else:
            g.side(RED, [(-1930, 2330), (-1380, 2150), (-1640, 3050)], -150, 150)
            g.side(W, [(-1770, 2726), (-1523, 2645), (-1640, 3050)], -158, 158)
        g.box(RED, (s * 1290, 0, 5050), (120, 280, 280))               # the stock's square ends
        g.box(RED, (s * 200, 0, 5640), (110, 200, 520))                # the shackle's cheeks
    g.box(W, (0, 0, 5050), (2500, 230, 230))                           # the stock
    g.box(RED, (0, 0, 5945), (510, 200, 110))                          # the shackle's flat top: 60 m
    g.box(STEEL, (0, 0, 5480), (560, 90, 90))                          # its square pin
    lead = [(255, 5760), (486, 5624), (717, 5488), (948, 5352), (1180, 5215)]
    for i, (a, b) in enumerate(zip(lead[:-1], lead[1:])):              # the chain led from the shackle to the stock's end
        tilt_box(g, STEEL, (a[0], -170, a[1]), (b[0], -170, b[1]), 60 if i % 2 else 150, 150 if i % 2 else 60)
    c = sc.part(P.motion(swing=3.0, pivot=(1180, -170, 5200), axis=90.0, rate=0.07, phase=0.3))
    for k in range(10):                                                # and hanging free: it swings, 14 s there and back
        c.box(STEEL, (1180, -170, 5075 - k * 250), (150, 60, 270) if k % 2 == 0 else (60, 150, 270))


JOBS = [
    ('SM_S1D5_QuayWarehouse', quay_warehouse),
    ('SM_S1D5_SteppedRow', stepped_row, STILL),
    ('SM_S1D5_CustomsHouse', customs_house, STILL),
    ('SM_S1D5_HarbourOffice', harbour_office),
    ('SM_S1D5_TwinSilos', twin_silos, STILL),
    ('SM_S1D5_SeamensMission', seamens_mission, STILL),
    ('SM_S1D5_Chandlery', chandlery, STILL),
    ('SM_S1D5_BollardRow', bollard_row, STILL),
    ('SM_S1D5_FenderStack', fender_stack, STILL),
    ('SM_S1D5_BoatTrestles', boat_trestles, STILL),
    ('SM_S1D5_CrabPots', crab_pots, STILL),
    ('SM_S1D5_KedgeAnchor', kedge_anchor, STILL),
    ('SM_S1D5_QuayLamp', quay_lamp, STILL),
    ('SM_S1D5_SignalMast', signal_mast),
    ('SM_S1D5_Gangway', gangway, STILL),
    ('SM_S1D5_NetRack', net_rack),
    ('SM_S1D5_Anchor', anchor, dict(far=True, grow=(60, 60, 0), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
