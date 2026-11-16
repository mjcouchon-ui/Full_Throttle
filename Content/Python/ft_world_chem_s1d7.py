"""Chemical Works, Stage 1, builder s1d7: district 7 "Tank Farm B", THE ROUND TOWN (17.4 to 21.4 km).
/Game/World/ChemicalWorks/SM_S1D7_*. Run: import ft_world_chem_s1d7 as K; K.run()

The tank men's village, made from converted and cut-down vessels: everything round. Cylinder houses with conical or
domed lids and a spiral stair wound round each, a sphere cabin on six legs, a lying-tank hall, an inn of two tanks and
a bridge, a gauge house with a dial, a water-tower house; one broad yellow band a building. The hero: THE GIANT KETTLE.
Palette: teal #19b5c9 (shells), deep teal #12849a (lids), hazard yellow #ffd23f (the band, rails), white #eef6ff
(rims, door rings), blue #33489e (legs, plinths); from the kit besides: lit window #ffe9a8, unlit pane #233a63,
steam #d8d0f0, steel #8f9bb3, navy #1c1f4a (a dial's pointer and ticks).

Every piece is an ft_people.Scene saved by ft_people.export: one material (M_People), one section, no collision.
cm; z = 0 is the land; FRONT faces -Y (the table turns -Y to the road); long pieces run along X.
Placed by C:/ftwork/s1/L1_s1d7.inl (written by C:/ftwork/jobs/L1s1d7/make_block.py).
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/ChemicalWorks'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))   # a piece with no moving part: true bounds

TEAL, TEALD, YEL, WHITE, BLUE = '#19b5c9', '#12849a', '#ffd23f', '#eef6ff', '#33489e'
WARM, PANE, STEAM, STEEL, NAVY = '#ffe9a8', '#233a63', '#d8d0f0', '#8f9bb3', '#1c1f4a'
LIT = 0.6            # a lit window's glow (steady; every piece that has one stands 28 m or more from the centre line)


# ------------------------------------------------------------------------------------------ helpers
def _rp(cx, cy, r, a):
    """A point r out from (cx, cy), a degrees round from the front: 0 = -Y (the road), 90 = +X."""
    t = math.radians(a)
    return (cx + r * math.sin(t), cy - r * math.cos(t))


def _p3(cx, cy, r, a, z):
    x, y = _rp(cx, cy, r, a)
    return (x, y, z)


def _shell(g, x, y, r, z0, z1, col=TEAL, steps=12, band=None, rim=True):
    """A standing tank: the shell from z0 (footing, below the land) to z1, a blue plinth, the yellow band, a white rim."""
    g.cyl(col, (x, y, z0), r, z1 - z0, steps=steps)
    g.cyl(BLUE, (x, y, z0), r + 16, 44 - z0, steps=steps)
    if band is not None:
        g.cyl(YEL, (x, y, band), r + 7, 72, steps=steps)
    if rim:
        g.cyl(WHITE, (x, y, z1), r + 14, 28, steps=steps)


def _cone(g, x, y, z, r, h, col=TEALD, steps=12, tip=30):
    g.cyl(col, (x, y, z), r, h, steps=steps, top_r=tip)


def _dome(g, x, y, z, r, col=TEALD, squash=0.5, steps=12):
    g.sphere(col, (x, y, z), r, scale=(1, 1, squash), steps=(4, steps))


def _port(g, cx, cy, r, a, z, pr=42, lit=False, steps=6):
    """A porthole on a tank's wall: a white ring and its pane."""
    rod(g, WHITE, _p3(cx, cy, r * 0.93, a, z), _p3(cx, cy, r + 8, a, z), pr, steps=steps)
    rod(g, WARM if lit else PANE, _p3(cx, cy, r * 0.93, a, z), _p3(cx, cy, r + 12, a, z), pr * 0.68, steps=steps, glow=LIT if lit else 0.0)


def _door(g, cx, cy, r, a, z=110, dr=86, col=BLUE, step=True):
    """A round door: a white ring, the leaf, a yellow hatch wheel, a step."""
    rod(g, WHITE, _p3(cx, cy, r * 0.93, a, z), _p3(cx, cy, r + 9, a, z), dr, steps=10)
    rod(g, col, _p3(cx, cy, r * 0.93, a, z), _p3(cx, cy, r + 15, a, z), dr * 0.8, steps=10)
    rod(g, YEL, _p3(cx, cy, r + 10, a, z), _p3(cx, cy, r + 23, a, z), dr * 0.24, steps=6)
    if step:
        x, y = _rp(cx, cy, r + 48, a)
        g.box(WHITE, (x, y, z - dr - 22), (dr * 1.7, 96, 30), rot=(0, -a, 0))


def _spiral(g, cx, cy, r, z0, z1, a0, a1, n, w=95):
    """A spiral stair wound round a tank from (a0, z0) to (a1, z1): n white treads, a yellow rail, a post every third tread."""
    prev = None
    da = (a1 - a0) / float(n)
    tread = max(abs(math.radians(da)) * (r + w * 0.5) * 0.92, 30.0)
    for k in range(n + 1):
        f = k / float(n)
        a, z = a0 + (a1 - a0) * f, z0 + (z1 - z0) * f
        t = math.radians(a)
        yaw = math.degrees(math.atan2(-math.cos(t), math.sin(t)))
        if k < n:
            x, y = _rp(cx, cy, r + w * 0.5 - 4, a)
            g.box(WHITE, (x, y, z), (w + 8, tread, 14), rot=(0, yaw, 0))
        top = _p3(cx, cy, r + w - 4, a, z + 104)
        if prev:
            tilt_box(g, YEL, prev, top, 10, 10)
        if k % 3 == 0:
            g.box(YEL, _p3(cx, cy, r + w - 4, a, z + 52), (8, 8, 104))
        prev = top
    x, y = _rp(cx, cy, r + w * 0.5 - 4, a1)                         # the landing
    t = math.radians(a1)
    g.box(WHITE, (x, y, z1), (w + 8, tread * 1.6, 14), rot=(0, math.degrees(math.atan2(-math.cos(t), math.sin(t))), 0))


