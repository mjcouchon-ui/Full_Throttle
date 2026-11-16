"""Theme Park, Stage 1, builder s1d6: district 6 "The Last Ride: Crescent Keep" (11.94 to 16.1 km).
/Game/World/ThemePark/SM_S1D6_*. Run: import ft_world_park_s1d6 as K; K.run()

A storybook castle town: thin round towers under tall witch-hat roofs, bridges between them, crenellated garden walls,
moons and stars on shutters and vanes. Lavender walls, purple hats, cream battlements, gold windows, cyan banners.
Every piece is an ft_people.Scene (one section, M_People), drawn in cm, front to -Y, standing on z = 0 with a footing.
All of it is invented here: no real place, brand, sign or likeness. No figures, no animals, nothing that flies or circles.
Light is steady (windows, lanterns, the moon); nothing blinks. Movement: banners, pennants, vanes, a bell, a telescope,
a bucket, a plume, the clock's one hand, the stars that hang from the moon; all in the vertex shader.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

LAV, PURPLE, CREAM, GOLD, CYAN = '#b9a4ff', '#3a2f6a', '#fff1d6', '#ffc93c', '#2ee6ff'
PLUM, NAVY, WHITE, MAG, TEAL = '#4a3a78', '#2b2350', '#fff6ea', '#ff4fb8', '#2fc6c0'
TIN, TIN_D, WOOD, WOOD_D, PINE, LAWN = '#a8b4d0', '#7f8aa6', '#9a6a44', '#6a4c36', '#2f6b52', '#4f9a6a'
WG = 0.07        # a wall's own steady glow (a night level: an unlit piece reads as a black shape); tall pieces only
WIN = 1.1        # a lit window
LAMP = 1.3       # a lantern
MOON = 0.55      # the moon's soft light


# ------------------------------------------------------------------------------------------ shapes
def _ring(a, r, x=0.0, y=0.0):
    """The point at angle a (degrees; 0 = the front, -Y; 90 = +X) on a circle of radius r about (x, y)."""
    t = math.radians(a)
    return (x + r * math.sin(t), y - r * math.cos(t))


def moon_pts(R, n=8):
    """A crescent in a side view (x, z), its horns open towards +X."""
    pts = []
    for k in range(n + 1):
        a = math.radians(50.0 + 260.0 * k / n)
        pts.append((R * math.cos(a), R * math.sin(a)))
    for k in range(n + 1):
        a = math.radians(283.0 - 206.0 * k / n)
        pts.append((0.35 * R + 0.82 * R * math.cos(a), 0.82 * R * math.sin(a)))
    return pts


def star_pts(R, n=5, turn=90.0, inner=0.45):
    pts = []
    for k in range(2 * n):
        rr = R if k % 2 == 0 else R * inner
        a = math.radians(turn + k * 180.0 / n)
        pts.append((rr * math.cos(a), rr * math.sin(a)))
    return pts


def moon(m, col, x, y, z, R, thick=10.0, glow=0.0):
    m.side(col, moon_pts(R), -thick / 2.0, thick / 2.0, glow=glow, loc=(x, y, z))


def star(m, col, x, y, z, R, thick=10.0, glow=0.0):
    m.side(col, star_pts(R), -thick / 2.0, thick / 2.0, glow=glow, loc=(x, y, z))


def window(g, x, y, z, r, a, w=70.0, h=150.0, lit=WIN, fine=True, wg=WG):
    """A pointed gold window in a cream frame on a round wall of radius r, at angle a."""
    px, py = _ring(a, r + 2.0, x, y)
    g.box(CREAM, (px, py, z), (w + 24.0, 12.0, h + 18.0), rot=(0, a, 0), glow=wg)
    if fine:
        g.box(CREAM, (px, py, z + h / 2.0 + 9.0), ((w + 24.0) * 0.71, 12.0, (w + 24.0) * 0.71), rot=(45, a, 0), glow=wg)
    px, py = _ring(a, r + 6.0, x, y)
    g.box(GOLD, (px, py, z), (w, 12.0, h), rot=(0, a, 0), glow=lit)
    g.box(GOLD, (px, py, z + h / 2.0), (w * 0.71, 12.0, w * 0.71), rot=(45, a, 0), glow=lit)


def flat_window(g, x, y, z, w=80.0, h=170.0, lit=WIN, fine=True):
    """The same on a flat front wall whose face is at y."""
    window(g, x, y + 2.0, z, 0.0, 0.0, w, h, lit, fine)


def shaft(g, x, y, r, h, wall=LAV, foot=200.0, flare=1.32, steps=8, glow=WG):
    """A thin round tower's body: a plum plinth with its footing, a tapering shaft, a corbelled flare and a cream
    string course at h. Returns the flared radius."""
    hs = h - r * 0.9
    g.cyl(PLUM, (x, y, -foot), r * 1.14, foot + 70.0, steps=steps, glow=glow)
    g.cyl(wall, (x, y, 70.0), r, hs - 70.0, steps=steps, top_r=r * 0.92, glow=glow)
    g.cyl(wall, (x, y, hs), r * 0.92, r * 0.9, steps=steps, top_r=r * flare, glow=glow)
    g.cyl(CREAM, (x, y, h), r * flare * 1.05, 36.0, steps=steps, glow=glow)
    return r * flare


def hat(g, x, y, z, r0, roof, steps=8, col=PURPLE, glow=WG):
    """A witch-hat roof: steep, drawn in at the waist, a gold band, a gold ball on the point. Returns the tip's z."""
    g.cyl(col, (x, y, z), r0, roof * 0.42, steps=steps, top_r=r0 * 0.46, glow=glow * 0.6)
    g.cyl(col, (x, y, z + roof * 0.42), r0 * 0.46, roof * 0.58, steps=steps, top_r=5.0, glow=glow * 0.6)
    g.cyl(GOLD, (x, y, z + roof * 0.42 - 8.0), r0 * 0.5, 16.0, steps=steps, glow=0.3)
    g.sphere(GOLD, (x, y, z + roof + 6.0), max(r0 * 0.07, 12.0), steps=(3, 6), glow=0.5)
    return z + roof


def tower(g, x, y, r, h, floors, roof, wall=LAV, wins=(-22.5,), foot=200.0, flare=1.32, steps=8, merlons=True, glow=WG,
          twist=0.0, fine=True, head_win=True):
    """A whole thin tower: shaft, a head wider than the base, cream battlements, a witch hat. wins: the angles of the
    windows on every floor (twist: degrees more on each floor up). Returns (the head's top, the hat's tip)."""
    rf = shaft(g, x, y, r, h, wall, foot, flare, steps, glow)
    hs = h - r * 0.9
    hd = r * 1.05
    g.cyl(wall, (x, y, h + 36.0), rf, hd, steps=steps, glow=glow)
    zt = h + 36.0 + hd
    if merlons:
        for k in range(steps):
            a = (k + 0.5) * 360.0 / steps
            px, py = _ring(a, rf * 0.96, x, y)
            g.box(CREAM, (px, py, zt + 28.0), (rf * 0.42, max(r * 0.16, 24.0), 56.0), rot=(0, a, 0), glow=glow)
    tip = hat(g, x, y, zt, rf * (0.84 if merlons else 1.18), roof, steps, glow=glow)
    ww = min(r * 0.42, 95.0)
    for f in range(floors):
        z = 70.0 + (f + 0.58) * (hs - 70.0) / floors
        rr = r * (1.0 - 0.08 * (z - 70.0) / max(hs - 70.0, 1.0))
        for a in wins:
            window(g, x, y, z, rr * 0.955, a + twist * f, ww, ww * 2.1, fine=fine, wg=glow)
    if head_win:
        window(g, x, y, h + 36.0 + hd * 0.5, rf * 0.955, wins[0] if wins else 22.5, ww * 0.8, hd * 0.42, fine=False, wg=glow)
    return zt, tip


