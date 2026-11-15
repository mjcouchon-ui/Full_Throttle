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
    m.box(WARM, (60, -374, H - 104), (120, 8, 22), glow=0.55)          # the trolley's lamp: it travels with it (third pass)
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


# ------------------------------------------------------------------------------------------ the ground plane (second pass)
# Flat pieces (nothing over 2 m, so the track's sight-line rule leaves them alone) that turn the land beside the road
# from one dark colour into ground somebody built on: yards, lots, ponds, bunds, pipe fields, plant roads, sidings.
# They are what a banked, looping or airborne camera sees, so they are drawn to be read from above as much as from
# the side: pale slabs on the dark land, chunky paint, a few saturated liquids. Each has its own slab height, so two
# that overlap never share a top (no flicker). The table puts them 26 m out and beyond (FTDressL1.inl).
PAVE = '#7f8fc6'      # floodlit plant concrete: slabs, pads, kerbs. Much paler than the land, so a slab reads as made
PAVE_D = '#56659f'    # the same in shade: aprons, rims, ballast
POOL = 0.35           # how much a pond's liquor glows (the ponds stand 40 m out and beyond)


def flat(fn, foot=60.0, col=MIDLAND, keep=0.0):
    """Wrap a ground piece's draw function: a footing `foot` cm deep under its whole footprint, in the land's colour.
    The track stands a piece level where it is put, sunk 15 cm; where the land falls gently along the road the
    downhill edge would show daylight under it, and with this it shows a low terrace instead. Shallow on purpose:
    with it a ground piece is still under 2.5 m from its lowest point to its top (the table keeps these pieces off
    the steep ramps, where no footing would save a level slab).
    keep (third pass): two small cubes far under the piece's opposite corners, this much beyond it. The track's
    clearance test works on the bounding box and keeps it 15.6 m from every centre line; it knows nothing of light. A
    ground piece that glows (a pond, the helipad's lamps, the car park's booth) is given LIT_KEEP, so the test keeps
    it 30 m from every stretch of road, whichever stretch it happens to land beside."""
    def draw(m):
        fn(m)
        lo, hi = [1e9] * 3, [-1e9] * 3
        for mesh in m.meshes.values():
            b = QUERY.get_mesh_bounding_box(mesh)
            for i, k in enumerate('xyz'):
                lo[i] = min(lo[i], getattr(b.min, k))
                hi[i] = max(hi[i], getattr(b.max, k))
        m.box(col, ((lo[0] + hi[0]) / 2.0, (lo[1] + hi[1]) / 2.0, lo[2] - foot / 2.0), (hi[0] - lo[0], hi[1] - lo[1], foot))
        if keep > 0.0:
            # two 10 cm cubes at opposite corners, 4 m down: the bounding box the test reads, and nothing to see
            # (a slab that wide came out of the ground at its downhill end wherever the land falls along the road)
            for sx, sy in ((-1, -1), (1, 1)):
                m.box(col, ((lo[0] + hi[0]) / 2.0 + sx * ((hi[0] - lo[0]) / 2.0 + keep - 5.0), (lo[1] + hi[1]) / 2.0 + sy * ((hi[1] - lo[1]) / 2.0 + keep - 5.0),
                            lo[2] - foot - 400.0), (10.0, 10.0, 10.0))
    return draw


LIT_KEEP = 1450.0      # 30 m, less the 15.6 m the clearance test gives every piece


def blob(r, rx, ry, n=9, wob=0.2, cx=0.0, cy=0.0):
    """An irregular outline (a lagoon, a puddle): n points round an ellipse, each pushed in or out a little."""
    return [(cx + math.cos(2 * math.pi * k / n) * rx * (1.0 + r.uniform(-wob, wob)),
             cy + math.sin(2 * math.pi * k / n) * ry * (1.0 + r.uniform(-wob, wob))) for k in range(n)]


def slab(m, L, D, T, col=PAVE, rim=PAVE_D, lip=30.0):
    """A yard's slab: the pale top on a darker apron that shows as a rim all round it."""
    m.box(rim, (0, 0, (T - 12) / 2.0), (L + 2 * lip, D + 2 * lip, T - 12))
    m.box(col, (0, 0, T / 2.0), (L, D, T))


def yard_lot(m, seed=0, L=3000, D=2000, T=46):
    """A maintenance yard: a slab with a blocked front edge and painted bays, and what is kept on it: pipe in chocked
    stacks (seed 0), skips and crates (1), drums on pallets and cable reels (2)."""
    r = random.Random(seed + 61)
    slab(m, L, D, T)
    n = 10
    for k in range(n):       # the front edge, blocked yellow and dark: chunky enough to read from the road
        m.box(HAZ if k % 2 == 0 else NAVY, (-L / 2.0 + L * (k + 0.5) / n, -D / 2.0 + 50, T + 2), (L / n, 100, 4))
    for k in range(1, 5):    # bays
        m.box(WHITE, (-L / 2.0 + L * k / 5.0, -D * 0.14, T + 2), (40, D * 0.46, 4))
    kind = seed % 3
    if kind == 0:
        for (x, y, rows, col) in ((-L * 0.26, D * 0.24, 2, TEAL), (L * 0.16, D * 0.26, 2, STEELB), (L * 0.3, -D * 0.08, 1, WHITE)):
            for i in range(rows):
                for j in range(3 - i):
                    px(m, col, x - 520, x + 520, y + (j - (2 - i) / 2.0) * 80, T + 38 + i * 66, 38, steps=6)
            for sx in (-1, 1):
                m.box(RUST, (x + sx * 400, y, T + 8), (26, 300, 16))
    elif kind == 1:
        for k in range(5):
            x, y = -L * 0.38 + k * L * 0.19, r.uniform(D * 0.04, D * 0.3)
            m.side((TEAL, MAG, STEELB, HAZ, TEAL_D)[k], [(-190, T), (190, T), (240, T + 126), (-240, T + 126)], y - 100, y + 100, loc=(x, 0, 0))
            m.box(NAVY, (x, y, T + 131), (486, 206, 10))
        for k in range(3):
            m.box((STEELB_D, TEAL_D, RUST)[k], (-L * 0.3 + k * L * 0.3, -D * 0.3, T + 60), (150, 120, 120))
    else:
        for (ax, ay) in ((-L * 0.3, D * 0.2), (-L * 0.02, D * 0.27)):
            m.box(RUST, (ax, ay, T + 8), (190, 190, 16))
            for i in range(2):
                for j in range(2):
                    drum(m, ax - 44 + i * 88, ay - 44 + j * 88, T + 16, r.choice([HAZ, TEAL, MAG, VIOLET, STEELB]))
        for k, x in enumerate((L * 0.3, L * 0.4)):       # cable reels standing on their rims
            y = -D * 0.1 + k * 320
            py(m, RUST, x, y - 54, y + 54, T + 66, 66, steps=10)
            py(m, STEELB_D, x, y - 40, y + 40, T + 66, 38, steps=8)


def car_park(m, L=3200, D=1900, T=50):
    """The shift's car park: asphalt in a pale kerb, bays in white, cars nose to the road, one bay hatched off, and the
    attendant's booth with its window lit (so the table keeps this one 40 m out)."""
    r = random.Random(23)
    m.box(PAVE, (0, 0, T / 2.0), (L, D, T))
    m.box(NAVY, (0, 0, T + 3), (L - 90, D - 90, 6))
    z = T + 6
    n = 8
    w = (L - 500) / n
    cols = [TEAL, MAG, WHITE, STEELB, HAZ, TEAL_D, VIOLET, WHITE]
    for k in range(n + 1):
        m.box(WHITE, (-L / 2.0 + 140 + k * w, -D * 0.18, z + 1), (34, 560, 4))
    for k in range(n):
        x = -L / 2.0 + 140 + (k + 0.5) * w
        if k == 5:           # the hatched bay
            for j in range(3):
                m.box(HAZ, (x, -D * 0.18 - 170 + j * 170, z + 1), (w * 0.8, 46, 4), rot=(0, 35, 0))
            continue
        if k == 2:
            continue         # an empty bay
        y = -D * 0.18 + r.uniform(-30, 30)
        m.box(BLACK, (x, y, z + 14), (176, 400, 28))
        m.box(cols[k], (x, y, z + 58), (190, 430, 64))
        m.box(GLASS, (x, y + 26, z + 110), (170, 220, 44))
    m.box(WHITE, (0, D * 0.3, z + 1), (L * 0.7, 34, 4))                  # the lane's line
    m.box(STEELB, (L / 2.0 - 260, D / 2.0 - 250, z + 62), (220, 220, 124))
    m.box(NAVY, (L / 2.0 - 260, D / 2.0 - 250, z + 130), (260, 260, 12))
    m.box(WARM, (L / 2.0 - 260, D / 2.0 - 363, z + 78), (140, 6, 56), glow=0.55)


def pond(m, a=MAG, b=CYAN, L=2600, D=1700, H=64):
    """Twin settling ponds: a concrete rim, two basins of process liquor (they glow a little: the table keeps them
    40 m out), the weir between them with foam below it, feed pipes over the back wall, a slab across for the operator."""
    W = 90
    m.box(PAVE_D, (0, 0, 14), (L + 70, D + 70, 28))
    for (x, y, sx, sy) in ((0, -D / 2.0 + W / 2.0, L, W), (0, D / 2.0 - W / 2.0, L, W), (-L / 2.0 + W / 2.0, 0, W, D),
                           (L / 2.0 - W / 2.0, 0, W, D), (0, 0, W, D)):
        m.box(PAVE, (x, y, H / 2.0), (sx, sy, H))
    for s, col in ((-1, a), (1, b)):
        m.box(col, (s * L / 4.0, 0, (H - 18) / 2.0), (L / 2.0 - W, D - 2 * W + 2, H - 18), glow=POOL)
    m.box(WHITE, (W / 2.0 + 60, 0, H - 14), (120, D * 0.3, 10))
    for k in range(3):
        m.sphere(WHITE, (W / 2.0 + 160 + k * 90, (k - 1) * D * 0.12, H - 18), 62, scale=(1, 1, 0.3), steps=(4, 6))
    for k, x in enumerate((-L * 0.36, -L * 0.2)):
        col = TEAL if k == 0 else TEAL_D
        py(m, col, x, D / 2.0 - 300, D / 2.0 + 240, H + 46, 36, steps=8)
        pz(m, col, x, D / 2.0 + 240, 0, H + 46, 36, steps=8)
        pz(m, col, x, D / 2.0 - 300, H - 18, H + 46, 36, steps=8)
    m.box(STEELB_D, (L / 4.0 + 120, 0, H + 7), (170, D - 40, 14))


def clarifier(m, R=950, H=72, liquor=TEAL):
    """A clarifier: a round tank let into the ground, the rake bridge from its hub to the rim and the drive on the end
    of it (the only steel-grey and orange on the piece: life() turns just those, slowly), foam round the hub, the feed
    pipe coming up to the wall."""
    pz(m, PAVE_D, 0, 0, 0, 30, R + 160, steps=16)
    n = 16
    seg = 2 * math.pi * (R + 40) / n + 8
    for k in range(n):
        a = 2 * math.pi * (k + 0.5) / n
        m.box(PAVE, (math.cos(a) * (R + 40), math.sin(a) * (R + 40), H / 2.0), (80, seg, H), rot=(0, math.degrees(a), 0))
    pz(m, liquor, 0, 0, 0, H - 22, R + 6, steps=16, glow=POOL)
    pz(m, PAVE, 0, 0, 0, 150, 110, steps=10)
    pz(m, WHITE, 0, 0, H - 22, H - 14, 300, steps=10)
    m.box(STEEL, (R / 2.0 + 20, 0, 150), (R + 60, 110, 24))
    m.box(STEEL, (R / 2.0 + 60, 0, 112), (R - 220, 18, 56))
    m.box(ORANGE, (R + 20, 0, 158), (120, 130, 50))
    py(m, TEAL_D, -R * 0.4, R * 0.94, R + 320, 60, 36, steps=8)
    pz(m, TEAL_D, -R * 0.4, R + 320, 0, 60, 36, steps=8)


def tank_pad(m, L=3000, D=2000):
    """An empty bund: the wall, the dark floor, two ring foundations (one still carrying the bottom course of its
    tank), the manifold along the front and the stubs to it, a puddle of the chemical in the low corner."""
    W, H = 50, 96
    m.box(PAVE_D, (0, 0, 18), (L, D, 36))
    for (x, y, sx, sy) in ((0, -D / 2.0, L + W, W), (0, D / 2.0, L + W, W), (-L / 2.0, 0, W, D), (L / 2.0, 0, W, D)):
        m.box(PAVE, (x, y, H / 2.0), (sx, sy, H))
    m.box(HAZ, (0, -D / 2.0 - W / 2.0 - 2, H - 18), (L + W, 4, 24))
    ym = -D / 2.0 + 240
    px(m, TEAL_D, -L / 2.0 + 220, L / 2.0 - 220, ym, 74, 32, steps=8)
    for (x, y, R, stub, col) in ((-650, 140, 560, True, TEAL), (720, 60, 400, False, MAG)):
        pz(m, PAVE, x, y, 30, 66, R, steps=14)
        pz(m, PAVE_D, x, y, 66, 70, R * 0.3, steps=10)
        if stub:
            pz(m, col, x, y, 66, 166, R - 70, steps=14)
            pz(m, NAVY, x, y, 166, 170, R - 110, steps=14)
        py(m, col, x, ym, y - R + 60, 72, 26, steps=6)
    r = random.Random(8)
    m.plan(VIOLET, blob(r, 300, 190, 8, 0.25, L * 0.3, D * 0.3), 36, 42, glow=POOL)


