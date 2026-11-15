"""FULL THROTTLE (Unreal) the dressing, level 2: the Volta Chemical Works (3.2).

The kit that turns the plant from a road with tanks beside it into a works you drive through stage by stage: the
gate and its fences, the tank farm, the process units under their pipe racks, the reactor block, the cooling field,
the loading yards and the rail spur, the waste run, and a horizon of cooling towers, stacks and far plant.
Placed by Source/FullThrottle/FTDressL1.inl. Rebuild: import ft_dress; ft_dress.run(['chem'])

Drawn like ft_chem's props (ft_art.Model, cm, flat colours from the same night-shift palette), with the dressing's own
axes: the long side along X (X is laid along the road), the FRONT towards -Y (the placement turns it to the road),
the base on z = 0 (the table sinks a piece 15 cm into the land where it stands). The nearest pieces (road barriers,
ground pipe) are lower than the road's own barrier and carry no light: they stand 17 to 24 m from the centre line;
the fence, the drums and the signs stand from 24 m out. Horizon pieces (SM_Far*) are built 100 to 190 m tall from
z = 0, which the table stands 25 m below the road.
"""
import math
import random
import unreal
from ft_chem import (STEELB, STEELB_D, TEAL, TEAL_D, CYAN, HAZ, NAVY, MAG, VIOLET, WHITE, ORANGE, RED, ladder)
from ft_car import BLACK, STEEL, GLASS
from ft_desert import rod, tilt_box

CONC = '#3c4a78'      # plant concrete, night
CONC_D = '#2a3560'
STEAM = '#d8d0f0'
WARM = '#ffe9a8'      # a lit office window
RUST = '#a8552f'
QUERY = unreal.GeometryScript_MeshQueries


# ------------------------------------------------------------------------------------------ helpers
def px(m, col, x0, x1, y, z, r, steps=10, glow=0.0):
    """A pipe along X."""
    m.cyl(col, (x0, y, z), r, x1 - x0, rot=(-90, 0, 0), steps=steps, glow=glow)


def py(m, col, x, y0, y1, z, r, steps=10, glow=0.0):
    """A pipe (or a wheel, or a drum on its side) along Y."""
    m.cyl(col, (x, y0, z), r, y1 - y0, rot=(0, 0, 90), steps=steps, glow=glow)


def pz(m, col, x, y, z0, z1, r, steps=10, glow=0.0, top_r=None):
    m.cyl(col, (x, y, z0), r, z1 - z0, steps=steps, glow=glow, top_r=top_r)


def rail(m, x0, x1, y, z, col=HAZ, posts=4):
    """A handrail along X: a top bar and posts down to z."""
    m.box(col, ((x0 + x1) / 2.0, y, z + 100), (x1 - x0, 8, 8))
    for k in range(posts + 1):
        m.box(col, (x0 + (x1 - x0) * k / float(posts), y, z + 50), (8, 8, 100))


def ladder_y(m, x, y, z0, z1, col=HAZ, step=100):
    """A ladder on a -Y face (ft_chem's own ladder faces -X and has a rung every 50 cm: too many triangles on a
    tall piece, and too fine to read from the road)."""
    for s in (-1, 1):
        m.box(col, (x + s * 22, y, (z0 + z1) / 2.0), (7, 7, z1 - z0))
    k = z0 + 30
    while k < z1:
        m.box(col, (x, y, k), (44, 6, 6))
        k += step


def ladder_x(m, x, y, z0, z1, col=HAZ, step=100):
    """The same on a face that looks along X."""
    for s in (-1, 1):
        m.box(col, (x, y + s * 22, (z0 + z1) / 2.0), (7, 7, z1 - z0))
    k = z0 + 30
    while k < z1:
        m.box(col, (x, y, k), (6, 44, 6))
        k += step


LAND = '#1d2350'      # the works' outer land and the floor under the level (FTTrack GroundBands, level 1)
MIDLAND = '#2b3466'   # the land between the roadside and the edge: the footing of anything that stands on the land


FAR_KEEP = 3000.0     # how far a horizon piece's footing reaches past its footprint (see buried)


def buried(fn, col=MIDLAND, depth=1.0, keep=0.0):
    """Wrap a piece's draw function: afterwards a block goes under its whole footprint, as deep as the piece is tall.
    Two jobs. (1) A footing: the table stands a piece on the land at its own spot, sunk 15 cm, but flat; where the road
    climbs, the downhill end of a long piece would hang in the air. The block's top is flush with the piece's base, so
    on level land it is out of sight under the ground and on a slope it shows as the plinth the piece stands on.
    (2) The export's finish pass (ft_visual_art.refine) hangs a service panel on the -X end of a piece's bounding box,
    at a third of its height: right for a shed, wrong beside a fence, a pipe rack or a cooling tower. The panel is
    placed by the box's proportions, so with this block under the piece it lands below the base, inside the block.
    (3) keep: the block reaches this far past the footprint on every side. The track's clearance test works on a
    piece's bounding box and lets it stand 1.5 m from a road; a horizon piece is put by one stretch of road but may
    land beside another, so its footing is made 30 m wider all round and the test keeps the whole piece that far off.
    The wide footing is below the base, where it reads as the bench a mesa or a works stands on."""
    def draw(m):
        fn(m)
        lo, hi = [1e9] * 3, [-1e9] * 3
        for mesh in m.meshes.values():
            b = QUERY.get_mesh_bounding_box(mesh)
            for i, k in enumerate('xyz'):
                lo[i] = min(lo[i], getattr(b.min, k))
                hi[i] = max(hi[i], getattr(b.max, k))
        d = max(hi[2] - lo[2], 50.0) * depth
        m.box(col, ((lo[0] + hi[0]) / 2.0, (lo[1] + hi[1]) / 2.0, lo[2] - d / 2.0), (hi[0] - lo[0] + 2 * keep, hi[1] - lo[1] + 2 * keep, d))
    return draw


