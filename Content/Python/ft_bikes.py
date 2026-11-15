"""Faction motorcycles and seated riders. Centimetres, +X forward, +Z up.

Run export() in the editor to rebuild just the ten gameplay meshes.
"""

import math

import unreal

import ft_art


INK = '#22202a'
RUBBER = '#26232e'
TREAD = '#34313e'
FRAME = '#434653'
STEEL = '#8f9bb3'
CHROME = '#d7e3f0'
LAMP = '#fff3a8'
TAIL = '#ff3355'
SIDES = (-1, 1)

BAYSHORE = dict(bike='#3a3e4c', bike2='#252735', stripe='#ff4fb8', suit='#d0213a',
                suit2='#841f35', helmet='#181620', visor='#2ee6ff')
OUTLAWS = {
    1: dict(bike='#ff3355', bike2='#622438', stripe='#ffd23f', suit='#322832',
            suit2='#4a2f2a', helmet='#201b24', visor='#ffc93c', skin='#d79a68', hair='#3a2a22'),
    2: dict(bike='#7b3bd6', bike2='#291b4c', stripe='#2ee6ff', suit='#2a2832',
            suit2='#424154', helmet='#292331', visor='#2ee6ff', skin='#b77961', hair='#c9c4d6'),
}

# Fork crown, fuel cell rings (X, Z, half width, half height), hip X/Z,
# shoulder X/Z, and grip X/half width/Z. The pose follows these hard points.
SPORT = {
    0: ((164, 122), ((80, 103, 8, 6), (105, 111, 19, 16), (143, 107, 21, 19), (163, 102, 9, 8)),
        (73, 113), (109, 172), (157, 34, 138)),
    1: ((167, 130), ((83, 108, 12, 8), (102, 113, 23, 19), (153, 113, 21, 21), (173, 107, 11, 13)),
        (76, 119), (99, 171), (156, 38, 151)),
    2: ((168, 135), ((74, 106, 8, 6), (99, 113, 21, 16), (136, 115, 22, 19), (161, 109, 10, 10)),
        (74, 120), (89, 177), (152, 36, 154)),
    3: ((158, 115), ((73, 96, 8, 6), (107, 101, 24, 15), (148, 97, 24, 19), (166, 88, 11, 10)),
        (69, 108), (118, 160), (156, 36, 127)),
    4: ((166, 121), ((84, 102, 10, 7), (113, 108, 22, 17), (145, 103, 24, 18), (171, 98, 10, 9)),
        (74, 112), (102, 169), (153, 36, 143)),
    5: ((181, 123), ((77, 94, 19, 15), (103, 100, 26, 22), (158, 95, 30, 23), (182, 88, 15, 17)),
        (76, 113), (102, 168), (150, 39, 147)),
    6: ((165, 123), ((69, 103, 10, 9), (99, 110, 22, 22), (144, 108, 25, 24), (169, 98, 13, 15)),
        (72, 116), (101, 165), (153, 37, 137)),
    7: ((162, 117), ((76, 98, 10, 9), (105, 103, 21, 15), (148, 100, 25, 17), (174, 89, 12, 9)),
        (71, 108), (115, 161), (157, 35, 128)),
}


def along(m, color, start, end, radius, tip=None, steps=8, glow=0.0):
    """A round strut with independent lengths and radii at its endpoints."""
    dx, dy, dz = (b - a for a, b in zip(start, end))
    horizontal = math.hypot(dx, dy)
    length = math.hypot(horizontal, dz)
    rotation = (math.degrees(math.atan2(-horizontal, dz)), math.degrees(math.atan2(dy, dx)), 0)
    m.cyl(color, start, radius, length, rot=rotation, steps=steps, glow=glow, top_r=tip)


def between(start, end, fraction):
    return tuple(a + (b - a) * fraction for a, b in zip(start, end))


