"""Bayshore, the beach town of level 1: pastel art-deco hotels, palms, lifeguard towers, beach
umbrellas, street lamps, a sunset billboard, and the far skyline with a pier Ferris wheel and a
lighthouse. Origin of each prop: middle of its base on the ground; its front faces -X (the road
camera looks along +X), so it reads from the chase camera."""
import math
import unreal
import ft_art

PINK = '#ff9ec7'
HOTPINK = '#ff4fb8'
MINT = '#7fe3c4'
LAV = '#b9a4ff'
PEACH = '#ffc59e'
CREAM = '#fff1d6'
CORAL = '#ff7a8a'
TEAL = '#2fc6c0'
SKY = '#8fe3ff'
GLASS = '#4a7fc0'
GLASS2 = '#6fd0f0'
WHITE = '#fffaf2'
TRUNK = '#c08a5a'
TRUNK_D = '#9a6a44'
LEAF = '#2fbf71'
LEAF_D = '#1f9a62'
YEL = '#ffe14d'
NAVY = '#2b2350'
SAND = '#ffe2a8'
RED = '#ff4d5e'


def palm(m, h=900, lean=12, seed=0, x=0, y=0):
    """A curved trunk of tapering rings and a crown of drooping fronds."""
    n = 9
    px, pz = x, 0.0
    a = math.radians(lean)
    for k in range(n):
        t = k / float(n)
        r0 = 34 - 16 * t
        seg = h / n
        ang = a * (0.3 + t * 1.2)
        m.cyl(TRUNK if k % 2 == 0 else TRUNK_D, (px, y, pz), r0, seg + 6, rot=(-math.degrees(ang), 0, 0), steps=10, top_r=r0 - 2)
        px += math.sin(ang) * seg
        pz += math.cos(ang) * seg
    top = (px, y, pz)
    for k in range(9):
        yaw = k * 40 + seed * 17
        for j, (dist, drop, ln) in enumerate([(0, -8, 190), (150, -40, 170), (290, -95, 140)]):
            yr = math.radians(yaw)
            cx = top[0] + math.cos(yr) * (dist + ln / 2) * 0.95
            cy = top[1] + math.sin(yr) * (dist + ln / 2) * 0.95
            m.box(LEAF if (k + j) % 2 == 0 else LEAF_D, (cx, cy, top[2] + drop), (ln, 70 - j * 16, 8), rot=(-(12 + j * 18), yaw, 0))
    for k in range(3):
        aa = k * 2.1
        m.sphere('#7a4b2a', (top[0] + math.cos(aa) * 26, top[1] + math.sin(aa) * 26, top[2] - 24), 20, steps=(6, 8))


class Wall(object):
    """One upright wall of a building, addressed the way a facade is drawn: u along it (toward the viewer's right,
    seen from outside), z up from the ground, out = centimetres proud of the wall. `origin` is the wall's bottom-left
    corner seen from outside; `normal` points out of it. The four walls of a box body come from walls()."""

    def __init__(self, m, glass, origin, right, normal, width):
        self.m, self.g, self.o, self.r, self.n, self.width = m, glass, origin, right, normal, width
        self.yaw = math.degrees(math.atan2(normal[1], normal[0]))

    def pt(self, u, out, z):
        return (self.o[0] + self.r[0] * u + self.n[0] * out, self.o[1] + self.r[1] * u + self.n[1] * out, self.o[2] + z)

    def box(self, col, u, z, su, sz, out0, out1, glow=0.0):
        """A box su wide and sz high, centred at (u, z), from out0 to out1 proud of the wall (negative = let in)."""
        self.m.box(col, self.pt(u, (out0 + out1) / 2.0, z), (abs(out1 - out0), su, sz), rot=(0, self.yaw, 0), glow=glow)

    def glass(self, u0, z0, width, height, panes, floor, out=3.0, room=True, pane0=0):
        """A band of panes from (u0, z0), its bottom-left corner."""
        self.g.band(self.pt(u0, out, z0), self.r, (0, 0, 1), width, height, panes, floor, pane0=pane0, room=room)

    def disc(self, col, u, z, r, out0, out1, steps=16, glow=0.0):
        """A round plate on the wall (a porthole's rim, a clock, a vent), its axis along the wall's normal."""
        self.m.cyl(col, self.pt(u, out0, z), r, out1 - out0, rot=(90, math.degrees(math.atan2(-self.n[1], -self.n[0])), 0), steps=steps, glow=glow)

    def porthole(self, u, z, r, floor, pane0=0, rim=WHITE):
        self.disc(rim, u, z, r + 14, 0, 9)
        self.g.disc(self.pt(u, 10.5, z), self.r, (0, 0, 1), r, floor, pane0=pane0)


def walls(m, glass, w, d):
    """The four walls of a box body standing on x 0..d, y -w/2..w/2 (front toward -X, the road toward -Y):
    front, road side, back, far side."""
    return (Wall(m, glass, (0, -w / 2.0, 0), (0, 1, 0), (-1, 0, 0), w),
            Wall(m, glass, (d, -w / 2.0, 0), (-1, 0, 0), (0, -1, 0), d),
            Wall(m, glass, (d, w / 2.0, 0), (0, -1, 0), (1, 0, 0), w),
            Wall(m, glass, (0, w / 2.0, 0), (1, 0, 0), (0, 1, 0), d))


FH, BASE = 320, 120       # a floor's height; the plinth under the first floor
SILL, WIN = 85, 130       # a window band starts SILL above its floor and is WIN high
GLOW = 0.8                # every tube on a building (one neon section per colour)