def tank_x(m, col, x0, x1, y, z, r, band=HAZ, n=3, steps=14):
    """A tank lying along X with end caps and bands (a tanker's barrel, a wagon's, a horizontal vessel)."""
    px(m, col, x0, x1, y, z, r, steps=steps)
    for x in (x0, x1):
        m.sphere(col, (x, y, z), r, scale=(0.35, 1, 1), steps=(5, steps))
    for k in range(n):
        xx = x0 + (x1 - x0) * (k + 0.5) / n
        px(m, band if k % 2 == 0 else BLACK, xx - 14, xx + 14, y, z, r + 4, steps=steps)


def wheels(m, xs, y_half, r=46, w=34, y0=0.0):
    for x in xs:
        for s in (-1, 1):
            py(m, BLACK, x, y0 + s * y_half - w / 2.0, y0 + s * y_half + w / 2.0, r, r, steps=10)


def drum(m, x, y, z, col, r=29, h=88, lying=False, yaw=0.0):
    if lying:
        a = math.radians(yaw)
        rod(m, col, (x, y, z + r), (x + math.cos(a) * h, y + math.sin(a) * h, z + r), r, steps=6)
    else:
        pz(m, col, x, y, z, z + h, r, steps=6)
        pz(m, BLACK, x, y, z + h * 0.5 - 4, z + h * 0.5 + 4, r + 2, steps=6)


# ------------------------------------------------------------------------------------------ the near roadside
def fence_run(m, L=1500):
    """The perimeter: mesh panels between blue posts on a concrete kerb, yellow post caps, one warning plate."""
    m.box(CONC_D, (0, 0, 14), (L, 26, 28))
    n = 5
    for k in range(n + 1):
        x = -L / 2.0 + L * k / n
        m.box(STEELB, (x, 0, 135), (12, 12, 250))
        m.box(STEELB, (x, 14, 268), (10, 40, 10), rot=(0, 0, -40))      # the outrigger that carries the wire
        m.box(HAZ, (x, 0, 256), (14, 14, 10))
    m.box(STEELB_D, (0, 0, 140), (L, 4, 190))
    m.box(STEELB, (0, 0, 240), (L, 8, 8))        # a blue rail, not a yellow one: yellow by the road is the road's own colour
    m.box(STEELB, (0, 0, 46), (L, 8, 8))
    m.box(HAZ, (-L * 0.18, -5, 150), (70, 4, 70), rot=(0, 0, 45))
    m.box(BLACK, (-L * 0.18, -8, 150), (10, 3, 36))


def jersey_row(m, L=1100):
    """Concrete road barriers end to end, chevroned, one knocked out of line."""
    n = 5
    w = L / n
    for k in range(n):
        x = -L / 2.0 + w * (k + 0.5)
        yaw = 11 if k == 3 else 0
        y = 26 if k == 3 else 0
        m.box(CONC, (x, y, 30), (w * 0.94, 60, 60), rot=(0, yaw, 0))
        m.box(CONC, (x, y, 72), (w * 0.94, 34, 28), rot=(0, yaw, 0))
        m.box(CONC_D, (x, y, 90), (w * 0.94, 22, 8), rot=(0, yaw, 0))
        m.box(HAZ if k % 2 == 0 else BLACK, (x, y - 31, 46), (w * 0.5, 3, 34), rot=(0, yaw, 0))


def drum_pile(m, seed=0):
    """Chemical drums on pallets: a stack two high, strays, one on its side."""
    r = random.Random(seed + 40)
    cols = [HAZ, TEAL, MAG, STEELB, TEAL_D, VIOLET]
    for px_, py_ in ((-110, -30), (95, 40)):
        m.box(RUST, (px_, py_, 16), (170, 150, 32))
        for i in range(2):
            for j in range(2):
                c = r.choice(cols)
                drum(m, px_ - 40 + i * 80, py_ - 36 + j * 72, 32, c)
                if r.random() < 0.6:
                    drum(m, px_ - 40 + i * 80, py_ - 36 + j * 72, 122, r.choice(cols))
    drum(m, 230, -90, 0, HAZ, lying=True, yaw=25)
    drum(m, -260, 110, 0, TEAL)
    drum(m, 10, -140, 0, MAG)
    m.box(STEELB_D, (250, 90, 45), (120, 90, 90))      # a crate
    m.box(HAZ, (250, 44, 45), (60, 3, 40))


def pipe_run(m, L=2200):
    """Ground pipework along the road: three lines on sleepers, an expansion loop, flanges and a valve."""
    for k in range(8):
        x = -L / 2.0 + L * (k + 0.5) / 8
        m.box(CONC_D, (x, 0, 22), (40, 170, 44))
    for y, r, col, z in ((-55, 24, TEAL, 70), (0, 17, HAZ, 62), (50, 13, MAG, 58)):
        px(m, col, -L / 2.0, L / 2.0, y, z, r)
        for k in range(4):
            x = -L / 2.0 + L * (k + 0.5) / 4
            px(m, BLACK, x - 6, x + 6, y, z, r + 6)
    # the expansion loop on the big line: up, along, down. Kept under the road barrier's height (1.6 m): this piece
    # stands 19 m from the centre line
    x0, x1 = -L * 0.08, L * 0.08
    for x in (x0, x1):
        pz(m, TEAL, x, -55, 70, 150, 24)
        m.sphere(TEAL, (x, -55, 150), 24, steps=(5, 10))
    px(m, TEAL, x0, x1, -55, 150, 24)
    # a hand valve on the yellow line
    pz(m, STEEL, L * 0.27, 0, 62, 130, 7, steps=6)
    m.cyl(RED, (L * 0.27, 0, 130), 34, 8, steps=10)


