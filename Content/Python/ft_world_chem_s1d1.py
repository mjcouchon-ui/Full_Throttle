"""Chemical Works, Stage 1, builder s1d1: district 1 "Factory Gate" (0 to 2.6 km). /Game/World/ChemicalWorks/SM_S1D1_*.
The brick company town at the gate: two and three storey houses and workshops, sawtooth (north-light) roofs, steep
gables with white barge boards, paired chimneys, round-arched windows lit warm, a cornice on everything. Cream leads
(the level is a night level), then brick, white, hazard yellow and glass. The hero is THE CLOCK GATE, a 48 m brick
gate tower straddling a works road, a 12 m clock on each of its four faces, a steam whistle on the roof.
Placed by the block C:/ftwork/s1/L1_s1d1.inl. Run: import ft_world_chem_s1d1 as K; K.run()
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/ChemicalWorks'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))            # a piece with no moving part: true bounds
MOVE = dict(grow=(60, 60, 70), grow_down=(60, 60, 0))        # smoke that floats, a bell, a sign, washing
HERO = dict(grow=(40, 40, 120), grow_down=(40, 40, 0))

CREAM, BRICK, WHITE, YEL, GLASS = '#ffe9a8', '#a8552f', '#eef6ff', '#ffd23f', '#233a63'      # the district's five
LAMP, STEEL, STEAM, RED, PAVE, TEAL, BLUE, PINK, BLACK = '#fff1c0', '#8f9bb3', '#d8d0f0', '#ff3355', '#7f8fc6', '#19b5c9', '#3a5ab8', '#f3c4dc', '#1c1a26'
GL = 0.4                                        # a lit window (steady; every lit piece stands 40 m or more out)
FOOT = 300.0                                    # a building's footing below z = 0 (the district has two ramps)


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def prof(g, col, pts, y0, y1, glow=0.0):
    """A profile [(x, z)] seen from the road, from y0 to y1 deep."""
    g.side(col, ccw(pts), y0, y1, glow=glow)


def disc_y(g, col, x, y, z, r, t=12.0, steps=8, glow=0.0):
    """A disc facing the road: its front face at y, t thick away from the road."""
    g.cyl(col, (x, y, z), r, t, rot=(0, 0, 90), steps=steps, glow=glow)


def disc_x(g, col, x, y, z, r, t=12.0, steps=8, glow=0.0):
    """A disc facing along the road: from x - t (its -X face) to x."""
    g.cyl(col, (x, y, z), r, t, rot=(90, 0, 0), steps=steps, glow=glow)


def arc(cx, cz, r, a0, a1, n):
    return [(cx + r * math.cos(math.radians(a0 + (a1 - a0) * k / float(n))), cz + r * math.sin(math.radians(a0 + (a1 - a0) * k / float(n)))) for k in range(n + 1)]


def walls(g, col, x, y, w, d, z0, z1):
    g.box(col, (x, y, (z0 + z1) / 2.0), (w, d, z1 - z0))


def band(g, col, x, y, w, d, z, hh=26.0, proud=10.0):
    """A stripe right round a box body: a cream band, a white cornice."""
    g.box(col, (x, y, z), (w + 2 * proud, d + 2 * proud, hh))


def arched(g, face, a, wall, z0, w, h, lit=True, sill=True, steps=8):
    """A round-arched window on the wall that faces `face` ('-y' the road, '+y', '-x', '+x'); a = where along the
    wall, wall = the wall's own plane, z0 = the sill, w wide, h high to the crown."""
    r = w / 2.0
    hb = h - r
    col, gl = (LAMP, GL) if lit else (GLASS, 0.0)
    s = -1.0 if face[0] == '-' else 1.0
    if face[1] == 'y':
        g.box(col, (a, wall + s * 3.0, z0 + hb / 2.0), (w, 14, hb), glow=gl)
        g.cyl(col, (a, wall + (-9.5 if s < 0 else -2.5), z0 + hb), r, 12, rot=(0, 0, 90), steps=steps, glow=gl)
        if sill:
            g.box(WHITE, (a, wall + s * 10.0, z0 - 7), (w + 24, 26, 14))
        g.box(WHITE, (a, wall + s * 11.0, z0 + h - 6), (w * 0.2 + 10, 8, 22))
    else:
        g.box(col, (wall + s * 3.0, a, z0 + hb / 2.0), (14, w, hb), glow=gl)
        g.cyl(col, (wall + (2.5 if s < 0 else 9.5), a, z0 + hb), r, 12, rot=(90, 0, 0), steps=steps, glow=gl)
        if sill:
            g.box(WHITE, (wall + s * 10.0, a, z0 - 7), (26, w + 24, 14))
        g.box(WHITE, (wall + s * 11.0, a, z0 + h - 6), (8, w * 0.2 + 10, 22))


def pane(g, face, a, wall, z, w, h, lit=True):
    """A plain small window on a side wall (two triangles a face: the cheap one)."""
    col, gl = (LAMP, GL) if lit else (GLASS, 0.0)
    s = -1.0 if face[0] == '-' else 1.0
    if face[1] == 'y':
        g.box(col, (a, wall + s * 3.0, z), (w, 12, h), glow=gl)
    else:
        g.box(col, (wall + s * 3.0, a, z), (12, w, h), glow=gl)


def chevron(x, w, z0, rise, over, t):
    s = rise / (w / 2.0)
    e = w / 2.0 + over
    zl = z0 - over * s
    tv = t * math.sqrt(1.0 + s * s)
    return [(x - e, zl), (x, z0 + rise), (x + e, zl), (x + e, zl + tv), (x, z0 + rise + tv), (x - e, zl + tv)]


def gable(g, x, y0, y1, w, z0, rise, wall=CREAM, roof=GLASS, over=26.0, oy=28.0, t=22.0):
    """A steep gable facing the road: its wall, its roof (ridge running away from the road) and a white barge board."""
    prof(g, wall, [(x - w / 2.0, z0), (x + w / 2.0, z0), (x, z0 + rise)], y0, y1)
    prof(g, roof, chevron(x, w, z0, rise, over, t), y0 - oy, y1 + oy)
    prof(g, WHITE, chevron(x, w + 6.0, z0 - 2.0, rise + 2.0, over, t + 10.0), y0 - oy - 10.0, y0 - oy + 2.0)


def tooth(g, x0, x1, y0, y1, z0, h, wall=CREAM):
    """One tooth of a sawtooth roof: an upright lit north light on its -X face, a slope down to x1."""
    yc = (y0 + y1) / 2.0
    prof(g, wall, [(x0, z0), (x1, z0), (x0, z0 + h)], y0, y1)
    g.box(LAMP, (x0 - 3.0, yc, z0 + h * 0.52), (10, (y1 - y0) - 70.0, h * 0.7), glow=GL)
    sl = h / (x1 - x0)
    tilt_box(g, GLASS, (x0 - 12.0, yc, z0 + h + 24.0), (x1 + 20.0, yc, z0 - 20.0 * sl + 12.0), (y1 - y0) + 50.0, 20.0)
    tilt_box(g, WHITE, (x0 - 12.0, y0 - 30.0, z0 + h + 26.0), (x1 + 20.0, y0 - 30.0, z0 - 20.0 * sl + 14.0), 12.0, 30.0)


def door(g, x, yf, w=90.0, h=200.0, col=YEL, fan=True, step=True):
    g.box(col, (x, yf - 4.0, h / 2.0), (w, 12, h))
    if fan:
        disc_y(g, WHITE, x, yf - 9.0, h, w / 2.0, 10.0)
    if step:
        g.box(WHITE, (x, yf - 30.0, 6), (w + 40, 60, 12))


