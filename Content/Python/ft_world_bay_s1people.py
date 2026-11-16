"""Bayshore, Stage 1, builder s1people: the people pass 3 left out. /Game/World/Bayshore/SM_S1PEOPLE_*.
Run: import ft_world_bay_s1people as K; K.run()

Every piece brings its own raised floor (2 m and more over the land: the land lies under the road and the barrier is
1.6 m) and is short along the road (7 m at most, 3 m of footing under z = 0: the hills). All of them are TALL: they
stand 40 m and more from the centre line (the banked bends' cuttings end there). No glow anywhere. Nothing is thrown,
nothing circles, nothing points a long dark thing at the road; boxes are white, teal, cream or sky blue.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Bayshore'

CREAM, WHITE, NAVY = '#fff1d6', '#fffaf2', '#2b2350'
HOTPINK, YEL, TEAL, TURQ = '#ff4fb8', '#ffe14d', '#2fc6c0', '#49e2d5'
HBLUE, SKY, PLANK = '#4a7fc0', '#8fe3ff', '#c9a06a'
RED, LAV, PINK = '#ff4d5e', '#b9a4ff', '#ff9ec7'
MINT, GOLD, VIOLET, PEACH = '#7fe3c4', '#e2b45c', '#3a2f6a', '#ffc59e'
GREEN, CORAL, ROCK, ROCK_D = '#1f9a62', '#ff7a8a', '#b8b4c8', '#8a86a0'
ASPHALT, WOOD, WOOD_L, OCHRE = '#4b4668', '#9a744a', '#c08a5a', '#e8b26a'
LEAF = '#2fbf71'

TOPS = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#3a5fa8', '#ff2d3c', '#57c7ff', '#f4a6ff')
BOTTOMS = ('#2b2350', '#3a5fa8', '#f1e4c8', '#7a8fb8', '#c94f6a', '#e8e0f0')
FOOT = 300.0


def fig(sc, r, x, y, z, yaw=0.0, **kw):
    d = P.look(r, tops=TOPS, bottoms=BOTTOMS)
    d.update(kw)
    d.setdefault('phase', r.random())
    d.setdefault('detail', 1)
    return P.figure(sc, x, y, z, yaw, **d)


def kid(r):
    return dict(build='kid', s=r.uniform(0.62, 0.7))


def slab(g, col, x0, x1, y0, y1, z0, z1):
    g.box(col, ((x0 + x1) / 2.0, (y0 + y1) / 2.0, (z0 + z1) / 2.0), (x1 - x0, y1 - y0, z1 - z0))


def block(g, col, x0, x1, y0, y1, top):
    """A raised floor with its footing."""
    slab(g, col, x0, x1, y0, y1, -FOOT, top)


def wave(r, amp=20, rate=None, **kw):
    d = dict(arm_r='wave', swing_r=amp, rate=rate or r.uniform(0.9, 1.4))
    d.update(kw)
    return d


def cheer(r, **kw):
    d = dict(arm_l='vee', arm_r='vee', bob=r.uniform(4, 7), rate=r.uniform(1.0, 1.5))
    d.update(kw)
    return d


# ---- what a hand holds (nothing thinner than 4 cm)
def h_tray(col=WHITE, cup=SKY):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 12, h[2] + 10), (46, 34, 5))
        m.cyl(cup, (h[0] - 10, h[1] - 14, h[2] + 12), 6, 14, steps=6)
        m.cyl(PINK, (h[0] + 10, h[1] - 8, h[2] + 12), 6, 18, steps=6)
    return f


def h_stick(col, tip, length=34.0, tip_r=0.0):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + length / 2.0), (5, 5, length))
        if tip_r:
            m.sphere(tip, (h[0], h[1], h[2] + length + tip_r * 0.7), tip_r, steps=(3, 6))
        else:
            m.box(tip, (h[0], h[1], h[2] + length + 5), (8, 8, 12))
    return f


def h_pennant(col, pole=WHITE):
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + 42), (5, 5, 100))
        m.box(col, (h[0] + side * 26, h[1], h[2] + 78), (48, 4, 26))
    return f


def h_fish(col=SKY):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 20), (10, 16, 42))
        m.box(col, (h[0], h[1], h[2] + 46), (5, 26, 12))
    return f


def h_basket(col=CREAM, load=SKY):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 8, h[2] + 4), (34, 26, 22))
        m.box(load, (h[0], h[1] - 8, h[2] + 18), (26, 18, 8))
    return f


def h_fiddle(col=WOOD_L):
    def f(m, h, side):
        m.box(col, (h[0] - side * 6, h[1] - 6, h[2] + 12), (20, 44, 9))
        m.box(NAVY, (h[0] - side * 6, h[1] - 34, h[2] + 14), (6, 22, 5))
    return f


def h_bow(col=WHITE):
    def f(m, h, side):
        m.box(col, (h[0] - side * 22, h[1], h[2] + 6), (58, 4, 4))
    return f


def h_bar(col=WHITE, weight=RED):
    def f(m, h, side):
        if side > 0:
            m.box(col, (0, h[1], h[2] + 8), (170, 7, 7))
            for sx in (-1, 1):
                m.sphere(weight, (sx * 88, h[1], h[2] + 8), 24, steps=(3, 6))
    return f


CAM = P.hold_camera(col='#fff6ea', lens='#8fe3ff')


def pennants(sc, r, x0, x1, y, z, cols, n=6, sag=30.0):
    g = sc.still()
    g.box(WHITE, ((x0 + x1) / 2.0, y, z), (x1 - x0, 4, 4))
    for k in range(n):
        x = x0 + (k + 0.5) * (x1 - x0) / n
        f = sc.part(P.motion(swing=12, pivot=(x, y, z), axis=0.0, rate=r.uniform(0.5, 0.8), phase=r.random()))
        f.box(cols[k % len(cols)], (x, y, z - 20), (34, 4, 36))


# ============================================================ s1d1 The Strip
def juke_podium(sc):
    """Dancing on a round two-tier podium round a jukebox-shaped kiosk (the Strip)."""
    r = random.Random(11)
    g = sc.still()
    g.cyl(CREAM, (0, 0, -FOOT), 294, FOOT + 150, steps=10)
    g.cyl(HOTPINK, (0, 0, 150), 300, 14, steps=10)
    g.cyl(CREAM, (0, 0, 164), 215, 66, steps=10)
    g.cyl(YEL, (0, 0, 230), 221, 12, steps=10)
    # the jukebox: an arched cabinet, horizontal bands
    g.box(CREAM, (0, 120, 242 + 140), (190, 80, 280))
    g.sphere(HOTPINK, (0, 120, 522), 96, scale=(1, 0.42, 0.9), steps=(4, 8))
    g.box(TEAL, (0, 78, 330), (150, 6, 90))
    for k in range(3):
        g.box(YEL, (0, 77, 410 + k * 34), (170, 6, 14))
    g.box(NAVY, (0, 77, 300), (110, 6, 14))
    for sx in (-1, 1):
        g.box(HOTPINK, (sx * 104, 120, 400), (16, 86, 300))
    spots = [(-150, 10, 20), (150, 0, -20), (-60, -130, 0), (70, -120, 10)]
    for k, (x, y, yaw) in enumerate(spots):
        fig(sc, r, x, y, 242, yaw, turn=r.uniform(14, 22), rate=r.uniform(0.7, 1.0), arm_l=('vee', 'hip', 'out', 'wave')[k], arm_r=('wave', 'vee', 'hip', 'out')[k],
            wear=('skirt', 'trousers', 'dress', 'shorts')[k])
    fig(sc, r, -222, -125, 164, 15, **cheer(r))
    fig(sc, r, 225, -122, 164, -15, **wave(r, **kid(r)))


def diner_steps(sc):
    """Waving from the diner's teal steps; a waitress with a tray under a round canopy on one column (the Strip)."""
    r = random.Random(12)
    g = sc.still()
    block(g, CREAM, -330, 330, 0, 230, 220)
    g.box(HOTPINK, (0, -3, 196), (664, 8, 22))
    for k, top in enumerate((165, 110, 55)):
        block(g, TEAL, -330, 330, -60 * (k + 1), -60 * k, top)
    g.cyl(HOTPINK, (225, 100, 220), 24, 330, steps=8, top_r=40)
    g.cyl(CREAM, (225, 100, 550), 118, 26, steps=10)
    g.cyl(YEL, (225, 100, 576), 96, 16, steps=10, top_r=40)
    g.box(YEL, (-250, 205, 330), (120, 14, 220))          # a fin with eyebrow ledges
    for k in range(3):
        g.box(HOTPINK, (-250, 196, 270 + k * 60), (150, 10, 12))
    fig(sc, r, 110, 60, 220, -10, top=WHITE, bottom=HOTPINK, wear='skirt', hat='cap', hat_col=HOTPINK, arm_r='hold', arm_l='hip', hold_r=h_tray(), rock=3, rate=0.5)
    fig(sc, r, -90, 80, 220, 5, **cheer(r))
    fig(sc, r, -220, 70, 220, 15, **wave(r))
    fig(sc, r, 40, -30, 165, 0, **wave(r, arm_l='wave', swing_l=16))
    fig(sc, r, -160, -90, 110, 10, **wave(r, **kid(r)))
    fig(sc, r, 250, -90, 110, -10, **cheer(r))