def shell(m, color, rows, close=False, glow=0.0):
    """Join corresponding vertices on cross-sections, with an optional closed profile."""
    sides = len(rows[0])
    triangles = []
    row_pairs = len(rows) if close else len(rows) - 1
    for row in range(row_pairs):
        first = row * sides
        second = ((row + 1) % len(rows)) * sides
        for side in range(sides):
            following = (side + 1) % sides
            triangles.extend(((first + side, first + following, second + following),
                              (first + side, second + following, second + side)))
    if not close:
        last = (len(rows) - 1) * sides
        for side in range(1, sides - 1):
            triangles.extend(((0, side + 1, side), (last, last + side, last + side + 1)))
    transform = m._t((0, 0, 0), (0, 0, 0))
    buffers = unreal.GeometryScriptSimpleMeshBuffers()
    buffers.vertices = [transform.transform_location(unreal.Vector(*point)) for row in rows for point in row]
    buffers.triangles = [unreal.IntVector(*tri) for tri in triangles]
    unreal.GeometryScript_MeshEdits.append_buffers_to_mesh(m._mesh(color, glow), buffers)


def coachwork(m, color, rings, center_y=0, glow=0.0):
    """Octagonal cross-sections give the panels broad faces with chamfered edges."""
    section = ((1, -.55), (1, .55), (.62, 1), (-.62, 1), (-1, .55), (-1, -.55), (-.62, -1), (.62, -1))
    shell(m, color, [[(x, center_y + width * y, z + height * h) for y, h in section]
                    for x, z, width, height in rings], glow=glow)


def ring_y(m, color, x, z, profile, steps=18):
    """Lathe a wheel cross-section in the X/Z plane around its axle."""
    angles = [2 * math.pi * i / steps for i in range(steps)]
    rows = [[(x + r * math.cos(a), y, z + r * math.sin(a)) for a in angles] for r, y in profile]
    shell(m, color, rows, close=True)


def wheel(m, x, z, radius, width, tread, spokes):
    half = width / 2
    inner = radius * .69
    shoulder = radius - 4
    ring_y(m, RUBBER, x, z, ((inner, -half * .8), (inner, half * .8),
                           (shoulder, half), (radius - 1.2, half * .64),
                           (radius - 1.2, -half * .64), (shoulder, -half)), steps=24)
    ring_y(m, FRAME, x, z, ((inner - 3, -half * .72), (inner - 3, half * .72),
                           (inner + 1, half * .72), (inner + 1, -half * .72)))
    for i in range(tread):
        angle = 2 * math.pi * i / tread
        m.box(TREAD, (x + (radius - 1.55) * math.cos(angle), 0, z + (radius - 1.55) * math.sin(angle)),
              (2.1, width * .82, 3.8), rot=(math.degrees(angle), 0, 0))
    along(m, STEEL, (x, -half - 2, z), (x, half + 2, z), 6.4)
    for i in range(spokes):
        a = (i + .26) * 2 * math.pi / spokes
        for side in SIDES:
            along(m, STEEL, (x + 3 * math.cos(a - .7), side * half * .33, z + 3 * math.sin(a - .7)),
                  (x + inner * math.cos(a), side * half * .58, z + inner * math.sin(a)),
                  1.6 if spokes < 7 else 1.25, 1.0, steps=5)
    rotor = radius * .51
    ring_y(m, CHROME, x, z, ((rotor - 3.3, half * .65), (rotor - 3.3, half * .83),
                            (rotor, half * .83), (rotor, half * .65)), steps=16)
    for i in range(4):
        a = i * math.pi / 2
        along(m, FRAME, (x, half * .72, z), (x + rotor * math.cos(a), half * .72, z + rotor * math.sin(a)),
              1.25, steps=5)
    m.box(FRAME, (x - rotor * .75, half * .85 + 1, z + rotor * .68), (7, 5, 8), rot=(35, 0, 0))


def linkage(m, axis, steering, width, metal):
    x, z = axis
    crown, top = steering
    for side in SIDES:
        fork_bottom = (x, side * width, z)
        fork_top = (crown, side * width, top)
        along(m, metal, fork_bottom, fork_top, 3.6)
        along(m, FRAME, fork_bottom, between(fork_bottom, fork_top, .54), 5.4, 4.6)
        along(m, INK, between(fork_bottom, fork_top, .53), between(fork_bottom, fork_top, .59), 5.0)
    for ratio in (.77, .98):
        brace = between((x, 0, z), (crown, 0, top), ratio)
        m.box(FRAME, brace, (13, (width + 4) * 2, 7))


