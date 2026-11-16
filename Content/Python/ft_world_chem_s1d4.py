"""Chemical Works, Stage 1, builder s1d4: WASTE RUN, the sludge works (8.09 to 9.46 km). /Game/World/ChemicalWorks/SM_S1D4_*.
Run in the editor: import ft_world_chem_s1d4 as K; K.run()

The oldest and worst corner of the works: crooked, patched tin sheds on stilts over the lagoons, lean-to roofs at odd
pitches, bent black stove pipes with pink cowls, funnel hoppers, everything stained magenta from the eaves down.
Palette (the zone plan's five): magenta (the lead: liquor, stains, doors), pink (drips, awnings, cowls), pale pink
(tin walls), black (stove pipes, stilts, tar roofs), steel (hoppers, patches); and from the kit a second tin
(#c391d8) for patch plates, the lit window, steam and unlit glass.

  buildings  SkimmersHut, FilterHouse, DrumWash, RagStore, AssayHut, OldPumpHouse, StiltRow, BoilHouse
  props      Troughs, Duckboards, Punt, SludgePump, DripBarrels (low);  SignPost, Scarecrow, StovePipes, HopperStand,
             Lamp (tall: 24 m and more)
  hero       SludgeFountain: a 52 m black funnel tower, three spouts on crutches pouring pink liquor into three
             stepped basins that overflow one into the next, a watchman's cabin clinging to the shaft

Every mesh is an ft_people.Scene (one material, one section). What moves, moves in the vertex shader: the old pump's
beam, a hoist's bale, washing, a weather vane, hanging boards, a crank, drips, steam, the liquor's froth (it travels
along the basins) and its falls (they shimmer: the builder's travel runs along X only, so nothing travels downwards).
Glow (steady) only on pieces every rule of which stands 31 m and more out. Placed by C:\\ftwork\\s1\\L1_s1d4.inl.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ChemicalWorks'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))   # a piece with no moving part: true bounds
SMALL = dict(grow=(12, 12, 12), grow_down=(12, 12, 0))
MOVES = dict(grow=(40, 40, 40), grow_down=(40, 40, 0))

MAG, PINK, TIN, BLK, STEEL = '#e84aff', '#ff7ad9', '#f3c4dc', '#1c1a26', '#8f9bb3'
TIN2, WIN, STEAM, GLASS, WHITE = '#c391d8', '#ffe9a8', '#d8d0f0', '#233a63', '#eef6ff'
G_WIN, G_LIQ, G_LAMP = 0.8, 0.45, 1.5               # steady light: a window, the liquor, a lamp


# ------------------------------------------------------------------------------------------ the words of the kit
def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def legs(g, x, y, w, d, top, nx=2, foot=150.0, t=22.0, col=BLK, brace=True):
    """Stilts under a deck w x d whose underside is at `top`: nx a row, two rows, a brace across the front."""
    xs = [x - w / 2.0 + t / 2.0 + (w - t) * i / float(nx - 1) for i in range(nx)]
    for px in xs:
        for py in (y - d / 2.0 + t / 2.0, y + d / 2.0 - t / 2.0):
            g.box(col, (px, py, (top - foot) / 2.0), (t, t, top + foot))
    if brace and top > 120:
        py = y - d / 2.0 + t / 2.0
        for i in range(nx - 1):
            a, b = (xs[i], xs[i + 1]) if i % 2 == 0 else (xs[i + 1], xs[i])
            tilt_box(g, col, (a, py, top * 0.12), (b, py, top * 0.88), 10, 10)


def shed(g, x, y, z, w, d, h, rise, wall=TIN, roof=BLK, over=30.0, rib=0, drips=0, patches=()):
    """A tin shed on (x, y, z): w wide, d deep, walls h high, under a lean-to roof that climbs `rise` to the back
    (negative: to the front). rib: corrugation, as that many thick ribs on the front; drips: pink drips hanging from
    the roof's front edge; patches: plates lying on the roof, each (x offset, 0..1 up the roof, w, l, colour).
    Returns the y of its front face."""
    g.box(wall, (x, y, z + h / 2.0), (w, d, h))
    if rib:
        ribs(g, x, y - d / 2.0, z, w - 30, h, rib, TIN2 if wall == TIN else TIN)
    r = abs(rise)
    for k in range(3):
        f = (k + 0.5) / 3.0
        hh = r * (f if rise > 0 else 1.0 - f)
        g.box(wall, (x, y - d / 2.0 + d * f, z + h + hh / 2.0 - 4), (w - 6, d / 3.0, hh + 8))
    zf, zb = (z + h, z + h + r) if rise > 0 else (z + h + r, z + h)
    k = over / max(d, 1.0)
    tilt_box(g, roof, (x, y - d / 2.0 - over, zf - (zb - zf) * k + 16), (x, y + d / 2.0 + over, zb + (zb - zf) * k + 16), w + 2 * over, 14)
    if drips:
        eave_drips(g, x, y - d / 2.0 - over, zf - (zb - zf) * k + 10, w, drips)
    roll = -math.degrees(math.atan2(zb - zf, d))
    for (px, f, pw, pl, pc) in patches:
        g.box(pc, (x + px, y - d / 2.0 + d * f, zf + (zb - zf) * f + 26), (pw, pl, 6), rot=(0, 0, roll))
    return y - d / 2.0


def ribs(g, x, yf, z, w, h, n, col=TIN2):
    """Corrugation shown as a few thick ribs on a front wall."""
    for k in range(n):
        g.box(col, (x - w / 2.0 + w * (k + 0.5) / n, yf - 0.5, z + h / 2.0), (9, 5, h - 8))


def eave_drips(g, x, y, z, w, n, col=PINK):
    """Drips hanging from a roof's front edge (y, z), each its own length."""
    for k in range(n):
        hh = 22 + (k * 37) % 46
        g.box(col, (x - w / 2.0 + w * (k + 0.5) / n + (k * 13) % 17 - 8, y, z - hh / 2.0), (9 + (k * 5) % 8, 6, hh))


def lantern(g, x, y, z, glow=G_LAMP):
    """A jar lantern hanging from (x, y, z): one small pale pink light, steady. Only on pieces 24 m and more out."""
    g.box(BLK, (x, y, z - 8), (5, 5, 16))
    g.box(STEEL, (x, y, z - 18), (22, 22, 5))
    g.box(TIN, (x, y, z - 34), (15, 15, 26), glow=glow)
    g.box(STEEL, (x, y, z - 49), (18, 18, 5))


def handcart(g, x, y, z, load=PINK):
    """A two-wheeled hand cart with its handles down, loaded."""
    g.box(TIN2, (x, y, z + 62), (150, 80, 10))
    for sy in (-1, 1):
        g.box(STEEL, (x, y + sy * 40, z + 80), (150, 6, 30))
    g.cyl(BLK, (x - 30, y - 52, z + 40), 40, 10, rot=(0, 0, 90), steps=8)
    g.cyl(BLK, (x - 30, y + 42, z + 40), 40, 10, rot=(0, 0, 90), steps=8)
    for sy in (-1, 1):
        tilt_box(g, BLK, (x + 70, y + sy * 34, z + 66), (x + 150, y + sy * 34, z + 22), 6, 6)
    g.box(load, (x - 10, y, z + 94), (110, 66, 50))
    g.box(TIN, (x + 10, y - 6, z + 132), (70, 50, 28))


def shed_x(g, x, y, z, w, d, h0, h1, wall=TIN, roof=BLK, over=30.0):
    """The same with the roof pitched ALONG the shed: walls h0 high at its -X end, h1 at its +X end."""
    g.side(wall, _ccw([(-w / 2.0, 0), (w / 2.0, 0), (w / 2.0, h1), (-w / 2.0, h0)]), -d / 2.0, d / 2.0, loc=(x, y, z))
    k = over / max(w, 1.0)
    tilt_box(g, roof, (x - w / 2.0 - over, y, z + h0 - (h1 - h0) * k + 10), (x + w / 2.0 + over, y, z + h1 + (h1 - h0) * k + 10), d + 2 * over, 14)
    return y - d / 2.0


def plate(g, x, yf, z, w, h, col=STEEL):
    """A patch plate on a front wall (yf = the wall's face)."""
    g.box(col, (x, yf - 3, z), (w, 6, h))


def door(g, x, yf, z, w=92.0, h=190.0, col=MAG):
    g.box(BLK, (x, yf - 3, z + h / 2.0 + 4), (w + 16, 6, h + 8))
    g.box(col, (x, yf - 6, z + h / 2.0), (w, 6, h))
    g.box(STEEL, (x + w * 0.32, yf - 9, z + h * 0.5), (8, 5, 18))


def window(g, x, yf, z, w=84.0, h=80.0, lit=False, shutter=None):
    g.box(BLK, (x, yf - 3, z), (w + 16, 6, h + 16))
    g.box(WIN if lit else GLASS, (x, yf - 6, z), (w, 6, h), glow=(G_WIN if lit else 0.0))
    g.box(BLK, (x, yf - 8, z), (6, 5, h))
    if shutter:
        g.box(shutter, (x + w / 2.0 + 26, yf - 6, z - 6), (40, 6, h + 10), rot=(7, 0, 0))


