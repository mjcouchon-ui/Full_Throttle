"""Theme Park, Stage 1, builder s1plants: what grows in the park. /Game/World/ThemePark/SM_S1PLANTS_*.
Run: import ft_world_park_s1plants as K; K.run()

Thirty-two plant pieces, every one an ft_people.Scene (one section, M_People), each with parts that sway, nod, float or
stir in the vertex shader. Placed by C:\\ftwork\\s1\\L4_s1plants.inl (made by C:\\ftwork\\jobs\\L4s1plants\\place.py).

Two kinds of piece:
  LOW    top under 2 m, bounds under 2.5 m, UNLIT: stands 22 to 24 m out behind the verge (the lily raft 19 to 22 m, on the
         lagoon's valley floor, seen from the rails above).
  TALL   stands 24 m or more out (its whole bounds do). It is a night level, so every TALL piece carries a steady glow
         (lift(): 0.05 on the leaves, more on flowers and bulbs); nothing blinks. The lagoon's three (the bulrush, the
         lotus, the papyrus) are 28 to 31 m tall: the land there lies 26 m under the rails and their heads sway at the
         rails' height.
Each district of the zone plan (L4_plan.md) has its own: Front Gate and Midway the show street's striped tubs and a plane
tree in bulbs; Pirate Cove mangrove, driftwood and hibiscus; the Lagoon water plants; Rocket Land tin tubs; Sweet Street
blossom trees and striped tulips (white and pink only); Crescent Keep cypress, wisteria and a clipped moon; Snowglobe
Square snow-laden firs, frosted birch and holly; the Giant's Garden sunflowers, dandelion clocks, bean poles, a fern,
bluebells and clover.
Not here, by the plan: nothing orange and round, no fruit, no green box with a pale handle, no key, no can with a handle,
nothing round on a stick in Sweet Street, no drifting seeds or petals in the air.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'

# the kit's palette (ft_dress_park.py) and the greens
NAVY, PURPLE, PLUM, MAG, CYAN, AMBER, BULB, CREAM = '#2b2350', '#3a2f6a', '#4a3a78', '#ff4fb8', '#2ee6ff', '#ffd23f', '#ffe28a', '#fff1d6'
RED, WHITE, LAV, MINT, TEAL, WOOD, WOOD_D, PINE = '#ff2d3c', '#fff6ea', '#b9a4ff', '#7fe3c4', '#2fc6c0', '#9a6a44', '#6a4c36', '#2f6b52'
GOLD, SAND, TIN, TIN_D, PEACH, KRAKEN, SPRAY, LAWN, POOL = '#ffc93c', '#efcf9f', '#a8b4d0', '#7f8aa6', '#ffc59e', '#c2408f', '#dff6ff', '#4f9a6a', '#3a5fa8'
LEAF, DEEPG, FERN, STEM, PINKL, ROSE, SILV, SILV_D = '#3fae6a', '#24403a', '#5fae6a', '#6fbf7a', '#ffb3d1', '#ff8fc4', '#c9d6e6', '#9fb0c8'
BLUEB, LILAC, VIOLET, DRIFT, AGAVE, CACT, CACT_D, SEED = '#6f8bff', '#c391d8', '#8f6bff', '#c9b89a', '#6fb8a8', '#4fae8a', '#2f8a6a', '#8a5a3a'
BULBS = (BULB, MAG, CYAN, AMBER)

LOW = dict(grow=(25, 25, 15), grow_down=(25, 25, 0))        # bounds stay under 2.5 m: no shadow, by the track's rule
TALL = dict(grow=(50, 50, 40), grow_down=(50, 50, 0))
HIGH = dict(grow=(110, 110, 60), grow_down=(110, 110, 0))   # the lagoon's 30 m stems: the heads swing 80 cm


def sway(sc, pivot, swing=3.0, axis=20.0, rate=0.35, phase=0.0, bob=0.0):
    """The model for everything that rocks about `pivot` (the foot of a stem, the top of a hanging trail)."""
    return sc.part(P.motion(swing=swing, pivot=pivot, axis=axis, rate=rate, phase=phase, bob=bob))


def blob(m, col, c, r, sq=(1, 1, 1), steps=(4, 6), glow=0.0):
    m.sphere(col, c, r, scale=sq, steps=steps, glow=glow)


def cone(m, col, a, b, r0, r1, steps=6, glow=0.0):
    """A cone from a (radius r0) to b (radius r1): a petal, a bell, a trumpet, a head."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def lerp(a, b, t):
    return (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t)


def add(a, d, k=1.0):
    return (a[0] + d[0] * k, a[1] + d[1] * k, a[2] + d[2] * k)


def ball_dirs(n, lo=-1.0, hi=1.0):
    """n directions spread over a ball (a golden spiral) between heights lo and hi."""
    out = []
    for i in range(n):
        z = lo + (hi - lo) * (i + 0.5) / n
        q = math.sqrt(max(0.0, 1.0 - z * z))
        a = i * 2.39996
        out.append((math.cos(a) * q, math.sin(a) * q, z))
    return out


def star_pts(cx, cz, R, r, n=5):
    return [(cx + math.cos(math.radians(90 + k * 180.0 / n)) * (R if k % 2 == 0 else r), cz + math.sin(math.radians(90 + k * 180.0 / n)) * (R if k % 2 == 0 else r)) for k in range(2 * n)]


def crescent_pts(cx, cz, R=130.0, q=110.0, d=55.0):
    xi = (R * R - q * q + d * d) / (2.0 * d)
    a0 = math.degrees(math.acos(xi / R))
    b0 = math.degrees(math.atan2(math.sqrt(R * R - xi * xi), xi - d))
    pts = []
    for k in range(13):
        a = math.radians(a0 + (360.0 - 2 * a0) * k / 12.0)
        pts.append((cx + math.cos(a) * R, cz + math.sin(a) * R))
    for k in range(1, 9):
        a = math.radians((360.0 - b0) - (360.0 - 2 * b0) * k / 9.0)
        pts.append((cx + d + math.cos(a) * q, cz + math.sin(a) * q))
    return pts


