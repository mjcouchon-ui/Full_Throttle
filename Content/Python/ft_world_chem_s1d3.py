"""Chemical Works, Stage 1, builder s1d3: district 3 "Loading Yards" (5.35 to 8.09 km), the freight town.
/Game/World/ChemicalWorks/SM_S1D3_*. Run: import ft_world_chem_s1d3 as K; K.run()

The yards' own town: stacked containers, sheds and signal cabins of one to four storeys under flat and barrel roofs,
outside stairs, diagonal hazard stripes, huge numerals and arrows (digits only, never a word). Palette: hazard yellow
(the lead), red, blue, steel, white; night concrete and paving for what it stands on, the kit's lit-window cream.
Every piece is one ft_people.Scene (one material, one section). Placed by C:/ftwork/s1/L1_s1d3.inl.

Seen from the road a piece's +X is on the LEFT (the front is -Y and the camera looks along +Y), so numerals are
drawn mirrored in X: digit() and number() do it.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box            # rod(m, col, a, b, r, steps=), tilt_box(m, col, a, b, width, depth)

DIR = '/Game/World/ChemicalWorks'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))

YEL, RED, BLUE, STEEL, WHITE = '#ffd23f', '#ff3355', '#3a5ab8', '#8f9bb3', '#eef6ff'
BLACK, NAVY, CONC, CONC_D, PAVE = '#1c1a26', '#1c1f4a', '#3c4a78', '#2a3560', '#7f8fc6'
WARM, GLASS, RUST, STEAM, BLUE_D = '#ffe9a8', '#233a63', '#a8552f', '#d8d0f0', '#2a3f8f'


# ------------------------------------------------------------------------------------------ helpers
def _ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def side(m, col, pts, y0, y1, glow=0.0):
    """A profile [(x, z)] in the plane the road sees, from y0 to y1."""
    m.side(col, _ccw(pts), y0, y1, glow=glow)


def px(m, col, x0, x1, y, z, r, steps=10, glow=0.0):
    """A pipe (a tank, an axle) along X."""
    m.cyl(col, (x0, y, z), r, x1 - x0, rot=(-90, 0, 0), steps=steps, glow=glow)


def py(m, col, x, y0, y1, z, r, steps=10, glow=0.0):
    """A pipe (a wheel, a dial) along Y."""
    m.cyl(col, (x, y0, z), r, y1 - y0, rot=(0, 0, 90), steps=steps, glow=glow)


def arc_pts(width, rise, z, n=8, c=0.0):
    """The outline of a barrel roof: an arc `rise` high over a chord `width` wide at height z, middle at c."""
    half = width / 2.0
    r = (half * half + rise * rise) / (2.0 * rise)
    a0 = math.asin(max(0.0, min(1.0, (r - rise) / r)))
    out = []
    for k in range(n + 1):
        a = a0 + (math.pi - 2.0 * a0) * k / n
        out.append((c + r * math.cos(a), z + rise - r + r * math.sin(a)))
    return out


def barrel_y(m, col, cx, y0, y1, z, width, rise, n=8, glow=0.0):
    """A barrel roof (or an arch) whose curve faces the road: it runs from y0 to y1."""
    side(m, col, arc_pts(width, rise, z, n, cx), y0, y1, glow)


def barrel_x(m, col, x0, x1, cy, z, width, rise, n=8):
    """A barrel roof whose ridge runs along the road, from x0 to x1 (the profile turned a quarter turn)."""
    m.side(col, _ccw(arc_pts(width, rise, z, n, cy)), -x1, -x0, rot=(0, 90, 0))


def hazard(m, x0, x1, z0, z1, y, c1=YEL, c2=BLACK):
    """Diagonal hazard stripes on a face at y that looks at the road: a band from x0 to x1 and z0 to z1."""
    h = z1 - z0
    m.box(c1, ((x0 + x1) / 2.0, y - 3, (z0 + z1) / 2.0), (x1 - x0, 6, h))
    x = x0 + h * 0.25
    while x + 2.0 * h <= x1:
        side(m, c2, [(x, z0), (x + h, z0), (x + 2.0 * h, z1), (x + h, z1)], y - 9, y - 1)
        x += 2.0 * h


SEG = {'0': 'abcdef', '1': 'bc', '2': 'abged', '3': 'abgcd', '4': 'fgbc', '5': 'afgcd', '6': 'afgedc', '7': 'abc',
       '8': 'abcdefg', '9': 'abfgcd'}


def digit(m, col, ch, x, z, h, y, glow=0.0):
    """A blocky numeral h tall, its middle at (x, z), proud of a face at y that looks at the road."""
    w, t, hh = h * 0.54, h * 0.16, h / 2.0
    q = hh / 2.0
    place = {'a': (0, hh - t / 2.0, w, t), 'g': (0, 0.0, w, t), 'd': (0, -hh + t / 2.0, w, t),
             'f': (-1, q, t, hh), 'b': (1, q, t, hh), 'e': (-1, -q, t, hh), 'c': (1, -q, t, hh)}
    for s in SEG[ch]:
        sx, dz, bw, bh = place[s]
        m.box(col, (x - sx * (w / 2.0 - t / 2.0), y - 4, z + dz), (bw, 10, bh), glow=glow)


def number(m, col, text, x, z, h, y, glow=0.0):
    """Digits side by side, the first on the viewer's left (the mesh's +X)."""
    step = h * 0.76
    for i, ch in enumerate(text):
        digit(m, col, ch, x + ((len(text) - 1) / 2.0 - i) * step, z, h, y, glow)


def arrow(m, col, x, z, length, y, to_left=True, t=None):
    """A fat arrow on a face that looks at the road; to_left: it points to the viewer's left (the mesh's +X)."""
    t = t or length * 0.2
    s = 1.0 if to_left else -1.0
    m.box(col, (x - s * length * 0.15, y - 4, z), (length * 0.7, 10, t))
    side(m, col, [(x + s * length * 0.5, z), (x + s * length * 0.1, z + t * 1.5), (x + s * length * 0.1, z - t * 1.5)], y - 9, y + 1)


def window(m, x, z, w, h, y, lit=0.45, frame=WHITE):
    """A framed window on a face at y that looks at the road; lit 0 = dark glass."""
    m.box(frame, (x, y - 5, z), (w + 18, 10, h + 18))
    m.box(WARM if lit else GLASS, (x, y - 8, z), (w, 10, h), glow=lit)


def rail_x(m, col, x0, x1, y, z, h=100.0, n=4):
    """A handrail along X standing on z: a top bar, a knee bar and posts."""
    m.box(col, ((x0 + x1) / 2.0, y, z + h), (x1 - x0, 6, 6))
    m.box(col, ((x0 + x1) / 2.0, y, z + h * 0.5), (x1 - x0, 5, 5))
    for k in range(n + 1):
        m.box(col, (x0 + (x1 - x0) * k / float(n), y, z + h / 2.0), (6, 6, h))


def ladder(m, col, x, y, z0, z1, step=90.0):
    """A ladder on a face that looks at the road."""
    for s in (-1, 1):
        m.box(col, (x + s * 22, y, (z0 + z1) / 2.0), (7, 7, z1 - z0))
    k = z0 + 40.0
    while k < z1:
        m.box(col, (x, y, k), (44, 6, 6))
        k += step


def flight(m, col, a, b, w=90.0, rail=YEL):
    """A stair flight from a to b (in the plane the road sees) with a handrail on its road side."""
    tilt_box(m, col, a, b, w, 14)
    tilt_box(m, rail, (a[0], a[1] - w / 2.0, a[2] + 95), (b[0], b[1] - w / 2.0, b[2] + 95), 6, 6)


def awning(m, x, y_wall, z, w, depth, c1=RED, c2=WHITE, n=6):
    """A flat canopy off a wall that looks at the road, a striped valance on its edge, two stays."""
    m.box(c1, (x, y_wall - depth / 2.0, z), (w, depth, 10))
    for k in range(n):
        m.box(c1 if k % 2 == 0 else c2, (x - w / 2.0 + w * (k + 0.5) / n, y_wall - depth + 4, z - 20), (w / float(n), 8, 34))
    for s in (-1, 1):
        tilt_box(m, STEEL, (x + s * (w / 2.0 - 8), y_wall, z - 70), (x + s * (w / 2.0 - 8), y_wall - depth + 8, z - 6), 6, 6)


def container(m, col, x, y, z, L=600.0, D=245.0, H=255.0, along='x', ribs=3, rib=WHITE):
    """A freight container, the middle of its floor at (x, y, z). along 'x': a long side looks at the road (a few
    thick ribs for the corrugation); along 'y': its doors do (two door bars and a sill)."""
    if along == 'x':
        m.box(col, (x, y, z + H / 2.0), (L, D, H))
        for k in range(ribs):
            m.box(rib, (x - L / 2.0 + L * (k + 0.5) / ribs, y - D / 2.0 - 3, z + H / 2.0), (16, 8, H - 30))
    else:
        m.box(col, (x, y, z + H / 2.0), (D, L, H))
        for s in (-1, 1):
            m.box(rib, (x + s * D * 0.4, y - L / 2.0 - 3, z + H / 2.0), (10, 8, H - 40))
        m.box(rib, (x, y - L / 2.0 - 3, z + 12), (D - 10, 8, 14))


def drum(m, col, x, y, z, band=WHITE, r=29.0, h=86.0, lying=None):
    """A drum, standing on (x, y, z) or lying along 'x' / 'y' with its middle there. Blue or white, never orange."""
    if lying == 'y':
        py(m, col, x, y - h / 2.0, y + h / 2.0, z + r, r, steps=8)
        py(m, band, x, y - 6, y + 6, z + r, r + 2, steps=8)
    elif lying == 'x':
        px(m, col, x - h / 2.0, x + h / 2.0, y, z + r, r, steps=8)
        px(m, band, x - 6, x + 6, y, z + r, r + 2, steps=8)
    else:
        m.cyl(col, (x, y, z), r, h, steps=8)
        m.cyl(band, (x, y, z + h * 0.42), r + 2, 12, steps=8)


def puff(sc, x, y, z, r, rate=0.16, phase=0.0, lift=14.0):
    """A ball of steam standing over a vent, slowly rising and settling."""
    s = sc.part(P.motion(bob=-lift, rate=rate, phase=phase))
    s.sphere(STEAM, (x, y, z), r, steps=(4, 6))


# ------------------------------------------------------------------------------------------ buildings (the street: 26 to 70 m)
def container_hotel(sc):
    """BUILDING. The container hotel, 11.8 x 7.9 m, 10.2 m to its washing line: twelve boxes three high and four wide,
    each its own colour and each with a lit (or dark) end window, a red door under a striped awning, the numeral 5,
    fire-escape walkways and two flights across the front, a roof terrace with a tank, vents and a washing line."""
    g = sc.still()
    g.box(CONC, (0, 0, -135), (1140, 690, 330))                       # plinth: 3 m of footing, top at z = 30
    g.box(PAVE, (0, -30, 36), (1180, 760, 12))
    cols = ((YEL, BLUE, RED, WHITE), (BLUE, WHITE, YEL, RED), (RED, YEL, BLUE, STEEL))
    lit = ((1, 0, 1, 1), (1, 1, 0, 1), (0, 1, 1, 0))
    for k in range(3):
        for i in range(4):
            x = -390 + i * 260
            y = (-40 if (k + i) % 2 else 0)
            z = 42 + k * 262
            c = cols[k][i]
            container(g, c, x, y, z, along='y', rib=STEEL if c == WHITE else WHITE)
            yf = y - 300
            if k == 0 and i == 1:
                g.box(RED, (x, yf - 5, z + 105), (120, 10, 210))                      # the door
                g.box(WARM, (x, yf - 8, z + 172), (60, 10, 40), glow=0.5)
            elif k == 2 and i == 3:
                number(g, WHITE, '5', x, z + 128, 170, yf)
            else:
                window(g, x, z + 142, 116, 88, yf, lit=0.5 if lit[k][i] else 0.0, frame=STEEL if c == WHITE else WHITE)
    awning(g, -130, -340, 258, 300, 90, RED, WHITE, 6)
    yw = -395                                                          # the fire escape across the front
    for k in (1, 2):
        z = 36 + k * 262
        g.box(STEEL, (0, yw, z), (1100, 80, 10))
        rail_x(g, YEL, -550, 550, yw - 38, z + 5, 95, 5)
    flight(g, STEEL, (-560, yw, 40), (-330, yw, 296), 80)
    flight(g, STEEL, (560, yw, 300), (330, yw, 558), 80)
    zr = 828                                                           # the roof terrace
    rail_x(g, YEL, -510, 510, -330, zr, 90, 6)
    g.cyl(BLUE, (330, 120, zr + 50), 75, 130, steps=8)
    g.cyl(WHITE, (330, 120, zr + 180), 80, 14, steps=8)
    for sx, sy in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
        g.box(STEEL, (330 + sx * 50, 120 + sy * 50, zr + 25), (10, 10, 50))
    for x in (-60, 120):
        g.cyl(STEEL, (x, 160, zr), 22, 60, steps=6)
        g.cyl(RED, (x, 160, zr + 60), 34, 26, steps=6, top_r=8)
    for x in (-430, 30):
        g.box(STEEL, (x, 40, zr + 90), (7, 7, 180))
    g.box(STEEL, (-200, 40, zr + 172), (460, 5, 5))
    w = sc.part(P.motion(swing=7, pivot=(-200, 40, zr + 172), axis=0, rate=0.33, phase=0.2))
    for x, wd, h, c in ((-380, 70, 100, WHITE), (-290, 60, 70, RED), (-210, 80, 110, BLUE), (-110, 60, 80, WHITE), (-30, 70, 96, YEL)):
        w.box(c, (x, 40, zr + 170 - h / 2.0), (wd, 5, h))


def signal_cabin(sc):
    """BUILDING. The signal cabin, 10.2 x 5.4 m, 14 m to its vane: a glazed white box with a red barrel roof on a yellow
    steel tower, a balcony all round with a hazard-striped edge, the numeral 2 on a blue board, a zigzag stair at one
    end, a steady red and a white lamp on a bracket, a stovepipe, and a vane that turns slowly."""
    g = sc.still()
    g.box(CONC, (0, 0, -140), (640, 540, 320))                         # footing, top at z = 20
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, YEL, (sx * 250, sy * 200, 10), (sx * 190, sy * 150, 800), 28, 28)
    for sy in (-1, 1):
        tilt_box(g, STEEL, (-240, sy * 196, 60), (185, sy * 152, 770), 10, 10)
        tilt_box(g, STEEL, (240, sy * 196, 60), (-185, sy * 152, 770), 10, 10)
        g.box(YEL, (0, sy * 176, 410), (450, 14, 14))
    for sx in (-1, 1):
        g.box(YEL, (sx * 220, 0, 410), (14, 350, 14))
    g.box(STEEL, (0, 0, 806), (620, 500, 24))                          # the balcony
    hazard(g, -310, 310, 776, 836, -250)
    rail_x(g, YEL, -305, 305, -245, 818, 95, 5)
    rail_x(g, YEL, -305, 305, 245, 818, 95, 3)
    for sx in (-1, 1):
        g.box(YEL, (sx * 305, 0, 913), (6, 490, 6))
    g.box(WHITE, (0, 20, 978), (440, 340, 320))                        # the cabin: z 818 to 1138
    g.box(BLUE, (0, 20, 850), (446, 346, 64))
    g.box(GLASS, (0, -152, 1015), (404, 8, 150))
    for x in (-150, -50, 50, 150):
        g.box(WARM, (x, -156, 1015), (82, 8, 132), glow=0.5)
    for sx in (-1, 1):
        g.box(GLASS, (sx * 222, 20, 1015), (8, 260, 150))
        g.box(WARM, (sx * 225, 20, 1015), (8, 110, 132), glow=0.4)
    g.box(RED, (0, 20, 1144), (500, 400, 14))
    barrel_x(g, RED, -250, 250, 20, 1150, 400, 90)
    g.cyl(BLACK, (150, 110, 1150), 12, 170, steps=6)
    g.cyl(BLACK, (150, 110, 1320), 20, 12, steps=6)
    g.box(STEEL, (-150, 20, 1310), (6, 6, 150))
    v = sc.part(P.motion(swing=30, pivot=(-150, 20, 1380), axis=P.UP, rate=0.18, phase=0.4))
    v.box(WHITE, (-140, 20, 1380), (100, 5, 14))
    v.box(RED, (-195, 20, 1380), (30, 5, 44))
    g.box(BLUE, (0, -214, 470), (230, 10, 210))                        # the cabin's number
    number(g, WHITE, '2', 0, 470, 150, -219)
    g.box(STEEL, (270, -190, 880), (70, 8, 8))                         # the lamp bracket
    g.box(RED, (300, -190, 905), (28, 28, 28), glow=0.7)
    g.box(WHITE, (300, -190, 860), (24, 24, 24), glow=0.5)
    flight(g, STEEL, (650, -100, 20), (340, -100, 410), 90)            # the stair: up, a landing, back, a bridge to the balcony
    g.box(STEEL, (310, -20, 410), (110, 260, 12))
    flight(g, STEEL, (340, 60, 416), (650, 60, 800), 90)
    g.box(STEEL, (500, 60, 806), (400, 100, 12))
    g.box(STEEL, (680, 60, 400), (12, 12, 800))
    rail_x(g, YEL, 320, 700, 12, 812, 95, 3)


def yard_office(sc):
    """BUILDING. The yard office, 8.8 x 7.3 m, 18.2 m to its mast: four storeys stacked a little out of line, yellow,
    white, blue and yellow, steel floor slabs between, lit and dark windows, a zigzag stair across the whole front, a
    huge white 3 on the top storey, a red door under a lamp, hazard stripes at the foot, a roof tank and a pennant."""
    g = sc.still()
    g.box(CONC, (0, 0, -140), (800, 640, 320))                         # footing, top at z = 20
    cols = (YEL, WHITE, BLUE, YEL)
    off = (0, 40, -30, 20)
    for k in range(4):
        z0 = 20 + k * 380
        x0 = off[k]
        g.box(cols[k], (x0, 0, z0 + 180), (720, 580, 360))
        g.box(STEEL, (x0, 0, z0 + 370), (790, 650, 20))
        yf = -290
        if k == 3:
            g.box(BLUE, (x0 + 150, yf - 4, z0 + 180), (300, 8, 300))
            number(g, WHITE, '3', x0 + 150, z0 + 180, 250, yf - 8)
            window(g, x0 - 190, z0 + 200, 150, 130, yf, lit=0.45, frame=STEEL)
        else:
            for i, x in enumerate((-220, 0, 220)):
                if k == 0 and i == 1:
                    g.box(RED, (x0 + x, yf - 5, z0 + 115), (130, 10, 230))
                    g.box(WARM, (x0 + x, yf - 12, z0 + 262), (40, 14, 20), glow=0.7)
                else:
                    window(g, x0 + x, z0 + 200, 130, 130, yf, lit=0.45 if (k + i) % 3 else 0.0, frame=STEEL if cols[k] == WHITE else WHITE)
    hazard(g, -360, 360, 24, 84, -292)
    ys = -350                                                          # the zigzag stair, in the plane the road sees
    for k in range(3):
        z0 = 30 + k * 380
        a, b = ((-340, ys, z0), (340, ys, z0 + 380)) if k % 2 == 0 else ((340, ys, z0), (-340, ys, z0 + 380))
        flight(g, STEEL, a, b, 90)
        g.box(STEEL, (b[0] + (40 if b[0] > 0 else -40), ys, z0 + 380), (120, 110, 12))
    for sx in (-1, 1):
        g.box(STEEL, (sx * 410, ys, 600), (10, 10, 1160))
    zr = 1540                                                          # the roof
    for sy in (-1, 1):
        g.box(RED, (20, sy * 300, zr + 25), (760, 20, 50))
    for sx in (-1, 1):
        g.box(RED, (20 + sx * 370, 0, zr + 25), (20, 600, 50))
    g.cyl(STEEL, (-180, 100, zr + 60), 90, 140, steps=8)
    g.cyl(BLUE, (-180, 100, zr + 200), 96, 16, steps=8)
    for sx, sy in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
        g.box(STEEL, (-180 + sx * 60, 100 + sy * 60, zr + 30), (10, 10, 60))
    g.box(STEEL, (230, 150, zr + 140), (7, 7, 280))
    f = sc.part(P.motion(swing=14, pivot=(230, 150, zr + 250), axis=P.UP, rate=0.3, phase=0.1))
    f.box(RED, (285, 150, zr + 250), (110, 5, 44))
    f.box(WHITE, (285, 150, zr + 250), (112, 6, 14))


def wagon_cafe(sc):
    """BUILDING (small). The drivers' cafe, 13.2 x 4.5 m, 6.3 m to its steam: a blue railway coach on a plinth, on
    its own rails, a white barrel roof, a lit serving hatch with a counter under a red and white awning, a row of
    steady bulbs, four stools, lit windows, a door with steps, a blank menu board, the coach's number 8, a puff of steam."""
    g = sc.still()
    g.box(CONC, (0, 0, -110), (1300, 430, 380))                        # plinth, top at z = 80
    g.box(PAVE, (0, -10, 84), (1320, 450, 8))
    for sy in (-22, 122):
        g.box(STEEL, (0, sy, 96), (1240, 10, 14))
    for k in range(7):
        g.box(RUST, (-540 + k * 180, 50, 88), (26, 200, 10))
    g.box(BLACK, (0, 50, 150), (1100, 230, 30))
    for x in (-400, -280, 280, 400):
        py(g, BLACK, x, -40, 140, 135, 44, steps=8)
    g.box(BLUE, (0, 50, 290), (1120, 280, 250))                        # the coach: z 165 to 415
    g.box(YEL, (0, -92, 180), (1124, 6, 22))
    barrel_x(g, WHITE, -575, 575, 50, 415, 300, 62)
    yf = -90
    g.box(WHITE, (-180, yf - 5, 318), (380, 10, 130))                  # the serving hatch
    g.box(WARM, (-180, yf - 8, 318), (350, 10, 104), glow=0.5)
    g.box(WHITE, (-180, yf - 34, 250), (390, 60, 10))
    awning(g, -180, yf, 404, 440, 120, RED, WHITE, 8)
    for k in range(6):
        g.box(WARM, (-380 + k * 80, yf - 120, 372), (10, 10, 10), glow=0.7)
    for x in (-330, -230, -130, -30):
        g.cyl(STEEL, (x, -150, 88), 8, 62, steps=6)
        g.cyl(RED, (x, -150, 150), 20, 10, steps=6)
    for x in (180, 330):
        window(g, x, 322, 90, 90, yf, lit=0.45)
    number(g, WHITE, '8', 255, 232, 60, yf)
    g.box(RED, (470, yf - 5, 280), (80, 10, 200))                      # the door and its steps
    g.box(STEEL, (470, yf - 40, 140), (110, 70, 12))
    g.box(STEEL, (470, yf - 80, 108), (110, 50, 12))
    g.box(STEEL, (585, -150, 150), (8, 8, 130))                        # the menu: blank coloured sheets
    g.box(WHITE, (585, -152, 250), (90, 8, 120))
    for i, c in enumerate((RED, BLUE, YEL)):
        g.box(c, (585, -158, 285 - i * 34), (66, 6, 18))
    g.cyl(BLACK, (-470, 60, 470), 12, 80, steps=6)
    g.cyl(BLACK, (-470, 60, 550), 20, 10, steps=6)
    puff(sc, -470, 60, 600, 34, rate=0.17, phase=0.3, lift=10.0)