def pipe_field(m, W=1500, L=3200):
    """A field of process lines running straight out from the road to the units behind: five pipes on sleepers, going
    to ground at each end, flanges, hand wheels, and a slab bridge over them for the operators."""
    n = 6
    for k in range(n + 1):
        m.box(PAVE_D, (0, -L / 2.0 + 220 + (L - 440) * k / n, 22), (W, 70, 44))
    for i, (x, r, col) in enumerate(((-560, 42, TEAL), (-290, 30, MAG), (-50, 38, WHITE), (220, 30, TEAL_D), (490, 42, STEELB))):
        z = 44 + r
        py(m, col, x, -L / 2.0 + 120, L / 2.0 - 120, z, r, steps=6)
        for y in (-L / 2.0 + 120, L / 2.0 - 120):
            pz(m, col, x, y, 0, z + r * 0.7, r, steps=6)
        for y in (-L * 0.24, L * 0.3):
            py(m, BLACK, x, y - 9, y + 9, z, r + 9, steps=6)
        if i in (1, 3):
            m.cyl(RED, (x, L * 0.08, z + r + 14), 32, 8, steps=8)
            pz(m, STEEL, x, L * 0.08, z, z + r + 14, 7, steps=4)
    # the bridge: deck, two ramps, solid side plates
    yb, zb = -L * 0.06, 140
    m.box(STEELB_D, (0, yb, zb), (W + 60, 250, 16))
    for s in (-1, 1):
        tilt_box(m, STEELB_D, (s * (W / 2.0 + 30), yb, zb), (s * (W / 2.0 + 330), yb, 0), 250, 14)
        m.box(STEELB, (0, yb + s * 122, zb + 22), (W + 60, 10, 30))


def access_road(m, W=760, L=3400):
    """A plant road running out from the perimeter towards the units: asphalt between pale kerbs, a centre line, a
    crossing and a stop bar at the road end, a boom across it (down), a turning head at the far end. It runs along Y:
    the table stands its near end 26 m from the centre line."""
    m.box(PAVE, (0, 0, 21), (W + 130, L, 42))
    m.box(NAVY, (0, 0, 44), (W, L - 70, 6))
    for k in range(4):
        m.box(WHITE, (0, -L / 2.0 + 1050 + k * 560, 49), (36, 300, 4))
    for k in range(5):
        m.box(WHITE, (-W / 2.0 + 76 + k * 152, -L / 2.0 + 560, 49), (84, 250, 4))
    m.box(WHITE, (0, -L / 2.0 + 300, 49), (W - 60, 60, 4))
    m.box(STEELB, (-W / 2.0 - 30, -L / 2.0 + 150, 44 + 55), (44, 44, 110))
    for k in range(4):
        m.box(WHITE if k % 2 == 0 else NAVY, (-W / 2.0 + (k + 0.5) * (W + 40) / 4.0, -L / 2.0 + 150, 44 + 92), ((W + 40) / 4.0, 22, 22))
    m.box(PAVE, (0, L / 2.0 - 380, 21), (W + 760, 760, 42))
    m.box(NAVY, (0, L / 2.0 - 380, 44), (W + 620, 620, 6))
    m.box(WHITE, (0, L / 2.0 - 380, 49), (300, 60, 4), rot=(0, 45, 0))
    m.box(WHITE, (0, L / 2.0 - 380, 49), (300, 60, 4), rot=(0, -45, 0))


def heli_pad(m, R=1050, T=56):
    """The works' helipad: an octagon of slab, the yellow ring, the H, four red edge lights (steady)."""
    def octagon(rr):
        return [(math.cos(math.pi / 8 + k * math.pi / 4) * rr, math.sin(math.pi / 8 + k * math.pi / 4) * rr) for k in range(8)]
    m.plan(PAVE_D, octagon(R + 70), 0, T - 12)
    m.plan(PAVE, octagon(R), 0, T)
    m.plan(HAZ, octagon(R * 0.8), T, T + 3)
    m.plan(PAVE, octagon(R * 0.68), T, T + 6)
    for x in (-170, 170):
        m.box(WHITE, (x, 0, T + 8), (90, 540, 4))
    m.box(WHITE, (0, 0, T + 8), (340, 90, 4))
    for k in range(4):
        a = math.pi / 4 + k * math.pi / 2
        m.sphere(RED, (math.cos(a) * R * 0.9, math.sin(a) * R * 0.9, T + 6), 30, scale=(1, 1, 0.6), steps=(4, 6), glow=1.2)


def lagoon(m, seed=0, RX=1500, RY=1050):
    """A waste lagoon: a dark liner bermed up out of the ground, the chemical standing in it (glowing a little), drums
    half sunk, the outfall pipe and its foam at the back, concrete blocks along the road side."""
    r = random.Random(seed + 17)
    out = blob(r, RX, RY, 11, 0.16)
    m.plan(NAVY, out, 0, 52)
    m.plan(VIOLET, [(x * 0.86, y * 0.86) for x, y in out], 52, 58, glow=POOL)
    for k in range(5):
        a, q = r.uniform(0, 2 * math.pi), r.uniform(0.15, 0.6)
        drum(m, math.cos(a) * RX * q, math.sin(a) * RY * q, 30, r.choice([HAZ, TEAL, MAG, STEELB]), lying=(k % 2 == 0), yaw=r.uniform(0, 180))
    py(m, TEAL_D, RX * 0.2, RY * 0.5, RY + 320, 98, 44, steps=8)
    pz(m, TEAL_D, RX * 0.2, RY + 320, 0, 98, 44, steps=8)
    m.sphere(WHITE, (RX * 0.2, RY * 0.42, 60), 150, scale=(1, 1.3, 0.25), steps=(4, 8))
    for k in range(4):
        x = -RX * 0.6 + k * RX * 0.4
        m.box(PAVE, (x, -RY - 150, 40), (300, 70, 80))
        m.box(HAZ if k % 2 == 0 else NAVY, (x, -RY - 187, 46), (150, 4, 40))


def rail_fan(m, L=4000, W=780, T=44):
    """A pair of sidings on their ballast: the turnout where they part, buffer stops, a flat wagon loaded with pipe.
    Heavy rails and a few wide sleepers, so the track still reads (and does not turn to scribble) from the road."""
    m.box(PAVE_D, (0, 0, T / 2.0), (L, W, T))
    for y in (-190, 190):
        for s in (-1, 1):
            m.box(STEEL, (0, y + s * 80, T + 9), (L - 200, 24, 18))
        for k in range(9):
            m.box(RUST, (-L / 2.0 + 300 + k * (L - 600) / 8.0, y, T + 3), (90, 270, 6))
        m.box(RUST, (L / 2.0 - 90, y, T + 60), (50, 250, 120))
        m.box(WHITE, (L / 2.0 - 118, y, T + 70), (8, 250, 34))
    for s in (-1, 1):
        tilt_box(m, STEEL, (-L * 0.42, -190 + s * 80, T + 9), (-L * 0.12, 190 + s * 80, T + 9), 24, 18)
    x = L * 0.14
    m.box(NAVY, (x, 190, T + 70), (1040, 240, 26))
    for xx in (x - 330, x + 330):
        m.box(BLACK, (xx, 190, T + 34), (200, 190, 44))
    for j in range(3):
        px(m, (TEAL, WHITE, TEAL)[j], x - 480, x + 480, 190 + (j - 1) * 66, T + 113, 30, steps=6)


def canal(m, L=3600, W=640, H=84):
    """A length of the cooling-water channel between two culvert headwalls: the water (life() makes it run), two weirs
    with foam below them, a pipe crossing on its saddles."""
    m.box(PAVE_D, (0, 0, 14), (L + 60, W + 260, 28))
    for s in (-1, 1):
        m.box(PAVE, (0, s * (W / 2.0 + 35), H / 2.0), (L, 70, H))
        m.box(PAVE, (s * (L / 2.0 - 60), 0, 80), (120, W + 140, 160))
        m.box(NAVY, (s * (L / 2.0 - 124), 0, 64), (10, W * 0.6, 72))
        m.box(PAVE_D, (L * 0.03, s * (W / 2.0 + 150), (H + 16) / 2.0), (90, 90, H + 16))
    m.box(CYAN, (0, 0, (H - 30) / 2.0), (L - 130, W + 2, H - 30))
    for x in (-L * 0.18, L * 0.2):
        m.box(PAVE, (x, 0, (H - 12) / 2.0), (60, W, H - 12))
        m.box(WHITE, (x + 85, 0, H - 28), (110, W * 0.9, 6))
    py(m, TEAL, L * 0.03, -W / 2.0 - 200, W / 2.0 + 200, H + 50, 34, steps=8)


# ------------------------------------------------------------------------------------------ what stands in the sky (second pass)
def catwalk(m, L=4200, H=1350):
    """A lit catwalk: a walkway bridge between two stair towers, solid parapets, a strip of light under the deck its
    whole length (life() runs it along), landing windows and a lamp on each tower, a line slung underneath."""
    for s in (-1, 1):
        x = s * (L / 2.0 - 190)
        m.box(STEELB_D, (x, 0, (H + 260) / 2.0), (380, 380, H + 260))
        m.box(NAVY, (x, 0, H + 271), (430, 430, 22))
        for k in range(3):
            m.box(WARM, (x, -192, 300 + k * 420), (150, 6, 110), glow=0.55)
        m.box(ORANGE, (x, -200, H + 215), (90, 40, 30), glow=0.9)
    m.box(STEELB, (0, 0, H), (L - 380, 260, 50))
    for y in (-124, 124):
        m.box(STEELB, (0, y, H + 85), (L - 380, 14, 120))
    m.box(CYAN, (0, -131, H - 40), (L - 520, 6, 30), glow=0.5)
    px(m, TEAL, -L / 2.0 + 380, L / 2.0 - 380, 60, H - 60, 34, steps=8)
    for x in (-L / 6.0, L / 6.0):
        m.box(STEELB_D, (x, 0, (H - 25) / 2.0), (60, 200, H - 25))


def column_trio(m):
    """Three distillation columns on one plinth, 88, 72 and 56 m: platforms ringing each one with a band of light under
    every platform (life() sends the light up them), the overhead line from the tallest down to the next, a vent and
    its steam on the middle one, a bridge between two, a beacon. Tall enough to stand in the sky of a banked turn."""
    m.box(PAVE_D, (0, 0, 40), (3000, 1700, 80))
    cols = ((-950, 80, 300, 8800, WHITE, TEAL), (60, -120, 250, 7200, TEAL, MAG), (980, 100, 210, 5600, STEELB, HAZ))
    for i, (x, y, R, H, col, band) in enumerate(cols):
        pz(m, PAVE, x, y, 80, 260, R + 90, steps=8)
        pz(m, col, x, y, 260, H, R, steps=8)
        m.sphere(col, (x, y, H), R, scale=(1, 1, 0.5), steps=(4, 8))
        nr = 3 if i == 2 else 4
        for k in range(nr):
            z = 260 + (H - 700) * (k + 1) / float(nr)
            pz(m, STEELB_D, x, y, z, z + 36, R + 180, steps=8)
            pz(m, CYAN, x, y, z - 96, z - 20, R + 14, steps=8, glow=0.5)
        pz(m, band, x, y, 700, 800, R + 10, steps=8)
    rod(m, WHITE, (-950, 80, 8900), (60, 200, 6900), 60, steps=6)
    pz(m, WHITE, 60, 200, 5200, 6900, 60, steps=6)
    tilt_box(m, STEELB_D, (60, -120, 3300), (980, 100, 3300), 110, 130)
    px(m, TEAL_D, 60, 980, -10, 3410, 44, steps=6)
    pz(m, STEEL, 60, -120, 7300, 7750, 64, steps=6)
    for j, (dx, dz, rr) in enumerate(((0, 330, 340), (260, 950, 430), (720, 1650, 490), (1350, 2350, 400))):
        m.sphere(STEAM, (60 + dx, -120 + 60 * j, 7700 + dz), rr, scale=(1.15, 1, 0.8), steps=(4, 6))
    m.sphere(RED, (-950, 80, 8800 + 210), 62, steps=(4, 6), glow=1.3)


def reactor_core(m):
    """THE reactor, the one landmark of the level: a containment dome on a pale concrete drum 84 m across and 110 m
    tall (violet slit windows up it, the control gallery's lit band under the roof ring, a corona of violet at the
    dome's foot), four banded stacks at its corners, and the needle on the dome with its column of violet light and
    three rings round the light (life() sends the light up them). 147 m to the top of the dome, 297 m to the top of
    the light. The table stands ONE of these in the reactor block (and one in the finale's), dead ahead of the jump at
    4.4 km, a kilometre out: FTDressL1.inl says why there.
    The first version (a dark drum 56 m tall, a thin needle of light) was lost a kilometre out: dark on a black sky,
    and no taller than the stacks and cooling towers in front of it. So the drum is pale and twice the height, and
    the light is a column 10 m through."""
    R, HD = 4200, 11000
    pz(m, PAVE_D, 0, 0, 0, 500, R + 900, steps=20)
    pz(m, PAVE, 0, 0, 500, HD, R, steps=20)
    for z in (HD * 0.3, HD * 0.62):
        pz(m, STEELB_D, 0, 0, z, z + 260, R + 14, steps=20)
    n = 10
    for k in range(n):
        a = 2 * math.pi * (k + 0.5) / n
        m.box(VIOLET, (math.cos(a) * (R - 30), math.sin(a) * (R - 30), 500 + HD * 0.44), (120, 420, HD * 0.7), rot=(0, math.degrees(a), 0), glow=1.0)
    pz(m, WARM, 0, 0, HD - 620, HD - 240, R + 30, steps=20, glow=0.6)
    pz(m, NAVY, 0, 0, HD - 240, HD + 60, R + 420, steps=20)
    RD = R - 200
    m.sphere(WHITE, (0, 0, HD), RD, scale=(1, 1, 0.92), steps=(8, 20))
    pz(m, VIOLET, 0, 0, HD + 40, HD + 320, RD + 70, steps=20, glow=1.0)
    pz(m, MAG, 0, 0, HD + RD * 0.92 * 0.6, HD + RD * 0.92 * 0.6 + 200, RD * 0.8 + 30, steps=20)
    top = HD + RD * 0.92
    for k in range(4):
        a = math.pi / 4 + k * math.pi / 2
        x, y = math.cos(a) * (R + 1000), math.sin(a) * (R + 1000)
        for j in range(5):
            pz(m, WHITE if j % 2 == 0 else ORANGE, x, y, 500 + j * 3100, 500 + (j + 1) * 3100, 700 - j * 60, steps=8, top_r=700 - (j + 1) * 60)
        m.sphere(RED, (x, y, 500 + 5 * 3100 + 140), 150, steps=(4, 6), glow=1.3)
    pz(m, STEEL, 0, 0, top - 150, top + 4200, 300, steps=8, top_r=110)
    pz(m, VIOLET, 0, 0, top + 600, top + 15000, 520, steps=8, top_r=150, glow=1.4)
    for (dz, rr) in ((3200, 2600), (7200, 2000), (11200, 1400)):
        nseg = 12
        for k in range(nseg):
            a = 2 * math.pi * (k + 0.5) / nseg
            m.box(VIOLET, (math.cos(a) * rr, math.sin(a) * rr, top + dz), (260, 2 * math.pi * rr / nseg + 14, 220), rot=(0, math.degrees(a), 0), glow=1.4)


