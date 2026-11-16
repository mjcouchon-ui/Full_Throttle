"""Under the Sea, Stage 1, builder s1d7: "Nereida: Lamp Row", 16.1 to 20.6 km: the pilots' and lamp-keepers' quarter.
/Game/World/UnderTheSea/SM_S1D7_*. Run: import ft_world_sea_s1d7 as K; K.run()

Tall narrow round towers banded red and white with a lamp room and a cone roof, galleries on brackets, keepers'
cottages with round gables. White, signal red (paint only: nothing red is lit), ink for roofs and trim, lamp cream
(the only thing that glows, steadily), teal. Every lamp's lens turns slowly behind fixed mullions: the glow is one
even drum of light, so nothing blinks and nothing sweeps. Placed by C:\\ftwork\\s1\\L6_s1d7.inl.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'
W, RED, INK, CREAM, TEAL = '#e6eef2', '#ff3355', '#16304f', '#fff1c0', '#3f8a9a'
ROCK, ROCK_D, ROCK_L = '#9a6a7e', '#6a4562', '#c79aa2'      # the kit's rock
BRASS, GLASS = '#c9b44a', '#cfeeff'                         # the kit's old gold and pale glass
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SMALL = dict(grow=(25, 25, 25), grow_down=(25, 25, 0))


# ------------------------------------------------------------------------------------------ helpers
def _pt(cx, cy, r, a):
    a = math.radians(a)
    return cx + math.cos(a) * r, cy + math.sin(a) * r


def ring(g, col, cx, cy, z, r, n, w=8.0, h=8.0, a0=0.0):
    """A rail: n straight bars joining the corners of an n-sided ring of radius r, at height z (its middle)."""
    seg = 2.0 * r * math.sin(math.pi / n) + w
    rm = r * math.cos(math.pi / n)
    for k in range(n):
        a = a0 + 360.0 * (k + 0.5) / n
        x, y = _pt(cx, cy, rm, a)
        g.box(col, (x, y, z), (seg, w, h), rot=(0, a + 90.0, 0))


def posts(g, col, cx, cy, z0, r, n, w, h, a0=0.0):
    for k in range(n):
        a = a0 + 360.0 * k / n
        x, y = _pt(cx, cy, r, a)
        g.box(col, (x, y, z0 + h / 2.0), (w, w, h), rot=(0, a, 0))


def struts(g, col, cx, cy, z0, r0, z1, r1, n, w=12.0, a0=0.0):
    """Brackets: n struts from the wall (r0, z0) out and up to (r1, z1)."""
    for k in range(n):
        a = a0 + 360.0 * k / n
        x0, y0 = _pt(cx, cy, r0, a)
        x1, y1 = _pt(cx, cy, r1, a)
        tilt_box(g, col, (x0, y0, z0), (x1, y1, z1), w, w)


def bands(g, cx, cy, z0, r0, r1, h, cols, steps=10):
    """A tapering round tower in bands of paint, one band a storey. Returns the radius at a height."""
    n = float(len(cols))
    for k, c in enumerate(cols):
        a = r0 + (r1 - r0) * k / n
        b = r0 + (r1 - r0) * (k + 1) / n
        g.cyl(c, (cx, cy, z0 + h * k / n), a, h / n, steps=steps, top_r=b)

    def rad(z):
        return r0 + (r1 - r0) * (z - z0) / float(h)
    return rad


def win(g, cx, cy, r, a, z, w=70.0, h=110.0, pane=INK, sill=W, glow=0.0, d=16.0):
    """A window on a round wall of radius r, looking out along angle a (-90 = the road)."""
    x, y = _pt(cx, cy, r, a)
    g.box(pane, (x, y, z), (w, d, h), rot=(0, a + 90.0, 0), glow=glow)
    if sill:
        x, y = _pt(cx, cy, r + d * 0.25, a)
        g.box(sill, (x, y, z - h / 2.0 - d * 0.4), (w + d * 1.5, d * 1.25, d * 0.75), rot=(0, a + 90.0, 0))


def gallery(g, cx, cy, z, r_in, r_out, n=8, deck=W, rail=INK, rail_h=95.0, drop=110.0, w=8.0, uprights=True, a0=0.0):
    """A gallery all round a tower at height z: deck, rail, uprights, and brackets under every second corner."""
    g.cyl(deck, (cx, cy, z - 2.5 * w), r_out, 2.5 * w, steps=n, rot=(0, a0, 0))
    ring(g, rail, cx, cy, z + rail_h, r_out - w, n, w, w, a0)
    if uprights:
        posts(g, rail, cx, cy, z, r_out - w, n, w, rail_h, a0)
    struts(g, rail, cx, cy, z - 2.5 * w - drop, r_in, z - 2.5 * w, r_out - 2.0 * w, max(n // 2, 3), 1.5 * w, a0)


def lamp_room(sc, g, cx, cy, z, r, h, glow=0.6, n=8, turn=0.05, louvre=True, cap=INK, ball=RED, w=10.0):
    """The lamp room: a sill, fixed mullions (and a fixed louvre band), a cone roof with a ball; inside, the lens: one
    even drum of steady light on a carriage that turns slowly. Returns the top."""
    g.cyl(INK, (cx, cy, z), r + w, 2.5 * w, steps=n)
    lens = sc.part(P.motion(base=(cx, cy), travel=(-1.0, turn)))
    lens.cyl(CREAM, (cx, cy, z + 2.5 * w), r * 0.58, h - 5.0 * w, steps=6, glow=glow)
    posts(lens, TEAL, cx, cy, z + 2.5 * w, r * 0.62, 3, w * 1.2, h - 5.0 * w)
    posts(g, INK, cx, cy, z + 2.5 * w, r, n, w, h - 2.5 * w)
    if louvre:
        ring(g, INK, cx, cy, z + h * 0.84, r, n, w * 0.6, h * 0.2)
    g.cyl(INK, (cx, cy, z + h - 1.5 * w), r + w, 1.5 * w, steps=n)
    rh = r * 1.3
    g.cyl(cap, (cx, cy, z + h), r + 3.0 * w, rh, steps=10, top_r=0.5 * w)
    g.sphere(ball, (cx, cy, z + h + rh + 1.2 * w), 2.0 * w, steps=(3, 6))
    return z + h + rh + 3.2 * w


def vane(sc, x, y, z, s=1.0, rate=0.07):
    """A weather vane: an arrow that swings slowly about its rod."""
    g = sc.still()
    g.cyl(INK, (x, y, z), 4.0 * s, 86.0 * s, steps=5)
    v = sc.part(P.motion(swing=28.0, pivot=(x, y, z + 78.0 * s), axis=P.UP, rate=rate))
    v.box(INK, (x + 6.0 * s, y, z + 78.0 * s), (96.0 * s, 5.0 * s, 9.0 * s))
    v.box(RED, (x - 42.0 * s, y, z + 78.0 * s), (26.0 * s, 5.0 * s, 30.0 * s))
    v.box(INK, (x + 54.0 * s, y, z + 78.0 * s), (18.0 * s, 5.0 * s, 18.0 * s), rot=(45.0, 0, 0))


def jar(g, x, y, z, s=1.0, col=CREAM, stop=INK):
    """An oil jar: pale, round-shouldered, stoppered (never orange, never green)."""
    g.cyl(col, (x, y, z), 18.0 * s, 26.0 * s, steps=6, top_r=28.0 * s)
    g.cyl(col, (x, y, z + 26.0 * s), 28.0 * s, 34.0 * s, steps=6, top_r=13.0 * s)
    g.cyl(stop, (x, y, z + 60.0 * s), 15.0 * s, 9.0 * s, steps=6)


def lantern(g, x, y, z, s=1.0, glow=0.6):
    """A hanging lantern, its top at z: a hanger, an ink cap, a cream glass (steady)."""
    t = max(6.0 * s, 4.5)
    g.box(INK, (x, y, z - 8.0 * s), (t, t, 16.0 * s))
    g.cyl(INK, (x, y, z - 30.0 * s), 24.0 * s, 16.0 * s, steps=6, top_r=4.0 * s)
    g.box(CREAM, (x, y, z - 52.0 * s), (28.0 * s, 28.0 * s, 44.0 * s), glow=glow)
    g.box(INK, (x, y, z - 77.0 * s), (32.0 * s, 32.0 * s, t))


def buoy_up(g, x, y, z, cols=(RED, W, RED), top='ball', s=1.0):
    """A marker buoy stood on end: a fat banded drum on its ballast skirt, a cage and a top mark. 2.2 to 2.9 m."""
    g.cyl(INK, (x, y, z), 40.0 * s, 30.0 * s, steps=8, top_r=62.0 * s)
    for k, c in enumerate(cols):
        g.cyl(c, (x, y, z + (30.0 + 44.0 * k) * s), 62.0 * s, 44.0 * s, steps=8)
    zt = z + (30.0 + 44.0 * len(cols)) * s
    g.cyl(cols[-1], (x, y, zt), 62.0 * s, 26.0 * s, steps=8, top_r=34.0 * s)
    posts(g, INK, x, y, zt + 26.0 * s, 26.0 * s, 4, 7.0 * s, 70.0 * s, 45.0)
    g.cyl(INK, (x, y, zt + 96.0 * s), 32.0 * s, 8.0 * s, steps=8)
    if top == 'ball':
        g.sphere(W, (x, y, zt + 128.0 * s), 24.0 * s, steps=(3, 6))
    else:
        g.cyl(TEAL, (x, y, zt + 104.0 * s), 28.0 * s, 52.0 * s, steps=6, top_r=3.0 * s)


def door(g, r, z, w=92.0, h=204.0, col=RED):
    """A door on the front (-Y) of a round wall of radius r, standing on z: ink frame, a small cone canopy."""
    g.box(INK, (0, -r + 6.0, z + h / 2.0 + 6.0), (w + 26.0, 30.0, h + 12.0))
    g.box(col, (0, -r - 10.0, z + h / 2.0), (w, 8.0, h))
    g.box(BRASS, (w * 0.3, -r - 15.0, z + h * 0.5), (8.0, 6.0, 8.0))
    g.cyl(INK, (0, -r - 4.0, z + h + 14.0), w * 0.95, 44.0, steps=6, top_r=4.0)


# ------------------------------------------------------------------------------------------ buildings
def lamp_tower_a(sc):
    """The striped lamp tower: six storeys, a gallery on brackets, the lamp room, a cone roof. 21 m."""
    g = sc.still()
    g.cyl(TEAL, (0, 0, -180), 340, 240, steps=10)                                  # the stone drum, 1.8 m of footing
    rad = bands(g, 0, 0, 60, 280, 200, 1440, (W, RED, W, RED, W, RED))
    door(g, 272, 60)
    g.box(ROCK_L, (0, -385, 20), (180, 90, 44))
    for k, a in enumerate((-90, -52, -128, -90, -52)):
        z = 60 + 240 * (k + 1) + 125
        win(g, 0, 0, rad(z) - 4, a, z)
    win(g, 0, 0, rad(905) - 4, 0, 905)
    win(g, 0, 0, rad(905) - 4, 180, 905)
    gallery(g, 0, 0, 1500, 200, 310)
    top = lamp_room(sc, g, 0, 0, 1500, 150, 250, 0.6)
    vane(sc, 0, 0, top - 12)
    jar(g, 130, -292, 60)
    jar(g, 178, -262, 60, 0.75, W)
    g.box(INK, (-120, -262, 250), (8, 26, 8))
    lantern(g, -120, -278, 250, 0.7, 0.6)


def lamp_tower_b(sc):
    """The tall lamp tower: eight storeys on a battered base, a stair turret, a keeper's balcony half way. 26 m."""
    g = sc.still()
    g.cyl(TEAL, (0, 0, -180), 310, 300, steps=10, top_r=268)
    rad = bands(g, 0, 0, 120, 250, 165, 1920, (RED, W, RED, W, RED, W, RED, W))
    door(g, 243, 120)
    g.box(ROCK_L, (0, -350, 40), (170, 110, 80))
    g.cyl(W, (222, 70, -100), 86, 1000, steps=8)                                   # the stair turret
    g.cyl(RED, (222, 70, 800), 89, 70, steps=8)
    g.cyl(INK, (222, 70, 900), 108, 150, steps=8, top_r=4)
    for z in (330, 620):
        g.box(INK, (222, -14, z), (22, 12, 84))
    zb = 1080.0                                                                    # the keeper's balcony
    rb = rad(zb)
    g.box(W, (0, -rb - 36, zb - 8), (230, 120, 16))
    g.box(RED, (0, -rb - 36, zb - 20), (238, 128, 8))
    g.box(INK, (0, -rb - 92, zb + 88), (230, 8, 8))
    for x in (-111, 0, 111):
        g.box(INK, (x, -rb - 92, zb + 44), (8, 8, 88))
    for x in (-80, 80):
        tilt_box(g, INK, (x, -rb + 12, zb - 110), (x, -rb - 82, zb - 20), 12, 12)
    win(g, 0, 0, rb - 6, -90, zb + 100, 78, 190, sill=None)
    for z, a in ((485, -128), (725, -52), (1445, -52), (1685, -90), (1925, -128)):
        win(g, 0, 0, rad(z) - 4, a, z, 62, 104)
    gallery(g, 0, 0, 2040, 165, 272)
    top = lamp_room(sc, g, 0, 0, 2040, 132, 240, 0.6)
    vane(sc, 0, 0, top - 12)
    jar(g, -150, -262, 120, 0.8)


