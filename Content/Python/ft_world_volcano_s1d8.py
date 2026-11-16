"""Volcano, Stage 1, builder s1d8: district 8 "The Last Ash" (22.3 to 25 km), THE BACKLOT.
/Game/World/Volcano/SM_S1D8_*. Run: import ft_world_volcano_s1d8 as K; K.run()

Where the film in the game builds its sets: everything is a thin painted front held up from behind by timber
A-braces and sandbags. Mint and butter-cream flats with false windows and cut-out skylines, navy doors, white-and-navy
clapper stripes, gold stars; and THE BIG SLATE, a clapperboard on a mint scaffold whose striped arm lifts and closes.
All of it is drawn here: no brand, no real name, no words, no likeness, no outside asset. Nothing red, nothing
orange, no vehicle, no hose, no jet, no camera, no light that is lit, nothing aimed at the road.
Every mesh is an ft_people.Scene (one section, M_People); movement is the builders' motion values only.
The placing rules are the block C:/ftwork/s1/L3_s1d8.inl (written by C:/ftwork/jobs/L3s1d8/block.py).

  buildings, low (to 10.5 m)    StreetFront, StudioBarn, CostumeStore, PaintShop     (behind the lava channels)
  buildings, tall (14 to 18 m)  PalaceFront, LighthouseFront, TownHallFront, WaterTower
  tall open props (33 m out)    BackdropRoll, StarArch, PaintCradle, LampStand       (thin legs: the lava shows through)
  tall open props (26 m out)    Reflector, LampStand
  low props (23 m, under 2 m)   PaintTins, ChairRow, CableCart, PropCutouts, CostumeRail
  hero                          BigSlate (40 m wide; the arm swings 13 degrees about its hinge, one beat in 14 s)
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

MINT, CREAM, NAVY, WHITE, GOLD = '#7fe0b8', '#fff1c9', '#1c2a4a', '#ffffff', '#ffd23f'   # the district's palette (L3_plan.md)
TIM, STEEL = '#b0a088', '#cdc5c8'            # the kit's PALE_L (timber braces, planks) and HUT (stands)
SKY, PINK, LILAC = '#6fc8ff', '#ff8fb1', '#b9b3d6'   # paint and cloth only: the backdrop's sky, costumes, tin lids
CLOTH = (LILAC, PINK, SKY, CREAM, WHITE, NAVY, LILAC, SKY)
GB, GP = 0.05, 0.04      # a small steady glow (pass 3's own way on this dark level): buildings and hero, tall props


class G(object):
    """An ft_art.Model with one steady glow on everything drawn through it."""

    def __init__(self, m, gl=0.0):
        self.m, self.gl = m, gl

    def box(self, col, c, size, rot=(0, 0, 0)):
        self.m.box(col, c, size, rot=rot, glow=self.gl)

    def cyl(self, col, base, r, h, steps=10, top_r=None):
        self.m.cyl(col, base, r, h, steps=steps, glow=self.gl, top_r=top_r)

    def sphere(self, col, c, r, scale=(1, 1, 1), steps=(4, 6)):
        self.m.sphere(col, c, r, scale=scale, steps=steps, glow=self.gl)

    def side(self, col, pts, y0, y1):
        self.m.side(col, pts, y0, y1, glow=self.gl)

    def tb(self, col, a, b, w, d=None):
        tilt_box(self.m, col, a, b, w, d, glow=self.gl)

    def rd(self, col, a, b, r, steps=8):
        rod(self.m, col, a, b, r, steps, self.gl)


# ------------------------------------------------------------------------------------------ the lot's own parts
def zb(g, col, x, y, z0, z1, w, d):
    """A box w along X, d along Y, from z0 up to z1."""
    g.box(col, (x, y, (z0 + z1) / 2.0), (w, d, z1 - z0))


def flat(g, col, pts, t=24.0, y=0.0):
    """A painted flat: the outline [(x, z)] cut out of board t thick, standing on the line y."""
    g.side(col, pts, y - t / 2.0, y + t / 2.0)


def win(g, x, z, w, h, frame=WHITE, pane=NAVY, yf=-12.0):
    """A false window painted on a front whose face is y = yf: a frame and a dark pane, each proud of the board."""
    zb(g, frame, x, yf - 3.0, z - 12, z + h + 12, w + 24, 10)
    zb(g, pane, x, yf - 7.0, z, z + h, w, 10)


def door(g, x, w, h, col=NAVY, frame=WHITE, yf=-12.0):
    zb(g, frame, x, yf - 3.0, 0, h + 14, w + 28, 10)
    zb(g, col, x, yf - 7.0, 0, h, w, 10)


def half_disc(g, col, x, z, r, y0, y1, n=6):
    g.side(col, [(x + r * math.cos(math.pi * k / n), z + r * math.sin(math.pi * k / n)) for k in range(n + 1)], y0, y1)


def brace(g, x, h, back=260.0, y0=10.0, w=14.0):
    """What holds a flat up: a timber strut from the board's back down to a sill on the ground, a tie, a sandbag."""
    g.tb(TIM, (x, y0, h), (x, y0 + back, 0), w)
    zb(g, TIM, x, y0 + back / 2.0, 0, 12, w, back + 20)
    zb(g, TIM, x, y0 + back * 0.3, h * 0.4 - 6, h * 0.4 + 6, w, back * 0.6)
    g.sphere(CREAM, (x, y0 + back - 20, 22), 26, scale=(1.3, 1.0, 0.55), steps=(3, 6))


