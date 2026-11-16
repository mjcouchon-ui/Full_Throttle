"""Volcano, Stage 1, builder s1plants: what grows on Mount Kaldera, district by district.
/Game/World/Volcano/SM_S1PLANTS_*.   Run: import ft_world_volcano_s1plants as K; K.run()

Twenty-one plant pieces, every one an ft_people.Scene (one section, M_People), every one stirring: lime moss on
basalt lumps and silver ash ferns everywhere; horsetails, tree ferns and a lemon tree in a cobalt tub for the spa
town; a black glass thorn in magenta flower and fireweed for the storm stations; straw tussocks (one in a tin tub)
and a wind-bent pine, charred on its windward side (and its mirror twin for the left of the road), for the miners;
bamboo and a plum in white blossom for the temple; ash sunflowers, pink thrift and a pollard willow for the mills;
reeds round an outfall pool and birch saplings in banded tubs for the power station; a clipped box ball with an ivy
mound and an old chestnut in candle for the keep; a painted board palm in a tub on wheels and real dandelion clocks
for the backlot. Nothing glows. Nothing is orange or red, nothing is a green or orange box.

The lava channels must stay in sight from the road (the dressing's channel rule), so: solid things are 1.3 m or
lower; everything taller is a thin stem, or a crown that starts 3.3 m up or more on a thin trunk.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
LOWG = dict(grow=(20, 20, 10), grow_down=(20, 20, 0))      # a low piece: its parts stir a hand's width
MIDG = dict(grow=(35, 35, 20), grow_down=(35, 35, 0))      # stems up to 3 m
TREE = dict(grow=(60, 60, 30), grow_down=(60, 60, 0))

# greens that read on black ground, and the districts' own colours (the zone plan's palettes)
LIME, LIME_D, LIME_L = '#b6f03a', '#8fd43a', '#d8f07a'
FERN, FERN_L, SILV = '#7fb08c', '#a9c9a4', '#cfe0c8'
JADE, GREEN, GREEN_D, LEAF_L = '#2fae7a', '#5fbf6a', '#2f8f5a', '#9ad06a'
MAG, MAG_L, PINK, PINK_L = '#ff4fb8', '#ff9ad6', '#ff8fb1', '#ffc0d4'
LEMON, CREAMY, WHITE, BLUSH, CREAM = '#ffe45c', '#fff1a8', '#fff6f2', '#ffd9e6', '#fff1c9'
COBALT, MINT, TEAL, TURQ, NAVY = '#2456c8', '#7fe0b8', '#1f8a8a', '#19c5c0', '#1c2a4a'
STRAW, STRAW_D, STRAW_L = '#e8d59a', '#d8c27a', '#f4ecc0'
BARK, BARK_D, BARK_P, CHAR = '#5c3324', '#3b2a22', '#8a7a6a', '#1e1820'
BAS_L, ASHC, ASH_L, PALE_L = '#3e3244', '#5a4e5e', '#6f6272', '#b0a088'
GLASS, VIOLET, LILAC = '#14101a', '#7a4cff', '#b9b3d6'
TIN, SAND, CHALK, GRAPH, POOL = '#c9c4d6', '#e8d9b0', '#fbf7ee', '#3a4a66', '#bfeee6'
SKY_L, SKY_M, SKY_P = '#bfe6ee', '#9fd6c8', '#dff4e8'


# ------------------------------------------------------------------------------------------ helpers
def pol(base, yaw, pitch, length):
    """The point `length` from base along yaw (degrees in the ground plane) and pitch (degrees above level)."""
    a, p = math.radians(yaw), math.radians(pitch)
    return (base[0] + math.cos(a) * math.cos(p) * length, base[1] + math.sin(a) * math.cos(p) * length, base[2] + math.sin(p) * length)


def blade(m, col, base, yaw, pitch, length, w, t=5.0):
    """A flat leaf from base outwards; returns its tip."""
    tip = pol(base, yaw, pitch, length)
    tilt_box(m, col, base, tip, w, t)
    return tip


def limb(m, col, a, b, r0, r1, steps=6):
    """A tapering limb from a to b."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, top_r=max(r1, 2.0))


def mv(sc, swing, pivot, axis, rate, phase):
    """The model for parts that rock `swing` degrees about pivot."""
    return sc.part(P.motion(swing=swing, pivot=pivot, axis=axis, rate=rate, phase=phase))


# ------------------------------------------------------------------------------------------ ground cover (every district)
def moss(sc):
    """Lime moss cushions on five pale lumps of old lava, pale spore stalks nodding over them. A row 5.6 m long, 0.6 m tall."""
    r = random.Random(31)
    g = sc.still()
    spots = [(-215, 5, 60), (-100, -18, 76), (18, 14, 68), (128, -10, 82), (236, 12, 56)]
    for i, (x, y, rad) in enumerate(spots):
        # review round: the lumps were dark and buried, so the moss read as slime spilt on bare ground; now a pale
        # rock 22 to 30 cm proud and wider than its cushion, the cushion smaller and on top of it
        g.cyl(PALE_L if i % 2 else ASH_L, (x, y, -40), rad * 1.12, 62 + r.uniform(0, 8), steps=6, top_r=rad * 0.9)
        c = mv(sc, 1.4, (x, y, 22), r.uniform(0, 180), 0.25, r.random())
        c.sphere(LIME if i % 2 == 0 else LIME_D, (x, y, 32), rad * 0.8, scale=(1.0, 0.72, 0.36), steps=(3, 6))
        c.sphere(LIME_L, (x + rad * 0.24, y - rad * 0.15, 41), rad * 0.36, scale=(1.0, 0.8, 0.42), steps=(3, 5))
        s = mv(sc, 2.5, (x, y, 38), r.uniform(0, 180), 0.5, r.random())
        for k in range(3):
            px, py = x + r.uniform(-rad * 0.4, rad * 0.4), y + r.uniform(-rad * 0.24, rad * 0.24)
            s.box(CREAMY, (px, py, 56), (4.5, 4.5, 24))
            s.box(WHITE, (px, py, 70), (8, 8, 7))


