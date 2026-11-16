"""City Center, Stage 1, builder s1d7: district 7 "Syndicate Territory, the arena" (7.66 to 13.56 km), the Chairman's
quarter. /Game/World/CityCenter/SM_S1D7_*.   Run: import ft_world_city_s1d7 as K; K.run()

Fortress blocks: battered black walls, gold-lit slit windows, heavy flat cornices, buttress fins. EVERY piece is STILL
(the district's house rule: nothing near the road moves; the boss fight is driven here). Light is steady and only on
pieces the table stands 24 m and more out. No chevron (the kit's own mark), no spire, no vehicle, no gun.

Where the table (C:/ftwork/s1/L7_s1d7.inl) stands them:
  kerb, 21.8 m out, behind the barrier rows     BollardRow, StrongboxCart, MountingBlock, LedgerTable (low, unlit)
  on the black wall's roof (9 m up), left       Strongroom, Mint, ClerksBastion, LanternCluster, PennantRow, RoofVents
  in the 13 m gap between two podiums           TollHouse (behind the banners), LedgerArchive (behind a checkpoint),
                                                SentryBox, LanternCluster
  62 to 68 m out, over the first row            CountingTower, KeepTower
  178 m out, left, three times                  Vault (the hero: a 110 m black cube, one face a round gold vault door)
Drawn in cm, front -Y, base z = 0 (a footing below), X along the road. One section, material M_People (ft_people).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))     # no moving part anywhere: true bounds
BLACK, GOLD, RED, IRON, SILVER, OX = '#1c1a26', '#ffc93c', '#ff2d3c', '#3f3d50', '#c9c4d6', '#54343c'
G1, G2, G3, G4 = 0.35, 0.9, 1.4, 2.0                  # the kit's steps of glow: soft, a lit window, a sign, a bulb


def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def trap(g, col, w0, w1, z0, z1, y0, y1, x=0.0, glow=0.0):
    """A battered wall seen from the front: w0 wide at z0, w1 wide at z1, from y0 to y1."""
    g.side(col, _ccw([(x - w0 / 2.0, z0), (x + w0 / 2.0, z0), (x + w1 / 2.0, z1), (x - w1 / 2.0, z1)]), y0, y1, glow=glow)


def trap_y(sc, col, d0, d1, z0, z1, hx, glow=0.0):
    """The same turned a quarter: d0 deep at z0, d1 deep at z1 (front and back slope), from x = -hx to +hx."""
    g = sc.still(yaw=90.0)
    g.side(col, _ccw([(-d0 / 2.0, z0), (d0 / 2.0, z0), (d1 / 2.0, z1), (-d1 / 2.0, z1)]), -hx, hx, glow=glow)
    return sc.still()


def fin_y(sc, col, xs, t, yb, out, z0, z1, glow=0.0):
    """Buttress fins on the front AND the back, at x = +x and -x for each x in xs: a triangle standing out from the
    wall (|y| = yb) by 'out' at z0 and by nothing at z1, t thick."""
    g = sc.still(yaw=90.0)
    for x in xs:
        for sx in (-1, 1):
            for s in (-1, 1):
                g.side(col, _ccw([(s * yb, z0), (s * (yb + out), z0), (s * yb, z1)]), sx * x - t / 2.0, sx * x + t / 2.0, glow=glow)
    return sc.still()


def fin_x(g, col, ys, t, xb, out, z0, z1, glow=0.0):
    """Buttress fins on both ends (|x| = xb), at each y in ys."""
    for y in ys:
        for s in (-1, 1):
            g.side(col, _ccw([(s * xb, z0), (s * (xb + out), z0), (s * xb, z1)]), y - t / 2.0, y + t / 2.0, glow=glow)


def slits(g, r, xs, y, zs, w, h, p=0.85, glow=G2):
    """Slit windows on a face that looks along Y (the front: y = the face): lit gold, or dark iron."""
    for z in zs:
        for x in xs:
            lit = r.random() < p
            g.box(GOLD if lit else IRON, (x, y, z), (w, 8, h), glow=glow if lit else 0.0)


def slits_x(g, r, x, ys, zs, w, h, p=0.85, glow=G2):
    """Slit windows on an end face (x = the face)."""
    for z in zs:
        for y in ys:
            lit = r.random() < p
            g.box(GOLD if lit else IRON, (x, y, z), (8, w, h), glow=glow if lit else 0.0)


def skirt(sc, col, w, d, z0, z1, spread=70.0):
    """The battered foot of a roof block: wider by 'spread' each way at z0."""
    g = sc.still()
    trap(g, col, w + 2 * spread, w, z0, z1, -d / 2.0 + 5, d / 2.0 - 5)
    return trap_y(sc, col, d + 2 * spread, d, z0, z1, w / 2.0 - 5)


def key_vane(g, x, y, z, h=420.0):
    """A weather vane: a plain gold key lying on an iron pole (a square bow, a shaft, two teeth)."""
    g.box(IRON, (x, y, z + h / 2.0), (26, 26, h))
    g.box(GOLD, (x, y, z + h + 8), (60, 60, 16))
    zz = z + h + 60
    g.box(GOLD, (x + 30, y, zz), (300, 16, 36))
    for dx, dz, sx, sz in ((-180, 0, 28, 150), (-90, 0, 28, 150), (-135, 61, 118, 28), (-135, -61, 118, 28)):
        g.box(GOLD, (x + dx, y, zz + dz), (sx, 16, sz))
    for dx, hh in ((120, 80), (165, 56)):
        g.box(GOLD, (x + dx, y, zz - 18 - hh / 2.0), (30, 16, hh))


# ------------------------------------------------------------------------------------------ buildings
def counting_tower(sc):
    """BUILDING, far (62 m and more out): a battered black counting tower 96 m tall, gold slit windows row on row,
    a heavy flat gold cornice, iron buttress fins, its corners edged in steady gold."""
    r = random.Random(71)
    W, H = 1600.0, 9200.0
    g = sc.still()
    g.box(BLACK, (0, 0, (H - 300) / 2.0), (W, W, H + 300))
    trap(g, BLACK, 2500, W, -300, 2800, -W / 2.0 + 10, W / 2.0 - 10)
    g = trap_y(sc, BLACK, 2500, W, -300, 2800, W / 2.0 - 10)
    g.box(GOLD, (0, 0, 2835), (W + 70, W + 70, 70), glow=G1)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(GOLD, (sx * W / 2.0, sy * W / 2.0, (H + 2900) / 2.0), (70, 70, H - 2900), glow=G1)
    zs = [3400 + k * 690 for k in range(8)]
    slits(g, r, (-510, -170, 170, 510), -W / 2.0 - 2, zs, 100, 400)
    for s in (-1, 1):
        slits_x(g, r, s * (W / 2.0 + 2), (-420, 0, 420), zs, 100, 400)
    g.box(RED, (0, -W / 2.0 - 2, 3060), (900, 8, 60), glow=G3)
    g.box(GOLD, (0, 0, H + 80), (W + 520, W + 520, 160), glow=G1)
    g.box(BLACK, (0, 0, H + 270), (W + 320, W + 320, 220))
    g.box(BLACK, (0, 0, H + 680), (900, 900, 600))
    g.box(GOLD, (0, 0, H + 1010), (1000, 1000, 60), glow=G1)
    g.box(IRON, (0, 0, H + 1100), (300, 300, 120))
    g = fin_y(sc, IRON, (520,), 130, W / 2.0, 760, -300, 5200)
    fin_x(g, IRON, (-520, 520), 130, W / 2.0, 760, -300, 5200)


def keep_tower(sc):
    """BUILDING, far: the twin keep. Two black shafts 78 m tall on one battered iron plinth, joined by an oxblood
    link below and a bridge above with one great gold-lit grille; flat gold cornices, oxblood attics."""
    r = random.Random(72)
    g = sc.still()
    trap(g, IRON, 3700, 3000, -300, 1600, -690, 690)
    g = trap_y(sc, IRON, 2100, 1400, -300, 1600, 1490)
    g.box(GOLD, (0, 0, 1630), (3050, 1450, 60), glow=G1)
    zs = [2150 + k * 640 for k in range(9)]
    for s in (-1, 1):
        x = s * 900.0
        g.box(BLACK, (x, 0, 4700), (1100, 1300, 6200))
        slits(g, r, (x - 270, x + 270), -652, zs, 110, 390)
        slits_x(g, r, s * 1452, (-320, 320), zs, 110, 390)
        g.box(GOLD, (s * 1450, -650, 4700), (60, 60, 6200), glow=G1)
        g.box(RED, (x, -652, 7640), (800, 8, 50), glow=G3)
        g.box(GOLD, (x, 0, 7870), (1420, 1620, 140), glow=G1)
        g.box(IRON, (x, 0, 8040), (1300, 1500, 200))
        g.box(OX, (x, 0, 8390), (800, 1000, 500))
        g.box(GOLD, (x, 0, 8670), (900, 1100, 60), glow=G1)
    g.box(OX, (0, 0, 2300), (720, 900, 1400))
    g.box(GOLD, (0, -452, 2050), (380, 8, 820))
    for x in (-95, 95):
        g.box(BLACK, (x, -458, 2050), (36, 12, 820))
    g.box(BLACK, (0, -458, 2250), (380, 12, 36))
    g.box(BLACK, (0, 0, 6500), (720, 1000, 1400))
    g.box(GOLD, (0, -502, 6500), (580, 8, 760), glow=G2)
    for x in (-150, 0, 150):
        g.box(BLACK, (x, -508, 6500), (34, 12, 760))
    g.box(GOLD, (0, 0, 7230), (760, 1040, 60), glow=G1)
    g = fin_y(sc, IRON, (900,), 130, 650, 640, -300, 4300)
    fin_x(g, IRON, (0,), 130, 1450, 640, -300, 4300)


def strongroom(sc):
    """BUILDING for a black wall's roof: a windowless strongroom 7.6 x 4.8 m and 12.5 m tall, gold buttress fins all
    round, a barred gold door plate under a steady red lamp, a heavy flat cornice, two iron cowls on its roof."""
    g = sc.still()
    g.box(BLACK, (0, 0, 525), (760, 480, 1250))
    g = skirt(sc, IRON, 760, 480, -100, 300)
    g = fin_y(sc, GOLD, (130, 330), 50, 240, 110, 0, 1000, glow=G1)
    fin_x(g, GOLD, (-120, 120), 50, 380, 130, 0, 1000, glow=G1)
    g.box(GOLD, (0, 0, 1165), (930, 650, 30), glow=G1)
    g.box(IRON, (0, 0, 1225), (900, 620, 90))
    g.box(GOLD, (0, -242, 450), (190, 8, 420))
    g.box(BLACK, (0, -248, 450), (24, 12, 420))
    for z in (340, 560):
        g.box(BLACK, (0, -248, z), (190, 12, 24))
    for x in (-62, 62):
        for z in (290, 610):
            g.box(SILVER, (x, -250, z), (30, 14, 30))
    g.box(BLACK, (0, -250, 720), (110, 20, 60))
    g.box(RED, (0, -262, 720), (60, 10, 34), glow=G3)
    for x in (-200, 160):
        g.box(IRON, (x, 60, 1325), (120, 120, 110))
        g.box(IRON, (x, 60, 1395), (170, 170, 30))
        g.box(BLACK, (x, -2, 1325), (90, 8, 60))
    g.box(BLACK, (-20, 150, 1330), (200, 90, 120))
    g.box(GOLD, (-20, 150, 1396), (220, 110, 12))


def mint(sc):
    """BUILDING for a black wall's roof: the mint. A low black hall with a row of gold slits, three stepped black
    chimneys banded in gold (the middle one taller, a still pale puff over it), gold ingots stacked on its parapet."""
    r = random.Random(74)
    g = sc.still()
    g.box(BLACK, (0, 0, 250), (800, 540, 700))
    g = skirt(sc, IRON, 800, 540, -100, 260)
    slits(g, r, [-330 + k * 132 for k in range(6) if k != 3], -272, (400,), 80, 220, p=1.0)
    g.box(GOLD, (66, -272, 210), (130, 8, 380))
    g.box(BLACK, (66, -278, 210), (20, 12, 380))
    g.box(RED, (66, -276, 440), (60, 10, 30), glow=G3)
    g.box(GOLD, (0, 0, 615), (840, 580, 30), glow=G1)
    g.box(IRON, (0, 0, 655), (800, 540, 50))
    for x, k in ((-250, 1.0), (0, 1.25), (250, 1.0)):
        z = 680.0
        for w, h in ((200, 380 * k), (160, 340 * k), (124, 300 * k)):
            g.box(BLACK, (x, 70, z + h / 2.0), (w, w, h))
            g.box(GOLD, (x, 70, z + h + 16), (w + 22, w + 22, 32), glow=G1)
            z += h + 32
        g.box(IRON, (x, 70, z + 12), (150, 150, 24))
    for (x, z, rad) in ((0, 2290, 100), (55, 2440, 130), (-25, 2620, 165)):
        g.sphere(SILVER, (x, 70, z), rad, steps=(4, 6))
    for j, n in enumerate((3, 2, 1)):
        for i in range(n):
            g.box(GOLD, (240 + (i - (n - 1) / 2.0) * 96, -190, 697 + j * 34), (88, 44, 32))
    g.box(IRON, (240, -190, 676), (320, 70, 10))


def clerks_bastion(sc):
    """BUILDING for a black wall's roof: the clerks' bastion. A black block with two rows of gold slits, four corner
    turrets capped in gold, an oxblood roof, and a gable tower with a gold clock that has twelve marks and no hands."""
    r = random.Random(75)
    g = sc.still()
    g.box(BLACK, (0, 0, 400), (780, 540, 1000))
    g = skirt(sc, IRON, 780, 540, -100, 280)
    xs = [-325 + k * 130 for k in range(6)]
    slits(g, r, xs, -272, (640,), 70, 200, p=0.9)
    slits(g, r, xs[:2] + xs[4:], -272, (330,), 70, 200, p=0.9)
    g.box(GOLD, (0, -272, 190), (150, 8, 380))
    g.box(BLACK, (0, -278, 190), (20, 12, 380))
    g.box(GOLD, (0, 0, 912), (810, 570, 24), glow=G1)
    trap(g, OX, 820, 520, 924, 1180, -290, 290)
    g.box(BLACK, (0, -40, 1150), (300, 460, 500))
    rod(g, GOLD, (0, -270, 1150), (0, -284, 1150), 118, steps=8, glow=G1)
    for k in range(12):
        c, s = math.cos(math.radians(k * 30)), math.sin(math.radians(k * 30))
        tilt_box(g, BLACK, (78 * c, -287, 1150 + 78 * s), (106 * c, -287, 1150 + 106 * s), 10, 12)
    g.box(BLACK, (0, -288, 1150), (24, 10, 24))
    g.box(GOLD, (0, -40, 1412), (330, 490, 24), glow=G1)
    trap(g, OX, 340, 130, 1424, 1640, -280, 200)
    g.box(GOLD, (0, -40, 1700), (36, 36, 120))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(BLACK, (sx * 362, sy * 242, 1010), (110, 110, 300))
            g.box(GOLD, (sx * 362, sy * 242, 1172), (132, 132, 24))


def ledger_archive(sc):
    """BUILDING for a gap between two podiums: the ledger archive, a cabinet 11 m wide and 48 m tall of twelve black
    drawers, each with a gold handle and a blank silver label, a line of gold light under every drawer; a flat gold
    cornice and a key for a weather vane. AS BUILT (seed 76): five drawers stand out 40 cm, none further, so no papers
    show; another seed would pull some out 1.2 to 2.2 m (not rebuilt: one generate run a builder)."""
    r = random.Random(76)
    g = sc.still()
    trap(g, IRON, 1260, 1120, -300, 400, -250, 250)
    g.box(IRON, (0, 20, 2440), (1100, 420, 4080))
    for k in range(12):
        z = 400 + k * 340 + 170
        pull = r.choice((0, 0, 0, 40, 120, 220))
        yf = -230.0 - pull
        g.box(BLACK, (0, yf + (460 + pull) / 2.0, z), (1040, 460 + pull, 300))
        g.box(GOLD, (0, yf - 12, z - 40), (320, 28, 44))
        g.box(SILVER, (0, yf - 3, z + 70), (210, 8, 90))
        g.box(GOLD, (0, -192, z - 170), (1000, 8, 22), glow=G2)
        if pull >= 120:
            g.box(SILVER, (0, yf + pull / 2.0, z + 153), (900, pull - 30, 8))
    g.box(GOLD, (0, -20, 4540), (1200, 580, 120), glow=G1)
    g.box(BLACK, (0, -20, 4680), (1140, 520, 160))
    key_vane(g, 250, -20, 4760, h=480)
    for x in (-360, -180):
        g.box(IRON, (x, 0, 4820), (110, 110, 120))
        g.box(IRON, (x, 0, 4895), (150, 150, 30))


def toll_house(sc):
    """BUILDING for a gap between two podiums: the toll house. Two battered black buttress piers, a house bridging
    them with two rows of gold slits and a blank gold plate, a gold portcullis drawn up in the passage under it, two
    steady red lamps on the lintel, a flat gold cornice, an oxblood attic, a still gold pennant on a black pole."""
    r = random.Random(77)
    g = sc.still()
    for s in (-1, 1):
        trap(g, BLACK, 300, 230, -300, 1900, -260, 260, x=s * 430)
        g.box(GOLD, (s * 430, -262, 1300), (236, 8, 40))
    g.box(OX, (0, 200, 675), (640, 40, 1650))
    for k in range(6):
        g.box(GOLD, (-225 + k * 90, -200, 1130), (28, 28, 560))
        g.box(GOLD, (-225 + k * 90, -200, 830), (16, 16, 40))
    for z in (1000, 1250):
        g.box(GOLD, (0, -200, z), (540, 22, 28))
    g.box(IRON, (0, -252, 1385), (700, 40, 90))
    for x in (-250, 250):
        g.box(RED, (x, -276, 1385), (50, 10, 40), glow=G3)
    g.box(BLACK, (0, 0, 2000), (1150, 500, 1300))
    xs = [-480 + k * 160 for k in range(7)]
    slits(g, r, xs, -252, (1740,), 70, 240, p=0.9)
    slits(g, r, xs[:3] + xs[4:], -252, (2260,), 70, 240, p=0.9)
    g.box(GOLD, (0, -252, 2260), (170, 8, 170), glow=G1)
    g.box(GOLD, (0, 0, 2705), (1240, 590, 110), glow=G1)
    g.box(BLACK, (0, 0, 2835), (1180, 540, 150))
    g.box(OX, (0, 0, 3090), (700, 400, 360))
    g.box(GOLD, (0, 0, 3295), (770, 470, 50), glow=G1)
    g.box(BLACK, (0, 0, 3670), (24, 24, 700))
    g.box(GOLD, (0, 0, 4035), (40, 40, 30))
    g.box(GOLD, (72, 0, 3950), (120, 6, 80))
    for dz in (26, -26):
        g.box(GOLD, (152, 0, 3950 + dz), (44, 6, 28))
    fin_y(sc, IRON, (430,), 120, 260, 200, -300, 1300)


# ------------------------------------------------------------------------------------------ props, tall (24 m and more out)
def lantern_cluster(sc):
    """PROP, tall: a black iron lantern standard 5.8 m, four arms each with a hanging lantern of red glass and one
    larger lantern on top, gold collars and caps. The red glass is lit, steady."""
    g = sc.still()
    g.box(IRON, (0, 0, 5), (96, 96, 70))
    g.box(BLACK, (0, 0, 60), (70, 70, 60))
    g.box(BLACK, (0, 0, 275), (22, 22, 370))
    for z in (110, 300, 452):
        g.box(GOLD, (0, 0, z), (32, 32, 16))
    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        g.box(BLACK, (dx * 56, dy * 56, 436), (12 + abs(dx) * 90, 12 + abs(dy) * 90, 12))
        x, y = dx * 100, dy * 100
        g.box(BLACK, (x, y, 424), (8, 8, 16))
        g.box(GOLD, (x, y, 412), (44, 44, 10))
        g.box(RED, (x, y, 382), (32, 32, 50), glow=G3)
        g.box(BLACK, (x, y, 354), (38, 38, 8))
    g.box(BLACK, (0, 0, 466), (50, 50, 12))
    g.box(RED, (0, 0, 508), (46, 46, 72), glow=G3)
    g.box(GOLD, (0, 0, 550), (62, 62, 12))
    g.box(GOLD, (0, 0, 568), (22, 22, 24))


def pennant_row(sc):
    """PROP, tall: five black poles 5.2 m on an iron rail, each with a plain gold swallow-tailed pennant, still (no
    mark on the cloth), a gold knob on every pole."""
    g = sc.still()
    g.box(IRON, (0, 0, 0), (690, 40, 60))
    for k in range(5):
        x = -300 + k * 150
        g.box(BLACK, (x, 0, 250), (14, 14, 500))
        g.box(GOLD, (x, 0, 40), (26, 26, 14))
        g.box(GOLD, (x, 0, 511), (24, 24, 22))
        g.box(GOLD, (x + 47, 0, 452), (80, 6, 64))
        g.box(OX, (x + 47, 0, 452), (80, 8, 14))
        for dz in (20, -20):
            g.box(GOLD, (x + 104, 0, 452 + dz), (36, 6, 24))


def sentry_box(sc):
    """PROP, tall: an empty sentry box 3.3 m, black with plain gold bands, a stepped black and gold roof, an oxblood
    back wall, a stool and an unlit hand lantern on a hook inside. Nobody in it."""
    g = sc.still()
    g.box(IRON, (0, 0, -5), (156, 156, 50))
    g.box(BLACK, (0, 62, 130), (134, 10, 240))
    g.box(OX, (0, 55, 130), (112, 6, 220))
    for s in (-1, 1):
        g.box(BLACK, (s * 62, 0, 130), (10, 134, 240))
        for z in (60, 205):
            g.box(GOLD, (s * 64, 0, z), (12, 136, 14))
        g.box(BLACK, (s * 62, -62, 130), (12, 12, 240))
    g.box(BLACK, (0, -62, 232), (134, 12, 36))
    g.box(GOLD, (0, -66, 232), (136, 6, 12))
    g.box(BLACK, (0, 0, 258), (168, 168, 16))
    g.box(GOLD, (0, 0, 274), (124, 124, 16))
    g.box(BLACK, (0, 0, 290), (82, 82, 16))
    g.box(GOLD, (0, 0, 316), (22, 22, 36))
    g.box(IRON, (22, 20, 46), (34, 34, 6))
    g.box(IRON, (22, 20, 21), (10, 10, 44))
    g.box(IRON, (-38, 40, 190), (6, 30, 6))
    g.box(BLACK, (-38, 28, 170), (16, 16, 26))
    g.box(GOLD, (-38, 28, 185), (20, 20, 5))


def roof_vents(sc):
    """PROP, tall (for a roof): three iron cowl vents with black louvres on a pipe run with a gold valve wheel, and a
    key for a weather vane on a pole."""
    g = sc.still()
    for k, x in enumerate((-210, -60, 90)):
        h = 110 + k * 40
        g.box(IRON, (x, 0, h / 2.0 - 10), (76, 76, h + 20))
        g.box(IRON, (x, 0, h + 14), (110, 110, 28))
        for z in (h - 30, h - 60):
            g.box(BLACK, (x, -40, z), (60, 8, 16))
        g.box(GOLD, (x, 0, h + 31), (40, 40, 6))
    g.box(IRON, (-60, 60, 20), (460, 18, 18))
    g.box(IRON, (-285, 60, 10), (18, 18, 40))
    g.box(IRON, (165, 60, 10), (18, 18, 40))
    for sx, sz in ((44, 8), (8, 44)):
        g.box(GOLD, (20, 48, 40), (sx, 6, sz))
    key_vane(g, 250, 0, -10, h=330)


# ------------------------------------------------------------------------------------------ props, low (from 17 m; unlit)
def bollard_row(sc):
    """PROP, low (1.0 m): seven square black bollards with a gold band and a gold cap, a sagging iron chain between."""
    g = sc.still()
    for k in range(7):
        x = -300 + k * 100
        g.box(BLACK, (x, 0, 30), (26, 26, 120))
        g.box(GOLD, (x, 0, 66), (31, 31, 12))
        g.box(GOLD, (x, 0, 95), (18, 18, 10))
        if k < 6:
            tilt_box(g, IRON, (x + 13, 0, 78), (x + 50, 0, 60), 6, 6)
            tilt_box(g, IRON, (x + 50, 0, 60), (x + 87, 0, 78), 6, 6)


def strongbox_cart(sc):
    """PROP, low (1.65 m): an iron hand cart with six small strongboxes stacked three, two, one: black, two gold
    straps and a gold lock plate each (every box under 60 cm), a ledger on top."""
    g = sc.still()
    g.box(IRON, (0, 0, 42), (176, 78, 8))
    for s in (-1, 1):
        rod(g, IRON, (-45, s * 42, 30), (-45, s * 49, 30), 30, steps=8)
        g.box(GOLD, (-45, s * 51, 30), (14, 5, 14))
        g.box(IRON, (70, s * 30, 9), (7, 7, 78))
        tilt_box(g, IRON, (86, s * 34, 46), (138, s * 34, 96), 7, 7)
    g.box(BLACK, (138, 0, 96), (8, 76, 8))
    for j, n in enumerate((3, 2, 1)):
        for i in range(n):
            x, z = (i - (n - 1) / 2.0) * 56 - 6, 46 + j * 38 + 19
            g.box(BLACK, (x, 0, z), (50, 56, 38))
            for dx in (-15, 15):
                g.box(GOLD, (x + dx, 0, z), (6, 58, 40))
            g.box(GOLD, (x, -29, z + 2), (12, 5, 12))
    g.box(OX, (-6, 0, 164), (40, 30, 8))
    g.box(GOLD, (-6, -15, 164), (40, 5, 5))


def mounting_block(sc):
    """PROP, low (1.25 m): a mounting block shaped like a black anvil with a gold top, an iron step with a gold tread,
    a black hitching post with a gold ring and cap, an iron boot scraper."""
    g = sc.still()
    g.box(BLACK, (0, 0, 5), (110, 60, 70))
    g.box(BLACK, (0, 0, 55), (60, 44, 30))
    g.box(BLACK, (0, 0, 82), (150, 60, 24))
    g.side(BLACK, _ccw([(75, 70), (132, 94), (75, 94)]), -22, 22)
    g.box(GOLD, (0, 0, 97), (150, 60, 6))
    g.box(IRON, (-102, 0, 5), (52, 60, 70))
    g.box(GOLD, (-102, 0, 42), (52, 60, 5))
    g.box(BLACK, (170, 0, 45), (16, 16, 150))
    g.box(GOLD, (170, 0, 123), (22, 22, 8))
    for dx, dz, sx, sz in ((-9, 0, 5, 22), (9, 0, 5, 22), (0, 9, 22, 5), (0, -9, 22, 5)):
        g.box(GOLD, (170 + dx, -12, 96 + dz), (sx, 5, sz))
    for x in (-190, -150):
        g.box(IRON, (x, 0, 0), (6, 30, 60))
    g.box(IRON, (-170, 0, 26), (40, 6, 8))


def ledger_table(sc):
    """PROP, low (1.5 m): a clerk's iron table: four ledgers stacked with gold spines, a gold abacus of red and gold
    beads, an inkwell, a stool. Nobody at it."""
    g = sc.still()
    g.box(IRON, (0, 0, 78), (190, 70, 8))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(BLACK, (sx * 86, sy * 28, 22), (8, 8, 104))
    g.box(GOLD, (0, -31, 40), (172, 5, 5))
    for k, (col, w, d) in enumerate(((OX, 56, 44), (BLACK, 52, 40), (IRON, 56, 44), (OX, 48, 38))):
        z = 88 + k * 12
        g.box(col, (-55, 0, z), (w, d, 12))
        g.box(GOLD, (-55, -d / 2.0 - 1, z), (w, 5, 6))
    for x in (12, 82):
        g.box(BLACK, (x, 10, 114), (6, 10, 64))
    for z in (85, 146):
        g.box(BLACK, (47, 10, z), (76, 10, 6))
    for j in range(4):
        z = 98 + j * 12
        g.box(IRON, (47, 10, z), (64, 4, 4))
        for i in range(5):
            x = 20 + i * 9 + (18 if i >= 2 + j % 2 else 0)
            g.box(RED if (i + j) % 3 == 0 else GOLD, (x, 10, z), (8, 12, 9))
    g.box(BLACK, (-5, -18, 87), (12, 12, 10))
    g.box(GOLD, (-5, -18, 93), (8, 8, 4))
    g.box(IRON, (0, -62, 46), (34, 34, 6))
    g.box(IRON, (0, -62, 7), (10, 10, 74))


# ------------------------------------------------------------------------------------------ the hero landmark
def vault(sc):
    """HERO: THE VAULT. A black cube 110 m each way on an iron plinth; its whole front is a round gold vault door 69 m
    across in an iron collar ringed with twelve gold bolts standing proud, a silver spoked wheel and hub, a ring of
    dial marks, two hinges and two locking bars, and one steady red lamp over it. Rows of gold slits on both ends, a
    flat iron cornice, the cube's edges in steady gold. A cube and a circle: no spire, no mast, no mark. STILL."""
    r = random.Random(70)
    g = sc.still()
    S = 11000.0
    F = -S / 2.0
    ZC = 5300.0
    g.box(BLACK, (0, 0, S / 2.0 - 150), (S, S, S + 300))
    g.box(IRON, (0, 0, 150), (S + 500, S + 500, 900))
    g.box(GOLD, (0, 0, 630), (S + 520, S + 520, 60), glow=G1)
    g.box(GOLD, (0, 0, S - 40), (S + 420, S + 420, 80), glow=G1)
    g.box(IRON, (0, 0, S + 140), (S + 400, S + 400, 280))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(GOLD, (sx * S / 2.0, sy * S / 2.0, S / 2.0 + 300), (140, 140, S - 700), glow=G1)
    # the door, in layers standing out of the front
    rod(g, GOLD, (0, F, ZC), (0, F - 20, ZC), 4700, steps=32, glow=G1)
    rod(g, IRON, (0, F - 20, ZC), (0, F - 60, ZC), 4550, steps=32)
    rod(g, GOLD, (0, F - 60, ZC), (0, F - 200, ZC), 3450, steps=32, glow=G1)
    rod(g, BLACK, (0, F - 200, ZC), (0, F - 215, ZC), 2900, steps=32)
    rod(g, GOLD, (0, F - 215, ZC), (0, F - 260, ZC), 2750, steps=32, glow=G1)
    rod(g, SILVER, (0, F - 260, ZC), (0, F - 420, ZC), 620, steps=16)
    rod(g, GOLD, (0, F - 420, ZC), (0, F - 500, ZC), 300, steps=12, glow=G1)
    for k in range(12):
        a = math.radians(k * 30 + 15)
        x, z = 4000 * math.cos(a), ZC + 4000 * math.sin(a)
        rod(g, GOLD, (x, F - 60, z), (x, F - 240, z), 260, steps=8, glow=G1)
    for k in range(16):
        a0, a1 = math.radians(k * 22.5), math.radians((k + 1) * 22.5)
        tilt_box(g, SILVER, (1900 * math.cos(a0), F - 380, ZC + 1900 * math.sin(a0)), (1900 * math.cos(a1), F - 380, ZC + 1900 * math.sin(a1)), 110, 150)
    for k in range(4):
        g.box(SILVER, (0, F - 360, ZC), (3800, 90, 150), rot=(k * 45.0, 0, 0))
    for k in range(8):
        a = math.radians(k * 45)
        g.sphere(SILVER, (1900 * math.cos(a), F - 470, ZC + 1900 * math.sin(a)), 170, steps=(4, 6))
    for k in range(24):
        a = math.radians(k * 15 + 7.5)
        c, s = math.cos(a), math.sin(a)
        tilt_box(g, BLACK, (2350 * c, F - 262, ZC + 2350 * s), (2620 * c, F - 262, ZC + 2620 * s), 30, 60)
    for dz in (-1300, 1300):
        g.box(SILVER, (3600, F - 230, ZC + dz), (1700, 120, 320))
    for dz in (-1900, 1900):
        g.box(SILVER, (-3700, F - 230, ZC + dz), (1500, 140, 520))
    g.box(BLACK, (-4420, F - 240, ZC), (160, 160, 4600))
    # the one red lamp, over the door, steady
    g.box(BLACK, (0, F - 120, 10270), (700, 240, 300))
    g.box(GOLD, (0, F - 160, 10450), (800, 320, 60))
    g.sphere(RED, (0, F - 260, 10270), 230, steps=(6, 10), glow=G4)
    # the ends: four rows of slits, most of them lit
    ys = [-3900 + k * 1300 for k in range(7)]
    for s in (-1, 1):
        slits_x(g, r, s * (S / 2.0 + 2), ys, (2400, 4600, 6800, 9000), 160, 700, p=0.8)
    # a service door at the plinth's foot, for the size of the thing
    g.box(GOLD, (4500, F - 254, 300), (260, 8, 420))
    g.box(BLACK, (4500, F - 258, 300), (24, 12, 420))
    for x in (4320, 4680):
        g.box(RED, (x, F - 256, 560), (50, 10, 40), glow=G3)


JOBS = [('SM_S1D7_CountingTower', counting_tower, STILL), ('SM_S1D7_KeepTower', keep_tower, STILL),
        ('SM_S1D7_Strongroom', strongroom, STILL), ('SM_S1D7_Mint', mint, STILL), ('SM_S1D7_ClerksBastion', clerks_bastion, STILL),
        ('SM_S1D7_LedgerArchive', ledger_archive, STILL), ('SM_S1D7_TollHouse', toll_house, STILL),
        ('SM_S1D7_LanternCluster', lantern_cluster, STILL), ('SM_S1D7_PennantRow', pennant_row, STILL),
        ('SM_S1D7_SentryBox', sentry_box, STILL), ('SM_S1D7_RoofVents', roof_vents, STILL),
        ('SM_S1D7_BollardRow', bollard_row, STILL), ('SM_S1D7_StrongboxCart', strongbox_cart, STILL),
        ('SM_S1D7_MountingBlock', mounting_block, STILL), ('SM_S1D7_LedgerTable', ledger_table, STILL),
        ('SM_S1D7_Vault', vault, STILL)]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
