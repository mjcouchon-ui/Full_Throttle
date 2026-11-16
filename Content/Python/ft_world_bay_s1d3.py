"""Bayshore, Stage 1, builder s1d3: district 3 "Pleasure Beach" (6.4 to 9.8 km), the fairground.
/Game/World/Bayshore/SM_S1D3_*. Run: import ft_world_bay_s1d3 as K; K.run()

1 to 2 storeys under big roofs: striped tent cones, onion and bell domes, scalloped valances, false fronts, round plans,
bunting, all in WEDGE stripes. Palette: yellow, red, white, pool turquoise, lavender (no orange).
Every piece is one ft_people.Scene (one material, one section). Placed by C:/ftwork/s1/L0_s1d3.inl.

How the wedge stripes are made: two many-sided cones (or prisms) of two colours on the same axis, one turned half a
side. Each one's corners stand proud of the other's flats, so the surface is pleats of alternate colours: tent canvas.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # rod(m, col, a, b, r, steps=), tilt_box(m, col, a, b, width, depth)

DIR = '/Game/World/Bayshore'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

YEL, RED, WHITE, TURQ, LAV = '#ffe14d', '#ff4d5e', '#fffaf2', '#49e2d5', '#b9a4ff'
NAVY, PINK, MINT, CREAM = '#2b2350', '#ff9ec7', '#7fe3c4', '#fff1d6'
WAFER, WOOD, WOOD_D, GREY, STEM = '#e8b26a', '#c9a06a', '#9a744a', '#c9c4d6', '#1f9a62'


# ------------------------------------------------------------------------------------------ helpers
def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def side(m, col, pts, y0, y1, glow=0.0):
    """A profile [(x, z)] in the plane the road sees, from y0 to y1."""
    m.side(col, _ccw(pts), y0, y1, glow=glow)


def stripes(m, c1, c2, base, r, h, n=8, top_r=None, turn=0.0):
    """Wedge stripes: a cone or a prism in two colours (see the header)."""
    m.cyl(c1, base, r, h, steps=n, top_r=top_r, rot=(0, turn, 0))
    m.cyl(c2, base, r, h, steps=n, top_r=top_r, rot=(0, turn + 180.0 / n, 0))


def frusta(m, cols, x, y, prof, n=8):
    """A turned shape (a dome, a bowl, a stem): prof = [(radius, z)] from the bottom up; one colour or two (pleats)."""
    for (r0, z0), (r1, z1) in zip(prof, prof[1:]):
        m.cyl(cols[0], (x, y, z0), r0, z1 - z0, steps=n, top_r=max(r1, 6.0))
        if len(cols) > 1:
            m.cyl(cols[1], (x, y, z0), r0, z1 - z0, steps=n, top_r=max(r1, 6.0), rot=(0, 180.0 / n, 0))


def ring(m, cols, x, y, z, r, n, w, a0=0.0):
    """A scalloped valance round a rim: n pennants (squares on their corners), their middles at z."""
    for k in range(n):
        a = a0 + 2 * math.pi * k / n
        m.box(cols[k % len(cols)], (x + math.cos(a) * r, y + math.sin(a) * r, z), (w, 6, w), rot=(45, math.degrees(a) + 90.0, 0))


def valance(m, cols, x0, x1, y, z, w):
    """The same along a straight edge that faces the road."""
    n = max(2, int(round((x1 - x0) / (w * 1.42))))
    for k in range(n):
        m.box(cols[k % len(cols)], (x0 + (k + 0.5) * (x1 - x0) / n, y, z), (w, 6, w), rot=(45, 0, 0))


def bunting(m, cols, a, b, n, w=44.0, sag=0.1, rope=WHITE):
    """A line of pennants from a to b, hanging a little."""
    L = math.sqrt(sum((b[i] - a[i]) ** 2 for i in range(3)))
    mid = ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0 - L * sag)
    tilt_box(m, rope, a, mid, 5, 5)
    tilt_box(m, rope, mid, b, 5, 5)
    yaw = math.degrees(math.atan2(b[1] - a[1], b[0] - a[0]))
    for k in range(n):
        f = (k + 0.5) / n
        p, q = (a, mid) if f < 0.5 else (mid, b)
        g = f * 2.0 if f < 0.5 else f * 2.0 - 1.0
        m.box(cols[k % len(cols)], (p[0] + (q[0] - p[0]) * g, p[1] + (q[1] - p[1]) * g, p[2] + (q[2] - p[2]) * g - w * 0.55), (w, 6, w), rot=(45, yaw, 0))


def pennant(sc, col, x, y, z, w=120.0, h=56.0, swing=14.0, rate=0.4, phase=0.0, mast=0.0, col2=None):
    """A flag that turns about its mast (drawn too when mast > 0: z is the flag's middle)."""
    if mast:
        g = sc.still()
        g.cyl(GREY, (x, y, z + h / 2.0 + 6 - mast), 6, mast, steps=6)
        g.sphere(YEL, (x, y, z + h / 2.0 + 14), 13, steps=(3, 6))
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + w / 2.0 + 5, y, z), (w, 5, h))
    if col2:
        f.box(col2, (x + w + 5 + w * 0.2, y, z), (w * 0.4, 5, h * 0.6))


def fan(m, cols, cx, cz, r, a0, a1, n, y0, y1):
    """A sunburst: n wedges from (cx, cz), from angle a0 to a1 (degrees, 0 = +X, 90 = up), in the plane the road sees."""
    for k in range(n):
        b0 = math.radians(a0 + (a1 - a0) * k / n)
        b1 = math.radians(a0 + (a1 - a0) * (k + 1) / n)
        side(m, cols[k % len(cols)], [(cx, cz), (cx + math.cos(b0) * r, cz + math.sin(b0) * r), (cx + math.cos(b1) * r, cz + math.sin(b1) * r)], y0, y1)


