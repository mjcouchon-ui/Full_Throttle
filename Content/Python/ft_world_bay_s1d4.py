"""Bayshore, Stage 1, builder s1d4: HOTEL ROW (9.8 to 13.2 km). /Game/World/Bayshore/SM_S1D4_*.
Run in the editor: import ft_world_bay_s1d4 as K; K.run()

The grand resort: mint palaces with gold domes. Everything in ARCHES: arcades at the ground, rows of round-headed
windows with balconies, hipped violet roofs, corner turrets under small gold domes, porte-cocheres on paired columns.
Palette (the zone plan's): mint walls, white columns and balconies, gold domes and rails, deep violet windows and
roofs, peach ONLY as awnings and parasols. Water is the kit's pool turquoise; a little leaf green on one roof garden.

  buildings  Casino, Ballroom, PalmCourt, RoofGarden, Gatehouse, GrandHotel, Belvedere, Loggia, Rotunda (town side),
             SpaHouse, BathPavilion (bay side, on the upper beach)
  props      Fountain, Bandstand, Flagpoles, Valet, Lamp (tall: 24 m and more), Balustrade, Trolley, TeaTables (low)
  hero       Shell: a scallop shell 42 m tall behind a round dance floor, a curved colonnade, a giant pearl, a fountain

Every mesh is an ft_people.Scene (one material, one section). What moves, moves in the vertex shader: flags and
pennants, a fountain's water, steam over the spa's pool, washing on two lines, a parasol, two weather vanes.
Glow (steady, never blinking) only on pieces that stand 36 m and more from the road. Placed by C:\\ftwork\\s1\\L0_s1d4.inl.
"""
import math
import ft_art
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Bayshore'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))   # a piece with no moving part: true bounds

MINT, WHITE, GOLD, VIOLET, PEACH = '#7fe3c4', '#fffaf2', '#e2b45c', '#3a2f6a', '#ffc59e'
CREAM, POOL, LEAF, YEL, WOOD = '#fff1d6', '#49e2d5', '#2fbf71', '#ffe14d', '#c08a5a'
FOOT = 300.0                                        # how far a building's walls go down under z = 0


# ------------------------------------------------------------------------------------------ the words of the kit
def _ccw(pts):
    """The polygon counter-clockwise, whichever way it was written (an extrusion written clockwise is inside out)."""
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


class Frame(object):
    """with Frame(g, at, yaw): what is drawn into g inside is moved there and turned (yaw 90: the model's front
    becomes its +X side; yaw -90: its -X side; 180: its back)."""

    def __init__(self, g, at=(0.0, 0.0, 0.0), yaw=0.0):
        self.g = g
        self.t = ft_art._xf(at, (0.0, yaw, 0.0), (1.0, 1.0, 1.0))
        self.old = None

    def __enter__(self):
        self.old = self.g.base
        self.g.base = self.t if self.old is None else self.t * self.old
        return self.g

    def __exit__(self, *a):
        self.g.base = self.old
        return False


def spread(n, w, margin=0.0):
    """n places evenly along a wall w wide, margin left bare at each end."""
    return [-w / 2.0 + margin + (w - 2.0 * margin) * (k + 0.5) / n for k in range(n)]


def arch_pts(xc, z0, w, h, n=3):
    r = w / 2.0
    zs = z0 + h - r
    pts = [(xc - r, z0), (xc + r, z0), (xc + r, zs)]
    for k in range(1, n + 1):
        a = math.pi * k / (n + 1.0)
        pts.append((xc + math.cos(a) * r, zs + math.sin(a) * r))
    pts.append((xc - r, zs))
    return pts


def arch(g, col, xc, y, z0, w, h, t=8.0, n=3, glow=0.0, rot=(0, 0, 0)):
    """A round-headed opening on the wall y (facing -Y), t proud of it."""
    g.side(col, arch_pts(xc, z0, w, h, n), y - t, y + 2.0, glow=glow, rot=rot)


def wins(g, xs, y, z0, w, h, col=VIOLET, t=8.0):
    for x in xs:
        arch(g, col, x, y, z0, w, h, t)


def arcade(g, col, x0, x1, y, z0, h, n, t=30.0, pier=0.26, spring=0.6, k=3):
    """A wall of n open arches from x0 to x1, z0 to z0 + h, one piece (a comb): the white of every colonnade here."""
    bay = (x1 - x0) / float(n)
    pw = bay * pier
    r = (bay - pw) / 2.0
    zs = min(z0 + h * spring, z0 + h - r - 30.0)
    pts = [(x0, z0)]
    for i in range(n):
        xl = x0 + i * bay + pw / 2.0
        pts.append((xl, z0))
        pts.append((xl, zs))
        for j in range(1, k + 1):
            a = math.pi * (1.0 - j / (k + 1.0))
            pts.append((xl + r + math.cos(a) * r, zs + math.sin(a) * r))
        pts.append((xl + 2.0 * r, zs))
        pts.append((xl + 2.0 * r, z0))
    pts += [(x1, z0), (x1, z0 + h), (x0, z0 + h)]
    g.side(col, _ccw(pts), y - t, y + 2.0)


def balcony(g, x0, x1, y, z, depth=60.0):
    """A balcony along the wall y: a white slab and parapet, a gold rail."""
    L, xc = x1 - x0, (x0 + x1) / 2.0
    g.box(WHITE, (xc, y - depth / 2.0, z), (L, depth, 12.0))
    g.box(WHITE, (xc, y - depth + 4.0, z + 26.0), (L, 6.0, 40.0))
    g.box(GOLD, (xc, y - depth + 4.0, z + 50.0), (L + 8.0, 9.0, 8.0))


def awning(g, x0, x1, y, z, depth=120.0, drop=50.0, n=3, cols=(PEACH, WHITE)):
    """A canvas awning hung on the wall y at z, falling towards the road, in stripes."""
    w = (x1 - x0) / float(n)
    roll = -math.degrees(math.atan2(drop, depth))
    for k in range(n):
        g.box(cols[k % len(cols)], (x0 + (k + 0.5) * w, y - depth / 2.0, z - drop / 2.0), (w, math.hypot(depth, drop), 10.0), rot=(0, 0, roll))


def hip(g, col, cx, cy, z, w, d, h, top=0.0):
    """A hipped roof on a w x d rectangle: two four-sided cones and the prism between them. top: how much of the
    short side is left flat on top (0 = a ridge)."""
    top = max(top, 0.02)
    if w >= d:
        r, L = d / 2.0, w - d
        for s in (-1, 1):
            g.cyl(col, (cx + s * L / 2.0, cy, z), r * 1.41421, h, rot=(0, 45, 0), steps=4, top_r=r * top * 1.41421)
        if L > 1.0:
            g.side(col, [(cy - r, z), (cy + r, z), (cy + r * top, z + h), (cy - r * top, z + h)], -(cx + L / 2.0), -(cx - L / 2.0), rot=(0, 90, 0))
    else:
        r, L = w / 2.0, d - w
        for s in (-1, 1):
            g.cyl(col, (cx, cy + s * L / 2.0, z), r * 1.41421, h, rot=(0, 45, 0), steps=4, top_r=r * top * 1.41421)
        g.side(col, [(cx - r, z), (cx + r, z), (cx + r * top, z + h), (cx - r * top, z + h)], cy - L / 2.0, cy + L / 2.0)


def dome(g, x, y, z, r, col=GOLD, squash=1.0, steps=(4, 8), finial=True):
    """A dome sunk to its middle at z, and its finial. Returns the top."""
    g.sphere(col, (x, y, z), r, scale=(1, 1, squash), steps=steps)
    top = z + r * squash
    if finial:
        g.cyl(col, (x, y, top - 8.0), max(r * 0.07, 5.0), r * 0.45, steps=5)
        g.sphere(col, (x, y, top + r * 0.45), max(r * 0.13, 9.0), steps=(3, 5))
        top += r * 0.45 + max(r * 0.13, 9.0)
    return top


def flag(sc, g, x, y, z, col, w=120.0, h=64.0, pole=220.0, phase=0.0, rate=0.35, swing=15.0):
    """A plain flag turning on a pole whose foot is at (x, y, z). Draw the pole into g (not inside a Frame)."""
    g.cyl(WHITE, (x, y, z), 5.0, pole, steps=5)
    g.sphere(GOLD, (x, y, z + pole + 8.0), 10.0, steps=(3, 5))
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z + pole - h / 2.0), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + w / 2.0 + 6.0, y, z + pole - h / 2.0 - 4.0), (w, 4.0, h))


def vane(sc, g, x, y, z, phase=0.0):
    """A weather vane: a small gold shell (a fan of three leaves, not an arrow) turning slowly on its rod."""
    g.cyl(GOLD, (x, y, z), 4.0, 110.0, steps=5)
    g.box(GOLD, (x, y, z + 40.0), (70.0, 5.0, 5.0))
    g.box(GOLD, (x, y, z + 40.0), (5.0, 70.0, 5.0))
    v = sc.part(P.motion(swing=50.0, pivot=(x, y, z + 110.0), axis=P.UP, rate=0.06, phase=phase))
    for a in (-32.0, 0.0, 32.0):
        tilt_box(v, GOLD, (x + 6.0, y, z + 96.0), (x + 6.0 + math.cos(math.radians(a)) * 70.0, y, z + 96.0 + math.sin(math.radians(a)) * 70.0), 5.0, 20.0)
    v.box(GOLD, (x - 30.0, y, z + 96.0), (40.0, 5.0, 12.0))


def urn(g, x, y, z, s=1.0):
    """A white garden urn, a gold ball in it. 71 s tall."""
    g.cyl(WHITE, (x, y, z), 12.0 * s, 14.0 * s, steps=6)
    g.cyl(WHITE, (x, y, z + 14.0 * s), 9.0 * s, 34.0 * s, steps=6, top_r=26.0 * s)
    g.sphere(GOLD, (x, y, z + 56.0 * s), 15.0 * s, steps=(3, 6))


def lantern(g, x, y, z, glow=0.7):
    """A carriage lamp on a wall bracket (steady glow: only on pieces 24 m and more from the road)."""
    g.box(GOLD, (x, y - 8.0, z), (10.0, 22.0, 8.0))
    g.sphere(YEL, (x, y - 16.0, z + 18.0), 13.0, steps=(3, 5), glow=glow)
    g.box(GOLD, (x, y - 16.0, z + 33.0), (16.0, 16.0, 5.0))


def vent(g, x, y, z):
    g.cyl(WHITE, (x, y, z), 16.0, 50.0, steps=6)
    g.cyl(VIOLET, (x, y, z + 50.0), 26.0, 14.0, steps=6, top_r=8.0)


def chimney(g, x, y, z, h=130.0):
    g.box(WHITE, (x, y, z + h / 2.0), (46.0, 46.0, h))
    g.box(VIOLET, (x, y, z + h + 6.0), (60.0, 60.0, 12.0))