def haz_sign(m):
    """A warning board on two posts: a yellow diamond on navy. Unlit: it is near the road."""
    for x in (-110, 110):
        m.box(STEELB, (x, 0, 150), (12, 12, 300))
    m.box(NAVY, (0, -4, 290), (300, 10, 190))
    m.box(HAZ, (0, -10, 290), (286, 4, 176))
    m.box(NAVY, (0, -13, 290), (262, 4, 152))
    m.box(HAZ, (0, -16, 290), (96, 4, 96), rot=(0, 0, 45))
    m.box(BLACK, (0, -19, 298), (14, 4, 46))
    m.box(BLACK, (0, -19, 262), (14, 4, 12))


def valve_station(m):
    """A manifold on a concrete pad: two headers, four risers with wheel valves, a gauge panel."""
    m.box(CONC, (0, 0, 20), (460, 240, 40))
    px(m, TEAL, -210, 210, 40, 70, 26)
    px(m, STEELB, -210, 210, -40, 150, 20)
    for k in range(4):
        x = -150 + k * 100
        pz(m, STEELB, x, -40, 20, 150, 13, steps=8)
        rod(m, STEELB, (x, -40, 110), (x, 40, 70), 12, steps=6)
        m.cyl(RED, (x, -62, 120), 26, 8, rot=(0, 0, 90), steps=10)
    for x in (-210, 210):
        pz(m, TEAL, x, 40, 0, 70, 26, steps=8)
    m.box(NAVY, (170, -90, 100), (110, 30, 160))
    m.box(STEELB, (170, -106, 150), (90, 4, 40))
    for k in range(3):
        m.sphere(WHITE, (140 + k * 30, -108, 90), 9, steps=(4, 6))


def tanker_trailer(m):
    """A road tanker parked up: barrel, chassis, landing legs, a cab with its lights off."""
    tank_x(m, TEAL, -330, 360, 0, 190, 115)
    m.box(NAVY, (10, 0, 70), (760, 150, 30))
    wheels(m, (-280, -190, 230), 100)
    for s in (-1, 1):
        m.box(STEEL, (120, s * 70, 30), (16, 16, 60))
    # the cab
    m.side(STEELB, [(380, 60), (560, 60), (560, 190), (520, 250), (380, 250)], -95, 95)
    m.box(GLASS, (548, 0, 205), (30, 150, 60), rot=(-22, 0, 0))
    m.box(HAZ, (-340, 0, 84), (8, 170, 26))
    for x in (-150, 60, 250):
        pz(m, STEEL, x, 0, 300, 322, 26, steps=8)


def pump_house(m):
    """A blockhouse for the pumps: a lit window, a hazard-edged roof, vents, a pipe coming out of the side."""
    m.box(STEELB, (0, 0, 170), (560, 420, 340))
    m.box(NAVY, (0, 0, 350), (600, 460, 24))
    m.box(HAZ, (0, -231, 350), (600, 4, 24))
    m.box(WARM, (-110, -212, 200), (150, 6, 90), glow=0.55)
    m.box(NAVY, (-110, -216, 200), (10, 4, 90))
    m.box(NAVY, (150, -212, 130), (110, 6, 240))
    m.box(HAZ, (150, -216, 252), (110, 4, 14))
    for x in (-150, 60):
        pz(m, STEEL, x, 60, 362, 440, 34, steps=8)
        pz(m, NAVY, x, 60, 440, 452, 46, steps=8)
    px(m, TEAL, 280, 420, 90, 110, 30)
    pz(m, TEAL, 420, 90, 0, 110, 30, steps=8)
    m.sphere(TEAL, (420, 90, 110), 30, steps=(5, 8))


# ------------------------------------------------------------------------------------------ the middle distance
def pipe_rack(m, L=3200, H=920, W=520):
    """THE pipe rack: portal frames carrying two decks of lines along the road, a walkway on top, an expansion loop."""
    n = 4
    for k in range(n + 1):
        x = -L / 2.0 + L * k / n
        for s in (-1, 1):
            m.box(STEELB, (x, s * W / 2.0, H / 2.0), (34, 34, H))
            m.box(CONC_D, (x, s * W / 2.0, 20), (70, 70, 40))
        for z in (H * 0.5, H):
            m.box(STEELB, (x, 0, z), (30, W + 34, 30))
        if k < n:
            xm = x + L / n / 2.0
            tilt_box(m, STEELB_D, (x, -W / 2.0, H * 0.5), (x + L / n, -W / 2.0, H), 16)
            tilt_box(m, STEELB_D, (xm - L / n / 2.0, W / 2.0, H), (xm + L / n / 2.0, W / 2.0, H * 0.5), 16)
    for y, r, col, z in ((-170, 58, TEAL, H * 0.5 + 73), (-40, 36, WHITE, H * 0.5 + 51), (70, 30, HAZ, H * 0.5 + 45), (170, 26, MAG, H * 0.5 + 41),
                         (-150, 46, STEELB_D, H + 61), (-30, 30, TEAL_D, H + 45)):
        px(m, col, -L / 2.0 - 60, L / 2.0 + 60, y, z, r, steps=10)
    # the loop: the big line climbs over the top deck and comes back
    for x in (-L * 0.12, L * 0.12):
        pz(m, TEAL, x, -170, H * 0.5 + 73, H + 330, 58)
    px(m, TEAL, -L * 0.12, L * 0.12, -170, H + 330, 58)
    # the walkway and its rail, on the road side
    m.box(STEELB_D, (0, 150, H + 20), (L, 150, 10))
    rail(m, -L / 2.0, L / 2.0, 222, H + 20, posts=8)
    m.box(CYAN, (0, -W / 2.0 - 20, H * 0.5 - 30), (L * 0.5, 6, 26), glow=0.5)


