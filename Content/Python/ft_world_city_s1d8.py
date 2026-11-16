"""City Center, Stage 1, builder s1d8: "Syndicate Territory, the circuit" (13.56 km to the end): the Chairman's works.
/Game/World/CityCenter/SM_S1D8_*. Run: import ft_world_city_s1d8 as K; K.run()

The Syndicate's yards and works behind its offices: cold steel frames, gasholder drums, riveted tanks on legs, lattice
gantries, round-ended sheds. Silver and pale steel with cold cyan light (steady), violet, blue-grey; black only as
trim; gold as ONE band a piece. Everything is STILL (the house rule of Syndicate Territory: nothing near the road
moves): no piece here has a moving part, so every piece is saved with true bounds.

Where the pieces are made to stand (the table block C:/ftwork/s1/L7_s1d8.inl):
  GAP buildings   in the 13 m gap between two podiums (22.6 m along from the slot's anchor), behind the Syndicate's
                  banners or its checkpoint: at most 11.3 m wide below 15 m. Gasholder, WaterTank, TramShed, SortingHall.
  ROOF pieces     on the Syndicate's black wall (SM_SynGate, roof 9 m), on the flat roof either side of its raised
                  middle: at most 6.4 m wide and 4.6 m deep. SignalCabin, SwitchHouse, LampMast, HandCrane, VentStack.
  LOW props       behind the Syndicate's barrier rows, 20.75 to 23.05 m out: under 2 m, at most 2.3 m deep, unlit.
                  BufferStop, CableDrums, PipeRack, YardGate.
  THE HOURGLASS   the hero: 120 m as built, stood 190 m out at 1.25.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/CityCenter'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))   # no piece here moves: true bounds

SIL, STE, BLG, CYA, VIO, GOL, BLK = '#c9c4d6', '#9a98a8', '#2a3a54', '#2ee6ff', '#8662b8', '#ffc93c', '#1c1a26'
G_GLASS, G_WIN, G_LINE, G_LAMP = 0.3, 0.7, 1.1, 2.0      # steady light, on pieces stood 24 m and more out only


# ------------------------------------------------------------------------------------------ helpers
def ring(g, col, c, r, n, w, d=None, glow=0.0, a0=0.0):
    """A flat ring of n straight girders round c = (x, y, z)."""
    pts = [(c[0] + r * math.cos(math.radians(a0 + 360.0 * k / n)), c[1] + r * math.sin(math.radians(a0 + 360.0 * k / n)), c[2]) for k in range(n)]
    for k in range(n):
        tilt_box(g, col, pts[k], pts[(k + 1) % n], w, d or w, glow=glow)


def hoop(g, col, c, r, n, w, d):
    """An upright ring (in the X-Z plane) of n straight bars round c: a wheel's rim seen from the road."""
    pts = [(c[0] + r * math.cos(math.radians(360.0 * k / n)), c[1], c[2] + r * math.sin(math.radians(360.0 * k / n))) for k in range(n)]
    for k in range(n):
        tilt_box(g, col, pts[k], pts[(k + 1) % n], w, d)


def cross(g, col, a, b, z0, z1, t):
    """An X of two braces between the posts at a and b (x, y), from z0 up to z1."""
    tilt_box(g, col, (a[0], a[1], z0), (b[0], b[1], z1), t, t)
    tilt_box(g, col, (a[0], a[1], z1), (b[0], b[1], z0), t, t)


def arch(w, wall, rise, n=8, cx=0.0, z0=0.0):
    """The outline (x, z) of a round-ended roof on its walls: w wide, the walls `wall` tall from z0, the roof `rise` more."""
    pts = []
    if wall > 0:
        pts += [(cx - w / 2.0, z0), (cx + w / 2.0, z0)]
    for k in range(n + 1):
        a = math.pi * k / n
        pts.append((cx + w / 2.0 * math.cos(a), z0 + wall + rise * math.sin(a)))
    return pts


def polar(r, deg, z=0.0, c=(0.0, 0.0)):
    return (c[0] + r * math.cos(math.radians(deg)), c[1] + r * math.sin(math.radians(deg)), z)


# ------------------------------------------------------------------------------------------ GAP buildings
def gasholder(sc):
    """BUILDING: a gasholder. A silver drum of two lifts risen out of its blue-grey tank inside a ring of eight guide
    columns, 11.3 m across, 26 m tall; a valve house at its foot. Stands in a gap behind the Syndicate's banners."""
    g = sc.still()
    R = 470.0
    g.cyl(BLG, (0, 0, -300), 500, 900, steps=16)                       # the tank (3 m of footing)
    g.cyl(CYA, (0, 0, 600), 506, 14, steps=16, glow=G_LINE)            # a steady line of light round its rim
    g.cyl(SIL, (0, 0, 614), R, 780, steps=16)                          # the first lift
    g.cyl(STE, (0, 0, 1000), R + 5, 16, steps=16)                      # a riveted hoop
    g.cyl(GOL, (0, 0, 1380), R + 8, 30, steps=16)                      # the one gold band: the cup between the lifts
    g.cyl(STE, (0, 0, 1410), R - 25, 690, steps=16)                    # the second lift
    g.cyl(SIL, (0, 0, 2100), R - 25, 150, steps=16, top_r=60)          # the crown
    g.cyl(VIO, (0, 0, 2250), 60, 70, steps=8)                          # the vent
    g.cyl(SIL, (0, 0, 2320), 86, 20, steps=8, top_r=20)
    cols = [polar(545, 22.5 + 45 * k) for k in range(8)]
    for k, c in enumerate(cols):
        g.box(SIL, (c[0], c[1], 1150), (34, 34, 2900))
        g.box(VIO, (c[0] * 0.955, c[1] * 0.955, 2540), (30, 30, 60))    # a guide roller at the head of each column
    for z in (880, 1740, 2580):
        ring(g, SIL, (0, 0, z), 545, 8, 22, 30, a0=22.5)
    for k in range(8):
        a, b = cols[k], cols[(k + 1) % 8]
        cross(g, STE, a, b, 1760, 2560, 16)                             # the top bay: an X
        tilt_box(g, STE, (a[0], a[1], 900), (b[0], b[1], 1720), 16, 16)  # the middle bay: one brace, all leaning one way
    # the valve house at the foot, on the road's side
    g.box(BLG, (0, -560, 110), (230, 160, 280))
    g.side(SIL, arch(250, 0, 60, n=6, cx=0.0, z0=250), -645, -475)
    g.box(CYA, (-55, -642, 150), (70, 6, 80), glow=G_WIN)
    g.box(VIO, (60, -642, 105), (70, 6, 190))
    rod(g, STE, (150, -560, 60), (300, -560, 60), 26, steps=8)
    rod(g, VIO, (300, -590, 60), (300, -598, 60), 44, steps=8)          # the stop valve's hand wheel