def awning(g, x0, x1, yf, z, depth=110.0, drop=45.0, cols=(YEL, WHITE), n=None):
    """A striped awning hung on the wall at yf, falling towards the road."""
    n = n or max(2, int(round((x1 - x0) / 60.0)))
    w = (x1 - x0) / float(n)
    for k in range(n):
        xc = x0 + (k + 0.5) * w
        tilt_box(g, cols[k % 2], (xc, yf, z), (xc, yf - depth, z - drop), w, 8.0)


def chimneys(g, x, y, z0, h, col=BRICK):
    """The town's paired chimneys: two brick flues under one cream cap, a pot on each."""
    for k in (-1, 1):
        g.box(col, (x + k * 36, y, z0 + h / 2.0), (52, 56, h))
        g.cyl(CREAM, (x + k * 36, y, z0 + h + 14), 15, 34, steps=6)
    g.box(CREAM, (x, y, z0 + h + 7), (140, 72, 14))


def smoke(sc, x, y, z, r=40.0, phase=0.0, bob=-30.0, rate=0.16):
    """A puff that floats up and down, slowly (pale: the kit's steam)."""
    m = sc.part(P.motion(bob=bob, rate=rate, phase=phase))
    m.sphere(STEAM, (x, y, z), r, steps=(4, 6))


def pyramid(g, col, x, y, z, half, h, top=6.0):
    """A square pyramid roof, its sides square to the road, `half` from the middle to an eave."""
    g.cyl(col, (x, y, z), half * 1.4142, h, rot=(0, 45, 0), steps=4, top_r=top * 1.4142)


def planter(g, x, yf, z, w=110.0, cols=(PINK, YEL)):
    """A window box on the wall at yf: a white trough, two rows of flowers (no green on this level)."""
    g.box(WHITE, (x, yf - 16.0, z), (w, 26, 18))
    g.box(cols[0], (x - w * 0.2, yf - 16.0, z + 13), (w * 0.5, 16, 10))
    g.box(cols[1], (x + w * 0.27, yf - 16.0, z + 12), (w * 0.34, 14, 8))


def lantern(g, x, y, z, s=36.0, glow=0.7):
    """A four-sided lantern: a lit box under a little cream roof (never a ball on a post)."""
    g.box(WHITE, (x, y, z - 4), (s + 12, s + 12, 8))
    g.box(LAMP, (x, y, z + s / 2.0), (s, s, s), glow=glow)
    pyramid(g, CREAM, x, y, z + s, s * 0.5 + 10, s * 0.6)


def vane(sc, x, y, z, phase=0.2):
    """A weather vane: an arrow that swings slowly about its mast."""
    g = sc.still()
    g.cyl(STEEL, (x, y, z), 3.5, 60, steps=6)
    v = sc.part(P.motion(swing=28, pivot=(x, y, z), axis=P.UP, rate=0.11, phase=phase))
    v.box(YEL, (x, y, z + 52), (120, 6, 8))
    v.box(YEL, (x - 52, y, z + 52), (30, 6, 34))
    v.box(YEL, (x + 60, y, z + 52), (22, 6, 22), rot=(45, 0, 0))


# ------------------------------------------------------------------------------------------ the buildings
def terrace(sc):
    """BUILDING: a row of four workers' houses, 16 x 7 m, 9.5 m to the ridges: four steep gables to the road, a
    yellow door and two arched windows a house, paired chimneys in the valleys between them, smoke from two."""
    g = sc.still()
    walls(g, BRICK, 0, 0, 1600, 700, -FOOT, 600)
    for k in (0, 2):
        g.box(CREAM, (-600 + 400 * k, 0, 455), (402, 702, 290))          # two of the four are cream upstairs
    band(g, CREAM, 0, 0, 1600, 700, 300, 24)
    band(g, WHITE, 0, 0, 1600, 700, 600, 24, 14)
    lit = ((1, 0), (0, 1), (1, 1), (1, 0))
    for k in range(4):
        hx = -600 + 400 * k
        gable(g, hx, -350, 350, 400, 600, 330, wall=(BRICK if k % 2 == 0 else CREAM), roof=BRICK, over=14.0)
        disc_y(g, LAMP if k != 1 else GLASS, hx, -362, 720, 34, 14, glow=(GL if k != 1 else 0.0))    # the attic's round window
        door(g, hx - 115, -350, col=(YEL if k % 2 == 0 else WHITE))
        arched(g, '-y', hx + 70, -350, 95, 110, 175, lit=bool(lit[k][0]))
        arched(g, '-y', hx + 10, -350, 365, 110, 175, lit=bool(lit[k][1]))
        planter(g, hx + 70, -350, 86, 120, cols=((PINK, YEL) if k % 2 == 0 else (YEL, WHITE)))
    for k in (1, 2, 3):
        chimneys(g, -800 + 400 * k, 110, 600, 360)
    for s in (-1, 1):
        pane(g, '-x' if s < 0 else '+x', -120, s * 800, 190, 90, 130, lit=(s > 0))
        pane(g, '-x' if s < 0 else '+x', 120, s * 800, 450, 90, 130, lit=(s < 0))
    smoke(sc, -436, 110, 1060, 40, 0.1)
    smoke(sc, 364, 110, 1075, 46, 0.6, bob=-36, rate=0.13)


def lodge(sc):
    """BUILDING: the gate lodge, 7 x 5 m, 6.2 m, a gable and a bay window to the road; beside it the striped barrier
    arm, 5.6 m long, that rises and falls over the works road."""
    ox = 65.0                                   # the lodge and its arm together are centred on the origin
    g = sc.still()
    walls(g, BRICK, 250 + ox, 0, 700, 500, -FOOT, 320)
    band(g, CREAM, 250 + ox, 0, 700, 500, 150, 22)
    band(g, WHITE, 250 + ox, 0, 700, 500, 320, 24, 14)
    gable(g, 250 + ox, -250, 250, 700, 320, 300, wall=CREAM, roof=GLASS)
    arched(g, '-y', 250 + ox, -250, 370, 110, 160)
    door(g, 30 + ox, -250)
    g.box(WHITE, (30 + ox, -300, 232), (150, 110, 12))                   # the porch's hood
    g.box(WHITE, (420 + ox, -285, 60), (230, 70, 120))                   # the bay window: plinth, glass, lid
    g.box(LAMP, (420 + ox, -285, 190), (210, 60, 140), glow=GL)
    g.box(WHITE, (420 + ox, -288, 268), (240, 84, 16))
    for dx in (-52, 52):
        g.box(WHITE, (420 + ox + dx, -317, 190), (8, 6, 140))
    chimneys(g, 470 + ox, 120, 380, 300)
    pane(g, '+x', 0, 600 + ox, 190, 100, 130)
    pane(g, '-x', 60, -100 + ox, 190, 100, 130)
    lantern(g, 130 + ox, -290, 250, 26, 0.6)
    # the barrier: a post, a rest, and the arm (drawn half up; it swings to level and to 50 degrees)
    px, py, pz = -170.0 + ox, -310.0, 115.0
    g.box(YEL, (px, py, 20), (34, 34, 220))
    g.box(WHITE, (px, py, 134), (44, 44, 10))
    g.box(WHITE, (px - 560, py, 10), (16, 16, 200))
    g.box(WHITE, (px - 560, py, 114), (16, 46, 10))
    a = sc.part(P.motion(swing=25, pivot=(px, py, pz), axis=90.0, rate=0.1, phase=0.0))
    ca, sa = math.cos(math.radians(25.0)), math.sin(math.radians(25.0))
    for k in range(5):
        p0 = (px - ca * 112 * k, py - 26, pz + sa * 112 * k)
        p1 = (px - ca * 112 * (k + 1), py - 26, pz + sa * 112 * (k + 1))
        tilt_box(a, YEL if k % 2 == 0 else WHITE, p0, p1, 12, 14)
    tilt_box(a, STEEL, (px, py - 26, pz), (px + ca * 70, py - 26, pz - sa * 70), 16, 30)     # the counterweight
    a.cyl(YEL, (px - ca * 300, py - 44, pz + sa * 300), 30, 6, rot=(0, 0, 90), steps=8)        # a blank yellow disc on the arm
    a.cyl(WHITE, (px - ca * 300, py - 48, pz + sa * 300), 17, 6, rot=(0, 0, 90), steps=8)
    smoke(sc, 434 + ox, 120, 780, 34, 0.35)
    planter(g, 420 + ox, -320, 128, 200)
    g.box(WHITE, (230 + ox, -300, 44), (150, 40, 8))                          # the gateman's bench
    for dx in (-60, 60):
        g.box(WHITE, (230 + ox + dx, -300, 20), (8, 36, 40))
    g.box(WHITE, (230 + ox, -284, 70), (150, 8, 40))
    g.box(STEEL, (-60 + ox, -262, 300), (8, 40, 8))                           # the gate bell on its bracket
    bl = sc.part(P.motion(swing=16, pivot=(-60 + ox, -280, 298), axis=0.0, rate=0.5, phase=0.6))
    bl.cyl(YEL, (-60 + ox, -280, 262), 16, 30, steps=6, top_r=8)


