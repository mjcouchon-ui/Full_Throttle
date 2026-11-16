"""Volcano, Stage 1, builder s1d3: district "Engine 13's Road" (8.34 to 11.1 km) = SLAGTOWN, the miners' company town.
/Game/World/Volcano/SM_S1D3_*.   Run: import ft_world_volcano_s1d3 as K; K.run()

Boxy timber and corrugated sheds on STILTS with single-pitch and saw-tooth roofs, verandas, outside stairs and water
tanks on legs, in mustard, cream, petrol teal, cocoa and white trim. Nothing red, no vehicle, no fire station, no hose.
Every mesh is an ft_people.Scene: one material (M_People), one section. Placed by C:/ftwork/s1/L3_s1d3.inl.

  buildings, LEFT of the road (61.6 to 71.2 m out, all inside 70 m at their anchors):
      PayOffice LampRoom TankHouse RowHouses (3.6 m deep or less: they stand between the basalt wall and the crags)
      Bunkhouse CookHouse (4.8 m deep, the open slots)
  buildings, RIGHT of the road (72.5 m out, behind the mine's railway: outside 70 m, so they cast no shadow):
      AssayShed BandHall CompanyStore
  props, low (top under 2 m, still):   SackBarrow PitProps TinBath PickRack
  props, tall and open (24.9 m and more out):   WashLine NoticeBoard SwingSeat StiltLarder
  hero:   Hopper (the 45 m teal ore tower, a pickaxe weathervane turning on top)
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))            # a piece with no moving part: true bounds
SWAY = dict(grow=(30, 40, 30), grow_down=(30, 40, 0))        # a tall prop with a small swinging part

MUS, CRM, TEAL, COCOA, WHT = '#c7a13b', '#f1e3c0', '#1f8a8a', '#5c3324', '#fbf7ee'      # the district's five
PANE, TIN, LIT, SACK, ASH = '#14101a', '#cdc5c8', '#ffe28a', '#b0a088', '#6f6272'       # the kit's: PANE HUT LIT PALE_L ASH_L


# ------------------------------------------------------------------------------------------ helpers
class _Shift(object):
    """A Scene drawn dx further along X: a piece is drawn round its main body and then centred on its footprint
    (stair and all), so the middle of the footprint is the origin."""

    def __init__(self, sc, dx):
        self.sc, self.dx = sc, float(dx)

    def part(self, mo=None, at=(0.0, 0.0, 0.0), yaw=0.0, s=1.0):
        if mo:
            mo = dict(mo)
            mo['pivot'] = (mo['pivot'][0] + self.dx, mo['pivot'][1], mo['pivot'][2])
            mo['base'] = (mo['base'][0] + self.dx, mo['base'][1])
        return self.sc.part(mo, (at[0] + self.dx, at[1], at[2]), yaw, s)

    def still(self, at=(0.0, 0.0, 0.0), yaw=0.0, s=1.0):
        return self.part(None, at, yaw, s)


def centred(fn, dx):
    def f(sc):
        fn(_Shift(sc, dx))
    f.__doc__ = fn.__doc__
    return f


def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def ywedge(g, col, xa, xb, pts):
    """A profile [(y, z)] seen from the end of the piece, run along X from xa to xb."""
    g.side(col, _ccw(pts), -xb, -xa, rot=(0, 90, 0))


def stilts(g, x0, x1, y0, y1, h, nx=3, foot=250.0, t=24.0, brace=True):
    """Cocoa posts under a floor h up: nx along the front and the back, crossed braces in the front bays, the floor."""
    xs = [x0 + t / 2.0 + k * (x1 - x0 - t) / float(nx - 1) for k in range(nx)]
    for x in xs:
        for y in (y0 + t / 2.0, y1 - t / 2.0):
            g.box(COCOA, (x, y, (h - foot) / 2.0), (t, t, h + foot))
    if brace:
        for a, b in zip(xs, xs[1:]):
            tilt_box(g, COCOA, (a, y0 + t / 2.0, h * 0.2), (b, y0 + t / 2.0, h * 0.88), 12, 12)
            tilt_box(g, COCOA, (a, y0 + t / 2.0, h * 0.88), (b, y0 + t / 2.0, h * 0.2), 12, 12)
    g.box(COCOA, ((x0 + x1) / 2.0, (y0 + y1) / 2.0, h - 12.0), (x1 - x0 + 30.0, y1 - y0 + 20.0, 24.0))


def lean(g, col, x0, x1, y0, y1, z0, z_f, z_b, roof=TEAL, over=35.0, t=16.0):
    """Walls from z0 under a single-pitch roof from the front (top z_f) to the back (top z_b)."""
    ywedge(g, col, x0, x1, [(y0, z0), (y1, z0), (y1, z_b), (y0, z_f)])
    s = (z_b - z_f) / float(y1 - y0)
    xc = (x0 + x1) / 2.0
    tilt_box(g, roof, (xc, y0 - over, z_f - s * over + t / 2.0 + 2), (xc, y1 + over, z_b + s * over + t / 2.0 + 2), (x1 - x0) + 2 * over, t)


def shed(g, col, x0, x1, y0, y1, z0, z_a, z_b, roof=TEAL, over=35.0, t=16.0):
    """Walls from z0 under a single-pitch roof along the piece: top z_a at the x0 end, z_b at the x1 end."""
    g.side(col, _ccw([(x0, z0), (x1, z0), (x1, z_b), (x0, z_a)]), y0, y1)
    s = (z_b - z_a) / float(x1 - x0)
    tilt_box(g, roof, (x0 - over, (y0 + y1) / 2.0, z_a - s * over + t / 2.0 + 2), (x1 + over, (y0 + y1) / 2.0, z_b + s * over + t / 2.0 + 2), (y1 - y0) + 2 * over, t)


def sawtooth(g, col, x0, x1, n, z0, rise, y0, y1):
    """A saw-tooth roof seen from the road: n teeth, each upright at its -X side and falling to +X."""
    s = (x1 - x0) / float(n)
    pts = [(x0, z0 - 30.0), (x1, z0 - 30.0), (x1, z0)]
    for k in range(n - 1, -1, -1):
        pts.append((x0 + k * s, z0 + rise))
        if k:
            pts.append((x0 + k * s, z0))
    g.side(col, _ccw(pts), y0, y1)


def ribs(g, col, x0, x1, y, z0, z1, n, w=10.0):
    """Upright ribs on a wall facing the road: corrugated sheet."""
    for k in range(n):
        g.box(col, (x0 + (k + 0.5) * (x1 - x0) / float(n), y, (z0 + z1) / 2.0), (w, 8, z1 - z0))


def window(g, x, y, z, w=70.0, h=90.0, lit=0.0, frame=WHT):
    g.box(frame, (x, y, z), (w + 18, 8, h + 18))
    g.box(LIT if lit else PANE, (x, y - 3, z), (w, 8, h), glow=lit)


def xwindow(g, x, y, z, w=70.0, h=90.0, lit=0.0):
    """A window on an end wall (it faces along the road)."""
    sd = 1.0 if x > 0 else -1.0
    g.box(WHT, (x, y, z), (8, w + 18, h + 18))
    g.box(LIT if lit else PANE, (x + sd * 3, y, z), (8, w, h), glow=lit)


def door(g, x, y, z0, w=90.0, h=190.0, col=TEAL):
    g.box(WHT, (x, y, z0 + h / 2.0 + 6), (w + 20, 8, h + 12))
    g.box(col, (x, y - 3, z0 + h / 2.0), (w, 8, h))
    g.box(MUS if col != MUS else WHT, (x + w * 0.3, y - 8, z0 + h * 0.5), (8, 6, 8))


def stair(g, x0, z0, x1, z1, y, w=90.0, treads=5):
    """An outside stair along the piece from (x0, z0) to (x1, z1): the string, treads, a hand rail on two posts."""
    tilt_box(g, COCOA, (x0, y, z0 - 8), (x1, y, z1 - 8), w, 12)
    for k in range(treads):
        f = (k + 0.5) / float(treads)
        g.box(CRM, (x0 + (x1 - x0) * f, y, z0 + (z1 - z0) * f + 6), (abs(x1 - x0) / float(treads) * 0.8, w, 8))
    tilt_box(g, COCOA, (x0, y - w / 2.0 + 5, z0 + 95), (x1, y - w / 2.0 + 5, z1 + 95), 8, 8)
    for (x, z) in ((x0, z0), (x1, z1)):
        g.box(COCOA, (x, y - w / 2.0 + 5, z + 46), (8, 8, 100))


def veranda(g, x0, x1, y0, y1, z0, h, n, roof=MUS, rail=True):
    """A veranda on the floor at z0: n white posts along the front edge y0, a rail, a lean-to roof up to the wall at y1."""
    for k in range(n):
        g.box(WHT, (x0 + 8 + k * (x1 - x0 - 16) / float(n - 1), y0 + 8, z0 + h / 2.0), (12, 12, h))
    if rail:
        g.box(COCOA, ((x0 + x1) / 2.0, y0 + 8, z0 + 92), (x1 - x0, 8, 10))
        g.box(COCOA, ((x0 + x1) / 2.0, y0 + 8, z0 + 46), (x1 - x0, 6, 8))
    tilt_box(g, roof, ((x0 + x1) / 2.0, y0 - 25, z0 + h - 4), ((x0 + x1) / 2.0, y1 + 4, z0 + h + 44), (x1 - x0) + 50, 12)


def stovepipe(g, x, y, z0, z1, r=20.0, col=TIN, cap=True):
    g.cyl(col, (x, y, z0), r, z1 - z0, steps=6)
    if cap:
        g.cyl(col, (x, y, z1 + 14), r * 2.0, 26, steps=6, top_r=5.0)
        g.box(COCOA, (x, y, z1 + 7), (8, 8, 16))


def overalls(m, x, y, z, col):
    """A pair of bib overalls pegged to a line at height z."""
    m.box(col, (x, y, z - 22), (30, 6, 44))
    m.box(col, (x, y, z - 58), (46, 6, 30))
    for sd in (-1, 1):
        m.box(col, (x + sd * 13, y, z - 104), (18, 6, 64))
        m.box(WHT, (x + sd * 10, y, z - 2), (6, 8, 12))


def shirt(m, x, y, z, col):
    m.box(col, (x, y, z - 32), (44, 6, 62))
    for sd in (-1, 1):
        tilt_box(m, col, (x + sd * 20, y, z - 8), (x + sd * 46, y, z - 36), 6, 15)


def pick(m, a, b, head=34.0, t=7.0):
    """A pickaxe: the handle from a (its foot) to b, the head across b in the plane along X."""
    tilt_box(m, COCOA, a, b, t, t)
    for sd in (-1, 1):
        tilt_box(m, TIN, b, (b[0] + sd * head, b[1], b[2] - head * 0.3), t, t + 4)


def shovel(m, a, b, t=7.0):
    """A shovel: the handle from a (the grip) to b (the blade's neck), the blade beyond b."""
    tilt_box(m, COCOA, a, b, t, t)
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    tilt_box(m, TIN, b, (b[0] + dx / L * 34, b[1] + dy / L * 34, b[2] + dz / L * 34), 26, 6)


def sack(g, x, y, z, yaw=0.0, col=SACK, tie=MUS):
    g.box(col, (x, y, z), (72, 46, 30), rot=(0, yaw, 0))
    g.box(tie, (x, y, z), (10, 48, 32), rot=(0, yaw, 0))


def barrel(g, x, y, z, r=30.0, h=80.0):
    g.cyl(COCOA, (x, y, z), r, h, steps=8)
    for f in (0.2, 0.75):
        g.cyl(TIN, (x, y, z + h * f), r + 2, 6, steps=8)


# ------------------------------------------------------------------------------------------ buildings, the left
def pay_office(sc):
    """The PAY OFFICE: a tall thin mustard box on stilts, one great round window, a pay hatch under a cream awning,
    a steep single-pitch roof, a wind arrow on its mast. 3.4 m deep, 16 m tall."""
    g = sc.still()
    stilts(g, -170, 170, -170, 170, 500, nx=2)
    lean(g, MUS, -150, 150, -120, 165, 500, 1310, 1480, roof=TEAL, over=30)
    for sx in (-1, 1):
        g.box(WHT, (sx * 150, -120, 905), (16, 16, 810))
    g.box(CRM, (0, -122, 880), (300, 8, 26))
    ribs(g, CRM, -140, 140, -121, 900, 1300, 7, w=8)
    # the round window: a white ring, a lit pane, a cross
    rod(g, WHT, (0, -118, 1090), (0, -130, 1090), 98, steps=12)
    rod(g, LIT, (0, -124, 1090), (0, -136, 1090), 80, steps=12, glow=0.8)
    g.box(WHT, (0, -137, 1090), (160, 6, 8))
    g.box(WHT, (0, -137, 1090), (8, 6, 160))
    # the pay hatch, its counter and awning; the door
    window(g, -62, -122, 690, 84, 70)
    g.box(COCOA, (-62, -138, 642), (120, 34, 8))
    tilt_box(g, CRM, (-62, -124, 790), (-62, -176, 752), 140, 8)
    for k in (-1, 1):
        g.box(MUS, (-62 + k * 44, -150, 774), (20, 60, 6), rot=(0, 0, 0))
    door(g, 80, -122, 500, 80, 190, TEAL)
    # the landing and the stair down the side
    g.box(COCOA, (0, -146, 588), (340, 8, 8))
    for x in (-166, 166):
        g.box(COCOA, (x, -146, 546), (8, 8, 92))
    stair(g, 170, 500, 560, 0, -80, w=80, treads=6)
    # a drainpipe, a tin cowl, the coin sign on its bracket (it swings a little)
    g.box(TIN, (-158, -128, 940), (10, 10, 860))
    stovepipe(g, 90, 90, 1440, 1560, r=16)
    g.box(COCOA, (150, -150, 1000), (10, 70, 10))
    s = sc.part(P.motion(swing=7, pivot=(150, -170, 996), axis=90.0, rate=0.3, phase=0.2))
    s.box(COCOA, (150, -170, 970), (6, 6, 50))
    rod(s, MUS, (146, -170, 920), (154, -170, 920), 30, steps=8)
    rod(s, WHT, (144, -170, 920), (156, -170, 920), 14, steps=6)
    # the mast and the wind arrow
    g.cyl(COCOA, (0, 60, 1440), 7, 190, steps=6)
    g.sphere(MUS, (0, 60, 1640), 14, steps=(3, 6))
    v = sc.part(P.motion(swing=24, pivot=(0, 60, 1590), axis=P.UP, rate=0.12, phase=0.6))
    v.box(COCOA, (0, 60, 1590), (150, 8, 8))
    v.box(MUS, (-66, 60, 1590), (40, 6, 44))
    tilt_box(v, MUS, (60, 60, 1576), (88, 60, 1590), 6, 10)
    tilt_box(v, MUS, (60, 60, 1604), (88, 60, 1590), 6, 10)
    # a little balcony under the round window, papers by the door, an unlit lamp on a bracket
    g.box(COCOA, (0, -150, 962), (200, 56, 8))
    g.box(COCOA, (0, -176, 1010), (200, 6, 6))
    for x in (-96, 0, 96):
        g.box(COCOA, (x, -176, 986), (6, 6, 50))
    for k, c in enumerate((WHT, CRM, WHT)):
        g.box(c, (18 + k * 2, -126, 640 + k * 34), (26, 5, 26))
    g.box(COCOA, (-150, -150, 1250), (8, 60, 8))
    g.box(TIN, (-150, -176, 1226), (20, 20, 34))
    g.box(MUS, (-150, -176, 1247), (28, 28, 8))
    g.box(WHT, (0, -122, 1296), (316, 10, 20))
    # the end walls: a window each
    xwindow(g, -151, 20, 1090, 70, 100, lit=0.6)
    xwindow(g, 151, 20, 1090, 70, 100)
    xwindow(g, 151, 20, 700, 70, 100, lit=0.6)


def lamp_room(sc):
    """The LAMP ROOM: a cream tin shed on stilts under a three-tooth saw-tooth roof, three long lit windows, the
    miners' lamps hanging in a row from a rail. 9 m long, 3.6 m deep, 11.6 m tall."""
    g = sc.still()
    stilts(g, -450, 450, -180, 180, 520, nx=4)
    g.box(CRM, (0, 25, 690), (880, 300, 340))
    g.box(MUS, (0, -127, 850), (884, 8, 34))
    sawtooth(g, TEAL, -450, 450, 3, 866, 250, -135, 180)
    for k in range(3):
        x = -450 + k * 300
        g.box(WHT, (x + 6, 20, 990), (10, 320, 250))            # the tooth's upright face: glazing bars
        g.box(LIT, (x + 2, 20, 1000), (8, 250, 150), glow=0.6)
        g.cyl(TIN, (x + 200, 60, 950), 16, 70, steps=6)
    for k in range(3):
        window(g, -300 + k * 300, -127, 720, 190, 80, lit=0.7)
        ribs(g, WHT, -435 + k * 300, -165 + k * 300, -126, 540, 640, 3, w=8)
    door(g, -150, -127, 520, 70, 150, TEAL)
    door(g, 150, -127, 520, 70, 150, TEAL)
    # the tally board: a mustard board, a grid of white tags
    g.box(MUS, (0, -129, 600), (120, 8, 110))
    for i in range(3):
        for j in range(3):
            g.box(WHT, (-34 + i * 34, -134, 568 + j * 32), (16, 6, 16))
    # the landing, its rail, the lamp rail and its lamps (they swing together)
    g.box(COCOA, (0, -172, 612), (900, 8, 8))
    for x in (-446, -150, 150, 446):
        g.box(COCOA, (x, -172, 566), (8, 8, 92))
    g.box(COCOA, (0, -160, 836), (860, 8, 8))
    for x in (-420, 0, 420):
        g.box(COCOA, (x, -146, 836), (8, 36, 8))
    r = random.Random(13)
    for k in range(8):
        x = -385 + k * 110
        m = sc.part(P.motion(swing=6, pivot=(x, -160, 832), axis=0.0, rate=0.32, phase=r.random()))
        m.box(COCOA, (x, -160, 818), (5, 5, 26))
        m.box(CRM, (x, -160, 792), (16, 16, 26))
        m.box(MUS, (x, -160, 776), (20, 20, 8))
    stair(g, 450, 520, 830, 0, -90, w=80, treads=6)
    xwindow(g, -441, 30, 700, 90, 90, lit=0.6)
    xwindow(g, 441, 30, 700, 90, 90)


def tank_house(sc):
    """The WATER TANK HOUSE: a teal tank with cream hoops and a mustard cone roof on four tall legs, a cream pump
    shed under it, a ladder, a filling spout with a canvas sock. 4.4 m long, 3.6 m deep, 15.4 m tall."""
    g = sc.still()
    stilts(g, -200, 200, -170, 170, 300, nx=2, t=20, brace=False)
    lean(g, CRM, -185, 185, -150, 160, 300, 600, 690, roof=COCOA, over=22)
    door(g, -70, -152, 300, 80, 180, MUS)
    window(g, 85, -152, 470, 70, 80, lit=0.7)
    ribs(g, WHT, 30, 180, -151, 310, 400, 4, w=8)
    # the four legs and their braces above the shed
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, COCOA, (sx * 214, sy * 172, -250), (sx * 168, sy * 146, 1040), 20, 20)
    for z0, z1 in ((720, 880), (880, 1040)):
        tilt_box(g, COCOA, (-176, -160, z0), (176, -160, z1), 10, 10)
        tilt_box(g, COCOA, (176, -160, z0), (-176, -160, z1), 10, 10)
        for sx in (-1, 1):
            tilt_box(g, COCOA, (sx * 178, -150, z0), (sx * 178, 150, z1), 10, 10)
    g.box(COCOA, (0, 0, 875), (372, 330, 10))
    # the platform, the tank, its hoops and roof
    g.box(COCOA, (0, 0, 1050), (430, 356, 20))
    g.box(COCOA, (0, -174, 1110), (430, 8, 8))
    for x in (-211, 0, 211):
        g.box(COCOA, (x, -174, 1086), (8, 8, 56))
    g.cyl(TEAL, (0, 0, 1060), 160, 330, steps=10)
    for z in (1090, 1210, 1340):
        g.cyl(CRM, (0, 0, z), 165, 18, steps=10)
    g.cyl(MUS, (0, 0, 1390), 186, 120, steps=10, top_r=10)
    g.sphere(WHT, (0, 0, 1522), 14, steps=(3, 6))
    # the down pipe to the pump shed and its brass wheel; the rail round the other three sides
    g.cyl(TIN, (120, 60, 690), 12, 370, steps=6)
    rod(g, MUS, (120, 40, 800), (120, 32, 800), 26, steps=8)
    g.box(COCOA, (120, 36, 800), (60, 6, 6))
    g.box(COCOA, (0, 174, 1110), (430, 8, 8))
    for sx in (-1, 1):
        g.box(COCOA, (sx * 211, 0, 1110), (8, 356, 8))
        g.box(COCOA, (sx * 211, 174, 1086), (8, 8, 56))
    for k in range(8):
        a = math.radians(k * 45 + 22.5)
        g.box(WHT, (math.cos(a) * 163, math.sin(a) * 163, 1225), (8, 8, 330), rot=(0, k * 45 + 22.5, 0))
    # the level gauge: a white board, a mustard marker
    g.box(WHT, (-96, -166, 1220), (26, 8, 230))
    g.box(MUS, (-96, -171, 1255), (40, 6, 14))
    # the ladder up the right-hand side
    for y in (-30, 30):
        tilt_box(g, COCOA, (236, y, 0), (216, y, 1060), 8, 8)
    for k in range(7):
        z = 110 + k * 140
        g.box(COCOA, (234 - 20 * z / 1060.0, 0, z), (8, 60, 8))
    # the filling spout on the left-hand side: out along the piece, then down, a canvas sock swinging on it
    rod(g, TIN, (-150, 0, 1100), (-330, 0, 1100), 16, steps=6)
    rod(g, TIN, (-330, 0, 1116), (-330, 0, 1000), 16, steps=6)
    tilt_box(g, COCOA, (-200, 0, 1040), (-320, 0, 1090), 8, 8)
    s = sc.part(P.motion(swing=7, pivot=(-330, 0, 1000), axis=90.0, rate=0.28, phase=0.4))
    s.cyl(CRM, (-330, 0, 870), 13, 130, steps=6, top_r=18)
    # a bucket on a rope under the platform
    b = sc.part(P.motion(swing=8, pivot=(90, -120, 1040), axis=0.0, rate=0.33, phase=0.1))
    b.box(SACK, (90, -120, 965), (5, 5, 150))
    b.cyl(TIN, (90, -120, 860), 15, 34, steps=6, top_r=19)


