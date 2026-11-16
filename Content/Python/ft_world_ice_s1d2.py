"""Ice Mountain, Stage 1, builder s1d2: "Black diamond", the avalanche works (2.43 to 5.65 km).
/Game/World/IceMountain/SM_S1D2_*. Run: import ft_world_ice_s1d2 as K; K.run()

Heavy wedge bunkers with the prow uphill, mono-pitch roofs, steel gantries and plain diamond plates (a black rhombus on
yellow: a shape, no word), in charcoal and yellow with concrete, steel, slate and dark glass; and THE BLACK DIAMOND, a
charcoal octahedron standing still on its point in a concrete cradle. Every piece is an ft_people.Scene (one material,
one section). A piece's +X points DOWN the hill on the right of the road and UP it on the left, so the pieces with an
uphill end (the splitter bunker, the patrol hut) have a left-hand twin (SM_..._L), drawn mirrored.
Nothing dark moves here (the chopper flies in this district): only yellow pennants, a yellow vane, a yellow bar and a
depth staff swing, slowly. Lamps and lit windows are steady; every placement is 40 m or more from the road.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
CHAR, YEL, CONC, STEEL, SLATE, GLASS = '#3b3f52', '#ffd23f', '#aab3c4', '#9aa4b8', '#5a6478', '#1d2740'
SNOW, SNOW_SH, LIT = '#f4f9ff', '#cdd6f1', '#ffe28a'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SWAY = dict(grow=(40, 40, 20), grow_down=(40, 40, 0))


def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


class MX(object):
    """An ft_art.Model seen through a mirror in x (d = -1) or as it is (d = 1): the left-hand twins are drawn with it."""

    def __init__(self, m, d=1):
        self.m, self.d = m, d

    def _p(self, p):
        return (p[0] * self.d, p[1], p[2])

    def _r(self, rot):
        return rot if self.d > 0 else (-rot[0], -rot[1], rot[2])

    def box(self, col, c, size, rot=(0, 0, 0), glow=0.0):
        self.m.box(col, self._p(c), size, rot=self._r(rot), glow=glow)

    def cyl(self, col, base, r, h, rot=(0, 0, 0), steps=16, glow=0.0, top_r=None):
        self.m.cyl(col, self._p(base), r, h, rot=self._r(rot), steps=steps, glow=glow, top_r=top_r)

    def sphere(self, col, c, r, scale=(1, 1, 1), steps=(8, 12), glow=0.0, rot=(0, 0, 0)):
        self.m.sphere(col, self._p(c), r, scale=scale, steps=steps, glow=glow, rot=self._r(rot))

    def side(self, col, pts, y0, y1, glow=0.0):
        self.m.side(col, _ccw([(x * self.d, z) for x, z in pts]), y0, y1, glow=glow)

    def plan(self, col, pts, z0, z1, glow=0.0):
        self.m.plan(col, _ccw([(x * self.d, y) for x, y in pts]), z0, z1, glow=glow)


def cone(g, col, a, dirn, r, top_r, h, steps=6, glow=0.0):
    """A tapered cylinder from a, h long, along dirn."""
    dx, dy, dz = dirn
    hyp = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-hyp, dz)) if hyp > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if hyp > 1e-6 else 0
    g.cyl(col, a, r, h, rot=(pitch, yaw, 0), steps=steps, top_r=top_r, glow=glow)


def chev(g, x0, x1, y, z, h, n, col=YEL, t=8.0):
    """A row of n chevrons on a wall that faces -Y."""
    step = (x1 - x0) / float(n)
    for k in range(n):
        x = x0 + (k + 0.25) * step
        tilt_box(g, col, (x, y, z - h / 2.0), (x + h * 0.5, y, z), t, h * 0.24)
        tilt_box(g, col, (x + h * 0.5, y, z), (x, y, z + h / 2.0), t, h * 0.24)


def diamond(g, x, y, z, s, t=10.0):
    """The district's plate on a wall that faces -Y: a black rhombus on a yellow one. No word, no mark."""
    g.box(YEL, (x, y, z), (s, t, s), rot=(45, 0, 0))
    g.box(CHAR, (x, y - 3, z), (s * 0.6, t, s * 0.6), rot=(45, 0, 0))


def diamond_x(g, x, y, z, s, sign=1, t=10.0):
    """The same on a wall that faces +X (sign 1) or -X (sign -1)."""
    g.box(YEL, (x, y, z), (t, s, s), rot=(0, 0, 45))
    g.box(CHAR, (x + 3 * sign, y, z), (t, s * 0.6, s * 0.6), rot=(0, 0, 45))


def snow(g, x, y, z, rx, ry, h=40.0):
    g.sphere(SNOW, (x, y, z), 100, scale=(rx / 100.0, ry / 100.0, h / 100.0), steps=(4, 7))


def pennant(sc, d, x, y, z0, h, phase=0.2, w=120.0):
    """A steel mast with a yellow pennant that swings about it (it flies down the hill)."""
    g = MX(sc.still(), d)
    g.cyl(STEEL, (x, y, z0), 7, h, steps=5)
    g.sphere(YEL, (x, y, z0 + h + 8), 12, steps=(4, 6))
    f = MX(sc.part(P.motion(swing=14, pivot=(x * d, y, z0 + h - 40), axis=P.UP, rate=0.45, phase=phase)), d)
    f.box(YEL, (x + w * 0.5 + 6, y, z0 + h - 40), (w, 5, 58))
    f.box(YEL, (x + w + 26, y, z0 + h - 40), (40, 5, 30))


