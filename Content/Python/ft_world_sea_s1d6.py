"""Under the Sea, Stage 1, builder s1d6: district 6 "Nereida: the Bubble Works" (12.2 to 16.1 km).
/Game/World/UnderTheSea/SM_S1D6_*. The glassblowers' quarter: houses of glass bubbles stacked or bunched like grapes on
copper stems, looping pipes, chimneys that let go strings of bubbles. Copper green, pale glass, magenta, sun yellow, ink.
The hero is THE BUBBLE TREE: a 60 m copper trunk with a dozen glass-sphere houses as its fruit and bubbles rising from its top.
Placed by the block C:/ftwork/s1/L6_s1d6.inl. Run: import ft_world_sea_s1d6 as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/UnderTheSea'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))        # a piece with no moving part: true bounds
SMALL = dict(grow=(25, 25, 25), grow_down=(25, 25, 0))   # a piece whose moving parts stay close to it
LOWMOVE = dict(grow=(10, 10, 12), grow_down=(10, 10, 0))

COP, GLASS, MAG, YEL, INK = '#3fae7a', '#cfeeff', '#ff4fb8', '#ffd23f', '#3a3140'   # the district's five
COP_D, PINK, CREAM, SAND, WOOD = '#1f5a3a', '#ff9ec7', '#f4efe0', '#fff1c0', '#6a4c36'
GW, GL = 0.35, 0.55                                      # glow: lit windows, lamps (steady; only on pieces 24 m out or more)
FOOT = 200.0


# ------------------------------------------------------------------------------------------ helpers
def vec(az, el=0.0):
    a, e = math.radians(az), math.radians(el)
    return (math.cos(e) * math.cos(a), math.cos(e) * math.sin(a), math.sin(e))


def at(c, d, k):
    return (c[0] + d[0] * k, c[1] + d[1] * k, c[2] + d[2] * k)


def bubble(g, c, r, steps=(6, 10), lights=(), door=None, cap=1, collar=True, glass=GLASS, glow=GW):
    """A glass-sphere room: the sphere, a copper collar round its middle, a copper cap, round lit windows
    (az degrees in the ground plane: 270 = the front, towards the road; el degrees up; colour; size as a part of r)."""
    x, y, z = c
    g.sphere(glass, c, r, steps=steps)
    if collar:
        g.cyl(COP, (x, y, z - r * 0.07), r * 1.035, r * 0.14, steps=steps[1])
    if cap:
        g.cyl(COP, (x, y, z + r * 0.78), r * 0.64, r * 0.34, steps=8, top_r=r * 0.16)
        if cap > 1:
            g.sphere(YEL, (x, y, z + r * 1.2), r * 0.14, steps=(3, 6))
    for az, el, col, f in lights:
        d = vec(az, el)
        rod(g, col, at(c, d, r * 0.88), at(c, d, r + 7.0), r * f, steps=6, glow=glow)
    if door:
        d = vec(door[0], door[1])
        rod(g, YEL, at(c, d, r * 0.85), at(c, d, r + 4.0), r * 0.31, steps=8)
        rod(g, INK, at(c, d, r * 0.85), at(c, d, r + 9.0), r * 0.25, steps=8)


def pipe(g, col, pts, r, steps=6, joints=True):
    for a, b in zip(pts, pts[1:]):
        rod(g, col, a, b, r, steps=steps)
    if joints:
        for p in pts[1:-1]:
            g.sphere(col, p, r * 1.12, steps=(3, 6))


def ring_pts(c, r, n, plane='xz', a0=0.0, a1=360.0):
    out = []
    for k in range(n + 1):
        a = math.radians(a0 + (a1 - a0) * k / float(n))
        if plane == 'xz':
            out.append((c[0] + r * math.cos(a), c[1], c[2] + r * math.sin(a)))
        else:
            out.append((c[0], c[1] + r * math.cos(a), c[2] + r * math.sin(a)))
    return out


def fizz(sc, x, y, z, n=5, r0=16.0, dz=70.0, amp=35.0, rate=0.3, seed=0, col=GLASS, spread=0.2):
    """A string of bubbles over a chimney's mouth: each floats up and down on its own beat and sways a little."""
    rr = random.Random(seed)
    for k in range(n):
        m = sc.part(P.motion(bob=-amp * (0.6 + 0.12 * k), rate=round(rate * rr.uniform(0.8, 1.25), 3), phase=round(k * 0.19 + rr.random() * 0.1, 3),
                             swing=2.5, pivot=(x, y, z), axis=round(rr.uniform(0.0, 180.0), 1)))
        m.sphere(col, (x + rr.uniform(-1, 1) * dz * spread * (k + 1) ** 0.5, y + rr.uniform(-1, 1) * dz * spread * (k + 1) ** 0.5, z + dz * (k + 1)),
                 r0 * (1.0 + 0.32 * k), steps=(4, 6))


def chimney(sc, g, x, y, z0, h, r=45.0, n=5, seed=0, amp=35.0, r0=18.0, dz=80.0):
    g.cyl(COP, (x, y, z0), r, h, steps=8)
    g.cyl(INK, (x, y, z0 + h * 0.55), r * 1.2, 26.0, steps=8)
    g.cyl(COP_D, (x, y, z0 + h), r, 40.0, steps=8, top_r=r * 1.45)
    fizz(sc, x, y, z0 + h + 40.0, n=n, r0=r0, dz=dz, amp=amp, seed=seed)


def planter(g, x, y, n=3, seed=0):
    rr = random.Random(seed)
    g.box(INK, (x, y, 22), (50 + 45 * n, 60, 44))
    for k in range(n):
        px = x - 22 * (n - 1) + 45 * k
        g.cyl(COP_D, (px, y, 40), 5, 40 + 12 * (k % 2), steps=5)
        g.sphere(rr.choice((PINK, MAG, YEL)), (px, y, 96 + 12 * (k % 2)), 17, steps=(4, 6))


