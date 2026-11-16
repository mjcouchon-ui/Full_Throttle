"""Nevada, Stage 1, builder s1d2: "Snake Eyes Country and Joshua Flats" (2.43 to 5.14 km).
/Game/World/Nevada/SM_S1D2_*. Run: import ft_world_nevada_s1d2 as K; K.run()

Scrap-and-tin country: half-barrel quonset huts, corrugated lean-tos, a cone-roofed grain-bin house, walls of flattened
car bodies (whole bodies, side on, no wheels), in sage green with bare sheet, purple doors and cream stencils. The
hero is THE TIN RATTLER, a coiled rattlesnake welded from sheet. Everything is an ft_people.Scene: one material
(M_People), one section, movement in the vertex shader only. No vehicles that stand as if on a road, no motorcycles,
no dice, no wrenches, no words.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

SAGE = '#8fb86a'
SAGE_D = '#5f8f4a'
SHEET = '#8f9bb3'
PURP = '#7b3bd6'
CREAM = '#f4efe6'
LAMP = '#ffc86a'
NAVY = '#2b2350'
TYRE = '#26232e'
GREY = '#c9c4d6'
WOOD = '#9a6a44'
CONC = '#c9b8a0'
MAG = '#ff4fb8'
HULK = (SAGE, PURP, SHEET, CREAM, SAGE_D, GREY)


# ---------------------------------------------------------------------------------------------- small shared parts
def ribs(g, col, x0, x1, y, z0, z1, n, t=8.0):
    """Corrugation: n upright ribs on a front wall whose face is at y."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        g.box(col, (x0 + (k + 0.5) * w, y - t / 2.0, (z0 + z1) / 2.0), (max(w * 0.42, 10.0), t, z1 - z0))


def win(g, x, z, y, w=90.0, h=100.0, lit=True):
    """A window on a front wall whose face is at y: a cream frame, a lit or a dark pane."""
    g.box(CREAM, (x, y - 4.0, z), (w + 18.0, 8.0, h + 18.0))
    g.box(LAMP if lit else NAVY, (x, y - 9.0, z), (w, 6.0, h), glow=0.45 if lit else 0.0)
    g.box(CREAM, (x, y - 11.0, z), (5.0, 5.0, h))


def diamond(g, col, x, y, z, s=70.0, t=6.0):
    """A stencil diamond on a front wall whose face is at y."""
    g.box(col, (x, y - t / 2.0, z), (s, t, s), rot=(45, 0, 0))


def door(g, x, y, z=0.0, w=110.0, h=210.0):
    """A purple door with a cream frame and a cream stencil diamond, on a front wall whose face is at y."""
    g.box(CREAM, (x, y - 4.0, z + h / 2.0 + 6.0), (w + 20.0, 8.0, h + 12.0))
    g.box(PURP, (x, y - 8.0, z + h / 2.0), (w, 8.0, h))
    diamond(g, CREAM, x, y - 12.0, z + h * 0.68, s=w * 0.3, t=5.0)
    g.box(LAMP, (x + w * 0.34, y - 13.0, z + h * 0.45), (9.0, 6.0, 9.0))


def hulk(g, x, y, z, k, L=400.0, D=150.0, H=46.0, flip=1):
    """A flattened car body lying side on (no wheels): the slab, the crushed cabin, the window's slit."""
    c = HULK[k % len(HULK)]
    g.box(c, (x, y, z + H * 0.4), (L, D, H * 0.8))
    g.box(c, (x - flip * L * 0.06, y, z + H * 0.88), (L * 0.46, D * 0.9, H * 0.24))
    g.box(NAVY, (x - flip * L * 0.06, y - D / 2.0 - 1.0, z + H * 0.86), (L * 0.36, 6.0, max(H * 0.14, 5.0)))


def tyres(g, x, y, z, n, r=42.0, h=24.0, steps=8):
    for k in range(n):
        g.cyl(TYRE, (x + (6.0 if k % 2 else -4.0), y, z + k * h), r, h - 2.0, steps=steps)
    return z + n * h


def drum(g, col, x, y, z=-20.0, r=30.0, h=110.0):
    g.cyl(col, (x, y, z), r, h, steps=8)
    g.cyl(CREAM, (x, y, z + h * 0.62), r + 2.0, 8.0, steps=8)


def lamp(g, x, y, z):
    """A hooded yard lamp on a wall whose face is at y (steady)."""
    g.box(SHEET, (x, y - 16.0, z + 14.0), (56.0, 34.0, 8.0))
    g.box(LAMP, (x, y - 14.0, z), (36.0, 18.0, 20.0), glow=0.5)


def cactus(g, x, y, z, h=70.0):
    g.cyl(SAGE, (x, y, z), 11.0, h, steps=6)
    g.box(SAGE, (x + 18.0, y, z + h * 0.5), (26.0, 9.0, 9.0))
    g.cyl(SAGE, (x + 28.0, y, z + h * 0.5), 6.0, h * 0.36, steps=6)
    g.box(MAG, (x, y, z + h + 3.0), (9.0, 9.0, 6.0))


def vane(sc, x, y, z, phase=0.0):
    """A weather vane: a welded rattler's arrow that rocks about its mast."""
    g = sc.still()
    g.cyl(SHEET, (x, y, z), 5.0, 120.0, steps=6)
    f = sc.part(P.motion(swing=24, pivot=(x, y, z + 110.0), axis=P.UP, rate=0.22, phase=phase))
    f.box(PURP, (x, y, z + 110.0), (150.0, 6.0, 8.0))
    f.box(CREAM, (x - 74.0, y, z + 110.0), (40.0, 6.0, 40.0), rot=(45, 0, 0))
    f.box(SAGE, (x + 66.0, y, z + 116.0), (36.0, 6.0, 44.0))