def fern(sc):
    """Three rosettes of silver-green ash fern, a lime crosier unrolling in the middle of each. 0.5 m tall."""
    r = random.Random(32)
    g = sc.still()
    for i, (x, y, R, n) in enumerate([(-115, 6, 86, 8), (36, -12, 72, 7), (152, 18, 56, 6)]):
        g.cyl(BARK_D, (x, y, -30), 10, 42, steps=5)
        g.cyl(LIME, (x, y, 10), 5, 22, steps=4, top_r=3)
        g.box(LIME_L, (x, y - 5, 33), (9, 9, 7))
        for k in range(n):
            yaw = k * 360.0 / n + r.uniform(-10, 10)
            c = mv(sc, 2.4, (x, y, 10), 30 + 75 * (k % 2) + i * 20, 0.3 + 0.06 * (k % 2), 0.37 * i + 0.5 * (k % 2))
            a = (x, y, 12)
            m1 = pol(a, yaw, 42, R * 0.55)
            tip = pol(m1, yaw, -18, R * 0.6)
            tilt_box(c, FERN if k % 2 else FERN_L, a, m1, 22, 5)
            tilt_box(c, SILV if k % 3 == 0 else FERN_L, m1, tip, 14, 5)


# ------------------------------------------------------------------------------------------ s1d1 Cinder Springs
def horsetail(sc):
    """Giant horsetails: five jointed reeds up to 3.2 m, a whorl of needles at every joint, two with a spore cone."""
    r = random.Random(33)
    g = sc.still()
    g.sphere(LIME_D, (0, 0, 2), 60, scale=(1.5, 1.0, 0.16), steps=(3, 6))
    for i, (x, y, n) in enumerate([(-70, 10, 6), (-22, -28, 5), (30, 22, 6), (78, -12, 4), (4, 44, 5)]):
        c = mv(sc, 3.0, (x, y, 0), r.uniform(0, 180), 0.32 + 0.04 * i, r.random())
        seg = 50
        for k in range(n):
            z = -10 + k * seg
            c.cyl(GREEN if k % 2 else LEAF_L, (x, y, z), 6.5 - k * 0.3, seg, steps=4)
            if k:
                c.box(GREEN_D, (x, y, z), (17, 17, 5))
                L = 46 - k * 4
                for a in (r.uniform(0, 90), r.uniform(90, 180)):
                    p0 = pol((x, y, z + 3), a, 0, -L / 2.0)
                    p1 = pol((x, y, z + 14), a, 0, L / 2.0)
                    tilt_box(c, LIME_D if k % 2 else GREEN, p0, p1, 5, 4)
        top = -10 + n * seg
        if i in (0, 2):
            c.cyl(BARK, (x, y, top), 8, 30, steps=5, top_r=3)
        else:
            c.cyl(LIME, (x, y, top), 5, 20, steps=4, top_r=2.5)


def tree_fern(sc):
    """A tree fern: a fat ringed trunk 4.6 m tall, nine great arching fronds, three crosiers standing in the crown. 6 m."""
    r = random.Random(34)
    g = sc.still()
    z = -80
    for k, (h, r0) in enumerate([(160, 36), (140, 32), (130, 29), (110, 26)]):
        g.cyl(BARK if k % 2 else BARK_D, (0, 0, z), r0, h, steps=7, top_r=r0 - 3)
        g.cyl(BARK_D if k % 2 else BARK, (0, 0, z + h - 12), r0 + 2, 12, steps=7)
        z += h
    head = (0, 0, z)                               # 460
    g.sphere(BARK, head, 34, scale=(1, 1, 0.7), steps=(3, 6))
    for k in range(3):
        b = pol(head, k * 120 + 20, 70, 62)
        limb(g, LIME_D, head, b, 6, 4, 4)
        g.sphere(LIME, b, 11, steps=(3, 4))
    for k in range(9):
        yaw = k * 40 + r.uniform(-7, 7)
        c = mv(sc, 3.2, head, yaw + 90, 0.26 + 0.03 * (k % 3), r.random())
        col, col2 = (JADE, GREEN) if k % 2 else (GREEN, LEAF_L)
        a = head
        for j, (pitch, L, w) in enumerate([(34, 110, 26), (4, 110, 20), (-34, 100, 12)]):
            b = pol(a, yaw, pitch, L)
            tilt_box(c, BARK if j == 0 else col, a, b, 9 if j == 0 else 7, 6)
            mid = ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0)
            for s in (-1, 1):                      # the pinnae: one comb of leaf each side
                tip = pol(mid, yaw + s * 90, -6, w * 2.3)
                tilt_box(c, col if j < 2 else col2, mid, tip, L * 0.86, 5)
            a = b