def canteen(sc):
    """BUILDING: the works canteen, 18 x 7.5 m, 6.8 m: a long hall under four lit sawtooth lights, a serving hatch
    under a striped awning, a trestle table, a kitchen stack 10.5 m with its steam."""
    g = sc.still()
    walls(g, CREAM, 0, 0, 1800, 750, -FOOT, 420)
    g.box(BRICK, (0, 0, 75), (1812, 762, 150))                           # the brick dado
    band(g, WHITE, 0, 0, 1800, 750, 420, 24, 14)
    for k in range(4):
        tooth(g, -900 + 450 * k, -450 + 450 * k, -375, 375, 420, 260)
        g.cyl(WHITE, (-860 + 450 * k + 130, 0, 590), 16, 46, steps=6)      # a vent on each tooth
    # the serving hatch
    g.box(LAMP, (-520, -378, 215), (380, 10, 150), glow=GL)
    g.box(WHITE, (-520, -398, 134), (420, 50, 12))
    awning(g, -730, -310, -375, 345, n=7)
    for x, z, col in ((-850, 275, YEL), (-782, 275, WHITE), (-816, 205, PINK)):   # the menu: three blank sheets
        g.box(col, (x, -378.5, z), (56, 5, 50))
    door(g, -150, -375, w=160, h=230)
    lantern(g, -150, -410, 330, 28, 0.6)
    for x in (150, 430, 710):
        arched(g, '-y', x, -375, 150, 130, 210, lit=(x != 430))
    for s in (-1, 1):
        pane(g, '-x' if s < 0 else '+x', -150, s * 900, 260, 110, 150, lit=True)
        pane(g, '-x' if s < 0 else '+x', 150, s * 900, 260, 110, 150, lit=(s < 0))
    # a trestle table and two benches on the pavement
    g.box(WHITE, (330, -470, 74), (300, 60, 8))
    for dx in (-120, 120):
        g.box(BRICK, (330 + dx, -470, 35), (10, 50, 70))
    for dy in (-52, 52):
        g.box(BRICK, (330, -470 + dy, 42), (280, 22, 8))
        for dx in (-110, 110):
            g.box(BRICK, (330 + dx, -470 + dy, 19), (8, 18, 38))
    for dx in (0, 34):                                                     # two churns by the door
        g.cyl(STEEL, (-30 + dx, -400, 0), 15, 52, steps=6)
        g.cyl(WHITE, (-30 + dx, -400, 52), 11, 8, steps=6)
    # the kitchen stack
    g.box(BRICK, (790, 215, (1050 - FOOT) / 2.0), (110, 110, 1050 + FOOT))
    band(g, CREAM, 790, 215, 110, 110, 700, 22, 8)
    band(g, CREAM, 790, 215, 110, 110, 1040, 22, 10)
    smoke(sc, 790, 215, 1110, 44, 0.0, bob=-34, rate=0.14)
    smoke(sc, 760, 215, 1195, 56, 0.45, bob=-42, rate=0.11)


def time_office(sc):
    """BUILDING: the time office, a narrow three-storey tower house 5.5 m square, 13.7 m: a bell cote on its steep
    roof (the bell swings), a weather vane, and the lean-to where the shift clocks on."""
    g = sc.still()
    walls(g, BRICK, 0, 0, 550, 550, -FOOT, 310)
    walls(g, CREAM, 0, 0, 550, 550, 310, 620)
    walls(g, BRICK, 0, 0, 550, 550, 620, 930)
    band(g, WHITE, 0, 0, 550, 550, 310, 22)
    band(g, WHITE, 0, 0, 550, 550, 620, 22)
    band(g, WHITE, 0, 0, 550, 550, 930, 26, 18)
    pyramid(g, GLASS, 0, 0, 930, 315, 220, 62)
    g.box(WHITE, (0, 0, 1155), (150, 150, 10))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WHITE, (sx * 55, sy * 55, 1205), (14, 14, 95))
    pyramid(g, CREAM, 0, 0, 1250, 90, 70)
    b = sc.part(P.motion(swing=20, pivot=(0, 0, 1248), axis=0.0, rate=0.45, phase=0.0))
    b.cyl(YEL, (0, 0, 1178), 30, 44, steps=8, top_r=14)
    b.box(STEEL, (0, 0, 1236), (6, 6, 28))
    vane(sc, 0, 0, 1318)
    door(g, -90, -275, w=100, h=210)
    prof(g, WHITE, chevron(-90, 170, 262, 50, 10, 12), -345, -275)          # the door's little hood
    g.box(YEL, (-6, -287, 150), (56, 24, 76))                              # the time recorder on the wall: a box, a blank dial
    disc_y(g, WHITE, -6, -303, 162, 18, 5, steps=6)
    arched(g, '-y', 130, -275, 90, 120, 180)
    arched(g, '-y', 0, -275, 380, 150, 200)
    arched(g, '-y', 0, -275, 690, 150, 200, lit=False)
    for s in (-1, 1):
        pane(g, '-x' if s < 0 else '+x', 0, s * 275, 470, 110, 150, lit=(s < 0))
        pane(g, '-x' if s < 0 else '+x', 0, s * 275, 780, 110, 150, lit=(s > 0))
    lantern(g, -190, -300, 240, 24, 0.6)
    for sx in (-1, 1):                                                      # cream quoins on the brick storeys
        for z in (60, 150, 240, 680, 770, 860):
            g.box(CREAM, (sx * 262, -262, z), (40, 40, 44))
    g.box(WHITE, (255, -283, 465), (8, 8, 930))                              # a rain pipe
    planter(g, 0, -275, 372, 170)
    # the clocking-on lean-to on the right
    walls(g, CREAM, 410, 60, 270, 400, -FOOT, 240)
    g.box(BRICK, (410, 60, 60), (276, 406, 120))
    tilt_box(g, GLASS, (410, -165, 232), (410, 270, 330), 300, 18)
    g.box(LAMP, (410, -142, 170), (170, 8, 110), glow=GL)                   # the open hatch
    g.box(WHITE, (410, -160, 112), (200, 44, 10))
    for k in range(3):                                                      # the queue's posts and rail
        g.box(WHITE, (300 + 110 * k, -330, 45), (10, 10, 90))
    g.box(WHITE, (410, -330, 88), (240, 6, 6))


