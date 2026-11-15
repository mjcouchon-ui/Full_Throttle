"""FULL THROTTLE 3.3: new models, all original designs in the house style (chunky flat-shaded low-poly blocks, bold
colour, hazard stripes, a few glowing lights). Units cm, X forward, Y right, Z up.

    THE CHOPPER        Choppers/SM_Heli (+ Themed/L1..L7/SM_Heli), SM_HeliRotor, SM_HeliTailRotor
    SWAT TEAM          Syndicate/SM_Henchman (rappelling, + Themed/L1..L7), Syndicate/SM_HenchmanHang (on a car door)
    SPLIT WHEELS       every car exported with ft_car.wheel re-exported body-only, its wheels in Content/Data/wheels.json;
                       Wheels/SM_FoeWheel, SM_FoeWheelChrome, SM_FoeStrut
    TESLA TRUCK        Bosses/SM_Tanker (level 2 boss), SM_TeslaTop (toroid, spins and pulses), SM_TeslaArc (lightning)
    CLOWN RIG          Bosses/SM_ClownRig (bolts onto SM_ClownCar), SM_ClownProp
    AUDIO              /Game/Audio/SFX/S_TeslaCharge, S_TeslaZap, S_PropBuzz (looping), S_BombWhistle

Run over the Unreal MCP connector, one group per call:
    import ft_v33
    ft_v33.export_heli(); ft_v33.export_swat(); ft_v33.export_wheels(); ft_v33.export_split(<group>); ft_v33.export_tesla()
    ft_v33.export_clown(); ft_v33.import_audio()
Translucent bits (the canopy, the rotor blur discs) are drawn with a colour key starting with '@' and saved as extra
sections with the M_FTTint instances (save()).
"""
import json
import math
import os
import random
import unreal
import ft_art
import ft_car
import ft_export
import ft_fleet
import ft_mats
from ft_car import BLACK, GLASS, CHROME, STEEL, TYRE, GOLD, PINK, CYAN, YEL, WHITE
from ft_desert import rod, tilt_box

AL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
GS = unreal.GeometryScript_Primitives
save_mesh, model = ft_export.save_mesh, ft_export.model
U = 9.14
RED = '#ff3355'
GREEN = '#3dff6a'
HAZ = '#ffd23f'
DK = '#16121f'       # interiors, wheel wells, chassis
DK2 = '#2a2632'
TESTS = r'C:\Users\mjcou\WispOS\projects\set-to-fail\full-throttle-tests\v330'
SHOTS = os.path.join(TESTS, 'models')
WHEELS_JSON = os.path.join(unreal.Paths.project_content_dir(), 'Data', 'wheels.json')


# =============================================================================================== helpers
def torus(m, col, center, R, r, rot=(0, 0, 0), major=24, minor=8, glow=0.0):
    """A donut lying in the local XY plane (axis local Z), centred at center."""
    GS.append_torus(m._mesh(col, glow), m.opts, m._t(center, rot), unreal.GeometryScriptRevolveOptions(), R, r, major, minor,
                    unreal.GeometryScriptPrimitiveOriginMode.CENTER)


def cone(m, col, a, b, r, steps=8, glow=0.0, tip=0.0):
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=tip)


def disc_y(m, col, c, r, y0, y1, steps=20, glow=0.0):
    """A disc/cylinder with its axis along Y from y0 to y1 (y0 < y1), centred on (c.x, c.z)."""
    m.cyl(col, (c[0], y0, c[1]), r, y1 - y0, rot=(0, 0, 90), steps=steps, glow=glow)


def xfp(loc, rot, p):
    v = ft_art._xf(loc, rot).transform_location(unreal.Vector(*p))
    return (v.x, v.y, v.z)


def with_base(m, loc, rot, fn):
    old = m.base
    xf = ft_art._xf(loc, rot)
    m.base = xf if old is None else xf * old
    try:
        fn()
    finally:
        m.base = old


# block letters on a 5-row grid ('#' = filled cell)
FONT = {
    'S': ['###', '#..', '###', '..#', '###'],
    'W': ['#...#', '#...#', '#.#.#', '#.#.#', '.#.#.'],
    'A': ['###', '#.#', '###', '#.#', '#.#'],
    'T': ['###', '.#.', '.#.', '.#.', '.#.'],
    'X': ['#.#', '#.#', '.#.', '#.#', '#.#'],
    'H': ['#.#', '#.#', '###', '#.#', '#.#'],
}


def text_width(text):
    return sum(len(FONT[c][0]) for c in text) + len(text) - 1


def block_text(m, col, text, c, cell, axis, right_sign, depth=0.8, glow=0.0):
    """Draw text on a vertical plane. c = centre of the text on the face; axis = 'x' (face normal along X, letters run
    along Y) or 'y' (normal along Y, letters run along X); right_sign = +1/-1, the reading direction along that axis."""
    n = text_width(text)
    u = -n * cell / 2.0
    for ch in text:
        g = FONT[ch]
        for row, line in enumerate(g):
            z = c[2] + (2 - row) * cell
            k = 0
            while k < len(line):
                if line[k] == '#':
                    j = k
                    while j < len(line) and line[j] == '#':
                        j += 1
                    mid = u + (k + j) / 2.0 * cell
                    ln = (j - k) * cell
                    if axis == 'x':
                        m.box(col, (c[0], c[1] + right_sign * mid, z), (depth, ln, cell), glow=glow)
                    else:
                        m.box(col, (c[0] + right_sign * mid, c[1], z), (ln, depth, cell), glow=glow)
                    k = j
                else:
                    k += 1
        u += (len(g[0]) + 1) * cell


# ---------------------------------------------------------------------------------- translucent materials
TINT = '/Game/Core/Materials/M_FTTint'
TINTS = {
    # key: (colour (linear), opacity, fresnel opacity, emissive gain)
    '@canopy': ((0.05, 0.10, 0.22), 0.38, 0.45, 1.0),
    '@blur': ((0.10, 0.10, 0.13), 0.08, 0.0, 1.0),
    '@blurtip': ((0.9, 0.7, 0.1), 0.14, 0.0, 1.0),
    '@propblur': ((0.9, 0.85, 0.8), 0.1, 0.0, 1.0),
}
_TMI = {}


