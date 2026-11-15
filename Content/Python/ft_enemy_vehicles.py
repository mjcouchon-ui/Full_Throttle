"""Racing enemy mesh pass for the existing Full Throttle packages.

Run rebuild() in the Unreal editor's Python environment. Coordinates follow
the original builders so wheel, rotor, weapon and crew pivots are preserved.
"""

import math

import unreal

import ft_art
import ft_car
import ft_v25
import ft_v28
import ft_v33
import ft_visual_art
from ft_desert import rod, tilt_box


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
    PURPLE, PINK, HAZ = '#7b3bd6', '#ff4fb8', '#ffd23f'
    bumper(m, hw - 12, 33, 19, 30, 28)
    for s in (-1, 1):
        y = s * 180
        flank(m, CAST, [(4, 173), (58, 173), (189, 248), (232, 295),
                        (68, 298), (4, 252)], hw - 3, 17, s)
        m.box(CAST, (0, y, 172), (22, 130, 183))
        # Overhaul: the two rear housings are the barrel bays. A tray carries each barrel out through the dark
        # doorway before it is dropped (SM_JuggTray and the game's own barrel: FTEntity.cpp, AnimJuggernaut).
        m.box(DARK, (-13, y, 150), (8, 104, 150))
        for k in range(5):
            m.box(HAZ if k % 2 == 0 else DARK, (-14, y - 56, 81 + k * 30), (10, 12, 30))
            m.box(DARK if k % 2 == 0 else HAZ, (-14, y + 56, 81 + k * 30), (10, 12, 30))
        m.box(HAZ, (-14, y, 232), (10, 124, 12))
        m.box(RED, (-16, y, 250), (6, 60, 14), glow=1.2)                 # the bay's lamp: lit as a barrel comes out
        mount(m, -24, s * 65, 42, 54, 28, HAZ)
        # the hull wears the Syndicate's purple: a band under the roof line and the plinths the roof shields stand on
        flank(m, PURPLE, [(20, 262), (960, 262), (960, 286), (20, 286)], hw + 1, 4, s)
        m.box(PURPLE, (40, s * 196, 310), (80, 150, 22))
    # the Syndicate's S on the tail door, and a toothed armour lip along the roof's rear edge
    ft_v33.block_text(m, PINK, 'S', (-5, 0, 186), 30, 'x', 1, depth=3, glow=0.5)
    for k in range(7):
        y = (k - 3) * 70
        m.side(CAST, [(0, 300), (46, 300), (0, 334)], y - 26, y + 26)
    # the turret ring in the roof (the turret, its guns and the roof shields are the game's)
    m.cyl(DARK, (425, 0, 296), 168, 12, steps=16)


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
        # Overhaul: a capacitor tower stands on each housing. The pair frames the coil from behind, and their charge
        # is lit with the rest of the truck as a strike builds (the body's Alert: FTEntity.cpp, AnimAlchemist).
        m.cyl(ft_v33.TNAVY, (72, y, 236), 50, 16, steps=12)
        m.cyl('#233a63', (72, y, 252), 40, 196, steps=12)
        m.cyl(ft_v33.VIOLET, (72, y, 262), 41.5, 150, steps=12, glow=0.45)
        for band in (300, 352, 404):
            m.cyl(ft_v33.TNAVY, (72, y, band), 44, 9, steps=12)
        m.cyl(ft_v33.COPPER, (72, y, 448), 45, 18, steps=12, glow=0.3)
        rod(m, LIGHT, (72, y, 466), (72, y, 512), 6, steps=6)
        m.sphere(LIGHT, (72, y, 526), 20, steps=(5, 8))
        m.sphere(ft_v33.ARC, (72, y, 526), 9, steps=(4, 6), glow=3.0)
        # its feed, slung across to the coil's primary
        rod(m, DARK, (72, s * 150, 440), (250, s * 150, 372), 8, steps=6)
        rod(m, DARK, (250, s * 150, 372), (360, s * 118, 330), 8, steps=6)


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
        # Overhaul: the carrier is a launcher. A light tree on each rear post counts every rider out (the lamps
        # themselves are the game's: SM_CarrierLampA / G): the lower three are the gate's, the upper three the lift's.
        m.box(DARK, (30, s * (hw - 10), 192), (12, 46, 136))
        m.box(DARK, (30, s * (hw - 10), 398), (12, 46, 136))
        m.box(CAST, (44, s * (hw - 10), 398), (26, 30, 150))
        # the air brakes' hinge posts on the top deck's flank (the flaps are SM_CarrierFlap)
        m.box(CAST, (336, s * (hw + 4), 400), (14, 14, 170))
    # The middle lane of each deck is the launch lane: its two bikes are the game's (carrier_body leaves them out).
    for k in range(4):
        x = 60 + k * 78
        m.plan('#ffd23f', [(x, -44), (x + 34, 0), (x, 44), (x + 16, 0)], 110.5, 112.5)
    # the top deck's lift: its well in the deck, and the ram that pushes it, standing in the lower deck's lane
    m.plan(DARK, [(392, -72), (668, -72), (668, 72), (392, 72)], 307.5, 309)
    for k in range(5):
        m.box('#ffd23f' if k % 2 == 0 else DARK, (530, 0, 128 + k * 34), (86, 86, 34))
    m.cyl(METAL, (530, 0, 290), 34, 18, steps=12)
    # the Dealer's dice ride a mast on the sleeper (SM_CarrierDie, tumbling): what he rolls is always snake eyes
    m.cyl(CAST, (1040, 0, 356), 16, 250, steps=8)
    m.box(CAST, (1040, 0, 600), (20, 300, 18))
    for s in (-1, 1):
        m.cyl(METAL, (1040, s * 120, 600), 7, 100, steps=6)
    # a wider brake light bar under the top deck: the brake check's own light
    m.box(RED, (-6, 0, 286), (4, hw * 1.7, 16), glow=1.0)


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
    # Overhaul: the fire cannon and the ladder are the game's (fire_cannon, fire_ladder). What they stand on is
    # here: the cannon's turntable on the tail of the roof, the ladder's cradle ahead of it, and the fuel tank
    # between them that feeds both (its sight glass is lit: it burns brighter with every wind-up, the body's Alert).
    m.cyl(CAST, (130, 0, 336), 104, 16, steps=14)
    for k in range(8):
        m.box('#ffd23f' if k % 2 == 0 else DARK, (130 + 96 * math.cos(k * 0.7854 + 0.39), 96 * math.sin(k * 0.7854 + 0.39), 344),
              (40, 8, 14), rot=(0, 45 * k + 22.5 + 90, 0))
    m.cyl('#b81c30', (330, 0, 340), 74, 180, rot=(-90, 0, 0), steps=12)
    for x in (350, 420, 490):
        m.cyl(LIGHT, (x, 0, 340), 77, 10, rot=(-90, 0, 0), steps=12)
    m.box('#ff8a2a', (420, 0, 418), (120, 20, 8), glow=0.9)
    m.box(CAST, (900, 0, 352), (70, 150, 36))


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
    # Overhaul: the head, the balloons and the pie hatch's lid are the game's (clown_head, clown_jaw, clown_balloons,
    # clown_lid, clown_pies). What they sit in is here: the neck's spring, the balloons' winch, the pie oven.
    L = ft_v25.L_BOSS
    for k in range(5):
        m.cyl(METAL if k % 2 == 0 else CAST, (L * 0.45, 0, 300 + k * 9), 96 - k * 5, 9, steps=14)
    m.cyl('#ff2d3c', (L * 0.15, 0, 384), 44, 40, steps=12)
    m.cyl(LIGHT, (L * 0.15, 0, 424), 16, 22, steps=8)
    # the pie oven on the tail: striped, open to the rear (the lid hangs over the opening)
    for k in range(6):
        m.box('#ff2d3c' if k % 2 == 0 else '#fff6ea', (76, -150 + (k + 0.5) * 50, 286), (116, 50, 112))
    m.box(DARK, (17, 0, 286), (6, 276, 92))
    m.box('#ffe14d', (76, 0, 346), (128, 312, 10))
    for s in (-1, 1):
        m.box('#ffe14d', (20, s * 150, 286), (16, 14, 122))


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
        # Overhaul: his eyes burn. Dim until the body's Alert lifts them: a charge, and all the time once he is berserk.
        m.sphere(RED, (-286, s * 46, 1040), 10, steps=(4, 6), glow=0.5)


