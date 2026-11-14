"""Level 2: the Volta Chemical Works. Our own industrial set, built the same way as ft_city:
origin at ground level, X away from the camera, Y right, Z up, centimetres. Props face -X.

The palette is night-shift industrial: deep blue steel, teal tanks, cyan glass, hazard yellow and a
violet chemical that glows. Nothing here is traced from any game; it is a generic chemical plant
(storage tanks, gas spheres, stacks, distillation towers, pipework and vats) in the FULL THROTTLE cel look.
"""
import math
import ft_art
from ft_car import wheel, BLACK, GLASS, CHROME, STEEL, TYRE, GOLD

STEELB = '#2a3f8f'   # deep blue steel
STEELB_D = '#1c2a66'
TEAL = '#19b5c9'
TEAL_D = '#12849a'
CYAN = '#5ff0ff'
HAZ = '#ffd23f'      # hazard yellow
NAVY = '#1c1f4a'
MAG = '#e84aff'      # magenta trim
VIOLET = '#b44dff'   # the chemical
WHITE = '#eef6ff'
ORANGE = '#ff9e3d'
RED = '#ff3355'


def ladder(m, x, y, z0, z1, col=HAZ):
    for s in (-1, 1):
        m.box(col, (x, y + s * 22, (z0 + z1) / 2.0), (6, 6, z1 - z0))
    k = z0 + 30
    while k < z1:
        m.box(col, (x, y, k), (5, 44, 4))
        k += 50


def bands(m, r, z0, z1, col, n, steps=20, w=40):
    for k in range(n):
        z = z0 + (k + 0.5) * (z1 - z0) / n
        m.cyl(col, (0, 0, z - w / 2.0), r + 4, w, steps=steps)


def tank(m, r=520, h=1500, col=TEAL, trim=HAZ):
    """A big upright storage tank: dome roof, hazard bands, a ladder and a catwalk ring."""
    m.cyl(NAVY, (0, 0, 0), r + 40, 40, steps=24)
    m.cyl(col, (0, 0, 40), r, h, steps=24)
    bands(m, r, 40, h + 40, TEAL_D if col == TEAL else STEELB_D, 4, steps=24, w=26)
    m.cyl(trim, (0, 0, h * 0.62), r + 6, 60, steps=24)
    for k in range(12):
        a = 2 * math.pi * k / 12
        m.box(BLACK if k % 2 else trim, (math.cos(a) * (r + 7), math.sin(a) * (r + 7), h * 0.62 + 30), (40, 40, 60), rot=(0, math.degrees(a), 0))
    m.sphere(col, (0, 0, h + 40), r, scale=(1, 1, 0.32), steps=(8, 24))
    # catwalk ring and rail at the top
    m.cyl(STEELB, (0, 0, h + 30), r + 70, 14, steps=24)
    m.cyl(HAZ, (0, 0, h + 110), r + 72, 8, steps=24)
    for k in range(16):
        a = 2 * math.pi * k / 16
        m.box(HAZ, (math.cos(a) * (r + 70), math.sin(a) * (r + 70), h + 75), (6, 6, 80))
    ladder(m, -r - 30, -r * 0.35, 40, h + 40)
    # a lit sign band on the front
    m.box(CYAN, (-r - 6, 0, h * 0.35), (6, r * 0.9, 90), glow=0.6)
    m.box(NAVY, (-r - 8, 0, h * 0.35), (4, r * 0.7, 40))


def sphere(m, r=620, col=MAG):
    """A gas sphere on legs, with a girder skirt and a stair spiral."""
    cz = r + 420
    for k in range(8):
        a = 2 * math.pi * k / 8 + 0.2
        x, y = math.cos(a) * r * 0.82, math.sin(a) * r * 0.82
        m.box(STEELB, (x, y, cz / 2.0), (34, 34, cz))
        m.box(HAZ, (x, y, 30), (40, 40, 60))
    m.cyl(STEELB_D, (0, 0, 300), r * 0.84, 18, steps=8)
    m.sphere(col, (0, 0, cz), r, steps=(12, 24))
    m.cyl(WHITE, (0, 0, cz - 20), r + 3, 40, steps=24)
    m.cyl(STEELB, (0, 0, cz + r - 30), 90, 60, steps=12)
    m.cyl(RED, (0, 0, cz + r + 30), 20, 40, steps=8, glow=1.2)


