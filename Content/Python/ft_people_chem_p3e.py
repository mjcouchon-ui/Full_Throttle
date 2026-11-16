"""FULL THROTTLE (Unreal) the population, level 2, the Chemical Works, pass 3, builder p3e: FAR AND SKY
(/Game/People/ChemicalWorks/SM_P3E_...).

What lives 150 m out to the horizon and in the air over the works. A night level: navy land, navy sky. What stands
at the sight line (a pipe rack 7.6 m tall at 26.5 m, a tank row 11 m tall at 28.5 m) hides everything under 35 to
50 m at this distance, so every piece here carries its life 45 to 160 m up, every figure is five to six times life
size (a block of lit hi-vis 9 to 10 m tall), every piece has a steady light of its own (nothing blinks) and is saved
with far=True (the always-moving master).

    SM_P3E_Steeplejacks   a banded chimney 110 m tall: climbers on its ladder, two painters' cradles riding up and
                          down its face, a gang on the top gallery waving a flag
    SM_P3E_FarCrowd_A/_B  the shift's far terrace: three steps on stilts 45 to 58 m up against a lit white sheet,
                          thirty of the shift hopping out of step (B: magenta canopy, two great flags)
    SM_P3E_TowerRim       a cooling tower 95 m tall and the gang along its rim, steam drifting behind them
    SM_P3E_Viaduct        the haul road on its piers 48 m up: shift buses and crew lorries (the crew on the bed,
                          hopping) crossing both ways and coming round again
    SM_P3E_Cableway       the works' ropeway between two pylons 70 m tall: lit cabins one way, open man-riders with
                          waving crews the other
    SM_P3E_Balloons_A/_B  hot-air balloons drifting in slow circles 70 to 140 m up, their baskets full of waving
                          people (B: two balloons and a tethered observation balloon riding its cable)
    SM_P3E_Blimp          the film unit's pale airship crossing along the road 120 m up, the camera crew on its deck
    SM_P3E_Kites          a gasholder's roof party flying four great kites that swing on their lines
    SM_P3E_GullTower      floodlit gulls wheeling in a column over a steaming vent stack
    SM_P3E_Lanterns       an office block's roof party letting paper lanterns go: thirty of them drifting and floating

Placed by C:/ftwork/p3/L1e.inl (the integrator pastes it into FTPeopleL1.inl). Nothing nearer than 150 m.
Run in the editor:  import ft_people_chem_p3e; ft_people_chem_p3e.run()
A piece is drawn like every prop: cm; it stands on z = 0; its FRONT faces -Y (the road); long pieces run along X.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/ChemicalWorks'

STEELB, STEELB_D, TEAL, HAZ, NAVY, MAG, VIOLET, WHITE = '#2a3f8f', '#1c2a66', '#19b5c9', '#ffd23f', '#1c1f4a', '#e84aff', '#b44dff', '#eef6ff'
PAVE, PAVE_D, DARK = '#7f8fc6', '#56659f', '#22202a'
ORANGE, LIME, YEL, RED, PINK, CYAN = '#ff7a3c', '#b8ff5a', '#ffc93c', '#ff2d3c', '#ff5fa2', '#2ee6ff'
LAMP, PALE, WICKER = '#fff1c0', '#c9d4ff', '#c9a070'
HIVIS = (ORANGE, ORANGE, LIME, YEL, '#fff6ea', PINK, CYAN)
HATS = (YEL, '#fff6ea', ORANGE, CYAN, RED)
GULL = '#fff6ea'
FAR = dict(far=True)


# ------------------------------------------------------------------------------------------ this kit's own builders
def dot(sc, r, x, y, z, s=5.5, yaw=0.0, top=None, glow=0.32, arms='down', legs=True, bob=0.0, rate=1.2, phase=None, wave=26.0,
        ride=None, flag=None):
    """A far person: blocks of lit colour (48 to 84 triangles). Legs (leave them out behind a parapet or a row in
    front), a hi-vis body, a head, a hard hat. arms: 'down' (none drawn), 'up' (over the head), 'wave' (over the head
    and swinging side to side). ride: dict(base=, travel=) of the thing it rides (a lorry, a cabin, a balloon): it then
    hops and waves at its own rate while it travels with it. flag: a colour: a flag in the right hand."""
    top = top or r.choice(HIVIS)
    phase = r.random() if phase is None else phase
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])

    common = dict(phase=phase, rate=rate, bob=bob, base=(x, y))
    if ride:
        common.update(ride)
    body = sc.part(P.motion(pivot=(x, y, z), **common), (x, y, z), yaw, s)
    if legs:
        body.box('#3a4a9a', (0, 0, 33), (44, 24, 68), glow=glow * 0.5)
    body.box(top, (0, 0, 95), (50, 30, 60), glow=glow)
    body.box(r.choice(P.SKINS), (0, 0, 142), (36, 34, 36), glow=glow * 0.5)
    body.box(r.choice(HATS), (0, 0, 167), (44, 42, 16), glow=glow)
    if arms == 'down' and not flag:
        return
    for side in (-1, 1):
        if arms == 'wave' or (flag and side > 0):
            arm = sc.part(P.motion(swing=side * wave, pivot=W((side * 31, 0, 120)), axis=yaw + 90.0, **common), (x, y, z), yaw, s)
        else:
            arm = body
        if arms != 'down' or (flag and side > 0):
            arm.box(top, (side * 33, 0, 152), (13, 15, 72), glow=glow)
        if flag and side > 0:
            arm.box(WHITE, (33, 0, 250), (8, 8, 200), glow=glow)
            arm.box(flag, (33 + 62, 0, 310), (116, 6, 72), glow=glow + 0.1)


def fowl(sc, x, y, z, heading=0.0, span=500.0, body=GULL, wing=GULL, beak=YEL, glow=0.6, phase=0.0, rate=1.2, flap=34.0, bob=0.0,
         orbit=None, bank=0.0, dihedral=8.0):
    """A far bird (48 triangles): a body, a beak, two wings that beat about its line of flight. Banked or with its
    wings raised it shows the wing's own colour from the side."""
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    rx, ry = -ty, tx
    tv, base = (0.0, 0.0), (x, y)
    if orbit:
        base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
    common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv)
    L = span * 0.4
    b = sc.part(P.motion(pivot=(x, y, z), **common))
    tilt_box(b, body, (x - tx * L * 0.55, y - ty * L * 0.55, z), (x + tx * L * 0.4, y + ty * L * 0.4, z + L * 0.05), L * 0.34, L * 0.3, glow=glow)
    tilt_box(b, beak, (x + tx * L * 0.4, y + ty * L * 0.4, z + L * 0.08), (x + tx * L * 0.72, y + ty * L * 0.72, z + L * 0.04), L * 0.16, L * 0.14, glow=glow)
    half = span * 0.5
    for side in (-1, 1):
        w = sc.part(P.motion(swing=side * flap, pivot=(x, y, z + L * 0.08), axis=heading, **common))
        th = math.radians(dihedral + side * bank)
        a = (x + side * rx * L * 0.1, y + side * ry * L * 0.1, z + L * 0.1)
        tip = (x + side * rx * half * math.cos(th) - tx * L * 0.25, y + side * ry * half * math.cos(th) - ty * L * 0.25, z + L * 0.1 + half * math.sin(th))
        dx, dy, dz = tip[0] - a[0], tip[1] - a[1], tip[2] - a[2]
        ln = math.sqrt(dx * dx + dy * dy + dz * dz)
        w.box(wing, ((a[0] + tip[0]) / 2.0, (a[1] + tip[1]) / 2.0, (a[2] + tip[2]) / 2.0), (ln, L * 0.8, max(L * 0.09, 8.0)),
              rot=(math.degrees(math.atan2(dz, math.hypot(dx, dy))), math.degrees(math.atan2(dy, dx)), 0), glow=glow)


