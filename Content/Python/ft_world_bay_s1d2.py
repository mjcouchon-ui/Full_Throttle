"""Bayshore, Stage 1, builder s1d2: district 2 "The Marina" (3.2 to 6.4 km). /Game/World/Bayshore/SM_S1D2_*.
Run: import ft_world_bay_s1d2 as K; K.run()

Nautical timber: white clapboard in vertical boards (sky-blue battens), steep 45 degree gabled roofs in harbour blue,
dormers, a cupola, a widow's walk, verandas, boat sheds. No pink anywhere. Seven buildings, seven props and the hero,
THE CLIPPER: a three-masted ship hauled out on a timber cradle as the yacht club's house.
Every piece is an ft_people.Scene: one material (M_People), one section. cm; z = 0 is the land; FRONT faces -Y.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Bayshore'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

W, BLUE, SKY, WOOD, NAVY = '#fffaf2', '#4a7fc0', '#8fe3ff', '#c9a06a', '#2b2350'
WOOD_D, CREAM, STONE, YEL = '#9a744a', '#fff1d6', '#b8b4c8', '#ffe14d'
FOOT = 300.0
R2 = math.sqrt(2.0)
FLAGS = [BLUE, YEL, SKY, W, NAVY]


# ------------------------------------------------------------------------------------------ helpers
def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def walls(g, cx, cy, w, d, h, col=W, n=None, bcol=SKY):
    """Clapboard walls from the footing up to h, and the boards' battens on the front."""
    g.box(col, (cx, cy, (h - FOOT) / 2.0), (w, d, h + FOOT))
    n = n or max(2, int(round(w / 140.0)))
    for k in range(n + 1):
        g.box(bcol, (cx - w / 2.0 + k * w / float(n), cy - d / 2.0 - 2.0, h / 2.0 + 15.0), (9, 8, h - 30.0))


def roof_x(g, cx, cy, w, d, h, over=40.0, col=BLUE, fill=W, t=16.0):
    """A 45 degree gabled roof, ridge along X (the eaves face the road). Ridge at h + d / 2."""
    s2 = d / R2
    g.box(fill, (cx, cy, h), (w, s2, s2), rot=(0, 0, 45))
    hd = d / 2.0 + over
    for s in (-1, 1):
        g.box(col, (cx, cy + s * (hd / 2.0 + t * 0.35), h + (d / 2.0 - over) / 2.0 + t * 0.35), (w + 2 * over, hd * R2, t), rot=(0, 0, s * 45))
    g.box(col, (cx, cy, h + d / 2.0 + t * 0.6), (w + 2 * over, t * 1.8, t * 1.2))


def roof_y(g, cx, cy, w, d, h, over=40.0, col=BLUE, fill=W, t=16.0):
    """A 45 degree gabled roof, ridge along Y (the gable faces the road). Ridge at h + w / 2."""
    s2 = w / R2
    g.box(fill, (cx, cy, h), (s2, d, s2), rot=(45, 0, 0))
    hw = w / 2.0 + over
    for s in (-1, 1):
        g.box(col, (cx + s * (hw / 2.0 + t * 0.35), cy, h + (w / 2.0 - over) / 2.0 + t * 0.35), (hw * R2, d + 2 * over, t), rot=(-s * 45, 0, 0))
    g.box(col, (cx, cy, h + w / 2.0 + t * 0.6), (t * 1.8, d + 2 * over, t * 1.2))


def win(g, x, yf, z, w=90.0, h=130.0, shut=SKY):
    """A window on a front wall at y = yf: a dark pane and two shutters."""
    g.box(NAVY, (x, yf - 3.0, z), (w, 8, h))
    if shut:
        for s in (-1, 1):
            g.box(shut, (x + s * (w / 2.0 + 14.0), yf - 3.0, z), (24, 7, h))


def dormer(g, x, y, z, w=150.0, d=180.0, h=130.0):
    """A dormer standing on a front roof slope: its floor at z, its face at y - d / 2."""
    g.box(W, (x, y, z + h / 2.0 - 30.0), (w, d, h + 60.0))
    roof_y(g, x, y, w, d, z + h, over=14.0, t=10.0)
    g.box(NAVY, (x, y - d / 2.0 - 3.0, z + h / 2.0 + 5.0), (w * 0.56, 7, h * 0.66))


