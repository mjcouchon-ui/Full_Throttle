"""Volcano, Stage 1, builder s1d7: district 7 "The Inner Wall", 19.5 to 22.3 km = THE WALL KEEP, an old fortress town
grown out of the crater's inner wall. /Game/World/Volcano/SM_S1D7_*. Run: import ft_world_volcano_s1d7 as K; K.run()

Massive sand stone, round drum towers with chalk battlements, buttressed curtain walls, navy slate cone roofs, timber
hoardings, long turquoise banners with old gold fringes. Nothing aims at the road (no cannon, catapult, crossbow), no
vehicle, nothing red or orange, no box or lantern that bobs or glows. Every piece is an ft_people.Scene: one material
(M_People), one section, movement in the vertex shader only (banners, flags, a bell, a hoist's load, a well bucket).

7 buildings (stand 63 m out and more, behind the lava channels and their basalt walls: all taller than 14 m):
  GateTower, CurtainWall, CornerTower, GreatHall, Granary, BellCote, WellBastion
4 tall OPEN props (24 to 40 m: thin poles and frames, the lava shows through): BannerPole, ShieldBoard, ScaffoldArch,
  PennantLine
4 LOW props (tops under 2 m): HorseTrough, StoneBalls, MarketStall, Barrels
1 hero: CrownKeep (a drum 30 m wide, 42 m to its crown, 54 m to the flag; four cone turrets, four 20 m banners)
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
SAND, STONE, TURQ, NAVY, CHALK, GOLD = '#e8d9b0', '#b0a088', '#19c5c0', '#1c2a4a', '#fbf7ee', '#c8b14b'   # STONE = kit PALE_L, GOLD = kit SULPH
PANE, WOOD, WOOD_L, LITC = '#14101a', '#5c3324', '#87746d', '#ffe28a'                                # kit PANE, RUST_D, PALE, LIT
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SMALL = dict(grow=(40, 40, 40), grow_down=(40, 40, 0))


def _cs(a):
    r = math.radians(a)
    return math.cos(r), math.sin(r)


# ------------------------------------------------------------------------------------------ the parts every piece shares
def ring(g, col, x, y, z, r, n, size, start=0.0):
    """n boxes round (x, y) at radius r; size = (radial, along the ring, up)."""
    for k in range(n):
        a = start + 360.0 * k / n
        c, s = _cs(a)
        g.box(col, (x + r * c, y + r * s, z), size, rot=(0, a, 0))


def drum(g, col, x, y, r, h, steps=10, foot=200):
    g.cyl(col, (x, y, -foot), r, h + foot, steps=steps)


def crown(g, x, y, z, r, n, steps=10, mh=90, start=None):
    """A corbelled parapet with n chalk merlons on top of a drum whose top is at z."""
    g.cyl(CHALK, (x, y, z - 60), r, 60, steps=steps, top_r=r + 36)
    g.cyl(SAND, (x, y, z), r + 36, 70, steps=steps)
    w = 2.0 * math.pi * (r + 14) / n * 0.5
    ring(g, CHALK, x, y, z + 70 + mh / 2.0, r + 14, n, (44, w, mh), start=(180.0 / n if start is None else start))


def cone(g, x, y, z, r, h, steps=8, col=NAVY, ball=True):
    g.cyl(col, (x, y, z), r, h, steps=steps, top_r=6)
    if ball:
        g.sphere(GOLD, (x, y, z + h + 12), 20, steps=(3, 6))


def slit(g, x, y, z, w=26, h=130, yaw=0.0, col=PANE):
    g.box(col, (x, y, z), (w, 10, h), rot=(0, yaw, 0))


def face_box(g, col, x, y, R, steps, a, z, w, h, extra=0.0, glow=0.0):
    """A flat thing (a slit, a pane) on a drum of radius R and `steps` sides, at angle a. It is drawn thick enough to
    show whether a corner or the middle of a side of the drum lies at that angle: from 4 cm inside the side's middle
    to 6 cm outside the corner (+ extra)."""
    fr = R * math.cos(math.pi / steps)
    inner, outer = fr - 4.0, R + 6.0 + extra
    c, s = _cs(a)
    m = (inner + outer) / 2.0
    g.box(col, (x + m * c, y + m * s, z), (w, outer - inner, h), rot=(0, a + 90.0, 0), glow=glow)


def drum_slit(g, x, y, R, steps, a, z, w=26, h=130):
    face_box(g, PANE, x, y, R, steps, a, z, w, h)


def merlons(g, x0, x1, y, z, n, d=50, h=90, col=CHALK):
    step = (x1 - x0) / float(n)
    for k in range(n):
        g.box(col, (x0 + (k + 0.5) * step, y, z + h / 2.0), (step * 0.55, d, h))


def emblem(f, kind, x, y, z, ang, s, col=CHALK):
    """An invented device on a banner or a shield: a sun, a tower or three wave bars. ang = the way it faces."""
    c, sn = _cs(ang)
    tx, ty = -sn, c
    if kind == 'sun':
        rod(f, col, (x + c * 1, y + sn * 1, z), (x + c * 9, y + sn * 9, z), s * 0.42, steps=8)
    elif kind == 'tower':
        f.box(col, (x + c * 3, y + sn * 3, z - s * 0.1), (10, s * 0.5, s * 0.7), rot=(0, ang, 0))
        for k in (-1, 0, 1):
            f.box(col, (x + c * 3 + tx * k * s * 0.19, y + sn * 3 + ty * k * s * 0.19, z + s * 0.33), (10, s * 0.13, s * 0.16), rot=(0, ang, 0))
    else:
        for k in (-1, 0, 1):
            f.box(col, (x + c * 3, y + sn * 3, z + k * s * 0.3), (10, s * 0.9, s * 0.13), rot=(0, ang, 0))


def hang(sc, x, y, ztop, w, length, n, ang, col=TURQ, rate=0.3, ph=0.0, kind='sun', amp=1.0, em=CHALK, fringe=GOLD):
    """A long banner hanging flat against a wall that faces `ang`: n lengths, each swaying in the wall's own plane
    about the top a little further and a little later than the one above (a slow ripple running down the cloth)."""
    c, s = _cs(ang)
    seg = length / float(n)
    for k in range(n):
        f = sc.part(P.motion(swing=amp * (0.5 + 0.55 * k), pivot=(x, y, ztop), axis=ang, rate=rate, phase=ph - 0.09 * k))
        zc = ztop - (k + 0.5) * seg
        f.box(col, (x, y, zc), (6, w, seg + 2), rot=(0, ang, 0))
        if k == 1 or (n >= 6 and k == 3):
            emblem(f, kind if k == 1 else 'wave', x, y, zc, ang, min(w, seg) * 0.8, col=em)
        if k == n - 1:
            f.box(fringe, (x + c * 1, y + s * 1, ztop - length - 6), (8, w, 16), rot=(0, ang, 0))
    g = sc.still()
    g.box(fringe, (x + c * 2, y + s * 2, ztop + 5), (12, w + 30, 12), rot=(0, ang, 0))


def flag(sc, x, y, z, w, h, n=3, col=TURQ, rate=0.55, ph=0.0, pole=150, pole_col=WOOD_L):
    """A flag flying from a short pole: lengths that swing round the pole, each further and later than the last."""
    g = sc.still()
    if pole:
        g.cyl(pole_col, (x, y, z - pole + h / 2.0), 5, pole + 8, steps=6)
        g.sphere(GOLD, (x, y, z + h / 2.0 + 16), 11, steps=(3, 6))
    seg = w / float(n)
    for k in range(n):
        f = sc.part(P.motion(swing=7 + 6 * k, pivot=(x, y, z), axis=P.UP, rate=rate, phase=ph - 0.12 * k))
        f.box(col, (x + (k + 0.5) * seg + 4, y, z), (seg + 1, 6, h - k * h * 0.12))


def window(g, x, y, z, w=90, h=150, lit=False, shutters=True):
    """A window on a front (-Y) face at y: chalk surround, dark or softly lit pane, turquoise shutters."""
    g.box(CHALK, (x, y - 2, z), (w + 28, 8, h + 28))
    if lit:
        g.box(LITC, (x, y - 5, z), (w, 8, h), glow=0.5)
        g.box(NAVY, (x, y - 8, z), (8, 6, h))
    else:
        g.box(PANE, (x, y - 5, z), (w, 8, h))
    if shutters:
        for s in (-1, 1):
            g.box(TURQ, (x + s * (w / 2.0 + 20), y - 7, z), (34, 8, h))


# ------------------------------------------------------------------------------------------ buildings
def gate_tower(sc):
    """The town gate: a square gate house between two round flanking towers with slate cones, its portcullis drawn
    up into the arch, a banner with a sun over the gate, a timber ramp. 11 m wide, 6.5 deep, 20.5 m to the cones."""
    g = sc.still()
    g.box(STONE, (0, 20, 20), (640, 560, 240))
    g.box(SAND, (0, 20, 620), (560, 480, 1440))                      # front face y = -220, top 1340
    for s in (-1, 1):
        x = s * 330
        drum(g, SAND, x, -170, 190, 1560, steps=8)
        g.cyl(STONE, (x, -170, -100), 232, 300, steps=8, top_r=190)
        g.cyl(CHALK, (x, -170, 980), 198, 30, steps=8)
        crown(g, x, -170, 1560, 190, 6, steps=8, start=30.0)
        cone(g, x, -170, 1630, 165, 420)
        for z in (520, 820, 1280):
            slit(g, x, -361, z)
        flag(sc, x, -170, 2150, 150, 60, n=3, ph=0.2 + 0.3 * s, rate=0.5)
    g.box(CHALK, (0, 20, 1360), (590, 510, 40))
    merlons(g, -135, 135, -205, 1380, 3)
    merlons(g, -280, 280, 245, 1380, 5)
    # the gate: chalk arch stones, the dark way through, the portcullis drawn up
    rod(g, CHALK, (0, -212, 600), (0, -224, 600), 172, steps=10)
    for s in (-1, 1):
        g.box(CHALK, (s * 156, -222, 300), (32, 10, 600))
    rod(g, PANE, (0, -218, 600), (0, -230, 600), 140, steps=10)
    g.box(PANE, (0, -226, 300), (280, 12, 600))
    for k in range(5):
        g.box(NAVY, (-100 + k * 50, -236, 590), (12, 10, 270))
    for z in (500, 640):
        g.box(NAVY, (0, -238, z), (250, 10, 12))
    tilt_box(g, WOOD, (0, -235, 26), (0, -470, -40), 300, 16)        # the ramp
    for s in (-1, 1):
        g.box(WOOD_L, (s * 140, -350, 30), (12, 230, 60))
    # two small lit windows of the winch room and the banner
    window(g, -112, -220, 1080, w=44, h=100, lit=True, shutters=False)
    window(g, 112, -220, 1080, w=44, h=100, lit=True, shutters=False)
    hang(sc, 0, -228, 1320, 150, 440, 4, -90.0, rate=0.3, ph=0.1, kind='sun')


def curtain_wall(sc):
    """Sixteen metres of curtain wall: three buttresses, chalk battlements along one half, a timber hoarding with a
    slate lean-to roof hung out over the other, a tower banner, a postern door. 13 m to the walk, 16 m to the hoarding."""
    g = sc.still()
    g.box(STONE, (0, 10, 40), (1620, 330, 280))
    g.box(SAND, (0, 20, 640), (1600, 240, 1280))                     # front face y = -100, walk at 1280
    g.box(CHALK, (0, 20, 1295), (1620, 280, 30))
    merlons(g, 20, 800, -100, 1310, 5, d=50, h=100)
    merlons(g, -800, 800, 135, 1310, 8, d=40, h=100)
    for x in (-640, -80, 480):
        g.box(SAND, (x, -160, 380), (150, 130, 760))
        g.box(STONE, (x, -175, 60), (180, 170, 200))
        tilt_box(g, CHALK, (x, -228, 752), (x, -100, 930), 150, 30)
    # the hoarding (a timber fighting gallery; its shutters are closed)
    g.box(WOOD, (-400, -170, 1400), (760, 150, 200))
    g.box(WOOD_L, (-400, -170, 1296), (790, 164, 14))
    for k in range(5):
        g.box(PANE, (-700 + k * 150, -247, 1410), (70, 8, 60))
    tilt_box(g, NAVY, (-400, -285, 1478), (-400, -55, 1610), 810, 24)
    for k in range(4):
        tilt_box(g, WOOD, (-740 + k * 227, -105, 1170), (-740 + k * 227, -238, 1292), 18, 18)
    for (x, z) in ((-360, 760), (105, 520), (105, 900), (690, 700)):
        slit(g, x, -101, z)
    # the postern: a turquoise door under a chalk lintel, two steps
    g.box(CHALK, (-330, -101, 310), (150, 8, 250))
    g.box(TURQ, (-330, -104, 290), (110, 8, 200))
    g.box(GOLD, (-296, -108, 290), (10, 6, 10))
    g.box(STONE, (-330, -200, 90), (200, 90, 180))
    g.box(STONE, (-330, -280, 40), (260, 70, 100))
    for s in (-1, 1):                                                # two drain spouts
        g.box(CHALK, (330 + s * 330, -130, 1230), (24, 70, 20))
    hang(sc, 300, -108, 1180, 170, 600, 4, -90.0, rate=0.28, ph=0.45, kind='tower')


def corner_tower(sc):
    """A round corner tower: battered foot, a string course, a corbelled crown of chalk merlons, a tall slate cone
    with a gold ball and a flag, a timber oriel under the crown, one lit window. 6.6 m across, 27 m to the cone's tip."""
    g = sc.still()
    drum(g, SAND, 0, 0, 330, 2000, steps=10)
    g.cyl(STONE, (0, 0, -200), 410, 560, steps=10, top_r=330)
    g.cyl(CHALK, (0, 0, 1180), 340, 36, steps=10)
    crown(g, 0, 0, 2000, 330, 10, steps=10, mh=100)
    cone(g, 0, 0, 2070, 295, 640, steps=10)
    # the porch and its door
    g.box(STONE, (0, -375, 130), (180, 90, 300))
    g.box(TURQ, (0, -422, 110), (100, 8, 200))
    g.box(CHALK, (0, -421, 222), (130, 8, 24))
    g.box(STONE, (0, -460, -10), (160, 80, 60))
    # windows and slits
    face_box(g, CHALK, 0, 0, 330, 10, -90.0, 1500, 112, 162)
    face_box(g, LITC, 0, 0, 330, 10, -90.0, 1500, 80, 130, extra=4, glow=0.5)
    face_box(g, NAVY, 0, 0, 330, 10, -90.0, 1500, 8, 130, extra=8)
    for s in (-1, 1):
        face_box(g, TURQ, 0, 0, 330, 10, -90.0 + s * 12.5, 1500, 30, 130, extra=5)
    for (a, z) in ((-126.0, 800), (-54.0, 1050), (-126.0, 1420), (-54.0, 1660), (-90.0, 760)):
        drum_slit(g, 0, 0, 330, 10, a, z)
    # the oriel: a timber box on two braces with its own little slate roof
    g.box(WOOD, (0, -372, 1800), (160, 110, 150))
    g.box(PANE, (0, -429, 1810), (70, 6, 50))
    tilt_box(g, NAVY, (0, -450, 1870), (0, -310, 1950), 190, 20)
    for s in (-1, 1):
        tilt_box(g, WOOD, (s * 60, -322, 1620), (s * 60, -420, 1728), 16, 16)
    flag(sc, 0, 0, 2860, 190, 80, n=3, ph=0.35, rate=0.5, pole=160)