def badge(g, x, y, z, r, col=WHITE):
    """The Row's sign without words: a scallop shell, five leaves fanned from a gold boss, on the wall y."""
    for k in range(-2, 3):
        a = math.radians(k * 28.0)
        tilt_box(g, col, (x, y, z), (x + math.sin(a) * r, y, z + math.cos(a) * r), 6.0, r * 0.24)
    g.box(GOLD, (x, y - 2.0, z), (r * 0.42, 8.0, r * 0.3))


def body(g, w, d, h, col=MINT, plinth=70.0):
    """Walls w x d x h on a cream plinth, FOOT of footing."""
    g.box(col, (0, 0, (h - FOOT) / 2.0), (w, d, h + FOOT))
    g.box(CREAM, (0, 0, (plinth - FOOT) / 2.0), (w + 24.0, d + 24.0, plinth + FOOT))


def flank(g, w, d, rows, n=3, ww=120.0, wh=200.0, margin=90.0, skip_front=0.0):
    """Plain windows on both side walls of a w x d body. rows: the sills' heights. skip_front: cm of each wall's road
    end left bare (a turret stands there)."""
    for yaw in (90.0, -90.0):
        with Frame(g, yaw=yaw):
            for z in rows:
                for x in spread(n, d - skip_front, margin):
                    xx = x + (skip_front / 2.0 if yaw > 0 else -skip_front / 2.0)
                    g.box(VIOLET, (xx, -w / 2.0 - 3.0, z + wh / 2.0), (ww, 10.0, wh))


def turret(g, x, y, top, r, rows=()):
    """A round corner turret (eight sides, one to the road) from the footing to `top`, a white collar, slit windows
    to the road at `rows`. Its dome is the caller's."""
    g.cyl(MINT, (x, y, -FOOT), r, top + FOOT, rot=(0, 22.5, 0), steps=8)
    g.cyl(WHITE, (x, y, top - 34.0), r + 16.0, 34.0, rot=(0, 22.5, 0), steps=8)
    for z in rows:
        g.box(VIOLET, (x, y - r * 0.924 - 2.0, z + 95.0), (r * 0.46, 10.0, 190.0))


def oval(rx, ry, n=12, cx=0.0, cy=0.0):
    return [(cx + math.cos(2.0 * math.pi * k / n) * rx, cy + math.sin(2.0 * math.pi * k / n) * ry) for k in range(n)]


# ------------------------------------------------------------------------------------------ buildings, town side
def casino(sc):
    """THE CASINO: 15 x 12 m, five storeys (19 m), a round turret under a gold dome on its road corner (25.6 m; 28 m with the flag), a
    white arcade at the ground, four floors of arched windows with two balconies, a hipped violet roof, a gold board
    of three white diamonds on the parapet. A flag turns on the dome."""
    g = sc.still()
    w, d, G, F, n = 1500.0, 1200.0, 500.0, 350.0, 4
    H = G + n * F
    yf = -d / 2.0
    body(g, w, d, H)
    x0, x1 = -w / 2.0 + 300.0, w / 2.0
    g.box(VIOLET, ((x0 + x1) / 2.0, yf - 2.0, 240.0), (x1 - x0 - 60.0, 8.0, 400.0))      # the shade under the arcade
    arcade(g, WHITE, x0, x1, yf - 4.0, 0.0, G, 4, t=34.0)
    g.box(GOLD, (150.0, yf - 90.0, G - 14.0), (330.0, 150.0, 12.0))                      # the door's canopy
    xs = [x0 + 300.0 * (k + 0.5) for k in range(4)]
    for i in range(n):
        z = G + i * F
        wins(g, xs, yf, z + 78.0, 150.0, 226.0)
        g.box(WHITE, (0, 0, z + 6.0), (w + 16.0, d + 16.0, 12.0))
        if i in (0, 2):
            balcony(g, x0 + 10.0, x1 - 10.0, yf, z + 62.0)
    flank(g, w, d, [G + i * F + 80.0 for i in range(n)], n=3, skip_front=260.0)
    with Frame(g, yaw=90.0):                                                             # the side door and its awnings
        for x in (-150.0, 250.0):
            arch(g, VIOLET, x, -w / 2.0, 40.0, 170.0, 330.0)
            awning(g, x - 110.0, x + 110.0, -w / 2.0, 430.0, depth=100.0, drop=44.0, n=2)
    g.box(WHITE, (0, 0, H + 14.0), (w + 70.0, d + 70.0, 28.0))
    hip(g, VIOLET, 0, 0, H + 28.0, w + 30.0, d + 30.0, 260.0, top=0.45)
    vent(g, 300.0, 100.0, H + 288.0)
    vent(g, 180.0, -120.0, H + 288.0)
    chimney(g, -220.0, 180.0, H + 288.0)
    g.box(GOLD, (150.0, yf - 42.0, H + 86.0), (440.0, 12.0, 116.0))                      # the board: three diamonds, no word
    for k in (-1, 0, 1):
        g.box(WHITE, (150.0 + k * 130.0, yf - 50.0, H + 86.0), (58.0, 8.0, 58.0), rot=(45, 0, 0))
    for x in (-150.0, 450.0):
        lantern(g, x, yf - 34.0, 300.0)
    tx, ty, tt = -w / 2.0 + 40.0, yf + 40.0, H + 250.0
    turret(g, tx, ty, tt, 230.0, rows=[60.0] + [G + i * F + 80.0 for i in range(n)] + [H + 40.0])
    g.cyl(WHITE, (tx, ty, G - 30.0), 246.0, 30.0, rot=(0, 22.5, 0), steps=8)
    top = dome(g, tx, ty, tt, 236.0, squash=1.15, steps=(5, 8))
    flag(sc, g, tx, ty, top - 10.0, WHITE, w=150.0, h=80.0, pole=260.0, phase=0.2)


def ballroom(sc):
    """THE BALLROOM: 15 x 12 m, one great hall 12.5 m to the cornice (18 m to the cupola): five arched windows 8 m
    tall, lit gold from inside, between white pilasters; a gold frieze, a parapet with urns, a violet hipped roof and
    a cupola under a gold dome; a flight of steps to the door. A flag on the cupola."""
    g = sc.still()
    w, d, H = 1500.0, 1200.0, 1250.0
    yf = -d / 2.0
    body(g, w, d, H, plinth=160.0)
    xs = spread(5, w, 90.0)
    for x in xs:
        arch(g, VIOLET, x, yf, 230.0, 176.0, 820.0, t=10.0)
        arch(g, GOLD, x, yf - 10.0, 262.0, 118.0, 742.0, t=4.0, glow=0.3)                # the lit hall behind the glass
        g.box(WHITE, (x, yf - 16.0, 600.0), (8.0, 6.0, 660.0))
        g.box(WHITE, (x, yf - 16.0, 900.0), (118.0, 6.0, 8.0))
    for k in range(6):
        g.box(WHITE, (-w / 2.0 + 90.0 + k * (w - 180.0) / 5.0, yf - 8.0, (160.0 + H) / 2.0), (48.0, 22.0, H - 160.0))
    g.box(GOLD, (0, 0, H - 34.0), (w + 10.0, d + 10.0, 38.0))
    g.box(WHITE, (0, 0, H + 20.0), (w + 70.0, d + 70.0, 40.0))
    g.box(WHITE, (0, yf - 16.0, H + 74.0), (w + 40.0, 12.0, 68.0))
    for s in (-1, 1):
        g.box(WHITE, (s * (w / 2.0 + 14.0), 0, H + 74.0), (12.0, d + 40.0, 68.0))
    for x in (-w / 2.0 - 10.0, -260.0, 260.0, w / 2.0 + 10.0):
        urn(g, x, yf - 16.0, H + 108.0, s=1.5)
    hip(g, VIOLET, 0, 0, H + 40.0, w - 80.0, d - 80.0, 300.0, top=0.5)
    g.cyl(WHITE, (0, 0, H + 320.0), 150.0, 170.0, rot=(0, 22.5, 0), steps=8)             # the cupola
    for a in (-45.0, 0.0, 45.0):
        with Frame(g, yaw=a):
            g.box(YEL, (0, -141.0, H + 410.0), (60.0, 8.0, 110.0), glow=0.5)
    g.cyl(WHITE, (0, 0, H + 486.0), 168.0, 16.0, rot=(0, 22.5, 0), steps=8)
    top = dome(g, 0, 0, H + 500.0, 158.0, squash=1.1)
    flag(sc, g, 0, 0, top - 8.0, MINT, w=140.0, h=74.0, pole=230.0, phase=0.55)
    for k in range(3):                                                                    # the steps, the door, its canopy
        hk, dep = 160.0 - 53.0 * k, 60.0 + 55.0 * k
        g.box(WHITE, (0, yf - dep / 2.0, (hk - 100.0) / 2.0), (520.0 + 70.0 * k, dep, hk + 100.0))
    g.box(WHITE, (0, yf - 14.0, 330.0), (150.0, 10.0, 330.0))
    g.box(GOLD, (0, yf - 20.0, 330.0), (8.0, 8.0, 330.0))
    g.box(GOLD, (0, yf - 80.0, 520.0), (290.0, 150.0, 12.0))
    for s in (-1, 1):
        lantern(g, s * 132.0, yf - 26.0, 420.0)
    for yaw in (90.0, -90.0):                                                             # three tall arches on each side
        with Frame(g, yaw=yaw):
            wins(g, spread(3, d, 150.0), -w / 2.0, 230.0, 170.0, 820.0, t=10.0)