def bulbs(m, pts, r=16.0, col=YEL, glow=0.0):
    for p in pts:
        m.sphere(col, p, r, steps=(3, 5), glow=glow)


def plush(m, x, y, z, s, col, bow):
    """A plush toy: round body, round head, two ears, a bow."""
    m.sphere(col, (x, y, z + 24 * s), 26 * s, steps=(3, 6))
    m.sphere(col, (x, y - 2 * s, z + 62 * s), 19 * s, steps=(3, 6))
    for k in (-1, 1):
        m.box(col, (x + k * 14 * s, y, z + 82 * s), (11 * s, 7 * s, 13 * s))
    m.box(bow, (x, y - 22 * s, z + 44 * s), (20 * s, max(6 * s, 5.0), max(9 * s, 6.0)))


# ------------------------------------------------------------------------------------------ BUILDINGS
def big_top(sc):
    """A round show tent 12 m across and 12.5 m to its pennant: red and white walls, a red and yellow cone, four
    masts with bunting to the crown, a porch."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -300), 590, 300, steps=12)
    stripes(g, RED, WHITE, (0, 0, 0), 600, 330, n=12)
    g.cyl(YEL, (0, 0, 330), 640, 46, steps=12)
    ring(g, (TURQ, LAV), 0, 0, 328, 644, 24, 78)
    stripes(g, RED, YEL, (0, 0, 376), 650, 470, n=12, top_r=120)
    stripes(g, YEL, RED, (0, 0, 846), 150, 150, n=6, top_r=44)
    g.sphere(TURQ, (0, 0, 1016), 44, steps=(4, 6))
    pennant(sc, LAV, 0, 0, 1210, w=150, h=64, mast=200, col2=YEL, phase=0.2)
    g = sc.still()
    g.box(NAVY, (0, -596, 140), (250, 40, 290))                       # the way in, its curtains and its porch
    for k in (-1, 1):
        g.box(LAV, (k * 96, -620, 150), (62, 10, 270), rot=(k * 5, 0, 0))
        g.box(WHITE, (k * 170, -740, 150), (16, 16, 300))
    side(g, YEL, [(-200, 296), (200, 296), (0, 410)], -760, -600)
    fan(g, (RED, WHITE), 0, 300, 96, 20, 160, 3, -768, -760)
    for k, a in enumerate((45, 135, 225, 315)):
        x, y = math.cos(math.radians(a)) * 760, math.sin(math.radians(a)) * 760
        g.cyl(WHITE, (x, y, -150), 8, 720, steps=6)
        g.sphere(YEL, (x, y, 580), 18, steps=(3, 6))
        bunting(g, (YEL, TURQ, WHITE, LAV), (x, y, 560), (math.cos(math.radians(a)) * 150, math.sin(math.radians(a)) * 150, 990), 6, w=50)


def duck_gallery(sc):
    """A hook-a-duck stall, four booths under one long striped roof with a scalloped valance, 16 x 5 m: yellow ducks
    ride a turquoise trough one way and a second row the other; prizes hang under the roof; a giant duck is the sign."""
    g = sc.still()
    g.box(WOOD, (0, 0, -140), (1600, 480, 320))
    g.box(RED, (0, 210, 230), (1600, 50, 420))
    for x in (-780, 780):
        g.box(LAV, (x, 0, 230), (40, 440, 420))
    for k in range(16):
        g.box(YEL if k % 2 else WHITE, (-750 + k * 100, -222, 95), (100, 44, 150))
        g.box(RED if k % 2 else YEL, (-750 + k * 100, -40, 486), (100, 640, 14), rot=(0, 0, -10))
    for x in (-800, -400, 0, 400, 800):
        g.box(WHITE, (x, -220, 230), (30, 30, 420))
    valance(g, (WHITE, TURQ), -800, 800, -356, 420, 56)
    for k, x in enumerate((-600, -200, 200, 600)):                      # targets on the back wall, prizes under the roof
        rod(g, WHITE, (x, 186, 345), (x, 176, 345), 46, steps=8)
        rod(g, RED, (x, 180, 345), (x, 170, 345), 26, steps=8)
        plush(g, x - 110, -250, 318, 0.8, PINK if k % 2 else WHITE, TURQ)
        plush(g, x + 110, -250, 318, 0.8, WHITE if k % 2 else PINK, RED)
    g.box(TURQ, (0, 110, 222), (1540, 110, 14))                         # the trough
    g.box(WHITE, (0, 50, 232), (1540, 12, 34))
    for row, (y, z, rate, n) in enumerate(((120, 244, 0.045, 7), (80, 244, -0.035, 5))):
        for k in range(n):
            x = -700 + (k + 0.37 * row) * 1400.0 / n
            d = sc.part(P.motion(travel=(1480.0, rate), base=(x, y), pivot=(x, y, z + 20)))
            s = 1 if rate > 0 else -1
            d.box(YEL, (x, y, z + 16), (58, 26, 32))
            d.box(YEL, (x + s * 22, y, z + 44), (26, 22, 26))
            d.box(RED, (x + s * 41, y, z + 42), (14, 12, 8))
            d.box(YEL, (x - s * 30, y, z + 34), (12, 20, 12))
    g = sc.still()
    g.box(WHITE, (0, 60, 560), (420, 40, 60))                           # the sign: a duck as big as a cow
    g.sphere(YEL, (-20, 60, 690), 130, scale=(1.35, 0.8, 0.9), steps=(5, 8))
    g.sphere(YEL, (130, 60, 830), 78, steps=(5, 8))
    g.box(RED, (222, 60, 820), (70, 50, 26))
    g.box(YEL, (-200, 60, 760), (70, 90, 60), rot=(30, 0, 0))
    for k in (-1, 1):
        g.box(NAVY, (160, 60 + k * 70, 852), (16, 8, 18))
        stripes(g, RED, WHITE, (k * 800, -224, 440), 60, 70, n=6)
        stripes(g, YEL, RED, (k * 800, -224, 510), 96, 150, n=6, top_r=8)
        pennant(sc, TURQ if k > 0 else LAV, k * 800, -224, 760, w=110, h=50, mast=130, phase=0.3 + 0.2 * k)
        g = sc.still()


def fun_house(sc):
    """The fun house, 12 x 9 m: a crooked false front 13.6 m tall with a sunburst of wedge stripes over a rolling
    tunnel for a door (it rocks half a turn each way), crooked windows, a striped turret on the hut behind."""
    g = sc.still()
    g.box(LAV, (0, 110, 150), (1080, 680, 900))
    side(g, RED, [(-570, 600), (570, 600), (0, 800)], -230, 450)
    g.box(YEL, (0, -270, 430), (1180, 40, 1460), rot=(3, 0, 0))                    # the false front, out of true
    fan(g, (RED, WHITE), 0, 300, 560, 0, 180, 9, -302, -292)
    g.box(RED, (-390, -272, 1205), (380, 46, 170), rot=(-6, 0, 0))                  # a parapet in three steps, none level
    g.box(TURQ, (10, -272, 1270), (430, 46, 190), rot=(3, 0, 0))
    g.box(WHITE, (400, -272, 1190), (370, 46, 150), rot=(8, 0, 0))
    for k, (x, tilt) in enumerate(((-400, 9), (400, -7))):
        g.box(WHITE, (x, -294, 940), (210, 12, 250), rot=(tilt, 0, 0))
        g.box(NAVY, (x, -300, 940), (170, 12, 210), rot=(tilt, 0, 0))
        g.box(YEL, (x - 30, -306, 925), (60, 8, 150), rot=(tilt, 0, 0), glow=0.5)
        g.box(TURQ if k else RED, (x, -330, 1085), (250, 90, 12), rot=(tilt, 0, -14))
    g.box(RED, (0, -292, 130), (400, 40, 430))                                     # the door's surround, the steps
    g.box(WOOD, (0, -420, -140), (440, 240, 300))
    bulbs(g, [(math.cos(math.radians(a)) * 232, -318, 150 + math.sin(math.radians(a)) * 232) for a in range(0, 181, 30)], r=17, glow=0.8)
    d = sc.part(P.motion(swing=180, pivot=(0, -300, 150), axis=90.0, rate=0.12))   # the rolling tunnel
    rod(d, LAV, (0, -250, 150), (0, -330, 150), 150, steps=10)
    d.box(WHITE, (0, -334, 150), (290, 8, 56))
    d.box(RED, (0, -334, 150), (56, 10, 290))
    rod(d, NAVY, (0, -330, 150), (0, -342, 150), 46, steps=8)
    g = sc.still()
    stripes(g, TURQ, WHITE, (330, 220, 640), 130, 330, n=6)                        # the turret
    g.cyl(YEL, (330, 220, 962), 176, 26, steps=12)
    stripes(g, RED, YEL, (330, 220, 988), 170, 250, n=6, top_r=8)
    pennant(sc, RED, 330, 220, 1340, w=120, h=54, mast=140, col2=WHITE, phase=0.6)
    g = sc.still()
    for k in (-1, 1):                                                              # bunting down to two masts in front
        g.cyl(WHITE, (k * 600, -640, -150), 8, 650, steps=6)
        g.sphere(RED if k > 0 else TURQ, (k * 600, -640, 512), 20, steps=(3, 6))
        bunting(g, (WHITE, RED, TURQ, YEL), (k * 600, -640, 490), (k * 560, -296, 1120), 6, w=50)
    g.box(WHITE, (-330, -330, 250), (150, 70, 12))                                  # a ticket hatch and its awning
    g.box(NAVY, (-330, -294, 330), (130, 10, 130))
    for k in range(4):
        g.box(RED if k % 2 else WHITE, (-390 + k * 40, -340, 425), (40, 110, 10), rot=(0, 0, -16))


def mirror_hall(sc):
    """The hall of mirrors, 10 x 11 m and 16.5 m to its pennant: an eight-sided turquoise and white drum under an
    onion dome, a lavender front of tall mirrors that lean each its own way, two small onion turrets."""
    g = sc.still()
    stripes(g, TURQ, WHITE, (0, 60, -300), 500, 860, n=8)
    g.cyl(YEL, (0, 60, 560), 530, 40, steps=16)
    ring(g, (RED, WHITE), 0, 60, 556, 532, 16, 70)
    g.cyl(WHITE, (0, 60, 600), 300, 90, steps=8)
    frusta(g, (TURQ, WHITE), 0, 60, [(270, 690), (440, 800), (480, 940), (390, 1090), (210, 1220), (80, 1330), (14, 1500)], n=8)
    g.sphere(YEL, (0, 60, 1510), 36, steps=(4, 6))
    pennant(sc, RED, 0, 60, 1600, w=130, h=56, mast=100, col2=YEL, phase=0.1)
    g = sc.still()
    g.box(LAV, (0, -470, 110), (940, 50, 820))
    g.box(YEL, (0, -476, 545), (1010, 76, 50))
    valance(g, (WHITE, TURQ), -500, 500, -512, 516, 52)
    for k, x in enumerate((-350, -175, 175, 350)):                                  # the mirrors: frame, glass, a glint
        t = (5, -4, 4, -6)[k]
        g.box(WHITE, (x, -498, 235), (136, 12, 430), rot=(t, 0, 0))
        g.box(TURQ, (x, -504, 235), (108, 12, 396), rot=(t, 0, 0))
        g.sphere(WHITE, (x, -506, 150 + 70 * (k % 2)), 44, scale=(1, 0.12, 1.5), steps=(4, 8))
        g.box(WHITE, (x - 12, -512, 300), (14, 6, 250), rot=(t + 22, 0, 0))
    g.box(NAVY, (0, -498, 120), (150, 12, 270))                                     # the door, round-headed
    rod(g, NAVY, (0, -492, 255), (0, -504, 255), 75, steps=10)
    rod(g, RED, (0, -486, 255), (0, -498, 255), 96, steps=10)
    g.box(WHITE, (0, -560, -140), (420, 150, 320))
    for k in (-1, 1):
        g.cyl(WHITE, (k * 470, -470, 520), 58, 170, steps=6)
        frusta(g, (RED,), k * 470, -470, [(64, 690), (108, 750), (60, 840), (10, 960)], n=6)
        g.sphere(YEL, (k * 470, -470, 968), 16, steps=(3, 6))
        bunting(g, (YEL, RED, WHITE), (k * 470, -470, 930), (k * 150, -40, 690), 5, w=44)


def dodgem_pavilion(sc):
    """The dodgems, a round pavilion 14 m across: a red and yellow roof on a ring of eight posts, a bell-domed
    lantern, and six round cars that go slowly round the middle (four with drivers)."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -300), 650, 320, steps=12)
    g.cyl(LAV, (0, 0, 20), 612, 14, steps=12)
    for k in range(12):
        a = 2 * math.pi * (k + 0.5) / 12
        g.box(RED if k % 2 else YEL, (math.cos(a) * 626, math.sin(a) * 626, 62), (330, 26, 56), rot=(0, math.degrees(a) + 90, 0))
    for k in range(8):
        a = 2 * math.pi * k / 8
        g.cyl(WHITE, (math.cos(a) * 590, math.sin(a) * 590, 30), 18, 370, steps=6)
    g.cyl(WHITE, (0, 0, 392), 690, 40, steps=12)
    ring(g, (TURQ, LAV), 0, 0, 392, 694, 24, 80)
    stripes(g, RED, YEL, (0, 0, 430), 700, 320, n=12, top_r=170)
    stripes(g, WHITE, TURQ, (0, 0, 750), 170, 140, n=6)
    frusta(g, (RED,), 0, 0, [(210, 890), (196, 960), (120, 1040), (16, 1110)], n=8)
    g.sphere(YEL, (0, 0, 1120), 26, steps=(4, 6))
    pennant(sc, TURQ, 0, 0, 1240, w=130, h=56, mast=130, col2=WHITE, phase=0.4)
    g = sc.still()
    g.cyl(TURQ, (0, 0, 30), 130, 90, steps=8)
    g.cyl(WHITE, (0, 0, 30), 30, 370, steps=6)
    bulbs(g, [(math.cos(math.radians(a)) * 704, math.sin(math.radians(a)) * 704, 436) for a in range(195, 360, 30)], r=18, glow=0.8)
    r = random.Random(31)
    cols = (TURQ, PINK, YEL, MINT, LAV, CREAM)
    rate = 0.06
    for k in range(6):
        a = math.radians(k * 60 + r.uniform(-14, 14))
        rad = 300 + (k % 2) * 150
        x, y = math.cos(a) * rad, math.sin(a) * rad
        c = sc.part(P.motion(travel=(-1.0, rate), base=(0.0, 0.0)))
        yaw = math.degrees(a) + 90.0
        c.box(WHITE, (x, y, 50), (176, 122, 18), rot=(0, yaw, 0))
        c.box(cols[k], (x, y, 74), (150, 100, 46), rot=(0, yaw, 0))
        c.box(cols[k], (x + math.sin(a) * 52, y - math.cos(a) * 52, 112), (26, 96, 60), rot=(0, yaw, 0))
        c.cyl(GREY, (x + math.sin(a) * 60, y - math.cos(a) * 60, 110), 5, 290, steps=6)
        if k not in (2, 5):
            d = P.look(r, tops=(RED, YEL, TURQ, LAV, WHITE))
            P.figure(sc, x, y, 40.0, yaw=math.degrees(a) + 180.0, legs='sit', seat=40.0, arm_l='fwd', arm_r='fwd', detail=0, orbit=(0.0, 0.0, rate), **d)