def banner(sc, x, y, ztop, w, length, a=0.0, col=CYAN, phase=0.0, swing=4.0, rate=0.3, mark=CREAM):
    """A long banner hung from a gold rod at (x, y, ztop) on a wall that faces angle a; it swings from the rod."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, ztop), axis=a, rate=rate, phase=phase))
    t = math.radians(a)
    tx, ty = math.cos(t), math.sin(t)
    f.box(GOLD, (x, y, ztop), (w + 16.0, 9.0, 9.0), rot=(0, a, 0))
    f.box(col, (x, y, ztop - length / 2.0), (w, 6.0, length), rot=(0, a, 0))
    for sgn in (-1.0, 1.0):       # a swallow tail
        f.box(col, (x + sgn * tx * w * 0.27, y + sgn * ty * w * 0.27, ztop - length - w * 0.2), (w * 0.46, 6.0, w * 0.4), rot=(0, a, 0))
    if mark:
        px, py = _ring(a, 3.0, x, y)
        f.box(mark, (px, py, ztop - length * 0.4), (w * 0.44, 6.0, w * 0.44), rot=(45, a, 0))
        f.box(PURPLE, (px, py, ztop - length * 0.82), (w, 6.0, w * 0.16), rot=(0, a, 0))


def pennant(sc, x, y, z, length=130.0, col=CYAN, phase=0.0, mast=170.0):
    """A mast on a roof's point with a pennant that turns in the wind (z = the mast's foot)."""
    g = sc.still()
    g.cyl(GOLD, (x, y, z), 5.0, mast, steps=5)
    f = sc.part(P.motion(swing=16.0, pivot=(x, y, z + mast), axis=P.UP, rate=0.4, phase=phase))
    f.box(col, (x + length * 0.5 + 4.0, y, z + mast - 26.0), (length, 5.0, 44.0))
    f.box(col, (x + length + 14.0, y, z + mast - 14.0), (30.0, 5.0, 18.0))
    f.box(col, (x + length + 14.0, y, z + mast - 38.0), (30.0, 5.0, 18.0))


def vane(sc, x, y, z, what='moon', R=46.0, phase=0.0, glow=0.5):
    """A weather vane on a roof's point: a gold moon or star that turns slowly to and fro (z = the mast's foot)."""
    g = sc.still()
    g.cyl(GOLD, (x, y, z), 5.0, 120.0, steps=5)
    g.box(GOLD, (x, y, z + 60.0), (90.0, 5.0, 5.0))
    g.box(GOLD, (x, y, z + 60.0), (5.0, 90.0, 5.0))
    f = sc.part(P.motion(swing=50.0, pivot=(x, y, z + 120.0), axis=P.UP, rate=0.07, phase=phase))
    if what == 'moon':
        moon(f, GOLD, x, y, z + 120.0 + R, R, glow=glow)
    else:
        star(f, GOLD, x, y, z + 120.0 + R * 0.9, R, glow=glow)


def door(g, x, y, w=120.0, h=210.0, a=0.0, r=0.0, lit=0.0):
    """A wooden door under a cream pointed arch, two cream steps, on a wall at (angle a, radius r) of (x, y)."""
    px, py = _ring(a, r + 3.0, x, y)
    g.box(CREAM, (px, py, 70.0 + h / 2.0), (w + 30.0, 14.0, h + 20.0), rot=(0, a, 0))
    g.box(CREAM, (px, py, 70.0 + h + 6.0), ((w + 30.0) * 0.71, 14.0, (w + 30.0) * 0.71), rot=(45, a, 0))
    px, py = _ring(a, r + 8.0, x, y)
    g.box(WOOD_D, (px, py, 70.0 + h / 2.0), (w, 14.0, h), rot=(0, a, 0))
    g.box(WOOD_D, (px, py, 70.0 + h), (w * 0.71, 14.0, w * 0.71), rot=(45, a, 0))
    g.box(GOLD, (px + 0.0, py - 6.0, 70.0 + h * 0.5), (12.0, 8.0, 12.0), glow=lit)
    px, py = _ring(a, r + 45.0, x, y)
    g.box(CREAM, (px, py, 35.0), (w + 70.0, 90.0, 70.0), rot=(0, a, 0))


def lantern(g, x, y, z, glow=LAMP):
    """A wall lantern: a bracket, a cream glass with a purple cap. Steady."""
    g.box(PURPLE, (x, y + 14.0, z + 30.0), (8.0, 40.0, 8.0))
    g.cyl(PURPLE, (x, y, z + 22.0), 20.0, 12.0, steps=6, top_r=6.0)
    g.cyl(CREAM, (x, y, z - 14.0), 13.0, 36.0, steps=6, glow=glow)
    g.cyl(PURPLE, (x, y, z - 20.0), 15.0, 6.0, steps=6)


def flower_box(g, x, y, z, w=90.0, a=0.0):
    g.box(WOOD_D, (x, y, z), (w, 26.0, 20.0), rot=(0, a, 0))
    t = math.radians(a)
    for k in (-1, 0, 1):
        g.box(MAG if k else CREAM, (x + k * w * 0.3 * math.cos(t), y + k * w * 0.3 * math.sin(t), z + 18.0), (22.0, 22.0, 20.0), rot=(0, a + 20.0 * k, 0))


def gable_roof(g, x, y, z0, length, half, rise, col=PURPLE, wall=LAV, thick=24.0, over=34.0, steps_n=4, glow=WG):
    """A steep roof whose ridge runs along X, with crow-stepped gable walls at both ends and a gold ridge."""
    k = (half + over) / float(half)
    tilt_box(g, col, (x, y - half * k, z0 - rise * (k - 1.0)), (x, y, z0 + rise), length, thick, glow=glow * 0.6)
    tilt_box(g, col, (x, y + half * k, z0 - rise * (k - 1.0)), (x, y, z0 + rise), length, thick, glow=glow * 0.6)
    g.box(GOLD, (x, y, z0 + rise + 8.0), (length + 10.0, 14.0, 14.0), glow=0.25)
    for sgn in (-1.0, 1.0):
        for j in range(steps_n):
            f = 1.0 - (j + 0.35) / float(steps_n)
            g.box(wall if j % 2 == 0 else CREAM, (x + sgn * (length / 2.0 - 30.0), y, z0 + rise * (j + 0.5) / steps_n + (30.0 if j == steps_n - 1 else 0.0)),
                  (44.0, 2.0 * half * f + 30.0, rise / steps_n + (60.0 if j == steps_n - 1 else 0.0)), glow=glow)


def crenels(g, x, y, z, length, n, col=CREAM, depth=44.0, h=52.0, glow=WG):
    """A row of merlons along X, the wall's top at z."""
    step = length / float(n)
    for k in range(n):
        g.box(col, (x - length / 2.0 + (k + 0.5) * step, y, z + h / 2.0), (step * 0.56, depth, h), glow=glow)


# ------------------------------------------------------------------------------------------ BUILDINGS
def round_tower_a(sc):
    """THE PAGE'S TOWER: the district's thin tower, three storeys, 5 m across its head, a moon vane on the hat's point."""
    g = sc.still()
    zt, tip = tower(g, 0, 0, 190.0, 1050.0, 3, 640.0, wins=(-22.5,), twist=45.0)
    door(g, 0, 0, 100.0, 190.0, a=22.5, r=190.0 * 0.93)
    lantern(g, -150.0, -215.0, 300.0)
    flower_box(g, *(_ring(-22.5, 190.0) + (300.0,)), w=80.0, a=-22.5)
    g.box(LAV, (120.0, 60.0, zt + 150.0), (44.0, 44.0, 200.0), glow=WG)          # a chimney through the hat's brim
    g.box(CREAM, (120.0, 60.0, zt + 256.0), (56.0, 56.0, 14.0), glow=WG)
    vane(sc, 0, 0, tip, 'moon', 44.0, phase=0.2)
    banner(sc, *(_ring(67.5, 190.0 * 0.9 + 8.0) + (860.0,)), w=70.0, length=330.0, a=67.5, phase=0.1)
    for a, z in ((-67.5, 420.0), (67.5, 360.0)):                                  # shutters with a moon cut in them
        px, py = _ring(a, 190.0 * 0.93, 0, 0)
        g.box(PURPLE, (px, py, z), (46.0, 10.0, 70.0), rot=(0, a, 0))
        g.box(GOLD, (px + 4.0 * math.sin(math.radians(a)), py - 4.0 * math.cos(math.radians(a)), z + 8.0), (16.0, 8.0, 16.0), rot=(45, a, 0))