def lift(sc, g):
    """A night level: every colour of a TALL piece gets a steady glow of its own (as the park's people have), so that
    a tree is not a black shape before the lit stalls. Only for pieces that stand 24 m or more out. Nothing blinks."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.05):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def tub(g, x, y, r, top, col, rim, z0=-30.0, steps=8):
    g.cyl(col, (x, y, z0), r * 0.84, top - z0, steps=steps, top_r=r)
    g.cyl(rim, (x, y, top), r + 5, 7, steps=steps)
    return top + 7


# ================================================================================ s1d1 Front Gate and Midway
def topiary_tubs(sc):
    """LOW. Show street: a ball, a cone and a spiral clipped in striped tubs on a striped plinth, star beds between."""
    r = random.Random(401)
    g = sc.still()
    g.box(CREAM, (0, 0, -12), (700, 124, 36))
    for k in range(7):
        g.box(RED, (-300 + k * 100, -62, -8), (50, 5, 22))
    g.box(AMBER, (0, 0, 8), (704, 128, 5))
    for k, x in enumerate((-250, 0, 250)):
        top = tub(g, x, 0, 46, 56, CREAM, AMBER, z0=8)
        for j in range(4):
            g.box(MAG if k == 1 else RED, (x - 27 + j * 18, -41, 34), (8, 5, 38))
        b = sway(sc, (x, 0, top), swing=3, axis=r.uniform(0, 180), rate=r.uniform(0.3, 0.45), phase=r.random())
        b.cyl(WOOD_D, (x, 0, top - 4), 6, 30, steps=5)
        if k == 0:
            blob(b, PINE, (x, 0, 136), 47, steps=(5, 8))
            blob(b, LEAF, (x - 12, -14, 152), 26, steps=(3, 5))
        elif k == 1:
            b.cyl(PINE, (x, 0, 84), 52, 104, steps=8, top_r=6)
            b.cyl(LEAF, (x, 0, 84), 56, 12, steps=8, top_r=50)
        else:
            for j in range(5):
                a = j * 1.4
                b.cyl(LEAF if j % 2 else PINE, (x + math.cos(a) * 9, math.sin(a) * 9, 86 + j * 20), 46 - j * 8, 19, steps=7, top_r=40 - j * 8)
    for x in (-125, 125):
        g.cyl(WOOD_D, (x, 0, 8), 40, 10, steps=5)
        f = sway(sc, (x, 0, 18), swing=6, axis=0, rate=0.5, phase=r.random())
        for j in range(5):
            a = j * 1.2566 + 0.3
            f.box(PINE, (x + math.cos(a) * 22, math.sin(a) * 22, 30), (5, 5, 26))
            blob(f, (MAG, AMBER, WHITE)[j % 3], (x + math.cos(a) * 22, math.sin(a) * 22, 46), 9, steps=(3, 5))


def rose_standards(sc):
    """LOW. Four rose trees on sticks in a cream trough over a clipped hedge, each head nodding."""
    r = random.Random(402)
    g = sc.still()
    g.box(CREAM, (0, 0, -5), (680, 96, 50))
    g.box(AMBER, (0, 0, 22), (688, 104, 5))
    for k in range(8):
        g.box(NAVY, (-297 + k * 85, -48, -2), (40, 5, 22))
    g.box(PINE, (0, 0, 42), (590, 56, 34))
    g.box(LEAF, (0, 0, 60), (570, 44, 6))
    for k, x in enumerate((-255, -85, 85, 255)):
        b = sway(sc, (x, 0, 24), swing=4, axis=r.uniform(0, 180), rate=r.uniform(0.35, 0.5), phase=r.random())
        b.box(WOOD_D, (x, 0, 72), (6, 6, 100))
        blob(b, PINE, (x, 0, 150), 34, steps=(4, 6))
        cols = ((RED, MAG), (WHITE, PINKL), (MAG, RED), (PINKL, WHITE))[k]
        for j in range(7):
            a = j * 0.9 + k
            blob(b, cols[j % 2], (x + math.cos(a) * 30, math.sin(a) * 26, 138 + (j % 4) * 10), 10, steps=(3, 5))
        b.box(LEAF, (x + 5, -4, 106), (20, 6, 10), rot=(0, 30, 20))
    for x in (-340, 340):
        g.cyl(CREAM, (x, 0, -30), 16, 74, steps=6)
        g.cyl(AMBER, (x, 0, 44), 11, 12, steps=6, top_r=3)


def bulb_plane(sc):
    """TALL. A plane tree in a striped round planter, a string of steady bulbs wound up through its crown."""
    r = random.Random(403)
    g = sc.still()
    g.cyl(CREAM, (0, 0, -150), 150, 190, steps=10)
    g.cyl(AMBER, (0, 0, 38), 157, 8, steps=10)
    g.cyl(PINE, (0, 0, 40), 140, 9, steps=10)
    for k in range(10):
        a = k * 0.6283
        g.box(MAG if k % 2 else RED, (math.cos(a) * 148, math.sin(a) * 148, 14), (44, 7, 44), rot=(0, math.degrees(a) + 90, 0))
    g.cyl(WOOD, (0, 0, 0), 36, 430, steps=7, top_r=24)
    for k in range(5):
        a = k * 1.9
        g.box(CREAM, (math.cos(a) * 29, math.sin(a) * 29, 90 + k * 68), (24, 24, 50), rot=(0, math.degrees(a), 0))
    c = sway(sc, (0, 0, 0), swing=1.6, axis=r.uniform(0, 180), rate=0.28, phase=r.random())
    for dx, dy in ((190, 40), (-170, 90), (-40, -190), (60, 170)):
        tilt_box(c, WOOD, (0, 0, 400), (dx, dy, 620), 22, 22)
    for x, y, z, rad, col in ((0, 0, 760, 230, LAWN), (190, 40, 660, 170, PINE), (-170, 90, 680, 180, LEAF), (-40, -190, 650, 170, LAWN),
                              (60, 170, 640, 160, PINE), (0, 0, 880, 150, LEAF)):
        blob(c, col, (x, y, z), rad, (1, 1, 0.8), (5, 8))
    for k in range(14):
        a = k * 0.9
        rr = 300 - k * 9
        c.box(BULBS[k % 4], (math.cos(a) * rr, math.sin(a) * rr, 560 + k * 26), (16, 16, 16), glow=1.3)
    g.cyl(WOOD_D, (0, 0, 46), 76, 8, steps=8)                    # a bench ring round the trunk
    for k in range(4):
        a = k * 1.5708 + 0.4
        g.box(WOOD_D, (math.cos(a) * 66, math.sin(a) * 66, 24), (8, 8, 44))


def topiary_tiers(sc):
    """TALL. A three-tier clipped cake stand in a striped tub, a bulb garland round each tier, a small gold star on top."""
    r = random.Random(404)
    g = sc.still()
    g.cyl(CREAM, (0, 0, -150), 78, 240, steps=8, top_r=92)
    for k in range(8):
        a = k * 0.7854 + 0.39
        g.box(RED, (math.cos(a) * 86, math.sin(a) * 86, 42), (28, 7, 88), rot=(0, math.degrees(a) + 90, 0))
    g.cyl(AMBER, (0, 0, 90), 98, 10, steps=8)
    c = sway(sc, (0, 0, 100), swing=1.5, axis=r.uniform(0, 180), rate=0.3, phase=r.random())
    c.cyl(WOOD_D, (0, 0, 96), 12, 480, steps=6)
    for k, (z, rad) in enumerate(((170, 150), (330, 112), (470, 74))):
        c.cyl(PINE, (0, 0, z), rad, 56, steps=10)
        c.cyl(LAWN, (0, 0, z + 56), rad, 34, steps=10, top_r=rad * 0.55)
        for j in range(8):
            a = j * 0.7854 + k * 0.3
            c.box(BULBS[(j + k) % 4], (math.cos(a) * (rad + 4), math.sin(a) * (rad + 4), z + 30), (13, 13, 13), glow=1.2)
    c.side(GOLD, star_pts(0, 618, 46, 20), -6, 6, glow=0.5)


# ================================================================================ s1d2 Pirate Cove
def mangrove(sc):
    """TALL. A mangrove clump on stilt roots, air roots swinging under the crown, orchids in the forks."""
    r = random.Random(405)
    g = sc.still()
    for k in range(9):
        a = k * 0.698 + r.uniform(-0.15, 0.15)
        R = r.uniform(170, 240)
        knee = (math.cos(a) * R * 0.62, math.sin(a) * R * 0.62, 120 + r.uniform(-20, 30))
        tilt_box(g, WOOD, (math.cos(a) * R, math.sin(a) * R, -120), knee, 16, 16)
        tilt_box(g, WOOD_D, knee, (math.cos(a) * 20, math.sin(a) * 20, 250), 18, 18)
    g.cyl(WOOD, (0, 0, 230), 34, 170, steps=6, top_r=26)
    for a in (0.6, 2.9, 4.6):
        blob(g, MAG, (math.cos(a) * 34, math.sin(a) * 34, 300 + a * 14), 15, (1, 1, 0.8), (3, 5), glow=0.3)
        blob(g, WHITE, (math.cos(a) * 44, math.sin(a) * 44, 300 + a * 14), 7, steps=(3, 5), glow=0.3)
    c = sway(sc, (0, 0, 250), swing=2.2, axis=r.uniform(0, 180), rate=0.3, phase=r.random())
    crown = ((0, 0, 470, 210, PINE), (160, 60, 430, 150, LAWN), (-150, -40, 440, 160, LEAF), (20, -150, 420, 140, LAWN), (-30, 150, 430, 140, PINE), (0, 0, 560, 130, LEAF))
    for x, y, z, rad, col in crown:
        blob(c, col, (x, y, z), rad, (1, 1, 0.55), (5, 8))
    for x, y, z, rad, col in crown[1:5]:
        tilt_box(c, WOOD_D, (0, 0, 380), (x * 0.8, y * 0.8, z - 20), 14, 14)
    for k in range(8):
        a = k * 0.785 + 0.2
        R = r.uniform(120, 230)
        ln = r.uniform(150, 260)
        h = sway(sc, (math.cos(a) * R, math.sin(a) * R, 400), swing=6, axis=math.degrees(a) + 90, rate=r.uniform(0.35, 0.5), phase=r.random())
        h.box(WOOD_D, (math.cos(a) * R, math.sin(a) * R, 400 - ln / 2.0), (6, 6, ln))


def sea_grass_drift(sc):
    """LOW. A bleached driftwood log and its fork on a sand bar, sea grass in tufts, two agaves, shells and a pink sea star."""
    r = random.Random(406)
    g = sc.still()
    for x, rr in ((-200, 170), (60, 190), (250, 130)):
        blob(g, SAND, (x, 0, -6), rr, (1.2, 0.36, 0.14), (3, 8))
    tilt_box(g, DRIFT, (-310, 10, 22), (150, -8, 40), 36, 32)
    tilt_box(g, DRIFT, (60, -4, 40), (200, 20, 150), 16, 16)
    tilt_box(g, DRIFT, (150, 12, 112), (240, -14, 172), 10, 10)
    tilt_box(g, DRIFT, (-120, 4, 34), (-160, -20, 96), 12, 12)
    for x, y in ((-215, -26), (288, 4)):
        s = sway(sc, (x, y, 6), swing=2, axis=r.uniform(0, 180), rate=0.3, phase=r.random())
        for k in range(8):
            a = k * 0.785
            tilt_box(s, AGAVE if k % 2 else TEAL, (x, y, 8), (x + math.cos(a) * 58, y + math.sin(a) * 34, 52 + (k % 2) * 24), 14, 8)
        s.box(AGAVE, (x, y, 52), (9, 9, 88))
    for k, x in enumerate((-280, -150, -50, 30, 200, 250)):
        y = (26, -32, 30, -34, 32, -30)[k]
        s = sway(sc, (x, y, 4), swing=7, axis=r.uniform(-30, 30), rate=r.uniform(0.4, 0.6), phase=r.random())
        for j in range(5):
            h = r.uniform(70, 124)
            s.box((MINT, LEAF, STEM)[j % 3], (x - 16 + j * 8, y, 2 + h / 2.0), (6, 4, h), rot=(0, 0, -14 + j * 7))
    for x, y in ((-90, -40), (110, -44), (300, -36)):
        g.cyl(CREAM, (x, y, 2), 10, 12, steps=6, top_r=2)
    g.plan(PEACH, [(a + 20, b - 40) for a, b in star_pts(0, 0, 22, 9)], 2, 8)
    g.cyl(WOOD, (-20, -42, 2), 26, 9, steps=8)
    g.cyl(WOOD, (-20, -42, 11), 18, 8, steps=8)


def hibiscus(sc):
    """TALL. A big hibiscus bush, red, pink and peach trumpets with gold stamens, three lobes stirring."""
    r = random.Random(407)
    g = sc.still()
    g.cyl(WOOD_D, (0, 0, -120), 60, 150, steps=7, top_r=40)
    lobes = (((0, 0, 190), 150, PINE, 0.9), ((130, 30, 150), 120, LAWN, 0.9), ((-120, -20, 160), 125, LEAF, 0.9), ((10, 20, 285), 90, LAWN, 0.85))
    dirs = ball_dirs(15, -0.15, 0.95)
    n = 0
    for k, (c0, rad, col, sq) in enumerate(lobes):
        m = sway(sc, (0, 0, 30), swing=2.5, axis=40 + k * 55, rate=0.3 + 0.04 * k, phase=r.random())
        blob(m, col, c0, rad, (1, 1, sq), (5, 8))
        for j in range(4 if k else 3):
            d = dirs[n % len(dirs)]
            if k == 1:
                d = (abs(d[0]), d[1], d[2])
            elif k == 2:
                d = (-abs(d[0]), d[1], d[2])
            n += 1
            a = add(c0, (d[0], d[1], d[2] * sq), rad * 0.93)
            fc = (RED, MAG, PEACH, ROSE)[(n + k) % 4]
            cone(m, fc, a, add(a, d, 30), 7, 36, steps=6, glow=0.25)
            tilt_box(m, GOLD, add(a, d, 28), add(a, d, 60), 5, 5, glow=0.3)


# ================================================================================ s1d3 The Lagoon
def giant_bulrush(sc):
    """TALL, 31 m. A clump of five giant bulrushes out of a root hummock: the lagoon's land lies 26 m under the rails
    and the velvet heads sway at the rails' height. Lily pads and short reeds at the foot."""
    r = random.Random(408)
    g = sc.still()
    g.cyl(PINE, (0, 0, -300), 130, 320, steps=7, top_r=96)
    for k in range(3):
        g.cyl(LEAF if k else LAWN, (r.uniform(-170, 170), r.uniform(-150, 150), -40), r.uniform(70, 105), 48, steps=8)
    for k in range(5):
        a = k * 1.257 + 0.4
        R = 34 + k * 13
        bx, by = math.cos(a) * R, math.sin(a) * R
        H = (3080, 2760, 2950, 2620, 2850)[k]
        top = (bx + math.cos(a) * H * 0.035, by + math.sin(a) * H * 0.035, H)
        foot = (bx, by, -40)
        s = sway(sc, (bx, by, 0), swing=r.uniform(1.0, 1.5), axis=r.uniform(0, 180), rate=r.uniform(0.18, 0.28), phase=r.random())
        t0, t1 = 1.0 - 420.0 / H, 1.0 - 90.0 / H
        rod(s, STEM, foot, lerp(foot, top, t0 + 0.01), 16, steps=5)
        cone(s, WOOD_D, lerp(foot, top, t0 - 0.012), lerp(foot, top, t0), 16, 42, steps=7)
        rod(s, WOOD_D, lerp(foot, top, t0), lerp(foot, top, t1), 42, steps=7)
        cone(s, WOOD_D, lerp(foot, top, t1), lerp(foot, top, t1 + 0.012), 42, 12, steps=7)
        cone(s, WOOD, lerp(foot, top, t1 + 0.01), top, 10, 4, steps=4)
        for da, f in ((1.0, 0.44), (-1.2, 0.3)):
            tilt_box(s, LEAF if da > 0 else LAWN, (bx, by, 0), (bx + math.cos(a + da) * 170, by + math.sin(a + da) * 170, H * f), 40, 7)
    s = sway(sc, (0, 0, 0), swing=4, axis=60, rate=0.4, phase=r.random())
    for k in range(7):
        a = k * 0.9
        h = r.uniform(180, 330)
        s.box(STEM if k % 2 else MINT, (math.cos(a) * 110, math.sin(a) * 110, h / 2.0), (8, 6, h), rot=(0, math.degrees(a), 8))


