"""Volcano, Stage 1, builder s1d2: district 2 "Fire and Brimstone" (3.76 to 8.34 km), STORMWATCH: the stations that
catch the ash cloud's lightning in the black glass canyon. Riveted metal, tall and narrow, banded with rings: drum
halls with ribbed caps, ring-insulator towers, ball tanks on legs, capacitor sheds with saw-cut skylines.
/Game/World/Volcano/SM_S1D2_*. Run: import ft_world_volcano_s1d2 as K; K.run()

Palette: violet (bodies), magenta (bands, doors), lilac steel (frames), white (caps), cyan (a STEADY glow, only on
pieces whose every rule stands 24 m or more out: the buildings, the tall props, the hero), the kit's CONC for plinths
and PANE for glass and obsidian. No orange, no red, no black body.
Every piece: front faces -Y, long axis X, stands on z = 0 with a footing below it. Boxes, cylinders and spheres only.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Volcano'
V, M, L, W, C = '#7a4cff', '#ff4fb8', '#b9b3d6', '#f2f0ff', '#3fe0ff'
CONC, PANE = '#9a98a8', '#14101a'
GLOW = 0.5
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))            # no moving part: true bounds
SLIM = dict(grow=(25, 10, 10), grow_down=(25, 10, 0))         # a pennant or a vane that swings inside its own depth
ROOF = dict(grow=(40, 40, 20), grow_down=(40, 40, 0))         # a vane turning on a roof


def seg(m, col, a, b, r0, r1, steps=8, glow=0.0):
    """A tapered cylinder from a (radius r0) to b (radius r1): ft_desert.rod with a second radius."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    ln = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    m.cyl(col, a, r0, ln, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=r1)


def ribbed_cap(g, z, r, squash=0.9, ribs=3, col=W, rib=L, steps=(6, 12)):
    """A ribbed half-sphere cap sitting on a drum whose top is at z: the lower half is inside the drum."""
    g.sphere(col, (0, 0, z), r, scale=(1, 1, squash), steps=steps)
    for k in range(ribs):
        g.sphere(rib, (0, 0, z), r * 1.035, scale=(1, 0.05, squash * 1.02), steps=(5, 10), rot=(0, k * 180.0 / ribs, 0))
    return z + r * squash


def wall_boxes(g, col, r, z, xs, size, glow=0.0):
    """Flat panels on the front of a drum of radius r (windows, doors): each turned to lie on the curve."""
    for x in xs:
        a = math.asin(max(-1.0, min(1.0, x / float(r))))
        g.box(col, (r * math.sin(a), -r * math.cos(a), z), size, rot=(0, math.degrees(a), 0), glow=glow)