def palm_court(sc):
    """THE PALM COURT: a winter garden, 15 x 11 m: a white arcade of five arches, a barrel vault of violet glass in
    white ribs with a gold crest over it (9.9 m), and a square pavilion with a pyramid roof at each end (14 m),
    peach awnings over their doors. Two flags."""
    g = sc.still()
    w, d, G = 1500.0, 1100.0, 540.0
    yf = -d / 2.0
    body(g, w, d, G)
    g.box(VIOLET, (0, yf - 2.0, 250.0), (800.0, 8.0, 420.0))
    arcade(g, WHITE, -420.0, 420.0, yf - 4.0, 0.0, G, 5, t=34.0)
    g.box(WHITE, (0, 0, G + 12.0), (w + 50.0, d + 50.0, 24.0))
    g.box(GOLD, (0, yf - 14.0, G + 50.0), (840.0, 8.0, 8.0))
    for x in spread(6, 840.0, 0.0):
        g.box(WHITE, (x, yf - 14.0, G + 36.0), (10.0, 8.0, 30.0))
    rod(g, VIOLET, (-420.0, 60.0, G), (420.0, 60.0, G), 430.0, steps=12)                 # the glass vault
    for k in range(6):
        x = -420.0 + 168.0 * k
        rod(g, WHITE, (x - 9.0, 60.0, G), (x + 9.0, 60.0, G), 443.0, steps=12)
    for s in (-1, 1):
        g.box(WHITE, (0, 60.0 + s * 313.0, G + 313.0), (840.0, 14.0, 14.0))
    g.box(GOLD, (0, 60.0, G + 447.0), (860.0, 18.0, 14.0))
    for x in (-300.0, 0.0, 300.0):
        g.sphere(GOLD, (x, 60.0, G + 468.0), 15.0, steps=(3, 5))
    for k, s in enumerate((-1, 1)):                                                       # the end pavilions
        px, py, ph = s * (w / 2.0 - 165.0), yf + 160.0, 1120.0
        g.box(MINT, (px, py, (ph - FOOT) / 2.0), (330.0, 360.0, ph + FOOT))
        pf = py - 180.0
        arch(g, VIOLET, px, pf, 20.0, 160.0, 370.0)
        awning(g, px - 110.0, px + 110.0, pf, 480.0, depth=110.0, drop=46.0, n=3)
        arch(g, VIOLET, px, pf, 640.0, 130.0, 300.0)
        balcony(g, px - 110.0, px + 110.0, pf, 622.0, depth=44.0)
        g.box(WHITE, (px, py, G + 12.0), (346.0, 376.0, 24.0))
        g.box(WHITE, (px, py, ph + 13.0), (372.0, 402.0, 26.0))
        g.cyl(VIOLET, (px, py, ph + 26.0), 186.0 * 1.41421, 250.0, rot=(0, 45, 0), steps=4, top_r=10.0)
        g.sphere(GOLD, (px, py, ph + 286.0), 18.0, steps=(3, 5))
        lantern(g, px + s * 118.0, pf - 2.0, 330.0)
        with Frame(g, yaw=90.0 * s):                                                      # the pavilion's outer side
            arch(g, VIOLET, -s * (d / 2.0 - 160.0), -w / 2.0, 640.0, 130.0, 300.0)
            arch(g, VIOLET, -s * (d / 2.0 - 160.0), -w / 2.0, 120.0, 130.0, 270.0)
            for x in (s * 80.0, s * 330.0):
                g.box(VIOLET, (x, -w / 2.0 - 3.0, 300.0), (120.0, 10.0, 240.0))
        flag(sc, g, px, py, ph + 296.0, (WHITE, MINT)[k], w=120.0, h=64.0, pole=200.0, phase=0.3 + 0.4 * k)


def roof_garden(sc):
    """THE ROOF-GARDEN WING: 15 x 11 m, three storeys (11.8 m) and a garden on top (15 m): three wide arches under
    peach awnings, two floors of six arched windows behind balconies, and on the roof two white pergolas under green,
    clipped balls in tubs, a parasol, a stair hut with a shell vane, and the staff's washing on a line at the back
    (it swings)."""
    g = sc.still()
    w, d, G, F = 1500.0, 1100.0, 440.0, 370.0
    H = G + 2 * F
    yf = -d / 2.0
    body(g, w, d, H)
    for x in spread(3, w, 110.0):
        arch(g, VIOLET, x, yf, 50.0, 300.0, 350.0, t=10.0)
        awning(g, x - 180.0, x + 180.0, yf, 448.0, depth=130.0, drop=56.0, n=4)
    g.box(WHITE, (0, 0, G + 6.0), (w + 16.0, d + 16.0, 12.0))
    for i in range(2):
        z = G + i * F
        wins(g, spread(6, w, 80.0), yf, z + 84.0, 130.0, 222.0)
        balcony(g, -w / 2.0 + 50.0, w / 2.0 - 50.0, yf, z + 66.0)
    for x in (-560.0, -110.0, 340.0):                                                     # window boxes on the top balcony
        g.box(LEAF, (x, yf - 58.0, G + F + 126.0), (150.0, 22.0, 22.0))
        g.box(WHITE, (x + 30.0, yf - 60.0, G + F + 140.0), (40.0, 14.0, 12.0))
        g.box(GOLD, (x - 40.0, yf - 60.0, G + F + 140.0), (30.0, 14.0, 12.0))
    flank(g, w, d, [130.0, G + 90.0, G + F + 90.0], n=3)
    g.box(WHITE, (0, 0, H + 12.0), (w + 60.0, d + 60.0, 24.0))
    R = H + 24.0
    g.box(WHITE, (0, yf - 10.0, R + 30.0), (w + 40.0, 10.0, 60.0))
    g.box(GOLD, (0, yf - 10.0, R + 64.0), (w + 48.0, 14.0, 8.0))
    for s in (-1, 1):
        g.box(WHITE, (s * (w / 2.0 + 10.0), 0, R + 30.0), (10.0, d + 30.0, 60.0))
    for k, px in enumerate((-420.0, 330.0)):                                              # the pergolas
        py = -150.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(WHITE, (px + sx * 230.0, py + sy * 150.0, R + 125.0), (14.0, 14.0, 250.0))
            g.box(WHITE, (px, py + sx * 150.0, R + 256.0), (540.0, 14.0, 14.0))
        for j in range(5):
            g.box(WHITE, (px - 220.0 + j * 110.0, py, R + 268.0), (14.0, 380.0, 10.0))
        g.box(LEAF, (px - 60.0 + 110.0 * k, py - 20.0, R + 286.0), (330.0, 250.0, 26.0))
        g.box(LEAF, (px + 228.0 - 456.0 * k, py - 150.0, R + 190.0), (40.0, 30.0, 130.0))
        g.box(WHITE, (px - 150.0 + 300.0 * k, py - 120.0, R + 304.0), (60.0, 50.0, 14.0))
    for x, y in ((-680.0, -380.0), (-40.0, -400.0), (640.0, -380.0)):
        g.box(WHITE, (x, y, R + 35.0), (70.0, 70.0, 70.0))
        g.sphere(LEAF, (x, y, R + 118.0), 56.0, steps=(3, 6))
    g.cyl(WHITE, (-40.0, 120.0, R), 5.0, 230.0, steps=5)                                 # a parasol
    g.cyl(PEACH, (-40.0, 120.0, R + 200.0), 140.0, 50.0, steps=8, top_r=8.0)
    hx, hy = 540.0, 330.0                                                                 # the stair hut
    g.box(MINT, (hx, hy, R + 125.0), (280.0, 260.0, 250.0))
    arch(g, VIOLET, hx, hy - 130.0, R, 100.0, 200.0)
    g.box(WHITE, (hx, hy, R + 258.0), (310.0, 290.0, 16.0))
    g.cyl(VIOLET, (hx, hy, R + 266.0), 145.0 * 1.41421, 150.0, rot=(0, 45, 0), steps=4, top_r=8.0)
    vane(sc, g, hx, hy, R + 410.0, phase=0.1)
    chimney(g, 200.0, 420.0, R)
    lx0, lx1, ly, lz = -650.0, -250.0, 400.0, R + 190.0                                   # the washing line
    for x in (lx0, lx1):
        g.box(WHITE, (x, ly, R + 100.0), (10.0, 10.0, 200.0))
    g.box(WHITE, ((lx0 + lx1) / 2.0, ly, lz), (lx1 - lx0, 4.0, 4.0))
    for k, (c, ww, hh) in enumerate(((WHITE, 110.0, 130.0), (MINT, 70.0, 100.0), (WHITE, 90.0, 120.0), (CREAM, 60.0, 80.0))):
        f = sc.part(P.motion(swing=11.0, pivot=(0.0, ly, lz), axis=0.0, rate=0.45, phase=0.17 * k + 0.05))
        f.box(c, (lx0 + 55.0 + k * 100.0, ly, lz - hh / 2.0), (ww, 4.0, hh))
    for s in (-1, 1):
        lantern(g, s * 300.0, yf - 2.0, 300.0)


def gatehouse(sc):
    """THE GATEHOUSE: a porte-cochere, a flat roof with a low violet hip and a small gold dome on four pairs of white
    columns, a lit globe under it, over a cream carriage way; and the doorman's lodge beside it under a hipped
    roof, a peach awning over its window, a blank gold plate by its door. 8.5 x 5.4 m, 8 m to the lodge's finial.
    A flag on the lodge."""
    g = sc.still()
    cx = -185.0                                                                           # the canopy's middle
    g.box(CREAM, (cx, 0, -147.0), (500.0, 560.0, 300.0))
    for sx in (-1, 1):
        for sy in (-1, 1):
            px, py = cx + sx * 195.0, sy * 215.0
            g.box(WHITE, (px, py, 30.0), (110.0, 54.0, 60.0))
            for q in (-1, 1):
                g.cyl(WHITE, (px + q * 26.0, py, 60.0), 16.0, 370.0, steps=6)
            g.box(WHITE, (px, py, 437.0), (110.0, 54.0, 14.0))
    g.box(WHITE, (cx, 0, 469.0), (520.0, 560.0, 50.0))
    g.box(GOLD, (cx, 0, 499.0), (534.0, 574.0, 10.0))
    hip(g, VIOLET, cx, 0, 504.0, 480.0, 520.0, 110.0, top=0.3)
    dome(g, cx, 0, 614.0, 62.0)
    g.cyl(GOLD, (cx, 0, 400.0), 4.0, 46.0, steps=5)
    g.sphere(YEL, (cx, 0, 384.0), 24.0, steps=(4, 6), glow=0.7)
    lx, ly, lw, ld, lh = 255.0, 60.0, 340.0, 420.0, 560.0                                 # the lodge
    g.box(MINT, (lx, ly, (lh - FOOT) / 2.0), (lw, ld, lh + FOOT))
    g.box(CREAM, (lx, ly, (60.0 - FOOT) / 2.0), (lw + 20.0, ld + 20.0, 60.0 + FOOT))
    lf = ly - ld / 2.0
    arch(g, VIOLET, lx - 80.0, lf, 20.0, 112.0, 260.0)
    arch(g, VIOLET, lx + 76.0, lf, 130.0, 100.0, 180.0)
    awning(g, lx + 10.0, lx + 142.0, lf, 350.0, depth=84.0, drop=38.0, n=2)
    g.box(VIOLET, (lx, lf - 3.0, 450.0), (110.0, 10.0, 70.0))
    g.box(GOLD, (lx - 4.0, lf - 4.0, 330.0), (44.0, 6.0, 30.0))                          # the blank plate
    lantern(g, lx - 150.0, lf - 2.0, 250.0)
    with Frame(g, yaw=90.0):
        g.box(VIOLET, (ly, -(lx + lw / 2.0) - 3.0, 250.0), (120.0, 10.0, 200.0))
    g.box(WHITE, (lx, ly, lh + 13.0), (lw + 44.0, ld + 44.0, 26.0))
    hip(g, VIOLET, lx, ly, lh + 26.0, lw + 30.0, ld + 30.0, 190.0)
    chimney(g, lx + 90.0, ly + 120.0, lh + 40.0, h=190.0)
    flag(sc, g, lx, ly, lh + 200.0, MINT, w=96.0, h=54.0, pole=150.0, phase=0.4)
    for s in (-1, 1):                                                                     # two bollards at the carriage way
        g.cyl(WHITE, (cx + s * 120.0, -300.0, -60.0), 12.0, 150.0, steps=6)
        g.sphere(GOLD, (cx + s * 120.0, -300.0, 100.0), 14.0, steps=(3, 5))


