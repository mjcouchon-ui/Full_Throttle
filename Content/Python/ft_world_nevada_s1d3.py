"""Nevada, Stage 1, builder s1d3: district 3 "Red Rock Pass and Last Chance Gas" (5.14 to 7.75 km).
/Game/World/Nevada/SM_S1D3_*. Run: import ft_world_nevada_s1d3 as K; K.run()

PAINTED ADOBE. Thick battered walls with rounded corners, flat roofs behind wavy white parapets, one to three stepped
storeys, roof-beam ends in a row, arched doors, round towers. Walls turquoise #3fd6d0, caps and surrounds #fff6ea,
doors and awnings #ffe14d, openings #233a63, pots and small trim #ff4fb8. Everything is our own: no words, no marks.

  buildings  TradingPost, Cantina, MotorCourt, FillingStation, RepairShop, PotShop (Last Chance Gas, a street 38 to
             45 m out) and CliffHouse, CliffTower (the pass: adobe rooms on a 17 m column of the cut's own rock)
  props      WaterJar, DryingRack, BrushShade, AirTower, LampPost, BlanketLine, PotStall (tall) and ClayOven,
             RadiatorStand, GardenWall (low)
  hero       BigPump: a 30 m antique fuel pump as a building, a white dial with one yellow hand, a yellow globe that
             glows steadily
Every piece is an ft_people.Scene: one material (M_People), one section, movement in the vertex shader only (bead
curtains, bells, strings of chillies, blankets, pennants, the dial's hand). Glow is steady and only on pieces the
table stands 24 m or more out (all of these stand 37 m or more out).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))       # a piece with no moving part: its true bounds

TQ, TQD, CAP, YEL, NAVY, MAG = '#3fd6d0', '#2fc6c0', '#fff6ea', '#ffe14d', '#233a63', '#ff4fb8'
LAMP, RED, GREY, DARK = '#ffc86a', '#ff3355', '#c9c4d6', '#2b2350'
WOOD, BARK, BARKD = '#9a6a44', '#8a6a4a', '#6a4c36'
ROCK, ROCKD, ROCKL = '#e0784a', '#b8553a', '#f59a62'
SAGE, SAGED, PUFF = '#8fb86a', '#5f8f4a', '#e8e0f0'
FOOT = 150.0                                            # how far walls go down under z = 0
WIN = 0.45                                              # a lit window (steady)
LANT = 0.7                                              # a lantern (steady)


# ------------------------------------------------------------------------------------------ the words of the kit
def _ccw(pts):
    """The polygon counter-clockwise, whichever way it was written."""
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def side_x(g, col, pts, y0, y1, glow=0.0):
    """A profile [(x, z)] seen from the road, extruded across y."""
    g.side(col, _ccw(pts), min(y0, y1), max(y0, y1), glow=glow)


def side_y(g, col, pts, x0, x1, glow=0.0):
    """A profile [(y, z)] seen from along the road, extruded along x."""
    g.side(col, _ccw(pts), -max(x0, x1), -min(x0, x1), glow=glow, rot=(0, 90, 0))


class Blk(object):
    """A battered block's faces: where its front (fy), back (by), left (lx) and right (rx) lie at a height."""

    def __init__(self, cx, cy, z0, z1, w, d, b):
        self.cx, self.cy, self.z0, self.z1, self.w, self.d, self.b = cx, cy, z0, z1, w, d, b

    def _t(self, z):
        return self.b * (z - self.z0) / float(self.z1 - self.z0)

    def fy(self, z):
        return self.cy - self.d / 2.0 + self._t(z)

    def by(self, z):
        return self.cy + self.d / 2.0 - self._t(z)

    def lx(self, z):
        return self.cx - self.w / 2.0 + self._t(z)

    def rx(self, z):
        return self.cx + self.w / 2.0 - self._t(z)


def block(g, col, cx, cy, z0, z1, w, d, b=36.0):
    """THE adobe wall: a block w x d at the foot whose four faces lean in by b to the top, its corners rounded (two
    crossed wedges and a cone in each corner)."""
    hw, hd = w / 2.0, d / 2.0
    side_x(g, col, [(cx - hw, z0), (cx + hw, z0), (cx + hw - b, z1), (cx - hw + b, z1)], cy - hd + b, cy + hd - b)
    side_y(g, col, [(cy - hd, z0), (cy + hd, z0), (cy + hd - b, z1), (cy - hd + b, z1)], cx - hw + b, cx + hw - b)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(col, (cx + sx * (hw - b), cy + sy * (hd - b), z0), b, z1 - z0, steps=8, top_r=3.0)
    return Blk(cx, cy, z0, z1, w, d, b)


def rrect(cx, cy, w, d, r, n=2):
    pts = []
    for sx, sy, a0 in ((1, 1, 0.0), (-1, 1, 90.0), (-1, -1, 180.0), (1, -1, 270.0)):
        ox, oy = cx + sx * (w / 2.0 - r), cy + sy * (d / 2.0 - r)
        for k in range(n + 1):
            a = math.radians(a0 + 90.0 * k / n)
            pts.append((ox + math.cos(a) * r, oy + math.sin(a) * r))
    return pts


def slab(g, col, cx, cy, z0, z1, w, d, r=30.0, n=2, glow=0.0):
    """A flat piece with rounded corners (a plinth, a roof, a step)."""
    g.plan(col, _ccw(rrect(cx, cy, w, d, r, n)), z0, z1, glow=glow)


def wave_pts(a0, a1, z, h, amp, waves, seg=3):
    """A strip from a0 to a1 standing on z, its top edge in waves."""
    pts = [(a0, z), (a1, z)]
    n = waves * seg
    for k in range(n + 1):
        t = k / float(n)
        pts.append((a1 + (a0 - a1) * t, z + h + amp * 0.5 * (1.0 - math.cos(2.0 * math.pi * waves * t))))
    return pts


def parapet(g, B, z=None, h=34.0, amp=18.0, t=30.0, col=CAP, ears=True, back=True):
    """The white wavy cap round a block's roof."""
    z = B.z1 if z is None else z
    x0, x1, y0, y1 = B.lx(z) - 8.0, B.rx(z) + 8.0, B.fy(z) - 8.0, B.by(z) + 8.0
    wx = max(2, int(round((x1 - x0) / 250.0)))
    wy = max(1, int(round((y1 - y0) / 250.0)))
    side_x(g, col, wave_pts(x0, x1, z - 14.0, h + 14.0, amp, wx), y0, y0 + t)
    if back:
        g.box(col, ((x0 + x1) / 2.0, y1 - t / 2.0, z + h / 2.0 - 7.0), (x1 - x0, t, h + 14.0))
    side_y(g, col, wave_pts(y0, y1, z - 14.0, h + 14.0, amp, wy), x0, x0 + t)
    side_y(g, col, wave_pts(y0, y1, z - 14.0, h + 14.0, amp, wy), x1 - t, x1)
    if ears:
        for sx in (x0 + t * 0.5, x1 - t * 0.5):
            g.cyl(col, (sx, y0 + t * 0.5, z - 14.0), t * 0.72, h + amp + 30.0, steps=6, top_r=t * 0.5)


def vigas(g, B, z, xs, out=44.0, r=9.0, col=BARK):
    """A row of round roof-beam ends sticking out of the front wall."""
    y = B.fy(z)
    for x in xs:
        rod(g, col, (x, y + 24.0, z), (x, y - out, z), r, steps=5)


def spread(x0, x1, n):
    return [x0 + (x1 - x0) * (k + 0.5) / n for k in range(n)]


def arch_pts(xc, z0, w, h, n=3):
    r = w / 2.0
    zs = z0 + h - r
    pts = [(xc - r, z0), (xc + r, z0), (xc + r, zs)]
    for k in range(1, n + 1):
        a = math.pi * k / (n + 1.0)
        pts.append((xc + math.cos(a) * r, zs + math.sin(a) * r))
    pts.append((xc - r, zs))
    return pts


def door(g, x, yf, z0, w=110.0, h=230.0, col=YEL, glow=0.0):
    """An arched door on the wall yf (facing -Y) in a white surround."""
    side_x(g, CAP, arch_pts(x, z0, w + 40.0, h + 20.0), yf - 7.0, yf + 40.0)
    side_x(g, col, arch_pts(x, z0, w, h), yf - 13.0, yf + 30.0, glow=glow)
    g.box(NAVY if col != NAVY else YEL, (x + w * 0.28, yf - 15.0, z0 + h * 0.45), (9.0, 6.0, 9.0))


def window(g, x, yf, z, w=70.0, h=90.0, glow=0.0, col=None):
    """A deep opening in a white surround (lit: warm and steady)."""
    g.box(CAP, (x, yf + 10.0, z), (w + 26.0, 44.0, h + 26.0))
    g.box(col or (LAMP if glow else NAVY), (x, yf - 3.0, z), (w, 28.0, h), glow=glow)


def window_x(g, xf, y, z, w=70.0, h=90.0, glow=0.0, sgn=1.0):
    """The same on an end wall (xf: the wall; sgn: +1 the right end, -1 the left)."""
    g.box(CAP, (xf - sgn * 10.0, y, z), (44.0, w + 26.0, h + 26.0))
    g.box(LAMP if glow else NAVY, (xf + sgn * 3.0, y, z), (28.0, w, h), glow=glow)


def awning(g, x0, x1, yf, z, depth=110.0, drop=45.0, cols=(YEL, CAP), n=None):
    """A striped awning hung on the wall yf, falling towards the road."""
    n = n or max(2, int(round((x1 - x0) / 60.0)))
    w = (x1 - x0) / float(n)
    for k in range(n):
        xc = x0 + (k + 0.5) * w
        tilt_box(g, cols[k % len(cols)], (xc, yf + 4.0, z), (xc, yf - depth, z - drop), w, 8.0)