def row_houses(sc):
    """THE ROW: three narrow miners' houses joined on one stilt deck, mustard, cream and teal, each roof pitched its
    own way, three doors on one landing, a line of washing. 12 m long, 3.6 m deep, 11.4 m tall."""
    g = sc.still()
    stilts(g, -600, 600, -180, 180, 470, nx=5)
    shed(g, MUS, -590, -200, -120, 175, 470, 800, 960, roof=COCOA, over=24)
    lean(g, CRM, -200, 200, -120, 175, 470, 960, 1110, roof=TEAL, over=24)
    shed(g, TEAL, 200, 590, -120, 175, 470, 900, 760, roof=MUS, over=24)
    door(g, -470, -122, 470, 80, 185, TEAL)
    door(g, -90, -122, 470, 80, 185, MUS)
    door(g, 300, -122, 470, 80, 185, CRM)
    window(g, -310, -122, 610, 80, 90, lit=0.7)
    window(g, 80, -122, 610, 80, 90)
    window(g, 480, -122, 610, 80, 90, lit=0.7)
    window(g, 0, -122, 850, 110, 80, lit=0.7)
    window(g, -300, -122, 790, 60, 60)
    for x, c in ((-310, TEAL), (80, TEAL), (480, WHT)):          # shutters
        for sd in (-1, 1):
            g.box(c, (x + sd * 62, -124, 610), (22, 6, 104))
    for x in (-200, 200):
        g.box(WHT, (x, -122, 700), (14, 12, 460))
    ribs(g, CRM, -580, -380, -121, 700, 790, 4, w=8)
    ribs(g, WHT, 230, 580, -121, 720, 750, 6, w=8)
    # the landing rail, the stair, three stovepipes, a rain barrel
    g.box(COCOA, (0, -172, 562), (1200, 8, 8))
    for x in (-596, -300, 0, 300, 596):
        g.box(COCOA, (x, -172, 516), (8, 8, 92))
    stair(g, -600, 470, -940, 0, -90, w=80, treads=6)
    stovepipe(g, -330, 90, 900, 1010, r=15)
    stovepipe(g, 110, 110, 1090, 1180, r=15)
    stovepipe(g, 470, 90, 800, 930, r=15)
    barrel(g, 560, -150, 470, r=26, h=70)
    # washing on a line between the first and the last house
    g.box(SACK, (-10, -168, 706), (760, 4, 4))
    w = sc.part(P.motion(swing=8, pivot=(0, -168, 706), axis=0.0, rate=0.3, phase=0.7))
    overalls(w, -260, -168, 704, TEAL)
    shirt(w, -150, -168, 704, WHT)
    overalls(w, 150, -168, 704, MUS)
    shirt(w, 250, -168, 704, CRM)
    xwindow(g, -591, 40, 640, 80, 90, lit=0.6)
    xwindow(g, 591, 40, 620, 80, 90, lit=0.6)