def penny_palace(sc):
    """The penny arcade, a palace of a false front 12 m wide and 12.6 m tall: three arches under a striped awning,
    a sunburst medallion, a line of bulbs, a striped turret with an onion at each end."""
    g = sc.still()
    g.box(LAV, (0, 90, 110), (1080, 640, 820))
    g.box(WHITE, (0, -250, 200), (1180, 50, 1000))
    g.box(RED, (0, -252, 745), (540, 56, 110))
    rod(g, YEL, (0, -222, 800), (0, -282, 800), 230, steps=12)
    fan(g, (RED, YEL), 0, 800, 220, 0, 180, 6, -292, -282)
    for k in (-1, 1):
        stripes(g, RED, YEL, (k * 560, -250, -300), 92, 1080, n=6)
        g.cyl(WHITE, (k * 560, -250, 780), 112, 30, steps=12)
        frusta(g, (TURQ,), k * 560, -250, [(84, 810), (132, 880), (76, 990), (10, 1130)], n=6)
        g.sphere(YEL, (k * 560, -250, 1138), 18, steps=(3, 6))
        pennant(sc, RED if k > 0 else LAV, k * 560, -250, 1210, w=110, h=50, mast=80, phase=0.25 + 0.3 * k)
        g = sc.still()
    for x in (-320, 0, 320):
        g.box(NAVY, (x, -278, 90), (210, 10, 300))
        rod(g, NAVY, (x, -273, 240), (x, -283, 240), 105, steps=10)
        g.box(YEL, (x, -284, 70), (150, 6, 200), glow=0.5)
    for k in range(10):
        g.box(YEL if k % 2 else WHITE, (-432 + k * 96, -330, 452), (96, 160, 12), rot=(0, 0, -14))
    valance(g, (TURQ, WHITE), -480, 480, -408, 420, 56)
    bulbs(g, [(-440 + k * 110, -282, 640) for k in range(9)], r=17, glow=0.8)
    g.box(WHITE, (0, -340, -140), (700, 160, 320))


