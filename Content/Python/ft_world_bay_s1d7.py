"""Bayshore, Stage 1, builder s1d7: district 7 "The Old Harbour" (21.1 km to the end). /Game/World/Bayshore/SM_S1D7_*.
An old fishing quarter, the level's only WOODEN district: narrow leaning timber-framed lofts on stilts with a hoist beam
under the gable, jettied floors, steep roofs that do not match, crooked chimneys, stone quay walls. Wood #9a744a and
#c08a5a, teal doors, shutters and hulls, cream plaster, red and white tiles and awnings, ochre stone.
The hero is THE BIG FISH: a round, smiling, striped fish-shaped market hall on stilts, the door in its mouth.
Placed by the block C:/ftwork/s1/L0_s1d7.inl. Run: import ft_world_bay_s1d7 as K; K.run()
"""
import math
import ft_art
import ft_people as P
from ft_desert import tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/Bayshore'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))            # a piece with no moving part: true bounds
SMALL = dict(grow=(25, 25, 20), grow_down=(25, 25, 0))       # a piece whose moving parts stay close to it
MOVE = dict(grow=(60, 60, 30), grow_down=(60, 60, 0))        # a building with a hook, nets, washing, a vane

WOOD_D, WOOD, TEAL, CREAM, RED, WHITE, OCHRE = '#9a744a', '#c08a5a', '#2fc6c0', '#fff1d6', '#ff4d5e', '#fffaf2', '#e8b26a'
NAVY, STONE, ROPE, NET, LEAF, SKY, YEL, PINK = '#2b2350', '#d6bd8c', '#c9a06a', '#7fe3c4', '#2fbf71', '#8fe3ff', '#ffe14d', '#ff9ec7'
FOOT = 300.0                                    # how far a building's footing and stilts go down (the harbour is hills)


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def side(g, col, pts, y0, y1, glow=0.0):
    """A profile [(x, z)] seen from the road, from y0 to y1 deep."""
    g.side(col, ccw(pts), y0, y1, glow=glow)


def disc_y(g, col, x, y, z, r, t, steps=10, glow=0.0):
    """A disc facing the road: one face at y, t thick towards +Y."""
    g.cyl(col, (x, y, z), r, t, rot=(0, 0, 90), steps=steps, glow=glow)


def frustum_x(g, col, x_hi, r_hi, x_lo, r_lo, y, z, steps=12, glow=0.0):
    """A round band lying along X: radius r_hi at x_hi, r_lo at x_lo (x_hi > x_lo)."""
    g.cyl(col, (x_hi, y, z), r_hi, x_hi - x_lo, rot=(90, 0, 0), steps=steps, glow=glow, top_r=r_lo)


class Bld(object):
    """A building that LEANS: everything still is drawn in a frame tipped by pitch (top towards -X) and roll (top
    towards +Y) about its foot; pt() gives where a point of that frame is in the mesh (a hanging thing hangs plumb
    from there, and a swing's pivot is always in the mesh's space)."""

    def __init__(self, sc, x=0.0, y=0.0, pitch=0.0, roll=0.0):
        self.sc, self.x, self.y, self.pitch, self.roll = sc, x, y, pitch, roll

    def g(self):
        m = self.sc.still()
        m.base = ft_art._xf((self.x, self.y, 0.0), (self.pitch, 0.0, self.roll), (1.0, 1.0, 1.0))
        return m

    def pt(self, p):
        cp, sp = math.cos(math.radians(self.pitch)), math.sin(math.radians(self.pitch))
        cr, sr = math.cos(math.radians(self.roll)), math.sin(math.radians(self.roll))
        px, py, pz = p
        return (self.x + px * cp + py * sr * sp - pz * cr * sp,
                self.y + py * cr + pz * sr,
                px * sp - py * sr * cp + pz * cr * cp)


def roof_gable(g, col, xc, yc, w, d, z0, h, over=35.0, trim=WHITE, t=14.0, wall=None):
    """A steep roof with its ridge along Y: the GABLE faces the road. w, d: the walls it sits on; z0: the eaves."""
    hw = w / 2.0
    ang = math.degrees(math.atan2(h, hw))
    ex, ez = hw + over, z0 - h * over / hw
    s = math.hypot(ex, z0 + h - ez)
    yf = yc - d / 2.0 - over
    for sx in (-1, 1):
        cx, cz = xc + sx * ex / 2.0, (z0 + h + ez) / 2.0
        g.box(col, (cx, yc, cz + t / 2.0), (s, d + 2 * over, t), rot=(-sx * ang, 0, 0))
        g.box(trim, (cx, yf - 2, cz + t / 2.0), (s + 6, 14, t + 12), rot=(-sx * ang, 0, 0))     # the verge board
    g.box(trim, (xc, yc, z0 + h + t), (30, d + 2 * over + 8, 18))                               # the ridge
    if wall:
        side(g, wall, [(xc - hw, z0), (xc + hw, z0), (xc, z0 + h)], yc - d / 2.0, yc + d / 2.0)


def roof_eaves(g, col, xc, yc, w, d, z0, h, over=35.0, trim=WHITE, t=14.0, wall=None):
    """A steep roof with its ridge along X: the EAVES face the road."""
    hd = d / 2.0
    ang = math.degrees(math.atan2(h, hd))
    ey, ez = hd + over, z0 - h * over / hd
    s = math.hypot(ey, z0 + h - ez)
    for sy in (-1, 1):
        g.box(col, (xc, yc + sy * ey / 2.0, (z0 + h + ez) / 2.0 + t / 2.0), (w + 2 * over, s, t), rot=(0, 0, sy * ang))
    g.box(trim, (xc, yc, z0 + h + t), (w + 2 * over + 8, 30, 18))
    if wall:                                                    # the gable ends, stepped in under the slopes
        g.box(wall, (xc, yc, z0 + h * 0.2), (w, d * 0.72, h * 0.4))
        g.box(wall, (xc, yc, z0 + h * 0.58), (w, d * 0.34, h * 0.36))


def stilts(g, xs, ys, z_top, th=34.0, col=WOOD_D, foot=FOOT):
    for x in xs:
        for y in ys:
            g.box(col, (x, y, (z_top - foot) / 2.0), (th, th, z_top + foot))


def window(g, x, yf, z, w=80.0, h=110.0, shut=TEAL):
    """A dark window on a wall whose face is at yf, a shutter either side."""
    g.box(NAVY, (x, yf - 3, z), (w, 10, h))
    if shut:
        for sx in (-1, 1):
            g.box(shut, (x + sx * (w * 0.5 + w * 0.24), yf - 5, z), (w * 0.44, 8, h + 8))


def frame(g, xs, yf, z0, z1, rails=(), diag=(), col=WOOD_D):
    """Timber framing on a wall face at yf: posts at xs, rails at heights, diagonals [(x0, z0, x1, z1)]."""
    for x in xs:
        g.box(col, (x, yf - 4, (z0 + z1) / 2.0), (16, 12, z1 - z0))
    for z in rails:
        g.box(col, ((xs[0] + xs[-1]) / 2.0, yf - 4, z), (xs[-1] - xs[0] + 16, 12, 14))
    for (xa, za, xb, zb) in diag:
        tilt_box(g, col, (xa, yf - 4, za), (xb, yf - 4, zb), 12, 12)


def awning(g, x0, x1, yw, z, depth=110.0, drop=45.0, cols=(RED, WHITE)):
    """A striped awning hung on the wall at yw, falling towards the road."""
    n = max(2, int(round((x1 - x0) / 62.0)))
    w = (x1 - x0) / float(n)
    ang = math.degrees(math.atan2(drop, depth))
    s = math.hypot(depth, drop)
    for k in range(n):
        g.box(cols[k % 2], (x0 + (k + 0.5) * w, yw - depth / 2.0, z - drop / 2.0), (w, s, 8), rot=(0, 0, -ang))


def chimney(g, x, y, z0, h, dx=0.0, dy=0.0, col=OCHRE, w=46.0):
    """A crooked chimney: it leans dx, dy over its height; a white band and a red pot on top."""
    tilt_box(g, col, (x, y, z0), (x + dx, y + dy, z0 + h), w, w)
    g.box(WHITE, (x + dx, y + dy, z0 + h), (w + 14, w + 14, 14))
    g.cyl(RED, (x + dx, y + dy, z0 + h + 6), 13, 30, steps=6, top_r=10)