def lemon_tub(sc):
    """A lemon tree grown as a tall standard in a cobalt tub: thin stem, a round head 3.3 m up, nine lemons. 4.7 m."""
    r = random.Random(35)
    g = sc.still()
    for s in (-1, 1):
        g.box(CHALK, (s * 30, 0, 5), (16, 70, 10))
    g.cyl(COBALT, (0, 0, 10), 44, 52, steps=8, top_r=52)
    g.cyl(CHALK, (0, 0, 60), 55, 8, steps=8)
    g.cyl(LEMON, (0, 0, 30), 50, 9, steps=8)
    g.cyl(BARK_D, (0, 0, 62), 44, 5, steps=8)
    g.cyl(MINT, (58, -30, 0), 12, 26, steps=6)                    # a little mint watering pot beside it
    limb(g, MINT, (58, -30, 20), (80, -44, 34), 4, 3, 4)
    c = mv(sc, 2.2, (0, 0, 62), 0, 0.3, 0.0)
    c.cyl(BARK, (0, 0, 60), 5.5, 290, steps=5, top_r=4.5)
    c.box(PALE_L, (0, 9, 200), (5, 5, 290))                        # its cane
    hd = (0, 0, 405)
    c.sphere(JADE, hd, 76, scale=(1, 1, 0.9), steps=(4, 7))
    c.sphere(GREEN, (38, -30, 440), 46, steps=(3, 6))
    c.sphere(GREEN_D, (-42, 26, 380), 44, steps=(3, 6))
    c.sphere(GREEN, (-20, -44, 436), 34, steps=(3, 5))
    for k in range(9):
        a, e = math.radians(k * 40 + r.uniform(-12, 12)), math.radians(r.uniform(-50, 25))
        p = (hd[0] + math.cos(a) * math.cos(e) * 78, hd[1] + math.sin(a) * math.cos(e) * 78, hd[2] + math.sin(e) * 70)
        c.sphere(LEMON, p, 10, scale=(1, 1, 1.25), steps=(3, 4))
    for k in range(5):
        a = math.radians(k * 72 + 20)
        c.box(WHITE, (hd[0] + math.cos(a) * 60, hd[1] + math.sin(a) * 60, hd[2] + 52), (11, 11, 6))


# ------------------------------------------------------------------------------------------ s1d2 Stormwatch
def glass_thorn(sc):
    """A thorn bush of black volcanic glass: glossy black limbs edged violet, lilac thorns, magenta flowers. 1.2 m."""
    r = random.Random(36)
    g = sc.still()
    g.cyl(GLASS, (0, 0, -30), 22, 52, steps=5, top_r=14)
    root = (0, 0, 18)
    for k in range(12):
        yaw = k * 30 + r.uniform(-10, 10)
        c = mv(sc, 2.4, root, yaw + 90, 0.33 + 0.05 * (k % 3), (k % 3) / 3.0 + 0.1)
        pitch = (22, 48, 68)[k % 3] + r.uniform(-6, 6)
        L = (112, 98, 84)[k % 3]
        mid = pol(root, yaw, pitch, L * 0.55)
        tip = pol(mid, yaw + r.uniform(-25, 25), pitch - 22, L * 0.5)
        limb(c, GLASS if k % 2 else VIOLET, root, mid, 8, 5, 4)
        limb(c, VIOLET if k % 2 else GLASS, mid, tip, 5, 2.2, 4)
        for j in range(2):
            p = pol(root, yaw, pitch, L * (0.25 + 0.2 * j))
            tilt_box(c, LILAC, p, pol(p, yaw + (70 if j else -70), 50, 20), 4.5, 4.5)
        if k % 3 != 1:
            c.sphere(MAG, tip, 10.5, steps=(3, 5))
            c.box(MAG_L, (tip[0], tip[1], tip[2] + 9), (7, 7, 6))


def fireweed(sc):
    """Fireweed: seven magenta flower spikes on thin leafy stems, up to 1.8 m, a rosette of leaves at the foot."""
    r = random.Random(37)
    g = sc.still()
    for k in range(7):
        blade(g, GREEN_D if k % 2 else FERN, (0, 0, 4), k * 51 + 10, 14, 58, 14, 4)
    spikes = [(-88, 6, 118), (-52, -22, 92), (-18, 18, 130), (14, -14, 104), (46, 20, 122), (78, -8, 88), (104, 16, 106)]
    for i, (x, y, h) in enumerate(spikes):
        c = mv(sc, 4.0, (x, y, 0), r.uniform(0, 180), 0.36 + 0.03 * (i % 4), r.random())
        c.cyl(GREEN, (x, y, -8), 3.2, h + 8, steps=4)
        for j in range(3):
            blade(c, GREEN if j % 2 else LEAF_L, (x, y, h * (0.3 + 0.2 * j)), i * 70 + j * 125, -12, 30, 8, 4)
        c.cyl(MAG, (x, y, h), 11, 44, steps=5, top_r=6)
        c.cyl(MAG_L, (x, y, h + 44), 6, 14, steps=5, top_r=2.2)


