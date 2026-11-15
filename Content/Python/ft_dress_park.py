"""FULL THROTTLE (Unreal) dressing kit, level 4: the Theme Park (Coaster Cove, at night). Themed lands, one after the
other: the front gate and its plaza, the midway of stalls, Pirate Cove, the lagoon under the coaster, Rocket Land,
the funhouse and the back lot, and the promenade of the finale. Placed by Source/FullThrottle/FTDressL4.inl.
See ft_dress.py for how a piece is drawn (front faces -Y, base on z = 0, long axis along X, cm).

    import ft_dress; ft_dress.run(['park'])

All of it is our own: the park's name is the game's, the mascot (STARRY, a star in a ringmaster's coat) and every
ride, sign and land are invented here; nothing is taken from a real park, brand or film.

MOTION. A ride's moving parts are drawn in "mover" colours (mv(): the same colour one step off, so those parts get
material slots of their own) and MOVERS below says, for each such mesh, what should move, about which axis and through
which point of the mesh. animate() at the bottom hands that to the living-world pass (ft_life.animate: it puts M_Life
on those slots only, so the hub turns and the deck stands still). Until it is run the rides stand in a mid-ride pose.
Run animate() again after every export: the export puts the plain palette back.

UNDER EVERY PIECE (_std): a dark foundation slab 1.6 m deep under the footprint, so a piece that stands where the land
tilts shows a plinth instead of daylight; and a thin hidden post going down 0.85 of the piece's height. The post is
there for the visual-finish pass every level but Bayshore gets on export (ft_visual_art.refine): it adds a service
panel just inside the -X face of a piece's bounding box, between 15% and 43% of its height. On a plain building that
is a welcome detail on the end wall; on a stall, a wheel or a sign on posts it would hang in the air. With the post the
box is deep enough that the panel lies under the ground.

THE LAGOON (SM_Lake, SM_Lake2) is not stood on the land like the rest: every lake is placed at one height above the
level's floor (SiteAfloat in the table), which is the lagoon's water level, so the humps of land under the coaster
rise out of one sheet of water and the hollows between them hold it.
"""
import math
import random
import unreal
import ft_city
import ft_visual_art
from ft_desert import rod, tilt_box

NAVY, DEEP, PURPLE, PLUM = '#2b2350', '#2a2546', '#3a2f6a', '#4a3a78'
MAG, CYAN, AMBER, BULB, CREAM = '#ff4fb8', '#2ee6ff', '#ffd23f', '#ffe28a', '#fff1d6'
RED, WHITE, LAV, MINT, TEAL = '#ff2d3c', '#fff6ea', '#b9a4ff', '#7fe3c4', '#2fc6c0'
WOOD, WOOD_D, PINE, GREY, GREY_L, ROCK, ROCK_D = '#9a6a44', '#6a4c36', '#2f6b52', '#6a6878', '#9a98a8', '#4a4466', '#3a3556'
ORANGE, GOLD, SAND, WATER, SHED, SHED_D, GEN = '#ff8a2a', '#ffc93c', '#efcf9f', '#1c5f8a', '#5a5870', '#4a4860', '#3f6a5a'
G1, G2, G3, G4 = 0.35, 0.9, 1.4, 2.0          # glow: soft, a lit window, a sign, a bulb
GW = 0.2                                      # lit water
BULBS = [BULB, MAG, CYAN, AMBER]
DIR = '/Game/Maps/ThemePark/Dress/'

# what moves (see animate()): mesh -> the M_Life parameters for its mover slots. Axes and pivots are in the mesh's own space (cm).
MOVERS = {
    'SM_JetRide': dict(what='the hub, its six arms and rockets, the planet on top: turns about Z',
                       SpinAxis=(0, 0, 1), SpinPivot=(0, 0, 0), SpinRate=0.7),
    'SM_SwingRide': dict(what='the crown and its ten chairs: turns about Z, faster',
                         SpinAxis=(0, 0, 1), SpinPivot=(0, 0, 0), SpinRate=1.25),
    'SM_PirateShip': dict(what='the ship and its hangers: swings 38 degrees each way about Y through the axle',
                          SpinAxis=(0, 1, 0), SpinPivot=(0, 0, 1350), SpinRate=1.4, SpinSwing=38.0),
    'SM_BigWheel': dict(what='the rims, spokes and gondolas: turns slowly about Y through the hub', far=True,
                        SpinAxis=(0, 1, 0), SpinPivot=(0, 0, 3400), SpinRate=0.1),
}
_MOVER_BASE = (NAVY, PURPLE, PLUM, MAG, CYAN, AMBER, BULB, CREAM, RED, WHITE, LAV, MINT, TEAL, WOOD, WOOD_D, GREY_L, ORANGE, GOLD)


def mv(col):
    """The mover's shade of a colour: one step off in blue, so the part gets its own material slot (see MOTION)."""
    b = int(col[5:7], 16)
    return col[:5] + '%02x' % (b - 1 if b == 255 else b + 1)


def mover_hexes():
    """The mover shades, as ft_life names a slot's colour (hex without #)."""
    return set(mv(c).lstrip('#').lower() for c in _MOVER_BASE)


def animate():
    """Set the rides going: M_Life's Spin on the mover slots of each ride (ft_life.py, the living-world pass). Run in the
    editor after ft_dress.run(['park']); it changes material slots only and saves each mesh in place."""
    import ft_life
    movers = mover_hexes()
    n = 0
    for name, p in MOVERS.items():
        kw = dict((k, v) for k, v in p.items() if k not in ('what', 'far'))
        n += ft_life.animate(DIR + name, 'Spin', slots=lambda slot, col, glow: col in movers, far=bool(p.get('far')), SpinRand=1.0, **kw)
    # the bulbs of the stalls and signs run in a chase; the lagoon's lights shimmer
    for name in ('SM_GameStall', 'SM_GameStall2', 'SM_GameStall3', 'SM_TicketGate', 'SM_HighStriker', 'SM_SignMidway', 'SM_SignPirate',
                 'SM_SignLagoon', 'SM_SignRocket', 'SM_SignFun', 'SM_SignRide'):
        n += ft_life.animate(DIR + name, 'Light', slots=lambda slot, col, glow: glow >= 1.9, LightAxis=(1, 0, 0), LightLen=260.0, LightRate=0.8,
                             LightDuty=0.6, LightMin=0.25)
    for name in ('SM_Lake', 'SM_Lake2', 'SM_Pond'):
        n += ft_life.animate(DIR + name, 'Light', slots=lambda slot, col, glow: 0.8 < glow < 1.0, LightHash=0.35, LightCell=300.0, LightRate=1.5,
                             LightMin=0.2)
    return n


def _ccw(pts):
    """The polygon counter-clockwise, whichever way it was written (an extrusion written clockwise comes out inside out)."""
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def _side(m, col, pts, y0, y1, glow=0.0):
    m.side(col, _ccw(pts), y0, y1, glow=glow)


def _foot(m, top):
    """The hidden post (see the module note)."""
    d = top * 0.85
    m.box(NAVY, (0, 0, -d / 2.0), (14, 14, d))


def _std(fn, slab=160.0):
    """A piece as the table stands it: fn's drawing, the foundation slab under its footprint, the hidden post."""
    def build(m):
        fn(m)
        env = ft_visual_art.envelope(m.meshes)
        if env is None:
            return
        lo, size = env
        if slab:
            m.box(DEEP, (lo[0] + size[0] / 2.0, lo[1] + size[1] / 2.0, -slab / 2.0 - 2.0), (size[0] * 0.97, size[1] * 0.97, slab))
        _foot(m, lo[2] + size[2])
    return build


def _awn(deg):
    """The roll that drops an awning's road-side (-Y) edge by deg."""
    return -ft_city._roll_sign() * deg


def _bulbs(m, a, b, n, cols=None, r=12, glow=G4):
    """n bulbs strung from a to b."""
    cols = cols or BULBS
    for k in range(n):
        t = (k + 0.5) / n
        m.sphere(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t), r, steps=(3, 5), glow=glow)


def _ring(m, col, centre, major, minor, rot=(0, 0, 90), steps=24, tube=4, glow=0.0):
    """A torus: rot (0, 0, 90) stands it up facing the road (its axis along Y); (0, 0, 0) lays it flat."""
    unreal.GeometryScript_Primitives.append_torus(m._mesh(col, glow), m.opts, m._t(centre, rot), unreal.GeometryScriptRevolveOptions(),
                                                 major, minor, steps, tube, unreal.GeometryScriptPrimitiveOriginMode.CENTER)


