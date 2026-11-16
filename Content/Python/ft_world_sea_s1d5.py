"""Under the Sea, Stage 1, builder s1d5: "Nereida: the Pearl Quarter" (7.94 to 12.2 km).
/Game/World/UnderTheSea/SM_S1D5_*. Run: import ft_world_sea_s1d5 as K; K.run()

The old sunken city inside the gate: colonnades, stepped plinths, round-arched arcades, low pink domes with a pearl on
top, in marble cream, plum shadow, pearl pink, rose stone and old gold. 7 buildings, 8 props, 1 hero (THE GREAT PEARL).
Every piece is an ft_people.Scene: one material (M_People), one section. Front faces -Y, long pieces run along X, cm.
No fin, no jaw, no fish, no wheel, no box of pickup size in orange or green; statues are animals (a seahorse).
Glow (steady, never blinking) only on pieces whose every rule stands 24 m or more out.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SMALL = dict(grow=(12, 12, 8), grow_down=(12, 12, 0))

CREAM, PLUM, PINK, ROSE, GOLD = '#f4efe0', '#6a4562', '#ff9ec7', '#c79aa2', '#c9b44a'
PEARL, LAV, MAG, GLASS = '#fff1c0', '#b9a4ff', '#ff4fb8', '#cfeeff'


# ---------------------------------------------------------------------------------------------- helpers
def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def arch(g, col, x0, x1, zs, zt, y0, y1, r, n=6):
    """The wall over a round arch: the bay x0..x1 from the springing zs up to zt, a half circle of radius r cut out."""
    cx = (x0 + x1) / 2.0
    pts = [(x0, zs), (cx - r, zs)]
    for k in range(1, n):
        a = math.pi - k * math.pi / n
        pts.append((cx + r * math.cos(a), zs + r * math.sin(a)))
    pts += [(cx + r, zs), (x1, zs), (x1, zt), (x0, zt)]
    g.side(col, _ccw(pts), y0, y1)


def column(g, x, y, z, h, r=45.0, cap=GOLD):
    """A column standing on z, h tall in all: a rose base, a tapering cream shaft, a gold capital."""
    g.box(ROSE, (x, y, z + 12), (r * 2.6, r * 2.6, 24))
    g.cyl(CREAM, (x, y, z + 24), r, h - 54, steps=6, top_r=r * 0.84)
    g.box(cap, (x, y, z + h - 15), (r * 2.7, r * 2.7, 30))


def dome(g, x, y, z, r, col=PINK, squash=0.8, steps=(5, 10)):
    g.sphere(col, (x, y, z), r, scale=(1, 1, squash), steps=steps)
    return z + r * squash


def finial(g, x, y, z, r, glow=0.7):
    """A gold stem and a pearl on it; returns the top."""
    g.cyl(GOLD, (x, y, z - 6), r * 0.45, r * 1.1, steps=6, top_r=r * 0.7)
    g.sphere(PEARL, (x, y, z + r * 1.9), r, steps=(4, 8), glow=glow)
    return z + r * 2.9


def tuft(g, x, y, z, r, col=LAV):
    g.sphere(col, (x, y, z), r, steps=(3, 6))


# ---------------------------------------------------------------------------------------------- buildings
def hall(sc):
    """The colonnaded hall: 23 x 12.5 m, 11.7 m tall. Eight columns across the front, a pink roof, banners that sway."""
    g = sc.still()
    g.box(ROSE, (0, 0, -60), (2300, 1100, 280))                 # plinth, 2 m of footing, top 80
    g.box(ROSE, (0, -655, -35), (1000, 90, 130))                # two wide steps down to the sand
    g.box(CREAM, (0, -590, -10), (1000, 90, 140))
    g.box(CREAM, (0, 0, 105), (2180, 1000, 50))                 # stylobate, top 130
    g.box(ROSE, (0, 130, 430), (1640, 560, 600))                # the cella, front at y = -150
    g.box(PLUM, (0, -153, 330), (240, 10, 400))                 # door
    g.box(GOLD, (0, -156, 545), (320, 12, 30))
    for x in (-560, -320, 320, 560):
        g.box(PLUM, (x, -153, 470), (110, 10, 230))
    for k in range(8):
        column(g, -980 + k * 280, -400, 130, 600)
    for sx in (-980, 980):
        for y in (-120, 160, 420):
            column(g, sx, y, 130, 600)
    g.box(CREAM, (0, 0, 765), (2200, 1040, 70))                 # entablature 730..800
    g.box(GOLD, (0, -522, 765), (2200, 8, 24))
    g.side(PINK, _ccw([(-1150, 800), (1150, 800), (0, 1080)]), -500, 520)
    g.side(CREAM, _ccw([(-1000, 806), (1000, 806), (0, 1040)]), -520, -498)
    g.sphere(GOLD, (0, -524, 890), 70, scale=(1, 0.25, 1), steps=(4, 8))
    g.sphere(PEARL, (0, -542, 890), 34, steps=(3, 6), glow=0.6)
    g.box(GOLD, (0, 10, 1086), (36, 1000, 26))                  # ridge
    finial(g, 0, -470, 1090, 44)
    for sx in (-1100, 1100):
        g.cyl(GOLD, (sx, -480, 800), 44, 120, steps=6, top_r=8)
    for sx in (-700, 700):                                       # urns at the head of the steps
        g.cyl(ROSE, (sx, -505, 130), 40, 30, steps=6)
        g.cyl(CREAM, (sx, -505, 160), 34, 80, steps=6, top_r=58)
        tuft(g, sx, -505, 270, 50)
    for i, x in enumerate((-560, 560)):                          # two long banners between the columns
        f = sc.part(P.motion(swing=5, pivot=(x, -405, 725), axis=0.0, rate=0.22, phase=0.2 + 0.4 * i))
        f.box(PINK, (x, -405, 520), (170, 8, 400))
        f.box(GOLD, (x, -411, 360), (170, 6, 30))
        f.box(GOLD, (x, -411, 690), (170, 6, 24))


def bath(sc):
    """The domed bath house: 16 x 15 m, 14 m tall. A three-arched porch, a great pink dome on a drum, a vent that
    lets go slow bubbles."""
    g = sc.still()
    g.box(ROSE, (0, 0, -70), (1600, 1400, 260))                 # plinth top 60
    g.box(ROSE, (0, -760, -25), (700, 120, 110))                # step
    g.box(CREAM, (0, 120, 360), (1300, 960, 600))               # block: y -360..600, z 60..660
    g.box(PLUM, (0, -364, 255), (1180, 10, 390))                # the porch's shadow
    g.box(PINK, (0, -372, 240), (200, 8, 330))                  # door curtain
    xs = (-600, -200, 200, 600)
    for x in xs:
        g.box(CREAM, (x, -560, 270), (96, 96, 420))
        g.box(GOLD, (x, -560, 322), (108, 108, 16))
    for k in range(3):
        arch(g, CREAM, xs[k], xs[k + 1], 330, 520, -605, -515, 150)
    g.box(CREAM, (0, -480, 545), (1340, 280, 50))               # porch roof 520..570
    g.box(GOLD, (0, -612, 585), (1340, 14, 30))
    g.box(ROSE, (0, 120, 680), (1380, 1040, 40))                # cornice
    g.cyl(CREAM, (0, 120, 700), 440, 170, steps=10)             # drum
    for a in (-50, -25, 0, 25, 50):
        ra = math.radians(a)
        g.box(PLUM, (math.sin(ra) * 438, 120 - math.cos(ra) * 438, 790), (64, 16, 96), rot=(0, a, 0))
    g.cyl(GOLD, (0, 120, 870), 452, 26, steps=10)
    top = dome(g, 0, 120, 890, 425, squash=0.82, steps=(6, 10))
    finial(g, 0, 120, top + 4, 56)
    for sx in (-560, 560):                                       # two small domes on the porch
        t = dome(g, sx, -480, 575, 120, steps=(4, 8))
        g.sphere(PEARL, (sx, -480, t + 18), 24, steps=(3, 6), glow=0.6)
    for sx in (-1, 1):
        for y in (-80, 320):
            g.box(PLUM, (sx * 652, y, 380), (10, 150, 300))
        g.box(ROSE, (sx * 400, -655, 95), (220, 70, 70))        # planters on the plinth
        tuft(g, sx * 400 - 50, -655, 150, 40)
        tuft(g, sx * 400 + 50, -655, 146, 36, MAG)
    g.cyl(ROSE, (430, 450, 660), 70, 230, steps=6, top_r=55)    # the vent
    g.cyl(GOLD, (430, 450, 890), 72, 20, steps=6)
    for k in range(3):
        b = sc.part(P.motion(bob=-40, rate=0.3, phase=k * 0.33))
        b.sphere(GLASS, (430 + (k - 1) * 30, 450, 990 + k * 95), 26 + k * 8, steps=(3, 6))


def temple(sc):
    """The stepped temple: 16 x 17 m, 14.5 m tall. Four tiers, a stair up the front, a domed shrine with the
    quarter's pearl in it, two pennants."""
    g = sc.still()
    tiers = ((1600, ROSE, -200, 150), (1340, CREAM, 150, 330), (1080, ROSE, 330, 510), (820, CREAM, 510, 690))
    for w, c, z0, z1 in tiers:
        g.box(c, (0, 0, (z0 + z1) / 2.0), (w, w, z1 - z0))
    for w, c, z0, z1 in tiers[1:]:
        g.box(PLUM, (0, 0, z0 + 8), (w + 14, w + 14, 16))       # a line of shadow at each tier's foot
    for k in range(6):                                           # the stair
        top, yf = 115.0 * (k + 1), -900.0 + 82.0 * k
        g.box(CREAM, (0, (yf - 400.0) / 2.0, (top - 40.0) / 2.0), (380, -400.0 - yf, top + 40.0))
    for sx in (-225, 225):
        tilt_box(g, ROSE, (sx, -915, 40), (sx, -420, 720), 60, 70)
        g.sphere(PEARL, (sx, -925, 125), 40, steps=(3, 6), glow=0.5)
    for sx in (-290, 290):
        for sy in (-290, 290):
            column(g, sx, sy, 690, 340, r=40)
    g.box(CREAM, (0, 0, 1050), (780, 780, 40))
    g.box(GOLD, (0, 0, 1076), (700, 700, 12))
    top = dome(g, 0, 0, 1075, 310, squash=0.75)
    finial(g, 0, 0, top + 4, 50)
    g.cyl(GOLD, (0, 0, 690), 70, 110, steps=8, top_r=110)       # the pearl of the shrine on its cup
    g.sphere(PEARL, (0, 0, 905), 115, steps=(5, 8), glow=0.9)
    for sx in (-600, 600):
        for sy in (-600, 600):
            g.cyl(GOLD, (sx, sy, 330), 16, 150, steps=6)
            g.sphere(PEARL, (sx, sy, 510), 34, steps=(3, 6), glow=0.6)
    for i, sx in enumerate((-360, 360)):
        g.cyl(CREAM, (sx, 360, 690), 10, 520, steps=6)
        f = sc.part(P.motion(swing=14, pivot=(sx, 360, 1170), axis=P.UP, rate=0.3, phase=0.5 * i))
        f.box(PINK, (sx + 95, 360, 1170), (190, 6, 70))
        f.box(GOLD, (sx + 195, 360, 1170), (20, 7, 80))