def _puff(sc, x, y, z, r=44, rate=0.2, ph=0.0, rise=34):
    """Steam off a stove pipe or a spout: two soft balls that float up and down slowly."""
    p = sc.part(P.motion(bob=-rise, rate=rate, phase=ph))
    p.sphere(STEAM, (x, y, z), r, steps=(3, 6))
    p.sphere(STEAM, (x + r * 0.55, y + r * 0.2, z + r * 0.95), r * 0.68, steps=(3, 6))


def _stove(sc, g, x, y, z0, h, ph=0.0, r=20):
    """A stove pipe with a yellow cowl and its steam."""
    g.cyl(WHITE, (x, y, z0), r, h, steps=6)
    g.cyl(YEL, (x, y, z0 + h), r + 14, 26, steps=6, top_r=8)
    _puff(sc, x, y, z0 + h + 92, r=r * 1.9, ph=ph)


def _ring_rail(g, cx, cy, r, z, n=8, col=YEL, h=100, a0=0.0, skip=()):
    """A rail round a round deck: n straight lengths and a post at each corner."""
    for k in range(n):
        if k in skip:
            continue
        a, b = a0 + k * 360.0 / n, a0 + (k + 1) * 360.0 / n
        tilt_box(g, col, _p3(cx, cy, r, a, z + h), _p3(cx, cy, r, b, z + h), 9, 9)
        g.box(col, _p3(cx, cy, r, a, z + h * 0.5), (8, 8, h))


# ------------------------------------------------------------------------------------------ the buildings
def cyl_house(sc):
    """A BUILDING: the cylinder house. Three storeys in a 5.4 m tank, a conical lid, a spiral stair to the first floor,
    a round door, portholes (three lit), a stove pipe that steams, a weather vane that turns slowly. 7.3 m across, 14.5 m."""
    g = sc.still()
    R = 270
    _shell(g, 0, 0, R, -200, 1000, band=612)
    _cone(g, 0, 0, 1028, R + 26, 270)
    g.cyl(WHITE, (0, 0, 1298), 34, 30, steps=8)
    _door(g, 0, 0, R, 0)
    tilt_box(g, YEL, (0, -R + 4, 250), (0, -R - 118, 212), 230, 10)                 # the awning over the door
    for (a, z, lit) in ((-54, 215, True), (54, 215, False), (-28, 470, False), (36, 470, True), (0, 830, True), (-62, 830, False), (62, 830, False)):
        _port(g, 0, 0, R, a, z, lit=lit)
    _spiral(g, 0, 0, R, 34, 700, 64, 64 + 328, 13)
    _door(g, 0, 0, R, 32, z=700 + 92, dr=62, col=TEALD, step=False)
    _stove(sc, g, 120, 70, 1120, 250, ph=0.2)
    g.cyl(STEEL, (0, 0, 1328), 7, 120, steps=6)
    v = sc.part(P.motion(swing=24, pivot=(0, 0, 1440), axis=P.UP, rate=0.11, phase=0.4))
    v.box(YEL, (10, 0, 1440), (150, 5, 12))
    v.box(YEL, (-62, 0, 1440), (40, 5, 46))
    v.box(WHITE, (84, 0, 1440), (22, 5, 34))


def sphere_cabin(sc):
    """A BUILDING: the sphere cabin. A 4.5 m ball on six blue legs, a round deck with a rail, a ladder, a round door,
    two portholes (one lit), a hatch on top with a stove pipe that steams. 6 m across, 9.4 m."""
    g = sc.still()
    R, CZ = 225, 570
    g.sphere(TEAL, (0, 0, CZ), R, steps=(6, 12))
    g.cyl(YEL, (0, 0, CZ - 36), R + 4, 72, steps=12)
    for k in range(6):
        a = 30 + k * 60
        tilt_box(g, BLUE, _p3(0, 0, 262, a, -200), _p3(0, 0, 150, a, CZ - 150), 28, 28)
        g.box(WHITE, _p3(0, 0, 258, a, 10), (60, 60, 20))
    DZ = CZ - 122                                                                  # the deck
    g.cyl(WHITE, (0, 0, DZ), 298, 12, steps=12)
    _ring_rail(g, 0, 0, 290, DZ + 6, n=12, a0=-15.0, skip=(11,))                    # a gap at the ladder's head (front left)
    rod(g, WHITE, (0, -R * 0.82, CZ - 28), (0, -R - 5, CZ - 28), 76, steps=10)      # the round door
    rod(g, BLUE, (0, -R * 0.82, CZ - 28), (0, -R - 11, CZ - 28), 60, steps=10)
    rod(g, YEL, (0, -R - 8, CZ - 28), (0, -R - 19, CZ - 28), 15, steps=6)
    for (a, lit) in ((-62, True), (62, False), (180, False)):
        _port(g, 0, 0, R * 0.985, a, CZ + 14, pr=40, lit=lit)
    for s in (-1, 1):                                                               # the ladder
        tilt_box(g, YEL, (-118 + s * 24, -372, -60), (-118 + s * 24, -292, DZ + 100), 8, 8)
    for k in range(7):
        f = (k + 0.6) / 7.6
        g.box(YEL, (-118, -372 + 80 * f, -60 + (DZ + 160) * f), (48, 7, 7))
    g.cyl(WHITE, (0, 0, CZ + R - 14), 72, 28, steps=8)                              # the hatch on top
    g.cyl(YEL, (0, 0, CZ + R + 14), 84, 56, steps=8, top_r=12)
    _stove(sc, g, 96, 60, CZ + R - 70, 150, ph=0.6, r=16)