# ------------------------------------------------------------------------------------------ buildings
def splitter(d):
    """THE SPLITTER BUNKER, 23 x 9.6 m: a concrete prow uphill, steel-shod, under a charcoal cap whose mono-pitch roof
    rises down the hill from 4.3 m (blunt) to 7.4 m."""
    def build(sc):
        g = MX(sc.still(), d)
        g.plan(CONC, [(-1200, -130), (-530, -490), (1130, -490), (1130, 490), (-530, 490), (-1200, 130)], -300, 40)
        g.plan(CHAR, [(-1150, -110), (-520, -450), (1100, -450), (1100, 450), (-520, 450), (-1150, 110)], 40, 330)     # fix round: charcoal walls on the concrete foot (were concrete: read pale grey)
        g.plan(CHAR, [(-1130, -100), (-520, -430), (-430, -430), (-430, 430), (-520, 430), (-1130, 100)], 330, 430)
        g.side(CHAR, [(-460, 330), (1100, 330), (1100, 720), (-460, 470)], -450, 450)
        tilt_box(g, SLATE, (-530, 0, 472), (1160, 0, 743), 990, 24)
        tilt_box(g, SNOW, (-330, 40, 533), (1040, 40, 753), 880, 34)
        snow(g, 500, 120, 700, 420, 300, 46)
        snow(g, -800, 0, 428, 260, 150, 30)
        for s in (-1, 1):                                              # the steel shoe of the prow and its rail
            tilt_box(g, STEEL, (-1162, s * 118, 180), (-530, s * 462, 180), 14, 210)
            tilt_box(g, YEL, (-1160, s * 116, 310), (-528, s * 460, 310), 16, 30)
            tilt_box(g, STEEL, (-1110, s * 92, 515), (-530, s * 410, 515), 7, 7)
            for k in range(3):
                x = -1090 + k * 270
                g.box(STEEL, (x, s * (92 + (x + 1110) * 0.548), 475), (7, 7, 90))
        g.box(YEL, (-1160, 0, 180), (16, 230, 210))                    # the blunt tip: a hazard plate
        for y in (-58, 58):
            g.box(CHAR, (-1166, y, 180), (12, 40, 200), rot=(0, 0, 28))
        g.box(YEL, (320, -453, 345), (1560, 8, 50))                    # the front: a yellow band (fix round: 50 deep, was 24)
        g.box(CHAR, (300, -451, 210), (1500, 6, 150))
        chev(g, -440, 1040, -456, 210, 120, 9)
        for k, x in enumerate((-250, 130, 510)):
            g.box(GLASS, (x, -452, 420), (190, 10, 40))
            g.box(CONC, (x, -464, 448), (220, 30, 14))
        g.box(LIT, (800, -452, 520), (150, 10, 76), glow=0.6)
        g.box(CONC, (800, -464, 566), (180, 30, 14))
        g.box(STEEL, (960, -455, 160), (150, 10, 240))
        for x in (875, 1045):
            g.box(YEL, (x, -454, 165), (20, 12, 250))
        g.box(YEL, (960, -454, 292), (190, 12, 20))
        g.box(CONC, (960, -505, 15), (240, 110, 50))
        g.box(LIT, (960, -470, 320), (60, 22, 16), glow=0.9)
        diamond_x(g, 1105, 0, 500, 180)                                # the tall end
        for y in (-270, 270):
            g.box(GLASS, (1103, y, 420), (10, 150, 90))
            g.box(CONC, (1110, y, 472), (24, 180, 14))
        g.box(YEL, (1103, 0, 345), (8, 900, 24))
        for x in (-300, 200, 700):                                     # buttresses behind
            g.box(CONC, (x, 480, 140), (70, 90, 380))
        for x, hh in ((250, 150), (420, 110)):                         # vents through the roof
            z = 480 + (x + 530) * 0.16
            g.cyl(STEEL, (x, 200, z), 28, hh, steps=6)
            g.box(SLATE, (x, 180, z + hh + 20), (84, 120, 44))
        pennant(sc, d, 760, -330, 640, 400)
    return build


