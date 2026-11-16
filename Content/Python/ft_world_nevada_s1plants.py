"""Nevada, Stage 1, builder s1plants: what grows along Highway 93, district by district.
/Game/World/Nevada/SM_S1PLANTS_*.   Run: import ft_world_nevada_s1plants as K; K.run()

Twenty-one plant pieces, every one an ft_people.Scene (one section, M_People), every one stirring in the wind (crowns
rock about the trunk's head, fronds, wands and plumes nod each on its own beat, flowers twist): the wash's cottonwood
and desert willow, Mojave yucca, cholla, barrel cactus and beavertail, creosote, rabbitbrush, mesquite and ocotillo
for the scrap country, century plants at the stops, saltbush, tamarisk and bunchgrass on the salt, pinyon for the
mining camp, a juniper and a bristlecone snag on a column of the cut's own rock for the rim, and for the two neon
approaches fan palms in their skirts, date palms in a planter, a clipped cloud hedge in bloom and juniper flames.
Sage and Joshua trees are the kit's own and are not made again. Nothing glows. Greens are the kit's two sage greens
and its teal; flowers magenta, yellow, cream and purple (never the pickups' orange, never a green box).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'
LOWG = dict(grow=(25, 25, 12), grow_down=(25, 25, 0))      # a low piece: its parts stir a hand's width
TREE = dict(grow=(60, 60, 40), grow_down=(60, 60, 0))

G, GD, TEAL = '#8fb86a', '#5f8f4a', '#2fc6c0'
MAG, YEL, CREAM, PURP, RED = '#ff4fb8', '#ffe14d', '#fff6ea', '#7b3bd6', '#ff3355'
BARK, BARK_D, GREY, SAND, DRYG = '#8a6a4a', '#6a4c36', '#c9c4d6', '#f2c58a', '#d8b060'
RK, RK_D, RK_L, RK_R, TALUS, MID = '#e0784a', '#b8553a', '#f59a62', '#c9603e', '#a04a34', '#c9805a'


# ------------------------------------------------------------------------------------------ helpers
def pol(base, yaw, pitch, length):
    """The point `length` from base along yaw (degrees in the ground plane) and pitch (degrees above level)."""
    a, p = math.radians(yaw), math.radians(pitch)
    return (base[0] + math.cos(a) * math.cos(p) * length, base[1] + math.sin(a) * math.cos(p) * length, base[2] + math.sin(p) * length)


def blade(m, col, base, yaw, pitch, length, w, t=6.0):
    """A flat leaf from base outwards; returns its tip."""
    tip = pol(base, yaw, pitch, length)
    tilt_box(m, col, base, tip, w, t)
    return tip


def limb(m, col, a, b, r0, r1, steps=6):
    """A tapering limb from a to b (rod's own turning, with a narrower end)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, top_r=max(r1, 2.0))


def mv(sc, swing, pivot, axis, rate, phase):
    """The model for parts that rock `swing` degrees about pivot."""
    return sc.part(P.motion(swing=swing, pivot=pivot, axis=axis, rate=rate, phase=phase))


def ring(r, cx, cy, rx, ry, n, rough=0.12, turn=0.0):
    """A lumpy outline of n points, counter-clockwise."""
    out = []
    for k in range(n):
        a = math.radians(turn + k * 360.0 / n)
        f = 1.0 + r.uniform(-rough, rough)
        out.append((cx + math.cos(a) * rx * f, cy + math.sin(a) * ry * f))
    return out


# ------------------------------------------------------------------------------------------ the open desert
def yucca(sc):
    """Mojave yucca: three shaggy trunks, each a head of stiff blades; the tallest carries a cream flower spike."""
    r = random.Random(11)
    g = sc.still()
    heads = ((-75, 25, 150, 1.0, -12), (60, -30, 225, 1.15, 10), (15, 85, 95, 0.8, 4))
    for i, (x, y, h, s, lean) in enumerate(heads):
        limb(g, BARK_D, (x - lean, y, -90), (x, y, h), 21 * s, 15 * s, 6)
        g.cyl(SAND if i % 2 else DRYG, (x, y, h - 78 * s), 36 * s, 74 * s, steps=6, top_r=19 * s)      # the skirt of dead blades
        c = mv(sc, 2.2, (x, y, h - 20), r.uniform(0, 180), 0.3, r.random())
        for k in range(12):
            blade(c, G if k % 2 else GD, (x, y, h), k * 30 + r.uniform(-8, 8), (-12, 24, 56)[k % 3] + r.uniform(-6, 6), 96 * s, 14 * s, 7)
        if i < 2:
            top = h + (150 if i else 105)
            f = mv(sc, 4.0, (x, y, h), 90 + i * 50, 0.35, r.random())
            limb(f, G, (x, y, h), (x + 4, y, top - 60), 8, 5, 5)
            for k in range(5 if i else 3):
                f.sphere(CREAM, (x + 4, y, top - 62 + k * 24), 33 - k * 5, scale=(1, 1, 0.8), steps=(3, 6))
    g.sphere(RK_D, (150, 40, 4), 38, scale=(1.2, 1, 0.6), steps=(3, 6))


def cholla(sc):
    """A cholla garden: three jointed chollas, the youngest joints pale with spines, yellow fruit."""
    r = random.Random(12)
    g = sc.still()
    for i, (x, y, H, arms) in enumerate(((-95, 10, 235, 4), (45, -45, 185, 3), (120, 65, 140, 3))):
        rod(g, BARK_D, (x, y, -70), (x, y, H * 0.36), 14, steps=6)
        top = mv(sc, 2.0, (x, y, H * 0.36), r.uniform(0, 180), 0.32, r.random())
        rod(top, GD, (x, y, H * 0.36), (x + 5, y, H * 0.7), 13, steps=6)
        rod(top, G, (x + 5, y, H * 0.7), (x, y, H * 0.93), 12, steps=6)
        top.sphere(CREAM, (x, y, H * 0.95), 19, steps=(3, 6))
        for k in range(arms):
            a0 = k * 360.0 / arms + r.uniform(-25, 25) + i * 40
            z0 = H * (0.38 + 0.13 * k)
            el = pol((x, y, z0), a0, 22, 46 + r.uniform(0, 14))
            tip = (el[0] + r.uniform(-6, 6), el[1] + r.uniform(-6, 6), el[2] + 44 + r.uniform(0, 20))
            m = mv(sc, 3.2, (x, y, z0), a0 + 90, 0.4, r.random())
            rod(m, GD, (x, y, z0), el, 11, steps=6)
            rod(m, G, el, tip, 11, steps=6)
            m.sphere(CREAM, (tip[0], tip[1], tip[2] + 4), 17, steps=(3, 6))
            if k % 2 == 0:
                m.box(YEL, (el[0], el[1], el[2] - 15), (11, 11, 14))
    g.sphere(RK_D, (-10, 95, 2), 34, scale=(1.3, 1, 0.55), steps=(3, 6))


def barrel(sc):
    """Barrel cacti, five of them, ribbed, each crowned with a ring of pink flowers that twists in the wind."""
    r = random.Random(13)
    g = sc.still()
    for i, (x, y, rr) in enumerate(((-110, 10, 44), (-20, -50, 34), (70, 30, 48), (150, -30, 30), (10, 80, 26))):
        cz = rr * 1.02 - 6
        g.sphere(GD, (x, y, cz), rr, scale=(1, 1, 1.22), steps=(5, 8))
        for k in range(4):
            g.box(G, (x, y, cz), (rr * 2.08, 7, rr * 2.0), rot=(0, k * 45 + i * 11, 0))
        top = cz + rr * 1.22
        c = mv(sc, 12.0, (x, y, top), P.UP, 0.25 + 0.03 * i, r.random())
        for k in range(5):
            blade(c, MAG, (x, y, top - 5), k * 72 + i * 20, 24, rr * 0.5, rr * 0.3, 6)
        c.sphere(YEL, (x, y, top + 1), rr * 0.2, steps=(3, 5))
    g.sphere(RK_D, (-40, 60, 0), 30, scale=(1.3, 1, 0.5), steps=(3, 6))
    g.sphere(RK, (110, 70, 0), 24, scale=(1.2, 1, 0.5), steps=(3, 6))


def beavertail(sc):
    """A beavertail prickly pear mound: flat pads on flat pads, magenta flowers along their top edges."""
    r = random.Random(14)
    g = sc.still()
    n = 0
    for i in range(6):
        a = i * 60 + r.uniform(-15, 15)
        d = r.uniform(40, 105)
        x, y = math.cos(math.radians(a)) * d * 1.3, math.sin(math.radians(a)) * d * 0.8
        rr = r.uniform(27, 34)
        yaw = r.uniform(0, 180)
        g.sphere(GD if i % 2 else G, (x, y, rr * 1.0), rr, scale=(1, 0.24, 1.25), steps=(4, 6), rot=(r.uniform(-10, 10), yaw, 0))
        jz = rr * 2.0
        m = mv(sc, 4.0, (x, y, jz), yaw, 0.35, r.random())
        for k in range(2 if i % 3 else 1):
            r2 = rr * r.uniform(0.75, 0.92)
            ox = (k * 2 - 1) * 16 if i % 3 else 0
            px, py = x + math.cos(math.radians(yaw)) * ox, y + math.sin(math.radians(yaw)) * ox
            m.sphere(G if (i + k) % 2 else GD, (px, py, jz + r2 * 0.95), r2, scale=(1, 0.24, 1.2), steps=(4, 6), rot=(0, yaw + r.uniform(-30, 30), r.uniform(-14, 14)))
            if n < 9:
                n += 1
                fz = jz + r2 * 2.1
                m.box(MAG, (px, py, fz), (17, 17, 12))
                m.box(YEL, (px, py, fz + 7), (7, 7, 6))
    g.sphere(RK_D, (0, 0, -2), 46, scale=(1.5, 1.1, 0.4), steps=(3, 6))


def creosote(sc):
    """A creosote bush: a fan of thin dark stems, small olive leaf clumps, yellow flecks."""
    r = random.Random(15)
    g = sc.still()
    g.sphere(MID, (0, 0, -6), 60, scale=(1.3, 1.1, 0.3), steps=(3, 6))
    parts = [mv(sc, 3.0, (0, 0, 0), k * 36 + 10, 0.36 + 0.03 * k, r.random()) for k in range(5)]
    for k in range(14):
        m = parts[k % 5]
        yaw = k * 360.0 / 14 + r.uniform(-10, 10)
        tip = pol((0, 0, -10), yaw, r.uniform(52, 80), r.uniform(170, 262))
        limb(m, BARK_D, (0, 0, -10), tip, 6.5, 4, 4)
        mid = ((tip[0]) * 0.62, tip[1] * 0.62, tip[2] * 0.62)
        m.sphere(GD, mid, r.uniform(26, 34), scale=(1, 1, 0.8), steps=(3, 5))
        m.sphere(G if k % 2 else GD, tip, r.uniform(24, 32), scale=(1, 1, 0.85), steps=(3, 5))
        m.box(YEL, (tip[0] + r.uniform(-14, 14), tip[1] + r.uniform(-14, 14), tip[2] + 20), (9, 9, 9))
        if k % 2:
            m.box(YEL, (mid[0] + r.uniform(-18, 18), mid[1] - 20, mid[2] + 14), (8, 8, 8))


def rabbitbrush(sc):
    """Rabbitbrush: three green domes capped in yellow bloom."""
    r = random.Random(16)
    g = sc.still()
    for i, (x, y, rr) in enumerate(((-85, 5, 70), (55, -40, 84), (40, 75, 58))):
        g.sphere(GD, (x, y, rr * 0.4), rr, scale=(1, 1, 0.72), steps=(5, 8))
        c = mv(sc, 3.0, (x, y, 0), r.uniform(0, 180), 0.4, r.random())
        c.sphere(YEL, (x, y, rr * 0.72), rr * 0.9, scale=(1, 1, 0.5), steps=(4, 8))
        for k in range(7):
            blade(c, YEL if k % 2 else G, (x, y, rr * 0.7), k * 51 + i * 17, r.uniform(28, 62), rr * 1.08, 12, 8)
    g.sphere(RK_D, (-10, -90, 0), 30, scale=(1.3, 1, 0.5), steps=(3, 6))


def ocotillo(sc):
    """An ocotillo: twelve long wands from one root, each leafed green and tipped red, each swaying on its own."""
    r = random.Random(17)
    g = sc.still()
    g.sphere(RK_D, (0, 0, -4), 48, scale=(1.2, 1.1, 0.4), steps=(3, 6))
    for k in range(12):
        yaw = k * 30 + r.uniform(-9, 9)
        splay = r.uniform(9, 27)
        p1 = pol((0, 0, -10), yaw, 90 - splay, r.uniform(200, 250))
        p2 = pol(p1, yaw + r.uniform(-12, 12), 90 - splay * 1.5, r.uniform(180, 250))
        m = mv(sc, r.uniform(2.6, 4.6), (0, 0, 0), yaw + 90, r.uniform(0.3, 0.5), r.random())
        limb(m, GD, (0, 0, -10), p1, 9, 7, 5)
        limb(m, GD if k % 2 else G, p1, p2, 7, 5, 5)
        for u in (0.45, 0.85):
            q = (p1[0] * u, p1[1] * u, p1[2] * u)
            m.box(G, (q[0], q[1], q[2]), (20, 20, 26), rot=(0, yaw, 0))
        limb(m, RED, p2, pol(p2, yaw, 90 - splay * 1.6, 46), 10, 3, 5)


def mesquite(sc):
    """A mesquite: a gnarled twin trunk under a flat-topped canopy, seed pods hanging and swinging."""
    r = random.Random(18)
    g = sc.still()
    limb(g, BARK_D, (0, 0, -130), (-40, 0, 150), 32, 25, 6)
    limb(g, BARK_D, (-40, 0, 150), (-160, 20, 335), 22, 15, 6)
    limb(g, BARK, (-40, 0, 150), (90, -10, 330), 23, 16, 6)
    limb(g, BARK, (90, -10, 330), (215, 30, 395), 15, 10, 5)
    limb(g, BARK_D, (-40, 0, 150), (-30, 110, 400), 14, 9, 5)
    pads = ((-195, 20, 395, 175), (40, -30, 425, 195), (235, 40, 405, 150), (-40, 125, 450, 140), (115, -140, 395, 120))
    for i, (x, y, z, rr) in enumerate(pads):
        m = mv(sc, 2.0, (x, y, z - 60), r.uniform(0, 180), 0.3, r.random())
        m.sphere(GD, (x, y, z), rr, scale=(1, 1, 0.3), steps=(5, 8))
        m.sphere(G, (x + 10, y - 8, z + rr * 0.2), rr * 0.74, scale=(1, 1, 0.26), steps=(4, 8))
        p = mv(sc, 11.0, (x, y, z - rr * 0.2), r.uniform(0, 180), 0.8, r.random())
        for k in range(3):
            a = math.radians(k * 120 + i * 40)
            p.box(DRYG, (x + math.cos(a) * rr * 0.55, y + math.sin(a) * rr * 0.55, z - rr * 0.3 - 22), (7, 7, 46))


# ------------------------------------------------------------------------------------------ the wash and the stops
def cottonwood(sc):
    """A cottonwood: a stout forked trunk and a broad crown of seven leaf masses, each rocking on its own limb; gold
    leaves flutter at the crown's edge."""
    r = random.Random(21)
    g = sc.still()
    g.cyl(BARK_D, (0, 0, -160), 72, 180, steps=8, top_r=46)
    limb(g, BARK, (0, 0, 0), (20, 0, 335), 46, 35, 8)
    ends = ((-150, 30, 615), (170, -20, 645), (30, 60, 720))
    limb(g, BARK, (20, 0, 335), ends[0], 30, 17, 6)
    limb(g, BARK_D, (20, 0, 335), ends[1], 30, 17, 6)
    limb(g, BARK, (20, 0, 335), ends[2], 26, 14, 6)
    blobs = ((-195, 30, 705, 195, 0), (185, -20, 735, 205, 1), (30, 75, 865, 225, 2), (-45, -95, 765, 170, 0), (115, 115, 650, 150, 1),
             (-120, 130, 905, 150, 2), (60, -40, 1005, 150, 2))
    for i, (x, y, z, rr, e) in enumerate(blobs):
        m = mv(sc, 2.6, ends[e], r.uniform(0, 180), 0.28 + 0.02 * (i % 3), r.random())
        m.sphere(G if i % 2 else GD, (x, y, z), rr, scale=(1, 1, 0.8), steps=(5, 8))
    for k in range(4):
        a = k * 90 + 30
        c = pol((0, 0, 760), a, 0, 300)
        f = mv(sc, 14.0, (c[0], c[1], c[2] + 60), a, 1.1, r.random())
        for j in range(4):
            f.box(DRYG if j % 2 else G, (c[0] + r.uniform(-90, 90), c[1] + r.uniform(-90, 90), c[2] + r.uniform(-120, 110)), (30, 7, 38), rot=(0, r.uniform(0, 180), 0))


def desert_willow(sc):
    """A desert willow in blossom: three leaning stems, long drooping leaves, pink trumpet flowers with cream throats."""
    r = random.Random(22)
    g = sc.still()
    stems = (((-110, 20, 265), (-205, 40, 475)), ((90, -30, 300), (175, -50, 525)), ((10, 70, 345), (25, 115, 590)), ((-20, -10, 380), (-10, -20, 640)))
    for i, (a, b) in enumerate(stems):
        limb(g, BARK_D, (0, 0, -130), a, 17, 11, 6)
        m = mv(sc, 3.4, a, r.uniform(0, 180), 0.33 + 0.03 * i, r.random())
        limb(m, BARK, a, b, 11, 6, 5)
        for k in range(7):
            blade(m, G if k % 2 else GD, (b[0], b[1], b[2] - 10), k * 51 + i * 23, (38, 8, -28, -52)[k % 4] + r.uniform(-8, 8), r.uniform(130, 170), 17, 6)
        for k in range(4):
            q = pol(b, k * 90 + i * 31 + r.uniform(-20, 20), r.uniform(-10, 40), r.uniform(60, 110))
            m.sphere(MAG, q, 24, steps=(3, 5))
            m.box(CREAM, (q[0], q[1], q[2] - 16), (13, 13, 12))


def century(sc):
    """A century plant: a rosette of thick teal blades and its one great flower stalk, yellow platters up the mast."""
    r = random.Random(23)
    g = sc.still()
    g.sphere(GD, (0, 0, 30), 44, scale=(1, 1, 1.1), steps=(4, 6))
    for k in range(14):
        yaw = k * 360.0 / 14 + r.uniform(-7, 7)
        p0 = (0, 0, 22)
        up = (62, 44, 28)[k % 3] + r.uniform(-5, 5)
        p1 = blade(g, TEAL if k % 2 else GD, p0, yaw, up, 118, 40, 15)
        p2 = blade(g, TEAL if k % 2 else GD, p1, yaw, up - 30, 92, 26, 11)
        g.box(CREAM, p2, (9, 9, 9))
    s = mv(sc, 2.2, (0, 0, 110), 0.0, 0.25, 0.2)
    limb(s, G, (0, 0, 90), (12, 0, 610), 15, 8, 6)
    for k in range(8):
        z = 330 + k * 37
        yaw = k * 137.5
        q = pol((6 + k * 0.7, 0, z), yaw, 22, 96 - k * 7)
        tilt_box(s, G, (6 + k * 0.7, 0, z), q, 8, 8)
        s.sphere(YEL, (q[0], q[1], q[2] + 8), 40 - k * 2, scale=(1, 1, 0.36), steps=(4, 6))
    s.sphere(YEL, (12, 0, 622), 26, scale=(1, 1, 0.7), steps=(4, 6))


def pinyon(sc):
    """A pinyon pine: a short stout trunk and a round crown in four boughs, each nodding on its own beat; brown cones."""
    r = random.Random(24)
    g = sc.still()
    limb(g, BARK_D, (0, 0, -160), (15, 0, 280), 36, 24, 7)
    tiers = ((185, 275, 200, 150), (320, 235, 150, 150), (455, 180, 100, 135), (575, 115, 26, 130))
    for i, (z0, r0, r1, h) in enumerate(tiers):
        ox, oy = 15 + r.uniform(-22, 22), r.uniform(-22, 22)
        m = mv(sc, 1.6 + 0.4 * i, (15, 0, z0), 40 + i * 50, 0.3, 0.17 * i)
        m.cyl(GD if i % 2 == 0 else G, (ox, oy, z0), r0, h, steps=8, top_r=r1, rot=(0, i * 22, 0))
        n = (4, 3, 2, 0)[i]
        for k in range(n):
            a = math.radians(k * 360.0 / max(n, 1) + i * 37)
            m.sphere(G if i % 2 == 0 else GD, (ox + math.cos(a) * r0 * 0.92, oy + math.sin(a) * r0 * 0.92, z0 + 30), 62, scale=(1, 1, 0.7), steps=(3, 6))
            m.box(BARK, (ox + math.cos(a + 0.5) * r0 * 0.8, oy + math.sin(a + 0.5) * r0 * 0.8, z0 + h * 0.6), (15, 15, 22))


def rim_pines(sc):
    """For the rock cuts: a juniper and a bleached bristlecone snag on a 19 m column of the cut's own beds (the cut is
    15 m: the trees stand over its rim against the sky)."""
    r = random.Random(25)
    g = sc.still()
    H = 1900.0
    tones = (RK, RK_D, RK_L, RK_R)
    k = 9
    zs = [-260.0] + [H * (i + 1.0) / k for i in range(k)]
    for i in range(k):
        f = 1.0 - 0.16 * (i / (k - 1.0))
        g.plan(SAND if i == k - 1 else tones[i % 4], ring(r, 0, 0, 430 * f, 265 * f, 9, 0.1, i * 13), zs[i], zs[i + 1])
    g.sphere(RK_L, (-20, -150, H + 12), 52, scale=(1.3, 1, 0.6), steps=(3, 6))
    # the juniper: a twisted trunk, a wind-swept crown
    limb(g, BARK, (-190, 10, H - 30), (-235, 20, H + 175), 32, 23, 6)
    limb(g, BARK_D, (-235, 20, H + 175), (-160, -15, H + 335), 22, 14, 6)
    j = mv(sc, 2.4, (-160, -15, H + 335), 0.0, 0.3, 0.1)
    for i, (x, y, z, rr) in enumerate(((-150, -10, H + 400, 150), (-30, -20, H + 470, 130), (-250, 30, H + 500, 115), (-110, 10, H + 590, 105))):
        j.sphere(GD if i % 2 == 0 else G, (x, y, z), rr, scale=(1.15, 1, 0.72), steps=(5, 8))
    for i in range(4):
        j.box(TEAL, (-150 + i * 62 - 60, -105 - (i % 2) * 18, H + 400 + i * 22), (15, 15, 15))
    # the bristlecone snag: bare, bleached, one live tuft
    s = mv(sc, 1.2, (240, 10, H), 90.0, 0.25, 0.6)
    limb(s, GREY, (235, 15, H - 30), (262, 0, H + 225), 38, 22, 6)
    limb(s, CREAM, (262, 0, H + 225), (200, -30, H + 440), 21, 10, 6)
    limb(s, GREY, (200, -30, H + 440), (222, -38, H + 560), 9, 3, 5)
    limb(s, GREY, (255, 0, H + 200), (392, 12, H + 335), 13, 5, 5)
    limb(s, CREAM, (240, -10, H + 300), (140, 45, H + 395), 11, 4, 5)
    t = mv(sc, 4.0, (392, 12, H + 335), 0.0, 0.4, 0.3)
    t.sphere(GD, (400, 12, H + 350), 72, scale=(1.2, 1, 0.7), steps=(4, 6))
    t.sphere(G, (360, 25, H + 395), 46, scale=(1, 1, 0.7), steps=(3, 6))
    for i, (x, y) in enumerate(((60, -120), (120, 110), (-330, -60))):
        q = mv(sc, 8.0, (x, y, H), i * 60.0, 0.55, 0.3 * i)
        for k2 in range(4):
            blade(q, DRYG if k2 % 2 else SAND, (x, y, H - 6), k2 * 90 + i * 30, 66, 62, 9, 6)


# ------------------------------------------------------------------------------------------ the salt flats
def saltbush(sc):
    """Saltbush and pickleweed on a plate of pale salt crust: silvery hummocks, pink-tipped fingers."""
    r = random.Random(31)
    g = sc.still()
    g.plan(CREAM, ring(r, 0, 0, 250, 175, 9, 0.14), -30, 7)
    g.plan(GREY, ring(r, 40, -20, 150, 95, 7, 0.14, 20), 7, 12)
    for i, (x, y, rr) in enumerate(((-120, 20, 74), (30, 60, 60), (110, -40, 82), (-30, -80, 50))):
        g.sphere(G, (x, y, rr * 0.4), rr, scale=(1, 1, 0.68), steps=(5, 8))
        c = mv(sc, 3.0, (x, y, 0), r.uniform(0, 180), 0.38, r.random())
        c.sphere(GREY, (x + 6, y - 6, rr * 0.72), rr * 0.82, scale=(1, 1, 0.5), steps=(4, 8))
    fing = [mv(sc, 6.0, (190 - k * 330, 80 - k * 150, 0), 30 + k * 70, 0.5, 0.4 * k) for k in range(2)]
    for k in range(10):
        m = fing[k % 2]
        x = (190 if k % 2 == 0 else -140) + r.uniform(-55, 55)
        y = (80 if k % 2 == 0 else -70) + r.uniform(-40, 40)
        h = r.uniform(38, 62)
        limb(m, GD, (x, y, 4), (x + r.uniform(-8, 8), y, 4 + h), 8, 6, 5)
        m.box(MAG, (x, y, 8 + h), (11, 11, 12))


def tamarisk(sc):
    """A tamarisk: dark arching stems, feathery green sprays, pink plumes that nod."""
    r = random.Random(32)
    g = sc.still()
    g.cyl(BARK_D, (0, 0, -140), 36, 170, steps=6, top_r=24)
    stems = ((-120, 30, 270), (110, -20, 300), (10, 100, 330), (-10, -90, 250))
    for i, a in enumerate(stems):
        limb(g, BARK_D, (0, 0, 10), a, 14, 9, 5)
        for k in range(3 if i < 2 else 2):
            yaw = math.degrees(math.atan2(a[1], a[0])) + (k - 1) * 55 + r.uniform(-12, 12)
            m = mv(sc, 5.0, a, yaw + 90, 0.38 + 0.04 * k, r.random())
            b = pol(a, yaw, r.uniform(38, 66), r.uniform(150, 195))
            limb(m, G if (i + k) % 2 else GD, a, b, 31, 9, 5)
            c = pol(b, yaw, r.uniform(-30, 5), r.uniform(80, 105))
            limb(m, MAG, b, c, 20, 4, 5)
            for sd in (-1, 1):
                blade(m, GD if (i + k) % 2 else G, ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0), yaw + sd * 62, 22, 88, 20, 7)