def great_hall(sc):
    """The great hall: a long stone hall on a plinth with a stair, crow-stepped gables at both ends, a steep slate
    roof, four tall lit windows, a turquoise double door under a striped awning, a louvre and a turning gold vane on
    the ridge. 17 m long, 8.6 deep, 18.6 m to the gables' tops."""
    g = sc.still()
    g.box(STONE, (0, 0, 40), (1700, 860, 280))
    g.box(SAND, (0, 0, 630), (1600, 760, 900))                       # walls 180 to 1080, front y = -380
    tilt_box(g, NAVY, (0, -412, 1066), (0, 0, 1692), 1540, 30)
    tilt_box(g, NAVY, (0, 412, 1066), (0, 0, 1692), 1540, 30)
    for s in (-1, 1):
        for k in range(6):
            hw = 400 - k * 70
            g.box(SAND, (s * 785, 0, 1135 + k * 110), (50, 2 * hw, 110))
            g.box(CHALK, (s * 785, 0, 1198 + k * 110), (62, 2 * hw + 12, 16))
    g.box(CHALK, (0, -384, 1070), (1560, 14, 24))                    # the eaves' course
    for x in (-600, -330, 330, 600):
        g.box(CHALK, (x, -382, 640), (146, 8, 470))
        g.box(LITC, (x, -385, 640), (110, 8, 430), glow=0.5)
        g.box(NAVY, (x, -388, 640), (8, 6, 430))
        for z in (530, 750):
            g.box(NAVY, (x, -388, z), (110, 6, 8))
    for x in (-465, 465):
        g.box(SAND, (x, -420, 520), (110, 90, 680))
        tilt_box(g, CHALK, (x, -462, 856), (x, -380, 980), 110, 26)
    # the door, its awning, the stair
    g.box(CHALK, (0, -382, 370), (260, 8, 390))
    for s in (-1, 1):
        g.box(TURQ, (s * 52, -386, 350), (98, 8, 330))
        g.box(GOLD, (s * 12, -391, 350), (8, 6, 40))
    for k in range(6):
        tilt_box(g, TURQ if k % 2 == 0 else CHALK, (-125 + k * 50, -384, 600), (-125 + k * 50, -530, 540), 50, 8)
    for s in (-1, 1):
        g.box(WOOD_L, (s * 150, -524, 330), (8, 8, 420))
    for k in range(3):
        g.box(STONE, (0, -450 - k * 50, 140 - k * 55), (360 + k * 60, 60, 70))
    # the ridge
    g.box(WOOD, (-380, 0, 1740), (130, 110, 110))
    g.cyl(NAVY, (-380, 0, 1795), 110, 90, steps=4, top_r=6, rot=(0, 45, 0))
    for x in (250, 520):
        g.box(SAND, (x, 150, 1560), (80, 80, 300))
        g.box(CHALK, (x, 150, 1718), (100, 100, 18))
    g.cyl(GOLD, (0, 0, 1690), 6, 190, steps=6)
    v = sc.part(P.motion(swing=28, pivot=(0, 0, 1850), axis=P.UP, rate=0.12, phase=0.2))
    v.box(GOLD, (40, 0, 1850), (150, 6, 44))
    rod(v, GOLD, (-60, -4, 1850), (-60, 4, 1850), 26, steps=8)


