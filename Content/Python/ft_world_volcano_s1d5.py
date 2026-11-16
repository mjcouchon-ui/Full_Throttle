"""Volcano, Stage 1, builder s1d5: district 5 "The Ash Fields", 13.9 to 16.7 km = WINDMILL FLATS, the ash farmers'
mills. Stubby round mills and barrel-roofed barns with cloth sails and patchwork shutters, in bubblegum pink, sky
blue, white, lemon cream and ash lilac. /Game/World/Volcano/SM_S1D5_*.
Run: import ft_world_volcano_s1d5 as K; K.run()

Every piece is an ft_people.Scene (one material, one section). Sails, fans and vanes turn by the builders' own motion
(a swing about an axle: a wheel goes round one way and comes back, as on the other levels); cloth flaps; nothing is
lit, nothing flies, nothing leaves its piece. No figures, no animals but four white doves sitting on the dovecote.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

PINK, BLUE, WHITE, LEMON, LILAC = '#ff8fb1', '#6fc8ff', '#fbfbf5', '#fff1a8', '#6f6272'   # the district's five
PANE, STRAW, STONE = '#14101a', '#b0a088', '#9a98a8'                                    # the kit's PANE, PALE_L, CONC
PATCH = (LEMON, BLUE, WHITE)


# ------------------------------------------------------------------------------------------ helpers
def ring(g, col, cx, cy, z, r, a, size):
    """A box on a round wall: r from the axis (cx, cy), a degrees round from the front (-Y), turned to face out."""
    ar = math.radians(a)
    g.box(col, (cx + r * math.sin(ar), cy - r * math.cos(ar), z), size, rot=(0, a, 0))


def window(g, x, y, z, w, h, k=0):
    """A window on a flat front (the wall's face at y): pane, white sill, two shutters, a patch sewn on each."""
    sw = w * 0.5
    g.box(PANE, (x, y - 4, z), (w, 10, h))
    g.box(WHITE, (x, y - 8, z - h / 2.0 - 5), (w + 2 * sw + 12, 18, 10))
    for s in (-1, 1):
        i = k + (1 if s > 0 else 0)
        g.box(PATCH[i % 3], (x + s * (w / 2.0 + sw / 2.0 + 2), y - 5, z), (sw, 8, h))
        g.box(PATCH[(i + 1) % 3], (x + s * (w / 2.0 + sw / 2.0 + 2), y - 9, z + h * 0.18), (sw * 0.6, 5, h * 0.3))


def rwindow(g, cx, cy, z, r, a, w, h, k=0):
    """The same on a round wall of radius r at that height: a pane and two shutters."""
    ring(g, PANE, cx, cy, z, r - 6, a, (w, 40, h))
    da = math.degrees(w * 0.78 / r)
    for s in (-1, 1):
        ring(g, PATCH[(k + (1 if s > 0 else 0)) % 3], cx, cy, z, r - 2, a + s * da, (w * 0.5, 40, h))


def sails(m, hub, R, n=4, wid=None, cols=(BLUE, WHITE, LEMON), patches=5, stock=12.0, inner=0.2, lattice=True):
    """Cloth sails in the X-Z plane about hub (facing -Y): a stock, a lattice and a patchwork of cloth down one side
    of each. Drawn as a plus, so the mesh's own box holds the whole turn."""
    hx, hy, hz = hub
    wid = wid or R * 0.2
    r0 = R * inner
    step = (R - r0) / float(patches)
    thin = max(stock * 0.5, 4.5)
    for k in range(n):
        a = math.radians(k * 360.0 / n)
        dx, dz = math.cos(a), math.sin(a)
        nx, nz = -dz, dx
        tilt_box(m, LILAC, (hx, hy, hz), (hx + dx * R, hy, hz + dz * R), stock, stock * 1.2)
        o = wid / 2.0 + stock * 0.6
        for i in range(patches):
            ra, rb = r0 + i * step + step * 0.04, r0 + (i + 1) * step - step * 0.04
            tilt_box(m, cols[(i + k) % len(cols)], (hx + dx * ra + nx * o, hy - stock * 0.3, hz + dz * ra + nz * o),
                     (hx + dx * rb + nx * o, hy - stock * 0.3, hz + dz * rb + nz * o), max(stock * 0.4, 5.0), wid)
        if lattice:
            oo = wid + stock * 0.6
            tilt_box(m, LILAC, (hx + dx * r0 + nx * oo, hy, hz + dz * r0 + nz * oo), (hx + dx * R + nx * oo, hy, hz + dz * R + nz * oo), thin, thin)
            for i in range(patches + 1):
                rr = r0 + i * step
                tilt_box(m, LILAC, (hx + dx * rr, hy, hz + dz * rr), (hx + dx * rr + nx * oo, hy, hz + dz * rr + nz * oo), thin, thin)


def fan(m, hub, R, n=8, cols=(LEMON, BLUE, WHITE), wid=None, thick=5.0):
    """A small many-bladed wind wheel in the X-Z plane about hub."""
    hx, hy, hz = hub
    wid = wid or R * 0.34
    for k in range(n):
        a = math.radians(k * 360.0 / n)
        dx, dz = math.cos(a), math.sin(a)
        tilt_box(m, cols[k % len(cols)], (hx + dx * R * 0.22, hy, hz + dz * R * 0.22), (hx + dx * R, hy, hz + dz * R), thick, wid)
    rod(m, WHITE, (hx, hy - thick, hz), (hx, hy + thick, hz), R * 0.2, steps=6)


def sack(g, x, y, z, r=32.0, tall=1.0, tie=True, patch=None):
    """An ash sack standing on z: a fat straw-coloured bag, a sky blue tie, sometimes a patch."""
    g.sphere(STRAW, (x, y, z + r * tall), r, scale=(1.05, 0.95, tall), steps=(4, 6))
    if tie:
        g.box(BLUE, (x, y, z + 2 * r * tall + 3), (15, 15, 12))
    if patch:
        g.box(patch, (x + 4, y - r * 0.95, z + r * tall), (22, 6, 22))


def barrel(g, x0, x1, z, r, cols, gable=PINK, y=0.0, steps=10):
    """A barrel roof along X in bands of colour, a round gable at each end with a lemon vent."""
    n = len(cols)
    w = (x1 - x0) / float(n)
    for i, c in enumerate(cols):
        rod(g, c, (x0 + i * w, y, z), (x0 + (i + 1) * w, y, z), r + (6 if i % 2 else 0), steps=steps)
    for s, xe in ((-1, x0), (1, x1)):
        rod(g, gable, (xe - s * 10, y, z), (xe + s * 12, y, z), r - 10, steps=steps)
        rod(g, LEMON, (xe + s * 8, y, z + r * 0.38), (xe + s * 20, y, z + r * 0.38), r * 0.2, steps=8)
        g.box(PANE, (xe + s * 19, y, z + r * 0.38), (8, r * 0.24, 12))


def vane(sc, x, y, z, phase=0.3, size=1.0):
    """A weather vane on a spindle at (x, y, z): a lemon sun and a blue tail swinging slowly about the spindle."""
    g = sc.still()
    g.cyl(WHITE, (x, y, z), 5 * size, 90 * size, steps=6)
    g.sphere(PINK, (x, y, z + 96 * size), 9 * size, steps=(3, 6))
    v = sc.part(P.motion(swing=55, pivot=(x, y, z + 70 * size), axis=P.UP, rate=0.06, phase=phase))
    v.box(LILAC, (x, y, z + 70 * size), (120 * size, 5, 6))
    v.box(BLUE, (x - 50 * size, y, z + 70 * size), (46 * size, 5, 44 * size))
    rod(v, LEMON, (x + 52 * size, y - 3, z + 70 * size), (x + 52 * size, y + 3, z + 70 * size), 20 * size, steps=8)


# ------------------------------------------------------------------------------------------ buildings
def post_mill(sc):
    """BUILDING. A small post mill on a trestle: a stubby round pink body, a sky blue cone cap, four cloth sails
    turning on the front, a tail pole and a stair behind. 6.6 m across, 11.6 m to the sails' tip."""
    g = sc.still()
    g.cyl(LILAC, (0, 0, -200), 330, 290, steps=10, top_r=300)
    g.cyl(WHITE, (0, 0, 88), 304, 12, steps=10)
    g.cyl(LILAC, (0, 0, 95), 34, 330, steps=6)
    for a in (45, 135, 225, 315):
        ar = math.radians(a)
        tilt_box(g, LILAC, (250 * math.cos(ar), 250 * math.sin(ar), 98), (40 * math.cos(ar), 40 * math.sin(ar), 400), 24, 24)
    g.box(LILAC, (0, 0, 114), (560, 26, 26))
    g.box(LILAC, (0, 0, 114), (26, 560, 26))
    g.cyl(WHITE, (0, 0, 400), 255, 24, steps=10)
    g.cyl(PINK, (0, 0, 424), 240, 470, steps=10, top_r=205)
    g.cyl(LEMON, (0, 0, 640), 232, 26, steps=10, top_r=230)
    g.cyl(WHITE, (0, 0, 892), 236, 22, steps=10)
    g.cyl(BLUE, (0, 0, 914), 228, 190, steps=10, top_r=28)
    g.sphere(LEMON, (0, 0, 1112), 24, steps=(3, 6))
    ring(g, BLUE, 0, 0, 424 + 82, 232, 22, (84, 40, 164))                 # the door, round to one side of the sails
    ring(g, WHITE, 0, 0, 424 + 172, 230, 22, (104, 40, 12))
    rwindow(g, 0, 0, 760, 212, -48, 46, 64, 0)
    rwindow(g, 0, 0, 760, 212, 62, 46, 64, 1)
    rwindow(g, 0, 0, 540, 228, -62, 46, 64, 2)
    ring(g, LEMON, 0, 0, 690, 212, -15, (60, 36, 60))                     # patches on the wall
    ring(g, BLUE, 0, 0, 560, 226, 150, (70, 36, 70))
    ring(g, WHITE, 0, 0, 800, 205, 105, (56, 36, 56))
    tilt_box(g, WHITE, (0, 232, 424), (0, 540, 0), 74, 10)                # the stair behind
    tilt_box(g, LILAC, (40, 232, 470), (40, 540, 50), 6, 6)
    tilt_box(g, LILAC, (70, 200, 520), (150, 640, 40), 12, 12)            # the tail pole
    rod(g, LILAC, (0, -190, 760), (0, -276, 760), 20, steps=6)            # the windshaft
    s = sc.part(P.motion(swing=180, pivot=(0, -270, 760), axis=90.0, rate=0.05, phase=0.2))
    sails(s, (0, -270, 760), 400, wid=92, patches=4, stock=9.0)
    rod(s, WHITE, (0, -258, 760), (0, -290, 760), 30, steps=6)


def barn(sc):
    """BUILDING. A barrel-roofed barn: pink walls and round pink gables, a roof in white, blue and lemon bands, a
    blue double door under a lemon awning, a cupola with a vane. 11.2 x 7 m, 9.4 m."""
    g = sc.still()
    g.box(LILAC, (0, 0, -60), (1120, 700, 240))
    g.box(PINK, (0, 0, 250), (1000, 580, 380))
    g.box(WHITE, (0, 0, 440), (1030, 610, 22))
    barrel(g, -510, 510, 445, 282, (WHITE, BLUE, WHITE, LEMON, WHITE))
    g.box(BLUE, (0, -292, 205), (260, 10, 290))
    g.box(WHITE, (0, -296, 356), (296, 14, 16))
    g.box(WHITE, (0, -298, 205), (8, 8, 290))
    for s in (-1, 1):
        g.box(WHITE, (s * 140, -296, 205), (14, 14, 290))
        tilt_box(g, WHITE, (s * 126, -299, 66), (s * 8, -299, 344), 6, 14)
    for k in range(3):                                                    # a lemon and white awning over the door
        tilt_box(g, LEMON if k != 1 else WHITE, (-110 + k * 110, -290, 430), (-110 + k * 110, -400, 384), 110, 8)
    window(g, -340, -290, 280, 70, 90, 0)
    window(g, 340, -290, 280, 70, 90, 1)
    g.box(LEMON, (-340, -293, 150), (90, 6, 70))                          # quilt squares on the wall
    g.box(BLUE, (340, -293, 150), (90, 6, 70))
    g.box(WHITE, (-210, -293, 120), (60, 6, 60))
    tilt_box(g, LILAC, (438, -300, 60), (418, -292, 430), 8, 44)          # a ladder
    for i, (x, y) in enumerate(((-470, -320), (-420, -335), (-448, -372))):
        sack(g, x, y, 60, r=30, patch=PATCH[i] if i else None)
    g.box(WHITE, (0, 0, 760), (120, 120, 60))
    g.cyl(BLUE, (0, 0, 790), 92, 70, steps=6, top_r=8)
    vane(sc, 0, 0, 858, 0.4)


def granary(sc):
    """BUILDING. A round granary on mushroom stones: a pink drum with patches sewn round it, a wide white cone
    roof, a pennant. 6.8 m across, 10.3 m."""
    g = sc.still()
    g.cyl(LILAC, (0, 0, -200), 340, 230, steps=10, top_r=320)
    for k in range(6):
        a = math.radians(30 + k * 60)
        x, y = 230 * math.cos(a), 230 * math.sin(a)
        g.cyl(STONE, (x, y, 30), 30, 120, steps=6, top_r=20)
        g.cyl(STONE, (x, y, 148), 58, 24, steps=8, top_r=40)
    g.cyl(WHITE, (0, 0, 172), 300, 24, steps=10)
    g.cyl(PINK, (0, 0, 196), 280, 420, steps=10, top_r=250)
    g.cyl(LEMON, (0, 0, 372), 276, 30, steps=10, top_r=274)
    for i, (a, z) in enumerate(((-36, 500), (36, 505), (-72, 290), (72, 300), (108, 500), (-108, 490))):
        ring(g, PATCH[i % 3], 0, 0, z, 251 if z > 400 else 262, a, (92, 36, 92))
    ring(g, BLUE, 0, 0, 196 + 88, 266, 0, (84, 40, 176))
    ring(g, WHITE, 0, 0, 196 + 184, 262, 0, (104, 40, 12))
    tilt_box(g, WHITE, (0, -282, 196), (0, -430, 30), 74, 10)
    g.cyl(BLUE, (0, 0, 608), 346, 16, steps=10)
    g.cyl(WHITE, (0, 0, 624), 340, 300, steps=10, top_r=26)
    g.sphere(LEMON, (0, 0, 934), 26, steps=(3, 6))
    g.cyl(LILAC, (0, 0, 940), 5, 90, steps=6)
    f = sc.part(P.motion(swing=40, pivot=(0, 0, 1000), axis=P.UP, rate=0.3, phase=0.2))
    f.box(BLUE, (42, 0, 1004), (80, 5, 40))
    f.box(LEMON, (96, 0, 1004), (30, 5, 26))
    sack(g, 250, -260, 30, r=30, patch=LEMON)
    sack(g, 300, -220, 30, r=28)


def farmhouse(sc):
    """BUILDING. The farmhouse: two pink storeys under a blue and white barrel roof, a blue door, a lemon awning,
    seven windows with a patchwork quilt of shutters, a white chimney. 9.8 x 6.4 m, 10.2 m."""
    g = sc.still()
    g.box(LILAC, (0, 0, -70), (980, 640, 220))
    g.box(PINK, (0, 0, 320), (880, 540, 560))
    g.box(WHITE, (0, 0, 318), (892, 552, 14))
    g.box(WHITE, (0, 0, 600), (910, 570, 20))
    barrel(g, -450, 450, 606, 262, (BLUE, WHITE, BLUE, WHITE))
    g.cyl(WHITE, (250, 70, 800), 46, 190, steps=8)
    g.cyl(LILAC, (250, 70, 990), 58, 18, steps=8)
    g.cyl(WHITE, (250, 70, 1008), 26, 30, steps=6)
    g.box(BLUE, (-110, -272, 145), (104, 10, 210))
    g.box(WHITE, (-110, -276, 256), (128, 14, 14))
    g.box(LEMON, (-110, -274, 280), (90, 8, 30))
    g.box(LEMON, (-86, -280, 150), (10, 8, 10))
    for k in range(3):
        tilt_box(g, (LEMON, WHITE, LEMON)[k], (-170 + k * 60, -270, 318), (-170 + k * 60, -360, 284), 60, 8)
    g.box(STONE, (-110, -310, 20), (150, 80, 40))
    g.box(STONE, (-110, -370, 5), (150, 50, 30))
    for i, x in enumerate((-330, 130, 330)):
        window(g, x, -270, 190, 66, 96, i)
    for i, x in enumerate((-330, -110, 110, 330)):
        window(g, x, -270, 460, 66, 96, i + 1)
    for i, (dx, dz) in enumerate(((-34, 0), (34, 0), (-34, -56), (34, -56))):          # a quilt aired from a window
        g.box((BLUE, LEMON, WHITE, BLUE)[i], (110 + dx, -284, 370 + dz), (66, 6, 54))
    g.cyl(BLUE, (400, -320, 40), 40, 90, steps=8)                         # a rain butt
    g.cyl(WHITE, (400, -320, 128), 42, 6, steps=8)
    g.box(BLUE, (30, -300, 70), (150, 40, 10))                            # a bench
    for s in (-1, 1):
        g.box(BLUE, (30 + s * 62, -300, 50), (10, 36, 40))
    g.box(BLUE, (30, -282, 104), (150, 8, 50))


def sieve_house(sc):
    """BUILDING. The sieve house: a tall open frame of three decks with round sieves hanging and swinging in it, a
    pink hopper under a white cone on top, a blue chute. 6.2 x 5.2 m, 13.6 m."""
    g = sc.still()
    g.box(LILAC, (0, 0, -80), (620, 520, 200))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(LILAC, (sx * 240, sy * 190, 520), (22, 22, 1000))
    for z in (340, 660, 960):
        g.box(WHITE, (0, 0, z), (560, 460, 16))
        g.box(LEMON, (0, -232, z), (560, 6, 30))
    for i, (z0, z1) in enumerate(((30, 330), (350, 650), (670, 950))):
        sgn = 1 if i % 2 else -1
        tilt_box(g, LILAC, (-sgn * 240, 190, z0), (sgn * 240, 190, z1), 12, 12)
        tilt_box(g, LILAC, (240, -sgn * 190, z0), (240, sgn * 190, z1), 12, 12)
    g.cyl(PINK, (0, 0, 968), 170, 200, steps=10, top_r=150)
    g.cyl(BLUE, (0, 0, 1040), 164, 26, steps=10, top_r=160)
    g.cyl(WHITE, (0, 0, 1168), 200, 130, steps=10, top_r=20)
    g.sphere(LEMON, (0, 0, 1306), 20, steps=(3, 6))
    g.cyl(LILAC, (0, 0, 1310), 5, 60, steps=6)
    f = sc.part(P.motion(swing=40, pivot=(0, 0, 1350), axis=P.UP, rate=0.3, phase=0.6))
    f.box(PINK, (38, 0, 1350), (70, 5, 34))
    tilt_box(g, BLUE, (130, -110, 980), (262, -236, 372), 44, 44)        # the chute, down to the first deck
    tilt_box(g, WHITE, (-290, -150, 20), (-262, -150, 960), 8, 50)        # a ladder up one side
    g.cyl(STRAW, (150, -60, 20), 130, 80, steps=8, top_r=12)              # sifted ash in a heap
    sack(g, -150, -160, 20, r=30, patch=BLUE)
    sack(g, -90, -190, 20, r=30)
    sack(g, -190, -110, 20, r=28, patch=LEMON)
    hung = ((-150, -120, 332, 200, 70, LEMON), (0, -130, 332, 190, 78, BLUE), (150, -120, 332, 205, 66, PINK),
            (-95, -120, 652, 520, 74, BLUE), (95, -120, 652, 510, 70, LEMON), (0, -110, 952, 820, 82, WHITE))
    for i, (x, y, zh, zs, r, c) in enumerate(hung):
        m = sc.part(P.motion(swing=7, pivot=(x, y, zh), axis=0.0, rate=0.3 + 0.03 * i, phase=0.17 * i))
        m.box(LILAC, (x, y, (zh + zs + r) / 2.0), (5, 5, zh - zs - r))
        rod(m, c, (x, y - 7, zs), (x, y + 7, zs), r, steps=8)
        rod(m, STRAW, (x, y - 9, zs), (x, y + 9, zs), r - 12, steps=8)


def sail_loft(sc):
    """BUILDING. The sail loft: long and low, pink under a sky blue barrel roof; patchwork sails laid out to dry on
    the roof, a line of cloths flapping along the ridge, rolled sails leaning by the loft door. 14.2 x 6.2 m, 9.3 m."""
    g = sc.still()
    g.box(LILAC, (0, 0, -70), (1420, 620, 220))
    g.box(PINK, (0, 0, 205), (1300, 520, 330))
    g.box(WHITE, (0, 0, 370), (1330, 550, 18))
    barrel(g, -660, 660, 374, 252, (BLUE, BLUE, LEMON, BLUE, BLUE))
    R = 252 + 6 + 22
    p1 = (-R * math.sin(math.radians(62)), 374 + R * math.cos(math.radians(62)))
    p2 = (-R * math.sin(math.radians(30)), 374 + R * math.cos(math.radians(30)))
    for cx, off in ((-390, 0), (390, 1)):                                 # two sails drying on the roof's front slope
        for i in range(5):
            x = cx - 208 + i * 104
            tilt_box(g, (WHITE, LEMON, BLUE, WHITE, PINK)[(i + off) % 5], (x, p1[0], p1[1]), (x, p2[0], p2[1]), 98, 6)
        g.box(LILAC, (cx, p1[0], p1[1]), (540, 8, 8))
        g.box(LILAC, (cx, p2[0], p2[1]), (540, 8, 8))
    g.box(WHITE, (0, -262, 180), (220, 10, 280))
    g.box(BLUE, (0, -266, 328), (252, 14, 16))
    for s in (-1, 1):
        g.box(BLUE, (s * 118, -266, 180), (16, 14, 280))
        tilt_box(g, BLUE, (s * 104, -269, 46), (s * 6, -269, 314), 6, 12)
    for i, x in enumerate((-500, -300, 300, 500)):
        window(g, x, -260, 230, 62, 86, i)
    for i, x in enumerate((-190, -160, 170)):                             # rolled sails leaning by the door
        rod(g, (BLUE, WHITE, LEMON)[i], (x, -300, 40), (x + 14, -268, 330), 15, steps=6)
    g.box(LILAC, (590, -300, 55), (150, 50, 30))                          # a sailmaker's bench, a bolt of cloth on it
    rod(g, LEMON, (530, -300, 86), (650, -300, 86), 16, steps=6)
    for x in (-600, 0, 600):
        g.box(LILAC, (x, 0, 770), (8, 8, 250))
    g.box(LILAC, (0, 0, 884), (1208, 5, 5))
    for k in range(9):
        x = -520 + k * 130
        m = sc.part(P.motion(swing=12 + 3 * (k % 3), pivot=(x, 0, 884), axis=0.0, rate=0.42 + 0.03 * (k % 4), phase=0.13 * k))
        m.box((LEMON, BLUE, WHITE, PINK)[k % 4], (x, 0, 860), (74, 5, 44))
        m.box((WHITE, LEMON, BLUE)[k % 3], (x, 0, 816), (74, 5, 44))


def dovecote(sc):
    """BUILDING. The dovecote: a round white tower with pink bands, rows of dark holes on lemon ledges, a wide blue
    cone roof with a lantern and a vane, four white doves sitting. 6 m across, 12.9 m."""
    g = sc.still()

    def rad(z):
        return 230.0 - 45.0 * (z - 40.0) / 780.0

    g.cyl(LILAC, (0, 0, -200), 300, 240, steps=10, top_r=285)
    g.cyl(WHITE, (0, 0, 40), 230, 780, steps=10, top_r=185)
    for z in (250, 560):
        g.cyl(PINK, (0, 0, z), rad(z) + 7, 50, steps=10, top_r=rad(z + 50) + 7)
    for z in (350, 450, 660, 740):
        for a in (-60, -30, 0, 30, 60):
            ring(g, PANE, 0, 0, z, rad(z) - 10, a, (36, 40, 46))
            ring(g, LEMON, 0, 0, z - 30, rad(z) - 2, a, (52, 44, 8))
    ring(g, BLUE, 0, 0, 40 + 80, rad(120) - 8, 0, (80, 40, 160))
    ring(g, WHITE, 0, 0, 40 + 168, rad(200) - 8, 0, (100, 40, 12))
    g.cyl(WHITE, (0, 0, 814), 296, 14, steps=10)
    g.cyl(BLUE, (0, 0, 828), 290, 260, steps=10, top_r=62)
    g.cyl(WHITE, (0, 0, 1076), 60, 90, steps=8)
    for a in (0, 90, 180, 270):
        ring(g, PANE, 0, 0, 1122, 50, a, (30, 24, 50))
    g.cyl(LEMON, (0, 0, 1166), 82, 70, steps=8, top_r=8)
    vane(sc, 0, 0, 1196, 0.1)
    for a in (-45, 45):                                                   # two perches under the eaves
        ar = math.radians(a)
        rod(g, LILAC, (190 * math.sin(ar), -190 * math.cos(ar), 790), (330 * math.sin(ar), -330 * math.cos(ar), 790), 6, steps=6)
    d = sc.part(P.motion(bob=3, rate=0.4, phase=0.2))
    for x, y, z in ((225, -225, 796), (-205, -205, 796), (112, -195, 324), (-102, -177, 634)):
        d.sphere(WHITE, (x, y, z + 13), 13, scale=(1.4, 1.0, 1.0), steps=(3, 6))
        d.sphere(WHITE, (x + 16, y, z + 26), 8, steps=(3, 5))
        d.box(LEMON, (x + 26, y, z + 25), (7, 5, 5))


# ------------------------------------------------------------------------------------------ low props (under 2 m)
def sack_row(sc):
    """PROP, LOW. A row of ash sacks with sky blue ties, stacked three high on a lilac pallet, patches sewn on.
    5.4 m long, 0.7 m deep, 1.9 m."""
    g = sc.still()
    g.box(LILAC, (0, 0, -42), (540, 70, 114))
    for i in range(6):
        sack(g, -210 + i * 80, 0, 15, r=31, tie=(i in (0, 5)), patch=(LEMON if i == 1 else (BLUE if i == 4 else None)))
    for i in range(4):
        sack(g, -130 + i * 80, 0, 72, r=30, tie=(i in (0, 3)), patch=(WHITE if i == 2 else None))
    for i in range(2):
        sack(g, -50 + i * 80, 0, 126, r=29, tie=True, patch=(PINK if i == 0 else None))
    sack(g, 245, 0, 15, r=24, tall=1.3, tie=True)
    g.box(LEMON, (-250, 0, 60), (8, 50, 90))                              # a sack barrow's board at one end
    g.box(LILAC, (-256, 0, 120), (6, 60, 6))


def basket_rack(sc):
    """PROP, LOW. Winnowing baskets on a rack: seven round straw trays with coloured rims leaning on two rails, a
    fork, a heap of chaff. 5 m long, 0.8 m deep, 1.9 m."""
    g = sc.still()
    for x in (-244, 0, 244):
        g.box(LILAC, (x, 14, 40), (12, 12, 280))
    for z in (24, 96, 172):
        g.box(LILAC, (0, 14, z), (500, 9, 9))
    ax = (0.0, -0.94, 0.34)

    def tray(x, z, r, c):
        cy = 2.0
        rod(g, c, (x, cy - ax[1] * 7, z - ax[2] * 7), (x, cy + ax[1] * 7, z + ax[2] * 7), r, steps=8)
        rod(g, STRAW, (x, cy - ax[1] * 5, z - ax[2] * 5), (x, cy + ax[1] * 10, z + ax[2] * 10), r - 9, steps=8)

    for i, x in enumerate((-180, -60, 60, 180)):
        tray(x, 136, 50, (LEMON, BLUE, PINK, WHITE)[i])
    for i, x in enumerate((-122, 0, 122)):
        tray(x, 58, 48, (BLUE, WHITE, LEMON)[i])
    tilt_box(g, STRAW, (226, -20, 0), (212, 8, 186), 6, 6)                # a winnowing fork
    for s in (-1, 0, 1):
        g.box(STRAW, (212 + s * 12, 8, 176), (5, 5, 30))
    g.sphere(LEMON, (-215, -8, 4), 56, scale=(1.3, 0.5, 0.45), steps=(3, 6))


def millstone(sc):
    """PROP, LOW. A millstone leaning on a lilac block, its eye sky blue and a white band round it; a second stone
    lying flat with a sack on it. 3 m long, 1.2 m deep, 1.75 m."""
    g = sc.still()
    g.box(LILAC, (-10, 44, 20), (130, 70, 140))
    ax = (0.0, -0.95, 0.31)
    c = (-10.0, -14.0, 86.0)
    rod(g, STONE, (c[0], c[1] - ax[1] * 15, c[2] - ax[2] * 15), (c[0], c[1] + ax[1] * 15, c[2] + ax[2] * 15), 84, steps=10)
    rod(g, WHITE, (c[0], c[1] - ax[1] * 6, c[2] - ax[2] * 6), (c[0], c[1] + ax[1] * 6, c[2] + ax[2] * 6), 88, steps=10)
    rod(g, BLUE, (c[0], c[1] - ax[1] * 19, c[2] - ax[2] * 19), (c[0], c[1] + ax[1] * 19, c[2] + ax[2] * 19), 16, steps=6)
    rod(g, LEMON, (c[0], c[1] + ax[1] * 14, c[2] + ax[2] * 14), (c[0], c[1] + ax[1] * 18, c[2] + ax[2] * 18), 40, steps=8)
    g.cyl(STONE, (150, 0, -40), 76, 66, steps=10)
    g.cyl(WHITE, (150, 0, 26), 22, 5, steps=6)
    sack(g, 168, 10, 26, r=28, patch=LEMON)
    sack(g, -130, -6, 0, r=26, tall=1.2)
    tilt_box(g, LILAC, (96, -40, 0), (70, -30, 110), 6, 6)                # a mill bill (the dressing pick)
    g.box(STONE, (70, -30, 112), (40, 10, 12))


def hand_cart(sc):
    """PROP, LOW. A hand cart: two wooden wheels, two shafts with a cross handle resting on the ground, a pink bed
    of sacks under a patchwork cover whose flap lifts a little. Nothing drives it. 3.6 m long, 1.5 m wide, 1.8 m."""
    g = sc.still()
    for s in (-1, 1):
        rod(g, STRAW, (0, s * 58, 55), (0, s * 68, 55), 55, steps=10)
        rod(g, LILAC, (0, s * 54, 55), (0, s * 74, 55), 14, steps=6)
        tilt_box(g, LILAC, (90, s * 40, 84), (262, s * 40, 16), 8, 8)
    g.box(LILAC, (262, 0, 16), (8, 96, 8))
    g.box(LILAC, (-88, 0, 36), (8, 8, 72))
    g.box(PINK, (0, 0, 92), (190, 104, 44))
    g.box(WHITE, (0, 0, 117), (198, 112, 6))
    g.box(LEMON, (0, -54, 92), (60, 6, 26))
    for i, x in enumerate((-56, 0, 56)):
        sack(g, x, 0, 104, r=27, tie=False)
    cols = ((LEMON, BLUE), (WHITE, LEMON), (BLUE, WHITE), (PINK, LEMON))
    xs = (-96, -48, 0, 48, 96)
    zs = (150, 170, 176, 170, 150)
    for i in range(4):
        for j, y in enumerate((-28, 28)):
            tilt_box(g, cols[i][j], (xs[i], y, zs[i]), (xs[i + 1], y, zs[i + 1]), 56, 6)
    f = sc.part(P.motion(swing=8, pivot=(-98, 0, 150), axis=90.0, rate=0.4, phase=0.3))
    f.box(WHITE, (-100, -28, 122), (6, 56, 56))
    f.box(BLUE, (-100, 28, 122), (6, 56, 56))
    g.cyl(BLUE, (150, -70, 0), 20, 44, steps=6, top_r=15)                 # a jug set down beside it


def quilt_fence(sc):
    """PROP, LOW. A windbreak hurdle: woven straw between lilac posts, three patchwork quilts aired over its rail
    and stirring, a paper windmill on one post. 5 m long, 1.95 m."""
    g = sc.still()
    for x in (-240, -80, 80, 240):
        g.box(LILAC, (x, 0, 25), (12, 12, 250))
    g.box(STRAW, (0, 0, 46), (480, 8, 72))
    g.box(LILAC, (0, 0, 86), (480, 9, 8))
    g.box(LILAC, (0, 0, 150), (480, 9, 8))
    sets = ((LEMON, BLUE, WHITE, PINK), (BLUE, WHITE, LEMON, BLUE), (PINK, LEMON, BLUE, WHITE))
    for q, x in enumerate((-160, 0, 160)):
        m = sc.part(P.motion(swing=6, pivot=(x, 0, 152), axis=0.0, rate=0.45 + 0.04 * q, phase=0.3 * q))
        for i, (dx, z) in enumerate(((-31, 126), (31, 126), (-31, 76), (31, 76))):
            m.box(sets[q][i], (x + dx, -8, z), (60, 5, 48))
        m.box(WHITE, (x, -4, 153), (126, 14, 6))
    g.cyl(LILAC, (240, -8, 150), 4, 22, steps=6)
    w = sc.part(P.motion(swing=180, pivot=(240, -14, 170), axis=90.0, rate=0.25, phase=0.5))
    fan(w, (240, -14, 170), 23, n=4, cols=(LEMON, BLUE), wid=14, thick=4.5)


# ------------------------------------------------------------------------------------------ tall props, open
def wind_pump(sc):
    """PROP, TALL, open. A small wind pump on a thin lattice: an eight-bladed patchwork wheel turning, a lemon tail
    with a blue stripe, a trough at its foot. 7.3 m."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, LILAC, (sx * 90, sy * 90, -100), (sx * 22, sy * 22, 540), 8, 8)
    for z in (150, 330):
        hw = 90.0 - 68.0 * (z + 100.0) / 640.0
        for s in (-1, 1):
            g.box(LILAC, (0, s * hw, z), (2 * hw, 6, 6))
            g.box(LILAC, (s * hw, 0, z), (6, 2 * hw, 6))
    g.box(WHITE, (0, 0, 545), (96, 96, 8))
    g.box(LILAC, (0, 0, 580), (30, 130, 30))
    g.box(WHITE, (0, 0, 270), (6, 6, 540))
    tilt_box(g, LILAC, (0, 60, 580), (0, 210, 600), 6, 6)
    g.box(LEMON, (0, 256, 608), (6, 110, 90))
    g.box(BLUE, (0, 256, 608), (8, 30, 90))
    g.box(LILAC, (140, 0, 0), (130, 64, 80))
    g.box(BLUE, (140, 0, 41), (116, 50, 4))
    tilt_box(g, WHITE, (20, 0, 60), (80, 0, 46), 8, 8)
    w = sc.part(P.motion(swing=180, pivot=(0, -78, 580), axis=90.0, rate=0.1, phase=0.4))
    fan(w, (0, -78, 580), 150, n=8, cols=(LEMON, BLUE, WHITE, PINK))


def scare_figure(sc):
    """PROP, TALL, open. A scare-figure of cloth on a cross pole: a straw-sack head under a lemon hat, a pink
    patchwork coat, two sleeves that flap, two straw stooks at its foot. 3.3 m."""
    g = sc.still()
    g.box(LILAC, (0, 0, 110), (10, 10, 420))
    g.box(LILAC, (0, 0, 236), (236, 8, 8))
    g.box(PINK, (0, -2, 186), (72, 26, 112))
    g.box(LEMON, (-16, -16, 204), (24, 5, 24))
    g.box(BLUE, (18, -16, 162), (22, 5, 26))
    g.box(WHITE, (-14, -16, 150), (18, 5, 18))
    g.box(BLUE, (0, -15, 238), (30, 6, 10))
    g.sphere(STRAW, (0, 0, 274), 30, steps=(4, 6))
    g.box(BLUE, (-10, -27, 280), (7, 5, 7))                               # two button eyes
    g.box(BLUE, (10, -27, 280), (7, 5, 7))
    g.cyl(LEMON, (0, 0, 294), 54, 6, steps=8)
    g.cyl(BLUE, (0, 0, 300), 29, 9, steps=8)
    g.cyl(LEMON, (0, 0, 309), 27, 22, steps=8, top_r=21)
    sk = sc.part(P.motion(swing=9, pivot=(0, 0, 132), axis=0.0, rate=0.5, phase=0.1))
    for i, x in enumerate((-24, 0, 24)):
        sk.box(PATCH[i], (x, -2, 108), (22, 8, 48))
    for s in (-1, 1):
        m = sc.part(P.motion(swing=26, pivot=(s * 112, 0, 236), axis=90.0, rate=0.45, phase=0.0 if s < 0 else 0.4))
        m.box(PINK, (s * 100, -1, 196), (34, 22, 84))
        m.box(LEMON, (s * 100, -1, 150), (38, 24, 10))
        m.box(WHITE, (s * 100, -1, 128), (8, 5, 36))
    for s in (-1, 1):
        g.cyl(STRAW, (s * 92, 10, -40), 42, 150, steps=6, top_r=8)
        g.cyl(BLUE, (s * 92, 10, 60), 21, 8, steps=6)


def wind_sock(sc):
    """PROP, TALL, open. A striped wind sock, pink and white, streaming from a thin banded pole. 7.1 m."""
    g = sc.still()
    g.cyl(LILAC, (0, 0, -100), 46, 130, steps=8, top_r=38)
    for k in range(4):
        g.cyl(WHITE if k % 2 == 0 else PINK, (0, 0, 30 + k * 165), 7, 165, steps=6)
    g.sphere(LEMON, (0, 0, 700), 12, steps=(3, 6))
    g.box(LILAC, (20, 0, 660), (44, 5, 5))
    g.cyl(BLUE, (0, 0, 110), 11, 26, steps=6)
    radii = (34, 30, 26, 22, 18, 14)
    for k in range(5):
        m = sc.part(P.motion(swing=3 + 3 * k, pivot=(40, 0, 660), axis=P.UP, rate=0.5, phase=0.06 * k))
        m.cyl(PINK if k % 2 == 0 else WHITE, (40 + k * 62, 0, 660 - 2.5 * k * k), radii[k], 62, rot=(-90, 0, 0), steps=8, top_r=radii[k + 1])


def mill_sign(sc):
    """PROP, TALL, open. A signpost with no words: a round white board carrying four little cloth sails, swinging
    from a bracket; a weather vane on top over four lemon balls. 5.4 m."""
    g = sc.still()
    g.box(STONE, (0, 0, -30), (70, 70, 140))
    g.box(LILAC, (0, 0, 230), (12, 12, 380))
    g.box(LILAC, (58, 0, 382), (116, 6, 6))
    tilt_box(g, LILAC, (6, 0, 330), (60, 0, 380), 5, 5)
    b = sc.part(P.motion(swing=7, pivot=(60, 0, 380), axis=0.0, rate=0.3, phase=0.2))
    for s in (-1, 1):
        b.box(LILAC, (60 + s * 30, 0, 364), (4.5, 4.5, 32))
    rod(b, PINK, (60, -3, 300), (60, 3, 300), 62, steps=10)
    rod(b, WHITE, (60, -5, 300), (60, 5, 300), 54, steps=10)
    for i, (sx, sz) in enumerate(((1, 1), (-1, 1), (-1, -1), (1, -1))):
        tilt_box(b, (BLUE, LEMON, BLUE, LEMON)[i], (60 + sx * 8, -7, 300 + sz * 8), (60 + sx * 34, -7, 300 + sz * 34), 5, 16)
    rod(b, PINK, (60, -9, 300), (60, -5, 300), 9, steps=6)
    g.box(LILAC, (0, 0, 432), (124, 5, 5))
    g.box(LILAC, (0, 0, 432), (5, 124, 5))
    for dx, dy in ((62, 0), (-62, 0), (0, 62), (0, -62)):
        g.sphere(LEMON, (dx, dy, 432), 9, steps=(3, 6))
    vane(sc, 0, 0, 420, 0.5, size=1.1)


def bunting(sc):
    """PROP, TALL, open. A washing line of patchwork quilt squares between two thin poles, each square flapping on
    its own; a blue wash tub and a basket under it. 6.2 m long, 3.5 m."""
    g = sc.still()
    for s in (-1, 1):
        g.box(LILAC, (s * 300, 0, 120), (10, 10, 440))
        g.sphere(LEMON, (s * 300, 0, 348), 11, steps=(3, 6))
        tilt_box(g, LILAC, (s * 300, 0, 330), (0, 0, 300), 5, 5)
    for k in range(7):
        x = -240 + k * 80
        zl = 300 + 30.0 * abs(x) / 300.0
        m = sc.part(P.motion(swing=12 + 3 * (k % 3), pivot=(x, 0, zl), axis=0.0, rate=0.42 + 0.03 * (k % 4), phase=0.13 * k))
        m.box((LEMON, BLUE, PINK, WHITE)[k % 4], (x, 0, zl - 22), (64, 5, 40))
        m.box((WHITE, LEMON, BLUE)[k % 3], (x, 0, zl - 64), (64, 5, 40))
        m.box(WHITE, (x, 0, zl + 1), (10, 7, 8))
    g.cyl(BLUE, (-230, -20, 0), 42, 40, steps=8, top_r=48)
    g.cyl(WHITE, (-230, -20, 36), 40, 6, steps=8)
    g.cyl(STRAW, (-140, 10, 0), 34, 34, steps=8, top_r=40)
    g.box(LEMON, (-140, 10, 38), (50, 40, 10))


# ------------------------------------------------------------------------------------------ the hero
def grand_mill(sc):
    """HERO. The Grand Mill: a pink tower mill 45 m to the tip of its sails, on a round white plinth with sky blue
    cellar doors; a gallery hung with patchwork cloths, four storeys of shuttered windows, a white cap with a tail
    fan, a sack on a hoist, and four vast patchwork sails (sky blue, white, lemon) turning slowly."""
    g = sc.still()

    def rad(z):
        return 620.0 - 240.0 * (z - 250.0) / 2550.0

    g.cyl(LILAC, (0, 0, -300), 1060, 380, steps=14, top_r=1000)
    g.cyl(WHITE, (0, 0, 80), 900, 170, steps=14)
    g.cyl(LEMON, (0, 0, 236), 916, 22, steps=14)
    for a in (-64, -34, 34, 64, 116, -116):
        ring(g, BLUE, 0, 0, 160, 872, a, (136, 70, 130))
        ring(g, WHITE, 0, 0, 228, 872, a, (156, 70, 10))
    for i in range(6):                                                    # the steps up to the door
        top = 250.0 - 42.0 * i
        g.box(STONE, (0, -905 - 50 * i, (top - 100.0) / 2.0), (270, 52, top + 100.0))
    g.cyl(PINK, (0, 0, 250), 620, 2550, steps=14, top_r=380)
    g.cyl(LEMON, (0, 0, 700), rad(700) + 8, 30, steps=14, top_r=rad(730) + 8)
    for z in (1750, 2350):
        g.cyl(WHITE, (0, 0, z), rad(z) + 10, 40, steps=14, top_r=rad(z + 40) + 10)
    # the door, its fanlight and awning
    ring(g, BLUE, 0, 0, 420, 594, 0, (190, 60, 340))
    ring(g, WHITE, 0, 0, 598, 590, 0, (240, 60, 22))
    ring(g, LEMON, 0, 0, 640, 578, 0, (150, 60, 56))
    for k in range(3):
        tilt_box(g, (LEMON, WHITE, LEMON)[k], (-100 + k * 100, -588, 730), (-100 + k * 100, -760, 672), 100, 10)
    # the gallery: deck, struts, rail, a cloth hung from every length of rail
    g.cyl(WHITE, (0, 0, 1100), 820, 28, steps=14)
    for k in range(8):
        a = math.radians(22.5 + k * 45.0)
        tilt_box(g, LILAC, (rad(800) * math.cos(a), rad(800) * math.sin(a), 800), (780 * math.cos(a), 780 * math.sin(a), 1100), 22, 22)
    seg = 2.0 * 800.0 * math.tan(math.pi / 14.0) + 10.0
    for k in range(14):
        a = k * 360.0 / 14.0
        ring(g, WHITE, 0, 0, 1240, 800, a, (seg, 14, 14))
        ring(g, WHITE, 0, 0, 1184, 806, a + 180.0 / 14.0, (20, 20, 112))
        ring(g, (LEMON, BLUE, WHITE, PINK, BLUE, LEMON, WHITE)[k % 7], 0, 0, 1192, 808, a, (120, 6, 76))
    ring(g, BLUE, 0, 0, 1270, rad(1270) - 14, 0, (130, 60, 280))          # the door onto the gallery
    ring(g, WHITE, 0, 0, 1418, rad(1418) - 14, 0, (164, 60, 16))
    # four storeys of windows
    k = 0
    for z, angs in ((900, (-40, 40)), (1500, (-42, 42)), (2050, (-44, 0, 44)), (2560, (-30, 30))):
        for a in angs:
            rwindow(g, 0, 0, z, rad(z), a, 110, 150, k)
            k += 1
    for i, (a, z) in enumerate(((-66, 1650), (64, 2230), (-18, 1850), (14, 880), (80, 1420), (-84, 2420), (0, 2400))):
        ring(g, PATCH[i % 3], 0, 0, z, rad(z) - 12, a, (150, 40, 150))    # great patches sewn on the tower
    # the cap, its finial and pennant
    g.cyl(WHITE, (0, 0, 2800), 402, 50, steps=14)
    g.cyl(WHITE, (0, 0, 2850), 412, 200, steps=14, top_r=300)
    g.cyl(BLUE, (0, 0, 2846), 420, 16, steps=14)
    g.cyl(WHITE, (0, 0, 3050), 300, 130, steps=14, top_r=110)
    g.sphere(LEMON, (0, 0, 3206), 42, steps=(4, 8))
    g.cyl(LILAC, (0, 0, 3230), 8, 130, steps=6)
    p = sc.part(P.motion(swing=35, pivot=(0, 0, 3330), axis=P.UP, rate=0.25, phase=0.3))
    p.box(BLUE, (80, 0, 3330), (150, 8, 70))
    p.box(LEMON, (180, 0, 3330), (60, 8, 46))
    # the windshaft's housing, the shaft, the sails
    g.box(WHITE, (0, -430, 2950), (210, 300, 210))
    g.box(BLUE, (0, -584, 2950), (150, 10, 150))
    rod(g, LILAC, (0, -560, 2950), (0, -660, 2950), 50, steps=8)
    s = sc.part(P.motion(swing=180, pivot=(0, -640, 2950), axis=90.0, rate=0.02, phase=0.1))
    sails(s, (0, -640, 2950), 1550, wid=300, patches=6, stock=40.0, inner=0.16)
    rod(s, WHITE, (0, -610, 2950), (0, -676, 2950), 92, steps=8)
    rod(s, LEMON, (0, -676, 2950), (0, -694, 2950), 46, steps=8)
    # the tail fan behind the cap
    tilt_box(g, LILAC, (0, 330, 2900), (0, 720, 3060), 24, 24)
    for sx in (-1, 1):
        tilt_box(g, LILAC, (sx * 220, 290, 2860), (0, 720, 3060), 16, 16)
    t = sc.part(P.motion(swing=180, pivot=(0, 740, 3060), axis=90.0, rate=0.12, phase=0.6))
    fan(t, (0, 740, 3060), 210, n=8, cols=(BLUE, LEMON, WHITE), thick=8.0)
    # the sack hoist on the gallery's side
    g.box(LILAC, (700, 0, 1750), (480, 22, 22))
    tilt_box(g, LILAC, (rad(1500), 0, 1500), (900, 0, 1740), 16, 16)
    h = sc.part(P.motion(swing=3, pivot=(905, 0, 1750), axis=90.0, rate=0.18, phase=0.5))
    h.box(LILAC, (905, 0, 1410), (6, 6, 680))
    h.sphere(STRAW, (905, 0, 1010), 62, scale=(1.0, 1.0, 1.2), steps=(4, 6))
    h.box(BLUE, (905, 0, 1084), (26, 26, 16))
    # millstones and sacks against the plinth
    for sx in (-1, 1):
        rod(g, STONE, (sx * 470, -850, 150), (sx * 474, -812, 168), 78, steps=10)
        rod(g, BLUE, (sx * 469, -860, 145), (sx * 470, -850, 150), 16, steps=6)
    for i, (x, y) in enumerate(((330, -920), (402, -888), (260, -945), (-330, -920), (-400, -890))):
        sack(g, x, y, 80, r=36, patch=PATCH[i % 3] if i % 2 else None)


JOBS = [
    ('SM_S1D5_PostMill', post_mill),
    ('SM_S1D5_Barn', barn),
    ('SM_S1D5_Granary', granary),
    ('SM_S1D5_Farmhouse', farmhouse, STILL),
    ('SM_S1D5_SieveHouse', sieve_house),
    ('SM_S1D5_SailLoft', sail_loft),
    ('SM_S1D5_Dovecote', dovecote),
    ('SM_S1D5_SackRow', sack_row, STILL),
    ('SM_S1D5_BasketRack', basket_rack, STILL),
    ('SM_S1D5_Millstone', millstone, STILL),
    ('SM_S1D5_HandCart', hand_cart, dict(grow=(10, 10, 10), grow_down=(10, 10, 0))),
    ('SM_S1D5_QuiltFence', quilt_fence, dict(grow=(10, 30, 5), grow_down=(10, 30, 0))),
    ('SM_S1D5_WindPump', wind_pump, dict(grow=(20, 20, 20), grow_down=(20, 20, 0))),
    ('SM_S1D5_ScareFigure', scare_figure, dict(grow=(50, 30, 10), grow_down=(50, 30, 0))),
    ('SM_S1D5_WindSock', wind_sock, dict(grow=(30, 110, 20), grow_down=(30, 110, 0))),
    ('SM_S1D5_MillSign', mill_sign, dict(grow=(30, 30, 10), grow_down=(30, 30, 0))),
    ('SM_S1D5_Bunting', bunting, dict(grow=(10, 40, 10), grow_down=(10, 40, 0))),
    ('SM_S1D5_GrandMill', grand_mill, dict(far=True, grow=(60, 60, 60), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