def blast_shelter(sc):
    """THE BLAST SHELTER, 13.6 x 9.8 m: a low concrete vault between charcoal end walls, a yellow blast door in a portal."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -110), (1360, 900, 300))
    rod(g, CONC, (-600, 0, 60), (600, 0, 60), 420, steps=12)
    for x in (-380, 330):
        rod(g, YEL, (x, 0, 60), (x + 50, 0, 60), 428, steps=12)
    for x in (-640, 590):
        rod(g, CHAR, (x, 0, 60), (x + 50, 0, 60), 452, steps=12)
    g.box(CONC, (0, -400, 170), (420, 220, 400))
    g.box(CHAR, (0, -405, 395), (480, 240, 54))
    g.box(YEL, (0, -514, 160), (260, 10, 280))
    for z in (80, 240):
        g.box(CHAR, (0, -518, z), (260, 8, 24))
    rod(g, STEEL, (0, -532, 160), (0, -516, 160), 36, steps=8)
    g.box(STEEL, (0, -536, 160), (90, 6, 10))
    for s in (-1, 1):
        g.box(CONC, (s * 300, -470, 90), (40, 300, 240), rot=(0, s * 25, 0))
        g.box(YEL, (s * 300, -470, 214), (44, 304, 10), rot=(0, s * 25, 0))
        g.box(GLASS, (s * 642, 0, 300), (10, 170, 40))
        g.box(CONC, (s * 650, 0, 330), (20, 200, 12))
    diamond(g, 0, -531, 395, 62, t=8)
    g.box(LIT, (170, -516, 330), (30, 14, 30), glow=0.9)
    for x, hh in ((-250, 130), (250, 190)):
        g.cyl(STEEL, (x, 60, 450), 30, hh, steps=6)
        g.box(SLATE, (x, 40, 470 + hh), (84, 120, 46))
        g.cyl(YEL, (x, 60, 480), 33, 16, steps=6)
    snow(g, 0, 110, 455, 520, 250, 50)
    snow(g, -420, -60, 440, 150, 120, 30)
    g.box(CONC, (0, -560, 10), (300, 110, 50))


def study_tower(sc):
    """THE SNOW-STUDY TOWER, 21 m: a square charcoal shaft carrying a glass room that hangs out over it on steel struts."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -150), (620, 620, 340))
    g.box(CHAR, (0, 0, 760), (500, 500, 1500))
    g.box(YEL, (0, 0, 1250), (510, 510, 60))
    g.box(YEL, (0, 0, 120), (510, 510, 40))
    for z in (380, 680):
        g.box(GLASS, (0, -252, z), (60, 8, 170))
        g.box(GLASS, (252, 0, z + 60), (8, 60, 170))
    g.box(LIT, (0, -252, 980), (60, 8, 170), glow=0.6)
    diamond(g, 0, -256, 1400, 150)
    for y in (-60, 60):                                                # the ladder on the uphill face, in its hoops
        g.box(STEEL, (-262, y, 760), (8, 8, 1400))
    for k in range(10):
        g.box(STEEL, (-262, 0, 130 + k * 140), (6, 120, 6))
    for z in (500, 800, 1100, 1400):
        g.box(STEEL, (-300, 0, z), (84, 160, 8))
    g.box(CONC, (0, -60, 1540), (940, 760, 60))                        # the room
    g.box(GLASS, (0, -60, 1700), (880, 700, 260))
    g.box(CHAR, (0, -60, 1860), (980, 800, 60))
    g.box(YEL, (0, -60, 1826), (986, 806, 14))
    snow(g, 0, -40, 1888, 430, 340, 40)
    for x in (-440, -220, 0, 220, 440):
        g.box(STEEL, (x, -412, 1700), (14, 10, 260))
    for s in (-1, 1):
        for y in (-410, -60, 290):
            g.box(STEEL, (s * 442, y, 1700), (10, 14, 260))
        tilt_box(g, STEEL, (s * 250, -250, 1180), (s * 430, -410, 1510), 16, 16)
        tilt_box(g, STEEL, (s * 250, 250, 1180), (s * 430, 290, 1510), 16, 16)
    g.box(LIT, (0, -414, 1596), (860, 6, 16), glow=0.6)
    g.box(GLASS, (-250, -200, 1930), (260, 170, 10), rot=(0, 0, -22))   # a solar panel
    g.cyl(STEEL, (250, 100, 1890), 10, 420, steps=6)                   # the instrument mast
    g.box(STEEL, (250, 100, 2200), (220, 8, 8))
    for s in (-1, 1):
        g.sphere(STEEL, (250 + s * 110, 100, 2200), 20, steps=(4, 6))
    v = MX(sc.part(P.motion(swing=22, pivot=(250, 100, 2290), axis=P.UP, rate=0.22, phase=0.4)))
    v.box(YEL, (300, 100, 2290), (150, 5, 44))
    g.sphere(YEL, (250, 100, 2318), 12, steps=(4, 6))


def patrol_hut(d):
    """THE PATROL HUT, 10 x 7 m: mono-pitch (3.3 m uphill, 5.6 m downhill), a yellow end wall with a charcoal zigzag
    facing up the road, a striped awning, skis racked by the door."""
    def build(sc):
        g = MX(sc.still(), d)
        g.box(CONC, (0, 0, -110), (980, 680, 300))
        g.side(CHAR, [(-450, 40), (450, 40), (450, 560), (-450, 330)], -300, 300)
        tilt_box(g, SLATE, (-530, 0, 320), (530, 0, 592), 730, 22)
        tilt_box(g, SNOW, (-430, 30, 364), (480, 30, 598), 640, 30)
        snow(g, 150, 60, 560, 300, 240, 36)
        g.box(YEL, (-454, 0, 185), (8, 580, 270))                      # the uphill end wall
        for k in range(8):
            g.box(CHAR, (-459, -245 + k * 70, 185), (8, 36, 150), rot=(0, 0, 32 if k % 2 else -32))
        g.box(YEL, (0, -303, 62), (900, 8, 36))                        # the front
        g.box(YEL, (250, -304, 170), (110, 8, 220))
        g.box(CHAR, (250, -308, 220), (60, 6, 60))
        g.box(GLASS, (-130, -304, 215), (220, 8, 110))
        for x in (-262, 2):
            g.box(YEL, (x, -307, 215), (36, 8, 124))
        for k in range(5):
            g.box(YEL if k % 2 == 0 else CHAR, (-244 + k * 57, -356, 304), (57, 120, 8), rot=(0, 0, -18))
        g.box(LIT, (250, -312, 300), (50, 18, 14), glow=0.9)
        g.box(CONC, (150, -390, 12), (420, 160, 56))
        for k in range(4):                                             # skis and probes against the wall
            g.box(YEL if k % 2 else STEEL, (66 + k * 22, -314, 160), (14, 6, 210), rot=(0, 0, -5))
        for k in range(3):
            rod(g, YEL, (370 + k * 20, -330, 40), (366 + k * 20, -306, 330), 5, steps=5)
        g.box(STEEL, (-330, -340, 60), (150, 40, 8))                   # a bench
        for x in (-390, -270):
            g.box(STEEL, (x, -340, 48), (8, 36, 40))
        diamond_x(g, 455, 0, 320, 150)                                 # the downhill end
        g.box(GLASS, (453, 190, 250), (8, 100, 90))
        g.cyl(STEEL, (300, 160, 520), 16, 190, steps=6)                # the stove pipe
        g.cyl(CHAR, (300, 160, 710), 27, 14, steps=6)
        pennant(sc, d, 420, -260, 560, 330, phase=0.6, w=110.0)
    return build