# ------------------------------------------------------------------------------------------ what moves (second pass)
def life(build=True):
    """What moves on this kit: ft_life.animate() on the slots that should (steam, beacons, lights that run, liquor,
    the clarifier's rake, the crane's trolley). The export puts the palette's still instances back on a mesh's slots,
    so run this again after any export:   import ft_dress_chem; ft_dress_chem.life()
    Near the road nothing is lit and nothing swings: the nearest moving light (the catwalk's) stands 40 m out; at the
    sight line there is only colour running down a pipe and two flywheels turning.
    Third pass: it first builds the kit's ft_life.Kit pieces (moving(): steam, vehicles, the flare, the lamp heads;
    build=False leaves them as they are), and life3() adds that pass's movement."""
    import ft_life
    D = '/Game/Maps/ChemicalWorks/Dress/'
    A = ft_life.animate
    n = 0
    if build:
        moving()

    def beacon(slot, colour, glow):
        return colour == 'ff3355' and glow > 1.0

    def glows(v):
        return lambda slot, colour, glow: abs(glow - v) < 0.011

    # steam churns and leans down the wind, more the higher it stands (far: it must be seen moving from a long way off)
    steam = dict(SwayPow=1.2, SwayRate=0.45, SwayLean=0.5, BreatheRate=0.8, BreatheVar=1.5)
    for name, base, height, amp, swell in (('SM_CoolingTower', 4600.0, 3200.0, 260.0, 36.0), ('SM_CoolingCells', 900.0, 900.0, 90.0, 16.0),
                                           ('SM_FarCooling', 10000.0, 8000.0, 600.0, 80.0), ('SM_FarStacks', 14000.0, 6000.0, 420.0, 60.0),
                                           ('SM_ColumnTrio', 7600.0, 2600.0, 200.0, 28.0)):
        n += A(D + name, ('Sway', 'Breathe'), ['#d8d0f0'], far=True, SwayBase=base, SwayHeight=height, SwayAmp=amp, BreatheAmp=swell, **steam)
    # beacons blink, each at its own moment
    for name in ('SM_ReactorHall', 'SM_GantryCrane', 'SM_Gasholder', 'SM_FarStacks', 'SM_FarPlant', 'SM_FarPlant2', 'SM_FarGasworks',
                 'SM_ColumnTrio', 'SM_ReactorCore'):
        n += A(D + name, 'Light', beacon, **ft_life.BLINK)
    # lit floors of the far works come and go; the reactor's windows swell and fade
    for name in ('SM_FarPlant', 'SM_FarPlant2'):
        n += A(D + name, 'Light', ['#5ff0ff*', '#ffd23f*'], **ft_life._windows(700.0, rate=0.1, off=0.2, low=0.25))
    n += A(D + 'SM_FarStacks', 'Light', ['#ffe9a8*', '#5ff0ff*'], **ft_life._windows(1040.0, rate=0.08, off=0.2, low=0.25))
    n += A(D + 'SM_ReactorHall', 'Light', ['#b44dff*'], **dict(ft_life.PULSE, LightRate=0.25, LightMin=0.55))
    # the reactor: the slits swell, the gallery's windows change, the light climbs the needle and its rings
    n += A(D + 'SM_ReactorCore', 'Light', glows(1.0), **dict(ft_life.PULSE, LightRate=0.2, LightMin=0.5))
    n += A(D + 'SM_ReactorCore', 'Light', ['#ffe9a8*'], **ft_life._windows(900.0, rate=0.08, off=0.15, low=0.3))
    n += A(D + 'SM_ReactorCore', 'Light', glows(1.4), **ft_life._chase((0.0, 0.0, 1.0), 5200.0, rate=0.3, duty=0.6, low=0.4))
    # lights that run: along the racks and the catwalks, up the columns
    for name in ('SM_PipeRack', 'SM_Catwalk'):
        n += A(D + name, 'Light', ['#5ff0ff*'], **ft_life._chase((1.0, 0.0, 0.0), 1600.0, rate=0.5, duty=0.5, low=0.35, rand=1.0))
    n += A(D + 'SM_ColumnTrio', 'Light', ['#5ff0ff*'], **ft_life._chase((0.0, 0.0, 1.0), 2600.0, rate=0.4, duty=0.5, low=0.3, rand=1.0))
    # liquor: the ponds swell a little, the channel's water runs, the clarifier's rake goes round
    for name in ('SM_Pond', 'SM_Pond2', 'SM_Lagoon', 'SM_TankPad'):
        n += A(D + name, 'Light', glows(POOL), **dict(ft_life.PULSE, LightRate=0.12, LightMin=0.7))
    n += A(D + 'SM_Canal', 'Light', ['#5ff0ff-'], FlowMix=1.0, Color2=(0.75, 1.0, 1.0),
           **ft_life._chase((1.0, 0.0, 0.0), 900.0, rate=0.25, duty=0.5, low=0.6, rand=1.0))
    n += A(D + 'SM_Clarifier', 'Light', glows(POOL), **dict(ft_life.PULSE, LightRate=0.12, LightMin=0.7))
    n += A(D + 'SM_Clarifier', 'Spin', ['#8f9bb3', '#ff9e3d'], SpinRate=0.15, SpinRand=1.0)
    # loading: the yard crane's trolley and its load travel the bridge (a box in the mesh's own space picks them out)
    n += A(D + 'SM_GantryCrane', 'Bob', ['#2a3f8f', '#233a63', '#1c1a26', '#19b5c9', '#ffd23f', '#ffe9a8'], far=True, BobDir=(0.0, 1.0, 0.0), BobAmp=450.0,
           BobRate=0.035, BobRand=1.0, MaskMin=(-130.0, -400.0, 620.0), MaskMax=(250.0, -120.0, 1520.0))
    n += life3(A, D)
    n += life4(A, D)
    unreal.log('FTDRESS chem life: %d slots animated' % n)
    return n


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


# ------------------------------------------------------------------------------------------ the cut banks (3.4)
# Inside a banked bend the track cuts the land down to the road's low edge, and that cutting was a bare slope a third
# of the frame high. A Bank rule has the track lay these on it (ft_dress_bank.py has the drawing rules: flat, x along
# the road, y up the slope, z out of the bank). The works face their cuttings with concrete: slabs in a darker frame,
# the plant's teal main along the bank on its saddles, a hazard placard, and here and there an outlet that has leaked
# the chemical down the wall. Unlit, like everything this near the road.
import ft_dress_bank as B   # noqa: E402


def cut_revet(m, leak=False):
    """A deep cutting's facing (11 x 9 m as drawn): two courses of three slabs under a coping, the teal main between
    the courses; a hazard placard, or with leak=True two outlets in the lower course and what has run out of them."""
    B.body(m, CONC_D, B.TALL)
    for x in (-360.0, 0.0, 360.0):
        B.block(m, PAVE_D, x, -227.0, 330.0, 335.0, 10.0)
        B.block(m, PAVE_D, x, 227.0, 330.0, 335.0, 10.0)
    B.block(m, PAVE, 0.0, 427.0, B.L, 46.0, 18.0)                       # the coping, under the land's edge
    B.block(m, TEAL, 0.0, 0.0, B.L, 56.0, 28.0)                         # the main
    for x in (-360.0, 0.0, 360.0):
        B.block(m, TEAL_D, x, 0.0, 44.0, 96.0, 34.0)                    # its saddles
    if leak:
        for x, w in ((-360.0, 1.0), (360.0, 0.7)):
            B.block(m, STEELB_D, x, -120.0, 100.0, 100.0, 20.0)         # an outlet's collar, its mouth
            B.block(m, NAVY, x, -120.0, 54.0, 54.0, 23.0)
            B.shape(m, VIOLET, [(x - 36.0 * w, -168.0), (x + 36.0 * w, -168.0), (x + 13.0, -392.0), (x - 13.0, -392.0)], 5.0, B.FACE + 10.0)
    else:
        B.block(m, HAZ, 0.0, 227.0, 140.0, 140.0, 16.0)                 # the placard: a yellow plate, a dark square
        B.block(m, NAVY, 0.0, 227.0, 76.0, 76.0, 20.0)


def cut_kerb(m, leak=False):
    """A shallow cutting's facing (11 x 4 m as drawn): one course of slabs under the coping; leak=True: an outlet in
    the middle slab and its stain."""
    B.body(m, CONC_D, B.LOW)
    for x in (-360.0, 0.0, 360.0):
        B.block(m, PAVE_D, x, -30.0, 330.0, 280.0, 10.0)
    B.block(m, PAVE, 0.0, 172.0, B.L, 56.0, 18.0)
    if leak:
        B.block(m, STEELB_D, 0.0, 56.0, 96.0, 80.0, 20.0)
        B.block(m, NAVY, 0.0, 56.0, 52.0, 42.0, 23.0)
        B.shape(m, VIOLET, [(-32.0, 16.0), (32.0, 16.0), (12.0, -166.0), (-12.0, -166.0)], 5.0, B.FACE + 10.0)


# ------------------------------------------------------------------------------------------ third pass (the overhaul, 15 Nov)
# "Pipe racks, loading bays, and tanks, with cycling machinery, steam releases, and moving work lights."
# What this pass adds to the kit, by distance from the road:
#   17 to 24 m   SM_KerbRun, SM_StoreRow: the works' kerb and what is kept behind it, a slot (45 m) long, so a Site
#                rule lays them end to end. Under 2 m, pale and unlit: this is the row a camera on the low side of a
#                banked bend, in a loop or beside the car looks down on.
#   24 to 30 m   SM_TankRow, SM_RackNear, SM_Compressor: tall and UNLIT, so they may stand at the sight line (the lit
#                racks, bays and tank clusters of the first pass start 30 m out). The rack's magenta line runs; the
#                compressor's two flywheels turn.
#   40 to 110 m  the machines and the steam: SM_BlowStack (a blow-down stack that lets go for five seconds in twelve),
#                SM_Kiln (a rotary kiln, turning), SM_WorkRoad (a works road with plant tugs driving it, lamps on),
#                SM_LightMast (a floodlight mast whose head swings), SM_FlareTower and SM_MegaVat (the two set pieces).
#   330 m on     SM_SkyCardA, B, C: three rows of flat skyline in three receding tones (the bluest and darkest nearest,
#                the palest and most violet furthest, as the sky does towards its horizon).
# Pieces that need parts sent along a path (steam puffs, vehicles, a flame) are built with ft_life.Kit, the way the
# level's own moving props are: MOVING lists them and moving() builds them. life() calls moving() first.
ROW = 4560.0          # a row piece: one 45.24 m slot, and a hand's breadth to lap the next


def kerb_run(m, kind=0, L=ROW):
    """The works' kerb, a slot long: a pale concrete upstand with yellow and navy blocks on its road face, squat
    white-capped bollards and a pipe rail on it; then, past the strip the ground pipe runs in, what the yard keeps
    at its edge. kind 0: drums on pallets, crates, a valve pit, a cable reel, loose barriers. kind 1 (the stores):
    gas bottles in a rack, pallet stacks, a loaded trolley, reels lying flat, totes in a low bund. Nothing over 2 m."""
    yk = -250.0
    m.box(PAVE, (0, yk, 28), (L, 70, 56))
    n = 8
    w = L / n
    for k in range(n):
        x = -L / 2.0 + w * (k + 0.5)
        col = HAZ if k % 2 == 0 else NAVY
        m.box(col, (x, yk - 36, 32), (w * 0.5, 6, 40))
        m.box(col, (x, yk, 58), (w * 0.5, 70, 6))
    for k in range(n + 1):                                             # bollards on the kerb, a pipe rail through them
        x = -L / 2.0 + 70 + (L - 140) * k / n
        pz(m, PAVE_D, x, yk, 56, 112, 20, steps=6)
        pz(m, WHITE, x, yk, 112, 130, 25, steps=6)
    px(m, TEAL, -L / 2.0 + 70, L / 2.0 - 70, yk, 96, 9, steps=6)
    r = random.Random(kind + 70)
    if kind == 0:
        for (ax, ay) in ((-1650, 150), (950, 190)):
            m.box(RUST, (ax, ay, 8), (190, 170, 16))
            for i in range(2):
                for j in range(2):
                    drum(m, ax - 46 + i * 92, ay - 42 + j * 84, 16, r.choice([HAZ, TEAL, MAG, STEELB]))
        m.box(STEELB_D, (-520, 160, 60), (170, 130, 120))
        m.box(TEAL_D, (-520, 160, 140), (120, 100, 40))      # lowered (review): with it the piece's box was over 2.5 m and every kerb cast a shadow
        m.box(HAZ, (-520, 93, 60), (80, 4, 50))
        m.box(CONC, (1750, 150, 12), (340, 220, 24))                   # the valve pit
        px(m, TEAL, 1600, 1900, 150, 70, 24, steps=8)
        for x in (1680, 1820):
            pz(m, STEELB, x, 150, 24, 150, 12, steps=6)
            m.cyl(RED, (x, 150, 150), 30, 8, steps=8)
        py(m, RUST, -2050, 96, 204, 66, 66, steps=10)                  # a cable reel on its rim
        py(m, STEELB_D, -2050, 110, 190, 66, 38, steps=8)
        for k in range(3):                                             # loose barriers, one knocked askew
            yaw = 9 if k == 1 else 0
            m.box(CONC, (150 + k * 250, 110 + (14 if k == 1 else 0), 40), (230, 60, 80), rot=(0, yaw, 0))
            m.box(PAVE, (150 + k * 250, 110 + (14 if k == 1 else 0), 86), (230, 30, 12), rot=(0, yaw, 0))
        drum(m, -1100, 230, 0, MAG, lying=True, yaw=20)
    else:
        m.box(NAVY, (-1500, 150, 8), (300, 100, 16))                   # the bottle rack
        for k in range(6):
            pz(m, (ORANGE, WHITE, TEAL)[k % 3], -1610 + k * 44, 150, 16, 166, 17, steps=6)
        m.box(HAZ, (-1500, 150, 120), (300, 110, 10))
        for (ax, ay, top) in ((-650, 150, WHITE), (-430, 190, TEAL)):
            m.box(RUST, (ax, ay, 42), (160, 140, 84))
            m.box(top, (ax, ay, 119), (130, 110, 70))
        m.box(NAVY, (500, 150, 62), (440, 170, 24))                    # a trolley loaded with pipe
        for s in (-1, 1):
            m.box(BLACK, (500 + s * 150, 150, 25), (90, 150, 50))
        for j in range(3):
            px(m, (TEAL, WHITE, TEAL)[j], 290, 710, 96 + j * 54, 104, 26, steps=6)
        pz(m, RUST, 1450, 170, 0, 44, 72, steps=10)                    # reels lying flat
        pz(m, STEELB_D, 1450, 170, 44, 96, 40, steps=8)
        pz(m, RUST, 1450, 170, 96, 140, 72, steps=10)
        m.box(PAVE_D, (2000, 160, 14), (420, 240, 28))                 # totes in a low bund
        for k in range(3):
            m.box(PAVE, (1880 + k * 120, 160, 84), (104, 104, 112))      # not white (review): small bright cubes beside the road read as pickups
            m.box(STEELB_D, (1880 + k * 120, 160, 22), (112, 112, 16))
        m.box(HAZ, (2000, 37, 22), (420, 6, 28))
        drum(m, -2000, 200, 0, TEAL)
        drum(m, -1930, 120, 0, HAZ)