def puppet_theatre(sc):
    """A puppet booth 5.4 m wide and 9 m tall: a red and white skirt, a turquoise-curtained opening where two
    puppets rock, a gable in wedge stripes, and three children on a bench in front."""
    g = sc.still()
    g.box(RED, (0, 0, -25), (500, 300, 550))
    for k in range(5):
        g.box(WHITE, (-200 + k * 100, -152, 125), (50, 8, 250))
    for k in (-1, 1):
        g.box(YEL, (k * 232, -132, 385), (40, 40, 270))
        g.box(LAV, (k * 242, 10, 385), (20, 250, 270))
        g.box(TURQ, (k * 172, -142, 400), (84, 10, 244), rot=(k * 6, 0, 0))
    g.box(LAV, (0, 132, 385), (500, 40, 270))
    g.box(NAVY, (0, 106, 385), (440, 10, 250))
    g.box(TURQ, (0, -142, 500), (430, 12, 50))
    g.box(WHITE, (0, -162, 250), (530, 64, 20))
    g.box(RED, (0, -100, 560), (550, 230, 80))
    valance(g, (WHITE, YEL), -270, 270, -218, 522, 46)
    side(g, YEL, [(-300, 600), (300, 600), (0, 800)], -220, 20)
    fan(g, (RED, WHITE), 0, 604, 180, 34, 146, 4, -228, -220)
    g.sphere(TURQ, (0, -100, 822), 30, steps=(4, 6))
    pennant(sc, RED, 0, -100, 900, w=100, h=46, mast=70, phase=0.5)
    for k, (x, col, ear) in enumerate(((-80, YEL, RED), (90, PINK, WHITE))):         # the puppets
        p = sc.part(P.motion(swing=16, pivot=(x, -60, 250), axis=90.0, rate=0.55, phase=0.5 * k))
        p.cyl(col, (x, -60, 250), 40, 100, steps=6, top_r=26)
        p.sphere(CREAM, (x, -60, 380), 34, steps=(4, 6))
        p.cyl(ear, (x, -60, 404), 30, 60, steps=6, top_r=6)
        for j in (-1, 1):
            tilt_box(p, col, (x + j * 24, -60, 330), (x + j * 62, -76, 372), 14, 14)
    r = random.Random(12)
    g = sc.still()
    g.box(WOOD, (0, -330, 24), (360, 44, 10))
    for x in (-150, 150):
        g.box(WOOD_D, (x, -330, -60), (16, 40, 160))
    for k in range(3):
        d = P.look(r, build='kid', s=r.uniform(0.62, 0.7))
        P.figure(sc, -110 + k * 110, -330, 0.0, yaw=180.0, legs='sit', seat=44.0, arm_l='vee' if k == 1 else 'down', arm_r='wave' if k != 1 else 'vee',
                 detail=0, rate=0.7, bob=3.0, phase=r.random(), **d)