def lotus_tower(sc):
    """TALL, 30 m. One giant lotus on a long winding stem, three lily pads up its length, the flower softly lit at the
    rails' height; pads and a bud at its foot."""
    r = random.Random(409)
    g = sc.still()
    g.cyl(PINE, (0, 0, -300), 110, 320, steps=7, top_r=70)
    for x, y, rad in ((-150, -60, 120), (140, 70, 100), (30, -170, 90)):
        g.cyl(LAWN, (x, y, -40), rad, 46, steps=9)
        g.cyl(LEAF, (x, y, 6), rad * 0.8, 5, steps=9)
    b = sway(sc, (-200, 120, 0), swing=5, axis=30, rate=0.4, phase=r.random())
    b.box(STEM, (-200, 120, 80), (8, 8, 170))
    cone(b, ROSE, (-200, 120, 160), (-200, 120, 250), 26, 5, steps=6)
    m = sway(sc, (0, 0, 0), swing=1.1, axis=r.uniform(0, 180), rate=0.2, phase=r.random())
    pts = ((0, 0, -60), (40, 10, 900), (-30, -10, 1800), (12, 0, 2600), (0, 0, 2885))
    for k in range(4):
        rod(m, STEM, pts[k], add(pts[k + 1], (0, 0, 1), 12), 20 - k * 2, steps=6)
    for k, (z, sx) in enumerate(((900, 1), (1720, -1), (2380, 1))):
        px = pts[k + 1][0] + sx * 170 if k < 2 else 12 + sx * 160
        py = pts[k + 1][1]
        zz = z if k < 2 else 2380
        tilt_box(m, STEM, (px - sx * 170, py, zz - 40), (px, py, zz), 12, 12)
        m.cyl(LAWN, (px, py, zz), 190 - k * 20, 12, steps=9)
        m.cyl(LEAF, (px, py, zz + 12), (190 - k * 20) * 0.8, 5, steps=9)
        m.cyl(MINT, (px, py, zz - 5), (190 - k * 20) * 0.6, 5, steps=9)
    c = (0, 0, 2890)
    m.cyl(GOLD, c, 44, 40, steps=8, glow=0.6)
    m.cyl(STEM, (0, 0, 2860), 30, 30, steps=8, top_r=46)
    for k in range(8):
        a = k * 0.785
        cone(m, ROSE, add(c, (math.cos(a) * 30, math.sin(a) * 30, 0)), add(c, (math.cos(a) * 205, math.sin(a) * 205, 96)), 40, 6, steps=4, glow=0.35)
    for k in range(6):
        a = k * 1.047 + 0.5
        cone(m, PINKL, add(c, (math.cos(a) * 24, math.sin(a) * 24, 10)), add(c, (math.cos(a) * 100, math.sin(a) * 100, 180)), 36, 6, steps=4, glow=0.4)


def papyrus(sc):
    """TALL, 28 m. Three papyrus stems, three-cornered, each ending in a great umbrella of rays that turns with the stem."""
    r = random.Random(410)
    g = sc.still()
    g.cyl(PINE, (0, 0, -300), 100, 320, steps=6, top_r=64)
    for k in range(5):
        a = k * 1.257
        cone(g, WOOD, (math.cos(a) * 50, math.sin(a) * 50, 10), (math.cos(a) * 64, math.sin(a) * 64, 150), 20, 6, steps=4)
    for k, (bx, by, H) in enumerate(((-40, 10, 2840), (50, 30, 2560), (0, -50, 2700))):
        lean = (bx * 2.0, by * 2.0)
        top = (bx + lean[0], by + lean[1], H)
        s = sway(sc, (bx, by, 0), swing=r.uniform(1.0, 1.4), axis=r.uniform(0, 180), rate=r.uniform(0.18, 0.26), phase=r.random())
        rod(s, STEM, (bx, by, -40), top, 15, steps=3)
        blob(s, LAWN, top, 60, steps=(4, 6))
        for j, d in enumerate(ball_dirs(16, -0.45, 1.0)):
            tilt_box(s, (LEAF, MINT, STEM)[j % 3], top, add(top, d, 250), 10, 10)