def stilts(g, col, hx, hy, h, w=220.0, rings=2):
    """Four legs and the beams that tie them: what a far piece stands on."""
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(col, (sx * hx, sy * hy, h / 2.0), (w, w, h))
    for i in range(rings):
        z = h * (i + 1.0) / (rings + 1.0)
        g.box(col, (0, -hy, z), (hx * 2.0, w * 0.6, w * 0.6))
        g.box(col, (0, hy, z), (hx * 2.0, w * 0.6, w * 0.6))


# ------------------------------------------------------------------------------------------ the pieces
def steeplejacks(seed=1, H=11000.0, R0=620.0, R1=380.0):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        n = 6
        for i in range(n):          # the banded chimney
            ra = R0 + (R1 - R0) * i / n
            rb = R0 + (R1 - R0) * (i + 1.0) / n
            g.cyl(WHITE if i % 2 else RED, (0, 0, H * i / n), ra, H / n, steps=10, top_r=rb, glow=0.14)
        for z in (H * 0.5, H):
            rr = R0 + (R1 - R0) * z / H
            g.cyl(PAVE, (0, 0, z), rr + 260, 36, steps=10, glow=0.06)
            g.cyl(HAZ, (0, 0, z + 36), rr + 260, 60, steps=10, glow=0.15)
        g.box(ORANGE, (0, 0, H + 420), (160, 160, 160), glow=0.9)                     # a steady lamp on its mast
        g.box(STEELB_D, (0, 0, H + 200), (40, 40, 400))
        g.box(HAZ, (0, -(R0 + R1) / 2.0 - 40, H / 2.0), (90, 60, H), glow=0.12)       # the ladder, as a band
        for i in range(6):          # the climbers: they work up and down the ladder
            z = 1300.0 + i * 1500.0 + (300.0 if i > 2 else 0.0)
            rr = R0 + (R1 - R0) * z / H
            dot(sc, r, (-150 if i % 2 else 150), -rr - 150, z, s=5.0, arms='up', bob=-r.uniform(260, 420), rate=r.uniform(0.16, 0.26))
        for k, (x, z, ph) in enumerate(((-680.0, 6900.0, 0.0), (700.0, 3600.0, 0.45))):   # the painters' cradles, down its flanks
            y = -300.0
            ride = dict(phase=ph, rate=0.07, bob=-800.0)
            c = sc.part(P.motion(pivot=(x, y, z), base=(x, y), **ride))
            c.box(PAVE, (x, y, z - 20), (760, 260, 40), glow=0.08)
            c.box(HAZ, (x, y - 130, z + 70), (760, 14, 110), glow=0.15)
            for sx in (-1, 1):
                c.box(WHITE, (x + sx * 350, y, z + 1500), (26, 26, 3000), glow=0.1)
            for j in range(3):
                dot(sc, r, x - 240 + j * 240, y + 20, z, s=4.6, arms='up' if j != 1 else 'down', legs=False, bob=ride['bob'], rate=ride['rate'], phase=ride['phase'])
        for i in range(7):          # the gang on the top gallery
            a = math.radians(-90.0 + (i - 3) * 24.0)
            dot(sc, r, math.cos(a) * (R1 + 130), math.sin(a) * (R1 + 130), H + 36, s=5.0, arms='wave' if i % 2 == 0 else 'up', legs=False,
                bob=r.uniform(24, 50), rate=r.uniform(1.1, 1.6), flag=(PINK if i == 3 else None))
    return build