def round_tower_b(sc):
    """THE STAR TOWER: five storeys, an oriel turret hung on its side under a hat of its own, a gold star on the point."""
    g = sc.still()
    zt, tip = tower(g, 0, 0, 215.0, 1680.0, 5, 780.0, wins=(-22.5,), twist=45.0)
    door(g, 0, 0, 110.0, 200.0, a=-22.5, r=215.0 * 0.93)
    # the oriel: a little round room on a corbel, 9 m up on the +X side, with its own hat and a pennant
    ox, oy = 250.0, -60.0
    g.cyl(LAV, (ox, oy, 760.0), 30.0, 130.0, steps=6, top_r=112.0, glow=WG)
    g.cyl(LAV, (ox, oy, 890.0), 112.0, 300.0, steps=6, glow=WG)
    g.cyl(CREAM, (ox, oy, 1190.0), 122.0, 26.0, steps=6, glow=WG)
    hat(g, ox, oy, 1216.0, 135.0, 330.0, steps=6)
    window(g, ox, oy, 1050.0, 104.0, 0.0, 44.0, 96.0, fine=False)
    window(g, ox, oy, 1050.0, 104.0, 60.0, 44.0, 96.0, fine=False)
    pennant(sc, ox, oy, 1546.0, 90.0, phase=0.4, mast=90.0)
    # a washing line from the oriel to a bracket on the shaft
    g.box(WOOD_D, (-215.0, -40.0, 640.0), (70.0, 8.0, 8.0))
    f = sc.part(P.motion(swing=5.0, pivot=(0.0, -215.0, 650.0), axis=0.0, rate=0.35, phase=0.6))
    f.box(WHITE, (0.0, -212.0, 648.0), (430.0, 5.0, 5.0))
    for k, c in enumerate((CREAM, CYAN, WHITE, MAG, CREAM)):
        f.box(c, (-150.0 + k * 75.0, -214.0, 614.0), (40.0, 5.0, 62.0 - 10.0 * (k % 2)))
    star(g, GOLD, 0, 0, tip + 80.0, 70.0, glow=0.6)
    g.cyl(GOLD, (0, 0, tip), 5.0, 40.0, steps=5)
    banner(sc, *(_ring(-67.5, 215.0 * 0.9 + 8.0) + (1380.0,)), w=80.0, length=420.0, a=-67.5, phase=0.5)
    lantern(g, 120.0, -238.0, 310.0)


def twin_turrets(sc):
    """THE SISTERS: two very thin towers of different heights, a covered bridge between them high up."""
    g = sc.still()
    zt1, tip1 = tower(g, -265.0, 0, 150.0, 1280.0, 3, 560.0, wins=(-22.5,), twist=45.0, fine=False, merlons=False)
    zt2, tip2 = tower(g, 265.0, 0, 150.0, 1560.0, 4, 620.0, wins=(22.5,), twist=-45.0, fine=False)
    # the bridge: a cream arch under a lavender gallery with a purple roof and three lit windows
    g.box(CREAM, (0, 0, 880.0), (300.0, 150.0, 36.0), glow=WG)
    for sgn in (-1.0, 1.0):
        tilt_box(g, CREAM, (sgn * 150.0, 0, 760.0), (sgn * 20.0, 0, 868.0), 120.0, 30.0, glow=WG)
    g.box(LAV, (0, 0, 990.0), (300.0, 140.0, 190.0), glow=WG)
    gable_roof(g, 0, 0, 1085.0, 310.0, 80.0, 130.0, steps_n=1, over=20.0)
    for k in (-1, 0, 1):
        flat_window(g, k * 90.0, -70.0, 985.0, 40.0, 80.0, fine=False)
    door(g, -265.0, 0, 90.0, 180.0, a=0.0, r=150.0 * 0.98)
    vane(sc, -265.0, 0, tip1, 'star', 40.0, phase=0.7)
    pennant(sc, 265.0, 0, tip2, 120.0, phase=0.1)
    banner(sc, 0.0, -78.0, 870.0, w=90.0, length=300.0, a=0.0, phase=0.3)
    lantern(g, 265.0 - 110.0, -160.0, 290.0)


def gatehouse(sc):
    """THE MOON GATE: two hat-roofed towers, a pointed gateway between them under a battlement, the portcullis half
    up, a little turret over the arch, two long banners, lanterns."""
    g = sc.still()
    for sgn in (-1.0, 1.0):
        tower(g, sgn * 540.0, 0, 230.0, 1180.0, 3, 700.0, wins=(sgn * -22.5,), twist=0.0)
    g.box(PLUM, (0, 30.0, -65.0), (700.0, 330.0, 270.0), glow=WG)
    g.box(LAV, (0, 30.0, 500.0), (700.0, 300.0, 860.0), glow=WG)
    g.box(CREAM, (0, 30.0, 946.0), (740.0, 340.0, 32.0), glow=WG)
    crenels(g, 0, -116.0, 962.0, 700.0, 7)
    crenels(g, 0, 176.0, 962.0, 700.0, 7)
    # the gateway: a dark pointed opening, cream voussoirs, gold bars
    g.box(CREAM, (0, -124.0, 330.0), (350.0, 16.0, 540.0))
    g.box(CREAM, (0, -124.0, 600.0), (247.0, 16.0, 247.0), rot=(45, 0, 0))
    g.box(NAVY, (0, -130.0, 320.0), (290.0, 16.0, 500.0))
    g.box(NAVY, (0, -130.0, 570.0), (205.0, 16.0, 205.0), rot=(45, 0, 0))
    for k in range(5):
        g.box(GOLD, (-116.0 + k * 58.0, -137.0, 500.0), (10.0, 8.0, 250.0), glow=0.3)
    for z in (420.0, 520.0, 620.0):
        g.box(GOLD, (0, -137.0, z), (270.0, 8.0, 10.0), glow=0.3)
    # a shield with a crescent over the arch, a little hat turret on the wall walk
    g.box(PURPLE, (0, -128.0, 830.0), (120.0, 14.0, 130.0))
    moon(g, GOLD, -6.0, -138.0, 832.0, 40.0, glow=0.5)
    g.cyl(LAV, (0, 30.0, 962.0), 120.0, 230.0, steps=8, glow=WG)
    g.cyl(CREAM, (0, 30.0, 1192.0), 136.0, 24.0, steps=8, glow=WG)
    tip = hat(g, 0, 30.0, 1216.0, 150.0, 420.0)
    window(g, 0, 30.0, 1085.0, 112.0, 22.5, 44.0, 96.0, fine=False)
    pennant(sc, 0, 30.0, tip, 140.0, phase=0.3)
    for sgn in (-1.0, 1.0):
        banner(sc, sgn * 250.0, -128.0, 900.0, w=96.0, length=480.0, a=0.0, phase=0.2 + 0.3 * sgn)
        lantern(g, sgn * 205.0, -150.0, 330.0)
        g.box(CREAM, (sgn * 330.0, -150.0, 60.0), (90.0, 90.0, 120.0))             # a planter either side
        g.sphere(PINE, (sgn * 330.0, -150.0, 170.0), 62.0, steps=(4, 6))
        g.sphere(LAWN, (sgn * 330.0, -150.0, 250.0), 40.0, steps=(3, 6))


def astronomer(sc):
    """THE ASTRONOMER'S TOWER: five storeys, a stair that winds round the outside, a balcony at the top with a brass
    telescope that pans slowly across the sky, stars painted on the head, a moon on the hat's point."""
    g = sc.still()
    R = 300.0
    zt, tip = tower(g, 0, 0, R, 2050.0, 5, 820.0, wins=(22.5, -67.5), twist=0.0, fine=False)
    door(g, 0, 0, 120.0, 210.0, a=-22.5, r=R * 0.93)
    for k in range(16):                                                           # the outside stair
        a = -60.0 + k * 13.0
        px, py = _ring(a, R * 0.96 + 30.0, 0, 0)
        g.box(CREAM, (px, py, 300.0 + k * 96.0), (86.0, 76.0, 24.0), rot=(0, a, 0), glow=WG)
    rf = R * 1.32
    zb = 2050.0 + 36.0 + 60.0
    g.box(CREAM, (0, -rf - 70.0, zb), (420.0, 200.0, 26.0), glow=WG)              # the balcony
    for x in (-200.0, 0.0, 200.0):
        g.box(GOLD, (x, -rf - 162.0, zb + 60.0), (10.0, 10.0, 100.0))
    g.box(GOLD, (0, -rf - 162.0, zb + 110.0), (420.0, 10.0, 10.0))
    for sgn in (-1.0, 1.0):
        tilt_box(g, CREAM, (sgn * 150.0, -rf + 10.0, zb - 150.0), (sgn * 150.0, -rf - 140.0, zb - 10.0), 30.0, 30.0)
    for k, (a, z) in enumerate(((-45.0, 2260.0), (-5.0, 2300.0), (48.0, 2250.0), (80.0, 2310.0))):   # painted stars
        px, py = _ring(a, rf + 4.0, 0, 0)
        g.box(GOLD, (px, py, z), (42.0, 8.0, 42.0), rot=(45, a, 0), glow=0.6)
    pv = (0.0, -rf - 70.0, zb + 13.0)
    tm = sc.part(P.motion(swing=22.0, pivot=pv, axis=P.UP, rate=0.04, phase=0.3))
    for dx, dy in ((-50.0, 30.0), (50.0, 30.0), (0.0, -50.0)):
        tilt_box(tm, WOOD_D, (pv[0] + dx, pv[1] + dy, pv[2]), (pv[0], pv[1], pv[2] + 120.0), 10.0, 10.0)
    rod(tm, GOLD, (pv[0], pv[1] + 130.0, pv[2] + 60.0), (pv[0], pv[1] - 300.0, pv[2] + 330.0), 36.0, steps=8, glow=0.25)
    rod(tm, TIN_D, (pv[0], pv[1] - 300.0, pv[2] + 330.0), (pv[0], pv[1] - 345.0, pv[2] + 358.0), 46.0, steps=8)
    rod(tm, PURPLE, (pv[0], pv[1] + 130.0, pv[2] + 60.0), (pv[0], pv[1] + 160.0, pv[2] + 41.0), 22.0, steps=6)
    vane(sc, 0, 0, tip, 'moon', 56.0, phase=0.5)
    banner(sc, *(_ring(67.5, R * 0.9 + 8.0) + (1700.0,)), w=100.0, length=560.0, a=67.5, phase=0.8)
    lantern(g, 150.0, -R - 30.0, 320.0)


