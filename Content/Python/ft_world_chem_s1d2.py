"""Chemical Works, Stage 1, builder s1d2: district 2 "Reactor Block" (2.60 to 5.35 km), the research quarter.
/Game/World/ChemicalWorks/SM_S1D2_*. Run: import ft_world_chem_s1d2 as K; K.run()

Laboratories and institutes of the reactor's scientists: smooth drums and lozenges that overhang a narrower base,
white domes, ribbon windows that wrap the corners, portholes, fins. Nothing brick, nothing pitched.
Palette: purple #7560b8 (walls, the lead), deep purple #4e45a6 (bases, fins), violet #b44dff (ribbon windows, steady),
cyan #5ff0ff (thin trims, door lights), white #eef6ff (domes); with the kit's navy (the dark inside a door), steel,
its pale lilacs and its rain white (glass).
  BUILDINGS (8)   Institute     a four-storey drum under a white dome, eight fins, four ribbon windows (23 m)
                  Lecture       the lecture theatre: a stepped half-dome shell with a glazed front (11 m)
                  Observatory   a dome with a slot on a tall stalk, a gallery under it; the dome turns slowly (25 m)
                  Annex         the control annex: a lozenge on stilts, a ribbon window all round (14 m)
                  Igloos        the isotope store: three white igloos joined by a tube (5 m)
                  Hostel        the scientists' hostel: a five-storey slab with rounded ends and portholes, lab coats
                                on a line between the roof tanks (22 m)
                  Refectory     a glazed drum on a stalk with an awning and tables under it, a cowl turning (14 m)
                  Lodge         the porter's lodge: a small drum kiosk under a dome, a vane on top (5.6 m)
  TALL PROPS (5)  Dish (a dish aerial on a tripod, nodding), Weather (louvred box, cups turning, a vane), Footbridge
                  (a glazed tube on two legs), PendantLamp (a hooked arm, a cyan lamp under a white shade, steady),
                  FlaskSign (a board with a flask, an arrow and three dots: no words)
  LOW PROPS (5)   under 2 m, unlit, 0.9 m deep: FlaskRack (giant flasks in a frame), Cylinders (white gas cylinders
                  in a cradle), Molecule (a bench and a ball-and-stick model on a plinth), Blackboard (chalk drawings,
                  no words), VentCaps (three mushroom vents)
  HERO (1)        GreatRetort: a glass still 62 m tall on six purple legs, half full of violet liquid (the surface and
                  five bubbles rise and fall slowly), a swan neck over and down into a white receiver. Saved far=True
                  (it stands 150 m out; M_People stops moving past 180 m). Nothing on top of it but the neck.
Nothing here blinks; glow is steady and only on pieces stood 37 m and more out. No words anywhere.
"""
import math
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ChemicalWorks'
PUR, DEEP, VIO, CYAN, WHITE = '#7560b8', '#4e45a6', '#b44dff', '#5ff0ff', '#eef6ff'
NAVY, STEEL, PALE, LILAC, GLASS = '#1c1f4a', '#8f9bb3', '#d8d0f0', '#c391d8', '#dfe8ff'
G_WIN, G_TRIM, G_LAMP, G_LIQ = 0.6, 0.9, 1.2, 0.5
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))


# ------------------------------------------------------------------------------------------ helpers
def drum(g, col, z, r, h, steps=12, glow=0.0, top_r=None, x=0.0, y=0.0):
    g.cyl(col, (x, y, z), r, h, steps=steps, glow=glow, top_r=top_r)


def aim(g, col, a, b, r, steps=8, glow=0.0, top_r=None):
    """A cylinder (or a cone, with top_r) from a to b."""
    dx, dy, dz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
    ln = math.sqrt(dx * dx + dy * dy + dz * dz) or 1.0
    h = math.hypot(dx, dy)
    pitch = math.degrees(math.atan2(-h, dz)) if h > 1e-6 else (0 if dz >= 0 else 180)
    yaw = math.degrees(math.atan2(dy, dx)) if h > 1e-6 else 0
    g.cyl(col, a, r, ln, rot=(pitch, yaw, 0), steps=steps, glow=glow, top_r=top_r)