def arcade(sc):
    """The arcade of shops: 27 x 9 m, 11.4 m tall, two storeys. Six round arches, counters in their shade, awnings
    that stir, three small domes and a washing line on the roof, hanging picture signs (a pearl, a shell)."""
    g = sc.still()
    g.box(ROSE, (0, 0, -80), (2700, 900, 240))                  # top 40
    g.box(CREAM, (0, 170, 440), (2600, 460, 800))               # y -60..400, z 40..840
    g.box(PLUM, (0, -64, 235), (2480, 10, 370))                 # shop fronts in shadow
    xs = [-1200 + 400 * k for k in range(7)]
    for x in xs:
        g.box(CREAM, (x, -360, 190), (84, 84, 300))
        g.box(GOLD, (x, -360, 352), (100, 100, 24))
    for k in range(6):
        arch(g, CREAM, xs[k], xs[k + 1], 364, 540, -400, -320, 150)
        cx = xs[k] + 200
        g.box(ROSE, (cx, -200, 85), (240, 70, 90))              # a counter
        if k % 3 == 0:
            g.sphere(PEARL, (cx - 50, -200, 152), 22, steps=(3, 6), glow=0.4)
            g.sphere(PEARL, (cx + 40, -200, 148), 18, steps=(3, 6), glow=0.4)
        elif k % 3 == 1:
            g.box(PINK, (cx - 40, -200, 146), (90, 40, 32))
            g.box(LAV, (cx + 60, -200, 142), (70, 40, 24))
        else:
            g.cyl(CREAM, (cx, -200, 130), 16, 50, steps=6, top_r=30)
            g.cyl(CREAM, (cx, -200, 180), 30, 30, steps=6, top_r=12)
        g.box(PLUM, (cx, -64, 700), (150, 10, 190))             # upper window
        g.box(ROSE, (cx, -76, 596), (190, 34, 22))              # its sill
    g.box(CREAM, (0, -215, 560), (2600, 410, 40))               # gallery floor 540..580
    g.box(CREAM, (0, -410, 628), (2600, 24, 20))                # balustrade
    for k in range(7):
        g.box(CREAM, (-1200 + 400 * k, -410, 599), (30, 24, 38))
    a = sc.part(P.motion(swing=3, pivot=(0, -66, 812), axis=0.0, rate=0.3, phase=0.1))
    for k in range(6):
        cx = xs[k] + 200
        tilt_box(a, PINK if k % 2 == 0 else GOLD, (cx, -66, 812), (cx, -190, 752), 190, 8)
    for i, k in enumerate((1, 3, 5)):                            # picture signs under the gallery's edge
        x = xs[k]
        s = sc.part(P.motion(swing=6, pivot=(x, -425, 540), axis=0.0, rate=0.35, phase=0.3 * i))
        s.box(CREAM, (x, -425, 512), (6, 6, 56))
        s.sphere(PINK if i % 2 == 0 else GOLD, (x, -425, 446), 44, scale=(1, 0.2, 1), steps=(3, 6))
    g.box(ROSE, (0, 170, 860), (2680, 540, 40))                 # cornice 840..880
    g.box(CREAM, (0, -80, 905), (2640, 30, 50))
    for x in (-900, 0, 900):
        g.cyl(CREAM, (x, 200, 880), 190, 60, steps=8)
        t = dome(g, x, 200, 940, 180, steps=(4, 8))
        g.sphere(PEARL, (x, 200, t + 24), 30, steps=(3, 6), glow=0.6)
    for x in (-450, 450):                                        # the washing line
        g.cyl(ROSE, (x, 150, 880), 7, 170, steps=6)
    g.box(PLUM, (0, 150, 1042), (900, 5, 5))
    w = sc.part(P.motion(swing=10, pivot=(0, 150, 1040), axis=0.0, rate=0.35, phase=0.6))
    for x, c, h in ((-300, PINK, 90), (-100, LAV, 70), (100, CREAM, 100), (300, PEARL, 80)):
        w.box(c, (x, 150, 1040 - h / 2.0), (90, 5, h))