def wagon(cols):
    body, trim, wheel, flowers = cols

    def build(sc):
        """A parade wagon drawn up to watch: a flat bed on four big wheels under a hoop of flowers, people waving on it."""
        r = random.Random(13 + len(body) + ord(trim[1]))
        g = sc.still()
        block(g, '#f1e4c8', -300, 300, -130, 130, 30)             # a little mound it is parked on
        for sx in (-180, 180):
            for sy in (-1, 1):
                rod(g, wheel, (sx, sy * 96, 108), (sx, sy * 112, 108), 78, steps=10)
                rod(g, WHITE, (sx, sy * 110, 108), (sx, sy * 118, 108), 22, steps=6)
        g.box(body, (0, 0, 200), (540, 190, 30))
        for k in range(9):
            g.box(trim if k % 2 else WHITE, (-240 + k * 60, -97, 172), (60, 6, 44))
        tilt_box(g, WHITE, (270, 0, 190), (345, 0, 110), 10, 10)      # the drawbar, along the road
        for sx in (-250, 250):
            g.box(WHITE, (sx, 80, 330), (10, 10, 240))
        g.box(WHITE, (0, 80, 452), (510, 10, 10))
        for k in range(8):
            g.sphere(flowers[k % len(flowers)], (-224 + k * 64, 80, 462), 24, steps=(3, 6))
        f = sc.part(P.motion(swing=10, pivot=(250, 80, 450), axis=P.UP, rate=0.45, phase=0.2))
        f.box(trim, (290, 80, 490), (70, 4, 40))
        f.box(WHITE, (250, 80, 480), (6, 6, 70))
        fig(sc, r, -170, -20, 215, 8, **wave(r))
        fig(sc, r, -50, 10, 215, -5, **cheer(r, **kid(r)))
        fig(sc, r, 60, -20, 215, 0, arm_r='up', hold_r=h_pennant(trim), swing_r=14, rate=0.8)
        fig(sc, r, 180, 0, 215, -12, **wave(r, wear='dress'))
    return build


