"""Theme Park, Stage 1, builder s1d3: district 3 "The Lagoon" (3.21 to 6.9074 km). /Game/World/ThemePark/SM_S1D3_*.
Run: import ft_world_park_s1d3 as K; K.run()

WATER PAVILIONS ON TALL PILES. The road is the coaster here and the land lies 26 m under the rails, so every building
is a slim stem carrying a pavilion at the rails' height (deck 27 m up), under a flared pagoda roof, a bell roof or a
glass jellyfish bell. Palette: WHITE pavilions, LAV roofs, POOL stems, CYAN glass and lamps, PLUM piles and shadow.

What the track's own rule asks of these pieces (FTTrack.cpp, RoadSpan and Clear, read on 16 Nov 2026): a coaster's
"road" is 38.1 m wide either side of the centre line and reaches from 14.4 m under the rails to 32 m over them. Any
piece that comes up into that height is pushed out to 39.6 m, and one that stands 2 m over it to 48 m. So:
  - the pieces at the rails' height (five near buildings, the channel marker, the lantern line, the high jetty) are
    SLIM across (6.4 m and less, front to back) and are placed 61 to 63 m out: their nearest face is 56 m or more out;
  - two bigger buildings and the fish vane stand 96 m out (never inside 70 m: the shadow rule is a mesh's);
  - three things stand on the valley FLOOR 27 to 38 m out and are under 9.5 m tall, to be looked down on from the rails;
  - two low, unlit props (under 1.9 m) are for Splashdown's roadside, where the road is back on the land.
The hero, THE SEAHORSE CASCADE, is a 62 m wedding-cake waterfall of five shell basins in a pool of its own, sheets of
water going round and falling from every basin, a rearing seahorse on top (saved with the always-moving master).

Night level: every tall piece carries a small steady glow (lift(), as the park's other kits do) and steady lamps,
all of it 27 m or more out; nothing blinks. No figures (the people are s1people's), no clown, no balloon, no words.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'
WHITE, LAV, POOL, CYAN, PLUM = '#fff6ea', '#b9a4ff', '#3a5fa8', '#2ee6ff', '#4a3a78'
NAVY, GOLD, BULB, MINT, MAG = '#2b2350', '#ffc93c', '#ffe28a', '#7fe3c4', '#ff4fb8'
WOOD, WOOD_D, SPRAY, WATER = '#9a6a44', '#6a4c36', '#dff6ff', '#1c5f8a'
DARK = (PLUM, NAVY, WOOD_D)
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))          # no moving part: true bounds
SLIM = dict(grow=(40, 0, 40), grow_down=(40, 0, 0))        # a piece at the rails' height: no growth towards the road
D = 2700.0        # a near pavilion's deck: 27 m over the valley floor, the height of the rails
D2 = 2750.0       # the far ones (the land has fallen a little more 96 m out)


# ------------------------------------------------------------------------------------------ helpers
def lift(sc, g):
    """A steady glow on every colour of the piece (the level is lit by night), a third of it on the dark colours."""
    for key in sc.order:
        model, mo = sc.parts[key]
        if not hasattr(model, 'meshes'):
            continue
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g / 3.0 if col in DARK else g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.1):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def cone(m, col, a, b, r0, r1=None, steps=8, glow=0.0):
    """A cylinder or a cone from a to b (rod()'s own turn, with a second radius)."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    L = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, L, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def footing(g, x, y, r=90.0, deep=300.0):
    g.cyl(PLUM, (x, y, -deep), r, deep + 60.0, steps=8, top_r=r * 0.82)


def pile(g, x, y, top, r=46.0, col=POOL, bands=(WHITE, LAV), step=620.0, steps=8):
    """A round stem from the valley floor to `top`, banded, on a plum footing 3 m deep."""
    footing(g, x, y, r * 1.9)
    g.cyl(col, (x, y, 0), r, top, steps=steps, top_r=r * 0.8)
    k, z = 0, 420.0
    while z < top - 300.0:
        g.cyl(bands[k % len(bands)], (x, y, z), r * (1.0 - 0.2 * z / top) + 7.0, 56.0, steps=steps)
        k += 1
        z += step


def post(g, x, y, top, w=44.0, col=PLUM, band=WHITE, bands=(900.0, 1800.0), deep=300.0):
    """A square pile (a trestle's leg), banded."""
    g.box(col, (x, y, (top - deep) / 2.0), (w, w, top + deep))
    for z in bands:
        g.box(band, (x, y, z), (w + 12.0, w + 12.0, 70.0))


def capital(g, x, y, top, r0, r1, h=260.0, col=WHITE, steps=8):
    """The flare under a deck: a stem opens like a lily."""
    g.cyl(col, (x, y, top - h), r0, h, steps=steps, top_r=r1)


def pagoda(g, x, y, z, R, col=LAV, trim=WHITE, steps=8, tips=True, finial=CYAN):
    """A flared roof: a lip, a shallow skirt, a steep cone, a collar, a spire and a ball. Returns the spire's top."""
    g.cyl(trim, (x, y, z), R, 20.0, steps=steps, top_r=R * 0.985)
    z1 = z + 20.0
    g.cyl(col, (x, y, z1), R * 0.97, R * 0.2, steps=steps, top_r=R * 0.6)
    z2 = z1 + R * 0.2
    g.cyl(col, (x, y, z2), R * 0.6, R * 0.46, steps=steps, top_r=R * 0.16)
    z3 = z2 + R * 0.46
    g.cyl(trim, (x, y, z3), R * 0.2, R * 0.08, steps=steps, top_r=R * 0.13)
    z4 = z3 + R * 0.08
    g.cyl(trim, (x, y, z4), 7.0 + R * 0.012, R * 0.3, steps=5, top_r=4.0)
    g.sphere(finial, (x, y, z4 + R * 0.12), max(R * 0.07, 9.0), steps=(3, 6), glow=0.6)
    if tips:
        n = steps // 2 if steps > 4 else steps
        for k in range(n):
            a = math.radians(k * 360.0 / n)
            ca, sa = math.cos(a), math.sin(a)
            tilt_box(g, trim, (x + ca * R * 0.86, y + sa * R * 0.86, z + 14.0), (x + ca * R * 1.1, y + sa * R * 1.1, z + 22.0 + R * 0.12), R * 0.07 + 6.0, 10.0)
    return z4 + R * 0.3


def bell_roof(g, x, z, half, y0, y1, H, col=LAV):
    """A bell roof along X (an ogee gable with upturned eaves), from y0 to y1."""
    K = [(-1.0, -0.05), (-1.11, 0.06), (-0.83, 0.11), (-0.58, 0.37), (-0.4, 0.7), (-0.21, 0.92), (0.0, 1.0),
         (0.21, 0.92), (0.4, 0.7), (0.58, 0.37), (0.83, 0.11), (1.11, 0.06), (1.0, -0.05)]
    g.side(col, [(x + half * a, z + H * b) for a, b in K], y0, y1)


def lamp(g, x, y, z, s=22.0, col=BULB, glow=1.3):
    g.box(col, (x, y, z), (s, s, s * 1.25), glow=glow)


def lamp_row(g, a, b, n, s=18.0, cols=(BULB,), glow=1.3):
    for k in range(n):
        f = (k + 0.5) / n
        lamp(g, a[0] + (b[0] - a[0]) * f, a[1] + (b[1] - a[1]) * f, a[2] + (b[2] - a[2]) * f, s, cols[k % len(cols)], glow)


def hang_lamp(g, x, y, z, glow=1.3, col=BULB):
    """A small square lantern hanging from z: a cord, a lavender cap, the light, a foot."""
    g.box(PLUM, (x, y, z - 16.0), (5.0, 5.0, 32.0))
    g.box(LAV, (x, y, z - 34.0), (36.0, 36.0, 8.0))
    g.box(col, (x, y, z - 56.0), (26.0, 26.0, 36.0), glow=glow)


def rail(g, x0, x1, y, z, h=90.0, col=WHITE, gap=160.0):
    g.box(col, ((x0 + x1) / 2.0, y, z + h), (x1 - x0, 8.0, 8.0))
    n = max(2, int(round((x1 - x0) / gap)) + 1)
    for k in range(n):
        g.box(col, (x0 + (x1 - x0) * k / (n - 1.0), y, z + h / 2.0), (8.0, 8.0, h))


def windows(g, xs, y, z, w=70.0, h=120.0, col=CYAN, glow=0.9, frame=None):
    for x in xs:
        if frame:
            g.box(frame, (x, y + 3.0, z), (w + 18.0, 6.0, h + 18.0))
        g.box(col, (x, y, z), (w, 8.0, h), glow=glow)


def pennant(sc, x, y, z, col=LAV, L=130.0, h=60.0, phase=0.0, swing=18.0):
    """A pennant turning to and fro on its staff (give the staff yourself)."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=0.35, phase=phase))
    f.box(col, (x + L / 2.0, y, z), (L, 5.0, h))
    f.box(WHITE, (x + L * 0.84, y, z), (L * 0.2, 6.0, h * 0.5))


def ring_box(g, col, r, z, size, ang, glow=0.0):
    """A box on a circle round the piece's middle, its long side along the circle."""
    a = math.radians(ang)
    g.box(col, (math.cos(a) * r, math.sin(a) * r, z), size, rot=(0, ang + 90.0, 0), glow=glow)


# ------------------------------------------------------------------------------------------ near buildings (61 to 63 m)
def boathouse(sc):
    """BUILDING. A boathouse on four braced piles: plank deck 27 m up, a white house with a slatted boat door under a
    striped awning, three lit windows, a lavender bell roof with a little pagoda cupola and a pennant; a rowing boat
    hangs from a davit off the end and rocks. 13.5 x 5.6 m, 36 m tall."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            post(g, sx * 420, sy * 190, D - 30, w=46, bands=(700.0, 1500.0, 2300.0))
    for sy in (-1, 1):
        for k in range(3):
            z0, z1 = 200 + k * 800, 200 + (k + 1) * 800
            tilt_box(g, WHITE, (-420 if k % 2 == 0 else 420, sy * 190, z0), (420 if k % 2 == 0 else -420, sy * 190, z1), 16, 16)
    g.box(PLUM, (0, 0, D - 55), (980, 430, 50))
    g.box(WHITE, (0, 0, D - 15), (1160, 560, 30))
    g.box(LAV, (0, -281, D - 15), (1160, 6, 34))
    g.box(WHITE, (-60, 20, D + 150), (820, 420, 300))                 # the house: x -470..350, y -190..230
    g.box(POOL, (-60, 20, D + 20), (830, 430, 40))
    g.box(PLUM, (-230, -192, D + 120), (280, 8, 220))                 # the boat door, slatted
    for k in range(4):
        g.box(LAV, (-335 + k * 70, -197, D + 120), (8, 6, 220))
    windows(g, (40, 150, 260), -191, D + 170, 70, 120, CYAN, 0.9, frame=LAV)
    for k in range(6):                                                # a striped awning over the door
        x = -355 + k * 50
        tilt_box(g, LAV if k % 2 == 0 else WHITE, (x, -190, D + 266), (x, -274, D + 230), 50, 6)
    bell_roof(g, -60, D + 300, 450, -215, 255, 250)
    g.box(WHITE, (-60, -217, D + 306), (880, 6, 26))
    lamp_row(g, (-440, -224, D + 284), (320, -224, D + 284), 7)
    g.box(WHITE, (-60, 20, D + 575), (120, 120, 70))                  # the cupola
    top = pagoda(g, -60, 20, D + 610, 120)
    pennant(sc, -60, 20, top - 22, LAV, 110, 46)
    cone(g, WHITE, (-430, -232, D + 175), (-430, -216, D + 175), 30, 30, 8)      # a life ring on the wall
    cone(g, LAV, (-430, -238, D + 175), (-430, -220, D + 175), 14, 14, 6)
    for k in range(2):                                                # oars leaning by the door
        x = 318 + k * 22
        tilt_box(g, WOOD, (x, -206, D + 4), (x, -196, D + 236), 7, 7)
        g.box(LAV if k else CYAN, (x, -196, D + 262), (22, 7, 56))
    g.box(POOL, (-520, -236, D + 22), (80, 56, 44))                   # a planter of reeds on the deck's corner
    g.sphere(MINT, (-520, -236, D + 62), 36, scale=(1.0, 0.7, 0.7), steps=(3, 6))
    for k in range(3):
        g.box(MINT, (-540 + k * 20, -236, D + 100), (6, 6, 90))
    g.box(PLUM, (540, -120, D + 130), (16, 16, 260))                  # the davit
    g.box(PLUM, (625, -120, D + 255), (190, 14, 14))
    b = sc.part(P.motion(swing=4.0, pivot=(645, -120, D + 255), axis=90.0, rate=0.22, phase=0.2))
    b.side(LAV, [(535, D - 300), (575, D - 362), (715, D - 362), (760, D - 300)], -172, -68)
    b.box(WHITE, (647, -120, D - 298), (228, 112, 8))
    b.box(PLUM, (647, -120, D - 290), (20, 100, 10))
    for x in (590, 705):
        b.box(WHITE, (x, -120, D - 22), (6, 6, 552))


def jelly_tea(sc):
    """BUILDING. The jellyfish tea room: one blue stem that opens like a lily under a round lavender dish, and on the
    dish a glass bell (cyan, softly lit) with five lavender ribs, a band of warm lit windows and a balcony of lamp
    posts; a fringe of ribbons sways under the dish's rim, lily-leaf landings step up the stem, a teapot for a
    weather vane. 6 m across, 33 m tall."""
    g = sc.still()
    T = D - 30
    pile(g, 0, 0, T - 240, r=62)
    capital(g, 0, 0, T, 50, 285, h=250, col=WHITE, steps=10)
    g.cyl(LAV, (0, 0, T), 300, 30, steps=10)
    for k, (a, z) in enumerate(((200, 800), (20, 1400), (160, 2000))):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        g.cyl(MINT if k % 2 else LAV, (ca * 130, sa * 60, z), 130, 12, steps=8)
    Z = D
    for r0, r1, h in ((250, 236, 110), (236, 188, 100), (188, 112, 78), (112, 30, 44)):
        g.cyl(CYAN, (0, 0, Z), r0, h, steps=10, top_r=r1, glow=0.4)
        Z += h
    g.cyl(WHITE, (0, 0, Z), 40, 30, steps=8, top_r=26)
    g.cyl(WHITE, (0, 0, Z + 30), 8, 150, steps=5, top_r=5)
    prof = [(254, 0), (240, 110), (192, 210), (116, 288), (36, 332)]
    for k in range(5):
        a = math.radians(k * 72 - 90)
        ca, sa = math.cos(a), math.sin(a)
        for (ra, za), (rb, zb) in zip(prof, prof[1:]):
            tilt_box(g, LAV, (ca * ra, sa * ra, D + za), (ca * rb, sa * rb, D + zb), 22, 10)
    for k in range(10):
        ring_box(g, BULB, 246, D + 56, (58, 10, 70), k * 36 - 72, glow=0.9)
    for k in range(10):
        a = math.radians(k * 36 - 90)
        ca, sa = math.cos(a), math.sin(a)
        g.box(WHITE, (ca * 290, sa * 290, D + 42), (8, 8, 84))
        if k % 2 == 0:
            lamp(g, ca * 290, sa * 290, D + 96, 18)
    for k in range(10):                                               # the fringe under the rim
        ang = k * 36 - 72
        a = math.radians(ang)
        ca, sa = math.cos(a), math.sin(a)
        f = sc.part(P.motion(swing=7.0, pivot=(ca * 296, sa * 296, T), axis=ang + 90.0, rate=0.3, phase=(k * 0.37) % 1.0))
        f.box(LAV if k % 2 else WHITE, (ca * 296, sa * 296, T - 42), (44, 6, 84), rot=(0, ang + 90.0, 0))
    v = sc.part(P.motion(swing=24.0, pivot=(0, 0, Z + 170), axis=P.UP, rate=0.13, phase=0.4))      # the teapot vane
    v.box(GOLD, (0, 0, Z + 190), (74, 34, 48))
    v.box(GOLD, (0, 0, Z + 220), (30, 20, 14))
    tilt_box(v, GOLD, (34, 0, Z + 182), (78, 0, Z + 214), 12, 12)
    v.box(GOLD, (-48, 0, Z + 192), (12, 10, 34))


def dive_tower(sc):
    """BUILDING. The diving tower: a slim white frame round a blue ladder core, three platforms at 24, 29 and 34 m
    each with a lavender springboard out along the road (they wobble), a look-out cabin under a pagoda roof on top,
    towels on a line under the middle platform. 10.7 x 3.3 m, 38 m tall."""
    g = sc.still()
    H = 3380.0
    g.box(PLUM, (0, 0, -120), (330, 330, 360))
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WHITE, (sx * 105, sy * 105, (H - 300) / 2.0), (30, 30, H + 300))
    g.box(POOL, (0, 0, H / 2.0), (70, 70, H))
    for k in range(7):
        g.box(LAV if k % 2 else WHITE, (0, 0, 320 + k * 300), (250, 250, 24))
    for k in range(7):
        z0, z1 = 332 + k * 300, 620 + k * 300
        tilt_box(g, WHITE, (-105 if k % 2 == 0 else 105, -112, z0), (105 if k % 2 == 0 else -105, -112, z1), 12, 12)
    for z, sx in ((2380.0, 1), (2880.0, -1), (3380.0, 1)):
        g.box(WHITE, (0, 0, z), (330, 300, 22))
        rail(g, -165, 165, -146, z + 11, 90, LAV, gap=165.0)
        g.box(PLUM, (sx * 200, 0, z + 12), (60, 90, 24))
        b = sc.part(P.motion(swing=2.5, pivot=(sx * 165, 0, z + 24), axis=90.0, rate=0.7, phase=(z * 0.0013) % 1.0))
        b.box(LAV, (sx * 345, 0, z + 30), (380, 76, 12))
        b.box(CYAN, (sx * 500, 0, z + 38), (60, 78, 6))
        lamp(g, -sx * 150, -146, z + 116, 18)
    t = sc.part(P.motion(swing=8.0, pivot=(0, -154, 2866), axis=0.0, rate=0.5, phase=0.1))          # towels on a line
    for k, c in enumerate((LAV, CYAN, WHITE, MAG)):
        t.box(c, (-108 + k * 72, -154, 2824), (52, 5, 84))
    g.box(WHITE, (0, 0, H + 96), (200, 200, 170))                     # the look-out cabin
    windows(g, (-50, 50), -101, H + 110, 56, 80, CYAN, 0.9)
    top = pagoda(g, 0, 0, H + 181, 190)
    pennant(sc, 0, 0, top - 26, LAV, 120, 50, phase=0.6)


def shell_stand(sc):
    """BUILDING. The shell bandstand: a round stage on a ribbed stem, a scallop shell of seven white and lavender
    ribs standing up behind it with a lamp at every tip, footlights, a gold harp, two conch horns, three music
    stands, lanterns hanging under the rim. 9.9 x 6 m, 33 m tall."""
    g = sc.still()
    T = D - 30
    pile(g, 0, 40, T - 220, r=66, bands=(LAV, WHITE))
    for ang in (90, 210, 330):
        ca, sa = math.cos(math.radians(ang)), math.sin(math.radians(ang))
        tilt_box(g, WHITE, (ca * 70, 40 + sa * 70, T - 900), (ca * 240, 30 + sa * 200, T - 20), 26, 26)
    capital(g, 0, 40, T, 60, 250, h=220, col=LAV, steps=10)
    g.cyl(WHITE, (0, 0, T), 300, 30, steps=10)
    g.cyl(LAV, (0, 0, T - 14), 306, 14, steps=10)
    base = (0, 250, D + 10)                                           # the fan lies in one plane (y = 250)
    for k in range(7):                                                # the shell
        a = math.radians(-66 + k * 22)
        tip = (math.sin(a) * 540, 250, D + 10 + math.cos(a) * 540)
        tilt_box(g, WHITE if k % 2 == 0 else LAV, base, tip, 26, 170)
        lamp(g, math.sin(a) * 556, 250, D + 10 + math.cos(a) * 556, 22)
    cone(g, LAV, (0, 205, D + 30), (0, 262, D + 30), 120, 120, 8)     # the hinge
    lamp_row(g, (-200, -270, D + 12), (200, -270, D + 12), 5, s=16)
    for k in range(6):
        a = math.radians(k * 60 - 60)
        hang_lamp(g, math.cos(a) * 288, math.sin(a) * 288, T - 14)
    g.box(GOLD, (-40, 60, D + 115), (12, 12, 230))                    # the harp
    tilt_box(g, GOLD, (-40, 60, D + 230), (90, 60, D + 190), 12, 12)
    tilt_box(g, GOLD, (90, 60, D + 190), (-30, 60, D + 8), 14, 14)
    for k in range(4):
        g.box(WHITE, (-18 + k * 22, 60, D + 122 - k * 12), (5, 5, 190 - k * 30))
    for sx in (-1, 1):                                                # conch horns, mouths to the road
        g.box(PLUM, (sx * 190, 100, D + 70), (10, 10, 140))
        cone(g, LAV, (sx * 190, 130, D + 150), (sx * 190, 20, D + 150), 14, 62, 8)
        g.box(PLUM, (sx * 110, -110, D + 55), (6, 6, 110))            # music stands
        g.box(WHITE, (sx * 110, -114, D + 118), (40, 6, 30))
    g.box(PLUM, (0, -130, D + 55), (6, 6, 110))
    g.box(WHITE, (0, -134, D + 118), (40, 6, 30))
    pennant(sc, -500, 250, D + 560, LAV, 100, 44, phase=0.3)
    pennant(sc, 500, 250, D + 560, CYAN, 100, 44, phase=0.8)
    g.box(WHITE, (-500, 250, D + 500), (8, 8, 150))
    g.box(WHITE, (500, 250, D + 500), (8, 8, 150))


def pearl_station(sc):
    """BUILDING. The pearl diver's station: a deck on two braced stems, a white cabin whose cupola is a brass diving
    helmet (three lit portholes, a knob, an air hose down to a pump), a giant clam with its lid ajar and a pearl in
    it, a hoist off the end with a glass diving bell that goes slowly up and down, spare helmets on a rack.
    11.8 x 5.4 m, 34 m tall."""
    g = sc.still()
    T = D - 30
    for sx in (-1, 1):
        pile(g, sx * 300, 0, T, r=44, step=760.0)
    for k in range(3):
        tilt_box(g, WHITE, (-300 if k % 2 == 0 else 300, 0, 250 + k * 800), (300 if k % 2 == 0 else -300, 0, 250 + (k + 1) * 800), 18, 18)
    g.box(PLUM, (0, 0, T - 25), (760, 300, 50))
    g.box(WHITE, (0, 0, T + 15), (1000, 540, 30))
    g.box(LAV, (0, -271, T + 15), (1000, 6, 34))
    cx = -240
    g.box(WHITE, (cx, 30, D + 130), (420, 380, 260))                  # the cabin
    g.box(POOL, (cx, 30, D + 18), (430, 390, 36))
    g.box(PLUM, (cx - 110, -162, D + 105), (90, 8, 190))
    cone(g, GOLD, (cx + 70, -156, D + 150), (cx + 70, -170, D + 150), 62, 62, 8)
    cone(g, CYAN, (cx + 70, -160, D + 150), (cx + 70, -176, D + 150), 46, 46, 8, glow=0.9)
    g.box(LAV, (cx, 30, D + 268), (470, 430, 18))
    lamp_row(g, (cx - 200, -190, D + 250), (cx + 200, -190, D + 250), 4)
    hz = D + 277
    g.cyl(GOLD, (cx, 30, hz), 190, 50, steps=10, top_r=150)           # the helmet's breastplate
    g.sphere(GOLD, (cx, 30, hz + 185), 160, steps=(6, 10))
    cone(g, GOLD, (cx, -110, hz + 185), (cx, -146, hz + 185), 74, 74, 8)
    cone(g, CYAN, (cx, -128, hz + 185), (cx, -154, hz + 185), 54, 54, 8, glow=1.0)
    for sx in (-1, 1):
        cone(g, GOLD, (cx + sx * 136, 30, hz + 185), (cx + sx * 172, 30, hz + 185), 56, 56, 8)
        cone(g, CYAN, (cx + sx * 156, 30, hz + 185), (cx + sx * 180, 30, hz + 185), 40, 40, 8, glow=1.0)
    g.cyl(GOLD, (cx, 30, hz + 335), 40, 40, steps=8)
    g.box(GOLD, (cx, 30, hz + 386), (120, 12, 12))
    tilt_box(g, POOL, (cx + 30, 40, hz + 360), (cx + 262, 62, hz + 240), 12, 12)       # the air hose
    tilt_box(g, POOL, (cx + 262, 62, hz + 240), (cx + 330, 62, D + 60), 12, 12)
    g.box(PLUM, (cx + 330, 62, D + 30), (74, 64, 60))                 # the pump and its wheel
    cone(g, WHITE, (cx + 330, 30, D + 46), (cx + 330, 16, D + 46), 34, 34, 8)
    for k in range(3):                                                # spare helmets on a rack
        g.box(PLUM, (40 + k * 76, 150, D + 40), (8, 8, 80))
        g.sphere(GOLD, (40 + k * 76, 150, D + 108), 30, steps=(3, 6))
    g.sphere(LAV, (270, 30, D + 36), 132, scale=(1.0, 0.86, 0.3), steps=(4, 8))        # the clam
    g.sphere(WHITE, (270, 20, D + 86), 46, steps=(4, 8), glow=0.5)
    lid = sc.part(P.motion(swing=5.0, pivot=(270, 140, D + 60), axis=0.0, rate=0.15, phase=0.5))
    tilt_box(lid, LAV, (270, 138, D + 62), (270, -30, D + 196), 250, 28)
    for k in range(3):
        tilt_box(lid, WHITE, (190 + k * 80, 132, D + 82), (190 + k * 80, -36, D + 216), 22, 10)
    rail(g, -20, 440, -262, D, 90)
    lamp(g, -20, -262, D + 106, 18)
    lamp(g, 440, -262, D + 106, 18)
    g.box(PLUM, (470, -200, D + 165), (16, 16, 330))                  # the hoist
    tilt_box(g, PLUM, (470, -200, D + 322), (640, -200, D + 362), 14, 14)
    bell = sc.part(P.motion(bob=-70.0, rate=0.1, phase=0.25))
    bell.box(PLUM, (624, -200, D + 20), (6, 6, 680))
    bell.cyl(CYAN, (624, -200, D - 430), 62, 110, steps=8, top_r=44, glow=0.35)
    bell.cyl(WHITE, (624, -200, D - 320), 46, 16, steps=8)
    pennant(sc, cx, 30, hz + 420, LAV, 110, 46, phase=0.5)
    g.box(WHITE, (cx, 30, hz + 410), (6, 6, 60))


# ------------------------------------------------------------------------------------------ far buildings (96 m)
def lily_pagoda(sc):
    """BUILDING (far lots). The lily pagoda: a thick ribbed stem with giant lily pads for steps, a round terrace of
    lamp posts 27.5 m up, two storeys of lit windows under two flared lavender roofs, lanterns under the lower eaves,
    two pennants. 14 m across, 40 m tall."""
    g = sc.still()
    T = D2 - 40
    footing(g, 0, 0, 230)
    g.cyl(POOL, (0, 0, 0), 120, T - 300, steps=10, top_r=92)
    for k, z in enumerate((500, 1150, 1800)):
        g.cyl(LAV if k % 2 else WHITE, (0, 0, z), 128 - k * 6, 70, steps=10)
    for k in range(5):
        a = math.radians(k * 72 + 18)
        ca, sa = math.cos(a), math.sin(a)
        tilt_box(g, WHITE, (ca * 124, sa * 124, 300), (ca * 100, sa * 100, T - 320), 22, 22)
    capital(g, 0, 0, T, 92, 520, h=320, col=WHITE, steps=10)
    for k in range(6):
        a = math.radians(k * 60 - 90)
        g.cyl(MINT if k % 2 else LAV, (math.cos(a) * 230, math.sin(a) * 230, 480 + k * 340), 150, 14, steps=8)
    g.cyl(LAV, (0, 0, T), 560, 40, steps=12)
    for k in range(12):
        a = math.radians(k * 30 - 90)
        ca, sa = math.cos(a), math.sin(a)
        g.box(WHITE, (ca * 540, sa * 540, D2 + 45), (10, 10, 90))
        if k % 2 == 0:
            lamp(g, ca * 540, sa * 540, D2 + 102, 20)
    g.cyl(WHITE, (0, 0, D2), 400, 300, steps=16)
    g.cyl(POOL, (0, 0, D2), 408, 34, steps=16)
    for k in range(8):
        if k == 0:
            ring_box(g, PLUM, 398, D2 + 125, (110, 18, 220), -90)
        else:
            ring_box(g, CYAN, 398, D2 + 170, (84, 18, 150), k * 45 - 90, glow=0.9)
    z = D2 + 300
    g.cyl(WHITE, (0, 0, z), 640, 20, steps=8, top_r=630)
    g.cyl(LAV, (0, 0, z + 20), 622, 150, steps=8, top_r=330)
    for k in range(4):
        a = math.radians(k * 90)
        ca, sa = math.cos(a), math.sin(a)
        tilt_box(g, WHITE, (ca * 550, sa * 550, z + 14), (ca * 704, sa * 704, z + 96), 50, 12)
    for k in range(8):
        a = math.radians(k * 45 + 22.5)
        hang_lamp(g, math.cos(a) * 560, math.sin(a) * 560, z + 4)
    z += 170
    g.cyl(WHITE, (0, 0, z - 40), 270, 290, steps=16)
    for k in range(6):
        ring_box(g, CYAN, 268, z + 140, (70, 16, 120), k * 60 - 90, glow=0.9)
    top = pagoda(g, 0, 0, z + 250, 430)
    pennant(sc, 0, 0, top - 30, LAV, 150, 60)
    pennant(sc, 0, 0, top - 100, CYAN, 110, 46, phase=0.5)


def pump_house(sc):
    """BUILDING (far lots). The pumping house: a deck on two braced stems, an engine house under a bell roof with a
    plum stack (a puff of steam floats over it), a great blue beam that rocks on its post, a wheel of six scoops that
    goes slowly round one stem half way up, and the water it lifts falling back in a ribbon. 13.7 x 7.1 m, 37 m tall."""
    g = sc.still()
    T = D2 - 30
    for sx in (-1, 1):
        pile(g, sx * 330, 0, T, r=52, step=760.0)
    for k in range(3):
        tilt_box(g, WHITE, (-330 if k % 2 == 0 else 330, 0, 250 + k * 810), (330 if k % 2 == 0 else -330, 0, 250 + (k + 1) * 810), 20, 20)
    g.box(PLUM, (0, 0, T - 25), (860, 340, 50))
    g.box(WHITE, (0, 0, T + 15), (1080, 620, 30))
    g.box(LAV, (0, -311, T + 15), (1080, 6, 34))
    ex = -250
    g.box(WHITE, (ex, 40, D2 + 170), (440, 420, 340))                 # the engine house
    g.box(POOL, (ex, 40, D2 + 20), (450, 430, 40))
    windows(g, (ex - 120, ex + 120), -171, D2 + 195, 80, 150, CYAN, 0.9, frame=LAV)
    g.box(PLUM, (ex, -172, D2 + 105), (80, 8, 200))
    bell_roof(g, ex, D2 + 340, 250, -180, 260, 200)
    lamp_row(g, (ex - 210, -188, D2 + 322), (ex + 210, -188, D2 + 322), 4)
    g.cyl(PLUM, (ex - 120, 150, D2 + 340), 40, 420, steps=8, top_r=30)
    g.cyl(LAV, (ex - 120, 150, D2 + 760), 44, 30, steps=8)
    for ph, dx, dz, r in ((0.3, 0, 870, 60), (0.8, 44, 960, 42)):
        s = sc.part(P.motion(bob=-45.0, rate=0.2, phase=ph))
        s.sphere(WHITE, (ex - 120 + dx, 150, D2 + dz), r, steps=(4, 6))
    px, pz = 120, D2 + 600
    g.box(PLUM, (px, 0, D2 + 300), (50, 120, 600))                    # the beam's post
    g.cyl(POOL, (px + 320, 0, D2), 46, 130, steps=8)                  # the pump's barrel
    b = sc.part(P.motion(swing=10.0, pivot=(px, 0, pz), axis=90.0, rate=0.2))
    tilt_box(b, POOL, (px - 330, 0, pz), (px + 330, 0, pz), 60, 46)
    b.box(WHITE, (px, 0, pz), (72, 76, 72))
    b.box(GOLD, (px + 330, 0, pz), (40, 72, 62))
    b.box(GOLD, (px - 330, 0, pz), (40, 72, 62))
    b.box(PLUM, (px + 320, 0, D2 + 330), (14, 14, 540))
    b.box(PLUM, (px - 320, 0, pz - 110), (14, 14, 220))
    wx, wz = 330.0, 1500.0                                            # the wheel of scoops, round the right stem
    w = sc.part(P.motion(base=(wx, 0.0), travel=(-1.0, 0.04)))
    w.cyl(LAV, (wx, 0, wz - 25), 84, 50, steps=8)
    for k in range(6):
        a = math.radians(k * 60)
        ca, sa = math.cos(a), math.sin(a)
        a2 = math.radians(k * 60 + 60)
        tilt_box(w, WHITE, (wx + ca * 60, sa * 60, wz), (wx + ca * 300, sa * 300, wz), 12, 12)
        tilt_box(w, POOL, (wx + ca * 300, sa * 300, wz), (wx + math.cos(a2) * 300, math.sin(a2) * 300, wz), 14, 14)
        w.box(LAV, (wx + ca * 312, sa * 312, wz + 30), (80, 90, 80), rot=(0, k * 60.0, 0))
        w.box(CYAN, (wx + ca * 312, sa * 312, wz + 74), (60, 70, 10), rot=(0, k * 60.0, 0), glow=0.5)
    g.box(LAV, (ex - 250, 0, D2 + 60), (70, 44, 44))                  # the spout, and the water falling from it
    for ph, dx, wdt in ((0.0, 0, 26), (0.5, 22, 16)):
        f = sc.part(P.motion(bob=-25.0, rate=0.9, phase=ph))
        f.box(CYAN if dx == 0 else SPRAY, (ex - 285 - dx, 0, (D2 + 40) / 2.0 + 10), (wdt, wdt, D2 + 20), glow=0.5)
    foam = sc.part(P.motion(bob=-8.0, rate=0.7, phase=0.2))
    foam.sphere(WHITE, (ex - 290, 0, 20), 90, scale=(1.0, 1.0, 0.3), steps=(3, 6))
    rail(g, 0, 520, -302, D2, 90)
    lamp(g, 0, -302, D2 + 106, 18)
    lamp(g, 520, -302, D2 + 106, 18)
    g.box(WHITE, (ex, 40, D2 + 570), (8, 8, 90))
    pennant(sc, ex, 40, D2 + 600, LAV, 120, 50, phase=0.2)


# ------------------------------------------------------------------------------------------ tall props
def channel_marker(sc):
    """PROP, tall and slim. A channel marker: a pile banded blue and white, 28 m, a tide board of ticks at its foot,
    two lavender cones point to point, a caged lamp (cyan, steady) under a lavender cap, a signal flag on an arm.
    1.8 m across, 31 m tall."""
    g = sc.still()
    footing(g, 0, 0, 80)
    H = 2800.0
    seg = H / 5.0
    for k in range(5):
        r0 = 36.0 - k * 2.4
        g.cyl(WHITE if k % 2 else POOL, (0, 0, k * seg), r0, seg, steps=8, top_r=r0 - 2.4)
    g.box(WHITE, (0, -40, 520), (50, 8, 420))
    for k in range(5):
        g.box(POOL, (8 if k % 2 else -8, -45, 340 + k * 90), (30, 6, 14))
    g.cyl(LAV, (0, 0, 900), 52, 16, steps=8)
    g.cyl(LAV, (0, 0, H - 260), 60, 110, steps=6, top_r=8)
    g.cyl(LAV, (0, 0, H - 150), 8, 110, steps=6, top_r=60)
    g.cyl(LAV, (0, 0, H), 70, 14, steps=8)
    g.cyl(PLUM, (0, 0, H + 14), 22, 90, steps=6)
    g.box(CYAN, (0, 0, H + 150), (64, 64, 90), glow=1.3)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(PLUM, (sx * 36, sy * 36, H + 150), (8, 8, 96))
    g.cyl(LAV, (0, 0, H + 198), 86, 60, steps=8, top_r=10)
    g.sphere(WHITE, (0, 0, H + 272), 16, steps=(3, 6))
    g.box(WHITE, (44, 0, H - 420), (90, 8, 8))
    f = sc.part(P.motion(swing=9.0, pivot=(70, 0, H - 424), axis=0.0, rate=0.4, phase=0.3))
    f.box(LAV, (70, 0, H - 462), (44, 5, 70))
    f.box(WHITE, (70, 0, H - 480), (44, 6, 22))


def lantern_line(sc):
    """PROP, tall and slim. Two white masts 30 m tall with shell finials and a line between them at the rails'
    height: five square lanterns (cyan and warm, steady) under lavender caps, four pennants that sway. 10.9 m long."""
    g = sc.still()
    H, X = 3000.0, 520.0
    for sx in (-1, 1):
        footing(g, sx * X, 0, 60)
        g.cyl(WHITE, (sx * X, 0, 0), 22, H, steps=6, top_r=14)
        for k, z in enumerate((600, 1500, 2400)):
            g.cyl(LAV if k % 2 == 0 else POOL, (sx * X, 0, z), 28, 50, steps=6)
        g.sphere(LAV, (sx * X, 0, H + 30), 40, scale=(1.0, 0.5, 1.0), steps=(4, 6))
    n = 9
    pts = [(-X + 2 * X * k / (n + 1.0), 0, H - 60 - 150 * math.sin(math.pi * k / (n + 1.0))) for k in range(n + 2)]
    for a, b in zip(pts, pts[1:]):
        tilt_box(g, PLUM, a, b, 6, 6)
    sw = sc.part(P.motion(swing=7.0, pivot=(0, 0, H - 150), axis=0.0, rate=0.3, phase=0.1))
    for k in range(1, n + 1):
        x, _, z = pts[k]
        if k % 2:
            g.box(LAV, (x, 0, z - 10), (40, 40, 8))
            g.box(CYAN if k % 4 == 1 else BULB, (x, 0, z - 34), (30, 30, 40), glow=1.2)
            g.box(LAV, (x, 0, z - 58), (16, 16, 8))
        else:
            sw.box(WHITE if k % 4 == 0 else LAV, (x, 0, z - 32), (36, 5, 56))


def high_jetty(sc):
    """PROP, tall and slim. A length of boardwalk at the rails' height on three pairs of braced blue piles: planks,
    a white rail behind, mooring posts and rope swags in front, four lanterns under the deck's edge, a bench, a life
    ring, two fishing rods left leaning, a pennant at each end. 11.2 x 5 m, 29 m tall. It joins two pavilions."""
    g = sc.still()
    L, T = 1120.0, D - 20
    for x in (-480, 0, 480):
        for sy in (-1, 1):
            post(g, x, sy * 95, T, w=34, col=POOL, band=WHITE, bands=(800.0, 1700.0))
        g.box(PLUM, (x, 0, T - 40), (40, 280, 40))
        tilt_box(g, WHITE, (x, -95, 2000), (x, 95, 2500), 12, 12)
    for xa, xb in ((-480, 0), (0, 480)):
        for j in range(2):
            z0 = 300 + j * 1150
            tilt_box(g, WHITE, (xa if j == 0 else xb, -95, z0), (xb if j == 0 else xa, -95, z0 + 1150), 14, 14)
    g.box(WOOD, (0, 0, T + 10), (L, 270, 20))
    for k in range(6):
        g.box(WOOD_D, (-450 + k * 180, 0, T + 21), (8, 270, 4))
    rail(g, -L / 2.0, L / 2.0, 128, D, 95, WHITE, gap=280.0)
    xs = (-500, -250, 0, 250, 500)
    for x in xs:
        g.cyl(PLUM, (x, -118, D), 16, 100, steps=6)
        g.cyl(WHITE, (x, -118, D + 100), 20, 16, steps=6)
    for a, b in zip(xs, xs[1:]):
        mid = (a + b) / 2.0
        tilt_box(g, LAV, (a, -118, D + 84), (mid, -118, D + 52), 8, 8)
        tilt_box(g, LAV, (mid, -118, D + 52), (b, -118, D + 84), 8, 8)
    for x in (-375, -125, 125, 375):
        hang_lamp(g, x, -128, T)
    cone(g, WHITE, (300, 122, D + 50), (300, 108, D + 50), 30, 30, 8)
    g.box(LAV, (-200, 80, D + 45), (180, 50, 10))
    for sx in (-1, 1):
        g.box(PLUM, (-200 + sx * 70, 80, D + 20), (10, 44, 40))
    for x in (120, 170):
        tilt_box(g, WOOD_D, (x, -100, D + 8), (x, -246, D + 236), 5, 5)
    g.cyl(WHITE, (-420, 40, D), 30, 16, steps=8)
    for sx in (-1, 1):
        g.box(WHITE, (sx * L / 2.0, 128, D + 150), (8, 8, 200))
        pennant(sc, sx * L / 2.0, 128, D + 230, LAV if sx < 0 else CYAN, 90, 40, phase=0.25 + 0.25 * sx)


def fish_vane(sc):
    """PROP (far lots). A weather-vane mast 31.5 m tall: a guyed white mast, a cross of arms with lavender balls
    (no letters), a steady cyan lamp, and on top a lavender fish 6.2 m long over an arrow, turning slowly to and fro."""
    g = sc.still()
    H = 3150.0
    footing(g, 0, 0, 90)
    g.cyl(WHITE, (0, 0, 0), 34, H, steps=8, top_r=16)
    for k, z in enumerate((500, 1200, 1900, 2600)):
        g.cyl(LAV if k % 2 else POOL, (0, 0, z), 40 - k * 4, 60, steps=8)
    for ang in (90, 210, 330):
        ca, sa = math.cos(math.radians(ang)), math.sin(math.radians(ang))
        tilt_box(g, PLUM, (ca * 260, sa * 260, -100), (ca * 20, sa * 20, 700), 14, 14)
    Z = H - 250
    tilt_box(g, WHITE, (-190, 0, Z), (190, 0, Z), 12, 12)
    tilt_box(g, WHITE, (0, -190, Z), (0, 190, Z), 12, 12)
    for ang in (0, 90, 180, 270):
        g.sphere(LAV, (math.cos(math.radians(ang)) * 200, math.sin(math.radians(ang)) * 200, Z), 26, steps=(3, 6))
    g.cyl(LAV, (0, 0, Z - 60), 46, 30, steps=6, top_r=20)
    g.box(CYAN, (0, 0, Z - 95), (52, 52, 64), glow=1.2)
    v = sc.part(P.motion(swing=26.0, pivot=(0, 0, H), axis=P.UP, rate=0.11, phase=0.2))
    fz = H + 120
    body = [(290, 0), (200, 74), (40, 100), (-110, 64), (-200, 22), (-300, 100), (-335, 0), (-300, -100), (-200, -22), (-110, -64), (40, -84), (200, -62)]
    v.side(LAV, [(x, fz + z) for x, z in body], -16, 16)
    v.box(WHITE, (40, 0, fz - 34), (300, 38, 34))
    v.side(WHITE, [(-60, fz + 70), (40, fz + 170), (120, fz + 90)], -8, 8)
    for sy in (-1, 1):
        v.box(PLUM, (215, sy * 17, fz + 24), (26, 6, 26))
    v.box(WHITE, (0, 0, H + 10), (14, 14, 60))
    tilt_box(v, WHITE, (-260, 0, H + 8), (330, 0, H + 8), 10, 10)
    v.box(WHITE, (340, 0, H + 8), (50, 12, 44))


# ------------------------------------------------------------------------------------------ on the valley floor (27 to 38 m)
def mooring_jetty(sc):
    """PROP on the valley floor (seen from above). A low jetty 12 m long over a strip of lit water in a white kerb:
    planks, mooring posts and rope, two lantern posts, two rowing boats moored in front and rocking, a bench, a life
    ring. 15 x 7 m, 5.2 m tall."""
    g = sc.still()
    L, Z = 1200.0, 130.0
    g.box(WHITE, (0, 0, -140), (1500, 700, 300))
    g.box(WATER, (0, 0, 13), (1460, 660, 10), glow=0.25)
    for x in (-540, -180, 180, 540):
        for sy in (-1, 1):
            g.box(PLUM, (x, sy * 110, Z / 2.0), (30, 30, Z))
    g.box(WOOD, (0, 0, Z + 10), (L, 280, 20))
    for k in range(7):
        g.box(WOOD_D, (-450 + k * 150, 0, Z + 21), (8, 280, 4))
    xs = (-520, -170, 170, 520)
    for x in xs:
        g.cyl(PLUM, (x, -124, Z + 20), 18, 95, steps=6)
        g.cyl(WHITE, (x, -124, Z + 115), 22, 16, steps=6)
    for a, b in zip(xs, xs[1:]):
        mid = (a + b) / 2.0
        tilt_box(g, LAV, (a, -124, Z + 100), (mid, -124, Z + 62), 8, 8)
        tilt_box(g, LAV, (mid, -124, Z + 62), (b, -124, Z + 100), 8, 8)
    for sx in (-1, 1):
        g.box(WHITE, (sx * 560, 118, Z + 185), (10, 10, 330))
        g.box(WHITE, (sx * 530, 118, Z + 345), (70, 8, 8))
        hang_lamp(g, sx * 500, 118, Z + 345)
    for i, (bx, col, ph) in enumerate(((-280, LAV, 0.1), (260, CYAN, 0.6))):
        b = sc.part(P.motion(bob=-6.0, rate=0.25, phase=ph))
        b.side(col, [(bx - 190, 84), (bx - 140, 20), (bx + 140, 20), (bx + 190, 84)], -292, -172)
        b.box(WHITE, (bx, -232, 86), (372, 124, 8))
        b.box(PLUM, (bx - 50, -232, 94), (20, 110, 10))
        b.box(PLUM, (bx + 60, -232, 94), (20, 110, 10))
        tilt_box(b, WOOD, (bx - 150, -262, 96), (bx + 150, -206, 104), 7, 7)
    g.box(LAV, (-150, 90, Z + 62), (200, 50, 10))
    for sx in (-1, 1):
        g.box(PLUM, (-150 + sx * 80, 90, Z + 38), (10, 44, 40))
    g.box(WHITE, (330, 120, Z + 70), (8, 8, 100))
    cone(g, WHITE, (330, 112, Z + 130), (330, 98, Z + 130), 30, 30, 8)
    g.cyl(WHITE, (60, 60, Z + 20), 30, 14, steps=8)


def _pedalo(m, x, y, z, col):
    m.box(col, (x, y, z + 18), (220, 110, 36))
    m.box(WHITE, (x + 95, y, z + 24), (40, 92, 34))
    for sy in (-1, 1):
        m.box(PLUM, (x - 20, y + sy * 26, z + 52), (50, 40, 34))
    cone(m, POOL, (x - 96, y - 44, z + 40), (x - 96, y + 44, z + 40), 34, 34, 8)


def pedalo_dock(sc):
    """PROP on the valley floor. The paddle-boat rack: six pedalos in lavender, cyan and white on two tiers of a
    plum and white rack under a little bell roof with a row of lamps, a paddle-wheel sign on a post at the end
    (a picture, no word). 10.6 x 2.6 m, 5.2 m tall."""
    g = sc.still()
    for x in (-420, 0, 420):
        for sy in (-1, 1):
            g.box(PLUM, (x, sy * 90, 110), (22, 22, 580))
    for z in (60, 230):
        for sy in (-1, 1):
            g.box(WHITE, (0, sy * 90, z), (900, 14, 14))
    cols = (LAV, CYAN, WHITE, CYAN, WHITE, LAV)
    for i in range(6):
        _pedalo(g, -280 + (i % 3) * 280, 0, 68 + (i // 3) * 170, cols[i])
    bell_roof(g, 0, 400, 440, -125, 125, 110)
    lamp_row(g, (-400, -128, 384), (400, -128, 384), 5)
    g.box(WHITE, (530, 0, 130), (14, 14, 500))
    cone(g, LAV, (530, -8, 400), (530, -22, 400), 72, 72, 8)
    for a in (0, 45, 90, 135):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(g, WHITE, (530 - ca * 66, -27, 400 - sa * 66), (530 + ca * 66, -27, 400 + sa * 66), 8, 10)
    g.box(POOL, (-520, 0, 20), (70, 70, 100))
    g.sphere(MINT, (-520, 0, 92), 44, scale=(1.0, 1.0, 0.7), steps=(3, 6))


def lily_fountain(sc):
    """PROP on the valley floor. The water-lily fountain: a round white basin of lit water with lily pads, a blue
    stem, three lavender lily dishes one over another with white petal tips, threads of water falling from dish to
    dish (they shimmer), a bud and a jet on top, four cyan lamps on the rim. 6.8 m across, 8.6 m tall."""
    g = sc.still()
    g.cyl(PLUM, (0, 0, -300), 330, 330, steps=10)
    g.cyl(WHITE, (0, 0, 0), 340, 60, steps=12, top_r=330)
    g.cyl(WATER, (0, 0, 50), 305, 14, steps=12, glow=0.25)
    g.cyl(POOL, (0, 0, 60), 46, 540, steps=8, top_r=30)
    w = [sc.part(P.motion(bob=-12.0, rate=0.8, phase=p)) for p in (0.0, 0.5)]
    prev = 64.0
    for i, (z, r) in enumerate(((220.0, 230.0), (390.0, 175.0), (540.0, 125.0))):
        g.cyl(LAV, (0, 0, z), r * 0.3, 56, steps=10, top_r=r)
        g.cyl(SPRAY, (0, 0, z + 52), r * 0.94, 10, steps=10, glow=0.3)
        for k in range(6):
            a = math.radians(k * 60 + i * 30)
            ca, sa = math.cos(a), math.sin(a)
            tilt_box(g, WHITE, (ca * r * 0.75, sa * r * 0.75, z + 40), (ca * r * 1.22, sa * r * 1.22, z + 84), 46, 10)
            a = math.radians(k * 60 + i * 30 + 30)
            ca, sa = math.cos(a), math.sin(a)
            w[k % 2].box(CYAN, (ca * r * 1.02, sa * r * 1.02, (z + 56 + prev) / 2.0), (16, 16, z + 56 - prev), glow=0.6)
        prev = z + 60.0
    g.cyl(LAV, (0, 0, 596), 30, 80, steps=8, top_r=58)
    g.cyl(WHITE, (0, 0, 676), 58, 66, steps=8, top_r=6)
    j = sc.part(P.motion(bob=-14.0, rate=1.1, phase=0.3))
    j.cyl(SPRAY, (0, 0, 736), 9, 120, steps=5, top_r=4, glow=0.6)
    for k, (ang, r) in enumerate(((40, 200), (170, 230), (290, 190))):
        ca, sa = math.cos(math.radians(ang)), math.sin(math.radians(ang))
        g.cyl(MINT, (ca * r, sa * r, 62), 58, 6, steps=8)
        if k != 1:
            g.cyl(LAV, (ca * r, sa * r, 68), 10, 30, steps=6, top_r=24)
    for ang in (45, 135, 225, 315):
        ca, sa = math.cos(math.radians(ang)), math.sin(math.radians(ang))
        lamp(g, ca * 334, sa * 334, 76, 22, CYAN, 1.2)


# ------------------------------------------------------------------------------------------ low props (Splashdown's roadside)
def oar_rack(sc):
    """PROP, LOW and unlit (top 1.85 m). A row of oars on a rack: two plum trestles, white rails, eight oars
    leaning with lavender, cyan and white blades, a bucket and a coil of rope. 7 m long."""
    g = sc.still()
    for x in (-280, 280):
        tilt_box(g, PLUM, (x, -70, 4), (x, 0, 150), 14, 14)
        tilt_box(g, PLUM, (x, 70, 4), (x, 0, 150), 14, 14)
    g.box(WHITE, (0, 0, 150), (640, 14, 14))
    g.box(WHITE, (0, -47, 54), (640, 12, 12))
    cols = (LAV, CYAN, WHITE)
    for k in range(8):
        x = -245 + k * 70
        tilt_box(g, WOOD, (x, -95, 12), (x, 22, 160), 7, 7)
        tilt_box(g, cols[k % 3], (x, 20, 157), (x, 38, 180), 26, 8)
    g.cyl(POOL, (330, -40, 0), 18, 30, steps=8, top_r=22)
    g.cyl(WHITE, (-340, -30, 0), 26, 10, steps=8)


def shell_bench(sc):
    """PROP, LOW and unlit (top 1.75 m). Two benches with scallop-shell backs in white and lavender, a blue tub of
    bulrushes between them. 6.2 m long."""
    g = sc.still()
    for bx in (-190, 190):
        g.box(WHITE, (bx, 0, 46), (240, 56, 12))
        for sx in (-1, 1):
            g.box(PLUM, (bx + sx * 95, 0, 20), (14, 50, 40))
        for k in range(5):
            a = math.radians(-50 + k * 25)
            tilt_box(g, LAV if k % 2 else WHITE, (bx, 34, 50), (bx + math.sin(a) * 115, 34, 50 + math.cos(a) * 115), 10, 50)
    g.cyl(POOL, (0, 0, 0), 46, 60, steps=8, top_r=54)
    g.sphere(MINT, (0, 0, 74), 40, scale=(1.0, 1.0, 0.5), steps=(3, 6))
    for dx, dy, h in ((-12, 0, 172), (10, 8, 150), (4, -10, 128)):
        g.box(MINT, (dx, dy, (h + 60) / 2.0), (7, 7, h - 60))
        g.box(PLUM, (dx, dy, h - 14), (10, 10, 28))


# ------------------------------------------------------------------------------------------ the hero
def seahorse_cascade(sc):
    """HERO LANDMARK. THE SEAHORSE CASCADE: a wedding-cake waterfall 62 m tall in a round pool 42 m across. Five
    white drums with lit arches, each carrying a lavender shell basin with white flutes; from every basin eight
    sheets of water fall to the one below, going slowly round (half one way, half the other) and rising and falling;
    foam floats where they land in the pool, shell lamps stand round its rim; on top a lavender seahorse 14.7 m tall
    with a white belly, a cyan fin and a gold crown rears and nods."""
    g = sc.still()
    g.cyl(PLUM, (0, 0, -300), 2050, 360, steps=16)
    g.cyl(WHITE, (0, 0, 0), 2100, 130, steps=16, top_r=2060)
    g.cyl(WATER, (0, 0, 110), 1980, 30, steps=16, glow=0.3)
    R = (1650.0, 1320.0, 1010.0, 720.0, 450.0)
    TH, Z0 = 900.0, 130.0
    wa = sc.part(P.motion(bob=-55.0, rate=0.45, phase=0.0, base=(0.0, 0.0), travel=(-1.0, 0.02)))
    wb = sc.part(P.motion(bob=-40.0, rate=0.6, phase=0.45, base=(0.0, 0.0), travel=(-1.0, -0.014)))
    for i, r in enumerate(R):
        z = Z0 + i * TH
        rd = r * 0.8
        g.cyl(WHITE, (0, 0, z - 40), rd, TH - 110, steps=12)
        g.cyl(LAV, (0, 0, z + TH - 150), rd, 150, steps=12, top_r=r)
        g.cyl(WHITE, (0, 0, z + TH), r, 26, steps=12, top_r=r * 0.985)
        g.cyl(WATER, (0, 0, z + TH + 8), r * 0.93, 24, steps=12, glow=0.3)
        for k in range(6):
            a = math.radians(k * 60 + i * 30)
            ca, sa = math.cos(a), math.sin(a)
            tilt_box(g, WHITE, (ca * rd, sa * rd, z + TH - 150), (ca * r * 1.03, sa * r * 1.03, z + TH + 10), r * 0.1, 30)
            ring_box(g, BULB, rd * 0.99, z + 330, (r * 0.16, rd * 0.07, 300), k * 60 + i * 30 + 30, glow=0.8)
        top, bot = z + TH + 20, (z + 20 if i else 122.0)
        for k in range(8):
            m = wa if k % 2 == 0 else wb
            a = math.radians(k * 45 + i * 22.5)
            m.box(CYAN if k % 2 == 0 else SPRAY, (math.cos(a) * r * 1.01, math.sin(a) * r * 1.01, (top + bot) / 2.0), (r * 0.3, 16, top - bot),
                  rot=(0, k * 45 + i * 22.5 + 90.0, 0), glow=0.55)
    for k in range(8):                                                # foam where the lowest sheets land
        a = math.radians(k * 45 + 10)
        f = sc.part(P.motion(bob=-10.0, rate=0.5, phase=(k * 0.29) % 1.0))
        f.sphere(WHITE, (math.cos(a) * 1720, math.sin(a) * 1720, 140), 110, scale=(1.0, 1.0, 0.3), steps=(3, 6))
    for k in range(12):                                               # shell lamps on the pool's rim
        a = math.radians(k * 30 + 15)
        ca, sa = math.cos(a), math.sin(a)
        g.box(LAV, (ca * 2075, sa * 2075, 150), (50, 50, 40))
        g.box(BULB, (ca * 2075, sa * 2075, 196), (40, 40, 52), glow=1.3)
    SZ = Z0 + 5 * TH + 30                                             # the seahorse's pedestal, in the top basin
    g.cyl(WHITE, (0, 0, SZ), 170, 150, steps=10, top_r=120)
    SZ += 150
    S = 1.05
    sh = sc.part(P.motion(swing=3.0, pivot=(0, 0, SZ), axis=90.0, rate=0.12, phase=0.0))
    body = [(-40, 0), (-170, 120), (-230, 330), (-210, 560), (-150, 760), (-170, 900), (-150, 1060), (-90, 1190), (10, 1260), (110, 1230),
            (170, 1150), (330, 1090), (420, 1060), (420, 1000), (300, 990), (200, 960), (130, 900), (110, 800), (190, 700), (230, 560),
            (190, 400), (110, 280), (60, 150), (150, 110), (230, 130), (260, 60), (190, 0)]
    sh.side(LAV, [(x * S, SZ + z * S) for x, z in body], -80, 80)
    for x, z in ((185, 700), (212, 560), (176, 410), (104, 290)):
        sh.box(WHITE, (x * S, 0, SZ + z * S), (56, 172, 86))
    sh.side(CYAN, [(x * S, SZ + z * S) for x, z in ((-222, 400), (-330, 470), (-352, 600), (-300, 700), (-205, 640))], -10, 10, glow=0.5)
    for k in range(3):
        tilt_box(sh, GOLD, ((-60 + k * 60) * S, 0, SZ + 1225 * S), ((-92 + k * 72) * S, 0, SZ + 1345 * S), 22, 22)
    for sy in (-1, 1):
        sh.box(WHITE, (150 * S, sy * 82, SZ + 1130 * S), (52, 6, 52))
        sh.box(PLUM, (156 * S, sy * 86, SZ + 1130 * S), (28, 6, 28))
    sh.box(WHITE, (425 * S, 0, SZ + 1030 * S), (30, 100, 70))


JOBS = [
    ('SM_S1D3_Boathouse', lit(boathouse), SLIM),
    ('SM_S1D3_JellyTea', lit(jelly_tea), SLIM),
    ('SM_S1D3_DiveTower', lit(dive_tower), SLIM),
    ('SM_S1D3_ShellStand', lit(shell_stand), SLIM),
    ('SM_S1D3_PearlStation', lit(pearl_station), dict(grow=(40, 0, 80), grow_down=(40, 0, 0))),
    ('SM_S1D3_LilyPagoda', lit(lily_pagoda), dict(grow=(60, 60, 40), grow_down=(60, 60, 0))),
    ('SM_S1D3_PumpHouse', lit(pump_house), dict(grow=(60, 60, 60), grow_down=(60, 60, 0))),
    ('SM_S1D3_ChannelMarker', lit(channel_marker), SLIM),
    ('SM_S1D3_LanternLine', lit(lantern_line), dict(grow=(0, 20, 0), grow_down=(0, 20, 0))),
    ('SM_S1D3_HighJetty', lit(high_jetty), SLIM),
    ('SM_S1D3_FishVane', lit(fish_vane), dict(grow=(60, 180, 30), grow_down=(60, 180, 0))),
    ('SM_S1D3_MooringJetty', lit(mooring_jetty), dict(grow=(10, 10, 10), grow_down=(10, 10, 0))),
    ('SM_S1D3_PedaloDock', lit(pedalo_dock), STILL),
    ('SM_S1D3_LilyFountain', lit(lily_fountain), dict(grow=(0, 0, 20), grow_down=(0, 0, 0))),
    ('SM_S1D3_OarRack', oar_rack, STILL),
    ('SM_S1D3_ShellBench', shell_bench, STILL),
    ('SM_S1D3_SeahorseCascade', lit(seahorse_cascade, 0.14), dict(far=True, grow=(60, 60, 80), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