def treasury(sc):
    """The round treasury: 11 m across, 13.5 m tall. A ring of ten columns round a rose drum, a pink cone and dome,
    a pearl finial, two hanging banners."""
    g = sc.still()
    g.cyl(ROSE, (0, 0, -200), 560, 280, steps=12)               # top 80
    g.cyl(CREAM, (0, 0, 80), 500, 50, steps=12)                 # top 130
    g.cyl(ROSE, (0, 0, 130), 330, 520, steps=10)
    g.box(PLUM, (0, -322, 300), (180, 24, 330))
    g.box(GOLD, (0, -326, 478), (240, 24, 26))
    for k in range(10):
        a = math.radians(18 + 36 * k)
        x, y = math.cos(a) * 440, math.sin(a) * 440
        g.cyl(CREAM, (x, y, 130), 38, 470, steps=6, top_r=32)
        g.box(GOLD, (x, y, 615), (100, 100, 30), rot=(0, 18 + 36 * k, 0))
    g.cyl(CREAM, (0, 0, 630), 500, 70, steps=12)                # entablature
    g.cyl(GOLD, (0, 0, 700), 510, 24, steps=12)
    g.cyl(PINK, (0, 0, 724), 500, 60, steps=12, top_r=380)
    g.cyl(CREAM, (0, 0, 784), 300, 140, steps=10)
    for a in (-60, -20, 20, 60):
        ra = math.radians(a)
        g.box(PLUM, (math.sin(ra) * 298, -math.cos(ra) * 298, 856), (60, 16, 84), rot=(0, a, 0))
    g.cyl(GOLD, (0, 0, 924), 310, 16, steps=10)
    top = dome(g, 0, 0, 938, 300)
    finial(g, 0, 0, top + 4, 62, glow=0.8)
    for sx in (-1, 1):                                           # a pearl on a cup each side of the door
        g.cyl(GOLD, (sx * 240, -470, 130), 20, 110, steps=6, top_r=40)
        g.sphere(PEARL, (sx * 240, -470, 280), 44, steps=(4, 6), glow=0.6)
    for i, a in enumerate((-36, 36)):                            # banners between the columns either side of the door
        ra = math.radians(a)
        x, y = math.sin(ra) * 445, -math.cos(ra) * 445
        f = sc.part(P.motion(swing=5, pivot=(x, y, 625), axis=float(a), rate=0.24, phase=0.45 * i))
        f.box(PINK, (x, y, 455), (150, 8, 330), rot=(0, a, 0))
        f.box(GOLD, (x, y, 300), (150, 10, 26), rot=(0, a, 0))


