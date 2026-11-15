"""Tactical rappellers, authored in centimeters around their existing deployment and door grips."""

import math

import ft_art
from ft_car import BLACK, CHROME, STEEL
from ft_desert import rod, tilt_box
from ft_v33 import block_text, cone, save, swat_kit, torus, with_base, xfp


RAPPEL_GRIP = (0, 0, 203)
BRAKE_GRIP = (-7, -59, 113)
ROPE = '#e8dcb0'
UNDERSUIT = '#191920'
BADGE = '#f4f2e8'

# The first value controls a faction's helmet attachments. Shoulder sizes stay modest
# so the arms and gear have room in both the rappel and window-cling poses.
KITS = (
    ('dual optic', 1.00, 1.00),
    ('face shield', 1.12, 1.08),
    ('wrapped', 0.97, 0.90),
    ('face shield', 1.16, 1.12),
    ('dual optic', 0.96, 1.14),
    ('wrapped', 1.10, 1.00),
    ('radio', 1.08, 1.16),
    ('radio', 1.23, 1.15),
)


def between(a, b, t):
    return tuple(u + (v - u) * t for u, v in zip(a, b))


def shift(p, direction, amount):
    return tuple(u + v * amount for u, v in zip(p, direction))


def helmet(m, k, equipment, size):
    shell, panel, trim, optic = k['helm'], k['vest2'], k['web'], k['lens']
    m.sphere(UNDERSUIT, (0, 0, -2), 12, (0.93, 0.89, 1.16), (6, 12))
    m.sphere(shell, (-1, 0, 5), 14, (1.05, size, 0.92), (6, 12))
    # Beveled brow extrusion and separate rubber seal around the colored glazing.
    m.plan(shell, [(7, -13), (13, -11), (16, -6), (16, 6), (13, 11), (7, 13)], 4, 8)
    for sign in (-1, 1):
        m.box(BLACK, (10.5, sign * 5.7, 1.5), (5.9, 10.4, 7.4), rot=(0, sign * 17, 0))
        m.box(optic, (13.15, sign * 5.7, 1.5), (1.2, 8.4, 4.9), glow=0.38, rot=(0, sign * 17, 0))
        m.box(trim, (-1.8, sign * (size * 13.4), 3.5), (12.2, 1.9, 2.4))
        m.sphere(panel, (-1.5, sign * 10.5, -4), 4.1, (1, 0.5, 1.3), (4, 8))
        tilt_box(m, BLACK, (-1, sign * 10.9, -5), (5, sign * 7, -13), 2.0, 1.8)
    m.side(panel, [(11, -3.5), (14.6, -5.7), (12.5, -12), (5.2, -15), (2.7, -9.2)], -7.7, 7.7)
    for y in (-4.2, 0, 4.2):
        m.box(BLACK, (13.5, y, -8), (1.8, 1.5, 3.5), rot=(-16, 0, 0))
    m.cyl(UNDERSUIT, (-1, 0, -22), 7.2, 11, steps=8)

    if equipment == 'dual optic':
        m.box(BLACK, (8, 0, 12), (8, 18, 4.5))
        for sign in (-1, 1):
            rod(m, panel, (5, sign * 7.5, 13), (15.4, sign * 7.5, 13), 3.1)
            rod(m, BLACK, (13.1, sign * 7.5, 13), (16.1, sign * 7.5, 13), 3.45)
            rod(m, optic, (16, sign * 7.5, 13), (16.5, sign * 7.5, 13), 2.15, glow=0.38)
    elif equipment == 'face shield':
        m.plan(panel, [(5, -14), (12, -14), (17, -10), (17, 10), (12, 14), (5, 14)], 6, 9)
        for sign in (-1, 1):
            m.side(panel, [(7.5, 7), (14.5, 5), (15, -5.5), (11, -10), (7, -6)],
                   min(sign * 14, sign * 9.8), max(sign * 14, sign * 9.8))
    elif equipment == 'wrapped':
        m.sphere(k['fat'], (-5, 0, -4), 11.4, (0.91, 1.12, 0.77), (5, 10))
        for sign in (-1, 1):
            tilt_box(m, panel, (-8, sign * 8.2, -10), (4, sign * 10.7, -6.8), 4.1, 2.9)
    else:
        m.box(panel, (-7.5, 13.3, 1.8), (8.2, 5.2, 8))
        m.box(trim, (-7.5, 16, 1.7), (4, 0.8, 4))
        rod(m, BLACK, (-9, 13.2, 5.5), (-14.5, 14.1, 24.5), 0.9, steps=6)


