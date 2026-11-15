"""Racing enemy mesh pass for the existing Full Throttle packages.

Run rebuild() in the Unreal editor's Python environment. Coordinates follow
the original builders so wheel, rotor, weapon and crew pivots are preserved.
"""

import unreal

import ft_art
import ft_car
import ft_v25
import ft_v28
import ft_v33
import ft_visual_art
from ft_desert import rod


ROOT = '/Game/Characters/'
AL = unreal.EditorAssetLibrary
DARK = '#1c1a26'
CAST = '#353848'
METAL = '#8f9bb3'
LIGHT = '#d7e3f0'
RED = '#ff3355'
U = 9.14

CLASSIC = dict(rammer='#7b3bd6', rammer_acc='#ffd23f', suv='#23202c',
               suv_acc='#ff4fb8', truck='#5f7a45', truck_band='#94ae66',
               stripe='#ff4fb8')


def flank(m, color, profile, y, depth, side, glow=0.0):
    """Extrude a panel from the side of the source shell."""
    lo, hi = (y, y + depth) if side > 0 else (-y - depth, -y)
    m.side(color, profile, lo, hi, glow=glow)


def bumper(m, width, setback, z, height, inset):
    """Chamfer the exposed corners of a welded bumper, leaving its mounts inside the hull."""
    m.plan(DARK, [(-setback, -width + inset), (-setback, width - inset),
                  (-setback * .48, width), (inset, width),
                  (inset, -width), (-setback * .48, -width)], z, z + height)
    m.box(CAST, (-setback + 1, 0, z + height * .62),
          (3, (width - inset) * 1.7, height * .38))


def mount(m, x, y, z, width, height, tone):
    """A service bracket stamped into the rear face."""
    m.box(CAST, (x, y, z), (12, width, height))
    m.box(tone, (x - 7, y, z), (3, width * .66, height * .66))
    for s in (-1, 1):
        rod(m, METAL, (x - 6, y + s * width * .4, z),
            (x - 9, y + s * width * .4, z), max(2, height * .094), steps=6)


def mission_panel(m, level, x, y, z, length, height, color, signal, s):
    """Fit crew-specific hardware to a flat flank without changing the weapon silhouette."""
    x0, x1 = x - length / 2, x + length / 2
    z0, z1 = z - height / 2, z + height / 2
    cut = min(height / 3, length / 9)
    flank(m, DARK, [(x0, z0 + cut), (x0 + cut, z0), (x1 - cut, z0),
                    (x1, z0 + cut), (x1, z1 - cut), (x1 - cut, z1),
                    (x0 + cut, z1), (x0, z1 - cut)], y, 4, s)
    face = y + 4
    if level in (1, 5):
        # Locking stations for chemical service and cold-weather recovery equipment.
        for p in (x - length * .24, x + length * .24):
            m.box(color, (p, s * (face + 2), z), (length * .32, 4, height * .65))
            m.box(signal, (p, s * (face + 4.5), z + height * .14),
                  (length * .14, 1, height * .14), glow=.3)
    elif level in (2, 3, 7):
        # Sand screens, ceramic heat sinks and the metro armor share a recessed frame.
        for i in (-1, 0, 1):
            p = x + i * length * .21
            flank(m, color, [(p - cut, z0 + cut * .5), (p, z0 + cut * .5),
                             (p + cut, z1 - cut * .5), (p, z1 - cut * .5)], face, 3, s)
        m.box(signal, (x - length * .35, s * (face + 1), z),
              (length * .074, 2, height * .42), glow=.32)
    elif level in (4, 6):
        # Carnival marker lamps or watertight pressure gauges.
        for p in (x - length * .22, x + length * .22):
            r = height * .31
            rod(m, color, (p, s * face, z), (p, s * (face + 4), z), r, steps=10)
            rod(m, signal, (p, s * (face + 4), z), (p, s * (face + 6), z),
                r * .62, steps=10, glow=.62)
    else:
        m.box(color, (x, s * (face + 2), z), (length * .78, 4, height * .52))
        m.box(signal, (x - length * .2, s * (face + 4.5), z),
              (length * .26, 1, height * .15), glow=.44)