def arch_gate(sc):
    """The triumphal arch, standing ALONG the road (its three openings face the road, nothing spans it): 19 x 4.8 m,
    12.6 m tall. Half columns, an attic with reliefs, three pearls on top, two pennants."""
    g = sc.still()
    piers = ((-770, 260), (-300, 160), (300, 160), (770, 260))
    for cx, w in piers:
        g.box(CREAM, (cx, 0, 335), (w, 420, 970))               # z -150..820
        g.box(ROSE, (cx, 0, 20), (w + 44, 480, 240))            # plinth -100..140
    for x0, x1 in ((-640, -380), (380, 640)):
        arch(g, CREAM, x0, x1, 420, 820, -210, 210, 122)
    arch(g, CREAM, -220, 220, 560, 820, -210, 210, 212, n=8)
    g.box(ROSE, (0, 0, 835), (1900, 480, 30))                   # cornice 820..850
    g.box(CREAM, (0, 0, 945), (1840, 440, 190))                 # attic 850..1040
    g.box(GOLD, (0, 0, 1052), (1900, 480, 24))
    for x in (-600, 0, 600):
        g.box(PLUM, (x, -223, 945), (360, 10, 110))             # relief panels
        g.box(PINK, (x, -227, 945), (300, 8, 22))
    for sx in (-510, 510):
        g.sphere(PINK, (sx, -214, 660), 62, scale=(1, 0.2, 1), steps=(4, 8))     # roundels over the side openings
        g.sphere(PEARL, (sx, -228, 660), 26, steps=(3, 6), glow=0.5)
    for x in (-880, -660, 660, 880):
        g.cyl(CREAM, (x, -232, 140), 34, 650, steps=6, top_r=29)
        g.box(GOLD, (x, -232, 803), (92, 60, 26))
    for x in (-700, 0, 700):
        g.cyl(GOLD, (x, 0, 1064), 60, 50, steps=8, top_r=90)
        g.sphere(PEARL, (x, 0, 1190 if x == 0 else 1170), 84 if x == 0 else 62, steps=(4, 8), glow=0.7)
    for i, sx in enumerate((-900, 900)):
        g.cyl(CREAM, (sx, 150, 1064), 9, 190, steps=6)
        f = sc.part(P.motion(swing=14, pivot=(sx, 150, 1225), axis=P.UP, rate=0.3, phase=0.5 * i))
        f.box(PINK, (sx + 85, 150, 1225), (170, 6, 60))