def stad(w, d, n=4, cx=0.0, cy=0.0):
    """A lozenge seen from above: w long (X), d deep, rounded ends. Counter-clockwise."""
    r, pts = d / 2.0, []
    hx = w / 2.0 - r
    for i in range(2 * n + 1):
        a = math.radians(-90.0 + 180.0 * i / (2 * n))
        pts.append((cx + hx + r * math.cos(a), cy + r * math.sin(a)))
    for i in range(2 * n + 1):
        a = math.radians(90.0 + 180.0 * i / (2 * n))
        pts.append((cx - hx + r * math.cos(a), cy + r * math.sin(a)))
    return pts


def half(r, c, n=8, k=1.0):
    """A half disc: (r cos a, c + k r sin a) for a = 0..180. Counter-clockwise."""
    return [(r * math.cos(math.radians(180.0 * i / n)), c + k * r * math.sin(math.radians(180.0 * i / n))) for i in range(n + 1)]


def port(g, col, x, y, z, r, nx, ny, t=8.0, steps=6, glow=0.0):
    """A porthole on a wall whose outward normal is (nx, ny)."""
    aim(g, col, (x - nx * 4.0, y - ny * 4.0, z), (x + nx * t, y + ny * t, z), r, steps=steps, glow=glow)


def door(g, x, y, z, w=150.0, h=230.0):
    """A dark door on a front wall (facing -Y) with a cyan light over it."""
    g.box(NAVY, (x, y, z + h / 2.0), (w, 14, h))
    g.box(CYAN, (x, y - 4, z + h + 14), (w + 30, 14, 12), glow=G_TRIM)


# ------------------------------------------------------------------------------------------ buildings
def institute(sc):
    g = sc.still()
    drum(g, DEEP, -200, 520, 540, 14)
    drum(g, DEEP, 340, 520, 60, 14, top_r=745)
    drum(g, PUR, 400, 745, 1500, 14)
    for k in range(4):
        drum(g, VIO, 550 + k * 375, 752, 120, 14, glow=G_WIN)
    drum(g, WHITE, 1130, 757, 26, 14)
    drum(g, WHITE, 1900, 775, 45, 14)
    g.sphere(WHITE, (0, 0, 1945), 700, scale=(1, 1, 0.5), steps=(6, 14))
    drum(g, CYAN, 2285, 70, 40, 8, glow=G_TRIM)
    drum(g, WHITE, 2325, 95, 16, 8)
    for k in range(8):
        a = math.radians(22.5 + 45.0 * k)
        g.box(DEEP, (790 * math.cos(a), 790 * math.sin(a), 1150), (100, 34, 1500), rot=(0, 22.5 + 45.0 * k, 0))
    door(g, 0, -518, 0, 160, 230)
    drum(g, WHITE, 262, 170, 16, 10, y=-560)
    g.box(PUR, (0, -600, -80), (320, 170, 200))
    for a in (-60.0, -120.0):
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        port(g, VIO, 520 * c, 520 * s, 190, 46, c, s, glow=G_WIN)


def lecture(sc):
    g = sc.still()
    y0, rr = -380.0, 1000.0
    g.plan(DEEP, stad(1700, 760, 3, cy=y0 + 420), -200, 180)
    for k in range(6):
        a0, a1 = math.radians(15.0 * k), math.radians(15.0 * (k + 1))
        g.plan(PUR if k < 2 else WHITE, half(rr * math.cos(a0), y0, 8), 180 + rr * 0.92 * math.sin(a0), 180 + rr * 0.92 * math.sin(a1))
    g.side(VIO, half(860, 180, 10, 0.92), y0 - 14, y0 + 2, glow=G_WIN)
    for x in (-580, -290, 290, 580):
        h = 0.92 * math.sqrt(860.0 * 860.0 - x * x)
        g.box(WHITE, (x, y0 - 18, 180 + h / 2.0), (22, 12, h))
    g.box(WHITE, (0, y0 - 18, 510), (1562, 12, 22))
    g.box(WHITE, (0, y0 - 18, 700), (22, 12, 540))
    door(g, 0, y0 - 22, 180, 190, 240)
    g.box(PUR, (0, y0 - 80, 50), (760, 130, 260))
    g.box(PUR, (0, y0 - 165, -10), (980, 90, 260))
    g.box(CYAN, (0, y0 - 6, 186), (1700, 20, 12), glow=G_TRIM)