def twin_towers(sc):
    """Two lamp towers, five storeys and seven, joined by a gallery on brackets with a lantern hung under it. 14 m long."""
    g = sc.still()
    g.box(TEAL, (0, 0, -60), (1420, 560, 240))
    for x, n, hz in ((-430, 5, 1150), (430, 7, 1610)):
        cols = tuple((RED, W)[(k + (0 if x < 0 else 1)) % 2] for k in range(n))
        rad = bands(g, x, 0, 60, 230, 170, hz, cols, steps=8)
        g.box(INK, (x, -226, 172), (96, 26, 214))
        g.box(RED, (x, -240, 166), (74, 8, 194))
        for k in range(2, n, 2):
            z = 60 + 230 * k + 120
            win(g, x, 0, rad(z) - 6, -90, z, 60, 100, sill=None, d=22.0)
        zt = 60 + hz
        gallery(g, x, 0, zt, 170, 270, n=6, uprights=False)
        top = lamp_room(sc, g, x, 0, zt, 124, 220, 0.6, n=6, louvre=False)
        if x > 0:
            vane(sc, x, 0, top - 12)
    zg = 830.0                                                                     # the gallery between them
    g.box(W, (0, 0, zg - 8), (480, 130, 16))
    g.box(RED, (0, 0, zg - 20), (488, 138, 8))
    for y in (-60, 60):
        g.box(INK, (0, y, zg + 90), (480, 8, 8))
        for x in (-236, 0, 236):
            g.box(INK, (x, y, zg + 45), (8, 8, 90))
        for sx in (-1, 1):
            tilt_box(g, INK, (sx * 236, y * 0.7, zg - 130), (sx * 110, y * 0.7, zg - 22), 12, 12)
    lantern(g, 0, 0, zg - 24, 0.9, 0.6)


