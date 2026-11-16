"""Chemical Works, Stage 1, builder s1animals: the animals of the eight districts. /Game/World/ChemicalWorks/SM_S1ANIMALS_*.
A NIGHT level: every animal is pale or coloured (white, cream, pink, lilac), never dark; nothing green, nothing orange.
Ground: the gate goat and her kid, cats on a brick wall, laboratory rabbits on a moss lawn, a carthorse at a trough,
rats in a file along a pipe, a hen run round a tank hen-house, greyhounds on a stepped plinth, salamanders on a warm
wall, a pelican on mooring piles. Water (each piece brings its own pool: this level has no Afloat rule): flamingos in
a liquor pool, swans round a basin, koi in an octagon pool, seals at a quay landing, three-eyed frogs on lily pads.
Sky and height (far=True, always moving): one barn owl on a wide slow circle, a wheel of pale gulls, a brick stack
with a stork's nest and a gliding stork, a drum dovecote with its flight, a lamp post with pale moths.
Placed by the block C:/ftwork/s1/L1_s1animals.inl. Run: import ft_world_chem_s1animals as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/ChemicalWorks'

# the kit's colours (ft_dress_chem.py and the zone plan); no green, no orange
WHITE, STEAM, CREAM, LAMP = '#eef6ff', '#d8d0f0', '#ffe9a8', '#fff1c0'
PPINK, PINK, MAG, VIOLET = '#f3c4dc', '#ff7ad9', '#e84aff', '#b44dff'
LILAC, MAUVE, PURPLE, DPURPLE, WALLP = '#b9a8e0', '#c391d8', '#8662b8', '#6a5ab0', '#7560b8'
YEL, RED, RUST, STEEL = '#ffd23f', '#ff3355', '#a8552f', '#8f9bb3'
PAVE, DPAVE, CONC, NAVY, BLACK, GLASS = '#7f8fc6', '#56659f', '#3c4a78', '#1c1f4a', '#1c1a26', '#233a63'
TEAL, DTEAL, CYAN, BLUE, DBLUE = '#19b5c9', '#12849a', '#5ff0ff', '#3a5ab8', '#33489e'


# ------------------------------------------------------------------------------------------ helpers
def ball(m, col, c, r3, steps=(4, 6)):
    """An ellipsoid: centre c, radii r3 = (x, y, z)."""
    m.sphere(col, c, 1.0, scale=r3, steps=steps)


def disc(m, col, r, z0, z1, n=8, at=(0.0, 0.0), turn=0.0, sx=1.0, sy=1.0):
    """A regular n-sided slab from z0 to z1 (its first corner at `turn` degrees), stretched sx, sy."""
    pts = [(at[0] + r * sx * math.cos(math.radians(turn + k * 360.0 / n)), at[1] + r * sy * math.sin(math.radians(turn + k * 360.0 / n))) for k in range(n)]
    m.plan(col, pts, z0, z1)


def ring_blocks(m, col, r, z, n, size, turn=0.0):
    """n blocks on the edges of the n-sided slab of disc(): a raised rim. r: the slab's corner radius."""
    ap = r * math.cos(math.pi / n) - size[1] * 0.5
    for k in range(n):
        a = turn + (k + 0.5) * 360.0 / n
        m.box(col, (ap * math.cos(math.radians(a)), ap * math.sin(math.radians(a)), z + size[2] * 0.5), size, rot=(0, a + 90.0, 0))


class Cr(object):
    """A creature's own frame (as ft_people.quadruped's): it stands on (x, y, z), its head towards -Y at yaw 0 (yaw 90
    heads it along +X). part() hands out the model for the parts that swing one way; pivots and axes are given in the
    creature's frame. travel = (length cm, cm a second) along the mesh's X; orbit = (centre x, centre y, rounds a second).
    Every part of one creature shares phase, rate, bob, travel: the bob and the travel must not pull it apart."""

    def __init__(self, sc, x, y, z=0.0, yaw=0.0, s=1.0, phase=0.0, rate=0.0, bob=0.0, travel=None, orbit=None):
        self.sc, self.at, self.yaw, self.s = sc, (x, y, z), yaw, s
        self.cy, self.sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        tv, base = (0.0, 0.0), (x, y)
        if travel:
            tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
        elif orbit:
            base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
        self.common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv)
        self.t4 = 4.4 / s                        # nothing thinner than 4 cm as built

    def W(self, p):
        x, y, z = self.at
        return (x + self.s * (p[0] * self.cy - p[1] * self.sy), y + self.s * (p[0] * self.sy + p[1] * self.cy), z + self.s * p[2])

    def part(self, swing=0.0, pivot=(0.0, 0.0, 0.0), axis=0.0):
        mo = dict(self.common)
        mo.update(swing=swing, pivot=self.W(pivot), axis=(P.UP if axis == P.UP else self.yaw + axis))
        return self.sc.part(P.motion(**mo), self.at, self.yaw, self.s)


def on_ring(r, a):
    return (r * math.cos(math.radians(a)), r * math.sin(math.radians(a)))


def fowl(sc, x, y, z=0.0, yaw=0.0, s=1.0, body=WHITE, wing=STEAM, beak=YEL, legs=YEL, bl=40.0, bh=26.0, bw=24.0, leg=14.0,
         neck=14.0, neck_up=60.0, head=9.0, beak_len=12.0, tail=12.0, tail_col=None, comb=None, pouch=None, feet=True, eyes=True, wings=True,
         peck=0.0, turn=0.0, stride=0.0, phase=0.0, rate=0.5, bob=0.0, travel=None, orbit=None):
    """A bird on its feet (ft_people.bird is a bird in the air): chunky, big head. It stands on (x, y, z), head to -Y.
    bl, bh, bw: the body's length, height, width; peck: degrees the neck and head nod about the shoulder; turn: degrees
    the head turns (instead); stride: degrees the legs swing. Returns the top of the head (mesh space)."""
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=bob, travel=travel, orbit=orbit)
    t4 = f.t4
    zb = leg + bh * 0.5
    b = f.part()
    ball(b, body, (0, 0, zb), (bw * 0.5, bl * 0.5, bh * 0.5))
    for sd in ((-1, 1) if wings else ()):
        b.box(wing, (sd * bw * 0.47, bl * 0.06, zb + bh * 0.06), (max(bw * 0.14, t4), bl * 0.66, bh * 0.56))
    if tail:
        tilt_box(b, tail_col or wing, (0, bl * 0.34, zb + bh * 0.1), (0, bl * 0.5 + tail, zb + bh * 0.34), max(bw * 0.5, t4), max(bh * 0.18, t4))
    lw = max(bw * 0.14, t4)
    for sd in (-1, 1):
        m = f.part(sd * stride, (sd * bw * 0.2, 0, leg), 0.0) if stride else b
        m.box(legs, (sd * bw * 0.2, 0, leg * 0.5), (lw, lw, leg + 2.0))
        if feet:
            m.box(legs, (sd * bw * 0.2, -lw, t4 * 0.5), (lw * 2.0, lw * 3.0, t4))
    sh = (0, -bl * 0.3, leg + bh * 0.75)
    a = math.radians(neck_up)
    hd = (0, sh[1] - math.cos(a) * neck, sh[2] + math.sin(a) * neck)
    nk = f.part(peck, sh, 0.0) if peck else b
    hm = nk if peck else (f.part(turn, hd, P.UP) if turn else b)
    tilt_box(nk, body, sh, hd, max(head * 0.9, t4), max(head * 0.9, t4))
    ball(hm, body, hd, (head, head * 1.1, head), steps=(3, 5))
    tilt_box(hm, beak, (0, hd[1] - head * 0.7, hd[2]), (0, hd[1] - head * 0.7 - beak_len, hd[2] - beak_len * 0.15), max(head * 0.5, t4), max(head * 0.42, t4))
    if pouch:
        tilt_box(hm, pouch, (0, hd[1] - head * 0.6, hd[2] - head * 0.55), (0, hd[1] - head * 0.7 - beak_len * 0.9, hd[2] - head * 0.5 - beak_len * 0.15),
                 max(head * 0.42, t4), max(head * 0.7, t4))
    if comb:
        hm.box(comb, (0, hd[1] - head * 0.1, hd[2] + head * 1.1), (t4, head * 1.4, head * 0.8))
    if eyes:
        for sd in (-1, 1):
            hm.box(BLACK, (sd * head * 0.86, hd[1] - head * 0.35, hd[2] + head * 0.2), (t4, t4, t4))
    return z + s * (hd[2] + head)


