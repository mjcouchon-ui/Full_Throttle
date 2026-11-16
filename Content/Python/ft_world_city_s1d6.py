"""City Center, Stage 1, builder s1d6: district 6 "Penthouse Row" (6.29 to 7.66 km). /Game/World/CityCenter/SM_S1D6_*.
Run: import ft_world_city_s1d6 as K; K.run()

White streamlined apartment towers read as stacked rounded terraces: porthole windows, fin stair towers, roof pools,
glass penthouse lanterns. Palette: white, blush, pool cyan, silver, violet shadow bands, peach light.
Seven buildings (TerraceTower, FinTower, LanternTower, PoolBlock, WinterGarden, HatBox, Observatory), eight props
(Canopy, Luggage, Loungers, Piano, CakeFountain, LiftKiosk, PlungePool, Sunfin) and the hero landmark (Cascade).
Every piece is an ft_people.Scene: one material (M_People), one section, movement only in the vertex shader.
Placed by C:/ftwork/s1/L7_s1d6.inl. No figures, no animals, no words, nothing of anyone else's.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SMALL = dict(grow=(8, 8, 8), grow_down=(8, 8, 0))

WHITE, BLUSH, CYAN, SILVER, VIOLET, PEACH = '#fff6ea', '#f3c4dc', '#2ee6ff', '#c9c4d6', '#8662b8', '#ffc89a'
GLASS, NAVY, WARM_L, PINE, GREEN_D, WOOD = '#233a63', '#2b2350', '#fff1c0', '#2f6b52', '#24403a', '#9a6a44'
LEAF = '#3fae6a'                                  # the one hex not in the metro kit (the brief's own leaf green)


def stad(w, d, n=4, cx=0.0, cy=0.0):
    """A stadium (a rectangle with round ends), w along X and d along Y (w > d), counter-clockwise."""
    r = d / 2.0
    a = w / 2.0 - r
    pts = []
    for k in range(n + 1):
        t = math.radians(-90.0 + 180.0 * k / n)
        pts.append((cx + a + r * math.cos(t), cy + r * math.sin(t)))
    for k in range(n + 1):
        t = math.radians(90.0 + 180.0 * k / n)
        pts.append((cx - a + r * math.cos(t), cy + r * math.sin(t)))
    return pts


def halfdisc(cx, cz, r, n=6):
    """The upper half of a disc as (x, z) points, for Model.side."""
    return [(cx + r * math.cos(math.radians(180.0 * k / n)), cz + r * math.sin(math.radians(180.0 * k / n))) for k in range(n + 1)]


def port(g, x, y, z, r, col=CYAN, glow=0.6, ring=None, steps=6):
    """A porthole on a face that looks at the road (-Y)."""
    if ring:
        rod(g, ring, (x, y, z), (x, y - 5.0, z), r * 1.28, steps=steps)
    rod(g, col, (x, y - 2.0, z), (x, y - 9.0, z), r, steps=steps, glow=glow)


def sunburst(g, c, R, col, n=7, w=40.0, t=16.0, span=150.0, r0=0.0, glow=0.0):
    """Rays fanned in the XZ plane (seen from the road) from c: every other ray shorter. w: a ray's width, t: its thickness."""
    for k in range(n):
        a = math.radians(-span / 2.0 + span * k / (n - 1))
        L = R * (1.0 if k % 2 == 0 else 0.72)
        p0 = (c[0] + r0 * math.sin(a), c[1], c[2] + r0 * math.cos(a))
        p1 = (c[0] + L * math.sin(a), c[1], c[2] + L * math.cos(a))
        tilt_box(g, col, p0, p1, t, w, glow=glow)


def ring_box(g, col, r, a_deg, z, size, glow=0.0):
    """A box on a round wall of radius r, at a_deg round from the road side (0 = the front, -Y), its face flat to the wall."""
    a = math.radians(a_deg)
    g.box(col, (r * math.sin(a), -r * math.cos(a), z), size, rot=(0, a_deg, 0), glow=glow)