# Block capitals for the neon signs: strokes (x0, y0, x1, y1) on a 4 x 6 grid, y up. Drawn here, not a font.
GLYPHS = {
    'A': [(0, 0, 0, 4), (0, 4, 2, 6), (2, 6, 4, 4), (4, 4, 4, 0), (0, 2.4, 4, 2.4)],
    'B': [(0, 0, 0, 6), (0, 6, 3, 6), (3, 6, 4, 5), (4, 5, 4, 3.6), (4, 3.6, 3, 3), (0, 3, 3, 3), (3, 3, 4, 2.4), (4, 2.4, 4, 1), (4, 1, 3, 0), (3, 0, 0, 0)],
    'C': [(4, 6, 0, 6), (0, 6, 0, 0), (0, 0, 4, 0)],
    'D': [(0, 0, 0, 6), (0, 6, 3, 6), (3, 6, 4, 5), (4, 5, 4, 1), (4, 1, 3, 0), (3, 0, 0, 0)],
    'E': [(4, 6, 0, 6), (0, 6, 0, 0), (0, 0, 4, 0), (0, 3, 3, 3)],
    'G': [(4, 6, 0, 6), (0, 6, 0, 0), (0, 0, 4, 0), (4, 0, 4, 2.8), (4, 2.8, 2, 2.8)],
    'H': [(0, 0, 0, 6), (4, 0, 4, 6), (0, 3, 4, 3)],
    'I': [(2, 0, 2, 6), (0.8, 0, 3.2, 0), (0.8, 6, 3.2, 6)],
    'K': [(0, 0, 0, 6), (4, 6, 0, 3), (0, 3, 4, 0)],
    'L': [(0, 6, 0, 0), (0, 0, 4, 0)],
    'M': [(0, 0, 0, 6), (0, 6, 2, 3), (2, 3, 4, 6), (4, 6, 4, 0)],
    'N': [(0, 0, 0, 6), (0, 6, 4, 0), (4, 0, 4, 6)],
    'O': [(0, 0, 0, 6), (0, 6, 4, 6), (4, 6, 4, 0), (4, 0, 0, 0)],
    'P': [(0, 0, 0, 6), (0, 6, 4, 6), (4, 6, 4, 3), (4, 3, 0, 3)],
    'R': [(0, 0, 0, 6), (0, 6, 4, 6), (4, 6, 4, 3), (4, 3, 0, 3), (1.6, 3, 4, 0)],
    'S': [(4, 6, 0, 6), (0, 6, 0, 3), (0, 3, 4, 3), (4, 3, 4, 0), (4, 0, 0, 0)],
    'T': [(0, 6, 4, 6), (2, 6, 2, 0)],
    'U': [(0, 6, 0, 0), (0, 0, 4, 0), (4, 0, 4, 6)],
    'V': [(0, 6, 2, 0), (2, 0, 4, 6)],
    'Y': [(0, 6, 2, 3), (4, 6, 2, 3), (2, 3, 2, 0)],
}
_ROLL = []


def _roll_sign():
    """+1 if a positive roll tips the Y axis down (so a stroke climbing to the right needs a positive roll)."""
    if not _ROLL:
        y = unreal.MathLibrary.get_right_vector(unreal.Rotator(roll=90.0, pitch=0.0, yaw=0.0))
        _ROLL.append(1.0 if y.z < 0 else -1.0)
    return _ROLL[0]


def text(wall, s, uc, z0, h, col, out0, out1, glow=GLOW, stack=False):
    """Neon block capitals on a wall, h high: centred on u = uc with their feet at z0, or stacked downward from z0
    (stack=True, a blade sign). Each stroke is one tube."""
    unit = h / 6.0
    t = unit * 0.8
    adv = 5.6 * unit
    u = uc - (len(s) * adv - 1.6 * unit) / 2.0
    z = z0
    for i, ch in enumerate(s):
        if stack:
            u, z = uc - 2.0 * unit, z0 - (i + 1) * h * 1.28
        for x0, y0, x1, y1 in GLYPHS.get(ch, ()):
            du, dz = (x1 - x0) * unit, (y1 - y0) * unit
            roll = _roll_sign() * math.degrees(math.atan2(dz, du))
            wall.m.box(col, wall.pt(u + (x0 + x1) / 2.0 * unit, (out0 + out1) / 2.0, z + (y0 + y1) / 2.0 * unit),
                       (abs(out1 - out0), math.hypot(du, dz) + t, t), rot=(0, wall.yaw, roll), glow=glow)
        u += adv


def _bands(wall, floors, u0, u1, col, pane0=0, rich=True, brow=48, pane=215, first=0):
    """Window bands up a wall between u0 and u1, from floor `first`: the glass, an eyebrow ledge over each band and a
    sill under it. A rich wall also gets mullions between the panes; on a plain one the glass draws its own frames."""
    width = u1 - u0
    n = max(1, int(round(width / float(pane))))
    um = (u0 + u1) / 2.0
    for f in range(first, floors):
        z = BASE + f * FH
        wall.glass(u0, z + SILL, width, WIN, n, f, pane0=pane0)
        wall.box(WHITE, um, z + SILL + WIN + 21, width + 110, 18, 0, brow)
        if rich:
            wall.box(WHITE, um, z + SILL - 6, width + 50, 9, 0, 12)
            for k in range(1, n):
                wall.box(col, u0 + k * width / n, z + SILL + WIN / 2.0, 26, WIN + 4, 2, 10)