def cockpit(m, steering, grip, tall=False):
    x, z = steering
    gx, gy, gz = grip
    along(m, FRAME, (x, 0, z - 5), (x - 5, 0, z + 12), 8, 6)
    for side in SIDES:
        base = (x - 3, side * 7, z + 4)
        elbow = (gx + 7, side * (13 if tall else gy * .55), gz + 1)
        end = (gx, side * gy, gz)
        along(m, CHROME, base, elbow, 2.75)
        along(m, CHROME, elbow, end, 2.75)
        along(m, INK, (gx, side * (gy - 5), gz), (gx, side * (gy + 5), gz), 4.2)
        along(m, CHROME, (gx + 4, side * (gy - 6), gz), (gx + 8, side * (gy + 4), gz - 2), 1.15, steps=5)
        bend = (x + 13, side * (gy * .45 + 3), z - 12)
        along(m, INK, (gx + 5, side * (gy - 7), gz), bend, 1.05, steps=5)
        along(m, INK, bend, (x + 2, side * 9, z - 32), 1.05, steps=5)
    m.box(INK, (x - 9, 0, z + 9), (13, 24, 7), rot=(-22, 0, 0))
    for side in SIDES:
        along(m, STEEL, (x - 10, side * 5, z + 11), (x - 10, side * 5, z + 14), 3.7)
        along(m, INK, (x - 10, side * 5, z + 14), (x - 10, side * 5, z + 14.5), 2.8)


def engine(m, x, z, wide=False):
    m.sphere(INK, (x, 0, z), 16, scale=(1.45, 1.17, .87), steps=(6, 10))
    for lean in (-1, 1):
        start = (x, 0, z + 4)
        end = (x + lean * (19 if wide else 12), 0, z + (40 if wide else 29))
        along(m, FRAME, start, end, 11, 10)
        for step in range(1, 5):
            lower = between(start, end, step * .19)
            upper = between(start, end, step * .19 + .055)
            along(m, STEEL, lower, upper, 12.5, steps=8)
    for side in SIDES:
        along(m, FRAME, (x - 4, side * 16, z), (x - 4, side * 23, z), 11, steps=12)
        along(m, STEEL, (x - 4, side * 23, z), (x - 4, side * 25, z), 5, steps=8)


def pipe(m, start, bend, outlet, color=STEEL, wide=False):
    along(m, color, start, bend, 3.7)
    m.sphere(color, bend, 3.7, steps=(5, 7))
    along(m, color, bend, outlet, 3.7, 4.8)
    muzzle = between(bend, outlet, .6)
    along(m, color, muzzle, outlet, 5.2, 6.4 if wide else 5.2)
    along(m, INK, outlet, (outlet[0] - .35, outlet[1], outlet[2]), 3.5)


def rear_arm(m, axle, mount, width):
    for side in SIDES:
        a = (axle[0], side * width, axle[1])
        b = (mount[0], side * width, mount[1])
        along(m, FRAME, a, b, 4.2, 5.3)
        shock_bottom = between(a, b, .22)
        shock_top = (b[0] - 13, side * (width + 1), b[2] + 36)
        along(m, STEEL, shock_bottom, shock_top, 2.6)
        for index in range(5):
            fraction = .21 + index * .12
            along(m, FRAME, between(shock_bottom, shock_top, fraction),
                  between(shock_bottom, shock_top, fraction + .055), 4.8, steps=6)
        along(m, STEEL, (a[0], side * width, a[2]), (a[0], side * (width + 3), a[2]), 3.5, steps=6)


def fender(m, color, x, z, radius, width, start, end):
    rings = []
    for i in range(7):
        a = math.radians(end + (start - end) * i / 6)
        c, s = math.cos(a), math.sin(a)
        span = width * (.63 if i in (0, 6) else 1)
        angles = ((radius + 4, -span), (radius + 6, -span * .65), (radius + 6, span * .65),
                  (radius + 4, span), (radius + 2.6, span), (radius + 4.4, span * .65),
                  (radius + 4.4, -span * .65), (radius + 2.6, -span))
        rings.append([(x + r * c, y, z + r * s) for r, y in angles])
    shell(m, color, rings)


def tail_weapon(m, x, y, z):
    """Low-profile rear-firing mount, breech and a recessed bore."""
    along(m, FRAME, (x + 22, y * .58, z - 14), (x + 13, y, z), 3.1, steps=6)
    m.box(INK, (x + 9, y, z), (20, 9, 10))
    along(m, FRAME, (x + 8, y, z), (x - 7, y, z), 3.1, steps=8)
    along(m, STEEL, (x - 3, y, z), (x - 9, y, z), 4.1, steps=8)
    along(m, INK, (x - 9, y, z), (x - 9.4, y, z), 2.45, steps=8)