# ---------------------------------------------------------------------------------------------- BUILDINGS
def quonset_garage(sc):
    """A half-barrel quonset garage on a block wall, its roller door half up, a lean-to, a hoist frame outside."""
    g = sc.still()
    g.box(SAGE, (-80, 0, 30), (1000, 880, 360))                         # the block wall, 1.5 m of footing
    rod(g, SAGE_D, (-80, -440, 210), (-80, 440, 210), 500, steps=12)       # the barrel
    for y in (-330, -110, 110, 330):
        rod(g, SHEET, (-80, y - 14, 210), (-80, y + 14, 210), 509, steps=12)
    rod(g, SHEET, (-80, -452, 210), (-80, -436, 210), 486, steps=12)       # the end wall, bare sheet
    g.box(SHEET, (-80, -446, 100), (972, 12, 216))
    g.box(NAVY, (-80, -456, 150), (470, 10, 320))                         # the opening
    g.box(PURP, (-80, -462, 252), (470, 10, 116))                         # the roller door, half up
    for z in (216, 252, 288):
        g.box(NAVY, (-80, -468, z), (470, 5, 5))
    g.box(CREAM, (-80, -462, 318), (510, 12, 16))
    diamond(g, CREAM, -80, -454, 520, s=96)
    diamond(g, PURP, -80, -460, 520, s=44)
    for x in (-260, 100):
        g.box(CREAM, (x, -458, 520), (70, 6, 14))
    lamp(g, -80, -456, 380)
    win(g, 250, 150, -452, w=110, h=90)
    win(g, -410, 150, -452, w=110, h=90, lit=False)
    # the lean-to on the left
    g.box(SHEET, (-700, 80, 60), (260, 620, 420))
    tilt_box(g, SAGE_D, (-850, 80, 264), (-560, 80, 284), 680, 16)
    ribs(g, SAGE, -830, -570, -230, -100, 262, 4)
    door(g, -700, -230, z=-10, w=96, h=196)
    g.box(PURP, (-700, -290, 232), (190, 110, 8), rot=(0, 0, -14))        # an awning
    for x in (-786, -614):
        g.box(SHEET, (x, -338, 100), (6, 6, 240))
    # on the roof
    g.cyl(SHEET, (120, 200, 600), 16, 230, steps=6)
    g.cyl(PURP, (120, 200, 830), 30, 24, steps=6, top_r=8)
    g.cyl(SHEET, (-80, -160, 696), 46, 44, steps=8)
    g.cyl(PURP, (-80, -160, 740), 60, 36, steps=8, top_r=10)
    vane(sc, -80, 150, 700, 0.2)
    # the yard in front: tyres, drums, a bench
    tyres(g, -400, -520, -20, 4)
    tyres(g, -470, -500, -20, 2)
    drum(g, PURP, 300, -510)
    drum(g, SHEET, 364, -500)
    g.box(WOOD, (190, -520, 44), (150, 40, 8))
    for x in (130, 250):
        g.box(SHEET, (x, -520, 10), (10, 36, 64))
    # the hoist: an A-frame, a chain, an engine block that sways
    top = (600, -330, 560)
    for a in ((480, -430, -120), (720, -430, -120), (600, -80, -120)):
        tilt_box(g, SHEET, a, top, 16, 16)
    tilt_box(g, SHEET, (520, -400, 200), (680, -400, 200), 10, 10)
    g.box(PURP, top, (70, 70, 40))
    h = sc.part(P.motion(swing=5, pivot=top, axis=90, rate=0.3, phase=0.4))
    h.box(TYRE, (600, -330, 430), (6, 6, 230))
    h.box(CREAM, (600, -330, 310), (26, 16, 26))
    h.box(SHEET, (600, -330, 252), (86, 56, 70))
    h.box(SAGE_D, (600, -330, 296), (60, 40, 20))
    for x in (570, 630):
        h.cyl(GREY, (x, -330, 286), 9, 26, steps=6)


def crusher_shed(sc):
    """The crusher's shed: a mono-pitch tin shed, the press working inside its open bay, a wall of flattened
    hulks beside it and a jib crane with a magnet over the wall."""
    g = sc.still()
    g.box(SHEET, (-330, 60, 150), (560, 600, 600))                        # the shed, 1.5 m of footing
    tilt_box(g, SAGE_D, (-330, -300, 530), (-330, 420, 470), 640, 16)
    g.box(SAGE_D, (-330, -250, 488), (600, 12, 64))
    g.box(SAGE, (-330, -244, 395), (560, 12, 110))
    ribs(g, SAGE_D, -610, -50, -238, 345, 448, 9)
    g.box(NAVY, (-330, -246, 150), (400, 10, 340))                        # the open bay
    g.box(CREAM, (-330, -250, 326), (430, 10, 14))
    for x in (-536, -124):
        g.box(SAGE_D, (x, -250, 150), (14, 14, 340))
    # the press: a purple frame, a platen that rises and falls
    for x in (-470, -190):
        g.box(PURP, (x, -290, 150), (36, 60, 330))
    g.box(PURP, (-330, -290, 322), (320, 70, 44))
    g.box(SHEET, (-330, -290, -10), (300, 90, 60))
    hulk(g, -330, -290, 20, 2, L=250, D=100, H=40)
    pl = sc.part(P.motion(bob=-34, rate=0.16, phase=0.1))
    pl.box(CREAM, (-330, -290, 150), (250, 80, 30))
    pl.box(SHEET, (-330, -290, 232), (40, 40, 140))
    diamond(pl, PURP, -330, -330, 150, s=18, t=5)
    lamp(g, -330, -252, 356)
    win(g, -330, 482, -240, w=120, h=56)
    g.cyl(SHEET, (-520, 250, 440), 18, 230, steps=6)
    g.cyl(PURP, (-520, 250, 670), 34, 22, steps=6, top_r=8)
    # the wall of hulks: two columns, nine deep
    k = 0
    for row in range(9):
        for col in range(2):
            hulk(g, 210 + col * 410 + (14 if row % 2 else -10), -140 + (6 if row % 3 == 1 else 0), -40 + row * 47, k + col + row * 3,
                 flip=(1 if (row + col) % 2 else -1))
            k += 1
    g.box(NAVY, (410, 20, 150), (800, 190, 420))                         # the dark behind the stack
    hulk(g, 330, -150, 386, 4, L=420, flip=-1)
    g.box(SAGE_D, (30, -60, 120), (16, 220, 520))
    g.box(SAGE_D, (840, -60, 120), (16, 220, 520))
    # the jib crane behind the wall
    g.cyl(SHEET, (120, 330, -150), 26, 1080, steps=8)
    g.box(SAGE_D, (120, 330, 420), (90, 90, 70))
    tilt_box(g, SAGE, (40, 380, 900), (640, -200, 1010), 22, 30)
    tilt_box(g, SHEET, (120, 330, 930), (360, 60, 958), 8, 8)
    tilt_box(g, SHEET, (120, 330, 1010), (640, -200, 1020), 6, 6)
    g.box(PURP, (20, 400, 880), (90, 90, 80))
    mg = sc.part(P.motion(swing=4, pivot=(640, -200, 1010), axis=0, rate=0.22, phase=0.6))
    mg.box(TYRE, (640, -200, 870), (6, 6, 280))
    mg.cyl(SHEET, (640, -200, 690), 70, 40, steps=8)
    mg.cyl(PURP, (640, -200, 730), 40, 20, steps=8)
    # the yard: tyres, a drum, a bale
    tyres(g, -700, -200, -20, 3)
    drum(g, PURP, -660, -280)
    g.box(GREY, (-30, -300, 20), (90, 70, 80))
    g.box(SAGE, (-30, -300, 80), (80, 60, 40))