def yeti_arms(m, sides=(-1, 1)):
    for s in sides:
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
    # (overhaul: the three pale bars that were here sat on the baked tail fin; they are on the moving tail now)


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
        # Overhaul: tail fins, gold edged, a red light down the back of each (they flare with every warning: the
        # body's Alert). The pair frames the boot's turret and gives the limo a shape from behind.
        m.side('#16141e', [(-6, 184), (150, 184), (60, 250), (-22, 318), (-22, 196)], s * (half_width - 16) - 9, s * (half_width - 16) + 9)
        m.side('#ffc93c', [(-22, 318), (60, 250), (64, 258), (-18, 328)], s * (half_width - 16) - 10.5, s * (half_width - 16) + 10.5)
        m.box(RED, (-24, s * (half_width - 16), 250), (5, 14, 96), glow=.9)
        # the rocket bays' frames in the rear wings (the bays are the game's: SM_LimoPod)
        m.box('#ffc93c', (75, s * 190, 186), (158, 128, 3))
        m.box(DARK, (75, s * 190, 187.5), (146, 116, 3))
    # a wing across the fins' tips: the gunship's line from behind. Gold along its trailing edge, and a row of red
    # lights under it (lit parts of the body: they flare with every wind-up, as the fins' own do)
    span = 2 * (half_width - 16) + 30
    m.box('#16141e', (2, 0, 324), (46, span, 12))
    m.box('#ffc93c', (-20, 0, 331), (6, span + 4, 4))
    for k in range(7):
        m.box(RED, (-22, (k - 3) * (span - 60) / 7.0, 319), (4, 44, 8), glow=.9)
    # the Chairman's crest on the boot: a gold crown (five points), and a gold spine along the roof
    for k in range(5):
        y = (k - 2) * 26
        m.side('#ffc93c', [(-12, 96), (-12, 126 + (16 if k % 2 == 0 else 0)), (-9, 126 + (16 if k % 2 == 0 else 0)), (-9, 96)], y - 11, y + 11, glow=.3)
    m.box('#ffc93c', (-10, 0, 92), (5, 132, 10), glow=.3)
    m.box('#ffc93c', (600, 0, 293), (640, 18, 6))
    m.cyl('#ffc93c', (625, 0, 290), 76, 5, steps=14)                      # the sunroof's rim (its turret comes up through it)
    m.cyl('#24202e', (625, 0, 292), 66, 5, steps=14)


BOSS_ART = {
    'SM_Juggernaut': juggernaut, 'SM_Tanker': tanker, 'SM_Carrier': carrier,
    'SM_FireTruck': fire_engine, 'SM_ClownCar': clown, 'SM_Yeti': yeti,
    'SM_YetiArms': yeti_arms, 'SM_SharkCar': shark, 'SM_Limo': limo,
}