def observatory(sc):
    g = sc.still()
    drum(g, DEEP, -200, 270, 260, 10)
    drum(g, DEEP, 60, 200, 1440, 10)
    for a in (30.0, 150.0, 270.0):
        c, s = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(g, DEEP, (430 * c, 430 * s, -150), (185 * c, 185 * s, 560), 46, 46)
    drum(g, PUR, 1500, 200, 200, 12, top_r=540)
    drum(g, CYAN, 1692, 566, 14, 12)   # fix round: unlit (a lit ring under a ball on a stalk read as the boss's coil)
    drum(g, WHITE, 1700, 560, 110, 12)
    drum(g, PUR, 1810, 430, 200, 12)
    drum(g, VIO, 1870, 436, 90, 12)
    for k in range(5):
        a = math.radians(-90.0 + 42.0 * (k - 2))
        port(g, CYAN, 200 * math.cos(a), 200 * math.sin(a), 330 + 250 * k, 34, math.cos(a), math.sin(a))
    g.box(NAVY, (0, -200, 180), (110, 14, 210))
    d = sc.part(P.motion(swing=40, pivot=(0, 0, 2010), axis=P.UP, rate=0.02))
    d.sphere(WHITE, (0, 0, 2010), 430, steps=(6, 12))

    def on(e, r=434.0):
        return (0.0, -r * math.cos(math.radians(e)), 2010 + r * math.sin(math.radians(e)))
    for e in (24.0, 50.0, 76.0):
        tilt_box(d, NAVY, on(e - 13), on(e + 13), 120, 30)
    aim(d, WHITE, (0, -150, 2180), (0, -460, 2410), 55, steps=8)
    aim(d, CYAN, (0, -458, 2408), (0, -466, 2414), 46, steps=8, glow=G_TRIM)


def annex(sc):
    g = sc.still()
    for x in (-560, 0, 560):
        for y in (-190, 190):
            g.box(DEEP, (x, y, 200), (56, 56, 800))
    drum(g, PUR, -200, 150, 800, 10, x=-280)
    door(g, -280, -152, 0, 110, 210)
    tilt_box(g, WHITE, (330, -250, -60), (-120, -250, 590), 90, 16)
    g.plan(DEEP, stad(1500, 520, 3), 560, 620)
    g.plan(PUR, stad(1700, 700, 4), 620, 1180)
    g.plan(VIO, stad(1712, 712, 4), 820, 960, glow=G_WIN)
    g.plan(CYAN, stad(1716, 716, 4), 790, 804, glow=G_TRIM)
    g.plan(WHITE, stad(1750, 750, 4), 1180, 1214)
    g.sphere(WHITE, (-430, 0, 1214), 230, scale=(1, 1, 0.6), steps=(5, 10))
    for x in (150, 380, 610):
        g.box(DEEP, (x, 0, 1290), (24, 420, 150))
    drum(g, STEEL, 1214, 7, 200, 6, x=760)
    g.box(CYAN, (760, 0, 1422), (16, 16, 16), glow=G_TRIM)
    for x in (-420, 420):
        port(g, WHITE, x, -260, 590, 24, 0, -1)


def igloos(sc):
    g = sc.still()
    for x in (-640, 0, 640):
        drum(g, DEEP, -200, 285, 270, 10, x=x)
        g.sphere(WHITE, (x, 0, 70), 330, scale=(1, 1, 1.2), steps=(6, 10))
        drum(g, CYAN, 62, 337, 14, 10, glow=G_TRIM, x=x)
        aim(g, PUR, (x, -240, 150), (x, -400, 150), 105, steps=8)
        aim(g, NAVY, (x, -398, 150), (x, -405, 150), 78, steps=8)
        g.box(DEEP, (x, -330, -60), (230, 180, 260))
    for x in (-320, 320):
        aim(g, PUR, (x - 150, 0, 190), (x + 150, 0, 190), 95, steps=8)
        aim(g, VIO, (x - 12, 0, 190), (x + 12, 0, 190), 104, steps=8, glow=G_WIN)
    drum(g, STEEL, 440, 24, 56, 6)
    drum(g, WHITE, 496, 52, 28, 8, top_r=16)
    for k, x in enumerate((-700, -640, -580)):
        aim(g, PUR, (x, -300 - 2 * abs(k - 1), 300), (x, -312, 302), 15, steps=6)