def flyer(sc, x, y, z, heading=0.0, span=130.0, body=WHITE, wing=STEAM, beak=None, phase=0.0, rate=1.8, flap=30.0, orbit=None, bob=0.0, thick=4.5):
    """A small thing on the wing (a dove, a moth) drawn with thick wings: ft_people.bird's wings are thinner than 4 cm
    under a span of 2.5 m. heading: degrees in the ground plane, 0 = +X. orbit = (centre x, centre y, rounds a second)."""
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    rx, ry = -ty, tx
    base, tv = ((orbit[0], orbit[1]), (-1.0, float(orbit[2]))) if orbit else ((x, y), (0.0, 0.0))
    common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv)
    L = span * 0.4
    b = sc.part(P.motion(pivot=(x, y, z), **common))
    tilt_box(b, body, (x - tx * L * 0.5, y - ty * L * 0.5, z), (x + tx * L * 0.5, y + ty * L * 0.5, z), L * 0.36, L * 0.3)
    if beak:
        bk = max(L * 0.14, thick)
        tilt_box(b, beak, (x + tx * L * 0.5, y + ty * L * 0.5, z), (x + tx * L * 0.7, y + ty * L * 0.7, z), bk, bk)
    for side in (-1, 1):
        w = sc.part(P.motion(swing=side * flap, pivot=(x, y, z + L * 0.08), axis=heading, **common))
        a = (x + side * rx * L * 0.1, y + side * ry * L * 0.1, z + L * 0.08)
        tip = (x + side * rx * span * 0.5, y + side * ry * span * 0.5, z + L * 0.14)
        tilt_box(w, wing, a, tip, L * 0.52, thick)


# ------------------------------------------------------------------------------------------ small creatures
def cat(sc, x, y, z, yaw=0.0, s=1.0, col=WHITE, pose='sit', phase=0.0):
    """pose: 'sit' (tail flicking), 'wash' (head and paw nod), 'loaf' (lying, breathing)."""
    loaf = pose == 'loaf'
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=(0.25 if loaf else 1.1 if pose == 'wash' else 0.7), bob=(-1.5 if loaf else 0.0))
    t4 = f.t4
    b = f.part()
    if loaf:
        ball(b, col, (0, 0, 13), (15, 25, 13))
        hd = (0, -24, 22)
        for sd in (-1, 1):
            b.box(col, (sd * 7, -22, 4), (7, 12, 7))
        tl = f.part(18, (10, 22, 6), P.UP)
        tilt_box(tl, col, (10, 22, 6), (30, -6, 6), 6, 6)
    else:
        ball(b, col, (0, 5, 20), (14, 16, 20))
        hd = (0, -7, 46)
        for sd in (-1, 1):
            b.box(col, (sd * 6, -10, 10), (6, 6, 22))
        tl = f.part(24, (8, 18, 5), P.UP)
        tilt_box(tl, col, (8, 18, 5), (44, 22, 5), 6, 6)
    h = f.part(14, (0, 0, 36), 0.0) if pose == 'wash' else f.part()
    ball(h, col, hd, (12, 11, 11), steps=(3, 6))
    for sd in (-1, 1):
        h.cyl(PPINK, (sd * 7, hd[1] + 1, hd[2] + 7), 4.5, 10, steps=4, top_r=3.0)
        h.box(YEL, (sd * 5, hd[1] - 10.5, hd[2] + 2), (t4, t4, t4))
    h.box(PPINK, (0, hd[1] - 11, hd[2] - 3), (t4, t4, t4))
    if pose == 'wash':
        tilt_box(h, col, (7, -10, 28), (5, -15, 44), 6, 6)


def rabbit(sc, x, y, z=0.0, yaw=0.0, s=1.0, pose='sit', phase=0.0, rate=1.0, bob=0.0, travel=None):
    """pose: 'sit', 'up' (on its haunches, ears twitching), 'nibble' (the head nods)."""
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=bob, travel=travel)
    t4 = f.t4
    up = pose == 'up'
    b = f.part()
    if up:
        ball(b, WHITE, (0, 4, 22), (13, 14, 22))
        hd = (0, -4, 50)
        ball(b, STEAM, (0, 17, 8), (7, 7, 7), steps=(3, 4))
    else:
        ball(b, WHITE, (0, 4, 15), (14, 22, 15))
        hd = (0, -20, 25)
        ball(b, STEAM, (0, 25, 15), (7, 7, 7), steps=(3, 4))
    for sd in (-1, 1):
        b.box(WHITE, (sd * 10, 8, 4), (7, 22, 8))
    h = f.part(12, (0, hd[1] + 8, hd[2] - 8), 0.0) if pose == 'nibble' else b
    ball(h, WHITE, hd, (10, 12, 10), steps=(3, 6))
    h.box(PINK, (0, hd[1] - 12, hd[2] - 1), (t4, t4, t4))
    for sd in (-1, 1):
        h.box(PINK, (sd * 8.5, hd[1] - 5, hd[2] + 3), (t4, t4, t4))
    e = f.part(10, (0, hd[1] + 4, hd[2] + 8), 0.0) if up else h
    for sd in (-1, 1):
        tilt_box(e, PPINK, (sd * 5, hd[1] + 4, hd[2] + 7), (sd * 7, hd[1] + 12, hd[2] + 34), 7, 5)


def rat(sc, x, y, z, yaw=0.0, s=1.0, phase=0.0, rate=1.0, bob=0.0, travel=None, rear=0.0):
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=bob, travel=travel)
    t4 = f.t4
    b = f.part(rear, (0, 12, 2), 0.0) if rear else f.part()
    ball(b, STEAM, (0, 0, 9), (8, 15, 9))
    ball(b, STEAM, (0, -16, 11), (6, 8, 6), steps=(3, 5))
    b.box(PINK, (0, -24, 10), (t4, t4, t4))
    for sd in (-1, 1):
        b.box(PINK, (sd * 5, -12, 17), (5, t4, 6))
    tl = f.part(20, (0, 14, 6), P.UP)
    tilt_box(tl, PINK, (0, 14, 6), (0, 40, 5), t4, t4)


def hound(sc, x, y, z, yaw=0.0, s=1.0, col=STEAM, chest=WHITE, collar=VIOLET, lift=0.0, wag=0.0, phase=0.0):
    """A greyhound lying down, its forelegs stretched towards -Y. lift: degrees its head and neck rise and sink."""
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=(0.22 if lift else 1.2))
    t4 = f.t4
    b = f.part()
    ball(b, col, (0, 10, 14), (13, 34, 13))
    ball(b, chest, (0, -16, 15), (12, 16, 15), steps=(3, 6))
    for sd in (-1, 1):
        tilt_box(b, col, (sd * 8, -24, 5), (sd * 9, -58, 4), 6, 6)
        tilt_box(b, col, (sd * 12, 34, 6), (sd * 17, 6, 5), 7, 7)
    tl = f.part(wag, (0, 42, 8), P.UP) if wag else b
    tilt_box(tl, col, (0, 42, 8), (22, 62, 5), t4, t4)
    n = f.part(lift, (0, -26, 20), 0.0) if lift else b
    tilt_box(n, col, (0, -26, 20), (0, -38, 42), 9, 9)
    n.box(collar, (0, -31, 30), (11, 6, 6))
    tilt_box(n, col, (0, -34, 44), (0, -62, 39), 9, 9)
    n.box(BLACK, (0, -63.5, 39), (t4, t4, t4))
    for sd in (-1, 1):
        tilt_box(n, col, (sd * 4.5, -38, 47), (sd * 8, -28, 41), t4, 6)
        n.box(BLACK, (sd * 4.6, -48, 44), (t4, t4, t4))