def stripes(g, x0, x1, z0, z1, y0, y1, n, lean):
    """Clapper stripes: a navy bar with n white leaning stripes proud of both its faces."""
    zb(g, NAVY, (x0 + x1) / 2.0, (y0 + y1) / 2.0, z0, z1, x1 - x0, y1 - y0)
    pitch = (x1 - x0 - abs(lean)) / float(n)
    for k in range(n):
        xa = x0 + k * pitch + (0 if lean > 0 else -lean) + pitch * 0.2
        g.side(WHITE, [(xa, z0), (xa + pitch * 0.5, z0), (xa + pitch * 0.5 + lean, z1), (xa + lean, z1)], y0 - 5, y1 + 5)


def star_pts(cx, cz, r, rin=0.42):
    out = []
    for k in range(10):
        a = math.radians(90 + k * 36)
        rr = r if k % 2 == 0 else r * rin
        out.append((cx + rr * math.cos(a), cz + rr * math.sin(a)))
    return out


def star(g, cx, cz, r, y0, y1, col=GOLD):
    g.side(col, star_pts(cx, cz, r), y0, y1)


def cloud(g, cx, cz, s, y0, y1, col=WHITE):
    base = [(-60, 0), (60, 0), (78, 22), (62, 48), (30, 62), (-8, 70), (-42, 56), (-74, 40), (-82, 18)]
    g.side(col, [(cx + x * s, cz + z * s) for x, z in base], y0, y1)


def tripod(g, h, col=NAVY, r=75.0):
    """A wheeled stand: three legs on castors, a column h tall."""
    for a in (90, 210, 330):
        e = (r * math.cos(math.radians(a)), r * math.sin(math.radians(a)), 10)
        g.tb(col, (0, 0, 44), e, 7)
        g.sphere(col, e, 9, steps=(3, 6))
    g.rd(STEEL, (0, 0, 40), (0, 0, h), 5)
    g.sphere(CREAM, (40, 45, 16), 18, scale=(1.4, 1.0, 0.5), steps=(3, 6))    # a sandbag on a leg


def tin(g, x, y, z, r, h, lid):
    g.cyl(WHITE, (x, y, z), r, h, steps=8)
    g.cyl(NAVY, (x, y, z + h * 0.4), r + 1.5, h * 0.2, steps=8)
    g.cyl(lid, (x, y, z + h), r - 3, 5, steps=8)


# ------------------------------------------------------------------------------------------ buildings
def street_front(sc):       # three false houses in a row, 18 m wide, 9.2 m to the parapet: a braced street
    g = G(sc.still(), GB)
    flat(g, MINT, [(-900, -150), (-300, -150), (-300, 540), (-600, 770), (-900, 540)])
    zb(g, CREAM, -600, -15, 300, 318, 600, 10)
    door(g, -760, 120, 230)
    win(g, -470, 90, 110, 130)
    win(g, -760, 370, 100, 120)
    win(g, -470, 370, 100, 120)
    g.rd(NAVY, (-600, -22, 610), (-600, -10, 610), 34)
    g.tb(WHITE, (-760, -12, 292), (-760, -96, 250), 190, 8)                 # an awning over the door
    for x in (-820, -760, -700):
        g.tb(NAVY, (x, -13, 293), (x, -97, 251), 32, 9)
    zb(g, NAVY, -470, -30, 60, 85, 130, 30)                                # a window box
    zb(g, PINK, -470, -30, 85, 105, 110, 22)
    flat(g, CREAM, [(-300, -150), (300, -150), (300, 760), (180, 760), (180, 860), (60, 860), (60, 920), (-60, 920),
                    (-60, 860), (-180, 860), (-180, 760), (-300, 760)])
    door(g, 0, 150, 260)
    star(g, 0, 325, 36, -24, -12)
    for z in (380, 580):
        for x in (-190, 0, 190):
            win(g, x, z, 90, 130, frame=MINT)
    for x in (-190, 190):
        win(g, x, 100, 100, 150, frame=MINT)
    zb(g, NAVY, 0, -45, 348, 360, 520, 60)                                 # a balcony
    zb(g, NAVY, 0, -72, 360, 412, 520, 6)
    zb(g, MINT, 0, -15, 742, 760, 600, 10)
    flat(g, MINT, [(300, -150), (900, -150), (900, 640), (300, 640)])
    stripes(g, 300, 900, 640, 710, -12, 12, 6, 40)
    door(g, 460, 120, 230)
    win(g, 700, 70, 260, 190, frame=CREAM)
    win(g, 460, 380, 100, 130)
    win(g, 700, 380, 100, 130)
    g.tb(CREAM, (700, -12, 330), (700, -100, 285), 300, 8)
    for x in (590, 700, 810):
        g.tb(NAVY, (x, -13, 331), (x, -101, 286), 40, 9)
    zb(g, NAVY, 560, -60, 300, 308, 8, 100)                                # a hanging board, blank
    zb(g, CREAM, 560, -70, 228, 300, 8, 70)
    g.rd(WHITE, (0, 0, 915), (0, 0, 1100), 4)
    for x, h in ((-780, 460), (-450, 460), (-150, 680), (150, 680), (450, 520), (780, 520)):
        brace(g, x, h)
    f = G(sc.part(P.motion(swing=14, pivot=(0, 0, 1050), axis=P.UP, rate=0.35, phase=0.2)), GB)
    f.side(MINT, [(4, 1095), (4, 1010), (140, 1052)], -3, 3)