def hostel(sc):
    g = sc.still()
    g.plan(DEEP, stad(1700, 560, 3), -200, 330)
    g.plan(PUR, stad(2200, 800, 4), 330, 2000)
    for k in (1, 3):
        g.plan(WHITE, stad(2212, 812, 4), 318 + 334 * k, 342 + 334 * k)
    for k in range(5):
        for i, x in enumerate((-640, -320, 0, 320, 640)):
            lit = (k * 5 + i * 3) % 4 != 0
            port(g, VIO if lit else NAVY, x, -400, 500 + 334 * k, 64, 0, -1, glow=G_WIN if lit else 0.0)
    for x in (-1101, 1101):
        g.box(CYAN, (x, 0, 1165), (12, 60, 1560), glow=G_TRIM)
    g.plan(WHITE, stad(2260, 860, 4), 2000, 2040)
    for x in (-650, 650):
        g.sphere(WHITE, (x, 0, 2040), 230, scale=(1, 1, 0.7), steps=(5, 10))
    for x in (-420, 420):
        g.box(STEEL, (x, 0, 2100), (8, 8, 130))
    rod(g, STEEL, (-420, 0, 2155), (420, 0, 2155), 3, steps=5)
    sw = sc.part(P.motion(swing=10, pivot=(0, 0, 2155), axis=0, rate=0.3))
    for x, col in ((-270, WHITE), (-90, PALE), (90, WHITE), (270, LILAC)):
        sw.box(col, (x, 0, 2106), (110, 5, 90))
    door(g, 0, -284, 0, 170, 240)
    drum(g, WHITE, 300, 215, 22, 10, y=-400)
    g.box(PUR, (0, -350, -90), (360, 150, 220))


def refectory(sc):
    g = sc.still()
    drum(g, DEEP, -200, 300, 760, 12)
    drum(g, PUR, 560, 300, 200, 14, top_r=800)
    drum(g, CYAN, 752, 806, 14, 14, glow=G_TRIM)
    drum(g, PUR, 760, 800, 420, 14)
    drum(g, VIO, 850, 807, 170, 14, glow=G_WIN)
    for k in range(7):
        a = 12.0 + 360.0 * k / 7.0
        g.box(DEEP, (812 * math.cos(math.radians(a)), 812 * math.sin(math.radians(a)), 935), (30, 36, 190), rot=(0, a, 0))
    drum(g, WHITE, 1180, 835, 36, 14)
    g.sphere(WHITE, (0, 0, 1216), 600, scale=(1, 1, 0.36), steps=(5, 14))
    drum(g, VIO, 286, 524, 14, 12)
    drum(g, WHITE, 300, 520, 50, 12, top_r=310)
    door(g, 0, -298, 0, 150, 220)
    for a in (-150.0, -30.0, 40.0, 140.0):
        x, y = 620 * math.cos(math.radians(a)), 620 * math.sin(math.radians(a))
        drum(g, DEEP, -80, 8, 150, 6, x=x, y=y)
        drum(g, WHITE, 70, 58, 9, 8, x=x, y=y)
    drum(g, STEEL, 1370, 22, 70, 6, x=330)
    c = sc.part(P.motion(travel=(-1, 0.07), base=(330, 0)))
    c.box(WHITE, (330, -20, 1470), (70, 100, 56))
    c.box(DEEP, (330, 62, 1480), (8, 70, 70))


def lodge(sc):
    g = sc.still()
    drum(g, DEEP, -200, 200, 290, 10)
    drum(g, PUR, 90, 200, 40, 10, top_r=262)
    drum(g, PUR, 130, 262, 210, 10)
    drum(g, VIO, 190, 267, 90, 10, glow=G_WIN)
    drum(g, WHITE, 340, 282, 20, 10)
    g.sphere(WHITE, (0, 0, 360), 262, scale=(1, 1, 0.5), steps=(5, 10))
    tilt_box(g, WHITE, (0, -255, 305), (0, -370, 262), 210, 8)
    g.box(WHITE, (0, -285, 150), (180, 50, 8))
    g.box(NAVY, (262, 0, 230), (12, 90, 190))
    g.box(CYAN, (268, 0, 338), (10, 110, 10), glow=G_TRIM)
    g.box(WHITE, (-340, -110, 45), (150, 40, 8))
    for x in (-400, -280):
        g.box(DEEP, (x, -110, -5), (10, 36, 92))
    drum(g, STEEL, 480, 5, 70, 5)
    v = sc.part(P.motion(swing=25, pivot=(0, 0, 548), axis=P.UP, rate=0.15))
    v.box(WHITE, (10, 0, 550), (92, 5, 12))
    v.box(VIO, (-40, 0, 558), (26, 5, 34))