def weigh_house(sc):
    """BUILDING (small). The weigh house, 7.2 x 4.6 m, 6.2 m: a white cabin under a red barrel roof whose whole front is
    a dial (a red pointer swinging slowly over yellow marks), a blue door and a lit window, the weighbridge plate with
    a hazard edge before it, three great weights, a sack on a hook that sways, a stovepipe, the numeral 1."""
    g = sc.still()
    g.box(CONC, (0, 0, -130), (720, 440, 340))                         # footing, top at z = 40
    g.box(WHITE, (-80, 60, 220), (480, 300, 360))                      # the cabin: z 40 to 400
    g.box(BLUE, (-80, 60, 70), (486, 306, 60))
    barrel_y(g, RED, -80, -110, 230, 400, 520, 150)
    yf = -90
    zc = 330
    py(g, BLUE, -80, yf - 8, yf, zc, 150, steps=12)                    # the dial
    py(g, WHITE, -80, yf - 14, yf - 6, zc, 128, steps=12)
    for k in range(9):
        a = math.radians(20 + k * 17.5)
        g.box(YEL, (-80 + math.cos(a) * 104, yf - 18, zc + math.sin(a) * 104), (14, 8, 14))
    p = sc.part(P.motion(swing=38, pivot=(-80, yf - 22, zc), axis=90, rate=0.11, phase=0.15))
    p.box(RED, (-80, yf - 22, zc + 50), (12, 8, 116))
    py(g, BLACK, -80, yf - 30, yf - 20, zc, 16, steps=6)
    g.box(BLUE, (-240, yf - 5, 130), (90, 10, 180))                    # door, window
    window(g, 80, 150, 80, 80, yf, lit=0.5, frame=STEEL)
    g.box(STEEL, (-60, -170, 46), (560, 110, 12))                      # the weighbridge plate
    hazard(g, -340, 220, 40, 60, -226)
    for i, (x, r, h) in enumerate(((255, 50, 90), (255, 40, 70), (255, 30, 50))):   # three weights, one on another
        z = 40 + sum((90, 70, 50)[:i])
        g.cyl(STEEL if i % 2 == 0 else BLUE_D, (x, -120, z), r, h, steps=8, top_r=r * 0.82)
    g.cyl(STEEL, (255, -120, 250), 12, 26, steps=6)
    number(g, WHITE, '1', 255, 300, 70, 30)
    g.box(BLUE, (255, 34, 300), (110, 8, 110))
    g.box(STEEL, (255, 40, 180), (10, 10, 280))
    g.box(STEEL, (200, 0, 395), (150, 8, 8))                           # the sack scale's bracket
    s = sc.part(P.motion(swing=5, pivot=(270, 0, 392), axis=90, rate=0.3, phase=0.6))
    s.box(BLACK, (270, 0, 352), (5, 5, 80))
    s.box(WHITE, (270, 0, 270), (56, 44, 86))
    g.cyl(BLACK, (-230, 120, 480), 12, 130, steps=6)
    g.cyl(BLACK, (-230, 120, 610), 20, 10, steps=6)


