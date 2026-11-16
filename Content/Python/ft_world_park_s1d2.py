"""Theme Park, Stage 1, builder s1d2: district 2 "Pirate Cove", a STILT HARBOUR (2.22 to 3.21 km).
/Game/World/ThemePark/SM_S1D2_*. Run: import ft_world_park_s1d2 as K; K.run()

Crooked plank houses on piles, every upper floor jutting over the one below, steep sagging shingle roofs, look-out
cupolas, shuttered windows lit gold. Seven buildings, four tall props, four low props and the hero, THE BOTTLE SHIP:
a bottle 50 m long lying in a cradle of rock and timber, a full-rigged ship rocking on a lit sea inside it.

Everything is an ft_people.Scene (one material, one section; movement in the vertex shader only). All of it is our
own: no brand, no real name, no words on any sign (the signs are pictures: a fish, an anchor, a chart).
Light: the buildings, the tall props and the hero carry a soft steady glow and steady lit windows and lanterns; they
stand 24 m and more from the road. The four LOW props (Pots, Capstan, RowBoat, Cannon) are unlit and still.
Nothing blinks. Placed by C:/ftwork/s1/L4_s1d2.inl.
"""
import math
import random
import ft_art
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'
WOOD, WOOD_D, TEAL, RED, CREAM, GOLD = '#9a6a44', '#6a4c36', '#2fc6c0', '#ff2d3c', '#fff1d6', '#ffc93c'
NAVY, PINE, ROCK, ROCK_D, TIN, TIN_D = '#2b2350', '#2f6b52', '#4a4466', '#3a3556', '#a8b4d0', '#7f8aa6'
WHITE, POOL, SPRAY, MAG, CYAN = '#fff6ea', '#3a5fa8', '#dff6ff', '#ff4fb8', '#2ee6ff'
AMB = 0.06            # the soft steady glow every colour of a lit piece carries (the park's people pieces: 0.05 to 0.1)
WIN, LAMP = 1.0, 1.3  # a lit window, a lantern
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SMALL = dict(grow=(30, 30, 30), grow_down=(30, 30, 0))


# ------------------------------------------------------------------------------------------ frames
def X(at=(0, 0, 0), lean=(0, 0, 0)):
    return ft_art._xf(at, lean)


def frame(g, at=(0, 0, 0), lean=(0, 0, 0)):
    """Everything drawn into g from now on stands in a frame of its own: at, leaning (pitch, yaw, roll) degrees."""
    g.base = X(at, lean)


def plain(g):
    g.base = X()


def place(at, lean, p):
    """Where the frame's point p is in the mesh's own space (Unreal's rotator: pitch, yaw, roll)."""
    pt, yw, rl = (math.radians(v) for v in lean)
    cp, sp, cy, sy, cr, sr = math.cos(pt), math.sin(pt), math.cos(yw), math.sin(yw), math.cos(rl), math.sin(rl)
    ax = (cp * cy, cp * sy, sp)
    ay = (sr * sp * cy - cr * sy, sr * sp * sy + cr * cy, -sr * cp)
    az = (-(cr * sp * cy + sr * sy), cy * sr - cr * sp * sy, cr * cp)
    return tuple(at[i] + p[0] * ax[i] + p[1] * ay[i] + p[2] * az[i] for i in range(3))


def _ccw(pts):
    """The polygon counter-clockwise, whichever way it was written (an extrusion written clockwise comes out inside out)."""
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def side(m, col, pts, y0, y1, **kw):
    m.side(col, _ccw(pts), y0, y1, **kw)


def plan(m, col, pts, z0, z1, **kw):
    m.plan(col, _ccw(pts), z0, z1, **kw)



def xcyl(g, col, x0, x1, y, z, r0, r1=None, steps=12, glow=0.0):
    """A cylinder or cone lying along X."""
    g.cyl(col, (x0, y, z), r0, x1 - x0, rot=(-90, 0, 0), steps=steps, glow=glow, top_r=r1)


def soft(fn, amb=AMB):
    """The piece fn draws, every unlit colour of it given the soft steady glow (pieces 24 m and more out only)."""
    def build(sc):
        fn(sc)
        for key in sc.order:
            model = sc.parts[key][0]
            model.meshes = dict(((col, glow if glow > 0.0 else amb), m) for (col, glow), m in model.meshes.items())
    return build


# ------------------------------------------------------------------------------------------ parts of a house
def piles(g, w, d, top, nx=3, ny=2, r=17.0, foot=-260.0, splay=16.0, seed=1, brace=True):
    """The piles a house stands on: none of them plumb. w, d: the footprint they carry; top: the deck's underside."""
    rr = random.Random(seed)
    for i in range(nx):
        for j in range(ny):
            x = -w / 2.0 + 30 + i * (w - 60.0) / max(nx - 1, 1)
            y = -d / 2.0 + 30 + j * (d - 60.0) / max(ny - 1, 1)
            rod(g, WOOD_D, (x + rr.uniform(-splay, splay), y + rr.uniform(-splay, splay), foot), (x, y, top), r, steps=5)
            if j == 0:
                g.cyl(CREAM, (x, y, top * 0.55), r + 3.0, 14.0, steps=5)            # a rope whipping
    if brace and top > 150.0:
        tilt_box(g, WOOD_D, (-w / 2.0 + 30, -d / 2.0 + 30, 15.0), (w / 2.0 - 30, -d / 2.0 + 30, top - 25.0), 11, 11)
        tilt_box(g, WOOD_D, (w / 2.0 - 30, -d / 2.0 + 30, 15.0), (-w / 2.0 + 30, -d / 2.0 + 30, top - 25.0), 11, 11)


def deck(g, cx, cy, z, w, d, t=22.0, ends=5):
    """A deck or a jutting floor: a dark slab, its beam ends showing along the front."""
    g.box(WOOD_D, (cx, cy, z + t / 2.0), (w, d, t))
    for k in range(ends):
        g.box(WOOD, (cx - w / 2.0 + (k + 0.5) * w / ends, cy - d / 2.0 - 7.0, z + t / 2.0 - 2.0), (16, 14, t + 6.0))
    return z + t


def rail(g, x0, x1, y, z, h=88.0, posts=4):
    """A rope rail along X."""
    for k in range(posts):
        x = x0 + k * (x1 - x0) / float(posts - 1)
        g.box(WOOD_D, (x, y, z + h / 2.0 + 4.0), (11, 11, h + 8.0))
    g.box(CREAM, ((x0 + x1) / 2.0, y, z + h - 8.0), (abs(x1 - x0), 5, 5))
    g.box(CREAM, ((x0 + x1) / 2.0, y, z + h * 0.5), (abs(x1 - x0), 5, 5))


def window(g, x, y, z, w=70.0, h=95.0, face='f', lit=True, shut=TEAL, box=False, simple=False):
    """A shuttered window on a wall: face 'f' (the -Y wall: y is the wall), 'b', 'l' (the -X wall: x is the wall), 'r'."""
    s = -1.0 if face in ('f', 'l') else 1.0

    def bx(col, a, b, c, sa, sb, sz, glow=0.0):       # a along the wall, b out of it, c up
        if face in ('f', 'b'):
            g.box(col, (x + a, y + s * b, z + c), (sa, sb, sz), glow=glow)
        else:
            g.box(col, (x + s * b, y + a, z + c), (sb, sa, sz), glow=glow)
    bx(WOOD_D, 0, 3, 0, w + 18, 8, h + 18)
    bx(GOLD if lit else NAVY, 0, 6, 0, w, 6, h, WIN if lit else 0.0)
    for k in (-1, 1):
        bx(shut, k * (w / 2.0 + 15), 5, 0, 24, 6, h + 6)
    if not simple:
        bx(WOOD_D, 0, 9, 0, 5, 4, h)
        bx(CREAM, 0, 9, -h / 2.0 - 11, w + 52, 18, 8)
    if box:                                           # a window box in flower
        bx(WOOD_D, 0, 20, -h / 2.0 - 26, w + 30, 22, 20)
        bx(PINE, 0, 20, -h / 2.0 - 12, w + 22, 18, 12)
        for k in (-1, 0, 1):
            bx((MAG, GOLD, RED)[k + 1], k * w * 0.36, 21, -h / 2.0 - 2, 14, 14, 12)


def door(g, x, y, z, w=92.0, h=192.0, col=RED):
    g.box(WOOD_D, (x, y - 3, z + h / 2.0), (w + 22, 8, h + 12))
    g.box(col, (x, y - 6, z + h / 2.0), (w, 6, h))
    g.box(GOLD, (x, y - 9, z + h * 0.76), (w * 0.42, 5, w * 0.34), glow=WIN)
    g.box(GOLD, (x + w * 0.32, y - 10, z + h * 0.46), (8, 6, 8))
    g.box(WOOD_D, (x, y - 26, z - 5), (w + 40, 50, 10))                     # the step


def storey(g, at, lean, w, d, h, wall=WOOD, wins=(), side_wins=(), doors=(), bands=2, boxes=False):
    """One floor of a plank house in a frame of its own: corner posts, plank lines, shuttered windows. wins: (x, z)
    on the front; side_wins: (side, y, z); doors: x on the front."""
    frame(g, at, lean)
    g.box(wall, (0, 0, h / 2.0), (w, d, h))
    for sx in (-1, 1):
        g.box(WOOD_D, (sx * (w / 2.0 - 7), -d / 2.0 - 4, h / 2.0), (15, 9, h))
    for k in range(bands):
        g.box(WOOD_D, (0, -d / 2.0 - 2.5, h * (k + 1.0) / (bands + 1.0)), (w - 30, 5, 5))
    for (x, z) in wins:
        window(g, x, -d / 2.0, z, box=boxes)
    for (sx, y, z) in side_wins:
        window(g, sx * w / 2.0, y, z, face='l' if sx < 0 else 'r', simple=True)
    for x in doors:
        door(g, x, -d / 2.0, 0.0)
    plain(g)


def hip(g, cx, cy, z, w, d, h, n=6, col=TEAL, top=0.2, sag=1.7):
    """A hipped shingle roof in courses: wide and flat at the eaves, steep at the top (it sags). Returns its top."""
    g.box(WOOD_D, (cx, cy, z + 5.0), (w + 16, d + 16, 10))
    for k in range(n):
        f = k / float(n)
        s = top + (1.0 - top) * (1.0 - f) ** sag
        g.box(col, (cx, cy, z + h * (f + 0.5 / n) + 5.0), (w * s, d * s, h / n + 1.0))
    return z + h + 5.0


def _sag(c, hw, z, rise, sag, dz=0.0, grow=0.0):
    w = hw + grow
    return [(c - w, z + dz), (c - w * 0.5, z + dz + rise * sag), (c, z + dz + rise), (c + w * 0.5, z + dz + rise * sag), (c + w, z + dz)]