def stain(g, x, yf, top, w, h, col=MAG):
    """A drip stain down a front wall from `top`."""
    g.box(col, (x, yf - 2, top - h / 2.0), (w, 5, h))
    g.box(col, (x + w * 0.2, yf - 2, top - h - 16), (w * 0.4, 5, 34))


def pipe(g, x, y, z, h, bend=60.0, yaw=0.0, r=13.0, cowl=PINK):
    """A bent black stove pipe: up, a knee `bend` cm to one side (yaw: 0 = +X), up again, a pink cowl.
    Returns the top's (x, y, z)."""
    dx, dy = math.cos(math.radians(yaw)) * bend, math.sin(math.radians(yaw)) * bend
    h1 = h * 0.55
    g.cyl(BLK, (x, y, z), r, h1, steps=6)
    rod(g, BLK, (x, y, z + h1 - r), (x + dx, y + dy, z + h1 + bend * 0.6), r, steps=6)
    g.cyl(BLK, (x + dx, y + dy, z + h1 + bend * 0.6 - r), r, h * 0.45, steps=6)
    top = z + h1 + bend * 0.6 - r + h * 0.45
    g.cyl(cowl, (x + dx, y + dy, top), r * 1.9, r * 1.5, steps=6, top_r=r * 0.5)
    return (x + dx, y + dy, top + r * 1.5)


def funnel(g, x, y, z, r0, r1, h, col=STEEL, steps=8, rim=MAG):
    """A funnel hopper: its mouth (r1) is at z + h, its neck below z."""
    g.cyl(col, (x, y, z), r0, h, steps=steps, top_r=r1)
    g.cyl(rim, (x, y, z + h), r1 + 5, 10, steps=steps)
    g.cyl(col, (x, y, z - h * 0.4), r0, h * 0.4, steps=6)


def ladder(g, x, y, z0, z1, w=44.0, col=BLK, lean=0.0, n=None):
    n = n or max(3, int((z1 - z0) / 46.0))
    for sx in (-1, 1):
        tilt_box(g, col, (x + sx * w / 2.0, y, z0), (x + sx * w / 2.0, y - lean, z1), 6, 6)
    for k in range(n):
        f = (k + 0.5) / n
        g.box(col, (x, y - lean * f, z0 + (z1 - z0) * f), (w, 5, 5))


def drum(g, x, y, z, col, r=30.0, h=88.0, band=None):
    g.cyl(col, (x, y, z), r, h, steps=8)
    if band:
        g.cyl(band, (x, y, z + h * 0.6), r + 2.5, 9, steps=8)


def puff(sc, x, y, z, r, phase, rate=0.14, rise=-28.0):
    """A puff of steam that floats up and down on its own clock."""
    s = sc.part(P.motion(bob=rise, rate=rate, phase=phase))
    s.sphere(STEAM, (x, y, z), r, scale=(1.0, 1.0, 0.8), steps=(4, 6))
    s.sphere(STEAM, (x + r * 0.7, y + r * 0.2, z + r * 0.55), r * 0.62, steps=(3, 5))


def drip(sc, x, y, z, phase, size=9.0, fall=-22.0, rate=0.45, col=PINK):
    """A drop hanging under a spout, sliding a hand's breadth down and up."""
    s = sc.part(P.motion(bob=fall, rate=rate, phase=phase))
    s.box(col, (x, y, z), (size, size, size * 1.8))


def awning(g, x, yf, z, w, out=70.0, col=PINK, stripe=TIN):
    tilt_box(g, col, (x, yf, z + 26), (x, yf - out, z), w, 6)
    for sx in (-1, 0, 1):
        g.box(stripe, (x + sx * w * 0.36, yf - out, z - 9), (w * 0.14, 5, 18))


# ------------------------------------------------------------------------------------------ buildings
def skimmers_hut(sc):
    """The skimmers' hut on stilts over a lagoon: a ladder, the punt tied under it, poles and a net on the deck."""
    g = sc.still()
    D = 400.0
    legs(g, 0, 0, 620, 470, D, nx=3)
    g.box(TIN2, (0, 0, D + 9), (660, 510, 18))
    for k in range(7):
        g.box(STEEL if k % 3 == 0 else TIN, (-276 + k * 92, -257, D + 8), (84, 6, 24))
    z = D + 18
    yf = shed(g, -80, 40, z, 430, 350, 232, 112, rib=7, drips=6, patches=((-110, 0.4, 130, 110, TIN2), (90, 0.7, 100, 90, STEEL)))
    door(g, -200, yf, z)
    window(g, -20, yf, z + 140, lit=True, shutter=PINK)
    plate(g, -250, yf, z + 205, 110, 40)
    plate(g, 80, yf, z + 60, 90, 70, TIN2)
    stain(g, -110, yf, z + 232, 26, 120)
    stain(g, 60, yf, z + 232, 18, 70)
    awning(g, -200, yf, z + 200, 130)
    pipe(g, 60, 110, z + 300, 150, bend=46, yaw=20)
    # the deck's own things: a rail, two poles, a net, a bucket
    for x in (-320, -110, 110, 320):
        g.box(BLK, (x, -245, z + 45), (8, 8, 90))
    g.box(BLK, (0, -245, z + 88), (648, 7, 7))
    rod(g, STEEL, (180, -180, z), (240, 150, z + 330), 4, steps=5)
    rod(g, STEEL, (215, -190, z), (270, 120, z + 300), 4, steps=5)
    g.box(PINK, (268, 110, z + 318), (50, 8, 44), rot=(12, 0, 0))
    g.cyl(STEEL, (250, -150, z), 17, 30, steps=6, top_r=20)
    g.cyl(MAG, (250, -150, z + 26), 16, 6, steps=6)
    ladder(g, 255, -262, -100, z + 60, lean=-70)
    # the punt, tied between the stilts
    g.box(TIN, (-60, -60, 34), (330, 104, 22))
    g.box(MAG, (-60, -112, 50), (330, 6, 14))
    g.box(MAG, (-60, -8, 50), (330, 6, 14))
    tilt_box(g, TIN, (-225, -60, 30), (-292, -60, 62), 104, 14)
    tilt_box(g, TIN, (105, -60, 30), (172, -60, 62), 104, 14)
    g.box(STEEL, (-60, -60, 52), (26, 100, 8))
    tilt_box(g, WHITE, (-290, -60, 60), (-300, -200, D - 10), 5, 5)
    g.box(BLK, (-10, 0, -60), (720, 560, 170))            # the lagoon's own black mud bank under it (footing)
    g.box(MAG, (0, -10, 29), (640, 470, 8))               # and the liquor standing round the stilts (unlit)
    # a rack of skimming nets drying, a lantern by the door, a mooring post with a white rope, a cowl vent
    for x in (120, 300):
        g.box(BLK, (x, 150, z + 75), (7, 7, 150))
    g.box(BLK, (210, 150, z + 148), (190, 6, 6))
    g.box(PINK, (170, 150, z + 100), (70, 5, 90))
    g.box(MAG, (256, 150, z + 108), (60, 5, 74))
    g.box(BLK, (-130, yf - 24, z + 226), (6, 50, 6))
    lantern(g, -130, yf - 44, z + 224)
    g.cyl(BLK, (-330, -262, -100), 12, 190, steps=6)
    g.cyl(WHITE, (-330, -262, 66), 15, 10, steps=6)
    g.cyl(STEEL, (-200, 140, z + 320), 16, 30, steps=6)
    g.cyl(PINK, (-200, 140, z + 350), 26, 12, steps=6, top_r=8)
    g.box(STEEL, (120, -200, z + 14), (60, 44, 28))            # a tool box, a coil
    g.cyl(WHITE, (40, -190, z), 24, 10, steps=6)