def ladder(g, a, b, w=46.0, col=WOOD, rungs=5, across='x'):
    """A pole ladder from a to b; its rails stand apart along x (or y)."""
    dx, dy = (w / 2.0, 0.0) if across == 'x' else (0.0, w / 2.0)
    for s in (-1, 1):
        tilt_box(g, col, (a[0] + s * dx, a[1] + s * dy, a[2]), (b[0] + s * dx, b[1] + s * dy, b[2] + 40.0), 7.0, 7.0)
    for k in range(rungs):
        t = (k + 0.6) / (rungs + 0.2)
        size = (w + 12.0, 6.0, 6.0) if across == 'x' else (6.0, w + 12.0, 6.0)
        g.box(col, (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t), size)


def stair(g, col, x0, x1, z0, z1, y0, y1, n=7, zb=-FOOT):
    """A solid outside stair along the wall, rising from x0 (at z0) to x1 (at z1)."""
    dx, dz = (x1 - x0) / float(n), (z1 - z0) / float(n)
    pts = [(x0, zb)]
    for k in range(n):
        pts.append((x0 + k * dx, z0 + (k + 1) * dz))
        pts.append((x0 + (k + 1) * dx, z0 + (k + 1) * dz))
    pts.append((x1, zb))
    side_x(g, col, pts, y0, y1)


def pot(g, x, y, z, r=22.0, h=42.0, col=MAG, plant=False, rim=None):
    """A painted pot (two cones), a cactus in it if asked."""
    g.cyl(col, (x, y, z), r * 0.62, h * 0.55, steps=6, top_r=r)
    g.cyl(rim or col, (x, y, z + h * 0.55), r, h * 0.45, steps=6, top_r=r * 0.7)
    if plant:
        g.cyl(SAGE, (x, y, z + h), r * 0.36, r * 1.7, steps=5, top_r=r * 0.26)
        g.box(SAGED, (x + r * 0.5, y, z + h + r * 0.9), (r * 0.5, r * 0.3, r * 0.3))
    return z + h


def olla(g, x, y, z, r, h, col=TQ, band=CAP, trim=MAG, steps=10, marks=6):
    """A big painted water jar: a belly, a white band of magenta diamonds, a neck and a rim. Returns its top."""
    g.cyl(col, (x, y, z), r * 0.5, h * 0.36, steps=steps, top_r=r)
    g.cyl(band, (x, y, z + h * 0.36), r * 1.02, h * 0.16, steps=steps)
    g.cyl(col, (x, y, z + h * 0.52), r, h * 0.3, steps=steps, top_r=r * 0.46)
    g.cyl(YEL, (x, y, z + h * 0.82), r * 0.46, h * 0.1, steps=steps, top_r=r * 0.4)
    g.cyl(band, (x, y, z + h * 0.92), r * 0.56, h * 0.08, steps=steps)
    for k in range(marks):
        a = 2.0 * math.pi * (k + 0.5) / marks
        g.box(trim, (x + math.cos(a) * r * 1.02, y + math.sin(a) * r * 1.02, z + h * 0.44), (r * 0.2, r * 0.2, r * 0.2), rot=(45.0, math.degrees(a), 0.0))
    return z + h


def lantern(g, x, y, z, s=30.0, glow=LANT):
    """A four-sided lantern: a lit pane under a white cap."""
    g.box(CAP, (x, y, z - 4.0), (s + 12.0, s + 12.0, 8.0))
    g.box(LAMP, (x, y, z + s / 2.0), (s, s, s), glow=glow)
    g.cyl(CAP, (x, y, z + s), s * 0.85, s * 0.5, steps=4, top_r=4.0, rot=(0, 45, 0))


def motif(g, x, yf, z, s=70.0):
    """The district's sign without words: a white board, a magenta diamond, a yellow bar."""
    g.box(CAP, (x, yf - 6.0, z), (s * 1.7, 12.0, s))
    g.box(MAG, (x - s * 0.36, yf - 12.0, z), (s * 0.46, 8.0, s * 0.46), rot=(45, 0, 0))
    g.box(YEL, (x + s * 0.36, yf - 12.0, z), (s * 0.6, 8.0, s * 0.2))


def ristras(sc, pts, length=70.0, w=15.0, col=RED, swing=7.0, rate=0.45, axis=0.0, tie=YEL):
    """Strings of chillies hung by their tops: each swings a little on its own."""
    for k, (x, y, z) in enumerate(pts):
        m = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=axis, rate=rate + 0.04 * (k % 3), phase=0.37 * k))
        m.cyl(col, (x, y, z - length), w * 0.3, length, steps=5, top_r=w * 0.62)
        m.box(tie, (x, y, z - 4.0), (w * 0.5, w * 0.5, 10.0))


def pennant(sc, x, y, z, h=150.0, col=MAG, phase=0.2, length=110.0):
    """A mast and a pennant that swings slowly about it."""
    g = sc.still()
    g.cyl(GREY, (x, y, z), 4.5, h, steps=5)
    f = sc.part(P.motion(swing=14.0, pivot=(x, y, z + h), axis=P.UP, rate=0.22, phase=phase))
    f.box(col, (x + length * 0.5 + 4.0, y, z + h - 22.0), (length, 5.0, 40.0))
    f.box(CAP, (x + length * 0.5 + 4.0, y, z + h - 22.0), (length * 0.3, 7.0, 40.0))


def puff(sc, x, y, z, r=34.0, phase=0.0):
    """A pale puff over a chimney that floats up and down, slowly."""
    m = sc.part(P.motion(bob=-22.0, rate=0.14, phase=phase))
    m.sphere(PUFF, (x, y, z), r, steps=(4, 6))
    m.sphere(PUFF, (x + r * 0.7, y, z + r * 0.9), r * 0.62, steps=(3, 5))


def chimney(sc, x, y, z0, h=120.0, r=34.0, smoke=True, phase=0.0):
    g = sc.still()
    g.cyl(TQ, (x, y, z0), r, h, steps=7, top_r=r * 0.8)
    g.cyl(CAP, (x, y, z0 + h), r * 0.95, 16.0, steps=7)
    if smoke:
        puff(sc, x, y, z0 + h + 80.0, r * 0.95, phase)


def bench(g, x, y, z=0.0, w=150.0, col=WOOD):
    g.box(col, (x, y, z + 42.0), (w, 40.0, 8.0))
    for s in (-1, 1):
        g.box(col, (x + s * (w / 2.0 - 12.0), y, z + 19.0), (8.0, 36.0, 38.0))


def crown(g, x, y, z, r, n=6, col=CAP, steps=10):
    """A round tower's white cap: a ring and n rounded ears."""
    g.cyl(col, (x, y, z - 14.0), r + 9.0, 36.0, steps=steps)
    for k in range(n):
        a = 2.0 * math.pi * k / n
        g.box(col, (x + math.cos(a) * r, y + math.sin(a) * r, z + 40.0), (30.0, r * 0.62, 40.0), rot=(0.0, math.degrees(a), 0.0))


def column(g, r0, top, seed, top_r=None, tiers=6):
    """The cut's own rock, as a column: banded tiers of red sandstone from 3 m under the ground to `top`, a ledge
    under the cap. Returns the cap's radius."""
    rr = random.Random(seed)
    top_r = top_r or r0 * 0.8
    cols = (ROCK, ROCKD, ROCKL, ROCK, ROCKL, ROCKD)
    zb = -300.0
    for k in range(tiers):
        z0 = zb + (top - zb) * k / float(tiers)
        z1 = zb + (top - zb) * (k + 1) / float(tiers)
        ra = (r0 + (top_r - r0) * k / float(tiers)) * rr.uniform(0.95, 1.1)
        rb = (r0 + (top_r - r0) * (k + 1) / float(tiers)) * rr.uniform(0.88, 1.0)
        g.cyl(cols[k % 6], (rr.uniform(-18, 18), rr.uniform(-18, 18), z0), ra, z1 - z0 + 6.0, steps=7, top_r=rb, rot=(0.0, rr.uniform(0.0, 51.0), 0.0))
    g.cyl(ROCKD, (0, 0, top - 150.0), top_r * 1.08, 40.0, steps=7, rot=(0, 20, 0))
    g.cyl(ROCKL, (0, 0, top - 40.0), top_r * 1.04, 40.0, steps=8, top_r=top_r * 0.98)
    # two boulders at its foot and a fin leaning on it
    g.sphere(ROCKD, (r0 * 0.9, -r0 * 0.5, 40.0), r0 * 0.34, scale=(1.2, 1.0, 0.8), steps=(4, 6))
    g.sphere(ROCK, (-r0 * 0.95, r0 * 0.2, 20.0), r0 * 0.3, scale=(1.0, 1.3, 0.9), steps=(4, 6))
    g.cyl(ROCK, (-r0 * 0.75, -r0 * 0.62, -200.0), r0 * 0.36, top * 0.48 + 200.0, steps=5, top_r=r0 * 0.14, rot=(0, 30, 0))
    return top_r


def little_pump(g, x, y, z=0.0):
    """An old hand pump of the filling station, 2.2 m: a turquoise cabinet, a white dial, a small steady globe."""
    g.box(CAP, (x, y, z + 8.0), (62.0, 52.0, 16.0))
    g.box(TQ, (x, y, z + 88.0), (48.0, 38.0, 144.0))
    g.box(CAP, (x, y, z + 164.0), (54.0, 44.0, 10.0))
    rod(g, CAP, (x, y - 14.0, z + 122.0), (x, y - 24.0, z + 122.0), 16.0, steps=8)
    g.box(YEL, (x, y - 26.0, z + 128.0), (5.0, 4.0, 18.0))
    g.cyl(CAP, (x, y, z + 169.0), 12.0, 12.0, steps=6)
    g.sphere(YEL, (x, y, z + 200.0), 21.0, steps=(4, 6), glow=LANT)
    tilt_box(g, DARK, (x + 26.0, y, z + 130.0), (x + 40.0, y, z + 50.0), 6.0, 6.0)


