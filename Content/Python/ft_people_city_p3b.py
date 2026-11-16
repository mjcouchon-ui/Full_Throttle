"""FULL THROTTLE (Unreal) the population, level 7: City Center, pass 3, builder p3b ("scenes").
/Game/People/CityCenter/SM_P3B_*.

Twelve story groups that could only be in this city at midnight, one mesh a scene, 8 to 13 figures each doing
something different, with the props that tell the story. Placed by the block C:/ftwork/p3/L7b.inl.

WHERE THEY STAND (FTDressL7.inl, ft_dress_metro.py): the avenue is a canyon, and every metre of ground from the kerb to
the towers is the dressing's. What is free is UP: the roofs of the podiums that carry nobody yet.
  - TEN ROOF SCENES, all drawn for one stage: 7.6 m along the street (x -380 .. 380) and 2.65 m deep (y -345 .. -80),
    where y = 0 is the podium's middle line and the roof's front edge is at y = -350 (the convention of
    ft_people_city.roof_party). The set's "up" is the roof: the tall shops of a shop row (8.88 m), the arcade (8.88 m),
    a picture house (11.82 m), a bank (10.36 m), a service yard's shed (6.64 m). Everything stays behind y = -345:
    25.55 m from the centre line and more.
  - SM_P3B_Brokers stands ON a bank's steps, between its columns (origin on the columns' line, the steps' top is the
    piece's z = 0: the set's "up" is 0.6 m). 23 m long. Its front is 24.8 m out.
  - SM_P3B_Mayor stands on the plaza before a monument (the rally's place: 24.4 to 27.3 m out), on a dais of its own.
All twelve are TALL and lit steadily (strings of bulbs, a lit floor, a faint light on the figures: it is night):
24 m or more from the road, always. Nothing blinks. Banners and placards carry bars and discs: never words, never a
chevron (the Syndicate's mark), no gold double anything.

Run in the editor:  import ft_people_city_p3b as K; K.run()        (or K.run(['SM_P3B_RoofJazz']))
"""
import math
import random
import unreal
import ft_people as P
import ft_people_city as B
from ft_desert import rod, tilt_box

DIR = '/Game/People/CityCenter'
NAVY, DARK, GREY, STEEL, WHITE, CREAM = '#2b2350', '#22202a', '#3f3d50', '#8f9bb3', '#fff6ea', '#fff1d6'
PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, BLUE = '#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#ff2d3c', '#ff7a3c', '#8f6bff', '#3a5fa8'
WARM, WOOD, GREEN, BROWN, LILAC = '#ffe28a', '#9a6a44', '#4fbf6a', '#6a4428', '#f4a6ff'
HOT = (PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, WHITE, LILAC, '#b8ff5a', '#57c7ff')
G_BULB, G_FLOOR, G_NEON = 1.8, 0.4, 1.6
X0, X1, Y0, Y1 = -380.0, 380.0, -345.0, -80.0
CHEER, DANCE = tuple(dict(d, rate=min(d['rate'], 2.0)) for d in B.CHEER), B.DANCE   # lit limbs: nothing faster than 2 a second (review round)
GUARD = ((55, 80), (140, 85))            # a boxer's guard: both fists up before the chin


def fig(sc, r, x, y, z=0.0, yaw=0.0, who=None, detail=0, **act):
    """One figure: a citizen (or `who`, a look dict), doing `act`. Every figure gets a phase of its own."""
    d = dict(who) if who is not None else B.person(r)
    d.update(act)
    d.setdefault('phase', r.random())
    return P.figure(sc, x, y, z=z, yaw=yaw, detail=detail, **d)


def deck(sc, cols, string=True, rail=True, post_h=330.0, glow=G_FLOOR):
    """The stage every roof scene stands on: a lit floor of four panels, the roof's rail along its front, and (string)
    a string of bulbs between two posts over the rail."""
    g = sc.still()
    w = (X1 - X0) / 4.0
    for i in range(4):
        g.box(cols[i % len(cols)], (X0 + (i + 0.5) * w, (Y0 + Y1) / 2.0, 2.5), (w - 8.0, Y1 - Y0 - 8.0, 5.0), glow=glow)
    if rail:
        B.rail(g, X0 + 4.0, X1 - 4.0, Y0 + 5.0)
    if string:
        for sx in (-1, 1):
            g.box(STEEL, (sx * (X1 - 6.0), Y0 + 12.0, post_h / 2.0), (7, 7, post_h))
        B.bulbs(g, (X0 + 6.0, Y0 + 12.0, post_h - 4.0), (X1 - 6.0, Y0 + 12.0, post_h - 4.0), 7, sag=36)
    return g


def disc(m, col, x, y, z, rad, glow=0.0, t=3.0, steps=10):
    """A flat disc facing the road (-Y), its face at y."""
    m.cyl(col, (x, y + t, z), rad, t, rot=(0, 0, 90), steps=steps, glow=glow)


def bits(sc, r, n, box, cols, size=(10, 10, 4), parts=3, bob=-22.0, rate=0.45):
    """Confetti, ticker tape, petals: n small bits a part, afloat (each part rises and falls out of step)."""
    x0, x1, y0, y1, z0, z1 = box
    for k in range(parts):
        m = sc.part(P.motion(bob=bob, rate=rate * (1.0 + 0.17 * k), phase=k / float(parts) + r.uniform(0.0, 0.2)))
        for _ in range(n):
            m.box(r.choice(cols), (r.uniform(x0, x1), r.uniform(y0, y1), r.uniform(z0, z1)), size, rot=(r.uniform(-40, 40), r.uniform(0, 180), r.uniform(-40, 40)))


# ------------------------------------------------------------------------------------------ what hands hold here
def hold_ball(col=RED, rad=11.0):
    def f(m, h, side):
        m.sphere(col, (h[0], h[1], h[2]), rad, steps=(3, 6))
    return f


def hold_bouquet(cols=(PINK, WHITE, LILAC)):
    def f(m, h, side):
        m.box(GREEN, (h[0], h[1], h[2] + 2), (5, 5, 22))
        for k, c in enumerate(cols):
            m.sphere(c, (h[0] + (k - 1) * 9, h[1] - 3, h[2] + 20 + (k % 2) * 6), 10, steps=(3, 5))
    return f


def hold_slip(col=WHITE):
    def f(m, h, side):
        m.box(col, (h[0], h[1], h[2] + 16), (12, 3, 20))
    return f


def hold_paper():
    def f(m, h, side):
        m.box(CREAM, (h[0], h[1] - 2, h[2] + 26), (44, 3, 56))
        for k in range(3):
            m.box(GREY, (h[0], h[1] - 4, h[2] + 10 + k * 16), (34, 1.5, 6))
    return f