# =============================================================================================== the bosses' rigs
# Overhaul (15 Nov 2026): every boss is its body plus moving parts, meshes of their own that AFTEntityActor attaches
# and poses from the fight's state (FTEntity.cpp, "the bosses' rigs"; the pivots there are these builders' origins).
def _tesla_top_broken(m):
    ft_v33.tesla_top(m, broken=True)


# ---- THE GREAT WHITE: the tail that swims, the dorsal fin (whole, then torn), the lower jaw
SHARK_D, SHARK_BELLY = '#5a6a84', '#e8eef8'


def shark_tail(m):
    """The tail (origin: its root on the body's centre line; it runs back along -X and sweeps from side to side)."""
    m.sphere(SHARK_D, (-10, 0, 0), 60, scale=(1.4, 0.6, 1.0), steps=(6, 9))                  # the root
    m.side(SHARK_D, [(-30, 55), (-95, 100), (-190, 270), (-140, 50), (-100, 0), (-130, -70), (-205, -200), (-110, -110), (-30, -55)], -20, 20)
    m.side('#7f8fa8', [(-95, 100), (-190, 270), (-158, 130)], -21.5, 21.5)                   # a lighter trailing edge
    m.side('#7f8fa8', [(-130, -70), (-205, -200), (-160, -96)], -21.5, 21.5)
    for k in range(3):                                                                       # the three pale bars the old tail wore
        m.box('#e8eef8', (-52 - k * 18, 0, -30 + k * 30), (10, 43, 14))


def shark_fin(m, torn=False):
    """The dorsal fin (origin: the middle of its root on the back). torn=True: what is left of it at damage 1."""
    if not torn:
        m.side(SHARK_D, [(-110, 0), (140, 0), (-70, 350), (-126, 300)], -18, 18)
        m.side('#7f8fa8', [(-110, 0), (-70, 350), (-126, 300), (-140, 90)], -19.5, 19.5)
    else:
        m.side(SHARK_D, [(-110, 0), (140, 0), (60, 120), (4, 96), (-20, 190), (-70, 150), (-96, 210), (-126, 120)], -18, 18)
        m.side('#ff3355', [(60, 120), (4, 96), (14, 108)], -19.5, 19.5, glow=0.6)             # the raw edge
        m.side('#ff3355', [(-20, 190), (-70, 150), (-56, 168)], -19.5, 19.5, glow=0.6)


def shark_jaw(m):
    """The lower jaw (origin: its hinge, across the head; it runs forward along +X). Pitched down it opens the mouth."""
    hw = ft_v25.W_BOSS / 2 - 20
    m.side(SHARK_BELLY, [(-40, -30), (150, -44), (196, -20), (196, 6), (-40, 6)], -hw * 0.66, hw * 0.66)
    m.box('#3a1a2a', (90, 0, 9), (190, hw * 1.22, 6))
    for k in range(9):
        y = -hw * 0.6 + k * hw * 0.15 + 20
        m.cyl('#fff6ea', (176, y, 6), 14, 50, steps=6, top_r=1)
    for s in (-1, 1):
        m.cyl('#fff6ea', (120, s * hw * 0.6, 6), 12, 40, steps=6, top_r=1)


# ---- THE CHAIRMAN: the gun turrets (the boot's, and the sunroof's that comes up in phase 1), the rocket bays
GOLD_, LIMO_D = '#ffc93c', '#16141e'


def limo_turret(m):
    """A gun turret (origin: the middle of its ring on the deck). Twin rotary guns run back along -X."""
    m.cyl(GOLD_, (0, 0, 0), 70, 12, steps=14)
    m.cyl('#24202e', (0, 0, 12), 58, 44, steps=12)
    m.box('#24202e', (-50, 0, 44), (30, 150, 64))                                            # the shield
    m.box(GOLD_, (-66, 0, 72), (4, 150, 6))
    m.box(GOLD_, (-66, 0, 18), (4, 150, 6))
    for s in (-1, 1):
        for k in range(3):
            a = math.radians(k * 120 + 30)
            rod(m, DARK, (-40, s * 30 + math.cos(a) * 10, 44 + math.sin(a) * 10), (-262, s * 30 + math.cos(a) * 10, 44 + math.sin(a) * 10), 7, steps=6)
        m.cyl(GOLD_, (-150, s * 30, 44), 21, 10, rot=(90, 0, 0), steps=10)
        m.cyl(GOLD_, (-252, s * 30, 44), 21, 10, rot=(90, 0, 0), steps=10)
        m.cyl('#ff8a2a', (-264, s * 30, 44), 15, 4, rot=(90, 0, 0), steps=8, glow=0.6)        # the hot ends
    m.sphere('#ff3355', (-20, 0, 62), 12, steps=(4, 6), glow=1.6)


def limo_pod(m):
    """A rocket bay's lid (origin: its hinge along the outer edge; the lid runs inboard along +Y for the left bay:
    the game mirrors the right one). Under it, six rocket noses in their tubes."""
    m.box(LIMO_D, (0, 60, 5), (150, 120, 10))
    m.box(GOLD_, (0, 60, 11), (130, 8, 4))
    m.box(GOLD_, (0, 116, 5), (150, 6, 12))
    for i in range(3):
        for j in range(2):
            x, y = -46 + i * 46, 32 + j * 52
            m.cyl('#3a3846', (x, y, -40), 19, 40, steps=8)
            m.cyl('#ff3355', (x, y, -8), 15, 26, steps=8, top_r=2, glow=0.7)


# ---- THE ABOMINABLE: his two arms, one mesh each with the shoulder at the origin, and the ice he wears
def _yeti_arm(m, s):
    ft_v28.yeti_arms(m, sides=(s,))
    yeti_arms(m, sides=(s,))


