"""City Center, Stage 1, builder s1d1: district 1 "Fifth and Main" (0 to 1.10 km), the old shopping street.
/Game/World/CityCenter/SM_S1D1_*. Run: import ft_world_city_s1d1 as K; K.run()

Old brick walk-ups and one grand department store under striped awnings: red, cream and brick, amber and teal for
the trim, wooden water tanks on the roofs. Everything is an ft_people.Scene (one material, one section), drawn in cm,
front to -Y, long axis along X. Placed by C:\\ftwork\\s1\\L7_s1d1.inl.

  BUILDINGS (11 m fronts, 64 to 85 m tall: this avenue is a canyon of towers 46 to 82 m tall with a podium and a
  tower in every slot, so a building reads only as a crown over the first row, or, at 0.7 of its size, in the 13 m of
  air between two towers behind an alley. The detail is in the upper half; 3 m of footing under z = 0):
    CornerStore   the department store's corner block: a striped drum turret, a lit show window, parasols on the roof
    WalkUp        a brick walk-up: a fat wooden water tank on legs, a washing line across the roof, a fire escape
    HatHouse      the hatter's: a round striped house wearing a brim, a red crown and a cream cupola, a plume in the band
    Bakery        a giant cream pretzel on a roof frame, two stacks with a puff of steam, loaves for roof vents
    ToyEmporium   a tin drum for a tower (its sticks beat slowly), a striped flagpole, toy blocks on the corner
    ArcadeHall    a barrel vault of warm glass on cream ribs, a loggia of five lit arches, four little turrets
    GableHouse    a stepped gable with a round attic light, a bay window five storeys tall, a weather vane
  PROPS, tall (24 m and more out):
    BarberClock   a street clock on a barber's pole (the stripes turn), two warm faces
    HatSign       a striped mast with a giant top hat hung from its arm
    CupSign       a teal column with a swan neck holding a giant coffee cup
    Bunting       two poles 11 m apart: a string of pennants and a washing line between them, 11 to 14 m up
    WaterTank     a wooden water tank on tall legs with a ladder
    ShoeShine     a shoe-shine stand, two red thrones under a tall striped parasol
  PROPS, low (under 2 m, unlit: 17 m and more out):
    FruitStall    a barrow of fruit under a red-and-cream awning
    Boards        three sandwich boards with pictures (a cup, a hat, a pretzel), flower buckets, a milk churn
    Tricycles     a rank of three cream-and-red delivery tricycles with cargo boxes
    HatboxCart    a hand cart piled with striped hat boxes and parcels
  HERO:
    Percolator    the department store's tower in red and cream, a giant coffee pot tipped on its roof pouring a
                  steady ribbon of light into a cup the size of a house; three pale puffs of steam sway over the cup

No words anywhere (signs are pictures), no brand, no likeness. Nothing here blinks: light is steady and only on
pieces whose every placement is 24 m and more from the road. Movement is the builders' vertex motion only.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/CityCenter'
RED, CREAM, BRICK, AMBER, TEAL, WOOD = '#ff2d3c', '#fff1d6', '#6a3f44', '#ffd23f', '#2fc6c0', '#9a6a44'
BRICK_D, WOOD_D, WARM, WARM_L, WHITE, GLASS, IRON = '#54343c', '#6a4c36', '#ffe28a', '#fff1c0', '#fff6ea', '#233a63', '#3f3d50'
PLUM, PINE = '#4a3a78', '#2f6b52'
G_WIN, G_RIM, G_SOFT = 0.9, 1.3, 0.4            # steady light: a window, a line of light, a faint face
LOW = dict(grow=(10.0, 10.0, 6.0), grow_down=(10.0, 10.0, 0.0))    # low props: bounds stay under 2.5 m (no shadow)


# ------------------------------------------------------------------------------------------ helpers
def awning(g, x0, x1, y, z, cols=(RED, CREAM), n=6, depth=130.0, drop=60.0, thick=12.0):
    """A striped awning on a wall at y (the wall faces -Y): n stripes side by side, sloping out and down."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        xc = x0 + w * (k + 0.5)
        tilt_box(g, cols[k % len(cols)], (xc, y, z), (xc, y - depth, z - drop), w, thick)


def ring(g, col, c, R, t, n=12, flat=False, glow=0.0, a0=0.0, a1=360.0, d=None):
    """A ring (or an arc from a0 to a1 degrees) of n straight pieces: standing up facing -Y, or lying flat."""
    pts = []
    for k in range(n + 1):
        a = math.radians(a0 + (a1 - a0) * k / float(n))
        if flat:
            pts.append((c[0] + R * math.cos(a), c[1] + R * math.sin(a), c[2]))
        else:
            pts.append((c[0] + R * math.cos(a), c[1], c[2] + R * math.sin(a)))
    for k in range(n):
        tilt_box(g, col, pts[k], pts[k + 1], t, d or t, glow=glow)


def cone_rod(m, col, a, b, r0, r1, steps=10, glow=0.0):
    """A cone from a (radius r0) to b (radius r1), any way up (ft_desert.rod's own turning)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def ball(g, col, c, r, glow=0.0, steps=(4, 6)):
    g.sphere(col, c, r, steps=steps, glow=glow)


def body(g, H, wall=BRICK, band=CREAM, W=1100.0, D=900.0, floors=8, z1=1200.0, seed=1, lit=0.62, awn=(RED, CREAM), frm=0.45, cols=3):
    """A walk-up's shaft: 3 m of footing, a cream band at every floor, and from `frm` of the way up (the rest is behind
    the street's own towers) windows on the front and both ends, a striped awning over every other row."""
    r = random.Random(seed)
    g.box(wall, (0, 0, (H - 300.0) / 2.0), (W, D, H + 300.0))
    pitch = (H - z1) / float(floors)
    for f in range(floors):
        z = z1 + (f + 0.5) * pitch
        g.box(band, (0, 0, z - pitch * 0.5), (W + 26, D + 26, 46))
        if f < floors * frm:
            continue
        for k in range(cols):
            x = (k - (cols - 1) / 2.0) * W * 0.86 / cols
            on = r.random() < lit
            g.box((WARM if r.random() < 0.7 else WARM_L) if on else GLASS, (x, -D / 2.0 - 4, z), (W * 0.56 / cols, 12, pitch * 0.46),
                  glow=G_WIN if on else 0.0)
        for sx in (-1, 1):
            on = r.random() < lit
            g.box(WARM if on else GLASS, (sx * (W / 2.0 + 4), 0, z), (12, D * 0.3, pitch * 0.46), glow=G_WIN if on else 0.0)
        if awn and f % 2 == 1:
            awning(g, -W * 0.44, W * 0.44, -D / 2.0 - 8, z + pitch * 0.4, awn, 6)
    return pitch