def tank_cluster(m):
    """Three tanks in a bund: the wall, the linking header, a stair up the big one, a lit name band."""
    for (x, y, sx, sy) in ((0, -620, 1700, 30), (0, 620, 1700, 30), (-850, 0, 30, 1240), (850, 0, 30, 1240)):
        m.box(CONC, (x, y, 55), (sx, sy, 110))
    m.box(HAZ, (0, -637, 96), (1700, 4, 20))
    for (x, y, r, h, col, band) in ((-430, 60, 330, 760, TEAL, HAZ), (330, 150, 260, 560, STEELB, MAG), (360, -340, 170, 420, WHITE, TEAL)):
        pz(m, NAVY, x, y, 0, 30, r + 30, steps=12)
        pz(m, col, x, y, 30, 30 + h, r, steps=12)
        pz(m, band, x, y, 30 + h * 0.62, 30 + h * 0.62 + 44, r + 5, steps=12)
        m.sphere(col, (x, y, 30 + h), r, scale=(1, 1, 0.3), steps=(5, 12))
        pz(m, STEELB_D, x, y, 30 + h - 6, 30 + h + 8, r + 40, steps=12)
    ladder_y(m, -300, -246, 30, 800, step=110)
    m.box(CYAN, (-470, -272, 300), (300, 8, 70), glow=0.6)
    px(m, TEAL_D, -430, 330, -470, 70, 30)
    for x in (-430, 330):
        py(m, TEAL_D, x, -470, -300, 70, 30)


def platform(m):
    """A maintenance platform round a vessel: two decks, rails, a stair, lamps on the top rail."""
    H = 900
    for x in (-260, 260):
        for y in (-220, 220):
            m.box(STEELB, (x, y, H / 2.0), (28, 28, H))
    for z in (H * 0.5, H):
        m.box(STEELB_D, (0, 0, z), (560, 480, 14))
        for y in (-236, 236):
            rail(m, -276, 276, y, z, posts=4)
        for x in (-276, 276):
            m.box(HAZ, (x, 0, z + 100), (8, 472, 8))
    pz(m, WHITE, 0, 0, 60, H + 240, 150, steps=14)
    m.sphere(WHITE, (0, 0, H + 240), 150, scale=(1, 1, 0.45), steps=(5, 14))
    pz(m, TEAL, 0, 0, H * 0.3, H * 0.3 + 40, 156, steps=14)
    pz(m, MAG, 0, 0, H + 120, H + 150, 156, steps=14)
    for x in (-120, 120):
        pz(m, CONC_D, x, 0, 0, 60, 40, steps=8)
    # the stair: a stringer and its treads
    tilt_box(m, STEELB_D, (300, -330, 0), (300, 150, H * 0.5), 60, 10)
    tilt_box(m, HAZ, (330, -330, 100), (330, 150, H * 0.5 + 100), 8, 8)
    for s in (-1, 1):
        m.box(STEELB, (s * 276, -236, H + 190), (10, 10, 180))
        m.box(ORANGE, (s * 276, -250, H + 280), (50, 40, 16), glow=0.9)
    rod(m, TEAL_D, (150, 0, H * 0.7), (150, 300, H * 0.7), 22)
    pz(m, TEAL_D, 150, 300, 0, H * 0.7, 22, steps=8)


def loading_bay(m):
    """A road loading bay: a canopy with a lit edge over two lanes, loading arms, a tanker under them, the cabin."""
    for x in (-900, 0, 900):
        for y in (-420, 420):
            m.box(STEELB, (x, y, 330), (36, 36, 660))
    m.box(NAVY, (0, 0, 690), (2100, 1000, 60))
    m.box(CYAN, (0, -502, 690), (2100, 4, 36), glow=0.6)
    m.box(HAZ, (0, -504, 650), (2100, 4, 14))
    for x in (-600, 0, 600):
        pz(m, TEAL, x, 120, 380, 660, 26, steps=8)
        rod(m, TEAL, (x, 120, 380), (x, -60, 330), 20, steps=8)
        m.sphere(STEEL, (x, 120, 380), 30, steps=(4, 8))
        px(m, TEAL_D, x - 300, x + 300, 120, 640, 24)
    tank_x(m, WHITE, -420, 260, -110, 200, 110, band=MAG)
    m.box(NAVY, (-80, -110, 70), (760, 150, 30))
    wheels(m, (-360, -260, 180), 100, y0=-110.0)
    for s in (-1, 1):
        m.box(CONC, (0, s * 250 - 110, 21), (2000, 60, 42))
        for k in range(6):
            m.box(HAZ if k % 2 else BLACK, (-750 + k * 300, s * 250 - 110, 44), (150, 62, 4))
    m.box(STEELB, (820, 250, 180), (300, 260, 300))
    m.box(WARM, (820, 118, 210), (200, 6, 80), glow=0.55)


def warehouse(m):
    """A plant shed with a saw-tooth roof: a band of lit windows, two roller doors in hazard frames, roof fans."""
    L, D, H = 2600, 1500, 720
    m.box(STEELB_D, (0, 0, H / 2.0), (L, D, H))
    for k in range(4):
        x0 = -L / 2.0 + k * L / 4.0
        m.side(STEELB, [(x0, H), (x0 + L / 4.0, H), (x0 + L / 4.0 - 90, H + 260)], -D / 2.0, D / 2.0)
        m.side(CYAN, [(x0 + L / 4.0 - 4, H + 20), (x0 + L / 4.0, H + 20), (x0 + L / 4.0 - 80, H + 230)], -D / 2.0 + 60, D / 2.0 - 60, glow=0.35)
    m.box(WARM, (-300, -D / 2.0 - 3, H * 0.72), (L * 0.62, 6, 70), glow=0.5)
    for k in range(7):
        m.box(STEELB_D, (-300 - L * 0.31 + k * L * 0.62 / 6.0, -D / 2.0 - 6, H * 0.72), (14, 6, 70))
    for x in (-760, 80):
        m.box(NAVY, (x, -D / 2.0 - 4, 210), (520, 8, 420))
        m.box(HAZ, (x, -D / 2.0 - 8, 430), (560, 6, 22))
        for s in (-1, 1):
            m.box(HAZ, (x + s * 270, -D / 2.0 - 8, 215), (22, 6, 430))
    m.box(TEAL, (980, -D / 2.0 - 60, 140), (360, 120, 280))            # a lean-to plant room
    for x in (-900, -300, 300):
        pz(m, STEEL, x, 250, H + 120, H + 190, 70, steps=10)
        pz(m, NAVY, x, 250, H + 190, H + 200, 84, steps=10)
    pz(m, TEAL, 900, 350, H, H + 340, 130, steps=12)
    m.sphere(TEAL, (900, 350, H + 340), 130, scale=(1, 1, 0.4), steps=(5, 12))