# ------------------------------------------------------------------------------------------ buildings
def three_bubble(sc):
    """A three-bubble house: the big room on a copper stem, a bedroom on top, a small one beside on its own stem."""
    g = sc.still()
    g.cyl(INK, (0, 0, -FOOT), 380, FOOT + 40, steps=10)
    g.cyl(COP, (0, 0, 40), 80, 130, steps=8, top_r=120)
    c1 = (0, 0, 470)
    bubble(g, c1, 330, lights=((238, 26, YEL, 0.2), (302, 26, MAG, 0.2), (180, 18, YEL, 0.22), (0, 18, MAG, 0.22)), door=(270, -36), cap=0)
    c2 = (60, 40, 960)
    g.cyl(COP, (40, 25, 760), 120, 60, steps=8)
    bubble(g, c2, 220, lights=((250, 20, MAG, 0.24), (300, 20, YEL, 0.2), (180, 10, MAG, 0.24), (0, 10, YEL, 0.24)), cap=1)
    c3 = (-440, 30, 330)
    g.cyl(COP, (-440, 30, -100), 42, 260, steps=6)
    bubble(g, c3, 190, steps=(5, 8), lights=((270, 15, YEL, 0.3), (180, 15, MAG, 0.26)), cap=2)
    pipe(g, COP, [(-440, 30, 500), (-440, 30, 650), (-340, 20, 740), (-235, 10, 700)], 22)
    # the stair to the door, its rail, an awning over it
    tilt_box(g, INK, (0, -560, 0), (0, -280, 262), 130, 14)
    for sx in (-60, 60):
        tilt_box(g, COP_D, (sx, -560, 70), (sx, -290, 330), 6, 6)
    tilt_box(g, MAG, (0, -262, 470), (0, -400, 420), 250, 12)
    tilt_box(g, YEL, (0, -400, 420), (0, -420, 396), 250, 10)
    # a pipe that loops out of the wall and back, a valve wheel on it
    pipe(g, COP, ring_pts((390, -40, 300), 120, 8, 'xz', -150, 150), 20, joints=False)
    rod(g, MAG, (505, -40, 300), (505, -66, 300), 34, steps=8)
    # the chimney on the bedroom, bent like a blower's pipe
    pipe(g, COP, [(110, 40, 1150), (110, 40, 1290), (190, 40, 1370), (190, 40, 1450)], 26)
    g.cyl(COP_D, (190, 40, 1450), 26, 36, steps=8, top_r=40)
    fizz(sc, 190, 40, 1486, n=5, r0=16, dz=70, amp=32, seed=11)
    planter(g, 250, -420, 3, 1)
    planter(g, -230, -430, 2, 2)