def accents(m, colors, rings):
    middle = rings[1:-1]
    for side in SIDES:
        strip = []
        for i, (x, z, w, h) in enumerate(middle):
            low = z + h * (.19 + .12 * i / (len(middle) - 1))
            high = low + h * .13
            outer, inner = side * (w + .45), side * (w - .2)
            row = [(x, outer, low), (x, outer, high), (x, inner, high), (x, inner, low)]
            strip.append(row if side > 0 else row[::-1])
        shell(m, colors['stripe'], strip)
    x, z, _, h = middle[-1]
    along(m, FRAME, (x - 7, 0, z + h - .8), (x - 7, 0, z + h + 2.5), 5.5, steps=10)


def lamp(m, x, z, y=0, radius=7.5):
    along(m, FRAME, (x - 8, y, z), (x + 1, y, z), radius + 2.5, radius + 1, steps=12)
    along(m, LAMP, (x + 1, y, z), (x + 2, y, z), radius, steps=12, glow=.55)


def ice_running_gear(m, colors):
    m.side(RUBBER, [(-8, 10), (4, 1), (94, 1), (116, 14), (121, 32),
                    (105, 45), (8, 45), (-9, 31)], -19, 19)
    for index in range(7):
        x = 4 + 15 * index
        m.box(TREAD, (x, 0, 2), (7, 44, 4))
        m.box(TREAD, (x, 0, 44), (7, 44, 4))
    for x in (12, 44, 77, 104):
        along(m, FRAME, (x, -20, 21), (x, 20, 21), 16, steps=12)
        along(m, STEEL, (x, -21, 21), (x, 21, 21), 7, steps=8)
    m.side(STEEL, [(156, 9), (231, 7), (252, 11), (270, 25), (262, 10),
                   (238, 2), (157, 3), (150, 7)], -15, 15)
    for side in SIDES:
        along(m, FRAME, (192, side * 11, 44), (212, side * 11, 10), 4.5)
        along(m, colors['stripe'], (211, side * 12, 10), (239, side * 12, 10), 2.5)