def cottage(sc):
    """A keeper's cottage: a round gable, a barrel roof, red door and shutters, a striped awning, the washing out."""
    g = sc.still()
    g.box(ROCK_L, (0, 0, -70), (800, 640, 160))
    g.box(W, (0, 0, 160), (700, 560, 300))
    g.box(RED, (0, 0, 42), (706, 566, 64))
    rod(g, INK, (0, -268, 310), (0, 268, 310), 372, steps=12)                      # the barrel roof
    rod(g, W, (0, -279, 310), (0, -262, 310), 338, steps=12)                       # the round gables
    rod(g, W, (0, 262, 310), (0, 279, 310), 338, steps=12)
    g.box(RED, (0, -281, 324), (672, 6, 26))
    rod(g, INK, (0, -286, 480), (0, -276, 480), 62, steps=8)                       # the gable's round window
    rod(g, CREAM, (0, -290, 480), (0, -284, 480), 40, steps=8, glow=0.5)
    g.box(RED, (-150, -283, 116), (104, 8, 212))
    g.box(INK, (-150, -284, 230), (130, 10, 14))
    g.box(ROCK_L, (-150, -345, 2), (170, 60, 16))
    for k in range(4):                                                             # the awning over the door
        x = -150 + (k - 1.5) * 38
        tilt_box(g, (RED, W)[k % 2], (x, -282, 266), (x, -376, 238), 38, 6)
    for x in (-222, -78):
        g.box(INK, (x, -372, 122), (6, 6, 228))
    g.box(INK, (150, -283, 172), (180, 8, 120))                                    # the window, its shutters, its box
    g.box(W, (150, -286, 172), (8, 6, 120))
    for x in (38, 262):
        g.box(RED, (x, -284, 172), (40, 8, 126))
    g.box(TEAL, (150, -298, 98), (204, 30, 26))
    for x in (90, 150, 210):
        g.sphere(W, (x, -298, 122), 15, steps=(3, 5))
    g.cyl(W, (220, 120, 560), 38, 80, steps=8)                                     # the chimney
    g.cyl(RED, (220, 120, 640), 40, 36, steps=8)
    g.cyl(W, (220, 120, 676), 38, 54, steps=8)
    g.cyl(INK, (220, 120, 730), 50, 14, steps=8)
    g.box(INK, (-60, -288, 238), (8, 20, 8))
    lantern(g, -60, -300, 238, 0.6, 0.6)
    g.box(TEAL, (-290, -318, 52), (130, 40, 8))                                    # a bench
    for x in (-345, -235):
        g.box(INK, (x, -318, 28), (8, 36, 44))
    jar(g, 300, -325, 10, 0.9)
    for x in (430, 660):                                                           # the washing line
        g.box(INK, (x, -100, 90), (8, 8, 270))
    g.box(INK, (545, -100, 218), (236, 4, 4))
    c = sc.part(P.motion(swing=7.0, pivot=(545, -100, 218), axis=0.0, rate=0.3))
    c.box(W, (475, -100, 174), (60, 4, 84))
    c.box(TEAL, (548, -100, 182), (50, 4, 68))
    c.box(CREAM, (622, -100, 170), (56, 4, 92))
    vane(sc, 0, -200, 676, 0.8)