def tint_master():
    if AL.does_asset_exist(TINT):
        return AL.load_asset(TINT)
    m = ft_mats.new_material('/Game/Core/Materials', 'M_FTTint', blend=unreal.BlendMode.BLEND_TRANSLUCENT,
                             shading=unreal.MaterialShadingModel.MSM_UNLIT, two_sided=True)
    c = ft_mats.vparam(m, 'Color', (0.05, 0.1, 0.2, 1))
    op = ft_mats.sparam(m, 'Opacity', 0.4, y=100)
    fo = ft_mats.sparam(m, 'FresnelOpacity', 0.4, y=200)
    gain = ft_mats.sparam(m, 'Gain', 1.0, y=300)
    fr = ft_mats.expr(m, unreal.MaterialExpressionFresnel, y=400)
    fr.set_editor_property('exponent', 3.0)
    e = ft_mats.custom(m, "return Col * Gain * (1.0 + 2.0 * F);", [('Col', c, ''), ('Gain', gain, ''), ('F', fr, '')], desc='Tint')
    o = ft_mats.custom(m, "return saturate(Op + F * FO);", [('Op', op, ''), ('F', fr, ''), ('FO', fo, '')],
                       out_type=unreal.CustomMaterialOutputType.CMOT_FLOAT1, y=200, desc='Opacity')
    MEL.connect_material_property(e, '', unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.connect_material_property(o, '', unreal.MaterialProperty.MP_OPACITY)
    return ft_mats.finish(m)


def tint_mi(key):
    if key in _TMI:
        return _TMI[key]
    col, op, fo, gain = TINTS[key]
    mi = ft_mats.mi(tint_master(), '/Game/Core/Materials', 'MI_Tint_' + key.lstrip('@'),
                    vectors={'Color': (col[0], col[1], col[2], 1)}, scalars={'Opacity': op, 'FresnelOpacity': fo, 'Gain': gain})
    _TMI[key] = mi
    return mi


def save(m, path, offset=(0, 0, 0), collision=None, kind='car'):
    """save_mesh with the '@' keys split off as translucent sections."""
    meshes = dict((k, v) for k, v in m.meshes.items() if not k[0].startswith('@'))
    extra = [(v, tint_mi(k[0]), 'Glass_' + k[0].lstrip('@')) for k, v in m.meshes.items() if k[0].startswith('@')]
    for v, _, _ in extra:
        unreal.GeometryScript_Normals.set_per_face_normals(v)
    sm = save_mesh(meshes, path, kind, offset, collision=collision, extra=extra or None, no_shadow=('Glass',))
    if not AL.does_asset_exist(path):
        raise RuntimeError('not saved: ' + path)
    return sm


# =============================================================================================== THE CHOPPER
HELI_CLASSIC = dict(body='#7b3bd6', body2='#5a2aa8', stripe=HAZ, glow=PINK)
MAIN_HUB = (0.0, 0.0, 306.0)
TAIL_HUB = (-641.0, 18.0, 280.0)       # the tail rotor hub (its mesh spans local y -2..12, so it meets the gearbox at y 16)
ROPE_L = (8.0, -162.0, 203.0)
ROPE_R = (8.0, 162.0, 203.0)
SEARCHLIGHT = (222.0, -44.0, 30.0)


def pilot(m, x, y, z, suit='#2f3a2e', helm='#f2efe4', visor='#ff8a2a'):
    m.box(DK2, (x - 22, y, z + 22), (10, 30, 50))                 # seat back
    m.box(suit, (x - 4, y, z + 20), (24, 28, 34))                  # torso
    for s in (-1, 1):
        rod(m, suit, (x - 2, y + s * 13, z + 30), (x + 22, y + s * 10, z + 10), 4.5, steps=6)   # arms to the stick
    m.sphere(helm, (x - 2, y, z + 48), 12, scale=(1.05, 1, 1.05), steps=(6, 9))
    m.box(visor, (x + 9, y, z + 48), (6, 17, 7), glow=0.6)
    m.box(DK, (x + 4, y, z + 38), (10, 12, 6))                     # oxygen mask


def heli33(m, P=None, L=0):
    """The Syndicate assault chopper. Origin on the ground under the main rotor mast (MAIN_HUB above it)."""
    P = P or HELI_CLASSIC
    B, B2, HZ, GL = P['body'], P['body2'], P['stripe'], P['glow']
    ARM = '#2b2a35'
    # ---- the cabin: hollow, both sliding doors open (door opening x -60..76, z 66..184)
    m.side(B, [(-172, 66), (-150, 52), (-118, 44), (150, 44), (150, 66)], -90, 90)              # belly
    m.side(B, [(-172, 184), (150, 184), (112, 212), (-110, 216), (-172, 198)], -90, 90)         # roof
    m.side(B, [(-178, 70), (-60, 66), (-60, 184), (-172, 184)], -90, 90)                        # behind the doors
    m.box(B, (113, 0, 125), (74, 180, 118))                                                     # ahead of the doors
    m.box(ARM, (25, 0, 40), (250, 150, 8))                                                      # belly armour
    # the inside: dark lining, floor, a bench, a grab rail
    m.box(DK, (8, 0, 67), (136, 178, 2))
    m.box(DK, (8, 0, 183), (136, 178, 2))
    m.box(DK, (-59, 0, 125), (2, 178, 118))
    m.box(DK, (75, 0, 125), (2, 178, 118))
    m.box(DK2, (8, 0, 90), (112, 34, 44))
    m.box(DK2, (8, 0, 128), (112, 10, 40))
    for s in (-1, 1):
        m.box(RED, (8, s * 12, 113), (108, 12, 5))
        rod(m, CHROME, (-54, s * 84, 177), (70, s * 84, 177), 2.2, steps=6)
    # door frames in hazard blocks, the slid-back doors on their rails
    for s in (-1, 1):
        y0, y1 = (90, 91.6) if s > 0 else (-91.6, -90)
        yc = s * 90.8
        for k in range(8):
            m.box(HZ if k % 2 == 0 else BLACK, (-56 + k * 17 + 8.5, yc, 187), (17, 1.8, 5))
            m.box(HZ if k % 2 == 1 else BLACK, (-56 + k * 17 + 8.5, yc, 63), (17, 1.8, 5))
        for x in (-62, 78):
            for k in range(7):
                m.box(HZ if k % 2 == 0 else BLACK, (x, yc, 66 + k * 17 + 8.5), (5, 1.8, 17))
        m.box(BLACK, (-62, s * 93, 190), (220, 3, 3))                                          # top rail
        m.box(BLACK, (-62, s * 93, 62), (220, 3, 3))                                           # bottom rail
        m.box(B2, (-114, s * 94, 126), (108, 4, 112))                                          # the door, slid back
        m.box(GLASS, (-100, s * 96.2, 150), (46, 1, 34))
        m.box(HZ, (-66, s * 96.5, 118), (6, 1.5, 18))                                          # handle
        if L == 6:
            for k in range(2):
                m.cyl('#5ff0ff', (-140 + k * 52, s * 96, 104), 11, 2, rot=(0, 0, 90 * s), steps=10, glow=0.8)
        # flank livery under the sill: a hazard stripe and a neon line
        m.side(HZ, [(-150, 54), (150, 54), (150, 60), (-146, 60)], y0, y1)
        m.side(GL, [(-140, 48), (150, 48), (150, 51), (-140, 51)], y0, y1, glow=0.6)
        # armour plates with rivets on the front section
        m.box(ARM, (113, s * 91, 150), (66, 2, 50))
        for (px, pz) in ((84, 170), (142, 170), (84, 130), (142, 130)):
            m.sphere(STEEL, (px, s * 92.2, pz), 2.2, steps=(3, 5))
        m.box(GLASS, (113, s * 92.2, 152), (40, 1, 8))                                          # vision slot
    # ---- the nose: armoured, tapering, a tinted bubble canopy with the crew inside
    m.side(B, [(150, 44), (205, 50), (262, 70), (300, 96), (306, 112), (292, 124), (150, 132)], -66, 66)
    for s in (-1, 1):
        tilt_box(m, '#3a3450', (150, s * 88, 96), (282, s * 62, 96), 10, 50)                   # cheek armour
        tilt_box(m, HZ, (152, s * 89, 60), (280, s * 64, 70), 3, 6)
        for k in range(4):
            t = (k + 0.5) / 4.0
            m.sphere(STEEL, (150 + 132 * t, s * (88 - 26 * t + 5.5), 112), 2, steps=(3, 5))
    m.box(DK, (246, 0, 128), (36, 96, 14))                                                     # instrument hood
    for y in (-26, 0, 26):
        m.box(GREEN if y else CYAN, (228, y, 134), (2, 16, 8), glow=1.0)
    for y in (-26, 26):
        pilot(m, 196, y, 120)
    m.sphere('@canopy', (206, 0, 132), 1, scale=(92, 62, 70), steps=(10, 16))
    for k in range(9):                                                                         # frame: a spine and a hoop
        a0, a1 = math.radians(10 + k * 17), math.radians(10 + (k + 1) * 17)
        p0 = (206 + 93 * math.cos(a0), 0, 132 + 71 * math.sin(a0))
        p1 = (206 + 93 * math.cos(a1), 0, 132 + 71 * math.sin(a1))
        tilt_box(m, B2, p0, p1, 4, 4)
    for k in range(8):
        a0, a1 = math.radians(k * 22.5), math.radians((k + 1) * 22.5)
        p0 = (170, 59 * math.cos(a0), 132 + 66 * math.sin(a0))
        p1 = (170, 59 * math.cos(a1), 132 + 66 * math.sin(a1))
        tilt_box(m, B2, p0, p1, 4, 4)
    tilt_box(m, STEEL, (292, 0, 126), (262, 0, 200), 3, 5)                                     # wire cutter
    for k in range(3):
        m.box(STEEL, (285 - k * 10, 3, 140 + k * 18), (6, 3, 4), rot=(0, 0, 0))
    rod(m, STEEL, (304, 0, 108), (352, 0, 108), 2.2, steps=6)                                  # pitot
    # chin gun: a ball turret with twin barrels
    m.cyl(ARM, (262, 0, 58), 14, 14, steps=10)
    m.sphere(ARM, (262, 0, 52), 21, steps=(6, 10))
    for y in (-6, 6):
        rod(m, BLACK, (272, y, 48), (350, y, 46), 3.2, steps=6)
        m.box(STEEL, (350, y, 46), (10, 7, 7))
    m.box(HZ, (282, 0, 56), (6, 18, 6))
    # the searchlight under the nose, on a yoke
    m.box(STEEL, (200, -44, 40), (14, 10, 10))
    rod(m, BLACK, (198, -44, 33), (220, -44, 30), 12, steps=12)
    m.cyl('#fff1c0', (219, -44, 30), 10, 3, rot=(-96, 0, 0), steps=12, glow=2.2)
    # ---- the engine deck, intakes, exhausts, the mast
    m.side(B2, [(-132, 212), (62, 210), (72, 236), (44, 256), (-112, 256), (-142, 236)], -60, 60)
    for k in range(5):
        m.box(BLACK, (-80 + k * 22, 0, 256.5), (6, 80, 2))
    for s in (-1, 1):
        m.cyl(B, (54, s * 38, 236), 17, 30, rot=(-90, 0, 0), steps=12)                          # intakes
        m.cyl(DK, (84.5, s * 38, 236), 13, 1, rot=(-90, 0, 0), steps=12)
        m.box(STEEL, (85.5, s * 38, 236), (1, 26, 2.5))
        m.box(STEEL, (85.5, s * 38, 236), (1, 2.5, 26))
        rod(m, STEEL, (-126, s * 40, 238), (-172, s * 64, 246), 13, steps=12)                   # exhausts
        rod(m, '#ff5a1a', (-171, s * 63.5, 245.8), (-174, s * 65, 246.3), 9, steps=12, glow=0.9)
        m.box(HZ, (40, s * 61, 234), (40, 2, 6))
    m.cyl(BLACK, (0, 0, 254), 14, 40, steps=12)                                                  # mast
    m.cyl(STEEL, (0, 0, 280), 24, 8, steps=14)                                                   # swashplate
    for a in (0, 120, 240):
        rod(m, STEEL, (math.cos(math.radians(a)) * 20, math.sin(math.radians(a)) * 20, 288),
            (math.cos(math.radians(a)) * 12, math.sin(math.radians(a)) * 12, 300), 2.2, steps=5)
    m.box(RED, (-104, 0, 260), (12, 12, 8), glow=1.6)                                           # beacon
    # ---- the stub wings with rocket pods, missiles and nav lights
    for s in (-1, 1):
        tilt_box(m, B2, (-112, s * 86, 152), (-112, s * 214, 142), 84, 10)
        m.box(HZ, (-72, s * 150, 147), (4, 120, 10))
        m.box(BLACK, (-112, s * 166, 132), (40, 8, 16))                                         # pylon
        m.cyl(ARM, (-172, s * 166, 120), 21, 112, rot=(-90, 0, 0), steps=12)
        m.cyl(ARM, (-172, s * 166, 120), 21, 26, rot=(90, 0, 0), steps=12, top_r=8)            # tail fairing
        m.cyl(DK, (-59.5, s * 166, 120), 18, 1, rot=(-90, 0, 0), steps=12)
        for k in range(7):
            a = k * 2 * math.pi / 6
            ry = 0 if k == 6 else math.cos(a) * 11
            rz = 0 if k == 6 else math.sin(a) * 11
            m.cyl(RED, (-60, s * 166 + ry, 120 + rz), 4, 2, rot=(-90, 0, 0), steps=6, glow=0.4)
        m.box(HZ, (-80, s * 166, 120), (6, 44, 44))
        # a missile on the wingtip rail
        m.box(BLACK, (-112, s * 214, 136), (70, 6, 6))
        m.cyl(STEEL, (-160, s * 214, 128), 7, 100, rot=(-90, 0, 0), steps=10)
        m.cyl(RED, (-60, s * 214, 128), 7, 18, rot=(-90, 0, 0), steps=10, top_r=0.5)
        for a in (0, 90):
            m.box(BLACK, (-152, s * 214, 128), (16, 2, 26), rot=(a, 0, 0))
        m.box(GREEN if s > 0 else RED, (-112, s * 218, 142), (16, 4, 6), glow=1.6)
    # ---- the rappel arms: a davit out of the top of each door with a pulley, the rope anchor under it
    for s in (-1, 1):
        m.box(BLACK, (8, s * 74, 218), (30, 30, 6))
        tilt_box(m, HZ, (8, s * 70, 224), (8, s * 166, 232), 15, 14)
        for k in range(4):
            t = (k + 0.5) / 4.0
            m.box(BLACK, (8, s * (78 + 76 * t), 225 + 8 * t), (15.6, 9, 14.6))
        tilt_box(m, STEEL, (8, s * 92, 192), (8, s * 136, 228), 8, 8)
        m.cyl(STEEL, (0, s * 162, 218), 12, 16, rot=(-90, 0, 0), steps=12)                      # pulley (axis X)
        m.cyl(BLACK, (-1, s * 162, 218), 5, 18, rot=(-90, 0, 0), steps=8)
        m.box(STEEL, (8, s * 162, 228), (16, 4, 10))
        torus(m, CHROME, (8, s * 162, 207), 4, 1.2, rot=(0, 0, 90), major=10, minor=4)          # the hook ring
    # ---- the tail boom, the fin, the stabiliser, the tail rotor gearbox
    m.cyl(B, (-150, 0, 180), 34, 470, rot=(90, 0, 0), steps=12, top_r=15)
    tilt_box(m, B2, (-150, 0, 210), (-570, 0, 192), 14, 10)
    for k, x in enumerate((-300, -400, -500)):
        rr = 34 - 19 * ((-150 - x) / 470.0) + 1.2
        m.cyl(HZ if k % 2 == 0 else BLACK, (x, 0, 180), rr, 16, rot=(90, 0, 0), steps=12)
    m.side(B, [(-572, 190), (-630, 178), (-682, 330), (-648, 338), (-600, 228)], -5, 5)
    m.side(HZ, [(-640, 300), (-675, 312), (-680, 330), (-648, 338)], -5.6, 5.6)
    m.side(B2, [(-600, 172), (-640, 168), (-652, 122), (-632, 120)], -4, 4)                     # ventral fin
    rod(m, STEEL, (-646, 0, 124), (-660, 0, 112), 2.2, steps=5)                                  # tail skid
    m.box(B2, (-540, 0, 188), (46, 176, 6))
    for s in (-1, 1):
        m.box(HZ, (-540, s * 88, 194), (40, 4, 30))
        block_text(m, WHITE, 'S', (-626, s * 5.8, 236), 6.5, 'y', s, depth=1.2)
    m.cyl(STEEL, (TAIL_HUB[0], 4, TAIL_HUB[2]), 13, 12, rot=(0, 0, 90), steps=12)                # gearbox
    m.box(WHITE, (-664, 0, 341), (8, 8, 6), glow=1.6)                                            # strobe
    m.box(WHITE, (-622, 0, 180), (6, 8, 8), glow=1.2)
    # ---- skids
    for s in (-1, 1):
        m.cyl(CHROME, (-140, s * 88, 8), 5, 322, rot=(-90, 0, 0), steps=10)
        rod(m, CHROME, (181, s * 88, 8), (214, s * 88, 30), 5, steps=10)
        for x in (-80, 110):
            rod(m, BLACK, (x, s * 88, 8), (x, s * 60, 46), 4.5, steps=8)
        m.box(BLACK, (8, s * 88, 24), (60, 16, 3))                                                # step
    m.box(RED, (40, 0, 35), (16, 16, 6), glow=1.6)                                                # belly beacon
    # ---- the level's kit
    if L == 4:      # balloons tied to the skids
        for k, c in enumerate(('#ff2d3c', '#ffd23f', '#2ee6ff')):
            rod(m, '#e8e0f0', (-80 + k * 80, 88, 6), (-80 + k * 80, 150, -160), 2, steps=4)
            m.sphere(c, (-80 + k * 80, 150, -200), 40, scale=(1, 1, 1.2), steps=(6, 8), glow=0.25)
    elif L == 3:    # a flame stripe down the boom
        for k in range(6):
            m.side(GL, [(-180 - k * 60, 190), (-150 - k * 60, 190), (-180 - k * 60, 206)], -24, 24, glow=1.4)
    elif L == 1:    # a glowing toxic drum in the cabin
        m.cyl('#2a6a3a', (40, 0, 68), 22, 50, steps=12)
        m.cyl(GREEN, (40, 0, 100), 23, 8, steps=12, glow=1.0)
    return m


def main_rotor(m, R=520):
    """Hub at the origin; four blades, hazard tips, a faint blur disc and a tip ring. Spins about Z."""
    m.cyl('#2a2a33', (0, 0, -14), 26, 22, steps=14)
    m.sphere(STEEL, (0, 0, 8), 18, scale=(1, 1, 0.55), steps=(5, 10))
    m.sphere(RED, (0, 0, 17), 5, steps=(3, 6), glow=1.0)
    for k in range(4):
        a = k * 90 + 45
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        m.box(STEEL, (ca * 38, sa * 38, 0), (44, 22, 12), rot=(0, a, 0))                            # blade grip
        m.box('#3a3846', (ca * (40 + R) / 2.0, sa * (40 + R) / 2.0, 1), (R - 40, 36, 4), rot=(0, a, 5))
        m.box(HAZ, (ca * (R - 22), sa * (R - 22), 1.2), (44, 37, 4.4), rot=(0, a, 5))
        m.box(BLACK, (ca * (R - 52), sa * (R - 52), 1.2), (16, 37, 4.4), rot=(0, a, 5))
    m.cyl('@blur', (0, 0, -5), R, 1, steps=48)
    torus(m, '@blurtip', (0, 0, -4.5), R - 22, 4, major=48, minor=4)


def tail_rotor(m, R=56):
    """Hub at the origin; three blades in the XZ plane, spinning about Y."""
    disc_y(m, '#2a2a33', (0, 0), 9, -2, 10, steps=10)
    disc_y(m, RED, (0, 0), 4, 10, 12, steps=8)
    for k in range(3):
        a = k * 120 + 90
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        m.box('#3a3846', (ca * (8 + R) / 2.0, 4, sa * (8 + R) / 2.0), (R - 8, 3, 13), rot=(a, 0, 0))
        m.box(HAZ, (ca * (R - 8), 4, sa * (R - 8)), (16, 3.4, 13.5), rot=(a, 0, 0))
    m.cyl('@blur', (0, 0.5, 0), R, 1, rot=(0, 0, 90), steps=28)


def heli_palette(L):
    import ft_v28
    if not L:
        return HELI_CLASSIC
    T = ft_v28.THEMES[L]
    return dict(body=T['heli'], body2=T['heli2'], stripe=T['heli_stripe'], glow=T['heli_glow'])


def export_heli(levels=(0, 1, 2, 3, 4, 5, 6, 7)):
    out = []
    C = '/Game/Characters/'
    for L in levels:
        mm = ft_art.Model('heli')
        heli33(mm, heli_palette(L), L)
        p = C + 'Choppers/SM_Heli' if not L else C + 'Themed/L%d/SM_Heli' % L
        save(mm, p, collision='box')
        out.append(p)
    if 0 in levels:
        r = ft_art.Model('rotor'); main_rotor(r); save(r, C + 'Choppers/SM_HeliRotor')
        t = ft_art.Model('trotor'); tail_rotor(t); save(t, C + 'Choppers/SM_HeliTailRotor')
        out += [C + 'Choppers/SM_HeliRotor', C + 'Choppers/SM_HeliTailRotor']
    return out


# =============================================================================================== SWAT TEAM
SWAT_CLASSIC = dict(fat='#26304d', vest='#121219', vest2='#2a2c3a', helm='#1b1d28', lens='#ff4fb8', web=HAZ)
FAT_DARK = {1: '#2b2f2a', 2: '#3a3026', 3: '#1e1820', 4: '#1d2233', 5: '#1a2440', 6: '#1e2a36', 7: '#141218'}
SKIN = '#c98a5e'
BALA = '#121117'
GLOVE = '#16151b'
BOOT = '#141318'


def lum(h):
    r, g, b = ft_art.hex_lin(h)
    return 0.2126 * r + 0.7152 * g + 0.0722 * b


def swat_kit(L):
    if not L:
        K = dict(SWAT_CLASSIC)
    else:
        import ft_v28
        T = ft_v28.THEMES[L]
        vest, vest2 = (('#c9a96a', '#a8834e') if L == 2 else (T['suit'], T['suit2']))
        K = dict(fat=FAT_DARK[L], vest=vest, vest2=vest2, helm=T['helmet'], lens=T['visor'], web=T['stripe'] if L != 7 else T['tie'])
    K['letter'] = '#1c1a26' if lum(K['vest']) > 0.25 else ('#f2efe4' if not L else ('#ffe28a' if L == 4 else ('#ffc93c' if L == 7 else '#f4f9ff')))
    return K


def swat_head(m, K):
    """Head frame: origin at the head's centre, face toward +X."""
    m.sphere(BALA, (0, 0, -1), 11, scale=(1, 0.92, 1.08), steps=(6, 10))                 # balaclava
    m.box(SKIN, (9.6, 0, 2.4), (3, 15, 5))                                                 # the eyes' slit
    m.box(BLACK, (10.4, 0, 2.6), (3, 20, 7.5))                                             # goggle frame
    for s in (-1, 1):
        m.box(K['lens'], (11.6, s * 4.6, 2.6), (1.6, 7.6, 5.2), glow=0.7)
    m.sphere(K['helm'], (-1, 0, 8.5), 13.4, scale=(1.08, 1.02, 0.68), steps=(6, 12))       # the helmet shell
    m.box(K['helm'], (10, 0, 6.2), (9, 22, 2))                                             # brim
    for s in (-1, 1):
        m.box(BLACK, (-1, s * 13.4, 7), (16, 2, 4))                                        # side rails
        m.box(K['web'], (-8, s * 10, 0), (3, 3, 12))                                       # chin strap
    m.box(BLACK, (-1, 0, 7.4), (30, 27.6, 1.6))                                            # goggle strap
    m.box(DK2, (9, 0, 13.5), (4, 7, 5))                                                    # NVG mount
    m.cyl(BALA, (-1, 0, -21), 6.5, 10, steps=8)                                            # neck


def swat_torso(m, K):
    """Torso frame: origin at the waist centre, chest toward +X."""
    F, V, V2 = K['fat'], K['vest'], K['vest2']
    m.box(F, (0, 0, -6), (24, 34, 16))                                                     # hips
    m.box(F, (0, 0, 26), (22, 34, 48))                                                     # shirt
    m.box(V, (1, 0, 28), (31, 43, 36))                                                     # the vest
    for s in (-1, 1):
        m.box(V, (0, s * 14, 46), (27, 9, 6))                                              # shoulder straps
        m.sphere(V, (0, s * 21, 44), 8, scale=(1.1, 0.9, 0.8), steps=(5, 8))               # shoulder pads
        m.box(V2, (0, s * 20.6, 22), (20, 1.5, 16))                                        # side panels
    m.box(BALA, (0, 0, 50), (20, 22, 5))                                                   # collar
    # front: three mag pouches, a radio pouch with an aerial, the letters
    for y in (-10, 0, 10):
        m.box(V2, (17.5, y, 17), (5, 8.6, 13))
        m.box(BLACK, (20, y, 22), (1, 7, 2.5))
    m.box(V2, (17, 13, 36), (4, 8, 10))
    rod(m, BLACK, (17, 15, 41), (14, 17, 60), 0.7, steps=4)
    block_text(m, K['letter'], 'SWAT', (17.1, -2, 37), 1.25, 'x', -1)
    # back: a light block-letter panel
    block_text(m, K['letter'], 'SWAT', (-15.3, 0, 31), 2.0, 'x', 1)
    m.box(V2, (-16, 0, 15), (3, 30, 8))
    # the belt, holster pouches, the rappel harness and its carabiner
    m.box('#26252c', (0, 0, 5), (26, 38, 7))
    m.box(STEEL, (13.6, 0, 5), (1, 6, 5))
    for s in (-1, 1):
        m.box(V2, (2, s * 19.5, 4), (10, 5, 10))
    m.box(K['web'], (0, 0, -1), (27, 39, 3))
    for s in (-1, 1):
        tilt_box(m, K['web'], (12.5, s * 4, -1), (8, s * 12, -14), 3, 2)
        tilt_box(m, K['web'], (-12, s * 6, 2), (-12, s * 12, 46), 3, 2)                    # back straps
        tilt_box(m, K['web'], (14, s * 6, 2), (14.5, s * 12, 46), 3, 2)                    # chest straps
    torus(m, CHROME, (15.5, 0, -4), 3.6, 1.0, rot=(0, 0, 90), major=10, minor=4)           # carabiner
    m.box(STEEL, (17, 0, -10), (2, 5, 9))                                                  # descender


def swat_limbs(m, K, J):
    F = K['fat']
    for s in ('r', 'l'):
        sh, el, ha = J['sh_' + s], J['el_' + s], J['ha_' + s]
        rod(m, F, sh, el, 6.4, steps=8)
        m.sphere(F, el, 6.2, steps=(4, 6))
        m.sphere(BLACK, el, 5.9, scale=(0.5, 1, 0.5), steps=(4, 6))                        # elbow pad
        rod(m, F, el, ha, 5.7, steps=8)
        m.sphere(GLOVE, ha, 6.2, steps=(4, 7))                                             # glove (a fist)
        hip, kn, an, toe = J['hip_' + s], J['kn_' + s], J['an_' + s], J['toe_' + s]
        rod(m, F, hip, kn, 8.6, steps=8)
        m.sphere(F, kn, 7.8, steps=(4, 6))
        # the knee pad sits on the front of the knee (toward the toe side of the shin's normal)
        fx, fy, fz = toe[0] - an[0], toe[1] - an[1], 0.0
        fl = math.hypot(fx, fy) or 1.0
        m.sphere(BLACK, (kn[0] + fx / fl * 6.2, kn[1] + fy / fl * 6.2, kn[2]), 7.4, scale=(0.75, 0.9, 1.0), steps=(4, 6))
        rod(m, F, kn, an, 7.2, steps=8)
        m.sphere(BOOT, an, 7.6, steps=(4, 6))
        tilt_box(m, BOOT, (an[0] - fx / fl * 5, an[1] - fy / fl * 5, an[2] - 4), (toe[0], toe[1], toe[2]), 11, 9)
    # a holster on the right thigh
    h, k = J['hip_r'], J['kn_r']
    c = (h[0] * 0.55 + k[0] * 0.45, h[1] * 0.55 + k[1] * 0.45, h[2] * 0.55 + k[2] * 0.45)
    ox, oy = J['hip_r'][0] - J['hip_l'][0], J['hip_r'][1] - J['hip_l'][1]
    ol = math.hypot(ox, oy) or 1.0
    m.box(BLACK, (c[0] + ox / ol * 8.5, c[1] + oy / ol * 8.5, c[2]), (10, 10, 16))


def trooper(m, K, torso_loc, torso_rot, head_loc, head_rot, J):
    J = dict(J)
    J['sh_r'] = xfp(torso_loc, torso_rot, (0, 21, 45))
    J['sh_l'] = xfp(torso_loc, torso_rot, (0, -21, 45))
    J['hip_r'] = xfp(torso_loc, torso_rot, (0, 10, -6))
    J['hip_l'] = xfp(torso_loc, torso_rot, (0, -10, -6))
    with_base(m, torso_loc, torso_rot, lambda: swat_torso(m, K))
    with_base(m, head_loc, head_rot, lambda: swat_head(m, K))
    swat_limbs(m, K, J)
    return J


def swat_rappel(m, K):
    """Rappelling: the right hand above the head grips the rope (which runs up +Z at x = y = 0), the left hand out to
    the side brakes, legs a little bent. Origin at the feet, facing +X, ~180 cm."""
    J = dict(el_r=(-10, 17, 176), ha_r=(0, 0, 202), el_l=(-15, -45, 126), ha_l=(-7, -63, 108),
             kn_r=(-3, 12, 52), an_r=(-15, 13, 11), toe_r=(3, 13, 5),
             kn_l=(1, -14, 57), an_l=(-11, -15, 13), toe_l=(6, -16, 9), frame='x')
    J = trooper(m, K, (-17, 0, 98), (6, 0, 0), (-20, 0, 166), (4, 0, 0), J)
    ROPE = '#e8dcb0'
    rod(m, ROPE, (0, 0, 196), (0, 0, 236), 1.6, steps=6)               # the rope above the fist (the game's rope meets it)
    rod(m, ROPE, (0, 0, 196), (-1.0, 0, 100), 1.6, steps=6)            # down to the carabiner
    rod(m, ROPE, (-1.0, 0, 100), (-6, -60, 106), 1.6, steps=6)         # to the brake hand
    rod(m, ROPE, (-7, -63, 106), (-14, -72, 18), 1.6, steps=6)         # the tail hanging down
    return J


def swat_hang(m, K):
    """Clinging to a car door: hands at the origin gripping the roof edge, chest toward +Y (the car), body hanging down
    the car's flank on the -Y side, knees up, one boot braced on the door, one dangling. ~175 cm from grip to boots."""
    J = dict(ha_r=(-13, 0, 0), el_r=(-29, -20, -20), ha_l=(13, 0, 0), el_l=(29, -20, -20),
             kn_r=(-18, -68, -118), an_r=(-16, -32, -146), toe_r=(-16, -19, -149),
             kn_l=(16, -58, -138), an_l=(16, -30, -168), toe_l=(16, -17, -172), frame='y')
    return trooper(m, K, (0, -31.9, -92.4), (-15, 90, 0), (0, -16, -31), (24, 90, 0), J)


def export_swat(levels=(0, 1, 2, 3, 4, 5, 6, 7)):
    out = []
    for L in levels:
        mm = ft_art.Model('swat')
        swat_rappel(mm, swat_kit(L))
        p = '/Game/Characters/Syndicate/SM_Henchman' if not L else '/Game/Characters/Themed/L%d/SM_Henchman' % L
        save(mm, p, collision='box', kind='prop')                 # matte cloth, not car paint
        out.append(p)
    if 0 in levels:
        mm = ft_art.Model('swat_hang')
        swat_hang(mm, swat_kit(0))
        save(mm, '/Game/Characters/Syndicate/SM_HenchmanHang', collision='box', kind='prop')
        out.append('/Game/Characters/Syndicate/SM_HenchmanHang')
    return out


# =============================================================================================== WHEELS
def foe_wheel(m, rim='#5a5f6e', spoke='#5a5f6e', cap=CHROME, nut=HAZ, wall=None, r=50.0, w=40.0, rows=2, lugs=12):
    """A LEFT wheel centred at the origin, axle along Y, the rim and hub on the -Y face. Tread blocks stand proud of the
    tyre so the spin reads; the pattern repeats every 360/lugs degrees."""
    rb = r - 4.5
    disc_y(m, TYRE, (0, 0), rb, -w / 2.0, w / 2.0, steps=24)
    for row in range(rows):
        yb = -w / 4.0 if row == 0 else w / 4.0
        for k in range(lugs):
            a = (k + 0.5 * row) * 360.0 / lugs
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            m.box('#34303d', (ca * (rb + 2), yb, sa * (rb + 2)), (2 * math.pi * rb / lugs * 0.55, w * 0.44, 5), rot=(a + 90, 0, 0))
    # shoulder blocks on the outer edge of the tread (they read from the side)
    for k in range(lugs):
        a = (k + 0.25) * 360.0 / lugs
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        m.box('#34303d', (ca * (rb - 3), -w / 2.0 - 0.6, sa * (rb - 3)), (2 * math.pi * rb / lugs * 0.4, 2, 7), rot=(a + 90, 0, 0))
    yf = -w / 2.0
    if wall:
        disc_y(m, wall, (0, 0), r * 0.74, yf - 0.8, yf + 1, steps=24)
        disc_y(m, TYRE, (0, 0), r * 0.64, yf - 1.2, yf + 1, steps=24)
    disc_y(m, rim, (0, 0), r * 0.6, yf - 1.6, yf + 2, steps=24)                       # the rim lip
    disc_y(m, '#24222b', (0, 0), r * 0.5, yf - 1.9, yf + 2, steps=20)                 # the dish
    for k in range(6):
        a = k * 60.0 + 30
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        m.box(spoke, (ca * r * 0.3, yf - 2.6, sa * r * 0.3), (r * 0.42, 2.4, r * 0.12), rot=(a, 0, 0))
    disc_y(m, cap, (0, 0), r * 0.17, yf - 4.5, yf + 1, steps=12)
    m.box(RED, (0, yf - 4.8, 0), (r * 0.12, 1, r * 0.12))
    for k in range(6):
        a = math.radians(k * 60.0)
        disc_y(m, nut, (math.cos(a) * r * 0.24, math.sin(a) * r * 0.24), r * 0.035, yf - 3.8, yf, steps=6)
    disc_y(m, '#2a2a33', (0, 0), r * 0.36, w / 2.0 - 1, w / 2.0 + 2.5, steps=14)       # brake drum on the inside


def strut(m):
    """A coil-over: shock body, chrome piston rod, spring seats and a yellow coil. Axis Z, z 0..100, 12 cm across."""
    m.box(BLACK, (0, 0, 2), (6, 8, 4))
    m.cyl('#1c1a26', (0, 0, 2), 3.6, 56, steps=10)
    m.cyl(CHROME, (0, 0, 56), 1.7, 40, steps=8)
    m.cyl(STEEL, (0, 0, 9), 6, 2, steps=14)
    m.cyl(STEEL, (0, 0, 90), 6, 2, steps=14)
    m.cyl('#1c1a26', (0, 0, 92), 4, 4, steps=10)
    m.box(BLACK, (0, 0, 98), (6, 8, 4))
    turns, seg = 7, 10
    pts = []
    for i in range(turns * seg + 1):
        a = 2 * math.pi * i / seg
        pts.append((math.cos(a) * 4.9, math.sin(a) * 4.9, 11 + 78.0 * i / (turns * seg)))
    for a, b in zip(pts, pts[1:]):
        rod(m, HAZ, a, b, 1.05, steps=5)


def export_wheels():
    D = '/Game/Characters/Wheels/'
    out = []
    for name, kw in (('SM_FoeWheel', {}), ('SM_FoeWheelChrome', dict(rim=CHROME, spoke=CHROME, cap=WHITE, nut=STEEL, wall='#f4f1e6'))):
        mm = ft_art.Model(name)
        foe_wheel(mm, **kw)
        save(mm, D + name)
        out.append(D + name)
    mm = ft_art.Model('strut')
    strut(mm)
    save(mm, D + 'SM_FoeStrut')
    out.append(D + 'SM_FoeStrut')
    return out


# ------------------------------------------------------------------------------------------- body-only vehicles
def _combined(meshes):
    c = unreal.DynamicMesh()
    for (col, glow), v in meshes.items():
        if not col.startswith('@'):
            unreal.GeometryScript_MeshEdits.append_mesh(c, v, unreal.Transform())
    return c


def _bounds(meshes):
    b = unreal.GeometryScript_MeshQueries.get_mesh_bounding_box(_combined(meshes))
    return [b.min.x, b.min.y, b.min.z], [b.max.x, b.max.y, b.max.z]


def underside(m, wheels):
    """Dark wheel wells on the body's flank round the top of every wheel (found by ray-casting the body), a pair of
    dark chassis rails and a cross member per axle, so the body never looks hollow without its wheels."""
    if not wheels:
        return
    body = _combined(m.meshes)
    S = unreal.GeometryScript_MeshSpatial
    bvh = S.build_bvh_for_mesh(body)
    if isinstance(bvh, tuple):
        bvh = [v for v in bvh if isinstance(v, unreal.GeometryScriptDynamicMeshBVH)][0]
    opts = unreal.GeometryScriptSpatialQueryOptions()

    def cast(o, d):
        res = S.find_nearest_ray_intersection_with_mesh(body, bvh, unreal.Vector(*o), unreal.Vector(*d), opts)
        hit = [v for v in res if isinstance(v, unreal.GeometryScriptRayHitResult)][0] if isinstance(res, tuple) else res
        if hit is None or not hit.get_editor_property('hit'):
            return None
        return hit.get_editor_property('hit_position')

    lo, hi = _bounds(m.meshes)
    for (x, yc, z, r, w, side) in wheels:
        s = 1 if yc >= 0 else -1
        inner = abs(yc) - w / 2.0
        # the flank: cast in from outside at the arch's shoulder height
        hp = cast((x, s * (hi[1] - lo[1] + 2000), z + r * 0.55), (0, -s, 0))
        if hp is None or abs(hp.y) < inner - 4 or abs(hp.y) > abs(yc) + w / 2.0 + 2:
            continue
        fy = abs(hp.y)
        R = r + 5
        pts = [(x + R, z)] + [(x + R * math.cos(math.radians(a)), z + R * math.sin(math.radians(a))) for a in range(15, 180, 15)] + [(x - R, z)]
        y0, y1 = fy - 0.2, fy + 1.0
        m.side(DK, pts, *((y0, y1) if s > 0 else (-y1, -y0)))
        # an inner fender behind the tyre, up inside the arch
        m.box(DK, (x, s * (inner - 2), z + r * 0.45), (2 * r, 3, r * 0.9))
    xs = [wv[0] for wv in wheels]
    rs = max(wv[3] for wv in wheels)
    inner = min(abs(wv[1]) - wv[4] / 2.0 for wv in wheels)
    zr = min(wv[2] for wv in wheels)
    x0, x1 = min(xs) - rs * 0.8, max(xs) + rs * 0.8
    for s in (-1, 1):
        m.box(DK, ((x0 + x1) / 2.0, s * (inner - 14), zr), (x1 - x0, 10, 16))
    for x in sorted(set(round(v, 1) for v in xs)):
        m.box(DK2, (x, 0, zr), (14, 2 * (inner - 8), 10))
        m.cyl(DK2, (x, 0, zr - 10), 14, 20, steps=10)            # a diff / axle housing


def split_vehicle(path, build, offset):
    """Build body-only (wheels skipped), add the underside, save to path; returns its wheels in the final mesh's space."""
    ft_car.WHEEL_MODE = 'skip'
    try:
        mm = ft_art.Model('split')
        build(mm)
    finally:
        ft_car.WHEEL_MODE = 'normal'
    wheels = list(getattr(mm, 'wheels', []))
    lo, hi = _bounds(mm.meshes)
    for (x, yc, z, r, w, side) in wheels:
        lo = [min(lo[0], x - r), min(lo[1], yc - w / 2.0), min(lo[2], z - r)]
        hi = [max(hi[0], x + r), max(hi[1], yc + w / 2.0), max(hi[2], z + r)]
    underside(mm, wheels)
    coll = (((lo[0] + hi[0]) / 2.0 + offset[0], (lo[1] + hi[1]) / 2.0 + offset[1], (lo[2] + hi[2]) / 2.0 + offset[2],
             max(hi[0] - lo[0], 2), max(hi[1] - lo[1], 2), max(hi[2] - lo[2], 2)))
    save(mm, path, offset, collision=coll)
    return [[round(x + offset[0], 2), round(yc + offset[1], 2), round(z + offset[2], 2), round(r, 2), round(w, 2)]
            for (x, yc, z, r, w, side) in wheels]


def split_jobs():
    import ft_chem, ft_desert, ft_v25, ft_v28
    C = '/Game/Characters/'
    J = {}
    J['fleet'] = [
        (C + 'Syndicate/SM_Rammer', lambda m: ft_fleet.rammer(m), (-235, 0, 0)),
        (C + 'Syndicate/SM_Suv', lambda m: ft_fleet.suv(m), (-260, 0, 0)),
        (C + 'Syndicate/SM_Truck', lambda m: ft_fleet.truck(m), (-450, 0, 0)),
        (C + 'Traffic/SM_Bus', lambda m: ft_fleet.bus(m), (-550, 0, 0)),
        (C + 'Traffic/SM_CarCarrier', lambda m: ft_fleet.ramp(m), (-565, 0, 0)),
        (C + 'Bosses/SM_Juggernaut', lambda m: ft_fleet.truck(m, True), (-625, 0, 0)),
        (C + 'Bosses/SM_Carrier', lambda m: ft_desert.carrier(m), (-500, 0, 0)),
    ]
    civ = [('SM_Sedan0', '#fff1d6', 'sedan'), ('SM_Sedan1', '#8fe3ff', 'sedan'), ('SM_Sedan2', '#ffe14d', 'sedan'),
           ('SM_Hatch', '#7fe3c4', 'hatch'), ('SM_Hatch1', '#b9a4ff', 'hatch'), ('SM_Van', '#ff9ec7', 'van'), ('SM_Van1', '#ffc59e', 'van')]
    J['traffic'] = []
    for name, col, kind in civ:
        Lg = {'sedan': 460, 'hatch': 390, 'van': 520}[kind]
        J['traffic'].append((C + 'Traffic/' + name, (lambda m, c=col, k=kind: ft_fleet.sedan(m, c, k)), (-Lg / 2.0, 0, 0)))
    J['bosses'] = [
        (C + 'Bosses/SM_FireTruck', ft_v25.fire_truck, (-625, 0, 0)),
        (C + 'Bosses/SM_ClownCar', ft_v25.clown_car, (-625, 0, 0)),
        (C + 'Bosses/SM_SharkCar', ft_v25.shark_car, (-625, 0, 0)),
    ]
    J['themed'] = []
    for L in sorted(ft_v28.THEMES):
        T = ft_v28.THEMES[L]
        for name, fn, off in (('SM_Rammer', ft_v28.rammer, (-235, 0, 0)), ('SM_Suv', ft_v28.suv, (-260, 0, 0)), ('SM_Truck', ft_v28.truck, (-450, 0, 0))):
            J['themed'].append(('%sThemed/L%d/%s' % (C, L, name), (lambda m, f=fn, L=L, T=T: f(m, L, T)), off))
    return J


def load_wheels_json():
    if os.path.exists(WHEELS_JSON):
        with open(WHEELS_JSON) as f:
            return json.load(f)
    return {}


def write_wheels_json(d):
    os.makedirs(os.path.dirname(WHEELS_JSON), exist_ok=True)
    with open(WHEELS_JSON, 'w') as f:
        f.write('{\n' + ',\n'.join('  %s: %s' % (json.dumps(k), json.dumps(d[k])) for k in sorted(d)) + '\n}\n')


def export_split(group):
    d = load_wheels_json()
    out = []
    for path, build, off in split_jobs()[group]:
        d[path] = split_vehicle(path, build, off)
        out.append('%s %d wheels' % (path, len(d[path])))
    write_wheels_json(d)
    return out


# =============================================================================================== TESLA TRUCK
W_BOSS, L_BOSS = 64 * U, 1250.0
COIL_X = 440.0
TOP_Z = 880.0                     # the toroid's centre above the road
TESLA_TOP = (COIL_X - 625.0, 0.0, TOP_Z)
COPPER = '#ff9a3c'
COPPER_D = '#7a3a18'
INSUL = '#efe6d8'
TNAVY = '#1c1f4a'
TSTEEL = '#2a3f8f'
VIOLET = '#b44dff'
ARC = '#bff6ff'


def tesla_truck(m):
    """THE ALCHEMIST's Tesla truck: a heavy armoured cab-over hauling a flatbed with a huge Tesla coil (insulator stack,
    glowing copper winding, capacitor jars, cables, spark gaps). The toroid and the top of the coil are SM_TeslaTop."""
    W, L = W_BOSS, L_BOSS
    hw = W / 2 - 16
    # chassis, deck, wheels
    m.box(BLACK, (L * 0.47, 0, 92), (L * 0.94, hw * 1.6, 44))
    for s in (-1, 1):
        for x in (0.08, 0.19, 0.64, 0.9):
            ft_car.wheel(m, L * x, 58, 58, hw - 50, W / 2.0, s)
    DECK_T = 136
    m.box('#4a4e5c', (L * 0.37, 0, 124), (L * 0.74, hw * 2, 24))
    for k in range(14):
        m.box('#5a5f70', (30 + k * 64, 0, DECK_T + 0.5), (40, hw * 1.9, 1))       # deck plates
    for s in (-1, 1):
        for k in range(18):                                                      # hazard edge
            m.box(HAZ if k % 2 == 0 else BLACK, ((k + 0.5) * L * 0.74 / 18, s * (hw + 1), 124), (L * 0.74 / 18, 3, 24))
        # armour skirts over the wheels
        for x0, x1 in ((40, 300), (740, 1180)):
            m.box(TSTEEL, ((x0 + x1) / 2.0, s * (W / 2.0 + 2), 128), (x1 - x0, 6, 36))
            for k in range(int((x1 - x0) / 60)):
                m.sphere(STEEL, (x0 + 30 + k * 60, s * (W / 2.0 + 5.5), 128), 3, steps=(3, 5))
    # ---- the rear: an armoured tailgate with angry red eyes, tail lights and a hazard bumper
    m.box(TSTEEL, (8, 0, 190), (16, hw * 1.7, 100))
    for s in (-1, 1):
        tilt_box(m, RED, (-1, s * 40, 214), (-1, s * 130, 200), 14, 4, glow=2.2)
        m.box(RED, (-2, s * (hw - 30), 92), (3, 50, 24), glow=1.4)
    m.box('#ff8a2a', (-1.5, 0, 168), (2, 120, 8), glow=1.0)
    for k in range(10):
        m.box(HAZ if k % 2 == 0 else BLACK, (-4, -hw + (k + 0.5) * hw * 0.2, 60), (8, hw * 0.2, 30))
    # ---- the coil: base platform, insulator stack, primary ring, the secondary winding (top in SM_TeslaTop)
    cx = COIL_X
    m.cyl(TNAVY, (cx, 0, DECK_T), 210, 18, steps=8)
    for k in range(16):
        a = 2 * math.pi * (k + 0.5) / 16
        m.box(HAZ if k % 2 == 0 else BLACK, (cx + math.cos(a) * 196, math.sin(a) * 196, DECK_T + 9), (78, 6, 18), rot=(0, math.degrees(a) + 90, 0))
    m.cyl(TSTEEL, (cx, 0, DECK_T + 18), 150, 26, steps=8)
    z = DECK_T + 44
    for k in range(6):                                                             # the insulator stack
        m.cyl(INSUL, (cx, 0, z), 112 if k % 2 == 0 else 86, 20, steps=16)
        z += 20
    m.cyl(TNAVY, (cx, 0, z), 120, 12, steps=16)
    z += 12
    for k in range(3):                                                             # the primary: thick copper rings
        torus(m, COPPER, (cx, 0, z + 8 + k * 14), 128 - k * 6, 6, major=24, minor=6, glow=0.25)
    for k in range(6):
        a = k * 60
        rod(m, INSUL, (cx + math.cos(math.radians(a)) * 100, math.sin(math.radians(a)) * 100, z), (cx + math.cos(math.radians(a)) * 128, math.sin(math.radians(a)) * 128, z + 40), 4, steps=6)
    # the secondary column: copper windings that glow, dark gaps, steel bands
    COIL_R = 66
    m.cyl(COPPER_D, (cx, 0, z), COIL_R, TOP_Z - 60 - z, steps=18)
    zz = z + 10
    k = 0
    while zz < TOP_Z - 70:
        m.cyl(COPPER, (cx, 0, zz), COIL_R + 2, 9, steps=18, glow=0.3 if k % 4 else 0.8)
        zz += 15
        k += 1
    for zb in (z + 4, (z + TOP_Z - 60) / 2.0):
        m.cyl(STEEL, (cx, 0, zb), COIL_R + 5, 10, steps=18)
    # spark gaps: arms out to the sides with electrode balls, and deck-level electrodes on posts
    for s in (-1, 1):
        za = 560
        m.cyl(TNAVY, (cx, s * (COIL_R - 4), za - 9), 18, 18, rot=(0, 0, 90 * s), steps=10)
        rod(m, INSUL, (cx, s * COIL_R, za), (cx, s * 200, za + 30), 8, steps=8)
        rod(m, CHROME, (cx, s * 200, za + 30), (cx, s * 330, za + 40), 6, steps=8)
        m.sphere(CHROME, (cx, s * 336, za + 40), 22, steps=(6, 10))
        m.sphere(ARC, (cx, s * 336, za + 40), 9, steps=(4, 6), glow=3.0)
        for xo in (-260, 260):
            m.cyl(TNAVY, (cx + xo, s * (hw - 40), DECK_T), 16, 140, steps=10)
            m.cyl(INSUL, (cx + xo, s * (hw - 40), DECK_T + 60), 26, 14, steps=12)
            m.cyl(INSUL, (cx + xo, s * (hw - 40), DECK_T + 90), 26, 14, steps=12)
            rod(m, CHROME, (cx + xo, s * (hw - 40), DECK_T + 140), (cx + xo, s * (hw + 70), DECK_T + 170), 5, steps=6)
            m.sphere(CHROME, (cx + xo, s * (hw + 76), DECK_T + 172), 16, steps=(5, 8))
    # the capacitor bank: a ring of glass jars with copper caps and terminal balls, cabled to the base
    for k in range(8):
        a = 2 * math.pi * (k + 0.5) / 8
        jx, jy = cx + math.cos(a) * 236, math.sin(a) * 210
        m.cyl('#233a63', (jx, jy, DECK_T), 30, 96, steps=12)
        m.cyl(VIOLET, (jx, jy, DECK_T + 20), 31, 46, steps=12, glow=0.25)
        m.cyl(COPPER, (jx, jy, DECK_T + 96), 32, 10, steps=12, glow=0.3)
        rod(m, CHROME, (jx, jy, DECK_T + 106), (jx, jy, DECK_T + 126), 3, steps=6)
        m.sphere(CHROME, (jx, jy, DECK_T + 130), 8, steps=(4, 6))
        # the cable arcs over to the platform
        mx, my = cx + math.cos(a) * 170, math.sin(a) * 150
        rod(m, BLACK, (jx, jy, DECK_T + 130), (mx, my, DECK_T + 150), 5, steps=6)
        rod(m, BLACK, (mx, my, DECK_T + 150), (cx + math.cos(a) * 140, math.sin(a) * 140, DECK_T + 40), 5, steps=6)
    # ---- the dynamo behind the cab, with fins, gauges and the fat feed cable
    m.box(TSTEEL, (850, 0, 230), (150, hw * 1.3, 190))
    for k in range(7):
        m.box(TNAVY, (790 + k * 20, 0, 328), (8, hw * 1.2, 16))
    for s in (-1, 1):
        for k in range(3):
            m.cyl('#fff1c0', (800 + k * 50, s * (hw * 0.65 + 1), 260), 14, 2, rot=(0, 0, 90 * s), steps=12, glow=0.8)
            m.box(RED, (800 + k * 50, s * (hw * 0.65 + 2.5), 262), (2, 1, 11), rot=(30 * k, 0, 0))
    for s in (-1, 1):
        rod(m, BLACK, (776, s * 60, 200), (620, s * 90, DECK_T + 30), 12, steps=8)
        rod(m, BLACK, (620, s * 90, DECK_T + 30), (cx + 150, s * 40, DECK_T + 30), 12, steps=8)
    # ---- the cab: armoured cab-over, slit windshield, a mean grille, red eyes, ram, stacks, light bar
    CAB = '#2a3f8f'
    m.side(CAB, [(L * 0.76, 100), (L, 100), (L + 6, 200), (L - 10, 300), (L * 0.78, 312), (L * 0.76, 300)], -hw + 6, hw - 6)
    m.side(GLASS, [(L - 6, 236), (L + 1, 236), (L - 9, 290), (L - 18, 290)], -hw + 30, hw - 30)
    for k in range(4):                                                            # armour louvres over the glass
        m.box(TNAVY, (L - 4 - k * 2.6, 0, 246 + k * 13), (8, hw * 2 - 70, 4))
    for s in (-1, 1):
        m.box(TNAVY, (L * 0.88, s * (hw - 4), 200), (L * 0.2, 6, 160))           # door armour
        for (px, pz) in ((L * 0.8, 140), (L * 0.96, 140), (L * 0.8, 264), (L * 0.96, 264)):
            m.sphere(STEEL, (px, s * (hw - 0.5), pz), 3, steps=(3, 5))
        m.box(GLASS, (L * 0.9, s * (hw - 2), 260), (90, 2, 34))
        m.box(HAZ, (L * 0.88, s * (hw - 0.4), 120), (L * 0.2, 2, 10))
        m.cyl(CHROME, (L * 0.76 + 10, s * (hw - 18), 300), 13, 170, steps=12)  # exhaust stacks
        m.cyl(BLACK, (L * 0.76 + 10, s * (hw - 18), 468), 10, 4, steps=12)
        m.cyl(CHROME, (L * 0.92, s * 70, 312), 8, 60, rot=(-90, 0, 0), steps=10, top_r=14)   # air horns
    # the grille: a deep black mouth with chrome teeth, and two slanted red eyes over it
    m.box(BLACK, (L + 4, 0, 160), (6, hw * 1.3, 90))
    for k in range(11):
        m.box(CHROME, (L + 7.5, -hw * 0.6 + k * hw * 0.12, 160), (3, 7, 84))
    m.box(CHROME, (L + 8, 0, 206), (4, hw * 1.36, 8))
    for s in (-1, 1):
        tilt_box(m, RED, (L + 5, s * 40, 214), (L + 5, s * 150, 232), 16, 4, glow=2.4)
        m.box('#fff1c0', (L + 6, s * (hw - 40), 130), (3, 40, 22), glow=1.2)
    # ram bar in hazard blocks
    for k in range(9):
        m.box(HAZ if k % 2 == 0 else BLACK, (L + 24, -hw + (k + 0.5) * hw * 2 / 9.0, 110), (14, hw * 2 / 9.0, 36))
    for s in (-1, 1):
        m.box(STEEL, (L + 12, s * hw * 0.5, 110), (24, 14, 20))
    # roof light bar
    m.box(BLACK, (L * 0.9, 0, 318), (30, hw * 1.4, 10))
    for k in range(6):
        m.box(ARC if k % 2 else '#fff1c0', (L * 0.9 + 14, -hw * 0.6 + k * hw * 0.24, 324), (4, 30, 14), glow=1.4)


def tesla_top(m):
    """The toroid and the top of the coil, centred on the toroid's centre (spins about Z, its glow pulses)."""
    R, r = 160.0, 50.0
    torus(m, '#bfe4ff', (0, 0, 0), R, r, major=28, minor=10, glow=0.12)
    # four spokes from the hub to the ring, a top dome with a breakout point
    for k in range(4):
        a = k * 90 + 45
        rod(m, STEEL, (0, 0, 0), (math.cos(math.radians(a)) * (R - 30), math.sin(math.radians(a)) * (R - 30), 0), 8, steps=8)
    m.cyl(STEEL, (0, 0, -20), 74, 30, steps=18)
    m.sphere('#bfe4ff', (0, 0, 10), 80, scale=(1, 1, 0.45), steps=(6, 18), glow=0.12)
    rod(m, CHROME, (0, 0, 40), (0, 0, 90), 6, steps=8)
    m.sphere(ARC, (0, 0, 98), 16, steps=(5, 8), glow=3.0)
    # the top of the winding (the column below it ends at TOP_Z - 60)
    m.cyl(COPPER_D, (0, 0, -60), 66, 40, steps=18)
    for k in range(3):
        m.cyl(COPPER, (0, 0, -58 + k * 13), 68, 8, steps=18, glow=0.6)
    # glowing studs round the ring: the pulse reads on them
    for k in range(8):
        a = math.radians(k * 45)
        m.sphere(ARC, (math.cos(a) * R, math.sin(a) * R, r - 4), 9, steps=(4, 6), glow=2.5)


def tesla_arc(m, h=1000.0, seed=9):
    """A jagged lightning bolt from (0,0,0) down to (0,0,-h), about 30 cm thick, with a few side forks."""
    rr = random.Random(seed)
    n = 11
    pts = [(0.0, 0.0, 0.0)]
    for k in range(1, n):
        pts.append((rr.uniform(-70, 70), rr.uniform(-70, 70), -h * k / n))
    pts.append((0.0, 0.0, -h))
    for a, b in zip(pts, pts[1:]):
        tilt_box(m, '#f4ffff', a, b, 18, 18, glow=3.0)
        tilt_box(m, '#7fe8ff', a, b, 32, 10, glow=2.0)
        tilt_box(m, '#7fe8ff', a, b, 10, 32, glow=2.0)
    for k in (3, 6, 8):
        q = pts[k]
        d = rr.choice((-1, 1))
        e1 = (q[0] + rr.uniform(-60, 60), q[1] + d * rr.uniform(120, 200), q[2] - rr.uniform(60, 120))
        e2 = (e1[0] + rr.uniform(-40, 40), e1[1] + d * rr.uniform(60, 120), e1[2] - rr.uniform(80, 140))
        tilt_box(m, '#f4ffff', q, e1, 10, 10, glow=3.0)
        tilt_box(m, '#f4ffff', e1, e2, 7, 7, glow=3.0)


def export_tesla():
    B = '/Game/Characters/Bosses/'
    d = load_wheels_json()
    d[B + 'SM_Tanker'] = split_vehicle(B + 'SM_Tanker', tesla_truck, (-625, 0, 0))
    write_wheels_json(d)
    mm = ft_art.Model('top'); tesla_top(mm); save(mm, B + 'SM_TeslaTop')
    mm = ft_art.Model('arc'); tesla_arc(mm); save(mm, B + 'SM_TeslaArc')
    return [B + 'SM_Tanker', B + 'SM_TeslaTop', B + 'SM_TeslaArc', 'top at %s' % (TESLA_TOP,)]


# =============================================================================================== CLOWN RIG
# prop hubs in the clown car's model space (origin = middle of its rear) -> SM_ClownCar local = x - 625
PROP_HUBS_MODEL = [(150, -400, 620), (150, 400, 620), (1090, -400, 620), (1090, 400, 620)]
PROP_HUBS = [(x - 625.0, y, z) for (x, y, z) in PROP_HUBS_MODEL]
CL_RED, CL_WHITE, CL_YEL, CL_BLUE, CL_MAG = '#ff2d3c', '#fff6ea', '#ffe14d', '#2e6bff', '#ff4fb8'


def striped_rod(m, a, b, r, cols, n, steps=8):
    for k in range(n):
        t0, t1 = k / float(n), (k + 1) / float(n)
        p0 = tuple(a[i] + (b[i] - a[i]) * t0 for i in range(3))
        p1 = tuple(a[i] + (b[i] - a[i]) * t1 for i in range(3))
        rod(m, cols[k % len(cols)], p0, p1, r, steps=steps)


def clown_rig(m):
    """MR. CHUCKLES' flying rig, in the clown car's model space (exported with the car's offset)."""
    L = L_BOSS
    BY = 330                      # the side beams
    BZ = 500
    for s in (-1, 1):
        # a candy-striped side beam from the rear pod to the front pod, and its struts down to the car's shoulders
        striped_rod(m, (150, s * BY, BZ), (1090, s * BY, BZ), 14, (CL_RED, CL_WHITE), 12)
        for x in (330, 880):
            striped_rod(m, (x, s * 150, 384), (x, s * BY, BZ), 10, (CL_YEL, CL_BLUE), 4)
            m.box(BLACK, (x, s * 150, 380), (50, 50, 16))                                     # bolted-on clamp
            for dx in (-16, 16):
                m.sphere(CHROME, (x + dx, s * 150, 389), 4, steps=(3, 5))
        # outriggers out to the motor pods
        for x, xp in ((150, 150), (1090, 1090)):
            striped_rod(m, (x, s * BY, BZ), (xp, s * 400, 560), 12, (CL_RED, CL_WHITE), 3)
        # the bomb rack under the beam: a rail and three cartoon bombs with lit fuses
        m.box('#3a3846', (620, s * BY, BZ - 26), (520, 16, 12))
        for k, x in enumerate((440, 620, 800)):
            m.box(BLACK, (x, s * BY, BZ - 46), (8, 8, 30))
            bc = (x, s * BY, BZ - 100)
            m.sphere(BLACK, bc, 38, steps=(7, 10))
            m.sphere(WHITE, (bc[0] - 14, bc[1] - s * 16, bc[2] + 14), 7, steps=(3, 5))
            m.cyl('#3a3846', (bc[0], bc[1], bc[2] + 30), 11, 12, steps=8)                    # (hangs from its neck)
            rod(m, '#c9b48a', (bc[0] + 8, bc[1] - s * 30, bc[2] + 6), (bc[0] + 18, bc[1] - s * 48, bc[2] + 26), 2.4, steps=5)
            m.sphere(YEL, (bc[0] + 19, bc[1] - s * 50, bc[2] + 28), 6, steps=(3, 5), glow=3.0)
            m.sphere('#ff8a2a', (bc[0] + 19, bc[1] - s * 50, bc[2] + 28), 10, steps=(3, 5), glow=1.4)
            m.cyl('#3a3846', (bc[0], bc[1] - s * 30, bc[2]), 9, 8, rot=(0, 0, -90 * s), steps=8)   # fuse socket
    # the motor pods: fat striped barrels with a chrome hub on top, a goofy face on the front
    for (x, y, z) in PROP_HUBS_MODEL:
        m.cyl(CL_RED, (x, y, z - 90), 42, 60, steps=14)
        m.cyl(CL_WHITE, (x, y, z - 70), 43, 16, steps=14)
        m.cyl(CL_YEL, (x, y, z - 30), 34, 14, steps=14)
        m.cyl(CHROME, (x, y, z - 16), 12, 16, steps=10)
        m.sphere(CL_RED, (x, y, z - 92), 42, scale=(1, 1, 0.5), steps=(5, 12))
        fx = 1 if x > 600 else -1
        for dy in (-14, 14):
            m.sphere(WHITE, (x + fx * 38, y + dy, z - 50), 9, steps=(4, 6))
            m.sphere(BLACK, (x + fx * 44, y + dy, z - 50), 4, steps=(3, 5))
        m.box(BLACK, (x + fx * 41, y, z - 74), (4, 26, 4), rot=(0, 0, 0))
        m.box(RED, (x, y + (-30 if y < 0 else 30), z - 40), (8, 4, 8), glow=1.6)
    # the striped mast behind the head, a pennant and a beacon
    striped_rod(m, (380, 0, 420), (380, 0, 1000), 12, (CL_RED, CL_WHITE), 10)
    m.sphere(CL_YEL, (380, 0, 1006), 16, steps=(4, 8))
    m.side(CL_MAG, [(370, 990), (370, 930), (250, 960)], -2, 2)
    m.sphere(CL_RED, (380, 0, 1028), 10, steps=(4, 6), glow=2.0)
    for s in (-1, 1):
        rod(m, '#e8e0f0', (380, 0, 980), (150, s * BY, BZ + 10), 1.6, steps=4)            # guy wires
        rod(m, '#e8e0f0', (380, 0, 980), (1090, s * BY, BZ + 10), 1.6, steps=4)


def clown_prop(m, R=170.0):
    m.cyl(CHROME, (0, 0, -8), 18, 16, steps=12)
    m.sphere(CL_RED, (0, 0, 8), 16, scale=(1, 1, 0.8), steps=(5, 10))
    m.sphere(CL_YEL, (0, 0, 20), 6, steps=(3, 6), glow=0.8)
    for k in range(3):
        a = k * 120.0
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        n = 5
        for j in range(n):
            r0 = 18 + (R - 18) * j / float(n)
            r1 = 18 + (R - 18) * (j + 1) / float(n)
            wdt = 46 - 14 * abs(j - 1.5) / 2.5
            m.box(CL_RED if j % 2 == 0 else CL_WHITE, (ca * (r0 + r1) / 2.0, sa * (r0 + r1) / 2.0, 0), (r1 - r0, wdt, 4), rot=(0, a, 12))
        m.sphere(CL_YEL, (ca * (R - 4), sa * (R - 4), 0), 14, scale=(1, 1.4, 0.4), steps=(4, 8))
    m.cyl('@propblur', (0, 0, -9), R + 6, 1, steps=40)


def export_clown():
    B = '/Game/Characters/Bosses/'
    mm = ft_art.Model('rig'); clown_rig(mm); save(mm, B + 'SM_ClownRig', (-625, 0, 0))
    mm = ft_art.Model('prop'); clown_prop(mm); save(mm, B + 'SM_ClownProp')
    return [B + 'SM_ClownRig', B + 'SM_ClownProp', PROP_HUBS]


# =============================================================================================== AUDIO
def import_audio(src=os.path.join(TESTS, 'audio')):
    tasks = []
    names = ('S_TeslaCharge', 'S_TeslaZap', 'S_PropBuzz', 'S_BombWhistle')
    for n in names:
        t = unreal.AssetImportTask()
        t.set_editor_property('filename', os.path.join(src, n + '.wav'))
        t.set_editor_property('destination_path', '/Game/Audio/SFX')
        t.set_editor_property('destination_name', n)
        t.set_editor_property('automated', True)
        t.set_editor_property('replace_existing', True)
        t.set_editor_property('save', True)
        tasks.append(t)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    out = []
    for n in names:
        p = '/Game/Audio/SFX/' + n
        sw = AL.load_asset(p)
        if n == 'S_PropBuzz':
            sw.set_editor_property('looping', True)
        AL.save_loaded_asset(sw)
        out.append((p, sw.get_editor_property('duration'), sw.get_editor_property('looping')))
    return out


# =============================================================================================== look shots
STAGE = (0.0, -150000.0, 0.0)
_FLOOR = '/Engine/BasicShapes/Plane'


def _spawn(path, loc, yaw=0.0, scale=1.0):
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    sm = unreal.load_asset(path)
    a = sub.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(*loc), unreal.Rotator(0, 0, yaw))
    a.static_mesh_component.set_static_mesh(sm)
    a.set_actor_scale3d(unreal.Vector(scale, scale, scale) if not isinstance(scale, tuple) else unreal.Vector(*scale))
    a.set_folder_path(unreal.Name('FT33Look'))
    return a