def fire_station(sc):
    """BUILDING: the works fire station, 12 x 7 m: a hall with two red arched doors under a stepped parapet, a hose
    tower 13.3 m with a louvred top and a bell that swings, a hose reel on the apron."""
    g = sc.still()
    walls(g, BRICK, -150, 0, 900, 700, -FOOT, 520)
    band(g, CREAM, -150, 0, 900, 700, 440, 26)
    g.box(CREAM, (-150, -340, 550), (900, 40, 60))                          # the stepped parapet
    g.box(CREAM, (-150, -340, 600), (420, 40, 50))
    g.box(WHITE, (-150, -340, 640), (200, 44, 34))
    g.box(WHITE, (-150, 0, 527), (928, 728, 14))
    for x in (-340, 40):
        g.box(WHITE, (x, -352, 125), (344, 8, 250))                         # the door's white surround
        disc_y(g, WHITE, x, -356, 250, 172, 8, steps=12)
        g.box(RED, (x, -356, 125), (300, 12, 250))
        disc_y(g, RED, x, -362, 250, 150, 10, steps=12)
        g.box(YEL, (x, -364, 100), (300, 6, 26))
        g.box(WHITE, (x, -364, 190), (6, 6, 380))                           # where the two leaves meet
    for k in range(3):                                                      # three fire buckets on a rail between the doors
        g.cyl(RED, (-176 + 26 * k, -366, 300), 10, 22, steps=6, top_r=12)
    g.box(WHITE, (-150, -362, 330), (130, 8, 8))
    lantern(g, -150, -372, 380, 26, 0.6)
    for s in (-1, 1):
        pane(g, '-x', s * 150, -600, 250, 120, 170, lit=(s > 0))
    for sy in (-1, 1):                                                      # a ladder hung on the end wall
        g.box(WHITE, (-606, 100, 300 + sy * 22), (8, 380, 8))
    for k in range(6):
        g.box(WHITE, (-606, -60 + 64 * k, 300), (6, 6, 44))
    # the hose tower
    walls(g, BRICK, 450, 60, 300, 300, -FOOT, 1100)
    band(g, CREAM, 450, 60, 300, 300, 520, 24)
    band(g, CREAM, 450, 60, 300, 300, 880, 24)
    band(g, WHITE, 450, 60, 300, 300, 1100, 24, 16)
    for k in range(3):
        g.box(WHITE, (450, -94, 935 + 45 * k), (190, 12, 20))               # louvres
        g.box(WHITE, (604, 60, 935 + 45 * k), (12, 190, 20))
    pyramid(g, GLASS, 450, 60, 1100, 180, 180)
    g.cyl(WHITE, (450, 60, 1270), 4, 56, steps=6)
    arched(g, '-y', 450, -90, 150, 110, 190)
    arched(g, '-y', 450, -90, 610, 110, 190, lit=False)
    pane(g, '+x', 60, 600, 700, 100, 140)
    g.box(WHITE, (330, -80, 560), (8, 8, 640))                              # a hose hung to dry
    g.box(STEEL, (450, -120, 880), (10, 60, 10))
    b = sc.part(P.motion(swing=18, pivot=(450, -140, 876), axis=0.0, rate=0.4, phase=0.3))
    b.cyl(YEL, (450, -140, 826), 24, 40, steps=8, top_r=11)
    # the hose reel on the apron: two wheels, a drum of red hose, a handle
    rx, ry = -150.0, -470.0
    for dy in (-40, 34):
        disc_y(g, WHITE, rx, ry + dy, 55, 55, 6, steps=10)
    g.cyl(RED, (rx, ry - 30, 55), 34, 60, rot=(0, 0, 90), steps=8)
    tilt_box(g, WHITE, (rx, ry, 55), (rx + 150, ry, 96), 8, 8)
    g.box(WHITE, (rx + 150, ry, 96), (8, 70, 8))


def pay_office(sc):
    """BUILDING: the pay office and company store, 11 x 7.5 m, 9.9 m: twin gables, a lit bow window and a pay hatch
    under a striped awning, a hanging coin sign that swings, crates by the door, paired chimneys."""
    g = sc.still()
    walls(g, BRICK, 0, 0, 1100, 750, -FOOT, 330)
    walls(g, CREAM, 0, 0, 1100, 750, 330, 640)
    band(g, WHITE, 0, 0, 1100, 750, 330, 22)
    band(g, WHITE, 0, 0, 1100, 750, 640, 24, 14)
    for x in (-275, 275):
        gable(g, x, -375, 375, 550, 640, 330, wall=CREAM, roof=BRICK, over=12.0)
        disc_y(g, LAMP if x < 0 else GLASS, x, -387, 760, 40, 14, glow=(GL if x < 0 else 0.0))
    # the shop front
    g.cyl(WHITE, (-270, -375, 0), 142, 84, steps=10)
    g.cyl(LAMP, (-270, -375, 84), 130, 180, steps=10, glow=GL)
    g.cyl(WHITE, (-270, -375, 264), 146, 16, steps=10)
    door(g, 40, -375, w=100, h=210)
    g.box(LAMP, (330, -378, 190), (130, 10, 110), glow=GL)                  # the pay hatch, barred
    for dx in (-36, 0, 36):
        g.box(WHITE, (330 + dx, -386, 190), (7, 6, 110))
    g.box(WHITE, (330, -398, 128), (170, 46, 10))
    awning(g, -440, 420, -375, 318, n=12)
    for x, on in ((-330, True), (0, False), (330, True)):
        arched(g, '-y', x, -375, 385, 120, 190, lit=on)
    g.box(STEEL, (500, -440, 500), (8, 130, 8))                             # the sign's bracket
    s = sc.part(P.motion(swing=9, pivot=(500, -480, 496), axis=0.0, rate=0.33, phase=0.2))
    s.box(STEEL, (500, -480, 470), (5, 5, 50))
    disc_x(s, YEL, 506, -480, 400, 48, 12, steps=10)
    disc_x(s, WHITE, 509, -480, 400, 30, 18, steps=8)
    chimneys(g, 0, 120, 640, 330)
    smoke(sc, -36, 120, 1070, 40, 0.75)
    g.box(WHITE, (-480, -420, 30), (70, 60, 60))                            # crates: white and blue, still
    g.box(BLUE, (-480, -420, 85), (56, 50, 50))
    g.box(WHITE, (-400, -430, 24), (60, 50, 48))
    for sgn in (-1, 1):
        pane(g, '-x' if sgn < 0 else '+x', -120, sgn * 550, 190, 100, 140, lit=(sgn > 0))
        pane(g, '-x' if sgn < 0 else '+x', 130, sgn * 550, 480, 100, 140, lit=(sgn < 0))