def bell_tower(sc):
    """THE EVENING BELL: a tower whose head is an open belfry of cream posts under the hat; the gold bell swings."""
    g = sc.still()
    R, H = 260.0, 1750.0
    rf = shaft(g, 0, 0, R, H)
    hs = H - R * 0.9
    for f in range(4):
        z = 70.0 + (f + 0.58) * (hs - 70.0) / 4
        window(g, 0, 0, z, R * 0.93, 22.5 if f % 2 else -22.5, 90.0, 190.0)
    for k in range(4):                                                            # louvres under the belfry
        g.box(PURPLE, (0, -R * 0.9, hs - 260.0 + k * 0.0 - k * 34.0), (110.0, 14.0, 16.0))
    zf = H + 36.0
    g.cyl(LAV, (0, 0, zf), rf, 70.0, steps=8, glow=WG)                            # the belfry's floor and parapet
    for k in range(8):
        a = (k + 0.5) * 45.0
        px, py = _ring(a, rf * 0.96, 0, 0)
        g.box(CREAM, (px, py, zf + 96.0), (rf * 0.42, 40.0, 52.0), rot=(0, a, 0), glow=WG)
    for k in range(6):
        px, py = _ring(k * 60.0 + 30.0, rf * 0.78, 0, 0)
        g.cyl(CREAM, (px, py, zf + 70.0), 26.0, 330.0, steps=6, glow=WG)
    g.cyl(CREAM, (0, 0, zf + 400.0), rf * 0.98, 40.0, steps=8, glow=WG)
    tip = hat(g, 0, 0, zf + 440.0, rf * 1.14, 760.0)
    g.box(WOOD_D, (0, 0, zf + 380.0), (rf * 1.5, 22.0, 22.0))
    b = sc.part(P.motion(swing=17.0, pivot=(0.0, 0.0, zf + 380.0), axis=0.0, rate=0.3, phase=0.0))
    b.cyl(GOLD, (0, 0, zf + 160.0), 120.0, 60.0, steps=8, top_r=96.0, glow=0.35)
    b.cyl(GOLD, (0, 0, zf + 220.0), 96.0, 120.0, steps=8, top_r=52.0, glow=0.35)
    b.cyl(GOLD, (0, 0, zf + 340.0), 26.0, 40.0, steps=6, glow=0.35)
    c = sc.part(P.motion(swing=24.0, pivot=(0.0, 0.0, zf + 330.0), axis=0.0, rate=0.3, phase=0.22))
    c.box(NAVY, (0, 0, zf + 230.0), (12.0, 12.0, 190.0))
    c.sphere(NAVY, (0, 0, zf + 130.0), 22.0, steps=(3, 6))
    door(g, 0, 0, 120.0, 210.0, a=22.5, r=R * 0.93)
    vane(sc, 0, 0, tip, 'star', 50.0, phase=0.1)
    banner(sc, *(_ring(-67.5, R * 0.9 + 8.0) + (1300.0,)), w=96.0, length=520.0, a=-67.5, phase=0.4)
    lantern(g, -150.0, -R - 30.0, 320.0)
    flower_box(g, *(_ring(-22.5, R * 0.95) + (hs * 0.14 + 70.0,)), w=100.0, a=-22.5)


def bridge_house(sc):
    """THE BRIDGE HOUSE: a little house with a steep roof on a bridge of two pointed arches, a stub tower at one end."""
    g = sc.still()
    for x in (-720.0, 0.0, 720.0):                                                # three piers
        g.box(PLUM, (x, 0, -65.0), (190.0, 480.0, 270.0), glow=WG)
        g.box(LAV, (x, 0, 330.0), (170.0, 460.0, 520.0), glow=WG)
        g.box(CREAM, (x, -236.0, 150.0), (110.0, 30.0, 160.0), glow=WG)          # a cutwater
    for x in (-360.0, 360.0):                                                     # two pointed arches
        for sgn in (-1.0, 1.0):
            tilt_box(g, LAV, (x + sgn * 290.0, 0, 430.0), (x + sgn * 10.0, 0, 640.0), 440.0, 70.0, glow=WG)
            tilt_box(g, CREAM, (x + sgn * 290.0, -224.0, 410.0), (x + sgn * 8.0, -224.0, 612.0), 16.0, 30.0, glow=WG)
    g.box(LAV, (0, 0, 660.0), (1640.0, 460.0, 120.0), glow=WG)                    # the deck
    g.box(CREAM, (0, 0, 728.0), (1700.0, 500.0, 24.0), glow=WG)
    crenels(g, -560.0, -226.0, 740.0, 440.0, 5)
    crenels(g, 500.0, -226.0, 740.0, 320.0, 4)
    # the house on the bridge
    g.box(LAV, (-30.0, 10.0, 940.0), (620.0, 400.0, 400.0), glow=WG)
    g.box(CREAM, (-30.0, 10.0, 1140.0), (650.0, 430.0, 20.0), glow=WG)
    gable_roof(g, -30.0, 10.0, 1150.0, 660.0, 215.0, 420.0)
    for k in (-1, 0, 1):
        flat_window(g, -30.0 + k * 190.0, -190.0, 960.0, 70.0, 150.0)
    g.box(CREAM, (-30.0, -212.0, 800.0), (560.0, 60.0, 18.0), glow=WG)           # a balcony on corbels
    for k in range(5):
        g.box(GOLD, (-290.0 + k * 130.0, -238.0, 850.0), (8.0, 8.0, 90.0))
    g.box(GOLD, (-30.0, -238.0, 896.0), (560.0, 8.0, 8.0))
    g.box(LAV, (160.0, 80.0, 1480.0), (60.0, 60.0, 260.0), glow=WG)              # chimney
    g.box(CREAM, (160.0, 80.0, 1616.0), (76.0, 76.0, 16.0), glow=WG)
    # the stub tower at the +X end, its hat over the roof
    g.cyl(LAV, (640.0, 30.0, 740.0), 150.0, 520.0, steps=8, top_r=170.0, glow=WG)
    g.cyl(CREAM, (640.0, 30.0, 1260.0), 182.0, 28.0, steps=8, glow=WG)
    tip = hat(g, 640.0, 30.0, 1288.0, 200.0, 520.0)
    window(g, 640.0, 30.0, 1020.0, 150.0, -22.5, 50.0, 110.0, fine=False)
    vane(sc, 640.0, 30.0, tip, 'moon', 40.0, phase=0.9)
    for k, x in enumerate((-360.0, 360.0)):
        banner(sc, x, -236.0, 640.0, w=110.0, length=300.0, a=0.0, phase=0.25 + 0.4 * k)
    lantern(g, -720.0, -262.0, 560.0)
    lantern(g, 0.0, -262.0, 560.0)