def gauge_house(sc):
    """A BUILDING: the gauge house. A slim 3.5 m cylinder under a dome with a 3.3 m dial on its front: the pointer
    swings slowly (one swing part, 14 seconds a cycle). A spiral stair to a hatch half way up. 5.3 m across, 13.6 m."""
    g = sc.still()
    R = 175
    _shell(g, 0, 0, R, -200, 1180, band=600)
    _dome(g, 0, 0, 1208, R + 8, squash=0.56)
    g.sphere(YEL, (0, 0, 1326), 26, steps=(3, 6))
    _door(g, 0, 0, R, 0, dr=78)
    DZ, FY = 905, -(R + 17)
    rod(g, YEL, (0, -R * 0.9, DZ), (0, -R - 10, DZ), 166, steps=12)                 # the dial
    rod(g, WHITE, (0, -R * 0.9, DZ), (0, FY, DZ), 146, steps=12)
    for k in range(8):
        t = math.radians(k * 45.0)
        g.box(NAVY, (math.sin(t) * 118, FY - 3, DZ + math.cos(t) * 118), (12, 6, 34), rot=(k * 45.0, 0, 0))
    rod(g, YEL, (0, FY, DZ), (0, FY - 12, DZ), 20, steps=6)
    n = sc.part(P.motion(swing=58, pivot=(0, FY - 6, DZ), axis=90.0, rate=0.07, phase=0.15))
    n.box(NAVY, (0, FY - 6, DZ + 50), (16, 6, 132))
    n.box(NAVY, (0, FY - 6, DZ - 22), (26, 6, 26))
    for (a, z, lit) in ((-58, 330, True), (58, 400, False), (-98, 905, False), (98, 905, True), (180, 700, False)):
        _port(g, 0, 0, R, a, z, pr=38, lit=lit)
    _spiral(g, 0, 0, R, 34, 640, 48, 48 + 424, 14, w=86)
    _door(g, 0, 0, R, 112, z=640 + 88, dr=56, col=TEALD, step=False)


def water_tower_house(sc):
    """A BUILDING: the water-tower house. A 5 m drum on a blue lattice stalk, a railed gallery round it, a curtained
    lit window, a conical lid with a vane that turns, a ladder up the stalk. 6 m across, 16.5 m."""
    g = sc.still()
    L, TZ, R = 150, 890, 250
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(BLUE, (sx * L, sy * L, (TZ - 200) / 2.0), (26, 26, TZ + 200))
    for z in (300, 600):
        for s in (-1, 1):
            g.box(BLUE, (0, s * L, z), (2 * L, 16, 16))
            g.box(BLUE, (s * L, 0, z), (16, 2 * L, 16))
    for s in (-1, 1):                                                               # braces on the front and the back
        tilt_box(g, BLUE, (-L, s * L, 20), (L, s * L, 300), 14, 14)
        tilt_box(g, BLUE, (L, s * L, 300), (-L, s * L, 600), 14, 14)
        tilt_box(g, BLUE, (-L, s * L, 600), (L, s * L, TZ - 10), 14, 14)
    g.cyl(TEALD, (0, 0, -200), 38, TZ + 200, steps=8)                               # the down pipe
    g.cyl(WHITE, (0, 0, TZ - 12), 296, 14, steps=12)                                # the gallery
    _ring_rail(g, 0, 0, 288, TZ, n=8, a0=22.5)
    g.cyl(TEAL, (0, 0, TZ), R, 440, steps=12)
    g.cyl(YEL, (0, 0, TZ + 250), R + 7, 72, steps=12)
    g.cyl(WHITE, (0, 0, TZ + 440), R + 14, 28, steps=12)
    _cone(g, 0, 0, TZ + 468, R + 26, 215)
    g.box(WARM, (0, -R - 2, TZ + 150), (104, 14, 124), glow=LIT)                    # the curtained window
    g.box(WHITE, (0, -R - 5, TZ + 150), (124, 10, 144))
    for s in (-1, 1):
        tilt_box(g, YEL, (s * 50, -R - 13, TZ + 212), (s * 26, -R - 13, TZ + 96), 7, 30)
    g.box(BLUE, (0, -R - 22, TZ + 70), (130, 34, 22))                               # its window box
    _port(g, 0, 0, R, 120, TZ + 170, pr=40)
    _port(g, 0, 0, R, -120, TZ + 170, pr=40, lit=True)
    _door(g, 0, 0, R, 180, z=TZ + 104, dr=70, step=False)
    for s in (-1, 1):                                                               # the ladder up the stalk
        g.box(YEL, (96 + s * 24, -L - 20, (TZ - 60) / 2.0), (8, 8, TZ + 60))
    for k in range(8):
        g.box(YEL, (96, -L - 20, 50 + k * 110), (48, 7, 7))
    g.cyl(STEEL, (0, 0, TZ + 683), 7, 90, steps=6)
    v = sc.part(P.motion(swing=30, pivot=(0, 0, TZ + 760), axis=P.UP, rate=0.09, phase=0.7))
    v.box(YEL, (8, 0, TZ + 760), (130, 5, 12))
    v.box(WHITE, (-56, 0, TZ + 760), (34, 5, 40))