def tank_row(m, L=4500.0):
    """A row of day tanks on one plinth, a slot long: four standing tanks of different girth, a bullet lying on its
    saddles, the header along the front and a riser to each. To 11 m, 7.6 m deep, and unlit: it may stand at the
    sight line (24 m), where the lit tank clusters may not."""
    m.box(PAVE_D, (0, 0, 25), (L, 760, 50))
    m.box(HAZ, (0, -382, 30), (L, 5, 26))
    for (x, r, h, col, band) in ((-1800, 300, 820, TEAL, HAZ), (-1080, 230, 540, WHITE, MAG), (-380, 310, 960, STEELB, TEAL), (350, 220, 480, WHITE, TEAL)):
        pz(m, NAVY, x, 30, 50, 82, r + 26, steps=10)
        pz(m, col, x, 30, 82, 82 + h, r, steps=10)
        pz(m, band, x, 30, 82 + h * 0.58, 82 + h * 0.58 + 52, r + 6, steps=10)
        m.sphere(col, (x, 30, 82 + h), r, scale=(1, 1, 0.3), steps=(4, 10))
        pz(m, MAG, x, -300, 110, 82 + h * 0.3, 20, steps=6)
        py(m, MAG, x, -300, 30 - r + 10, 82 + h * 0.3, 20, steps=6)
    tank_x(m, TEAL, 1020, 1960, 30, 330, 200, band=HAZ, n=3, steps=10)
    for x in (1180, 1800):
        m.box(CONC, (x, 30, 100), (130, 400, 110))
    px(m, MAG, -L / 2.0 + 120, L / 2.0 - 120, -300, 110, 22, steps=6)
    m.box(STEELB_D, (-730, 30, 640), (440, 120, 30))                   # a walkway between two tank tops


def rack_near(m, L=ROW, H=520.0, W=340.0):
    """A low pipe rack for the roadside, a slot long: portal frames every 9 m carrying two decks of lines, the big
    line's expansion loop over the top. No lamps on it (so it may stand at the sight line); life() runs the colour
    down its magenta line."""
    n = 5
    for k in range(n + 1):
        x = -L / 2.0 + L * k / n
        for s in (-1, 1):
            m.box(STEELB, (x, s * W / 2.0, H / 2.0), (30, 30, H))
            m.box(PAVE_D, (x, s * W / 2.0, 18), (64, 64, 36))
        for z in (H * 0.55, H):
            m.box(STEELB, (x, 0, z), (26, W + 30, 26))
    for y, r, col, z in ((-110, 46, TEAL, H * 0.55 + 59), (0, 30, MAG, H * 0.55 + 43), (105, 26, WHITE, H * 0.55 + 39),
                         (-70, 40, HAZ, H + 53), (75, 28, TEAL_D, H + 41)):
        px(m, col, -L / 2.0, L / 2.0, y, z, r, steps=8)
    for x in (-L * 0.3, L * 0.1, L * 0.38):                            # flanges on the big line
        px(m, NAVY, x - 8, x + 8, -110, H * 0.55 + 59, 56, steps=8)
    for x in (-L * 0.1, L * 0.02):                                     # the loop: up from the lower deck, over, down
        pz(m, TEAL, x, -110, H * 0.55 + 59, H + 190, 46, steps=8)
    px(m, TEAL, -L * 0.1, L * 0.02, -110, H + 190, 46, steps=8)


def compressor(m):
    """A compressor shed, open at the sides: a flat roof on six posts, and under it two reciprocating compressors, each
    with a flywheel as tall as a man and a half facing the road (life() turns them, each at its own speed). Bottles
    and a header behind. Unlit, 5.7 m: for the sight line and just beyond."""
    m.box(PAVE_D, (0, 0, 20), (1500, 720, 40))
    for x in (-700, 0, 700):
        for y in (-320, 320):
            m.box(STEELB, (x, y, 290), (30, 30, 500))
    m.box(NAVY, (0, 0, 555), (1560, 780, 30))
    m.box(TEAL, (0, -392, 555), (1560, 6, 30))
    for (x, rim, spoke) in ((-350.0, HAZ, WHITE), (350.0, ORANGE, STEAM)):
        m.box(STEELB_D, (x + 170, 40, 130), (320, 260, 180))           # the crank case
        m.box(TEAL_D, (x + 170, 40, 300), (190, 190, 160))             # the cylinder
        pz(m, TEAL_D, x + 170, 40, 380, 470, 52, steps=8)
        py(m, rim, x - 60, -150, -112, 200, 152, steps=12)             # the flywheel, facing the road
        m.box(spoke, (x - 60, -158, 200), (272, 16, 46))
        m.box(spoke, (x - 60, -158, 200), (46, 16, 272))
        py(m, spoke, x - 60, -170, -112, 200, 36, steps=8)
        pz(m, PAVE, x + 300, 220, 40, 430, 60, steps=8)                # a discharge bottle
        m.sphere(PAVE, (x + 300, 220, 430), 60, steps=(4, 8))
    px(m, MAG, -640, 640, 300, 250, 26, steps=6)
    m.box(STEELB_D, (640, -250, 110), (120, 90, 140))                  # the control box
    m.box(TEAL, (640, -297, 130), (80, 4, 50))


def kiln(m, L=4000.0, R=185.0, Z=930.0):
    """A rotary kiln: a steel drum 40 m long turning on three tall piers (its axis is 9 m up, so that it shows over the
    roadside rows), lifter bars along it and a riding ring over each pier (that is what shows it turning: life() spins
    every rust, white, orange and yellow part about the drum's axis), the drive house beside the middle ring, the feed
    tower with its vent and steam at one end, the firing hood and the burner pipe at the other, a glow at the hood's
    port. For the middle distance: 59 m long, 32 m with its steam."""
    m.box(PAVE_D, (0, 0, 20), (L + 1900, 720, 40))
    px(m, RUST, -L / 2.0, L / 2.0, 0, Z, R, steps=10)
    for k in range(4):
        a = math.radians(45 + 90 * k)
        m.box(WHITE if k % 2 == 0 else ORANGE, (0, math.cos(a) * (R + 6), Z + math.sin(a) * (R + 6)), (L * 0.86, 46, 46))
    for x in (-L * 0.32, 0.0, L * 0.32):
        px(m, HAZ, x - 42, x + 42, 0, Z, R + 28, steps=10)
        m.box(PAVE, (x, 0, 365), (260, 580, 650))                      # the pier
        m.box(STEELB_D, (x, 0, 700), (320, 640, 24))
        for s in (-1, 1):
            px(m, STEEL, x - 60, x + 60, s * 160, Z - 188, 46, steps=8)    # its rollers
    m.box(TEAL, (300, -300, 380), (320, 220, 680))                     # the drive house
    m.box(STEELB, (90, -300, 650), (160, 160, 160))
    xf = -L / 2.0 - 280.0                                              # the feed end
    m.box(STEELB_D, (xf, 0, 760), (560, 640, 1440))
    m.box(NAVY, (xf, 0, 1492), (600, 680, 24))
    m.box(TEAL_D, (xf, 0, 1650), (300, 300, 300))
    pz(m, STEELB, xf, 150, 1800, 2200, 60, steps=8)
    for z in (520, 980):
        m.box(WARM, (xf, -323, z), (240, 6, 110), glow=0.55)
    tilt_box(m, TEAL_D, (xf - 600, 0, 1800), (xf - 100, 0, 1650), 120, 90)   # the feed conveyor's gallery
    m.box(STEELB, (xf - 560, 0, 900), (70, 70, 1800))
    for j, (dx, dz, rr) in enumerate(((0, 200, 160), (130, 520, 220), (350, 880, 250))):
        m.sphere(STEAM, (xf + dx, 150 + 40 * j, 2200 + dz), rr, scale=(1.15, 1, 0.8), steps=(4, 6))
    xb = L / 2.0 + 230.0                                               # the firing end
    m.box(STEELB_D, (xb, 0, 660), (460, 580, 1240))
    m.box(NAVY, (xb, 0, 1292), (500, 620, 24))
    m.box(ORANGE, (xb, -293, Z), (150, 6, 150), glow=0.9)
    px(m, TEAL, xb + 230, xb + 560, 0, Z, 50, steps=8)
    pz(m, TEAL, xb + 560, 0, 0, Z, 50, steps=8)
    m.sphere(TEAL, (xb + 560, 0, Z), 50, steps=(4, 8))


def mega_vat(m, R=3800.0, H=5200.0):
    """THE MEGA VAT (the level's second scene of that name had no vat): a concrete drum 76 m across and 52 m tall,
    ribbed and hooped, the chemical standing in it to the brim and running down the wall in three places into catch
    basins (life() swells the glow), the stirrer bridge turning across the top with its drive in the middle (the only
    steel-grey and orange on the piece), steam rolling off the surface, two feed mains climbing the wall and over the
    rim, a stair tower with lit landings on the road side, a beacon. FTDressL1.inl stands one dead ahead of the jump
    at 7.5 km, where the road swerves round it."""
    pz(m, PAVE_D, 0, 0, 0, 300, R + 500, steps=16)
    pz(m, PAVE, 0, 0, 300, H, R, steps=16)
    for z in (H * 0.34, H * 0.68):
        pz(m, STEELB_D, 0, 0, z, z + 220, R + 14, steps=16)
    pz(m, HAZ, 0, 0, H - 520, H - 400, R + 16, steps=16)
    pz(m, NAVY, 0, 0, H - 60, H + 60, R + 160, steps=16)               # the rim walk
    n = 10
    for k in range(n):
        a = 2 * math.pi * (k + 0.5) / n
        m.box(PAVE_D, (math.cos(a) * (R + 40), math.sin(a) * (R + 40), 300 + (H - 700) / 2.0), (150, 420, H - 700), rot=(0, math.degrees(a), 0))
    pz(m, VIOLET, 0, 0, H + 60, H + 76, R - 140, steps=16, glow=1.0)   # the chemical, to the brim
    for a_deg in (-72, -108, -36):                                     # where it runs down the wall, road side
        a = math.radians(a_deg)
        cx, cy = math.cos(a) * (R + 22), math.sin(a) * (R + 22)
        m.box(VIOLET, (cx, cy, 300 + (H - 300) / 2.0), (60, 300, H - 300), rot=(0, a_deg, 0), glow=1.02)      # their own slot: they run (life4)
        m.box(PAVE, (math.cos(a) * (R + 260), math.sin(a) * (R + 260), 380), (520, 620, 160), rot=(0, a_deg, 0))
        m.box(VIOLET, (math.cos(a) * (R + 260), math.sin(a) * (R + 260), 468), (400, 500, 16), rot=(0, a_deg, 0), glow=1.0)
    # the stirrer: a bridge across the top, the drive on it, two paddle shafts going down
    m.box(STEEL, (0, 0, H + 330), (2 * R + 300, 320, 180))
    m.box(ORANGE, (0, 0, H + 560), (700, 700, 300))
    for s in (-1, 1):
        m.box(STEEL, (s * R * 0.55, 0, H + 130), (160, 160, 240))
        m.box(ORANGE, (s * (R + 60), 0, H + 330), (200, 420, 260))
    for j, (dx, dy, dz, rr) in enumerate(((-1500, 600, 700, 900), (300, -900, 1100, 1150), (1700, 500, 800, 950), (-300, 1300, 1700, 1250),
                                          (900, 100, 2300, 1100), (2000, -600, 2900, 850))):
        m.sphere(STEAM, (dx, dy, H + dz), rr, scale=(1.15, 1, 0.75), steps=(4, 7))
    for (a_deg, col, r) in ((160, TEAL, 150), (20, MAG, 110)):         # the feed mains
        a = math.radians(a_deg)
        x, y = math.cos(a) * (R + r + 60), math.sin(a) * (R + r + 60)
        pz(m, col, x, y, 0, H + 420, r, steps=8)
        rod(m, col, (x, y, H + 420), (math.cos(a) * (R - 500), math.sin(a) * (R - 500), H + 420), r, steps=8)
        m.sphere(col, (x, y, H + 420), r, steps=(4, 8))
    xs, ys = 900.0, -(R + 330.0)                                       # the stair tower, road side
    m.box(STEELB_D, (xs, ys, (H + 300) / 2.0), (520, 520, H + 300))
    m.box(NAVY, (xs, ys, H + 312), (580, 580, 24))
    for k in range(6):
        m.box(WARM, (xs, ys - 262, 700 + k * 800), (300, 6, 180), glow=0.55)
    ap = math.radians(-144.0)                                          # the plate with the vat's number on it
    m.box(CYAN, (math.cos(ap) * (R + 30), math.sin(ap) * (R + 30), H * 0.5), (8, 1500, 110), rot=(0, -144.0, 0), glow=0.6)
    pz(m, STEELB, xs, ys, H + 324, H + 900, 45, steps=6)
    m.sphere(RED, (xs, ys, H + 930), 60, steps=(4, 6), glow=1.3)


