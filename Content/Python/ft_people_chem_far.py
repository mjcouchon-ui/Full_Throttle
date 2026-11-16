"""FULL THROTTLE (Unreal) the population, level 2, the Chemical Works: FAR LIFE AND CREATURES (/Game/People/ChemicalWorks/SM_Far...).

The extra worker's kit beside ft_people_chem.py (the night shift itself, the main worker's). This one owns the
animals of the works and what lives from the middle distance to the horizon. The works is a night level: navy land,
navy sky, a violet glow along the skyline. So nothing here is small or dark on dark:
  - the sky's birds are the works' floodlit gulls: white, 6 to 9 m across, with a light of their own, so that they
    read against the navy sky (the first test drive: a near-black crow in that sky is not seen at all);
  - the crows (near black, a violet sheen on the wing, a yellow beak, 3 m across) are given a lit white hoarding to
    be seen against, like a shadow play;
  - people are three times life size, blocks of lit hi-vis on a lit footbridge or a pale tank top (at two and a
    third times life size and a glow of 0.08 the first test showed black dots on a white band);
  - dogs and rats stand on something pale and raised (a container stack, a fat lagged pipe), twice life size.
Everything stands HIGH: a pipe rack 7.6 m tall at 26.5 m hides whatever is under 15 m at 75 m, and a tank row (11 m
at 28.5 m) more. Every piece is saved with far=True (the always-moving master): they stand 44 to 260 m out.

Placed by the "far" block of Source/FullThrottle/FTPeopleL1.inl. Run in the editor:
    import ft_people_chem_far; ft_people_chem_far.run()
A piece is drawn like every prop: cm; it stands on z = 0; its FRONT faces -Y (the road); long pieces run along X.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/ChemicalWorks'

# the works' palette (ft_chem / ft_dress_chem) and what the shift wears (ft_people_chem.py uses the same)
STEELB, STEELB_D, TEAL, HAZ, NAVY, MAG, VIOLET, WHITE = '#2a3f8f', '#1c2a66', '#19b5c9', '#ffd23f', '#1c1f4a', '#e84aff', '#b44dff', '#eef6ff'
PAVE, PAVE_D, RUST, DARK = '#7f8fc6', '#56659f', '#a8552f', '#22202a'
ORANGE, LIME, YEL, RED, PINK, CYAN = '#ff7a3c', '#b8ff5a', '#ffc93c', '#ff2d3c', '#ff5fa2', '#2ee6ff'
LAMP = '#fff1c0'
PALE = '#c9d4ff'          # a floodlit tank, lagging on a pipe
HIVIS = (ORANGE, ORANGE, LIME, YEL, '#fff6ea', PINK, CYAN)
HATS = (YEL, '#fff6ea', ORANGE, CYAN, RED)
CROW, CROW_W, GULL, GULL_W = '#2a2230', '#3d2c7a', '#fff6ea', '#cfd8ff'


# ------------------------------------------------------------------------------------------ the far kit's own builders
def fowl(sc, x, y, z, heading=0.0, span=400.0, body=GULL, wing=GULL_W, beak=YEL, glow=0.0, phase=0.0, rate=1.2, flap=38.0, bob=0.0,
         orbit=None, travel=None, bank=0.0, dihedral=0.0):
    """A far bird, cheap (48 triangles; P.bird is 110): a body, a head in the beak's colour, two wings that beat about
    its line of flight. span: tip to tip, cm (3 to 9 m: these are seen from 60 to 300 m). orbit: (cx, cy, rounds a
    second), travel: (length cm, cm a second) as P.figure's. bob < 0: it rises and sinks as it beats; > 0: it hops.
    bank: degrees it is rolled about its line of flight (its left wing up); dihedral: degrees both wings are raised.
    From the road a far bird is seen from the side: flying level, its wings are two edges and the cel pass inks
    them black (the second test drive). Banked or with its wings raised it shows its wing's own colour."""
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    rx, ry = -ty, tx
    tv, base = (0.0, 0.0), (x, y)
    if travel:
        tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    elif orbit:
        base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
    common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv)
    L = span * 0.4
    b = sc.part(P.motion(pivot=(x, y, z), **common))
    b.base = None
    tilt_box(b, body, (x - tx * L * 0.55, y - ty * L * 0.55, z), (x + tx * L * 0.4, y + ty * L * 0.4, z + L * 0.05), L * 0.34, L * 0.3, glow=glow)
    tilt_box(b, beak, (x + tx * L * 0.4, y + ty * L * 0.4, z + L * 0.08), (x + tx * L * 0.72, y + ty * L * 0.72, z + L * 0.04), L * 0.16, L * 0.14, glow=glow)
    half = span * 0.5
    for side in (-1, 1):
        w = sc.part(P.motion(swing=side * flap, pivot=(x, y, z + L * 0.08), axis=heading, **common))
        w.base = None
        th = math.radians(dihedral + side * bank)
        a = (x + side * rx * L * 0.1, y + side * ry * L * 0.1, z + L * 0.1)
        tip = (x + side * rx * half * math.cos(th) - tx * L * 0.25, y + side * ry * half * math.cos(th) - ty * L * 0.25, z + L * 0.1 + half * math.sin(th))
        dx, dy, dz = tip[0] - a[0], tip[1] - a[1], tip[2] - a[2]
        ln = math.sqrt(dx * dx + dy * dy + dz * dz)
        w.box(wing, ((a[0] + tip[0]) / 2.0, (a[1] + tip[1]) / 2.0, (a[2] + tip[2]) / 2.0), (ln, L * 0.8, max(L * 0.09, 8.0)),
              rot=(math.degrees(math.atan2(dz, math.hypot(dx, dy))), math.degrees(math.atan2(dy, dx)), 0), glow=glow)