def gallery_portal(sc):
    """THE GALLERY PORTAL, 19.6 x 7.6 m: four raked concrete frames in a row under a charcoal lintel with chevrons, three
    dark bays behind; a yellow height bar hangs in the middle one."""
    g = MX(sc.still())
    g.box(CONC, (0, 40, -120), (1960, 720, 300))
    g.box(CHAR, (0, 300, 300), (1860, 60, 560))                        # fix round: the back wall charcoal (was concrete)
    for x in (-900, -300, 300, 900):
        tilt_box(g, CONC, (x, -300, 30), (x, -200, 580), 90, 90)
        g.box(CONC, (x, 50, 612), (100, 640, 80), rot=(0, 0, -6))
        g.box(YEL, (x, -296, 110), (96, 10, 120))                         # fix round: taller yellow feet on the frames
    g.box(CHAR, (0, -215, 660), (1960, 70, 150))
    chev(g, -900, 900, -252, 660, 100, 12)
    g.box(SLATE, (0, 60, 712), (2000, 780, 40), rot=(0, 0, -6))
    g.box(SNOW, (0, 100, 752), (1900, 640, 50), rot=(0, 0, -6))
    snow(g, -300, 180, 800, 520, 220, 50)
    for k, x in enumerate((-600, 0, 600)):
        g.box(GLASS, (x, 264, 290), (480, 10, 470))
        g.box(LIT, (x, -200, 574), (140, 30, 14), glow=0.7)
    g.box(STEEL, (-600, 256, 240), (420, 10, 390))                     # a roller door in the first bay
    for k in range(4):
        g.box(SLATE, (-600, 250, 90 + k * 100), (420, 6, 12))
    for k in range(3):                                                 # pipes stacked in the last
        rod(g, CONC, (480 + k * 90, 40, 75), (480 + k * 90, 240, 75), 42, steps=8)
    rod(g, CONC, (525, 40, 150), (525, 240, 150), 42, steps=8)
    rod(g, CONC, (615, 40, 150), (615, 240, 150), 42, steps=8)
    g.box(STEEL, (0, -215, 760), (14, 14, 60))
    diamond(g, 0, -228, 850, 150)
    b = MX(sc.part(P.motion(swing=3, pivot=(0, -150, 580), axis=0, rate=0.3, phase=0.1)))
    b.box(YEL, (0, -150, 400), (440, 12, 34))
    for x in (-190, 190):
        b.box(YEL, (x, -150, 495), (6, 6, 170))