# the block capitals of the game's neon signs (ft_city.GLYPHS: strokes on a 4 x 6 grid) and the letters it lacks
_GLYPHS = dict(ft_city.GLYPHS)
_GLYPHS.update({
    'F': [(0, 0, 0, 6), (0, 6, 4, 6), (0, 3, 3, 3)],
    'W': [(0, 6, 1, 0), (1, 0, 2, 4), (2, 4, 3, 0), (3, 0, 4, 6)],
    'X': [(0, 0, 4, 6), (0, 6, 4, 0)],
    'Z': [(0, 6, 4, 6), (4, 6, 0, 0), (0, 0, 4, 0)],
    'J': [(4, 6, 4, 1), (4, 1, 3, 0), (3, 0, 1, 0), (1, 0, 0, 1)],
})


def _letters(m, word, y, z0, h, col, glow=G3, x=0.0):
    """Block capitals h high that face the road, centred on x, their feet at z0, standing 12 cm proud of the plane y.
    Drawn as ft_city.text draws them (one box a stroke), with the fuller alphabet above."""
    unit = h / 6.0
    t = unit * 0.8
    adv = 5.6 * unit
    width = len(word) * adv
    wall = ft_city.Wall(m, None, (x + width / 2.0, y, 0), (-1, 0, 0), (0, -1, 0), width)   # a wall facing -Y reads from +X to -X
    u = width / 2.0 - (len(word) * adv - 1.6 * unit) / 2.0
    for ch in word:
        for x0, y0, x1, y1 in _GLYPHS.get(ch, ()):
            du, dz = (x1 - x0) * unit, (y1 - y0) * unit
            roll = ft_city._roll_sign() * math.degrees(math.atan2(dz, du))
            m.box(col, wall.pt(u + (x0 + x1) / 2.0 * unit, 6.0, z0 + (y0 + y1) / 2.0 * unit), (12, math.hypot(du, dz) + t, t), rot=(0, wall.yaw, roll),
                  glow=glow)
        u += adv
    return width


def _stripes(m, x0, x1, y, z, depth, thick, cols, n, roll=0.0):
    """A striped canvas (an awning, a valance, a canopy): n strips side by side along X."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        m.box(cols[k % len(cols)], (x0 + w * (k + 0.5), y, z), (w, depth, thick), rot=(0, 0, roll))


def _pennant(m, at, h=260, col=MAG):
    m.cyl(GOLD, at, 5, h, steps=5)
    _side(m, col, [(at[0], at[2] + h), (at[0] + 110, at[2] + h - 35), (at[0], at[2] + h - 70)], at[1] - 3, at[1] + 3)


def _star(m, col, x, z, R, y0, y1, glow=G1):
    pts = []
    for k in range(10):
        a = math.pi / 2.0 + k * math.pi / 5.0
        rr = R if k % 2 == 0 else R * 0.44
        pts.append((x + math.cos(a) * rr, z + math.sin(a) * rr))
    _side(m, col, pts, y0, y1, glow=glow)


# ------------------------------------------------------------------------------------------ the verge (low, unlit)
def queue_rails(m):
    """Switchback queue rails under a striped canvas shade, a height-check post at the entrance: low and unlit."""
    L = 2200
    for j in range(3):
        y = -110 + j * 150
        m.box(GREY_L, (0, y, 104), (L, 8, 8))
        m.box(GREY_L, (0, y, 58), (L, 6, 6))
        for k in range(8):
            m.box(PURPLE, (-L / 2.0 + 40 + k * (L - 80) / 7.0, y, 54), (10, 10, 108))
    for sx in (-1, 1):
        m.box(WOOD_D, (560 + sx * 330, -110, 92), (12, 12, 184))
    _stripes(m, 230, 890, -110, 172, 14, 26, [PLUM, CREAM], 6)
    m.box(CREAM, (-980, -190, 90), (18, 18, 180))
    m.box(RED, (-980, -200, 160), (120, 8, 36))


def park_fence(m):
    """An ornamental fence with striped posts, gold knobs and clipped hedge balls behind it. Like everything on the
    verge it is under 2 m: nothing nearer the road than 24 m may be taller or lit."""
    L = 2000
    m.box(PURPLE, (0, 0, 20), (L, 30, 40))
    for k in range(11):
        x = -L / 2.0 + k * L / 10.0
        m.box(CREAM if k % 2 else PLUM, (x, 0, 78), (26, 26, 156))
        m.sphere(GOLD, (x, 0, 170), 20, steps=(3, 6))
    m.box(CREAM, (0, 0, 116), (L, 8, 8))
    m.box(CREAM, (0, 0, 60), (L, 8, 8))
    for k in range(5):
        m.sphere(PINE, (-800 + k * 400, 90, 80), 90, scale=(1, 1, 1.1), steps=(4, 7))


def plaza_seats(m):
    """Benches round a planter with a clipped cone tree, a barrel bin, a finger post: the verge's furniture."""
    m.cyl(GREY_L, (0, 40, 0), 150, 50, steps=10)
    m.cyl(PINE, (0, 40, 50), 110, 140, steps=8, top_r=12)
    for s in (-1, 1):
        m.box(WOOD, (s * 330, 20, 46), (300, 56, 10))
        m.box(WOOD, (s * 330, 46, 80), (300, 10, 60))
        for e in (-1, 1):
            m.box(PURPLE, (s * 330 + e * 130, 24, 22), (12, 50, 44))
    m.cyl(WOOD_D, (-560, 20, 0), 34, 90, steps=8)
    m.cyl(PLUM, (-560, 20, 90), 38, 12, steps=8)
    m.box(CREAM, (600, 30, 95), (14, 14, 190))
    for k, c in enumerate((PLUM, WOOD, TEAL)):
        m.box(c, (640 if k % 2 else 560, 30, 170 - k * 44), (150, 8, 34))