def water_tank(g, x, y, z, r=250.0, h=360.0, legs=220.0):
    """A fat wooden water tank on four legs, cream hoops, a pointed roof with an amber ball."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(BRICK_D, (x + sx * r * 0.62, y + sy * r * 0.62, z + legs / 2.0), (30, 30, legs))
    g.box(WOOD_D, (x, y, z + legs + 12), (r * 1.7, r * 1.7, 24))
    g.cyl(WOOD, (x, y, z + legs + 24), r, h, steps=10)
    for k in (0.22, 0.74):
        g.cyl(CREAM, (x, y, z + legs + 24 + h * k), r + 8, 22, steps=10)
    g.cyl(WOOD_D, (x, y, z + legs + 24 + h), r + 22, r * 0.62, steps=10, top_r=14)
    ball(g, AMBER, (x, y, z + legs + 24 + h + r * 0.62 + 16), 26)
    return z + legs + 24 + h + r * 0.62 + 42


def fire_escape(g, x, y, z0, pitch, n=4, w=300.0, col=TEAL):
    """A fire escape drawn as fat bars: landings and the zigzag of stairs between them, on a wall at y."""
    for k in range(n):
        z = z0 + k * pitch
        g.box(col, (x, y - 50, z), (w, 100, 22))
        g.box(col, (x, y - 96, z + 60), (w, 14, 22))
        if k < n - 1:
            s = 1 if k % 2 == 0 else -1
            tilt_box(g, col, (x - s * w * 0.42, y - 60, z + 10), (x + s * w * 0.42, y - 60, z + pitch - 10), 26, 40)


def pennant(sc, x, y, z, col=TEAL, w=150.0, h=80.0, swing=14.0, rate=0.35, phase=0.0):
    """A flag on a mast top at (x, y, z), flying towards +X, swinging about the mast."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + w * 0.5, y, z), (w, 8, h))
    f.box(col, (x + w * 1.1, y, z + h * 0.25), (w * 0.3, 8, h * 0.5))


# ------------------------------------------------------------------------------------------ the buildings
def corner_store(sc):
    """The department store's corner block: a striped drum turret on the road-side corner, a two-storey show window
    lit amber, urns along the parapet, a roof terrace with striped parasols."""
    g = sc.still()
    H, W, D = 7000.0, 1100.0, 900.0
    body(g, H, BRICK, CREAM, seed=11, awn=(RED, CREAM), cols=2)
    g.box(CREAM, (0, 0, H + 45), (W + 90, D + 90, 90))
    g.box(RED, (0, 0, H + 110), (W + 40, D + 40, 40))
    # the show window: two storeys of warm light behind cream mullions, a red-and-cream awning over it
    g.box(WARM_L, (150, -D / 2.0 - 6, 5700), (640, 14, 900), glow=1.0)
    for k in range(4):
        g.box(CREAM, (150 - 320 + k * 213, -D / 2.0 - 14, 5700), (26, 20, 900))
    g.box(CREAM, (150, -D / 2.0 - 14, 5700), (640, 20, 26))
    awning(g, -190, 490, -D / 2.0 - 10, 6260, (RED, CREAM), 8, depth=170, drop=80)
    # the turret
    tx, ty = -W / 2.0 + 40, -D / 2.0 + 40
    for k in range(8):
        g.cyl(RED if k % 2 else CREAM, (tx, ty, 4000 + k * 430), 280, 430, steps=10)
    for k in range(3):
        a = math.radians(-150 + k * 30)
        g.box(WARM, (tx + 284 * math.cos(a), ty + 284 * math.sin(a), 6900), (90, 14, 260), rot=(0, math.degrees(a) + 90, 0), glow=G_WIN)
    g.cyl(CREAM, (tx, ty, 7440), 350, 56, steps=10)
    g.cyl(RED, (tx, ty, 7496), 320, 660, steps=10, top_r=18)
    g.cyl(BRICK_D, (tx, ty, 8150), 9, 300, steps=5)
    ball(g, AMBER, (tx, ty, 8180), 58)
    pennant(sc, tx, ty, 8400, TEAL, w=170, h=90, phase=0.2)
    # urns on the parapet, parasols and a tea pavilion on the roof
    for k in range(4):
        x = -120 + k * 200
        g.cyl(CREAM, (x, -D / 2.0 - 20, H + 130), 34, 90, steps=6, top_r=52)
    g.box(CREAM, (220, 180, H + 250), (420, 360, 240))
    awning(g, 20, 420, 0, H + 360, (TEAL, CREAM), 4, depth=130, drop=50)
    g.box(WARM, (220, -2, H + 230), (300, 10, 150), glow=G_WIN)
    for k, (x, y) in enumerate(((-150, -150), (-330, 160))):
        g.cyl(WOOD_D, (x, y, H + 130), 8, 250, steps=5)
        g.cyl(RED if k else AMBER, (x, y, H + 370), 170, 60, steps=8, top_r=12)
        g.cyl(CREAM, (x, y, H + 360), 176, 12, steps=8)


def walk_up(sc):
    """A brick walk-up: a fat wooden water tank on legs, a washing line across the flat roof, a fire escape in teal
    bars, a cream bay."""
    g = sc.still()
    H, W, D = 6200.0, 1100.0, 900.0
    p = body(g, H, BRICK, CREAM, seed=5, awn=(RED, CREAM), cols=2)
    g.box(CREAM, (0, 0, H + 35), (W + 70, D + 70, 70))
    g.box(BRICK_D, (0, D / 2.0 - 40, H + 150), (W, 80, 160))
    fire_escape(g, 300, -D / 2.0, 1200 + 4 * p - 40, p, n=4)
    g.box(CREAM, (-290, -D / 2.0 - 60, 4900), (300, 120, 2300))
    g.box(RED, (-290, -D / 2.0 - 60, 6080), (340, 160, 60))
    water_tank(g, 240, 120, H + 70)
    g.box(BRICK_D, (-330, 250, H + 190), (240, 240, 240))
    g.box(TEAL, (-330, 126, H + 160), (90, 10, 170))
    for k in range(2):
        g.cyl(BRICK_D, (-420 + k * 70, 300, H + 310), 24, 110, steps=6)
        g.cyl(RED, (-420 + k * 70, 300, H + 420), 28, 50, steps=6, top_r=20)
    # the washing line, along the front edge of the roof
    for x in (-500, -40):
        g.box(WOOD_D, (x, -330, H + 190), (16, 16, 240))
    g.box(WOOD_D, (-270, -330, H + 300), (460, 8, 8))
    wash = ((CREAM, 110, 130), (RED, 70, 100), (TEAL, 80, 90), (AMBER, 60, 110), (WHITE, 120, 120))
    for k, (col, w, h) in enumerate(wash):
        x = -450 + k * 92
        m = sc.part(P.motion(swing=9, pivot=(x, -330, H + 300), axis=0.0, rate=0.3, phase=(k % 3) / 3.0))
        m.box(col, (x, -330, H + 296 - h / 2.0), (w * 0.8, 8, h))