def grand_hotel(sc):
    """THE GRAND: 13 x 11 m, seven storeys (25.7 m), a round turret under a tall gold dome at each road corner
    (34 m), an arcade at the ground with two peach awnings, six floors of arched windows with three balconies,
    a violet mansard with three dormers and a gold cresting, a gold board with the Row's white shell on the
    parapet. A flag on each dome."""
    g = sc.still()
    w, d, G, F, n = 1300.0, 1100.0, 500.0, 345.0, 6
    H = G + n * F
    yf = -d / 2.0
    body(g, w, d, H)
    g.box(VIOLET, (0, yf - 2.0, 240.0), (880.0, 8.0, 400.0))
    arcade(g, WHITE, -460.0, 460.0, yf - 4.0, 0.0, G, 4, t=34.0)
    for s in (-1, 1):
        awning(g, s * 345.0 - 80.0, s * 345.0 + 80.0, yf - 36.0, 330.0, depth=96.0, drop=44.0, n=2)
        lantern(g, s * 115.0, yf - 36.0, 300.0)
    xs = spread(4, 920.0, 20.0)
    for i in range(n):
        z = G + i * F
        wins(g, xs, yf, z + 76.0, 130.0, 216.0)
        g.box(WHITE, (0, 0, z + 6.0), (w + 16.0, d + 16.0, 12.0))
        if i in (0, 2, 4):
            balcony(g, -450.0, 450.0, yf, z + 60.0)
    flank(g, w, d, [G + i * F + 80.0 for i in range(n)], n=2, ww=130.0, skip_front=240.0, margin=110.0)
    with Frame(g, yaw=90.0):
        arch(g, VIOLET, 150.0, -w / 2.0, 40.0, 170.0, 330.0)
    g.box(WHITE, (0, 0, H + 14.0), (w + 70.0, d + 70.0, 28.0))
    R = H + 28.0
    hip(g, VIOLET, 0, 20.0, R, w - 60.0, d - 60.0, 330.0, top=0.62)
    for x in (-300.0, 0.0, 300.0):                                                        # dormers
        g.box(WHITE, (x, yf + 110.0, R + 130.0), (120.0, 150.0, 150.0))
        g.box(VIOLET, (x, yf + 33.0, R + 130.0), (80.0, 6.0, 100.0))
    fy, fx, tz = 322.0, 422.0, R + 330.0
    for s in (-1, 1):
        g.box(GOLD, (0, 20.0 + s * fy, tz + 15.0), (2.0 * fx, 8.0, 30.0))
        g.box(GOLD, (s * fx, 20.0, tz + 15.0), (8.0, 2.0 * fy, 30.0))
    vent(g, -250.0, 120.0, tz)
    chimney(g, 260.0, 200.0, tz)
    vane(sc, g, 0.0, 60.0, tz, phase=0.6)
    g.box(GOLD, (0, yf - 30.0, H + 92.0), (360.0, 12.0, 128.0))
    badge(g, 0.0, yf - 40.0, H + 50.0, 92.0)
    for k, s in enumerate((-1, 1)):
        tx, ty, tt = s * (w / 2.0 - 30.0), yf + 30.0, H + 330.0
        turret(g, tx, ty, tt, 190.0, rows=[70.0] + [G + i * F + 80.0 for i in (0, 2, 4)] + [H + 90.0])
        g.cyl(WHITE, (tx, ty, G - 30.0), 204.0, 30.0, rot=(0, 22.5, 0), steps=8)
        g.cyl(WHITE, (tx, ty, H - 6.0), 204.0, 30.0, rot=(0, 22.5, 0), steps=8)
        top = dome(g, tx, ty, tt, 196.0, squash=1.3, steps=(5, 8))
        flag(sc, g, tx, ty, top - 10.0, (MINT, WHITE)[k], w=140.0, h=74.0, pole=240.0, phase=0.15 + 0.5 * k)


def belvedere(sc):
    """THE BELVEDERE: 13 x 11 m, five storeys (19.4 m): a free colonnade of six columns carrying a terrace, a middle
    bay standing forward with four arched loggias one over another and a pediment with a gold boss, wings of arched
    windows under peach awnings, and over the hipped roof a white drum, a great gold dome and a lantern (29 m).
    A flag on the lantern."""
    g = sc.still()
    w, d, G, F, n = 1300.0, 1100.0, 500.0, 360.0, 4
    H = G + n * F
    yf = -d / 2.0
    body(g, w, d, H)
    yc = yf - 70.0
    g.box(MINT, (0, yf - 20.0, (H + 200.0 - FOOT) / 2.0), (460.0, 100.0, H + 200.0 + FOOT))  # the middle bay
    for x in spread(6, w, 40.0):
        g.cyl(WHITE, (x, yf - 170.0, -40.0), 24.0, G - 10.0, steps=6)
    g.box(WHITE, (0, yf - 95.0, G - 25.0), (w + 20.0, 200.0, 50.0))
    g.box(GOLD, (0, yf - 190.0, G + 40.0), (w + 20.0, 8.0, 8.0))
    for x in spread(7, w + 20.0, 0.0):
        g.box(WHITE, (x, yf - 190.0, G + 18.0), (10.0, 8.0, 40.0))
    arch(g, VIOLET, 0.0, yc, 20.0, 220.0, 400.0)
    for s in (-1, 1):
        lantern(g, s * 150.0, yc - 2.0, 300.0)
        g.box(VIOLET, (s * 440.0, yf - 3.0, 250.0), (300.0, 8.0, 300.0))
    for i in range(n):
        z = G + i * F
        g.box(WHITE, (0, 0, z + 6.0), (w + 16.0, d + 16.0, 12.0))
        for s in (-1, 1):
            wins(g, (s * 335.0, s * 545.0), yf, z + 80.0, 124.0, 216.0)
        arch(g, VIOLET, 0.0, yc, z + 64.0, 260.0, 262.0)
        balcony(g, -200.0, 200.0, yc, z + 54.0, depth=50.0)
    for s in (-1, 1):                                                                     # awnings on the first floor's wings
        for x in (335.0, 545.0):
            awning(g, s * x - 76.0, s * x + 76.0, yf, G + 318.0, depth=74.0, drop=40.0, n=2)
    g.cyl(GOLD, (0.0, yc - 12.0, H + 96.0), 70.0, 14.0, rot=(0, 0, 90), steps=10)        # the boss in the attic
    badge(g, 0.0, yc - 16.0, H + 66.0, 56.0)
    g.side(WHITE, [(-262.0, H + 200.0), (262.0, H + 200.0), (0.0, H + 330.0)], yc - 10.0, yf + 40.0)
    flank(g, w, d, [G + i * F + 80.0 for i in range(n)], n=3)
    for yaw in (90.0, -90.0):
        with Frame(g, yaw=yaw):
            wins(g, (-200.0, 200.0), -w / 2.0, 60.0, 150.0, 320.0)
    g.box(WHITE, (0, 0, H + 14.0), (w + 70.0, d + 70.0, 28.0))
    for s in (-1, 1):
        urn(g, s * (w / 2.0 + 14.0), yf - 14.0, H + 28.0, s=1.7)
    hip(g, VIOLET, 0, 0, H + 28.0, w - 40.0, d - 40.0, 250.0, top=0.5)
    dz = H + 250.0
    g.cyl(WHITE, (0, 40.0, dz), 250.0, 230.0, rot=(0, 22.5, 0), steps=8)
    for a in (-45.0, 0.0, 45.0):
        with Frame(g, at=(0.0, 40.0, 0.0), yaw=a):
            g.box(VIOLET, (0, -234.0, dz + 130.0), (90.0, 8.0, 150.0))
    g.cyl(WHITE, (0, 40.0, dz + 222.0), 270.0, 20.0, rot=(0, 22.5, 0), steps=8)
    top = dome(g, 0, 40.0, dz + 240.0, 258.0, squash=1.1, steps=(5, 10), finial=False)
    g.cyl(WHITE, (0, 40.0, top - 20.0), 60.0, 120.0, steps=8)
    g.cyl(GOLD, (0, 40.0, top + 100.0), 78.0, 90.0, steps=8, top_r=6.0)
    flag(sc, g, 0.0, 40.0, top + 180.0, WHITE, w=150.0, h=80.0, pole=230.0, phase=0.7)


def loggia(sc):
    """THE LOGGIA: 13 x 11 m, six storeys (21.6 m): three arched doors under peach awnings, then five floors that
    are each an open loggia of five white arches over deep shade, a gold rail to every floor, peach blinds half
    down in some arches; a low violet roof with deep eaves on white brackets, and a stair tower with a pyramid
    roof and a shell vane (27.5 m). A flag at the other end."""
    g = sc.still()
    w, d, G, F, n = 1300.0, 1100.0, 460.0, 340.0, 5
    H = G + n * F
    yf = -d / 2.0
    body(g, w, d, H)
    for k, x in enumerate(spread(3, w, 120.0)):
        arch(g, VIOLET, x, yf, 40.0, 250.0, 370.0, t=10.0)
        if k != 1:
            awning(g, x - 150.0, x + 150.0, yf, 452.0, depth=120.0, drop=52.0, n=3)
    for s in (-1, 1):
        lantern(g, s * 176.0, yf - 2.0, 300.0)
    blinds = ((0, 1), (0, 4), (1, 2), (2, 0), (2, 3), (3, 1), (3, 4), (4, 2))
    for i in range(n):
        z = G + i * F
        g.box(VIOLET, (0, yf - 2.0, z + 180.0), (w - 140.0, 8.0, 250.0))
        arcade(g, WHITE, -w / 2.0 + 50.0, w / 2.0 - 50.0, yf - 8.0, z + 40.0, F - 50.0, 5, t=30.0, pier=0.2, spring=0.5)
        g.box(WHITE, (0, yf - 24.0, z + 26.0), (w - 40.0, 60.0, 14.0))
        g.box(GOLD, (0, yf - 48.0, z + 64.0), (w - 60.0, 8.0, 8.0))
        for (fi, k) in blinds:
            if fi == i:
                g.box(PEACH, (-w / 2.0 + 50.0 + 240.0 * (k + 0.5), yf - 9.0, z + 222.0), (186.0, 6.0, 74.0))
    flank(g, w, d, [G + i * F + 80.0 for i in range(n)], n=3)
    g.box(WHITE, (0, 0, H + 10.0), (w + 40.0, d + 40.0, 20.0))
    for x in spread(7, w + 160.0, 0.0):
        g.box(WHITE, (x, yf - 70.0, H - 6.0), (22.0, 120.0, 52.0))
    g.box(VIOLET, (0, 0, H + 32.0), (w + 260.0, d + 260.0, 24.0))
    hip(g, VIOLET, 0, 0, H + 44.0, w + 200.0, d + 200.0, 210.0, top=0.15)
    tx, ty, tt = w / 2.0 - 190.0, d / 2.0 - 190.0, H + 430.0                              # the stair tower
    g.box(MINT, (tx, ty, (H + tt) / 2.0), (300.0, 300.0, tt - H))
    arch(g, VIOLET, tx, ty - 150.0, H + 230.0, 120.0, 160.0)
    with Frame(g, at=(tx, ty, 0.0), yaw=90.0):
        arch(g, VIOLET, 0.0, -150.0, H + 230.0, 120.0, 160.0)
    g.box(WHITE, (tx, ty, tt + 10.0), (336.0, 336.0, 20.0))
    g.cyl(VIOLET, (tx, ty, tt + 20.0), 168.0 * 1.41421, 180.0, rot=(0, 45, 0), steps=4, top_r=8.0)
    vane(sc, g, tx, ty, tt + 196.0, phase=0.35)
    flag(sc, g, -w / 2.0 + 160.0, 0.0, H + 130.0, WHITE, w=140.0, h=74.0, pole=330.0, phase=0.9)