def pennant(sc, x, y, z, col, length=160.0, hgt=50.0, swing=16.0, rate=0.35, phase=0.0):
    """A pennant in three tapering lengths, swinging about the pole at (x, y)."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    for j in range(3):
        f.box(col, (x + 6.0 + length * (j + 0.5) / 3.0, y, z), (length / 3.0, 5, hgt * (1.0 - j * 0.27)))


def hull(L, B, bow, stern=0.55, cx=0.0):
    """A boat seen from above: bow towards +X."""
    h = B / 2.0
    return [(cx - L / 2.0, -h * stern), (cx - L / 2.0 + L * 0.1, -h), (cx + L / 2.0 - bow, -h), (cx + L / 2.0 - bow * 0.4, -h * 0.62),
            (cx + L / 2.0, 0.0), (cx + L / 2.0 - bow * 0.4, h * 0.62), (cx + L / 2.0 - bow, h), (cx - L / 2.0 + L * 0.1, h), (cx - L / 2.0, h * stern)]


def life_ring(g, x, y, z, r=32.0):
    """A life ring flat on a front wall or a post: white, four blue bands, a dark middle."""
    rod(g, W, (x, y, z), (x, y - 8.0, z), r, steps=8)
    rod(g, NAVY, (x, y - 8.0, z), (x, y - 9.5, z), r * 0.45, steps=8)
    for dx, dz in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        g.box(BLUE, (x + dx * r * 0.74, y - 5.0, z + dz * r * 0.74), (r * 0.5, 10, r * 0.5))


def boat_vane(sc, x, y, z, rate=0.15, phase=0.0):
    """A weather vane: a small boat under sail, turning slowly on its rod."""
    g = sc.still()
    g.cyl(WOOD_D, (x, y, z), 5, 110, steps=6)
    v = sc.part(P.motion(swing=24, pivot=(x, y, z + 110), axis=P.UP, rate=rate, phase=phase))
    v.side(BLUE, _ccw([(-58, 0), (58, 0), (40, -18), (-42, -18)]), -4, 4, loc=(x, y, z + 128))
    v.side(W, _ccw([(-6, 6), (48, 6), (-6, 84)]), -4, 4, loc=(x, y, z + 128))


def rope(g, a, b, sag=24.0, w=7.0, col=CREAM):
    """A rope from a to b that sags in the middle (two lengths)."""
    m = ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0 - sag)
    tilt_box(g, col, a, m, w, w)
    tilt_box(g, col, m, b, w, w)


# ------------------------------------------------------------------------------------------ buildings
def harbour_office(sc):
    """The harbour master's office: 9 x 7 m, two storeys, a look-out cupola on the ridge, a veranda, and a signal
    mast at its corner with code flags on the yard (they sway) and a pennant (it swings)."""
    g = sc.still()
    walls(g, 0, 0, 900, 700, 640)
    roof_x(g, 0, 0, 900, 700, 640)                                       # ridge 990
    g.box(WOOD, (0, -450, -120), (940, 200, 360))                        # the veranda's deck (top 60)
    for x in (-450, -150, 150, 450):
        g.box(W, (x, -535, 195), (14, 14, 270))
    g.box(BLUE, (0, -455, 348), (990, 250, 14), rot=(0, 0, -12))
    g.box(W, (-300, -540, 150), (300, 8, 8)); g.box(W, (300, -540, 150), (300, 8, 8))
    g.box(WOOD_D, (0, -570, 15), (200, 60, 30))                          # the step
    g.box(BLUE, (0, -353, 165), (110, 8, 210))                           # the door
    g.box(W, (0, -356, 280), (130, 6, 16))
    for x in (-270, 270):
        win(g, x, -350, 190)
    for x in (-270, 270):
        win(g, x, -350, 500)
    rod(g, W, (0, -350, 500), (0, -358, 500), 62, steps=8)               # the tide clock between the upper windows
    rod(g, SKY, (0, -358, 500), (0, -360, 500), 50, steps=8)
    g.box(NAVY, (0, -362, 518), (7, 4, 44)); g.box(NAVY, (14, -362, 500), (34, 4, 7))
    life_ring(g, -130, -350, 200, 30)
    g.box(SKY, (130, -470, 85), (120, 40, 10)); g.box(SKY, (130, -452, 110), (120, 8, 50))     # a bench on the veranda
    g.cyl(CREAM, (-360, -470, 60), 36, 20, steps=8); g.cyl(CREAM, (-360, -470, 80), 30, 18, steps=8)   # coiled rope
    # the cupola
    g.cyl(W, (0, 0, 940), 125, 200, steps=8)
    g.cyl(SKY, (0, 0, 1010), 129, 74, steps=8)
    g.cyl(BLUE, (0, 0, 1140), 170, 150, steps=8, top_r=12)
    g.sphere(YEL, (0, 0, 1300), 18, steps=(3, 6))
    boat_vane(sc, 0, 0, 1310, phase=0.2)
    g.box(STONE, (-330, 120, 900), (90, 90, 320))                        # a chimney
    g.box(W, (-330, 120, 1066), (110, 110, 14))
    # the signal mast at the corner
    mx, my = 560, -250
    g.cyl(STONE, (mx, my, -150), 60, 210, steps=8)
    g.cyl(W, (mx, my, 60), 12, 1440, steps=6, top_r=7)
    g.box(WOOD, (mx, my, 1150), (420, 9, 9))
    g.sphere(YEL, (mx, my, 1508), 14, steps=(3, 6))
    pennant(sc, mx, my, 1460, BLUE, length=190, hgt=54, phase=0.6)
    for k, (sx, cols) in enumerate(((-1, (BLUE, YEL, SKY)), (1, (W, BLUE, YEL)))):
        f = sc.part(P.motion(swing=6, pivot=(mx + sx * 190, my, 1150), axis=90.0, rate=0.3, phase=0.35 * k))
        f.box(CREAM, (mx + sx * 190, my, 1020), (5, 5, 260))
        for j, c in enumerate(cols):
            f.box(c, (mx + sx * 190, my - 1, 1092 - j * 74), (70, 6, 56))


def sail_loft(sc):
    """The sail loft: 7 x 9 m, tall, its gable to the road, a hoist beam under the apex with a sail bundle on the
    fall (it sways), loft doors, an outside stair, a sail drying on a rack beside it."""
    g = sc.still()
    walls(g, 0, 0, 700, 900, 900, n=5)
    roof_y(g, 0, 0, 700, 900, 900)                                       # ridge 1250
    g.box(BLUE, (0, -453, 150), (300, 8, 300))                           # the ground doors
    g.box(NAVY, (0, -458, 150), (8, 4, 300))
    g.box(W, (0, -456, 308), (330, 6, 16))
    g.box(BLUE, (0, -453, 660), (200, 8, 240))                           # the loft doors
    g.box(NAVY, (0, -458, 660), (8, 4, 240))
    g.box(W, (0, -456, 788), (230, 6, 16))
    g.box(WOOD, (0, -500, 535), (260, 100, 12))                          # the landing under them
    for x in (-250, 250):
        win(g, x, -450, 660, w=70, h=110)
        win(g, x, -450, 200, w=70, h=110)
    rod(g, W, (0, -450, 1010), (0, -457, 1010), 58, steps=8)             # a porthole in the gable
    rod(g, NAVY, (0, -457, 1010), (0, -460, 1010), 42, steps=8)
    g.box(WOOD_D, (0, -590, 1120), (28, 320, 32))                        # the hoist beam
    g.box(WOOD_D, (0, -735, 1098), (40, 30, 20))
    f = sc.part(P.motion(swing=5, pivot=(0, -735, 1090), axis=90.0, rate=0.25, phase=0.1))
    f.box(CREAM, (0, -735, 960), (6, 6, 260))
    f.cyl(CREAM, (0, -735, 700), 44, 140, steps=8)
    f.cyl(BLUE, (0, -735, 755), 47, 24, steps=8)
    f.box(NAVY, (0, -735, 846), (20, 12, 20))
    # the outside stair up the left wall
    tilt_box(g, WOOD, (-420, -420, -20), (-420, 160, 470), 100, 14)
    g.box(WOOD, (-420, 250, 470), (110, 200, 14))
    for y in (-420, -130, 160, 340):
        g.box(WOOD_D, (-466, y, 120 + (y + 420) * 0.25), (10, 10, 480))
    tilt_box(g, W, (-468, -420, 80), (-468, 160, 570), 8, 8)
    # the drying rack on the right
    g.box(WOOD_D, (430, -300, 180), (12, 12, 760))
    g.box(WOOD_D, (800, -300, -40), (12, 12, 320))
    tilt_box(g, WOOD_D, (430, -300, 60), (800, -300, 60), 8, 8)
    g.side(CREAM, _ccw([(436, 70), (800, 70), (436, 540)]), -304, -296)
    g.box(BLUE, (470, -306, 300), (40, 5, 60))
    g.cyl(W, (0, 380, 1250), 7, 260, steps=6)
    pennant(sc, 0, 380, 1480, SKY, length=170, hgt=52, phase=0.4)


def chandlery(sc):
    """The chandlery: a long low shop, 13 x 6.5 m, two dormers, a veranda the whole front under a blue and white
    awning, coiled rope, crates, oars and an anchor on it, a fish board swinging from the awning."""
    g = sc.still()
    walls(g, 0, 0, 1300, 650, 380, n=10)
    roof_x(g, 0, 0, 1300, 650, 380)                                      # ridge 705
    for x in (-330, 330):
        dormer(g, x, -200, 505)
    g.box(WOOD, (0, -435, -120), (1340, 220, 360))                       # the veranda's deck (top 60)
    for k in range(5):
        g.box(W, (-650 + k * 325, -530, 195), (14, 14, 270))
    for k in range(8):
        g.box(BLUE if k % 2 == 0 else W, (-585 + k * 167.1, -445, 340), (167.1, 262, 12), rot=(0, 0, -14))
    g.box(BLUE, (0, -328, 165), (110, 8, 210))                           # the door
    for x in (-300, 300):
        g.box(NAVY, (x, -328, 190), (210, 8, 150))
        g.box(W, (x, -331, 190), (8, 6, 150))
        g.box(SKY, (x, -330, 100), (230, 10, 12))
    for x in (-540, 540):
        win(g, x, -325, 200, w=70, h=110, shut=None)
    for j in range(3):
        g.cyl(CREAM, (-545, -470, 60 + j * 21), 40 - j * 4, 21, steps=8)        # coiled rope
    g.box(W, (-420, -470, 88), (56, 56, 56)); g.box(BLUE, (-362, -475, 84), (48, 48, 48)); g.box(SKY, (-395, -470, 140), (50, 50, 46))
    tilt_box(g, NAVY, (425, -400, 62), (470, -372, 250), 16, 16)         # an anchor leaning on the wall
    g.box(NAVY, (436, -396, 76), (110, 14, 16)); g.box(WOOD_D, (466, -374, 232), (90, 12, 12))
    for k in range(2):
        tilt_box(g, WOOD, (560 + k * 34, -400, 62), (590 + k * 34, -345, 330), 9, 9)   # oars
        g.box(SKY if k else W, (592 + k * 34, -342, 336), (22, 8, 76))
    life_ring(g, 150, -326, 250, 30)
    f = sc.part(P.motion(swing=7, pivot=(0, -566, 318), axis=0.0, rate=0.4, phase=0.2))   # the fish board
    f.box(SKY, (0, -566, 262), (170, 7, 70))
    f.box(W, (-10, -571, 262), (96, 4, 30)); f.box(W, (52, -571, 262), (22, 4, 44))
    f.box(CREAM, (-70, -566, 306), (5, 5, 24)); f.box(CREAM, (70, -566, 306), (5, 5, 24))
    g.box(STONE, (480, 120, 620), (90, 90, 330))
    g.box(W, (480, 120, 792), (110, 110, 14))
    g.cyl(W, (-600, 0, 705), 7, 230, steps=6)
    pennant(sc, -600, 0, 905, YEL, length=150, hgt=46, phase=0.75)


def boat_shed(sc):
    """A boat shed: 11 x 9 m, its gable to the road, the big doors slid open and a hull on the slip poking out, a
    lean-to of racked dinghies at the side, a boat for a weather vane (it turns)."""
    g = sc.still()
    walls(g, 0, 0, 1100, 900, 480, n=8)
    roof_y(g, 0, 0, 1100, 900, 480)                                      # ridge 1030
    g.box(NAVY, (0, -453, 200), (560, 8, 400))
    for s in (-1, 1):
        g.box(BLUE, (s * 418, -458, 200), (262, 10, 400))
        g.box(W, (s * 418, -464, 200), (262, 4, 14)); tilt_box(g, W, (s * 418 - 120, -464, 20), (s * 418 + 120, -464, 380), 6, 12)
    g.box(WOOD_D, (0, -462, 412), (1090, 12, 18))
    rod(g, W, (0, -450, 740), (0, -457, 740), 70, steps=8)               # the porthole in the gable
    rod(g, NAVY, (0, -457, 740), (0, -460, 740), 50, steps=8)
    g.box(W, (0, -462, 740), (100, 5, 8)); g.box(W, (0, -462, 740), (8, 5, 100))
    # the hull on the slip, bow to the road
    yaw = (0, -90, 0)
    g.plan(W, hull(640, 215, 230), 95, 195, loc=(0, -580, 0), rot=yaw)
    g.plan(BLUE, hull(656, 228, 236), 195, 222, loc=(0, -580, 0), rot=yaw)
    g.plan(WOOD, hull(600, 180, 216), 222, 230, loc=(0, -580, 0), rot=yaw)
    g.box(NAVY, (0, -580, 78), (16, 520, 40))
    for y in (-420, -720):
        g.box(WOOD_D, (0, y, 20), (250, 30, 150))
    for s in (-1, 1):
        tilt_box(g, WOOD_D, (s * 80, -430, 40), (s * 80, -1010, -50), 18, 22)
    for k, c in enumerate((W, BLUE, W)):                                  # mooring buoys hung on the wall
        g.sphere(c, (-495, -462, 150 + k * 62), 26, steps=(3, 6))
    # the lean-to
    g.box(BLUE, (690, 0, 335), (290, 620, 12), rot=(-15, 0, 0))
    for y in (-280, 280):
        g.box(W, (818, y, 60), (12, 12, 500))
    for k, c in enumerate((W, SKY)):
        g.plan(c, hull(430, 112, 150), 70 + k * 95, 118 + k * 95, loc=(690, 0, 0), rot=yaw)
        g.box(WOOD_D, (690, -120, 58 + k * 95), (200, 12, 12)); g.box(WOOD_D, (690, 120, 58 + k * 95), (200, 12, 12))
    for y in (-120, 120):
        g.box(WOOD_D, (600, y, 60), (10, 10, 300)); g.box(WOOD_D, (780, y, 60), (10, 10, 300))
    boat_vane(sc, 0, -440, 1040, rate=0.14, phase=0.5)


def club_house(sc):
    """The yacht club's shore house: 12 x 8 m, two storeys, a look-out deck across the front under an awning, a
    widow's walk on the ridge with the club's flagpole and yardarm (pennant and burgees swing), bunting."""
    g = sc.still()
    walls(g, 0, 0, 1200, 800, 620, n=8)
    roof_x(g, 0, 0, 1200, 800, 620)                                      # ridge 1020
    # the widow's walk
    g.box(WOOD, (0, 0, 1034), (540, 250, 16))
    for x in (-262, -87, 87, 262):
        for y in (-118, 118):
            g.box(W, (x, y, 1088), (10, 10, 92))
    for y in (-118, 118):
        g.box(W, (0, y, 1134), (540, 9, 9)); g.box(SKY, (0, y, 1086), (520, 5, 40))
    for x in (-262, 262):
        g.box(W, (x, 0, 1134), (9, 240, 9))
    tilt_box(g, NAVY, (150, -60, 1100), (215, -112, 1128), 14, 14)       # a telescope on its post
    g.box(WOOD_D, (182, -86, 1078), (8, 8, 72))
    g.cyl(W, (0, 0, 1042), 10, 540, steps=6, top_r=6)
    g.box(W, (0, 0, 1400), (320, 8, 8))
    g.sphere(YEL, (0, 0, 1590), 14, steps=(3, 6))
    pennant(sc, 0, 0, 1545, BLUE, length=230, hgt=60, phase=0.1)
    for k, (sx, c) in enumerate(((-1, YEL), (1, SKY))):
        f = sc.part(P.motion(swing=8, pivot=(sx * 150, 0, 1400), axis=90.0, rate=0.33, phase=0.3 + 0.4 * k))
        f.box(CREAM, (sx * 150, 0, 1350), (5, 5, 100))
        f.box(c, (sx * 150, -1, 1330), (74, 6, 60)); f.box(NAVY if c == YEL else W, (sx * 150, -4, 1330), (30, 4, 24))
    # the look-out deck
    g.box(WOOD, (0, -520, 320), (1240, 240, 18))
    for k in range(5):
        x = -610 + k * 305
        g.box(W, (x, -630, 10), (16, 16, 620))
        g.box(W, (x, -630, 510), (10, 10, 380))
    g.box(W, (0, -636, 422), (1240, 9, 9))
    for k in range(4):
        g.box(SKY, (-457.5 + k * 305, -636, 372), (270, 5, 62))
    for k in range(6):
        g.box(BLUE if k % 2 == 0 else W, (-516.7 + k * 206.7, -520, 668), (206.7, 262, 12), rot=(0, 0, -12))
    tilt_box(g, WOOD, (930, -520, -30), (640, -520, 318), 110, 14)       # the stair up to it, along the front
    tilt_box(g, W, (930, -572, 60), (640, -572, 408), 8, 8)
    for k in range(10):                                                  # bunting under the deck's edge
        g.box(FLAGS[k % 4], (-540 + k * 120, -642, 286), (38, 4, 38), rot=(45, 0, 0))
    for x in (-360, 0, 360):                                             # upper: three glazed doors
        g.box(NAVY, (x, -403, 440), (130, 8, 220)); g.box(W, (x, -407, 440), (7, 5, 220))
    for x in (-610, 610):
        g.box(YEL, (x, -630, 716), (26, 26, 34), glow=0.5)               # two lanterns (steady)
    g.box(BLUE, (0, -403, 165), (130, 8, 220))
    for x in (-380, 380):
        win(g, x, -400, 180, w=170, h=150)
    life_ring(g, -190, -400, 190, 30)
    g.box(STONE, (430, 150, 880), (100, 100, 360)); g.box(W, (430, 150, 1066), (122, 122, 14))


def smokehouse(sc):
    """The smokehouse: 7 x 7 m, its gable to the road, slatted vents, a tall white chimney with a fish for a vane
    (it turns), racks of fish drying beside it (they sway), split logs."""
    g = sc.still()
    walls(g, 0, 0, 700, 700, 420, n=5)
    roof_y(g, 0, 0, 700, 700, 420)                                       # ridge 770
    for k in range(3):
        g.box(NAVY, (0, -353, 545 + k * 34), (170 - k * 44, 7, 16))
    g.box(BLUE, (0, -353, 135), (130, 8, 250))
    tilt_box(g, W, (-55, -358, 20), (55, -358, 250), 5, 12); g.box(W, (0, -358, 262), (150, 5, 14))
    win(g, -220, -350, 200, w=70, h=100)
    cx, cy = -240, 190
    g.box(STONE, (cx, cy, 60), (200, 200, 720))
    g.box(W, (cx, cy, 800), (150, 150, 900))                              # the chimney: to 1250
    g.box(BLUE, (cx, cy, 1160), (166, 166, 44))
    g.box(W, (cx, cy, 1260), (186, 186, 22))
    g.cyl(WOOD_D, (cx, cy, 1270), 5, 120, steps=6)
    v = sc.part(P.motion(swing=28, pivot=(cx, cy, 1400), axis=P.UP, rate=0.16, phase=0.3))
    v.side(BLUE, _ccw([(-60, 0), (-20, -24), (42, -15), (74, 0), (42, 15), (-20, 24)]), -5, 5, loc=(cx, cy, 1410))
    v.side(BLUE, _ccw([(-56, 0), (-98, -26), (-98, 26)]), -5, 5, loc=(cx, cy, 1410))
    # the drying racks
    for x in (420, 780):
        g.box(WOOD_D, (x, -280, 60), (12, 12, 420))
    for j, z in enumerate((262, 170)):
        g.box(WOOD_D, (600, -280, z), (390, 10, 10))
        f = sc.part(P.motion(swing=6, pivot=(600, -280, z), axis=0.0, rate=0.4, phase=0.3 * j))
        for k in range(7 - j):
            f.box(CREAM if (k + j) % 2 else SKY, (455 + j * 24 + k * 48, -280, z - 42), (22, 9, 70))
    for j in range(3):                                                   # split logs, stacked
        for k in range(3 - j):
            g.box(WOOD if (j + k) % 2 else WOOD_D, (150 + k * 62 + j * 31, -385, 16 + j * 30), (56, 90, 28))
    g.box(W, (300, -390, 20), (70, 50, 46))                               # a fish basket
    g.box(SKY, (300, -390, 46), (60, 40, 8))


def pilot_cottage(sc):
    """The pilot's cottage: 8 x 6 m, one storey and two dormers, a gabled porch, a stone chimney with a boat vane,
    a picket fence, and a washing line at the side (the washing swings)."""
    g = sc.still()
    walls(g, 0, 0, 800, 600, 330, n=6)
    roof_x(g, 0, 0, 800, 600, 330)                                       # ridge 630
    for x in (-210, 210):
        dormer(g, x, -180, 458, w=140, d=170, h=120)
    g.box(WOOD, (0, -365, -125), (270, 150, 310))                        # the porch (top 30)
    for x in (-105, 105):
        g.box(W, (x, -425, 135), (12, 12, 210))
    roof_y(g, 0, -360, 250, 150, 240, over=12.0, t=10.0)
    g.box(BLUE, (0, -303, 135), (100, 8, 200))
    rod(g, NAVY, (0, -303, 190), (0, -309, 190), 16, steps=8)
    for x in (-270, 270):
        win(g, x, -300, 185, w=80, h=110)
        g.box(BLUE, (x, -312, 118), (130, 22, 18))                        # window boxes
    g.box(STONE, (300, 60, 520), (96, 96, 420)); g.box(W, (300, 60, 736), (116, 116, 14))
    boat_vane(sc, 300, 60, 743, rate=0.17, phase=0.8)
    for k in range(7):                                                   # the picket fence
        g.box(W, (-390 + k * 34, -460, 20), (12, 7, 150))
    g.box(W, (-288, -464, 60), (220, 5, 9))
    g.box(SKY, (-270, -325, 44), (120, 36, 10)); g.box(SKY, (-270, -308, 70), (120, 8, 44))   # a bench
    # the washing line
    for x in (470, 830):
        g.box(W, (x, -200, 60), (10, 10, 360))
    g.box(CREAM, (650, -200, 226), (360, 5, 5))
    f = sc.part(P.motion(swing=10, pivot=(650, -200, 226), axis=0.0, rate=0.45, phase=0.15))
    f.box(W, (520, -200, 192), (56, 5, 64)); f.box(BLUE, (520, -200, 196), (58, 6, 14))
    f.box(SKY, (600, -200, 182), (74, 5, 84))
    f.box(BLUE, (682, -200, 198), (44, 5, 52))
    f.box(W, (752, -200, 186), (60, 5, 76)); f.box(NAVY, (752, -200, 200), (62, 6, 12)); f.box(NAVY, (752, -200, 172), (62, 6, 12))


# ------------------------------------------------------------------------------------------ props
def dinghies(sc):
    """LOW: two dinghies turned over on trestles for painting, a pair of oars, a paint pot."""
    g = sc.still()
    for k, (x, lo, hi, yw) in enumerate(((-200, W, BLUE, 5.0), (200, BLUE, W, -6.0))):
        rot = (0, yw, 0)
        g.plan(lo, hull(330, 130, 120), 76, 106, loc=(x, 0, 0), rot=rot)
        g.plan(hi, hull(292, 100, 104), 106, 129, loc=(x, 0, 0), rot=rot)
        g.plan(hi, hull(232, 56, 84), 129, 143, loc=(x, 0, 0), rot=rot)
        g.box(WOOD_D, (x - 10, 0, 149), (240, 8, 12), rot=rot)
        for tx in (x - 95, x + 80):
            g.box(WOOD, (tx, 0, 69), (18, 170, 14))
            for sy in (-1, 1):
                for sx in (-1, 1):
                    tilt_box(g, WOOD_D, (tx + sx * 26, sy * 76, -60), (tx, sy * 64, 64), 9, 9)
    for k in range(2):
        tilt_box(g, WOOD, (-150 + k * 20, -120, 10), (150 + k * 20, -112, 16), 8, 8)
        g.box(SKY, (182 + k * 20, -111, 17), (64, 20, 7))
    g.cyl(W, (0, -96, -20), 16, 44, steps=6); g.cyl(BLUE, (0, -96, 24), 17, 5, steps=6)
    g.box(WOOD_D, (4, -96, 40), (6, 6, 30), rot=(0, 0, 20))


def anchor(sc):
    """TALL (4 m): a great old anchor stood on a stone plinth, a plaque with no words, rope posts round it."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -150), 125, 250, steps=8)
    g.cyl(W, (0, 0, 100), 138, 18, steps=8)
    g.box(SKY, (0, -118, 50), (90, 8, 50)); g.box(W, (0, -122, 50), (60, 4, 8))
    g.box(NAVY, (0, 0, 255), (24, 24, 250))                               # the shank: 130 to 380
    g.box(WOOD_D, (0, 0, 356), (170, 18, 18), rot=(0, 25, 0))             # the stock
    g.box(NAVY, (0, 0, 398), (46, 12, 46), rot=(45, 0, 0))                # the ring
    for s in (-1, 1):
        tilt_box(g, NAVY, (0, 0, 138), (s * 78, 0, 150), 22, 22)
        tilt_box(g, NAVY, (s * 78, 0, 150), (s * 128, 0, 222), 20, 20)
        g.box(NAVY, (s * 132, 0, 232), (56, 14, 66), rot=(-s * 28, 0, 0))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(W, (sx * 180, sy * 180, -90), 9, 180, steps=6)
            g.cyl(BLUE, (sx * 180, sy * 180, 90), 11, 10, steps=6)
    for sx in (-1, 1):
        rope(g, (sx * 180, -180, 78), (sx * 180, 180, 78), sag=22)
    rope(g, (-180, 180, 78), (180, 180, 78), sag=22)