def themed_kit(m, level, colors):
    main, second, stripe = (colors[k] for k in ('bike', 'bike2', 'stripe'))
    if level == 1:
        for side in SIDES:
            y = side * 24
            along(m, FRAME, (33, side * 15, 91), (27, y, 109), 5)
            along(m, INK, (20, y, 87), (41, y, 123), 10, 9, steps=10)
            along(m, main, (23, y, 93), (37, y, 116), 10.4, steps=10)
            along(m, stripe, (28, y, 102), (32, y, 109), 10.7, steps=10, glow=.3)
            m.side(INK, [(125, 93), (133, 94), (141, 115), (133, 114)], y - 1, y + 1)
    elif level == 2:
        for side in SIDES:
            y = side * 24
            along(m, FRAME, (43, side * 15, 91), (43, y, 80), 4)
            coachwork(m, second, ((19, 80, 4, 11), (26, 82, 9, 14), (56, 85, 9, 14), (63, 88, 4, 10)), y)
            m.box(STEEL, (34, side * 33, 88), (5, 2.2, 8))
        along(m, '#a87a4a', (28, -22, 102), (28, 22, 102), 9.5, steps=10)
        for y in (-14, 14):
            along(m, second, (28, y - 2, 102), (28, y + 2, 102), 10.0, steps=10)
        m.side(second, [(174, 128), (182, 129), (175, 151), (167, 151)], -17, 17)
    elif level == 3:
        for side in SIDES:
            y = side * 22
            coachwork(m, main, ((106, 67, 2, 12), (129, 68, 5, 23), (175, 73, 5, 23), (187, 81, 2, 11)), y)
            m.side(stripe, [(125, 82), (162, 92), (178, 90), (166, 84), (125, 78)], y - 3, y + 3, glow=.16)
            for i in range(3):
                x = 145 + i * 8
                m.side(INK, [(x, 65), (x + 4, 66), (x + 7, 77), (x + 3, 76)], y + side * 6 - 1, y + side * 6 + 1)
    elif level == 4:
        coachwork(m, second, ((146, 104, 8, 11), (163, 108, 23, 21), (188, 108, 20, 18), (201, 104, 11, 10)))
        m.side(main, [(8, 92), (48, 99), (30, 116), (16, 124), (20, 103)], -5, 5)
        for side in SIDES:
            for i in range(3):
                m.sphere(stripe, (161 + i * 7, side * (23 - i), 111 - i * 2), 2.65,
                         steps=(4, 6), glow=.26)
            lamp(m, 196, 107, side * 11, radius=4.3)
    elif level == 5:
        coachwork(m, second, ((150, 112, 8, 11), (164, 110, 23, 20), (189, 104, 24, 17), (207, 99, 14, 11)))
        coachwork(m, '#42607c', ((162, 145, 12, 2.8), (172, 133, 20, 5), (185, 121, 21, 3.5)))
        for side in SIDES:
            y = side * 26
            along(m, FRAME, (28, y, 83), (20, y, 128), 4)
            along(m, stripe, (20, y, 123), (16, y, 143), 3.6)
            lamp(m, 204, 101, side * 9, radius=5)
    elif level == 6:
        for side in SIDES:
            y = side * 24
            along(m, FRAME, (41, side * 13, 82), (36, y, 78), 4.5)
            along(m, second, (18, y, 76), (63, y, 89), 8, 6, steps=10)
            along(m, second, (28, y, 75), (8, y, 73), 9.6, steps=12)
            along(m, INK, (8, y, 73), (7.5, y, 73), 6.7, steps=12)
            along(m, stripe, (7.5, y, 73), (7, y, 73), 3, glow=.35)
        coachwork(m, main, ((137, 101, 9, 9), (155, 105, 24, 17), (179, 101, 25, 18), (194, 97, 12, 10)))
        lamp(m, 195, 100, radius=8)
    elif level == 7:
        for side in SIDES:
            y = side * 23
            coachwork(m, second, ((71, 76, 3, 8), (115, 73, 5, 17), (169, 76, 7, 20), (196, 91, 4, 11)), y)
            m.side(stripe, [(115, 81), (165, 90), (180, 94), (163, 87), (115, 78)], y + side * 6 - .7, y + side * 6 + .7, glow=.23)
            lamp(m, 197, 101, side * 12, radius=4.5)


def helmet(m, colors, level, x, z, outlaw=0):
    shell_color = colors['helmet']
    visor = colors['visor']
    if outlaw:
        m.sphere(colors['skin'], (x + 2, 0, z - 2), 14, scale=(.87, .82, 1.12), steps=(7, 10))
        m.sphere(shell_color, (x - 2, 0, z + 8), 15.5, scale=(1.08, .96, .64), steps=(6, 10))
        m.sphere(colors['hair'], (x + 8, 0, z - 11), 9, scale=(.84, .96, .68), steps=(5, 8))
        for side in SIDES:
            m.sphere(INK, (x + 11, side * 6, z + 3), 5.3, scale=(.45, 1.04, .71), steps=(5, 8))
            m.sphere(visor, (x + 13, side * 6, z + 3), 4.1, scale=(.4, 1, .69), steps=(5, 8))
            along(m, INK, (x - 4, side * 11, z + 4), (x + 4, side * 8, z - 11), 1.2, steps=5)
        if outlaw == 1:
            along(m, colors['hair'], (x - 14, 0, z + 1), (x - 21, 1, z - 12), 4.7, 2.3, steps=6)
        else:
            m.sphere(colors['stripe'], (x - 4, 0, z + 17), 8, scale=(1.4, .23, .19), steps=(4, 8))
        return
    size = 17 if level != 6 else 20
    m.sphere(shell_color, (x, 0, z), size, scale=(1.02, .9, 1), steps=(7, 12))
    if level == 6:
        along(m, FRAME, (x + 14, 0, z), (x + 23, 0, z), 12, 10, steps=12)
        along(m, visor, (x + 23, 0, z), (x + 24, 0, z), 8, steps=12, glow=.3)
        for side in SIDES:
            along(m, colors['bike2'], (x, side * 12, z - 2), (x, side * 22, z - 2), 7, steps=10)
        return
    if level in (1, 2):
        mask = INK if level == 1 else colors['tie']
        m.sphere(mask, (x + 11, 0, z - 6), 10, scale=(.75, 1.12, .75), steps=(6, 10))
        for side in SIDES:
            if level == 1:
                along(m, FRAME, (x + 9, side * 8, z - 7), (x + 16, side * 12, z - 8), 4.2, steps=8)
            else:
                along(m, mask, (x - 11, side * 5, z - 9), (x - 28, side * 11, z - 15), 3.5, .8, steps=6)
            m.sphere(INK, (x + 12, side * 6, z + 3), 5.5, scale=(.74, 1.16, 1.0), steps=(5, 8))
            m.sphere(visor, (x + 15, side * 6, z + 3), 4.2, scale=(.6, 1.15, .85), steps=(5, 8), glow=.16)
        return
    if level == 4:
        m.sphere(colors['skin'], (x + 8, 0, z - 3), 11, scale=(.86, 1.05, 1.0), steps=(6, 10))
        m.sphere(colors['bike'], (x + 18, 0, z - 2), 3.6, steps=(5, 7))
        for side in SIDES:
            point = (x - 13, side * 31, z + 22)
            color = shell_color if side < 0 else colors['suit']
            elbow = (x - 6, side * 20, z + 24)
            along(m, color, (x - 3, side * 8, z + 10), elbow, 8.5, 5.2, steps=7)
            along(m, color, elbow, point, 5.2, 1.0, steps=7)
            m.sphere(colors['stripe'], point, 3.9, steps=(5, 7))
    brow = 5 if level == 5 else 2
    if level == 5:
        m.sphere(colors['bike2'], (x + 8, 0, z + 1), 14, scale=(.58, 1.05, .89), steps=(6, 12))
    m.sphere(INK, (x + 10, 0, z + brow), 13, scale=(.67, 1.07, .61), steps=(6, 12))
    m.sphere(visor, (x + 14, 0, z + brow + 1), 11.5, scale=(.45, 1.08, .46), steps=(6, 12),
             glow=.14 if level in (0, 5) else 0.0)
    if level in (0, 3, 7):
        m.sphere(shell_color, (x + 12, 0, z - 8), 9, scale=(.68, 1.12, .55), steps=(5, 10))