def water_tank(sc):
    """BUILDING: a riveted water tank on six splayed legs, a violet band round it, a walkway and its rail, a ladder up
    the front, a gauge board with a cyan marker. 9.3 m across, 23 m tall. Stands in a gap behind a checkpoint."""
    g = sc.still()
    foot = [polar(420, 60 * k, -300) for k in range(6)]
    top = [polar(330, 60 * k, 1420) for k in range(6)]
    for k in range(6):
        tilt_box(g, SIL, foot[k], top[k], 36, 36)

    def at(z):
        f = (z + 300) / 1720.0
        return [polar(420 - 90 * f, 60 * k) for k in range(6)]

    for z in (450, 950):
        p = at(z)
        for k in range(6):
            tilt_box(g, SIL, (p[k][0], p[k][1], z), (p[(k + 1) % 6][0], p[(k + 1) % 6][1], z), 20, 26)
    for z0, z1 in ((470, 930), (970, 1380)):
        p0, p1 = at(z0), at(z1)
        for k in range(6):
            j = (k + 1) % 6
            tilt_box(g, STE, (p0[k][0], p0[k][1], z0), (p1[j][0], p1[j][1], z1), 14, 14)
            tilt_box(g, STE, (p0[j][0], p0[j][1], z0), (p1[k][0], p1[k][1], z1), 14, 14)
    g.cyl(STE, (0, 0, -300), 40, 1650, steps=8)                         # the riser
    g.cyl(STE, (0, 0, 1300), 120, 150, steps=12, top_r=350)             # the tank's dished bottom
    g.cyl(GOL, (0, 0, 1450), 357, 24, steps=12)                         # the one gold band: the base ring
    g.cyl(SIL, (0, 0, 1474), 350, 540, steps=12)                        # the tank
    g.cyl(STE, (0, 0, 1600), 354, 16, steps=12)
    g.cyl(VIO, (0, 0, 1700), 356, 90, steps=12)                         # the violet band
    g.cyl(STE, (0, 0, 1880), 354, 16, steps=12)
    g.cyl(BLG, (0, 0, 2010), 380, 230, steps=12, top_r=30)              # the roof
    g.sphere(SIL, (0, 0, 2262), 34, steps=(3, 6))
    ring(g, STE, (0, 0, 1420), 430, 6, 70, 8)                           # the walkway
    ring(g, SIL, (0, 0, 1520), 462, 6, 8, 8)                            # its rail
    for k in range(6):
        c = polar(462, 60 * k)
        g.box(SIL, (c[0], c[1], 1472), (8, 8, 100))
    for s in (-1, 1):                                                   # the ladder, between the two front legs
        g.box(SIL, (s * 24, -362, 1000), (6, 6, 2020))
    for k in range(10):
        g.box(SIL, (0, -362, 110 + k * 200), (50, 6, 6))
    g.box(BLG, (150, -320, 1730), (40, 10, 400))                        # the gauge board
    g.box(CYA, (150, -327, 1820), (48, 6, 44), glow=G_WIN)              # and its marker: nearly full
    rod(g, STE, (-200, -290, 1400), (-200, -398, 1320), 22, steps=6)    # the filling spout, folded down