def bunchgrass(sc):
    """Bunchgrass in a drift of blown sand: seven tufts, each leaning to the wind on its own beat."""
    r = random.Random(33)
    g = sc.still()
    g.sphere(SAND, (0, 0, -12), 330, scale=(1, 0.36, 0.1), steps=(4, 8))
    cols = (DRYG, SAND, G, DRYG, CREAM, GD, DRYG)
    for i in range(7):
        x = -300 + i * 100 + r.uniform(-25, 25)
        y = r.uniform(-80, 80)
        s = r.uniform(0.8, 1.25)
        m = mv(sc, 8.0, (x, y, 0), 90 + r.uniform(-30, 30), 0.55 + r.uniform(-0.08, 0.08), r.random())
        for k in range(7):
            blade(m, cols[(i + k) % 7], (x, y, -12), k * 51 + r.uniform(-12, 12), r.uniform(56, 84), s * r.uniform(72, 112), 10, 6)
        tip = blade(m, DRYG, (x, y, -12), r.uniform(0, 360), 82, s * 108, 6, 6)
        m.box(CREAM, tip, (10, 10, 22))


def wildflowers(sc):
    """A wildflower carpet: gold and purple mats on the flat, cushions of leaf, flower heads nodding over them."""
    r = random.Random(34)
    g = sc.still()
    g.plan(YEL, ring(r, -150, 10, 230, 130, 8, 0.16), -25, 6)
    g.plan(PURP, ring(r, 170, -10, 210, 120, 8, 0.16, 15), -25, 8)
    g.plan(CREAM, ring(r, 20, 40, 90, 60, 6, 0.16), 8, 12)
    for i in range(9):
        x = -310 + i * 78 + r.uniform(-20, 20)
        y = r.uniform(-95, 95)
        rr = r.uniform(38, 56)
        col = (YEL, PURP, YEL, PURP, CREAM, PURP, YEL, MAG, PURP)[i]
        g.sphere(GD if i % 2 else G, (x, y, rr * 0.2), rr, scale=(1, 1, 0.42), steps=(4, 6))
        m = mv(sc, 7.0, (x, y, 0), r.uniform(0, 180), 0.6 + r.uniform(-0.1, 0.1), r.random())
        for k in range(4):
            tip = blade(m, G, (x, y, rr * 0.3), k * 90 + r.uniform(-25, 25), r.uniform(62, 84), r.uniform(42, 70), 6, 6)
            m.box(col, tip, (18, 18, 11))