def hold_horn(col=YEL, L=46.0, bell=13.0):
    """A trumpet at the lips: a tube ahead of the hand and its bell."""
    def f(m, h, side):
        m.box(col, (h[0] - side * 4, h[1] - L / 2.0, h[2] + 8), (6, L, 6))
        m.box(col, (h[0] - side * 4, h[1] - L, h[2] + 8), (bell * 2, 6, bell * 2))
    return f


def hold_sax(col=YEL):
    def f(m, h, side):
        if side > 0:
            tilt_box(m, col, (h[0] - 22, h[1] - 6, h[2] + 26), (h[0] - 22, h[1] - 14, h[2] - 30), 9, 9)
            tilt_box(m, col, (h[0] - 22, h[1] - 14, h[2] - 30), (h[0] - 22, h[1] - 34, h[2] - 14), 13, 13)
    return f


def hold_tuba(col=YEL):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 8, h[2] + 6), (30, 22, 46))
            m.cyl(col, (h[0] - 24, h[1] - 4, h[2] + 28), 9, 62, steps=7, top_r=30)
    return f


def hold_drum(col=RED, skin=WHITE):
    def f(m, h, side):
        if side > 0:
            m.cyl(col, (h[0] - 24, h[1] + 8, h[2] - 4), 27, 26, rot=(0, 0, 90), steps=8)
            m.cyl(skin, (h[0] - 24, h[1] - 18, h[2] - 4), 24, 2, rot=(0, 0, 90), steps=8)
        m.box(WOOD, (h[0], h[1] - 9, h[2] + 4), (4.5, 22, 4.5))
    return f


def hold_sticks():
    def f(m, h, side):
        tilt_box(m, CREAM, (h[0], h[1] + 4, h[2]), (h[0] - side * 6, h[1] - 34, h[2] - 8), 4.5, 4.5)
    return f


def hold_pole_rag(col=PINK, L=220.0):
    """The pigeon fancier's long pole with a rag at its tip."""
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + L / 2.0 - 20), (4.5, 4.5, L))
        m.box(col, (h[0] + side * 26, h[1], h[2] + L - 40), (52, 3, 34))
    return f


def hold_can(col=CYAN):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 6, h[2] - 14), (22, 30, 24))
        tilt_box(m, col, (h[0], h[1] - 20, h[2] - 14), (h[0], h[1] - 48, h[2] + 2), 6, 6)
    return f


def hold_tube(col=GREY, L=64.0, glow=0.0):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 8, h[2] + 10), (L, 7, 7), glow=glow)
    return f


def hold_scissors():
    def f(m, h, side):
        for a in (-1, 1):
            tilt_box(m, STEEL, (h[0], h[1] - 4, h[2] + a * 5), (h[0], h[1] - 62, h[2] - a * 12), 5, 9)
            m.box(ORANGE, (h[0], h[1] + 6, h[2] + a * 10), (6, 16, 14))
    return f


def hold_tray(col=RED, bar=WHITE):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 10, h[2] + 4), (56, 34, 16))
            for k in range(3):
                m.box(bar, (h[0] - 44 + k * 20, h[1] - 10, h[2] + 16), (12, 28, 20))
    return f


def hold_board(col=WOOD):
    def f(m, h, side):
        if side > 0:
            m.box(col, (h[0] - 24, h[1] - 10, h[2] + 14), (40, 5, 40))
    return f


def hold_clip():
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 4, h[2] + 10), (22, 4, 30))
        m.box(WHITE, (h[0], h[1] - 6.5, h[2] + 9), (18, 1.5, 24))
    return f