def palace_front(sc):       # a palace front with a cut-out dome, 16 m wide, 17.7 m to the star
    g = G(sc.still(), GB)
    flat(g, CREAM, [(-800, -150), (800, -150), (800, 760), (-800, 760)])
    zb(g, MINT, 0, -10, 760, 820, 1680, 50)
    flat(g, CREAM, [(-430, 820), (430, 820), (430, 900), (-430, 900)])
    g.side(MINT, [(430 * math.cos(math.pi * k / 12), 900 + 560 * math.sin(math.pi * k / 12)) for k in range(13)], -12, 12)
    flat(g, CREAM, [(-60, 1440), (60, 1440), (60, 1560), (-60, 1560)], t=30)
    flat(g, MINT, [(-90, 1560), (90, 1560), (0, 1650)], t=30)
    zb(g, WHITE, 0, 0, 1650, 1720, 8, 8)
    star(g, 0, 1768, 58, -6, 6)
    for sx in (-1, 1):
        x = sx * 690
        flat(g, CREAM, [(x - 90, 820), (x + 90, 820), (x + 90, 1080), (x - 90, 1080)])
        flat(g, MINT, [(x - 110, 1080), (x + 110, 1080), (x + 120, 1130), (x + 60, 1200), (x, 1290), (x - 60, 1200), (x - 120, 1130)], t=30)
        win(g, x, 900, 50, 110)
        g.rd(WHITE, (x, 0, 1285), (x, 0, 1400), 4)
    for x in (-720, -480, -240, 240, 480, 720):
        zb(g, WHITE, x, -18, 0, 700, 56, 14)
        zb(g, WHITE, x, -20, 700, 740, 92, 18)
        zb(g, WHITE, x, -20, 0, 36, 92, 18)
    zb(g, WHITE, 0, -15, 0, 330, 300, 10)
    half_disc(g, WHITE, 0, 330, 150, -20, -10)
    zb(g, NAVY, 0, -19, 0, 330, 260, 10)
    half_disc(g, NAVY, 0, 330, 130, -24, -14)
    star(g, 0, 380, 40, -30, -24)
    for x in (-600, -360, 360, 600):
        zb(g, NAVY, x, -19, 250, 470, 110, 10)
        half_disc(g, NAVY, x, 470, 55, -24, -14)
        zb(g, WHITE, x, -19, 232, 250, 140, 14)
    zb(g, WHITE, 0, -70, 0, 20, 420, 110)
    zb(g, WHITE, 0, -50, 20, 40, 360, 70)
    for x in (-700, -350, 350, 700):
        brace(g, x, 620, 280)
    for x in (-300, 0, 300):
        g.tb(TIM, (x, 10, 1250 if x == 0 else 1050), (x, 330, 0), 16)
        zb(g, TIM, x, 180, 0, 12, 16, 330)
    zb(g, TIM, 0, 190, 520, 534, 640, 14)
    for sx, ph in ((-1, 0.1), (1, 0.6)):
        f = G(sc.part(P.motion(swing=14, pivot=(sx * 690, 0, 1360), axis=P.UP, rate=0.3, phase=ph)), GB)
        f.side(WHITE if sx < 0 else CREAM, [(sx * 690 + 4, 1400), (sx * 690 + 4, 1330), (sx * 690 + 120, 1365)], -3, 3)


def studio_barn(sc):        # an open studio barn, 16.6 m wide, 9 m to the crown: a half-open sliding door and a star
    g = G(sc.still(), GB)
    zb(g, MINT, 0, 80, -150, 620, 1600, 400)
    g.side(CREAM, [(830 * math.cos(math.pi * k / 9), 600 + 300 * math.sin(math.pi * k / 9)) for k in range(10)], -150, 310)
    zb(g, NAVY, -120, -122, 0, 520, 720, 10)                               # the dark of the open stage
    zb(g, SKY, -300, -127, 70, 400, 280, 6)                                # a backdrop seen through the door
    cloud(g, -300, 290, 0.9, -134, -126)
    zb(g, CREAM, 330, -134, 0, 520, 560, 12)                               # the door leaf, slid half across
    stripes(g, 50, 610, 440, 520, -142, -128, 5, 50)
    star(g, 330, 235, 115, -148, -136)
    zb(g, WHITE, 60, -134, 525, 548, 1340, 16)                             # the door's rail
    door(g, -650, 90, 210, yf=-120)
    win(g, 700, 250, 110, 110, yf=-120)
    for x in (-400, 0, 400):                                               # vents on the roof
        z0 = 600 + 300 * math.sqrt(1.0 - (x / 830.0) ** 2) - 12
        zb(g, WHITE, x, 80, z0, z0 + 70, 50, 50)
        zb(g, NAVY, x, 80, z0 + 70, z0 + 90, 92, 92)
    zv = 600 + 300 * math.sqrt(1.0 - (600 / 830.0) ** 2) - 12
    zb(g, WHITE, 600, 80, zv, zv + 170, 8, 8)
    for x in (-760, 760):
        brace(g, x, 400, 150, y0=282)
    v = G(sc.part(P.motion(swing=35, pivot=(600, 80, zv + 150), axis=P.UP, rate=0.12, phase=0.4)), GB)
    v.box(NAVY, (600, 80, zv + 150), (170, 6, 10))
    star(v, 690, zv + 150, 36, 77, 83)