def gable_front(g, x0, x1, yf, yb, z, rise, wall=CREAM, col=TEAL, sag=0.34, eave=38.0):
    """A steep sagging roof, its gable to the road, jutting 30 cm over the wall below. yf / yb: the wall's faces."""
    xm, hw = (x0 + x1) / 2.0, (x1 - x0) / 2.0 + eave
    side(g, WOOD_D, _sag(xm, hw, z, rise, sag, -15, 15), yf - 22, yb + 22)
    side(g, col, _sag(xm, hw, z, rise, sag), yf - 30, yb + 30)
    side(g, wall, _sag(xm, hw - 52, z + 4, rise - 70, sag), yf - 37, yf - 29)        # the gable's own wall
    for sx in (-1, 1):                                                                # barge boards
        tilt_box(g, WOOD_D, (xm + sx * hw, yf - 34, z + 6), (xm + sx * hw * 0.5, yf - 34, z + rise * sag + 8), 10, 16)
        tilt_box(g, WOOD_D, (xm + sx * hw * 0.5, yf - 34, z + rise * sag + 8), (xm, yf - 34, z + rise + 10), 10, 16)
    return z + rise


def gable_x(g, x0, x1, yf, yb, z, rise, col=TEAL, sag=0.34, eave=38.0):
    """The same roof with its ridge along X (the eaves to the road)."""
    ym, hw = (yf + yb) / 2.0, (yb - yf) / 2.0 + eave
    side(g, WOOD_D, _sag(ym, hw, z, rise, sag, -15, 15), -(x1 + 14), -(x0 - 14), rot=(0, 90, 0))
    side(g, col, _sag(ym, hw, z, rise, sag), -(x1 + 24), -(x0 - 24), rot=(0, 90, 0))
    return z + rise


def cupola(g, x, y, z, w=170.0, h=140.0, roof=120.0):
    """A look-out: a little lit room with a roof of its own. Returns its top."""
    g.box(WOOD_D, (x, y, z + 6), (w + 30, w + 30, 12))
    g.box(CREAM, (x, y, z + 12 + h / 2.0), (w, w, h))
    for sx, sy in ((0, -1), (0, 1), (-1, 0), (1, 0)):
        g.box(GOLD, (x + sx * (w / 2.0 + 1), y + sy * (w / 2.0 + 1), z + 12 + h * 0.55),
              (w * 0.62 if sy else 6, w * 0.62 if sx else 6, h * 0.56), glow=WIN)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WOOD_D, (x + sx * w / 2.0, y + sy * w / 2.0, z + 12 + h / 2.0), (12, 12, h))
    return hip(g, x, y, z + 12 + h, w + 70, w + 70, roof, n=4, top=0.12, sag=1.5)


def vane(sc, x, y, z, phase=0.0, fish=True, h=150.0):
    """A weather vane on a rod: it swings slowly round. z: the rod's foot."""
    g = sc.still()
    g.cyl(WOOD_D, (x, y, z), 5.0, h, steps=5)
    g.box(GOLD, (x, y, z + h * 0.55), (70, 5, 5))
    g.box(GOLD, (x, y, z + h * 0.55), (5, 70, 5))
    v = sc.part(P.motion(swing=38, pivot=(x, y, z), axis=P.UP, rate=0.11, phase=phase))
    if fish:
        side(v, GOLD, [(-60, 0), (-20, 22), (38, 10), (70, 30), (70, -30), (38, -10), (-20, -22)], y - 3, y + 3, loc=(x, 0, z + h + 18))
    else:
        v.box(GOLD, (x, y, z + h + 12), (130, 5, 8))
        side(v, GOLD, [(-80, 0), (-40, 26), (-40, -26)], y - 3, y + 3, loc=(x, 0, z + h + 12))
        side(v, GOLD, [(40, 30), (80, 30), (60, 0), (80, -30), (40, -30)], y - 3, y + 3, loc=(x, 0, z + h + 12))


def flag(sc, x, y, z, w=110.0, h=60.0, col=RED, phase=0.0, swing=16.0, rate=0.5, tail=True):
    """A flag at a mast's top (x, y, z = its upper inner corner): it swings about the mast."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + w * 0.3, y, z - h / 2.0), (w * 0.6, 5, h))
    if tail:
        f.box(col, (x + w * 0.8, y, z - h * 0.28), (w * 0.4, 5, h * 0.42))
        f.box(col, (x + w * 0.8, y, z - h * 0.78), (w * 0.4, 5, h * 0.42))
    else:
        f.box(col, (x + w * 0.8, y, z - h / 2.0), (w * 0.4, 5, h))


def lantern(g, x, y, z, s=1.0):
    """A ship's lantern hanging from z."""
    g.box(WOOD_D, (x, y, z - 4 * s), (26 * s, 26 * s, 8 * s))
    g.box(GOLD, (x, y, z - 22 * s), (18 * s, 18 * s, 28 * s), glow=LAMP)
    g.box(WOOD_D, (x, y, z - 39 * s), (24 * s, 24 * s, 6 * s))


def hang_lantern(sc, x, y, z, s=1.0, drop=40.0, phase=0.0, axis=90.0):
    """A lantern on a short chain: it swings a little."""
    m = sc.part(P.motion(swing=5, pivot=(x, y, z), axis=axis, rate=0.3, phase=phase))
    m.box(WOOD_D, (x, y, z - drop / 2.0), (5, 5, drop))
    lantern(m, x, y, z - drop, s)


def hang_sign(sc, x, y, z, picture='fish', phase=0.0, w=110.0, h=80.0):
    """A sign on a bracket out over the road side of a wall (x, y, z = the bracket's root on the wall): a board in
    the YZ plane with a picture and no words. It swings a little."""
    g = sc.still()
    g.box(WOOD_D, (x, y - 70, z), (10, 140, 10))
    tilt_box(g, WOOD_D, (x, y - 4, z - 60), (x, y - 100, z - 6), 8, 8)
    yc, zc = y - 80.0, z - 18.0 - h / 2.0
    m = sc.part(P.motion(swing=6, pivot=(x, yc, z), axis=90.0, rate=0.27, phase=phase))
    m.box(WOOD_D, (x, yc, z - 9), (5, 5, 18))
    m.box(WOOD_D, (x, yc, zc), (10, w + 12, h + 12))
    m.box(CREAM, (x, yc, zc), (14, w, h))
    for sx in (-1, 1):
        px = x + sx * 8.5
        if picture == 'fish':
            m.box(TEAL, (px, yc - 8, zc), (5, w * 0.5, h * 0.36))
            m.box(TEAL, (px, yc + w * 0.33, zc), (5, w * 0.16, h * 0.5))
            m.box(NAVY, (px + sx, yc - w * 0.2, zc + 5), (5, 6, 6))
        elif picture == 'anchor':
            m.box(NAVY, (px, yc, zc + 4), (5, 9, h * 0.66))
            m.box(NAVY, (px, yc, zc + h * 0.22), (5, w * 0.36, 8))
            m.box(NAVY, (px, yc, zc - h * 0.3), (5, w * 0.56, 9))
        else:                                                               # a chart: a dotted course to a cross
            for k in range(4):
                m.box(NAVY, (px, yc - w * 0.3 + k * w * 0.16, zc - h * 0.2 + (k % 2) * h * 0.16), (5, 8, 6))
            m.box(RED, (px, yc + w * 0.3, zc + h * 0.12), (5, 22, 6), rot=(0, 0, 45))
            m.box(RED, (px, yc + w * 0.3, zc + h * 0.12), (5, 22, 6), rot=(0, 0, -45))


def coil(g, x, y, z, r=38.0, n=3):
    for k in range(n):
        g.cyl(CREAM, (x, y, z + k * 11.0), r - k * 4.0, 11.0, steps=6)


def barrel(g, x, y, z, r=33.0, h=84.0):
    g.cyl(WOOD, (x, y, z), r, h, steps=6)
    g.cyl(WOOD_D, (x, y, z + h * 0.22), r + 2.0, 8.0, steps=6)
    g.cyl(WOOD_D, (x, y, z + h * 0.7), r + 2.0, 8.0, steps=6)


def tub(g, x, y, z, r=40.0, h=46.0, spikes=False, seed=1):
    """A half barrel planted up: flowers, or a spiky agave."""
    rr = random.Random(seed)
    g.cyl(WOOD, (x, y, z), r, h, steps=6)
    g.cyl(WOOD_D, (x, y, z + h * 0.5), r + 2.0, 8.0, steps=6)
    if spikes:
        for k in range(5):
            a = k * 1.2566 + rr.uniform(-0.3, 0.3)
            tilt_box(g, PINE, (x, y, z + h - 4), (x + math.cos(a) * r * 1.1, y + math.sin(a) * r * 1.1, z + h + 46), 12, 8)
        tilt_box(g, PINE, (x, y, z + h - 4), (x + 3, y, z + h + 62), 12, 8)
    else:
        g.sphere(PINE, (x, y, z + h + 8), r * 0.86, scale=(1, 1, 0.6), steps=(3, 6))
        for k in range(4):
            a = k * 1.5708 + 0.5
            g.box((MAG, GOLD, RED, WHITE)[k], (x + math.cos(a) * r * 0.5, y + math.sin(a) * r * 0.5, z + h + 24), (13, 13, 12))


def ladder(g, a, b, w=46.0, rungs=6):
    """A ladder from a to b (it runs in the YZ plane)."""
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (a[0] + sx * w / 2.0, a[1], a[2]), (b[0] + sx * w / 2.0, b[1], b[2]), 7, 7)
    for k in range(rungs):
        f = (k + 0.7) / (rungs + 0.4)
        g.box(WOOD, (a[0], a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f), (w, 6, 6))


def steps_down(g, x, y, z, n=3, w=120.0):
    """Plank steps from a deck's front edge (x, y, z) down towards the road."""
    for k in range(n):
        g.box(WOOD, (x, y - 22 - k * 34, z - 18 - k * 36), (w, 40, 12))
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (x + sx * w / 2.0, y, z - 8), (x + sx * w / 2.0, y - 14 - n * 34, z - 22 - n * 36), 9, 12)