def pot(g, x, y, z):
    """A geranium in a clay pot."""
    g.box(OCHRE, (x, y, z + 10), (24, 24, 20))
    g.box(LEAF, (x, y, z + 28), (34, 30, 18))
    g.box(RED, (x, y, z + 40), (22, 20, 10))


def hook(sc, piv, drop, load=None, phase=0.0, swing=5.0, axis=90.0, rate=0.22):
    """What hangs from a hoist beam at piv (mesh space): a rope, a block, a load. It swings slowly about the beam's end."""
    m = sc.part(P.motion(swing=swing, pivot=piv, axis=axis, rate=rate, phase=phase))
    x, y, z = piv
    m.box(ROPE, (x, y, z - drop / 2.0), (6, 6, drop))
    m.box(WOOD_D, (x, y, z - drop - 12), (24, 16, 26))
    if load == 'sack':
        m.box(CREAM, (x, y, z - drop - 62), (62, 50, 74))
        m.box(ROPE, (x, y, z - drop - 46), (66, 54, 8))
    elif load == 'ice':
        m.box(SKY, (x, y, z - drop - 58), (64, 56, 64))
        m.box(WHITE, (x, y, z - drop - 24), (58, 50, 6))
    elif load == 'boxes':
        m.box(WHITE, (x, y, z - drop - 44), (84, 54, 32))
        m.box(TEAL, (x, y, z - drop - 76), (84, 54, 32))
        m.box(ROPE, (x, y, z - drop - 60), (8, 60, 72))
    return m


def vane(sc, piv, phase=0.0):
    """A weather vane: a fish that turns slowly to and fro on its spindle (piv: its foot on the roof, mesh space)."""
    g = sc.still()
    g.base = None
    x, y, z = piv
    g.box(WOOD_D, (x, y, z + 55), (6, 6, 110))
    m = sc.part(P.motion(swing=28.0, pivot=(x, y, z), axis=P.UP, rate=0.11, phase=phase))
    side(m, TEAL, [(x - 46, z + 112), (x - 20, z + 126), (x + 30, z + 122), (x + 52, z + 136), (x + 52, z + 92), (x + 30, z + 104), (x - 20, z + 100)],
         y - 4, y + 4)
    m.box(WHITE, (x - 24, y, z + 114), (8, 10, 8))


def net(sc, x0, x1, y, z, drop, phase=0.0, col=NET, floats=True, axis=0.0, swing=4.0):
    """A net hung to dry from a pole at height z, from x0 to x1 (mesh space): it sways about the pole."""
    m = sc.part(P.motion(swing=swing, pivot=((x0 + x1) / 2.0, y, z), axis=axis, rate=0.3, phase=phase))
    w = x1 - x0
    m.box(col, ((x0 + x1) / 2.0, y, z - drop / 2.0), (w, 6, drop))
    if floats:
        n = max(2, int(w / 60.0))
        for k in range(n):
            m.box(WHITE if k % 2 else RED, (x0 + (k + 0.5) * w / n, y, z - drop - 6), (16, 12, 16))
    return m


# ------------------------------------------------------------------------------------------ the buildings
def net_loft(sc):
    """NET LOFT: 5.5 x 6 m on stilts, the loft jettied out over a boarded store, a steep red gable to the road with a
    hoist beam under it (a sack swings on the hook), a net drying on a pole at its side, a ladder to the door."""
    b = Bld(sc, 0, 0, pitch=1.6, roll=-1.0)
    g = b.g()
    W, D, F1, F2, E, RH = 550.0, 600.0, 250.0, 540.0, 920.0, 370.0
    stilts(g, (-240, 0, 240), (-260, 260), F1)
    tilt_box(g, WOOD_D, (-240, -262, 10), (0, -262, F1 - 30), 12, 16)
    tilt_box(g, WOOD_D, (240, -262, 10), (0, -262, F1 - 30), 12, 16)
    g.box(WOOD_D, (0, 0, F1), (W + 40, D + 40, 26))
    g.box(WOOD, (0, -340, F1 + 7), (W, 90, 14))                                   # the porch
    g.box(WOOD, (0, 0, (F1 + F2) / 2.0), (W, D, F2 - F1))                         # the boarded store
    for z in (F1 + 90, F1 + 190):
        g.box(WOOD_D, (0, -302, z), (W + 4, 8, 8))
    g.box(WOOD_D, (0, -25, F2), (W + 80, D + 70, 22))                             # the jetty
    for x in (-250, 0, 250):
        tilt_box(g, WOOD_D, (x, -300, F2 - 70), (x, -352, F2 - 8), 14, 14)
    YF = -350.0
    g.box(CREAM, (0, -25, (F2 + E) / 2.0), (W + 60, D + 50, E - F2))              # the loft: plaster and frame
    frame(g, (-297, -100, 100, 297), YF, F2 + 11, E, rails=((F2 + E) / 2.0 + 70,),
          diag=((-297, F2 + 20, -100, (F2 + E) / 2.0 + 60), (297, F2 + 20, 100, (F2 + E) / 2.0 + 60)))
    for x in (-198, 198):
        window(g, x, YF, (F2 + E) / 2.0 - 30, w=78, h=104)
    roof_gable(g, RED, 0, -25, W + 60, D + 50, E, RH, over=40, wall=CREAM)
    g.box(TEAL, (0, YF - 4, E + 95), (92, 10, 150))                               # the loft door in the gable
    g.box(WOOD_D, (0, YF - 4, E + 250), (16, 12, 150))
    g.box(WOOD_D, (0, YF - 4, E + 176), (300, 12, 14))
    g.box(WOOD_D, (0, YF - 95, E + 300), (18, 190, 20))                           # the hoist beam
    tilt_box(g, WOOD_D, (0, YF - 2, E + 215), (0, YF - 120, E + 292), 10, 12)
    g.box(TEAL, (-130, -304, F1 + 110), (100, 8, 200))                            # the store's door, a window
    window(g, 140, -300, F1 + 170, w=70, h=90)
    for x in (-178, -82):                                                         # the ladder
        tilt_box(g, WOOD_D, (x, -470, -60), (x, -388, F1 + 6), 9, 9)
    for k in range(5):
        f = (k + 1.0) / 6.0
        g.box(WOOD_D, (-130, -470 + 82 * f, -60 + (F1 + 66) * f), (96, 8, 8))
    pot(g, 60, -350, F1 + 14)
    pot(g, 215, -350, F1 + 14)
    g.box(WOOD_D, (W / 2.0 + 70, -180, F2 - 30), (150, 10, 10))                   # the net pole
    hook(sc, b.pt((0, YF - 172, E + 290)), 300.0, 'sack', phase=0.2)
    p0, p1 = b.pt((W / 2.0 + 28, -180, F2 - 36)), b.pt((W / 2.0 + 140, -180, F2 - 36))
    net(sc, p0[0], p1[0], p0[1], (p0[2] + p1[2]) / 2.0, 250.0, phase=0.5)