# ------------------------------------------------------------------------------------------ the buildings
def trading_post(sc):
    """STEPPED ADOBE TRADING POST, 15 x 6.4 m, 8.9 m: three storeys stepped back to the right, an outside stair up
    to a roof terrace under a brush shade (two card players at a table), rows of beam ends, a striped awning over
    the shop window, blankets over the parapet, chillies by the door (they swing), a chimney with a pale puff."""
    g = sc.still()
    A = block(g, TQ, 0, 40, -FOOT, 330, 1500, 600, 44)
    B = block(g, TQ, 250, 90, 316, 610, 900, 470, 30)
    C = block(g, TQ, 440, 112, 596, 836, 420, 370, 24)
    parapet(g, A)
    parapet(g, B, ears=False)
    parapet(g, C, h=30.0, amp=16.0)
    vigas(g, A, 276.0, spread(-60.0, 700.0, 5))
    vigas(g, B, 556.0, spread(-150.0, 650.0, 5))
    vigas(g, C, 786.0, spread(270.0, 610.0, 3), out=36.0, r=8.0)
    yf = A.fy(0.0)
    door(g, 150.0, yf, 0.0, 120.0, 236.0)
    window(g, 470.0, A.fy(165.0), 165.0, 170.0, 110.0, glow=WIN)
    awning(g, 370.0, 570.0, A.fy(245.0), 258.0, 105.0, 46.0, n=4)
    window(g, 655.0, A.fy(170.0), 170.0, 56.0, 80.0)
    window(g, 40.0, B.fy(460.0), 460.0, 70.0, 92.0)
    window(g, 330.0, B.fy(460.0), 460.0, 70.0, 92.0, glow=WIN)
    door(g, 560.0, B.fy(330.0), 330.0, 84.0, 190.0, col=NAVY)
    door(g, 440.0, C.fy(610.0), 610.0, 76.0, 170.0)
    window_x(g, A.rx(170.0), 60.0, 170.0, 70.0, 90.0)
    # the outside stair, up the front to the terrace
    stair(g, TQD, -40.0, -640.0, 0.0, 330.0, yf - 100.0, yf + 12.0, n=8)
    g.box(CAP, (-340.0, yf - 93.0, 200.0), (660.0, 14.0, 64.0), rot=(-28.8, 0, 0))
    # the terrace: a brush shade on four posts, a table, two players
    for px in (-690.0, -330.0):
        for py in (-150.0, 190.0):
            g.cyl(WOOD, (px, py, 330.0), 7.0, 236.0, steps=5)
    g.box(BARK, (-510.0, 20.0, 572.0), (440.0, 420.0, 14.0))
    g.box(BARKD, (-500.0, 10.0, 586.0), (400.0, 360.0, 12.0), rot=(0, 9, 0))
    g.cyl(WOOD, (-510.0, 30.0, 330.0), 8.0, 68.0, steps=5)
    g.cyl(CAP, (-510.0, 30.0, 398.0), 46.0, 7.0, steps=8)
    P.figure(sc, -585.0, 30.0, 330.0, yaw=90.0, legs='sit', seat=40.0, detail=0, top=MAG, bottom=NAVY, skin=P.SKINS[3], hat='cowboy', hat_col=CAP,
             arm_r='hold', arm_l='down', rate=0.5, phase=0.2, swing_r=10.0, axis_r='x')
    P.figure(sc, -435.0, 30.0, 330.0, yaw=-90.0, legs='sit', seat=40.0, detail=0, top=YEL, bottom=DARK, skin=P.SKINS[1], hair='bun', hair_col=P.HAIRS[0],
             arm_l='hold', arm_r='wave', rate=0.6, phase=0.7, swing_r=16.0)
    for k, c in enumerate((MAG, CAP, YEL)):
        g.box(c, (-250.0 + k * 64.0, A.fy(330.0) - 14.0, 322.0), (60.0, 9.0, 96.0))
    ladder(g, (-100.0, B.fy(330.0) - 46.0, 330.0), (-100.0, B.fy(610.0) - 12.0, 612.0), rungs=5)
    chimney(sc, -120.0, 240.0, 606.0, 110.0, 30.0, phase=0.3)
    ristras(sc, [(58.0, yf - 16.0, 236.0), (82.0, yf - 16.0, 228.0), (242.0, yf - 16.0, 236.0)])
    lantern(g, 250.0, yf - 26.0, 120.0, 24.0)
    g.box(WOOD, (250.0, yf - 10.0, 112.0), (10.0, 34.0, 8.0))
    pot(g, 30.0, yf - 40.0, 0.0, 30.0, 56.0, plant=True)
    pot(g, 268.0, yf - 44.0, 0.0, 24.0, 44.0, col=CAP, rim=MAG)
    pot(g, 690.0, A.fy(0) - 36.0, 0.0, 28.0, 52.0, col=YEL, plant=True)


def cantina(sc):
    """ROUND-TOWER CANTINA, 11.4 x 6.4 m, 9.9 m: a round two-storey tower with a white crown and a pennant, a bead
    curtain in its arched door (the strands swing), round steps, a low wing with a roof terrace, a string of small
    steady lamps from the tower to a pole, a bench and pots."""
    g = sc.still()
    tx, ty, R = -270.0, 30.0, 290.0
    g.cyl(TQ, (tx, ty, -FOOT), R, 760.0 + FOOT, steps=12, top_r=R * 0.86)
    Rt = R * 0.86
    crown(g, tx, ty, 760.0, Rt, n=8, steps=12)
    g.cyl(TQD, (tx, ty, 746.0), Rt - 30.0, 30.0, steps=8)
    W = block(g, TQ, 260.0, 60.0, -FOOT, 350.0, 640.0, 500.0, 38)
    parapet(g, W)
    vigas(g, W, 296.0, spread(20.0, 520.0, 5))
    # the tower's beam ends, round it
    for k in range(7):
        a = math.radians(200.0 + k * 23.5)
        cx_, sy_ = math.cos(a), math.sin(a)
        rod(g, BARK, (tx + cx_ * (Rt + 4.0), ty + sy_ * (Rt + 4.0), 690.0), (tx + cx_ * (Rt + 62.0), ty + sy_ * (Rt + 62.0), 690.0), 9.0, steps=5)
    # the door: a deep arch, a bead curtain
    yd = ty - R + 6.0
    side_x(g, CAP, arch_pts(tx, 0.0, 190.0, 290.0), yd - 20.0, yd + 60.0)
    side_x(g, NAVY, arch_pts(tx, 0.0, 150.0, 268.0), yd - 26.0, yd + 50.0)
    beads = (MAG, YEL, CAP, MAG, YEL, CAP, MAG)
    for k in range(7):
        x = tx - 60.0 + k * 20.0
        m = sc.part(P.motion(swing=5.0 + (k % 3), pivot=(x, yd - 32.0, 232.0), axis=0.0, rate=0.5 + 0.06 * (k % 3), phase=0.29 * k))
        m.box(beads[k], (x, yd - 32.0, 232.0 - 82.0 - 6.0 * (k % 2)), (9.0, 6.0, 164.0 + 12.0 * (k % 2)))
    g.cyl(CAP, (tx, yd - 10.0, -FOOT), 150.0, FOOT + 14.0, steps=10)
    g.cyl(CAP, (tx, yd + 4.0, -FOOT), 108.0, FOOT + 30.0, steps=10)
    # tower windows: slits, one lit
    for k, (ang, z, lit) in enumerate(((235.0, 470.0, 0.0), (270.0, 520.0, WIN), (305.0, 470.0, 0.0), (270.0, 340.0, 0.0))):
        a = math.radians(ang)
        rr_ = R - (R - Rt) * (z + FOOT) / (760.0 + FOOT)
        px, py = tx + math.cos(a) * rr_, ty + math.sin(a) * rr_
        g.box(CAP, (px, py, z), (64.0, 36.0, 104.0), rot=(0.0, ang + 90.0, 0.0))
        g.box(LAMP if lit else NAVY, (px + math.cos(a) * 8.0, py + math.sin(a) * 8.0, z), (40.0, 30.0, 80.0), rot=(0.0, ang + 90.0, 0.0), glow=lit)
    pennant(sc, tx, ty, 776.0, 150.0, MAG, 0.4)
    # the wing: a door, a window, the terrace's rail of pots
    yw = W.fy(0.0)
    door(g, 180.0, yw, 0.0, 100.0, 220.0)
    window(g, 420.0, W.fy(170.0), 170.0, 110.0, 96.0, glow=WIN)
    awning(g, 340.0, 500.0, W.fy(240.0), 250.0, 90.0, 40.0, cols=(MAG, CAP), n=4)
    for k, c in enumerate((MAG, YEL, MAG)):
        pot(g, 80.0 + k * 180.0, W.fy(350.0) + 8.0, 384.0, 20.0, 36.0, col=c, plant=(k == 1))
    bench(g, 430.0, yw - 34.0)
    pot(g, 560.0, yw - 30.0, 0.0, 26.0, 50.0, plant=True)
    # a string of small lamps from the tower to a pole at the wing's end
    g.cyl(WOOD, (590.0, yw - 60.0, -60.0), 8.0, 520.0, steps=5)
    a0, a1 = (tx + 175.0, ty - 185.0, 600.0), (590.0, yw - 60.0, 450.0)
    pv = a0
    for k in range(1, 7):
        t = k / 6.0
        sag = 70.0 * math.sin(math.pi * t)
        p = (a0[0] + (a1[0] - a0[0]) * t, a0[1] + (a1[1] - a0[1]) * t, a0[2] + (a1[2] - a0[2]) * t - sag)
        tilt_box(g, DARK, pv, p, 5.0, 5.0)
        if k < 6:
            g.box((YEL, MAG, CAP)[k % 3], (p[0], p[1], p[2] - 14.0), (16.0, 16.0, 20.0), glow=LANT * 0.8)
        pv = p
    chimney(sc, 470.0, 230.0, 350.0, 130.0, 30.0, phase=0.6)


