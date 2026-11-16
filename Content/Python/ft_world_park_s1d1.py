"""Theme Park, Stage 1, builder s1d1: district 1 "Front Gate and Midway" (0 to 2.22 km), SHOW STREET.
/Game/World/ThemePark/SM_S1D1_*. Run: import ft_world_park_s1d1 as K; K.run()

Tall narrow false-front show buildings (stepped and scalloped parapets, striped barrel awnings, a row of steady bulbs
on every cornice, onion caps), the street's furniture, and THE GREAT ORGAN. All of it is drawn here: no brand, no
real name, no words, no likeness, no outside asset. No clown, no balloon, nothing on a spring, no crate.
Every mesh is an ft_people.Scene (one section, M_People); movement is the builders' motion values only.
The placing rules are the block C:/ftwork/s1/L4_s1d1.inl.

  buildings (26 to 70 m)   BoxOffice, PennyArcade, MirrorMaze, ShootGallery, IceParlour, CostumeShop, PhotoStudio
  tall props (24 to 40 m)  TicketKiosk, BulbArch, PrizeRack, WeighChair, BannerMast, BulbPost
  low props (17 to 26 m)   Turnstiles, BarrelOrgan, PosterBoards, RingToss     (under 2 m, unlit, still, 0.9 m deep)
  hero                     GreatOrgan (46 m wide, 34 m tall: bellows pump, beaters beat, bells rock, the star turns)
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

CREAM, RED, AMBER, MAG, NAVY = '#fff1d6', '#ff2d3c', '#ffd23f', '#ff4fb8', '#2b2350'
GOLD, WHITE, BULB, PLUM, LAV = '#ffc93c', '#fff6ea', '#ffe28a', '#4a3a78', '#b9a4ff'
WOOD, WOOD_D, TIN, PEACH, SPRAY = '#9a6a44', '#6a4c36', '#a8b4d0', '#ffc59e', '#dff6ff'
G_WIN, G_SIGN, G_BULB = 0.9, 1.4, 2.0


# ------------------------------------------------------------------------------------------ the street's own parts
def xrod(g, col, x0, x1, y, z, r, steps=8, glow=0.0):
    rod(g, col, (x0, y, z), (x1, y, z), r, steps, glow)


def ydisc(g, col, x, y0, y1, z, r, steps=10, glow=0.0):
    """A disc facing the road: a cylinder along Y from y0 to y1."""
    rod(g, col, (x, y0, z), (x, y1, z), r, steps, glow)


def bulbs(g, x0, x1, y, z, n, s=26.0, col=BULB, glow=G_BULB):
    for k in range(n):
        g.box(col, (x0 + (x1 - x0) * k / float(max(n - 1, 1)), y, z), (s, s, s), glow=glow)


def ring_bulbs(g, x, y, z, r, n, s=22.0, half=False, turn=0.0):
    """Bulbs round a circle in the ground plane (half: only the road's side)."""
    for k in range(n):
        a = math.radians(turn + (180.0 + 180.0 * k / float(n - 1) if half else 360.0 * k / float(n)))
        g.box(BULB, (x + math.cos(a) * r, y + math.sin(a) * r, z), (s, s, s), glow=G_BULB)


def arc_bulbs(g, x, y, z, r, n, s=26.0, a0=0.0, a1=180.0):
    """Bulbs round an arch that faces the road."""
    for k in range(n):
        a = math.radians(a0 + (a1 - a0) * k / float(n - 1))
        g.box(BULB, (x + math.cos(a) * r, y, z + math.sin(a) * r), (s, s, s), glow=G_BULB)


def body(g, col, x, W, D, H, y=0.0, foot=150.0):
    g.box(col, (x, y, (H - foot) / 2.0), (W, D, H + foot))


def cornice(g, x, W, D, z, n=7, col=AMBER, y=0.0):
    """The top of every show front: a band wider than the wall, a row of steady bulbs on it."""
    g.box(col, (x, y - 12, z + 18), (W + 70, D + 64, 36))
    if n:
        bulbs(g, x - W / 2.0 + 20, x + W / 2.0 - 20, y - D / 2.0 - 46, z + 50, n)


def barrel(g, x0, x1, yf, z, r, n, cols=(RED, CREAM)):
    """A striped barrel awning along the front: half of it stands out of the wall."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        xrod(g, cols[k % 2], x0 + k * w, x0 + (k + 1) * w, yf, z, r)
    g.box(cols[1], ((x0 + x1) / 2.0, yf - r * 0.5, z - r * 0.62), (x1 - x0, r, 10))


def scallops(g, col, x0, x1, yf, z, n, t=50.0, trim=None):
    """A scalloped parapet: a row of half discs standing on the cornice."""
    r = (x1 - x0) / (2.0 * n)
    for k in range(n):
        xc = x0 + r * (2 * k + 1)
        ydisc(g, col, xc, yf, yf + t, z, r, steps=10)
        if trim:
            ydisc(g, trim, xc, yf - 8, yf, z + r * 0.18, r * 0.42, steps=8)
    return r


def steps_top(g, x, W, yf, z, cols=(CREAM, RED, AMBER), t=60.0):
    """A stepped parapet: three shrinking blocks."""
    zz = z
    for k, (f, h) in enumerate(((0.74, 110.0), (0.46, 100.0), (0.2, 80.0))):
        g.box(cols[k % len(cols)], (x, yf + t / 2.0, zz + h / 2.0), (W * f, t, h))
        zz += h
    return zz


def arch_win(g, x, z0, w, h, yf, pane=BULB, glow=G_WIN, frame=NAVY):
    """A round-topped window: z0 is its sill, h the height to the spring of the arch."""
    g.box(frame, (x, yf - 2, z0 + h / 2.0), (w + 28, 10, h))
    ydisc(g, frame, x, yf - 7, yf + 3, z0 + h, (w + 28) / 2.0, steps=8)
    g.box(pane, (x, yf - 8, z0 + h / 2.0 + 6), (w, 8, h - 12), glow=glow)
    ydisc(g, pane, x, yf - 12, yf - 4, z0 + h, w / 2.0, steps=8, glow=glow)
    g.box(CREAM, (x, yf - 12, z0 - 8), (w + 44, 26, 16))


def door(g, x, w, h, yf, lit=True):
    g.box(NAVY, (x, yf - 2, h / 2.0), (w, 10, h))
    ydisc(g, NAVY, x, yf - 7, yf + 3, h, w / 2.0, steps=8)
    if lit:
        g.box(BULB, (x, yf - 8, h * 0.5), (w * 0.56, 8, h * 0.78), glow=G_WIN)
    g.box(AMBER, (x, yf - 16, 8), (w + 60, 40, 16))


def onion(g, x, y, z, r, cap=MAG, drum=CREAM):
    """An onion cap: a drum, a swelling, a point, a gold ball. Returns its top."""
    g.cyl(drum, (x, y, z), r * 0.78, r * 0.42, steps=8)
    g.cyl(AMBER, (x, y, z + r * 0.42), r * 0.86, r * 0.1, steps=8)
    g.cyl(cap, (x, y, z + r * 0.52), r * 0.72, r * 0.5, steps=8, top_r=r * 1.12)
    g.cyl(cap, (x, y, z + r * 1.02), r * 1.12, r * 1.4, steps=8, top_r=9.0)
    g.cyl(GOLD, (x, y, z + r * 2.36), 8, r * 0.5, steps=5)
    g.sphere(GOLD, (x, y, z + r * 2.94), max(r * 0.15, 14.0), steps=(3, 5))
    return z + r * 3.1


def star_pts(cx, cz, R, r=None, n=5, turn=90.0):
    r = r or R * 0.45
    out = []
    for k in range(2 * n):
        a = math.radians(turn + k * 180.0 / n)
        rad = R if k % 2 == 0 else r
        out.append((cx + rad * math.cos(a), cz + rad * math.sin(a)))
    return out


def star(g, col, x, y0, y1, z, R, glow=0.0):
    g.side(col, star_pts(x, z, R), y0, y1, glow=glow)


def pennant(sc, g, x, y, z, col, h=260.0, L=170.0, phase=0.0):
    """A thin mast from z and a pennant that swings round it."""
    g.cyl(TIN, (x, y, z), 7, h, steps=5)
    g.sphere(GOLD, (x, y, z + h + 10), 14, steps=(3, 5))
    p = sc.part(P.motion(swing=16, pivot=(x, y, z + h - 50), axis=P.UP, rate=0.45, phase=phase))
    p.side(col, [(x + 6, z + h - 95), (x + L, z + h - 50), (x + 6, z + h - 5)], y - 4, y + 4)


def pilasters(g, x0, x1, yf, H, n=5, w=56.0):
    """Striped corner posts either side of a front."""
    for x in (x0, x1):
        g.box(CREAM, (x, yf - 8, H / 2.0), (w, 30, H))
        for k in range(n):
            g.box(RED, (x, yf - 10, H * (k + 0.5) / n), (w + 6, 30, H / (2.2 * n)))


# ------------------------------------------------------------------------------------------ buildings
def box_office(sc):                             # 8.3 x 6.4 m, 17.2 m: the box office and its onion-capped ticket turret
    g = sc.still()
    W, D, H, X = 620.0, 560.0, 980.0, 105.0
    yf = -D / 2.0
    body(g, CREAM, X, W, D, H)
    g.box(NAVY, (X, 0, 30), (W + 24, D + 24, 60))
    for k in (-1, 0, 1):                        # three ticket windows under one awning
        arch_win(g, X + k * 185, 150, 110, 150, yf)
        g.box(AMBER, (X + k * 185, yf - 26, 140), (150, 44, 14))
    barrel(g, X - 290, X + 290, yf, 430, 64, 7)
    g.box(AMBER, (X, yf - 4, 520), (W, 14, 28))
    for k in (-1, 1):
        arch_win(g, X + k * 180, 600, 130, 200, yf)
    ydisc(g, MAG, X, yf - 12, yf, 720, 84, steps=10)                  # the medallion: a gold star on magenta
    star(g, GOLD, X, yf - 22, yf - 12, 720, 62, glow=G_SIGN)
    cornice(g, X, W, D, H, 8)
    top = steps_top(g, X, W, yf, H + 36)
    pennant(sc, g, X, yf + 30, top, MAG, phase=0.2)
    tx, ty, R, TH = -265.0, -150.0, 150.0, 1250.0                     # the turret on the corner
    g.cyl(CREAM, (tx, ty, -150), R, TH + 150, steps=10)
    for z in (190, 470, 750, 1030):
        g.cyl(RED, (tx, ty, z), R + 5, 90, steps=10)
    g.box(NAVY, (tx, ty - R + 2, 250), (120, 14, 150))
    g.box(BULB, (tx, ty - R - 2, 250), (84, 10, 110), glow=G_WIN)
    g.box(AMBER, (tx, ty - R - 16, 168), (150, 40, 14))
    g.box(NAVY, (tx, ty - R + 2, 890), (80, 14, 170))
    g.box(BULB, (tx, ty - R - 2, 890), (50, 10, 136), glow=G_WIN)
    g.cyl(AMBER, (tx, ty, TH), R + 26, 26, steps=10)
    ring_bulbs(g, tx, ty, TH - 26, R + 24, 9, half=True)
    onion(g, tx, ty, TH + 26, R)


def penny_arcade(sc):                           # 8.4 x 6.4 m, 13.6 m: a penny arcade under a striped barrel roof
    g = sc.still()
    W, D, H = 800.0, 580.0, 780.0
    yf = -D / 2.0
    body(g, CREAM, 0, W, D, H)
    g.box(NAVY, (0, 0, 30), (W + 24, D + 24, 60))
    n = 6                                        # the roof: a barrel end on to the road, striped along its length
    for k in range(n):
        rod(g, (RED, CREAM)[k % 2], (0, yf + k * D / n, H), (0, yf + (k + 1) * D / n, H), W / 2.0, 14)
    ydisc(g, CREAM, 0, yf - 10, yf, H, W / 2.0 - 46, steps=14)        # the gable: a sunburst
    for k in range(7):
        a = math.radians(18 + k * 24)
        tilt_box(g, AMBER if k % 2 == 0 else MAG, (math.cos(a) * 120, yf - 16, H + 36 + math.sin(a) * 120),
                 (math.cos(a) * 322, yf - 16, H + 36 + math.sin(a) * 322), 12, 44)
    ydisc(g, MAG, 0, yf - 24, yf - 10, H + 36, 112, steps=10)
    ydisc(g, GOLD, 0, yf - 34, yf - 24, H + 36, 78, steps=10, glow=G_SIGN)          # the penny
    star(g, NAVY, 0, yf - 42, yf - 34, H + 36, 44)
    arc_bulbs(g, 0, yf - 26, H + 36, W / 2.0 - 30, 13, a0=4, a1=176)
    g.box(AMBER, (0, yf - 14, H + 10), (W + 60, 60, 44))
    bulbs(g, -W / 2.0 + 20, W / 2.0 - 20, yf - 48, H - 26, 9)
    door(g, 0, 210, 330, yf)
    for k in (-1, 1):
        arch_win(g, k * 265, 190, 130, 220, yf)
        barrel(g, k * 265 - 110, k * 265 + 110, yf, 560, 54, 4)
        g.box(MAG, (k * 265, yf - 30, 120), (150, 60, 120))            # a penny machine under each window
        g.box(AMBER, (k * 265, yf - 62, 150), (90, 8, 36))
        ydisc(g, GOLD, k * 265 + 96, yf - 40, yf - 30, 150, 22, steps=6)
    barrel(g, -130, 130, yf, 500, 60, 4, (MAG, CREAM))
    pilasters(g, -W / 2.0 + 28, W / 2.0 - 28, yf, H, 5)
    pennant(sc, g, -W / 2.0 + 40, 0, H + 150, AMBER, h=330, phase=0.5)
    pennant(sc, g, W / 2.0 - 40, 0, H + 150, MAG, h=330, phase=0.9)


def mirror_maze(sc):                            # 7.9 x 6.2 m, 17 m: three tall arched mirrors for windows
    g = sc.still()
    W, D, H = 740.0, 560.0, 1280.0
    yf = -D / 2.0
    body(g, CREAM, 0, W, D, H)
    g.box(NAVY, (0, 0, 30), (W + 24, D + 24, 60))
    for k in (-1, 0, 1):
        x = k * 205
        arch_win(g, x, 440, 140, 560, yf, pane=SPRAY, glow=0.35)
        tilt_box(g, WHITE, (x - 44, yf - 15, 560), (x + 34, yf - 15, 700), 8, 14, glow=0.6)    # the glint on the glass
        tilt_box(g, WHITE, (x - 20, yf - 15, 800), (x + 46, yf - 15, 920), 8, 10, glow=0.6)
    door(g, 0, 190, 280, yf)
    for k in (-1, 1):                           # the way in: mirror panels set zigzag, each a different tint
        for j, (c, yaw) in enumerate(((LAV, 24), (SPRAY, -24))):
            g.box(c, (k * (170 + j * 110), yf - 34, 170), (104, 12, 250), rot=(0, yaw * k, 0), glow=0.35)
            g.box(GOLD, (k * (170 + j * 110), yf - 34, 302), (112, 16, 14), rot=(0, yaw * k, 0))
    barrel(g, -330, 330, yf, 372, 58, 8, (MAG, CREAM))
    pilasters(g, -W / 2.0 + 28, W / 2.0 - 28, yf, H, 7)
    cornice(g, 0, W, D, H, 9)
    r = scallops(g, CREAM, -W / 2.0 + 60, W / 2.0 - 60, yf, H + 36, 5, trim=MAG)
    zc = H + 36 + r + 120                         # the crest: a diamond mirror in a gold frame
    g.box(GOLD, (0, yf + 26, zc), (230, 20, 230), rot=(45, 0, 0))
    g.box(SPRAY, (0, yf + 16, zc), (170, 14, 170), rot=(45, 0, 0), glow=0.5)
    g.box(CREAM, (0, yf + 30, H + 36 + r * 0.5 + 30), (60, 30, r + 60))
    for k in (-1, 1):
        onion(g, k * (W / 2.0 - 34), yf + 40, H + 36, 56)
    pennant(sc, g, 0, yf + 30, zc + 150, MAG, h=200, L=150, phase=0.7)


def shoot_gallery(sc):                          # 8.5 x 6.6 m, 12.6 m: a hall, a scalloped parapet, targets going by
    g = sc.still()
    W, D, H = 820.0, 600.0, 720.0
    yf = -D / 2.0
    body(g, CREAM, 0, W, D, H)
    g.box(NAVY, (0, 0, 30), (W + 24, D + 24, 60))
    g.box(NAVY, (0, yf - 2, 265), (660, 10, 310))                         # the open front, dark inside
    g.box(PLUM, (0, yf - 5, 300), (620, 8, 16))                            # the rail the targets run on
    for k in range(8):                            # the counter, in stripes
        g.box((RED, CREAM)[k % 2], (-289 + k * 82.5, yf - 30, 62), (82.5, 60, 100))
    g.box(AMBER, (0, yf - 34, 118), (700, 76, 14))
    for k in range(5):                            # the back row stands still: gold stars
        star(g, GOLD, -240 + k * 120, yf - 14, yf - 6, 370, 26, glow=G_WIN)
    for k in range(4):                            # the front row goes by, plate after plate, and comes round again
        xk = -225 + k * 150
        t = sc.part(P.motion(pivot=(xk, yf - 14, 232), base=(xk, yf - 14), travel=(600.0, 0.07)))
        ydisc(t, CREAM, xk, yf - 18, yf - 8, 232, 34, steps=8)
        ydisc(t, RED, xk, yf - 24, yf - 18, 232, 20, steps=6)
        t.box(PLUM, (xk, yf - 12, 270), (8, 6, 44))
    barrel(g, -350, 350, yf, 446, 62, 9)
    for k in range(4):
        g.box(NAVY, (-270 + k * 180, yf - 2, 590), (96, 10, 120))
        g.box(BULB, (-270 + k * 180, yf - 6, 590), (68, 8, 92), glow=G_WIN)
    pilasters(g, -W / 2.0 + 28, W / 2.0 - 28, yf, H, 4)
    cornice(g, 0, W, D, H, 9)
    r = scallops(g, CREAM, -W / 2.0 + 30, W / 2.0 - 30, yf, H + 36, 4, trim=RED)
    zc = H + 36 + r + 150                         # the crest: a great bullseye
    g.box(CREAM, (0, yf + 30, H + 36 + r * 0.5 + 40), (70, 30, r + 80))
    for k, (c, rr) in enumerate(((RED, 170), (CREAM, 124), (RED, 80), (GOLD, 36))):
        ydisc(g, c, 0, yf + 24 - k * 8, yf + 44, zc, rr, steps=12, glow=(G_SIGN if k == 3 else 0.0))
    arc_bulbs(g, 0, yf + 4, zc, 196, 12, a0=0, a1=330)
    pennant(sc, g, -W / 2.0 + 60, yf + 40, H + 36, RED, h=420, phase=0.1)
    pennant(sc, g, W / 2.0 - 60, yf + 40, H + 36, AMBER, h=420, phase=0.6)


def ice_parlour(sc):                            # 8.2 x 7 m, 16.6 m: two storeys, a balcony, a soft ice on the roof
    g = sc.still()
    W, D, H, F = 760.0, 560.0, 980.0, 480.0
    yf = -D / 2.0
    body(g, CREAM, 0, W, D, H)
    g.box(NAVY, (0, 0, 30), (W + 24, D + 24, 60))
    g.box(NAVY, (-120, yf - 2, 235), (400, 10, 290))                      # the shop window: ices in a row
    g.box(BULB, (-120, yf - 6, 235), (364, 8, 254), glow=G_WIN)
    for k in range(4):
        x = -255 + k * 90
        g.cyl(AMBER, (x, yf - 20, 130), 8, 90, steps=6, top_r=30)
        for j, c in enumerate(((MAG, CREAM, PEACH, MAG)[k], (CREAM, MAG, MAG, PEACH)[k])):
            g.cyl(c, (x, yf - 20, 220 + j * 34), 32 - j * 9, 34, steps=6, top_r=22 - j * 12)
    door(g, 235, 150, 270, yf)
    barrel(g, -350, 350, yf, 420, 60, 8, (MAG, CREAM))
    g.box(AMBER, (0, yf - 70, F), (W + 40, 170, 24))                       # the balcony
    g.box(MAG, (0, yf - 148, F + 100), (W + 40, 12, 14))
    for k in range(9):
        g.box(CREAM, (-W / 2.0 - 12 + k * (W + 24) / 8.0, yf - 148, F + 56), (14, 12, 88))
    for k in (-1, 1):
        arch_win(g, k * 215, F + 40, 150, 250, yf)
        barrel(g, k * 215 - 120, k * 215 + 120, yf, F + 400, 50, 4)
        g.cyl(TIN, (k * 215, yf - 96, F + 12), 6, 150, steps=5)           # a little table and its parasol
        g.cyl(CREAM, (k * 215, yf - 96, F + 76), 34, 8, steps=6)
        g.cyl((MAG, RED)[k > 0], (k * 215, yf - 96, F + 150), 78, 46, steps=6, top_r=8)
    ydisc(g, MAG, 0, yf - 12, yf, F + 250, 60, steps=8)
    star(g, GOLD, 0, yf - 20, yf - 12, F + 250, 44, glow=G_SIGN)
    cornice(g, 0, W, D, H, 8)
    top = steps_top(g, 0, W, yf, H + 36, (CREAM, MAG, AMBER))
    z = top                                       # the soft ice: a wafer cone, four turns, a point
    g.cyl(AMBER, (0, yf + 40, z), 26, 210, steps=8, top_r=104)
    g.cyl(WOOD, (0, yf + 40, z + 196), 110, 16, steps=8)
    z += 212
    for k, (c, rr) in enumerate(((CREAM, 112), (MAG, 96), (CREAM, 76), (MAG, 54))):
        g.cyl(c, (0, yf + 40 + (k % 2) * 8, z), rr, 58, steps=8, top_r=rr * 0.74)
        z += 52
    g.cyl(CREAM, (0, yf + 44, z), 38, 80, steps=6, top_r=8)
    for k in (-1, 1):
        pennant(sc, g, k * (W / 2.0 - 50), yf + 40, H + 36, (MAG, AMBER)[k > 0], h=300, phase=0.3 + 0.25 * k)


def costume_shop(sc):                           # 7.9 x 6.2 m, 16.5 m: a tower of hat boxes for a chimney
    g = sc.still()
    W, D, H = 720.0, 560.0, 1040.0
    yf = -D / 2.0
    body(g, CREAM, 0, W, D, H)
    g.box(NAVY, (0, 0, 30), (W + 24, D + 24, 60))
    door(g, 0, 170, 290, yf)
    for k in (-1, 1):                           # two show windows, a dress form in each
        x = k * 225
        g.box(NAVY, (x, yf - 2, 240), (190, 10, 300))
        g.box(BULB, (x, yf - 6, 240), (158, 8, 268), glow=G_WIN)
        g.cyl((MAG, RED)[k > 0], (x, yf - 22, 120), 58, 130, steps=6, top_r=22)
        g.cyl((MAG, RED)[k > 0], (x, yf - 22, 250), 22, 60, steps=6, top_r=34)
        g.cyl(GOLD, (x, yf - 22, 310), 10, 24, steps=5)
        g.box(WOOD_D, (x, yf - 22, 112), (60, 30, 12))
    barrel(g, -330, 330, yf, 440, 58, 8, (MAG, CREAM))
    g.box(AMBER, (0, yf - 4, 540), (W, 14, 28))
    for k in (-1, 0, 1):
        arch_win(g, k * 210, 810, 110, 120, yf)
    zb = 680.0                                    # the sign: a great bow tie
    g.side(RED, [(-190, zb - 76), (-24, zb), (-190, zb + 76)], yf - 16, yf)
    g.side(RED, [(190, zb + 76), (24, zb), (190, zb - 76)], yf - 16, yf)
    g.box(MAG, (0, yf - 12, zb), (64, 24, 70))
    for k in (-1, 1):
        ydisc(g, GOLD, k * 300, yf - 10, yf, zb, 30, steps=8, glow=G_WIN)     # two gold buttons
    pilasters(g, -W / 2.0 + 28, W / 2.0 - 28, yf, H, 6)
    cornice(g, 0, W, D, H, 8)
    r = scallops(g, CREAM, -W / 2.0 + 40, 80, yf, H + 36, 3, trim=MAG)
    x, z = 215.0, H + 36.0                        # the chimney: five hat boxes and a top hat
    for k, (c, rr, hh) in enumerate(((MAG, 120, 100), (CREAM, 104, 90), (RED, 88, 84), (AMBER, 74, 74), (MAG, 60, 64))):
        xo = x + (-1) ** k * 10
        g.cyl(c, (xo, 30, z), rr, hh, steps=10)
        g.cyl((CREAM, NAVY, CREAM, NAVY, CREAM)[k], (xo, 30, z + hh - 22), rr + 7, 22, steps=10)
        g.box(GOLD, (xo, 30 - rr - 4, z + hh * 0.42), (rr * 0.5, 8, 12))
        z += hh
    g.cyl(NAVY, (x, 30, z), 66, 10, steps=10)
    g.cyl(NAVY, (x, 30, z + 10), 42, 84, steps=10, top_r=46)
    g.cyl(RED, (x, 30, z + 12), 44, 16, steps=10)
    xs, zs = -W / 2.0 + 70, H + 36 + r             # a cotton reel on the corner, a needle through it
    g.cyl(CREAM, (xs, yf + 50, zs), 50, 12, steps=8)
    g.cyl(RED, (xs, yf + 50, zs + 12), 36, 90, steps=8)
    g.cyl(CREAM, (xs, yf + 50, zs + 102), 50, 12, steps=8)
    tilt_box(g, TIN, (xs - 60, yf + 50, zs + 20), (xs + 70, yf + 50, zs + 230), 8, 8)
    pennant(sc, g, -40, yf + 40, H + 36 + r, AMBER, h=330, phase=0.4)


def photo_studio(sc):                           # 7.9 x 6.4 m, 15.4 m: a giant box camera on the roof, panning
    g = sc.still()
    W, D, H = 740.0, 560.0, 920.0
    yf = -D / 2.0
    body(g, CREAM, 0, W, D, H)
    g.box(NAVY, (0, 0, 30), (W + 24, D + 24, 60))
    door(g, -225, 150, 280, yf)
    g.box(NAVY, (110, yf - 2, 235), (400, 10, 290))                       # the shop window: three framed pictures
    g.box(BULB, (110, yf - 6, 235), (368, 8, 258), glow=G_WIN)
    for k, (c, c2) in enumerate(((MAG, PEACH), (LAV, CREAM), (RED, PEACH))):
        x = -10 + k * 120
        g.box(GOLD, (x, yf - 14, 240 - (k % 2) * 30), (96, 10, 130))
        g.box(c, (x, yf - 18, 240 - (k % 2) * 30), (72, 8, 104))
        g.box(c2, (x, yf - 22, 226 - (k % 2) * 30), (34, 6, 50))           # a plain shape for a sitter: nobody's face
    barrel(g, -350, 350, yf, 430, 60, 8)
    g.box(AMBER, (0, yf - 4, 530), (W, 14, 28))
    g.box(NAVY, (0, yf - 2, 715), (560, 10, 290))                         # the studio's north light
    g.box(BULB, (0, yf - 6, 715), (528, 8, 258), glow=G_WIN)
    for k in range(1, 4):
        g.box(NAVY, (-264 + k * 132, yf - 10, 715), (12, 8, 258))
    g.box(NAVY, (0, yf - 10, 715), (528, 8, 12))
    pilasters(g, -W / 2.0 + 28, W / 2.0 - 28, yf, H, 5)
    cornice(g, 0, W, D, H, 8)
    g.box(CREAM, (0, yf + 30, H + 86), (W * 0.74, 60, 100))
    for k in (-1, 1):                           # an empty gold frame stands at each end of the parapet
        x = k * (W / 2.0 - 90)
        g.box(GOLD, (x, yf + 30, H + 190), (130, 16, 14)); g.box(GOLD, (x, yf + 30, H + 310), (130, 16, 14))
        g.box(GOLD, (x - 58, yf + 30, H + 250), (14, 16, 130)); g.box(GOLD, (x + 58, yf + 30, H + 250), (14, 16, 130))
    zt = H + 136.0                                # the tripod stands still, the camera pans
    for a in (90, 210, 330):
        tilt_box(g, WOOD, (math.cos(math.radians(a)) * 190, 40 + math.sin(math.radians(a)) * 150, zt), (0, 40, zt + 210), 22, 22)
    g.cyl(WOOD_D, (0, 40, zt + 200), 44, 24, steps=8)
    c = sc.part(P.motion(swing=13, pivot=(0, 40, zt + 220), axis=P.UP, rate=0.11, phase=0.2))
    zc = zt + 224 + 120
    c.box(NAVY, (0, 90, zc), (290, 230, 240))
    c.box(GOLD, (0, 90, zc + 112), (300, 240, 16)); c.box(GOLD, (0, 90, zc - 112), (300, 240, 16))
    for k, s in enumerate((210, 176, 142)):       # the bellows
        c.box(PLUM if k % 2 == 0 else RED, (0, -45 - k * 44, zc), (s, 44, s))
    c.box(WOOD, (0, -190, zc), (150, 26, 150))
    rod(c, GOLD, (0, -203, zc), (0, -262, zc), 58, 10)
    rod(c, SPRAY, (0, -262, zc), (0, -270, zc), 44, 10, glow=0.5)
    c.box(GOLD, (96, 60, zc + 150), (60, 90, 44))                          # the finder
    c.box(TIN, (-170, 90, zc + 110), (14, 14, 240))                        # the flash pan on its arm
    c.box(AMBER, (-170, 70, zc + 240), (150, 70, 14))
    tilt_box(c, NAVY, (0, 205, zc + 110), (0, 290, zc - 150), 250, 14)     # the cloth
    pennant(sc, g, 0, yf + 30, H + 136, MAG, h=250, phase=0.8)


# ------------------------------------------------------------------------------------------ tall props (24 m and out)
def ticket_kiosk(sc):                           # 3.7 m across, 6.3 m: an eight-sided ticket kiosk under an onion cap
    g = sc.still()
    g.cyl(NAVY, (0, 0, -100), 140, 130, steps=8)
    g.cyl(CREAM, (0, 0, 30), 116, 260, steps=8)
    for z in (30, 96, 236):
        g.cyl(RED, (0, 0, z), 120, 44, steps=8)
    g.box(NAVY, (0, -112, 170), (120, 14, 100))
    g.box(BULB, (0, -116, 170), (88, 10, 72), glow=G_WIN)
    g.box(AMBER, (0, -132, 114), (150, 50, 12))
    rod(g, CREAM, (-150, -40, 150), (-118, -40, 150), 30, 8)             # a roll of tickets on the side, a strip hanging
    rod(g, RED, (-126, -40, 150), (-118, -40, 150), 34, 8)
    g.box(AMBER, (-160, -40, 96), (10, 22, 110))
    for k in range(4):
        g.box(RED, (-160, -40, 60 + k * 26), (12, 24, 6))
    g.cyl(AMBER, (0, 0, 290), 184, 16, steps=8)
    for k in range(8):                            # the valance: eight striped flaps
        a = math.radians(k * 45 + 22.5)
        g.box((RED, CREAM)[k % 2], (math.cos(a) * 168, math.sin(a) * 168, 268), (126, 10, 44), rot=(0, k * 45 + 112.5, 0))
    ring_bulbs(g, 0, 0, 316, 172, 8, s=22, turn=22.5)
    onion(g, 0, 0, 306, 108)
    g.box(NAVY, (150, -150, 50), (16, 16, 100))                           # a queue post and its rope
    g.sphere(GOLD, (150, -150, 110), 14, steps=(3, 5))
    tilt_box(g, RED, (150, -150, 96), (70, -128, 80), 6, 6)


def bulb_arch(sc):                              # 8.6 m wide, 9.5 m: a bulb arch over a side path, its star turning
    g = sc.still()
    S, PH, R = 380.0, 420.0, 380.0
    for k in (-1, 1):
        x = k * S
        g.box(NAVY, (x, 0, -20), (110, 80, 160))
        g.cyl(CREAM, (x, 0, 60), 30, PH - 60, steps=8)
        for j in range(4):
            g.cyl(RED, (x, 0, 100 + j * 86), 34, 40, steps=8)
        g.cyl(AMBER, (x, 0, PH), 44, 18, steps=8)
        g.cyl(MAG, (x + k * 40, 0, 60), 34, 46, steps=6, top_r=42)        # a tub of flowers at each foot
        g.sphere(RED, (x + k * 40, 0, 118), 34, scale=(1.0, 1.0, 0.6), steps=(3, 6))
    n = 12
    for k in range(n):                            # the arch itself, in two colours
        a0, a1 = math.radians(180.0 * k / n), math.radians(180.0 * (k + 1) / n)
        tilt_box(g, (AMBER, MAG)[k % 2], (math.cos(a0) * R, 0, PH + math.sin(a0) * R), (math.cos(a1) * R, 0, PH + math.sin(a1) * R), 30, 30)
    arc_bulbs(g, 0, -24, PH, R - 34, 15, s=26, a0=6, a1=174)
    for k in (-1, 1):                            # a swag of pennants under it
        for j in range(4):
            x = k * (70 + j * 76)
            g.side((RED, CREAM, MAG, AMBER)[j], [(x - 26, PH + 250 - j * 40), (x, PH + 196 - j * 40), (x + 26, PH + 250 - j * 40)], -4, 4)
    g.cyl(GOLD, (0, 0, PH + R + 10), 10, 60, steps=5)
    s = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, 0.1)))            # the star goes round and round on its pin
    star(s, GOLD, 0, -8, 8, PH + R + 140, 78, glow=G_SIGN)


def prize_rack(sc):                             # 5.8 x 2 m, 5.2 m: a prize rack of oversized plush stars and bears
    g = sc.still()
    for k in (-1, 1):
        g.box(NAVY, (k * 250, 40, 180), (26, 26, 560))
    g.box(PLUM, (0, 60, 240), (500, 10, 400))
    for z in (60, 190, 320):
        g.box(CREAM, (0, 0, z), (520, 130, 14))
    g.box(RED, (0, -40, 26), (520, 60, 52))
    barrel(g, -290, 290, -20, 450, 60, 8, (MAG, CREAM))
    bulbs(g, -260, 260, -84, 440, 7, s=22)
    cols = (MAG, CREAM, AMBER, LAV, RED, PEACH)
    for row, z in enumerate((67, 197, 327)):
        for k in range(4):
            x = -190 + k * 127 + (row % 2) * 16
            c = cols[(k + row * 2) % 6]
            if (k + row) % 2 == 0:                # a bear: a round body, a head, two ears, a pale muzzle
                g.sphere(c, (x, -10, z + 34), 36, steps=(3, 6))
                g.sphere(c, (x, -14, z + 86), 26, steps=(3, 6))
                for e in (-1, 1):
                    g.box(c, (x + e * 20, -14, z + 112), (16, 10, 16))
                    g.box(c, (x + e * 34, -22, z + 22), (18, 18, 22))
                g.box(WHITE, (x, -38, z + 82), (20, 10, 14))
            else:                                 # a plush star with two eyes
                star(g, c, x, -24, 4, z + 56, 54)
                for e in (-1, 1):
                    g.box(NAVY, (x + e * 12, -27, z + 60), (7, 6, 9))
    for k in (-1, 1):                            # a great star hangs at each end and sways
        h = sc.part(P.motion(swing=7, pivot=(k * 250, -30, 400), axis=0.0, rate=0.3, phase=0.25 + 0.25 * k))
        h.box(TIN, (k * 250, -30, 370), (6, 6, 60))
        star(h, (AMBER, MAG)[k > 0], k * 250, -42, -18, 290, 60)


def weigh_chair(sc):                            # 3 x 1.4 m, 4.9 m: a weighing chair under its dial; the needle swings
    g = sc.still()
    g.box(NAVY, (20, 0, -30), (300, 140, 100))
    g.box(AMBER, (20, 0, 24), (310, 150, 8))
    X = -70.0
    g.cyl(CREAM, (X, 10, 20), 22, 330, steps=8)
    for j in range(4):
        g.cyl(RED, (X, 10, 50 + j * 76), 26, 34, steps=8)
    ydisc(g, AMBER, X, -6, 30, 380, 98, steps=12)                          # the dial
    ydisc(g, CREAM, X, -16, -6, 380, 84, steps=12)
    for k in range(9):
        a = math.radians(200 - k * 27.5)
        g.box(NAVY, (X + math.cos(a) * 68, -19, 380 + math.sin(a) * 68), (9, 6, 9))
    n = sc.part(P.motion(swing=34, pivot=(X, -22, 380), axis=90.0, rate=0.33, phase=0.1))
    tilt_box(n, RED, (X, -24, 372), (X, -24, 446), 8, 8)
    ydisc(g, GOLD, X, -30, -16, 380, 12, steps=6)
    g.sphere(GOLD, (X, 10, 492), 16, steps=(3, 5))
    g.box(TIN, (10, 10, 318), (190, 10, 10))                               # the beam
    tilt_box(g, TIN, (X, 10, 250), (40, 10, 316), 8, 8)
    c = sc.part(P.motion(swing=3.5, pivot=(95, 10, 318), axis=90.0, rate=0.25, phase=0.6))
    for e in (-1, 1):
        c.box(TIN, (95 + e * 36, 10, 232), (6, 6, 170))
    c.box(MAG, (95, 0, 140), (96, 80, 14))
    c.box(CREAM, (95, 36, 196), (96, 12, 100))
    for e in (-1, 1):
        c.box(RED, (95 + e * 44, 0, 168), (10, 80, 10))
    for k, rr in enumerate((30, 24, 18)):         # the brass weights
        g.cyl(GOLD, (-10 + k * 4, -44, 28 + k * 22), rr, 22, steps=8)


def banner_mast(sc):                            # 2 m wide, 8 m: a striped mast, a picture board, a pennant, three bulbs
    g = sc.still()
    g.cyl(NAVY, (0, 0, -100), 44, 150, steps=8)
    g.cyl(AMBER, (0, 0, 50), 34, 14, steps=8)
    g.cyl(CREAM, (0, 0, 64), 15, 640, steps=6)
    for j in range(6):
        g.cyl(RED, (0, 0, 90 + j * 100), 18, 44, steps=6)
    g.box(TIN, (0, 0, 610), (220, 10, 10))
    b = sc.part(P.motion(swing=4.5, pivot=(0, 0, 606), axis=0.0, rate=0.28, phase=0.3))   # the board sways on its hooks
    for e in (-1, 1):
        b.box(TIN, (e * 70, 0, 590), (6, 6, 34))
    b.box(MAG, (0, 0, 470), (184, 8, 212))
    b.box(CREAM, (0, -6, 470), (156, 8, 184))
    star(b, AMBER, 0, -16, -10, 478, 58, glow=G_WIN)
    for k in range(3):
        b.box((RED, CREAM)[k % 2], (-52 + k * 52, -12, 392), (52, 6, 16))
    for a in (0, 120, 240):
        g.box(BULB, (math.cos(math.radians(a)) * 30, math.sin(math.radians(a)) * 30, 700), (20, 20, 20), glow=G_BULB)
    pennant(sc, g, 0, 0, 704, MAG, h=100, L=120, phase=0.5)


def bulb_post(sc):                              # 2.4 m across, 7.7 m: a bulb crown, a tub of flowers, a vane going round
    g = sc.still()
    g.cyl(NAVY, (0, 0, -100), 78, 170, steps=8)
    for k in range(4):
        a = math.radians(k * 90 + 45)
        g.box(CREAM, (math.cos(a) * 74, math.sin(a) * 74, 30), (40, 12, 76), rot=(0, k * 90 + 135, 0))
    for k in range(5):
        a = math.radians(k * 72)
        g.sphere((MAG, RED, CREAM, MAG, AMBER)[k], (math.cos(a) * 44, math.sin(a) * 44, 84), 26, scale=(1.0, 1.0, 0.7), steps=(3, 5))
    g.cyl(CREAM, (0, 0, 60), 16, 490, steps=6)
    for j in range(5):
        g.cyl(MAG, (0, 0, 120 + j * 86), 19, 40, steps=6)
    g.cyl(AMBER, (0, 0, 540), 66, 14, steps=8)
    ring_bulbs(g, 0, 0, 524, 62, 8, s=22)
    top = onion(g, 0, 0, 554, 50, cap=RED)
    v = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, 0.06)))            # the vane: a star and a tail, slowly round
    v.box(TIN, (0, 0, top + 16), (150, 8, 8))
    star(v, GOLD, 62, -5, 5, top + 16, 30)
    v.side(MAG, [(-78, top - 10), (-40, top + 16), (-78, top + 42)], -4, 4)


# ------------------------------------------------------------------------------------------ low props (17 m and out)
def turnstiles(sc):                             # 7 x 0.9 m, 1.3 m: a turnstile row
    g = sc.still()
    for k in range(7):
        g.box((RED, CREAM)[k % 2], (-300 + k * 100, 0, 3), (100, 90, 10))
    for k in range(5):
        x = -320 + k * 160
        g.box(CREAM, (x, 0, 54), (40, 76, 100))
        g.box(RED, (x, 0, 108), (48, 84, 12))
        g.box(AMBER, (x, -39, 70), (22, 6, 34))
        if k < 4:                                 # three arms on a hub between each pair of posts
            hx = x + 26
            rod(g, GOLD, (hx, 0, 84), (hx + 16, 0, 84), 14, 6)
            for a in (90, 210, 330):
                tilt_box(g, TIN, (hx + 16, 0, 84), (hx + 96, math.cos(math.radians(a)) * 36, 84 + math.sin(math.radians(a)) * 36), 7, 7)
    for x in (-340, 340):
        g.cyl(NAVY, (x, 0, 0), 12, 118, steps=6)
        g.sphere(GOLD, (x, 0, 126), 12, steps=(3, 5))


def barrel_organ(sc):                           # 2.6 x 0.9 m, 1.9 m: a barrel organ on a hand cart
    g = sc.still()
    for k in (-1, 1):
        rod(g, RED, (20, k * 34, 56), (20, k * 44, 56), 56, 10)
        rod(g, AMBER, (20, k * 30, 56), (20, k * 45, 56), 14, 6)
    g.box(WOOD, (0, 0, 72), (190, 64, 14))
    for k in (-1, 1):
        tilt_box(g, WOOD_D, (-90, k * 26, 72), (-128, k * 26, 96), 8, 8)
    g.box(WOOD_D, (-128, 0, 96), (8, 62, 8))
    g.box(WOOD_D, (84, 0, 36), (10, 10, 72))
    g.box(CREAM, (10, 0, 121), (150, 58, 84))                              # the case
    g.box(RED, (10, -31, 121), (118, 6, 56))
    g.box(AMBER, (10, 0, 166), (160, 64, 8))
    g.box(AMBER, (10, 0, 82), (160, 64, 6))
    star(g, GOLD, 10, -38, -32, 121, 20)
    for k in range(5):                            # the pelmet: a scalloped valance
        ydisc(g, MAG, -42 + k * 26, -36, -30, 160, 13, steps=6)
    for k in range(7):                            # the pipes on top
        g.cyl(GOLD if k % 2 == 0 else CREAM, (-44 + k * 18, 6, 170), 6.5, 19 - abs(k - 3) * 3.5, steps=5)
    g.box(TIN, (90, -10, 126), (12, 8, 8))                                  # the crank
    g.box(TIN, (96, -10, 114), (8, 8, 30))
    g.box(RED, (104, -10, 102), (16, 10, 10))


def poster_boards(sc):                          # 5.6 x 0.6 m, 1.75 m: picture boards between brass posts and ropes
    g = sc.still()
    for k in range(3):
        x = -180 + k * 180
        for e in (-1, 1):
            g.box(NAVY, (x + e * 54, 8, 80), (10, 10, 160))
        g.box(MAG, (x, 6, 163), (124, 14, 18))
        g.box(CREAM, (x, 0, 92), (104, 8, 124))
        if k == 0:                                # a star
            star(g, AMBER, x, -10, -4, 96, 38)
        elif k == 1:                              # a ticket: a stub, a dotted tear
            g.box(RED, (x, -7, 96), (80, 6, 44))
            for j in range(3):
                g.box(CREAM, (x + 16, -10, 82 + j * 14), (6, 6, 6))
            star(g, GOLD, x - 14, -13, -10, 96, 13)
        else:                                     # a striped tent
            g.side(RED, [(x - 40, 66), (x + 40, 66), (x, 132)], -10, -4)
            for j in (-1, 1):
                g.box(CREAM, (x + j * 14, -12, 80), (8, 6, 28))
    for x in (-270, -90, 90, 270):
        g.cyl(GOLD, (x, -20, 0), 6, 96, steps=5)
        g.sphere(GOLD, (x, -20, 102), 9, steps=(3, 5))
    for x0, x1 in ((-270, -234), (-126, -90), (90, 126), (234, 270)):
        tilt_box(g, RED, (x0, -20, 86), (x1, -20, 78) if x0 < 0 else (x1, -20, 86), 6, 6)


def ring_toss(sc):                              # 3.2 x 0.9 m, 1.55 m: a ring-toss table, a post of spare rings, a stool
    g = sc.still()
    for k in range(8):
        g.box((RED, CREAM)[k % 2], (-135 + k * 30, 0, 36), (30, 80, 72))
    g.box(NAVY, (-30, 0, 76), (252, 86, 8))
    for i in range(5):
        for j in range(2):
            x, y = -120 + i * 45, -20 + j * 40
            g.cyl(CREAM, (x, y, 80), 5.5, 32, steps=5)
            if (i + j) % 2 == 0:
                g.cyl((MAG, AMBER, RED)[(i + j) % 3], (x, y, 82 + (i % 2) * 8), 13, 6, steps=6)
    g.cyl(NAVY, (128, 10, 0), 9, 150, steps=6)                              # the post of spare rings
    for k in range(4):
        g.cyl((MAG, CREAM, RED, AMBER)[k], (128, 10, 60 + k * 20), 20, 8, steps=6)
    g.sphere(GOLD, (128, 10, 156), 11, steps=(3, 5))
    g.cyl(RED, (-150, -4, 0), 16, 44, steps=6, top_r=22)                    # the stool
    g.cyl(CREAM, (-150, -4, 44), 24, 8, steps=6)


# ------------------------------------------------------------------------------------------ the hero
def great_organ(sc):                            # THE GREAT ORGAN: 46 m wide, 34 m tall, 12.5 m deep
    g = sc.still()
    g.box(NAVY, (0, -60, -10), (4600, 1250, 380))                          # the plinth (2 m of footing)
    g.box(AMBER, (0, -60, 190), (4600, 1250, 24))
    for k in range(11):                           # its skirt, in stripes
        g.box((RED, CREAM)[k % 2], (-2000 + k * 400, -688, 90), (400, 10, 150))
    ZC, ZT = 200.0, 1080.0
    g.box(CREAM, (0, 50, (ZC + ZT) / 2.0), (4000, 800, ZT - ZC))           # the case
    for k in range(-4, 5):
        if k == 0:
            continue
        g.box(RED, (k * 400 + (200 if k < 0 else -200) + (-150 if k < 0 else 150), -352, 640), (290, 10, 560))
        g.box(MAG, (k * 400 + (200 if k < 0 else -200) + (-150 if k < 0 else 150), -358, 640), (170, 8, 420))
    g.box(AMBER, (0, 44, ZT + 20), (4080, 840, 40))
    bulbs(g, -1980, 1980, -390, ZT + 76, 17, s=62)
    bulbs(g, -2240, 2240, -690, 232, 13, s=56)
    g.box(NAVY, (0, -354, 560), (640, 12, 520))                            # the proscenium: a lit arch
    ydisc(g, NAVY, 0, -360, -348, 820, 320, steps=14)
    g.box(BULB, (0, -360, 540), (500, 10, 480), glow=G_WIN)
    ydisc(g, BULB, 0, -366, -354, 800, 250, steps=14, glow=G_WIN)
    arc_bulbs(g, 0, -372, 820, 350, 11, s=50, a0=8, a1=172)
    star(g, GOLD, 0, -384, -366, 600, 150, glow=G_SIGN)
    for k in (-1, 1):                            # the side towers under onion caps
        x = k * 2020
        g.box(CREAM, (x, 50, 1140), (520, 800, 1880))
        for j in range(5):
            g.box(RED, (x, 44, 500 + j * 340), (532, 812, 110))
        g.box(NAVY, (x, -352, 1500), (200, 10, 420))
        ydisc(g, NAVY, x, -358, -346, 1710, 100, steps=8)
        g.box(BULB, (x, -358, 1500), (140, 8, 380), glow=G_WIN)
        g.box(AMBER, (x, 44, 2100), (600, 870, 44))
        bulbs(g, x - 250, x + 250, -400, 2150, 5, s=58)
        onion(g, x, 50, 2122, 250)
    for i in range(-10, 11):                     # the front rank: cream and gold pipes rising to the middle
        x, h = i * 172.0, 1850.0 - abs(i) * 108.0
        c = CREAM if i % 2 == 0 else GOLD
        g.cyl(c, (x, -210, ZT + 40), 70, h, steps=6)
        g.box(NAVY, (x, -274, ZT + 250), (64, 14, 96))
    for i in range(-9, 10):                      # the rank behind, between them
        x, h = i * 172.0 + 86.0, 1560.0 - abs(i + 0.5) * 100.0
        g.cyl(AMBER if i % 2 == 0 else PEACH, (x, 40, ZT + 40), 66, h, steps=4)
    for i in range(-8, 9, 4):                    # five magenta shells across the pipes' feet
        ydisc(g, MAG, i * 172.0, -290, -270, ZT + 110, 190, steps=8)
    g.box(GOLD, (0, -282, ZT + 60), (3700, 30, 44))
    ZS = 2930.0                                   # the star on the tallest pipe goes round and round
    g.cyl(GOLD, (0, -210, ZS), 16, 60, steps=6)
    s = sc.part(P.motion(base=(0.0, -210.0), travel=(-1.0, 0.07)))
    star(s, GOLD, 0, -230, -190, ZS + 250, 220, glow=G_SIGN)
    star(s, AMBER, 0, -240, -180, ZS + 250, 96, glow=G_BULB)
    for k in (-1, 1):                            # the bellows pump, turn about
        x = k * 1500
        g.box(WOOD_D, (x, -520, 226), (760, 300, 52))
        for j in range(5):
            m = sc.part(P.motion(bob=-(22.0 + j * 24.0), rate=0.4, phase=0.25 + 0.25 * k))
            if j < 4:
                m.box((MAG, CREAM)[j % 2], (x, -520, 300 + j * 90), (700 - (j % 2) * 70, 260 - (j % 2) * 40, 60))
            else:
                m.box(RED, (x, -520, 664), (760, 300, 56))
                m.box(GOLD, (x, -676, 664), (300, 14, 34))
                m.box(WOOD_D, (x + k * 330, -520, 760), (30, 30, 150))
    for k in (-1, 1):                            # two bass drums; the beaters beat slowly
        x, zd = k * 820, 560.0
        rod(g, RED, (x, -560, zd), (x, -380, zd), 230, 12)
        rod(g, CREAM, (x, -574, zd), (x, -560, zd), 214, 12)
        rod(g, AMBER, (x, -566, zd), (x, -552, zd), 244, 12)
        b = sc.part(P.motion(swing=26, pivot=(x + k * 250, -600, zd + 330), axis=90.0, rate=0.5, phase=0.5 + 0.25 * k))
        tilt_box(b, WOOD, (x + k * 250, -600, zd + 330), (x + k * 60, -600, zd + 40), 30, 30)
        b.sphere(WHITE, (x + k * 46, -600, zd + 20), 56, steps=(3, 6))
        g.box(GOLD, (x + k * 250, -590, zd + 330), (60, 50, 60))
    for i in range(-2, 3):                       # five gold bells over the arch rock on their beam
        x = i * 250.0
        r = sc.part(P.motion(swing=11, pivot=(x, -420, 1010), axis=90.0, rate=0.6, phase=0.2 * (i + 2)))
        r.cyl(GOLD, (x, -420, 880), 86, 120, steps=6, top_r=36)
        r.box(NAVY, (x, -420, 866), (24, 24, 30))
    g.box(TIN, (0, -420, 1016), (1200, 20, 20))


# every low prop is saved with its true bounds (STILL); the masts and posts keep theirs tight (they stand at 25 m)
JOBS = [
    ('SM_S1D1_BoxOffice', box_office), ('SM_S1D1_PennyArcade', penny_arcade), ('SM_S1D1_MirrorMaze', mirror_maze),
    ('SM_S1D1_ShootGallery', shoot_gallery, dict(grow=(60, 60, 60), grow_down=(60, 60, 0))),
    ('SM_S1D1_IceParlour', ice_parlour), ('SM_S1D1_CostumeShop', costume_shop), ('SM_S1D1_PhotoStudio', photo_studio),
    ('SM_S1D1_TicketKiosk', ticket_kiosk, STILL),
    ('SM_S1D1_BulbArch', bulb_arch, dict(grow=(20, 80, 20), grow_down=(20, 80, 0))),
    ('SM_S1D1_PrizeRack', prize_rack, dict(grow=(30, 30, 10), grow_down=(30, 30, 0))),
    ('SM_S1D1_WeighChair', weigh_chair, dict(grow=(30, 30, 10), grow_down=(30, 30, 0))),
    ('SM_S1D1_BannerMast', banner_mast, dict(grow=(30, 30, 10), grow_down=(30, 30, 0))),
    ('SM_S1D1_BulbPost', bulb_post, dict(grow=(20, 20, 10), grow_down=(20, 20, 0))),
    ('SM_S1D1_Turnstiles', turnstiles, STILL), ('SM_S1D1_BarrelOrgan', barrel_organ, STILL),
    ('SM_S1D1_PosterBoards', poster_boards, STILL), ('SM_S1D1_RingToss', ring_toss, STILL),
    ('SM_S1D1_GreatOrgan', great_organ, dict(far=True, grow=(60, 260, 140), grow_down=(60, 260, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