# ------------------------------------------------------------------------------------------ 1  the wedding on the roof
def wedding(sc):
    """A wedding under an arch of flowers and a heart of lights: the couple at the rail waving at the chase, the
    registrar on a box behind them, guests throwing confetti, a child with flowers, the photographer, the cake."""
    r = random.Random(101)
    g = deck(sc, (CREAM, PINK), string=False)
    for sx in (-1, 1):
        g.box(WHITE, (sx * 100, -150, 135), (9, 9, 270))
        for k in range(3):
            g.sphere((WHITE, PINK)[k % 2], (sx * 100, -156, 60 + k * 75), 15, steps=(3, 5))
    g.box(WHITE, (0, -150, 274), (212, 9, 10))
    for k in range(9):
        a = k / 8.0
        g.sphere((PINK, WHITE, LILAC)[k % 3], (-100 + a * 200, -154, 262 + 34 * math.sin(a * math.pi)), 19, steps=(3, 5))
    for k in range(12):                                             # the heart of lights (steady)
        t = k / 12.0 * 2.0 * math.pi
        hx = 16.0 * math.sin(t) ** 3
        hz = 13.0 * math.cos(t) - 5.0 * math.cos(2 * t) - 2.0 * math.cos(3 * t) - math.cos(4 * t)
        g.box(PINK, (hx * 3.4, -152, 372 + hz * 3.4), (13, 13, 13), glow=G_BULB)
    g.box(WOOD, (0, -196, 17), (76, 54, 34))                        # the registrar's box
    g.box(WHITE, (-322, -132, 38), (104, 60, 76))                   # the cake's table
    for k, (rad, c) in enumerate(((30, WHITE), (22, PINK), (14, WHITE))):
        g.cyl(c, (-322, -132, 76 + k * 18), rad, 18, steps=8)
    g.sphere(RED, (-322, -132, 136), 7, steps=(3, 5))
    fig(sc, r, -34, -292, who=B.person(r, top=WHITE, wear='dress', hair='long', s=0.96), detail=1, arm_r='wave', swing_r=20, rate=1.0, bob=3,
        hold_l=hold_bouquet(), arm_l='hold', react=True)
    fig(sc, r, 34, -292, who=B.person(r, top=CYAN, bottom=NAVY, sleeves='long', wear='trousers', s=1.0), detail=1, arm_l='wave', swing_l=-20, rate=1.0,
        bob=3, arm_r='hip', react=True)
    fig(sc, r, 0, -196, z=34, who=B.person(r, top=VIOLET, wear='coat', sleeves='long', hair='bald'), arm_l='vee', arm_r='vee', swing_l=8, swing_r=-8, rate=0.5)
    for i, (x, y) in enumerate(((-150, -296), (-232, -240), (-300, -300), (150, -296), (236, -238))):
        fig(sc, r, x, y, yaw=(-30 if x < 0 else 30) + r.uniform(-8, 8), react=True, **CHEER[(0, 4, 1, 0, 5)[i]])
    fig(sc, r, -92, -316, who=B.person(r, build='kid'), arm_r='up', hold_r=hold_bouquet((YEL, PINK, WHITE)), swing_r=14, rate=1.4, bob=6)
    fig(sc, r, 322, -300, yaw=-72, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=10, rate=0.5)
    fig(sc, r, -244, -132, who=B.person(r, top=WHITE, bottom=DARK, hat='chef', hat_col=WHITE), arm_r='fwd', arm_l='hip', swing_r=8, axis_r='x', rate=0.6)
    bits(sc, r, 6, (-130, 130, -325, -235, 205, 300), HOT)
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 2  the open-air picture show
def cinema(sc):
    """An open-air picture show on the roof: the lit screen (two heads and a sun: no words), a row in deck chairs still
    watching it, and the rest up at the rail, turned to the better film in the street. A popcorn seller."""
    r = random.Random(102)
    g = deck(sc, (NAVY, VIOLET), string=False, glow=0.25)
    for sx in (-1, 1):
        g.box(DARK, (sx * 215, -90, 50), (10, 10, 100))
    g.box(DARK, (0, -90, 222), (486, 8, 256))
    g.box('#cfe8ff', (0, -95, 222), (462, 4, 232), glow=0.7)
    g.box(VIOLET, (0, -98, 146), (462, 2, 80), glow=0.45)
    disc(g, YEL, 138, -99, 268, 42, glow=0.9)
    for sx in (-1, 1):                                              # the two heads on the screen
        disc(g, DARK, sx * 52 - 30, -100, 212, 36)
        g.box(DARK, (sx * 62 - 30, -100, 150), (74, 2, 70))
    g.box(DARK, (-340, -300, 118), (44, 62, 34))                    # the projector
    g.box(STEEL, (-340, -300, 50), (7, 7, 100))
    rl = sc.part(P.motion(swing=50.0, pivot=(-340.0, -300.0, 152.0), axis=0.0, rate=0.5))
    for sy in (-1, 1):
        rl.cyl(STEEL, (-340 + 2, -300 + sy * 22, 152), 17, 4, rot=(0, 90, 0), steps=8)
    for i, x in enumerate((-230, -115, 0, 115, 230)):               # the deck chairs: striped canvas, backs to the road
        g = sc.still()
        c = (PINK, CYAN, YEL, MINT, ORANGE)[i]
        g.box(WOOD, (x, -214, 36), (52, 50, 6))
        tilt_box(g, c, (x, -238, 36), (x, -262, 100), 52, 5)
        if i != 3:
            fig(sc, r, x, -214, yaw=180, legs='sit', seat=40, **((dict(arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=3, rate=1.3),
                                                                    dict(arm_r='face', hold_r=P.hold_cup(), rate=0.4, bob=-1),
                                                                    dict(arm_l='point', swing_l=8, axis_l='x', rate=0.6),
                                                                    dict(), dict(arm_r='wave', swing_r=18, rate=1.0))[i]))
    for i, x in enumerate((-292, -176, 58, 176, 300)):
        fig(sc, r, x, -312, yaw=r.uniform(-18, 18), react=True, detail=1 if i == 2 else 0, **CHEER[(2, 0, 3, 6, 4)[i]])
    fig(sc, r, 345, -170, yaw=-20, who=B.person(r, top=RED, hat='cap', hat_col=WHITE, band=WHITE), arm_l='carry', arm_r='carry', hold_r=hold_tray(), rate=0.5, bob=-2)
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 3  the jazz band
def jazz(sc):
    """A band playing on the roof: drums on a riser, an upright bass, a piano, a saxophone, a trumpet, the singer at
    her microphone, a couple dancing, and three notes of light afloat over them."""
    r = random.Random(103)
    g = deck(sc, (VIOLET, NAVY))
    g.box(DARK, (0, -135, 15), (250, 104, 30))                      # the drummer's riser and kit
    g.cyl(RED, (0, -150, 62), 30, 34, rot=(0, 0, 90), steps=8)
    disc(g, WHITE, 0, -186, 62, 26)
    g.cyl(STEEL, (-52, -160, 30), 4, 58, steps=5)
    g.cyl(CYAN, (-52, -160, 88), 17, 12, steps=8)
    g.cyl(STEEL, (58, -160, 30), 4, 92, steps=5)
    cy = sc.part(P.motion(swing=9.0, pivot=(58.0, -160.0, 122.0), axis=0.0, rate=2.0))
    cy.cyl(YEL, (58, -160, 121), 25, 3, steps=8)
    g.box(WOOD, (-196, -176, 80), (48, 20, 100))                    # the bass
    g.box(BROWN, (-196, -176, 172), (9, 9, 90))
    g.box(DARK, (-350, -200, 62), (52, 150, 124))                   # the piano
    g.box(WHITE, (-314, -200, 76), (24, 136, 8))
    g.box(STEEL, (-60, -330, 76), (4.5, 4.5, 152))                  # the microphone
    g.sphere(STEEL, (-60, -330, 156), 8, steps=(3, 5))
    fig(sc, r, 0, -108, z=30, legs='sit', seat=44, who=B.person(r, top=WHITE, hat='cap', hat_col=RED), arm_l='fwd', arm_r='fwd', swing_l=22, swing_r=-22,
        axis_l='x', axis_r='x', hold_l=hold_sticks(), hold_r=hold_sticks(), rate=2.0)
    fig(sc, r, -236, -160, yaw=24, who=B.person(r, top=YEL, hat='cap', hat_col=DARK), arm_r='fwd', arm_l='hold', swing_r=16, axis_r='x', rate=2.0)
    fig(sc, r, -272, -200, yaw=-90, legs='sit', seat=46, who=B.person(r, top=MINT), arm_l='fwd', arm_r='fwd', swing_l=9, swing_r=-9, axis_l='x', axis_r='x', rate=2.0)
    fig(sc, r, 140, -290, who=B.person(r, top=ORANGE, hat='cap', hat_col=NAVY), detail=1, arm_l='hold', arm_r='hold', hold_r=hold_sax(), rock=6, rate=0.8)
    fig(sc, r, 238, -276, who=B.person(r, top=CYAN), detail=1, arm_r='face', arm_l='hip', hold_r=hold_horn(), bob=4, rate=1.6)
    fig(sc, r, -60, -296, who=B.person(r, top=RED, wear='dress', hair='long', s=0.98), detail=1, arm_l='out', arm_r='face', turn=14, rate=0.7)
    fig(sc, r, 318, -176, yaw=-30, **DANCE[1])
    fig(sc, r, 338, -262, yaw=20, **DANCE[3])
    for k, (x, c) in enumerate(((-150, CYAN), (70, PINK), (250, YEL))):
        m = sc.part(P.motion(bob=-18.0, rate=0.4 + 0.07 * k, phase=k / 3.0))
        z = 262 + k * 16
        m.sphere(c, (x, -210, z), 13, steps=(3, 6), glow=0.5)
        m.box(c, (x + 11, -210, z + 30), (5, 5, 56), glow=0.5)
        m.box(c, (x + 22, -210, z + 54), (26, 5, 9), glow=0.5)
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 4  the boxing gym
def boxing(sc):
    """A boxing gym on the roof: a ring with its ropes, two fighters sparring, the referee, a third working the heavy
    bag (it swings), the trainer shouting from the floor, a kid shadow-boxing."""
    r = random.Random(104)
    g = deck(sc, (GREY, NAVY), string=True, glow=0.2)
    cx, cyy = -70.0, -212.0
    g.box(BLUE, (cx, cyy, 22), (436, 236, 44))
    g.box(CREAM, (cx, cyy, 47), (428, 228, 6), glow=0.3)
    for i, (sx, sy) in enumerate(((-1, -1), (1, -1), (1, 1), (-1, 1))):
        g.box((RED, WHITE, BLUE, WHITE)[i], (cx + sx * 210, cyy + sy * 110, 115), (10, 10, 136))
    for k, c in enumerate((RED, WHITE, CYAN)):
        z = 96 + k * 36
        for sy in (-1, 1):
            g.box(c, (cx, cyy + sy * 110, z), (420, 4.5, 4.5))
        for sx in (-1, 1):
            g.box(c, (cx + sx * 210, cyy, z), (4.5, 220, 4.5))
    spar = dict(sleeves='none', wear='shorts', hair='short', s=0.98)
    fig(sc, r, cx - 44, cyy, z=50, yaw=90, who=B.person(r, top=RED, bottom=WHITE, **spar), detail=1, arm_l=GUARD, arm_r=GUARD, swing_l=28, swing_r=-28,
        axis_l='x', axis_r='x', hold_l=hold_ball(RED), hold_r=hold_ball(RED), legs='stride', rate=2.0, bob=4)
    fig(sc, r, cx + 44, cyy, z=50, yaw=-90, who=B.person(r, top=CYAN, bottom=NAVY, **spar), detail=1, arm_l=GUARD, arm_r=GUARD, swing_l=-26, swing_r=26,
        axis_l='x', axis_r='x', hold_l=hold_ball(CYAN), hold_r=hold_ball(CYAN), legs='stride', rate=2.0, bob=4)
    fig(sc, r, cx + 6, cyy + 78, z=50, who=B.person(r, top=WHITE, bottom=DARK, band=DARK, sleeves='long'), arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=1.0)
    g = sc.still()
    g.box(STEEL, (352, -150, 135), (9, 9, 270))                     # the heavy bag on its gallows
    tilt_box(g, STEEL, (352, -150, 262), (290, -150, 262), 8, 8)
    bag = sc.part(P.motion(swing=12.0, pivot=(290.0, -150.0, 258.0), axis=90.0, rate=0.8))
    bag.box(DARK, (290, -150, 244), (4.5, 4.5, 28))
    bag.cyl(RED, (290, -150, 132), 20, 98, steps=8)
    fig(sc, r, 236, -150, yaw=90, who=B.person(r, top=YEL, bottom=DARK, sleeves='none', wear='shorts'), arm_l=GUARD, arm_r=GUARD, swing_l=30, swing_r=-30,
        axis_l='x', axis_r='x', hold_l=hold_ball(BLUE), hold_r=hold_ball(BLUE), legs='stride', rate=2.0, bob=3)
    fig(sc, r, 214, -296, yaw=-58, who=B.person(r, top=ORANGE, hat='cap', hat_col=WHITE, build='stocky'), detail=1, arm_r='face', arm_l='point', swing_l=14,
        axis_l='x', rate=1.1, react=True)
    fig(sc, r, 318, -292, yaw=-20, who=B.person(r, build='kid', top=MINT), arm_l=GUARD, arm_r=GUARD, swing_l=30, swing_r=-30, axis_l='x', axis_r='x', rate=2.0, bob=6)
    fig(sc, r, 296, -224, yaw=-40, react=True, **CHEER[1])
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 5  the pigeon loft
def pigeon_loft(sc):
    """A pigeon fancier's loft: the coop on its legs, the fancier waving his long pole with a rag at its tip, his birds
    wheeling close over the roof, a man with his arms up to them, a child, an old hand on a box feeding two on the
    floor, and the neighbour's washing on its line."""
    r = random.Random(105)
    g = deck(sc, (GREY, '#5a4a3c'), string=False, glow=0.2)
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(WOOD, (-240 + sx * 100, -150 + sy * 55, 35), (8, 8, 70))
    g.box(MINT, (-240, -150, 140), (224, 130, 140))
    g.box(CREAM, (-240, -217, 150), (190, 4, 90))
    for k in range(5):
        g.box(GREY, (-320 + k * 40, -220, 150), (5, 3, 90))
    g.side(RED, [(-362, 210), (-118, 210), (-240, 268)], -225, -75)
    g.box(WOOD, (-240, -232, 100), (150, 26, 5))
    P.bird(sc, -290, -150, 262, heading=-80, span=58, body='#c9c4d6', wing='#8f9bb3', beak=ORANGE, flap=0.0, bob=2.0, rate=1.3, phase=0.2)
    cols = (('#f4f1ea', '#c9c4d6'), ('#c9c4d6', '#8f9bb3'), ('#fff6ea', '#a39cc2'))
    for i in range(5):
        a = i * 72.0 + r.uniform(-12, 12)
        R = r.uniform(92, 118)
        b, w = cols[i % 3]
        P.bird(sc, 20 + math.cos(math.radians(a)) * R, -212 + math.sin(math.radians(a)) * R, 340 + i * 30, heading=a + 90.0, span=84, body=b, wing=w,
               beak=ORANGE, phase=r.random(), rate=2.0, flap=36.0, orbit=(20, -212, 0.2))
    for x, y in ((92, -312), (64, -270)):
        P.bird(sc, x, y, 12, heading=r.uniform(0, 360), span=52, body='#c9c4d6', wing='#8f9bb3', beak=ORANGE, flap=0.0, bob=3.0, rate=2.0, phase=r.random())
    g = sc.still()
    for x in (130, 366):                                            # the washing line
        g.box(WOOD, (x, -118, 105), (7, 7, 210))
    g.box(GREY, (248, -118, 206), (236, 4.5, 4.5))
    for k, c in enumerate((WHITE, PINK, CYAN, YEL)):
        w = sc.part(P.motion(swing=10.0, pivot=(170.0 + k * 52, -118.0, 206.0), axis=0.0, rate=0.6, phase=k * 0.21))
        w.box(c, (170 + k * 52, -118, 174), (42, 3.5, 60))
    g.box(WOOD, (158, -292, 20), (44, 44, 40))
    fig(sc, r, -66, -276, who=B.person(r, top=ORANGE, hat='cap', hat_col=NAVY), detail=1, arm_r='up', hold_r=hold_pole_rag(), swing_r=20, rate=0.7, arm_l='hip')
    fig(sc, r, 16, -306, who=B.person(r, build='kid'), arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=8, rate=1.5)
    fig(sc, r, -150, -306, **CHEER[4])
    fig(sc, r, 158, -288, legs='sit', seat=40, who=B.person(r, top=BLUE, hat='cap', hat_col=GREY, hair='bald'), arm_r='fwd', swing_r=14, axis_r='x', rate=0.9)
    fig(sc, r, 262, -168, yaw=168, who=B.person(r, top=PINK, wear='dress', hair='bun'), arm_l='up', arm_r='up', swing_l=6, swing_r=-6, rate=0.5)
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 6  the neon crew
def worker(r, **fixed):
    d = B.person(r, top=ORANGE, bottom=NAVY, sleeves='long', wear='trousers', hat='hard', hat_col=YEL, band=WHITE)
    d.update(fixed)
    return d