def bunkhouse(sc):
    """The BUNKHOUSE: a long cream shed on stilts, a veranda its whole length under a mustard lean-to, four teal
    doors, overalls drying on the rail, a blank name board, the stair at one end. 16.6 m long, 4.8 m deep, 11 m tall."""
    g = sc.still()
    stilts(g, -700, 700, -240, 240, 520, nx=5)
    lean(g, CRM, -685, 685, -90, 235, 520, 850, 1020, roof=TEAL)
    g.box(MUS, (0, -92, 552), (1368, 8, 60))
    veranda(g, -700, 700, -240, -90, 520, 300, 6, roof=MUS)
    for k in range(4):
        door(g, -525 + k * 350, -92, 520, 84, 190, TEAL)
    for k in range(3):
        window(g, -350 + k * 350, -92, 690, 90, 90, lit=(0.7 if k != 1 else 0.0))
        for sd in (-1, 1):
            g.box(TEAL, (-350 + k * 350 + sd * 66, -94, 690), (22, 6, 104))
    ribs(g, WHT, -680, 680, -91, 880, 900, 1, w=1360)
    ribs(g, WHT, -640, 640, 236, 560, 1000, 9, w=10)
    # the name board over the middle: blank, three shapes
    g.box(WHT, (0, -262, 890), (330, 8, 84))
    g.box(MUS, (0, -266, 890), (310, 6, 66))
    g.box(TEAL, (-100, -270, 890), (36, 6, 36))
    rod(g, WHT, (0, -267, 890), (0, -273, 890), 20, steps=8)
    g.box(TEAL, (100, -270, 890), (36, 6, 36), rot=(45, 0, 0))
    for x in (-150, 150):
        g.box(COCOA, (x, -258, 850), (8, 8, 60))
    # a bench, a row of boots, a tin jug
    g.box(COCOA, (-350, -150, 566), (150, 34, 8))
    for x in (-410, -290):
        g.box(COCOA, (x, -150, 542), (8, 30, 44))
    for k in range(4):
        g.box(COCOA, (120 + k * 30, -120, 532), (14, 26, 22))
    g.cyl(TIN, (-350, -150, 570), 12, 30, steps=6, top_r=8)
    # overalls and shirts drying on the veranda rail
    w = sc.part(P.motion(swing=7, pivot=(0, -234, 614), axis=0.0, rate=0.3, phase=0.3))
    for k, c in enumerate((TEAL, WHT, MUS, TEAL, CRM)):
        x = -610 + k * 115 + (300 if k > 2 else 0)
        if k % 2:
            w.box(c, (x, -238, 584), (50, 6, 64))
        else:
            w.box(c, (x, -238, 590), (34, 6, 52))
            for sd in (-1, 1):
                w.box(c, (x + sd * 12, -238, 540), (16, 6, 52))
    # the stair at the right-hand end, a stove chimney, two tin vents, a water butt
    stair(g, 700, 520, 960, 150, -165, w=90, treads=5)
    stair(g, 960, 150, 1060, 0, -165, w=90, treads=2)
    stovepipe(g, 480, 150, 960, 1130, r=22)
    for x in (-420, 20):
        g.cyl(TIN, (x, 120, 950), 26, 60, steps=6)
        g.cyl(TIN, (x, 120, 1016), 44, 20, steps=6, top_r=8)
    barrel(g, -660, -160, 520, r=30, h=84)
    xwindow(g, -686, 60, 700, 90, 100, lit=0.6)
    xwindow(g, 686, 60, 700, 90, 100)