def lily_raft(sc):
    """LOW, unlit. A raft of giant lily pads, each floating on its own, three water lilies open on them and a bud.
    For the lagoon's valley floor, 19 to 22 m out: seen from the rails above."""
    r = random.Random(411)
    pads = ((-230, -20, 105), (-60, 60, 90), (40, -70, 80), (190, 10, 110), (-150, -95, 58), (270, -90, 55), (110, 95, 62))
    g = sc.still()
    for k, (x, y, rad) in enumerate(pads):
        g.cyl(PINE, (x, y, -120), rad * 0.3, 120, steps=5)
        p = sc.part(P.motion(bob=-3.0, rate=r.uniform(0.2, 0.3), phase=r.random(), pivot=(x, y, 0)))
        p.cyl(LAWN if k % 2 else LEAF, (x, y, -6), rad, 12, steps=9)
        p.cyl(MINT if k % 2 else LAWN, (x, y, 6), rad * 0.8, 5, steps=9)
        if k in (0, 3, 6):
            c = (x + 14, y - 8, 9)
            for j in range(8):
                a = j * 0.785
                cone(p, WHITE, c, add(c, (math.cos(a) * 48, math.sin(a) * 48, 22)), 17, 3, steps=4)
            for j in range(5):
                a = j * 1.257 + 0.4
                cone(p, PINKL, c, add(c, (math.cos(a) * 22, math.sin(a) * 22, 44)), 14, 3, steps=4)
            p.cyl(AMBER, c, 9, 16, steps=6)
    b = sway(sc, (-20, -10, 0), swing=6, axis=20, rate=0.45, phase=r.random())
    b.box(STEM, (-20, -10, 24), (6, 6, 56))
    cone(b, PINKL, (-20, -10, 44), (-20, -10, 80), 13, 3, steps=6)


def willow(sc):
    """TALL. A weeping willow, sixteen curtains of leaves hanging from its crown and each swinging on its own."""
    r = random.Random(412)
    g = sc.still()
    g.cyl(WOOD_D, (0, 0, -200), 62, 420, steps=7, top_r=44)
    tilt_box(g, WOOD_D, (0, 0, 200), (40, -10, 560), 60, 54)
    for dx, dy in ((200, 60), (-190, 30), (20, -200), (-40, 190)):
        tilt_box(g, WOOD_D, (36, -8, 520), (dx, dy, 800), 26, 26)
    c = sway(sc, (0, 0, 0), swing=1.0, axis=r.uniform(0, 180), rate=0.25, phase=r.random())
    for x, y, z, rad, col in ((20, 0, 900, 300, LAWN), (210, 60, 840, 200, LEAF), (-200, 30, 850, 210, PINE), (20, -210, 830, 200, LEAF), (-40, 200, 840, 200, LAWN)):
        blob(c, col, (x, y, z), rad, (1, 1, 0.6), (5, 8))
    for k in range(16):
        a = k * 0.3927 + 0.1
        R = 330 + (k % 3) * 34
        x, y = 20 + math.cos(a) * R, math.sin(a) * R
        ln = r.uniform(480, 660)
        zt = 860
        h = sway(sc, (x, y, zt), swing=r.uniform(4.5, 7), axis=math.degrees(a) + 90 * (k % 2), rate=r.uniform(0.25, 0.4), phase=r.random())
        h.box(MINT if k % 2 else STEM, (x, y, zt - ln / 2.0), (34, 9, ln), rot=(0, math.degrees(a) + 90, 0))
        h.box(LAWN, (x + math.cos(a) * 9, y + math.sin(a) * 9, zt - ln * 0.4), (20, 7, ln * 0.8), rot=(0, math.degrees(a) + 90, 0))


# ================================================================================ s1d4 Rocket Land and Funhouse
def _tin_tub(g, x, r=52.0, top=72.0):
    g.cyl(TIN, (x, 0, 2), r, top - 2, steps=8)
    g.cyl(TIN_D, (x, 0, 18), r + 2, 8, steps=8)
    g.cyl(TIN_D, (x, 0, top - 14), r + 2, 8, steps=8)
    g.cyl(PURPLE, (x, 0, top), r - 5, 5, steps=8)
    for j in range(5):
        g.box(SILV, (x - 32 + j * 16, -r * 0.93, 38), (5, 5, 5))


def silver_spirals(sc):
    """TALL. Tin-toy town: a silver spiral, a stack of cubes and three balls clipped in riveted tin tubs with red fins
    and a lit porthole each."""
    r = random.Random(413)
    g = sc.still()
    g.box(TIN_D, (0, 0, -14), (660, 140, 32))
    for k in range(11):
        g.box(SILV, (-300 + k * 60, -71, -10), (7, 4, 7))
    for k, x in enumerate((-230, 0, 230)):
        _tin_tub(g, x)
        g.box(GOLD, (x, -53, 54), (30, 4, 30))
        g.box(CYAN, (x, -55, 54), (20, 4, 20), glow=0.5)
        for sx in (-1, 1):
            g.side(RED, [(x + sx * 50, 2), (x + sx * 88, 2), (x + sx * 50, 64)], -5, 5)
        b = sway(sc, (x, 0, 78), swing=2, axis=r.uniform(0, 180), rate=r.uniform(0.28, 0.4), phase=r.random())
        b.cyl(WOOD_D, (x, 0, 74), 8, 62, steps=5)
        if k == 0:
            for j in range(8):
                a = j * 0.9
                rad = 72 - j * 7
                b.cyl(SILV if j % 2 else SILV_D, (x + math.cos(a) * 14, math.sin(a) * 14, 130 + j * 36), rad, 30, steps=8, top_r=rad * 0.8)
        elif k == 1:
            for j, (s, zc) in enumerate(((124, 196), (94, 306), (66, 386))):
                b.box(SILV if j != 1 else SILV_D, (x, 0, zc), (s, s, s), rot=(0, 45 * j, 0))
            blob(b, SILV_D, (x, 0, 438), 22, steps=(3, 6))
        else:
            for j, (rad, zc) in enumerate(((72, 204), (54, 316), (38, 396))):
                blob(b, SILV_D if j == 1 else SILV, (x, 0, zc), rad, steps=(5, 8))


def cactus_tins(sc):
    """LOW. Four cactus balls and columns in riveted tin tubs on a tin tray, a pale flower on each."""
    r = random.Random(414)
    g = sc.still()
    g.box(TIN_D, (0, 0, -14), (660, 120, 32))
    for k in range(11):
        g.box(SILV, (-300 + k * 60, -61, -10), (7, 4, 7))
    for k, x in enumerate((-250, -84, 84, 250)):
        _tin_tub(g, x, 40.0, 52.0)
        b = sway(sc, (x, 0, 54), swing=2.5, axis=r.uniform(0, 180), rate=r.uniform(0.3, 0.45), phase=r.random())
        fl = (WHITE, LAV, PINKL, WHITE)[k]
        if k == 0:
            blob(b, CACT, (x, 0, 98), 45, (1, 1, 0.95), (4, 8))
            ft = 140
        elif k == 1:
            b.cyl(CACT_D, (x, 0, 52), 24, 104, steps=7, top_r=20)
            blob(b, CACT_D, (x, 0, 156), 20, steps=(3, 7))
            tilt_box(b, CACT, (x + 20, 0, 96), (x + 48, 0, 104), 16, 16)
            b.box(CACT, (x + 48, 0, 124), (16, 16, 50))
            tilt_box(b, CACT, (x - 20, 0, 116), (x - 44, 0, 122), 14, 14)
            b.box(CACT, (x - 44, 0, 138), (14, 14, 40))
            ft = 174
        elif k == 2:
            blob(b, CACT, (x, 0, 90), 38, steps=(4, 8))
            blob(b, CACT_D, (x + 8, 0, 142), 26, steps=(4, 7))
            ft = 166
        else:
            blob(b, CACT_D, (x, 0, 96), 42, (1, 0.4, 1.05), (4, 7))
            blob(b, CACT, (x + 30, 0, 146), 28, (1, 0.4, 1), (3, 6))
            blob(b, CACT, (x - 30, 0, 140), 24, (1, 0.4, 1), (3, 6))
            ft = 172
        for j in range(5):
            a = j * 1.257
            b.box(fl, (x + (30 if k == 3 else 0) + math.cos(a) * 9, math.sin(a) * 9, ft + 3), (10, 10, 5), rot=(0, math.degrees(a), 0))
        b.box(CREAM, (x + (30 if k == 3 else 0), 0, ft + 6), (6, 6, 6))