def dome_house(sc):
    """A BUILDING: the tall one. Four storeys in a 5.7 m tank under a dome with a lit lantern; a spiral stair nearly
    twice round it up to a little balcony on the top floor; eight portholes (four lit). 7.5 m across, 18.7 m."""
    g = sc.still()
    R = 285
    _shell(g, 0, 0, R, -200, 1480, band=1085)
    _dome(g, 0, 0, 1508, R + 8, squash=0.62)
    g.cyl(WHITE, (0, 0, 1660), 70, 60, steps=8)                                     # the lantern
    g.cyl(WARM, (0, 0, 1720), 60, 54, steps=8, glow=LIT)
    g.cyl(YEL, (0, 0, 1774), 96, 92, steps=8, top_r=8)
    _door(g, 0, 0, R, 0)
    for (a, z, lit) in ((-52, 215, False), (52, 215, True), (-24, 585, True), (40, 585, False), (-58, 940, False), (26, 940, True),
                        (-64, 1310, True), (64, 1310, False)):
        _port(g, 0, 0, R, a, z, pr=40, lit=lit, steps=6)
    BZ = 1190                                                                       # the balcony
    g.box(WHITE, (0, -R - 34, BZ), (250, 110, 14))
    for s in (-1, 1):
        g.box(YEL, (s * 120, -R - 84, BZ + 52), (8, 8, 104))
    g.box(YEL, (0, -R - 84, BZ + 104), (248, 9, 9))
    _door(g, 0, 0, R, 0, z=BZ + 92, dr=64, col=TEALD, step=False)
    _spiral(g, 0, 0, R, 34, BZ, 70, 70 + 630, 18, w=90)


def drum_cottage(sc):
    """A BUILDING: the drum cottage. A squat 5.2 m drum under a low overhanging cone, a little drum annex, a fat
    chimney that steams, two lit portholes, a pipe bench by the door. 7.4 m long, 6.9 m."""
    g = sc.still()
    R, X = 260, -84
    _shell(g, X, 0, R, -200, 370, band=262)
    _cone(g, X, 0, 398, R + 44, 180, tip=70)
    g.cyl(WHITE, (X, 0, 578), 70, 16, steps=8)
    _door(g, X, 0, R, 0, dr=84)
    _port(g, X, 0, R, -52, 200, pr=46, lit=True)
    _port(g, X, 0, R, 50, 200, pr=46, lit=True)
    _port(g, X, 0, R, 180, 200, pr=40)
    tilt_box(g, YEL, (X, -R + 4, 246), (X, -R - 112, 214), 230, 10)                 # the awning
    g.cyl(WHITE, (X - 110, 80, 440), 46, 190, steps=8)                              # the chimney
    g.cyl(YEL, (X - 110, 80, 630), 62, 30, steps=8, top_r=30)
    _puff(sc, X - 110, 80, 760, r=58, ph=0.1, rate=0.16)
    AX, AR = 262, 132                                                               # the annex
    g.cyl(TEAL, (AX, 20, -200), AR, 470, steps=10)
    g.cyl(BLUE, (AX, 20, -200), AR + 14, 244, steps=10)
    g.cyl(WHITE, (AX, 20, 270), AR + 12, 22, steps=10)
    _dome(g, AX, 20, 292, AR + 4, squash=0.45, steps=10)
    _port(g, AX, 20, AR, 12, 170, pr=34)
    rod(g, WHITE, (X + 150, -R - 40, 44), (AX + 60, -AR - 30, 44), 22, steps=6)     # the pipe bench
    for bx in (X + 170, AX + 40):
        g.box(BLUE, (bx, -R + 60 if bx < 200 else -AR - 28, 4), (22, 40, 70))


def inn(sc):
    """A BUILDING: the tank men's inn. Two tanks (a domed one of 9 m and a coned one of 12.6 m) joined by a covered
    bridge, a big lit round window, a round door, a short spiral stair, a hanging sign (a blank yellow lid: no
    words) that sways, a chimney that steams. 11.6 m long, 15.4 m."""
    g = sc.still()
    AX, AR, BX, BR = -325, 255, 345, 225
    _shell(g, AX, 0, AR, -200, 900, band=560)
    _dome(g, AX, 0, 928, AR + 8, squash=0.5)
    _shell(g, BX, 0, BR, -200, 1260, band=560)
    _cone(g, BX, 0, 1288, BR + 24, 240)
    g.cyl(WHITE, (BX, 0, 1528), 30, 26, steps=6)
    rod(g, WHITE, (AX, -AR * 0.93, 300), (AX, -AR - 9, 300), 112, steps=12)         # the big lit window
    rod(g, WARM, (AX, -AR * 0.93, 300), (AX, -AR - 13, 300), 88, steps=12, glow=LIT)
    g.box(WHITE, (AX, -AR - 15, 300), (9, 6, 176))
    g.box(WHITE, (AX, -AR - 15, 300), (176, 6, 9))
    _door(g, BX, 0, BR, 0)
    tilt_box(g, YEL, (BX, -BR + 4, 250), (BX, -BR - 112, 214), 220, 10)
    for (cx, r, a, z, lit) in ((AX, AR, 0, 720, True), (AX, AR, 180, 420, False), (BX, BR, -8, 760, False), (BX, BR, 30, 1080, True), (BX, BR, 180, 700, False)):
        _port(g, cx, 0, r, a, z, pr=38, lit=lit, steps=6)
    BZ = 640                                                                        # the bridge
    g.box(WHITE, (10, 0, BZ), (300, 130, 16))
    rod(g, TEALD, (-140, 0, BZ + 150), (160, 0, BZ + 150), 74, steps=8)
    for s in (-1, 1):
        g.box(YEL, (10, s * 62, BZ + 104), (290, 9, 9))
        for x in (-70, 10, 90):
            g.box(YEL, (x, s * 62, BZ + 52), (8, 8, 104))
    _spiral(g, BX, 0, BR, 34, 420, 70, 70 + 170, 6, w=86)
    _stove(sc, g, AX + 90, 90, 1000, 190, ph=0.5)
    g.box(BLUE, (0, -150, BZ - 30), (14, 240, 14))                                  # the sign's bracket, towards the road
    s2 = sc.part(P.motion(swing=7, pivot=(0, -250, BZ - 36), axis=0.0, rate=0.3, phase=0.2))
    s2.box(STEEL, (0, -250, BZ - 62), (6, 6, 52))
    rod(s2, YEL, (-6, -250, BZ - 150), (6, -250, BZ - 150), 64, steps=10)
    rod(s2, WHITE, (-9, -250, BZ - 150), (9, -250, BZ - 150), 34, steps=8)


