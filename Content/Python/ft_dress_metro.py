"""FULL THROTTLE (Unreal) dressing kit, level 7: City Center (Midtown at midnight). Whole blocks round the level's own
towers: a street wall of shop fronts, a picture house, a bank, a parking garage and its ramps, alleys with fire
escapes, service yards; the sidewalk's furniture and parked cars; mid-rise blocks with their rooftops, an elevated
railway, a plaza's monument and clock tower; distant towers, a radio mast and a bridge on the skyline. And the
Syndicate's territory, which has a livery of its own. Placed by Source/FullThrottle/FTDressL7.inl.
See ft_dress.py for how a piece is drawn (front faces -Y, base on z = 0, long axis along X, cm).

    import ft_dress; ft_dress.run(['metro'])

THE STREET WALL. The level's own towers stand every 45 m with their middles 33-42 m from the centre line, so their
fronts are 23-32 m out. The wall pieces here (SM_Shops*, SM_Marquee, SM_BankFront, SM_PentEntry, SM_GarageRamp,
SM_ServiceYard, SM_SynGate) are 32 m long and 7-9 m deep and are stood with their fronts 25 m out: each becomes the
podium of the tower behind it, and the 13 m left between two of them is an alley (SM_Alley fits it).

THE SYNDICATE'S LIVERY (ours: the Chairman's tower on the level's horizon already carries gold chevrons and a red
beacon): black, a gold DOUBLE CHEVRON, red warning lights. It is on the banners (SM_SynBanner), the barrier rows
(SM_SynBarriers), the black cars (SM_SynCars), the checkpoints (SM_Checkpoint), the searchlight towers (SM_SearchTower),
the armoured gates (SM_SynGate), the headquarters blocks (SM_SynHQ) and the far towers (SM_FarSyndicate), and on
nothing else.

Every name on a sign here is invented (CAFE, DELI, NOVA, ATLAS, TRUST ...); nothing is a real brand's.

THE SIGHT LINE. Nothing lit or over 2 m tall stands nearer the road than 24 m (the track itself moves such a piece out
where a bend brings it nearer), so everything made for the sidewalk here is under 2 m and unlit: it has to read by
its colour under the street's own lamps (yellow cabs, gold-and-black barriers).
"""
import math
import random
import unreal
import ft_art
import ft_city
import ft_visual_art
from ft_desert import rod, tilt_box

NAVY, INDIGO, SLATE, SLATE_L, PLUM = '#2b2350', '#26243a', '#33304e', '#3a3456', '#4a3a78'
BRICK, BRICK_D, TAN, STONE, STONE_L, CONC, CONC_D = '#6a3f44', '#54343c', '#8a6f5c', '#706e80', '#9a98a8', '#5a5870', '#4a4860'
CYAN, WARM, WARM_L, AMBER, MAG, ORANGE, RED = '#2ee6ff', '#ffe28a', '#fff1c0', '#ffd23f', '#ff4fb8', '#ff6a3d', '#ff2d3c'
CREAM, WHITE, TEAL, GREEN_D, PINE, BLUE, WOOD, WOOD_D = '#fff1d6', '#fff6ea', '#2fc6c0', '#24403a', '#2f6b52', '#3a5fa8', '#9a6a44', '#6a4c36'
GLASS, GLASS_D, TYRE, SILVER, GOLD, BLACK, GREY_D, SAND_D = '#233a63', '#1c1f4a', '#26232e', '#c9c4d6', '#ffc93c', '#1c1a26', '#3f3d50', '#b89a6a'
G1, G2, G3, G4 = 0.35, 0.9, 1.4, 2.0          # glow: soft, a lit window, a sign, a bulb
DIR = '/Game/Maps/Metro/Dress/'


def animate():
    """What moves here (ft_life.py, the living-world pass; run it again after any export of these two meshes: the export
    puts the plain palette back). The picture house's bulbs run in a chase along its canopy, and the radio mast's beacons
    wink slowly, far off and high up. Nothing else: the Syndicate's red lights and the bank's ticker stand at the height
    the Chairman's shots fly in the finale, and stay steady so that his fire is what flickers there."""
    import ft_life
    n = ft_life.animate(DIR + 'SM_Marquee', 'Light', slots=lambda slot, col, glow: glow >= 1.9, LightAxis=(1, 0, 0), LightLen=260.0,
                        LightRate=0.8, LightDuty=0.6, LightMin=0.25)
    n += ft_life.animate(DIR + 'SM_FarRadio', 'Light', slots=lambda slot, col, glow: glow >= 1.9, LightRate=0.35, LightDuty=0.5, LightMin=0.15)
    return n


def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def _side(m, col, pts, y0, y1, glow=0.0):
    m.side(col, _ccw(pts), y0, y1, glow=glow)


def _std(fn, foot=False, slab=0.0):
    """A piece as the table stands it: fn's drawing; for a street-wall piece a foundation slab under its footprint (it
    shows only where the street climbs and one end of the piece would stand in the air); for an open piece the hidden
    post (see the module note)."""
    def build(m):
        fn(m)
        env = ft_visual_art.envelope(m.meshes)
        if env is None:
            return
        lo, size = env
        if slab:
            m.box(INDIGO, (lo[0] + size[0] / 2.0, lo[1] + size[1] / 2.0, -slab / 2.0 - 2.0), (size[0] * 0.98, size[1] * 0.98, slab))
        # foot: the hidden post is no longer drawn. ft_dress.export stopped running the finish pass's refine() on dressing
        # pieces (15 Nov), so there is no service panel to hide; the argument stays to mark the open pieces.
    return build


def _awn(deg):
    """The roll that drops an awning's road-side (-Y) edge by deg."""
    return -ft_city._roll_sign() * deg


def _bulbs(m, a, b, n, cols=(WARM, MAG, CYAN, AMBER), r=12, glow=G4):
    for k in range(n):
        t = (k + 0.5) / n
        m.sphere(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t), r, steps=(3, 5), glow=glow)


def _ring(m, col, centre, major, minor, rot=(0, 0, 90), steps=24, tube=4, glow=0.0):
    """A torus: rot (0, 0, 90) stands it up facing the road; (0, 0, 0) lays it flat."""
    unreal.GeometryScript_Primitives.append_torus(m._mesh(col, glow), m.opts, m._t(centre, rot), unreal.GeometryScriptRevolveOptions(),
                                                 major, minor, steps, tube, unreal.GeometryScriptPrimitiveOriginMode.CENTER)


_GLYPHS = dict(ft_city.GLYPHS)
_GLYPHS.update({
    'F': [(0, 0, 0, 6), (0, 6, 4, 6), (0, 3, 3, 3)],
    'W': [(0, 6, 1, 0), (1, 0, 2, 4), (2, 4, 3, 0), (3, 0, 4, 6)],
    'X': [(0, 0, 4, 6), (0, 6, 4, 0)],
    'Z': [(0, 6, 4, 6), (4, 6, 0, 0), (0, 0, 4, 0)],
})


def _letters(m, word, y, z0, h, col, glow=G3, x=0.0):
    """Block capitals h high that face the road, centred on x, their feet at z0, 12 cm proud of the plane y (drawn as
    ft_city.text draws them, one box a stroke, with the letters it lacks added above)."""
    unit = h / 6.0
    t = unit * 0.8
    adv = 5.6 * unit
    width = len(word) * adv
    wall = ft_city.Wall(m, None, (x + width / 2.0, y, 0), (-1, 0, 0), (0, -1, 0), width)
    u = width / 2.0 - (len(word) * adv - 1.6 * unit) / 2.0
    for ch in word:
        for x0, y0, x1, y1 in _GLYPHS.get(ch, ()):
            du, dz = (x1 - x0) * unit, (y1 - y0) * unit
            roll = ft_city._roll_sign() * math.degrees(math.atan2(dz, du))
            m.box(col, wall.pt(u + (x0 + x1) / 2.0 * unit, 6.0, z0 + (y0 + y1) / 2.0 * unit), (12, math.hypot(du, dz) + t, t), rot=(0, wall.yaw, roll),
                  glow=glow)
        u += adv


def _stack(m, word, x, y, z_top, h, col, glow=G3):
    """The same capitals one under the other (a blade sign), the first with its top at z_top."""
    for k, ch in enumerate(word):
        _letters(m, ch, y, z_top - (k + 1) * h * 1.3, h, col, glow, x)


def _stripes(m, x0, x1, y, z, depth, thick, cols, n, roll=0.0):
    w = (x1 - x0) / float(n)
    for k in range(n):
        m.box(cols[k % len(cols)], (x0 + w * (k + 0.5), y, z), (w, depth, thick), rot=(0, 0, roll))


def _chevrons(m, x, y, z, w, col=GOLD, glow=G2, n=2, t=None):
    """The Syndicate's mark on a plane facing the road: n chevrons one over the other, w wide, their points up."""
    t = t or w * 0.12
    for k in range(n):
        zz = z + k * w * 0.34
        tilt_box(m, col, (x - w / 2.0, y, zz), (x, y, zz + w * 0.3), 10, t, glow=glow)
        tilt_box(m, col, (x + w / 2.0, y, zz), (x, y, zz + w * 0.3), 10, t, glow=glow)