def sail_loft(sc):
    """SAIL MAKER'S HOUSE: 4.5 m wide and four storeys, each floor jettied over the last, a white-tiled gable, a sail
    hung from a yard to dry across the front (it sways), a striped awning over the door, a fish vane on the ridge."""
    b = Bld(sc, 0, 0, pitch=-2.0, roll=0.8)
    g = b.g()
    W, D = 450.0, 540.0
    Z1, Z2, Z3, E, RH = 300.0, 610.0, 920.0, 1180.0, 330.0
    g.box(OCHRE, (0, 0, (Z1 - FOOT) / 2.0), (W, D, Z1 + FOOT))                    # stone below
    for z in (90, 190):
        g.box(STONE, (0, -D / 2.0 - 2, z), (W + 6, 8, 10))
    g.box(TEAL, (-95, -D / 2.0 - 3, 105), (104, 8, 210))
    window(g, 110, -D / 2.0, 160, w=84, h=100, shut=None)
    g.box(WHITE, (110, -D / 2.0 - 6, 104), (100, 14, 10))
    awning(g, -205, 205, -D / 2.0, 285, depth=120, drop=50)
    g.box(WOOD_D, (0, -15, Z1), (W + 50, D + 40, 20))                             # first jetty: boards
    g.box(WOOD, (0, -15, (Z1 + Z2) / 2.0), (W + 20, D + 30, Z2 - Z1))
    Y2 = -D / 2.0 - 30
    window(g, 0, Y2, (Z1 + Z2) / 2.0, w=90, h=120)
    for z in (Z1 + 70, Z2 - 70):
        g.box(WOOD_D, (0, Y2 - 2, z), (W + 24, 8, 8))
    g.box(WOOD_D, (0, -32, Z2), (W + 80, D + 74, 20))                             # second jetty: plaster and frame
    g.box(CREAM, (0, -32, (Z2 + E) / 2.0), (W + 44, D + 64, E - Z2))
    Y3 = -D / 2.0 - 64
    frame(g, (-239, -80, 80, 239), Y3, Z2 + 10, E, rails=(Z3,),
          diag=((-239, Z2 + 16, -80, Z3 - 10), (239, Z3 + 14, 80, E - 10)))
    window(g, 0, Y3, (Z2 + Z3) / 2.0, w=84, h=120, shut=None)
    g.box(TEAL, (0, Y3 - 4, (Z3 + E) / 2.0 - 10), (96, 10, 190))                  # the sail door
    for x in (-250, 250):
        tilt_box(g, WOOD_D, (x, -D / 2.0 - 28, Z2 - 60), (x, Y3 + 2, Z2 - 6), 14, 14)
    roof_gable(g, WHITE, 0, -32, W + 44, D + 64, E, RH, over=38, trim=RED, wall=CREAM)
    g.box(WOOD_D, (0, Y3 - 4, E + 130), (16, 12, 240))
    chimney(g, 120, 150, E + 150, 260, dx=18, dy=-10)
    for x in (-215, 215):                                                         # the yard's two brackets
        g.box(WOOD_D, (x, Y3 - 55, Z3 + 190), (12, 110, 12))
    yd = b.pt((0, Y3 - 100, Z3 + 190))
    m = sc.part(P.motion(swing=3.5, pivot=yd, axis=0.0, rate=0.24, phase=0.1))
    m.box(WOOD_D, yd, (500, 10, 10))
    m.box(CREAM, (yd[0], yd[1], yd[2] - 215), (420, 6, 420))
    m.box(RED, (yd[0], yd[1] - 1, yd[2] - 300), (420, 9, 46))
    m.box(RED, (yd[0], yd[1] - 1, yd[2] - 120), (420, 9, 22))
    for (x, z) in ((180, 16), (200, 44)):                                         # rolls of sailcloth by the door
        g.cyl(CREAM, (x, -D / 2.0 - 44, z), 14, 150, rot=(90, 0, 0), steps=6)
    g.cyl(WHITE, (215, -D / 2.0 - 74, 16), 14, 150, rot=(90, 0, 0), steps=6)
    g.box(WOOD, (0, Y2 - 12, (Z1 + Z2) / 2.0 - 74), (110, 24, 16))                # a window box
    g.box(LEAF, (0, Y2 - 12, (Z1 + Z2) / 2.0 - 58), (100, 22, 16))
    g.box(RED, (-24, Y2 - 14, (Z1 + Z2) / 2.0 - 48), (30, 20, 10))
    g.box(WHITE, (26, Y2 - 14, (Z1 + Z2) / 2.0 - 48), (26, 20, 10))
    vane(sc, b.pt((0, -230, E + RH + 20)), phase=0.6)


def smokery(sc):
    """SMOKERY: a low stone house, 8 m of it, under a red roof whose eaves face the road, a row of four crooked
    chimneys of four heights on the ridge (pale puffs rise and sink over two), fish hung under the eaves, a lean-to."""
    b = Bld(sc, 0, 0, pitch=0.0, roll=-1.2)
    g = b.g()
    W, D, Z1, E, RH = 760.0, 580.0, 330.0, 560.0, 270.0
    g.box(OCHRE, (0, 0, (Z1 - FOOT) / 2.0), (W, D, Z1 + FOOT))
    for z in (80, 170, 260):
        g.box(STONE, (0, -D / 2.0 - 2, z), (W + 6, 8, 10))
    g.box(WOOD_D, (0, -20, Z1), (W + 50, D + 60, 20))
    g.box(WOOD, (0, -20, (Z1 + E) / 2.0), (W + 30, D + 50, E - Z1))
    YF = -D / 2.0 - 45
    for x in (-300, -150, 0, 150, 300):
        g.box(WOOD_D, (x, YF - 2, (Z1 + E) / 2.0), (10, 8, E - Z1))
    for x in (-225, 75):
        window(g, x, YF, (Z1 + E) / 2.0 + 10, w=64, h=80)
    for sx in (-1, 1):                                                            # the double doors
        g.box(TEAL, (sx * 62 - 60, -D / 2.0 - 3, 115), (116, 8, 230))
        g.box(WHITE, (sx * 62 - 60, -D / 2.0 - 7, 170), (104, 6, 10))
    window(g, 230, -D / 2.0, 170, w=76, h=90, shut=None)
    g.box(WHITE, (230, -D / 2.0 - 6, 120), (92, 14, 10))
    roof_eaves(g, RED, 0, -20, W + 30, D + 50, E, RH, over=40, wall=WOOD)
    zr = E + RH
    for k, (x, h, dx, dy) in enumerate(((-285, 250, -22, 8), (-95, 170, 16, -10), (95, 310, 26, 12), (285, 210, -14, -6))):
        chimney(g, x, 30, zr - 60, h + 60, dx=dx, dy=dy, col=OCHRE if k % 2 else CREAM)
    g.box(WOOD_D, (0, YF - 30, E - 40), (W - 80, 8, 8))                           # the rail the fish hang from
    for x in (-340, 340):
        g.box(WOOD_D, (x, YF - 16, E - 40), (10, 40, 10))
    rl = b.pt((0, YF - 30, E - 40))
    m = sc.part(P.motion(swing=5.0, pivot=rl, axis=0.0, rate=0.35, phase=0.3))
    for k in range(9):
        m.box(WHITE if k % 2 else SKY, (rl[0] - 300 + k * 75, rl[1], rl[2] - 28), (14, 8, 46))
    g.box(WOOD, (W / 2.0 + 65, 60, 87.5), (130, 300, 425))                        # the lean-to, its shingle roof
    g.box(WOOD_D, (W / 2.0 + 70, 60, 338), (180, 340, 14), rot=(-16, 0, 0))
    for x in (-260, -200, -140):                                                  # split logs stacked by the door
        g.box(WOOD_D, (x, -D / 2.0 - 40, 26), (52, 70, 52))
    g.box(WOOD_D, (-230, -D / 2.0 - 40, 76), (52, 70, 46))
    for k, (ci, h) in enumerate(((0, 250), (2, 310))):
        x = (-285, -95, 95, 285)[ci] + (-22, 16, 26, -14)[ci]
        top = b.pt((x, 30 + (8, -10, 12, -6)[ci], zr + h + 70))
        s = sc.part(P.motion(bob=-26.0, rate=0.14, phase=0.3 + 0.4 * k, pivot=top))
        s.sphere(CREAM, (top[0], top[1], top[2] + 20), 30, steps=(3, 5))
        s.sphere(WHITE, (top[0] + 22, top[1] + 6, top[2] + 72), 40, steps=(3, 5))