def candy_floss(sc):
    """The candy-floss palace: an eight-sided kiosk 8 m across its awning and 10 m tall, lavender and white, three
    hatches, a red and yellow awning with a valance, a turquoise and white bell dome, a pink cloud of floss on top."""
    g = sc.still()
    stripes(g, LAV, WHITE, (0, 0, -300), 270, 600, n=8, turn=22.5)
    g.cyl(WHITE, (0, 0, 92), 300, 18, steps=8, rot=(0, 22.5, 0))
    for k, a in enumerate((-90, -45, -135)):
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        g.box(NAVY, (c * 266, s * 266, 195), (150, 14, 150), rot=(0, a + 90, 0))
        g.box(YEL, (c * 272, s * 272, 180), (100, 8, 90), rot=(0, a + 90, 0), glow=0.5)
        g.cyl(WHITE, (c * 290 + s * 50, s * 290 - c * 50, 108), 6, 64, steps=6)   # a stick of floss on each counter
        g.sphere((PINK, WHITE, TURQ)[k], (c * 290 + s * 50, s * 290 - c * 50, 190), 26, steps=(3, 6))
    stripes(g, YEL, RED, (0, 0, 300), 400, 96, n=8, top_r=270, turn=22.5)
    ring(g, (WHITE, TURQ), 0, 0, 298, 396, 16, 70, a0=math.radians(11.25))
    frusta(g, (TURQ, WHITE), 0, 0, [(272, 396), (264, 480), (236, 570), (180, 650), (100, 710), (30, 742)], n=8)
    g.cyl(WHITE, (0, 0, 736), 10, 150, steps=6)
    for x, y, z, rr in ((0, 0, 900, 92), (56, -30, 850, 66), (-58, 24, 860, 62), (10, 20, 962, 58)):
        g.sphere(PINK, (x, y, z), rr, steps=(4, 6))
    g.box(WHITE, (0, -330, -140), (260, 140, 310))
    pennant(sc, RED, 240, 60, 470, w=90, h=44, mast=100, phase=0.7)