def tank_hall(sc):
    """A BUILDING: the lying-tank hall. A 6.6 m bullet tank on blue saddles, 13 m long with its dished ends: a round
    door in one end up two steps, a big round window in the other, a row of five portholes (three lit), two roof
    vents, a railed walkway along the top, a stove pipe that steams. 6.6 m deep, 8.3 m."""
    g = sc.still()
    R, CZ, H = 330, 345, 520
    g.cyl(TEAL, (-H, 0, CZ), R, 2 * H, rot=(-90, 0, 0), steps=12)
    for s in (-1, 1):
        g.sphere(TEAL, (s * H, 0, CZ), R, scale=(0.36, 1, 1), steps=(5, 12))
        g.cyl(WHITE, (s * (H - 40) - 15, 0, CZ), R + 6, 30, rot=(-90, 0, 0), steps=12)
    g.cyl(YEL, (-36, 0, CZ), R + 7, 72, rot=(-90, 0, 0), steps=12)
    for x in (-350, 0, 350):
        g.box(BLUE, (x, 0, (CZ - 200 - 90) / 2.0), (70, 560, CZ + 200 - 90))
    for k, x in enumerate((-400, -240, 150, 290, 430)):
        rod(g, WHITE, (x, -R * 0.93, CZ + 20), (x, -R - 8, CZ + 20), 44, steps=8)
        lit = k in (0, 2, 3)
        rod(g, WARM if lit else PANE, (x, -R * 0.93, CZ + 20), (x, -R - 12, CZ + 20), 30, steps=8, glow=LIT if lit else 0.0)
    EX = H + R * 0.36                                                               # the round door in the -X end
    rod(g, WHITE, (-EX + 40, 0, CZ - 40), (-EX - 8, 0, CZ - 40), 128, steps=10)
    rod(g, BLUE, (-EX + 40, 0, CZ - 40), (-EX - 14, 0, CZ - 40), 102, steps=10)
    rod(g, YEL, (-EX - 10, 0, CZ - 40), (-EX - 24, 0, CZ - 40), 26, steps=6)
    for k in range(2):
        g.box(WHITE, (-EX - 40 - k * 46, 0, (150 - k * 70 - 200) / 2.0), (50, 190, 150 - k * 70 + 200))
    rod(g, WHITE, (EX - 40, 0, CZ + 10), (EX + 8, 0, CZ + 10), 150, steps=10)       # the round window in the +X end
    rod(g, WARM, (EX - 40, 0, CZ + 10), (EX + 12, 0, CZ + 10), 120, steps=10, glow=LIT)
    g.box(WHITE, (EX + 14, 0, CZ + 10), (6, 9, 238))
    g.box(WHITE, (EX + 14, 0, CZ + 10), (6, 238, 9))
    TOP = CZ + R
    g.box(WHITE, (0, 0, TOP + 2), (700, 96, 14))                                    # the walkway along the top
    for s in (-1, 1):
        g.box(YEL, (0, s * 46, TOP + 100), (700, 9, 9))
        for x in (-346, -115, 115, 346):
            g.box(YEL, (x, s * 46, TOP + 52), (8, 8, 100))
    for x in (-270, 220):                                                           # the roof vents
        g.cyl(WHITE, (x, 0, TOP - 20), 46, 70, steps=8)
        g.cyl(TEALD, (x, 0, TOP + 50), 70, 60, steps=8, top_r=10)
    _stove(sc, g, 400, 110, TOP - 60, 170, ph=0.35)


# ------------------------------------------------------------------------------------------ the props
def dipsticks(sc):
    """A PROP, TALL (6.9 m): a rack of five giant dipsticks with yellow and teal level marks (one rises and falls in
    its tube, slowly: somebody is reading it) and a gauge board with two blank dials. 5.8 m long, 0.7 m deep."""
    g = sc.still()
    g.box(BLUE, (-40, 0, -14), (500, 70, 52))
    for (x, h) in ((-240, 430), (-150, 610), (-60, 330), (30, 520)):
        g.cyl(BLUE, (x, 0, 12), 16, 124, steps=6)
        g.cyl(WHITE, (x, 0, 136), 9, h - 136, steps=6)
        g.box(WHITE, (x, 0, h + 12), (62, 12, 24))
        for k in range(3):
            g.cyl(YEL if k % 2 == 0 else TEAL, (x, 0, 190 + k * (h - 250) / 3.0), 13, 28, steps=6)
    g.cyl(BLUE, (120, 0, 12), 16, 124, steps=6)
    m = sc.part(P.motion(bob=-48, rate=0.11, phase=0.3))
    m.cyl(WHITE, (120, 0, 150), 9, 500, steps=6)
    m.box(YEL, (120, 0, 662), (62, 12, 24))
    m.cyl(TEAL, (120, 0, 300), 13, 28, steps=6)
    m.cyl(YEL, (120, 0, 450), 13, 28, steps=6)
    for x in (196, 290):                                                            # the gauge board
        g.box(BLUE, (x, 0, 100), (12, 12, 280))
    g.box(WHITE, (243, -4, 205), (136, 10, 120))
    for x in (212, 274):
        rod(g, YEL, (x, -9, 210), (x, -15, 210), 26, steps=8)
        g.box(NAVY, (x + 6, -17, 216), (7, 5, 30), rot=(30, 0, 0))