def moon_vine(sc):
    """TALL. A moon-flower vine wound on a tin hoop over a riveted drum: pale blooms softly lit, tendrils hanging and swinging."""
    r = random.Random(415)
    g = sc.still()
    g.cyl(TIN, (0, 0, -150), 64, 210, steps=10)
    g.cyl(TIN_D, (0, 0, 40), 68, 10, steps=10)
    g.box(TIN_D, (0, 0, 70), (20, 20, 40))
    C = (0, 0, 340)
    R = 250.0
    for k in range(14):
        a0, a1 = k * 0.4488, (k + 1) * 0.4488
        tilt_box(g, TIN_D if k % 2 else TIN, (math.cos(a0) * R, 0, 340 + math.sin(a0) * R), (math.cos(a1) * R, 0, 340 + math.sin(a1) * R), 18, 18)
    v = sway(sc, C, swing=2.0, axis=90, rate=0.22, phase=r.random())
    for k in range(14):
        a = k * 0.4488 + 0.2
        v.box(PINE if k % 2 else LAWN, (math.cos(a) * (R + 4), -13, 340 + math.sin(a) * (R + 4)), (62, 10, 28), rot=(-math.degrees(a) + 90, 0, 0))
    for k in range(9):
        a = k * 0.698 + 0.5
        p = (math.cos(a) * (R - 6), -24, 340 + math.sin(a) * (R - 6))
        blob(v, WHITE, p, 28, (1, 0.45, 1), (3, 6), glow=0.55)
        blob(v, BULB, (p[0], p[1] - 10, p[2]), 9, steps=(3, 5), glow=0.8)
    for k in range(6):
        a = math.radians((200, 235, 270, 305, 340, 20)[k])
        top = (math.cos(a) * (R - 16), -6, 340 + math.sin(a) * (R - 16))
        ln = r.uniform(90, 170)
        t = sway(sc, top, swing=7, axis=90, rate=r.uniform(0.35, 0.5), phase=r.random())
        t.box(PINE, (top[0], top[1], top[2] - ln / 2.0), (7, 7, ln))
        t.box(LAWN, (top[0] + 10, top[1], top[2] - ln * 0.5), (22, 6, 14))
        blob(t, SPRAY, (top[0], top[1] - 6, top[2] - ln), 18, (1, 0.5, 1), (3, 6), glow=0.5)


# ================================================================================ s1d5 Sweet Street
def blossom_swirl(sc):
    """TALL. A tree clipped as a soft-ice swirl in a chocolate tub with an icing rim, a band of pink blossom wound up it."""
    r = random.Random(416)
    g = sc.still()
    g.cyl(WOOD_D, (0, 0, -150), 100, 250, steps=9, top_r=116)
    g.cyl(WHITE, (0, 0, 96), 122, 16, steps=9)
    for k in range(9):
        a = k * 0.698
        g.box(WHITE, (math.cos(a) * 116, math.sin(a) * 116, 96 - (20 + (k % 3) * 14) / 2.0), (26, 12, 20 + (k % 3) * 14), rot=(0, math.degrees(a) + 90, 0))
    g.cyl(WOOD_D, (0, 0, 100), 32, 170, steps=6)
    for k in range(6):
        g.box(PINKL, (r.uniform(-90, 90), r.uniform(-90, 90), 113), (12, 9, 5), rot=(0, r.uniform(0, 90), 0))
    c = sway(sc, (0, 0, 100), swing=1.8, axis=r.uniform(0, 180), rate=0.28, phase=r.random())
    for k in range(9):
        a = k * 0.8
        rad = 235 - k * 24
        z = 300 + k * 62
        blob(c, LAWN if k % 2 else LEAF, (math.cos(a) * 34, math.sin(a) * 34, z), rad, (1, 1, 0.42), (5, 8))
    c.cyl(LEAF, (math.cos(7.2) * 20, math.sin(7.2) * 20, 800), 30, 70, steps=6, top_r=4)
    for k in range(18):
        a = k * 0.72
        f = k / 17.0
        rad = 232 - f * 196
        blob(c, PINKL if k % 3 else WHITE, (math.cos(a) * rad, math.sin(a) * rad, 300 + f * 500), 20 - f * 6, steps=(3, 5), glow=0.3)


def blossom_cones(sc):
    """TALL. Three clipped cones in a peach planter iced white, pink blossom garlands wound round them, marshmallow bushes between."""
    r = random.Random(417)
    g = sc.still()
    g.box(PEACH, (0, 0, -40), (700, 150, 220))
    g.box(WHITE, (0, 0, 76), (712, 162, 14))
    for k in range(10):
        g.box(WHITE, (-315 + k * 70, -80, 62 - (k % 3) * 6), (26, 8, 20 + (k % 3) * 12))
        g.box(KRAKEN, (-280 + k * 70, -76, 20), (16, 5, 70))
    for k, x in enumerate((-240, 0, 240)):
        c = sway(sc, (x, 0, 84), swing=2.2, axis=r.uniform(0, 180), rate=r.uniform(0.28, 0.4), phase=r.random())
        c.cyl(WOOD_D, (x, 0, 80), 14, 90, steps=5)
        H = (360, 420, 340)[k]
        c.cyl(LAWN if k % 2 else LEAF, (x, 0, 150), 92, H, steps=8, top_r=8)
        c.cyl(PINE, (x, 0, 150), 96, 14, steps=8, top_r=88)
        for j in range(9):
            a = j * 1.0 + k
            f = j / 9.0
            rad = 92 - f * 78
            blob(c, PINKL if j % 2 else WHITE, (x + math.cos(a) * rad, math.sin(a) * rad, 170 + f * H * 0.92), 13, steps=(3, 5), glow=0.3)
    for x, col, rad in ((-120, WHITE, 52), (120, PINKL, 50)):
        blob(g, col, (x, 0, 83 + rad * 0.55), rad, (1, 1, 0.7), (4, 8))
        blob(g, PINKL if col == WHITE else WHITE, (x + 34, -30, 100), 24, (1, 1, 0.8), (3, 6))


def candy_tulips(sc):
    """LOW. Candy-striped tulips, white and pink, in a chocolate kerb with an icing top, marshmallow bushes between the clumps."""
    r = random.Random(418)
    g = sc.still()
    g.box(WOOD_D, (0, 0, -8), (680, 130, 44))
    g.box(WHITE, (0, 0, 17), (672, 122, 6))
    for k in range(9):
        g.box(WHITE, (-300 + k * 75, -66, 8 - (k % 2) * 5), (22, 5, 14 + (k % 2) * 10))
    for x, col, rad in ((-300, WHITE, 38), (-100, PINKL, 46), (100, WHITE, 44), (300, PINKL, 36)):
        blob(g, col, (x, 6, 20 + rad * 0.6), rad, (1, 1, 0.72), (4, 8))
    for k, x in enumerate((-200, 0, 200)):
        for row in range(2):
            y = -30 + row * 44
            f = sway(sc, (x, y, 20), swing=6, axis=0, rate=0.45 + 0.07 * row, phase=r.random())
            for j in range(3 if row else 2):
                tx = x - 44 + j * 44 + (22 if not row else 0) + r.uniform(-6, 6)
                h = r.uniform(50, 72)
                cup, st = ((PINKL, WHITE), (WHITE, ROSE))[(j + row + k) % 2]
                f.box(PINE, (tx, y, 20 + h / 2.0), (5, 5, h))
                f.box(LEAF, (tx + 7, y, 40), (6, 14, 30), rot=(0, 0, 16))
                f.cyl(cup, (tx, y, 18 + h), 9, 24, steps=6, top_r=13)
                f.box(st, (tx, y, 31 + h), (5, 28, 22))
                f.box(st, (tx, y, 31 + h), (28, 5, 22))