def far_crowd(seed=2, L=9000.0, canopy=TEAL, flags=0):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z0, STEP = 4500.0, 640.0
        stilts(g, PAVE_D, L / 2.0 - 400, 420, Z0 + 2 * STEP, w=240.0, rings=2)
        for k in range(3):
            g.box(PAVE, (0, -330 + k * 400, Z0 + k * STEP - 20), (L, 440, 40), glow=0.06)
        g.box(HAZ, (0, -555, Z0 + 60), (L, 14, 120), glow=0.15)
        g.box(WHITE, (0, 760, Z0 + 1500), (L - 300, 12, 3000), glow=0.24)            # the lit sheet behind them
        g.box(ORANGE, (0, 752, Z0 + 2940), (L - 300, 8, 120), glow=0.35)
        tilt_box(g, canopy, (0, 800, Z0 + 3100), (0, -300, Z0 + 3500), L + 200, 40, glow=0.12)
        for sx in (-1, 1):
            g.box(LAMP, (sx * (L / 2.0 - 200), -560, Z0 + 3360), (300, 20, 140), glow=0.85)
        n = 10
        s = 5.6
        for k in range(3):
            for i in range(n):
                x = -L / 2.0 + (i + 0.5 + (0.25 if k % 2 else -0.1)) * L / n + r.uniform(-80, 80)
                fl = None
                if flags and k == 2 and i in (2, 7):
                    fl = (MAG, CYAN)[i // 5]
                dot(sc, r, x, -330 + k * 400, Z0 + k * STEP, s=s * r.uniform(0.92, 1.06), legs=(k == 0),
                    arms=('wave', 'up', 'down')[(i + k) % 3], bob=r.uniform(26, 60), rate=r.uniform(1.0, 1.7), flag=fl)
    return build


def tower_rim(seed=3, H=9500.0):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(PALE, (0, 0, 0), 2600, 5500, steps=12, top_r=1700, glow=0.07)
        g.cyl(PALE, (0, 0, 5500), 1700, 2000, steps=12, top_r=1760, glow=0.07)
        g.cyl(PALE, (0, 0, 7500), 1760, 2000, steps=12, top_r=2080, glow=0.07)
        g.cyl(TEAL, (0, 0, 4300), 1920, 260, steps=12, glow=0.12)
        g.cyl(MAG, (0, 0, 7250), 1730, 120, steps=12, glow=0.3)
        g.cyl(PAVE, (0, 0, H), 2200, 40, steps=12, glow=0.06)
        g.cyl(HAZ, (0, 0, H + 40), 2200, 70, steps=12, glow=0.15)
        for i in range(3):          # steam, drifting up behind the gang
            st = sc.part(P.motion(phase=i * 0.33, rate=0.08, bob=-260.0, pivot=(0, 600, H + 900)))
            st.sphere(WHITE, (-700 + i * 700, 500 + (i % 2) * 400, H + 800 + i * 520), 760 + i * 130, steps=(4, 6), glow=0.14)
        n = 14
        for i in range(n):
            a = math.radians(-90.0 + (i - (n - 1) / 2.0) * 11.5) + r.uniform(-0.02, 0.02)
            dot(sc, r, math.cos(a) * 2020, math.sin(a) * 2020, H + 40, s=5.8 * r.uniform(0.93, 1.06), legs=False,
                arms=('wave', 'up', 'down', 'wave')[i % 4], bob=r.uniform(28, 62), rate=r.uniform(1.0, 1.7), flag=(PINK if i == 4 else LIME if i == 10 else None))
    return build


def viaduct(seed=4, L=20000.0, Z=4800.0):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(PAVE, (0, 0, Z - 60), (L, 1500, 120), glow=0.06)
        g.box(HAZ, (0, -755, Z + 55), (L, 16, 110), glow=0.16)
        for i in range(5):
            x = -L / 2.0 + (i + 0.5) * L / 5.0
            g.box(PAVE_D, (x, 0, (Z - 120) / 2.0), (320, 1000, Z - 120))
            g.box(STEELB_D, (x, 720, Z + 500), (40, 40, 1000))
            g.box(LAMP, (x, 640, Z + 1010), (220, 200, 60), glow=0.85)
        run = L - 1600.0
        fleet = (('bus', -0.42, -380, 1700.0, CYAN), ('lorry', -0.16, -380, 1700.0, ORANGE), ('bus', 0.12, -380, 1700.0, PINK), ('lorry', 0.37, -380, 1700.0, LIME),
                 ('lorry', -0.3, 380, -1400.0, YEL), ('bus', 0.02, 380, -1400.0, WHITE), ('lorry', 0.33, 380, -1400.0, CYAN))
        for kind, f, y, v, col in fleet:
            x = f * run
            d = 1.0 if v > 0 else -1.0
            ride = dict(base=(x, y), travel=(run, v / run))
            m = sc.part(P.motion(pivot=(x, y, Z + 300), **ride))
            for wx in (-600, 600):
                m.box(DARK, (x + wx, y, Z + 90), (300, 600, 180))
            if kind == 'bus':
                m.box(col, (x, y, Z + 470), (2200, 600, 620), glow=0.18)
                m.box(LAMP, (x, y, Z + 590), (2000, 612, 190), glow=0.65)
            else:
                m.box(col, (x + d * 760, y, Z + 420), (560, 600, 520), glow=0.18)
                m.box(LAMP, (x + d * 1000, y, Z + 520), (90, 520, 170), glow=0.65)
                m.box(PAVE_D, (x - d * 260, y, Z + 230), (1560, 620, 100), glow=0.05)
                for j in range(3):
                    dot(sc, r, x - d * (660 - j * 420), y - 60, Z + 280, s=4.2, arms=('wave', 'up', 'wave')[j], bob=r.uniform(20, 44),
                        rate=r.uniform(1.1, 1.7), ride=ride)
    return build


def cableway(seed=5, SPAN=16000.0, Z=7000.0):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 1):
            px = sx * SPAN / 2.0
            for ax in (-1, 1):
                for ay in (-1, 1):
                    tilt_box(g, PAVE_D, (px + ax * 700, ay * 700, 0), (px + ax * 150, ay * 150, Z), 200, 200)
            g.box(PAVE, (px, 0, Z * 0.45), (1100, 1100, 120))
            g.box(TEAL, (px, 0, Z - 200), (1500, 2100, 900), glow=0.12)                 # the station the cars pass through
            g.box(LAMP, (px, -1056, Z - 150), (900, 10, 300), glow=0.8)
            g.box(STEELB_D, (px, 0, Z + 290), (1700, 2300, 80))
        for y in (-650, 650):
            g.box(PALE, (0, y, Z + 60), (SPAN, 50, 50), glow=0.2)
        run = SPAN
        cols = (CYAN, ORANGE, PINK, LIME)
        for i in range(4):          # lit cabins on the near cable, towards +X
            x = -run / 2.0 + (i + 0.3) * run / 4.0
            ride = dict(base=(x, -650), travel=(run, 560.0 / run))
            m = sc.part(P.motion(pivot=(x, -650, Z), **ride))
            m.box(STEELB_D, (x, -650, Z - 180), (50, 50, 480))
            m.box(cols[i], (x, -650, Z - 760), (900, 560, 700), glow=0.2)
            m.box(LAMP, (x, -650, Z - 660), (760, 572, 240), glow=0.7)
        for i in range(4):          # open man-riders on the far cable, towards -X, their crews waving
            x = -run / 2.0 + (i + 0.7) * run / 4.0
            ride = dict(base=(x, 650), travel=(run, -470.0 / run))
            m = sc.part(P.motion(pivot=(x, 650, Z), **ride))
            m.box(STEELB_D, (x, 650, Z - 420), (50, 50, 960))
            m.box(HAZ, (x, 650, Z - 1080), (1000, 520, 360), glow=0.2)
            for j in range(3):
                dot(sc, r, x - 320 + j * 320, 600, Z - 1180, s=4.0, legs=False, arms=('wave', 'up', 'wave')[(i + j) % 3], rate=r.uniform(1.0, 1.6), ride=ride)
    return build