def torso(m, k, shoulder_size):
    cloth, armor, plate, trim = k['fat'], k['vest'], k['vest2'], k['web']
    m.sphere(cloth, (0, 0, 22), 26, (0.61, 0.84, 1.10), (6, 12))
    m.sphere(cloth, (-2, 0, -4), 19, (0.74, 0.98, 0.62), (5, 10))
    m.side(armor, [(-12, 10), (11, 9), (16, 16), (17, 40), (11, 49), (-11, 49),
                   (-17, 41), (-16, 18)], -19, 19)
    m.sphere(UNDERSUIT, (0, 0, 48.6), 11, (1.09, 1.22, 0.36), (4, 10))
    for sign in (-1, 1):
        shoulder_y = sign * 21
        m.sphere(cloth, (-1, shoulder_y, 43), 8, (1.14, 1, 1.03), (5, 10))
        m.sphere(armor, (-1, shoulder_y, 45), 10.3 * shoulder_size, (1.14, 0.84, 0.65), (5, 10))
        m.box(trim, (-1, shoulder_y, 50.6 * min(shoulder_size, 1.05)), (9, 5.6, 1.7), rot=(0, 0, sign * 19))
        lo, hi = sorted((sign * 4, sign * 16.6))
        m.side(plate, [(15, 26), (19, 30), (20, 40), (16, 44), (13, 42), (13, 28)], lo, hi)
        m.side(plate, [(-11, 15), (8, 15), (9, 19), (-10, 20)], shoulder_y - 1.4, shoulder_y + 1.4)
        tilt_box(m, trim, (16, sign * 9, 4), (20, sign * 14, 42), 3.0, 1.8)
        tilt_box(m, trim, (-16, sign * 8, 5), (-22, sign * 15, 43), 3.2, 1.8)
        tilt_box(m, trim, (12.2, sign * 5, -1), (7.4, sign * 16, -11), 3.2, 2)
    m.box(BLACK, (20.1, 0, 35), (1.9, 24, 8.1))
    block_text(m, BADGE, 'SWAT', (21.2, 0, 35), 1.16, 'x', 1, depth=0.55)
    for y in (-8, 0, 8):
        m.box(plate, (18.2, y, 17), (6, 6.8, 11), rot=(-5, 0, 0))
        m.box(BLACK, (20.8, y, 21), (1.6, 5.6, 2))
    m.side(plate, [(-20, 15), (-24, 21), (-24, 37), (-20, 43), (-14, 43), (-14, 17)], -14.8, 14.8)
    m.box(BLACK, (-24.2, 0, 33), (1.8, 28.8, 10))
    block_text(m, BADGE, 'SWAT', (-25.3, 0, 33), 1.48, 'x', -1, depth=0.7)
    m.box(plate, (1.8, -20.5, 35), (12, 6, 13))
    rod(m, BLACK, (0, -22, 40), (-3.6, -23, 59), 1, steps=6)
    m.box(BLACK, (0, 0, 5), (29, 38, 7.8))
    m.box(trim, (0, 0, 0.7), (29.8, 38.7, 3.1))
    m.box(STEEL, (15.2, 0, 5), (2.3, 8, 4.4))
    # Carabiner stands proud of the harness, aligned with the live branch of the line.
    torus(m, CHROME, (16.1, 0, -3.0), 4.25, 1.15, rot=(0, 0, 90), major=12, minor=5)
    m.sphere(STEEL, (17.2, 0, -9), 5.1, (0.62, 0.7, 1.07), (4, 8))
    rod(m, trim, (16.4, -3.5, -7.2), (16.4, 3.5, -7.2), 1.3, steps=6)