def rammer(m, level, theme):
    hw = 15 * U - 12
    shell, stripe = theme['rammer'], theme['rammer_acc']
    bumper(m, hw - 6, 18, 27, 14, 18)
    mount(m, -17, 0, 34, 46, 21, METAL)
    for s in (-1, 1):
        # The reinforcement rises from the rear pan into the rear deck.
        flank(m, shell, [(4, 59), (70, 76), (136, 80), (176, 73), (122, 65), (36, 58)],
              hw - 10, 18, s)
        flank(m, CAST, [(144, 35), (266, 35), (283, 57), (166, 62)], hw - 1, 8, s)
        mission_panel(m, level, 210, hw + 7, 48, 96, 19, stripe, theme['stripe'], s)
        rod(m, CAST, (428, s * (hw - 9), 37), (473, s * (hw - 9), 47), 10, steps=7)
        m.box(stripe, (-4.5, s * (hw - 26), 59), (4, 40, 4))


def suv(m, level, theme):
    hw = 18 * U - 12
    bumper(m, hw - 4, 19, 24, 19, 18)
    for s in (-1, 1):
        y = s * (hw - 29)
        # Shoulder brackets frame the existing spare and brake lights.
        flank(m, CAST, [(-8, 62), (59, 62), (115, 101), (95, 111), (5, 104), (-8, 92)],
              hw - 3, 11, s)
        m.box(CAST, (-4, y, 87), (10, 54, 34))
        m.box(RED, (-10, y, 89), (4, 38, 9), glow=1.0)
        m.box(theme['suv_acc'], (-10, y, 101), (4, 46, 4))
        rod(m, METAL, (-5, s * 49, 107), (14, s * 64, 133), 5, steps=7)
        rod(m, CAST, (-4, s * 49, 65), (21, s * 73, 85), 6, steps=7)
        mission_panel(m, level, 251, hw - .5, 90, 82, 26,
                      theme['suv_acc'], theme['stripe'], s)
        m.box(CAST, (262, s * (hw - 13), 34), (175, 29, 15))


def truck(m, level, theme):
    hw = 23 * U - 16
    bumper(m, hw - 5, 22, 19, 21, 24)
    for s in (-1, 1):
        y = s * (hw - 29)
        # Uprights attach to the cargo box. The middle stays clear for the existing vault.
        m.box(CAST, (-2, y, 149), (12, 38, 161))
        m.box(theme['truck_band'], (-9, y, 205), (3, 24, 23))
        mount(m, -8, y, 171, 25, 20, METAL)
        mount(m, -8, y, 103, 25, 20, METAL)
        flank(m, theme['truck'], [(12, 186), (56, 229), (546, 229), (619, 209),
                                   (588, 192), (117, 192), (86, 172), (12, 172)],
              hw - .5, 6, s)
        mission_panel(m, level, 195, hw + 5.5, 208, 132, 35,
                      theme['truck_band'], theme['stripe'], s)
        for x in (33, 316, 535):
            rod(m, METAL, (x, s * (hw + 5), 210), (x, s * (hw + 7), 210), 4, steps=6)


ROAD_ART = {'SM_Rammer': rammer, 'SM_Suv': suv, 'SM_Truck': truck}