def hat_house(sc):
    """The hatter's: a round house in red and cream hoops wearing a hat: a wide brim, a red crown with an amber band and
    a teal bow, a cream cupola on top and a plume that sways."""
    g = sc.still()
    R, H = 520.0, 5600.0
    n = 8
    hh = (H + 300.0) / n
    for k in range(n):
        g.cyl(CREAM if k % 2 else RED, (0, 0, -300 + k * hh), R, hh, steps=12)
    r = random.Random(3)
    for row in range(3):
        z = H - 380 - row * hh
        for k in range(5):
            a = math.radians(-150 + k * 30)
            on = r.random() < 0.7
            g.box(WARM if on else GLASS, ((R + 2) * math.cos(a), (R + 2) * math.sin(a), z), (150, 14, 330), rot=(0, math.degrees(a) + 90, 0),
                  glow=G_WIN if on else 0.0)
    # awnings under the brim, all the way round the front
    for k in range(9):
        a0 = math.radians(-170 + k * 20)
        a1 = math.radians(-150 + k * 20)
        am = (a0 + a1) / 2.0
        tilt_box(g, AMBER if k % 2 else CREAM, (R * math.cos(am), R * math.sin(am), H - 60), ((R + 150) * math.cos(am), (R + 150) * math.sin(am), H - 130),
                 2 * (R + 75) * math.sin((a1 - a0) / 2.0), 12)
    g.cyl(CREAM, (0, 0, H), 800, 70, steps=14)
    g.cyl(RED, (0, 0, H + 70), 480, 760, steps=12, top_r=450)
    g.cyl(AMBER, (0, 0, H + 80), 494, 140, steps=12)
    g.box(TEAL, (-150, -490, H + 150), (240, 40, 150))
    g.box(TEAL, (-150, -506, H + 150), (60, 40, 190))
    g.cyl(CREAM, (0, 0, H + 830), 470, 40, steps=12)
    g.cyl(CREAM, (0, 0, H + 870), 190, 170, steps=8)
    for k in range(4):
        a = math.radians(45 + k * 90)
        g.box(WARM, (192 * math.cos(a), 192 * math.sin(a), H + 955), (90, 12, 110), rot=(0, math.degrees(a) + 90, 0), glow=G_WIN)
    g.sphere(CREAM, (0, 0, H + 1040), 230, scale=(1, 1, 0.8), steps=(4, 8))
    g.cyl(BRICK_D, (0, 0, H + 1200), 9, 180, steps=5)
    ball(g, AMBER, (0, 0, H + 1400), 46)
    pl = sc.part(P.motion(swing=6, pivot=(380, -300, H + 150), axis=0.0, rate=0.22, phase=0.4))
    tilt_box(pl, CREAM, (380, -300, H + 150), (520, -330, H + 620), 90, 40)
    tilt_box(pl, TEAL, (520, -330, H + 620), (760, -340, H + 900), 70, 36)
    tilt_box(pl, CREAM, (500, -330, H + 520), (700, -345, H + 640), 50, 30)


def bakery(sc):
    """The bakery: cream walls with red bands, a giant cream pretzel on a roof frame (a soft steady light), two brick
    stacks with a puff of steam, loaves for roof vents."""
    g = sc.still()
    H, W, D = 5400.0, 1100.0, 900.0
    body(g, H, CREAM, RED, seed=7, awn=(AMBER, CREAM))
    g.box(BRICK, (0, 0, H + 40), (W + 70, D + 70, 80))
    # the frame and the pretzel
    cz = H + 760.0
    for sx in (-1, 1):
        tilt_box(g, BRICK_D, (sx * 430, -260, H + 80), (sx * 250, -260, cz - 330), 30, 30)
        tilt_box(g, BRICK_D, (sx * 430, 40, H + 80), (sx * 250, -240, cz - 200), 26, 26)
    g.box(BRICK_D, (0, -260, H + 250), (880, 26, 26))
    pz = dict(glow=G_SOFT)
    ring(g, CREAM, (0, -270, cz), 400, 110, n=12, **pz)
    tilt_box(g, CREAM, (-230, -270, cz - 330), (190, -300, cz + 330), 100, 100, glow=G_SOFT)
    tilt_box(g, CREAM, (230, -270, cz - 330), (-190, -240, cz + 330), 100, 100, glow=G_SOFT)
    ball(g, CREAM, (0, -270, cz - 90), 120, glow=G_SOFT)
    for k in range(7):
        a = math.radians(20 + k * 52)
        g.box(AMBER, (400 * math.cos(a), -332, cz + 400 * math.sin(a)), (30, 16, 30))
    # stacks, steam, loaves
    for k, x in enumerate((-380, 380)):
        g.box(BRICK, (x, 280, H + 330), (190, 190, 500))
        g.box(CREAM, (x, 280, H + 595), (230, 230, 40))
        st = sc.part(P.motion(swing=3, pivot=(x, 280, H + 600), axis=0.0, rate=0.11, phase=0.5 * k, bob=-18))
        st.sphere(WHITE, (x + 20, 280, H + 760), 120, steps=(4, 6))
        st.sphere(WHITE, (x - 40, 290, H + 930), 90, steps=(4, 6))
    for k in range(3):
        x = -200 + k * 200
        rod(g, WOOD, (x, 120, H + 150), (x, 400, H + 150), 80, steps=8)
        for j in range(3):
            g.box(CREAM, (x, 180 + j * 80, H + 226), (90, 16, 14))


def toy_emporium(sc):
    """The toy emporium: red walls with cream bands and teal awnings, a tin drum for a tower (amber lacing, two sticks
    that beat slowly), a striped flagpole with a teal flag, toy blocks stacked on the corner."""
    g = sc.still()
    H, W, D = 5800.0, 1100.0, 900.0
    body(g, H, RED, CREAM, seed=9, awn=(TEAL, CREAM))
    g.box(TEAL, (0, 0, H + 40), (W + 80, D + 80, 80))
    g.box(CREAM, (0, 0, H + 95), (W + 30, D + 30, 30))
    # the drum
    dx, dy, dz, R, dh = 100.0, 120.0, H + 110.0, 380.0, 560.0
    g.cyl(RED, (dx, dy, dz), R, dh, steps=12)
    for z in (dz, dz + dh - 60):
        g.cyl(CREAM, (dx, dy, z), R + 22, 60, steps=12)
    g.cyl(WHITE, (dx, dy, dz + dh), R - 10, 14, steps=12)
    for k in range(12):
        a0, a1 = math.radians(k * 30), math.radians(k * 30 + 30)
        z0, z1 = (dz + 70, dz + dh - 70) if k % 2 == 0 else (dz + dh - 70, dz + 70)
        tilt_box(g, AMBER, (dx + (R + 8) * math.cos(a0), dy + (R + 8) * math.sin(a0), z0), (dx + (R + 8) * math.cos(a1), dy + (R + 8) * math.sin(a1), z1), 22, 22)
    for k, sx in enumerate((-1, 1)):
        px, pz = dx + sx * 520, dz + dh + 330
        m = sc.part(P.motion(swing=11, pivot=(px, dy - 60, pz), axis=90.0, rate=0.4, phase=0.5 * k))
        rod(m, WOOD, (px, dy - 60, pz), (dx + sx * 90, dy - 60, dz + dh + 90), 22, steps=6)
        ball(m, CREAM, (dx + sx * 90, dy - 60, dz + dh + 80), 52)
        g.box(TEAL, (px, dy - 60, dz + dh / 2.0 + 180), (30, 30, dh + 360))
    # the flagpole
    fx, fy = W / 2.0 - 90, -D / 2.0 + 90
    for k in range(8):
        g.cyl(RED if k % 2 else CREAM, (fx, fy, H + 110 + k * 190), 20, 190, steps=6)
    ball(g, AMBER, (fx, fy, H + 1660), 44)
    pennant(sc, fx, fy, H + 1500, TEAL, w=240, h=130, swing=16, rate=0.33, phase=0.1)
    # toy blocks, bigger than a pickup and none of them orange or green
    bx, by = -W / 2.0 + 190, -D / 2.0 + 170
    g.box(AMBER, (bx, by, H + 230), (240, 240, 240))
    g.box(TEAL, (bx + 250, by + 20, H + 230), (240, 240, 240), rot=(0, 12, 0))
    g.box(CREAM, (bx + 110, by + 10, H + 470), (240, 240, 240), rot=(0, 28, 0))
    ball(g, RED, (bx + 110, by + 10, H + 700), 110, steps=(5, 8))
    for (cx, cz) in ((bx, H + 230), (bx + 110, H + 470)):
        g.box(RED, (cx, by - 124, cz), (110, 10, 110))