def rider(m, colors, level, hip, shoulder, grip, peg, knees, outlaw=0):
    hx, hz = hip
    sx, sz = shoulder
    gx, gy, gz = grip
    px, pz = peg
    kx, kz = knees
    suit = colors['suit']
    seam = colors['suit2']
    pitch = math.degrees(math.atan2(-(sx - hx), sz - hz))
    chest = (hx + (sx - hx) * .68, 0, hz + (sz - hz) * .68)
    waist = (hx + (sx - hx) * .3, 0, hz + (sz - hz) * .3)
    m.sphere(seam, (hx, 0, hz + 1), 16, scale=(1.17, 1.12, .8), steps=(6, 10), rot=(pitch, 0, 0))
    m.sphere(suit, waist, 17, scale=(.87, 1.07, 1.04), steps=(6, 10), rot=(pitch, 0, 0))
    m.sphere(suit, chest, 23, scale=(.79, 1.05, 1.05), steps=(7, 12), rot=(pitch, 0, 0))
    along(m, seam, (sx - 2, 0, sz - 3), (sx + 7, 0, sz + 10), 11.6, 9.6)
    for side in SIDES:
        arm = (sx - 1, side * 19, sz - 3)
        elbow = (sx + (gx - sx) * .31 - 4, side * (gy + 3), sz - (9 if outlaw == 1 else 21))
        wrist = (gx - 3, side * gy, gz)
        knee = (kx, side * (24 if outlaw else 23), kz)
        ankle = (px - 5, side * 26, pz + 7)
        m.sphere(suit, arm, 11.2, scale=(1, 1.14, 1.03), steps=(6, 9))
        along(m, suit, arm, elbow, 9, 6.3)
        m.sphere(seam, elbow, 6.6, steps=(5, 8))
        along(m, suit, elbow, wrist, 7.1, 4.5)
        along(m, seam, between(elbow, wrist, .70), wrist, 5.7, 5.1)
        m.sphere(INK, (gx, side * gy, gz), 5.9, scale=(1.25, .9, .91), steps=(5, 8))
        thigh = (hx + 2, side * 12, hz - 4)
        along(m, seam, thigh, knee, 12.8, 8.3)
        m.sphere(suit, knee, 9, scale=(1, .86, .94), steps=(5, 8))
        along(m, seam, knee, ankle, 8.2, 5.7)
        along(m, INK, between(knee, ankle, .5), ankle, 7.8, 6.1)
        m.sphere(INK, (px, side * 26, pz + 3), 7.4, scale=(1.77, .82, .65), steps=(5, 9))
        along(m, STEEL, (px, side * 15, pz), (px, side * 32, pz), 2.4, steps=6)
        pad_col = seam if not outlaw else suit
        m.sphere(pad_col, (arm[0] - 1, side * 25, arm[2] + 1), 8.5,
                 scale=(.96, .7, .91), steps=(5, 8))
        strap_a = (sx - 14, side * 12, sz - 3)
        strap_b = (hx - 8, side * 10, hz + 23)
        along(m, INK, strap_a, strap_b, 2.7, steps=6)
        m.box(STEEL, (strap_a[0] - 2, strap_a[1], strap_a[2] - 4), (3, 6, 5), rot=(pitch, 0, 0))
    if level == 6:
        for side in SIDES:
            y = side * 10
            bottom = (hx - 16, y, hz + 12)
            top = (sx - 25, y, sz + 2)
            along(m, colors['bike2'], bottom, top, 7.8, 7.2)
            along(m, FRAME, between(bottom, top, .35), between(bottom, top, .46), 8.4)
            along(m, INK, top, (sx - 17, y, sz + 15), 2.5, steps=6)
    if outlaw:
        # Two large stitched dice form the rear club emblem and stay visible from chase view.
        for side in SIDES:
            y = side * 7.5
            x = chest[0] - 17.5
            z = chest[2] + side * 3
            m.box('#fff6ea', (x, y, z), (2.2, 13, 15), rot=(pitch, 0, side * 15))
            m.sphere(TAIL, (x - 1.25, y, z), 2.25, steps=(4, 6))
    helmet(m, colors, level, sx + (7 if outlaw else 10), sz + 22, outlaw)


