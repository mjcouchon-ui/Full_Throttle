"""Ice Mountain, Stage 1, builder s1d5: "The white ridge", the aurora watch (11.6 to 14.3 km).
/Game/World/IceMountain/SM_S1D5_*.   Run: import ft_world_ice_s1d5 as K; K.run()

Wind-shaped: low stone refuges under cabled hog-back roofs, a slit dome, a cabin on one leg, a kite loft, a curved
wind wall, an instrument house; vanes, streamer lines, a telescope, a lamp cairn, a cup anemometer, a wind-break, a
sky board (shapes, no words); and THE WIND HARP, nine leaning masts strung with cables and long streamers.
Violet #b9a2ff leads, with mint #8ff0cf, slate #5a6478, asphalt blue #3d4763 and warm windows #ffe28a.

THE WIND blows DOWN the road. A piece on the left of the road is turned round, so every piece the wind shows on has a
twin: SM_S1D5_<Name> for the RIGHT (the wind goes to the mesh's +X) and SM_S1D5_<Name>L for the LEFT (to its -X).
The twin is the same drawing mirrored in X (class G does it: every point, pivot and base). The hero stands on the
left only and is drawn as an L piece. The hill falls 7 in 100 along the road, downwind: in a drawing's own (unmirrored)
coordinates the land's height is gz(x) = -0.07 x on both sides.

Every mesh is an ft_people.Scene: one section, M_People (the hero: M_PeopleFar, it must stream beyond 180 m).
Glow is steady (windows 0.9, lamps 1.3) and only on pieces the table places 24 m out or more. Nothing blinks.
"""
import math
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/World/IceMountain'
VIO, MINT, SLATE, ROCK, ASPH, LIT = '#b9a2ff', '#8ff0cf', '#5a6478', '#6a7488', '#3d4763', '#ffe28a'
SNOW, STEEL, PANE, WOOD, WOOD_D, CREAM = '#f4f9ff', '#9aa4b8', '#1d2740', '#9a6a44', '#6a4c36', '#f4efe6'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
WIN, LAMP = 0.9, 1.3


def gz(x):
    """The land's height at x along the piece (cm), in the drawing's own coordinates: it falls downwind."""
    return -0.07 * x


def _rod(m, col, a, b, r, top_r=None, steps=6, glow=0.0):
    """A cylinder (or a taper) from a to b."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=top_r)


class G(object):
    """A drawing hand that mirrors in X when w = -1 (the left-hand twin). Only shapes that mirror cleanly."""

    def __init__(self, sc, w=1.0, mo=None):
        self.sc, self.w = sc, w
        self.m = sc.part(mo) if mo else sc.still()

    def X(self, p):
        return (p[0] * self.w, p[1], p[2])

    def box(self, col, c, size, glow=0.0, yaw=0.0):
        self.m.box(col, self.X(c), size, rot=(0, yaw * self.w, 0), glow=glow)

    def dia(self, col, c, size, glow=0.0):
        """A square stood on its corner, facing the road."""
        self.m.box(col, self.X(c), size, rot=(45, 0, 0), glow=glow)

    def cyl(self, col, base, r, h, steps=8, top_r=None, glow=0.0):
        self.m.cyl(col, self.X(base), r, h, steps=steps, top_r=top_r, glow=glow)

    def sph(self, col, c, r, scale=(1, 1, 1), steps=(4, 6), glow=0.0):
        self.m.sphere(col, self.X(c), r, scale=scale, steps=steps, glow=glow)

    def tb(self, col, a, b, wd, d=None, glow=0.0):
        tilt_box(self.m, col, self.X(a), self.X(b), wd, d, glow)

    def rod(self, col, a, b, r, top_r=None, steps=6, glow=0.0):
        _rod(self.m, col, self.X(a), self.X(b), r, top_r, steps, glow)

    def mov(self, **kw):
        if 'pivot' in kw:
            p = kw['pivot']
            kw['pivot'] = (p[0] * self.w, p[1], p[2])
        if 'base' in kw:
            b = kw['base']
            kw['base'] = (b[0] * self.w, b[1])
        if self.w < 0 and 'axis' in kw and kw['axis'] < 900.0:
            kw['axis'] = 180.0 - kw['axis']
        return G(self.sc, self.w, P.motion(**kw))


def _arch(ys, z0, rise):
    half = (ys[-1] - ys[0]) / 2.0
    mid = (ys[-1] + ys[0]) / 2.0
    return [z0 + rise * math.cos(math.pi * (y - mid) / (2.0 * half)) for y in ys]


def _pennant(g, col, col2, at, length=130.0, deep=44.0, swing=10.0, rate=0.9, phase=0.0):
    """A cloth on a pole's top, flying downwind and swinging round the pole."""
    mv = g.mov(swing=swing, pivot=at, axis=P.UP, rate=rate, phase=phase)
    mv.tb(col, (at[0] + 2, at[1], at[2]), (at[0] + length, at[1], at[2] - length * 0.12), 6, deep)
    mv.tb(col2, (at[0] + length - 6, at[1], at[2] - length * 0.12), (at[0] + length * 1.45, at[1], at[2] - length * 0.2), 6, deep * 0.55)