def lighthouse_front(sc):   # a false lighthouse, mint and cream bands, 18.9 m to the star, cut-out waves at its foot
    g = G(sc.still(), GB)
    zs = [-150, 250, 480, 710, 940, 1170, 1400]
    hw = lambda z: 300.0 - 110.0 * max(z, 0.0) / 1400.0
    for k in range(6):
        z0, z1 = zs[k], zs[k + 1]
        flat(g, MINT if k % 2 == 0 else CREAM, [(-hw(z0), z0), (hw(z0), z0), (hw(z1), z1), (-hw(z1), z1)])
    door(g, 0, 110, 230)
    win(g, 0, 560, 60, 90)
    win(g, 0, 1020, 60, 90)
    zb(g, NAVY, 0, -10, 1400, 1430, 520, 70)
    for x in (-250, -125, 0, 125, 250):
        zb(g, NAVY, x, -40, 1430, 1500, 8, 8)
    zb(g, NAVY, 0, -40, 1500, 1510, 520, 8)
    flat(g, CREAM, [(-130, 1430), (130, 1430), (130, 1620), (-130, 1620)])
    for x in (-124, -42, 42, 124):
        zb(g, NAVY, x, -15, 1430, 1620, 14, 10)
    flat(g, MINT, [(-170, 1620), (170, 1620), (0, 1760)], t=30)
    zb(g, WHITE, 0, 0, 1760, 1812, 8, 8)
    star(g, 0, 1850, 45, -6, 6)
    for col, y, top, low, sh in ((CREAM, -36, 215, 120, 50), (WHITE, -60, 150, 60, 0)):
        pts = [(-600, -150), (600, -150)]
        for i in range(13):
            pts.append((600 - i * 100 + (sh if 0 < i < 12 else 0), top if i % 2 else low))
        g.side(col, pts, y, y + 12)
    for sx in (-1, 1):
        g.tb(TIM, (sx * 120, 10, 1100), (sx * 120, 340, 0), 16)
        zb(g, TIM, sx * 120, 185, 0, 12, 16, 340)
        brace(g, sx * 240, 420)
    zb(g, TIM, 0, 175, 543, 557, 260, 14)


def costume_store(sc):      # an open-fronted costume store, 12.6 m wide: rails of cloth that sway, a top hat cut out on the roof
    g = G(sc.still(), GB)
    zb(g, CREAM, 0, 150, -150, 480, 1200, 24)
    for sx in (-1, 1):
        zb(g, MINT, sx * 588, 30, -150, 480, 24, 260)
    zb(g, TIM, 0, 30, 0, 30, 1200, 260)
    zb(g, MINT, 0, 20, 480, 500, 1260, 300)
    for k in range(12):
        x = -577.5 + k * 105
        g.tb(WHITE if k % 2 else NAVY, (x, -110, 500), (x, -180, 455), 105, 8)
    for x in (-520, -40, 40, 520):
        zb(g, WHITE, x, -40, 30, 300, 8, 8)
    g.rd(WHITE, (-520, -40, 300), (-40, -40, 300), 5)
    g.rd(WHITE, (40, -40, 300), (520, -40, 300), 5)
    zb(g, TIM, 0, 130, 360, 372, 900, 40)
    for i, x in enumerate((-300, -100, 100, 300)):
        g.cyl(CLOTH[i + 1], (x, 125, 378), 28, 34, steps=8)
        g.cyl(CLOTH[i + 1], (x, 125, 372), 46, 6, steps=8)
    hat = [(-110, 0), (110, 0), (110, 25), (70, 25), (70, 170), (-70, 170), (-70, 25), (-110, 25)]
    g.side(NAVY, [(x * 1.6, 500 + z * 1.6) for x, z in hat], -8, 8)
    zb(g, CREAM, 0, 0, 548, 600, 226, 26)
    star(g, 0, 690, 44, -14, 14)
    g.rd(WHITE, (0, -75, 30), (0, -75, 150), 4)                            # a dress form on a pole
    g.cyl(PINK, (0, -75, 150), 24, 74, steps=8, top_r=34)
    for x in (-450, 0, 450):
        brace(g, x, 400, 120, y0=162)
    for x0, ph in ((-520, 0.1), (40, 0.6)):
        c = G(sc.part(P.motion(swing=4, pivot=(0, -40, 300), axis=0.0, rate=0.3, phase=ph)), GB)
        for i in range(8):
            h = (150, 190, 130, 170, 150, 200, 140, 180)[i]
            c.box(CLOTH[(i + (3 if x0 > 0 else 0)) % 8], (x0 + 30 + i * 60, -40, 292 - h / 2.0), (44, 14, h))


def paint_shop(sc):         # the paint shop, 10 m wide: cream paint running down a mint front, three giant brushes leaning on it
    g = G(sc.still(), GB)
    flat(g, MINT, [(-500, -150), (500, -150), (500, 600), (-500, 600)])
    drops = (0, 90, 20, 140, 40, 60, 170, 30, 100, 10)
    pts = [(500, 640), (-500, 640)]
    for i, d in enumerate(drops):
        pts += [(-500 + i * 100, 520 - d), (-400 + i * 100, 520 - d)]
    g.side(CREAM, pts, -18, 14)
    door(g, -300, 130, 240)
    win(g, 150, 150, 380, 200)
    for i, col in enumerate((CREAM, SKY, PINK, LILAC, WHITE)):
        zb(g, col, 10 + i * 70, -26, 200, 300, 50, 6)
    for x, col, L in ((-80, SKY, 700.0), (300, PINK, 620.0), (430, CREAM, 560.0)):
        a, b = (x, -170.0, 0.0), (x + 40.0, -30.0, L)
        at = lambda t: (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t)
        g.rd(TIM, a, at(0.62), 13)
        g.tb(NAVY, at(0.60), at(0.72), 64, 32)
        g.tb(col, at(0.72), at(1.0), 74, 36)
    g.cyl(WHITE, (-430, -120, 0), 70, 130, steps=10)                       # a giant pot
    g.cyl(NAVY, (-430, -120, 50), 72, 26, steps=10)
    g.cyl(SKY, (-430, -120, 128), 62, 6, steps=10)
    for x in (-175, -125):                                                  # a ladder
        g.rd(TIM, (x, -150, 0), (x, -16, 560), 4)
    for k in range(6):
        t = 0.12 + k * 0.15
        zb(g, TIM, -150, -150 + 134 * t, 560 * t - 3, 560 * t + 3, 50, 6)
    for x in (-420, -140, 140, 420):
        brace(g, x, 440)