def moto(m, level=0, colors=None):
    c = BAYSHORE if colors is None else colors
    steering, tank, hip, shoulder, grip = SPORT[level]
    rear, front = (40, 34), (190, 32)
    main, second = c['bike'], c['bike2']
    if level == 5:
        ice_running_gear(m, c)
        front = (192, 42)
    else:
        tread = 16 if level in (1, 2) else 12
        wheel(m, *rear, 34, 22, tread, 6 if level == 2 else 5)
        wheel(m, *front, 32, 17, tread, 6 if level == 2 else 5)
        fender(m, main, *front, 32, 11, 156, 20)
    linkage(m, front, steering, 12, CHROME)
    rear_arm(m, rear, (103, 60), 14)
    for side in SIDES:
        y = side * 12
        along(m, FRAME, (63, y, 84), (steering[0] - 4, y, steering[1] - 4), 5.4, 4.2)
        along(m, FRAME, (steering[0] - 4, y, steering[1] - 13), (137, y, 49), 4.4, 3.7)
        along(m, FRAME, (137, y, 49), (92, y, 50), 3.7)
        along(m, FRAME, (92, y, 50), (63, y, 84), 3.7)
    engine(m, 114, 64)
    coachwork(m, main, tank)
    accents(m, c, tank)
    coachwork(m, second, ((10, 83, 7, 5), (26, 93, 16, 10), (58, 94, 18, 12), (84, 91, 10, 7)))
    coachwork(m, INK, ((31, 104, 8, 3), (53, 106, 15, 4), (78, hip[1] - 11, 16, 5), (89, hip[1] - 13, 9, 3)))
    m.box(TAIL, (12, 0, 87), (3.2, 15, 4.5), rot=(8, 0, 0), glow=.6)
    tail_weapon(m, 20, -19, 90)
    cockpit(m, steering, grip)
    if level not in (4, 5, 6, 7):
        lamp(m, steering[0] + 14, steering[1] - 3, radius=8.5 if level == 2 else 6.8)
    pipe(m, (138, 17, 79), (124, 25, 45), (22, 25, 51), wide=level == 3)
    if level == 3:
        pipe(m, (127, -17, 71), (114, -25, 48), (16, -25, 49), wide=True)
    themed_kit(m, level, c)
    rider(m, c, level, hip, shoulder, grip, (98, 59), (118, hip[1] - 22))