# ------------------------------------------------------------------------------------------ buildings
def drum_hall(sc):
    """BUILDING 1, the drum hall: a riveted violet drum 6.8 m across and 12 m tall under a ribbed white cap, five
    ring bands, three rows of windows (the middle row lit cyan), a magenta door under an awning, a meter cupboard,
    a vane turning slowly on the finial. 17 m."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -150), 380, 190, steps=12)
    g.cyl(V, (0, 0, 40), 340, 1160, steps=12)
    for k, z in enumerate((40, 330, 620, 910, 1170)):
        g.cyl(M if k % 2 == 0 else L, (0, 0, z), 352, 36, steps=12)
    top = ribbed_cap(g, 1200, 335)
    g.cyl(L, (0, 0, top - 20), 14, 190, steps=6)
    g.sphere(C, (0, 0, top + 190), 28, steps=(4, 6), glow=GLOW)
    wall_boxes(g, M, 342, 170, (0,), (170, 16, 260))                    # the door
    g.box(W, (0, -348, 312), (220, 14, 28))
    tilt_box(g, M, (0, -338, 350), (0, -480, 300), 230, 10)             # its awning
    for x in (-105, 105):
        g.box(L, (x, -470, 140), (8, 8, 330))
    wall_boxes(g, PANE, 344, 480, (-170, 0, 170), (76, 12, 110))
    wall_boxes(g, C, 344, 770, (-170, 0, 170), (76, 12, 110), glow=GLOW)
    wall_boxes(g, PANE, 344, 1050, (-170, 0, 170), (76, 12, 90))
    for x in (-250, -85, 85, 250):                                       # a row of rivet heads under the top band
        wall_boxes(g, W, 356, 1150, (x,), (14, 8, 14))
    g.box(L, (-392, -40, 110), (110, 150, 300))                          # the meter cupboard on its flank
    g.box(M, (-392, -40, 268), (124, 164, 16))
    g.box(W, (-392, -118, 150), (60, 6, 60), rot=(45, 0, 0))
    g.cyl(L, (250, 190, 1150), 20, 330, steps=6)                         # a vent pipe with a mushroom cap
    g.cyl(W, (250, 190, 1480), 52, 30, steps=8, top_r=22)
    f = sc.part(P.motion(swing=28, pivot=(0, 0, top + 120), axis=P.UP, rate=0.12, phase=0.2))
    f.box(M, (48, 0, top + 120), (110, 6, 36))
    f.box(W, (-40, 0, top + 120), (44, 6, 14))


def insulator_tower(sc):
    """BUILDING 2, the insulator tower: six white ring sheds stacked on a lilac core, each with a violet barrel and
    a magenta band, a white ball on top over a steady cyan collar, a ladder, a cabinet at its foot. 18.7 m, 4 m wide."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -150), 200, 230, steps=10)
    g.cyl(L, (0, 0, 80), 60, 1500, steps=8)
    for k in range(6):
        z = 180 + k * 230
        r = 190 - k * 12
        g.cyl(M, (0, 0, z - 22), r + 6, 22, steps=10)
        g.cyl(W, (0, 0, z), r, 90, steps=10, top_r=72)
        g.cyl(V, (0, 0, z + 90), 86, 118, steps=8)
    g.cyl(V, (0, 0, 1560), 100, 60, steps=8)
    g.cyl(C, (0, 0, 1620), 72, 30, steps=8, glow=GLOW)
    g.sphere(W, (0, 0, 1750), 125, steps=(6, 10))
    g.box(L, (0, -92, 820), (34, 8, 1440))                               # the ladder's back
    for k in range(9):
        g.box(W, (0, -98, 200 + k * 160), (44, 6, 8))
    g.box(V, (140, -120, 150), (90, 90, 140))
    g.box(W, (140, -120, 226), (102, 102, 12))
    g.box(M, (140, -167, 150), (40, 6, 40), rot=(45, 0, 0))


def sphere_tank(sc):
    """BUILDING 3, the ball tank: a violet ball 4.2 m across on three splayed legs, a magenta belt, a white crown,
    a spiral stair round the down pipe under it, a dial on its face, a pennant on the rod. 14.4 m."""
    g = sc.still()
    for k in range(3):
        a = math.radians(90 + k * 120)
        ca, sa = math.cos(a), math.sin(a)
        rod(g, L, (205 * ca, 205 * sa, -150), (120 * ca, 120 * sa, 850), 16, steps=6)
        g.box(CONC, (205 * ca, 205 * sa, -40), (60, 60, 100))
        b = math.radians(90 + (k + 1) * 120)
        rod(g, L, (178 * ca, 178 * sa, 330), (178 * math.cos(b), 178 * math.sin(b), 330), 7, steps=5)
    g.sphere(V, (0, 0, 1000), 210, steps=(7, 12))
    g.cyl(M, (0, 0, 984), 214, 32, steps=12)
    g.cyl(L, (0, 0, 1108), 186, 14, steps=12)
    g.cyl(W, (0, 0, 1196), 72, 30, steps=8)
    g.cyl(L, (0, 0, 1226), 6, 200, steps=6)
    g.sphere(C, (0, 0, 1436), 22, steps=(4, 6), glow=GLOW)
    g.cyl(L, (0, 0, -120), 16, 930, steps=6)                             # the down pipe the stair winds round
    for k in range(14):
        a = k * 40.0
        ra = math.radians(a)
        g.box(W, (66 * math.cos(ra), 66 * math.sin(ra), 70 + k * 52), (100, 28, 8), rot=(0, a, 0))
    g.box(W, (0, -207, 1000), (74, 10, 74), rot=(45, 0, 0))             # the dial
    g.box(M, (0, -214, 1000), (12, 6, 54))
    f = sc.part(P.motion(swing=13, pivot=(0, 0, 1380), axis=90, rate=0.45, phase=0.6))
    f.box(M, (58, 0, 1380), (104, 5, 40))


