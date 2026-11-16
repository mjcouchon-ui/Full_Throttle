"""Theme Park, Stage 1, builder s1d4: district 4 "Rocket Land and Funhouse" (6.9074 to 9.2974 km), TIN-TOY TOWN.
/Game/World/ThemePark/SM_S1D4_*. Run: import ft_world_park_s1d4 as K; K.run()

Wind-up tin toys grown to buildings: riveted drums and boxes with seams, red fins for buttresses, cyan portholes, dome
roofs with an aerial ball, and a gold butterfly key that turns slowly, back and forth, on a side wall. From 8.12 km the
same tin goes crooked (leaning floors, doors out of true, stairs that stop in the air); the back lot keeps the props.
Hero: THE WIND-UP OBSERVATORY (a 34 m tin drum under a dome, a telescope barrel out of its slit, a 16 m key each side).

Every piece is an ft_people.Scene: one material (M_People), one section, movement in the vertex shader only.
LIGHT: every TALL piece carries a steady lift (0.07: the park is a night level) and steady lit portholes and lamps; it
stands 24 m or more from the centre line by the table. The LOW pieces (WobblyMirrors, SpringBench, GearBed) and the
fuel pumps are unlit. Nothing blinks. All of it is our own: no real brand, sign, word or likeness.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'
TIN, TIN_D, RED, GOLD, PURPLE, CYAN = '#a8b4d0', '#7f8aa6', '#ff2d3c', '#ffc93c', '#3a2f6a', '#2ee6ff'
NAVY, WHITE, SPRAY, CREAM, PLUM = '#2b2350', '#fff6ea', '#dff6ff', '#fff1d6', '#4a3a78'
WIN, LAMP, LIFT = 0.6, 1.2, 0.07          # a lit porthole, a lamp, the steady lift of a tall piece
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
BLD = dict(grow=(40, 40, 40), grow_down=(40, 40, 0))


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def lift(sc, g=LIFT):
    """Every colour of the piece gets a steady glow of at least g (a tin toy under the park's lights)."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=LIFT):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


def rot_pt(l, pitch=0.0, yaw=0.0, roll=0.0):
    """A local vector turned as the engine turns a primitive given rot=(pitch, yaw, roll)."""
    p, y, r = math.radians(pitch), math.radians(yaw), math.radians(roll)
    cp, sp, cy, sy, cr, sr = math.cos(p), math.sin(p), math.cos(y), math.sin(y), math.cos(r), math.sin(r)
    X = (cp * cy, cp * sy, sp)
    Y = (sr * sp * cy - cr * sy, sr * sp * sy + cr * cy, -sr * cp)
    Z = (-(cr * sp * cy + sr * sy), cy * sr - cr * sp * sy, cr * cp)
    return tuple(l[0] * X[i] + l[1] * Y[i] + l[2] * Z[i] for i in range(3))


def add(a, b, k=1.0):
    return (a[0] + b[0] * k, a[1] + b[1] * k, a[2] + b[2] * k)


def drum(g, col, x, y, z, r, h, steps=10, seams=(), seam=TIN_D):
    """A tin drum; seams: heights (from its base) of rolled seams standing proud."""
    g.cyl(col, (x, y, z), r, h, steps=steps)
    for s in seams:
        g.cyl(seam, (x, y, z + s - 8), r + 8, 16, steps=steps)


def dome(g, col, x, y, z, r, h, steps=10, rings=3, glow=0.0):
    pr, pz = r, 0.0
    for k in range(1, rings + 1):
        a = math.pi / 2.0 * k / rings
        nr, nz = max(r * math.cos(a), r * 0.05), h * math.sin(a)
        g.cyl(col, (x, y, z + pz), pr, nz - pz, steps=steps, top_r=nr, glow=glow)
        pr, pz = nr, nz


def aerial(g, x, y, z, h=180.0, ball=RED, r=24.0, glow=0.0):
    """The aerial ball every tin roof ends in."""
    g.cyl(TIN_D, (x, y, z), 5, h, steps=5)
    g.sphere(ball, (x, y, z + h + r * 0.7), r, steps=(4, 6), glow=glow)


def porthole(g, p, r, d=(0, -1, 0), glow=WIN, rim=TIN_D, glass=CYAN):
    """A porthole on a wall at p, looking along d."""
    rod(g, rim, add(p, d, -16), add(p, d, 8), r, steps=8)
    rod(g, glass, add(p, d, 8), add(p, d, 13), r * 0.68, steps=6, glow=glow)


def ring_pt(x, y, r, deg, z):
    """A point on a drum's wall, deg round from the front (-Y) towards +X, and the way out of the wall there."""
    a = math.radians(deg)
    d = (math.sin(a), -math.cos(a), 0.0)
    return (x + d[0] * r, y + d[1] * r, z), d


def fins(sc, x, y, z, r0, w, h, n=4, col=RED, t=24.0, turn=45.0):
    """n swept fins round (x, y), standing on z, from r0 out to r0 + w, h tall: the toy's buttresses."""
    for k in range(n):
        g = sc.still((x, y, z), turn + k * 360.0 / n)
        g.side(col, ccw([(r0, 0), (r0 + w, 0), (r0 + w * 0.3, h * 0.5), (r0, h)]), -t / 2.0, t / 2.0)
    return sc.still()


def key(sc, at, R, sx=1, axis='x', col=GOLD, rate=0.06, swing=170.0, phase=0.0):
    """THE BUTTERFLY KEY: a shaft out of a wall at `at` along sx * X (or Y), two round wings with a hole each, R cm
    from the shaft to a wing's tip. It turns slowly one way and back, like a toy being wound."""
    x, y, z = at
    d = (float(sx), 0.0, 0.0) if axis == 'x' else (0.0, float(sx), 0.0)
    k = sc.part(P.motion(swing=swing, pivot=(float(x), float(y), float(z)), axis=(0.0 if axis == 'x' else 90.0), rate=rate, phase=phase))
    L = R * 0.55
    tip = add(at, d, L)
    rod(k, TIN_D, add(at, d, -10), tip, max(R * 0.08, 6.0), steps=6)
    t = max(R * 0.1, 8.0)
    for s in (-1, 1):
        c = (tip[0], tip[1], tip[2] + s * R * 0.52)
        rod(k, col, add(c, d, -t / 2.0), add(c, d, t / 2.0), R * 0.48, steps=8)
        rod(k, PURPLE, add(c, d, -t / 2.0 - 3), add(c, d, t / 2.0 + 3), R * 0.2, steps=6)
    k.box(col, tip, (t * 1.5, R * 0.3, R * 0.3) if axis == 'x' else (R * 0.3, t * 1.5, R * 0.3))


def awning(g, x, y, z, w, d, n=4, cols=(RED, WHITE), slope=-20.0):
    """A striped awning out from a wall at y (towards -Y), its top edge at z."""
    for k in range(n):
        g.box(cols[k % len(cols)], (x - w / 2.0 + (k + 0.5) * w / n, y - d / 2.0, z - d * 0.18), (w / n, d, 8), rot=(0, 0, slope))


def star_pts(cx, cz, R, r=0.44, n=5):
    pts = []
    for k in range(2 * n):
        a = math.pi / 2.0 + k * math.pi / n
        rr = R if k % 2 == 0 else R * r
        pts.append((cx + math.cos(a) * rr, cz + math.sin(a) * rr))
    return ccw(pts)


def moon_pts(cx, cz, R, n=7):
    """A crescent, its horns to the right."""
    pts = []
    for k in range(n + 1):
        a = math.radians(60 + 240.0 * k / n)
        pts.append((cx + math.cos(a) * R, cz + math.sin(a) * R))
    for k in range(n - 1, 0, -1):
        a = math.radians(60 + 240.0 * k / n)
        pts.append((cx + R * 0.42 + math.cos(a) * R * 0.74, cz + math.sin(a) * R * 0.74))
    return ccw(pts)


# ------------------------------------------------------------------------------------------ the buildings of Rocket Land
def rocket_garage(sc):
    """BUILDING. The rocket garage: a tin drum lying on its side for a hangar, 9 m wide and 9.5 deep, a red round-topped
    door, tail fins either side and a swept fin along the ridge (12 m), vents, a key turning on the -X wall."""
    g = sc.still()
    g.box(TIN_D, (0, 0, -40), (860, 760, 220))                              # the bed it lies in (footing to -150)
    rod(g, TIN, (0, -380, 300), (0, 380, 300), 450, steps=12)              # the drum: top 7.5 m
    for y0 in (-392, -130, 114, 378):
        rod(g, TIN_D, (0, y0, 300), (0, y0 + 16, 300), 462, steps=12)      # rolled seams and the two rims
    g.box(RED, (0, -386, 165), (420, 16, 330))                              # the door
    rod(g, RED, (0, -394, 330), (0, -378, 330), 210, steps=10)
    for k in range(3):
        g.box(GOLD, (0, -396, 70 + k * 100), (424, 8, 12))
    g.box(NAVY, (0, -396, 360), (220, 8, 70))
    g.box(CYAN, (0, -399, 360), (190, 6, 46), glow=WIN)                    # the fanlight
    for sx in (-1, 1):
        porthole(g, (sx * 330, -392, 330), 56)
        g.side(RED, ccw([(sx * 425, -100), (sx * 690, -100), (sx * 560, 150), (sx * 425, 540)]), 150, 180)   # tail fins
        g.box(GOLD, (sx * 200, -420, 560), (30, 30, 30), glow=LAMP)        # two steady lamps over the door
    g = sc.still((0, 0, 0), 90.0)                                           # the ridge fin, in the Y-Z plane
    g.side(RED, ccw([(-330, 735), (330, 735), (385, 1200), (130, 905)]), -14, 14)
    g = sc.still()
    for y0 in (-240, 250):
        g.cyl(TIN_D, (150, y0, 720), 30, 80, steps=6)
        g.cyl(GOLD, (150, y0, 800), 52, 16, steps=6, top_r=20)
    aerial(g, -150, -300, 720, 220, RED, 24, glow=LAMP * 0.5)
    g.box(PURPLE, (0, -470, -62), (700, 176, 136))                          # the apron
    for sx in (-1, 1):
        tilt_box(g, GOLD, (sx * 300, -540, 8), (sx * 60, -410, 8), 26, 6)   # chevrons to the door
    key(sc, (-448, 0, 300), 190, sx=-1, phase=0.2)


def clockwork_cafe(sc):
    """BUILDING. The clockwork cafe: three tin drums, one on another (16 m), a red awning ring, a lit look-out band,
    a purple dome and its aerial ball, a cup on a bracket that swings, a 4.6 m key turning on the +X wall."""
    g = sc.still()
    drum(g, TIN, 0, 0, -150, 400, 570, steps=12, seams=(560,))
    drum(g, TIN, 0, 0, 420, 350, 380, steps=12, seams=(372,))
    drum(g, TIN_D, 0, 0, 800, 300, 280, steps=12)
    g.cyl(CYAN, (0, 0, 890), 304, 110, steps=12, glow=WIN)                 # the look-out's glazing
    g.cyl(GOLD, (0, 0, 1080), 332, 26, steps=12)
    dome(g, PURPLE, 0, 0, 1106, 312, 250, steps=12)
    aerial(g, 0, 0, 1348, 190, RED, 26, glow=LAMP * 0.5)
    g.cyl(RED, (0, 0, 300), 520, 90, steps=12, top_r=404)                  # the awning ring
    g.cyl(WHITE, (0, 0, 286), 524, 16, steps=12)
    g.box(GOLD, (0, -394, 155), (190, 16, 310))
    g.box(NAVY, (0, -400, 150), (150, 16, 290))
    g.box(CYAN, (0, -410, 215), (90, 6, 80), glow=WIN)
    for deg in (-42, 0, 42):
        p, d = ring_pt(0, 0, 350, deg, 610)
        porthole(g, p, 58, d)
    for deg in (-128, 128):
        p, d = ring_pt(0, 0, 350, deg, 610)
        porthole(g, p, 58, d)
    g.cyl(TIN_D, (-170, 130, 1150), 22, 210, steps=6)                       # the stovepipe
    tilt_box(g, TIN_D, (-170, 130, 1350), (-230, 130, 1410), 40, 40)
    g.cyl(GOLD, (-240, 130, 1400), 44, 30, steps=6, top_r=12)
    g.box(TIN_D, (-250, -440, 486), (12, 130, 12))                         # the sign's bracket: a cup, no words
    c = sc.part(P.motion(swing=7, pivot=(-250, -480, 486), axis=0.0, rate=0.3, phase=0.4))
    c.box(TIN_D, (-250, -480, 462), (6, 6, 44))
    c.cyl(WHITE, (-250, -480, 388), 34, 54, steps=8, top_r=44)
    c.cyl(GOLD, (-250, -480, 378), 60, 10, steps=8)
    c.box(WHITE, (-200, -480, 416), (24, 8, 30))
    fins(sc, 0, 0, -120, 384, 250, 640, n=4, turn=45.0)
    key(sc, (346, 0, 610), 230, sx=1)


def control_tower(sc):
    """BUILDING. The control tower: a finned base, a seamed tin shaft, a lit cab under a purple cap (19 m) and a dish
    that turns slowly from side to side on top (22 m)."""
    g = sc.still()
    g.box(TIN_D, (0, 0, 60), (440, 440, 420))
    g.box(GOLD, (0, 0, 280), (470, 470, 20))
    drum(g, TIN, 0, 0, 290, 150, 1110, steps=10, seams=(280, 560, 840))
    g.cyl(TIN_D, (0, 0, 1400), 160, 130, steps=10, top_r=330)
    g.cyl(GOLD, (0, 0, 1530), 372, 16, steps=10)
    g.cyl(NAVY, (0, 0, 1546), 320, 60, steps=10)
    g.cyl(CYAN, (0, 0, 1606), 326, 120, steps=10, glow=WIN)
    g.cyl(TIN, (0, 0, 1726), 352, 30, steps=10)
    g.cyl(PURPLE, (0, 0, 1756), 352, 150, steps=10, top_r=70)
    aerial(g, -130, 0, 1840, 260, RED, 22, glow=LAMP * 0.5)
    g.box(RED, (0, -224, 125), (150, 12, 250))
    g.box(NAVY, (0, -229, 120), (110, 12, 220))
    g.box(CYAN, (0, -236, 180), (60, 6, 50), glow=WIN)
    for z in (700, 1080):
        porthole(g, (0, -150, z), 46)
    for k in range(5):
        a = math.radians(k * 72 + 18)
        g.box(GOLD, (math.cos(a) * 364, math.sin(a) * 364, 1574), (10, 10, 60))   # the walkway's posts
    fins(sc, 0, 0, -120, 205, 250, 860, n=4, turn=45.0)
    d = sc.part(P.motion(swing=35, pivot=(60.0, 0.0, 1906.0), axis=P.UP, rate=0.07))
    d.cyl(TIN_D, (60, 0, 1880), 14, 140, steps=6)
    d.cyl(WHITE, (60, 0, 2020), 26, 70, steps=10, top_r=170, rot=(0, 0, -55))
    ax = rot_pt((0, 0, 1), roll=-55)
    rod(d, GOLD, (60, 0, 2020), add((60, 0, 2020), ax, 160), 6, steps=5)
    d.box(RED, add((60, 0, 2020), ax, 166), (22, 22, 22))
    d.box(TIN_D, (60, 50, 2000), (40, 60, 40))


def planetarium(sc):
    """BUILDING. The tin planetarium: a low seamed drum under a 9 m purple dome set with gold stars that glow (9 m),
    a lit lantern and an aerial ball (11.5 m), a round-roofed porch, a key turning on the -X wall."""
    g = sc.still()
    drum(g, TIN, 0, 0, -150, 440, 480, steps=12, seams=(300, 472))
    g.cyl(GOLD, (0, 0, 330), 462, 22, steps=12)
    dome(g, PURPLE, 0, 0, 352, 440, 440, steps=12, rings=4)
    g.cyl(TIN_D, (0, 0, 776), 70, 54, steps=8)
    g.cyl(CYAN, (0, 0, 830), 60, 70, steps=8, glow=LAMP)
    g.cyl(GOLD, (0, 0, 900), 82, 50, steps=8, top_r=10)
    aerial(g, 0, 0, 940, 150, RED, 20)
    for (a, e) in ((-52, 24), (-22, 50), (14, 30), (44, 56), (64, 20), (-74, 50), (0, 72), (-100, 28), (100, 40)):
        ca, ce = math.radians(a), math.radians(e)
        dv = (math.sin(ca) * math.cos(ce), -math.cos(ca) * math.cos(ce), math.sin(ce))
        g.box(GOLD, add((0, 0, 352), dv, 440), (36, 36, 36), rot=(45, a, 45), glow=LAMP * 0.6)
    g.box(TIN_D, (0, -470, 80), (260, 170, 360))                            # the porch
    rod(g, RED, (0, -558, 260), (0, -390, 260), 130, steps=10)
    g.box(NAVY, (0, -557, 110), (120, 8, 200))
    g.box(CYAN, (0, -560, 232), (70, 6, 34), glow=WIN)
    for deg in (-62, -34, 34, 62):
        p, d = ring_pt(0, 0, 440, deg, 200)
        porthole(g, p, 52, d)
    fins(sc, 0, 0, -120, 424, 150, 420, n=4, turn=45.0)
    key(sc, (-438, 0, 180), 150, sx=-1, phase=0.6)


def box_flats(sc):
    """BUILDING. The box flats: a riveted tin box of four storeys (13 m; 16.5 m with its tank and dome), seams at
    every floor, portholes in rows, a striped awning, a balcony, washing on a line across the roof that sways, tail
    fins on both side walls and a key turning on the +X wall."""
    g = sc.still()
    W, D = 760.0, 640.0
    g.box(TIN, (0, 0, 575), (W, D, 1450))
    for k in range(1, 5):
        g.box(TIN_D, (0, 0, 325 * k), (W + 16, D + 16, 16))
    g.box(PURPLE, (0, 0, 1340), (W - 80, D - 80, 64))
    drum(g, TIN_D, -190, 70, 1372, 110, 150, steps=8)                       # the water tank
    g.cyl(RED, (-190, 70, 1522), 122, 60, steps=8, top_r=12)
    dome(g, GOLD, 200, 40, 1372, 150, 150, steps=8, rings=2)
    aerial(g, 200, 40, 1516, 130, RED, 20, glow=LAMP * 0.5)
    for k in (2, 3):                                                        # the two top floors: portholes
        for x in (-240, 0, 240):
            porthole(g, (x, -D / 2.0, 325 * k + 170), 56)
    for x in (-240, 0, 240):                                                # the first floor: shuttered windows
        g.box(CYAN, (x, -D / 2.0 - 2, 500), (90, 10, 130), glow=WIN)
        for s in (-1, 1):
            g.box(RED, (x + s * 62, -D / 2.0 - 3, 500), (30, 10, 134))
    g.box(GOLD, (-90, -D / 2.0 - 2, 150), (170, 12, 300))
    g.box(NAVY, (-90, -D / 2.0 - 6, 145), (130, 12, 270))
    awning(g, -90, -D / 2.0, 318, 300, 150)
    g.box(CYAN, (210, -D / 2.0 - 2, 170), (190, 10, 150), glow=WIN)        # the shop window
    g.box(GOLD, (210, -D / 2.0 - 4, 84), (210, 14, 16))
    g.box(TIN_D, (0, -D / 2.0 - 34, 660), (320, 68, 10))                    # the balcony
    g.box(GOLD, (0, -D / 2.0 - 64, 712), (320, 8, 8))
    for x in (-150, 0, 150):
        g.box(GOLD, (x, -D / 2.0 - 64, 686), (8, 8, 50))
    for sx in (-1, 1):
        g.side(RED, ccw([(sx * 375, -100), (sx * 525, -100), (sx * 430, 400), (sx * 375, 830)]), -215, -190)
        g.box(TIN_D, (sx * 330, -240, 1437), (10, 10, 130))                 # the washing line's posts
    g.box(TIN_D, (0, -240, 1496), (670, 5, 5))
    w = sc.part(P.motion(swing=11, pivot=(0.0, -240.0, 1496.0), axis=0.0, rate=0.32, phase=0.1))
    for k, col in enumerate((WHITE, RED, CYAN, GOLD, WHITE, PLUM)):
        w.box(col, (-262 + k * 105, -240, 1448 - (k % 2) * 8), (74, 5, 84 + (k % 2) * 16))
    key(sc, (382, 110, 830), 210, sx=1, phase=0.35)


# ------------------------------------------------------------------------------------------ the crooked town (from 8.12 km)
def crooked_house(sc):
    """BUILDING. The crooked house: four tin floors, every one leaning another way (15 m), a pyramid cap gone over,
    lit windows out of line, a door out of true, a stair across the front that stops in the air, a vane, a key on +X."""
    g = sc.still()
    lean = ((0, 0, 0), (-7, 5, 30), (8, -6, -25), (-9, 7, 35))               # pitch, yaw, sideways
    z = 0.0
    top = (0.0, 0.0, 0.0)
    for k in range(4):
        w, d, h = 640.0 - 50 * k, 560.0 - 40 * k, 360.0
        pt, yw, ox = lean[k]
        col = TIN if k % 2 == 0 else TIN_D
        if k == 0:
            g.box(col, (0, 0, 105), (w, d, 510))
            c = (0.0, 0.0, 180.0)
        else:
            c = (ox, 0.0, z + h / 2.0 - 30)
            g.box(col, c, (w, d, h + 90), rot=(pt, yw, 0))
            g.box(GOLD if k % 2 else RED, add(c, rot_pt((0, 0, -h / 2.0 + 6), pt, yw)), (w + 18, d + 18, 16), rot=(pt, yw, 0))
        for wx in ((-150, 130) if k % 2 == 0 else (-40, 170)):
            if k == 0 and wx < 0:
                continue
            p = add(c, rot_pt((wx, -d / 2.0 - 1, 20 if k else 40), pt, yw))
            g.box(CYAN, p, (84, 14, 120), rot=(pt + (9 if wx > 0 else -11), yw, 0), glow=WIN)
            g.box(RED, add(p, rot_pt((0, -3, -70), pt, yw)), (110, 16, 12), rot=(pt, yw, 0))
        top = add(c, rot_pt((0, 0, h / 2.0 + 40), pt, yw))
        z += h - 14
    g.cyl(PURPLE, top, 400, 280, steps=4, top_r=24, rot=(-16, 52, 0))       # the cap, gone over
    tip = add(top, rot_pt((0, 0, 280), -16, 52))
    tilt_box(g, TIN_D, tip, add(tip, (40, 0, 150)), 8, 8)
    g.sphere(GOLD, add(tip, (46, 0, 172)), 24, steps=(4, 6), glow=LAMP * 0.4)
    g.box(RED, (-170, -284, 155), (170, 14, 310), rot=(6, 0, 0))            # the door, out of true
    g.box(NAVY, (-170, -290, 150), (130, 14, 280), rot=(6, 0, 0))
    g.box(GOLD, (-128, -300, 150), (14, 10, 14), glow=LAMP * 0.5)
    for k in range(10):                                                     # the stair that goes nowhere
        g.box(RED if k % 2 else GOLD, (-250 + k * 58, -340, 26 + k * 34), (62, 104, 16))
        g.box(TIN_D, (-250 + k * 58, -340, (26 + k * 34) / 2.0 - 30), (14, 14, 26 + k * 34 + 60))
    g.box(GOLD, (300, -388, 392), (10, 10, 90))
    tilt_box(g, GOLD, (-250, -388, 96), (300, -388, 434), 8, 8)
    g.cyl(TIN_D, (210, 120, 1280), 24, 220, steps=6, rot=(14, 0, 0))        # a bent chimney
    v = sc.part(P.motion(swing=28, pivot=(-200.0, 100.0, 1400.0), axis=P.UP, rate=0.12))
    v.cyl(TIN_D, (-200, 100, 1290), 6, 170, steps=5)
    v.box(GOLD, (-200, 100, 1460), (150, 8, 12))
    v.box(RED, (-140, 100, 1460), (40, 8, 46))
    key(sc, (318, 30, 560), 200, sx=1, phase=0.5)


def mirror_hall(sc):
    """BUILDING. The hall of mirrors: a tin box 9 m wide behind a front folded in zigzag mirror panels that glow
    softly, a red sawtooth crown with a lamp on every tooth (8 m), two gold domes, a great hand mirror rocking on
    the roof (10.5 m), a key turning on the -X wall."""
    g = sc.still()
    W, D, H = 900.0, 520.0, 520.0
    g.box(TIN_D, (0, 40, (H - 150) / 2.0), (W, D, H + 150))                 # the front wall at y = -220
    g.box(PURPLE, (0, -258, -30), (W, 150, 100))
    n = 8
    pw = W / n
    for k in range(n):
        if k in (3, 4):
            continue
        g.box(SPRAY if k % 2 == 0 else CYAN, (-W / 2.0 + (k + 0.5) * pw, -256, 240), (pw / math.cos(math.radians(32)), 10, 420),
              rot=(0, 32 if k % 2 == 0 else -32, 0), glow=(0.25 if k % 2 == 0 else WIN * 0.6))
    g.box(GOLD, (0, -226, 180), (250, 14, 360), rot=(5, 0, 0))              # the door, out of true
    g.box(NAVY, (0, -232, 170), (200, 14, 330), rot=(5, 0, 0))
    awning(g, 0, -236, 420, 280, 130, n=4, cols=(RED, GOLD))
    g.side(RED, ccw([(-470, H - 20), (470, H - 20), (470, H + 90), (360, H + 210), (240, H + 70), (120, H + 250), (0, H + 60),
                     (-130, H + 270), (-250, H + 80), (-370, H + 200), (-470, H + 100)]), -238, -208)
    for (x, zt) in ((360, 210), (120, 250), (-130, 270), (-370, 200)):
        g.box(GOLD, (x, -224, H + zt + 16), (28, 28, 28), glow=LAMP)
    g.box(PURPLE, (0, 40, H + 15), (W + 30, D + 30, 30))
    for sx in (-1, 1):
        dome(g, GOLD, sx * 330, 150, H + 30, 110, 120, steps=8, rings=2)
        aerial(g, sx * 330, 150, H + 146, 110, RED, 18)
        g.side(RED, ccw([(sx * 445, -100), (sx * 560, -100), (sx * 490, 300), (sx * 445, 560)]), -180, -152)
        g.box(SPRAY, (sx * (W / 2.0 + 3), -60, 260), (8, 150, 300), glow=0.25)     # a mirror on each end wall
    m = sc.part(P.motion(swing=7, pivot=(0.0, 60.0, H + 30), axis=0.0, rate=0.2))
    m.box(GOLD, (0, 60, H + 110), (44, 30, 170))
    rod(m, GOLD, (0, 44, H + 370), (0, 76, H + 370), 190, steps=10)
    rod(m, SPRAY, (0, 38, H + 370), (0, 44, H + 370), 160, steps=10, glow=0.35)
    key(sc, (-W / 2.0, 80, 300), 170, sx=-1, phase=0.15)


def crooked_tower(sc):
    """BUILDING. The crooked tower: three tin drums stacked in a zigzag (14 m), gold collars at the knees, a cab
    askew under a cap gone over (19 m), a stair that winds half round the foot and stops, a pennant, a key on -X."""
    g = sc.still()
    g.cyl(TIN_D, (0, 0, -150), 300, 250, steps=10)
    g.cyl(GOLD, (0, 0, 100), 312, 16, steps=10)
    p = (0.0, 0.0, 110.0)
    for (pt, r, h, col) in ((9, 240, 470, TIN), (-12, 205, 450, TIN_D), (10, 175, 430, TIN)):
        d = rot_pt((0, 0, 1), pt)
        g.cyl(col, add(p, d, -40), r, h + 40, steps=10, rot=(pt, 0, 0))
        mid = add(p, d, h * 0.55)
        porthole(g, (mid[0], -r * 0.95, mid[2]), 46)
        p = add(p, d, h)
        g.cyl(GOLD, add(p, d, -8), r + 14, 16, steps=10, rot=(pt, 0, 0))
    c = add(p, (0, 0, 110))
    g.box(TIN_D, c, (420, 380, 250), rot=(-8, 12, 0))                       # the cab
    for wx in (-110, 100):
        g.box(CYAN, add(c, rot_pt((wx, -191, 10), -8, 12)), (110, 12, 110), rot=(-8 + (10 if wx > 0 else -8), 12, 0), glow=WIN)
    t = add(c, rot_pt((0, 0, 120), -8, 12))
    g.cyl(PURPLE, t, 310, 230, steps=8, top_r=16, rot=(-15, 0, 0))
    tip = add(t, rot_pt((0, 0, 232), -15))
    g.sphere(GOLD, add(tip, (0, 0, 20)), 26, steps=(4, 6), glow=LAMP * 0.4)
    for k in range(9):                                                      # the stair that winds up and stops
        a = math.radians(-140 + k * 26)
        g.box(RED if k % 2 else GOLD, (math.sin(a) * 300, -math.cos(a) * 300, 70 + k * 38), (74, 116, 14), rot=(0, math.degrees(a), 0))
    a = math.radians(-140 + 8 * 26)
    g.box(GOLD, (math.sin(a) * 340, -math.cos(a) * 340, 70 + 8 * 38 + 50), (10, 10, 100))
    g.box(RED, (-30, -246, 265), (140, 14, 250), rot=(9, 0, 0))             # the door, out of true
    g.box(NAVY, (-30, -252, 260), (104, 14, 220), rot=(9, 0, 0))
    f = sc.part(P.motion(swing=14, pivot=(tip[0], tip[1], tip[2] + 40), axis=P.UP, rate=0.3))
    f.cyl(TIN_D, (tip[0], tip[1], tip[2] + 30), 5, 150, steps=5)
    f.box(RED, (tip[0] + 70, tip[1], tip[2] + 150), (130, 6, 50))
    key(sc, (-272, 0, 330), 170, sx=-1, phase=0.7)


def prop_store(sc):
    """BUILDING. The back lot's prop store: a ribbed tin shed 9 m wide (6 m), its door half slid open on a warm light,
    a giant tin star (6 m) leaning on the front, painted flats stacked beside it, a ladder, vents, a vane that swings."""
    g = sc.still()
    W, D, H = 900.0, 600.0, 430.0
    g.box(TIN_D, (0, 40, (H - 150) / 2.0), (W, D, H + 150))                 # the front wall at y = -260
    for k in range(9):
        g.box(TIN, (-W / 2.0 + 50 + k * 100, -263, H / 2.0), (22, 8, H))
    g.side(PURPLE, ccw([(-W / 2.0 - 30, H), (W / 2.0 + 30, H), (0, H + 170)]), -290, 370)
    g.box(NAVY, (-190, -268, 170), (330, 10, 340))
    g.box(GOLD, (-290, -271, 160), (120, 6, 300), glow=0.4)                 # the light inside
    g.box(TIN, (-90, -278, 170), (190, 10, 340))                            # the door, half open
    g.box(RED, (-90, -284, 170), (150, 6, 16))
    g.box(GOLD, (-170, -276, 352), (540, 8, 14))
    g.box(GOLD, (-190, -292, 392), (30, 30, 30), glow=LAMP)                # one lamp over the door
    g.box(RED, (360, -264, 300), (160, 8, 180))                             # a patch
    for x in (-250, 200):
        g.cyl(TIN_D, (x, 60, H + 60), 34, 110, steps=6)
        g.cyl(GOLD, (x, 60, H + 170), 56, 18, steps=6, top_r=18)
    s = sc.still((250, -500, 0))                                            # the star, leaning back on the wall
    s.side(GOLD, star_pts(0, 300, 310), -16, 16, rot=(0, 0, 22))
    s.side(RED, star_pts(0, 300, 130), -24, -16, rot=(0, 0, 22))
    f = sc.still((-330, -400, 0))                                           # flats, leaning
    f.side(CREAM, ccw([(-110, 0), (110, 0), (110, 380), (-110, 380)]), -5, 5, rot=(0, 0, 18))
    f.side(CYAN, ccw([(-60, 0), (130, 0), (130, 300), (-60, 300)]), -17, -7, rot=(0, 0, 18))
    f.side(RED, moon_pts(30, 170, 80), -24, -17, rot=(0, 0, 18))
    g = sc.still()
    for sx in (-1, 1):                                                      # the ladder on the +X end
        tilt_box(g, WHITE, (410 + sx * 28, -330, -20), (410 + sx * 28, -262, 440), 8, 8)
    for k in range(5):
        g.box(WHITE, (410, -322 + k * 14, 50 + k * 86), (60, 6, 6))
    v = sc.part(P.motion(swing=30, pivot=(0.0, 40.0, H + 170), axis=P.UP, rate=0.12, phase=0.3))
    v.cyl(TIN_D, (0, 40, H + 160), 6, 150, steps=5)
    v.box(GOLD, (0, 40, H + 310), (190, 8, 12))
    rod(v, GOLD, (-88, 36, H + 310), (-88, 44, H + 310), 30, steps=8)
    v.box(RED, (96, 40, H + 310), (36, 8, 44))


# ------------------------------------------------------------------------------------------ the props
def spare_key(sc):
    """PROP, TALL (5.3 m). A spare key standing in its pedestal: a tin stand with three fins, a 3.4 m gold butterfly
    key in the socket, turning slowly one way and back."""
    g = sc.still()
    g.cyl(TIN_D, (0, 0, -40), 96, 70, steps=8)
    g.cyl(RED, (0, 0, 30), 86, 22, steps=8)
    g.cyl(TIN, (0, 0, 52), 70, 150, steps=8, top_r=46)
    g.cyl(GOLD, (0, 0, 202), 56, 16, steps=8)
    fins(sc, 0, 0, 0, 56, 66, 150, n=3, turn=90.0, t=16.0)
    k = sc.part(P.motion(swing=60, pivot=(0.0, 0.0, 218.0), axis=P.UP, rate=0.08))
    k.cyl(TIN_D, (0, 0, 210), 13, 180, steps=6)
    k.box(TIN_D, (0, 0, 262), (64, 16, 20))
    k.box(TIN_D, (0, 0, 300), (44, 16, 16))
    for s in (-1, 1):
        rod(k, GOLD, (s * 82, -11, 440), (s * 82, 11, 440), 86, steps=8)
        rod(k, PURPLE, (s * 82, -14, 440), (s * 82, 14, 440), 34, steps=6)
    k.box(GOLD, (0, 0, 400), (74, 22, 64))


def fuel_pump(sc):
    """PROP, TALL (3.9 m). Two tin fuel pumps on an island: a red belt and cap, a dial, a hose, a glass globe on top
    (pale, unlit, not orange)."""
    g = sc.still()
    g.box(TIN_D, (0, 0, -8), (390, 150, 56))
    g.box(RED, (0, -76, 6), (390, 6, 16))
    for (x, s) in ((-95, 1.0), (105, 0.82)):
        h = 210.0 * s
        g.box(TIN, (x, 0, 20 + h / 2.0), (110 * s, 84 * s, h))
        g.box(RED, (x, 0, 20 + h * 0.2), (116 * s, 90 * s, 18 * s))
        g.box(RED, (x, 0, 20 + h + 8 * s), (124 * s, 96 * s, 16 * s))
        rod(g, WHITE, (x, -40 * s, 20 + h * 0.68), (x, -47 * s, 20 + h * 0.68), 30 * s, steps=8)
        g.box(NAVY, (x + 8 * s, -49 * s, 20 + h * 0.72), (26 * s, 5, 6))
        g.box(GOLD, (x, -45 * s, 20 + h * 0.42), (60 * s, 6, 30 * s))
        g.cyl(TIN_D, (x, 0, 20 + h + 16 * s), 16 * s, 50 * s, steps=6)
        g.sphere(SPRAY, (x, 0, 20 + h + 106 * s), 46 * s, steps=(4, 8))
        g.cyl(GOLD, (x, 0, 20 + h + 146 * s), 14 * s, 16 * s, steps=6)
        tilt_box(g, NAVY, (x + 56 * s, -20 * s, 20 + h * 0.8), (x + 84 * s, -34 * s, 20 + h * 0.36), 8, 8)
        tilt_box(g, NAVY, (x + 84 * s, -34 * s, 20 + h * 0.36), (x + 60 * s, -44 * s, 20 + h * 0.56), 8, 8)
        g.box(GOLD, (x + 60 * s, -46 * s, 20 + h * 0.6), (12, 12, 28 * s))


def tin_dog(sc):
    """PROP, TALL (2.9 m). A tin dog on a wheeled board, a statue: seamed body, square head, a pull cord; its ears
    and tail move a little and the small key on its back turns."""
    g = sc.still()
    g.box(RED, (0, 0, 46), (330, 130, 16))
    g.box(TIN_D, (0, 0, 4), (70, 70, 70))
    for x in (-120, 120):
        for y in (-72, 72):
            rod(g, GOLD, (x, y - 8, 38), (x, y + 8, 38), 38, steps=8)
            g.box(TIN_D, (x * 0.8, y * 0.45, 82), (30, 26, 60))
    g.box(TIN, (0, 0, 152), (250, 96, 92))
    g.box(TIN_D, (0, 0, 152), (16, 100, 96))
    g.box(GOLD, (-108, 0, 190), (20, 84, 84), rot=(40, 0, 0))               # the collar
    tilt_box(g, TIN, (-105, 0, 180), (-152, 0, 236), 70, 70)
    g.box(TIN, (-176, 0, 252), (92, 86, 80))
    g.box(TIN_D, (-234, 0, 238), (52, 58, 46))
    g.box(RED, (-262, 0, 250), (14, 24, 18))
    for sy in (-1, 1):
        g.box(NAVY, (-186, sy * 43, 266), (16, 6, 16))
        e = sc.part(P.motion(swing=9, pivot=(-160.0, sy * 44.0, 288.0), axis=0.0, rate=0.4, phase=0.2 + 0.1 * sy))
        e.box(TIN_D, (-160, sy * 48, 256), (46, 8, 66))
    t = sc.part(P.motion(swing=16, pivot=(124.0, 0.0, 192.0), axis=0.0, rate=0.8))
    tilt_box(t, RED, (122, 0, 188), (172, 0, 262), 12, 12)
    tilt_box(g, NAVY, (-165, 0, 54), (-262, 0, 6), 6, 6)                    # the pull cord
    g.box(GOLD, (-270, 0, 6), (30, 30, 8))
    k = sc.part(P.motion(swing=170, pivot=(24.0, 0.0, 196.0), axis=P.UP, rate=0.07))
    k.cyl(TIN_D, (24, 0, 190), 7, 44, steps=5)
    for s in (-1, 1):
        rod(k, GOLD, (24 + s * 25, -5, 258), (24 + s * 25, 5, 258), 25, steps=8)


def signal_dish(sc):
    """PROP, TALL (6.5 m). A signal dish on a tin tripod: red feet, a steady cyan pilot lamp, the dish turning slowly
    from side to side."""
    g = sc.still()
    for k in range(3):
        a = math.radians(90 + k * 120)
        f = (math.cos(a) * 170, math.sin(a) * 170, -30.0)
        tilt_box(g, TIN_D, f, (0, 0, 300), 16, 16)
        g.cyl(RED, f, 30, 46, steps=6)
    g.cyl(TIN, (0, 0, 280), 34, 60, steps=8)
    g.cyl(GOLD, (0, 0, 340), 42, 12, steps=8)
    g.box(CYAN, (0, -36, 310), (20, 8, 20), glow=LAMP)
    g.box(TIN_D, (0, 0, 150), (150, 10, 10))
    d = sc.part(P.motion(swing=40, pivot=(0.0, 0.0, 352.0), axis=P.UP, rate=0.06))
    d.cyl(TIN_D, (0, 0, 352), 12, 80, steps=6)
    d.cyl(WHITE, (0, 0, 432), 30, 90, steps=10, top_r=210, rot=(0, 0, -50))
    ax = rot_pt((0, 0, 1), roll=-50)
    rod(d, TIN_D, (0, 0, 432), add((0, 0, 432), ax, 200), 6, steps=5)
    d.box(RED, add((0, 0, 432), ax, 206), (24, 24, 24))
    d.box(TIN_D, (0, 60, 420), (40, 60, 40))


def wobbly_mirrors(sc):
    """PROP, LOW (1.96 m). Three funhouse mirrors in gold frames on a tin plinth, their glass in two bulges out of
    line; each rocks a little on its foot. Unlit."""
    g = sc.still()
    g.box(TIN_D, (0, 0, -8), (460, 70, 44))
    g.box(RED, (0, -36, 6), (460, 6, 14))
    for x, h, ph in ((-150, 150.0, 0.0), (0, 172.0, 0.35), (150, 140.0, 0.7)):
        m = sc.part(P.motion(swing=4, pivot=(float(x), 0.0, 14.0), axis=0.0, rate=0.3, phase=ph))
        m.box(GOLD, (x, 0, 14 + h / 2.0), (104, 12, h))
        m.box(SPRAY, (x - 8, -6, 14 + h * 0.3), (70, 6, h * 0.42))
        m.box(SPRAY, (x + 8, -6, 14 + h * 0.74), (70, 6, h * 0.38))
        m.box(PURPLE, (x, 0, 14 + h + 5), (60, 14, 10))


def spring_bench(sc):
    """PROP, LOW (1.42 m). A giant coiled spring lying ON ITS SIDE in two red cradles, gold caps on its ends and a
    plank along the top for a seat. Still, unlit."""
    g = sc.still()
    turns, seg, R, L, zc = 6, 6, 52.0, 400.0, 74.0
    pts = []
    for i in range(turns * seg + 1):
        a = 2.0 * math.pi * i / seg
        pts.append((-L / 2.0 + L * i / (turns * seg), math.cos(a) * R, zc + math.sin(a) * R))
    for a, b in zip(pts, pts[1:]):
        tilt_box(g, TIN, a, b, 13, 13)
    for sx in (-1, 1):
        rod(g, GOLD, (sx * (L / 2.0 + 2), 0, zc), (sx * (L / 2.0 + 16), 0, zc), 58, steps=8)
        g.box(RED, (sx * 150, 0, 4), (40, 150, 48))
    g.box(TIN_D, (0, 0, zc + R + 10), (430, 70, 12))


def aerial_lamp(sc):
    """PROP, TALL (7.2 m). A tin lamp standard: a seamed post with gold collars, a cyan globe that glows steadily on
    top under a gold cap and an aerial ball, two smaller globes on a crossbar."""
    g = sc.still()
    g.cyl(TIN_D, (0, 0, -40), 60, 70, steps=8)
    g.cyl(GOLD, (0, 0, 30), 44, 30, steps=8, top_r=22)
    g.cyl(TIN, (0, 0, 30), 18, 470, steps=6)
    for z in (150, 290, 430):
        g.cyl(GOLD, (0, 0, z), 26, 14, steps=6)
    g.box(TIN_D, (0, 0, 470), (270, 14, 14))
    for sx in (-1, 1):
        g.cyl(TIN_D, (sx * 124, 0, 420), 5, 50, steps=5)
        g.cyl(PURPLE, (sx * 124, 0, 414), 36, 14, steps=6, top_r=10)
        g.sphere(CYAN, (sx * 124, 0, 392), 26, steps=(4, 6), glow=LAMP)
    g.cyl(TIN_D, (0, 0, 500), 44, 46, steps=8)
    g.sphere(CYAN, (0, 0, 582), 46, steps=(4, 8), glow=LAMP)
    g.cyl(GOLD, (0, 0, 616), 30, 26, steps=6, top_r=6)
    g.cyl(TIN_D, (0, 0, 640), 5, 60, steps=5)
    g.sphere(RED, (0, 0, 708), 13, steps=(3, 5))


def gear_bed(sc):
    """PROP, LOW (1.9 m). Three clockwork gears standing in a purple bed: the gold one is still, the tin one and the
    small red one turn slowly, one against the other. Unlit."""
    g = sc.still()
    g.box(PURPLE, (0, 0, -6), (420, 90, 48))

    def gear(m, x, zc, r, col, teeth=8, t=26.0):
        rod(m, col, (x, -t / 2.0, zc), (x, t / 2.0, zc), r, steps=10)
        rod(m, PURPLE, (x, -t / 2.0 - 4, zc), (x, t / 2.0 + 4, zc), r * 0.3, steps=6)
        for k in range(teeth):
            a = 2.0 * math.pi * k / teeth
            m.box(col, (x + math.cos(a) * (r + 7), 0, zc + math.sin(a) * (r + 7)), (18, t, 22), rot=(math.degrees(a), 0, 0))

    gear(g, -110, 96, 78, GOLD)
    gear(sc.part(P.motion(swing=180, pivot=(40.0, 0.0, 76.0), axis=90.0, rate=0.05)), 40, 76, 58, TIN)
    gear(sc.part(P.motion(swing=-180, pivot=(146.0, 0.0, 60.0), axis=90.0, rate=0.05)), 146, 60, 40, RED, teeth=6)


def stage_flats(sc):
    """PROP, TALL (4.5 m). The back lot's rack of painted tin flats: an A-frame, a cream flat with a gold crescent
    and a red star, a taller purple one behind with a gold zigzag, a work lamp on the rack. No words."""
    g = sc.still()
    for sx in (-1, 1):
        tilt_box(g, TIN_D, (sx * 232, -120, -30), (sx * 232, 60, 430), 14, 14)
        tilt_box(g, TIN_D, (sx * 232, 220, -30), (sx * 232, 60, 430), 14, 14)
    g.box(TIN_D, (0, 60, 430), (490, 14, 14))
    g.box(TIN_D, (0, 150, 190), (490, 10, 10))
    g.box(GOLD, (150, 52, 450), (30, 30, 26), glow=LAMP)
    f = sc.still((40, -96, 0))                                              # the back flat
    f.side(PURPLE, ccw([(-170, 0), (170, 0), (170, 470), (-170, 470)]), -5, 5, rot=(0, 0, 21))
    f.side(GOLD, ccw([(-150, 380), (-90, 440), (-30, 380), (30, 440), (90, 380), (150, 440), (150, 410), (90, 350), (30, 410),
                      (-30, 350), (-90, 410), (-150, 350)]), -11, -5, rot=(0, 0, 21))
    f = sc.still((-30, -150, 0))                                            # the front flat
    f.side(CREAM, ccw([(-200, 0), (200, 0), (200, 400), (-200, 400)]), -5, 5, rot=(0, 0, 21))
    f.side(GOLD, moon_pts(-80, 250, 100), -12, -5, rot=(0, 0, 21))
    f.side(RED, star_pts(110, 280, 66), -12, -5, rot=(0, 0, 21))
    for k in range(3):
        f.side(CYAN, ccw([(-180, 30 + k * 40), (180, 30 + k * 40), (180, 50 + k * 40), (-180, 50 + k * 40)]), -11, -5, rot=(0, 0, 21))
    sc.still()


def nowhere_stair(sc):
    """PROP, TALL (5.9 m). A stair to nowhere: nine tin steps with red nosings climb to a landing where a door frame,
    out of true, opens on the air; the door swings slowly; a gold rail and a steady cyan lamp."""
    g = sc.still()
    for k in range(9):
        x, z = -250 + k * 52, 20 + k * 36
        y = (k % 2) * 16 - 8
        g.box(TIN if k % 2 else TIN_D, (x, y, (z - 30) / 2.0), (56, 120, z + 30))
        g.box(RED, (x, y, z + 4), (58, 124, 8))
    g.box(PURPLE, (275, 0, 157), (130, 136, 374))                           # the landing: top 344
    g.box(GOLD, (275, 0, 348), (134, 140, 8))
    for x in (226, 326):
        g.box(RED, (x, -62, 462), (14, 14, 230), rot=(-5 if x < 300 else 4, 0, 0))
    g.box(RED, (276, -62, 582), (130, 14, 16), rot=(-6, 0, 0))
    tilt_box(g, GOLD, (-250, 60, 96), (220, 60, 420), 8, 8)
    for k in (0, 3, 6):
        g.box(GOLD, (-250 + k * 52, 60, 20 + k * 36 + 40), (8, 8, 80))
    g.cyl(TIN_D, (318, 56, 344), 8, 150, steps=5)
    g.sphere(CYAN, (318, 56, 512), 20, steps=(4, 6), glow=LAMP)
    d = sc.part(P.motion(swing=24, pivot=(232.0, -62.0, 344.0), axis=P.UP, rate=0.15))
    d.box(NAVY, (278, -62, 452), (88, 8, 204))
    d.box(GOLD, (312, -68, 446), (10, 8, 10))


# ------------------------------------------------------------------------------------------ the hero
def observatory(sc):
    """HERO. THE WIND-UP OBSERVATORY: a seamed tin drum 32 m across on eight red buttress fins, a lit gallery, a tin
    dome (41 m) with its slit open, the telescope's barrel out of the slit (56 m) sweeping slowly, an aerial ball, a
    ring of portholes and lit windows, and a butterfly key 16 m across turning on each side wall."""
    g = sc.still()
    R = 1600.0
    drum(g, TIN, 0, 0, -300, R, 2500, steps=18, seams=(1000, 1700, 2400))
    g.cyl(GOLD, (0, 0, 2200), R + 90, 40, steps=18)
    g.cyl(TIN_D, (0, 0, 2240), R + 30, 110, steps=18)
    g.cyl(CYAN, (0, 0, 2262), R + 34, 50, steps=18, glow=WIN)
    dome(g, TIN, 0, 0, 2350, R + 20, 1750, steps=18, rings=5)
    g.cyl(GOLD, (0, 0, 2350), R + 40, 30, steps=18)
    g.cyl(TIN_D, (-260, 320, 4000), 16, 720, steps=6)
    g.sphere(RED, (-260, 320, 4790), 90, steps=(5, 8), glow=LAMP)
    for k in range(10):
        p, d = ring_pt(0, 0, R, k * 36.0, 1350)
        porthole(g, p, 130, d)
        p, d = ring_pt(0, 0, R - 6, 18 + k * 36.0, 600)
        g.box(CYAN, p, (150, 34, 230), rot=(0, 18 + k * 36.0, 0), glow=WIN)
        p, d = ring_pt(0, 0, R + 74, 18 + k * 36.0, 2262)
        g.box(GOLD, p, (40, 40, 44), glow=LAMP)                             # lamps round the gallery
    for k in range(18):
        p, d = ring_pt(0, 0, R + 6, k * 20.0, 1820)
        g.box(TIN_D, p, (30, 30, 30))                                       # rivets under the top seam
    g.box(GOLD, (0, -R + 10, 110), (440, 70, 820))                          # the door
    rod(g, GOLD, (0, -R - 25, 520), (0, -R + 45, 520), 220, steps=10)
    g.box(NAVY, (0, -R - 14, 100), (320, 40, 800))
    rod(g, NAVY, (0, -R - 34, 500), (0, -R + 6, 500), 160, steps=10)
    g.box(CYAN, (0, -R - 36, 520), (170, 10, 120), glow=WIN)
    g.box(PURPLE, (0, -R - 110, -100), (620, 220, 280))
    for sx in (-1, 1):
        rod(g, GOLD, (sx * (R - 20), 0, 1150), (sx * (R + 18), 0, 1150), 170, steps=10)   # the winding plates
    s = sc.still((0, 0, 0), 25.0)                                           # the slit, 25 degrees round from the front
    for e in (24, 42, 60, 78):
        ce, se = math.cos(math.radians(e)), math.sin(math.radians(e))
        s.box(PURPLE, (0, -ce * (R + 20) * 0.99, 2350 + se * 1750 * 0.99), (470, 70, 640), rot=(0, 0, e))
    fins(sc, 0, 0, -250, R - 14, 520, 1950, n=8, turn=22.5, t=70.0)
    t = sc.part(P.motion(swing=5, pivot=(0.0, 0.0, 2900.0), axis=P.UP, rate=0.03), (0, 0, 0), 25.0)
    c = (0.0, 0.0, 2900.0)
    d = (0.0, -math.cos(math.radians(58)), math.sin(math.radians(58)))
    rod(t, TIN_D, add(c, d, 900), add(c, d, 2300), 250, steps=12)
    rod(t, TIN, add(c, d, 2300), add(c, d, 2950), 300, steps=12)
    rod(t, GOLD, add(c, d, 2250), add(c, d, 2330), 318, steps=12)
    rod(t, GOLD, add(c, d, 2890), add(c, d, 2960), 330, steps=12)
    rod(t, CYAN, add(c, d, 2950), add(c, d, 2966), 262, steps=12, glow=WIN)
    f0 = add(add(c, d, 1700), (330, 0, 0))
    rod(t, GOLD, f0, add(f0, d, 800), 60, steps=6)                          # the finder
    key(sc, (R, 0, 1150), 800, sx=1, rate=0.045)
    key(sc, (-R, 0, 1150), 800, sx=-1, rate=0.045, phase=0.5)


JOBS = [
    ('SM_S1D4_RocketGarage', lit(rocket_garage), BLD),
    ('SM_S1D4_ClockworkCafe', lit(clockwork_cafe), BLD),
    ('SM_S1D4_ControlTower', lit(control_tower), BLD),
    ('SM_S1D4_Planetarium', lit(planetarium), BLD),
    ('SM_S1D4_BoxFlats', lit(box_flats), BLD),
    ('SM_S1D4_CrookedHouse', lit(crooked_house), BLD),
    ('SM_S1D4_MirrorHall', lit(mirror_hall), BLD),
    ('SM_S1D4_CrookedTower', lit(crooked_tower), BLD),
    ('SM_S1D4_PropStore', lit(prop_store), BLD),
    ('SM_S1D4_SpareKey', lit(spare_key, 0.05), dict(grow=(20, 160, 10), grow_down=(20, 160, 0))),
    ('SM_S1D4_FuelPump', fuel_pump, STILL),
    ('SM_S1D4_TinDog', lit(tin_dog), dict(grow=(20, 30, 20), grow_down=(20, 30, 0))),
    ('SM_S1D4_SignalDish', lit(signal_dish), dict(grow=(80, 80, 20), grow_down=(80, 80, 0))),
    ('SM_S1D4_WobblyMirrors', wobbly_mirrors, dict(grow=(5, 20, 5), grow_down=(5, 20, 0))),
    ('SM_S1D4_SpringBench', spring_bench, STILL),
    ('SM_S1D4_AerialLamp', lit(aerial_lamp), STILL),
    ('SM_S1D4_GearBed', gear_bed, dict(grow=(8, 8, 8), grow_down=(8, 8, 0))),
    ('SM_S1D4_StageFlats', lit(stage_flats), STILL),
    ('SM_S1D4_NowhereStair', lit(nowhere_stair), dict(grow=(20, 100, 10), grow_down=(20, 100, 0))),
    ('SM_S1D4_Observatory', lit(observatory), dict(far=True, grow=(250, 250, 100), grow_down=(250, 250, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