def house(sc):
    """A town house of the quarter: 13.4 x 8.6 m with its stair and its washing pole, 13.6 m tall. A rose ground
    storey with a shop awning, a cream upper storey with shutters and window boxes, a loggia under a small dome, a
    gold vane that turns slowly, washing on a line."""
    g = sc.still()
    g.box(ROSE, (0, 0, 90), (900, 800, 580))                    # z -200..380, front y = -400
    g.box(PLUM, (-220, -403, 150), (150, 10, 260))              # door
    g.box(GOLD, (-220, -405, 292), (190, 10, 24))
    g.box(PLUM, (190, -403, 200), (280, 10, 180))               # shop window
    g.box(CREAM, (190, -410, 104), (300, 24, 14))
    g.box(CREAM, (0, 0, 395), (940, 840, 30))                   # string course 380..410
    g.box(CREAM, (0, 40, 580), (780, 680, 340))                 # upper storey 410..750, front y = -300
    g.box(CREAM, (0, -400, 440), (900, 20, 60))                 # terrace wall
    for sx in (-200, 200):
        g.box(PLUM, (sx, -303, 590), (120, 10, 190))
        for e in (-82, 82):
            g.box(PINK, (sx + e, -305, 590), (40, 8, 190))
        g.box(ROSE, (sx, -325, 478), (170, 44, 32))             # window box
        tuft(g, sx - 40, -325, 510, 28, MAG)
        tuft(g, sx + 40, -325, 508, 26)
    g.box(ROSE, (0, 40, 765), (820, 720, 30))                   # cornice 750..780
    for sx in (-250, 250):
        for sy in (-180, 260):
            column(g, sx, sy, 780, 260, r=30)
    g.box(CREAM, (0, 40, 1055), (620, 560, 30))
    top = dome(g, 0, 40, 1070, 250)
    g.cyl(GOLD, (0, 40, top - 6), 10, 90, steps=6)
    v = sc.part(P.motion(swing=40, pivot=(0, 40, top + 70), axis=P.UP, rate=0.08, phase=0.3))
    v.box(GOLD, (0, 40, top + 70), (230, 8, 8))
    v.box(GOLD, (-95, 40, top + 70), (50, 6, 50))
    v.sphere(PEARL, (118, 40, top + 70), 20, steps=(3, 6), glow=0.5)
    for k in range(5):                                           # the outside stair up the right flank
        tz = 82.0 * (k + 1)
        g.box(ROSE, (530, -330 + 110 * k, (tz - 100.0) / 2.0), (160, 110, tz + 100.0))
    a = sc.part(P.motion(swing=3, pivot=(190, -405, 332), axis=0.0, rate=0.3, phase=0.7))
    for i, x in enumerate((90, 190, 290)):
        tilt_box(a, PINK if i != 1 else CREAM, (x, -405, 332), (x, -520, 272), 100, 8)
    g.cyl(ROSE, (-720, -300, -100), 9, 780, steps=6)            # the washing pole, top 680
    tilt_box(g, PLUM, (-390, -300, 690), (-720, -300, 672), 5, 5)
    w = sc.part(P.motion(swing=9, pivot=(-555, -300, 680), axis=0.0, rate=0.33, phase=0.15))
    for x, c, h in ((-460, PINK, 80), (-555, CREAM, 105), (-650, LAV, 75)):
        w.box(c, (x, -300, 676 - h / 2.0), (72, 5, h))


# ---------------------------------------------------------------------------------------------- props
def broken_column(sc):
    """LOW (1.8 m). A broken column on its slab, its fallen drum and capital in the sand beside it."""
    g = sc.still()
    g.box(ROSE, (-30, 10, -10), (260, 230, 60))                 # slab -40..20
    g.box(CREAM, (-30, 10, 33), (150, 150, 26))
    g.cyl(CREAM, (-30, 10, 46), 56, 112, steps=8, top_r=52)     # stump to 158
    g.cyl(GOLD, (-30, 10, 128), 59, 12, steps=8)
    g.box(CREAM, (-42, 4, 160), (70, 56, 22), rot=(10, 30, 0))  # the break
    rod(g, CREAM, (130, -80, 54), (300, -40, 54), 54, steps=8)  # the fallen drum
    rod(g, GOLD, (196, -64.5, 54), (210, -61.2, 54), 57, steps=8)
    g.box(CREAM, (-210, -90, 24), (120, 120, 44), rot=(0, 25, 8))    # the capital
    g.box(GOLD, (-210, -90, 50), (132, 132, 10), rot=(0, 25, 8))
    g.sphere(PINK, (60, 80, 26), 34, scale=(1, 1, 0.3), steps=(3, 5))    # a sea star on the slab


def bench_urn(sc):
    """LOW (1.9 m). A stone bench with a back and an urn of weed whose strands stir."""
    g = sc.still()
    g.box(ROSE, (0, 0, -10), (430, 150, 60))                    # paving -40..20
    for x in (-200, 20):
        g.box(ROSE, (x, 0, 42), (30, 64, 44))
    g.box(CREAM, (-90, 0, 72), (290, 72, 16))                   # seat 64..80
    g.box(GOLD, (-90, -37, 72), (290, 5, 8))
    g.box(CREAM, (-90, 32, 112), (290, 14, 64))                 # back to 144
    for x in (-225, 45):
        g.sphere(CREAM, (x, 32, 150), 20, steps=(3, 6))
    g.box(ROSE, (155, 0, 45), (72, 72, 50))                     # the urn's block 20..70
    g.cyl(CREAM, (155, 0, 70), 20, 18, steps=8)
    g.cyl(CREAM, (155, 0, 88), 24, 46, steps=8, top_r=44)
    g.cyl(GOLD, (155, 0, 134), 46, 8, steps=8)
    tuft(g, 155, 0, 160, 27)
    s = sc.part(P.motion(swing=6, pivot=(155, 0, 142), axis=0.0, rate=0.4, phase=0.2))
    tilt_box(s, MAG, (155, -30, 144), (155, -68, 96), 12, 10)
    tilt_box(s, LAV, (175, 28, 144), (190, 62, 100), 12, 10)