def tram_shed(sc):
    """BUILDING: a round-ended tram shed end-on to the road: a steel hall 10 m wide under a barrel roof with a cyan-lit
    clerestory along its ridge, two round-headed doors, a tram's nose out of one, a fanlight in the gable. 13.9 m tall."""
    g = sc.still()
    g.side(SIL, arch(1000, 1000, 400, n=10, z0=-300), -430, 450)        # the hall: walls to 7 m, the roof to 11 m
    for y in (-436, 0, 436):
        g.side(BLG, arch(1026, 1000, 413, n=10, z0=-300), y - 14, y + 14)   # the frames
    # the front: the first frame is a solid plate, its face at F
    F = -450.0
    g.box(BLG, (-230, F - 5, 210), (360, 10, 420))                      # the shut door
    rod(g, BLG, (-230, F - 1, 420), (-230, F - 11, 420), 180, steps=12)
    g.box(BLK, (230, F - 3, 210), (360, 6, 420))                        # the open one: black inside
    rod(g, BLK, (230, F - 1, 420), (230, F - 7, 420), 180, steps=12)
    g.box(VIO, (230, F - 70, 190), (290, 150, 300))                     # the tram's nose
    g.box(SIL, (230, F - 70, 395), (300, 160, 110))
    g.box(CYA, (230, F - 152, 330), (240, 6, 100), glow=G_GLASS)        # its windscreen
    g.box(STE, (230, F - 148, 140), (60, 8, 60))                        # its lamp, unlit
    g.box(BLK, (230, F - 154, 50), (300, 12, 30))                       # its bumper
    tilt_box(g, VIO, (230, F - 70, 450), (230, F - 16, 606), 8, 8)      # its trolley pole, up to the wire
    for x in (-230, 230):
        for s in (-1, 1):
            g.box(STE, (x + s * 72, F - 90, 6), (10, 180, 12))          # the rails out of each door
        tilt_box(g, SIL, (x, F, 640), (x, F - 120, 606), 8, 8)          # a bracket for the wire over each door
        g.sphere(SIL, (x, F - 120, 598), 14, steps=(3, 5))
    g.box(GOL, (0, F - 5, 650), (1000, 8, 26))                          # the one gold band, across the front
    g.side(CYA, arch(520, 0, 250, n=8, z0=690), F - 8, F + 2, glow=G_WIN)   # the fanlight
    for a in (45, 90, 135):
        tilt_box(g, BLG, (0, F - 10, 690), (250 * math.cos(math.radians(a)), F - 10, 690 + 240 * math.sin(math.radians(a))), 8, 12)
    rod(g, VIO, (0, F - 1, 540), (0, F - 11, 540), 46, steps=8)         # the shed's number plate: blank
    for s in (-1, 1):
        g.box(STE, (s * 492, F - 8, 350), (12, 12, 700))                # downpipes
    # the clerestory along the ridge
    g.box(BLG, (0, 10, 1150), (260, 800, 160))
    for s in (-1, 1):
        g.box(CYA, (s * 132, 10, 1160), (6, 760, 90), glow=G_LINE)
    g.side(SIL, arch(300, 0, 70, n=6, z0=1225), -400, 420)
    for y in (-220, 240):
        g.cyl(STE, (0, y, 1280), 26, 70, steps=6)
        g.cyl(VIO, (0, y, 1350), 48, 36, steps=6, top_r=6)
    for s in (-1, 1):                                                   # windows down both sides
        for k in range(4):
            g.box(CYA, (s * 502, -300 + k * 200, 430), (6, 120, 200), glow=G_WIN)


def sorting_hall(sc):
    """BUILDING: a sorting hall under a round-ended roof, a covered conveyor climbing out of it to the head of a
    hopper on four braced legs. 11.1 m wide, 19.3 m tall. Stands in a gap behind a checkpoint."""
    g = sc.still()
    g.side(SIL, arch(500, 820, 230, n=8, cx=-310, z0=-300), -350, 350)      # the hall: walls to 5.2 m, the roof to 7.5 m
    for y in (-350, 350):
        g.side(BLG, arch(524, 820, 242, n=8, cx=-310, z0=-300), y - 12, y + 12)
    g.box(BLG, (-310, -364, 200), (260, 8, 400))                        # its door
    g.box(CYA, (-310, -364, 560), (300, 8, 60), glow=G_WIN)
    g.cyl(STE, (-470, 150, 600), 28, 300, steps=6)                      # a vent pipe
    g.cyl(VIO, (-470, 150, 900), 52, 40, steps=6, top_r=6)
    # the hopper
    hx = 320.0
    legs = [(hx - 190, -190), (hx + 190, -190), (hx + 190, 190), (hx - 190, 190)]
    for c in legs:
        g.box(SIL, (c[0], c[1], 350), (30, 30, 1300))
    for k in range(4):
        a, b = legs[k], legs[(k + 1) % 4]
        tilt_box(g, SIL, (a[0], a[1], 460), (b[0], b[1], 460), 18, 22)
        cross(g, STE, a, b, 480, 980, 14)
    g.cyl(STE, (hx, 0, 700), 64, 300, steps=4, top_r=300, rot=(0, 45, 0))  # the bin's funnel
    g.box(SIL, (hx, 0, 1250), (424, 424, 500))                          # the bin
    g.box(GOL, (hx, 0, 1020), (434, 434, 40))                           # the one gold band
    g.box(STE, (hx, 0, 1320), (432, 432, 16))
    g.cyl(VIO, (hx, 0, 540), 46, 170, steps=6)                          # the spout
    g.box(VIO, (hx, 0, 70), (190, 120, 70))                             # a tipping tub under it (a hand tub)
    g.box(STE, (hx, 0, 22), (230, 90, 26))
    g.box(BLG, (hx, 0, 1650), (380, 380, 300))                          # the head house
    g.side(SIL, arch(404, 0, 130, n=8, cx=hx, z0=1800), -202, 202)
    g.box(CYA, (hx, -192, 1660), (200, 6, 90), glow=G_WIN)
    g.box(CYA, (hx + 192, 0, 1660), (6, 200, 90), glow=G_WIN)
    for k in range(4):                                                  # the gallery round the bin's top
        a, b = legs[k], legs[(k + 1) % 4]
        tilt_box(g, SIL, (a[0] * 1.0 + (a[0] - hx) * 0.22, a[1] * 1.22, 1590), (b[0] + (b[0] - hx) * 0.22, b[1] * 1.22, 1590), 8, 8)
    g.box(STE, (hx, 0, 1505), (470, 470, 10))
    # the conveyor, from the hall's far end up to the head house
    a, b = (-520.0, 0.0, 600.0), (150.0, 0.0, 1640.0)
    tilt_box(g, SIL, a, b, 150, 130)
    tilt_box(g, CYA, (a[0] + 40, -78, a[2] + 40), (b[0] - 30, -78, b[2] - 40), 6, 46, glow=G_WIN)   # its row of lit panes
    for k in range(5):
        f = (k + 0.5) / 5.0
        tilt_box(g, BLG, (a[0] + (b[0] - a[0]) * f, 0, a[2] + (b[2] - a[2]) * f - 10), (a[0] + (b[0] - a[0]) * f + 12, 0, a[2] + (b[2] - a[2]) * f + 8), 160, 140)   # hoops
    for x in (-150.0, 20.0):                                            # trestles off the hall's roof and the ground
        z = a[2] + (x - a[0]) / (b[0] - a[0]) * (b[2] - a[2]) - 70
        z0 = 680.0 if x < -60 else -300.0
        for s in (-1, 1):
            tilt_box(g, SIL, (x, s * 150, z0), (x, s * 50, z), 18, 18)
        g.box(SIL, (x, 0, (z0 + z) / 2.0), (14, 200, 14))
    # a zigzag stair up the hopper's front, and a heap of silver ore before it
    tilt_box(g, STE, (hx - 190, -214, 0), (hx + 190, -214, 480), 60, 10)
    tilt_box(g, STE, (hx + 190, -214, 980), (hx - 190, -214, 1500), 60, 10)
    g.box(STE, (hx, -214, 730), (460, 60, 10))
    tilt_box(g, STE, (hx + 190, -214, 480), (hx - 190, -214, 730), 60, 10)
    tilt_box(g, STE, (hx - 190, -214, 730), (hx + 190, -214, 980), 60, 10)
    g.cyl(SIL, (30, -250, 0), 110, 110, steps=7, top_r=10)


