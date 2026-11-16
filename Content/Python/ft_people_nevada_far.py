"""FULL THROTTLE (Unreal) the population of level 3, Nevada (level index 2): FAR LIFE AND CREATURES, /Game/People/Nevada/SM_Far*.

The extra worker's kit for the far band (150 m to the land's edge and the horizon) and the level's running creatures.
The main kit (ft_people_nevada.py) has the people by the road and its own herds out to about 140 m; these pieces stand
beyond that, and are built for it: three times life size, a few strong coats (black, white, gold, red) on a PALE strip
of ground that the piece brings with it (the land is orange: a dark horse on orange is lost, on a pale trail it reads),
and one big gesture each.

    SM_FarWagonTrain    prospectors heading for the strike: three covered wagons, mule teams, outriders, a striped pennant
    SM_FarRidgeRiders   a ridge of the canyon's own rock, and riders in single file along its crest against the sky
    SM_FarMustangs      a wild herd at the gallop down a long pale swell, dust rolling behind it
    SM_FarCattleDrive   longhorns strung out on the trail, drovers swinging their hats, the dust of the drive
    SM_FarPronghorns    a band of pronghorn bounding across the flat, out of step, white rumps flashing
    SM_FarRunners       (middle distance, 38 m and out) a rock ledge: roadrunners sprinting its length, jackrabbits bounding

What the game's frames taught (15 Nov): at 150 m and beyond the haze turns everything pink and the ink eats thin dark
shapes, so coats are white, gold, cream and black, bodies are deep, and the ridge riders are DARK against the sky.

All but the last are saved far=True (M_PeopleFar: they keep moving past 180 m). Everything moves through the vertex
shader; no light, no words, no marks. The placing rules are the "far" block of Source/FullThrottle/FTPeopleL2.inl.
Run in the editor:  import ft_people_nevada_far; ft_people_nevada_far.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/Nevada'

SAND, SANDST, SANDST_D, SANDST_L, ROCK_R, TALUS = '#f2c58a', '#e0784a', '#b8553a', '#f59a62', '#c9603e', '#a04a34'
PALE, DUST = '#f7cf98', '#fff1dc'          # the trail a herd runs on, and what it kicks up
SAGE, SAGE_D, WOOD, BARK_D = '#8fb86a', '#5f8f4a', '#9a6a44', '#6a4c36'
INK, NAVY, TEAL, RED, MAG, CYAN = '#22202a', '#2b2350', '#2fc6c0', '#ff2d3c', '#ff4fb8', '#2ee6ff'
CREAM, WHITE, GOLD, YEL, DENIM = '#f4efe6', '#fff6ea', '#ffc93c', '#ffe14d', '#3a5fa8'
ROCK_TONES = (TALUS, SANDST_D, ROCK_R, SANDST, SANDST_L)


# ------------------------------------------------------------------------------------------ the ground a piece brings
def swell(g, r, L, D, H, tones=(SANDST_D, SANDST, PALE), foot=-150.0, top=0.5, rough=0.12, n=7, step=500.0):
    """A long low rise along X, `n` points a side, each bed stepping in: the top bed is the pale strip life runs on.
    Returns the half depth of the flat top."""
    beds = len(tones)
    edge = [(1.0 + r.uniform(-rough, rough)) for _ in range(2 * n)]
    half = D / 2.0
    for k, col in enumerate(tones):
        f = 1.0 - (1.0 - top) * k / max(beds - 1, 1)
        z0 = foot if k == 0 else H * k / beds
        z1 = H * (k + 1) / beds
        pts = []
        for i in range(n):          # the road's side, left to right
            u = i / (n - 1.0)
            taper = 0.35 + 0.65 * math.sin(math.pi * (0.08 + 0.84 * u))
            pts.append(((u - 0.5) * (L + step * (beds - k)), -half * f * taper * edge[i]))
        for i in range(n):          # the far side, right to left
            u = 1.0 - i / (n - 1.0)
            taper = 0.35 + 0.65 * math.sin(math.pi * (0.08 + 0.84 * u))
            pts.append(((u - 0.5) * (L + step * (beds - k)), half * f * taper * edge[n + i]))
        g.plan(col, pts, z0, z1)
    return half * top * 0.35


def bush(g, r, x, y, z, s=1.0):
    g.sphere(SAGE if r.random() < 0.5 else SAGE_D, (x, y, z + 40 * s), 70 * s, scale=(1.2, 1.0, 0.75), steps=(4, 7))


def puff(sc, x, y, z, size, phase, rate, tv, col=DUST):
    """Dust that goes along with a runner: a flattened ball, swelling and sinking."""
    m = sc.part(P.motion(phase=phase, rate=rate, bob=-size * 0.22, base=(x, y), travel=tv, pivot=(x, y, z)))
    m.sphere(col, (x, y, z + size * 0.5), size, scale=(1.6, 1.0, 0.72), steps=(4, 8))


# ------------------------------------------------------------------------------------------ a far animal
def beast(sc, x, y, z=0.0, yaw=0.0, s=1.0, coat='#8e5a3a', mane=None, belly=None, legs_col=None, length=160.0, height=122.0, girth=56.0,
          neck=58.0, neck_up=40.0, head=44.0, horns=None, prong=None, rump=None, ears=None, tail=45.0, tail_col=None, tail_dip=25.0, tail_w=None,
          phase=0.0, rate=0.0, bob=0.0, stride=0.0, pitch=0.0, travel=None, rider=None):
    """A four-legged animal of about a hundred triangles, made to be seen side-on from a long way off: its head towards
    +X at yaw 0 (180 heads it along -X). The body rocks (pitch) about its middle and the four legs scissor (stride).
    horns: a colour (a longhorn's, out and up); prong: a colour (two short uprights); rump: a colour (a pronghorn's
    white patch); ears: a colour (a mule's long ones). travel: (length cm along the mesh's X, speed cm/s: negative with
    yaw 180). rider: dict for _rider(). Returns the top of the head (mesh z)."""
    c, sn = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):
        return (x + s * (p[0] * c - p[1] * sn), y + s * (p[0] * sn + p[1] * c), z + s * p[2])

    tv = (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)
    common = dict(phase=phase, rate=rate, bob=bob, base=(x, y), travel=tv)

    def part(swing=0.0, pivot=(0.0, 0.0, 0.0)):
        return sc.part(P.motion(swing=swing, pivot=W(pivot), axis=yaw + 90.0, **common), (x, y, z), yaw, s)

    legs_col = legs_col or coat
    hl = length / 2.0
    mid = (0.0, 0.0, height - girth / 2.0)
    b = part(pitch, mid)
    b.box(coat, mid, (length, girth, girth))
    if belly:
        b.box(belly, (0, 0, height - girth * 0.88), (length * 0.8, girth * 1.04, girth * 0.34))
    if rump:
        b.box(rump, (-hl + girth * 0.12, 0, height - girth * 0.45), (girth * 0.34, girth * 1.06, girth * 0.86))
    a = math.radians(neck_up)
    n0 = (hl - girth * 0.25, 0.0, height - girth * 0.35)
    n1 = (n0[0] + math.cos(a) * neck, 0.0, n0[2] + math.sin(a) * neck)
    tilt_box(b, coat, n0, n1, girth * 0.5, girth * 0.56)
    b.box(coat, (n1[0] + head * 0.3, 0, n1[2] + head * 0.02), (head, girth * 0.46, head * 0.56))
    if mane:
        tilt_box(b, mane, (n0[0] - girth * 0.24, 0, n0[2] + girth * 0.34), (n1[0] - girth * 0.18, 0, n1[2] + girth * 0.34), girth * 0.24, girth * 0.34)
    hx, hz = n1[0] + head * 0.05, n1[2] + head * 0.3
    if horns:
        for side in (-1, 1):
            tilt_box(b, horns, (hx, side * girth * 0.2, hz), (hx + head * 0.5, side * girth * 1.0, hz + head * 1.0), head * 0.2, head * 0.2)
    if prong:
        for side in (-1, 1):
            b.box(prong, (hx, side * girth * 0.16, hz + head * 0.45), (head * 0.2, head * 0.16, head * 0.9))
    if ears:
        for side in (-1, 1):
            b.box(ears, (hx - head * 0.25, side * girth * 0.2, hz + head * 0.5), (head * 0.22, head * 0.16, head * 1.0))
    lw = max(girth * 0.3, 5.0)
    if tail:
        td = math.radians(tail_dip)
        t0 = (-hl + 2.0, 0.0, height - girth * 0.14)
        tilt_box(b, tail_col or mane or coat, t0, (t0[0] - math.cos(td) * tail, 0.0, t0[2] - math.sin(td) * tail), tail_w or lw * 0.8, tail_w or lw * 0.8)
    leg_h = height - girth * 0.8
    for lx, side, sgn in ((hl - lw, 1, 1), (hl - lw, -1, -1), (-hl + lw, 1, -1), (-hl + lw, -1, 1)):
        ly = side * (girth / 2.0 - lw / 2.0)
        m = part(sgn * stride, (lx, ly, leg_h)) if stride else b
        m.box(legs_col, (lx, ly, leg_h / 2.0), (lw * 1.15, lw, leg_h + 4.0))
    if rider:
        _rider(sc, W, common, (x, y, z), yaw, s, height, girth, rider)
    return z + s * (n1[2] + head * 0.6)


def _rider(sc, W, common, at, yaw, s, height, girth, d):
    """Somebody in the saddle, drawn in the horse's own frame: a block of a body, a wide hat, one arm that waves.
    d: top, bottom, skin, hat (colours), blanket; act: 'hat' (waves it), 'flag' (carries a pennant: flag, stripe), None."""
    m = sc.part(P.motion(pivot=W((0, 0, height)), axis=yaw + 90.0, **common), at, yaw, s)
    m.box(d.get('blanket', RED), (-2, 0, height + 2), (60, girth + 6, 7))
    m.box(d.get('bottom', DENIM), (6, 0, height - 16), (22, girth + 16, 46))
    m.box(d['top'], (0, 0, height + 38), (28, 42, 62))
    m.box(d.get('skin', '#d9a070'), (3, 0, height + 84), (26, 28, 30))
    act = d.get('act')
    hat = d.get('hat', CREAM)
    if act != 'hat':
        m.box(hat, (3, 0, height + 101), (66, 66, 6))
        m.box(hat, (3, 0, height + 111), (32, 34, 18))
    if not act:
        return
    sh = (0.0, -26.0, height + 62.0)
    a = sc.part(P.motion(swing=d.get('swing', 28.0), pivot=W(sh), axis=yaw + 90.0, **common), at, yaw, s)
    hand = (6.0, -32.0, height + 122.0)
    tilt_box(a, d['top'], sh, hand, 13, 13)
    if act == 'hat':
        a.box(hat, (hand[0], hand[1], hand[2] + 8), (58, 58, 6))
        a.box(hat, (hand[0], hand[1], hand[2] + 17), (30, 30, 16))
    elif act == 'flag':
        a.box(CREAM, (hand[0], hand[1], hand[2] + 50), (6, 6, 130))
        a.box(d.get('flag', RED), (hand[0] - 62, hand[1], hand[2] + 82), (118, 5, 70))
        a.box(d.get('stripe', WHITE), (hand[0] - 62, hand[1], hand[2] + 82), (120, 6, 20))


HORSE_COATS = (('#fff6ea', '#cfcfd8'), ('#ffc93c', '#fff6ea'), ('#22202a', '#22202a'), ('#f4efe6', '#22202a'), ('#c94f2e', '#fff6ea'),
               ('#cfcfd8', '#2b2350'), ('#ffe14d', '#22202a'), ('#fff6ea', '#c94f2e'))
RIDER_TOPS = (RED, CYAN, GOLD, MAG, WHITE, TEAL)


def hand_look(r, i, act=None, **kw):
    d = dict(top=RIDER_TOPS[i % len(RIDER_TOPS)], bottom=r.choice((DENIM, NAVY, '#5a4a3c')), skin=r.choice(P.SKINS), hat=r.choice((CREAM, '#5a4a3c', WHITE, INK)),
             blanket=r.choice((RED, TEAL, GOLD, MAG)), act=act)
    d.update(kw)
    return d


# ------------------------------------------------------------------------------------------ the pieces
def wagon_train(sc):
    """Prospectors heading for the strike: three covered wagons behind their mules on a pale trail, the canvas banded
    in the outfit's colours, an outrider with a striped pennant ahead and one behind waving his hat. They cross 150 m
    and come round again."""
    r = random.Random(301)
    g = sc.still()
    L, S, Z = 15000.0, 3.6, 260.0
    swell(g, r, L + 1400, 2200, Z, top=0.55)
    for k in range(6):
        bush(g, r, -L / 2.0 + 900 + k * 2600 + r.uniform(-300, 300), r.choice((-1, 1)) * r.uniform(520, 600), Z - 120, 1.8)
    sp, rate = 300.0, 1.1
    covers = ((CREAM, RED), (WHITE, TEAL), (CREAM, MAG))
    unit = 3000.0
    mules = ('#cfcfd8', '#f4efe6', '#d9a070')
    for i, (cover, trim) in enumerate(covers):
        x = 2300.0 - i * unit
        y = (-60, 50, -20)[i]
        ph = i * 0.37
        tv = (L, sp / L)
        m = sc.part(P.motion(phase=ph, rate=rate, bob=-6.0, base=(x, y), travel=tv, pivot=(x, y, Z + 300)), (x, y, Z), 0.0, S)
        m.box(WOOD, (0, 0, 118), (320, 130, 64))
        m.box(trim, (0, 0, 140), (328, 138, 24))
        rod(m, cover, (-150, 0, 180), (150, 0, 180), 86, steps=8)                 # the canvas, flared at both ends
        for sx in (-1, 1):
            rod(m, cover, (sx * 150, 0, 182), (sx * 176, 0, 184), 100, steps=8)
            rod(m, trim, (sx * 62 - 9, 0, 180), (sx * 62 + 9, 0, 180), 89, steps=7)   # two bands of the outfit's colour
        m.box(BARK_D, (178, 0, 186), (6, 120, 120))                               # the dark mouth of the cover
        for wx, wr in ((-104, 80), (112, 62)):
            for side in (-1, 1):
                rod(m, trim, (wx, side * 66, wr), (wx, side * 86, wr), wr, steps=7)
                m.box(GOLD, (wx, side * 88, wr), (wr * 0.6, 5, wr * 0.6))
        m.box(RIDER_TOPS[(i + 2) % 6], (206, -24, 196), (30, 44, 66))             # the driver
        m.box(r.choice(P.SKINS), (208, -24, 244), (28, 30, 32))
        m.box(INK if i == 1 else GOLD, (208, -24, 262), (70, 70, 7))
        m.box(INK if i == 1 else GOLD, (208, -24, 273), (32, 34, 18))
        m.box(BARK_D, (320, 0, 96), (200, 9, 9))                                  # the tongue
        for k, side in enumerate((-1, 1)):
            beast(sc, x + S * 360.0, y + side * S * 46.0, Z, s=S, coat=mules[(i + k) % 3], mane=INK, ears=INK, legs_col=INK,
                  length=150, height=116, girth=52, neck=46, neck_up=34, head=42, tail=44, phase=ph + k * 0.5, rate=rate, bob=7, stride=20, pitch=2,
                  travel=(L, sp))
        puff(sc, x - S * 300.0, y, Z, 190, ph, rate, tv)
    beast(sc, 3900.0, -180.0, Z, s=S * 1.05, coat=WHITE, mane='#cfcfd8', tail=62, tail_w=16, phase=0.2, rate=rate * 1.4, bob=12, stride=24, pitch=3,
          travel=(L, sp), neck_up=50, rider=hand_look(r, 0, 'flag', flag=RED, stripe=WHITE, swing=12, top=NAVY))
    beast(sc, -5600.0, 140.0, Z, s=S * 1.02, coat=GOLD, mane=WHITE, tail=62, tail_w=16, phase=0.6, rate=rate * 1.4, bob=12, stride=24, pitch=3,
          travel=(L, sp), neck_up=50, rider=hand_look(r, 2, 'hat', hat=CREAM, top=RED))
    px = -6600.0                                                                   # the pack mule at the tail, its load
    beast(sc, px, -90.0, Z, s=S * 0.9, coat='#cfcfd8', mane=INK, ears=INK, legs_col=INK, length=150, height=116, girth=52, neck=46, neck_up=30,
          head=42, tail=44, phase=0.9, rate=rate, bob=7, stride=20, travel=(L, sp))
    pm = sc.part(P.motion(phase=0.9, rate=rate, bob=7, base=(px, -90.0), travel=(L, sp / L), pivot=(px, -90.0, Z)), (px, -90.0, Z), 0.0, S * 0.9)
    pm.box(CREAM, (-6, 0, 140), (90, 78, 50))
    pm.box(TEAL, (-6, 0, 140), (32, 82, 52))


def ridge_riders(sc):
    """A ridge of the canyon's rock, and five riders in single file along its crest against the sky: dark horses, bright
    shirts, the leader carrying a pennant, two waving their hats at the chase below. Five times life size: they stand
    a quarter of a kilometre off."""
    r = random.Random(311)
    g = sc.still()
    L, H, S = 9000.0, 1100.0, 5.0
    swell(g, r, L, 3000, H, tones=(TALUS, SANDST_D, ROCK_R, SANDST, SAND), top=0.3, foot=-300.0, rough=0.16, n=8, step=400.0)
    TL, sp, rate = 8000.0, 480.0, 1.4
    coats = ((INK, INK), ('#5e3b28', INK), (INK, WHITE), ('#3b2a22', INK), (INK, INK))
    for i in range(5):
        coat, mane = coats[i]
        act = ('flag', 'hat', None, 'hat', None)[i]
        d = hand_look(r, i, act, flag=RED, stripe=WHITE, swing=(12 if act == 'flag' else 30), hat=(WHITE if i % 2 else GOLD), bottom=NAVY)
        beast(sc, 3000.0 - i * 1550.0 + r.uniform(-90, 90), r.uniform(-40, 40), H, s=S * r.uniform(0.95, 1.05), coat=coat, mane=mane, tail=62, tail_w=18,
              neck_up=48, phase=i * 0.23, rate=rate, bob=16, stride=24, pitch=3, travel=(TL, sp), rider=d)


def mustangs(sc):
    """A wild herd at the gallop: fourteen horses and two foals stretched out along a pale swell 200 m long, the dust
    rolling behind them, a white stallion in front. They cross it and come round again."""
    r = random.Random(321)
    g = sc.still()
    L, S, Z = 20000.0, 3.8, 300.0
    swell(g, r, L + 1600, 3000, Z, top=0.55)
    for k in range(10):
        bush(g, r, -L / 2.0 + 900 + k * 2000 + r.uniform(-400, 400), r.choice((-1, 1)) * r.uniform(720, 800), Z - 130, 1.8)
    sp, rate = 1500.0, 2.1
    tv = (L, sp / L)
    n = 14
    for i in range(n):
        coat, mane = HORSE_COATS[i % len(HORSE_COATS)] if i else (WHITE, WHITE)
        x = 4200.0 - i * 640.0 + r.uniform(-160, 160)
        y = ((i * 5) % 7 - 3) * 140.0 + r.uniform(-40, 40) if i else -60.0
        ph = r.random()
        beast(sc, x, y, Z, s=S * (1.15 if i == 0 else r.uniform(0.9, 1.06)), coat=coat, mane=mane, belly=('#c94f2e' if i % 5 == 3 else None), tail=70, tail_w=20,
              tail_dip=8, neck_up=30, phase=ph, rate=rate, bob=44, stride=34, pitch=7, travel=(L, sp))
        if i % 2:
            puff(sc, x - 1100.0, y + 60, Z, 250, ph, rate * 0.5, tv)
    for k in range(2):
        beast(sc, 900.0 - k * 2700.0, 420.0 - k * 760, Z, s=S * 0.55, coat=('#ffc93c', '#fff6ea')[k], mane=WHITE, tail=50, tail_w=16, tail_dip=8, neck_up=40,
              phase=0.3 + k * 0.4, rate=rate * 1.3, bob=28, stride=40, pitch=6, travel=(L, sp))
    for k in range(3):
        puff(sc, -5200.0 - k * 800.0, (k - 1) * 280.0, Z, 380 - k * 60, k * 0.3, 0.7, tv)


def cattle_drive(sc):
    """A trail drive: a dozen longhorns strung out on the trail, white, red and black, horns up, two drovers swinging
    their hats on the flanks and the dust of the drive hanging behind."""
    r = random.Random(331)
    g = sc.still()
    L, S, Z = 17000.0, 3.3, 280.0
    swell(g, r, L + 1500, 3000, Z, top=0.55)
    for k in range(8):
        bush(g, r, -L / 2.0 + 900 + k * 2100 + r.uniform(-400, 400), r.choice((-1, 1)) * r.uniform(720, 800), Z - 130, 1.8)
    sp, rate = 520.0, 1.5
    tv = (L, sp / L)
    kinds = (dict(coat=WHITE, belly='#b8553a', tail_col='#b8553a'), dict(coat='#b8553a', belly=WHITE), dict(coat=INK, belly='#cfcfd8'),
             dict(coat=GOLD, belly=CREAM), dict(coat=WHITE, belly=INK, tail_col=INK))
    for i in range(12):
        x = 3600.0 - i * 640.0 + r.uniform(-140, 140)
        y = ((i * 3) % 5 - 2) * 160.0 + r.uniform(-40, 40)
        ph = r.random()
        beast(sc, x, y, Z, s=S * r.uniform(0.9, 1.08), length=190, height=125, girth=62, neck=30, neck_up=6, head=44, horns=CREAM, tail=56, tail_dip=75,
              phase=ph, rate=rate, bob=9, stride=22, pitch=3, travel=(L, sp), **kinds[i % len(kinds)])
        if i % 3 == 1:
            puff(sc, x - 900.0, y, Z, 230, ph, rate * 0.5, tv)
    beast(sc, 1900.0, -600.0, Z, s=S * 1.05, coat='#ffc93c', mane=WHITE, tail=62, tail_w=16, neck_up=46, phase=0.1, rate=rate * 1.3, bob=12, stride=26, pitch=3,
          travel=(L, sp), rider=hand_look(r, 0, 'hat', hat=CREAM))
    beast(sc, -4500.0, 60.0, Z, s=S * 1.05, coat=WHITE, mane=INK, tail=62, tail_w=16, neck_up=46, phase=0.55, rate=rate * 1.3, bob=12, stride=26, pitch=3,
          travel=(L, sp), rider=hand_look(r, 1, 'hat', hat=INK))
    for k in range(3):
        puff(sc, -5200.0 - k * 800.0, (k - 1) * 300.0, Z, 340 - k * 50, k * 0.3, 0.6, tv)


def pronghorns(sc):
    """A band of pronghorn bounding across the flat: twelve of them, out of step, each leap a body's height, white
    rumps and bellies flashing."""
    r = random.Random(341)
    g = sc.still()
    L, S, Z = 15000.0, 3.6, 240.0
    swell(g, r, L + 1400, 2800, Z, top=0.55)
    for k in range(8):
        bush(g, r, -L / 2.0 + 800 + k * 1900 + r.uniform(-300, 300), r.choice((-1, 1)) * r.uniform(680, 760), Z - 120, 1.8)
    sp = 1300.0
    for i in range(12):
        x = 3300.0 - i * 600.0 + r.uniform(-150, 150)
        y = ((i * 4) % 7 - 3) * 140.0 + r.uniform(-40, 40)
        beast(sc, x, y, Z, s=S * r.uniform(0.88, 1.08), coat=('#ffc93c' if i % 3 else '#ff9a3c'), belly=WHITE, rump=WHITE, prong=INK, legs_col=INK,
              length=118, height=96, girth=40, neck=42, neck_up=62, head=30, tail=0, phase=r.random(), rate=r.uniform(1.0, 1.3), bob=r.uniform(240, 360),
              stride=36, pitch=9, travel=(L, sp))


def _roadrunner(sc, x, y, z, s, phase, travel, rate=4.2, body=DENIM):
    """A roadrunner at full stretch, heading +X: neck and tail level, crest back, long legs a blur."""
    tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    common = dict(phase=phase, rate=rate, bob=5.0, base=(x, y), travel=tv)

    def W(p):
        return (x + s * p[0], y + s * p[1], z + s * p[2])

    b = sc.part(P.motion(swing=5.0, pivot=W((0, 0, 60)), axis=90.0, **common), (x, y, z), 0.0, s)
    tilt_box(b, body, (-26, 0, 62), (26, 0, 70), 28, 24)
    b.box(WHITE, (0, 0, 53), (50, 29, 14))
    tilt_box(b, WHITE, (22, 0, 66), (52, 0, 84), 15, 15)                   # the neck, stretched
    b.box(body, (58, 0, 90), (28, 18, 18))
    b.box(RED, (55, 0, 89), (12, 19, 9))                                   # the patch behind the eye
    tilt_box(b, INK, (52, 0, 99), (28, 0, 114), 9, 10)                     # the crest
    tilt_box(b, GOLD, (70, 0, 89), (102, 0, 87), 8, 8)                     # the beak
    tilt_box(b, body, (-24, 0, 64), (-90, 0, 84), 14, 9)                   # the long tail
    b.box(WHITE, (-90, 0, 86), (18, 15, 8))
    for side, sgn in ((-1, 1), (1, -1)):
        m = sc.part(P.motion(swing=sgn * 42.0, pivot=W((0, side * 7, 52)), axis=90.0, **common), (x, y, z), 0.0, s)
        m.box(GOLD, (0, side * 7, 26), (7, 7, 54))
        m.box(GOLD, (8, side * 7, 3), (20, 8, 6))


def _jackrabbit(sc, x, y, z, s, phase, travel, rate=1.5, hop=60.0, coat='#eebf98'):
    """A jackrabbit in mid bound, heading +X: all hind leg and ear, the ears black at the tips."""
    tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    common = dict(phase=phase, rate=rate, bob=hop, base=(x, y), travel=tv)

    def W(p):
        return (x + s * p[0], y + s * p[1], z + s * p[2])

    b = sc.part(P.motion(swing=10.0, pivot=W((0, 0, 30)), axis=90.0, **common), (x, y, z), 0.0, s)
    tilt_box(b, coat, (-22, 0, 34), (20, 0, 26), 24, 24)
    b.box(WHITE, (-2, 0, 20), (34, 25, 8))
    b.box(coat, (30, 0, 36), (22, 18, 18))
    b.box(WHITE, (-28, 0, 40), (12, 14, 12))                               # the scut
    for side in (-1, 1):
        tilt_box(b, coat, (28, side * 6, 44), (14, side * 9, 84), 7, 9)    # the ears
        tilt_box(b, INK, (16, side * 9, 78), (12, side * 10, 92), 7, 9)
    for sgn, lx, ln in ((1, 14, 22), (-1, -18, 30)):                       # fore and hind legs, flung out
        m = sc.part(P.motion(swing=sgn * 34.0, pivot=W((lx, 0, 22)), axis=90.0, **common), (x, y, z), 0.0, s)
        m.box(coat, (lx, 0, 22 - ln / 2.0), (9 if sgn > 0 else 13, 26, ln))


def runners(sc):
    """A ledge of red rock, sage and a barrel cactus on it: three roadrunners sprint its length nose to tail, and four
    jackrabbits bound along behind them, ears up. They come round again."""
    r = random.Random(351)
    g = sc.still()
    L, H = 2300.0, 240.0
    swell(g, r, L, 640, H, tones=(TALUS, SANDST_D, ROCK_R, SAND), top=0.62, rough=0.14, step=110.0)
    for k in range(4):
        bush(g, r, -L / 2.0 + 300 + k * 560 + r.uniform(-80, 80), 150 + r.uniform(-20, 20), H - 30, 0.55)
    for cx in (-560, 660):
        g.cyl(SAGE_D, (cx, 120, H - 10), 34, 96, steps=7, top_r=28)
        g.box(MAG, (cx, 120, H + 92), (20, 20, 12))
    TL = 2100.0
    for i in range(3):
        _roadrunner(sc, 500.0 - i * 400.0, -80.0 + i * 12.0, H, 2.6, i * 0.37, (TL, 760.0), body=(DENIM, TEAL, DENIM)[i])
    for i in range(4):
        _jackrabbit(sc, -820.0 + i * 480.0 + r.uniform(-40, 40), 40.0 + (i % 2) * 40.0, H, 2.4, r.random(), (TL, 300.0), rate=r.uniform(1.3, 1.7),
                    hop=r.uniform(70, 110), coat=('#eebf98', '#f6d3b4')[i % 2])


JOBS = [
    ('SM_FarWagonTrain', wagon_train, dict(far=True, grow=(200.0, 120.0, 120.0), grow_down=(200.0, 120.0, 0.0))),
    ('SM_FarRidgeRiders', ridge_riders, dict(far=True, grow=(200.0, 120.0, 200.0), grow_down=(200.0, 120.0, 0.0))),
    ('SM_FarMustangs', mustangs, dict(far=True, grow=(200.0, 120.0, 160.0), grow_down=(200.0, 120.0, 0.0))),
    ('SM_FarCattleDrive', cattle_drive, dict(far=True, grow=(200.0, 120.0, 120.0), grow_down=(200.0, 120.0, 0.0))),
    ('SM_FarPronghorns', pronghorns, dict(far=True, grow=(200.0, 120.0, 500.0), grow_down=(200.0, 120.0, 0.0))),
    ('SM_FarRunners', runners, dict(grow=(120.0, 80.0, 260.0), grow_down=(120.0, 80.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