# ------------------------------------------------------------------------------------------ s1d3 Slagtown
def tussock(sc):
    """Pale straw tussocks: one planted in a miner's tin tub with a teal band, two loose beside it, seed plumes. 1.2 m."""
    r = random.Random(38)
    g = sc.still()
    g.cyl(TIN, (-70, 0, 0), 40, 46, steps=8, top_r=46)
    g.cyl(TEAL, (-70, 0, 28), 45, 9, steps=8)
    g.cyl(BARK_D, (-70, 0, 44), 40, 4, steps=8)
    for s in (-1, 1):
        g.box(TIN, (-70 + s * 49, 0, 36), (6, 16, 8))                # the tub's two handles
    for i, (x, y, z, n, L) in enumerate([(-70, 0, 46, 12, 62), (38, -16, 0, 13, 78), (112, 22, 0, 10, 60)]):
        for k in range(n):
            c = mv(sc, 4.0, (x, y, z), 40 + 70 * (k % 2) + i * 25, 0.4 + 0.07 * (k % 2), 0.3 * i + 0.45 * (k % 2))
            blade(c, (STRAW, STRAW_D, STRAW_L)[k % 3], (x, y, z), k * 360.0 / n + r.uniform(-9, 9), (48, 64, 78)[k % 3] + r.uniform(-5, 5), L * r.uniform(0.82, 1.0), 8, 4)
            if k % 6 == 0:
                tip = pol((x, y, z), k * 360.0 / n + 16, 80, L * 1.02)
                tilt_box(c, STRAW_D, (x, y, z), tip, 4, 4)
                c.cyl(CREAM, (tip[0], tip[1], tip[2] - 4), 5.5, 22, steps=4, top_r=2.2)


def bent_pine(sc, f=1.0):
    """A pine bent by the mountain's wind (towards f * X): its windward side charred to bare black stubs, new green
    streaming off the lee side in flat pads with lime tips, a lump of basalt at its foot. 7.4 m."""
    r = random.Random(39)
    g = sc.still()
    X = lambda p: (p[0] * f, p[1], p[2])
    pts = [(-10, 0, -90), (22, 4, 150), (86, -4, 330), (176, 4, 470), (286, 0, 570)]
    rad = [24, 19, 15, 11, 7]
    for k in range(4):
        limb(g, CHAR if k % 2 == 0 else BARK_D, X(pts[k]), X(pts[k + 1]), rad[k], rad[k + 1], 6)
    g.cyl(BAS_L, X((-52, 30, -30)), 44, 62, steps=6, top_r=30)
    for (x, y, z, yaw) in ((0, 0, 4, 200), (0, 0, 4, 320), (0, 0, 4, 80)):
        tilt_box(g, BARK_D, X((x, y, z + 14)), X(pol((x, y, z - 8), yaw, 0, 70)), 12, 12)
    for (k, yaw, pitch, L) in ((1, 172, 22, 96), (2, 190, 30, 82), (2, 150, 12, 64), (3, 178, 34, 60)):   # the windward stubs
        b = pol(pts[k], yaw, pitch, L)
        limb(g, CHAR, X(pts[k]), X(b), 6, 2.5, 4)
    pads = [(2, (150, -30, 400), 96, JADE), (3, (250, 34, 500), 118, GREEN_D), (3, (300, -40, 560), 104, JADE),
            (4, (372, 10, 622), 124, GREEN_D), (4, (300, 20, 676), 84, JADE)]
    for i, (k, ctr, R, col) in enumerate(pads):
        piv = X(pts[k])
        c = mv(sc, 2.4, piv, 90, 0.28 + 0.04 * (i % 3), r.random())
        limb(c, BARK_D, piv, X(ctr), 6, 3.5, 4)
        c.sphere(col, X(ctr), R, scale=(1.5, 1.0, 0.34), steps=(4, 7))
        c.sphere(LIME, X((ctr[0] + R * 1.15, ctr[1] + 12, ctr[2] + 10)), R * 0.42, scale=(1.4, 1.0, 0.4), steps=(3, 5))
        c.cyl(BARK, X((ctr[0] + R * 0.3, ctr[1] - R * 0.4, ctr[2] - R * 0.34 - 16)), 7, 18, steps=5, top_r=4)   # a cone hanging under it


def bent_pine_l(sc):
    bent_pine(sc, -1.0)


# ------------------------------------------------------------------------------------------ s1d4 the Cliff Temple
def bamboo(sc):
    """A clump of bamboo: eight ringed canes up to 7.4 m leaning out of one root, leaf sprays only on the top half."""
    r = random.Random(40)
    g = sc.still()
    g.sphere(BAS_L, (0, 0, -6), 74, scale=(1.2, 1.0, 0.3), steps=(3, 6))
    for k in range(5):
        g.cyl(STRAW_D, (r.uniform(-50, 50), r.uniform(-40, 40), -6), 7, r.uniform(26, 50), steps=4, top_r=2.5)   # new shoots
    for i in range(8):
        a = i * 45 + r.uniform(-14, 14)
        x, y = math.cos(math.radians(a)) * r.uniform(14, 52), math.sin(math.radians(a)) * r.uniform(12, 42)
        H = r.uniform(520, 740)
        lean = r.uniform(2, 7)
        c = mv(sc, 2.6 + 0.3 * (i % 3), (x, y, 0), a + 90, 0.24 + 0.03 * (i % 4), r.random())
        foot, top = (x, y, -20), pol((x, y, -20), a, 90 - lean, H + 20)
        col = (LEAF_L, GREEN, STRAW)[i % 3]
        third = [tuple(foot[q] + (top[q] - foot[q]) * t for q in range(3)) for t in (0.0, 0.36, 0.7, 1.0)]
        for j in range(3):
            limb(c, col, third[j], third[j + 1], 6.5 - j * 1.3, 5.2 - j * 1.3, 4)
        for t in (0.18, 0.36, 0.54, 0.7):
            p = tuple(foot[q] + (top[q] - foot[q]) * t for q in range(3))
            c.box(GREEN_D, p, (15 - t * 5, 15 - t * 5, 4))
        for j in range(6):
            t = 0.6 + j * 0.078
            p = tuple(foot[q] + (top[q] - foot[q]) * t for q in range(3))
            blade(c, JADE if j % 2 else LIME_D, p, a + j * 137, -14 - j * 4, 78 - j * 4, 14, 4)