def look(name, items, dist, target=(0, 0, 150), elev=14.0, fov=40.0, w=1280, h=720, floor='#8f93a8', azim=90.0, exposure=1.6):
    """items: [(asset path, (x, y, z) relative to the stage, yaw, scale)]. azim 0 = the camera on -Y looking +Y; the
    default 90 puts it on +X looking -X: in L_Bayshore the faces that look +X are the lit ones. Saves SHOTS/<name>.png."""
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    made = []
    try:
        for it in items:
            p, loc = it[0], it[1]
            yaw = it[2] if len(it) > 2 else 0.0
            sc = it[3] if len(it) > 3 else 1.0
            made.append(_spawn(p, (STAGE[0] + loc[0], STAGE[1] + loc[1], STAGE[2] + loc[2]), yaw, sc))
        if floor:
            fa = _spawn(_FLOOR, STAGE, 0, (200, 200, 1))
            fa.static_mesh_component.set_material(0, ft_export.palette_mi(floor, 0.0, 'prop'))
            made.append(fa)
            wy = 6000 if math.cos(math.radians(azim)) > 0 else -6000
            wa = _spawn(_FLOOR, (STAGE[0] - math.sin(math.radians(azim)) * 6000, STAGE[1] + math.cos(math.radians(azim)) * 6000, STAGE[2] + 4000), 0, (200, 200, 1))
            wa.set_actor_rotation(unreal.Rotator(roll=90, pitch=0, yaw=azim), False)
            wa.static_mesh_component.set_material(0, ft_export.palette_mi('#b9bdd0', 0.0, 'prop'))
            made.append(wa)
        e, az = math.radians(elev), math.radians(azim)
        tx, ty, tz = STAGE[0] + target[0], STAGE[1] + target[1], STAGE[2] + target[2]
        cam = (tx + math.sin(az) * dist * math.cos(e), ty - math.cos(az) * dist * math.cos(e), tz + dist * math.sin(e))
        cap = sub.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(*cam))
        made.append(cap)
        cap.set_actor_rotation(unreal.Rotator(roll=0, pitch=-elev, yaw=90 + azim), False)
        cc = cap.get_editor_property('capture_component2d')
        rt = unreal.RenderingLibrary.create_render_target2d(world, w, h, unreal.TextureRenderTargetFormat.RTF_RGBA8_SRGB, unreal.LinearColor(0, 0, 0, 1))
        cc.set_editor_property('texture_target', rt)
        cc.set_editor_property('fov_angle', fov)
        cc.set_editor_property('capture_source', unreal.SceneCaptureSource.SCS_FINAL_TONE_CURVE_HDR)
        cc.set_editor_property('primitive_render_mode', unreal.SceneCapturePrimitiveRenderMode.PRM_USE_SHOW_ONLY_LIST)
        for a in made:
            if a is not cap:
                cc.show_only_actor_components(a, False)
        if exposure is not None:
            pp = cc.get_editor_property('post_process_settings')
            pp.set_editor_property('override_auto_exposure_bias', True)
            pp.set_editor_property('auto_exposure_bias', exposure)
            cc.set_editor_property('post_process_settings', pp)
        cc.set_editor_property('capture_every_frame', False)
        cc.set_editor_property('capture_on_movement', False)
        cc.capture_scene()
        cc.capture_scene()
        os.makedirs(SHOTS, exist_ok=True)
        unreal.RenderingLibrary.export_render_target(world, rt, SHOTS, name + '.png')
    finally:
        for a in made:
            sub.destroy_actor(a)
    return os.path.join(SHOTS, name + '.png')


def vehicle_items(path, at=(0, 0, 0), yaw=0.0, wheels=True, chrome=False, struts=True):
    """Look-shot items for a split vehicle: the body, and its wheels from wheels.json the way the game should place
    them (scaled to r and w, right wheels mirrored with a negative Y scale), with a strut from axle up into the body."""
    items = [(path, at, yaw)]
    if not wheels:
        return items
    W = '/Game/Characters/Wheels/'
    for (x, y, z, r, w) in load_wheels_json().get(path, []):
        p = xfp(at, (0, yaw, 0), (x, y, z))
        items.append((W + ('SM_FoeWheelChrome' if chrome else 'SM_FoeWheel'), p, yaw, (r / 50.0, (w / 40.0) * (-1 if y > 0 else 1), r / 50.0)))
        if struts:
            q = xfp(at, (0, yaw, 0), (x, y - (1 if y > 0 else -1) * (w / 2.0 + 4), z))
            items.append((W + 'SM_FoeStrut', q, yaw, (1.0, 1.0, r * 1.1 / 100.0)))
    return items