def filter_house(sc):
    """The filter house: a tall thin shed of three crooked storeys, six funnel hoppers in a row along its wall."""
    g = sc.still()
    legs(g, 0, 20, 440, 380, 200, nx=2, t=26)
    g.box(BLK, (0, 20, 208), (470, 410, 16))
    z = 216.0
    g.box(TIN2, (0, 20, z + 190), (420, 360, 380))
    ribs(g, 0, 20 - 180, z, 390, 380, 6, TIN)
    ribs(g, 26, 26 - 175, z + 384, 370, 356, 6, TIN2)
    g.box(TIN, (26, 26, z + 380 + 180), (400, 350, 360))
    g.box(BLK, (26, 26, z + 384), (424, 374, 12))
    z3 = z + 740
    yf3 = shed_x(g, -14, 22, z3, 384, 336, 300, 470)
    g.box(BLK, (-14, 22, z3 + 4), (408, 360, 12))
    yf = 20 - 180
    door(g, -120, yf, z)
    window(g, 90, yf, z + 250, shutter=PINK)
    window(g, -60, 26 - 175, z + 560, lit=True)
    window(g, 40, yf3, z3 + 190, w=70, h=70)
    plate(g, 130, 26 - 175, z + 470, 120, 80)
    plate(g, -120, yf3, z3 + 110, 90, 120, TIN2)
    plate(g, -150, yf, z + 300, 80, 60)
    # the hoppers: a steel shelf on brackets, six funnels, a drip pipe from each down to the trough at the foot
    sy = yf - 70
    g.box(STEEL, (0, sy, z + 420), (470, 120, 10))
    for x in (-215, 215):
        tilt_box(g, BLK, (x, yf, z + 330), (x, sy - 50, z + 418), 8, 8)
    for k in range(6):
        x = -195 + k * 78
        funnel(g, x, sy, z + 426, 9, 34, 62, steps=6, rim=MAG if k % 2 == 0 else PINK)
        g.box(PINK, (x, sy, z + 250), (7, 7, 300))
        stain(g, x, yf, z + 410, 14, 80 + (k * 37) % 60)
        drip(sc, x, sy, z + 84, phase=k * 0.17)
    g.box(STEEL, (0, sy, z + 40), (500, 96, 44))
    g.box(MAG, (0, sy, z + 60), (480, 76, 8))
    for x in (-230, 230):
        g.box(BLK, (x, sy, (z + 18 - 150) / 2.0), (20, 20, z + 18 + 150))
    ladder(g, 196, yf - 8, z, z + 410, w=40)
    pipe(g, -150, 60, z3 + 330, 240, bend=70, yaw=180)
    pipe(g, 120, 100, z3 + 430, 150, bend=40, yaw=60, r=10)
    puff(sc, -225, 62, z3 + 680, 46, 0.3)
    stain(g, 150, yf3, z3 + 400, 30, 240)
    stain(g, -170, 26 - 175, z + 740, 22, 150)
    eave_drips(g, -14, yf3 - 30, z3 + 300, 380, 6)
    lantern(g, -190, yf - 30, z + 250)
    g.box(BLK, (-190, yf - 16, z + 252), (6, 40, 6))
    drum(g, 250, yf - 60, 0, TIN, band=MAG)
    drum(g, 300, yf - 10, 0, STEEL)
    g.box(PINK, (-250, sy, z + 70), (40, 60, 30))


def drum_wash(sc):
    """The drum-wash: an open lean-to, a rack of upturned drums, the wash trough, brushes on poles."""
    g = sc.still()
    g.box(STEEL, (0, 0, -55), (960, 520, 160))                # the slab (footing), wet
    g.box(MAG, (-120, -150, 27), (420, 150, 5))
    g.box(MAG, (160, -205, 27), (190, 80, 5))
    ribs(g, 0, 218, 20, 880, 320, 12, TIN2)
    eave_drips(g, 0, -250, 290, 940, 9)
    handcart(g, -230, -190, 22, load=TIN)
    lantern(g, 150, -190, 296)
    g.box(TIN, (0, 225, 180), (920, 14, 330))                 # the back wall
    for x in (-380, -120, 150, 400):
        plate(g, x, 218, 120 + (x % 90), 130, 90, TIN2 if x % 20 else STEEL)
    for x in (-440, -150, 150, 440):
        g.box(BLK, (x, -200, 150), (18, 18, 300))
        g.box(BLK, (x, 215, 195), (18, 18, 390))
    tilt_box(g, BLK, (0, -250, 296), (0, 262, 400), 980, 14)   # the roof drops to the front
    for x in (-300, 60, 330):
        g.box(TIN2, (x, -80, 342), (150, 120, 6), rot=(0, x % 9 - 4, -11.5))
        stain(g, x, -246, 300, 22, 60 + x % 40, PINK)
    # the rack: two rails, six drums upside down, draining
    for yy in (60, 120):
        g.box(STEEL, (-170, yy, 96), (520, 9, 9))
    for x in (-420, -170, 80):
        g.box(BLK, (x, 90, 48), (12, 80, 96))
    cols = (PINK, STEEL, TIN, MAG, TIN, PINK)
    for k in range(6):
        x = -385 + k * 86
        drum(g, x, 90, 102, cols[k], band=BLK if k % 2 else None)
        drip(sc, x, 90, 88, phase=k * 0.21, size=7, fall=-16)
    # the trough, the hose reel, the brushes
    g.box(STEEL, (270, 60, 52), (300, 130, 64))
    g.box(PINK, (270, 60, 82), (280, 110, 6))
    for x in (140, 400):
        g.box(BLK, (x, 60, 10), (16, 120, 20))
    g.cyl(BLK, (420, -120, 96), 46, 22, rot=(0, 0, 90), steps=8)
    g.cyl(PINK, (420, -128, 96), 30, 38, rot=(0, 0, 90), steps=8)
    g.box(BLK, (420, -110, 48), (12, 12, 96))
    for k, x in enumerate((180, 215, 250)):
        rod(g, STEEL, (x, 200, 20), (x + 14, 214, 250 + k * 22), 4, steps=5)
        g.box(PINK if k % 2 else WHITE, (x + 14, 212, 262 + k * 22), (34, 12, 22))
    drum(g, -380, -150, 18, TIN, band=MAG)
    g.cyl(STEEL, (-300, -120, 18), 30, 88, rot=(84, 30, 0), steps=8)
    pipe(g, -400, 150, 380, 170, bend=50, yaw=0)
    puff(sc, -345, 152, 640, 40, 0.6)


def rag_store(sc):
    """The rag store: a leaning two-storey barn, its upper floor overhanging, a hoist beam with a bale on a rope,
    rags hanging from the loading door, a vane on the ridge."""
    g = sc.still()
    legs(g, 0, 0, 600, 500, 160, nx=3, t=26, brace=False)
    g.box(BLK, (0, 0, 168), (640, 540, 16))
    z = 176.0
    g.box(TIN2, (0, 10, z + 180), (600, 500, 360), rot=(1.5, 0, 0))
    ribs(g, 0, 10 - 250, z + 10, 560, 340, 8, TIN)
    z2 = z + 360
    g.box(BLK, (0, -10, z2 + 6), (690, 580, 12))
    yf2 = shed_x(g, 14, -10, z2 + 12, 670, 560, 330, 180, wall=TIN)
    yf = 10 - 250
    # the barn doors: black, cross-braced in pink
    for sx in (-1, 1):
        g.box(BLK, (sx * 74, yf - 3, z + 125), (140, 6, 250))
        tilt_box(g, PINK, (sx * 74 - 60, yf - 7, z + 12), (sx * 74 + 60, yf - 7, z + 238), 5, 9)
        tilt_box(g, PINK, (sx * 74 + 60, yf - 7, z + 12), (sx * 74 - 60, yf - 7, z + 238), 5, 9)
    plate(g, -230, yf, z + 250, 110, 130)
    plate(g, 235, yf, z + 110, 100, 90, TIN)
    window(g, 232, yf, z + 270, w=60, h=60)
    # the loading door above, rags hung out of it, the hoist beam
    g.box(BLK, (-120, yf2 - 2, z2 + 122), (156, 6, 200))
    g.box(MAG, (-120, yf2 - 5, z2 + 118), (140, 6, 190))
    cols = (PINK, TIN, MAG, WHITE, PINK)
    for k in range(5):
        r = sc.part(P.motion(swing=5 + k, pivot=(-176 + k * 28, yf2 - 12, z2 + 26), axis=0.0, rate=0.22, phase=k * 0.23))
        r.box(cols[k], (-176 + k * 28, yf2 - 12, z2 - 22 - (k * 13) % 30), (22, 5, 96 + (k * 13) % 30))
    g.box(BLK, (-120, yf2 - 130, z2 + 262), (26, 300, 26))
    tilt_box(g, BLK, (-120, yf2, z2 + 190), (-120, yf2 - 200, z2 + 252), 10, 10)
    b = sc.part(P.motion(swing=5, pivot=(-120, yf2 - 250, z2 + 250), axis=90.0, rate=0.16, phase=0.4))
    b.box(WHITE, (-120, yf2 - 250, z2 + 110), (5, 5, 280))
    b.box(PINK, (-120, yf2 - 250, z2 - 76), (110, 90, 100))
    b.box(STEEL, (-120, yf2 - 250, z2 - 76), (116, 20, 106))
    b.box(MAG, (-88, yf2 - 250, z2 - 20), (30, 60, 14))
    window(g, 180, yf2, z2 + 130, w=90, h=70, lit=True, shutter=PINK)
    plate(g, 60, yf2, z2 + 212, 150, 50)
    plate(g, 250, yf2, z2 + 40, 110, 60, TIN2)
    stain(g, 30, yf2, z2 + 200, 30, 170)
    stain(g, 280, yf2, z2 + 180, 20, 90)
    stain(g, -280, yf, z + 350, 26, 200)
    # the bale pile at the foot, a hand cart of rags
    g.box(PINK, (270, yf - 110, 40), (120, 90, 80))
    g.box(TIN, (286, yf - 96, 112), (104, 80, 64))
    g.box(STEEL, (270, yf - 110, 40), (126, 20, 86))
    pipe(g, 250, 120, z2 + 230, 200, bend=64, yaw=0)
    ribs(g, 150, yf2, z2 + 12, 330, 190, 5, TIN2)
    eave_drips(g, 14, yf2 - 30, z2 + 12 + 250, 640, 8)
    handcart(g, -250, yf - 150, 0)
    for k, c in enumerate((PINK, TIN, MAG)):                       # bales stacked by the door
        g.box(c, (330 - k * 12, yf - 40 - k * 6, 40 + k * 78), (110 - k * 8, 84, 76))
        g.box(STEEL, (330 - k * 12, yf - 40 - k * 6, 40 + k * 78), (116 - k * 8, 18, 80))
    g.box(BLK, (-10, yf - 30, z + 300), (6, 60, 6))
    lantern(g, -10, yf - 56, z + 298)
    for x in (-300, -40, 220):                                     # patch plates on the long roof
        g.box(TIN2 if x % 3 else STEEL, (x, -150 + x % 70, z2 + 12 + 330 - 150 * (x + 321) / 670.0 + 22), (140, 120, 6), rot=(-12.6, 0, 0))
    # the vane on the high end of the ridge: a rag pennant on a pole, turning
    vx, vz = -300, z2 + 12 + 330
    g.cyl(BLK, (vx, -10, vz), 5, 150, steps=5)
    v = sc.part(P.motion(swing=28, pivot=(vx, -10, vz + 120), axis=P.UP, rate=0.12, phase=0.1))
    v.box(PINK, (vx + 46, -10, vz + 124), (84, 5, 34))
    v.box(MAG, (vx + 96, -10, vz + 124), (20, 5, 50))
    v.box(STEEL, (vx - 26, -10, vz + 124), (40, 5, 10))