# ------------------------------------------------------------------------------------------ buildings (the depots: 80 m and more)
def customs_shed(sc):
    """BUILDING. The customs shed, 19.8 x 12.9 m, 9.7 m: a yellow hall under a red barrel roof with white ribs, a red
    sliding door half open on blue and white crates, the numeral 4 and an arrow to the door, a row of small lit
    windows, hazard stripes along the foot, a loading dock with striped bollards, roof vents, a blue lean-to office
    with a stovepipe, a lamp over the door, and a plain pennant that swings."""
    g = sc.still()
    L, D, H = 1600.0, 1100.0, 600.0
    g.box(YEL, (-160, 0, 150), (L, D, 900))                            # the hall: 3 m of footing, eaves at 6 m
    barrel_x(g, RED, -160 - L / 2.0 - 30, -160 + L / 2.0 + 30, 0, H, D + 60, 300, n=10)
    for k in range(4):
        x = -160 - L / 2.0 + 120 + k * (L - 240) / 3.0
        barrel_x(g, WHITE, x - 18, x + 18, 0, H + 2, D + 70, 306, n=10)
    yf = -D / 2.0
    g.box(NAVY, (-420, yf - 3, 250), (520, 8, 440))                    # the doorway, the door half across it
    g.box(RED, (-560, yf - 12, 250), (300, 14, 440))
    for k in range(3):
        g.box(WHITE, (-650 + k * 90, yf - 20, 250), (14, 8, 400))
    g.box(STEEL, (-420, yf - 14, 484), (700, 16, 16))
    g.box(BLUE, (-330, yf - 70, 110), (150, 110, 120))                 # crates in the doorway, on the dock
    g.box(WHITE, (-310, yf - 66, 215), (110, 100, 90))
    g.box(WHITE, (-215, yf - 60, 100), (90, 90, 100))
    g.box(WARM, (-420, yf - 22, 520), (60, 30, 22), glow=0.7)
    number(g, WHITE, '4', 330, 330, 300, yf)
    arrow(g, WHITE, 30, 330, 220, yf, to_left=False)
    for k in range(5):
        window(g, -60 + k * 150, 530, 70, 60, yf, lit=0.45 if k != 2 else 0.0)
    hazard(g, -960, 640, 20, 110, yf - 1)
    g.box(CONC_D, (-160, yf - 80, -100), (L + 80, 160, 300))           # the dock, 0.5 m high
    for x in (-900, -160, 580):
        g.cyl(YEL, (x, yf - 140, 50), 16, 80, steps=6)
        g.cyl(BLACK, (x, yf - 140, 130), 18, 16, steps=6)
    for x in (-600, 200):
        g.cyl(STEEL, (x, 0, H + 280), 40, 60, steps=6)
        g.cyl(STEEL, (x, 0, H + 340), 70, 34, steps=6, top_r=14)
    g.box(BLUE, (800, -150, 10), (320, 520, 620))                      # the lean-to office: z -300 to 320
    g.box(RED, (800, -150, 328), (360, 560, 16))
    window(g, 800, 200, 120, 90, -410, lit=0.5)
    g.cyl(BLACK, (880, -40, 336), 12, 150, steps=6)
    g.box(STEEL, (-940, -200, H + 220), (7, 7, 300))
    f = sc.part(P.motion(swing=12, pivot=(-940, -200, H + 330), axis=P.UP, rate=0.28, phase=0.5))
    f.box(WHITE, (-880, -200, H + 330), (120, 5, 50))
    f.box(RED, (-880, -200, H + 330), (122, 6, 16))


