"""FULL THROTTLE 2.8: THE ABOMINABLE redrawn, and every level's Syndicate dressed for its level.

THE ABOMINABLE (SM_Yeti + SM_YetiArms): an original design in the spirit of the classic ski-game abominable
snowman: huge, white and shaggy, a small head sunk into its shoulders, both arms thrown up over its head with
black claws, a big open roaring mouth, angry little eyes. It faces BACK up the run at the player (-X); the game
turns it round (it chases in from behind first). The arms are their own model pivoting at the shoulders
(SHOULDER_Z, = AFTEntityActor::YetiShoulderZ) so the game can wave them.

The themed Syndicate (Themed/L<n>/SM_Moto, SM_Rammer, SM_Suv, SM_Truck, SM_Heli, SM_Henchman). Bayshore (L1 in the
game, index 0) keeps the classic purple Syndicate. Same footprints and origins as the classic models (ft_fleet,
ft_heli, ft_export.henchman), so hit boxes and placement are unchanged:

    1 CHEMICAL WORKS  HAZMAT CREW    hazard yellow and black, gas masks, glowing green toxic drums, a sprayer turret
    2 NEVADA          DUST DEVILS    sun-faded rust and sand, dusters, goggles and bandanas, bull horns, roll cages
    3 VOLCANO         MAGMA CORPS    charcoal with glowing lava cracks, silver fire suits with gold visors, heat shields
    4 THEME PARK      CARNIVAL CREW  candy stripes, jesters on the bikes, a bumper car, a circus wagon, clowns
    5 GLACIER         ICE PATROL     snowmobiles on a ski and a track, blue parkas with fur hoods, a plough, a snowcat
    6 ABYSS           DEEP DIVERS    brass diving helmets, yellow mini-subs with portholes, propellers, a periscope
    7 METRO           THE SUITS      gloss black and gold, mirrored visors, a cash truck with a vault door

Run over the Unreal MCP connector:  import ft_v28; ft_v28.export()    (or export(yeti_only=True))
"""
import math
import unreal
import ft_art
import ft_export
import ft_fleet
import ft_heli
from ft_car import wheel, BLACK, GLASS, CHROME, STEEL, TYRE, GOLD, PINK, CYAN, YEL, WHITE
from ft_desert import rod, tilt_box

save_mesh, model = ft_export.save_mesh, ft_export.model
U = ft_fleet.U
SHOULDER_Z = 820.0