def washing(sc, a, b, n=5, phase=0.0, seed=1, drop=58.0):
    """A washing line from a to b: the line is still, what hangs on it sways."""
    rr = random.Random(seed)
    g = sc.still()
    tilt_box(g, CREAM, a, b, 5, 5)
    m = sc.part(P.motion(swing=9, pivot=((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0), axis=0.0, rate=0.36, phase=phase))
    cols = (CREAM, RED, TEAL, WHITE, GOLD, CREAM)
    for k in range(n):
        f = (k + 0.7) / (n + 0.4)
        w, h = rr.uniform(44, 64), rr.uniform(drop * 0.7, drop * 1.25)
        m.box(cols[(k + seed) % len(cols)], (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f - h / 2.0 - 2), (w, 5, h))


def hull_plan(L, B):
    """A boat's outline from above: a pointed bow at +X, a transom at -X."""
    return [(-L, -B * 0.62), (-L * 0.55, -B), (L * 0.15, -B), (L * 0.62, -B * 0.66), (L, 0.0),
            (L * 0.62, B * 0.66), (L * 0.15, B), (-L * 0.55, B), (-L, B * 0.62)]


def hull(g, col, cx, cy, z, L, B, H, n=4, up=True, alt=None, yaw=0.0, glow=0.0):
    """A clinker hull in n planks. up: the right way up (widest at the gunwale); else upturned (the keel on top)."""
    for k in range(n):
        f = (k + 1.0) / n if up else 1.0 - k / float(n)
        s = 0.3 + 0.7 * f ** 0.75
        plan(g, alt if (alt and k % 2) else col, hull_plan(L * (0.78 + 0.22 * f), B * s), z + H * k / n, z + H * (k + 1.0) / n,
               glow=glow, loc=(cx, cy, 0), rot=(0, yaw, 0))


def chimney(g, x, y, z, h=190.0, pots=2):
    """A crooked plaster stack with red pots."""
    tilt_box(g, CREAM, (x, y, z), (x + 14, y, z + h), 46, 46)
    g.box(WOOD_D, (x + 14, y, z + h + 5), (62, 60, 10))
    for k in range(pots):
        g.cyl(RED, (x + 14 + (k - (pots - 1) / 2.0) * 26, y, z + h + 10), 10, 26, steps=5, top_r=8)


# ------------------------------------------------------------------------------------------ the buildings
def harbour_tower(sc):
    """BUILDING. The harbour master's leaning tower: three floors on six piles, each wider than the one below, a
    hipped roof, a look-out, a tide gauge, a spyglass on the gallery. 6.9 x 6.7 m, about 21 m tall."""
    g = sc.still()
    D0 = 260.0
    piles(g, 470, 470, D0, nx=3, ny=2, seed=11)
    z1 = deck(g, 0, -25, D0, 600, 620)
    rail(g, -290, 60, -325, z1, posts=4)
    steps_down(g, 190, -335, z1, n=4, w=110)
    L1 = (2.5, 0, 0)
    storey(g, (0, 10, z1), L1, 430, 430, 320, WOOD, wins=[(-110, 185)], side_wins=[(-1, 0, 185), (1, 0, 185)], doors=[105])
    z2 = deck(g, 4, -35, z1 + 318, 540, 560, t=24)
    L2 = (-2.0, 0, 1.5)
    storey(g, (10, -40, z2), L2, 490, 490, 300, CREAM, wins=[(-125, 160), (125, 160)], side_wins=[(-1, 0, 160), (1, 0, 160)], bands=1, boxes=True)
    frame(g, (10, -40, z2), L2)
    for sx in (-1, 1):                                                 # timber framing on the plaster
        tilt_box(g, WOOD_D, (sx * 30, -249, 20), (sx * 30, -249, 280), 6, 12)
    g.box(WOOD_D, (0, -249, 255), (460, 6, 12))
    plain(g)
    z3 = deck(g, 0, -75, z2 + 298, 620, 640, t=24)
    rail(g, -300, 300, -385, z3, h=80, posts=5)
    L3 = (3.0, 0, -1.0)
    storey(g, (-10, -60, z3), L3, 540, 520, 280, WOOD, wins=[(-150, 150), (150, 150)], side_wins=[(-1, 0, 150), (1, 0, 150)])
    tx, ty, tz = place((-10, -60, z3), L3, (0, 0, 280))
    top = hip(g, tx, ty, tz - 8, 690, 670, 430, n=7)
    ct = cupola(g, tx, ty, top - 150, 180, 150, 130)
    g.cyl(WOOD_D, (tx, ty, ct - 20), 7, 250, steps=5)
    g.sphere(GOLD, (tx, ty, ct + 236), 12, steps=(3, 5), glow=0.5)
    flag(sc, tx, ty, ct + 222, w=150, h=62, phase=0.2)
    vane(sc, tx - 170, ty + 40, top - 330, phase=0.4, fish=False, h=230)
    # the tide gauge: a striped board up the front left pile, a spyglass on the gallery
    for k in range(7):
        g.box(RED if k % 2 else CREAM, (-205, -229, -30 + k * 40), (22, 12, 40))
    g.box(WOOD_D, (150, -360, z3 + 50), (10, 10, 100))
    tilt_box(g, GOLD, (150, -330, z3 + 100), (150, -430, z3 + 128), 16, 16, glow=0.3)
    tilt_box(g, GOLD, (150, -430, z3 + 128), (150, -460, z3 + 136), 22, 22, glow=0.3)
    # a string of signal flags from the look-out down to the gallery's corner (our own colours: they spell nothing)
    a, b = (tx + 60, ty - 100, ct - 125), (300, -385, z3 + 84)
    tilt_box(g, CREAM, a, b, 5, 5)
    fl = sc.part(P.motion(swing=5, pivot=((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, (a[2] + b[2]) / 2.0), axis=0.0, rate=0.4, phase=0.6))
    for k in range(6):
        f = (k + 0.8) / 7.0
        fl.box((RED, CREAM, TEAL, GOLD, RED, TEAL)[k], (a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f - 26), (40, 5, 46))
    hang_lantern(sc, -230, -300, z2 - 24, 1.3, phase=0.1)
    hang_lantern(sc, 268, -380, z3 - 24, 1.3, phase=0.7)
    hang_sign(sc, -150, -205, z1 + 285, 'anchor', phase=0.3)
    coil(g, -210, -250, z1)
    barrel(g, -110, -270, z1)


def sail_loft(sc):
    """BUILDING. The sail loft: a long low loft on piles, its great door open on the lit floor, a plaster upper floor,
    a ridge along the quay, and three sails hoisted to dry on a yard over the roof (they sway). 12.4 x 6.6 m."""
    g = sc.still()
    D0 = 190.0
    piles(g, 1080, 470, D0, nx=4, ny=2, seed=21)
    z1 = deck(g, 0, -40, D0, 1190, 620, ends=6)
    rail(g, -590, -230, -345, z1, posts=3)
    rail(g, 230, 590, -345, z1, posts=3)
    steps_down(g, 0, -350, z1, n=3, w=260)
    L1 = (1.2, 0, 0)
    storey(g, (0, 10, z1), L1, 1060, 480, 310, WOOD, wins=[(-400, 180), (400, 180)], bands=3)
    frame(g, (0, 10, z1), L1)
    g.box(WOOD_D, (0, -243, 138), (310, 8, 276))                       # the great door, open
    g.box(GOLD, (0, -246, 132), (270, 6, 256), glow=0.7)
    for sx in (-1, 1):
        g.box(RED, (sx * 178, -250, 132), (78, 7, 256))
        tilt_box(g, CREAM, (sx * 145, -255, 14), (sx * 211, -255, 250), 5, 8)
        tilt_box(g, CREAM, (sx * 211, -255, 14), (sx * 145, -255, 250), 5, 8)
    g.box(CREAM, (-40, -262, 60), (150, 30, 90))                       # a bolt of canvas on the threshold
    g.box(CREAM, (60, -262, 34), (120, 40, 44), rot=(0, 12, 0))
    plain(g)
    z2 = deck(g, 0, -40, z1 + 308, 1200, 640, t=24, ends=6)
    L2 = (-1.5, 0, 0)
    storey(g, (0, -45, z2), L2, 1140, 540, 280, CREAM, wins=[(-430, 150), (-215, 150), (215, 150), (430, 150)],
           side_wins=[(-1, 0, 150), (1, 0, 150)], bands=1, boxes=True)
    frame(g, (0, -45, z2), L2)
    for x in (-320, -108, 108, 320):
        g.box(WOOD_D, (x, -273, 140), (12, 6, 280))
    g.box(TEAL, (0, -274, 150), (150, 8, 190))                         # the loading hatch under the hoist
    g.box(WOOD_D, (0, -278, 150), (6, 5, 190))
    ridge = gable_x(g, -570, 570, -270, 270, 276, 330)
    g.box(WOOD_D, (0, -360, ridge - 60), (18, 250, 18))                # the hoist beam
    for x in (-380, 380):                                              # the drying masts on the ridge
        g.cyl(WOOD_D, (x, 0, ridge - 40), 11, 520, steps=5)
        tilt_box(g, WOOD_D, (x, 0, ridge + 150), (x - (140 if x > 0 else -140), 0, ridge - 10), 7, 7)
    g.box(WOOD_D, (0, 0, ridge + 440), (980, 13, 13))
    g.sphere(GOLD, (-380, 0, ridge + 490), 11, steps=(3, 5), glow=0.5)
    plain(g)
    for k, (x, w, h, col) in enumerate(((-250, 230, 330, CREAM), (0, 250, 360, CREAM), (255, 220, 300, WHITE))):
        px = place((0, -45, z2), L2, (x, 0, ridge + 432))
        s = sc.part(P.motion(swing=5, pivot=px, axis=0.0, rate=0.24, phase=0.3 * k))
        s.box(col, (px[0], px[1], px[2] - h / 2.0), (w, 6, h))
        s.box(WOOD_D, (px[0], px[1] - 1, px[2] - h * 0.42), (w, 5, 5))
        s.box(WOOD_D, (px[0], px[1], px[2] - h - 3), (w + 16, 9, 9))
        if k == 1:
            s.box(RED, (px[0], px[1] - 1.5, px[2] - h * 0.72), (w * 0.5, 5, h * 0.22))
        if k == 2:
            s.box(TEAL, (px[0] + 40, px[1] - 1.5, px[2] - h * 0.3), (60, 5, 60))
    fp = place((0, -45, z2), L2, (380, 0, ridge + 478))
    flag(sc, fp[0], fp[1], fp[2], w=120, h=52, phase=0.5)
    hk = place((0, -45, z2), L2, (0, -455, ridge - 70))
    m = sc.part(P.motion(swing=4, pivot=hk, axis=90.0, rate=0.22, phase=0.1))  # a sail bundle on the hoist
    m.box(CREAM, (hk[0], hk[1], hk[2] - 60), (5, 5, 120))
    xcyl(m, CREAM, hk[0] - 110, hk[0] + 110, hk[1], hk[2] - 150, 34, steps=6)
    for x in (-60, 60):
        xcyl(m, WOOD_D, hk[0] + x - 5, hk[0] + x + 5, hk[1], hk[2] - 150, 36, steps=6)
    hang_lantern(sc, -230, -330, z2 - 28, 1.3, phase=0.4)
    hang_lantern(sc, 230, -330, z2 - 28, 1.3, phase=0.9)
    hang_sign(sc, 520, -232, z1 + 290, 'fish', phase=0.5)
    cz = place((0, -45, z2), L2, (-150, 0, ridge - 60))
    ct = cupola(g, cz[0], cz[1], cz[2], 150, 110, 100)
    vane(sc, cz[0], cz[1], ct - 10, phase=0.2, h=110)
    coil(g, -470, -280, z1, r=44, n=4)
    barrel(g, 480, -280, z1)


def chandler(sc):
    """BUILDING. The ship chandler's: a shop on a porch (a bow window full of light under a striped awning), a plaster
    floor over it and a great iron anchor on the gable. 9.0 x 6.6 m, about 11 m tall."""
    g = sc.still()
    D0 = 130.0
    piles(g, 800, 600, D0, nx=4, ny=2, seed=31, brace=False)
    z1 = deck(g, 0, -55, D0, 880, 650, ends=7)
    rail(g, -430, -150, -372, z1, posts=3)
    steps_down(g, -50, -378, z1, n=2, w=150)
    L1 = (-1.5, 0, 0)
    yf = -240.0
    storey(g, (0, 30, z1), L1, 760, 480, 300, WOOD, doors=[-250], side_wins=[(-1, 0, 175), (1, 0, 175)], bands=1)
    frame(g, (0, 30, z1), L1)
    g.box(WOOD_D, (130, yf - 16, 150), (360, 34, 190))                 # the bow window
    g.box(GOLD, (130, yf - 34, 152), (330, 6, 160), glow=WIN)
    for k in range(4):
        g.box(WOOD_D, (130 - 132 + k * 88, yf - 37, 152), (7, 5, 160))
    g.box(WOOD_D, (130, yf - 37, 152), (330, 5, 7))
    g.box(TEAL, (130, yf - 16, 250), (376, 44, 14))
    g.box(TEAL, (130, yf - 16, 52), (376, 44, 14))
    for k, (dx, c) in enumerate(((-110, TEAL), (-50, RED), (20, CREAM), (90, TEAL))):   # the wares: floats and blocks, small
        g.box(c, (130 + dx, yf - 30, 86 + (k % 2) * 8), (26, 10, 26 + (k % 2) * 16))
    for k in range(9):                                                 # the awning
        x = -340 + k * 85
        tilt_box(g, RED if k % 2 else CREAM, (x, yf - 2, 292), (x, yf - 150, 236), 85, 6)
        g.box(RED if k % 2 else CREAM, (x, yf - 151, 222), (85, 6, 26))
    for x in (-380, 380):
        g.box(WOOD_D, (x, yf - 146, 118), (9, 9, 236))
    plain(g)
    lantern(g, -250, 30 + yf - 60, z1 + 250, 1.1)
    z2 = deck(g, 0, -15, z1 + 298, 840, 580, t=24, ends=7)
    L2 = (2.0, 0, 0)
    storey(g, (0, -20, z2), L2, 820, 540, 270, CREAM, wins=[(-300, 145), (300, 145)], side_wins=[(-1, 0, 145), (1, 0, 145)], bands=1, boxes=True)
    frame(g, (0, -20, z2), L2)
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 150, -273, 10), (sx * 205, -273, 262), 6, 12)
    top = gable_front(g, -410, 410, -270, 270, 268, 400)
    # the anchor, 4.4 m of it, hung on the gable
    y = -318.0
    g.box(TIN, (0, y, 290), (30, 22, 430))
    g.box(WOOD, (0, y - 2, 452), (250, 24, 26))
    for k in range(8):
        a0, a1 = k * math.pi / 4.0, (k + 1) * math.pi / 4.0
        tilt_box(g, TIN_D, (math.cos(a0) * 40, y, 542 + math.sin(a0) * 40), (math.cos(a1) * 40, y, 542 + math.sin(a1) * 40), 16, 14)
    for sx in (-1, 1):
        tilt_box(g, TIN, (0, y, 84), (sx * 110, y, 104), 22, 30)
        tilt_box(g, TIN, (sx * 100, y, 100), (sx * 172, y, 190), 22, 28)
        side(g, TIN_D, [(sx * 158, 168), (sx * 136, 250), (sx * 222, 226)], y - 12, y + 12)
    tilt_box(g, CREAM, (26, y - 13, 520), (-60, y - 13, 330), 6, 6)   # its cable, round the shank
    tilt_box(g, CREAM, (-60, y - 13, 330), (20, y - 13, 180), 6, 6)
    plain(g)
    cx = place((0, -20, z2), L2, (250, 60, 268 + 100))
    chimney(g, cx[0], cx[1], cx[2], 260)
    rx = place((0, -20, z2), L2, (0, 120, top - 14))
    vane(sc, rx[0], rx[1], rx[2], phase=0.7, h=120)
    hang_sign(sc, 395, -292, z2 + 250, 'anchor', phase=0.8)
    coil(g, 330, -330, z1, r=42, n=4)
    coil(g, 250, -340, z1, r=30, n=2)
    barrel(g, 60, -335, z1)
    for k in range(3):                                                 # oars against the wall
        tilt_box(g, CREAM, (-330 - k * 22, -330, z1), (-330 - k * 22, -226, z1 + 250), 7, 7)
        tilt_box(g, TEAL if k != 1 else RED, (-330 - k * 22, -226, z1 + 250), (-330 - k * 22, -212, z1 + 300), 20, 7)
    tub(g, 400, -340, z1, r=30, h=36, seed=7)


