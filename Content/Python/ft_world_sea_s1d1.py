"""Under the Sea, Stage 1, builder s1d1: district 1 "The reef" (0 to 1.7 km), a village grown out of coral.
/Game/World/UnderTheSea/SM_S1D1_*. Run: import ft_world_sea_s1d1 as K; K.run()

Every piece is an ft_people.Scene: one material (M_People), one section, no collision. Round, soft, no straight wall:
urchin-shell and conch huts on short rock stilts, ribbed dome roofs with a hole at the top, spiral ramps.
Palette (the kit's own): coral, peach, magenta, lavender, shell cream; rock dusty rose; nothing blue, nothing green or
orange at pickup size. Light is steady (windows, lamp globes); nothing blinks. No words anywhere: picture signs only.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

CORAL, PEACH, MAG, LAV, CREAM = '#ff7a6a', '#ffb04f', '#ff4fb8', '#b9a4ff', '#fff1c0'
ROCK, ROCK_D, ROCK_L = '#9a6a7e', '#6a4562', '#c79aa2'
INK, PEARL, STRAW, GLASS, WEED, WEED_L = '#2b2350', '#f4efe0', '#c9b44a', '#cfeeff', '#3f8a4a', '#7fcf6a'


def pol(cx, cy, r, a):
    """A point r from (cx, cy) at a degrees: 0 = +X, -90 = the front (-Y, the road)."""
    return (cx + r * math.cos(math.radians(a)), cy + r * math.sin(math.radians(a)))


def stilts(g, pts, r=46.0, top=110.0, foot=220.0):
    for k, (x, y) in enumerate(pts):
        g.cyl(ROCK if k % 2 else ROCK_D, (x, y, -foot), r, top + foot, steps=6, top_r=r * 0.62)


DOME = ((1.0, 0.0), (0.88, 0.42), (0.56, 0.82), (0.2, 1.0))


def shell_roof(g, x, y, z, R, H, col, rib=CREAM, n=8, steps=10, prof=DOME, hole=True, a0=0.0):
    """A ribbed shell dome: stacked rings of the profile (radius, height as fractions), n ribs, a hole at the top."""
    for (r0, h0), (r1, h1) in zip(prof, prof[1:]):
        g.cyl(col, (x, y, z + h0 * H), r0 * R, (h1 - h0) * H, steps=steps, top_r=max(r1 * R, 3.0))
    for k in range(n):
        a = a0 + k * 360.0 / n
        for (r0, h0), (r1, h1) in zip(prof, prof[1:]):
            p0, p1 = pol(x, y, r0 * R + 5, a), pol(x, y, r1 * R + 5, a)
            tilt_box(g, rib, (p0[0], p0[1], z + h0 * H), (p1[0], p1[1], z + h1 * H + 3), max(R * 0.055, 12), max(R * 0.04, 10))
    if hole:
        rt = prof[-1][0] * R
        g.cyl(rib, (x, y, z + H - 6), rt + 10, 20, steps=8)
        g.cyl(INK, (x, y, z + H + 10), rt * 0.72, 7, steps=8)


def mouth(g, cx, cy, z, R, a, r, rim=CREAM, dark=INK, glow=0.0, steps=6, deep=14.0, proud=16.0):
    """A round door or window on a round wall of radius R, at angle a: a rim and the dark (or lit) mouth in it."""
    p0, p1, p2 = pol(cx, cy, R - deep, a), pol(cx, cy, R + proud, a), pol(cx, cy, R + proud + 7, a)
    rod(g, rim, (p0[0], p0[1], z), (p1[0], p1[1], z), r + max(r * 0.14, 8), steps=steps)
    rod(g, dark, (p0[0], p0[1], z), (p2[0], p2[1], z), r, steps=steps, glow=glow)


def ramp(g, cx, cy, R0, R1, z0, z1, a0, turns, n, w=110.0, col=CREAM, post=None, t=14.0, every=2):
    """A spiral ramp round (cx, cy): n flat segments from (R0, z0) at angle a0 to (R1, z1) after `turns`."""
    pts = []
    for i in range(n + 1):
        f = i / float(n)
        x, y = pol(cx, cy, R0 + (R1 - R0) * f, a0 + turns * 360.0 * f)
        pts.append((x, y, z0 + (z1 - z0) * f))
    for i in range(n):
        a, b = pts[i], pts[i + 1]
        e = 0.06
        tilt_box(g, col, tuple(a[j] - (b[j] - a[j]) * e for j in range(3)), tuple(b[j] + (b[j] - a[j]) * e for j in range(3)), w, t)
        if post and i % every == 0:
            f = (i + 0.5) / float(n)
            x, y = pol(cx, cy, R0 + (R1 - R0) * f + w * 0.5 - w * 0.07, a0 + turns * 360.0 * f)
            zz = z0 + (z1 - z0) * f
            g.box(post, (x, y, zz + w * 0.32), (w * 0.1, w * 0.1, w * 0.64))
            g.sphere(PEARL, (x, y, zz + w * 0.7), w * 0.09, steps=(3, 5))
    return pts


def awning(g, x, y, z, w, out, cols=(CREAM, MAG, CREAM), drop=32.0, a=-90.0):
    """Striped slats over a door at (x, y, z), `out` long, facing a (the front by default)."""
    n = len(cols)
    ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
    for k, c in enumerate(cols):
        o = (k - (n - 1) / 2.0) * w / n
        px, py = x - sa * o, y + ca * o
        tilt_box(g, c, (px, py, z), (px + ca * out, py + sa * out, z - drop), w / n - 2, 8)


def planter(g, x, y, z, col=LAV, r=34.0, tufts=(MAG, PEARL, CREAM)):
    """A shell pot with a tuft of sea flowers (still)."""
    g.cyl(col, (x, y, z), r * 0.6, r * 1.2, steps=6, top_r=r)
    g.cyl(CREAM, (x, y, z + r * 1.2 - 3), r + 4, 8, steps=6)
    for k, c in enumerate(tufts):
        px, py = pol(x, y, r * 0.45, k * 120.0 + 20)
        tilt_box(g, ROCK_D, (x, y, z + r * 1.2), (px, py, z + r * 2.3 + k * 6), 5, 5)
        g.sphere(c, (px, py, z + r * 2.4 + k * 6), r * 0.3, steps=(3, 5))


def streamer(sc, x, y, z, length, cols, height=40.0, swing=12.0, rate=0.2, phase=0.0, thick=6.0, segs=3, a=0.0):
    """A pennant of weed or cloth on a mast top: `segs` pieces, each swinging a little more and later than the last."""
    ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
    L = length / float(segs)
    for k in range(segs):
        m = sc.part(P.motion(swing=swing * (k + 1.0) / segs, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase + 0.07 * k))
        c0, c1 = L * k + 6.0, L * (k + 1.0) + 10.0
        mid = (c0 + c1) / 2.0
        m.box(cols[k % len(cols)], (x + ca * mid, y + sa * mid, z - height * 0.5 * (1.0 - 0.22 * k) - k * 5.0),
              (c1 - c0, thick, height * (1.0 - 0.22 * k)), rot=(0, a, 0))


def fan(g, hinge, ends, cols, w, start=0.28, thick=10.0, knob=CREAM, knob_r=0.0):
    """A scallop fan: flat ribs from near the hinge to each end point, every other one a hand higher (no two faces lie
    in one plane), a knob at each tip."""
    for k, e in enumerate(ends):
        up = 6.0 if k % 2 else 0.0
        a = tuple(hinge[j] + (e[j] - hinge[j]) * start for j in range(3))
        tilt_box(g, cols[k % len(cols)], (a[0], a[1], a[2] + up), (e[0], e[1], e[2] + up), w, thick)
        if knob_r:
            g.sphere(knob, (e[0], e[1], e[2] + up), knob_r, steps=(3, 5))


def kerb(g, length, depth=104.0, col=ROCK_L):
    """What a low prop stands on: a strip of pale rock, its top 8 cm over the sand, 1 m of footing."""
    g.box(col, (0, 0, -46), (length, depth, 108))
    return 8.0


# ============================================================================================ BUILDINGS
def urchin_hut(sc):
    """BUILDING 1. An urchin-shell hut: 6.7 m across, 7 m to its pennant. One round room on four rock stilts under a
    ribbed magenta dome with spines and a smoke hole; a plank ramp to the door. The pennant sways."""
    g = sc.still()
    FL = 120.0
    stilts(g, [pol(0, 0, 225, a) for a in (45, 135, 225, 315)], r=48, top=FL)
    g.cyl(CREAM, (0, 0, FL), 335, 22, steps=12)
    g.cyl(CORAL, (0, 0, FL + 22), 285, 190, steps=12, top_r=298)
    g.cyl(CREAM, (0, 0, FL + 206), 322, 18, steps=12)
    RZ = FL + 222
    shell_roof(g, 0, 0, RZ, 320, 250, MAG, n=8, steps=12)
    for k in range(8):                                               # the urchin's spines, between the ribs
        a = 22.5 + 45.0 * k
        for rr, hh, L in ((0.93, 0.24, 78.0),) if k % 2 else ((0.93, 0.24, 78.0), (0.7, 0.66, 60.0)):
            p = pol(0, 0, rr * 320 - 8, a)
            q = pol(0, 0, rr * 320 + L * 0.8, a)
            rod(g, LAV, (p[0], p[1], RZ + hh * 250), (q[0], q[1], RZ + hh * 250 + L * 0.55), 11, steps=4)
    mouth(g, 0, 0, FL + 22 + 92, 290, -90, 78, steps=10)             # the door mouth
    for a, lit in ((-38, 0.6), (-142, 0.0), (35, 0.6), (145, 0.6)):
        mouth(g, 0, 0, FL + 150, 292, a, 34, dark=(CREAM if lit else INK), glow=lit)
    awning(g, 0, -300, FL + 232, 230, 120)
    tilt_box(g, CREAM, (0, -325, FL + 12), (0, -600, -30), 120, 14)   # the plank ramp and its cleats
    for k in range(3):
        g.box(LAV, (0, -386 - k * 70, FL - 9 - k * 38), (124, 10, 10))
    planter(g, -150, -318, FL + 22, LAV)
    planter(g, 150, -318, FL + 22, PEACH, tufts=(PEARL, MAG, LAV))
    g.cyl(CREAM, (0, 0, RZ + 250), 6, 150, steps=5)                  # a mast through the smoke hole's rim
    streamer(sc, 0, 0, RZ + 392, 170, (MAG, LAV, MAG), height=36, phase=0.2)


def limpet_store(sc):
    """BUILDING 2. A limpet-shell store: 6.2 m across, 5.4 m tall. A round peach room on a rock plinth under a wide
    ribbed lavender limpet cone; an open counter with jars under a striped awning, a vent pipe, a picture board (a
    pearl), two cowrie stools. A string of shells sways under the awning."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -220), 300, 260, steps=8, top_r=272)
    g.cyl(PEACH, (0, 0, 40), 225, 178, steps=10)
    g.box(INK, (0, -212, 152), (214, 40, 96))                         # the counter's opening
    g.box(CREAM, (0, -250, 100), (256, 76, 12))
    g.box(CREAM, (-118, -236, 152), (14, 30, 104)); g.box(CREAM, (118, -236, 152), (14, 30, 104))
    for k, c in enumerate((LAV, PEARL, MAG, CREAM, LAV)):             # jars and a heap of pearls on the counter
        g.cyl(c, (-88 + k * 44, -262, 106), 11, 22 + (k % 2) * 8, steps=5)
    for k in range(3):
        g.sphere(PEARL, (-12 + k * 14, -236, 114), 8, steps=(3, 5))
    prof = ((1.0, 0.0), (0.72, 0.4), (0.4, 0.78), (0.1, 1.0))
    shell_roof(g, 0, 0, 214, 312, 300, LAV, n=10, steps=10, prof=prof, hole=False, a0=18)
    g.sphere(PEARL, (0, 0, 528), 30, steps=(4, 6))
    awning(g, 0, -232, 250, 268, 130, cols=(MAG, CREAM, MAG, CREAM), drop=44)
    rod(g, CREAM, (150, 150, 300), (220, 220, 380), 20, steps=6)      # the vent pipe: an elbow and a cap
    g.cyl(CREAM, (220, 220, 372), 20, 70, steps=6)
    g.cyl(MAG, (220, 220, 442), 32, 12, steps=6, top_r=8)
    g.cyl(CREAM, (268, -210, -150), 9, 430, steps=5)                  # the picture board: a pearl in its shell
    rod(g, CORAL, (268, -206, 262), (268, -222, 262), 46, steps=8)
    rod(g, LAV, (268, -216, 256), (268, -228, 256), 30, steps=6)
    rod(g, PEARL, (268, -224, 262), (268, -236, 262), 15, steps=6)
    for x in (-150, -70):                                             # two cowrie stools on the plinth's lip
        g.sphere(MAG if x < -100 else CREAM, (x, -330, 14), 30, scale=(1.2, 1.0, 0.8), steps=(4, 6))
    g.cyl(ROCK_L, (-110, -330, -200), 90, 196, steps=6)
    m = sc.part(P.motion(swing=7, pivot=(0, -340, 214), axis=0.0, rate=0.3, phase=0.4))   # shells on a string
    m.box(STRAW, (0, -340, 208), (230, 5, 5))
    for k, c in enumerate((PEARL, MAG, CREAM, LAV, PEARL)):
        m.cyl(c, (-92 + k * 46, -340, 180), 5, 24, steps=5, top_r=13)