# ================================================================================ s1d6 Crescent Keep
def cypress(sc):
    """TALL. Three tall thin cypresses in a row over a bed of lavender behind a little battlement."""
    r = random.Random(419)
    g = sc.still()
    g.box(CREAM, (0, 0, -50), (640, 150, 200))
    for k in range(9):
        g.box(CREAM, (-288 + k * 72, -70, 60), (40, 10, 22))
    g.box(PLUM, (0, 0, 50), (620, 130, 6))
    for k, x in enumerate((-210, 0, 210)):
        H = (1080, 930, 1020)[k]
        c = sway(sc, (x, 20, 50), swing=1.6, axis=r.uniform(0, 180), rate=r.uniform(0.22, 0.32), phase=r.random())
        c.cyl(WOOD_D, (x, 20, 40), 14, 60, steps=5)
        c.cyl(DEEPG, (x, 20, 90), 44, 230, steps=8, top_r=72)
        c.cyl(PINE if k % 2 else DEEPG, (x, 20, 320), 72, H - 320, steps=8, top_r=6)
        c.cyl(PINE if not k % 2 else DEEPG, (x + 14, 10, 300), 40, H * 0.5, steps=5, top_r=5)
    for k in range(6):
        x = -262 + k * 105
        f = sway(sc, (x, -34, 52), swing=7, axis=0, rate=r.uniform(0.4, 0.55), phase=r.random())
        blob(f, PINE, (x, -34, 60), 30, (1, 1, 0.6), (3, 6))
        for j in range(4):
            f.box(VIOLET, (x - 21 + j * 14, -34, 92), (6, 6, 44), rot=(0, 0, -12 + j * 8))
            f.box(LAV, (x - 24 + j * 16, -34, 116), (8, 8, 14))


def topiary_moon(sc):
    """TALL. A crescent moon and a star clipped on stems in cream urns with gold bands, a clipped ball between."""
    r = random.Random(420)
    g = sc.still()
    for x in (-150, 150):
        g.cyl(CREAM, (x, 0, -150), 40, 190, steps=8, top_r=30)
        g.cyl(CREAM, (x, 0, 40), 30, 60, steps=8, top_r=52)
        g.cyl(GOLD, (x, 0, 62), 44, 8, steps=8)
        g.cyl(CREAM, (x, 0, 100), 56, 8, steps=8)
    a = sway(sc, (-150, 0, 104), swing=2.5, axis=0, rate=0.3, phase=r.random())
    a.cyl(WOOD_D, (-150, 0, 100), 9, 170, steps=5)
    a.side(PINE, crescent_pts(-170, 390), -28, 28)
    a.side(LAWN, crescent_pts(-170, 390, 118, 104, 50), -32, -28)
    b = sway(sc, (150, 0, 104), swing=2.5, axis=0, rate=0.34, phase=r.random())
    b.cyl(WOOD_D, (150, 0, 100), 9, 190, steps=5)
    b.side(PINE, star_pts(150, 390, 124, 52), -28, 28)
    b.side(LAWN, star_pts(150, 390, 96, 40), -32, -28)
    g.box(CREAM, (0, 0, -40), (150, 90, 110))
    g.box(GOLD, (0, 0, 17), (158, 98, 5))
    c = sway(sc, (0, 0, 20), swing=3, axis=90, rate=0.4, phase=r.random())
    blob(c, PINE, (0, 0, 76), 58, steps=(5, 8))
    blob(c, LAWN, (-14, -18, 96), 30, steps=(3, 6))


def wisteria_arch(sc):
    """TALL. A pointed arch on two lavender piers, wisteria over it: fourteen racemes hang and swing, softly lit."""
    r = random.Random(421)
    g = sc.still()
    for sx in (-1, 1):
        g.box(LAV, (sx * 280, 0, 105), (66, 66, 510))
        g.box(CREAM, (sx * 280, 0, 366), (82, 82, 14))
        g.box(CREAM, (sx * 280, 0, -20), (86, 86, 120))
        tilt_box(g, CREAM, (sx * 280, 0, 366), (0, 0, 536), 36, 36)
        tilt_box(g, LAV, (sx * 250, 0, 330), (0, 0, 470), 18, 18)
        for k in range(5):
            tilt_box(g, WOOD, (sx * (318 - (k % 2) * 8), -34 + (k % 2) * 12, k * 76), (sx * (310 + (k % 2) * 8), -22 - (k % 2) * 12, 80 + k * 76), 10, 10)
    g.box(CREAM, (0, 30, 22), (300, 50, 10))
    g.box(CREAM, (-120, 30, -40), (16, 44, 120))
    g.box(CREAM, (120, 30, -40), (16, 44, 120))
    c = sway(sc, (0, 0, 536), swing=1.2, axis=90, rate=0.25, phase=r.random())
    for k in range(7):
        x = -270 + k * 90
        z = 376 + (1 - abs(x) / 280.0) * 170
        blob(c, (PINE, LAWN, LEAF)[k % 3], (x, -4, z + 16), 62, (1.2, 0.8, 0.55), (4, 7))
    for k in range(14):
        x = -292 + k * 45
        zt = 372 + (1 - min(abs(x), 280) / 280.0) * 170 - 4
        ln = r.uniform(90, 170)
        y = -22 + (k % 3) * 20
        h = sway(sc, (x, y, zt), swing=r.uniform(5, 8), axis=90 if k % 2 else 0, rate=r.uniform(0.3, 0.48), phase=r.random())
        h.cyl((LILAC, VIOLET, LAV)[k % 3], (x, y, zt - ln), 4, ln, steps=5, top_r=21, glow=0.3)
        h.cyl(PINE, (x, y, zt - 6), 14, 12, steps=5)


def lavender_bed(sc):
    """LOW. A bed of lavender in clumps behind a little cream battlement, a stone crescent set in the middle."""
    r = random.Random(422)
    g = sc.still()
    g.box(CREAM, (0, 0, -6), (690, 130, 48))
    for k in range(9):
        g.box(CREAM, (-312 + k * 78, -60, 29), (42, 10, 22))
    g.box(PLUM, (0, 4, 18), (670, 104, 5))
    g.side(CREAM, crescent_pts(0, 62, 40, 34, 17), 20, 32)
    g.box(CREAM, (-8, 26, 26), (30, 16, 14))
    for k in range(8):
        x = -290 + k * 83
        y = -14 if k % 2 else 22
        f = sway(sc, (x, y, 20), swing=7, axis=0, rate=r.uniform(0.4, 0.55), phase=r.random())
        blob(f, PINE, (x, y, 32), 34, (1, 1, 0.6), (3, 6))
        for j in range(5):
            f.box(VIOLET, (x - 24 + j * 12, y, 70), (6, 6, 50), rot=(0, 0, -14 + j * 7))
            f.box(LAV, (x - 28 + j * 14, y, 98), (8, 8, 14))