def grain_bin_bar(sc):
    """The roundhouse: a grain bin with a cone hat turned into a bar, a ring porch round it, lamps strung under the eaves."""
    g = sc.still()
    g.cyl(GREY, (0, 0, -150), 640, 170, steps=12)                         # the porch deck, 1.5 m of footing
    g.cyl(SAGE, (0, 0, -150), 400, 720, steps=12)                         # the bin
    for z in (150, 300, 450):
        g.cyl(SHEET, (0, 0, z), 407, 12, steps=12)
    g.cyl(SAGE_D, (0, 0, 570), 450, 250, steps=12, top_r=70)              # the cone hat
    g.cyl(SHEET, (0, 0, 815), 78, 40, steps=8)
    g.cyl(PURP, (0, 0, 855), 96, 50, steps=8, top_r=10)
    g.cyl(PURP, (0, 0, 262), 650, 58, steps=12, top_r=402)                # the ring porch's roof
    g.cyl(CREAM, (0, 0, 254), 654, 9, steps=12)
    for k in range(12):
        a = math.radians(k * 30.0 + 15.0)
        cx, cy = math.cos(a), math.sin(a)
        if k % 2 == 0 and cy < 0.9:
            g.cyl(SHEET, (cx * 612, cy * 612, 20), 9, 236, steps=6)       # posts
        if cy < 0.3:
            g.box(LAMP, (cx * 628, cy * 628, 236), (16, 16, 20), glow=0.5)    # the lamps under the eaves
    for k, deg in enumerate((-150, -120, -60, -30, 20, 160)):
        a = math.radians(deg)
        cx, cy = math.cos(a), math.sin(a)
        g.box(CREAM, (cx * 402, cy * 402, 500), (64, 8, 64), rot=(45, deg - 90, 0))    # stencil diamonds on the band
    # the door (front, -Y), windows either side
    g.box(CREAM, (0, -398, 126), (140, 16, 232))
    g.box(PURP, (0, -404, 120), (116, 12, 216))
    diamond(g, CREAM, 0, -410, 166, s=36, t=5)
    g.box(PURP, (0, -470, 240), (220, 130, 8), rot=(0, 0, -10))
    for deg in (-125, -55):
        a = math.radians(deg)
        cx, cy = math.cos(a), math.sin(a)
        g.box(CREAM, (cx * 398, cy * 398, 150), (118, 12, 118), rot=(0, deg - 90, 0))
        g.box(LAMP, (cx * 404, cy * 404, 150), (92, 10, 92), rot=(0, deg - 90, 0), glow=0.45)
    # barrel tables on the porch, stools, a drum stove's pipe
    for (x, y) in ((-330, -400), (300, -430), (500, -200)):
        g.cyl(SHEET, (x, y, 20), 34, 86, steps=8)
        g.cyl(CREAM, (x, y, 106), 52, 8, steps=8)
        g.cyl(PURP, (x + 80, y - 10, 20), 17, 46, steps=6)
        g.cyl(PURP, (x - 76, y + 24, 20), 17, 46, steps=6)
    tilt_box(g, SHEET, (300, 280, 380), (430, 400, 520), 22, 22)
    g.cyl(SHEET, (430, 400, 500), 16, 380, steps=6)
    g.cyl(PURP, (430, 400, 880), 30, 22, steps=6, top_r=8)
    tilt_box(g, SHEET, (-470, 120, 20), (-330, 80, 800), 8, 50)           # a ladder to the hat
    for z in (120, 260, 400, 540, 680):
        g.box(CREAM, (-450 + (z - 20) * 0.18, 114 - (z - 20) * 0.05, z), (10, 56, 8))
    # a bunting of sheet pennants between the porch and a mast: they swing
    g.cyl(SHEET, (-700, -380, -100), 9, 560, steps=6)
    tilt_box(g, TYRE, (-700, -380, 450), (-430, -470, 300), 5, 5)
    for k in range(4):
        f = sc.part(P.motion(swing=16, pivot=(-664 + k * 62, -392 - k * 21, 428 - k * 35), axis=18, rate=0.4, phase=k * 0.23))
        f.box((PURP, CREAM, SAGE, CREAM)[k], (-664 + k * 62, -392 - k * 21, 404 - k * 35), (40, 5, 46))
    vane(sc, 0, 0, 890, 0.5)


def tyre_workshop(sc):
    """The tyre-wall workshop: a ground floor walled with stacked tyres, a tin storey over it, a stove pipe, a tyre swing."""
    g = sc.still()
    g.box(NAVY, (0, 30, 60), (820, 560, 420))                             # the dark inside, 1.5 m of footing
    for k in range(10):
        x = -405 + k * 90
        if k in (4, 5):
            continue
        g.cyl(TYRE, (x, -260, -150), 47, 420, steps=8)                   # a column of tyres
        for z in (-60, 60, 180):
            g.cyl(GREY, (x, -260, z), 26, 6, steps=6)
    for side in (-1, 1):
        for k in range(6):
            g.cyl(TYRE, (side * 410, -200 + k * 96, -150), 47, 420, steps=8)
    g.box(SAGE_D, (0, 30, 286), (900, 640, 32))                           # the wall plate
    g.box(PURP, (-46, -268, 110), (88, 10, 220))                          # double doors between the columns
    g.box(PURP, (46, -268, 110), (88, 10, 220))
    g.box(CREAM, (0, -272, 226), (200, 10, 14))
    diamond(g, CREAM, -46, -274, 150, s=30, t=5)
    diamond(g, CREAM, 46, -274, 150, s=30, t=5)
    # the tin storey
    g.box(SAGE, (0, 50, 420), (840, 560, 240))
    ribs(g, SAGE_D, -420, 420, -230, 304, 540, 14)
    tilt_box(g, SAGE_D, (0, -300, 566), (0, 390, 536), 920, 16)
    g.box(SHEET, (0, -304, 552), (924, 10, 22))
    for x in (-270, 0, 270):
        win(g, x, 420, -236, w=96, h=90, lit=(x != 0))
    g.box(CREAM, (0, -240, 330), (300, 8, 30))                            # a stencil board: three diamonds, no words
    for x in (-90, 0, 90):
        diamond(g, PURP, x, -244, 330, s=18, t=5)
    lamp(g, -160, -262, 300)
    g.box(PURP, (250, -330, 270), (300, 140, 8), rot=(0, 0, -12))         # an awning on posts
    for x in (110, 390):
        g.box(SHEET, (x, -392, 120), (6, 6, 290))
    g.cyl(SHEET, (300, 200, 520), 18, 260, steps=6)
    g.cyl(PURP, (300, 200, 780), 34, 22, steps=6, top_r=8)
    g.cyl(SHEET, (-250, 120, 540), 60, 110, steps=8)                      # a roof tank
    g.cyl(SAGE_D, (-250, 120, 650), 66, 30, steps=8, top_r=12)
    # the tyre swing on its beam
    g.box(SHEET, (-520, -330, 130), (10, 10, 500))
    tilt_box(g, SHEET, (-520, -330, 370), (-360, -330, 380), 10, 10)
    g.box(SHEET, (-600, -330, 374), (170, 10, 10))
    sw = sc.part(P.motion(swing=11, pivot=(-640, -330, 370), axis=0, rate=0.27, phase=0.15))
    sw.box(CREAM, (-640, -330, 260), (5, 5, 220))
    rod(sw, TYRE, (-640, -344, 120), (-640, -316, 120), 44, steps=8)
    # the yard: loose tyres, a rim rack, drums
    tyres(g, 470, -360, -20, 5)
    tyres(g, 560, -330, -20, 3)
    g.box(SHEET, (-250, -330, 60), (190, 40, 160))
    for k in range(3):
        rod(g, (CREAM, GREY, PURP)[k], (-310 + k * 60, -356, 80), (-310 + k * 60, -348, 80), 24, steps=8)
    drum(g, PURP, -420, -350)