def pilot_house(sc):
    """The pilot house: an eight-sided look-out on a stone drum, a gallery all round, signal shapes on the roof."""
    g = sc.still()
    A0 = 22.5
    g.cyl(TEAL, (0, 0, -180), 430, 440, steps=8, rot=(0, A0, 0))
    g.cyl(RED, (0, 0, 204), 436, 40, steps=8, rot=(0, A0, 0))
    gallery(g, 0, 0, 282, 430, 545, n=8, a0=A0, drop=120.0)
    g.cyl(W, (0, 0, 280), 360, 300, steps=8, rot=(0, A0, 0))
    for k in range(8):
        a = 45.0 * k
        if k == 6:                                                                 # the front: the door
            x, y = _pt(0, 0, 330, a)
            g.box(RED, (x, y, 392), (100, 14, 220))
            g.box(INK, (x, y + 2, 508), (124, 14, 12))
            continue
        x, y = _pt(0, 0, 330, a)
        g.box(INK, (x, y, 452), (196, 14, 150), rot=(0, a + 90.0, 0))
        x, y = _pt(0, 0, 334, a)
        g.box(W, (x, y, 452), (8, 14, 150), rot=(0, a + 90.0, 0))
    g.cyl(RED, (0, 0, 572), 458, 12, steps=8, rot=(0, A0, 0))
    g.cyl(INK, (0, 0, 584), 452, 230, steps=8, top_r=72, rot=(0, A0, 0))
    g.cyl(W, (0, 0, 814), 70, 26, steps=8)                                         # the cupola: a steady light
    g.cyl(CREAM, (0, 0, 840), 64, 44, steps=8, glow=0.5)
    g.cyl(INK, (0, 0, 884), 96, 110, steps=8, top_r=5)
    for k in range(7):                                                             # the stair up to the gallery
        top = 40.0 * (k + 1)
        g.box(W, (-390 + k * 60, -590, (top - 80) / 2.0), (60, 110, top + 80))
    tilt_box(g, INK, (-420, -642, 100), (0, -642, 380), 8, 8)
    for x in (-420, -210, 0):
        g.box(INK, (x, -642, 50 + (x + 420) / 1.5), (8, 8, 100))
    g.cyl(W, (0, 0, 990), 6, 340, steps=6)                                         # the signal yard
    g.box(INK, (0, 0, 1230), (310, 6, 6))
    for x in (-125, 125):
        g.box(INK, (x, 0, 1195), (4, 4, 70))
    g.sphere(W, (-125, 0, 1134), 32, steps=(4, 8))
    g.cyl(TEAL, (125, 0, 1100), 32, 62, steps=6, top_r=3)
    t = sc.part(P.motion(swing=22.0, pivot=(420, -250, 400), axis=P.UP, rate=0.06))   # a spyglass on the gallery
    rod(t, BRASS, (372, -250, 392), (486, -250, 412), 9, steps=6)
    rod(t, INK, (486, -250, 412), (500, -250, 414), 13, steps=6)
    for a in (90, 210, 330):
        x, y = _pt(420, -250, 34, a)
        tilt_box(g, INK, (x, y, 282), (420, -250, 396), 6, 6)
    jar(g, 120, -400, 282, 0.7)
    g.box(INK, (-150, -340, 520), (8, 30, 8))
    lantern(g, -150, -356, 520, 0.6, 0.6)


def bell_tower(sc):
    """The bell tower: four banded legs leaning in, an open top, a big bell that swings slowly. 14.5 m."""
    g = sc.still()
    g.box(TEAL, (0, 0, -70), (620, 620, 220))
    lo, mid, hi = 250.0, 207.0, 164.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, RED, (sx * lo, sy * lo, 40), (sx * mid, sy * mid, 380), 34, 34)
            tilt_box(g, W, (sx * mid, sy * mid, 380), (sx * hi, sy * hi, 720), 30, 30)
    cs = ((-1, -1), (1, -1), (1, 1), (-1, 1))
    for k in range(4):
        a, b = cs[k], cs[(k + 1) % 4]
        tilt_box(g, INK, (a[0] * mid, a[1] * mid, 380), (b[0] * mid, b[1] * mid, 380), 12, 12)
        tilt_box(g, INK, (a[0] * mid, a[1] * mid, 390), (b[0] * hi, b[1] * hi, 706), 10, 10)
        tilt_box(g, INK, (b[0] * mid, b[1] * mid, 390), (a[0] * hi, a[1] * hi, 706), 10, 10)
    g.box(RED, (0, 0, 716), (444, 444, 10))
    g.box(W, (0, 0, 728), (430, 430, 16))
    for k in range(4):
        a, b = cs[k], cs[(k + 1) % 4]
        tilt_box(g, INK, (a[0] * 205, a[1] * 205, 826), (b[0] * 205, b[1] * 205, 826), 8, 8)
    for sx, sy in cs:
        g.box(W, (sx * 180, sy * 180, 915), (22, 22, 360))
    g.cyl(RED, (0, 0, 1090), 300, 14, steps=8, rot=(0, 22.5, 0))
    g.cyl(INK, (0, 0, 1104), 292, 250, steps=8, top_r=5, rot=(0, 22.5, 0))
    g.sphere(RED, (0, 0, 1368), 20, steps=(3, 6))
    vane(sc, 0, 0, 1380)
    g.box(INK, (0, 0, 1076), (380, 20, 20))                                        # the beam the bell hangs from
    b = sc.part(P.motion(swing=13.0, pivot=(0, 0, 1062), axis=0.0, rate=0.22))
    b.box(INK, (0, 0, 1058), (120, 26, 22))
    b.cyl(BRASS, (0, 0, 872), 150, 26, steps=10, top_r=138)
    b.cyl(BRASS, (0, 0, 898), 132, 122, steps=10, top_r=66)
    b.cyl(BRASS, (0, 0, 1020), 66, 28, steps=10, top_r=22)
    b.box(INK, (0, 0, 925), (6, 6, 110))
    b.sphere(INK, (0, 0, 866), 22, steps=(3, 6))
    for x in (-30, 30):                                                            # the ladder at the back
        tilt_box(g, INK, (x, 330, 40), (x, 222, 720), 6, 6)
    for k in range(7):
        f = (k + 0.5) / 7.0
        g.box(INK, (0, 330 - 108 * f, 40 + 680 * f), (60, 5, 5))
    g.box(INK, (0, 0, 708), (6, 6, 16))
    lantern(g, 0, 0, 700, 0.8, 0.6)
    g.box(TEAL, (-200, -250, 62), (150, 40, 8))                                    # the ringers' bench
    for x in (-262, -138):
        g.box(INK, (x, -250, 49), (8, 36, 22))