def planter_rail(sc):
    """LOW (1.8 m). A length of balustrade between two pearl-topped posts, a planter of weed in front that stirs."""
    g = sc.still()
    g.box(ROSE, (0, 0, -10), (470, 96, 60))                     # -40..20
    for x in (-210, 210):
        g.box(CREAM, (x, 12, 80), (38, 38, 120))                # 20..140
        g.box(GOLD, (x, 12, 144), (46, 46, 8))
        g.sphere(PEARL, (x, 12, 164), 18, steps=(3, 6))         # no glow: it stands on the verge
    g.box(CREAM, (0, 12, 120), (400, 26, 16))
    g.box(CREAM, (0, 12, 32), (400, 26, 14))
    for x in (-140, -70, 0, 70, 140):
        g.cyl(CREAM, (x, 12, 38), 10, 76, steps=6, top_r=7)
    g.box(ROSE, (0, -26, 45), (320, 38, 50))                    # planter 20..70
    t = sc.part(P.motion(swing=5, pivot=(0, -26, 70), axis=0.0, rate=0.4, phase=0.5))
    for x, c, r in ((-110, LAV, 24), (-40, PINK, 20), (40, MAG, 22), (110, LAV, 24)):
        t.sphere(c, (x, -26, 86), r, steps=(3, 6))


def pearl_lamp(sc):
    """TALL (5.5 m). A pearl on a cream post with two smaller pearls on gold arms; a steady soft glow."""
    g = sc.still()
    g.box(ROSE, (0, 0, 0), (100, 100, 80))                      # -40..40
    g.box(CREAM, (0, 0, 52), (70, 70, 24))
    g.cyl(CREAM, (0, 0, 64), 16, 320, steps=6, top_r=13)        # to 384
    for z in (150, 372):
        g.cyl(GOLD, (0, 0, z), 23, 14, steps=6)
    g.box(GOLD, (0, 0, 392), (180, 10, 10))
    for sx in (-82, 82):
        g.cyl(GOLD, (sx, 0, 396), 9, 16, steps=6, top_r=20)
        g.sphere(PEARL, (sx, 0, 436), 26, steps=(3, 6), glow=0.9)
    g.cyl(GOLD, (0, 0, 396), 16, 42, steps=6, top_r=44)
    g.sphere(PEARL, (0, 0, 490), 56, steps=(5, 8), glow=1.0)


def seahorse(sc):
    """TALL (6.6 m). The quarter's statue: a gold seahorse (an animal, no likeness, no fin on its back) on a plinth."""
    g = sc.still()
    g.box(ROSE, (0, 0, -10), (220, 220, 100))                   # -60..40
    g.box(CREAM, (0, 0, 130), (150, 150, 180))                  # 40..220
    g.box(PLUM, (0, -77, 130), (90, 6, 110))
    g.box(GOLD, (0, 0, 228), (170, 170, 16))
    g.box(CREAM, (0, 0, 244), (130, 130, 16))                   # top 252
    z = 252.0
    tilt_box(g, GOLD, (24, 0, z + 20), (-34, 0, z + 30), 40, 36)        # the curled tail
    tilt_box(g, GOLD, (-34, 0, z + 24), (-52, 0, z + 84), 40, 40)
    tilt_box(g, GOLD, (-52, 0, z + 76), (-8, 0, z + 134), 44, 46)
    tilt_box(g, GOLD, (-12, 0, z + 120), (30, 0, z + 232), 56, 76)      # the trunk
    g.sphere(PINK, (40, 0, z + 185), 50, scale=(0.8, 0.62, 1.2), steps=(4, 6))    # belly
    tilt_box(g, GOLD, (30, 0, z + 220), (4, 0, z + 304), 48, 56)        # neck
    g.box(GOLD, (30, 0, z + 322), (112, 50, 58))                        # head
    tilt_box(g, GOLD, (76, 0, z + 318), (152, 0, z + 300), 24, 24)      # snout
    for dx, dz, h in ((0, 362, 28), (22, 366, 34), (44, 360, 26)):
        g.box(GOLD, (dx, 0, z + dz), (14, 14, h))                       # a coronet
    g.sphere(PEARL, (22, 0, z + 396), 14, steps=(3, 6))
    for sy in (-26, 26):
        g.box(PLUM, (48, sy, z + 330), (14, 6, 14))


def fountain(sc):
    """TALL (5.6 m with its bubbles). A mosaic basin, a shell bowl on a stem with a pearl in it, slow bubbles."""
    g = sc.still()
    g.cyl(ROSE, (0, 0, -40), 260, 115, steps=10)                # to 75
    g.cyl(GLASS, (0, 0, 60), 225, 22, steps=10)                 # the water, to 82
    for k in range(10):
        a = 18 + 36 * k
        ra = math.radians(a)
        g.box(PINK if k % 2 else GOLD, (math.cos(ra) * 243, math.sin(ra) * 243, 80), (60, 34, 12), rot=(0, a + 90, 0))
    g.cyl(CREAM, (0, 0, 75), 34, 150, steps=8, top_r=26)        # stem to 225
    g.cyl(PINK, (0, 0, 225), 30, 55, steps=10, top_r=135)       # shell bowl to 280
    g.cyl(GOLD, (0, 0, 280), 138, 10, steps=10)
    g.sphere(PEARL, (0, 0, 334), 48, steps=(5, 8), glow=0.9)
    for k in range(6):
        ra = math.radians(60 * k + 20)
        b = sc.part(P.motion(bob=-35, rate=0.26 + 0.02 * k, phase=k / 6.0))
        b.sphere(GLASS, (math.cos(ra) * 150, math.sin(ra) * 150, 150 + 46 * k), 14 + 3 * (k % 3), steps=(3, 6))
    for k in range(3):
        b = sc.part(P.motion(bob=-30, rate=0.3, phase=0.2 + k * 0.3))
        b.sphere(GLASS, (20 * (k - 1), 14 * (1 - k), 430 + 45 * k), 16, steps=(3, 6))