# ------------------------------------------------------------------------------------------ PROPS
def strength_tower(sc):
    """Test your strength: a striker 9.7 m tall, a scale in bands of colour, a bell under a small striped cap with
    a sunburst behind it; the red puck hops up the scale and falls back. No light."""
    g = sc.still()
    g.box(RED, (0, 0, -55), (190, 170, 190))
    g.box(WHITE, (0, 0, 44), (200, 180, 10))
    g.box(YEL, (0, -60, 60), (76, 56, 24))
    tilt_box(g, WOOD, (70, -70, 54), (128, -84, 150), 10, 10)                       # the mallet, leant on the base
    g.box(WOOD_D, (132, -84, 160), (40, 30, 30), rot=(60, 0, 0))
    g.box(WHITE, (0, 24, 390), (72, 24, 700))
    for k in (-1, 1):
        g.box(RED, (k * 43, 22, 390), (14, 32, 700))
    for k in range(7):
        g.box((YEL, TURQ, LAV, WHITE, YEL, TURQ, RED)[k], (0, 9, 96 + k * 92), (58, 8, 84))
    fan(g, (YEL, RED), 0, 800, 150, 0, 360, 8, 30, 38)
    g.sphere(YEL, (0, 4, 800), 58, scale=(1, 0.6, 1), steps=(4, 8))
    g.box(WHITE, (0, 20, 745), (120, 40, 14))
    stripes(g, RED, WHITE, (0, 24, 870), 110, 100, n=6, top_r=8)
    g.cyl(WHITE, (0, 24, 740), 6, 134, steps=6)
    bulbs(g, [(k * 43, 4, 110 + j * 150) for k in (-1, 1) for j in range(5)], r=11, col=WHITE)
    g.box(WOOD, (-150, 20, -40), (90, 70, 220))                                     # a prize shelf beside it
    plush(g, -150, 20, 70, 0.8, PINK, WHITE)
    p = sc.part(P.motion(bob=420.0, rate=0.2))
    p.box(RED, (0, -1, 84), (46, 12, 30))


def ticket_drum(sc):
    """A ticket booth like a striped drum, 2.8 m across and 5.3 m tall: a hatch, a cone roof with a valance, a giant
    blank ticket on top, two rope posts."""
    g = sc.still()
    stripes(g, RED, WHITE, (0, 0, -150), 140, 400, n=8, turn=22.5)
    g.box(NAVY, (0, -137, 172), (120, 14, 96))
    g.box(WHITE, (0, -152, 118), (156, 44, 10))
    g.box(YEL, (0, -141, 232), (136, 10, 16))
    g.cyl(WHITE, (0, 0, 250), 196, 14, steps=16)
    ring(g, (TURQ, WHITE), 0, 0, 250, 198, 12, 54)
    stripes(g, YEL, RED, (0, 0, 264), 200, 130, n=8, top_r=24)
    g.cyl(WHITE, (0, 0, 390), 8, 60, steps=6)
    g.box(YEL, (0, 0, 480), (170, 14, 84))                                           # the ticket: no words
    g.box(RED, (0, -8, 480), (40, 6, 40), rot=(45, 0, 0))
    g.box(RED, (0, 8, 480), (40, 6, 40), rot=(45, 0, 0))
    for k in (-1, 1):
        g.box(WHITE, (k * 60, -9, 480), (8, 6, 60))
        g.cyl(LAV, (k * 120, -80, -40), 7, 150, steps=6)
        g.sphere(YEL, (k * 120, -80, 116), 12, steps=(3, 6))
    tilt_box(g, WHITE, (-120, -80, 96), (120, -80, 96), 5, 5)
    pennant(sc, LAV, 0, 0, 560, w=80, h=40, mast=60, phase=0.15)


def lollipops(sc):
    """Three giant lollipops on posts, 4.6 to 6.2 m tall, each a disc in wedge stripes that turns on its post, one
    turn in eight seconds and more. No light."""
    g = sc.still()
    g.box(WHITE, (0, 0, -55), (430, 70, 190))
    for k in range(4):
        g.box(RED, (-162 + k * 108, 0, -55), (54, 74, 192))
    for k, (x, h, rr, c1, c2, rate) in enumerate(((-150, 400, 62, RED, WHITE, 0.12), (0, 520, 76, TURQ, WHITE, -0.1), (150, 350, 56, YEL, RED, 0.11))):
        g.cyl(WHITE, (x, 0, 40), 7, h - 40, steps=6)
        g.box(YEL if k != 2 else TURQ, (x, 0, h - rr - 16), (40, 10, 16))
        p = sc.part(P.motion(travel=(-1.0, rate), base=(float(x), 0.0), pivot=(x, 0, h)))
        rod(p, c1, (x, -8, h), (x, 8, h), rr, steps=10)
        fan(p, (c2,), x, h, rr - 2, 0, 45, 1, -13, 13)
        fan(p, (c2,), x, h, rr - 2, 90, 135, 1, -13, 13)
        fan(p, (c2,), x, h, rr - 2, 180, 225, 1, -13, 13)
        fan(p, (c2,), x, h, rr - 2, 270, 315, 1, -13, 13)


def bunting_mast(sc):
    """A ribbon mast 8.4 m tall, red and white in bands, six lines of pennants from its crown to pegs 1.7 m out."""
    g = sc.still()
    stripes(g, YEL, RED, (0, 0, -150), 56, 200, n=6)
    for k in range(7):
        g.cyl(RED if k % 2 else WHITE, (0, 0, 50 + k * 100), 8, 100, steps=6)
    stripes(g, RED, YEL, (0, 0, 750), 40, 60, n=6, top_r=6)
    g.sphere(TURQ, (0, 0, 822), 16, steps=(3, 6))
    cols = ((YEL, RED, WHITE), (TURQ, WHITE, LAV), (RED, YEL, TURQ))
    for k in range(6):
        a = math.radians(k * 60 + 30)
        x, y = math.cos(a) * 168, math.sin(a) * 168
        g.box(WHITE, (x, y, 10), (14, 14, 60))
        bunting(g, cols[k % 3], (x, y, 40), (math.cos(a) * 10, math.sin(a) * 10, 740), 6, w=38, sag=0.03)
    pennant(sc, RED, 0, 0, 800, w=110, h=48, col2=YEL, phase=0.35)