def yeti_ice(m):
    """A crust of ice on a shoulder (origin: the top of the shoulder hump). Left, right and (bigger) across the
    back of his neck: it is shot off him piece by piece."""
    tones = ('#b8cada', '#d7e3f0', '#eef6ff', '#9fd8ff')
    for k, (x, y, z, r, h) in enumerate(((0, 0, 0, 62, 250), (-50, 60, -10, 44, 170), (40, -66, -6, 48, 190), (60, 50, -12, 40, 150), (-40, -50, -14, 36, 130))):
        ft_v33.cone(m, tones[k % 4], (x, y, z - 30), (x * 1.9, y * 1.9, z + h), r, steps=5, tip=3, glow=0.12 if k == 0 else 0.0)
    m.sphere('#d7e3f0', (0, 0, -20), 96, scale=(1.1, 1.1, 0.5), steps=(5, 8))


# ---- ENGINE 13: the fire cannon on its turntable and the ladder with the second nozzle
def fire_cannon(m):
    """The fire cannon (origin: the middle of its turntable). It points back at the car (-X): the game turns it at
    the lane it is about to burn and tips it up to lob fireballs. Its shield carries the engine's number."""
    BRASS, RED_, HAZ = '#ffc93c', '#e8283c', '#ffd23f'
    m.cyl(METAL, (0, 0, 0), 92, 30, steps=14)
    for s in (-1, 1):
        m.side(CAST, [(-50, 30), (50, 30), (30, 124), (-30, 124)], s * 66 - 12, s * 66 + 12)        # the yoke
    m.cyl(BRASS, (120, 0, 96), 46, 250, rot=(90, 0, 0), steps=12)                                     # the barrel
    m.cyl(METAL, (40, 0, 96), 52, 24, rot=(90, 0, 0), steps=12)
    m.cyl(METAL, (-70, 0, 96), 52, 20, rot=(90, 0, 0), steps=12)
    m.cyl(BRASS, (-130, 0, 96), 46, 96, rot=(90, 0, 0), steps=12, top_r=86)                           # the bell mouth
    m.cyl('#ff6a1a', (-222, 0, 96), 78, 6, rot=(90, 0, 0), steps=12, glow=1.2)                        # its throat: the pilot light
    m.cyl('#ffe14d', (-226, 0, 96), 46, 5, rot=(90, 0, 0), steps=10, glow=2.2)
    # the splash shield with the number: 13
    m.box(RED_, (-64, 0, 196), (12, 300, 104))
    for k in range(6):
        m.box(HAZ if k % 2 == 0 else DARK, (-72, -125 + k * 50, 148), (6, 50, 12))
    N = '#fff6ea'
    m.box(N, (-72, -62, 204), (6, 16, 70))                                                            # 1
    m.box(N, (-72, -78, 228), (6, 22, 14))
    for z in (236, 204, 172):                                                                         # 3
        m.box(N, (-72, 46, z), (6, 62, 14))
    m.box(N, (-72, 70, 204), (6, 14, 70))
    # the feed pipe and a pressure gauge
    rod(m, CAST, (118, 40, 96), (118, 40, 20), 12, steps=6)
    m.cyl(LIGHT, (60, -60, 150), 26, 8, rot=(90, 0, 0), steps=10)
    m.cyl(RED_, (52, -60, 150), 5, 3, rot=(90, 0, 0), steps=6, glow=1.0)


def fire_ladder(m):
    """The ladder (origin: its hinge behind the cab; it runs BACK along -X, 800 cm, and is carried raised, so its tip
    hangs over the tail above the cannon). The tip holds the second nozzle, tipped down at the road: at five alarms
    that one burns a lane of its own."""
    BRASS = '#ffc93c'
    for s in (-1, 1):
        m.box(LIGHT, (-400, s * 62, 0), (800, 12, 20))
        m.box(LIGHT, (-400, s * 62, 46), (760, 8, 8))
        for k in range(8):
            tilt_box(m, LIGHT, (-20 - k * 96, s * 62, 6), (-68 - k * 96, s * 62, 44), 6, 6)
    for k in range(14):
        m.box(LIGHT, (-30 - k * 57, 0, 0), (8, 124, 8))
    m.box(CAST, (0, 0, 0), (60, 150, 40))                                                             # the hinge block
    m.box(CAST, (-790, 0, 4), (44, 150, 44))                                                          # the tip's cradle
    m.cyl(BRASS, (-790, 0, 0), 30, 90, rot=(135, 0, 0), steps=10, top_r=58)                           # the second nozzle: back and down
    m.cyl('#ff6a1a', (-852, 0, -62), 52, 5, rot=(135, 0, 0), steps=10, glow=1.2)
    m.cyl('#ffe14d', (-855, 0, -65), 30, 5, rot=(135, 0, 0), steps=8, glow=2.2)
    m.sphere('#ff2d3c', (-790, 0, 46), 20, steps=(4, 6), glow=1.6)                                    # a beacon on the tip