# ============================================================ s1d2 The Marina
def net_deck(sc):
    """Net menders on a quay deck on white posts, the nets hung behind them (the Marina)."""
    r = random.Random(21)
    g = sc.still()
    g.box(PLANK, (0, 40, 208), (680, 330, 18))
    for x in (-320, 0, 320):
        for y in (-110, 190):
            g.box(WHITE, (x, y, (200 - FOOT) / 2.0), (22, 22, 200 + FOOT))
    for x in (-160, 160):
        tilt_box(g, WHITE, (x - 150, -110, 20), (x + 150, -110, 190), 10, 10)
    for k in range(11):
        g.box(WHITE if k % 2 else SKY, (-300 + k * 60, -124, 170), (56, 6, 60))      # a board skirt, vertical boards
    for x in (-300, 300):
        g.box(WHITE, (x, 190, 360), (12, 12, 290))
    g.box(WHITE, (0, 190, 500), (620, 10, 10))
    n = sc.part(P.motion(swing=3, pivot=(0, 190, 500), axis=0.0, rate=0.3, phase=0.4))
    n.box(SKY, (-130, 190, 400), (300, 5, 190))
    n.box(HBLUE, (170, 190, 420), (220, 5, 150))
    for k in range(6):
        n.sphere(WHITE, (-260 + k * 104, 186, 492), 13, steps=(3, 5))
    g = sc.still()
    for k, (x, y) in enumerate(((-250, 120), (-205, 120), (-228, 120))):
        g.box((WHITE, HBLUE, WHITE)[k], (x, y, 217 + 22 + (44 if k == 2 else 0)), (42, 60, 44))    # lobster pots, white and blue
    for x in (-60, 120):
        g.box(WHITE, (x, 5, 240), (50, 44, 46))
        fig(sc, r, x, 0, 217, 0, legs='sit', arm_l='carry', arm_r='carry', swing_r=10, axis_r='x', rate=0.8, hat='cap' if x < 0 else None, hat_col=HBLUE)
    g.box(SKY, (30, -52, 250), (230, 60, 6), rot=(0, 0, 10))                               # the net across their knees
    fig(sc, r, 250, 20, 217, -15, top=WHITE, band=HBLUE, **wave(r))
    fig(sc, r, -150, -20, 217, 12, **cheer(r))
    fig(sc, r, 215, 120, 217, 160, arm_l='up', arm_r='up', swing_l=6, swing_r=6, axis_l='x', axis_r='x', rate=0.5)    # hanging the net


def hull_painters(sc):
    """A white dinghy on trestles being painted from a plank; one stands in her waving a pennant (the Marina)."""
    r = random.Random(22)
    g = sc.still()
    block(g, PLANK, -340, 340, -210, 150, 60)
    hull = [(-270, -80), (150, -80), (310, 0), (150, 80), (-270, 80)]
    g.plan(WHITE, hull, 260, 340, loc=(0, 30, 0))
    g.plan(HBLUE, [(x * 1.03, y * 1.06) for x, y in hull], 340, 358, loc=(0, 30, 0))
    g.box(PLANK, (-60, 30, 350), (20, 150, 10))
    for x in (-200, 170):                                   # trestles under her
        for sx in (-1, 1):
            tilt_box(g, WOOD, (x + sx * 50, 30, 60), (x, 30, 262), 120, 16)
    g.box(WHITE, (40, 30, 520), (10, 10, 340))               # a stub mast and a furled sail
    g.box(CREAM, (40, 30, 560), (26, 26, 220))
    f = sc.part(P.motion(swing=14, pivot=(40, 30, 690), axis=P.UP, rate=0.4, phase=0.1))
    f.box(SKY, (90, 30, 672), (96, 4, 30))
    g = sc.still()
    g.box(PLANK, (0, -130, 208), (640, 60, 14))             # the painters' plank on two trestles
    for x in (-280, 280):
        for sx in (-1, 1):
            tilt_box(g, WOOD, (x + sx * 40, -130, 60), (x, -130, 202), 56, 12)
    g.cyl(WHITE, (-120, -130, 215), 16, 26, steps=6)
    g.cyl(SKY, (150, -112, 215), 16, 26, steps=6)
    fig(sc, r, -210, -130, 215, 150, top=WHITE, arm_r='fwd', hold_r=h_stick(PLANK, HBLUE), swing_r=14, axis_r='x', rate=0.9, hat='cap', hat_col=WHITE)
    fig(sc, r, 60, -130, 215, -160, arm_l='fwd', hold_l=h_stick(PLANK, HBLUE), swing_l=14, axis_l='x', rate=0.8, wear='shorts')
    fig(sc, r, 230, -130, 215, 0, **wave(r))
    fig(sc, r, -120, 30, 300, 0, arm_r='up', hold_r=h_pennant(SKY), swing_r=14, rate=0.8, top=HBLUE)
    fig(sc, r, 150, 30, 300, -10, **cheer(r, **kid(r)))