def capacitor_shed(sc):
    """BUILDING 4, the capacitor shed: a violet three-storey lean-to 12 m long with a saw-cut skyline of four white
    roof slabs over cyan lights, two magenta sliding doors under a white rail, windows, an outside stair, a diamond
    sign, and a side bay carrying three ringed capacitor cans. 13.6 m."""
    g = sc.still()
    g.box(V, (0, 0, 450), (1200, 500, 1200))
    g.box(L, (0, 0, 30), (1222, 522, 80))
    for z in (400, 740):
        g.box(M, (0, -4, z), (1212, 512, 24))
    for k in range(4):
        x0 = -600 + k * 300
        tilt_box(g, W, (x0, 0, 1056), (x0 + 300, 0, 1236), 510, 14)
        g.box(C, (x0 + 293, 0, 1146), (10, 470, 170), glow=GLOW)
        g.box(V, (x0 + 226, 0, 1098), (150, 498, 96))
        g.box(V, (x0 + 262, 0, 1178), (76, 498, 64))
    for x in (-300, 150):
        g.box(M, (x, -253, 190), (220, 10, 380))
        g.box(W, (x, -259, 190), (10, 6, 330))
    g.box(W, (-75, -255, 396), (760, 8, 22))
    for z in (570, 900):
        for k, x in enumerate((-450, -150, 150, 450)):
            lit = (z == 900 and k in (1, 2)) or (z == 570 and k == 3)
            g.box(C if lit else PANE, (x, -253, z), (150, 8, 110), glow=GLOW if lit else 0.0)
    tilt_box(g, L, (585, -300, -30), (330, -300, 395), 92, 14)           # the outside stair and its landing
    g.box(L, (270, -300, 398), (130, 92, 12))
    g.box(L, (585, -300, 100), (10, 10, 260))
    g.box(W, (460, -258, 250), (150, 8, 150))                            # the sign: a magenta diamond on white
    g.box(M, (460, -264, 250), (78, 6, 78), rot=(45, 0, 0))
    g.box(L, (-682, 40, 200), (164, 380, 700))                           # the side bay
    g.box(M, (-682, 40, 558), (180, 396, 18))
    for y in (-90, 40, 170):
        g.cyl(W, (-682, y, 567), 44, 250, steps=8)
        for z in (620, 700, 780):
            g.cyl(V, (-682, y, z), 50, 20, steps=8)
        g.cyl(M, (-682, y, 817), 30, 26, steps=6, top_r=12)
    for x in (-380, 380):                                                 # vent pipes with mushroom caps
        g.cyl(L, (x, 170, 1040), 22, 300, steps=6)
        g.cyl(W, (x, 170, 1340), 54, 30, steps=8, top_r=22)