def salamander(sc, x, z, ang=0.0, s=1.0, col=LAMP, spot=PINK, phase=0.0, rate=0.5, travel=None):
    """On the wall's face (the plane y = 0, seen from -Y), heading `ang` degrees up from +X. It wriggles in the wall's
    plane and its tail swings. travel = (length cm, rounds a second) along X."""
    ca, sa = math.cos(math.radians(ang)), math.sin(math.radians(ang))

    def Wp(u, v, off=8.0):
        return (x + s * (u * ca - v * sa), -off * s, z + s * (u * sa + v * ca))

    common = dict(phase=phase, rate=rate, axis=90.0, base=(x, 0.0), travel=(travel or (0.0, 0.0)))
    b = sc.part(P.motion(swing=5.0, pivot=Wp(0, 0), **common))
    tilt_box(b, col, Wp(-30, 0), Wp(30, 0), 14 * s, 20 * s)
    tilt_box(b, col, Wp(28, 0), Wp(54, 0), 13 * s, 25 * s)
    for u in (-16, 4, 22):
        tilt_box(b, spot, Wp(u - 5, 0), Wp(u + 5, 0), 15.5 * s, 9 * s)
    for su in (-1, 1):
        for sv in (-1, 1):
            tilt_box(b, col, Wp(su * 20, sv * 6), Wp(su * 20 + 7, sv * 27), 8 * s, 7 * s)
    for sv in (-1, 1):
        b.box(BLACK, Wp(47, sv * 9, 14.6), (5.0, 5.0, 5.0))
    t = sc.part(P.motion(swing=16.0, pivot=Wp(-30, 0), **common))
    tilt_box(t, col, Wp(-30, 0), Wp(-56, -5), 11 * s, 13 * s)
    tilt_box(t, col, Wp(-54, -5), Wp(-80, -14), 9 * s, 8 * s)


def flamingo(sc, x, y, z, yaw=0.0, s=1.0, pose='dip', phase=0.0):
    """pose: 'dip' (the neck dips to the pool and rises), 'preen' (a small nod), 'tuck' (one leg, head on its back)."""
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate={'dip': 0.3, 'preen': 0.7, 'tuck': 0.2}[pose], bob=(-1.5 if pose == 'tuck' else 0.0))
    t4 = f.t4
    leg = 70.0
    zb = leg + 13.0
    b = f.part()
    ball(b, PINK, (0, 0, zb), (12, 22, 13))
    for sd in (-1, 1):
        b.box(PPINK, (sd * 11.5, 3, zb + 1), (t4, 30, 16))
    b.box(MAG, (0, 23, zb + 3), (9, 8, 6))
    b.box(PPINK, (-4, 2, leg * 0.5), (t4, t4, leg))
    if pose == 'tuck':
        tilt_box(b, PPINK, (4, 2, leg), (4, 15, leg - 20), t4, t4)
        tilt_box(b, PINK, (0, -16, zb + 6), (0, -6, zb + 20), 7, 7)
        ball(b, PINK, (0, 5, zb + 17), (7, 9, 7), steps=(3, 5))
        return
    b.box(PPINK, (4, 2, leg * 0.5), (t4, t4, leg))
    sh = (0, -18, zb + 6)
    n = f.part(38 if pose == 'dip' else 13, sh, 0.0)
    tilt_box(n, PINK, sh, (0, -48, zb + 16), 7, 7)
    tilt_box(n, PINK, (0, -46, zb + 16), (0, -74, zb - 2), 6.5, 6.5)
    ball(n, PINK, (0, -77, zb - 4), (6.5, 8, 6.5), steps=(3, 5))
    tilt_box(n, WHITE, (0, -81, zb - 6), (0, -89, zb - 17), 5, 5)
    n.box(BLACK, (0, -90.5, zb - 20), (5, 5, 6))


def swan(sc, x, y, z, yaw=0.0, s=1.0, phase=0.0, orbit=None):
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=0.2, orbit=orbit)
    b = f.part()
    ball(b, WHITE, (0, 4, 10), (15, 28, 12))
    for sd in (-1, 1):
        tilt_box(b, WHITE, (sd * 12, -8, 14), (sd * 13, 26, 24), 5, 16)
    b.box(WHITE, (0, 32, 17), (10, 10, 8))
    n = f.part(7, (0, -18, 14), 0.0)
    tilt_box(n, WHITE, (0, -18, 14), (0, -30, 40), 7, 7)
    tilt_box(n, WHITE, (0, -30, 39), (0, -23, 58), 6.5, 6.5)
    ball(n, WHITE, (0, -28, 62), (6, 9, 6), steps=(3, 5))
    tilt_box(n, YEL, (0, -36, 61), (0, -48, 56), 5, 5)
    n.box(BLACK, (0, -35, 65), (5, 5, 4))


def cygnet(sc, x, y, z, yaw=0.0, s=1.0, phase=0.0, orbit=None):
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=0.2, orbit=orbit)
    b = f.part()
    ball(b, STEAM, (0, 2, 6), (8, 13, 7), steps=(3, 6))
    ball(b, STEAM, (0, -11, 14), (5, 6, 5), steps=(3, 5))
    b.box(BLACK, (0, -18, 13), (f.t4, 5, f.t4))


def koi(sc, r, a, z, s=1.0, col=WHITE, patch=RED, turns=0.05, phase=0.0):
    """A koi `r` cm from the pool's middle at `a` degrees, swimming round it (turns: rounds a second, <0 clockwise),
    rising through the surface and sinking again."""
    x, y = on_ring(r, a)
    f = Cr(sc, x, y, z, (a + 180.0 if turns > 0 else a), s, phase=phase, rate=0.5, bob=-6.0, orbit=(0.0, 0.0, turns))
    t4 = f.t4
    b = f.part()
    ball(b, col, (0, 0, 0), (8, 26, 7))
    b.box(patch, (0, -6, 5.5), (9, 14, 4))
    b.box(col, (0, 5, 8), (t4, 14, 6))
    for sd in (-1, 1):
        b.box(col, (sd * 10, -8, -1), (10, 8, t4))
    tl = f.part(22, (0, 24, 0), P.UP)
    tl.box(col, (0, 34, 0), (t4, 18, 14))


def seal(sc, x, y, z, yaw=0.0, s=1.0, phase=0.0, lift=20.0, afloat=False):
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=0.2, bob=(-6.0 if afloat else 0.0))
    t4 = f.t4
    b = f.part()
    ball(b, STEAM, (0, 0, 17), (19, 36, 17))
    if not afloat:
        ball(b, STEAM, (0, 38, 11), (12, 24, 10), steps=(3, 6))
        for sd in (-1, 1):
            tilt_box(b, LILAC, (sd * 6, 58, 8), (sd * 16, 80, 6), 10, 5)
            tilt_box(b, LILAC, (sd * 16, -14, 8), (sd * 34, -4, 3), 12, 5)
            b.box(LILAC, (sd * 9, 14, 31), (6, 7, 5))
    n = f.part(lift, (0, -22, 16), 0.0)
    tilt_box(n, STEAM, (0, -20, 18), (0, -40, 40), 22, 22)
    ball(n, STEAM, (0, -46, 48), (12, 14, 11), steps=(3, 6))
    n.box(WHITE, (0, -59, 45), (12, 6, 7))
    n.box(BLACK, (0, -62.5, 47), (t4, t4, t4))
    for sd in (-1, 1):
        n.box(BLACK, (sd * 7, -56, 52), (t4, t4, t4))


def frog(sc, x, y, z, yaw=0.0, s=1.0, phase=0.0, rate=0.15, bob=0.0, travel=None, turn=0.0):
    """A three-eyed frog. turn: degrees it turns on its pad."""
    f = Cr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=bob, travel=travel)
    t4 = f.t4
    b = f.part(turn, (0, 0, 0), P.UP) if turn else f.part()
    ball(b, MAUVE, (0, 2, 11), (15, 17, 11))
    b.box(PPINK, (0, -12, 8), (20, 12, 10))
    for sd in (-1, 1):
        tilt_box(b, MAUVE, (sd * 13, 12, 5), (sd * 21, -4, 5), 8, 8)
        b.box(MAUVE, (sd * 9, -15, 3), (6, 8, 6))
    for ex in (-9, 0, 9):
        ez = 21 + (3 if ex == 0 else 0)
        ball(b, WHITE, (ex, -9, ez), (4.6, 4.6, 4.6), steps=(3, 4))
        b.box(BLACK, (ex, -13.0, ez), (t4, t4, t4))