def assay_hut(sc):
    """The assay hut: a tin cabin on four stumps, a crooked chimney, ONE lit window under a pink awning, a shelf of
    sample flasks, a hanging board."""
    g = sc.still()
    legs(g, 0, 20, 420, 340, 150, nx=2, t=30, brace=False)
    g.box(STEEL, (0, 0, 158), (470, 420, 16))
    z = 166.0
    yf = shed(g, 0, 30, z, 410, 330, 250, 96, rib=7, drips=6, patches=((-90, 0.35, 120, 100, STEEL), (110, 0.6, 90, 120, TIN2)))
    door(g, -120, yf, z, col=MAG)
    window(g, 70, yf, z + 150, w=120, h=90, lit=True)
    awning(g, 70, yf, z + 222, 170, out=80)
    g.box(STEEL, (70, yf - 26, z + 88), (160, 50, 7))             # the shelf under the window
    for k, c in enumerate((MAG, WHITE, PINK, TIN2, MAG)):
        g.cyl(c, (10 + k * 30, yf - 30, z + 92), 8, 18 + (k * 7) % 12, steps=5, top_r=4)
    plate(g, -160, yf, z + 222, 90, 40)
    plate(g, 160, yf, z + 40, 80, 60, TIN2)
    stain(g, -40, yf, z + 250, 20, 110)
    stain(g, 180, yf, z + 250, 14, 60)
    # steps up to the door
    for k in range(3):
        g.box(TIN2, (-120, yf - 30 - k * 34, z - 30 - k * 46), (120, 36, 14))
        g.box(BLK, (-120, yf - 30 - k * 34, (z - 37 - k * 46 - 100) / 2.0), (100, 12, z - 37 - k * 46 + 100))
    # the crooked chimney: three knees
    a = pipe(g, 120, 90, z + 300, 150, bend=44, yaw=200)
    b = pipe(g, a[0], a[1], a[2] - 30, 130, bend=40, yaw=10, cowl=MAG)
    puff(sc, b[0] + 40, b[1], b[2] + 70, 38, 0.15)
    # a board on a bracket, blank, creaking
    g.box(BLK, (-196, yf - 50, z + 236), (8, 110, 8))
    s = sc.part(P.motion(swing=4, pivot=(-196, yf - 80, z + 232), axis=0.0, rate=0.3, phase=0.2))
    s.box(TIN2, (-196, yf - 80, z + 190), (6, 70, 70))
    s.box(MAG, (-200, yf - 80, z + 190), (5, 40, 40))
    # the rain butt and its spout
    drum(g, 240, yf - 20, 0, STEEL, r=34, h=104, band=BLK)
    g.cyl(PINK, (240, yf - 20, 100), 30, 5, steps=8)
    rod(g, BLK, (212, yf + 4, z + 240), (240, yf - 20, 150), 6, steps=5)
    g.box(BLK, (240, yf - 20, -60), (90, 90, 120))
    # a lantern over the door, the assayer's bench with a balance and three flasks, a crate of samples
    g.box(BLK, (-120, yf - 26, z + 216), (6, 50, 6))
    lantern(g, -120, yf - 46, z + 214)
    g.box(TIN2, (0, yf - 130, 84), (190, 60, 8))
    for x in (-84, 84):
        g.box(BLK, (x, yf - 130, -10), (10, 50, 180))
    g.box(STEEL, (-40, yf - 130, 104), (8, 8, 34))
    g.box(STEEL, (-40, yf - 130, 122), (70, 6, 5))
    for sx in (-1, 1):
        g.cyl(TIN, (-40 + sx * 30, yf - 130, 104), 12, 4, steps=6)
    for k, c in enumerate((MAG, PINK, WHITE)):
        g.cyl(c, (30 + k * 24, yf - 130, 88), 9, 22, steps=5, top_r=4)
    g.box(TIN, (150, yf - 110, 30), (70, 56, 60))
    g.box(STEEL, (150, yf - 110, 30), (76, 14, 64))


def old_pump_house(sc):
    """The old pump house: half sunk and tilted, its walking beam still rocking slowly over the well."""
    g = sc.still()
    yf_l = 60 - 200
    g.box(BLK, (0, 30, -75), (760, 560, 200))                     # the sump it sank into (footing)
    g.box(MAG, (0, 10, 28), (700, 500, 6))
    g.box(TIN2, (-110, 60, 60), (520, 400, 360), rot=(4, 0, 0))
    for k in range(7):
        g.box(TIN, (-330 + k * 74, 60 - 200 - 0.5, 130 + (k - 3) * 5), (9, 5, 200), rot=(4, 0, 0))
    eave_drips(g, -110, -178, 222, 540, 7)
    # the flywheel by the frame, turning to and fro with the beam; a belt to the house; a valve; sunk fence posts
    fw = sc.part(P.motion(swing=40, pivot=(230, -96, 150), axis=90.0, rate=0.13, phase=0.25))
    fw.cyl(TIN, (230, -110, 150), 110, 12, rot=(0, 0, 90), steps=10)
    fw.cyl(MAG, (230, -116, 150), 26, 24, rot=(0, 0, 90), steps=6)
    for k in range(3):
        fw.box(BLK, (230, -113, 150), (200, 5, 14), rot=(k * 60, 0, 0))
    g.box(BLK, (230, -84, 75), (30, 30, 170))
    tilt_box(g, BLK, (120, -104, 240), (-20, -104, 180), 12, 6)
    rod(g, STEEL, (420, -210, 60), (330, -210, 60), 12, steps=6)
    g.cyl(WHITE, (372, -224, 60), 26, 6, rot=(0, 0, 90), steps=8)
    for k, x in enumerate((-340, -250, 620)):
        tilt_box(g, BLK, (x, -230, -100), (x + 14 - k * 12, -236, 120 - k * 18), 12, 12)
    g.box(STEEL, (-295, -232, 80), (100, 6, 12), rot=(-6, 0, 0))
    lantern(g, -200, yf_l - 40, 222)
    g.box(BLK, (-200, yf_l - 20, 224), (6, 46, 6))
    tilt_box(g, BLK, (-110, -175, 222), (-110, 295, 318), 590, 14)
    yf = 60 - 200
    g.box(BLK, (-200, yf - 2, 90), (110, 6, 190), rot=(4, 0, 0))
    g.box(MAG, (-200, yf - 5, 86), (94, 6, 180), rot=(4, 0, 0))
    window(g, -10, yf, 150, w=90, h=70)
    for k in (-1, 1):                                             # boarded up
        tilt_box(g, STEEL, (-60, yf - 9, 150 - k * 30), (40, yf - 9, 150 + k * 30), 5, 12)
    plate(g, -300, yf, 170, 90, 80)
    plate(g, 80, yf, 60, 100, 60, TIN)
    stain(g, -100, yf, 225, 26, 120)
    stain(g, 100, yf, 215, 18, 70)
    pipe(g, -280, 150, 270, 190, bend=56, yaw=180)
    # the A-frame and the beam
    for sy in (-1, 1):
        tilt_box(g, STEEL, (170, -40 + sy * 90, 0), (230, -40 + sy * 14, 470), 16, 16)
        tilt_box(g, STEEL, (290, -40 + sy * 90, 0), (230, -40 + sy * 14, 470), 16, 16)
    g.box(BLK, (230, -40, 474), (40, 60, 20))
    bm = sc.part(P.motion(swing=8, pivot=(230, -40, 490), axis=90.0, rate=0.13, phase=0.0))
    bm.box(BLK, (230, -40, 496), (620, 30, 34))
    bm.box(PINK, (-64, -40, 470), (70, 60, 90))                   # the counterweight
    bm.box(STEEL, (-64, -40, 470), (76, 20, 96))
    bm.box(STEEL, (520, -40, 350), (9, 9, 290))                   # the pump rod
    bm.box(BLK, (520, -40, 500), (56, 40, 46))
    # the well head and its gush
    g.cyl(STEEL, (520, -40, -60), 62, 290, steps=8)
    g.cyl(BLK, (520, -40, 226), 68, 14, steps=8)
    rod(g, STEEL, (520, -90, 170), (520, -190, 150), 16, steps=6)
    g.box(PINK, (520, -196, 96), (22, 14, 110))
    g.box(STEEL, (520, -210, 30), (150, 90, 56))
    g.box(MAG, (520, -210, 56), (132, 72, 6))
    drip(sc, 520, -205, 150, 0.3, size=12, fall=-30, rate=0.5)