def lobster_pots(sc):
    """LOW (1.8 m): a stack of white and blue lobster pots on a pallet, marker buoys on their poles, coiled rope."""
    g = sc.still()
    g.box(WOOD, (0, 0, -25), (340, 120, 90))                              # the pallet (top 20)
    spots = [(-98, 20), (0, 20), (98, 20), (-49, 63), (49, 63), (0, 106)]
    for k, (x, z) in enumerate(spots):
        body = W if k % 2 == 0 else SKY
        g.box(body, (x, 0, z + 20), (88, 62, 40))
        for dx in (-34, 0, 34):
            g.box(BLUE if k % 3 else NAVY, (x + dx, 0, z + 21), (7, 66, 44))
    for k, (x, lean, c) in enumerate(((-190, -8, BLUE), (176, 6, YEL), (206, 16, SKY))):
        tilt_box(g, WOOD_D, (x, 30, -30), (x + lean, 30, 150), 6, 6)
        g.cyl(W, (x + lean * 0.55, 30, 70), 15, 42, steps=6)
        g.cyl(BLUE, (x + lean * 0.55, 30, 84), 16, 12, steps=6)
        g.box(c, (x + lean + 16, 30, 160), (30, 5, 22))
    for j in range(2):
        g.cyl(CREAM, (-150, -30, 20 + j * 16), 34 - j * 5, 16, steps=8)
    for k in range(5):                                                   # a string of net floats over the stack
        g.box(W if k % 2 else BLUE, (60 + k * 22, -34, 30 + (k % 2) * 4), (16, 16, 16))