def watch_office(sc):
    """BUILDING 5, the watch office: a round violet cabin with a wrap-round cyan window and a ribbed white cap, on a
    lilac lattice 9 m tall, a ring gallery with a magenta rail, a ladder, a vane swinging on its mast. 17 m, 6 m wide."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, L, (sx * 175, sy * 175, -150), (sx * 110, sy * 110, 900), 18, 18)
    for z, d in ((260, 153), (600, 131)):
        g.box(L, (0, -d, z), (2 * d, 10, 10))
        g.box(L, (0, d, z), (2 * d, 10, 10))
        g.box(L, (-d, 0, z), (10, 2 * d, 10))
        g.box(L, (d, 0, z), (10, 2 * d, 10))
    for s in (-1, 1):                                                     # one diagonal a face
        tilt_box(g, L, (-150, s * 153, 260), (131, s * 131, 600), 9, 9)
        tilt_box(g, L, (s * 153, -150, 260), (s * 131, 131, 600), 9, 9)
    g.box(L, (0, -168, 420), (36, 8, 960))
    for k in range(6):
        g.box(W, (0, -174, 80 + k * 150), (46, 6, 8))
    g.cyl(M, (0, 0, 840), 120, 62, steps=8)
    g.cyl(L, (0, 0, 900), 300, 20, steps=12)
    for k in range(8):                                                    # the gallery's rail
        a0, a1 = math.radians(k * 45.0), math.radians(k * 45.0 + 45.0)
        p0 = (290 * math.cos(a0), 290 * math.sin(a0))
        p1 = (290 * math.cos(a1), 290 * math.sin(a1))
        g.box(L, (p0[0], p0[1], 965), (8, 8, 100))
        tilt_box(g, M, (p0[0], p0[1], 1012), (p1[0], p1[1], 1012), 8, 8)
    g.cyl(V, (0, 0, 920), 200, 300, steps=10)
    g.cyl(C, (0, 0, 1050), 204, 92, steps=10, glow=GLOW)
    for k in range(5):
        wall_boxes(g, V, 208, 1096, (-170 + k * 85,), (12, 8, 96))       # mullions across the lit band
    g.cyl(W, (0, 0, 1220), 240, 30, steps=10)
    top = ribbed_cap(g, 1250, 198, squash=0.55, ribs=2, steps=(5, 10))
    g.cyl(L, (0, 0, top - 10), 6, 270, steps=6)
    g.sphere(W, (0, 0, top + 270), 18, steps=(4, 6))
    f = sc.part(P.motion(swing=30, pivot=(0, 0, top + 200), axis=P.UP, rate=0.1, phase=0.4))
    f.box(M, (46, 0, top + 200), (104, 5, 34))
    f.box(W, (-36, 0, top + 200), (40, 5, 12))


def coil_house(sc):
    """BUILDING 6, the twin coil house: two fat violet coils wound with magenta and lilac rings, each under a white
    cap and a steady cyan ball, a bus bar with hanging insulators between them, under one arched white roof on six
    lilac posts with magenta end ribs and two vent caps; steps and a meter cabinet in front. 13 m, 10 m long."""
    g = sc.still()
    g.box(CONC, (0, 0, -35), (1040, 520, 230))
    for x in (-490, 0, 490):
        for y in (-225, 225):
            g.box(L, (x, y, 500), (26, 26, 840))
    n = 6
    arc = [(270 * math.cos(math.pi * k / n), 920 + 210 * math.sin(math.pi * k / n)) for k in range(n + 1)]
    for k in range(n):
        (y0, z0), (y1, z1) = arc[k], arc[k + 1]
        tilt_box(g, W, (0, y0, z0), (0, y1, z1), 1060, 14)
        for x in (-524, 524):
            tilt_box(g, M, (x, y0 * 1.03, z0 + 6), (x, y1 * 1.03, z1 + 6), 26, 24)
    for x in (-250, 250):
        g.cyl(V, (x, 0, 80), 150, 700, steps=10)
        for k in range(6):
            g.cyl(M if k % 2 == 0 else L, (x, 0, 150 + k * 105), 176, 56, steps=10)
        g.cyl(W, (x, 0, 780), 120, 40, steps=10)
        g.sphere(C, (x, 0, 850), 42, steps=(4, 6), glow=GLOW)
    g.box(L, (0, 0, 800), (300, 18, 18))
    for x in (-70, 70):
        g.cyl(W, (x, 0, 700), 26, 92, steps=6, top_r=12)
    g.box(CONC, (0, -292, 15), (300, 90, 70))
    g.box(W, (0, -236, 190), (120, 40, 190))
    g.box(M, (0, -259, 215), (60, 6, 60), rot=(45, 0, 0))
    for x in (-300, 300):
        g.cyl(L, (x, 0, 1120), 20, 130, steps=6)
        g.cyl(W, (x, 0, 1250), 50, 28, steps=8, top_r=20)


def kiln(sc):
    """BUILDING 7, the glass-blower's kiln: a violet bottle kiln on a lilac drum with a white neck and a magenta lip,
    its mouth lit a steady cyan in a lilac frame, a showcase of obsidian panes in a lilac frame beside it, a bench of
    blown glass balls and a rack of blowpipes. 12.5 m."""
    g = sc.still()
    g.cyl(L, (0, 0, -150), 210, 400, steps=10)
    g.cyl(V, (0, 0, 250), 200, 450, steps=10, top_r=150)
    g.cyl(V, (0, 0, 700), 150, 300, steps=10, top_r=70)
    g.cyl(W, (0, 0, 1000), 70, 220, steps=8)
    g.cyl(M, (0, 0, 1220), 88, 30, steps=8)
    g.cyl(M, (0, 0, 250), 208, 30, steps=10)
    g.cyl(L, (0, 0, 700), 158, 24, steps=10)
    g.box(C, (0, -190, 350), (100, 30, 140), glow=GLOW)                  # the mouth
    for x in (-62, 62):
        g.box(L, (x, -200, 350), (20, 30, 180))
    g.box(L, (0, -200, 450), (150, 30, 24))
    g.box(CONC, (335, 0, 10), (270, 300, 60))                            # the showcase
    for x in (225, 445):
        for y in (-140, 140):
            g.box(L, (x, y, 250), (14, 14, 480))
    tilt_box(g, W, (215, 0, 520), (455, 0, 470), 310, 12)
    g.box(PANE, (335, -140, 250), (206, 6, 400))
    g.box(PANE, (445, 0, 250), (6, 266, 400))
    g.box(W, (290, -144, 330), (10, 5, 150), rot=(20, 0, 0))             # glints on the black glass
    g.box(W, (370, -144, 200), (8, 5, 90), rot=(20, 0, 0))
    g.box(L, (-300, -110, 92), (170, 54, 10))                            # the bench
    for x in (-370, -230):
        g.box(L, (x, -110, 20), (10, 50, 140))
    for x, col, r in ((-350, W, 24), (-300, M, 20), (-250, L, 26)):
        g.sphere(col, (x, -110, 97 + r), r, steps=(4, 6))
    for k in range(3):
        rod(g, L, (-200 + k * 22, 150, -20), (-150 + k * 22, 60, 330), 4, steps=4)
    g.box(L, (-165, 100, 190), (110, 10, 10))


# ------------------------------------------------------------------------------------------ props, low (top 2 m or under)
def coil_drum(sc):
    """PROP 1, LOW: a coil drum lying on a skid: a violet drum between white flanges, four magenta windings, lilac
    runners and cradles, a white terminal on top. 2.6 m long, 0.9 m deep, 1.9 m."""
    g = sc.still()
    for y in (-34, 34):
        g.box(L, (0, y, 0), (260, 16, 80))
    for x in (-72, 72):
        g.box(V, (x, 0, 62), (22, 84, 50))
    rod(g, V, (-100, 0, 122), (100, 0, 122), 38, steps=10)
    for x in (-112, 100):
        rod(g, W, (x, 0, 122), (x + 12, 0, 122), 45, steps=10)
    for k in range(4):
        x = -78 + k * 46
        rod(g, M, (x, 0, 122), (x + 20, 0, 122), 41, steps=10)
    g.cyl(W, (0, 0, 158), 13, 20, steps=6, top_r=8)
    g.sphere(L, (0, 0, 181), 8, steps=(3, 5))


def insulator_bollards(sc):
    """PROP 2, LOW: a row of five ceramic insulator bollards on a lilac rail: three white sheds each, a magenta
    knob. 7 m long, 0.7 m deep, 1.6 m."""
    g = sc.still()
    g.box(L, (0, 0, 14), (700, 12, 12))
    for k in range(5):
        x = -320 + k * 160
        g.cyl(L, (x, 0, -40), 24, 76, steps=8)
        for j in range(3):
            g.cyl(W, (x, 0, 36 + j * 32), 34, 26, steps=8, top_r=18)
        g.sphere(M, (x, 0, 142), 20, steps=(4, 6))


def shard_plinths(sc):
    """PROP 3, LOW: three obsidian shards on stone plinths with lilac collars and a magenta ring, a white glint on
    each. 3.8 m long, 0.9 m deep, 1.95 m."""
    g = sc.still()
    for x, hp, hs, lean, yaw in ((-150, 60, 117, 7, 0), (0, 44, 96, -9, 40), (150, 52, 108, 6, 110)):
        g.cyl(CONC, (x, 0, -40), 44, 40 + hp, steps=6)
        g.cyl(L, (x, 0, hp), 40, 14, steps=6)
        g.cyl(M, (x, 0, hp + 14), 30, 8, steps=6)
        g.cyl(PANE, (x, 0, hp + 16), 30, hs, steps=5, top_r=5, rot=(lean, yaw, 0))
        g.cyl(PANE, (x + 14, 6, hp + 16), 16, hs * 0.55, steps=4, top_r=4, rot=(-lean * 1.5, yaw + 60, 0))
        g.box(W, (x - 6, -22, hp + 16 + hs * 0.42), (6, 5, hs * 0.36), rot=(lean, 0, 0))


def meter_bench(sc):
    """PROP 4, LOW: a bench of four white meter boxes with violet diamond dials and magenta needles, a terminal on
    each, a violet conduit along the back. 3.2 m long, 0.5 m deep, 1.7 m."""
    g = sc.still()
    g.box(L, (0, 0, 45), (320, 46, 10))
    for x in (-140, 0, 140):
        g.box(L, (x, 0, 5), (12, 40, 76))
    for k, x in enumerate((-120, -40, 40, 120)):
        g.box(W, (x, 0, 95), (56, 40, 90))
        g.box(V, (x, -21, 100), (34, 5, 34), rot=(45, 0, 0))
        g.box(M, (x, -24, 100), (6, 5, 30), rot=(-50 + k * 35, 0, 0))
        g.cyl(M, (x, 0, 140), 8, 28, steps=5)
    g.box(V, (0, 16, 152), (300, 8, 8))


# ------------------------------------------------------------------------------------------ props, tall and open (24 m and more)
def lightning_rod(sc):
    """PROP 5, TALL, open: a lightning rod on a tripod: a lilac mast with three ring fins (violet, magenta, violet),
    two white insulator sheds at its foot, a steady cyan ball at the tip. 10.9 m, 1 m across. Nothing moves."""
    g = sc.still()
    for k in range(3):
        a = math.radians(90 + k * 120)
        rod(g, L, (48 * math.cos(a), 48 * math.sin(a), -60), (0, 0, 330), 5, steps=5)
    g.cyl(L, (0, 0, 120), 30, 8, steps=6)
    g.cyl(L, (0, 0, 300), 7, 760, steps=6)
    for z in (330, 372):
        g.cyl(W, (0, 0, z), 22, 32, steps=8, top_r=10)
    for z, r, col in ((560, 48, V), (700, 40, M), (840, 32, V)):
        g.cyl(col, (0, 0, z), r, 12, steps=10)
        g.cyl(W, (0, 0, z + 12), r * 0.45, 8, steps=8)
    g.sphere(C, (0, 0, 1074), 15, steps=(4, 6), glow=GLOW)


def cable_arch(sc):
    """PROP 6, TALL, open: a cable arch: two thin lilac masts 9 m apart with white insulator caps and magenta knobs,
    a violet cable sagging between them strung with white ball insulators, a steady cyan bead at its middle, three
    pennants (magenta, white, magenta) that swing. 8.2 m tall, 0.4 m deep."""
    g = sc.still()
    for s in (-1, 1):
        g.cyl(CONC, (s * 450, 0, -80), 20, 110, steps=6)
        g.cyl(L, (s * 450, 0, -80), 9, 820, steps=6)
        g.cyl(W, (s * 450, 0, 740), 20, 40, steps=8, top_r=9)
        g.sphere(M, (s * 450, 0, 796), 14, steps=(4, 6))
    n = 8
    pts = []
    for k in range(n + 1):
        x = -450 + 900.0 * k / n
        pts.append((x, 0, 560 + 200.0 * (x / 450.0) ** 2))
    for k in range(n):
        rod(g, V, pts[k], pts[k + 1], 4, steps=4)
    for k in (1, 2, 3, 5, 6, 7):
        g.sphere(W, pts[k], 13, steps=(3, 5))
    g.sphere(C, pts[4], 15, steps=(4, 6), glow=GLOW)
    for i, (k, col) in enumerate(((2, M), (4, W), (6, M))):
        x, _, z = pts[k]
        f = sc.part(P.motion(swing=11, pivot=(x, 0, z - 14), axis=0, rate=0.5, phase=0.3 * i))
        f.box(col, (x, 0, z - 48), (40, 5, 58))


def instrument_vane(sc):
    """PROP 7, TALL, open: an instrument vane: a lilac mast with a violet cabinet and a white louvred screen, a ring
    fin, four white cups that go round slowly (one round in seven seconds) and a magenta vane that swings. 7.2 m."""
    g = sc.still()
    g.cyl(L, (0, 0, -60), 7, 720, steps=6)
    g.box(V, (0, 0, 40), (50, 50, 100))
    g.box(W, (0, 0, 95), (58, 58, 10))
    g.box(W, (0, -18, 300), (44, 34, 56))
    for z in (284, 300, 316):
        g.box(L, (0, -37, z), (40, 5, 6))
    g.cyl(V, (0, 0, 520), 30, 10, steps=10)
    g.cyl(M, (0, 0, 626), 9, 28, steps=6)
    c = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, 0.14)))
    c.box(L, (0, 0, 640), (96, 5, 5))
    c.box(L, (0, 0, 640), (5, 96, 5))
    for x, y in ((48, 0), (-48, 0), (0, 48), (0, -48)):
        c.sphere(W, (x, y, 640), 13, steps=(3, 5))
    v = sc.part(P.motion(swing=30, pivot=(0, 0, 690), axis=P.UP, rate=0.11, phase=0.5))
    v.box(M, (22, 0, 690), (56, 5, 26))
    v.box(W, (-24, 0, 690), (22, 5, 10))


def signal_mast(sc):
    """PROP 8, TALL, open: the storm signal mast: a lilac mast and yard with shapes hoisted on two halyards (a white
    ball, a magenta diamond; a violet drum, a white cone: shapes, no words), a steady cyan ball at the masthead and a
    magenta streamer that swings. 9.3 m, 3 m along, 0.7 m deep."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -60), 24, 100, steps=6)
    g.cyl(L, (0, 0, -60), 9, 960, steps=6)
    g.box(L, (0, 0, 760), (300, 8, 8))
    for s in (-1, 1):
        g.cyl(L, (s * 140, 0, 330), 3, 430, steps=4)
        g.box(L, (s * 140, 0, 322), (26, 8, 8))
    g.sphere(W, (-140, 0, 650), 34, steps=(5, 8))
    g.cyl(M, (-140, 0, 520), 34, 44, steps=6, top_r=3)
    g.cyl(M, (-140, 0, 476), 3, 44, steps=6, top_r=34)
    g.cyl(V, (140, 0, 610), 26, 64, steps=8)
    g.cyl(W, (140, 0, 630), 28, 12, steps=8)
    g.cyl(W, (140, 0, 470), 34, 64, steps=6, top_r=3)
    g.sphere(C, (0, 0, 914), 15, steps=(4, 6), glow=GLOW)
    f = sc.part(P.motion(swing=12, pivot=(0, 0, 880), axis=90, rate=0.4, phase=0.1))
    f.box(M, (72, 0, 880), (130, 5, 30))