def plum(sc):
    """A plum tree in white blossom: a dark twisted trunk, four boughs, clouds of white and blush blossom from 3.3 m
    up, petals drifting under it. 6 m."""
    r = random.Random(41)
    g = sc.still()
    pts = [(0, 0, -80), (-16, 6, 110), (20, -8, 230), (-6, 4, 330)]
    for k in range(3):
        limb(g, BARK_D, pts[k], pts[k + 1], 22 - k * 4, 18 - k * 4, 6)
    g.cyl(PALE_L, (0, 0, -20), 60, 26, steps=6, top_r=50)                 # a ring of pale temple stone round its foot
    fork = pts[3]
    boughs = [(30, 38, 150), (150, 46, 140), (265, 34, 150), (95, 70, 120)]
    ends = []
    for (yaw, pitch, L) in boughs:
        e = pol(fork, yaw, pitch, L)
        limb(g, BARK_D, fork, e, 11, 6, 5)
        ends.append(e)
    clouds = [(0, (0, 30, 34), 84), (0, (-46, -20, 76), 62), (1, (0, -30, 30), 90), (1, (50, 30, 80), 58), (2, (0, 0, 36), 86),
              (2, (-30, 40, 84), 60), (3, (0, 0, 60), 78), (3, (30, -30, 122), 54)]
    for i, (b, off, R) in enumerate(clouds):
        e = ends[b]
        c = mv(sc, 2.0, fork, boughs[b][0] + 90, 0.27 + 0.04 * b, 0.2 * b + 0.1)
        ctr = (e[0] + off[0], e[1] + off[1], e[2] + off[2])
        c.sphere(WHITE if i % 3 else BLUSH, ctr, R, scale=(1.15, 1.15, 0.78), steps=(4, 6))
        c.sphere(BLUSH if i % 3 else WHITE, (ctr[0] + R * 0.5, ctr[1] - R * 0.5, ctr[2] + R * 0.3), R * 0.42, steps=(3, 5))
    for k in range(6):                                                   # petals on the air
        a = k * 60 + 15
        p = pol((0, 0, 190 + (k % 3) * 50), a, 0, 110 + (k % 2) * 60)
        c = sc.part(P.motion(bob=-26, swing=14, pivot=(0, 0, 200), axis=P.UP, rate=0.16 + 0.02 * (k % 3), phase=k / 6.0))
        c.box(WHITE if k % 2 else BLUSH, p, (13, 13, 4.5))


# ------------------------------------------------------------------------------------------ s1d5 Windmill Flats
def sunflower(sc):
    """A farm row of five ash sunflowers: grey-green stems and leaves, pale yellow heads that turn a little. 2.4 m."""
    r = random.Random(42)
    g = sc.still()
    g.box(ASH_L, (0, 6, 3), (470, 60, 12))                                # the ridge of ash they are sown in
    for i, x in enumerate((-190, -96, 0, 94, 188)):
        y = r.uniform(-8, 8)
        H = r.uniform(168, 212)
        g.cyl(FERN, (x, y, -10), 4.5, H + 10, steps=4)
        lf = mv(sc, 5.0, (x, y, H * 0.5), 0, 0.42 + 0.05 * (i % 3), r.random())
        for j in range(3):
            blade(lf, FERN_L if j % 2 else FERN, (x, y, H * (0.3 + 0.2 * j)), (j * 150 + i * 40) % 360, 10, 42, 24, 4)
        h = mv(sc, 13.0, (x, y, H), P.UP, 0.1 + 0.015 * (i % 3), r.random())
        ctr = (x, y - 6, H + 14)
        dirv = (0.0, -math.cos(math.radians(22)), math.sin(math.radians(22)))
        back = (ctr[0], ctr[1] + 6, ctr[2] - 3)
        tilt_box(h, FERN, (x, y, H - 4), back, 8, 8)
        rod(h, CREAMY, ctr, (ctr[0] + dirv[0] * 5, ctr[1] + dirv[1] * 5, ctr[2] + dirv[2] * 5), 40, steps=9)
        rod(h, LEMON, (ctr[0], ctr[1] + dirv[1] * 5, ctr[2] + dirv[2] * 5), (ctr[0], ctr[1] + dirv[1] * 9, ctr[2] + dirv[2] * 9), 30, steps=9)
        rod(h, BARK, (ctr[0], ctr[1] + dirv[1] * 9, ctr[2] + dirv[2] * 9), (ctr[0], ctr[1] + dirv[1] * 14, ctr[2] + dirv[2] * 14), 20, steps=7)