def stack(m, r=200, h=3600):
    """A tapering smokestack in hazard bands with a red beacon and a little smoke."""
    m.cyl(NAVY, (0, 0, 0), r + 80, 120, steps=16)
    n = 9
    for k in range(n):
        r0 = r - k * r * 0.35 / n
        r1 = r - (k + 1) * r * 0.35 / n
        m.cyl(WHITE if k % 2 == 0 else ORANGE, (0, 0, 120 + k * h / n), r0, h / n, steps=16, top_r=r1)
    m.cyl(NAVY, (0, 0, 120 + h), r * 0.7, 60, steps=16)
    for z in (h * 0.45, h * 0.85):
        m.cyl(HAZ, (0, 0, 120 + z), r * 0.85, 12, steps=16)
    m.sphere(RED, (0, 0, 120 + h + 90), 40, glow=1.4)
    for k, (dz, rr) in enumerate([(260, 160), (480, 220), (720, 280)]):
        m.sphere('#d8d0f0', (40 * k, -60 * k, 120 + h + dz), rr, steps=(6, 10))


def tower(m, r=240, h=3000):
    """A distillation column: a tall teal cylinder with stacked platforms, rails and side pipes."""
    m.cyl(NAVY, (0, 0, 0), r + 60, 60, steps=16)
    m.cyl(TEAL, (0, 0, 60), r, h, steps=16)
    m.sphere(TEAL, (0, 0, h + 60), r, scale=(1, 1, 0.5), steps=(6, 16))
    for k in range(5):
        z = 400 + k * (h - 500) / 4.0
        m.cyl(STEELB, (0, 0, z), r + 120, 16, steps=16)
        m.cyl(HAZ, (0, 0, z + 80), r + 122, 8, steps=16)
        m.box(CYAN, (-r - 4, 0, z + 140), (6, 80, 50), glow=0.5)
    for s in (-1, 1):
        m.cyl(STEEL, (0, s * (r + 60), 60), 34, h * 0.9, steps=10)
        m.cyl(VIOLET, (0, s * (r + 60), h * 0.9 + 60), 36, 30, steps=10, glow=0.6)
    ladder(m, -r - 20, r * 0.4, 60, h + 40)


def pipework(m, w=1300, h=1000):
    """A tangle of pipes on a girder frame: two big runs, elbows down to the ground, valves."""
    for y in (-w / 2.0, w / 2.0):
        m.box(STEELB, (0, y, h / 2.0), (40, 40, h))
    m.box(STEELB, (0, 0, h), (44, w + 40, 40))
    m.box(STEELB, (0, 0, h * 0.55), (40, w + 40, 30))
    for k in range(4):
        y0 = -w / 2.0 + w * k / 4.0
        m.box(STEELB_D, (0, y0 + w / 8.0, h * 0.78), (20, w / 4.0 * 1.12, 20), rot=(0, 0, 45 if k % 2 else -45))
    # the pipe runs, across the frame
    for z, rr, col in ((h + 70, 60, TEAL), (h * 0.55 + 60, 42, HAZ), (h * 0.55 - 60, 34, MAG)):
        m.cyl(col, (-40, -w / 2.0 - 120, z), rr, w + 240, rot=(0, 0, 90), steps=14)
        for y in (-w / 2.0 + 40, w / 2.0 - 40):
            m.cyl(BLACK, (-40, y, z), rr + 10, 30, rot=(0, 0, 90), steps=14)
    # elbows down to the ground at each end
    for s in (-1, 1):
        y = s * (w / 2.0 + 120)
        m.cyl(TEAL, (-40, y, 0), 60, h + 70, steps=14)
        m.sphere(TEAL, (-40, y, h + 70), 60, steps=(6, 14))
    # valve wheels
    for y in (-w * 0.2, w * 0.25):
        m.cyl(RED, (-110, y, h * 0.55 + 60), 50, 10, rot=(90, 0, 0), steps=12)
        m.cyl(HAZ, (-100, y, h * 0.55 + 60), 12, 30, rot=(90, 0, 0), steps=8)
    m.box(CYAN, (-30, 0, h + 140), (6, w * 0.5, 50), glow=0.5)


