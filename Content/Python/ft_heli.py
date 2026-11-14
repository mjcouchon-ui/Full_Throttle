"""The Syndicate chopper (1.3): a light utility helicopter in Syndicate purple that drops henchmen onto
the KESTREL. Same conventions as ft_car: X forward, Y right, Z up, centimetres. The origin is on the
ground under the rotor mast (the skids stand on it). The main and tail rotor blades are not modelled:
the game draws them spinning, as a blurred disc, so they never freeze in a still frame.
"""
import ft_art
from ft_car import BLACK, GLASS, CHROME, STEEL, PINK, CYAN, YEL, WHITE

PURPLE = '#7b3bd6'
PURPLE_D = '#5a2aa8'
HAZARD = '#ffd23f'
DOOR = '#16121f'


def heli(m):
    # ---- the cabin pod: a rounded bubble nose and a flat back where the boom starts
    pod = [(-120, 40), (150, 40), (212, 58), (238, 100), (226, 148), (176, 188), (60, 206), (-110, 202), (-150, 172), (-162, 92)]
    m.side(PURPLE, pod, -95, 95)
    # the bubble canopy over the nose, and the side windows
    m.side(GLASS, [(150, 104), (236, 100), (224, 150), (172, 190), (118, 192), (116, 120)], -97, 97)
    m.side(GLASS, [(-100, 130), (-20, 130), (-20, 182), (-96, 178)], -97, 97)
    # both sliding doors are open: the cabin is dark inside
    m.side(DOOR, [(-10, 52), (110, 52), (110, 178), (-10, 180)], -96.5, 96.5)
    # a hazard stripe down each flank and a black belly
    for s in (-1, 1):
        y0, y1 = (95, 97.5) if s > 0 else (-97.5, -95)
        m.side(HAZARD, [(-158, 92), (230, 92), (232, 102), (-160, 102)], y0, y1)
        m.side(PINK, [(-158, 104), (226, 104), (226, 108), (-158, 108)], y0, y1, glow=0.4)
    m.box(BLACK, (40, 0, 44), (300, 150, 10))
    # ---- the engine deck on top, exhausts, the mast and the hub (the blades are drawn by the game)
    m.box(PURPLE_D, (-40, 0, 220), (190, 120, 40))
    m.box(BLACK, (-40, 0, 242), (170, 100, 6))
    for s in (-1, 1):
        m.cyl(CHROME, (-120, s * 40, 226), 9, 30, rot=(-90, 0, 0), steps=12)
        m.cyl(BLACK, (-150, s * 40, 226), 6, 4, rot=(-90, 0, 0), steps=12)
    m.cyl(BLACK, (20, 0, 240), 10, 44, steps=12)
    m.box(STEEL, (20, 0, 286), (46, 46, 16))
    m.box(HAZARD, (20, 0, 296), (24, 24, 6))
    # ---- the tail boom, the fin and the stabiliser
    m.side(PURPLE, [(-150, 150), (-570, 178), (-570, 198), (-150, 200)], -22, 22)
    m.side(HAZARD, [(-160, 198), (-560, 196), (-560, 200), (-160, 202)], -14, 14)
    m.side(PURPLE_D, [(-530, 176), (-600, 170), (-648, 310), (-604, 312), (-560, 206)], -5, 5)
    m.box(PURPLE_D, (-520, 0, 186), (40, 150, 6))
    for s in (-1, 1):
        m.box(HAZARD, (-520, s * 72, 186), (40, 8, 8))
    m.cyl(STEEL, (-612, 6, 240), 9, 10, rot=(0, 0, 90), steps=12)
    # ---- skids and struts
    for s in (-1, 1):
        m.cyl(CHROME, (-130, s * 88, 6), 5, 330, rot=(-90, 0, 0), steps=10)
        m.cyl(CHROME, (200, s * 88, 6), 5, 30, rot=(-60, 0, 0), steps=10)
        for x in (-60, 110):
            m.box(BLACK, (x, s * 80, 26), (10, 10, 44), rot=(0, 0, s * -14))
    # ---- lights: a red beacon under the belly, pink nav lights, a white strobe on the fin,
    # and the searchlight under the nose
    m.box('#ff3355', (40, 0, 36), (18, 18, 8), glow=1.4)
    m.box(PINK, (-575, 0, 200), (10, 10, 10), glow=1.4)
    m.box(WHITE, (-640, 0, 312), (8, 8, 8), glow=1.2)
    for s in (-1, 1):
        m.box(CYAN if s > 0 else '#ff3355', (100, s * 99, 200), (10, 4, 8), glow=1.2)
    m.cyl(BLACK, (205, 0, 30), 14, 18, steps=12)
    m.cyl(YEL, (205, 0, 26), 11, 3, steps=12, glow=1.6)
    return m