def thrift(sc):
    """Pink thrift: four grey-green cushions studded with pink pompoms on short stems. 0.6 m."""
    r = random.Random(43)
    g = sc.still()
    for i, (x, y, R) in enumerate([(-150, 6, 58), (-44, -14, 70), (64, 12, 62), (160, -8, 50)]):
        g.sphere(FERN if i % 2 else GREEN_D, (x, y, 6), R, scale=(1.0, 0.8, 0.42), steps=(3, 6))
        for k in range(5):
            a = math.radians(k * 72 + i * 25)
            d = R * (0.2 + 0.12 * (k % 3))
            px, py = x + math.cos(a) * d * 1.6, y + math.sin(a) * d * 1.2
            c = mv(sc, 5.0, (px, py, 20), k * 36.0, 0.45 + 0.05 * (k % 2), 0.21 * i + 0.5 * (k % 2))
            hgt = 30 + 9 * (k % 3)
            c.box(FERN_L, (px, py, 20 + hgt / 2.0), (4.5, 4.5, hgt))
            c.sphere(PINK if (k + i) % 2 else PINK_L, (px, py, 24 + hgt), 9.5, steps=(3, 4))


def willow(sc):
    """A pollard willow: a fat pale knuckled trunk cut off at 3.3 m, fourteen withies shooting from the knob, each
    trailing long sky-pale leaf streamers that swing. 6.6 m."""
    r = random.Random(44)
    g = sc.still()
    g.cyl(BARK_P, (0, 0, -80), 46, 250, steps=7, top_r=38)
    g.cyl(BARK_P, (0, 0, 170), 38, 150, steps=7, top_r=44)
    g.sphere(BARK_P, (0, 0, 336), 56, scale=(1, 1, 0.66), steps=(4, 7))
    for (yaw, z) in ((40, 60), (200, 140), (300, 230)):
        p = pol((0, 0, z), yaw, 0, 40)
        g.sphere(BARK, p, 15, steps=(3, 5))                              # knots
    head = (0, 0, 350)
    for k in range(14):
        yaw = k * (360.0 / 14) + r.uniform(-8, 8)
        c = mv(sc, 3.6, head, yaw + 90, 0.24 + 0.03 * (k % 4), r.random())
        e = pol(head, yaw, (66, 54, 76)[k % 3], (230, 200, 290)[k % 3] * r.uniform(0.92, 1.05))
        limb(c, LEAF_L, head, e, 6, 3.5, 4)
        col = (SKY_L, SKY_M, SKY_P)[k % 3]
        d1 = pol(e, yaw, -62, 130)
        tilt_box(c, col, e, d1, 20, 5)
        tilt_box(c, (SKY_M, SKY_P, SKY_L)[k % 3], d1, pol(d1, yaw, -84, 60), 13, 5)
        mid = ((head[0] + e[0]) / 2.0, (head[1] + e[1]) / 2.0, (head[2] + e[2]) / 2.0)
        tilt_box(c, SKY_P if k % 2 else SKY_L, mid, pol(mid, yaw + 40, -50, 80), 14, 5)


# ------------------------------------------------------------------------------------------ s1d6 Geotherm
def reed_bed(sc):
    """Reeds round a pale outfall pool: twenty blades and five cattails standing out of a concrete-lipped puddle. 2.7 m."""
    r = random.Random(45)
    g = sc.still()
    g.cyl(TIN, (0, 0, -30), 150, 34, steps=8)
    g.cyl(POOL, (0, 0, 2), 128, 5, steps=8)
    for k in range(20):
        a = k * 18 + r.uniform(-6, 6)
        d = r.uniform(30, 118)
        x, y = math.cos(math.radians(a)) * d, math.sin(math.radians(a)) * d * 0.8
        c = mv(sc, 4.0, (x, y, 0), 30 + 60 * (k % 3), 0.34 + 0.05 * (k % 3), (k % 3) / 3.0 + 0.07)
        blade(c, (FERN_L, LIME_D, GREEN)[k % 3], (x, y, 0), a + 30, r.uniform(74, 86), r.uniform(170, 262), 9, 4)
    for i, (x, y, H) in enumerate([(-60, 20, 196), (-10, -40, 232), (44, 30, 210), (86, -18, 176), (-96, -24, 184)]):
        c = mv(sc, 3.0, (x, y, 0), r.uniform(0, 180), 0.3 + 0.04 * i, r.random())
        c.cyl(LEAF_L, (x, y, -6), 3, H + 6, steps=4)
        c.cyl(BARK, (x, y, H), 8, 36, steps=6)
        c.cyl(STRAW, (x, y, H + 36), 3, 16, steps=4, top_r=2.2)


def birch_tub(sc):
    """Three pale birch saplings in white tubs with a lime band, the power station's planting: white stems with
    graphite marks, airy lime-yellow heads from 3.5 m up. 5.7 m."""
    r = random.Random(46)
    g = sc.still()
    for i, x in enumerate((-175, 0, 175)):
        y = (-6, 8, -4)[i]
        g.cyl('#f2f4ee', (x, y, 0), 40, 50, steps=8, top_r=44)
        g.cyl(LIME, (x, y, 30), 44, 10, steps=8)
        g.cyl(BARK_D, (x, y, 48), 38, 4, steps=8)
        H = (380, 430, 360)[i]
        c = mv(sc, 2.6, (x, y, 50), r.uniform(0, 180), 0.3 + 0.04 * i, r.random())
        c.cyl('#f2f4ee', (x, y, 48), 6, H, steps=5, top_r=4)
        for k in range(4):
            c.box(GRAPH, (x - 4, y - 3, 90 + k * 70 + i * 12), (9, 7, 6))
        for k in range(6):
            a = math.radians(k * 60 + i * 20)
            d = (40, 26, 46)[k % 3]
            ctr = (x + math.cos(a) * d, y + math.sin(a) * d * 0.8, 48 + H - 40 + (k % 3) * 46 + k * 8)
            c.sphere((LIME_L, LIME, LIME_D)[k % 3], ctr, 36 + (k % 2) * 8, scale=(1, 1, 0.8), steps=(3, 5))
            limb(c, '#f2f4ee', (x, y, 48 + H - 70), ctr, 4, 2.5, 4)