def cook_house(sc):
    """The COOK HOUSE: a mustard kitchen on stilts, a fat banded tin chimney under a cone hat, a serving hatch under a
    striped cream awning, pots on the counter, the dinner triangle swinging. 8 m long, 4.8 m deep, 13.4 m tall."""
    g = sc.still()
    stilts(g, -400, 400, -240, 240, 460, nx=3)
    shed(g, MUS, -385, 385, -130, 235, 460, 800, 960, roof=TEAL)
    g.box(CRM, (0, -132, 492), (770, 8, 60))
    ribs(g, CRM, -120, 380, -131, 800, 830, 1, w=500)
    # the serving hatch, its counter and pots
    g.box(WHT, (120, -132, 650), (330, 8, 140))
    g.box(PANE, (120, -135, 650), (300, 8, 110))
    g.box(LIT, (120, -137, 690), (300, 6, 26), glow=0.7)
    g.box(COCOA, (120, -160, 588), (350, 56, 10))
    for k, (r0, h) in enumerate(((20, 34), (26, 28), (16, 40))):
        g.cyl(TIN, (20 + k * 100, -164, 593), r0, h, steps=6)
        g.box(COCOA, (20 + k * 100, -164, 598 + h), (8, 8, 8))
    # the awning: cream, mustard stripes, a valance; two thin props
    tilt_box(g, CRM, (120, -130, 800), (120, -250, 742), 420, 8)
    for k in range(4):
        tilt_box(g, MUS, (-45 + k * 110, -130, 805), (-45 + k * 110, -250, 747), 50, 6)
    for k in range(7):
        g.box(CRM if k % 2 else MUS, (-60 + k * 60, -250, 724), (56, 6, 30))
    for x in (-84, 324):
        g.box(WHT, (x, -244, 615), (8, 8, 250))
    # the door, its window, the plate sign (a cream disc, a teal ring)
    door(g, -270, -132, 460, 86, 190, TEAL)
    window(g, -150, -132, 660, 60, 70, lit=0.7)
    rod(g, TEAL, (-270, -134, 740), (-270, -142, 740), 40, steps=10)
    rod(g, CRM, (-270, -138, 740), (-270, -146, 740), 30, steps=10)
    # the veranda rail and the stair at the left-hand end
    g.box(COCOA, (-240, -232, 552), (320, 8, 8))
    for x in (-396, -90):
        g.box(COCOA, (x, -232, 506), (8, 8, 92))
    stair(g, -400, 460, -740, 0, -185, w=90, treads=6)
    # the chimney: fat, banded, a cone hat on three struts; two puffs of pale smoke floating over it
    g.cyl(TIN, (240, 110, 460), 72, 760, steps=10)
    for z in (700, 960, 1180):
        g.cyl(COCOA, (240, 110, z), 76, 14, steps=10)
    g.cyl(MUS, (240, 110, 1270), 124, 70, steps=10, top_r=8)
    for a in (0, 120, 240):
        g.box(COCOA, (240 + math.cos(math.radians(a)) * 62, 110 + math.sin(math.radians(a)) * 62, 1246), (8, 8, 54))
    for k, (dz, r0, ph) in enumerate(((1400, 44, 0.0), (1490, 58, 0.45))):
        s = sc.part(P.motion(bob=-26.0, rate=0.14, phase=ph))
        s.sphere(TIN, (250 + k * 40, 110, dz), r0, scale=(1.2, 1.0, 0.8), steps=(4, 6))
    stovepipe(g, -250, 120, 840, 980, r=16)
    # the dinner triangle on its bracket
    g.box(COCOA, (372, -170, 800), (10, 90, 10))
    t = sc.part(P.motion(swing=11, pivot=(372, -206, 796), axis=90.0, rate=0.45, phase=0.2))
    t.box(SACK, (372, -206, 780), (5, 5, 30))
    tilt_box(t, TIN, (372, -206, 766), (346, -206, 716), 6, 6)
    tilt_box(t, TIN, (372, -206, 766), (398, -206, 716), 6, 6)
    t.box(TIN, (372, -206, 716), (56, 6, 6))
    # sacks of flour and a barrel on the deck
    sack(g, -40, -200, 478, yaw=12, col=CRM, tie=TEAL)
    sack(g, -36, -196, 508, yaw=-8, col=CRM, tie=TEAL)
    barrel(g, 350, -200, 462, r=28, h=76)
    xwindow(g, 386, 60, 660, 90, 90, lit=0.6)
    xwindow(g, -386, 60, 640, 90, 90)