# ------------------------------------------------------------------------------------------ the six buildings
def refuge(sc, w):
    """THE HOG-BACK REFUGE: 8.8 x 5.8 m of slate under a violet arched roof that three cables with stone weights hold
    down; a battered prow to windward, snow piled on the lee end, a bent chimney, shutters, firewood, a bench."""
    g = G(sc, w)
    g.box(SLATE, (0, 0, 5), (860, 560, 510))
    g.box(ROCK, (0, 0, 36), (876, 576, 40))
    g.box(ROCK, (0, 0, 238), (872, 572, 30))
    g.tb(ROCK, (-520, 0, -230), (-440, 0, 250), 420, 110)                       # the prow
    g.tb(SLATE, (-486, 0, -230), (-446, 0, 150), 250, 60)
    for sx in (-1, 1):                                                          # the gable ends, stepped stone
        g.box(SLATE, (sx * 415, 0, 300), (30, 540, 84))
        g.box(SLATE, (sx * 415, 0, 375), (30, 400, 70))
        g.box(SLATE, (sx * 415, 0, 428), (30, 200, 40))
    ys = (-330, -220, -110, 0, 110, 220, 330)
    zs = _arch(ys, 262, 196)
    for i in range(6):
        g.tb(VIO, (0, ys[i], zs[i]), (0, ys[i + 1], zs[i + 1]), 940, 26)
    for i in range(1, 5):                                                       # snow on the lee end
        g.tb(SNOW, (300, ys[i], zs[i] + 20), (300, ys[i + 1], zs[i + 1] + 20), 330, 20)
    g.box(MINT, (0, 0, zs[3] + 24), (1000, 44, 26))                             # the ridge board, swept up downwind
    g.tb(MINT, (496, 0, zs[3] + 24), (590, 0, zs[3] + 118), 44, 26)
    for x in (-350, -130, 90):                                                  # the cables and their weights
        for i in range(6):
            g.tb(ASPH, (x, ys[i], zs[i] + 20), (x, ys[i + 1], zs[i + 1] + 20), 10, 10)
        for sy in (-1, 1):
            g.tb(ASPH, (x, sy * 334, 272), (x, sy * 346, 130), 10, 10)
            g.box(ROCK, (x, sy * 348, 100), (50, 44, 64))
    for x in (-250, -40):                                                       # the front
        g.box(LIT, (x, -283, 150), (110, 10, 90), glow=WIN)
        g.box(MINT, (x, -282, 204), (138, 10, 14))
        g.box(MINT, (x, -284, 98), (138, 14, 10))
        g.box(VIO, (x - 72, -284, 150), (26, 8, 98))
        g.box(VIO, (x + 72, -284, 150), (26, 8, 98))
    g.box(ASPH, (220, -283, 100), (110, 10, 200))
    g.box(VIO, (220, -286, 150), (70, 6, 16))
    g.box(MINT, (220, -282, 208), (138, 10, 16))
    g.box(ROCK, (220, -320, -40), (150, 70, 110))                               # the door stone
    g.box(ASPH, (306, -292, 214), (10, 26, 8))
    g.box(LIT, (306, -302, 192), (20, 20, 28), glow=LAMP)                       # the door lantern
    g.box(WOOD_D, (-385, -306, 45), (130, 44, 90))                              # firewood
    g.box(WOOD, (-385, -330, 45), (118, 6, 78))
    g.box(SNOW, (-385, -306, 96), (140, 54, 12))
    g.box(WOOD, (40, -312, 46), (170, 36, 10))                                  # the bench
    g.box(WOOD_D, (-30, -312, 20), (10, 30, 44))
    g.box(WOOD_D, (110, -312, 20), (10, 30, 44))
    g.tb(VIO, (322, -300, 0), (336, -288, 178), 12, 6)                          # skis by the door
    g.tb(MINT, (344, -300, 0), (354, -288, 170), 12, 6)
    g.box(LIT, (432, 0, 150), (10, 120, 90), glow=WIN)                          # the lee end's window
    g.box(MINT, (434, 0, 204), (10, 148, 14))
    g.tb(SLATE, (250, 120, 380), (276, 120, 610), 74, 74)                       # the chimney leans, its cowl turned away
    g.tb(MINT, (270, 120, 606), (336, 120, 650), 60, 40)
    g.sph(SNOW, (440, 40, 0), 150, scale=(1.1, 1.6, 0.5), steps=(4, 8))         # the drift in the lee
    g.rod(ASPH, (-400, -250, 250), (-378, -250, 570), 7)
    _pennant(g, VIO, MINT, (-378, -250, 552), phase=0.2)


def slit_dome(sc, w):
    """THE SLIT DOME: a violet dome on a slate drum, a slit of dark glass with a mint glow up its front and a telescope
    looking out of it at the sky, a cable over its back, buttresses, a porch, a vane, snow on its lee shoulder."""
    g = G(sc, w)
    g.cyl(SLATE, (0, 0, -250), 330, 530, steps=12)
    g.cyl(ROCK, (0, 0, 20), 342, 36, steps=12)
    g.cyl(MINT, (0, 0, 268), 344, 26, steps=12)
    g.sph(VIO, (0, 0, 290), 322, scale=(1, 1, 0.98), steps=(6, 12))
    R = 324.0
    th = [math.radians(a) for a in (10, 30, 50, 70, 88)]
    for i in range(4):                                                          # the slit, up the front
        a = (0, -R * math.cos(th[i]), 290 + R * 0.98 * math.sin(th[i]))
        b = (0, -R * math.cos(th[i + 1]), 290 + R * 0.98 * math.sin(th[i + 1]))
        g.tb(ASPH, a, b, 126, 14)
        g.tb(PANE, (0, a[1] - 5, a[2] + 5), (0, b[1] - 5, b[2] + 5), 86, 14)
        g.tb(MINT, (0, a[1] - 9, a[2] + 9), (0, b[1] - 9, b[2] + 9), 28, 14, glow=WIN)
    g.rod(STEEL, (0, -80, 560), (0, -160, 700), 30, steps=8)                    # the telescope looks up, not at the road
    g.rod(ASPH, (0, -160, 700), (0, -176, 728), 37, steps=8)
    ph = [math.radians(a) for a in (22, 48, 74, 100, 126, 152)]                 # a cable over the back, weights on the drum
    Rb = 300.0
    for i in range(5):
        g.tb(ASPH, (Rb * math.cos(ph[i]), 130, 296 + Rb * 0.98 * math.sin(ph[i])), (Rb * math.cos(ph[i + 1]), 130, 296 + Rb * 0.98 * math.sin(ph[i + 1])), 10, 10)
    for sx in (-1, 1):
        g.tb(ASPH, (sx * 280, 130, 400), (sx * 322, 130, 170), 10, 10)
        g.box(ROCK, (sx * 326, 130, 140), (46, 52, 66))
    for a in (20, 70, 110, 160, 215, 325):                                      # buttresses
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        g.tb(ROCK, (410 * c, 410 * s, -230), (318 * c, 318 * s, 210), 90, 74)
    g.box(SLATE, (-190, -288, 95), (150, 70, 250))                              # the porch
    g.box(ASPH, (-190, -324, 98), (92, 8, 190))
    g.box(MINT, (-190, -324, 200), (116, 10, 14))
    g.tb(VIO, (-190, -338, 226), (-190, -250, 252), 180, 16)
    g.box(ROCK, (-190, -360, -50), (130, 60, 120))
    g.box(MINT, (200, -262, 170), (88, 16, 88))                                 # a porthole
    g.box(LIT, (200, -268, 170), (62, 20, 62), glow=WIN)
    g.box(LIT, (-140, -330, 232), (18, 18, 24), glow=LAMP)
    g.sph(SNOW, (175, 40, 536), 190, scale=(1.0, 1.2, 0.45), steps=(4, 8))
    g.rod(ASPH, (0, 150, 560), (16, 150, 770), 6)
    mv = g.mov(swing=16, pivot=(16, 150, 756), axis=P.UP, rate=0.5, phase=0.4)
    mv.box(MINT, (62, 150, 756), (110, 6, 28))
    mv.box(VIO, (-24, 150, 756), (44, 6, 54))