def water_tower(sc):        # a cream water tower on four timber legs, one gold star, 14.6 m: a canvas sock on its spout sways
    g = G(sc.still(), GB)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.tb(TIM, (sx * 280, sy * 280, -150), (sx * 200, sy * 200, 790), 22)
    for z, p in ((400, 233), (90, 259)):
        for s in (-1, 1):
            zb(g, TIM, 0, s * p, z - 9, z + 9, 2 * p, 14)
            zb(g, TIM, s * p, 0, z - 9, z + 9, 14, 2 * p)
    for s in (-1, 1):
        g.tb(TIM, (-255, s * 257, 100), (212, s * 210, 700), 12)
        g.tb(TIM, (255, s * 257, 100), (-212, s * 210, 700), 12)
        g.tb(TIM, (s * 257, -255, 100), (s * 210, 212, 700), 12)
        g.tb(TIM, (s * 257, 255, 100), (s * 210, -212, 700), 12)
    g.cyl(NAVY, (0, 0, 780), 300, 14, steps=12)
    g.cyl(CREAM, (0, 0, 794), 250, 380, steps=12)
    g.cyl(NAVY, (0, 0, 826), 254, 24, steps=12)
    g.cyl(NAVY, (0, 0, 1110), 254, 24, steps=12)
    g.cyl(MINT, (0, 0, 1174), 280, 150, steps=12, top_r=20)
    zb(g, WHITE, 0, 0, 1320, 1380, 8, 8)
    star(g, 0, 1418, 40, -5, 5)
    zb(g, CREAM, 0, -250, 930, 1040, 120, 20)
    star(g, 0, 985, 105, -272, -256)
    for y in (-25, 25):
        g.rd(NAVY, (300, y, 0), (300, y, 800), 4)
    for k in range(9):
        zb(g, NAVY, 300, 0, 60 + k * 85, 66 + k * 85, 6, 50)
    g.rd(STEEL, (120, 0, 790), (120, 0, 620), 14)
    s = G(sc.part(P.motion(swing=6, pivot=(120, 0, 620), axis=0.0, rate=0.3, phase=0.3)), GB)
    s.box(CREAM, (120, 0, 575), (26, 26, 90))


def town_hall_front(sc):    # a four-storey town hall front with a stepped gable, 11 m wide, 16 m: two banners sway
    g = G(sc.still(), GB)
    flat(g, CREAM, [(-550, -150), (550, -150), (550, 1100), (330, 1100), (330, 1220), (160, 1220), (160, 1330), (-160, 1330),
                    (-160, 1220), (-330, 1220), (-330, 1100), (-550, 1100)])
    for x in (-520, 520):
        zb(g, MINT, x, -18, 0, 1100, 60, 14)
    for z in (330, 700, 1080):
        zb(g, MINT, 0, -16, z, z + 26, 1100, 16)
    for z in (430, 790):
        for x in (-350, 0, 350):
            win(g, x, z, 110, 170)
    for x in (-350, 350):
        win(g, x, 90, 110, 170)
    zb(g, WHITE, 0, -15, 0, 250, 240, 10)
    half_disc(g, WHITE, 0, 250, 120, -20, -10)
    zb(g, NAVY, 0, -19, 0, 250, 200, 10)
    half_disc(g, NAVY, 0, 250, 100, -24, -14)
    zb(g, NAVY, 0, -50, 398, 410, 300, 70)
    zb(g, NAVY, 0, -82, 410, 470, 300, 6)
    g.rd(WHITE, (0, -22, 1250), (0, -10, 1250), 75, steps=10)
    star(g, 0, 1250, 52, -28, -20)
    flat(g, MINT, [(-70, 1330), (70, 1330), (70, 1430), (-70, 1430)])
    flat(g, NAVY, [(-100, 1430), (100, 1430), (0, 1530)], t=30)
    zb(g, WHITE, 0, 0, 1530, 1600, 8, 8)
    zb(g, WHITE, 0, -80, 0, 24, 400, 130)
    zb(g, WHITE, 0, -60, 24, 48, 330, 90)
    for x in (-450, -150, 150, 450):
        brace(g, x, 760, 300)
    for sx in (-1, 1):
        g.tb(TIM, (sx * 80, 10, 1250), (sx * 80, 340, 0), 16)
        zb(g, TIM, sx * 80, 185, 0, 12, 16, 340)
    for sx, col, ph in ((-1, NAVY, 0.15), (1, MINT, 0.55)):
        f = G(sc.part(P.motion(swing=3, pivot=(sx * 175, -25, 1075), axis=0.0, rate=0.25, phase=ph)), GB)
        f.box(col, (sx * 175, -27, 895), (90, 8, 360))
        star(f, sx * 175, 1000, 30, -37, -29)


# ------------------------------------------------------------------------------------------ props, low (under 2 m)
def paint_tins(sc):         # a painter's plank of tins and a giant roller, 5.6 m long, 1.93 m
    g = G(sc.still())
    zb(g, CREAM, 0, 0, 0, 5, 560, 120)                                     # a drop cloth
    for x in (-150, 150):
        g.tb(TIM, (x, -35, 0), (x, 0, 88), 8)
        g.tb(TIM, (x, 35, 0), (x, 0, 88), 8)
    zb(g, TIM, 0, 0, 88, 98, 420, 44)
    for x, r, h, lid in ((-150, 24, 46, SKY), (-80, 20, 36, PINK), (10, 28, 56, CREAM), (100, 22, 40, LILAC), (170, 18, 30, WHITE)):
        tin(g, x, 0, 98, r, h, lid)
    tin(g, -250, 10, 5, 34, 70, SKY)
    tin(g, 255, -15, 5, 30, 60, PINK)
    zb(g, STEEL, -40, -45, 5, 15, 70, 30)
    g.rd(TIM, (215, 30, 0), (215, 22, 150), 4)
    zb(g, NAVY, 215, 21, 148, 154, 150, 6)
    for x in (142, 288):
        zb(g, NAVY, x, 21, 148, 176, 6, 6)
    g.rd(CREAM, (150, 21, 172), (280, 21, 172), 20)
    g.rd(SKY, (150, 21, 172), (195, 21, 172), 21)