# ------------------------------------------------------------------------------------------ ROOF pieces (on the black wall: z = 0 is its roof)
def signal_cabin(sc):
    """BUILDING (a roof-top one): a signal cabin on braced stilts, glass all round under a round-ended roof, a
    stovepipe, a weather vane, a ladder, a semaphore post with its arm at rest. 6.3 x 4.6 m, 8.7 m tall."""
    g = sc.still()
    legs = [(-250, -170), (250, -170), (250, 170), (-250, 170)]
    for c in legs:
        g.box(SIL, (c[0], c[1], 130), (24, 24, 320))
    for k in range(4):
        cross(g, STE, legs[k], legs[(k + 1) % 4], 20, 270, 12)
    g.box(BLG, (0, 0, 300), (600, 440, 24))                             # the floor
    g.box(GOL, (0, -222, 300), (600, 6, 24))                            # the one gold band: its front edge
    g.box(SIL, (0, 0, 362), (560, 400, 100))                            # the panelled waist
    g.box(VIO, (0, -203, 364), (240, 6, 50))                            # the cabin's name board: blank
    g.box(CYA, (0, 0, 487), (540, 380, 150), glow=G_GLASS)              # glass all round
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(SIL, (sx * 272, sy * 192, 487), (16, 16, 150))
        g.box(SIL, (sx * 272, 0, 487), (10, 10, 150))
    for x in (-136, 0, 136):
        g.box(SIL, (x, -192, 487), (10, 8, 150))
        g.box(SIL, (x, 192, 487), (10, 8, 150))
    g.box(SIL, (0, 0, 570), (584, 424, 16))
    g.box(CYA, (0, -214, 558), (560, 6, 8), glow=G_LINE)                # a steady line under the eave
    g.side(BLG, arch(620, 0, 110, n=8, z0=578), -226, 226)              # the round-ended roof
    g.cyl(STE, (190, 110, 620), 14, 170, steps=6)                       # the stovepipe
    g.cyl(VIO, (190, 110, 790), 30, 26, steps=6, top_r=5)
    g.cyl(SIL, (-60, 0, 688), 6, 120, steps=5)                          # the weather vane: a plain arrow
    g.box(SIL, (-60, 0, 800), (130, 6, 8))
    g.box(VIO, (-112, 0, 800), (34, 6, 40))
    g.sphere(SIL, (-60, 0, 822), 12, steps=(3, 5))
    for s in (-1, 1):                                                   # the ladder at the left end
        g.box(SIL, (-296, s * 26 - 100, 150), (6, 6, 300))
    for k in range(5):
        g.box(SIL, (-296, -100, 40 + k * 56), (6, 52, 6))
    g.cyl(SIL, (292, -200, 0), 10, 860, steps=6)                        # the semaphore post
    g.box(VIO, (212, -208, 806), (170, 8, 34))                          # its arm, at rest
    g.box(SIL, (146, -209, 806), (38, 10, 34))
    g.sphere(SIL, (292, -200, 868), 14, steps=(3, 5))


def switch_house(sc):
    """BUILDING (a roof-top one): a switch house. A riveted steel cube with louvres and a door, a cyan line of light
    round its cornice, three fat insulators on its roof under a bus-bar gantry with arcing horns. 5.9 x 4.4 m, 9.6 m tall."""
    g = sc.still()
    g.box(BLG, (0, 0, 10), (560, 420, 80))
    g.box(GOL, (0, 0, 62), (566, 426, 24))                              # the one gold band
    g.box(SIL, (0, 0, 292), (460, 400, 436))                            # the cube
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STE, (sx * 224, sy * 194, 292), (20, 20, 440))
    for z in (190, 400):
        g.box(STE, (0, 0, z), (466, 406, 16))
    for z in (190, 400):
        for k in range(4):
            g.box(BLG, (-170 + k * 113, -205, z), (9, 6, 9))            # rivets on the straps
    for k in range(5):
        g.box(BLG, (-110, -202, 228 + k * 30), (160, 8, 16))            # louvres
    g.box(BLG, (120, -202, 200), (130, 6, 250))                         # the door
    g.box(VIO, (120, -207, 270), (46, 6, 46), rot=(45, 0, 0))           # a plain violet diamond on it
    g.box(CYA, (0, 0, 516), (470, 410, 12), glow=G_LINE)                # the cyan line
    g.box(BLG, (0, 0, 532), (490, 430, 20))
    for x in (-150, 0, 150):                                            # the insulators
        for k in range(4):
            g.cyl(SIL if k % 2 == 0 else VIO, (x, 0, 542 + k * 38), 44 if k % 2 == 0 else 30, 34, steps=6)
        g.box(STE, (x, 0, 786), (8, 8, 176))
    for s in (-1, 1):
        g.box(SIL, (s * 232, 0, 706), (16, 16, 330))                    # the gantry
        tilt_box(g, VIO, (s * 250, 0, 880), (s * 290, 0, 960), 8, 8)    # arcing horns
        tilt_box(g, VIO, (s * 250, 0, 880), (s * 216, 0, 960), 8, 8)
        rod(g, STE, (228, s * 100, 400), (286, s * 100, 440), 20, steps=6)     # wall bushings
        rod(g, VIO, (250, s * 100, 415), (262, s * 100, 424), 34, steps=6)
    g.box(SIL, (0, 0, 876), (520, 16, 20))
    g.box(STE, (0, -216, 22), (400, 20, 30))                            # the cable trough