def neon_crew(sc):
    """Sign hangers mending a neon sign: a star alight, a ring half dark. One on the scaffold reaching up with a tool,
    one carrying a tube, one footing the ladder, the foreman pointing, one hauling a bundle of tubes up on a rope
    (it rises and falls), one at lunch on a cable drum."""
    r = random.Random(106)
    g = deck(sc, (GREY, NAVY), string=False, glow=0.2)
    for sx in (-1, 1):
        g.box(DARK, (sx * 220, -88, 90), (10, 10, 180))
    g.box(DARK, (0, -88, 330), (520, 8, 300))
    pts = [(-110 + 96 * math.sin(k * 2 * math.pi / 5), 332 + 96 * math.cos(k * 2 * math.pi / 5)) for k in range(5)]
    for k in range(5):
        a, b = pts[k], pts[(k + 2) % 5]
        tilt_box(g, PINK, (a[0], -96, a[1]), (b[0], -96, b[1]), 9, 9, glow=G_NEON)
    for k in range(10):
        a0, a1 = k * math.pi / 5, (k + 1) * math.pi / 5
        on = k not in (0, 1, 2, 9)
        tilt_box(g, CYAN if on else GREY, (130 + 86 * math.cos(a0), -96, 332 + 86 * math.sin(a0)), (130 + 86 * math.cos(a1), -96, 332 + 86 * math.sin(a1)), 9, 9,
                 glow=G_NEON if on else 0.0)
    for sx in (-1, 1):                                              # the scaffold
        for sy in (-1, 1):
            g.box(STEEL, (sx * 208, -150 + sy * 46, 160), (7, 7, 320))
    g.box(WOOD, (0, -150, 200), (430, 92, 8))
    g.box(YEL, (0, -196, 300), (416, 5, 5))
    tilt_box(g, STEEL, (-208, -196, 10), (208, -196, 196), 5, 5)
    for sx in (-1, 1):                                              # the ladder
        tilt_box(g, YEL, (-300 + sx * 19, -292, 0), (-232 + sx * 19, -200, 214), 5, 5)
    for k in range(5):
        t = (k + 0.7) / 5.6
        g.box(YEL, (-300 + 68 * t, -292 + 92 * t, 214 * t), (36, 4.5, 4.5))
    g.box(STEEL, (346, -140, 215), (8, 8, 430))                     # the hoist
    tilt_box(g, STEEL, (346, -140, 426), (300, -190, 426), 7, 7)
    g.box(CREAM, (300, -190, 330), (4.5, 4.5, 190))
    load = sc.part(P.motion(bob=-48.0, rate=0.25, phase=0.1))
    for k in range(3):
        load.box((CYAN, PINK, YEL)[k], (300, -190 + (k - 1) * 9, 214), (84, 7, 7), glow=0.7)
    load.box(CREAM, (300, -190, 226), (10, 30, 5))
    g.cyl(WOOD, (160, -300, 0), 36, 44, steps=8)                    # the cable drum
    fig(sc, r, 132, -150, z=204, yaw=180, who=worker(r), detail=1, arm_r='up', swing_r=14, rate=1.2, hold_r=P.hold_tool(col=STEEL, grip=CYAN, length=30.0), arm_l='hip')
    fig(sc, r, -70, -150, z=204, yaw=160, who=worker(r), arm_l='carry', arm_r='carry', hold_r=hold_tube(), rock=3, rate=0.5)
    fig(sc, r, -332, -312, yaw=50, who=worker(r), arm_l='push', arm_r='push', rate=0.4, bob=-1)
    fig(sc, r, 20, -304, yaw=150, who=worker(r, hat_col=WHITE, top=YEL), detail=1, arm_r=((140, 60), (150, 60)), swing_r=8, axis_r='x', rate=0.6, arm_l='hold',
        hold_l=hold_clip())
    fig(sc, r, 300, -276, yaw=170, who=worker(r), arm_l=((120, 60), (150, 70)), arm_r=((120, 60), (150, 70)), swing_l=12, swing_r=12, axis_l='x', axis_r='x',
        rate=0.25, phase=0.1)
    fig(sc, r, 160, -296, legs='sit', seat=44, who=worker(r, hat=None), arm_r='face', hold_r=P.hold_cup(), rate=0.3, bob=-1)
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 7  the fashion show
def catwalk(sc):
    """A fashion show on the roof: three models in bold gowns walking a lit runway along the roof's edge and coming
    round again, the front row seated behind it, two photographers, the designer peeking round the backdrop."""
    r = random.Random(107)
    g = deck(sc, (NAVY, VIOLET), string=False, glow=0.2)
    g.box(DARK, (0, -292, 18), (744, 88, 36))
    g.box(WHITE, (0, -292, 38.5), (734, 78, 5), glow=0.5)
    for k in range(9):
        g.box(PINK, (-340 + k * 85, -338, 30), (14, 5, 12), glow=1.2)
    g.box(DARK, (0, -88, 146), (300, 10, 292))
    B.cloth(g, -140, 140, -94, 20, 282, base=NAVY, bars=(PINK, CYAN, YEL), kind=1, glow=0.35)
    g.box(GREY, (0, -196, 12.5), (704, 76, 25))                     # the front row's riser
    for i, x in enumerate((-240, -144, -48, 48, 144, 240)):
        g = sc.still()
        g.box(WHITE, (x, -190, 47), (46, 46, 44))
        fig(sc, r, x, -196, z=25, legs='sit', seat=44, **((CHEER[1], dict(arm_r='wave', swing_r=20, rate=1.0), dict(arm_r='face', rate=0.4, bob=-1), CHEER[1],
                                                         dict(arm_l='point', swing_l=8, axis_l='x', rate=0.6), dict(arm_l='up', arm_r='up', swing_l=10, swing_r=-10, rate=1.2))[i]))
    for sx in (-1, 1):
        fig(sc, r, sx * 334, -204, z=25, yaw=sx * 20, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=16, rate=0.5)
    fig(sc, r, 176, -112, who=B.person(r, top=DARK, hair='bun'), arm_l='hold', arm_r='hold', swing_l=12, swing_r=-12, axis_l='z', axis_r='z', rate=2.0)
    gowns = ((RED, 'long', 'sun', YEL), (CYAN, 'bun', None, PINK), (LILAC, 'afro', None, MINT))
    for i, (x, y, way, v) in enumerate(((-190, -306, 1, 85.0), (160, -306, 1, 85.0), (0, -276, -1, 72.0))):
        c, hair, hat, hc = gowns[i]
        fig(sc, r, x, y, z=41, yaw=90 * way, who=B.person(r, top=c, wear='dress', hair=hair, hat=hat, hat_col=hc, build='slim', s=1.0), detail=1,
            arm_l='hip', arm_r='down', swing_r=14, axis_r='x', stride=20, rate=v / 95.0, bob=2, travel=(700, v * way))
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 8  the roof court
def hoops(sc):
    """A basketball court on the roof under its wire fence: one going up for the shot (the ball with him), one
    jumping to block, one bouncing a second ball, a guard with arms wide, one calling for the pass; two on the bench
    beside a radio, a kid cheering."""
    r = random.Random(108)
    g = deck(sc, ('#ff9a5c', ORANGE), string=False, glow=0.3)
    g.box(WHITE, (0, -212, 5.5), (6, 250, 2))
    g.box(WHITE, (268, -212, 5.5), (150, 6, 2))
    g.box(WHITE, (194, -212, 5.5), (6, 150, 2))
    g.box(STEEL, (372, -215, 150), (10, 10, 300))                   # the hoop
    tilt_box(g, STEEL, (372, -215, 296), (336, -215, 310), 8, 8)
    g.box(WHITE, (333, -215, 322), (6, 112, 78))
    g.box(RED, (329, -215, 308), (2, 44, 34))
    g.cyl(RED, (306, -215, 288), 22, 4, steps=8)
    g.cyl(WHITE, (306, -215, 258), 13, 30, steps=6, top_r=20)
    for k in range(5):                                              # the wire fence behind
        g.box(STEEL, (-372 + k * 186, -84, 150), (6, 6, 300))
    for z in (100, 200, 298):
        g.box(STEEL, (0, -84, z), (750, 4.5, 4.5))
    g.box(WOOD, (-292, -124, 42), (156, 38, 8))                     # the bench and the radio
    for sx in (-1, 1):
        g.box(STEEL, (-292 + sx * 66, -124, 20), (6, 34, 40))
    g.box(DARK, (-232, -124, 62), (52, 20, 32))
    for sx in (-1, 1):
        disc(g, CYAN, -232 + sx * 14, -136, 62, 9)
    red = dict(top=RED, bottom=WHITE, sleeves='none', wear='shorts')
    blu = dict(top=CYAN, bottom=NAVY, sleeves='none', wear='shorts')
    p0 = 0.3
    fig(sc, r, 214, -232, z=5, yaw=90, who=B.person(r, s=1.0, **red), detail=1, arm_l='up', arm_r='up', bob=16, rate=1.0, phase=p0)
    ball = sc.part(P.motion(bob=16.0, rate=1.0, phase=p0))
    ball.sphere(ORANGE, (236, -232, 226), 13, steps=(4, 7))
    fig(sc, r, 268, -204, z=5, yaw=-90, who=B.person(r, s=1.0, **blu), detail=1, arm_l='up', arm_r='up', bob=14, rate=1.0, phase=p0 + 0.12)
    fig(sc, r, -40, -286, z=5, yaw=90, who=B.person(r, **blu), arm_l=((40, 70), (80, 85)), swing_l=22, axis_l='x', arm_r='out', legs='stride', rate=2.0)
    b2 = sc.part(P.motion(bob=62.0, rate=1.2, phase=0.0))
    b2.sphere(ORANGE, (-6, -322, 19), 13, steps=(4, 7))
    fig(sc, r, 62, -250, z=5, yaw=-90, who=B.person(r, **red), legs='wide', arm_l='out', arm_r='out', rock=6, rate=1.2)
    fig(sc, r, 130, -150, z=5, yaw=-30, who=B.person(r, **blu), arm_r='wave', swing_r=24, rate=1.4, bob=4)
    fig(sc, r, -322, -122, legs='sit', seat=46, react=True, **CHEER[1])
    fig(sc, r, -262, -122, legs='sit', seat=46, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=1.3)
    fig(sc, r, -346, -270, who=B.person(r, build='kid'), react=True, **CHEER[0])
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 9  the roof farm
def farm(sc):
    """A farm on the roof: beds of greens and tomatoes, sunflowers nodding, bee hives with their keeper in white and
    bees circling, one watering, one planting, one carrying a box of greens along the beds and back, a scarecrow
    rocking, two hens and a child after them."""
    r = random.Random(109)
    g = deck(sc, ('#5a4a3c', '#6f7a4a'), string=True, glow=0.2)
    for bx in (-250, 0, 250):
        g.box(WOOD, (bx, -300, 18), (200, 70, 36))
        g.box(BROWN, (bx, -300, 37), (190, 60, 4))
        for k in range(4):
            g.sphere(GREEN, (bx - 72 + k * 48, -300, 50), 17, steps=(3, 5))
            if k % 2 == 0:
                g.box(RED, (bx - 72 + k * 48, -318, 58), (8, 8, 8))
    for k in range(5):
        x = -340 + k * 62
        h = 160 + (k % 3) * 18
        f = sc.part(P.motion(swing=5.0, pivot=(float(x), -110.0, 0.0), axis=90.0, rate=0.35, phase=r.random()))
        f.box(GREEN, (x, -110, h / 2.0), (5, 5, h))
        disc(f, YEL, x, -116, h + 8, 27, steps=8)
        disc(f, BROWN, x, -119, h + 8, 11, steps=6)
    for i, hx in enumerate((252, 322)):                             # the hives
        g.box(WOOD, (hx, -120, 12), (52, 52, 24))
        for k in range(3):
            g.box((WHITE, YEL, CYAN, PINK)[(k + i) % 4], (hx, -120, 37 + k * 26), (46, 46, 25))
        g.box(GREY, (hx, -120, 104), (54, 54, 6))
    for k in range(2):
        bees = sc.part(P.motion(base=(287.0, -120.0), travel=(-1.0, (0.5, -0.38)[k])))
        for j in range(3):
            a = j * 2.1 + k
            bees.box(YEL, (287 + math.cos(a) * (38 + k * 14), -120 + math.sin(a) * (38 + k * 14), 132 + j * 16 + k * 10), (7, 7, 7), glow=0.3)
    g.box(WOOD, (132, -112, 95), (6, 6, 190))                       # the scarecrow
    sw = sc.part(P.motion(swing=6.0, pivot=(132.0, -112.0, 0.0), axis=90.0, rate=0.5))
    sw.box(WOOD, (132, -112, 150), (150, 6, 6))
    sw.box(RED, (132, -114, 128), (62, 14, 62))
    sw.sphere('#e8c26a', (132, -114, 186), 19, steps=(3, 6))
    sw.cyl(WOOD, (132, -114, 198), 40, 4, steps=8)
    sw.cyl(WOOD, (132, -114, 200), 18, 20, steps=8, top_r=12)
    for x in (112, 152):
        P.bird(sc, x, -250, 14, heading=r.uniform(0, 360), span=56, body=(WHITE, '#b87a50')[x > 130], wing=(CREAM, BROWN)[x > 130], beak=ORANGE, flap=0.0,
               bob=3.0, rate=1.8, phase=r.random())
    fig(sc, r, -250, -232, who=B.person(r, top=PINK, hat='sun', hat_col=YEL), detail=1, arm_r='fwd', hold_r=hold_can(), swing_r=10, axis_r='x', rate=0.6, arm_l='hip')
    fig(sc, r, 0, -234, legs='sit', seat=22, who=B.person(r, top=MINT), arm_l='fwd', arm_r='fwd', swing_l=10, swing_r=-10, axis_l='x', axis_r='x', rate=0.8)
    fig(sc, r, 60, -176, yaw=90, who=B.person(r, top=YEL, band=GREEN), arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=MINT), stride=22, rate=0.8, bob=2,
        travel=(460, 62.0))
    fig(sc, r, 287, -192, yaw=176, who=B.person(r, top=WHITE, bottom=WHITE, sleeves='long', wear='trousers', hat='sun', hat_col=WHITE), detail=1, arm_r='fwd',
        swing_r=10, axis_r='x', hold_r=P.hold_tool(col=STEEL, grip=GREY, length=14.0), rate=0.8, arm_l='hip')
    fig(sc, r, 204, -246, yaw=-80, who=B.person(r, build='kid'), arm_l='fwd', arm_r='fwd', bob=6, rate=1.6)
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 10  the dojo
def dojo(sc):
    """A karate class on the roof, in two rows on red and blue mats: eight in white, striking and hopping all together
    (the back row half a beat behind), the teacher at the end of the rows, a child breaking a board another holds."""
    r = random.Random(110)
    g = deck(sc, ('#e0505a', BLUE), string=False, glow=0.3)
    g.box(DARK, (0, -150, 15), (744, 96, 30))
    g.box('#e0505a', (0, -150, 31.5), (736, 88, 3), glow=0.3)
    for sx in (-1, 1):
        g.box(WOOD, (sx * 205, -84, 160), (8, 8, 320))
    ban = sc.part(P.motion(swing=3.0, pivot=(0.0, -84.0, 312.0), axis=0.0, rate=0.4))
    B.cloth(ban, -200, 200, -84, 214, 312, base=WHITE, bars=(RED, DARK, RED), kind=2, glow=0.14)
    belts = (YEL, ORANGE, MINT, BLUE, BROWN, YEL, MINT, ORANGE)

    def gi(k, **fixed):
        return B.person(r, top=WHITE, bottom=WHITE, sleeves='long', wear='trousers', band=belts[k % len(belts)], hat=None, **fixed)

    strike = dict(legs='wide', arm_l='fwd', arm_r='hip', swing_l=36, swing_r=-20, axis_l='x', axis_r='x', rate=1.0, bob=5)
    for i, x in enumerate((-216, -72, 72, 216)):
        fig(sc, r, x, -286, who=gi(i), detail=1 if i == 1 else 0, phase=0.25, **strike)
    for i, x in enumerate((-286, -150, -14, 122)):
        fig(sc, r, x, -150, z=33, who=gi(i + 4), phase=0.75, **strike)
    fig(sc, r, -350, -224, yaw=90, who=B.person(r, top=WHITE, bottom=WHITE, sleeves='long', wear='trousers', band=DARK, hair='bald', s=1.0), arm_r='point',
        swing_r=12, axis_r='x', arm_l='hip', rate=1.0, phase=0.25)
    fig(sc, r, 322, -150, z=33, yaw=-90, who=gi(2), arm_l='carry', arm_r='carry', hold_r=hold_board(), rate=0.5, bob=-1)
    fig(sc, r, 250, -150, z=33, yaw=90, who=gi(1, build='kid'), arm_r='fwd', arm_l='hip', swing_r=34, axis_r='x', rate=1.5, bob=5)
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 11  the brokers on the bank's steps
def trader(r, **fixed):
    d = B.person(r, top=r.choice((RED, CYAN, YEL, MINT, ORANGE, VIOLET, PINK)), bottom=r.choice((NAVY, DARK, GREY)), sleeves='long', wear='trousers', hat=None)
    d.update(fixed)
    return d