# ------------------------------------------------------------------------------------------ s1d7 the Wall Keep
def box_tub(sc):
    """A clipped box ball in a sand-stone tub with a turquoise lozenge, a clipped cone in a smaller tub, and an ivy
    mound grown over a fallen block between them. 1.3 m."""
    r = random.Random(47)
    g = sc.still()
    g.box(SAND, (-100, 0, 24), (84, 84, 48))
    g.box(CHALK, (-100, 0, 50), (92, 92, 8))
    g.box(TURQ, (-100, -43, 24), (26, 4, 26), rot=(45, 0, 0))
    g.box(SAND, (118, 6, 18), (60, 60, 36))
    g.box(CHALK, (118, 6, 38), (66, 66, 6))
    g.box(PALE_L, (14, 10, 16), (78, 56, 40), rot=(0, 14, 6))
    b = mv(sc, 1.5, (-100, 0, 54), 0, 0.3, 0.0)
    b.sphere(GREEN_D, (-100, 0, 90), 38, steps=(5, 8))
    b.sphere(GREEN, (-100, 0, 108), 22, scale=(1.2, 1.2, 0.5), steps=(3, 6))
    k2 = mv(sc, 1.5, (118, 6, 40), 90, 0.34, 0.4)
    k2.cyl(GREEN_D, (118, 6, 40), 28, 78, steps=7, top_r=4)
    k2.sphere(GREEN, (118, 6, 120), 8, steps=(3, 4))
    for k in range(9):
        a = k * 40 + r.uniform(-12, 12)
        c = mv(sc, 4.0, (14, 10, 30), a + 90, 0.42 + 0.05 * (k % 3), (k % 3) / 3.0)
        p = pol((14, 10, 34), a, 0, r.uniform(14, 38))
        c.sphere(JADE if k % 2 else GREEN, p, r.uniform(15, 22), scale=(1.2, 1.2, 0.6), steps=(3, 5))
        blade(c, GREEN_D if k % 2 else JADE, p, a, -34, r.uniform(42, 66), 12, 4)


def chestnut(sc):
    """The keep's old chestnut: a massive trunk, three boughs, a broad dark canopy from 4.4 m up, cream candles of
    blossom standing all over it. 10.6 m."""
    r = random.Random(48)
    g = sc.still()
    g.cyl(BARK_D, (0, 0, -100), 78, 190, steps=7, top_r=58)
    g.cyl(BARK, (0, 0, 90), 58, 280, steps=7, top_r=48)
    fork = (0, 0, 360)
    for (yaw, z) in ((60, 150), (250, 240)):
        g.sphere(BARK_D, pol((0, 0, z), yaw, 0, 50), 20, steps=(3, 5))
    boughs = [(20, 50, 300), (140, 44, 320), (260, 52, 290)]
    lobes = [(0, (0, 0, 120), 230, GREEN_D), (0, (150, 90, 20), 170, GREEN), (1, (0, 0, 110), 240, GREEN),
             (1, (-120, 80, 10), 160, GREEN_D), (2, (0, 0, 130), 220, GREEN_D), (2, (60, -150, 20), 170, GREEN)]
    for b, (yaw, pitch, L) in enumerate(boughs):
        e = pol(fork, yaw, pitch, L)
        limb(g, BARK, fork, e, 30, 16, 6)
        c = mv(sc, 1.4, fork, yaw + 90, 0.2 + 0.03 * b, 0.3 * b)
        for (bb, off, R, col) in lobes:
            if bb != b:
                continue
            ctr = (e[0] + off[0], e[1] + off[1], e[2] + off[2])
            c.sphere(col, ctr, R, scale=(1.1, 1.1, 0.72), steps=(5, 8))
            for k in range(3):
                a = math.radians(k * 120 + b * 40 + R)
                p = (ctr[0] + math.cos(a) * R * 0.6, ctr[1] + math.sin(a) * R * 0.6, ctr[2] + R * 0.52)
                c.cyl(CREAM, p, 15, 58, steps=5, top_r=3)
    top = mv(sc, 1.4, fork, 0, 0.22, 0.5)
    top.sphere(GREEN, (0, 0, 860), 200, scale=(1.2, 1.2, 0.74), steps=(5, 8))
    limb(top, BARK, fork, (0, 0, 760), 24, 12, 5)
    for k in range(4):
        a = math.radians(k * 90 + 30)
        top.cyl(CREAM, (math.cos(a) * 130, math.sin(a) * 130, 960), 15, 58, steps=5, top_r=3)


