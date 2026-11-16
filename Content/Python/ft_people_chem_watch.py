"""FULL THROTTLE (Unreal) the population, level 2: the Volta Chemical Works, THE WATCHERS (/Game/People/ChemicalWorks/SM_Watch*).

The night shift has downed tools to watch the chase. This road runs behind its own metre of barrier, the kerb and its
stores, and then pipe racks (to 7.6 m) or tank rows (to 11 m) at the sight line: a watcher who is to be seen from the
road stands 38 m and more out and HIGH (decks from 7 m up to 16 m), in a MASS, in hi-vis against lit white sheeting,
a third to a half over life size, and hops, waves and swings flags out of step, livelier as the chase comes by.

    SM_WatchStand_A / _B   a works grandstand run up out of scaffold tube: four (three) stepped decks, 7 to 13 m up, lit
                           sheeting behind every row, the shift massed on it, flags, a striped banner under the first deck
    SM_WatchLift_A / _B    a mast lift (two of them side by side in _B), its cage 16 m up over everything, three or four
                           of the shift in it waving a flag
    SM_WatchRoof           a control block: the office staff and the guards along the roof edge (9 m) and the balcony
    SM_WatchTankFar        (always moving) a storage tank 22 m tall, its roof crowded, the stair round it lined: 60 to 130 m

Placed by the "watch" block of Source/FullThrottle/FTPeopleL1.inl. Every piece is TALL (24 m or more from the centre
line; the table keeps them 39 m and more out) and carries a steady work light only.
Run in the editor:  import ft_people_chem_watch; ft_people_chem_watch.run()
A piece is drawn like every prop: cm; it stands on z = 0; its FRONT faces -Y (the road); long pieces run along X.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/ChemicalWorks'

STEELB, STEELB_D, TEAL, HAZ, NAVY, MAG, VIOLET, WHITE = '#2a3f8f', '#1c2a66', '#19b5c9', '#ffd23f', '#1c1f4a', '#e84aff', '#b44dff', '#eef6ff'
PAVE, PAVE_D, DARK = '#7f8fc6', '#56659f', '#22202a'
POLE = '#aab6e0'
BOARD = '#d9b98a'
ORANGE, LIME, YEL, RED, PINK, CYAN = '#ff7a3c', '#b8ff5a', '#ffc93c', '#ff2d3c', '#ff5fa2', '#2ee6ff'
LAMP = '#fff1c0'
HIVIS = (ORANGE, ORANGE, LIME, YEL, '#fff6ea', PINK)
HATS = (YEL, '#fff6ea', ORANGE, CYAN, RED)
GLOW = 0.25          # the light of its own that hi-vis has under the work lamps (steady; these pieces stand 40 m out). At 0.07 the
                     # shift read as dark teeth in a white strip from 100 m and as a black mass from the side (review, 15 Nov)

CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=9, rate=1.5),
    dict(arm_r='wave', swing_r=30, rate=1.3, bob=5),
    dict(arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=9, rate=1.3),
    dict(arm_l='wave', arm_r='wave', swing_l=-26, swing_r=26, rate=1.1, bob=6),
    dict(arm_r='point', arm_l='hip', swing_r=14, axis_r='x', rate=0.8, bob=3),
)


def worker(r, **fixed):
    top = r.choice(HIVIS)
    d = P.look(r, tops=HIVIS, bottoms=('#2b2350', '#3a5fa8', DARK), sleeves='long', wear='trousers', hat='hard',
               hat_col=r.choice(HATS), band=('#fff6ea' if top != '#fff6ea' else ORANGE))
    d['top'] = top
    d.update(fixed)
    return d


def fan(sc, r, x, y, z, s, react=True, arms=None, tops=HIVIS, hats=HATS, rate=None, bob=None):
    """One of a mass: a broad block of hi-vis, short dark legs, a head and a hard hat (48 triangles), hopping on its own
    beat. The block is the widest and tallest part on purpose: the cel pass inks every edge, and from 100 m a narrow
    figure is all ink.
    arms: None, 'up' (both arms over the head, hopping with it) or 'wave' (both arms swing side to side)."""
    mo = dict(phase=r.random(), rate=rate or r.uniform(1.1, 1.7), bob=bob if bob is not None else r.uniform(8, 16) * s, react=react)
    m = sc.part(P.motion(**mo), (x, y, z), 0.0, s)
    top = r.choice(tops)
    m.box('#3a5fa8', (0, 0, 27), (42, 24, 54), glow=GLOW)
    m.box(top, (0, 0, 91), (64, 30, 74), glow=GLOW)
    m.box(r.choice(P.SKINS), (0, 0, 145), (38, 34, 34), glow=GLOW)
    m.box(r.choice(hats), (0, 0, 168), (46, 42, 14), glow=GLOW)
    if arms == 'up':
        for sx in (-1, 1):
            tilt_box(m, top, (sx * 36, 0, 116), (sx * 52, 0, 190), 16, 16)
    elif arms == 'wave':
        for sx in (-1, 1):
            a = sc.part(P.motion(swing=sx * 26.0, pivot=(x + sx * 36 * s, y, z + 118 * s), axis=90.0, **mo), (x, y, z), 0.0, s)
            a.box(top, (sx * 40, 0, 156), (16, 16, 78), glow=GLOW)
    return top


def lit(fn, glow=GLOW):
    """Everything in the piece that moves is given a little light of its own (the figure builder's people have none)."""
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


def lamp(g, x, y, z, glow=0.8):
    g.box(POLE, (x, y, z + 60), (9, 9, 120))
    g.box(DARK, (x, y - 14, z + 124), (70, 44, 28))
    g.box(LAMP, (x, y - 32, z + 110), (60, 28, 5), glow=glow)


def banner(sc, x, y, z, L, H, cols, phase=0.0):
    """A striped banner hung by its top edge, stirring (stripes and blocks: never words)."""
    m = sc.part(P.motion(phase=phase, rate=0.35, swing=5.0, pivot=(x, y, z + H), axis=0.0))
    n = len(cols)
    for k, c in enumerate(cols):
        m.box(c, (x - L / 2.0 + (k + 0.5) * L / n, y, z + H / 2.0), (L / n, 5, H), glow=0.22)
    m.box(DARK, (x, y, z + H + 4), (L + 10, 8, 8))


# ------------------------------------------------------------------------------------------ the grandstand
def stand(seed=1, L=1400.0, tiers=(700.0, 900.0, 1100.0, 1300.0), per=6, s=1.5, cols=(PINK, WHITE, CYAN, WHITE, PINK, WHITE, CYAN, WHITE), tops=HIVIS):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        n = len(tiers)
        TD = 170.0                       # a deck's depth
        D = TD * n
        y0 = -D / 2.0
        bays = max(int(round(L / 350.0)), 2)
        # the standards: a row at every step, as tall as the deck it carries and its rail
        for k in range(bays + 1):
            x = -L / 2.0 + L * k / bays
            for j in (0, n):             # the front and the back row: the rows between are hidden by the sheeting
                top = (tiers[min(j, n - 1)] + (110 if j < n else 330))
                g.box(POLE, (x, y0 + j * TD, top / 2.0), (11, 11, top))
            g.box(NAVY, (x, 0, 4), (40, D + 40, 8))
        for k in range(0, bays, 2):      # diagonals on the front face, under the first deck
            xa, xb = -L / 2.0 + L * k / bays, -L / 2.0 + L * (k + 1) / bays
            tilt_box(g, POLE, (xa, y0 - 2, 30), (xb, y0 - 2, tiers[0] - 240), 10, 10)
            tilt_box(g, POLE, (xb, y0 - 2, 30), (xa, y0 - 2, tiers[0] - 240), 10, 10)
        for z in (tiers[0] * 0.33, tiers[0] * 0.66):
            g.box(POLE, (0, y0, z), (L, 9, 9))
        for j, z in enumerate(tiers):
            yc = y0 + (j + 0.5) * TD
            g.box(BOARD, (0, yc, z - 7), (L + 30, TD + 6, 14))
            g.box(HAZ, (0, y0 + j * TD - 4, z + 6), (L + 30, 5, 26))
            # the lit sheeting behind the row: the pale ground its colours read against
            hz = (tiers[j + 1] - z) if j < n - 1 else 320.0
            g.box(WHITE, (0, y0 + (j + 1) * TD, z + hz / 2.0 + 20), (L, 5, hz + 40), glow=0.2)
            if j == n - 1:
                g.box(ORANGE, (0, y0 + (j + 1) * TD - 4, z + hz + 14), (L, 3, 26), glow=0.3)
        for sx in (-1, 1):               # the ends closed with dark sheet, so the steps read as a block
            for j, z in enumerate(tiers):
                g.box(STEELB, (sx * (L / 2.0 + 8), y0 + (j + 0.5) * TD, z - 110), (5, TD, 220))
            lamp(g, sx * (L / 2.0 - 60), y0 + n * TD, tiers[-1] + 330)
        banner(sc, 0, y0 - 10, tiers[0] - 250, L - 80, 230, cols, phase=r.random())
        # the shift: the front row are whole figures, waving and with flags; the rows behind a hopping mass
        for j, z in enumerate(tiers):
            yc = y0 + (j + 0.5) * TD - 10
            m = per + (j % 2)
            for i in range(m):
                x = -L / 2.0 + 90 + (L - 180) * (i + 0.5) / m + r.uniform(-18, 18)
                if j == 0:
                    d = worker(r)
                    d.update(CHEER[(i * 2 + seed) % len(CHEER)])
                    d['s'] = s * r.uniform(0.95, 1.03)
                    if i in (1, m - 2):
                        d.update(arm_r='wave', swing_r=26, hold_r=P.hold_flag(col=(PINK if i == 1 else CYAN), w=90.0), rate=0.9, bob=4)
                    P.figure(sc, x, yc, z=z, yaw=r.uniform(-15, 15), phase=r.random(), react=True, detail=0, **d)
                else:
                    fan(sc, r, x, yc, z, s * r.uniform(0.92, 1.05), arms=('up', 'wave', None, 'up')[(i + j + seed) % 4], tops=tops)
    return build


# ------------------------------------------------------------------------------------------ the mast lift
def lift(seed=2, masts=((0.0, 1600.0, 4),), s=1.55):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for (x0, H, n) in masts:
            # the carriage: a chassis on four wheels, outriggers down
            g.box(ORANGE, (x0, 0, 95), (420, 210, 90))
            g.box(DARK, (x0, 0, 45), (460, 180, 30))
            for sx in (-1, 1):
                for sy in (-1, 1):
                    g.cyl(DARK, (x0 + sx * 150, sy * 105 - 15, 45), 45, 30, rot=(0, 0, 90), steps=8)
                    tilt_box(g, HAZ, (x0 + sx * 200, sy * 100, 80), (x0 + sx * 270, sy * 190, 6), 16, 16)
                    g.box(DARK, (x0 + sx * 270, sy * 190, 4), (50, 50, 8))
            # the mast: three sections, hazard bands
            for k, (w, z0, z1) in enumerate(((90, 140, H * 0.4), (66, H * 0.4, H * 0.72), (46, H * 0.72, H - 10))):
                g.box(POLE if k % 2 else HAZ, (x0, 20, (z0 + z1) / 2.0), (w, w, z1 - z0))
                g.box(DARK, (x0, 20, z1 - 12), (w + 12, w + 12, 24))
            # the cage, and the lit sheet hung behind it
            W = 150.0 * n * s / 1.55 + 60
            g.box(BOARD, (x0, 0, H - 6), (W, 160, 12))
            g.box(HAZ, (x0, -82, H + 8), (W, 6, 28))
            g.box(HAZ, (x0, -82, H + 110), (W, 8, 8))
            for sx in (-1, 1):
                g.box(HAZ, (x0 + sx * W / 2.0, 0, H + 110), (8, 160, 8))
                g.box(HAZ, (x0 + sx * W / 2.0, -80, H + 55), (8, 8, 110))
                g.box(HAZ, (x0 + sx * W / 2.0, 80, H + 170), (8, 8, 340))
            g.box(WHITE, (x0, 82, H + 190), (W, 5, 340), glow=0.22)
            g.box(ORANGE, (x0, 78, H + 345), (W, 3, 22), glow=0.3)
            lamp(g, x0, 82, H + 340)
            for i in range(n):
                x = x0 - W / 2.0 + 50 + (W - 100) * (i + 0.5) / n
                d = worker(r)
                d.update(CHEER[(i * 3 + seed) % 4])
                d['s'] = s * r.uniform(0.95, 1.02)
                if i == 0:
                    d.update(arm_l='wave', swing_l=-28, hold_l=P.hold_flag(col=PINK, w=110.0), rate=0.9, bob=4)
                P.figure(sc, x, -52, z=H, yaw=r.uniform(-12, 12), phase=r.random(), react=True, detail=1, **d)
    return build


# ------------------------------------------------------------------------------------------ the control block
def staff(r, **fixed):
    """The office and the laboratory: no hard hats, bright shirts and white coats; one in three a guard in red."""
    if r.random() < 0.34:
        d = P.look(r, sleeves='long', wear='trousers', hat='peak', hat_col=RED, band='#fff6ea')
        d.update(top=RED, bottom=DARK, shoes=DARK)
    elif r.random() < 0.5:
        d = P.look(r, sleeves='long', wear='coat')
        d.update(top='#fff6ea')
    else:
        d = P.look(r, tops=(PINK, CYAN, YEL, LIME, '#fff6ea', ORANGE))
    d.update(fixed)
    return d


def roof(seed=4, L=1600.0, D=640.0, H=900.0, s=1.5):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        yf = -D / 2.0
        g.box(PAVE_D, (0, 60, H / 2.0), (L, D - 120, H))
        g.box(STEELB_D, (0, 60, H - 20), (L + 30, D - 90, 40))
        g.box(LAMP, (0, yf + 118, 640), (L - 160, 4, 150), glow=0.45)            # the control room's window band
        for k in range(1, 6):
            g.box(PAVE_D, (-L / 2.0 + 80 + k * (L - 160) / 6.0, yf + 116, 640), (14, 6, 150))
        g.box(TEAL, (-L / 2.0 + 150, yf + 118, 110), (150, 5, 220))
        # the balcony under the windows
        BZ = 450.0
        g.box(PAVE, (0, yf + 60, BZ - 10), (L - 100, 130, 20))
        g.box(HAZ, (0, yf, BZ + 12), (L - 100, 6, 30))
        g.box(HAZ, (0, yf, BZ + 105), (L - 100, 8, 8))
        for k in range(7):
            x = -L / 2.0 + 50 + k * (L - 100) / 6.0
            g.box(HAZ, (x, yf, BZ + 55), (8, 8, 110))
            tilt_box(g, PAVE_D, (x, yf + 4, BZ - 20), (x, yf + 120, BZ - 150), 12, 12)
        banner(sc, 0, yf - 8, BZ - 240, L - 300, 220, (LIME, WHITE, PINK, WHITE, LIME, WHITE, PINK, WHITE), phase=r.random())
        n = 6
        for i in range(n):
            x = -L / 2.0 + 140 + (L - 280) * (i + 0.5) / n + r.uniform(-20, 20)
            d = staff(r)
            d.update(CHEER[(i * 2 + seed) % len(CHEER)])
            d['s'] = 1.35 * r.uniform(0.95, 1.03)
            if i == 2:
                d.update(arm_r='face', arm_l='down', swing_r=0, swing_l=0, hold_r=P.hold_phone(glow=0.6), rate=0.4, bob=-2)
            P.figure(sc, x, yf + 62, z=BZ, yaw=r.uniform(-15, 15), phase=r.random(), react=True, detail=0, **d)
        # the roof: a parapet, the lit plant housing behind, and everyone who could get up the stairs
        g.box(STEELB, (0, yf + 126, H + 22), (L, 12, 44))
        g.box(WHITE, (0, yf + 330, H + 190), (L - 300, 8, 380), glow=0.2)
        g.box(ORANGE, (0, yf + 324, H + 365), (L - 300, 3, 24), glow=0.3)
        g.box(STEELB_D, (0, yf + 420, H + 150), (L - 340, 170, 300))
        for sx in (-1, 1):
            lamp(g, sx * (L / 2.0 - 190), yf + 330, H + 380)
        m = 11
        for i in range(m):
            x = -L / 2.0 + 110 + (L - 220) * (i + 0.5) / m + r.uniform(-14, 14)
            fan(sc, r, x, yf + 200 + (30 if i % 2 else -20), H, s * r.uniform(0.92, 1.05), arms=('up', 'wave', None)[(i + seed) % 3],
                tops=(PINK, CYAN, YEL, LIME, '#fff6ea', ORANGE, RED), hats=(RED, '#fff6ea', '#2a2230', '#6a4428', YEL))
    return build


# ------------------------------------------------------------------------------------------ the far tank
def tank_far(seed=5, R=800.0, H=2200.0, s=2.4):
    """Seen from 60 to 130 m: a pale storage tank 22 m tall, its roof crowded with the shift (two and a third times life size, a
    hopping band of hi-vis against a lit sheet), two great flags, more of them on the landings of the stair. Always moving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(PAVE, (0, 0, 0), R, H, steps=16)
        g.cyl(TEAL, (0, 0, H * 0.55), R + 4, 130, steps=16)
        g.cyl(HAZ, (0, 0, H - 60), R + 6, 60, steps=16)
        g.cyl(PAVE_D, (0, 0, 0), R + 60, 70, steps=16)
        # the stair up the front: two flights and two landings
        for k, (xa, za, xb, zb) in enumerate(((-640, 0, -190, H * 0.42), (130, H * 0.42, 420, H * 0.72))):
            xl = xb + 160                                                    # the landing's middle
            wall = -math.sqrt(R * R - xl * xl)                               # the tank's wall behind the landing's middle
            tilt_box(g, HAZ, (xa, -R - 60, za + 20), (xb, -R - 60, zb), 26, 110)
            g.box(BOARD, (xl, (-R - 175 + wall) / 2.0, zb - 8), (320, wall + R + 175, 16))   # the deck runs back to the wall
            g.box(HAZ, (xl, -R - 172, zb + 110), (320, 9, 9))
            for sx in (-1, 1):
                g.box(HAZ, (xl + sx * 156, -R - 172, zb + 55), (9, 9, 110))
                tilt_box(g, POLE, (xl + sx * 150, -R - 160, zb - 10), (xl + sx * 150, wall, zb - 260), 12, 12)   # a bracket to the wall
            for i in range(2):
                fan(sc, r, xl - 70 + i * 140, -R - 90, zb, 1.9, react=False, arms=('up', 'wave')[i])
        g.box(WHITE, (0, 140, H + 300), (R * 1.5, 8, 600), glow=0.25)
        g.box(ORANGE, (0, 134, H + 575), (R * 1.5, 3, 40), glow=0.3)
        for sx in (-1, 1):
            lamp(g, sx * R * 0.7, 140, H + 600)
            # a great flag at each end of the crowd, swung to and fro
            f = sc.part(P.motion(phase=0.25 * (sx + 1), rate=0.7, swing=24.0, pivot=(sx * R * 0.82, -R * 0.2, H), axis=90.0))
            f.box(POLE, (sx * R * 0.82, -R * 0.2, H + 330), (12, 12, 660))
            f.box(PINK if sx < 0 else CYAN, (sx * R * 0.82 + 150, -R * 0.2, H + 560), (300, 6, 200), glow=0.2)
            f.box(WHITE, (sx * R * 0.82 + 150, -R * 0.2 - 4, H + 560), (300, 3, 60), glow=0.2)
        n = 16
        for i in range(n):
            a = math.radians(200.0 + 140.0 * (i + 0.5) / n)                # round the front of the roof
            rr = R - 110 - (130 if i % 2 else 0)
            fan(sc, r, math.cos(a) * rr * 0.92, math.sin(a) * rr * 0.75, H, s * r.uniform(0.92, 1.05), react=False,
                arms=('up', 'wave', None, 'up')[i % 4], tops=(ORANGE, LIME, YEL, PINK, CYAN, '#fff6ea'))
        for k in range(16):
            a = math.radians(k * 22.5)
            g.box(HAZ, (math.cos(a) * (R - 20), math.sin(a) * (R - 20), H + 60), (10, 10, 120))
    return build


JOBS = [
    ('SM_WatchStand_A', lit(stand(1))),
    ('SM_WatchStand_B', lit(stand(7, L=1800.0, tiers=(760.0, 980.0, 1200.0), per=8, cols=(ORANGE, WHITE, LIME, WHITE, ORANGE, WHITE, LIME, WHITE, ORANGE, WHITE)))),
    ('SM_WatchLift_A', lit(lift(2), 0.22)),
    ('SM_WatchLift_B', lit(lift(9, masts=((-340.0, 1350.0, 3), (340.0, 1750.0, 3))), 0.22)),
    ('SM_WatchRoof', lit(roof(4))),
    ('SM_WatchTankFar', tank_far(5), dict(far=True, grow=(160.0, 160.0, 160.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