def granary(sc):
    """The buttressed granary: four massive stone buttresses, three loading doors one over another, a hoist beam
    under a slate hood with a sack on its rope, vent slits, two little louvres on the ridge. 10.6 x 7.6 m, 15.8 m tall."""
    g = sc.still()
    g.box(STONE, (0, 0, 30), (1060, 760, 260))
    g.box(SAND, (0, 0, 620), (1000, 700, 960))                       # walls to 1100, front y = -350
    tilt_box(g, NAVY, (0, -384, 1086), (0, 0, 1474), 1040, 30)
    tilt_box(g, NAVY, (0, 384, 1086), (0, 0, 1474), 1040, 30)
    for s in (-1, 1):
        for k in range(3):
            g.box(SAND, (s * 480, 0, 1160 + k * 120), (40, 2 * (330 - k * 110), 120))
    for x in (-430, -150, 150, 430):
        g.box(SAND, (x, -400, 420), (130, 110, 840))
        g.box(STONE, (x, -420, 60), (170, 170, 200))
        tilt_box(g, CHALK, (x, -458, 832), (x, -350, 1040), 130, 30)
    for z in (270, 570, 870):
        g.box(CHALK, (0, -351, z), (150, 8, 200))
        g.box(TURQ, (0, -354, z - 5), (120, 8, 170))
        g.box(NAVY, (0, -357, z - 5), (6, 6, 170))
    for x in (-290, 290):
        for z in (400, 700, 1000):
            slit(g, x, -351, z, h=90)
    # the hoist: a beam under a hood, a sack swinging on its rope
    g.box(WOOD, (0, -500, 1075), (30, 320, 30))
    tilt_box(g, NAVY, (0, -660, 1090), (0, -350, 1180), 150, 20)
    rod(g, GOLD, (-8, -620, 1052), (8, -620, 1052), 22, steps=8)
    h = sc.part(P.motion(swing=3.5, pivot=(0, -620, 1050), axis=90.0, rate=0.22, phase=0.3))
    h.box(WOOD_L, (0, -620, 810), (6, 6, 480))
    h.sphere(CHALK, (0, -620, 530), 46, scale=(1.0, 0.9, 1.35), steps=(4, 6))
    h.box(GOLD, (0, -620, 596), (30, 30, 10))
    # a stack of sacks by the door and the ridge louvres
    for (x, z) in ((-250, 20), (-180, 20), (-215, 64)):
        g.sphere(CHALK, (x, -520, z), 38, scale=(1.3, 0.9, 0.7), steps=(3, 6))
    for x in (-250, 250):
        g.box(WOOD, (x, 0, 1500), (110, 90, 90))
        g.cyl(NAVY, (x, 0, 1545), 95, 80, steps=4, top_r=6, rot=(0, 45, 0))