def _car(m, x, y, col, kind='sedan', stripe=None):
    """A parked car, nose along +X: chunky and unlit."""
    L, W = (520.0, 210.0) if kind == 'van' else (460.0, 190.0)
    m.box(col, (x, y, 62), (L, W, 64))
    if kind == 'van':
        m.box(col, (x - 30, y, 142), (L - 100, W - 10, 96))
        m.box(GLASS, (x + L / 2.0 - 62, y, 136), (50, W - 30, 60))
    else:
        m.box(GLASS, (x - 20, y, 122), (L * 0.52, W - 24, 58))
        m.box(col, (x - 20, y, 154), (L * 0.46, W - 30, 8))
    if stripe:
        m.box(stripe, (x, y - W / 2.0 - 1, 70), (L - 40, 3, 12))
    for sx in (-1, 1):
        rod(m, TYRE, (x + sx * L * 0.3, y - W / 2.0 - 4, 34), (x + sx * L * 0.3, y + W / 2.0 + 4, 34), 34, steps=8)
    if kind == 'taxi':
        m.box(CREAM, (x - 20, y, 168), (60, 30, 20))


def _rows(m, x0, x1, y, z0, floors, pitch, col, alt=None, h=130, seed=0, p=0.82):
    """Lit window bands up a wall that faces the road."""
    r = random.Random(seed)
    for f in range(floors):
        if r.random() < p:
            m.box(col if (alt is None or r.random() < 0.7) else alt, ((x0 + x1) / 2.0, y - 2, z0 + f * pitch), (x1 - x0, 4, h), glow=G2)


def _ends(m, x, y0, y1, z0, floors, pitch, col, h=130, seed=0, p=0.7):
    """The same on an end wall (x is the wall's plane)."""
    r = random.Random(seed)
    for f in range(floors):
        if r.random() < p:
            m.box(col, (x, (y0 + y1) / 2.0, z0 + f * pitch), (4, y1 - y0, h), glow=G2)


# ------------------------------------------------------------------------------------------ the street wall
def shops(m, names=('CAFE', 'TOYS', 'DELI'), walls=(BRICK, TAN, BRICK_D), awns=((RED, CREAM), (TEAL, CREAM), (AMBER, NAVY)),
          signs=(AMBER, CYAN, MAG), tall=(1,), seed=1):
    """A row of three shops, 32 m long: lit windows under striped awnings, a name over each, a flat over one of them,
    the roof's clutter."""
    r = random.Random(seed)
    L, D = 3200.0, 700.0
    w = L / len(names)
    for k, name in enumerate(names):
        x0 = -L / 2.0 + k * w
        xc = x0 + w / 2.0
        H = 860.0 if k in tall else 520.0
        m.box(walls[k % len(walls)], (xc, 0, H / 2.0), (w - 6, D, H))
        m.box(STONE_L, (xc, 0, H + 14), (w + 8, D + 24, 28))
        m.box(WARM if k % 2 == 0 else WARM_L, (xc - 80, -D / 2.0 - 2, 170), (w - 330, 4, 230), glow=G2)
        m.box(NAVY, (x0 + w - 120, -D / 2.0 - 2, 132), (120, 4, 256))
        m.box(CYAN if k % 2 else WARM_L, (x0 + w - 120, -D / 2.0 - 5, 200), (76, 3, 80), glow=G1)
        _stripes(m, x0 + 40, x0 + w - 40, -D / 2.0 - 46, 318, 120, 8, list(awns[k % len(awns)]), 6, roll=_awn(18))
        m.box(NAVY, (xc, -D / 2.0 - 6, 424), (w - 80, 12, 104))
        _letters(m, name, -D / 2.0 - 12, 388, 72, signs[k % len(signs)], x=xc)
        if H > 600:
            for j in range(3):
                m.box(CYAN if (j + k) % 2 else WARM, (x0 + 150 + j * (w - 300) / 2.0, -D / 2.0 - 2, 690), (170, 4, 180), glow=G2)
            m.box(NAVY, (x0 + 70, -D / 2.0 - 40, 900), (110, 70, 760))          # a blade sign of lit bars, well over the roof
            for j in range(7):
                m.box(signs[(k + j) % len(signs)], (x0 + 70, -D / 2.0 - 78, 590 + j * 100), (80, 6, 60), glow=G3)
            m.sphere(signs[k % len(signs)], (x0 + 70, -D / 2.0 - 40, 1310), 40, steps=(4, 6), glow=G4)
            m.cyl(WOOD, (xc + 200, 60, H + 150), 110, 210, steps=8)
            m.cyl(WOOD_D, (xc + 200, 60, H + 360), 124, 90, steps=8, top_r=8)
            for a in (-1, 1):
                m.box(GREY_D, (xc + 200 + a * 80, 60, H + 90), (12, 150, 130))
        else:
            m.box(GREY_D, (xc + r.uniform(-200, 200), 80, H + 70), (190, 150, 90))
            m.cyl(GREY_D, (xc - 260, -80, H + 28), 26, 110, steps=6)


def diner_row(m):
    """A chrome diner under its neon name, an amusement arcade with a blade sign, a pawn shop: 32 m of street."""
    D = 700.0
    # the diner: low, a chrome band, one long lit window
    m.box(SILVER, (-950, 0, 210), (1300, D, 420))
    m.box(MAG, (-950, -D / 2.0 - 3, 96), (1300, 6, 60))
    m.box(WARM_L, (-950, -D / 2.0 - 2, 250), (1100, 4, 190), glow=G2)
    m.box(TEAL, (-950, 0, 432), (1340, D + 40, 24))
    m.box(NAVY, (-950, -200, 520), (760, 16, 150))
    _letters(m, 'DINER', -208, 470, 100, MAG, x=-950.0)
    for s in (-1, 1):
        m.box(GREY_D, (-950 + s * 340, -200, 458), (14, 14, 60))
    # the arcade: tall, a blade sign, coloured lights round the door
    m.box(PLUM, (200, 0, 430), (900, D, 860))
    m.box(STONE_L, (200, 0, 874), (920, D + 24, 28))
    m.box(NAVY, (200, -D / 2.0 - 2, 170), (500, 4, 300))
    m.box(CYAN, (200, -D / 2.0 - 5, 190), (360, 3, 220), glow=G1)
    _bulbs(m, (-80, -D / 2.0 - 10, 30), (-80, -D / 2.0 - 10, 340), 5)
    _bulbs(m, (480, -D / 2.0 - 10, 30), (480, -D / 2.0 - 10, 340), 5)
    _bulbs(m, (-80, -D / 2.0 - 10, 350), (480, -D / 2.0 - 10, 350), 7)
    m.box(NAVY, (-190, -D / 2.0 - 40, 640), (110, 80, 560))
    _stack(m, 'PLAY', -190.0, -D / 2.0 - 80, 900, 84, AMBER)
    for j in range(2):
        for k in range(3):
            m.box(WARM if (j + k) % 2 else CYAN, (40 + k * 190, -D / 2.0 - 2, 520 + j * 190), (130, 4, 130), glow=G2)
    # the pawn shop
    m.box(BRICK_D, (1150, 0, 280), (900, D, 560))
    m.box(STONE_L, (1150, 0, 574), (920, D + 24, 28))
    m.box(WARM, (1090, -D / 2.0 - 2, 170), (560, 4, 220), glow=G2)
    m.box(NAVY, (1490, -D / 2.0 - 2, 132), (120, 4, 256))
    _stripes(m, 760, 1540, -D / 2.0 - 46, 318, 120, 8, [GREEN_D, CREAM], 6, roll=_awn(18))
    m.box(NAVY, (1150, -D / 2.0 - 6, 440), (760, 12, 104))
    _letters(m, 'PAWN', -D / 2.0 - 12, 404, 72, AMBER, x=1150.0)
    for k in range(3):
        m.sphere(GOLD, (1150 + (k - 1) * 60, -D / 2.0 - 60, 560 - abs(k - 1) * 50), 26, steps=(4, 6))
    m.box(GREY_D, (900, 100, 620), (200, 160, 90))


def marquee(m):
    """A picture house: the name in lights over a lit canopy, a blade sign, poster cases, a ticket booth."""
    L, D, H = 3200.0, 700.0, 1150.0
    m.box(PLUM, (0, 0, H / 2.0), (L, D, H))
    m.box(STONE_L, (0, 0, H + 16), (L + 12, D + 24, 32))
    m.box(NAVY, (0, -D / 2.0 - 2, 170), (900, 4, 320))
    for k in range(4):
        m.box(GOLD, (-300 + k * 200, -D / 2.0 - 5, 160), (16, 3, 290))
    m.box(MAG, (0, -D / 2.0 - 30, 400), (1500, 60, 26))
    m.box(WARM_L, (0, -D / 2.0 - 62, 470), (1500, 6, 120), glow=G2)
    for k in range(5):
        m.box(NAVY, (-520 + k * 260, -D / 2.0 - 66, 470), (150, 3, 26))
    _bulbs(m, (-750, -D / 2.0 - 60, 386), (750, -D / 2.0 - 60, 386), 12)
    _bulbs(m, (-750, -D / 2.0 - 68, 540), (750, -D / 2.0 - 68, 540), 12, (AMBER, WARM))
    m.box(NAVY, (0, -D / 2.0 - 10, 700), (900, 16, 250))
    _letters(m, 'NOVA', -D / 2.0 - 18, 620, 160, MAG, x=0.0)
    m.box(NAVY, (-1320, -D / 2.0 - 40, 760), (130, 80, 720))
    _stack(m, 'NOVA', -1320.0, -D / 2.0 - 80, 1110, 100, CYAN)
    m.sphere(AMBER, (-1320, -D / 2.0 - 40, 1150), 46, steps=(4, 6), glow=G4)
    for k, c in enumerate((CYAN, AMBER, MAG, WARM)):
        x = -1050 + k * 250 if k < 2 else 550 + (k - 2) * 250
        m.box(c, (x, -D / 2.0 - 2, 190), (170, 4, 240), glow=G2)
        m.box(GOLD, (x, -D / 2.0 - 4, 326), (190, 3, 14))
    m.box(BRICK_D, (0, -D / 2.0 - 30, 110), (150, 60, 220))
    m.box(WARM, (0, -D / 2.0 - 62, 150), (100, 4, 80), glow=G2)
    for k in range(5):
        m.box(WARM if k % 2 else CYAN, (-1100 + k * 550, -D / 2.0 - 2, 960), (200, 4, 150), glow=G2)


