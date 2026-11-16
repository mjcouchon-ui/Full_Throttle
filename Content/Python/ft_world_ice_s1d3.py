"""Ice Mountain, Stage 1, builder s1d3: "The valley run" (5.65 to 8.94 km), the painted valley town.
/Game/World/IceMountain/SM_S1D3_*. Run: import ft_world_ice_s1d3 as K; K.run()

Narrow townhouses of 3 to 5 storeys, gable end-on to the road, every floor jettied over the one below, bell-cast roofs in
pink shingle under a deep snow cap, dark timber frames round cream render, pink shutters, warm windows. The chase camera
looks ALONG the road, so the side walls are drawn as carefully as the fronts and every hanging sign stands square to the
front (its face looks up and down the road). Signs carry shapes only: a loaf, a sun, a rocking horse, plain arrow boards.

8 buildings, 9 props, 1 hero (THE GREAT COWBELL). One material, one section (ft_people.Scene). cm; front = -Y.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

# the kit's own colours (ft_dress_ice.py)
PINK, TIM, TIM_D, CREAM, LIT, PANE = '#ff4fb8', '#9a6a44', '#6a4c36', '#f4efe6', '#ffe28a', '#1d2740'
SNOW, SNOW_SH, ICE, ICE_D = '#f4f9ff', '#cdd6f1', '#bfe3ff', '#7fb0e0'
STONE, STONE_L, STEEL = '#5a6478', '#aab3c4', '#9aa4b8'
RED, YEL, TEAL, PINE = '#ff2d3c', '#ffd23f', '#19b5c9', '#2f6b52'
GLOW = 1.4


def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def win(g, x, y, z, w=90.0, h=130.0, face='f', lit=True, shut=True, col=PINK):
    """A window on the front (face 'f': the wall's plane is y) or on a side wall ('l' / 'r': the wall's plane is x)."""
    if face == 'f':
        if shut:
            g.box(col, (x, y - 3, z), (w * 2.1, 6, h + 8))
        g.box(PANE, (x, y - 6, z), (w, 8, h))
        if lit:
            g.box(LIT, (x, y - 10, z), (w - 24, 6, h - 28), glow=GLOW)
    else:
        s = -1.0 if face == 'l' else 1.0
        if shut:
            g.box(col, (x + s * 3, y, z), (6, w * 2.1, h + 8))
        g.box(PANE, (x + s * 6, y, z), (8, w, h))
        if lit:
            g.box(LIT, (x + s * 10, y, z), (6, w - 24, h - 28), glow=GLOW)


def bell_prof(x, z, half, rise, k0=1.0, dz=0.0):
    """The bell-cast gable: shallow at the eaves, steep to the ridge. k0 < 1 starts part-way up the eaves (the snow)."""
    def zz(k):   # height of the roof line at k x half from the ridge
        if k >= 0.62:
            return rise * 0.17 * (1.0 - k) / 0.38
        if k >= 0.27:
            return rise * (0.17 + 0.43 * (0.62 - k) / 0.35)
        return rise * (0.60 + 0.40 * (0.27 - k) / 0.27)
    ks = [k for k in (k0, 0.62, 0.27) if k <= k0]
    left = [(x - half * k, z + dz + zz(k)) for k in ks]
    right = [(x + half * k, z + dz + zz(k)) for k in reversed(ks)]
    return ccw(left + [(x, z + dz + rise)] + right)


def roof(g, x, z, w, rise, y0, y1, col=PINK, over=45.0, snow=True):
    """Ridge along Y, gable to the road. Pink shingle shows at the eaves and at the verge; snow lies on the rest."""
    half = w / 2.0 + over
    g.side(col, bell_prof(x, z, half, rise), y0 - over, y1 + over * 0.6)
    if snow:
        g.side(SNOW, bell_prof(x, z, half, rise, 0.80, 30.0), y0 - over + 16, y1 + over * 0.6 - 16)
    g.box(TIM_D, (x, (y0 + y1) / 2.0, z + 8), (w + 30, y1 - y0 + 30, 18))      # the wall plate
    return z + rise + 30


def gable(g, x, z, w, rise, y, lit=True, mark=None):
    """The gable wall under the roof's verge: cream panel, a king post, a small window."""
    half = w / 2.0 - 14
    g.side(CREAM, bell_prof(x, z, half, rise * 0.86), y - 8, y)
    g.box(TIM_D, (x, y - 11, z + rise * 0.40), (16, 8, rise * 0.78))
    g.box(TIM_D, (x, y - 11, z + rise * 0.36), (half * 1.15, 8, 14))
    if mark is None:
        g.box(PANE, (x - half * 0.36, y - 12, z + rise * 0.19), (62, 8, 70))
        g.box(PANE, (x + half * 0.36, y - 12, z + rise * 0.19), (62, 8, 70))
        if lit:
            g.box(LIT, (x + half * 0.36, y - 16, z + rise * 0.19), (42, 6, 48), glow=GLOW)


def chimney(g, x, y, z0, z1, w=62.0, col=STONE):
    g.box(col, (x, y, (z0 + z1) / 2.0), (w, w, z1 - z0))
    g.box(STONE_L, (x, y, z1 + 8), (w + 16, w + 16, 16))
    g.box(SNOW, (x, y, z1 + 24), (w + 4, w + 4, 16))


def house(g, w, d, floors, fh=285.0, jet=16.0, jy=22.0, x=0.0, y=0.0, foot=280.0, ground='stone', nwin=2, swin=2, seed=1,
          shut=PINK, braces=True, door=PINK, back_posts=True):
    """Walls only (the roof is the caller's): stone plinth, jettied floors, posts, beams, windows front and sides.
    Returns (z of the eaves, width at the top, front y at the top, back y)."""
    r = random.Random(seed)
    g.box(STONE, (x, y, 30 - foot / 2.0), (w + 36, d + 36, foot + 60))          # plinth and footing: top at z = 60
    z = 60.0
    yb = y + d / 2.0
    wi, yf = w, y - d / 2.0
    for i in range(floors):
        wi = w + 2 * jet * i
        yf = y - d / 2.0 - jy * i
        di = yb - yf
        yc = (yf + yb) / 2.0
        zc = z + fh / 2.0
        stone = (i == 0 and ground in ('stone', 'shop'))
        g.box(STONE_L if stone else CREAM, (x, yc, zc), (wi, di, fh))
        if i > 0:
            g.box(TIM_D, (x, yc - 4, z + 9), (wi + 18, di + 26, 20))             # the jetty's bressummer
        for sx in (-1, 1):                                                       # corner posts, front and back
            g.box(TIM_D if not stone else STONE, (x + sx * (wi / 2.0 - 4), yf + 6, zc), (22, 22, fh))
            if back_posts:
                g.box(TIM_D if not stone else STONE, (x + sx * (wi / 2.0 - 4), yb - 6, zc), (22, 22, fh))
        if i == 0:
            if ground == 'stone':
                g.box(door, (x - wi * 0.22, yf - 4, z + 115), (96, 8, 215))
                g.box(TIM_D, (x - wi * 0.22, yf - 6, z + 232), (124, 12, 18))
                win(g, x + wi * 0.2, yf, z + 150, 100, 110, 'f', True, True, shut)
            for sx, face in ((-1, 'l'), (1, 'r')):
                win(g, x + sx * wi / 2.0, yc + di * 0.18, z + 160, 84, 100, face, r.random() < 0.5, False)
        else:
            for k in range(nwin):
                xx = x + (k - (nwin - 1) / 2.0) * (wi / float(nwin)) * 0.92
                win(g, xx, yf, zc + 8, 84, 128, 'f', r.random() < 0.72, True, shut)
            for sx, face in ((-1, 'l'), (1, 'r')):
                for k in range(swin):
                    yy = yc + (k - (swin - 1) / 2.0) * (di / float(swin)) * 0.92
                    win(g, x + sx * wi / 2.0, yy, zc + 8, 80, 124, face, r.random() < 0.6, (i + k) % 2 == 0, shut)
                if braces and i % 2 == 1:                                        # half timbering on the side walls
                    xs = x + sx * (wi / 2.0 + 3)
                    tilt_box(g, TIM_D, (xs, yc - di * 0.46, z + 24), (xs, yc - di * 0.30, z + fh - 6), 8, 14)
                    tilt_box(g, TIM_D, (xs, yc + di * 0.46, z + 24), (xs, yc + di * 0.30, z + fh - 6), 8, 14)
        z += fh
    return z, wi, yf, yb