def brokers(sc):
    """For a bank's steps (SM_BankFront: origin on its columns' line, z = 0 the steps' top; the columns stand at x =
    -1000, -600, -200, 200, 600, 1000 and nobody stands within 60 cm of one). The closing bell: the floor's traders
    out on the steps in their bright jackets, waving slips, on the telephone, throwing ticker tape (it floats);
    a newsboy with the late edition, the bank's doorman, a banner of bars between the two middle columns."""
    r = random.Random(111)
    g = sc.still()
    ban = sc.part(P.motion(swing=3.0, pivot=(0.0, -46.0, 412.0), axis=0.0, rate=0.4))
    B.cloth(ban, -150, 150, -46, 322, 412, base=CREAM, bars=(PINK, CYAN, YEL), kind=0, glow=0.14)
    for sx in (-1, 1):
        g.box(CREAM, (sx * 152, -45, 420), (5, 5, 16))
    slip, slip2 = hold_slip(WHITE), hold_slip(PINK)
    acts = (
        dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.6, hold_l=slip, hold_r=slip2),
        dict(arm_r='face', hold_r=P.hold_phone(col=DARK, screen=GREY), arm_l='up', swing_l=18, rate=1.4, hold_l=slip),
        dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.0),
        dict(arm_r='wave', swing_r=26, rate=1.3, bob=3, hold_r=slip),
        dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.4, hold_r=slip2),
        dict(arm_r='up', arm_l='down', swing_r=18, rate=1.8, bob=6, hold_r=slip),
        dict(arm_r='point', arm_l='face', hold_l=P.hold_phone(col=DARK, screen=GREY), swing_r=10, axis_r='x', rate=0.7),
    )
    places = ((-905, 10, 0), (-800, -26, 1), (-700, 14, 2), (-455, -18, 3), (-345, 8, 4), (345, -12, 5), (455, 10, 6), (700, 12, 4), (800, -26, 0), (905, 14, 3),
              (-1092, 0, 5), (1092, 0, 2))
    for i, (x, y, a) in enumerate(places):
        fig(sc, r, x, y, yaw=r.uniform(-14, 14), who=trader(r), detail=1 if i in (1, 8) else 0, react=True, **acts[a])
    fig(sc, r, 98, 18, who=B.person(r, top=GREY, bottom=DARK, sleeves='long', wear='coat', hat='peak', hat_col=GREY, build='stocky', s=1.0), arm_l='hip', arm_r='hip',
        rock=2, rate=0.3)
    fig(sc, r, -70, -30, who=B.person(r, build='kid', hat='cap', hat_col=BROWN, top=MINT), detail=1, arm_r='up', hold_r=hold_paper(), swing_r=18, rate=1.2, bob=4,
        react=True)
    fig(sc, r, 10, 22, who=trader(r, top=VIOLET, wear='coat', build='stocky'), arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.7)
    bits(sc, r, 5, (-1000, 1000, -44, 30, 214, 300), (WHITE, CREAM, PINK, CYAN, WHITE), size=(6, 3.5, 34), bob=-30.0, rate=0.36)
    B.lit(sc)