def _skyline(r, L, lo, hi, kinds, step=(900, 2600)):
    """The shapes of a flat skyline, left to right: [(outline [(x, z)] closed along the base, x from, x to, height)].
    Boxes, stacks on a block, cooling towers, gas spheres, columns with a bridge. kinds: what this row is made of."""
    out = []
    x = -L / 2.0
    base = lo * 0.35
    while x < L / 2.0 - 800:
        kind = r.choice(kinds)
        w = min(r.uniform(*step), L / 2.0 - x - 400)
        h = r.uniform(lo, hi)
        if kind == 'block':
            pts, top = [(x, h), (x + w, h)], h
        elif kind == 'stack':                         # a stack on a block
            hb = r.uniform(lo * 0.5, lo)
            s0 = x + w * r.uniform(0.2, 0.45)
            sw = max(w * 0.24, 480)
            pts, top = [(x, hb), (s0, hb), (s0 + sw * 0.15, h * 1.25), (s0 + sw * 0.85, h * 1.25), (s0 + sw, hb), (x + w, hb)], hb
        elif kind == 'tower':                         # a waisted cooling tower
            pts, top = [(x + w * 0.22, h * 0.62), (x + w * 0.2, h), (x + w * 0.8, h), (x + w * 0.78, h * 0.62)], h * 0.6
        elif kind == 'sphere':                        # a gas sphere on its legs, as an octagon
            hs = min(h, w * 0.9)
            pts, top = [(x + w * 0.08, hs * 0.45), (x + w * 0.3, hs * 0.9), (x + w * 0.7, hs * 0.9), (x + w * 0.92, hs * 0.45)], 0.0
        else:                                         # 'column': two columns of different height with a bridge
            c = w * 0.3
            pts, top = [(x, h), (x + c, h), (x + c, h * 0.55), (x + w - c, h * 0.55), (x + w - c, h * 0.82), (x + w, h * 0.82)], h * 0.55
        out.append(([(x, base)] + pts + [(x + w, base)], x, x + w, top))
        x += w + min(r.uniform(120, 500), L / 2.0 - x - w - 20)
    return out, base


def sky_card(m, seed=0, L=42000.0, lo=3000.0, hi=7500.0, col='#2b3a94', kinds=('block', 'stack', 'column', 'sphere'), lights=10, deep=7000.0,
             step=(900, 2600)):
    """One length of far skyline, FLAT: a silhouette two metres thick (the horizon's rows are cards: three of them, in
    three tones, one behind another): a base strip carried down below the land's edge and one outline to each shape
    on it, a few lit floors and a beacon or two so that the works out there are working. No footing block: SM_SkyCard's
    JOBS entry adds a wide one that keeps the card clear of every stretch of road (see buried, keep)."""
    r = random.Random(seed * 13 + 5)
    shapes, base = _skyline(r, L, lo, hi, kinds, step)
    m.box(col, (0, 0, (base - deep) / 2.0), (L, 200, base + deep))
    for pts, x0, x1, top in shapes:
        m.side(col, pts, -100, 100)
    wide = [q for q in shapes if q[3] > 1600 and q[2] - q[1] > L * 0.03]      # the shapes a lit floor fits on
    for k in range(lights):
        if not wide:
            break
        pts, x0, x1, top = wide[(k * 7 + seed) % len(wide)]
        x = r.uniform(x0 + (x1 - x0) * 0.38, x1 - (x1 - x0) * 0.38)
        z = r.uniform(700, top - 500)
        if k % 5 == 4:
            m.box(RED, (x, -106, top - 150), (L * 0.006, 12, L * 0.006), glow=1.3)
        else:
            m.box(CYAN if k % 3 else WARM, (x, -106, z), (min(L * 0.022, (x1 - x0) * 0.3), 12, L * 0.0045), glow=0.6)


# ---- the pieces built with ft_life.Kit (parts that travel: steam, vehicles, a flame, a swinging lamp head)
def blow_stack(k):
    """A blow-down stack: the knock-out drum on its saddles and a 16 m stack with a silencer on its tip, which lets go
    for five seconds in every twelve: chunky balls of steam leave the tip small, climb 23 m, lean down the wind,
    swell and thin out. Every stack on the level keeps its own time."""
    s = k.group()
    s.box(PAVE_D, (0, 0, 20), (760, 460, 40))
    tank_x(s, WHITE, -300, 120, 80, 170, 95, band=TEAL, n=2, steps=8)
    for x in (-230, 50):
        s.box(CONC, (x, 80, 55), (60, 220, 30))
    X, Y, H = 250.0, -70.0, 1500.0
    pz(s, STEELB, X, Y, 40, H, 50, steps=8)
    for z in (520, 1010):
        pz(s, HAZ, X, Y, z, z + 46, 57, steps=8)
    pz(s, NAVY, X, Y, H, H + 150, 78, steps=8)
    pz(s, STEELB_D, X, Y, H + 150, H + 170, 62, steps=8)
    rod(s, WHITE, (120, 80, 170), (X, Y, 300), 22, steps=6)
    s.box(MIDLAND, (0, 0, -200), (760, 460, 400))
    top = H + 170.0
    rise, drift, r, size1 = 2300.0, 800.0, 85.0, 3.0
    p = k.group('Plume', rough=0.95, PlumeBase=(X, Y, top), PlumeRise=rise, PlumeDrift=drift, PlumeSize0=0.4, PlumeSize1=size1,
                PlumeRate=0.2, PlumeCycle=0.085, PlumeDuty=0.42, PlumeSpread=r * 1.1, PlumeEase=0.75)
    for i in range(9):
        p.piece(lambda m: m.sphere(STEAM, (0, 0, 0), r, steps=(4, 7)), a=i / 9.0, b=0.8 + 0.4 * ((i * 7) % 5) / 4.0, c=i * 0.37)
    reach = drift * 1.15 + r * size1 + 60.0
    return dict(grow=(reach, reach, rise + r * size1 + 100.0), grow_down=(reach, reach, 0.0))


def work_road(k, L=13500.0):
    """A works road three slots long with the night shift on it: two plant tugs each way, each pulling a loaded
    trailer, head lamps lit and an amber beacon turning on the cab. The tugs are parts sent along the road and round
    again (ft_life's Conveyor); every stretch of road keeps its own time."""
    s = k.group()
    s.box(PAVE_D, (0, 0, 15), (L, 760, 30))
    for y in (-365, 365):
        s.box(PAVE, (0, y, 22), (L, 30, 44))
    n = 15
    for i in range(n):
        s.box(WHITE, (-L / 2.0 + L * (i + 0.5) / n, 0, 32), (L / n * 0.4, 26, 4))
    s.box(MIDLAND, (0, 0, -40), (L, 760 + 900, 80))                  # wider than the road: the lamps keep 30 m from every centre line

    def tug(m, d):
        m.box(HAZ, (d * 60, 0, 95), (260, 150, 110))
        m.box(STEELB, (d * -10, 0, 190), (120, 140, 80))
        m.box(BLACK, (0, 0, 30), (300, 160, 40))
        m.box(TEAL, (d * -330, 0, 105), (300, 150, 130))
        m.box(BLACK, (d * -330, 0, 26), (320, 140, 32))
        m.box(WARM, (d * 194, 0, 100), (10, 120, 30), glow=1.2)

    for d, y, rate, starts in ((1.0, -180.0, 0.07, (0.05, 0.52)), (-1.0, 180.0, 0.055, (0.24, 0.71))):
        conv = dict(ConvStart=(-d * L / 2.0, y, 30.0), ConvDir=(d, 0.0, 0.0), ConvLen=L, ConvRate=rate, ConvRand=1.0)
        g = k.group('Conveyor', **conv)
        b = k.group(('Conveyor', 'Light'), LightRate=1.1, LightDuty=0.45, LightSoft=0.06, LightMin=0.12, LightRand=1.0, **conv)
        for a in starts:
            g.piece(lambda m, d=d: tug(m, d), a=a)
            b.piece(lambda m, d=d: m.box(ORANGE, (d * -10, 0, 244), (40, 40, 24), glow=1.4), a=a)
    return dict(grow=(0.0, 0.0, 270.0), grow_down=(0.0, 0.0, 0.0))


def light_mast(k, H=1800.0):
    """A floodlight mast, 20 m: a lamp head with a bank of lamps on each of two faces and a short cone of light below
    each (painted the way a cel painter paints light: three lengths stepping down to the night), swinging slowly from
    side to side about the mast. Each mast keeps its own time."""
    s = k.group()
    s.box(PAVE_D, (0, 0, 30), (260, 260, 60))
    pz(s, STEELB, 0, 0, 60, H, 36, steps=8, top_r=24)
    s.box(STEELB_D, (0, 0, H * 0.5), (96, 96, 34))
    pz(s, STEELB_D, 0, 0, H, H + 26, 130, steps=8)
    s.box(MIDLAND, (0, 0, -150), (260, 260, 300))
    h = k.group('Spin', far=True, rough=0.8, SpinPivot=(0.0, 0.0, H), SpinAxis=(0.0, 0.0, 1.0), SpinSwing=48.0, SpinRate=0.5, SpinRand=1.0)
    h.box(NAVY, (0, 0, H + 120), (170, 320, 160))
    for sx in (-1, 1):
        for i in range(3):
            h.box(WARM, (sx * 89, -100 + i * 100, H + 120), (8, 76, 100), glow=1.2)
        x, z, w = sx * 100.0, H + 120.0, 110.0
        for col, glow, ln in (('#fff1c0', 0.30, 300.0), ('#b9a8e0', 0.14, 330.0), ('#6a5ab0', 0.08, 360.0)):
            dx, dz = sx * ln * math.cos(math.radians(38)), -ln * math.sin(math.radians(38))
            tilt_box(h, col, (x, 0, z), (x + dx, 0, z + dz), w * 2.4, w, glow=glow)
            x, z, w = x + dx, z + dz, w * 1.3
    return dict(grow=(900.0, 900.0, 0.0), grow_down=(900.0, 900.0, 0.0))


def flare_tower(k, H=8200.0):
    """A flare derrick, 82 m to the tip and a flame 26 m long on it that whips and flickers: four raking legs, five
    decks with a band of light under each (the light climbs), the flare line up the middle, beacons at the top deck's
    corners. FTDressL1.inl stands eight pairs of these as an avenue over the finale's long run (12.8 to 13.8 km)."""
    s = k.group()
    s.box(PAVE_D, (0, 0, 30), (2200, 2200, 60))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(s, STEELB, (sx * 900, sy * 900, 0), (sx * 170, sy * 170, H), 200)
    for i in range(1, 6):
        z = H * i / 5.0
        w = 900 - 730 * i / 5.0
        s.box(STEELB_D, (0, 0, z), (2 * w + 300, 2 * w + 300, 140))
        for sx in (-1, 1):
            s.box(HAZ, (sx * (w + 150), 0, z + 130), (30, 2 * w + 300, 120))
    pz(s, STEEL, 0, 0, 0, H + 500, 150, steps=8)
    pz(s, NAVY, 0, 0, H + 500, H + 620, 230, steps=8)
    s.box(MIDLAND, (0, 0, -500), (2200, 2200, 1000))
    lt = k.group('Light', rough=0.6, LightAxis=(0.0, 0.0, 1.0), LightLen=3280.0, LightRate=0.4, LightDuty=0.5, LightSoft=0.1, LightMin=0.3, LightRand=1.0)
    for i in range(1, 6):
        z = H * i / 5.0
        w = 900 - 730 * i / 5.0
        lt.box(CYAN, (0, -(w + 154), z - 110), (2 * w + 300, 8, 80), glow=0.5)
        lt.box(CYAN, (0, (w + 154), z - 110), (2 * w + 300, 8, 80), glow=0.5)
    bc = k.group('Light', rough=0.6, LightRate=0.7, LightDuty=0.45, LightSoft=0.06, LightMin=0.12, LightRand=1.0)
    for sx in (-1, 1):
        for sy in (-1, 1):
            bc.sphere(RED, (sx * 270, sy * 270, H + 150), 60, steps=(4, 6), glow=1.3)
    top = H + 620.0
    # The flame's LIGHT only swells, slowly and by a fifth (three steps a second at most). These flames are 26 to 34 m
    # tall and thirty of them stand on the level: a deep flicker nine times a second (the small flare stack's own
    # setting) over that much of the frame would be hard on the eyes. What moves is the flutter.
    fl = k.group(('Flutter', 'Light'), far=True, rough=0.9, FlutterPin=(0.0, 0.0, top), FlutterAxis=(0.0, 0.0, 1.0), FlutterDir=(0.8, 0.6, 0.0),
                 FlutterSpan=1800.0, FlutterLen=2000.0, FlutterRate=6.5, FlutterAmp=330.0, LightHash=0.45, LightCell=100000.0, LightRate=3.0,
                 LightMin=0.8)
    for col, r0, h, glow in (('#ff8a2a', 430.0, 2600.0, 2.2), ('#ffd23f', 240.0, 1700.0, 3.0)):
        n = 5
        for i in range(n):
            fl.cyl(col, (0, 0, top + h * i / n), r0 * (1.0 - i / float(n)) + 14, h / n, steps=6, top_r=r0 * (1.0 - (i + 1) / float(n)) + 14, glow=glow)
    em = k.group('Plume', rough=0.9, PlumeBase=(0.0, 0.0, top + 1800.0), PlumeRise=1200.0, PlumeDrift=520.0, PlumeSize0=1.0, PlumeSize1=0.3,
                 PlumeRate=0.22, PlumeSpread=150.0, PlumeEase=0.7)
    for i in range(8):                                                   # fourth pass: embers leave the flame, drift down the wind, go out
        em.piece(lambda m: m.sphere('#ffd23f', (0, 0, 0), 26, steps=(3, 5), glow=2.6), a=i / 8.0, b=0.8 + 0.4 * ((i * 3) % 4) / 3.0, c=i * 0.37)
    return dict(grow=(700.0, 700.0, 600.0), grow_down=(700.0, 700.0, 0.0))


MOVING = [('SM_BlowStack', blow_stack), ('SM_WorkRoad', work_road), ('SM_LightMast', light_mast), ('SM_FlareTower', flare_tower)]