def bank_front(m):
    """A bank: steps, six columns under a pediment with a lit clock, gold doors, the trust's name in the frieze, a
    ticker of lights running along the wing."""
    L, D, H = 3200.0, 700.0, 1000.0
    m.box(STONE, (0, 0, H / 2.0), (L, D, H))
    m.box(STONE_L, (0, 0, H + 18), (L + 16, D + 30, 36))
    m.box(STONE_L, (0, -D / 2.0 - 60, 30), (2300, 120, 60))
    for k in range(6):
        x = -1000 + k * 400
        m.cyl(STONE_L, (x, -D / 2.0 - 60, 60), 42, 640, steps=8)
        m.box(STONE_L, (x, -D / 2.0 - 60, 712), (110, 110, 24))
    m.box(STONE_L, (0, -D / 2.0 - 60, 770), (2300, 130, 90))
    _side(m, STONE_L, [(-1200, 815), (1200, 815), (0, 1090)], -D / 2.0 - 120, -D / 2.0)
    rod(m, WARM_L, (0, -D / 2.0 - 128, 905), (0, -D / 2.0 - 120, 905), 64, steps=12, glow=G2)
    m.box(NAVY, (0, -D / 2.0 - 131, 920), (8, 3, 50))
    m.box(NAVY, (16, -D / 2.0 - 131, 905), (40, 3, 8))
    _letters(m, 'TRUST', -D / 2.0 - 126, 740, 56, GOLD, glow=G1, x=0.0)
    m.box(WARM, (0, -D / 2.0 - 2, 330), (1900, 4, 460), glow=G1)
    for k in (-1, 1):
        m.box(GOLD, (k * 110, -D / 2.0 - 6, 220), (190, 6, 360))
    for s in (-1, 1):
        x = s * 1350
        _rows(m, x - 200, x + 200, -D / 2.0, 250, 3, 250, CYAN, WARM, h=150, seed=3 + s)
        m.box(NAVY, (x, -D / 2.0 - 6, 110), (480, 10, 44))
        for k in range(6):
            m.box([AMBER, MAG, CYAN][k % 3] if (k + s) % 4 else RED, (x - 200 + k * 80, -D / 2.0 - 12, 110), (50, 4, 20), glow=G3)


def pent_entry(m):
    """The street door of a tall, dear building: dark stone and gold, a canopy, lanterns, clipped trees in tubs."""
    L, D, H = 3200.0, 700.0, 940.0
    m.box(SLATE_L, (0, 0, H / 2.0), (L, D, H))
    m.box(GOLD, (0, -D / 2.0 - 3, 30), (L, 6, 14))
    m.box(GOLD, (0, 0, H + 10), (L + 12, D + 24, 20))
    m.box(NAVY, (0, -D / 2.0 - 2, 190), (420, 4, 370))
    m.box(CYAN, (0, -D / 2.0 - 5, 180), (300, 3, 300), glow=G1)
    m.box(NAVY, (0, -D / 2.0 - 60, 392), (620, 130, 20))
    m.box(GOLD, (0, -D / 2.0 - 126, 392), (620, 4, 30))
    for s in (-1, 1):
        m.cyl(GOLD, (s * 290, -D / 2.0 - 110, 0), 8, 384, steps=6)
        m.box(WARM, (s * 420, -D / 2.0 - 8, 250), (40, 16, 70), glow=G3)
        m.box(WARM, (s * 1380, -D / 2.0 - 8, 250), (40, 16, 70), glow=G3)
    for k in range(4):
        x = -1200 + k * 800
        m.box(CONC_D, (x, -D / 2.0 - 70, 40), (110, 110, 80))
        m.cyl(PINE, (x, -D / 2.0 - 70, 80), 62, 230, steps=7, top_r=8)
    for k in range(4):
        x = -1150 + k * 760 + (60 if k > 1 else -60)
        m.box(WARM_L if k % 2 else WARM, (x, -D / 2.0 - 2, 270), (300, 4, 260), glow=G1)
        _stripes(m, x - 170, x + 170, -D / 2.0 - 30, 420, 80, 8, [NAVY, GOLD], 4, roll=_awn(22))
    _rows(m, -1450, 1450, -D / 2.0, 610, 2, 190, WARM, CYAN, h=110, seed=6, p=1.0)
    for s in (-1, 1):
        tilt_box(m, GOLD, (s * 700, -D / 2.0, 780), (s * 760, -D / 2.0 - 220, 900), 8, 8)
        m.box(CREAM if s < 0 else BLUE, (s * 765, -D / 2.0 - 200, 800), (6, 130, 150))


def garage(m, seed=4):
    """A parking garage: four open decks with cars on them, the ramps up its front, its name down a blade sign."""
    r = random.Random(seed)
    L, D = 3200.0, 900.0
    cols = [RED, TEAL, AMBER, CREAM, BLUE, MAG, SILVER]
    for k in range(5):
        z = 20 + k * 300
        m.box(CONC, (0, 0, z), (L, D, 34))
        if k < 4:
            m.box(CONC_D, (0, -D / 2.0 + 6, z + 60), (L, 12, 84))
            for j in range(5):
                x = -L / 2.0 + 60 + j * (L - 120) / 4.0
                m.box(CONC_D, (x, -D / 2.0 + 30, z + 150), (50, 50, 300))
                m.box(CONC_D, (x, D / 2.0 - 30, z + 150), (50, 50, 300))
            for j in range(5):
                if r.random() < 0.75:
                    m.box(r.choice(cols), (-1300 + j * 620 + r.uniform(-60, 60), 120, z + 62), (380, 170, 90))
            m.box(WARM_L, (0, D / 2.0 - 70, z + 262), (L - 200, 8, 10), glow=G1)
    m.box(CONC_D, (0, D / 2.0 - 8, 620), (L, 16, 1240))
    tilt_box(m, CONC_D, (-1500, -D / 2.0 - 130, 30), (-200, -D / 2.0 - 130, 330), 240, 22)
    tilt_box(m, CONC_D, (1500, -D / 2.0 - 130, 330), (200, -D / 2.0 - 130, 630), 240, 22)
    m.box(CONC_D, (0, -D / 2.0 - 130, 330), (420, 240, 22))
    m.box(NAVY, (1450, -D / 2.0 - 30, 760), (130, 60, 700))
    _stack(m, 'PARK', 1450.0, -D / 2.0 - 60, 1100, 100, CYAN)
    m.box(AMBER, (-1250, -D / 2.0 - 2, 250), (300, 4, 60), glow=G3)
    tilt_box(m, RED, (-900, -D / 2.0 - 20, 110), (-560, -D / 2.0 - 20, 190), 10, 10)
    m.box(GREY_D, (-900, -D / 2.0 - 20, 60), (20, 20, 120))


def alley(m, seed=5):
    """An alley between two blocks, 13 m wide: fire escapes up its back wall, bins, a lit back door, washing on a line."""
    r = random.Random(seed)
    W, D, H = 1300.0, 700.0, 1000.0
    m.box(BRICK, (-W / 2.0 + 100, 0, H / 2.0), (200, D, H))
    m.box(BRICK_D, (W / 2.0 - 100, 0, H * 0.42), (200, D, H * 0.84))
    m.box(BRICK_D, (0, D / 2.0 - 30, H / 2.0), (W - 400, 60, H))
    m.box(INDIGO, (0, 0, 4), (W - 400, D, 8))
    for k in range(3):
        z = 300 + k * 290
        m.box(GREY_D, (-60, D / 2.0 - 120, z), (520, 120, 10))
        m.box(GREY_D, (-60, D / 2.0 - 178, z + 50), (520, 4, 6))
        m.box(GREY_D, (-60, D / 2.0 - 178, z + 96), (520, 4, 6))
        if k < 2:
            tilt_box(m, GREY_D, (140, D / 2.0 - 150, z), (-260, D / 2.0 - 150, z + 290), 60, 8)
        m.box(WARM if k != 1 else CYAN, (-170, D / 2.0 - 62, z + 130), (150, 4, 150), glow=G2)
        m.box(GLASS_D, (120, D / 2.0 - 62, z + 130), (150, 4, 150))
    tilt_box(m, GREY_D, (190, D / 2.0 - 150, 300), (190, D / 2.0 - 190, 40), 40, 6)
    m.box(NAVY, (260, D / 2.0 - 62, 110), (110, 4, 220))
    m.box(WARM, (260, D / 2.0 - 70, 250), (50, 14, 24), glow=G3)
    for k in range(2):
        x = -330 + k * 250
        m.box(GREEN_D, (x, 120 - k * 150, 66), (210, 120, 130))
        m.box(GREY_D, (x, 120 - k * 150, 138), (220, 130, 12), rot=(0, 0, 8 if k else 0))
    for k in range(4):
        m.sphere(INDIGO, (r.uniform(-380, 380), r.uniform(-250, 60), 30), r.uniform(34, 52), scale=(1, 1, 0.8), steps=(3, 6))
    rod(m, GREY_D, (-W / 2.0 + 215, 200, 0), (-W / 2.0 + 215, 200, 900), 12, steps=5)
    rod(m, GREY_D, (W / 2.0 - 215, -100, 0), (W / 2.0 - 215, -100, 700), 10, steps=5)
    m.box(GREY_D, (0, -120, 640), (W - 420, 3, 3))
    for k in range(5):
        m.box([CREAM, MAG, CYAN, WARM, TEAL][k], (-320 + k * 160, -120, 606), (90, 4, 64))
    m.cyl(WOOD, (-W / 2.0 + 100, 60, H), 80, 150, steps=7)
    m.cyl(WOOD_D, (-W / 2.0 + 100, 60, H + 150), 92, 60, steps=7, top_r=6)