def rotunda(sc):
    """THE ROTUNDA: a round hotel 11.2 m across, five storeys (18.8 m): a ring of white columns carrying a terrace
    with a gold rail, four floors of arched windows on every side that shows, white bands, and a great gold dome
    with a white lantern (27 m). A canopy and round steps at the door, two peach awnings. A flag on the lantern."""
    g = sc.still()
    R, G, F, n, N = 560.0, 480.0, 350.0, 4, 12
    H = G + n * F
    ri = R * math.cos(math.radians(15.0))
    rt = (0, 15.0, 0)
    g.cyl(MINT, (0, 0, -FOOT), R, H + FOOT, rot=rt, steps=N)
    g.cyl(CREAM, (0, 0, -FOOT), R + 26.0, FOOT + 70.0, rot=rt, steps=N)
    for k in range(N):
        a = math.radians(15.0 + 30.0 * k)
        if math.sin(a) < 0.72:
            g.cyl(WHITE, (math.cos(a) * (R + 92.0), math.sin(a) * (R + 92.0), -40.0), 22.0, G - 10.0, steps=6)
    g.cyl(WHITE, (0, 0, G - 50.0), R + 128.0, 50.0, rot=rt, steps=N)
    g.cyl(GOLD, (0, 0, G + 36.0), R + 122.0, 8.0, rot=rt, steps=N)
    for th in (180.0, 210.0, 240.0, 270.0, 300.0, 330.0, 0.0):                            # the faces that show
        yaw = th + 90.0
        for i in range(n):
            arch(g, VIOLET, 0.0, -ri, G + i * F + 80.0, 132.0, 216.0, rot=(0, yaw, 0))
        if th in (240.0, 270.0, 300.0):
            arch(g, VIOLET, 0.0, -ri, 30.0, 160.0, 340.0, rot=(0, yaw, 0))
    for i in range(1, n):
        g.cyl(WHITE, (0, 0, G + i * F), R + 9.0, 12.0, rot=rt, steps=N)
    g.cyl(WHITE, (0, 0, G + 2 * F + 52.0), R + 52.0, 12.0, rot=rt, steps=N)              # a balcony ring at the third floor
    g.cyl(GOLD, (0, 0, G + 2 * F + 104.0), R + 50.0, 8.0, rot=rt, steps=N)
    g.cyl(WHITE, (0, 0, H), R + 40.0, 30.0, rot=rt, steps=N)
    g.cyl(WHITE, (0, 0, H + 30.0), R - 60.0, 120.0, rot=rt, steps=N)
    top = dome(g, 0, 0, H + 150.0, R - 52.0, squash=0.85, steps=(5, 12), finial=False)
    g.cyl(WHITE, (0, 0, top - 24.0), 90.0, 150.0, steps=8)
    for a in (-45.0, 0.0, 45.0):
        with Frame(g, yaw=a):
            g.box(YEL, (0, -86.0, top + 60.0), (44.0, 8.0, 80.0), glow=0.5)
    g.cyl(GOLD, (0, 0, top + 126.0), 112.0, 100.0, steps=8, top_r=6.0)
    flag(sc, g, 0.0, 0.0, top + 216.0, MINT, w=150.0, h=80.0, pole=230.0, phase=0.25)
    g.cyl(CREAM, (0, -R - 110.0, -FOOT), 250.0, FOOT + 34.0, steps=10)                   # the round steps, the canopy
    g.box(GOLD, (0, -R - 150.0, G - 86.0), (320.0, 200.0, 12.0))
    for s in (-1, 1):
        g.cyl(WHITE, (s * 140.0, -R - 230.0, 34.0), 12.0, G - 126.0, steps=6)
        lantern(g, s * 110.0, -ri - 2.0, 290.0)
        with Frame(g, yaw=s * 60.0):
            arch(g, VIOLET, 0.0, -ri, 110.0, 150.0, 250.0)
            awning(g, -100.0, 100.0, -ri, 400.0, depth=90.0, drop=42.0, n=2)


# ------------------------------------------------------------------------------------------ buildings, bay side
def spa_house(sc):
    """THE SPA HOUSE, on the upper beach: a white terrace 24 x 10 m, a mint hall under a hipped roof with a louvred
    steam vent (9.6 m), a white colonnade wing on each side with a gold rail and urns on its roof, loungers under
    them, and a plunge pool in front. Steam (white puffs, no glow) drifts up from the pool and the vent."""
    g = sc.still()
    T = 40.0
    g.box(WHITE, (0, 0, (T - FOOT) / 2.0), (2400.0, 1000.0, T + FOOT))
    hw, hd, hy, H = 900.0, 560.0, 200.0, 640.0
    yf = hy - hd / 2.0
    g.box(MINT, (0, hy, (H + T) / 2.0), (hw, hd, H - T))
    arch(g, VIOLET, 0.0, yf, T, 200.0, 380.0)
    awning(g, -160.0, 160.0, yf, T + 450.0, depth=120.0, drop=50.0, n=3)
    for s in (-1, 1):
        arch(g, VIOLET, s * 300.0, yf, T + 110.0, 130.0, 260.0)
        lantern(g, s * 150.0, yf - 2.0, T + 260.0)
    g.box(GOLD, (0, yf - 6.0, H - 60.0), (150.0, 10.0, 56.0))
    badge(g, 0.0, yf - 12.0, H - 82.0, 48.0)
    g.box(WHITE, (0, hy, H + 12.0), (hw + 60.0, hd + 60.0, 24.0))
    hip(g, VIOLET, 0, hy, H + 24.0, hw + 40.0, hd + 40.0, 220.0, top=0.25)
    vz = H + 244.0
    g.box(WHITE, (0, hy, vz + 50.0), (170.0, 130.0, 100.0))                              # the steam vent
    for j in range(3):
        g.box(VIOLET, (0, hy - 66.0, vz + 22.0 + j * 28.0), (130.0, 6.0, 12.0))
    g.box(VIOLET, (0, hy, vz + 106.0), (200.0, 160.0, 12.0))
    for s in (-1, 1):                                                                     # the colonnade wings
        wx = s * 800.0
        for k in range(5):
            g.cyl(WHITE, (s * (500.0 + 150.0 * k), -30.0, T), 20.0, 400.0, steps=6)
        g.box(WHITE, (wx, 440.0, T + 210.0), (700.0, 20.0, 420.0))
        g.box(WHITE, (wx, 200.0, T + 420.0), (760.0, 560.0, 40.0))
        g.box(GOLD, (wx, -70.0, T + 480.0), (760.0, 8.0, 8.0))
        for k in range(5):
            g.box(WHITE, (wx - 360.0 + 180.0 * k, -70.0, T + 458.0), (10.0, 8.0, 40.0))
        urn(g, s * 1150.0, -50.0, T + 440.0, s=1.3)
        for k in range(2):
            g.box(WHITE, (wx - 120.0 + 240.0 * k, 240.0, T + 20.0), (70.0, 190.0, 40.0))
            g.box(MINT, (wx - 120.0 + 240.0 * k, 310.0, T + 46.0), (60.0, 50.0, 14.0))
    g.plan(WHITE, _ccw(oval(440.0, 170.0, 12, 0.0, -300.0)), T, T + 26.0)                 # the plunge pool
    g.plan(POOL, _ccw(oval(410.0, 140.0, 12, 0.0, -300.0)), T + 20.0, T + 31.0)
    for s in (-1, 1):
        urn(g, s * 520.0, -300.0, T, s=1.4)
    puffs = ((-250.0, -300.0, 120.0, 52.0), (-90.0, -270.0, 190.0, 44.0), (80.0, -320.0, 140.0, 56.0), (240.0, -290.0, 210.0, 42.0),
             (0.0, -300.0, 280.0, 38.0), (-20.0, hy, vz + 170.0 - T, 46.0), (30.0, hy + 10.0, vz + 270.0 - T, 36.0))
    for k, (x, y, z, r) in enumerate(puffs):
        p = sc.part(P.motion(bob=-28.0, rate=0.13 + 0.02 * (k % 3), phase=0.14 * k))
        p.sphere(WHITE, (x, y, T + z), r, scale=(1.2, 1.0, 0.8), steps=(3, 6))
    flag(sc, g, 300.0, hy, H + 180.0, MINT, w=120.0, h=64.0, pole=230.0, phase=0.45)