# ---- THE JUGGERNAUT: the hull without its 1.x turret, the new turret and its two rotary guns, the roof shields
def juggernaut_body(m):
    """The hull: ft_fleet.truck(boss=True) without its turret (the turret is a moving part now). The same wheels in
    the same order, as wheels.json has them."""
    W, L = 64 * U, 1250
    hull, cab = '#2e2b38', '#3f3c4c'
    hw = W / 2 - 16
    bh = 300
    m.box(hull, (L * 0.39, 0, bh / 2 + 18), (L * 0.78, hw * 2, bh - 36))
    m.side(cab, [(L * 0.8, 40), (L, 40), (L, 150), (L * 0.95, 230), (L * 0.8, 240)], -hw + 10, hw - 10)
    for k in range(4):
        m.box(DARK, (L * 0.39, 0, 60 + k * (bh - 70) / 3.0), (L * 0.79, hw * 2 + 3, 4))
    m.box(DARK, (-1, 0, bh / 2 + 18), (3, hw * 1.1, bh * 0.75))
    m.box(METAL, (-3, 0, bh / 2 + 18), (2, 10, bh * 0.75))
    for s in (-1, 1):
        m.box('#ff3355', (-3, s * (hw - 20), 50), (2, 26, 20), glow=1.0)
        for x in (0.1, 0.24, 0.62, 0.9):
            ft_car.wheel(m, L * x, 46, 46, hw - 40, W / 2.0, s)
    m.box(DARK, (0, 0, 22), (8, W - 20, 18))
    for k in range(10):
        m.box(ft_car.YEL if k % 2 else DARK, (-3, -hw + (k + 0.5) * hw * 0.2, 22), (2, hw * 0.2, 18))
    for s in (-1, 1):
        m.cyl(LIGHT, (L * 0.84, s * (hw - 10), 200), 12, 160, steps=12)


def jugg_turret(m):
    """The turret (origin: the middle of its ring, on the roof). It faces back at the car (-X); the two rotary guns
    (jugg_gun) sit in its cradles at (-176, +-64, 70)."""
    ARM, PURPLE, HAZ = '#4a4e58', '#7b3bd6', '#ffd23f'
    m.cyl(DARK, (0, 0, 0), 150, 20, steps=16)
    m.plan(ARM, [(-176, -128), (-176, 128), (110, 156), (176, 84), (176, -84), (110, -156)], 20, 118)
    m.plan(PURPLE, [(-150, -104), (-150, 104), (96, 132), (150, 70), (150, -70), (96, -132)], 118, 132)
    m.box(DARK, (-180, 0, 70), (12, 236, 84))                             # the mantlet
    for s in (-1, 1):
        m.cyl(METAL, (-168, s * 64, 70), 40, 30, rot=(90, 0, 0), steps=12)  # the guns' cradles
        m.box(RED, (-188, s * 118, 104), (6, 30, 12), rot=(0, 0, s * 18), glow=1.6)   # its eyes
        m.cyl('#5a5f70', (30, s * 156, 62), 46, 30, rot=(0, 0, 90 * s), steps=12)   # ammunition drums
        m.cyl(HAZ, (30, s * 186, 62), 22, 6, rot=(0, 0, 90 * s), steps=10)
    for k in range(6):
        m.box(HAZ if k % 2 == 0 else DARK, (-188, -75 + k * 30, 22), (6, 30, 14))        # a hazard sill under the guns
    m.cyl(DARK, (70, 0, 132), 46, 12, steps=10)                            # the hatch
    rod(m, METAL, (120, 96, 132), (156, 96, 330), 4, steps=5)              # the aerial
    m.sphere(RED, (156, 96, 336), 9, steps=(3, 5), glow=1.5)


def jugg_gun(m):
    """A six-barrel rotary gun (origin on its axis at the cradle; the barrels run back along -X). It spins up
    through the last 0.6 s before a fan of rounds: the faster, the nearer."""
    import math
    for k in range(6):
        a = math.radians(k * 60)
        y, z = math.cos(a) * 25, math.sin(a) * 25
        rod(m, DARK, (0, y, z), (-292, y, z), 9.5, steps=6)
    for x in (-40, -170, -280):
        m.cyl(METAL, (x, 0, 0), 39, 14, rot=(90, 0, 0), steps=12)
    m.box('#ffd23f', (-170, 0, 39), (14, 14, 6))                           # a mark on one clamp: the spin reads on it
    m.cyl('#ff8a2a', (-294, 0, 0), 31, 5, rot=(90, 0, 0), steps=12, glow=0.5)   # the hot end


def jugg_shield(m):
    """A roof shield (origin: its hinge along the bottom edge; it stands 170 cm). One each side of the turret: the
    pair swings up as he arrives and is shot away as he is worn down."""
    PURPLE, HAZ = '#7b3bd6', '#ffd23f'
    m.side(PURPLE, [(0, 0), (16, 0), (30, 110), (22, 170), (4, 170), (-4, 110)], -78, 78)
    for k in range(4):
        m.box(HAZ if k % 2 == 0 else DARK, (-6, -58.5 + k * 39, 30), (5, 39, 26))
    m.box(CAST, (2, 0, 164), (30, 164, 12))
    for s in (-1, 1):
        m.sphere(LIGHT, (-5, s * 56, 120), 7, steps=(3, 5))
        m.sphere(LIGHT, (-5, s * 56, 76), 7, steps=(3, 5))
    m.box('#ff4fb8', (-6, 0, 104), (4, 70, 12), glow=0.6)


def jugg_tray(m):
    """A barrel bay's tray (origin: the middle of its floor at the hull's tail face): it runs out 120 cm with a
    barrel standing on it, lets it go, and runs back in."""
    HAZ = '#ffd23f'
    m.box(CAST, (60, 0, -5), (150, 100, 10))
    for s in (-1, 1):
        m.box(METAL, (60, s * 50, 8), (150, 6, 22))
    for k in range(4):
        m.box(HAZ if k % 2 == 0 else DARK, (-14, -37.5 + k * 25, 4), (8, 25, 28))