def lamp_mast(sc):
    """PROP (tall, for a roof): a lattice lamp mast with four cyan globes, steady. 2.6 m across, 9 m tall."""
    g = sc.still()
    g.box(BLG, (0, 0, 10), (130, 130, 80))
    g.box(GOL, (0, 0, 56), (136, 136, 16))                              # the one gold band
    H = 760.0

    def at(z):
        f = (z - 50) / (H - 50)
        w = 50 - 34 * f
        return [(-w, -w), (w, -w), (w, w), (-w, w)]

    b, t = at(50), at(H)
    for k in range(4):
        tilt_box(g, SIL, (b[k][0], b[k][1], 50), (t[k][0], t[k][1], H), 10, 10)
    zs = [50, 200, 340, 470, 590, 690, 760]
    for i in range(6):
        p0, p1 = at(zs[i]), at(zs[i + 1])
        for k in range(4):
            j = (k + 1) % 4
            a, c = (p0[k], p1[j]) if i % 2 == 0 else (p0[j], p1[k])
            tilt_box(g, STE, (a[0], a[1], zs[i]), (c[0], c[1], zs[i + 1]), 7, 7)
    for z in (340, 590):
        p = at(z)
        for k in range(4):
            j = (k + 1) % 4
            tilt_box(g, SIL, (p[k][0], p[k][1], z), (p[j][0], p[j][1], z), 8, 8)
    g.box(STE, (0, 0, 765), (110, 110, 10))
    g.cyl(SIL, (0, 0, 770), 8, 110, steps=6)
    g.cyl(VIO, (0, 0, 880), 16, 22, steps=6, top_r=3)
    g.box(SIL, (0, 0, 800), (270, 10, 10))
    g.box(SIL, (0, 0, 800), (10, 270, 10))
    for c in ((130, 0), (-130, 0), (0, 130), (0, -130)):
        g.cyl(BLG, (c[0], c[1], 806), 20, 14, steps=6)
        g.sphere(CYA, (c[0], c[1], 850), 34, steps=(4, 6), glow=G_LAMP)


def hand_crane(sc):
    """PROP (tall, for a roof): a hand crane. A short still jib on a king post over a turntable, a violet crank wheel,
    a counterweight, a chain and hook holding one silver ingot over a stack of them. The jib lies along the road. 5.6 m tall."""
    g = sc.still()
    g.cyl(BLG, (-120, 0, -30), 90, 96, steps=8)
    g.cyl(GOL, (-120, 0, 66), 94, 16, steps=8)                          # the one gold band
    g.cyl(STE, (-120, 0, 82), 70, 24, steps=8)
    for k in range(6):
        c = polar(80, 60 * k, 0, (-120, 0))
        g.box(SIL, (c[0], c[1], 70), (10, 10, 10))                      # the holding-down bolts
    g.box(SIL, (-120, 0, 260), (40, 40, 320))                           # the king post
    for s in (-1, 1):
        tilt_box(g, SIL, (-110, s * 30, 300), (220, s * 8, 540), 14, 14)   # the jib's two chords
    for f in (0.25, 0.5, 0.75):
        g.box(STE, (-110 + 330 * f, 0, 300 + 240 * f), (10, 60 - 40 * f, 10))
    tilt_box(g, STE, (-120, 0, 420), (220, 0, 548), 8, 8)               # the tie
    tilt_box(g, STE, (-120, 0, 420), (-255, 0, 170), 8, 8)              # the back stay
    g.box(STE, (-190, 0, 100), (170, 40, 14))
    g.box(BLG, (-255, 0, 122), (90, 120, 90))                           # the counterweight
    rod(g, VIO, (-120, -24, 240), (-120, -36, 240), 70, steps=10)       # the crank wheel
    rod(g, SIL, (-120, -36, 240), (-120, -46, 240), 16, steps=6)
    g.box(SIL, (-120, -52, 292), (12, 22, 12))
    rod(g, STE, (-120, 24, 240), (-120, 36, 240), 42, steps=8)          # the gear
    rod(g, STE, (220, -12, 540), (220, 12, 540), 26, steps=8)           # the jib's sheave
    g.box(STE, (220, 0, 400), (6, 6, 230))                              # the chain
    g.box(VIO, (220, 0, 272), (28, 22, 30))                             # the hook block
    g.box(STE, (220, 0, 246), (8, 8, 26))
    g.box(STE, (232, 0, 232), (28, 8, 8))
    g.box(STE, (244, 0, 244), (8, 8, 22))
    for s in (-1, 1):                                                   # the slung ingot
        tilt_box(g, STE, (232, 0, 232), (232 + s * 30, 0, 196), 5, 5)
    g.box(SIL, (232, 0, 184), (84, 44, 22))
    g.box(BLG, (215, 0, 8), (190, 120, 16))                             # the pallet and its stack
    for row, n in enumerate((3, 2, 1)):
        for k in range(n):
            g.box(SIL, (215 + (k - (n - 1) / 2.0) * 54, 0, 27 + row * 22), (46, 96, 20))