def prize_stall(sc):
    """A prize stall 4 x 2.6 m and 4.6 m to the bear on its roof: three steps of plush toys in pink and white under a
    striped awning with a valance."""
    g = sc.still()
    g.box(RED, (0, -70, -25), (400, 90, 250))
    for k in range(4):
        g.box(WHITE, (-150 + k * 100, -117, -25), (50, 8, 250))
    g.box(WHITE, (0, -72, 104), (420, 104, 10))
    for x in (-190, 190):
        for y in (-110, 120):
            g.box(WHITE, (x, y, 150), (14, 14, 320))
    for k in range(8):
        g.box(RED if k % 2 else YEL, (-175 + k * 50, -10, 322), (50, 310, 10), rot=(0, 0, -10))
    valance(g, (WHITE, TURQ), -200, 200, -164, 290, 42)
    g.box(LAV, (0, 124, 150), (390, 12, 300))
    r = random.Random(8)
    for tier in range(3):
        g.box(WOOD, (0, 20 + tier * 34, 20 + tier * 62), (380, 40, 220 + 0 * tier))
        for k in range(4):
            plush(g, -150 + k * 100 + (tier % 2) * 30 - 15, 14 + tier * 34, 130 + tier * 62, r.uniform(0.62, 0.78),
                  PINK if (k + tier) % 2 else WHITE, TURQ if k % 2 else RED)
    plush(g, 60, 20, 318, 1.7, PINK, WHITE)                                          # the top prize, on the roof
    for k in range(4):
        g.sphere(WHITE if k % 2 else PINK, (-150 + k * 100, -150, 232), 20, steps=(3, 6))


def mirror_row(sc):
    """Four distorting mirrors in a row, 6 m long, 1.9 m tall (LOW): coloured frames with round heads on a lavender
    plinth, turquoise glass with a bulge and a glint, the same on both faces."""
    g = sc.still()
    g.box(LAV, (0, 0, -60), (600, 60, 180))
    for k in range(4):
        x = -225 + k * 150
        col = (RED, YEL, TURQ, RED)[k]
        g.box(col, (x, 0, 95), (118, 18, 130))
        rod(g, col, (x, -9, 160), (x, 9, 160), 30, steps=8)
        for s in (-1, 1):
            g.box(TURQ, (x, s * 10, 95), (94, 6, 108))
            g.sphere(WHITE if k % 2 else TURQ, (x, s * 11, 80 + 26 * (k % 2)), 30, scale=(1, 0.14, 1.4 - 0.5 * (k % 2)), steps=(3, 6))
            g.box(WHITE, (x - 22, s * 14, 104), (10, 5, 80), rot=(20, 0, 0))
        for s in (-1, 1):
            g.box(WHITE, (x + s * 62, 0, 40), (10, 40, 20))


def rocking_horses(sc):
    """Three rocking horses on a red and white platform, 5.4 m long, 1.9 m tall (LOW): each rocks on its bows, out
    of step with the next. No light."""
    g = sc.still()
    g.box(RED, (0, 0, -62), (540, 112, 176))
    for k in range(5):
        g.box(WHITE, (-216 + k * 108, 0, 28), (54, 116, 6))
    for k, (x, col, col2, sad) in enumerate(((-170, WHITE, LAV, RED), (0, PINK, WHITE, TURQ), (170, CREAM, RED, YEL))):
        h = sc.part(P.motion(swing=8, pivot=(x, 0, 34), axis=90.0, rate=0.45, phase=0.31 * k))
        arc = [(x - 78, 64), (x - 40, 44), (x, 36), (x + 40, 44), (x + 78, 64), (x + 78, 76), (x + 40, 56), (x, 48), (x - 40, 56), (x - 78, 76)]
        for s in (-1, 1):
            side(h, sad, arc, s * 20 - 3, s * 20 + 3)
            for e in (-1, 1):
                h.box(col, (x + e * 30, s * 11, 74), (11, 9, 52))
        h.box(col, (x, 0, 112), (88, 30, 36))
        tilt_box(h, col, (x + 34, 0, 120), (x + 58, 0, 160), 20, 22)
        h.box(col, (x + 68, 0, 158), (36, 18, 20), rot=(-25, 0, 0))
        tilt_box(h, col2, (x + 26, 0, 134), (x + 46, 0, 168), 8, 10)
        tilt_box(h, col2, (x - 43, 0, 124), (x - 64, 0, 84), 9, 9)
        for s in (-1, 1):
            h.box(col, (x + 54, s * 7, 174), (6, 5, 10))
        h.box(sad, (x - 2, 0, 133), (36, 34, 8))
        h.cyl(YEL, (x, 0, 40), 5, 134, steps=6)
        h.sphere(YEL, (x, 0, 178), 8, steps=(3, 6))