def chart_house(sc):
    """The chart house: a round white drum under a glass dome on ink ribs, a wind rose on the wall, an armillary turning."""
    g = sc.still()
    R0 = (0, 15.0, 0)
    g.cyl(TEAL, (0, 0, -180), 520, 240, steps=12, rot=R0)
    g.cyl(W, (0, 0, 60), 470, 330, steps=12, rot=R0)
    g.cyl(RED, (0, 0, 330), 476, 60, steps=12, rot=R0)
    g.cyl(INK, (0, 0, 390), 490, 26, steps=12, rot=R0)
    g.sphere(GLASS, (0, 0, 416), 430, scale=(1, 1, 0.72), steps=(5, 10), glow=0.3)
    for k in range(8):                                                             # the dome's ribs
        a = 45.0 * k + 22.5
        p0, p1, p2 = _pt(0, 0, 436, a), _pt(0, 0, 356, a), _pt(0, 0, 222, a)
        tilt_box(g, INK, (p0[0], p0[1], 420), (p1[0], p1[1], 604), 12, 12)
        tilt_box(g, INK, (p1[0], p1[1], 604), (p2[0], p2[1], 694), 12, 12)
    g.cyl(INK, (0, 0, 686), 232, 12, steps=8)
    g.cyl(INK, (0, 0, 712), 120, 34, steps=8)
    g.cyl(INK, (0, 0, 746), 5, 60, steps=5)
    s = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, 0.04)))                    # the armillary
    s.sphere(TEAL, (0, 0, 850), 44, steps=(4, 8))
    s.box(BRASS, (0, 0, 850), (150, 7, 7))
    s.box(BRASS, (0, 0, 850), (7, 150, 7), rot=(0, 0, 30.0))
    s.box(BRASS, (0, 0, 850), (7, 7, 150))
    g.box(W, (0, -500, 125), (360, 110, 450))                                      # the porch, its round hood
    rod(g, INK, (0, -552, 350), (0, -448, 350), 172, steps=10)
    g.box(RED, (0, -557, 176), (110, 6, 226))
    rod(g, CREAM, (0, -560, 350), (0, -553, 350), 60, steps=8, glow=0.5)
    g.box(ROCK_L, (0, -590, 20), (200, 70, 80))
    for x in (-150, 150):
        g.box(INK, (x, -560, 282), (8, 16, 8))
        lantern(g, x, -570, 282, 0.6, 0.6)
    xr, yr = _pt(0, 0, 458, 300.0)                                                 # the wind rose (a picture, no letters)
    g.box(INK, (xr, yr, 200), (170, 5, 170), rot=(45.0, 30.0, 0))
    for p, c, ln, wd in ((0, CREAM, 190, 22), (90, CREAM, 190, 22), (45, RED, 120, 18), (135, RED, 120, 18)):
        x, y = _pt(0, 0, 462, 300.0)
        g.box(c, (x, y, 200), (ln, 6, wd), rot=(p, 30.0, 0))
    for a in (240.0, 210.0, 330.0, 0.0, 180.0):                                    # portholes
        p0, p1, p2 = _pt(0, 0, 448, a), _pt(0, 0, 460, a), _pt(0, 0, 465, a)
        rod(g, INK, (p0[0], p0[1], 210), (p1[0], p1[1], 210), 50, steps=8)
        rod(g, CREAM, (p1[0], p1[1], 210), (p2[0], p2[1], 210), 30, steps=6, glow=0.4)
    g.box(TEAL, (226, -436, 92), (60, 60, 64))                                     # a tub of rolled charts
    for dx, dy in ((-12, 0), (10, 8), (4, -14)):
        rod(g, CREAM, (226 + dx, -436 + dy, 100), (226 + dx * 3, -436 + dy * 3, 210), 7, steps=5)


def buoy_shed(sc):
    """The buoy shed: a long barrel-roofed store on its quay, sliding doors open, marker buoys stood on end outside."""
    g = sc.still()
    g.box(ROCK_L, (0, 0, -70), (1560, 760, 200))
    g.box(W, (0, 20, 190), (1400, 560, 320))
    g.box(RED, (0, 20, 72), (1406, 566, 84))
    rod(g, INK, (-720, 20, 350), (720, 20, 350), 300, steps=12)
    for sx in (-1, 1):
        rod(g, W, (sx * 690, 20, 350), (sx * 712, 20, 350), 266, steps=12)
        g.box(RED, (sx * 714, 20, 470), (6, 380, 40))
        rod(g, INK, (sx * 706, 20, 400), (sx * 718, 20, 400), 44, steps=8)
    g.box(INK, (-250, -261, 172), (420, 8, 276))                                   # the dark doorway, the doors run back
    for x in (-530, 30):
        g.box(TEAL, (x, -268, 172), (200, 10, 284))
        tilt_box(g, W, (x - 90, -275, 40), (x + 90, -275, 304), 5, 14)
    g.box(INK, (-250, -272, 322), (800, 8, 12))
    g.box(INK, (440, -262, 200), (170, 8, 112))
    g.box(W, (440, -266, 200), (8, 6, 112))
    for x in (348, 532):
        g.box(RED, (x, -263, 200), (30, 8, 118))
    for x in (-660, 170):
        g.box(INK, (x, -272, 300), (8, 28, 8))
        lantern(g, x, -288, 300, 0.7, 0.6)
    for x in (-380, 380):                                                          # roof vents
        g.cyl(W, (x, 20, 630), 30, 62, steps=6)
        g.cyl(INK, (x, 20, 692), 50, 46, steps=6, top_r=4)
    vane(sc, 0, 20, 648)
    tilt_box(g, INK, (-250, -250, 345), (-250, -372, 345), 14, 14)                 # the hoist beam and its tackle
    g.box(INK, (-250, -360, 318), (5, 5, 50))
    g.box(BRASS, (-250, -360, 286), (18, 8, 18))
    buoy_up(g, 250, -310, 30, (RED, W, RED), 'ball')
    buoy_up(g, 420, -326, 30, (W, TEAL, W), 'cone', 0.86)
    buoy_up(g, 574, -306, 30, (RED, W), 'ball', 0.78)
    buoy_up(g, -250, -326, 30, (W, RED, W), 'cone', 0.9)
    g.cyl(INK, (-640, -330, 30), 62, 22, steps=8)                                  # a coil of chain
    g.cyl(INK, (-640, -330, 52), 46, 20, steps=8)
    g.box(TEAL, (680, -320, 62), (120, 46, 8))                                     # a bench of top marks
    for x in (634, 726):
        g.box(INK, (x, -320, 46), (8, 40, 30))
    g.sphere(W, (654, -320, 92), 26, steps=(3, 6))
    g.cyl(TEAL, (712, -320, 66), 26, 50, steps=6, top_r=3)