# ------------------------------------------------------------------------------------------ buildings, the right
def assay_shed(sc):
    """The ASSAY SHED: a teal tin shed on stilts, and over its roof a cream sign board with a balance drawn on it in
    mustard, the beam tipping slowly; sieves on the wall, ore samples on a bench. 9 m long, 5 m deep, 13.4 m tall."""
    g = sc.still()
    stilts(g, -450, 450, -250, 250, 470, nx=4)
    shed(g, TEAL, -435, 435, -130, 245, 470, 930, 800, roof=CRM)
    g.box(MUS, (0, -132, 500), (870, 8, 56))
    veranda(g, -450, 450, -250, -130, 470, 290, 4, roof=MUS)
    door(g, -280, -132, 470, 86, 190, CRM)
    window(g, -120, -132, 650, 90, 90, lit=0.7)
    window(g, 300, -132, 650, 90, 90)
    # the sieves on the wall: three tin rings
    for k in range(3):
        rod(g, TIN, (30 + k * 80, -134, 690 - k * 24), (30 + k * 80, -142, 690 - k * 24), 30, steps=8)
        rod(g, PANE, (30 + k * 80, -138, 690 - k * 24), (30 + k * 80, -146, 690 - k * 24), 22, steps=8)
    # the sample bench: ore in three trays, two sacks beside it
    g.box(COCOA, (110, -200, 548), (260, 50, 8))
    for x in (-10, 230):
        g.box(COCOA, (x, -200, 520), (8, 44, 52))
    for k, c in enumerate((ASH, SACK, MUS)):
        g.box(WHT, (30 + k * 80, -200, 558), (60, 40, 12))
        g.box(c, (30 + k * 80, -200, 568), (44, 28, 14))
    sack(g, 380, -200, 487, yaw=10)
    sack(g, 384, -196, 517, yaw=-14)
    # the sign board over the roof on two posts, and the balance on it
    for x in (-190, 190):
        g.box(COCOA, (x, -150, 1060), (14, 14, 330))
    g.box(WHT, (0, -158, 1140), (470, 8, 300))
    g.box(CRM, (0, -162, 1140), (444, 6, 274))
    g.box(MUS, (0, -167, 1120), (14, 6, 190))
    g.box(MUS, (0, -167, 1022), (130, 6, 16))
    rod(g, MUS, (0, -165, 1226), (0, -171, 1226), 16, steps=8)
    b = sc.part(P.motion(swing=7, pivot=(0, -168, 1215), axis=90.0, rate=0.16, phase=0.3))
    b.box(MUS, (0, -170, 1215), (330, 6, 14))
    for sd in (-1, 1):
        b.box(COCOA, (sd * 150, -170, 1160), (5, 5, 100))
        b.box(MUS, (sd * 150, -170, 1104), (96, 6, 14))
        tilt_box(b, MUS, (sd * 150 - 48, -170, 1110), (sd * 150 - 34, -170, 1132), 6, 6)
        tilt_box(b, MUS, (sd * 150 + 48, -170, 1110), (sd * 150 + 34, -170, 1132), 6, 6)
    b.box(ASH, (-150, -170, 1122), (40, 8, 22))
    # the furnace stack, the stair at the right-hand end
    g.cyl(COCOA, (-320, 130, 880), 30, 400, steps=8)
    g.cyl(CRM, (-320, 130, 1180), 34, 30, steps=8)
    g.cyl(TIN, (-320, 130, 1294), 50, 24, steps=8, top_r=6)
    stair(g, 450, 470, 800, 0, -190, w=90, treads=6)
    xwindow(g, -436, 70, 700, 90, 100, lit=0.6)
    xwindow(g, 436, 70, 650, 90, 100)
    ribs(g, CRM, -420, 420, 246, 500, 780, 8, w=10)


def band_hall(sc):
    """The BAND HALL: a cream hall on stilts under a teal tin barrel roof with mustard hoops, four tall lit windows,
    a white arch over the mustard double door, a bass drum on the veranda, a blank board of five lines and notes.
    11 m long, 5 m deep, 10.4 m tall."""
    g = sc.still()
    stilts(g, -550, 550, -250, 250, 440, nx=5)
    g.box(CRM, (0, 50, 610), (1060, 400, 340))
    rod(g, TEAL, (-540, 50, 780), (540, 50, 780), 205, steps=12)
    for k in range(5):
        x = -500 + k * 250
        rod(g, MUS, (x - 9, 50, 780), (x + 9, 50, 780), 211, steps=12)
    for sx in (-1, 1):                                       # the round gable ends: a white sun of three bars
        g.box(WHT, (sx * 542, 50, 880), (8, 200, 14))
        g.box(WHT, (sx * 542, 50, 880), (8, 14, 150))
    veranda(g, -550, 550, -250, -150, 440, 280, 6, roof=MUS)
    # the double door under its arch
    rod(g, WHT, (0, -152, 650), (0, -160, 650), 78, steps=12)
    door(g, -34, -152, 440, 64, 200, MUS)
    door(g, 34, -152, 440, 64, 200, MUS)
    for k in range(4):
        x = (-420, -230, 230, 420)[k]
        window(g, x, -152, 610, 70, 170, lit=0.75)
        g.box(WHT, (x, -158, 610), (70, 6, 8))
    # the board: five white lines and mustard notes on teal (shapes, no words)
    g.box(WHT, (0, -262, 792), (300, 8, 110))
    g.box(TEAL, (0, -266, 792), (284, 6, 94))
    for k in range(5):
        g.box(WHT, (0, -270, 762 + k * 15), (260, 6, 4))
    for k, dz in enumerate((0, 22, 8, 38, 15)):
        rod(g, MUS, (-100 + k * 50, -270, 766 + dz), (-100 + k * 50, -276, 766 + dz), 9, steps=6)
    for x in (-120, 120):
        g.box(COCOA, (x, -258, 740), (8, 8, 40))
    # the bass drum and a stool on the veranda; a horn-shaped tin vent on the roof
    rod(g, CRM, (-330, -222, 500), (-330, -186, 500), 46, steps=10)
    rod(g, MUS, (-330, -226, 500), (-330, -218, 500), 49, steps=10)
    g.box(COCOA, (-330, -200, 448), (70, 30, 16))
    g.cyl(COCOA, (-240, -200, 440), 16, 44, steps=6)
    for x in (-250, 250):
        g.cyl(TIN, (x, 50, 975), 20, 50, steps=6)
        g.cyl(TIN, (x, 50, 1025), 22, 34, steps=6, top_r=44)
    stair(g, 550, 440, 880, 0, -200, w=90, treads=6)
    g.box(COCOA, (-260, -242, 532), (560, 8, 8))
    xwindow(g, -531, 50, 600, 110, 130, lit=0.6)
    xwindow(g, 531, 50, 600, 110, 130, lit=0.6)


def company_store(sc):
    """The COMPANY STORE: two mustard storeys on stilts behind a stepped cream false front, a blank sign of a sun and
    two stripes, a wide shop window, barrels, sacks and a swinging disc sign on the veranda. 10 m long, 5 m deep, 13.4 m."""
    g = sc.still()
    stilts(g, -500, 500, -250, 250, 430, nx=4)
    lean(g, MUS, -480, 480, -130, 245, 430, 1060, 980, roof=COCOA, over=20)
    # the false front: stepped, white caps
    g.box(CRM, (0, -136, 1130), (990, 14, 200))
    g.box(CRM, (0, -136, 1280), (440, 14, 110))
    g.box(WHT, (0, -138, 1340), (470, 20, 16))
    for sx in (-1, 1):
        g.box(WHT, (sx * 370, -138, 1236), (270, 20, 14))
        g.box(COCOA, (sx * 300, 40, 1100), (10, 340, 10), rot=(0, 0, 0))
    # the sign: a teal panel, a mustard sun, two white stripes
    g.box(TEAL, (0, -145, 1190), (400, 6, 150))
    rod(g, MUS, (-110, -147, 1190), (-110, -153, 1190), 48, steps=10)
    for k in range(2):
        g.box(WHT, (70, -150, 1166 + k * 48), (190, 6, 22))
    # the upper storey: four windows; the veranda and the shop front
    for k in range(4):
        window(g, -360 + k * 240, -132, 900, 90, 110, lit=(0.7 if k in (0, 2) else 0.0))
    veranda(g, -500, 500, -250, -130, 430, 310, 5, roof=TEAL)
    g.box(WHT, (-190, -132, 600), (430, 8, 190))
    g.box(PANE, (-190, -135, 600), (400, 8, 160))
    g.box(LIT, (-190, -137, 660), (400, 6, 30), glow=0.7)
    for x in (-290, -190, -90):
        g.box(WHT, (x, -139, 600), (8, 6, 160))
    door(g, 150, -132, 430, 96, 200, TEAL)
    window(g, 340, -132, 620, 90, 100)
    g.box(CRM, (0, -132, 462), (960, 8, 56))
    # goods on the veranda
    barrel(g, -440, -200, 432, r=30, h=82)
    barrel(g, -374, -206, 432, r=26, h=70)
    sack(g, 300, -200, 447, yaw=8)
    sack(g, 310, -196, 477, yaw=-12)
    sack(g, 380, -204, 447, yaw=30, tie=TEAL)
    g.box(CRM, (40, -200, 456), (60, 44, 46))
    g.box(COCOA, (40, -200, 456), (64, 10, 50))
    # the hanging sign: a mustard disc on a bracket
    g.box(COCOA, (480, -190, 720), (10, 110, 10))
    s = sc.part(P.motion(swing=8, pivot=(480, -236, 716), axis=90.0, rate=0.3, phase=0.5))
    s.box(COCOA, (480, -236, 690), (5, 5, 50))
    rod(s, MUS, (476, -236, 640), (484, -236, 640), 32, steps=8)
    rod(s, TEAL, (474, -236, 640), (486, -236, 640), 16, steps=6)
    stovepipe(g, 330, 120, 1010, 1150, r=18)
    stair(g, -500, 430, -830, 0, -190, w=90, treads=6)
    xwindow(g, -481, 60, 900, 90, 110, lit=0.6)
    xwindow(g, 481, 60, 900, 90, 110)
    xwindow(g, 481, 60, 620, 90, 110, lit=0.6)