def smithy(sc):
    """BUILDING: the wheelwright's and smith's shop, 13 x 7.5 m: two big sawtooth lights, a yellow sliding door half
    open on a lit shop, a hoist beam with a crate, cart wheels against the wall, a round stack 12.7 m and its smoke."""
    g = sc.still()
    walls(g, BRICK, 0, 0, 1300, 750, -FOOT, 450)
    band(g, CREAM, 0, 0, 1300, 750, 300, 26)
    band(g, WHITE, 0, 0, 1300, 750, 450, 24, 14)
    for k in range(2):
        tooth(g, -650 + 650 * k, 0 + 650 * k, -375, 375, 450, 330)
        g.box(WHITE, (-520 + 650 * k, 0, 752), (60, 200, 30))
    g.box(LAMP, (-250, -378, 165), (360, 10, 330), glow=0.3)                # the open half of the doorway
    g.box(YEL, (-350, -392, 172), (200, 14, 340))                           # the sliding door
    tilt_box(g, WHITE, (-440, -401, 20), (-260, -401, 330), 6, 12)
    tilt_box(g, WHITE, (-260, -401, 20), (-440, -401, 330), 6, 12)
    g.box(WHITE, (-250, -392, 352), (640, 12, 14))                          # its rail
    for x in (300, 520):
        arched(g, '-y', x, -375, 110, 130, 230, lit=(x == 300))
    g.box(STEEL, (120, -455, 430), (16, 160, 18))                           # the hoist beam, its chain, a white crate
    g.box(STEEL, (120, -520, 360), (5, 5, 130))
    g.box(WHITE, (120, -520, 260), (70, 60, 70))
    for k, x in enumerate((30, 75)):                                        # cart wheels leaning on the wall
        disc_y(g, WHITE, x, -392 - 8 * k, 58, 56, 6, steps=10)
        disc_y(g, BRICK, x, -394 - 8 * k, 58, 14, 6, steps=6)
    walls(g, CREAM, -760, 80, 220, 420, -FOOT, 230)                         # the coal shed
    tilt_box(g, GLASS, (-640, 80, 300), (-880, 80, 236), 460, 16)
    g.box(BRICK, (-760, -134, 90), (120, 8, 180))
    for s in (-1, 1):
        pane(g, '+x', s * 150, 650, 240, 110, 160, lit=(s < 0))
    g.cyl(BRICK, (540, 250, -FOOT), 78, 1250 + FOOT, steps=8, top_r=52)     # the round stack
    g.cyl(CREAM, (540, 250, 820), 68, 24, steps=8)
    g.cyl(CREAM, (540, 250, 1236), 60, 30, steps=8)
    smoke(sc, 540, 250, 1320, 46, 0.2, bob=-34, rate=0.15)
    smoke(sc, 575, 250, 1410, 60, 0.7, bob=-44, rate=0.11)
    shoe = arc(-250.0, 470.0, 62.0, -60.0, 240.0, 8) + list(reversed(arc(-250.0, 470.0, 40.0, -60.0, 240.0, 8)))
    prof(g, WHITE, shoe, -392, -382)                                        # a white horseshoe over the door
    g.box(BRICK, (-20, -450, 30), (50, 50, 60))                             # the anvil on its stump
    g.box(STEEL, (-20, -450, 72), (90, 30, 24))
    g.box(STEEL, (36, -450, 76), (30, 16, 12))
    g.cyl(STEEL, (-560, -420, 0), 30, 80, steps=8)                          # the quenching butt
    g.cyl(TEAL, (-560, -420, 80), 26, 4, steps=8)


def institute(sc):
    """BUILDING: the workers' institute, 12 x 7.5 m, 12.6 m: an arcade of three round arches lit from within, three
    arched windows and a balcony above, a mansard roof with a white cupola and a weather vane."""
    g = sc.still()
    walls(g, BRICK, 0, 0, 1200, 750, -FOOT, 20)                             # the plinth
    walls(g, BRICK, 0, 75, 1200, 600, 20, 360)                              # the wall behind the arcade
    pts = [(-600.0, 20.0)]
    for c in (-360.0, 0.0, 360.0):
        pts += [(c - 130, 20.0)] + list(reversed(arc(c, 210.0, 130.0, 0.0, 180.0, 6))) + [(c + 130, 20.0)]
    pts += [(600.0, 20.0), (600.0, 360.0), (-600.0, 360.0)]
    prof(g, CREAM, pts, -375, -335)
    for c in (-360, 0, 360):
        g.box(LAMP, (c, -228, 150), (220, 8, 250), glow=0.3)                 # light behind each arch
        g.box(WHITE, (c, -380, 345), (40, 12, 30))                           # keystones
    for x, w in ((-545, 116), (-180, 106), (180, 106), (545, 116)):
        g.box(WHITE, (x, -355, 34), (w, 56, 28))                             # the piers' bases
    g.box(WHITE, (0, -400, 8), (1240, 60, 16))                               # a step
    walls(g, CREAM, 0, 0, 1200, 750, 360, 700)
    band(g, WHITE, 0, 0, 1200, 750, 372, 24)
    band(g, WHITE, 0, 0, 1200, 750, 700, 26, 16)
    for x in (-360, 0, 360):
        arched(g, '-y', x, -375, 430, 150, 220, lit=(x != 360), sill=(x != 0))
    g.box(WHITE, (0, -420, 416), (260, 90, 14))                             # the balcony
    g.box(WHITE, (0, -461, 470), (260, 8, 8))
    for dx in (-126, 0, 126):
        g.box(WHITE, (dx, -461, 446), (8, 8, 50))
    for s in (-1, 1):
        pane(g, '-x' if s < 0 else '+x', 0, s * 600, 530, 130, 170, lit=(s > 0))
        lantern(g, s * 180, -396, 250, 24, 0.6)
    # the mansard
    tilt_box(g, GLASS, (0, -410, 706), (0, -150, 906), 1250, 20)
    tilt_box(g, GLASS, (0, 410, 706), (0, 150, 906), 1250, 20)
    g.box(BRICK, (0, 0, 728), (1200, 600, 50))
    g.box(BRICK, (0, 0, 800), (1200, 420, 100))
    g.box(BRICK, (0, 0, 870), (1200, 300, 50))
    g.box(WHITE, (0, 0, 904), (1250, 320, 16))
    for x in (-330, 330):                                                   # two lit dormers in the mansard
        g.box(CREAM, (x, -300, 800), (130, 150, 110))
        g.box(LAMP, (x, -377, 800), (84, 8, 70), glow=GL)
        g.box(WHITE, (x, -305, 860), (160, 180, 12))
    for x in (-250, 250):
        planter(g, x, -375, 30, 130, cols=(PINK, WHITE))
    g.cyl(WHITE, (0, 0, 912), 110, 110, steps=8)                            # the cupola
    for a in range(4):
        g.box(LAMP, (0, 0, 968), (226, 40, 70), rot=(0, a * 45.0, 0), glow=GL)
    g.cyl(CREAM, (0, 0, 1022), 132, 140, steps=8, top_r=8)
    vane(sc, 0, 0, 1160, 0.55)