def boat_shed(sc):
    """BOAT BUILDER'S SHED: 9 x 7 m, open to the road under a wide shingled gable, a half-planked hull on its cradle
    inside (teal strakes below, bare ribs above), plank stacks, a pennant on the ridge that swings."""
    b = Bld(sc, 0, 0, pitch=0.8, roll=0.0)
    g = b.g()
    W, D, E, RH = 880.0, 680.0, 470.0, 250.0
    for sx in (-1, 1):
        g.box(WOOD, (sx * (W / 2.0 - 10), 0, (E - FOOT) / 2.0), (20, D, E + FOOT))
        for y in (-200, 0, 200):
            g.box(WOOD_D, (sx * (W / 2.0 + 2), y, E / 2.0), (8, 12, E))
        g.box(WOOD_D, (sx * (W / 2.0 - 16), -D / 2.0 + 16, (E - FOOT) / 2.0), (32, 32, E + FOOT))
        tilt_box(g, WOOD_D, (sx * (W / 2.0 - 16), -D / 2.0 + 14, E - 130), (sx * (W / 2.0 - 150), -D / 2.0 + 14, E - 6), 14, 14)
    g.box(WOOD, (0, D / 2.0 - 10, (E - FOOT) / 2.0), (W, 20, E + FOOT))
    g.box(OCHRE, (0, 0, -FOOT / 2.0 + 6), (W, D, FOOT))                           # the stone floor
    g.box(WOOD_D, (0, -D / 2.0 + 14, E - 10), (W + 20, 24, 24))
    roof_gable(g, WOOD_D, 0, 0, W, D, E, RH, over=45, wall=None)
    side(g, WOOD, [(-W / 2.0, E), (W / 2.0, E), (0, E + RH)], D / 2.0 - 20, D / 2.0)
    side(g, WOOD, [(-W / 2.0 + 150, E + 80), (W / 2.0 - 150, E + 80), (0, E + RH)], -D / 2.0, -D / 2.0 + 14)
    g.box(CREAM, (0, -D / 2.0 - 6, E + 118), (150, 10, 64))                       # a blank board in the gable
    g.box(SKY, (-150, 0, E + 184), (150, 240, 10), rot=(29.6, 0, 0))              # a skylight
    # the hull, broadside to the road
    hy, hz = 30.0, 150.0
    side(g, TEAL, [(-300, hz + 62), (-262, hz + 4), (-190, hz - 34), (190, hz - 34), (268, hz + 6), (312, hz + 70)], hy - 78, hy + 78)
    g.box(WHITE, (0, hy - 80, hz + 60), (600, 8, 12))
    g.box(WOOD_D, (0, hy, hz - 44), (560, 16, 22))                                # the keel
    for k in range(7):                                                            # the ribs, bare above the strakes
        x = -240 + k * 80
        for sy in (-1, 1):
            tilt_box(g, WOOD, (x, hy + sy * 76, hz + 50), (x, hy + sy * 92, hz + 148), 10, 10)
    for sx, top in ((-1, 196), (1, 214)):                                         # stem and stern post
        tilt_box(g, WOOD_D, (sx * 286, hy, hz + 20), (sx * 322, hy, hz + top), 16, 16)
    for sy in (-1, 1):
        g.box(WOOD, (0, hy + sy * 94, hz + 148), (520, 8, 10))
    for x in (-170, 170):                                                         # the cradle
        g.box(WOOD_D, (x, hy, 56), (24, 240, 16))
        for sy in (-1, 1):
            tilt_box(g, WOOD_D, (x, hy + sy * 110, 0), (x, hy + sy * 60, hz - 20), 14, 14)
            g.box(WOOD_D, (x, hy + sy * 110, 30), (20, 20, 60))
    for k in range(4):                                                            # planks stacked outside, a trestle
        g.box(WOOD if k % 2 else ROPE, (-250, -D / 2.0 - 70, 12 + k * 14), (330 - k * 26, 44, 12))
    for x in (160, 330):
        g.box(WOOD_D, (x, -D / 2.0 - 70, 40), (12, 70, 80))
    g.box(WOOD, (245, -D / 2.0 - 70, 86), (260, 34, 12))
    g.box(TEAL, (250, -D / 2.0 - 70, 104), (70, 26, 24))
    top = b.pt((0, -D / 2.0 - 30, E + RH + 20))
    g2 = sc.still()
    g2.base = None
    g2.box(WOOD_D, (top[0], top[1], top[2] + 100), (8, 8, 220))
    f = sc.part(P.motion(swing=16.0, pivot=(top[0], top[1], top[2] + 180), axis=P.UP, rate=0.3, phase=0.4))
    f.box(TEAL, (top[0] + 66, top[1], top[2] + 180), (120, 5, 56))
    f.box(WHITE, (top[0] + 150, top[1], top[2] + 180), (50, 5, 34))


def harbour_inn(sc):
    """HARBOUR INN: stone below, two jettied floors of plaster and frame, a red roof with its eaves to the road and
    a crooked dormer gable, a chimney at each end, a blank board swinging on its bracket, a bay window, an awning."""
    b = Bld(sc, 0, 0, pitch=1.2, roll=-0.8)
    g = b.g()
    W, D = 760.0, 560.0
    Z1, Z2, E, RH = 300.0, 590.0, 860.0, 300.0
    g.box(OCHRE, (0, 0, (Z1 - FOOT) / 2.0), (W, D, Z1 + FOOT))
    for z in (100, 200):
        g.box(STONE, (0, -D / 2.0 - 2, z), (W + 6, 8, 10))
    g.box(TEAL, (-40, -D / 2.0 - 3, 108), (110, 8, 216))                          # the door, its steps
    g.box(STONE, (-40, -D / 2.0 - 40, -14), (170, 80, 40))
    g.box(CREAM, (-250, -D / 2.0 - 30, 170), (200, 60, 150))                      # the bay window
    g.box(NAVY, (-250, -D / 2.0 - 62, 176), (170, 6, 110))
    for x in (-306, -250, -194):
        g.box(WOOD_D, (x, -D / 2.0 - 64, 176), (8, 8, 116))
    g.box(RED, (-250, -D / 2.0 - 34, 254), (220, 80, 16))
    awning(g, 60, 350, -D / 2.0, 290, depth=120, drop=50)
    g.box(WOOD, (205, -D / 2.0 - 70, 46), (240, 36, 8))                           # a bench under it
    for x in (110, 300):
        g.box(WOOD_D, (x, -D / 2.0 - 70, 22), (10, 34, 44))
    g.box(WOOD_D, (0, -18, Z1), (W + 60, D + 50, 22))                             # first jetty
    g.box(CREAM, (0, -18, (Z1 + Z2) / 2.0), (W + 30, D + 36, Z2 - Z1))
    Y1 = -D / 2.0 - 36
    frame(g, (-387, -190, 0, 190, 387), Y1, Z1 + 11, Z2, rails=((Z1 + Z2) / 2.0 + 74,),
          diag=((-387, Z1 + 20, -190, (Z1 + Z2) / 2.0 + 60), (387, Z1 + 20, 190, (Z1 + Z2) / 2.0 + 60)))
    for x in (-95, 95):
        window(g, x, Y1, (Z1 + Z2) / 2.0 - 30, w=70, h=100)
    g.box(WOOD_D, (0, -36, Z2), (W + 90, D + 86, 22))                             # second jetty
    g.box(CREAM, (0, -36, (Z2 + E) / 2.0), (W + 60, D + 72, E - Z2))
    Y2 = -D / 2.0 - 72
    frame(g, (-402, -200, 0, 200, 402), Y2, Z2 + 11, E, rails=((Z2 + E) / 2.0 + 66,))
    for x in (-300, -100, 100, 300):
        window(g, x, Y2, (Z2 + E) / 2.0 - 34, w=62, h=92, shut=None)
    for x in (-380, 0, 380):
        tilt_box(g, WOOD_D, (x, Y1 + 2, Z2 - 60), (x, Y2 + 2, Z2 - 8), 14, 14)
    roof_eaves(g, RED, 0, -36, W + 60, D + 72, E, RH, over=40, wall=CREAM)
    g.box(CREAM, (120, Y2 + 60, E + 110), (190, 150, 190))                        # the dormer, a little askew
    side(g, CREAM, [(25, E + 205), (215, E + 205), (120, E + 320)], Y2 - 15, Y2 + 135)
    tilt_box(g, RED, (10, Y2 + 50, E + 190), (124, Y2 + 50, E + 330), 190, 12)
    tilt_box(g, RED, (230, Y2 + 50, E + 190), (116, Y2 + 50, E + 330), 190, 12)
    g.box(NAVY, (120, Y2 - 18, E + 120), (70, 8, 96))
    chimney(g, -330, 40, E + 120, 330, dx=-20, dy=6)
    chimney(g, 340, 60, E + 120, 250, dx=14, dy=-12, col=CREAM)
    g.box(WOOD_D, (-330, Y1 - 75, Z2 - 40), (12, 150, 12))                        # the sign's bracket
    tilt_box(g, WOOD_D, (-330, Y1 - 2, Z2 - 130), (-330, Y1 - 110, Z2 - 46), 9, 9)
    pv = b.pt((-330, Y1 - 100, Z2 - 46))
    m = sc.part(P.motion(swing=9.0, pivot=pv, axis=90.0, rate=0.3, phase=0.7))
    for dy in (-40, 40):
        m.box(WOOD_D, (pv[0], pv[1] + dy, pv[2] - 14), (5, 5, 28))
    m.box(CREAM, (pv[0], pv[1], pv[2] - 68), (8, 120, 84))                        # a blank board: no word, no mark
    m.box(TEAL, (pv[0], pv[1], pv[2] - 68), (12, 128, 10))
    m.box(WOOD_D, (pv[0], pv[1], pv[2] - 24), (10, 128, 8))
    m.box(WOOD_D, (pv[0], pv[1], pv[2] - 112), (10, 128, 8))
    pot(g, -110, -D / 2.0 - 46, 6)
    pot(g, 30, -D / 2.0 - 46, 6)


