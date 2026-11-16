"""Volcano, Stage 1, builder s1d1: district 1 "The Ash Road" (0 to 3.76 km), CINDER SPRINGS.
/Game/World/Volcano/SM_S1D1_*. Run: import ft_world_volcano_s1d1 as K; K.run()

A hot-spring spa town at the mountain's foot that carries on bathing while it erupts: rounded whitewashed bath-houses
with shallow cobalt domes, barrel vaults, arcades of round arches, mint tile bands and steam pipes with mushroom caps;
lemon awnings and shutters; and THE STEAM CLOCK. All of it is drawn here: no brand, no real name, no words, no
likeness, no outside asset. Nothing red, nothing orange, no vehicle, no hose, no jet, nothing aimed at the road.
Every mesh is an ft_people.Scene (one section, M_People); movement is the builders' motion values only.
The placing rules are the block C:/ftwork/s1/L3_s1d1.inl.

  buildings, deep (left, 65 m)      BathHouse, TeaPavilion, BoilerHouse            (7 m deep: behind the left channel's wall)
  buildings, shallow (70 m)         PumpRoom, GuestHouse, Laundry, ChangingTowers  (4.6 m deep or less: behind the right-hand wall and railway)
  tall open props (26.7 m and more) ParasolRow, TowelLine, SunArch, SteamVane      (thin legs: the lava shows through)
  low props (23.6 m)                PlungeRim, SpringTap, TubStack, Loungers, TeaBench   (under 2 m, unlit, still)
  hero                              SteamClock (38 m: the hands sweep, a pendulum swings, the kettle spout steams)
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

W, COB, LEM, MINT, PANE = '#f4efe6', '#2456c8', '#ffe45c', '#8fe0c0', '#14101a'   # the district's palette (L3_plan.md)
STONE, WOOD, BRASS = '#cdc5c8', '#b0a088', '#c8b14b'        # the kit's HUT, PALE_L, SULPH: plinths and steps, tubs, wheels
STEAM, WATER = '#fbf7ee', '#c9f4e6'                         # steam, and a hot spring's pale water (never a lava colour)
G_LAMP, G_WIN, G_STUD = 0.9, 0.8, 1.1                       # steady light: only on pieces that stand 63 m out and more


# ------------------------------------------------------------------------------------------ the town's own parts
def zbox(g, col, x, y, z0, z1, w, d, glow=0.0):
    """A box w along X, d along Y, from z0 up to z1."""
    g.box(col, (x, y, (z0 + z1) / 2.0), (w, d, z1 - z0), glow=glow)


def xrod(g, col, x0, x1, y, z, r, steps=10, glow=0.0):
    rod(g, col, (x0, y, z), (x1, y, z), r, steps, glow)


def yrod(g, col, x, y0, y1, z, r, steps=10, glow=0.0):
    """A disc that faces the road: a cylinder along Y."""
    rod(g, col, (x, y0, z), (x, y1, z), r, steps, glow)


def dome(g, col, x, y, z, r, squash=0.62, steps=(5, 10)):
    """A shallow dome: a squashed ball sunk to its middle in what it stands on."""
    g.sphere(col, (x, y, z), r, scale=(1.0, 1.0, squash), steps=steps)


def arch(g, col, x, yf, z0, w, h, t=8.0, steps=8, glow=0.0):
    """A round-topped opening on a front wall (the wall's face is y = yf): w wide, h tall to the crown."""
    zs = z0 + h - w / 2.0
    zbox(g, col, x, yf - t / 2.0 + 2.0, z0, zs, w, t, glow)
    yrod(g, col, x, yf - t + 2.0, yf + 2.0, zs, w / 2.0, steps, glow)


def arch_x(g, col, xf, sx, y, z0, w, h, t=8.0, steps=8):
    """The same on an end wall (its face is x = xf, looking along sx)."""
    zs = z0 + h - w / 2.0
    zbox(g, col, xf + sx * (t / 2.0 - 2.0), y, z0, zs, t, w)
    rod(g, col, (xf - sx * 2.0, y, zs), (xf + sx * (t - 2.0), y, zs), w / 2.0, steps)


def rwin(g, cx, cy, z, R, ang, r=36.0):
    """A round window in a cobalt ring on a drum of radius R, `ang` degrees round from the front."""
    a = math.radians(ang)
    dx, dy = math.sin(a), -math.cos(a)
    rod(g, COB, (cx + dx * (R - 8), cy + dy * (R - 8), z), (cx + dx * (R + 5), cy + dy * (R + 5), z), r + 9, 8)
    rod(g, PANE, (cx + dx * (R - 6), cy + dy * (R - 6), z), (cx + dx * (R + 9), cy + dy * (R + 9), z), r, 8)


def mushroom(g, x, y, z0, h, r=16.0, band=True):
    """A steam pipe with a mushroom cap: the town's chimney."""
    g.cyl(MINT, (x, y, z0), r, h, steps=8)
    if band:
        g.cyl(COB, (x, y, z0 + h * 0.55), r + 3.0, 12.0, steps=8)
    g.cyl(MINT, (x, y, z0 + h), r * 1.1, r * 1.2, steps=8, top_r=r * 2.8)
    g.sphere(MINT, (x, y, z0 + h + r * 1.2), r * 2.8, scale=(1.0, 1.0, 0.45), steps=(4, 8))
    return z0 + h + r * 2.5


def puffs(sc, x, y, z, n=2, r=40.0, seed=0.0):
    """Steam standing over a pipe: pale balls that float up and down, each on its own clock. Buildings and the hero only."""
    for k in range(n):
        p = sc.part(P.motion(bob=-(22.0 + 9.0 * k), rate=0.17 + 0.05 * k, phase=(seed + 0.37 * k) % 1.0))
        p.sphere(STEAM, (x + (k % 2) * r * 0.5, y, z + k * r * 1.5), r * (1.0 + 0.28 * k), steps=(4, 7))


def pennant(sc, x, y, z, col, phase=0.0, h=110.0):
    """A little pennant turning on a brass mast."""
    g = sc.still()
    g.cyl(BRASS, (x, y, z), 4.0, h, steps=6)
    g.sphere(LEM, (x, y, z + h + 8.0), 9.0, steps=(3, 6))
    f = sc.part(P.motion(swing=20, pivot=(x, y, z + h - 22.0), axis=P.UP, rate=0.3, phase=phase))
    f.box(col, (x + 40.0, y, z + h - 22.0), (72, 5, 32))


def awning(g, x0, x1, yf, z, out=90.0, drop=40.0, n=6, cols=(LEM, W)):
    """A striped awning sloping out of a front wall."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        xc = x0 + (k + 0.5) * w
        tilt_box(g, cols[k % 2], (xc, yf, z), (xc, yf - out, z - drop), w, 7.0)


def wheel(m, x, y, z, r, col=BRASS):
    """A hand wheel that faces the road: a rim of eight, two spokes, a hub."""
    pts = [(x + math.cos(math.radians(45.0 * k)) * r, z + math.sin(math.radians(45.0 * k)) * r) for k in range(9)]
    for k in range(8):
        tilt_box(m, col, (pts[k][0], y, pts[k][1]), (pts[k + 1][0], y, pts[k + 1][1]), 9.0, 9.0)
    m.box(col, (x, y, z), (r * 2.0, 7.0, 8.0))
    m.box(col, (x, y, z), (8.0, 7.0, r * 2.0))
    yrod(m, COB, x, y - 7.0, y + 7.0, z, r * 0.2, 6)


# ------------------------------------------------------------------------------------------ buildings, deep (left, 65 m)
def bath_house(sc):
    """The domed bath-house with an arcade: 11.6 x 7.6 m, 13 m tall. A pennant turns, the awning lifts, steam stands over a pipe."""
    g = sc.still()
    zbox(g, STONE, 0, 0, -250, 60, 1160, 720)
    zbox(g, STONE, 0, -372, -250, 30, 520, 30)
    zbox(g, W, 0, 0, 60, 600, 1060, 640)
    for sx in (-1, 1):
        g.cyl(W, (sx * 530, -320, 60), 46, 540, steps=6)                 # soft corners
    for k in range(5):                                                   # the arcade: five round arches, the middle one the door
        arch(g, COB if k == 2 else PANE, -360 + k * 180, -320, 60, 124, 330)
    for k in range(6):
        x = -450 + k * 180
        g.cyl(W, (x, -338, 60), 17, 265, steps=6)
        g.box(LEM, (x, -352, 350), (22, 14, 26), glow=G_LAMP)            # a lamp on each pillar
    zbox(g, MINT, 0, -322, 440, 480, 1000, 8)                            # a band of tiles
    zbox(g, COB, 0, 0, 600, 640, 1100, 680)
    for sx in (-1, 1):
        dome(g, COB, sx * 400, 0, 640, 125, 0.62, (4, 8))
        g.sphere(LEM, (sx * 400, 0, 728), 14, steps=(3, 6))
    g.cyl(W, (0, 0, 640), 290, 250, steps=12)                            # the drum
    for a in (-60, -30, 0, 30, 60):
        rwin(g, 0, 0, 775, 290, a, 34)
    g.cyl(COB, (0, 0, 890), 302, 30, steps=12)
    dome(g, COB, 0, 0, 920, 296, 0.62, (6, 12))
    g.cyl(W, (0, 0, 1085), 46, 80, steps=8)                              # the lantern
    dome(g, COB, 0, 0, 1165, 56, 0.8, (4, 8))
    pennant(sc, 0, 0, 1200, LEM, 0.4)
    top = mushroom(g, -440, 210, 640, 330, 18)
    mushroom(g, 450, 190, 640, 220, 15)
    puffs(sc, -440, 210, top + 60, 2, 42, 0.1)
    aw = sc.part(P.motion(swing=3, pivot=(0, -320, 425), axis=0, rate=0.35, phase=0.2))
    awning(aw, -96, 96, -328, 425, 80, 34, 4)


def tea_pavilion(sc):
    """The tea pavilion on its plinth: 9.9 x 7.2 m, 12 m tall, open sides, three mint vaults, a great kettle on the
    table. The bunting sways, a pennant turns, the kettle steams."""
    g = sc.still()
    zbox(g, W, 0, 20, -250, 500, 860, 620)                               # the plinth: store rooms under the floor
    for x in (-250, 0, 250):
        arch(g, PANE if x else COB, x, -290, 0, 130, 300)
    zbox(g, MINT, 0, -292, 400, 440, 860, 6)
    for k in range(8):                                                   # the outside stair up the left end
        zbox(g, STONE, -480, -250 + k * 78, -250, (k + 1) * 62.5, 100, 78)
    zbox(g, STONE, 0, 20, 500, 530, 920, 680)
    for x in (-420, -140, 140, 420):
        for y in (-290, 330):
            g.cyl(W, (x, y, 530), 20, 300, steps=6)
    zbox(g, COB, 0, -304, 530, 600, 840, 6)                              # the rail on the road's side
    zbox(g, W, 0, 20, 830, 870, 920, 680)
    for x in (-290, 0, 290):                                             # three vaults, their round ends to the road
        yrod(g, MINT, x, -320, 360, 870, 140, 10)
        yrod(g, W, x, -327, -319, 870, 104, 10)
    yrod(g, LEM, 0, -333, -326, 884, 48, 8)                              # a sun in the middle one
    g.cyl(W, (0, 20, 1000), 40, 70, steps=8)
    dome(g, COB, 0, 20, 1070, 52, 0.8, (4, 8))
    pennant(sc, 0, 20, 1100, COB, 0.7)
    zbox(g, W, 0, 20, 530, 612, 210, 90)                                 # the tea table and the great kettle
    g.sphere(MINT, (0, 20, 658), 48, steps=(4, 8))
    tilt_box(g, MINT, (38, 20, 650), (96, 20, 704), 14, 14)
    g.sphere(LEM, (0, 20, 710), 10, steps=(3, 6))
    for x in (-84, -62):
        g.cyl(LEM, (x, 0, 612), 8, 12, steps=6)
    puffs(sc, 104, 20, 762, 1, 24, 0.3)
    bt = sc.part(P.motion(swing=7, pivot=(0, -300, 818), axis=0, rate=0.45, phase=0.6))
    for k in range(9):
        bt.box((LEM, COB, W)[k % 3], (-360 + k * 90, -300, 792), (44, 5, 50))


def boiler_house(sc):
    """The boiler house: 9.6 x 7.2 m, 13 m tall, a mint tank on its roof, a fat pipe down the front, a tall white
    chimney with a mushroom cap. Steam stands over the chimney."""
    g = sc.still()
    yf = -290
    zbox(g, STONE, 0, 0, -250, 60, 920, 700)
    zbox(g, W, 0, 20, 60, 640, 860, 620)
    for sx in (-1, 1):
        g.cyl(W, (sx * 430, yf, 60), 44, 580, steps=6)
    arch(g, COB, -40, yf, 60, 220, 380)                                  # the double door, a sun for a fan light
    zbox(g, W, -40, yf - 8, 60, 330, 8, 6)
    yrod(g, LEM, -40, yf - 10, yf - 5, 332, 58, 8, glow=G_WIN)
    for x in (-300, 250):
        yrod(g, COB, x, yf - 5, yf + 2, 400, 62, 8)
        yrod(g, PANE, x, yf - 8, yf, 400, 50, 8)
    zbox(g, MINT, -300, yf - 3, 60, 150, 250, 6)                         # a dado of tiles either side of the door
    zbox(g, MINT, 250, yf - 3, 60, 150, 340, 6)
    zbox(g, COB, 0, 20, 640, 680, 900, 660)
    for x in (-60, 260):                                                 # the tank in its cradles
        zbox(g, W, x, 60, 680, 770, 50, 300)
    xrod(g, MINT, -210, 400, 60, 880, 165, 12)
    for x in (-150, 100, 340):
        xrod(g, COB, x - 12, x + 12, 60, 880, 170, 12)
    for (x, s) in ((-210, 1), (400, 1)):
        g.sphere(MINT, (x, 60, 880), 163, scale=(0.4, 1.0, 1.0), steps=(5, 10))
    yrod(g, W, 225, -114, -102, 905, 44, 8)                              # a gauge on the tank, blank but for its needle
    zbox(g, PANE, 225, -117, 905, 940, 6, 5)
    g.cyl(MINT, (100, 60, 1040), 40, 40, steps=8)
    g.sphere(BRASS, (100, 60, 1088), 16, steps=(3, 6))
    g.cyl(MINT, (380, -312, -100), 30, 980, steps=8)                     # the fat pipe down the front
    yrod(g, MINT, 380, -312, -60, 880, 30, 8)
    wheel(g, 380, -348, 250, 46)
    g.cyl(W, (-300, 150, 680), 75, 520, steps=8, top_r=55)               # the chimney
    g.cyl(COB, (-300, 150, 1020), 68, 30, steps=8)
    g.cyl(MINT, (-300, 150, 1200), 55, 30, steps=8, top_r=110)
    dome(g, MINT, -300, 150, 1230, 110, 0.45, (4, 8))
    puffs(sc, -300, 150, 1350, 3, 46, 0.2)
    for k in range(6):                                                   # the outside stair up the left end
        zbox(g, STONE, -480, -250 + k * 70, -250, 60 + (k + 1) * 50, 100, 70)
    g.box(LEM, (110, yf - 12, 420), (24, 16, 30), glow=G_LAMP)


# ------------------------------------------------------------------------------------------ buildings, shallow (70 m)
def pump_room(sc):
    """The pump room: a drum 5 m across in two tiers with a ring of round windows, 14 m tall; the pump and its brass
    wheel stand beside it. The wheel rocks, a pennant turns, steam stands over a pipe."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -250), 235, 310, steps=12)
    g.cyl(W, (0, 0, 60), 212, 560, steps=12)
    g.cyl(MINT, (0, 0, 330), 217, 34, steps=12)
    g.cyl(COB, (0, 0, 620), 228, 36, steps=12)
    g.cyl(W, (0, 0, 656), 168, 400, steps=12)
    for a in (-66, -33, 0, 33, 66):
        rwin(g, 0, 0, 850, 168, a, 36)
    for a in (-52, 52):
        rwin(g, 0, 0, 480, 212, a, 30)
    g.cyl(COB, (0, 0, 1056), 185, 30, steps=12)
    dome(g, COB, 0, 0, 1086, 180, 0.72, (6, 12))
    g.cyl(W, (0, 0, 1200), 36, 70, steps=8)
    dome(g, COB, 0, 0, 1270, 46, 0.8, (4, 8))
    pennant(sc, 0, 0, 1300, LEM, 0.15)
    arch(g, COB, 0, -209, 60, 110, 250)
    xrod(g, LEM, -80, 80, -212, 338, 44, 8)                              # a half-round awning over the door
    g.box(LEM, (96, -214, 250), (20, 14, 26), glow=G_LAMP)
    zbox(g, MINT, 300, -40, -100, 215, 70, 70)                           # the pump
    xrod(g, MINT, 190, 300, -40, 150, 16, 8)
    g.sphere(MINT, (300, -40, 215), 40, scale=(1.0, 1.0, 0.6), steps=(4, 8))
    wh = sc.part(P.motion(swing=170, pivot=(300, -84, 250), axis=90, rate=0.06, phase=0.1))
    wheel(wh, 300, -84, 250, 76)
    top = mushroom(g, 190, 55, 656, 300, 13)
    puffs(sc, 190, 55, top + 50, 2, 32, 0.5)


def guest_house(sc):
    """The guest house: three storeys, 8.4 x 4.6 m, 17 m tall: stacked balconies, lemon shutters, a cobalt barrel
    vault, an outside stair. The top awning lifts, the weather vane turns."""
    g = sc.still()
    yf = -190
    zbox(g, STONE, 0, 0, -250, 60, 780, 440)
    zbox(g, W, 0, 10, 60, 1290, 720, 400)
    for f in range(3):
        z0 = 60 + f * 410
        for k in range(3):
            x = -230 + k * 230
            if f == 0 and k == 1:
                arch(g, COB, x, yf, z0, 120, 270)
            else:
                arch(g, PANE, x, yf, z0 + 110, 84, 200)
                for sx in (-1, 1):
                    zbox(g, LEM, x + sx * 58, yf - 3, z0 + 110, z0 + 270, 24, 6)
        if f:
            zbox(g, W, 0, yf - 35, z0 - 14, z0, 760, 70)                 # a balcony and its rail
            zbox(g, COB, 0, yf - 67, z0, z0 + 70, 760, 6)
            for sx in (-1, 1):
                zbox(g, COB, sx * 377, yf - 35, z0, z0 + 70, 6, 64)
    aw = sc.part(P.motion(swing=3, pivot=(0, yf, 1215), axis=0, rate=0.3, phase=0.5))
    awning(aw, -360, 360, yf - 2, 1215, 76, 36, 8)
    awning(g, -84, 84, yf - 2, 390, 64, 28, 2)
    g.box(LEM, (96, yf - 10, 290), (20, 14, 26), glow=G_LAMP)
    zbox(g, COB, 0, 10, 1290, 1320, 760, 440)
    xrod(g, COB, -360, 360, 10, 1320, 200, 10)                           # the barrel vault
    for k in range(6):                                                   # the outside stair up the right end
        zbox(g, STONE, 405, -160 + k * 62, -250, 60 + (k + 1) * 68, 90, 62)
    mushroom(g, -260, 10, 1490, 150, 13)
    g.cyl(BRASS, (150, 10, 1500), 4, 175, steps=6)                       # the weather vane: a kettle, not an arrow
    xrod(g, BRASS, 105, 195, 10, 1590, 3, 6)
    yrod(g, BRASS, 150, -35, 55, 1590, 3, 6)
    v = sc.part(P.motion(swing=50, pivot=(150, 10, 1650), axis=P.UP, rate=0.12, phase=0.3))
    v.box(COB, (150, 10, 1650), (64, 5, 34))
    tilt_box(v, COB, (182, 10, 1650), (210, 10, 1676), 5, 10)
    v.box(COB, (112, 10, 1652), (16, 5, 22))
    v.sphere(LEM, (150, 10, 1676), 8, steps=(3, 6))


def laundry(sc):
    """The laundry: 13 x 4.8 m, a whitewashed barrel vault with four steam chimneys, a line of washing along its
    front, 13 m to the caps. The washing swings, steam stands over two chimneys."""
    g = sc.still()
    yf = -190
    zbox(g, STONE, 0, 0, -250, 60, 1300, 440)
    zbox(g, W, 0, 10, 60, 720, 1240, 400)
    for k in range(5):
        x = -480 + k * 240
        if k == 2:
            arch(g, COB, x, yf, 60, 150, 300)
        else:
            arch(g, PANE, x, yf, 150, 130, 240)
            zbox(g, MINT, x, yf - 3, 110, 150, 150, 6)
    for k in range(5):
        x = -480 + k * 240
        yrod(g, COB, x, yf - 5, yf + 2, 590, 42, 8)
        yrod(g, PANE, x, yf - 8, yf, 590, 32, 8)
    zbox(g, COB, 0, 10, 720, 750, 1270, 430)
    xrod(g, W, -620, 620, 10, 750, 200, 10)
    for sx in (-1, 1):
        xrod(g, COB, sx * 618, sx * 630, 10, 750, 204, 10)
    for k in range(4):
        top = mushroom(g, -450 + k * 300, 10, 930, 270 + (k % 2) * 60, 21)
        if k % 2 == 0:
            puffs(sc, -450 + k * 300, 10, top + 55, 2, 38, 0.3 * k)
    for sx in (-1, 1):                                                   # the washing line under the eaves
        g.cyl(COB, (sx * 600, -235, -60), 6, 580, steps=6)
    xrod(g, STONE, -600, 600, -235, 505, 3, 6)
    cols = (W, COB, LEM, MINT, W, LEM, COB, MINT)
    for k in range(8):
        t = sc.part(P.motion(swing=8 + 3 * (k % 3), pivot=(0, -235, 505), axis=0, rate=0.34 + 0.05 * (k % 3), phase=0.21 * (k % 3)))
        t.box(cols[k], (-525 + k * 150, -235, 438), (96, 5, 134))
    awning(g, -110, 110, yf - 2, 410, 60, 30, 4)


def changing_towers(sc):
    """Three round changing towers joined by two bridges: 12.2 x 3.7 m, 13 m tall, cobalt domes, striped curtain
    doors. Pennants turn, towels on the bridge rails swing."""
    g = sc.still()
    for k, x in enumerate((-440, 0, 440)):
        h = (780, 1000, 780)[k]
        g.cyl(STONE, (x, 0, -250), 172, 310, steps=10)
        g.cyl(W, (x, 0, 60), 150, h, steps=10)
        g.cyl(MINT, (x, 0, 60 + h * 0.45), 155, 30, steps=10)
        g.cyl(COB, (x, 0, 60 + h), 166, 26, steps=10)
        dome(g, COB, x, 0, 86 + h, 160, 0.75, (5, 10))
        arch(g, LEM, x, -147, 60, 96, 210)                               # a curtain, lemon and white
        for sx in (-1, 1):
            zbox(g, W, x + sx * 22, -154, 60, 215, 14, 5)
        rwin(g, x, 0, h - 90, 150, 0, 30)
        pennant(sc, x, 0, 86 + h + 105, (LEM, MINT, LEM)[k], 0.3 * k)
    for sx in (-1, 1):
        zbox(g, W, sx * 220, 0, 540, 566, 190, 90)
        for y in (-42, 42):
            zbox(g, COB, sx * 220, y, 566, 632, 150, 6)
        t = sc.part(P.motion(swing=10, pivot=(0, -47, 632), axis=0, rate=0.4, phase=0.25 + 0.25 * sx))
        t.box(LEM if sx > 0 else MINT, (sx * 220 - 34, -48, 596), (44, 5, 72))
        t.box(W, (sx * 220 + 30, -48, 590), (44, 5, 84))


# ------------------------------------------------------------------------------------------ the hero
def steam_clock(sc):
    """THE STEAM CLOCK: a whitewashed bath-house tower 38 m tall on three steps, a cobalt dome, a great blank clock
    face on three sides (twelve lemon studs, no numerals), a mint kettle spout beside the dome. The hands sweep
    slowly, a brass pendulum swings in the shaft, steam stands over the spout (it rises straight up; nothing is aimed)."""
    g = sc.still()
    zbox(g, STONE, 0, 0, -300, 80, 1600, 1600)
    zbox(g, STONE, 0, 0, 80, 160, 1400, 1400)
    zbox(g, STONE, 0, 0, 160, 240, 1200, 1200)
    zbox(g, W, 0, 0, 240, 860, 1000, 1000)                               # the bath hall
    for x in (-300, 0, 300):
        arch(g, COB if x == 0 else PANE, x, -500, 240, 170, 400)
    for x in (-150, 150):
        yrod(g, LEM, x, -508, -499, 705, 34, 8, glow=G_WIN)
    for sx in (-1, 1):
        for y in (-250, 250):
            arch_x(g, PANE, sx * 500, sx, y, 240, 170, 400)
    zbox(g, MINT, 0, 0, 760, 800, 1010, 1010)
    zbox(g, COB, 0, 0, 860, 900, 1060, 1060)
    for sx in (-1, 1):
        for sy in (-1, 1):
            dome(g, COB, sx * 395, sy * 395, 900, 130, 0.65, (4, 8))
            g.sphere(LEM, (sx * 395, sy * 395, 996), 16, steps=(3, 6))
    mushroom(g, -230, 400, 900, 260, 18)
    mushroom(g, 230, 400, 900, 200, 16)
    zbox(g, W, 0, 0, 900, 2600, 640, 640)                                # the shaft
    for z in (1300, 1800, 2300):
        zbox(g, MINT, 0, 0, z, z + 34, 652, 652)
    for z in (1000, 1450):
        arch(g, LEM, 0, -320, z, 90, 240, glow=G_WIN)
        for sx in (-1, 1):
            arch_x(g, PANE, sx * 320, sx, 0, z, 90, 240)
    arch(g, PANE, 0, -320, 1880, 190, 400)                               # the pendulum's window
    pd = sc.part(P.motion(swing=11, pivot=(0, -336, 2240), axis=90, rate=0.3, phase=0.0))
    pd.box(BRASS, (0, -336, 2105), (14, 8, 270))
    rod(pd, BRASS, (0, -342, 1975), (0, -330, 1975), 56, 10)
    zbox(g, COB, 0, 0, 2600, 2640, 860, 860)
    zbox(g, W, 0, 0, 2640, 3290, 800, 800)                               # the clock stage
    zc = 2965.0
    yrod(g, COB, 0, -412, -398, zc, 310, 16)                             # the face to the road
    yrod(g, W, 0, -420, -410, zc, 272, 16)
    for k in range(12):
        a = math.radians(30.0 * k)
        s = 56 if k % 3 == 0 else 38
        g.box(LEM, (math.sin(a) * 228, -424, zc + math.cos(a) * 228), (s, 14, s), glow=G_STUD)
    mh = sc.part(P.motion(swing=176, pivot=(0, -432, zc), axis=90, rate=0.035, phase=0.1))
    mh.box(COB, (0, -432, zc + 100), (28, 8, 250))
    hh = sc.part(P.motion(swing=70, pivot=(0, -442, zc), axis=90, rate=0.012, phase=0.6))
    hh.box(COB, (0, -442, zc + 66), (40, 8, 170))
    yrod(g, BRASS, 0, -452, -420, zc, 30, 8)
    for sx in (-1, 1):                                                   # and one on each end
        xf = sx * 400.0
        rod(g, COB, (xf - sx * 2, 0, zc), (xf + sx * 12, 0, zc), 310, 16)
        rod(g, W, (xf + sx * 10, 0, zc), (xf + sx * 20, 0, zc), 272, 16)
        for k in range(12):
            a = math.radians(30.0 * k)
            s = 56 if k % 3 == 0 else 38
            g.box(LEM, (xf + sx * 24, math.sin(a) * 228, zc + math.cos(a) * 228), (14, s, s), glow=G_STUD)
        m2 = sc.part(P.motion(swing=176, pivot=(xf + sx * 32, 0, zc), axis=0, rate=0.035, phase=0.1 + 0.2 * sx))
        m2.box(COB, (xf + sx * 32, 0, zc + 100), (8, 28, 250))
        h2 = sc.part(P.motion(swing=70, pivot=(xf + sx * 42, 0, zc), axis=0, rate=0.012, phase=0.6 + 0.2 * sx))
        h2.box(COB, (xf + sx * 42, 0, zc + 66), (8, 40, 170))
        rod(g, BRASS, (xf + sx * 20, 0, zc), (xf + sx * 52, 0, zc), 30, 8)
    zbox(g, COB, 0, 0, 3290, 3330, 860, 860)
    g.cyl(W, (0, 0, 3330), 300, 140, steps=12)
    for a in (-60, -30, 0, 30, 60):
        rwin(g, 0, 0, 3400, 300, a, 34)
    dome(g, COB, 0, 0, 3470, 330, 0.75, (7, 14))
    g.cyl(W, (0, 0, 3700), 50, 60, steps=8)
    g.sphere(LEM, (0, 0, 3790), 34, steps=(4, 8))
    rod(g, MINT, (280, 0, 3350), (470, 0, 3400), 42, 8)                  # the kettle spout: out of the drum, then straight up
    rod(g, MINT, (462, 0, 3385), (545, 0, 3530), 38, 8)
    rod(g, MINT, (545, 0, 3515), (550, 0, 3690), 34, 8)
    g.cyl(MINT, (550, 0, 3685), 32, 50, steps=8, top_r=58)
    g.cyl(BRASS, (550, 0, 3600), 38, 16, steps=8)
    puffs(sc, 550, 0, 3830, 3, 58, 0.15)


# ------------------------------------------------------------------------------------------ tall open props (26.7 m and more)
def parasol_row(sc):
    """Three lemon and white parasols on thin poles, two little tables between them: 5.9 m long, 3.2 m tall. Each sways."""
    g = sc.still()
    for k, x in enumerate((-230, 0, 230)):
        p = sc.part(P.motion(swing=2.5, pivot=(x, 0, 0), axis=0, rate=0.3, phase=0.31 * k))
        p.cyl(STONE, (x, 0, -80), 5, 345, steps=6)
        p.cyl(W if k == 1 else LEM, (x, 0, 262), 66, 40, steps=8, top_r=6)
        p.cyl(COB, (x, 0, 253), 67, 9, steps=8)
        p.sphere(COB, (x, 0, 306), 9, steps=(3, 6))
    for x in (-115, 115):
        g.cyl(W, (x, 10, -40), 6, 112, steps=6)
        g.cyl(W, (x, 10, 70), 32, 7, steps=8)
        g.cyl(LEM, (x - 8, 6, 77), 6, 10, steps=6)


def towel_line(sc):
    """A towel line between two cobalt posts, six towels, a basket: 5.6 m long, 3.5 m tall. Every towel swings on its own."""
    g = sc.still()
    for sx in (-1, 1):
        g.cyl(COB, (sx * 270, 0, -80), 6, 410, steps=6)
        g.sphere(LEM, (sx * 270, 0, 338), 11, steps=(3, 6))
    xrod(g, STONE, -270, 270, 0, 300, 3, 6)
    cols = (COB, W, LEM, MINT, W, COB)
    for k in range(6):
        x = -210 + k * 84
        t = sc.part(P.motion(swing=9 + 3 * (k % 3), pivot=(0, 0, 300), axis=0, rate=0.32 + 0.06 * (k % 3), phase=0.17 * k))
        t.box(cols[k], (x, 0, 238), (64, 5, 124))
        if k % 2 == 0:
            t.box(COB if cols[k] != COB else LEM, (x, 0, 200), (64, 8, 14))
    g.cyl(WOOD, (180, 20, -30), 30, 70, steps=8, top_r=36)
    zbox(g, W, 180, 20, 40, 52, 40, 40)


def sun_arch(sc):
    """The springs' sign arch: two thin white posts, a mint and white hoop, a blank round board with a lemon sun
    hanging in it (a shape, no words): 4.8 m wide, 5.9 m tall. The board sways."""
    g = sc.still()
    R, zs, n = 230.0, 330.0, 8
    for sx in (-1, 1):
        g.cyl(W, (sx * R, 0, -80), 10, zs + 80, steps=8)
        zbox(g, COB, sx * R, 0, -80, 36, 40, 40)
    pts = [(math.cos(math.pi * k / n) * R, zs + math.sin(math.pi * k / n) * R) for k in range(n + 1)]
    for k in range(n):
        tilt_box(g, MINT if k % 2 else W, (pts[k][0], 0, pts[k][1]), (pts[k + 1][0], 0, pts[k + 1][1]), 14, 22)
    g.sphere(LEM, (0, 0, zs + R + 22), 14, steps=(3, 6))
    zc = zs + R - 172.0
    bd = sc.part(P.motion(swing=4, pivot=(0, 0, zs + R), axis=0, rate=0.3, phase=0.5))
    for x in (-50, 50):
        bd.box(BRASS, (x, 0, zs + R - 44), (5, 5, 90))
    yrod(bd, COB, 0, -5, 5, zc, 96, 12)
    yrod(bd, W, 0, -9, 9, zc, 80, 12)
    yrod(bd, LEM, 0, -13, 13, zc, 32, 8)
    for k in range(8):
        a = math.radians(45.0 * k)
        bd.box(LEM, (math.cos(a) * 54, 0, zc + math.sin(a) * 54), (13, 24, 13))


def steam_vane(sc):
    """A thin mint steam pipe with a mushroom cap, a valve wheel and a weather vane in the shape of a kettle (not an
    arrow): 6.3 m tall. The vane turns slowly. No steam on this one: it stands by the road."""
    g = sc.still()
    zbox(g, W, 0, 0, -80, 40, 56, 56)
    g.cyl(MINT, (0, 0, 40), 9, 400, steps=8)
    for z in (150, 300):
        g.cyl(COB, (0, 0, z), 12, 14, steps=8)
    g.cyl(MINT, (0, 0, 440), 10, 20, steps=8, top_r=40)
    g.sphere(MINT, (0, 0, 460), 40, scale=(1.0, 1.0, 0.45), steps=(4, 8))
    g.cyl(BRASS, (0, 0, 470), 4, 135, steps=6)
    xrod(g, BRASS, -45, 45, 0, 520, 3, 6)
    yrod(g, BRASS, 0, -45, 45, 520, 3, 6)
    for (x, y) in ((-45, 0), (45, 0), (0, -45), (0, 45)):
        g.sphere(LEM, (x, y, 520), 8, steps=(3, 6))
    wheel(g, 0, -15, 110, 24)
    v = sc.part(P.motion(swing=50, pivot=(0, 0, 575), axis=P.UP, rate=0.1, phase=0.2))
    v.box(COB, (0, 0, 575), (70, 5, 36))
    tilt_box(v, COB, (34, 0, 575), (64, 0, 602), 5, 10)
    v.box(COB, (-42, 0, 578), (16, 5, 24))
    v.sphere(LEM, (0, 0, 602), 9, steps=(3, 6))


# ------------------------------------------------------------------------------------------ low props (23.6 m; under 2 m, still)
def plunge_rim(sc):
    """A tiled plunge pool's rim with a step and a brass handrail, pale spring water in it, towels and a bucket on
    the rim: 5.4 x 1.4 m, 1.3 m to the rail."""
    g = sc.still()
    for y in (-55, 55):
        zbox(g, W, 0, y, -60, 58, 420, 22)
    for sx in (-1, 1):
        zbox(g, W, sx * 199, 0, -60, 58, 22, 90)
    zbox(g, WATER, 0, 0, -60, 46, 378, 90)
    zbox(g, COB, 0, -67.5, 28, 46, 420, 5)
    zbox(g, MINT, 0, -67.5, 8, 24, 420, 5)
    zbox(g, STONE, 242, 0, -60, 30, 64, 100)
    for y in (-40, 40):
        g.cyl(BRASS, (236, y, 30), 4, 100, steps=6)
    rod(g, BRASS, (236, -40, 128), (236, 40, 128), 4, 6)
    zbox(g, LEM, -150, -55, 58, 80, 60, 30)
    zbox(g, COB, -150, -55, 80, 92, 56, 28)
    g.cyl(MINT, (-240, -30, -20), 16, 56, steps=6, top_r=20)


def spring_tap(sc):
    """A drinking spring: a round whitewashed basin under a mint swan-neck tap, a cobalt tiled back with a lemon sun,
    a brass wheel, three cups: 1.7 m wide, 1.8 m tall. One of them, never a row."""
    g = sc.still()
    g.cyl(W, (0, 0, -60), 58, 112, steps=10)
    g.cyl(WATER, (0, 0, 49), 46, 6, steps=10)
    zbox(g, COB, 0, 52, -60, 100, 170, 14)
    yrod(g, COB, 0, 45, 59, 100, 80, 10)
    yrod(g, W, 0, 41, 47, 100, 60, 10)
    yrod(g, LEM, 0, 37, 43, 104, 24, 8)
    g.cyl(MINT, (0, 30, 50), 7, 92, steps=6)
    rod(g, MINT, (0, 32, 138), (0, -14, 150), 7, 6)
    rod(g, MINT, (0, -14, 152), (0, -16, 126), 7, 6)
    wheel(g, 34, 24, 96, 15)
    for (x, y) in ((-40, -30), (-22, -44), (38, -34)):
        g.cyl(LEM, (x, y, 52), 7, 11, steps=6)


def tub_stack(sc):
    """Six wooden bath tubs stacked three, two, one, cobalt hoops, a towel on top, a long brush: 3.8 x 1.2 m, 1.5 m tall."""
    g = sc.still()

    def tub(x, z):
        g.cyl(WOOD, (x, 0, z), 48, 46, steps=8, top_r=58)
        g.cyl(COB, (x, 0, z + 30), 57, 7, steps=8)

    for x in (-130, 0, 130):
        tub(x, -4)
    for x in (-65, 65):
        tub(x, 42)
    tub(0, 88)
    zbox(g, LEM, 0, 0, 134, 144, 70, 56)
    rod(g, WOOD, (170, 0, -10), (200, 0, 120), 4, 6)
    zbox(g, W, 204, 0, 112, 132, 30, 12)


def loungers(sc):
    """Three blue and white deck loungers side on to the road, a little table with a cup: 6 x 0.6 m, 0.9 m tall."""
    g = sc.still()
    for x in (-210, 0, 210):
        for j in range(4):
            zbox(g, COB if j % 2 else W, x - 60 + j * 30, 0, 26, 34, 30, 58)
        tilt_box(g, W, (x + 72, 0, 32), (x + 112, 0, 84), 58, 8)
        tilt_box(g, COB, (x + 85, -1, 50), (x + 99, -1, 68), 60, 9)
        for lx in (-64, 62):
            zbox(g, WOOD, x + lx, 0, -40, 26, 8, 56)
    g.cyl(W, (-105, 0, -40), 5, 96, steps=6)
    g.cyl(W, (-105, 0, 54), 26, 6, steps=8)
    g.cyl(LEM, (-110, -6, 60), 6, 10, steps=6)


def tea_bench(sc):
    """The tea server's bench: a white table on cobalt legs, a fat mint kettle with a cobalt handle, four lemon
    cups, a stack of towels, a bucket: 2.3 x 0.6 m, 1.7 m tall."""
    g = sc.still()
    zbox(g, W, 0, 0, 62, 72, 230, 62)
    for sx in (-1, 1):
        zbox(g, COB, sx * 95, 0, -50, 62, 12, 54)
    zbox(g, STONE, 0, 0, 10, 18, 190, 48)
    g.cyl(MINT, (-50, 0, 72), 26, 8, steps=8)
    g.sphere(MINT, (-50, 0, 114), 40, steps=(4, 8))
    tilt_box(g, MINT, (-20, 0, 106), (14, 0, 140), 12, 12)
    g.sphere(LEM, (-50, 0, 156), 9, steps=(3, 6))
    hp = ((-84, 138), (-68, 170), (-32, 170), (-16, 138))
    for k in range(3):
        tilt_box(g, COB, (hp[k][0], 0, hp[k][1]), (hp[k + 1][0], 0, hp[k + 1][1]), 8, 8)
    for k in range(4):
        g.cyl(LEM, (26 + k * 22, -12, 72), 8, 12, steps=6)
    zbox(g, COB, 92, 10, 72, 84, 44, 36)
    zbox(g, W, 92, 10, 84, 96, 42, 34)
    zbox(g, LEM, 92, 10, 96, 108, 40, 32)
    g.cyl(MINT, (40, 0, 18), 16, 34, steps=6, top_r=20)


JOBS = [('SM_S1D1_BathHouse', bath_house), ('SM_S1D1_TeaPavilion', tea_pavilion), ('SM_S1D1_BoilerHouse', boiler_house),
        ('SM_S1D1_PumpRoom', pump_room), ('SM_S1D1_GuestHouse', guest_house), ('SM_S1D1_Laundry', laundry),
        ('SM_S1D1_ChangingTowers', changing_towers),
        ('SM_S1D1_SteamClock', steam_clock, dict(far=True)),      # review round: 134 / 138 m out, the hands stopped past 180 m
        ('SM_S1D1_ParasolRow', parasol_row), ('SM_S1D1_TowelLine', towel_line), ('SM_S1D1_SunArch', sun_arch),
        ('SM_S1D1_SteamVane', steam_vane),
        ('SM_S1D1_PlungeRim', plunge_rim, STILL), ('SM_S1D1_SpringTap', spring_tap, STILL), ('SM_S1D1_TubStack', tub_stack, STILL),
        ('SM_S1D1_Loungers', loungers, STILL), ('SM_S1D1_TeaBench', tea_bench, STILL)]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