def arcade_hall(sc):
    """The arcade hall: a barrel vault of warm glass on cream ribs, a loggia of five lit arches under it, four little
    turrets with red caps."""
    g = sc.still()
    H, W, D = 5600.0, 1100.0, 900.0
    body(g, H, BRICK, CREAM, seed=13, awn=(RED, CREAM), floors=7)
    g.box(CREAM, (0, 0, H + 40), (W + 70, D + 70, 80))
    # the loggia on the top floor
    lz = H - 420.0
    g.box(WARM, (0, -D / 2.0 - 8, lz), (W * 0.92, 12, 560), glow=1.0)
    for k in range(6):
        g.box(CREAM, (-W * 0.46 + k * W * 0.184, -D / 2.0 - 22, lz), (60, 44, 600))
    for k in range(5):
        x = -W * 0.368 + k * W * 0.184
        ring(g, CREAM, (x, -D / 2.0 - 22, lz + 180), 86, 40, n=5, a0=0, a1=180, d=44)
    g.box(CREAM, (0, -D / 2.0 - 22, lz + 300), (W * 0.96, 44, 70))
    awning(g, -W * 0.46, W * 0.46, -D / 2.0 - 44, lz - 290, (RED, CREAM), 8, depth=150, drop=70)
    # the vault
    vz = H + 80.0
    rod(g, WARM_L, (-W / 2.0 + 60, 0, vz), (W / 2.0 - 60, 0, vz), 400, steps=12, glow=G_WIN)
    for k in range(6):
        x = -W / 2.0 + 60 + k * (W - 120) / 5.0
        rod(g, CREAM, (x - 20, 0, vz), (x + 20, 0, vz), 418, steps=12)
    g.box(CREAM, (0, 0, vz + 420), (W - 60, 50, 40))
    for sx in (-1, 1):
        for k in range(3):
            a = math.radians(30 + k * 60)
            tilt_box(g, CREAM, (sx * (W / 2.0 - 30), 0, vz), (sx * (W / 2.0 - 30), 380 * math.cos(a), vz + 380 * math.sin(a)), 30, 30)
        g.cyl(BRICK_D, (sx * (W / 2.0 - 60), 0, vz + 420), 8, 150, steps=5)
        ball(g, AMBER, (sx * (W / 2.0 - 60), 0, vz + 600), 40)
        for sy in (-1, 1):
            g.cyl(CREAM, (sx * (W / 2.0 - 70), sy * (D / 2.0 - 70), H + 80), 70, 230, steps=6)
            g.cyl(RED, (sx * (W / 2.0 - 70), sy * (D / 2.0 - 70), H + 310), 90, 190, steps=6, top_r=8)
    pennant(sc, W / 2.0 - 60, 0, vz + 520, RED, w=150, h=80, phase=0.6)


def gable_house(sc):
    """A stepped gable in dark brick with cream caps and a round attic light, a cream bay five storeys tall, a teal fire
    escape, a red roof and a weather vane that swings."""
    g = sc.still()
    H, W, D = 6600.0, 1100.0, 900.0
    p = body(g, H, BRICK_D, CREAM, seed=17, awn=(RED, CREAM), cols=2)
    fy = -D / 2.0 + 70
    ws = (1100.0, 880.0, 660.0, 440.0, 220.0)
    for k, w in enumerate(ws):
        g.box(BRICK_D, (0, fy, H + 100 + k * 200), (w, 140, 200))
        g.box(CREAM, (0, fy, H + 212 + k * 200), (w + 30, 170, 30))
    rod(g, WARM_L, (0, fy - 78, H + 520), (0, fy - 60, H + 520), 120, steps=10, glow=1.0)
    ring(g, CREAM, (0, fy - 80, H + 520), 132, 30, n=8)
    for sx in (-1, 1):
        tilt_box(g, RED, (sx * 540, 80, H + 20), (0, 80, H + 900), D - 160, 30)
    # the bay
    g.box(CREAM, (-280, -D / 2.0 - 70, 4700), (320, 140, 3300))
    for k in range(5):
        on = k != 2
        g.box(WARM if on else GLASS, (-280, -D / 2.0 - 144, 3350 + k * 660), (220, 10, 380), glow=G_WIN if on else 0.0)
    tilt_box(g, RED, (-280, -D / 2.0, 6480), (-280, -D / 2.0 - 190, 6350), 360, 26)
    fire_escape(g, 290, -D / 2.0, 1200 + 4 * p - 60, p, n=4)
    for k, x in enumerate((-430, 430)):
        g.box(BRICK, (x, 300, H + 380), (150, 150, 760))
        for j in range(2):
            g.cyl(RED, (x - 36 + j * 72, 300, H + 760), 26, 90, steps=6, top_r=20)
    # the weather vane
    vz = H + 100 + 5 * 200 + 30
    g.cyl(BRICK_D, (0, fy, vz), 8, 330, steps=5)
    ball(g, AMBER, (0, fy, vz + 130), 34)
    v = sc.part(P.motion(swing=28, pivot=(0, fy, vz + 300), axis=P.UP, rate=0.12, phase=0.3))
    v.box(AMBER, (0, fy, vz + 300), (330, 10, 18))
    v.box(AMBER, (190, fy, vz + 300), (60, 10, 70))
    v.box(AMBER, (-150, fy, vz + 300), (90, 10, 90))
    v.box(CREAM, (-60, fy, vz + 350), (110, 10, 70))