def vat(m, r=650, h=380):
    """An open vat of glowing violet chemical with a hazard rim and a stirrer gantry."""
    m.cyl(STEELB, (0, 0, 0), r, h, steps=28)
    bands(m, r, 0, h, STEELB_D, 2, steps=28, w=24)
    m.cyl(HAZ, (0, 0, h), r + 6, 24, steps=28)
    m.cyl(VIOLET, (0, 0, h + 24), r - 34, 6, steps=28, glow=1.1)
    for k in range(14):
        a = 2 * math.pi * k / 14
        m.box(BLACK, (math.cos(a) * (r + 8), math.sin(a) * (r + 8), h + 12), (60, 50, 26), rot=(0, math.degrees(a) + 30, 0))
    for s in (-1, 1):
        m.box(STEELB, (0, s * (r + 40), (h + 500) / 2.0), (40, 40, h + 500))
    m.box(STEELB, (0, 0, h + 500), (60, 2 * r + 120, 50))
    m.box(HAZ, (0, 0, h + 530), (64, 2 * r + 124, 12))
    m.cyl(STEEL, (0, 0, h + 20), 24, 480, steps=8)
    m.box(STEEL, (0, 0, h + 40), (300, 30, 20))
    # bubbles
    for k, (x, y, rr) in enumerate([(-200, 120, 60), (150, -220, 46), (60, 260, 38), (-80, -90, 30)]):
        m.sphere('#e3b8ff', (x, y, h + 30), rr, scale=(1, 1, 0.6), steps=(5, 10), glow=0.8)


def lamp(m, flip=1):
    """A plant floodlight: a girder mast with a sodium-orange head over the road."""
    m.cyl(NAVY, (0, 0, 0), 16, 40, steps=8)
    m.box(STEELB, (0, 0, 380), (22, 22, 720))
    for k in range(7):
        m.box(HAZ if k % 2 else STEELB_D, (0, 0, 60 + k * 100), (26, 26, 12))
    m.box(STEELB, (0, flip * 110, 730), (14, 240, 14))
    m.box(NAVY, (0, flip * 220, 712), (60, 80, 26))
    m.box(ORANGE, (0, flip * 220, 698), (52, 70, 4), glow=1.2)
    m.box(CYAN, (-14, 0, 440), (4, 34, 160), glow=0.5)


def tanker(m):
    """The level 2 boss: THE ALCHEMIST's chemical tanker. A wide navy cab-over rig hauling a fat
    hazard-striped tank of violet chemical, with vent stacks, a twin-gun turret and red eyes."""
    W, L = 64 * 9.14, 1250
    hw = W / 2 - 16
    # chassis
    m.box(NAVY, (L * 0.45, 0, 70), (L * 0.9, hw * 2, 60))
    for s in (-1, 1):
        m.box(RED, (-3, s * (hw - 20), 60), (2, 30, 22), glow=1.0)
        for x in (0.08, 0.2, 0.62, 0.9):
            wheel(m, L * x, 46, 46, hw - 40, W / 2.0, s)
    # the cab at the front
    m.side(STEELB, [(L * 0.8, 100), (L, 100), (L, 230), (L * 0.95, 290), (L * 0.8, 300)], -hw + 10, hw - 10)
    m.box(CYAN, (L * 0.8 + 2, 0, 250), (4, hw * 1.4, 40), glow=0.6)
    # the tank: a long cylinder along X, with hazard stripes and end caps
    R = hw * 0.95
    m.cyl(TEAL, (10, 0, 110 + R), R, L * 0.74, rot=(-90, 0, 0), steps=24)
    for k in range(5):
        m.cyl(HAZ if k % 2 == 0 else BLACK, (60 + k * L * 0.14, 0, 110 + R), R + 4, 40, rot=(-90, 0, 0), steps=24)
    m.cyl(STEELB_D, (0, 0, 110 + R), R * 0.92, 14, rot=(-90, 0, 0), steps=24)
    m.cyl(VIOLET, (-6, 0, 110 + R), R * 0.42, 8, rot=(-90, 0, 0), steps=20, glow=1.2)
    m.box(RED, (-8, 0, 110 + R * 0.3), (4, 70, 22), glow=1.4)
    # vent stacks venting chemical
    for x in (0.2, 0.5):
        m.cyl(STEEL, (L * x, R * 0.4, 110 + 2 * R - 20), 26, 120, steps=10)
        m.sphere(VIOLET, (L * x, R * 0.4, 110 + 2 * R + 110), 40, glow=1.0)
    # the turret and its red eyes
    m.box(STEELB, (L * 0.3, 0, 110 + 2 * R + 20), (220, 180, 50))
    m.cyl(STEELB, (L * 0.3, 0, 110 + 2 * R + 30), 80, 50, steps=16)
    for y in (-36, 36):
        m.cyl(BLACK, (L * 0.3 - 30, y, 110 + 2 * R + 60), 11, 280, rot=(90, 0, 0), steps=10)
    for y in (-60, 60):
        m.box(RED, (L * 0.3 - 92, y, 110 + 2 * R + 64), (4, 26, 10), glow=1.5)
    m.box(HAZ, (0, 0, 40), (6, W - 30, 20))