def reactor_hall(m):
    """The reactor: a dark hall with slit windows burning violet, a containment dome, risers up the wall, a vent stack."""
    L, D, H = 2400, 1700, 1300
    m.box(STEELB_D, (0, 0, H / 2.0), (L, D, H))
    m.box(NAVY, (0, 0, H + 20), (L + 60, D + 60, 40))
    m.box(HAZ, (0, -D / 2.0 - 4, 80), (L, 8, 60))
    for k in range(7):
        x = -L / 2.0 + 240 + k * 320
        m.box(VIOLET, (x, -D / 2.0 - 3, H * 0.6), (70, 6, H * 0.5), glow=1.0)
    m.box(NAVY, (0, -D / 2.0 - 6, H * 0.88), (L * 0.94, 6, 20))
    # the containment, half out of the roof
    m.sphere(WHITE, (-500, 150, H), 620, steps=(8, 16))
    pz(m, TEAL, -500, 150, H - 10, H + 60, 640, steps=16)
    pz(m, MAG, -500, 150, H + 300, H + 330, 560, steps=16)
    m.sphere(RED, (-500, 150, H + 640), 36, steps=(4, 6), glow=1.3)
    # risers and the stack
    for k, x in enumerate((500, 640, 780)):
        pz(m, (TEAL, HAZ, TEAL_D)[k], x, -D / 2.0 - 60, 0, H + 160 - k * 120, 46, steps=8)
        m.sphere((TEAL, HAZ, TEAL_D)[k], (x, -D / 2.0 - 60, H + 160 - k * 120), 46, steps=(4, 8))
    for k in range(6):
        pz(m, WHITE if k % 2 == 0 else ORANGE, 900, 500, H + k * 260, H + (k + 1) * 260, 110 - k * 9, steps=10, top_r=110 - (k + 1) * 9)
    m.sphere(RED, (900, 500, H + 6 * 260 + 40), 34, steps=(4, 6), glow=1.3)
    m.box(NAVY, (-950, -D / 2.0 - 4, 200), (300, 8, 400))
    m.box(CYAN, (-950, -D / 2.0 - 9, 430), (300, 4, 26), glow=0.6)


def gantry_crane(m):
    """A yard crane on rails over its stacks: two portal legs, the bridge, a trolley with a load on the hook."""
    S, H, L = 1700, 1500, 500
    for x in (-L, L):
        for s in (-1, 1):
            tilt_box(m, HAZ, (x, s * S / 2.0 - s * 0, 0), (x * 0.5, s * S / 2.0, H), 46, 46)
    for s in (-1, 1):
        m.box(HAZ, (0, s * S / 2.0, 30), (2 * L + 200, 60, 60))
        m.box(BLACK, (0, s * S / 2.0, 16), (2600, 20, 32))            # the rail it runs on
    m.box(HAZ, (0, 0, H + 40), (L + 160, S + 240, 110))
    m.box(BLACK, (0, 0, H + 100), (L + 120, S * 0.5, 14))
    m.box(STEELB, (60, -260, H - 40), (240, 220, 90))                  # the trolley
    m.box(GLASS, (60, -372, H - 70), (160, 6, 60))
    for s in (-1, 1):
        m.box(BLACK, (60 + s * 60, -260, H - 400), (6, 6, 640))
    m.box(TEAL, (60, -260, H - 760), (330, 230, 200))                  # the load
    m.box(HAZ, (60, -377, H - 760), (330, 4, 30))
    # the stacks underneath
    r = random.Random(5)
    for (x, y, n) in ((-700, 200, 3), (-700, -300, 2), (650, 320, 2), (700, -200, 1), (0, 420, 2)):
        for k in range(n):
            m.box(r.choice([TEAL, MAG, STEELB, HAZ, TEAL_D, WHITE]), (x + r.uniform(-20, 20), y, 100 + k * 205), (560, 230, 200))
    m.sphere(RED, (0, S / 2.0 + 120, H + 110), 26, steps=(4, 6), glow=1.2)


def rail_spur(m):
    """A siding on its ballast with three tank wagons and a buffer stop."""
    L = 3800
    U = 24      # the ballast stands this much higher than a bare slab would: the table sinks a piece 15 cm into the land
    m.box(CONC_D, (0, 0, 8 + U / 2.0), (L, 330, 16 + U))
    for s in (-1, 1):
        m.box(STEEL, (0, s * 72, 24 + U), (L, 12, 16))
    for k in range(10):
        m.box(RUST, (-L / 2.0 + 100 + k * 400, 0, 18 + U), (44, 250, 8))
    for k, (col, band) in enumerate(((TEAL, HAZ), (WHITE, MAG), (STEELB, HAZ))):
        x = -1250 + k * 1150
        m.box(NAVY, (x, 0, 96 + U), (1040, 180, 26))
        tank_x(m, col, x - 440, x + 440, 0, 250 + U, 138, band=band, n=2, steps=10)
        for xx in (x - 315, x + 315):
            m.box(BLACK, (xx, 0, 56 + U), (200, 170, 44))              # the bogie, as one dark block
            for s in (-1, 1):
                py(m, BLACK, xx, s * 72 - 10, s * 72 + 10, 50 + U, 44, steps=6)
        pz(m, STEEL, x, 0, 380 + U, 420 + U, 46, steps=8)
        if k == 1:
            ladder_y(m, x - 300, -142, 100 + U, 380 + U, step=70)
    m.box(RED, (L / 2.0 - 60, 0, 90 + U), (40, 260, 120))
    m.box(HAZ, (L / 2.0 - 84, 0, 110 + U), (8, 260, 40))