def hotel(m, w=1800, d=1200, floors=7, col=PINK, trim=MINT, neon=HOTPINK, tower=True, sign=YEL, name='', blade='HOTEL', balcony=False):
    """An art-deco beach hotel, dressed on every side: eyebrow ledges over window bands, a glass-block stair strip
    and a neon blade sign on the side that faces the road, a canopy over the lobby, a parapet and the roof's
    clutter, and either a rounded corner tower with a stepped, neon-ringed cap or a stepped pylon over the middle bay.
    Its windows are real glass (ft_buildings.Panes, M_BayGlass: a lit room behind every pane) and its neon is real
    neon (the 'neon' part, M_BayNeon); the walls stay palette paint, so the cel pass still bands them.
    name: the roof sign's word; blade: the word stacked down the blade sign; balcony: balconies on the road side."""
    import ft_buildings
    glass = ft_buildings.panes(m)
    NEON = ft_buildings.NEON_PART
    H = floors * FH + BASE
    front, road, back, far = walls(m, glass, w, d)
    m.box(col, (d / 2.0, 0, H / 2.0), (d, w, H))
    for wl in (front, road, back, far):
        wl.box(trim, wl.width / 2.0, BASE / 2.0, wl.width, BASE, 0, 12)             # the plinth
        wl.box(trim, wl.width / 2.0, H + 28, wl.width, 56, -26, 0)                  # the parapet

    # ---- the front: ribbon windows, the lobby under its canopy
    m0 = 0.08 * w
    f0, f1 = m0, (w - 300 if tower else w - m0)       # the tower stands on the front's far corner
    fc = (f0 + f1) / 2.0
    if tower:
        _bands(front, floors, f0, f1, col, pane0=0, first=1)
        lobby = min(520.0, (f1 - f0) * 0.42)
    else:
        # no tower: a middle bay between two fins that climb past the roof into a stepped pylon
        bay = min(0.3 * w, 520.0)
        lobby = bay - 70
        _bands(front, floors, f0, fc - bay / 2.0 - 50, col, pane0=0, first=1)
        _bands(front, floors, fc + bay / 2.0 + 50, f1, col, pane0=20, first=1)
        for s in (-1, 1):
            front.box(trim, fc + s * bay / 2.0, (H + 330) / 2.0, 44, H + 330, 0, 72)
            front.box(WHITE, fc + s * 62, (BASE + 330 + H) / 2.0, 14, H - BASE - 330, 0, 14)
        front.glass(fc - 48, BASE + 250, 96, H - BASE - 330, 1, 50, room=False)           # the stair hall's glass block
        for k, (wd, z0, z1) in enumerate(((bay, 0, 230), (bay * 0.62, 230, 360), (bay * 0.3, 360, 470))):
            front.box(trim if k != 1 else col, fc, H + (z0 + z1) / 2.0, wd, z1 - z0, -170, 34)
        m.part = NEON
        for s in (-1, 1):
            front.box(neon, fc + s * bay / 2.0, (H + 330 + BASE) / 2.0, 9, H + 330 - BASE, 72, 81, glow=GLOW)
        front.box(neon, fc, H + 474, bay * 0.3 + 24, 9, -60, -51, glow=GLOW)
        m.part = None
        if name:
            front.box(NAVY, fc, H + 115, bay - 30, 190, 34, 38)
            m.part = NEON
            text(front, name, fc, H + 50, min(130.0, (bay - 70) / (len(name) * 0.933 - 0.27)), sign, 38, 47)
            m.part = None
    # the ground floor is the lobby level: tall glass under a canopy, shopfronts either side, a white band over it
    front.box(trim, fc, 348, lobby + 170, 26, 0, 250)                               # the canopy
    front.box(WHITE, fc, 330, lobby + 190, 10, 0, 262)
    front.glass(fc - lobby / 2.0, 14, lobby, 300, 1, -1, out=13.0)                  # the lobby: one room, always lit
    for k in (1, 2, 3):
        front.box(WHITE, fc - lobby / 2.0 + k * lobby / 4.0, 164, 12, 300, 13, 19)  # its door frames
    for s in (-1, 1):
        front.box(WHITE, fc + s * (lobby / 2.0 + 16), 168, 32, 336, 0, 22)
        a, b = (f0, fc - lobby / 2.0 - 90) if s < 0 else (fc + lobby / 2.0 + 90, f1)
        if b - a > 150:
            front.glass(a, 70, b - a, 230, max(1, int(round((b - a) / 260.0))), -2, out=13.0, pane0=33 + 3 * s)
            front.box(WHITE, (a + b) / 2.0, 62, b - a + 30, 14, 0, 18)
    front.box(WHITE, (f0 + f1) / 2.0, BASE + FH - 2, f1 - f0 + 110, 22, 0, 30)
    m.part = NEON
    front.box(neon, fc, 348, lobby + 170, 9, 250, 259, glow=GLOW)                   # a tube along the canopy's edge
    front.box(neon, (f0 + f1) / 2.0, H - 30, f1 - f0 + 110, 9, 2, 11, glow=GLOW)    # and under the parapet
    m.part = None

    # ---- the side that looks at the road (u runs from the back corner to the front one)
    stair = 0.08 * d + 150                               # the stair strip's far edge
    _bands(road, floors, stair + 110, d - 0.08 * d, col, pane0=40)
    if balcony:
        # a balcony to every floor above the lobby: a slab, a solid front and two cheeks, shading the band below it
        b0, b1 = stair + 80, d - 0.08 * d - 60           # stops short of the blade sign at the front corner
        for f in range(1, floors):
            z = BASE + f * FH
            road.box(WHITE, (b0 + b1) / 2.0, z + 22, b1 - b0, 14, 0, 135)
            road.box(trim, (b0 + b1) / 2.0, z + 76, b1 - b0, 94, 126, 135)
            for u in (b0 + 5, b1 - 5):
                road.box(trim, u, z + 76, 10, 94, 0, 126)
    road.glass(0.08 * d, BASE + 40, 150, H - BASE - 110, 1, 51, room=False)
    for u in (0.08 * d - 22, stair + 22):
        road.box(trim, u, (BASE + H) / 2.0, 30, H - BASE, 0, 34)
    road.box(trim, 0.08 * d + 75, 268, 300, 20, 0, 150)                            # the side door and its canopy
    road.box(NAVY, 0.08 * d + 75, 118, 116, 230, 3, 16)
    m.part = NEON
    road.box(neon, d / 2.0, H - 30, d * 0.84 + 110, 9, 2, 11, glow=GLOW)
    m.part = None
    if blade:
        # a blade sign near the front corner, standing out toward the road, read by whoever drives up to it
        bh = min(118.0, (0.62 * H) / (len(blade) * 1.28 + 0.4))
        bw, top = bh * 0.667 + 90, 0.92 * H
        bz = top - (len(blade) * 1.28 + 0.4) * bh
        m.box(NAVY, (90, -w / 2.0 - bw / 2.0 - 14, (top + bz) / 2.0), (22, bw, top - bz))
        for z in (top - 10, bz + 10):
            m.box(trim, (90, -w / 2.0 - 12, z), (14, 30, 14))
        face = Wall(m, glass, (79, -w / 2.0 - 14 - bw, 0), (0, 1, 0), (-1, 0, 0), bw)
        m.part = NEON
        text(face, blade, bw / 2.0, top - 0.12 * bh, bh, neon, 0, 9, stack=True)
        for s in (0, 1):
            face.box(sign, s * bw, (top + bz) / 2.0, 8, top - bz, 0, 8, glow=GLOW)
        m.part = None

    # ---- the back and the far side: plain, but nothing is left blank
    _bands(back, floors, 0.08 * w, 0.92 * w, col, pane0=60, rich=False, brow=22)
    _bands(far, floors, (300 if tower else 0.08 * d), 0.92 * d, col, pane0=80, rich=False, brow=22)

    # ---- the roof: what a hotel keeps up there
    for x, y in ((d * 0.84, -w * 0.27), (d * 0.84, w * 0.06)):
        m.box(WHITE, (x, y, H + 62), (230, 170, 124))                               # air handlers, clear of the head house
        m.box(NAVY, (x - 117, y, H + 66), (4, 130, 70))
        m.cyl(NAVY, (x, y, H + 124), 58, 10, steps=12)
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(NAVY, (d * 0.8 + sx * 70, w * 0.32 + sy * 70, H + 85), (12, 12, 170))    # the water tank's legs
    m.cyl(trim, (d * 0.8, w * 0.32, H + 170), 118, 210, steps=14)
    m.cyl(WHITE, (d * 0.8, w * 0.32, H + 380), 124, 46, steps=14, top_r=14)
    hy = -w * 0.38
    m.box(col, (d * 0.52, hy, H + 130), (300, 250, 260))                           # the stair's head house
    m.box(NAVY, (d * 0.52 - 152, hy, H + 105), (4, 90, 200))
    m.box(WHITE, (d * 0.52, hy, H + 266), (330, 280, 14))
    if not tower:
        m.cyl(NAVY, (d * 0.52, hy, H + 273), 5, 460, steps=6)                      # the aerial and its warning light
        for k in range(3):
            m.box(NAVY, (d * 0.52, hy, H + 520 + k * 60), (6, 130 - k * 34, 6))
        m.part = NEON
        m.sphere(RED, (d * 0.52, hy, H + 745), 16, glow=1.0)
        m.part = None

    if tower:
        # stepped crown with the roof sign, facing the way the cars come
        m.box(trim, (d * 0.42, -w * 0.02, H + 60), (d * 0.5, w * 0.5, 120))
        m.box(col, (d * 0.42, -w * 0.02, H + 160), (d * 0.34, w * 0.34, 80))
        sw = w * 0.5
        roof = Wall(m, glass, (d * 0.17, -w * 0.02 - sw / 2.0, H), (0, 1, 0), (-1, 0, 0), sw)
        roof.box(NAVY, sw / 2.0, 245, sw, 210, -22, 0)
        for u in (sw * 0.16, sw * 0.84):
            roof.box(NAVY, u, 70, 14, 140, -18, -4)
        m.part = NEON
        if name:
            text(roof, name, sw / 2.0, 180, min(140.0, (sw - 60) / (len(name) * 0.933 - 0.27)), sign, 0, 9)
        roof.box(neon, sw / 2.0, 356, sw + 16, 9, -6, 3, glow=GLOW)
        roof.box(neon, sw / 2.0, 134, sw + 16, 9, -6, 3, glow=GLOW)
        m.part = None
        # the corner tower: glass block ring by ring, a stepped cap in neon hoops, the spire
        ty = w / 2.0
        m.cyl(trim, (0, ty, 0), 260, H + 250, steps=20)
        for f in range(floors):
            _tower_glass(glass, (0, ty), 266, BASE + f * FH + 90, 110, 20, f)
            m.cyl(WHITE, (0, ty, BASE + f * FH + 230), 290, 16, steps=20)
        m.cyl(WHITE, (0, ty, H + 250), 284, 18, steps=20)
        m.cyl(col, (0, ty, H + 268), 214, 110, steps=20)
        m.cyl(trim, (0, ty, H + 378), 160, 90, steps=20)
        m.cyl(col, (0, ty, H + 468), 100, 70, steps=16, top_r=56)
        m.part = NEON
        for z, r in ((H + 300, 221), (H + 340, 221), (H + 410, 167)):
            m.cyl(neon, (0, ty, z), r, 9, steps=20, glow=GLOW)
        m.cyl(neon, (0, ty, H + 538), 20, 300, steps=8, glow=GLOW)
        m.sphere(sign, (0, ty, H + 870), 44, glow=GLOW)
        m.part = None