# ------------------------------------------------------------------------------------------ the tall props
def barber_clock(sc):
    """A street clock on a barber's pole, 6.3 m: the red and teal stripes turn slowly round the cream pole, two warm
    clock faces (ten past ten, for ever), a little striped cap and an amber ball."""
    g = sc.still()
    g.box(BRICK, (0, 0, 30), (76, 76, 100))
    g.box(CREAM, (0, 0, 92), (90, 90, 24))
    g.cyl(CREAM, (0, 0, 104), 17, 396, steps=8)
    g.cyl(RED, (0, 0, 104), 26, 30, steps=8)
    g.cyl(RED, (0, 0, 470), 26, 30, steps=8)
    s = sc.part(P.motion(travel=(-1.0, 0.12), base=(0.0, 0.0)))
    for k in range(12):
        a0, a1 = math.radians(k * 60), math.radians(k * 60 + 60)
        z0 = 140 + k * 25
        tilt_box(s, RED if k % 2 else TEAL, (23 * math.cos(a0), 23 * math.sin(a0), z0), (23 * math.cos(a1), 23 * math.sin(a1), z0 + 40), 12, 6)
    hz = 560.0
    g.box(CREAM, (0, 0, 505), (46, 46, 20))
    rod(g, RED, (0, -26, hz), (0, 26, hz), 70, steps=12)
    rod(g, WARM_L, (0, -32, hz), (0, 32, hz), 58, steps=12, glow=G_SOFT)
    for sy in (-1, 1):
        y = sy * 34
        for k in range(4):
            a = math.radians(k * 90)
            g.box(BRICK_D, (46 * math.cos(a), y, hz + 46 * math.sin(a)), (9, 5, 9))
        tilt_box(g, BRICK_D, (0, y, hz), (-sy * 20, y, hz + 26), 7, 5)
        tilt_box(g, BRICK_D, (0, y, hz), (sy * 30, y, hz + 28), 6, 5)
    g.cyl(RED, (0, 0, hz + 66), 46, 30, steps=8, top_r=8)
    g.cyl(CREAM, (0, 0, hz + 62), 50, 6, steps=8)
    ball(g, AMBER, (0, 0, hz + 108), 13)


def hat_sign(sc):
    """A striped mast, 16 m, with a giant top hat hung from its arm (it sways along the street) and a pennant: the
    hatter's sign, a picture and no word. It also stands inside an alley's side wall: all the show is over 11 m."""
    g = sc.still()
    g.box(BRICK, (0, 0, -40), (60, 60, 320))
    for k in range(8):
        g.cyl(RED if k % 2 else CREAM, (0, 0, 120 + k * 185), 15, 185, steps=6)
    ball(g, AMBER, (0, 0, 1622), 24)
    az = 1500.0
    g.box(BRICK_D, (0, -70, az), (12, 150, 12))
    tilt_box(g, BRICK_D, (0, -8, az - 110), (0, -130, az - 8), 10, 10)
    ring(g, AMBER, (0, -75, az + 40), 28, 7, n=6, d=7)
    h = sc.part(P.motion(swing=5, pivot=(0, -110, az), axis=90.0, rate=0.25, phase=0.2))
    h.box(BRICK_D, (0, -110, az - 30), (6, 6, 60))
    h.cyl(CREAM, (0, -110, az - 78), 96, 14, steps=10)
    h.cyl(RED, (0, -110, az - 204), 62, 126, steps=10, top_r=70)
    h.cyl(AMBER, (0, -110, az - 204), 66, 30, steps=10)
    h.cyl(CREAM, (0, -110, az - 218), 100, 14, steps=10)
    ball(h, WARM_L, (0, -110, az - 250), 16, glow=G_WIN)
    g.box(BRICK_D, (0, 40, 1250), (8, 90, 8))
    pennant(sc, 0, 84, 1220, TEAL, w=90, h=56, swing=12, rate=0.4, phase=0.7)


def cup_sign(sc):
    """A teal column, 15 m, with a swan neck holding a giant cream coffee cup on its saucer, three amber curls of steam
    over it (they sway): the coffee house's sign."""
    g = sc.still()
    g.box(BRICK, (0, 0, -40), (64, 64, 320))
    g.cyl(TEAL, (0, 0, 120), 17, 1180, steps=8, top_r=13)
    for z in (140, 700, 1260):
        g.cyl(CREAM, (0, 0, z), 23, 34, steps=8)
    pts = ((0, 0, 1300), (0, -30, 1420), (0, -90, 1490), (0, -150, 1470))
    for k in range(3):
        tilt_box(g, TEAL, pts[k], pts[k + 1], 14, 14)
    ball(g, AMBER, (0, 0, 1316), 20)
    c = sc.part(P.motion(swing=4, pivot=(0, -150, 1470), axis=90.0, rate=0.22, phase=0.5))
    cz = 1300.0
    c.box(BRICK_D, (0, -150, cz + 120), (6, 6, 100))
    c.cyl(CREAM, (0, -150, cz - 14), 96, 12, steps=10)
    c.cyl(CREAM, (0, -150, cz), 44, 76, steps=10, top_r=66)
    c.cyl(RED, (0, -150, cz + 44), 61, 14, steps=10, top_r=65)
    c.cyl(WOOD_D, (0, -150, cz + 72), 58, 6, steps=10)
    ring(c, CREAM, (74, -150, cz + 40), 22, 9, n=6, d=9)
    for k in range(3):
        x = -30 + k * 30
        st = sc.part(P.motion(swing=7, pivot=(x, -150, cz + 80), axis=0.0, rate=0.3, phase=k / 3.0))
        tilt_box(st, AMBER, (x, -150, cz + 86), (x + 12, -150, cz + 112), 7, 7)
        tilt_box(st, AMBER, (x + 12, -150, cz + 112), (x - 8, -150, cz + 140), 7, 7)


def bunting(sc):
    """Two cream poles 11.2 m apart and 14.6 m tall: a string of pennants and, under it, a washing line (sheets, long
    johns, shirts, socks), both swaying. Its poles stand inside an alley's side walls; the lines hang 10.6 to 13.9 m up."""
    g = sc.still()
    for sx in (-1, 1):
        g.box(CREAM, (sx * 560, 0, 580), (22, 22, 1760))
        g.cyl(RED, (sx * 560, 0, 1460), 20, 40, steps=6, top_r=6)
        ball(g, AMBER, (sx * 560, 0, 1512), 16)
    n = 14
    cols = (RED, CREAM, TEAL, AMBER)

    def sag(x, z_end, dip):
        t = x / 560.0
        return z_end - dip * (1.0 - t * t)

    for k in range(n):
        x0, x1 = -560 + k * 80.0, -560 + (k + 1) * 80.0
        tilt_box(g, BRICK_D, (x0, 0, sag(x0, 1420, 60)), (x1, 0, sag(x1, 1420, 60)), 5, 5)
        tilt_box(g, WOOD_D, (x0, 30, sag(x0, 1260, 50)), (x1, 30, sag(x1, 1260, 50)), 5, 5)
    for k in range(13):
        x = -520 + k * 86.6
        z = sag(x, 1420, 60)
        m = sc.part(P.motion(swing=12, pivot=(x, 0, z), axis=0.0, rate=0.35, phase=(k % 4) / 4.0))
        m.box(cols[k % 4], (x, 0, z - 24), (52, 5, 44))
        m.box(cols[k % 4], (x, 0, z - 58), (26, 5, 26))
    wash = ((CREAM, 170, 150), (RED, 60, 120), (RED, 60, 120), (TEAL, 90, 90), (WHITE, 110, 80), (AMBER, 70, 110), (CREAM, 40, 60), (CREAM, 40, 60), (TEAL, 120, 130))
    x = -480.0
    for k, (col, w, h) in enumerate(wash):
        xc = x + w / 2.0
        z = sag(xc, 1260, 50)
        m = sc.part(P.motion(swing=8, pivot=(xc, 30, z), axis=0.0, rate=0.28, phase=(k % 3) / 3.0))
        m.box(col, (xc, 30, z - h / 2.0 - 4), (w, 5, h))
        x += w + (1040.0 - 760.0) / 8.0 - 10