# ------------------------------------------------------------------------------------------ buildings
def terrace_tower(sc):
    """13 x 10 m, 82 m to the roof, a fin stair tower to 95 m: twelve rounded terraces with solid parapets round a violet core."""
    g = sc.still()
    g.box(VIOLET, (0, 40, 3950), (800, 620, 8500))
    for k in range(12):
        z = 330 + k * 650
        cut = max(0, k - 8)
        w, d = 1300 - cut * 150, 1000 - cut * 110
        g.plan(BLUSH if k % 4 == 3 else WHITE, stad(w, d), z, z + 140)
        lit = (k * 7 + 3) % 5 != 0
        g.box(PEACH if lit else GLASS, (0, -272, z + 370), (640, 6, 230), glow=0.5 if lit else 0.0)
        if k % 2 == 0:
            for s in (-1, 1):
                g.box(WARM_L, (s * 402, 40, z + 370), (6, 420, 200), glow=0.4)
    for k, x in ((0, -420), (1, 380), (3, -300), (5, 420)):                   # parasols on four terraces
        z = 330 + k * 650 + 140
        g.cyl(SILVER, (x, -400, z), 5, 200, steps=5)
        g.cyl(BLUSH if k % 2 else CYAN, (x, -400, z + 180), 115, 44, steps=6, top_r=8)
    g.box(WHITE, (-730, 60, 4300), (200, 300, 9200))                           # the fin stair tower
    g.side(BLUSH, [(-830, 8900), (-630, 8900), (-630, 9150), (-760, 9500), (-830, 9320)], 20, 100)
    for k in range(7):
        port(g, -730, -90, 900 + k * 1200, 60, CYAN, 0.7)
    g.plan(WHITE, stad(900, 660, cy=40), 8200, 8260)                           # the roof: a pool, a glass penthouse, cowls
    g.plan(CYAN, stad(520, 260, cx=130, cy=-60), 8260, 8272, glow=0.9)
    g.box(PEACH, (-230, 130, 8400), (260, 240, 280), glow=0.8)
    g.box(WHITE, (-230, 130, 8555), (330, 310, 30))
    for x in (60, 150):
        g.cyl(SILVER, (x, 260, 8260), 28, 70, steps=6)
    g.cyl(SILVER, (320, 220, 8260), 7, 520, steps=5)
    f = sc.part(P.motion(swing=14, pivot=(320, 220, 8700), axis=P.UP, rate=0.3, phase=0.2))
    f.box(BLUSH, (400, 220, 8700), (150, 6, 70))


def fin_tower(sc):
    """A slim white slab, 6.4 x 5.2 m, 92 m to the roof, with a blush fin blade up one side to 104 m, a porthole column,
    wrap balconies on its top five floors and a lit penthouse under a blush sunburst."""
    g = sc.still()
    g.box(WHITE, (0, 0, 4450), (640, 520, 9500))
    for k in range(9):
        z = 800 + k * 950
        g.box(VIOLET, (0, 0, z), (656, 536, 70))
        lit = (k * 3 + 1) % 4 != 0
        g.box(PEACH if lit else GLASS, (-60, -262, z + 400), (330, 6, 300), glow=0.5 if lit else 0.0)
        port(g, 215, -260, z + 400, 55, CYAN, 0.7)
    for k in range(5):
        z = 4600 + k * 950
        g.plan(WHITE, stad(820, 300, n=3, cy=-250), z + 40, z + 150)
    g.side(BLUSH, [(330, -300), (470, -300), (470, 9800), (400, 10400), (330, 9950)], -70, 70)
    g.box(PEACH, (-60, 0, 9390), (420, 360, 380), glow=0.9)
    g.plan(WHITE, stad(620, 460, n=3, cx=-60), 9580, 9640)
    g.box(SILVER, (-60, 0, 9655), (220, 60, 30))
    sunburst(g, (-60, 0, 9670), 400, BLUSH, n=7, w=40, t=24, glow=0.3)