def grape_tenement(sc):
    """Six bubbles bunched like grapes on one copper stem, a tendril at the top, a washing line between two of them."""
    g = sc.still()
    g.cyl(INK, (0, 0, -FOOT), 230, FOOT + 50, steps=10)
    g.cyl(COP, (0, 40, 50), 62, 1750, steps=8, top_r=40)
    pipe(g, COP, [(0, 40, 1800), (40, 40, 1920), (150, 40, 1985), (255, 40, 1930), (265, 40, 1840), (205, 40, 1800)], 24)
    g.box(COP_D, (120, 40, 2030), (150, 10, 60), rot=(20, 0, 0))                      # the vine's leaf
    grapes = [((0, -250, 1420), 240, 270), ((-262, 190, 1400), 235, 150), ((262, 190, 1400), 235, 30),
              ((-250, -40, 950), 250, 200), ((250, -60, 940), 250, 340), ((0, -70, 490), 270, 270)]
    for k, (c, r, az) in enumerate(grapes):
        side = 180 if c[0] < -10 else 0
        lit = [(az if c[1] < 0 else (320 if side == 0 else 220), 22, YEL if k % 2 else MAG, 0.2)]
        lit.append((side if c[0] else 180, 12, MAG if k % 2 else YEL, 0.22))
        if c[0] == 0:
            lit.append((0, 12, YEL if k % 2 else MAG, 0.22))
        if c[1] < 0:
            lit.append((270 + (25 if c[0] <= 0 else -25), -20, YEL, 0.17))
        bubble(g, c, r, lights=lit, cap=1 if k < 3 else 0)
        rod(g, COP, (0, 40, c[2] + 40), (c[0] * 0.5, c[1] * 0.5 + 20, c[2] + 20), 26, steps=6)
        if az == 270:                                                                 # a balcony on the bubbles that face the road
            d = vec(az, -30)
            b = at(c, d, r * 0.98)
            g.box(INK, (b[0], b[1] - 30, b[2]), (210, 90, 10))
            g.box(COP_D, (b[0], b[1] - 72, b[2] + 50), (210, 6, 8))
            for sx in (-100, 0, 100):
                g.box(COP_D, (b[0] + sx, b[1] - 72, b[2] + 26), (6, 6, 50))
    g.box(INK, (0, -330, 110), (150, 12, 220))                                        # the street door in the stem's foot
    g.box(YEL, (0, -326, 235), (170, 10, 16))
    tilt_box(g, INK, (0, -330, 0), (0, -200, 0), 150, 8)
    # the washing line: from the low bubble's cap side out to the left middle bubble
    a, b = (-250, -270, 860), (250, -290, 850)
    rod(g, INK, a, b, 3.0, steps=5)
    w = sc.part(P.motion(swing=9, pivot=((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0), axis=0.0, rate=0.35, phase=0.2))
    for k, col in enumerate((MAG, CREAM, YEL, PINK, CREAM)):
        f = 0.12 + 0.19 * k
        w.box(col, (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f - 34), (50, 5, 62 if k % 2 else 48))
    planter(g, 210, -300, 2, 5)


def furnace_hall(sc):
    """The furnace: a copper drum with a glowing mouth, two bubbling chimneys, a bellows that works, a heap of sand."""
    g = sc.still()
    g.cyl(COP, (0, 0, -FOOT), 480, FOOT + 600, steps=12)
    for z in (60, 520):
        g.cyl(INK, (0, 0, z), 494, 40, steps=12)
    g.sphere(COP, (0, 0, 600), 480, scale=(1, 1, 0.45), steps=(6, 12))
    for k in range(12):                                                               # rivets round the upper band
        d = vec(k * 30 + 15)
        g.box(YEL, (d[0] * 497, d[1] * 497, 540), (16, 16, 16), rot=(0, k * 30 + 15, 0))
    # the mouth: an ink arch, a magenta ring, the yellow fire (steady)
    rod(g, INK, (0, -400, 260), (0, -492, 260), 250, steps=12)
    rod(g, MAG, (0, -450, 260), (0, -500, 260), 205, steps=12, glow=GW)
    rod(g, YEL, (0, -450, 260), (0, -508, 260), 150, steps=10, glow=GL)
    g.box(INK, (0, -520, 60), (360, 90, 120))
    tilt_box(g, INK, (0, -700, 0), (0, -560, 110), 300, 14)
    for sx, sd in ((-210, 21), (210, 22)):
        chimney(sc, g, sx, 90, 760, 760, r=58, n=5, seed=sd, amp=38, r0=22, dz=90)
    for k in range(3):                                                                # skylights: half-sunk bubbles on the dome
        d = vec(210 + k * 60)
        g.sphere(GLASS, (d[0] * 250, d[1] * 250, 745), 80, steps=(4, 8))
    # the annex: a foreman's bubble, piped to the drum
    c = (640, -40, 230)
    g.cyl(COP, (640, -40, -100), 46, 160, steps=6)
    bubble(g, c, 200, steps=(5, 8), lights=((270, 18, MAG, 0.26), (0, 15, YEL, 0.26)), door=(270, -34), cap=2)
    pipe(g, COP, [(640, -40, 430), (640, -40, 520), (560, -20, 560), (470, 0, 540)], 20)
    # the bellows: a wedge that opens and closes on the other side
    g.box(INK, (-600, -60, 60), (200, 150, 120))
    bl = sc.part(P.motion(swing=11, pivot=(-500, -60, 125), axis=90.0, rate=0.4, phase=0.0))
    bl.box(WOOD, (-610, -60, 150), (230, 140, 16))
    bl.box(MAG, (-705, -60, 190), (14, 100, 70))
    g.box(CREAM, (-555, -60, 132), (120, 120, 20))
    g.cyl(SAND, (-330, -540, 0), 170, 120, steps=8, top_r=12)
    g.cyl(SAND, (-150, -600, 0), 110, 80, steps=7, top_r=10)
    tilt_box(g, WOOD, (-250, -570, 60), (-200, -630, 200), 7, 7)
    g.box(INK, (-196, -634, 215), (30, 5, 40))


def blowpipe_tower(sc):
    """The blowpipe: a 20 m pipe on a tripod, a half-blown bubble at its top, swaying as the blower turns it."""
    g = sc.still()
    g.cyl(INK, (0, 0, -FOOT), 300, FOOT + 120, steps=10)
    bubble(g, (-170, 70, 300), 170, steps=(5, 8), lights=((270, 15, YEL, 0.28), (180, 12, MAG, 0.26)), door=(300, -30), cap=2)
    for k in range(3):
        d = vec(90 + k * 120)
        rod(g, COP, (d[0] * 270, d[1] * 270, 110), (d[0] * 60, d[1] * 60, 900), 22, steps=6)
    g.cyl(INK, (0, 0, 890), 200, 14, steps=10)
    for k in range(8):                                                                # the gallery's rail
        d0, d1 = vec(k * 45), vec(k * 45 + 45)
        g.box(COP_D, (d0[0] * 192, d0[1] * 192, 950), (7, 7, 100))
        tilt_box(g, COP_D, (d0[0] * 192, d0[1] * 192, 1000), (d1[0] * 192, d1[1] * 192, 1000), 7, 7)
    for k in range(11):                                                               # a spiral stair up to it
        d = vec(250 - k * 26)
        g.box(INK, (d[0] * 250, d[1] * 250, 150 + k * 72), (120, 46, 10), rot=(0, 250 - k * 26, 0))
    g.cyl(COP, (0, 0, 904), 70, 60, steps=8)
    p = sc.part(P.motion(swing=2.2, pivot=(0, 0, 120), axis=90.0, rate=0.11, phase=0.0))
    p.cyl(COP, (0, 0, 120), 28, 1960, steps=8)
    p.cyl(COP_D, (0, 0, 120), 60, 90, steps=8, top_r=28)
    for z in (520, 1240, 1640, 1960):
        p.cyl(INK, (0, 0, z), 40, 30, steps=8)
    p.cyl(MAG, (0, 0, 2040), 62, 60, steps=8, glow=GL)                                # the gather: hot glass at the pipe's end
    p.sphere(GLASS, (0, 0, 2390), 310, scale=(1, 1, 1.12), steps=(7, 12))
    p.sphere(PINK, (0, 0, 2130), 110, scale=(1, 1, 0.8), steps=(4, 8))
    tilt_box(p, YEL, (0, -30, 1300), (0, -30, 1560), 6, 40)                           # a long pennant strip down the pipe
    planter(g, 190, -210, 2, 7)


def bottle_shop(sc):
    """A shop the shape of a bottle: a striped awning, lit windows, bubbles painted on its band, a glass sign swinging."""
    g = sc.still()
    g.cyl(GLASS, (0, 0, -FOOT), 300, FOOT + 700, steps=12)
    g.cyl(COP, (0, 0, 0), 314, 90, steps=12)
    g.cyl(MAG, (0, 0, 400), 306, 170, steps=12)
    g.cyl(GLASS, (0, 0, 700), 300, 250, steps=12, top_r=110)
    g.cyl(COP, (0, 0, 690), 306, 30, steps=12)
    g.cyl(GLASS, (0, 0, 950), 110, 300, steps=10)
    g.cyl(COP, (0, 0, 1250), 136, 44, steps=10)
    g.sphere(MAG, (0, 0, 1400), 118, steps=(5, 8))
    g.sphere(YEL, (0, 0, 1530), 30, steps=(3, 6))
    for az, rr, zz in ((270, 46, 490), (252, 30, 520), (290, 24, 455), (180, 46, 490), (198, 28, 520), (0, 46, 490), (342, 28, 520)):   # the picture on the band: bubbles
        d = vec(az)
        rod(g, CREAM, (d[0] * 290, d[1] * 290, zz), (d[0] * 313, d[1] * 313, zz), rr, steps=8)
    g.box(INK, (0, -300, 120), (130, 14, 240))
    rod(g, INK, (0, -293, 240), (0, -307, 240), 65, steps=8)
    g.box(YEL, (0, -306, 120), (20, 6, 20))
    for az in (236, 304, 180, 0):                                                     # shop windows, lit
        d = vec(az)
        g.box(YEL, (d[0] * 292, d[1] * 292, 230), (118, 26, 130), rot=(0, az + 90, 0), glow=GW)
        g.box(COP_D, (d[0] * 298, d[1] * 298, 156), (140, 26, 12), rot=(0, az + 90, 0))
    for az in (270, 180, 0):
        d = vec(az, 0)
        rod(g, MAG, (d[0] * 290, d[1] * 290, 640), (d[0] * 309, d[1] * 309, 640), 34, steps=8, glow=GW)
    for k in range(6):                                                                # the awning: magenta and cream
        x = -150 + 60 * k
        tilt_box(g, MAG if k % 2 else CREAM, (x, -290, 350), (x, -470, 290), 60, 10)
        g.box(MAG if k % 2 else CREAM, (x, -472, 272), (60, 8, 30))
    for sx in (-170, 170):
        g.box(COP_D, (sx, -462, 140), (7, 7, 290))
    # the sign: a glass bottle hung from a curled bracket, both faces to the road's two directions
    pipe(g, COP, [(280, -60, 780), (420, -60, 800), (500, -60, 760), (510, -60, 700)], 12)
    s = sc.part(P.motion(swing=7, pivot=(510, -60, 700), axis=0.0, rate=0.3, phase=0.4))
    s.cyl(INK, (510, -60, 650), 4, 50, steps=5)
    s.cyl(MAG, (510, -60, 520), 34, 90, steps=8)
    s.cyl(MAG, (510, -60, 610), 34, 26, steps=8, top_r=12)
    s.cyl(CREAM, (510, -60, 632), 12, 22, steps=6)
    chimney(sc, g, -170, 120, 800, 330, r=30, n=3, seed=31, amp=26, r0=14, dz=60)
    planter(g, -250, -400, 3, 8)
    g.box(WOOD, (250, -400, 45), (170, 46, 10))                                       # a bench
    for sx in (180, 320):
        g.box(INK, (sx, -400, 20), (10, 40, 40))


def pump_house(sc):
    """The pump house: seven copper pipes of rising height like an organ, a glass boiler, a rocking beam."""
    g = sc.still()
    g.box(INK, (0, 0, -40), (1340, 420, 320))
    g.box(COP_D, (0, -212, 96), (1340, 10, 26))
    for k in range(7):
        x = -480 + 160 * k
        h = 420 + 180 * k
        g.cyl(COP, (x, 0, 110), 62, h, steps=8)
        g.cyl(COP_D, (x, 0, 110 + h), 62, 34, steps=8, top_r=76)
        g.cyl(MAG if k % 2 else YEL, (x, 0, 110 + h * 0.62), 68, 24, steps=8)
        g.box(INK, (x, -60, 250), (70, 12, 46))
        g.box(YEL, (x, -62, 222), (70, 10, 8))
        fizz(sc, x, 0, 150 + h, n=2, r0=17, dz=75, amp=30, rate=0.28 + 0.03 * k, seed=40 + k)
    # the boiler: a glass bubble on an ink cradle, magenta inside
    g.box(INK, (-790, 0, 60), (260, 300, 200))
    bubble(g, (-790, 0, 330), 205, steps=(6, 10), lights=((270, 5, MAG, 0.34), (180, 5, MAG, 0.3), (270, 48, YEL, 0.14)), cap=2)
    pipe(g, COP, [(-790, 0, 560), (-790, 0, 660), (-640, 0, 700), (-540, 0, 640)], 20)
    # the beam
    g.box(INK, (760, 0, 300), (46, 46, 520))
    g.box(INK, (760, 0, 20), (160, 200, 120))
    b = sc.part(P.motion(swing=13, pivot=(760, 0, 560), axis=90.0, rate=0.3, phase=0.0))
    b.box(COP_D, (760, 0, 560), (520, 30, 40))
    b.sphere(YEL, (1010, 0, 560), 46, steps=(4, 8))
    b.box(INK, (530, 0, 470), (16, 16, 180))
    g.cyl(MAG, (760, -30, 560), 30, 14, rot=(0, 0, 90), steps=8)
    for sx in (-300, 20, 340):                                                        # glass gauges on the plinth's front
        rod(g, GLASS, (sx, -206, 40), (sx, -222, 40), 34, steps=8)
        g.box(MAG, (sx, -224, 52), (6, 5, 30))


def dome_row(sc):
    """The cooling gallery: five half-sunk glass domes in a row under one copper spine pipe, racks of glass in front."""
    g = sc.still()
    g.box(INK, (0, 0, -90), (1560, 540, 240))
    for k in range(5):
        x = -620 + 310 * k
        g.sphere(GLASS, (x, 0, 60), 235, steps=(6, 10))
        g.cyl(COP, (x, 0, 28), 246, 50, steps=10)
        g.cyl(COP, (x, 0, 262), 120, 70, steps=8, top_r=40)
        col = YEL if k % 2 else MAG
        rod(g, col, (x, -200, 170), (x, -222, 190), 52, steps=8, glow=GW)
        rod(g, INK, (x, -226, 96), (x, -244, 96), 44, steps=8)
    for ex, az in ((-620, 180), (620, 0)):
        d = vec(az, 25)
        rod(g, YEL, (ex + d[0] * 205, 0, 60 + d[2] * 205), (ex + d[0] * 240, 0, 60 + d[2] * 240), 60, steps=8, glow=GW)
    rod(g, COP, (-700, 0, 345), (700, 0, 345), 30, steps=8)
    for k in range(5):
        g.cyl(INK, (-620 + 310 * k, 0, 325), 44, 40, steps=8)
    pipe(g, COP, [(700, 0, 345), (780, 0, 345), (780, 0, 150), (780, 0, 20)], 30)
    chimney(sc, g, -700, 0, 345, 300, r=34, n=4, seed=51, amp=28, r0=15, dz=64)
    for sx in (-380, 240):                                                            # racks of finished glass, two shelves
        for z in (60, 120):
            g.box(WOOD, (sx, -330, z), (330, 60, 8))
        for ex in (-160, 160):
            g.box(INK, (sx + ex, -330, 70), (8, 60, 140))
        for j in range(5):
            col = (GLASS, PINK, YEL, GLASS, MAG)[(j + (1 if sx > 0 else 0)) % 5]
            g.cyl(col, (sx - 120 + 60 * j, -330, 124), 15, 34, steps=6, top_r=7)
            g.sphere(col, (sx - 120 + 60 * j, -330, 84), 15, steps=(3, 6))
    tilt_box(g, MAG, (0, -272, 250), (0, -420, 205), 300, 10)                          # an awning between the racks
    for sx in (-146, 146):
        g.box(COP_D, (sx, -414, 100), (7, 7, 210))


def stem_villa(sc):
    """One great bubble high on a curled copper stem, a stair winding round it, a small bubble hung under like a drop."""
    g = sc.still()
    g.cyl(INK, (0, 120, -FOOT), 240, FOOT + 40, steps=10)
    stem = [(0, 120, 30), (0, 130, 330), (-70, 90, 620), (-50, 0, 850), (0, -10, 1010)]
    pipe(g, COP, stem, 52, steps=8)
    g.cyl(COP, (0, -10, 990), 150, 70, steps=8, top_r=210)
    c = (0, 0, 1400)
    bubble(g, c, 400, steps=(7, 12), lights=((240, 24, YEL, 0.17), (300, 24, MAG, 0.17), (180, 20, MAG, 0.2), (0, 20, YEL, 0.2), (270, 52, YEL, 0.12),
                                             (150, 22, YEL, 0.14), (30, 22, MAG, 0.14)), door=(270, -30), cap=2)
    g.cyl(INK, (0, -370, 1186), 190, 12, steps=10)                                    # the round balcony at the door
    for k in range(7):
        d0, d1 = vec(180 + k * 30), vec(210 + k * 30)
        g.box(COP_D, (d0[0] * 182, -370 + d0[1] * 182, 1240), (6, 6, 96))
        if k < 6:
            tilt_box(g, COP_D, (d0[0] * 182, -370 + d0[1] * 182, 1288), (d1[0] * 182, -370 + d1[1] * 182, 1288), 6, 6)
    for k in range(15):                                                               # the stair round the stem
        d = vec(300 - k * 30)
        g.box(INK, (d[0] * 135, 100 + d[1] * 135 - k * 7, 70 + k * 76), (130, 44, 10), rot=(0, 300 - k * 30, 0))
    # the drop: a small bubble hung under the big one on a looping pipe
    pipe(g, COP, [(330, 60, 1250), (430, 60, 1170), (440, 60, 1010), (400, 60, 930)], 20)
    bubble(g, (400, 60, 760), 175, steps=(5, 8), lights=((270, 10, MAG, 0.28), (0, 10, YEL, 0.28)), cap=1)
    # a tendril on the cap, a lantern bubble hung from it (steady)
    pipe(g, COP, [(0, 0, 1830), (0, 0, 1960), (-70, 0, 2040), (-160, 0, 2020), (-180, 0, 1950)], 16)
    w = sc.part(P.motion(swing=5, pivot=(-180, 0, 1950), axis=0.0, rate=0.22, phase=0.1))
    w.cyl(INK, (-180, 0, 1890), 4, 60, steps=5)
    w.sphere(MAG, (-180, 0, 1850), 44, steps=(4, 8), glow=GL)
    planter(g, 250, -150, 3, 12)
    planter(g, -260, -130, 2, 13)


# ------------------------------------------------------------------------------------------ props, tall (24 m and more)
def bubble_lamp(sc):
    """A bubble lamp on a curled stem (steady): the glass hangs from the curl and sways; a smaller one lower down."""
    g = sc.still()
    g.cyl(INK, (0, 0, -100), 40, 130, steps=8)
    g.cyl(COP_D, (0, 0, 30), 26, 30, steps=8, top_r=14)
    pipe(g, COP, [(0, 0, 30), (0, 0, 430), (12, 0, 540), (62, 0, 618), (135, 0, 640), (196, 0, 604), (210, 0, 545), (180, 0, 510)], 11)
    pipe(g, COP, [(0, 0, 300), (-50, 0, 370), (-112, 0, 372), (-132, 0, 330)], 9)
    for z in (150, 300):
        g.cyl(INK, (0, 0, z), 17, 16, steps=6)
    a = sc.part(P.motion(swing=4, pivot=(210, 0, 545), axis=0.0, rate=0.24, phase=0.0))
    a.cyl(INK, (210, 0, 482), 4, 64, steps=5)
    a.cyl(COP, (210, 0, 462), 30, 26, steps=8, top_r=8)
    a.sphere(GLASS, (210, 0, 418), 58, steps=(6, 10), glow=GL)
    a.sphere(MAG, (210, 0, 356), 14, steps=(3, 6), glow=GL)
    b = sc.part(P.motion(swing=5, pivot=(-132, 0, 330), axis=0.0, rate=0.3, phase=0.5))
    b.cyl(INK, (-132, 0, 286), 4, 46, steps=5)
    b.sphere(PINK, (-132, 0, 252), 38, steps=(5, 8), glow=GL)


def float_rack(sc):
    """A rack of glass floats hung in rows to cool: pale glass, pink and cream, each row swinging on its rail."""
    g = sc.still()
    for sx in (-270, 270):
        g.box(INK, (sx, 0, 135), (16, 16, 470))
        tilt_box(g, INK, (sx, -70, 0), (sx, 0, 150), 10, 10)
        tilt_box(g, INK, (sx, 70, 0), (sx, 0, 150), 10, 10)
    tilt_box(g, COP, (-300, 0, 372), (0, 0, 400), 60, 10)
    tilt_box(g, COP, (0, 0, 400), (300, 0, 372), 60, 10)
    g.sphere(YEL, (0, 0, 420), 18, steps=(3, 6))
    cols = (GLASS, GLASS, PINK, GLASS, SAND, GLASS, PINK, GLASS, GLASS, SAND, PINK, GLASS, GLASS, SAND, GLASS)
    for row, z in enumerate((340, 230, 120)):
        rod(g, COP_D, (-270, 0, z), (270, 0, z), 6, steps=6)
        m = sc.part(P.motion(swing=5, pivot=(0, 0, z), axis=0.0, rate=0.26 + 0.03 * row, phase=0.33 * row))
        for k in range(5):
            x = -200 + 100 * k
            m.cyl(INK, (x, 0, z - 22), 3.5, 22, steps=4)
            m.sphere(cols[row * 5 + k], (x, 0, z - 54), 34, steps=(5, 8))


def seahorse_vane(sc):
    """The quarter's weather-cock: a copper seahorse on a tall pole, turning slowly; glass balls for the four quarters."""
    g = sc.still()
    g.cyl(INK, (0, 0, -100), 46, 130, steps=8)
    g.cyl(COP, (0, 0, 30), 10, 520, steps=6)
    g.cyl(COP_D, (0, 0, 30), 22, 60, steps=6, top_r=10)
    for az in (0, 90):
        d = vec(az)
        rod(g, COP_D, (-d[0] * 66, -d[1] * 66, 470), (d[0] * 66, d[1] * 66, 470), 5, steps=5)
        for sg in (-1, 1):
            g.sphere(GLASS if az == 0 else PINK, (sg * d[0] * 70, sg * d[1] * 70, 470), 15, steps=(4, 6))
    g.sphere(YEL, (0, 0, 556), 16, steps=(4, 6))
    m = sc.part(P.motion(swing=55, pivot=(0, 0, 0), axis=P.UP, rate=0.06, phase=0.0))
    z0 = 600.0
    m.cyl(COP, (0, 0, 560), 6, 60, steps=5)
    rod(m, COP_D, (-120, 0, z0), (120, 0, z0), 5, steps=5)
    m.sphere(YEL, (128, 0, z0), 14, steps=(4, 6))
    m.box(MAG, (-135, 0, z0), (50, 7, 44))
    spine = [(-12, 42, 12), (16, 26, 14), (32, 48, 16), (26, 78, 20), (4, 100, 26), (-18, 136, 36), (-26, 176, 42), (-10, 214, 34), (18, 238, 26), (34, 254, 24)]
    for (x0, zz0, w0), (x1, zz1, w1) in zip(spine, spine[1:]):
        tilt_box(m, COP, (x0, 0, z0 + zz0), (x1, 0, z0 + zz1), 10, (w0 + w1) / 2.0)
    m.sphere(COP, (-30, 0, z0 + 168), 27, scale=(1, 0.3, 1.25), steps=(4, 8))            # the belly
    m.box(COP, (42, 0, z0 + 258), (48, 10, 32), rot=(-18, 0, 0))                          # the head
    tilt_box(m, COP, (58, 0, z0 + 250), (92, 0, z0 + 236), 10, 13)                        # a short round snout
    m.sphere(COP, (95, 0, z0 + 235), 9, steps=(3, 6))
    for k in range(3):                                                                    # the coronet: three knobs
        m.box(COP_D, (26 + k * 13, 0, z0 + 280 + (4 if k == 1 else 0)), (8, 8, 14))
    m.sphere(YEL, (46, 0, z0 + 264), 7, scale=(1, 1.3, 1), steps=(3, 6))


def chimney_stack(sc):
    """A works chimney standing by itself: a copper pipe with a kink in it, letting go a tall string of bubbles."""
    g = sc.still()
    g.cyl(INK, (0, 0, -150), 70, 180, steps=8)
    pipe(g, COP, [(0, 0, 30), (0, 0, 420), (80, 0, 520), (80, 0, 740), (0, 0, 840), (0, 0, 1000)], 38, steps=8)
    for p in ((0, 0, 200), (80, 0, 620), (0, 0, 900)):
        g.cyl(INK, p, 47, 26, steps=8)
    g.cyl(COP_D, (0, 0, 1000), 38, 50, steps=8, top_r=58)
    tilt_box(g, COP_D, (-90, 0, 0), (-10, 0, 330), 7, 7)
    rod(g, MAG, (0, -40, 300), (0, -62, 300), 30, steps=8)
    rod(g, INK, (0, -36, 300), (0, -68, 300), 6, steps=5)
    fizz(sc, 0, 0, 1050, n=6, r0=22, dz=84, amp=36, seed=61, spread=0.12)


def bottle_tree(sc):
    """A draining tree: a copper post with rings of arms, bottles hung upside down on them, turning slowly."""
    g = sc.still()
    g.cyl(INK, (0, 0, -100), 60, 124, steps=8)
    g.cyl(COP, (0, 0, 24), 11, 440, steps=6)
    g.sphere(GLASS, (0, 0, 486), 24, steps=(4, 6))
    m = sc.part(P.motion(swing=24, pivot=(0, 0, 0), axis=P.UP, rate=0.05, phase=0.0))
    cols = (GLASS, MAG, YEL, PINK, GLASS, SAND)
    n = 0
    for tier, (z, L) in enumerate(((150, 104), (265, 84), (370, 62))):
        m.cyl(COP_D, (0, 0, z - 8), 18, 16, steps=6)
        for k in range(5):
            d = vec(k * 72 + tier * 36)
            tip = (d[0] * L, d[1] * L, z + L * 0.42)
            rod(m, COP, (0, 0, z), tip, 4.5, steps=4)
            col = cols[n % 6]
            n += 1
            m.cyl(col, (tip[0], tip[1], tip[2] - 4), 13, 36, steps=6)
            m.cyl(col, (tip[0], tip[1], tip[2] - 20), 6, 18, steps=5)


def sign_post(sc):
    """A picture sign on a curled copper bracket: bubbles rising on one board, a bottle on the other. No words."""
    g = sc.still()
    g.cyl(INK, (0, 0, -100), 38, 126, steps=8)
    pipe(g, COP, [(0, 0, 26), (0, 0, 400), (0, 10, 470), (0, 45, 500), (0, 78, 470), (0, 70, 430)], 10)
    pipe(g, COP, [(0, 0, 250), (0, -15, 300), (0, -66, 318), (0, -90, 290)], 9)
    g.sphere(YEL, (0, 0, 420), 15, steps=(4, 6))
    a = sc.part(P.motion(swing=5, pivot=(0, 70, 430), axis=90.0, rate=0.27, phase=0.0))
    a.cyl(INK, (0, 70, 404), 4, 28, steps=5)
    rod(a, INK, (-6, 70, 340), (6, 70, 340), 66, steps=10)
    for sg in (-1, 1):                                                                   # bubbles rising, on both faces
        for (y, z, r) in ((52, 310, 20), (84, 344, 14), (64, 376, 9)):
            rod(a, GLASS, (sg * 5, y, z), (sg * 10, y, z), r, steps=8)
    b = sc.part(P.motion(swing=6, pivot=(0, -90, 290), axis=90.0, rate=0.31, phase=0.4))
    b.cyl(INK, (0, -90, 268), 4, 24, steps=5)
    b.box(YEL, (0, -90, 214), (10, 96, 110))
    for sg in (-1, 1):                                                                   # a bottle, on both faces
        b.box(INK, (sg * 6, -90, 198), (5, 40, 54))
        b.box(INK, (sg * 6, -90, 238), (5, 16, 30))
        b.box(MAG, (sg * 7, -90, 196), (5, 40, 12))


def glass_stall(sc):
    """A market stall of glass: a striped awning on copper posts, vases on the counter, baubles hung along the front."""
    g = sc.still()
    g.box(INK, (0, 20, 45), (310, 90, 90))
    g.box(WOOD, (0, 14, 94), (330, 110, 8))
    for sx in (-150, 150):
        for sy in (-36, 70):
            g.box(COP, (sx, sy, 150), (8, 8, 300))
    for k in range(6):
        x = -137.5 + 55 * k
        col = MAG if k % 2 else CREAM
        tilt_box(g, col, (x, 78, 300), (x, -96, 240), 55, 9)
        g.box(col, (x, -98, 224), (55, 7, 26))
    g.box(WOOD, (0, 74, 170), (300, 26, 7))                                              # the back shelf
    for k, col in enumerate((GLASS, MAG, YEL, PINK, GLASS)):
        x = -120 + 60 * k
        g.cyl(col, (x, 6, 98), 13, 26 + 8 * (k % 3), steps=6, top_r=7 + 4 * (k % 2))
        g.sphere(col, (x, 6, 98 + 34 + 8 * (k % 3)), 12 + 2 * (k % 2), steps=(3, 6))
    for k, col in enumerate((PINK, GLASS, YEL, GLASS)):
        g.cyl(col, (-105 + 70 * k, 74, 174), 11, 40, steps=6, top_r=5)
    rod(g, COP_D, (-150, -36, 222), (150, -36, 222), 4, steps=5)
    m = sc.part(P.motion(swing=8, pivot=(0, -36, 222), axis=0.0, rate=0.4, phase=0.0))
    for k, col in enumerate((GLASS, PINK, GLASS, MAG, GLASS)):
        x = -110 + 55 * k
        m.cyl(INK, (x, -36, 196), 3.5, 26, steps=4)
        m.sphere(col, (x, -36, 182), 15, steps=(4, 6))
    g.cyl(INK, (190, -10, 0), 24, 40, steps=6)                                           # a bucket of sand beside it
    g.cyl(SAND, (190, -10, 40), 22, 14, steps=6, top_r=6)


# ------------------------------------------------------------------------------------------ props, low (from 17 m)
def mould_bench(sc):
    """A mould bench: three bottle moulds on an ink bench (one open like a book, the bottle in it), a blowpipe, a bucket."""
    g = sc.still()
    g.box(INK, (0, 0, 80), (430, 86, 12))
    for sx in (-200, 200):
        for sy in (-32, 32):
            g.box(INK, (sx, sy, 37), (12, 12, 78))
    g.box(INK, (0, 0, 22), (400, 10, 10))
    for k, (x, col) in enumerate(((-140, GLASS), (0, PINK), (140, SAND))):
        gap = (26, 12, 0)[k]
        for sg in (-1, 1):
            g.box(INK, (x + sg * (20 + gap), 0, 126), (20, 56, 80))
            g.box(YEL, (x + sg * (20 + gap), -30, 126), (8, 6, 12))
        g.cyl(col, (x, 0, 86), 15, 62, steps=6)
        g.cyl(col, (x, 0, 148), 15, 14, steps=6, top_r=6)
        g.cyl(col, (x, 0, 162), 6, 12, steps=5)
    tilt_box(g, COP, (236, -20, 0), (262, -20, 168), 7, 7)                               # a blowpipe leant on the end
    g.sphere(GLASS, (262, -20, 172), 12, steps=(3, 6))
    g.cyl(INK, (-262, 6, 0), 24, 38, steps=6)
    g.cyl(SAND, (-262, 6, 38), 22, 12, steps=6, top_r=5)


def pipe_vent(sc):
    """A low copper pipe run on ink saddles, two upturned mouths venting small bubbles, a magenta valve wheel."""
    g = sc.still()
    rod(g, COP, (-260, 0, 44), (260, 0, 44), 22, steps=8)
    for sx in (-260, 260):
        rod(g, COP, (sx, 0, 44), (sx + (40 if sx > 0 else -40), 0, -60), 22, steps=8)
        g.sphere(COP, (sx, 0, 44), 25, steps=(4, 8))
    for sx in (-130, 60, 220):
        g.box(INK, (sx, 0, 14), (30, 64, 30))
    for sx in (-40, 140):
        g.cyl(INK, (sx - 13, 0, 44), 28, 26, rot=(-90, 0, 0), steps=8)
    for k, sx in enumerate((-200, 150)):
        g.cyl(COP, (sx, 0, 44), 18, 70, steps=8)
        g.cyl(COP_D, (sx, 0, 114), 18, 22, steps=8, top_r=30)
        fizz(sc, sx, 0, 132, n=3, r0=7, dz=13, amp=6, rate=0.5, seed=70 + k, spread=0.3)
    g.cyl(COP_D, (0, 0, 60), 5, 34, steps=5)
    rod(g, MAG, (0, -6, 104), (0, 6, 104), 24, steps=8)
    rod(g, INK, (0, -9, 104), (0, 9, 104), 6, steps=5)


def sand_sledge(sc):
    """The sand carriers' sledge: runners, a plank deck, two heaps of sand, shovels, a sieve, a rope."""
    g = sc.still()
    for sy in (-46, 46):
        tilt_box(g, INK, (-170, sy, 8), (150, sy, 8), 10, 12)
        tilt_box(g, INK, (150, sy, 8), (196, sy, 44), 10, 12)
    g.box(WOOD, (-10, 0, 28), (300, 112, 10))
    for sx in (-120, 0, 110):
        g.box(INK, (sx, 0, 18), (12, 100, 12))
    g.cyl(SAND, (-80, 0, 33), 58, 70, steps=8, top_r=6)
    g.cyl(SAND, (40, 6, 33), 46, 52, steps=7, top_r=6)
    for (x, y, lean) in ((-96, -14, 24), (58, 16, -18)):
        tilt_box(g, WOOD, (x, y, 70), (x + lean, y, 150), 6, 6)
        g.box(INK, (x + lean, y, 156), (22, 6, 10))
        g.box(INK, (x - lean * 0.2, y, 62), (22, 5, 30))
    g.cyl(MAG, (110, -18, 33), 20, 34, steps=6, top_r=24)                                # a bucket
    g.cyl(WOOD, (-150, 26, 33), 30, 9, steps=8)                                          # a sieve
    g.cyl(CREAM, (216, 0, 0), 26, 10, steps=8)                                           # the tow rope, coiled
    tilt_box(g, CREAM, (190, 0, 40), (216, 0, 8), 5, 5)


def gazing_globes(sc):
    """A bed of finished glass globes on copper stakes: pale glass, pink and cream, between an ink kerb."""
    r = random.Random(77)
    g = sc.still()
    g.box(INK, (0, -56, 8), (360, 8, 16))
    g.box(INK, (0, 56, 8), (360, 8, 16))
    for sx in (-180, 180):
        g.box(INK, (sx, 0, 8), (8, 120, 16))
    g.box(SAND, (0, 0, 4), (352, 104, 8))
    cols = (GLASS, PINK, GLASS, SAND, GLASS, PINK, GLASS, GLASS)
    for k in range(8):
        x = -150 + 43 * k + r.uniform(-8, 8)
        y = (-22 if k % 2 else 24) + r.uniform(-8, 8)
        h = r.uniform(46, 118)
        rr = r.uniform(20, 34)
        g.cyl(COP_D, (x, y, 0), 4.5, h, steps=5)
        g.cyl(COP, (x, y, h - 6), rr * 0.45, 8, steps=6)
        g.sphere(cols[k], (x, y, h + rr * 0.9), rr, steps=(5, 8))


# ------------------------------------------------------------------------------------------ the hero
def bubble_tree(sc):
    """THE BUBBLE TREE: a 60 m copper trunk, twelve glass-sphere houses on its boughs, bubbles rising from its top."""
    g = sc.still()
    segs = ((-300, 1100, 300, 215, 0), (1100, 2700, 215, 165, 30), (2700, 4300, 165, 110, -10), (4300, 5700, 110, 64, 20))
    for z0, z1, r0, r1, ox in segs:
        g.cyl(COP, (ox, 0, z0), r0, z1 - z0 + 30, steps=10, top_r=r1)
        g.cyl(INK, (ox, 0, z1 - 20), r1 * 1.25, 60, steps=10)
    for k in range(5):                                                                    # the roots
        d = vec(k * 72 + 20)
        rod(g, COP, (d[0] * 560, d[1] * 560, -220), (d[0] * 170, d[1] * 170, 520), 85, steps=6)
    g.box(INK, (0, -300, 150), (170, 30, 300))                                            # the door in the trunk
    g.box(YEL, (0, -312, 310), (200, 20, 22), glow=GW)
    n = 12
    for k in range(n):
        f = k / float(n - 1)
        z = 1250 + 4150 * f
        az = 270 + k * 137.5
        L = 760 - 330 * f
        r = 330 - 130 * f
        tr = 250 - 180 * f
        ox = 30 if 1100 <= z < 2700 else (-10 if 2700 <= z < 4300 else (20 if z >= 4300 else 0))
        d = vec(az)
        a = (ox + d[0] * tr * 0.6, d[1] * tr * 0.6, z)
        mid = (ox + d[0] * (tr + L * 0.6), d[1] * (tr + L * 0.6), z + L * 0.12)
        end = (ox + d[0] * (tr + L), d[1] * (tr + L), z + L * 0.42)
        rod(g, COP, a, mid, 46 - 14 * f, steps=6)
        rod(g, COP, mid, end, 40 - 12 * f, steps=6)
        c = (end[0], end[1], end[2] - r * 0.86)                                            # the house hangs under the bough's end
        g.sphere(GLASS, c, r, steps=(6, 10) if k < 7 else (5, 8))
        g.cyl(COP, (c[0], c[1], c[2] - r * 0.07), r * 1.035, r * 0.14, steps=10 if k < 7 else 8)
        g.cyl(COP, (c[0], c[1], c[2] + r * 0.74), r * 0.66, r * 0.4, steps=8, top_r=r * 0.14)
        col = YEL if k % 2 else MAG
        for daz, el, ff in ((0, 22, 0.22), (110, 16, 0.2), (-110, 16, 0.2)):               # lit windows: outward and to both sides
            dd = vec(az + daz, el)
            rod(g, col if daz == 0 else (MAG if col == YEL else YEL), at(c, dd, r * 0.88), at(c, dd, r + 9.0), r * ff, steps=6, glow=GL)
    # the crown: a funnel, and the works' bubbles going up
    g.cyl(COP_D, (20, 0, 5730), 64, 170, steps=10, top_r=190)
    g.cyl(MAG, (20, 0, 5890), 196, 24, steps=10, glow=GW)
    rr = random.Random(99)
    for k in range(9):
        m = sc.part(P.motion(bob=-(70 + 14 * k), rate=round(0.16 + 0.02 * (k % 4), 3), phase=round(k * 0.137, 3),
                             swing=1.5, pivot=(20, 0, 5900), axis=round(rr.uniform(0, 180), 1)))
        m.sphere(GLASS, (20 + rr.uniform(-1, 1) * (40 + 30 * k), rr.uniform(-1, 1) * (40 + 30 * k), 6020 + 125 * k), 44 + 13 * k, steps=(5, 8) if k > 3 else (4, 6))


JOBS = [
    ('SM_S1D6_ThreeBubble', three_bubble),
    ('SM_S1D6_GrapeTenement', grape_tenement, SMALL),
    ('SM_S1D6_FurnaceHall', furnace_hall),
    ('SM_S1D6_BlowpipeTower', blowpipe_tower, dict(grow=(130, 40, 40), grow_down=(130, 40, 0))),
    ('SM_S1D6_BottleShop', bottle_shop, SMALL),
    ('SM_S1D6_PumpHouse', pump_house),
    ('SM_S1D6_DomeRow', dome_row),
    ('SM_S1D6_StemVilla', stem_villa, SMALL),
    ('SM_S1D6_BubbleLamp', bubble_lamp, SMALL),
    ('SM_S1D6_FloatRack', float_rack, SMALL),
    ('SM_S1D6_SeahorseVane', seahorse_vane, dict(grow=(30, 115, 20), grow_down=(30, 115, 0))),
    ('SM_S1D6_ChimneyStack', chimney_stack, dict(grow=(40, 40, 60), grow_down=(40, 40, 0))),
    ('SM_S1D6_BottleTree', bottle_tree, SMALL),
    ('SM_S1D6_SignPost', sign_post, SMALL),
    ('SM_S1D6_GlassStall', glass_stall, SMALL),
    ('SM_S1D6_MouldBench', mould_bench, STILL),
    ('SM_S1D6_PipeVent', pipe_vent, LOWMOVE),
    ('SM_S1D6_SandSledge', sand_sledge, STILL),
    ('SM_S1D6_GazingGlobes', gazing_globes, STILL),
    ('SM_S1D6_BubbleTree', bubble_tree, dict(far=True, grow=(80, 80, 140), grow_down=(80, 80, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; NEVER build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