def blocky(sc, r, x, y, z, yaw=0.0, s=3.0, top=None, glow=0.3, phase=None, rate=1.2, bob=0.0, stride=0.0, swing=0.0, arms='down',
           travel=None, wave=0.0):
    """A far person: five blocks of colour that walk (84 triangles). Blue legs, a hi-vis body, a head, a hard hat, two
    arms; all of it with a light of its own (hi-vis under the work lamps). yaw as P.figure's (90 walks towards +X).
    stride / swing: degrees the legs and arms swing fore and aft. arms: 'down' or 'up' (over the head); wave:
    degrees the raised arms swing side to side."""
    top = top or r.choice(HIVIS)
    phase = r.random() if phase is None else phase
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])

    tv = (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)
    common = dict(phase=phase, rate=rate, bob=bob, base=(x, y), travel=tv)

    def part(sw=0.0, pivot=(0, 0, 100), axis=0.0):
        return sc.part(P.motion(swing=sw, pivot=W(pivot), axis=yaw + axis, **common), (x, y, z), yaw, s)

    body = part()
    body.box(top, (0, 0, 95), (50, 30, 60), glow=glow)
    body.box(r.choice(P.SKINS), (0, 0, 142), (36, 34, 36), glow=glow * 0.5)
    body.box(r.choice(HATS), (0, 0, 167), (44, 42, 16), glow=glow)
    for side in (-1, 1):
        leg = part(side * stride, (side * 13, 0, 66), 0.0) if stride else body
        leg.box('#3a4a9a', (side * 13, 0, 33), (20, 24, 68), glow=glow * 0.5)
        if arms == 'up':
            arm = part(side * wave, (side * 31, 0, 120), 90.0) if wave else body
            arm.box(top, (side * 33, 0, 152), (13, 15, 72), glow=glow)
        else:
            arm = part(-side * swing, (side * 31, 0, 122), 0.0) if swing else body
            arm.box(top, (side * 31, 0, 92), (13, 15, 62), glow=glow)


def lit(fn, glow=0.25):
    """Wrap a piece: every part of it that moves (its creatures) is given a steady light of its own of at least
    `glow`: the works' night leaves an unlit animal black on navy. (P.quadruped has no glow of its own.) Only for
    pieces that stand 24 m and more from the centre line."""
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


# ------------------------------------------------------------------------------------------ the pieces
def flock(seed=1, n=30, r0=1200.0, r1=5200.0, z0=3000.0, z1=8000.0, span=900.0):
    """GULLS OVER THE WORKS: thirty floodlit gulls wheeling in three layers that turn at their own speeds (one of them
    the other way), each banked into its turn and rising and sinking as it beats. Wings 8 to 12 m across, 30 to 80 m up
    (what stands 25 to 40 m from the road hides everything lower at this distance). TALL, always moving."""
    def build(sc):
        r = random.Random(seed)
        for i in range(n):
            a = r.uniform(0, 6.2832)
            rad = r.uniform(r0, r1)
            rps = (0.045, -0.03, 0.06)[i % 3]
            fowl(sc, math.cos(a) * rad, math.sin(a) * rad, r.uniform(z0, z1), heading=math.degrees(a) + (90.0 if rps > 0 else -90.0),
                 span=span * r.uniform(0.85, 1.3), body=GULL, wing=GULL, beak=YEL, glow=0.6, phase=r.random(), rate=r.uniform(0.8, 1.3), flap=30,
                 bob=-r.uniform(60, 160), orbit=(0.0, 0.0, rps), bank=(-48.0 if rps > 0 else 48.0), dihedral=8.0)
        sc.still().box(NAVY, (0, 0, 2), (40, 40, 4))
    return build