def _balloon(sc, r, x, y, z, R, cols, ride, riders=3):
    """A hot-air balloon whose envelope's middle is at (x, y, z): four bands of lit colour, the burner, the basket,
    its people from the chest up, waving. ride: what carries it (an orbit; or a float on a tether)."""
    m = sc.part(P.motion(pivot=(x, y, z), **ride))
    bands = ((-1.05, 0.30, 0.50, 0.82), (-0.55, 0.82, 0.55, 1.0), (0.0, 1.0, 0.45, 0.9), (0.45, 0.9, 0.45, 0.35))
    for i, (z0, ra, h, rb) in enumerate(bands):
        m.cyl(cols[i % len(cols)], (x, y, z + z0 * R), ra * R, h * R, steps=10, top_r=rb * R, glow=0.36)
    bz = z - 1.75 * R
    m.box(ORANGE, (x, y, z - 1.2 * R), (R * 0.16, R * 0.16, R * 0.2), glow=0.9)
    for sx in (-1, 1):
        m.box(WHITE, (x + sx * R * 0.2, y, z - 1.35 * R), (24, 24, R * 0.62), glow=0.12)
    m.box(WICKER, (x, y, bz), (R * 0.5, R * 0.42, R * 0.3), glow=0.14)
    s = R / 260.0
    for j in range(riders):
        kw = dict(ride)
        ph = kw.pop('phase', None)
        rt = kw.pop('rate', None)
        bb = kw.pop('bob', 0.0)
        if rt is None:      # an orbiting balloon: its people wave at their own rate
            dot(sc, r, x + (j - (riders - 1) / 2.0) * R * 0.19, y - R * 0.1, bz - s * 30, s=s, legs=False, arms=('wave', 'up', 'wave')[j % 3],
                rate=r.uniform(1.0, 1.6), ride=kw)
        else:               # a floating one: they ride with it
            dot(sc, r, x + (j - (riders - 1) / 2.0) * R * 0.19, y - R * 0.1, bz - s * 30, s=s, legs=False, arms='up', rate=rt, phase=ph, bob=bb, ride=kw)