def hog(m, variant=1, with_rider=True, x0=0.0):
    old_base = m.base
    if x0:
        translation = ft_art._xf((x0, 0, 0))
        m.base = translation if old_base is None else translation * old_base
    try:
        build_hog(m, variant, with_rider)
    finally:
        m.base = old_base


def build_hog(m, variant, with_rider):
    c = OUTLAWS[variant]
    low = variant == 2
    main, second = c['bike'], c['bike2']
    wheel(m, 48, 36, 36, 30, 14, 5 if low else 8)
    wheel(m, 318, 30, 30, 15 if low else 12, 12, 5 if low else 8)
    steer = (258, 118) if low else (248, 135)
    grip = (227, 35, 154) if low else (224, 43, 200)
    linkage(m, (318, 30), steer, 10.4, CHROME)
    rear_arm(m, (48, 36), (139, 52), 17)
    fender(m, main, 48, 36, 36, 17, 157, 30)
    fender(m, second, 318, 30, 30, 9, 143, 31)
    for side in SIDES:
        y = side * 13
        along(m, FRAME, (83, y, 73), (steer[0] - 7, y, steer[1] - 3), 4.8)
        along(m, FRAME, (steer[0] - 7, y, steer[1] - 12), (183, y, 43), 4.4)
        along(m, FRAME, (183, y, 43), (114, y, 42), 4.4)
        along(m, FRAME, (114, y, 42), (83, y, 73), 4.4)
    engine(m, 158, 60, wide=True)
    tank = ((143, 100, 9, 6), (165, 105, 15, 12), (205, 107, 23, 19), (230, 106, 12, 10)) if low else (
            (146, 105, 5, 5), (174, 115, 21, 17), (215, 120, 25, 20), (239, 116, 9, 11))
    coachwork(m, main, tank)
    accents(m, c, tank)
    seat = ((79, 81, 8, 5), (94, 89, 15, 5), (136, 89, 16, 6), (154, 94, 11, 5)) if low else (
            (79, 87, 8, 5), (95, 94, 15, 6), (137, 98, 16, 6), (154, 105, 11, 5))
    coachwork(m, INK, seat)
    m.box(TAIL, (12, 0, 63), (3.5, 14, 5), glow=.55)
    tail_weapon(m, 26, -19, 73)
    cockpit(m, steer, grip, tall=not low)
    if low:
        m.side(second, [(254, 116), (273, 123), (268, 140), (251, 144), (247, 127)], -15, 15)
        lamp(m, 272, 125, radius=6.8)
        pipe(m, (189, 18, 87), (147, 28, 47), (36, 29, 61), color=FRAME, wide=True)
        pipe(m, (172, -18, 87), (149, -28, 47), (43, -29, 59), color=FRAME, wide=True)
    else:
        lamp(m, 269, 129, radius=10)
        pipe(m, (187, 19, 92), (171, 25, 50), (14, 25, 53), color=CHROME, wide=True)
        pipe(m, (147, 18, 87), (136, 28, 35), (7, 28, 38), color=CHROME, wide=True)
        for side in SIDES:
            along(m, CHROME, (86, side * 13, 80), (69, side * 11, 111), 2.7)
        along(m, CHROME, (69, -11, 111), (69, 11, 111), 2.7)
    if with_rider:
        pose = ((116, 101), (153, 158), (180, 52), (163, 94)) if low else (
            (113, 109), (127, 176), (177, 58), (157, 111))
        hip, shoulder, peg, knees = pose
        rider(m, c, 0, hip, shoulder, grip, peg, knees, variant)


def export():
    import ft_export
    import ft_v28

    root = '/Game/Characters/'
    paths = []
    for level in range(8):
        model = ft_art.Model('Motorcycle')
        moto(model, level, ft_v28.THEMES.get(level))
        folder = 'Syndicate' if not level else 'Themed/L%d' % level
        path = root + folder + '/SM_Moto'
        ft_export.save_mesh(model.meshes, path, 'car', (-115, 0, 0), collision='box')
        paths.append(path)
    for variant, name in ((1, 'SM_Hog'), (2, 'SM_Hog2')):
        model = ft_art.Model('Snake Eyes')
        hog(model, variant)
        path = root + 'SnakeEyes/' + name
        ft_export.save_mesh(model.meshes, path, 'car', (-150, 0, 0), collision='box')
        paths.append(path)
    return paths