def banner_hall(sc):
    """THE BANNER HALL: a long hall under a steep purple roof, a row of gold pointed windows, a bartizan with a hat on
    each front corner, five long cyan banners along the eaves."""
    g = sc.still()
    L, D, H = 2000.0, 760.0, 640.0
    g.box(PLUM, (0, 0, -90.0), (L + 60.0, D + 60.0, 320.0), glow=WG)
    g.box(LAV, (0, 0, 70.0 + H / 2.0), (L, D, H), glow=WG)
    g.box(CREAM, (0, 0, 70.0 + H + 12.0), (L + 50.0, D + 50.0, 24.0), glow=WG)
    gable_roof(g, 0, 0, 70.0 + H + 24.0, L + 40.0, D / 2.0, 820.0, steps_n=5)
    for k in range(6):
        if k in (2, 3):
            continue
        flat_window(g, -L / 2.0 + 200.0 + k * 320.0, -D / 2.0, 430.0, 90.0, 220.0)
    # the great door between the middle bays, a star window over it
    g.box(CREAM, (0, -D / 2.0 - 6.0, 270.0), (300.0, 16.0, 400.0))
    g.box(CREAM, (0, -D / 2.0 - 6.0, 470.0), (212.0, 16.0, 212.0), rot=(45, 0, 0))
    g.box(WOOD_D, (0, -D / 2.0 - 12.0, 260.0), (240.0, 16.0, 380.0))
    g.box(WOOD_D, (0, -D / 2.0 - 12.0, 450.0), (170.0, 16.0, 170.0), rot=(45, 0, 0))
    g.box(GOLD, (0, -D / 2.0 - 20.0, 260.0), (10.0, 8.0, 380.0))
    star(g, GOLD, 0, -D / 2.0 - 14.0, 640.0, 46.0, glow=WIN)
    g.box(CREAM, (0, -D / 2.0 - 70.0, 35.0), (360.0, 140.0, 70.0))
    for sgn in (-1.0, 1.0):                                                       # the bartizans
        x = sgn * (L / 2.0 - 10.0)
        y = -D / 2.0 + 10.0
        g.cyl(LAV, (x, y, 380.0), 40.0, 150.0, steps=8, top_r=125.0, glow=WG)
        g.cyl(LAV, (x, y, 530.0), 125.0, 360.0, steps=8, glow=WG)
        g.cyl(CREAM, (x, y, 890.0), 136.0, 26.0, steps=8, glow=WG)
        tip = hat(g, x, y, 916.0, 150.0, 440.0)
        window(g, x, y, 700.0, 117.0, sgn * 22.5, 44.0, 100.0, fine=False)
        if sgn < 0:
            vane(sc, x, y, tip, 'moon', 38.0, phase=0.3)
        else:
            pennant(sc, x, y, tip, 120.0, phase=0.6)
        lantern(g, sgn * 190.0, -D / 2.0 - 30.0, 360.0)
    for k in range(5):                                                            # the banners
        x = -L / 2.0 + 360.0 + k * 320.0
        if k == 2:
            continue
        banner(sc, x, -D / 2.0 - 16.0, 690.0, w=100.0, length=330.0, a=0.0, phase=0.17 * k, col=CYAN if k % 2 == 0 else CREAM,
               mark=CREAM if k % 2 == 0 else CYAN)
    for k in range(3):                                                            # dormers with lit eyes
        x = -560.0 + k * 560.0
        g.box(LAV, (x, -190.0, 1130.0), (120.0, 160.0, 150.0), glow=WG)
        g.box(GOLD, (x, -274.0, 1130.0), (60.0, 10.0, 84.0), glow=WIN)
        g.box(PURPLE, (x, -190.0, 1216.0), (150.0, 190.0, 24.0))
    g.box(LAV, (620.0, 150.0, 1300.0), (80.0, 80.0, 360.0), glow=WG)             # chimney
    g.box(CREAM, (620.0, 150.0, 1486.0), (100.0, 100.0, 18.0), glow=WG)


def dovecote(sc):
    """THE DOVECOTE: a fat short tower with two rings of pigeon holes under a wide hat, a lantern cupola on top, a
    ladder against it. (The doves are the animal builder's.)"""
    g = sc.still()
    R, H = 330.0, 800.0
    rf = shaft(g, 0, 0, R, H, flare=1.36)
    hd = 330.0
    g.cyl(WHITE, (0, 0, H + 36.0), rf, hd, steps=8, glow=WG)
    for row in range(2):
        for k in range(8):
            a = k * 45.0 + 22.5
            if math.cos(math.radians(a)) < -0.5:
                continue                                                          # the back is never seen
            px, py = _ring(a, rf * 0.924 + 2.0, 0, 0)
            z = H + 36.0 + 100.0 + row * 140.0
            g.box(NAVY, (px, py, z), (56.0, 12.0, 70.0), rot=(0, a, 0))
            g.box(NAVY, (px, py, z + 35.0), (40.0, 12.0, 40.0), rot=(45, a, 0))
            px, py = _ring(a, rf * 0.924 + 16.0, 0, 0)
            g.box(WOOD, (px, py, z - 42.0), (70.0, 30.0, 8.0), rot=(0, a, 0))
    zt = H + 36.0 + hd
    g.cyl(CREAM, (0, 0, zt), rf * 1.06, 26.0, steps=8, glow=WG)
    g.cyl(PURPLE, (0, 0, zt + 26.0), rf * 1.26, 300.0, steps=8, top_r=150.0, glow=WG * 0.6)
    g.cyl(GOLD, (0, 0, zt + 318.0), 156.0, 16.0, steps=8, glow=0.3)
    g.cyl(WHITE, (0, 0, zt + 326.0), 120.0, 150.0, steps=8, glow=WG)             # the cupola
    for k in range(4):
        px, py = _ring(k * 90.0 + 22.5 - 45.0, 116.0, 0, 0)
        g.box(GOLD, (px, py, zt + 400.0), (40.0, 10.0, 70.0), rot=(0, k * 90.0 - 22.5, 0), glow=WIN)
    tip = hat(g, 0, 0, zt + 476.0, 160.0, 380.0)
    vane(sc, 0, 0, tip, 'star', 42.0, phase=0.6)
    door(g, 0, 0, 110.0, 190.0, a=-22.5, r=R * 0.93)
    window(g, 0, 0, 470.0, R * 0.93, 22.5, 80.0, 150.0)
    for sgn in (-1.0, 1.0):                                                       # the ladder
        tilt_box(g, WOOD, (230.0 + sgn * 26.0, -420.0, 0.0), (230.0 + sgn * 26.0, -300.0, 820.0), 9.0, 9.0)
    for k in range(8):
        f = (k + 0.5) / 8.0
        g.box(WOOD, (230.0, -420.0 + 120.0 * f, 820.0 * f), (60.0, 8.0, 8.0))
    banner(sc, *(_ring(67.5, R * 0.9 + 8.0) + (620.0,)), w=90.0, length=300.0, a=67.5, phase=0.9)
    g.cyl(WOOD, (-300.0, -330.0, 0.0), 46.0, 70.0, steps=8)                       # a seed barrel and a sack
    g.box(CREAM, (-380.0, -300.0, 30.0), (60.0, 50.0, 60.0))


# ------------------------------------------------------------------------------------------ PROPS
def garden_wall(sc):
    """A run of crenellated garden wall, 9 m: cream merlons, a moon and two stars let into it, a hat-capped post at
    each end, a climbing rose, a steady lantern on a bracket. 3.6 m to the caps."""
    g = sc.still()
    g.box(PLUM, (0, 0, -70.0), (900.0, 70.0, 220.0), glow=WG)
    g.box(LAV, (0, 0, 135.0), (900.0, 56.0, 190.0), glow=WG)
    g.box(CREAM, (0, 0, 238.0), (910.0, 70.0, 16.0), glow=WG)
    crenels(g, 0, 0, 246.0, 780.0, 8, depth=56.0, h=46.0)
    moon(g, NAVY, -8.0, -31.0, 140.0, 52.0, thick=8.0)
    star(g, NAVY, -250.0, -31.0, 140.0, 42.0, thick=8.0)
    star(g, NAVY, 250.0, -31.0, 140.0, 42.0, thick=8.0)
    for sgn in (-1.0, 1.0):
        g.box(LAV, (sgn * 440.0, 0, 130.0), (84.0, 84.0, 300.0), glow=WG)
        g.box(CREAM, (sgn * 440.0, 0, 286.0), (100.0, 100.0, 14.0), glow=WG)
        g.cyl(PURPLE, (sgn * 440.0, 0, 293.0), 62.0, 66.0, steps=6, top_r=5.0)
    for k in range(9):                                                            # the rose
        x = -430.0 + k * 34.0 + (k % 3) * 9.0
        z = 30.0 + k * 22.0
        g.box(PINE, (x, -34.0, z), (40.0, 14.0, 40.0), rot=(0, 0, 20.0 * (k % 2)))
        if k % 2:
            g.box(MAG, (x + 8.0, -42.0, z + 10.0), (18.0, 8.0, 18.0))
    lantern(g, 390.0, -60.0, 250.0)
    for x in (-130.0, 130.0):                                                     # lavender at its foot
        g.cyl(PLUM, (x, -70.0, 0.0), 40.0, 56.0, steps=6, top_r=14.0)
        g.cyl(LAV, (x + 40.0, -80.0, 0.0), 30.0, 44.0, steps=6, top_r=10.0)