def water_tank_prop(sc):
    """A wooden water tank on tall braced legs, 16 m: cream hoops, a pointed roof, a teal ladder, a little striped
    pennant. Made to straddle an alley."""
    g = sc.still()
    L, hx, hy = 1100.0, 250.0, 160.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(BRICK_D, (sx * hx, sy * hy, L / 2.0 - 100), (26, 26, L + 200))
        tilt_box(g, BRICK_D, (sx * hx, -hy, 620), (sx * hx, hy, 1040), 16, 16)
    for z in (600, 1060):
        g.box(BRICK_D, (0, -hy, z), (2 * hx, 16, 16))
        g.box(BRICK_D, (0, hy, z), (2 * hx, 16, 16))
    tilt_box(g, BRICK_D, (-hx, -hy, 620), (hx, -hy, 1040), 16, 16)
    g.box(WOOD_D, (0, 0, L + 12), (2 * hx + 120, 2 * hy + 160, 24))
    g.cyl(WOOD, (0, 0, L + 24), 240, 330, steps=10)
    for k in (0.2, 0.5, 0.8):
        g.cyl(CREAM, (0, 0, L + 24 + 330 * k), 248, 20, steps=10)
    g.cyl(WOOD_D, (0, 0, L + 354), 262, 150, steps=10, top_r=14)
    g.cyl(BRICK_D, (0, 0, L + 500), 6, 90, steps=5)
    ball(g, AMBER, (0, 0, L + 520), 18)
    pennant(sc, 0, 0, L + 572, RED, w=70, h=40, swing=14, rate=0.4, phase=0.3)
    for sx in (-1, 1):
        g.box(TEAL, (150 + sx * 24, -252, L + 100), (7, 7, 520))
    for k in range(8):
        g.box(TEAL, (150, -252, L - 130 + k * 62), (48, 7, 7))
    tilt_box(g, TEAL, (-200, -150, L + 60), (-200, -250, L - 140), 14, 14)


def shoe_shine(sc):
    """A shoe-shine stand: a cream platform, two red thrones with amber foot rests, drawers of brushes, a tall red and
    cream parasol (it nods) and a picture of a boot on a bracket."""
    g = sc.still()
    g.box(CREAM, (0, 20, 30), (260, 150, 60))
    g.box(RED, (0, -62, 30), (260, 14, 44))
    g.box(BRICK, (0, -80, 12), (200, 40, 24))
    for sx in (-1, 1):
        x = sx * 66
        g.box(RED, (x, 30, 96), (88, 84, 22))
        g.box(RED, (x, 68, 166), (88, 14, 140))
        g.box(AMBER, (x, 68, 244), (96, 16, 16))
        for a in (-1, 1):
            g.box(WOOD_D, (x + a * 40, 30, 122), (10, 80, 10))
        g.box(WOOD_D, (x, 30, 72), (70, 70, 26))
        g.box(AMBER, (x, -36, 86), (26, 44, 10))
        g.box(BRICK_D, (x, -36, 70), (8, 8, 30))
    g.box(WOOD, (0, -36, 76), (36, 50, 32))
    for k in range(3):
        g.box((RED, TEAL, AMBER)[k], (-8 + k * 9, -36, 100), (6, 30, 14))
    m = sc.part(P.motion(swing=2, pivot=(0, 96, 60), axis=90.0, rate=0.2, phase=0.1))
    m.cyl(WOOD_D, (0, 96, 60), 6, 268, steps=5)
    for k in range(8):
        a0, a1 = math.radians(k * 45), math.radians(k * 45 + 45)
        am = (a0 + a1) / 2.0
        tilt_box(m, RED if k % 2 else CREAM, (0, 96, 344), (150 * math.cos(am), 96 + 150 * math.sin(am), 296), 2 * 80 * math.sin(math.radians(22.5)) + 60, 6)
    ball(m, AMBER, (0, 96, 350), 10)
    g.box(BRICK_D, (124, 20, 150), (8, 8, 240))
    g.box(BRICK_D, (124, -10, 262), (8, 60, 8))
    g.box(BRICK_D, (124, -36, 222), (8, 26, 60))
    g.box(BRICK_D, (124, -52, 196), (8, 58, 22))
    g.box(AMBER, (124, -52, 182), (9, 62, 6))


# ------------------------------------------------------------------------------------------ the low props (under 2 m, unlit)
def wheel(g, x, y, z, r, col=WOOD_D, hub=AMBER, w=8.0):
    """A cart wheel standing in the X-Z plane, its axle along Y."""
    rod(g, col, (x, y - w / 2.0, z), (x, y + w / 2.0, z), r, steps=10)
    rod(g, hub, (x, y - w / 2.0 - 2, z), (x, y + w / 2.0 + 2, z), r * 0.28, steps=6)


def wheel_x(g, x, y, z, r, col=IRON, hub=CREAM, w=8.0):
    """A wheel whose axle lies along X (a tricycle parked nose to the road)."""
    rod(g, col, (x - w / 2.0, y, z), (x + w / 2.0, y, z), r, steps=10)
    rod(g, hub, (x - w / 2.0 - 2, y, z), (x + w / 2.0 + 2, y, z), r * 0.3, steps=6)


def fruit_stall(sc):
    """A fruit barrow under a red-and-cream awning, 3.6 m long, 1.93 m to the awning's ridge: stepped trays of red,
    amber, cream and plum fruit, a pair of amber scales, a price board with pictures."""
    g = sc.still()
    g.box(WOOD, (0, 10, 78), (300, 96, 12))
    g.box(WOOD_D, (0, -40, 60), (300, 8, 36))
    for sx in (-1, 1):
        wheel(g, sx * 110, -52, 36, 36)
        g.box(WOOD_D, (sx * 142, 50, 39), (10, 10, 78))
        g.box(WOOD_D, (sx * 172, 0, 82), (56, 8, 8))
    trays = ((-100, RED), (-34, AMBER), (34, CREAM), (100, PLUM))
    for k, (x, col) in enumerate(trays):
        g.box(WOOD_D, (x, -14, 92), (60, 44, 16))
        g.box(WOOD_D, (x, 34, 110), (60, 44, 44))
        g.sphere(col, (x, -14, 104), 26, scale=(1.05, 0.78, 0.5), steps=(3, 6))
        g.sphere(col, (x, 34, 136), 26, scale=(1.05, 0.78, 0.55), steps=(3, 6))
        g.box(CREAM, (x, -38, 88), (22, 4, 14))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WOOD_D, (sx * 150, 10 + sy * 50, 124), (7, 7, 92))
    for k in range(8):
        x = -160 + k * 40 + 20
        col = RED if k % 2 else CREAM
        tilt_box(g, col, (x, 14, 190), (x, -72, 166), 40, 6)
        tilt_box(g, col, (x, 14, 190), (x, 72, 172), 40, 6)
    f = sc.part(P.motion(swing=5, pivot=(0, -72, 166), axis=0.0, rate=0.3, phase=0.2))
    for k in range(8):
        f.box(CREAM if k % 2 else RED, (-160 + k * 40 + 20, -73, 157), (36, 5, 16))
    g.box(BRICK_D, (126, -30, 102), (5, 5, 44))
    g.box(BRICK_D, (126, -30, 124), (44, 5, 5))
    for sx in (-1, 1):
        g.cyl(AMBER, (126 + sx * 18, -30, 104), 11, 5, steps=6)
    g.box(BRICK_D, (-176, -60, 44), (8, 50, 88))
    g.box(CREAM, (-181, -60, 54), (4, 40, 56))
    g.sphere(RED, (-184, -60, 66), 9, steps=(3, 5))
    g.sphere(AMBER, (-184, -60, 44), 9, steps=(3, 5))