def veranda(roof, shutter):
    def build(sc):
        """A white boarded veranda on posts: two in rocking chairs, two at the rail, a pennant on the gable."""
        r = random.Random(23 + ord(roof[1]))
        g = sc.still()
        block(g, WHITE, -330, 330, -80, 200, 210)
        g.box(PLANK, (0, 60, 216), (680, 300, 12))
        for k in range(12):
            g.box(SKY if k % 2 else WHITE, (-302 + k * 55, -83, 120), (50, 6, 170))       # boards
        g.box(WHITE, (0, 196, 380), (660, 12, 320))             # the house wall behind
        for x in (-190, 190):
            g.box(NAVY, (x, 188, 380), (90, 6, 130))
            for sx in (-1, 1):
                g.box(shutter, (x + sx * 62, 187, 380), (30, 6, 130))
        g.box(shutter, (0, 188, 330), (90, 6, 220))
        for x in (-320, -110, 110, 320):
            g.box(WHITE, (x, -70, 370), (14, 14, 300))
        g.box(WHITE, (0, -70, 308), (640, 8, 10))
        for x in (-215, 0, 215):
            g.box(WHITE, (x, -70, 262), (8, 8, 90))
        tilt_box(g, roof, (0, -110, 505), (0, 210, 590), 700, 14)   # the lean-to roof
        g.box(WHITE, (0, -108, 495), (700, 10, 26))
        g.box(WHITE, (0, 196, 660), (8, 8, 160))
        f = sc.part(P.motion(swing=16, pivot=(0, 196, 720), axis=P.UP, rate=0.4, phase=0.6))
        f.box(roof, (52, 196, 716), (100, 4, 36))
        for x, ph in ((-160, 0.1), (150, 0.55)):                 # rocking chairs
            piv = (x, 100, 222)
            c = sc.part(P.motion(swing=7, pivot=piv, axis=0.0, rate=0.45, phase=ph, base=(x, 100)))
            c.box(PLANK, (x, 104, 222 + 42), (56, 50, 8))
            c.box(PLANK, (x, 130, 222 + 86), (56, 8, 96))
            c.box(PLANK, (x - 26, 104, 222 + 6), (8, 76, 10))
            c.box(PLANK, (x + 26, 104, 222 + 6), (8, 76, 10))
            fig(sc, r, x, 100, 222, 0, legs='sit', seat=46, arm_r='wave' if x < 0 else 'down', arm_l='down', hat='sun' if x > 0 else None, hat_col=WHITE,
                sway=dict(swing=7, pivot=piv, axis=0.0), rate=0.45, phase=ph)
        fig(sc, r, 20, -20, 222, 0, **wave(r, top=shutter))
        fig(sc, r, 260, -10, 222, -10, **cheer(r, **kid(r)))
        fig(sc, r, -270, -10, 222, 12, arm_l='face', arm_r='hip', rock=2, rate=0.4)
    return build


# ============================================================ s1d3 Pleasure Beach
def drum(g, x, y, top, rad, c1=YEL, c2=RED):
    g.cyl(c1, (x, y, -FOOT), rad, FOOT + top - 14, steps=8)
    g.cyl(c2, (x, y, top - 14), rad + 8, 14, steps=8)
    for k in range(4):
        a = math.radians(k * 90 + 22.5)
        g.box(c2, (x + math.cos(a) * rad * 0.93, y + math.sin(a) * rad * 0.93, top * 0.5), (rad * 0.5, 8, top * 0.8), rot=(0, k * 90 + 112.5, 0))


def show_plinths(sc):
    """A strongman on a plinth, a juggler on a drum, a ringmaster; bunting between two masts (Pleasure Beach)."""
    r = random.Random(31)
    g = sc.still()
    drum(g, 0, 60, 280, 120)
    drum(g, -230, -40, 200, 92, WHITE, RED)
    drum(g, 230, -40, 170, 92, WHITE, TURQ)
    block(g, YEL, -110, 110, -200, -110, 120)
    for x in (-335, 335):
        g.box(WHITE, (x, 150, 150), (12, 12, 800))
    pennants(sc, r, -335, 335, 150, 540, (RED, YEL, WHITE, TURQ), n=8)
    fig(sc, r, 0, 60, 280, 0, build='stocky', top=RED, bottom=WHITE, wear='shorts', sleeves='none', hair='bald', arm_l='up', arm_r='up', hold_r=h_bar(),
        bob=5, rate=0.6)
    fig(sc, r, -230, -40, 200, 10, top=YEL, bottom=RED, arm_l='hold', arm_r='hold', hold_l=h_stick(WHITE, RED, 30, 9), hold_r=h_stick(WHITE, TURQ, 30, 9),
        swing_l=30, swing_r=-30, axis_l='x', axis_r='x', rate=1.1)
    fig(sc, r, 230, -40, 170, -10, top=RED, bottom=NAVY, wear='coat', sleeves='long', hat='peak', hat_col=RED, arm_r='wave', swing_r=16, arm_l='hip', rate=0.9)
    fig(sc, r, -55, -155, 120, 5, **cheer(r, **kid(r)))
    fig(sc, r, 55, -155, 120, -5, **cheer(r))