def lid_table(sc):
    """A PROP, LOW (1 m): two manhole-lid tables on pipe legs with drum stools between and beside them, mugs on the
    lids. Nothing moves, nothing is lit. 5.8 m long, 1.3 m deep."""
    g = sc.still()
    for tx in (-150, 150):
        g.cyl(BLUE, (tx, 0, -40), 14, 118, steps=6)
        g.cyl(YEL, (tx, 0, 73), 66, 6, steps=10)
        g.cyl(STEEL, (tx, 0, 79), 62, 9, steps=10)
        g.cyl(WHITE, (tx + 22, -16, 88), 7, 12, steps=6)
        g.cyl(TEALD, (tx - 24, 12, 88), 7, 12, steps=6)
    for (x, col) in ((-268, TEAL), (0, BLUE), (268, TEALD)):
        g.cyl(col, (x, 0, -40), 24, 86, steps=8)
        g.cyl(YEL, (x, 0, 20), 26, 8, steps=8)
        g.cyl(WHITE, (x, 0, 46), 26, 5, steps=8)


def elbow_stack(sc):
    """A PROP, LOW (1.45 m): a stack of three pipe lengths on sleepers, two pipe elbows standing by it and three loose
    flanges on edge, teal and white. Still and unlit. 7.9 m long, 1.2 m deep."""
    g = sc.still()
    for x in (-190, 60):
        g.box(BLUE, (x, 0, -12), (26, 120, 56))
    for (y, z, col) in ((-36, 50, TEAL), (36, 50, WHITE), (0, 110, TEAL)):
        rod(g, col, (-260, y, z), (130, y, z), 34, steps=8)
        rod(g, WHITE if col == TEAL else TEAL, (-262, y, z), (-248, y, z), 41, steps=8)
    rod(g, TEAL, (215, 0, -40), (215, 0, 96), 36, steps=8)                          # an elbow, standing
    g.sphere(TEAL, (215, 0, 96), 38, steps=(4, 8))
    rod(g, TEAL, (215, 0, 96), (305, 0, 96), 36, steps=8)
    rod(g, WHITE, (300, 0, 96), (313, 0, 96), 46, steps=8)
    rod(g, WHITE, (372, 6, -40), (372, 6, 62), 30, steps=8)                         # a smaller one, white
    g.sphere(WHITE, (372, 6, 62), 32, steps=(4, 8))
    rod(g, WHITE, (372, 6, 62), (372, -44, 62), 30, steps=8)
    rod(g, TEAL, (372, -42, 62), (372, -54, 62), 39, steps=8)
    for (x, col) in ((-340, TEAL), (-322, WHITE), (-304, TEAL)):                    # loose flanges on edge
        rod(g, col, (x, 0, 8), (x + 11, 0, 8), 48, steps=10)


def wash_line(sc):
    """A PROP, LOW (1.85 m): a washing line of overalls and a sheet between two posts, a basket at its foot: five
    garments, each swaying on the line at its own time. 6.2 m long."""
    g = sc.still()
    LZ = 172
    for x in (-300, 300):
        g.cyl(WHITE, (x, 0, -40), 7, 225, steps=6)
        g.cyl(BLUE, (x, 0, -40), 16, 56, steps=6)
        g.box(WHITE, (x, 0, 178), (8, 60, 7))
    g.box(STEEL, (0, 0, LZ), (600, 5, 5))
    g.box(WHITE, (262, -6, -8), (56, 40, 44))                                       # the basket
    g.box(YEL, (262, -6, 16), (60, 44, 6))
    for k, (x, col, kind) in enumerate(((-222, TEAL, 0), (-112, WHITE, 1), (0, BLUE, 0), (108, YEL, 2), (200, TEALD, 1))):
        m = sc.part(P.motion(swing=11, pivot=(x, 0, LZ), axis=0.0, rate=0.28 + 0.03 * (k % 3), phase=0.19 * k))
        if kind == 0:                                                               # overalls
            m.box(col, (x, 0, LZ - 34), (46, 6, 62))
            for s in (-1, 1):
                m.box(col, (x + s * 13, 0, LZ - 90), (19, 6, 54))
                m.box(col, (x + s * 14, 0, LZ + 4), (7, 6, 16))
        elif kind == 1:                                                             # a shirt
            m.box(col, (x, 0, LZ - 30), (44, 6, 58))
            for s in (-1, 1):
                m.box(col, (x + s * 36, 0, LZ - 12), (30, 6, 20))
        else:                                                                       # a sheet
            m.box(col, (x, 0, LZ - 44), (88, 5, 84))


