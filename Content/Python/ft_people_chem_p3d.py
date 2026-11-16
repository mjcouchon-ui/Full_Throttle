"""FULL THROTTLE (Unreal) the population, Chemical Works, pass 3, builder p3d: HEIGHTS (/Game/People/ChemicalWorks/SM_P3D_*).

People UP HIGH, so that they clear the road's barrier, the kerb stores, the pipe racks (to 7.6 m) and the tank rows
(to 11 m) that stand at this level's sight line. Every piece brings its own structure and carries its people 8 to
20 m up, a third to a half over life size, in hi-vis against a lit pale sheet, and every one of them moves.

    SM_P3D_StackGallery    a chimney stack with a ring gallery 13 m up, the shift round the rail, a steeplejack in a
                           bosun's chair swinging under the crown
    SM_P3D_PipeBridge_A/_B a pipe bridge on two trestles (deck 11 m / 9 m): walkers crossing it, the rest at the rail
    SM_P3D_CraneBasket     a tower crane: the driver on the cab's step 20 m up, a man basket hung from the jib, swinging
    SM_P3D_ColumnDecks     a process column with three ring decks (8, 13, 18 m), a climber on the ladder between two
    SM_P3D_SphereCrown     a gas sphere on its legs, the inspection crew on the crown platform (15 m) with flags
    SM_P3D_Balconies       a control block: staff on three balconies (5, 9, 13 m) and the roof
    SM_P3D_BoomLifts       two boom lifts, their baskets 10 and 13.5 m up, swaying
    SM_P3D_SiloWalk        three silos and the catwalk across their tops (15 m), one walking it
    SM_P3D_WaterTower      a water tower: the shift sitting along the gallery's edge (11.5 m), legs over the side
    SM_P3D_Hoarding        a striped hoarding (no words): a painters' cradle sliding to and fro across it 8 m up, the
                           riggers on the catwalk along its top (14 m)

Every piece is TALL: 24 m or more from the centre line (the table block C:/ftwork/p3/L1d.inl keeps them 44 m and
more out). Light: steady only (lit sheeting, lamps, a little light of their own on the figures); nothing blinks.
Run in the editor:  import ft_people_chem_p3d; ft_people_chem_p3d.run()
A piece is drawn like every prop: cm; it stands on z = 0; its FRONT faces -Y (the road); long pieces run along X.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/ChemicalWorks'

STEELB, STEELB_D, TEAL, HAZ, NAVY, WHITE = '#2a3f8f', '#1c2a66', '#19b5c9', '#ffd23f', '#1c1f4a', '#eef6ff'
PAVE, PAVE_D, DARK = '#7f8fc6', '#56659f', '#22202a'
POLE = '#aab6e0'
BOARD = '#d9b98a'
ORANGE, LIME, YEL, RED, PINK, CYAN, VIOLET = '#ff7a3c', '#b8ff5a', '#ffc93c', '#ff2d3c', '#ff5fa2', '#2ee6ff', '#b44dff'
LAMP = '#fff1c0'
HIVIS = (ORANGE, ORANGE, LIME, YEL, '#fff6ea', PINK)
BRIGHT = (PINK, CYAN, YEL, LIME, '#fff6ea', ORANGE, RED)
HATS = (YEL, '#fff6ea', ORANGE, CYAN, RED)
GLOW = 0.24          # the steady light of its own that a figure has under the work lamps (these pieces stand 44 m and more out)

CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=9, rate=1.5),
    dict(arm_r='wave', swing_r=30, rate=1.3, bob=5),
    dict(arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=9, rate=1.3),
    dict(arm_l='wave', arm_r='wave', swing_l=-26, swing_r=26, rate=1.1, bob=6),
    dict(arm_r='point', arm_l='hip', swing_r=14, axis_r='x', rate=0.8, bob=3),
)


# ------------------------------------------------------------------------------------------ the cast
def worker(r, **fixed):
    top = r.choice(HIVIS)
    d = P.look(r, tops=HIVIS, bottoms=('#2b2350', '#3a5fa8', DARK), sleeves='long', wear='trousers', hat='hard',
               hat_col=r.choice(HATS), band=('#fff6ea' if top != '#fff6ea' else ORANGE))
    d['top'] = top
    d.update(fixed)
    return d


def staff(r, **fixed):
    """The office and the laboratory: bright shirts, white coats, one in three a guard in red."""
    k = r.random()
    if k < 0.3:
        d = P.look(r, sleeves='long', wear='trousers', hat='peak', hat_col=RED, band='#fff6ea')
        d.update(top=RED, bottom=DARK, shoes=DARK)
    elif k < 0.6:
        d = P.look(r, sleeves='long', wear='coat')
        d.update(top='#fff6ea')
    else:
        d = P.look(r, tops=BRIGHT)
    d.update(fixed)
    return d


def fan(sc, r, x, y, z, s, react=True, arms=None, tops=HIVIS, hats=HATS):
    """One of a mass: a broad block of hi-vis, short dark legs, a head and a hard hat, hopping on its own beat."""
    mo = dict(phase=r.random(), rate=r.uniform(1.1, 1.7), bob=r.uniform(8, 15) * s, react=react)
    m = sc.part(P.motion(**mo), (x, y, z), 0.0, s)
    top = r.choice(tops)
    m.box('#3a5fa8', (0, 0, 27), (42, 24, 54))
    m.box(top, (0, 0, 91), (64, 30, 74))
    m.box(r.choice(P.SKINS), (0, 0, 145), (38, 34, 34))
    m.box(r.choice(hats), (0, 0, 168), (46, 42, 14))
    if arms == 'up':
        for sx in (-1, 1):
            tilt_box(m, top, (sx * 36, 0, 116), (sx * 52, 0, 190), 16, 16)
    elif arms == 'wave':
        for sx in (-1, 1):
            a = sc.part(P.motion(swing=sx * 26.0, pivot=(x + sx * 36 * s, y, z + 118 * s), axis=90.0, **mo), (x, y, z), 0.0, s)
            a.box(top, (sx * 40, 0, 156), (16, 16, 78))


def person(sc, r, x, y, z, s, i=0, yaw=None, detail=0, cast=worker, flag=None, **over):
    """One whole figure of the shift, cheering in its own way; flag: a colour for a flag in the right hand."""
    d = cast(r)
    d.update(CHEER[i % len(CHEER)])
    d['s'] = s * r.uniform(0.95, 1.03)
    if flag:
        d.update(arm_r='wave', swing_r=26, axis_r='y', hold_r=P.hold_flag(col=flag, w=95.0), rate=0.9, bob=4)
    d.update(over)
    P.figure(sc, x, y, z=z, yaw=(r.uniform(-15, 15) if yaw is None else yaw), phase=r.random(), react=True, detail=detail, **d)


def row(sc, r, xs, y, z, s, seed=0, cast=worker, tops=HIVIS, flags=()):
    """People along a rail: every other one a whole figure, the rest the hopping blocks of a mass."""
    for i, x in enumerate(xs):
        if i % 2 == 0:
            person(sc, r, x, y, z, s, i=i // 2 + seed, detail=(1 if i % 4 == 0 else 0), cast=cast,
                   flag=(flags[(i // 2) % len(flags)] if (flags and i in (0, len(xs) - 1 - (len(xs) - 1) % 2)) else None))
        else:
            fan(sc, r, x, y + 12, z, s * r.uniform(0.92, 1.04), arms=('up', 'wave', None)[(i + seed) % 3], tops=tops)


def spread(x0, x1, n, r=None, j=14.0):
    return [x0 + (x1 - x0) * (i + 0.5) / n + (r.uniform(-j, j) if r else 0.0) for i in range(n)]


def lit(fn, glow=GLOW):
    """Everything in the piece that moves is given a little steady light of its own (the builder's people have none)."""
    def build(sc):
        fn(sc)
        for key in sc.order:
            model, mo = sc.parts[key]
            if not mo['rate']:
                continue
            out = {}
            for (col, gl), mesh in model.meshes.items():
                k2 = (col, max(gl, glow))
                if k2 in out:
                    unreal.GeometryScript_MeshEdits.append_mesh(out[k2], mesh, unreal.Transform())
                else:
                    out[k2] = mesh
            model.meshes = out
    return build


# ------------------------------------------------------------------------------------------ the steel
def lamp(g, x, y, z, glow=0.8):
    g.box(POLE, (x, y, z + 60), (9, 9, 120))
    g.box(DARK, (x, y - 14, z + 124), (70, 44, 28))
    g.box(LAMP, (x, y - 32, z + 110), (60, 28, 5), glow=glow)


def sheet(g, x, y, z, L, H, trim=ORANGE):
    """The lit pale sheeting behind a row: the ground its colours read against."""
    g.box(WHITE, (x, y, z + H / 2.0), (L, 5, H), glow=0.2)
    g.box(trim, (x, y - 4, z + H - 12), (L, 3, 24), glow=0.3)


def deck(g, x, y, z, L, D, posts=4):
    """A deck with its top at z: boards, a hazard kick plate and a rail along the front (-Y) edge."""
    g.box(BOARD, (x, y, z - 7), (L, D, 14))
    g.box(HAZ, (x, y - D / 2.0, z + 10), (L, 5, 26))
    g.box(HAZ, (x, y - D / 2.0, z + 105), (L, 8, 8))
    for k in range(posts):
        g.box(HAZ, (x - L / 2.0 + 4 + (L - 8) * k / max(posts - 1, 1), y - D / 2.0, z + 55), (8, 8, 110))


def trestle(g, x, y, W, D, H, col=POLE):
    """A four-legged steel trestle H tall, W along X and D deep, braced on its front face."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(col, (x + sx * W / 2.0, y + sy * D / 2.0, H / 2.0), (14, 14, H))
    n = max(int(H / 420.0), 1)
    for k in range(n):
        z0, z1 = H * k / n, H * (k + 1) / n
        a, b = (-1, 1) if k % 2 else (1, -1)
        tilt_box(g, col, (x + a * W / 2.0, y - D / 2.0, z0 + 10), (x + b * W / 2.0, y - D / 2.0, z1 - 10), 10, 10)
        g.box(col, (x, y - D / 2.0, z1 - 6), (W, 10, 10))
    g.box(NAVY, (x, y, 5), (W + 60, D + 60, 10))


def banner(sc, x, y, z, L, H, cols, phase=0.0):
    """A striped banner hung by its top edge, stirring (stripes and blocks: never words)."""
    m = sc.part(P.motion(phase=phase, rate=0.35, swing=5.0, pivot=(x, y, z + H), axis=0.0))
    n = len(cols)
    for k, c in enumerate(cols):
        m.box(c, (x - L / 2.0 + (k + 0.5) * L / n, y, z + H / 2.0), (L / n, 5, H))
    m.box(DARK, (x, y, z + H + 4), (L + 10, 8, 8))


# ------------------------------------------------------------------------------------------ 1. the stack and its gallery
def stack_gallery(sc):
    r = random.Random(41)
    g = sc.still()
    H, R0, R1, GZ = 2600.0, 230.0, 150.0, 1300.0
    g.cyl(PAVE, (0, 0, 0), R0, H, steps=12, top_r=R1)
    g.cyl(NAVY, (0, 0, 0), R0 + 40, 90, steps=12)
    for z in (H - 420, H - 180):
        rr = R0 + (R1 - R0) * z / H
        g.cyl(RED, (0, 0, z), rr + 4, 110, steps=12)
    g.cyl(DARK, (0, 0, H), R1 + 14, 30, steps=12)
    # the gallery: a ring deck, a kick ring, a rail, and the lit band of the stack behind the shift
    RG = 430.0
    g.cyl(BOARD, (0, 0, GZ - 14), RG, 14, steps=12)
    g.cyl(HAZ, (0, 0, GZ), RG + 3, 24, steps=12)
    g.cyl(WHITE, (0, 0, GZ + 30), R0 + (R1 - R0) * GZ / H + 5, 330, steps=12, glow=0.22)
    for k in range(12):
        a = math.radians(k * 30.0 + 15.0)
        g.box(HAZ, (math.cos(a) * (RG - 6), math.sin(a) * (RG - 6), GZ + 55), (8, 8, 110))
        a2 = math.radians(k * 30.0 + 45.0)
        tilt_box(g, HAZ, (math.cos(a) * (RG - 6), math.sin(a) * (RG - 6), GZ + 108), (math.cos(a2) * (RG - 6), math.sin(a2) * (RG - 6), GZ + 108), 8, 8)
    for k in range(6):
        a = math.radians(k * 60.0)
        tilt_box(g, PAVE_D, (math.cos(a) * (RG - 30), math.sin(a) * (RG - 30), GZ - 14), (math.cos(a) * 200, math.sin(a) * 200, GZ - 300), 12, 12)
    lamp(g, 0, -R0 + 60, GZ + 360)
    n = 7
    for i in range(n):
        a = 200.0 + 140.0 * (i + 0.5) / n
        x, y = math.cos(math.radians(a)) * 335.0, math.sin(math.radians(a)) * 335.0
        if i % 2 == 0:
            person(sc, r, x, y, GZ, 1.45, i=i, yaw=a - 270.0, detail=(1 if i in (2, 4) else 0), flag=(PINK if i == 0 else (CYAN if i == 6 else None)))
        else:
            fan(sc, r, x, y, GZ, 1.45, arms=('up', 'wave')[i // 2 % 2])
    # the steeplejack: a bosun's chair hung from a bracket under the crown, swinging gently across the stack's face
    piv = (0.0, -215.0, H - 60.0)
    g.box(DARK, (0, -175, H - 50), (30, 120, 20))
    ph = r.random()
    ch = sc.part(P.motion(phase=ph, rate=0.25, swing=5.0, pivot=piv, axis=90.0))
    CZ, CY = 1960.0, -262.0
    ch.box(BOARD, (0, CY, CZ - 5), (170, 46, 10))
    for sx in (-1, 1):
        ch.box(HAZ, (sx * 78, CY, CZ + 165), (6, 6, 330))
    ch.box(HAZ, (0, CY, CZ + 333), (170, 8, 8))
    tilt_box(ch, HAZ, (0, CY, CZ + 333), piv, 6, 6)
    ch.box(ORANGE, (72, CY + 30, CZ - 40), (26, 26, 34))                          # the paint kettle
    d = worker(r, top=ORANGE, band='#fff6ea', hat_col=YEL)
    d['s'] = 1.4
    P.figure(sc, 0, CY + 8, z=CZ - 46 * 1.4, yaw=0, legs='sit', arm_l='wave', arm_r='up', phase=ph, rate=0.25, detail=1,
             sway=dict(swing=5.0, pivot=piv, axis=90.0), **d)


# ------------------------------------------------------------------------------------------ 2. the pipe bridge
def pipe_bridge(seed=3, L=2400.0, H=1100.0, pipes=(TEAL, PAVE, ORANGE), cols=(PINK, WHITE, CYAN, WHITE, PINK, WHITE), walkers=3):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        D = 300.0
        for sx in (-1, 1):
            trestle(g, sx * (L / 2.0 - 120), 0, 220, D + 40, H - 60)
        # the span: two chords under the deck, the pipes it carries, the deck, its rail, the lit sheet behind
        for sy in (-1, 1):
            g.box(STEELB, (0, sy * D / 2.0, H - 40), (L, 16, 50))
        for k, c in enumerate(pipes):
            rod(g, c, (-L / 2.0 - 60, -70 + k * 70, H - 110), (L / 2.0 + 60, -70 + k * 70, H - 110), 26, steps=6)
        deck(g, 0, 0, H, L, D, posts=7)
        sheet(g, 0, D / 2.0, H, L - 40, 330)
        for sx in (-1, 1):
            lamp(g, sx * (L / 2.0 - 200), D / 2.0, H + 330)
        banner(sc, 0, -D / 2.0 - 8, H - 330, L * 0.5, 210, cols, phase=r.random())
        # at the rail
        row(sc, r, spread(-L / 2.0 + 150, L / 2.0 - 150, 7, r), -D / 2.0 + 62, H, 1.45, seed=seed, flags=(PINK, CYAN))
        # crossing: they walk the deck's back half and come round again
        for i in range(walkers):
            way = 1 if i % 2 == 0 else -1
            d = worker(r)
            d['s'] = 1.4
            if i == 1:
                d.update(arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=BOARD))
                P.figure(sc, -L / 2.0 + (i + 0.5) * L / walkers, 70, z=H, yaw=90 * way, stride=22, rate=1.0, bob=3, phase=r.random(), detail=0,
                         travel=(L - 260, way * 110.0), **d)
            else:
                P.figure(sc, -L / 2.0 + (i + 0.5) * L / walkers, 60 + 10 * i, z=H, yaw=90 * way, stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x',
                         rate=1.1, bob=3, phase=r.random(), detail=0, travel=(L - 260, way * 130.0), **d)
    return build


# ------------------------------------------------------------------------------------------ 3. the tower crane
def crane_basket(sc):
    r = random.Random(57)
    g = sc.still()
    MH = 2000.0
    g.box(NAVY, (0, 0, 20), (420, 420, 40))
    g.box(HAZ, (0, 0, MH / 2.0 + 20), (110, 110, MH))
    for k in range(1, 8):
        g.box(DARK, (0, 0, k * 250.0), (118, 118, 22))
    # a landing half way up the mast, two of the shift on it
    deck(g, 0, -150, 1000.0, 420, 200, posts=3)
    sheet(g, 0, -52, 1000.0, 400, 300)
    fan(sc, r, -100, -150, 1000.0, 1.45, arms='up')
    fan(sc, r, 100, -150, 1000.0, 1.45, arms='wave')
    # the slewing unit, the cab and its step, the jib and the counter jib
    g = sc.still()
    g.cyl(DARK, (0, 0, MH + 20), 100, 40, steps=8)
    g.box(ORANGE, (0, -60, MH + 170), (230, 240, 220))
    g.box(LAMP, (0, -182, MH + 200), (190, 4, 110), glow=0.45)
    deck(g, 210, -120, MH + 60, 190, 170, posts=2)
    g.box(HAZ, (500, 0, MH + 320), (1900, 56, 56))
    g.box(HAZ, (-550, 0, MH + 320), (500, 56, 56))
    g.box(PAVE_D, (-700, 0, MH + 250), (220, 150, 130))
    g.box(HAZ, (0, 0, MH + 450), (60, 60, 320))
    tilt_box(g, POLE, (0, 0, MH + 600), (1300, 0, MH + 350), 8, 8)
    tilt_box(g, POLE, (0, 0, MH + 600), (-700, 0, MH + 350), 8, 8)
    lamp(g, 1400, 0, MH + 345, glow=0.6)
    d = worker(r, top=LIME, hat_col='#fff6ea')
    d.update(CHEER[1])
    d['s'] = 1.45
    P.figure(sc, 215, -120, z=MH + 60, yaw=-10, phase=r.random(), react=True, detail=1, **d)
    # the man basket hung under the jib, swinging: its ropes, the basket and its riders move as one
    piv = (950.0, 0.0, MH + 290.0)
    ph, BZ = r.random(), 1250.0
    b = sc.part(P.motion(phase=ph, rate=0.2, swing=3.0, pivot=piv, axis=90.0))
    for sx in (-1, 1):
        tilt_box(b, DARK, (piv[0] + sx * 150, 0, BZ + 250), piv, 6, 6)
    b.box(BOARD, (piv[0], 0, BZ - 6), (360, 170, 12))
    b.box(HAZ, (piv[0], -85, BZ + 14), (360, 6, 34))
    b.box(HAZ, (piv[0], -85, BZ + 108), (360, 8, 8))
    b.box(HAZ, (piv[0], 85, BZ + 60), (360, 6, 120))
    for sx in (-1, 1):
        b.box(HAZ, (piv[0] + sx * 178, 0, BZ + 125), (8, 170, 250))
    b.box(WHITE, (piv[0], 88, BZ + 150), (330, 4, 240), glow=0.2)
    for i in range(3):
        d = worker(r)
        d['s'] = 1.45
        P.figure(sc, piv[0] - 115 + i * 115, -20, z=BZ, yaw=r.uniform(-12, 12), arm_l=('wave', 'up', 'down')[i], arm_r=('up', 'vee', 'wave')[i],
                 hold_r=(P.hold_flag(col=PINK, w=95.0) if i == 2 else None), phase=ph, rate=0.2, detail=(1 if i == 1 else 0),
                 sway=dict(swing=3.0, pivot=piv, axis=90.0), **d)


# ------------------------------------------------------------------------------------------ 4. the column and its ring decks
def column_decks(sc):
    r = random.Random(63)
    g = sc.still()
    R, H = 170.0, 2350.0
    g.cyl(NAVY, (0, 0, 0), R + 50, 80, steps=12)
    g.cyl(PAVE, (0, 0, 0), R, H, steps=12)
    g.cyl(TEAL, (0, 0, H), R, 90, steps=12, top_r=40)
    g.cyl(TEAL, (0, 0, 420), R + 4, 90, steps=12)
    rod(g, ORANGE, (R + 50, 60, 0), (R + 50, 60, H - 100), 24, steps=6)
    levels = (800.0, 1300.0, 1800.0)
    RD = 390.0
    for j, z in enumerate(levels):
        g.cyl(BOARD, (0, 0, z - 14), RD, 14, steps=12)
        g.cyl(HAZ, (0, 0, z), RD + 3, 24, steps=12)
        g.cyl(WHITE, (0, 0, z + 30), R + 5, 320, steps=12, glow=0.22)
        g.cyl(ORANGE, (0, 0, z + 350), R + 7, 22, steps=12, glow=0.3)
        for k in range(8):
            a = math.radians(202.5 + k * 19.3)
            g.box(HAZ, (math.cos(a) * (RD - 6), math.sin(a) * (RD - 6), z + 55), (8, 8, 110))
        for k in range(7):
            a, a2 = math.radians(202.5 + k * 19.3), math.radians(202.5 + (k + 1) * 19.3)
            tilt_box(g, HAZ, (math.cos(a) * (RD - 6), math.sin(a) * (RD - 6), z + 108), (math.cos(a2) * (RD - 6), math.sin(a2) * (RD - 6), z + 108), 8, 8)
        for k in range(4):
            a = math.radians(45.0 + k * 90.0)
            tilt_box(g, PAVE_D, (math.cos(a) * (RD - 30), math.sin(a) * (RD - 30), z - 14), (math.cos(a) * R, math.sin(a) * R, z - 240), 12, 12)
    # the ladder between the first two decks, on the stack's left front
    la = math.radians(225.0)
    lx, ly = math.cos(la) * (R + 26), math.sin(la) * (R + 26)
    for sx in (-1, 1):
        g.box(HAZ, (lx + sx * 22 * math.sin(la), ly - sx * 22 * math.cos(la), 1050), (7, 7, 520))
    for k in range(6):
        g.box(HAZ, (lx, ly, 830 + k * 85.0), (50, 50, 8), rot=(0, 45, 0))
    n_at = (3, 3, 2)
    for j, z in enumerate(levels):
        n = n_at[j]
        for i in range(n):
            a = 262.0 + (i - (n - 1) / 2.0) * 34.0 + (24.0 if j == 0 else 0.0)
            x, y = math.cos(math.radians(a)) * 300.0, math.sin(math.radians(a)) * 300.0
            if (i + j) % 2 == 0:
                person(sc, r, x, y, z, 1.4, i=i + j * 2, yaw=a - 270.0, detail=(1 if i == 1 else 0), flag=(CYAN if (j == 2 and i == 0) else None))
            else:
                fan(sc, r, x, y, z, 1.4, arms=('up', 'wave')[(i + j) % 2])
    d = worker(r, top=PINK, band='#fff6ea', hat_col=YEL)
    d['s'] = 1.2
    P.figure(sc, lx + math.cos(la) * 34, ly + math.sin(la) * 34, z=950.0, yaw=225.0 - 90.0, arm_l='up', arm_r='vee', swing_l=12, swing_r=-12,
             axis_l='x', axis_r='x', stride=10, rate=0.14, bob=-120, phase=r.random(), detail=1, **d)
    lamp(g, 0, -R + 40, levels[-1] + 350)


# ------------------------------------------------------------------------------------------ 5. the gas sphere
def sphere_crown(sc):
    r = random.Random(71)
    g = sc.still()
    R, CZ = 520.0, 880.0
    g.sphere(PAVE, (0, 0, CZ), R, steps=(6, 12))
    g.cyl(TEAL, (0, 0, CZ - 40), R + 4, 80, steps=12)
    for k in range(6):
        a = math.radians(k * 60.0 + 30.0)
        g.box(STEELB, (math.cos(a) * (R - 40), math.sin(a) * (R - 40), CZ / 2.0), (26, 26, CZ))
        a2 = math.radians(k * 60.0 + 90.0)
        tilt_box(g, POLE, (math.cos(a) * (R - 40), math.sin(a) * (R - 40), 40), (math.cos(a2) * (R - 40), math.sin(a2) * (R - 40), CZ - 200), 10, 10)
    g.cyl(NAVY, (0, 0, 0), R + 30, 14, steps=12)
    # the stair up the front in two flights, a landing between them
    tilt_box(g, HAZ, (-760, -R - 40, 20), (-300, -R - 40, 720), 24, 100)
    g.box(BOARD, (-230, -R - 70, 720), (180, 180, 12))
    tilt_box(g, HAZ, (-160, -R - 120, 730), (-160, -300, CZ + R), 100, 24)
    g.box(POLE, (-230, -R - 40, 360), (12, 12, 720))
    # the crown platform
    PZ = CZ + R - 20.0
    deck(g, 0, -60, PZ, 760, 300, posts=5)
    g.box(STEELB, (0, -60, PZ - 60), (500, 220, 100))
    sheet(g, 0, 92, PZ, 740, 330)
    for sx in (-1, 1):
        lamp(g, sx * 300, 92, PZ + 330)
    banner(sc, 0, -R - 10, CZ - 150, 520, 200, (LIME, WHITE, PINK, WHITE, LIME, WHITE), phase=r.random())
    row(sc, r, spread(-330, 330, 7, r, 8.0), -130, PZ, 1.5, seed=2, flags=(PINK, LIME))
    person(sc, r, -250, -R - 70, 726.0, 1.35, i=4, detail=0)


# ------------------------------------------------------------------------------------------ 6. the control block's balconies
def balconies(sc):
    r = random.Random(83)
    g = sc.still()
    L, D, H = 1500.0, 520.0, 1700.0
    yf = -D / 2.0 + 110
    g.box(PAVE_D, (0, 55, H / 2.0), (L, 410, H))
    g.box(STEELB_D, (0, 55, H - 20), (L + 30, 440, 40))
    g.box(STEELB, (L / 2.0 - 110, 30, H / 2.0 + 120), (220, 460, H + 240))                 # the stair tower
    g.box(LAMP, (L / 2.0 - 110, yf - 52, H + 120), (120, 4, 150), glow=0.4)
    levels = (500.0, 900.0, 1300.0)
    BL = L - 300.0
    bx = -150.0
    for j, z in enumerate(levels):
        deck(g, bx, yf - 70, z, BL, 140, posts=5)
        g.box(LAMP, (bx, yf - 2, z + 190), (BL - 60, 4, 230), glow=0.4)             # the lit window band behind the balcony
        g.box(PAVE_D, (bx, yf - 4, z + 320), (BL, 8, 30))
        for k in range(1, 5):
            g.box(PAVE_D, (bx - BL / 2.0 + k * BL / 5.0, yf - 4, z + 190), (14, 8, 230))
        for sx in (-1, 1):
            tilt_box(g, PAVE_D, (bx + sx * (BL / 2.0 - 30), yf - 136, z - 14), (bx + sx * (BL / 2.0 - 30), yf, z - 150), 12, 12)
        xs = spread(bx - BL / 2.0 + 100, bx + BL / 2.0 - 100, 5, r)
        for i, x in enumerate(xs):
            if (i + j) % 2 == 0:
                person(sc, r, x, yf - 66, z, 1.35, i=i + j, detail=(1 if (i == 2 and j == 0) else 0), cast=staff,
                       flag=((PINK, CYAN, LIME)[j] if i in (0, 4) and j != 1 else None))
            else:
                fan(sc, r, x, yf - 60, z, 1.35, arms=('up', 'wave', None)[(i + j) % 3], tops=BRIGHT, hats=(RED, '#fff6ea', '#2a2230', '#6a4428', YEL))
    banner(sc, bx, yf - 146, levels[0] - 250, BL - 200, 210, (LIME, WHITE, PINK, WHITE, LIME, WHITE, PINK, WHITE), phase=r.random())
    g = sc.still()
    g.box(TEAL, (-L / 2.0 + 150, yf - 2, 110), (150, 5, 220))
    # the roof
    g.box(STEELB, (-110, yf + 6, H + 22), (L - 220, 12, 44))
    sheet(g, -110, yf + 200, H, L - 500, 340)
    lamp(g, -110, yf + 200, H + 340)
    for i, x in enumerate(spread(-L / 2.0 + 240, L / 2.0 - 460, 4, r)):
        fan(sc, r, x, yf + 90, H, 1.45, arms=('up', 'wave')[i % 2], tops=BRIGHT)


# ------------------------------------------------------------------------------------------ 7. the boom lifts
def boom_lifts(sc):
    r = random.Random(97)
    g = sc.still()
    for k, (x0, BZ, way, body) in enumerate(((-420.0, 1000.0, 1, ORANGE), (430.0, 1350.0, -1, CYAN))):
        g = sc.still()
        g.box(DARK, (x0, 0, 45), (440, 190, 34))
        g.box(body, (x0, 0, 105), (400, 220, 90))
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.cyl(DARK, (x0 + sx * 150, sy * 110 - 15, 48), 48, 30, rot=(0, 0, 90), steps=8)
        g.cyl(DARK, (x0, 0, 150), 80, 40, steps=8)
        # everything above the turntable sways as one: the two booms, the basket, its sheet and its riders
        piv = (x0, 0.0, 170.0)
        ph = r.random()
        rate = 0.22 + 0.05 * k
        b = sc.part(P.motion(phase=ph, rate=rate, swing=2.5, pivot=piv, axis=90.0))
        knee = (x0 - way * 330.0, 40.0, BZ * 0.55)
        tip = (x0 + way * 260.0, 0.0, BZ - 40.0)
        tilt_box(b, HAZ, (x0, 40, 190), knee, 50, 50)
        tilt_box(b, POLE, knee, tip, 38, 38)
        b.box(DARK, knee, (70, 70, 70))
        bx = tip[0]
        W = 300.0
        b.box(BOARD, (bx, -20, BZ - 6), (W, 160, 12))
        b.box(HAZ, (bx, -100, BZ + 14), (W, 6, 34))
        b.box(HAZ, (bx, -100, BZ + 108), (W, 8, 8))
        for sx in (-1, 1):
            b.box(HAZ, (bx + sx * W / 2.0, -20, BZ + 108), (8, 160, 8))
            b.box(HAZ, (bx + sx * W / 2.0, -100, BZ + 55), (8, 8, 110))
            b.box(HAZ, (bx + sx * W / 2.0, 60, BZ + 170), (8, 8, 340))
        b.box(WHITE, (bx, 62, BZ + 185), (W, 5, 330), glow=0.22)
        b.box(body, (bx, 58, BZ + 338), (W, 3, 22), glow=0.3)
        for i in range(2):
            d = worker(r)
            d['s'] = 1.45
            P.figure(sc, bx - 70 + i * 140, -30, z=BZ, yaw=r.uniform(-12, 12), arm_l=('wave', 'up')[(i + k) % 2], arm_r=('up', 'wave')[(i + k) % 2],
                     hold_r=(P.hold_flag(col=(PINK, LIME)[k], w=95.0) if i == 1 else None), phase=ph, rate=rate, detail=1,
                     sway=dict(swing=2.5, pivot=piv, axis=90.0), **d)
    # the banksman on the lower lift's chassis deck would be under the barrier's line: one on a drum stack instead
    g = sc.still()
    g.cyl(TEAL, (0, 150, 0), 150, 420, steps=10)
    g.cyl(HAZ, (0, 150, 420), 154, 20, steps=10)
    person(sc, r, 0, 150, 440.0, 1.45, i=3, detail=0)


# ------------------------------------------------------------------------------------------ 8. the silos and their catwalk
def silo_walk(sc):
    r = random.Random(103)
    g = sc.still()
    R, H = 270.0, 1500.0
    for k, x in enumerate((-640.0, 0.0, 640.0)):
        g.cyl(PAVE if k != 1 else '#9aa8d8', (x, 110, 0), R, H, steps=12)
        g.cyl(PAVE_D, (x, 110, H), R, 110, steps=12, top_r=60)
        g.cyl((TEAL, ORANGE, TEAL)[k], (x, 110, H * 0.62), R + 4, 100, steps=12)
        g.cyl(NAVY, (x, 110, 0), R + 30, 60, steps=12)
    # the catwalk along the front of the tops, bracketed off the silos
    L = 1960.0
    deck(g, 0, -210, H, L, 300, posts=7)
    for x in (-900.0, -320.0, 320.0, 900.0):
        tilt_box(g, PAVE_D, (x, -340, H - 14), (x, -160, H - 300), 12, 12)
    sheet(g, 0, -56, H, L - 60, 330, trim=LIME)
    for sx in (-1, 1):
        lamp(g, sx * 700, -56, H + 330)
    # the inclined gallery that feeds the first silo
    tilt_box(g, STEELB, (-1500, 110, 60), (-800, 110, H + 60), 150, 130)
    trestle(g, -1180, 110, 160, 160, 700)
    banner(sc, 0, -368, H - 330, 900, 210, (ORANGE, WHITE, LIME, WHITE, ORANGE, WHITE, LIME, WHITE), phase=r.random())
    row(sc, r, spread(-L / 2.0 + 120, L / 2.0 - 120, 9, r), -300, H, 1.45, seed=1, flags=(LIME, PINK))
    d = worker(r)
    d['s'] = 1.4
    P.figure(sc, 0, -140, z=H, yaw=90, stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=1.1, bob=3, phase=r.random(), detail=0,
             travel=(L - 200, 120.0), **d)


# ------------------------------------------------------------------------------------------ 9. the water tower
def water_tower(sc):
    r = random.Random(109)
    g = sc.still()
    GZ, R = 1150.0, 380.0
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(g, STEELB, (sx * 420, sy * 420, 0), (sx * 250, sy * 250, GZ), 22, 22)
    for z, w in ((380.0, 363.0), (760.0, 307.0)):
        for sy in (-1, 1):
            g.box(POLE, (0, sy * w, z), (w * 2, 10, 10))
        for sx in (-1, 1):
            g.box(POLE, (sx * w, 0, z), (10, w * 2, 10))
    tilt_box(g, POLE, (-400, -400, 30), (300, -300, 760), 10, 10)
    tilt_box(g, POLE, (400, -400, 30), (-300, -300, 760), 10, 10)
    rod(g, TEAL, (0, 0, 0), (0, 0, GZ), 34, steps=6)
    g.cyl(BOARD, (0, 0, GZ - 14), R + 150, 14, steps=12)
    g.cyl('#9aa8d8', (0, 0, GZ), R, 520, steps=12)
    g.cyl(WHITE, (0, 0, GZ + 20), R + 4, 330, steps=12, glow=0.22)
    g.cyl(ORANGE, (0, 0, GZ + 350), R + 6, 24, steps=12, glow=0.3)
    g.cyl(STEELB, (0, 0, GZ + 520), R + 20, 170, steps=12, top_r=40)
    # the gallery's front: a straight board the shift sits along, legs over the side, a rail behind them at the tank
    FY = -R - 150.0
    g.box(BOARD, (0, FY + 70, GZ - 7), (1000, 150, 14))
    g.box(HAZ, (0, FY - 4, GZ - 12), (1000, 6, 24))
    for sx in (-1, 1):
        tilt_box(g, PAVE_D, (sx * 460, FY + 10, GZ - 14), (sx * 250, -250, GZ - 300), 12, 12)
        lamp(g, sx * 330, -R + 30, GZ + 400)
    s = 1.45
    for i, x in enumerate(spread(-410, 410, 6, r, 8.0)):
        d = worker(r)
        d['s'] = s
        pose = (dict(arm_l='up', arm_r='up', swing_l=18, swing_r=-18, rate=1.4), dict(arm_r='wave', swing_r=30, rate=1.2),
                dict(arm_l='wave', arm_r='wave', swing_l=-26, swing_r=26, rate=1.0))[i % 3]
        d.update(pose)
        if i == 5:
            d.update(arm_r='wave', swing_r=26, hold_r=P.hold_flag(col=CYAN, w=95.0), rate=0.9)
        P.figure(sc, x, FY + 22, z=GZ - 46 * s, yaw=r.uniform(-10, 10), legs='sit', phase=r.random(), react=True, detail=(1 if i % 2 else 0), **d)
    for i, x in enumerate((-300.0, 0.0, 300.0)):
        fan(sc, r, x, FY + 110, GZ, 1.45, arms=('up', 'wave', 'up')[i])
    banner(sc, 0, -300, 800, 520, 200, (CYAN, WHITE, PINK, WHITE, CYAN, WHITE), phase=r.random())


# ------------------------------------------------------------------------------------------ 10. the hoarding and its cradle
def hoarding(sc):
    r = random.Random(113)
    g = sc.still()
    L, Z0, Z1 = 1800.0, 480.0, 1400.0
    for x in (-700.0, 0.0, 700.0):
        g.box(STEELB, (x, 60, Z1 / 2.0), (26, 26, Z1))
        tilt_box(g, POLE, (x, 60, Z0), (x, 330, 20), 12, 12)
        g.box(NAVY, (x, 190, 6), (120, 420, 12))
    g.box(NAVY, (0, 30, (Z0 + Z1) / 2.0), (L, 30, Z1 - Z0))
    # the picture: bands and blocks of colour, half painted (never words, never a mark)
    cols = (PINK, WHITE, CYAN, YEL, VIOLET, WHITE, LIME, ORANGE)
    n = len(cols)
    for k, c in enumerate(cols):
        hgt = (Z1 - Z0 - 80) * (1.0 if k < 5 else 0.45)
        g.box(c, (-L / 2.0 + 40 + (k + 0.5) * (L - 80) / n, 12, Z1 - 40 - hgt / 2.0), ((L - 80) / n, 6, hgt), glow=0.2)
    # the catwalk along the top, the riggers on it
    deck(g, 0, 10, Z1, L, 170, posts=7)
    sheet(g, 0, 98, Z1, L - 300, 320, trim=PINK)
    for x in (-600.0, 0.0, 600.0):
        tilt_box(g, DARK, (x, -70, Z1 + 20), (x, -260, Z1 + 60), 10, 10)
        g.box(LAMP, (x, -265, Z1 + 50), (90, 30, 6), glow=0.7)                # the lamps that light the picture
    row(sc, r, spread(-L / 2.0 + 260, L / 2.0 - 260, 5, r), -15, Z1, 1.45, seed=3, flags=(CYAN, PINK))
    # the painters' cradle: it slides to and fro across the picture (a slow swing about a pivot 50 m under the ground)
    piv = (0.0, -70.0, -5000.0)
    ph, CZ, CY = r.random(), 800.0, -75.0
    c = sc.part(P.motion(phase=ph, rate=0.09, swing=3.5, pivot=piv, axis=90.0))
    c.box(BOARD, (0, CY, CZ - 6), (520, 100, 12))
    c.box(HAZ, (0, CY - 50, CZ + 105), (520, 8, 8))
    c.box(HAZ, (0, CY - 50, CZ + 12), (520, 6, 30))
    for sx in (-1, 1):
        c.box(HAZ, (sx * 256, CY, CZ + 55), (8, 100, 110))
        c.box(DARK, (sx * 240, CY + 10, (CZ + Z1) / 2.0 + 60), (6, 6, Z1 - CZ + 120))
    c.box(ORANGE, (120, CY, CZ + 22), (34, 34, 44))
    for i in range(3):
        d = worker(r, top=('#fff6ea', LIME, ORANGE)[i], band=(ORANGE, '#fff6ea', '#fff6ea')[i])
        d['s'] = 1.4
        if i == 1:      # one has turned round to watch the chase
            P.figure(sc, -10, CY - 5, z=CZ, yaw=0, arm_l='up', arm_r='wave', phase=ph, rate=0.09, detail=1, sway=dict(swing=3.5, pivot=piv, axis=90.0), **d)
        else:
            P.figure(sc, -170 + i * 170, CY + 5, z=CZ, yaw=180, arm_l='down', arm_r='up', hold_r=P.hold_tool(col=(PINK, CYAN, CYAN)[i], grip=POLE, length=60.0),
                     phase=ph, rate=0.09, detail=0, sway=dict(swing=3.5, pivot=piv, axis=90.0), **d)


JOBS = [
    ('SM_P3D_StackGallery', lit(stack_gallery)),
    ('SM_P3D_PipeBridge_A', lit(pipe_bridge(3))),
    ('SM_P3D_PipeBridge_B', lit(pipe_bridge(8, L=3000.0, H=900.0, pipes=(ORANGE, TEAL, PAVE), cols=(LIME, WHITE, ORANGE, WHITE, LIME, WHITE, ORANGE, WHITE), walkers=4))),
    ('SM_P3D_CraneBasket', lit(crane_basket)),
    ('SM_P3D_ColumnDecks', lit(column_decks), dict(grow=(60.0, 60.0, 140.0), grow_down=(60.0, 60.0, 0.0))),
    ('SM_P3D_SphereCrown', lit(sphere_crown)),
    ('SM_P3D_Balconies', lit(balconies)),
    ('SM_P3D_BoomLifts', lit(boom_lifts)),
    ('SM_P3D_SiloWalk', lit(silo_walk)),
    ('SM_P3D_WaterTower', lit(water_tower)),
    ('SM_P3D_Hoarding', lit(hoarding), dict(grow=(330.0, 60.0, 60.0), grow_down=(330.0, 60.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