def engine_shed(sc):
    """BUILDING. The engine shed, 19.8 x 16.4 m, 14.7 m to its steam: a yellow hall of two arched roads under twin red barrel
    roofs whose arcs face the road, white arch rings, the numerals 1 and 2 on blue plates, an engine's smokebox and
    red buffer beam showing in one road (a steady lamp) and a red door half shut on the other, rails and sleepers out
    of both, a clerestory along each ridge, a white smoke vent between the roofs with two slow balls of steam, a lit
    window in the pier, hazard stripes, a coal bunker and a vane."""
    g = sc.still()
    W, D, H = 1800.0, 1300.0, 620.0
    g.box(YEL, (0, 0, 160), (W, D, 920))                               # the hall: 3 m of footing, eaves at 6.2 m
    yf = -D / 2.0
    for sx in (-1, 1):
        cx = sx * 450
        barrel_y(g, RED, cx, yf - 20, D / 2.0 + 20, H, 900, 300, n=10)
        g.box(STEEL, (cx, 0, H + 310), (120, 900, 50))
        g.box(GLASS, (cx, 0, H + 312), (126, 860, 26))
        barrel_y(g, WHITE, cx, yf - 8, yf, 320, 600, 290, n=10)        # the arch ring, the dark road inside it
        g.box(WHITE, (cx, yf - 4, 165), (600, 8, 330))
        barrel_y(g, NAVY, cx, yf - 14, yf - 6, 330, 500, 250, n=10)
        g.box(NAVY, (cx, yf - 10, 170), (500, 8, 330))
        for sy in (-72, 72):
            g.box(STEEL, (cx + sy, yf - 160, 10), (12, 330, 14))
        for k in range(3):
            g.box(RUST, (cx, yf - 60 - k * 110, 4), (230, 26, 10))
        g.box(BLUE, (cx, yf - 24, 770), (190, 8, 190))
        number(g, WHITE, '1' if sx > 0 else '2', cx, 770, 140, yf - 28)
    cx = 450                                                           # the engine in road 1 (the viewer's left)
    py(g, BLACK, cx, yf - 60, yf - 12, 240, 140, steps=10)
    py(g, STEEL, cx, yf - 66, yf - 58, 240, 90, steps=10)
    g.box(RED, (cx, yf - 70, 95), (380, 30, 50))
    for s in (-1, 1):
        py(g, STEEL, cx + s * 130, yf - 110, yf - 84, 95, 26, steps=8)
    g.box(WARM, (cx, yf - 74, 240), (40, 16, 40), glow=0.7)
    g.cyl(BLACK, (cx, yf - 36, 370), 30, 90, steps=8, top_r=40)
    cx = -450                                                          # road 2: the door half shut
    g.box(RED, (cx + 125, yf - 20, 200), (250, 12, 400))
    for k in range(2):
        g.box(WHITE, (cx + 65 + k * 120, yf - 28, 200), (14, 8, 360))
    window(g, 0, 300, 80, 170, yf, lit=0.45)                           # the pier between the roads
    hazard(g, -110, 110, 20, 90, yf - 1)
    g.cyl(WHITE, (0, 0, H), 80, 560, steps=8)                          # the smoke vent
    g.cyl(RED, (0, 0, H + 560), 120, 50, steps=8, top_r=40)
    puff(sc, 0, 0, H + 700, 70, rate=0.13, phase=0.0, lift=30.0)
    puff(sc, 40, 20, H + 800, 46, rate=0.13, phase=0.35, lift=40.0)
    g.box(BLACK, (990, -300, -40), (180, 380, 520))                    # the coal bunker
    g.box(NAVY, (990, -300, 232), (150, 340, 30))
    g.box(STEEL, (-820, 300, H + 200), (7, 7, 300))
    v = sc.part(P.motion(swing=26, pivot=(-820, 300, H + 330), axis=P.UP, rate=0.2, phase=0.7))
    v.box(WHITE, (-800, 300, H + 330), (140, 5, 14))
    v.box(RED, (-880, 300, H + 330), (40, 5, 50))