def lantern_tower(sc):
    """A white round shaft 6.6 m across, 90 m, with balcony discs, and on its top a pool ring round a glass penthouse
    lit like a lantern under a blush cone roof (104 m)."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -300), 330, 9300, steps=10)
    for k in range(6):
        z = 1000 + k * 1350
        g.cyl(BLUSH if k % 2 else VIOLET, (0, 0, z), 342, 150, steps=10)
        g.cyl(WHITE, (0, 0, z + 150), 470, 80, steps=10)
        g.box(PEACH, (0, -322, z + 620), (220, 30, 420), glow=0.5)
    g.cyl(VIOLET, (0, 0, 8700), 330, 300, steps=10, top_r=600)
    g.cyl(WHITE, (0, 0, 9000), 620, 110, steps=12)
    g.cyl(CYAN, (0, 0, 9110), 560, 10, steps=12, glow=0.9)
    g.cyl(PEACH, (0, 0, 9120), 380, 620, steps=8, glow=1.0)
    for a in range(8):
        ring_box(g, SILVER, 384, 45.0 * a + 22.5, 9430, (30, 26, 620))
    g.cyl(WHITE, (0, 0, 9740), 520, 60, steps=12)
    g.cyl(BLUSH, (0, 0, 9800), 480, 260, steps=12, top_r=60)
    g.cyl(SILVER, (0, 0, 10060), 8, 300, steps=5)
    g.sphere(CYAN, (0, 0, 10380), 36, steps=(4, 6), glow=1.0)


def pool_block(sc):
    """12 x 9 m, 52 m: wrap balconies with solid parapets, and on the roof a pool whose glass end hangs 4 m over the
    street side, lit cyan from inside; a striped cabana, loungers, a diving board."""
    g = sc.still()
    H = 5200.0
    g.box(WHITE, (0, 0, H / 2.0 - 150), (1200, 900, H + 300))
    for k in range(9):
        z = 500 + k * 520
        g.plan(BLUSH if k % 3 == 0 else WHITE, stad(1330, 1000, n=3), z, z + 110)
        g.box(VIOLET, (0, 0, z + 150), (1212, 912, 80))
        lit = (k * 5 + 2) % 4 != 0
        g.box(PEACH if lit else GLASS, (0, -452, z + 330), (880, 6, 220), glow=0.5 if lit else 0.0)
    for k in range(5):
        port(g, -400 + k * 200, -450, 270, 46, CYAN, 0.6, ring=SILVER)
    g.box(SILVER, (0, 0, H + 15), (1240, 940, 30))
    g.box(WHITE, (0, -460, H + 115), (1040, 800, 170))                         # the pool's tray: 4.1 m beyond the front
    g.box(CYAN, (0, -460, H + 198), (960, 720, 12), glow=0.9)
    g.box(CYAN, (0, -862, H + 115), (1040, 8, 170), glow=0.6)                   # its glass end
    g.box(CYAN, (0, -680, H + 27), (900, 300, 8), glow=0.6)                     # and glass floor, seen from the street
    for s in (-1, 1):
        tilt_box(g, SILVER, (s * 420, -452, H - 520), (s * 420, -840, H + 30), 30, 30)
    g.box(WHITE, (-380, 260, H + 170), (360, 300, 280))                        # the cabana
    g.box(BLUSH, (-380, 260, H + 325), (430, 370, 30))
    g.box(VIOLET, (-380, 108, H + 150), (150, 6, 220))
    for k in range(3):
        g.box(BLUSH if k % 2 else WHITE, (40 + k * 110, 220, H + 50), (64, 170, 28))
    g.cyl(SILVER, (420, 250, H + 30), 5, 210, steps=5)
    g.cyl(BLUSH, (420, 250, H + 220), 130, 44, steps=6, top_r=8)
    g.box(SILVER, (400, -120, H + 245), (50, 280, 10))                          # the diving board
    g.box(SILVER, (400, -10, H + 215), (40, 40, 60))
    for x in (-150, -60):
        g.cyl(SILVER, (x, 380, H + 30), 26, 64, steps=6)


def winter_garden(sc):
    """11 x 9 m, 70 m: a white frame of eight open floors full of green, troughs spilling over the parapets, hanging
    vines that sway, a glass barrel vault on top."""
    g = sc.still()
    W, D, H = 1100.0, 900.0, 6400.0
    g.box(GREEN_D, (0, 40, H / 2.0 - 150), (W - 160, D - 200, H + 300))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WHITE, (sx * (W / 2.0 - 45), sy * (D / 2.0 - 45), H / 2.0 - 150), (90, 90, H + 300))
    g.box(WHITE, (0, -(D / 2.0 - 40), H / 2.0 - 150), (70, 70, H + 300))
    for k in range(9):
        z = 300 + k * 760
        g.box(WHITE, (0, 0, z), (W + 70, D + 70, 70))
        if k == 8:
            break
        for j, (x, y) in enumerate(((-275, -330), (275, -300))):
            g.sphere(PINE if (k + j) % 2 else LEAF, (x + (40 if k % 2 else -30), y, z + 340), 235, scale=(1.1, 0.8, 1.0), steps=(3, 6))
        g.box(PINE if k % 2 else LEAF, ((-1 if k % 2 else 1) * 250, -D / 2.0 - 30, z + 110), (W * 0.42, 44, 130))
    for i, x in enumerate((-430, -120, 160, 410)):                              # vines: each sways about its own top
        zt = 300 + (7 - i % 3 * 2) * 760
        f = sc.part(P.motion(swing=3, pivot=(x, -D / 2.0 - 44, zt), axis=0.0, rate=0.3, phase=0.23 * i))
        f.box(LEAF if i % 2 else PINE, (x, -D / 2.0 - 44, zt - 190), (84, 30, 380))
    zr = 300 + 8 * 760 + 35
    g.side(CYAN, [(-W / 2.0, zr)] + halfdisc(0.0, zr, W / 2.0, n=6)[::-1][1:-1] + [(W / 2.0, zr)], -D / 2.0, D / 2.0, glow=0.35)
    for y in (-D / 2.0 - 6, 0.0, D / 2.0 + 6):
        g.side(WHITE, halfdisc(0.0, zr, W / 2.0 + 14, n=6) + halfdisc(0.0, zr, W / 2.0 - 30, n=6)[::-1], y - 20, y + 20)
    g.cyl(SILVER, (0, 0, zr + W / 2.0), 8, 220, steps=5)
    g.sphere(BLUSH, (0, 0, zr + W / 2.0 + 240), 30, steps=(3, 6))


def hat_box(sc):
    """A round apartment drum 11.2 m across, 48 m, in blush and white bands like a hat box: a lid, a violet ribbon over
    it and down its sides, a bow on top that sways a little, arched windows round the front."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -300), 560, 4800, steps=12)
    for k in range(4):
        g.cyl(BLUSH, (0, 0, 500 + k * 1000), 568, 480, steps=12)
        for a in (-56, -28, 0, 28, 56):
            lit = (k * 5 + a // 28) % 4 != 0
            ring_box(g, PEACH if lit else GLASS, 562, a, 240 + k * 1000, (130, 30, 200), glow=0.5 if lit else 0.0)
    g.cyl(BLUSH, (0, 0, 4500), 600, 330, steps=12)
    g.cyl(WHITE, (0, 0, 4500), 608, 60, steps=12)
    for a in (0, 90, 180, 270):
        ring_box(g, VIOLET, 590, a, 2270, (150, 50, 5140))
    g.box(VIOLET, (0, 0, 4842), (150, 1220, 24))
    g.box(VIOLET, (0, 0, 4842), (1220, 150, 24))
    f = sc.part(P.motion(swing=3, pivot=(0, 0, 4850), axis=0.0, rate=0.25, phase=0.4))
    f.sphere(VIOLET, (0, 0, 4940), 95, steps=(3, 6))
    for s in (-1, 1):
        tilt_box(f, VIOLET, (s * 60, 0, 4940), (s * 330, 0, 5110), 130, 150)
        tilt_box(f, VIOLET, (s * 50, -40, 4900), (s * 250, -150, 4862), 90, 20)


def observatory(sc):
    """A white round tower 8.4 m across, 62 m, banded blush and violet, a fin stair up its side, a gallery, and a white
    dome on top, SHUT: one silver rib over it, no opening, no instrument showing (69 m)."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -300), 420, 6500, steps=10)
    for k in range(5):
        z = 900 + k * 1100
        g.cyl(VIOLET if k % 2 else BLUSH, (0, 0, z), 430, 110, steps=10)
        for a in (-36, 0, 36):
            lit = (k + a // 36) % 3 != 0
            ring_box(g, PEACH if lit else GLASS, 418, a, z + 520, (120, 30, 300), glow=0.5 if lit else 0.0)
    g.side(BLUSH, [(400, -300), (600, -300), (600, 5100), (400, 5800)], -60, 60)
    g.cyl(VIOLET, (0, 0, 5900), 420, 300, steps=10, top_r=520)
    g.cyl(WHITE, (0, 0, 6200), 520, 120, steps=12)
    g.cyl(SILVER, (0, 0, 6320), 524, 16, steps=12)
    g.cyl(WHITE, (0, 0, 6320), 440, 180, steps=12)
    g.sphere(WHITE, (0, 0, 6500), 440, steps=(6, 12))
    g.side(SILVER, halfdisc(0.0, 6500.0, 456, n=6) + halfdisc(0.0, 6500.0, 424, n=6)[::-1], -70, 70)
    g.cyl(PEACH, (0, 0, 6336), 446, 30, steps=12, glow=0.5)


# ------------------------------------------------------------------------------------------ the hero landmark
def cascade(sc):
    """THE CASCADE: a white wedding-cake tower of nine round terraces, 40 m across at the foot, 117 m to the top
    terrace and 126 m to the tip of its blush sunburst fin. A cyan pool on every terrace, and down one corner a steady
    cyan fall from each pool to the next, nine in a line from the top to a basin at the foot. Palms on six terraces."""
    g = sc.still()
    N, TH = 9, 1300.0
    af = 40.0
    for k in range(N):
        r = 2000.0 - k * 185.0
        z0 = k * TH - (300.0 if k == 0 else 0.0)
        z1 = (k + 1) * TH
        g.cyl(WHITE, (0, 0, z0), r, z1 - z0, steps=14)
        g.cyl(VIOLET, (0, 0, z1 - 330), r + 6, 150, steps=14)
        g.cyl(BLUSH if k % 3 == 2 else WHITE, (0, 0, z1 - 180), r + 40, 180, steps=14)
        g.cyl(CYAN, (0, 0, z1 - 6), r - 30, 14, steps=14, glow=0.9)
        for j in range(5):
            a = -56.0 + 28.0 * j
            lit = (k * 3 + j) % 5 != 0
            ring_box(g, PEACH if lit else GLASS, r, a, k * TH + 560, (max(r * 0.2, 110), 24, 420), glow=0.5 if lit else 0.0)
        rf = r + 52.0                                                          # the fall from this terrace's pool
        ring_box(g, CYAN, rf, af, k * TH + TH / 2.0 + 4, (240 if k else 320, 26, TH + 8), glow=1.0)
    a = math.radians(af)
    g.cyl(WHITE, (2052 * math.sin(a), -2052 * math.cos(a), -100), 330, 150, steps=10)
    g.cyl(CYAN, (2052 * math.sin(a), -2052 * math.cos(a), 50), 290, 10, steps=10, glow=0.9)
    for i, (k, ad) in enumerate(((0, -38), (1, 4), (2, -62), (3, -12), (5, -44), (6, 8))):
        r = 2000.0 - k * 185.0 - 100.0
        z = (k + 1) * TH
        px, py = r * math.sin(math.radians(ad)), -r * math.cos(math.radians(ad))
        g.cyl(WHITE, (px, py, z), 46, 50, steps=6)
        g.cyl(WOOD, (px, py, z + 50), 16, 400, steps=5, top_r=11)
        c = sc.part(P.motion(swing=3, pivot=(px, py, z), axis=20.0, rate=0.3, phase=0.17 * i))
        for q in range(5):
            b = math.radians(72.0 * q + 20.0 * i)
            c.box(PINE if q % 2 else LEAF, (px + 110 * math.cos(b), py + 110 * math.sin(b), z + 440), (250, 56, 12), rot=(-18, 72.0 * q + 20.0 * i, 0))
    zt = N * TH
    g.cyl(WHITE, (0, 0, zt), 300, 120, steps=10)
    g.side(PEACH, halfdisc(0.0, zt + 120, 250, n=6), -30, 30, glow=0.8)
    sunburst(g, (0, 0, zt + 120), 780, BLUSH, n=9, w=64, t=40, span=160.0, r0=270.0, glow=0.5)
    g.box(BLUSH, (0, -2020, 420), (520, 60, 40))                                # the door: a blush canopy and a lit doorway
    g.box(PEACH, (0, -2004, 200), (360, 16, 400), glow=0.6)


# ------------------------------------------------------------------------------------------ props
def canopy(sc):
    """A striped door canopy on six silver poles over a violet carpet, 2.6 x 4.2 m, 3.6 m: a scalloped valance that
    sways, two lit globes at its street end, ropes between the poles. TALL."""
    g = sc.still()
    g.box(VIOLET, (0, 0, 4), (200, 420, 8))
    g.box(BLUSH, (0, 0, 8), (120, 420, 8))
    for sx in (-1, 1):
        for y in (-190, 0, 190):
            g.cyl(SILVER, (sx * 118, y, 0), 6, 292, steps=6)
        for y0, y1 in ((-190, 0), (0, 190)):
            tilt_box(g, VIOLET, (sx * 118, y0, 96), (sx * 118, y1, 96), 8, 8)
        g.sphere(PEACH, (sx * 118, -190, 312), 20, steps=(3, 6), glow=1.0)
    arch = [(-132, 290), (132, 290), (132, 302), (76, 346), (0, 362), (-76, 346), (-132, 302)]
    for k in range(7):
        y = -210 + k * 60
        g.side(BLUSH if k % 2 else WHITE, arch, y, y + 60)
    f = sc.part(P.motion(swing=5, pivot=(0, -213, 292), axis=0.0, rate=0.35, phase=0.1))
    for k in range(5):
        f.box(WHITE if k % 2 else BLUSH, (-104 + k * 52, -213, 268), (50, 6, 50))


def luggage(sc):
    """A silver luggage trolley with trunks and hat boxes, a steamer trunk and a stack of hat boxes beside it:
    4.2 x 0.9 m, 1.7 m. LOW, unlit. White, blush and violet only."""
    g = sc.still()
    g.box(SILVER, (0, 0, 26), (210, 90, 10))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.sphere(NAVY, (sx * 85, sy * 34, 13), 13, steps=(3, 6))
        g.cyl(SILVER, (sx * 100, 0, 31), 5, 132, steps=6)
        g.sphere(BLUSH, (sx * 100, 0, 168), 9, steps=(3, 5))
    g.box(SILVER, (0, 0, 160), (200, 10, 10))
    g.box(VIOLET, (-42, 0, 56), (100, 70, 50))
    g.box(WHITE, (-42, 0, 56), (14, 74, 54))
    g.box(WHITE, (-42, 0, 96), (84, 60, 30))
    g.box(BLUSH, (-42, 0, 96), (10, 64, 34))
    g.box(BLUSH, (56, 0, 51), (78, 64, 40))
    g.cyl(WHITE, (56, 0, 71), 30, 34, steps=10)
    g.cyl(VIOLET, (56, 0, 105), 32, 8, steps=10)
    g.cyl(BLUSH, (56, 0, 113), 22, 26, steps=8)
    g.cyl(WHITE, (56, 0, 139), 24, 6, steps=8)
    g.box(WHITE, (-172, 8, 55), (64, 56, 110))                                  # the steamer trunk, standing
    for z in (26, 84):
        g.box(VIOLET, (-172, 8, z), (68, 60, 10))
    g.box(SILVER, (-172, -22, 56), (12, 6, 14))
    g.cyl(BLUSH, (168, 10, 0), 36, 40, steps=10)                                # three hat boxes
    g.cyl(WHITE, (168, 10, 40), 38, 8, steps=10)
    g.cyl(WHITE, (168, 10, 48), 29, 32, steps=10)
    g.cyl(VIOLET, (168, 10, 80), 31, 8, steps=10)
    g.cyl(VIOLET, (168, 10, 88), 21, 24, steps=8)
    g.cyl(BLUSH, (168, 10, 112), 23, 6, steps=8)
    tilt_box(g, VIOLET, (-120, 34, 4), (-134, 30, 104), 8, 8)                  # a furled umbrella against the trunk


def loungers(sc):
    """Three sun loungers facing the road, two little tables with a glass each, a folded parasol: 4.6 x 2 m, 1.9 m.
    LOW, unlit."""
    g = sc.still()
    for i, x in enumerate((-150, 0, 150)):
        col = (BLUSH, VIOLET, BLUSH)[i]
        g.box(WHITE, (x, -10, 30), (62, 130, 8))
        tilt_box(g, WHITE, (x, 50, 32), (x, 95, 78), 62, 8)
        g.box(col, (x, -14, 38), (54, 118, 8))
        tilt_box(g, col, (x, 52, 40), (x, 92, 82), 54, 8)
        g.box(SILVER, (x, -60, 13), (58, 8, 26))
        g.box(SILVER, (x, 40, 13), (58, 8, 26))
    for x in (-75, 75):
        g.cyl(WHITE, (x, -30, 0), 6, 40, steps=5)
        g.cyl(BLUSH, (x, -30, 40), 20, 6, steps=8)
        g.cyl(CYAN, (x + 6, -30, 46), 5, 12, steps=5)
    g.box(VIOLET, (-150, -46, 45), (40, 50, 6))
    g.cyl(WHITE, (215, 20, 0), 30, 10, steps=8)
    g.cyl(SILVER, (215, 20, 0), 5, 186, steps=5)
    g.cyl(BLUSH, (215, 20, 66), 17, 112, steps=6, top_r=7)
    g.box(VIOLET, (215, 20, 110), (36, 36, 6))


def piano(sc):
    """A white grand piano on a blush and white plinth, its lid raised, a stool at the keyboard (the road side):
    3.4 x 2.3 m, 1.9 m. LOW, unlit."""
    g = sc.still()
    g.plan(BLUSH, stad(340, 230, n=3), 0, 22)
    g.plan(WHITE, stad(312, 204, n=3), 22, 30)
    body = [(-75, -50), (75, -50), (75, 10), (60, 55), (22, 82), (-30, 88), (-75, 66)]
    g.plan(WHITE, body, 100, 132)
    for x, y in ((-60, -38), (60, -38), (-12, 66)):
        g.cyl(WHITE, (x, y, 30), 6, 70, steps=6)
    g.box(WHITE, (0, -62, 104), (146, 24, 8))
    g.box(NAVY, (0, -62, 110), (134, 16, 6))
    g.box(WHITE, (0, -44, 148), (60, 6, 30))
    g.plan(WHITE, [(x + 75, y) for x, y in body], 0, 8, loc=(-75, 0, 134), rot=(18, 0, 0))
    g.box(SILVER, (40, 20, 152), (6, 6, 40))
    g.cyl(BLUSH, (0, -92, 30), 20, 44, steps=8)
    g.cyl(WHITE, (0, -92, 74), 23, 8, steps=8)


def cake_fountain(sc):
    """A tiered cake-stand fountain: a round white basin 5.2 m across, three white plates on a silver stem, cyan water
    on each and skirts of water between them, a small plume that floats up and down a hand's breadth: 6 m. TALL, lit steadily."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -100), 260, 170, steps=12)
    g.cyl(BLUSH, (0, 0, -100), 268, 124, steps=12)
    g.cyl(CYAN, (0, 0, 70), 236, 8, steps=12, glow=0.9)
    g.cyl(SILVER, (0, 0, 70), 22, 470, steps=6)
    tiers = ((175.0, 200.0), (125.0, 350.0), (80.0, 480.0))
    zl = 78.0
    for r, z in tiers:
        g.cyl(CYAN, (0, 0, zl), r + 12, z - zl, steps=12, top_r=r - 4, glow=0.5)   # the skirt of water under this plate
        g.cyl(WHITE, (0, 0, z), r, 16, steps=12)
        g.cyl(CYAN, (0, 0, z + 16), r - 10, 6, steps=12, glow=0.9)
        zl = z + 22.0
    for k in range(4):
        b = math.radians(45.0 + 90.0 * k)
        g.sphere(BLUSH, (72 * math.cos(b), 72 * math.sin(b), 508), 12, steps=(3, 5))
    g.cyl(SILVER, (0, 0, 502), 8, 40, steps=5)
    f = sc.part(P.motion(bob=-6.0, rate=0.5, phase=0.3))
    f.sphere(CYAN, (0, 0, 566), 30, scale=(1.0, 1.0, 1.3), steps=(4, 6), glow=1.0)


def lift_kiosk(sc):
    """A porthole-fronted lift kiosk: a rounded white cabin 2.7 x 1.9 m with two silver door leaves, a porthole in each,
    a floor dial over the door and a blush dome roof with a lit finial: 5.3 m. TALL, lit steadily."""
    g = sc.still()
    g.plan(SILVER, stad(300, 220, n=3), -100, 14)
    g.plan(WHITE, stad(270, 190, n=3), 14, 360)
    g.box(VIOLET, (0, -94, 170), (124, 10, 252))
    for s in (-1, 1):
        g.box(SILVER, (s * 30, -100, 168), (56, 8, 240))
        port(g, s * 30, -104, 216, 17, PEACH, 0.8)
        port(g, s * 100, -62, 232, 22, CYAN, 0.7, ring=SILVER)
    g.box(BLUSH, (0, -104, 302), (170, 34, 12))
    g.side(SILVER, halfdisc(0.0, 310.0, 44, n=6), -102, -94)
    tilt_box(g, VIOLET, (0, -106, 312), (24, -106, 340), 6, 6)
    g.plan(BLUSH, stad(330, 250, n=3), 360, 392)
    g.sphere(BLUSH, (0, 0, 392), 110, scale=(1.25, 0.9, 0.6), steps=(4, 8))
    g.cyl(SILVER, (0, 0, 450), 6, 56, steps=5)
    g.sphere(PEACH, (0, 0, 518), 14, steps=(3, 6), glow=1.0)


def plunge_pool(sc):
    """A roof plunge pool: a rounded white tub 4.4 x 1.8 m with cyan water, a glass panel in its street side, a ladder,
    a blush swim ring that floats a little: 1.7 m. LOW; lit steadily (it stands on roofs 27 m out)."""
    g = sc.still()
    g.plan(BLUSH, stad(452, 192, n=4), 0, 16)
    g.plan(WHITE, stad(440, 180, n=4), 16, 110)
    g.plan(CYAN, stad(400, 140, n=4), 104, 116, glow=0.9)
    g.box(CYAN, (0, -91, 62), (240, 6, 66), glow=0.6)
    for sy in (-1, 1):
        g.cyl(SILVER, (206, sy * 20, 60), 4, 110, steps=5)
    for z in (124, 150):
        g.box(SILVER, (206, 0, z), (8, 40, 6))
    g.box(VIOLET, (110, -86, 106), (54, 14, 12))
    g.box(VIOLET, (110, -94, 80), (54, 6, 60))
    f = sc.part(P.motion(bob=-3.0, rate=0.4, phase=0.6))
    f.cyl(BLUSH, (-90, 8, 116), 30, 12, steps=8)
    f.cyl(CYAN, (-90, 8, 118), 13, 12, steps=8, glow=0.9)


def sunfin(sc):
    """The district's roof ornament: a blush half sunburst round a lit peach half sun on a silver mast with four
    compass arms, turning a little to and fro like a weather vane (12 degrees, once in 8 seconds): 5.2 m. TALL."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, 0), 46, 40, steps=8)
    g.cyl(BLUSH, (0, 0, 40), 34, 14, steps=8)
    g.cyl(SILVER, (0, 0, 54), 7, 300, steps=6)
    g.box(SILVER, (0, 0, 250), (120, 8, 8))
    g.box(SILVER, (0, 0, 250), (8, 120, 8))
    for x, y in ((60, 0), (-60, 0), (0, 60), (0, -60)):
        g.sphere(BLUSH, (x, y, 250), 10, steps=(3, 5))
    v = sc.part(P.motion(swing=12, pivot=(0, 0, 360), axis=P.UP, rate=0.12, phase=0.5))
    v.box(SILVER, (0, 0, 364), (200, 12, 12))
    v.side(PEACH, halfdisc(0.0, 370.0, 62, n=6), -8, 8, glow=0.8)
    sunburst(v, (0, 0, 370), 152, BLUSH, n=7, w=22, t=10, r0=74.0, glow=0.25)


JOBS = [
    ('SM_S1D6_TerraceTower', terrace_tower),
    ('SM_S1D6_FinTower', fin_tower, STILL),
    ('SM_S1D6_LanternTower', lantern_tower, STILL),
    ('SM_S1D6_PoolBlock', pool_block, STILL),
    ('SM_S1D6_WinterGarden', winter_garden),
    ('SM_S1D6_HatBox', hat_box),
    ('SM_S1D6_Observatory', observatory, STILL),
    ('SM_S1D6_Cascade', cascade),
    ('SM_S1D6_Canopy', canopy, dict(grow=(10, 20, 10), grow_down=(10, 20, 0))),
    ('SM_S1D6_Luggage', luggage, STILL),
    ('SM_S1D6_Loungers', loungers, STILL),
    ('SM_S1D6_Piano', piano, STILL),
    ('SM_S1D6_CakeFountain', cake_fountain, dict(grow=(8, 8, 12), grow_down=(8, 8, 0))),
    ('SM_S1D6_LiftKiosk', lift_kiosk, STILL),
    ('SM_S1D6_PlungePool', plunge_pool, SMALL),
    ('SM_S1D6_Sunfin', sunfin, dict(grow=(60, 60, 10), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