def limbs(m, k, orientation, joints):
    cloth, armor, plate, trim = k['fat'], k['vest'], k['vest2'], k['web']
    front = (1, 0, 0) if orientation == 'x' else (0, 1, 0)
    side = (0, 1, 0) if orientation == 'x' else (-1, 0, 0)
    for label, sign in (('r', 1), ('l', -1)):
        shoulder, elbow, hand = (joints[part + label] for part in ('sh_', 'el_', 'ha_'))
        hip, knee, ankle, toe = (joints[part + label] for part in ('hip_', 'kn_', 'an_', 'toe_'))
        cone(m, cloth, shoulder, elbow, 7.6, tip=5.3)
        m.sphere(cloth, elbow, 5.7, steps=(5, 8))
        m.sphere(plate, shift(elbow, side, sign * 4.1), 5.1, (0.92, 0.66, 1.12), (4, 8))
        wrist = between(elbow, hand, 0.82)
        cone(m, cloth, elbow, wrist, 6, tip=4.2)
        cone(m, plate, between(elbow, hand, 0.29), between(elbow, hand, 0.71), 6.12, tip=5.25)
        cone(m, trim, between(elbow, hand, 0.71), between(elbow, hand, 0.81), 5.32, tip=5.06)
        cone(m, UNDERSUIT, wrist, hand, 4.7, tip=5.5)
        palm = shift(hand, front, -1.8)
        m.sphere(UNDERSUIT, palm, 5.25, (1.05, 0.98, 1.2), (5, 8))
        for index in (-1, 0, 1):
            offset = index * 2.5
            knuckle = shift(shift(hand, front, 3.2), side, offset)
            m.sphere(plate, knuckle, 1.75, (0.8, 1, 1.15), (4, 6))
        thumb = shift(shift(hand, side, -sign * 4.3), front, 1.5)
        m.sphere(UNDERSUIT, thumb, 2.75, (0.85, 0.85, 1.18), (4, 6))

        cone(m, cloth, hip, knee, 9.45, tip=6.65)
        cuff_start, cuff_end = between(hip, knee, 0.15), between(hip, knee, 0.27)
        cone(m, trim, cuff_start, cuff_end, 9.45, tip=9.15)
        m.sphere(cloth, knee, 6.6, steps=(4, 8))
        weft = (toe[0] - ankle[0], toe[1] - ankle[1], 0)
        length = math.hypot(*weft[:2]) or 1
        boot_front = (weft[0] / length, weft[1] / length, 0)
        knee_pad = shift(knee, boot_front, 5.6)
        m.sphere(armor, knee_pad, 7.05, (0.89, 0.84, 1.18), (4, 8))
        m.sphere(plate, shift(knee_pad, boot_front, 2.6), 4.95, (0.80, 0.85, 1.1), (4, 8))
        boot_top = between(knee, ankle, 0.57)
        cone(m, cloth, knee, boot_top, 6.4, tip=5.8)
        cone(m, UNDERSUIT, boot_top, ankle, 7.2, tip=6.1)
        cuff = between(boot_top, ankle, 0.16)
        cone(m, plate, boot_top, cuff, 7.7, tip=7.35)
        m.sphere(UNDERSUIT, ankle, 6.7, (1, 0.92, 1), (4, 8))
        heel = shift(ankle, boot_front, -5.6)
        heel = (heel[0], heel[1], toe[2] + 1.8)
        tip = shift(toe, boot_front, 6.2)
        tilt_box(m, BLACK, shift(heel, (0, 0, 1), -3.9), shift(tip, (0, 0, 1), -3.6), 12, 3.8)
        tilt_box(m, UNDERSUIT, heel, toe, 11.0, 7.9)
        yaw = math.degrees(math.atan2(boot_front[1], boot_front[0]))
        m.plan(UNDERSUIT, [(-4.8, -5.6), (3, -5.4), (6.2, -3.8), (6.2, 3.8),
                           (3, 5.4), (-4.8, 5.6)], -1.9, 2.1, loc=toe, rot=(0, yaw, 0))
        m.plan(plate, [(-4.8, -4.7), (2.7, -4.6), (5.1, -3.2), (5.1, 3.2),
                      (2.7, 4.6), (-4.8, 4.7)], 2.05, 3.8, loc=toe, rot=(0, yaw, 0))
    thigh = between(joints['hip_r'], joints['kn_r'], 0.33)
    holster = shift(shift(thigh, side, 8.2), front, -2.1)
    m.sphere(BLACK, holster, 8.8, (0.66, 0.75, 1.20), (4, 8))
    m.box(plate, shift(holster, (0, 0, 1), 5.8), (6.6, 6.6, 6.3))