# ------------------------------------------------------------------------------------------ tall props (37 m and out)
def dish(sc):
    g = sc.still()
    for a in (90.0, 210.0, 330.0):
        tilt_box(g, DEEP, (120 * math.cos(math.radians(a)), 120 * math.sin(math.radians(a)), -120), (0, 0, 250), 14, 14)
    drum(g, DEEP, 232, 26, 30, 8)
    drum(g, STEEL, 250, 10, 100, 6)
    g.box(WHITE, (170, 0, 10), (60, 44, 140))
    g.box(VIO, (170, -24, 52), (40, 4, 12), glow=G_WIN)
    g.box(CYAN, (170, -24, 28), (10, 4, 10), glow=G_TRIM)
    d = sc.part(P.motion(swing=10, pivot=(0, 0, 360), axis=0, rate=0.07))
    aim(d, WHITE, (0, 0, 360), (0, -64, 424), 24, steps=10, top_r=175)
    aim(d, PALE, (0, -63, 423), (0, -66, 426), 158, steps=10)
    aim(d, STEEL, (0, -64, 424), (0, -140, 500), 4, steps=5)
    d.box(VIO, (0, -140, 500), (16, 16, 16))
    aim(d, DEEP, (0, 0, 360), (0, 46, 314), 20, steps=6)


def weather(sc):
    g = sc.still()
    g.box(PALE, (0, 0, -45), (540, 150, 100))
    for x in (-28, 28):
        for y in (-22, 22):
            g.box(DEEP, (x, y, 75), (8, 8, 150))
    g.box(WHITE, (0, 0, 190), (76, 64, 80))
    for k in range(4):
        g.box(PUR, (0, -33, 162 + k * 18), (60, 5, 9))
    g.box(WHITE, (0, 0, 236), (96, 84, 10))
    drum(g, STEEL, 0, 6, 520, 6, x=110)
    c = sc.part(P.motion(travel=(-1, 0.35), base=(110, 0)))
    c.cyl(DEEP, (110, 0, 508), 9, 16, steps=6)
    for a in (0.0, 120.0, 240.0):
        ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
        tilt_box(c, STEEL, (110, 0, 516), (110 + 62 * ca, 62 * sa, 516), 5, 5)
        c.cyl(WHITE, (110 + 66 * ca, 66 * sa, 505), 13, 22, steps=6)
    drum(g, STEEL, 0, 6, 420, 6, x=-110)
    v = sc.part(P.motion(swing=28, pivot=(-110, 0, 430), axis=P.UP, rate=0.11))
    v.box(WHITE, (-100, 0, 430), (120, 5, 10))
    v.box(CYAN, (-158, 0, 442), (34, 5, 40), glow=0.5)
    v.box(VIO, (-46, 0, 430), (14, 7, 16))
    drum(g, DEEP, 0, 8, 120, 6, x=-200)
    drum(g, WHITE, 120, 14, 40, 8, top_r=26, x=-200)
    for y in (-70, 70):
        g.box(DEEP, (0, y, 60), (520, 6, 8))
        for x in (-255, 255):
            g.box(DEEP, (x, y, 35), (8, 8, 70))


def footbridge(sc):
    g = sc.still()
    for x in (-520, 520):
        tilt_box(g, DEEP, (x - 150, 0, -150), (x, 0, 440), 60, 60)
        tilt_box(g, DEEP, (x + 150, 0, -150), (x, 0, 440), 60, 60)
        g.box(DEEP, (x, 0, -110), (420, 90, 80))
    g.box(PUR, (0, 0, 450), (1600, 230, 60))
    rod(g, VIO, (-790, 0, 575), (790, 0, 575), 128, steps=8, glow=G_WIN)
    for x in (-800, -400, 0, 400, 800):
        rod(g, WHITE, (x - 16, 0, 575), (x + 16, 0, 575), 146, steps=10)
    g.box(WHITE, (0, 0, 712), (1600, 110, 18))
    for s in (-1, 1):
        rod(g, WHITE, (s * 816, 0, 575), (s * 823, 0, 575), 150, steps=10)   # fix round: a plain white end cap (was a lit cyan disc with a dark centre: a ring facing the driver)
    for x in (-400, 0, 400):
        g.box(CYAN, (x, 0, 416), (60, 20, 10), glow=G_TRIM)


def pendant_lamp(sc):
    g = sc.still()
    drum(g, WHITE, -100, 26, 150, 8, x=-60)
    arm = [(-60, 50), (-60, 300), (-40, 380), (10, 425), (60, 405)]
    for a, b in zip(arm, arm[1:]):
        rod(g, DEEP, (a[0], 0, a[1]), (b[0], 0, b[1]), 8, steps=6)
    drum(g, PUR, 200, 13, 20, 6, x=-60)
    rod(g, STEEL, (60, 0, 372), (60, 0, 408), 3, steps=5)
    drum(g, WHITE, 342, 48, 30, 10, top_r=14, x=60)
    drum(g, CYAN, 330, 30, 12, 8, glow=G_LAMP, x=60)