def skein(seed=3, run=30000.0, speed=2300.0, span=950.0):
    """THE SKEIN: two V formations of great white birds crossing the works along the road, 36 to 52 m up, wings raised, one after
    the other over 300 m and round again. TALL, always moving."""
    def build(sc):
        r = random.Random(seed)
        for x0, z0, cnt in ((0.0, 3600.0, 13), (run / 2.0, 4600.0, 9)):
            for i in range(cnt):
                k = (i + 1) // 2
                side = 1 if i % 2 else -1
                x = x0 - run / 4.0 - k * 900.0 + r.uniform(-90, 90)
                y = side * k * 700.0 + r.uniform(-60, 60)
                fowl(sc, x, y, z0 + k * 60.0 + r.uniform(-80, 80), heading=0.0, span=span * r.uniform(0.9, 1.15), body=GULL, wing=GULL, beak=ORANGE,
                     glow=0.6, phase=0.13 * k + r.uniform(0, 0.1), rate=0.9, flap=34, bob=-70.0, travel=(run, speed), dihedral=32.0)
        sc.still().box(NAVY, (0, 0, 2), (40, 40, 4))
    return build


def shift_change(seed=2, L=6400.0, H=1600.0, n=(7, 6), s=3.0):
    """SHIFT CHANGE: a lit footbridge 64 m long and 16 m up between two stair huts, and the two shifts filing across
    it in opposite directions, out of one hut and into the other, for ever. Figures three times life size in lit
    hi-vis against a lit white sheet. TALL, always moving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 0, 1):
            g.box(PAVE_D, (sx * (L / 2.0 - 260), 60, H / 2.0), (220, 220, H))
        g.box(PAVE, (0, 0, H - 20), (L, 420, 40))
        g.box(HAZ, (0, -212, H + 16), (L, 6, 32))
        g.box(WHITE, (0, 204, H + 400), (L - 500, 8, 760), glow=0.22)
        g.box(ORANGE, (0, 198, H + 740), (L - 500, 4, 46), glow=0.3)
        for sx in (-1, 1):   # the stair huts the files come out of and go into
            g.box(TEAL, (sx * (L / 2.0 - 280), 0, H + 400), (560, 430, 800))
            g.box(STEELB_D, (sx * (L / 2.0 - 280), 0, H + 820), (620, 480, 40))
            g.box(LAMP, (sx * (L / 2.0 - 280), -218, H + 480), (300, 6, 190), glow=0.8)
            tilt_box(g, PAVE_D, (sx * (L / 2.0 - 280), 260, 0), (sx * (L / 2.0 - 280), 0, H), 200, 30)
        run = L - 760.0
        for cnt, y, yaw, v in ((n[0], -95.0, 90.0, 360.0), (n[1], 95.0, -90.0, -310.0)):
            for i in range(cnt):
                x = -run / 2.0 + (i + r.uniform(0.2, 0.8)) * run / cnt
                blocky(sc, r, x, y, H, yaw=yaw, s=s, rate=r.uniform(0.9, 1.15), bob=8.0, stride=26.0, swing=24.0, travel=(run, v))
    return build


def tank_crew(seed=4, R=800.0, H=2200.0, n=11, s=3.0):
    """THE TANK-TOP CREW: a pale storage tank 22 m tall and the gang on its roof, three times life size, in lit hi-vis,
    arms over their heads, hopping out of step and waving at the chase. An amber lamp on a mast. TALL, always moving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(NAVY, (0, 0, 0), R + 40, 40, steps=14)
        g.cyl(PALE, (0, 0, 40), R, H - 40, steps=14, glow=0.06)
        g.cyl(TEAL, (0, 0, H * 0.55), R + 6, 110, steps=14)
        g.cyl(MAG, (0, 0, H * 0.55 + 150), R + 6, 36, steps=14, glow=0.3)
        g.cyl(PAVE, (0, 0, H), R + 40, 24, steps=14)
        g.cyl(HAZ, (0, 0, H + 24), R + 40, 26, steps=14)
        g.box(HAZ, (R * 0.72, -R * 0.72, H * 0.5), (60, 60, H))                      # the ladder up its flank, as a band
        g.box(STEELB_D, (0, R * 0.5, H + 330), (30, 30, 620))
        g.box(ORANGE, (0, R * 0.5, H + 660), (150, 150, 70), glow=0.9)
        for i in range(n):
            a = math.radians(-90.0 + (i - (n - 1) / 2.0) * 150.0 / n) + r.uniform(-0.04, 0.04)
            rad = R - 150.0 - (140.0 if i % 2 else 0.0)
            blocky(sc, r, math.cos(a) * rad, math.sin(a) * rad, H + 50, yaw=r.uniform(-15, 15), s=s * r.uniform(0.92, 1.08), rate=r.uniform(1.1, 1.7),
                   bob=r.uniform(22, 46), arms='up' if i % 3 != 1 else 'down', wave=24.0 if i % 3 == 0 else 0.0)
    return build