def _whorls(g, x, y, z, tiers, steps=10, band=CREAM, bh=22.0):
    """Stacked tapering rings (col, r0, r1, h), a cream band at each joint. Returns [(z0, r0, r1, h)]."""
    out = []
    for k, (c, r0, r1, h) in enumerate(tiers):
        g.cyl(c, (x, y, z), r0, h, steps=steps, top_r=max(r1, 3.0))
        out.append((z, r0, r1, h))
        z += h
        if k < len(tiers) - 1:
            g.cyl(band, (x, y, z - bh * 0.5), max(r1, tiers[k + 1][1]) + bh * 0.6, bh, steps=steps)
    return out


def conch_house(sc):
    """BUILDING 3. A conch house: two storeys and a spire, 11.4 m tall, 9 m across with its ramp. Coral ground floor,
    peach upper floor, magenta and lavender spire with a ring of horns; a spiral ramp winds once round to the upper
    door; a flared cream-and-magenta shell lip beside the front door; a washing line to a coral pole. The washing
    sways and the scallop vane on the tip turns slowly."""
    g = sc.still()
    FL = 130.0
    stilts(g, [pol(0, 0, 245, a) for a in (30, 150, 270)], r=54, top=FL, foot=260)
    g.cyl(CREAM, (0, 0, FL - 6), 352, 22, steps=12)
    T = _whorls(g, 0, 0, FL + 16, ((CORAL, 330, 300, 290), (PEACH, 288, 238, 260), (MAG, 232, 160, 150), (LAV, 152, 95, 130), (MAG, 88, 14, 150)), steps=12)
    Z2 = T[1][0]                                                      # the upper floor
    for k in range(8):                                                # the conch's horns on the shoulder
        a = 20 + 45.0 * k
        p, q = pol(0, 0, 236, a), pol(0, 0, 300, a)
        rod(g, CREAM, (p[0], p[1], T[2][0] - 8), (q[0], q[1], T[2][0] + 46), 14, steps=4)
    mouth(g, 0, 0, FL + 16 + 96, 326, -90, 80, steps=10)
    for a, lit in ((-150, 0.6), (-25, 0.6), (40, 0.0), (100, 0.6), (165, 0.6)):
        mouth(g, 0, 0, FL + 190, 314, a, 36, dark=(CREAM if lit else INK), glow=lit)
    mouth(g, 0, 0, Z2 + 88, 276, -50, 64, steps=8)                    # the upper door, where the ramp ends
    for a, lit in ((-118, 0.6), (-178, 0.0), (12, 0.6), (74, 0.6), (132, 0.0)):
        mouth(g, 0, 0, Z2 + 150, 262, a, 32, dark=(CREAM if lit else INK), glow=lit)
    for a in (-90, 30, 150):
        mouth(g, 0, 0, T[2][0] + 76, 198, a, 22, dark=CREAM, glow=0.6)
    pts = ramp(g, 0, 0, 455, 392, -30, Z2 + 4, -50, 1.0, 12, w=112, post=LAV)
    for i in (3, 6, 9):                                               # rock props under the ramp
        g.cyl(ROCK, (pts[i][0], pts[i][1], -200), 26, pts[i][2] + 196, steps=5, top_r=16)
    g.box(CREAM, (-372, -214, FL + 290), (30, 330, 440), rot=(0, 35, 0))     # the flared lip
    g.box(MAG, (-362, -228, FL + 290), (18, 280, 390), rot=(0, 35, 0))
    for k in range(4):
        rod(g, CREAM, (-372 - 96, -214 - 134, FL + 130 + k * 110), (-372 - 130, -214 - 182, FL + 160 + k * 110), 13, steps=4)
    rod(g, CORAL, (640, 170, -200), (628, 170, 470), 13, steps=5)      # the washing line
    g.sphere(PEARL, (628, 170, 482), 18, steps=(3, 5))
    a0, a1 = (268, 70, Z2 + 60), (628, 170, 452)
    tilt_box(g, STRAW, a0, a1, 5, 5)
    for k, (c, w, h) in enumerate(((LAV, 62, 86), (CREAM, 54, 104), (MAG, 46, 70))):
        f = 0.3 + 0.24 * k
        px, py, pz = (a0[0] + (a1[0] - a0[0]) * f, a0[1] + (a1[1] - a0[1]) * f, a0[2] + (a1[2] - a0[2]) * f)
        m = sc.part(P.motion(swing=9, pivot=(px, py, pz), axis=15.5, rate=0.3, phase=0.2 + 0.3 * k))
        m.box(c, (px, py, pz - h * 0.5), (w, 5, h), rot=(0, 15.5, 0))
    top = T[4][0] + 150
    g.cyl(CREAM, (0, 0, top - 20), 6, 110, steps=5)
    m = sc.part(P.motion(swing=32, pivot=(0, 0, top + 70), axis=P.UP, rate=0.08, phase=0.1))      # the scallop vane
    m.box(CREAM, (0, 0, top + 70), (190, 6, 8))
    fan(m, (-70, 0, top + 70), [(-70 + 60 * math.cos(math.radians(t)), 0, top + 70 + 60 * math.sin(math.radians(t))) for t in (60, 90, 120, 150, 180)], (MAG, CREAM), 6, start=0.1, thick=26)
    m.sphere(PEARL, (96, 0, top + 70), 12, steps=(3, 5))