def chair_row(sc):          # four tall folding chairs with blank backs (one gold star), a megaphone cone on a seat, 1.9 m
    g = G(sc.still())
    for i, x in enumerate((-195, -65, 65, 195)):
        for yy in (-24, 24):
            g.tb(TIM, (x - 28, yy, 0), (x + 28, yy, 105), 6)
            g.tb(TIM, (x + 28, yy, 0), (x - 28, yy, 105), 6)
        zb(g, NAVY, x, 0, 103, 109, 62, 54)
        zb(g, TIM, x, -24, 40, 46, 62, 6)
        for sx in (-30, 30):
            zb(g, TIM, x + sx, 0, 128, 134, 6, 54)
            zb(g, TIM, x + sx, 24, 105, 188, 6, 6)
            zb(g, TIM, x + sx, -24, 105, 131, 6, 6)
        zb(g, CREAM if i != 2 else WHITE, x, 24, 150, 186, 62, 6)
    star(g, -65, 168, 14, 17, 23)
    g.cyl(CREAM, (65, 0, 109), 17, 36, steps=8, top_r=7)


def cable_cart(sc):         # a hand truck hung with coils of cable, a junction box and a cable ramp, 1.9 m
    g = G(sc.still())
    for x in (-36, 36):
        g.rd(NAVY, (x - 5, 10, 18), (x + 5, 10, 18), 18)
        zb(g, STEEL, x * 0.78, 10, 18, 188, 6, 6)
    for z in (60, 120, 184):
        zb(g, STEEL, 0, 10, z, z + 6, 62, 6)
    zb(g, STEEL, 0, -12, 4, 10, 70, 44)
    for z, col in ((150, NAVY), (98, CREAM), (48, LILAC)):
        g.rd(col, (0, -8, z), (0, -24, z), 26, steps=10)
        g.rd(STEEL, (0, -8, z), (0, -27, z), 9, steps=6)
    zb(g, CREAM, 120, 0, 0, 50, 60, 40)
    zb(g, NAVY, 120, 0, 50, 58, 66, 46)
    zb(g, NAVY, 60, -5, 0, 6, 120, 8)
    zb(g, NAVY, 120, 60, 0, 6, 8, 120)
    g.side(NAVY, [(-260, 0), (-80, 0), (-110, 14), (-230, 14)], -40, 40)
    for y in (-30, 30):
        zb(g, WHITE, -170, y, 12, 17, 120, 8)


def prop_cutouts(sc):       # board cut-outs standing in a timber rack: a prop cactus, a prop palm, a cloud, a star on a stick; 1.92 m
    g = G(sc.still())
    zb(g, TIM, 0, 0, 0, 14, 440, 60)
    for x in (-215, 215):
        zb(g, TIM, x, 0, 0, 60, 8, 60)
    cactus = [(-25, 14), (25, 14), (25, 70), (60, 70), (60, 50), (85, 50), (85, 105), (25, 105), (25, 190), (-25, 190), (-25, 130),
              (-85, 130), (-85, 75), (-60, 75), (-60, 100), (-25, 100)]
    g.side(MINT, [(x - 120, z) for x, z in cactus], -5, 3)
    g.side(TIM, [(20, 14), (44, 14), (62, 135), (42, 135)], -5, 3)
    for p in (30, -30, 0):
        g.box(MINT, (52, -1, 148), (124, 8, 26), rot=(p, 0, 0))
    cloud(g, 150, 14, 1.0, 12, 20)
    zb(g, TIM, -20, 18, 14, 125, 6, 6)
    star(g, -20, 142, 30, 15, 21)


def costume_rail(sc):       # a wheeled rail of costumes that sway, 3.2 m long, 1.93 m
    g = G(sc.still())
    for x in (-150, 150):
        zb(g, STEEL, x, 0, 10, 16, 6, 60)
        zb(g, STEEL, x, 0, 16, 186, 6, 6)
        g.sphere(STEEL, (x, 0, 187), 6, steps=(3, 6))
        for y in (-26, 26):
            g.sphere(NAVY, (x, y, 7), 7, steps=(3, 6))
    g.rd(STEEL, (-158, 0, 183), (158, 0, 183), 4)
    zb(g, STEEL, 0, 0, 30, 36, 300, 6)
    c = G(sc.part(P.motion(swing=5, pivot=(0, 0, 183), axis=0.0, rate=0.3, phase=0.2)))
    for i in range(7):
        h = (120, 150, 100, 140, 90, 150, 110)[i]
        c.box(CLOTH[i], (-120 + i * 40, 0, 176 - h / 2.0), (32, 12, h))