# ---- MR. CHUCKLES: the head on its spring, the jaw, the bomb balloons, the pie hatch
def clown_head(m):
    """The clown's head (origin: the top of the neck's spring, 200 cm under the middle of the head). It faces the
    player (-X). The mouth is a dark cavity with a row of teeth; the jaw (clown_jaw) closes it."""
    WHITE_, BLUE, RED_, MAG_ = '#fff6ea', '#2e6bff', '#ff2d3c', '#ff4fb8'
    hz = 200.0
    m.sphere(WHITE_, (0, 0, hz), 200, steps=(8, 12))
    m.sphere(RED_, (-200, 0, hz + 6), 60, steps=(6, 8), glow=0.6)                     # the nose: it lights as he winds up
    for s in (-1, 1):
        m.sphere(BLUE, (10, s * 205, hz + 60), 104, steps=(6, 8))                     # hair
        m.sphere(BLUE, (40, s * 150, hz + 150), 74, steps=(5, 7))
        m.sphere(DARK, (-166, s * 74, hz + 74), 30, steps=(5, 7))                     # eyes
        m.sphere('#fff6ea', (-186, s * 68, hz + 84), 9, steps=(3, 5))
        tilt_box(m, DARK, (-176, s * 36, hz + 124), (-166, s * 112, hz + 152), 20, 14)   # brows: angry, down to the nose
        m.sphere(MAG_, (-150, s * 128, hz - 20), 34, scale=(0.5, 1, 1), steps=(4, 6))  # cheeks
    m.box('#3e0b1a', (-150, 0, hz - 96), (70, 210, 84))                                # the mouth's cavity
    for k in range(5):
        m.box('#fff6ea', (-184, (k - 2) * 38, hz - 66), (8, 30, 26))                   # top teeth
    m.cyl(DARK, (0, 0, hz + 186), 74, 96, steps=12)                                    # the hat
    m.cyl(DARK, (0, 0, hz + 186), 126, 12, steps=14)
    m.cyl(RED_, (0, 0, hz + 206), 76, 18, steps=12)
    m.sphere(MAG_, (0, 0, hz + 296), 24, steps=(4, 6), glow=0.8)
    m.cyl(METAL, (0, 0, -30), 70, 40, steps=12)                                        # the collar on the spring


def clown_jaw(m):
    """The jaw (origin: the head's origin; it slides down to open the mouth): the chin and a fat red lower lip."""
    hz = 200.0
    m.box('#fff6ea', (-160, 0, hz - 116), (64, 216, 64))
    m.box('#ff2d3c', (-196, 0, hz - 88), (14, 224, 22))
    for s in (-1, 1):
        m.box('#ff2d3c', (-192, s * 104, hz - 74), (12, 22, 34), rot=(0, 0, s * 20))


BALLOONS = (('#ff2d3c', 0), ('#2e6bff', 1), ('#3dff6a', 2), ('#ff4fb8', 0), ('#ffe14d', 1), ('#7b3bd6', 2), ('#ff8a2a', 0), ('#2ee6ff', 1))


def clown_balloons(m, keep=8):
    """The bomb balloons (origin: the winch on the car's tail where they are tied). keep = how many are left: 8,
    then 5 and 3 as he is shot up (SM_ClownBalloons, B, C). A bomb hangs under each, its fuse lit."""
    import math
    for k, (color, row) in enumerate(BALLOONS):
        if k >= keep:
            continue
        a = k * 2 * math.pi / len(BALLOONS)
        bx, by, bz = math.cos(a) * 150, math.sin(a) * 240, 900 + row * 125
        rod(m, '#e8e0f0', (0, 0, 20), (bx, by, bz - 96), 3.5, steps=4)
        m.sphere(color, (bx, by, bz), 100, scale=(1, 1, 1.2), steps=(6, 10), glow=0.25)
        m.cyl(color, (bx, by, bz - 128), 14, 18, steps=6)
        m.sphere(DARK, (bx, by, bz - 172), 40, steps=(5, 8))
        rod(m, '#c9b48a', (bx, by, bz - 140), (bx - 14, by, bz - 122), 3, steps=4)
        m.sphere('#ffe14d', (bx - 16, by, bz - 120), 9, steps=(3, 5), glow=3.0)


def clown_lid(m):
    """The pie oven's lid (origin: its hinge along the top of the opening; it hangs 100 cm down from it)."""
    for k in range(6):
        m.box('#ff2d3c' if k % 2 == 0 else '#fff6ea', (-4, -138 + (k + 0.5) * 46, -50), (8, 46, 100))
    m.box('#ffe14d', (-6, 0, -96), (10, 284, 10))
    m.cyl(METAL, (0, -146, 0), 6, 292, rot=(0, 0, 90), steps=6)


def clown_pies(m):
    """Seven pies racked in the oven, cream toward the player, the oven lit behind them in the foes' red: shown
    while a fan of pies winds up (origin: the middle of the opening)."""
    m.box('#ff3355', (6, 0, 0), (2, 268, 84), glow=1.2)
    for k in range(7):
        y = (k - 3) * 38
        m.cyl('#d9a05a', (8, y, 0), 19, 8, rot=(90, 0, 0), steps=8)
        m.cyl('#fff6ea', (0, y, 0), 15, 8, rot=(90, 0, 0), steps=8)


# ---- THE BIG DEALER: the carrier's launch gear (model space: x from the trailer's tail, as ft_desert.carrier)
def carrier_body(m):
    """The carrier as ft_desert.carrier builds it, without the two bikes of the middle lane: those are the game's
    now (one waits at the lower gate, one stands on the top deck's lift: FTEntity.cpp, AnimDealer)."""
    import ft_desert
    real, seen = ft_desert.hog, [0]

    def outer_lanes(mm, variant=1, rider=True, x0=0.0):
        seen[0] += 1
        if seen[0] not in (3, 4):          # the middle lane's lower bike, then its upper one
            real(mm, variant, rider, x0)

    ft_desert.hog = outer_lanes
    try:
        ft_desert.carrier(m)
    finally:
        ft_desert.hog = real