def motor_court(sc):
    """MOTOR COURT GATE, 17.4 x 5 m, 8.2 m: a thick arch between two lodge rooms, a stepped white gable over it
    with a yellow bell in its own little arch (the bell swings), a lantern under the arch, a lit window, benches,
    a yellow door to each room."""
    g = sc.still()
    L = block(g, TQ, -600.0, 30.0, -FOOT, 330.0, 540.0, 460.0, 36)
    R = block(g, TQ, 600.0, 30.0, -FOOT, 330.0, 540.0, 460.0, 36)
    for B in (L, R):
        parapet(g, B)
        vigas(g, B, 280.0, spread(B.cx - 200.0, B.cx + 200.0, 4))
    # the arch: a wall with a round-headed way through it, its gable stepping up in the middle
    x0, x1, r = -340.0, 340.0, 215.0
    pts = [(x0, -FOOT), (-r, -FOOT), (-r, 230.0)]
    for k in range(1, 6):
        a = math.pi * (1.0 - k / 6.0)
        pts.append((math.cos(a) * r, 230.0 + math.sin(a) * r))
    pts += [(r, 230.0), (r, -FOOT), (x1, -FOOT), (x1, 430.0), (190.0, 520.0), (190.0, 600.0), (130.0, 600.0), (130.0, 760.0),
            (-130.0, 760.0), (-130.0, 600.0), (-190.0, 600.0), (-190.0, 520.0), (x0, 430.0)]
    side_x(g, TQ, pts, -40.0, 60.0)
    # the gable's white cap, in steps
    for (cx_, w_, z_) in ((0.0, 290.0, 760.0), (-160.0, 80.0, 600.0), (160.0, 80.0, 600.0)):
        slab(g, CAP, cx_, 10.0, z_, z_ + 22.0, w_, 130.0, 20.0, 1)
    for s in (-1, 1):
        tilt_box(g, CAP, (s * 196.0, 10.0, 530.0), (s * 344.0, 10.0, 440.0), 120.0, 20.0)
    # the bell: a dark niche with a white arch, a yoke on two brackets, the bell swinging before it
    side_x(g, CAP, arch_pts(0.0, 560.0, 150.0, 170.0), -47.0, -20.0)
    side_x(g, NAVY, arch_pts(0.0, 566.0, 118.0, 148.0), -52.0, -20.0)
    g.box(WOOD, (0.0, -92.0, 722.0), (170.0, 14.0, 14.0))
    for s in (-1, 1):
        g.box(WOOD, (s * 74.0, -66.0, 722.0), (12.0, 64.0, 12.0))
    bell = sc.part(P.motion(swing=13.0, pivot=(0.0, -92.0, 716.0), axis=0.0, rate=0.32, phase=0.1))
    bell.cyl(YEL, (0.0, -92.0, 628.0), 44.0, 62.0, steps=8, top_r=23.0)
    bell.cyl(YEL, (0.0, -92.0, 690.0), 23.0, 24.0, steps=8, top_r=10.0)
    bell.cyl(DARK, (0.0, -92.0, 606.0), 9.0, 24.0, steps=5)
    motif(g, 0.0, -44.0, 486.0, 50.0)
    lantern(g, 0.0, 10.0, 396.0, 30.0)
    g.box(DARK, (0.0, 10.0, 436.0), (5.0, 5.0, 26.0))
    # the rooms
    for B, s in ((L, -1), (R, 1)):
        yb = B.fy(0.0)
        door(g, B.cx + s * 110.0, yb, 0.0, 96.0, 214.0)
        window(g, B.cx - s * 110.0, B.fy(165.0), 165.0, 96.0, 90.0, glow=(WIN if s > 0 else 0.0))
        bench(g, B.cx - s * 110.0, yb - 34.0, 0.0, 130.0)
        pot(g, B.cx + s * 215.0, yb - 30.0, 0.0, 24.0, 46.0, col=(MAG if s < 0 else YEL), plant=True)
        lantern(g, B.cx + s * 20.0, yb - 22.0, 190.0, 20.0)
        g.box(WOOD, (B.cx + s * 20.0, yb - 8.0, 184.0), (8.0, 30.0, 8.0))
    chimney(sc, -700.0, 150.0, 330.0, 120.0, 30.0, phase=0.1)
    chimney(sc, 700.0, 150.0, 330.0, 120.0, 30.0, smoke=False)
    ristras(sc, [(-386.0, -56.0, 300.0), (386.0, -56.0, 300.0)], length=84.0)


def filling_station(sc):
    """LAST CHANCE FILLING STATION, 14.2 x 6.6 m, 7.2 m: a wavy white canopy on two fat adobe columns over an
    island with two old hand pumps (small steady globes), a shop with a yellow door and a lit window, an ice box,
    oil tins, a roof board with the district's sign and no words. No vehicles under it."""
    g = sc.still()
    S = block(g, TQ, 400.0, 90.0, -FOOT, 350.0, 600.0, 460.0, 36)
    parapet(g, S)
    vigas(g, S, 298.0, spread(180.0, 620.0, 5))
    ys = S.fy(0.0)
    door(g, 300.0, ys, 0.0, 104.0, 226.0)
    window(g, 520.0, S.fy(170.0), 170.0, 130.0, 100.0, glow=WIN)
    window_x(g, S.rx(170.0), 90.0, 170.0, 80.0, 90.0)
    # the canopy on its columns
    cz = 400.0
    for cx_ in (-470.0, -110.0):
        g.cyl(TQ, (cx_, -70.0, -FOOT), 82.0, cz + FOOT, steps=10, top_r=58.0)
        g.cyl(CAP, (cx_, -70.0, cz - 40.0), 66.0, 40.0, steps=10, top_r=96.0)
        g.cyl(CAP, (cx_, -70.0, -FOOT), 96.0, FOOT + 16.0, steps=10)
    x0, x1, y0, y1 = -700.0, 130.0, -330.0, 250.0
    slab(g, TQ, (x0 + x1) / 2.0, (y0 + y1) / 2.0, cz, cz + 40.0, x1 - x0, y1 - y0, 70.0, 3)
    side_x(g, CAP, wave_pts(x0 + 20.0, x1 - 20.0, cz + 34.0, 30.0, 34.0, 4, 4), y0 + 6.0, y0 + 36.0)
    side_y(g, CAP, wave_pts(y0 + 20.0, y1 - 20.0, cz + 34.0, 30.0, 34.0, 3, 4), x1 - 36.0, x1 - 6.0)
    side_y(g, CAP, wave_pts(y0 + 20.0, y1 - 20.0, cz + 34.0, 30.0, 34.0, 3, 4), x0 + 6.0, x0 + 36.0)
    # the island and its pumps
    slab(g, CAP, -290.0, -70.0, -FOOT, 14.0, 500.0, 130.0, 50.0, 2)
    little_pump(g, -340.0, -70.0, 14.0)
    little_pump(g, -230.0, -70.0, 14.0)
    lantern(g, -290.0, -70.0, cz - 46.0, 30.0)
    g.box(DARK, (-290.0, -70.0, cz - 8.0), (5.0, 5.0, 20.0))
    # the shop's things: an ice box, tins, a bench, the roof board
    g.box(CAP, (160.0, ys - 40.0, 48.0), (110.0, 62.0, 96.0))
    g.box(MAG, (160.0, ys - 40.0, 100.0), (116.0, 68.0, 10.0))
    g.box(TQD, (160.0, ys - 73.0, 52.0), (70.0, 6.0, 44.0))
    for k in range(3):
        g.cyl((YEL, MAG, CAP)[k], (410.0 + k * 34.0, ys - 30.0, 0.0), 13.0, 34.0, steps=6)
    g.cyl(YEL, (427.0, ys - 30.0, 34.0), 13.0, 34.0, steps=6)
    pot(g, 660.0, ys - 34.0, 0.0, 28.0, 52.0, plant=True)
    for sx in (270.0, 530.0):
        g.cyl(GREY, (sx, 60.0, 350.0), 6.0, 190.0, steps=5)
    g.box(CAP, (400.0, 56.0, 590.0), (380.0, 14.0, 130.0))
    g.box(TQ, (400.0, 48.0, 590.0), (340.0, 8.0, 34.0))
    g.box(MAG, (290.0, 46.0, 590.0), (64.0, 8.0, 64.0), rot=(45, 0, 0))
    g.box(YEL, (500.0, 46.0, 590.0), (110.0, 8.0, 30.0))
    tilt_box(g, YEL, (566.0, 46.0, 590.0), (540.0, 46.0, 622.0), 8.0, 16.0)
    tilt_box(g, YEL, (566.0, 46.0, 590.0), (540.0, 46.0, 558.0), 8.0, 16.0)
    ristras(sc, [(232.0, ys - 16.0, 250.0), (368.0, ys - 16.0, 250.0)], length=64.0)
    chimney(sc, 600.0, 240.0, 350.0, 120.0, 28.0, phase=0.8)