def ice_house(sc):
    """ICE HOUSE: a windowless plaster box on an ochre plinth, buttressed, under a steep red gable; big strapped doors,
    a louvre in the gable, a hoist with a block of ice on the hook (it swings), ice stacked on a sledge by the door."""
    b = Bld(sc, 0, 0, pitch=-1.2, roll=0.0)
    g = b.g()
    W, D, P0, E, RH = 660.0, 580.0, 130.0, 700.0, 390.0
    g.box(OCHRE, (0, 0, (P0 - FOOT) / 2.0), (W + 40, D + 40, P0 + FOOT))
    g.box(CREAM, (0, 0, (P0 + E) / 2.0), (W, D, E - P0))
    for sx in (-1, 1):                                                            # the buttresses
        side(g, OCHRE, [(sx * (W / 2.0 - 40), -60), (sx * (W / 2.0 + 70), -60), (sx * (W / 2.0 + 20), 420), (sx * (W / 2.0 - 40), 420)], -D / 2.0 - 30, -D / 2.0 + 50)
        g.box(STONE, (sx * (W / 2.0 - 8), -D / 2.0 + 10, 426), (70, 90, 14))
    g.box(WOOD_D, (0, 0, E), (W + 30, D + 30, 22))
    for sx in (-1, 1):                                                            # the doors, their straps
        g.box(TEAL, (sx * 82, -D / 2.0 - 3, P0 + 150), (156, 8, 300))
        for z in (P0 + 60, P0 + 240):
            g.box(WHITE, (sx * 82, -D / 2.0 - 7, z), (140, 6, 12))
    g.box(WOOD_D, (0, -D / 2.0 - 4, P0 + 312), (360, 12, 22))
    g.box(STONE, (0, -D / 2.0 - 70, P0 / 2.0 - 30), (380, 130, P0 + 60))          # the loading step
    roof_gable(g, RED, 0, 0, W, D, E, RH, over=44, wall=CREAM)
    YF = -D / 2.0
    for k in range(4):                                                            # the louvre
        g.box(WOOD_D, (0, YF - 5, E + 96 + k * 26), (120 - k * 14, 12, 12))
    g.box(WOOD_D, (0, YF - 105, E + 300), (20, 210, 22))                          # the hoist
    tilt_box(g, WOOD_D, (0, YF - 2, E + 200), (0, YF - 130, E + 290), 10, 12)
    g.box(WOOD_D, (0, YF - 4, E + 250), (16, 12, 120))
    hook(sc, b.pt((0, YF - 190, E + 290)), 330.0, 'ice', phase=0.6, swing=4.0)
    g.box(WOOD_D, (-250, -D / 2.0 - 110, P0 - 100), (150, 90, 12))                # the sledge, three blocks on it
    for sy in (-1, 1):
        g.box(WOOD_D, (-250, -D / 2.0 - 110 + sy * 36, P0 - 114), (170, 10, 16))
    g.box(SKY, (-282, -D / 2.0 - 110, P0 - 64), (62, 60, 60))
    g.box(WHITE, (-214, -D / 2.0 - 110, P0 - 64), (62, 60, 60))
    g.box(SKY, (-250, -D / 2.0 - 110, P0 - 6), (62, 60, 56))
    g.box(WOOD_D, (250, -D / 2.0 - 20, 330), (10, 60, 10))                        # a lamp bracket, unlit
    g.box(WHITE, (250, -D / 2.0 - 50, 306), (26, 26, 38))
    g.box(WOOD_D, (250, -D / 2.0 - 50, 330), (34, 34, 8))
    for (x, z) in ((240, 24), (300, 24), (270, 66)):                              # sacks of sawdust
        g.box(CREAM, (x, -D / 2.0 - 110, z), (56, 44, 42))
    for x in (-210, -70, 70, 210):                                                # tie plates under the wall plate
        g.box(WOOD_D, (x, -D / 2.0 - 3, E - 70), (18, 8, 18))
    vane(sc, b.pt((0, -200, E + RH + 20)), phase=0.2)


def twin_gables(sc):
    """TWO HOUSES SHOULDER TO SHOULDER, leaning on each other: a tall red gable with a jettied top floor and a lower
    house with white tiles and its eaves to the road; two crooked chimneys, a washing line between them (it sways)."""
    # ---- the tall one, left
    a = Bld(sc, -235, 0, pitch=-2.2, roll=-0.6)
    g = a.g()
    W, D, Z1, Z2, E, RH = 410.0, 540.0, 290.0, 580.0, 860.0, 320.0
    g.box(WOOD, (0, 0, (Z1 - FOOT) / 2.0), (W, D, Z1 + FOOT))
    for z in (70, 150, 230):
        g.box(WOOD_D, (0, -D / 2.0 - 2, z), (W + 4, 8, 8))
    g.box(TEAL, (-80, -D / 2.0 - 3, 105), (100, 8, 210))
    window(g, 95, -D / 2.0, 165, w=64, h=90)
    g.box(WOOD_D, (0, -15, Z1), (W + 40, D + 40, 20))
    g.box(CREAM, (0, -15, (Z1 + Z2) / 2.0), (W + 16, D + 30, Z2 - Z1))
    Y1 = -D / 2.0 - 30
    frame(g, (-205, 0, 205), Y1, Z1 + 10, Z2, rails=((Z1 + Z2) / 2.0 + 70,))
    for x in (-102, 102):
        window(g, x, Y1, (Z1 + Z2) / 2.0 - 28, w=60, h=96, shut=None)
    g.box(WOOD_D, (0, -34, Z2), (W + 70, D + 78, 20))
    g.box(CREAM, (0, -34, (Z2 + E) / 2.0), (W + 40, D + 68, E - Z2))
    Y2 = -D / 2.0 - 68
    frame(g, (-217, 0, 217), Y2, Z2 + 10, E, diag=((-217, Z2 + 16, 0, E - 16), (217, Z2 + 16, 0, E - 16)))
    window(g, -108, Y2, (Z2 + E) / 2.0 + 10, w=58, h=90, shut=None)
    window(g, 108, Y2, (Z2 + E) / 2.0 + 10, w=58, h=90, shut=None)
    roof_gable(g, RED, 0, -34, W + 40, D + 68, E, RH, over=34, wall=CREAM)
    g.box(NAVY, (0, Y2 - 3, E + 110), (56, 10, 80))
    chimney(g, -90, 140, E + 130, 290, dx=-22, dy=8)
    wa = a.pt((W / 2.0 + 20, Y2 - 6, Z2 + 60))
    # ---- the low one, right
    c = Bld(sc, 222, 20, pitch=2.4, roll=0.6)
    g = c.g()
    W, D, Z1, E, RH = 450.0, 520.0, 300.0, 610.0, 280.0
    g.box(OCHRE, (0, 0, (Z1 - FOOT) / 2.0), (W, D, Z1 + FOOT))
    for z in (95, 195):
        g.box(STONE, (0, -D / 2.0 - 2, z), (W + 6, 8, 10))
    g.box(TEAL, (90, -D / 2.0 - 3, 105), (104, 8, 210))
    window(g, -95, -D / 2.0, 165, w=76, h=96, shut=None)
    awning(g, -190, 0, -D / 2.0, 270, depth=96, drop=40)
    g.box(WOOD_D, (0, -18, Z1), (W + 50, D + 44, 20))
    g.box(WOOD, (0, -18, (Z1 + E) / 2.0), (W + 24, D + 36, E - Z1))
    Y1 = -D / 2.0 - 36
    for x in (-180, -60, 60, 180):
        g.box(WOOD_D, (x, Y1 - 2, (Z1 + E) / 2.0), (10, 8, E - Z1 - 20))
    for x in (-120, 120):
        window(g, x, Y1, (Z1 + E) / 2.0, w=66, h=110)
    roof_eaves(g, WHITE, 0, -18, W + 24, D + 36, E, RH, over=36, trim=RED, wall=WOOD)
    chimney(g, 130, 30, E + 150, 300, dx=24, dy=-8, col=CREAM)
    pot(g, -40, -D / 2.0 - 30, 0)
    wb = c.pt((60, Y1 - 6, E - 50))
    # ---- the washing, on a line from one house's window to the other's
    piv = ((wa[0] + wb[0]) / 2.0, (wa[1] + wb[1]) / 2.0, (wa[2] + wb[2]) / 2.0)
    m = sc.part(P.motion(swing=6.0, pivot=piv, axis=0.0, rate=0.4, phase=0.15))
    tilt_box(m, ROPE, wa, wb, 5, 5)
    for k, col in enumerate((WHITE, TEAL, PINK, CREAM)):
        f = (k + 0.7) / 4.4
        x, y, z = wa[0] + (wb[0] - wa[0]) * f, wa[1] + (wb[1] - wa[1]) * f, wa[2] + (wb[2] - wa[2]) * f
        m.box(col, (x, y, z - 30), (34 if k % 2 else 46, 5, 56))