# ------------------------------------------------------------------------------------------ props
def buoy_side(sc):
    """A marker buoy laid on its side on two chocks for painting: red and white bands, cage and top mark. 3.3 m long."""
    g = sc.still()
    for x in (-70, 70):
        g.box(TEAL, (x, 0, 10), (30, 170, 44))
    z = 100.0
    rod(g, INK, (-150, 0, z), (-118, 0, z), 78, steps=8)
    for k, c in enumerate((RED, W, RED)):
        rod(g, c, (-118 + 56 * k, 0, z), (-62 + 56 * k, 0, z), 78, steps=8)
    g.cyl(RED, (50, 0, z), 78, 34, steps=8, top_r=42, rot=(-90.0, 0, 0))
    for a in (45, 135, 225, 315):
        y, zz = _pt(0, z, 30, a)
        rod(g, INK, (84, y, zz), (150, y, zz), 4, steps=4)
    rod(g, INK, (150, 0, z), (158, 0, z), 38, steps=8)
    g.sphere(W, (186, 0, z), 26, steps=(3, 6))
    g.cyl(W, (-200, 40, -4), 20, 34, steps=6)                                      # a paint pot, a brush across it
    g.cyl(INK, (-200, 40, 30), 22, 4, steps=6)
    g.box(BRASS, (-200, 40, 38), (50, 6, 5))
    tilt_box(g, INK, (-250, -40, 0), (-232, -40, 236), 6, 6)                       # a boat hook leaning on the cage
    tilt_box(g, INK, (-232, -40, 236), (-214, -40, 226), 5, 5)


def bollard_row(sc):
    """Four mooring bollards on a kerb, a chain sagging from one to the next. Low: 0.95 m."""
    g = sc.still()
    g.box(ROCK_L, (0, 0, -22), (640, 56, 60))
    xs = (-270, -90, 90, 270)
    for x in xs:
        g.cyl(W, (x, 0, 8), 16, 44, steps=8)
        g.cyl(RED, (x, 0, 52), 17.5, 20, steps=8)
        g.cyl(W, (x, 0, 72), 16, 8, steps=8)
        g.cyl(INK, (x, 0, 80), 24, 13, steps=8, top_r=18)
    for k in range(3):
        a, b = xs[k], xs[k + 1]
        m = (a + b) / 2.0
        tilt_box(g, INK, (a + 16, 0, 64), (m, 0, 40), 6, 6)
        tilt_box(g, INK, (m, 0, 40), (b - 16, 0, 64), 6, 6)


def bracket_lamp(sc):
    """A lamp post with two bracket arms and a lantern on each: steady light. 5 m."""
    g = sc.still()
    g.cyl(TEAL, (0, 0, -120), 30, 150, steps=8)
    g.cyl(RED, (0, 0, 30), 15, 60, steps=8)
    g.cyl(W, (0, 0, 90), 15, 60, steps=8)
    g.cyl(INK, (0, 0, 150), 9, 340, steps=6)
    g.sphere(RED, (0, 0, 500), 15, steps=(3, 6))
    for sx in (-1, 1):
        tilt_box(g, INK, (0, 0, 372), (sx * 70, 0, 452), 7, 7)
        g.box(INK, (sx * 38, 0, 456), (84, 7, 7))
        lantern(g, sx * 74, 0, 452, 1.0, 0.7)
    g.box(INK, (0, 0, 250), (64, 6, 6))                                            # the lamplighter's ladder rest


def signal_mast(sc):
    """A signal mast: banded, a yard, shapes hoisted on its halyards (balls and cones: nothing that spells). 11.5 m."""
    g = sc.still()
    g.cyl(TEAL, (0, 0, -120), 42, 170, steps=8)
    g.cyl(RED, (0, 0, 50), 16, 250, steps=8, top_r=14)
    g.cyl(W, (0, 0, 300), 14, 420, steps=8, top_r=11)
    g.cyl(RED, (0, 0, 720), 11, 200, steps=8, top_r=9)
    g.cyl(W, (0, 0, 920), 9, 190, steps=8, top_r=6)
    g.cyl(INK, (0, 0, 1110), 14, 34, steps=6, top_r=2)
    g.box(INK, (0, 0, 820), (500, 8, 8))
    for sx in (-1, 1):
        tilt_box(g, INK, (sx * 296, 0, 20), (0, 0, 700), 5, 5)                     # stays, along the road
        g.box(TEAL, (sx * 300, 0, 4), (40, 40, 32))
    h = sc.part(P.motion(swing=2.5, pivot=(0, 0, 820), axis=90.0, rate=0.2))       # what is hoisted sways a little
    h.box(INK, (-200, 0, 700), (4, 4, 240))
    h.sphere(W, (-200, 0, 720), 44, steps=(4, 8))
    h.cyl(TEAL, (-200, 0, 590), 42, 78, steps=6, top_r=3)
    h.box(INK, (200, 0, 690), (4, 4, 260))
    h.cyl(W, (200, 0, 716), 40, 60, steps=6, top_r=3)
    h.cyl(W, (200, 0, 656), 3, 60, steps=6, top_r=40)
    h.sphere(TEAL, (200, 0, 572), 40, steps=(4, 8))