def valve_wheel(sc):
    """A PROP, LOW (1.85 m): a ground manifold with two hand valve wheels 1.6 m across, facing the road. White
    wheels on blue pedestals; still and unlit (nothing turns, nothing bobs). 4.9 m long, 0.9 m deep."""
    g = sc.still()
    rod(g, TEAL, (-232, 10, 46), (232, 10, 46), 30, steps=8)
    for x in (-236, 224):
        rod(g, WHITE, (x, 10, 46), (x + 12, 10, 46), 40, steps=8)
    for x in (-160, 0, 160):
        g.box(BLUE, (x, 10, -8), (38, 76, 64))
    for x in (-90, 90):
        g.box(BLUE, (x, 10, 80), (54, 54, 72))                                      # the valve's body
        rod(g, STEEL, (x, -40, 104), (x, 12, 104), 9, steps=6)                      # the stem
        rod(g, WHITE, (x, -44, 104), (x, -28, 104), 17, steps=8)                    # the hub
        g.box(WHITE, (x, -36, 104), (150, 10, 10))
        g.box(WHITE, (x, -36, 104), (10, 10, 150))
        for k in range(8):
            a, b = math.radians(k * 45.0), math.radians(k * 45.0 + 45.0)
            tilt_box(g, WHITE, (x + math.cos(a) * 76, -36, 104 + math.sin(a) * 76), (x + math.cos(b) * 76, -36, 104 + math.sin(b) * 76), 12, 12)


def rain_butt(sc):
    """A PROP, LOW (1.85 m): two round rain-water butts with yellow bands, a stand pipe with a spout over the big one,
    a bucket, a tin bath and a watering can. Still and unlit. 5 m long, 1.2 m deep."""
    g = sc.still()
    g.cyl(TEAL, (-120, 0, -40), 55, 170, steps=10)
    g.cyl(YEL, (-120, 0, 52), 58, 18, steps=10)
    g.cyl(WHITE, (-120, 0, 126), 59, 8, steps=10)
    g.cyl(WHITE, (-196, 14, -40), 9, 225, steps=6)                                  # the stand pipe and its spout
    rod(g, WHITE, (-196, 14, 180), (-140, 6, 166), 9, steps=6)
    g.sphere(WHITE, (-196, 14, 182), 12, steps=(3, 6))
    g.cyl(WHITE, (-22, -8, -40), 17, 72, steps=8, top_r=21)                         # the bucket
    g.cyl(BLUE, (82, 0, -40), 46, 148, steps=10)
    g.cyl(YEL, (82, 0, 40), 49, 16, steps=10)
    g.cyl(WHITE, (82, 0, 104), 50, 8, steps=10)
    g.box(STEEL, (212, 0, -6), (128, 58, 68))                                       # the tin bath
    g.box(WHITE, (212, 0, 30), (136, 66, 7))
    g.cyl(TEALD, (-250, -4, -40), 17, 76, steps=8)                                  # the watering can
    tilt_box(g, TEALD, (-262, -4, 16), (-296, -4, 40), 7, 7)


def pipe_lamp(sc):
    """A PROP, TALL (5 m): a street lamp made of pipe: a flanged post with a yellow band, an elbow and an arm towards
    the road, a white shade over a steady warm bulb; a blank yellow house roundel on a bracket (no words) and a pipe
    bench at its foot. Nothing moves, nothing blinks. 2.6 m long, 1.6 m deep."""
    g = sc.still()
    g.cyl(BLUE, (0, 40, -40), 30, 64, steps=8)
    g.cyl(TEAL, (0, 40, 24), 11, 468, steps=8)
    g.cyl(YEL, (0, 40, 150), 14, 50, steps=8)
    g.sphere(TEAL, (0, 40, 492), 15, steps=(4, 8))
    rod(g, TEAL, (0, 40, 492), (0, -60, 492), 10, steps=6)
    g.cyl(STEEL, (0, -60, 462), 5, 30, steps=6)
    g.cyl(WHITE, (0, -60, 428), 46, 36, steps=8, top_r=12)
    g.sphere(WARM, (0, -60, 426), 19, steps=(4, 8), glow=0.9)
    g.box(BLUE, (40, 40, 330), (70, 8, 8))                                          # the roundel
    rod(g, YEL, (104, 34, 330), (104, 46, 330), 34, steps=10)
    rod(g, WHITE, (104, 30, 330), (104, 50, 330), 16, steps=8)
    rod(g, WHITE, (-110, 30, 42), (60, 30, 42), 21, steps=6)                        # the pipe bench
    for x in (-84, 34):
        g.box(BLUE, (x, 30, -4), (20, 38, 72))