# ------------------------------------------------------------------------------------------ the props
def dry_rack(sc):
    """FISH-DRYING RACKS (low, top 1.7 m): three racks, two rails each, split fish hung along them that sway."""
    g = sc.still()
    k2 = 0
    for (x0, y0, L) in ((-240, -90, 300), (150, -20, 340), (-120, 100, 320)):
        for ex in (x0 - L / 2.0, x0 + L / 2.0):
            tilt_box(g, WOOD_D, (ex, y0 - 46, -50), (ex, y0, 168), 10, 10)
            tilt_box(g, WOOD_D, (ex, y0 + 46, -50), (ex, y0, 168), 10, 10)
        for j, (dy, z) in enumerate(((-10, 126), (0, 168))):
            g.box(WOOD, (x0, y0 + dy, z), (L + 30, 8, 8))
            m = sc.part(P.motion(swing=6.0, pivot=(x0, y0 + dy, z), axis=0.0, rate=0.4, phase=0.17 * k2))
            n = 7
            for k in range(n):
                m.box(WHITE if (k + j) % 2 else SKY, (x0 - L / 2.0 + (k + 0.5) * L / n, y0 + dy, z - 26), (16, 6, 44))
            k2 += 1
    g.box(WOOD, (40, -150, 16), (90, 60, 32))                                     # a salt tub, a low bench
    g.box(WHITE, (40, -150, 34), (76, 46, 6))


def fish_boxes(sc):
    """STACKED FISH BOXES (low, top 1.3 m): white and teal only, the catch showing in the open ones, a coil of rope,
    a wicker basket, a hand barrow."""
    g = sc.still()
    k = 0
    for (x, y, n, yaw) in ((-150, 20, 4, 0), (-60, 30, 3, 8), (40, 10, 2, -6), (-110, -60, 1, 20), (130, -30, 1, -14)):
        for j in range(n):
            g.box(TEAL if (k + j) % 2 else WHITE, (x, y, 16 + j * 32), (84, 56, 30), rot=(0, yaw + (j % 2) * 5, 0))
        g.box(SKY, (x, y, 16 + n * 32 - 12), (70, 42, 10), rot=(0, yaw, 0))        # the catch, on ice
        g.box(WHITE, (x + 12, y, 16 + n * 32 - 6), (30, 12, 8), rot=(0, yaw + 30, 0))
        k += 1
    g.cyl(ROPE, (150, 60, 0), 40, 14, steps=8)
    g.cyl(ROPE, (150, 60, 14), 30, 12, steps=8)
    g.cyl(ROPE, (60, -80, 0), 26, 44, steps=8, top_r=36)
    g.box(SKY, (60, -80, 44), (40, 40, 8))
    g.box(WOOD, (230, 30, 40), (150, 70, 10))                                     # the barrow
    for sy in (-1, 1):
        g.box(WOOD_D, (250, 30 + sy * 32, 34), (230, 8, 8))
        g.box(WOOD_D, (300, 30 + sy * 32, 16), (8, 8, 34))
    g.cyl(WOOD_D, (170, 4, 20), 20, 52, rot=(0, 0, 90), steps=8)
    g.box(WHITE, (225, 30, 60), (84, 56, 30))


def capstan(sc):
    """CAPSTAN (low, top 1.45 m): a tapered timber head with its six bars shipped, on a round stone bed, a rope led
    off to a mooring ring."""
    g = sc.still()
    g.cyl(OCHRE, (0, 0, -50), 170, 74, steps=10)
    g.cyl(STONE, (0, 0, 24), 150, 8, steps=10)
    g.cyl(WOOD_D, (0, 0, 32), 56, 14, steps=8)
    g.cyl(WOOD, (0, 0, 46), 42, 70, steps=8, top_r=32)
    for k in range(4):
        g.box(WOOD_D, (0, 0, 80), (12, 92, 62), rot=(0, k * 45.0, 0))
    g.cyl(WOOD_D, (0, 0, 112), 48, 26, steps=8)
    g.cyl(WOOD, (0, 0, 138), 30, 8, steps=8)
    for k in range(3):
        g.box(WOOD, (0, 0, 125), (310, 10, 10), rot=(0, k * 60.0 + 15.0, 0))
    g.cyl(ROPE, (0, 0, 56), 46, 30, steps=8)
    tilt_box(g, ROPE, (40, -20, 70), (150, -110, 34), 7, 7)
    g.cyl(STONE, (150, -110, 24), 16, 26, steps=6)
    g.cyl(ROPE, (-100, 80, 32), 34, 12, steps=8)


def net_boat(sc):
    """BEACHED ROWING BOAT (low, top 1.5 m): teal, hauled out on chocks and heeled over, full of nets and floats,
    her oars leant against her."""
    g = sc.still()
    g.base = ft_art._xf((0, 0, 0), (0, 0, -9.0), (1, 1, 1))
    side(g, TEAL, [(-235, 104), (-205, 40), (-150, 14), (150, 14), (205, 40), (248, 114)], -72, 72)
    for sy in (-1, 1):
        g.box(WHITE, (0, sy * 74, 100), (476, 8, 12))
    g.box(WHITE, (0, 0, 10), (430, 14, 10))
    for x in (-110, 60):
        g.box(WOOD, (x, 0, 92), (30, 140, 8))
    g.sphere(NET, (-10, 0, 100), 60, scale=(2.7, 1.0, 0.62), steps=(4, 8))
    for k, (x, y) in enumerate(((-120, -30), (-60, 28), (10, -36), (70, 20), (130, -14))):
        g.sphere(WHITE if k % 2 else RED, (x, y, 136), 13, steps=(3, 5))
    g = sc.still()
    g.base = None
    for x in (-140, 140):
        g.box(WOOD_D, (x, -40, 4), (30, 150, 30))
    tilt_box(g, WOOD, (-190, -118, 0), (150, -96, 118), 8, 8)
    tilt_box(g, WOOD, (-150, -130, 0), (190, -104, 112), 8, 8)
    g.box(WOOD, (-196, -119, 10), (60, 6, 22), rot=(18, 4, 0))
    g.box(WOOD, (-156, -131, 10), (60, 6, 22), rot=(18, 4, 0))
    g.box(WOOD_D, (60, 120, 20), (16, 16, 110), rot=(0, 0, 14))                   # a prop under her high side