# ------------------------------------------------------------------------------------------ props, tall and open
def lamp_stand(sc):         # an UNLIT film lamp with barn doors on a wheeled stand, turned away from the road, 4.7 m
    g = G(sc.still(), GP)
    tripod(g, 348)
    zb(g, NAVY, 0, 0, 345, 351, 110, 10)
    for sx in (-52, 52):
        zb(g, NAVY, sx, 0, 345, 430, 6, 10)
    g.rd(CREAM, (0, -40, 395), (0, 40, 395), 44, steps=10)
    g.rd(NAVY, (0, -54, 395), (0, -40, 395), 32)
    g.rd(NAVY, (0, 40, 395), (0, 46, 395), 47, steps=10)
    g.rd(WHITE, (0, 46, 395), (0, 50, 395), 40, steps=10)
    g.tb(NAVY, (0, 46, 442), (0, 84, 470), 90, 5)
    g.tb(NAVY, (0, 46, 348), (0, 84, 320), 90, 5)
    for sx in (-1, 1):
        g.tb(NAVY, (sx * 47, 46, 395), (sx * 75, 84, 395), 5, 90)


def reflector(sc):          # a cream reflector board in a navy frame on a thin wheeled stand, tilting slowly, 4.1 m
    g = G(sc.still())
    tripod(g, 240)
    zb(g, STEEL, 0, 0, 237, 243, 176, 6)
    for sx in (-85, 85):
        zb(g, STEEL, sx, 0, 240, 333, 6, 6)
    b = G(sc.part(P.motion(swing=12, pivot=(0, 0, 330), axis=0.0, rate=0.12, phase=0.2)))
    b.box(CREAM, (0, 0, 330), (150, 6, 150))
    for z in (255, 405):
        b.box(NAVY, (0, 0, z), (162, 9, 8))
    for x in (-77, 77):
        b.box(NAVY, (x, 0, 330), (8, 9, 150))


def backdrop_roll(sc):      # a painted sky on a roller between two posts, 9.4 m wide, 6.2 m: the cloth sways
    g = G(sc.still(), GP)
    for x in (-450, 450):
        zb(g, CREAM, x, 0, -100, 600, 14, 14)
        zb(g, NAVY, x, 0, 0, 10, 20, 180)
        g.tb(NAVY, (x, -85, 5), (x, 0, 120), 7)
        g.tb(NAVY, (x, 85, 5), (x, 0, 120), 7)
        g.sphere(CREAM, (x, 70, 20), 22, scale=(1.0, 1.3, 0.55), steps=(3, 6))
        g.rd(CREAM, (x - 12, 0, 600), (x + 12, 0, 600), 28)
    g.rd(NAVY, (-470, 0, 600), (470, 0, 600), 20)
    c = G(sc.part(P.motion(swing=2.5, pivot=(0, 0, 590), axis=0.0, rate=0.22, phase=0.3)), GP)
    c.box(SKY, (0, 0, 415), (860, 6, 330))
    c.side(MINT, [(-430, 250), (430, 250), (430, 290), (250, 330), (60, 300), (-120, 345), (-300, 305), (-430, 330)], -6, 6)
    cloud(c, -250, 470, 1.3, -7, 7)
    cloud(c, 120, 420, 1.0, -7, 7)
    cloud(c, 310, 500, 0.8, -7, 7)
    c.rd(CREAM, (-440, 0, 250), (440, 0, 250), 6, steps=6)


def star_arch(sc):          # the lot's gate: clapper-striped posts and beam, three gold stars, a line of pennants; 9.9 m wide, 7.9 m
    g = G(sc.still(), GP)
    for x in (-480, 480):
        zb(g, NAVY, x, 0, -150, 0, 30, 30)
        for k in range(10):
            zb(g, WHITE if k % 2 else NAVY, x, 0, k * 50, k * 50 + 50, 30, 30)
    stripes(g, -520, 520, 500, 570, -15, 15, 8, 40)
    for x, h, r in ((-250, 60, 40), (0, 110, 60), (250, 60, 40)):
        zb(g, WHITE, x, 0, 570, 570 + h, 8, 8)
        star(g, x, 570 + h + r * 0.8, r, -5, 5)
    line = [(-465 + i * 93.0, 470 - 55 * (1 - ((i - 5) / 5.0) ** 2)) for i in range(11)]
    for i in range(10):
        (xa, za), (xb, zc) = line[i], line[i + 1]
        g.tb(WHITE, (xa, 0, za), (xb, 0, zc), 5)
    parts = [G(sc.part(P.motion(swing=14, pivot=(0, 0, 440), axis=0.0, rate=0.5, phase=ph)), GP) for ph in (0.0, 0.35, 0.7)]
    for i in range(1, 10):
        x, z = line[i]
        parts[i % 3].side((MINT, CREAM, WHITE)[i % 3], [(x - 28, z), (x + 28, z), (x, z - 72)], -3, 3)


def paint_cradle(sc):       # the scene painters' rolling tower: a plank deck, pots, a big star half painted gold swinging on two cords; 5.2 m
    g = G(sc.still(), GP)
    for px in (-110, 110):
        for py in (-60, 60):
            zb(g, CREAM, px, py, 20, 520, 8, 8)
            g.rd(NAVY, (px - 5, py, 12), (px + 5, py, 12), 12)
    for py in (-60, 60):
        zb(g, CREAM, 0, py, 26, 34, 228, 8)
        zb(g, CREAM, 0, py, 396, 404, 228, 8)
        zb(g, CREAM, 0, py, 514, 522, 228, 8)
    for px in (-110, 110):
        zb(g, CREAM, px, 0, 26, 34, 8, 128)
        zb(g, CREAM, px, 0, 396, 404, 8, 128)
    g.tb(CREAM, (-110, 60, 40), (110, 60, 290), 6)
    g.tb(CREAM, (110, 60, 40), (-110, 60, 290), 6)
    zb(g, TIM, 0, 0, 290, 302, 240, 140)
    for z in (80, 140, 200, 260):
        zb(g, CREAM, -110, 0, z, z + 6, 6, 128)
    tin(g, -60, 20, 302, 20, 34, SKY)
    tin(g, 10, 30, 302, 24, 44, PINK)
    tin(g, 70, 10, 302, 18, 30, CREAM)
    for sx in (-70, 70):
        zb(g, CREAM, sx, -110, 514, 522, 8, 100)
    s = G(sc.part(P.motion(swing=4, pivot=(0, -150, 518), axis=90.0, rate=0.2, phase=0.1)), GP)
    for sx in (-70, 70):
        zb(s, WHITE, sx, -150, 250, 518, 5, 5)
    pts = star_pts(0, 220, 115)
    s.side(WHITE, pts, -154, -146)
    s.side(GOLD, pts[0:6], -157, -143)


