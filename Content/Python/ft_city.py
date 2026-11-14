"""Bayshore, the beach town of level 1: pastel art-deco hotels, palms, lifeguard towers, beach
umbrellas, street lamps, a sunset billboard, and the far skyline with a pier Ferris wheel and a
lighthouse. Origin of each prop: middle of its base on the ground; its front faces -X (the road
camera looks along +X), so it reads from the chase camera."""
import math
import unreal
import ft_art

PINK = '#ff9ec7'
HOTPINK = '#ff4fb8'
MINT = '#7fe3c4'
LAV = '#b9a4ff'
PEACH = '#ffc59e'
CREAM = '#fff1d6'
CORAL = '#ff7a8a'
TEAL = '#2fc6c0'
SKY = '#8fe3ff'
GLASS = '#4a7fc0'
GLASS2 = '#6fd0f0'
WHITE = '#fffaf2'
TRUNK = '#c08a5a'
TRUNK_D = '#9a6a44'
LEAF = '#2fbf71'
LEAF_D = '#1f9a62'
YEL = '#ffe14d'
NAVY = '#2b2350'
SAND = '#ffe2a8'
RED = '#ff4d5e'


def palm(m, h=900, lean=12, seed=0, x=0, y=0):
    """A curved trunk of tapering rings and a crown of drooping fronds."""
    n = 9
    px, pz = x, 0.0
    a = math.radians(lean)
    for k in range(n):
        t = k / float(n)
        r0 = 34 - 16 * t
        seg = h / n
        ang = a * (0.3 + t * 1.2)
        m.cyl(TRUNK if k % 2 == 0 else TRUNK_D, (px, y, pz), r0, seg + 6, rot=(-math.degrees(ang), 0, 0), steps=10, top_r=r0 - 2)
        px += math.sin(ang) * seg
        pz += math.cos(ang) * seg
    top = (px, y, pz)
    for k in range(9):
        yaw = k * 40 + seed * 17
        for j, (dist, drop, ln) in enumerate([(0, -8, 190), (150, -40, 170), (290, -95, 140)]):
            yr = math.radians(yaw)
            cx = top[0] + math.cos(yr) * (dist + ln / 2) * 0.95
            cy = top[1] + math.sin(yr) * (dist + ln / 2) * 0.95
            m.box(LEAF if (k + j) % 2 == 0 else LEAF_D, (cx, cy, top[2] + drop), (ln, 70 - j * 16, 8), rot=(-(12 + j * 18), yaw, 0))
    for k in range(3):
        aa = k * 2.1
        m.sphere('#7a4b2a', (top[0] + math.cos(aa) * 26, top[1] + math.sin(aa) * 26, top[2] - 24), 20, steps=(6, 8))


def hotel(m, w=1800, d=1200, floors=7, col=PINK, trim=MINT, neon=HOTPINK, tower=True, sign=YEL):
    """An art-deco beach hotel: eyebrow ledges over window bands, a rounded corner tower, a stepped top."""
    fh = 320
    H = floors * fh + 120
    m.box(col, (d / 2, 0, H / 2), (d, w, H))
    for f in range(floors):
        z = 120 + f * fh
        m.box(GLASS if f % 2 else GLASS2, (-3, 0, z + 150), (6, w * 0.84, 130))
        m.box(WHITE, (-24, 0, z + 236), (48, w * 0.9, 18))
        for k in range(int(w / 300)):
            m.box(col, (-6, -w * 0.42 + (k + 1) * w * 0.84 / (int(w / 300) + 1), z + 150), (8, 26, 134))
    m.box(trim, (-6, 0, 60), (12, w, 120))
    m.box(GLASS, (-14, 0, 70), (10, w * 0.4, 110))
    m.box(neon, (-16, 0, H - 40), (8, w, 16), glow=0.7)
    m.box(neon, (-16, -w / 2 + 10, H / 2), (8, 16, H - 80), glow=0.7)
    # stepped crown
    m.box(trim, (d / 2, 0, H + 60), (d * 0.7, w * 0.6, 120))
    m.box(col, (d / 2, 0, H + 160), (d * 0.45, w * 0.36, 80))
    m.box(sign, (d * 0.3, 0, H + 270), (20, w * 0.5, 140), glow=0.3)
    m.box(neon, (d * 0.3 - 12, 0, H + 270), (6, w * 0.42, 40), glow=0.9)
    if tower:
        m.cyl(trim, (0, w / 2, 0), 260, H + 260, steps=20)
        for f in range(floors):
            m.cyl(GLASS2, (0, w / 2, 120 + f * fh + 90), 266, 110, steps=20)
            m.cyl(WHITE, (0, w / 2, 120 + f * fh + 230), 290, 16, steps=20)
        m.cyl(col, (0, w / 2, H + 260), 200, 120, steps=20)
        m.cyl(neon, (0, w / 2, H + 380), 26, 260, steps=10, glow=0.7)
        m.sphere(YEL, (0, w / 2, H + 660), 50, glow=0.6)