def quay_crane(sc):
    """TIMBER QUAY CRANE (TALL, 6.5 m): sheer legs on a stone bed, a back stay, a hand winch between the legs; two fish
    boxes in a sling swing slowly from its block."""
    g = sc.still()
    g.box(OCHRE, (0, 30, -130), (300, 300, 300))
    g.box(STONE, (0, 30, 24), (270, 270, 8))
    top = (0.0, -90.0, 640.0)
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 115, 60, 20), (sx * 8, top[1], top[2]), 20, 20)
    tilt_box(g, WOOD_D, (0, 160, 20), (0, top[1] + 10, top[2] - 10), 14, 14)
    g.box(WOOD_D, (0, 14, 250), (150, 14, 14))
    g.box(WOOD_D, (0, top[1], top[2] + 6), (60, 30, 26))
    g.box(RED, (0, top[1], top[2] + 26), (34, 22, 14))
    g.cyl(WOOD, (56, 50, 105), 22, 112, rot=(90, 0, 0), steps=8)                  # the winch's roller, its cranks
    for sx in (-1, 1):
        g.box(WOOD_D, (sx * 78, 50, 105), (8, 10, 76))
        g.box(WOOD_D, (sx * 86, 50, 140), (22, 8, 8))
    tilt_box(g, ROPE, (0, 46, 122), (0, top[1] + 6, top[2] - 6), 5, 5)
    tilt_box(g, WOOD_D, (-96, 34, 130), (62, -22, 400), 10, 10)                   # the legs' cross braces
    tilt_box(g, WOOD_D, (96, 34, 130), (-62, -22, 400), 10, 10)
    for k, (x, y, z, col) in enumerate(((190, -40, 16, WHITE), (196, 30, 16, TEAL), (192, -6, 48, WHITE))):
        g.box(col, (x, y, z), (56, 84, 30))
    g.cyl(ROPE, (-190, -30, 0), 32, 14, steps=8)
    g.cyl(ROPE, (-190, -30, 14), 24, 12, steps=8)
    f = sc.part(P.motion(swing=16.0, pivot=(0, top[1], top[2] + 60), axis=P.UP, rate=0.3, phase=0.5))
    f.box(WOOD_D, (0, top[1], top[2] + 62), (6, 6, 60))
    f.box(TEAL, (40, top[1], top[2] + 74), (74, 5, 34))
    hook(sc, (top[0], top[1] - 4, top[2] - 14), 300.0, 'boxes', phase=0.3, swing=5.0, axis=90.0, rate=0.2)


def bollards(sc):
    """STONE MOORING POSTS (low, top 0.95 m): four on a strip of quay stones, rope looped from post to post, a coil
    and a ring at one end."""
    g = sc.still()
    g.box(OCHRE, (0, 0, -20), (660, 110, 60))
    for k in range(6):
        g.box(STONE, (-275 + k * 110, 0, 12), (100, 104, 6))
    xs = (-255, -85, 85, 255)
    for x in xs:
        g.cyl(OCHRE, (x, 0, 10), 22, 62, steps=8, top_r=18)
        g.cyl(STONE, (x, 0, 72), 28, 14, steps=8)
        g.cyl(OCHRE, (x, 0, 86), 16, 8, steps=8)
    for x0, x1 in zip(xs[:-1], xs[1:]):
        xm = (x0 + x1) / 2.0
        tilt_box(g, ROPE, (x0, -22, 58), (xm, -24, 34), 7, 7)
        tilt_box(g, ROPE, (xm, -24, 34), (x1, -22, 58), 7, 7)
    g.cyl(ROPE, (300, 20, 15), 30, 12, steps=8)
    g.cyl(ROPE, (300, 20, 27), 22, 10, steps=8)
    g.cyl(WOOD_D, (-310, 0, 15), 14, 14, steps=6)


def net_mast(sc):
    """NET-DRYING POLES (TALL, 5.6 m): three leaning poles lashed at the top, a yard across them, two nets hung from
    it that sway, cork floats along their feet, glass floats on a string."""
    g = sc.still()
    top = (10.0, 20.0, 550.0)
    for (x, y) in ((-170, -30), (175, -10), (0, 150)):
        tilt_box(g, WOOD_D, (x, y, -60), top, 13, 13)
    g.box(ROPE, (top[0], top[1], top[2] - 16), (30, 30, 22))
    g.box(WOOD_D, (0, -6, 400), (430, 10, 10))
    g.box(WOOD_D, (0, 74, 300), (250, 10, 10))
    net(sc, -200, -10, -6, 396, 250.0, phase=0.0)
    net(sc, 20, 205, -6, 396, 310.0, phase=0.35, col=CREAM)
    net(sc, -110, 110, 74, 296, 200.0, phase=0.6, floats=False)
    m = sc.part(P.motion(swing=7.0, pivot=(212, -6, 400), axis=0.0, rate=0.35, phase=0.2))
    m.box(ROPE, (212, -6, 365), (5, 5, 70))
    for k, col in enumerate((SKY, WHITE, SKY)):
        m.sphere(col, (212, -6, 318 - k * 30), 14, steps=(3, 5))
    g.box(WOOD, (-60, -110, 18), (110, 60, 36))                                   # a net tub at the foot
    g.box(NET, (-60, -110, 40), (96, 46, 12))
    g.cyl(ROPE, (110, -100, 0), 28, 46, steps=8, top_r=38)                        # wicker creels
    g.cyl(ROPE, (160, -60, 0), 24, 38, steps=8, top_r=32)
    g.box(WOOD, (40, 190, 40), (160, 34, 8))                                      # a mending bench, a shuttle on it
    for x in (-30, 110):
        g.box(WOOD_D, (x, 190, 18), (10, 30, 36))
    g.box(WHITE, (60, 190, 48), (40, 10, 8))


def quay_wall(sc):
    """STONE QUAY WALL (low, top 1.9 m): 8 m of ochre blocks with a cap course, steps down at one end, a timber
    mooring post, geraniums on the cap, a coil of rope."""
    g = sc.still()
    H = 110.0
    g.box(OCHRE, (-60, 0, (H - 60) / 2.0), (680, 120, H + 60))
    g.box(STONE, (-60, 0, H + 8), (696, 136, 16))
    for z in (36, 74):
        g.box(STONE, (-60, -62, z), (680, 8, 8))
    for k, x in enumerate((-320, -150, 20, 190)):
        g.box(STONE, (x + (k % 2) * 60, -62, 55 if k % 2 else 18), (8, 8, 36))
    for k in range(3):                                                            # the steps
        g.box(OCHRE, (340, 40 - k * 40, (H - k * 37 - 60) / 2.0), (120, 40, H - k * 37 + 60))
        g.box(STONE, (340, 40 - k * 40, H - k * 37 + 4), (124, 44, 8))
    g.cyl(WOOD_D, (-330, 10, H + 16), 13, 58, steps=6)
    g.cyl(WOOD_D, (-330, 10, H + 62), 17, 10, steps=6)
    tilt_box(g, ROPE, (-330, 10, H + 46), (-250, -66, H - 20), 6, 6)
    pot(g, -120, 20, H + 16)
    pot(g, 60, 26, H + 16)
    g.cyl(ROPE, (180, 10, H + 16), 30, 12, steps=8)
    g.box(TEAL, (-60, -64, 8), (680, 8, 16))                                      # the tide line
    g.box(WHITE, (-220, 0, H + 31), (84, 56, 30))                                 # a fish box on the cap, a net over the wall
    g.box(SKY, (-220, 0, H + 44), (70, 42, 8))
    g.box(NET, (130, -66, 74), (150, 8, 92))
    g.box(NET, (130, 0, H + 19), (150, 130, 6))
    for x in (-380, 250):
        disc_y(g, STONE, x + (60 if x < 0 else 0), -70, 56, 14, 10, steps=6)