def leg_cabin(sc, w):
    """THE ONE-LEG CABIN: a violet watch cabin on a single leaning leg, stayed by three cables, a ladder up the
    windward side, a band of warm window, a balcony rail, a small cabled hog-back roof."""
    g = G(sc, w)
    g.cyl(SLATE, (0, 0, -250), 130, 330, steps=8, top_r=84)
    g.rod(ASPH, (0, 0, 60), (80, 0, 566), 30, steps=8)
    g.rod(MINT, (36, 0, 290), (42, 0, 326), 37, steps=8)
    cx = 96.0
    g.box(ASPH, (cx, 0, 566), (460, 420, 22))
    for sx, sy in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
        g.tb(ASPH, (56, 0, 400), (cx + sx * 190, sy * 170, 556), 12, 12)
    g.box(VIO, (cx, 0, 710), (400, 360, 266))
    g.box(MINT, (cx, 0, 590), (412, 372, 22))
    g.box(MINT, (cx, 0, 838), (412, 372, 18))
    g.box(LIT, (cx, -181, 720), (300, 8, 84), glow=WIN)
    g.box(ASPH, (cx - 50, -184, 720), (10, 8, 90))
    g.box(ASPH, (cx + 50, -184, 720), (10, 8, 90))
    g.box(LIT, (cx + 201, 0, 720), (8, 200, 84), glow=WIN)
    g.box(PANE, (cx - 201, 0, 720), (8, 120, 60))
    ys = (-205, -100, 0, 100, 205)
    zs = _arch(ys, 848, 72)
    for i in range(4):
        g.tb(ASPH, (cx, ys[i], zs[i]), (cx, ys[i + 1], zs[i + 1]), 440, 20)
    for i in (1, 2):
        g.tb(SNOW, (cx + 115, ys[i], zs[i] + 17), (cx + 115, ys[i + 1], zs[i + 1] + 17), 200, 16)
    for x in (cx - 160, cx - 40):
        for i in range(4):
            g.tb(MINT, (x, ys[i], zs[i] + 15), (x, ys[i + 1], zs[i + 1] + 15), 9, 9)
        for sy in (-1, 1):
            g.tb(MINT, (x, sy * 208, 850), (x, sy * 214, 584), 9, 9)
    for sy in (-24, 24):                                                        # the ladder
        g.tb(WOOD_D, (-190, sy, -30), (-122, sy, 566), 8, 8)
    for k in range(8):
        t = 0.08 + k * 0.115
        g.box(WOOD, (-190 + 68 * t, 0, -30 + 596 * t), (8, 56, 8))
    for a, b in (((cx - 222, -202, 558), (-430, -330, gz(-430) - 10)), ((cx - 222, 202, 558), (-430, 330, gz(-430) - 10)), ((cx + 222, 0, 558), (560, 0, gz(560) - 10))):
        g.tb(ASPH, a, b, 7, 7)                                                  # the stays and their stones
        g.box(ROCK, (b[0], b[1], b[2] - 60), (70, 70, 180))
    g.box(ASPH, (cx, -244, 566), (300, 70, 16))                                 # the balcony
    g.box(MINT, (cx, -274, 646), (300, 8, 10))
    for x in (-145, 0, 145):
        g.box(ASPH, (cx + x, -274, 606), (8, 8, 80))
    g.box(LIT, (cx - 60, -150, 536), (22, 22, 30), glow=LAMP)                   # the lantern under the floor
    g.rod(STEEL, (cx + 120, 120, 880), (cx + 150, 120, 968), 12)
    g.rod(ASPH, (cx + 150, 0, 900), (cx + 178, 0, 1062), 6)
    _pennant(g, VIO, MINT, (cx + 176, 0, 1046), length=140, phase=0.6, rate=0.8)