def helicopter(m, level, palette):
    """The rear shell and mechanical davit brackets keep both door openings clear."""
    stripe, signal = palette['stripe'], palette['glow']
    for s in (-1, 1):
        flank(m, palette['body2'], [(-172, 74), (-147, 63), (-66, 63),
                                    (-66, 82), (-151, 94)], 89, 11, s)
        # Shell skin runs behind the slid-back door where it cannot intersect riders.
        mission_panel(m, level, -114, 96, 108, 87, 24, stripe, signal, s)
        flank(m, CAST, [(84, 112), (98, 99), (142, 98), (149, 112),
                        (133, 125), (90, 125)], 89.5, 9, s)
        m.box(METAL, (110, s * 98.5, 107), (38, 3, 4))
        # Cheeks above the original hook sleeve, on the existing pulley shaft at X=8.
        m.box(CAST, (8, s * 162, 231), (24, 28, 11))
        for x in (-2, 18):
            m.box(CAST, (x, s * 162, 222), (4, 27, 13))
            m.box(stripe, (x, s * 176.1, 229), (4, 2, 8))
        rod(m, METAL, (-3, s * 162, 218), (19, s * 162, 218), 4.2, steps=8)
        # Rear-facing engine shroud and hydraulic rail, leaving the bleed outlets open.
        rod(m, CAST, (-128, s * 40, 238), (-162, s * 58, 244), 16, steps=10)
        m.box(signal, (-175, s * 73, 175), (5, 17, 12), glow=.5)
        m.box(CAST, (-171, s * 73, 175), (9, 24, 19))
        rod(m, METAL, (-166, s * 32, 172), (-236, s * 18, 160), 5.5, steps=7)
        flank(m, palette['body2'], [(-631, 204), (-596, 216), (-632, 270), (-649, 264)],
              4.5, 2, s)


def juggernaut(m):
    hw = 32 * U - 16
    bumper(m, hw - 12, 33, 19, 30, 28)
    for s in (-1, 1):
        y = s * 180
        flank(m, CAST, [(4, 173), (58, 173), (189, 248), (232, 295),
                        (68, 298), (4, 252)], hw - 3, 17, s)
        m.box(CAST, (0, y, 172), (22, 130, 183))
        m.box('#3f3c4c', (-13, y, 172), (8, 103, 141))
        for i in range(3):
            m.box(DARK, (-18, y, 133 + i * 32), (4, 85, 14))
            m.box(METAL, (-19, y, 139 + i * 32), (5, 69, 5))
        rod(m, CAST, (318, s * 72, 306), (245, s * 72, 345), 20, steps=8)
        mount(m, -24, s * 65, 42, 54, 28, '#ffd23f')


def tanker(m):
    # Deck-level power housings, clear of the Tesla pivot and animated toroid.
    for s in (-1, 1):
        y = s * 186
        m.side('#2a3f8f', [(6, 138), (15, 228), (85, 244),
                           (150, 190), (150, 136)], y - 38, y + 38)
        m.box(ft_v33.INSUL, (19, y, 196), (9, 42, 40))
        for j in (-1, 0, 1):
            rod(m, ft_v33.COPPER, (28, y + j * 12, 188), (6, y + j * 12, 188), 3, steps=6)
        rod(m, DARK, (114, y, 190), (202, y, 142), 8, steps=8)
        rod(m, ft_v33.COPPER, (157, y, 158), (202, y, 142), 4, steps=7)
        mount(m, -10, s * 106, 63, 64, 30, ft_v33.COPPER)


def carrier(m):
    hw = 32 * U - 16
    for s in (-1, 1):
        # Side trusses sit outside the launch decks and overlap their existing uprights.
        flank(m, CAST, [(32, 109), (50, 109), (292, 285), (292, 301),
                        (266, 301), (32, 125)], hw - 18, 13, s)
        flank(m, CAST, [(300, 286), (327, 288), (604, 124), (604, 108),
                        (576, 109), (300, 268)], hw - 18, 13, s)
        m.box('#ffd23f', (40, s * (hw - 10), 215), (25, 26, 146))
        m.box(RED, (25, s * (hw - 10), 231), (8, 17, 27), glow=.68)
        rod(m, METAL, (-184, s * (hw * .72), 49), (33, s * (hw * .72), 111), 6, steps=7)
        mount(m, 2, s * (hw - 39), 78, 51, 31, LIGHT)


def fire_engine(m):
    for s in (-1, 1):
        y = s * 135
        # Pump controls are mounted below the cannon and the original ladder.
        m.box(CAST, (1, y, 207), (14, 158, 112))
        m.box('#b81c30', (-8, y, 207), (6, 138, 88))
        for t in (-37, 37):
            rod(m, METAL, (-11, y + t, 218), (-24, y + t, 218), 17, steps=10)
            rod(m, DARK, (-23, y + t, 218), (-27, y + t, 218), 11, steps=8)
        rod(m, LIGHT, (-11, y - 44, 179), (-11, y + 44, 179), 4, steps=6)
        rod(m, '#ffd23f', (-8, s * 237, 174), (-8, s * 237, 306), 7, steps=7)
        m.box(CAST, (5, s * 237, 246), (36, 22, 22))
        m.box(LIGHT, (-13, s * 229, 311), (7, 36, 11))
    bumper(m, 255, 24, 83, 24, 23)