def vent_stack(sc):
    """PROP (tall, for a roof): three ventilators on a louvred plenum: a tall pipe under a mushroom cap with a plain
    weather vane, an onion cowl, a wide one under a coolie-hat cap; a cyan pilot line, a valve. 5.3 x 3 m, 7.4 m tall."""
    g = sc.still()
    g.box(BLG, (0, 0, 40), (520, 300, 140))
    g.box(GOL, (0, 0, 116), (526, 306, 12))                             # the one gold band
    for k in range(6):
        g.box(STE, (-200 + k * 80, -152, 50), (60, 8, 70))              # louvres
    g.box(CYA, (0, -154, 100), (500, 6, 10), glow=G_LINE)               # the pilot line
    # the tall one
    g.cyl(SIL, (-170, 0, 110), 50, 340, steps=8)
    g.cyl(VIO, (-170, 0, 330), 54, 30, steps=8)
    for k in range(3):
        c = polar(44, 120 * k, 0, (-170, 0))
        g.box(STE, (c[0], c[1], 462), (8, 8, 30))
    g.cyl(STE, (-170, 0, 474), 96, 14, steps=8)
    g.cyl(SIL, (-170, 0, 488), 96, 66, steps=8, top_r=10)
    g.cyl(SIL, (-170, 0, 550), 6, 150, steps=5)                         # the vane: an arrow, four plain balls for the winds
    g.box(SIL, (-170, 0, 700), (150, 6, 10))
    tilt_box(g, SIL, (-95, 0, 700), (-125, 0, 722), 6, 8)
    tilt_box(g, SIL, (-95, 0, 700), (-125, 0, 678), 6, 8)
    g.box(VIO, (-232, 0, 700), (40, 6, 44))
    g.box(SIL, (-170, 0, 620), (100, 6, 6))
    g.box(SIL, (-170, 0, 620), (6, 100, 6))
    for c in ((50, 0), (-50, 0), (0, 50), (0, -50)):
        g.sphere(SIL, (-170 + c[0], c[1], 620), 10, steps=(3, 5))
    g.sphere(SIL, (-170, 0, 728), 12, steps=(3, 5))
    # the onion cowl
    g.cyl(SIL, (10, 0, 110), 60, 190, steps=8)
    g.sphere(SIL, (10, 0, 352), 86, scale=(1.0, 1.0, 0.8), steps=(5, 8))
    g.cyl(VIO, (10, 0, 290), 66, 20, steps=8)
    g.cyl(STE, (10, 0, 418), 16, 30, steps=6, top_r=4)
    # the wide one
    g.cyl(STE, (185, 0, 110), 70, 120, steps=8)
    for k in range(3):
        c = polar(60, 120 * k + 60, 0, (185, 0))
        g.box(SIL, (c[0], c[1], 246), (8, 8, 36))
    g.cyl(VIO, (185, 0, 262), 104, 56, steps=8, top_r=8)
    rod(g, STE, (255, -100, 60), (255, -190, 60), 20, steps=6)          # a pipe stub and its valve
    rod(g, VIO, (255, -190, 60), (255, -196, 60), 34, steps=8)


# ------------------------------------------------------------------------------------------ LOW props (under 2 m, unlit, at most 2.3 m deep)
def buffer_stop(sc):
    """PROP (low): the end of a siding. Rails on sleepers, a buffer stop with a violet beam, two buffers and an unlit
    lamp, a points lever, a hand trolley carrying three coils of cable. 9.4 m long, 1.9 m tall."""
    g = sc.still()
    g.box(BLG, (0, 0, -12), (940, 200, 56))                             # the ballast (40 cm of footing)
    for k in range(9):
        g.box(STE, (-420 + k * 105, 0, 21), (26, 190, 10))
    for s in (-1, 1):
        g.box(SIL, (0, s * 62, 33), (940, 10, 14))
        tilt_box(g, SIL, (300, s * 62, 40), (432, s * 62, 128), 14, 14)      # the stop's raking struts
        g.box(SIL, (440, s * 62, 90), (16, 16, 110))
        rod(g, STE, (425, s * 62, 128), (392, s * 62, 128), 24, steps=8)     # the buffers
        rod(g, SIL, (392, s * 62, 128), (380, s * 62, 128), 30, steps=8)
    g.box(VIO, (440, 0, 128), (30, 210, 44))                            # the beam
    g.box(GOL, (423, 0, 128), (6, 80, 14))                              # the one gold band
    g.box(SIL, (440, 0, 162), (10, 10, 26))
    g.box(BLG, (440, 0, 174), (28, 28, 24))                             # the lamp (unlit: it stands inside 24 m)
    rod(g, VIO, (426, 0, 174), (422, 0, 174), 10, steps=6)
    # the points lever
    g.box(BLG, (60, -80, 30), (60, 30, 30))
    tilt_box(g, SIL, (60, -80, 40), (100, -80, 150), 8, 8)
    rod(g, VIO, (100, -84, 150), (100, -76, 150), 20, steps=8)
    # the hand trolley
    g.box(SIL, (-250, 0, 70), (210, 120, 10))
    for sx in (-1, 1):
        for sy in (-1, 1):
            rod(g, STE, (-250 + sx * 70, sy * 56, 52), (-250 + sx * 70, sy * 68, 52), 16, steps=8)
    for s in (-1, 1):
        g.box(VIO, (-350, s * 50, 122), (8, 8, 96))
    g.box(VIO, (-350, 0, 170), (8, 108, 8))
    for k in range(3):
        g.cyl(VIO if k != 1 else SIL, (-240, 0, 75 + k * 34), 46 - k * 3, 30, steps=8)