def scallop_market(sc):
    """BUILDING 4. The scallop-roofed market hall: 13 m wide, 8 m deep, 5.8 m tall, open on every side. A fan of nine
    cream and magenta ribs from a coral trunk at the back, on five forked coral columns; three stalls (pearls,
    sponges, corals and jars), bunting without words, two shell lamps (steady) and a picture sign (a shell) that sway."""
    g = sc.still()
    H = (0.0, 330.0, 560.0)
    ts = [-62 + k * 15.5 for k in range(9)]
    ends = [(660 * math.sin(math.radians(t)), 330 - 720 * math.cos(math.radians(t)), 400.0) for t in ts]
    pl = [(-150, 430), (150, 430)] + [(700 * math.sin(math.radians(t)), 330 - 760 * math.cos(math.radians(t))) for t in (66, 44, 22, 0, -22, -44, -66)]
    pl.reverse()                                                      # counter-clockwise seen from above
    g.plan(ROCK_L, pl, -220, 40)
    g.cyl(CORAL, (0, 330, 40), 44, 520, steps=7, top_r=30)            # the trunk the fan springs from
    g.sphere(CREAM, (0, 330, 572), 62, scale=(1, 1, 0.6), steps=(4, 7))
    fan(g, H, ends, (CREAM, MAG), 176, start=0.1, thick=12, knob_r=30)
    for t in (-56, -28, 0, 28, 56):                                   # forked coral columns under the rim
        x, y = 610 * math.sin(math.radians(t)), 330 - 660 * math.cos(math.radians(t))
        rod(g, CORAL, (x, y, 40), (x, y, 300), 20, steps=6)
        for s in (-1, 1):
            rod(g, CORAL, (x, y, 290), (x + s * 46, y - 10, 404), 13, steps=5)
        g.cyl(CREAM, (x, y, 40), 30, 20, steps=6)
    for k, t in enumerate((-40, 0, 40)):                              # three stalls
        x, y = 400 * math.sin(math.radians(t)), 330 - 440 * math.cos(math.radians(t))
        g.box(CREAM, (x, y, 128), (200, 84, 10), rot=(0, -t, 0))
        g.cyl(LAV if k != 1 else MAG, (x, y, 40), 26, 84, steps=6, top_r=40)
        if k == 0:
            g.cyl(LAV, (x, y, 133), 52, 8, steps=7)
            for j in range(5):
                p = pol(x, y, 24, j * 72)
                g.sphere(PEARL, (p[0], p[1], 150), 13, steps=(3, 5))
        elif k == 1:
            for j, c in enumerate((CREAM, LAV, PEARL, CREAM)):
                g.cyl(c, (x - 66 + j * 44, y, 133), 15, 26 + (j % 2) * 10, steps=6, top_r=19)
        else:
            for j, c in enumerate((MAG, CORAL, LAV)):
                bx = x - 50 + j * 50
                g.cyl(CREAM, (bx, y, 133), 12, 20, steps=5)
                for s in (-1, 0, 1):
                    rod(g, c, (bx, y, 150), (bx + s * 14, y + s * 6, 196 + abs(s) * -8), 5, steps=4)
    awning(g, -560, 60, 330, 220, 130, cols=(MAG, CREAM, MAG), drop=40, a=180)        # a side awning and its bench
    g.box(CREAM, (-600, 60, 78), (60, 190, 10)); g.box(LAV, (-600, 0, 56), (40, 20, 36)); g.box(LAV, (-600, 120, 56), (40, 20, 36))
    for k in range(8):                                                # bunting along the front of the fan
        a, b = ends[k], ends[k + 1]
        tilt_box(g, STRAW, (a[0], a[1], 392), (b[0], b[1], 392), 5, 5)
        mx, my = (a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0
        g.box((MAG, CREAM, LAV)[k % 3], (mx, my, 374), (30, 5, 32), rot=(0, -(ts[k] + 7.75), 0))
    for s, ph in ((-1, 0.1), (1, 0.55)):                              # two shell lamps hung under the fan
        m = sc.part(P.motion(swing=6, pivot=(s * 230, -90, 470), axis=90.0, rate=0.25, phase=ph))
        m.box(STRAW, (s * 230, -90, 420), (5, 5, 100))
        m.cyl(CREAM, (s * 230, -90, 356), 12, 18, steps=6, top_r=26)
        m.sphere(LAV, (s * 230, -90, 344), 22, steps=(3, 6), glow=0.7)
    m = sc.part(P.motion(swing=8, pivot=(0, -392, 396), axis=0.0, rate=0.3, phase=0.8))           # the picture sign
    m.box(STRAW, (-34, -392, 372), (5, 5, 48)); m.box(STRAW, (34, -392, 372), (5, 5, 48))
    rod(m, CREAM, (0, -388, 312), (0, -400, 312), 50, steps=8)
    rod(m, MAG, (-6, -398, 308), (-6, -408, 308), 30, steps=7)
    rod(m, CREAM, (4, -406, 314), (4, -414, 314), 15, steps=6)


def barnacle_tower(sc):
    """BUILDING 5. The barnacle tower house: three stacked barnacle cones (coral, peach, lavender), 12.2 m, with a
    cream crater rim and plate ridges on each, a balcony round the middle one, three small barnacles at its foot and
    a pearl-diver's hoist: the basket swings on its rope, the streamer on the rim sways."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -260), 430, 320, steps=8, top_r=384)
    tiers = ((CORAL, 0, 60, 360, 250, 420), (PEACH, 22, 480, 285, 200, 380), (LAV, -8, 860, 215, 140, 330))
    for c, ox, z, r0, r1, h in tiers:
        g.cyl(c, (ox, 0, z), r0, h, steps=10, top_r=r1)
        g.cyl(CREAM, (ox, 0, z + h - 10), r1 + 16, 26, steps=10)
        for k in range(6):
            a = 30 + 60.0 * k
            p, q = pol(ox, 0, r0 + 4, a), pol(ox, 0, r1 + 6, a)
            tilt_box(g, CREAM, (p[0], p[1], z + 6), (q[0], q[1], z + h - 12), 24, 12)
    g.cyl(INK, (-8, 0, 860 + 330 + 12), 118, 7, steps=10)             # the top crater
    g.cyl(CREAM, (22, 0, 462), 350, 16, steps=10)                     # the balcony
    bp = [pol(22, 0, 336, 18 + k * 36) for k in range(10)]
    for k in range(10):
        g.box(LAV, (bp[k][0], bp[k][1], 478 + 38), (11, 11, 76))
        n = bp[(k + 1) % 10]
        tilt_box(g, CREAM, (bp[k][0], bp[k][1], 478 + 74), (n[0], n[1], 478 + 74), 8, 8)
    mouth(g, 0, 0, 60 + 100, 334, -90, 86, steps=10)
    awning(g, 0, -340, 60 + 222, 240, 120, cols=(LAV, CREAM, LAV))
    for a, lit in ((-38, 0.6), (38, 0.0), (140, 0.6), (-140, 0.6)):
        mouth(g, 0, 0, 60 + 270, 289, a, 36, dark=(CREAM if lit else INK), glow=lit)
    mouth(g, 22, 0, 478 + 80, 268, -90, 58, steps=8)                  # the balcony door
    for a, lit in ((0, 0.6), (90, 0.6), (180, 0.0)):
        mouth(g, 22, 0, 480 + 190, 242, a, 32, dark=(CREAM if lit else INK), glow=lit)
    for a, lit in ((-90, 0.6), (30, 0.6), (150, 0.0)):
        mouth(g, -8, 0, 860 + 150, 181, a, 30, dark=(CREAM if lit else INK), glow=lit)
    for (a, c, s) in ((-28, CORAL, 1.0), (205, LAV, 0.85), (118, PEACH, 0.7)):      # little barnacles at the foot
        p = pol(0, 0, 430, a)
        g.cyl(c, (p[0], p[1], -200), 112 * s, 200 + 150 * s, steps=7, top_r=58 * s)
        g.cyl(CREAM, (p[0], p[1], 150 * s - 8), 66 * s, 16, steps=7)
        g.cyl(INK, (p[0], p[1], 150 * s + 8), 46 * s, 5, steps=7)
    tilt_box(g, CORAL, (-8, -110, 1150), (-8, -330, 1196), 16, 16)     # the hoist: an arm, a rope, a basket of pearls
    tilt_box(g, CORAL, (-8, -128, 1050), (-8, -250, 1176), 12, 12)
    m = sc.part(P.motion(swing=4, pivot=(-8, -322, 1190), axis=90.0, rate=0.22, phase=0.3))
    m.box(STRAW, (-8, -322, 1030), (5, 5, 320))
    m.cyl(STRAW, (-8, -322, 832), 26, 42, steps=7, top_r=36)
    m.cyl(CREAM, (-8, -322, 868), 38, 8, steps=7)
    for k in range(3):
        p = pol(-8, -322, 14, k * 120)
        m.sphere(PEARL, (p[0], p[1], 884), 12, steps=(3, 5))
    g.cyl(CREAM, (110, 60, 1190), 6, 150, steps=5)
    streamer(sc, 110, 60, 1332, 210, (MAG, CREAM, MAG), height=40, phase=0.6)


def clam_band_shell(sc):
    """BUILDING 6. The clam-shell band shell: a giant clam 10.4 m across, its lid propped open on two coral struts,
    7.3 m tall. A round cream stage in the lower shell with a conch horn on a stand, two shell drums and a weed-string
    harp (no players: the people builder's), five shell footlights (steady), steps. Two streamers on the lid sway."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -240), 340, 250, steps=8, top_r=310)
    g.cyl(LAV, (0, 0, 0), 300, 150, steps=12, top_r=520)              # the lower shell, a ribbed bowl
    for k in range(12):
        a = 15 + 30.0 * k
        p, q = pol(0, 0, 304, a), pol(0, 0, 526, a)
        tilt_box(g, CREAM, (p[0], p[1], 2), (q[0], q[1], 152), 26, 10)
    g.cyl(CREAM, (0, 0, 150), 508, 14, steps=12)                      # the stage
    HG = (0.0, 440.0, 170.0)
    el = math.radians(58.0)
    def lid(t, rho):
        tr = math.radians(t)
        return (HG[0] + rho * math.sin(tr), HG[1] - rho * math.cos(tr) * math.cos(el), HG[2] + rho * math.cos(tr) * math.sin(el) + abs(math.sin(tr)) * 90.0)
    ts = [-66 + k * 16.5 for k in range(9)]
    fan(g, HG, [lid(t, 650.0) for t in ts], (MAG, CORAL), 188, start=0.1, thick=14, knob_r=34)
    g.sphere(CREAM, (0, 440, 176), 90, scale=(1.6, 0.7, 0.7), steps=(4, 7))            # the hinge
    for s in (-1, 1):                                                 # the struts that prop the lid
        e = lid(s * 34, 500.0)
        rod(g, CORAL, (s * 380, -40, 160), (e[0], e[1], e[2] - 8), 16, steps=6)
        rod(g, CORAL, (s * 378, 60, 330), (s * 300, 150, 420), 10, steps=5)
    g.cyl(CREAM, (-150, 60, 164), 8, 120, steps=5)                    # the conch horn on its stand
    g.cyl(MAG, (-150, 76, 280), 12, 130, rot=(52, 90, 0), steps=7, top_r=62)
    for x, c in ((120, MAG), (220, LAV)):                             # two shell drums
        g.cyl(CREAM, (x, 80, 164), 40, 52, steps=8); g.cyl(c, (x, 80, 186), 43, 12, steps=8); g.cyl(PEARL, (x, 80, 216), 36, 5, steps=8)
    tilt_box(g, LAV, (10, 190, 164), (-40, 190, 330), 12, 12)          # the harp
    tilt_box(g, LAV, (-40, 190, 330), (70, 190, 300), 12, 12)
    tilt_box(g, LAV, (10, 190, 164), (70, 190, 300), 10, 10)
    for k in range(4):
        g.box(CREAM, (-18 + k * 20, 190, 240 + k * 6), (4.5, 4.5, 110 - k * 14))
    for k in range(5):                                                # footlights along the stage's lip
        p = pol(0, 0, 486, -130 + k * 20)
        g.cyl(CREAM, (p[0], p[1], 164), 20, 12, steps=6, top_r=26)
        g.sphere(LAV, (p[0], p[1], 186), 17, steps=(3, 6), glow=0.7)
    for k in range(3):                                                # the steps
        g.box(CREAM if k % 2 == 0 else LAV, (0, -520 - k * 46, 126 - k * 44), (240, 60, 40 + k * 0))
    g.box(ROCK_L, (0, -590, -110), (250, 190, 220))
    for s, ph in ((-1, 0.15), (1, 0.6)):
        e = lid(s * 16.5, 650.0)
        g.cyl(CREAM, (e[0], e[1], e[2]), 6, 110, steps=5)
        streamer(sc, e[0], e[1], e[2] + 104, 180, (CREAM, LAV, CREAM), height=34, phase=ph)


def brain_bath(sc):
    """BUILDING 7. The brain-coral bath house: a coral dome 8.8 m across wound with cream meanders, a round pool in
    front (pale water in a shell-set rim), 14 m deep in all, 4.6 m tall. Three vents on top let bubbles go (they
    drift up and down), towels sway on a rack, two cowrie loungers by the pool."""
    g = sc.still()
    CY, R, K = 250.0, 430.0, 0.7
    g.cyl(ROCK, (0, CY, -220), 480, 222, steps=10, top_r=456)
    g.cyl(CREAM, (0, CY, 0), 442, 72, steps=12)
    g.sphere(CORAL, (0, CY, 70), R, scale=(1, 1, K), steps=(7, 12))
    def on(phi, a, lift=1.012):
        s, c = math.sin(math.radians(phi)), math.cos(math.radians(phi))
        return (R * lift * s * math.cos(math.radians(a)), CY + R * lift * s * math.sin(math.radians(a)), 70 + R * K * lift * c)
    for k in range(7):                                                # the brain coral's meanders
        a0 = 12 + k * 51.4
        prev = None
        for j in range(5):
            phi = 14 + j * 16.5
            p = on(phi, a0 + 13 * math.sin(j * 1.9 + k))
            if prev:
                tilt_box(g, CREAM, prev, p, 28, 12)
            prev = p
    rod(g, CREAM, (0, CY - 330, 150), (0, CY - 448, 150), 96, steps=10)                # the door mouth, a porch
    rod(g, INK, (0, CY - 330, 150), (0, CY - 455, 150), 80, steps=10)
    for a, lit in ((-34, 0.6), (-146, 0.6), (22, 0.0), (158, 0.6)):
        p0, p1, p2 = pol(0, CY, 335, a), pol(0, CY, 412, a), pol(0, CY, 419, a)
        rod(g, CREAM, (p0[0], p0[1], 196), (p1[0], p1[1], 196), 40, steps=6)
        rod(g, CREAM if lit else INK, (p0[0], p0[1], 196), (p2[0], p2[1], 196), 33, steps=6, glow=lit)
    for k, a in enumerate((80, 200, 320)):                            # three vents and their bubbles
        p = pol(0, CY, 150, a)
        g.cyl(CREAM, (p[0], p[1], 320), 26, 86, steps=6)
        g.cyl(INK, (p[0], p[1], 406), 18, 5, steps=6)
        for j in range(2):
            m = sc.part(P.motion(bob=-16, rate=0.22 + 0.05 * j, phase=0.3 * k + 0.45 * j))
            m.sphere(GLASS, (p[0] + j * 14 - 7, p[1], 446 + j * 50), 15 - j * 4, steps=(3, 5))
    planter(g, -150, CY - 452, 0, LAV, r=40)
    planter(g, 150, CY - 452, 0, MAG, r=40, tufts=(PEARL, CREAM, LAV))
    PY = -470.0                                                       # the pool
    g.cyl(ROCK, (0, PY, -220), 272, 232, steps=10)
    g.cyl(CREAM, (0, PY, 10), 262, 40, steps=12)
    g.cyl(GLASS, (0, PY, 47), 232, 7, steps=12)
    for k in range(8):
        p = pol(0, PY, 248, 22.5 + k * 45)
        g.sphere(MAG if k % 2 else LAV, (p[0], p[1], 52), 24, scale=(1, 1, 0.55), steps=(3, 5))
    for s in (-1, 1):                                                 # cowrie loungers on rock pads
        g.cyl(ROCK_L, (s * 360, PY + 60, -200), 86, 214, steps=6)
        g.sphere(LAV if s < 0 else CREAM, (s * 360, PY + 60, 38), 62, scale=(0.85, 1.5, 0.5), steps=(4, 6))
        g.box(CREAM if s < 0 else MAG, (s * 360, PY + 60, 70), (8, 150, 6))
    for x in (-470, -330):                                            # the towel rack
        g.cyl(CORAL, (x, PY - 150, -150), 9, 300, steps=5)
    g.box(CREAM, (-400, PY - 150, 146), (160, 7, 7))
    for k, (c, c2) in enumerate(((CREAM, MAG), (LAV, CREAM))):
        m = sc.part(P.motion(swing=8, pivot=(-436 + k * 72, PY - 150, 146), axis=0.0, rate=0.28, phase=0.2 + 0.4 * k))
        m.box(c, (-436 + k * 72, PY - 150, 100), (52, 5, 92))
        m.box(c2, (-436 + k * 72, PY - 153, 84), (54, 5, 14))


# ============================================================================================ THE HERO
def great_conch(sc):
    """THE HERO. The Great Conch: a spiral shell 45 m tall standing on its point in a rock foot, ten whorls (coral
    and peach below the shoulder, magenta and lavender above) with a cream band at each turn, a ring of round windows
    (steady light) on seven of them, a cream ramp winding three times round it from the sand to the balcony on the
    shoulder, the great flared lip, ten horns. The pennant of weed on the tip sways slowly."""
    g = sc.still()
    PR = [(0, 150), (450, 330), (950, 520), (1500, 720), (2100, 930), (2700, 1100), (3100, 960), (3550, 680), (3950, 420), (4300, 200), (4500, 40)]
    cols = [CORAL, PEACH, CORAL, PEACH, CORAL, MAG, LAV, MAG, LAV, MAG]

    def rad(z):
        for (z0, r0), (z1, r1) in zip(PR, PR[1:]):
            if z <= z1:
                return r0 + (r1 - r0) * (z - z0) / float(z1 - z0)
        return PR[-1][1]

    g.cyl(ROCK, (0, 0, -300), 1000, 800, steps=10, top_r=390)         # the rock foot the point stands in
    for k, (a, c) in enumerate(((20, ROCK_L), (150, ROCK_D), (265, ROCK_L))):
        p = pol(0, 0, 880, a)
        g.sphere(c, (p[0], p[1], 60), 330, scale=(1.2, 1.0, 0.75), steps=(4, 7))
    for i in range(10):
        (z0, r0), (z1, r1) = PR[i], PR[i + 1]
        g.cyl(cols[i], (0, 0, z0), r0, z1 - z0, steps=14, top_r=r1)
        if i < 9:
            g.cyl(CREAM, (0, 0, z1 - 16), r1 + 24, 34, steps=14)
    for k in range(10):                                               # the horns on the shoulder
        a = 18 + 36.0 * k
        p, q = pol(0, 0, 1080, a), pol(0, 0, 1330, a)
        rod(g, CREAM, (p[0], p[1], 2760), (q[0], q[1], 2960), 46, steps=5)
    N, TURNS, Z0, Z1 = 36, 3.0, -40.0, 2690.0
    A_DOOR = -60 + TURNS * 360.0 * (720 - Z0) / (Z1 - Z0)             # where the ramp passes 7.2 m up
    n_win = (0, 5, 6, 6, 7, 6, 5, 3, 0, 0)
    for i in range(10):                                               # a ring of round windows at each turn
        if not n_win[i]:
            continue
        zc = (PR[i][0] + PR[i + 1][0]) / 2.0 + 30
        rm = rad(zc)
        for k in range(n_win[i]):
            a = 25 * i + k * 360.0 / n_win[i]
            if i == 1 and abs((a - A_DOOR + 180.0) % 360.0 - 180.0) < 42:
                continue
            lit = (k + i) % 3 != 0
            p0, p1, p2 = pol(0, 0, rm - 70, a), pol(0, 0, rm + 42, a), pol(0, 0, rm + 36, a)
            rod(g, CREAM, (p0[0], p0[1], zc), (p1[0], p1[1], zc), 78, steps=6)
            g.box(CREAM if lit else INK, (p2[0], p2[1], zc), (40, 92, 112), rot=(0, a, 0), glow=(0.8 if lit else 0.0))
    mouth(g, 0, 0, 720, rad(720), A_DOOR, 135, steps=10, deep=40, proud=40)            # the great door, on the ramp
    pts = []
    for i in range(N + 1):
        f = i / float(N)
        z = Z0 + (Z1 - Z0) * f
        rr = max(rad(max(z, 0.0)) + 175.0, 1000.0 - (z + 300.0) / 800.0 * 610.0 + 120.0 if z < 500 else 0.0)
        x, y = pol(0, 0, rr, -60 + TURNS * 360.0 * f)
        pts.append((x, y, z, rr))
    for i in range(N):
        a, b = pts[i], pts[i + 1]
        tilt_box(g, CREAM, (a[0] - (b[0] - a[0]) * 0.05, a[1] - (b[1] - a[1]) * 0.05, a[2]), (b[0] + (b[0] - a[0]) * 0.05, b[1] + (b[1] - a[1]) * 0.05, b[2]), 270, 40)
        if i % 3 == 1:
            f = (i + 0.5) / float(N)
            p = pol(0, 0, (a[3] + b[3]) / 2.0 + 112, -60 + TURNS * 360.0 * f)
            zz = (a[2] + b[2]) / 2.0
            g.box(LAV, (p[0], p[1], zz + 90), (28, 28, 180))
            g.sphere(PEARL, (p[0], p[1], zz + 200), 30, steps=(3, 5))
    g.cyl(CREAM, (0, 0, 2676), 1290, 30, steps=14)                    # the balcony on the shoulder
    for k in range(14):
        p = pol(0, 0, 1262, k * 360.0 / 14)
        g.box(MAG, (p[0], p[1], 2706 + 70), (30, 30, 140))
    A = -128.0                                                        # the flared lip, towards the road and the left
    p = pol(0, 0, 1260, A)
    g.box(CREAM, (p[0], p[1], 2300), (860, 76, 2100), rot=(0, A, 0))
    q = pol(p[0], p[1], 46, A + 90)
    g.box(MAG, (q[0], q[1], 2300), (760, 30, 1880), rot=(0, A, 0))
    for k in range(5):
        e = pol(0, 0, 1700, A)
        g.sphere(CREAM, (e[0], e[1], 1420 + k * 440), 125, steps=(4, 6))
    g.cyl(CREAM, (0, 0, 4470), 14, 330, steps=6)                      # the tip's mast and the pennant of weed
    g.sphere(PEARL, (0, 0, 4810), 34, steps=(4, 6))
    streamer(sc, 0, 0, 4760, 900, (WEED, WEED_L, WEED), height=130, swing=11, rate=0.12, phase=0.3, thick=16)


# ============================================================================================ LOW PROPS (17 m on)
def cowrie_stools(sc):
    """PROP 1, LOW. A shell table and four cowrie stools in a row on a strip of pale rock: 5.6 m long, 1 m deep,
    0.9 m tall. A dish of pearls and a shell cup on the table. Still."""
    g = sc.still()
    z = kerb(g, 560)
    g.cyl(CREAM, (0, 0, z), 22, 54, steps=6, top_r=14)
    g.cyl(CREAM, (0, 0, z + 54), 50, 10, steps=8)
    g.cyl(LAV, (-14, 0, z + 64), 22, 6, steps=6)
    for k in range(3):
        p = pol(-14, 0, 10, k * 120)
        g.sphere(PEARL, (p[0], p[1], z + 76), 8, steps=(3, 5))
    g.cyl(MAG, (24, -8, z + 64), 7, 16, steps=5, top_r=11)
    for k, (x, c) in enumerate(((-232, LAV), (-116, MAG), (116, CORAL), (232, LAV))):
        g.sphere(c, (x, 0, z + 26), 31, scale=(1.25, 1.0, 0.82), steps=(4, 6))
        g.box(CREAM, (x, 0, z + 50), (54, 7, 6))
    for x in (-174, 174):
        g.cyl(PEARL, (x, -30, z), 17, 5, steps=6)


def _basket(g, x, y, z, r=38.0, h=50.0, col=STRAW, band=LAV, pearls=True):
    g.cyl(col, (x, y, z), r * 0.78, h, steps=7, top_r=r)
    g.cyl(band, (x, y, z + h * 0.55), r * 0.96, 8, steps=7)
    if pearls:
        g.cyl(CREAM, (x, y, z + h - 2), r * 0.86, 5, steps=7)
        for k in range(3):
            p = pol(x, y, r * 0.38, k * 120 + 30)
            g.sphere(PEARL, (p[0], p[1], z + h + 7), r * 0.3, steps=(3, 5))
    else:
        g.cyl(col, (x, y, z + h), r, 14, steps=7, top_r=r * 0.3)


def basket_stack(sc):
    """PROP 2, LOW. A pearl diver's baskets: five straw baskets in two rows (three heaped with pearls, two lidded), a
    coil of rope, a weight stone, a net bag of shells: 4.4 m long, 1 m deep, 1.3 m tall. Straw and cream. Still."""
    g = sc.still()
    z = kerb(g, 440)
    for k, x in enumerate((-160, -72, 16)):
        _basket(g, x, 0, z, col=(STRAW if k != 1 else CREAM), band=(LAV, MAG, LAV)[k], pearls=(k == 2))
    _basket(g, -116, 0, z + 50, band=MAG, pearls=True)
    _basket(g, -28, 0, z + 50, col=CREAM, band=LAV, pearls=True)
    g.cyl(STRAW, (120, 0, z), 34, 12, steps=8); g.cyl(STRAW, (120, 0, z + 12), 25, 12, steps=8); g.cyl(ROCK_D, (120, 0, z + 24), 12, 5, steps=6)
    g.sphere(ROCK, (186, 10, z + 16), 22, scale=(1.2, 1, 0.8), steps=(3, 6))
    g.sphere(CREAM, (60, -18, z + 16), 20, scale=(1, 1, 0.8), steps=(3, 6))
    g.box(STRAW, (60, -18, z + 34), (6, 6, 12))


def pearl_fence(sc):
    """PROP 3, LOW. A pearl fence: five shell posts with a pearl on each, two sagging rope rails, a scallop medallion
    on each span: 6 m long, 1.2 m tall. Still."""
    g = sc.still()
    z = kerb(g, 600, depth=80)
    xs = (-270, -135, 0, 135, 270)
    for x in xs:
        g.cyl(CREAM, (x, 0, z), 17, 88, steps=6, top_r=10)
        g.sphere(PEARL, (x, 0, z + 100), 15, steps=(3, 6))
    for k in range(4):
        a, b = xs[k], xs[k + 1]
        mx = (a + b) / 2.0
        for h in (72.0, 40.0):
            tilt_box(g, STRAW, (a, 0, z + h), (mx, 0, z + h - 13), 6, 6)
            tilt_box(g, STRAW, (mx, 0, z + h - 13), (b, 0, z + h), 6, 6)
        rod(g, (MAG, LAV, CORAL, MAG)[k], (mx, -2, z + 43), (mx, -9, z + 43), 17, steps=7)
        rod(g, CREAM, (mx, -6, z + 43), (mx, -13, z + 43), 7, steps=5)


def conch_planters(sc):
    """PROP 4, LOW. Three conch-shell pots on rock stubs, a tuft of sea flowers in each (pearl, magenta, cream on
    plum stalks): 4.2 m long, 1.4 m tall. The tufts sway."""
    g = sc.still()
    z = kerb(g, 420, depth=90)
    r = random.Random(41)
    for k, (x, c) in enumerate(((-140, CORAL), (0, LAV), (140, MAG))):
        g.cyl(ROCK, (x, 0, z), 32, 30, steps=6, top_r=26)
        g.cyl(c, (x, 0, z + 30), 20, 46, steps=7, top_r=40)
        g.cyl(CREAM, (x, 0, z + 72), 44, 8, steps=7)
        rod(g, CREAM, (x + 30, 0, z + 40), (x + 56, 0, z + 30), 9, steps=4)      # the conch's spout
        m = sc.part(P.motion(swing=6, pivot=(x, 0, z + 78), axis=(0.0 if k % 2 else 90.0), rate=0.33, phase=r.random()))
        for j, fc in enumerate((PEARL, MAG, CREAM, LAV)):
            p = pol(x, 0, 24, j * 90 + 20 * k)
            tilt_box(m, ROCK_D, (x, 0, z + 76), (p[0], p[1], z + 112 + (j % 2) * 12), 5, 5)
            m.sphere(fc, (p[0], p[1], z + 118 + (j % 2) * 12), 10, steps=(3, 5))


# ============================================================================================ TALL PROPS (24 m on)
def coral_lamp(sc):
    """PROP 5, TALL. A coral-branch lamp post, 4.6 m: a coral trunk on a rock stub, three branches, a shell cup and
    a pearl globe on each (steady light, magenta and lavender; never amber, never mint). Still."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -150), 62, 180, steps=6, top_r=40)
    rod(g, CORAL, (0, 0, 30), (8, 0, 236), 17, steps=6)
    rod(g, CORAL, (8, 0, 232), (-6, 0, 336), 13, steps=6)
    ends = (((8, 0, 236), (96, 0, 322)), ((-6, 0, 336), (-84, 8, 398)), ((-6, 0, 336), (12, 0, 430)))
    for k, (a, b) in enumerate(ends):
        rod(g, CORAL, a, b, 10, steps=5)
        g.cyl(CREAM, (b[0], b[1], b[2] - 4), 12, 18, steps=6, top_r=27)
        g.sphere((MAG, LAV, LAV)[k], (b[0], b[1], b[2] + 30), 23, steps=(4, 6), glow=0.8)
    for a, b in (((4, 0, 130), (-34, 0, 168)), ((8, 0, 200), (40, 6, 214))):   # stubs of branches
        rod(g, CORAL, a, b, 8, steps=4)
        g.sphere(CREAM, b, 9, steps=(3, 5))


def shell_sign(sc):
    """PROP 6, TALL. The spiral-shell signpost, 4.1 m: a cream post with a spiral shell on top and three picture
    boards pointing along the road: a shell, a pearl, a starfish, each with an arrow head. No words. Still."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -150), 56, 176, steps=6, top_r=36)
    g.cyl(CREAM, (0, 0, 20), 11, 330, steps=6)
    for k, (r, h) in enumerate(((32, 18), (24, 16), (16, 14), (8, 14))):
        g.cyl(MAG if k % 2 == 0 else CREAM, (0, 0, 350 + sum((18, 16, 14, 14)[:k])), r, h, steps=7, top_r=r * 0.8)
    for k, (s, zc, c) in enumerate(((1, 300, CORAL), (-1, 236, LAV), (1, 172, MAG))):
        g.box(c, (s * 72, -3, zc), (118, 10, 50))
        tilt_box(g, c, (s * 128, -3, zc + 25), (s * 158, -3, zc), 10, 14)
        tilt_box(g, c, (s * 128, -3, zc - 25), (s * 158, -3, zc), 10, 14)
        g.box(CREAM, (s * 72, -3, zc + 27), (118, 12, 5)); g.box(CREAM, (s * 72, -3, zc - 27), (118, 12, 5))
        px = s * 66
        if k == 0:                                                    # a shell
            rod(g, CREAM, (px, -6, zc), (px, -13, zc), 18, steps=7)
            rod(g, LAV, (px - 4, -11, zc - 2), (px - 4, -17, zc - 2), 9, steps=5)
        elif k == 1:                                                  # a pearl in an open shell
            rod(g, MAG, (px, -6, zc - 4), (px, -13, zc - 4), 18, steps=5)
            rod(g, PEARL, (px, -11, zc + 2), (px, -18, zc + 2), 10, steps=6)
        else:                                                         # a starfish
            for j in range(5):
                a = math.radians(90 + j * 72)
                tilt_box(g, CREAM, (px, -11, zc), (px + 19 * math.cos(a), -11, zc + 19 * math.sin(a)), 6, 8)


def shell_chimes(sc):
    """PROP 7, TALL. The shell vane and chimes, 4.7 m: a lavender mast, a scallop vane with an arrow bar that turns
    slowly to and fro, and five shell chimes on a crossbar that sway each in its own time."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -150), 56, 176, steps=6, top_r=36)
    g.cyl(LAV, (0, 0, 20), 10, 400, steps=6)
    g.box(CREAM, (0, 0, 300), (264, 8, 8))
    r = random.Random(77)
    for k, c in enumerate((MAG, CREAM, CORAL, PEARL, MAG)):
        x = -112 + k * 56
        L = 50 + (k * 37) % 44
        m = sc.part(P.motion(swing=10, pivot=(x, 0, 298), axis=0.0, rate=0.3 + 0.03 * k, phase=r.random()))
        m.box(STRAW, (x, 0, 298 - L / 2.0), (5, 5, L))
        m.cyl(c, (x, 0, 298 - L - 26), 15, 28, steps=6, top_r=5)
    v = sc.part(P.motion(swing=34, pivot=(0, 0, 430), axis=P.UP, rate=0.09, phase=0.2))
    v.box(CREAM, (10, 0, 430), (210, 6, 8))
    tilt_box(v, CREAM, (96, 0, 446), (120, 0, 430), 6, 8); tilt_box(v, CREAM, (96, 0, 414), (120, 0, 430), 6, 8)
    fan(v, (-60, 0, 434), [(-60 + 62 * math.cos(math.radians(t)), 0, 434 + 62 * math.sin(math.radians(t))) for t in (50, 80, 110, 140, 170)], (MAG, CREAM), 6, start=0.1, thick=28)
    g.sphere(PEARL, (0, 0, 444), 13, steps=(3, 6))


def sponge_rack(sc):
    """PROP 8, TALL. A net rack with drying sponges, 2.9 m tall, 4.6 m long: two coral A-frames, three straw rails,
    nine sponges (cream, lavender, magenta) hung in two rows that sway, a net draped from the low rail, a basket."""
    g = sc.still()
    for x in (-212, 212):
        tilt_box(g, CORAL, (x, -46, -110), (x, 0, 284), 13, 13)
        tilt_box(g, CORAL, (x, 46, -110), (x, 0, 284), 13, 13)
        g.sphere(PEARL, (x, 0, 292), 14, steps=(3, 5))
    for zc in (252, 172, 92):
        g.box(STRAW, (0, 0, zc), (436, 7, 7))
    for row, (zc, n, ph) in enumerate(((252, 5, 0.1), (172, 4, 0.55))):
        m = sc.part(P.motion(swing=7, pivot=(0, 0, zc), axis=0.0, rate=0.27, phase=ph))
        for k in range(n):
            x = -160 + k * (320.0 / (n - 1))
            c = (CREAM, LAV, CREAM, MAG, LAV)[(k + row) % 5]
            m.box(STRAW, (x, 0, zc - 10), (5, 5, 20))
            m.cyl(c, (x, 0, zc - 54), 19, 36, steps=6, top_r=25)
    for k in range(4):                                                # the net, in the rack's plane
        x = -150 + k * 100
        tilt_box(g, STRAW, (x - 44, 0, 90), (x + 44, 0, 4), 5, 5)
        tilt_box(g, STRAW, (x + 44, 0, 90), (x - 44, 0, 4), 5, 5)
    _basket(g, 150, -6, 0, r=30, h=40, col=CREAM, band=LAV, pearls=False)


def shell_well(sc):
    """PROP 9, TALL. The shell-rimmed well, 3.1 m: a ring of pale rock set with eight scallops, two coral posts, a
    little scallop roof of magenta and cream slats, a windlass with a crank. The shell bucket swings on its rope."""
    g = sc.still()
    g.cyl(ROCK_L, (0, 0, -150), 96, 232, steps=10)
    g.cyl(INK, (0, 0, 78), 76, 6, steps=10)
    for k in range(8):
        p = pol(0, 0, 86, k * 45)
        g.sphere(MAG if k % 2 else CREAM, (p[0], p[1], 86), 22, scale=(1, 1, 0.55), steps=(3, 5))
    for s in (-1, 1):
        rod(g, CORAL, (s * 108, 0, -60), (s * 98, 0, 256), 11, steps=5)
    g.box(CREAM, (0, 0, 256), (236, 10, 10))
    for k in range(5):
        x = -92 + k * 46
        for s in (-1, 1):
            tilt_box(g, MAG if k % 2 == 0 else CREAM, (x, 0, 304 + (k % 2) * 4), (x, s * 88, 250 + (k % 2) * 4), 44, 7)
    g.box(CREAM, (0, 0, 308), (240, 12, 10))
    rod(g, STRAW, (-100, 0, 204), (100, 0, 204), 7, steps=5)
    g.box(CREAM, (118, 0, 190), (8, 8, 36)); g.box(CREAM, (130, 0, 174), (26, 8, 8))
    m = sc.part(P.motion(swing=5, pivot=(0, 0, 204), axis=90.0, rate=0.25, phase=0.35))
    m.box(STRAW, (0, 0, 160), (5, 5, 86))
    m.cyl(LAV, (0, 0, 92), 16, 28, steps=6, top_r=23)
    m.cyl(CREAM, (0, 0, 114), 25, 6, steps=6)


def wash_line(sc):
    """PROP 10, TALL. A washing line between two coral poles, 3.3 m tall, 5.7 m long: a lavender cloth, a square of
    net, a cream cloth with a magenta stripe, a pair of plum swim fins and three starfish on a string, each swaying
    in its own time."""
    g = sc.still()
    for s in (-1, 1):
        rod(g, CORAL, (s * 282, 0, -150), (s * 270, 0, 318), 10, steps=5)
        rod(g, CORAL, (s * 276, 0, 150), (s * 244, 0, 196), 7, steps=4)
        g.sphere(PEARL, (s * 270, 0, 326), 13, steps=(3, 5))
    tilt_box(g, STRAW, (-270, 0, 302), (0, 0, 280), 5, 5)
    tilt_box(g, STRAW, (0, 0, 280), (270, 0, 302), 5, 5)
    r = random.Random(13)

    def hang(x):
        zl = 280 + abs(x) / 270.0 * 22
        return sc.part(P.motion(swing=9, pivot=(x, 0, zl), axis=0.0, rate=0.26 + 0.05 * r.random(), phase=r.random())), zl

    m, zl = hang(-196); m.box(LAV, (-196, 0, zl - 48), (74, 5, 92))
    m, zl = hang(-96)
    for k in range(3):
        m.box(STRAW, (-96 - 30 + k * 30, 0, zl - 42), (4.5, 4.5, 80)); m.box(STRAW, (-96, 0, zl - 14 - k * 28), (66, 4.5, 4.5))
    m, zl = hang(2); m.box(CREAM, (2, 0, zl - 56), (62, 5, 108)); m.box(MAG, (2, -3, zl - 78), (64, 5, 16))
    m, zl = hang(92)
    for s in (-1, 1):
        m.box(ROCK_D, (92 + s * 13, 0, zl - 34), (17, 5, 64)); m.box(ROCK_D, (92 + s * 13, 0, zl - 74), (25, 5, 22))
    m, zl = hang(190)
    m.box(STRAW, (190, 0, zl - 62), (5, 5, 124))
    for k, c in enumerate((MAG, CREAM, MAG)):
        rod(m, c, (190, 3, zl - 30 - k * 40), (190, -4, zl - 30 - k * 40), 17, steps=5)


JOBS = [
    ('SM_S1D1_UrchinHut', urchin_hut, dict(grow=(40, 40, 10), grow_down=(40, 40, 0))),
    ('SM_S1D1_LimpetStore', limpet_store, dict(grow=(10, 30, 0), grow_down=(10, 30, 0))),
    ('SM_S1D1_ConchHouse', conch_house, dict(grow=(40, 40, 10), grow_down=(40, 40, 0))),
    ('SM_S1D1_ScallopMarket', scallop_market, dict(grow=(20, 30, 0), grow_down=(20, 30, 0))),
    ('SM_S1D1_BarnacleTower', barnacle_tower, dict(grow=(50, 50, 10), grow_down=(50, 50, 0))),
    ('SM_S1D1_ClamBandShell', clam_band_shell, dict(grow=(50, 50, 10), grow_down=(50, 50, 0))),
    ('SM_S1D1_BrainBath', brain_bath, dict(grow=(10, 30, 20), grow_down=(10, 30, 0))),
    ('SM_S1D1_GreatConch', great_conch, dict(grow=(200, 200, 20), grow_down=(200, 200, 0))),
    ('SM_S1D1_CowrieStools', cowrie_stools, STILL),
    ('SM_S1D1_BasketStack', basket_stack, STILL),
    ('SM_S1D1_PearlFence', pearl_fence, STILL),
    ('SM_S1D1_ConchPlanters', conch_planters, dict(grow=(12, 12, 4), grow_down=(12, 12, 0))),
    ('SM_S1D1_CoralLamp', coral_lamp, STILL),
    ('SM_S1D1_ShellSign', shell_sign, STILL),
    ('SM_S1D1_ShellChimes', shell_chimes, dict(grow=(20, 50, 4), grow_down=(20, 50, 0))),
    ('SM_S1D1_SpongeRack', sponge_rack, dict(grow=(6, 16, 0), grow_down=(6, 16, 0))),
    ('SM_S1D1_ShellWell', shell_well, dict(grow=(12, 4, 0), grow_down=(12, 4, 0))),
    ('SM_S1D1_WashLine', wash_line, dict(grow=(6, 22, 0), grow_down=(6, 22, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