def lineman_house(sc):
    """The lineman's substation house: a two-storey tin house, a transformer yard behind a fence, a gantry of insulators."""
    g = sc.still()
    g.box(SAGE, (-330, 40, 220), (480, 480, 740))                         # the house, 1.5 m of footing
    ribs(g, SAGE_D, -570, -90, -200, 0, 590, 8)
    g.box(SHEET, (-330, -204, 296), (492, 10, 14))
    tilt_box(g, SAGE_D, (-330, -260, 618), (-330, 330, 596), 560, 16)
    g.box(SHEET, (-330, -264, 606), (564, 10, 20))
    door(g, -420, -206, z=0, w=100, h=200)
    win(g, -230, 130, -206, w=100, h=100)
    win(g, -420, 440, -206, w=90, h=90)
    win(g, -230, 440, -206, w=90, h=90, lit=False)
    g.box(PURP, (-420, -270, 236), (190, 120, 8), rot=(0, 0, -12))
    g.box(GREY, (-420, -270, -40), (200, 130, 80))                        # the step
    lamp(g, -330, -206, 330)
    g.cyl(SHEET, (-460, 180, 590), 16, 200, steps=6)
    g.cyl(PURP, (-460, 180, 790), 30, 20, steps=6, top_r=8)
    vane(sc, -230, 60, 610, 0.7)
    # the cable drum by the door
    rod(g, CREAM, (-640, -330, 70), (-640, -318, 70), 90, steps=10)
    rod(g, SHEET, (-640, -318, 70), (-640, -262, 70), 50, steps=8)
    rod(g, CREAM, (-640, -262, 70), (-640, -250, 70), 90, steps=10)
    # the transformer yard: a slab, two transformers, a gantry
    g.box(CONC, (330, 40, -60), (640, 460, 140))
    for i, x in enumerate((180, 470)):
        g.box(SHEET, (x, 60, 120), (170, 130, 220))
        for k in range(5):
            g.box(GREY, (x - 60 + k * 30, -14, 116), (10, 20, 170))       # cooling fins
        g.box(PURP, (x, 60, 238), (184, 144, 18))
        for dx in (-50, 0, 50):
            g.cyl(CREAM, (x + dx, 60, 246), 13, 70, steps=6)
            g.cyl(CREAM, (x + dx, 60, 268), 22, 9, steps=6)
            g.cyl(CREAM, (x + dx, 60, 292), 22, 9, steps=6)
        diamond(g, CREAM, x, -7, 190, s=44, t=5)
    for x in (60, 600):
        g.cyl(WOOD, (x, 180, -150), 15, 1000, steps=6)                   # the gantry's poles
    g.box(WOOD, (330, 180, 790), (640, 14, 16))
    g.box(WOOD, (330, 180, 640), (600, 12, 14))
    for x in (120, 330, 540):
        for z in (806, 828, 850):
            g.cyl(CREAM, (x, 180, z), 20 - (z - 806) * 0.2, 12, steps=6)
        g.cyl(PURP, (x, 180, 868), 9, 16, steps=6)
        tilt_box(g, TYRE, (x, 180, 800), (x - 20, 70, 330), 5, 5)         # the drops to the transformers
    tilt_box(g, TYRE, (60, 180, 846), (-330, 100, 640), 5, 5)             # the service wire to the house
    g.box(LAMP, (600, 160, 600), (30, 30, 34), glow=0.5)
    g.box(SHEET, (600, 164, 624), (50, 46, 8))
    # the fence: posts, two rails, a purple gate
    for k in range(8):
        g.box(SHEET, (20 + k * 88, -200, 60), (8, 8, 250))
    for z in (60, 150):
        g.box(SHEET, (330, -200, z), (630, 5, 6))
    g.box(PURP, (330, -206, 100), (84, 6, 150))
    diamond(g, CREAM, 330, -210, 120, s=40, t=5)
    for x in (154, 506):
        g.box(CREAM, (x, -206, 120), (50, 6, 50), rot=(45, 0, 0))
    cactus(g, 60, -280, -20, h=90)


def tank_house(sc):
    """The tank house: a water tank on four splayed legs turned into a house, a porch all round, a ladder, washing out."""
    g = sc.still()
    Z = 470.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, SHEET, (sx * 390, sy * 390, -200), (sx * 270, sy * 270, Z), 26, 26)
    for sx in (-1, 1):                                                    # the bracing, front and sides
        tilt_box(g, SHEET, (sx * 370, -370, -40), (-sx * 300, -300, 380), 10, 10)
        tilt_box(g, SHEET, (sx * 360, -360, 0), (sx * 300, 300, 380), 10, 10)
    g.box(SHEET, (0, -326, 200), (660, 10, 12))
    g.cyl(WOOD, (0, 0, Z), 430, 22, steps=12)                             # the deck
    g.cyl(SAGE_D, (0, 0, Z - 14), 300, 14, steps=12)
    g.cyl(SAGE, (0, 0, Z + 22), 300, 400, steps=12)                       # the tank
    for z in (Z + 110, Z + 300):
        g.cyl(SHEET, (0, 0, z), 306, 12, steps=12)
    g.cyl(SAGE_D, (0, 0, Z + 422), 345, 190, steps=12, top_r=40)          # the hat
    g.cyl(PURP, (0, 0, Z + 612), 52, 40, steps=8, top_r=10)
    vane(sc, 0, 0, Z + 640, 0.3)
    # the door and the round windows
    g.box(CREAM, (0, -298, Z + 126), (116, 12, 212))
    g.box(PURP, (0, -304, Z + 120), (94, 10, 196))
    diamond(g, CREAM, 0, -310, Z + 160, s=30, t=5)
    for deg in (-130, -50, 10, 170):
        a = math.radians(deg)
        cx, cy = math.cos(a), math.sin(a)
        rod(g, CREAM, (cx * 290, cy * 290, Z + 220), (cx * 306, cy * 306, Z + 220), 54, steps=8)
        rod(g, LAMP, (cx * 300, cy * 300, Z + 220), (cx * 311, cy * 311, Z + 220), 40, steps=8, glow=0.45)
    g.box(LAMP, (0, -316, Z + 340), (30, 16, 24), glow=0.5)
    g.box(SHEET, (0, -320, Z + 356), (50, 30, 8))
    # the porch rail: posts and rails round the deck
    n = 10
    pts = [(math.cos(math.radians(k * 36.0 + 18.0)) * 412, math.sin(math.radians(k * 36.0 + 18.0)) * 412) for k in range(n)]
    for k in range(n):
        a, b = pts[k], pts[(k + 1) % n]
        g.box(SHEET, (a[0], a[1], Z + 70), (8, 8, 100))
        if not (a[1] < -300 and b[1] < -300):                             # the gap at the ladder's head
            tilt_box(g, CREAM, (a[0], a[1], Z + 116), (b[0], b[1], Z + 116), 8, 8)
            tilt_box(g, SHEET, (a[0], a[1], Z + 66), (b[0], b[1], Z + 66), 5, 5)
    # the ladder
    for dx in (-36, 36):
        tilt_box(g, SHEET, (dx, -640, -150), (dx, -420, Z + 20), 8, 8)
    for k in range(8):
        f = (k + 0.5) / 8.0
        g.box(CREAM, (0, -640 + 220 * f, -150 + (Z + 170) * f), (80, 8, 8))
    # a flower box, a stove pipe, a down pipe
    g.box(PURP, (150, -296, Z + 86), (110, 30, 26))
    for dx in (-36, 0, 36):
        g.box(MAG, (150 + dx, -300, Z + 106), (16, 14, 14))
    g.cyl(SHEET, (180, 150, Z + 500), 14, 200, steps=6)
    g.cyl(PURP, (180, 150, Z + 700), 28, 20, steps=6, top_r=8)
    g.cyl(SHEET, (296, 60, -150), 12, Z + 150, steps=6)
    drum(g, PURP, 330, 10, z=-40)
    # the washing line from the deck to a post: it swings
    g.cyl(WOOD, (760, -150, -150), 11, 800, steps=6)
    tilt_box(g, TYRE, (400, -90, Z + 116), (760, -150, 640), 5, 5)
    for k in range(4):
        f = (k + 0.7) / 4.6
        px, py, pz = 400 + 360 * f, -90 - 60 * f, Z + 116 + (640 - Z - 116) * f
        w = sc.part(P.motion(swing=13, pivot=(px, py, pz), axis=-9.5, rate=0.34, phase=k * 0.21))
        w.box((CREAM, PURP, SHEET, CREAM)[k], (px, py, pz - 36), ((50, 62, 44, 70)[k], 5, (64, 54, 70, 46)[k]))
    tyres(g, -330, -470, -20, 3)