def repair_shop(sc):
    """REPAIR SHOP UNDER THE BIG JAR, 12.4 x 6.2 m, 9.2 m: a tall adobe shed, a wide arched doorway with a yellow
    slatted door half up, a giant painted water jar on the roof (turquoise, a white band of magenta diamonds), a
    stack of tyres, a side lean-to with a bench and a hanging lamp, a stove pipe."""
    g = sc.still()
    A = block(g, TQ, -90.0, 40.0, -FOOT, 420.0, 900.0, 560.0, 44)
    parapet(g, A)
    vigas(g, A, 366.0, spread(-460.0, 280.0, 7))
    yf = A.fy(0.0)
    # the wide doorway
    side_x(g, CAP, arch_pts(-200.0, 0.0, 400.0, 330.0, 5), yf - 7.0, yf + 50.0)
    side_x(g, NAVY, arch_pts(-200.0, 0.0, 350.0, 304.0, 5), yf - 13.0, yf + 40.0)
    for k in range(4):
        zk = 286.0 - k * 34.0
        wk = 2.0 * math.sqrt(175.0 ** 2 - (zk - 129.0) ** 2) - 12.0
        g.box(YEL if k % 2 == 0 else CAP, (-200.0, yf - 18.0, zk), (wk, 8.0, 32.0))
    door(g, 190.0, yf, 0.0, 90.0, 210.0)
    window(g, 190.0, A.fy(320.0), 320.0, 80.0, 60.0, glow=WIN)
    # the jar on the roof, on a white ring
    g.cyl(CAP, (-150.0, 60.0, 420.0), 150.0, 26.0, steps=10)
    olla(g, -150.0, 60.0, 440.0, 190.0, 470.0, steps=10, marks=7)
    # tyres, a drum, the stove pipe
    for k in range(3):
        g.cyl(DARK, (330.0, yf - 60.0, k * 24.0), 40.0, 22.0, steps=8)
    g.cyl(DARK, (400.0, yf - 34.0, 40.0), 40.0, 22.0, steps=8, rot=(0, 0, 80))
    g.cyl(GREY, (240.0, 220.0, 420.0), 14.0, 200.0, steps=6)
    g.cyl(GREY, (240.0, 220.0, 620.0), 24.0, 16.0, steps=6, top_r=6.0)
    # the lean-to at the right end
    for px, py in ((420.0, -200.0), (590.0, -200.0), (590.0, 180.0)):
        g.cyl(WOOD, (px, py, -60.0), 8.0, 320.0, steps=5)
    g.box(BARK, (488.0, -10.0, 266.0), (270.0, 460.0, 14.0), rot=(0, 0, -5))
    g.box(BARKD, (486.0, -16.0, 280.0), (240.0, 400.0, 12.0), rot=(0, 6, -5))
    g.box(WOOD, (500.0, 60.0, 84.0), (150.0, 70.0, 10.0))
    for s in (-1, 1):
        g.box(WOOD, (500.0 + s * 62.0, 60.0, 40.0), (10.0, 60.0, 80.0))
    g.box(GREY, (480.0, 60.0, 102.0), (40.0, 30.0, 26.0))
    lantern(g, 500.0, -80.0, 196.0, 24.0)
    g.box(DARK, (500.0, -80.0, 240.0), (5.0, 5.0, 44.0))
    pot(g, -470.0, yf - 36.0, 0.0, 28.0, 52.0, plant=True)
    ristras(sc, [(60.0, yf - 16.0, 250.0), (88.0, yf - 16.0, 240.0)])
    pennant(sc, -440.0, 220.0, 430.0, 330.0, YEL, 0.7)


def pot_shop(sc):
    """POT SHOP, 7 x 4.3 m, 5.2 m: one adobe room, a striped awning on two posts, shelves of painted pots in front,
    a big jar on each front corner of the roof, a lit doorway, a pennant."""
    g = sc.still()
    A = block(g, TQ, 0.0, 40.0, -FOOT, 300.0, 640.0, 360.0, 32)
    parapet(g, A, h=30.0, amp=16.0)
    vigas(g, A, 250.0, spread(-250.0, 250.0, 5), out=36.0, r=8.0)
    yf = A.fy(0.0)
    door(g, -150.0, yf, 0.0, 100.0, 214.0, col=LAMP, glow=WIN)
    window(g, 130.0, A.fy(150.0), 150.0, 190.0, 100.0)
    # the awning on two posts
    for px in (-20.0, 280.0):
        g.cyl(WOOD, (px, yf - 150.0, -60.0), 7.0, 270.0, steps=5)
    awning(g, -40.0, 300.0, A.fy(240.0), 250.0, 160.0, 42.0, n=5)
    # the shelves: three steps of pots
    for k in range(3):
        g.box(CAP, (130.0, yf - 40.0 - k * 34.0, (3 - k) * 26.0 - 30.0), (300.0, 36.0, (3 - k) * 52.0 + 60.0))
    cols = (MAG, YEL, TQ, CAP, MAG, TQ, YEL, MAG, CAP)
    for k in range(9):
        row, i = k // 3, k % 3
        g.cyl(cols[k], (30.0 + i * 100.0 + row * 12.0, yf - 40.0 - (2 - row) * 34.0, (row + 1) * 52.0 - 4.0), 12.0, 30.0, steps=6, top_r=19.0)
    olla(g, -250.0, -40.0, 300.0, 54.0, 150.0, col=MAG, band=CAP, trim=YEL, steps=8, marks=4)
    olla(g, 250.0, -40.0, 300.0, 54.0, 150.0, col=YEL, band=CAP, trim=MAG, steps=8, marks=4)
    pot(g, -260.0, yf - 34.0, 0.0, 30.0, 58.0, col=TQD, rim=CAP, plant=True)
    pennant(sc, 0.0, 150.0, 300.0, 200.0, MAG, 0.15, 90.0)


def cliff_house(sc):
    """CLIFF HOUSE (the pass and its cuts), 24.6 m: three adobe rooms stacked and stepped on a 17 m column of the
    cut's own red rock, pole ladders between them and one down the rock, beam ends, white caps, a lit door and a lit
    window (steady), a line of chillies that swings, a pennant."""
    g = sc.still()
    Rt = column(g, 540.0, 1700.0, 31, 430.0)
    Z = 1700.0
    A = block(g, TQ, -40.0, 30.0, Z - 30.0, Z + 270.0, 600.0, 440.0, 34)
    B = block(g, TQ, 60.0, 80.0, Z + 258.0, Z + 510.0, 400.0, 330.0, 26)
    C = block(g, TQ, 130.0, 110.0, Z + 498.0, Z + 700.0, 240.0, 230.0, 20)
    parapet(g, A)
    parapet(g, B, ears=False)
    parapet(g, C, h=28.0, amp=14.0)
    vigas(g, A, Z + 222.0, spread(-290.0, 210.0, 5))
    vigas(g, B, Z + 462.0, spread(-90.0, 210.0, 3))
    door(g, -190.0, A.fy(Z), Z, 96.0, 190.0, col=LAMP, glow=WIN)
    window(g, 80.0, A.fy(Z + 140.0), Z + 140.0, 80.0, 84.0)
    window(g, 150.0, B.fy(Z + 390.0), Z + 390.0, 76.0, 80.0, glow=WIN)
    door(g, -40.0, B.fy(Z + 270.0), Z + 270.0, 70.0, 160.0)
    window(g, 130.0, C.fy(Z + 600.0), Z + 600.0, 60.0, 70.0)
    window_x(g, A.lx(Z + 140.0), 30.0, Z + 140.0, 70.0, 84.0, sgn=-1.0)
    ladder(g, (-110.0, A.fy(Z + 270.0) + 26.0, Z + 270.0), (-110.0, B.fy(Z + 510.0) - 10.0, Z + 512.0), rungs=4)
    ladder(g, (60.0, B.fy(Z + 510.0) + 20.0, Z + 510.0), (60.0, C.fy(Z + 700.0) - 6.0, Z + 700.0), rungs=3)
    # the long ladder down the rock, in two flights with a ledge
    ladder(g, (0.0, -Rt - 130.0, 880.0), (0.0, -Rt + 6.0, Z - 4.0), rungs=9, w=50.0)
    g.box(ROCKL, (-20.0, -Rt - 90.0, 860.0), (260.0, 200.0, 36.0))
    ladder(g, (-70.0, -Rt - 330.0, 0.0), (-70.0, -Rt - 170.0, 870.0), rungs=9, w=50.0)
    # the line of chillies, a pennant, pots on the caps
    g.cyl(WOOD, (-300.0, -150.0, Z + 270.0), 6.0, 150.0, steps=5)
    tilt_box(g, DARK, (-300.0, -150.0, Z + 410.0), (-130.0, B.fy(Z + 410.0), Z + 420.0), 5.0, 5.0)
    ristras(sc, [(-270.0, -150.0 + 12.0, Z + 410.0), (-235.0, -150.0 + 26.0, Z + 412.0), (-200.0, -150.0 + 40.0, Z + 414.0)], length=74.0, w=18.0)
    pennant(sc, 130.0, 110.0, Z + 700.0, 190.0, MAG, 0.5, 130.0)
    pot(g, 190.0, A.fy(Z + 270.0) + 30.0, Z + 270.0, 24.0, 44.0, col=MAG, plant=True)
    pot(g, -300.0, -Rt * 0.55, Z, 30.0, 56.0, col=YEL, rim=CAP)
    chimney(sc, -200.0, 170.0, Z + 270.0, 100.0, 28.0, phase=0.45)


