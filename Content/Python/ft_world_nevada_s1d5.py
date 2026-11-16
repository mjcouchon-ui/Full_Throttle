"""Nevada, Stage 1, builder s1d5: district 5 "Salt Flats" (10.69 to 14.5 km), a speed-week camp and a salt works.
/Game/World/Nevada/SM_S1D5_*.   Run: import ft_world_nevada_s1d5 as K; K.run()

Streamline shapes: round-ended timing towers with portholes and wrap-round decks, arched canvas pit shelters, polished
caravans WITHOUT tow cars, a conveyor gallery climbing to white salt cones; one to three storeys.
Palette (the kit's own hexes): polished metal #d7e3f0, salt and canvas white #fff6ea, stripes #ff2d3c, checks and
pennants #ffe14d, portholes and shadow #233a63; tyres and deep shadow #2b2350, bare steel #8f9bb3.

8 buildings (TimingTower, PitShelter, ScrutineerShed, CaravanDiner, CommentaryBox, CaravanCamp: 37 m out;
EvaporatorHouse, SaltTower: 66 m out), 9 props (SaltCone, PennantMasts, Windsock, ResultsBoard, SpeakerPole, SunShade,
FloodMast: tall, 26.5 m and more out; MarkerDrums, TyreRack: low, 21.5 m), the hero THE BIG STOPWATCH (140 to 148 m).
Every piece: ft_people.Scene, one section, M_People (the hero M_PeopleFar: its hand sweeps at any distance).
Front faces -Y, base on z = 0 with a metre of footing, X along the road, cm. No figure, no animal, no word, no numeral.
Not here on purpose: start-light columns, dice, vehicles, wrenches, anything orange or green of a metre, anything that flies.
Light: steady only, and only on pieces that stand 26.5 m or more out. Movement: vertex shader only (P.motion).
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/Nevada'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))
SILVER, WHITE, RED, YEL, NAVY = '#d7e3f0', '#fff6ea', '#ff2d3c', '#ffe14d', '#233a63'
DARK, STEEL = '#2b2350', '#8f9bb3'
GL, GW = 0.9, 0.55            # a lamp, a lit window (steady)


# ------------------------------------------------------------------------------------------ helpers
def ccw(pts):
    a = 0.0
    for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
        a += x0 * y1 - x1 * y0
    return list(pts) if a >= 0 else list(reversed(pts))


def stad(w, d, n=4, cx=0.0, cy=0.0):
    """A plan with two round ends (the streamline shape): w along X, d across."""
    r = d / 2.0
    hx = max(w / 2.0 - r, 0.0)
    pts = []
    for i in range(n + 1):
        a = math.radians(-90.0 + 180.0 * i / n)
        pts.append((cx + hx + r * math.cos(a), cy + r * math.sin(a)))
    for i in range(n + 1):
        a = math.radians(90.0 + 180.0 * i / n)
        pts.append((cx - hx + r * math.cos(a), cy + r * math.sin(a)))
    return pts


def arc(cx, cz, a, b, t0, t1, n):
    """Points of an ellipse (half axes a, b) from angle t0 to t1, degrees."""
    return [(cx + a * math.cos(math.radians(t0 + (t1 - t0) * i / float(n))), cz + b * math.sin(math.radians(t0 + (t1 - t0) * i / float(n))))
            for i in range(n + 1)]


def rrect(w, h, r, n=3, cx=0.0, z0=0.0):
    """A side profile with four round corners (a caravan's body)."""
    pts = []
    for (ox, oz, t0) in ((w / 2.0 - r, r, -90.0), (w / 2.0 - r, h - r, 0.0), (-w / 2.0 + r, h - r, 90.0), (-w / 2.0 + r, r, 180.0)):
        pts += arc(cx + ox, z0 + oz, r, r, t0, t0 + 90.0, n)
    return pts


def rbox(w, d, r, n=3, cx=0.0, cy=0.0):
    """A plan with four round corners: flat fronts between them."""
    return rrect(w, d, r, n, cx, cy - d / 2.0)


def teardrop(L, H, cx=0.0, z0=0.0, flip=1.0):
    """A teardrop caravan's side: a full round nose, a roof falling to a low tail."""
    r = H / 2.0
    pts = arc(-L / 2.0 + r, z0 + r, r, r, 90.0, 270.0, 5)
    pts += [(L / 2.0 - 30.0, z0), (L / 2.0, z0 + H * 0.22), (L / 2.0 - 40.0, z0 + H * 0.5), (L / 2.0 - 190.0, z0 + H * 0.9)]
    return ccw([(cx + flip * x, z) for x, z in pts])


def disc(g, col, x, y, z, r, th=8.0, steps=8, glow=0.0, face=-1.0):
    """A round plate on a face that looks along -Y (face=1: +Y): a porthole, a lamp, a hub."""
    rod(g, col, (x, y - face * 2.0, z), (x, y + face * th, z), r, steps=steps, glow=glow)


def checks(g, x0, x1, y, z, h, n, odd=0, col=YEL, th=6.0):
    """Every other square of a chequered band on a front face."""
    dx = (x1 - x0) / float(n)
    for i in range(n):
        if i % 2 == odd:
            g.box(col, (x0 + (i + 0.5) * dx, y, z), (dx, th, h))


def vent(g, x, y, z, col=SILVER):
    g.cyl(col, (x, y, z), 10, 34, steps=6)
    g.cyl(RED, (x, y, z + 34), 24, 14, steps=6, top_r=8)


def horn(g, x, y, z, yaw, L=90.0, r=48.0, col=SILVER):
    """A horn loudspeaker, its mouth looking along yaw (0 = +X, -90 = the road)."""
    g.cyl(col, (x, y, z), 12, L, rot=(-90, yaw, 0), steps=8, top_r=r)


def pennant(sc, x, y, z, col, L=80.0, H=46.0, swing=16.0, rate=0.45, phase=0.0):
    """A pennant flying from a mast top, swinging round the mast."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.side(col, ccw([(x, z + H / 2.0), (x, z - H / 2.0), (x + L, z)]), y - 2.5, y + 2.5)


def bunting(sc, pts, y, cols, w=44.0, h=58.0, swing=14.0, rate=0.6, phase=0.0):
    """Small flags hanging from a line at the points (x, z), swinging about the line."""
    for i, (x, z) in enumerate(pts):
        f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=0.0, rate=rate, phase=phase + 0.37 * (i % 3)))
        f.side(cols[i % len(cols)], ccw([(x - w / 2.0, z), (x + w / 2.0, z), (x, z - h)]), y - 2.5, y + 2.5)


def lerp3(a, b, t):
    return (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t)


# ------------------------------------------------------------------------------------------ buildings
def timing_tower(sc):
    """Three round-ended decks, 11.6 x 6.8 m, 13.4 m to the mast top: lit control room, portholes, a chequered flag."""
    g = sc.still()
    g.plan(WHITE, stad(1000, 520), -100, 330)
    g.plan(RED, stad(1012, 532), 120, 160)
    g.plan(SILVER, stad(1160, 680), 330, 352)                 # first deck
    g.plan(SILVER, stad(840, 440), 352, 650)
    g.plan(RED, stad(852, 452), 452, 482)
    g.plan(SILVER, stad(980, 580), 650, 670)                  # second deck
    g.plan(WHITE, stad(660, 360), 670, 930)
    g.plan(YEL, stad(670, 370), 770, 860, glow=GW)            # the control room's wrap-round window, lit
    g.plan(SILVER, stad(800, 480), 930, 955)
    g.plan(RED, stad(460, 240), 955, 985)
    for x in (-150, -50, 50, 150):                            # mullions
        g.box(NAVY, (x, -186, 815), (10, 8, 92))
    for x in (-215, -120, 120, 215):
        disc(g, NAVY, x, -260, 235, 40)
    for x in (-150, 0, 150):
        disc(g, NAVY, x, -220, 560, 36)
    g.box(NAVY, (0, -262, 100), (110, 8, 210))                # door
    g.box(YEL, (0, -266, 150), (60, 4, 50), glow=GW)
    checks(g, -240, 240, -341, 341, 22, 8)                    # chequered edge of the first deck
    g.box(SILVER, (0, -335, 440), (480, 5, 6))                # rails
    for x in (-240, -120, 0, 120, 240):
        g.box(SILVER, (x, -335, 396), (5, 5, 88))
    g.box(SILVER, (0, -285, 755), (400, 5, 6))
    for x in (-200, -67, 67, 200):
        g.box(SILVER, (x, -285, 712), (5, 5, 86))
    tilt_box(g, STEEL, (540, -300, -40), (540, 120, 340), 80, 14)      # outside stair at the right end
    tilt_box(g, SILVER, (582, -300, 50), (582, 120, 430), 5, 6)
    g.cyl(SILVER, (0, 0, 985), 7, 330, steps=6)               # mast
    g.sphere(YEL, (0, 0, 1322), 14, steps=(3, 5))
    g.box(SILVER, (0, 0, 1180), (150, 5, 5))
    pennant(sc, 0, 0, 1280, RED, L=120, H=60, swing=18, rate=0.4)
    cups = sc.part(P.motion(travel=(-1.0, 0.6), base=(75.0, 0.0)))     # anemometer on the yard arm
    for k in range(3):
        a = math.radians(k * 120.0)
        cups.sphere(SILVER, (75 + 26 * math.cos(a), 26 * math.sin(a), 1196), 10, steps=(3, 5))
    g.cyl(SILVER, (75, 0, 1180), 3, 18, steps=4)
    fl = sc.part(P.motion(swing=10, pivot=(-300, -250, 760), axis=P.UP, rate=0.35, phase=0.4))   # chequered flag on the top deck
    g.cyl(SILVER, (-300, -250, 670), 4, 190, steps=4)
    fl.box(WHITE, (-245, -250, 815), (110, 4, 80))
    for dy in (-3, 3):
        fl.box(NAVY, (-272, -250 + dy, 835), (55, 4, 40)); fl.box(NAVY, (-218, -250 + dy, 795), (55, 4, 40))


def pit_shelter(sc):
    """An arched canvas shelter, 11.4 x 8.2 m, 5.4 m: white with red hoops, benches, a shape under a dust sheet."""
    g = sc.still()
    A, B = 560.0, 520.0
    for (y0, y1, col) in ((-400, -240, WHITE), (-240, -140, RED), (-140, 60, WHITE), (60, 160, RED), (160, 400, WHITE)):
        g.side(col, arc(0, 0, A, B, 0, 180, 8) + arc(0, 0, A - 14, B - 14, 180, 0, 8), y0, y1)
    g.side(RED, arc(0, 0, A + 10, B + 10, 0, 180, 8) + arc(0, 0, A - 22, B - 22, 180, 0, 8), -414, -400)   # the front hoop
    g.side(NAVY, arc(0, 0, A - 14, B - 14, 0, 180, 8), 384, 396)                                         # the back, in shadow
    for sx in (-1, 1):
        for y in (-380, 380):
            g.box(STEEL, (sx * (A - 6), y, -45), (40, 40, 110))
    g.box(WHITE, (0, -425, -40), (1140, 20, 110))             # kerb, chequered
    checks(g, -560, 560, -437, 0, 24, 14)
    g.box(SILVER, (-140, 80, 52), (420, 170, 104))            # the shape under its dust sheet (lumps, no wheels)
    g.sphere(SILVER, (-230, 80, 104), 80, scale=(1.5, 1.0, 0.55), steps=(4, 6))
    g.sphere(SILVER, (-30, 80, 104), 70, scale=(1.3, 1.0, 0.5), steps=(4, 6))
    g.box(STEEL, (-140, 320, 45), (640, 40, 8)); g.box(STEEL, (-420, 320, 20), (8, 40, 44)); g.box(STEEL, (140, 320, 20), (8, 40, 44))   # bench
    g.box(RED, (380, 250, 45), (100, 60, 90)); g.box(SILVER, (380, 250, 94), (108, 66, 8))                 # tool chest
    for k in range(3):                                        # spare tyres
        g.cyl(DARK, (380, -120, k * 24), 36, 22, steps=8)
    g.box(WHITE, (300, 60, 78), (160, 80, 8)); g.box(YEL, (300, 20, 60), (160, 4, 40))                     # trestle table, a yellow cloth
    g.box(STEEL, (240, 60, 37), (6, 60, 74)); g.box(STEEL, (360, 60, 37), (6, 60, 74))
    g.box(STEEL, (0, -200, 480), (4, 4, 60)); g.sphere(WHITE, (0, -200, 440), 20, steps=(3, 5), glow=GL)   # lamp at the crown
    line = [(-420 + i * 140, 372 - 14 * (3 - abs(i - 3))) for i in range(7)]                              # a line of flags across the mouth
    for (p0, p1) in zip(line, line[1:]):
        tilt_box(g, STEEL, (p0[0], -418, p0[1]), (p1[0], -418, p1[1]), 4, 4)
    bunting(sc, [((p0[0] + p1[0]) / 2.0, (p0[1] + p1[1]) / 2.0) for (p0, p1) in zip(line, line[1:])], -418, (YEL, RED, WHITE), w=50, h=62)
    g.cyl(SILVER, (-600, -400, -100), 6, 720, steps=6)
    pennant(sc, -600, -400, 600, YEL, L=110, H=56, phase=0.2)


def evaporator_house(sc):
    """The salt works: a round-ended hall, a conveyor gallery climbing to a head house, three white cones. 33 x 14 m, 16.4 m."""
    X = -200.0
    g = sc.still(at=(X, 0, 0))
    g.plan(SILVER, rbox(1200, 760, 200, cx=-900), -100, 600)
    g.plan(RED, rbox(1212, 772, 206, cx=-900), 180, 220)
    g.plan(WHITE, rbox(1300, 860, 240, cx=-900), 600, 625)
    g.plan(WHITE, rbox(800, 440, 150, cx=-900), 625, 760)                # clerestory
    g.plan(YEL, rbox(808, 448, 154, cx=-900), 660, 720, glow=GW)
    g.plan(SILVER, rbox(880, 520, 180, cx=-900), 760, 780)
    checks(g, -1300, -500, -432, 612, 24, 16)
    g.box(NAVY, (-900, -382, 150), (260, 8, 320)); g.box(SILVER, (-900, -387, 150), (10, 4, 320))        # doors
    for x in (-1230, -1090, -710, -570):
        disc(g, NAVY, x, -380, 400, 44)
    for x in (-1180, -900, -620):
        vent(g, x, 0, 780)
    g.cyl(SILVER, (-1250, 200, 600), 46, 560, steps=8)                  # the stack
    g.cyl(RED, (-1250, 200, 1050), 50, 40, steps=8); g.cyl(RED, (-1250, 200, 950), 50, 24, steps=8)
    a, b = (-430.0, 0.0, 560.0), (900.0, 0.0, 1250.0)                     # conveyor gallery
    tilt_box(g, SILVER, a, b, 220, 200)
    tilt_box(g, RED, (a[0], -113, a[2] - 40), (b[0], -113, b[2] - 40), 6, 36)
    for i in range(5):
        p = lerp3(a, b, 0.14 + i * 0.18)
        disc(g, NAVY, p[0], -112, p[2] + 30, 34)
    for x in (0.0, 350.0, 700.0):                                         # trestle bents
        zt = a[2] + (x - a[0]) / (b[0] - a[0]) * (b[2] - a[2]) - 110.0
        for sy in (-1, 1):
            tilt_box(g, STEEL, (x, sy * 190, -100), (x, sy * 90, zt), 18, 18)
        g.box(STEEL, (x, 0, zt * 0.5), (14, 300, 14))
    g.box(WHITE, (960, 0, 1310), (330, 300, 300))                        # head house
    g.plan(RED, stad(420, 340, cx=960), 1460, 1490)
    disc(g, YEL, 960, -150, 1320, 46, glow=GW)
    g.cyl(SILVER, (1010, 0, 970), 30, 200, steps=8, top_r=64)             # chute
    g.cyl(WHITE, (1010, 0, -100), 560, 1030, steps=12, top_r=24)          # the cones
    g.cyl(WHITE, (1500, 160, -100), 340, 600, steps=10, top_r=16)
    g.cyl(WHITE, (1470, -280, -100), 230, 400, steps=8, top_r=12)
    for i in range(3):                                                    # evaporation pans in front of the hall
        x = -1220 + i * 320
        g.box(SILVER, (x, -560, -30), (290, 200, 100)); g.box(WHITE, (x, -560, 22), (262, 172, 6))
    rod(g, STEEL, (-1040, -380, 60), (-1040, -470, 60), 14, steps=6)
    g.box(STEEL, (-1506, 150, 250), (8, 60, 700))                        # ladder
    for k in range(6):
        g.box(STEEL, (-1512, 150, 60 + k * 100), (6, 56, 6))
    for x in (-1160, -640):
        g.sphere(WHITE, (x, -390, 520), 18, steps=(3, 5), glow=GL)
    g.cyl(SILVER, (960, 0, 1490), 6, 190, steps=6)                        # weather vane on the head house
    g.box(SILVER, (960, 0, 1560), (120, 5, 5)); g.box(SILVER, (960, 0, 1560), (5, 120, 5))
    v = sc.part(P.motion(swing=40, pivot=(960 + X, 0, 1640), axis=P.UP, rate=0.12), at=(X, 0, 0))
    v.box(RED, (960, 0, 1640), (200, 5, 8))
    v.side(RED, ccw([(1060, 1640), (1020, 1664), (1020, 1616)]), -3, 3)
    v.side(YEL, ccw([(860, 1640), (800, 1680), (800, 1600)]), -3, 3)


def scrutineer_shed(sc):
    """The scrutineering shed: a bow roof on fat posts over a chequered apron, a weighing dial, 14 x 8.6 m, 8.6 m."""
    g = sc.still()
    g.box(WHITE, (0, 0, -35), (1400, 800, 130))
    checks(g, -700, 700, -403, 15, 20, 14, odd=0)
    checks(g, -700, 700, -403, -5, 20, 14, odd=1)
    for i in range(4):
        g.box(YEL, (-525 + i * 350, -290, 31), (175, 175, 4))
    g.side(SILVER, arc(0, 430, 750, 170, 0, 180, 8), -430, 430)                                      # bow roof
    g.side(RED, arc(0, 430, 762, 182, 0, 180, 8) + arc(0, 430, 736, 156, 180, 0, 8), -446, -430)
    for x in (-660, -220, 220, 660):
        g.cyl(SILVER, (x, -370, 30), 22, 400, steps=6)
        g.cyl(RED, (x, -370, 150), 25, 40, steps=6)
    g.box(WHITE, (0, 388, 230), (1400, 20, 400))
    for sx in (-1, 1):
        g.box(WHITE, (sx * 690, 230, 230), (20, 300, 400))
    for x in (-450, -150, 150, 450):
        disc(g, NAVY, x, 378, 290, 44)
    g.box(SILVER, (-300, -40, 37), (440, 230, 14))            # the weighbridge plate
    g.box(RED, (-520, -40, 170), (12, 12, 280)); g.box(RED, (-80, -40, 170), (12, 12, 280)); g.box(RED, (-300, -40, 310), (452, 12, 12))   # height hoop
    for k in range(4):
        g.box(YEL if k % 2 else WHITE, (120, 100, 60 + k * 60), (10, 10, 60))                    # measuring staff
    g.box(WHITE, (420, 160, 62), (200, 90, 64)); g.box(NAVY, (420, 160, 97), (210, 100, 6))       # desk
    g.sphere(WHITE, (490, 160, 130), 14, steps=(3, 5), glow=GL)
    g.cyl(STEEL, (40, 250, 30), 7, 240, steps=6)              # the weighing dial on its post
    disc(g, WHITE, 40, 244, 300, 62, steps=10)
    n = sc.part(P.motion(swing=28, pivot=(40, 236, 300), axis=90.0, rate=0.3))
    n.box(RED, (40, 234, 322), (8, 4, 60))
    for x in (-400, 0, 400):
        g.box(STEEL, (x, -150, 400), (4, 4, 60)); g.sphere(WHITE, (x, -150, 360), 18, steps=(3, 5), glow=GL)
    g.box(STEEL, (-90, 0, 660), (8, 8, 160)); g.box(STEEL, (90, 0, 660), (8, 8, 160))               # chequered board on the roof
    g.box(WHITE, (0, 0, 780), (330, 10, 170))
    for (cx, cz) in ((-123, 822), (41, 822), (-41, 737), (123, 737)):
        g.box(YEL, (cx, -7, cz), (82, 4, 85)); g.box(YEL, (cx, 7, cz), (82, 4, 85))
    g.cyl(SILVER, (690, -390, 30), 6, 640, steps=6)
    pennant(sc, 690, -390, 640, RED, L=110, H=56, phase=0.6)


def caravan_diner(sc):
    """A polished caravan as a diner: red stripe, lit hatch, striped awning with a valance, stools. 9.1 x 5.6 m, 4.2 m."""
    g = sc.still()
    g.side(SILVER, rrect(800, 250, 105, z0=70), -130, 130)
    g.box(RED, (0, -132, 205), (600, 6, 26)); g.box(RED, (0, -132, 118), (700, 6, 14))
    g.box(NAVY, (-90, -132, 256), (330, 6, 84)); g.box(YEL, (-90, -136, 256), (300, 4, 62), glow=GW)   # the hatch
    g.box(NAVY, (-90, -139, 256), (8, 4, 62))
    g.box(WHITE, (-90, -166, 208), (350, 64, 10))             # counter
    g.box(NAVY, (255, -132, 182), (84, 6, 196)); disc(g, WHITE, 255, -136, 235, 20, th=4)
    disc(g, NAVY, -330, -131, 245, 34)
    for x in (-70, 60):
        rod(g, DARK, (x, -112, 62), (x, -142, 62), 50, steps=8); disc(g, SILVER, x, -142, 62, 20, th=4)
    for (x, y) in ((-340, -110), (340, -110), (-340, 110), (340, 110)):
        g.box(STEEL, (x, y, -15), (16, 16, 170))               # jacks: it stands, no tow car
    tilt_box(g, STEEL, (400, -60, 90), (520, 0, 90), 12, 12); tilt_box(g, STEEL, (400, 60, 90), (520, 0, 90), 12, 12)
    g.box(STEEL, (520, 0, 30), (14, 14, 130))
    for i in range(7):                                        # the awning, red and white
        x = -90 + (i - 3) * 72
        tilt_box(g, RED if i % 2 == 0 else WHITE, (x, -130, 322), (x, -430, 270), 72, 8)
    val = sc.part(P.motion(swing=9, pivot=(0, -430, 268), axis=0.0, rate=0.5))
    for i in range(7):
        val.box(WHITE if i % 2 == 0 else RED, (-90 + (i - 3) * 72, -430, 250), (72, 4, 34))
    for x in (-330, 150):
        g.cyl(SILVER, (x, -424, -100), 6, 372, steps=6)
    for i in range(5):                                        # bulbs along the awning's edge
        g.sphere(WHITE, (-282 + i * 96, -436, 282), 10, steps=(3, 5), glow=GL)
    for x in (-200, -90, 20):                                 # stools
        g.cyl(STEEL, (x, -230, -40), 5, 100, steps=4); g.cyl(RED, (x, -230, 60), 20, 9, steps=6)
    vent(g, -200, 0, 320)
    g.cyl(STEEL, (120, 60, 320), 11, 70, steps=6); g.cyl(SILVER, (120, 60, 390), 20, 16, steps=6, top_r=6)   # stove pipe
    g.side(RED, ccw([(230, 318), (395, 318), (395, 410)]), -6, 6)                                         # tail fin on the roof
    tilt_box(g, NAVY, (330, -330, -20), (350, -300, 90), 70, 6); tilt_box(g, STEEL, (372, -270, -20), (350, -300, 90), 70, 5)   # A-board
    g.box(WHITE, (255, -180, 20), (90, 60, 40)); g.box(WHITE, (255, -230, 0), (90, 50, 40))               # steps
    g.cyl(SILVER, (-420, -200, -30), 20, 90, steps=6); g.cyl(SILVER, (-420, -200, 60), 12, 18, steps=6)   # a churn
    g.box(WHITE, (-450, -110, 10), (60, 60, 80)); g.box(RED, (-450, -110, 53), (64, 64, 8))               # an ice box


def commentary_box(sc):
    """The commentary box on stilts: a round-ended cabin 4.6 m up with a lit window and four horn loudspeakers. 6.4 x 4.6 m, 10.6 m."""
    g = sc.still()
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, SILVER, (sx * 300, sy * 220, -100), (sx * 225, sy * 150, 452), 24, 24)
    tilt_box(g, STEEL, (-292, -214, 10), (228, -154, 430), 10, 10); tilt_box(g, STEEL, (292, -214, 10), (-228, -154, 430), 10, 10)
    tilt_box(g, STEEL, (-292, 214, 10), (228, 154, 430), 10, 10); tilt_box(g, STEEL, (292, 214, 10), (-228, 154, 430), 10, 10)
    g.plan(SILVER, stad(660, 420), 450, 470)
    g.plan(WHITE, stad(560, 330), 470, 720)
    g.plan(RED, stad(568, 338), 498, 524)
    g.plan(NAVY, stad(566, 336), 572, 668)                    # the wrap-round window
    g.box(YEL, (0, -170, 620), (230, 6, 78), glow=GW)
    for x in (-58, 58):
        g.box(NAVY, (x, -174, 620), (8, 4, 78))
    checks(g, -115, 115, -168, 548, 22, 6)
    tilt_box(g, SILVER, (0, -165, 700), (0, -245, 678), 250, 6)                                        # visor
    g.plan(SILVER, stad(640, 400), 720, 742)
    g.plan(RED, stad(340, 180), 742, 766)
    g.box(SILVER, (0, -205, 560), (240, 5, 6))
    for x in (-120, -40, 40, 120):
        g.box(SILVER, (x, -205, 515), (5, 5, 90))
    for sy in (-34, 34):                                      # ladder at the right end
        tilt_box(g, STEEL, (392, sy, -60), (332, sy, 470), 7, 7)
    for k in range(7):
        g.box(STEEL, (386 - k * 8.2, 0, 10 + k * 72), (6, 68, 6))
    g.cyl(SILVER, (0, 0, 766), 9, 150, steps=6)               # the horn cluster
    horn(g, 0, 0, 880, 0.0); horn(g, 0, 0, 880, 180.0); horn(g, 0, 0, 850, -90.0)
    g.cyl(SILVER, (-180, 40, 742), 5, 60, steps=4); horn(g, -180, 40, 800, -135.0, L=70, r=38)
    g.cyl(SILVER, (200, 60, 742), 3, 300, steps=4)            # aerial
    pennant(sc, 200, 60, 1030, YEL, L=90, H=46, swing=20, rate=0.5, phase=0.3)
    g.sphere(WHITE, (0, 0, 432), 16, steps=(3, 5), glow=GL)   # a lamp under the floor


def caravan_camp(sc):
    """Two teardrop caravans with a washing line between them, deck chairs, a stove pipe. 14.4 x 4.2 m, 3.6 m."""
    g = sc.still()
    for (cx, col, stripe, flip) in ((-440, SILVER, RED, 1.0), (440, WHITE, YEL, -1.0)):
        g.side(col, teardrop(540, 230, cx=cx, z0=55, flip=flip), -20, 200)
        g.box(stripe, (cx - flip * 40, -22, 150), (360, 6, 22))
        g.box(NAVY, (cx - flip * 40, -22, 145), (70, 7, 170))                  # door
        disc(g, NAVY, cx - flip * 150, -21, 215, 26)
        disc(g, WHITE, cx - flip * 40, -26, 200, 14, th=3)
        rod(g, DARK, (cx + flip * 40, -6, 48), (cx + flip * 40, -32, 48), 44, steps=8); disc(g, SILVER, cx + flip * 40, -32, 48, 18, th=3)
        g.box(STEEL, (cx - flip * 235, 90, -20), (14, 14, 150)); g.box(STEEL, (cx + flip * 230, 90, -20), (14, 14, 150))
        g.box(WHITE, (cx - flip * 40, -60, 15), (80, 50, 34))
        vent(g, cx - flip * 120, 90, 285)
    g.cyl(STEEL, (360, 150, 240), 10, 110, steps=6); g.cyl(SILVER, (360, 150, 350), 18, 14, steps=6, top_r=6)   # stove pipe
    g.cyl(STEEL, (-520, 120, 262), 3, 173, steps=4)                           # an aerial
    for k in range(3):
        g.box(STEEL, (-520, 120, 370 + k * 26), (5, 70 - k * 16, 5))
    for x in (-135, 135):                                                      # the washing line
        g.cyl(SILVER, (x, 60, -100), 5, 345, steps=6)
    g.box(STEEL, (0, 60, 238), (270, 4, 4))
    for i, (col, w, h) in enumerate(((WHITE, 46, 60), (RED, 34, 48), (YEL, 50, 40), (NAVY, 30, 66), (WHITE, 40, 44))):
        w1 = sc.part(P.motion(swing=13 + 2 * (i % 2), pivot=(-104 + i * 52, 60, 238), axis=0.0, rate=0.5 + 0.05 * i, phase=0.21 * i))
        w1.box(col, (-104 + i * 52, 60, 238 - h / 2.0), (w, 4, h))
    for (x, col) in ((-60, RED), (70, YEL)):                                   # deck chairs
        tilt_box(g, col, (x - 30, -170, 12), (x + 30, -170, 52), 52, 5)
        tilt_box(g, WHITE, (x + 30, -170, 52), (x + 48, -170, 112), 52, 5)
        g.box(STEEL, (x - 28, -170, -20), (5, 56, 70)); g.box(STEEL, (x + 34, -170, -5), (5, 56, 100))
    g.cyl(STEEL, (5, -230, -40), 4, 90, steps=4); g.cyl(WHITE, (5, -230, 50), 26, 5, steps=6)   # a little table
    g.cyl(SILVER, (5, -230, 55), 8, 16, steps=4)
    for i in range(3):                                                         # a small yellow awning on the white caravan
        tilt_box(g, YEL if i % 2 == 0 else WHITE, (420 + i * 50, -20, 262), (420 + i * 50, -150, 232), 50, 5)
    g.cyl(SILVER, (402, -146, -100), 4, 330, steps=4); g.cyl(SILVER, (538, -146, -100), 4, 330, steps=4)
    g.cyl(SILVER, (-250, -120, -30), 18, 80, steps=6); g.cyl(SILVER, (-250, -120, 50), 10, 14, steps=6)   # a churn


def salt_tower(sc):
    """The salt wash tower: three round-cornered storeys, a bucket elevator, a hopper over a heap. 12.6 x 8 m, 17 m."""
    g = sc.still()
    g.plan(SILVER, rbox(900, 700, 160), -100, 480)
    g.plan(RED, rbox(912, 712, 166), 200, 240)
    g.plan(WHITE, rbox(1000, 800, 200), 480, 505)
    g.plan(WHITE, rbox(740, 580, 140), 505, 940)
    g.plan(RED, rbox(752, 592, 146), 700, 730)
    g.plan(SILVER, rbox(840, 680, 180), 940, 962)
    g.plan(SILVER, rbox(540, 420, 120), 962, 1330)
    g.plan(YEL, rbox(548, 428, 124), 1180, 1262, glow=GW)     # the lamp room
    g.plan(SILVER, rbox(640, 520, 160), 1330, 1352)
    g.plan(RED, stad(400, 300), 1352, 1392)
    for x in (-126, -42, 42, 126):
        g.box(NAVY, (x, -216, 1221), (9, 8, 84))
    for x in (-280, -140, 140, 280):
        disc(g, NAVY, x, -350, 360, 42)
    for x in (-160, 0, 160):
        disc(g, NAVY, x, -290, 820, 38)
    g.box(NAVY, (0, -352, 90), (120, 8, 220)); tilt_box(g, RED, (0, -350, 215), (0, -430, 195), 170, 6)
    checks(g, -300, 300, -402, 492, 22, 12)
    checks(g, -240, 240, -342, 951, 20, 10)
    tilt_box(g, STEEL, (-420, -385, -30), (120, -385, 482), 60, 12)            # the outside stair, two flights
    tilt_box(g, SILVER, (-420, -415, 60), (120, -415, 572), 5, 6)
    g.box(STEEL, (200, -355, 492), (160, 90, 10))
    tilt_box(g, STEEL, (240, -325, 505), (-200, -325, 942), 56, 12)
    tilt_box(g, SILVER, (240, -352, 595), (-200, -352, 1032), 5, 6)
    g.box(SILVER, (390, 300, 720), (74, 74, 1640))                             # the bucket elevator
    g.box(RED, (390, 300, 1590), (120, 120, 130))
    tilt_box(g, STEEL, (390, 300, 1540), (150, 120, 1352), 40, 40)
    g.cyl(WHITE, (-640, 0, 430), 56, 230, steps=8, top_r=180)                  # the hopper on its legs
    g.cyl(SILVER, (-640, 0, 660), 186, 20, steps=8)
    for (dx, dy) in ((-150, -150), (150, -150), (-150, 150), (150, 150)):
        g.box(STEEL, (-640 + dx, dy, 280), (14, 14, 760))
    tilt_box(g, SILVER, (-370, 0, 800), (-600, 0, 690), 60, 50)
    g.cyl(WHITE, (-640, 0, -100), 260, 420, steps=8, top_r=14)                 # the heap under it
    for (x, y) in ((-150, 60), (60, -40), (170, 90)):
        vent(g, x, y, 1392)
    rod(g, STEEL, (330, -352, -60), (330, -352, 480), 12, steps=6)             # a rising main and its valve wheel
    disc(g, RED, 330, -364, 130, 30, th=5)
    for x in (-430, 430):
        g.sphere(WHITE, (x, -404, 440), 18, steps=(3, 5), glow=GL)
    g.cyl(SILVER, (0, 0, 1392), 6, 210, steps=6)                               # weather vane
    g.box(SILVER, (0, 0, 1480), (130, 5, 5)); g.box(SILVER, (0, 0, 1480), (5, 130, 5))
    v = sc.part(P.motion(swing=45, pivot=(0, 0, 1570), axis=P.UP, rate=0.1, phase=0.5))
    v.box(RED, (0, 0, 1570), (220, 5, 8))
    v.side(RED, ccw([(110, 1570), (66, 1596), (66, 1544)]), -3, 3)
    v.side(YEL, ccw([(-110, 1570), (-170, 1612), (-170, 1528)]), -3, 3)
    a, b = (390.0, 240.0, 1500.0), (500.0, -400.0, 505.0)                      # a line of flags from the elevator head
    tilt_box(g, STEEL, a, b, 4, 4)
    for i in range(6):
        p = lerp3(a, b, 0.14 + i * 0.14)
        f = sc.part(P.motion(swing=14, pivot=p, axis=0.0, rate=0.6, phase=0.3 * (i % 3)))
        f.side((YEL, RED, WHITE)[i % 3], ccw([(p[0] - 26, p[2]), (p[0] + 26, p[2]), (p[0], p[2] - 66)]), p[1] - 2.5, p[1] + 2.5)


# ------------------------------------------------------------------------------------------ props
def salt_cone(sc):
    """A white salt cone with a shovel, a barrow and a red flag on its top. 7 m across, 5.6 m."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -100), 340, 100, steps=10, top_r=310)
    g.cyl(WHITE, (0, 0, 0), 310, 430, steps=10, top_r=16)
    g.cyl(WHITE, (250, -170, -100), 130, 220, steps=8, top_r=10)
    g.cyl(STEEL, (0, 0, 420), 4, 120, steps=4)
    pennant(sc, 0, 0, 520, RED, L=90, H=50, swing=20, rate=0.5)
    rod(g, STEEL, (-190, -230, 60), (-150, -205, 200), 4, steps=4)             # shovel stuck in the salt
    g.box(SILVER, (-194, -232, 50), (34, 6, 46), rot=(0, 30, 0))
    g.box(STEEL, (-150, -205, 204), (30, 6, 6), rot=(0, 30, 0))
    tilt_box(g, RED, (-330, -290, 52), (-250, -290, 44), 70, 36)               # the barrow
    g.box(SILVER, (-290, -290, 68), (70, 60, 6))
    rod(g, DARK, (-345, -302, 22), (-345, -278, 22), 22, steps=8)
    tilt_box(g, STEEL, (-250, -322, 46), (-170, -322, 70), 5, 5); tilt_box(g, STEEL, (-250, -258, 46), (-170, -258, 70), 5, 5)
    g.box(STEEL, (-245, -322, 20), (5, 5, 46)); g.box(STEEL, (-245, -258, 20), (5, 5, 46))
    tilt_box(g, STEEL, (150, -300, -20), (150, -250, 150), 120, 5)              # a sieve leaning on the heap
    tilt_box(g, SILVER, (150, -299, 10), (150, -257, 130), 100, 4)
    for (x, y) in ((300, 60), (-310, 90)):
        g.cyl(RED, (x, y, -60), 5, 190, steps=4); g.box(YEL, (x, y, 122), (30, 4, 22))   # boundary stakes


def pennant_masts(sc):
    """Two striped masts with a line of pennants between them. 13 m long, 7.6 m."""
    g = sc.still()
    pts = []
    for sx in (-1, 1):
        x = sx * 600.0
        g.cyl(SILVER, (x, 0, -100), 9, 830, steps=6, top_r=6)
        for k in range(3):
            g.cyl(RED, (x, 0, 120 + k * 180), 11, 60, steps=6)
        g.cyl(WHITE, (x, 0, -100), 34, 170, steps=8); g.cyl(RED, (x, 0, 20), 36, 22, steps=8)
        g.sphere(YEL, (x, 0, 738), 14, steps=(3, 5))
        pennant(sc, x, 0, 690, RED if sx < 0 else YEL, L=110, H=56, swing=18, rate=0.42, phase=0.3 + 0.2 * sx)
    for (a, b) in (((-600, 0, 650), (-300, 0, 560)), ((-300, 0, 560), (0, 0, 530)), ((0, 0, 530), (300, 0, 560)), ((300, 0, 560), (600, 0, 650))):
        tilt_box(g, STEEL, a, b, 4, 4)
        for t in (0.2, 0.5, 0.8):
            p = lerp3(a, b, t)
            pts.append((p[0], p[2]))
    bunting(sc, pts, 0, (YEL, RED, WHITE, YEL, WHITE, RED), w=54, h=74, swing=16, rate=0.55)


def marker_drums(sc):
    """LOW: six red-and-white striped marker drums in a row and a striped plank. 8.2 m long, 1.05 m."""
    g = sc.still()
    for i in range(6):
        x = -375 + i * 150
        g.cyl(WHITE, (x, 0, -60), 30, 150, steps=6)
        g.cyl(RED, (x, 0, 12), 32, 22, steps=6); g.cyl(RED, (x, 0, 54), 32, 22, steps=6)
    for i in range(5):
        g.box(RED if i % 2 == 0 else WHITE, (-300 + i * 150, 0, 98), (150, 12, 14))


def tyre_rack(sc):
    """LOW: a rack of spare tyres, two axle stands, a trolley jack, an air bottle. 4.6 m long, 1.8 m."""
    g = sc.still()
    for x in (-150, 150):
        g.box(SILVER, (x, 0, 60), (8, 50, 240))                                 # fix round: 0.6 m of footing, not 1 m (the bounds were 2.81 m tall: a low piece that cast a shadow)
    for z in (40, 110):
        g.box(SILVER, (0, 0, z), (300, 50, 6))
    for row, z in enumerate((78, 148)):
        for k in range(4):
            x = -105 + k * 70
            rod(g, DARK, (x - 12, 0, z), (x + 12, 0, z), 33, steps=8)
            rod(g, SILVER if (k + row) % 2 else RED, (x - 14, 0, z), (x + 14, 0, z), 13, steps=6)
    for x in (178, 250):
        g.cyl(RED, (x, -20, -40), 16, 80, steps=4, top_r=5)                     # axle stands
    g.box(RED, (-215, -10, -10), (70, 26, 22)); tilt_box(g, STEEL, (-190, -10, 2), (-150, -10, 62), 5, 5)   # trolley jack
    g.cyl(WHITE, (-215, 30, -40), 12, 110, steps=6); g.cyl(RED, (-215, 30, 40), 13, 12, steps=6)            # air bottle


def windsock(sc):
    """A striped mast with a red-and-white windsock and an anemometer. 8.9 m."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -100), 40, 150, steps=8, top_r=30)
    g.cyl(SILVER, (0, 0, 50), 9, 780, steps=6, top_r=6)
    for k in range(3):
        g.cyl(RED, (0, 0, 150 + k * 190), 11, 70, steps=6)
    s = sc.part(P.motion(swing=22, pivot=(0, 0, 780), axis=P.UP, rate=0.22))
    g.cyl(STEEL, (0, 0, 760), 12, 44, steps=6)
    r0 = 40.0
    for k in range(4):
        r1 = r0 - 7.0
        s.cyl(RED if k % 2 == 0 else WHITE, (14 + k * 62, 0, 782), r0, 62, rot=(-90, 0, 0), steps=8, top_r=r1)
        r0 = r1
    g.box(SILVER, (0, 0, 640), (5, 130, 5))                    # the yard: anemometer and vane
    cups = sc.part(P.motion(travel=(-1.0, 0.7), base=(0.0, -65.0)))
    for k in range(3):
        a = math.radians(k * 120.0)
        cups.sphere(SILVER, (26 * math.cos(a), -65 + 26 * math.sin(a), 664), 10, steps=(3, 5))
        tilt_box(cups, STEEL, (0, -65, 660), (26 * math.cos(a), -65 + 26 * math.sin(a), 662), 4, 4)
    g.cyl(SILVER, (0, -65, 640), 3, 22, steps=4)
    v = sc.part(P.motion(swing=30, pivot=(0, 65, 664), axis=P.UP, rate=0.18, phase=0.4))
    g.cyl(SILVER, (0, 65, 640), 3, 24, steps=4)
    v.box(STEEL, (4, 65, 666), (64, 4, 4)); v.box(YEL, (-36, 65, 666), (26, 4, 26))
    g.sphere(RED, (0, 0, 838), 12, steps=(3, 5))
    for a in (30, 150, 270):                                   # three stay blocks at the foot
        g.box(STEEL, (70 * math.cos(math.radians(a)), 70 * math.sin(math.radians(a)), -40), (20, 20, 110))


def results_board(sc):
    """The results chalkboard on a trestle: tally bars (no words, no numerals), a chequered head, a lamp. 4 m, 3.5 m."""
    g = sc.still()
    for sx in (-1, 1):
        tilt_box(g, STEEL, (sx * 170, -70, -100), (sx * 170, 0, 320), 10, 10)
        tilt_box(g, STEEL, (sx * 170, 80, -100), (sx * 170, 0, 320), 10, 10)
    g.box(WHITE, (0, -8, 210), (392, 10, 232))
    g.box(NAVY, (0, -12, 200), (368, 8, 190))
    checks(g, -184, 184, -16, 312, 20, 8, col=YEL)
    checks(g, -184, 184, -16, 312, 20, 8, odd=1, col=DARK)
    marks = (RED, YEL, WHITE, SILVER, RED)
    bars = (230, 190, 250, 150, 205)
    for k in range(5):
        z = 272 - k * 34
        g.box(marks[k], (-158, -18, z), (22, 4, 22))
        g.box(WHITE, (-130 + bars[k] / 2.0, -18, z), (bars[k], 4, 9))
        g.box(WHITE, (150, -18, z), (30, 4, 9))
    g.box(STEEL, (0, -24, 100), (368, 24, 6))                  # chalk shelf
    tilt_box(g, STEEL, (150, 0, 326), (150, -60, 372), 5, 5)   # lamp on a gooseneck
    g.sphere(WHITE, (150, -64, 368), 14, steps=(3, 5), glow=GL)
    g.box(STEEL, (-250, -60, -30), (60, 50, 8)); g.box(STEEL, (-250, -60, 20), (50, 40, 8))    # step stool
    for (dx, dy) in ((-24, -20), (24, -20), (-24, 20), (24, 20)):
        g.box(STEEL, (-250 + dx, -60 + dy, -40), (5, 5, 120))
    g.cyl(SILVER, (250, -50, -40), 18, 70, steps=6, top_r=22)  # a bucket
    g.cyl(SILVER, (-178, 0, 320), 4, 150, steps=4)
    pennant(sc, -178, 0, 450, YEL, L=70, H=40, swing=20, rate=0.5, phase=0.1)


def speaker_pole(sc):
    """A loudspeaker pole: three horns, a cable drum at its foot. 7.3 m."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -100), 26, 160, steps=8)
    g.cyl(SILVER, (0, 0, 60), 10, 600, steps=6, top_r=7)
    g.cyl(RED, (0, 0, 200), 12, 50, steps=6); g.cyl(RED, (0, 0, 420), 11, 50, steps=6)
    g.box(STEEL, (0, 0, 610), (150, 8, 8)); g.box(STEEL, (0, -30, 580), (8, 70, 8))
    horn(g, 60, 0, 624, 0.0); horn(g, -60, 0, 624, 180.0); horn(g, 0, -60, 594, -90.0)
    g.box(STEEL, (0, -14, 120), (36, 16, 50)); disc(g, RED, 0, -22, 126, 8, th=3, steps=6)        # junction box
    rod(g, WHITE, (110, -20, -10), (110, 30, -10), 44, steps=8)                                    # cable drum
    rod(g, RED, (110, -26, -10), (110, -20, -10), 54, steps=8); rod(g, RED, (110, 30, -10), (110, 36, -10), 54, steps=8)
    g.cyl(SILVER, (0, 0, 660), 3, 60, steps=4)
    pennant(sc, 0, 0, 706, YEL, L=64, H=36, swing=22, rate=0.55, phase=0.7)


def sun_shade(sc):
    """A family pitch: two parasols, a striped windbreak, deck chairs, an ice box. 6.6 x 3.2 m, 2.9 m."""
    g = sc.still()
    for (x, y, c0, c1, ph) in ((-170, 0, RED, WHITE, 0.0), (190, 40, WHITE, YEL, 0.5)):
        g.cyl(SILVER, (x, y, -100), 5, 340, steps=4)
        t = sc.part(P.motion(swing=3, pivot=(x, y, 0), axis=20.0, rate=0.3, phase=ph))
        t.cyl(c0, (x, y, 228), 150, 42, steps=8, top_r=60)
        t.cyl(c1, (x, y, 270), 60, 20, steps=8, top_r=6)
        t.cyl(c1, (x, y, 218), 152, 10, steps=8)
    for i in range(6):                                         # the windbreak behind
        g.box(RED if i % 2 == 0 else WHITE, (-250 + i * 100, 150, 55), (100, 4, 110))
    for x in (-300, -100, 100, 300):
        g.cyl(STEEL, (x, 150, -100), 4, 230, steps=4)
    for (x, col) in ((-230, YEL), (-110, RED), (150, NAVY)):   # deck chairs
        tilt_box(g, col, (x - 28, -60, 10), (x + 28, -60, 46), 50, 5)
        tilt_box(g, WHITE, (x + 28, -60, 46), (x + 44, -60, 104), 50, 5)
        g.box(STEEL, (x - 26, -60, -25), (5, 54, 70)); g.box(STEEL, (x + 32, -60, -15), (5, 54, 110))
    g.box(WHITE, (40, -90, 0), (64, 44, 80)); g.box(RED, (40, -90, 43), (68, 48, 8))               # the ice box
    g.box(WHITE, (260, -70, 34), (90, 60, 6)); g.box(STEEL, (260, -70, -5), (6, 50, 76))           # a table, a flask
    g.cyl(SILVER, (250, -70, 37), 7, 26, steps=4); g.cyl(RED, (250, -70, 63), 7, 7, steps=4)
    for k in range(3):                                         # a tripod with field glasses
        a = math.radians(90 + k * 120)
        tilt_box(g, STEEL, (20 + 30 * math.cos(a), -130 + 30 * math.sin(a), -40), (20, -130, 120), 4, 4)
    g.box(NAVY, (20, -134, 126), (22, 30, 12))


def flood_mast(sc):
    """A generator cart with a floodlight mast: four round white lamps, steady. 3.2 m across, 8.6 m."""
    g = sc.still()
    g.box(SILVER, (0, 20, 60), (220, 120, 90))
    g.box(RED, (0, -41, 70), (220, 4, 20))
    disc(g, NAVY, -60, -40, 80, 20, steps=6)
    for x in (-70, 70):
        rod(g, DARK, (x, -46, 12), (x, -66, 12), 28, steps=8)
    for (sx, sy) in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
        tilt_box(g, STEEL, (sx * 100, 20 + sy * 50, 30), (sx * 150, 20 + sy * 110, -100), 8, 8)
    g.cyl(STEEL, (0, 20, 105), 13, 280, steps=6)
    g.cyl(SILVER, (0, 20, 385), 10, 260, steps=6)
    g.cyl(SILVER, (0, 20, 645), 7, 150, steps=6)
    g.cyl(RED, (0, 20, 385), 15, 16, steps=6); g.cyl(RED, (0, 20, 645), 12, 14, steps=6)
    g.box(STEEL, (0, 20, 790), (170, 8, 8)); g.box(STEEL, (0, 20, 730), (170, 8, 8)); g.box(STEEL, (0, 20, 760), (8, 8, 100))
    for (x, z) in ((-62, 800), (62, 800), (-62, 728), (62, 728)):
        g.cyl(SILVER, (x, 26, z), 34, 20, rot=(-90, -90, 0), steps=8, top_r=30)
        disc(g, WHITE, x, 4, z, 28, th=4, glow=1.0)
    g.cyl(STEEL, (-120, 70, 105), 18, 16, steps=6)             # a cable coil on the lid
    g.cyl(STEEL, (80, 60, 105), 9, 50, steps=6); g.cyl(STEEL, (80, 60, 155), 14, 8, steps=6)   # exhaust


# ------------------------------------------------------------------------------------------ the hero
def big_stopwatch(sc):
    """THE BIG STOPWATCH: a 36 m polished stopwatch standing on its winding crown, braced on its two pushers; a white dial
    (both faces) with twelve navy ticks and no numerals, one red hand sweeping slowly, a yellow-and-white chequered band."""
    g = sc.still()
    ZC, R, T = 2050.0, 1520.0, 190.0
    g.cyl(WHITE, (0, 0, -250), 980, 280, steps=12, top_r=520)                  # the salt mound it stands in
    g.cyl(SILVER, (0, 0, 0), 270, 300, steps=12)                               # the winding crown, knurled
    for k in range(6):
        a = k * 60.0
        g.box(RED, (270 * math.cos(math.radians(a)), 270 * math.sin(math.radians(a)), 150), (36, 70, 250), rot=(0, a, 0))
    g.cyl(SILVER, (0, 0, 300), 150, 240, steps=8)
    g.cyl(RED, (0, 0, 300), 190, 44, steps=8)
    g.box(NAVY, (0, -272, 105), (90, 10, 190)); disc(g, YEL, 0, -274, 235, 26, th=5, glow=GW)     # a door in the crown, for scale
    g.box(WHITE, (0, -330, 10), (150, 110, 40)); g.box(WHITE, (0, -420, -20), (150, 90, 40))
    rod(g, SILVER, (0, T, ZC), (0, -T, ZC), R, steps=28)                       # the case
    n = 28
    for k in range(n):                                                          # the chequered band round it
        a0, a1 = math.radians(k * 360.0 / n), math.radians((k + 1) * 360.0 / n)
        tilt_box(g, YEL if k % 2 else WHITE, ((R + 20) * math.cos(a0), 0, ZC + (R + 20) * math.sin(a0)),
                 ((R + 20) * math.cos(a1), 0, ZC + (R + 20) * math.sin(a1)), 2 * T - 120, 60)
    for f in (-1.0, 1.0):                                                       # the dial, both faces
        rod(g, WHITE, (0, f * (T - 2), ZC), (0, f * (T + 14), ZC), 1380, steps=28, glow=0.3)
        for k in range(12):
            a = math.radians(90.0 - k * 30.0)
            r0, wd = (1040.0, 84.0) if k % 3 == 0 else (1150.0, 56.0)
            tilt_box(g, NAVY, (r0 * math.cos(a), f * (T + 18), ZC + r0 * math.sin(a)), (1320 * math.cos(a), f * (T + 18), ZC + 1320 * math.sin(a)), 10, wd)
        rod(g, YEL, (0, f * (T + 30), ZC), (0, f * (T + 62), ZC), 96, steps=8)   # the hub
    for k in range(28):                                                         # steady bulbs round the bezel, the road's face
        a = math.radians((k + 0.5) * 360.0 / 28)
        g.sphere(WHITE, (1450 * math.cos(a), -(T + 6), ZC + 1450 * math.sin(a)), 40, steps=(3, 5), glow=GL)
    for k in range(60):                                                         # minute marks, the road's face only
        if k % 5:
            a = math.radians(90.0 - k * 6.0)
            tilt_box(g, NAVY, (1262 * math.cos(a), -(T + 18), ZC + 1262 * math.sin(a)), (1320 * math.cos(a), -(T + 18), ZC + 1320 * math.sin(a)), 8, 22)
    rod(g, SILVER, (0, -(T + 12), ZC + 600), (0, -(T + 22), ZC + 600), 270, steps=12)             # the small seconds dial
    sm = sc.part(P.motion(swing=180, pivot=(0, 0, ZC + 600), axis=90.0, rate=0.11, phase=0.3))
    tilt_box(sm, NAVY, (0, -(T + 28), ZC + 560), (0, -(T + 28), ZC + 830), 8, 30)
    h = sc.part(P.motion(swing=180, pivot=(0, 0, ZC), axis=90.0, rate=0.04))                      # the sweeping hand
    for f in (-1.0, 1.0):
        y = f * (T + 44)
        tilt_box(h, RED, (0, y, ZC - 300), (0, y, ZC + 1120), 12, 76, glow=0.35)
        h.side(RED, ccw([(-70, ZC + 1120), (70, ZC + 1120), (0, ZC + 1290)]), y - 6, y + 6, glow=0.35)
        rod(h, RED, (0, y - 6, ZC - 300), (0, y + 6, ZC - 300), 120, steps=8, glow=0.35)
    for sx in (-1.0, 1.0):                                                      # the two pushers, braced on the salt
        a, b = (sx * 960.0, 0.0, ZC - 1180.0), (sx * 1640.0, 0.0, -120.0)
        rod(g, SILVER, b, a, 78, steps=8)
        rod(g, RED, lerp3(a, b, 0.16), a, 118, steps=8)
        g.cyl(RED, (b[0], 0, -250), 180, 260, steps=8)
        g.cyl(SILVER, (b[0], 0, 10), 120, 40, steps=8)
    for sx in (-1, 1):                                                          # two small pennant masts at its foot
        g.cyl(SILVER, (sx * 620, -700, -250), 10, 900, steps=6)
        pennant(sc, sx * 620, -700, 620, YEL if sx < 0 else RED, L=170, H=90, swing=16, rate=0.35, phase=0.5 + 0.2 * sx)


JOBS = [
    ('SM_S1D5_TimingTower', timing_tower),
    ('SM_S1D5_PitShelter', pit_shelter),
    ('SM_S1D5_EvaporatorHouse', evaporator_house),
    ('SM_S1D5_ScrutineerShed', scrutineer_shed),
    ('SM_S1D5_CaravanDiner', caravan_diner),
    ('SM_S1D5_CommentaryBox', commentary_box),
    ('SM_S1D5_CaravanCamp', caravan_camp),
    ('SM_S1D5_SaltTower', salt_tower),
    ('SM_S1D5_SaltCone', salt_cone),
    ('SM_S1D5_PennantMasts', pennant_masts),
    ('SM_S1D5_MarkerDrums', marker_drums, STILL),
    ('SM_S1D5_TyreRack', tyre_rack, STILL),
    ('SM_S1D5_Windsock', windsock, dict(grow=(20, 110, 10), grow_down=(20, 110, 0))),
    ('SM_S1D5_ResultsBoard', results_board),
    ('SM_S1D5_SpeakerPole', speaker_pole),
    ('SM_S1D5_SunShade', sun_shade, dict(grow=(20, 20, 20), grow_down=(20, 20, 0))),
    ('SM_S1D5_FloodMast', flood_mast, STILL),
    ('SM_S1D5_BigStopwatch', big_stopwatch, dict(far=True, grow=(60, 60, 60), grow_down=(60, 60, 0))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