def flower_box(g, x, y, z, w=120.0):
    g.box(TIM, (x, y - 14, z), (w, 26, 22))
    g.box(PINK, (x, y - 14, z + 16), (w - 12, 30, 14))


def vane(sc, x, y, z, col=YEL, rate=0.11):
    """A weather vane: an arrow and a little flag swinging slowly about its mast."""
    g = sc.still()
    g.cyl(TIM_D, (x, y, z), 5, 150, steps=6)
    g.sphere(col, (x, y, z + 156), 12, steps=(4, 6))
    f = sc.part(P.motion(swing=34, pivot=(x, y, z), axis=P.UP, rate=rate, phase=0.3))
    f.box(col, (x, y, z + 110), (130, 6, 8))
    f.box(col, (x - 58, y, z + 126), (34, 6, 38))
    f.box(PINK, (x + 62, y, z + 112), (22, 8, 22), rot=(45, 0, 0))


def lantern(g, x, y, z, s=1.0):
    g.box(TIM_D, (x, y, z + 22 * s), (30 * s, 30 * s, 8 * s))
    g.box(LIT, (x, y, z), (22 * s, 22 * s, 36 * s), glow=1.6)
    g.box(TIM_D, (x, y, z - 22 * s), (26 * s, 26 * s, 8 * s))


# ---------------------------------------------------------------------------------------------- the buildings
def tall_house(sc):
    """THE TALL TOWNHOUSE: five storeys, 6.2 m wide, pink shutters, a carved balcony, a vane on the ridge."""
    g = sc.still()
    z, wi, yf, yb = house(g, 620, 700, 5, seed=11, back_posts=False)
    top = roof(g, 0, z, wi, 520, yf, yb)
    gable(g, 0, z, wi, 520, yf)
    zb = 60 + 285 * 3
    g.box(TIM_D, (0, yf - 24, zb + 4), (wi - 60, 92, 14))                        # the balcony on the fourth floor
    g.box(TIM, (0, yf - 66, zb + 52), (wi - 60, 8, 84))
    for sx in (-1, 1):
        g.box(TIM, (sx * (wi / 2.0 - 34), yf - 22, zb + 52), (8, 84, 84))
        tilt_box(g, TIM_D, (sx * (wi / 2.0 - 60), yf + 20, zb - 70), (sx * (wi / 2.0 - 60), yf - 60, zb), 12, 12)
    flower_box(g, 0, -350 - 22, 60 + 285 + 70, 300)
    chimney(g, wi * 0.22, 120, z + 180, z + 470)
    vane(sc, 0, -120, top - 34)


def arcade_house(sc):
    """THE ARCADE HOUSE: three round arches on stone piers under a jettied front, a lantern in the walk."""
    g = sc.still()
    w, d = 960.0, 760.0
    z, wi, yf, yb = house(g, w, d, 3, fh=310, jet=18, jy=30, ground='arcade', nwin=3, swin=2, seed=23)
    y0 = -d / 2.0
    g.box(PANE, (0, y0 + 70, 60 + 150), (w - 60, 150, 300))                      # the walk's shadow
    for k in range(4):                                                           # piers
        xx = -w / 2.0 + 40 + k * (w - 80) / 3.0
        g.box(STONE_L, (xx, y0 - 8, 60 + 150), (74, 60, 300))
        g.box(STONE, (xx, y0 - 8, 60 + 12), (90, 76, 24))
    g.box(STONE_L, (0, y0 - 8, 60 + 250), (w - 10, 56, 120))                     # the spandrel over the arches
    for k in range(3):                                                           # the arches' round heads, dark
        xx = -w / 2.0 + 40 + (k + 0.5) * (w - 80) / 3.0
        pts = [(xx + 108 * math.cos(math.radians(a)), 60 + 190 + 108 * math.sin(math.radians(a))) for a in range(0, 181, 30)]
        g.side(PANE, ccw(pts), y0 - 44, y0 - 36)
        g.box(STONE, (xx, y0 - 42, 60 + 190 + 100), (30, 12, 34))                # a keystone
    lantern(g, 0, y0 - 60, 60 + 150)
    g.box(TIM_D, (0, y0 - 60, 60 + 182), (6, 6, 30))
    g.box(PINK, (-300, y0 - 9, 60 + 110), (100, 8, 210))                         # the shop door inside the walk
    g.box(LIT, (120, y0 - 8, 60 + 130), (330, 6, 150), glow=GLOW)
    top = roof(g, 0, z, wi, 610, yf, yb)
    gable(g, 0, z, wi, 610, yf)
    for k in range(3):
        flower_box(g, (k - 1) * wi * 0.3, y0 - 30, 60 + 310 + 76, 110)
    chimney(g, -wi * 0.25, 60, z + 190, z + 520)
    g.box(PINK, (0, yf - 26, 60 + 310 + 290), (wi * 0.5, 8, 30))                 # a plain painted board under the top floor
    g.box(YEL, (0, yf - 30, 60 + 310 + 290), (40, 8, 40), rot=(45, 0, 0))


def bakery(sc):
    """THE BAKERY: a big plain loaf hanging square to the front, a striped awning, a great chimney that smokes."""
    g = sc.still()
    w, d = 800.0, 720.0
    z, wi, yf, yb = house(g, w, d, 3, fh=290, ground='shop', nwin=2, swin=2, seed=31)
    y0 = -d / 2.0
    g.box(PINK, (-270, y0 - 4, 60 + 112), (100, 8, 214))
    g.box(PANE, (110, y0 - 5, 60 + 150), (380, 8, 170))
    g.box(LIT, (110, y0 - 9, 60 + 150), (350, 6, 140), glow=GLOW)
    g.box(TIM, (110, y0 - 16, 60 + 96), (360, 22, 10))                           # the shelf of loaves in the window
    for k in range(5):
        g.sphere(TIM, (110 + (k - 2) * 68, y0 - 18, 60 + 116), 22, scale=(1.3, 0.8, 0.7), steps=(4, 6))
    for k in range(7):                                                           # the awning, pink and cream
        xx = 110 + (k - 3) * 62
        tilt_box(g, PINK if k % 2 == 0 else CREAM, (xx, y0 - 2, 60 + 262), (xx, y0 - 150, 60 + 214), 62, 8)
        g.box(PINK if k % 2 == 0 else CREAM, (xx, y0 - 150, 60 + 198), (62, 6, 30))
    g.box(SNOW, (110, y0 - 60, 60 + 256), (420, 90, 10), rot=(0, 0, -18))
    top = roof(g, 0, z, wi, 560, yf, yb)
    gable(g, 0, z, wi, 560, yf)
    # the sign: a wrought bracket and the loaf, face up and down the road
    f1 = y0 - 22                                                                 # the first floor's front
    sx0 = -w / 2.0 + 60
    g.box(TIM_D, (sx0, f1 - 95, 60 + 290 + 250), (10, 190, 10))
    tilt_box(g, TIM_D, (sx0, f1 - 4, 60 + 290 + 170), (sx0, f1 - 110, 60 + 290 + 246), 8, 8)
    s = sc.part(P.motion(swing=5, pivot=(sx0, f1 - 120, 60 + 290 + 250), axis=0, rate=0.3, phase=0.2))
    s.box(TIM_D, (sx0, f1 - 120, 60 + 290 + 222), (6, 6, 50))
    s.sphere(TIM, (sx0, f1 - 120, 60 + 290 + 150), 60, scale=(0.36, 1.5, 0.95), steps=(5, 8))
    for k in (-1, 0, 1):
        s.box(CREAM, (sx0, f1 - 120 + k * 46, 60 + 290 + 168), (50, 12, 34), rot=(0, 0, 24))
    # the oven's chimney and its smoke: grey puffs, square, rising and fading into each other
    chimney(g, wi * 0.24, 150, z + 120, z + 640, w=96)
    for k in range(3):
        pf = sc.part(P.motion(bob=-34, rate=0.16, phase=0.33 * k))
        pf.box(STONE_L, (wi * 0.24 + 26 * k, 150, z + 720 + k * 96), (70 + 22 * k, 70 + 22 * k, 60 + 16 * k), rot=(0, 45, 0))