def gear_store(sc):
    """THE GEAR STORE, 11 x 8 m: a ribbed steel shed, its gable to the road with the diamond plate over a yellow door."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -110), (1100, 800, 300))
    g.side(STEEL, [(-480, 40), (480, 40), (480, 430), (0, 650), (-480, 430)], -360, 360)
    tilt_box(g, SLATE, (-540, 0, 396), (0, 0, 668), 800, 22)
    tilt_box(g, SLATE, (0, 0, 668), (540, 0, 396), 800, 22)
    tilt_box(g, SNOW, (-470, 20, 452), (-40, 20, 668), 720, 26)
    tilt_box(g, SNOW, (40, 20, 668), (470, 20, 452), 720, 26)
    for k in range(8):
        x = -420 + k * 120
        g.box(SLATE, (x, -362, 235), (12, 8, 390))
    g.box(YEL, (0, -364, 180), (360, 10, 280))
    for k in range(3):
        g.box(CHAR, (0, -368, 80 + k * 90), (360, 6, 22))
    g.box(CHAR, (0, -364, 330), (400, 10, 22))
    diamond(g, 0, -368, 500, 140)
    g.box(LIT, (0, -376, 372), (70, 20, 16), glow=0.9)
    g.box(CONC, (0, -410, 12), (520, 100, 56))
    g.box(YEL, (0, -462, 14), (520, 12, 58))
    g.box(GLASS, (-482, -100, 290), (8, 220, 90))
    g.box(SLATE, (610, -100, 310), (270, 440, 14))                     # the lean-to rack
    snow(g, 610, -100, 322, 130, 210, 22)
    for y in (-300, 100):
        g.box(STEEL, (730, y, 170), (10, 10, 270))
    for k in range(5):
        rod(g, YEL, (520 + k * 34, -260, 40), (500 + k * 34, -210, 300), 6, steps=5)
    g.cyl(CHAR, (640, 20, 40), 52, 30, steps=8)
    g.cyl(YEL, (640, 20, 70), 30, 8, steps=8)
    g.box(CHAR, (640, -110, 80), (120, 70, 70))                        # a charcoal chest under the rack, a yellow lid
    g.box(YEL, (640, -110, 118), (124, 74, 8))
    g.cyl(STEEL, (0, 180, 640), 30, 70, steps=6)
    g.cyl(SLATE, (0, 180, 710), 44, 14, steps=6)
    pennant(sc, 1, 0, -340, 650, 300, phase=0.8, w=110.0)


def winch_house(sc):
    """THE WINCH HOUSE, 15 x 6 m: a charcoal block, and beside it a cable drum under a yellow A-frame, its cable run
    out to an anchor block."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -110), (1500, 600, 300))
    g.box(CHAR, (-380, 40, 330), (560, 480, 580))
    g.box(YEL, (-380, 40, 560), (566, 486, 36))
    g.box(SLATE, (-380, 40, 652), (650, 570, 30), rot=(0, 0, -7))
    g.box(SNOW, (-380, 60, 690), (580, 480, 36), rot=(0, 0, -7))
    g.box(GLASS, (-420, -202, 400), (240, 8, 110))
    g.box(LIT, (-420, -203, 330), (240, 6, 14), glow=0.6)
    g.box(YEL, (-180, -203, 180), (100, 8, 240))
    g.box(GLASS, (-98, 40, 300), (8, 300, 360))
    diamond_x(g, -665, 40, 360, 160, sign=-1)
    g.box(LIT, (-180, -212, 320), (50, 18, 14), glow=0.9)
    rod(g, SLATE, (250, -150, 200), (250, 150, 200), 130, steps=10)    # the drum
    for y in (-172, 150):
        rod(g, YEL, (250, y, 200), (250, y + 22, 200), 176, steps=10)
    rod(g, STEEL, (250, -230, 200), (250, 230, 200), 18, steps=6)
    for s in (-1, 1):
        g.box(STEEL, (250, s * 200, 110), (130, 30, 200))
        tilt_box(g, YEL, (60, s * 240, 40), (250, s * 240, 520), 26, 26)
        tilt_box(g, YEL, (440, s * 240, 40), (250, s * 240, 520), 26, 26)
        g.box(YEL, (250, s * 240, 250), (220, 20, 20))
    g.box(YEL, (250, 0, 530), (44, 520, 44))
    rod(g, STEEL, (250, -30, 500), (250, 30, 500), 50, steps=8)
    tilt_box(g, SLATE, (250, 0, 330), (250, 0, 500), 9, 9)
    tilt_box(g, SLATE, (250, 0, 520), (690, 0, 80), 9, 9)
    g.box(CONC, (700, 0, 30), (110, 130, 100))
    g.box(YEL, (700, 0, 84), (114, 134, 10))
    rod(g, STEEL, (680, -40, 90), (680, 40, 90), 14, steps=6)
    g.cyl(STEEL, (-520, 160, 640), 22, 120, steps=6)
    g.cyl(CHAR, (-520, 160, 760), 32, 12, steps=6)
    snow(g, 520, -180, 44, 130, 80, 24)