def oil_jar_rack(sc):
    """A rack of oil jars under a striped awning: pale jars, two shelves, a funnel and a can. Low: 1.85 m."""
    g = sc.still()
    g.box(ROCK_L, (0, 0, -22), (250, 96, 60))
    for x in (-108, 108):
        for y in (-34, 34):
            g.box(INK, (x, y, 86), (7, 7, 158))
    g.box(TEAL, (0, 0, 22), (228, 80, 8))
    g.box(TEAL, (0, 0, 100), (228, 80, 8))
    for k in range(4):
        jar(g, -81 + 54 * k, 0, 26, 0.92, (CREAM, W, CREAM, TEAL)[k])
    for k in range(5):
        jar(g, -88 + 44 * k, 0, 104, 0.66, (W, CREAM, TEAL, CREAM, W)[k])
    for k in range(6):
        x = -100 + 40 * k
        tilt_box(g, (RED, W)[k % 2], (x, 44, 183), (x, -52, 168), 40, 5)
    g.cyl(W, (160, -20, 8), 15, 36, steps=6)                                       # the can and its funnel
    g.cyl(INK, (160, -20, 44), 5, 16, steps=5)
    g.cyl(BRASS, (-158, 10, 8), 4, 22, steps=5, top_r=20)


def ladder_bench(sc):
    """The lamp trimmers' bench: a step ladder, a bucket and a mop, a folded cloth, a spare lens pane. Low: 1.7 m."""
    g = sc.still()
    g.box(ROCK_L, (0, 0, -22), (420, 100, 60))
    g.box(TEAL, (-90, 0, 50), (170, 44, 8))
    g.box(TEAL, (-90, 22, 84), (170, 6, 34))
    for x in (-165, -15):
        g.box(INK, (x, 0, 28), (8, 40, 40))
        g.box(INK, (x, 22, 62), (6, 6, 76))
    g.box(W, (-120, -4, 60), (44, 30, 12))
    g.cyl(BRASS, (-52, -4, 54), 11, 16, steps=6)
    for sy in (-1, 1):                                                             # the step ladder
        tilt_box(g, INK, (60, sy * 24, 8), (112, sy * 24, 166), 6, 6)
        tilt_box(g, INK, (164, sy * 24, 8), (112, sy * 24, 166), 6, 6)
    for k in range(4):
        f = (k + 0.6) / 4.6
        g.box(W, (60 + 52 * f, 0, 8 + 158 * f), (16, 50, 5))
    g.box(INK, (112, 0, 168), (30, 60, 6))
    g.cyl(W, (-196, -10, 8), 17, 30, steps=6, top_r=21)                            # a bucket, a mop in it
    g.cyl(RED, (-196, -10, 28), 21, 6, steps=6)
    tilt_box(g, INK, (-196, -10, 14), (-178, 20, 150), 5, 5)
    g.box(CREAM, (188, 14, 60), (6, 60, 104), rot=(0, 0, 0))                       # a spare pane in its frame
    g.box(INK, (188, 14, 8), (10, 66, 8))
    g.box(INK, (188, 14, 114), (10, 66, 6))


def spyglass(sc):
    """A pilot's spyglass on a tripod, on a stepped stand with a rail: it pans slowly. 2.7 m."""
    g = sc.still()
    g.box(W, (0, 0, 5), (170, 120, 170))                                           # the stand: 0.9 m, footing under it
    g.box(RED, (0, 0, 84), (176, 126, 12))
    for k in range(3):
        top = 30.0 * (k + 1)
        g.box(W, (-175 + 30 * k, 0, (top - 80) / 2.0), (30, 110, top + 80))
        g.box(RED, (-175 + 30 * k, 0, top - 3), (32, 112, 6))
    for x, y in ((-80, 55), (80, 55), (80, -55)):
        g.box(INK, (x, y, 135), (6, 6, 90))
    g.box(INK, (0, 55, 180), (166, 6, 6))
    g.box(INK, (80, 0, 180), (6, 116, 6))
    for a in (90, 210, 330):
        x, y = _pt(0, 0, 44, a)
        tilt_box(g, INK, (x, y, 90), (0, 0, 226), 7, 7)
    g.cyl(INK, (0, 0, 222), 9, 16, steps=6)
    t = sc.part(P.motion(swing=24.0, pivot=(0, 0, 240), axis=P.UP, rate=0.06))
    rod(t, BRASS, (-88, 0, 238), (10, 0, 252), 9, steps=6)
    rod(t, W, (10, 0, 252), (84, 0, 262), 12, steps=6)
    rod(t, INK, (84, 0, 262), (96, 0, 264), 16, steps=6)
    rod(t, INK, (-96, 0, 237), (-88, 0, 238), 11, steps=6)


def gallery_walk(sc):
    """A length of gallery on three banded posts with cone caps: brackets, rails, lanterns hung under it, a stair. 7.6 m."""
    g = sc.still()
    for x in (-520, 0, 520):
        g.cyl(TEAL, (x, 0, -150), 46, 190, steps=8)
        g.cyl(RED, (x, 0, 40), 30, 180, steps=8)
        g.cyl(W, (x, 0, 220), 30, 190, steps=8)
        g.cyl(W, (x, 0, 410), 22, 250, steps=8)
        g.cyl(RED, (x, 0, 640), 26, 14, steps=8)
        g.cyl(INK, (x, 0, 654), 72, 108, steps=8, top_r=4)
        for sx in (-1, 1):
            tilt_box(g, INK, (x + sx * 28, 0, 296), (x + sx * 150, 0, 400), 12, 12)
    g.box(W, (0, 0, 410), (1300, 150, 16))
    g.box(RED, (0, 0, 398), (1310, 158, 10))
    for sy in (-1, 1):
        g.box(INK, (0, sy * 70, 512), (1300, 8, 8))
        for k in range(9):
            x = -640 + k * 160
            if sy > 0 and x == 0:
                continue                                                           # the stair's head
            g.box(INK, (x, sy * 70, 465), (8, 8, 94))
    for x in (-260, 260):
        lantern(g, x, 0, 398, 0.9, 0.6)
    for k in range(7):                                                             # the stair, down the back
        top = 410.0 - 58.0 * (k + 1)
        g.box(W, (60, 110 + 60 * k, (top - 80) / 2.0), (110, 60, top + 80))
    tilt_box(g, INK, (118, 80, 505), (118, 500, 100), 8, 8)