def _tower_glass(glass, centre, r, z, h, steps, floor):
    """A ring of flat panes round the corner tower (one per facet of its cylinder), skipping the quarter that sits
    inside the building (the tower's centre is the body's front right corner, seen from the road)."""
    cx, cy = centre
    for k in range(steps):
        a0 = 2 * math.pi * (k + 1) / steps      # seen from outside, the viewer's right is toward the smaller angle
        a1 = 2 * math.pi * k / steps
        am = (a0 + a1) / 2
        if math.cos(am) > 0.05 and math.sin(am) < -0.05:
            continue
        p0 = (cx + r * math.cos(a0), cy + r * math.sin(a0), z)
        p1 = (cx + r * math.cos(a1), cy + r * math.sin(a1), z)
        width = math.hypot(p1[0] - p0[0], p1[1] - p0[1])
        right = ((p1[0] - p0[0]) / width, (p1[1] - p0[1]) / width, 0)
        glass.band(p0, right, (0, 0, 1), width, h, 1, floor, pane0=100 + k, room=False)


def _arc_glass(glass, centre, r, deg0, deg1, steps, z, h, floor, pane0=0, room='flat'):
    """Flat panes round an arc from deg0 to deg1 (deg0 < deg1), one per facet, facing out from `centre`. They share
    one pane index, so the whole curve is one room's window and lights as one."""
    cx, cy = centre
    for k in range(steps):
        a0 = math.radians(deg0 + (deg1 - deg0) * (k + 1) / float(steps))     # the viewer's right is the smaller angle
        a1 = math.radians(deg0 + (deg1 - deg0) * k / float(steps))
        p0 = (cx + r * math.cos(a0), cy + r * math.sin(a0), z)
        p1 = (cx + r * math.cos(a1), cy + r * math.sin(a1), z)
        width = math.hypot(p1[0] - p0[0], p1[1] - p0[1])
        glass.band(p0, ((p1[0] - p0[0]) / width, (p1[1] - p0[1]) / width, 0), (0, 0, 1), width, h, 1, floor, pane0=pane0, room=room)