def armour(sc):
    """An EMPTY suit of armour on a plinth, hands on a sword, a shield with a crescent against the plinth, a cyan
    plume that stirs. A statue, toy-big like the park's own: 4.3 m."""
    g = sc.still()
    g.box(TIN_D, (0, 0, -60.0), (130.0, 130.0, 200.0))
    g.box(CREAM, (0, 0, 50.0), (116.0, 116.0, 100.0))
    g.box(PURPLE, (0, -59.0, 52.0), (70.0, 6.0, 50.0))
    g.box(CREAM, (0, 0, 106.0), (136.0, 136.0, 12.0))
    z0 = 112.0
    for sgn in (-1.0, 1.0):
        g.box(TIN_D, (sgn * 20.0, -6.0, z0 + 6.0), (24.0, 44.0, 12.0))           # sabatons
        g.box(TIN, (sgn * 20.0, 0, z0 + 46.0), (22.0, 24.0, 70.0))               # greaves
        g.box(TIN_D, (sgn * 20.0, -4.0, z0 + 84.0), (26.0, 28.0, 12.0))          # knees
        g.box(TIN, (sgn * 21.0, 0, z0 + 112.0), (25.0, 27.0, 46.0))              # cuisses
        g.sphere(TIN_D, (sgn * 44.0, 0, z0 + 208.0), 21.0, steps=(3, 6))         # pauldrons
        tilt_box(g, TIN, (sgn * 46.0, 0, z0 + 200.0), (sgn * 38.0, -30.0, z0 + 152.0), 16.0, 16.0)
        tilt_box(g, TIN, (sgn * 38.0, -30.0, z0 + 152.0), (sgn * 6.0, -40.0, z0 + 132.0), 15.0, 15.0)
    g.cyl(TIN_D, (0, 0, z0 + 132.0), 40.0, 22.0, steps=8, top_r=33.0)            # the skirt of tassets
    g.cyl(TIN, (0, 0, z0 + 154.0), 30.0, 62.0, steps=8, top_r=44.0)              # the breastplate
    g.box(GOLD, (0, -34.0, z0 + 186.0), (16.0, 6.0, 16.0), rot=(45, 0, 0))
    g.cyl(TIN_D, (0, 0, z0 + 216.0), 16.0, 12.0, steps=6)                         # gorget
    g.cyl(TIN, (0, 0, z0 + 226.0), 24.0, 30.0, steps=8)                           # the helm
    g.cyl(TIN, (0, 0, z0 + 256.0), 24.0, 18.0, steps=8, top_r=8.0)
    g.box(NAVY, (0, -22.0, z0 + 244.0), (30.0, 8.0, 7.0))                         # the visor's dark slit: nobody inside
    g.box(TIN_D, (0, -25.0, z0 + 234.0), (8.0, 8.0, 22.0))
    g.box(TIN, (0, -42.0, z0 + 62.0), (9.0, 6.0, 140.0))                          # the sword, point down
    g.box(GOLD, (0, -42.0, z0 + 132.0), (48.0, 8.0, 8.0))
    g.box(GOLD, (0, -42.0, z0 + 146.0), (8.0, 8.0, 22.0))
    tilt_box(g, PURPLE, (58.0, -66.0, 8.0), (66.0, -50.0, 96.0), 64.0, 8.0)       # the shield
    f = sc.part(P.motion(swing=8.0, pivot=(0.0, 4.0, z0 + 272.0), axis=0.0, rate=0.5, phase=0.3))
    tilt_box(f, CYAN, (0, 4.0, z0 + 270.0), (0, 30.0, z0 + 316.0), 12.0, 12.0)
    tilt_box(f, CYAN, (0, 30.0, z0 + 316.0), (0, 52.0, z0 + 300.0), 10.0, 10.0)


def banner_pole(sc):
    """A banner pole, 9.6 m: a striped foot, a gold crossbar with a long cyan gonfalon (a cream star, a purple band,
    a swallow tail) that swings, a second short one in cream behind, a gold crescent on the point, a pennant."""
    g = sc.still()
    g.cyl(TIN_D, (0, 0, -80.0), 46.0, 120.0, steps=8)
    g.cyl(CREAM, (0, 0, 40.0), 38.0, 40.0, steps=8)
    g.cyl(PURPLE, (0, 0, 80.0), 30.0, 30.0, steps=8, top_r=14.0)
    g.cyl(TIN, (0, 0, 80.0), 10.0, 800.0, steps=6)
    g.cyl(GOLD, (0, 0, 620.0), 14.0, 16.0, steps=6)
    g.sphere(GOLD, (0, 0, 884.0), 13.0, steps=(3, 6))
    moon(g, GOLD, 0, 0, 934.0, 36.0, thick=8.0)
    tilt_box(g, GOLD, (0, 0, 780.0), (0, -70.0, 800.0), 8.0, 8.0)
    banner(sc, 0.0, -70.0, 800.0, w=120.0, length=400.0, a=0.0, phase=0.0, swing=6.0, rate=0.4)
    tilt_box(g, GOLD, (0, 0, 620.0), (0, 60.0, 636.0), 8.0, 8.0)
    banner(sc, 0.0, 60.0, 636.0, w=90.0, length=240.0, a=0.0, phase=0.45, swing=6.0, rate=0.4, col=CREAM, mark=CYAN)
    f = sc.part(P.motion(swing=18.0, pivot=(0.0, 0.0, 860.0), axis=P.UP, rate=0.45, phase=0.2))
    f.box(CYAN, (52.0, 0, 852.0), (96.0, 5.0, 30.0))


def well(sc):
    """A wishing well under a witch hat: a round stone wall, two posts, a windlass, a bucket on its rope that swings a
    little, pots of flowers. 4.4 m."""
    g = sc.still()
    g.cyl(TIN_D, (0, 0, -80.0), 130.0, 100.0, steps=10)
    g.cyl(TIN, (0, 0, 20.0), 112.0, 80.0, steps=10)
    g.cyl(CREAM, (0, 0, 100.0), 122.0, 14.0, steps=10)
    g.cyl(NAVY, (0, 0, 102.0), 84.0, 14.0, steps=10)
    for sgn in (-1.0, 1.0):
        g.box(WOOD_D, (sgn * 100.0, 0, 200.0), (16.0, 16.0, 200.0))
        tilt_box(g, WOOD_D, (sgn * 100.0, 0, 250.0), (sgn * 60.0, 0, 300.0), 10.0, 10.0)
    g.box(WOOD_D, (0, 0, 300.0), (230.0, 16.0, 16.0))
    rod(g, WOOD, (-70.0, 0, 250.0), (70.0, 0, 250.0), 14.0, steps=6)
    tilt_box(g, TIN_D, (104.0, 0, 250.0), (140.0, 0, 250.0), 6.0, 6.0)            # the crank
    g.box(TIN_D, (140.0, 0, 232.0), (6.0, 6.0, 40.0))
    g.cyl(PURPLE, (0, 0, 300.0), 176.0, 60.0, steps=8, top_r=96.0)                # the hat
    g.cyl(PURPLE, (0, 0, 360.0), 96.0, 76.0, steps=8, top_r=5.0)
    g.cyl(GOLD, (0, 0, 352.0), 100.0, 10.0, steps=8)
    star(g, GOLD, 0, 0, 462.0, 22.0, thick=8.0)
    g.cyl(GOLD, (0, 0, 430.0), 4.0, 16.0, steps=5)
    f = sc.part(P.motion(swing=6.0, pivot=(0.0, 0.0, 250.0), axis=0.0, rate=0.35, phase=0.5))
    f.box(CREAM, (0, 0, 196.0), (5.0, 5.0, 100.0))
    f.cyl(WOOD, (0, 0, 118.0), 18.0, 30.0, steps=6, top_r=22.0)
    for k, (x, y) in enumerate(((-150.0, -90.0), (150.0, -100.0), (-40.0, -150.0))):
        g.cyl(WOOD, (x, y, 0.0), 22.0, 30.0, steps=6, top_r=27.0)
        g.sphere(MAG if k != 1 else CREAM, (x, y, 44.0), 22.0, steps=(3, 6))