# ------------------------------------------------------------------------------------------ the hero
def clock_gate(sc):
    """HERO: THE CLOCK GATE. A brick gate tower 22 x 14 m, 47.9 m to the whistle: a round arch 14 m high over the
    works road (yellow gates swung open, a lantern hung in the crown), a shaft with tall lit windows, a clock 12 m
    across on each of the four faces (the hands turn slowly, each its own part), a steep cream roof with four lit
    dormers and four pinnacles, and a steam whistle on the top whose three puffs swell and sink."""
    g = sc.still()
    pts = [(-1100.0, -FOOT), (-400.0, -FOOT)] + list(reversed(arc(0.0, 1000.0, 400.0, 0.0, 180.0, 10))) + [(400.0, -FOOT), (1100.0, -FOOT), (1100.0, 1900.0), (-1100.0, 1900.0)]
    prof(g, BRICK, pts, -700, 700)
    for s in (-1, 1):
        g.box(YEL, (s * 750, 0, 60), (720, 1420, 120))                        # the legs' hazard plinths
        g.box(CREAM, (s * 750, 0, 1000), (720, 1420, 40))                     # the springing band
        prof(g, CREAM, [(s * 700, 1900), (s * 1100, 1900), (s * 700, 2160)], -700, 700)   # the shoulders
        for y, face in ((-700, '-y'), (700, '+y')):
            arched(g, face, s * 750, y, 300, 170, 320, lit=(s * y < 0))
            if y < 0:
                arched(g, face, s * 750, y, 1180, 170, 360, lit=(s > 0))
    g.box(WHITE, (0, -712, 1425), (90, 30, 110))                             # the keystone
    for s in (-1, 1):
        lantern(g, s * 470, -740, 700, 60, 0.7)                              # a lantern on each leg, beside the arch
        g.box(STEEL, (s * 470, -716, 690), (14, 40, 14))
    g.box(WHITE, (0, -760, 2890), (760, 130, 20))                            # the balcony under the clock
    g.box(WHITE, (0, -820, 2990), (760, 10, 12))
    for k in range(7):
        g.box(WHITE, (-360 + 120 * k, -820, 2940), (12, 10, 100))
    g.box(CREAM, (0, 0, 1900), (2240, 1440, 50))
    # the gates, swung open towards the road
    for s in (-1, 1):
        hx, hy = s * 400.0, -700.0
        dx, dy = s * 0.342, -0.940
        for k in range(6):
            t = 380.0 * k / 5.0
            g.box(YEL, (hx + dx * t, hy + dy * t, 350), (16, 16, 700))
        yaw = math.degrees(math.atan2(dy, dx))
        for z in (60, 380, 690):
            g.box(YEL, (hx + dx * 190, hy + dy * 190, z), (380, 14, 20), rot=(0, yaw, 0))
    # the lantern in the crown of the arch
    g.box(STEEL, (0, 0, 1290), (10, 10, 220))
    la = sc.part(P.motion(swing=3, pivot=(0, 0, 1400), axis=90.0, rate=0.2, phase=0.1))
    la.box(WHITE, (0, 0, 1182), (110, 110, 12))
    la.box(LAMP, (0, 0, 1120), (90, 90, 110), glow=0.7)
    la.box(WHITE, (0, 0, 1060), (110, 110, 12))
    # the shaft
    walls(g, BRICK, 0, 0, 1400, 1400, 1900, 2900)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(BRICK, (sx * 690, sy * 690, 2400), (160, 160, 1000))      # corner pilasters
    for face in ('-y', '+y', '-x', '+x'):
        wall = -700.0 if face[0] == '-' else 700.0
        for a in (-300, 300):
            arched(g, face, a, wall, 2120, 190, 560, lit=not (face == '+x' and a > 0), steps=10)
    band(g, WHITE, 0, 0, 1500, 1500, 2900, 56, 44)
    # the clock stage
    walls(g, BRICK, 0, 0, 1500, 1500, 2900, 4250)
    band(g, WHITE, 0, 0, 1500, 1500, 4250, 60, 60)
    zc = 3575.0
    for face in ('-y', '+y', '-x', '+x'):
        s = -1.0 if face[0] == '-' else 1.0
        w = s * 750.0
        hm = sc.part(P.motion(swing=175, pivot=((0, w, zc) if face[1] == 'y' else (w, 0, zc)), axis=(90.0 if face[1] == 'y' else 0.0), rate=0.02, phase=0.0))
        hh = sc.part(P.motion(swing=150, pivot=((0, w, zc) if face[1] == 'y' else (w, 0, zc)), axis=(90.0 if face[1] == 'y' else 0.0), rate=0.004, phase=0.3))
        if face[1] == 'y':
            g.cyl(YEL, (0, w + (-30 if s < 0 else 0), zc), 645, 30, rot=(0, 0, 90), steps=16)
            g.cyl(WHITE, (0, w + (-44 if s < 0 else 4), zc), 590, 40, rot=(0, 0, 90), steps=16, glow=0.3)
            g.cyl(YEL, (0, w + (-64 if s < 0 else 44), zc), 44, 20, rot=(0, 0, 90), steps=8)
            for k in range(12):
                th = math.radians(k * 30.0)
                big = (k % 3 == 0)
                rr = 500.0 if big else 520.0
                g.box(GLASS, (rr * math.sin(th), w + s * 48, zc + rr * math.cos(th)), (44 if big else 28, 10, 130 if big else 80), rot=(-k * 30.0, 0, 0))
            hm.box(GLASS, (0, w + s * 54, zc + 215), (46, 10, 530))
            hh.box(GLASS, (0, w + s * 60, zc + 140), (64, 10, 360))
        else:
            g.cyl(YEL, (w + (0 if s < 0 else 30), 0, zc), 645, 30, rot=(90, 0, 0), steps=16)
            g.cyl(WHITE, (w + (-4 if s < 0 else 44), 0, zc), 590, 40, rot=(90, 0, 0), steps=16, glow=0.3)
            g.cyl(YEL, (w + (-44 if s < 0 else 64), 0, zc), 44, 20, rot=(90, 0, 0), steps=8)
            for k in range(12):
                th = math.radians(k * 30.0)
                big = (k % 3 == 0)
                rr = 500.0 if big else 520.0
                g.box(GLASS, (w + s * 48, rr * math.sin(th), zc + rr * math.cos(th)), (10, 44 if big else 28, 130 if big else 80), rot=(0, 0, k * 30.0))
            hm.box(GLASS, (w + s * 54, 0, zc + 215), (10, 46, 530))
            hh.box(GLASS, (w + s * 60, 0, zc + 140), (10, 64, 360))
    # the roof: a steep cream pyramid, four lit dormers, four pinnacles, the whistle
    pyramid(g, CREAM, 0, 0, 4280, 810, 450, 40)
    for sx in (-1, 1):
        for sy in (-1, 1):
            pyramid(g, CREAM, sx * 760, sy * 760, 4280, 62, 210)
    for face in ('-y', '+y', '-x', '+x'):
        s = -1.0 if face[0] == '-' else 1.0
        d = s * 500.0
        if face[1] == 'y':
            g.box(CREAM, (0, d, 4440), (190, 170, 180))
            g.box(LAMP, (0, d + s * 86, 4440), (110, 8, 110), glow=GL)
            g.box(WHITE, (0, d + s * 30, 4538), (230, 240, 16))
        else:
            g.box(CREAM, (d, 0, 4440), (170, 190, 180))
            g.box(LAMP, (d + s * 86, 0, 4440), (8, 110, 110), glow=GL)
            g.box(WHITE, (d + s * 30, 0, 4538), (240, 230, 16))
    g.cyl(WHITE, (0, 0, 4720), 46, 14, steps=8)
    g.cyl(YEL, (0, 0, 4734), 22, 56, steps=8)                               # the steam whistle
    g.cyl(YEL, (0, 0, 4790), 30, 6, steps=8)
    g.box(STEEL, (34, 0, 4760), (40, 6, 6))
    for k, (dx, z, r, bob, ph) in enumerate(((20, 4860, 60, -22, 0.0), (70, 4975, 85, -38, 0.3), (150, 5120, 115, -55, 0.62))):
        m = sc.part(P.motion(bob=bob, rate=0.17 - 0.03 * k, phase=ph))
        m.sphere(STEAM, (dx, 0, z), r, steps=(5, 8))