def cable_drums(sc):
    """PROP (low): two great cable drums on their rims, chocked, their faces to the road (one silver with violet cable,
    one violet with steel cable), and a small one lying flat. 7.4 m long, 1.9 m tall."""
    g = sc.still()
    g.box(BLG, (-60, 0, -15), (640, 190, 50))                           # the skid (40 cm of footing)
    for x, r, fl, core in ((-210.0, 90.0, SIL, VIO), (20.0, 84.0, VIO, STE)):
        z = 10 + r
        rod(g, fl, (x, -62, z), (x, -50, z), r, steps=12)
        rod(g, fl, (x, 50, z), (x, 62, z), r, steps=12)
        rod(g, core, (x, -50, z), (x, 50, z), r * 0.66, steps=10)
        rod(g, BLG, (x, -66, z), (x, -62, z), 20, steps=6)              # the hub
        for k in range(4):
            c = (x + r * 0.62 * math.cos(math.radians(45 + 90 * k)), z + r * 0.62 * math.sin(math.radians(45 + 90 * k)))
            g.box(BLG, (c[0], -64, c[1]), (10, 6, 10))                  # the flange bolts
        for s in (-1, 1):
            tilt_box(g, STE, (x + s * (r + 6), 0, 10), (x + s * (r * 0.62), 0, 40), 130, 14)   # chocks
    g.cyl(SIL, (255, 0, 10), 64, 10, steps=10)                          # the small one, flat
    g.cyl(CYA, (255, 0, 20), 42, 44, steps=10)                          # its cyan cable (unlit)
    g.cyl(SIL, (255, 0, 64), 64, 10, steps=10)
    tilt_box(g, CYA, (296, -10, 40), (350, -40, 12), 9, 9)


def pipe_rack(sc):
    """PROP (low): three riveted pipes on two cradles (two under, one over), flanged at the ends and the middle,
    strapped down, a hand wheel on a stub. They lie along the road. 7.6 m long, 1.95 m tall."""
    g = sc.still()
    for x in (-230, 230):
        g.box(BLG, (x, 0, 10), (40, 206, 70))                           # the cradles (25 cm of footing)
        g.box(BLG, (x, -96, 62), (40, 14, 60))
        g.box(BLG, (x, 96, 62), (40, 14, 60))
    for (y, z, band) in ((-46, 72, GOL), (46, 72, None), (0, 145, VIO)):
        rod(g, SIL, (-380, y, z), (380, y, z), 42, steps=10)
        for x in (-374, 0, 374):
            rod(g, STE, (x - 6, y, z), (x + 6, y, z), 49, steps=10)     # flanges
        for s in (-1, 1):
            rod(g, STE, (s * 380, y, z), (s * 382, y, z), 34, steps=8)  # the end cap (fix round: was a black bore, like a launcher seen end-on)
        if band:
            rod(g, band, (120, y, z), (170, y, z), 44, steps=10)        # the one gold band, and a violet one
    for x in (-230, 230):                                               # the straps
        tilt_box(g, STE, (x + 26, -92, 60), (x + 26, -30, 178), 8, 8)
        tilt_box(g, STE, (x + 26, 92, 60), (x + 26, 30, 178), 8, 8)
        g.box(STE, (x + 26, 0, 191), (8, 70, 6))
    rod(g, STE, (-110, -46, 72), (-110, -100, 72), 12, steps=6)         # the stub and its hand wheel
    rod(g, VIO, (-110, -100, 72), (-110, -106, 72), 30, steps=8)


def yard_gate(sc):
    """PROP (low): a run of steel yard fence, 12 m, with a round rolling gate in its middle: a spoked steel ring on a
    track, a gold hub. 1.95 m tall."""
    g = sc.still()
    g.box(BLG, (0, 0, -10), (1214, 40, 60))                             # the kerb (40 cm of footing)
    for x in (-600, -400, -200, 200, 400, 600):
        g.box(SIL, (x, 0, 102), (14, 14, 176))
        g.box(VIO, (x, 0, 191), (22, 22, 8))
    for x0 in (-600, -400, 200, 400):
        for z in (60, 160):
            g.box(STE, (x0 + 100, 0, z), (186, 6, 8))
        for k in range(5):
            g.box(SIL, (x0 + 34 + k * 33, -4, 104), (10, 6, 152))
    g.box(SIL, (0, 0, 190), (400, 10, 10))                              # the gate's track
    g.box(STE, (0, 0, 22), (400, 10, 6))
    hoop(g, SIL, (0, 0, 104), 78, 12, 8, 10)
    for k in range(3):
        a = math.radians(60 * k + 30)
        tilt_box(g, STE, (-72 * math.cos(a), 0, 104 - 72 * math.sin(a)), (72 * math.cos(a), 0, 104 + 72 * math.sin(a)), 6, 6)
    rod(g, GOL, (0, -9, 104), (0, 9, 104), 22, steps=8)                 # the one gold band: the hub
    rod(g, VIO, (0, -12, 104), (0, -9, 104), 13, steps=6)
    for s in (-1, 1):
        tilt_box(g, STE, (s * 193, 0, 150), (s * 150, 0, 185), 6, 6)    # gussets
        g.box(SIL, (s * 60, 0, 186), (10, 14, 14))                      # the rollers the ring hangs from