def signal_mast(sc):
    """TALL (15 m): the harbour's signal mast on a stone drum: a yard with two hoists of code flags (they sway), a
    gaff with the harbour's plain flag and a pennant at the truck (they swing), a white time ball, four stays."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -150), 150, 230, steps=8)
    g.cyl(W, (0, 0, 80), 164, 16, steps=8)
    g.cyl(W, (0, 0, 80), 17, 900, steps=8, top_r=12)
    g.cyl(W, (0, 0, 940), 46, 12, steps=8)
    g.cyl(W, (0, 0, 900), 10, 570, steps=6, top_r=6)
    g.box(WOOD, (0, 0, 1040), (760, 10, 10))
    tilt_box(g, WOOD, (0, 0, 700), (270, 0, 905), 9, 9)
    g.sphere(YEL, (0, 0, 1482), 16, steps=(3, 6))
    g.sphere(W, (0, 0, 1230), 36, steps=(4, 8))
    g.cyl(BLUE, (0, 0, 1222), 37, 14, steps=8)
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, CREAM, (0, 0, 940), (sx * 300, sy * 300, -20), 6, 6)
    pennant(sc, 0, 0, 1440, SKY, length=230, hgt=58, phase=0.3)
    e = sc.part(P.motion(swing=14, pivot=(270, 0, 905), axis=P.UP, rate=0.3, phase=0.7))
    e.box(BLUE, (355, 0, 860), (160, 5, 96)); e.box(W, (355, -1, 860), (160, 6, 22))
    hoists = (((-340), ((BLUE, W), (YEL, None), (SKY, BLUE), (W, NAVY))), ((340), ((W, BLUE), (BLUE, None), (YEL, BLUE), (SKY, None))))
    for k, (x, cols) in enumerate(hoists):
        f = sc.part(P.motion(swing=7, pivot=(x, 0, 1040), axis=90.0, rate=0.28, phase=0.45 * k))
        f.box(CREAM, (x, 0, 880), (5, 5, 320))
        for j, (c, c2) in enumerate(cols):
            f.box(c, (x, -1, 985 - j * 76), (84, 6, 62))
            if c2:
                f.box(c2, (x, -3, 985 - j * 76), (40, 5, 28))


def rope_fence(sc):
    """LOW (1.1 m): eight metres of quay fence: white posts with blue caps, a sagging rope, two life rings."""
    g = sc.still()
    xs = [-400, -200, 0, 200, 400]
    for x in xs:
        g.cyl(W, (x, 0, -70), 9, 170, steps=6)
        g.cyl(BLUE, (x, 0, 100), 12, 10, steps=6)
    for a, b in zip(xs, xs[1:]):
        rope(g, (a, 0, 88), (b, 0, 88), sag=26)
    for x in (-200, 200):
        life_ring(g, x, -10, 56, 30)
    g.cyl(STONE, (-455, 0, -60), 20, 110, steps=6); g.cyl(NAVY, (-455, 0, 50), 26, 12, steps=6)   # a mooring bollard at one end


def ships_bell(sc):
    """TALL (3.9 m): a ship's bell hung in a timber frame under its own little roof, on a stone step (the bell
    swings slowly), a pennant on the roof."""
    g = sc.still()
    g.box(STONE, (0, 0, -55), (270, 170, 170))                            # top 30
    for x in (-95, 95):
        g.box(WOOD, (x, 0, 160), (18, 18, 260))
        tilt_box(g, WOOD_D, (x * 0.99, 0, 200), (x * 0.5, 0, 246), 10, 10)
    g.box(WOOD, (0, 0, 252), (250, 22, 20))
    roof_x(g, 0, 0, 250, 120, 290, over=14.0, t=10.0)                     # ridge 350
    g.box(SKY, (0, -86, 20), (80, 6, 40))
    b = sc.part(P.motion(swing=13, pivot=(0, 0, 244), axis=0.0, rate=0.3, phase=0.2))
    b.cyl(WOOD, (0, 0, 140), 62, 12, steps=8)
    b.cyl(WOOD, (0, 0, 152), 56, 66, steps=8, top_r=34)
    b.cyl(WOOD, (0, 0, 218), 34, 18, steps=8, top_r=14)
    b.box(WOOD_D, (0, 0, 238), (10, 10, 14))
    b.sphere(NAVY, (0, 0, 136), 11, steps=(3, 6))
    b.box(CREAM, (0, 0, 92), (5, 5, 80))
    g.cyl(W, (0, 0, 350), 5, 36, steps=6)
    pennant(sc, 0, 0, 376, BLUE, length=70, hgt=24, phase=0.55)


def dry_dinghy(sc):
    """TALL (5.6 m): a sailing dinghy on its launching trolley, mast up, the sail furled on the boom under a blue
    cover, a pennant at the masthead (it swings)."""
    g = sc.still()
    g.box(WOOD_D, (0, 0, 34), (30, 200, 14))
    for s in (-1, 1):
        rod(g, NAVY, (0, s * 98, 34), (0, s * 110, 34), 34, steps=8)
        rod(g, W, (0, s * 110, 34), (0, s * 112, 34), 12, steps=6)
        g.box(WOOD, (20, s * 42, 52), (380, 12, 12))
    g.box(WOOD_D, (245, 0, 20), (14, 14, 110))
    tilt_box(g, WOOD, (200, 0, 52), (300, 0, 40), 10, 10)
    g.plan(BLUE, hull(340, 92, 120), 58, 84)
    g.plan(W, hull(400, 150, 140), 84, 134)
    g.plan(SKY, hull(408, 158, 144), 134, 143)
    g.plan(WOOD, hull(372, 126, 132), 143, 148)
    g.box(W, (-204, 0, 96), (8, 12, 96)); tilt_box(g, WOOD_D, (-198, 0, 150), (-120, 0, 162), 6, 6)   # rudder and tiller
    g.cyl(W, (60, 0, 148), 7, 420, steps=6, top_r=4)
    g.box(WOOD, (-52, 0, 226), (236, 8, 8))
    g.box(CREAM, (-56, 0, 210), (212, 18, 24))
    for x in (-120, -20):
        g.box(BLUE, (x, 0, 210), (56, 20, 26))
    tilt_box(g, CREAM, (196, 0, 148), (60, 0, 548), 5, 5)
    pennant(sc, 60, 0, 550, BLUE, length=72, hgt=26, phase=0.9)


# ------------------------------------------------------------------------------------------ the hero
def clipper(sc):
    """THE CLIPPER: a three-masted sailing ship hauled out on a timber cradle as the yacht club's house. Hull 45 m
    (white, a harbour-blue band), masts to 50 m with furled cream sails, dressed overall with flags, long pennants
    that swing at the three trucks, a veranda let into her side with a stair up to it, a club party on deck."""
    r = random.Random(52)
    g = sc.still()
    # ---- the cradle
    for x in (-1500, -750, 0, 750, 1400):
        g.box(WOOD_D, (x, 0, -125), (60, 1320, 300))
        g.box(WOOD, (x, 0, 100), (90, 150, 170))
        for s in (-1, 1):
            tilt_box(g, WOOD, (x, s * 620, -20), (x, s * 318, 480), 38, 38)
    for s in (-1, 1):
        g.box(WOOD_D, (-50, s * 620, 0), (3300, 60, 60))
    # ---- the hull
    g.box(NAVY, (-100, 0, 235), (3300, 50, 120))
    g.plan(NAVY, hull(3900, 540, 900), 280, 472)
    g.plan(W, hull(4250, 800, 1000), 470, 782)
    g.plan(BLUE, hull(4400, 880, 1050), 780, 852)
    g.plan(W, hull(4500, 860, 1080), 850, 960)
    g.plan(WOOD, hull(4380, 780, 1040), 958, 966)
    g.box(NAVY, (-2290, 0, 400), (150, 24, 340))                          # the rudder
    poop = [(-2250, -240), (-2025, -428), (-1250, -428), (-1250, 428), (-2025, 428), (-2250, 240)]
    g.plan(W, _ccw(poop), 960, 1112)
    g.plan(WOOD, _ccw([(x * 0.985 - 20, y * 0.93) for x, y in poop]), 1110, 1118)
    fore = [(1250, -400), (1780, -270), (1780, 270), (1250, 400)]
    g.plan(W, _ccw(fore), 960, 1052)
    g.plan(WOOD, _ccw([(x * 0.99, y * 0.92) for x, y in fore]), 1050, 1058)
    g.box(W, (-350, 0, 1066), (700, 360, 200)); g.box(BLUE, (-350, 0, 1174), (770, 430, 16))       # the deck houses
    g.box(SKY, (-350, 0, 1196), (300, 150, 30))
    g.box(W, (640, 0, 1050), (300, 280, 170)); g.box(BLUE, (640, 0, 1142), (350, 330, 14))
    for k in range(10):                                                  # portholes
        x = -1750 + k * 320
        if -700 < x < 180:
            continue
        g.box(NAVY, (x, -401, 650), (46, 8, 46))
    for y in (-150, 0, 150):                                             # the stern windows and the quarter lights (steady)
        g.box(YEL, (-2252, y, 1035), (8, 110, 84), glow=0.6)
    for x in (-2000, -1800, -1600, -1400):
        g.box(YEL, (x, -430, 1035), (120, 8, 80), glow=0.6)
    g.box(BLUE, (-1750, -431, 975), (900, 6, 16))
    for s in (-1, 1):                                                    # two stern lanterns
        g.box(W, (-2240, s * 200, 1160), (8, 8, 90)); g.box(YEL, (-2240, s * 200, 1222), (30, 30, 40), glow=0.7)
    # ---- the veranda let into her side, and the stair up to it
    g.box(NAVY, (-250, -404, 615), (780, 10, 260))
    g.box(BLUE, (-250, -410, 590), (96, 8, 196))
    for x in (-480, -20):
        g.box(YEL, (x, -410, 640), (120, 6, 110), glow=0.4)
    g.box(WOOD, (-250, -500, 480), (820, 200, 16))
    for k in range(4):
        x = -640 + k * 260
        g.box(W, (x, -592, 625), (14, 14, 280))
        tilt_box(g, WOOD_D, (x, -404, 380), (x, -590, 474), 14, 14)
    g.box(W, (-250, -594, 572), (820, 9, 9))
    for k in range(3):
        g.box(SKY, (-510 + k * 260, -594, 528), (230, 5, 56))
    for k in range(6):
        g.box(BLUE if k % 2 == 0 else W, (-591.7 + k * 136.7, -500, 772), (136.7, 226, 10), rot=(0, 0, -14))
    tilt_box(g, WOOD, (-1560, -520, -40), (-690, -520, 474), 120, 16)     # the stair, along her side
    tilt_box(g, W, (-1560, -578, 60), (-690, -578, 574), 9, 9)
    for k in range(4):
        x = -1500 + k * 262
        z = -40 + (x + 1560) * 0.591
        g.box(W, (x, -578, z + 50), (10, 10, 100))
        g.box(WOOD_D, (x, -470, z / 2.0 - 80), (14, 14, abs(z) + 180))
    # ---- masts, yards with furled sails, tops, shrouds
    masts = ((1150, 4600, (1500, 1250, 1000, 760), (1900, 2700, 3400, 3950)),
             (50, 5000, (1650, 1350, 1080, 800), (2000, 2900, 3700, 4350)),
             (-1150, 4300, (1250, 1050, 850, 640), (1800, 2550, 3200, 3700)))
    yaw = 20.0
    for mx, top, lens, zs in masts:
        g.cyl(WOOD, (mx, 0, 960), 36, top - 960, steps=8, top_r=12)
        zt = 960 + (top - 960) * 0.42
        g.cyl(W, (mx, 0, zt), 80, 16, steps=8)
        g.sphere(YEL, (mx, 0, top + 14), 20, steps=(3, 6))
        for ln, z in zip(lens, zs):
            g.box(WOOD_D, (mx, 0, z), (ln, 22, 22), rot=(0, yaw, 0))
            g.box(CREAM, (mx, 0, z - 36), (ln * 0.9, 36, 48), rot=(0, yaw, 0))
        for s in (-1, 1):
            for dx in (-110, 110):
                tilt_box(g, WOOD_D, (mx + dx, s * 415, 960), (mx, s * 30, zt), 8, 8)
    tilt_box(g, WOOD, (-1150, 0, 2300), (-1820, 0, 2720), 18, 18)         # the spanker's gaff and boom
    tilt_box(g, WOOD, (-1150, 0, 1270), (-2180, 0, 1350), 22, 22)
    tilt_box(g, CREAM, (-1200, 0, 1312), (-2100, 0, 1382), 40, 46)
    tilt_box(g, WOOD, (2050, 0, 1000), (2960, 0, 1330), 30, 30)           # the bowsprit
    g.box(NAVY, (1980, -300, 900), (120, 14, 90)); g.box(NAVY, (1980, -306, 900), (30, 8, 150))    # the bower anchor
    # ---- dressed overall: a line of flags from the bowsprit over the three trucks to the stern
    pts = [(2960, 0, 1330), (1150, 0, 4600), (50, 0, 5000), (-1150, 0, 4300), (-2240, 0, 1200)]
    n = 0
    for a, b in zip(pts, pts[1:]):
        tilt_box(g, CREAM, a, b, 7, 7)
        L = math.sqrt(sum((b[i] - a[i]) ** 2 for i in range(3)))
        cnt = int(L / 300.0)
        for k in range(cnt):
            t = (k + 0.5) / cnt
            g.box(FLAGS[n % 5], (a[0] + (b[0] - a[0]) * t, 0, a[2] + (b[2] - a[2]) * t - 44), (86, 5, 68))
            n += 1
    for k, (mx, top, col) in enumerate(((1150, 4600, SKY), (50, 5000, BLUE), (-1150, 4300, YEL))):
        pennant(sc, mx, 0, top - 30, col, length=720, hgt=96, swing=18, rate=0.2 + 0.03 * k, phase=0.33 * k)
    g.cyl(W, (-2236, 0, 1112), 7, 400, steps=6)                           # the ensign staff: the club's plain flag
    e = sc.part(P.motion(swing=14, pivot=(-2236, 0, 1400), axis=P.UP, rate=0.3, phase=0.5))
    e.box(BLUE, (-2380, 0, 1420), (280, 5, 160)); e.box(W, (-2380, -1, 1420), (280, 6, 34)); e.box(YEL, (-2290, -2, 1462), (50, 6, 40))
    # ---- the party: parasols and tables on the poop, members at the rail and on the veranda
    for k, (x, y, c) in enumerate(((-1500, -200, BLUE), (-1850, 150, W), (-1450, 220, SKY))):
        g.cyl(W, (x, y, 1118), 6, 250, steps=6)
        g.cyl(c, (x, y, 1340), 140, 44, steps=8, top_r=8)
        g.cyl(W, (x + 70, y - 40, 1118), 42, 74, steps=6)
    tops, bottoms = [W, BLUE, SKY, YEL, NAVY], [W, NAVY, BLUE, CREAM]
    who = [(-1960, -360, 1118), (-1700, -370, 1118), (-900, -335, 966), (350, -335, 966), (900, -330, 966), (1500, -250, 1058),
           (-420, -530, 488), (-100, -530, 488)]
    for k, (x, y, z) in enumerate(who):
        d = P.look(r, tops=tops, bottoms=bottoms, s=1.25)
        P.figure(sc, x, y, z, yaw=r.uniform(-20, 20), detail=0, arm_r='wave' if k % 2 == 0 else 'down', arm_l='wave' if k % 3 == 0 else 'hip',
                 swing_r=18 if k % 2 == 0 else 0.0, swing_l=14 if k % 3 == 0 else 0.0, rate=0.5 + 0.05 * (k % 3), phase=r.random(), **d)


JOBS = [('SM_S1D2_HarbourOffice', harbour_office),
        ('SM_S1D2_SailLoft', sail_loft),
        ('SM_S1D2_Chandlery', chandlery),
        ('SM_S1D2_BoatShed', boat_shed),
        ('SM_S1D2_ClubHouse', club_house),
        ('SM_S1D2_Smokehouse', smokehouse),
        ('SM_S1D2_PilotCottage', pilot_cottage),
        ('SM_S1D2_Dinghies', dinghies, STILL),
        ('SM_S1D2_Anchor', anchor, STILL),
        ('SM_S1D2_LobsterPots', lobster_pots, STILL),
        ('SM_S1D2_SignalMast', signal_mast),
        ('SM_S1D2_RopeFence', rope_fence, STILL),
        ('SM_S1D2_ShipsBell', ships_bell),
        ('SM_S1D2_DryDinghy', dry_dinghy),
        ('SM_S1D2_Clipper', clipper, dict(far=True, grow=(120, 260, 80), grow_down=(120, 260, 0)))]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