def bell_cote(sc):
    """The bell tower: a square stone tower with corner buttresses, an open belfry with a great gold bell that
    swings slowly, a slate pyramid, a gold vane that turns. 5.2 m square, 26 m tall."""
    g = sc.still()
    g.box(STONE, (0, 0, 40), (600, 600, 280))
    g.box(SAND, (0, 0, 650), (520, 520, 1700))                       # to 1500, front y = -260
    for z in (700, 1200):
        g.box(CHALK, (0, 0, z), (540, 540, 30))
    for s in (-1, 1):
        g.box(SAND, (s * 245, -255, 460), (130, 130, 920))
        tilt_box(g, CHALK, (s * 245, -322, 912), (s * 245, -250, 1040), 130, 26)
    g.box(CHALK, (0, 0, 1512), (580, 580, 26))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(SAND, (sx * 205, sy * 205, 1725), (110, 110, 400))
    g.box(SAND, (0, 0, 1960), (540, 540, 70))
    g.box(CHALK, (0, 0, 2005), (580, 580, 24))
    g.cyl(NAVY, (0, 0, 2017), 420, 540, steps=4, top_r=6, rot=(0, 45, 0))
    g.cyl(GOLD, (0, 0, 2540), 6, 150, steps=6)
    v = sc.part(P.motion(swing=30, pivot=(0, 0, 2660), axis=P.UP, rate=0.1, phase=0.6))
    v.box(GOLD, (36, 0, 2660), (140, 6, 40))
    rod(v, GOLD, (-56, -4, 2660), (-56, 4, 2660), 24, steps=8)
    # the bell
    g.box(WOOD, (0, 0, 1905), (430, 34, 34))
    b = sc.part(P.motion(swing=15, pivot=(0, 0, 1890), axis=0.0, rate=0.28, phase=0.1))
    b.box(WOOD, (0, 0, 1876), (240, 44, 30))
    b.cyl(GOLD, (0, 0, 1640), 128, 60, steps=8, top_r=112)
    b.cyl(GOLD, (0, 0, 1700), 112, 150, steps=8, top_r=58)
    b.sphere(NAVY, (0, 0, 1628), 24, steps=(3, 6))
    # the front: a louvred window, slits, a door with a striped hood, steps
    g.box(CHALK, (0, -262, 1010), (150, 8, 220))
    for k in range(4):
        g.box(TURQ, (0, -266, 930 + k * 52), (118, 8, 34))
    slit(g, 0, -261, 620)
    slit(g, 0, -261, 1350)
    g.box(CHALK, (0, -262, 310), (150, 8, 250))
    g.box(WOOD, (0, -266, 300), (116, 8, 210))
    for k in range(4):
        tilt_box(g, TURQ if k % 2 == 0 else CHALK, (-66 + k * 44, -262, 470), (-66 + k * 44, -360, 430), 44, 8)
    for k in range(2):
        g.box(STONE, (0, -330 - k * 50, 120 - k * 60), (200 + k * 60, 60, 70))