def moving(only=None):
    """Build the kit's ft_life.Kit pieces (MOVING) into the level's Dress folder. They carry their own M_Life
    instances, so an export of the plain pieces does not touch them; life() calls this first."""
    import ft_life
    done = []
    for name, fn in MOVING:
        if only and name not in only:
            continue
        try:
            k = ft_life.Kit()
            ext = fn(k) or {}
            sm = k.save('/Game/Maps/ChemicalWorks/Dress/' + name, **ext)
            b = sm.get_bounds()
            e = b.box_extent
            unreal.log('FTDRESS chem %s (moving) triangles %d size m %.1f %.1f %.1f base z %.0f' % (
                name, sm.get_num_triangles(0), e.x / 50.0, e.y / 50.0, e.z / 50.0, b.origin.z - e.z))
            done.append(name)
        except Exception as e:  # noqa
            import traceback
            unreal.log_error('FTDRESS chem %s (moving) FAILED: %r %s' % (name, e, traceback.format_exc()[-900:]))
            print('JAM ERR kit piece chem', name, repr(e)[:300], flush=True)
    print('JAM kit chem moving pieces built', len(done), 'of', len(MOVING), flush=True)
    return done


def life3(A, D):
    """The third pass's movement (see life)."""
    import ft_life
    n = 0
    # the roadside rack's magenta line runs (colour only: nothing near the road is lit)
    n += A(D + 'SM_RackNear', 'Light', ['#e84aff-'], FlowMix=1.0, Color2=(1.0, 0.55, 1.0),
           **ft_life._chase((1.0, 0.0, 0.0), 760.0, rate=0.3, duty=0.5, low=0.6, rand=1.0))
    # cycling machinery: the compressor's flywheels, the kiln's drum, the vat's stirrer, the gas holders' bells
    n += A(D + 'SM_Compressor', 'Spin', ['#ffd23f', '#eef6ff'], SpinPivot=(-410.0, 0.0, 200.0), SpinAxis=(0.0, 1.0, 0.0), SpinRate=2.6, SpinRand=1.0)
    n += A(D + 'SM_Compressor', 'Spin', ['#ff9e3d', '#d8d0f0'], SpinPivot=(290.0, 0.0, 200.0), SpinAxis=(0.0, 1.0, 0.0), SpinRate=2.1, SpinRand=1.0)
    n += A(D + 'SM_Kiln', 'Spin', ['#a8552f', '#eef6ff', '#ff9e3d-', '#ffd23f'], far=True, SpinPivot=(0.0, 0.0, 930.0), SpinAxis=(1.0, 0.0, 0.0),
           SpinRate=0.45, SpinRand=1.0)
    n += A(D + 'SM_Kiln', 'Light', ['#ff9e3d*'], **dict(ft_life.PULSE, LightRate=0.6, LightMin=0.45))
    n += A(D + 'SM_Kiln', ('Sway', 'Breathe'), ['#d8d0f0'], far=True, SwayBase=2200.0, SwayHeight=1000.0, SwayAmp=110.0, SwayPow=1.2, SwayRate=0.5,
           SwayLean=0.5, BreatheAmp=18.0, BreatheRate=0.8, BreatheVar=1.5)
    n += A(D + 'SM_MegaVat', 'Spin', ['#8f9bb3', '#ff9e3d'], far=True, SpinPivot=(0.0, 0.0, 5500.0), SpinAxis=(0.0, 0.0, 1.0), SpinRate=0.14, SpinRand=0.0)
    n += A(D + 'SM_MegaVat', 'Light', ['#b44dff*'], **dict(ft_life.PULSE, LightRate=0.18, LightMin=0.55))
    n += A(D + 'SM_MegaVat', 'Light', ['#ffe9a8*'], **ft_life._windows(800.0, rate=0.08, off=0.2, low=0.3))
    n += A(D + 'SM_MegaVat', 'Light', ['#5ff0ff*'], **ft_life._chase((1.0, 0.0, 0.0), 1500.0, rate=0.35, duty=0.5, low=0.35, rand=0.0))
    n += A(D + 'SM_MegaVat', 'Light', ['#ff3355*'], **ft_life.BLINK)
    n += A(D + 'SM_MegaVat', ('Sway', 'Breathe'), ['#d8d0f0'], far=True, SwayBase=5200.0, SwayHeight=3200.0, SwayAmp=330.0, SwayPow=1.2, SwayRate=0.4,
           SwayLean=0.5, BreatheAmp=60.0, BreatheRate=0.6, BreatheVar=1.5)
    n += A(D + 'SM_Gasholder', 'Bob', ['#12849a'], far=True, BobDir=(0.0, 0.0, 1.0), BobAmp=130.0, BobRate=0.035, BobRand=1.0)
    # the horizon's cards: their lit floors come and go, their beacons blink
    for name in ('SM_SkyCardA', 'SM_SkyCardB', 'SM_SkyCardC'):
        n += A(D + name, 'Light', ['#5ff0ff*', '#ffe9a8*'], **ft_life._windows(1500.0, rate=0.07, off=0.25, low=0.3))
        n += A(D + name, 'Light', ['#ff3355*'], **ft_life.BLINK)
    return n





# ---------------------------------------------------------------------------------------------------------------------
# The fourth pass (15 Nov, "the built world in motion"): traffic that is not the chase, a helicopter, machines at work,
# one flare that lets go on a clock, tall columns for the sky half of the banked bends. Every piece is built with
# ft_life.Kit; nothing is a figure or an animal (another machine makes those). All of it stands 70 m and more from the
# road or is placed by a Far rule, so by the track's shadow rule (within 70 m AND over 2.5 m) none of it casts a shadow.
# The works are dense near the road (rows 26 to 37 m out, 5 to 9 m tall): from the driving seat a thing 80 m out shows
# only above about 20 m. So what travels here travels HIGH: the tank train on a viaduct, the highway on piers, the
# drums in a gallery, the helicopter.
def tank_train(k, L=36195.0, Z=2000.0):
    """362 m of works railway on a steel viaduct 20 m up (eight slots exactly) and a tank train on it: a yellow
    shunter with a turning beacon and ten tank wagons, 9 m a second, round again every 40 s. A loading shed stands over
    the line at each end (the wagons are parts sent along the line; they shrink to nothing at its two ends, inside the
    sheds). The table stands it 80 m out, where it rides over the near rows as seen from the driving seat. It works on
    either side: on the right the train comes towards the driver, on the left (the table turns a left-hand piece
    round) it goes his way. Every length keeps its own time."""
    s = k.group()
    s.box(STEELB_D, (0, 0, Z - 60), (L, 420, 120))
    for y in (-76, 76):
        s.box(STEEL, (0, y, Z + 8), (L, 14, 16))
    for sy in (-1, 1):
        s.box(HAZ, (0, sy * 214, Z + 70), (L, 8, 14))
    n = 16                                                               # a bent every 22.6 m
    for i in range(n + 1):
        x = -L / 2.0 + L * i / n
        for sy in (-1, 1):
            tilt_box(s, STEELB, (x, sy * 430, 0), (x, sy * 150, Z - 120), 70)
        s.box(STEELB_D, (x, 0, Z * 0.5), (60, 600, 60))
        s.box(MIDLAND, (x, 0, -150), (260, 1040, 300))
    for sx in (-1, 1):                                                   # the loading sheds
        x = sx * (L / 2.0 - 1500.0)
        for sy in (-1, 1):
            s.box(NAVY, (x, sy * 300, Z + 330), (3000, 30, 660))
            s.box(HAZ, (x, sy * 318, Z + 90), (3000, 6, 60))
        s.box(STEELB_D, (x, 0, Z + 680), (3060, 680, 40))
        for i in range(3):
            pz(s, WHITE, x - 900 + i * 900, 0, Z + 700, Z + 1150, 130, steps=8)
        for sy in (-1, 1):
            s.box(WARM, (x, sy * 322, Z + 420), (2400, 6, 90), glow=0.5)  # the shed's lit band (steady, 80 m out)
    conv = dict(ConvStart=(L / 2.0, 0.0, Z + 16.0), ConvDir=(-1.0, 0.0, 0.0), ConvLen=L, ConvRate=9.0 * 100.0 / L, ConvRand=1.0)
    t = k.group('Conveyor', **conv)
    b = k.group(('Conveyor', 'Light'), LightRate=1.0, LightDuty=0.45, LightSoft=0.06, LightMin=0.12, LightRand=0.0, **conv)

    def shunter(m):
        m.box(HAZ, (0, 0, 230), (1300, 290, 300))
        m.box(NAVY, (380, 0, 450), (420, 280, 160))                      # the cab, behind the hood: the train runs towards -X
        m.box(GLASS, (380, 0, 460), (424, 240, 80))
        m.box(BLACK, (0, 0, 45), (1200, 250, 90))
        m.box(WARM, (-654, 0, 300), (8, 120, 50), glow=1.2)

    def wagon(m, col, band):
        px(m, col, -620, 620, 0, 255, 150, steps=8)
        px(m, band, -50, 50, 0, 255, 157, steps=8)
        m.box(STEELB_D, (0, 0, 420), (220, 120, 60))
        m.box(BLACK, (0, 0, 60), (1300, 240, 110))

    cols = ((WHITE, HAZ), (TEAL, WHITE), (MAG, WHITE), (WHITE, TEAL), (ORANGE, NAVY), (TEAL, HAZ), (WHITE, MAG), (VIOLET, WHITE),
            (WHITE, HAZ), (TEAL, WHITE))
    head = 0.6
    t.piece(shunter, a=head)
    b.piece(lambda m: m.box(ORANGE, (380, 0, 548), (56, 56, 32), glow=1.4), a=head)
    for i, (c, bd) in enumerate(cols):
        t.piece(lambda m, c=c, bd=bd: wagon(m, c, bd), a=(head - (i + 1) * 1500.0 / L) % 1.0)
    return dict(grow=(0.0, 0.0, 0.0), grow_down=(0.0, 0.0, 0.0))


def far_highway(k, L=72000.0, Z=8000.0):
    """720 m of the ring road that runs round the works on tall piers, for the horizon (a Far rule stands a piece's
    z = 0 25 m under the road: the deck is 55 m over it): the traffic is streaks of light, warm white one way and red
    the other, 25 m a second, and a line of cyan lamps over the deck. It is never nearer than 250 m."""
    s = k.group()
    s.box('#2b3a94', (0, 0, Z), (L, 760, 170))
    for i in range(8):
        x = -L / 2.0 + L * (i + 0.5) / 8
        s.box('#243080', (x, 0, (Z - 85) / 2.0), (300, 460, Z - 85))
        s.box('#243080', (x, 0, Z - 160), (460, 760, 150))
    for sy in (-1, 1):
        s.box('#32409c', (0, sy * 380, Z + 140), (L, 22, 110))
    lamps = k.group(rough=0.6)
    for i in range(12):
        x = -L / 2.0 + L * (i + 0.5) / 12
        lamps.box('#32409c', (x, 0, Z + 420), (36, 36, 560))
        lamps.box(CYAN, (x, 0, Z + 700), (60, 420, 26), glow=0.7)
    for sy, col, d, glow in ((-1, '#fff1c0', 1.0, 2.4), (1, '#ff2d3c', -1.0, 2.0)):
        g = k.group('Conveyor', far=True, ConvStart=(-d * L / 2.0, sy * 190.0, Z + 100.0), ConvDir=(d, 0.0, 0.0), ConvLen=L, ConvRate=0.035,
                    ConvRand=1.0)
        r = random.Random(3 + int(sy))
        for i in range(14):
            g.piece(lambda m, c=col, gl=glow: m.box(c, (0, 0, 40), (760, 130, 76), glow=gl), a=(i + r.uniform(-0.35, 0.35)) / 14.0)
    return dict(grow=(0.0, 0.0, 0.0), grow_down=(0.0, 0.0, 0.0))


def heli(k, R=15000.0, H=11500.0):
    """The works' helicopter on its round: one circle 300 m across in 50 s, 90 m over the road's level (a Far rule
    stands z = 0 25 m under it), nose along the circle, its searchlight (a solid beam painted in lengths that step
    down from warm white to the night's violet, as a cel painter paints light) raking the ground inside the circle,
    a red beacon blinking on the fin. Bigger than life (22 m): it is never nearer than 130 m. One mesh: the whole
    craft is one group turned about the circle's middle."""
    spin = dict(SpinPivot=(0.0, 0.0, H), SpinAxis=(0.0, 0.0, 1.0), SpinRate=0.125, SpinRand=1.0)
    g = k.group('Spin', far=True, rough=0.5, **spin)
    g.box(WHITE, (R, 0, H), (360, 1100, 380))
    g.box(HAZ, (R, -60, H - 60), (366, 700, 90))
    g.box(GLASS, (R, 640, H - 10), (320, 300, 280))
    g.box(NAVY, (R, -1050, H + 90), (130, 1100, 130))
    g.box(HAZ, (R, -1560, H + 260), (44, 240, 440))
    g.box(NAVY, (R, -1560, H + 110), (420, 150, 30))
    pz(g, STEELB_D, R, 0, H + 190, H + 310, 60, steps=6)
    g.cyl('#3a4a8a', (R, 0, H + 310), 1050, 12, steps=12)                 # the rotor: a dark disc with two pale blades on it
    g.box(STEAM, (R, 0, H + 326), (2080, 120, 10))
    g.box(STEAM, (R, 0, H + 326), (120, 2080, 10))
    for sx in (-1, 1):
        g.box(BLACK, (R + sx * 200, 0, H - 270), (50, 900, 40))
        for sy in (-1, 1):
            g.box(BLACK, (R + sx * 190, sy * 300, H - 220), (30, 30, 90))
    x, y, z, w = R - 120.0, 300.0, H - 200.0, 90.0                        # the beam: down and in, 105 m
    dx, dy, dz = -0.56, 0.0, -0.83
    for col, glow, ln in (('#fff1c0', 0.30, 1900.0), ('#f3c4dc', 0.16, 2400.0), ('#c391d8', 0.10, 2900.0), ('#8662b8', 0.07, 3300.0)):
        tilt_box(g, col, (x, y, z), (x + dx * ln, y + dy * ln, z + dz * ln), w * 2.0, w * 2.0, glow=glow)
        x, y, z, w = x + dx * ln, y + dy * ln, z + dz * ln, w * 1.45
    bc = k.group(('Spin', 'Light'), far=True, rough=0.6, LightRate=0.9, LightDuty=0.4, LightSoft=0.06, LightMin=0.12, LightRand=1.0, **spin)
    bc.sphere(RED, (R, -1560, H + 510), 56, steps=(4, 6), glow=1.4)
    bc.sphere('#5ff0ff', (R, 660, H - 190), 40, steps=(4, 6), glow=1.2)
    reach = R + 1300.0
    return dict(grow=(0.0, reach, 0.0), grow_down=(2.0 * R + 1500.0, reach, 0.0))