# ------------------------------------------------------------------------------------------ the ground pieces
def gate_goat(sc):
    """s1d1. A nanny goat tethered on the verge behind the company town's garden wall, grazing; her kid hops."""
    g = sc.still()
    g.box(RUST, (-30, 175, 10), (460, 34, 220))                       # a stub of brick wall, 1 m of footing
    g.box(CREAM, (-30, 175, 126), (480, 46, 12))
    g.box(CREAM, (-30, 157, 62), (460, 4, 12))
    for k in range(4):
        g.box(CREAM, (-200 + k * 115, 157, 24 + (k % 2) * 66), (44, 4, 14))
    g.cyl(CREAM, (-205, 40, -100), 9, 215, steps=6)                    # the tether post and its rope
    g.cyl(YEL, (-205, 40, 115), 12, 10, steps=6)
    tilt_box(g, WHITE, (-205, 40, 96), (-52, -22, 98), 5, 5)
    g.cyl(STEEL, (190, 110, 0), 22, 60, steps=8)                       # a milk churn, a stool, a pail
    g.cyl(STEEL, (190, 110, 60), 22, 16, steps=8, top_r=12)
    g.cyl(WHITE, (190, 110, 76), 14, 6, steps=8)
    g.box(CREAM, (120, 96, 32), (44, 32, 6))
    for (lx, ly) in ((104, 86), (136, 86), (120, 108)):
        g.box(RUST, (lx, ly, 14), (6, 6, 30))
    g.cyl(WHITE, (62, 60, 0), 13, 24, steps=8, top_r=16)
    r = random.Random(11)
    for k in range(9):                                                 # weeds through the cobbles
        tx, ty = r.uniform(-230, 230), r.uniform(-130, 120)
        g.box(STEAM, (tx, ty, 9), (6, 6, 18))
        g.box(CREAM, (tx, ty, 21), (11, 11, 7))
    goat = dict(length=80, height=62, girth=28, neck=24, head=22, body=WHITE, belly=STEAM, ears=PPINK, horns=CREAM, tail=12, snout=PPINK, legs_col=STEAM)
    P.quadruped(sc, 20, 0, yaw=-70, s=1.7, neck_up=-25, graze=16, wag=24, rate=0.45, phase=0.1, **goat)
    kid = dict(goat, length=46, height=38, girth=18, neck=13, head=21, horns=None, tail=8)
    P.quadruped(sc, 150, -75, yaw=120, s=1.6, neck_up=40, bob=12, stride=14, rate=1.1, phase=0.4, **kid)


def wall_cats(sc):
    """s1d1, s1d3, s1d5. Three cats on a brick wall: one washes, one flicks its tail, one sleeps."""
    g = sc.still()
    g.box(RUST, (0, 0, 0), (600, 40, 300))
    g.box(CREAM, (0, 0, 156), (620, 52, 12))
    g.box(CREAM, (0, -21, 76), (600, 4, 12))
    for k in range(5):
        g.box(CREAM, (-230 + k * 115, -21, 28 + (k % 2) * 86), (44, 4, 14))
    for sx in (-1, 1):
        g.box(RUST, (sx * 296, 0, 12), (50, 62, 324))
        g.box(CREAM, (sx * 296, 0, 180), (62, 74, 12))
    g.cyl(STEEL, (236, -28, -20), 6, 176, steps=6)                     # a drainpipe, a crate, a saucer of milk
    g.box(BLUE, (-150, -64, 18), (60, 44, 36))
    g.box(WHITE, (-150, -64, 38), (64, 48, 5))
    g.cyl(WHITE, (-80, -66, 0), 16, 5, steps=8)
    g.cyl(STEEL, (40, -70, 0), 20, 50, steps=8)                        # a dustbin with its lid askew
    g.cyl(STEAM, (44, -72, 50), 23, 6, steps=8, rot=(8, 0, 0))
    cat(sc, -170, 0, 162, yaw=0, s=1.5, col=WHITE, pose='wash', phase=0.2)
    cat(sc, 30, 0, 162, yaw=15, s=1.5, col=CREAM, pose='sit', phase=0.6)
    cat(sc, 185, 0, 162, yaw=90, s=1.5, col=LILAC, pose='loaf', phase=0.9)


def lab_rabbits(sc):
    """s1d2. White laboratory rabbits loose on a moss lawn by their hutch: two hop from one pipe mouth to the other."""
    g = sc.still()
    g.box(DTEAL, (0, 0, -47), (600, 380, 100))                         # the lawn (moss: teal), a white kerb round it
    for sy in (-1, 1):
        g.box(WHITE, (0, sy * 194, -44), (616, 10, 106))
    for sx in (-1, 1):
        g.box(WHITE, (sx * 303, 0, -44), (10, 380, 106))
    for (py, col) in ((-70, WHITE), (80, STEAM)):                      # the pipe mouths the hoppers come from and go to
        for sx in (-1, 1):
            rod(g, col, (sx * 222, py, 30), (sx * 296, py, 30), 30, steps=8)
            g.box(BLACK, (sx * 220, py, 28), (4, 34, 40))
    hx, hy = 150, 140                                                  # the hutch
    for (lx, ly) in ((-48, -28), (48, -28), (-48, 28), (48, 28)):
        g.box(WALLP, (hx + lx, hy + ly, 20), (8, 8, 40))
    g.box(WHITE, (hx, hy, 70), (112, 72, 60))
    g.box(WALLP, (hx, hy, 104), (132, 92, 8))
    g.box(DPURPLE, (hx, hy, 112), (60, 92, 8))
    g.box(BLACK, (hx - 24, hy - 37, 66), (30, 4, 36))
    g.box(CYAN, (hx + 26, hy - 37, 70), (36, 4, 5))
    tilt_box(g, STEAM, (hx - 24, hy - 38, 42), (hx - 24, hy - 96, 4), 30, 5)
    g.cyl(WHITE, (-40, 60, 3), 18, 8, steps=8)                         # a bowl of violet pellets
    g.cyl(VIOLET, (-40, 60, 11), 13, 4, steps=8)
    rabbit(sc, -120, -70, 3, yaw=90, s=1.7, phase=0.0, rate=1.4, bob=12, travel=(440, 55))
    rabbit(sc, 60, 80, 3, yaw=-90, s=1.6, phase=0.5, rate=1.2, bob=12, travel=(440, -46))
    rabbit(sc, 30, 150, 3, yaw=20, s=1.7, pose='up', phase=0.3, rate=1.5)
    rabbit(sc, -46, 96, 3, yaw=-170, s=1.7, pose='nibble', phase=0.7, rate=1.7)
    rabbit(sc, 30, -150, 3, yaw=-30, s=1.7, pose='sit')
    rabbit(sc, 86, -138, 3, yaw=40, s=1.0, pose='sit', phase=0.2, rate=1.7, bob=6)


def carthorse(sc):
    """s1d3. A pale heavy horse drinking at the yard trough, its dray of sacks behind it; the tail swishes."""
    g = sc.still()
    g.box(STEEL, (178, 0, 62), (72, 250, 60))                          # the trough on its legs
    g.box(GLASS, (178, 0, 93), (60, 238, 4))
    for sy in (-1, 1):
        g.box(YEL, (178, sy * 100, -34), (60, 14, 136))
    for sx in (-1, 1):                                                 # the hitching rail behind
        g.cyl(YEL, (sx * 150, 150, -100), 8, 240, steps=6)
    rod(g, STEEL, (-170, 150, 128), (170, 150, 128), 6, steps=6)
    g.box(BLUE, (60, 144, 96), (30, 12, 44))                           # a nosebag on the rail
    g.box(WHITE, (60, 144, 120), (34, 14, 6))
    g.cyl(RED, (250, 120, 0), 16, 30, steps=8, top_r=20)               # a pail
    for k, (bx, by) in enumerate(((255, -120), (300, -116), (276, -118))):   # straw bales
        g.box(CREAM, (bx - 20 * (k == 2), by, 20 + 40 * (k == 2)), (58, 40, 40))
    g.box(YEL, (-390, 20, 78), (210, 120, 12))                         # the dray: a flat bed, two wheels, shafts, sacks
    g.box(STEEL, (-390, 20, 66), (190, 14, 12))
    for sy in (-1, 1):
        rod(g, RED, (-400, 20 + sy * 64, 46), (-400, 20 + sy * 76, 46), 46, steps=10)
        rod(g, WHITE, (-400, 20 + sy * 75, 46), (-400, 20 + sy * 80, 46), 12, steps=6)
        tilt_box(g, STEEL, (-290, 20 + sy * 46, 78), (-178, sy * 50, 150), 7, 7)
    g.box(STEEL, (-300, 20, 36), (8, 8, 72))
    for k, (sx, sy2) in enumerate(((-450, 0), (-400, 44), (-390, -8), (-340, 30))):
        ball(g, WHITE if k % 2 else STEAM, (sx, sy2, 104 + 8 * (k == 2)), (32, 24, 20))
    g.box(BLUE, (-455, 52, 104), (46, 40, 40))
    horse = dict(length=170, height=135, girth=62, neck=55, head=44, body=LAMP, belly=CREAM, mane=WHITE, tail=60, tail_col=WHITE, legs_col=WHITE,
                 snout=PPINK, ears=LAMP)
    P.quadruped(sc, -60, 0, yaw=90, s=1.5, neck_up=-35, graze=9, wag=26, rate=0.4, phase=0.2, **horse)
    f = Cr(sc, -60, 0, 0, 90, 1.5)                                     # feathered feet and a collar
    h = f.part()
    for (lx, ly) in ((23, -69), (-23, -69), (23, 69), (-23, 69)):
        h.box(WHITE, (lx, ly - 2, 11), (22, 26, 22))
    h.box(BLUE, (0, -66, 112), (72, 16, 74))
    h.box(YEL, (0, -66, 152), (20, 18, 10))