# ------------------------------------------------------------------------------------------ props, low (under 2 m)
def sack_barrow(sc):
    """A HAND BARROW of ore sacks: two carrying poles on four short legs (no wheels: it is carried), six sacks with
    mustard ties, a shovel stuck in a heap of ore beside it. Top 1.7 m."""
    g = sc.still()
    for y in (-30, 30):
        g.box(COCOA, (-20, y, 46), (300, 9, 9))
    for x in (-90, 60):
        for y in (-30, 30):
            g.box(COCOA, (x, y, 2), (10, 10, 84))
    g.box(COCOA, (-15, 0, 54), (170, 76, 8))
    r = random.Random(31)
    for k, (x, z) in enumerate(((-60, 74), (20, 74), (-20, 105), (-56, 136), (24, 134), (-16, 164))):
        sack(g, x, r.uniform(-6, 6), z, yaw=r.uniform(-14, 14))
    g.box(WHT, (-96, -26, 100), (6, 14, 20))
    g.cyl(ASH, (165, 4, -40), 58, 86, steps=7, top_r=8)
    for (x, y) in ((120, -30), (196, 30), (150, 40)):
        g.box(ASH, (x, y, 6), (22, 20, 16), rot=(0, 30, 0))
    shovel(g, (190, 0, 168), (172, 2, 60))


def pit_props(sc):
    """PIT PROPS: twelve pale timbers stacked between four cocoa stakes, ends painted mustard and teal, a cross-cut
    saw across the top, a short stack of sawn planks beside it. Top 1.45 m."""
    g = sc.still()
    k = 0
    for row, n in enumerate((5, 4, 3)):
        for i in range(n):
            y = (i - (n - 1) / 2.0) * 23
            z = 22 + row * 27
            rod(g, SACK, (-160, y, z), (160, y, z), 13, steps=6)
            g.box((MUS, TEAL, WHT)[k % 3], (-162, y, z), (6, 13, 13))
            k += 1
    for x in (-110, 110):
        for y in (-56, 56):
            g.box(COCOA, (x, y, 30), (10, 10, 150))
        g.box(COCOA, (x, 0, 5), (12, 130, 10))
    tilt_box(g, TIN, (-70, 0, 106), (86, 0, 112), 16, 4)
    for x in (-78, 94):
        g.box(COCOA, (x, 0, 122), (8, 8, 30))
    for j in range(4):
        g.box(CRM, (250, 0, 8 + j * 12), (150, 60 - j * 6, 10), rot=(0, j * 4 - 6, 0))
    g.box(COCOA, (250, 0, -18), (30, 70, 44))
    g.box(MUS, (250, 0, 58), (26, 26, 6), rot=(0, 20, 0))


def tin_bath(sc):
    """THE WASH BENCH: a tin bath and two buckets on a plank bench, a washboard, a striped towel on a post, a pair
    of boots. Top 1.8 m."""
    g = sc.still()
    g.box(COCOA, (0, 0, 46), (300, 56, 8))
    for x in (-120, 120):
        tilt_box(g, COCOA, (x - 22, 0, -40), (x, 0, 44), 50, 8)
        tilt_box(g, COCOA, (x + 22, 0, -40), (x, 0, 44), 50, 8)
    g.cyl(TIN, (-50, 0, 50), 44, 40, steps=8, top_r=54)
    g.cyl(WHT, (-50, 0, 82), 48, 6, steps=8)
    for (x, y, r0) in ((-66, -8, 14), (-40, 10, 11), (-52, -14, 9)):
        g.sphere(WHT, (x, y, 92), r0, steps=(3, 6))
    for x in (70, 124):
        g.cyl(TIN, (x, 0, 50), 16, 30, steps=6, top_r=20)
        g.box(COCOA, (x, 0, 92), (30, 5, 5))
        for sd in (-1, 1):
            g.box(COCOA, (x + sd * 15, 0, 86), (5, 5, 16))
    tilt_box(g, CRM, (16, -22, 52), (16, 4, 104), 40, 6)
    for k in range(4):
        tilt_box(g, TIN, (16, -24 + k * 5, 60 + k * 10), (16, -22 + k * 5, 66 + k * 10), 32, 5)
    g.box(COCOA, (-170, 14, 70), (10, 10, 220))
    g.box(COCOA, (-150, 14, 172), (60, 8, 8))
    for k in range(4):
        g.box(TEAL if k % 2 else CRM, (-140, 14, 160 - k * 18), (44, 12, 18))
    for x in (176, 200):
        g.box(COCOA, (x, -6, 12), (16, 30, 24))
        g.box(COCOA, (x, 4, 30), (16, 14, 26))


def pick_rack(sc):
    """THE PICK RACK: a cocoa A-frame rack, three pickaxes and two shovels leaning on it, three mustard hard hats
    on pegs, a coil of rope. Top 1.85 m."""
    g = sc.still()
    for x in (-130, 130):
        tilt_box(g, COCOA, (x, -34, -40), (x, 0, 150), 10, 10)
        tilt_box(g, COCOA, (x, 34, -40), (x, 0, 150), 10, 10)
        g.box(COCOA, (x, 0, 60), (10, 50, 8))
    g.box(COCOA, (0, 0, 150), (290, 10, 10))
    g.box(COCOA, (0, -14, 62), (270, 8, 8))
    for k, x in enumerate((-96, -30, 40)):
        pick(g, (x, -40, 4), (x, -8, 176), head=30)
    shovel(g, (86, -8, 180), (90, -40, 40))
    shovel(g, (112, -8, 172), (116, -40, 32))
    for k, x in enumerate((-70, 0, 70)):
        g.box(COCOA, (x, 10, 160), (6, 6, 20))
        g.sphere(MUS, (x, 10, 170), 17, scale=(1.0, 1.0, 0.8), steps=(4, 6))
        g.cyl(MUS, (x, 10, 166), 22, 4, steps=6)
    for k in range(3):
        g.cyl(SACK, (-150, 26, 4 + k * 10), 26 - k * 2, 9, steps=7)
    g.box(WHT, (120, 6, 110), (30, 5, 24))


# ------------------------------------------------------------------------------------------ props, tall and open
def wash_line(sc):
    """A WASHING LINE of overalls: two cocoa posts with cross arms, two lines, bib overalls, shirts and long johns
    pegged out with gaps between them, swaying; a basket at the foot. 8 m long, 3 m tall, open."""
    g = sc.still()
    for x in (-380, 380):
        g.box(COCOA, (x, 0, 45), (12, 12, 490))
        g.box(COCOA, (x, 0, 280), (10, 84, 10))
        tilt_box(g, COCOA, (x, -30, 230), (x, 0, 196), 8, 8)
        tilt_box(g, COCOA, (x, 30, 230), (x, 0, 196), 8, 8)
    for y in (-34, 34):
        g.box(SACK, (0, y, 276), (760, 4, 4))
    r = random.Random(7)
    a = sc.part(P.motion(swing=9, pivot=(0, -34, 276), axis=0.0, rate=0.33, phase=0.0))
    overalls(a, -300, -34, 274, TEAL)
    shirt(a, -150, -34, 274, WHT)
    overalls(a, 60, -34, 274, MUS)
    shirt(a, 290, -34, 274, CRM)
    b = sc.part(P.motion(swing=8, pivot=(0, 34, 276), axis=0.0, rate=0.29, phase=0.4))
    overalls(b, -210, 34, 274, COCOA)
    for sd in (-1, 1):                                       # long johns
        b.box(WHT, (-20 + sd * 11, 34, 232), (17, 6, 84))
    b.box(WHT, (-20, 34, 262), (42, 6, 26))
    overalls(b, 180, 34, 274, TEAL)
    for k in range(4):                                       # socks
        b.box(MUS if k % 2 else CRM, (300 + k * 18, 34, 262), (9, 6, 26))
    g.cyl(SACK, (-320, 20, -30), 30, 62, steps=7, top_r=36)
    g.box(WHT, (-320, 20, 36), (40, 30, 10))
    tilt_box(g, COCOA, (120, 30, -30), (100, 0, 274), 6, 6)