# ------------------------------------------------------------------------------------------ the neon approaches
def fan_palm(sc):
    """A desert fan palm, 12 m: a skirt of dead fronds under the crown, twelve fans each nodding on its own stalk."""
    r = random.Random(41)
    g = sc.still()
    g.cyl(BARK_D, (0, 0, -160), 52, 190, steps=8, top_r=36)
    limb(g, BARK, (0, 0, 20), (26, 0, 480), 34, 29, 8)
    limb(g, BARK, (26, 0, 480), (42, 0, 800), 29, 25, 8)
    g.cyl(DRYG, (38, 0, 690), 52, 170, steps=8, top_r=98)
    g.cyl(SAND, (41, 0, 860), 98, 165, steps=8, top_r=64)
    C = (42, 0, 1035)
    g.sphere(GD, C, 56, steps=(4, 6))
    for k in range(12):
        yaw = k * 30 + r.uniform(-8, 8)
        pitch = (58, -8, 30, -26)[k % 4] + r.uniform(-6, 6)
        m = mv(sc, 5.0, C, yaw + 90, 0.32 + 0.02 * (k % 4), r.random())
        e = blade(m, GD, C, yaw, pitch, 135, 11, 9)
        for d in (-27, 0, 27):
            blade(m, G if k % 2 else GD, e, yaw + d, pitch - 10, 128 if d == 0 else 112, 50, 6)