def toy_maker(sc):
    """THE TOY-MAKER'S: narrow, four storeys; a wooden wheel in the gable, a turning fan in a cupola on the ridge,
    a rocking horse for a sign and a shelf of plain toys over the door."""
    g = sc.still()
    w, d = 520.0, 640.0
    z, wi, yf, yb = house(g, w, d, 4, fh=280, jet=18, jy=24, nwin=2, swin=2, seed=47, shut=TEAL, door=TEAL)
    rise = 500.0
    top = roof(g, 0, z, wi, rise, yf, yb)
    gable(g, 0, z, wi, rise, yf, mark='wheel')
    zc = z + rise * 0.30
    g.cyl(TIM_D, (0, yf - 24, zc), 86, 10, rot=(0, 0, 90), steps=12)            # the wheel in the gable
    g.cyl(YEL, (0, yf - 30, zc), 70, 8, rot=(0, 0, 90), steps=12)
    for k in range(4):
        g.box(TIM_D, (0, yf - 34, zc), (150, 8, 12), rot=(k * 45, 0, 0))
    g.cyl(PINK, (0, yf - 44, zc), 18, 12, rot=(0, 0, 90), steps=8)
    y0 = -d / 2.0
    g.box(TIM, (60, y0 - 30, 60 + 262), (300, 44, 10))                           # the shelf of toys: blocks, a cone, a ball
    g.box(RED, (-50, y0 - 30, 60 + 287), (40, 40, 40))
    g.box(YEL, (0, y0 - 30, 60 + 284), (34, 34, 34), rot=(0, 30, 0))
    g.cyl(TEAL, (52, y0 - 30, 60 + 267), 22, 52, steps=6, top_r=3)
    g.sphere(PINK, (104, y0 - 30, 60 + 290), 23, steps=(4, 6))
    g.box(CREAM, (160, y0 - 30, 60 + 282), (50, 30, 30))
    # the cupola on the ridge and its fan
    zr = top - 40
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(TIM_D, (sx * 62, 40 + sy * 62, zr + 80), (14, 14, 200))
    g.box(TIM_D, (0, 40, zr + 4), (170, 170, 16))
    g.cyl(PINK, (0, 40, zr + 176), 150, 130, rot=(0, 45, 0), steps=4, top_r=8)
    g.cyl(SNOW, (0, 40, zr + 214), 104, 100, rot=(0, 45, 0), steps=4, top_r=6)
    f = sc.part(P.motion(travel=(-1, 0.11), base=(0, 40)))
    f.cyl(TIM, (0, 40, zr + 20), 7, 150, steps=6)
    for k in range(6):
        a = math.radians(k * 60)
        f.box(YEL if k % 2 else RED, (math.cos(a) * 26, 40 + math.sin(a) * 26, zr + 120), (44, 8, 26), rot=(0, k * 60, 28))
    f.box(TEAL, (0, 40, zr + 62), (70, 70, 8), rot=(0, 20, 0))
    for k in range(4):
        a = math.radians(k * 90 + 20)
        f.box((PINK, CREAM, YEL, RED)[k], (math.cos(a) * 30, 40 + math.sin(a) * 30, zr + 80), (14, 14, 28))
    # the sign: a rocking horse, face up and down the road, rocking
    f1 = y0 - 24
    px, py, pz = w / 2.0 - 40, f1 - 120, 60 + 280 + 250
    g.box(TIM_D, (px, f1 - 85, pz), (10, 170, 10))
    h = sc.part(P.motion(swing=9, pivot=(px, py, pz - 150), axis=0, rate=0.45, phase=0.6))
    h.box(TIM_D, (px, py, pz - 20), (6, 6, 44))
    h.box(CREAM, (px, py, pz - 78), (10, 92, 40))
    h.box(CREAM, (px, py - 50, pz - 44), (10, 28, 56), rot=(0, 0, -24))
    h.box(CREAM, (px, py - 66, pz - 22), (10, 40, 22))
    h.box(PINK, (px, py + 4, pz - 56), (12, 34, 10))
    for sy in (-30, 28):
        h.box(CREAM, (px, py + sy, pz - 112), (10, 12, 40))
    h.box(RED, (px, py, pz - 138), (10, 128, 12))
    h.box(RED, (px, py - 62, pz - 130), (10, 14, 22), rot=(0, 0, 30))
    h.box(RED, (px, py + 62, pz - 130), (10, 14, 22), rot=(0, 0, -30))
    chimney(g, -wi * 0.24, 190, z + 200, z + 430, w=54)