def _corner(w, R, off, steps=10):
    """Plan points round the liner's rounded corner (the front's road end), `off` cm outside the wall line: from
    the front wall round to the road wall."""
    cx, cy = R, -w / 2.0 + R
    return [(cx + (R + off) * math.cos(math.radians(a)), cy + (R + off) * math.sin(math.radians(a)))
            for a in [180.0 + 90.0 * k / steps for k in range(steps + 1)]]


def liner(m, w=1500, d=1000, floors=5, col=MINT, trim=PEACH, neon=HOTPINK, sign=SKY, name='LIDO'):
    """A Streamline Moderne hotel, built like the liners its architects admired: the corner toward the road is
    rounded and the ribbon windows and their ledges run round it without a break, three speed lines race round it
    between the floors, portholes climb the side that looks at the road, the roof has a ship's rail and a round
    pilot house with a mast, and the name is stacked down a pylon at the front's far end. Same footprint as the
    hotel it replaces (front toward -X, the road toward -Y)."""
    import ft_buildings
    glass = ft_buildings.panes(m)
    NEON = ft_buildings.NEON_PART
    H = floors * FH + BASE
    R = 320.0
    front, road, back, far = walls(m, glass, w, d)
    arc = _corner(w, R, 0)
    m.plan(col, arc + [(d, -w / 2.0), (d, w / 2.0), (0, w / 2.0)], 0, H)

    def wrap(colr, off, z0, z1, run_front, run_road, glow=0.0):
        """A band standing `off` proud of the walls, round the corner and `run_*` along each wall from it."""
        outer = [(-off, -w / 2.0 + R + run_front)] + _corner(w, R, off) + [(R + run_road, -w / 2.0 - off)]
        inner = [(R + run_road, -w / 2.0 + 6), (R, -w / 2.0 + 6)] + list(reversed(_corner(w, R, -6)[1:-1])) + [(6, -w / 2.0 + R), (6, -w / 2.0 + R + run_front)]
        m.plan(colr, outer + inner, z0, z1, glow=glow)

    pyl = 250.0                                          # the name's pylon takes the front's far end
    fr, rr = w - R - pyl - 0.06 * w, d - R - 0.2 * d     # how far the ribbons run along the front and the road side
    wrap(trim, 12, 0, BASE, w - R, d - R)                # the plinth
    wrap(WHITE, 6, BASE + FH - 14, BASE + FH + 8, fr + 60, rr + 60)
    for f in range(floors):
        z = BASE + f * FH
        if f > 0:
            nf = max(1, int(round(fr / 215.0)))
            front.glass(R, z + SILL, fr, WIN, nf, f, pane0=0)
            for k in range(1, nf):
                front.box(col, R + k * fr / nf, z + SILL + WIN / 2.0, 26, WIN + 4, 2, 10)
            _arc_glass(glass, (R, -w / 2.0 + R), R + 3, 180, 270, 6, z + SILL, WIN, f, pane0=30)
        nr = max(1, int(round(rr / 215.0)))
        road.glass(d - R - rr, z + SILL, rr, WIN, nr, f, pane0=40)
        for k in range(1, nr):
            road.box(col, d - R - rr + k * rr / nr, z + SILL + WIN / 2.0, 26, WIN + 4, 2, 10)
        # the ledge, all the way round; on the front it ends inside the pylon's near fin (further and it pokes through
        # the name board, which stands less proud than the ledge)
        wrap(WHITE, 48, z + SILL + WIN + 12, z + SILL + WIN + 30, fr + 30, rr + 60)
        if f > 0:
            wrap(WHITE, 12, z + SILL - 10, z + SILL - 1, fr + 30, rr + 40)
        for k in range(3):                                                                    # speed lines
            if z + SILL + WIN + 74 + k * 30 < H - 40:
                wrap(trim, 7, z + SILL + WIN + 62 + k * 30, z + SILL + WIN + 74 + k * 30, 190 - k * 40, 190 - k * 40)
        road.porthole(0.1 * d, z + SILL + WIN / 2.0, 52, f, pane0=60)
    road.box(trim, 0.1 * d, (BASE + H) / 2.0, 150, H - BASE, 0, 5)                           # the portholes' panel
    # ground floor on the front: the lobby (one room, always lit) under a round-ended canopy
    lc = R + fr * 0.5
    front.glass(lc - 230, 14, 460, 300, 1, -1, out=13.0)
    for k in (1, 2, 3):
        front.box(WHITE, lc - 230 + k * 115, 164, 12, 300, 13, 19)
    front.box(trim, lc, 348, 620, 26, 0, 230)
    for s in (-1, 1):
        m.cyl(trim, front.pt(lc + s * 310, 115, 335), 115, 26, steps=16)                     # its rounded ends
        front.box(WHITE, lc + s * 246, 168, 32, 336, 0, 22)
    m.part = NEON
    front.box(neon, lc, 348, 620, 9, 230, 239, glow=GLOW)
    wrap(neon, 9, H - 34, H - 25, fr + 60, rr + 60, glow=GLOW)                               # a tube under the roof line
    m.part = None
    for wl in (back, far):
        wl.box(trim, wl.width / 2.0, BASE / 2.0, wl.width, BASE, 0, 12)
    _bands(back, floors, 0.08 * w, 0.92 * w, col, pane0=70, rich=False, brow=22)
    _bands(far, floors, 0.08 * d, 0.92 * d, col, pane0=90, rich=False, brow=22)

    # ---- the pylon: two fins and the name stacked between them, stepping up past the roof
    pu = w - 0.06 * w - pyl / 2.0
    top = H + 380
    for s in (-1, 1):
        front.box(trim, pu + s * (pyl / 2.0 - 20), top / 2.0, 40, top, 0, 80)
    front.box(NAVY, pu, (BASE + top) / 2.0, pyl - 80, top - BASE, 0, 40)
    front.box(trim, pu, top + 30, pyl * 0.55, 60, 0, 80)
    front.box(WHITE, pu, top + 85, pyl * 0.26, 50, 10, 70)
    lh = min(150.0, (top - 120 - BASE - 220) / (len(name) * 1.28 + 0.2)) if name else 0
    m.part = NEON
    if name:
        text(front, name, pu, top - 110, lh, sign, 40, 49, stack=True)
    for s in (-1, 1):
        front.box(neon, pu + s * (pyl / 2.0 - 20), (BASE + top) / 2.0, 9, top - BASE, 80, 89, glow=GLOW)
    m.part = None

    # ---- the roof: a ship's rail, the pilot house, a mast
    rail = [(-4, w / 2.0 - 0.06 * w - pyl)] + _corner(w, R, 4) + [(d - 30, -w / 2.0 - 4)]
    for z in (H + 44, H + 86):
        for a, b in zip(rail[:-1], rail[1:]):
            L = math.hypot(b[0] - a[0], b[1] - a[1])
            m.box(WHITE, ((a[0] + b[0]) / 2.0, (a[1] + b[1]) / 2.0, z), (L + 5, 6, 6), rot=(0, math.degrees(math.atan2(b[1] - a[1], b[0] - a[0])), 0))
    for k, (x, y) in enumerate(rail):
        m.box(WHITE, (x, y, H + 45), (6, 6, 90))
    for u in range(1, 5):
        m.box(WHITE, (-4, -w / 2.0 + R + u * (w - R - 0.06 * w - pyl - 10) / 5.0, H + 45), (6, 6, 90))
        m.box(WHITE, (R + u * (d - R - 40) / 5.0, -w / 2.0 - 4, H + 45), (6, 6, 90))
    px, py = d * 0.42, -w * 0.12
    m.cyl(WHITE, (px, py, H), 250, 230, steps=20)
    m.cyl(trim, (px, py, H + 230), 290, 16, steps=20)
    m.cyl(col, (px, py, H + 246), 150, 60, steps=16)
    _arc_glass(glass, (px, py), 253, 0, 360, 20, H + 90, 100, floors + 1, pane0=120, room='flat')
    m.cyl(NAVY, (px, py, H + 306), 6, 520, steps=6)
    m.box(NAVY, (px, py, H + 660), (6, 190, 6))
    m.box(CORAL, (px, py - 70, H + 770), (4, 130, 80))                                       # the house flag
    m.part = NEON
    m.sphere(RED, (px, py, H + 838), 16, glow=1.0)
    m.cyl(neon, (px, py, H + 232), 294, 9, steps=20, glow=GLOW)
    m.part = None
    for x, y in ((d * 0.82, w * 0.2), (d * 0.82, -w * 0.3)):
        m.box(WHITE, (x, y, H + 62), (230, 170, 124))                                        # air handlers
        m.box(NAVY, (x - 117, y, H + 66), (4, 130, 70))
        m.cyl(NAVY, (x, y, H + 124), 58, 10, steps=12)
    for wl in (back, far):
        wl.box(trim, wl.width / 2.0, H + 28, wl.width, 56, -26, 0)