def bath_pavilion(sc):
    """THE BATHING PAVILION, on the upper beach: 24 x 6.4 m: a white arcade of eight arches on a cream platform, peach
    awnings in four of them, a roof terrace with a gold-railed parapet and three peach parasols, a middle bay with
    a round white head and the Row's shell in gold (9.7 m), and a tower at each end under a small gold dome
    (12.5 m). Towels dry on a line on the terrace (they swing). A flag on each tower."""
    g = sc.still()
    P0, G = 50.0, 430.0
    g.box(CREAM, (0, -40.0, (P0 - FOOT) / 2.0), (2480.0, 880.0, P0 + FOOT))
    for k in range(2):
        g.box(CREAM, (0, -500.0 - 50.0 * k, (P0 - 20.0 * (k + 1) - FOOT) / 2.0), (640.0, 60.0, P0 - 20.0 * (k + 1) + FOOT))
    top = P0 + G
    g.box(MINT, (0, 0, (top - FOOT) / 2.0), (1760.0, 600.0, top + FOOT))
    g.box(VIOLET, (0, -302.0, P0 + 200.0), (1700.0, 8.0, 340.0))
    arcade(g, WHITE, -880.0, 880.0, -304.0, P0, G, 8, t=34.0)
    for k in (1, 2, 5, 6):
        x = -880.0 + 220.0 * (k + 0.5)
        awning(g, x - 78.0, x + 78.0, -338.0, P0 + 300.0, depth=90.0, drop=40.0, n=2)
    for k in (0, 7):                                                                      # cabin doors in the end arches
        x = -880.0 + 220.0 * (k + 0.5)
        g.box(WHITE, (x, -308.0, P0 + 100.0), (110.0, 8.0, 200.0))
        g.box(MINT, (x, -313.0, P0 + 100.0), (40.0, 6.0, 200.0))
    g.box(WHITE, (0, 0, top + 12.0), (1800.0, 650.0, 24.0))
    tz = top + 24.0
    g.box(WHITE, (0, -318.0, tz + 28.0), (1760.0, 10.0, 56.0))
    g.box(GOLD, (0, -318.0, tz + 60.0), (1768.0, 14.0, 8.0))
    for k, x in enumerate((-700.0, -400.0)):
        g.cyl(WHITE, (x, 40.0, tz), 5.0, 230.0, steps=5)
        g.cyl(PEACH, (x, 40.0, tz + 196.0), 130.0, 48.0, steps=8, top_r=8.0)
        g.box(WHITE, (x + 20.0, -60.0, tz + 22.0), (60.0, 150.0, 44.0))
    g.box(MINT, (0, 0, (760.0 - FOOT) / 2.0), (420.0, 640.0, 760.0 + FOOT))              # the middle bay
    g.side(WHITE, arch_pts(0.0, 760.0, 420.0, 215.0, n=4), -322.0, 322.0)
    for s in (-1, 1):                                                                     # its two doors, seen through the arcade
        g.box(VIOLET, (s * 110.0, -324.0, P0 + 150.0), (112.0, 8.0, 300.0))
    g.cyl(GOLD, (0.0, -332.0, 640.0), 84.0, 14.0, rot=(0, 0, 90), steps=10)
    badge(g, 0.0, -336.0, 604.0, 66.0)
    for k, s in enumerate((-1, 1)):                                                       # the end towers
        tx, th = s * 1040.0, 900.0
        g.box(MINT, (tx, 0, (th - FOOT) / 2.0), (320.0, 640.0, th + FOOT))
        arch(g, VIOLET, tx, -320.0, P0, 150.0, 330.0)
        arch(g, VIOLET, tx, -320.0, 560.0, 130.0, 240.0)
        balcony(g, tx - 110.0, tx + 110.0, -320.0, 542.0, depth=44.0)
        with Frame(g, yaw=90.0 * s):
            arch(g, VIOLET, 0.0, -1200.0, 560.0, 130.0, 240.0)
        g.box(WHITE, (tx, 0, th + 13.0), (360.0, 680.0, 26.0))
        g.cyl(WHITE, (tx, 0, th + 26.0), 140.0, 50.0, rot=(0, 22.5, 0), steps=8)
        dtop = dome(g, tx, 0, th + 76.0, 148.0, squash=1.1)
        flag(sc, g, tx, 0.0, dtop - 8.0, (MINT, WHITE)[k], w=120.0, h=64.0, pole=200.0, phase=0.1 + 0.45 * k)
        lantern(g, tx - s * 110.0, -322.0, P0 + 250.0)
    lx0, lx1, ly, lz = 250.0, 830.0, 120.0, tz + 170.0                                    # towels on a line
    for x in (lx0, lx1):
        g.box(WHITE, (x, ly, tz + 90.0), (10.0, 10.0, 180.0))
    g.box(WHITE, ((lx0 + lx1) / 2.0, ly, lz), (lx1 - lx0, 4.0, 4.0))
    for k, c in enumerate((MINT, WHITE, CREAM, WHITE, MINT)):
        f = sc.part(P.motion(swing=10.0, pivot=(0.0, ly, lz), axis=0.0, rate=0.4, phase=0.19 * k + 0.1))
        f.box(c, (lx0 + 60.0 + k * 112.0, ly, lz - 55.0), (80.0, 4.0, 110.0))