def balloons(seed=6, tethered=False):
    def build(sc):
        r = random.Random(seed)
        sc.still().box(NAVY, (0, 0, 2), (40, 40, 4))
        fleet = (((2600.0, -600.0, 9000.0), 1100.0, (PINK, WHITE, CYAN, WHITE), (0.0, 0.0, 0.012)),
                 ((-3300.0, 1200.0, 13200.0), 950.0, (YEL, ORANGE, YEL, RED), (-600.0, 800.0, -0.009)),
                 ((300.0, 3600.0, 7200.0), 1250.0, (LIME, WHITE, MAG, WHITE), (800.0, 600.0, 0.007)))
        if tethered:
            fleet = fleet[:2]
        for (x, y, z), R, cols, (cx, cy, rps) in fleet:
            _balloon(sc, r, x, y, z, R, cols, dict(base=(cx, cy), travel=(-1.0, rps)))
        if tethered:    # the observation balloon on its cable, riding up and down over its winch lorry
            x, y, z, R = -400.0, -1500.0, 7600.0, 1200.0
            g = sc.still()
            g.box(ORANGE, (x, y, 260), (1500, 600, 520), glow=0.18)
            g.box(LAMP, (x + 500, y - 306, 380), (360, 8, 160), glow=0.7)
            g.box(WHITE, (x, y, (z - 1.9 * R) / 2.0 + 200), (40, 40, z - 1.9 * R + 400), glow=0.2)
            _balloon(sc, r, x, y, z, R, (WHITE, TEAL, WHITE, ORANGE), dict(base=(x, y), phase=0.2, rate=0.06, bob=-380.0), riders=4)
    return build