# ------------------------------------------------------------------------------------------ the hero
def hourglass(sc):
    """HERO: THE HOURGLASS. A silver lattice frame 120 m tall (four lattice posts between two blue-grey discs, ring
    girders and cross bracing) holding a giant hourglass in silver ribs: the lower bulb nearly full of steady cyan
    light, the upper nearly empty, one gold band at the waist. The Chairman's time running out. STILL."""
    g = sc.still()
    g.cyl(BLG, (0, 0, -300), 2500, 700, steps=16)                       # the foot disc (3 m of footing)
    g.cyl(CYA, (0, 0, 400), 2520, 40, steps=16, glow=G_LINE)
    g.cyl(SIL, (0, 0, 440), 900, 260, steps=12, top_r=520)              # the lower cup
    g.cyl(CYA, (0, 0, 11260), 2520, 40, steps=16, glow=G_LINE)
    g.cyl(BLG, (0, 0, 11300), 2500, 300, steps=16)                      # the head disc
    g.cyl(SIL, (0, 0, 11600), 1300, 260, steps=12, top_r=300)
    g.sphere(SIL, (0, 0, 11880), 120, steps=(4, 8))
    g.cyl(SIL, (0, 0, 11000), 520, 300, steps=12, top_r=900)            # the upper cup
    for k in range(4):                                                  # the four lattice posts
        a = 45 + 90 * k
        p0, p1 = polar(2100, a), polar(2400, a)
        g.box(BLG, ((p0[0] + p1[0]) / 2.0, (p0[1] + p1[1]) / 2.0, 150), (420, 420, 900), rot=(0, a, 0))
        g.box(SIL, (p0[0], p0[1], 5850), (60, 60, 10900), rot=(0, a, 0))
        g.box(SIL, (p1[0], p1[1], 5850), (60, 60, 10900), rot=(0, a, 0))
        zs = [400 + i * 1362.5 for i in range(9)]
        for i in range(8):
            u, v = (p0, p1) if i % 2 == 0 else (p1, p0)
            tilt_box(g, STE, (u[0], u[1], zs[i]), (v[0], v[1], zs[i + 1]), 40, 40)
    post = [polar(2250, 45 + 90 * k) for k in range(4)]
    tiers = (400, 3125, 5850, 8575, 11300)
    for z in tiers[1:4]:
        ring(g, SIL, (0, 0, z), 2250, 4, 50, 80, a0=45)
    for i in range(4):
        for k in range(4):
            cross(g, STE, post[k], post[(k + 1) % 4], tiers[i] + 60, tiers[i + 1] - 60, 36)
    # the glass: (z, radius) up the lower bulb; the upper is its mirror about the waist at 58.5 m
    prof = [(700, 520), (1100, 1250), (1900, 1450), (3300, 1450), (4050, 1190), (4500, 1010), (5350, 300)]
    sand = 5        # the lower bulb's first five lengths are full of light; the top one is clear glass
    for i in range(6):
        (z0, r0), (z1, r1) = prof[i], prof[i + 1]
        if i < sand:
            g.cyl(CYA, (0, 0, z0), r0, z1 - z0, steps=16, top_r=r1, glow=1.6)
        else:
            g.cyl(VIO, (0, 0, z0), r0, z1 - z0, steps=16, top_r=r1, glow=0.15)
        u0, u1 = 11700 - z1, 11700 - z0                                 # the upper bulb's matching length
        if i == 5:
            g.cyl(CYA, (0, 0, u0), r1, u1 - u0, steps=16, top_r=r0, glow=1.6)      # what is left in the funnel
        else:
            g.cyl(VIO, (0, 0, u0), r1, u1 - u0, steps=16, top_r=r0, glow=0.15)
    g.cyl(CYA, (0, 0, 5350), 210, 1000, steps=10, glow=1.6)             # the thread running through the neck
    g.cyl(GOL, (0, 0, 5600), 330, 500, steps=12)                        # THE one gold band, at the waist
    for z in (2600, 9100):
        g.cyl(SIL, (0, 0, z - 20), 1466, 40, steps=16)                  # a hoop round each bulb
    for k in range(8):                                                  # eight silver ribs down the glass
        a = 22.5 + 45 * k
        for i in range(6):
            (z0, r0), (z1, r1) = prof[i], prof[i + 1]
            tilt_box(g, SIL, polar(r0 + 12, a, z0), polar(r1 + 12, a, z1), 50, 50)
            tilt_box(g, SIL, polar(r1 + 12, a, 11700 - z1), polar(r0 + 12, a, 11700 - z0), 50, 50)
    # the foot disc's door and windows, on the road's side
    g.box(BLK, (0, -2500, 120), (260, 30, 420))
    for s in (-1, 1):
        g.box(CYA, (s * 520, -2440, 200), (260, 30, 120), glow=G_WIN)


JOBS = [('SM_S1D8_Gasholder', gasholder, STILL), ('SM_S1D8_WaterTank', water_tank, STILL), ('SM_S1D8_TramShed', tram_shed, STILL),
        ('SM_S1D8_SortingHall', sorting_hall, STILL), ('SM_S1D8_SignalCabin', signal_cabin, STILL), ('SM_S1D8_SwitchHouse', switch_house, STILL),
        ('SM_S1D8_LampMast', lamp_mast, STILL), ('SM_S1D8_HandCrane', hand_crane, STILL), ('SM_S1D8_VentStack', vent_stack, STILL),
        ('SM_S1D8_BufferStop', buffer_stop, STILL), ('SM_S1D8_CableDrums', cable_drums, STILL), ('SM_S1D8_PipeRack', pipe_rack, STILL),
        ('SM_S1D8_YardGate', yard_gate, STILL), ('SM_S1D8_Hourglass', hourglass, STILL)]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