def stilt_walkers(sc):
    """Two stilt walkers swaying over a family eating candy floss on a raised bench (Pleasure Beach)."""
    r = random.Random(32)
    g = sc.still()
    block(g, YEL, -340, 40, -40, 200, 150)
    g.box(RED, (-150, -44, 130), (392, 8, 26))
    block(g, WHITE, -340, 40, 110, 200, 230)
    for k in range(7):
        g.box(RED if k % 2 else WHITE, (-312 + k * 54, -43, 60), (54, 6, 110))
    g.box(TURQ, (-150, 60, 173), (360, 46, 46))
    floss = (PINK, SKY, PINK)
    for k, x in enumerate((-280, -150, -30)):
        d = kid(r) if k == 1 else {}
        fig(sc, r, x, 55, 150 + (10 if k == 1 else 0), 0, legs='sit', arm_r='face', hold_r=h_stick(WHITE, floss[k], 26, 15), swing_r=8, axis_r='x', rate=0.5, **d)
    fig(sc, r, -210, 155, 230, 5, **cheer(r))
    block(g, '#f1e4c8', 60, 345, -170, 170, 20)
    for k, (x, y, yaw, top, leg) in enumerate(((150, -70, 10, RED, YEL), (270, 60, -15, TURQ, WHITE))):
        ph, rt = r.random(), 0.5 + 0.1 * k
        sway = dict(swing=3.5, pivot=(x, y, 20), axis=yaw + 90.0)
        m = sc.part(P.motion(phase=ph, rate=rt, base=(x, y), **sway))
        for sx in (-1, 1):
            m.box(leg, (x + sx * 10, y, 20 + 110), (15, 17, 222))
            m.box(RED, (x + sx * 10, y, 20 + 60), (16, 18, 20))
            m.box(RED, (x + sx * 10, y, 20 + 150), (16, 18, 20))
        fig(sc, r, x, y, 20 + 196, yaw, top=top, bottom=leg, hat='sun', hat_col=YEL, arm_l='out', arm_r='wave', sway=sway, rate=rt, phase=ph)


# ============================================================ s1d4 Hotel Row
def balus_terrace(sc):
    """Hotel guests on a mint terrace behind a white balustrade; a waiter with a tray; a table under a peach parasol."""
    r = random.Random(41)
    g = sc.still()
    block(g, MINT, -340, 340, -110, 220, 230)
    g.box(WHITE, (0, -113, 210), (690, 8, 40))
    for k in range(4):                                      # arches let into the wall
        x = -255 + k * 170
        g.box(VIOLET, (x, -113, 90), (90, 6, 130))
        rod(g, VIOLET, (x, -110, 155), (x, -116, 155), 45, steps=8)
    g.box(WHITE, (0, -95, 322), (690, 22, 14))              # the rail and its urn-shaped posts
    g.box(GOLD, (0, -95, 332), (690, 12, 6))
    for k in range(7):
        g.cyl(WHITE, (-330 + k * 110, -95, 230), 13, 86, steps=6, top_r=8)
    g.cyl(WHITE, (190, 110, 230), 46, 72, steps=8)
    g.box(WHITE, (190, 110, 400), (8, 8, 240))
    g.cyl(PEACH, (190, 110, 470), 118, 56, steps=8, top_r=12)
    for sx in (-1, 1):
        g.box(WHITE, (190 + sx * 92, 110, 253), (44, 44, 46))
    fig(sc, r, 98, 110, 230, -70, legs='sit', arm_r='face', hold_r=P.hold_cup(), wear='dress', hat='sun', hat_col=WHITE)
    fig(sc, r, 282, 110, 230, 70, legs='sit', arm_l='wave', swing_l=14, rate=0.8, top=WHITE)
    fig(sc, r, -40, 60, 230, 10, top=WHITE, bottom=VIOLET, sleeves='long', arm_r='hold', hold_r=h_tray(GOLD, WHITE), arm_l='back', rock=2, rate=0.4)
    fig(sc, r, -200, -40, 230, 8, **wave(r, wear='dress', top=PEACH))
    fig(sc, r, -290, -35, 230, -6, **cheer(r, top=MINT))
    fig(sc, r, 60, -40, 230, 0, **wave(r, **kid(r)))


def string_trio(sc):
    """A string trio on a round white dais before a fluted gold-rimmed screen; two guests listen on the step."""
    r = random.Random(42)
    g = sc.still()
    g.cyl(WHITE, (0, 0, -FOOT), 300, FOOT + 130, steps=10)
    g.cyl(MINT, (0, 20, 130), 240, 80, steps=10)
    g.cyl(GOLD, (0, 20, 210), 246, 10, steps=10)
    for k in range(5):                                      # the screen: five flutes fanning, arched
        a = math.radians(-50 + k * 25)
        x, y = math.sin(a) * 215, 20 + math.cos(a) * 215
        h = 300 + (2 - abs(k - 2)) * 60
        g.box(WHITE if k % 2 else MINT, (x, y, 220 + h / 2.0), (96, 14, h), rot=(0, -math.degrees(a), 0))
        g.sphere(GOLD, (x, y, 220 + h), 26, steps=(3, 6))
    fig(sc, r, -110, 0, 220, 15, top=WHITE, bottom=VIOLET, sleeves='long', arm_l=((70, 60), (120, 110)), hold_l=h_fiddle(), arm_r='fwd', hold_r=h_bow(), swing_r=14, axis_r='z', rate=1.0, rock=0)
    g = sc.still()
    g.box(WHITE, (30, 45, 243), (46, 44, 46))
    fig(sc, r, 30, 40, 220, 0, legs='sit', top=VIOLET, bottom=VIOLET, wear='dress', arm_r='fwd', hold_r=h_bow(), swing_r=16, axis_r='z', rate=0.7, arm_l='hold')
    g = sc.still()
    g.box(WOOD_L, (30, -22, 290), (40, 16, 96))              # the cello between her knees
    g.box(NAVY, (30, -22, 366), (8, 8, 60))
    fig(sc, r, 150, 10, 220, -18, top=WHITE, bottom=VIOLET, sleeves='long', arm_l=((70, 60), (120, 110)), hold_l=h_fiddle(GOLD), arm_r='fwd', hold_r=h_bow(), swing_r=12, axis_r='z', rate=1.2)
    fig(sc, r, -215, -150, 130, 20, **wave(r, wear='dress', top=PEACH))
    fig(sc, r, 200, -170, 130, -15, arm_l='hip', arm_r='face', hold_r=P.hold_cup(), rock=3, rate=0.5, top=MINT)