def well_bastion(sc):
    """The covered well on its round bastion: a low battered drum with a crown of merlons, the town well on top
    under a slate cone on four timber posts, a windlass and a bucket that swings, water jars, a wave banner."""
    g = sc.still()
    drum(g, SAND, 0, 0, 450, 800, steps=12)
    g.cyl(STONE, (0, 0, -200), 530, 520, steps=12, top_r=450)
    crown(g, 0, 0, 800, 450, 12, steps=12, mh=90, start=15.0)
    for a in (-135.0, -45.0):
        drum_slit(g, 0, 0, 450, 12, a, 470)
    # the well
    g.cyl(CHALK, (0, 0, 870), 124, 86, steps=10)
    g.cyl(PANE, (0, 0, 950), 96, 8, steps=10)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WOOD, (sx * 150, sy * 150, 1040), (18, 18, 340))
    g.box(WOOD, (0, 0, 1204), (340, 340, 16))
    cone(g, 0, 0, 1212, 265, 290, steps=8)
    rod(g, WOOD, (-150, 0, 1060), (150, 0, 1060), 12, steps=6)
    g.box(WOOD_L, (172, 0, 1040), (8, 8, 70))
    b = sc.part(P.motion(swing=5, pivot=(0, 0, 1060), axis=0.0, rate=0.3, phase=0.4))
    b.box(WOOD_L, (0, 0, 1010), (6, 6, 100))
    b.cyl(WOOD, (0, 0, 930), 24, 36, steps=6, top_r=28)
    for (x, y) in ((-250, -180), (-300, -90), (260, -150)):
        g.cyl(TURQ, (x, y, 870), 30, 60, steps=6, top_r=20)
        g.cyl(CHALK, (x, y, 930), 22, 8, steps=6)
    # the bastion's door and stair, its banner
    g.box(STONE, (-230, -420, 110), (170, 110, 260))
    g.box(TURQ, (-230, -478, 95), (96, 8, 180))
    g.box(CHALK, (-230, -477, 196), (124, 8, 22))
    hang(sc, 110, -458, 760, 140, 400, 3, -90.0, rate=0.3, ph=0.7, kind='wave')


# ------------------------------------------------------------------------------------------ tall OPEN props (24 to 40 m)
def banner_pole(sc):
    """A banner on a tall thin pole: a stone foot, a gold yard and finial, a long turquoise banner with a sun and
    wave bars and a gold fringe, a small chalk pennant at the head. 9.4 m; open (one pole)."""
    g = sc.still()
    g.box(STONE, (0, 0, -5), (90, 90, 130))
    g.box(CHALK, (0, 0, 66), (100, 100, 12))
    g.cyl(WOOD_L, (0, 0, 60), 8, 850, steps=6)
    g.sphere(GOLD, (0, 0, 926), 20, steps=(4, 6))
    g.cyl(GOLD, (0, 0, 880), 14, 10, steps=6)
    g.box(GOLD, (0, -14, 852), (10, 10, 30))
    for s in (-1, 1):
        g.box(GOLD, (s * 92, -16, 812), (6, 6, 50))
    hang(sc, 0, -16, 838, 150, 560, 5, -90.0, rate=0.32, ph=0.0, kind='sun', amp=1.5)
    flag(sc, 0, 0, 890, 130, 44, n=3, col=CHALK, ph=0.5, rate=0.6, pole=0)


def shield_board(sc):
    """A heraldic board on two posts under a little slate roof: three shields with invented devices (a sun, a tower,
    wave bars), a row of cloth tabs fluttering under it. 5.9 m; open below 3.3 m."""
    g = sc.still()
    for s in (-1, 1):
        g.box(STONE, (s * 215, 0, -5), (56, 56, 110))
        g.cyl(WOOD, (s * 215, 0, 40), 9, 520, steps=6)
        g.sphere(GOLD, (s * 215, 0, 574), 15, steps=(3, 6))
    g.box(WOOD_L, (0, 0, 430), (480, 12, 190))
    for z in (332, 528):
        g.box(CHALK, (0, -2, z), (500, 16, 14))
    tilt_box(g, NAVY, (0, -64, 520), (0, 2, 566), 540, 14)
    tilt_box(g, NAVY, (0, 64, 520), (0, -2, 566), 540, 14)
    fields = ((-150, NAVY, 'sun', GOLD), (0, TURQ, 'tower', CHALK), (150, CHALK, 'wave', TURQ))
    for (x, col, kind, em) in fields:
        g.box(col, (x, -9, 452), (112, 8, 96))
        rod(g, col, (x, -5, 404), (x, -13, 404), 56, steps=8)
        emblem(g, kind, x, -13, 428, -90.0, 84, col=em)
    for k in range(6):
        t = sc.part(P.motion(swing=9, pivot=(-200 + k * 80, -2, 324), axis=0.0, rate=0.55, phase=k * 0.17))
        t.box(TURQ if k % 2 == 0 else GOLD, (-200 + k * 80, -2, 296), (62, 6, 56))