# ------------------------------------------------------------------------------------------ the hero landmark
def giant_kettle(sc):
    """THE HERO: THE GIANT KETTLE. A tank built as a kettle, 52 m tall: a fat teal body 35 m across with a yellow band,
    a deep teal domed lid with a yellow ball knob, a hoop handle over the top with a white grip, a curved spout towards
    the road and the oncoming cars with a slow plume of steam (five puffs, each floating at its own time), a ring of
    white stub legs, a stair winding up it to a gallery of lit portholes, a blank white gauge on its belly."""
    g = sc.still()
    N = 20
    for k in range(8):                                                              # the ring of stub legs
        x, y = _rp(0, 0, 1260, 22.5 + k * 45)
        g.cyl(WHITE, (x, y, -600), 170, 900, steps=8)
    g.cyl(BLUE, (0, 0, 180), 1470, 130, steps=N)
    g.cyl(TEALD, (0, 0, 300), 1260, 620, steps=N, top_r=1660)                        # the body, foot to shoulder
    g.cyl(TEAL, (0, 0, 920), 1660, 1380, steps=N, top_r=1730)
    g.cyl(YEL, (0, 0, 2300), 1760, 300, steps=N)
    g.cyl(TEAL, (0, 0, 2600), 1730, 900, steps=N, top_r=1260)
    g.cyl(WHITE, (0, 0, 3500), 1290, 190, steps=N, top_r=1180)
    g.sphere(TEALD, (0, 0, 3690), 1170, scale=(1, 1, 0.55), steps=(5, N))            # the lid
    g.cyl(WHITE, (0, 0, 4300), 120, 200, steps=8)
    g.sphere(YEL, (0, 0, 4650), 260, steps=(5, 10))
    SA = -52.0                                                                      # the spout's quarter: the road and the oncoming cars
    ux, uy = _rp(0, 0, 1.0, SA)
    pts = [(1450, 1750, 330), (2150, 2100, 300), (2560, 2760, 262), (2780, 3440, 232), (3020, 3900, 215)]
    for k in range(len(pts) - 1):
        (r0, z0, w0), (r1, z1, w1) = pts[k], pts[k + 1]
        rod(g, TEAL, (ux * r0, uy * r0, z0), (ux * r1, uy * r1, z1), w0, steps=10)
        g.sphere(TEAL, (ux * r1, uy * r1, z1), w0 * 1.02, steps=(4, 10))
    rt, zt = pts[-1][0], pts[-1][1]
    rod(g, WHITE, (ux * (rt - 40), uy * (rt - 40), zt - 70), (ux * (rt + 50), uy * (rt + 50), zt + 90), 262, steps=10)   # the spout's lip
    for k, (dr, dz, r, ph, rate) in enumerate(((120, 330, 250, 0.0, 0.1), (300, 640, 330, 0.3, 0.085), (160, 1010, 400, 0.55, 0.075),
                                               (420, 1380, 450, 0.8, 0.065), (230, 1800, 380, 0.15, 0.06))):
        p = sc.part(P.motion(bob=-(150 + 40 * k), rate=rate, phase=ph))
        p.sphere(STEAM, (ux * (rt + dr), uy * (rt + dr), zt + dz), r, steps=(4, 8))
    hx, hy = ux, uy                                                                 # the hoop handle, in the spout's plane
    M = 10
    prev = None
    for k in range(M + 1):
        t = math.pi * k / float(M)
        p = (hx * math.cos(t) * 1330, hy * math.cos(t) * 1330, 3420 + math.sin(t) * 1650)
        if prev:
            grip = 3 <= k <= 7
            rod(g, WHITE if grip else YEL, prev, p, 150 if grip else 112, steps=8)
            if k < M:
                g.sphere(WHITE if (grip and k < 7) else YEL, p, 152 if (grip and k < 7) else 116, steps=(3, 8))
        prev = p
    prev = None                                                                     # the stair: a white ramp and a yellow rail
    S = 16
    for k in range(S + 1):
        f = k / float(S)
        a = 60 + 300 * f
        z = 320 + (2330 - 320) * f
        rr = (1260 + (1660 - 1260) * min(1.0, (z - 300) / 620.0)) if z < 920 else (1660 + 70 * (z - 920) / 1380.0)
        p = _p3(0, 0, rr + 90, a, z)
        q = _p3(0, 0, rr + 170, a, z + 150)
        if prev:
            tilt_box(g, WHITE, prev[0], p, 210, 26)
            tilt_box(g, YEL, prev[1], q, 22, 22)
        prev = (p, q)
    g.cyl(WHITE, (0, 0, 2270), 1840, 30, steps=N)                                    # the gallery under the band
    for k in range(N // 2):
        a, b = k * 720.0 / N, (k + 1) * 720.0 / N
        tilt_box(g, YEL, _p3(0, 0, 1825, a, 2440), _p3(0, 0, 1825, b, 2440), 22, 22)
    for k in range(7):                                                              # lit portholes above the band
        a = SA + 38 + k * 46
        rod(g, WHITE, _p3(0, 0, 1560, a, 2880), _p3(0, 0, 1668, a, 2880), 120, steps=8)
        rod(g, WARM, _p3(0, 0, 1560, a, 2880), _p3(0, 0, 1680, a, 2880), 84, steps=8, glow=0.5)
    GA = 24.0                                                                       # the gauge on the belly: a blank white face, a still pointer
    rod(g, YEL, _p3(0, 0, 1560, GA, 1560), _p3(0, 0, 1722, GA, 1560), 330, steps=12)
    rod(g, WHITE, _p3(0, 0, 1560, GA, 1560), _p3(0, 0, 1738, GA, 1560), 280, steps=12)
    tilt_box(g, NAVY, _p3(0, 0, 1746, GA, 1540), _p3(0, 0, 1746, GA + 4.5, 1770), 34, 22)
    rod(g, WHITE, _p3(0, 0, 1180, 8, 520), _p3(0, 0, 1400, 8, 520), 150, steps=10)   # the door at its foot
    rod(g, BLUE, _p3(0, 0, 1180, 8, 520), (_p3(0, 0, 1412, 8, 520)), 118, steps=10)


JOBS = [
    ('SM_S1D7_CylHouse', cyl_house),
    ('SM_S1D7_SphereCabin', sphere_cabin),
    ('SM_S1D7_GaugeHouse', gauge_house),
    ('SM_S1D7_WaterTowerHouse', water_tower_house),
    ('SM_S1D7_DomeHouse', dome_house),
    ('SM_S1D7_DrumCottage', drum_cottage),
    ('SM_S1D7_Inn', inn),
    ('SM_S1D7_TankHall', tank_hall),
    ('SM_S1D7_Dipsticks', dipsticks),
    ('SM_S1D7_LidTable', lid_table, STILL),
    ('SM_S1D7_ElbowStack', elbow_stack, STILL),
    ('SM_S1D7_WashLine', wash_line, dict(grow=(0, 34, 4), grow_down=(0, 34, 0))),
    ('SM_S1D7_ValveWheel', valve_wheel, STILL),
    ('SM_S1D7_RainButt', rain_butt, STILL),
    ('SM_S1D7_PipeLamp', pipe_lamp, STILL),
    ('SM_S1D7_GiantKettle', giant_kettle, dict(far=True, grow=(0, 0, 320), grow_down=(0, 0, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilds them
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