def drum_gallery(k, L=13572.0, Z=2400.0):
    """A conveyor gallery, 136 m (three slots): two transfer towers and an open truss between them 24 m up, on which
    drums of hot product travel from one tower to the other, 5 m a second, each glowing through its bung. The table
    stands it 75 m out. Every gallery keeps its own time."""
    s = k.group()
    for sx in (-1, 1):
        x = sx * (L / 2.0 - 450.0)
        s.box(NAVY, (x, 0, (Z + 900) / 2.0), (900, 800, Z + 900))
        s.box(STEELB_D, (x, 0, Z + 920), (980, 880, 40))
        for i in range(3):
            s.box(WARM, (x, -404, 700 + i * 800), (520, 8, 160), glow=0.5)
        s.box(HAZ, (x, -404, Z + 300), (900, 8, 80))
        s.box(MIDLAND, (x, 0, -200), (1100, 1000, 400))
    span = L - 1800.0
    for z in (Z - 40, Z + 420):
        for sy in (-1, 1):
            s.box(STEELB, (0, sy * 150, z), (span, 30, 40))
    nb = 12
    for i in range(nb):
        x0 = -span / 2.0 + span * i / nb
        x1 = x0 + span / nb
        a, c = ((x0, Z - 40), (x1, Z + 420)) if i % 2 == 0 else ((x0, Z + 420), (x1, Z - 40))
        for sy in (-1, 1):
            tilt_box(s, STEELB, (a[0], sy * 150, a[1]), (c[0], sy * 150, c[1]), 26)
    s.box(BLACK, (0, 0, Z - 10), (span, 240, 20))                         # the belt
    for i in (1, 2):                                                      # two trestle bents under the span
        x = -span / 2.0 + span * i / 3.0
        for sy in (-1, 1):
            tilt_box(s, STEELB_D, (x, sy * 520, 0), (x, sy * 150, Z - 60), 80)
        s.box(MIDLAND, (x, 0, -200), (300, 1300, 400))
    g = k.group('Conveyor', ConvStart=(-span / 2.0, 0.0, Z + 2.0), ConvDir=(1.0, 0.0, 0.0), ConvLen=span, ConvRate=0.043, ConvRand=1.0)

    def drum_part(m):
        pz(m, TEAL, 0, 0, 0, 190, 86, steps=8)
        pz(m, HAZ, 0, 0, 80, 110, 90, steps=8)
        pz(m, ORANGE, 0, 0, 190, 204, 50, steps=6, glow=1.1)

    for i in range(10):
        g.piece(drum_part, a=i / 10.0)
    return dict(grow=(0.0, 0.0, 0.0), grow_down=(0.0, 0.0, 0.0))


def big_flare(k):
    """THE GROUND FLARE: a refractory wall 26 m across with the burner in it, a pilot flame 9 m tall that never goes
    out, and every twelve seconds, for four, the flare lets go: balls of fire leave the burner small, climb 60 m,
    swell to 9 m across and thin out, leaning down the wind. One on the level, 110 m and more from the road, on the
    outside of a bend where the road looks straight at it. Light: the pilot's glow only swells (three steps a second
    at most, never under four fifths); a burst is a slow swell of fire, twelve seconds apart, not a flicker."""
    s = k.group()
    R, n = 1300.0, 14
    for i in range(n):
        a = 2 * math.pi * i / n
        s.box(RUST, (math.cos(a) * R, math.sin(a) * R, 320), (70, 600, 640), rot=(0, math.degrees(a), 0))
        s.box(HAZ, (math.cos(a) * (R + 38), math.sin(a) * (R + 38), 600), (8, 600, 60), rot=(0, math.degrees(a), 0))
    pz(s, CONC, 0, 0, 0, 60, R + 300, steps=14)
    pz(s, STEELB_D, 0, 0, 60, 520, 210, steps=8)
    pz(s, NAVY, 0, 0, 520, 600, 280, steps=8)
    for a_deg in (40, 160, 280):                                          # the three feed lines over the wall
        a = math.radians(a_deg)
        rod(s, STEEL, (math.cos(a) * (R + 900), math.sin(a) * (R + 900), 760), (math.cos(a) * 240, math.sin(a) * 240, 420), 60, steps=6)
        pz(s, STEEL, math.cos(a) * (R + 900), math.sin(a) * (R + 900), 0, 760, 60, steps=6)
    s.box(MIDLAND, (0, 0, -300), (2 * R + 2200, 2 * R + 2200, 600))
    top = 600.0
    fl = k.group(('Flutter', 'Light'), far=True, rough=0.9, FlutterPin=(0.0, 0.0, top), FlutterAxis=(0.0, 0.0, 1.0), FlutterDir=(0.8, 0.6, 0.0),
                 FlutterSpan=700.0, FlutterLen=800.0, FlutterRate=6.5, FlutterAmp=120.0, LightHash=0.45, LightCell=100000.0, LightRate=3.0,
                 LightMin=0.8)
    for col, r0, h, glow in (('#ff8a2a', 230.0, 900.0, 2.2), ('#ffd23f', 130.0, 600.0, 3.0)):
        m5 = 4
        for i in range(m5):
            fl.cyl(col, (0, 0, top + h * i / m5), r0 * (1.0 - i / float(m5)) + 10, h / m5, steps=6, top_r=r0 * (1.0 - (i + 1) / float(m5)) + 10, glow=glow)
    rise, drift, rr, size1 = 6000.0, 900.0, 260.0, 3.4
    p = k.group('Plume', rough=0.9, PlumeBase=(0.0, 0.0, top + 100.0), PlumeRise=rise, PlumeDrift=drift, PlumeSize0=0.5, PlumeSize1=size1,
                PlumeRate=0.3, PlumeCycle=1.0 / 12.0, PlumeDuty=0.34, PlumeSpread=200.0, PlumeEase=0.8)
    fire = (('#ff8a2a', 2.0), ('#ffd23f', 2.6), ('#ff5a1f', 1.7))
    for i in range(12):
        c, gl = fire[i % 3]
        p.piece(lambda m, c=c, gl=gl: m.sphere(c, (0, 0, 0), rr, steps=(4, 7), glow=gl), a=i / 12.0, b=0.8 + 0.4 * ((i * 7) % 5) / 4.0, c=i * 0.37)
    reach = drift * 1.15 + rr * size1 + 100.0
    return dict(grow=(reach, reach, rise + rr * size1 + 200.0), grow_down=(reach, reach, 0.0))


def column_row(k, L=4400.0, seed=0):
    """Four process columns in a row, 54 to 70 m tall, for the OUTSIDE of the banked bends, 72 m and more from the
    road: there the road's own raised edge hides everything under about 26 m, and the sky over it was empty for two
    thirds of this level. Platforms every 11 m with a band of light under each (the light climbs, column by column
    out of step), a beacon on each head, and one column of the four vents: its feather of steam leans down the wind
    and swells."""
    r = random.Random(41 + seed)
    s = k.group()
    lt = k.group('Light', far=True, rough=0.6, LightAxis=(0.0, 0.0, 1.0), LightLen=2200.0, LightRate=0.35, LightDuty=0.5, LightSoft=0.1, LightMin=0.3,
                 LightRand=1.0)
    bc = k.group('Light', far=True, rough=0.6, LightRate=0.7, LightDuty=0.45, LightSoft=0.06, LightMin=0.12, LightRand=1.0)
    s.box(PAVE_D, (0, 0, 30), (L, 900, 60))
    s.box(MIDLAND, (0, 0, -400), (L, 900, 800))
    heads = []
    for i in range(4):
        x = -L / 2.0 + L * (i + 0.5) / 4.0
        y = r.choice((-120.0, 0.0, 140.0))
        H = r.choice((5400.0, 6000.0, 6500.0, 7000.0))
        rad = r.choice((170.0, 200.0, 240.0))
        col = (WHITE, TEAL, STEELB, WHITE)[(i + seed) % 4]
        pz(s, PAVE_D, x, y, 60, 360, rad + 70, steps=8)
        pz(s, col, x, y, 360, H, rad, steps=8)
        pz(s, NAVY, x, y, H, H + 160, rad * 0.72, steps=8)
        z = 1400.0
        while z < H - 500:
            pz(s, STEELB_D, x, y, z, z + 40, rad + 130, steps=6)
            pz(lt, CYAN, x, y, z - 70, z - 16, rad + 12, steps=6, glow=0.6)
            z += 1100.0
        pz(s, HAZ, x, y, H - 420, H - 330, rad + 8, steps=8)
        bc.sphere(RED, (x, y, H + 215), 55, steps=(4, 6), glow=1.3)
        heads.append((x, y, H + 160, rad))
    x, y, top, rad = heads[1 + seed % 2]
    st = k.group(('Sway', 'Breathe'), far=True, rough=0.95, SwayBase=top, SwayHeight=1500.0, SwayAmp=260.0, SwayPow=1.2, SwayRate=0.45, SwayLean=0.5,
                 BreatheAmp=30.0, BreatheRate=0.8, BreatheVar=1.5)
    for j, (dx, dz, rr) in enumerate(((0, 150, 190), (120, 480, 260), (300, 860, 330), (560, 1250, 380))):
        st.sphere(STEAM, (x + dx, y, top + dz), rr, scale=(1.15, 1, 0.8), steps=(4, 7))
    return dict(grow=(700.0, 700.0, 300.0), grow_down=(700.0, 700.0, 0.0))


def tanker_road(k, L=13572.0, Z=1800.0):
    """An elevated works road, 136 m (three slots) on portal frames 18 m up, with the night's convoy on it: four road
    tankers nose to tail one way at 11 m a second, head lamps lit, and a tug with its trailer coming the other way. A
    gatehouse spans the deck at each end (the vehicles are parts sent along the road; they shrink to nothing inside
    it). The table stands it 76 m out. Every length keeps its own time."""
    s = k.group()
    s.box(PAVE_D, (0, 0, Z - 40), (L, 800, 80))
    for sy in (-1, 1):
        s.box(HAZ, (0, sy * 392, Z + 50), (L, 16, 100))
    n = 6
    for i in range(n + 1):
        x = -L / 2.0 + L * i / n
        for sy in (-1, 1):
            s.box(STEELB, (x, sy * 360, (Z - 80) / 2.0), (90, 90, Z - 80))
        s.box(STEELB_D, (x, 0, Z - 130), (110, 820, 100))
        s.box(MIDLAND, (x, 0, -150), (300, 1000, 300))
    for i in range(4):
        x = -L / 2.0 + L * (i + 0.5) / 4
        s.box(STEELB, (x, 380, Z + 300), (30, 30, 500))
        s.box(WARM, (x, 300, Z + 540), (60, 180, 20), glow=0.8)
    for sx in (-1, 1):                                                   # the gatehouses
        x = sx * (L / 2.0 - 520.0)
        s.box(NAVY, (x, 0, Z + 330), (1040, 880, 660))
        s.box(HAZ, (x, -444, Z + 120), (1040, 8, 70))
        s.box(WARM, (x, -444, Z + 430), (700, 8, 110), glow=0.5)

    def road_tanker(m, d, col):
        m.box(HAZ, (d * 520, 0, 170), (260, 250, 260))
        m.box(GLASS, (d * 560, 0, 230), (190, 254, 90))
        x0, x1 = (-700.0, 360.0) if d > 0 else (-360.0, 700.0)
        px(m, col, x0, x1, 0, 200, 125, steps=8)
        px(m, HAZ, (x0 + x1) / 2.0 - 40, (x0 + x1) / 2.0 + 40, 0, 200, 131, steps=8)
        m.box(BLACK, (0, 0, 40), (1380, 230, 60))
        m.box(WARM, (d * 654, 0, 120), (8, 200, 40), glow=1.2)
        m.box(RED, (-d * 705, 0, 120), (8, 200, 30), glow=0.9)

    def tug2(m, d):
        m.box(HAZ, (d * 60, 0, 95), (260, 150, 110))
        m.box(STEELB, (d * -10, 0, 190), (120, 140, 80))
        m.box(BLACK, (0, 0, 30), (300, 160, 40))
        m.box(TEAL, (d * -330, 0, 105), (300, 150, 130))
        m.box(WARM, (d * 194, 0, 100), (10, 120, 30), glow=1.2)

    g = k.group('Conveyor', ConvStart=(-L / 2.0, -190.0, Z), ConvDir=(1.0, 0.0, 0.0), ConvLen=L, ConvRate=11.0 * 100.0 / L, ConvRand=1.0)
    for i, col in enumerate((WHITE, TEAL, WHITE, MAG)):
        g.piece(lambda m, col=col: road_tanker(m, 1.0, col), a=(0.55 - i * 1560.0 / L) % 1.0)
    g2 = k.group('Conveyor', ConvStart=(L / 2.0, 190.0, Z), ConvDir=(-1.0, 0.0, 0.0), ConvLen=L, ConvRate=0.06, ConvRand=1.0)
    for a in (0.3, 0.8):
        g2.piece(lambda m: tug2(m, -1.0), a=a)
    return dict(grow=(0.0, 0.0, 0.0), grow_down=(0.0, 0.0, 0.0))