# ------------------------------------------------------------------------------------------ the hero
def black_diamond(sc):
    """THE BLACK DIAMOND: a charcoal octahedron 43 m tall and 30 m across, standing still on its point in a concrete
    cradle (the point 3.2 m up), a yellow band on every edge, belts of slate, dark glass and a lit line, four steel stays
    to anchor blocks. Nothing of it moves."""
    g = MX(sc.still())
    R, zb, ze, zt = 1500.0, 320.0, 2470.0, 4620.0

    def rad(z):
        if z <= ze:
            return 24.0 + (R - 24.0) * max(z - zb, 0.0) / (ze - zb)
        return 10.0 + (R - 10.0) * max(zt - z, 0.0) / (zt - ze)

    def belt(col, z0, dz, out=6.0, glow=0.0):
        g.cyl(col, (0, 0, z0), rad(z0) + out, dz, steps=4, top_r=rad(z0 + dz) + out, glow=glow)

    g.cyl(CHAR, (0, 0, zb), 24, ze - zb, steps=4, top_r=R)
    g.cyl(CHAR, (0, 0, ze), R, zt - ze, steps=4, top_r=10)
    belt(YEL, ze - 26, 26, out=10)                                     # the girdle
    belt(YEL, ze, 26, out=10)
    belt(YEL, zb - 6, 300, out=8)                                      # the point and the tip
    belt(YEL, zt - 380, 376, out=8)
    for z in (900, 1500, 3400, 3950):
        belt(SLATE, z, 18, out=5)
    belt(GLASS, ze + 520, 90, out=5)
    belt(LIT, ze + 470, 22, out=6, glow=0.5)
    belt(GLASS, ze - 610, 90, out=5)
    belt(LIT, ze - 640, 22, out=6, glow=0.5)
    k = 1.004
    V = [(R * k, 0, ze), (0, R * k, ze), (-R * k, 0, ze), (0, -R * k, ze)]
    for i in range(4):                                                 # the twelve edges
        a, b = V[i], V[(i + 1) % 4]
        tilt_box(g, YEL, a, b, 46, 46)
        tilt_box(g, YEL, a, (0, 0, zt - 60), 64, 40)
        tilt_box(g, YEL, a, (0, 0, zb + 60), 64, 40)
    g.cyl(STEEL, (0, 0, zt - 20), 9, 300, steps=5)                     # a rod and a steady lamp on the tip
    g.sphere(LIT, (0, 0, zt + 300), 42, steps=(4, 6), glow=1.0)
    # the cradle
    g.cyl(CONC, (0, 0, -600), 1040, 700, steps=8, rot=(0, 22.5, 0))     # eight-sided, a flat face to the road
    g.cyl(CONC, (0, 0, 100), 780, 150, steps=8, rot=(0, 22.5, 0))
    g.cyl(YEL, (0, 0, 236), 790, 26, steps=8, rot=(0, 22.5, 0))
    g.cyl(STEEL, (0, 0, 640), 310, 70, steps=8)
    snow(g, 0, 0, 250, 700, 700, 26)
    for i in range(4):
        a = math.radians(i * 90)
        c, s = math.cos(a), math.sin(a)
        tilt_box(g, CONC, (c * 800, s * 800, 120), (c * 380, s * 380, 860), 230, 230)      # a jaw under each lower edge
        tilt_box(g, YEL, (c * 715, s * 715, 512), (c * 526, s * 526, 845), 236, 14)
        g.box(CONC, (c * 2100, s * 2100, -200), (190, 190, 600))                           # the stays and their anchors
        g.box(YEL, (c * 2100, s * 2100, 104), (196, 196, 12))
        tilt_box(g, STEEL, (c * R * 1.01, s * R * 1.01, ze), (c * 2100, s * 2100, 90), 16, 16)
        a2 = a + math.radians(45)
        c2, s2 = math.cos(a2), math.sin(a2)
        g.box(CONC, (c2 * 760, s2 * 760, 260), (150, 150, 330), rot=(0, 45 + i * 90, 0))    # four posts between, each with a lamp
        g.box(LIT, (c2 * 740, s2 * 740, 440), (70, 70, 30), rot=(0, 45 + i * 90, 0), glow=0.8)
        g.cyl(CHAR, (c2 * 760, s2 * 760, 455), 8, 120, steps=4, top_r=86)                    # and a small diamond of its own on top
        g.cyl(CHAR, (c2 * 760, s2 * 760, 575), 86, 120, steps=4, top_r=6)
        g.cyl(YEL, (c2 * 760, s2 * 760, 568), 90, 14, steps=4)
    for i in range(8):                                                 # the rail round the cradle's deck
        a = math.radians(i * 45 + 22.5)
        g.box(STEEL, (math.cos(a) * 990, math.sin(a) * 990, 150), (9, 9, 110))
        a0, a1 = math.radians(i * 45 - 22.5), math.radians(i * 45 + 22.5)
        tilt_box(g, STEEL, (math.cos(a0) * 990, math.sin(a0) * 990, 205), (math.cos(a1) * 990, math.sin(a1) * 990, 205), 8, 8)
    for k2 in range(5):                                                # steps up from the road's side, a door
        g.box(CONC, (0, -990 - k2 * 50, 80 - k2 * 38), (260, 60, 40))
    g.box(CHAR, (0, -724, 170), (150, 12, 130))
    g.box(YEL, (0, -728, 240), (190, 12, 14))


# ------------------------------------------------------------------------------------------ props
def diamond_plate(sc):
    """TALL, 5.4 m: the warning plate on its post, a striped tab under it."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -30), (110, 110, 140))
    g.box(YEL, (0, 0, 46), (118, 118, 14))
    g.box(STEEL, (0, 0, 240), (18, 18, 400))
    diamond(g, 0, -14, 400, 190)
    g.box(YEL, (0, -13, 215), (124, 8, 54))
    for x in (-34, 34):
        g.box(CHAR, (x, -16, 215), (22, 6, 54))
    tilt_box(g, STEEL, (0, 70, 50), (0, 10, 210), 8, 8)
    g.sphere(SNOW, (0, -4, 536), 22, scale=(1.3, 0.6, 0.5), steps=(4, 6))


def bollard_row(sc):
    """LOW: five fat marker bollards, charcoal with yellow bands and caps, on a concrete kerb, a chain between."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -35), (780, 76, 130))
    g.box(YEL, (0, -39, 10), (780, 4, 24))
    for k in range(5):
        x = -320 + k * 160
        g.cyl(CHAR, (x, 0, 30), 26, 92, steps=7)
        g.cyl(YEL, (x, 0, 54), 29, 20, steps=7)
        g.cyl(YEL, (x, 0, 92), 29, 20, steps=7)
        g.sphere(YEL, (x, 0, 122), 27, scale=(1, 1, 0.6), steps=(4, 7))
        if k < 4:
            tilt_box(g, STEEL, (x + 26, 0, 100), (x + 80, 0, 82), 6, 6)
            tilt_box(g, STEEL, (x + 80, 0, 82), (x + 134, 0, 100), 6, 6)


def _pod(g, cx, cy, cz, turn, col=CONC, L=80.0, r=27.0, tip=None):
    g.sphere(col, (cx, cy, cz), r * 1.12, steps=(4, 6))
    cone(g, col, (cx, cy, cz), (0, 0, 1), r, r * 0.74, L, steps=6)
    if tip:
        g.cyl(tip, (cx, cy, cz + L - 14), r * 0.8, 16, steps=6)
    for i in range(3):
        a = math.radians(turn + i * 120)
        cone(g, col, (cx, cy, cz), (math.cos(a) * 0.943, math.sin(a) * 0.943, -0.333), r, r * 0.74, L, steps=6)