def carrier_lift(m):
    """The top deck's lift (origin: the middle of the plate's top face, where the bike stands). It rises 183 cm with
    a rider on it through the wind-up of a jump: that is where the jumper leaves from."""
    m.box('#4b4568', (0, 0, -6), (270, 136, 12))
    for s in (-1, 1):
        for k in range(6):
            m.box('#ffd23f' if k % 2 == 0 else DARK, (-112.5 + k * 45, s * 62, -5.5), (45, 12, 13))
        m.box(CAST, (0, s * 40, 1.5), (230, 10, 3))                 # the wheel channel's rails
    m.cyl(LIGHT, (0, 0, -200), 22, 190, steps=10)                    # the ram's piston
    m.cyl(CAST, (0, 0, -24), 40, 14, steps=12)
    for x in (-110, 110):
        m.box(RED, (x, 0, -13), (10, 110, 5), glow=0.8)


def carrier_flap(m):
    """An air brake (origin: its hinge, the panel runs back along -X from it and stands 150 cm). The two swing out
    from the top deck's flanks while the brake check lasts."""
    for k in range(6):
        m.box(RED if k % 2 == 0 else '#f4f2e8', (-22 - k * 44, 0, 0), (44, 10, 150))
    m.box(CAST, (-132, 0, 79), (270, 14, 8))
    m.box(CAST, (-132, 0, -79), (270, 14, 8))
    m.box(RED, (-250, 0, 0), (24, 14, 110), glow=1.0)
    m.cyl(METAL, (0, 0, -85), 9, 170, steps=8)


def carrier_lamp(m, color):
    """A pair of launch lamps, one for each rear post (origin between them). Shown only while they are lit."""
    hw = 32 * U - 16
    for s in (-1, 1):
        m.box(color, (0, s * (hw - 10), 0), (6, 36, 32), glow=2.6)


def carrier_die(m):
    """One of the Dealer's dice (origin: its middle): every face shows one pip."""
    S = 170.0
    m.box('#f4f2e8', (0, 0, 0), (S, S, S))
    for axis in range(3):
        for way in (-1, 1):
            c = [0, 0, 0]
            c[axis] = way * (S / 2 + 1)
            size = [S * 0.34] * 3
            size[axis] = 4
            m.box(RED, tuple(c), tuple(size), glow=0.5)


# leaf: (builder, offset, wreck). wreck = it flies off as a wreck of its own when the boss dies, so it needs a
# collision box (a mesh without one cannot be a rigid body).
BOSS_PARTS = {
    'SM_TeslaTop': (ft_v33.tesla_top, (0, 0, 0), True),
    'SM_TeslaTopB': (_tesla_top_broken, (0, 0, 0), True),
    'SM_TeslaArc': (ft_v33.tesla_arc, (0, 0, 0), False),
    'SM_TeslaCollar': (ft_v33.tesla_collar, (0, 0, 0), False),
    'SM_CarrierLift': (carrier_lift, (0, 0, 0), True),
    'SM_CarrierFlap': (carrier_flap, (0, 0, 0), True),
    'SM_CarrierLampA': (lambda m: carrier_lamp(m, '#ff8a2a'), (0, 0, 0), False),
    'SM_CarrierLampG': (lambda m: carrier_lamp(m, '#3dff6a'), (0, 0, 0), False),
    'SM_CarrierDie': (carrier_die, (0, 0, 0), True),
    'SM_JuggTurret': (jugg_turret, (0, 0, 0), True),
    'SM_JuggGun': (jugg_gun, (0, 0, 0), True),
    'SM_JuggShield': (jugg_shield, (0, 0, 0), True),
    'SM_JuggTray': (jugg_tray, (0, 0, 0), False),
    'SM_FireCannon': (fire_cannon, (0, 0, 0), True),
    'SM_FireLadder': (fire_ladder, (0, 0, 0), True),
    'SM_SharkTail': (shark_tail, (0, 0, 0), True),
    'SM_SharkFin': (shark_fin, (0, 0, 0), True),
    'SM_SharkFinB': (lambda m: shark_fin(m, True), (0, 0, 0), True),
    'SM_SharkJaw': (shark_jaw, (0, 0, 0), True),
    'SM_LimoTurret': (limo_turret, (0, 0, 0), True),
    'SM_LimoPod': (limo_pod, (0, 0, 0), True),
    'SM_YetiArmL': (lambda m: _yeti_arm(m, -1), (0, 285, 0), False),
    'SM_YetiArmR': (lambda m: _yeti_arm(m, 1), (0, -285, 0), False),
    'SM_YetiIce': (yeti_ice, (0, 0, 0), True),
    'SM_ClownRig': (ft_v33.clown_rig, (-625, 0, 0), True),
    'SM_ClownProp': (ft_v33.clown_prop, (0, 0, 0), True),
    'SM_ClownHead': (clown_head, (0, 0, 0), True),
    'SM_ClownJaw': (clown_jaw, (0, 0, 0), False),
    'SM_ClownBalloons': (clown_balloons, (0, 0, 0), False),
    'SM_ClownBalloonsB': (lambda m: clown_balloons(m, 5), (0, 0, 0), False),
    'SM_ClownBalloonsC': (lambda m: clown_balloons(m, 3), (0, 0, 0), False),
    'SM_ClownLid': (clown_lid, (0, 0, 0), False),
    'SM_ClownPies': (clown_pies, (0, 0, 0), False),
}
# boss: its body mesh, then its parts
BOSS_SETS = {
    'juggernaut': ('SM_Juggernaut', 'SM_JuggTurret', 'SM_JuggGun', 'SM_JuggShield', 'SM_JuggTray'),
    'alchemist': ('SM_Tanker', 'SM_TeslaTop', 'SM_TeslaTopB', 'SM_TeslaArc', 'SM_TeslaCollar'),
    'dealer': ('SM_Carrier', 'SM_CarrierLift', 'SM_CarrierFlap', 'SM_CarrierLampA', 'SM_CarrierLampG', 'SM_CarrierDie'),
    'engine13': ('SM_FireTruck', 'SM_FireCannon', 'SM_FireLadder'),
    'chuckles': ('SM_ClownCar', 'SM_ClownRig', 'SM_ClownProp', 'SM_ClownHead', 'SM_ClownJaw', 'SM_ClownBalloons', 'SM_ClownBalloonsB',
                 'SM_ClownBalloonsC', 'SM_ClownLid', 'SM_ClownPies'),
    'yeti': ('SM_Yeti', 'SM_YetiArms', 'SM_YetiArmL', 'SM_YetiArmR', 'SM_YetiIce'),
    'shark': ('SM_SharkCar', 'SM_SharkTail', 'SM_SharkFin', 'SM_SharkFinB', 'SM_SharkJaw'),
    'chairman': ('SM_Limo', 'SM_LimoTurret', 'SM_LimoPod'),
}