# ------------------------------------------------------------------------------------------ 12  the mayor cuts the ribbon
def mayor(sc):
    """For the plaza before a monument (6.6 m by 2.4 m: y -110 .. 130): the mayor on a dais hung with striped cloth,
    cutting a ribbon with a great pair of scissors; an aide with the cushion, a child with flowers, a brass band of
    three, two photographers, two citizens with flags, two flags on poles behind."""
    r = random.Random(112)
    g = sc.still()
    g.box(NAVY, (0, 45, 40), (560, 150, 80))
    B.cloth(g, -280, 280, -33, 6, 76, base=WHITE, bars=(RED, CYAN, RED), kind=1, glow=0.12)
    g.box(NAVY, (-306, 45, 20), (52, 100, 40))
    for sx in (-1, 1):
        g.box(WHITE, (sx * 270, -24, 136), (7, 7, 112))
        g.box(STEEL, (sx * 274, 114, 200), (7, 7, 400))
        fl = sc.part(P.motion(swing=12.0, pivot=(sx * 274.0, 114.0, 360.0), axis=P.UP, rate=0.9, phase=0.3 * sx))
        fl.box((PINK, CYAN)[sx > 0], (sx * 274 - 50, 114, 360), (92, 3.5, 60), glow=0.12)
        fl.box(WHITE, (sx * 274 - 50, 112, 360), (92, 2, 18), glow=0.12)
    rb = sc.part(P.motion(swing=2.5, pivot=(0.0, -24.0, 190.0), axis=0.0, rate=0.5))
    rb.box(RED, (0, -24, 184), (540, 3.5, 12))
    for sx in (-1, 1):
        rb.box(RED, (sx * 16 + 70, -26, 184), (26, 3.5, 22))
    top = fig(sc, r, 0, 18, z=80, who=B.person(r, top=VIOLET, bottom=DARK, sleeves='long', wear='coat', band=RED, build='stocky', hair='short', s=1.0), detail=1,
              arm_r='fwd', hold_r=hold_scissors(), swing_r=9, axis_r='x', arm_l='wave', swing_l=-16, rate=0.8)
    g = sc.still()
    g.cyl(WHITE, (0, 18, top - 6), 36, 4, steps=8)                   # his tall white hat
    g.cyl(WHITE, (0, 18, top - 4), 23, 30, steps=8)
    g.box(RED, (0, 18, top + 2), (47, 47, 6))
    fig(sc, r, -112, 34, z=80, who=B.person(r, top=CYAN, sleeves='long'), arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=PINK, w=40.0), rate=0.4, bob=-1)
    fig(sc, r, 104, 20, z=80, who=B.person(r, build='kid'), arm_r='up', hold_r=hold_bouquet(), swing_r=14, rate=1.3, bob=6)
    band = dict(top=RED, bottom=WHITE, sleeves='long', wear='trousers', hat='peak', hat_col=RED, band=WHITE)
    fig(sc, r, 196, 66, z=80, who=B.person(r, build='stocky', **band), arm_l='hold', arm_r='hold', hold_r=hold_tuba(), rock=4, rate=1.0)
    fig(sc, r, 252, 22, z=80, who=B.person(r, **band), arm_r='face', arm_l='hip', hold_r=hold_horn(), bob=4, rate=1.6)
    fig(sc, r, -216, 62, z=80, who=B.person(r, **band), arm_l='fwd', arm_r='fwd', swing_l=20, swing_r=-20, axis_l='x', axis_r='x', hold_l=hold_drum(), hold_r=hold_drum(),
        rate=2.0)
    for sx in (-1, 1):
        fig(sc, r, sx * 316, -82, yaw=sx * -140, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), turn=12, rate=0.5)
    fig(sc, r, -196, -82, arm_r='wave', swing_r=24, hold_r=P.hold_flag(col=PINK, pole=WHITE, w=70.0), rate=0.9, bob=4, react=True)
    fig(sc, r, 190, -82, who=B.person(r, build='kid'), arm_r='wave', swing_r=24, hold_r=P.hold_flag(col=CYAN, pole=WHITE, w=60.0), rate=1.1, bob=6, react=True)
    B.lit(sc)


JOBS = [
    ('SM_P3B_RoofWedding', wedding),
    ('SM_P3B_RoofCinema', cinema),
    ('SM_P3B_RoofJazz', jazz),
    ('SM_P3B_RoofBoxing', boxing),
    ('SM_P3B_PigeonLoft', pigeon_loft),
    ('SM_P3B_NeonCrew', neon_crew),
    ('SM_P3B_RoofCatwalk', catwalk),
    ('SM_P3B_RoofHoops', hoops),
    ('SM_P3B_RoofFarm', farm),
    ('SM_P3B_RoofDojo', dojo),
    ('SM_P3B_Brokers', brokers),
    ('SM_P3B_Mayor', mayor),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