def lifeguard(m, col=CORAL, col2=WHITE):
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(WHITE, (sx * 90, sy * 90, 150), (16, 16, 300))
    m.box(col2, (0, 0, 300), (260, 260, 16))
    m.box(col, (0, 0, 420), (220, 220, 220))
    for k in range(4):
        m.box(col2 if k % 2 else col, (-112, -80 + k * 54, 420), (4, 50, 220))
    m.box(SKY, (-114, 0, 450), (4, 150, 80))
    m.plan(TEAL, [(-150, -150), (150, -150), (150, 150), (-150, 150)], 530, 548)
    m.cyl(TEAL, (0, 0, 548), 150, 70, steps=4, top_r=10, rot=(0, 45, 0))
    m.side(WHITE, [(-200, 0), (-130, 300), (-110, 300), (-180, 0)], -40, 40)
    m.cyl(WHITE, (0, 0, 610), 4, 220, steps=6)
    m.side(RED, [(0, 830), (90, 805), (0, 780)], -2, 2)


def umbrella(m, col=HOTPINK, col2=WHITE, x=0, y=0):
    m.cyl(WHITE, (x, y, 0), 4, 230, steps=6, rot=(-8, 0, 0))
    m.cyl(col, (x + 30, y, 200), 170, 60, steps=8, top_r=8)
    m.cyl(col2, (x + 30, y, 196), 172, 8, steps=8)
    m.box(col2 if col2 != WHITE else SKY, (x - 20, y + 120, 2), (200, 90, 4))
    m.box(col, (x - 20, y + 120, 5), (200, 20, 4))