def box_tower(sc):
    """BUILDING. The box tower, 9.9 x 7 m, 15.1 m to its mast: nine containers stacked crosswise five high, each its own
    colour, the numerals 6, 0 and 7 across their sides, hazard stripes along the bottom pair, a ladder up the front, a
    lookout box on top with a lit band of windows and two steady lamps, a yellow jib with a white crate swinging
    slowly on its hook, a pennant."""
    g = sc.still()
    g.box(CONC, (0, 0, -135), (700, 660, 330))                         # slab: 3 m of footing, top at z = 30
    z = 30
    container(g, YEL, 0, -128, z, along='x', rib=BLACK)                # level 0: two side by side
    container(g, BLUE, 0, 128, z, along='x')
    hazard(g, -290, 290, z + 8, z + 68, -252)
    number(g, BLACK, '6', 150, z + 150, 150, -251)
    z += 262
    container(g, RED, -165, 0, z, along='y')                           # level 1: two across
    container(g, WHITE, 165, 0, z, along='y', rib=STEEL)
    window(g, 165, z + 140, 110, 80, -300, lit=0.45, frame=STEEL)
    z += 262
    container(g, BLUE, 30, -128, z, along='x')                         # level 2: two side by side, a little along
    container(g, STEEL, 30, 128, z, along='x')
    number(g, WHITE, '0', -100, z + 128, 170, -251)
    z += 262
    container(g, YEL, -60, 0, z, along='x', rib=BLACK)                 # level 3: one, and one across that overhangs
    container(g, RED, 330, -60, z, along='y')
    number(g, BLACK, '7', -200, z + 128, 170, -123)
    arrow(g, BLACK, 10, z + 128, 180, -123, to_left=False)
    z += 262
    g.box(WHITE, (-60, 0, z + 110), (420, 250, 220))                   # the lookout
    g.box(GLASS, (-60, -127, z + 130), (380, 8, 90))
    for x in (-180, -60, 60):
        g.box(WARM, (x, -131, z + 130), (96, 8, 74), glow=0.5)
    g.box(RED, (-60, 0, z + 226), (470, 300, 14))
    for sx in (-1, 1):
        g.box(WARM, (-60 + sx * 220, -140, z + 245), (24, 24, 24), glow=0.7)
    ladder(g, STEEL, -262, -266, 40, z + 10, step=110)
    g.box(STEEL, (-200, 60, z + 330), (7, 7, 200))
    f = sc.part(P.motion(swing=14, pivot=(-200, 60, z + 400), axis=P.UP, rate=0.3, phase=0.2))
    f.box(RED, (-145, 60, z + 400), (110, 5, 44))
    tilt_box(g, YEL, (120, 0, z + 233), (560, 0, z + 420), 30, 30)     # the jib
    tilt_box(g, STEEL, (120, 0, z + 420), (560, 0, z + 420), 8, 8)
    g.box(YEL, (120, 0, z + 330), (20, 20, 190))
    h = sc.part(P.motion(swing=4, pivot=(560, 0, z + 420), axis=0, rate=0.14, phase=0.4))
    h.box(BLACK, (560, 0, z + 260), (6, 6, 320))
    h.box(STEEL, (560, 0, z + 96), (30, 30, 30))
    h.box(WHITE, (560, 0, z + 20), (130, 130, 120))
    h.box(BLUE, (560, 0, z + 20), (134, 30, 124))


# ------------------------------------------------------------------------------------------ props
def _pad(g, w, d, col=PAVE):
    g.box(col, (0, 0, -24), (w, d, 60))                                # a paving pad: top at z = 6, half a metre of footing


def pallet_stack(sc):
    """PROP, LOW (1.9 m). Crates on two pallets, blue and white, one stack under a white tarpaulin roped down, a crate
    stencilled 2, a sack leaning on the pile. Unlit, nothing moves."""
    g = sc.still()
    _pad(g, 440, 240)
    for x in (-110, 110):
        g.box(RUST, (x, 0, 14), (190, 170, 16))
        for sy in (-60, 0, 60):
            g.box(RUST, (x, sy, 3), (190, 20, 6))
    g.box(BLUE, (-150, -30, 62), (90, 90, 80))
    g.box(WHITE, (-60, -20, 57), (80, 100, 70))
    g.box(WHITE, (-150, 50, 52), (90, 60, 60))
    g.box(BLUE, (-110, -20, 142), (110, 90, 80))
    number(g, WHITE, '2', -110, 142, 46, -65)
    g.box(WHITE, (110, 0, 100), (200, 180, 156))                       # the tarpaulin over the second stack
    g.box(STEEL, (110, 0, 181), (150, 130, 8))
    for x in (60, 160):
        g.box(BLACK, (x, -92, 100), (6, 5, 160))
    g.box(BLACK, (110, -92, 60), (204, 5, 6))
    tilt_box(g, WHITE, (-5, -70, 22), (20, -40, 108), 50, 30)


def sack_trucks(sc):
    """PROP, LOW (1.5 m). Four sack trucks leaning on a steel rail and a platform trolley with a yellow handle, three
    white sacks and a blue crate on it. Unlit, nothing moves."""
    g = sc.still()
    _pad(g, 460, 200)
    g.box(STEEL, (-110, 40, 110), (300, 7, 7))
    for x in (-255, 35):
        g.box(STEEL, (x, 40, 58), (8, 8, 104))
    for k in range(4):
        x = -220 + k * 74
        for s in (-1, 1):
            tilt_box(g, RED if k % 2 else BLUE, (x + s * 16, -40, 22), (x + s * 16, 34, 150), 6, 6)
            py(g, BLACK, x + s * 22, -52, -44, 20, 14, steps=6)
        g.box(STEEL, (x, -52, 10), (44, 30, 5))
        g.box(STEEL, (x, 0, 90), (38, 5, 6))
    g.box(STEEL, (130, -10, 32), (150, 90, 8))                         # the trolley
    for sx in (-1, 1):
        for sy in (-1, 1):
            py(g, BLACK, 130 + sx * 56, -10 + sy * 34 - 4, -10 + sy * 34 + 4, 16, 12, steps=6)
        g.box(YEL, (205, -10 + sx * 38, 78), (6, 6, 92))
    g.box(YEL, (205, -10, 124), (6, 82, 6))
    g.sphere(WHITE, (100, -24, 54), 26, scale=(1.3, 0.9, 0.7), steps=(4, 6))
    g.sphere(WHITE, (104, 10, 54), 26, scale=(1.3, 0.9, 0.7), steps=(4, 6))
    g.sphere(WHITE, (102, -8, 88), 24, scale=(1.3, 0.9, 0.7), steps=(4, 6))
    g.box(BLUE, (160, -10, 62), (50, 60, 52))