def town_gate(sc):
    """THE TOWN GATE: a stone tower with a dark passage facing the road (nothing drives through), two jettied timber
    storeys, a boarded gallery and a bell-cast pyramid roof; lanterns by the arch, two long pennants."""
    g = sc.still()
    w = 640.0
    g.box(STONE, (0, 0, -110), (w + 60, w + 60, 340))                            # footing: top at 60
    g.box(STONE_L, (0, 0, 60 + 230), (w, w, 460))
    for sx in (-1, 1):                                                           # quoins and buttresses
        g.box(STONE, (sx * (w / 2.0 - 30), -w / 2.0 - 14, 60 + 150), (90, 60, 300))
        g.box(STONE, (sx * (w / 2.0 - 30), -w / 2.0 - 8, 60 + 320), (70, 40, 60))
        for k in range(3):
            g.box(STONE, (sx * (w / 2.0 + 2), (k - 1) * 200, 60 + 110 + k * 120), (12, 80, 46))
    for sy in (-1, 1):                                                           # the passage, front and back
        yy = sy * (w / 2.0)
        g.box(PANE, (0, yy, 60 + 135), (280, 30, 270))
        g.cyl(PANE, (0, yy - 15, 60 + 270), 140, 30, rot=(0, 0, 90), steps=12)
        for k in range(5):                                                       # the raised portcullis: timber teeth
            g.box(TIM_D, ((k - 2) * 56, yy + sy * 18, 60 + 340), (16, 10, 110))
        g.box(TIM_D, (0, yy + sy * 18, 60 + 372), (270, 10, 16))
    for sx in (-1, 1):
        lantern(g, sx * 210, -w / 2.0 - 30, 60 + 250, 1.3)
        g.box(TIM_D, (sx * 210, -w / 2.0 - 14, 60 + 290), (10, 44, 10))
    g.box(PINK, (0, -w / 2.0 - 6, 60 + 440), (110, 10, 110), rot=(45, 0, 0))     # a plain painted lozenge over the arch
    g.box(CREAM, (0, -w / 2.0 - 12, 60 + 440), (56, 10, 56), rot=(45, 0, 0))
    z = 520.0
    r = random.Random(5)
    for i in range(2):                                                           # two jettied timber storeys
        wi = w + 50 + 44 * i
        g.box(TIM_D, (0, 0, z + 10), (wi + 20, wi + 20, 22))
        g.box(CREAM, (0, 0, z + 150), (wi, wi, 280))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(TIM_D, (sx * (wi / 2.0 - 4), sy * (wi / 2.0 - 4), z + 150), (24, 24, 280))
            for k in (-1, 1):
                win(g, k * wi * 0.22, -wi / 2.0, z + 160, 80, 120, 'f', r.random() < 0.7, True)
                win(g, sx * wi / 2.0, k * wi * 0.22, z + 160, 80, 120, 'l' if sx < 0 else 'r', r.random() < 0.6, k > 0)
            xs = sx * (wi / 2.0 + 3)
            tilt_box(g, TIM_D, (xs, -40, z + 24), (xs, 40, z + 270), 8, 14)
            tilt_box(g, TIM_D, (xs, 40, z + 24), (xs, -40, z + 270), 8, 14)
        tilt_box(g, TIM_D, (-40, -wi / 2.0 - 3, z + 24), (40, -wi / 2.0 - 3, z + 270), 8, 14)
        tilt_box(g, TIM_D, (40, -wi / 2.0 - 3, z + 24), (-40, -wi / 2.0 - 3, z + 270), 8, 14)
        z += 290
    wg = w + 190                                                                 # the gallery: boards, a band of pink
    g.box(TIM_D, (0, 0, z + 10), (wg + 16, wg + 16, 22))
    g.box(TIM, (0, 0, z + 100), (wg, wg, 160))
    g.box(PINK, (0, 0, z + 150), (wg + 8, wg + 8, 26))
    for k in range(5):
        for sy in (-1, 1):
            g.box(PANE, ((k - 2) * wg * 0.19, sy * (wg / 2.0 + 1), z + 96), (60, 8, 60))
            g.box(PANE, (sy * (wg / 2.0 + 1), (k - 2) * wg * 0.19, z + 96), (8, 60, 60))
    z += 180
    rr = (wg / 2.0 + 60) * 1.4142
    g.cyl(PINK, (0, 0, z), rr, 170, rot=(0, 45, 0), steps=4, top_r=rr * 0.62)    # the bell-cast skirt, then the spire
    g.cyl(PINK, (0, 0, z + 170), rr * 0.62, 520, rot=(0, 45, 0), steps=4, top_r=14)
    g.cyl(SNOW, (0, 0, z + 60), rr * 0.84, 126, rot=(0, 45, 0), steps=4, top_r=rr * 0.60)
    g.cyl(SNOW, (0, 0, z + 196), rr * 0.60, 300, rot=(0, 45, 0), steps=4, top_r=rr * 0.26)
    g.cyl(TIM_D, (0, 0, z + 680), 7, 260, steps=6)
    g.sphere(YEL, (0, 0, z + 700), 22, steps=(4, 6))
    for k, (col, zz) in enumerate(((PINK, 900), (CREAM, 850))):                  # two pennants streaming
        f = sc.part(P.motion(swing=14, pivot=(0, 0, z + zz), axis=P.UP, rate=0.5, phase=0.4 * k))
        f.box(col, (95, 0, z + zz), (180, 6, 40))
        f.box(col, (215, 0, z + zz - 8), (70, 6, 22))


def granary(sc):
    """THE GRANARY: a log store on six stone staddles (mushroom stones), a pink door up a ladder, a hoist with a sack."""
    g = sc.still()
    w, d = 760.0, 540.0
    for sx in (-1, 0, 1):
        for sy in (-1, 1):
            xx, yy = sx * (w / 2.0 - 70), sy * (d / 2.0 - 70)
            g.cyl(STONE, (xx, yy, -260), 36, 370, steps=6, top_r=26)
            g.cyl(STONE_L, (xx, yy, 110), 66, 18, steps=8)
    g.box(TIM_D, (0, 0, 142), (w + 30, d + 30, 28))
    z = 156.0
    for k in range(7):                                                           # the logs: two tones, their ends proud
        g.box(TIM if k % 2 else TIM_D, (0, 0, z + 28 + k * 56), (w + (16 if k % 2 else 0), d + (0 if k % 2 else 16), 56))
    zt = z + 7 * 56
    g.box(PINK, (-150, -d / 2.0 - 6, z + 110), (120, 10, 200))                   # the door, pink, with a snow sill
    g.box(TIM_D, (-150, -d / 2.0 - 8, z + 218), (150, 12, 18))
    g.box(TIM_D, (-150, -d / 2.0 - 60, z - 4), (170, 110, 12))                   # the landing and its ladder
    for sx in (-1, 1):
        tilt_box(g, TIM, (-150 + sx * 40, -d / 2.0 - 110, z - 6), (-150 + sx * 40, -d / 2.0 - 230, -40), 10, 10)
    for k in range(4):
        g.box(TIM, (-150, -d / 2.0 - 128 - k * 26, z - 30 - k * 40), (90, 10, 8))
    win(g, 190, -d / 2.0, z + 250, 70, 80, 'f', False, True)
    for sx, face in ((-1, 'l'), (1, 'r')):
        win(g, sx * (w / 2.0 + 8), 0, z + 250, 70, 80, face, True, True)
        for k in range(3):                                                       # corn hung to dry under the eaves, yellow
            g.box(YEL, (sx * (w / 2.0 + 14), (k - 1) * 150, zt - 40), (10, 70, 46))
    top = roof(g, 0, zt, w + 16, 430, -d / 2.0, d / 2.0, over=70)
    gable(g, 0, zt, w + 16, 430, -d / 2.0 - 8, lit=False)
    g.box(TIM_D, (190, -d / 2.0 - 90, zt + 250), (14, 240, 14))                  # the hoist beam, its rope and sack
    s = sc.part(P.motion(swing=4, pivot=(190, -d / 2.0 - 190, zt + 250), axis=0, rate=0.25, phase=0.1))
    s.box(TIM, (190, -d / 2.0 - 190, zt + 150), (5, 5, 200))
    s.sphere(CREAM, (190, -d / 2.0 - 190, zt + 20), 42, scale=(1.0, 1.0, 1.4), steps=(4, 6))
    s.box(TIM_D, (190, -d / 2.0 - 190, zt + 72), (26, 26, 10))