# ------------------------------------------------------------------------------------------ tall props (24 m or more out)
def lamp(sc):
    """PROP, tall: a cast-iron lamp standard, 5.6 m: three steady cream lanterns, a ladder bar, a stone foot."""
    g = sc.still()
    g.box(PAVE, (0, 0, -55), (64, 64, 190))
    g.box(WHITE, (0, 0, 46), (46, 46, 12))
    g.cyl(STEEL, (0, 0, 40), 10, 430, steps=6, top_r=7)
    g.box(STEEL, (0, 0, 392), (190, 8, 8))
    for s in (-1, 1):
        tilt_box(g, STEEL, (s * 6, 0, 330), (s * 92, 0, 392), 7, 7)
        lantern(g, s * 95, 0, 402, 30, 0.7)
    lantern(g, 0, 0, 476, 40, 0.8)
    g.cyl(STEEL, (0, 0, 540), 3.5, 22, steps=6)


def notice_board(sc):
    """PROP, tall: the works notice board, 4.3 m: two posts on a brick foot, a little roof, six blank coloured
    sheets (no words), a lamp over it."""
    g = sc.still()
    g.box(BRICK, (0, 0, -45), (340, 40, 210))
    g.box(WHITE, (0, 0, 66), (360, 52, 12))
    for s in (-1, 1):
        g.box(WHITE, (s * 150, 0, 235), (14, 14, 330))
    g.box(CREAM, (0, 2, 285), (300, 10, 180))
    cols = (WHITE, YEL, PINK, TEAL, WHITE, YEL)
    for k in range(6):
        x = -100 + 100 * (k % 3)
        z = 325 if k < 3 else 242
        g.box(cols[k], (x + (6 if k % 2 else -4), -6, z), (62 if k != 4 else 78, 5, 64 if k != 1 else 52), rot=((4 if k == 2 else 0), 0, 0))
    tilt_box(g, GLASS, (0, -60, 392), (0, 4, 426), 350, 9)
    tilt_box(g, GLASS, (0, 60, 392), (0, -4, 426), 350, 9)
    g.box(WHITE, (0, 0, 428), (356, 12, 10))
    g.box(LAMP, (0, -40, 388), (120, 12, 10), glow=0.7)


def sentry_box(sc):
    """PROP, tall: a striped sentry box, 3.6 m, and its brazier: a small steady cream glow, a wisp over it."""
    g = sc.still()
    g.box(BRICK, (0, 0, -65), (160, 160, 170))
    g.box(WHITE, (0, 0, 26), (156, 156, 12))
    for k in range(5):
        g.box(YEL if k % 2 == 0 else WHITE, (-52 + 26 * k, 0, 150), (26, 130, 240))
    g.box(GLASS, (0, -63, 135), (80, 8, 190))
    disc_y(g, GLASS, 0, -67, 230, 40, 8)
    pyramid(g, CREAM, 0, 0, 270, 84, 84)
    g.cyl(STEEL, (0, 0, 350), 3.5, 20, steps=6)
    bx = 150.0
    for a in (0, 120, 240):
        g.box(STEEL, (bx + 20 * math.cos(math.radians(a)), -20 + 20 * math.sin(math.radians(a)), 22), (6, 6, 50))
    g.cyl(BLACK, (bx, -20, 44), 28, 40, steps=8, top_r=32)
    g.cyl(CREAM, (bx, -20, 84), 26, 6, steps=8, glow=0.6)
    g.box(RED, (bx - 8, -24, 92), (10, 10, 6))
    g.box(RED, (bx + 10, -14, 92), (8, 8, 6))
    smoke(sc, bx, -20, 150, 16, 0.4, bob=-12, rate=0.3)


def washing(sc):
    """PROP, tall: a washing line, 7 m long, 3.1 m: two posts with cross arms, two lines of washing that sway."""
    g = sc.still()
    for s in (-1, 1):
        g.box(WHITE, (s * 330, 0, 75), (14, 14, 450))
        g.box(WHITE, (s * 330, 0, 292), (10, 120, 10))
        tilt_box(g, WHITE, (s * 330, 0, 200), (s * 270, 0, 286), 8, 8)
    g.box(WHITE, (150, -95, 22), (70, 46, 44))                               # the basket
    g.box(CREAM, (150, -95, 48), (58, 36, 12))
    rows = (
        (-46.0, 0.0, 0.38, ((-250, 86, 110, WHITE), (-150, 64, 78, CREAM), (-60, 70, 96, YEL), (40, 100, 120, WHITE), (150, 60, 70, PINK), (240, 78, 100, CREAM))),
        (46.0, 0.45, 0.31, ((-240, 70, 90, TEAL), (-140, 96, 126, WHITE), (-30, 60, 74, CREAM), (70, 76, 100, WHITE), (170, 90, 60, YEL), (255, 56, 84, WHITE))),
    )
    for y, ph, rate, items in rows:
        g.box(STEEL, (0, y, 290), (660, 4, 4))
        m = sc.part(P.motion(swing=9, pivot=(0, y, 290), axis=0.0, rate=rate, phase=ph))
        for x, w, h, col in items:
            m.box(col, (x, y, 288 - h / 2.0), (w, 5, h))
        m.box(BLUE, (items[1][0] - 14, y, 288 - items[1][2] - 30), (22, 5, 62))       # a pair of overalls' legs
        m.box(BLUE, (items[1][0] + 14, y, 288 - items[1][2] - 30), (22, 5, 62))


def gate_piers(sc):
    """PROP, tall: a works side gate, 9.9 m wide, 6.6 m: two brick piers with stone caps, an iron arch with a lantern
    that hangs steady and swings a little, yellow bar gates standing open, low wing walls."""
    g = sc.still()
    for s in (-1, 1):
        g.box(BRICK, (s * 250, 0, 115), (90, 90, 530))
        g.box(CREAM, (s * 250, 0, 391), (114, 114, 22))
        pyramid(g, WHITE, s * 250, 0, 402, 50, 46)
        g.box(BRICK, (s * 395, 0, -20), (200, 40, 260))
        g.box(WHITE, (s * 395, 0, 116), (210, 52, 12))
        hx, a = s * 205.0, math.radians(70.0)
        dx, dy = -s * math.cos(a), math.sin(a)
        for k in range(5):
            t = 12 + 178.0 * k / 4.0
            g.box(YEL, (hx + dx * t, dy * t, 150), (7, 7, 250))
        yaw = math.degrees(math.atan2(dy, dx))
        for z in (40, 150, 262):
            g.box(YEL, (hx + dx * 100, dy * 100, z), (196, 7, 10), rot=(0, yaw, 0))
    ring = arc(0.0, 402.0, 262.0, 14.0, 166.0, 8) + list(reversed(arc(0.0, 402.0, 244.0, 14.0, 166.0, 8)))
    prof(g, STEEL, ring, -8, 8)
    la = sc.part(P.motion(swing=5, pivot=(0, 0, 646), axis=90.0, rate=0.25, phase=0.2))
    la.box(STEEL, (0, 0, 612), (5, 5, 66))
    la.box(WHITE, (0, 0, 580), (50, 50, 8))
    la.box(LAMP, (0, 0, 556), (38, 38, 40), glow=0.7)
    la.box(WHITE, (0, 0, 532), (50, 50, 8))