# ------------------------------------------------------------------------------------------ s1d8 the Backlot
def prop_palm(sc):
    """The prop department's palm: a flat painted board palm standing in a mint tub on four wheels, a timber brace
    behind it, and a patch of real weeds at its wheels. The board rocks on its brace. 4.9 m."""
    r = random.Random(49)
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            rod(g, NAVY, (sx * 36, sy * 30 - 5, 11), (sx * 36, sy * 30 + 5, 11), 11, steps=7)
    g.box(MINT, (0, 0, 44), (100, 74, 46))
    g.box(CREAM, (0, -38, 44), (100, 4, 12))
    g.box(NAVY, (0, 0, 69), (106, 80, 6))
    tilt_box(g, NAVY, (52, 0, 40), (112, 0, 78), 5, 5)                    # the pull handle
    g.box(NAVY, (114, 0, 80), (6, 34, 6))
    b = mv(sc, 1.6, (0, 0, 70), 0, 0.35, 0.0)
    b.box(CREAM, (0, 0, 235), (28, 6, 330))
    for k in range(6):
        b.box(NAVY if k % 2 else MINT, (0, -4, 110 + k * 52), (28, 4, 14), rot=(12 if k % 2 else -12, 0, 0))
    tilt_box(b, BARK, (0, 62, 74), (0, 6, 300), 9, 9)                    # the brace
    top = (0, 0, 400)
    b.box(BARK, (0, 0, 398), (44, 7, 30))
    for k, (ang, L) in enumerate([(8, 170), (38, 180), (68, 160), (112, 160), (142, 180), (172, 170), (90, 120)]):
        e = (top[0] + math.cos(math.radians(ang)) * L, 0, top[2] + math.sin(math.radians(ang)) * L * 0.52 - (0 if k == 6 else 18))
        tilt_box(b, (MINT, JADE, GREEN)[k % 3], top, e, 6, 36 if k < 6 else 30)
        if k < 6:
            tilt_box(b, (JADE, GREEN, MINT)[k % 3], e, (e[0] + (30 if e[0] > 0 else -30), 0, e[2] - 56), 6, 22)
    for k in range(3):
        b.sphere(CREAMY, (-16 + k * 16, -3, 384 - (k % 2) * 8), 9, scale=(1, 0.5, 1), steps=(3, 5))   # painted dates
    for k in range(8):                                                  # the real weeds
        x, y = -74 + (k % 4) * 9, -34 + (k // 4) * 60
        c = mv(sc, 5.0, (x, y, 0), 60.0 * (k % 3), 0.5 + 0.06 * (k % 2), 0.5 * (k % 2) + 0.1)
        blade(c, LIME_D if k % 2 else GREEN, (x, y, 0), 150 + k * 28, 52 + (k % 3) * 12, 46 + (k % 3) * 9, 8, 4)


def dandelion(sc):
    """Giant dandelion clocks, the backlot's real weeds: five white seed heads and two yellow flowers on thin stems
    over a rosette of toothed leaves. 1.9 m."""
    r = random.Random(50)
    g = sc.still()
    for k in range(9):
        t = blade(g, JADE if k % 2 else GREEN, (0, 0, 4), k * 40 + 8, 12, 66, 18, 4)
        blade(g, GREEN if k % 2 else JADE, t, k * 40 + 8, -10, 24, 9, 4)
    stems = [(-60, 8, 136, 0), (-22, -24, 104, 0), (10, 22, 150, 0), (48, -10, 128, 0), (84, 14, 144, 0), (-90, -16, 84, 1), (28, -40, 70, 1)]
    for i, (x, y, H, kind) in enumerate(stems):
        c = mv(sc, 4.6, (x, y, 0), r.uniform(0, 180), 0.34 + 0.04 * (i % 3), r.random())
        c.cyl(LEAF_L, (x, y, -6), 3, H + 6, steps=4)
        if kind == 0:
            c.sphere('#ffffff', (x, y, H + 16), 23, steps=(4, 6))
            c.sphere(STRAW_L, (x, y, H + 16), 9, steps=(3, 4))
        else:
            c.cyl(LEMON, (x, y, H), 21, 7, steps=8, top_r=24)
            c.cyl('#ffd23f', (x, y, H + 7), 11, 5, steps=6)


JOBS = [('SM_S1PLANTS_MossCushion', moss, LOWG), ('SM_S1PLANTS_AshFern', fern, LOWG),
        ('SM_S1PLANTS_Horsetail', horsetail, MIDG), ('SM_S1PLANTS_TreeFern', tree_fern, TREE), ('SM_S1PLANTS_LemonTub', lemon_tub, MIDG),
        ('SM_S1PLANTS_GlassThorn', glass_thorn, LOWG), ('SM_S1PLANTS_Fireweed', fireweed, LOWG),
        ('SM_S1PLANTS_Tussock', tussock, LOWG), ('SM_S1PLANTS_BentPine', bent_pine, TREE), ('SM_S1PLANTS_BentPineL', bent_pine_l, TREE),
        ('SM_S1PLANTS_Bamboo', bamboo, TREE), ('SM_S1PLANTS_PlumBlossom', plum, TREE),
        ('SM_S1PLANTS_AshSunflower', sunflower, MIDG), ('SM_S1PLANTS_PinkThrift', thrift, LOWG), ('SM_S1PLANTS_Willow', willow, TREE),
        ('SM_S1PLANTS_ReedBed', reed_bed, MIDG), ('SM_S1PLANTS_BirchTub', birch_tub, MIDG),
        ('SM_S1PLANTS_BoxTub', box_tub, LOWG), ('SM_S1PLANTS_Chestnut', chestnut, TREE),
        ('SM_S1PLANTS_PropPalm', prop_palm, MIDG), ('SM_S1PLANTS_Dandelion', dandelion, LOWG)]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