def cliff_tower(sc):
    """CLIFF TOWER (the pass and its cuts), 26.9 m: a round adobe watch tower on a 17 m needle of red rock, beam
    ends round it for steps, a white crown, a lamp room under a flat roof on four posts (one steady yellow lamp), a
    small room beside it, a pole ladder up the rock, a rack of chillies that swings, a pennant."""
    g = sc.still()
    Rt = column(g, 430.0, 1700.0, 47, 330.0)
    Z = 1700.0
    tx, ty, R, H = -60.0, 40.0, 190.0, 560.0
    g.cyl(TQ, (tx, ty, Z - 30.0), R, H + 30.0, steps=10, top_r=R * 0.82)
    Rtop = R * 0.82
    crown(g, tx, ty, Z + H, Rtop, n=6)
    # the lamp room
    for k in range(4):
        a = math.radians(45.0 + 90.0 * k)
        g.cyl(CAP, (tx + math.cos(a) * 100.0, ty + math.sin(a) * 100.0, Z + H), 10.0, 150.0, steps=5)
    g.cyl(CAP, (tx, ty, Z + H + 150.0), 160.0, 24.0, steps=10)
    g.cyl(TQ, (tx, ty, Z + H + 174.0), 150.0, 46.0, steps=10, top_r=30.0)
    g.box(LAMP, (tx, ty, Z + H + 76.0), (70.0, 70.0, 90.0), glow=0.85)
    g.box(CAP, (tx, ty, Z + H + 22.0), (90.0, 90.0, 18.0))
    pennant(sc, tx, ty, Z + H + 216.0, 170.0, MAG, 0.3, 130.0)
    # beam-end steps winding up the tower, a door, slit windows
    for k in range(9):
        a = math.radians(300.0 - k * 27.0)
        z = Z + 90.0 + k * 50.0
        rr_ = R - (R - Rtop) * (z - Z + 30.0) / (H + 30.0)
        rod(g, BARK, (tx + math.cos(a) * (rr_ - 10.0), ty + math.sin(a) * (rr_ - 10.0), z), (tx + math.cos(a) * (rr_ + 56.0), ty + math.sin(a) * (rr_ + 56.0), z), 9.0, steps=5)
    yd = ty - R + 4.0
    side_x(g, CAP, arch_pts(tx, Z, 124.0, 196.0), yd - 16.0, yd + 50.0)
    side_x(g, YEL, arch_pts(tx, Z, 88.0, 176.0), yd - 22.0, yd + 40.0)
    for ang, z, lit in ((240.0, Z + 380.0, WIN), (290.0, Z + 300.0, 0.0), (270.0, Z + 470.0, 0.0)):
        a = math.radians(ang)
        rr_ = R - (R - Rtop) * (z - Z + 30.0) / (H + 30.0)
        px, py = tx + math.cos(a) * rr_, ty + math.sin(a) * rr_
        g.box(CAP, (px, py, z), (58.0, 34.0, 92.0), rot=(0.0, ang + 90.0, 0.0))
        g.box(LAMP if lit else NAVY, (px + math.cos(a) * 8.0, py + math.sin(a) * 8.0, z), (36.0, 28.0, 70.0), rot=(0.0, ang + 90.0, 0.0), glow=lit)
    # the small room beside it
    B = block(g, TQ, 170.0, 60.0, Z - 30.0, Z + 230.0, 260.0, 300.0, 24)
    parapet(g, B, h=28.0, amp=14.0, ears=False)
    vigas(g, B, Z + 186.0, spread(90.0, 250.0, 3), out=36.0, r=8.0)
    window(g, 170.0, B.fy(Z + 120.0), Z + 120.0, 70.0, 80.0)
    pot(g, 250.0, B.fy(Z + 230.0) + 40.0, Z + 230.0, 24.0, 44.0, col=MAG, plant=True)
    # the rack of chillies at the rim, the ladder down the rock
    for px in (60.0, 190.0):
        g.cyl(WOOD, (px, -215.0, Z), 6.0, 170.0, steps=5)
    g.box(WOOD, (125.0, -215.0, Z + 166.0), (170.0, 8.0, 8.0))
    ristras(sc, [(80.0, -215.0, Z + 160.0), (110.0, -215.0, Z + 160.0), (140.0, -215.0, Z + 160.0), (170.0, -215.0, Z + 160.0)], length=80.0, w=18.0)
    ladder(g, (-60.0, -Rt - 130.0, 900.0), (-60.0, -Rt + 10.0, Z - 4.0), rungs=9, w=50.0)
    g.box(ROCKL, (-80.0, -Rt - 80.0, 880.0), (250.0, 190.0, 36.0))
    ladder(g, (-130.0, -Rt - 300.0, 0.0), (-130.0, -Rt - 150.0, 890.0), rungs=9, w=50.0)


# ------------------------------------------------------------------------------------------ the props
def water_jar(sc):
    """GIANT PAINTED WATER JAR ON A PLINTH, 4.9 m: a turquoise jar with a white band of magenta diamonds on a white
    stepped plinth, a dipper on a post beside it."""
    g = sc.still()
    slab(g, CAP, 0.0, 0.0, -FOOT, 50.0, 250.0, 250.0, 60.0, 2)
    slab(g, TQD, 0.0, 0.0, 50.0, 100.0, 190.0, 190.0, 50.0, 2)
    olla(g, 0.0, 0.0, 100.0, 120.0, 390.0, steps=10, marks=6)
    g.cyl(WOOD, (150.0, -60.0, -60.0), 6.0, 250.0, steps=5)
    g.box(WOOD, (150.0, -76.0, 180.0), (8.0, 40.0, 8.0))
    g.cyl(YEL, (150.0, -96.0, 140.0), 14.0, 30.0, steps=6, top_r=18.0)


def drying_rack(sc):
    """DRYING RACK, 3.4 m: two forked posts and two rails, eight strings of red chillies and yellow corn hung from
    them (each swings), baskets underneath."""
    g = sc.still()
    for s in (-1, 1):
        g.cyl(WOOD, (s * 170.0, 0.0, -80.0), 9.0, 400.0, steps=5, top_r=7.0)
        tilt_box(g, WOOD, (s * 170.0, 0.0, 300.0), (s * 150.0, -26.0, 340.0), 8.0, 8.0)
        tilt_box(g, WOOD, (s * 170.0, 0.0, 300.0), (s * 190.0, 26.0, 340.0), 8.0, 8.0)
        tilt_box(g, WOOD, (s * 170.0, 0.0, 120.0), (s * 120.0, 0.0, -40.0), 8.0, 8.0)
    g.box(BARK, (0.0, 0.0, 306.0), (400.0, 9.0, 9.0))
    g.box(BARK, (0.0, 0.0, 216.0), (350.0, 8.0, 8.0))
    top = [(-140.0 + k * 56.0, 0.0, 300.0) for k in range(6)]
    ristras(sc, top[0::2], length=92.0, w=20.0, col=RED)
    ristras(sc, top[1::2], length=78.0, w=18.0, col=YEL, tie=RED, rate=0.38)
    ristras(sc, [(-84.0, 0.0, 212.0), (84.0, 0.0, 212.0)], length=66.0, w=18.0, col=RED, rate=0.52)
    g.cyl(BARK, (-60.0, -50.0, 0.0), 30.0, 28.0, steps=7, top_r=38.0)
    g.cyl(RED, (-60.0, -50.0, 28.0), 32.0, 8.0, steps=7)
    g.cyl(BARK, (40.0, -40.0, 0.0), 24.0, 24.0, steps=7, top_r=30.0)
    g.cyl(YEL, (40.0, -40.0, 24.0), 25.0, 8.0, steps=7)


def brush_shade(sc):
    """BRUSH SHADE, 3.2 m: a roof of brush on four crooked posts, an ice box (white, a magenta lid), a bench, a
    canvas water bag hung from a beam (it swings), a pot."""
    g = sc.still()
    for sx, sy, lean in ((-1, -1, 10.0), (1, -1, -8.0), (-1, 1, 6.0), (1, 1, -12.0)):
        tilt_box(g, WOOD, (sx * 180.0 + lean, sy * 120.0, -80.0), (sx * 180.0, sy * 120.0, 270.0), 14.0, 14.0)
    g.box(WOOD, (0.0, -120.0, 268.0), (400.0, 10.0, 10.0))
    g.box(WOOD, (0.0, 120.0, 268.0), (400.0, 10.0, 10.0))
    g.box(BARK, (0.0, 0.0, 284.0), (440.0, 300.0, 16.0))
    g.box(BARKD, (-10.0, 6.0, 300.0), (400.0, 250.0, 14.0), rot=(0, 7, 0))
    g.box(SAGED, (30.0, -10.0, 313.0), (300.0, 170.0, 10.0), rot=(0, -11, 0))
    g.box(CAP, (-90.0, 40.0, 44.0), (110.0, 64.0, 88.0))
    g.box(MAG, (-90.0, 40.0, 92.0), (116.0, 70.0, 10.0))
    g.box(TQ, (-90.0, 6.0, 46.0), (70.0, 6.0, 40.0))
    bench(g, 80.0, 50.0, 0.0, 150.0)
    bag = sc.part(P.motion(swing=8.0, pivot=(120.0, -120.0, 262.0), axis=0.0, rate=0.4, phase=0.3))
    bag.box(DARK, (120.0, -120.0, 236.0), (5.0, 5.0, 52.0))
    bag.cyl(CAP, (120.0, -120.0, 150.0), 20.0, 60.0, steps=6, top_r=13.0)
    pot(g, -160.0, -90.0, 0.0, 22.0, 42.0, plant=True)


def air_tower(sc):
    """AIR TOWER, 5.4 m: a battered adobe post with a white cap and a yellow ball, a white gauge dial, a yoke arm
    with a yellow bell (it swings), a coiled hose on a hook."""
    g = sc.still()
    g.cyl(CAP, (0.0, 0.0, -FOOT), 62.0 * 1.4142, FOOT + 24.0, steps=4, rot=(0, 45, 0))
    g.cyl(TQ, (0.0, 0.0, 24.0), 46.0 * 1.4142, 400.0, steps=4, top_r=32.0 * 1.4142, rot=(0, 45, 0))
    slab(g, CAP, 0.0, 0.0, 424.0, 450.0, 92.0, 92.0, 20.0, 1)
    g.cyl(TQ, (0.0, 0.0, 450.0), 20.0, 40.0, steps=6, top_r=12.0)
    g.sphere(YEL, (0.0, 0.0, 512.0), 26.0, steps=(4, 6), glow=0.5)
    rod(g, CAP, (0.0, -30.0, 300.0), (0.0, -46.0, 300.0), 30.0, steps=8)
    g.box(NAVY, (0.0, -48.0, 308.0), (5.0, 4.0, 26.0), rot=(30, 0, 0))
    # the yoke arm and its bell
    g.box(WOOD, (70.0, 0.0, 396.0), (150.0, 12.0, 12.0))
    tilt_box(g, WOOD, (30.0, 0.0, 330.0), (120.0, 0.0, 392.0), 9.0, 9.0)
    bell = sc.part(P.motion(swing=14.0, pivot=(126.0, 0.0, 390.0), axis=0.0, rate=0.36, phase=0.55))
    bell.cyl(YEL, (126.0, 0.0, 318.0), 32.0, 46.0, steps=8, top_r=16.0)
    bell.cyl(YEL, (126.0, 0.0, 364.0), 16.0, 22.0, steps=6, top_r=6.0)
    bell.cyl(DARK, (126.0, 0.0, 302.0), 7.0, 18.0, steps=5)
    # the hose: three coils on a hook, one end down to the ground
    g.box(WOOD, (-52.0, 0.0, 210.0), (40.0, 10.0, 10.0))
    for k in range(3):
        rod(g, MAG, (-58.0 - k * 9.0, 0.0, 170.0), (-66.0 - k * 9.0, 0.0, 170.0), 44.0 - k * 3.0, steps=8)
    tilt_box(g, MAG, (-80.0, -20.0, 130.0), (-96.0, -60.0, 6.0), 8.0, 8.0)