def pipe_rats(sc):
    """s1d3, s1d4. Big pale rats in a file along a length of pipe on saddles, from one valve box to the other."""
    g = sc.still()
    rod(g, STEEL, (-440, 0, 90), (440, 0, 90), 22, steps=8)
    for fx in (-250, 0, 250):
        rod(g, MAG, (fx - 6, 0, 90), (fx + 6, 0, 90), 29, steps=8)
    for fx in (-125, 125):
        rod(g, WHITE, (fx - 9, 0, 90), (fx + 9, 0, 90), 23.5, steps=8)
        g.box(CONC, (fx, 0, -16), (44, 64, 172))
    for sx in (-1, 1):                                                 # the valve boxes: the file comes out of one and into the other
        g.box(CONC, (sx * 428, 0, 0), (76, 76, 300))
        g.box(DPAVE, (sx * 428, 0, 154), (86, 86, 8))
        g.box(BLACK, (sx * 389, 0, 118), (4, 30, 26))
        g.cyl(STEEL, (sx * 428, -42, 60), 4, 44, steps=6)
        rod(g, YEL, (sx * 428, -44, 104), (sx * 428, -50, 104), 20, steps=8)
    g.plan(MAG, [(-40, -74), (22, -92), (64, -70), (50, -44), (-10, -40)], 0, 4)      # a puddle under a weeping flange
    g.box(MAG, (0, -6, 44), (5, 5, 50))
    for k in range(5):
        rat(sc, -330 + k * 165, 0, 112, yaw=90, s=2.1, phase=(0.13 * k) % 1.0, rate=1.9, bob=2.5, travel=(778, 85))
    rat(sc, 428, 6, 158, yaw=-20, s=2.1, phase=0.4, rate=0.5, rear=9)


def hen_run(sc):
    """s1d7. A round run of posts and rails round a hen-house made of a cut-down tank: white hens peck, a cockerel
    crows on a drum, a hen leads three chicks round the house."""
    g = sc.still()
    R = 280.0
    for k in range(8):
        px, py = on_ring(R, k * 45.0)
        g.box(TEAL if k % 2 else YEL, (px, py, 5), (10, 10, 210))
        a = (k + 0.5) * 45.0
        mx, my = on_ring(R * math.cos(math.pi / 8), a)
        L = 2.0 * R * math.sin(math.pi / 8)
        for hz in (50, 96):
            g.box(WHITE, (mx, my, hz), (L, 5, 5), rot=(0, a + 90.0, 0))
    g.cyl(DBLUE, (0, 0, -100), 20, 130, steps=6)                       # the hen-house: a tank on a stub, a lid, a round door
    g.cyl(TEAL, (0, 0, 30), 72, 96, steps=8)
    g.cyl(YEL, (0, 0, 66), 74, 14, steps=8)
    g.cyl(WHITE, (0, 0, 126), 80, 6, steps=8)
    g.cyl(DTEAL, (0, 0, 132), 78, 46, steps=8, top_r=10)
    rod(g, WHITE, (0, -70, 78), (0, -76, 78), 24, steps=8)
    rod(g, BLACK, (0, -74, 78), (0, -78, 78), 18, steps=8)
    g.box(WHITE, (0, -92, 44), (44, 40, 6))
    g.box(DBLUE, (0, -104, 20), (8, 8, 44))
    dx, dy = on_ring(205, 200)                                         # the cockerel's drum
    g.cyl(DBLUE, (dx, dy, 0), 30, 84, steps=8)
    g.cyl(WHITE, (dx, dy, 84), 31, 5, steps=8)
    fowl(sc, dx, dy, 89, yaw=40, s=1.8, body=WHITE, wing=STEAM, tail_col=MAUVE, tail=18, comb=RED, leg=12, neck=16, neck_up=70, peck=14, rate=0.35, phase=0.1)
    tx, ty = on_ring(200, 300)                                         # the feed trough
    g.box(WHITE, (tx, ty, 12), (110, 30, 16), rot=(0, 30, 0))
    g.box(CREAM, (tx, ty, 21), (98, 20, 4), rot=(0, 30, 0))
    hens = ((225, 318, 150, WHITE, 1.3, 0.0), (190, 284, -20, LAMP, 1.6, 0.4), (215, 80, 200, WHITE, 1.1, 0.7), (228, 132, 20, STEAM, 1.5, 0.2))
    for (hr, ha, hyaw, col, rate, ph) in hens:
        hx2, hy2 = on_ring(hr, ha)
        fowl(sc, hx2, hy2, 0, yaw=hyaw, s=1.7, body=col, wing=STEAM, comb=RED, leg=10, neck=13, neck_up=20, peck=30, rate=rate, phase=ph, feet=False, eyes=False, wings=False)
    mx, my = on_ring(140, 0)                                           # the mother and her chicks, round and round the house
    fowl(sc, mx, my, 0, yaw=180, s=1.7, body=LAMP, wing=CREAM, comb=RED, leg=10, neck=13, neck_up=55, stride=18, rate=1.6, phase=0.0, bob=2,
         orbit=(0, 0, 0.045), feet=False)
    for k, a in enumerate((-17, -29, -40)):
        cx, cy = on_ring(138 + 6 * (k % 2), a)
        f = Cr(sc, cx, cy, 0, 180 + a, 1.6, phase=0.3 * k, rate=1.8, bob=3, orbit=(0, 0, 0.045))
        c = f.part()
        ball(c, YEL, (0, 0, 9), (6, 8, 6), steps=(3, 4))
        ball(c, YEL, (0, -7, 14), (4, 4, 4), steps=(3, 4))
        c.box(RED, (0, -11.5, 14), (f.t4, f.t4, f.t4))


def greyhounds(sc):
    """s1d8. Two pale greyhounds lying on a stepped plinth: one lifts its head and looks, the other's tail thumps."""
    g = sc.still()
    g.box(NAVY, (0, 0, -80), (480, 320, 240))                          # three steps (the copper quarter's ziggurat edge)
    g.box(NAVY, (0, 24, 60), (400, 236, 40))
    g.box(DPAVE, (0, 40, 95), (330, 176, 30))
    g.box(RUST, (0, -161, 18), (480, 4, 12))
    g.box(RUST, (0, -95, 66), (400, 4, 10))
    g.box(VIOLET, (0, -49, 103), (330, 4, 5))
    for sx in (-1, 1):
        g.box(RUST, (sx * 241, 0, 18), (4, 320, 12))
        g.box(NAVY, (sx * 212, -124, 62), (40, 40, 44))                # square bases at the front corners
        g.box(STEAM, (sx * 212, -124, 88), (48, 48, 8))
    g.cyl(STEEL, (150, -70, 80), 20, 12, steps=8, top_r=24)            # a water bowl and a folded blanket on the middle step
    g.box(CREAM, (-150, -72, 86), (70, 46, 12))
    g.box(RUST, (-150, -72, 93), (70, 10, 4))
    hound(sc, -70, 30, 110, yaw=-25, s=1.8, col=STEAM, chest=WHITE, collar=VIOLET, lift=17, phase=0.2)
    hound(sc, 82, 44, 110, yaw=28, s=1.8, col=PPINK, chest=WHITE, collar=CYAN, wag=12, phase=0.6)