def boat_shed(sc):
    """BUILDING. The boat shed: plank walls under an upturned hull for a roof (clinker planks, a red keel, a rudder,
    lit hatches), a skiff's bow on the slip in the open door. 11.9 x 6.6 m, about 9 m tall."""
    g = sc.still()
    D0 = 110.0
    piles(g, 1020, 500, D0, nx=4, ny=2, seed=41, brace=False)
    z1 = deck(g, 0, -40, D0, 1120, 620, ends=8)
    L1 = (1.0, 0, 0)
    storey(g, (0, 10, z1), L1, 1000, 500, 290, WOOD, wins=[(-370, 170), (370, 170)], side_wins=[(1, 0, 170)], bands=3)
    frame(g, (0, 10, z1), L1)
    g.box(WOOD_D, (0, -253, 130), (350, 8, 260))
    g.box(GOLD, (0, -256, 126), (310, 6, 244), glow=0.7)
    for sx in (-1, 1):
        g.box(RED, (sx * 200, -260, 126), (84, 7, 244))
        g.box(CREAM, (sx * 200, -265, 126), (84, 5, 8))
        g.box(CREAM, (sx * 200, -265, 200), (84, 5, 8))
    plain(g)
    hull(g, TEAL, 0, -250, z1 + 34, 170, 62, 62, n=3, alt=CREAM, yaw=-90)           # the skiff on the slip
    g.box(RED, (0, -250, z1 + 28), (14, 330, 14))
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 70, -200, z1 + 16), (sx * 70, -372, z1 - 44), 12, 12)
    # the roof: a hull turned over
    zr = z1 + 286.0
    hull(g, WOOD, -10, 0, zr, 620, 340, 300, n=6, up=False, alt=WOOD_D)
    plan(g, TEAL, hull_plan(628, 348), zr - 12, zr + 6, loc=(-10, 0, 0))                  # the gunwale: the eaves
    g.box(RED, (-30, 0, zr + 312), (1040, 24, 30))                                  # the keel
    tilt_box(g, RED, (480, 0, zr + 312), (620, 0, zr + 20), 24, 30)                 # the stem
    g.box(TEAL, (-585, 0, zr + 150), (16, 50, 290))                                 # the rudder, still shipped
    g.box(WOOD_D, (-570, 0, zr + 250), (60, 12, 12))
    for x in (-300, -40, 220):                                                      # hatches cut in the planking
        g.box(WOOD_D, (x, -278, zr + 86), (100, 60, 78))
        g.box(GOLD, (x, -310, zr + 86), (76, 6, 52), glow=WIN)
        tilt_box(g, TEAL, (x, -308, zr + 126), (x, -372, zr + 150), 92, 6)
    g.cyl(WOOD_D, (150, 0, zr + 300), 10, 300, steps=5)                             # a signal mast stepped on the keel
    g.box(WOOD_D, (150, 0, zr + 520), (230, 9, 9))
    lantern(g, 50, 0, zr + 518, 1.1)
    lantern(g, 250, 0, zr + 518, 1.1)
    flag(sc, 150, 0, zr + 598, w=120, h=50, phase=0.15)
    hang_sign(sc, -470, -240, z1 + 270, 'fish', phase=0.2)
    hang_lantern(sc, -205, -290, zr - 12, 1.2, phase=0.3)
    hang_lantern(sc, 205, -290, zr - 12, 1.2, phase=0.8)
    g.cyl(WOOD_D, (-420, -300, z1), 30, 50, steps=6)                                # the winch
    xcyl(g, WOOD, -460, -380, -300, z1 + 74, 24, steps=6)
    xcyl(g, CREAM, -445, -395, -300, z1 + 74, 30, steps=6)
    tilt_box(g, TIN_D, (-372, -300, z1 + 74), (-350, -300, z1 + 120), 8, 8)
    tilt_box(g, CREAM, (-420, -300, z1 + 66), (-70, -330, z1 + 30), 5, 5)
    coil(g, -300, -310, z1, r=34, n=2)
    tub(g, 480, -310, z1, r=34, h=40, spikes=True, seed=3)


def net_house(sc):
    """BUILDING. The net house: one room and a loft high up on six tall piles, nets hung out to dry on poles either
    side and under the deck (they sway), a ladder, a hoist with a basket, washing on a line. 9.6 x 6.7 m, about 15 m."""
    g = sc.still()
    D0 = 520.0
    piles(g, 600, 500, D0, nx=3, ny=2, r=20, seed=51)
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 270, -220, 30), (sx * 270, 220, D0 - 30), 11, 11)
    z1 = deck(g, 0, -35, D0, 720, 640, ends=6)
    rail(g, -350, 150, -350, z1, posts=5)
    ladder(g, (250, -440, -60), (250, -362, z1 + 60), rungs=11)
    L1 = (-2.0, 0, 0.8)
    storey(g, (0, 30, z1), L1, 540, 440, 290, WOOD, wins=[(140, 172)], side_wins=[(-1, 0, 172), (1, 0, 172)], doors=[-130])
    z2 = deck(g, 0, 0, z1 + 288, 620, 520, t=24)
    L2 = (2.5, 0, 0)
    storey(g, (0, -10, z2), L2, 600, 500, 190, WOOD, bands=1)
    frame(g, (0, -10, z2), L2)
    g.box(TEAL, (0, -254, 96), (130, 8, 140))                          # the loft hatch
    g.box(WOOD_D, (0, -258, 96), (6, 5, 140))
    top = gable_front(g, -300, 300, -250, 250, 188, 360, wall=WOOD)
    g.box(GOLD, (0, -287, 188 + 110), (70, 6, 70), glow=WIN)           # a lit bull's eye in the gable
    g.box(WOOD_D, (0, -290, 188 + 110), (70, 5, 6))
    g.box(WOOD_D, (0, -290, 188 + 110), (6, 5, 70))
    g.box(WOOD_D, (0, -350, top - 40), (16, 200, 16))                  # the hoist beam
    plain(g)
    hk = place((0, -10, z2), L2, (0, -430, top - 50))
    m = sc.part(P.motion(swing=5, pivot=hk, axis=90.0, rate=0.2, phase=0.3))
    m.box(CREAM, (hk[0], hk[1], hk[2] - 110), (5, 5, 220))
    m.cyl(WOOD, (hk[0], hk[1], hk[2] - 290), 44, 70, steps=6, top_r=52)             # a basket of the catch
    for k in range(3):
        m.box((SPRAY, TIN, SPRAY)[k], (hk[0] - 20 + k * 20, hk[1], hk[2] - 212), (14, 40, 20), rot=(0, 0, 20 - k * 20))
    # the net poles and their nets
    for sx, ph in ((-1, 0.0), (1, 0.45)):
        a, b = (sx * 330, -120, z1), (sx * 540, -160, z1 + 470)
        tilt_box(g, WOOD_D, a, b, 11, 11)
        tilt_box(g, CREAM, b, (sx * 300, -160, z1 + 560), 5, 5)
        n = sc.part(P.motion(swing=5, pivot=(sx * 420, -160, z1 + 500), axis=0.0, rate=0.21, phase=ph))
        side(n, CREAM, [(sx * 530, z1 + 462), (sx * 320, z1 + 545), (sx * 345, z1 + 250), (sx * 470, z1 + 170)], -164, -157)
        for k in range(3):
            n.box(WOOD_D, (sx * (350 + k * 55), -160, z1 + 370 - k * 20), (5, 9, 300 - k * 40))
        for k in range(4):
            n.box(TEAL if k % 2 else RED, (sx * (345 + k * 56), -160, z1 + 530 - k * 22), (20, 20, 20))
    u = sc.part(P.motion(swing=4, pivot=(0, -200, D0 - 20), axis=0.0, rate=0.18, phase=0.7))   # a net slung under the deck
    side(u, CREAM, [(-240, D0 - 24), (240, D0 - 24), (150, D0 - 230), (0, D0 - 280), (-150, D0 - 230)], -204, -197)
    for k in range(5):
        u.box(WOOD_D, (-180 + k * 90, -200, D0 - 130), (5, 9, 190 - abs(k - 2) * 40))
    for k in range(5):
        u.box(RED if k % 2 else TEAL, (-200 + k * 100, -200, D0 - 36), (22, 22, 22))
    washing(sc, (-355, -352, z1 + 190), (-150, -196, z1 + 262), n=4, phase=0.2, seed=5)
    g.box(WOOD_D, (-355, -352, z1 + 100), (10, 10, 200))
    cx = place((0, -10, z2), L2, (-170, 80, 188 + 100))
    chimney(g, cx[0], cx[1], cx[2], 250, pots=1)
    vx = place((0, -10, z2), L2, (0, 150, top - 12))
    vane(sc, vx[0], vx[1], vx[2], phase=0.9, h=120)
    hang_lantern(sc, -130, -250, z1 + 288, 1.2, phase=0.5)
    lantern(g, 345, -350, z1 + 150, 1.2)
    g.box(WOOD_D, (345, -350, z1 + 55), (10, 10, 110))
    barrel(g, 60, -290, z1)
    coil(g, -250, -280, z1)