def lifeguard(m, col=CORAL, col2=WHITE):
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(WHITE, (sx * 90, sy * 90, 150), (16, 16, 300))
    m.box(col2, (0, 0, 300), (260, 260, 16))
    m.box(col, (0, 0, 420), (220, 220, 220))
    for k in range(4):
        m.box(col2 if k % 2 else col, (-112, -80 + k * 54, 420), (4, 50, 220))
    m.box(SKY, (-114, 0, 450), (4, 150, 80))
    m.plan(TEAL, [(-150, -150), (150, -150), (150, 150), (-150, 150)], 530, 548)
    m.cyl(TEAL, (0, 0, 548), 150, 70, steps=4, top_r=10, rot=(0, 45, 0))
    m.side(WHITE, [(-200, 0), (-130, 300), (-110, 300), (-180, 0)], -40, 40)
    m.cyl(WHITE, (0, 0, 610), 4, 220, steps=6)
    m.side(RED, [(0, 830), (90, 805), (0, 780)], -2, 2)


def umbrella(m, col=HOTPINK, col2=WHITE, x=0, y=0):
    m.cyl(WHITE, (x, y, 0), 4, 230, steps=6, rot=(-8, 0, 0))
    m.cyl(col, (x + 30, y, 200), 170, 60, steps=8, top_r=8)
    m.cyl(col2, (x + 30, y, 196), 172, 8, steps=8)
    m.box(col2 if col2 != WHITE else SKY, (x - 20, y + 120, 2), (200, 90, 4))
    m.box(col, (x - 20, y + 120, 5), (200, 20, 4))


def lamp(m, flip=1):
    m.cyl(NAVY, (0, 0, 0), 14, 30, steps=8)
    m.cyl(TEAL, (0, 0, 30), 7, 700, steps=8)
    m.box(TEAL, (0, flip * 120, 720), (10, 250, 10))
    m.box(NAVY, (0, flip * 240, 706), (40, 70, 22))
    m.box(YEL, (0, flip * 240, 694), (34, 60, 4), glow=0.8)
    m.box(HOTPINK, (-8, 0, 420), (4, 40, 180), glow=0.5)


def billboard(m):
    """A sunset billboard: striped 80s sun, sea, a palm silhouette. Faces -X."""
    W, H, z0 = 1200, 600, 560
    for y in (-420, 420):
        m.box(NAVY, (40, y, z0 / 2), (30, 30, z0 + 40))
    m.box(NAVY, (10, 0, z0 + H / 2), (20, W + 40, H + 40))
    bands = ['#ffd2f0', '#ffb3e0', '#ff9ec7', '#ff8bb0', '#ff7a9a']
    for k, b in enumerate(bands):
        m.box(b, (-2, 0, z0 + H - (k + 0.5) * H * 0.6 / len(bands)), (4, W, H * 0.6 / len(bands) + 1))
    m.box(TEAL, (-2, 0, z0 + H * 0.2), (4, W, H * 0.4))
    m.box(SKY, (-3, 0, z0 + H * 0.3), (4, W, 10))
    m.box(SKY, (-3, 0, z0 + H * 0.15), (4, W * 0.8, 8))
    # the sun: a disc cut by stripes
    m.cyl(YEL, (-4, 0, z0 + H * 0.42), 170, 4, rot=(90, 0, 0), steps=28, glow=0.4)
    for k in range(4):
        m.box(bands[3], (-7, 0, z0 + H * 0.45 + k * 26), (4, 360, 8 - k * 1.5))
    m.box(TEAL, (-6, 0, z0 + H * 0.31), (4, 400, H * 0.12))
    m.box(NAVY, (-8, 380, z0 + H * 0.5), (4, 24, H * 0.6))
    for k in range(5):
        m.box(NAVY, (-8, 380 + (k - 2) * 40, z0 + H * 0.82 - abs(k - 2) * 20), (4, 120, 16), rot=(0, 0, (k - 2) * 25))
    m.box(HOTPINK, (-14, 0, z0 - 6), (6, W + 40, 14), glow=0.9)
    m.box(SKY, (-14, 0, z0 + H + 6), (6, W + 40, 14), glow=0.9)