def lamp_post(sc):
    """GATE POST AND LANTERN, 3.9 m: a fat adobe pillar with a white cap and a lit niche (steady), a pot with a
    cactus on top, a short wavy wall wing each side."""
    g = sc.still()
    g.cyl(TQ, (0.0, 0.0, -FOOT), 50.0 * 1.4142, 300.0 + FOOT, steps=4, top_r=38.0 * 1.4142, rot=(0, 45, 0))
    slab(g, CAP, 0.0, 0.0, 300.0, 326.0, 100.0, 100.0, 22.0, 1)
    g.box(CAP, (0.0, -34.0, 216.0), (58.0, 22.0, 74.0))
    g.box(LAMP, (0.0, -40.0, 216.0), (40.0, 16.0, 56.0), glow=LANT)
    pot(g, 0.0, 0.0, 326.0, 22.0, 40.0, col=MAG, plant=True)
    for s in (-1, 1):
        side_x(g, TQ, wave_pts(s * 40.0, s * 150.0, -FOOT, FOOT + 86.0, 26.0, 1, 4), -16.0, 16.0)
        side_x(g, CAP, wave_pts(s * 40.0, s * 150.0, 86.0, 6.0, 26.0, 1, 4), -20.0, 20.0)


def blanket_line(sc):
    """BLANKET LINE, 3.3 m: two posts and a line, four striped blankets and a rug over it (they swing a little),
    a basket."""
    g = sc.still()
    for s in (-1, 1):
        g.cyl(WOOD, (s * 240.0, 0.0, -80.0), 8.0, 400.0, steps=5)
        tilt_box(g, WOOD, (s * 240.0, 0.0, 180.0), (s * 300.0, 0.0, -40.0), 7.0, 7.0)
    g.box(DARK, (0.0, 0.0, 306.0), (480.0, 5.0, 5.0))
    looks = ((MAG, CAP, YEL), (TQ, CAP, MAG), (YEL, NAVY, CAP), (CAP, MAG, TQ))
    for k, cols in enumerate(looks):
        x = -168.0 + k * 112.0
        m = sc.part(P.motion(swing=5.0 + k % 2, pivot=(x, 0.0, 306.0), axis=0.0, rate=0.3 + 0.05 * (k % 3), phase=0.31 * k))
        h = 150.0 + 26.0 * (k % 2)
        m.box(cols[0], (x, -5.0, 306.0 - h / 2.0), (92.0, 7.0, h))
        m.box(cols[1], (x, -9.0, 306.0 - h * 0.3), (92.0, 5.0, 20.0))
        m.box(cols[2], (x, -9.0, 306.0 - h * 0.62), (92.0, 5.0, 26.0))
        m.box(cols[1], (x, -9.0, 306.0 - h * 0.9), (92.0, 5.0, 12.0))
    g.cyl(BARK, (150.0, -60.0, 0.0), 30.0, 30.0, steps=7, top_r=38.0)
    g.box(CAP, (150.0, -60.0, 34.0), (46.0, 40.0, 14.0))


def pot_stall(sc):
    """POT STALL, 3.1 m: a table under a striped awning on four posts, three steps of painted pots, a rug over the
    front rail, two strings of chillies (they swing), big jars on the ground."""
    g = sc.still()
    for sx in (-1, 1):
        g.cyl(WOOD, (sx * 190.0, -90.0, -80.0), 7.0, 330.0, steps=5)
        g.cyl(WOOD, (sx * 190.0, 90.0, -80.0), 7.0, 390.0, steps=5)
    for k in range(6):
        xc = -200.0 + (k + 0.5) * 400.0 / 6.0
        tilt_box(g, (YEL, CAP)[k % 2], (xc, 100.0, 306.0), (xc, -110.0, 246.0), 400.0 / 6.0, 8.0)
    g.box(WOOD, (0.0, 20.0, 60.0), (360.0, 120.0, 10.0))
    for sx in (-1, 1):
        g.box(WOOD, (sx * 160.0, 20.0, 12.0), (10.0, 110.0, 150.0))
    g.box(CAP, (0.0, 50.0, 84.0), (340.0, 50.0, 40.0))
    g.box(CAP, (0.0, 70.0, 122.0), (340.0, 26.0, 40.0))
    cols = (MAG, YEL, TQ, CAP, MAG, YEL, TQ, MAG)
    for k in range(5):
        g.cyl(cols[k], (-140.0 + k * 70.0, -6.0, 65.0), 12.0, 30.0, steps=6, top_r=19.0)
    for k in range(4):
        g.cyl(cols[k + 3], (-105.0 + k * 70.0, 44.0, 104.0), 11.0, 28.0, steps=6, top_r=17.0)
    g.box(MAG, (-90.0, -44.0, 30.0), (110.0, 7.0, 70.0))
    g.box(CAP, (-90.0, -48.0, 36.0), (110.0, 5.0, 14.0))
    g.box(TQ, (60.0, -44.0, 34.0), (90.0, 7.0, 60.0))
    g.box(YEL, (60.0, -48.0, 30.0), (90.0, 5.0, 12.0))
    ristras(sc, [(-150.0, -96.0, 244.0), (150.0, -96.0, 244.0)], length=70.0, w=17.0)
    pot(g, 230.0, -50.0, 0.0, 30.0, 60.0, col=TQ, rim=CAP)
    pot(g, -236.0, -30.0, 0.0, 26.0, 50.0, col=YEL, rim=MAG)


def clay_oven(sc):
    """CLAY OVEN AND WOOD PILE, LOW (1.7 m): a white-capped beehive oven on a turquoise base, a dark door, a
    wood pile, a baker's peel leaning, a tray of loaves."""
    g = sc.still()
    slab(g, TQD, -40.0, 0.0, -60.0, 46.0, 230.0, 210.0, 50.0, 2)
    g.sphere(TQ, (-40.0, 0.0, 46.0), 92.0, scale=(1.0, 1.0, 1.12), steps=(6, 9))
    g.cyl(CAP, (-40.0, 0.0, 140.0), 26.0, 18.0, steps=7, top_r=20.0)
    side_x(g, CAP, arch_pts(-40.0, 46.0, 72.0, 78.0), -100.0, -60.0)
    side_x(g, NAVY, arch_pts(-40.0, 46.0, 48.0, 62.0), -104.0, -70.0)
    r = random.Random(4)
    for k in range(7):
        row = 0 if k < 4 else 1
        x = 120.0 + (k if k < 4 else k - 3.5) * 22.0
        rod(g, BARK if k % 2 else WOOD, (x, -55.0 + r.uniform(-6, 6), 12.0 + row * 22.0), (x, 55.0 + r.uniform(-6, 6), 12.0 + row * 22.0), 11.0, steps=5)
    g.box(BARKD, (156.0, 0.0, -28.0), (120.0, 130.0, 64.0))
    tilt_box(g, WOOD, (-150.0, -70.0, 0.0), (-128.0, -40.0, 150.0), 7.0, 7.0)
    g.box(WOOD, (-126.0, -38.0, 160.0), (30.0, 6.0, 36.0), rot=(0, 30, 0))
    g.box(WOOD, (60.0, -80.0, 30.0), (70.0, 40.0, 60.0))
    g.box(CAP, (60.0, -80.0, 64.0), (76.0, 46.0, 8.0))
    for k in range(3):
        g.sphere(YEL, (38.0 + k * 22.0, -80.0, 72.0), 10.0, scale=(1.0, 1.3, 0.7), steps=(3, 5))


def radiator_stand(sc):
    """RADIATOR-WATER STAND, LOW (1.9 m): three turquoise barrels with white hoops on a plank stand, tin cans, a
    watering can, a white board with a yellow arrow (no words)."""
    g = sc.still()
    g.box(WOOD, (0.0, 0.0, 34.0), (330.0, 90.0, 10.0))
    for s in (-1, 0, 1):
        g.box(WOOD, (s * 150.0, 0.0, -13.0), (12.0, 80.0, 94.0))
    for k in range(3):
        x = -105.0 + k * 105.0
        g.cyl(TQ, (x, 0.0, 39.0), 38.0, 86.0, steps=8)
        g.cyl(CAP, (x, 0.0, 60.0), 39.5, 9.0, steps=8)
        g.cyl(CAP, (x, 0.0, 98.0), 39.5, 9.0, steps=8)
        g.cyl(YEL if k == 1 else MAG, (x, 0.0, 125.0), 14.0, 14.0, steps=6)
    g.cyl(GREY, (-200.0, -40.0, 0.0), 13.0, 30.0, steps=6)
    g.cyl(YEL, (-228.0, -30.0, 0.0), 12.0, 26.0, steps=6)
    g.cyl(GREY, (205.0, -50.0, 0.0), 22.0, 40.0, steps=6, top_r=18.0)
    tilt_box(g, GREY, (220.0, -50.0, 24.0), (262.0, -50.0, 52.0), 7.0, 7.0)
    # the arrow board on its post
    g.cyl(WOOD, (230.0, 30.0, -60.0), 7.0, 210.0, steps=5)
    g.box(CAP, (150.0, 28.0, 150.0), (200.0, 10.0, 56.0))
    g.box(YEL, (160.0, 21.0, 150.0), (120.0, 6.0, 18.0))
    tilt_box(g, YEL, (92.0, 21.0, 150.0), (124.0, 21.0, 172.0), 6.0, 16.0)
    tilt_box(g, YEL, (92.0, 21.0, 150.0), (124.0, 21.0, 128.0), 6.0, 16.0)