def loading_gauge(sc):
    """PROP, TALL (5.9 m). The loading gauge: a yellow and black hoop frame over a short siding that runs at the road,
    the white gauge bar hung from it on two chains and swinging a little, a blue plate with the numeral 4, striped feet."""
    g = sc.still()
    g.box(CONC_D, (0, 0, -22), (560, 520, 56))                         # ballast, top at z = 6
    for sx in (-72, 72):
        g.box(STEEL, (sx, 0, 14), (12, 500, 14))
    for k in range(5):
        g.box(RUST, (0, -200 + k * 100, 8), (230, 26, 10))
    pts = [(-230, 0), (-230, 380), (-150, 520), (0, 580), (150, 520), (230, 380), (230, 0)]
    for i in range(len(pts) - 1):
        (x0, z0), (x1, z1) = pts[i], pts[i + 1]
        tilt_box(g, YEL if i % 2 == 0 else BLACK, (x0, 0, z0), (x1, 0, z1), 18, 18)
    for sx in (-1, 1):
        g.box(BLACK, (sx * 230, 0, 40), (30, 30, 80))
        g.box(WHITE, (sx * 230, 0, 100), (30, 30, 40))
    g.box(BLUE, (310, -6, 300), (110, 8, 130))
    g.box(STEEL, (270, 0, 300), (60, 6, 6))
    number(g, WHITE, '4', 310, 300, 90, -10)
    b = sc.part(P.motion(swing=5, pivot=(0, 0, 560), axis=0, rate=0.25, phase=0.3))
    for sx in (-1, 1):
        b.box(BLACK, (sx * 90, 0, 500), (5, 5, 110))
        tilt_box(b, WHITE, (sx * 170, 0, 410), (sx * 80, 0, 450), 10, 10)
    b.box(WHITE, (0, 0, 450), (164, 10, 10))


def water_crane(sc):
    """PROP, TALL (5 m). A water crane for the engines: a blue column on a white drum, a steel arm with a counterweight,
    a rust-brown leather bag hanging from the arm and swaying, a yellow hand wheel, a drain grate, and a brazier with a
    steady warm glow to keep it from freezing."""
    g = sc.still()
    _pad(g, 300, 220, CONC_D)
    g.cyl(WHITE, (0, 0, 6), 46, 50, steps=8)
    g.cyl(BLUE, (0, 0, 56), 24, 380, steps=8, top_r=18)
    g.cyl(WHITE, (0, 0, 436), 30, 20, steps=8)
    g.cyl(BLUE, (0, 0, 456), 14, 40, steps=6, top_r=5)
    px(g, STEEL, -80, 250, 0, 400, 14, steps=8)
    g.box(BLACK, (-100, 0, 400), (50, 44, 44))
    py(g, YEL, 0, -40, -32, 200, 30, steps=8)
    g.box(STEEL, (0, -30, 200), (8, 20, 8))
    g.box(STEEL, (120, -40, 8), (110, 80, 6))
    s = sc.part(P.motion(swing=7, pivot=(240, 0, 392), axis=90, rate=0.28, phase=0.2))
    s.cyl(RUST, (240, 0, 240), 14, 150, steps=6, top_r=17)
    s.box(BLACK, (240, 0, 300), (38, 38, 8))
    g.cyl(BLACK, (-95, -50, 6), 6, 40, steps=5)                        # the brazier
    g.cyl(BLACK, (-95, -50, 46), 26, 44, steps=8, top_r=30)
    g.box(WARM, (-95, -50, 90), (36, 36, 8), glow=0.6)


def buffer_stop(sc):
    """PROP, LOW (1.9 m). The end of a siding: rails on sleepers, two raked struts, a red and white striped beam with two
    steel buffers, a steady red lamp on a short post, a white stone with the numeral 0."""
    g = sc.still()
    g.box(CONC_D, (0, 0, -22), (400, 230, 56))
    for sy in (-72, 72):
        g.box(STEEL, (40, sy, 14), (320, 12, 14))
        tilt_box(g, RUST, (60, sy, 16), (-110, sy, 120), 14, 14)
        g.box(RUST, (-110, sy, 70), (14, 14, 120))
        px(g, STEEL, -84, -50, sy, 110, 20, steps=8)
    for k in range(4):
        g.box(RUST, (-80 + k * 90, 0, 8), (26, 210, 10))
    for k in range(5):
        g.box(RED if k % 2 == 0 else WHITE, (-110, -88 + k * 44, 110), (30, 44, 44))
    g.box(BLACK, (-150, 0, 80), (10, 10, 160))
    g.box(RED, (-150, 0, 172), (26, 26, 26), glow=0.7)
    g.box(WHITE, (150, -96, 30), (50, 12, 50))
    number(g, BLACK, '0', 150, 30, 34, -102)


def forklift_bay(sc):
    """PROP, TALL (3.1 m). A forklift parked with its forks down under a canopy: four steel posts and a red roof with a
    hazard valance, the truck in blue with a white overhead guard and a steel mast, black wheels, a white crate on a
    pallet beside it, a blue charger box with a cable, a hazard edge to the pad. Nothing moves, nothing is lit."""
    g = sc.still()
    _pad(g, 520, 300)
    hazard(g, -260, 260, 6, 22, -150)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL, (sx * 235, sy * 125, 150), (10, 10, 290))
    g.box(RED, (0, 0, 302), (540, 320, 14))
    hazard(g, -270, 270, 262, 296, -160)
    x0 = -60                                                           # the truck, side on, its mast at the +X end
    g.box(BLUE, (x0, 0, 75), (170, 100, 70))
    g.box(BLACK, (x0 - 100, 0, 80), (40, 104, 84))
    g.box(BLUE_D, (x0 - 30, 0, 126), (60, 60, 34))
    for sx in (-55, 60):
        for sy in (-1, 1):
            py(g, BLACK, x0 + sx, sy * 50 - 9, sy * 50 + 9, 34, 28, steps=8)
    for sx in (-80, 50):
        for sy in (-1, 1):
            g.box(WHITE, (x0 + sx, sy * 44, 160), (7, 7, 110))
    g.box(WHITE, (x0 - 15, 0, 216), (150, 100, 8))
    for sy in (-1, 1):
        g.box(STEEL, (x0 + 100, sy * 30, 125), (12, 12, 230))
        g.box(STEEL, (x0 + 150, sy * 28, 10), (100, 10, 6))
    g.box(STEEL, (x0 + 104, 0, 60), (10, 90, 60))
    g.box(RUST, (190, 40, 14), (110, 110, 14))
    g.box(WHITE, (190, 40, 66), (90, 90, 90))
    g.box(BLUE, (-220, 110, 110), (40, 30, 70))
    g.box(STEEL, (-220, 110, 40), (8, 8, 70))
    tilt_box(g, BLACK, (-200, 100, 90), (-150, 40, 60), 5, 5)


def numeral_board(sc):
    """PROP, TALL (6.4 m). A yard board on two steel posts: a blue panel with the white numerals 47, a red panel with a
    white arrow under it, hazard stripes at the feet, braces, and a white tally plate hung below that swings a little."""
    g = sc.still()
    g.box(CONC_D, (0, 0, -22), (420, 90, 56))
    for sx in (-1, 1):
        g.box(STEEL, (sx * 170, 0, 320), (14, 14, 630))
        tilt_box(g, STEEL, (sx * 170, 40, 10), (sx * 170, 4, 260), 8, 8)
        g.box(YEL, (sx * 170, -1, 50), (20, 18, 80))
        g.box(BLACK, (sx * 170, -1, 110), (20, 18, 40))
    g.box(BLUE, (0, -8, 520), (380, 10, 230))
    g.box(WHITE, (0, -10, 520), (360, 8, 210))
    g.box(BLUE, (0, -12, 520), (344, 8, 194))
    number(g, WHITE, '47', 0, 520, 150, -16)
    g.box(RED, (0, -8, 350), (380, 10, 90))
    arrow(g, WHITE, 0, 350, 240, -13, to_left=True, t=30)
    t = sc.part(P.motion(swing=6, pivot=(0, -8, 300), axis=0, rate=0.3, phase=0.5))
    for sx in (-1, 1):
        t.box(BLACK, (sx * 50, -8, 280), (5, 5, 44))
    t.box(WHITE, (0, -8, 225), (150, 6, 70))
    t.box(YEL, (0, -12, 225), (110, 6, 16))