# ------------------------------------------------------------------------------------------ the hero
def great_lamp(sc):
    """THE GREAT LAMP: a banded lighthouse on a rock, 81 m in all. Ten bands of red and white, a keeper's gallery half
    way, the great gallery on brackets, a lit watch room, and the lamp room: fixed ink shutters, and behind them the
    lens, one even drum of steady light on a carriage that turns once in half a minute. Nothing sweeps, nothing blinks.
    A keeper's house and a small harbour light on the rock's shoulders."""
    g = sc.still()
    g.sphere(ROCK, (0, 0, 150), 1700, scale=(1.0, 0.9, 0.6), steps=(5, 10))
    g.sphere(ROCK_D, (-1300, 500, 0), 1500, scale=(1.0, 0.9, 0.5), steps=(4, 8))
    g.sphere(ROCK_L, (1400, -300, -50), 1300, scale=(1.1, 0.8, 0.45), steps=(4, 8))
    g.sphere(ROCK_L, (-500, -1300, -80), 1000, scale=(1.2, 0.8, 0.4), steps=(4, 7))
    g.sphere(ROCK_D, (700, 1200, -60), 1200, scale=(1.0, 1.0, 0.5), steps=(4, 7))
    g.cyl(TEAL, (0, 0, 780), 1150, 360, steps=14)                                  # the platform, to z = 1140
    ring(g, W, 0, 0, 1250, 1120, 14, 14, 14)
    posts(g, W, 0, 0, 1140, 1120, 14, 14, 110)
    g.cyl(W, (0, 0, 1140), 860, 90, steps=14)
    rad = bands(g, 0, 0, 1140, 800, 470, 5200, (RED, W) * 5, steps=14)             # the shaft, to z = 6340
    g.box(W, (0, -790, 1350), (330, 60, 450))                                      # the door
    g.box(RED, (0, -806, 1330), (210, 40, 380))
    g.cyl(INK, (0, -800, 1580), 230, 110, steps=6, top_r=8)
    for k in range(8):
        z = 1140 + 520 * k + 790
        a = -90 + ((k % 3) - 1) * 40
        win(g, 0, 0, rad(z) - 10, a, z, 150, 240, pane=INK, d=40.0)
        x, y = _pt(0, 0, rad(z) + 12, a)
        g.box(CREAM, (x, y, z), (96, 12, 180), rot=(0, a + 90.0, 0), glow=0.5)
    gallery(g, 0, 0, 3740, rad(3740), 790, n=10, rail_h=120.0, drop=260.0, w=12.0, uprights=False)
    gallery(g, 0, 0, 6340, 470, 900, n=14, rail_h=130.0, drop=420.0, w=14.0)
    g.cyl(INK, (0, 0, 6340), 540, 330, steps=14)                                   # the watch room and its lit band
    g.cyl(CREAM, (0, 0, 6440), 546, 120, steps=14, glow=0.5)
    g.cyl(W, (0, 0, 6670), 610, 40, steps=14)
    ring(g, INK, 0, 0, 6810, 590, 14, 10, 10)
    lens = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, 0.033)))                # the lens, turning behind the shutters
    lens.cyl(CREAM, (0, 0, 6740), 290, 620, steps=6, glow=0.9)
    posts(lens, TEAL, 0, 0, 6740, 302, 6, 30, 620)
    lens.box(TEAL, (0, 0, 6728), (640, 60, 24))
    lens.box(TEAL, (0, 0, 6728), (60, 640, 24))
    posts(g, INK, 0, 0, 6710, 430, 10, 44, 700)                                    # the fixed shutters
    ring(g, INK, 0, 0, 7290, 430, 10, 20, 240)
    ring(g, INK, 0, 0, 6770, 430, 10, 20, 120)
    g.cyl(INK, (0, 0, 7410), 470, 40, steps=14)
    g.cyl(RED, (0, 0, 7450), 560, 30, steps=14)
    g.cyl(INK, (0, 0, 7480), 540, 460, steps=14, top_r=40)
    g.sphere(RED, (0, 0, 7975), 60, steps=(4, 8))
    vane(sc, 0, 0, 8000, 3.0, 0.05)
    g.box(W, (1700, -500, 640), (520, 400, 360))                                   # the keeper's house on the shoulder
    g.box(RED, (1700, -500, 500), (526, 406, 60))
    rod(g, INK, (1430, -500, 820), (1970, -500, 820), 216, steps=10)
    for x0, x1 in ((1422, 1434), (1966, 1978)):
        rod(g, W, (x0, -500, 820), (x1, -500, 820), 190, steps=10)
    g.box(RED, (1610, -703, 600), (90, 8, 190))
    g.box(CREAM, (1800, -703, 650), (110, 8, 90), glow=0.5)
    tilt_box(g, W, (1450, -620, 560), (900, -760, 1130), 120, 20)                  # the stair up to the platform
    tilt_box(g, INK, (1450, -690, 650), (900, -830, 1220), 10, 10)
    bands(g, -1350, 350, 640, 130, 95, 800, (W, RED, W, RED), steps=8)             # the harbour light
    g.cyl(INK, (-1350, 350, 1440), 110, 16, steps=8)
    g.cyl(CREAM, (-1350, 350, 1456), 66, 90, steps=8, glow=0.7)
    g.cyl(INK, (-1350, 350, 1546), 100, 110, steps=8, top_r=5)


JOBS = [
    ('SM_S1D7_LampTower_A', lamp_tower_a, SMALL),
    ('SM_S1D7_LampTower_B', lamp_tower_b, SMALL),
    ('SM_S1D7_TwinTowers', twin_towers, SMALL),
    ('SM_S1D7_KeeperCottage', cottage, SMALL),
    ('SM_S1D7_PilotHouse', pilot_house, SMALL),
    ('SM_S1D7_BellTower', bell_tower, SMALL),
    ('SM_S1D7_ChartHouse', chart_house, SMALL),
    ('SM_S1D7_BuoyShed', buoy_shed, SMALL),
    ('SM_S1D7_BuoySide', buoy_side, STILL),
    ('SM_S1D7_BollardRow', bollard_row, STILL),
    ('SM_S1D7_BracketLamp', bracket_lamp, STILL),
    ('SM_S1D7_SignalMast', signal_mast, SMALL),
    ('SM_S1D7_OilJarRack', oil_jar_rack, STILL),
    ('SM_S1D7_LadderBench', ladder_bench, STILL),
    ('SM_S1D7_Spyglass', spyglass, SMALL),
    ('SM_S1D7_GalleryWalk', gallery_walk, STILL),
    ('SM_S1D7_GreatLamp', great_lamp, dict(far=True, grow=(60, 60, 60), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