def date_palms(sc):
    """A pair of date palms in a cream planter with a teal band: ringed trunks, arching feather fronds, date clusters."""
    r = random.Random(42)
    g = sc.still()
    g.box(CREAM, (0, 0, 5), (540, 230, 110))
    g.box(TEAL, (0, 0, 66), (556, 246, 16))
    g.box(BARK_D, (0, 0, 72), (500, 190, 10))
    for k in range(4):
        fm = mv(sc, 6.0, (-150 + k * 100, -70, 76), 0.0, 0.5, 0.25 * k)
        fm.sphere(GD, (-150 + k * 100, -70, 88), 30, scale=(1, 1, 0.6), steps=(3, 6))
        fm.box(MAG if k % 2 else CREAM, (-150 + k * 100, -70, 112), (20, 20, 12))
    for sd in (-1, 1):
        x0 = sd * 150
        p = (x0, 10, 70)
        for i in range(4):
            q = (x0 + sd * (i + 1) * 9, 10, 70 + (i + 1) * (205 if sd < 0 else 185))
            limb(g, BARK if i % 2 else BARK_D, p, q, 32 - i * 2, 27 - i * 2, 8)
            p = q
        C = (p[0], p[1], p[2] + 20)
        g.sphere(BARK_D, C, 46, scale=(1, 1, 1.15), steps=(4, 6))
        for k in range(9):
            yaw = k * 40 + r.uniform(-8, 8)
            up = (56, 30, 8)[k % 3] + r.uniform(-5, 5)
            m = mv(sc, 4.5, C, yaw + 90, 0.3 + 0.02 * (k % 3), r.random())
            e = blade(m, GD, C, yaw, up, 195, 36, 7)
            blade(m, G, e, yaw, up - 50, 170, 26, 6)
        for k in range(3):
            d = mv(sc, 8.0, C, k * 120.0, 0.6, r.random())
            q = pol(C, k * 120 + 30, -40, 70)
            tilt_box(d, DRYG, C, q, 6, 6)
            d.sphere(DRYG, (q[0], q[1], q[2] - 16), 25, scale=(1, 1, 1.2), steps=(3, 6))