# ------------------------------------------------------------------------------------------ the hero
def _horn(k, n, side):
    t = k / float(n)
    return (side * (260.0 + 1450.0 * math.sin(math.pi * t) * (1.0 - 0.22 * t)), 0.0, 620.0 + 4880.0 * t), 230.0 - 170.0 * t


def storm_harp(sc):
    """HERO, the Storm Harp: two violet horns 55 m tall rising from a drum base, bowing out to 31 m and curving back
    towards each other, ringed at every joint (magenta, lilac, white), a white ball and a cyan one on each tip; a lilac
    yoke and a bridge between them carry seven lilac strings, and on every string two cyan beads slide slowly up and
    down (steady light, each on its own clock). The base: a stepped drum with a magenta band, a ring of cyan windows,
    a magenta door and a ribbed white dome."""
    g = sc.still()
    g.cyl(CONC, (0, 0, -300), 900, 420, steps=16)
    g.cyl(V, (0, 0, 120), 760, 380, steps=16)
    g.cyl(M, (0, 0, 462), 772, 40, steps=16)
    g.cyl(L, (0, 0, 500), 600, 160, steps=14)
    ribbed_cap(g, 660, 560, squash=0.45, ribs=3, steps=(6, 14))
    wall_boxes(g, M, 764, 250, (0,), (200, 16, 250))
    for x in (-560, -400, -230, 230, 400, 560):
        wall_boxes(g, C, 764, 330, (x,), (90, 14, 120), glow=GLOW)
    for a in (35, 145, 215, 325):                                         # four buttress fins
        ra = math.radians(a)
        tilt_box(g, V, (860 * math.cos(ra), 860 * math.sin(ra), -100), (640 * math.cos(ra), 640 * math.sin(ra), 520), 60, 60)
    n = 12
    for side in (-1, 1):
        for k in range(n):
            (a, r0), (b, r1) = _horn(k, n, side), _horn(k + 1, n, side)
            seg(g, V, a, b, r0, r1, steps=8)
            if k > 0:
                g.cyl((M, L, W)[k % 3], (a[0], 0, a[2] - 26), r0 * 1.28, 52, steps=8)
        tip, rt = _horn(n, n, side)
        g.sphere(W, (tip[0], 0, tip[2] + 40), 105, steps=(5, 8))
        g.sphere(C, (tip[0], 0, tip[2] + 170), 58, steps=(4, 6), glow=0.6)
    (yk, _r), (bk, _r2) = _horn(9.36, n, 1), _horn(1.67, n, 1)            # the yoke (z 44.3 m) and the bridge (z 13 m)
    for (px, _y, pz), r in ((yk, 40), (bk, 34)):
        rod(g, L, (-px, 0, pz), (px, 0, pz), r, steps=8)
        for s in (-1, 1):
            g.sphere(M, (s * px, 0, pz), r * 2.2, steps=(4, 8))
    z0, z1 = bk[2], yk[2]
    span = z1 - z0
    for k in range(7):
        x = -480 + k * 160
        g.box(L, (x, 0, (z0 + z1) / 2.0), (22, 22, span))
        g.cyl(M, (x, 0, z1 + 30), 26, 60, steps=6)                       # the tuning pegs on the yoke
        for j, f in enumerate((0.27, 0.73)):
            bead = sc.part(P.motion(bob=-span * 0.2, rate=0.035 + 0.004 * ((k * 3 + j * 5) % 7), phase=(k * 0.37 + j * 0.5) % 1.0))
            bead.sphere(C, (x, 0, z0 + span * f), 68, steps=(4, 6), glow=0.6)