def flask_sign(sc):
    g = sc.still()
    for x in (-95, 95):
        g.box(DEEP, (x, 4, 95), (12, 12, 390))
    g.box(PUR, (0, 2, 235), (264, 8, 190))
    g.box(WHITE, (0, -3, 235), (244, 8, 170))
    aim(g, VIO, (-70, -6, 212), (-70, -11, 212), 34, steps=8)
    g.box(VIO, (-70, -9, 258), (16, 5, 40))
    g.box(VIO, (-70, -9, 280), (30, 5, 7))
    g.box(PUR, (36, -9, 215), (90, 5, 12))
    tilt_box(g, PUR, (62, -9, 240), (86, -9, 215), 5, 12)
    tilt_box(g, PUR, (62, -9, 190), (86, -9, 215), 5, 12)
    for x in (10, 36, 62):
        aim(g, LILAC, (x, -6, 285), (x, -11, 285), 8, steps=6)
    g.box(CYAN, (0, -10, 326), (220, 8, 8), glow=G_TRIM)
    g.box(WHITE, (0, -10, 334), (240, 24, 6))


# ------------------------------------------------------------------------------------------ low props (23.5 m, under 2 m, unlit)
def flask_rack(sc):
    g = sc.still()
    for x in (-310, 310):
        g.box(DEEP, (x, 0, 78), (12, 60, 216))
    g.box(DEEP, (0, 0, 20), (640, 64, 10))
    g.box(DEEP, (0, 30, 180), (640, 10, 10))
    for i, x in enumerate((-240, -120, 0, 120, 240)):
        if i % 2 == 0:
            g.sphere(VIO, (x, 0, 63), 38, steps=(5, 8))
            drum(g, WHITE, 94, 11, 56, 6, x=x)
        else:
            drum(g, VIO, 25, 40, 34, 8, top_r=28, x=x)
            drum(g, WHITE, 59, 28, 53, 8, top_r=11, x=x)
            drum(g, WHITE, 112, 11, 38, 6, x=x)
        drum(g, WHITE, 150, 15, 6, 6, x=x)
        drum(g, PUR, 156, 9, 12, 6, x=x)


def cylinders(sc):
    g = sc.still()
    g.box(DEEP, (0, 0, -5), (560, 64, 50))
    for i in range(6):
        x = -225 + 90 * i
        drum(g, WHITE, 20, 27, 120, 8, x=x)
        drum(g, WHITE, 140, 27, 18, 8, top_r=13, x=x)
        drum(g, PUR, 158, 17, 22, 8, x=x)
        drum(g, VIO if i % 3 == 1 else PUR, 100, 28.5, 14, 8, x=x)
    for x in (-275, 275):
        g.box(DEEP, (x, 0, 75), (12, 50, 140))
    for y in (-30, 30):
        g.box(DEEP, (0, y, 95), (560, 6, 10))


def molecule(sc):
    g = sc.still()
    g.box(WHITE, (-150, 0, 45), (230, 42, 8))
    for x in (-240, -60):
        g.box(DEEP, (x, 0, 6), (12, 40, 72))
    g.box(PALE, (-150, 0, 52), (44, 30, 6), rot=(0, 18, 0))
    g.box(DEEP, (140, 0, 25), (64, 60, 110))
    g.box(WHITE, (140, 0, 84), (74, 66, 8))
    drum(g, STEEL, 88, 4, 30, 5, x=140)
    c = (140.0, 0.0, 135.0)
    g.sphere(WHITE, c, 19, steps=(4, 8))
    for k, (dx, dy, dz) in enumerate(((-38, -8, 24), (40, 6, 22), (4, -14, 44), (-22, 12, -20), (30, -10, -18))):
        p = (c[0] + dx, c[1] + dy, c[2] + dz)
        rod(g, STEEL, c, p, 3, steps=5)
        g.sphere(VIO if k % 2 == 0 else LILAC, p, 12, steps=(4, 6))