if not getattr(ft_visual_art.refine, 'boss_parts', False):
    _refine = ft_visual_art.refine

    def _refine_but_boss_parts(meshes, path):
        """ft_visual_art.refine gives any /Characters mesh it does not know the generic crew panels: on a boss's
        moving part that was an identification board across the Tesla coil's toroid and across the clown's rig.
        The parts listed here are finished by their own builders."""
        if path.startswith(ROOT + 'Bosses/') and path.rsplit('/', 1)[-1] in BOSS_PARTS:
            return
        return _refine(meshes, path)

    _refine_but_boss_parts.boss_parts = True
    ft_visual_art.refine = _refine_but_boss_parts


def _box_collision(path, wheels=()):
    """One box round the whole mesh, joined with its wheels' extents when given (wheels.json rows, already in the
    saved mesh's space): what ft_v33.split_vehicle gave a body when its package was made. A mesh updated in place
    keeps its old box, so a body whose parts moved out is fitted again here (a wreck tumbles about its box)."""
    sm = AL.load_asset(path)
    b = sm.get_bounding_box()
    lo = [b.min.x, b.min.y, b.min.z]
    hi = [b.max.x, b.max.y, b.max.z]
    for x, y, z, r, w in wheels:
        lo = [min(lo[0], x - r), min(lo[1], y - w / 2.0), min(lo[2], z - r)]
        hi = [max(hi[0], x + r), max(hi[1], y + w / 2.0), max(hi[2], z + r)]
    c = [(lo[i] + hi[i]) / 2.0 for i in range(3)]
    d = [max(hi[i] - lo[i], 2) for i in range(3)]
    box = unreal.DynamicMesh()
    unreal.GeometryScript_Primitives.append_box(
        box, unreal.GeometryScriptPrimitiveOptions(),
        unreal.Transform(unreal.Vector(*c), unreal.Rotator(0, 0, 0), unreal.Vector(1, 1, 1)),
        d[0], d[1], d[2], 0, 0, 0, unreal.GeometryScriptPrimitiveOriginMode.CENTER)
    options = unreal.GeometryScriptCollisionFromMeshOptions()
    options.set_editor_property('method', unreal.GeometryScriptCollisionGenerationMethod.ALIGNED_BOXES)
    unreal.GeometryScript_Collision.set_static_mesh_collision_from_mesh(box, sm, options)
    if not AL.save_loaded_asset(sm):
        raise RuntimeError('could not save the collision of ' + path)


def _save_part(leaf):
    build, offset, wreck = BOSS_PARTS[leaf]
    path = ROOT + 'Bosses/' + leaf
    m = ft_art.Model(leaf)
    build(m)
    if ft_v33.save(m, path, offset) is None:
        raise RuntimeError('could not save boss part: ' + path)
    if wreck:
        try:
            _box_collision(path)
        except Exception as e:  # noqa: the part still shows; it only cannot fly off as a wreck of its own
            print('JAM WARN no collision box for %s: %r' % (path, e))
    return path


def rebuild_bosses(names=None, bodies=True, parts=True):
    """Export the bosses' bodies (existing packages, in place) and their moving parts (made when missing), and
    nothing else: the rest of the enemies are ft_enemy_overhaul.rebuild()'s. names: keys of BOSS_SETS (default all)."""
    wanted = set(leaf for name in (names or sorted(BOSS_SETS)) for leaf in BOSS_SETS[name])
    wheel_table = ft_v33.load_wheels_json()
    saved = []
    if bodies:
        for path, build, offset, split in _jobs():
            folder, leaf = path.rsplit('/', 1)
            if folder == ROOT + 'Bosses' and leaf in wanted and leaf not in BOSS_PARTS:
                if ft_v33.save(_body(path, build, offset, split, wheel_table), path, offset) is None:
                    raise RuntimeError('could not save boss body: ' + path)
                if leaf != 'SM_YetiArms':
                    try:
                        _box_collision(path, wheel_table.get(path, ()))
                    except Exception as e:  # noqa: the body still shows; its wreck keeps the old box
                        print('JAM WARN no new collision box for %s: %r' % (path, e))
                saved.append(path)
    if parts:
        saved += [_save_part(leaf) for leaf in sorted(BOSS_PARTS) if leaf in wanted]
    return saved


def _jobs():
    source = ft_v33.split_jobs()
    for path, build, offset in source['fleet'] + source['themed'] + source['bosses']:
        # (SM_Carrier and SM_Juggernaut: ft_v33.split_jobs names the overhaul's builders itself now)
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
    # overhaul: and the bosses' moving parts (the yeti's arms are a job above already)
    saved += [_save_part(leaf) for leaf in sorted(BOSS_PARTS) if ROOT + 'Bosses/' + leaf not in paths]
    return saved