def crow(sc, r, x, y, z, heading=-90.0, span=300.0, bob=0.0, flap=8.0, rate=1.2):
    fowl(sc, x, y, z, heading=heading, span=span, body=CROW, wing=CROW_W, beak=YEL, glow=0.0, phase=r.random(), rate=rate, flap=flap, bob=bob,
         dihedral=24.0)


def crows_rail(seed=5, L=3200.0, Z=1300.0, SH=760.0):
    """THE CROWS' HOARDING: a lit white hoarding 32 m long, 13 to 21 m up on its posts, and the works' crows against it like a
    shadow play: a row hopping and shuffling on its top edge, a row on the fat pipe in front of it, and five that
    flutter up off the pipe and settle again. Crows 3 m across, near black with a violet wing and a yellow beak.
    TALL, always moving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, -0.33, 0.33, 1):
            g.box(PAVE_D, (sx * (L / 2.0 - 60), 60, (Z + SH) / 2.0), (60, 60, Z + SH))
        g.box(WHITE, (0, 40, Z + SH / 2.0), (L, 10, SH), glow=0.24)
        g.box(MAG, (0, 34, Z + 30), (L, 6, 50), glow=0.3)
        g.box(PAVE, (0, 40, Z + SH + 12), (L + 60, 70, 24))
        g.cyl(HAZ, (-L / 2.0 - 40, -150, Z + 190), 46, L + 80, rot=(-90, 0, 0), steps=8, glow=0.08)       # the pipe in front
        for sx in (-1, 0, 1):
            g.box(PAVE_D, (sx * (L / 2.0 - 200), -150, (Z + 150) / 2.0), (50, 50, Z + 150))
        for i in range(9):      # on the top edge
            x = -L / 2.0 + 180 + i * (L - 360) / 8.0 + r.uniform(-60, 60)
            crow(sc, r, x, 40, Z + SH + 60, heading=r.choice((-90, -60, -120, 0, 180)), span=r.uniform(270, 340), bob=r.choice((14, 30, 60)),
                 flap=r.choice((6, 10, 26)), rate=r.uniform(0.8, 1.5))
        for i in range(7):      # on the pipe
            x = -L / 2.0 + 260 + i * (L - 520) / 6.0 + r.uniform(-80, 80)
            crow(sc, r, x, -150, Z + 270, heading=r.choice((-90, -70, -110, 0, 180)), span=r.uniform(270, 340), bob=r.choice((12, 26, 50)),
                 flap=r.choice((6, 10, 24)), rate=r.uniform(0.8, 1.5))
        for i in range(5):      # the ones that flutter up in front of the hoarding and settle again
            x = -L / 2.0 + 500 + i * (L - 1000) / 4.0 + r.uniform(-100, 100)
            crow(sc, r, x, -90, Z + 330, heading=r.choice((0, 180, -90)), span=r.uniform(330, 400), bob=r.uniform(170, 300), flap=42, rate=r.uniform(0.45, 0.7))
    return build


def rat(sc, r, x, y, z, s=2.0, travel=None, col='#d8d0f0', glow=0.3):
    """A rat, far over life size (1.3 m, nose to rump): a pale body, pink ears and tail. It scurries."""
    tv = (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)
    yaw = 0.0 if (not travel or travel[1] >= 0) else 180.0
    m = sc.part(P.motion(phase=r.random(), rate=r.uniform(2.2, 2.9), bob=6.0 * s, base=(x, y), travel=tv, pivot=(x, y, z)), (x, y, z), yaw, s)
    m.box(col, (0, 0, 13), (46, 22, 20), glow=glow)
    m.box(col, (30, 0, 12), (20, 15, 15), glow=glow)
    m.box(PINK, (22, 0, 26), (6, 26, 9), glow=glow)
    m.box(PINK, (-44, 0, 9), (44, 5, 5), glow=glow)


def yard_dogs(seed=6):
    """THE STRAYS OF THE YARD: two containers stacked on an old tank's plinth, a fat lagged pipe behind. On the stack a pack of five dogs, twice
    life size: three bark at the chase (they hop, their heads go, their tails go) and two trot the length of the
    roof and come round again. Along the pipe behind and above them the rats of the works run in a stream, both
    ways. TALL, always moving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L, D, CH, BASE = 1220.0, 260.0, 260.0, 520.0
        g.box(PAVE_D, (20, 60, BASE / 2.0), (L + 260, D + 420, BASE))          # an old tank's plinth: the stack stands 5 m up
        g.box(HAZ, (20, -D / 2.0 - 152, BASE - 30), (L + 260, 6, 40))
        for k, (col, band) in enumerate(((TEAL, WHITE), ('#dfe6ff', ORANGE))):
            z = BASE + k * CH
            g.box(col, (40 * k, 0, z + CH / 2.0), (L, D, CH - 6), glow=0.05)
            g.box(band, (40 * k, -D / 2.0 - 2, z + CH * 0.5), (L * 0.9, 4, 46), glow=0.1)
            for i in range(6):
                g.box(STEELB_D, (-L / 2.0 + 110 + i * 200 + 40 * k, -D / 2.0 - 2, z + CH / 2.0), (10, 4, CH - 30))
        top = BASE + 2 * CH
        g.box(PAVE, (40, 0, top + 4), (L + 20, D + 20, 10), glow=0.05)
        PZ = BASE + 840.0      # the pipe behind: on three trestles, 13.6 m up
        for sx in (-1, 0, 1):
            g.box(PAVE_D, (sx * 700, 300, PZ / 2.0), (60, 60, PZ))
        g.cyl(STEELB, (-900, 300, PZ), 70, 1800, rot=(-90, 0, 0), steps=8)      # dark, so that the pale rats show on it (pale on a pale pipe: lost)
        for x in (-520, 180, 700):
            g.cyl(MAG, (x, 300, PZ), 78, 40, rot=(-90, 0, 0), steps=8, glow=0.25)
        dog = dict(length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=24)
        coats = (('#f1e4c8', '#8e5a3a'), ('#e8c26a', '#fff6ea'), ('#fff6ea', '#5a4a3c'), ('#d9a070', '#fff6ea'), ('#cfcfd8', '#22202a'))
        for i, x in enumerate((-430, -60, 330)):   # barking at the road
            body, ears = coats[i]
            P.quadruped(sc, x + 40, -40, top + 9, yaw=r.uniform(-25, 25), s=r.uniform(2.0, 2.4), body=body, ears=ears, snout=ears, tail_col=ears,
                        phase=r.random(), rate=r.uniform(1.6, 2.2), bob=r.uniform(14, 26), graze=14.0, wag=38.0, **dog)
        for i, (x, v) in enumerate(((-250, 420.0), (300, -360.0))):   # trotting the roof
            body, ears = coats[3 + i]
            P.quadruped(sc, x + 40, 70, top + 9, yaw=90.0 if v > 0 else -90.0, s=2.1, body=body, ears=ears, snout=ears, tail_col=ears, phase=r.random(),
                        rate=2.4, bob=8.0, stride=30.0, wag=24.0, travel=(L - 240.0, v), **dog)
        for i in range(10):
            v = 520.0 if i % 3 else -430.0
            rat(sc, r, -800 + i * 172 + r.uniform(-40, 40), 300 + (-18 if v > 0 else 18), PZ + 68, s=r.uniform(1.8, 2.4), travel=(1660.0, v))
    return build


JOBS = [
    ('SM_FarFlock_A', flock(1), dict(far=True, grow=(5600.0, 5600.0, 300.0), grow_down=(5600.0, 5600.0, 0.0))),
    ('SM_FarFlock_B', skein(3), dict(far=True, grow=(16000.0, 200.0, 200.0), grow_down=(16000.0, 200.0, 0.0))),
    ('SM_FarShift_A', shift_change(2), dict(far=True, grow=(60.0, 60.0, 80.0))),
    ('SM_FarTankCrew', tank_crew(4), dict(far=True, grow=(60.0, 60.0, 160.0))),
    ('SM_FarCrows_A', crows_rail(5), dict(far=True, grow=(120.0, 120.0, 400.0))),
    ('SM_FarYardDogs', lit(yard_dogs(6)), dict(far=True, grow=(200.0, 80.0, 120.0), grow_down=(200.0, 80.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