def banner_mast(sc):
    """TALL (10.3 m). A mast with a long pink banner (a pearl on it, no words), a streamer at the top, a pearl finial."""
    g = sc.still()
    g.box(ROSE, (0, 0, -10), (130, 130, 100))                   # -60..40
    g.box(CREAM, (0, 0, 60), (90, 90, 40))
    g.cyl(CREAM, (0, 0, 80), 13, 860, steps=6, top_r=9)         # to 940
    for z in (300, 600):
        g.cyl(GOLD, (0, 0, z), 20, 14, steps=6)
    g.box(GOLD, (0, 0, 882), (200, 10, 10))                     # the yard
    g.cyl(GOLD, (0, 0, 940), 12, 30, steps=6, top_r=22)
    g.sphere(PEARL, (0, 0, 1000), 30, steps=(4, 6), glow=0.8)
    f = sc.part(P.motion(swing=6, pivot=(0, -8, 878), axis=0.0, rate=0.25, phase=0.4))
    f.box(PINK, (0, -8, 640), (150, 6, 470))
    f.box(GOLD, (0, -12, 852), (150, 5, 24))
    f.box(GOLD, (0, -12, 428), (150, 5, 24))
    f.sphere(PEARL, (0, -14, 690), 34, scale=(1, 0.2, 1), steps=(3, 6))
    f.sphere(GOLD, (0, -13, 560), 22, scale=(1, 0.2, 1), steps=(3, 6))
    for sx in (-48, 48):
        f.box(PINK, (sx, -8, 378), (54, 6, 60))
    s = sc.part(P.motion(swing=18, pivot=(0, 0, 925), axis=P.UP, rate=0.3, phase=0.1))
    s.box(PINK, (120, 0, 925), (220, 5, 26))
    s.box(LAV, (150, 0, 898), (180, 5, 18))


def amphora_stall(sc):
    """TALL (3.2 m). A merchant's stall: a striped awning that stirs over a counter of jars and dishes of pearls."""
    g = sc.still()
    g.box(ROSE, (0, 0, -10), (460, 320, 60))                    # -40..20
    for sx in (-205, 205):
        for sy, h in ((-135, 240), (135, 290)):
            g.cyl(CREAM, (sx, sy, 20), 9, h, steps=6)
    a = sc.part(P.motion(swing=2.5, pivot=(0, 150, 312), axis=0.0, rate=0.35, phase=0.3))
    for k in range(5):
        x = -184 + 92 * k
        c = PINK if k % 2 == 0 else CREAM
        tilt_box(a, c, (x, 160, 314), (x, -165, 262), 92, 8)
        a.box(GOLD if k % 2 == 0 else PINK, (x, -168, 240), (92, 6, 40))
    g.box(ROSE, (0, -60, 65), (370, 84, 90))                    # the counter 20..110
    g.box(CREAM, (0, -60, 116), (390, 100, 12))
    for x, c in ((-140, CREAM), (-70, ROSE), (10, PEARL)):
        g.cyl(c, (x, -60, 122), 14, 30, steps=6, top_r=26)
        g.cyl(c, (x, -60, 152), 26, 22, steps=6, top_r=11)
    g.cyl(PINK, (120, -60, 122), 34, 8, steps=8)
    for dx, dy in ((-12, -6), (12, -4), (0, 12)):
        g.sphere(PEARL, (120 + dx, -60 + dy, 138), 10, steps=(3, 5), glow=0.4)
    for sx, c in ((-165, ROSE), (165, CREAM)):                  # two tall jars on the floor
        g.cyl(c, (sx, 70, 20), 18, 70, steps=8, top_r=46)
        g.cyl(c, (sx, 70, 90), 46, 50, steps=8, top_r=20)
        g.cyl(c, (sx, 70, 140), 16, 26, steps=8, top_r=22)
    g.box(CREAM, (0, 128, 150), (360, 30, 12))                  # a shelf at the back
    for x, c in ((-110, PINK), (-20, LAV), (80, PINK)):
        g.box(c, (x, 128, 172), (60, 26, 32))