# ------------------------------------------------------------------------------------------ THE HERO
def big_sundae(sc):
    """THE BIG SUNDAE, 46 m tall: a round ice-cream parlour 20 m across (lit windows, a red and white awning 27 m
    across with a valance, a roof terrace with parasols) and on its roof a sundae glass: a white fluted stem, a
    turquoise bowl 28 m across, three scoops 14 m across (pink, mint, cream), whipped cream, a red cherry (no
    light), a striped wafer fan and a rolled wafer. Bunting from under the bowl to the terrace."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -300), 1000, 720, steps=16, rot=(0, 11.25, 0))
    g.cyl(LAV, (0, 0, -300), 1030, 330, steps=16, rot=(0, 11.25, 0))
    for k in range(16):
        a = 2 * math.pi * k / 16
        if k == 12:
            continue
        c, s = math.cos(a), math.sin(a)
        g.box(NAVY, (c * 978, s * 978, 232), (230, 12, 236), rot=(0, math.degrees(a) + 90, 0))
        g.box(YEL, (c * 984, s * 984, 232), (190, 10, 196), rot=(0, math.degrees(a) + 90, 0), glow=0.5)
    g.box(RED, (0, -984, 150), (300, 16, 330))                                       # the door
    g.box(NAVY, (0, -990, 140), (250, 12, 290))
    g.box(YEL, (0, -994, 150), (10, 10, 290), glow=0.5)
    side(g, YEL, [(-230, 330), (230, 330), (0, 470)], -1500, -1000)
    for k in (-1, 1):
        g.box(WHITE, (k * 200, -1470, 100), (22, 22, 460))
    stripes(g, RED, WHITE, (0, 0, 420), 1380, 190, n=16, top_r=1000)                 # the awning
    ring(g, (YEL, TURQ), 0, 0, 420, 1372, 24, 150)
    g.cyl(LAV, (0, 0, 600), 1000, 24, steps=16, rot=(0, 11.25, 0))                   # the terrace
    for k in range(12):
        a = 2 * math.pi * (k + 0.5) / 12
        x, y = math.cos(a) * 950, math.sin(a) * 950
        g.box(WHITE, (x, y, 680), (20, 20, 120))
    for k in range(4):
        a = math.radians(45 + k * 90)
        x, y = math.cos(a) * 760, math.sin(a) * 760
        g.cyl(WHITE, (x, y, 620), 8, 280, steps=6)
        stripes(g, (YEL, TURQ)[k % 2], WHITE, (x, y, 876), 190, 70, n=6, top_r=10)
    # ---- the glass
    g.cyl(WHITE, (0, 0, 624), 560, 50, steps=16)
    frusta(g, (WHITE, LAV), 0, 0, [(520, 674), (240, 830), (200, 1100), (176, 1380)], n=8)
    g.sphere(WHITE, (0, 0, 1420), 250, scale=(1, 1, 0.6), steps=(5, 10))
    frusta(g, (TURQ,), 0, 0, [(230, 1470), (760, 1620), (1180, 1950), (1400, 2420)], n=16)
    g.cyl(WHITE, (0, 0, 2420), 1440, 60, steps=16)
    tilt_box(g, WHITE, (-668, -953, 1900), (-806, -1150, 2370), 90, 10)               # two glints on the bowl
    tilt_box(g, WHITE, (-890, -760, 2000), (-1000, -855, 2300), 50, 10)
    # ---- the ice cream
    r = random.Random(7)
    for col, (x, y, z) in ((PINK, (-430, -160, 2630)), (MINT, (430, -160, 2630)), (CREAM, (0, 170, 3230))):
        g.sphere(col, (x, y, z), 700, steps=(7, 12))
        g.cyl(col, (x, y, z - 330), 735, 130, steps=12, top_r=610)
        if col != CREAM:
            for j in range(6):
                az, el = math.radians(r.uniform(200, 340)), math.radians(r.uniform(15, 70))
                p = (x + 702 * math.cos(el) * math.cos(az), y + 702 * math.cos(el) * math.sin(az), z + 702 * math.sin(el))
                g.box((YEL, RED, TURQ, LAV, WHITE)[j % 5], p, (110, 30, 30), rot=(r.uniform(-50, 50), r.uniform(0, 180), 0))
    frusta(g, (WHITE, CREAM), 0, 170, [(430, 3800), (330, 3960), (190, 4090), (70, 4190)], n=8)
    g.sphere(RED, (30, 170, 4270), 150, steps=(6, 10))
    tilt_box(g, STEM, (30, 170, 4400), (130, 170, 4570), 16, 16)
    fan(g, (WAFER, WHITE), 380, 3000, 1050, 22, 88, 6, 400, 440)                     # the wafer fan, behind the top scoop
    rod(g, WAFER, (-420, 320, 3000), (-880, 440, 3950), 70, steps=8)                 # the rolled wafer
    for f in (0.35, 0.6, 0.85):
        a = (-420 - 460 * f, 320 + 120 * f, 3000 + 950 * f)
        rod(g, RED, a, (a[0] - 26, a[1] + 7, a[2] + 54), 77, steps=8)
    # ---- bunting, and the terrace's flags
    cols = ((YEL, RED, WHITE, TURQ), (LAV, WHITE, RED, YEL))
    for k in range(6):
        a = 2 * math.pi * (k + 0.5) / 6
        bunting(g, cols[k % 2], (math.cos(a) * 950, math.sin(a) * 950, 760), (math.cos(a) * 250, math.sin(a) * 250, 1400), 6, w=100, sag=0.06)
    for k in range(4):
        a = 2 * math.pi * (k * 3 + 0.5) / 12
        pennant(sc, (RED, TURQ, YEL, LAV)[k], math.cos(a) * 950, math.sin(a) * 950, 900, w=170, h=80, mast=150, phase=0.23 * k)


JOBS = [('SM_S1D3_BigTop', big_top), ('SM_S1D3_DuckGallery', duck_gallery), ('SM_S1D3_FunHouse', fun_house),
        ('SM_S1D3_MirrorHall', mirror_hall), ('SM_S1D3_DodgemPavilion', dodgem_pavilion), ('SM_S1D3_PennyPalace', penny_palace),
        ('SM_S1D3_PuppetTheatre', puppet_theatre), ('SM_S1D3_CandyFloss', candy_floss),
        ('SM_S1D3_StrengthTower', strength_tower, dict(grow=(10, 10, 10), grow_down=(10, 10, 0))),
        ('SM_S1D3_TicketDrum', ticket_drum, dict(grow=(20, 30, 10), grow_down=(20, 30, 0))),
        ('SM_S1D3_Lollipops', lollipops, dict(grow=(10, 70, 10), grow_down=(10, 70, 0))),
        ('SM_S1D3_BuntingMast', bunting_mast, dict(grow=(10, 10, 10), grow_down=(10, 10, 0))),
        ('SM_S1D3_PrizeStall', prize_stall, STILL), ('SM_S1D3_MirrorRow', mirror_row, STILL),
        ('SM_S1D3_RockingHorses', rocking_horses, dict(grow=(30, 20, 20), grow_down=(30, 20, 0))),
        ('SM_S1D3_BigSundae', big_sundae)]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilds them
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