def service_yard(m):
    """A service yard behind a wire fence: a loading dock, roller doors (one up, the light on inside), a delivery van,
    pallets and drums, a floodlight."""
    L, D = 3200.0, 700.0
    m.box(CONC_D, (0, 170, 320), (L, 360, 640))
    m.box(STONE_L, (0, 170, 652), (L + 10, 380, 24))
    m.box(CONC, (-300, -80, 55), (2200, 160, 110))
    for k in range(3):
        x = -1000 + k * 700
        m.box(NAVY if k != 1 else WARM, (x, -12, 270), (480, 4, 320), glow=0.0 if k != 1 else G1)
        if k != 1:
            for j in range(5):
                m.box(GREY_D, (x, -16, 150 + j * 60), (470, 3, 8))
        m.box(AMBER, (x, -14, 450), (480, 4, 22))
    _car(m, 1050.0, -180.0, WHITE, 'van', MAG)
    for k in range(3):
        m.box(WOOD, (-1350 + (k % 2) * 130, -230, 50 + (k // 2) * 96), (120, 120, 96))
    for k in range(3):
        m.cyl([TEAL, RED, CONC][k], (300 + k * 80, -250, 0), 32, 96, steps=7)
    for k in range(6):
        m.box(GREY_D, (-1550 + k * 620, -D / 2.0 + 6, 110), (8, 8, 220))
    for z in (70, 140, 210):
        m.box(GREY_D, (-630, -D / 2.0 + 6, z), (1900, 4, 5))
        m.box(GREY_D, (1300, -D / 2.0 + 6, z), (560, 4, 5))
    for k in range(3):
        m.cyl(AMBER, (420 + k * 190, -D / 2.0 - 10, 0), 12, 90, steps=6)
    m.cyl(GREY_D, (-1500, -260, 0), 9, 640, steps=5)
    m.box(WARM_L, (-1470, -260, 640), (90, 60, 30), glow=G4)
    for k in range(4):
        m.box(CYAN if k % 2 else WARM, (-1200 + k * 800, -12, 540), (260, 4, 90), glow=G2)


def syn_gate(m):
    """The Syndicate's street front: a black wall, the double chevron in gold over a steel shutter, red lights, cameras,
    spikes along the top, KEEP OUT."""
    L, D, H = 3200.0, 700.0, 900.0
    m.box(BLACK, (0, 0, H / 2.0), (L, D, H))
    m.box(GOLD, (0, -D / 2.0 - 3, 40), (L, 6, 12))
    m.box(RED, (0, -D / 2.0 - 3, H - 20), (L, 6, 16), glow=G3)
    m.box(BLACK, (0, 0, H + 110), (1100, D, 220))
    m.box(RED, (0, -D / 2.0 - 3, H + 212), (1100, 6, 12), glow=G3)
    for s in (-1, 1):
        m.box(BLACK, (s * 1540, -D / 2.0 + 60, 700), (120, 120, 1400))
        m.box(RED, (s * 1540, -D / 2.0 - 2, 700), (14, 4, 1200), glow=G3)
        m.box(GOLD, (s * 1540, -D / 2.0 + 60, 1410), (150, 150, 20))
    m.box(GREY_D, (0, -D / 2.0 - 2, 230), (900, 4, 460))
    for k in range(7):
        m.box(SILVER, (0, -D / 2.0 - 5, 40 + k * 62), (890, 3, 8))
    _chevrons(m, 0.0, -D / 2.0 - 8, 500.0, 900.0, glow=G3)
    for s in (-1, 1):
        m.box(GOLD, (s * 520, -D / 2.0 - 3, H / 2.0), (14, 6, H - 60))
        m.sphere(RED, (s * 560, -D / 2.0 - 20, 500), 26, steps=(3, 5), glow=G4)
        m.sphere(RED, (s * 1540, -D / 2.0 + 60, 1466), 46, steps=(4, 6), glow=G4)
        tilt_box(m, GREY_D, (s * 900, -D / 2.0, 700), (s * 900, -D / 2.0 - 70, 680), 14, 14)
        m.box(SILVER, (s * 900, -D / 2.0 - 90, 672), (30, 50, 30))
        rod(m, GREY_D, (s * 1300, -D / 2.0 - 30, H + 40), (s * 1300, -D / 2.0 - 90, H + 40), 46, steps=8)
        rod(m, WARM_L, (s * 1300, -D / 2.0 - 92, H + 40), (s * 1300, -D / 2.0 - 96, H + 40), 38, steps=8, glow=G4)
    for k in range(15):
        m.cyl(SILVER, (-1400 + k * 200, -D / 2.0 + 20, H), 14, 60, steps=4, top_r=1)
    _letters(m, 'KEEP OUT', -D / 2.0 - 8, 250, 70, RED, x=-1050.0)
    _chevrons(m, 1100.0, -D / 2.0 - 8, 220.0, 260.0, glow=G1)
    for z in (140, 660):
        m.box(RED, (1100, -D / 2.0 - 4, z), (500, 3, 10), glow=G2)


# ------------------------------------------------------------------------------------------ the sidewalk (low, unlit: nearer than 24 m)
def parked_cars(m, cols=(AMBER, TEAL, CREAM), kinds=('taxi', 'sedan', 'van')):
    """Three cars at the kerb, a parking meter by each."""
    for k in range(3):
        x = -640 + k * 640
        _car(m, x, 0.0, cols[k], kinds[k])
        m.cyl(GREY_D, (x + 280, 150, 0), 5, 110, steps=5)
        m.box(SILVER, (x + 280, 150, 122), (16, 12, 26))


def syn_cars(m):
    """The Syndicate's cars at the kerb: two black saloons and an armoured van, a gold line down each."""
    _car(m, -640.0, 0.0, BLACK, 'sedan', GOLD)
    _car(m, 0.0, 0.0, BLACK, 'sedan', GOLD)
    _car(m, 680.0, 0.0, BLACK, 'van', GOLD)
    for x in (-660.0, -20.0):
        m.box(GOLD, (x, 0, 160), (200, 60, 4))
    m.box(GOLD, (650, 0, 191), (380, 60, 3))
    _chevrons(m, 640.0, -108.0, 110.0, 90.0, glow=0.0, t=10)


def sidewalk(m):
    """The sidewalk's furniture: a bench, a clipped tree in a tub, a bin, a mail box, a bike rack, paper boxes. Like
    everything on the sidewalk it is under 2 m and unlit: nothing nearer the road than 24 m may be taller or lit."""
    m.box(CONC_D, (0, 30, 34), (200, 200, 68))
    m.cyl(WOOD_D, (0, 30, 68), 12, 60, steps=5)
    m.sphere(PINE, (0, 30, 138), 56, steps=(4, 7))
    m.box(WOOD, (-330, 20, 46), (280, 54, 10))
    m.box(WOOD, (-330, 46, 82), (280, 10, 62))
    for e in (-1, 1):
        m.box(GREY_D, (-330 + e * 120, 24, 22), (12, 50, 44))
    m.cyl(GREEN_D, (-560, 20, 0), 32, 96, steps=8)
    m.box(BLUE, (330, 30, 62), (64, 64, 124))
    m.sphere(BLUE, (330, 30, 124), 32, scale=(1, 1, 0.6), steps=(3, 6))
    for k in range(3):
        x = 520 + k * 70
        m.box(GREY_D, (x, 0, 44), (5, 5, 88))
        m.box(GREY_D, (x, 60, 44), (5, 5, 88))
        m.box(GREY_D, (x, 30, 88), (5, 65, 5))
    for k, c in enumerate((RED, AMBER, TEAL)):
        m.box(c, (-760 - k * 62, 30, 56), (54, 50, 112))


def bus_stop(m):
    """A bus shelter (no lights in it), its bench, the stop's pole."""
    for sx in (-1, 1):
        for sy in (0, 1):
            m.box(GREY_D, (sx * 250, sy * 150, 92), (10, 10, 184))
    m.box(CONC_D, (0, 60, 190), (560, 210, 12))
    m.box(GLASS, (0, 150, 100), (500, 6, 160))
    m.box(GLASS, (-250, 75, 100), (6, 150, 160))
    m.box(NAVY, (250, 75, 95), (8, 150, 180))
    for k, c in enumerate((MAG, TEAL, AMBER)):
        m.box(c, (255, 75, 50 + k * 50), (4, 120, 40))
    m.box(WOOD, (-20, 110, 48), (360, 44, 10))
    m.cyl(GREY_D, (430, 0, 0), 6, 150, steps=5)
    rod(m, BLUE, (430, -6, 158), (430, 6, 158), 36, steps=10)
    m.box(CREAM, (430, 0, 90), (50, 6, 60))


def syn_barriers(m):
    """The Syndicate's roadside: black concrete barriers with the gold chevron on each, bollards, a spike strip, cones."""
    for k in range(6):
        x = -1000 + k * 400
        body, mark = (GOLD, BLACK) if k % 2 else (BLACK, GOLD)      # unlit, so they are gold and black turn about: they must read by the street's own light
        m.box(body, (x, 0, 34), (370, 62, 68))
        m.box(body, (x, 0, 78), (370, 30, 22))
        _chevrons(m, x, -33.0, 14.0, 110.0, col=mark, glow=0.0, n=2, t=11)
    for s in (-1, 1):
        m.cyl(BLACK, (s * 1300, 0, 0), 22, 110, steps=7)
        m.cyl(GOLD, (s * 1300, 0, 84), 24, 12, steps=7)
    m.box(GREY_D, (0, -110, 4), (900, 40, 8))
    for k in range(9):
        m.cyl(SILVER, (-400 + k * 100, -110, 8), 7, 16, steps=4, top_r=1)
    for k in range(3):
        m.cyl(ORANGE, (-900 + k * 900, 110, 0), 20, 62, steps=6, top_r=5)


def checkpoint(m):
    """A Syndicate checkpoint: the guard's hut with a red beacon, a lifted barrier arm, sandbags, NO ENTRY."""
    m.box(BLACK, (0, 40, 150), (300, 300, 300))
    m.box(GOLD, (0, 40, 306), (340, 340, 14))
    m.box(WARM, (0, -112, 190), (200, 4, 90), glow=G2)
    m.box(WARM, (-152, 40, 190), (4, 200, 90), glow=G2)
    _chevrons(m, 0.0, -112.0, 40.0, 150.0, glow=G3, t=14)
    m.cyl(GREY_D, (0, 40, 313), 8, 40, steps=5)
    m.sphere(RED, (0, 40, 380), 40, steps=(4, 6), glow=G4)
    m.box(BLACK, (-330, -90, 60), (40, 40, 120))
    for k in range(5):
        tilt_box(m, RED if k % 2 == 0 else WHITE, (-330 - k * 70 * 0.5, -90, 120 + k * 70 * 0.86), (-330 - (k + 1) * 70 * 0.5, -90, 120 + (k + 1) * 70 * 0.86), 12, 12)
    for j in range(2):
        for k in range(5 - j):
            m.sphere(SAND_D, (220 + k * 76 + j * 38, -100, 24 + j * 40), 40, scale=(1.0, 0.7, 0.55), steps=(3, 6))
    for s in (-1, 1):
        m.box(GREY_D, (520 + s * 190, 0, 150), (10, 10, 300))
    m.box(BLACK, (520, 0, 250), (440, 12, 150))
    _letters(m, 'NO ENTRY', -8, 224, 50, RED, x=520.0)
    m.cyl(GREY_D, (-440, 60, 0), 9, 620, steps=5)
    m.box(WARM_L, (-440, 30, 620), (60, 90, 30), glow=G4)


def syn_banner(m):
    """Two of the Syndicate's banners on black poles: the gold double chevron on black cloth."""
    for s in (-1, 1):
        x = s * 600
        m.cyl(BLACK, (x, 0, 0), 10, 900, steps=6)
        m.sphere(RED, (x, 0, 918), 26, steps=(4, 6), glow=G4)
        m.box(BLACK, (x, 0, 20), (70, 70, 40))
        m.box(GOLD, (x + 90, 0, 860), (200, 8, 8))
        m.box(BLACK, (x + 100, 0, 560), (170, 6, 580))
        m.box(RED, (x + 100, 0, 276), (170, 8, 12), glow=G3)
        _chevrons(m, x + 100, -6.0, 520.0, 130.0, glow=G3, t=16)
        _chevrons(m, x + 100, 6.0, 520.0, 130.0, glow=G3, t=16)


def search_tower(m):
    """A Syndicate watch tower: a black lattice, a platform with the chevron on its rail, a searchlight, a red beacon."""
    H = 1800.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, GREY_D, (sx * 300, sy * 300, 0), (sx * 130, sy * 130, H), 26, 26)
    for k in range(1, 4):
        z = k * H / 4.0
        w = 300 - 170 * k / 4.0
        for s in (-1, 1):
            m.box(GREY_D, (0, s * w, z), (2 * w, 14, 14))
            m.box(GREY_D, (s * w, 0, z), (14, 2 * w, 14))
    m.box(BLACK, (0, 0, H + 10), (420, 420, 20))
    for s in (-1, 1):
        m.box(BLACK, (0, s * 205, H + 70), (420, 8, 100))
        m.box(BLACK, (s * 205, 0, H + 70), (8, 420, 100))
    _chevrons(m, 0.0, -212.0, H + 30, 150.0, glow=G3, t=14)
    for k in range(1, 4):
        m.sphere(RED, (-300 + 42.5 * k, -300 + 42.5 * k - 20, k * H / 4.0), 22, steps=(3, 5), glow=G4)
    rod(m, GREY_D, (0, -60, H + 190), (-40, -200, H + 150), 56, steps=8)
    rod(m, WARM_L, (-40, -200, H + 150), (-42, -208, H + 148), 48, steps=8, glow=G4)
    m.cyl(GREY_D, (0, 0, H + 20), 14, 150, steps=5)
    m.cyl(GREY_D, (120, 120, H + 20), 6, 300, steps=4)
    m.sphere(RED, (120, 120, H + 330), 24, steps=(4, 6), glow=G4)
    tilt_box(m, GREY_D, (260, -310, 0), (150, -150, H), 40, 6)


# ------------------------------------------------------------------------------------------ the middle distance
def mid_block(m, seed=2):
    """A mid-rise block with its roof: a water tank on legs, plant, a sign on a frame."""
    r = random.Random(seed)
    W, D, H = 2400.0, 2000.0, 3200.0
    m.box(SLATE, (0, 0, H / 2.0), (W, D, H))
    m.box(STONE, (0, 0, 200), (W + 40, D + 40, 400))
    m.box(WARM_L, (0, -D / 2.0 - 22, 190), (900, 4, 220), glow=G2)
    _rows(m, -W * 0.45, W * 0.45, -D / 2.0, 620, 8, 320, WARM, CYAN, seed=seed)
    _ends(m, -W / 2.0 - 2, -D * 0.45, D * 0.45, 620, 8, 320, CYAN, seed=seed + 1)
    _ends(m, W / 2.0 + 2, -D * 0.45, D * 0.45, 620, 8, 320, WARM, seed=seed + 2)
    m.box(STONE, (0, 0, H + 30), (W + 30, D + 30, 60))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(GREY_D, (600 + sx * 110, 300 + sy * 110, H + 190), (14, 14, 260))
    m.cyl(WOOD, (600, 300, H + 320), 170, 300, steps=10)
    m.cyl(WOOD_D, (600, 300, H + 620), 186, 130, steps=10, top_r=10)
    for k in range(3):
        m.box(GREY_D, (-800 + k * 330, 500, H + 110), (240, 200, 100))
    for s in (-1, 1):
        m.box(GREY_D, (-300 + s * 620, -D / 2.0 + 60, H + 230), (16, 16, 340))
    m.box(NAVY, (-300, -D / 2.0 + 50, H + 400), (1340, 20, 300))
    _letters(m, 'ATLAS', -D / 2.0 + 40, H + 300, 200, MAG, x=-300.0)


def glass_block(m, seed=3):
    """A glass office slab banded with light, a pool and a landing pad on its roof."""
    W, D, H = 2000.0, 1600.0, 4400.0
    m.box(GLASS_D, (0, 0, H / 2.0), (W, D, H))
    m.box(SLATE_L, (0, 0, 180), (W + 30, D + 30, 360))
    m.box(WARM_L, (0, -D / 2.0 - 17, 180), (700, 4, 220), glow=G2)
    _rows(m, -W * 0.47, W * 0.47, -D / 2.0, 560, 12, 320, CYAN, WARM, h=150, seed=seed, p=0.9)
    _ends(m, -W / 2.0 - 2, -D * 0.46, D * 0.46, 560, 12, 320, CYAN, h=150, seed=seed + 1, p=0.75)
    _ends(m, W / 2.0 + 2, -D * 0.46, D * 0.46, 560, 12, 320, CYAN, h=150, seed=seed + 2, p=0.75)
    m.box(SLATE_L, (0, 0, H + 20), (W + 20, D + 20, 40))
    m.box(CYAN, (-450, -300, H + 44), (700, 320, 8), glow=G1)
    m.box(CREAM, (-450, -300, H + 42), (760, 380, 6))
    m.cyl(GREY_D, (500, 250, H + 40), 330, 10, steps=14)
    _ring(m, AMBER, (500, 250, H + 54), 250, 10, rot=(0, 0, 0), steps=16, glow=G2)
    m.box(SLATE_L, (-500, 400, H + 190), (600, 500, 300))
    m.box(WARM, (-500, 148, H + 200), (460, 4, 150), glow=G2)
    m.cyl(SILVER, (700, -500, H + 40), 10, 600, steps=5)
    m.sphere(RED, (700, -500, H + 650), 30, steps=(4, 6), glow=G4)


def el_rail(m):
    """Sixty metres of the elevated railway: the viaduct on its bents, a three-car train on it with its lights on, a
    station's canopy and stair at one end."""
    L = 6000.0
    m.box(GREY_D, (0, 0, 900), (L, 520, 70))
    for s in (-1, 1):
        m.box(TEAL, (0, s * 262, 950), (L, 18, 130))
    for k in range(4):
        x = -2250 + k * 1500
        for s in (-1, 1):
            m.cyl(GREY_D, (x, s * 200, 0), 34, 870, steps=6)
        m.box(GREY_D, (x, 0, 850), (90, 520, 60))
    for k in range(3):
        x = -1900 + k * 1560
        m.box(SILVER, (x, 0, 1110), (1500, 300, 300))
        m.box(MAG, (x, -152, 1010), (1500, 4, 40))
        m.box(WARM_L, (x, -152, 1150), (1320, 4, 100), glow=G2)
        m.box(GREY_D, (x, 0, 1272), (900, 200, 24))
        for e in (-1, 1):
            m.box(TYRE, (x + e * 520, 0, 950), (260, 240, 40))
    m.box(PLUM, (2300, -330, 1420), (1300, 260, 20))
    for e in (-1, 1):
        m.box(GREY_D, (2300 + e * 600, -420, 1180), (12, 12, 480))
    m.box(GREY_D, (2300, -330, 940), (1300, 200, 30))
    tilt_box(m, GREY_D, (2960, -330, 0), (2960, -330, 930), 14, 14)
    tilt_box(m, CONC, (2900, -560, 0), (2300, -450, 920), 180, 16)
    for k in range(3):
        m.sphere(WARM, (1850 + k * 450, -330, 1390), 26, steps=(4, 6), glow=G4)
    m.box(CYAN, (2300, -462, 1330), (420, 4, 80), glow=G3)


def monument(m):
    """The plaza's centre: a ring of water round a plinth, a gold figure of stacked blocks and a ring, flagpoles,
    trees in tubs, globe lamps."""
    m.cyl(STONE_L, (0, 0, 0), 560, 44, steps=16)
    m.cyl(CYAN, (0, 0, 44), 460, 6, steps=16, glow=G1)
    m.cyl(STONE, (0, 0, 44), 170, 250, steps=8)
    for k in range(3):
        m.box(GOLD, (0, 0, 370 + k * 150), (190 - k * 40, 190 - k * 40, 150), rot=(0, k * 30, 0))
    _ring(m, GOLD, (0, 0, 880), 190, 22, steps=16, glow=G1)
    cols = [CREAM, CYAN, MAG, AMBER]
    for k in range(4):
        a = k * math.pi / 2.0 + math.pi / 4.0
        x, y = math.cos(a) * 760, math.sin(a) * 760
        m.cyl(SILVER, (x, y, 0), 7, 900, steps=5)
        _side(m, cols[k], [(x, 880), (x + 150, 835), (x, 790)], y - 3, y + 3)
        x2, y2 = math.cos(a + 0.6) * 640, math.sin(a + 0.6) * 640
        m.box(CONC_D, (x2, y2, 40), (120, 120, 80))
        m.cyl(PINE, (x2, y2, 80), 66, 240, steps=7, top_r=8)
        x3, y3 = math.cos(a - 0.6) * 640, math.sin(a - 0.6) * 640
        m.cyl(GREY_D, (x3, y3, 0), 6, 300, steps=5)
        m.sphere(WARM_L, (x3, y3, 320), 30, steps=(4, 6), glow=G4)


def clock_tower(m):
    """The plaza's clock tower: lit faces on all four sides, a pointed roof."""
    W, H = 640.0, 2600.0
    m.box(STONE, (0, 0, H / 2.0), (W, W, H))
    m.box(STONE_L, (0, 0, 150), (W + 80, W + 80, 300))
    m.box(STONE_L, (0, 0, 1900), (W + 60, W + 60, 40))
    m.box(STONE_L, (0, 0, H + 20), (W + 80, W + 80, 40))
    for s in (-1, 1):
        rod(m, WARM_L, (0, s * (W / 2.0 + 2), 2240), (0, s * (W / 2.0 + 10), 2240), 220, steps=14, glow=G2)
        rod(m, WARM_L, (s * (W / 2.0 + 2), 0, 2240), (s * (W / 2.0 + 10), 0, 2240), 220, steps=14, glow=G2)
    m.box(NAVY, (0, -W / 2.0 - 12, 2300), (16, 4, 150))
    m.box(NAVY, (50, -W / 2.0 - 12, 2240), (120, 4, 16))
    for k in range(5):
        m.box(WARM if k % 2 else CYAN, (0, -W / 2.0 - 2, 480 + k * 280), (90, 4, 170), glow=G2)
    m.cyl(SLATE_L, (0, 0, H + 40), 470, 640, steps=4, top_r=14, rot=(0, 45, 0))
    m.cyl(GOLD, (0, 0, H + 680), 8, 260, steps=5)
    m.sphere(GOLD, (0, 0, H + 950), 34, steps=(4, 6), glow=G1)


def syn_hq(m):
    """A Syndicate headquarters block: black, three double chevrons up its face in gold, red slits of light, red
    beacons at its corners, searchlights and masts on the roof."""
    W, D, H = 2600.0, 2200.0, 6400.0
    m.box(BLACK, (0, 0, H / 2.0), (W, D, H))
    m.box(GREY_D, (0, 0, 220), (W + 60, D + 60, 440))
    m.box(GOLD, (0, -D / 2.0 - 32, 440), (W + 60, 4, 16))
    m.box(BLACK, (0, 0, H + 450), (W * 0.66, D * 0.66, 900))
    for k in range(3):
        _chevrons(m, 0.0, -D / 2.0 - 6, 1300.0 + k * 1750, 1500.0, t=150)
    r = random.Random(11)
    for k in range(9):
        m.box(RED, (r.choice((-1, 1)) * r.uniform(500, 1000), -D / 2.0 - 2, 700 + k * 620), (r.uniform(300, 520), 4, 50), glow=G2)
    for s in (-1, 1):
        m.box(GOLD, (s * (W / 2.0 + 2), 0, H / 2.0), (4, 40, H - 600))
        for k in range(6):
            m.box(RED, (s * (W / 2.0 + 2), (k % 3 - 1) * 600, 900 + k * 900), (4, 420, 46), glow=G2)
        for e in (-1, 1):
            m.sphere(RED, (s * W / 2.0, e * D / 2.0, H + 40), 60, steps=(4, 6), glow=G4)
        rod(m, GREY_D, (s * 700, -500, H + 960), (s * 760, -640, H + 930), 90, steps=8)
        rod(m, WARM_L, (s * 760, -640, H + 930), (s * 764, -650, H + 928), 78, steps=8, glow=G4)
    m.cyl(SILVER, (0, 0, H + 900), 30, 1500, steps=6, top_r=4)
    m.sphere(RED, (0, 0, H + 2440), 50, steps=(4, 6), glow=G4)
    m.cyl(SILVER, (300, 200, H + 900), 12, 700, steps=5)


# ------------------------------------------------------------------------------------------ the horizon (built from z = 0 up)
def far_towers(m, seed=7, deco=False):
    """A cluster of downtown towers for the skyline: slabs banded with light (or stepped deco towers with spires)."""
    r = random.Random(seed)
    bodies = [INDIGO, SLATE, SLATE_L, '#2a3a54', '#3a3858']
    lits = [CYAN, WARM, WARM_L, '#9ff0ff', '#ffc89a']
    x = -11000.0
    for k in range(5):
        w = r.choice([2600, 3200, 3800])
        h = r.choice([12000, 15000, 18500, 22000]) * (1.15 if k == 2 else 1.0)
        d = 2600.0
        body, lit = bodies[k % 5], lits[(k + seed) % 5]
        xc = x + w / 2.0
        if deco:
            for j in range(3):
                f = 1.0 - j * 0.22
                z0, z1 = h * (0, 0.55, 0.8)[j], h * (0.55, 0.8, 1.0)[j]
                m.box(body, (xc, 0, (z0 + z1) / 2.0), (w * f, d * f, z1 - z0))
                for s in (-1, 0, 1):
                    m.box(lit, (xc + s * w * f * 0.3, -d * f / 2.0 - 8, (z0 + z1) / 2.0), (w * f * 0.09, 16, (z1 - z0) * 0.86), glow=G2)
            m.cyl(SILVER, (xc, 0, h), 160, h * 0.16, steps=6, top_r=8)
            m.sphere(RED, (xc, 0, h * 1.16 + 60), 130, steps=(4, 6), glow=G4)
        else:
            m.box(body, (xc, 0, h / 2.0), (w, d, h))
            n = int(h / 1100)
            for j in range(1, n):
                if r.random() < 0.8:
                    m.box(lit if r.random() < 0.75 else r.choice(lits), (xc, -d / 2.0 - 8, j * 1100.0), (w * 0.86, 16, 420), glow=G2)
            if k % 2:
                m.box(MAG if k == 1 else CYAN, (xc, 0, h + 40), (w + 40, d + 40, 80), glow=G3)
            else:
                m.box(body, (xc, 0, h + 500), (w * 0.5, d * 0.5, 1000))
                m.sphere(RED, (xc, 0, h + 1120), 120, steps=(4, 6), glow=G4)
        x += w + r.choice([300, 700, 1100])


def far_radio(m, H=21000.0):
    """The radio mast on the skyline: an orange and white lattice 230 m tall, red lights up it."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, ORANGE, (sx * 2400, sy * 2400, 0), (sx * 220, sy * 220, H), 300, 300)
    for k in range(1, 8):
        z = k * H / 8.0
        w = 2400 - 2180.0 * k / 8.0
        c = WHITE if k % 2 else ORANGE
        for s in (-1, 1):
            m.box(c, (0, s * w, z), (2 * w, 150, 150))
            m.box(c, (s * w, 0, z), (150, 2 * w, 150))
        if k % 2 == 0:
            m.sphere(RED, (0, -w - 120, z), 150, steps=(4, 6), glow=G4)
    m.box(GREY_D, (0, 0, H * 0.62), (1900, 1900, 200))
    m.box(WARM, (0, -960, H * 0.62 + 300), (1500, 20, 300), glow=G2)
    m.cyl(WHITE, (0, 0, H), 110, 3000, steps=6, top_r=20)
    m.sphere(RED, (0, 0, H + 3100), 200, steps=(4, 6), glow=G4)


def far_bridge(m, L=42000.0):
    """A suspension bridge across the skyline: two towers, the cables' long curves, lights along the deck."""
    top, deck = 13000.0, 3400.0
    for sx in (-1, 1):
        x = sx * 9000
        for sy in (-1, 1):
            m.box(STONE, (x, sy * 620, top / 2.0), (520, 420, top))
        for z in (deck + 2400, deck + 5600, top - 300):
            m.box(STONE, (x, 0, z), (420, 1500, 380))
        m.sphere(RED, (x, 0, top + 150), 170, steps=(4, 6), glow=G4)
    m.box(SLATE, (0, 0, deck), (L, 1500, 300))

    def cable(x):
        ax = abs(x)
        if ax <= 9000:
            return deck + 900 + (top - deck - 900) * (ax / 9000.0) ** 2
        return top - (top - deck - 500) * ((ax - 9000) / (L / 2.0 - 9000)) ** 1.15
    n = 28
    for sy in (-1, 1):
        for k in range(n):
            x0, x1 = -L / 2.0 + k * L / n, -L / 2.0 + (k + 1) * L / n
            tilt_box(m, AMBER, (x0, sy * 620, cable(x0)), (x1, sy * 620, cable(x1)), 110, 110, glow=G1)
    for k in range(1, 12):
        x = -9000 + k * 1500
        m.box(SLATE_L, (x, -620, (cable(x) + deck) / 2.0), (50, 50, cable(x) - deck))
    for k in range(21):
        m.sphere(WARM if k % 2 else AMBER, (-L / 2.0 + (k + 0.5) * L / 21.0, -780, deck + 260), 120, steps=(3, 5), glow=G4)


def far_syndicate(m):
    """The Syndicate's towers on the skyline: three black needles behind one wall, a gold double chevron and a red
    beacon on each, searchlights along the wall."""
    m.box(BLACK, (0, 0, 1400), (17000, 1800, 2800))
    m.box(RED, (0, -910, 2500), (16000, 20, 160), glow=G2)
    for x, h, w in ((-5200, 17000.0, 2600.0), (0, 25000.0, 3200.0), (5400, 14500.0, 2400.0)):
        m.box(BLACK, (x, 0, h * 0.35), (w, w, h * 0.7))
        m.box(BLACK, (x, 0, h * 0.8), (w * 0.7, w * 0.7, h * 0.2))
        m.box(BLACK, (x, 0, h * 0.95), (w * 0.42, w * 0.42, h * 0.1))
        m.cyl(SILVER, (x, 0, h), 90, h * 0.12, steps=6, top_r=8)
        m.sphere(RED, (x, 0, h * 1.12 + 80), 190, steps=(5, 8), glow=G4)
        for k in range(2):
            _chevrons(m, x, -w / 2.0 - 10, h * (0.28 + 0.22 * k), w * 0.8, t=w * 0.09)
        for k in range(4):
            m.box(RED, (x, -w / 2.0 - 8, h * (0.1 + 0.035 * k)), (w * 0.7, 16, 120), glow=G2)
    for k in range(6):
        m.sphere(WARM_L, (-7500 + k * 3000, -960, 2900), 190, steps=(4, 6), glow=G4)


# ------------------------------------------------------------------------------------------ second pass: the ground plane
# What the city is made of where a banked, looping or flying camera looks down on it: side streets between the blocks,
# parking lots, a corporate plaza, and the faces of the cut banks beside the banked bends. All of it FLAT or lying on
# the bank, and UNLIT. The track sinks a piece 15 cm into the land, so each has a kerb going 1.4 m down and its top
# 25 to 35 cm up; each kind has its own top height, so two that overlap lie one over the other instead of flickering.
def _disc(m, col, r, z0, z1, at=(0.0, 0.0), steps=20, glow=0.0):
    m.cyl(col, (at[0], at[1], z0), r, z1 - z0, steps=steps, glow=glow)


def _turned(m, x, y, z, yaw, draw):
    """Draw with the model's own frame moved to (x, y, z) and turned by yaw: for a car parked across a piece."""
    m.base = ft_art._xf((x, y, z), (0.0, yaw, 0.0))
    try:
        draw()
    finally:
        m.base = None


def floor_cross(m, L=5000.0, W=1100.0):
    """A side street between two blocks, 50 m long, running AWAY from the avenue (along Y): tarmac between pale kerbs, a
    zebra crossing and a stop line at its mouth, a yellow centre line, manholes, three cars parked along it."""
    m.box(STONE_L, (0, 0, -57), (W + 160, L, 166))
    m.box(INDIGO, (0, 0, 24), (W, L, 8))
    for k in range(6):
        m.box(CREAM, (-W / 2.0 + 110 + k * (W - 220) / 5.0, -L / 2.0 + 300, 29), (100, 420, 5))
    m.box(CREAM, (0, -L / 2.0 + 600, 29), (W - 80, 44, 5))
    for k in range(6):
        m.box(AMBER, (0, -L / 2.0 + 1000 + k * 640, 29), (22, 330, 5))
    for y in (-600, 1300):
        _disc(m, GREY_D, 46, 27, 30.5, at=(-200, y), steps=10)
    for k, (y, col, kind) in enumerate(((-900, AMBER, 'taxi'), (400, RED, 'sedan'), (1700, TEAL, 'van'))):
        _turned(m, (W / 2.0 - 130) * (1 if k % 2 else -1), y, 28, 90.0, lambda col=col, kind=kind: _car(m, 0, 0, col, kind))


def floor_lot(m, L=2800.0, W=1700.0):
    """A parking lot, 28 m by 17 m: tarmac inside a pale kerb, bays painted down both sides of a lane, an arrow, seven
    cars, a pay booth, a clipped tree in a tub at two corners."""
    m.box(STONE_L, (0, 0, -58), (L, W, 164))
    m.box(INDIGO, (0, 0, 21), (L - 70, W - 70, 8))
    for sy in (-1, 1):
        for k in range(9):
            m.box(CREAM, (-L / 2.0 + 190 + k * 300, sy * (W / 2.0 - 290), 26), (16, 460, 4))
    m.plan(CREAM, _ccw([(-420, -36), (120, -36), (120, -110), (420, 0), (120, 110), (120, 36), (-420, 36)]), 25, 28)
    cars = ((0, -1, AMBER, 'taxi'), (2, -1, RED, 'sedan'), (3, -1, SILVER, 'sedan'), (6, -1, TEAL, 'van'), (1, 1, BLUE, 'sedan'),
            (4, 1, CREAM, 'van'), (7, 1, AMBER, 'taxi'))
    for k, sy, col, kind in cars:
        _turned(m, -L / 2.0 + 340 + k * 300, sy * (W / 2.0 - 300), 25, 90.0 * sy, lambda col=col, kind=kind: _car(m, 0, 0, col, kind))
    m.box(CREAM, (L / 2.0 - 150, 0, 110), (150, 150, 170))
    m.box(RED, (L / 2.0 - 150, 0, 200), (180, 180, 14))
    for sy in (-1, 1):
        m.box(CONC_D, (L / 2.0 - 150, sy * (W / 2.0 - 150), 56), (150, 150, 64))
        m.sphere(PINE, (L / 2.0 - 150, sy * (W / 2.0 - 150), 138), 60, steps=(4, 7))


def floor_plaza(m, S=2400.0):
    """A corporate plaza, 24 m square: pale stone laid in big squares of two tones, a round basin with a gold figure
    in it, a tub with a clipped tree at each corner, four benches. Nothing over 1.9 m, nothing lit."""
    m.box(STONE, (0, 0, -57), (S, S, 166))
    m.box(STONE_L, (0, 0, 24), (S - 80, S - 80, 8))
    q = (S - 80) / 4.0
    for i in range(4):
        for j in range(4):
            if (i + j) % 2 == 0:
                m.box(CREAM, (-1.5 * q + i * q, -1.5 * q + j * q, 27), (q - 40, q - 40, 5))
    _disc(m, STONE, 420, 26, 64, steps=16)
    _disc(m, BLUE, 370, 60, 67, steps=16)
    m.cyl(GOLD, (0, 0, 64), 40, 60, steps=6, top_r=70)
    m.sphere(GOLD, (0, 0, 150), 44, scale=(1, 1, 1.3), steps=(4, 6))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(CONC_D, (sx * (S / 2.0 - 190), sy * (S / 2.0 - 190), 60), (190, 190, 64))
            m.sphere(PINE, (sx * (S / 2.0 - 190), sy * (S / 2.0 - 190), 146), 66, steps=(4, 7))
        m.box(WOOD, (sx * 640, 0, 70), (60, 300, 10))
        m.box(WOOD, (0, sx * 640, 70), (300, 60, 10))
        for e in (-1, 1):
            m.box(GREY_D, (sx * 640, e * 120, 46), (50, 12, 40))
            m.box(GREY_D, (e * 120, sx * 640, 46), (12, 50, 40))


# The sidewalk, brighter: everything on it must be under 2 m and unlit, so these read by colour under the street's lamps.
def kerb_cafe(m):
    """A pavement cafe, 17 m of it: four striped parasols over round tables and bright chairs, a planter of flowers
    towards the road, a menu board."""
    for k in range(4):
        x = -600 + k * 400
        a, b = ((RED, CREAM), (TEAL, CREAM))[k % 2]
        m.cyl(WOOD_D, (x, 40, 0), 5, 150, steps=5)
        m.cyl(a, (x, 40, 150), 150, 42, steps=8, top_r=10)
        m.cyl(b, (x, 40, 144), 156, 7, steps=8)
        m.cyl(GREY_D, (x, 40, 0), 6, 70, steps=5)
        m.cyl(CREAM, (x, 40, 70), 48, 6, steps=8)
        for e in (-1, 1):
            m.box(MAG if k % 2 else AMBER, (x + e * 84, 40, 24), (40, 40, 48))
            m.box(MAG if k % 2 else AMBER, (x + e * 102, 40, 62), (6, 40, 40))
    m.box(CONC_D, (0, -80, 24), (1700, 44, 48))
    for k in range(9):
        m.sphere((MAG, AMBER, WHITE)[k % 3] if k % 2 else PINE, (-760 + k * 190, -80, 62), 44, scale=(1.6, 0.8, 0.7), steps=(3, 6))
    m.box(NAVY, (900, 20, 62), (12, 76, 124))
    m.box(CREAM, (893, 20, 70), (4, 56, 84))


def kerb_works(m):
    """Road works on the sidewalk, 15 m of them: orange barrels with white bands, cones, a striped barrier on trestles,
    a striped tent over a manhole, a cable drum. Bright, low and unlit."""
    for k in range(5):
        x = -700 + k * 150
        m.cyl(ORANGE, (x, -30 + (k % 2) * 50, 0), 30, 100, steps=8)
        m.cyl(WHITE, (x, -30 + (k % 2) * 50, 30), 31, 14, steps=8)
        m.cyl(WHITE, (x, -30 + (k % 2) * 50, 66), 31, 14, steps=8)
    for k in range(4):
        x = 80 + k * 110
        m.box(NAVY, (x, -60, 4), (44, 44, 8))
        m.cyl(ORANGE, (x, -60, 8), 20, 66, steps=6, top_r=4)
        m.cyl(WHITE, (x, -60, 34), 14, 12, steps=6, top_r=11)
    for e in (-1, 1):
        m.box(GREY_D, (300 + e * 200, 60, 50), (10, 60, 100))
    for k in range(6):
        m.box(ORANGE if k % 2 else WHITE, (125 + k * 70, 60, 96), (70, 8, 30))
    _side(m, AMBER, [(540, 0), (700, 0), (620, 150)], -40, 110)
    for k in range(3):
        m.box(WHITE, (566 + k * 54, -42, 60 - abs(k - 1) * 30), (22, 4, 110 - abs(k - 1) * 60))
    rod(m, WOOD, (-60, 30, 62), (-60, 110, 62), 62, steps=8)
    rod(m, WOOD_D, (-60, 22, 62), (-60, 30, 62), 76, steps=8)
    rod(m, WOOD_D, (-60, 110, 62), (-60, 118, 62), 76, steps=8)


# The banks of the banked bends (see ft_dress_park.py's note on them: the same cut, the same way of standing a piece
# on it; these are bent to the city's tighter bends, a 70 m radius). Unlit; nothing on them points left or right.
BANK_DEG = 58.6
BANK_R = 7000.0


def _bank_len(m, k, kerb, face, draw):
    """Draw one 8 m length (k = -1, 0, 1) in the bank's own frame: x along the road, y up the slope, z out of it."""
    xc = k * 800.0
    m.base = ft_art._xf((xc, xc * xc / (2.0 * BANK_R), 0.0), (0.0, math.degrees(xc / BANK_R), _awn(BANK_DEG)))
    try:
        m.box(kerb, (0, 0, -22), (812, 1060, 80))
        m.box(face, (0, 0, 15), (812, 950, 10))
        draw(k)
    finally:
        m.base = None


def bank_tiles(m):
    """A tiled embankment: dark tiles carrying the decade's shapes in the street's colours. A banded sun in the middle
    length; a disc, a triangle, a zigzag and a block of squares either side."""
    def draw(k):
        if k == 0:
            _disc(m, AMBER, 330, 18, 28, at=(0, 40), steps=20)
            for j, (y, t) in enumerate(((-30, 30), (-110, 40), (-200, 54))):
                m.box(NAVY, (0, y, 26), (700, t, 8))
            for j, c in enumerate((MAG, CYAN)):
                m.box(c, (0, -360 - j * 60, 23), (812, 30, 8))
        else:
            a, b, c = ((MAG, AMBER, CYAN), (TEAL, MAG, AMBER))[0 if k < 0 else 1]
            _disc(m, a, 190, 18, 28, at=(-k * 150, 230), steps=16)
            m.plan(b, _ccw([(k * 40, -330), (k * 330, -330), (k * 185, -60)]), 18, 27)
            for j in range(4):
                m.box(c, (-k * 250 + j * k * 95, -200 + (j % 2) * 70, 23), (150, 30, 8), rot=(0, 38 if j % 2 else -38, 0))
            for i in range(3):
                for j in range(2):
                    m.box(CREAM, (k * 150 + i * 80, 150 + j * 80, 23), (46, 46, 8))
    for k in (-1, 0, 1):
        _bank_len(m, k, STONE_L, NAVY, draw)


def syn_bank(m):
    """The Syndicate's embankment: black plates, the gold double chevron on every length, a red line top and bottom.
    Unlit: the gold and the red are paint here."""
    def draw(k):
        for ya in (300, 40):
            for s in (-1, 1):
                m.box(GOLD, (s * 150, ya - 126, 23), (400, 64, 8), rot=(0, -s * 40.0, 0))
        for s in (-1, 1):
            m.box(RED, (0, s * 440, 23), (812, 36, 8))
    for k in (-1, 0, 1):
        _bank_len(m, k, GREY_D, BLACK, draw)


def blade_sign(m, word='HOTEL', col=MAG, trim=CYAN, base=1150.0):
    """A blade sign on a tower's front, over its podium: a dark blade standing out towards the avenue from 11.5 m up,
    the word stacked in lights on both faces (it is read along the street, from either way), a tube of light down each
    edge, a gold ball on top. Lit and STEADY (nothing on it blinks), and stood 31.8 m out, so its lights are 30 m and
    more from the road's centre line. Its mast is inside the podium."""
    lh = 150.0
    H = len(word) * lh * 1.3 + 160.0
    yc = -40.0
    m.box(NAVY, (0, yc, base + H / 2.0), (36, 250, H))
    for s in (-1, 1):
        m.box(trim, (0, yc + s * 132, base + H / 2.0), (44, 14, H), glow=G2)
        _turned(m, s * 18.0, yc, 0.0, s * 90.0, lambda: _stack(m, word, 0.0, -2.0, base + H - 60.0, lh, col))
    m.sphere(AMBER, (0, yc, base + H + 44), 44, steps=(4, 6), glow=G4)
    for z in (base + H * 0.2, base + H * 0.8):
        m.box(GREY_D, (0, yc + 205, z), (20, 170, 20))
    m.box(GREY_D, (0, 110, base / 2.0 + H * 0.4), (26, 26, base + H * 0.8))


W = 300.0     # the street wall's foundation
JOBS = [
    ('SM_BladeHotel', lambda m: blade_sign(m, 'HOTEL', MAG, CYAN)), ('SM_BladeClub', lambda m: blade_sign(m, 'CLUB', CYAN, MAG)),
    ('SM_BladeDiner', lambda m: blade_sign(m, 'DINER', AMBER, MAG)),
    ('SM_FloorCross', floor_cross), ('SM_FloorLot', floor_lot), ('SM_FloorPlaza', floor_plaza), ('SM_KerbCafe', kerb_cafe),
    ('SM_KerbWorks', kerb_works), ('SM_BankTiles', bank_tiles), ('SM_SynBank', syn_bank),
    ('SM_Shops1', _std(shops, slab=W)),
    ('SM_Shops2', _std(lambda m: shops(m, ('BAR', 'HATS', 'LOANS'), (TAN, BRICK_D, STONE), ((MAG, CREAM), (BLUE, CREAM), (GREEN_D, CREAM)),
                                     (MAG, AMBER, CYAN), tall=(0, 2), seed=2), slab=W)),
    ('SM_Shops3', _std(diner_row, slab=W)),
    ('SM_Marquee', _std(marquee, slab=W)), ('SM_BankFront', _std(bank_front, slab=W)), ('SM_PentEntry', _std(pent_entry, slab=W)),
    ('SM_GarageRamp', _std(garage, slab=W)), ('SM_Alley', _std(alley, slab=W)), ('SM_ServiceYard', _std(service_yard, slab=W)),
    ('SM_SynGate', _std(syn_gate, slab=W)),
    ('SM_ParkedCars', _std(parked_cars, foot=True)),
    ('SM_ParkedCars2', _std(lambda m: parked_cars(m, (RED, SILVER, BLUE), ('sedan', 'sedan', 'taxi')), foot=True)),
    ('SM_SynCars', _std(syn_cars, foot=True)), ('SM_Sidewalk', _std(sidewalk, foot=True)), ('SM_BusStop', _std(bus_stop, foot=True)),
    ('SM_SynBarriers', _std(syn_barriers, foot=True)), ('SM_Checkpoint', _std(checkpoint, foot=True)),
    ('SM_SynBanner', _std(syn_banner, foot=True)), ('SM_SearchTower', _std(search_tower, foot=True)),
    ('SM_MidBlock', _std(mid_block)), ('SM_GlassBlock', _std(glass_block)), ('SM_ElRail', _std(el_rail, foot=True)),
    ('SM_Monument', _std(monument, foot=True)), ('SM_ClockTower', _std(clock_tower)), ('SM_SynHQ', _std(syn_hq)),
    ('SM_FarTowersA', _std(far_towers, foot=True)), ('SM_FarTowersB', _std(lambda m: far_towers(m, seed=12, deco=True), foot=True)),
    ('SM_FarRadio', _std(far_radio, foot=True)), ('SM_FarBridge', _std(far_bridge, foot=True)), ('SM_FarSyndicate', _std(far_syndicate, foot=True)),
]