def blimp(seed=8, RUN=40000.0, Z=12000.0, SPEED=900.0):
    def build(sc):
        r = random.Random(seed)
        sc.still().box(NAVY, (0, 0, 2), (40, 40, 4))
        ride = dict(base=(0.0, 0.0), travel=(RUN, SPEED / RUN))
        m = sc.part(P.motion(pivot=(0, 0, Z), **ride))
        m.sphere(GULL, (0, 0, Z), 900, scale=(3.4, 1.0, 1.0), steps=(6, 10), glow=0.3)
        m.cyl(PINK, (-260, 0, Z), 912, 520, rot=(-90, 0, 0), steps=10, glow=0.4)            # the crew's pink band round its middle
        m.box(PINK, (-2500, 0, Z + 620), (800, 60, 760), glow=0.35)
        m.box(PINK, (-2500, 0, Z - 620), (800, 60, 760), glow=0.35)
        m.box(PINK, (-2500, 0, Z), (800, 1500, 60), glow=0.35)
        m.box(STEELB, (300, 0, Z - 1010), (1300, 420, 330), glow=0.1)                       # the gondola
        m.box(LAMP, (300, -214, Z - 960), (1100, 8, 150), glow=0.75)
        m.box(PAVE, (-760, 0, Z - 1160), (900, 520, 40), glow=0.08)                         # the open camera deck behind it
        m.box(HAZ, (-760, -262, Z - 1090), (900, 12, 110), glow=0.16)
        m.box(DARK, (-1080, -200, Z - 1000), (220, 150, 150))
        m.box(YEL, (-1080, -200, Z - 1090), (60, 60, 120), glow=0.2)
        for j in range(3):
            dot(sc, r, -900 + j * 250, -60, Z - 1140, s=3.4, legs=False, arms=('down', 'wave', 'up')[j], rate=r.uniform(1.0, 1.5), ride=ride, top=(ORANGE, YEL, ORANGE)[j])
    return build