# ------------------------------------------------------------------------------------------ the hero
def big_slate(sc):          # THE BIG SLATE: a clapperboard 40 m wide on a mint scaffold; its striped arm lifts and closes
    g = G(sc.still(), GB)
    for x in (-1800, -900, 0, 900, 1800):
        for y in (-220, 220):
            zb(g, MINT, x, y, -400, 500, 50, 50)
    zb(g, MINT, 0, 0, 470, 500, 4100, 560)
    for k in range(4):
        x0, x1 = -1800 + k * 900, -900 + k * 900
        g.tb(MINT, (x0, -220, 20), (x1, -220, 460), 24)
        g.tb(MINT, (x1, -220, 20), (x0, -220, 460), 24)
    zb(g, MINT, 0, -220, 230, 254, 3650, 24)
    zb(g, NAVY, 0, 0, 500, 2300, 4000, 60)                                 # the board, blank but for three chalk rules and a star
    zb(g, WHITE, 0, 0, 500, 560, 4000, 76)
    zb(g, WHITE, 0, 0, 2240, 2300, 4000, 76)
    for x in (-1970, 1970):
        zb(g, WHITE, x, 0, 500, 2300, 60, 76)
    for z in (1880, 1480, 1080):
        zb(g, WHITE, -590, -32, z, z + 36, 2620, 16)
    zb(g, WHITE, 740, -32, 560, 2240, 36, 16)
    star(g, 1350, 1330, 500, -42, -28)
    stripes(g, -2000, 2000, 2300, 2520, -35, 35, 8, 130)                   # the fixed stick
    g.rd(WHITE, (-2000, -50, 2525), (-2000, 50, 2525), 150, steps=12)      # the hinge
    for x in (-1050, -1350):                                               # a painters' cradle left hanging on the board
        zb(g, WHITE, x, -70, 900, 2300, 6, 6)
    zb(g, TIM, -1200, -80, 880, 900, 340, 70)
    zb(g, TIM, -1200, -112, 900, 990, 340, 6)
    for x in (-1300, 0, 1300):                                             # the braces behind
        g.tb(TIM, (x, 30, 2000), (x, 1000, -300), 50)
        zb(g, TIM, x, 480, 780, 820, 50, 900)
    zb(g, TIM, 0, 520, 960, 1000, 2700, 40)
    hx, hz, th = -2000.0, 2530.0, math.radians(6.5)
    ca, sa = math.cos(th), math.sin(th)
    T = lambda u, v: (hx + u * ca - v * sa, hz + u * sa + v * ca)
    a = G(sc.part(P.motion(swing=6.5, pivot=(hx, 0.0, hz), axis=90.0, rate=0.07, phase=0.0)), GB)
    a.side(NAVY, [T(0, 0), T(4000, 0), T(4000, 220), T(0, 220)], -35, 35)
    for k in range(8):
        u = 130 + k * 500
        a.side(WHITE, [T(u, 0), T(u + 250, 0), T(u + 120, 220), T(u - 130, 220)], -40, 40)


MOVE = dict(grow=(20, 40, 20), grow_down=(20, 40, 0))
JOBS = [
    ('SM_S1D8_StreetFront', street_front, dict(grow=(20, 20, 20), grow_down=(20, 20, 0))),
    ('SM_S1D8_PalaceFront', palace_front, dict(grow=(20, 20, 20), grow_down=(20, 20, 0))),
    ('SM_S1D8_StudioBarn', studio_barn, dict(grow=(20, 20, 20), grow_down=(20, 20, 0))),
    ('SM_S1D8_LighthouseFront', lighthouse_front, STILL),
    ('SM_S1D8_CostumeStore', costume_store, dict(grow=(0, 30, 0), grow_down=(0, 30, 0))),
    ('SM_S1D8_PaintShop', paint_shop, STILL),
    ('SM_S1D8_WaterTower', water_tower, dict(grow=(0, 20, 0), grow_down=(0, 20, 0))),
    ('SM_S1D8_TownHallFront', town_hall_front, dict(grow=(0, 30, 0), grow_down=(0, 30, 0))),
    ('SM_S1D8_PaintTins', paint_tins, STILL),
    ('SM_S1D8_ChairRow', chair_row, STILL),
    ('SM_S1D8_CableCart', cable_cart, STILL),
    ('SM_S1D8_PropCutouts', prop_cutouts, STILL),
    ('SM_S1D8_CostumeRail', costume_rail, dict(grow=(0, 30, 0), grow_down=(0, 30, 0))),
    ('SM_S1D8_LampStand', lamp_stand, STILL),
    ('SM_S1D8_Reflector', reflector, dict(grow=(0, 30, 10), grow_down=(0, 30, 0))),
    ('SM_S1D8_BackdropRoll', backdrop_roll, MOVE),
    ('SM_S1D8_StarArch', star_arch, MOVE),
    ('SM_S1D8_PaintCradle', paint_cradle, dict(grow=(30, 20, 0), grow_down=(30, 20, 0))),
    ('SM_S1D8_BigSlate', big_slate, dict(far=True, grow=(100, 60, 500), grow_down=(100, 60, 0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