def blackboard(sc):
    g = sc.still()
    g.box(DEEP, (0, 0, 125), (300, 8, 130))
    g.box(WHITE, (0, 0, 191), (312, 12, 8))
    g.box(WHITE, (0, 0, 59), (312, 12, 8))
    for x in (-154, 154):
        g.box(WHITE, (x, 0, 125), (8, 12, 138))
    g.box(WHITE, (0, -11, 60), (200, 14, 5))
    for x in (-120, 120):
        tilt_box(g, PUR, (x, -38, -30), (x, 0, 120), 8, 8)
        tilt_box(g, PUR, (x, 38, -30), (x, 0, 120), 8, 8)
    aim(g, WHITE, (-95, -3, 102), (-95, -8, 102), 20, steps=8)
    g.box(WHITE, (-95, -6, 136), (10, 4, 34))
    aim(g, VIO, (-95, -6, 99), (-95, -10, 99), 12, steps=6)
    g.box(CYAN, (-22, -6, 112), (50, 4, 6))
    tilt_box(g, CYAN, (-6, -6, 126), (6, -6, 112), 4, 6)
    tilt_box(g, CYAN, (-6, -6, 98), (6, -6, 112), 4, 6)
    tri = [(40, 90), (100, 90), (70, 140)]
    for a, b in zip(tri, tri[1:] + tri[:1]):
        tilt_box(g, WHITE, (a[0], -6, a[1]), (b[0], -6, b[1]), 4, 6)
    zig = [(-60, 150), (-30, 172), (0, 156), (36, 178), (70, 162)]
    for a, b in zip(zig, zig[1:]):
        tilt_box(g, LILAC, (a[0], -6, a[1]), (b[0], -6, b[1]), 4, 6)
    for x in (112, 126, 140):
        aim(g, LILAC, (x, -3, 110), (x, -8, 110), 5, steps=6)
    drum(g, DEEP, -30, 6, 72, 6, x=215)
    drum(g, WHITE, 42, 20, 8, 8, x=215)


def vent_caps(sc):
    g = sc.still()
    g.box(DEEP, (0, 0, -8), (480, 84, 44))
    for x, h in ((-160, 95), (0, 128), (165, 62)):
        drum(g, DEEP, 14, 21, h, 8, x=x)
        drum(g, VIO, 14 + h, 30, 20, 8, x=x)
        drum(g, WHITE, 34 + h, 42, 30, 10, top_r=14, x=x)
    g.box(WHITE, (85, 0, 34), (50, 40, 40))
    g.box(PUR, (85, 0, 57), (56, 46, 6))