def semaphore(sc):
    """PROP, TALL (9 m). A semaphore signal: a white post with black and red bands and a finial, a red arm with a
    white stripe that rises and falls slowly, a yellow arm below it out of step, a steady red lamp, a balance weight,
    a little railed platform, a ladder, and a blue relay box with the numeral 3."""
    g = sc.still()
    g.box(CONC_D, (0, 0, -22), (200, 200, 56))
    g.box(WHITE, (0, 0, 420), (26, 26, 830))
    g.box(BLACK, (0, 0, 60), (30, 30, 110))
    g.box(RED, (0, 0, 480), (30, 30, 50))
    g.cyl(WHITE, (0, 0, 835), 20, 60, steps=4, top_r=4)
    ladder(g, STEEL, 0, 22, 20, 640, step=100)
    g.box(STEEL, (0, 0, 640), (130, 110, 8))
    rail_x(g, STEEL, -65, 65, -52, 644, 80, 2)
    g.box(RED, (-24, -18, 770), (26, 22, 26), glow=0.7)
    g.box(BLACK, (-24, -18, 792), (30, 26, 8))
    a = sc.part(P.motion(swing=20, pivot=(0, -22, 770), axis=90, rate=0.07, phase=0.0))
    a.box(RED, (95, -22, 770), (200, 8, 34))
    a.box(WHITE, (150, -27, 770), (24, 6, 34))
    a.box(STEEL, (-30, -22, 770), (40, 8, 20))
    b = sc.part(P.motion(swing=20, pivot=(0, -22, 690), axis=90, rate=0.07, phase=0.5))
    b.box(YEL, (90, -22, 690), (190, 8, 30))
    b.box(BLACK, (140, -27, 690), (22, 6, 30))
    g.box(STEEL, (0, 30, 200), (40, 10, 70))
    g.box(BLACK, (0, 30, 150), (50, 16, 30))
    g.box(BLUE, (70, 0, 60), (60, 44, 108))
    g.box(STEEL, (70, 0, 118), (70, 54, 8))
    number(g, WHITE, '3', 70, 66, 50, -22)


def tally_tower(sc):
    """PROP, TALL (7.8 m). The checker's tower: a red and white striped hut under a red barrel roof on a yellow pedestal
    with four struts, a railed deck, a lit window, a blue door, a ladder, hazard stripes at the foot, a steel horn,
    and a striped wind sock that swings on its mast."""
    g = sc.still()
    g.box(CONC_D, (0, 0, -22), (260, 240, 56))
    g.box(YEL, (0, 0, 195), (60, 60, 380))
    for zb in (40, 120):
        g.box(BLACK, (0, 0, zb), (66, 66, 40))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, YEL, (sx * 26, sy * 26, 240), (sx * 120, sy * 100, 380), 10, 10)
    g.box(STEEL, (0, 0, 386), (290, 250, 14))
    rail_x(g, YEL, -142, 142, -122, 393, 90, 3)
    rail_x(g, YEL, -142, 142, 122, 393, 90, 2)
    for k in range(5):
        g.box(RED if k % 2 == 0 else WHITE, (-80 + k * 40, 20, 493), (40, 170, 200))
    window(g, 20, 510, 80, 70, -65, lit=0.5, frame=STEEL)
    g.box(BLUE, (-70, -68, 478), (50, 8, 160))
    barrel_y(g, RED, 0, -80, 120, 593, 230, 70, n=6)
    ladder(g, STEEL, -110, -128, 10, 390, step=95)
    g.cyl(STEEL, (95, -70, 640), 8, 50, rot=(0, 0, 90), steps=6, top_r=26)
    g.box(STEEL, (60, 80, 700), (6, 6, 150))
    w = sc.part(P.motion(swing=12, pivot=(60, 80, 760), axis=P.UP, rate=0.25, phase=0.3))
    for k in range(3):
        w.cyl(RED if k % 2 == 0 else WHITE, (64 + k * 34, 80, 760), 16 - k * 3, 34, rot=(-90, 0, 0), steps=6, top_r=13 - k * 3)


def drum_dock(sc):
    """PROP, LOW (1.8 m). A low dock of blue and white drums: three standing on the dock, two lying in a steel cradle,
    one on the ground, two steps, a striped bollard, the numeral 5 on the dock's face. Unlit, nothing moves."""
    g = sc.still()
    _pad(g, 460, 240)
    g.box(CONC, (-70, 30, 50), (280, 170, 88))                         # the dock: top at z = 94
    g.box(YEL, (-70, -57, 86), (280, 6, 14))
    number(g, WHITE, '5', -150, 46, 56, -56)
    drum(g, BLUE, -160, 50, 94)
    drum(g, WHITE, -95, 60, 94, band=BLUE)
    drum(g, BLUE, -30, 40, 94)
    g.box(STEEL, (40, -30, 30), (70, 60, 8))                           # two steps
    g.box(STEEL, (40, 20, 62), (70, 50, 8))
    for sy in (-34, 34):
        g.box(STEEL, (160, sy, 12), (130, 8, 12))
    drum(g, WHITE, 128, 0, 16, band=BLUE, lying='y')
    drum(g, BLUE, 192, 0, 16, lying='y')
    drum(g, BLUE, 160, 0, 68, lying='y')
    drum(g, WHITE, 60, -80, 6, band=BLUE)
    g.cyl(YEL, (-200, -90, 6), 12, 70, steps=6)
    g.cyl(BLACK, (-200, -90, 76), 14, 14, steps=6)