def ship_loader(k):
    """THE EXPORT BERTH: a product tanker 150 m long lying alongside a jetty in a dock cut into the works, and the
    loading arm working her: a tower on the jetty whose boom slews slowly out over the ship's manifold and back, the
    hose block under it rising and falling, the ship's radar turning, her deck lights and the bridge's windows lit, a
    beacon at the masthead. The table stands two on the export terminal, 120 m and more from the road."""
    s = k.group()
    s.box('#12849a', (0, 1800, 12), (19000, 4400, 24))                   # the dock's water
    s.box(PAVE_D, (0, -500, 60), (19000, 1000, 120))                     # the quay
    s.box(PAVE, (0, -20, 130), (19000, 40, 60))
    hy = 1700.0                                                          # the hull's middle line
    s.box(RUST, (-400, hy, 160), (13000, 2300, 280))
    s.box(NAVY, (-400, hy, 620), (13000, 2360, 640))
    s.side(NAVY, [(-6900, 940), (-6900, 300), (-8300, 940)], hy - 1180, hy + 1180)
    s.side(RUST, [(-6900, 300), (-6900, 20), (-7500, 300)], hy - 1150, hy + 1150)
    s.box(STEELB_D, (-400, hy, 960), (13000, 2300, 40))
    for i in range(6):                                                   # deck pipes and tank hatches
        x = -5600 + i * 1500
        pz(s, WHITE, x, hy, 980, 1080, 260, steps=8)
    px(s, TEAL, -6200, 2600, hy - 300, 1100, 60, steps=6)
    px(s, MAG, -6200, 2600, hy + 300, 1100, 60, steps=6)
    bx = 4600.0                                                          # the accommodation, aft
    s.box(WHITE, (bx, hy, 1700), (2200, 2100, 1500))
    s.box(WHITE, (bx + 200, hy, 2600), (1500, 2500, 320))
    for i in range(3):
        s.box(WARM, (bx - 1104, hy, 1250 + i * 420), (8, 1700, 150), glow=0.5)
        s.box(WARM, (bx, hy - 1054, 1250 + i * 420), (1800, 8, 150), glow=0.5)
    s.box(GLASS, (bx + 200, hy - 1254, 2600), (1300, 8, 180))
    pz(s, ORANGE, bx + 700, hy, 2760, 3500, 320, steps=8)
    pz(s, NAVY, bx + 700, hy, 3500, 3640, 330, steps=8)
    pz(s, STEELB, bx - 300, hy, 2760, 3900, 40, steps=6)
    s.box(MIDLAND, (0, 1300, -300), (19000, 5400, 600))
    tx = -1500.0                                                         # the loading arm's tower, on the quay
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(s, STEELB, (tx + sx * 320, -500 + sy * 320, 120), (tx + sx * 120, -500 + sy * 120, 2700), 60)
    for z in (900, 1700, 2500):
        s.box(STEELB_D, (tx, -500, z), (640, 640, 40))
    s.box(NAVY, (tx, -500, 2790), (420, 420, 180))
    arm = k.group('Spin', far=True, rough=0.6, SpinPivot=(tx, -500.0, 2800.0), SpinAxis=(0.0, 0.0, 1.0), SpinSwing=26.0, SpinRate=0.25, SpinRand=1.0)
    arm.box(HAZ, (tx, 700, 2960), (220, 3000, 160))                      # the boom, out over the ship
    arm.box(STEELB_D, (tx, -1200, 2960), (300, 700, 300))                # its counterweight
    tilt_box(arm, STEELB, (tx, -500, 3500), (tx, 2100, 3040), 40)
    arm.box(STEELB, (tx, -500, 3200), (60, 60, 640))
    px(arm, WHITE, tx - 30, tx + 30, 2150, 2300, 70, steps=6)
    arm.box(WHITE, (tx, 2150, 1900), (70, 70, 900))                      # the hose, hanging to the manifold
    rd = k.group('Spin', far=True, rough=0.5, SpinPivot=(bx - 300.0, hy, 3900.0), SpinAxis=(0.0, 0.0, 1.0), SpinRate=1.6, SpinRand=1.0)
    rd.box(WHITE, (bx - 300, hy, 3930), (420, 40, 60))
    bc = k.group('Light', far=True, rough=0.6, LightRate=0.7, LightDuty=0.45, LightSoft=0.06, LightMin=0.12, LightRand=1.0)
    bc.sphere(RED, (bx - 300, hy, 4020), 50, steps=(4, 6), glow=1.3)
    bc.sphere(RED, (tx, -500, 3560), 50, steps=(4, 6), glow=1.3)
    return dict(grow=(600.0, 600.0, 0.0), grow_down=(600.0, 600.0, 0.0))


def furnace_hall(k):
    """A furnace hall with its charging door on the road side: every eighteen seconds the door rises slowly, the fire
    behind it shows for a moment, and the door drops shut (the door is one group sent up and down; each hall keeps its
    own time). A stack beside it breathes steam. The light is steady behind a slow door: nothing blinks. The table
    stands them 72 m and more from the road."""
    s = k.group()
    s.box(NAVY, (0, 400, 800), (3000, 1800, 1600))
    s.box(STEELB_D, (0, 400, 1700), (2400, 700, 200))
    s.box(HAZ, (0, -504, 1480), (3000, 8, 70))
    s.box(PAVE_D, (0, -1100, 20), (3200, 1200, 40))
    s.box(ORANGE, (0, -504, 560), (1100, 8, 920), glow=1.6)              # the fire, behind the door
    s.box('#ffd23f', (0, -508, 360), (700, 6, 420), glow=2.2)
    for sx in (-1, 1):
        s.box(STEELB_D, (sx * 610, -516, 560), (100, 30, 1000))
    s.box(STEELB_D, (0, -516, 1080), (1320, 30, 100))
    pz(s, STEELB, 1150, 700, 1600, 3000, 110, steps=8)
    for z in (2100, 2600):
        pz(s, HAZ, 1150, 700, z, z + 46, 117, steps=8)
    s.box(MIDLAND, (0, 0, -300), (3200, 3400, 600))
    door = k.group('Bob', far=True, rough=0.6, BobDir=(0.0, 0.0, 1.0), BobAmp=900.0, BobRate=1.0 / 18.0, BobShape=1.0, BobRand=1.0)
    door.box(STEELB, (0, -530, 560), (1130, 16, 940))
    door.box(HAZ, (0, -540, 160), (1130, 6, 120))
    st = k.group(('Sway', 'Breathe'), far=True, rough=0.95, SwayBase=3000.0, SwayHeight=900.0, SwayAmp=120.0, SwayPow=1.2, SwayRate=0.5, SwayLean=0.5,
                 BreatheAmp=18.0, BreatheRate=0.8, BreatheVar=1.5)
    for dx, dz, rr in ((0, 120, 130), (90, 380, 180), (220, 680, 230)):
        st.sphere(STEAM, (1150 + dx, 700, 3000 + dz), rr, scale=(1.15, 1, 0.8), steps=(4, 7))
    return dict(grow=(300.0, 300.0, 300.0), grow_down=(300.0, 300.0, 0.0))


def rain_lamp(k, H=3000.0):
    """A floodlight tower, 30 m, lighting the yard at its foot, and the night's rain falling through its light: the
    cone is painted in three lengths stepping down to the night (as on the floodlight masts), and pale streaks fall
    through it, 13 m a second, slanted a little down the wind (parts sent down a line and round again, each at its own
    place in the cone). The table stands them 72 m and more from the road, the light towards it."""
    s = k.group()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(s, STEELB, (sx * 300, sy * 300, 0), (sx * 90, sy * 90, H), 44)
    for z in (H * 0.33, H * 0.66):
        s.box(STEELB_D, (0, 0, z), (460, 460, 30))
    s.box(NAVY, (0, -60, H + 120), (760, 260, 240))
    s.box(MIDLAND, (0, 0, -200), (700, 700, 400))
    for i in range(4):
        s.box(WARM, (-270 + i * 180, -194, H + 110), (130, 8, 170), glow=1.2)
    x, y, z, w = 0.0, -200.0, H + 60.0, 240.0
    for col, glow, ln in (('#fff1c0', 0.30, 800.0), ('#b9a8e0', 0.14, 900.0), ('#6a5ab0', 0.08, 1000.0)):
        dy, dz = -ln * 0.42, -ln * 0.91
        tilt_box(s, col, (x, y, z), (x, y + dy, z + dz), w * 2.2, w, glow=glow)
        y, z, w = y + dy, z + dz, w * 1.3
    rn = k.group('Conveyor', rough=0.4, ConvStart=(0.0, -420.0, H - 100.0), ConvDir=(0.0, -0.4, -0.917), ConvLen=2500.0, ConvRate=0.52, ConvRand=1.0)
    r = random.Random(77)
    for i in range(14):
        ox, oy = r.uniform(-420, 420), r.uniform(-260, 260)
        rn.piece(lambda m, ox=ox, oy=oy: m.box('#dfe8ff', (ox, oy, 0), (6, 6, 170), glow=0.5), a=r.random())
    return dict(grow=(500.0, 1500.0, 0.0), grow_down=(500.0, 1500.0, 0.0))


MOVING4 = [('SM_TankTrain', tank_train), ('SM_FarHighway', far_highway), ('SM_Heli', heli), ('SM_DrumGallery', drum_gallery),
           ('SM_BigFlare', big_flare), ('SM_ColumnRow', column_row), ('SM_ColumnRow2', lambda k: column_row(k, seed=1)),
           ('SM_TankerRoad', tanker_road), ('SM_ShipLoader', ship_loader), ('SM_FurnaceHall', furnace_hall), ('SM_RainLamp', rain_lamp)]
MOVING += MOVING4


def life4(A, D):
    """The fourth pass's movement on pieces the kit already had (see life)."""
    import ft_life
    n = 0
    # the mega vat's three falls RUN: bands of paler violet travel down them (their own slot: glow 1.02, see mega_vat)
    n += A(D + 'SM_MegaVat', 'Light', lambda slot, colour, glow: abs(glow - 1.02) < 0.006, far=True, FlowMix=1.0, Color2=(0.86, 0.6, 1.0),
           **ft_life._chase((0.0, 0.0, -1.0), 1100.0, rate=0.8, duty=0.5, low=0.55, rand=0.0))
    return n


def after_export(only=None):
    """Everything this kit needs after ft_dress.run has exported its JOBS (ft_dress.run calls it, if a kit has one):
    the pieces built with ft_life.Kit (MOVING, by moving()) and every movement (life(): an export puts the still
    materials back). `only`, the names just exported, is not used to do less: all of it is safe to run again and
    takes a few seconds."""
    return life(build=True)
JOBS = [
    ('SM_CutRevet', cut_revet), ('SM_CutRevet2', lambda m: cut_revet(m, True)), ('SM_CutKerb', cut_kerb), ('SM_CutKerb2', lambda m: cut_kerb(m, True)),
    ('SM_FenceRun', buried(fence_run)), ('SM_JerseyRow', buried(jersey_row)), ('SM_DrumPile', buried(drum_pile)),
    ('SM_DrumPile2', buried(lambda m: drum_pile(m, 7))), ('SM_PipeRun', buried(pipe_run)), ('SM_HazSign', buried(haz_sign)),
    ('SM_ValveStation', buried(valve_station)), ('SM_TankerTrailer', buried(tanker_trailer)), ('SM_PumpHouse', buried(pump_house)),
    ('SM_PipeRack', buried(pipe_rack)), ('SM_TankCluster', buried(tank_cluster)), ('SM_Platform', buried(platform)),
    ('SM_LoadingBay', buried(loading_bay)), ('SM_Warehouse', buried(warehouse)), ('SM_ReactorHall', buried(reactor_hall)),
    ('SM_GantryCrane', buried(gantry_crane)), ('SM_RailSpur', buried(rail_spur)), ('SM_Gasholder', buried(gasholder)),
    ('SM_Substation', buried(substation)), ('SM_CoolingCells', buried(cooling_cells)), ('SM_CoolingTower', buried(cooling_tower)),
    # second pass. The ground plane: flat pieces on a shallow footing
    ('SM_YardLot', flat(yard_lot)), ('SM_YardLot2', flat(lambda m: yard_lot(m, 1, 2600, 2200, 52))),
    ('SM_YardLot3', flat(lambda m: yard_lot(m, 2, 3400, 1800, 58))), ('SM_CarPark', flat(car_park, keep=LIT_KEEP)),
    ('SM_Pond', flat(pond, keep=LIT_KEEP)), ('SM_Pond2', flat(lambda m: pond(m, VIOLET, TEAL, 2200, 1900, 58), keep=LIT_KEEP)),
    ('SM_Clarifier', flat(clarifier, keep=LIT_KEEP)), ('SM_TankPad', flat(tank_pad, keep=LIT_KEEP)), ('SM_PipeField', flat(pipe_field)),
    ('SM_AccessRoad', flat(access_road)), ('SM_HeliPad', flat(heli_pad, keep=LIT_KEEP)), ('SM_Lagoon', flat(lagoon, keep=LIT_KEEP)),
    ('SM_RailFan', flat(rail_fan)), ('SM_Canal', flat(canal)),
    # second pass. What stands in the sky of the banked turns, and the reactor (its footing reaches 15 m past it, so
    # the track's clearance test keeps the whole landmark that far off every stretch of road)
    ('SM_Catwalk', buried(catwalk)), ('SM_ColumnTrio', buried(column_trio)),
    ('SM_ReactorCore', buried(reactor_core, depth=0.12, keep=1500.0)),
    # the horizon stands on the floor past the land's edge: its footing takes the floor's colour
    ('SM_FarCooling', buried(far_cooling, LAND, keep=FAR_KEEP)), ('SM_FarStacks', buried(far_stacks, LAND, keep=FAR_KEEP)),
    ('SM_FarPlant', buried(far_plant, LAND, keep=FAR_KEEP)), ('SM_FarPlant2', buried(lambda m: far_plant(m, 12), LAND, keep=FAR_KEEP)),
    ('SM_FarGasworks', buried(far_gasworks, LAND, keep=FAR_KEEP)),
    # third pass. The roadside rows, the machines, the set piece, and the horizon's three rows of cards. A card's
    # footing is a thin slab under its foot, far wider than the card: the track's clearance test keeps the whole
    # box off every stretch of road, so a card 60 to 170 m tall never stands close beside another part of the route
    # (this road folds back on itself everywhere): row A 200 m off, row B 450 m, row C 700 m
    ('SM_KerbRun', flat(kerb_run, 80.0)), ('SM_StoreRow', flat(lambda m: kerb_run(m, 1), 80.0)),
    ('SM_TankRow', buried(tank_row)), ('SM_RackNear', buried(rack_near)), ('SM_Compressor', buried(compressor)),
    ('SM_Kiln', buried(kiln, keep=600.0)), ('SM_MegaVat', buried(mega_vat, depth=0.15, keep=300.0)),
    ('SM_SkyCardA', buried(lambda m: sky_card(m, 0, 42000.0, 3000.0, 6400.0, '#2b3a94', ('block', 'stack', 'column', 'sphere', 'block'), 12, 7000.0,
                                              (900, 2600)), LAND, depth=0.02, keep=20000.0)),
    ('SM_SkyCardB', buried(lambda m: sky_card(m, 1, 60000.0, 5500.0, 11000.0, '#4e45a6', ('tower', 'block', 'stack', 'tower', 'column'), 10, 9000.0,
                                              (1600, 4400)), LAND, depth=0.02, keep=45000.0)),
    ('SM_SkyCardC', buried(lambda m: sky_card(m, 2, 90000.0, 8000.0, 17000.0, '#7560b8', ('tower', 'block', 'tower', 'sphere'), 8, 11000.0,
                                              (3200, 8200)), LAND, depth=0.02, keep=70000.0)),
]