def skyline(m, width=24000, seed=11):
    """The far plant across the chemical lake: towers, stacks, spheres and gantries, with lit windows."""
    import random
    r = random.Random(seed)
    y = -width / 2.0 + 200
    blues = ['#33489e', '#2a3f8f', '#3a5ab8', '#24357a', '#4060c0']
    while y < width / 2.0 - 1400:
        kind = r.random()
        x = r.choice([0, 600, 1400, 2400])
        if kind < 0.35:   # a block with windows
            w = r.choice([700, 900, 1200])
            h = r.choice([1400, 2000, 2600, 3200])
            col = r.choice(blues)
            m.box(col, (x + 500, y + w / 2.0, h / 2.0), (1000, w, h))
            for f in range(int(h / 320)):
                m.box(CYAN if f % 3 else HAZ, (x - 2, y + w / 2.0, 220 + f * 320), (4, w * 0.7, 50), glow=0.5)
            if r.random() < 0.5:
                m.cyl(STEEL, (x + 500, y + w / 2.0, h), 14, 700, steps=6)
                m.sphere(RED, (x + 500, y + w / 2.0, h + 720), 40, glow=1.2)
        elif kind < 0.55:  # a stack
            w = 500
            h = r.choice([3600, 4400, 5200])
            for k in range(6):
                m.cyl(WHITE if k % 2 == 0 else ORANGE, (x + 300, y + w / 2.0, k * h / 6.0), 220 - k * 18, h / 6.0, steps=12, top_r=220 - (k + 1) * 18)
            m.sphere(RED, (x + 300, y + w / 2.0, h + 60), 50, glow=1.2)
        elif kind < 0.75:  # a sphere
            w = 1300
            rr = r.choice([450, 550, 650])
            m.sphere(r.choice([MAG, TEAL, '#ff7ad9']), (x + 700, y + w / 2.0, rr + 350), rr, steps=(8, 16))
            for s in (-1, 1):
                m.box(STEELB, (x + 700, y + w / 2.0 + s * rr * 0.7, 300), (30, 30, 600))
        else:              # a column with platforms
            w = 600
            h = r.choice([2800, 3400, 4000])
            m.cyl(TEAL, (x + 300, y + w / 2.0, 0), 200, h, steps=12)
            for k in range(4):
                m.cyl(STEELB, (x + 300, y + w / 2.0, 500 + k * (h - 600) / 3.0), 300, 20, steps=12)
                m.box(CYAN, (x + 90, y + w / 2.0, 560 + k * (h - 600) / 3.0), (4, 120, 40), glow=0.6)
        y += w + r.choice([0, 150, 400])
    # a long gantry with a tube running across the whole strip
    m.box(STEELB_D, (300, 0, 1500), (60, width * 0.96, 60))
    m.cyl(TEAL, (250, -width * 0.48, 1620), 70, width * 0.96, rot=(0, 0, 90), steps=12)