# ============================================================ s1d5 The Point
def painters_terrace(sc):
    """Painters at easels on a white garden-wall terrace with a coral coping and clipped hedges (the Point)."""
    r = random.Random(51)
    g = sc.still()
    block(g, WHITE, -340, 340, -110, 210, 220)
    g.box(CORAL, (0, -112, 212), (696, 12, 18))
    g.box(NAVY, (0, -113, 80), (110, 6, 150))               # an arched niche in the wall
    rod(g, NAVY, (0, -110, 155), (0, -116, 155), 55, steps=8)
    for x in (-290, 290):
        g.box(GREEN, (x, 150, 290), (90, 110, 140))
        g.sphere(GREEN, (x, 150, 380), 52, steps=(4, 6))
    g.box(GREEN, (0, 195, 270), (480, 30, 100))
    for k, (x, col) in enumerate(((-150, CORAL), (120, SKY))):        # two easels
        for sx in (-1, 1):
            tilt_box(g, WOOD_L, (x + sx * 34, -30, 220), (x, -22, 380), 6, 6)
        tilt_box(g, WOOD_L, (x, 20, 220), (x, -22, 380), 6, 6)
        g.box(WHITE, (x, -34, 330), (70, 5, 56), rot=(0, 0, 0))
        g.box(col, (x - 8, -37, 322), (34, 4, 24))
        g.box(YEL, (x + 16, -37, 340), (16, 4, 14))
        fig(sc, r, x + 55, 30, 220, 20, arm_l='fwd', hold_l=h_stick(WOOD_L, col, 18), swing_l=8, axis_l='z', rate=0.7, hat='sun', hat_col=(WHITE, CORAL)[k], arm_r='hip')
    fig(sc, r, 270, -30, 220, -10, **wave(r, top=CORAL))
    fig(sc, r, -280, -30, 220, 10, **cheer(r, **kid(r)))
    fig(sc, r, 0, 90, 220, 0, arm_l='face', arm_r='hip', rock=2, rate=0.35, top=WHITE, wear='dress')