def salamanders(sc):
    """s1d8, s1d6. Large pale salamanders on a warm wall with lit vents: three wriggle, one creeps from fin to fin."""
    g = sc.still()
    g.box(PAVE, (0, 30, 50), (600, 60, 500))                           # the wall, 2 m of footing; its face is y = 0
    g.box(DPAVE, (0, 30, 306), (720, 80, 12))
    for bz in (22, 276):
        g.box(RUST, (0, -2, bz), (600, 4, 14))
    for sx in (-1, 1):                                                 # two buttress fins
        g.box(DPAVE, (sx * 310, -15, 25), (80, 150, 450))
        g.box(DPAVE, (sx * 310, -10, 268), (80, 110, 36))
    for k in range(3):                                                 # the vents: a steady warm light, never nearer than 24 m
        g.box(NAVY, (-180 + k * 180, -2, 222), (76, 4, 30))
        g.box(LAMP, (-180 + k * 180, -4, 222), (62, 4, 16), glow=0.5)
    rod(g, STEEL, (-280, -34, 30), (280, -34, 30), 12, steps=6)        # a steam pipe along the foot, two puffs
    g.box(STEEL, (100, 30, 332), (22, 22, 44))                         # a vent on the coping
    for (px, py, pz, ph) in ((-252, -40, 96, 0.2), (104, 26, 392, 0.7)):
        s2 = sc.part(P.motion(bob=-12.0, rate=0.12, phase=ph))
        s2.sphere(STEAM, (px, py, pz), 1.0, scale=(24, 20, 20), steps=(3, 6))
        s2.sphere(WHITE, (px + 14, py - 2, pz + 30), 1.0, scale=(15, 13, 13), steps=(3, 5))
    salamander(sc, -140, 90, ang=25, s=1.7, col=LAMP, spot=PINK, phase=0.1, rate=0.5)
    salamander(sc, 20, 150, ang=160, s=1.9, col=PPINK, spot=VIOLET, phase=0.5, rate=0.4)
    salamander(sc, 228, 150, ang=80, s=1.5, col=WHITE, spot=PINK, phase=0.8, rate=0.6)
    salamander(sc, -100, 236, ang=0, s=1.5, col=LAMP, spot=MAG, phase=0.3, rate=0.6, travel=(470.0, 0.03))


def pelican_piles(sc):
    """s1d5. Three mooring piles on a quay stub: a white pelican preens on the tallest, a pale gull looks about."""
    g = sc.still()
    g.box(WHITE, (10, 10, -80), (300, 150, 240))
    g.box(DTEAL, (10, -67, 30), (300, 6, 20))
    g.box(RED, (10, -66, 8), (300, 4, 10))
    piles = ((-80, 0, 190), (20, 30, 260), (110, -15, 140))
    for k, (px, py, top) in enumerate(piles):
        g.cyl(DTEAL, (px, py, 40), 20, top - 40, steps=8)
        g.cyl(WHITE, (px, py, top), 22, 10, steps=8)
        g.cyl(CREAM, (px, py, 96), 22.5, 14, steps=8)
    g.cyl(RED, (20, 30, 226), 21.5, 12, steps=8)
    tilt_box(g, CREAM, (-80, 0, 104), (20, 30, 104), 6, 6)             # a hawser from pile to pile
    tilt_box(g, CREAM, (20, 30, 104), (110, -15, 104), 6, 6)
    g.cyl(CREAM, (-80, 0, 200), 17, 9, steps=8)                        # a coil of rope on the third
    g.cyl(CREAM, (-80, 0, 209), 12, 7, steps=8)
    fowl(sc, 20, 30, 270, yaw=-20, s=2.2, body=WHITE, wing=STEAM, beak=LAMP, pouch=CREAM, legs=YEL, bl=44, bh=30, bw=28, leg=8, neck=22, neck_up=75,
         head=10, beak_len=34, tail=8, peck=13, rate=0.22, phase=0.3)
    fowl(sc, 110, -15, 150, yaw=60, s=1.7, body=WHITE, wing=LILAC, beak=YEL, legs=YEL, bl=34, bh=20, bw=18, leg=9, neck=9, neck_up=65, head=7.5,
         beak_len=9, tail=12, turn=35, rate=0.2, phase=0.6)


# ------------------------------------------------------------------------------------------ the water pieces
def flamingos(sc):
    """s1d4. Six pink flamingos standing in a shallow pool of magenta liquor: three dip, one preens, two sleep."""
    g = sc.still()
    pts = [(-455, -90), (-300, -235), (-30, -270), (265, -225), (450, -70), (425, 150), (215, 255), (-70, 270), (-320, 235), (-465, 95)]
    g.plan(PPINK, [(x * 1.06, y * 1.07) for x, y in pts], -150, 8)     # a tin-pale bank, 1.5 m of footing
    g.plan(MAG, [(x * 0.95, y * 0.94) for x, y in pts], 4, 12)         # the liquor: unlit
    for (lx, ly, lw, ld) in ((-200, -80, 150, 40), (120, 60, 180, 36), (-40, 150, 120, 30), (250, -110, 110, 30)):
        g.box(PINK, (lx, ly, 12), (lw, ld, 4))                         # pale streaks on it
    g.cyl(BLACK, (-380, 205, -100), 7, 330, steps=6)                   # a skimmers' marker and a dripping stove pipe
    g.box(PPINK, (-380, 200, 196), (70, 5, 40))
    g.box(STEEL, (-380, 198, 150), (50, 5, 26))
    g.cyl(BLACK, (392, 215, -100), 14, 250, steps=6)
    tilt_box(g, BLACK, (392, 215, 150), (330, 140, 136), 24, 24)
    g.box(PINK, (326, 136, 70), (7, 7, 124))
    birds = ((-250, -40, 20, 'dip', 0.0), (-120, 70, -60, 'dip', 0.35), (40, -90, 140, 'dip', 0.7), (180, 50, -150, 'preen', 0.2),
             (290, -50, 60, 'tuck', 0.5), (-20, 120, 200, 'tuck', 0.9))
    for (bx, by, byaw, pose, ph) in birds:
        flamingo(sc, bx, by, 4, yaw=byaw, s=1.5, pose=pose, phase=ph)


def swans(sc):
    """s1d6. Two white swans and three cygnets gliding round the plinth of a rimmed, steaming basin; an egret on the rim."""
    g = sc.still()
    R = 370.0
    disc(g, PAVE, R, -150, 45, n=12)
    ring_blocks(g, STEAM, R, 45, 12, (2.0 * R * math.sin(math.pi / 12) + 4, 44, 14))
    disc(g, GLASS, R * math.cos(math.pi / 12) - 27, 45, 49, n=12)
    for (a, rr, L) in ((20, 250, 90), (100, 150, 70), (200, 270, 100), (290, 180, 80), (330, 280, 60)):
        lx, ly = on_ring(rr, a)
        g.box(LILAC, (lx, ly, 49), (L, 8, 4), rot=(0, a + 90, 0))     # ripples
    disc(g, STEAM, 76, 45, 110, n=8)                                   # the stepped plinth and its shallow bowl
    disc(g, LILAC, 50, 110, 150, n=8, turn=22.5)
    g.cyl(STEAM, (0, 0, 150), 26, 26, steps=8, top_r=62)
    g.cyl(WHITE, (0, 0, 176), 56, 5, steps=8)
    for (px, py, pz, ph) in ((10, 0, 226, 0.1), (-60, 40, 150, 0.6)):
        s2 = sc.part(P.motion(bob=-12.0, rate=0.1, phase=ph))
        s2.sphere(STEAM, (px, py, pz), 1.0, scale=(30, 26, 22), steps=(3, 6))
        s2.sphere(WHITE, (px + 20, py + 6, pz + 30), 1.0, scale=(18, 16, 14), steps=(3, 5))
    turns = 0.018
    ax, ay = on_ring(215, 0)
    swan(sc, ax, ay, 47, yaw=180, s=2.3, phase=0.1, orbit=(0, 0, turns))
    for k, a in enumerate((-22, -36, -49)):
        cx, cy = on_ring(205 + 22 * (k % 2), a)
        cygnet(sc, cx, cy, 47, yaw=180 + a, s=2.0, phase=0.2 * k, orbit=(0, 0, turns))
    bx, by = on_ring(235, 172)
    swan(sc, bx, by, 47, yaw=180 + 172, s=2.3, phase=0.6, orbit=(0, 0, turns))
    ex, ey = on_ring(R * math.cos(math.pi / 12) - 22, 255)
    fowl(sc, ex, ey, 59, yaw=20, s=1.7, body=WHITE, wing=WHITE, beak=YEL, legs=YEL, bl=34, bh=20, bw=16, leg=42, neck=30, neck_up=68, head=7,
         beak_len=16, tail=10, peck=18, rate=0.25, phase=0.4)