def clown(m):
    # Backlit fairground scallops stay low on the body, away from the moving props.
    for s in (-1, 1):
        for i, color in enumerate(('#ff2d3c', '#2ee6ff', '#ff4fb8')):
            y = s * (32 + i * 67)
            rod(m, '#fff6ea', (35, y, 195), (23, y, 195), 32, steps=10)
            rod(m, color, (23, y, 195), (17, y, 195), 23, steps=10, glow=.65)
        flank(m, '#ff2d3c', [(32, 149), (121, 149), (174, 221), (96, 222), (32, 197)],
              32 * U - 22, 9, s)
    # Raked circus trim wraps the factory shell rather than intersecting the clown rig.
    bumper(m, 244, 12, 126, 23, 24)
    m.box('#ff2d3c', (17, 0, 138), (31, 370, 16))


def yeti(m):
    """Sculpt tiered ice on the snowbound hide while keeping its face and arm sweep free."""
    tones = ('#b8cada', '#d7e3f0', '#eef6ff')
    for s in (-1, 1):
        for x, y, z in ((-127, 277, 633), (-176, 218, 704), (-148, 228, 766)):
            a = (x + 45, s * (y - 36), z + 36)
            b = (x - 58, s * (y + 58), z + 93)
            ft_v33.cone(m, tones[int(z / 60) % 3], a, b, 35, steps=5, tip=2)
        for j in range(3):
            x, z = (-162, 53) if s < 0 else (118, 103)
            ft_v33.cone(m, tones[j], (x, s * (130 + 32 * j), z),
                        (x - 38, s * (131 + 34 * j), z + 43), 22, steps=5, tip=0)


def yeti_arms(m):
    for s in (-1, 1):
        # The hand overlaps the moving source arm at the same wrist and palm positions.
        for i in range(3):
            y = s * (378 + i * 41)
            ft_v33.cone(m, '#d7e3f0', (-49, y, 651), (-167, y, 586 + i * 11),
                        23, steps=5, tip=0)


def shark(m):
    # Structural skin added near the launcher mount and the outboard fin root.
    for s in (-1, 1):
        flank(m, '#5a6a84', [(195, 197), (331, 160), (453, 175), (523, 241),
                             (385, 228), (276, 222)], 191, 11, s)
        for k in range(3):
            x = 324 + k * 39
            flank(m, '#e8eef8', [(x, 183), (x + 12, 186), (x + 38, 214), (x + 26, 219)],
                  202, 3, s)
        rod(m, CAST, (135, s * 120, 400), (175, s * 120, 400), 49, steps=10)
        rod(m, METAL, (155, s * 138, 367), (206, s * 178, 323), 11, steps=8)
        rod(m, CAST, (-30, s * 120, 120), (89, s * 39, 188), 16, steps=8)
        m.box(RED, (-33, s * 120, 120), (6, 43, 9), glow=.75)
    for i in range(3):
        z = 212 + 53 * i
        flank(m, '#e8eef8', [(-20, z), (26, z - 12), (23, z + 9), (-16, z + 19)], 12, 3, 1)
        flank(m, '#e8eef8', [(-20, z), (26, z - 12), (23, z + 9), (-16, z + 19)], 12, 3, -1)


def limo(m):
    half_width = 32 * U * .92 - 10
    bumper(m, half_width - 7, 23, 84, 22, 22)
    for s in (-1, 1):
        # Recessed armored fascia and clean gold detail on the executive body.
        m.box('#24202e', (-2, s * 143, 142), (13, 193, 57))
        m.box('#c9a04a', (-9, s * 134, 164), (4, 172, 5))
        m.box(RED, (-10, s * 166, 133), (4, 107, 9), glow=.86)
        for x in (187.5, 1025):
            y = s * (half_width + 40)
            rod(m, CAST, (x - 1, y, 60), (x - 26, y, 60), 71, steps=12)
            rod(m, '#c9a04a', (x - 26, y, 60), (x - 31, y, 60), 61, steps=12)
            rod(m, DARK, (x - 31, y, 60), (x - 34, y, 60), 46, steps=12)
            rod(m, '#ff8a2a', (x - 34, y, 60), (x - 36, y, 60), 29, steps=12, glow=.72)
            rod(m, CAST, (x + 12, s * (half_width - 12), 97), (x + 12, y, 110), 16, steps=8)