def bloom_hedge(sc):
    """A clipped cloud hedge in bloom on a cream kerb: six rounded bays, magenta and cream blossom that stirs."""
    r = random.Random(43)
    g = sc.still()
    g.box(CREAM, (0, 0, -8), (720, 170, 44))
    g.box(TEAL, (0, -86, 6), (720, 6, 8))
    for i in range(6):
        x = -290 + i * 116
        rr = 80 if i % 2 else 70
        g.sphere(GD if i % 2 else G, (x, 0, 70), rr, scale=(1, 0.86, 1.12 if i % 2 else 1.0), steps=(5, 8))
        m = mv(sc, 5.0, (x, 0, 60), i * 30.0, 0.45 + 0.03 * i, r.random())
        for k in range(4):
            a, e = math.radians(k * 90 + i * 37 + r.uniform(-20, 20)), math.radians(r.uniform(15, 70))
            m.box((MAG, CREAM, MAG, YEL)[(k + i) % 4],
                  (x + math.cos(a) * math.cos(e) * rr * 0.98, math.sin(a) * math.cos(e) * rr * 0.84, 70 + math.sin(e) * rr * 1.02), (17, 17, 12))
        blade(m, G, (x, 0, 70 + rr * 0.9), i * 60, 78, 36, 8, 6)