def scaffold_arch(sc):
    """A mason's timber scaffold arch with a pulley: four poles, two top beams, braces, a plank deck with cut
    stone on it, a ladder, a gold pulley wheel and a block of stone swinging on its rope. 7.4 m; open."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WOOD, (sx * 270, sy * 75, 320), (16, 16, 760))
        g.box(WOOD, (sx * 270, 0, 340), (14, 170, 14))
        g.box(WOOD, (sx * 270, 0, 690), (14, 170, 14))
        tilt_box(g, WOOD_L, (sx * 270, -75, 40), (sx * 270, 75, 330), 12, 12)
        g.box(STONE, (sx * 270, 0, -20), (70, 210, 70))
    for sy in (-1, 1):
        g.box(WOOD, (0, sy * 75, 700), (640, 18, 18))
        tilt_box(g, WOOD_L, (-270, sy * 75, 480), (-80, sy * 75, 700), 12, 12)
        tilt_box(g, WOOD_L, (270, sy * 75, 480), (80, sy * 75, 700), 12, 12)
    g.box(WOOD_L, (-130, 0, 352), (290, 150, 10))
    g.box(SAND, (-200, 10, 380), (70, 50, 46))
    g.box(SAND, (-120, -20, 374), (60, 44, 34))
    g.cyl(WOOD, (-40, 30, 357), 20, 34, steps=6, top_r=24)
    for s in (-1, 1):                                                # the ladder
        tilt_box(g, WOOD, (370, s * 26 - 100, -20), (290, s * 26 - 100, 352), 10, 10)
    for k in range(5):
        g.box(WOOD_L, (358 - k * 15.5, -100, 40 + k * 70), (8, 52, 8))
    g.box(WOOD, (120, 0, 712), (18, 190, 18))
    rod(g, GOLD, (120, -8, 672), (120, 8, 672), 30, steps=8)
    g.box(WOOD_L, (152, 0, 400), (6, 6, 560))                        # the fall of the rope, made fast below
    h = sc.part(P.motion(swing=3.0, pivot=(92, 0, 672), axis=90.0, rate=0.2, phase=0.15))
    h.box(WOOD_L, (92, 0, 470), (6, 6, 400))
    h.box(SAND, (92, 0, 236), (76, 60, 56))
    h.box(GOLD, (92, 0, 268), (84, 10, 8))
    flag(sc, -270, 75, 790, 120, 46, n=2, col=GOLD, ph=0.2, rate=0.6, pole=96, pole_col=WOOD)
    c = sc.part(P.motion(swing=6, pivot=(270, -78, 690), axis=0.0, rate=0.4, phase=0.5))   # a turquoise wind cloth
    c.box(TURQ, (270, -84, 590), (150, 6, 190))


def pennant_line(sc):
    """Two thin masts fourteen metres apart with two sagging lines of pennants between them, turquoise, chalk and
    gold, every pennant fluttering on its own. 6.7 m; open."""
    g = sc.still()
    for s in (-1, 1):
        g.box(STONE, (s * 700, 0, -5), (70, 70, 110))
        g.cyl(WOOD_L, (s * 700, 0, 40), 8, 670, steps=6)
        g.sphere(GOLD, (s * 700, 0, 726), 16, steps=(3, 6))
    cols = (TURQ, CHALK, GOLD)
    for (z0, sag, n, size) in ((610, 110, 9, 62), (430, 70, 8, 44)):
        pts = []
        for k in range(n + 2):
            x = -700 + 1400.0 * k / (n + 1)
            pts.append((x, z0 - sag * (1.0 - (x / 700.0) ** 2)))
        for k in range(n + 1):
            tilt_box(g, NAVY, (pts[k][0], 0, pts[k][1]), (pts[k + 1][0], 0, pts[k + 1][1]), 6, 6)
        for k in range(1, n + 1):
            x, z = pts[k]
            f = sc.part(P.motion(swing=15, pivot=(x, 0, z), axis=0.0, rate=0.5 + 0.04 * (k % 3), phase=k * 0.13 + (0.4 if n == 8 else 0.0)))
            rod(f, cols[(k + n) % 3], (x, -3, z - size * 0.8), (x, 3, z - size * 0.8), size, steps=3)
    flag(sc, -700, 0, 680, 110, 40, n=2, col=TURQ, ph=0.1, rate=0.6, pole=0)
    flag(sc, 700, 0, 680, 110, 40, n=2, col=TURQ, ph=0.6, rate=0.6, pole=0)


# ------------------------------------------------------------------------------------------ LOW props (tops under 2 m)
def horse_trough(sc):
    """A stone horse trough: chalk rim, water, a carved pier with a gold spout at one end, a tethering rail with a
    saddle and blanket on it, a hay rack, a bucket. 6 m long, 1.8 m at the pier."""
    g = sc.still()
    g.box(SAND, (0, -20, 22), (320, 90, 105))
    for s in (-1, 1):
        g.box(CHALK, (0, -20 + s * 40, 80), (332, 12, 12))
        g.box(CHALK, (s * 160, -20, 80), (12, 90, 12))
        g.box(STONE, (s * 120, -20, -12), (50, 100, 36))
    g.box(TURQ, (0, -20, 73), (300, 70, 6))
    g.box(SAND, (-205, -20, 75), (62, 70, 210))                      # the pier, to 1.8 m
    g.box(CHALK, (-205, -20, 174), (74, 82, 12))
    g.box(GOLD, (-166, -20, 120), (26, 14, 12))
    g.sphere(CHALK, (-205, -62, 140), 20, steps=(3, 6))
    for x in (-60, 250):                                             # the rail
        g.box(WOOD, (x, 60, 55), (14, 14, 170))
    g.box(WOOD, (95, 60, 128), (340, 12, 12))
    g.box(TURQ, (40, 60, 138), (64, 40, 8))
    g.box(NAVY, (40, 60, 148), (44, 30, 14))
    for x in (150, 205):
        rod(g, GOLD, (x, 52, 112), (x, 46, 112), 10, steps=6)
    g.box(WOOD, (250, -30, 70), (16, 110, 16))                       # the hay rack
    g.box(WOOD_L, (250, -30, 20), (90, 70, 100))
    g.box(GOLD, (250, -30, 78), (80, 60, 26))
    g.cyl(WOOD_L, (-110, -100, -30), 22, 64, steps=6, top_r=26)
    g.cyl(TURQ, (-110, -100, 28), 20, 6, steps=6)


def stone_balls(sc):
    """A row of five sand-stone balls on chalk-capped plinths, a navy chain sagging between them; the middle one
    larger, with a gold band. 9 m long, 1.6 m."""
    g = sc.still()
    xs = (-400, -200, 0, 200, 400)
    for k, x in enumerate(xs):
        big = k == 2
        ph = 100 if big else 84
        g.box(STONE, (x, 0, ph / 2.0 - 15), (60 if big else 52, 60 if big else 52, ph + 30))
        g.box(CHALK, (x, 0, ph + 5), (70 if big else 60, 70 if big else 60, 10))
        r = 38 if big else 30
        g.sphere(SAND, (x, 0, ph + 8 + r), r, steps=(4, 8))
        if big:
            g.cyl(GOLD, (x, 0, ph + 4 + r), r + 2, 8, steps=8)
    for k in range(4):
        x0, x1 = xs[k] + 28, xs[k + 1] - 28
        xm = (x0 + x1) / 2.0
        tilt_box(g, NAVY, (x0, 0, 62), (xm, 0, 40), 6, 6)
        tilt_box(g, NAVY, (xm, 0, 40), (x1, 0, 62), 6, 6)


def market_stall(sc):
    """A market stall under a turquoise and chalk striped cloth: a timber counter with cheeses, loaves, jars and
    bolts of cloth, baskets and a sack beside it, a stool; the valance flaps. 3.6 m long, 1.9 m."""
    g = sc.still()
    g.box(WOOD, (0, 0, 27), (340, 110, 115))
    g.box(CHALK, (0, 0, 88), (352, 122, 7))
    for sx in (-1, 1):
        g.box(WOOD, (sx * 166, -56, 75), (10, 10, 150))
        g.box(WOOD, (sx * 166, 56, 92), (10, 10, 184))
    for k in range(7):
        col = TURQ if k % 2 == 0 else CHALK
        x = -150 + k * 50
        tilt_box(g, col, (x, -76, 150), (x, 72, 184), 50, 7)
        f = sc.part(P.motion(swing=11, pivot=(x, -77, 149), axis=0.0, rate=0.6 + 0.05 * (k % 3), phase=k * 0.19))
        f.box(col, (x, -77, 133), (50, 6, 30))
    for k in range(3):                                               # cheeses, loaves, jars, cloth
        g.cyl(CHALK, (-130, -10, 92 + k * 13), 20 - k * 3, 13, steps=8)
        g.sphere(GOLD, (-70 + k * 34, 14, 100), 15, scale=(1.3, 0.9, 0.7), steps=(3, 6))
        g.cyl(NAVY, (40 + k * 30, 20, 92), 11, 24, steps=6)
        g.cyl(CHALK, (40 + k * 30, 20, 116), 9, 5, steps=6)
        g.box((TURQ, SAND, NAVY)[k], (126, -8 + k * 2, 96 + k * 9), (70 - k * 6, 30, 9))
    g.cyl(WOOD_L, (-225, -20, -30), 34, 70, steps=8, top_r=40)        # baskets, a sack, a stool
    g.sphere(CHALK, (-225, -20, 44), 30, scale=(1.0, 1.0, 0.45), steps=(3, 6))
    g.cyl(WOOD_L, (232, 30, -30), 28, 62, steps=8, top_r=34)
    g.sphere(GOLD, (232, 30, 34), 25, scale=(1.0, 1.0, 0.45), steps=(3, 6))
    g.sphere(STONE, (215, -60, 22), 34, scale=(1.0, 0.9, 1.3), steps=(4, 6))
    g.box(WOOD, (0, 96, 40), (44, 36, 8))
    for s in (-1, 1):
        g.box(WOOD, (s * 16, 96, 3), (8, 8, 66))


def barrels(sc):
    """A stack of barrels in sand and navy with gold hoops: three standing, two lying across them, sacks and a
    coil of rope beside. 3.2 m long, 1.75 m."""
    g = sc.still()
    stand = ((-90, 0, SAND), (0, 10, NAVY), (90, 0, SAND))
    for (x, y, col) in stand:
        g.cyl(col, (x, y, -30), 40, 125, steps=8)
        for z in (16, 70):
            g.cyl(GOLD, (x, y, z), 42, 7, steps=8)
        g.cyl(WOOD_L, (x, y, 95), 34, 4, steps=8)
    for (x, col) in ((-45, NAVY), (45, SAND)):
        rod(g, col, (x, -50, 135), (x, 60, 135), 38, steps=8)
        for y in (-30, 40):
            rod(g, GOLD, (x, y - 3, 135), (x, y + 4, 135), 40, steps=8)
    for (x, y, z) in ((-190, 10, 14), (-165, -40, 14), (-180, -12, 52)):
        g.sphere(CHALK, (x, y, z), 36, scale=(1.25, 0.9, 0.7), steps=(3, 6))
    for k in range(3):
        g.cyl(WOOD_L, (175, -10, -6 + k * 9), 34 - k * 3, 9, steps=8)
    g.cyl(TURQ, (170, 60, -30), 22, 80, steps=6, top_r=16)             # a turquoise water jar
    g.cyl(CHALK, (170, 60, 50), 18, 6, steps=6)


# ------------------------------------------------------------------------------------------ the hero
def crown_keep(sc):
    """THE CROWN KEEP: a round sand-stone drum 30 m wide on a battered foot, two chalk string courses, a corbelled
    crown of sixteen chalk merlons 44 m up, four hanging turrets with slate cones, a lantern tower with a slate cone
    and a great flag in the middle, a fore-building with the door, and four turquoise banners 20 m long that ripple
    slowly down its face (two to the road, one each way along it). 54 m to the cone's tip."""
    g = sc.still()
    R = 1500.0
    g.cyl(STONE, (0, 0, -300), 1690, 900, steps=16, top_r=R)
    g.cyl(SAND, (0, 0, -300), R, 4300, steps=16)                      # to 4000
    for z in (1640, 2900):
        g.cyl(CHALK, (0, 0, z), R + 22, 50, steps=16)
    g.cyl(CHALK, (0, 0, 3830), R, 170, steps=16, top_r=R + 140)
    g.cyl(SAND, (0, 0, 4000), R + 140, 180, steps=16)
    ring(g, CHALK, 0, 0, 4290, R + 100, 16, (70, 330, 220), start=11.25)
    # four hanging turrets
    for a in (45.0, 135.0, 225.0, 315.0):
        c, s = _cs(a)
        cx, cy = 1570 * c, 1570 * s
        g.cyl(STONE, (cx, cy, 3180), 50, 420, steps=8, top_r=300)
        g.cyl(SAND, (cx, cy, 3600), 300, 860, steps=8)
        g.cyl(CHALK, (cx, cy, 4460), 334, 40, steps=8)
        cone(g, cx, cy, 4500, 365, 700, steps=8)
        g.box(PANE, (cx + 298 * c, cy + 298 * s, 4100), (10, 40, 190), rot=(0, a, 0))
        g.box(CHALK, (cx + 292 * c, cy + 292 * s, 3760), (16, 250, 30), rot=(0, a, 0))
    # the lantern tower and the flag
    g.cyl(SAND, (0, 0, 4180), 620, 420, steps=10)
    g.cyl(CHALK, (0, 0, 4600), 660, 40, steps=10)
    ring(g, CHALK, 0, 0, 4690, 620, 10, (50, 190, 100), start=18.0)
    cone(g, 0, 0, 4640, 560, 700, steps=10, ball=False)
    g.sphere(GOLD, (0, 0, 5360), 34, steps=(4, 6))
    g.cyl(WOOD_L, (0, 0, 5330), 9, 420, steps=6)
    for k in range(4):
        f = sc.part(P.motion(swing=6 + 5 * k, pivot=(0, 0, 5640), axis=P.UP, rate=0.4, phase=0.3 - 0.12 * k))
        f.box(TURQ, ((k + 0.5) * 110 + 8, 0, 5640), (112, 8, 200 - k * 18))
        if k == 1:
            rod(f, GOLD, (165, -9, 5640), (165, 9, 5640), 46, steps=8)
    # pilasters, slits, lit windows
    for a in (-157.5, -112.5, -67.5, -22.5):
        c, s = _cs(a)
        g.box(STONE, (R * c, R * s, 800), (110, 150, 2200), rot=(0, a, 0))
        g.box(CHALK, (R * c, R * s, 1912), (130, 170, 24), rot=(0, a, 0))
    faces = (-168.75, -146.25, -123.75, -101.25, -78.75, -56.25, -33.75, -11.25)
    for k, a in enumerate(faces):
        drum_slit(g, 0, 0, R, 16, a, 1150, w=40, h=210)
        if a in (-123.75, -56.25):
            drum_slit(g, 0, 0, R, 16, a, 2300, w=40, h=210)
            face_box(g, CHALK, 0, 0, R, 16, a, 3420, 150, 250)
            face_box(g, LITC, 0, 0, R, 16, a, 3420, 110, 210, extra=4, glow=0.5)
            face_box(g, NAVY, 0, 0, R, 16, a, 3420, 10, 210, extra=8)
    # the fore-building: the keep's door at the head of a stair
    g.box(SAND, (0, -1590, 300), (560, 360, 1200))                    # front y = -1770, top 900
    g.box(CHALK, (0, -1590, 912), (590, 390, 28))
    merlons(g, -280, 280, -1745, 926, 4, d=50, h=90)
    rod(g, CHALK, (0, -1762, 560), (0, -1774, 560), 150, steps=10)
    for s in (-1, 1):
        g.box(CHALK, (s * 136, -1772, 380), (30, 10, 360))
        g.box(TURQ, (s * 58, -1778, 380), (112, 8, 356))
        g.box(GOLD, (s * 14, -1783, 380), (8, 6, 46))
    rod(g, TURQ, (0, -1768, 560), (0, -1780, 560), 118, steps=10)
    for k in range(4):
        g.box(STONE, (0, -1810 - k * 60, 170 - k * 70), (420 + k * 80, 70, 80 + k * 20))
    for s in (-1, 1):
        g.cyl(SAND, (s * 330, -1790, -300), 80, 760, steps=8)
        cone(g, s * 330, -1790, 460, 96, 190, steps=8)
    # the four banners
    kinds = ('tower', 'sun', 'sun', 'tower')
    for k, a in enumerate((-168.75, -101.25, -78.75, -11.25)):
        c, s = _cs(a)
        hang(sc, (R + 10) * c, (R + 10) * s, 3700, 330, 2000, 6, a, rate=0.2, ph=0.23 * k, kind=kinds[k], amp=1.0, em=GOLD)