def koi_pool(sc):
    """s1d6, s1d2. Pale koi circling in an octagon pool, rising through the surface and sinking; lily pads in the middle."""
    g = sc.still()
    R = 256.0
    disc(g, WHITE, R, -150, 40, n=8, turn=22.5)
    ring_blocks(g, LILAC, R, 40, 8, (2.0 * R * math.sin(math.pi / 8) + 4, 36, 12), turn=22.5)
    disc(g, GLASS, R - 20, 40, 44, n=8, turn=22.5)
    for (a, rr, pr) in ((40, 26, 30), (170, 34, 24), (280, 30, 27)):
        lx, ly = on_ring(rr, a)
        disc(g, STEAM, pr, 44, 48, n=6, at=(lx, ly), turn=a)
        disc(g, LILAC, pr * 0.45, 48, 52, n=6, at=(lx, ly), turn=a + 30)
    g.cyl(VIOLET, (-4, 4, 48), 9, 20, steps=6, top_r=3)                # one bud
    g.cyl(WHITE, (-4, 4, 48), 4.5, 8, steps=6)
    for k in range(8):                                                 # a cyan tile at each corner of the rim
        cx, cy = on_ring(R - 20, 22.5 + k * 45.0)
        g.box(CYAN, (cx, cy, 53), (16, 16, 5), rot=(0, 22.5 + k * 45.0, 0))
    fish = ((96, 10, WHITE, RED, 0.05, 0.0), (104, 190, LAMP, WHITE, 0.05, 0.4), (150, 95, PPINK, WHITE, -0.04, 0.7), (156, 250, WHITE, PINK, -0.04, 0.2),
            (178, 150, WHITE, RED, 0.035, 0.55), (176, 330, LAMP, RED, 0.035, 0.85))
    for (r, a, col, patch, turns, ph) in fish:
        koi(sc, r, a, 32, s=1.9, col=col, patch=patch, turns=turns, phase=ph)


def seal_slip(sc):
    """s1d5. A quay landing with broad steps down into a strip of dock water: one seal on the landing lifts its head,
    a second floats in the dock. The steps go down AWAY from the road: no ramp faces it."""
    g = sc.still()
    g.box(GLASS, (0, 70, -70), (700, 440, 160))                        # the dock water, top at z = 10
    g.box(DTEAL, (0, -150, -57), (716, 24, 206))                       # the quay edge on the road's side, a red band
    g.box(RED, (0, -163, 22), (716, 4, 10))
    g.box(WHITE, (-130, -70, -60), (430, 140, 200))                    # the landing and two steps down to the water
    g.box(WHITE, (-130, 30, -70), (430, 70, 180))
    g.box(STEAM, (-130, 90, -80), (430, 60, 184))
    for sx in (-1, 1):
        g.box(DTEAL, (sx * 354, 70, -57), (8, 440, 206))
    g.box(DTEAL, (0, 294, -57), (716, 8, 206))
    for (px, py, top) in ((250, 230, 150), (310, 180, 110)):           # two piles in the water
        g.cyl(STEEL, (px, py, -150), 14, top + 150, steps=8)
        g.cyl(WHITE, (px, py, top), 16, 9, steps=8)
        g.cyl(RED, (px, py, top - 30), 15, 10, steps=8)
    for (lx, ly, L) in ((180, 60, 110), (60, 210, 90), (240, -60, 80), (-200, 220, 100)):
        g.box(LILAC, (lx, ly, 10), (L, 8, 4))                          # ripples
    g.box(BLUE, (50, -24, 56), (64, 46, 32))                           # a fish crate and a coil of rope on the landing
    g.box(WHITE, (50, -24, 74), (68, 50, 5))
    for k in range(3):
        g.box(STEAM, (34 + k * 16, -24, 80), (10, 36, 7))
    g.cyl(CREAM, (40, -108, 40), 22, 9, steps=8)
    g.cyl(CREAM, (40, -108, 49), 15, 8, steps=8)
    seal(sc, -150, -64, 40, yaw=90, s=2.4, phase=0.1, lift=20)
    seal(sc, 170, 130, -22, yaw=-140, s=2.4, phase=0.6, lift=10, afloat=True)


def lagoon_frogs(sc):
    """s1d4. Big three-eyed frogs on pale lily pads in a purple lagoon: one hops from pad to pad to pad and dives."""
    g = sc.still()
    pts = [(-400, -60), (-270, -210), (0, -250), (250, -200), (400, -40), (370, 160), (160, 240), (-120, 230), (-350, 150)]
    g.plan(PPINK, [(x * 1.07, y * 1.08) for x, y in pts], -150, 4)
    g.plan(PURPLE, [(x * 0.96, y * 0.95) for x, y in pts], 2, 8)
    pads = ((-150, -70, 58, 0), (0, -70, 62, 40), (150, -70, 58, 90), (-200, 110, 66, 20), (120, 120, 70, 70), (285, 30, 50, 10))
    for (px, py, pr, a) in pads:
        disc(g, STEAM, pr, 8, 13, n=7, at=(px, py), turn=a)
        g.box(LILAC, (px, py, 13.5), (pr * 1.5, 5, 4), rot=(0, a, 0))
    tilt_box(g, WHITE, (-330, 150, -60), (-300, 120, 170), 30, 30)     # a dead white stump with bracket fungus
    tilt_box(g, WHITE, (-304, 124, 120), (-250, 96, 190), 14, 14)
    g.box(PINK, (-322, 128, 60), (40, 26, 8))
    g.box(MAUVE, (-312, 118, 100), (34, 22, 8))
    L, rho = 300.0, 0.3                                                # the hopper: one bob cycle a crossing = two hops, landing on the pads
    frog(sc, -150, -70, 13, yaw=90, s=2.1, phase=(-150.0 / L + 0.5) % 1.0, rate=rho, bob=34, travel=(L, L * rho))
    frog(sc, -200, 110, 13, yaw=-30, s=2.3, phase=0.2, rate=0.15, turn=14)
    frog(sc, 120, 120, 13, yaw=40, s=2.1, phase=0.6, rate=0.12, turn=18)
    frog(sc, 285, 30, 13, yaw=110, s=1.6, phase=0.9, rate=0.2, turn=10)
    frog(sc, 318, -120, 4, yaw=-60, s=2.0, phase=0.4, rate=0.1, turn=8)


# ------------------------------------------------------------------------------------------ sky and height
def white_owl(sc):
    """s1d1, s1d2. One barn owl gliding a wide slow circle (18 m radius) round the piece's middle."""
    P.bird(sc, 1800, 0, 0, heading=90, span=360, body=WHITE, wing=LAMP, beak=YEL, rate=0.5, flap=9, phase=0.2, orbit=(0, 0, 0.03))