def boards(sc):
    """Three sandwich boards with pictures (a cup, a hat, a pretzel's ring), four buckets of flowers that nod, a striped
    milk churn: 3.4 m long, 1.2 m tall."""
    g = sc.still()
    pics = (RED, TEAL, AMBER)
    for k in range(3):
        x = -120 + k * 78
        for sy in (-1, 1):
            tilt_box(g, CREAM, (x, sy * 26, 0), (x, sy * 3, 104), 56, 6)
            tilt_box(g, RED if k != 1 else TEAL, (x, sy * 27.5, 4), (x, sy * 22, 26), 58, 5)
        y = -17.5
        if k == 0:
            g.box(pics[k], (x - 3, y - 2, 60), (22, 5, 20))
            g.box(pics[k], (x + 12, y - 2, 62), (8, 5, 10))
            g.box(pics[k], (x - 3, y - 4, 46), (34, 5, 5))
        elif k == 1:
            g.box(pics[k], (x, y - 3, 66), (20, 5, 24))
            g.box(pics[k], (x, y - 5, 52), (38, 5, 6))
        else:
            ring(g, pics[k], (x, y - 3, 60), 13, 6, n=6, d=5)
    for k in range(4):
        x = 70 + k * 30
        y = -16 if k % 2 else 18
        g.cyl((TEAL, RED, CREAM, TEAL)[k], (x, y, 0), 11, 30, steps=6, top_r=15)
        m = sc.part(P.motion(swing=5, pivot=(x, y, 30), axis=0.0, rate=0.3, phase=k / 4.0))
        for j in range(3):
            a = math.radians(j * 120 + k * 40)
            m.box(PINE, (x + 5 * math.cos(a), y + 5 * math.sin(a), 44), (5, 5, 30))
            m.sphere((RED, CREAM, AMBER, WHITE)[(k + j) % 4], (x + 8 * math.cos(a), y + 8 * math.sin(a), 62), 10, steps=(3, 5))
    for k in range(4):
        g.cyl(CREAM if k % 2 else RED, (-164, 0, k * 18), 20, 18, steps=8)
    g.cyl(CREAM, (-164, 0, 72), 20, 16, steps=8, top_r=11)
    g.cyl(BRICK_D, (-164, 0, 88), 13, 8, steps=8)


def tricycles(sc):
    """A rank of three delivery tricycles, nose to the road: a cream cargo box with a red lid and stripe between two
    front wheels, a saddle, teal handlebars, one wheel behind; a little amber pennant on each (it flutters). Pedal
    tricycles, not motorbikes: no engine, no fairing, no rider. 4.2 m long, 1.6 m to the pennants."""
    g = sc.still()
    g.box(BRICK_D, (0, 92, 40), (400, 8, 8))
    for sx in (-1, 1):
        g.box(BRICK_D, (sx * 196, 92, 20), (8, 8, 48))
    for k in range(3):
        x = -134 + k * 134
        g.box(CREAM, (x, -30, 62), (74, 70, 54))
        g.box(RED, (x, -30, 93), (78, 74, 8))
        g.box(RED, (x, -66, 62), (60, 4, 14))
        g.box((TEAL, AMBER, TEAL)[k], (x, -67, 62), (16, 4, 16))
        for sx in (-1, 1):
            wheel_x(g, x + sx * 44, -30, 30, 30)
        g.box(BRICK_D, (x, 30, 46), (8, 70, 8))
        tilt_box(g, BRICK_D, (x, 10, 46), (x, 30, 84), 8, 8)
        g.box(RED, (x, 34, 90), (26, 34, 10))
        tilt_box(g, BRICK_D, (x, 2, 60), (x, 8, 112), 7, 7)
        g.box(TEAL, (x, 8, 114), (60, 7, 7))
        wheel_x(g, x, 66, 30, 30)
        g.box(BRICK_D, (x + 30, 58, 100), (5, 5, 104))
        m = sc.part(P.motion(swing=14, pivot=(x + 30, 58, 144), axis=P.UP, rate=0.5, phase=k / 3.0))
        m.box(AMBER, (x + 46, 58, 144), (30, 5, 18))


def hatbox_cart(sc):
    """A hand cart piled with round striped hat boxes tied with amber ribbon and brown-paper parcels, a red cloche on a
    hat stand: 2.6 m long, 1.8 m tall."""
    g = sc.still()
    g.box(WOOD, (0, 0, 70), (190, 90, 10))
    for sy in (-1, 1):
        g.box(WOOD_D, (0, sy * 43, 86), (190, 6, 26))
        wheel(g, 10, sy * 54, 46, 46)
    for sy in (-1, 1):
        tilt_box(g, WOOD_D, (95, sy * 36, 74), (128, sy * 36, 96), 8, 8)
    g.box(WOOD_D, (128, 0, 96), (8, 84, 8))
    g.box(WOOD_D, (-84, 0, 35), (10, 10, 70))
    boxes = ((-50, -6, 75, 38, 34, RED), (26, 8, 75, 34, 30, CREAM), (-44, 0, 109, 30, 28, CREAM), (30, 4, 105, 28, 24, TEAL), (-8, 2, 137, 24, 22, RED))
    for (x, y, z, r, h, col) in boxes:
        g.cyl(col, (x, y, z), r, h * 0.5, steps=8)
        g.cyl(CREAM if col != CREAM else RED, (x, y, z + h * 0.5), r, h * 0.3, steps=8)
        g.cyl(col, (x, y, z + h * 0.8), r + 2, h * 0.2, steps=8)
        g.box(AMBER, (x, y, z + h + 2), (2 * r + 4, 6, 4))
    g.box(WOOD, (70, -20, 130), (40, 30, 22))
    g.box(RED, (70, -20, 130), (42, 5, 24))
    g.box(CREAM, (-74, 20, 150), (30, 30, 18))
    g.box(RED, (-74, 20, 150), (5, 32, 20))
    g.box(BRICK_D, (72, 18, 132), (5, 5, 44))
    g.cyl(RED, (72, 18, 150), 17, 20, steps=8, top_r=13)
    g.cyl(RED, (72, 18, 148), 22, 5, steps=8)
    g.box(TEAL, (72, 2, 156), (10, 5, 8))