def ferris(m, R=1500):
    """The pier Ferris wheel, in the YZ plane so it faces the road camera."""
    cz = R + 200
    # the A-frame legs, built as slanted boxes in the YZ plane (roll only)
    for s_ in (-1, 1):
        ang = math.degrees(math.atan2(cz, R * 0.6))
        m.box(WHITE, (40, s_ * R * 0.3, cz / 2), (24, 30, math.hypot(cz, R * 0.6)), rot=(0, 0, -s_ * (90 - ang)))
    n = 16
    for k in range(n):
        a = 2 * math.pi * k / n
        m.box(WHITE, (0, math.cos(a) * R / 2, cz + math.sin(a) * R / 2), (14, R, 14), rot=(0, 0, SPOKE_SIGN * math.degrees(a)))
        m.box([MINT, YEL, SKY, CORAL][k % 4], (-30, math.cos(a) * R, cz + math.sin(a) * R - 150), (150, 150, 150))
    ro = unreal.GeometryScriptRevolveOptions()
    for rr, col in ((R, HOTPINK), (R * 0.55, WHITE)):
        unreal.GeometryScript_Primitives.append_torus(m._mesh(col, 0.3 if col == HOTPINK else 0.0), m.opts, m._t((0, 0, cz), (90, 0, 0)), ro, rr, 24, 32, 6, unreal.GeometryScriptPrimitiveOriginMode.CENTER)
    m.cyl(YEL, (-60, 0, cz), 90, 120, rot=(90, 0, 0), steps=16)


SPOKE_SIGN = -1


def lighthouse(m):
    for k in range(6):
        m.cyl(RED if k % 2 == 0 else WHITE, (0, 0, k * 300), 300 - k * 22, 300, steps=16, top_r=300 - (k + 1) * 22)
    m.cyl(NAVY, (0, 0, 1800), 200, 40, steps=16)
    m.cyl(YEL, (0, 0, 1840), 130, 220, steps=12, glow=0.8)
    m.cyl(RED, (0, 0, 2060), 170, 160, steps=12, top_r=10)


def skyline(m, width=24000, seed=7):
    """A long, tileable strip of far buildings behind the beach. Front at x=0, extends to +X."""
    import random
    r = random.Random(seed)
    cols = [PINK, MINT, LAV, PEACH, CREAM, SKY, CORAL, '#ffd6a5', '#c9f2e3', '#e7d1ff']
    y = -width / 2 + 200
    while y < width / 2 - 1400:
        w = r.choice([600, 800, 1000, 1200])
        h = r.choice([1200, 1800, 2400, 3000, 3600, 4400, 5600])
        col = r.choice(cols)
        x = r.choice([0, 600, 1400, 2400])
        m.box(col, (x + 500, y + w / 2, h / 2), (1000, w, h))
        for f in range(int(h / 360)):
            m.box(WHITE if f % 2 else GLASS2, (x - 2, y + w / 2, 240 + f * 360), (4, w * 0.8, 70))
        if r.random() < 0.4:
            m.box(col, (x + 500, y + w / 2, h + 200), (600, w * 0.5, 400))
            m.cyl(WHITE, (x + 500, y + w / 2, h + 400), 12, 600, steps=6)
        if r.random() < 0.3:
            m.box(HOTPINK, (x - 4, y + w / 2, h - 120), (4, w, 40), glow=0.6)
        y += w + r.choice([0, 100, 300])