def stilt_row(sc):
    """Three tiny crooked sheds on one deck over the water, a washing line across their fronts."""
    g = sc.still()
    D = 310.0
    legs(g, 0, 0, 1240, 380, D, nx=4)
    g.box(TIN2, (0, 0, D + 8), (1290, 430, 16))
    for k in range(10):
        g.box(STEEL if k % 4 == 1 else TIN, (-580 + k * 129, -216, D + 7), (118, 6, 22))
    z = D + 16
    ya = shed(g, -430, 40, z, 310, 300, 205, 84, rib=5, drips=4, patches=((40, 0.5, 100, 90, TIN2),))
    yb = shed(g, -40, 30, z, 350, 320, 262, -74, wall=TIN2, rib=6, drips=5, patches=((-60, 0.4, 110, 100, STEEL),))
    yc = shed(g, 400, 46, z, 290, 290, 184, 100, rib=5, drips=4, patches=((-30, 0.6, 90, 100, STEEL),))
    door(g, -470, ya, z, w=80, h=170)
    door(g, -90, yb, z, w=84, h=180, col=PINK)
    door(g, 430, yc, z, w=78, h=160, col=BLK)
    window(g, -350, ya, z + 120, w=60, h=60, lit=True)
    window(g, 50, yb, z + 160, w=70, h=70, shutter=MAG)
    window(g, 330, yc, z + 110, w=54, h=54)
    plate(g, -520, ya, z + 190, 80, 30)
    plate(g, 80, yb, z + 60, 90, 60)
    plate(g, 480, yc, z + 150, 70, 50, TIN2)
    stain(g, -400, ya, z + 205, 18, 80)
    stain(g, -150, yb, z + 262, 26, 130)
    stain(g, 370, yc, z + 184, 16, 70)
    pipe(g, -360, 90, z + 260, 130, bend=40, yaw=0)
    pipe(g, 60, 110, z + 250, 170, bend=50, yaw=160)
    pipe(g, 450, 100, z + 250, 120, bend=36, yaw=200, r=10)
    # a plank bridge between the first two and a window box of dead white twigs
    g.box(STEEL, (-232, -60, z + 4), (90, 60, 8))
    g.box(MAG, (50, yb - 14, z + 112), (90, 22, 18))
    for k in range(3):
        g.box(WHITE, (24 + k * 26, yb - 14, z + 142), (5, 5, 46), rot=(0, 0, -12 + k * 12))
    # the washing line: two posts, the line, six things swaying on it
    zl = z + 268
    for x in (-612, 612):
        g.box(BLK, (x, -200, z + 140), (10, 10, 280))
    g.box(WHITE, (0, -200, zl), (1224, 4, 4))
    cols = (PINK, TIN, MAG, WHITE, STEEL, PINK)
    for k in range(6):
        x = -480 + k * 190 + (k % 2) * 30
        w = sc.part(P.motion(swing=9, pivot=(x, -200, zl), axis=0.0, rate=0.24, phase=k * 0.19))
        w.box(cols[k], (x, -200, zl - 44 - (k % 3) * 8), (70 + (k % 2) * 30, 4, 84 + (k % 3) * 16))
    ladder(g, 590, -224, -100, z + 40, lean=-60)
    g.box(BLK, (0, 0, -65), (1340, 470, 180))
    g.box(MAG, (0, -6, 28), (1260, 400, 6))
    lantern(g, -250, -150, z + 250)
    g.box(BLK, (-250, -120, z + 252), (6, 70, 6))
    g.box(BLK, (-250, -90, z + 126), (8, 8, 252))
    drum(g, 230, -150, z, PINK, r=24, h=64, band=STEEL)
    g.box(TIN, (200, -70, z + 20), (60, 50, 40))


def boil_house(sc):
    """The boil house: three sheds stacked crooked, a zigzag stair up its side, a big hopper on the top floor
    feeding a pink pipe down its face, a balcony, steam."""
    g = sc.still()
    legs(g, 0, 10, 540, 460, 180, nx=3, t=28)
    g.box(BLK, (0, 10, 188), (580, 500, 16))
    z1 = 196.0
    g.box(TIN2, (0, 10, z1 + 180), (530, 450, 360))
    ribs(g, 0, 10 - 225, z1, 500, 360, 8, TIN)
    z2 = z1 + 360
    g.box(BLK, (30, 0, z2 + 6), (500, 450, 12))
    g.box(TIN, (36, 0, z2 + 12 + 165), (470, 420, 330))
    ribs(g, 36, 0 - 210, z2 + 12, 440, 330, 7, TIN2)
    z3 = z2 + 342
    g.box(BLK, (-30, 14, z3 + 6), (430, 400, 12))
    yf3 = shed(g, -36, 14, z3 + 12, 390, 360, 290, 160, rib=6, drips=6, patches=((-90, 0.3, 120, 110, TIN2), (80, 0.65, 110, 100, STEEL)))
    yf1, yf2 = 10 - 225, 0 - 210
    door(g, -150, yf1, z1)
    window(g, 80, yf1, z1 + 190, w=110, h=80, lit=True, shutter=PINK)
    window(g, -80, yf2, z2 + 180, w=90, h=90)
    window(g, 130, yf2, z2 + 180, w=70, h=90, lit=True)
    window(g, -100, yf3, z3 + 170, w=80, h=70)
    plate(g, 190, yf1, z1 + 60, 110, 90)
    plate(g, -200, yf1, z1 + 290, 90, 60, TIN)
    plate(g, 210, yf2, z2 + 60, 80, 70, TIN2)
    plate(g, 60, yf3, z3 + 240, 120, 50)
    # the balcony on the second floor: a deck on brackets, a rail, an awning
    g.box(STEEL, (36, yf2 - 50, z2 + 16), (480, 100, 9))
    for x in (-190, 36, 262):
        g.box(BLK, (x, yf2 - 96, z2 + 62), (8, 8, 92))
        tilt_box(g, BLK, (x, yf2, z2 - 60), (x, yf2 - 90, z2 + 10), 7, 7)
    g.box(BLK, (36, yf2 - 96, z2 + 106), (470, 7, 7))
    awning(g, 130, yf2, z2 + 262, 130, out=76)
    # the hopper on the top floor's corner, its pipe down the face to a tub
    hx, hy = 190, yf1 - 14
    funnel(g, hx, hy, z3 + 130, 20, 96, 150, steps=8)
    for sx in (-1, 1):
        tilt_box(g, BLK, (hx + sx * 70, hy, z3 + 250), (hx + sx * 40, yf3 + 4, z3 + 60), 8, 8)
    g.box(PINK, (hx, hy, (z3 + 70 + 70) / 2.0), (14, 14, z3 + 70 - 70))
    for zz in (z1 + 340, z2 + 300, z3 + 30):
        g.box(STEEL, (hx, hy + 16, zz), (40, 44, 10))
    g.box(STEEL, (hx, hy, 44), (130, 110, 60))
    g.box(MAG, (hx, hy, 72), (112, 92, 6))
    g.box(BLK, (hx, hy, -60), (120, 100, 150))
    drip(sc, hx, hy, 92, 0.5, size=12, fall=-18)
    stain(g, hx - 40, yf3, z3 + 130, 34, 200)
    stain(g, -150, yf2, z2 + 330, 24, 150)
    stain(g, 150, yf1, z1 + 360, 20, 110)
    # the zigzag stair on the -X side
    sx = -300
    tilt_box(g, BLK, (sx, yf1 + 30, -80), (sx, 200, z1 + 4), 70, 10)
    g.box(STEEL, (sx, 228, z1 + 6), (80, 70, 9))
    tilt_box(g, BLK, (sx - 20, 220, z1 + 8), (sx - 20, yf2 + 20, z2 + 4), 70, 10)
    g.box(STEEL, (sx - 10, yf2 + 4, z2 + 8), (90, 70, 9))
    tilt_box(g, BLK, (sx + 10, yf2 + 20, z2 + 10), (sx + 10, 190, z3 + 4), 70, 10)
    g.box(STEEL, (sx + 10, 204, z3 + 8), (90, 70, 9))
    for (ya, za, yb, zb) in ((yf1 + 30, -80, 200, z1 + 4), (220, z1 + 8, yf2 + 20, z2 + 4), (yf2 + 20, z2 + 10, 190, z3 + 4)):
        tilt_box(g, STEEL, (sx - 40, ya, za + 90), (sx - 40, yb, zb + 90), 5, 5)
    pipe(g, -120, 110, z3 + 400, 190, bend=60, yaw=180)
    c = pipe(g, 60, 120, z3 + 420, 200, bend=70, yaw=20, r=16, cowl=MAG)
    puff(sc, c[0] + 50, c[1], c[2] + 70, 52, 0.0)
    puff(sc, -230, 110, z3 + 760, 36, 0.55)
    lantern(g, -150, yf1 - 40, z1 + 236)
    g.box(BLK, (-150, yf1 - 20, z1 + 238), (6, 46, 6))
    lantern(g, -100, yf2 - 92, z2 + 250)
    g.box(BLK, (-100, yf2 - 46, z2 + 252), (6, 100, 6))
    drum(g, -40, yf2 - 56, z2 + 20, PINK, r=22, h=60, band=STEEL)      # on the balcony: a keg, a stool, a mop
    g.box(TIN2, (30, yf2 - 60, z2 + 40), (40, 40, 40))
    rod(g, STEEL, (240, yf2 - 80, z2 + 20), (262, yf2 - 10, z2 + 200), 4, steps=5)
    drum(g, -230, yf1 - 70, 0, STEEL, band=MAG)
    drum(g, -170, yf1 - 96, 0, TIN)
    g.box(PINK, (60, yf1 - 60, 30), (90, 70, 60))