def tetrapods(sc):
    """LOW (1.9 m): five concrete tetrapods in a heap, one tipped yellow, snow between."""
    g = MX(sc.still())
    g.cyl(SNOW_SH, (0, 0, -45), 235, 52, steps=8)
    _pod(g, -95, -40, 46, 10)
    _pod(g, 95, -50, 46, 70, tip=YEL)
    _pod(g, 0, 105, 46, 40)
    _pod(g, -10, -10, 104, 95, col=SLATE)
    _pod(g, 150, 80, 44, 20, col=SLATE, L=70.0, r=24.0)
    g.sphere(SNOW, (-120, 70, 14), 60, scale=(1.3, 1.0, 0.3), steps=(4, 6))


def net_anchor(sc):
    """LOW (1.7 m): two steel A-frames on concrete blocks with the avalanche net rolled and tied between them, one bay let down."""
    g = MX(sc.still())
    for s in (-1, 1):
        g.box(CONC, (s * 300, 0, -10), (90, 130, 130))
        g.box(YEL, (s * 300, 0, 58), (94, 134, 8))
        tilt_box(g, STEEL, (s * 300, -54, 56), (s * 300, 0, 168), 12, 12)
        tilt_box(g, STEEL, (s * 300, 54, 56), (s * 300, 0, 168), 12, 12)
        tilt_box(g, STEEL, (s * 300, 0, 165), (s * 388, 0, -40), 6, 6)
    g.box(STEEL, (0, 0, 165), (620, 10, 10))
    rod(g, CHAR, (-270, 0, 128), (270, 0, 128), 26, steps=6)
    for x in (-220, -80, 60, 200):
        rod(g, YEL, (x, 0, 128), (x + 14, 0, 128), 30, steps=6)
    g.box(CHAR, (130, -8, 62), (230, 6, 96))
    for k in range(3):
        g.box(YEL, (50 + k * 80, -8, 12), (30, 12, 16))
    g.sphere(SNOW, (-150, 0, 156), 30, scale=(3.0, 0.7, 0.4), steps=(4, 6))


def siren_mast(sc):
    """TALL, 11 m: a steel lattice mast on a concrete foot, three plain yellow horns under a steady lamp."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -60), (200, 200, 220))
    g.box(YEL, (0, 0, 54), (206, 206, 10))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, STEEL, (sx * 80, sy * 80, 50), (sx * 30, sy * 30, 820), 12, 12)
    for z in (240, 440, 640):
        w = 80 - (z - 50) * 50.0 / 770.0
        for s in (-1, 1):
            g.box(STEEL, (0, s * w, z), (2 * w, 8, 8))
            g.box(STEEL, (s * w, 0, z), (8, 2 * w, 8))
    g.box(CHAR, (0, 0, 150), (150, 150, 110))
    g.box(YEL, (0, 0, 212), (156, 156, 14))
    g.box(SLATE, (0, 0, 827), (170, 170, 14))
    for s in (-1, 1):
        g.box(YEL, (0, s * 82, 890), (170, 6, 6))
        g.box(YEL, (s * 82, 0, 890), (6, 170, 6))
        for t in (-1, 1):
            g.box(YEL, (s * 82, t * 82, 862), (6, 6, 62))
    g.cyl(STEEL, (0, 0, 834), 11, 190, steps=6)
    for dirn, a in (((1, 0, 0.12), (30, 0, 930)), ((-1, 0, 0.12), (-30, 0, 930)), ((0, -1, 0.12), (0, -30, 985))):
        cone(g, YEL, a, dirn, 15, 64, 125, steps=8)
        cone(g, CHAR, a, dirn, 20, 20, 24, steps=6)
    g.cyl(CHAR, (0, 0, 1024), 24, 22, steps=6)
    g.sphere(LIT, (0, 0, 1072), 30, steps=(4, 6), glow=1.0)
    g.cyl(CHAR, (0, 0, 1098), 36, 10, steps=6)
    g.box(CHAR, (0, -112, 150), (70, 30, 100))
    diamond(g, 0, -129, 160, 40, t=6)


def depth_gantry(sc):
    """TALL, 8.7 m: a steel arch with a trussed beam, a yellow-banded depth staff hanging from it (it swings a little),
    a scale of yellow blocks up one leg."""
    g = MX(sc.still())
    for s in (-1, 1):
        g.box(CONC, (s * 420, 0, -40), (130, 150, 200))
        g.box(YEL, (s * 420, 0, 64), (134, 154, 10))
        g.box(STEEL, (s * 420, 0, 370), (28, 28, 620))
        tilt_box(g, STEEL, (s * 420, 0, 470), (s * 290, 0, 600), 14, 14)
    g.box(STEEL, (0, 0, 684), (900, 24, 24))
    g.box(STEEL, (0, 0, 600), (900, 18, 18))
    for k in range(6):
        x0 = -420 + k * 140
        tilt_box(g, STEEL, (x0, 0, 600 if k % 2 == 0 else 684), (x0 + 140, 0, 684 if k % 2 == 0 else 600), 10, 10)
    g.box(SNOW, (0, 0, 704), (840, 40, 16))
    g.box(CHAR, (420, -19, 340), (54, 8, 450))
    for k in range(5):
        g.box(YEL, (420, -24, 150 + k * 88), (54, 6, 44))
    g.box(SLATE, (-420, -28, 200), (64, 40, 84))
    g.box(GLASS, (-420, -49, 210), (42, 4, 38))
    g.box(STEEL, (0, 0, 726), (10, 10, 60))
    diamond(g, 0, -12, 800, 116)
    g.box(CHAR, (0, -5, 800), (74, 6, 74), rot=(45, 0, 0))
    st = MX(sc.part(P.motion(swing=4, pivot=(0, 0, 592), axis=0, rate=0.3, phase=0.3)))
    for k in range(6):
        st.box(YEL if k % 2 == 0 else CONC, (0, 0, 550 - k * 68), (30, 30, 68))
    st.cyl(CONC, (0, 0, 120), 4, 60, steps=6, top_r=26)


def chevron_barrier(sc):
    """LOW (1.3 m): eight metres of concrete blast wall with yellow chevrons on a charcoal band."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -40), (820, 110, 130))
    g.box(CONC, (0, 0, 70), (800, 56, 110))
    g.box(CHAR, (0, -30, 72), (780, 6, 88))
    chev(g, -372, 372, -35, 72, 78, 7, t=6.0)
    g.box(YEL, (0, 30, 72), (780, 6, 30))
    for x in (-300, 0, 300):
        g.box(CONC, (x, 52, 50), (44, 60, 120))
    for x in (-250, 250):
        g.box(STEEL, (x, 0, 132), (44, 10, 16))
    g.sphere(SNOW, (120, 0, 124), 30, scale=(5.0, 0.8, 0.3), steps=(4, 6))