def gasholder(m):
    """A gas holder: the bell riding in its guide frame, two lifts showing, a stair up the frame."""
    R, H = 900, 2300
    pz(m, CONC, 0, 0, 0, 60, R + 60, steps=14)
    pz(m, STEELB, 0, 0, 60, 60 + H * 0.42, R, steps=14)
    pz(m, TEAL_D, 0, 0, 60 + H * 0.42, 60 + H * 0.66, R - 40, steps=14)
    m.sphere(TEAL_D, (0, 0, 60 + H * 0.66), R - 40, scale=(1, 1, 0.22), steps=(5, 14))
    n = 12
    for k in range(n):
        a = 2 * math.pi * k / n
        x, y = math.cos(a) * (R + 46), math.sin(a) * (R + 46)
        m.box(HAZ if k % 3 == 0 else STEELB_D, (x, y, H / 2.0 + 30), (30, 30, H), rot=(0, math.degrees(a), 0))
    for z in (H * 0.45, H * 0.72, H + 20):
        pz(m, STEELB_D, 0, 0, z, z + 22, R + 62, steps=14)
    for k in range(n):
        a0, a1 = 2 * math.pi * k / n, 2 * math.pi * (k + 1) / n
        tilt_box(m, STEELB_D, (math.cos(a0) * (R + 46), math.sin(a0) * (R + 46), H * 0.72),
                 (math.cos(a1) * (R + 46), math.sin(a1) * (R + 46), H + 20), 14)
    ladder_y(m, 0, -R - 70, 60, H, step=160)
    m.box(CYAN, (0, -R - 8, H * 0.2), (R * 0.9, 8, 80), glow=0.55)
    m.sphere(RED, (0, 0, H + 80), 30, steps=(4, 6), glow=1.2)


def substation(m):
    """High voltage: three finned transformers behind a fence, a busbar gantry on insulators, a warning plate."""
    m.box(CONC, (0, 0, 20), (1700, 1000, 40))
    for k in range(3):
        x = -520 + k * 520
        m.box(STEELB, (x, 100, 190), (300, 240, 340))
        for j in range(7):
            m.box(STEELB_D, (x - 126 + j * 42, -40, 180), (10, 60, 280))
        for s in (-1, 0, 1):
            pz(m, WHITE, x + s * 90, 100, 360, 520, 22, steps=6, top_r=12)
            m.sphere(STEEL, (x + s * 90, 100, 530), 16, steps=(4, 6))
        pz(m, TEAL, x + 110, 190, 360, 470, 44, steps=8)
    for x in (-800, 800):
        m.box(STEELB, (x, 330, 420), (26, 26, 840))
        tilt_box(m, STEELB_D, (x, 330, 100), (x * 0.82, 330, 500), 14)
    m.box(STEELB, (0, 330, 840), (1660, 30, 30))
    for k in range(6):
        x = -700 + k * 280
        pz(m, WHITE, x, 330, 700, 826, 18, steps=6)
    px(m, ORANGE, -760, 760, 330, 690, 8, steps=6)
    # the fence, front and sides
    for (x, y, sx, sy) in ((0, -500, 1700, 6), (-850, 0, 6, 1000), (850, 0, 6, 1000)):
        m.box(STEELB_D, (x, y, 130), (sx, sy, 220))
        m.box(HAZ, (x, y, 244), (sx + 4, sy + 4, 8))
    m.box(HAZ, (0, -506, 150), (90, 4, 90), rot=(0, 0, 45))
    m.box(STEELB, (640, -300, 160), (240, 200, 300))
    m.box(CYAN, (640, -402, 200), (120, 4, 30), glow=0.5)


def cooling_cells(m):
    """A bank of forced-draught cooling cells: louvred box, fan stacks on top, steam standing over them."""
    L, D, H = 2400, 800, 620
    m.box(STEELB_D, (0, 0, H / 2.0 + 60), (L, D, H))
    for x in (-L / 2.0 + 60, 0, L / 2.0 - 60):
        for y in (-D / 2.0 + 60, D / 2.0 - 60):
            m.box(CONC_D, (x, y, 30), (90, 90, 60))
    for k in range(6):
        m.box(TEAL_D, (0, -D / 2.0 - 4, 140 + k * 96), (L * 0.96, 8, 30))
    for k in range(4):
        x = -L / 2.0 + L * (k + 0.5) / 4.0
        pz(m, STEEL, x, 0, H + 60, H + 300, 250, steps=10, top_r=220)
        pz(m, HAZ, x, 0, H + 300, H + 316, 228, steps=10)
        m.box(NAVY, (x, 0, H + 250), (420, 40, 16), rot=(0, 30 * k, 0))
        m.box(NAVY, (x, 0, H + 250), (420, 40, 16), rot=(0, 30 * k + 90, 0))
        for j, (dz, rr) in enumerate(((460, 210), (800, 260))):
            m.sphere(STEAM, (x + 50 * j, 60 * j, H + dz), rr, scale=(1, 1, 0.8), steps=(5, 8))
    rail(m, -L / 2.0, L / 2.0, -D / 2.0, H + 60, posts=6)
    ladder_x(m, L / 2.0 + 20, -100, 60, H + 60, step=110)
    px(m, TEAL, -L / 2.0 - 300, -L / 2.0, 200, 150, 60)
    pz(m, TEAL, -L / 2.0 - 300, 200, 0, 150, 60, steps=8)