def map_house(sc):
    """BUILDING. The chart maker's: plaster and timber, a balcony with a spyglass, and a great compass rose on the
    gable whose needle swings slowly. 8.8 x 6.7 m, about 13 m tall."""
    g = sc.still()
    D0 = 170.0
    piles(g, 720, 480, D0, nx=3, ny=2, seed=61)
    z1 = deck(g, 0, -40, D0, 840, 620, ends=7)
    rail(g, 60, 410, -345, z1, posts=4)
    steps_down(g, -180, -350, z1, n=3, w=140)
    L1 = (1.5, 0, 0)
    storey(g, (0, 20, z1), L1, 700, 460, 300, CREAM, wins=[(190, 175)], side_wins=[(1, 0, 175)], doors=[-180], bands=0, boxes=True)
    frame(g, (0, 20, z1), L1)
    for (xa, xb) in ((-340, -40), (-40, -340), (40, 90), (300, 340)):  # timber braces on the plaster
        tilt_box(g, WOOD_D, (xa, -233, 12), (xb, -233, 290), 6, 12)
    g.box(WOOD_D, (0, -233, 290), (700, 6, 14))
    plain(g)
    g.box(WOOD_D, (-30, -300, z1 + 40), (120, 46, 80))                 # a rack of rolled charts by the door
    for k in range(5):
        tilt_box(g, CREAM if k % 2 == 0 else WHITE, (-76 + k * 23, -300, z1 + 50), (-80 + k * 25, -292, z1 + 150 + (k % 3) * 18), 15, 15)
    for k in range(3):                                                 # surveyor's poles, striped
        x = 300 + k * 30
        for j in range(4):
            tilt_box(g, RED if j % 2 else CREAM, (x, -330 + j * 17, z1 + j * 64), (x, -313 + j * 17, z1 + 64 + j * 64), 9, 9)
    z2 = deck(g, 0, -55, z1 + 298, 800, 640, t=24, ends=7)
    rail(g, -150, 150, -368, z2, h=76, posts=4)
    L2 = (-2.0, 0, -1.0)
    storey(g, (0, -30, z2), L2, 760, 520, 280, WOOD, wins=[(-250, 150), (250, 150)], side_wins=[(-1, 0, 150), (1, 0, 150)])
    frame(g, (0, -30, z2), L2)
    g.box(WOOD_D, (0, -263, 96), (110, 8, 196))                        # the balcony door, lit
    g.box(GOLD, (0, -266, 96), (88, 6, 176), glow=WIN)
    g.box(WOOD_D, (0, -270, 96), (6, 5, 176))
    top = gable_front(g, -380, 380, -260, 260, 278, 470)
    # the compass rose
    y, zc = -299.0, 278 + 205.0
    g.cyl(WOOD_D, (0, y, zc), 172, 8, rot=(0, 0, -90), steps=12)
    g.cyl(CREAM, (0, y, zc), 150, 14, rot=(0, 0, -90), steps=12)
    for k in range(8):                                                 # four long points (north is red), four short gilt ones
        a = k * math.pi / 4.0
        L, w, col = (190.0, 26.0, RED if k == 2 else TEAL) if k % 2 == 0 else (118.0, 20.0, GOLD)
        c0 = (math.cos(a) * 16, y - 16, zc + math.sin(a) * 16)
        c1 = (math.cos(a) * L * 0.55, y - 16, zc + math.sin(a) * L * 0.55)
        c2 = (math.cos(a) * L, y - 16, zc + math.sin(a) * L)
        tilt_box(g, col, c0, c1, 8, w)
        tilt_box(g, col, c1, c2, 8, w * 0.45)
    plain(g)
    A, Ln = (0, -30, z2), L2
    hub = place(A, Ln, (0, y - 26, zc))
    nd = sc.part(P.motion(swing=24, pivot=hub, axis=90.0, rate=0.09, phase=0.3))    # the needle
    nd.box(NAVY, (hub[0], hub[1], hub[2] + 62), (16, 7, 124))
    nd.box(RED, (hub[0], hub[1], hub[2] - 62), (16, 7, 124))
    nd.cyl(GOLD, (hub[0], hub[1] + 4, hub[2]), 15, 12, rot=(0, 0, -90), steps=6, glow=0.4)
    # the spyglass on the balcony
    g.box(WOOD_D, (60, -350, z2 + 45), (9, 9, 90))
    tilt_box(g, GOLD, (60, -322, z2 + 92), (60, -420, z2 + 122), 15, 15, glow=0.3)
    tilt_box(g, GOLD, (60, -420, z2 + 122), (60, -450, z2 + 131), 21, 21, glow=0.3)
    cz = place(A, Ln, (0, 60, top - 150))
    ct = cupola(g, cz[0], cz[1], cz[2], 150, 120, 110)
    vane(sc, cz[0], cz[1], ct - 10, phase=0.55, fish=False, h=120)
    cx = place(A, Ln, (-260, 120, 278 + 90))
    chimney(g, cx[0], cx[1], cx[2], 270)
    hang_sign(sc, -330, -210, z1 + 288, 'chart', phase=0.6)
    hang_lantern(sc, -180, -300, z1 + 298, 1.2, phase=0.1)
    hang_lantern(sc, 330, -345, z2 - 30, 1.2, phase=0.6)
    tub(g, -370, -300, z1, r=32, h=38, spikes=True, seed=2)


def lookout_inn(sc):
    """BUILDING. The crooked inn: three floors leaning three ways, a gallery with the washing out, a great lantern on
    a carved bracket, a hipped roof, a look-out and a mast with a yard on top. 9.0 x 6.9 m, about 21 m tall."""
    g = sc.still()
    D0 = 150.0
    piles(g, 680, 480, D0, nx=3, ny=2, seed=71, brace=False)
    z1 = deck(g, 0, -45, D0, 780, 640, ends=4)
    steps_down(g, 250, -365, z1, n=3, w=130)
    rail(g, -380, 150, -360, z1, posts=3)
    L1 = (-2.0, 0, 0)
    storey(g, (0, 25, z1), L1, 640, 440, 300, WOOD, wins=[(-215, 172), (215, 172)], side_wins=[(1, 0, 172)], doors=[0])
    z2 = deck(g, 10, -60, z1 + 298, 800, 680, t=24, ends=5)
    rail(g, -385, 405, -392, z2, h=80, posts=5)
    L2 = (2.5, 0, 1.0)
    storey(g, (15, -10, z2), L2, 700, 500, 290, CREAM, wins=[(-230, 155), (0, 155), (230, 155)], side_wins=[(-1, 0, 155), (1, 0, 155)], bands=0)
    frame(g, (15, -10, z2), L2)
    for x in (-115, 115):
        g.box(WOOD_D, (x, -253, 145), (12, 6, 290))
    for sx in (-1, 1):
        tilt_box(g, WOOD_D, (sx * 340, -253, 12), (sx * 240, -253, 80), 6, 12)
    g.box(WOOD_D, (0, -253, 282), (700, 6, 14))
    plain(g)
    washing(sc, (-380, -392, z2 + 150), (-40, -392, z2 + 168), n=5, phase=0.1, seed=3)
    g.box(WOOD_D, (-380, -392, z2 + 120), (10, 10, 90))
    g.box(WOOD_D, (-40, -392, z2 + 125), (10, 10, 100))
    z3 = deck(g, -5, -50, z2 + 288, 800, 600, t=24, ends=5)
    L3 = (-3.0, 0, -1.0)
    storey(g, (-15, -45, z3), L3, 760, 540, 270, WOOD, wins=[(-230, 140), (230, 140)], side_wins=[(-1, 0, 140), (1, 0, 140)])
    tx, ty, tz = place((-15, -45, z3), L3, (0, 0, 270))
    top = hip(g, tx, ty, tz - 8, 900, 690, 400, n=7)
    ct = cupola(g, tx - 120, ty, top - 170, 170, 140, 110)
    tilt_box(g, GOLD, (tx - 120, ty - 86, ct - 190), (tx - 120, ty - 190, ct - 170), 13, 13, glow=0.3)   # a spyglass out of the look-out
    mz, mx = top - 200, tx + 130
    g.cyl(WOOD_D, (mx, ty, mz), 9, 500, steps=5)                       # a mast with a yard
    g.box(WOOD_D, (mx, ty, mz + 380), (300, 10, 10))
    for sx in (-1, 1):
        tilt_box(g, CREAM, (mx + sx * 150, ty, mz + 380), (mx, ty, mz + 490), 5, 5)
    lantern(g, mx - 130, ty, mz + 376, 1.0)
    lantern(g, mx + 130, ty, mz + 376, 1.0)
    flag(sc, mx, ty, mz + 498, w=150, h=64, phase=0.35)
    cx = place((-15, -45, z3), L3, (300, 120, 270 + 60))
    chimney(g, cx[0], cx[1], cx[2], 330, pots=2)
    # the great lantern on its bracket, over the door
    bz = z2 - 40.0
    g.box(WOOD_D, (0, -330, bz + 10), (16, 220, 16))
    tilt_box(g, WOOD_D, (0, -225, bz - 80), (0, -400, bz + 2), 12, 12)
    gl = sc.part(P.motion(swing=4, pivot=(0, -420, bz), axis=90.0, rate=0.24, phase=0.2))
    gl.box(WOOD_D, (0, -420, bz - 20), (6, 6, 40))
    lantern(gl, 0, -420, bz - 40, 2.6)
    hang_sign(sc, -300, -195, z1 + 286, 'fish', phase=0.75)
    # a hoist with a barrel on the left end of the gallery
    hx, hy, hz = -470.0, -200.0, z3 + 10.0
    g.box(WOOD_D, (-420, hy, hz), (190, 14, 14))
    hb = sc.part(P.motion(swing=4, pivot=(hx, hy, hz), axis=0.0, rate=0.19, phase=0.5))
    hb.box(CREAM, (hx, hy, hz - 150), (5, 5, 300))
    barrel(hb, hx, hy, hz - 384, r=34, h=86)
    tub(g, -300, -310, z1, seed=5)
    hang_lantern(sc, 380, -335, z3 - 24, 1.2, phase=0.85)