def moon_dial(sc):
    """A stone moon dial, LOW (1.8 m): two round steps, a short column, a dial plate with its hours, a gold crescent
    for the gnomon, lavender round its foot. Unlit, still."""
    g = sc.still()
    g.cyl(TIN_D, (0, 0, -60.0), 120.0, 80.0, steps=10)
    g.cyl(CREAM, (0, 0, 20.0), 92.0, 22.0, steps=10)
    g.cyl(TIN, (0, 0, 42.0), 36.0, 58.0, steps=8, top_r=28.0)
    g.cyl(CREAM, (0, 0, 100.0), 74.0, 12.0, steps=10)
    for k in range(8):
        px, py = _ring(k * 45.0, 58.0, 0, 0)
        g.box(PURPLE, (px, py, 113.0), (8.0, 16.0, 4.0), rot=(0, k * 45.0, 0))
    moon(g, GOLD, 0, 0, 148.0, 32.0, thick=8.0)
    for k in range(5):
        px, py = _ring(k * 72.0 + 20.0, 118.0, 0, 0)
        g.cyl(PLUM, (px, py, 0.0), 26.0, 44.0, steps=5, top_r=8.0)
        g.cyl(LAV, (px + 18.0, py + 10.0, 0.0), 16.0, 34.0, steps=5, top_r=6.0)


def topiary_knight(sc):
    """A knight on a horse clipped out of yew, in a striped tub: 4.5 m. Only the pennon on his lance is cloth; it stirs."""
    g = sc.still()
    g.cyl(TIN_D, (0, 0, -80.0), 100.0, 90.0, steps=8)
    g.cyl(CREAM, (0, 0, 10.0), 92.0, 70.0, steps=8, top_r=104.0)
    for k in range(4):
        px, py = _ring(k * 90.0 + 45.0, 99.0, 0, 0)
        g.box(PURPLE, (px, py, 46.0), (44.0, 10.0, 62.0), rot=(0, k * 90.0 + 45.0, 0))
    g.cyl(WOOD_D, (0, 0, 80.0), 14.0, 80.0, steps=6)
    # the horse, heading +X
    g.box(PINE, (0, 0, 250.0), (230.0, 84.0, 96.0))
    g.box(LAWN, (0, 0, 300.0), (200.0, 70.0, 10.0))
    for x in (-88.0, 88.0):
        for y in (-26.0, 26.0):
            g.box(PINE, (x, y, 180.0), (30.0, 28.0, 70.0))
    tilt_box(g, PINE, (96.0, 0, 270.0), (160.0, 0, 360.0), 62.0, 60.0)
    g.box(PINE, (188.0, 0, 372.0), (96.0, 52.0, 50.0))
    g.box(LAWN, (150.0, 0, 404.0), (18.0, 40.0, 22.0))
    tilt_box(g, PINE, (-112.0, 0, 286.0), (-170.0, 0, 210.0), 24.0, 24.0)
    # the rider
    g.box(PINE, (-6.0, 0, 340.0), (64.0, 110.0, 44.0))
    g.cyl(PINE, (-6.0, 0, 340.0), 34.0, 76.0, steps=6, top_r=40.0)
    g.sphere(PINE, (-6.0, 0, 440.0), 28.0, steps=(4, 6))
    g.cyl(LAWN, (-6.0, 0, 458.0), 20.0, 22.0, steps=6, top_r=5.0)
    tilt_box(g, PINE, (-6.0, -44.0, 396.0), (60.0, -52.0, 350.0), 18.0, 18.0)
    tilt_box(g, WOOD_D, (40.0, -54.0, 300.0), (150.0, -54.0, 520.0), 8.0, 8.0)   # the lance
    f = sc.part(P.motion(swing=14.0, pivot=(146.0, -54.0, 500.0), axis=P.UP, rate=0.45, phase=0.6))
    f.box(CYAN, (106.0, -54.0, 494.0), (76.0, 5.0, 30.0))


def market_cart(sc):
    """A market cart, LOW (1.9 m): two spoked wheels, a plank bed, a striped canopy on four posts, loaves, jugs and
    bunches of lavender on it; its shafts rest on a trestle. Unlit, still. (Cream and purple: nothing of a pickup.)"""
    g = sc.still()
    for sgn in (-1.0, 1.0):
        rod(g, WOOD_D, (40.0, sgn * 62.0, 50.0), (40.0, sgn * 72.0, 50.0), 50.0, steps=10)
        rod(g, CREAM, (40.0, sgn * 72.0, 50.0), (40.0, sgn * 76.0, 50.0), 14.0, steps=6)
        g.box(WOOD, (40.0, sgn * 74.0, 50.0), (84.0, 5.0, 8.0))
        g.box(WOOD, (40.0, sgn * 74.0, 50.0), (8.0, 5.0, 84.0))
        tilt_box(g, WOOD_D, (-110.0, sgn * 40.0, 78.0), (-230.0, sgn * 30.0, 62.0), 8.0, 8.0)   # the shafts
    g.box(WOOD_D, (-226.0, 0, 30.0), (10.0, 80.0, 60.0))                          # the trestle
    g.box(WOOD, (20.0, 0, 84.0), (270.0, 118.0, 14.0))                            # the bed
    g.box(WOOD_D, (20.0, -56.0, 100.0), (270.0, 8.0, 22.0))
    g.box(WOOD_D, (20.0, 56.0, 100.0), (270.0, 8.0, 22.0))
    for x in (-104.0, 144.0):
        for y in (-52.0, 52.0):
            g.box(WOOD_D, (x, y, 136.0), (8.0, 8.0, 90.0))
    for k in range(6):                                                            # the canopy, cream and purple
        g.box(CREAM if k % 2 == 0 else PURPLE, (-110.0 + k * 52.0, 0, 184.0), (52.0, 136.0, 8.0))
    g.box(GOLD, (20.0, -68.0, 176.0), (312.0, 5.0, 8.0))
    for k in range(3):                                                            # loaves, jugs, lavender
        g.cyl(WOOD, (-70.0 + k * 34.0, -20.0, 91.0), 14.0, 12.0, steps=6, top_r=9.0)
    for k in range(2):
        g.cyl(TIN, (60.0 + k * 40.0, 20.0, 91.0), 13.0, 30.0, steps=6, top_r=8.0)
    for k in range(3):
        g.cyl(PLUM, (40.0 + k * 30.0, -30.0, 91.0), 9.0, 34.0, steps=5, top_r=14.0)
    g.box(CREAM, (-40.0, 26.0, 99.0), (60.0, 40.0, 16.0))


def moon_lamp(sc):
    """A lamp post, 4.6 m: a purple post on a stone foot, a crescent bracket, a small cream star lantern hanging from
    it on a chain (steady light, pale), a second moon on the point."""
    g = sc.still()
    g.cyl(TIN_D, (0, 0, -80.0), 40.0, 110.0, steps=8)
    g.cyl(CREAM, (0, 0, 30.0), 30.0, 26.0, steps=8)
    g.cyl(PURPLE, (0, 0, 56.0), 11.0, 370.0, steps=6)
    g.cyl(GOLD, (0, 0, 240.0), 15.0, 12.0, steps=6)
    g.cyl(GOLD, (0, 0, 420.0), 15.0, 12.0, steps=6)
    moon(g, GOLD, 30.0, 0, 388.0, 50.0, thick=8.0)                                # the bracket: a crescent, horns to +X
    g.cyl(PURPLE, (0, 0, 426.0), 16.0, 34.0, steps=6, top_r=5.0)
    f = sc.part(P.motion(swing=5.0, pivot=(62.0, 0.0, 424.0), axis=90.0, rate=0.25, phase=0.2))
    f.box(GOLD, (62.0, 0, 396.0), (5.0, 5.0, 56.0))
    star(f, CREAM, 62.0, 0, 346.0, 26.0, thick=14.0, glow=LAMP)
    f.box(PURPLE, (62.0, 0, 370.0), (16.0, 16.0, 6.0))