def kite_loft(sc, w):
    """THE KITE LOFT: a tall thin violet shed on a stone foot, its roof one pitch that the wind slides over, a hoist
    beam over the loft door, racks of rolled kites up the front, one great mint kite on the wall, tails on a line."""
    g = G(sc, w)
    g.box(SLATE, (0, 0, -5), (420, 500, 490))
    g.box(ROCK, (0, 0, 232), (436, 516, 26))
    g.box(VIO, (0, 0, 620), (390, 470, 760))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(MINT, (sx * 196, sy * 236, 620), (18, 18, 760))
    g.box(MINT, (0, -238, 580), (390, 10, 14))
    g.box(MINT, (0, -238, 830), (390, 10, 14))
    g.tb(ASPH, (-250, 0, 1010), (262, 0, 1112), 560, 24)                        # the roof: low to windward
    g.tb(SNOW, (-30, 0, 1072), (256, 0, 1130), 540, 16)
    g.box(VIO, (70, 0, 1022), (250, 470, 44))
    g.box(PANE, (0, -236, 910), (120, 8, 140))                                  # the loft door and its hoist
    g.box(MINT, (0, -238, 986), (146, 10, 14))
    g.tb(WOOD_D, (0, -230, 1004), (0, -404, 1004), 14, 18)
    g.tb(ASPH, (0, -392, 998), (0, -392, 900), 6, 6)
    g.box(STEEL, (0, -392, 888), (16, 16, 24))
    for sx in (-1, 1):                                                          # the racks of rolled kites
        g.box(WOOD_D, (sx * 182, -264, 400), (12, 36, 310))
    for k, z in enumerate((290, 345, 400, 455, 510)):
        col = (VIO, MINT, CREAM, MINT, VIO)[k]
        ln = (165, 150, 168, 140, 160)[k]
        g.rod(col, (-ln, -268, z), (ln, -268, z), 21)
        g.box(ASPH, (-ln * 0.5, -268, z), (10, 46, 46))
        g.box(ASPH, (ln * 0.5, -268, z), (10, 46, 46))
    g.dia(MINT, (0, -240, 700), (150, 8, 150))                                  # the great kite on the wall
    g.box(VIO, (0, -246, 700), (8, 6, 208))
    g.box(VIO, (0, -246, 700), (208, 6, 8))
    for k in range(4):
        g.dia((VIO, MINT)[k % 2], (26 + k * 30, -244, 584 - k * 8 - (k % 2) * 10), (24, 6, 24))
    for sx in (-1, 1):
        g.box(LIT, (sx * 138, -237, 760), (40, 8, 130), glow=WIN)
        g.box(MINT, (sx * 138, -238, 830), (58, 10, 12))
    g.box(ASPH, (-120, -252, 100), (100, 8, 196))                               # the door
    g.box(MINT, (-120, -252, 204), (124, 10, 14))
    g.box(ROCK, (-120, -290, -50), (140, 60, 120))
    g.box(LIT, (-40, -262, 196), (18, 18, 26), glow=LAMP)
    g.rod(ASPH, (256, 0, 1116), (276, 0, 1256), 6)
    mv = g.mov(swing=8, pivot=(274, 0, 1240), axis=P.UP, rate=0.6, phase=0.1)   # the long streamer off the roof's high end
    mv.tb(VIO, (276, 0, 1240), (460, 0, 1218), 6, 40)
    mv.tb(MINT, (450, 0, 1219), (560, 0, 1204), 6, 24)
    g.rod(WOOD_D, (330, -330, -60), (340, -330, 300), 9)                        # kite tails drying on a line
    g.tb(ASPH, (196, -240, 500), (339, -330, 296), 6, 6)
    for k in range(4):
        t = 0.2 + k * 0.2
        p = (196 + 143 * t, -240 - 90 * t, 500 - 204 * t)
        mk = g.mov(swing=14, pivot=p, axis=58.0, rate=1.0 + 0.1 * k, phase=k * 0.23)
        mk.box((MINT, VIO)[k % 2], (p[0], p[1], p[2] - 24), (30, 6, 42), yaw=-32)