def cooling_tower(m, H=4600):
    """A natural-draught cooling tower for the middle distance: the waisted shell on its ring of raking legs over
    the basin, a white band, a red-lit lip, steam leaning downwind. 46 m to the lip, built from z = 0."""
    r0, rw, rt = 1750, 1020, 1180
    n = 10
    for i in range(n):
        a0, a1 = 2 * math.pi * i / n, 2 * math.pi * (i + 0.5) / n
        tilt_box(m, CONC_D, (math.cos(a0) * (r0 + 70), math.sin(a0) * (r0 + 70), 0), (math.cos(a1) * (r0 - 30), math.sin(a1) * (r0 - 30), 430), 50)
    pz(m, CONC_D, 0, 0, 0, 70, r0 + 240, steps=16)
    pz(m, CONC_D, 0, 0, 410, 480, r0 + 30, steps=16)
    pz(m, CONC, 0, 0, 480, H * 0.68, r0, steps=16, top_r=rw)
    pz(m, CONC, 0, 0, H * 0.68, H, rw, steps=16, top_r=rt)
    zb = H * 0.33
    rb = r0 + (rw - r0) * (zb - 480) / (H * 0.68 - 480)
    pz(m, WHITE, 0, 0, zb, zb + 120, rb + 12, steps=16, top_r=rb - 20)
    pz(m, RED, 0, 0, H - 90, H, rt + 14, steps=16, glow=0.5)
    px(m, TEAL, -r0 - 900, -r0 + 200, -300, 150, 70, steps=8)
    pz(m, TEAL, -r0 - 900, -300, 0, 150, 70, steps=8)
    ladder_y(m, 0, -r0 - 20, 480, 1500, step=170)
    for j, (dx, dz, rr) in enumerate(((0, 350, 1000), (550, 1250, 1250), (1350, 2200, 1400), (2350, 3000, 1150))):
        m.sphere(STEAM, (dx, 140 * j, H + dz), rr, scale=(1.15, 1, 0.8), steps=(5, 8))


# ------------------------------------------------------------------------------------------ the horizon
def far_cooling(m):
    """Two cooling towers and their steam. 110 m tall, built from z = 0."""
    for (x, y, k) in ((-5200, 0, 1.0), (4700, 1800, 0.86)):
        H = 11000 * k
        r0, rw, rt = 4300 * k, 2500 * k, 2900 * k
        pz(m, CONC, x, y, 0, H * 0.66, r0, steps=20, top_r=rw)
        pz(m, CONC, x, y, H * 0.66, H, rw, steps=20, top_r=rt)
        pz(m, CONC_D, x, y, 0, 500, r0 + 60, steps=20)
        pz(m, WHITE, x, y, H * 0.30, H * 0.30 + 260, r0 - (r0 - rw) * 0.455 + 20, steps=20)
        pz(m, RED, x, y, H - 160, H, rt + 30, steps=20, glow=0.5)
        for j, (dx, dz, rr) in enumerate(((0, 900, 2500), (900, 2700, 3000), (2100, 4700, 3300), (3700, 6300, 2800), (5200, 7300, 2100))):
            m.sphere(STEAM, (x + dx * k, y + 300 * j, H + dz * k), rr * k, scale=(1.15, 1, 0.8), steps=(5, 8))
    # the pump hall between them
    m.box(STEELB_D, (-50, 1200, 900), (1600, 1800, 1800))
    for k in range(3):
        m.box(WARM, (-570 + k * 520, 296, 1150), (260, 8, 140), glow=0.6)


def far_stacks(m):
    """A boiler house and four tall banded stacks with beacons and smoke. Up to 170 m, built from z = 0."""
    m.box(STEELB_D, (0, 600, 1500), (11000, 3200, 3000))
    m.box(NAVY, (0, 600, 3060), (11300, 3500, 120))
    for k in range(10):
        m.box(WARM if k % 3 else CYAN, (-4700 + k * 1040, -1004, 1900), (520, 8, 260), glow=0.6)
    for i, (x, H) in enumerate(((-4200, 15000), (-1500, 17000), (1400, 13500), (4300, 16000))):
        n = 5
        for k in range(n):
            ra, rb = 560 - k * 48, 560 - (k + 1) * 48
            pz(m, WHITE if k % 2 == 0 else ORANGE, x, 900, 3000 + k * (H - 3000) / n, 3000 + (k + 1) * (H - 3000) / n, ra, steps=8, top_r=rb)
        m.sphere(RED, (x, 900, H + 120), 150, steps=(4, 6), glow=1.4)
        for j, (dz, rr) in enumerate(((1200, 1100), (3600, 1700))):
            m.sphere(STEAM, (x + 700 * j, 900 - 300 * j, H + dz), rr, scale=(1.2, 1, 0.8), steps=(5, 8))
    px(m, TEAL, -5600, 5600, -1300, 2200, 240, steps=10)
    for x in (-5200, -2600, 0, 2600, 5200):
        m.box(STEELB, (x, -1300, 1000), (160, 160, 2000))