def picnic_wall(sc):
    """A picnic on a terrace over a dry-stone wall, a family waving, a white telescope turned to the sky (the Point)."""
    r = random.Random(52)
    g = sc.still()
    block(g, ROCK, -340, 340, -120, 200, 150)
    for k in range(9):
        g.box(ROCK_D, (-300 + k * 75, -122, 30 + (k % 3) * 40), (58, 6, 28))
    block(g, WHITE, -320, 320, -80, 200, 215)
    g.box(CORAL, (-120, 50, 218), (260, 200, 6))            # the rug, checked
    for k in range(4):
        g.box(WHITE, (-210 + (k % 2) * 130 + 30 * (k // 2), 0 + (k // 2) * 95, 220), (60, 60, 5))
    g.box(WHITE, (-120, 60, 238), (56, 40, 34))             # the hamper, white
    g.box(PLANK, (-120, 60, 257), (60, 44, 6))
    for x, yaw, k in ((-215, -60, 0), (-30, 60, 1), (-125, 5, 2)):
        d = kid(r) if k == 2 else {}
        fig(sc, r, x, 60 + (75 if k == 2 else 0), 222, yaw, legs='sit', seat=16, arm_r='face' if k < 2 else 'wave', hold_r=P.hold_cup() if k < 2 else None,
            swing_r=0 if k < 2 else 16, rate=0.9, hat='sun' if k == 0 else None, hat_col=CORAL, **d)
    g = sc.still()
    g.cyl(WHITE, (230, 90, 215), 8, 120, steps=6)           # the telescope on its post, turned along the wall and up
    tilt_box(g, WHITE, (190, 90, 318), (285, 90, 372), 20, 20)
    g.box(GREEN, (285, 90, 372), (10, 24, 24))
    fig(sc, r, 160, 95, 215, 80, arm_l='face', arm_r='hip', rock=2, rate=0.3)
    fig(sc, r, 120, -30, 215, 0, **wave(r, top=GREEN))
    fig(sc, r, 250, -30, 215, -8, **cheer(r, top=CORAL, wear='dress'))


# ============================================================ s1d6 Downtown waterfront
def premiere_steps(sc):
    """A premiere crowd on navy steps behind gold rope posts, a pink carpet down the middle, a doorman at the top."""
    r = random.Random(61)
    g = sc.still()
    for k, top in enumerate((250, 180, 110)):
        block(g, NAVY if k % 2 == 0 else VIOLET, -340, 340, -70 - 80 * k, 220 if k == 0 else -70 - 80 * (k - 1), top)
        g.box(TURQ, (0, -72 - 80 * k, top - 5), (684, 6, 10))
    for k, top in enumerate((250, 180, 110)):
        y0 = -70 - 80 * k
        slab(g, HOTPINK, -60, 60, y0 - 3, (220 if k == 0 else y0 + 80), top, top + 4)
    for x in (-320, -200, 200, 320):
        g.cyl(GOLD, (x, -205, 110), 7, 86, steps=6)
        g.sphere(GOLD, (x, -205, 202), 11, steps=(3, 6))
    for x in (-260, 260):
        g.box(HOTPINK, (x, -205, 176), (120, 6, 8))
    g.box(VIOLET, (0, 214, 420), (680, 12, 340))             # the wall behind: a grid of yellow windows
    for i in range(5):
        g.box(YEL, (-260 + i * 130, 206, 420), (70, 6, 110))
    g.box(HOTPINK, (0, 160, 600), (460, 120, 16))            # the canopy
    g.box(TURQ, (0, 98, 600), (470, 6, 26))
    fig(sc, r, 110, 120, 250, -8, top=NAVY, bottom=NAVY, wear='coat', sleeves='long', hat='peak', hat_col=HOTPINK, band=GOLD, arm_r='point', arm_l='back', rock=2, rate=0.3)
    fig(sc, r, -200, 0, 250, 10, **cheer(r))
    fig(sc, r, -110, -10, 250, 0, arm_l='hold', arm_r='hold', hold_r=CAM, rock=3, rate=0.5)
    fig(sc, r, 230, -5, 250, -12, **cheer(r, wear='dress', top=HOTPINK))
    fig(sc, r, -270, -110, 180, 14, **wave(r, top=TURQ))
    fig(sc, r, 280, -185, 110, -15, arm_l='hold', arm_r='hold', hold_r=CAM, rock=3, rate=0.6, top=YEL)


def photo_riser(sc):
    """Photographers on a two-step riser with small pale cameras, one up a white step-ladder (Downtown)."""
    r = random.Random(62)
    g = sc.still()
    block(g, ASPHALT, -320, 320, -150, 200, 150)
    block(g, VIOLET, -320, 320, 20, 200, 240)
    for y, z in ((-152, 142), (18, 232)):
        g.box(TURQ, (0, y, z), (644, 6, 12))
    for k in range(6):
        g.box(HOTPINK if k % 2 else TURQ, (-275 + k * 110, -152, 60), (60, 6, 60))
    lx = 210                                                # the step-ladder
    for sx in (-1, 1):
        tilt_box(g, WHITE, (lx + sx * 46, 110, 240), (lx + sx * 14, 110, 366), 46, 8)
    g.box(WHITE, (lx, 110, 370), (50, 50, 8))
    fig(sc, r, lx, 110, 374, -10, arm_l='hold', arm_r='hold', hold_r=CAM, rock=2, rate=0.4, top=YEL)
    fig(sc, r, -220, 110, 240, 12, arm_l='hold', arm_r='hold', hold_r=CAM, rock=3, rate=0.5, hat='cap', hat_col=TURQ)
    fig(sc, r, -60, 110, 240, 0, **wave(r, top=HOTPINK))
    fig(sc, r, 60, -60, 150, -6, arm_l='hold', arm_r='hold', hold_r=CAM, rock=3, rate=0.6)
    fig(sc, r, -150, -70, 150, 10, arm_r='face', hold_r=CAM, arm_l='hip', rock=2, rate=0.45, top=TURQ)
    fig(sc, r, 250, -70, 150, -14, **cheer(r))


# ============================================================ s1d7 The Old Harbour
def fish_stalls(sc):
    """Fishwives at two raised stalls under red and white awnings, the catch in white and teal boxes (the Old Harbour)."""
    r = random.Random(71)
    g = sc.still()
    block(g, OCHRE, -340, 340, -150, 210, 120)
    block(g, WOOD, -330, 330, -70, 210, 210)
    for k in range(6):
        g.box(WOOD_L, (-275 + k * 110, -72, 165), (14, 6, 86))
    for sx in (-170, 170):
        for px in (-130, 130):
            g.box(WOOD, (sx + px, -40, 360), (12, 12, 300))
            g.box(WOOD, (sx + px, 190, 390), (12, 12, 360))
        for j in range(5):
            tilt_box(g, RED if j % 2 else WHITE, (sx - 130 + j * 65, -70, 492), (sx - 130 + j * 65, 196, 566), 65, 8)
        g.box(WOOD_L, (sx, 10, 286), (270, 70, 12))           # the counter
        for j in range(3):
            g.box((WHITE, TEAL, WHITE)[j], (sx - 86 + j * 86, 10, 306), (72, 56, 30))
            g.box((SKY, PINK, SKY)[j], (sx - 86 + j * 86, 10, 324), (50, 14, 8))
    fig(sc, r, -170, 100, 210, 0, top=TEAL, bottom=CREAM, wear='dress', hat='sun', hat_col=CREAM, arm_r='up', hold_r=h_fish(SKY), swing_r=10, rate=0.8, arm_l='hip')
    fig(sc, r, 170, 100, 210, -5, top=RED, bottom=CREAM, wear='dress', hair='bun', arm_l='wave', swing_l=16, rate=1.0, arm_r='hold', hold_r=h_fish(PINK), build='stocky')
    fig(sc, r, 0, -10, 210, 0, **cheer(r, **kid(r)))
    fig(sc, r, -290, -110, 120, 20, arm_r='point', arm_l='carry', hold_l=h_basket(), rock=2, rate=0.4)
    fig(sc, r, 30, -112, 120, 0, **wave(r))
    fig(sc, r, 290, -110, 120, -20, arm_l='face', arm_r='hip', rock=2, rate=0.5, hat='cap', hat_col=TEAL)


def net_haulers(sc):
    """Four haul a net over the end of an ochre quay wall, all together; one calls the time (the Old Harbour)."""
    r = random.Random(72)
    g = sc.still()
    block(g, OCHRE, -340, 340, -100, 180, 230)
    for k in range(8):
        g.box(WOOD_L, (-300 + k * 86, -102, 60 + (k % 2) * 80), (70, 6, 34))
    for x in (-300, 60):
        g.cyl(ROCK_D, (x, 120, 230), 20, 46, steps=6, top_r=26)
    g.box(CREAM, (30, -10, 230 + 108), (520, 7, 7))          # the rope
    tilt_box(g, CREAM, (290, -10, 338), (322, -10, 236), 7, 7)
    n = sc.part(P.motion(swing=5, pivot=(322, -10, 236), axis=0.0, rate=0.5, phase=0.0))
    n.box(TEAL, (322, -10, 130), (50, 170, 210))
    for k in range(4):
        n.sphere(WHITE, (323, -70 + k * 46, 226), 14, steps=(3, 5))
    for k, x in enumerate((-230, -100, 30, 160)):
        fig(sc, r, x, 5, 230, 90, arm_l='fwd', arm_r='fwd', legs='stride', sway=dict(swing=7, pivot=(x, 5, 230), axis=90.0), rate=0.5, phase=0.0,
            top=(TEAL, CREAM, RED, WHITE)[k], hat='beanie' if k % 2 else None, hat_col=RED, build='stocky' if k == 0 else 'avg')
    fig(sc, r, -40, 110, 230 + 0, 0, **cheer(r, rate=0.5))
    g = sc.still()
    for k in range(3):
        g.box((WHITE, TEAL, WHITE)[k], (220 + (k % 2) * 20, 120, 230 + 18 + k * 36), (80, 56, 34))     # fish boxes


def accordion(sc):
    """An accordion player on a square stone plinth, two dancing on a timber stage, children clapping (the Old Harbour)."""
    r = random.Random(73)
    g = sc.still()
    block(g, WOOD, -330, 150, -110, 200, 160)
    for k in range(9):
        g.box(WOOD_L, (-306 + k * 54, -112, 90), (46, 6, 130))
    g.box(RED, (-90, -113, 152), (484, 6, 14))
    block(g, OCHRE, 170, 330, -60, 100, 250)
    g.box(CREAM, (250, 20, 256), (176, 176, 12))
    block(g, OCHRE, 170, 330, -130, -60, 130)
    g.box(WOOD, (-320, 190, 400), (12, 12, 480))
    g.box(WOOD, (140, 190, 400), (12, 12, 480))
    pennants(sc, r, -320, 140, 190, 630, (RED, WHITE, TEAL), n=6)
    fig(sc, r, 250, 20, 262, -8, top=CREAM, bottom=WOOD, hat='cap', hat_col=TEAL, arm_l='hold', arm_r='hold', swing_l=10, swing_r=-10, axis_l='z', axis_r='z', rate=0.9)
    b = sc.part(P.motion(bob=-3, rate=0.9, phase=0.2))
    b.box(RED, (250 - 5, -10, 262 + 104), (22, 20, 36))
    b.box(WHITE, (250 - 5, -10, 262 + 104), (10, 22, 38))
    b.box(RED, (250 - 22, -10, 262 + 104), (10, 22, 40))
    b.box(RED, (250 + 14, -10, 262 + 104), (10, 22, 40))
    fig(sc, r, -200, 40, 160, 20, turn=20, rate=0.8, arm_l='out', arm_r='hip', wear='skirt', top=RED, bottom=CREAM)
    fig(sc, r, -80, 40, 160, -20, turn=20, rate=0.8, arm_l='hip', arm_r='out', top=TEAL)
    fig(sc, r, 60, -40, 160, -5, **cheer(r, **kid(r)))
    fig(sc, r, -280, -50, 160, 10, **cheer(r, **kid(r)))
    fig(sc, r, 60, 110, 160, -10, **wave(r, top=CREAM))


TALL = dict(grow=(60, 60, 60), grow_down=(60, 60, 0))
JOBS = [
    ('SM_S1PEOPLE_JukePodium', juke_podium, TALL), ('SM_S1PEOPLE_DinerSteps', diner_steps, TALL),
    ('SM_S1PEOPLE_Wagon_A', wagon((CREAM, HOTPINK, TEAL, (HOTPINK, YEL, WHITE))), TALL),
    ('SM_S1PEOPLE_Wagon_B', wagon((YEL, RED, TURQ, (RED, WHITE, LAV))), TALL),
    ('SM_S1PEOPLE_NetDeck', net_deck, TALL), ('SM_S1PEOPLE_HullPainters', hull_painters, TALL),
    ('SM_S1PEOPLE_Veranda_A', veranda(HBLUE, HBLUE), TALL), ('SM_S1PEOPLE_Veranda_B', veranda(CORAL, GREEN), TALL),
    ('SM_S1PEOPLE_ShowPlinths', show_plinths, TALL), ('SM_S1PEOPLE_StiltWalkers', stilt_walkers, TALL),
    ('SM_S1PEOPLE_BalusTerrace', balus_terrace, TALL), ('SM_S1PEOPLE_StringTrio', string_trio, TALL),
    ('SM_S1PEOPLE_PaintersTerrace', painters_terrace, TALL), ('SM_S1PEOPLE_PicnicWall', picnic_wall, TALL),
    ('SM_S1PEOPLE_PremiereSteps', premiere_steps, TALL), ('SM_S1PEOPLE_PhotoRiser', photo_riser, TALL),
    ('SM_S1PEOPLE_FishStalls', fish_stalls, TALL), ('SM_S1PEOPLE_NetHaulers', net_haulers, TALL),
    ('SM_S1PEOPLE_Accordion', accordion, TALL),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