# ---------------------------------------------------------------------------------------------- the hero
def great_pearl(sc):
    """THE GREAT PEARL: a stepped pyramid 70 x 60 m and 35 m tall carrying a giant open shell, in it a pearl 12 m
    across with a steady soft glow; 55.5 m in all over its footing, wider than tall. Four pennants turn slowly."""
    g = sc.still()
    tiers = ((7000, 6000, -500, 700, ROSE), (5800, 5000, 700, 1400, CREAM), (4600, 4000, 1400, 2100, ROSE),
             (3400, 3000, 2100, 2800, CREAM), (2200, 2000, 2800, 3500, ROSE))
    for i, (w, d, z0, z1, c) in enumerate(tiers):
        g.box(c, (0, 0, (z0 + z1) / 2.0), (w, d, z1 - z0))
        g.box(CREAM if c == ROSE else ROSE, (0, 0, z1 - 30), (w + 80, d + 80, 60))      # the lip
        g.box(PLUM, (0, 0, z1 - 85), (w + 24, d + 24, 50))                                # its shadow
        zf = max(z0, 0.0)
        if i % 2 == 0:                                           # niches in shadow
            x = 800.0
            while x < w / 2.0 - 250:
                for sx in (-1, 1):
                    g.box(PLUM, (sx * x, -d / 2.0 - 3, zf + 300), (300, 14, 400))
                x += 600.0
            for sx in (-1, 1):
                for y in (-d / 4.0, 0.0, d / 4.0):
                    g.box(PLUM, (sx * (w / 2.0 + 3), y, zf + 300), (14, 300, 400))
        elif i < 4:                                              # a colonnade
            x = 800.0
            while x < w / 2.0 - 200:
                for sx in (-1, 1):
                    g.cyl(CREAM if c == ROSE else ROSE, (sx * x, -d / 2.0 - 30, zf), 70, 560, steps=6, top_r=60)
                    g.box(GOLD, (sx * x, -d / 2.0 - 30, zf + 585), (190, 190, 50))
                x += 550.0
        if i < 4:                                                # pearls on gold cups at the front corners
            for sx in (-1, 1):
                for sy in ((-1, 1) if i == 0 else (-1,)):
                    cx, cy = sx * (w / 2.0 - 160), sy * (d / 2.0 - 160)
                    g.cyl(GOLD, (cx, cy, z1), 40, 160, steps=6, top_r=75)
                    g.sphere(PEARL, (cx, cy, z1 + 255), 100, steps=(4, 8), glow=0.8)
    for k in range(10):                                          # the great stair up the front
        top, yf = 350.0 * (k + 1), -3500.0 + 250.0 * k
        g.box(CREAM, (0, (yf - 900.0) / 2.0, (top - 500.0) / 2.0), (1000, -900.0 - yf, top + 500.0))
    for sx in (-570, 570):
        tilt_box(g, ROSE, (sx, -3520, 150), (sx, -1100, 3560), 140, 220)
    for sx in (-2900, 2900):                                     # two domed kiosks on the first terrace
        g.box(CREAM, (sx, -2750, 850), (320, 320, 300))
        g.box(PLUM, (sx, -2913, 830), (130, 10, 200))
        dome(g, sx, -2750, 1000, 190, steps=(4, 8))
    g.cyl(GOLD, (0, 0, 3500), 520, 80, steps=12)                # the shell's foot
    g.cyl(PINK, (0, 0, 3580), 380, 420, steps=12, top_r=1350)   # the lower shell, to 4000
    g.cyl(GOLD, (0, 0, 3990), 1375, 22, steps=12)
    g.cyl(CREAM, (0, 0, 4004), 1280, 16, steps=12)              # its lining
    g.sphere(PEARL, (0, 0, 4560), 600, steps=(8, 12), glow=0.85)    # the pearl: 3960..5160
    for k in range(9):                                           # the upper shell: a standing fan of nine ribs
        a = math.radians(-72 + 18 * k)
        tip = (math.sin(a) * 1560, 760, 3900 + math.cos(a) * 1560)
        tilt_box(g, PINK if k % 2 == 0 else ROSE, (0, 760, 3900), tip, 90, 500)
        g.sphere(GOLD, tip, 95, steps=(3, 6))
    g.box(GOLD, (0, 760, 3920), (700, 130, 260))                # the hinge
    for i, (sx, sy) in enumerate(((-950, -850), (950, -850), (-950, 850), (950, 850))):
        g.cyl(CREAM, (sx, sy, 3500), 30, 1100, steps=6, top_r=22)
        g.sphere(GOLD, (sx, sy, 4630), 46, steps=(3, 6))
        f = sc.part(P.motion(swing=12, pivot=(sx, sy, 4500), axis=P.UP, rate=0.12, phase=0.25 * i))
        d = 1 if sx > 0 else -1
        f.box(PINK, (sx + d * 360, sy, 4500), (720, 20, 220))
        f.box(GOLD, (sx + d * 740, sy, 4500), (50, 22, 240))


JOBS = [
    ('SM_S1D5_Hall', hall), ('SM_S1D5_BathHouse', bath), ('SM_S1D5_StepTemple', temple),
    ('SM_S1D5_ShopArcade', arcade), ('SM_S1D5_Treasury', treasury), ('SM_S1D5_ArchGate', arch_gate),
    ('SM_S1D5_TownHouse', house),
    ('SM_S1D5_BrokenColumn', broken_column, STILL), ('SM_S1D5_BenchUrn', bench_urn, SMALL),
    ('SM_S1D5_PlanterRail', planter_rail, SMALL), ('SM_S1D5_PearlLamp', pearl_lamp, STILL),
    ('SM_S1D5_Seahorse', seahorse, STILL), ('SM_S1D5_Fountain', fountain), ('SM_S1D5_BannerMast', banner_mast),
    ('SM_S1D5_AmphoraStall', amphora_stall),
    ('SM_S1D5_GreatPearl', great_pearl, dict(grow=(200, 200, 60), grow_down=(200, 200, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