def oriel_house(sc):
    """THE ORIEL HOUSE: four storeys with a round corner oriel under its own bell-cast cap."""
    g = sc.still()
    w, d = 740.0, 720.0
    z, wi, yf, yb = house(g, w, d, 4, fh=290, nwin=2, swin=2, seed=59)
    top = roof(g, 0, z, wi, 560, yf, yb)
    gable(g, 0, z, wi, 560, yf)
    ox, oy = -w / 2.0 - 10, -d / 2.0 - 30                                        # the oriel: on a corbel from the first floor up
    z0 = 60 + 290
    g.cyl(TIM_D, (ox, oy, z0 - 110), 40, 110, steps=8, top_r=150)
    g.cyl(CREAM, (ox, oy, z0), 150, 290 * 3 + 40, steps=8)
    for i in range(3):
        g.cyl(TIM_D, (ox, oy, z0 + i * 290 - 6), 158, 20, steps=8)
        for a in (180, 225, 270, 315):
            ar = math.radians(a)
            g.box(PANE, (ox + math.cos(ar) * 146, oy + math.sin(ar) * 146, z0 + i * 290 + 160), (70, 12, 120), rot=(0, a + 90, 0))
            if (i + a // 45) % 2 == 0:
                g.box(LIT, (ox + math.cos(ar) * 151, oy + math.sin(ar) * 151, z0 + i * 290 + 160), (48, 8, 94), rot=(0, a + 90, 0), glow=GLOW)
    zc = z0 + 290 * 3 + 40
    g.cyl(PINK, (ox, oy, zc), 215, 80, steps=8, top_r=140)                       # the cap: flared, then steep
    g.cyl(PINK, (ox, oy, zc + 80), 140, 330, steps=8, top_r=8)
    g.cyl(SNOW, (ox, oy, zc + 40), 176, 60, steps=8, top_r=132)
    g.cyl(SNOW, (ox, oy, zc + 100), 126, 150, steps=8, top_r=68)
    g.sphere(YEL, (ox, oy, zc + 420), 18, steps=(4, 6))
    for k in (-1, 1):
        flower_box(g, 120 + k * 150, -d / 2.0 - 44, 60 + 290 * 2 + 70, 110)
    chimney(g, wi * 0.26, 100, z + 180, z + 500)


def gallery_inn(sc):
    """THE INN: three storeys, timber galleries along both side walls on carved brackets, a plain sun for a sign."""
    g = sc.still()
    w, d = 880.0, 700.0
    z, wi, yf, yb = house(g, w, d, 3, fh=300, jet=10, jy=24, nwin=3, swin=2, seed=71, braces=False)
    for i in (1, 2):                                                             # the galleries
        zz = 60 + 300 * i
        wf = w + 20 * i
        for sx in (-1, 1):
            xs = sx * (wf / 2.0 + 56)
            g.box(TIM_D, (xs, 10, zz + 6), (112, d - 60, 14))
            g.box(TIM, (sx * (wf / 2.0 + 108), 10, zz + 100), (8, d - 60, 14))
            g.box(PINK, (sx * (wf / 2.0 + 108), 10, zz + 56), (8, d - 100, 50))
            for k in range(5):
                yy = 10 + (k - 2) * (d - 80) / 4.0
                g.box(TIM, (sx * (wf / 2.0 + 106), yy, zz + (56 if i == 1 else 150)), (12, 12, 100 if i == 1 else 290))
            for k in (-1, 1):
                tilt_box(g, TIM_D, (sx * (wf / 2.0 + 4), k * (d / 2.0 - 70), zz - 90), (sx * (wf / 2.0 + 100), k * (d / 2.0 - 70), zz), 12, 12)
    top = roof(g, 0, z, wi + 230, 600, yf, yb, over=30)                          # one wide roof over house and galleries
    gable(g, 0, z, wi, 600 * 0.8, yf)
    f1 = -d / 2.0 - 24
    for k in range(3):
        flower_box(g, (k - 1) * wi * 0.3, f1, 60 + 300 + 78, 110)
    px, pz = w / 2.0 - 40, 60 + 300 + 250                                        # the sign: a sun on a bracket
    g.box(TIM_D, (px, f1 - 115, pz), (10, 230, 10))
    tilt_box(g, TIM_D, (px, f1 - 4, pz - 90), (px, f1 - 120, pz - 6), 8, 8)
    s = sc.part(P.motion(swing=5, pivot=(px, f1 - 150, pz), axis=0, rate=0.28, phase=0.5))
    s.box(TIM_D, (px, f1 - 150, pz - 24), (6, 6, 44))
    s.cyl(YEL, (px - 6, f1 - 150, pz - 110), 52, 12, rot=(-90, 0, 0), steps=10)
    for k in range(4):
        s.box(YEL, (px, f1 - 150, pz - 110), (8, 150, 16), rot=(0, 0, k * 45))
    s.cyl(PINK, (px - 9, f1 - 150, pz - 110), 24, 18, rot=(-90, 0, 0), steps=8)
    chimney(g, -wi * 0.2, 80, z + 200, z + 560, w=78)
    lantern(g, 120, -d / 2.0 - 24, 60 + 250)
    g.box(TIM_D, (120, -d / 2.0 - 12, 60 + 282), (8, 30, 8))


# ---------------------------------------------------------------------------------------------- the props
def fountain_trough(sc):
    """LOW. A carved stone trough, frozen over, icicles at the spout, a bucket."""
    g = sc.still()
    g.box(STONE, (0, 0, -15), (300, 150, 70))
    g.box(STONE_L, (0, 0, 55), (260, 110, 70))
    g.box(STONE, (0, 0, 92), (276, 126, 12))
    g.box(ICE, (0, 0, 96), (232, 84, 10))
    g.box(ICE_D, (30, -6, 98), (90, 30, 8), rot=(0, 20, 0))
    for sx in (-1, 1):
        g.box(PINK, (sx * 80, -57, 58), (40, 6, 40), rot=(45, 0, 0))             # painted lozenges on the front
    g.box(STONE_L, (-150, 0, 80), (54, 54, 200))                                 # the spout's post
    g.box(STONE, (-150, 0, 172), (70, 70, 16))
    g.box(SNOW, (-150, 0, 186), (60, 60, 12))
    g.box(TIM_D, (-112, 0, 136), (60, 12, 12))
    for k, (dx, hh) in enumerate(((0, 34), (14, 22), (-12, 16))):
        g.cyl(ICE_D, (-92 + dx, 0, 130 - hh), 2.5, hh, steps=5, top_r=6)
    g.cyl(TIM, (120, -96, 0), 22, 40, steps=8, top_r=26)                         # the bucket
    g.cyl(ICE, (120, -96, 34), 22, 6, steps=8)
    g.sphere(SNOW, (0, 70, 14), 130, scale=(1.3, 0.4, 0.2), steps=(4, 8))


def sled_rack(sc):
    """LOW. A timber rack of plain sleds, pink, teal, yellow, red and cream, one lying in front."""
    g = sc.still()
    for sx in (-1, 1):
        g.box(TIM_D, (sx * 170, 20, 62), (14, 14, 244))
        tilt_box(g, TIM_D, (sx * 170, -50, 0), (sx * 170, 20, 150), 10, 10)
    g.box(TIM_D, (0, 20, 160), (370, 12, 12))
    g.box(TIM_D, (0, 20, 60), (370, 12, 12))
    g.box(SNOW, (0, 20, 172), (350, 22, 12))
    cols = (PINK, TEAL, YEL, RED, CREAM)
    for k in range(5):                                                           # sleds leaning on the rail
        xx = (k - 2) * 64
        for dx in (-17, 17):
            tilt_box(g, TIM, (xx + dx, -46, 8), (xx + dx, 10, 140), 6, 10)
        tilt_box(g, cols[k], (xx, -34, 18), (xx, 4, 120), 40, 8)
    for dx in (-17, 17):                                                         # one on the snow
        g.box(TIM, (210 + dx, -70, 8), (6, 110, 10))
    g.box(PINK, (210, -70, 24), (40, 96, 8))
    g.box(STONE, (0, 0, -30), (380, 60, 60))


def ribbon_pole(sc):
    """TALL. A festival pole banded pink and cream, a green wreath near the top, eight long ribbons streaming."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -120), 60, 150, steps=8)
    g.cyl(SNOW, (0, 0, 30), 52, 14, steps=8)
    for k in range(8):
        g.cyl(PINK if k % 2 == 0 else CREAM, (0, 0, 30 + k * 110), 14 - k * 0.6, 110, steps=8)
    zt = 910.0
    for k in range(8):                                                           # the wreath
        a = math.radians(k * 45)
        g.box(PINE, (math.cos(a) * 82, math.sin(a) * 82, zt - 60), (70, 26, 26), rot=(0, k * 45 + 90, 0))
        if k % 2 == 0:
            g.box(PINK, (math.cos(a) * 98, math.sin(a) * 98, zt - 60), (20, 20, 20))
    for k in range(4):
        a = math.radians(k * 90 + 45)
        tilt_box(g, TIM_D, (0, 0, zt - 10), (math.cos(a) * 80, math.sin(a) * 80, zt - 56), 5, 5)
    g.sphere(YEL, (0, 0, zt + 14), 20, steps=(4, 6))
    for k in range(8):                                                           # the ribbons: each sways on its own
        a = math.radians(k * 45 + 10)
        ax = (k * 45 + 100) % 180
        f = sc.part(P.motion(swing=9, pivot=(0, 0, zt), axis=ax, rate=0.35 + 0.03 * (k % 3), phase=0.13 * k))
        tilt_box(f, PINK if k % 2 == 0 else CREAM, (math.cos(a) * 10, math.sin(a) * 10, zt - 4),
                 (math.cos(a) * 210, math.sin(a) * 210, zt - 420), 16, 6)
        tilt_box(f, PINK if k % 2 == 0 else CREAM, (math.cos(a) * 210, math.sin(a) * 210, zt - 420),
                 (math.cos(a) * 290, math.sin(a) * 290, zt - 520), 16, 6)


def market_stall(sc):
    """TALL (3.2 m). A stall under a striped awning: cheeses, loaves, a crate of red apples, a lit lantern."""
    g = sc.still()
    g.box(STONE, (0, 0, -50), (340, 200, 100))
    for sx in (-1, 1):
        g.box(TIM_D, (sx * 160, 80, 140), (12, 12, 290))
        g.box(TIM_D, (sx * 160, -80, 115), (12, 12, 240))
    g.box(TIM, (0, -70, 50), (330, 60, 100))
    g.box(TIM_D, (0, -70, 104), (350, 76, 10))
    g.box(TIM, (0, 60, 44), (330, 50, 88))
    for k in range(7):
        xx = (k - 3) * 52
        col = PINK if k % 2 == 0 else CREAM
        tilt_box(g, col, (xx, 96, 292), (xx, -112, 232), 52, 8)
        g.box(col, (xx, -114, 214), (52, 6, 30))
    g.box(SNOW, (0, 20, 282), (300, 110, 10), rot=(0, 0, -16))
    for k in range(3):
        g.cyl(YEL, (-110 + k * 22, -70, 109 + k * 14), 30 - k * 3, 14, steps=8)
    for k in range(3):
        g.sphere(TIM, (-10 + k * 46, -72, 122), 20, scale=(1.3, 0.8, 0.7), steps=(4, 6))
    g.box(TIM_D, (120, -70, 120), (70, 50, 24))
    for k in range(4):
        g.sphere(RED, (100 + (k % 2) * 36, -80 + (k // 2) * 24, 136), 12, steps=(4, 6))
    lantern(g, 0, -30, 200)
    g.box(TIM_D, (0, -30, 240), (6, 6, 40))
    for k in range(5):                                                           # a line of plain pennons on the valance
        f = sc.part(P.motion(swing=10, pivot=((k - 2) * 62, -118, 198), axis=0, rate=0.6, phase=0.2 * k))
        f.box((YEL, TEAL, PINK, CREAM, YEL)[k], ((k - 2) * 62, -118, 182), (30, 5, 32))
    g.box(TIM, (150, 110, 30), (60, 46, 60))                                     # a barrel and a sack behind
    g.sphere(CREAM, (-150, 112, 24), 34, scale=(1.0, 1.0, 1.2), steps=(4, 6))


def woodstack(sc):
    """LOW. Split firewood under a little shingle roof, an axe in the chopping block."""
    g = sc.still()
    g.box(STONE, (0, 0, -30), (330, 110, 60))
    for sx in (-1, 1):
        g.box(TIM_D, (sx * 150, 0, 78), (14, 90, 156))
    g.box(TIM_D, (0, 6, 70), (290, 80, 132))
    for row in range(4):
        for k in range(7):
            g.cyl(TIM if (row + k) % 3 else CREAM, (-126 + k * 42 + (row % 2) * 12, -36, 24 + row * 33), 16, 12, rot=(0, 0, 90), steps=6)
    g.box(PINK, (0, -4, 158), (350, 150, 12), rot=(0, 0, -8))
    g.box(SNOW, (0, 8, 170), (330, 110, 12), rot=(0, 0, -8))
    g.cyl(TIM, (215, -30, -40), 30, 90, steps=8)                                 # the block and the axe
    tilt_box(g, TIM_D, (215, -30, 56), (250, -30, 120), 7, 7)
    g.box(STEEL, (213, -30, 58), (26, 8, 20))
    g.box(TIM, (-200, -50, 10), (60, 20, 20), rot=(0, 30, 0))
    g.box(TIM, (-212, -20, 10), (60, 20, 20), rot=(0, -20, 0))


def lantern_post(sc):
    """TALL (4.4 m). A carved timber post, two lanterns on a wrought arm, steady; a pink bow whose tails stir."""
    g = sc.still()
    g.box(STONE, (0, 0, -60), (70, 70, 160))
    g.box(SNOW, (0, 0, 26), (62, 62, 12))
    g.box(TIM_D, (0, 0, 200), (20, 20, 380))
    g.box(TIM, (0, 0, 120), (30, 30, 30))
    g.box(TIM, (0, 0, 300), (28, 28, 20))
    g.box(TIM_D, (0, 0, 392), (16, 170, 12))                                     # the arm lies across: seen spread from the road
    for sy in (-1, 1):
        tilt_box(g, TIM_D, (0, sy * 10, 330), (0, sy * 66, 388), 7, 7)
        g.box(TIM_D, (0, sy * 74, 376), (6, 6, 22))
        lantern(g, 0, sy * 74, 340, 1.2)
        g.box(SNOW, (0, sy * 74, 374), (34, 34, 8))
    g.box(PINK, (0, 0, 410), (30, 30, 30), rot=(0, 45, 0))
    g.box(SNOW, (0, 0, 432), (24, 24, 12))
    g.box(PINK, (0, -13, 250), (46, 8, 22))
    for k, sx in enumerate((-1, 1)):
        f = sc.part(P.motion(swing=8, pivot=(sx * 8, -14, 244), axis=90, rate=0.5, phase=0.3 * k))
        tilt_box(f, PINK, (sx * 8, -14, 244), (sx * 26, -14, 176), 6, 14)


def wash_line(sc):
    """TALL (3 m). Two posts and a line of washing, pink, cream, teal and yellow, each piece lifting in the wind."""
    g = sc.still()
    for sx in (-1, 1):
        g.box(STONE, (sx * 320, 0, -50), (50, 50, 110))
        g.box(TIM_D, (sx * 320, 0, 150), (14, 14, 300))
        g.box(TIM_D, (sx * 320, 0, 286), (14, 70, 10))
        g.box(SNOW, (sx * 320, 0, 304), (20, 70, 8))
    rod(g, TIM, (-320, 0, 280), (320, 0, 280), 2.5, steps=5)
    cloth = ((PINK, 86, 110), (CREAM, 120, 80), (TEAL, 60, 96), (CREAM, 70, 130), (YEL, 90, 70), (PINK, 64, 100))
    x = -270.0
    for k, (col, cw, ch) in enumerate(cloth):
        f = sc.part(P.motion(swing=11, pivot=(x + cw / 2.0, 0, 280), axis=0, rate=0.4 + 0.05 * (k % 3), phase=0.17 * k))
        f.box(col, (x + cw / 2.0, 0, 280 - ch / 2.0), (cw, 5, ch))
        if k % 2 == 0:
            f.box(CREAM if col != CREAM else PINK, (x + cw / 2.0, -1, 280 - ch * 0.7), (cw * 0.6, 7, 12))
        x += cw + 16
    g.cyl(TIM, (120, -60, 0), 34, 30, steps=8, top_r=40)                         # the basket
    g.box(CREAM, (120, -60, 34), (50, 40, 14), rot=(0, 20, 0))


def finger_post(sc):
    """TALL (3.9 m). Three plain arrow boards (pink, cream, yellow: no words) and a cow for a weather vane."""
    g = sc.still()
    g.box(STONE, (0, 0, -60), (64, 64, 150))
    g.box(SNOW, (0, 0, 20), (56, 56, 12))
    g.box(TIM_D, (0, 0, 170), (18, 18, 320))
    arrow = [(-70, -16), (50, -16), (50, -28), (86, 0), (50, 28), (50, 16), (-70, 16)]
    for k, (col, zz, yaw) in enumerate(((PINK, 290, 0), (CREAM, 240, 180), (YEL, 190, 25))):
        g.side(col, ccw(arrow), -14 - 2 * k, -6 - 2 * k, rot=(0, yaw, 0), loc=(0, 0, zz))
        g.side(TIM_D, ccw([(-56, -4), (40, -4), (40, 4), (-56, 4)]), -18 - 2 * k, -12 - 2 * k, rot=(0, yaw, 0), loc=(0, 0, zz))
    g.box(SNOW, (0, -10, 322), (110, 14, 8))
    g.cyl(TIM_D, (0, 0, 330), 5, 60, steps=6)
    f = sc.part(P.motion(swing=24, pivot=(0, 0, 360), axis=P.UP, rate=0.12, phase=0.4))
    f.box(YEL, (0, 0, 372), (70, 8, 30))                                          # the cow: body, head, horns, legs, tail
    f.box(YEL, (44, 0, 384), (22, 8, 24))
    f.box(CREAM, (50, 0, 400), (6, 8, 10))
    f.box(YEL, (-24, 0, 350), (8, 8, 16))
    f.box(YEL, (22, 0, 350), (8, 8, 16))
    f.box(YEL, (-40, 0, 372), (8, 6, 26), rot=(20, 0, 0))
    f.box(PINK, (36, 0, 364), (10, 10, 12))                                       # and her bell


def bench_planter(sc):
    """LOW. A bench with pink cushions between two half barrels of alpine rose, snow on the back rail."""
    g = sc.still()
    g.box(STONE, (0, 0, -40), (360, 90, 80))
    for sx in (-1, 1):
        g.box(TIM_D, (sx * 90, 0, 24), (12, 60, 48))
        g.box(TIM_D, (sx * 90, 26, 76), (12, 10, 80))
        g.cyl(TIM, (sx * 170, 0, 0), 40, 56, steps=8, top_r=46)
        g.cyl(TIM_D, (sx * 170, 0, 30), 47, 8, steps=8)
        g.sphere(PINE, (sx * 170, 0, 70), 38, scale=(1.0, 1.0, 0.6), steps=(4, 6))
        for k in range(4):
            a = math.radians(k * 90 + 30 * sx)
            g.sphere(PINK, (sx * 170 + math.cos(a) * 24, math.sin(a) * 24, 86), 15, steps=(4, 6))
        g.sphere(PINK, (sx * 170, 0, 98), 15, steps=(4, 6))
    g.box(TIM, (0, 0, 50), (210, 60, 8))
    g.box(TIM, (0, 30, 96), (210, 8, 44))
    g.box(PINK, (-50, -2, 60), (80, 50, 12))
    g.box(CREAM, (50, -2, 60), (80, 50, 12))
    g.box(SNOW, (0, 30, 122), (200, 14, 10))
    g.box(TIM, (0, -52, 14), (70, 30, 28))                                       # a footstool with a folded pink blanket
    g.box(PINK, (0, -52, 32), (60, 26, 8))


# ---------------------------------------------------------------------------------------------- the hero
def great_cowbell(sc):
    """THE GREAT COWBELL: a 50 m timber belfry on a stone foot, open at the top, where one enormous pink cowbell
    (8 m, a flat-sided herd bell with a cream band and a strap) swings slowly under a steep shingled spire."""
    g = sc.still()
    r = random.Random(9)
    g.box(STONE, (0, 0, -100), (1500, 1500, 400))                                # footing: top at 100
    g.box(STONE_L, (0, 0, 100 + 300), (1300, 1300, 600))                         # the stone foot, a dark arch each face
    for a in range(4):
        ar = math.radians(a * 90)
        cx, cy = int(round(math.cos(ar))), int(round(math.sin(ar)))      # whole numbers: "if cx" must be False across the axis
        g.box(PANE, (cx * 650, cy * 650, 100 + 190), (300 if cy else 20, 300 if cx else 20, 380))
        g.cyl(PANE, (cx * 662, cy * 662, 100 + 380), 150, 22, rot=(0, a * 90 + 90, 90), steps=12)
        g.box(STONE, (cx * 660, cy * 660, 100 + 560), (700 if cy else 30, 700 if cx else 30, 40))
        for k in (-1, 1):                                                        # buttresses at the corners
            g.box(STONE, (cx * 640 - cy * k * 590, cy * 640 + cx * k * 590, 100 + 230), (130, 130, 460))
            lantern(g, cx * 690 - cy * k * 250, cy * 690 + cx * k * 250, 100 + 330, 1.8)
    g.box(SNOW, (0, 0, 712), (1340, 1340, 24))
    # the timber shaft: four storeys, tapering, cream panels in a dark frame, crossed braces, lit windows
    z = 720.0
    for i in range(4):
        wi = 1080.0 - i * 40
        fh = 520.0
        g.box(CREAM, (0, 0, z + fh / 2.0), (wi, wi, fh))
        g.box(TIM_D, (0, 0, z + 14), (wi + 40, wi + 40, 30))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(TIM_D, (sx * (wi / 2.0 - 6), sy * (wi / 2.0 - 6), z + fh / 2.0), (44, 44, fh))
        for a in range(4):
            ar = math.radians(a * 90)
            cx, cy = int(round(math.cos(ar))), int(round(math.sin(ar)))      # whole numbers: "if cx" must be False across the axis
            fx, fy = cx * (wi / 2.0 + 5), cy * (wi / 2.0 + 5)
            for k in (-1, 1):
                p0 = (fx - cy * k * (wi / 2.0 - 40), fy + cx * k * (wi / 2.0 - 40), z + 40)
                p1 = (fx - cy * k * 110, fy + cx * k * 110, z + fh - 20)
                tilt_box(g, TIM_D, p0, p1, 12, 26)
            lit = r.random() < 0.7
            g.box(PANE, (cx * (wi / 2.0 + 6), cy * (wi / 2.0 + 6), z + fh * 0.5), (140 if cy else 14, 140 if cx else 14, 210))
            g.box(PINK, (cx * (wi / 2.0 + 3), cy * (wi / 2.0 + 3), z + fh * 0.5), (290 if cy else 8, 290 if cx else 8, 220))
            if lit:
                g.box(LIT, (cx * (wi / 2.0 + 11), cy * (wi / 2.0 + 11), z + fh * 0.5), (100 if cy else 10, 100 if cx else 10, 170), glow=GLOW)
        z += fh
    # z = 2800: the gallery, jettied on brackets, a balustrade with pink panels
    wg = 1420.0
    for a in range(4):
        ar = math.radians(a * 90)
        cx, cy = int(round(math.cos(ar))), int(round(math.sin(ar)))      # whole numbers: "if cx" must be False across the axis
        for k in (-1, 0, 1):
            p0 = (cx * 480 - cy * k * 400, cy * 480 + cx * k * 400, z - 220)
            p1 = (cx * 690 - cy * k * 400, cy * 690 + cx * k * 400, z - 10)
            tilt_box(g, TIM_D, p0, p1, 24, 24)
        g.box(TIM, (cx * (wg / 2.0 - 6), cy * (wg / 2.0 - 6), z + 130), (wg if cy else 12, wg if cx else 12, 20))
        g.box(PINK, (cx * (wg / 2.0 - 6), cy * (wg / 2.0 - 6), z + 78), (wg - 80 if cy else 10, wg - 80 if cx else 10, 70))
        for k in range(-3, 4):
            g.box(TIM, (cx * (wg / 2.0 - 6) - cy * k * 215, cy * (wg / 2.0 - 6) + cx * k * 215, z + 80), (16, 16, 120))
    g.box(TIM_D, (0, 0, z + 12), (wg, wg, 30))
    g.box(SNOW, (0, 0, z + 30), (wg - 60, wg - 60, 10))
    # the open belfry: four great posts, braces, a headstock; 11 m of clear air for the bell
    zb = z + 30
    hb = 1160.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(TIM_D, (sx * 600, sy * 600, zb + hb / 2.0), (70, 70, hb))
            tilt_box(g, TIM_D, (sx * 600, sy * 600, zb + hb - 260), (sx * 330, sy * 600, zb + hb - 20), 30, 30)
            tilt_box(g, TIM_D, (sx * 600, sy * 600, zb + hb - 260), (sx * 600, sy * 330, zb + hb - 20), 30, 30)
    for s2 in (-1, 1):
        g.box(TIM_D, (0, s2 * 600, zb + hb - 20), (1270, 60, 60))
        g.box(TIM_D, (s2 * 600, 0, zb + hb - 20), (60, 1270, 60))
    zp = zb + hb - 60                                                            # the pivot: the headstock, lying along X
    g.box(TIM, (0, 0, zp), (1260, 90, 90))
    for sx in (-1, 1):
        g.box(STEEL, (sx * 420, 0, zp), (30, 110, 110))
    bell = sc.part(P.motion(swing=13, pivot=(0, 0, zp), axis=0, rate=0.14, phase=0.0))
    bell.box(TIM_D, (0, 0, zp - 90), (300, 60, 150))                             # the strap
    bell.box(STEEL, (0, 0, zp - 110), (360, 70, 20))
    rb = 1.4142
    bell.cyl(PINK, (0, 0, zp - 930), 350 * rb, 780, rot=(0, 45, 0), steps=4, top_r=215 * rb)     # 7 m at the mouth, 8 m with the strap
    bell.cyl(PINK, (0, 0, zp - 150), 215 * rb, 50, rot=(0, 45, 0), steps=4, top_r=150 * rb)
    bell.cyl(CREAM, (0, 0, zp - 760), 322 * rb, 70, rot=(0, 45, 0), steps=4, top_r=313 * rb)    # the painted band
    bell.cyl(CREAM, (0, 0, zp - 938), 360 * rb, 34, rot=(0, 45, 0), steps=4, top_r=358 * rb)    # the lip
    bell.cyl(PANE, (0, 0, zp - 944), 318 * rb, 12, rot=(0, 45, 0), steps=4)                      # the dark inside
    for sy in (-1, 1):                                                           # a painted lozenge each broad face
        bell.box(CREAM, (0, sy * 276, zp - 470), (150, 24, 150), rot=(45, 0, -9.8 * sy))
    clap = sc.part(P.motion(swing=16, pivot=(0, 0, zp - 200), axis=0, rate=0.14, phase=0.12))
    clap.cyl(STEEL, (0, 0, zp - 960), 16, 760, steps=6)
    clap.sphere(STEEL, (0, 0, zp - 990), 70, steps=(5, 8))
    # the spire: a flared skirt and a steep pink pyramid, snow on the skirt, four pinnacles, a gold star and a pennant
    zs = zb + hb + 10
    rs = 820.0 * rb
    g.box(TIM_D, (0, 0, zs), (1340, 1340, 30))
    g.cyl(PINK, (0, 0, zs + 10), rs, 230, rot=(0, 45, 0), steps=4, top_r=rs * 0.58)
    g.cyl(PINK, (0, 0, zs + 240), rs * 0.58, 820, rot=(0, 45, 0), steps=4, top_r=22)
    g.cyl(SNOW, (0, 0, zs + 80), rs * 0.86, 176, rot=(0, 45, 0), steps=4, top_r=rs * 0.56)
    g.cyl(SNOW, (0, 0, zs + 270), rs * 0.555, 330, rot=(0, 45, 0), steps=4, top_r=rs * 0.33)
    for k in range(3):                                                           # shingle courses showing through
        g.cyl(PINK, (0, 0, zs + 640 + k * 120), rs * (0.30 - k * 0.075), 26, rot=(0, 45, 0), steps=4, top_r=rs * (0.285 - k * 0.075))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(PINK, (sx * 640, sy * 640, zs + 20), 80, 250, steps=6, top_r=6)
            g.sphere(YEL, (sx * 640, sy * 640, zs + 280), 22, steps=(4, 6))
    zt = zs + 1060
    g.cyl(TIM_D, (0, 0, zt - 20), 12, 330, steps=6)
    g.sphere(YEL, (0, 0, zt + 40), 46, steps=(5, 8))
    f = sc.part(P.motion(swing=16, pivot=(0, 0, zt + 250), axis=P.UP, rate=0.4, phase=0.3))
    f.box(PINK, (190, 0, zt + 250), (360, 10, 80))
    f.box(CREAM, (190, 0, zt + 250), (364, 14, 24))
    f.box(PINK, (440, 0, zt + 236), (150, 10, 44))


JOBS = [
    ('SM_S1D3_TallHouse', tall_house, dict(grow=(40, 40, 40), grow_down=(40, 40, 0))),
    ('SM_S1D3_ArcadeHouse', arcade_house, STILL),
    ('SM_S1D3_Bakery', bakery, dict(grow=(20, 20, 60), grow_down=(20, 20, 0))),
    ('SM_S1D3_ToyMaker', toy_maker, dict(grow=(30, 30, 30), grow_down=(30, 30, 0))),
    ('SM_S1D3_TownGate', town_gate, dict(grow=(120, 120, 20), grow_down=(120, 120, 0))),
    ('SM_S1D3_Granary', granary, dict(grow=(10, 30, 0), grow_down=(10, 30, 0))),
    ('SM_S1D3_OrielHouse', oriel_house, STILL),
    ('SM_S1D3_GalleryInn', gallery_inn, dict(grow=(10, 30, 0), grow_down=(10, 30, 0))),
    ('SM_S1D3_FountainTrough', fountain_trough, STILL),
    ('SM_S1D3_SledRack', sled_rack, STILL),
    ('SM_S1D3_RibbonPole', ribbon_pole, dict(grow=(90, 90, 0), grow_down=(90, 90, 0))),
    ('SM_S1D3_MarketStall', market_stall, dict(grow=(0, 20, 0), grow_down=(0, 20, 0))),
    ('SM_S1D3_Woodstack', woodstack, STILL),
    ('SM_S1D3_LanternPost', lantern_post, dict(grow=(20, 0, 0), grow_down=(20, 0, 0))),
    ('SM_S1D3_WashLine', wash_line, dict(grow=(0, 40, 0), grow_down=(0, 40, 0))),
    ('SM_S1D3_FingerPost', finger_post, dict(grow=(50, 50, 0), grow_down=(50, 50, 0))),
    ('SM_S1D3_BenchPlanter', bench_planter, STILL),
    ('SM_S1D3_GreatCowbell', great_cowbell, dict(grow=(200, 400, 60), grow_down=(200, 400, 0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