def juniper_flames(sc):
    """Three clipped junipers, each a tall green flame on a short trunk, on a cream kerb with yellow flowers between."""
    r = random.Random(44)
    g = sc.still()
    g.box(CREAM, (0, 0, -8), (680, 160, 44))
    g.box(BARK_D, (0, 0, 16), (640, 124, 8))
    for i, (x, s) in enumerate(((-230, 0.86), (0, 1.0), (230, 0.9))):
        g.cyl(BARK, (x, 0, 10), 13, 60 * s, steps=6)
        g.cyl(GD, (x, 0, 55 * s), 66 * s, 170 * s, steps=8, top_r=86 * s)
        m = mv(sc, 1.8, (x, 0, 55 * s), 40 + i * 50.0, 0.3, 0.3 * i)
        m.cyl(G if i % 2 else GD, (x, 0, 225 * s), 86 * s, 160 * s, steps=8, top_r=56 * s, rot=(0, 22, 0))
        m.cyl(GD if i % 2 else G, (x, 0, 385 * s), 56 * s, 122 * s, steps=8, top_r=5)
        m.box(G, (x - 80 * s, -20, 300 * s), (16, 16, 40))
    for k in range(4):
        x = (-115, 115, -330, 330)[k]
        f = mv(sc, 6.0, (x, -40, 20), 0.0, 0.5, 0.25 * k)
        f.sphere(G, (x, -40, 30), 30, scale=(1, 1, 0.6), steps=(3, 6))
        f.box(YEL if k % 2 else MAG, (x, -40, 54), (20, 20, 12))