def garden_wall(sc):
    """GARDEN WALL, LOW (1.8 m): 7 m of thick wavy adobe wall with a white cap, a gap with a small yellow gate,
    painted pots on it, a hitching ring."""
    g = sc.still()
    for x0, x1, w in ((-350.0, -70.0, 1), (70.0, 350.0, 1)):
        side_x(g, TQ, wave_pts(x0, x1, -60.0, 60.0 + 84.0, 30.0, w, 5), -20.0, 20.0)
        side_x(g, CAP, wave_pts(x0, x1, 84.0, 5.0, 30.0, w, 5), -25.0, 25.0)
    for s in (-1, 1):
        g.cyl(TQ, (s * 70.0, 0.0, -60.0), 30.0, 60.0 + 128.0, steps=7, top_r=24.0)
        g.cyl(CAP, (s * 70.0, 0.0, 128.0), 28.0, 14.0, steps=7)
        g.sphere(MAG, (s * 70.0, 0.0, 154.0), 14.0, steps=(3, 5))
    g.box(YEL, (0.0, 0.0, 54.0), (86.0, 7.0, 80.0))
    g.box(CAP, (0.0, -5.0, 54.0), (86.0, 5.0, 10.0))
    pot(g, -210.0, 0.0, 112.0, 20.0, 36.0, col=MAG, plant=True)
    pot(g, 210.0, 0.0, 112.0, 20.0, 36.0, col=YEL)
    pot(g, 330.0, -50.0, 0.0, 26.0, 48.0, col=CAP, rim=MAG, plant=True)
    pot(g, -322.0, -46.0, 0.0, 22.0, 40.0, col=TQD, rim=CAP)


# ------------------------------------------------------------------------------------------ the hero landmark
def big_pump(sc):
    """THE BIG PUMP, 30.4 m: an antique roadside fuel pump as an adobe building. A white stepped plinth, a tall
    turquoise cabinet with battered walls and rounded corners (a yellow arched door at its foot, windows up its
    front, two lit, a blank white reel window, rows of beam ends), a white waist, a head with a white dial 5 m across
    (twelve navy ticks, no numerals, one yellow hand that swings slowly), a wavy white cornice, and a round yellow
    globe that glows steadily. A dark hose loops down its right side to a nozzle on a hook; a yellow crank on the
    left rocks slowly."""
    g = sc.still()
    slab(g, CAP, 0.0, 0.0, -300.0, 60.0, 1320.0, 1060.0, 130.0, 3)
    slab(g, TQD, 0.0, 10.0, 60.0, 112.0, 1120.0, 880.0, 110.0, 3)
    L = block(g, TQ, 0.0, 30.0, 100.0, 1750.0, 880.0, 680.0, 62)
    slab(g, CAP, 0.0, 30.0, 1742.0, 1806.0, 810.0, 610.0, 50.0, 2)
    H = block(g, TQ, 0.0, 30.0, 1800.0, 2440.0, 800.0, 600.0, 36)
    slab(g, CAP, 0.0, 30.0, 2428.0, 2462.0, 790.0, 590.0, 50.0, 2)
    parapet(g, H, 2450.0, h=40.0, amp=30.0, t=40.0)
    # the neck and the globe
    g.cyl(CAP, (0.0, 30.0, 2462.0), 170.0, 60.0, steps=10, top_r=120.0)
    g.cyl(TQ, (0.0, 30.0, 2522.0), 124.0, 34.0, steps=10)
    g.sphere(YEL, (0.0, 30.0, 2776.0), 236.0, steps=(7, 12), glow=0.9)
    g.cyl(CAP, (0.0, 30.0, 2764.0), 240.0, 24.0, steps=12)
    g.cyl(CAP, (0.0, 30.0, 3004.0), 34.0, 34.0, steps=6, top_r=8.0)
    # the dial
    zd = 2112.0
    yd = H.fy(zd)
    rod(g, YEL, (0.0, yd + 12.0, zd), (0.0, yd - 14.0, zd), 276.0, steps=16)
    rod(g, CAP, (0.0, yd + 6.0, zd), (0.0, yd - 24.0, zd), 250.0, steps=16)
    for k in range(12):
        a = 2.0 * math.pi * k / 12.0
        big = (k % 3 == 0)
        rr_ = 204.0 if big else 212.0
        g.box(NAVY, (math.cos(a) * rr_, yd - 26.0, zd + math.sin(a) * rr_), (58.0 if big else 40.0, 8.0, 18.0 if big else 12.0), rot=(math.degrees(a), 0.0, 0.0))
    rod(g, NAVY, (0.0, yd - 22.0, zd), (0.0, yd - 40.0, zd), 30.0, steps=8)
    hand = sc.part(P.motion(swing=55.0, pivot=(0.0, yd - 34.0, zd), axis=90.0, rate=0.035, phase=0.1))
    hand.box(YEL, (0.0, yd - 34.0, zd + 84.0), (30.0, 8.0, 228.0))
    hand.box(YEL, (0.0, yd - 34.0, zd + 196.0), (58.0, 8.0, 58.0), rot=(45, 0, 0))
    # the cabinet's front: the door, an awning, windows, the reel window, beam ends
    yf = L.fy(112.0)
    slab(g, CAP, 0.0, yf - 60.0, 60.0, 130.0, 420.0, 240.0, 60.0, 2)
    door(g, 0.0, yf, 112.0, 200.0, 350.0)
    awning(g, -170.0, 170.0, L.fy(540.0), 548.0, 150.0, 60.0, n=5)
    for z, lits in ((760.0, (WIN, 0.0)), (1060.0, (0.0, WIN)), (1340.0, (0.0, 0.0))):
        for s, lit in zip((-1, 1), lits):
            window(g, s * 230.0, L.fy(z), z, 96.0, 136.0, glow=lit)
    zr = 1570.0
    g.box(CAP, (0.0, L.fy(zr) + 8.0, zr), (470.0, 44.0, 150.0))
    for k in (-1, 0, 1):
        g.box(NAVY, (k * 140.0, L.fy(zr) - 6.0, zr), (104.0, 26.0, 100.0))
        g.box(CAP, (k * 140.0, L.fy(zr) - 18.0, zr), (104.0, 6.0, 10.0))
    vigas(g, L, 910.0, spread(-330.0, 330.0, 6), out=60.0, r=13.0)
    vigas(g, L, 1210.0, spread(-320.0, 320.0, 6), out=60.0, r=13.0)
    vigas(g, L, 1690.0, spread(-320.0, 320.0, 7), out=60.0, r=13.0)
    # a balcony under the top windows
    g.box(CAP, (0.0, L.fy(1250.0) - 34.0, 1256.0), (640.0, 90.0, 16.0))
    g.box(CAP, (0.0, L.fy(1250.0) - 74.0, 1290.0), (640.0, 10.0, 56.0))
    # the ends: windows
    for s, lx in ((1.0, L.rx), (-1.0, L.lx)):
        for z, lit in ((760.0, 0.0), (1200.0, WIN if s < 0 else 0.0)):
            window_x(g, lx(z), 30.0, z, 96.0, 136.0, glow=lit, sgn=s)
    # the hose, looping down the right side to its nozzle
    xr = L.rx(1560.0)
    g.box(CAP, (xr + 20.0, 30.0, 1560.0), (80.0, 100.0, 90.0))
    path = [(xr + 30.0, 30.0, 1540.0), (xr + 150.0, 30.0, 1450.0), (xr + 215.0, 30.0, 1200.0), (xr + 222.0, 30.0, 880.0),
            (xr + 180.0, 30.0, 600.0), (xr + 100.0, 30.0, 500.0), (xr + 40.0, 30.0, 620.0), (L.rx(940.0) + 40.0, 30.0, 940.0)]
    for a, b in zip(path, path[1:]):
        rod(g, DARK, a, b, 22.0, steps=6)
    xn = L.rx(1000.0)
    g.box(CAP, (xn + 16.0, 30.0, 1090.0), (60.0, 70.0, 40.0))
    g.box(GREY, (xn + 44.0, 30.0, 1010.0), (64.0, 54.0, 150.0))
    tilt_box(g, GREY, (xn + 50.0, 30.0, 1080.0), (xn + 140.0, 30.0, 1150.0), 34.0, 34.0)
    # the crank on the left side
    xl = L.lx(1250.0)
    rod(g, CAP, (xl + 10.0, 30.0, 1250.0), (xl - 40.0, 30.0, 1250.0), 60.0, steps=8)
    crank = sc.part(P.motion(swing=9.0, pivot=(xl - 30.0, 30.0, 1250.0), axis=90.0, rate=0.07, phase=0.6))
    tilt_box(crank, YEL, (xl - 34.0, 30.0, 1250.0), (xl - 190.0, 30.0, 1470.0), 40.0, 40.0)
    crank.sphere(MAG, (xl - 196.0, 30.0, 1480.0), 44.0, steps=(4, 6))
    # the door's lanterns, pots, a ladder to the balcony
    for s in (-1, 1):
        lantern(g, s * 170.0, yf - 26.0, 400.0, 40.0)
        g.box(WOOD, (s * 170.0, yf - 8.0, 392.0), (12.0, 40.0, 10.0))
        pot(g, s * 330.0, yf - 70.0, 112.0, 46.0, 86.0, col=MAG, plant=True)
    ladder(g, (-340.0, yf - 130.0, 112.0), (-340.0, L.fy(1250.0) - 84.0, 1262.0), w=60.0, rungs=12)
    ristras(sc, [(-128.0, yf - 20.0, 470.0), (128.0, yf - 20.0, 470.0)], length=120.0, w=30.0, swing=5.0, rate=0.3)


JOBS = [
    ('SM_S1D3_TradingPost', trading_post),
    ('SM_S1D3_Cantina', cantina),
    ('SM_S1D3_MotorCourt', motor_court),
    ('SM_S1D3_FillingStation', filling_station),
    ('SM_S1D3_RepairShop', repair_shop),
    ('SM_S1D3_PotShop', pot_shop),
    ('SM_S1D3_CliffHouse', cliff_house),
    ('SM_S1D3_CliffTower', cliff_tower),
    ('SM_S1D3_WaterJar', water_jar, STILL),
    ('SM_S1D3_DryingRack', drying_rack),
    ('SM_S1D3_BrushShade', brush_shade),
    ('SM_S1D3_AirTower', air_tower),
    ('SM_S1D3_LampPost', lamp_post, STILL),
    ('SM_S1D3_BlanketLine', blanket_line),
    ('SM_S1D3_PotStall', pot_stall),
    ('SM_S1D3_ClayOven', clay_oven, STILL),
    ('SM_S1D3_RadiatorStand', radiator_stand, STILL),
    ('SM_S1D3_GardenWall', garden_wall, STILL),
    ('SM_S1D3_BigPump', big_pump),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