def stack(sc):
    """PROP, tall: a works chimney on its own, eight-sided brick, 15 m, two cream bands and a plain crown, a lit
    arched fire door in its square foot, three slow puffs of smoke."""
    g = sc.still()
    g.box(BRICK, (0, 0, -30), (300, 300, 540))
    g.box(CREAM, (0, 0, 250), (320, 320, 22))
    g.cyl(BRICK, (0, 0, 240), 118, 1260, steps=8, top_r=74)
    g.cyl(CREAM, (0, 0, 760), 104, 26, steps=8)
    g.cyl(CREAM, (0, 0, 1466), 84, 34, steps=8)
    arched(g, '-y', 0, -150, 40, 110, 170, sill=False)
    for dx, z, r, bob, ph, rate in ((0, 1560, 52, -26, 0.0, 0.16), (40, 1660, 70, -38, 0.35, 0.13), (100, 1790, 92, -50, 0.7, 0.1)):
        smoke(sc, dx, 0, z, r, ph, bob=bob, rate=rate)


# ------------------------------------------------------------------------------------------ low props (from 17 m)
def time_cards(sc):
    """PROP, LOW (top 1.93 m), 4 m long, 0.76 m deep: a rack of time-card boards under a little roof, a recorder box."""
    g = sc.still()
    for s in (-1, 1):
        g.box(WHITE, (s * 178, 4, 71), (10, 10, 222))
    g.box(CREAM, (0, 6, 124), (350, 8, 116))
    for row in range(3):
        for k in range(9):
            col = YEL if (k * 3 + row * 5) % 7 == 0 else WHITE
            if (k + row * 4) % 11 == 5:
                continue                                                    # a card is out
            g.box(col, (-144 + 36 * k, -1, 90 + 34 * row), (22, 6, 26))
    tilt_box(g, BRICK, (0, -38, 172), (0, 4, 190), 400, 6)
    tilt_box(g, BRICK, (0, 38, 172), (0, -4, 190), 400, 6)
    g.box(WHITE, (0, 0, 190), (404, 10, 6))
    g.box(YEL, (150, -14, 60), (30, 22, 40))
    g.box(WHITE, (150, -27, 64), (16, 4, 16))


def bike_rack(sc):
    """PROP, LOW (top 1.9 m), 8.3 m long, 0.8 m deep: the bicycle shed: four bicycles in a row under a lean-to."""
    g = sc.still()
    cols = (YEL, WHITE, TEAL, CREAM)
    for k in range(4):
        bx = -300.0 + 200.0 * k
        c = cols[k]
        for dx in (-52, 52):
            disc_y(g, STEEL, bx + dx, -3, 33, 33, 6, steps=8)
        bb, seat, head = (bx, 0, 36), (bx - 25, 0, 82), (bx + 38, 0, 84)
        tilt_box(g, c, seat, head, 5, 5)
        tilt_box(g, c, bb, head, 5, 5)
        tilt_box(g, c, bb, seat, 5, 5)
        tilt_box(g, c, head, (bx + 52, 0, 33), 5, 5)
        tilt_box(g, c, seat, (bx - 52, 0, 33), 5, 5)
        g.box(BRICK, (bx - 27, 0, 88), (26, 11, 6))
        g.box(STEEL, (bx + 36, 0, 96), (6, 44, 6))
        g.box(STEEL, (bx + 37, 0, 90), (5, 5, 12))
    g.box(WHITE, (0, 28, 58), (800, 5, 5))
    for k in range(5):
        g.box(WHITE, (-400 + 200 * k, 28, 10), (6, 6, 100))
    for x in (-400, 0, 400):
        g.box(WHITE, (x, 36, 72), (8, 8, 224))
    tilt_box(g, GLASS, (0, 40, 186), (0, -38, 170), 830, 6)
    g.box(WHITE, (0, -38, 168), (830, 5, 8))


def churn_cart(sc):
    """PROP, LOW (top 1.3 m), 2.9 m long, 0.84 m deep: a hand cart of six milk churns."""
    g = sc.still()
    g.box(BRICK, (0, 0, 62), (190, 62, 8))
    for dy in (-33, 33):
        g.box(BRICK, (0, dy, 76), (190, 5, 20))
        disc_y(g, STEEL, 0, dy * 1.15 - 3, 46, 46, 6, steps=10)
        disc_y(g, WHITE, 0, dy * 1.24 - 3, 46, 10, 6, steps=6)
        tilt_box(g, BRICK, (95, dy * 0.8, 64), (175, dy * 0.8, 84), 5, 6)
    g.box(BRICK, (175, 0, 84), (6, 58, 6))
    g.box(BRICK, (-80, 0, 30), (8, 8, 62))
    g.box(BRICK, (80, 0, 30), (8, 8, 62))
    for kx in (-60, 0, 60):
        for dy in (-15, 15):
            g.cyl(STEEL, (kx, dy, 66), 14, 44, steps=6)
            g.cyl(STEEL, (kx, dy, 110), 14, 10, steps=6, top_r=9)
            g.cyl(WHITE, (kx, dy, 120), 11, 5, steps=6)
    g.box(PAVE, (-120, 0, -15), (40, 40, 50))                                # a chock under a wheel end (the footing)
    g.box(PAVE, (120, 0, -15), (40, 40, 50))


def trough(sc):
    """PROP, LOW (top 1.6 m), 4.4 m long, 0.7 m deep: a stone horse trough, a cast-iron hand pump, a bucket, two
    stone bollards."""
    g = sc.still()
    g.box(PAVE, (0, 0, 8), (240, 70, 96))
    g.box(TEAL, (0, 0, 58), (214, 46, 4))
    g.cyl(WHITE, (-160, 0, -40), 10, 190, steps=6)
    g.cyl(WHITE, (-160, 0, 150), 14, 10, steps=6)
    g.box(WHITE, (-136, 0, 112), (50, 8, 8))
    g.box(WHITE, (-114, 0, 102), (8, 8, 16))
    tilt_box(g, STEEL, (-160, 0, 140), (-208, 0, 92), 6, 6)
    g.cyl(STEEL, (152, 0, 0), 15, 30, steps=6, top_r=19)
    for x in (-205, 205):
        g.cyl(PAVE, (x, 0, -40), 16, 112, steps=6)
        g.cyl(WHITE, (x, 0, 62), 18, 8, steps=6)


JOBS = [
    ('SM_S1D1_Terrace', terrace, MOVE), ('SM_S1D1_Lodge', lodge, MOVE), ('SM_S1D1_Canteen', canteen, MOVE),
    ('SM_S1D1_TimeOffice', time_office, MOVE), ('SM_S1D1_FireStation', fire_station, MOVE), ('SM_S1D1_PayOffice', pay_office, MOVE),
    ('SM_S1D1_Smithy', smithy, MOVE), ('SM_S1D1_Institute', institute, MOVE),
    ('SM_S1D1_ClockGate', clock_gate, HERO),
    ('SM_S1D1_Lamp', lamp, STILL), ('SM_S1D1_NoticeBoard', notice_board, STILL), ('SM_S1D1_SentryBox', sentry_box, MOVE),
    ('SM_S1D1_Washing', washing, MOVE), ('SM_S1D1_GatePiers', gate_piers, MOVE), ('SM_S1D1_Stack', stack, MOVE),
    ('SM_S1D1_TimeCards', time_cards, STILL), ('SM_S1D1_BikeRack', bike_rack, STILL), ('SM_S1D1_ChurnCart', churn_cart, STILL),
    ('SM_S1D1_Trough', trough, STILL),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