# ------------------------------------------------------------------------------------------ props, low
def troughs(sc):
    """LOW: three settling troughs in a row, each a step lower than the last, pink liquor standing in them (unlit),
    a ladle laid across."""
    g = sc.still()
    for k in range(3):
        x, top = -280 + k * 280, 118 - k * 22
        g.box(STEEL, (x, 0, top - 22), (250, 76, 44))
        g.box(MAG if k != 1 else PINK, (x, 0, top - 2), (234, 60, 6))
        for sx in (-1, 1):
            g.box(BLK, (x + sx * 100, 0, (top - 44 - 90) / 2.0), (14, 60, top - 44 + 90))
        if k < 2:
            g.box(STEEL, (x + 138, 0, top - 16), (40, 24, 8))
            g.box(PINK, (x + 152, 0, top - 34), (8, 12, 30))
        plate(g, x - 60 + k * 40, -38, top - 24, 60, 26, TIN2 if k % 2 else TIN)
        stain(g, x + 70, -38, top - 4, 12, 34)
    rod(g, BLK, (-330, -20, 124), (-190, 26, 124), 4, steps=5)
    g.cyl(STEEL, (-176, 30, 112), 16, 20, steps=6, top_r=20)
    g.cyl(TIN, (300, -20, 80), 14, 26, steps=6, top_r=17)
    g.box(BLK, (0, 0, -70), (860, 96, 60))


def duckboards(sc):
    """LOW: a duckboard walkway on stumps, nine crooked boards, a bucket, a coil of rope, a magnet on a string."""
    g = sc.still()
    for k in range(5):
        x = -400 + k * 200
        for sy in (-1, 1):
            g.cyl(BLK, (x, sy * 36, -100), 9, 160, steps=5)
    for sy in (-1, 1):
        g.box(BLK, (0, sy * 36, 62), (880, 9, 9))
    cols = (TIN, TIN2, TIN, STEEL, TIN, TIN2, TIN, TIN, STEEL)
    for k in range(9):
        g.box(cols[k], (-392 + k * 98, 0, 71), (86, 96 + (k * 5) % 12, 8), rot=(0, (k * 7) % 9 - 4, 0))
    g.cyl(STEEL, (-300, -10, 75), 15, 28, steps=6, top_r=18)
    g.cyl(MAG, (-300, -10, 100), 14, 5, steps=6)
    g.cyl(WHITE, (120, 10, 75), 22, 9, steps=6)
    g.cyl(WHITE, (120, 10, 84), 15, 8, steps=6)
    rod(g, STEEL, (330, -30, 76), (368, 36, 150), 4, steps=5)      # a short rod propped on the edge
    g.box(WHITE, (368, 38, 112), (4, 4, 74))
    g.box(MAG, (368, 38, 68), (14, 10, 16))
    g.box(PINK, (-60, 0, 78), (60, 44, 6), rot=(0, 20, 0))


def punt(sc):
    """LOW: a skimming punt pulled up on a mud bank on two rollers, its two poles and a net laid across it."""
    g = sc.still()
    g.box(BLK, (0, 0, -34), (520, 150, 112))
    g.box(TIN2, (0, 0, 24), (470, 130, 8))
    for x in (-110, 120):
        g.cyl(STEEL, (x, -66, 28), 11, 132, rot=(0, 0, 90), steps=6)
    g.box(TIN, (0, 0, 50), (330, 104, 20))
    for sy in (-1, 1):
        g.box(TIN, (0, sy * 52, 74), (330, 8, 34))
        g.box(MAG, (0, sy * 57, 82), (330, 5, 12))
    tilt_box(g, TIN, (-165, 0, 44), (-232, 0, 86), 104, 12)
    tilt_box(g, TIN, (165, 0, 44), (232, 0, 86), 104, 12)
    g.box(STEEL, (-40, 0, 80), (22, 104, 7))
    g.box(STEEL, (70, 0, 80), (22, 104, 7))
    rod(g, STEEL, (-250, -24, 100), (250, 14, 96), 4, steps=5)
    rod(g, BLK, (-226, 20, 98), (236, -14, 104), 4, steps=5)
    g.box(PINK, (246, -14, 104), (40, 44, 6))
    g.cyl(PINK, (130, 10, 62), 15, 26, steps=6, top_r=18)
    stain(g, -90, -57, 76, 16, 30)
    plate(g, 60, -55, 66, 50, 22, STEEL)


def sludge_pump(sc):
    """LOW: a hand-cranked sludge pump on a plank base: its wheel and crank turn to and fro, a bent spout, a bucket."""
    g = sc.still()
    g.box(TIN2, (0, 0, 9), (230, 110, 18))
    g.box(BLK, (0, 0, -50), (200, 90, 100))
    g.cyl(STEEL, (-40, 0, 18), 28, 100, steps=8)
    g.cyl(BLK, (-40, 0, 116), 32, 8, steps=8)
    g.cyl(MAG, (-40, 0, 70), 30, 9, steps=8)
    g.box(BLK, (-40, -22, 108), (16, 30, 16))
    w = sc.part(P.motion(swing=34, pivot=(-40, -40, 108), axis=90.0, rate=0.35, phase=0.0))
    w.cyl(TIN, (-40, -36, 108), 44, 8, rot=(0, 0, 90), steps=8)
    w.cyl(BLK, (-40, -48, 108), 9, 14, rot=(0, 0, 90), steps=6)
    w.box(BLK, (-40, -52, 126), (8, 6, 44))
    w.box(PINK, (-40, -62, 146), (10, 24, 10))
    rod(g, STEEL, (-16, 0, 96), (40, 0, 124), 8, steps=6)
    rod(g, STEEL, (40, 0, 124), (76, 0, 92), 8, steps=6)
    g.cyl(STEEL, (78, 0, 18), 20, 34, steps=6, top_r=24)
    g.cyl(PINK, (78, 0, 48), 22, 5, steps=6)
    drip(sc, 76, 0, 78, 0.2, size=6, fall=-10, rate=0.5)
    g.cyl(BLK, (-100, 20, 18), 26, 10, steps=8)
    g.cyl(BLK, (-100, 20, 28), 19, 9, steps=8)
    g.box(MAG, (60, -30, 20), (70, 36, 5))


def drip_barrels(sc):
    """LOW: leaking barrels: three standing, one on its side oozing pink into a puddle, a plank bench across two."""
    g = sc.still()
    g.box(BLK, (0, 0, -31), (330, 130, 98))
    drum(g, -110, 10, 0, STEEL, band=MAG)
    drum(g, -40, 16, 0, TIN, band=BLK)
    drum(g, 110, 20, 0, PINK, r=28, h=80, band=STEEL)
    rod(g, TIN2, (-28, -30, 32), (60, -30, 32), 30, steps=8)
    g.box(TIN2, (-75, 12, 94), (170, 44, 7))
    g.cyl(WHITE, (-100, 6, 98), 7, 12, steps=5)
    g.cyl(MAG, (-110, 10, 86), 26, 4, steps=8)
    g.box(MAG, (40, -44, 21), (150, 34, 5))
    g.box(MAG, (112, -40, 21), (60, 22, 5))
    stain(g, 112, -9, 60, 10, 40, MAG)
    plate(g, -40, -15, 50, 30, 30, STEEL)
    drip(sc, 110, -10, 16, 0.6, size=6, fall=-8, rate=0.4)