def lamp(m, flip=1):
    m.cyl(NAVY, (0, 0, 0), 14, 30, steps=8)
    m.cyl(TEAL, (0, 0, 30), 7, 700, steps=8)
    m.box(TEAL, (0, flip * 120, 720), (10, 250, 10))
    m.box(NAVY, (0, flip * 240, 706), (40, 70, 22))
    m.box(YEL, (0, flip * 240, 694), (34, 60, 4), glow=0.8)
    m.box(HOTPINK, (-8, 0, 420), (4, 40, 180), glow=0.5)


def billboard(m):
    """A sunset billboard: striped 80s sun, sea, a palm silhouette. Faces -X."""
    W, H, z0 = 1200, 600, 560
    for y in (-420, 420):
        m.box(NAVY, (40, y, z0 / 2), (30, 30, z0 + 40))
    m.box(NAVY, (10, 0, z0 + H / 2), (20, W + 40, H + 40))
    bands = ['#ffd2f0', '#ffb3e0', '#ff9ec7', '#ff8bb0', '#ff7a9a']
    for k, b in enumerate(bands):
        m.box(b, (-2, 0, z0 + H - (k + 0.5) * H * 0.6 / len(bands)), (4, W, H * 0.6 / len(bands) + 1))
    m.box(TEAL, (-2, 0, z0 + H * 0.2), (4, W, H * 0.4))
    m.box(SKY, (-3, 0, z0 + H * 0.3), (4, W, 10))
    m.box(SKY, (-3, 0, z0 + H * 0.15), (4, W * 0.8, 8))
    # the sun: a disc cut by stripes
    m.cyl(YEL, (-4, 0, z0 + H * 0.42), 170, 4, rot=(90, 0, 0), steps=28, glow=0.4)
    for k in range(4):
        m.box(bands[3], (-7, 0, z0 + H * 0.45 + k * 26), (4, 360, 8 - k * 1.5))
    m.box(TEAL, (-6, 0, z0 + H * 0.31), (4, 400, H * 0.12))
    m.box(NAVY, (-8, 380, z0 + H * 0.5), (4, 24, H * 0.6))
    for k in range(5):
        m.box(NAVY, (-8, 380 + (k - 2) * 40, z0 + H * 0.82 - abs(k - 2) * 20), (4, 120, 16), rot=(0, 0, (k - 2) * 25))
    m.box(HOTPINK, (-14, 0, z0 - 6), (6, W + 40, 14), glow=0.9)
    m.box(SKY, (-14, 0, z0 + H + 6), (6, W + 40, 14), glow=0.9)


AD_W, AD_H, AD_Z0 = 1200, 600, 560  # the printed face: same size and height as billboard()'s
AD_FACE_X = -22                       # the face plane, 22 cm in front of the backing (front is -X)


def ad_billboard(m, accent=YEL):
    """A printed billboard (2.5): billboard()'s navy posts and backing, a cream frame lip round the printed face,
    a catwalk whose fascia carries the brand colour, three gooseneck lamps. The face itself is a separate
    textured section (ft_billboards.face_mesh) at x = AD_FACE_X, recessed 4 cm inside the lip so its edges never
    show or z-fight. Origin = middle of the base; the track sets it 20 cm under the ground, so the white
    footings stand 40 cm proud of it. Faces -X."""
    W, H, z0 = AD_W, AD_H, AD_Z0
    for y in (-420, 420):
        m.cyl(WHITE, (40, y, -60), 60, 120, steps=12)              # concrete footing
        m.box(NAVY, (40, y, z0 / 2), (40, 40, z0 + 80))            # steel post, buried 20 cm
    m.box(NAVY, (40, 0, z0 * 0.55), (30, 840, 30))                 # cross brace between the posts
    m.box(NAVY, (10, 0, z0 + H / 2), (20, W + 40, H + 40))          # backing panel, x 0..20
    # cream frame lip, x -26..0, sitting just outside the face's edges
    m.box(CREAM, (-13, 0, z0 + H + 15), (26, W + 60, 30))
    m.box(CREAM, (-13, 0, z0 - 15), (26, W + 60, 30))
    for s in (-1, 1):
        m.box(CREAM, (-13, s * (W / 2 + 15), z0 + H / 2), (26, 30, H))
    # catwalk under the face, its front painted in the advertiser's colour
    m.box(NAVY, (-80, 0, z0 - 70), (160, W, 10))
    m.box(accent, (-163, 0, z0 - 60), (6, W, 34))
    for y in (-400, 0, 400):
        m.box(NAVY, (-150, y, z0 - 46), (8, 8, 40))                # lamp stem
        m.box(NAVY, (-132, y, z0 - 20), (48, 56, 14))              # lamp hood, kept below the face's bottom edge
        m.box(YEL, (-132, y, z0 - 28.5), (40, 44, 3), glow=0.5)    # the lit lens, a small accent