JOBS = [('SM_S1D7_GateTower', gate_tower, SMALL), ('SM_S1D7_CurtainWall', curtain_wall, SMALL), ('SM_S1D7_CornerTower', corner_tower, SMALL),
        ('SM_S1D7_GreatHall', great_hall, SMALL), ('SM_S1D7_Granary', granary, SMALL), ('SM_S1D7_BellCote', bell_cote, SMALL),
        ('SM_S1D7_WellBastion', well_bastion, SMALL),
        ('SM_S1D7_BannerPole', banner_pole, dict(grow=(70, 70, 30), grow_down=(70, 70, 0))),
        ('SM_S1D7_ShieldBoard', shield_board, dict(grow=(20, 20, 20), grow_down=(20, 20, 0))),
        ('SM_S1D7_ScaffoldArch', scaffold_arch, dict(grow=(40, 40, 30), grow_down=(40, 40, 0))),
        ('SM_S1D7_PennantLine', pennant_line, dict(grow=(40, 40, 30), grow_down=(40, 40, 0))),
        ('SM_S1D7_HorseTrough', horse_trough, STILL), ('SM_S1D7_StoneBalls', stone_balls, STILL),
        ('SM_S1D7_MarketStall', market_stall, dict(grow=(8, 8, 6), grow_down=(8, 8, 0))), ('SM_S1D7_Barrels', barrels, STILL),
        ('SM_S1D7_CrownKeep', crown_keep, dict(far=True, grow=(160, 160, 80), grow_down=(160, 160, 0)))]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)            # (name, build[, Scene.save options])


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