# ------------------------------------------------------------------------------------------ the hero
def great_retort(sc):
    g = sc.still()
    x0, x1, zc, rr, n = -1050.0, 1950.0, 3000.0, 1500.0, 16
    lats = [-90, -72, -54, -36, -18, 0, 18, 36, 54, 72, 90]
    for i in range(10):
        a0, a1 = math.radians(lats[i]), math.radians(lats[i + 1])
        z0, z1 = zc + rr * math.sin(a0), zc + rr * math.sin(a1)
        liquid = lats[i] < 0
        g.cyl(VIO if liquid else GLASS, (x0, 0, z0), max(rr * math.cos(a0), 30.0), z1 - z0, steps=n,
              glow=G_LIQ if liquid else 0.06, top_r=max(rr * math.cos(a1), 30.0))
    for k in range(5):                                           # graduations on the glass
        a = math.radians(8.0 + 9.0 * k)
        g.box(WHITE, (x0, -rr * math.cos(a) - 4, zc + rr * math.sin(a)), (300 if k % 2 == 0 else 160, 30, 36))
    s = sc.part(P.motion(bob=-30, rate=0.09))                    # the liquid's surface
    s.cyl(LILAC, (x0, 0, zc - 22), rr + 14, 46, steps=n, glow=0.8)
    for az, lat, r, rate, ph in ((-90, -22, 190, 0.13, 0.0), (-55, -36, 150, 0.17, 0.3), (-128, -30, 170, 0.11, 0.6),
                                 (-20, -18, 140, 0.15, 0.8), (-160, -42, 130, 0.19, 0.45)):
        q = rr * math.cos(math.radians(lat)) - 0.35 * r
        b = sc.part(P.motion(bob=-170, rate=rate, phase=ph))
        b.sphere(PALE, (x0 + q * math.cos(math.radians(az)), q * math.sin(math.radians(az)), zc + rr * math.sin(math.radians(lat))), r,
                 steps=(5, 8), glow=0.5)
    legs = []
    for k in range(6):                                           # six legs, a collar, braces
        a = math.radians(30.0 + 60.0 * k)
        foot = (x0 + 1560 * math.cos(a), 1560 * math.sin(a), -300.0)
        top = (x0 + 1130 * math.cos(a), 1130 * math.sin(a), 2120.0)
        tilt_box(g, PUR, foot, top, 170, 170)
        drum(g, DEEP, -300, 230, 380, 8, x=foot[0], y=foot[1])
        legs.append(tuple(foot[j] + (top[j] - foot[j]) * 0.45 for j in range(3)))
    for k in range(6):
        tilt_box(g, DEEP, legs[k], legs[(k + 1) % 6], 60, 60)
    drum(g, DEEP, 2060, 1250, 130, n, x=x0)
    drum(g, DEEP, -300, 260, 1540, 10, x=x0)                     # the burner under it: no flame, a steady violet glow
    drum(g, DEEP, 1240, 260, 160, 10, top_r=120, x=x0)
    drum(g, VIO, 1400, 110, 20, 10, glow=0.8, x=x0)
    drum(g, PUR, 4400, 380, 120, 12, x=x0)                       # the swan neck
    neck = [(x0, 4430, 340), (x0, 5250, 300), (x0 + 330, 5720, 285), (x0 + 900, 5900, 270), (x0 + 1600, 5800, 250),
            (x0 + 2250, 5350, 225), (x0 + 2750, 4550, 195), (x1, 3400, 165), (x1, 1450, 140)]
    for a, b in zip(neck, neck[1:]):
        aim(g, GLASS, (a[0], 0, a[1]), (b[0], 0, b[1]), a[2], steps=10, glow=0.06, top_r=b[2])
    for p in neck[1:-1]:
        g.sphere(GLASS, (p[0], 0, p[1]), p[2] * 1.02, steps=(4, 10), glow=0.06)
    tip = (x0 - 1560.0, 0.0, 4480.0)                             # a side neck with a stopper
    aim(g, GLASS, (x0 - 1120.0, 0.0, 3950.0), tip, 130, steps=8, glow=0.06)
    aim(g, WHITE, tip, (tip[0] - 60, 0, tip[2] + 72), 150, steps=8)
    drum(g, DEEP, -300, 660, 420, 14, x=x1)                      # the receiver
    drum(g, WHITE, 120, 600, 1180, 14, x=x1)
    g.sphere(WHITE, (x1, 0, 1300), 600, scale=(1, 1, 0.35), steps=(5, 14))
    drum(g, PUR, 1400, 200, 150, 10, x=x1)
    for z in (300, 1100):
        drum(g, PUR, z, 612, 50, 14, x=x1)
    g.box(VIO, (x1, -596, 700), (120, 30, 720), glow=0.7)
    g.box(NAVY, (x1 + 330, -520, 230), (130, 60, 220))


JOBS = [('SM_S1D2_Institute', institute, STILL), ('SM_S1D2_Lecture', lecture, STILL),
        ('SM_S1D2_Observatory', observatory, dict(grow=(10, 10, 10), grow_down=(10, 10, 0))),
        ('SM_S1D2_Annex', annex, STILL), ('SM_S1D2_Igloos', igloos, STILL),
        ('SM_S1D2_Hostel', hostel, dict(grow=(10, 20, 10), grow_down=(10, 20, 0))),
        ('SM_S1D2_Refectory', refectory, dict(grow=(10, 10, 10), grow_down=(10, 10, 0))),
        ('SM_S1D2_Lodge', lodge, dict(grow=(20, 20, 10), grow_down=(20, 20, 0))),
        ('SM_S1D2_Dish', dish, dict(grow=(20, 60, 60), grow_down=(20, 60, 0))),
        ('SM_S1D2_Weather', weather, dict(grow=(40, 90, 20), grow_down=(40, 90, 0))),
        ('SM_S1D2_Footbridge', footbridge, STILL), ('SM_S1D2_PendantLamp', pendant_lamp, STILL),
        ('SM_S1D2_FlaskSign', flask_sign, STILL),
        ('SM_S1D2_FlaskRack', flask_rack, STILL), ('SM_S1D2_Cylinders', cylinders, STILL),
        ('SM_S1D2_Molecule', molecule, STILL), ('SM_S1D2_Blackboard', blackboard, STILL),
        ('SM_S1D2_VentCaps', vent_caps, STILL),
        ('SM_S1D2_GreatRetort', great_retort, dict(far=True, grow=(20, 20, 40), grow_down=(20, 20, 0)))]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