JOBS = [
    ('SM_S1D2_DrumHall', drum_hall, ROOF), ('SM_S1D2_InsulTower', insulator_tower, STILL),
    ('SM_S1D2_SphereTank', sphere_tank, SLIM), ('SM_S1D2_CapShed', capacitor_shed, STILL),
    ('SM_S1D2_WatchOffice', watch_office, ROOF), ('SM_S1D2_CoilHouse', coil_house, STILL), ('SM_S1D2_Kiln', kiln, STILL),
    ('SM_S1D2_CoilDrum', coil_drum, STILL), ('SM_S1D2_InsulBollards', insulator_bollards, STILL),
    ('SM_S1D2_ShardPlinths', shard_plinths, STILL), ('SM_S1D2_MeterBench', meter_bench, STILL),
    ('SM_S1D2_LightningRod', lightning_rod, STILL), ('SM_S1D2_CableArch', cable_arch, SLIM),
    ('SM_S1D2_InstrumentVane', instrument_vane, dict(grow=(10, 10, 10), grow_down=(10, 10, 0))),
    ('SM_S1D2_SignalMast', signal_mast, SLIM),
    ('SM_S1D2_StormHarp', storm_harp, dict(far=True, grow=(20, 20, 20), grow_down=(20, 20, 0))),   # review round: far=True, the beads stopped past 180 m
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