JOBS = [('SM_S1PLANTS_Yucca', yucca, TREE), ('SM_S1PLANTS_Cholla', cholla, TREE), ('SM_S1PLANTS_Barrel', barrel, LOWG),
        ('SM_S1PLANTS_Beavertail', beavertail, LOWG), ('SM_S1PLANTS_Creosote', creosote, TREE), ('SM_S1PLANTS_Rabbitbrush', rabbitbrush, LOWG),
        ('SM_S1PLANTS_Ocotillo', ocotillo, TREE), ('SM_S1PLANTS_Mesquite', mesquite, TREE), ('SM_S1PLANTS_Cottonwood', cottonwood, TREE),
        ('SM_S1PLANTS_DesertWillow', desert_willow, TREE), ('SM_S1PLANTS_Century', century, TREE), ('SM_S1PLANTS_Pinyon', pinyon, TREE),
        ('SM_S1PLANTS_RimPines', rim_pines, TREE), ('SM_S1PLANTS_Saltbush', saltbush, LOWG), ('SM_S1PLANTS_Tamarisk', tamarisk, TREE),
        ('SM_S1PLANTS_Bunchgrass', bunchgrass, LOWG), ('SM_S1PLANTS_Wildflowers', wildflowers, LOWG), ('SM_S1PLANTS_FanPalm', fan_palm, TREE),
        ('SM_S1PLANTS_DatePalms', date_palms, TREE), ('SM_S1PLANTS_BloomHedge', bloom_hedge, LOWG), ('SM_S1PLANTS_JuniperFlames', juniper_flames, TREE)]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