def car_wall_house(sc):
    """The house of flattened cars: its front wall is two columns of whole bodies laid side on; a tin penthouse with a
    barrel roof sits on top behind a rail of bonnets."""
    g = sc.still()
    g.box(NAVY, (0, 60, 150), (800, 520, 600))                            # the body behind the wall, 1.5 m of footing
    g.box(SAGE_D, (-405, 60, 150), (14, 540, 600))
    g.box(SAGE_D, (405, 60, 150), (14, 540, 600))
    k = 0
    for row in range(9):
        for col in range(2):
            hulk(g, -200 + col * 400 + (12 if row % 2 else -8), -210 + (5 if row % 3 == 2 else 0), -30 + row * 50, k * 5 + col * 2 + row,
                 L=392, D=140, H=49, flip=(1 if (row + col) % 2 else -1))
            k += 1
    g.box(SAGE, (0, 60, 436), (860, 580, 30))                             # the roof slab
    # the door and the windows cut into the wall
    g.box(CREAM, (0, -284, 110), (150, 10, 236))
    g.box(PURP, (0, -290, 104), (124, 10, 220))
    diamond(g, CREAM, 0, -296, 150, s=40, t=5)
    g.box(PURP, (0, -350, 250), (250, 140, 8), rot=(0, 0, -12))
    for x in (-112, 112):
        g.box(SHEET, (x, -412, 100), (6, 6, 280))
    for x in (-250, 250):
        win(g, x, 300, -282, w=110, h=80)
    lamp(g, 0, -284, 262)
    # the penthouse: tin walls, a barrel roof, a lit window
    g.box(SAGE, (60, 120, 560), (480, 360, 220))
    ribs(g, SAGE_D, -180, 300, -60, 452, 668, 8)
    rod(g, SAGE_D, (-200, 120, 668), (320, 120, 668), 190, steps=10)
    for x in (-150, 60, 270):
        rod(g, SHEET, (x - 10, 120, 668), (x + 10, 120, 668), 197, steps=10)
    win(g, -60, 570, -60, w=90, h=90)
    g.box(PURP, (170, -70, 548), (84, 10, 190))
    g.box(CREAM, (170, -76, 590), (30, 5, 30), rot=(45, 0, 0))
    # the rail of bonnets along the terrace
    for i, x in enumerate((-370, -250, -130, 330)):
        g.box((SHEET, PURP, CREAM, SAGE_D)[i], (x, -250, 498), (104, 10, 84))
        g.box(NAVY, (x, -256, 520), (70, 5, 5))
    g.box(SHEET, (-20, -250, 544), (820, 8, 8))
    # on the terrace: a tub, a deck chair, an aerial; a stove pipe
    g.cyl(SHEET, (-280, -100, 451), 50, 46, steps=8)
    g.cyl(SAGE, (-280, -100, 497), 12, 60, steps=6)
    g.box(MAG, (-280, -100, 560), (12, 12, 8))
    tilt_box(g, CREAM, (-330, 60, 470), (-250, 60, 540), 60, 6)
    g.cyl(SHEET, (330, 250, 451), 6, 400, steps=6)
    for z in (760, 800, 840):
        g.box(SHEET, (330, 250, z), (150 - (z - 760) * 1.0, 5, 5))
    g.cyl(SHEET, (-330, 260, 451), 16, 280, steps=6)
    g.cyl(PURP, (-330, 260, 731), 30, 20, steps=6, top_r=8)
    vane(sc, 60, 120, 856, 0.9)
    # beside the house: a lean-to carport of sheet with hubcaps on its post, drums
    g.box(SHEET, (600, 100, 100), (10, 10, 500))
    g.box(SHEET, (600, -220, 80), (10, 10, 460))
    tilt_box(g, SAGE_D, (400, -60, 360), (640, -60, 300), 420, 12)
    for z in (120, 190, 260):
        rod(g, (CREAM, GREY, PURP)[(z // 70) % 3], (600, -232, z), (600, -224, z), 26, steps=8)
    drum(g, PURP, 520, 40)
    drum(g, SHEET, 520, 110)
    tyres(g, -520, -260, -20, 4)
    cactus(g, -500, -360, -20, h=100)


# ---------------------------------------------------------------------------------------------- PROPS
def hubcap_fence(sc):
    """LOW: a fence of pipe hung with hubcaps."""
    g = sc.still()
    for k in range(5):
        g.box(SHEET, (-400 + k * 200, 0, 20), (10, 10, 200))
    for z in (50, 108):
        g.box(SHEET, (0, 0, z), (810, 6, 6))
    cols = (CREAM, GREY, PURP, CREAM, SHEET, CREAM, PURP, GREY)
    for k in range(8):
        x = -350 + k * 100
        z = 80 + (10 if k % 3 == 0 else (-8 if k % 3 == 1 else 0))
        r = 30 if k % 2 else 25
        rod(g, cols[k], (x, -9, z), (x, -3, z), r, steps=8)
        rod(g, NAVY if cols[k] != NAVY else GREY, (x, -13, z), (x, -8, z), r * 0.34, steps=6)
    g.box(SAGE, (-400, 0, 126), (30, 14, 14))
    g.box(SAGE, (400, 0, 126), (30, 14, 14))


def drum_planters(sc):
    """LOW: oil drums cut down for planters, purple and bare sheet, cactus in them, a plank bench between."""
    g = sc.still()
    for i, x in enumerate((-230, -150, 170, 250)):
        y = (-14, 20, 10, -18)[i]
        h = (84, 64, 70, 88)[i]
        g.cyl((PURP, SHEET, PURP, SHEET)[i], (x, y, -70), 32, 70 + h, steps=8)
        g.cyl(CREAM, (x, y, h * 0.5), 34, 7, steps=8)
        cactus(g, x, y, h, h=(62, 46, 70, 40)[i])
    g.box(WOOD, (10, 0, 48), (240, 44, 8))
    for x in (-90, 110):
        g.cyl(TYRE, (x, 0, -70), 30, 114, steps=8)
    g.box(CREAM, (10, -24, 30), (60, 5, 26), rot=(45, 0, 0))


def mail_snake(sc):
    """LOW: a row of mail boxes on a rail welded as a little rattler."""
    g = sc.still()
    pts = [(-330, 104), (-220, 124), (-110, 100), (0, 122), (110, 100), (220, 122), (330, 106)]
    for k in range(6):
        a, b = pts[k], pts[k + 1]
        tilt_box(g, SAGE if k % 2 else SAGE_D, (a[0], 0, a[1]), (b[0], 0, b[1]), 22, 20)
        g.box(PURP, ((a[0] + b[0]) / 2.0, -12, (a[1] + b[1]) / 2.0), (22, 5, 22), rot=(45, 0, 0))
    for x in (-275, -55, 165):
        g.box(SHEET, (x, 0, 30), (10, 10, 160))
    g.box(SAGE, (372, 0, 116), (70, 34, 26))                              # the head
    g.box(CREAM, (376, 0, 100), (58, 28, 8))
    g.box(LAMP, (388, -18, 122), (8, 5, 8))
    tilt_box(g, CREAM, (-330, 0, 104), (-372, 0, 150), 12, 12)            # the rattle
    tilt_box(g, CREAM, (-372, 0, 150), (-380, 0, 176), 8, 8)
    for i, x in enumerate((-220, -110, 0, 110, 220)):
        z = pts[i + 1][1] + 12
        c = (SHEET, PURP, CREAM, SHEET, PURP)[i]
        g.box(c, (x, 0, z + 13), (40, 56, 26))
        rod(g, c, (x, -28, z + 26), (x, 28, z + 26), 20, steps=6)
        g.box(NAVY, (x, -30, z + 20), (30, 5, 30))
        g.box(CREAM if c != CREAM else PURP, (x + 22, -8, z + 40), (5, 16, 12))


def car_stack(sc):
    """TALL: three flattened car bodies stacked on sleepers, a bonnet leant against them."""
    g = sc.still()
    for x in (-130, 130):
        g.box(WOOD, (x, 0, -40), (30, 190, 110))
    hulk(g, 0, 0, 14, 0, L=430, D=170, H=76)
    hulk(g, 16, 6, 92, 1, L=420, D=166, H=74, flip=-1)
    hulk(g, -12, -4, 168, 2, L=426, D=168, H=72)
    for i, z in enumerate((30, 108, 184)):
        diamond(g, CREAM if i != 1 else SAGE, -150 + i * 20, -86, z + 16, s=30, t=5)
        g.box(NAVY, (150, -86, z + 10), (40, 5, 22))
    tilt_box(g, SAGE, (250, -150, -20), (236, -96, 150), 120, 8)
    g.box(CREAM, (243, -128, 70), (60, 5, 5))
    tyres(g, -270, -40, -20, 3, r=38)
    g.box(SHEET, (0, 0, 250), (60, 60, 8))
    g.cyl(PURP, (0, 0, 254), 16, 40, steps=6)


def whirligig(sc):
    """TALL: a scrap whirligig totem, three wheels of vanes that rock, a tail fin, hubcaps down the pole."""
    g = sc.still()
    g.cyl(TYRE, (0, 0, -60), 50, 84, steps=8)
    g.cyl(PURP, (0, 0, 20), 32, 100, steps=8)
    g.cyl(SHEET, (0, 0, -60), 10, 760, steps=6)
    for i, z in enumerate((170, 240, 310)):
        rod(g, (CREAM, GREY, CREAM)[i], (0, -16, z), (0, -10, z), 30 - i * 3, steps=8)
    g.box(SAGE, (0, -12, 430), (70, 6, 100))                              # a bonnet shield
    diamond(g, PURP, 0, -15, 430, s=36, t=5)
    tilt_box(g, SHEET, (0, 0, 480), (-150, 0, 540), 8, 8)
    tilt_box(g, SHEET, (0, 0, 380), (140, 0, 430), 8, 8)
    g.box(PURP, (0, 80, 690), (8, 150, 60))                               # the tail fin
    g.box(CREAM, (0, 150, 690), (8, 40, 40), rot=(0, 0, 0))
    hubs = (((0, -22, 690), 120, 8, 0.25, 34), ((-150, -14, 540), 70, 6, 0.4, -40), ((140, -14, 430), 60, 6, 0.33, 44))
    for i, (c, r, n, rate, sw) in enumerate(hubs):
        w = sc.part(P.motion(swing=sw, pivot=c, axis=90, rate=rate, phase=i * 0.31))
        rod(w, CREAM, (c[0], c[1] - 5, c[2]), (c[0], c[1] + 5, c[2]), r * 0.2, steps=6)
        for k in range(n):
            a = math.radians(k * 360.0 / n)
            cx, cz = math.cos(a), math.sin(a)
            w.box((PURP, CREAM, SAGE, SHEET)[k % 4], (c[0] + cx * r * 0.62, c[1], c[2] + cz * r * 0.62), (r * 0.7, 5, r * 0.3),
                  rot=(math.degrees(a), 0, 0))


def tyre_tower(sc):
    """TALL: a tower of tyres with a bottle tree growing out of it, the bottles catching the light."""
    g = sc.still()
    z = -60.0
    for k in range(9):
        r = 64 - k * 3.2
        g.cyl(TYRE, ((5 if k % 2 else -5), (4 if k % 3 == 0 else -3), z), r, 30, steps=8)
        if k % 3 == 1:
            g.cyl(GREY, (0, 0, z + 12), r + 2, 5, steps=8)
        z += 32
    g.cyl(SHEET, (0, 0, z - 40), 9, 330, steps=6)
    cols = (PURP, CREAM, LAMP, PURP, SAGE, LAMP, CREAM, PURP)
    for half in range(2):
        b = sc.part(P.motion(swing=3, pivot=(0, 0, z), axis=40 + half * 90, rate=0.3, phase=half * 0.4))
        for k in range(4):
            i = half * 4 + k
            a = math.radians(i * 45.0 + 10.0)
            h0 = z + 60 + (i % 4) * 56
            tip = (math.cos(a) * (120 - (i % 4) * 14), math.sin(a) * (120 - (i % 4) * 14), h0 + 66)
            tilt_box(b, SHEET, (0, 0, h0), tip, 6, 6)
            b.cyl(cols[i], (tip[0], tip[1], tip[2] - 4), 10, 30, steps=6, glow=0.3 if cols[i] == LAMP else 0.0)
            b.cyl(cols[i], (tip[0], tip[1], tip[2] + 26), 5, 14, steps=6)
    g.cyl(PURP, (0, 0, z + 290), 10, 30, steps=6)
    g.cyl(PURP, (0, 0, z + 320), 5, 14, steps=6)
    g.cyl(TYRE, (110, -40, -60), 44, 80, steps=8)
    g.cyl(SAGE, (110, -40, 20), 12, 60, steps=6)
    g.box(MAG, (110, -40, 84), (10, 10, 8))


def wash_line(sc):
    """TALL: a washing line between two pipe posts, shirts and sheets swinging, a tub and a basket under it."""
    g = sc.still()
    for x in (-300, 300):
        g.cyl(SHEET, (x, 0, -80), 9, 380, steps=6)
        g.box(SHEET, (x, 0, 292), (10, 90, 8))
    for y in (-36, 36):
        g.box(TYRE, (0, y, 290), (600, 5, 5))
    items = ((-230, -36, CREAM, 70, 80), (-140, -36, PURP, 54, 64), (-40, -36, SHEET, 90, 110), (70, -36, CREAM, 50, 60), (170, -36, SAGE, 64, 74),
             (-180, 36, SAGE_D, 110, 96), (-30, 36, CREAM, 120, 100), (130, 36, PURP, 60, 70), (230, 36, CREAM, 46, 86))
    for i, (x, y, c, w, h) in enumerate(items):
        p = sc.part(P.motion(swing=12 + (i % 3) * 3, pivot=(x, y, 290), axis=0, rate=0.3 + (i % 2) * 0.06, phase=i * 0.17))
        p.box(c, (x, y, 286 - h / 2.0), (w, 5, h))
        if i in (1, 4, 7):                                                # shirts have sleeves
            p.box(c, (x, y, 268), (w + 40, 5, 20))
        if i == 2:
            p.box(PURP, (x, y - 1, 286 - h * 0.5), (26, 6, 26), rot=(45, 0, 0))
    g.cyl(SHEET, (-80, -110, -40), 44, 76, steps=8)
    g.cyl(CREAM, (-80, -110, 30), 38, 8, steps=8)
    g.box(WOOD, (120, -100, 0), (70, 46, 60))
    g.box(CREAM, (120, -100, 36), (56, 34, 14))
    cactus(g, 360, 40, -40, h=90)


def scrap_sign(sc):
    """TALL: a roadside board made of a car's bonnet and doors: a cream arrow and diamonds (no words), a hooded lamp,
    a small board swinging under it."""
    g = sc.still()
    for x in (-130, 130):
        g.cyl(SHEET, (x, 0, -100), 11, 700, steps=6)
        rod(g, CREAM, (x, -14, 180), (x, -9, 180), 26, steps=8)
    g.box(SAGE, (0, -6, 440), (340, 12, 210))
    g.box(PURP, (0, -8, 552), (360, 16, 16))
    g.box(PURP, (0, -8, 328), (360, 16, 16))
    g.box(CREAM, (-30, -14, 440), (170, 5, 34))                           # the arrow: a shaft and a head
    g.box(CREAM, (84, -14, 440), (74, 5, 74), rot=(45, 0, 0))
    g.box(SAGE, (40, -16, 440), (56, 5, 110))
    g.box(CREAM, (56, -18, 440), (40, 5, 34))
    for x in (-130, -90, -50):
        diamond(g, PURP, x, -12, 510, s=22, t=5)
    for x in (-130, 0, 130):
        diamond(g, CREAM, x, -12, 368, s=20, t=5)
    g.box(SHEET, (0, -40, 590), (220, 70, 8))
    g.box(LAMP, (0, -34, 574), (150, 20, 16), glow=0.5)
    tilt_box(g, SHEET, (-130, 0, 600), (130, 0, 600), 8, 8)
    s = sc.part(P.motion(swing=8, pivot=(0, 0, 320), axis=0, rate=0.3, phase=0.2))
    for x in (-60, 60):
        s.box(TYRE, (x, -6, 296), (5, 5, 50))
    s.box(CREAM, (0, -6, 238), (170, 8, 70))
    s.box(PURP, (0, -11, 238), (34, 5, 34), rot=(45, 0, 0))
    for x in (-56, 56):
        s.box(SAGE, (x, -11, 238), (22, 5, 22), rot=(45, 0, 0))
    drum(g, PURP, 190, -20, z=-60, h=120)
    tyres(g, -200, -20, -60, 3, r=40)


def insulator_stack(sc):
    """TALL: a line pole with a crossarm of insulators and a pole transformer, a lamp, spare insulators stacked at its foot."""
    g = sc.still()
    g.cyl(WOOD, (0, 0, -150), 16, 1010, steps=6)
    g.box(WOOD, (0, 0, 800), (360, 14, 16))
    g.box(WOOD, (0, 0, 690), (260, 12, 14))
    tilt_box(g, WOOD, (0, -8, 700), (140, -8, 794), 8, 8)
    tilt_box(g, WOOD, (0, -8, 700), (-140, -8, 794), 8, 8)
    for x in (-160, -60, 60, 160):
        for k in range(3):
            g.cyl(CREAM, (x, 0, 808 + k * 20), 20 - k * 4, 12, steps=6)
        g.cyl(PURP, (x, 0, 868), 8, 14, steps=6)
    for x in (-110, 110):
        g.cyl(CREAM, (x, 0, 698), 16, 12, steps=6)
        g.cyl(CREAM, (x, 0, 716), 11, 10, steps=6)
    g.cyl(SHEET, (0, -62, 500), 40, 120, steps=8)                         # the pole transformer
    g.cyl(PURP, (0, -62, 580), 43, 12, steps=8)
    g.cyl(SAGE_D, (0, -62, 620), 43, 8, steps=8, top_r=30)
    for dx in (-18, 18):
        g.cyl(CREAM, (dx, -62, 628), 8, 40, steps=6)
        g.cyl(CREAM, (dx, -62, 646), 14, 8, steps=6)
        tilt_box(g, TYRE, (dx, -62, 668), (dx * 3.2, 0, 700), 5, 5)
    g.box(SHEET, (0, -28, 540), (14, 40, 8))
    diamond(g, CREAM, 0, -103, 540, s=30, t=5)
    tilt_box(g, SHEET, (0, 0, 420), (110, -40, 440), 8, 8)                # the lamp's arm
    g.box(SHEET, (130, -46, 444), (70, 50, 8))
    g.box(LAMP, (130, -46, 428), (44, 30, 20), glow=0.5)
    for k in range(5):
        g.box(CREAM, (0, -20, 60 + k * 62), (5, 8, 5))                    # the climbing pegs
        g.box(SHEET, ((26 if k % 2 else -26), 0, 90 + k * 62), (30, 6, 6))
    # the stays' short stubs (the level's own phone line carries the long wires)
    tilt_box(g, TYRE, (-160, 0, 812), (-200, 0, 790), 5, 5)
    tilt_box(g, TYRE, (160, 0, 812), (200, 0, 790), 5, 5)
    # the spare insulators on a pallet, a coil of cable
    g.box(WOOD, (170, 30, -30), (170, 130, 70))
    for i, (x, y) in enumerate(((120, 0), (200, 10), (160, 70))):
        for k in range(4):
            g.cyl(CREAM, (x, y, 5 + k * 24), 30 - k * 4, 16, steps=8)
        g.cyl(PURP, (x, y, 101), 10, 18, steps=6)
    for k in range(3):
        g.cyl(TYRE, (-160, 20, -50 + k * 22), 54 - k * 4, 20, steps=8)
    g.cyl(GREY, (-160, 20, 16), 26, 8, steps=8)


# ---------------------------------------------------------------------------------------------- THE HERO
def tin_rattler(sc):
    """THE TIN RATTLER: a coiled rattlesnake 27 m across and 24 m to the brow, welded from sheet and car bonnets: sage and
    steel plates, purple diamonds down its back, a cream belly. The head, reared over the coil, turns slowly from side
    to side; the rattle rocks. It stands on a round welded deck ringed with flattened hulks."""
    g = sc.still()
    g.cyl(SHEET, (0, 0, -300), 1480, 320, steps=14)                       # the deck, 3 m of footing
    g.cyl(PURP, (0, 0, 14), 1500, 12, steps=14)
    for k in range(7):                                                    # hulks leant round the front of the deck
        a = math.radians(-90.0 + (k - 3) * 22.0)
        cx, cy = math.cos(a), math.sin(a)
        c = HULK[k % len(HULK)]
        g.box(c, (cx * 1530, cy * 1530, -60), (400, 46, 150), rot=(0, math.degrees(a) + 90.0, 0))
        g.box(NAVY, (cx * 1556, cy * 1556, -30), (150, 6, 40), rot=(0, math.degrees(a) + 90.0, 0))

    plates = (SAGE, SHEET, SAGE_D, SAGE, GREY, SAGE_D)

    def seg(m, a, b, w, d, k, belly=None, mark='diamond'):
        dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
        yaw = math.degrees(math.atan2(dy, dx))
        pitch = math.degrees(math.atan2(dz, math.hypot(dx, dy)))
        L = math.sqrt(dx * dx + dy * dy + dz * dz)
        e = 0.12                                                          # the plates overlap like scales
        a2 = (a[0] - dx * e, a[1] - dy * e, a[2] - dz * e)
        b2 = (b[0] + dx * e, b[1] + dy * e, b[2] + dz * e)
        tilt_box(m, plates[k % len(plates)], a2, b2, w, d)
        mid = ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0)
        sp, cp = math.sin(math.radians(pitch)), math.cos(math.radians(pitch))
        n = (-sp * math.cos(math.radians(yaw)), -sp * math.sin(math.radians(yaw)), cp)     # the segment's own "up": its back

        def off(h):
            return (mid[0] + n[0] * h, mid[1] + n[1] * h, mid[2] + n[2] * h)

        if mark == 'diamond':                                             # level segments: a purple diamond (fix round: no cream square on it, the plan said no pips)
            s = min(w * 0.56, L * 0.9)
            m.box(PURP, off(d / 2.0 + 4.0), (s, s, 14), rot=(pitch, yaw + 45.0, 0))
        elif mark == 'band':                                              # steep segments: a purple saddle
            m.box(PURP, off(d / 2.0 + 4.0), (L * 0.5, w * 0.5, 14), rot=(pitch, yaw, 0))
        if belly == 'under':
            m.box(CREAM, off(-d / 2.0 - 2.0), (L * 1.1, w * 0.62, 30), rot=(pitch, yaw, 0))
        elif belly is not None:
            ox, oy = belly
            m.box(CREAM, (mid[0] + ox, mid[1] + oy, mid[2] - d * 0.2), (L * 1.1, 26, d * 0.56), rot=(pitch, yaw, 0))

    # the coil: two and a half turns, rising and tightening; it ends heading for the road
    N = 40
    pts = []
    for i in range(N + 1):
        f = i / float(N)
        a = math.radians(900.0 * f)
        r = 1250.0 - 450.0 * f
        pts.append((math.cos(a) * r, math.sin(a) * r, 190.0 + 640.0 * f))
    for i in range(N):
        a, b = pts[i], pts[i + 1]
        mx, my = (a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0
        rr = math.hypot(mx, my)
        w = 300.0 + 60.0 * math.sin(math.pi * i / float(N))
        seg(g, a, b, w, 300.0, i, belly=(mx / rr * (w / 2.0 - 6.0), my / rr * (w / 2.0 - 6.0)))

    # the tail: back from the coil's outer end (the road's side, on the right) and up; the rattle on its tip rocks
    tail = [pts[0], (1290.0, -260.0, 380.0), (1310.0, -430.0, 720.0), (1290.0, -470.0, 1060.0)]
    for i in range(3):
        seg(g, tail[i + 1], tail[i], 250.0 - i * 50.0, 240.0 - i * 50.0, i + 3, mark='band' if i < 2 else None)
    rt = sc.part(P.motion(swing=11, pivot=(1290.0, -470.0, 1060.0), axis=0, rate=1.1, phase=0.0))
    for k in range(6):
        s = 150.0 - k * 16.0
        rt.box(CREAM if k % 2 == 0 else LAMP, (1290.0, -470.0, 1090.0 + k * 66.0), (s, s, 60.0), rot=(0, 45.0 if k % 2 else 0.0, 0))
    rt.box(PURP, (1290.0, -470.0, 1090.0 + 6 * 66.0 - 10.0), (40, 40, 40), rot=(0, 45, 0))

    # the neck: an S off the top of the coil, up and over the coil's front towards the road
    neck = [pts[N], (-790.0, -360.0, 930.0), (-720.0, -600.0, 1180.0), (-600.0, -620.0, 1480.0), (-470.0, -470.0, 1760.0),
            (-350.0, -300.0, 2000.0), (-240.0, -220.0, 2200.0), (-140.0, -300.0, 2350.0), (-60.0, -480.0, 2420.0), (0.0, -700.0, 2420.0)]
    M = len(neck) - 1
    hd = sc.part(P.motion(swing=4, pivot=neck[5], axis=P.UP, rate=0.11, phase=0.0))
    for i in range(M):
        m = g if i < 5 else hd
        w = 330.0 - 6.0 * i
        if i >= 6:                                                        # the hood spreads behind the head
            w = 330.0 + (i - 5) * 45.0
        seg(m, neck[i], neck[i + 1], w, 270.0 - 5.0 * i, i + 1, belly='under', mark='band')
    # the head: a wedge of bonnets, a cream jaw, lamp eyes, fangs, a forked tongue
    tilt_box(hd, SAGE, (0.0, -620.0, 2430.0), (0.0, -1270.0, 2330.0), 420.0, 230.0)
    tilt_box(hd, SAGE_D, (0.0, -1190.0, 2340.0), (0.0, -1450.0, 2285.0), 300.0, 150.0)
    tilt_box(hd, SHEET, (0.0, -670.0, 2550.0), (0.0, -1070.0, 2490.0), 250.0, 30.0)
    hd.box(PURP, (0.0, -850.0, 2550.0), (150, 150, 16), rot=(0, 45, 0))
    tilt_box(hd, CREAM, (0.0, -670.0, 2270.0), (0.0, -1350.0, 2110.0), 340.0, 70.0)       # the jaw, open
    hd.box(NAVY, (0.0, -1000.0, 2262.0), (300, 500, 60))
    for sx in (-1, 1):
        tilt_box(hd, PURP, (sx * 214.0, -700.0, 2510.0), (sx * 214.0, -1130.0, 2420.0), 14.0, 90.0)   # the brow plates
        hd.box(LAMP, (sx * 212.0, -970.0, 2400.0), (16, 90, 60), glow=0.5)                # the eyes
        hd.box(NAVY, (sx * 221.0, -970.0, 2400.0), (8, 20, 56))
        hd.cyl(CREAM, (sx * 110.0, -1330.0, 2262.0), 22, 150, rot=(180, 0, 0), steps=6, top_r=8)     # the fangs point down
        hd.box(NAVY, (sx * 70.0, -1452.0, 2300.0), (30, 6, 16))
        tilt_box(hd, PURP, (0.0, -1540.0, 2152.0), (sx * 70.0, -1800.0, 2110.0), 26.0, 12.0)
    tilt_box(hd, PURP, (0.0, -1300.0, 2180.0), (0.0, -1560.0, 2150.0), 40.0, 14.0)


JOBS = [
    ('SM_S1D2_QuonsetGarage', quonset_garage),
    ('SM_S1D2_CrusherShed', crusher_shed),
    ('SM_S1D2_GrainBinBar', grain_bin_bar),
    ('SM_S1D2_TyreWorkshop', tyre_workshop),
    ('SM_S1D2_LinemanHouse', lineman_house),
    ('SM_S1D2_TankHouse', tank_house),
    ('SM_S1D2_CarWallHouse', car_wall_house),
    ('SM_S1D2_HubcapFence', hubcap_fence, STILL),
    ('SM_S1D2_DrumPlanters', drum_planters, STILL),
    ('SM_S1D2_MailSnake', mail_snake, STILL),
    ('SM_S1D2_CarStack', car_stack, STILL),
    ('SM_S1D2_Whirligig', whirligig),
    ('SM_S1D2_TyreTower', tyre_tower),
    ('SM_S1D2_WashLine', wash_line),
    ('SM_S1D2_ScrapSign', scrap_sign),
    ('SM_S1D2_InsulatorStack', insulator_stack),
    ('SM_S1D2_TinRattler', tin_rattler, dict(grow=(140, 140, 80), grow_down=(140, 140, 0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