# ------------------------------------------------------------------------------------------ THE HERO
def crescent_keep(sc):
    """THE CRESCENT KEEP: a keep whose hat stands 76 m up, carrying a softly lit crescent moon 30 m across like a
    sail (its top 80 m up); under it a great clock face whose ONE hand creeps; four thin towers round its foot,
    joined to it by bridges; long banners; five gold stars hang from the moon's upper horn and sway."""
    g = sc.still()
    S = 12
    g.cyl(PLUM, (0, 0, -300.0), 1500.0, 560.0, steps=S, top_r=1380.0, glow=WG)    # the rock it stands on
    g.cyl(CREAM, (0, 0, 260.0), 1400.0, 40.0, steps=S, glow=WG)
    g.cyl(LAV, (0, 0, 300.0), 900.0, 3900.0, steps=S, top_r=780.0, glow=WG)       # the keep
    g.cyl(CREAM, (0, 0, 2200.0), 850.0, 50.0, steps=S, glow=WG)
    g.cyl(LAV, (0, 0, 4200.0), 780.0, 500.0, steps=S, top_r=1060.0, glow=WG)
    g.cyl(CREAM, (0, 0, 4700.0), 1100.0, 60.0, steps=S, glow=WG)
    g.cyl(LAV, (0, 0, 4760.0), 1060.0, 420.0, steps=S, glow=WG)
    for k in range(S):
        a = (k + 0.5) * 30.0
        px, py = _ring(a, 1020.0, 0, 0)
        g.box(CREAM, (px, py, 5270.0), (300.0, 80.0, 180.0), rot=(0, a, 0), glow=WG)
    # windows: three rings of five, gold and lit, on the faces towards the road
    for row, z in enumerate((1000.0, 1750.0, 2900.0, 3650.0)):
        rr = 900.0 - 120.0 * (z - 300.0) / 3900.0
        for a in (-75.0, -45.0, -15.0, 15.0, 45.0, 75.0):
            if row % 2 == 1 and abs(a) > 50.0:
                continue
            window(g, 0, 0, z, rr * 0.966, a, 110.0, 300.0, lit=WIN, fine=False)
    # the great door and its drawbridge
    g.box(CREAM, (0, -880.0, 620.0), (420.0, 40.0, 660.0), glow=WG)
    g.box(CREAM, (0, -880.0, 950.0), (297.0, 40.0, 297.0), rot=(45, 0, 0), glow=WG)
    g.box(NAVY, (0, -896.0, 600.0), (330.0, 40.0, 600.0))
    g.box(NAVY, (0, -896.0, 900.0), (233.0, 40.0, 233.0), rot=(45, 0, 0))
    g.box(WOOD_D, (0, -1150.0, 290.0), (330.0, 520.0, 30.0))
    # the clock stage and its hat
    g.cyl(LAV, (0, 0, 5180.0), 640.0, 1350.0, steps=S, top_r=600.0, glow=WG)
    g.cyl(LAV, (0, 0, 6530.0), 600.0, 220.0, steps=S, top_r=800.0, glow=WG)
    g.cyl(CREAM, (0, 0, 6750.0), 830.0, 50.0, steps=S, glow=WG)
    g.cyl(PURPLE, (0, 0, 6800.0), 900.0, 380.0, steps=S, top_r=380.0, glow=WG * 0.6)
    g.cyl(PURPLE, (0, 0, 7180.0), 380.0, 440.0, steps=S, top_r=8.0, glow=WG * 0.6)
    g.cyl(GOLD, (0, 0, 7170.0), 400.0, 24.0, steps=S, glow=0.3)
    g.sphere(GOLD, (0, 0, 7640.0), 40.0, steps=(4, 8), glow=0.5)
    # the clock: a cream face, a gold rim, twelve marks, one hand
    rod(g, GOLD, (0, -600.0, 5900.0), (0, -640.0, 5900.0), 520.0, steps=16, glow=0.3)
    rod(g, CREAM, (0, -640.0, 5900.0), (0, -660.0, 5900.0), 470.0, steps=16, glow=0.4)
    for k in range(12):
        a = math.radians(k * 30.0)
        sz = (56.0, 16.0, 56.0)
        if k % 3 == 0:
            sz = (46.0, 16.0, 130.0) if k % 6 == 0 else (130.0, 16.0, 46.0)
        g.box(PURPLE, (400.0 * math.sin(a), -666.0, 5900.0 + 400.0 * math.cos(a)), sz)
    hnd = sc.part(P.motion(swing=160.0, pivot=(0.0, -676.0, 5900.0), axis=90.0, rate=0.008, phase=0.1))
    hnd.box(NAVY, (0, -678.0, 6060.0), (44.0, 16.0, 400.0))
    hnd.box(NAVY, (0, -678.0, 6250.0), (90.0, 16.0, 90.0), rot=(45, 0, 0))
    hnd.box(NAVY, (0, -678.0, 5840.0), (30.0, 16.0, 120.0))
    rod(g, GOLD, (0, -676.0, 5900.0), (0, -704.0, 5900.0), 40.0, steps=8, glow=0.4)
    # THE MOON: behind the clock stage, the keep's hat rising through its hollow
    MX, MY, MZ, MR = -330.0, 300.0, 6500.0, 1500.0
    g.side(CREAM, moon_pts(MR, n=10), -70.0, 70.0, glow=MOON, loc=(MX, MY, MZ))
    g.side(GOLD, moon_pts(MR * 0.93, n=10), -80.0, -66.0, glow=MOON * 0.8, loc=(MX - 30.0, MY, MZ))
    tilt_box(g, GOLD, (-560.0, 150.0, 5600.0), (-900.0, 300.0, 5560.0), 40.0, 40.0)        # two stays to the keep
    tilt_box(g, GOLD, (-300.0, 100.0, 7000.0), (-760.0, 300.0, 7400.0), 30.0, 30.0)
    r = random.Random(6)
    for k, ln in enumerate((120.0, 330.0, 170.0, 380.0, 220.0)):                  # the hanging stars, under the upper horn
        hx = 300.0 + 85.0 * k
        hz = MZ + math.sqrt(max((0.82 * MR) ** 2 - (hx - MX - 0.35 * MR) ** 2, 0.0)) + 20.0
        f = sc.part(P.motion(swing=4.0, pivot=(hx, MY, hz), axis=90.0, rate=0.18 + 0.03 * k, phase=r.random()))
        f.box(GOLD, (hx, MY - 80.0, hz - ln / 2.0), (8.0, 8.0, ln))
        star(f, GOLD, hx, MY - 80.0, hz - ln - 60.0, 80.0, thick=20.0, glow=0.9)
    # four thin towers round the foot, a bridge from each to the keep
    for k, (tx, ty, h) in enumerate(((-1180.0, -520.0, 1700.0), (1180.0, -520.0, 2100.0), (-1100.0, 640.0, 2400.0), (1100.0, 640.0, 1500.0))):
        zt, tip = tower(g, tx, ty, 190.0, h, 0, 760.0, foot=300.0, steps=6, merlons=False, head_win=False)
        for z in (h * 0.45, h * 0.8):
            window(g, tx, ty, z, 190.0 * 0.93, -22.5 if tx < 0 else 22.5, 76.0, 170.0, fine=False)
        zb = h * 0.62
        ex = 760.0 if tx > 0 else -760.0
        tilt_box(g, CREAM, (tx, ty, zb), (ex * 0.98, ty * 0.45, zb + 120.0), 110.0, 60.0, glow=WG)
        tilt_box(g, LAV, (tx, ty, zb + 70.0), (ex * 0.98, ty * 0.45, zb + 190.0), 90.0, 80.0, glow=WG)
        if k % 2 == 0:
            pennant(sc, tx, ty, tip, 260.0, phase=0.2 * k, mast=220.0)
        else:
            vane(sc, tx, ty, tip, 'star', 90.0, phase=0.3 * k)
    for sgn in (-1.0, 1.0):                                                       # two great banners down the front
        px, py = _ring(sgn * 30.0, 860.0, 0, 0)
        banner(sc, px, py, 4150.0, w=260.0, length=1500.0, a=sgn * 30.0, phase=0.3 + 0.2 * sgn, swing=2.0, rate=0.2)


JOBS = [
    ('SM_S1D6_RoundTower_A', round_tower_a), ('SM_S1D6_RoundTower_B', round_tower_b), ('SM_S1D6_TwinTurrets', twin_turrets),
    ('SM_S1D6_Gatehouse', gatehouse), ('SM_S1D6_Astronomer', astronomer), ('SM_S1D6_BellTower', bell_tower),
    ('SM_S1D6_BridgeHouse', bridge_house), ('SM_S1D6_BannerHall', banner_hall), ('SM_S1D6_Dovecote', dovecote),
    ('SM_S1D6_GardenWall', garden_wall), ('SM_S1D6_Armour', armour), ('SM_S1D6_BannerPole', banner_pole), ('SM_S1D6_Well', well),
    ('SM_S1D6_MoonDial', moon_dial, STILL), ('SM_S1D6_TopiaryKnight', topiary_knight), ('SM_S1D6_MarketCart', market_cart, STILL),
    ('SM_S1D6_MoonLamp', moon_lamp),
    ('SM_S1D6_CrescentKeep', crescent_keep, dict(far=True, grow=(120, 120, 60), grow_down=(120, 120, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