# ------------------------------------------------------------------------------------------ props, tall
def sign_post(sc):
    """TALL: a leaning post with three blank coloured boards (no words), one hanging by a nail and creaking."""
    g = sc.still()
    tilt_box(g, BLK, (-10, 0, -100), (30, 0, 440), 14, 14)
    g.box(STEEL, (-6, 0, 12), (60, 60, 24))
    g.box(MAG, (64, -9, 380), (190, 6, 50), rot=(4, 0, 0))
    tilt_box(g, MAG, (160, -9, 356), (190, -9, 380), 6, 36)
    g.box(TIN, (-50, -9, 310), (170, 6, 46), rot=(-6, 0, 0))
    g.box(PINK, (22, -9, 300), (20, 7, 30))
    s = sc.part(P.motion(swing=5, pivot=(40, -9, 262), axis=90.0, rate=0.28, phase=0.3))
    s.box(PINK, (74, -9, 232), (150, 6, 44), rot=(-16, 0, 0))
    s.box(TIN2, (120, -12, 224), (30, 5, 30), rot=(-16, 0, 0))
    g.box(TIN2, (40, 30, 30), (60, 50, 30))
    g.box(PINK, (60, 34, 54), (40, 36, 18))
    g.cyl(BLK, (26, 0, 440), 12, 8, steps=5)
    g.box(STEEL, (-60, 24, 12), (50, 40, 24))                      # stones and a broken crate at its foot
    g.box(STEEL, (-30, -30, 10), (36, 30, 20))
    g.box(TIN, (100, 10, 22), (56, 50, 44), rot=(0, 20, 8))
    g.box(BLK, (0, -8, 150), (40, 5, 5))                           # a nail with a tin can on a string
    g.box(WHITE, (-16, -9, 132), (4, 4, 34))
    g.cyl(STEEL, (-16, -9, 104), 8, 16, steps=5)


def scarecrow(sc):
    """TALL: a scarecrow in a chemical suit's cast-offs on a pole, arms out. Pale, static, faceless."""
    g = sc.still()
    g.box(BLK, (0, 0, 60), (12, 12, 320))
    g.box(BLK, (0, 0, 236), (236, 10, 10))
    g.box(STEEL, (0, 0, 8), (70, 70, 16))
    g.box(TIN, (0, -4, 200), (66, 34, 96))
    g.box(WHITE, (0, -4, 150), (70, 36, 10))
    for sx in (-1, 1):
        g.box(TIN, (sx * 70, -2, 236), (84, 26, 26))
        g.box(PINK, (sx * 122, -2, 222), (22, 18, 40), rot=(sx * 10, 0, 0))
        g.box(TIN, (sx * 16, -4, 116), (24, 26, 74), rot=(sx * 5, 0, 0))
        g.box(TIN2, (sx * 18, -4, 72), (20, 22, 18))
    g.cyl(WHITE, (0, -2, 252), 26, 46, steps=7)
    g.cyl(WHITE, (0, -2, 298), 26, 12, steps=7, top_r=14)
    g.box(STEEL, (0, -24, 276), (40, 8, 18))
    g.cyl(TIN2, (0, -2, 306), 40, 5, steps=7)
    g.box(PINK, (24, -22, 196), (16, 6, 60), rot=(8, 0, 0))
    g.box(MAG, (-18, -22, 214), (20, 6, 20))


def stove_pipes(sc):
    """TALL: a patched tin boiler on skids with three bent black stove pipes of three heights, steam off two."""
    g = sc.still()
    for sy in (-1, 1):
        g.box(BLK, (0, sy * 50, -40), (250, 18, 120))
    g.box(TIN, (0, 0, 96), (230, 140, 150))
    g.box(BLK, (0, 0, 176), (246, 156, 12))
    plate(g, -50, -70, 110, 70, 80)
    g.box(MAG, (50, -74, 80), (60, 6, 70))
    g.box(BLK, (50, -78, 80), (8, 5, 30))
    plate(g, 80, -70, 150, 50, 30, TIN2)
    stain(g, -90, -70, 170, 16, 90)
    a = pipe(g, -70, 10, 180, 190, bend=40, yaw=180)
    b = pipe(g, 10, -10, 180, 300, bend=56, yaw=40, r=15)
    c = pipe(g, 80, 20, 180, 250, bend=36, yaw=300, cowl=MAG)
    puff(sc, b[0], b[1], b[2] + 50, 34, 0.0)
    puff(sc, a[0] - 20, a[1], a[2] + 40, 24, 0.45)
    g.cyl(STEEL, (c[0], c[1], c[2] - 60), 17, 6, steps=6)