# ------------------------------------------------------------------------------------------ the hero
def percolator(sc):
    """THE PERCOLATOR. The department store's tower: a brick base, a red shaft in cream bands between cream piers, a
    cream top stage; on its roof a giant red-and-cream coffee pot tipped over a cup the size of a house that stands on
    a bracket at the tower's shoulder. The pour is a steady cream ribbon of light; three pale puffs of steam sway over
    the cup. 111 m as built (the table stands it at 1.25)."""
    g = sc.still()
    r = random.Random(21)
    g.box(BRICK, (0, 0, 1350), (2600, 2200, 3300))
    g.box(CREAM, (0, 0, 3040), (2700, 2300, 80))
    awning(g, -1200, 1200, -1150, 3000, (RED, CREAM), 10, depth=260, drop=120, thick=20)
    g.box(RED, (0, 0, 5740), (2300, 1900, 5320))
    for k in range(8):
        g.box(CREAM, (0, 0, 3400 + k * 660), (2340, 1940, 60))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(CREAM, (sx * 1150, sy * 950, 5800), (170, 170, 5600))
    for row in range(7):
        z = 3740 + row * 660
        for k in range(4):
            on = r.random() < 0.72
            g.box((WARM if r.random() < 0.7 else WARM_L) if on else GLASS, (-720 + k * 480, -956, z), (260, 12, 380), glow=G_WIN if on else 0.0)
        if row >= 2:
            for sx in (-1, 1):
                for k in range(2):
                    on = r.random() < 0.7
                    g.box(WARM if on else GLASS, (sx * 1156, -380 + k * 760, z), (12, 300, 380), glow=G_WIN if on else 0.0)
    g.box(CREAM, (0, 0, 8440), (2560, 2160, 90))
    g.box(CREAM, (0, 0, 8900), (2000, 1700, 840))
    g.box(RED, (0, 0, 9120), (2030, 1730, 90))
    awning(g, -900, 900, -850, 8760, (RED, CREAM), 6, depth=220, drop=100, thick=18)
    rod(g, CREAM, (0, -868, 9000), (0, -850, 9000), 250, steps=12)
    g.cyl(RED, (-30, -872, 8900), 90, 150, steps=8, top_r=130)       # the store's sign: a cup, a picture and no word
    ring(g, RED, (140, -872, 8980), 50, 22, n=6, d=14)
    for k in range(3):
        g.box(WARM, (-600 + k * 600, -856, 8640), (300, 12, 240), glow=G_WIN)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.cyl(CREAM, (sx * 1150, sy * 950, 8600), 110, 420, steps=6, top_r=40)
            ball(g, AMBER, (sx * 1150, sy * 950, 9060), 70)
    g.cyl(CREAM, (0, 0, 9320), 1000, 60, steps=14)
    # the pot, tipped 38 degrees towards +X
    t = math.radians(38.0)
    d = (math.sin(t), 0.0, math.cos(t))
    n = (math.cos(t), 0.0, -math.sin(t))
    A = (-350.0, 0.0, 9750.0)

    def at(u, v=0.0, y=0.0):
        return (A[0] + d[0] * u + n[0] * v, y, A[2] + d[2] * u + n[2] * v)

    g.box(RED, (-560, 0, 9530), (700, 900, 300))
    tilt_box(g, CREAM, (-900, 0, 9700), (60, 0, 9400), 760, 60)
    cone_rod(g, CREAM, at(-40), at(40), 590, 590, steps=12)
    cone_rod(g, RED, at(0), at(1300), 560, 420, steps=12)
    cone_rod(g, CREAM, at(520), at(690), 512, 494, steps=12)
    cone_rod(g, CREAM, at(1300), at(1560), 440, 200, steps=12)
    cone_rod(g, RED, at(1560), at(1620), 200, 170, steps=12)
    g.sphere(AMBER, at(1700), 120, steps=(5, 8), glow=1.0)
    rod(g, RED, at(480, 420), at(1250, 980), 115, steps=8)
    rod(g, CREAM, at(1210, 950), at(1290, 1010), 130, steps=8)
    hp = (at(320, -520), at(320, -900), at(1000, -900), at(1000, -430))
    for k in range(3):
        tilt_box(g, CREAM, hp[k], hp[k + 1], 150, 110)
    # the cup on its bracket
    cx, cz = 1750.0, 8520.0
    g.box(CREAM, (1700, 0, cz - 60), (1300, 1300, 90))
    for sy in (-1, 1):
        tilt_box(g, RED, (1160, sy * 500, 7500), (2250, sy * 500, cz - 100), 90, 90)
    g.cyl(CREAM, (cx, 0, cz - 14), 760, 50, steps=14)
    g.cyl(CREAM, (cx, 0, cz + 36), 380, 620, steps=12, top_r=560)
    g.cyl(RED, (cx, 0, cz + 380), 494, 110, steps=12, top_r=526)
    g.cyl(WOOD_D, (cx, 0, cz + 620), 520, 26, steps=12)
    ring(g, CREAM, (cx + 600, 0, cz + 340), 170, 80, n=8, d=80)
    pour = (at(1250, 980), (1450.0, 0.0, 9950.0), (1620.0, 0.0, 9620.0), (1700.0, 0.0, cz + 640))
    for k in range(3):
        tilt_box(g, WARM_L, pour[k], pour[k + 1], 100, 100, glow=G_RIM)
    for k in range(3):
        px = cx - 60 + (k % 2) * 220 - 40 * k
        pz = cz + 1150 + k * 430
        m = sc.part(P.motion(swing=3, pivot=(cx, 0, cz + 640), axis=0.0, rate=0.1 + 0.02 * k, phase=k / 3.0, bob=-40))
        m.sphere(WHITE, (px, 0, pz), 210 - 30 * k, steps=(5, 8), glow=0.25)
        m.sphere(WHITE, (px + 150, 40, pz + 110), 150 - 20 * k, steps=(4, 6), glow=0.25)
        m.sphere(WHITE, (px - 130, -30, pz + 90), 130 - 20 * k, steps=(4, 6), glow=0.25)


JOBS = [
    ('SM_S1D1_CornerStore', corner_store), ('SM_S1D1_WalkUp', walk_up), ('SM_S1D1_HatHouse', hat_house), ('SM_S1D1_Bakery', bakery),
    ('SM_S1D1_ToyEmporium', toy_emporium), ('SM_S1D1_ArcadeHall', arcade_hall), ('SM_S1D1_GableHouse', gable_house),
    ('SM_S1D1_BarberClock', barber_clock), ('SM_S1D1_HatSign', hat_sign), ('SM_S1D1_CupSign', cup_sign), ('SM_S1D1_Bunting', bunting),
    ('SM_S1D1_WaterTank', water_tank_prop), ('SM_S1D1_ShoeShine', shoe_shine),
    ('SM_S1D1_FruitStall', fruit_stall, LOW), ('SM_S1D1_Boards', boards, LOW), ('SM_S1D1_Tricycles', tricycles, LOW),
    ('SM_S1D1_HatboxCart', hatbox_cart, LOW),
    ('SM_S1D1_Percolator', percolator, dict(far=True, grow=(80.0, 80.0, 80.0), grow_down=(80.0, 80.0, 0.0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