def cable_spool(sc):
    """LOW (1.9 m): a steel cable drum with yellow flanges in concrete chocks, a second lying flat beside it."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -18), (310, 230, 46))
    rod(g, SLATE, (0, -62, 100), (0, 62, 100), 62, steps=10)
    for y in (-76, 62):
        rod(g, YEL, (0, y, 100), (0, y + 14, 100), 92, steps=10)
    rod(g, CHAR, (0, -84, 100), (0, 84, 100), 16, steps=6)
    for s in (-1, 1):
        g.box(CONC, (s * 78, 0, 22), (50, 170, 44), rot=(s * 20, 0, 0))
    g.cyl(YEL, (190, 40, 4), 70, 8, steps=10)
    g.cyl(SLATE, (190, 40, 12), 52, 44, steps=10)
    g.cyl(YEL, (190, 40, 56), 70, 8, steps=10)
    g.cyl(CHAR, (190, 40, 64), 16, 6, steps=6)
    tilt_box(g, SLATE, (40, -40, 40), (120, -100, 8), 8, 8)
    tilt_box(g, SLATE, (120, -100, 8), (230, -80, 8), 8, 8)
    g.sphere(SNOW, (-110, 60, 10), 40, scale=(1.2, 1.0, 0.3), steps=(4, 6))


def vent_cluster(sc):
    """TALL, 3.9 m: three concrete vent stacks with steel cowls on a pad, a louvre box behind, a yellow rail in front."""
    g = MX(sc.still())
    g.box(CONC, (0, 0, -30), (380, 250, 140))
    for x, hh in ((-115, 180), (0, 300), (115, 240)):
        g.cyl(CONC, (x, 0, 40), 38, hh, steps=8)
        g.cyl(YEL, (x, 0, 40 + hh - 34), 41, 16, steps=8)
        cone(g, STEEL, (x, 0, 34 + hh), (0, -0.75, 0.66), 40, 48, 76, steps=8)
        g.sphere(SNOW, (x, 14, 40 + hh + 54), 24, scale=(1.2, 1.0, 0.4), steps=(4, 6))
    g.box(CHAR, (0, 86, 110), (310, 54, 140))
    for k in range(3):
        g.box(SLATE, (0, 57, 70 + k * 40), (280, 6, 14))
    g.box(YEL, (0, -116, 120), (380, 7, 7))
    for x in (-185, 0, 185):
        g.box(YEL, (x, -116, 80), (7, 7, 84))


JOBS = [
    ('SM_S1D2_SplitterBunker', splitter(1), SWAY), ('SM_S1D2_SplitterBunker_L', splitter(-1), SWAY),
    ('SM_S1D2_BlastShelter', blast_shelter, STILL),
    ('SM_S1D2_StudyTower', study_tower, SWAY),
    ('SM_S1D2_PatrolHut', patrol_hut(1), SWAY), ('SM_S1D2_PatrolHut_L', patrol_hut(-1), SWAY),
    ('SM_S1D2_GalleryPortal', gallery_portal, SWAY),
    ('SM_S1D2_GearStore', gear_store, SWAY),
    ('SM_S1D2_WinchHouse', winch_house, STILL),
    ('SM_S1D2_BlackDiamond', black_diamond, STILL),
    ('SM_S1D2_DiamondPlate', diamond_plate, STILL),
    ('SM_S1D2_BollardRow', bollard_row, STILL),
    ('SM_S1D2_Tetrapods', tetrapods, STILL),
    ('SM_S1D2_NetAnchor', net_anchor, STILL),
    ('SM_S1D2_SirenMast', siren_mast, STILL),
    ('SM_S1D2_DepthGantry', depth_gantry, SWAY),
    ('SM_S1D2_ChevronBarrier', chevron_barrier, STILL),
    ('SM_S1D2_CableSpool', cable_spool, STILL),
    ('SM_S1D2_VentCluster', vent_cluster, STILL),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