def cove_barrels(m, seed=4):
    """Barrels, crates and a rowing boat hauled up: Pirate Cove's clutter, low and unlit."""
    r = random.Random(seed)
    for k in range(5):
        x, y = -360 + k * 150 + r.uniform(-20, 20), r.uniform(-60, 60)
        m.cyl(WOOD, (x, y, 0), 44, 110, steps=8)
        m.cyl(WOOD_D, (x, y, 30), 46, 10, steps=8)
        m.cyl(WOOD_D, (x, y, 80), 46, 10, steps=8)
    for k in range(3):
        m.box(WOOD_D if k % 2 else WOOD, (430 + (k % 2) * 110, r.uniform(-40, 40), 50 + (k // 2) * 90), (100, 100, 100), rot=(0, r.uniform(-15, 15), 0))
    _side(m, WOOD, [(-200, 0), (180, 0), (260, 80), (-240, 80)], 150, 290)
    m.box(WOOD_D, (10, 220, 60), (300, 120, 12))
    m.cyl(WOOD_D, (-520, 60, 0), 10, 190, steps=5)
    _side(m, NAVY, [(-510, 186), (-330, 160), (-510, 134)], 57, 63)


# ------------------------------------------------------------------------------------------ the midway
def game_stall(m, col=MAG, trim=CREAM, sign=AMBER, seed=1):
    """A midway game stall: an open front, a counter, a striped valance, prizes along the back wall under a strip
    light, a sign board edged with bulbs on the roof."""
    r = random.Random(seed)
    W, D, H = 640, 420, 300
    m.box(col, (0, D / 2.0 - 15, H / 2.0), (W, 30, H))
    for s in (-1, 1):
        m.box(col, (s * (W / 2.0 - 15), 0, H / 2.0), (30, D, H))
    m.box(NAVY, (0, 0, 10), (W, D, 20))
    m.box(trim, (0, -D / 2.0 + 20, 95), (W - 60, 56, 14))
    m.box(col, (0, -D / 2.0 + 22, 45), (W - 70, 36, 90))
    _stripes(m, -W / 2.0, W / 2.0, -D / 2.0 - 8, H - 44, 12, 88, [trim, col], 8)
    m.box(trim, (0, -40, H + 12), (W + 60, D + 140, 24))
    prizes = [CYAN, AMBER, MINT, RED, LAV, WHITE]
    for j in range(2):
        for k in range(6):
            m.sphere(r.choice(prizes), (-W / 2.0 + 90 + k * (W - 180) / 5.0, D / 2.0 - 56, 136 + j * 84), 28, steps=(3, 6))
    m.box(BULB, (0, D / 2.0 - 36, 262), (W - 90, 6, 22), glow=G2)
    m.box(NAVY, (0, -D / 2.0 - 30, H + 96), (W - 110, 16, 136))
    m.box(sign, (0, -D / 2.0 - 40, H + 96), (W - 200, 6, 64), glow=G3)
    _bulbs(m, (-W / 2.0 + 70, -D / 2.0 - 42, H + 170), (W / 2.0 - 70, -D / 2.0 - 42, H + 170), 9)


def food_stand(m):
    """A food stand under a striped box of popcorn the size of a car."""
    W, D, H = 520, 400, 280
    m.box(CREAM, (0, 30, H / 2.0), (W, D - 60, H))
    m.box(NAVY, (0, -D / 2.0 + 58, 170), (W - 120, 6, 130))
    m.box(BULB, (0, -D / 2.0 + 54, 170), (W - 150, 4, 100), glow=G2)
    m.box(RED, (0, -D / 2.0 + 30, 96), (W - 40, 70, 12))
    _stripes(m, -W / 2.0 - 20, W / 2.0 + 20, -D / 2.0 - 30, H - 12, 190, 12, [RED, CREAM], 8, roll=_awn(14))
    m.box(RED, (0, 30, H + 10), (W + 40, D - 20, 20))
    bz = H + 20
    for k in range(5):
        m.box(RED if k % 2 else WHITE, (-120 + k * 60, 40, bz + 130), (60, 200, 260))
    r = random.Random(7)
    for k in range(12):
        m.sphere(BULB if k % 3 else WHITE, (r.uniform(-130, 130), 40 + r.uniform(-80, 80), bz + 270 + r.uniform(0, 90)), r.uniform(40, 62), steps=(3, 6),
                 glow=G1)
    _bulbs(m, (-W / 2.0, -D / 2.0 - 110, H - 40), (W / 2.0, -D / 2.0 - 110, H - 40), 8)


def candy_cart(m):
    """A candy-floss cart on two big wheels, a parasol over it."""
    m.box(MAG, (0, 0, 130), (300, 170, 120))
    m.box(CREAM, (0, 0, 196), (320, 190, 12))
    for s in (-1, 1):
        rod(m, NAVY, (s * 90, -92, 70), (s * 90, -104, 70), 70, steps=10)
        rod(m, GOLD, (s * 90, -104, 70), (s * 90, -110, 70), 18, steps=6)
    m.box(WOOD_D, (-190, 0, 130), (110, 12, 12))
    m.cyl(GOLD, (60, 0, 200), 6, 230, steps=5)
    m.cyl(MAG, (60, 0, 400), 190, 60, steps=10, top_r=10)
    m.cyl(CREAM, (60, 0, 394), 194, 8, steps=10)
    for k, c in enumerate((MAG, CYAN, LAV)):
        m.cyl(WOOD, (-90 + k * 60, 20, 200), 3, 70, steps=4)
        m.sphere(c, (-90 + k * 60, 20, 300), 44, scale=(1, 1, 1.2), steps=(4, 6), glow=G1)


def high_striker(m):
    """A test-your-strength tower: lights that climb the scale to a gold bell and a burst of rays."""
    H = 920
    m.box(NAVY, (0, 0, 20), (280, 220, 40))
    m.box(RED, (0, 40, H / 2.0 + 20), (84, 30, H))
    m.box(CREAM, (0, 22, H / 2.0 + 20), (44, 6, H - 90))
    cols = [MINT, MINT, MINT, AMBER, AMBER, AMBER, ORANGE, ORANGE, RED]
    for k in range(9):
        m.box(cols[k], (0, 16, 120 + k * 84), (60, 6, 28), glow=G2 if k < 6 else G3)
    m.sphere(GOLD, (0, 30, H + 86), 62, steps=(5, 8), glow=G1)
    for k in range(8):
        a = k * math.pi / 4.0
        tilt_box(m, AMBER, (math.cos(a) * 90, 30, H + 86 + math.sin(a) * 90), (math.cos(a) * 190, 30, H + 86 + math.sin(a) * 190), 16, 10, glow=G3)
    m.box(WOOD_D, (0, -70, 52), (96, 70, 26))
    rod(m, WOOD, (130, -90, 0), (160, -60, 170), 6, steps=5)
    m.box(GREY, (160, -60, 184), (44, 64, 44))


def bandstand(m):
    """An eight-sided bandstand: a domed striped roof on thin posts, a ring of bulbs under the eaves."""
    R = 420
    m.cyl(GREY_L, (0, 0, 0), R + 40, 70, steps=8)
    m.cyl(PLUM, (0, 0, 70), R, 14, steps=8)
    for k in range(8):
        a = k * math.pi / 4.0 + math.pi / 8.0
        m.cyl(CREAM, (math.cos(a) * (R - 30), math.sin(a) * (R - 30), 84), 9, 330, steps=6)
        m.sphere(BULB, (math.cos(a) * (R + 10), math.sin(a) * (R + 10), 406), 16, steps=(3, 5), glow=G4)
    m.cyl(MAG, (0, 0, 414), R + 40, 40, steps=8)
    m.cyl(CREAM, (0, 0, 454), R + 20, 210, steps=8, top_r=70)
    m.cyl(MAG, (0, 0, 664), 70, 60, steps=8, top_r=8)
    _pennant(m, (0, 0, 724), 200, AMBER)
    m.box(GREY_L, (0, -R - 70, 30), (220, 120, 40))


def pond(m):
    """A round ornamental pond in a stone rim: lit water, a jet, lily lights. Small enough to stand on a slope."""
    m.cyl(GREY_L, (0, 0, -300), 760, 330, steps=12)
    m.cyl(WATER, (0, 0, -290), 700, 335, steps=12, glow=GW)
    m.cyl(CYAN, (0, 0, 45), 12, 240, steps=5, top_r=2, glow=G2)
    for k in range(5):
        a = k * 2 * math.pi / 5.0 + 0.4
        m.sphere([MAG, AMBER, CYAN][k % 3], (math.cos(a) * 430, math.sin(a) * 430, 52), 22, steps=(3, 5), glow=G4)
        m.cyl(PINE, (math.cos(a) * 430, math.sin(a) * 430, 45), 60, 5, steps=6)
    for k in range(3):
        a = k * 2 * math.pi / 3.0 + 1.3
        m.sphere(PINE, (math.cos(a) * 800, math.sin(a) * 800, 60), 80, steps=(4, 6))


# ------------------------------------------------------------------------------------------ the front gate and its plaza
def ticket_gate(m):
    """The park's gate: two striped towers, the name in lights on the arch between them, ticket booths under it."""
    S = 1150
    for s in (-1, 1):
        m.box(CREAM, (s * S, 0, 450), (300, 300, 900))
        for k in range(4):
            m.box(MAG, (s * S, 0, 150 + k * 210), (306, 306, 70))
        m.cyl(MAG, (s * S, 0, 900), 230, 330, steps=8, top_r=10)
        m.cyl(CREAM, (s * S, 0, 892), 236, 10, steps=8)
        _pennant(m, (s * S, 0, 1230), 240, AMBER if s < 0 else CYAN)
        m.box(BULB, (s * S, -152, 560), (120, 4, 200), glow=G2)
    m.box(NAVY, (0, 0, 700), (2 * S - 300, 110, 260))
    m.box(MAG, (0, 0, 838), (2 * S - 300, 130, 16))
    m.box(MAG, (0, 0, 562), (2 * S - 300, 130, 16))
    _letters(m, 'COASTER COVE', -55, 612, 170, AMBER)
    _bulbs(m, (-S + 170, -62, 858), (S - 170, -62, 858), 11)
    _bulbs(m, (-S + 170, -62, 544), (S - 170, -62, 544), 11)
    for k in range(3):
        x = -560 + k * 560
        m.box(PLUM, (x, 20, 130), (200, 200, 260))
        m.box(BULB, (x, -82, 160), (130, 4, 90), glow=G2)
        m.cyl(MAG, (x, 20, 260), 150, 90, steps=8, top_r=10)
        for e in (-1, 1):
            m.box(GREY_L, (x + e * 190, -40, 50), (8, 120, 8))
            m.box(GREY_L, (x + e * 190, -40, 100), (8, 120, 8))


def fountain(m):
    """The plaza's fountain: three tiers, lit water, a ring of jets."""
    m.cyl(GREY_L, (0, 0, 0), 460, 70, steps=14)
    m.cyl(CYAN, (0, 0, 70), 420, 6, steps=14, glow=G1)
    m.cyl(GREY_L, (0, 0, 70), 90, 170, steps=8)
    m.cyl(GREY_L, (0, 0, 240), 230, 30, steps=12, top_r=250)
    m.cyl(CYAN, (0, 0, 270), 236, 6, steps=12, glow=G1)
    m.cyl(GREY_L, (0, 0, 270), 50, 150, steps=8)
    m.cyl(GREY_L, (0, 0, 420), 110, 24, steps=10, top_r=124)
    m.cyl(CYAN, (0, 0, 444), 10, 240, steps=5, top_r=2, glow=G2)
    for k in range(8):
        a = k * math.pi / 4.0
        tilt_box(m, CYAN, (math.cos(a) * 330, math.sin(a) * 330, 76), (math.cos(a) * 150, math.sin(a) * 150, 300), 10, 10, glow=G2)
        m.sphere(MAG if k % 2 else AMBER, (math.cos(a) * 480, math.sin(a) * 480, 60), 16, steps=(3, 5), glow=G4)


def mascot(m):
    """STARRY, the park's own mascot: a star in a ringmaster's coat and top hat, waving from a striped drum."""
    m.cyl(CREAM, (0, 0, 0), 190, 130, steps=12)
    for k in range(6):
        a = k * math.pi / 3.0
        m.box(MAG, (math.cos(a) * 186, math.sin(a) * 186, 65), (10, 90, 130), rot=(0, math.degrees(a), 0))
    for s in (-1, 1):
        m.cyl(NAVY, (s * 60, 0, 130), 34, 150, steps=6)
        m.box(WHITE, (s * 60, -20, 140), (80, 120, 30))
    m.sphere(MAG, (0, 0, 380), 150, scale=(1.0, 0.8, 1.15), steps=(5, 8))
    for k in range(3):
        m.sphere(GOLD, (0, -118, 320 + k * 60), 16, steps=(3, 5))
    m.box(WHITE, (0, -110, 500), (120, 30, 40))
    rod(m, MAG, (-130, 0, 440), (-250, -20, 330), 30, steps=6)
    rod(m, MAG, (130, 0, 450), (270, -20, 620), 30, steps=6)
    m.sphere(WHITE, (-262, -22, 318), 40, steps=(4, 6))
    m.sphere(WHITE, (282, -22, 640), 44, steps=(4, 6))
    _star(m, AMBER, 0, 720, 230, -45, 45)
    for s in (-1, 1):
        m.sphere(NAVY, (s * 50, -50, 740), 20, steps=(3, 5))
    m.box(NAVY, (0, -48, 672), (90, 8, 14))
    m.cyl(NAVY, (0, 0, 900), 96, 14, steps=10)
    m.cyl(NAVY, (0, 0, 910), 66, 130, steps=10)
    m.cyl(MAG, (0, 0, 922), 68, 24, steps=10)
    for a in (-1, 1):
        m.sphere(CYAN if a < 0 else AMBER, (a * 260, -200, 20), 26, steps=(3, 5), glow=G4)


def big_wheel(m, R=3000):
    """The observation wheel: legs, axle and loading deck, and the wheel itself (two lit rims, spokes, twelve
    gondolas) in mover colours: it turns about Y through the hub."""
    hub = R + 400
    for sy in (-1, 1):
        for sx in (-1, 1):
            tilt_box(m, CREAM, (sx * 1500, sy * 290, 0), (0, sy * 190, hub), 110, 90)
        tilt_box(m, CREAM, (-760, sy * 240, hub / 2.0), (760, sy * 240, hub / 2.0), 70, 60)
    rod(m, GOLD, (0, -300, hub), (0, 300, hub), 110, steps=10)
    m.box(PURPLE, (0, 0, 40), (3400, 900, 80))
    m.box(PLUM, (0, -420, 190), (900, 220, 300))
    m.box(BULB, (0, -532, 200), (600, 4, 140), glow=G2)
    _stripes(m, -500, 500, -470, 350, 320, 12, [MAG, CREAM], 8, roll=_awn(12))
    _bulbs(m, (-1650, -440, 100), (1650, -440, 100), 12)
    for y in (-120, 120):
        _ring(m, mv(MAG) if y < 0 else mv(CYAN), (0, y, hub), R, 34, steps=24, glow=G2)
    _ring(m, mv(CREAM), (0, 0, hub), R * 0.55, 22, steps=20)
    n = 12
    for k in range(n):
        a = 2 * math.pi * k / n + 0.2
        cx, cz = math.cos(a), math.sin(a)
        tilt_box(m, mv(CREAM), (cx * 130, 0, hub + cz * 130), (cx * R, 0, hub + cz * R), 26, 26)
        m.box(mv([AMBER, MAG, CYAN, CREAM][k % 4]), (cx * R, 0, hub + cz * R - 130), (190, 200, 170), glow=G1)
        m.box(mv(NAVY), (cx * R, -102, hub + cz * R - 110), (150, 4, 70))
    m.sphere(mv(AMBER), (0, -150, hub), 150, scale=(1, 0.5, 1), steps=(5, 8), glow=G3)


# ------------------------------------------------------------------------------------------ Pirate Cove
def pirate_ship(m, axle=1350, drop=1080):
    """The swinging ship: two A-frames, the axle between them, a boarding deck with rope rails; and the ship itself
    (hull, striped rail, benches, mast and flag, hung from the axle) in mover colours: it swings about Y through
    the axle. Drawn at the middle of its swing."""
    for sy in (-1, 1):
        for sx in (-1, 1):
            tilt_box(m, WOOD, (sx * 620, sy * 330, 0), (0, sy * 300, axle), 60, 60)
        tilt_box(m, WOOD_D, (-330, sy * 318, axle * 0.47), (330, sy * 318, axle * 0.47), 40, 40)
        m.sphere(GOLD, (0, sy * 300, axle + 40), 46, steps=(4, 6))
    rod(m, WOOD_D, (0, -330, axle), (0, 330, axle), 34, steps=8)
    m.box(WOOD_D, (0, -420, 100), (1500, 170, 20))
    for k in range(6):
        x = -700 + k * 280
        m.box(WOOD, (x, -500, 160), (14, 14, 120))
        m.box(WOOD_D, (x, -420, 50), (20, 150, 100))
    m.box(CREAM, (0, -500, 214), (1420, 8, 8))
    m.box(NAVY, (-900, -420, 250), (30, 30, 500))
    m.box(RED, (-900, -438, 440), (260, 8, 110))
    m.box(AMBER, (-900, -444, 440), (200, 4, 50), glow=G3)
    _bulbs(m, (-620, -334, 40), (0, -304, axle), 7, [BULB, AMBER])
    _bulbs(m, (620, -334, 40), (0, -304, axle), 7, [BULB, AMBER])
    z = axle - drop
    _side(m, mv(WOOD_D), [(-860, z + 250), (-700, z), (640, z), (900, z + 250), (1040, z + 330), (880, z + 330)], -170, 170)
    _side(m, mv(RED), [(-860, z + 250), (900, z + 250), (900, z + 300), (-860, z + 300)], -178, 178)
    m.box(mv(AMBER), (0, -180, z + 276), (1500, 4, 16), glow=G2)
    for k in range(6):
        m.box(mv(CREAM), (-600 + k * 240, 0, z + 270), (70, 300, 30))
    for sy in (-1, 1):
        rod(m, mv(WOOD), (0, sy * 150, axle), (-420, sy * 150, z + 280), 20, steps=6)
        rod(m, mv(WOOD), (0, sy * 150, axle), (420, sy * 150, z + 280), 20, steps=6)
    m.cyl(mv(WOOD), (40, 0, z + 250), 16, 560, steps=6)
    _side(m, mv(NAVY), [(60, z + 800), (330, z + 730), (60, z + 650)], -3, 3)
    m.sphere(mv(GOLD), (1050, 0, z + 350), 40, steps=(4, 6), glow=G1)
    _bulbs(m, (-820, -182, z + 320), (880, -182, z + 320), 10, [mv(BULB), mv(MAG)])


def skull_rock(m, seed=3):
    """The cove's landmark: a rock with two lit caves for eyes and a row of pale stones for teeth over a lit cave
    mouth, torches either side, a strip of falling water down its flank."""
    r = random.Random(seed)
    m.sphere(ROCK, (0, 120, 420), 760, scale=(1.15, 0.9, 0.95), steps=(6, 9))
    m.sphere(ROCK_D, (-520, 200, 200), 520, scale=(1.2, 1.0, 0.8), steps=(5, 8))
    m.sphere(ROCK_D, (560, 240, 240), 560, scale=(1.1, 1.0, 0.85), steps=(5, 8))
    for s in (-1, 1):
        rod(m, NAVY, (s * 270, -540, 620), (s * 270, -440, 620), 150, steps=10)
        rod(m, AMBER, (s * 270, -546, 620), (s * 270, -540, 620), 104, steps=10, glow=G2)
    _side(m, NAVY, [(-60, 400), (60, 400), (0, 520)], -610, -520)
    for k in range(7):
        m.box(CREAM, (-270 + k * 90, -560, 240 + (10 if k % 2 else 0)), (64, 140, 110))
    m.box(NAVY, (0, -540, 100), (560, 130, 190))
    m.box(AMBER, (0, -607, 96), (420, 4, 120), glow=G1)
    for s in (-1, 1):
        m.cyl(WOOD_D, (s * 620, -600, 0), 12, 300, steps=5)
        m.sphere(ORANGE, (s * 620, -600, 330), 40, scale=(1, 1, 1.4), steps=(4, 6), glow=G4)
    m.box(CYAN, (700, -322, 215), (120, 10, 420), rot=(0, -30, 0), glow=G2)
    for k in range(5):
        m.sphere(ROCK_D, (r.uniform(-900, 900), -620 + r.uniform(-60, 60), 40), r.uniform(70, 130), scale=(1.2, 1, 0.7), steps=(4, 6))


# ------------------------------------------------------------------------------------------ Rocket Land
def jet_ride(m, hub=900):
    """The rocket ride: a round deck, a fence, the tower, the operator's booth; and what turns on the tower (the hub,
    six arms at different heights with a rocket on each, a ringed planet on top) in mover colours: about Z."""
    m.cyl(PURPLE, (0, 0, 0), 760, 50, steps=16)
    m.cyl(CYAN, (0, 0, 50), 700, 6, steps=16, glow=G1)
    m.cyl(CREAM, (0, 0, 50), 120, hub - 110, steps=10)
    for k in range(4):
        m.cyl(MAG, (0, 0, 160 + k * 170), 126, 50, steps=10)
    for k in range(12):
        a = k * math.pi / 6.0
        m.box(GREY_L, (math.cos(a) * 760, math.sin(a) * 760, 100), (10, 10, 110))
    m.box(PLUM, (-520, -520, 150), (220, 220, 200))
    m.box(BULB, (-520, -632, 170), (140, 4, 90), glow=G2)
    m.box(MAG, (-520, -520, 258), (260, 260, 16))
    m.cyl(mv(CYAN), (0, 0, hub - 60), 170, 120, steps=10, glow=G1)
    m.sphere(mv(MAG), (0, 0, hub + 230), 170, steps=(5, 8), glow=G1)
    m.cyl(mv(AMBER), (0, 0, hub + 222), 270, 14, steps=14, glow=G2)
    cols = [RED, CYAN, AMBER, MINT, WHITE, LAV]
    for k in range(6):
        a = k * math.pi / 3.0
        cx, cy = math.cos(a), math.sin(a)
        z = 420 + (k % 3) * 170
        rod(m, mv(CREAM), (cx * 150, cy * 150, hub - 10), (cx * 640, cy * 640, z), 22, steps=6)
        tx, ty = -cy, cx
        tail = (cx * 640 - tx * 170, cy * 640 - ty * 170, z)
        nose = (cx * 640 + tx * 170, cy * 640 + ty * 170, z)
        rod(m, mv(cols[k]), tail, nose, 62, steps=8)
        rod(m, mv(CREAM), nose, (nose[0] + tx * 120, nose[1] + ty * 120, z), 44, steps=8)
        m.sphere(mv(RED), (nose[0] + tx * 140, nose[1] + ty * 140, z), 34, steps=(3, 6))
        m.box(mv(MAG), (tail[0], tail[1], z + 60), (120, 12, 110), rot=(0, math.degrees(a) + 90, 0))
        m.sphere(mv(ORANGE), (tail[0] - tx * 30, tail[1] - ty * 30, z), 40, steps=(3, 6), glow=G4)


def saucer(m):
    """A flying-saucer pavilion on three legs: a lit dome, a ring of lights round its rim, a ramp down to the ground."""
    for k in range(3):
        a = k * 2 * math.pi / 3.0 + 0.5
        rod(m, GREY_L, (math.cos(a) * 520, math.sin(a) * 520, 0), (math.cos(a) * 300, math.sin(a) * 300, 330), 26, steps=6)
        m.cyl(GREY, (math.cos(a) * 520, math.sin(a) * 520, 0), 70, 16, steps=8)
    m.cyl(GREY_L, (0, 0, 300), 300, 90, steps=14, top_r=640)
    m.cyl(LAV, (0, 0, 390), 640, 60, steps=14)
    m.cyl(GREY_L, (0, 0, 450), 640, 70, steps=14, top_r=320)
    m.sphere(CYAN, (0, 0, 510), 300, scale=(1, 1, 0.75), steps=(5, 10), glow=G1)
    for k in range(14):
        a = k * 2 * math.pi / 14.0
        m.sphere([AMBER, MAG, CYAN][k % 3], (math.cos(a) * 650, math.sin(a) * 650, 420), 24, steps=(3, 5), glow=G4)
    tilt_box(m, GREY, (0, -200, 300), (0, -760, 0), 200, 16)
    m.box(BULB, (0, -296, 240), (140, 4, 120), glow=G2)
    m.cyl(GREY, (0, 0, 730), 6, 220, steps=5)
    m.sphere(RED, (0, 0, 960), 26, steps=(3, 5), glow=G4)


def planet_posts(m):
    """Planets on poles: three globes at different heights, one ringed, small lights between."""
    for x, h, r_, c in ((-360, 520, 130, MAG), (0, 760, 170, AMBER), (380, 420, 110, CYAN)):
        m.cyl(GREY_L, (x, 0, 0), 12, h, steps=6)
        m.sphere(c, (x, 0, h + r_ * 0.8), r_, steps=(5, 8), glow=G1)
        m.cyl(GREY, (x, 0, 0), 60, 14, steps=8)
    m.cyl(CREAM, (0, 0, 760 + 126), 260, 12, steps=14)
    _bulbs(m, (-360, -10, 300), (380, -10, 300), 6, [BULB, CYAN], r=10)


# ------------------------------------------------------------------------------------------ the funhouse and the back lot
def funhouse(m):
    """A funhouse: a front in patchwork panels, a grinning doorway with teeth, round eye windows, a barrel to walk
    through, a turret gone askew, its name on a board across the roof."""
    W, D, H = 1000, 700, 820
    m.box(LAV, (0, 120, H / 2.0), (W, D, H))
    cols = [MINT, MAG, AMBER, CYAN, CREAM, PLUM]
    for j in range(3):
        for k in range(4):
            if (j + k) % 2 == 0:
                m.box(cols[(j * 2 + k) % 6], (-W / 2.0 + 125 + k * 250, -233, 140 + j * 260), (240, 8, 250))
    _side(m, MAG, [(-300, 0), (300, 0), (340, 240), (200, 400), (-200, 400), (-340, 240)], -252, -236)
    _side(m, NAVY, [(-230, 0), (230, 0), (260, 220), (150, 330), (-150, 330), (-260, 220)], -262, -252)
    for k in range(5):
        m.box(CREAM, (-200 + k * 100, -268, 296), (70, 10, 70))
        m.box(CREAM, (-200 + k * 100, -268, 34), (70, 10, 60))
    rod(m, PLUM, (0, -430, 170), (0, -272, 170), 150, steps=10)
    for k in range(4):
        rod(m, AMBER, (0, -431 + k * 40, 170), (0, -421 + k * 40, 170), 154, steps=10)
    for s in (-1, 1):
        rod(m, CREAM, (s * 300, -250, 590), (s * 300, -234, 590), 110, steps=10)
        rod(m, CYAN, (s * 300, -254, 590), (s * 300, -250, 590), 70, steps=10, glow=G2)
        rod(m, NAVY, (s * 300 + 20, -258, 590), (s * 300 + 20, -254, 590), 28, steps=8)
    m.box(MINT, (330, 220, H + 230), (260, 260, 460), rot=(-9, 0, 0))
    m.cyl(MAG, (296, 220, H + 440), 220, 320, steps=8, top_r=8, rot=(-9, 0, 0))
    m.box(NAVY, (-120, -150, H + 100), (720, 20, 170))
    _letters(m, 'FUNHOUSE', -160, H + 58, 90, AMBER, x=-120.0)
    _bulbs(m, (-470, -250, H + 20), (-470, -250, 60), 6, [BULB, MAG])
    _bulbs(m, (470, -250, H + 20), (470, -250, 60), 6, [BULB, CYAN])


def swing_ride(m, top=1050):
    """The chair swing: a deck, a fence, the striped column; and what turns on it (the crown, its ring of bulbs, ten
    chairs flung out on their chains) in mover colours: about Z."""
    m.cyl(PURPLE, (0, 0, 0), 620, 50, steps=14)
    m.cyl(CREAM, (0, 0, 50), 100, top - 50, steps=10)
    for k in range(5):
        m.cyl(MAG if k % 2 else TEAL, (0, 0, 150 + k * 180), 106, 60, steps=10)
    for k in range(10):
        a = k * math.pi / 5.0
        m.box(GREY_L, (math.cos(a) * 620, math.sin(a) * 620, 100), (10, 10, 110))
    m.cyl(mv(MAG), (0, 0, top), 560, 60, steps=14)
    m.cyl(mv(CREAM), (0, 0, top + 60), 560, 250, steps=14, top_r=60)
    m.cyl(mv(MAG), (0, 0, top + 310), 60, 70, steps=8, top_r=6)
    m.sphere(mv(AMBER), (0, 0, top + 400), 40, steps=(4, 6), glow=G4)
    for k in range(14):
        a = k * 2 * math.pi / 14.0
        m.sphere(mv([BULB, MAG, CYAN][k % 3]), (math.cos(a) * 566, math.sin(a) * 566, top + 30), 20, steps=(3, 5), glow=G4)
    cols = [RED, CYAN, AMBER, MINT, WHITE]
    for k in range(10):
        a = k * math.pi / 5.0
        cx, cy = math.cos(a), math.sin(a)
        seat = (cx * 860, cy * 860, top - 480 + (k % 2) * 60)
        rod(m, mv(GREY_L), (cx * 520, cy * 520, top), seat, 5, steps=4)
        m.box(mv(cols[k % 5]), seat, (70, 70, 50), rot=(0, math.degrees(a), 0))
        m.box(mv(NAVY), (seat[0], seat[1], seat[2] + 60), (40, 40, 70), rot=(0, math.degrees(a), 0))


def maintenance(m, seed=5):
    """The back lot: a ribbed shed with a roller door, spare coaster cars on a rack, a generator, cable drums, bins,
    a ladder left against the wall, one amber lamp over the door. No colour: this is where the park is not on show."""
    W, D, H = 1100, 700, 430
    m.box(SHED, (0, 150, H / 2.0), (W, D, H))
    for k in range(11):
        m.box(SHED_D, (-W / 2.0 + 50 + k * 100, -202, H / 2.0), (16, 8, H))
    _side(m, SHED_D, [(-W / 2.0 - 30, H), (W / 2.0 + 30, H), (0, H + 170)], -210, 510)
    m.box(NAVY, (-200, -206, 170), (420, 8, 340))
    for k in range(6):
        m.box(GREY, (-200, -212, 40 + k * 56), (410, 4, 8))
    m.box(GREY_L, (330, -206, 110), (110, 8, 220))
    m.sphere(AMBER, (-200, -230, 380), 22, steps=(3, 5), glow=G3)
    for k in range(3):
        z = 40 + k * 130
        m.box([RED, TEAL, AMBER][k], (-900, 60, z + 50), (260, 150, 90))
        m.box(NAVY, (-900, 60, z + 104), (200, 110, 30))
        m.box(GREY, (-900, 60, z - 4), (300, 180, 10))
    for s in (-1, 1):
        m.box(GREY, (-900 + s * 150, 60, 200), (10, 10, 420))
    m.box(GEN, (840, -40, 110), (360, 200, 220))
    m.cyl(GREY, (960, -40, 220), 18, 200, steps=6)
    m.box(AMBER, (840, -142, 160), (120, 4, 40), glow=G1)
    for k in range(3):
        rod(m, WOOD if k % 2 else WOOD_D, (520 + k * 120, -330, 60), (520 + k * 120, -250, 60), 60, steps=8)
    for k in range(3):
        m.cyl([GEN, SHED, PLUM][k], (-470 + k * 80, -300, 0), 34, 100, steps=7)
    tilt_box(m, GREY_L, (620, -250, 0), (600, -206, 380), 50, 6)
    for k in range(5):
        m.box(GREY_L, (-600 + k * 300, -420, 110), (8, 8, 220))
    for z in (60, 130, 200):
        m.box(GREY, (0, -420, z), (1220, 4, 6))


# ------------------------------------------------------------------------------------------ the lagoon under the coaster
def lake(m, seed=6, serpent=False, L=4400, W=7000):
    """One lake of the lagoon the coaster flies over, 44 m along the rails and 70 m across them: dark water with streaks
    of the park's lights on it inside a sand bank, an island with a lamp, pedal boats, a humped bridge strung with bulbs,
    jets, floating lanterns; the second kind has the lagoon's sea serpent arching out of it. Its z = 0 is the hollow's
    own ground; the water stands 1.3 m over that, and the bank and the water go down well below it (see THE LAGOON)."""
    r = random.Random(seed)
    zw = 130.0
    hx, hy, c = L / 2.0, W / 2.0, 700.0
    out = [(-hx + c, -hy), (hx - c, -hy), (hx, -hy + c), (hx, hy - c), (hx - c, hy), (-hx + c, hy), (-hx, hy - c), (-hx, -hy + c)]
    m.plan(WATER, out, -380, zw, glow=GW)
    for a, b in zip(out, out[1:] + out[:1]):
        tilt_box(m, SAND, (a[0], a[1], zw - 250), (b[0], b[1], zw - 250), 280, 580)
    for k in range(10):
        m.box([MAG, AMBER, CYAN][k % 3], (r.uniform(-1500, 1500), r.uniform(-2900, 2900), zw + 3), (r.uniform(260, 620), 26, 4), glow=G2)
    s = 1 if seed % 2 else -1
    ix, iy = r.uniform(-400, 400), s * 1500
    m.sphere(PINE, (ix, iy, zw), 420, scale=(1.3, 1.0, 0.3), steps=(4, 8))
    m.cyl(CREAM, (ix, iy, zw + 110), 8, 330, steps=5)
    m.sphere(BULB, (ix, iy, zw + 460), 34, steps=(4, 6), glow=G4)
    m.sphere(PINE, (ix - 160, iy + 90, zw + 170), 110, steps=(4, 6))
    m.sphere(PINE, (ix + 190, iy - 60, zw + 150), 90, steps=(4, 6))
    for x, y, col in ((-900, -s * 700, MAG), (500, -s * 1900, CYAN), (-300, s * 2700, AMBER)):
        m.box(WHITE, (x, y, zw + 26), (200, 110, 50))
        m.box(col, (x, y, zw + 126), (190, 120, 10))
        for sx in (-1, 1):
            m.box(WHITE, (x + sx * 85, y, zw + 76), (8, 8, 100))
    arch = [(-700, 30), (-520, 160), (-260, 250), (0, 280), (260, 250), (520, 160), (700, 30), (560, 30), (430, 110), (220, 180), (0, 202), (-220, 180),
            (-430, 110), (-560, 30)]
    by = -s * 2500
    _side(m, CREAM, [(x, z + zw - 40) for x, z in arch], by - 60, by + 60)
    for k in range(7):
        t = (k + 0.5) / 7.0
        m.sphere(BULB if k % 2 else MAG, (-620 + 1240 * t, by - 72, zw + 250 * math.sin(math.pi * t) + 50), 16, steps=(3, 5), glow=G4)
    for k in range(3):
        m.cyl(CYAN, (r.uniform(-1600, 1600), r.uniform(-600, 600), zw), 14, r.uniform(260, 420), steps=5, top_r=2, glow=G2)
    for k in range(8):
        m.sphere([BULB, MAG, CYAN, AMBER][k % 4], (r.uniform(-1900, 1900), r.uniform(-3100, 3100), zw + 14), 18, steps=(3, 5), glow=G4)
    if serpent:
        sy = s * 300
        for k, x in enumerate((-700, -60, 520)):
            _ring(m, MINT if k % 2 else TEAL, (x, sy, zw - 60), 300 - k * 30, 70 - k * 8, steps=12, tube=5)
        rod(m, TEAL, (900, sy, zw - 60), (1120, sy, zw + 260), 62, steps=7)
        m.sphere(TEAL, (1200, sy, zw + 300), 110, scale=(1.5, 1.0, 0.9), steps=(4, 7))
        for e in (-1, 1):
            m.sphere(AMBER, (1260, sy + e * 70, zw + 340), 22, steps=(3, 5), glow=G4)
        _side(m, MAG, [(1040, zw + 330), (1120, zw + 470), (1160, zw + 340)], sy - 6, sy + 6)


def path_lights(m, seed=8):
    """A stretch of lit footpath: pale slabs, lamps on tall poles with strings of bulbs between them, flower beds."""
    r = random.Random(seed)
    for k in range(4):
        m.box(SAND, (-1200 + k * 800, 60 * math.sin(k * 1.3), 20), (860, 260, 40), rot=(0, 8 * math.cos(k * 1.1), 0))
    poles = [(-1500, 190, 520), (-500, -190, 560), (500, 190, 520), (1500, -190, 560)]
    for x, y, h in poles:
        m.cyl(CREAM, (x, y, -200), 9, h + 200, steps=5)
        m.sphere(BULB, (x, y, h + 26), 30, steps=(4, 6), glow=G4)
    for a, b in zip(poles, poles[1:]):
        mid = ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, min(a[2], b[2]) - 90)
        _bulbs(m, a, mid, 4, [MAG, AMBER, CYAN], r=10)
        _bulbs(m, mid, b, 4, [CYAN, AMBER, MAG], r=10)
    for k in range(5):
        x, y = r.uniform(-1500, 1500), r.choice((-1, 1)) * r.uniform(260, 420)
        m.sphere(PINE, (x, y, 20), 120, scale=(1.4, 1.0, 0.35), steps=(4, 6))
        m.sphere([MAG, AMBER, LAV][k % 3], (x, y, 60), 60, scale=(1.5, 1.1, 0.4), steps=(3, 6))


def coaster_hill(m, L=6400, H=3000):
    """A length of the park's own coaster: hills and a loop on a forest of white supports, the rail picked out in
    pink, a train on the first climb, a station under an awning. Runs along X; its supports go on down below z = 0."""
    pts = []
    n = 24
    for k in range(n + 1):
        t = k / float(n)
        z = 500 + (H - 500) * (0.5 - 0.5 * math.cos(2 * math.pi * t * 1.5)) * (0.55 + 0.45 * math.cos(math.pi * t * 0.9))
        pts.append((-L / 2.0 + L * t, 0.0, max(z, 420.0)))
    for a, b in zip(pts, pts[1:]):
        tilt_box(m, WHITE, a, b, 130, 40)
        tilt_box(m, MAG, (a[0], a[1] - 70, a[2] + 30), (b[0], b[1] - 70, b[2] + 30), 12, 12, glow=G2)
    for p in pts[::2]:
        m.box(CREAM, (p[0], 0, (p[2] - 500) / 2.0), (34, 34, p[2] + 500))
        tilt_box(m, CREAM, (p[0] - 150, 110, -200), (p[0] + 150, -110, p[2] * 0.72), 18, 18)
    _ring(m, WHITE, (L * 0.36, 0, 1500), 1080, 44, steps=24)
    _ring(m, CYAN, (L * 0.36, -60, 1500), 1080, 12, steps=24, glow=G2)
    for k in range(4):
        m.box([RED, AMBER, CYAN, MAG][k], (pts[4][0] + k * 190, 0, pts[4][2] + 110 + k * 70), (170, 150, 110))
    m.box(PLUM, (-L / 2.0 + 500, -300, 150), (900, 400, 300))
    m.box(BULB, (-L / 2.0 + 500, -502, 170), (600, 4, 130), glow=G2)
    _stripes(m, -L / 2.0 + 50, -L / 2.0 + 950, -360, 330, 560, 12, [MAG, CREAM], 8, roll=_awn(10))


# ------------------------------------------------------------------------------------------ the names of the lands
def land_sign(m, word, col=AMBER, trim=MAG, emblem=None):
    """A land's name in lights: a board on two striped posts, bulbs along its top, the land's emblem over it."""
    h = 120.0
    w = len(word) * 5.6 * h / 6.0 + 160.0
    half = w / 2.0
    for s in (-1, 1):
        m.box(CREAM, (s * (half + 30), 0, 330), (60, 60, 660))
        for k in range(3):
            m.box(trim, (s * (half + 30), 0, 120 + k * 200), (64, 64, 60))
        m.sphere(GOLD, (s * (half + 30), 0, 696), 44, steps=(4, 6))
    m.box(NAVY, (0, 0, 560), (w, 30, 230))
    m.box(trim, (0, 0, 684), (w + 60, 44, 18))
    m.box(trim, (0, 0, 438), (w + 60, 44, 18))
    _letters(m, word, -15, 500, h, col)
    _bulbs(m, (-half, -26, 706), (half, -26, 706), max(6, int(w / 120)))
    if emblem:
        emblem(m, 0.0, 820.0)


def _em_star(m, x, z):
    _star(m, AMBER, x, z, 120, -14, 14, glow=G3)


def _em_skull(m, x, z):
    tilt_box(m, CREAM, (x - 130, 0, z - 80), (x + 130, 0, z + 80), 16, 26)
    tilt_box(m, CREAM, (x - 130, 0, z + 80), (x + 130, 0, z - 80), 16, 26)
    m.sphere(CREAM, (x, -10, z + 10), 80, scale=(1, 0.7, 1.05), steps=(4, 7))
    for e in (-1, 1):
        m.sphere(NAVY, (x + e * 30, -62, z + 22), 20, steps=(3, 5))


def _em_wave(m, x, z):
    for k in range(3):
        _ring(m, CYAN if k % 2 == 0 else TEAL, (x - 150 + k * 150, 0, z - 90), 90, 14, steps=12, tube=4, glow=G2)
    m.sphere(AMBER, (x, 0, z + 60), 36, steps=(4, 6), glow=G4)


def _em_rocket(m, x, z):
    rod(m, CREAM, (x - 110, 0, z - 60), (x + 70, 0, z + 60), 40, steps=8)
    rod(m, RED, (x + 70, 0, z + 60), (x + 130, 0, z + 100), 26, steps=6)
    m.sphere(ORANGE, (x - 135, 0, z - 78), 36, steps=(3, 6), glow=G4)
    tilt_box(m, MAG, (x - 120, 0, z - 10), (x - 60, 0, z - 110), 12, 70)


def _em_hat(m, x, z):
    m.cyl(NAVY, (x, 0, z - 80), 120, 16, steps=10)
    m.cyl(NAVY, (x, 0, z - 70), 80, 160, steps=10)
    m.cyl(MAG, (x, 0, z - 54), 82, 30, steps=10)
    _star(m, AMBER, x, z + 20, 44, -90, -80, glow=G3)


def _em_nose(m, x, z):
    m.cyl(CREAM, (x, 0, z - 90), 150, 26, steps=12)
    m.cyl(CYAN, (x, 0, z - 64), 110, 20, steps=12)
    m.sphere(RED, (x, 0, z + 40), 86, steps=(5, 8), glow=G2)


# ------------------------------------------------------------------------------------------ the horizon (built from z = 0 up)
def far_coaster(m, L=26000, H=8200):
    """The big coaster on the skyline: a quarter of a kilometre of hills and a loop."""
    pts = []
    n = 28
    for k in range(n + 1):
        t = k / float(n)
        z = 1400 + (H - 1400) * (0.5 - 0.5 * math.cos(2 * math.pi * t * 2.5)) * (1.0 - 0.55 * t)
        pts.append((-L / 2.0 + L * t, 0.0, z))
    for a, b in zip(pts, pts[1:]):
        tilt_box(m, WHITE, a, b, 420, 150)
        tilt_box(m, MAG, (a[0], -240, a[2] + 110), (b[0], -240, b[2] + 110), 60, 60, glow=G2)
    for p in pts[::2]:
        m.box(CREAM, (p[0], 0, p[2] / 2.0), (130, 130, p[2]))
        tilt_box(m, CREAM, (p[0] - 500, 300, 0), (p[0] + 500, -300, p[2] * 0.7), 70, 70)
    _ring(m, WHITE, (L * 0.3, 0, 3600), 2900, 150, steps=24)
    _ring(m, CYAN, (L * 0.3, -200, 3600), 2900, 50, steps=24, glow=G2)
    for k in range(7):
        m.sphere(BULB if k % 2 else AMBER, (-L / 2.0 + (k + 0.5) * L / 7.0, -320, 420), 130, steps=(3, 6), glow=G4)


def far_wheel(m, R=5600):
    """A giant wheel on the skyline, a hundred and twenty metres of it, lit round both rims."""
    hub = R + 700
    for sy in (-1, 1):
        for sx in (-1, 1):
            tilt_box(m, CREAM, (sx * 2800, sy * 600, 0), (0, sy * 320, hub), 320, 260)
        _ring(m, MAG if sy < 0 else CYAN, (0, sy * 260, hub), R, 110, steps=28, glow=G2)
    _ring(m, CREAM, (0, 0, hub), R * 0.5, 70, steps=20)
    n = 16
    for k in range(n):
        a = 2 * math.pi * k / n
        cx, cz = math.cos(a), math.sin(a)
        tilt_box(m, CREAM, (cx * 300, 0, hub + cz * 300), (cx * R, 0, hub + cz * R), 80, 80)
        m.box([AMBER, MAG, CYAN, CREAM][k % 4], (cx * R, 0, hub + cz * R - 300), (520, 620, 440), glow=G2)
    m.sphere(AMBER, (0, -420, hub), 420, scale=(1, 0.4, 1), steps=(5, 8), glow=G3)
    m.box(PURPLE, (0, 0, 200), (7000, 2000, 400))
    _bulbs(m, (-3300, -1020, 460), (3300, -1020, 460), 10, [BULB, MAG, CYAN], r=110)


def far_chute(m, H=15000):
    """A parachute tower: a lattice mast a hundred and fifty metres tall, a crown of arms, canopies at every height."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, CREAM, (sx * 1500, sy * 1500, 0), (sx * 300, sy * 300, H), 260, 260)
    for k in range(1, 8):
        z = k * H / 8.0
        w = 1500 - (1200.0 * k / 8.0)
        for s in (-1, 1):
            m.box(CREAM, (0, s * w, z), (2 * w, 110, 110))
            m.box(CREAM, (s * w, 0, z), (110, 2 * w, 110))
        m.box(MAG if k % 2 else CYAN, (0, -w - 80, z), (2 * w, 40, 90), glow=G2)
    m.cyl(MAG, (0, 0, H), 2700, 260, steps=12)
    m.cyl(CREAM, (0, 0, H + 260), 2700, 900, steps=12, top_r=200)
    m.sphere(RED, (0, 0, H + 1400), 220, steps=(4, 6), glow=G4)
    cols = [AMBER, MAG, CREAM, CYAN, RED, MINT]
    for k in range(6):
        a = k * math.pi / 3.0 + 0.3
        x, y = math.cos(a) * 2500, math.sin(a) * 2500
        z = H * (0.35 + 0.1 * ((k * 3) % 5))
        rod(m, GREY_L, (x, y, z + 900), (x, y, H), 14, steps=4)
        m.cyl(cols[k], (x, y, z + 300), 800, 600, steps=10, top_r=80)
        m.box(NAVY, (x, y, z - 150), (220, 220, 200))
        rod(m, GREY_L, (x, y, z - 60), (x - 600, y, z + 320), 10, steps=4)
        rod(m, GREY_L, (x, y, z - 60), (x + 600, y, z + 320), 10, steps=4)


def far_bigtops(m):
    """Three big tops on the skyline, striped, each with its pennant, bulbs strung from peak to peak."""
    peaks = []
    for x, R, h, c in ((-4200, 2600, 1700, RED), (300, 3400, 2300, MAG), (4700, 2300, 1500, TEAL)):
        n = 12
        for k in range(n):
            a = k * 2 * math.pi / n
            m.box(c if k % 2 else WHITE, (x + math.cos(a) * R, math.sin(a) * R, h / 2.0), (120, 2 * math.pi * R / n + 20, h), rot=(0, math.degrees(a), 0))
        m.cyl(WHITE, (x, 0, h), R + 60, R * 0.95, steps=n, top_r=60)
        for k in range(0, n, 2):
            a = k * 2 * math.pi / n
            tilt_box(m, c, (x + math.cos(a) * (R + 40), math.sin(a) * (R + 40), h + 20), (x, 0, h + R * 0.95), 260, 40)
        top = h + R * 0.95
        m.cyl(GOLD, (x, 0, top), 30, 900, steps=5)
        _side(m, MAG if c != MAG else AMBER, [(x, top + 900), (x + 520, top + 740), (x, top + 580)], -12, 12, glow=G1)
        peaks.append((x, -200.0, top))
        m.box(NAVY, (x, -R - 30, 500), (900, 80, 1000))
        m.box(BULB, (x, -R - 80, 520), (640, 20, 700), glow=G2)
    for a, b in zip(peaks, peaks[1:]):
        mid = ((a[0] + b[0]) / 2.0, -300.0, min(a[2], b[2]) - 1500)
        _bulbs(m, a, mid, 6, [BULB, MAG, CYAN], r=90)
        _bulbs(m, mid, b, 6, [CYAN, AMBER, BULB], r=90)


def far_rocket(m, H=13000):
    """Rocket Land's skyline: a striped rocket a hundred and thirty metres tall on its pad, a gantry beside it."""
    m.cyl(GREY, (0, 0, 0), 3000, 300, steps=12)
    m.cyl(CREAM, (0, 0, 900), 1000, H * 0.62, steps=14)
    m.cyl(MAG, (0, 0, 900 + H * 0.2), 1010, H * 0.07, steps=14)
    m.cyl(MAG, (0, 0, 900 + H * 0.45), 1010, H * 0.05, steps=14)
    m.cyl(CREAM, (0, 0, 900 + H * 0.62), 1000, H * 0.26, steps=14, top_r=40)
    m.cyl(RED, (0, 0, 900 + H * 0.8), 430, H * 0.09, steps=10, top_r=20)
    rod(m, CYAN, (0, -1000, 900 + H * 0.5), (0, -1030, 900 + H * 0.5), 360, steps=12, glow=G2)
    for k in range(4):
        a = k * math.pi / 2.0 + math.pi / 4.0
        cx, cy = math.cos(a), math.sin(a)
        tilt_box(m, MAG, (cx * 2100, cy * 2100, 300), (cx * 900, cy * 900, 3600), 120, 900)
    m.cyl(ORANGE, (0, 0, 300), 700, 600, steps=10, top_r=420, glow=G3)
    gx = 2900
    m.box(GREY_L, (gx, 0, H * 0.36), (700, 700, H * 0.72))
    for k in range(9):
        z = 500 + k * H * 0.075
        m.box(NAVY, (gx, -356, z), (560, 10, H * 0.05))
        m.box(AMBER if k % 3 else CYAN, (gx, -366, z + H * 0.03), (300, 10, 90), glow=G2)
    for z in (H * 0.3, H * 0.5, H * 0.66):
        m.box(GREY_L, (gx / 2.0 + 400, 0, 900 + z), (gx - 900, 220, 180))
    m.sphere(RED, (gx, 0, H * 0.72 + 200), 200, steps=(4, 6), glow=G4)
    for s in (-1, 1):
        m.sphere(BULB, (s * 2600, -2200, 420), 190, steps=(4, 6), glow=G4)


def far_fort(m):
    """Pirate Cove's skyline: a fort on its rock, round towers under pointed roofs, the masts of a ship behind it."""
    stone, stone_l = '#5a5478', '#6a6490'
    m.sphere(ROCK, (0, 600, 600), 5200, scale=(1.3, 0.8, 0.5), steps=(6, 10))
    m.box(stone, (0, 0, 3300), (6200, 3200, 2600))
    for k in range(9):
        m.box(stone, (-2800 + k * 700, -1500, 4800), (380, 260, 420))
    for k in range(6):
        m.box(AMBER, (-2300 + k * 920, -1612, 3400 + (k % 2) * 500), (300, 20, 460), glow=G2)
    for x, h, r_ in ((-3600, 6400, 1000), (3400, 7600, 1150), (200, 5200, 800)):
        m.cyl(stone_l, (x, -400, 1500), r_, h - 1500, steps=10)
        m.cyl(MAG, (x, -400, h), r_ + 160, r_ * 1.9, steps=10, top_r=30)
        m.box(AMBER, (x, -400 - r_ - 8, h - 700), (260, 20, 420), glow=G2)
        m.cyl(GOLD, (x, -400, h + r_ * 1.9), 26, 800, steps=5)
        _side(m, RED, [(x, h + r_ * 1.9 + 800), (x + 620, h + r_ * 1.9 + 620), (x, h + r_ * 1.9 + 440)], -412, -388)
    for x, h in ((-1700, 9800), (-300, 11200), (1100, 9200)):
        m.cyl(WOOD_D, (x, 2400, 2000), 90, h - 2000, steps=6)
        for j in range(3):
            z = h - 900 - j * 2100
            m.box(CREAM, (x, 2380, z), (2400 - j * 100, 40, 1500))
            m.box(WOOD_D, (x, 2400, z + 800), (2700 - j * 100, 60, 60))
    for s in (-1, 1):
        m.sphere(ORANGE, (s * 3200, -1700, 2300), 200, scale=(1, 1, 1.4), steps=(4, 6), glow=G4)


_NEAR = [
    ('SM_QueueRails', queue_rails), ('SM_ParkFence', park_fence), ('SM_PlazaSeats', plaza_seats), ('SM_CoveBarrels', cove_barrels),
    ('SM_GameStall', game_stall), ('SM_GameStall2', lambda m: game_stall(m, TEAL, CREAM, MAG, seed=2)),
    ('SM_GameStall3', lambda m: game_stall(m, PLUM, AMBER, CYAN, seed=3)), ('SM_FoodStand', food_stand), ('SM_CandyCart', candy_cart),
    ('SM_HighStriker', high_striker), ('SM_Bandstand', bandstand),
    ('SM_TicketGate', ticket_gate), ('SM_Fountain', fountain), ('SM_Mascot', mascot), ('SM_BigWheel', big_wheel),
    ('SM_PirateShip', pirate_ship), ('SM_SkullRock', skull_rock),
    ('SM_JetRide', jet_ride), ('SM_Saucer', saucer), ('SM_PlanetPosts', planet_posts),
    ('SM_Funhouse', funhouse), ('SM_SwingRide', swing_ride), ('SM_Maintenance', maintenance),
    ('SM_PathLights', path_lights), ('SM_CoasterHill', coaster_hill),
    ('SM_SignMidway', lambda m: land_sign(m, 'THE MIDWAY', AMBER, MAG, _em_star)),
    ('SM_SignPirate', lambda m: land_sign(m, 'PIRATE COVE', CREAM, RED, _em_skull)),
    ('SM_SignLagoon', lambda m: land_sign(m, 'THE LAGOON', CYAN, TEAL, _em_wave)),
    ('SM_SignRocket', lambda m: land_sign(m, 'ROCKET LAND', CYAN, MAG, _em_rocket)),
    ('SM_SignFun', lambda m: land_sign(m, 'FUNHOUSE', MAG, TEAL, _em_hat)),
    ('SM_SignRide', lambda m: land_sign(m, 'THE LAST RIDE', AMBER, RED, _em_nose)),
]
_WATER = [('SM_Pond', pond), ('SM_Lake', lake), ('SM_Lake2', lambda m: lake(m, seed=13, serpent=True))]
_FAR = [('SM_FarCoaster', far_coaster), ('SM_FarWheel', far_wheel), ('SM_FarChute', far_chute), ('SM_FarBigTops', far_bigtops),
        ('SM_FarRocket', far_rocket), ('SM_FarFort', far_fort)]
JOBS = [(n, _std(f)) for n, f in _NEAR] + [(n, _std(f, slab=0.0)) for n, f in _WATER + _FAR]