def notice_board(sc):
    """The NOTICE BOARD: a cream board on two tall legs under a little teal tin roof, squares of paper in mustard,
    teal, white and cocoa, a row of brass tally discs, one sheet flapping. 2.6 m long, 3.3 m tall, open below."""
    g = sc.still()
    for x in (-112, 112):
        g.box(COCOA, (x, 0, 50), (12, 12, 500))
        tilt_box(g, COCOA, (x, 0, -30), (x, 34, -30 + 90), 8, 8)
    g.box(COCOA, (0, 0, 210), (250, 10, 160))
    g.box(CRM, (0, -6, 210), (226, 6, 138))
    tilt_box(g, TEAL, (0, -44, 306), (0, 30, 330), 290, 8)
    g.box(MUS, (0, -44, 296), (290, 6, 14))
    for (x, z, w, h, c) in ((-84, 240, 36, 44, MUS), (-40, 246, 30, 34, WHT), (-2, 240, 30, 46, TEAL), (-84, 182, 40, 36, WHT),
                            (-34, 186, 34, 30, COCOA), (10, 186, 26, 30, MUS)):
        g.box(c, (x, -11, z), (w, 5, h))
    g.box(TEAL, (70, -11, 252), (64, 5, 20))
    for k in range(5):
        rod(g, MUS, (44 + k * 13, -10, 222), (44 + k * 13, -15, 222), 5, steps=6)
    p = sc.part(P.motion(swing=13, pivot=(70, -11, 204), axis=0.0, rate=0.5, phase=0.2))
    p.box(WHT, (70, -12, 176), (58, 5, 56))
    g.box(COCOA, (0, -30, 126), (120, 30, 6))
    for x in (-52, 52):
        tilt_box(g, COCOA, (x, -6, 100), (x, -42, 124), 6, 6)


def swing_seat(sc):
    """The SWING SEAT: a bench of cream slats with a teal cushion hung in a cocoa A-frame under a striped tin
    canopy, swinging gently. 3 m long, 2.9 m tall, open."""
    g = sc.still()
    for x in (-140, 140):
        tilt_box(g, COCOA, (x, -42, -40), (x, 0, 250), 10, 10)
        tilt_box(g, COCOA, (x, 42, -40), (x, 0, 250), 10, 10)
        g.box(COCOA, (x, 0, 110), (8, 50, 8))
    g.box(COCOA, (0, 0, 250), (300, 10, 10))
    g.box(CRM, (0, 0, 272), (330, 104, 8))
    for k in range(4):
        g.box(MUS, (-120 + k * 80, 0, 277), (40, 104, 5))
    for k in range(9):
        g.box(MUS if k % 2 else CRM, (-128 + k * 32, -52, 258), (30, 5, 22))
    s = sc.part(P.motion(swing=7, pivot=(0, 0, 250), axis=0.0, rate=0.28, phase=0.1))
    for x in (-100, 100):
        s.box(COCOA, (x, 0, 160), (5, 5, 180))
    s.box(CRM, (0, 0, 66), (220, 46, 8))
    for k in range(3):
        s.box(CRM, (0, 24, 86 + k * 20), (220, 6, 14))
    s.box(TEAL, (-50, -2, 76), (80, 40, 12))
    s.box(MUS, (60, -2, 76), (50, 36, 10))
    s.box(COCOA, (0, 24, 100), (8, 8, 70))