# ================================================================================ s1d7 Snowglobe Square
def _fir(sc, r, x, y, H, tiers, rad, parts=1, lumps=3):
    """A snow-laden fir: green skirts under thick white caps. The upper tiers swing further than the lower ones."""
    g = sc.still()
    g.cyl(WOOD_D, (x, y, -200), rad * 0.13, 200 + H * 0.2, steps=6)
    axis, rate, ph = r.uniform(0, 180), r.uniform(0.2, 0.3), r.random()
    step = (H * 0.84) / tiers
    th = step * 1.45
    for k in range(tiers):
        f = k / float(tiers)
        m = sway(sc, (x, y, 0), swing=0.8 + 1.1 * (k * parts // tiers), axis=axis, rate=rate, phase=ph)
        z0 = H * 0.14 + k * step
        rr = rad * (1.0 - 0.72 * f)
        m.cyl(DEEPG if k % 2 else PINE, (x, y, z0), rr, th, steps=8, top_r=rr * 0.3)
        m.cyl(WHITE, (x, y, z0 + th * 0.36), rr * 0.82, th * 0.68, steps=8, top_r=rr * 0.3)
        for j in range(lumps):
            a = j * 2.094 + k
            blob(m, WHITE, (x + math.cos(a) * rr * 0.78, y + math.sin(a) * rr * 0.78, z0 + th * 0.3), rr * 0.16, (1, 1, 0.7), (3, 5))
        if k == 0:
            for j in range(5):
                a = j * 1.257 + 0.3
                p = (x + math.cos(a) * rr * 0.9, y + math.sin(a) * rr * 0.9, z0 + 4)
                cone(m, SPRAY, p, add(p, (0, 0, -1), rr * 0.22), rr * 0.035 + 4, 2, steps=4, glow=0.25)


def snow_fir_tall(sc):
    """TALL, 13 m. One great snow-laden fir, icicles under its lowest skirt, a drift at its foot."""
    r = random.Random(423)
    _fir(sc, r, 0, 0, 1300, 6, 280, parts=3)
    g = sc.still()
    blob(g, WHITE, (0, 0, -10), 250, (1, 1, 0.2), (4, 8))
    blob(g, SPRAY, (150, -60, 0), 120, (1, 1, 0.22), (3, 7))


def snow_fir_group(sc):
    """TALL. Three snow firs of three sizes in one drift."""
    r = random.Random(424)
    _fir(sc, r, -210, 20, 740, 4, 190, parts=2, lumps=2)
    _fir(sc, r, 70, -20, 500, 3, 140, lumps=2)
    _fir(sc, r, 270, 40, 340, 3, 100, lumps=1)
    g = sc.still()
    blob(g, WHITE, (0, 10, -14), 330, (1.1, 0.42, 0.18), (4, 9))
    blob(g, WHITE, (-210, 20, 0), 150, (1, 0.8, 0.24), (3, 7))
    blob(g, SPRAY, (190, -20, 0), 130, (1, 0.7, 0.2), (3, 7))


def frost_birch(sc):
    """TALL. Two frosted birches, white trunks, bare branches hung with ice leaves that glint (a steady glow) and stir."""
    r = random.Random(425)
    g = sc.still()
    blob(g, WHITE, (0, 0, -10), 210, (1, 0.8, 0.2), (4, 8))
    for k, (foot, top) in enumerate((((-50, 0, -200), (-110, 10, 840)), ((60, 0, -200), (130, -20, 700)))):
        tilt_box(g, WHITE, foot, top, 30, 30)
        for j in range(5):
            p = lerp(foot, top, 0.3 + j * 0.13)
            g.box(PLUM, (p[0], p[1] - 14, p[2]), (22, 5, 9))
        for j in range(5):
            t = 0.5 + j * 0.115
            p = lerp(foot, top, t)
            a = j * 2.2 + k * 1.3
            ln = 250 - j * 26
            e = add(p, (math.cos(a) * ln * 0.75, math.sin(a) * ln * 0.75, ln * 0.6))
            b = sway(sc, p, swing=4, axis=math.degrees(a) + 90, rate=r.uniform(0.3, 0.45), phase=r.random())
            tilt_box(b, SPRAY, p, e, 12, 12)
            blob(b, WHITE, lerp(p, e, 0.25), 15, (1, 1, 0.6), (3, 5))
            for i in range(3):
                d = (math.cos(a + (i - 1) * 0.8), math.sin(a + (i - 1) * 0.8), 0.5 + 0.2 * i)
                q = add(e, d, 70)
                tilt_box(b, SPRAY, e, q, 7, 7)
                b.box(CYAN if (i + j) % 2 else SPRAY, q, (17, 17, 17), rot=(45, 45, 0), glow=0.4)


def holly_low(sc):
    """LOW. Three holly bushes under snow caps, red berries, ice flowers between them, in a snowy kerb with a blue timber edge."""
    r = random.Random(426)
    g = sc.still()
    g.box(WHITE, (0, 0, -6), (680, 130, 48))
    g.box(POOL, (0, -64, -4), (684, 6, 30))
    for k in range(6):
        blob(g, WHITE, (-290 + k * 116, 8, 18), 44, (1.3, 1, 0.2), (3, 6))
    for k, x in enumerate((-230, 0, 230)):
        b = sway(sc, (x, 0, 20), swing=2, axis=r.uniform(0, 180), rate=r.uniform(0.3, 0.42), phase=r.random())
        blob(b, DEEPG, (x, 0, 82), 58, steps=(4, 6))
        blob(b, PINE, (x + 20, -10, 128), 40, steps=(4, 6))
        blob(b, WHITE, (x + 10, -4, 166), 42, (1, 1, 0.36), (3, 7))
        blob(b, WHITE, (x - 34, 0, 122), 26, (1, 1, 0.4), (3, 6))
        for j in range(6):
            a = j * 1.05 + k
            blob(b, RED, (x + math.cos(a) * 50, -30 - abs(math.sin(a)) * 22, 70 + (j % 3) * 22), 7, steps=(3, 5))
    for x in (-115, 115):
        for j in range(3):
            px = x - 30 + j * 30
            f = sway(sc, (px, -10, 20), swing=6, axis=0, rate=r.uniform(0.4, 0.55), phase=r.random())
            h = 44 + j * 14
            f.box(SPRAY, (px, -10, 20 + h / 2.0), (5, 5, h))
            for i in range(3):
                f.box(CYAN if i == 0 else SPRAY, (px, -10, 22 + h), (38, 5, 5), rot=(i * 60, 0, 0))


# ================================================================================ s1d8 Giant's Garden
def sunflower(sc):
    """TALL, 12 m. Two giant sunflowers: the heads nod towards the road, the heart leaves flap one by one."""
    r = random.Random(427)
    g = sc.still()
    blob(g, PINE, (0, 0, -20), 150, (1.3, 0.9, 0.5), (4, 7))
    for k, (x, y, H) in enumerate(((-80, 20, 1000), (100, -10, 790))):
        g.cyl(LAWN, (x, y, -200), 18, H + 150, steps=6, top_r=13)
        for j in range(6):
            z = 150 + j * (H - 330) / 5.0
            a = j * 2.4 + k
            at = (x, y, z)
            e = add(at, (math.cos(a) * 170, math.sin(a) * 120, 44))
            lf = sway(sc, at, swing=5, axis=math.degrees(a) + 90, rate=r.uniform(0.3, 0.45), phase=r.random())
            tilt_box(lf, LEAF if j % 2 else FERN, at, e, 96 - j * 6, 8)
        n = sway(sc, (x, y, H - 60), swing=7, axis=0, rate=0.22 + 0.04 * k, phase=r.random())
        tilt_box(n, LAWN, (x, y, H - 70), (x, y - 56, H + 26), 22, 22)
        hc = (x, y - 78, H + 44)
        blob(n, LAWN, (hc[0], hc[1] + 14, hc[2]), 112, (1, 0.16, 1), (4, 8))
        blob(n, WOOD_D, hc, 100, (1, 0.28, 1), (5, 8), glow=0.08)
        blob(n, SEED, (hc[0], hc[1] - 10, hc[2]), 66, (1, 0.3, 1), (4, 8), glow=0.08)
        for j in range(16):
            a = j * 0.3927
            ca, sa = math.cos(a), math.sin(a)
            tilt_box(n, GOLD if j % 2 else AMBER, (hc[0] + ca * 84, hc[1] + 2, hc[2] + sa * 84), (hc[0] + ca * 200, hc[1] + 8, hc[2] + sa * 200), 46, 8, glow=0.15)


def dandelion_clocks(sc):
    """TALL, white. A clump of giant dandelion clocks on stalks rooted in a rosette of toothed leaves: three full clocks
    (a core and twenty rays each: a spiky seed head, not a ball), one half blown, one closed bud. Stands 30 m or more out."""
    r = random.Random(428)
    g = sc.still()
    g.cyl(PINE, (0, 0, -150), 50, 170, steps=6)
    for k in range(8):
        a = k * 0.785
        e = (math.cos(a) * 180, math.sin(a) * 180, 34)
        tilt_box(g, LAWN if k % 2 else LEAF, (0, 0, 14), e, 56, 8)
        g.box(LAWN, lerp((0, 0, 14), e, 0.6), (20, 86, 7), rot=(0, math.degrees(a), 0))
    for k, (H, lean) in enumerate(((640, 0.10), (540, 0.16), (470, 0.2), (390, 0.24), (330, 0.2))):
        a = k * 1.257 + 0.6
        foot = (math.cos(a) * 18, math.sin(a) * 18, 0)
        top = (math.cos(a) * H * lean, math.sin(a) * H * lean, H)
        s = sway(sc, foot, swing=3.5, axis=math.degrees(a) + 90, rate=r.uniform(0.25, 0.4), phase=r.random())
        rod(s, STEM, foot, top, 9, steps=5)
        if k < 3:
            blob(s, CREAM, top, 22, steps=(3, 6), glow=0.2)
            for d in ball_dirs(20):
                tilt_box(s, WHITE, top, add(top, d, 84), 5, 5, glow=0.2)
        elif k == 3:
            blob(s, CREAM, top, 20, steps=(3, 6), glow=0.2)
            for d in ball_dirs(18)[9:]:
                tilt_box(s, WHITE, top, add(top, d, 80), 5, 5, glow=0.2)
        else:
            cone(s, LAWN, add(top, (0, 0, -30)), add(top, (0, 0, 40)), 20, 10, steps=6)
            cone(s, WHITE, add(top, (0, 0, 38)), add(top, (0, 0, 62)), 10, 3, steps=5)


def bean_poles(sc):
    """TALL. A wigwam of six bean poles tied at the top, vines up every pole with red flowers and hanging pods, flowerpots at the foot."""
    r = random.Random(429)
    g = sc.still()
    apex = (0, 0, 690)
    g.cyl(CREAM, (0, 0, 664), 30, 16, steps=7)
    for x, y, rz in ((150, -150, 0), (190, -110, 46)):
        g.cyl(WOOD, (x, y, rz), 30, 46, steps=7, top_r=40)
    for k in range(6):
        a = k * 1.047 + 0.3
        foot = (math.cos(a) * 240, math.sin(a) * 240, -140)
        tip = add(apex, (apex[0] - foot[0], apex[1] - foot[1], apex[2] - foot[2]), 0.12)
        rod(g, WOOD, foot, tip, 9, steps=5)
        v = sway(sc, apex, swing=1.5, axis=math.degrees(a) + 90, rate=r.uniform(0.3, 0.45), phase=r.random())
        px, py = -math.sin(a), math.cos(a)
        for j in range(7):
            t = 0.24 + j * 0.1
            p = lerp(foot, apex, t)
            o = 18 if j % 2 else -18
            v.box((LAWN, PINE, LEAF)[j % 3], (p[0] + px * o, p[1] + py * o, p[2]), (48, 9, 40), rot=(20, math.degrees(a) + 90, 0))
            if j in (1, 3, 5):
                blob(v, RED, (p[0] - px * o * 1.1, p[1] - py * o * 1.1, p[2] + 14), 12, steps=(3, 5), glow=0.3)
            if j in (2, 4):
                v.box(MINT, (p[0] - px * o, p[1] - py * o, p[2] - 30), (9, 9, 66))


def giant_fern(sc):
    """TALL. A giant fern: seven arching fronds, each swinging on its own, three young fronds upright in the middle."""
    r = random.Random(430)
    g = sc.still()
    g.cyl(WOOD_D, (0, 0, -150), 50, 200, steps=7, top_r=36)
    for k in range(7):
        a = k * 0.8976 + 0.2
        ca, sa = math.cos(a), math.sin(a)
        pts = [(ca * 22, sa * 22, 40), (ca * 80, sa * 80, 250), (ca * 170, sa * 170, 370), (ca * 258, sa * 258, 338), (ca * 326, sa * 326, 236)]
        f = sway(sc, pts[0], swing=4, axis=math.degrees(a) + 90, rate=r.uniform(0.26, 0.38), phase=r.random())
        col = (FERN, LEAF, LAWN)[k % 3]
        for j in range(4):
            tilt_box(f, col, pts[j], pts[j + 1], 20, 9)
        for j in range(5):
            p = lerp(pts[j // 2 + 1 if j < 4 else 3], pts[min(j // 2 + 2, 4)], 0.5 * (j % 2) if j < 4 else 0.8)
            ln = 118 - j * 16
            for sgn in (-1, 1):
                tilt_box(f, LEAF if col != LEAF else FERN, p, (p[0] - sa * sgn * ln, p[1] + ca * sgn * ln, p[2] - 16), 34, 6)
    for k in range(3):
        a = k * 2.094 + 0.7
        y = sway(sc, (0, 0, 50), swing=3, axis=math.degrees(a) + 90, rate=0.4, phase=r.random())
        tilt_box(y, FERN, (math.cos(a) * 14, math.sin(a) * 14, 40), (math.cos(a) * 60, math.sin(a) * 60, 430 + k * 40), 14, 10)
        blob(y, MINT, (math.cos(a) * 50, math.sin(a) * 50, 450 + k * 40), 24, steps=(3, 6))


def bluebells(sc):
    """TALL. Giant bluebells: four arching stems, four bells hanging from each, the bells lit steadily from inside."""
    r = random.Random(431)
    g = sc.still()
    blob(g, PINE, (0, 0, -20), 250, (1.1, 0.34, 0.3), (3, 8))
    for k in range(8):
        x = -260 + k * 74
        tilt_box(g, LAWN if k % 2 else FERN, (x, 10, -20), (x + (-40 if k % 2 else 40), 26 - (k % 3) * 22, 150 + (k % 3) * 40), 34, 8)
    for k, x in enumerate((-190, -64, 64, 190)):
        sx = -1 if k % 2 else 1
        H = (330, 390, 360, 310)[k]
        s = sway(sc, (x, 0, 0), swing=3, axis=90, rate=r.uniform(0.25, 0.36), phase=r.random())
        p0, p1, p2, p3 = (x, 0, -100), (x + sx * 10, -10, H - 80), (x + sx * 80, -36, H), (x + sx * 170, -56, H - 50)
        rod(s, LAWN, p0, p1, 10, steps=5)
        rod(s, LAWN, p1, p2, 9, steps=5)
        rod(s, LAWN, p2, p3, 8, steps=5)
        for j in range(4):
            q = lerp(p1, p2, 0.6) if j == 0 else lerp(p2, p3, (j - 1) * 0.5)
            top = (q[0], q[1], q[2] - 12 - (j % 2) * 10)
            s.box(LAWN, (q[0], q[1], q[2] - 8), (5, 5, 20))
            cone(s, BLUEB, top, add(top, (0, 0, -1), 62), 13, 34, steps=6, glow=0.7)
            cone(s, LAV, add(top, (0, 0, -1), 60), add(top, (0, 0, -1), 72), 34, 42, steps=6, glow=0.5)
            s.box(CYAN, (top[0], top[1], top[2] - 70), (9, 9, 16), glow=1.0)


def clover_patch(sc):
    """LOW. Giant clover, seven stems, one of them with four leaves, and two pink clover heads."""
    r = random.Random(432)
    g = sc.still()
    blob(g, PINE, (0, 0, -8), 330, (1, 0.2, 0.09), (3, 9))
    for k in range(7):
        x = -285 + k * 95
        y = (-12, 16, -18, 10, -14, 18, -8)[k]
        h = (120, 70, 150, 96, 136, 78, 110)[k]
        s = sway(sc, (x, y, 0), swing=5, axis=r.uniform(0, 180), rate=r.uniform(0.35, 0.5), phase=r.random())
        s.box(LAWN, (x, y, h / 2.0 - 10), (6, 6, h + 20))
        n = 4 if k == 4 else 3
        for j in range(n):
            a = j * 6.2832 / n + k
            blob(s, LEAF if (j + k) % 2 else LAWN, (x + math.cos(a) * 30, y + math.sin(a) * 30, h + 4), 33, (1, 1, 0.24), (3, 6))
        s.box(MINT, (x, y, h + 10), (12, 12, 5))
    for x, y, h in ((-140, 34, 104), (190, -34, 126)):
        s = sway(sc, (x, y, 0), swing=6, axis=20, rate=0.42, phase=r.random())
        s.box(LAWN, (x, y, h / 2.0 - 10), (5, 5, h + 20))
        blob(s, PINKL, (x, y, h + 14), 24, (1, 1, 1.1), (4, 6))
        blob(s, WHITE, (x, y, h + 30), 13, steps=(3, 5))


JOBS = [
    ('SM_S1PLANTS_TopiaryTubs', topiary_tubs, LOW), ('SM_S1PLANTS_RoseStandards', rose_standards, LOW),
    ('SM_S1PLANTS_BulbPlane', lit(bulb_plane), TALL), ('SM_S1PLANTS_TopiaryTiers', lit(topiary_tiers), TALL),
    ('SM_S1PLANTS_Mangrove', lit(mangrove), TALL), ('SM_S1PLANTS_SeaGrassDrift', sea_grass_drift, LOW), ('SM_S1PLANTS_Hibiscus', lit(hibiscus), TALL),
    ('SM_S1PLANTS_GiantBulrush', lit(giant_bulrush, 0.07), HIGH), ('SM_S1PLANTS_LotusTower', lit(lotus_tower, 0.07), HIGH),
    ('SM_S1PLANTS_Papyrus', lit(papyrus, 0.07), HIGH), ('SM_S1PLANTS_LilyRaft', lily_raft, LOW), ('SM_S1PLANTS_Willow', lit(willow), TALL),
    ('SM_S1PLANTS_SilverSpirals', lit(silver_spirals), TALL), ('SM_S1PLANTS_CactusTins', cactus_tins, LOW), ('SM_S1PLANTS_MoonVine', lit(moon_vine), TALL),
    ('SM_S1PLANTS_BlossomSwirl', lit(blossom_swirl), TALL), ('SM_S1PLANTS_BlossomCones', lit(blossom_cones), TALL), ('SM_S1PLANTS_CandyTulips', candy_tulips, LOW),
    ('SM_S1PLANTS_Cypress', lit(cypress), TALL), ('SM_S1PLANTS_TopiaryMoon', lit(topiary_moon), TALL), ('SM_S1PLANTS_WisteriaArch', lit(wisteria_arch), TALL),
    ('SM_S1PLANTS_LavenderBed', lavender_bed, LOW),
    ('SM_S1PLANTS_SnowFirTall', lit(snow_fir_tall), TALL), ('SM_S1PLANTS_SnowFirGroup', lit(snow_fir_group), TALL),
    ('SM_S1PLANTS_FrostBirch', lit(frost_birch), TALL), ('SM_S1PLANTS_HollyLow', holly_low, LOW),
    ('SM_S1PLANTS_Sunflower', lit(sunflower), TALL), ('SM_S1PLANTS_DandelionClocks', lit(dandelion_clocks), TALL), ('SM_S1PLANTS_BeanPoles', lit(bean_poles), TALL),
    ('SM_S1PLANTS_GiantFern', lit(giant_fern), TALL), ('SM_S1PLANTS_Bluebells', lit(bluebells), TALL), ('SM_S1PLANTS_CloverPatch', clover_patch, LOW),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