def kites(seed=9, R=1500.0, H=4400.0):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(PALE, (0, 0, 0), R, H, steps=12, glow=0.07)
        g.cyl(TEAL, (0, 0, H * 0.5), R + 8, 200, steps=12, glow=0.12)
        for i in range(6):
            a = math.radians(i * 60.0 + 30.0)
            g.box(STEELB, (math.cos(a) * (R + 90), math.sin(a) * (R + 90), (H + 500) / 2.0), (110, 110, H + 500))
        g.cyl(PAVE, (0, 0, H), R + 60, 36, steps=12, glow=0.06)
        g.cyl(HAZ, (0, 0, H + 36), R + 60, 60, steps=12, glow=0.15)
        g.box(LAMP, (0, R * 0.7, H + 700), (260, 260, 80), glow=0.85)
        g.box(STEELB_D, (0, R * 0.7, H + 350), (40, 40, 700))
        ks = ((-1000.0, 0, -2600.0, 9300.0, PINK, CYAN), (-330.0, 1, -500.0, 11400.0, YEL, RED), (330.0, 2, 1300.0, 8600.0, LIME, MAG), (1000.0, 3, 3200.0, 10600.0, ORANGE, WHITE))
        n = 9
        for i in range(n):
            a = math.radians(-90.0 + (i - (n - 1) / 2.0) * 17.0)
            flyer = i in (1, 3, 5, 7)
            dot(sc, r, math.cos(a) * (R - 150), math.sin(a) * (R - 150), H + 36, s=5.4 * r.uniform(0.94, 1.05), legs=False, arms='up' if flyer else ('wave', 'down')[i % 4 == 0],
                bob=0.0 if flyer else r.uniform(26, 56), rate=r.uniform(1.0, 1.6))
        for (hx, k, kx, kz, col, col2) in ks:     # each kite swings on its line about its flyer's hands
            hand = (hx, -R * 0.82, H + 36 + 1000.0)
            ky = -R * 0.82 - 500.0
            m = sc.part(P.motion(phase=k * 0.27, rate=0.2 + k * 0.03, swing=13.0, pivot=hand, axis=90.0))
            tilt_box(m, WHITE, hand, (kx, ky, kz - 600), 30, 30, glow=0.25)
            m.box(col, (kx, ky, kz), (1000, 40, 1000), rot=(45, 0, 0), glow=0.5)
            m.box(col2, (kx, ky - 24, kz), (460, 12, 460), rot=(45, 0, 0), glow=0.55)
            for t in range(4):
                m.box(col2 if t % 2 else col, (kx - 120 - t * 110, ky, kz - 900 - t * 330), (300, 30, 150), glow=0.45)
    return build


def gull_tower(seed=10, n=22):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(PAVE_D, (0, 0, 0), 520, 6000, steps=10, top_r=360, glow=0.05)
        g.cyl(HAZ, (0, 0, 5200), 392, 240, steps=10, glow=0.16)
        g.cyl(MAG, (0, 0, 3000), 450, 160, steps=10, glow=0.3)
        for i in range(4):          # the steam they ride on
            st = sc.part(P.motion(phase=i * 0.25, rate=0.07, bob=-320.0, pivot=(0, 0, 7000)))
            st.sphere(WHITE, (r.uniform(-300, 300), r.uniform(-200, 200), 6500 + i * 900), 560 + i * 170, steps=(4, 6), glow=0.14)
        for i in range(n):
            a = r.uniform(0, 6.2832)
            z = 6800.0 + i * 400.0 + r.uniform(-150, 150)
            rad = 900.0 + (z - 6800.0) * 0.3 + r.uniform(-200, 500)
            rps = (0.06, 0.045, -0.05)[i % 3]
            fowl(sc, math.cos(a) * rad, math.sin(a) * rad, z, heading=math.degrees(a) + (90.0 if rps > 0 else -90.0), span=r.uniform(600, 900),
                 phase=r.random(), rate=r.uniform(0.8, 1.3), flap=30.0, bob=-r.uniform(60, 150), orbit=(0.0, 0.0, rps), bank=(-46.0 if rps > 0 else 46.0))
    return build