def stilt_larder(sc):
    """The STILT LARDER: a cream meat safe with a teal louvred door on four thin splayed legs, tin rat guards on
    every leg, a mustard tin roof, a ladder, a sack hanging under it. 1.6 m long, 3.5 m tall, open below."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, COCOA, (sx * 66, sy * 40, -40), (sx * 50, sy * 30, 220), 10, 10)
            g.cyl(TIN, (sx * 55, sy * 33, 150), 22, 14, steps=6, top_r=7)
    for sy in (-1, 1):
        tilt_box(g, COCOA, (-60, sy * 36, 40), (52, sy * 31, 200), 6, 6)
    g.box(COCOA, (0, 0, 222), (130, 84, 8))
    g.box(CRM, (0, 0, 280), (116, 74, 110))
    g.box(TEAL, (-20, -38, 278), (60, 6, 90))
    for k in range(5):
        g.box(WHT, (-20, -42, 244 + k * 17), (52, 5, 6))
    g.box(PANE, (36, -38, 290), (30, 6, 40))
    g.box(WHT, (36, -41, 290), (5, 5, 40))
    tilt_box(g, MUS, (0, -56, 334), (0, 50, 362), 150, 8)
    for y in (-14, 14):
        tilt_box(g, COCOA, (122, y, -30), (64, y, 226), 6, 6)
    for k in range(4):
        z = 30 + k * 52
        g.box(COCOA, (122 - 58 * (z + 30) / 256.0, 0, z), (6, 34, 6))
    s = sc.part(P.motion(swing=9, pivot=(-10, 0, 218), axis=0.0, rate=0.3, phase=0.6))
    s.box(SACK, (-10, 0, 190), (5, 5, 56))
    s.box(SACK, (-10, 0, 140), (34, 28, 50))
    s.box(MUS, (-10, 0, 166), (20, 20, 6))


# ------------------------------------------------------------------------------------------ the hero
def hopper(sc):
    """THE HOPPER: a 45 m ore tower. A square teal shaft banded in cream on four splayed cocoa legs, a huge cream
    funnel under it over a heap of ore, a gallery and a zig-zag stair, a cream head house, a cream conveyor gallery
    climbing to it from a loading shed, and on the mast a mustard PICKAXE weathervane turning slowly."""
    g = sc.still()

    def leg(z):                                              # how far out a leg stands at height z
        return 900.0 - 500.0 * (z + 300.0) / 2200.0

    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, COCOA, (sx * 900, sy * 900, -300), (sx * 400, sy * 400, 1900), 90, 90)
            g.box(ASH, (sx * 885, sy * 885, -10), (240, 240, 120))
    for z in (700.0, 1400.0):
        p = leg(z)
        for s in (-1, 1):
            g.box(COCOA, (0, s * p, z), (2 * p, 44, 44))
            g.box(COCOA, (s * p, 0, z), (44, 2 * p, 44))
    for z0, z1 in ((0.0, 700.0), (700.0, 1400.0)):
        a, b = leg(z0), leg(z1)
        for s in (-1, 1):
            tilt_box(g, COCOA, (-a, s * a, z0), (b, s * b, z1), 30, 30)
            tilt_box(g, COCOA, (a, s * a, z0), (-b, s * b, z1), 30, 30)
            tilt_box(g, COCOA, (s * a, -a, z0), (s * b, b, z1), 30, 30)
            tilt_box(g, COCOA, (s * a, a, z0), (s * b, -b, z1), 30, 30)
    # the funnel, its spout and gate, the ore under it
    g.cyl(CRM, (0, 0, 1150), 130, 760, steps=8, top_r=570)
    g.cyl(TEAL, (0, 0, 1560), 352, 60, steps=8, top_r=388)
    g.cyl(MUS, (0, 0, 960), 130, 200, steps=8)
    g.box(COCOA, (0, 0, 1150), (300, 300, 24))
    g.box(COCOA, (170, 0, 1060), (60, 14, 200))
    g.cyl(ASH, (0, 0, -60), 430, 400, steps=8, top_r=24)
    g.cyl(SACK, (160, -220, -40), 150, 150, steps=6, top_r=12)
    # the shaft: teal, cream bands, white corner boards, corrugation
    g.box(TEAL, (0, 0, 2850), (840, 840, 1900))
    for z in (1930, 2560, 3190, 3776):
        g.box(CRM, (0, 0, z), (868, 868, 54))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WHT, (sx * 418, sy * 418, 2850), (34, 34, 1900))
    for k in range(6):
        g.box(CRM, (-300 + k * 120, -422, 2374), (12, 8, 290))
    for sx in (-1, 1):
        for k in range(5):
            g.box(CRM, (sx * 422, -280 + k * 140, 2374), (8, 12, 290))
        g.cyl(TIN, (sx * 440, 380, 1960), 16, 1830, steps=6)
    # the top balcony on the road face, under the fourth band
    g.box(COCOA, (0, -470, 3690), (520, 100, 12))
    g.box(COCOA, (0, -516, 3790), (520, 8, 8))
    for x in (-256, -85, 85, 256):
        g.box(COCOA, (x, -516, 3742), (8, 8, 100))
    door(g, 0, -424, 3690, 80, 180, MUS)
    # five unlit lamps under the gallery, and the tally of the day: a row of white discs
    for k in range(5):
        g.box(COCOA, (-400 + k * 200, -545, 3138), (5, 5, 30))
        g.box(CRM, (-400 + k * 200, -545, 3112), (18, 18, 26))
    for k in range(6):
        rod(g, WHT, (-250 + k * 100, -424, 2030), (-250 + k * 100, -432, 2030), 26, steps=8)
    # windows: three rows on the road face, two rows on each end
    for j, z in enumerate((2250, 2880, 3480)):
        for i in range(3):
            window(g, -250 + i * 250, -424, z, 90, 140, lit=(0.8 if (i + j) % 2 == 0 else 0.0))
    for z in (2880, 3480):
        for y in (-170, 170):
            xwindow(g, -424, y, z, 90, 140, lit=(0.7 if y < 0 else 0.0))
            xwindow(g, 424, y, z, 90, 140, lit=(0.7 if y > 0 else 0.0))
    # the blank company panel: mustard, a white ring, a cream diamond
    g.box(MUS, (0, -424, 2570), (520, 8, 250))
    rod(g, WHT, (-130, -426, 2570), (-130, -434, 2570), 86, steps=12)
    rod(g, MUS, (-130, -430, 2570), (-130, -438, 2570), 60, steps=12)
    g.box(CRM, (130, -430, 2570), (120, 8, 120), rot=(45, 0, 0))
    # the gallery round the shaft at the third band, its rail
    g.box(COCOA, (0, 0, 3160), (1120, 1120, 18))
    for s in (-1, 1):
        g.box(COCOA, (0, s * 552, 3270), (1120, 10, 10))
        g.box(COCOA, (s * 552, 0, 3270), (10, 1120, 10))
        for t in (-1, 1):
            g.box(COCOA, (s * 552, t * 552, 3220), (12, 12, 110))
    # the zig-zag stair up the road face, from the legs' top to the gallery; a ladder below it
    zs = (1900, 2330, 2760, 3160)
    for k in range(3):
        x0, x1 = (-380, 380) if k % 2 == 0 else (380, -380)
        tilt_box(g, COCOA, (x0, -476, zs[k]), (x1, -476, zs[k + 1]), 80, 12)
        tilt_box(g, CRM, (x0, -512, zs[k] + 100), (x1, -512, zs[k + 1] + 100), 8, 8)
        g.box(COCOA, (x1, -476, zs[k + 1] - 4), (120, 96, 10))
        g.box(COCOA, (x1, -470, zs[k + 1] - 60), (10, 10, 110))
    for x in (-410, -350):
        g.box(COCOA, (x, -476, 950), (10, 10, 1900))
    for k in range(9):
        g.box(COCOA, (-380, -476, 120 + k * 200), (60, 8, 8))
    # the head: a mustard single-pitch roof, a cream head house with its own, a tin dust cowl
    lean(g, TEAL, -420, 420, -420, 420, 3800, 3830, 3990, roof=MUS, over=50, t=22)
    lean(g, CRM, -200, 200, -170, 230, 3900, 4190, 4290, roof=TEAL, over=30, t=18)
    window(g, -90, -174, 4060, 80, 90, lit=0.8)
    window(g, 90, -174, 4060, 80, 90, lit=0.8)
    g.cyl(TIN, (300, 250, 3960), 46, 150, steps=8)
    g.cyl(TIN, (300, 250, 4126), 86, 44, steps=8, top_r=10)
    # the winding sheaves over the head house: two cocoa wheels in a frame
    for y in (60, 190):
        rod(g, COCOA, (-150, y - 8, 4420), (-150, y + 8, 4420), 90, steps=12)
        rod(g, CRM, (-150, y - 11, 4420), (-150, y + 11, 4420), 30, steps=8)
    for sd in (-1, 1):
        tilt_box(g, COCOA, (-150 + sd * 120, 125, 4270), (-150, 125, 4424), 16, 16)
    g.box(COCOA, (-150, 125, 4420), (14, 170, 14))
    # the mast, the four fixed arms with their balls, the pickaxe turning over them
    g.cyl(COCOA, (80, -40, 4230), 13, 280, steps=6)
    MX, MY = 80.0, -40.0
    g.box(COCOA, (MX, MY, 4380), (300, 12, 12))
    g.box(COCOA, (MX, MY, 4380), (12, 300, 12))
    for (x, y) in ((150, 0), (-150, 0), (0, 150), (0, -150)):
        g.sphere(CRM, (MX + x, MY + y, 4380), 20, steps=(3, 6))
    g.sphere(MUS, (MX, MY, 4526), 22, steps=(3, 6))
    v = sc.part(P.motion(base=(MX, MY), travel=(-1.0, 0.05)))
    v.box(MUS, (MX + 30, MY, 4470), (560, 24, 24))
    tilt_box(v, MUS, (MX + 310, MY, 4470), (MX + 236, MY, 4640), 24, 34)
    tilt_box(v, MUS, (MX + 310, MY, 4470), (MX + 236, MY, 4300), 24, 34)
    v.box(MUS, (MX + 310, MY, 4470), (44, 30, 60))
    v.sphere(MUS, (MX - 250, MY, 4470), 34, steps=(3, 6))
    # the conveyor gallery from the loading shed up to the shaft, on two trestles
    tilt_box(g, CRM, (2300, 150, 200), (425, 150, 3000), 200, 190)
    tilt_box(g, MUS, (2300, 46, 200), (425, 46, 3000), 8, 60)
    tilt_box(g, TEAL, (2300, 150, 304), (425, 150, 3104), 230, 14)
    for x in (1700.0, 1100.0):
        z = 200.0 + (2300.0 - x) / 1875.0 * 2800.0 - 110.0
        for sd in (-1, 1):
            tilt_box(g, COCOA, (x, 150 + sd * 230, -250), (x, 150 + sd * 70, z), 30, 30)
        g.box(COCOA, (x, 150, z * 0.5), (26, 310, 26))
    stilts(g, 2180, 2560, -20, 320, 150, nx=2, brace=False)
    lean(g, CRM, 2190, 2550, -10, 310, 150, 420, 500, roof=TEAL, over=24)
    door(g, 2420, -12, 150, 80, 180, MUS)
    window(g, 2280, -12, 310, 70, 70, lit=0.7)


JOBS = [
    # every building and the hero are centred on their footprint along the road (the stair counts)
    ('SM_S1D3_PayOffice', centred(pay_office, -190)), ('SM_S1D3_LampRoom', centred(lamp_room, -185)),
    ('SM_S1D3_TankHouse', centred(tank_house, 54)), ('SM_S1D3_RowHouses', centred(row_houses, 164)),
    ('SM_S1D3_Bunkhouse', centred(bunkhouse, -170)), ('SM_S1D3_CookHouse', centred(cook_house, 161)),
    ('SM_S1D3_AssayShed', centred(assay_shed, -165)), ('SM_S1D3_BandHall', centred(band_hall, -155)),
    ('SM_S1D3_CompanyStore', centred(company_store, 155)),
    ('SM_S1D3_SackBarrow', sack_barrow, STILL), ('SM_S1D3_PitProps', pit_props, STILL), ('SM_S1D3_TinBath', tin_bath, STILL),
    ('SM_S1D3_PickRack', pick_rack, STILL),
    ('SM_S1D3_WashLine', wash_line, SWAY), ('SM_S1D3_NoticeBoard', notice_board, SWAY), ('SM_S1D3_SwingSeat', swing_seat, SWAY),
    ('SM_S1D3_StiltLarder', stilt_larder, SWAY),
    ('SM_S1D3_Hopper', centred(hopper, -785)),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