def gull_wheel(sc):
    """s1d3, s1d5. Five pale gulls wheeling, 9 to 13 m from the piece's middle, at five heights."""
    r = random.Random(7)
    for k in range(5):
        a = k * 72.0 + r.uniform(-14, 14)
        rr = 900 + k * 95
        x, y = on_ring(rr, a)
        flyer(sc, x, y, k * 85.0, heading=a + 90.0, span=270, body=WHITE, wing=STEAM, beak=YEL, rate=0.7, flap=14, phase=r.random(),
              orbit=(0, 0, 0.034 + 0.003 * (k % 3)), thick=5.0)
        hx, hy = on_ring(rr, a)                                        # a round head, so it is a gull and not a dart
        hd = sc.part(P.motion(pivot=(x, y, k * 85.0), phase=0.0, rate=0.0, base=(0.0, 0.0), travel=(-1.0, 0.034 + 0.003 * (k % 3))))
        hd.sphere(WHITE, (hx - math.sin(math.radians(a)) * 48.0, hy + math.cos(math.radians(a)) * 48.0, k * 85.0 + 8.0), 1.0, scale=(17, 17, 15), steps=(3, 5))


def stork_stack(sc):
    """s1d6, s1d7. A square brick stack with a stork's nest: one stork stands clattering its bill, its mate glides
    a circle (7.5 m) round the top."""
    g = sc.still()
    for (w, z0, z1) in ((300, -200, 400), (268, 400, 800), (236, 800, 1200), (208, 1200, 1540)):
        g.box(RUST, (0, 0, (z0 + z1) * 0.5), (w, w, z1 - z0))
    g.box(CREAM, (0, 0, 1555), (246, 246, 30))
    g.box(RUST, (0, 0, 1580), (216, 216, 20))
    g.box(STEEL, (-40, -152, 380), (8, 8, 760))                        # the ladder up the first lift
    g.box(STEEL, (40, -152, 380), (8, 8, 760))
    for k in range(9):
        g.box(STEEL, (0, -152, 60 + k * 84), (84, 6, 6))
    g.box(RUST, (-230, 20, 20), (160, 200, 240))                       # a lean-to at its foot: cream door, a lit pane
    g.box(CREAM, (-230, 20, 146), (180, 220, 12))
    g.box(CREAM, (-262, -81, 60), (50, 4, 110))
    g.box(LAMP, (-196, -81, 84), (40, 4, 40), glow=0.4)
    r = random.Random(4)
    for k in range(8):                                                 # the nest: a shaggy heap of sticks
        g.box(CREAM if k % 2 else STEAM, (r.uniform(-20, 20), r.uniform(-20, 20), 1596 + k * 5), (r.uniform(230, 300), 16, 12),
              rot=(r.uniform(-6, 6), k * 23.0, 0))
    g.box(WHITE, (0, 0, 1638), (120, 110, 10))
    fowl(sc, 10, -10, 1643, yaw=-30, s=2.3, body=WHITE, wing=STEAM, beak=RED, legs=RED, bl=46, bh=28, bw=24, leg=52, neck=38, neck_up=72, head=8.5,
         beak_len=30, tail=10, peck=20, rate=0.3, phase=0.3)
    P.bird(sc, 750, 0, 1900, heading=90, span=380, body=WHITE, wing=STEAM, beak=RED, rate=0.4, flap=7, phase=0.5, orbit=(0, 0, 0.05))


def doves(sc):
    """s1d7, s1d8. A dovecote made of a drum on a post, white doves on its landing boards, and a flight of six
    circling it 6 to 8 m out."""
    g = sc.still()
    g.box(DBLUE, (0, 0, 150), (18, 18, 700))
    for (dx, dy) in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        tilt_box(g, DBLUE, (dx * 9, dy * 9, 380), (dx * 80, dy * 80, 500), 8, 8)
    g.cyl(TEAL, (0, 0, 500), 96, 130, steps=10)
    g.cyl(YEL, (0, 0, 548), 98, 14, steps=10)
    g.cyl(WHITE, (0, 0, 630), 112, 8, steps=10)
    g.cyl(DTEAL, (0, 0, 638), 108, 84, steps=10, top_r=8)
    perch = []
    for k in range(4):
        a = 270.0 + k * 90.0
        hx, hy = on_ring(96, a)
        g.box(BLACK, (hx, hy, 596), (26, 5, 30), rot=(0, a + 90.0, 0))
        bx, by = on_ring(122, a)
        g.box(WHITE, (bx, by, 566), (60, 50, 5), rot=(0, a + 90.0, 0))
        perch.append((bx, by, a))
    for k, (bx, by, a) in enumerate(perch[:3]):
        fowl(sc, bx, by, 568.5, yaw=a - 270.0 + (20 if k else -15), s=1.8, body=WHITE, wing=STEAM, beak=PPINK, legs=PINK, bl=26, bh=16, bw=15, leg=6,
             neck=8, neck_up=60, head=6, beak_len=5, tail=9, turn=30, rate=0.3, phase=0.3 * k, feet=False)
    r = random.Random(9)
    for k in range(6):
        a = k * 60.0 + r.uniform(-18, 18)
        rr = 600 + r.uniform(0, 220)
        x, y = on_ring(rr, a)
        flyer(sc, x, y, 660 + r.uniform(0, 240), heading=a + 90.0, span=150, body=WHITE, wing=STEAM, beak=PPINK, phase=r.random(), rate=1.8, flap=28,
              orbit=(0, 0, 0.07))


def lamp_moths(sc):
    """s1d2, s1d6. A tall swan-neck lamp post with a hooded lamp (steady) and nine pale moths circling it, 14 m up."""
    g = sc.still()
    g.box(LILAC, (0, 0, -40), (56, 56, 320))
    g.box(STEAM, (0, 0, 126), (66, 66, 12))
    g.box(WHITE, (0, 0, 760), (15, 15, 1280))
    for bz in (420, 800, 1180):
        g.box(LILAC, (0, 0, bz), (21, 21, 16))
    tilt_box(g, WHITE, (0, 0, 1392), (0, -70, 1478), 12, 12)
    tilt_box(g, WHITE, (0, -66, 1476), (0, -136, 1486), 12, 12)
    g.box(DPURPLE, (0, -136, 1470), (120, 84, 26))                     # the hood and its lens
    g.box(DPURPLE, (0, -136, 1490), (70, 50, 16))
    g.box(LAMP, (0, -136, 1454), (98, 64, 6), glow=1.0)
    r = random.Random(13)
    for k in range(9):
        a = k * 40.0 + r.uniform(-12, 12)
        rr = 110 + r.uniform(0, 110)
        cw = k % 3 == 0
        x, y = on_ring(rr, a)
        flyer(sc, x, y - 136, 1330 + r.uniform(0, 230), heading=a + (-90.0 if cw else 90.0), span=84, body=LAMP, wing=WHITE, phase=r.random(), rate=1.9,
              flap=40, orbit=(0, -136, (-1 if cw else 1) * (0.12 + 0.02 * (k % 4))), bob=-8)


JOBS = [
    ('SM_S1ANIMALS_GateGoat', gate_goat),
    ('SM_S1ANIMALS_WallCats', wall_cats),
    ('SM_S1ANIMALS_LabRabbits', lab_rabbits),
    ('SM_S1ANIMALS_Carthorse', carthorse),
    ('SM_S1ANIMALS_PipeRats', pipe_rats),
    ('SM_S1ANIMALS_HenRun', hen_run),
    ('SM_S1ANIMALS_Greyhounds', greyhounds),
    ('SM_S1ANIMALS_Salamanders', salamanders),
    ('SM_S1ANIMALS_PelicanPiles', pelican_piles),
    ('SM_S1ANIMALS_Flamingos', flamingos),
    ('SM_S1ANIMALS_Swans', swans),
    ('SM_S1ANIMALS_KoiPool', koi_pool),
    ('SM_S1ANIMALS_SealSlip', seal_slip),
    ('SM_S1ANIMALS_LagoonFrogs', lagoon_frogs),
    ('SM_S1ANIMALS_WhiteOwl', white_owl, dict(far=True, grow=(120, 1900, 120), grow_down=(3760, 1900, 120))),
    ('SM_S1ANIMALS_GullWheel', gull_wheel, dict(far=True, grow=(500, 500, 120), grow_down=(500, 500, 120))),
    ('SM_S1ANIMALS_StorkStack', stork_stack, dict(far=True, grow=(80, 800, 100), grow_down=(820, 800, 0))),
    ('SM_S1ANIMALS_Doves', doves, dict(far=True, grow=(400, 400, 80), grow_down=(400, 400, 0))),
    ('SM_S1ANIMALS_LampMoths', lamp_moths, dict(far=True, grow=(220, 160, 60), grow_down=(220, 260, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; NEVER build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