def far_plant(m, seed=3):
    """A stretch of far works: columns, spheres, blocks with lit floors, a tube on a gantry. 260 m long, to 100 m."""
    r = random.Random(seed)
    blues = ['#33489e', '#2a3f8f', '#3a5ab8', '#24357a', '#4060c0']
    x = -12500
    while x < 11500:
        kind = r.random()
        y = r.choice([0, 1200, 2600])
        if kind < 0.36:
            w, h = r.choice([1800, 2400, 3000]), r.choice([3800, 5200, 6800, 8400])
            m.box(r.choice(blues), (x + w / 2.0, y + 1200, h / 2.0), (w, 2400, h))
            f = 700
            while f < h - 400:
                m.box(CYAN if int(f / 700) % 3 else HAZ, (x + w / 2.0, y - 4, f), (w * 0.72, 8, 130), glow=0.55)
                f += 760
            if r.random() < 0.5:
                pz(m, STEEL, x + w / 2.0, y + 1200, h, h + 1500, 40, steps=6)
                m.sphere(RED, (x + w / 2.0, y + 1200, h + 1560), 110, steps=(4, 6), glow=1.3)
        elif kind < 0.56:
            w, h = 1500, r.choice([8500, 10000])
            pz(m, TEAL, x + 700, y + 700, 0, h, 480, steps=8)
            m.sphere(TEAL, (x + 700, y + 700, h), 480, scale=(1, 1, 0.5), steps=(4, 8))
            for k in range(4):
                pz(m, STEELB, x + 700, y + 700, 1500 + k * (h - 2200) / 3.0, 1560 + k * (h - 2200) / 3.0, 700, steps=8)
                m.box(CYAN, (x + 700, y + 4, 1720 + k * (h - 2200) / 3.0), (320, 8, 110), glow=0.6)
        elif kind < 0.78:
            w, rr = 3000, r.choice([1100, 1400])
            m.sphere(r.choice([MAG, TEAL, '#ff7ad9']), (x + 1500, y + 1500, rr + 900), rr, steps=(6, 10))
            for s in (-1, 1):
                for t in (-1, 1):
                    m.box(STEELB, (x + 1500 + s * rr * 0.6, y + 1500 + t * rr * 0.6, 700), (90, 90, 1400))
        else:
            w, h = 1300, r.choice([9000, 11000])
            for k in range(6):
                pz(m, WHITE if k % 2 == 0 else ORANGE, x + 600, y + 600, k * h / 6.0, (k + 1) * h / 6.0, 420 - k * 32, steps=8, top_r=420 - (k + 1) * 32)
            m.sphere(RED, (x + 600, y + 600, h + 100), 120, steps=(4, 6), glow=1.3)
        x += w + r.choice([700, 1300, 1900])
    m.box(STEELB_D, (0, -500, 3000), (24000, 160, 160))
    px(m, TEAL, -12000, 12000, -500, 3260, 170, steps=10)
    for xx in range(-11000, 11001, 2750):
        m.box(STEELB_D, (xx, -500, 1500), (150, 150, 3000))


def far_gasworks(m):
    """Three great gas spheres on legs and a flare derrick with its flame. To 140 m, built from z = 0."""
    for (x, y, rr, col) in ((-5200, 400, 2500, MAG), (0, 1500, 3000, TEAL), (5600, 200, 2200, '#ff7ad9')):
        cz = rr + 1700
        m.sphere(col, (x, y, cz), rr, steps=(7, 12))
        pz(m, WHITE, x, y, cz - 80, cz + 80, rr + 30, steps=12)
        for k in range(6):
            a = 2 * math.pi * k / 6
            m.box(STEELB, (x + math.cos(a) * rr * 0.8, y + math.sin(a) * rr * 0.8, cz / 2.0), (130, 130, cz))
        m.sphere(RED, (x, y, cz + rr + 80), 100, steps=(4, 6), glow=1.3)
    # the derrick
    X, H = 9800, 14000
    for s in (-1, 1):
        for t in (-1, 1):
            tilt_box(m, STEELB, (X + s * 900, t * 900, 0), (X + s * 150, t * 150, H), 110)
    for k in range(1, 6):
        z = H * k / 6.0
        w = 900 - 750 * k / 6.0
        m.box(STEELB_D, (X, 0, z), (2 * w + 110, 2 * w + 110, 70))
    pz(m, STEEL, X, 0, 0, H + 600, 90, steps=8)
    m.sphere(ORANGE, (X, 0, H + 900), 380, scale=(1, 1, 1.5), steps=(5, 8), glow=1.5)
    m.sphere(HAZ, (X, 0, H + 760), 220, scale=(1, 1, 1.3), steps=(4, 6), glow=1.6)
    px(m, TEAL, -7400, X, -300, 900, 150, steps=8)


JOBS = [
    ('SM_FenceRun', buried(fence_run)), ('SM_JerseyRow', buried(jersey_row)), ('SM_DrumPile', buried(drum_pile)),
    ('SM_DrumPile2', buried(lambda m: drum_pile(m, 7))), ('SM_PipeRun', buried(pipe_run)), ('SM_HazSign', buried(haz_sign)),
    ('SM_ValveStation', buried(valve_station)), ('SM_TankerTrailer', buried(tanker_trailer)), ('SM_PumpHouse', buried(pump_house)),
    ('SM_PipeRack', buried(pipe_rack)), ('SM_TankCluster', buried(tank_cluster)), ('SM_Platform', buried(platform)),
    ('SM_LoadingBay', buried(loading_bay)), ('SM_Warehouse', buried(warehouse)), ('SM_ReactorHall', buried(reactor_hall)),
    ('SM_GantryCrane', buried(gantry_crane)), ('SM_RailSpur', buried(rail_spur)), ('SM_Gasholder', buried(gasholder)),
    ('SM_Substation', buried(substation)), ('SM_CoolingCells', buried(cooling_cells)), ('SM_CoolingTower', buried(cooling_tower)),
    # the horizon stands on the floor past the land's edge: its footing takes the floor's colour
    ('SM_FarCooling', buried(far_cooling, LAND, keep=FAR_KEEP)), ('SM_FarStacks', buried(far_stacks, LAND, keep=FAR_KEEP)),
    ('SM_FarPlant', buried(far_plant, LAND, keep=FAR_KEEP)), ('SM_FarPlant2', buried(lambda m: far_plant(m, 12), LAND, keep=FAR_KEEP)),
    ('SM_FarGasworks', buried(far_gasworks, LAND, keep=FAR_KEEP)),
]