# ------------------------------------------------------------------------------------------ the tall props
def seahorse(g, x, y, z, s=1.0, col=TEAL):
    """A carved seahorse in profile (the XZ plane), z its lowest curl."""
    def p(pts):
        return [(x + a * s, z + b * s) for a, b in pts]
    body = [(-10, 0), (26, -8), (52, 12), (58, 46), (40, 70), (14, 66), (10, 50), (30, 46), (34, 28), (20, 16), (-4, 22), (-22, 50),
            (-30, 96), (-20, 150), (4, 196), (20, 236), (16, 268), (52, 262), (96, 244), (100, 260), (60, 286), (44, 312), (14, 322),
            (-18, 306), (-34, 268), (-40, 226), (-62, 180), (-76, 120), (-68, 56), (-44, 14)]
    side(g, col, p(body), y - 26 * s, y + 26 * s)
    side(g, GOLD, p([(-34, 268), (-18, 306), (14, 322), (-6, 350), (-30, 330), (-52, 336), (-50, 300)]), y - 6 * s, y + 6 * s)     # the crest
    side(g, GOLD, p([(-76, 120), (-62, 180), (-40, 226), (-84, 222), (-110, 176), (-106, 132)]), y - 6 * s, y + 6 * s)               # the fin
    for k in range(4):                                                                                                               # the belly's ridges
        g.box(CREAM, (x + (6 - k * 9) * s, y, z + (176 - k * 34) * s), (22 * s, 58 * s, 9 * s))
    for sy in (-1, 1):
        g.box(NAVY, (x + 18 * s, y + sy * 27 * s, z + 290 * s), (12 * s, 5, 12 * s))


def figurehead(sc):
    """TALL PROP. A ship's figurehead set up on a post: a carved seahorse, gilt crest and fin, on a scrolled bracket,
    a lantern either side. 2.4 x 1.4 m, about 6.4 m tall."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -120), 70, 150, steps=7, top_r=58)
    g.cyl(WOOD_D, (0, 0, 20), 24, 250, steps=6, top_r=19)
    g.cyl(CREAM, (0, 0, 60), 27, 16, steps=6)
    g.cyl(CREAM, (0, 0, 84), 26, 10, steps=6)
    tilt_box(g, WOOD_D, (0, 0, 180), (-64, 0, 268), 18, 30)            # the scroll under it
    tilt_box(g, WOOD_D, (0, 0, 180), (64, 0, 268), 18, 30)
    g.box(WOOD, (0, 0, 276), (170, 46, 16))
    g.box(GOLD, (0, -24, 276), (150, 5, 6))
    seahorse(g, -6, 0, 292, 1.02)
    g.box(RED, (-38, 0, 480), (74, 60, 14))                            # a collar, like a coat's
    g.box(GOLD, (-6, -31, 480), (8, 5, 8))
    g.box(WOOD_D, (0, 0, 236), (250, 9, 9))                            # the lantern yard
    hang_lantern(sc, -112, 0, 232, 1.1, drop=26, phase=0.2, axis=90.0)
    hang_lantern(sc, 112, 0, 232, 1.1, drop=26, phase=0.7, axis=90.0)
    coil(g, 60, -34, 30, r=26, n=2)


def signal_mast(sc):
    """TALL PROP. The harbour's signal mast: a yard and a gaff, two hoists of flags (our own colours, they spell
    nothing) that stir, a long pennant, lanterns at the yard arms. 4.6 x 1.6 m, about 14 m tall."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -120), 96, 150, steps=8, top_r=80)
    g.cyl(WOOD_D, (0, 0, 30), 52, 60, steps=8, top_r=40)
    g.cyl(WOOD, (0, 0, 30), 15, 930, steps=6, top_r=11)
    g.cyl(WOOD, (0, 0, 900), 10, 440, steps=5, top_r=6)
    g.box(WOOD_D, (0, 0, 935), (70, 30, 12))
    g.box(WOOD_D, (0, 0, 800), (440, 12, 12))
    tilt_box(g, WOOD_D, (0, 0, 620), (150, 0, 720), 9, 9)
    for sx in (-1, 1):
        tilt_box(g, CREAM, (sx * 215, 0, 800), (0, 0, 1010), 5, 5)
        tilt_box(g, CREAM, (sx * 215, 0, 800), (sx * 62, 0, 96), 5, 5)
        lantern(g, sx * 206, 0, 792, 1.1)
        g.box(WOOD_D, (sx * 62, 0, 92), (10, 10, 10))
    cols = ((RED, CREAM, TEAL, GOLD, RED), (TEAL, GOLD, RED, CREAM, TEAL))
    for i, sx in enumerate((-1, 1)):
        m = sc.part(P.motion(swing=6, pivot=(0, 0, 450), axis=P.UP, rate=0.42, phase=0.5 * i))
        for k in range(5):
            f = (k + 0.6) / 5.6
            x, z = sx * (62 + (215 - 62) * f), 96 + (800 - 96) * f
            m.box(cols[i][k], (x + sx * 4, 0, z), (58, 5, 50), rot=(-sx * 12, 0, 0))
            m.box(cols[i][(k + 2) % 5], (x + sx * 4, 0, z), (20, 6, 20), rot=(-sx * 12, 0, 0))
    flag(sc, 0, 0, 1330, w=230, h=44, phase=0.3, swing=20, rate=0.45)
    g.sphere(GOLD, (0, 0, 1346), 11, steps=(3, 5), glow=0.5)
    coil(g, -66, -38, 30, r=30, n=3)
    g.box(WOOD_D, (60, -50, 60), (56, 10, 60))                          # a cleat board
    g.box(TIN_D, (60, -57, 70), (40, 6, 8))