def cone(m, col, a, b, r, steps=7, glow=0.0, tip=0.0):
    """A cone from base centre a to tip b (a tuft of fur, a claw, a tooth, a horn)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=tip)


# =============================================================================================== THE ABOMINABLE
FUR, FUR2, FUR3 = '#f7faff', '#e3ebf7', '#c6d3e9'
FACE, DARK, MOUTH, TONGUE, TOOTH, CLAW = '#7e8aa8', '#16131f', '#3e0b1a', '#ff6f8a', '#ffffff', '#24212d'
FURS = (FUR, FUR2, FUR3)


def _shag(m, c, rx, ry, rz, rows, per_row, z0=0.15, z1=0.92, skip=None, length=(110, 170), r=(50, 66), seed=0):
    """Tufts of fur all over an ellipsoid (centre c, radii rx ry rz): cones hanging outward and down."""
    k = seed
    for i in range(rows):
        t = z0 + (z1 - z0) * i / max(1, rows - 1)          # 0 = bottom, 1 = top
        el = math.pi * (t - 0.5)                            # elevation
        n = max(5, int(per_row * math.cos(el) + 0.5))
        for j in range(n):
            k += 1
            az = 2 * math.pi * (j + 0.5 * (i % 2)) / n
            nx, ny, nz = math.cos(el) * math.cos(az), math.cos(el) * math.sin(az), math.sin(el)
            if skip and skip(nx, ny, nz):
                continue
            p = (c[0] + rx * nx * 0.92, c[1] + ry * ny * 0.92, c[2] + rz * nz * 0.92)
            ln = length[0] + (length[1] - length[0]) * ((k * 37) % 11) / 10.0
            # outward, and down: fur hangs
            d = (nx, ny, nz - 1.1)
            dl = math.sqrt(d[0] ** 2 + d[1] ** 2 + d[2] ** 2)
            tip = (p[0] + d[0] / dl * ln, p[1] + d[1] / dl * ln, p[2] + d[2] / dl * ln)
            cone(m, FURS[k % 3], p, tip, r[0] + (r[1] - r[0]) * ((k * 53) % 7) / 6.0, steps=6)


def yeti(m):
    """The body, legs and head (no arms). Faces -X. Origin between the feet, on the snow."""
    # legs mid-bound, short and thick, and big flat feet
    for s, fx, fz in ((-1, -150, 40), (1, 130, 90)):
        hip = (0, s * 150, 330)
        foot = (fx, s * 175, fz + 30)
        rod(m, FUR2, hip, foot, 110, steps=10)
        m.sphere(FUR3, (fx - 30, s * 178, fz), 100, scale=(1.65, 1.0, 0.45), steps=(5, 9))
        for t in range(3):
            cone(m, CLAW, (fx - 170, s * 178 + (t - 1) * 45, fz + 10), (fx - 215, s * 178 + (t - 1) * 55, fz - 15), 16, steps=5)
        _shag(m, ((hip[0] + foot[0]) / 2, (hip[1] + foot[1]) / 2, (hip[2] + foot[2]) / 2), 120, 120, 170, 3, 8, 0.2, 0.8, length=(70, 110), r=(40, 52), seed=s * 7)
    # the torso: a big pear of white fur
    body = (0, 0, 580)
    m.sphere(FUR, body, 330, scale=(0.86, 1.04, 1.2), steps=(10, 16))
    m.sphere(FUR2, (40, 0, 380), 300, scale=(0.95, 1.05, 0.8), steps=(8, 14))      # the haunches
    # shoulder humps the arms come out of
    for s in (-1, 1):
        m.sphere(FUR, (0, s * 270, SHOULDER_Z), 150, steps=(7, 10))
    # fur everywhere except the face
    face = lambda nx, ny, nz: nx < -0.55 and nz > 0.35
    _shag(m, body, 284, 343, 396, 7, 16, 0.08, 0.86, skip=face, seed=1)
    _shag(m, (40, 0, 380), 285, 315, 240, 3, 14, 0.0, 0.45, seed=4)
    # the head, sunk into the shoulders, the face looking back up the run (-X)
    hx, hz = -60, 1010
    m.sphere(FUR, (hx, 0, hz), 185, scale=(0.95, 1.0, 1.0), steps=(9, 14))
    _shag(m, (hx, 0, hz), 176, 185, 185, 4, 12, 0.35, 1.0, skip=lambda nx, ny, nz: nx < -0.35, length=(80, 130), r=(42, 56), seed=9)
    # a crest of shaggy fur on top
    for k in range(5):
        a = (k - 2) * 0.35
        cone(m, FURS[k % 3], (hx + 20, math.sin(a) * 90, hz + 150), (hx + 70, math.sin(a) * 150, hz + 300 - abs(k - 2) * 30), 46, steps=6)
    # the face: a grey-blue mask, angry brows, little black eyes, and a huge open roaring mouth
    fx = hx - 150
    m.sphere(FACE, (fx, 0, hz - 10), 128, scale=(0.42, 1.08, 1.05), steps=(7, 12))
    for s in (-1, 1):
        tilt_box(m, DARK, (fx - 50, s * 22, hz + 62), (fx - 46, s * 92, hz + 92), 22, 18)       # brows: in and down
        m.sphere(TOOTH, (fx - 48, s * 50, hz + 34), 23, steps=(5, 8))
        m.sphere(DARK, (fx - 64, s * 46, hz + 30), 13, steps=(4, 6))
        m.sphere(DARK, (fx - 58, s * 12, hz - 4), 9, steps=(4, 6))                              # nostrils
    m.sphere(MOUTH, (fx - 28, 0, hz - 92), 92, scale=(0.38, 1.0, 0.8), steps=(7, 12))
    m.sphere(TONGUE, (fx - 44, 0, hz - 132), 44, scale=(0.36, 1.15, 0.45), steps=(5, 9))
    for k in range(5):                                                                           # top teeth
        y = (k - 2) * 34
        cone(m, TOOTH, (fx - 50, y, hz - 30), (fx - 56, y, hz - 30 - (42 if k in (0, 4) else 26)), 12, steps=5)
    for s in (-1, 1):                                                                            # bottom fangs
        cone(m, TOOTH, (fx - 50, s * 58, hz - 152), (fx - 56, s * 58, hz - 104), 14, steps=5)
    # snow kicked up behind him
    for k in range(7):
        m.sphere(WHITE, (260 + k * 70, (k % 3 - 1) * 170, 50 + k * 22), 92 - k * 9, steps=(5, 7))


def yeti_arms(m, sides=(-1, 1)):
    """Both arms thrown up over his head, big hands, black claws. Origin = the middle of the shoulder line.
    Overhaul (15 Nov): sides=(-1,) or (1,) draws one arm, for the two arms that move on their own
    (SM_YetiArmL / SM_YetiArmR, each exported with its shoulder at the origin)."""
    for s in sides:
        sh = (0, s * 285, 0)
        el = (-70, s * 470, 300)
        wr = (-60, s * 430, 610)
        rod(m, FUR, sh, el, 100, steps=10)
        rod(m, FUR, el, wr, 92, steps=10)
        m.sphere(FUR, el, 100, steps=(6, 9))
        _shag(m, ((sh[0] + el[0]) / 2, (sh[1] + el[1]) / 2, (sh[2] + el[2]) / 2), 110, 130, 150, 3, 8, 0.15, 0.85, length=(80, 120), r=(42, 54), seed=s * 3)
        _shag(m, ((el[0] + wr[0]) / 2, (el[1] + wr[1]) / 2, (el[2] + wr[2]) / 2), 105, 105, 160, 3, 8, 0.15, 0.85, length=(70, 110), r=(40, 50), seed=s * 5)
        # the hand, palm toward the player, fingers spread with black claws
        hc = (-80, s * 425, 700)
        m.sphere(FUR2, hc, 115, scale=(0.62, 1.05, 1.0), steps=(6, 10))
        m.sphere(FACE, (hc[0] - 52, hc[1], hc[2] - 5), 76, scale=(0.3, 1.0, 1.0), steps=(5, 8))   # bare palm
        for f in range(4):
            a = math.radians(-50 + f * 33) * s
            base = (hc[0] - 20, hc[1] + math.sin(a) * 95, hc[2] + math.cos(a) * 95)
            tipf = (hc[0] - 40, hc[1] + math.sin(a) * 190, hc[2] + math.cos(a) * 190)
            rod(m, FUR2, base, tipf, 30, steps=6)
            cone(m, CLAW, tipf, (tipf[0] - 50, hc[1] + math.sin(a) * 255, hc[2] + math.cos(a) * 250), 22, steps=5)
        # the thumb
        cone(m, FUR2, (hc[0] - 10, hc[1] - s * 70, hc[2] - 60), (hc[0] - 50, hc[1] - s * 150, hc[2] - 20), 30, steps=6)


# =============================================================================================== themed Syndicate
THEMES = {
    1: dict(name='HAZMAT CREW',
            bike='#ffcf1f', bike2='#1c1a26', stripe='#3dff6a', suit='#ffd23f', suit2='#e0b416', skin='#e0a070', helmet='#ffd23f', visor='#3dff6a',
            rammer='#ffcf1f', rammer_acc='#1c1a26', suv='#2b2f2a', suv_acc='#ffd23f', truck='#d9d4c0', truck_cab='#ffcf1f', truck_band='#1c1a26',
            heli='#ffcf1f', heli2='#1c1a26', heli_stripe='#1c1a26', heli_glow='#3dff6a', tie='#3dff6a'),
    2: dict(name='DUST DEVILS',
            bike='#c8743a', bike2='#5a3a24', stripe='#ffd23f', suit='#7a4a2a', suit2='#5a3420', skin='#d79a68', helmet='#d9b98a', visor='#ff8a2a',
            rammer='#d08a4a', rammer_acc='#8a4a2a', suv='#d9c08a', suv_acc='#c0392b', truck='#c9a96a', truck_cab='#a8834e', truck_band='#6a4a2a',
            heli='#c9a96a', heli2='#8a6a3e', heli_stripe='#c0392b', heli_glow='#ff8a2a', tie='#c0392b'),
    3: dict(name='MAGMA CORPS',
            bike='#1e1820', bike2='#2a2230', stripe='#ff6a1a', suit='#c9ccd6', suit2='#9a9eac', skin='#e0a070', helmet='#c9ccd6', visor='#ffc93c',
            rammer='#262028', rammer_acc='#ff6a1a', suv='#1e1820', suv_acc='#ff6a1a', truck='#2a2430', truck_cab='#3a3240', truck_band='#ff6a1a',
            heli='#1e1820', heli2='#2a2230', heli_stripe='#ff6a1a', heli_glow='#ff6a1a', tie='#ff6a1a'),
    4: dict(name='CARNIVAL CREW',
            bike='#ff2d3c', bike2='#fff6ea', stripe='#ffe28a', suit='#7b3bd6', suit2='#ffd23f', skin='#fff6ea', helmet='#ffd23f', visor='#ff4fb8',
            rammer='#2ee6ff', rammer_acc='#ffe28a', suv='#ff2d3c', suv_acc='#fff6ea', truck='#d0213a', truck_cab='#ffd23f', truck_band='#ffc93c',
            heli='#ff4fb8', heli2='#fff6ea', heli_stripe='#ffd23f', heli_glow='#ffe28a', tie='#ff2d3c'),
    5: dict(name='ICE PATROL',
            bike='#1f4fa8', bike2='#e8f0ff', stripe='#ff8a2a', suit='#1f4fa8', suit2='#163a80', skin='#e0a070', helmet='#e8f0ff', visor='#ff8a2a',
            rammer='#2a62c8', rammer_acc='#ffd23f', suv='#d8262f', suv_acc='#fff6ea', truck='#ff7a1a', truck_cab='#ff7a1a', truck_band='#1c1a26',
            heli='#f4f9ff', heli2='#d8262f', heli_stripe='#d8262f', heli_glow='#ff8a2a', tie='#ff8a2a'),
    6: dict(name='DEEP DIVERS',
            bike='#1fa39a', bike2='#c9a04a', stripe='#5ff0ff', suit='#2a3a4a', suit2='#1e2a36', skin='#e0a070', helmet='#c9a04a', visor='#5ff0ff',
            rammer='#ffd23f', rammer_acc='#c9a04a', suv='#1a6a7a', suv_acc='#c9a04a', truck='#2a4a5a', truck_cab='#335a6c', truck_band='#c9a04a',
            heli='#ffd23f', heli2='#c9a04a', heli_stripe='#1a6a7a', heli_glow='#5ff0ff', tie='#5ff0ff'),
    7: dict(name='THE SUITS',
            bike='#141218', bike2='#2a2832', stripe='#2ee6ff', suit='#1a1820', suit2='#26242e', skin='#e0a070', helmet='#141218', visor='#ffc93c',
            rammer='#18161e', rammer_acc='#ffc93c', suv='#18161e', suv_acc='#ffc93c', truck='#b8bcc8', truck_cab='#a0a4b2', truck_band='#ffc93c',
            heli='#141218', heli2='#2a2832', heli_stripe='#ffc93c', heli_glow='#2ee6ff', tie='#ffc93c'),
}


# ---------------------------------------------------------------------------------------- the riders' heads
def rider_head(m, L, T, x, z, r=24):
    """The helmet/headgear for a rider (or henchman) whose head is centred at (x, 0, z), facing +X."""
    if L == 1:      # a hazmat hood with a black gas mask, two filter canisters and green goggles
        m.sphere(T['helmet'], (x, 0, z), r * 1.1, scale=(1.05, 1, 1.05))
        m.sphere(BLACK, (x + r * 0.75, 0, z - r * 0.35), r * 0.55, scale=(0.8, 1, 0.8))
        for s in (-1, 1):
            m.cyl(BLACK, (x + r * 0.9, s * r * 0.45, z - r * 0.55), r * 0.24, r * 0.5, rot=(-90, s * 35, 0), steps=8)
            m.sphere(T['visor'], (x + r * 0.85, s * r * 0.36, z + r * 0.2), r * 0.24, glow=0.8)
    elif L == 2:    # a tan helmet, goggles pushed down, a red bandana over the face
        m.sphere(T['helmet'], (x, 0, z), r * 1.05, scale=(1.1, 1, 1))
        m.box('#3a2a22', (x + r * 0.6, 0, z + r * 0.25), (r * 0.6, r * 1.8, r * 0.35))
        for s in (-1, 1):
            m.sphere(T['visor'], (x + r * 0.95, s * r * 0.4, z + r * 0.25), r * 0.26, glow=0.5)
        m.box(T['tie'], (x + r * 0.6, 0, z - r * 0.45), (r * 0.9, r * 1.95, r * 0.6))
        cone(m, T['tie'], (x - r * 0.6, 0, z - r * 0.4), (x - r * 1.8, r * 0.3, z - r * 0.9), r * 0.35, steps=5)
    elif L == 3:    # a silver fire-suit hood with a big gold visor
        m.sphere(T['helmet'], (x, 0, z), r * 1.15, scale=(1.05, 1, 1.15))
        m.box(T['visor'], (x + r * 0.95, 0, z + r * 0.05), (r * 0.25, r * 1.5, r * 0.9), glow=0.35)
        m.box(T['suit2'], (x, 0, z - r * 1.0), (r * 1.6, r * 2.3, r * 0.5))
    elif L == 4:    # a jester's hat: two floppy horns with bells, and a white clown face
        m.sphere(T['skin'], (x, 0, z), r)
        m.sphere('#ff2d3c', (x + r * 0.95, 0, z - r * 0.1), r * 0.24, glow=0.4)
        for s in (-1, 1):
            col = T['helmet'] if s < 0 else T['visor']
            cone(m, col, (x, s * r * 0.4, z + r * 0.6), (x - r * 0.8, s * r * 2.0, z + r * 1.6), r * 0.5, steps=6)
            m.sphere(T['stripe'], (x - r * 0.8, s * r * 2.0, z + r * 1.6), r * 0.25, glow=0.8)
    elif L == 5:    # a parka hood with a white fur ruff and wide orange goggles
        m.sphere(T['suit'], (x, 0, z), r * 1.15, scale=(1.05, 1, 1.1))
        for k in range(10):
            a = 2 * math.pi * k / 10
            m.sphere('#f4f9ff', (x + r * 0.75, math.cos(a) * r * 0.75, z + math.sin(a) * r * 0.8), r * 0.3, steps=(4, 6))
        m.box(T['visor'], (x + r * 0.95, 0, z + r * 0.2), (r * 0.25, r * 1.5, r * 0.4), glow=0.7)
    elif L == 6:    # a brass diving helmet with a round glowing porthole and side ports
        m.sphere(T['helmet'], (x, 0, z), r * 1.35)
        m.cyl(T['visor'], (x + r * 1.1, 0, z), r * 0.6, r * 0.35, rot=(-90, 0, 0), steps=12, glow=0.7)
        m.cyl(T['helmet'], (x + r * 1.05, 0, z), r * 0.75, r * 0.2, rot=(-90, 0, 0), steps=12)
        for s in (-1, 1):
            m.cyl(T['visor'], (x, s * r * 1.2, z), r * 0.32, r * 0.2, rot=(0, 0, 90 * s), steps=10, glow=0.5)
        m.cyl(T['helmet'], (x, 0, z - r * 1.4), r * 1.2, r * 0.4, steps=12)
    elif L == 7:    # a gloss black helmet with a mirrored gold visor
        m.sphere(T['helmet'], (x, 0, z), r * 1.1, scale=(1.1, 1, 1))
        m.box(T['visor'], (x + r * 0.9, 0, z + r * 0.05), (r * 0.25, r * 1.45, r * 0.5), glow=0.3)
        m.box(T['stripe'], (x - r * 0.2, 0, z + r * 1.05), (r * 1.6, r * 0.2, r * 0.12), glow=0.8)


# ------------------------------------------------------------------------------------------------- the bike
def moto(m, L, T):
    from ft_bikes import moto as build_moto
    build_moto(m, L, T)


# ----------------------------------------------------------------------------------------------- the rammer
def rammer(m, L, T):
    W, Lg = 30 * U, 470
    col = T['rammer']
    ft_fleet.car(m, W, Lg, col, cab=0.38, cab_x=(0.34, 0.66), body_h=74, cab_h=118, accent=T['rammer_acc'])
    hw = W / 2.0 - 12
    if L == 4:
        # a bumper car: a fat black rubber ring round it and a pole with a spark on top
        for s in (-1, 1):
            m.box(TYRE, (Lg / 2.0, s * (hw + 8), 34), (Lg + 30, 16, 26))
        m.box(TYRE, (Lg + 12, 0, 34), (16, hw * 2 + 30, 26))
        m.box(TYRE, (-10, 0, 34), (16, hw * 2 + 30, 26))
        rod(m, STEEL, (40, 0, 90), (20, 0, 330), 4)
        m.sphere(T['rammer_acc'], (20, 0, 336), 14, glow=2.0)
        for k in range(6):
            m.sphere(T['rammer_acc'], (60 + k * 60, hw + 1, 70), 7, steps=(3, 5), glow=1.4)
            m.sphere(T['rammer_acc'], (60 + k * 60, -hw - 1, 70), 7, steps=(3, 5), glow=1.4)
        return
    if L == 5:
        # a snow plough blade instead of the ram bar
        m.side(T['rammer_acc'], [(Lg + 10, 10), (Lg + 70, 10), (Lg + 40, 110), (Lg + 10, 90)], -W / 2.0 - 20, W / 2.0 + 20)
        for k in range(5):
            m.box(BLACK, (Lg + 46, -W / 2.0 + k * W / 4.0, 60), (4, 20, 80))
    elif L == 6:
        # a mini-sub: a ram nose cone, portholes, a tail fin
        cone(m, T['rammer_acc'], (Lg - 10, 0, 50), (Lg + 90, 0, 50), 40, steps=12)
        for k in range(3):
            for s in (-1, 1):
                m.cyl('#5ff0ff', (Lg * 0.3 + k * 70, s * (hw + 1), 56), 14, 3, rot=(0, 0, 90), steps=10, glow=0.8)
        m.side(T['rammer_acc'], [(0, 76), (60, 76), (20, 170), (-10, 170)], -4, 4)
    else:
        m.box(STEEL if L != 7 else GOLD, (Lg + 6, 0, 46), (18, W - 10, 22))
        for s in (-1, 1):
            m.box(STEEL if L != 7 else GOLD, (Lg + 2, s * (W / 2 - 20), 46), (14, 10, 46))
    if L == 1:      # a toxic drum on the back deck and chevrons on the bonnet
        m.cyl('#2a6a3a', (60, 0, 76), 30, 60, steps=12)
        m.cyl(T['stripe'], (60, 0, 104), 31, 8, steps=12, glow=1.0)
        for k in range(3):
            m.side(BLACK, [(Lg * 0.72 + k * 34, 77), (Lg * 0.72 + k * 34 + 16, 77), (Lg * 0.72 + k * 34 + 30, 77.5), (Lg * 0.72 + k * 34 + 14, 77.5)], -hw * 0.8, hw * 0.8)
            m.box(BLACK, (Lg * 0.72 + k * 34 + 10, 0, 76.5), (14, hw * 1.6, 2))
    elif L == 2:    # bull horns on the bonnet, rust patches, a jerrycan
        for s in (-1, 1):
            cone(m, '#efe3c8', (Lg * 0.86, s * 30, 74), (Lg * 0.95, s * 120, 120), 14, steps=8)
            cone(m, '#efe3c8', (Lg * 0.95, s * 120, 120), (Lg * 0.9, s * 150, 170), 9, steps=8)
            m.box(T['rammer_acc'], (Lg * 0.4, s * (hw + 0.6), 50), (90, 2, 24))
        m.box('#4a7a3a', (20, -40, 90), (30, 20, 34))
    elif L == 3:    # glowing lava cracks down the flanks and a heat shield over the cab
        for s in (-1, 1):
            for k in range(5):
                m.box(T['rammer_acc'], (60 + k * 80, s * (hw + 0.8), 48 + (k % 2) * 10), (60, 2, 5), rot=(0, 0, 0), glow=1.6)
        m.box(STEEL, (Lg * 0.5, 0, 122), (160, hw * 1.2, 6))
    elif L == 7:    # a gold grille and a cyan underglow
        m.box(GOLD, (Lg + 1, 0, 60), (3, hw * 1.2, 22))
        m.box(T['stripe'] if 'stripe' in T else CYAN, (Lg / 2.0, 0, 24), (Lg - 40, hw * 2, 3), glow=1.6)
    else:           # the blower (classic)
        m.box(CHROME, (Lg * 0.62, 0, 86), (60, 50, 22))


# --------------------------------------------------------------------------------------------------- the SUV
def suv(m, L, T):
    W, Lg = 36 * U, 520
    ft_fleet.car(m, W, Lg, T['suv'], cab=0.44, cab_x=(0.12, 0.78), body_h=112, cab_h=196, wedge=False, wheel_r=44, accent=T['suv_acc'])
    hw = W / 2.0 - 12
    m.cyl(TYRE, (-24, 0, 92), 40, 22, rot=(90, 0, 0), steps=18)
    m.cyl(GOLD if L != 6 else T['suv_acc'], (-26, 0, 92), 24, 3, rot=(90, 0, 0), steps=16)
    if L == 1:      # a sprayer tank on the roof, nozzle aimed back
        m.cyl('#2a6a3a', (Lg * 0.45, -10, 216), 46, 22, rot=(90, 0, 0), steps=14)
        m.cyl(T['suv_acc'], (Lg * 0.45, 0, 205), 50, 8, steps=14)
        m.cyl('#3dff6a', (Lg * 0.45, 0, 238), 30, 20, steps=12, glow=1.0)
        rod(m, BLACK, (Lg * 0.45, 0, 250), (Lg * 0.2, 0, 262), 6)
        for k in range(6):
            m.side(BLACK, [(40 + k * 80, 40), (60 + k * 80, 40), (90 + k * 80, 110), (70 + k * 80, 110)], -hw - 1.2, hw + 1.2)
        return
    if L == 4:      # a popcorn wagon: a striped awning and a bulb sign
        for k in range(8):
            m.box(T['suv_acc'] if k % 2 else T['suv'], (Lg * 0.15 + k * Lg * 0.09, 0, 214), (Lg * 0.09, W * 0.95, 14))
        m.box('#ffd23f', (Lg * 0.45, 0, 260), (20, W * 0.6, 60))
        for k in range(7):
            m.sphere('#ffe28a', (Lg * 0.45 - 12, -W * 0.27 + k * W * 0.09, 290), 8, steps=(3, 5), glow=1.6)
        return
    if L == 5:      # skis on a roof rack and a light bar
        m.box(BLACK, (Lg * 0.45, 0, 204), (Lg * 0.6, W * 0.8, 6))
        for k in range(4):
            m.box(['#ffd23f', '#2ee6ff', '#fff6ea', '#ff4fb8'][k], (Lg * 0.45, -W * 0.3 + k * W * 0.2, 212), (Lg * 0.66, 14, 4))
        m.box('#ff8a2a', (Lg * 0.75, 0, 208), (16, W * 0.7, 12), glow=1.2)
        return
    if L == 6:      # a periscope and big searchlights
        rod(m, T['suv_acc'], (Lg * 0.5, 0, 196), (Lg * 0.5, 0, 330), 12)
        m.box(T['suv_acc'], (Lg * 0.5 + 20, 0, 336), (60, 26, 26))
        m.box('#5ff0ff', (Lg * 0.5 + 51, 0, 336), (3, 20, 18), glow=1.4)
        for s in (-1, 1):
            m.cyl('#5ff0ff', (Lg * 0.78, s * 50, 204), 18, 10, rot=(-90, 0, 0), steps=12, glow=1.4)
        for k in range(3):
            for s in (-1, 1):
                m.cyl(GLASS, (Lg * 0.25 + k * 90, s * (hw + 1), 70), 16, 3, rot=(0, 0, 90), steps=10)
        return
    # the classic gun turret (Nevada, Volcano, Metro), in the level's kit
    m.box(STEEL, (Lg * 0.45, 0, 206), (90, 80, 22))
    m.cyl(BLACK, (Lg * 0.45, 0, 216), 26, 26, steps=16)
    for y in (-10, 10):
        m.cyl(BLACK, (Lg * 0.45 + 10, y, 230), 4.5, 120, rot=(-90, 0, 0), steps=8)
    if L == 2:      # a roll cage and jerrycans
        for s in (-1, 1):
            rod(m, BLACK, (Lg * 0.15, s * (hw - 10), 196), (Lg * 0.15, s * (hw - 10), 260), 5)
            rod(m, BLACK, (Lg * 0.75, s * (hw - 10), 196), (Lg * 0.75, s * (hw - 10), 260), 5)
            rod(m, BLACK, (Lg * 0.15, s * (hw - 10), 260), (Lg * 0.75, s * (hw - 10), 260), 5)
            m.box('#4a7a3a', (-20, s * 60, 140), (24, 30, 50))
        rod(m, BLACK, (Lg * 0.75, -(hw - 10), 260), (Lg * 0.75, hw - 10, 260), 5)
        for y in (-45, -15, 15, 45):
            m.box('#fff1c0', (Lg * 0.75 + 8, y, 268), (6, 20, 16), glow=0.8)
    elif L == 3:    # a flame nozzle with a glowing tip, and orange underglow
        m.cyl(T['suv_acc'], (Lg * 0.45 + 10, 0, 250), 7, 10, rot=(-90, 0, 0), steps=8, glow=2.0)
        m.box(T['suv_acc'], (Lg / 2.0, 0, 26), (Lg - 40, hw * 2, 3), glow=1.6)
        for s in (-1, 1):
            m.box(STEEL, (Lg * 0.5, s * (hw + 1), 80), (Lg * 0.7, 3, 40))
    elif L == 7:    # gold trim, tinted glass, a cyan light bar
        m.box(T['suv_acc'], (Lg / 2.0, 0, 114), (Lg, hw * 2 + 3, 4))
        m.box('#2ee6ff', (Lg * 0.75, 0, 200), (14, W * 0.7, 10), glow=1.4)


# -------------------------------------------------------------------------------------------------- the truck
def truck(m, L, T):
    W, Lg = 46 * U, 900
    body, cab = T['truck'], T['truck_cab']
    hw = W / 2 - 16
    bh = 240
    m.box(body, (Lg * 0.39, 0, bh / 2 + 18), (Lg * 0.78, hw * 2, bh - 36))
    m.side(cab, [(Lg * 0.8, 40), (Lg, 40), (Lg, 150), (Lg * 0.95, 200), (Lg * 0.8, 210)], -hw + 10, hw - 10)
    m.side(GLASS, [(Lg * 0.9, 150), (Lg * 0.985, 150), (Lg * 0.94, 196), (Lg * 0.9, 200)], -hw + 8, hw - 8)
    m.box(BLACK, (-1, 0, bh / 2 + 18), (3, hw * 1.1, bh * 0.75))
    for s in (-1, 1):
        m.box('#ff3355', (-3, s * (hw - 20), 50), (2, 26, 20), glow=1.0)
    m.box(BLACK, (0, 0, 22), (8, W - 20, 18))
    if L == 5:
        # a snowcat: tank tracks down both sides and a plough
        for s in (-1, 1):
            m.box(TYRE, (Lg * 0.5, s * (W / 2.0 - 20), 50), (Lg * 0.95, 40, 90))
            for k in range(18):
                m.box('#3a3846', (Lg * 0.05 + k * Lg * 0.05, s * (W / 2.0 - 20), 96), (14, 44, 6))
        m.side('#ffd23f', [(Lg + 10, 10), (Lg + 80, 10), (Lg + 50, 130), (Lg + 10, 110)], -W / 2.0 - 30, W / 2.0 + 30)
        m.box(T['truck_band'], (Lg * 0.39, 0, bh * 0.62), (Lg * 0.785, hw * 2 + 3, 20))
        m.box('#ff8a2a', (Lg * 0.9, 0, 222), (30, 120, 14), glow=0.8)
        return
    for s in (-1, 1):
        for x in (0.1, 0.24, 0.62, 0.9):
            wheel(m, Lg * x, 46, 46, hw - 40, W / 2.0, s)
    if L == 1:      # hazard band and green toxic drums lashed on the roof
        for k in range(12):
            m.box(T['truck_band'] if k % 2 else '#ffd23f', (Lg * 0.03 + k * Lg * 0.065, 0, bh * 0.45), (Lg * 0.065, hw * 2 + 3, 30))
        for k in range(3):
            m.cyl('#2a6a3a', (Lg * 0.15 + k * 120, 0, bh + 2), 44, 70, steps=12)
            m.cyl('#3dff6a', (Lg * 0.15 + k * 120, 0, bh + 40), 45, 10, steps=12, glow=1.0)
    elif L == 2:    # sand camo blotches and a spare tyre
        import random
        r = random.Random(4)
        for k in range(14):
            m.box(T['truck_band'], (r.uniform(40, Lg * 0.74), (1 if k % 2 else -1) * (hw + 1), r.uniform(60, bh - 20)), (r.uniform(50, 120), 2, r.uniform(30, 70)))
        m.cyl(TYRE, (Lg * 0.4, 0, bh + 8), 50, 24, steps=16)
        m.cyl(GOLD, (Lg * 0.4, 0, bh + 30), 28, 4, steps=12)
    elif L == 3:    # riveted heat-shield plates with glowing vents
        for k in range(6):
            m.box('#4a4458', (Lg * 0.06 + k * Lg * 0.125, 0, bh * 0.55), (Lg * 0.11, hw * 2 + 4, bh * 0.6))
        for s in (-1, 1):
            for k in range(5):
                m.box(T['truck_band'], (Lg * 0.12 + k * Lg * 0.13, s * (hw + 3), bh * 0.85), (50, 2, 8), glow=1.8)
    elif L == 4:    # a circus wagon: gold trim, stars, a striped roof
        for k in range(10):
            m.box('#fff6ea' if k % 2 else T['truck'], (Lg * 0.04 + k * Lg * 0.078, 0, bh + 2), (Lg * 0.078, hw * 2 + 6, 14))
        for s in (-1, 1):
            m.box(T['truck_band'], (Lg * 0.39, s * (hw + 1), 40), (Lg * 0.78, 3, 12))
            m.box(T['truck_band'], (Lg * 0.39, s * (hw + 1), bh - 22), (Lg * 0.78, 3, 12))
            for k in range(4):
                m.sphere('#ffe28a', (Lg * 0.1 + k * Lg * 0.18, s * (hw + 2), bh * 0.55), 18, steps=(4, 6), glow=1.2)
    elif L == 6:    # brass rivets and glowing portholes
        for s in (-1, 1):
            for k in range(5):
                m.cyl(T['truck_band'], (Lg * 0.1 + k * Lg * 0.15, s * (hw + 1), bh * 0.55), 34, 3, rot=(0, 0, 90), steps=12)
                m.cyl('#5ff0ff', (Lg * 0.1 + k * Lg * 0.15, s * (hw + 3), bh * 0.55), 24, 3, rot=(0, 0, 90), steps=12, glow=0.9)
        m.box(T['truck_band'], (Lg * 0.39, 0, bh - 16), (Lg * 0.79, hw * 2 + 3, 8))
    elif L == 7:    # an armoured cash truck: gold stripe and a round vault door at the back
        m.box(T['truck_band'], (Lg * 0.39, 0, bh * 0.5), (Lg * 0.785, hw * 2 + 3, 16))
        m.cyl('#8f9bb3', (2, 0, bh * 0.55), 80, 8, rot=(0, 90, 90), steps=18)
        m.cyl(GOLD, (-4, 0, bh * 0.55), 18, 6, rot=(0, 90, 90), steps=10)
        m.box(GOLD, (-5, 0, bh * 0.55), (3, 120, 6))
        m.box(GOLD, (-5, 0, bh * 0.55), (3, 6, 120))
    m.box(T['truck_band'], (Lg * 0.9, 0, 222), (30, 120, 14), glow=0.6)


# --------------------------------------------------------------------------------------------- the chopper
def heli(m, L, T):
    """ft_heli.heli with the level's paint (its colours are module globals), plus the level's kit."""
    keep = (ft_heli.PURPLE, ft_heli.PURPLE_D, ft_heli.HAZARD, ft_heli.PINK)
    ft_heli.PURPLE, ft_heli.PURPLE_D, ft_heli.HAZARD, ft_heli.PINK = T['heli'], T['heli2'], T['heli_stripe'], T['heli_glow']
    try:
        ft_heli.heli(m)
    finally:
        ft_heli.PURPLE, ft_heli.PURPLE_D, ft_heli.HAZARD, ft_heli.PINK = keep
    if L == 4:      # balloons tied to the skids
        for k, c in enumerate(('#ff2d3c', '#ffd23f', '#2ee6ff')):
            rod(m, '#e8e0f0', (-80 + k * 80, 88, 6), (-80 + k * 80, 150, -160), 2, steps=4)
            m.sphere(c, (-80 + k * 80, 150, -200), 40, scale=(1, 1, 1.2), steps=(6, 8), glow=0.25)
    elif L == 6:    # portholes
        for s in (-1, 1):
            for k in range(3):
                m.cyl('#5ff0ff', (-120 + k * 45, s * 97, 150), 14, 3, rot=(0, 0, 90), steps=10, glow=0.8)
    elif L == 3:    # a flame stripe down the boom
        for k in range(6):
            m.side(T['heli_glow'], [(-170 - k * 60, 168), (-140 - k * 60, 168), (-170 - k * 60, 186)], -23, 23, glow=1.4)


# ------------------------------------------------------------------------------------------- the henchman
def henchman(m, L, T):
    suit, tie = T['suit'], T['tie']
    BLK = '#141218'
    for s in (-1, 1):
        m.box(BLK if L in (7,) else T['suit2'], (0, s * 14, 45), (24, 16, 90))
        m.box(suit, (0, s * 34, 128), (16, 14, 70), rot=(0, 0, s * 20))
    m.box(suit, (0, 0, 125), (30, 46, 70))
    if L == 7:
        m.box('#fff6ea', (15.5, 0, 140), (1, 14, 30))
    m.box(tie, (16, 0, 132), (2, 8, 40), glow=0.4 if L in (1, 6) else 0.0)
    if L == 4:
        for k in range(6):
            m.sphere(['#ff2d3c', '#2ee6ff', '#3dff6a'][k % 3], (15.5, -14 + (k % 3) * 14, 110 + (k // 3) * 30), 4, steps=(3, 5))
    if L == 7:      # sunglasses on a bare head
        m.sphere('#e0a070', (0, 0, 176), 15)
        m.box('#0a0a10', (13, 0, 179), (4, 26, 6))
        m.box('#2a2430', (0, 0, 186), (28, 30, 8))
    else:
        rider_head(m, L, T, 0, 176, r=15)
    if L == 2:      # a cowboy hat over the helmet
        m.cyl('#6a4a2a', (0, 0, 190), 30, 3, steps=14)
        m.cyl('#6a4a2a', (0, 0, 192), 15, 16, steps=12)


# =============================================================================================== export
KINDS = (('SM_Moto', moto, (-115, 0, 0)), ('SM_Rammer', rammer, (-235, 0, 0)), ('SM_Suv', suv, (-260, 0, 0)),
         ('SM_Truck', truck, (-450, 0, 0)), ('SM_Heli', heli, (0, 0, 0)), ('SM_Henchman', henchman, (0, 0, 0)))


def export_yeti():
    import ft_v33
    C = '/Game/Characters/Bosses/'
    ft_v33.save(model(yeti), C + 'SM_Yeti', collision='box')
    ft_v33.save(model(yeti_arms), C + 'SM_YetiArms')
    return 'yeti exported'


def export(levels=None, yeti_only=False):
    import ft_v33
    out = [export_yeti()]
    if yeti_only:
        return out
    levels = tuple(levels or sorted(THEMES))
    for L in levels:
        T = THEMES[L]
        for name, fn, off in KINDS:
            path = '/Game/Characters/Themed/L%d/%s' % (L, name)
            if name in ('SM_Rammer', 'SM_Suv', 'SM_Truck'):
                ft_v33.split_vehicle(path, lambda m: fn(m, L, T), off)
            elif name == 'SM_Moto':
                save_mesh(model(lambda m: fn(m, L, T)).meshes, path, 'car', off, collision='box')
        out.append('L%d %s' % (L, T['name']))
    ft_v33.export_heli(levels)
    ft_v33.export_swat(levels)
    return out


# =============================================================================================== a look-board
def board(at=(0, 60000, 0)):
    """Spawn every themed model in a grid in the open level (for a screenshot), the yeti at the front."""
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    made = []
    def place(path, loc, yaw=0):
        sm = unreal.load_asset(path)
        if not sm:
            return
        a = sub.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(*loc), unreal.Rotator(0, 0, yaw))
        a.static_mesh_component.set_static_mesh(sm)
        a.set_folder_path(unreal.Name('FT28Board'))
        made.append(a)
        return a
    x0, y0, z0 = at
    place('/Game/Characters/Bosses/SM_Yeti', (x0 - 1400, y0, z0), 0)
    place('/Game/Characters/Bosses/SM_YetiArms', (x0 - 1400, y0, z0 + SHOULDER_Z), 0)
    for i, L in enumerate([0] + sorted(THEMES)):
        for j, (name, fn, off) in enumerate(KINDS):
            p = ('/Game/Characters/Themed/L%d/%s' % (L, name)) if L else ('/Game/Characters/%s/%s' % ('Choppers' if name == 'SM_Heli' else 'Syndicate', name))
            place(p, (x0 + j * 1300, y0 + (i - 4) * 900, z0), 0)
    return len(made)


def clear_board():
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    n = 0
    for a in sub.get_all_level_actors():
        if str(a.get_folder_path()) == 'FT28Board':
            sub.destroy_actor(a)
            n += 1
    return n