def hopper_stand(sc):
    """TALL: a funnel hopper on four splayed black legs, dripping into a tub of liquor; a ladder, a stain."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, BLK, (sx * 130, sy * 110, -100), (sx * 60, sy * 50, 330), 14, 14)
    g.box(BLK, (0, 0, 322), (150, 130, 12))
    funnel(g, 0, 0, 250, 24, 150, 230, steps=8)
    plate(g, -40, -96, 410, 70, 44, TIN)
    g.box(TIN2, (60, -70, 360), (50, 6, 40), rot=(20, 0, 0))
    g.box(MAG, (10, -120, 440), (28, 6, 70))
    g.box(STEEL, (0, 0, 40), (150, 120, 60))
    g.box(MAG, (0, 0, 68), (134, 104, 6))
    g.box(BLK, (0, 0, -55), (130, 100, 130))
    g.box(PINK, (0, 0, 118), (10, 10, 70))
    drip(sc, 0, 0, 92, 0.1, size=10, fall=-14, rate=0.5)
    ladder(g, 110, -30, 0, 470, w=40, lean=-60)
    g.box(BLK, (0, 60, 520), (8, 8, 90))
    g.box(PINK, (22, 60, 548), (44, 5, 26))


def lamp(sc):
    """TALL: a crooked lamp post with a jar lantern on a bracket: one small pale pink light, steady."""
    g = sc.still()
    tilt_box(g, BLK, (0, 0, -100), (16, 0, 250), 14, 14)
    tilt_box(g, BLK, (16, 0, 244), (-8, 0, 430), 12, 12)
    g.box(STEEL, (4, 0, 10), (56, 56, 20))
    tilt_box(g, BLK, (-8, 0, 420), (-8, -70, 440), 8, 8)
    tilt_box(g, BLK, (-4, 0, 360), (-8, -50, 430), 6, 6)
    g.box(BLK, (-8, -70, 420), (5, 5, 30))
    g.box(STEEL, (-8, -70, 404), (36, 36, 6))
    g.box(TIN, (-8, -70, 384), (24, 24, 34), glow=G_LAMP)
    g.box(STEEL, (-8, -70, 364), (30, 30, 6))
    for sx in (-1, 1):
        g.box(BLK, (-8 + sx * 14, -70 - sx * 14, 384), (4, 4, 36))
    g.box(PINK, (10, -8, 200), (60, 6, 40), rot=(-8, 0, 0))
    g.box(TIN2, (30, 30, 34), (50, 44, 30))
    tilt_box(g, BLK, (-34, -96, 412), (-8, -70, 428), 5, 40)       # a little tin hood over the jar
    tilt_box(g, BLK, (18, -96, 412), (-8, -70, 428), 5, 40)
    for k in range(5):                                             # climbing pegs
        g.box(STEEL, (6 + k * 2, 0, 70 + k * 36), (36, 5, 5))
    drum(g, -44, 16, 0, STEEL, r=22, h=60, band=MAG)
    g.box(BLK, (30, -10, 300), (5, 30, 5))                         # a hook with a pink pail
    g.cyl(PINK, (30, -24, 268), 11, 22, steps=6, top_r=14)
    g.box(WHITE, (30, -24, 294), (4, 4, 12))


# ------------------------------------------------------------------------------------------ the hero
def sludge_fountain(sc):
    """THE SLUDGE FOUNTAIN: a black funnel tower 52 m tall, wider at the top than the foot; three spouts on crutches
    pour pink liquor into three stepped basins on stilts that overflow one into the next towards the road; froth
    drifts along each basin, the falls shimmer, steam stands off the lowest; a watchman's cabin clings to the shaft."""
    g = sc.still()
    TX, TY = 0.0, 520.0
    # ---- the tower
    g.cyl(BLK, (TX, TY, -300), 600, 520, steps=12)
    g.cyl(STEEL, (TX, TY, 220), 560, 40, steps=12)
    g.cyl(BLK, (TX, TY, 260), 430, 2740, steps=12, top_r=560)
    g.cyl(MAG, (TX, TY, 1150), 482, 60, steps=12)
    g.cyl(PINK, (TX, TY, 2200), 532, 60, steps=12)
    g.cyl(BLK, (TX, TY, 3000), 560, 1700, steps=12, top_r=1150)
    g.cyl(STEEL, (TX, TY, 2960), 580, 70, steps=12)
    g.cyl(PINK, (TX, TY, 4700), 1180, 120, steps=12)
    g.cyl(MAG, (TX, TY, 4820), 1090, 30, steps=12, glow=G_LIQ)
    hp = sc.part(P.motion(bob=-30, rate=0.16, phase=0.0))
    hp.cyl(PINK, (TX, TY, 4850), 760, 230, steps=10, top_r=240, glow=G_LIQ)
    hp.sphere(PINK, (TX + 60, TY, 5080), 190, scale=(1.0, 1.0, 0.6), steps=(4, 8), glow=G_LIQ)
    # fix round: pink bands down the road face (-Y) of the shaft and the flare, unlit: the black funnel did not read on the night sky
    for ang in (-126, -108, -90, -72, -54):
        a = math.radians(ang)
        tilt_box(g, PINK, (TX + math.cos(a) * 596, TY + math.sin(a) * 596, 3080), (TX + math.cos(a) * 1146, TY + math.sin(a) * 1146, 4660), 110, 14)
    for ang in (-112, -90, -68):
        a = math.radians(ang)
        tilt_box(g, PINK, (TX + math.cos(a) * 485, TY + math.sin(a) * 485, 1260), (TX + math.cos(a) * 528, TY + math.sin(a) * 528, 2150), 90, 14)
    # patch plates on the shaft and the flare, each turned to its own face
    for (ang, zz, rr, w, h, col) in ((-70, 700, 452, 220, 300, STEEL), (-115, 1650, 500, 260, 240, TIN), (-60, 2500, 540, 200, 320, TIN2),
                                     (-100, 3500, 730, 320, 260, STEEL), (-50, 4100, 940, 300, 300, TIN), (-135, 4300, 1010, 280, 220, TIN2)):
        a = math.radians(ang)
        g.box(col, (TX + math.cos(a) * rr, TY + math.sin(a) * rr, zz), (w, 20, h), rot=(0, ang + 90, 0))
    # ---- the three basins, stepping down towards the road, each on its stilts
    tiers = ((0.0, -950.0, 1500.0, 1700.0), (0.0, -1500.0, 950.0, 2600.0), (0.0, -2100.0, 420.0, 3600.0))
    for i, (bx, by, rim, w) in enumerate(tiers):
        d, hh = 500.0, 170.0
        legs(g, bx, by, w - 120, d - 80, rim - hh, nx=(3, 4, 5)[i], foot=300.0, t=44.0)
        g.box(TIN, (bx, by, rim - hh / 2.0), (w, d, hh))
        g.box(BLK, (bx, by, rim - hh - 8), (w + 30, d + 30, 16))
        g.box(STEEL, (bx, by, rim + 4), (w + 20, d + 20, 10))
        g.box(MAG, (bx, by, rim + 8), (w - 50, d - 50, 8), glow=G_LIQ)
        for k in range(i + 3):
            px = bx - w / 2.0 + w * (k + 0.5) / (i + 3) + 40
            plate(g, px, by - d / 2.0, rim - 70 - (k * 31) % 50, 150, 70, (STEEL, TIN2, STEEL, TIN2, STEEL)[k])
            stain(g, px - 160, by - d / 2.0, rim, 28, 90 + (k * 43) % 70)
        # froth drifting along the basin (it travels the mesh's X and comes round)
        n = i + 3
        for k in range(n):
            fx = -w / 2.0 + 140 + (w - 280) * k / float(n - 1)
            fy = by + (-110 if k % 2 else 90)
            f = sc.part(P.motion(travel=(w - 240, (0.05, -0.04, 0.03)[i]), base=(fx, fy), pivot=(fx, fy, rim + 20)))
            f.box(WHITE if k % 2 else TIN, (fx, fy, rim + 20), (150, 90, 16), glow=0.2)
    # ---- the falls from one basin to the next: strips that shimmer, each on its own clock
    def fall(x, y, z0, z1, w, n, ph):
        for k in range(n):
            s = sc.part(P.motion(bob=-26, rate=0.5, phase=ph + k * 0.31))
            s.box(PINK if k % 2 else MAG, (x - w / 2.0 + w * (k + 0.5) / n, y, (z0 + z1) / 2.0), (w / n - 8, 12, z1 - z0), glow=G_LIQ)
        g.box(STEEL, (x, y + 20, z1 + 6), (w + 40, 60, 12))
    fall(0, -1215, 950, 1500, 900, 4, 0.0)
    fall(-800, -1765, 420, 950, 620, 3, 0.2)
    fall(800, -1765, 420, 950, 620, 3, 0.6)
    fall(-1250, -2365, 30, 420, 420, 2, 0.1)
    fall(0, -2365, 30, 420, 520, 3, 0.4)
    fall(1250, -2365, 30, 420, 420, 2, 0.8)
    g.box(BLK, (0, -2420, -100), (3700, 220, 260))                 # the gutter the last fall lands in
    g.box(MAG, (0, -2420, 34), (3600, 150, 8), glow=G_LIQ)
    # ---- the three spouts: a trough from the tower on a crutch, a column of liquor falling into its basin
    spouts = (((0.0, -110.0, 3420.0), (0.0, -950.0, 3300.0), 1500.0), ((-330.0, 130.0, 2620.0), (-900.0, -1500.0, 2470.0), 950.0),
              ((340.0, 170.0, 1940.0), (1300.0, -2100.0, 1780.0), 420.0))
    for i, (a, b, rim) in enumerate(spouts):
        tilt_box(g, STEEL, a, b, 150, 26)
        tilt_box(g, MAG, (a[0], a[1], a[2] + 16), (b[0], b[1], b[2] + 16), 100, 10, glow=G_LIQ)
        t, cb = (0.75, 955.0) if i == 2 else (0.5, -300.0)     # the third's crutch stands on the second basin's rim
        mx, my, mz = a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t
        tilt_box(g, BLK, (mx - 160, my, cb), (mx, my, mz - 10), 40, 40)
        tilt_box(g, BLK, (mx + 160, my, cb), (mx, my, mz - 10), 40, 40)
        g.box(BLK, (b[0], b[1], b[2] - 6), (200, 60, 50))
        stain(g, a[0], a[1] - 40, a[2] - 20, 120, 500)
        hcol = b[2] - rim
        for k in range(4):
            s = sc.part(P.motion(bob=-40, rate=0.6, phase=i * 0.3 + k * 0.25))
            s.box(PINK if k % 2 else MAG, (b[0], b[1] - 20, rim + hcol * (k + 0.5) / 4.0), (110 - k * 6, 60, hcol / 4.0 + 30), glow=G_LIQ)
        g.sphere(WHITE, (b[0], b[1] - 20, rim + 40), 110, scale=(1.3, 0.9, 0.4), steps=(3, 6))
    # ---- the watchman's cabin on the shaft, 20 m up: a deck on brackets, a tin cabin, a lit window, a long ladder
    cz = 2000.0
    g.box(STEEL, (-640, 300, cz), (420, 380, 14))
    for sy in (-1, 1):
        tilt_box(g, BLK, (-470, 300 + sy * 150, cz - 300), (-820, 300 + sy * 150, cz - 6), 16, 16)
    cf = shed(g, -660, 320, cz + 7, 300, 260, 210, 90)
    window(g, -620, cf, cz + 130, w=90, h=70, lit=True)
    door(g, -740, cf, cz + 7, w=70, h=160, col=PINK)
    pipe(g, -700, 380, cz + 290, 160, bend=50, yaw=180)
    for x in (-840, -640, -440):
        g.box(BLK, (x, 112, cz + 50), (8, 8, 100))
    g.box(BLK, (-640, 112, cz + 98), (410, 7, 7))
    ladder(g, -500, 120, -200, cz, w=60, n=16)
    # ---- steam standing off the lowest basin, and a puff at the rim
    for k, (x, r) in enumerate(((-1400, 150), (-300, 190), (900, 160), (1650, 130))):
        puff(sc, x, -2300, 640 + (k % 2) * 120, r, k * 0.27, rate=0.1, rise=-60)
    puff(sc, 700, 300, 5080, 170, 0.5, rate=0.08, rise=-70)


JOBS = [
    ('SM_S1D4_SkimmersHut', skimmers_hut, STILL),
    ('SM_S1D4_FilterHouse', filter_house, SMALL),
    ('SM_S1D4_DrumWash', drum_wash, SMALL),
    ('SM_S1D4_RagStore', rag_store, MOVES),
    ('SM_S1D4_AssayHut', assay_hut, SMALL),
    ('SM_S1D4_OldPumpHouse', old_pump_house, MOVES),
    ('SM_S1D4_StiltRow', stilt_row, SMALL),
    ('SM_S1D4_BoilHouse', boil_house, SMALL),
    ('SM_S1D4_Troughs', troughs, STILL),
    ('SM_S1D4_Duckboards', duckboards, STILL),
    ('SM_S1D4_Punt', punt, STILL),
    ('SM_S1D4_SludgePump', sludge_pump, dict(grow=(8, 8, 8), grow_down=(8, 8, 0))),
    ('SM_S1D4_DripBarrels', drip_barrels, dict(grow=(4, 4, 4), grow_down=(4, 4, 0))),
    ('SM_S1D4_SignPost', sign_post, SMALL),
    ('SM_S1D4_Scarecrow', scarecrow, STILL),
    ('SM_S1D4_StovePipes', stove_pipes, SMALL),
    ('SM_S1D4_HopperStand', hopper_stand, SMALL),
    ('SM_S1D4_Lamp', lamp, STILL),
    ('SM_S1D4_SludgeFountain', sludge_fountain, dict(far=True, grow=(80, 80, 80), grow_down=(80, 80, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