def ferris(m, R=1500):
    """The pier Ferris wheel, in the YZ plane so it faces the road camera."""
    cz = R + 200
    # the A-frame legs, built as slanted boxes in the YZ plane (roll only)
    for s_ in (-1, 1):
        ang = math.degrees(math.atan2(cz, R * 0.6))
        m.box(WHITE, (40, s_ * R * 0.3, cz / 2), (24, 30, math.hypot(cz, R * 0.6)), rot=(0, 0, -s_ * (90 - ang)))
    n = 16
    for k in range(n):
        a = 2 * math.pi * k / n
        m.box(WHITE, (0, math.cos(a) * R / 2, cz + math.sin(a) * R / 2), (14, R, 14), rot=(0, 0, SPOKE_SIGN * math.degrees(a)))
        m.box([MINT, YEL, SKY, CORAL][k % 4], (-30, math.cos(a) * R, cz + math.sin(a) * R - 150), (150, 150, 150))
    ro = unreal.GeometryScriptRevolveOptions()
    for rr, col in ((R, HOTPINK), (R * 0.55, WHITE)):
        unreal.GeometryScript_Primitives.append_torus(m._mesh(col, 0.3 if col == HOTPINK else 0.0), m.opts, m._t((0, 0, cz), (90, 0, 0)), ro, rr, 24, 32, 6, unreal.GeometryScriptPrimitiveOriginMode.CENTER)
    m.cyl(YEL, (-60, 0, cz), 90, 120, rot=(90, 0, 0), steps=16)


SPOKE_SIGN = -1


def lighthouse(m):
    for k in range(6):
        m.cyl(RED if k % 2 == 0 else WHITE, (0, 0, k * 300), 300 - k * 22, 300, steps=16, top_r=300 - (k + 1) * 22)
    m.cyl(NAVY, (0, 0, 1800), 200, 40, steps=16)
    m.cyl(YEL, (0, 0, 1840), 130, 220, steps=12, glow=0.8)
    m.cyl(RED, (0, 0, 2060), 170, 160, steps=12, top_r=10)


def skyline(m, width=24000, seed=7):
    """A long, tileable strip of far buildings behind the beach. Front at x=0, extends to +X.
    Every floor is a band of panes lit one by one (M_BayGlass, 'flat': no rooms at this distance) on all four faces;
    the same seed gives the same towers as before."""
    import random
    import ft_buildings
    glass = ft_buildings.panes(m)
    r = random.Random(seed)
    cols = [PINK, MINT, LAV, PEACH, CREAM, SKY, CORAL, '#ffd6a5', '#c9f2e3', '#e7d1ff']
    y = -width / 2 + 200
    k = 0
    while y < width / 2 - 1400:
        w = r.choice([600, 800, 1000, 1200])
        h = r.choice([1200, 1800, 2400, 3000, 3600, 4400, 5600])
        col = r.choice(cols)
        x = r.choice([0, 600, 1400, 2400])
        m.box(col, (x + 500, y + w / 2, h / 2), (1000, w, h))
        # all four faces: the road loops back on itself, so every side of the skyline is seen from somewhere
        front, road, back, far = walls(m, glass, w, 1000)
        front.o, road.o, back.o, far.o = (x, y, 0), (x + 1000, y, 0), (x + 1000, y + w, 0), (x, y + w, 0)
        nw = max(2, int(round(0.8 * w / 190.0)))
        for f in range(int(h / 360)):
            z = 150 + f * 360
            front.glass(0.1 * w, z, 0.8 * w, 170, nw, f, room='flat', pane0=k * 32)
            back.glass(0.1 * w, z, 0.8 * w, 170, nw, f, room='flat', pane0=k * 32 + 16)
            road.glass(80, z, 840, 170, 4, f, room='flat', pane0=k * 32 + 8)
            far.glass(80, z, 840, 170, 4, f, room='flat', pane0=k * 32 + 24)
            if f % 2:
                front.box(WHITE, w / 2.0, z + 196, 0.86 * w, 16, 0, 26)
        penthouse = r.random() < 0.4
        if penthouse:
            m.box(col, (x + 500, y + w / 2, h + 200), (600, w * 0.5, 400))
            m.cyl(WHITE, (x + 500, y + w / 2, h + 400), 12, 600, steps=6)
        if r.random() < 0.3:
            m.part = ft_buildings.NEON_PART
            m.box(HOTPINK, (x - 6, y + w / 2, h + 46), (6, w, 28), glow=GLOW)       # along the roof line, above the top windows
            m.part = None
        # roofs seen from the road's loops and bridges: a parapet line and a tank or two
        m.box(WHITE, (x + 500, y + w / 2, h + 14), (1010, w + 10, 28))
        if not penthouse:
            m.cyl(WHITE, (x + 700, y + w * 0.3, h + 28), 90, 170, steps=10)
        y += w + r.choice([0, 100, 300])
        k += 1