def trooper(m, level, orientation, torso_location, torso_rotation, head_location, head_rotation, joints):
    kit = swat_kit(level)
    equipment, helmet_size, shoulder_size = KITS[level]
    for label, sign in (('r', 1), ('l', -1)):
        joints['sh_' + label] = xfp(torso_location, torso_rotation, (0, sign * 21, 44))
        joints['hip_' + label] = xfp(torso_location, torso_rotation, (0, sign * 10, -5))
    with_base(m, torso_location, torso_rotation, lambda: torso(m, kit, shoulder_size))
    with_base(m, head_location, head_rotation, lambda: helmet(m, kit, equipment, helmet_size))
    limbs(m, kit, orientation, joints)


def rappel(m, level):
    torso_location, torso_rotation = (-17, 0, 98), (6, 0, 0)
    joints = dict(ha_r=RAPPEL_GRIP, el_r=(-11, 20, 174), ha_l=BRAKE_GRIP, el_l=(-17, -41, 133),
                  kn_r=(6, 16, 53), an_r=(-15, 16, 11), toe_r=(3, 17, 5),
                  kn_l=(9, -15, 63), an_l=(-8, -19, 17), toe_l=(8, -22, 11))
    trooper(m, level, 'x', torso_location, torso_rotation, (-20, 0, 166), (9, -9, 2), joints)
    harness = xfp(torso_location, torso_rotation, (17.2, 0, -9))
    tail = (-16, -69, 24)
    path = (RAPPEL_GRIP, harness, BRAKE_GRIP, (-13, -67, 48), tail)
    for start, end in zip(path, path[1:]):
        rod(m, ROPE, start, end, 2.1, steps=8)
    rod(m, STEEL, (-16, -69, 28), tail, 2.15, steps=8)


def window_cling(m, level):
    joints = dict(ha_r=(-13, 0, 0), el_r=(-29, -20, -20), ha_l=(13, 0, 0), el_l=(29, -20, -20),
                  kn_r=(-18, -68, -118), an_r=(-16, -32, -146), toe_r=(-16, -19, -149),
                  kn_l=(16, -58, -138), an_l=(16, -30, -168), toe_l=(16, -17, -172))
    trooper(m, level, 'y', (0, -31.9, -92.4), (-15, 90, 0), (0, -16, -31), (24, 90, 0), joints)


def export(levels=range(8)):
    paths = []
    for level in levels:
        folder = f'/Game/Characters/Themed/L{level}' if level else '/Game/Characters/Syndicate'
        for name, build in (('SM_Henchman', rappel), ('SM_HenchmanHang', window_cling)):
            model = ft_art.Model(name)
            build(model, level)
            path = folder + '/' + name
            save(model, path, collision='box', kind='prop')
            paths.append(path)
    return paths