# ------------------------------------------------------------------------------------------ props
def fountain(sc):
    """A TIERED FOUNTAIN, 5.2 m across, 4.9 m: a white basin with four gold spouts, two bowls one over the other,
    a gold shell finial; the water falls from bowl to bowl in threads and a jet stands on top (they bob)."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -200.0), 260.0, 255.0, steps=12)
    g.cyl(POOL, (0, 0, 40.0), 236.0, 22.0, steps=12)
    g.cyl(WHITE, (0, 0, -200.0), 330.0, 212.0, steps=12)                                  # a step round it
    g.cyl(WHITE, (0, 0, 55.0), 44.0, 115.0, steps=8, top_r=26.0)
    g.cyl(WHITE, (0, 0, 170.0), 40.0, 40.0, steps=10, top_r=150.0)
    g.cyl(POOL, (0, 0, 204.0), 138.0, 12.0, steps=10)
    g.cyl(WHITE, (0, 0, 210.0), 26.0, 90.0, steps=8, top_r=18.0)
    g.cyl(WHITE, (0, 0, 300.0), 22.0, 30.0, steps=8, top_r=84.0)
    g.cyl(POOL, (0, 0, 324.0), 76.0, 10.0, steps=8)
    g.cyl(GOLD, (0, 0, 330.0), 12.0, 46.0, steps=6)
    for a in (-35.0, 0.0, 35.0):
        tilt_box(g, GOLD, (0, 0, 372.0), (math.sin(math.radians(a)) * 44.0, 0, 372.0 + math.cos(math.radians(a)) * 44.0), 8.0, 14.0)
    for k in range(4):
        a = math.radians(45.0 + 90.0 * k)
        g.sphere(GOLD, (math.cos(a) * 250.0, math.sin(a) * 250.0, 66.0), 20.0, steps=(3, 5))
    for k in range(6):                                                                    # the falling threads
        a = math.radians(60.0 * k + 10.0)
        v = sc.part(P.motion(bob=-5.0, rate=0.9, phase=k / 6.0))
        v.box(POOL, (math.cos(a) * 144.0, math.sin(a) * 144.0, 132.0), (12.0, 12.0, 150.0))
        v.box(WHITE, (math.cos(a + 0.5) * 80.0, math.sin(a + 0.5) * 80.0, 268.0), (9.0, 9.0, 112.0))
    j = sc.part(P.motion(bob=18.0, rate=0.7, phase=0.3))
    j.cyl(POOL, (0, 0, 400.0), 9.0, 56.0, steps=6)
    j.sphere(WHITE, (0, 0, 464.0), 20.0, steps=(3, 6))


def bandstand(sc):
    """A GOLD-RAILED BANDSTAND for a string trio, 6 m across, 6.3 m: an eight-sided white plinth and steps, eight
    posts, gold rails, a peach valance, a violet roof with a gold finial; inside three chairs, three gold music
    stands, a cello and a gold harp. A pennant turns on the finial."""
    g = sc.still()
    R, Z = 300.0, 90.0
    rt = (0, 22.5, 0)
    g.cyl(WHITE, (0, 0, -250.0), R, 250.0 + Z, rot=rt, steps=8)
    g.cyl(MINT, (0, 0, Z - 2.0), R - 16.0, 7.0, rot=rt, steps=8)
    ri = R * math.cos(math.radians(22.5))
    for k in range(2):
        g.box(WHITE, (0, -ri - 30.0 - 50.0 * k, (60.0 - 30.0 * k - 150.0) / 2.0), (170.0, 60.0, 60.0 - 30.0 * k + 150.0))
    for k in range(8):
        a = math.radians(22.5 + 45.0 * k)
        g.cyl(WHITE, (math.cos(a) * (R - 22.0), math.sin(a) * (R - 22.0), Z), 9.0, 300.0, steps=5)
        th = 45.0 * k
        if k != 6:                                                                        # the road side is open: the steps
            b = math.radians(th)
            rr = (R - 22.0) * math.cos(math.radians(22.5))
            g.box(WHITE, (math.cos(b) * rr, math.sin(b) * rr, Z + 34.0), (212.0, 6.0, 46.0), rot=(0, th + 90.0, 0))
            g.box(GOLD, (math.cos(b) * rr, math.sin(b) * rr, Z + 68.0), (216.0, 9.0, 8.0), rot=(0, th + 90.0, 0))
    g.cyl(PEACH, (0, 0, Z + 286.0), R + 22.0, 26.0, rot=rt, steps=8)
    g.cyl(WHITE, (0, 0, Z + 310.0), R + 30.0, 8.0, rot=rt, steps=8)
    g.cyl(VIOLET, (0, 0, Z + 318.0), R + 30.0, 150.0, rot=rt, steps=8, top_r=36.0)
    g.cyl(GOLD, (0, 0, Z + 468.0), 36.0, 30.0, steps=8, top_r=10.0)
    g.sphere(GOLD, (0, 0, Z + 508.0), 16.0, steps=(3, 5))
    g.cyl(WHITE, (0, 0, Z + 520.0), 4.0, 90.0, steps=5)
    f = sc.part(P.motion(swing=18.0, pivot=(0.0, 0.0, Z + 590.0), axis=P.UP, rate=0.3, phase=0.2))
    f.box(MINT, (58.0, 0.0, Z + 588.0), (100.0, 4.0, 44.0))
    for k, (x, y) in enumerate(((-120.0, 60.0), (0.0, 130.0), (120.0, 60.0))):           # the trio's places
        g.box(WHITE, (x, y, Z + 23.0), (40.0, 40.0, 46.0))
        g.box(WHITE, (x, y + 18.0, Z + 72.0), (40.0, 6.0, 56.0))
        g.box(GOLD, (x, y - 70.0, Z + 55.0), (5.0, 5.0, 110.0))
        g.box(GOLD, (x, y - 72.0, Z + 118.0), (36.0, 5.0, 26.0), rot=(0, 0, -20))
    g.box(WOOD, (-150.0, -10.0, Z + 44.0), (34.0, 18.0, 76.0), rot=(0, 0, 12))           # the cello, leaning on a chair
    g.box(WOOD, (-150.0, -2.0, Z + 108.0), (8.0, 8.0, 60.0), rot=(0, 0, 12))
    hx, hy = 150.0, -30.0                                                                 # the harp
    tilt_box(g, GOLD, (hx - 30.0, hy, Z), (hx - 30.0, hy, Z + 140.0), 8.0, 10.0)
    tilt_box(g, GOLD, (hx - 30.0, hy, Z + 140.0), (hx + 40.0, hy, Z + 110.0), 8.0, 10.0)
    tilt_box(g, GOLD, (hx + 40.0, hy, Z + 110.0), (hx - 26.0, hy, Z + 6.0), 8.0, 10.0)


def flagpoles(sc):
    """A ROW OF FIVE FLAGPOLES on a white kerb 12 m long, the middle one tallest (9.7 m): gold balls, plain
    swallow-tailed pennants in mint, gold, white and violet (they turn), a gold-capped post between the poles."""
    g = sc.still()
    g.box(WHITE, (0, 0, -120.0), (1200.0, 70.0, 270.0))
    cols = (MINT, GOLD, WHITE, VIOLET, MINT)
    for k in range(5):
        x = -500.0 + 250.0 * k
        h = 760.0 + (140.0 if k == 2 else 60.0 if k in (1, 3) else 0.0)
        g.box(WHITE, (x, 0, 40.0), (46.0, 46.0, 50.0))
        g.box(GOLD, (x, 0, 68.0), (52.0, 52.0, 6.0))
        g.cyl(WHITE, (x, 0, 70.0), 7.0, h, steps=5)
        g.sphere(GOLD, (x, 0, h + 80.0), 14.0, steps=(3, 5))
        f = sc.part(P.motion(swing=13.0, pivot=(x, 0.0, h), axis=P.UP, rate=0.28 + 0.03 * k, phase=0.21 * k))
        f.box(cols[k], (x + 88.0, 0, h + 10.0), (160.0, 4.0, 86.0))
        f.box(cols[k], (x + 198.0, 0, h + 32.0), (64.0, 4.0, 40.0))
        f.box(cols[k], (x + 198.0, 0, h - 12.0), (64.0, 4.0, 40.0))
        if k < 4:
            g.cyl(WHITE, (x + 125.0, 0, 15.0), 9.0, 70.0, steps=5)
            g.sphere(GOLD, (x + 125.0, 0, 92.0), 11.0, steps=(3, 5))


def valet(sc):
    """THE VALET'S DESK on a mint carpet, 4.2 x 3 m, 3.2 m: a white desk with a gold top and a bell, a violet key
    board with gold keys behind it, a peach parasol (it sways), a blank board on an easel (three gold bars, no
    words), a brass luggage stand with a cream case, two gold-capped bollards."""
    g = sc.still()
    g.box(MINT, (0, 0, -72.0), (420.0, 300.0, 150.0))
    g.box(WHITE, (0, 40.0, 53.0), (170.0, 60.0, 100.0))
    g.box(GOLD, (0, 40.0, 107.0), (182.0, 72.0, 8.0))
    g.box(GOLD, (0, 8.0, 58.0), (120.0, 5.0, 50.0))
    g.cyl(GOLD, (40.0, 40.0, 111.0), 10.0, 5.0, steps=6)
    g.sphere(GOLD, (40.0, 40.0, 121.0), 9.0, steps=(3, 5))
    for s in (-1, 1):                                                                     # the key board
        g.box(WHITE, (s * 70.0, 115.0, 80.0), (8.0, 8.0, 160.0))
    g.box(VIOLET, (0, 115.0, 170.0), (160.0, 10.0, 110.0))
    for i in range(2):
        for k in range(4):
            g.box(GOLD, (-54.0 + 36.0 * k, 108.0, 190.0 - 44.0 * i), (9.0, 5.0, 24.0))
    px, py = -150.0, 70.0                                                                 # the parasol
    g.cyl(WHITE, (px, py, 3.0), 6.0, 300.0, steps=6)
    c = sc.part(P.motion(swing=2.5, pivot=(px, py, 3.0), axis=0.0, rate=0.22, phase=0.3))
    c.cyl(WHITE, (px, py, 244.0), 174.0, 10.0, steps=10)
    c.cyl(PEACH, (px, py, 254.0), 170.0, 56.0, steps=10, top_r=10.0)
    c.sphere(GOLD, (px, py, 316.0), 9.0, steps=(3, 5))
    ex, ey = 150.0, -70.0                                                                 # the easel
    for s in (-1, 1):
        tilt_box(g, WOOD, (ex + s * 34.0, ey + 10.0, 3.0), (ex + s * 12.0, ey + 30.0, 150.0), 6.0, 6.0)
    g.box(WHITE, (ex, ey + 14.0, 100.0), (76.0, 6.0, 96.0), rot=(0, 0, -8))
    for j in range(3):
        g.box(GOLD, (ex, ey + 9.0, 124.0 - 22.0 * j), (50.0 - 8.0 * j, 5.0, 7.0), rot=(0, 0, -8))
    sx, sy = 150.0, 80.0                                                                  # the luggage stand
    for s in (-1, 1):
        g.box(GOLD, (sx + s * 34.0, sy, 30.0), (6.0, 50.0, 54.0))
    g.box(CREAM, (sx, sy, 74.0), (84.0, 54.0, 32.0))
    g.box(GOLD, (sx, sy, 74.0), (8.0, 56.0, 34.0))
    for s in (-1, 1):
        g.cyl(WHITE, (s * 190.0, -130.0, 3.0), 11.0, 92.0, steps=6)
        g.sphere(GOLD, (s * 190.0, -130.0, 104.0), 13.0, steps=(3, 5))


def lamp(sc):
    """A THREE-GLOBE LAMP STANDARD, 4.3 m: a white base, a gold post and arms, three steady yellow globes, a blank
    mint banner on a bracket (it swings a little)."""
    g = sc.still()
    g.box(WHITE, (0, 0, -60.0), (60.0, 60.0, 180.0))
    g.box(GOLD, (0, 0, 33.0), (68.0, 68.0, 6.0))
    g.cyl(GOLD, (0, 0, 30.0), 9.0, 330.0, steps=6, top_r=6.0)
    g.cyl(GOLD, (0, 0, 150.0), 14.0, 12.0, steps=6)
    for s in (-1, 1):
        tilt_box(g, GOLD, (0, 0, 330.0), (s * 58.0, 0, 352.0), 7.0, 7.0)
        g.cyl(GOLD, (s * 58.0, 0, 350.0), 12.0, 8.0, steps=6)
        g.sphere(YEL, (s * 58.0, 0, 380.0), 23.0, steps=(4, 6), glow=0.7)
    g.cyl(GOLD, (0, 0, 360.0), 12.0, 8.0, steps=6)
    g.sphere(YEL, (0, 0, 392.0), 25.0, steps=(4, 6), glow=0.7)
    g.sphere(GOLD, (0, 0, 422.0), 7.0, steps=(3, 5))
    g.box(GOLD, (34.0, 0, 290.0), (64.0, 5.0, 5.0))
    f = sc.part(P.motion(swing=5.0, pivot=(0.0, 0.0, 290.0), axis=0.0, rate=0.4, phase=0.6))
    f.box(MINT, (40.0, 0, 234.0), (50.0, 4.0, 108.0))
    f.box(WHITE, (40.0, 0, 234.0), (28.0, 6.0, 28.0), rot=(45, 0, 0))


def balustrade(sc):
    """A BALUSTRADE WITH URNS, 8 m, LOW (1.9 m): a white rail on square balusters between three piers with gold
    bands, an urn with a gold ball on each."""
    g = sc.still()
    g.box(WHITE, (0, 0, -110.0), (800.0, 40.0, 250.0))
    g.box(WHITE, (0, 0, 99.0), (800.0, 34.0, 14.0))
    for k in range(14):
        x = -325.0 + 50.0 * k
        if abs(x) > 30.0:
            g.box(WHITE, (x, 0, 53.0), (14.0, 14.0, 78.0))
    for x in (-380.0, 0.0, 380.0):
        g.box(WHITE, (x, 0, 60.0), (46.0, 46.0, 120.0))
        g.box(GOLD, (x, 0, 106.0), (52.0, 52.0, 8.0))
        urn(g, x, 0, 120.0, s=0.95)


def trolley(sc):
    """A LUGGAGE TROLLEY, LOW (1.9 m): a brass cage trolley on a cream flagstone, stacked with a white trunk in gold
    straps, a mint case, a cream case and two flat hat boxes with bows; a steamer trunk on end and a mint suitcase
    beside it. Luggage is white, mint and cream only."""
    g = sc.still()
    g.box(CREAM, (0, 0, -74.0), (400.0, 150.0, 150.0))
    g.box(GOLD, (0, 0, 21.0), (190.0, 84.0, 6.0))
    g.box(VIOLET, (0, 0, 27.0), (180.0, 74.0, 8.0))
    for sx in (-1, 1):
        for sy in (-1, 1):
            rod(g, VIOLET, (sx * 70.0, sy * 30.0 - 5.0, 10.0), (sx * 70.0, sy * 30.0 + 5.0, 10.0), 9.0, steps=6)
            g.box(GOLD, (sx * 88.0, sy * 36.0, 105.0), (6.0, 6.0, 160.0))
        g.box(GOLD, (sx * 88.0, 0, 186.0), (6.0, 78.0, 6.0))
    g.box(GOLD, (0, 0, 189.0), (182.0, 6.0, 6.0))
    g.box(WHITE, (-22.0, 0, 57.0), (118.0, 60.0, 52.0))
    for x in (-58.0, 14.0):
        g.box(GOLD, (x, 0, 57.0), (7.0, 63.0, 55.0))
    g.box(MINT, (-26.0, 0, 101.0), (100.0, 50.0, 36.0))
    g.box(WHITE, (-26.0, -26.0, 101.0), (20.0, 5.0, 8.0))
    g.box(CREAM, (-30.0, 0, 133.0), (72.0, 44.0, 28.0))
    g.box(GOLD, (-30.0, 0, 133.0), (6.0, 46.0, 30.0))
    g.cyl(WHITE, (56.0, 0, 31.0), 26.0, 20.0, steps=8)                                    # hat boxes: flat, with bows
    g.cyl(MINT, (56.0, 0, 51.0), 28.0, 6.0, steps=8)
    g.cyl(CREAM, (56.0, 0, 57.0), 20.0, 16.0, steps=8)
    g.cyl(WHITE, (56.0, 0, 73.0), 22.0, 5.0, steps=8)
    g.box(GOLD, (56.0, 0, 82.0), (26.0, 8.0, 8.0))
    g.box(WHITE, (156.0, 12.0, 61.0), (64.0, 56.0, 120.0))                               # the steamer trunk on end
    for z in (22.0, 100.0):
        g.box(GOLD, (156.0, 12.0, z), (67.0, 59.0, 7.0))
    g.box(GOLD, (156.0, -17.0, 62.0), (12.0, 5.0, 14.0))
    g.box(MINT, (-158.0, 16.0, 17.0), (92.0, 60.0, 32.0))
    g.box(WHITE, (-158.0, -15.0, 17.0), (24.0, 5.0, 8.0))


def tea_tables(sc):
    """AFTERNOON TEA, LOW (1.8 m): a white dais with a mint carpet, three round tables under cloths, a tiered gold
    and white cake stand and a teapot on each, six white chairs, a gold urn of tea on a side table."""
    g = sc.still()
    D = 46.0
    g.box(WHITE, (0, 0, (D - 160.0) / 2.0), (640.0, 260.0, D + 160.0))
    g.box(MINT, (0, 0, D + 2.0), (580.0, 200.0, 5.0))
    for i, x in enumerate((-200.0, -10.0, 180.0)):
        g.cyl(WHITE, (x, 0, D), 8.0, 60.0, steps=5)
        g.cyl(MINT if i % 2 else WHITE, (x, 0, D + 44.0), 52.0, 18.0, steps=8, top_r=46.0)
        g.cyl(WHITE, (x, 0, D + 62.0), 46.0, 5.0, steps=8)
        g.cyl(GOLD, (x, 0, D + 67.0), 5.0, 56.0, steps=5)
        g.cyl(WHITE, (x, 0, D + 80.0), 24.0, 5.0, steps=6)
        g.cyl(WHITE, (x, 0, D + 102.0), 16.0, 5.0, steps=6)
        g.sphere(GOLD, (x, 0, D + 128.0), 7.0, steps=(3, 5))
        g.sphere(WHITE, (x + 28.0, -20.0, D + 76.0), 10.0, steps=(3, 5))
        g.box(WHITE, (x + 40.0, -20.0, D + 78.0), (10.0, 5.0, 5.0))
        for s in (-1, 1):
            g.box(WHITE, (x + s * 74.0, 14.0, D + 22.0), (38.0, 38.0, 44.0))
            g.box(WHITE, (x + s * 90.0, 14.0, D + 70.0), (6.0, 38.0, 54.0))
    g.box(WHITE, (285.0, 60.0, D + 40.0), (50.0, 50.0, 80.0))                            # the tea urn
    g.cyl(GOLD, (285.0, 60.0, D + 80.0), 16.0, 40.0, steps=6, top_r=20.0)
    g.sphere(GOLD, (285.0, 60.0, D + 126.0), 9.0, steps=(3, 5))


# ------------------------------------------------------------------------------------------ the hero
def shell(sc):
    """THE SHELL: a scallop shell 42 m tall and 43 m wide stood on its hinge: eleven flutes, white and mint by
    turns, each with a raised rib, hollow towards the road like a bandshell, under a scalloped gold rim; white ears
    at the hinge with gold edges, and a pearl 3.8 m across in a gold cup before it. At its foot a round dance floor
    25 m across inlaid in mint, white and a gold fan, two curved wings of white columns with gold finials, a small
    domed temple at each side, steady footlights along the floor's road edge, and before it all a fountain whose
    jet bobs. Two tall mint pennants turn."""
    g = sc.still()
    R0, TH, N, HZ = 3700.0, 66.0, 11, 300.0
    da = 2.0 * TH / N

    def rad(a):
        return R0 * (1.0 - 0.36 * (a / TH) ** 2)

    def pt(a, r):
        return (math.sin(math.radians(a)) * r, HZ + math.cos(math.radians(a)) * r)

    for k in range(N):
        a0 = -TH + k * da
        a1, am = a0 + da, a0 + da / 2.0
        col = WHITE if k % 2 == 0 else MINT
        yb = 330.0 + 300.0 * math.cos(math.radians(am * 90.0 / TH))
        tip, e0, e1 = rad(am) + 70.0, rad(a0) - 40.0, rad(a1) - 40.0
        g.side(col, _ccw([pt(a1, 240.0), pt(a1, e1), pt(am, tip), pt(a0, e0), pt(a0, 240.0)]), yb, yb + 120.0)
        b0, b1 = a0 + da * 0.3, a1 - da * 0.3
        g.side(col, _ccw([pt(b1, 700.0), pt(b1, tip - 260.0), pt(am, tip - 150.0), pt(b0, tip - 260.0), pt(b0, 700.0)]), yb - 36.0, yb + 2.0)
        g.side(GOLD, _ccw([pt(a0, e0), pt(am, tip), pt(a1, e1), pt(a1, e1 + 120.0), pt(am, tip + 120.0), pt(a0, e0 + 120.0)]), yb - 24.0, yb + 140.0)
    for s in (-1, 1):                                                                     # the ears, the struts behind
        g.side(WHITE, _ccw([(s * 200.0, HZ - 60.0), (s * 980.0, HZ - 60.0), (s * 980.0, HZ + 300.0), (s * 520.0, HZ + 430.0)]), 330.0, 450.0)
        tilt_box(g, GOLD, (s * 990.0, 390.0, HZ + 310.0), (s * 520.0, 390.0, HZ + 442.0), 150.0, 40.0)
        tilt_box(g, WHITE, (s * 1000.0, 1500.0, -150.0), (s * 800.0, 760.0, 1900.0), 90.0, 90.0)
    g.box(WHITE, (0, 480.0, (HZ - 40.0 - FOOT) / 2.0), (2300.0, 560.0, HZ - 40.0 + FOOT))  # the hinge's plinth
    g.box(GOLD, (0, 480.0, HZ - 70.0), (2320.0, 580.0, 30.0))
    g.cyl(GOLD, (0, 300.0, HZ + 20.0), 330.0, 200.0, rot=(0, 0, 90), steps=12)             # the hinge's boss, over the flutes' roots
    g.box(WHITE, (0, -150.0, (60.0 - FOOT) / 2.0), (2700.0, 1500.0, 60.0 + FOOT))        # the terrace
    g.cyl(GOLD, (0, 130.0, 60.0), 70.0, 110.0, steps=10, top_r=200.0)                    # the pearl in its cup
    g.sphere(WHITE, (0, 130.0, 345.0), 190.0, steps=(5, 10), glow=0.25)
    cy, FR = -1150.0, 1250.0                                                              # the dance floor
    g.cyl(WHITE, (0, cy, -FOOT), FR, FOOT + 60.0, steps=20)
    g.cyl(MINT, (0, cy, 58.0), FR - 120.0, 6.0, steps=20)
    g.cyl(WHITE, (0, cy, 60.0), 820.0, 7.0, steps=16)
    for k in range(-2, 3):
        a = math.radians(k * 28.0)
        dx, dy = math.sin(a), -math.cos(a)
        g.box(GOLD, (dx * 430.0, cy + dy * 430.0, 69.0), (640.0, 96.0, 6.0), rot=(0, math.degrees(math.atan2(dy, dx)), 0))
    g.cyl(GOLD, (0, cy, 66.0), 150.0, 9.0, steps=12)
    CR = 1380.0                                                                           # the colonnade's two wings
    for s in (-1, 1):
        prev = None
        for ph in (20.0, 38.0, 56.0, 74.0):
            a = math.radians(ph)
            x, y = s * math.cos(a) * CR, cy + math.sin(a) * CR
            g.cyl(WHITE, (x, y, 60.0), 30.0, 560.0, steps=6)
            g.sphere(GOLD, (x, y, 706.0), 30.0, steps=(3, 5))
            if prev:
                tilt_box(g, WHITE, (prev[0], prev[1], 650.0), (x, y, 650.0), 76.0, 60.0)
            prev = (x, y)
        tx, ty = s * 1560.0, cy - 60.0                                                    # a temple at each side
        g.cyl(MINT, (tx, ty, -FOOT), 170.0, FOOT + 430.0, rot=(0, 22.5, 0), steps=8)
        g.box(VIOLET, (tx, ty - 158.0, 150.0), (110.0, 12.0, 250.0))
        g.cyl(WHITE, (tx, ty, 430.0), 192.0, 30.0, rot=(0, 22.5, 0), steps=8)
        dome(g, tx, ty, 460.0, 176.0)
        px, py = s * 1180.0, cy - 1000.0                                                  # a pennant mast at each side
        g.box(WHITE, (px, py, -110.0), (90.0, 90.0, 340.0))
        g.cyl(WHITE, (px, py, 60.0), 10.0, 1100.0, steps=6)
        g.sphere(GOLD, (px, py, 1176.0), 20.0, steps=(3, 5))
        f = sc.part(P.motion(swing=12.0, pivot=(px, py, 1080.0), axis=P.UP, rate=0.25, phase=0.2 + 0.3 * s))
        f.box(MINT, (px + 170.0, py, 1080.0), (320.0, 5.0, 130.0))
        f.box(MINT, (px + 390.0, py, 1115.0), (120.0, 5.0, 60.0))
        f.box(MINT, (px + 390.0, py, 1045.0), (120.0, 5.0, 60.0))
    for k in range(8):                                                                    # the footlights (steady)
        a = math.radians(200.0 + 20.0 * k)
        x, y = math.cos(a) * (FR - 50.0), cy + math.sin(a) * (FR - 50.0)
        g.cyl(GOLD, (x, y, 60.0), 22.0, 14.0, steps=6)
        g.sphere(YEL, (x, y, 88.0), 24.0, steps=(3, 5), glow=0.8)
    fy = cy - FR - 400.0                                                                  # the fountain before it
    g.cyl(WHITE, (0, fy, -FOOT), 330.0, FOOT + 50.0, steps=12)
    g.cyl(POOL, (0, fy, 36.0), 300.0, 22.0, steps=12)
    g.cyl(WHITE, (0, fy, 50.0), 44.0, 150.0, steps=8, top_r=22.0)
    g.cyl(GOLD, (0, fy, 200.0), 30.0, 44.0, steps=10, top_r=130.0)
    for k in range(4):
        a = math.radians(45.0 + 90.0 * k)
        g.sphere(GOLD, (math.cos(a) * 316.0, fy + math.sin(a) * 316.0, 64.0), 24.0, steps=(3, 5))
    j = sc.part(P.motion(bob=-45.0, rate=0.4, phase=0.1))
    j.cyl(POOL, (0, fy, 236.0), 18.0, 520.0, steps=6)
    j.sphere(WHITE, (0, fy, 780.0), 52.0, steps=(3, 6))
    j.cyl(WHITE, (0, fy, 236.0), 60.0, 60.0, steps=6, top_r=20.0)


JOBS = [
    ('SM_S1D4_Casino', casino), ('SM_S1D4_Ballroom', ballroom), ('SM_S1D4_PalmCourt', palm_court),
    ('SM_S1D4_RoofGarden', roof_garden), ('SM_S1D4_Gatehouse', gatehouse), ('SM_S1D4_GrandHotel', grand_hotel),
    ('SM_S1D4_Belvedere', belvedere), ('SM_S1D4_Loggia', loggia), ('SM_S1D4_Rotunda', rotunda),
    ('SM_S1D4_SpaHouse', spa_house), ('SM_S1D4_BathPavilion', bath_pavilion),
    ('SM_S1D4_Fountain', fountain), ('SM_S1D4_Bandstand', bandstand), ('SM_S1D4_Flagpoles', flagpoles),
    ('SM_S1D4_Valet', valet), ('SM_S1D4_Lamp', lamp),
    ('SM_S1D4_Balustrade', balustrade, STILL), ('SM_S1D4_Trolley', trolley, STILL), ('SM_S1D4_TeaTables', tea_tables, STILL),
    ('SM_S1D4_Shell', shell),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