def lanterns(seed=11, n=30):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        W, D, H = 2600.0, 1700.0, 4600.0
        g.box(STEELB, (0, 0, H / 2.0), (W, D, H), glow=0.04)
        for k in range(4):
            g.box(LAMP, (0, -D / 2.0 - 4, 900 + k * 950), (W - 400, 8, 260), glow=0.5)
        g.box(PAVE, (0, 0, H + 18), (W + 120, D + 120, 36), glow=0.06)
        g.box(HAZ, (0, -D / 2.0 - 60, H + 80), (W + 120, 14, 100), glow=0.15)
        warm = ('#ffd98a', PINK, '#ffe9b0', PINK, '#ffd98a')     # review round: pale yellow and pink only, tall, dimmer: never a pickup's orange box
        for i in range(8):          # the roof party: three still hold theirs
            x = -W / 2.0 + 240 + i * (W - 480) / 7.0
            dot(sc, r, x, -D / 2.0 + 260 + (i % 2) * 300, H + 36, s=5.2 * r.uniform(0.94, 1.05), legs=False, arms='up' if i % 3 == 0 else ('wave', 'down')[i % 2],
                bob=r.uniform(20, 46), rate=r.uniform(1.0, 1.5))
            if i % 3 == 0:
                g.box(warm[i % 5], (x, -D / 2.0 + 260 + (i % 2) * 300, H + 36 + 1270), (260, 260, 520), glow=0.5)
        for i in range(n):          # the lanterns: drifting round on the night air, rising and sinking
            a = r.uniform(0, 6.2832)
            rad = r.uniform(500, 4600)
            x, y, z = math.cos(a) * rad, math.sin(a) * rad, 5600.0 + i * 360.0 + r.uniform(-200, 200)
            m = sc.part(P.motion(phase=r.random(), rate=r.uniform(0.05, 0.1), bob=-r.uniform(160, 420), pivot=(x, y, z), base=(0.0, 0.0),
                                 travel=(-1.0, r.choice((0.006, 0.009, -0.007, 0.012)))))
            sz = r.uniform(280, 420)
            m.box(warm[i % 5], (x, y, z), (sz * 0.8, sz * 0.8, sz * 1.6), glow=0.5)
            m.box(DARK, (x, y, z - sz * 0.88), (sz * 0.4, sz * 0.4, sz * 0.16))
    return build


JOBS = [
    ('SM_P3E_Steeplejacks', steeplejacks(1), dict(far=True, grow=(100.0, 100.0, 900.0), grow_down=(100.0, 100.0, 0.0))),
    ('SM_P3E_FarCrowd_A', far_crowd(2), dict(far=True, grow=(200.0, 100.0, 200.0), grow_down=(200.0, 100.0, 0.0))),
    ('SM_P3E_FarCrowd_B', far_crowd(21, canopy=MAG, flags=2), dict(far=True, grow=(200.0, 100.0, 200.0), grow_down=(200.0, 100.0, 0.0))),
    ('SM_P3E_TowerRim', tower_rim(3), dict(far=True, grow=(200.0, 200.0, 400.0), grow_down=(200.0, 200.0, 0.0))),
    ('SM_P3E_Viaduct', viaduct(4), dict(far=True, grow=(200.0, 100.0, 200.0), grow_down=(200.0, 100.0, 0.0))),
    ('SM_P3E_Cableway', cableway(5), dict(far=True, grow=(200.0, 100.0, 200.0), grow_down=(200.0, 100.0, 0.0))),
    ('SM_P3E_Balloons_A', balloons(6), dict(far=True, grow=(4200.0, 4200.0, 200.0), grow_down=(4200.0, 4200.0, 0.0))),
    ('SM_P3E_Balloons_B', balloons(7, tethered=True), dict(far=True, grow=(4200.0, 4200.0, 500.0), grow_down=(4200.0, 4200.0, 0.0))),
    ('SM_P3E_Blimp', blimp(8), dict(far=True, grow=(20000.0, 200.0, 200.0), grow_down=(20000.0, 200.0, 0.0))),
    ('SM_P3E_Kites', kites(9), dict(far=True, grow=(2600.0, 300.0, 300.0), grow_down=(2600.0, 300.0, 0.0))),
    ('SM_P3E_GullTower', gull_tower(10), dict(far=True, grow=(1600.0, 4400.0, 300.0), grow_down=(1600.0, 4400.0, 0.0))),
    ('SM_P3E_Lanterns', lanterns(11), dict(far=True, grow=(3600.0, 6000.0, 500.0), grow_down=(3600.0, 6000.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
