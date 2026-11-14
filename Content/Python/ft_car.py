"""The KESTREL-88: a Countach-style wedge pushed way past sensible.

Origin is the middle of the rear bumper at ground level. X forward, Y right, Z up (cm).
Everything the chase camera can see from behind is over-built: a double-decker V wing, a chrome
supercharger blower bursting through the engine cover, quad exhausts, fender flares, missile pods,
a roof light bar, a whip antenna with a pennant, mud flaps and pink neon underglow.
"""
import math
import ft_art

RED = '#ff2d3c'
RED_D = '#c81a35'
BLACK = '#1c1a26'
GLASS = '#233a63'
GOLD = '#ffc93c'
CHROME = '#d7e3f0'
STEEL = '#8f9bb3'
TYRE = '#26232e'
PINK = '#ff4fb8'
CYAN = '#2ee6ff'
YEL = '#ffe14d'
WHITE = '#fff6ea'


# 1.4: the game draws the wheels as separate parts (spinning, steering, riding on springs). The body
# is filmed with WHEEL_MODE = 'key', every wheel in pure key green, so tools/bake.py can cut holes where
# a wheel stands in front of the body; the wheels themselves are filmed alone (ft_render.run_wheels).
WHEEL_MODE = 'normal'
KEY = '#00ff00'
LUG = '#a39cc2'
LUGS = 10          # tread blocks per row (two staggered rows): the pattern repeats every 36 degrees,
SPIN_PERIOD = 72.0  # and the five-hole rim every 72, so 72 degrees of spin frames loop seamlessly


def wheel(m, x, z, r, yin, yout, side, spin=0.0, steer=0.0):
    """A fat tyre with a gold telephone-dial rim and chunky tread blocks; yin/yout are |y| of the inner
    and outer faces. spin turns it about its axle (degrees), steer about the vertical."""
    w = yout - yin
    yc = side * (yin + yout) / 2.0
    if not hasattr(m, 'wheels'):
        m.wheels = []
    m.wheels.append((x, yc, z, r, w, side))
    if WHEEL_MODE == 'skip':
        return
    K = (lambda c: KEY) if WHEEL_MODE == 'key' else (lambda c: c)
    old = m.base
    xf = ft_art._xf((x, yc, z), (spin, steer, 0))
    m.base = xf if old is None else xf * old
    try:
        m.cyl(K(TYRE), (0, -w / 2.0, 0), r, w, rot=(0, 0, 90), steps=20)
        # tread: two staggered rows of raised blocks round the tyre, so you can see it turn from behind
        for row in (0, 1):
            yb = (-w / 4.0) if row == 0 else (w / 4.0)
            for k in range(LUGS):
                a = math.radians((k + 0.5 * row) * 360.0 / LUGS)
                rr = r + 0.6
                m.box(K(LUG), (math.cos(a) * rr, yb, math.sin(a) * rr), (2 * math.pi * r / LUGS * 0.46, w * 0.44, 3.2), rot=(-(math.degrees(a) + 90), 0, 0))
        # the rim, proud of the tyre on the outside
        yo = side * w / 2.0 - 1.5
        m.cyl(K(GOLD), (0, yo, 0), r * 0.66, 3, rot=(0, 0, 90), steps=20)
        hy = yo + (1 if side > 0 else -1)
        m.cyl(K(BLACK), (0, hy, 0), r * 0.18, 3, rot=(0, 0, 90), steps=10)
        for k in range(5):
            a = k * 2 * math.pi / 5
            m.cyl(K(BLACK), (math.cos(a) * r * 0.42, hy, math.sin(a) * r * 0.42), r * 0.1, 3, rot=(0, 0, 90), steps=8)
    finally:
        m.base = old


def build(name='KESTREL'):
    m = ft_art.Model(name)
    return build_into(m)