# ------------------------------------------------------------------------------------------ the hero
def great_scales(sc):
    """HERO. THE GREAT SCALES, 79.6 m across its pans (the beam 70 m) and 58.1 m tall: a yellow A-frame tower (two A's, banded white, tied and
    braced in steel) on a stepped plinth with a hazard-striped face, a red beam 70 m long across its head that tilts
    slowly a few degrees and back (ONE swing part: the beam, its pointer, the chains, both pans and what lies in
    them), a blue tank wagon in one pan and six striped containers in the other, a white scale plate under the pivot
    that the red pointer sweeps, the numerals 5 and 0 on blue boards, the weighmaster's lit cabin hung in the frame, a
    stair up to it, four steady lamps on the plinth, a red cap and a vane. The wagon carries nothing."""
    g = sc.still()
    g.box(CONC, (0, 0, -100), (2700, 1500, 600))                       # plinth: 4 m of footing, top at z = 200
    g.box(PAVE, (0, 0, 250), (2300, 1200, 100))
    hazard(g, -1300, 1300, 30, 180, -751)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(STEEL, (sx * 1250, sy * 650, 290), (14, 14, 180))
            g.box(WARM, (sx * 1250, sy * 650, 396), (40, 40, 36), glow=0.6)
    ZP = 5250.0                                                        # the pivot
    for sy in (-350, 350):                                             # the two A's
        for sx in (-1, 1):
            tilt_box(g, YEL, (sx * 1100, sy, 300), (sx * 150, sy, 5200), 150, 150)
            for k in range(5):
                t = (k + 0.5) / 5.0
                g.box(WHITE, (sx * (1100 - 950 * t), sy, 300 + 4900 * t), (236, 162, 70))
            g.box(BLACK, (sx * 1100, sy, 340), (300, 220, 90))
        g.box(YEL, (0, sy, 1800), (1680, 120, 130))
        g.box(YEL, (0, sy, 3600), (980, 110, 120))
        tilt_box(g, STEEL, (-800, sy, 1870), (440, sy, 3540), 40, 40)
        tilt_box(g, STEEL, (800, sy, 1870), (-440, sy, 3540), 40, 40)
    for z, hx in ((1800, 809), (3600, 460)):
        for sx in (-1, 1):
            g.box(STEEL, (sx * hx, 0, z), (90, 700, 90))
    g.box(YEL, (0, 0, 5300), (520, 900, 420))                          # the head: z 5090 to 5510
    hazard(g, -260, 260, 5430, 5510, -451)
    py(g, STEEL, 0, -500, 500, ZP, 90, steps=10)
    g.cyl(RED, (0, 0, 5510), 420, 200, steps=4, top_r=30, rot=(0, 45, 0))
    g.box(STEEL, (0, 0, 5740), (10, 10, 120))
    v = sc.part(P.motion(swing=24, pivot=(0, 0, 5770), axis=P.UP, rate=0.1, phase=0.3))
    v.box(WHITE, (40, 0, 5770), (260, 8, 24))
    v.box(RED, (-110, 0, 5770), (60, 8, 80))
    # the scale plate under the pivot and its marks (the pointer is part of the beam)
    g.box(WHITE, (0, -456, 4480), (760, 12, 300))
    g.box(BLUE, (0, -460, 4610), (760, 10, 40))
    for sx in (-1, 1):
        g.box(STEEL, (sx * 300, -438, 4480), (30, 30, 30))
    for k in range(7):
        g.box(RED if k == 3 else BLACK, (-270 + k * 90, -464, 4410 + (30 if k == 3 else 0)), (22, 8, 120 if k != 3 else 180))
    # the numerals, hung on the lower tie
    for sx, ch in ((1, '5'), (-1, '0')):
        g.box(BLUE, (sx * 330, -416, 1800), (520, 14, 640))
        g.box(WHITE, (sx * 330, -424, 1800), (480, 8, 600))
        g.box(BLUE, (sx * 330, -428, 1800), (450, 8, 570))
        number(g, WHITE, ch, sx * 330, 1800, 440, -432)
    # the weighmaster's cabin on the upper tie, and the stair to it
    g.box(WHITE, (0, -350, 3800), (420, 360, 280))
    g.box(RED, (0, -350, 3948), (480, 420, 20))
    barrel_x(g, RED, -240, 240, -350, 3956, 420, 80)
    for x in (-120, 0, 120):
        g.box(WARM, (x, -534, 3820), (90, 8, 130), glow=0.6)
    g.box(STEEL, (0, -440, 3655), (1000, 200, 14))
    rail_x(g, STEEL, -500, 500, -536, 3662, 110, 6)
    flight(g, STEEL, (-1090, -480, 320), (-800, -480, 1900), 110, YEL)  # the ladderway up the leg, a landing half way
    g.box(STEEL, (-790, -480, 1900), (200, 130, 14))
    flight(g, STEEL, (-800, -480, 1906), (-470, -480, 3655), 110, YEL)
    # ---- what tilts: one swing part about the pivot, the axis across the road's line
    b = sc.part(P.motion(swing=3.2, pivot=(0, 0, ZP), axis=90, rate=0.055, phase=0.0))
    b.box(RED, (0, 0, ZP), (7000, 170, 250))
    b.box(RED, (0, 0, ZP), (3200, 180, 420))
    for sx in (-1, 1):
        for k in range(3):
            b.box(WHITE, (sx * (2300 + k * 420), 0, ZP), (120, 178, 258))
        b.box(YEL, (sx * 3400, 0, ZP - 60), (220, 260, 330))
        b.box(BLACK, (sx * 3400, 0, ZP - 240), (120, 120, 60))
    b.box(YEL, (900, 0, ZP + 190), (260, 230, 130))                    # the rider weight on the beam
    b.box(RED, (0, -478, ZP - 350), (50, 26, 700))                     # the pointer
    side(b, RED, [(-70, ZP - 700), (70, ZP - 700), (0, ZP - 820)], -491, -465)
    ZC, ZD = ZP - 270, 2760.0                                          # chains: from the beam's ends to the pans' corners
    for sx in (-1, 1):
        xe = sx * 3400
        for cx in (-1, 1):
            for cy in (-1, 1):
                tilt_box(b, BLACK, (xe, 0, ZC), (xe + cx * 500, cy * 250, ZD), 16, 16)
        b.box(STEEL, (xe, 0, ZD - 20), (1160, 620, 40))                # the pan
        for cy in (-1, 1):
            b.box(YEL, (xe, cy * 300, ZD + 20), (1160, 20, 60))
        for cx in (-1, 1):
            b.box(YEL, (xe + cx * 570, 0, ZD + 20), (20, 600, 60))
    # the tank wagon, in the pan on the viewer's left (+X): it carries nothing
    xw, z0 = 3400.0, ZD
    for sy in (-72, 72):
        b.box(STEEL, (xw, sy, z0 + 8), (900, 14, 16))
    for dx in (-300, -190, 190, 300):
        py(b, BLACK, xw + dx, -86, 86, z0 + 62, 46, steps=8)
    b.box(BLACK, (xw, 0, z0 + 122), (860, 190, 34))
    for dx in (-250, 250):
        b.box(BLACK, (xw + dx, 0, z0 + 165), (60, 200, 60))
    px(b, BLUE, xw - 400, xw + 400, 0, z0 + 300, 150, steps=12)
    for dx in (-390, -130, 130, 390):
        px(b, WHITE, xw + dx - 12, xw + dx + 12, 0, z0 + 300, 156, steps=12)
    b.cyl(STEEL, (xw, 0, z0 + 440), 50, 50, steps=8)
    b.cyl(WHITE, (xw, 0, z0 + 490), 60, 14, steps=8)
    for s in (-1, 1):
        b.box(RED, (xw + s * 445, 0, z0 + 122), (30, 240, 44))
    # six striped containers, three high and two deep, in the other pan
    xc = -3400.0
    front = (YEL, RED, WHITE)
    back = (BLUE, WHITE, YEL)
    for k in range(3):
        z = ZD + k * 262
        container(b, front[k], xc + (40 if k == 1 else 0), -128, z, along='x', ribs=2, rib=BLACK if front[k] != RED else WHITE)
        b.box(back[k], (xc - (30 if k == 1 else 0), 128, z + 127.5), (600, 245, 255))
        dx = xc + (40 if k == 1 else 0)
        sc_ = BLUE if front[k] == WHITE else (WHITE if front[k] == RED else BLACK)
        for j in (-1, 0, 1):                                           # diagonal stripes on the side the road sees
            x0 = dx + j * 170 - 100
            side(b, sc_, [(x0, z + 40), (x0 + 60, z + 40), (x0 + 200, z + 215), (x0 + 140, z + 215)], -260, -250)


JOBS = [
    ('SM_S1D3_ContainerHotel', container_hotel, dict(grow=(20, 30, 20), grow_down=(20, 30, 0))),
    ('SM_S1D3_SignalCabin', signal_cabin, dict(grow=(40, 40, 10), grow_down=(40, 40, 0))),
    ('SM_S1D3_YardOffice', yard_office, dict(grow=(60, 60, 10), grow_down=(60, 60, 0))),
    ('SM_S1D3_WagonCafe', wagon_cafe, dict(grow=(0, 0, 20), grow_down=(0, 0, 0))),
    ('SM_S1D3_WeighHouse', weigh_house, dict(grow=(30, 20, 10), grow_down=(30, 20, 0))),
    ('SM_S1D3_CustomsShed', customs_shed, dict(grow=(60, 60, 10), grow_down=(60, 60, 0))),
    ('SM_S1D3_EngineShed', engine_shed, dict(grow=(60, 60, 60), grow_down=(60, 60, 0))),
    ('SM_S1D3_BoxTower', box_tower, dict(grow=(60, 60, 10), grow_down=(60, 60, 0))),
    ('SM_S1D3_PalletStack', pallet_stack, STILL),
    ('SM_S1D3_SackTrucks', sack_trucks, STILL),
    ('SM_S1D3_LoadingGauge', loading_gauge, dict(grow=(10, 40, 0), grow_down=(10, 40, 0))),
    ('SM_S1D3_WaterCrane', water_crane, dict(grow=(30, 10, 0), grow_down=(30, 10, 0))),
    ('SM_S1D3_BufferStop', buffer_stop, STILL),
    ('SM_S1D3_ForkliftBay', forklift_bay, STILL),
    ('SM_S1D3_NumeralBoard', numeral_board, dict(grow=(0, 30, 0), grow_down=(0, 30, 0))),
    ('SM_S1D3_Semaphore', semaphore, dict(grow=(40, 0, 80), grow_down=(40, 0, 0))),
    ('SM_S1D3_TallyTower', tally_tower, dict(grow=(60, 60, 0), grow_down=(60, 60, 0))),
    ('SM_S1D3_DrumDock', drum_dock, STILL),
    ('SM_S1D3_GreatScales', great_scales, dict(far=True, grow=(260, 80, 260), grow_down=(260, 80, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