BOSS_ART = {
    'SM_Juggernaut': juggernaut, 'SM_Tanker': tanker, 'SM_Carrier': carrier,
    'SM_FireTruck': fire_engine, 'SM_ClownCar': clown, 'SM_Yeti': yeti,
    'SM_YetiArms': yeti_arms, 'SM_SharkCar': shark, 'SM_Limo': limo,
}


def _jobs():
    source = ft_v33.split_jobs()
    for path, build, offset in source['fleet'] + source['themed'] + source['bosses']:
        if not path.startswith(ROOT + 'Traffic/'):
            yield path, build, offset, True
    for leaf, build, offset, split in (
        ('SM_Tanker', ft_v33.tesla_truck, (-625, 0, 0), True),
        ('SM_Yeti', ft_v28.yeti, (0, 0, 0), False),
        ('SM_YetiArms', ft_v28.yeti_arms, (0, 0, 0), False),
        ('SM_Limo', ft_v25.limo, (-625, 0, 0), False),
    ):
        yield ROOT + 'Bosses/' + leaf, build, offset, split
    for level in range(8):
        path = ROOT + ('Themed/L%d/SM_Heli' % level if level else 'Choppers/SM_Heli')
        palette = ft_v33.heli_palette(level)
        yield path, (lambda m, p=palette, l=level: ft_v33.heli33(m, p, l)), (0, 0, 0), False


def decorate(m, path):
    folder, leaf = path.rsplit('/', 1)
    if folder == ROOT + 'Bosses' and leaf in BOSS_ART:
        BOSS_ART[leaf](m)
    elif folder.startswith(ROOT + 'Themed/L') or folder in (ROOT + 'Choppers', ROOT + 'Syndicate'):
        level = int(folder.rsplit('/L', 1)[-1]) if '/Themed/' in folder else 0
        if leaf == 'SM_Heli':
            helicopter(m, level, ft_v33.heli_palette(level))
        elif leaf in ROAD_ART:
            ROAD_ART[leaf](m, level, ft_v28.THEMES.get(level, CLASSIC))


def _body(path, build, offset, split, wheel_table):
    m = ft_art.Model(path.rsplit('/', 1)[-1])
    if split:
        old_wheel_mode = ft_car.WHEEL_MODE
        ft_car.WHEEL_MODE = 'skip'
        try:
            build(m)
        finally:
            ft_car.WHEEL_MODE = old_wheel_mode
        wheels = getattr(m, 'wheels', [])
        recorded = [[round(x + offset[0], 2), round(y + offset[1], 2),
                     round(z + offset[2], 2), round(r, 2), round(w, 2)]
                    for x, y, z, r, w, _ in wheels]
        if path not in wheel_table or recorded != wheel_table[path]:
            raise RuntimeError('wheel layout differs from wheels.json: ' + path)
        ft_v33.underside(m, wheels)
    else:
        build(m)
    return m


def rebuild():
    """Update existing mesh packages in place and return the exact saved package paths."""
    jobs = list(_jobs())
    paths = {path for path, _, _, _ in jobs}
    for path in paths:
        if not AL.does_asset_exist(path) or not ft_visual_art.enabled(path):
            raise RuntimeError('expected existing upgradeable mesh: ' + path)

    wheel_table = ft_v33.load_wheels_json()
    saved = []
    for path, build, offset, split in jobs:
        body = _body(path, build, offset, split, wheel_table)
        mesh = ft_v33.save(body, path, offset)
        if mesh is None:
            raise RuntimeError('could not save vehicle mesh: ' + path)
        saved.append(path)
    return saved