def fish_rack(sc):
    """TALL PROP. A fish-drying rack: two trestle ends, two poles of split fish that swing a little, the day's
    best catch hung by the tail beside a striped measuring pole. 5.2 x 1.3 m, about 3.9 m tall."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, WOOD_D, (sx * 205, sy * 56, -80), (sx * 190, 0, 318), 11, 11)
        g.box(WOOD_D, (sx * 198, 0, 120), (9, 96, 9))
    g.box(WOOD, (0, 0, 312), (450, 10, 10))
    g.box(WOOD, (0, 0, 214), (440, 10, 10))
    fishes = [(-150, SPRAY, 62), (-98, TIN, 54), (-50, SPRAY, 66), (0, CREAM, 58), (52, TIN, 64), (102, SPRAY, 52), (150, CREAM, 60)]
    for i, zt in enumerate((308.0, 210.0)):
        m = sc.part(P.motion(swing=7, pivot=(0, 0, zt), axis=0.0, rate=0.3, phase=0.4 * i))
        for k, (x, col, L) in enumerate(fishes):
            if i == 1 and k % 2 == 0:
                continue
            x += i * 22
            side(m, col, [(x - 5, zt), (x + 5, zt), (x + 9, zt - 16), (x + 15, zt - L * 0.5), (x + 6, zt - L), (x + 15, zt - L - 16),
                         (x - 15, zt - L - 16), (x - 6, zt - L), (x - 15, zt - L * 0.5), (x - 9, zt - 16)], -5, 5)
    # the prize catch on a gallows at the right end, the measuring pole beside it
    g.cyl(WOOD_D, (252, 0, -80), 10, 470, steps=5)
    g.box(WOOD_D, (222, 0, 380), (90, 10, 10))
    b = sc.part(P.motion(swing=4, pivot=(196, 0, 376), axis=0.0, rate=0.2, phase=0.6))
    b.box(CREAM, (196, 0, 362), (5, 5, 28))
    side(b, TEAL, [(180, 350), (212, 350), (204, 322), (222, 250), (216, 180), (196, 140), (176, 180), (170, 250), (188, 322)], -11, 11)
    side(b, CREAM, [(196, 150), (212, 190), (214, 240), (196, 250), (178, 240), (180, 190)], -13, 13)
    b.box(NAVY, (196, -12, 172), (9, 5, 9))
    for j in range(7):
        g.box(RED if j % 2 else CREAM, (252, -13, 20 + j * 46), (16, 8, 46))
    g.cyl(WOOD, (-120, 0, 0), 46, 52, steps=7)                         # the salting tub
    g.cyl(WOOD_D, (-120, 0, 26), 48, 8, steps=7)
    g.cyl(WHITE, (-120, 0, 48), 40, 8, steps=7)
    coil(g, 40, 10, 0, r=30, n=2)


def wheel_arch(sc):
    """TALL PROP. A gate of two crooked ship's timbers with a ship's wheel hung in it, turning slowly a little each
    way; lanterns, rope swags, picture finger boards (a fish, an anchor, a wave), two tubs of agave.
    5.4 x 1.5 m, about 6.6 m tall."""
    g = sc.still()
    for sx in (-1, 1):
        pts = [(sx * 235, -120), (sx * 228, 180), (sx * 196, 380), (sx * 120, 540), (0, 610)]
        for a, b in zip(pts[:-1], pts[1:]):
            tilt_box(g, WOOD, (a[0], 0, a[1]), (b[0], 0, b[1]), 30, 34)
        g.cyl(CREAM, (sx * 232, 0, 96), 24, 14, steps=6)
        g.cyl(CREAM, (sx * 232, 0, 118), 24, 10, steps=6)
        tub(g, sx * 236, -52, 0, r=30, h=36, spikes=True, seed=3 + sx)
        tilt_box(g, CREAM, (sx * 196, -6, 380), (sx * 90, -6, 330), 6, 6)        # rope swags
        tilt_box(g, CREAM, (sx * 90, -6, 330), (0, -6, 350), 6, 6)
    g.box(WOOD_D, (0, 0, 612), (150, 40, 40))
    g.box(GOLD, (0, -22, 612), (60, 5, 22), glow=0.4)
    g.cyl(WOOD_D, (0, 0, 630), 7, 30, steps=5)
    g.sphere(GOLD, (0, 0, 668), 12, steps=(3, 5), glow=0.5)
    hang_lantern(sc, -150, 0, 470, 1.2, drop=30, phase=0.2, axis=90.0)
    hang_lantern(sc, 150, 0, 470, 1.2, drop=30, phase=0.6, axis=90.0)
    # the wheel
    hz = 440.0
    g.box(WOOD_D, (0, 0, 560), (12, 12, 110))
    g.box(WOOD_D, (0, 8, hz), (16, 24, 16))
    w = sc.part(P.motion(swing=40, pivot=(0, 0, hz), axis=90.0, rate=0.07, phase=0.0))
    w.cyl(GOLD, (0, 6, hz), 20, 14, rot=(0, 0, -90), steps=8, glow=0.3)
    for k in range(8):
        a0, a1 = k * math.pi / 4.0, (k + 1) * math.pi / 4.0
        tilt_box(w, WOOD, (math.cos(a0) * 84, 0, hz + math.sin(a0) * 84), (math.cos(a1) * 84, 0, hz + math.sin(a1) * 84), 12, 14)
        tilt_box(w, WOOD_D, (math.cos(a0) * 16, 0, hz + math.sin(a0) * 16), (math.cos(a0) * 118, 0, hz + math.sin(a0) * 118), 8, 8)
        w.box(GOLD, (math.cos(a0) * 118, 0, hz + math.sin(a0) * 118), (11, 11, 11))
    # finger boards on the left timber: pictures, no words
    for k, (col, z, sx) in enumerate(((TEAL, 250, -1), (RED, 200, 1), (CREAM, 150, -1))):
        x = -232 + sx * 62
        side(g, col, [(x - 52, z - 16), (x + 34, z - 16), (x + 56, z), (x + 34, z + 16), (x - 52, z + 16)] if sx > 0 else
               [(x + 52, z - 16), (x - 34, z - 16), (x - 56, z), (x - 34, z + 16), (x + 52, z + 16)], -24, -18)
        g.box(NAVY if col != RED else CREAM, (x, -27, z), (36 - k * 6, 5, 8 + k * 3))
        g.box(NAVY if col != RED else CREAM, (x + (k - 1) * 8, -27, z), (8, 5, 20 - k * 4))


# ------------------------------------------------------------------------------------------ the low props (unlit, still)
def lobster_pots(sc):
    """LOW PROP. A stack of wicker lobster pots (round-backed, three, two and one), their rope and glass floats, a
    marker stick. 3.3 x 1.0 m, 1.65 m high."""
    g = sc.still()
    g.box(WOOD_D, (-10, 0, -31), (300, 96, 78))
    R, Lp = 30.0, 70.0
    rows = (((-96, -24, 48), 1), ((-60, 12), 2), ((-24,), 3))
    for xs, row in rows:
        z = 8 + R + (row - 1) * 52.0
        for x in xs:
            g.cyl(WOOD, (x, -Lp / 2.0, z), R, Lp, rot=(0, 0, 90), steps=6)
            g.cyl(WOOD_D, (x, -Lp / 2.0 - 3, z), R + 2, 7, rot=(0, 0, 90), steps=6)
            g.cyl(WOOD_D, (x, Lp / 2.0 - 4, z), R + 2, 7, rot=(0, 0, 90), steps=6)
            g.cyl(NAVY, (x, -Lp / 2.0 - 5, z), 11, 5, rot=(0, 0, 90), steps=5)
    coil(g, 106, 4, 8, r=30, n=3)
    for k, (x, y) in enumerate(((112, 4), (146, -24), (78, -30))):
        g.sphere(TEAL if k != 1 else CREAM, (x, y, 8 + 13 + (33 if k == 0 else 0)), 14, steps=(3, 5))
    g.box(WOOD_D, (150, 22, 78), (7, 7, 140))
    g.box(RED, (136, 22, 136), (28, 5, 22))


def capstan(sc):
    """LOW PROP. A capstan on its round platform, six bars shipped, a hawser coiled down beside it and made fast to
    a bollard. 3.5 x 1.9 m, 1.25 m high."""
    g = sc.still()
    g.cyl(WOOD_D, (-70, 0, -70), 94, 84, steps=8)
    g.cyl(WOOD, (-70, 0, 14), 40, 74, steps=8, top_r=30)
    g.cyl(WOOD_D, (-70, 0, 88), 44, 22, steps=8)
    g.cyl(WOOD, (-70, 0, 110), 30, 10, steps=8, top_r=18)
    for k in range(6):
        a = k * math.pi / 3.0 + 0.3
        tilt_box(g, WOOD, (-70 + math.cos(a) * 30, math.sin(a) * 30, 99), (-70 + math.cos(a) * 92, math.sin(a) * 92, 99), 9, 9)
    for k in range(4):
        g.cyl(CREAM, (-70, 0, 30 + k * 11), 42 - k, 9, steps=8)
    g.box(WOOD_D, (95, 0, -31), (150, 150, 78))
    coil(g, 70, 10, 8, r=52, n=4)
    g.cyl(WOOD_D, (140, -30, 8), 15, 62, steps=6, top_r=19)
    g.cyl(CREAM, (140, -30, 34), 20, 12, steps=6)
    tilt_box(g, CREAM, (-32, 6, 40), (30, 10, 46), 6, 6)
    tilt_box(g, CREAM, (100, -4, 46), (136, -26, 40), 6, 6)


def row_boat(sc):
    """LOW PROP. A rowing boat hauled out on two trestles for painting: clinker planks, thwarts, the oars laid in
    her, a paint pot and a brush. 5.3 x 1.8 m, 1.5 m high."""
    g = sc.still()
    for x in (-140, 130):
        for sy in (-1, 1):
            tilt_box(g, WOOD_D, (x, sy * 80, -70), (x, sy * 26, 64), 10, 12)
        g.box(WOOD_D, (x, 0, 60), (14, 150, 12))
    hull(g, TEAL, 0, 0, 66, 250, 84, 66, n=4, alt=CREAM)
    plan(g, RED, hull_plan(253, 87), 130, 138)
    plan(g, WOOD_D, hull_plan(236, 72), 134, 140)
    g.box(RED, (0, 0, 62), (470, 10, 10))
    for x in (-150, -30, 100):
        g.box(WOOD, (x, 0, 140), (24, 146, 6))
    for sy in (-1, 1):
        tilt_box(g, CREAM, (-190, sy * 30, 144), (210, sy * 44, 150), 7, 7)
        tilt_box(g, RED, (210, sy * 44, 150), (262, sy * 46, 150), 20, 6)
    g.cyl(TIN, (-232, 60, -70), 13, 96, steps=6)                       # a paint pot on a post
    g.cyl(TIN, (-232, 60, 26), 16, 22, steps=6)
    g.cyl(TEAL, (-232, 60, 48), 14, 4, steps=6)
    tilt_box(g, WOOD, (-232, 60, 50), (-216, 52, 76), 5, 5)


def cannon(sc):
    """LOW PROP. An old ship's gun on a stepped wooden carriage, pointing AWAY from the road, spiked long ago: a
    posy grows out of the muzzle and a rope makes it fast to a ring post. 2.9 x 1.9 m, 1.2 m high."""
    g = sc.still()
    g.box(WOOD_D, (0, 0, -31), (230, 180, 78))
    for sx in (-1, 1):
        g.box(WOOD, (sx * 38, 0, 34), (16, 150, 36))
        g.box(WOOD, (sx * 38, 22, 60), (16, 96, 20))
        for y in (-48, 48):
            xcyl(g, WOOD_D, sx * 48 - 6, sx * 48 + 6, y, 26, 18, steps=7)
    g.box(WOOD_D, (0, -56, 28), (76, 12, 12))
    g.box(WOOD_D, (0, 44, 28), (76, 12, 12))
    a, b = (0, -74, 62), (0, 76, 92)
    rod(g, NAVY, a, b, 21, steps=8)
    rod(g, TIN_D, (0, -80, 61), (0, -66, 64), 25, steps=8)
    rod(g, TIN_D, (0, 20, 81), (0, 30, 83), 24, steps=8)
    rod(g, TIN_D, (0, 64, 90), (0, 80, 93), 25, steps=8)
    g.sphere(NAVY, (0, -86, 60), 11, steps=(3, 5))
    xcyl(g, TIN_D, -50, 50, 6, 74, 8, steps=5)                         # the trunnions
    g.sphere(PINE, (0, 88, 100), 21, steps=(3, 6))                     # the posy
    for k, col in enumerate((MAG, GOLD, WHITE, MAG)):
        g.box(col, (-12 + k * 8, 92 + (k % 2) * 6, 114 + (k % 2) * 5), (10, 10, 10))
    g.cyl(WOOD_D, (96, -60, -70), 11, 150, steps=6)
    g.cyl(TIN_D, (96, -60, 80), 14, 6, steps=6)
    tilt_box(g, CREAM, (96, -60, 62), (44, -40, 50), 6, 6)
    tub(g, -98, 40, 8, r=28, h=32, seed=6)


# ------------------------------------------------------------------------------------------ the hero
def bottle_ship(sc):
    """HERO. THE BOTTLE SHIP: a bottle 50 m long lying in a cradle of rock and timber, corked and sealed, a
    full-rigged ship rocking on a softly lit sea inside it. The glass is drawn as its far half, its hoops and its
    highlights, so the ship shows. A stilt landing with lanterns before it. 50.7 x 19 m, about 21 m tall."""
    g = sc.still()
    ZC, R = 1150.0, 850.0
    XA, XB = -2500.0, 1100.0
    GL = 0.12

    def ring(m, x, r, t0, t1, n, col, w, d, glow=0.0):
        for k in range(n):
            a0 = math.radians(t0 + (t1 - t0) * k / float(n))
            a1 = math.radians(t0 + (t1 - t0) * (k + 1.0) / n)
            tilt_box(m, col, (x, math.cos(a0) * r, ZC + math.sin(a0) * r), (x, math.cos(a1) * r, ZC + math.sin(a1) * r), w, d, glow=glow)

    # the far half of the glass and its belly under the sea (t: degrees from +Y, the far side, up through the top)
    for k in range(7):
        t = -90.0 + (k + 0.5) * 180.0 / 7.0
        g.box(POOL, ((XA + XB) / 2.0, math.cos(math.radians(t)) * R, ZC + math.sin(math.radians(t)) * R), (XB - XA, 26, 396), rot=(0, 0, -t), glow=GL)
    for k in range(3):
        t = 206.5 + (k + 0.5) * 63.5 / 3.0
        g.box(POOL, ((XA + XB) / 2.0, math.cos(math.radians(t)) * R, ZC + math.sin(math.radians(t)) * R), (XB - XA, 26, 330), rot=(0, 0, -t), glow=GL)
    xcyl(g, POOL, XA - 40, XA, 0, ZC, R + 8, steps=14, glow=GL)                     # the bottle's foot
    xcyl(g, SPRAY, XA - 46, XA - 38, 0, ZC, R * 0.5, steps=10, glow=0.2)
    xcyl(g, POOL, XB, XB + 520, 0, ZC, R + 8, 262, steps=14, glow=GL)               # the shoulder
    xcyl(g, POOL, XB + 500, XB + 1200, 0, ZC, 260, steps=12, glow=GL)               # the neck
    xcyl(g, SPRAY, XB + 1180, XB + 1270, 0, ZC, 322, steps=12, glow=0.2)            # the lip
    xcyl(g, WOOD, XB + 1250, XB + 1420, 0, ZC, 214, 236, steps=10)                  # the cork
    xcyl(g, RED, XB + 1150, XB + 1190, 0, ZC, 336, steps=12)                        # sealing wax round the lip, a run of it
    g.box(RED, (XB + 1170, -250, ZC - 250), (60, 60, 190))
    g.box(RED, (XB + 1170, -120, ZC - 330), (50, 50, 110))
    # hoops and highlights: the near half of the glass
    for x in (XA + 20, -1600, -700, 200, XB - 20):
        ring(g, x, R, 90.0, 270.0, 7, SPRAY, 44, 30, glow=0.22)
    for t, x0, x1 in ((128.0, -2300, 300), (141.0, -1500, 900), (240.0, -2200, -200)):
        a, rg = math.radians(t), R + (12 if t > 200 else -6)
        g.box(WHITE, ((x0 + x1) / 2.0, math.cos(a) * rg, ZC + math.sin(a) * rg), (x1 - x0, 30, 30), glow=0.3)
    # the sea inside
    ZS = ZC - 380.0
    g.box(CYAN, ((XA + XB) / 2.0, 0, ZS - 30), (XB - XA - 30, 1440, 60), glow=0.28)
    wv = sc.part(P.motion(bob=-14, rate=0.22, phase=0.0))
    wv2 = sc.part(P.motion(bob=-14, rate=0.22, phase=0.5))
    rr = random.Random(77)
    for k in range(8):
        x, y = XA + 300 + k * 430 + rr.uniform(-60, 60), rr.uniform(-520, 420)
        if abs(x + 700) < 900 and abs(y) < 260:
            y = -480 if k % 2 else 420
        (wv if k % 2 else wv2).box(SPRAY, (x, y, ZS + 6), (rr.uniform(130, 220), 46, 22), glow=0.3)
    # the ship: she rocks about her own length
    SX = -700.0
    s = sc.part(P.motion(swing=5, pivot=(SX, 0, ZS), axis=0.0, rate=0.2, phase=0.1))
    hull(s, WOOD, SX, 0, ZS - 30, 830, 205, 250, n=5, alt=WOOD_D)
    zd = ZS + 220.0
    plan(s, RED, hull_plan(836, 211), zd - 26, zd, loc=(SX, 0, 0))
    plan(s, CREAM, hull_plan(800, 186), zd, zd + 8, loc=(SX, 0, 0))
    s.box(WOOD, (SX - 640, 0, zd + 70), (330, 330, 140))                           # the stern castle
    s.box(RED, (SX - 640, 0, zd + 146), (350, 346, 14))
    s.box(WOOD, (SX + 560, 0, zd + 45), (240, 230, 90))                            # the forecastle
    s.box(RED, (SX + 560, 0, zd + 94), (256, 244, 10))
    for k in range(3):
        s.box(GOLD, (SX - 808, -100 + k * 100, zd + 80), (8, 60, 60), glow=WIN)
    for sy in (-1, 1):
        for k in range(3):
            s.box(GOLD, (SX - 720 + k * 90, sy * 167, zd + 80), (46, 8, 50), glow=WIN)
        for k in range(3):
            s.box(NAVY, (SX - 240 + k * 200, sy * 194, zd - 70), (50, 30, 40))
    lantern(s, SX - 830, 0, zd + 250, 3.0)
    s.box(WOOD_D, (SX - 820, 0, zd + 190), (14, 14, 120))
    tilt_box(s, WOOD_D, (SX + 640, 0, zd + 60), (SX + 1130, 0, zd + 250), 22, 22)   # the bowsprit
    masts = ((-430.0, 700.0, 0.84), (0.0, 880.0, 1.0), (400.0, 790.0, 0.92))
    for i, (mx, mh, f) in enumerate(masts):
        x = SX + mx
        s.cyl(WOOD_D, (x, 0, zd), 15, mh, steps=5, top_r=9)
        z = zd + 110.0
        for j, (w, h) in enumerate(((470, 250), (390, 220), (290, 180))):
            w, h = w * f, h * f
            s.box(WOOD_D, (x, 0, z + h + 6), (14, w + 50, 14), rot=(0, 34, 0))
            s.box(CREAM if (i + j) % 3 else WHITE, (x + 10, 0, z + h / 2.0), (12, w, h), rot=(0, 34, 0))
            z += h + 26.0
        s.box(RED, (x - 62, 0, zd + mh - 24), (120, 8, 44))                         # her pennants stream aft
        s.box(RED, (x - 150, 0, zd + mh - 16), (70, 8, 22))
    side(s, CREAM, [(SX + 440, zd + 130), (SX + 1090, zd + 240), (SX + 440, zd + 660)], -5, 5)      # the jibs
    side(s, WHITE, [(SX + 470, zd + 250), (SX + 900, zd + 250), (SX + 470, zd + 560)], 18, 28)
    side(s, CREAM, [(SX - 440, zd + 170), (SX - 800, zd + 220), (SX - 720, zd + 520), (SX - 440, zd + 470)], -5, 5)   # the spanker
    tilt_box(s, WOOD_D, (SX - 430, 0, zd + 480), (SX - 740, 0, zd + 540), 12, 12)
    tilt_box(s, WOOD_D, (SX - 430, 0, zd + 165), (SX - 820, 0, zd + 215), 12, 12)
    for (xa, za, xb, zb) in ((400, 790, 1130, 250), (0, 880, 400, 790), (-430, 700, 0, 880), (-430, 700, -800, 150)):
        tilt_box(s, WOOD_D, (SX + xa, 0, zd + za), (SX + xb, 0, zd + zb), 6, 6)
    # the cradle: two timber saddles lashed with rope, on a reef of rock
    for cx in (-1900.0, 500.0):
        ring(g, cx, R + 44, 200.0, 340.0, 6, WOOD, 170, 70)
        ring(g, cx, R + 20, 60.0, 290.0, 8, CREAM, 36, 22)
        for sy in (-1, 1):
            rod(g, WOOD_D, (cx + 30, sy * 900, -300), (cx, sy * 640, ZC - 600), 46, steps=6)
            rod(g, WOOD_D, (cx - 30, sy * 420, -300), (cx, sy * 300, ZC - 840), 46, steps=6)
            tilt_box(g, WOOD_D, (cx, sy * 880, 120), (cx, sy * 60, 300), 40, 40)
        g.box(WOOD_D, (cx, 0, 260), (60, 1700, 50))
    for (x, y, z, r, sx, sy, sz, col) in ((-1900, 120, -40, 620, 1.3, 1.25, 0.56, ROCK), (500, 80, -40, 600, 1.35, 1.3, 0.55, ROCK),
                                          (-700, 260, -60, 560, 1.9, 1.1, 0.42, ROCK_D), (-2500, -300, -60, 420, 1.0, 1.2, 0.6, ROCK_D),
                                          (1500, 200, -60, 520, 1.2, 1.2, 0.62, ROCK_D), (1250, -520, -50, 300, 1.2, 1.0, 0.6, ROCK)):
        g.sphere(col, (x, y, z), r, scale=(sx, sy, sz), steps=(4, 7))
    rod(g, WOOD_D, (2050, 0, -300), (2050, 0, ZC - 262), 40, steps=6)               # a prop under the neck
    g.box(WOOD_D, (2050, 0, ZC - 280), (120, 420, 40))
    # the tag on the neck: a picture, no words
    tg = sc.part(P.motion(swing=5, pivot=(2000, -262, ZC), axis=0.0, rate=0.17, phase=0.4))
    tg.box(CREAM, (2000, -268, ZC - 150), (6, 6, 300))
    tg.box(CREAM, (2000, -276, ZC - 480), (300, 12, 400))
    tg.box(RED, (2000, -284, ZC - 360), (240, 6, 16))
    tg.box(NAVY, (2000, -284, ZC - 490), (22, 6, 170))
    tg.box(NAVY, (2000, -284, ZC - 440), (110, 6, 20))
    tg.box(NAVY, (2000, -284, ZC - 566), (150, 6, 22))
    g.cyl(CREAM, (2000, 0, ZC), 268, 30, rot=(-90, 0, 0), steps=12)
    # the landing: a stilt jetty along the front, steps, lanterns on posts, a mast with a pennant by the neck
    zl = 300.0
    for k in range(4):
        for y in (-1090, -960):
            rod(g, WOOD_D, (-1500 + k * 870 + (16 if k % 2 else -14), y, -300), (-1500 + k * 870, y, zl), 24, steps=5)
    g.box(WOOD_D, (-200, -1025, zl + 12), (2800, 190, 24))
    for k in range(9):
        g.box(WOOD, (-1540 + k * 335, -1125, zl + 10), (26, 16, 30))
    for k in range(5):
        x = -1580 + k * 690
        g.box(WOOD_D, (x, -1112, zl + 70), (14, 14, 100))
    g.box(CREAM, (-200, -1112, zl + 108), (2780, 6, 6))
    g.box(CREAM, (-200, -1112, zl + 66), (2780, 6, 6))
    for k in range(4):
        g.box(WOOD, (1290 + k * 70, -1025, zl - 30 - k * 70), (80, 180, 14))
    for x in (-1500, -200, 1100):
        g.box(WOOD_D, (x, -940, zl + 170), (16, 16, 300))
        g.box(WOOD_D, (x, -990, zl + 316), (12, 110, 12))
        lantern(g, x, -1034, zl + 312, 2.0)
    g.cyl(WOOD_D, (1800, -700, -300), 20, 1900, steps=6, top_r=12)
    g.sphere(GOLD, (1800, -700, 1612), 22, steps=(3, 5), glow=0.5)
    flag(sc, 1800, -700, 1590, w=420, h=110, phase=0.3, swing=14, rate=0.36)
    tilt_box(g, CREAM, (1800, -700, 1300), (2300, -240, ZC + 180), 8, 8)


JOBS = [
    ('SM_S1D2_HarbourTower', soft(harbour_tower)), ('SM_S1D2_SailLoft', soft(sail_loft)), ('SM_S1D2_Chandler', soft(chandler)),
    ('SM_S1D2_BoatShed', soft(boat_shed)), ('SM_S1D2_NetHouse', soft(net_house)), ('SM_S1D2_MapHouse', soft(map_house)),
    ('SM_S1D2_LookoutInn', soft(lookout_inn)),
    ('SM_S1D2_Figurehead', soft(figurehead), SMALL), ('SM_S1D2_SignalMast', soft(signal_mast)),
    ('SM_S1D2_FishRack', soft(fish_rack), SMALL), ('SM_S1D2_WheelArch', soft(wheel_arch), SMALL),
    ('SM_S1D2_Pots', lobster_pots, STILL), ('SM_S1D2_Capstan', capstan, STILL), ('SM_S1D2_RowBoat', row_boat, STILL),
    ('SM_S1D2_Cannon', cannon, STILL),
    ('SM_S1D2_BottleShip', soft(bottle_ship, 0.05), dict(grow=(120, 120, 60), grow_down=(120, 120, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