# ------------------------------------------------------------------------------------------ the hero
def big_fish(sc):
    """THE BIG FISH: a market hall as a round, smiling, striped fish on a forest of stilts. 47 m from lip to tail and
    20 m tall (placed at 1.05). Teal and white bands, a cream belly, red fins; the door is in its open mouth, up a
    gangway that climbs along its flank; stalls under striped awnings stand between the stilts; its tail swings
    slowly, its side fins fan. Friendly and round: no teeth, no point, no grey."""
    g = sc.still()
    A, B, ZC = 2000.0, 640.0, 1060.0

    def r(x):
        return B * math.sqrt(max(0.0, 1.0 - (x / A) ** 2))

    xs = [1500, 1250, 1000, 750, 500, 250, 0, -250, -500, -750, -1000, -1250, -1500, -1700]
    for k in range(len(xs) - 1):
        frustum_x(g, TEAL if k % 2 == 0 else WHITE, xs[k], r(xs[k]), xs[k + 1], r(xs[k + 1]), 0, ZC, steps=14)
    RH = r(1500)
    HL = 1.12

    def rh(x):                                                  # the head's own radius at x (x >= 1500)
        return RH * math.sqrt(max(0.0, 1.0 - ((x - 1500) / (RH * HL)) ** 2))

    g.sphere(TEAL, (1500, 0, ZC), RH, scale=(HL, 1.0, 1.0), steps=(8, 14))        # the round head
    g.sphere(TEAL, (-1700, 0, ZC), r(-1700), steps=(5, 10))                       # the tail's root
    g.sphere(CREAM, (-80, 0, ZC - 110), 610, scale=(2.75, 1.0, 1.0), steps=(6, 14))   # the belly
    # the mouth: red lips round a dark doorway, a teal door, a landing
    MX, MZ = 1500 + RH * HL, ZC - 50
    frustum_x(g, RED, MX + 4, 235, MX - 70, 285, 0, MZ, steps=12)
    frustum_x(g, NAVY, MX + 12, 190, MX - 20, 190, 0, MZ, steps=12)
    g.box(TEAL, (MX + 14, 0, MZ - 60), (10, 150, 230))
    g.box(WHITE, (MX + 20, 40, MZ - 50), (6, 14, 14))
    LZ = MZ - 190.0
    g.box(WOOD, (MX + 60, -150, LZ - 8), (300, 620, 16))
    for (x, y) in ((MX + 190, -430), (MX + 190, 130), (MX - 60, -430)):
        g.box(WOOD_D, (x, y, (LZ - 400) / 2.0), (30, 30, LZ + 400))
    g.box(WHITE, (MX + 204, -150, LZ + 100), (8, 620, 8))
    for y in (-450, -300, -150, 0, 150):
        g.box(WHITE, (MX + 204, y, LZ + 50), (8, 8, 100))
    # the gangway: down the flank, towards the tail
    GY, GX0, GX1 = -560.0, MX - 90.0, 640.0
    tilt_box(g, WOOD, (GX0, GY, LZ - 6), (GX1, GY, -20), 240, 14)
    for sy in (-1, 1):
        tilt_box(g, WHITE, (GX0, GY + sy * 116, LZ + 96), (GX1, GY + sy * 116, 82), 8, 8)
    for k in range(6):
        f = k / 5.0
        x, z = GX0 + (GX1 - GX0) * f, LZ + (-20 - LZ) * f
        g.box(WOOD_D, (x, GY - 116, (z - 400) / 2.0 + 50), (18, 18, z + 400 + 100))
        g.box(WHITE, (x, GY + 116, z + 50), (8, 8, 100))
    # the eyes, the smile, the portholes
    for sy in (-1, 1):
        ex, ez = 1560.0, ZC + 190.0
        ey = math.sqrt(max(1.0, rh(ex) ** 2 - 190.0 ** 2))
        y0 = -ey - 34 if sy < 0 else ey - 30
        disc_y(g, WHITE, ex, y0, ez, 118, 64, steps=12)
        disc_y(g, NAVY, ex + 14, y0 - 12 if sy < 0 else y0 + 56, ez - 6, 60, 20, steps=10)
        g.box(WHITE, (ex + 34, (y0 - 16) if sy < 0 else (y0 + 80), ez + 20), (26, 10, 26))
        pts = [(MX - 120, -150), (1790, -250), (1640, -285), (1500, -250), (1400, -160)]
        for (xa, za), (xb, zb) in zip(pts[:-1], pts[1:]):
            ra = rh(xa) if xa >= 1500 else r(xa)
            rb = rh(xb) if xb >= 1500 else r(xb)
            ya = math.sqrt(max(1.0, ra ** 2 - za ** 2)) + 6
            yb = math.sqrt(max(1.0, rb ** 2 - zb ** 2)) + 6
            tilt_box(g, RED, (xa, sy * ya, ZC + za), (xb, sy * yb, ZC + zb), 30, 26)
        for k in (2, 4, 6, 8, 10):
            xm = (xs[k] + xs[k + 1]) / 2.0
            ra = (r(xs[k]) + r(xs[k + 1])) / 2.0
            ya = math.sqrt(max(1.0, ra ** 2 - 60.0 ** 2))
            disc_y(g, WHITE, xm, -ya - 14 if sy < 0 else ya - 26, ZC + 60, 96, 40, steps=10)
            disc_y(g, YEL, xm, -ya - 22 if sy < 0 else ya + 2, ZC + 60, 70, 20, steps=10, glow=0.35)
    # the fins
    side(g, RED, [(-950, ZC + 520), (760, ZC + 560), (420, ZC + 880), (-200, ZC + 960), (-780, ZC + 840)], -28, 28)
    for x in (-560, -120, 320):
        tilt_box(g, WHITE, (x + 60, 0, ZC + 600), (x - 60, 0, ZC + 860), 66, 22)
    for sy in (-1, 1):
        y = sy * 640.0
        m = sc.part(P.motion(swing=sy * 8.0, pivot=(260, sy * 600.0, ZC - 90), axis=0.0, rate=0.16, phase=0.25))
        side(m, RED, [(430, ZC - 70), (150, ZC - 100), (-290, ZC - 400), (40, ZC - 470), (400, ZC - 310)], y - 16, y + 16)
        tilt_box(m, WHITE, (300, y, ZC - 150), (-60, y, ZC - 400), 42, 20)
    t = sc.part(P.motion(swing=11.0, pivot=(-1720, 0, ZC), axis=P.UP, rate=0.1, phase=0.0))
    frustum_x(t, TEAL, -1700, r(-1700) - 20, -2060, 130, 0, ZC, steps=10)
    side(t, RED, [(-1990, ZC + 100), (-2480, ZC + 640), (-2610, ZC + 570), (-2370, ZC), (-2610, ZC - 570), (-2480, ZC - 640), (-1990, ZC - 100)], -26, 26)
    tilt_box(t, WHITE, (-2080, 0, ZC + 130), (-2470, 0, ZC + 540), 62, 22)
    tilt_box(t, WHITE, (-2080, 0, ZC - 130), (-2470, 0, ZC - 540), 62, 22)
    # the stilts and the stalls
    px = (-1450, -950, -450, 50, 550, 1050)
    for x in px:
        for y in (-340, 0, 340):
            zt = ZC - math.sqrt(max(1.0, r(x) ** 2 - y * y)) + 30 if abs(y) < r(x) else ZC
            g.box(WOOD_D, (x, y, (zt - 400) / 2.0), (40, 40, zt + 400))
    for k in range(len(px) - 1):
        z0, z1 = (20, 330) if k % 2 else (330, 20)
        tilt_box(g, WOOD_D, (px[k], 342, z0), (px[k + 1], 342, z1), 14, 18)
    for k, x in enumerate((-1200, -700, -200, 300)):
        g.box(WOOD, (x, -250, 45), (300, 110, 90))
        g.box(WHITE if k % 2 else TEAL, (x - 70, -250, 104), (84, 56, 28))
        g.box(TEAL if k % 2 else WHITE, (x + 60, -250, 104), (84, 56, 28))
        for sx in (-1, 1):
            g.box(WOOD_D, (x + sx * 150, -200, 130), (10, 10, 260))
        awning(g, x - 170, x + 170, -190, 262, depth=150, drop=46)


JOBS = [
    ('SM_S1D7_NetLoft', net_loft, MOVE), ('SM_S1D7_SailLoft', sail_loft, MOVE), ('SM_S1D7_Smokery', smokery, MOVE),
    ('SM_S1D7_BoatShed', boat_shed, MOVE), ('SM_S1D7_HarbourInn', harbour_inn, MOVE), ('SM_S1D7_IceHouse', ice_house, MOVE),
    ('SM_S1D7_TwinGables', twin_gables, MOVE),
    ('SM_S1D7_DryRack', dry_rack, SMALL), ('SM_S1D7_FishBoxes', fish_boxes, STILL), ('SM_S1D7_Capstan', capstan, STILL),
    ('SM_S1D7_NetBoat', net_boat, STILL), ('SM_S1D7_QuayCrane', quay_crane, MOVE), ('SM_S1D7_Bollards', bollards, STILL),
    ('SM_S1D7_NetMast', net_mast, MOVE), ('SM_S1D7_QuayWall', quay_wall, STILL),
    ('SM_S1D7_BigFish', big_fish, dict(grow=(60, 260, 40), grow_down=(60, 260, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