def wind_wall(sc, w):
    """THE WIND WALL: a slate screen curved round to windward, mint-capped, slits of warm light in it, pennants along
    its top; in its lee a bench hall under a violet lean-to with a lantern."""
    g = G(sc, w)
    cx0, cy0, R = 150.0, 480.0, 680.0
    pts = []
    for a in (200, 220, 240, 260, 280, 300):
        r = math.radians(a)
        x = cx0 + R * math.cos(r)
        pts.append((x, cy0 + R * math.sin(r), gz(x) * 0.5))
    for i in range(5):
        a, b = pts[i], pts[i + 1]
        g.tb(SLATE, (a[0], a[1], 40), (b[0], b[1], 40), 70, 560)
        g.tb(ROCK, (a[0], a[1], -90), (b[0], b[1], -90), 98, 300)
        g.tb(MINT, (a[0], a[1], 328), (b[0], b[1], 328), 80, 16)
        if i >= 3:
            g.tb(SNOW, (a[0], a[1], 343), (b[0], b[1], 343), 70, 14)
        if 1 <= i <= 3:                                                         # a slit of light through the wall
            mx, my = (a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0
            L = math.hypot(b[0] - a[0], b[1] - a[1])
            ux, uy = (b[0] - a[0]) / L, (b[1] - a[1]) / L
            nx, ny = (mx - cx0) / R, (my - cy0) / R
            g.tb(ASPH, (mx - ux * 46 + nx * 33, my - uy * 46 + ny * 33, 214), (mx + ux * 46 + nx * 33, my + uy * 46 + ny * 33, 214), 8, 46)
            g.tb(LIT, (mx - ux * 36 + nx * 36, my - uy * 36 + ny * 36, 214), (mx + ux * 36 + nx * 36, my + uy * 36 + ny * 36, 214), 8, 28, glow=WIN)
    g.tb(ROCK, (pts[0][0] - 40, pts[0][1] + 30, -230), (pts[0][0] + 20, pts[0][1] - 6, 250), 110, 110)   # the windward end stone
    for i in (1, 2, 3, 4):                                                      # pennants along the top
        p = pts[i]
        g.rod(ASPH, (p[0], p[1], 330), (p[0] + 10, p[1], 450), 6)
        _pennant(g, (VIO, MINT)[i % 2], (MINT, VIO)[i % 2], (p[0] + 10, p[1], 436), length=96, deep=36, phase=i * 0.19, rate=0.8 + 0.06 * i)
    g.tb(VIO, (100, -182, 322), (100, 196, 236), 520, 20)                       # the hall in the lee
    g.tb(SNOW, (230, -150, 332), (230, 190, 254), 250, 16)
    g.box(MINT, (100, 200, 228), (530, 12, 14))
    for x in (-150, 100, 350):
        g.box(WOOD_D, (x, 190, 70), (14, 14, 330))
    g.box(WOOD, (100, 60, 46), (420, 40, 10))
    g.box(WOOD, (100, 82, 84), (420, 8, 46))
    for x in (-90, 100, 290):
        g.box(WOOD_D, (x, 62, 4), (10, 34, 80))
    g.box(VIO, (0, 60, 54), (92, 46, 8))
    g.box(MINT, (190, 60, 56), (30, 30, 12))
    g.tb(ASPH, (100, 20, 300), (100, 20, 252), 6, 6)
    g.box(LIT, (100, 20, 238), (22, 22, 30), glow=LAMP)
    g.tb(VIO, (300, 110, 0), (310, 128, 176), 12, 6)                            # skis and poles in the rack
    g.tb(MINT, (322, 110, 0), (330, 128, 168), 12, 6)
    g.tb(ASPH, (344, 112, 0), (348, 126, 130), 6, 6)
    g.box(ROCK, (100, 60, -94), (560, 300, 200))                                # the hall's floor of stone


def instrument_house(sc, w):
    """THE INSTRUMENT HOUSE: a louvred cube on four splayed legs, a double violet roof, a ladder, a rain gauge, a
    tilted sky panel on its lee side, a vane on the roof, a lamp under the floor."""
    g = G(sc, w)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.rod(ASPH, (sx * 192, sy * 192, -200), (sx * 122, sy * 122, 344), 13)
            g.box(ROCK, (sx * 192, sy * 192, -60), (92, 92, 220))
    for sy in (-1, 1):
        g.tb(ASPH, (-176, sy * 176, 30), (132, sy * 132, 300), 9, 9)
        g.tb(ASPH, (176, sy * 176, 30), (-132, sy * 132, 300), 9, 9)
    for sx in (-1, 1):
        g.tb(ASPH, (sx * 176, -176, 30), (sx * 132, 132, 300), 9, 9)
    g.box(VIO, (0, 0, 340), (344, 344, 16))
    g.box(ASPH, (0, 0, 492), (300, 300, 296))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(VIO, (sx * 150, sy * 150, 492), (24, 24, 296))
    for k in range(5):
        z = 396 + k * 46
        g.box(MINT, (0, -154, z), (262, 12, 22))
        g.box(MINT, (-154, 0, z), (12, 262, 22))
        g.box(MINT, (154, 0, z), (12, 262, 22))
    g.tb(VIO, (-212, 0, 652), (212, 0, 692), 404, 16)                           # the double roof, rising to the lee
    g.tb(VIO, (-236, 0, 694), (236, 0, 744), 446, 18)
    g.tb(SNOW, (0, 0, 734), (232, 0, 759), 436, 14)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(MINT, (sx * 150, sy * 150, 676 + sx * 14), (14, 14, 44))
    for sx in (-40, 40):                                                        # the ladder to the door
        g.tb(WOOD_D, (sx, -340, -30), (sx, -166, 340), 8, 8)
    for k in range(6):
        t = 0.1 + k * 0.15
        g.box(WOOD, (0, -340 + 174 * t, -30 + 370 * t), (88, 8, 8))
    g.box(PANE, (0, -152, 470), (90, 10, 150))
    g.cyl(STEEL, (-200, -262, -100), 26, 262, steps=8)                          # the rain gauge
    g.cyl(MINT, (-200, -262, 162), 20, 30, steps=8, top_r=40)
    g.tb(ASPH, (150, 0, 560), (232, 0, 520), 12, 12)                            # the sky panel on its bracket
    g.tb(ASPH, (190, 0, 452), (290, 0, 606), 250, 10)
    g.tb(MINT, (184, -66, 458), (284, -66, 612), 76, 6)
    g.tb(MINT, (184, 66, 458), (284, 66, 612), 76, 6)
    g.box(LIT, (0, -100, 322), (26, 26, 26), glow=LAMP)
    g.rod(ASPH, (0, 0, 730), (10, 0, 906), 6)
    mv = g.mov(swing=18, pivot=(10, 0, 892), axis=P.UP, rate=0.45, phase=0.7)
    mv.box(MINT, (66, 0, 892), (120, 6, 24))
    mv.box(VIO, (-34, 0, 892), (52, 6, 62))


# ------------------------------------------------------------------------------------------ the seven props
def vane_row(sc, w):
    """A ROW OF SMALL WIND VANES on a stone kerb, five of them stepped in height, each turning a little on its own
    clock. LOW: its top is 1.9 m."""
    g = G(sc, w)
    g.box(SLATE, (0, 0, -5), (640, 70, 90))
    g.box(SNOW, (0, 20, 44), (640, 28, 10))
    for k in range(5):
        x = -260 + k * 130
        h = (115, 150, 176, 135, 160)[k]
        g.box(ROCK, (x, -6, 46), (40, 40, 16))
        g.box(ASPH, (x, -6, 54 + (h - 54) / 2.0), (8, 8, h - 54))
        mv = g.mov(swing=14 + 2 * k, pivot=(x, -6, h), axis=P.UP, rate=0.35 + 0.07 * k, phase=k * 0.21)
        mv.box(MINT, (x - 28, -6, h), (38, 6, 12))
        if k % 2:
            mv.dia((VIO, MINT)[(k // 2) % 2], (x + 34, -6, h), (30, 6, 30))
        else:
            mv.box(VIO, (x + 32, -6, h), (46, 6, 28))


def streamer_line(sc, w):
    """A STREAMER LINE: two leaning posts on cairns 8 m apart, a sagging cable, eight long violet and mint streamers
    flying downwind off it, each on its own clock."""
    g = G(sc, w)
    tops = ((-370.0, 0.0, 560.0), (430.0, 0.0, 536.0))
    for k, x in enumerate((-400.0, 400.0)):
        z = gz(x)
        g.cyl(SLATE, (x, 0, z - 200), 64, 250, steps=6, top_r=38)
        g.rod(WOOD_D, (x, 0, z + 30), tops[k], 10)
        g.box(MINT, (tops[k][0], 0, tops[k][2] + 6), (18, 18, 16))
        ax = x - 150
        g.tb(ASPH, (tops[k][0], 0, tops[k][2] - 8), (ax, 0, gz(ax) - 10), 7, 7)
        g.box(ROCK, (ax, 0, gz(ax) - 50), (60, 60, 160))

    def at(t):
        return (tops[0][0] + (tops[1][0] - tops[0][0]) * t, 0.0, tops[0][2] + (tops[1][2] - tops[0][2]) * t - 240.0 * t * (1.0 - t))
    for i in range(6):
        g.tb(ASPH, at(i / 6.0), at((i + 1) / 6.0), 7, 7)
    for k in range(8):
        p = at((k + 0.5) / 8.0)
        col, col2 = ((VIO, MINT), (MINT, VIO))[k % 2]
        mv = g.mov(swing=12, pivot=p, axis=90.0, rate=0.8 + 0.05 * k, phase=0.13 * k)
        mv.tb(col, (p[0] + 2, 0, p[2] - 6), (p[0] + 124, 0, p[2] - 46), 6, 36)
        mv.tb(col2, (p[0] + 112, 0, p[2] - 42), (p[0] + 178, 0, p[2] - 66), 6, 18)


def telescope(sc):
    """A TELESCOPE ON A PIER: a round stone platform with a rail, a slate pier, a stubby violet tube in a fork looking
    up and AWAY from the road, a finder, a star plate of dots (no words), a stool."""
    g = G(sc)
    g.cyl(ROCK, (0, 0, -200), 200, 230, steps=10)
    g.cyl(SLATE, (0, 0, 30), 150, 26, steps=10)
    g.cyl(SLATE, (0, 0, 56), 62, 150, steps=8, top_r=46)
    g.cyl(MINT, (0, 0, 200), 52, 14, steps=8)
    for sx in (-1, 1):
        g.box(ASPH, (sx * 42, 10, 262), (12, 34, 104))
    g.box(ASPH, (0, 10, 216), (96, 34, 14))
    g.rod(VIO, (0, -62, 232), (0, 84, 388), 30, steps=8)
    g.rod(ASPH, (0, 84, 388), (0, 104, 410), 37, steps=8)
    g.rod(MINT, (0, 104, 410), (0, 108, 414), 33, steps=8)
    g.rod(ASPH, (0, -62, 232), (0, -80, 212), 14)
    g.rod(MINT, (40, -24, 296), (40, 44, 370), 8)
    g.box(STEEL, (0, -92, 196), (30, 30, 30))
    g.box(ASPH, (0, -58, 130), (72, 8, 72))                                     # the star plate
    for x, z in ((-20, 148), (14, 152), (22, 118), (-12, 112)):
        g.dia(LIT, (x, -63, z), (9, 6, 9), glow=0.6)
    g.tb(MINT, (-28, -63, 132), (26, -63, 136), 6, 6)
    g.box(WOOD, (-124, -64, 76), (44, 44, 8))
    for dx, dy in ((-16, -16), (16, -16), (0, 16)):
        g.box(WOOD_D, (-124 + dx, -64 + dy, 54), (8, 8, 48))
    g.box(ROCK, (0, -216, -85), (120, 60, 200))
    g.box(ROCK, (0, -268, -100), (120, 50, 180))
    pr = []
    for a in (15, 52, 90, 128, 165):                                            # the rail round the back
        r = math.radians(a)
        pr.append((186 * math.cos(r), 186 * math.sin(r)))
        g.box(ASPH, (pr[-1][0], pr[-1][1], 76), (8, 8, 96))
    for i in range(4):
        g.tb(MINT, (pr[i][0], pr[i][1], 122), (pr[i + 1][0], pr[i + 1][1], 122), 8, 8)
    g.box(VIO, (150, -70, 44), (60, 44, 28))                                    # the folded cover


def lamp_cairn(sc):
    """A CAIRN WITH A MINT LAMP: stacked stones wound with a violet scarf, a small steady lamp in a cage on top, a
    string of little flags to a peg. LOW: its top is under 2 m."""
    g = G(sc)
    g.box(SLATE, (0, 0, 2), (150, 130, 96), yaw=10)
    g.box(ROCK, (4, -4, 75), (120, 104, 50), yaw=-15)
    g.box(SLATE, (-4, 4, 118), (92, 84, 36), yaw=25)
    g.box(ROCK, (2, 0, 148), (64, 60, 26), yaw=-8)
    g.box(VIO, (4, -4, 96), (126, 110, 10), yaw=-15)
    g.box(SNOW, (30, 22, 103), (50, 36, 8), yaw=-15)
    g.box(SNOW, (-22, 10, 139), (36, 30, 8), yaw=25)
    g.box(ASPH, (0, 0, 164), (26, 26, 6))
    g.box(MINT, (0, 0, 177), (18, 18, 20), glow=LAMP)
    g.box(ASPH, (0, 0, 190), (28, 28, 6))
    for sx, sy in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
        g.box(ASPH, (sx * 11, sy * 11, 177), (5, 5, 20))
    g.tb(ASPH, (20, 0, 150), (150, 0, 24), 5, 5)
    g.box(ROCK, (154, 0, -6), (30, 30, 56))
    for k in range(3):
        t = 0.25 + k * 0.25
        g.box((VIO, MINT, VIO)[k], (20 + 130 * t, 0, 150 - 126 * t - 12), (22, 5, 18))


def anemometer(sc, w):
    """A CUP ANEMOMETER MAST: 7 m of leaning steel on a stone foot, three stays, rungs, an instrument box, a vane, and
    at the top three mint cups going round slowly (0.55 turns a second)."""
    g = G(sc, w)
    g.cyl(SLATE, (0, 0, -200), 72, 250, steps=6, top_r=50)
    a, b = (0.0, 0.0, 40.0), (36.0, 0.0, 684.0)

    def at(t):
        return (a[0] + (b[0] - a[0]) * t, 0.0, a[2] + (b[2] - a[2]) * t)
    g.rod(STEEL, a, b, 9)
    g.rod(VIO, at(0.30), at(0.345), 13)
    g.rod(MINT, at(0.60), at(0.645), 13)
    g.box(ASPH, (10, -18, 160), (46, 30, 58))
    for k in range(3):
        g.box(MINT, (10, -34, 144 + k * 16), (38, 6, 8))
    for k in range(6):
        p = at(0.3 + k * 0.08)
        g.box(ASPH, (p[0], 0, p[2]), (8, 46, 6))
    for q in ((-300, -170), (-300, 170), (330, 0)):
        g.tb(ASPH, at(0.75), (q[0], q[1], gz(q[0]) - 10), 6, 6)
        g.box(ROCK, (q[0], q[1], gz(q[0]) - 50), (56, 56, 160))
    g.box(ASPH, (b[0], 0, 690), (16, 16, 20))
    rot = g.mov(base=(b[0], 0.0), travel=(-1.0, 0.55))                          # the cups
    for k in range(3):
        c, s = math.cos(math.radians(k * 120 + 15)), math.sin(math.radians(k * 120 + 15))
        rot.tb(STEEL, (b[0], 0, 696), (b[0] + 70 * c, 70 * s, 696), 6, 6)
        rot.sph((MINT, MINT, VIO)[k], (b[0] + 80 * c, 80 * s, 696), 21)
    p = at(0.9)
    g.tb(ASPH, p, (p[0] - 40, 0, p[2] + 4), 6, 6)
    mv = g.mov(swing=13, pivot=(p[0] - 40, 0, p[2] + 14), axis=P.UP, rate=0.4, phase=0.3)
    mv.box(MINT, (p[0] - 12, 0, p[2] + 14), (56, 6, 10))
    mv.box(VIO, (p[0] - 74, 0, p[2] + 14), (40, 6, 34))


def wind_break(sc):
    """A STONE WIND-BREAK CRESCENT with a bench in its lee: a blanket, a flask, skis and poles against the wall, a
    ring of stones with an ember. LOW: its top is under 2 m."""
    g = G(sc)
    pts = []
    for a in (10, 38, 66, 90, 114, 142, 170):
        r = math.radians(a)
        pts.append((240 * math.cos(r), -40 + 240 * math.sin(r)))
    for i in range(6):
        a, b = pts[i], pts[i + 1]
        low = i in (0, 5)
        top = 112 if low else 150
        g.tb(SLATE, (a[0], a[1], (top - 55) / 2.0), (b[0], b[1], (top - 55) / 2.0), 58, top + 55)
        g.tb(ROCK, (a[0], a[1], top - 8), (b[0], b[1], top - 8), 66, 18)
        if not low:
            g.tb(SNOW, (a[0], a[1], top + 6), (b[0], b[1], top + 6), 56, 12)
    g.box(WOOD, (0, 96, 44), (210, 38, 10))
    g.box(WOOD_D, (-80, 96, 18), (10, 32, 44))
    g.box(WOOD_D, (80, 96, 18), (10, 32, 44))
    g.box(VIO, (-50, 96, 52), (72, 42, 8))
    g.cyl(MINT, (58, 96, 49), 8, 24, steps=6)
    g.tb(VIO, (-128, 140, 0), (-136, 168, 184), 12, 6)
    g.tb(MINT, (-106, 148, 0), (-112, 174, 176), 12, 6)
    g.tb(ASPH, (-84, 154, 0), (-88, 176, 132), 6, 6)
    for a in range(0, 360, 60):                                                 # the ember ring
        r = math.radians(a)
        g.box(ROCK, (34 * math.cos(r), -20 + 34 * math.sin(r), 8), (22, 22, 20), yaw=a)
    g.box(LIT, (0, -20, 10), (28, 28, 10), glow=0.6)
    g.box(ASPH, (0, -20, 2), (60, 60, 8))


def sky_board(sc):
    """A SKY BOARD: a sign with no words. Under a little hog-back roof, on asphalt blue, the aurora as bands of violet
    and mint over a slate ridge, and five stars."""
    g = G(sc)
    for sx in (-1, 1):
        g.box(WOOD_D, (sx * 124, 0, 60), (16, 16, 520))
        g.box(ROCK, (sx * 124, 0, -80), (72, 72, 240))
    g.box(ASPH, (0, -4, 210), (280, 10, 172))
    g.box(MINT, (0, -8, 300), (292, 10, 8))
    g.box(MINT, (0, -8, 120), (292, 10, 8))
    g.tb(VIO, (0, -62, 318), (0, 0, 346), 330, 14)
    g.tb(VIO, (0, 0, 346), (0, 62, 318), 330, 14)
    g.tb(SNOW, (0, 2, 357), (0, 60, 330), 330, 10)
    g.box(MINT, (0, 0, 356), (350, 16, 12))
    for j, (col, r0) in enumerate(((VIO, 118), (MINT, 96), (VIO, 76))):         # the aurora
        pa = []
        for a in (158, 128, 98, 68, 38):
            r = math.radians(a)
            pa.append((r0 * 1.05 * math.cos(r) - 10 + j * 8, 150 + r0 * 0.9 * math.sin(r)))
        for i in range(4):
            g.tb(col, (pa[i][0], -12, pa[i][1]), (pa[i + 1][0], -12, pa[i + 1][1]), 6, 14)
    for x0, x1, zt in ((-130, -50, 176), (-70, 20, 190), (0, 130, 168)):        # the ridge
        xm = (x0 + x1) / 2.0
        g.tb(ROCK, (x0, -12, 132), (xm, -12, zt), 6, 12)
        g.tb(ROCK, (xm, -12, zt), (x1, -12, 132), 6, 12)
    for x, z in ((-104, 268), (-40, 280), (30, 272), (96, 262), (116, 216)):
        g.dia(LIT, (x, -12, z), (10, 6, 10), glow=0.6)
    g.box(ASPH, (150, -8, 290), (40, 8, 8))
    g.box(LIT, (166, -8, 270), (20, 20, 28), glow=LAMP)


# ------------------------------------------------------------------------------------------ the hero
def wind_harp(sc):
    """THE WIND HARP (left of the road: drawn as an L piece, the wind to the mesh's -X). A fan of nine leaning masts,
    23 to 55 m, on stone feet that step down the hill; a cable from the great anchor stone to every top, two rows of
    strings from mast to mast, a long streamer off every top and a shorter one below it, violet and mint by turns,
    all streaming one way; a steady mint lamp on every top; the tuner's hut and a crow's nest on the tallest."""
    g = G(sc, -1.0)
    H = (2300.0, 2800.0, 3300.0, 3750.0, 4200.0, 4600.0, 4950.0, 5250.0, 5500.0)
    X0 = -3600.0
    feet, tops = [], []
    for i in range(9):
        x = X0 + i * 680.0
        y = (i - 4) * (i - 4) * 12.0 - 100.0
        feet.append((x, y, gz(x)))
        tops.append((x + 0.26 * H[i], y, gz(x) + H[i]))

    def on(i, t):
        f, p = feet[i], tops[i]
        return (f[0] + (p[0] - f[0]) * t, f[1], f[2] + (p[2] - f[2]) * t)
    for i in range(9):
        f = feet[i]
        g.cyl(SLATE, (f[0], f[1], f[2] - 300), 200, 420, steps=6, top_r=112)
        g.rod(ASPH, (f[0], f[1], f[2] - 100), tops[i], 100, top_r=34, steps=6)     # fix round: masts 2 m thick at the foot (were 1.2: a faint comb from the road)
        for t, col in ((0.33, VIO), (0.62, MINT)):
            g.rod(col, on(i, t), on(i, t + 0.035), 100 - 66 * t + 9, top_r=100 - 66 * (t + 0.035) + 9, steps=6)
        g.box(MINT, (tops[i][0], tops[i][1], tops[i][2] + 40), (90, 90, 90), glow=LAMP)
    ax = X0 - 700.0                                                             # the anchor stone
    az = gz(ax)
    g.box(SLATE, (ax, -100, az), (540, 440, 700))
    g.box(ROCK, (ax, -100, az + 330), (570, 470, 50))
    g.tb(VIO, (ax - 290, -100, az + 372), (ax + 290, -100, az + 440), 480, 22)
    g.box(LIT, (ax, -322, az + 200), (120, 8, 90), glow=WIN)
    g.box(MINT, (ax, -322, az + 254), (150, 10, 14))
    anchor = (ax + 120, -100, az + 420)
    for i in range(9):
        g.rod(ASPH, anchor, tops[i], 7, steps=4)
    for i in range(8):                                                          # the strings
        for t in (0.5, 0.78):
            g.rod(STEEL, on(i, t), on(i + 1, t), 6, steps=4)
    for i in range(9):                                                          # the streamers
        col, col2 = ((VIO, MINT), (MINT, VIO))[i % 2]
        a = tops[i]
        L = 900.0 + i * 90.0
        rate = 0.34 + 0.02 * i

        def d(s, dz=0.0):
            return (a[0] + s, a[1], a[2] - s * 0.10 + dz)
        m1 = g.mov(swing=5, pivot=a, axis=90.0, rate=rate, phase=0.11 * i)
        m1.tb(col, d(10), d(L * 0.55), 8, 300)                                # fix round: streamers 3 m deep (were 1.2)
        m2 = g.mov(swing=8, pivot=a, axis=90.0, rate=rate, phase=0.11 * i - 0.12)
        m2.tb(col, d(L * 0.5), d(L), 8, 220)
        m2.tb(col, d(L - 20, 55), d(L + 260, 130), 8, 80)
        m2.tb(col, d(L - 20, -55), d(L + 260, -130), 8, 80)
        b = on(i, 0.7)
        m3 = g.mov(swing=7, pivot=b, axis=90.0, rate=rate + 0.05, phase=0.11 * i + 0.4)
        m3.tb(col2, (b[0] + 10, b[1], b[2]), (b[0] + L * 0.6, b[1], b[2] - L * 0.07), 8, 180)
    for i in (1, 3, 5, 7):                                                      # pennants on the anchor cables
        p = tuple(anchor[k] + (tops[i][k] - anchor[k]) * 0.5 for k in range(3))
        mp = g.mov(swing=10, pivot=p, axis=90.0, rate=0.6, phase=0.2 * i)
        mp.tb((VIO, MINT)[(i // 2) % 2], (p[0], p[1], p[2] - 34), (p[0] + 190, p[1], p[2] - 60), 8, 60)
    for i in range(8):                                                          # and on the upper strings
        u, v = on(i, 0.78), on(i + 1, 0.78)
        p = tuple((u[k] + v[k]) / 2.0 for k in range(3))
        ms = g.mov(swing=11, pivot=p, axis=90.0, rate=0.7 + 0.03 * i, phase=0.17 * i)
        ms.tb((MINT, VIO)[i % 2], (p[0], p[1], p[2] - 30), (p[0] + 150, p[1], p[2] - 52), 8, 50)
    f = feet[8]                                                                 # the tuner's hut by the tallest mast
    hx, hy, hz = f[0] - 330, f[1] - 260, gz(f[0] - 330)
    g.box(SLATE, (hx, hy, hz), (420, 320, 500))
    ys = (-180, -90, 0, 90, 180)
    zs = _arch(ys, hz + 252, 90)
    for k in range(4):
        g.tb(VIO, (hx, hy + ys[k], zs[k]), (hx, hy + ys[k + 1], zs[k + 1]), 470, 20)
    g.box(LIT, (hx - 80, hy - 162, hz + 150), (110, 8, 80), glow=WIN)
    g.box(ASPH, (hx + 110, hy - 162, hz + 100), (90, 8, 190))
    g.box(MINT, (hx + 110, hy - 162, hz + 202), (114, 10, 14))
    n = on(8, 0.36)                                                             # the crow's nest
    g.box(ASPH, (n[0], n[1], n[2]), (300, 300, 18))
    for sx, sy, sz in ((0, -146, (300, 8, 10)), (0, 146, (300, 8, 10)), (-146, 0, (8, 300, 10)), (146, 0, (8, 300, 10))):
        g.box(MINT, (n[0] + sx, n[1] + sy, n[2] + 100), sz)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(ASPH, (n[0] + sx * 146, n[1] + sy * 146, n[2] + 54), (8, 8, 100))


# ------------------------------------------------------------------------------------------ the jobs
def _twin(fn, w):
    return lambda sc: fn(sc, w)


TWINS = [('Refuge', refuge, {}), ('SlitDome', slit_dome, {}), ('LegCabin', leg_cabin, {}), ('KiteLoft', kite_loft, {}),
         ('WindWall', wind_wall, {}), ('InstrumentHouse', instrument_house, {}),
         ('VaneRow', vane_row, dict(grow=(20, 20, 6), grow_down=(20, 20, 0))), ('StreamerLine', streamer_line, {}),
         ('Anemometer', anemometer, dict(grow=(40, 40, 10), grow_down=(40, 40, 0)))]
SINGLES = [('Telescope', telescope, STILL), ('LampCairn', lamp_cairn, STILL), ('WindBreak', wind_break, STILL), ('SkyBoard', sky_board, STILL),
           ('WindHarp', wind_harp, dict(far=True, grow=(120, 60, 280), grow_down=(120, 60, 0)))]
JOBS = []
for _n, _f, _o in TWINS:
    JOBS.append(('SM_S1D5_' + _n, _twin(_f, 1.0), _o))
    JOBS.append(('SM_S1D5_' + _n + 'L', _twin(_f, -1.0), _o))
for _n, _f, _o in SINGLES:
    JOBS.append(('SM_S1D5_' + _n, _f, _o))


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