def build_into(m, wrecked=False, damage=0):
    # damage (1.3): 0 showroom, 1 dented (scraped flanks, a smashed lamp, a lost mirror, a bent aerial),
    # 2 trashed (half the top wing torn off and drooping, the bumper hanging, the blower knocked askew,
    # soot and bare metal). The game swaps these in as the armor drops.
    D = damage
    # ---- the wedge
    body = [(0, 22), (425, 22), (448, 32), (446, 44), (300, 82), (60, 88), (10, 86), (0, 66)]
    m.side(RED, body, -100, 100)
    # front splitter and rear diffuser
    m.box(BLACK, (440, 0, 21), (30, 214, 5))
    if D < 2:
        m.side(BLACK, [(-14, 14), (40, 14), (40, 24), (0, 30), (-14, 22)], -92, 92)
        for y in (-66, -33, 0, 33, 66):
            m.box(STEEL, (6, y, 20), (40, 3, 14))
    else:
        # the diffuser has torn loose on the left and drags on the road
        m.side(BLACK, [(-14, 14), (40, 14), (40, 24), (0, 30), (-14, 22)], 0, 92)
        for y in (33, 66):
            m.box(STEEL, (6, y, 20), (40, 3, 14))
        m.box(BLACK, (4, -48, 10), (44, 96, 7), rot=(0, 6, -16))
        m.box(STEEL, (0, -62, 6), (40, 3, 12), rot=(0, 6, -16))
    # the greenhouse and its roof
    m.side(GLASS, [(300, 82), (208, 120), (126, 118), (52, 89)], -70, 70)
    m.side(RED, [(214, 119), (206, 123), (126, 121), (122, 118)], -64, 64)
    # engine cover louvres on the slope behind the roof
    for k in range(6):
        t = (k + 0.5) / 6.0
        x = 120 - t * 62
        z = 117 - t * 26
        m.box(BLACK, (x, 0, z + 1.2), (5, 112, 3.2), rot=(-19, 0, 0))
    # ---- the big rear fender flares and the skinny front ones
    for s in (-1, 1):
        y0, y1 = (s * 100, s * 126) if s > 0 else (s * 126, s * 100)
        # 1.4: the flare's lip sits higher, so the fat rear tyres show (and you can see them turn)
        m.side(RED_D, [(14, 50), (150, 50), (154, 64), (120, 92), (14, 92)], y0, y1)
        m.side(RED_D, [(320, 30), (402, 30), (404, 50), (380, 70), (330, 74), (318, 56)], *((100, 112) if s > 0 else (-112, -100)))
        # the airbox scoops on top of the rear fenders (Countach signature, made huge)
        m.side(BLACK, [(110, 88), (210, 86), (200, 116), (130, 118)], *((76, 104) if s > 0 else (-104, -76)))
        m.box(RED, (134, s * 90, 118), (24, 30, 6))
        # the NACA side duct stripe and the livery
        m.side(CYAN, [(160, 58), (300, 66), (300, 70), (160, 64)], *((100, 101.6) if s > 0 else (-101.6, -100)))
        m.side(PINK, [(160, 50), (310, 56), (310, 60), (160, 55)], *((100, 101.6) if s > 0 else (-101.6, -100)))
        # missile pods slung along the flanks, warheads forward, tails at the back
        hang = D >= 2 and s < 0
        m.part = 'PodL' if s < 0 else 'PodR'
        for dz in (0, 11):
            if hang:
                # the left pod has sheared a mount and hangs nose-down
                m.cyl(STEEL, (40, s * 149, 58 + dz), 5.5, 110, rot=(-104, 0, 0), steps=10)
                m.box(BLACK, (40, s * 149, 58 + dz), (6, 16, 3))
                continue
            m.cyl(STEEL, (40, s * 147, 70 + dz), 5.5, 110, rot=(-90, 0, 0), steps=10)
            m.cyl(RED, (150, s * 147, 70 + dz), 5.5, 16, rot=(-90, 0, 0), steps=10, top_r=0.5)
            m.box(BLACK, (40, s * 147, 70 + dz), (6, 16, 3))
        m.box(BLACK, (95, s * 127, 88), (60, 40, 4))
        m.part = None
        # 1.4: no mud flaps any more: they hid the rear tyres from the chase camera, and now you can see
        # them turn. A pink neon strip under each flare's lip instead.
        m.box(PINK, (16, s * 113, 49), (2, 24, 2.5), glow=0.4)
        # chunky wing mirrors (the right one is knocked off first)
        if D >= 1 and s > 0:
            m.box(BLACK, (298, s * 76, 90), (6, 4, 6))
        else:
            m.part = 'MirrorR' if s > 0 else None
            m.box(RED, (292, s * 88, 98), (14, 30, 16))
            m.box(BLACK, (284, s * 88, 98), (2, 24, 11))
            m.box(BLACK, (298, s * 76, 90), (6, 4, 12))
            m.part = None
    # ---- wheels: fat at the back, narrow up front
    for s in (-1, 1):
        wheel(m, 82, 40, 40, 84, 140, s)  # 1.4: fatter, standing out past the flares
        wheel(m, 362, 31, 31, 86, 112, s)
    # ---- the double-decker V wing
    m.part = 'Wing'
    for s in (-1, 1):
        m.side(BLACK, [(26, 86), (44, 86), (34, 154), (20, 154)], *((44, 52) if s > 0 else (-52, -44)))
        if D >= 2 and s < 0:
            continue  # the left endplate is gone
        m.side(BLACK, [(14, 150), (78, 150), (74, 192), (10, 192)], *((136, 142) if s > 0 else (-142, -136)))
        m.box(PINK, (44, s * 139, 186), (60, 7, 4), glow=0.5)
    if D < 2:
        m.side(RED, [(12, 150), (78, 156), (78, 162), (12, 160)], -138, 138)
        m.side(BLACK, [(16, 172), (66, 176), (66, 181), (16, 179)], -134, 134)
        m.box(CYAN, (12, 0, 155), (2, 270, 4), glow=0.3)
    else:
        # the top deck snapped in the middle: the right half holds, the left half droops to the body
        m.side(RED, [(12, 150), (78, 156), (78, 162), (12, 160)], -60, 138)
        m.box(RED, (45, -98, 146), (66, 80, 8), rot=(0, 0, 20))
        m.side(BLACK, [(16, 172), (66, 176), (66, 181), (16, 179)], -4, 134)
        m.box(BLACK, (42, -66, 160), (50, 128, 5), rot=(4, 0, 22))
        m.box(CYAN, (12, 70, 155), (2, 130, 4), glow=0.3)
    # ---- the supercharger blower punching up through the engine cover
    m.part = 'Blower'
    br = (0, 0, 9) if D >= 2 else (0, 0, 0)  # a trashed car's blower is knocked askew
    m.box(CHROME, (92, 0, 116), (56, 72, 26), rot=br)
    for k in range(5):
        m.box(STEEL, (92, 0, 106 + k * 5), (58, 74, 1.4), rot=br)
    m.box(BLACK, (92, 0, 132), (44, 60, 8), rot=br)
    if D < 2:
        m.box(GOLD, (92, 0, 141), (36, 52, 10))
    else:
        m.box(GOLD, (92, 10, 140), (36, 30, 8), rot=(6, 0, 16))
    for ix in (-1, 1):
        for iy in (-1.5, -0.5, 0.5, 1.5):
            if D >= 2 and iy < 0 and ix < 0:
                continue  # two velocity stacks sheared off
            m.cyl(CHROME, (92 + ix * 9, iy * 12, 146), 4.5, 18, steps=10, top_r=6.0, rot=br)
    # ---- the roof: light bar, guns and the whip antenna with a pennant
    m.part = 'LightBar'
    m.box(BLACK, (176, 0, 124), (14, 120, 4))
    for y in (-45, -15, 15, 45):
        m.cyl(BLACK, (180, y, 126), 9, 6, rot=(-90, 0, 0), steps=14)
        smashed = (D >= 1 and y == 45) or (D >= 2 and y == -15)
        m.cyl('#4a4436' if smashed else YEL, (186, y, 126), 7, 1.5, rot=(-90, 0, 0), steps=14, glow=0.0 if smashed else 0.6)
    m.part = None
    for s in (-1, 1):
        m.box(STEEL, (232, s * 34, 124), (40, 10, 9))
        m.cyl(BLACK, (250, s * 34, 124), 2.4, 40, rot=(-90, 0, 0), steps=8)
    m.part = 'Antenna'
    if D < 1:
        m.cyl(BLACK, (34, -60, 88), 1.2, 140, steps=6, rot=(-12, 0, 0))
        m.side(PINK, [(10, 200), (34, 196), (10, 190)], -61, -59, glow=0.3)
    else:
        # the whip aerial is bent over and the pennant is gone
        m.cyl(BLACK, (34, -60, 88), 1.2, 60, steps=6, rot=(-12, 0, 0))
        m.cyl(BLACK, (22, -60, 146), 1.2, 70, steps=6, rot=(-70, 0, -30))
    m.part = None
    # ---- the back end: black panel, triple tail lamps, a plate and quad exhausts
    m.box(BLACK, (-1, 0, 56), (4, 196, 44))
    for s in (-1, 1):
        for k, (col, g) in enumerate([('#ff3355', 1.1), ('#ff3355', 1.1), ('#ffb03a', 0.8)]):
            yc = s * (84 - k * 21)
            if (D >= 1 and s < 0 and k == 0) or (D >= 2 and s > 0 and k == 1):
                # a smashed lamp: dark glass with a white crack across it
                m.box('#3a1a22', (-3.5, yc, 62), (2, 19, 18))
                m.box(WHITE, (-3.8, yc, 62), (1, 20, 1.6), rot=(0, 0, 38))
                continue
            m.box(col, (-3.5, yc, 62), (2, 19, 18), glow=g)
            m.box(WHITE, (-3.6, yc, 52), (2, 19, 4), glow=0.2)
    if D < 2:
        m.box(YEL, (-3.6, 0, 46), (2, 46, 14))
        m.box(BLACK, (-3.7, 0, 46), (2, 36, 3))
    else:
        m.box(YEL, (-4.5, 6, 40), (2, 46, 14), rot=(0, 0, 24))  # the plate hangs off one bolt
    for y in (-50, -32, 32, 50):
        m.cyl(CHROME, (8, y, 30), 7.5, 24, rot=(90, 0, 0), steps=14)
        m.cyl(BLACK, (-15, y, 30), 4.5, 2, rot=(90, 0, 0), steps=12)
    # a cyan centre stripe over the whole car, 80s style
    m.side(CYAN, [(0, 86.5), (60, 88.5), (300, 82.5), (446, 44.5), (446, 46), (300, 84), (60, 90), (0, 88)], -8, 8)
    # ---- the damage you take (1.3): dents read as dark facets in the cel light, scrapes as bare metal
    if D >= 1:
        DENT, SOOT = '#9c1129', '#2a2228'
        # dents in the left rear flare and the right flank, a crumpled corner of the rear deck
        m.box(DENT, (70, -127, 66), (44, 2, 24), rot=(0, 9, 16))
        m.box(DENT, (36, -127, 52), (26, 2, 16), rot=(0, -12, -20))
        m.box(DENT, (228, 102, 60), (50, 2, 14), rot=(0, 6, -10))
        m.box(DENT, (22, -70, 88.5), (30, 40, 2), rot=(8, 0, -6))
        # long scrapes down the flanks
        for s in (-1, 1):
            for k in range(3):
                m.box(STEEL, (110 + k * 26, s * (102 if k else 127), 44 + k * 7), (60 - k * 10, 1.6, 2.2), rot=(-5 + k * 4, 0, 0))
        m.box(STEEL, (-2, 40, 34), (3, 50, 2.5), rot=(0, 0, -8))
    if D >= 2:
        # soot round the blower and the rear deck, bare metal where the paint came off,
        # a cracked rear screen, and the left livery stripe scraped away
        for (x, y, z, w, l) in [(118, -20, 120, 40, 36), (64, 34, 98, 46, 30), (160, 60, 116, 30, 26), (30, -100, 90, 30, 30)]:
            m.box(SOOT, (x, y, z), (w, l, 2), rot=(-8, 0, 0))
        for (x, y, z) in [(98, -128, 70), (260, -102, 52), (20, 127, 62)]:
            m.box(STEEL, (x, y, z), (34, 2, 18), rot=(0, 0, 12))
        for k in range(5):
            m.box('#c9d6ff', (96 - k * 4, -30 + k * 13, 107 + k * 2), (40, 1.2, 1.2), rot=(-21, k * 31, 0))
        m.side('#3a3036', [(160, 50), (310, 56), (310, 70), (160, 64)], -102.4, -100.2)
        m.box(DENT, (-1.2, -60, 64), (2.5, 50, 30), rot=(0, 10, 8))
    # ---- pink neon underglow
    if not wrecked:
        m.box(PINK, (215, 0, 3), (430, 220, 1), glow=1.6 if D < 2 else 0.9)
    else:
        # burnt out: everything goes to soot and ash, the glow dies
        ash = {RED: '#3a2f33', RED_D: '#2a2228', GLASS: '#1a1a22', GOLD: '#5a5048', CHROME: '#6a6870', CYAN: '#3a3d44', PINK: '#3a3036', YEL: '#4a4640', WHITE: '#555555'}
        old = m.meshes
        m.meshes = {}
        for (col, glow), mesh in old.items():
            key = (ash.get(col, '#2a2a2e' if col not in (TYRE, BLACK, STEEL) else col), 0.0)
            if key in m.meshes:
                unreal_append(m.meshes[key], mesh)
            else:
                m.meshes[key] = mesh
    return m


def unreal_append(dst, src):
    import unreal
    unreal.GeometryScript_MeshEdits.append_mesh(dst, src, unreal.Transform())


def cam_for(psi_deg, elev_deg=16.0, dist=1650.0, target=(205, 0, 78)):
    """Camera that sees the car as if it had yawed psi to the right, from behind and above."""
    a = math.radians(-psi_deg)
    e = math.radians(elev_deg)
    dx, dy, dz = math.cos(e) * math.cos(a), math.cos(e) * math.sin(a), -math.sin(e)
    loc = (target[0] - dx * dist, target[1] - dy * dist, target[2] - dz * dist)
    return loc, (-elev_deg, math.degrees(a), 0)
