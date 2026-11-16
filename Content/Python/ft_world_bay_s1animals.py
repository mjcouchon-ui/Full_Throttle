"""Bayshore, Stage 1, builder s1animals: the animals of the seven districts. /Game/World/Bayshore/SM_S1ANIMALS_*.
Ground: pelicans on mooring posts, sandpipers running the wet sand, crabs, macaws on a perch, cats on lobster pots,
peacocks (the fan opens), goats on a dry-stone wall, seals on a rock, dogs chasing along the beach, doves round a bird
bath, egrets in a reed bed, a pannier donkey with its keeper, a carriage horse, a giant tortoise, swans on a pool.
Water (far=True): sea turtles, flying fish, one whale with her calf. Sky (far=True): a line of pink spoonbills, a wide
slow ring of white terns. Every bird is white, pink or bright; nothing dark circles; nothing is thrown.
Placed by the block C:/ftwork/s1/L0_s1animals.inl. Run: import ft_world_bay_s1animals as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box            # tilt_box(m, col, (x,y,z) from, (x,y,z) to, width, depth)

DIR = '/Game/World/Bayshore'

WHITE, CREAM, SAND, DSAND = '#fffaf2', '#fff1d6', '#f6e3b6', '#d6bd8c'
WOOD, DWOOD, LWOOD, OCHRE = '#c9a06a', '#9a744a', '#c08a5a', '#e8b26a'
ROCK, DROCK, STEEL = '#b8b4c8', '#8a86a0', '#c9c4d6'
TEAL, BLUE, SKY, TURQ, MINT = '#2fc6c0', '#4a7fc0', '#8fe3ff', '#49e2d5', '#7fe3c4'
PINK, HOT, RED, CORAL, YEL, GOLD = '#ff9ec7', '#ff4fb8', '#ff4d5e', '#ff7a8a', '#ffe14d', '#e2b45c'
LEAF, DLEAF, NAVY, EYE = '#2fbf71', '#1f9a62', '#2b2350', '#22202a'


# ------------------------------------------------------------------------------------------ helpers
class Fr(object):
    """A creature's own frame (as ft_people.quadruped's): it stands on (x, y, z), its head towards -Y at yaw 0 (yaw 90
    heads it along +X). part() hands out the model for the parts that swing one way; pivots and axes are given in the
    creature's frame. sway: dict(swing, pivot (MESH space), axis): every part of the creature swings that one way."""

    def __init__(self, sc, x, y, z=0.0, yaw=0.0, s=1.0, phase=0.0, rate=0.0, bob=0.0, travel=None, orbit=None, sway=None):
        self.sc, self.x, self.y, self.z, self.yaw, self.s, self.sway = sc, x, y, z, yaw, s, sway
        self.cy, self.sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        tv, base = (0.0, 0.0), (x, y)
        if travel:
            tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
        elif orbit:
            base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
        self.common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv)

    def W(self, p):
        x, y, z, s, cy, sy = self.x, self.y, self.z, self.s, self.cy, self.sy
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])

    def part(self, swing=0.0, pivot=(0, 0, 0), axis=0.0, **over):
        mo = dict(self.common)
        mo.update(over)
        if self.sway:
            mo.update(swing=self.sway['swing'], pivot=self.sway['pivot'], axis=self.sway['axis'])
        else:
            mo.update(swing=swing, pivot=self.W(pivot), axis=(P.UP if axis == P.UP else self.yaw + axis))
        return self.sc.part(P.motion(**mo), (self.x, self.y, self.z), self.yaw, self.s)


def bird_s(sc, x, y, z=0.0, yaw=0.0, s=1.0, body=WHITE, wing=STEEL, beak=YEL, legs=GOLD, bl=40.0, bh=26.0, bw=24.0, leg=14.0,
           neck=14.0, neck_up=60.0, head=9.0, beak_len=12.0, beak_w=5.0, tail=12.0, tail_drop=4.0, tail_col=None, crest=None,
           pouch=None, one_leg=False, feet=True, phase=0.0, rate=0.5, peck=0.0, turn=0.0, bob=0.0, stride=0.0, flap=0.0,
           travel=None, orbit=None, sway=None):
    """A bird on its feet (ft_people.bird is a bird in the air): chunky, big head. Standing on (x, y, z), head to -Y.
    bl, bh, bw: the body's length, height, width; leg, neck, head (its radius), beak_len: cm. peck: degrees the neck
    and head nod about the shoulder; turn: degrees the head turns (instead); flap: degrees the wings beat, held out;
    stride: degrees the legs swing. Returns the top of the head (mesh space)."""
    f = Fr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=bob, travel=travel, orbit=orbit, sway=sway)
    t4 = 4.3 / s                                 # nothing thinner than 4 cm as placed
    zb = leg + bh * 0.5
    mid = (0, 0, zb)
    sh = (0, -bl * 0.3, leg + bh * 0.7)          # the shoulder: where the neck leaves the body
    a = math.radians(neck_up)
    hd = (0, sh[1] - math.cos(a) * neck, sh[2] + math.sin(a) * neck)
    b = f.part(pivot=mid)
    b.sphere(body, mid, 1.0, scale=(bw * 0.5, bl * 0.5, bh * 0.5), steps=(4, 6))
    if tail:
        tilt_box(b, tail_col or wing, (0, bl * 0.34, zb + bh * 0.1), (0, bl * 0.5 + tail, zb + bh * 0.1 - tail_drop), max(bw * 0.5, t4), max(bh * 0.16, t4))
    lw = max(bw * 0.14, t4)
    if leg > 3.0:
        for sd in (-1, 1):
            if one_leg and sd == 1:
                continue
            m = f.part(sd * stride, (sd * bw * 0.2, 0, leg), 0.0) if stride else f.part(pivot=mid)
            m.box(legs, (sd * bw * 0.2, 0, leg * 0.5), (lw, lw, leg + 2.0))
            if feet:
                m.box(legs, (sd * bw * 0.2, -lw, t4 * 0.5), (lw * 1.6, lw * 2.6, t4))
    for sd in (-1, 1):
        if flap:                                 # a wing held out and beating
            m = f.part(sd * flap, (sd * bw * 0.36, 0, zb + bh * 0.3), 90.0)
            tilt_box(m, wing, (sd * bw * 0.36, 0, zb + bh * 0.3), (sd * (bw * 0.36 + bl * 0.8), bl * 0.12, zb + bh * 0.5), bl * 0.5, max(bh * 0.12, t4))
        else:
            m = f.part(pivot=mid)
            m.box(wing, (sd * bw * 0.46, bl * 0.06, zb + bh * 0.08), (max(bw * 0.16, t4), bl * 0.66, bh * 0.56))
    if peck:
        h = f.part(peck, sh, 0.0)
    elif turn:
        h = f.part(turn, sh, P.UP)
    else:
        h = f.part(pivot=mid)
    nw = max(bw * 0.3, t4)
    tilt_box(h, body, sh, hd, nw, nw)
    h.sphere(body, hd, head, steps=(3, 5))
    tip = (0, hd[1] - head * 0.7 - beak_len, hd[2] - beak_len * 0.25)
    tilt_box(h, beak, (0, hd[1] - head * 0.6, hd[2]), tip, max(beak_w, t4), max(beak_w * 0.8, t4))
    if pouch:
        tilt_box(h, pouch, (0, hd[1] - head * 0.5, hd[2] - head * 0.55), (0, tip[1] + beak_len * 0.15, tip[2] - beak_w * 0.7), max(beak_w * 0.9, t4), max(beak_w * 1.3, t4))
    for sd in (-1, 1):
        h.box(EYE, (sd * head * 0.82, hd[1] - head * 0.35, hd[2] + head * 0.2), (t4, t4, t4))
    if crest:
        tilt_box(h, crest, (0, hd[1], hd[2] + head * 0.7), (0, hd[1] + head * 0.6, hd[2] + head * 1.8), t4, max(head * 0.8, t4))
    return z + s * (hd[2] + head)


def q_body(sc, x, y, z, yaw, s, height, phase=0.0, rate=0.0, bob=0.0, travel=None):
    """The model ft_people.quadruped draws the animal's BODY into (same motion key): for a saddle, a collar, panniers."""
    return Fr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=bob, travel=travel).part(0.0, (0, 0, height), 0.0)


def q_neck(sc, x, y, z, yaw, s, length, height, girth, graze, phase=0.0, rate=0.0, bob=0.0):
    """The model quadruped draws the NECK AND HEAD into when graze is not 0: for long ears, a plume."""
    return Fr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=bob).part(graze, (0, -length / 2.0 + girth * 0.2, height - girth * 0.3), 0.0)


def q_head(length, height, girth, neck, neck_up):
    """Where quadruped puts the top of the neck (the animal's own frame)."""
    a = math.radians(neck_up)
    n0 = (0, -length / 2.0 + girth * 0.25, height - girth * 0.35)
    return (0, n0[1] - math.cos(a) * neck, n0[2] + math.sin(a) * neck)


def post(g, x, y, h, r=11.0, col=WOOD, cap=WHITE, foot=150.0):
    g.cyl(col, (x, y, -foot), r, h + foot, steps=7)
    g.cyl(cap, (x, y, h - 9), r + 2.5, 9, steps=7)


def reeds(sc, r, x0, y0, n, spread, hmin, hmax, phase):
    """A clump of reeds and pampas plumes that sways about its own foot."""
    m = sc.part(P.motion(swing=4.0, pivot=(x0, y0, 0), axis=20.0, rate=0.3, phase=phase))
    for k in range(n):
        x, y = x0 + r.uniform(-spread, spread), y0 + r.uniform(-spread * 0.5, spread * 0.5)
        h, lean = r.uniform(hmin, hmax), r.uniform(-14, 14)
        tilt_box(m, LEAF if k % 3 else DLEAF, (x, y, -25), (x + lean, y, h), 6.5, 5.0)
        if k % 2 == 0:
            m.box(CREAM, (x + lean * 1.08, y, h + 10), (10, 7, 30))


# ------------------------------------------------------------------------------------------ the ground
PELICAN = dict(body=WHITE, wing=STEEL, beak='#ffe9a0', pouch='#ffc59e', legs=CORAL, bl=44.0, bh=28.0, bw=26.0, leg=6.0, neck=22.0,
               neck_up=70.0, head=9.0, beak_len=30.0, beak_w=6.0, tail=8.0, s=1.5)


def pelican_posts(sc):
    """Four mooring posts with a sagging rope and a coil; three white pelicans on them, one on the sand."""
    g = sc.still()
    xs, hs = (-270, -90, 90, 270), (70, 100, 84, 56)
    for x, h in zip(xs, hs):
        post(g, x, 0, h)
        g.cyl(BLUE, (x, 0, h - 30), 12.5, 10, steps=7)                      # a harbour-blue band
    for k in range(3):                                                       # the rope, two sagging lengths a gap
        a, b = (xs[k], -13, hs[k] - 16), (xs[k + 1], -13, hs[k + 1] - 16)
        mid = ((a[0] + b[0]) / 2.0, -13, min(a[2], b[2]) - 22)
        tilt_box(g, CREAM, a, mid, 5.5, 5.5)
        tilt_box(g, CREAM, mid, b, 5.5, 5.5)
    g.cyl(CREAM, (180, -70, -4), 30, 16, steps=8)                           # a coil of rope
    g.cyl(WOOD, (180, -70, 10), 16, 4.5, steps=8)
    g.sphere(WHITE, (-180, -60, 12), 18, steps=(3, 6))                      # a net float
    g.box(TEAL, (-180, -60, 12), (37, 37, 7))
    bird_s(sc, xs[0], 0, hs[0], yaw=20, turn=38, rate=0.16, phase=0.1, **PELICAN)
    bird_s(sc, xs[1], 0, hs[1], yaw=-15, flap=24, rate=0.5, phase=0.5, **PELICAN)        # drying its wings
    bird_s(sc, xs[2], 0, hs[2], yaw=160, peck=14, rate=0.3, phase=0.8, **PELICAN)
    bird_s(sc, 330, -95, 0, yaw=-40, turn=30, bob=4, rate=0.4, phase=0.3, **PELICAN)


PIPER = dict(body=WHITE, wing=WOOD, beak=DWOOD, legs=GOLD, bl=28.0, bh=17.0, bw=15.0, leg=13.0, neck=6.0, neck_up=55.0, head=6.5,
             beak_len=13.0, beak_w=2.6, tail=6.0, feet=False, s=1.7)


def sandpipers(sc):
    """Seven sandpipers on the wet sand: four run the beach one way and the other and come round, three stop to probe."""
    g = sc.still()
    for k, (x, y) in enumerate(((-520, 150), (-140, -120), (260, 170), (610, -90))):          # shells left by the tide
        g.sphere(PINK if k % 2 else WHITE, (x, y, 5), 13, scale=(1, 1, 0.5), steps=(3, 6))
    r = random.Random(11)
    for k, (x, y) in enumerate(((-300, -40), (40, 20))):
        bird_s(sc, x, y, 0, yaw=90, stride=30, bob=2, rate=1.9, phase=r.random(), travel=(2400, 240), **PIPER)
    for k, (x, y) in enumerate(((250, 110), (-120, 150))):
        bird_s(sc, x, y, 0, yaw=-90, stride=30, bob=2, rate=1.7, phase=r.random(), travel=(2400, -190), **PIPER)
    for k, (x, y, yaw) in enumerate(((-420, 60, 30), (160, -80, -60), (520, 60, 200))):
        bird_s(sc, x, y, 0, yaw=yaw, peck=26, rate=0.9 + 0.2 * k, phase=r.random(), **PIPER)


def crab(sc, x, y, yaw, s, col, claw, mode, phase, rate):
    """A chunky crab, its front to -Y. 'scuttle': the whole crab runs sideways on an arc and back; 'wave': it lifts its claws."""
    if mode == 'scuttle':
        cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        f = Fr(sc, x, y, 0, yaw, s, phase=phase, rate=rate, bob=3.0, sway=dict(swing=13.0, pivot=(x - 520 * sy, y + 520 * cy, 0), axis=P.UP))
    else:
        f = Fr(sc, x, y, 0, yaw, s, phase=phase, rate=rate, bob=2.0)
    t4 = 4.3 / s
    mid = (0, 0, 15)
    b = f.part(pivot=mid)
    b.sphere(col, (0, 0, 16), 1.0, scale=(25, 17, 10), steps=(4, 6))
    for sd in (-1, 1):
        b.box(col, (sd * 8, -14, 29), (t4, t4, 12))                           # eye stalks
        b.box(WHITE, (sd * 8, -14, 37), (6, 6, 6))
        for yk in (-4, 6, 16):
            tilt_box(b, col, (sd * 20, yk, 13), (sd * 39, yk * 1.4, 2), t4, t4)
        m = b if mode == 'scuttle' else f.part(sd * 30.0, (sd * 20, -10, 16), 90.0)
        tilt_box(m, col, (sd * 20, -10, 16), (sd * 36, -24, 26), 7, 7)
        m.box(claw, (sd * 40, -29, 31), (13, 11, 10))


def crabs(sc):
    """Five red and coral crabs round a sandcastle: three scuttle sideways and back, two wave their claws."""
    g = sc.still()
    g.sphere(DSAND, (0, 60, 0), 1.0, scale=(150, 110, 16), steps=(3, 8))     # a low mound of sand
    g.box(CREAM, (10, 70, 30), (64, 64, 34))                                 # the sandcastle
    g.cyl(CREAM, (10, 70, 46), 20, 34, steps=6, top_r=16)
    for sx, sy2 in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
        g.cyl(SAND, (10 + sx * 30, 70 + sy2 * 30, 14), 11, 46, steps=5)
    g.cyl(WOOD, (10, 70, 78), 2.3, 44, steps=5)
    fl = sc.part(P.motion(swing=18, pivot=(10, 70, 112), axis=P.UP, rate=0.5, phase=0.2))
    fl.box(TURQ, (24, 70, 112), (26, 4.5, 16))                               # its paper pennant
    g = sc.still()
    g.sphere(WHITE, (-150, -30, 6), 14, scale=(1, 1, 0.5), steps=(3, 6))
    g.sphere(PINK, (170, 20, 6), 12, scale=(1, 1, 0.5), steps=(3, 6))
    crab(sc, -110, -90, 10, 1.35, RED, CORAL, 'scuttle', 0.0, 0.33)
    crab(sc, 120, -60, -20, 1.2, CORAL, RED, 'scuttle', 0.45, 0.4)
    crab(sc, 230, 130, 160, 1.0, RED, CORAL, 'scuttle', 0.7, 0.5)
    crab(sc, -40, -20, 0, 1.3, CORAL, RED, 'wave', 0.2, 0.9)
    crab(sc, -220, 80, 50, 1.1, RED, CORAL, 'wave', 0.6, 1.1)


MACAW = dict(bl=30.0, bh=22.0, bw=18.0, leg=5.0, neck=6.0, neck_up=70.0, head=9.0, beak_len=7.0, beak_w=6.5, tail=34.0, tail_drop=27.0,
             beak=CREAM, legs=STEEL, feet=False, s=1.3)


def macaw_perch(sc):
    """A T-perch on a round base with a seed cup and a hanging swing: three macaws, red, turquoise and yellow."""
    g = sc.still()
    g.cyl(CREAM, (0, 0, -100), 44, 110, steps=10)
    g.cyl(HOT, (0, 0, 10), 36, 5, steps=10)
    g.cyl(WOOD, (0, 0, 10), 6.5, 108, steps=6)
    g.box(WOOD, (0, 0, 118), (200, 7, 7))
    g.cyl(TEAL, (-96, 0, 121), 10, 11, steps=7)                              # the seed cup
    g.cyl(YEL, (-96, 0, 130), 7.5, 4.5, steps=7)
    g.sphere(GOLD, (0, 0, 126), 8, steps=(3, 6))
    sway = dict(swing=13.0, pivot=(64, 0, 118), axis=0.0)
    sw = sc.part(P.motion(rate=0.45, phase=0.3, **sway))                     # the swing: two cords and a bar
    for dx in (-17, 17):
        sw.box(CREAM, (64 + dx, 0, 86), (4.5, 4.5, 64))
    sw.box(WOOD, (64, 0, 52), (46, 6, 6))
    bird_s(sc, -52, 0, 121.5, yaw=10, body=RED, wing=BLUE, tail_col=RED, turn=30, bob=2.5, rate=0.5, phase=0.1, **MACAW)
    bird_s(sc, -14, 0, 121.5, yaw=-14, body=TURQ, wing=YEL, tail_col=BLUE, flap=30, rate=0.8, phase=0.6, **MACAW)
    bird_s(sc, 64, 0, 55, yaw=6, body=YEL, wing=RED, tail_col=TURQ, rate=0.45, phase=0.3, sway=sway, **MACAW)


CAT = dict(length=42.0, height=27.0, girth=15.0, neck=8.0, neck_up=42.0, head=15.0, tail=30.0, s=1.45)


def pot(g, x, y, z, band):
    """A lobster pot: a white slatted box with a coloured band and a rope handle."""
    g.box(WHITE, (x, y, z + 22), (80, 56, 44))
    g.box(band, (x, y, z + 22), (84, 60, 9))
    g.box(band, (x, y, z + 43), (30, 60, 5))
    for dx in (-26, 0, 26):
        g.box(NAVY, (x + dx, y - 28.5, z + 22), (8, 4.5, 30))               # the gaps between the slats


def cats_pots(sc):
    """Lobster pots stacked on the quay, a fish box and a coil of rope; three cats: one on top with a swishing tail,
    one washing on the box, one stalking round the bottom."""
    g = sc.still()
    pot(g, -50, 0, 0, BLUE)
    pot(g, 40, 6, 0, TEAL)
    pot(g, -8, 2, 44, SKY)
    g.box(WHITE, (150, -10, 16), (76, 50, 32))                               # a fish box: white with a teal band
    g.box(TEAL, (150, -10, 26), (80, 54, 7))
    g.cyl(CREAM, (-150, -30, 0), 30, 15, steps=8)
    g.cyl(WOOD, (-150, -30, 12), 15, 4.5, steps=8)
    for k, (x, y) in enumerate(((95, -60), (-95, -62), (118, -74))):
        g.sphere(WHITE if k != 1 else TEAL, (x, y, 11), 11, steps=(3, 6))    # net floats
    P.quadruped(sc, -8, 2, 88, yaw=35, body=CREAM, ears=LWOOD, tail_col=LWOOD, snout=WHITE, wag=38, rate=0.6, phase=0.2, **CAT)
    P.quadruped(sc, 150, -10, 32, yaw=-25, body=ROCK, ears=DROCK, belly=WHITE, snout=WHITE, graze=13, wag=14, rate=0.3, phase=0.6, **CAT)
    P.quadruped(sc, -40, -85, 0, yaw=110, body=LWOOD, ears=DWOOD, belly=CREAM, snout=CREAM, graze=8, wag=26, rate=0.9, phase=0.9, **CAT)


PEA = dict(bl=40.0, bh=26.0, bw=22.0, leg=20.0, neck=30.0, neck_up=78.0, head=8.0, beak_len=8.0, beak_w=5.0, tail=6.0, s=1.5)


def peacocks(sc):
    """A peacock whose fan opens and closes slowly, a peahen pecking and two chicks, by a white balustrade and an urn."""
    g = sc.still()
    g.box(WHITE, (0, 150, -40), (420, 26, 100))                              # the balustrade's plinth (footing below 0)
    g.box(WHITE, (0, 150, 82), (440, 34, 12))
    for k in range(7):
        g.cyl(WHITE, (-180 + k * 60, 150, 10), 9, 68, steps=6, top_r=6)
    g.cyl(WHITE, (-230, 150, -60), 24, 150, steps=8)                         # an end pier and its urn
    g.cyl(GOLD, (-230, 150, 90), 12, 14, steps=8, top_r=24)
    g.sphere(HOT, (-230, 150, 116), 22, steps=(3, 6))                        # hibiscus in the urn
    g.sphere(DLEAF, (-212, 150, 106), 14, steps=(3, 6))
    x, y, yaw, s = 40.0, -20.0, 8.0, PEA['s']
    bird_s(sc, x, y, 0, yaw=yaw, body=BLUE, wing=TEAL, beak=CREAM, legs=GOLD, crest=GOLD, turn=22, rate=0.2, phase=0.3, **PEA)
    f = Fr(sc, x, y, 0, yaw, s, phase=0.0, rate=0.09)                        # the fan: two halves swing apart about the rump
    rump = (0, PEA['bl'] * 0.3, PEA['leg'] + PEA['bh'] * 0.6)
    rad = 86.0
    for sd in (-1, 0, 1):
        m = f.part(sd * 25.0, rump, 90.0)
        for a in ((0.0,) if sd == 0 else (15.0, 28.0, 41.0, 54.0)):
            an = math.radians(a * sd)
            tip = (rad * math.sin(an), rump[1] + 18, rump[2] + rad * math.cos(an))
            tilt_box(m, DLEAF, rump, tip, 10, 5)
            m.box(GOLD, (tip[0] * 0.93, tip[1] - 2, rump[2] + (tip[2] - rump[2]) * 0.93), (15, 7, 15))
            m.box(BLUE, (tip[0] * 0.93, tip[1] - 4, rump[2] + (tip[2] - rump[2]) * 0.93), (7.5, 7, 7.5))
    hen = dict(PEA)
    hen.update(s=1.3, neck=24.0, neck_up=50.0)
    bird_s(sc, -150, -60, 0, yaw=-50, body=WOOD, wing=DWOOD, beak=CREAM, legs=GOLD, crest=CREAM, tail_col=DWOOD, peck=26, rate=0.7, phase=0.5, **hen)
    chick = dict(bl=22.0, bh=16.0, bw=14.0, leg=7.0, neck=5.0, neck_up=60.0, head=7.0, beak_len=5.0, beak_w=3.0, tail=4.0, feet=False, s=1.3)
    bird_s(sc, -230, -110, 0, yaw=-80, body=CREAM, wing=WOOD, beak=YEL, bob=6, rate=1.1, phase=0.2, **chick)
    bird_s(sc, -90, -130, 0, yaw=-30, body=CREAM, wing=WOOD, beak=YEL, peck=24, rate=1.3, phase=0.7, **chick)


GOAT = dict(length=80.0, height=62.0, girth=26.0, neck=22.0, head=22.0, tail=12.0, horns=CREAM)


def goat_wall(sc):
    """A dry-stone garden wall with a step at one end and a clay jar; two goats on top, a kid hopping on the step,
    one grazing in front."""
    g = sc.still()
    g.box(ROCK, (-40, 60, -42), (600, 96, 315))                              # the wall: 1.15 m, 2 m of footing
    r = random.Random(4)
    for k in range(8):                                                       # cap stones, white and grey
        g.box(WHITE if k % 2 else DROCK, (-302 + k * 75, 60, 121), (70, 100 + r.uniform(0, 8), 12 + r.uniform(0, 5)))
    for k in range(7):                                                       # stones proud of the face
        g.box(DROCK if k % 3 else WHITE, (-290 + k * 84 + r.uniform(-10, 10), 10, 30 + r.uniform(0, 60)), (46, 8, 24))
    g.box(ROCK, (320, 60, -70), (120, 96, 260))                              # the step: 0.6 m
    g.box(WHITE, (320, 60, 63), (124, 100, 8))
    g.cyl(WHITE, (-300, -40, -20), 30, 76, steps=8, top_r=22)                # a big clay jar, white and coral
    g.cyl(CORAL, (-300, -40, 30), 31, 9, steps=8)
    for k in range(5):                                                       # a tuft on the wall for the goat
        tilt_box(g, LEAF if k % 2 else DLEAF, (-222 + k * 9, 60, 126), (-230 + k * 13, 60, 126 + 26 + (k % 2) * 8), 7, 6)
    P.quadruped(sc, -130, 60, 128, yaw=-90, s=1.15, body=WHITE, ears=CREAM, belly=CREAM, neck_up=-12, graze=12, wag=20, rate=0.4, phase=0.1, **GOAT)
    P.quadruped(sc, 130, 62, 128, yaw=8, s=1.15, body='#fff1d6', ears=LWOOD, tail_col=LWOOD, snout=WHITE, neck_up=50, graze=5, wag=30, rate=1.1, phase=0.5, **GOAT)
    P.quadruped(sc, 320, 60, 67, yaw=-70, s=0.72, body=WHITE, ears=LWOOD, neck_up=45, bob=8, wag=30, rate=0.9, phase=0.3, **GOAT)
    P.quadruped(sc, 60, -110, 0, yaw=200, s=1.05, body=LWOOD, ears=DWOOD, belly=CREAM, snout=CREAM, neck_up=-25, graze=12, wag=18, rate=0.5, phase=0.8, **GOAT)


def seal(sc, x, y, z, yaw, s, col, belly, mode, phase):
    """A seal lying with its head to -Y. 'sway': head up, swaying; 'clap': head up, clapping its flippers; 'bask':
    flat out, the tail flippers lifting; 'pup': small and bouncing."""
    rate = dict(sway=0.3, clap=1.1, bask=0.45, pup=0.9)[mode]
    f = Fr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=(5.0 if mode == 'pup' else 0.0))
    t4 = 4.3 / s
    mid = (0, 0, 20)
    b = f.part(pivot=mid)
    b.sphere(col, (0, 10, 20), 1.0, scale=(24, 58, 20), steps=(4, 6))
    b.sphere(belly, (0, -14, 15), 1.0, scale=(20, 34, 15), steps=(3, 5))
    hd = (0, -74, 24) if mode == 'bask' else (0, -64, 62)
    h = f.part(14.0, (0, -30, 20), 90.0) if mode == 'sway' else f.part(pivot=mid)
    tilt_box(h, col, (0, -34, 22), hd, 26, 24)
    h.sphere(col, hd, 15, steps=(3, 5))
    h.box(belly, (0, hd[1] - 13, hd[2] - 4), (14, 8, 10))                    # the muzzle
    h.box(EYE, (0, hd[1] - 17, hd[2] - 1), (8, 7, 7))                        # the nose
    for sd in (-1, 1):
        h.box(EYE, (sd * 10, hd[1] - 10, hd[2] + 5), (t4, t4, t4))
        m = f.part(sd * 32.0, (sd * 20, -26, 22), 90.0) if mode == 'clap' else f.part(pivot=mid)
        tilt_box(m, col, (sd * 20, -26, 20), (sd * 46, -34, 14 if mode == 'clap' else 5), 16, 6.5)
    t = f.part(22.0, (0, 62, 16), 0.0) if mode == 'bask' else f.part(pivot=mid)
    for sd in (-1, 1):
        tilt_box(t, col, (sd * 4, 62, 16), (sd * 16, 94, 22), 14, 6.5)


def seal_rock(sc):
    """A stepped rock with a tide pool and a starfish; four seals hauled out: one swaying on top, one clapping, one
    basking, a pup."""
    g = sc.still()
    g.cyl(ROCK, (0, 0, -200), 270, 262, steps=7, top_r=232)                  # three ledges: 0.62, 1.1, 1.5 m
    g.cyl(DROCK, (-50, 30, 60), 172, 50, steps=7, top_r=150, rot=(0, 25, 0))
    g.cyl(ROCK, (-95, 50, 108), 96, 44, steps=6, top_r=80, rot=(0, 50, 0))
    g.cyl(TURQ, (150, -90, 59), 52, 5, steps=7)                              # the tide pool
    for k in range(5):                                                       # a coral starfish
        a = math.radians(k * 72 + 10)
        tilt_box(g, CORAL, (-190, -100, 65), (-190 + 26 * math.cos(a), -100 + 26 * math.sin(a), 65), 9, 6)
    for x, y, z in ((60, -150, 62), (-150, -60, 112), (210, 60, 62), (-20, -110, 62)):
        g.sphere(WHITE, (x, y, z), 9, scale=(1, 1, 0.6), steps=(3, 5))       # limpets
    seal(sc, -95, 46, 152, 12, 1.25, STEEL, CREAM, 'sway', 0.1)
    seal(sc, 28, -52, 110, -28, 1.2, ROCK, WHITE, 'clap', 0.5)
    seal(sc, 150, 70, 62, 150, 1.3, STEEL, CREAM, 'bask', 0.8)
    seal(sc, -160, -150, 62, 40, 0.7, WHITE, CREAM, 'pup', 0.3)


DOG = dict(length=70.0, height=48.0, girth=20.0, neck=16.0, neck_up=35.0, head=18.0, tail=22.0)


def beach_dog(sc):
    """A white dog tearing along the beach after three white terns that keep just ahead, a puppy behind it; a brown
    dog trotting back the other way. All come round again."""
    P.quadruped(sc, 0, -40, 0, yaw=90, s=1.5, body=WHITE, ears=LWOOD, tail_col=LWOOD, snout=CREAM, stride=32, wag=30, bob=6, rate=1.8, phase=0.0,
                travel=(3000, 520), **DOG)
    P.quadruped(sc, -280, 10, 0, yaw=90, s=1.0, body=CREAM, ears=WOOD, snout=WHITE, stride=34, wag=34, bob=5, rate=1.9, phase=0.4,
                travel=(3000, 520), **DOG)
    for k, (x, y, z) in enumerate(((380, -70, 110), (470, 10, 150), (560, -40, 125))):
        P.bird(sc, x, y, z, heading=0, span=150, beak=RED, rate=1.9, flap=34, phase=0.3 * k, travel=(3000, 520))
    P.quadruped(sc, 200, 170, 0, yaw=-90, s=1.3, body=LWOOD, ears=DWOOD, belly=CREAM, snout=CREAM, stride=24, wag=20, bob=3, rate=1.3, phase=0.6,
                travel=(3000, -330), **DOG)


DOVE = dict(bl=30.0, bh=20.0, bw=18.0, leg=6.0, neck=6.0, neck_up=60.0, head=8.0, beak_len=6.0, beak_w=3.5, tail=10.0, beak=CORAL,
            legs=CORAL, feet=False, s=1.5)


def doves(sc):
    """A white bird bath and eight doves and pigeons, white and pale grey: five peck, one hops, two sit on the rim."""
    g = sc.still()
    g.cyl(WHITE, (0, 60, -100), 26, 112, steps=8)
    g.cyl(WHITE, (0, 60, 12), 13, 58, steps=8)
    g.cyl(WHITE, (0, 60, 70), 22, 16, steps=10, top_r=56)
    g.cyl(TURQ, (0, 60, 82), 50, 4.5, steps=10)
    g.cyl(GOLD, (0, 60, 8), 30, 5, steps=8)
    r = random.Random(7)
    spots = ((-150, -60, 20), (-70, -110, -40), (40, -90, 150), (130, -30, -70), (-190, 40, 80))
    for k, (x, y, yaw) in enumerate(spots):
        bird_s(sc, x, y, 0, yaw=yaw, body=WHITE if k != 2 else STEEL, wing=STEEL if k % 2 else WHITE, peck=28, rate=0.9 + 0.13 * k, phase=r.random(), **DOVE)
    bird_s(sc, 200, 60, 0, yaw=-110, body=STEEL, wing=WHITE, bob=7, rate=1.0, phase=0.4, **DOVE)
    bird_s(sc, -50, 60, 86, yaw=95, body=WHITE, wing=WHITE, turn=32, rate=0.3, phase=0.2, **DOVE)
    bird_s(sc, 48, 44, 86, yaw=-70, body=WHITE, wing=STEEL, flap=26, rate=0.8, phase=0.7, **DOVE)


EGRET = dict(body=WHITE, wing=WHITE, beak=YEL, legs=GOLD, bl=46.0, bh=26.0, bw=20.0, leg=55.0, neck=50.0, head=9.0, beak_len=22.0,
             beak_w=4.0, tail=8.0, s=1.1)


def heron_reeds(sc):
    """A reed bed round a pool: two egrets fishing (the necks dip slowly), a blue-grey heron on one leg, a wader."""
    g = sc.still()
    g.cyl(TURQ, (20, -20, -1), 150, 5.5, steps=9)
    g.cyl(SAND, (20, -20, -30), 172, 30, steps=9)
    for k, (x, y) in enumerate(((-90, -70), (120, 10), (40, -110))):
        g.cyl(LEAF, (x, y, 4), 20, 4.5, steps=6)                             # lily pads
        if k != 1:
            g.sphere(PINK, (x + 4, y, 12), 8, steps=(3, 5))
    r = random.Random(21)
    reeds(sc, r, -190, 90, 6, 60, 105, 156, 0.0)
    reeds(sc, r, 0, 140, 7, 80, 115, 160, 0.3)
    reeds(sc, r, 200, 80, 6, 60, 100, 155, 0.6)
    reeds(sc, r, -230, -80, 4, 40, 80, 130, 0.8)
    bird_s(sc, -60, 20, 0, yaw=30, neck_up=52, peck=26, rate=0.14, phase=0.1, crest=WHITE, **EGRET)
    bird_s(sc, 130, -60, 0, yaw=-70, neck_up=58, peck=22, rate=0.11, phase=0.6, **EGRET)
    heron = dict(EGRET)
    heron.update(body=SKY, wing=BLUE, s=1.2)
    bird_s(sc, 230, -10, 0, yaw=-20, neck_up=74, turn=26, rate=0.12, phase=0.4, one_leg=True, crest=BLUE, **heron)
    wader = dict(PIPER)
    wader.update(s=1.6, wing=PINK, legs=CORAL)
    bird_s(sc, -130, -130, 0, yaw=60, peck=24, bob=-2, rate=0.8, phase=0.2, **wader)


def donkey(sc):
    """A donkey with white and teal panniers at a hitching rail, a water trough and hay; her keeper pats her neck."""
    g = sc.still()
    for x in (-150, 150):
        post(g, x, 90, 96, r=8.0, cap=TEAL)
    g.box(WOOD, (0, 90, 80), (320, 9, 9))
    g.box(TEAL, (-170, -40, 18), (110, 46, 44))                              # the trough
    g.box(SKY, (-170, -40, 39), (98, 34, 5))
    g.sphere(YEL, (175, -30, 10), 1.0, scale=(52, 40, 30), steps=(3, 7))     # hay
    g.cyl(OCHRE, (235, 40, 0), 24, 40, steps=7, top_r=28)                    # a spare basket
    g.cyl(WHITE, (235, 40, 38), 22, 8, steps=7)
    L, H, G, NK, NU = 115.0, 100.0, 40.0, 36.0, 38.0
    x, y, yaw, s = 0.0, 0.0, 76.0, 1.0
    P.quadruped(sc, x, y, 0, yaw=yaw, s=s, length=L, height=H, girth=G, neck=NK, neck_up=NU, head=34, body=WOOD, belly=CREAM, snout=CREAM,
                mane=DWOOD, ears=WOOD, tail=44, graze=9, wag=28, rate=0.35, phase=0.2)
    b = q_body(sc, x, y, 0, yaw, s, H, phase=0.2, rate=0.35)
    b.box(TEAL, (0, 2, H + 3), (G + 8, 56, 6))                               # the blanket
    b.box(WHITE, (0, 2, H + 7), (G + 2, 14, 5))
    for sd in (-1, 1):
        b.box(OCHRE, (sd * (G / 2.0 + 12), 2, H - 20), (24, 46, 36))         # the panniers
        b.box(WHITE if sd > 0 else TEAL, (sd * (G / 2.0 + 12), 2, H + 2), (20, 40, 9))
        b.sphere(WHITE, (sd * (G / 2.0 + 12), -8, H + 12), 12, steps=(3, 5))
        b.sphere(TEAL if sd > 0 else WHITE, (sd * (G / 2.0 + 12), 14, H + 11), 11, steps=(3, 5))
    n1 = q_head(L, H, G, NK, NU)
    nk = q_neck(sc, x, y, 0, yaw, s, L, H, G, 9, phase=0.2, rate=0.35)
    for sd in (-1, 1):                                                       # the long ears
        tilt_box(nk, WOOD, (sd * 8, n1[1] + 2, n1[2] + 12), (sd * 15, n1[1] + 8, n1[2] + 44), 6, 9)
    P.figure(sc, 130, -62, 0, yaw=-130, top='#fffaf2', bottom='#4a7fc0', hat='sun', hat_col='#e8b26a', arm_r='fwd', detail=1, rate=0.35, phase=0.2,
             swing_r=8, axis_r='x')


def wheel(g, x, y, r):
    rod(g, GOLD, (x, y - 4, r), (x, y + 4, r), r, steps=12)
    rod(g, WHITE, (x, y - 8, r), (x, y + 8, r), r * 0.22, steps=8)
    for a in (0, 90):
        g.box(CREAM, (x, y, r), (r * 1.8, 12, 6), rot=(a, 0, 0))


def carriage_horse(sc):
    """A white carriage horse with a gold mane, a mint collar and a pink plume, standing in the shafts of an open mint
    and white carriage; a dove on the folded hood."""
    g = sc.still()
    for y in (-62, 62):
        wheel(g, -190, y, 58)
        wheel(g, -20, y, 44)
    g.box(WHITE, (-105, 0, 70), (150, 96, 14))
    g.box(MINT, (-105, 0, 95), (196, 104, 40))                               # the tub
    g.box(PINK, (-105, 0, 113), (200, 108, 7))
    g.box(WHITE, (-178, 0, 132), (40, 96, 44))                               # the back seat
    g.box(PINK, (-152, 0, 122), (34, 88, 12))
    g.box(MINT, (-34, 0, 132), (50, 90, 34))                                 # the driver's box
    g.box(PINK, (-34, 0, 152), (44, 80, 8))
    tilt_box(g, CREAM, (-8, 0, 112), (16, 0, 158), 100, 6)                   # the dash
    g.box(CREAM, (-214, 0, 142), (30, 104, 56))                              # the hood, folded
    g.box(CREAM, (-236, 0, 128), (20, 100, 40))
    g.box(GOLD, (-105, -62, 56), (34, 22, 5))                                # the step
    for y in (-48, 48):
        g.box(GOLD, (-4, y, 150), (8, 8, 26))                                # the lamps: unlit
        g.box(WHITE, (-4, y, 166), (11, 11, 9))
        rod(g, WOOD, (0, y * 0.8, 96), (175, y * 0.62, 100), 3.4, steps=5)   # the shafts
    x, y, yaw = 215.0, 0.0, 90.0
    L, H, G, NK, NU = 150.0, 120.0, 42.0, 50.0, 55.0
    P.quadruped(sc, x, y, 0, yaw=yaw, body=WHITE, belly=CREAM, mane=GOLD, ears=WHITE, snout=CREAM, tail_col=GOLD, legs_col=WHITE, graze=6, wag=24,
                rate=0.28, phase=0.4)
    b = q_body(sc, x, y, 0, yaw, 1.0, H, phase=0.4, rate=0.28)
    b.box(MINT, (0, -L / 2.0 + 16, H - G / 2.0), (G + 10, 12, G + 10))       # the collar
    b.box(PINK, (0, 6, H + 2.5), (G + 4, 40, 5))                             # the saddle pad
    b.box(GOLD, (0, 6, H - G / 2.0), (G + 5, 7, G + 5))                      # the girth
    n1 = q_head(L, H, G, NK, NU)
    nk = q_neck(sc, x, y, 0, yaw, 1.0, L, H, G, 6, phase=0.4, rate=0.28)
    nk.box(PINK, (0, n1[1] - 4, n1[2] + 26), (7, 7, 22))                     # the plume
    bird_s(sc, -214, 0, 170, yaw=20, body=WHITE, wing=WHITE, turn=30, rate=0.3, phase=0.1, **DOVE)


def tortoise(sc):
    """A giant tortoise plodding on the spot towards a dish of leaves, its head reaching; a young one bobbing behind."""
    g = sc.still()
    g.cyl(CORAL, (0, -150, 0), 34, 9, steps=8, top_r=40)
    g.sphere(LEAF, (0, -150, 13), 1.0, scale=(26, 26, 9), steps=(3, 6))
    g.sphere(ROCK, (150, 60, 6), 1.0, scale=(40, 30, 22), steps=(3, 6))
    g.sphere(DROCK, (188, 30, 2), 1.0, scale=(22, 18, 14), steps=(3, 5))
    for x, y, yaw, s, ph in ((0, 0, 4, 1.4, 0.0), (-150, 90, -30, 0.75, 0.5)):
        f = Fr(sc, x, y, 0, yaw, s, phase=ph, rate=0.4 / s, bob=1.5)
        mid = (0, 0, 30)
        b = f.part(pivot=mid)
        b.sphere(GOLD, (0, 0, 22), 1.0, scale=(38, 48, 34), steps=(4, 7))    # the shell
        b.sphere(DWOOD, (0, 0, 14), 1.0, scale=(42, 52, 9), steps=(3, 7))    # its rim
        for dx, dy in ((0, -18), (0, 14), (-16, -2), (16, -2)):
            b.box(DWOOD, (dx, dy, 52 if dx == 0 else 46), (15, 15, 7))       # scutes
        tilt_box(b, CREAM, (0, 46, 14), (0, 62, 8), 7, 6)                    # the tail
        h = f.part(16.0, (0, -40, 18), P.UP, rate=0.15)                      # the head looks about
        tilt_box(h, CREAM, (0, -40, 18), (0, -62, 30), 13, 12)
        h.sphere(CREAM, (0, -66, 33), 12, steps=(3, 5))
        for sd in (-1, 1):
            h.box(EYE, (sd * 10, -70, 36), (4.3 / s, 4.3 / s, 4.3 / s))
            for k, ly in enumerate((-28, 30)):
                m = f.part((1 if k == 0 else -1) * sd * 12.0, (sd * 32, ly, 16), 0.0)
                m.box(CREAM, (sd * 34, ly, 8), (15, 16, 18))


def swan(sc, ang, rad, z, s, col, beak, rate, phase, c=(0.0, 0.0)):
    """A swan (or a cygnet) circling the point c on the water, anticlockwise."""
    a = math.radians(ang)
    x, y = c[0] + rad * math.cos(a), c[1] + rad * math.sin(a)
    f = Fr(sc, x, y, z, ang + 180.0, s, phase=phase, rate=0.3, bob=-1.5, orbit=(c[0], c[1], rate))
    t4 = 4.3 / s
    b = f.part(pivot=(0, 0, 12))
    b.sphere(col, (0, 0, 12), 1.0, scale=(15, 28, 12), steps=(4, 6))
    for sd in (-1, 1):
        b.box(col, (sd * 11, 6, 22), (max(6, t4), 30, 14))                   # the raised wings
    tilt_box(b, col, (0, 24, 16), (0, 38, 27), 10, max(5, t4))
    tilt_box(b, col, (0, -20, 18), (0, -27, 52), max(7, t4), max(7, t4))     # the neck
    tilt_box(b, col, (0, -25, 53), (0, -40, 49), 9, 9)
    tilt_box(b, beak, (0, -40, 49), (0, -51, 46), max(5, t4), max(5, t4))


def swan_pool(sc):
    """A round white fountain pool with lily pads and a small jet; two swans and three cygnets circle on it."""
    g = sc.still()
    g.cyl(WHITE, (0, 0, -120), 235, 160, steps=12)
    g.cyl(TURQ, (0, 0, 37), 208, 5, steps=12)
    g.cyl(WHITE, (0, 0, 0), 32, 82, steps=8, top_r=18)
    g.cyl(WHITE, (0, 0, 82), 26, 14, steps=8, top_r=58)
    g.cyl(GOLD, (0, 0, 96), 10, 16, steps=6)
    for k, a in enumerate((20, 150, 265)):
        x, y = 182 * math.cos(math.radians(a)), 182 * math.sin(math.radians(a))
        g.cyl(LEAF, (x, y, 41), 20, 4.5, steps=6)
        if k != 1:
            g.sphere(PINK, (x, y, 50), 8, steps=(3, 5))
    jet = sc.part(P.motion(bob=-7, rate=0.7, phase=0.0, pivot=(0, 0, 110)))
    jet.cyl(SKY, (0, 0, 106), 5, 46, steps=5)
    jet.sphere(WHITE, (0, 0, 154), 11, steps=(3, 5))
    swan(sc, 0, 125, 42, 1.5, WHITE, CORAL, 0.035, 0.0)
    swan(sc, 172, 120, 42, 1.4, WHITE, CORAL, 0.035, 0.5)
    for k, a in enumerate((196, 214, 232)):
        swan(sc, a, 112 + 14 * (k % 2), 42, 0.7, STEEL, DROCK, 0.035, 0.2 * k)


# ------------------------------------------------------------------------------------------ the water (far=True)
def turtle(sc, ang, rad, s, rate, phase):
    a = math.radians(ang)
    x, y = rad * math.cos(a), rad * math.sin(a)
    f = Fr(sc, x, y, 0, ang + 180.0, s, phase=phase, rate=0.45, bob=-5.0, orbit=(0.0, 0.0, rate))
    mid = (0, 0, 6)
    b = f.part(pivot=mid)
    b.sphere(TEAL, (0, 0, 4), 1.0, scale=(38, 48, 17), steps=(4, 7))         # the shell
    b.sphere(CREAM, (0, 0, 0), 1.0, scale=(43, 53, 7), steps=(3, 7))
    for dx, dy in ((0, -20), (0, 14), (-17, -3), (17, -3)):
        b.box(GOLD, (dx, dy, 19 if dx == 0 else 15), (16, 16, 6))            # gold scutes
    tilt_box(b, MINT, (0, -44, 4), (0, -62, 10), 15, 12)
    b.sphere(MINT, (0, -68, 12), 13, steps=(3, 5))
    for sd in (-1, 1):
        b.box(EYE, (sd * 11, -72, 15), (5, 5, 5))
        tilt_box(b, MINT, (sd * 24, 36, 2), (sd * 46, 64, 0), 15, 6)
        m = f.part(sd * 22.0, (sd * 30, -30, 2), 90.0)                       # the fore flippers paddle
        tilt_box(m, MINT, (sd * 30, -30, 2), (sd * 76, -42, 0), 21, 6)


def turtles(sc):
    """Three sea turtles paddling a slow ring round a raft of sea grass, shells of teal and gold just out of the water."""
    w = sc.part(P.motion(bob=-4, rate=0.25, phase=0.3, pivot=(0, 0, 0)))
    for k in range(6):
        a = math.radians(k * 60 + 15)
        tilt_box(w, LEAF if k % 2 else DLEAF, (0, 0, 2), (70 * math.cos(a), 70 * math.sin(a), 2), 16, 5)
    turtle(sc, 0, 380, 1.5, 0.022, 0.0)
    turtle(sc, 130, 330, 1.2, 0.022, 0.4)
    turtle(sc, 235, 420, 0.9, 0.022, 0.7)


def flying_fish(sc):
    """A shoal of eight flying fish skipping along the water: each leaps in long arcs along the piece and comes round."""
    r = random.Random(31)
    for k in range(8):
        x, y = -420 + k * 120 + r.uniform(-30, 30), r.uniform(-320, 320)
        s = 1.5 + 0.3 * (k % 3)
        f = Fr(sc, x, y, -44.0, 90.0, s, phase=r.random(), rate=0.34 + 0.02 * (k % 4), bob=120.0 + 12 * (k % 3), travel=(3000, 480))
        b = f.part(pivot=(0, 0, 0))
        b.sphere(STEEL, (0, 0, 0), 1.0, scale=(8, 30, 11), steps=(4, 6))
        b.box(BLUE, (0, 2, 9), (7, 40, 5))                                   # the blue back
        tilt_box(b, BLUE, (0, 26, 0), (0, 46, 13), 4.3 / s, 9)               # the forked tail
        tilt_box(b, BLUE, (0, 26, 0), (0, 44, -11), 4.3 / s, 8)
        for sd in (-1, 1):
            tilt_box(b, SKY, (sd * 6, -8, 2), (sd * 46, 6, 11), 17, 4.3 / s) # the wings
            b.box(EYE, (sd * 6.5, -20, 3), (4.3 / s, 4.3 / s, 4.3 / s))


def whale_at(sc, x, y, s, phase, rate, calf=False):
    """A round blue whale heading +X, half out of the water: she pitches slowly, the tail lifts and falls, she blows."""
    f = Fr(sc, x, y, 0.0, 90.0, s, phase=phase, rate=rate)
    piv = (0, 0, -200)
    b = f.part(7.0, piv, 0.0)
    b.sphere(BLUE, (0, 0, 0), 1.0, scale=(250, 880, 230), steps=(6, 10))
    b.sphere(BLUE, (0, -520, 10), 1.0, scale=(238, 420, 215), steps=(5, 8))  # the blunt brow
    b.sphere(CREAM, (0, -120, -45), 1.0, scale=(264, 780, 150), steps=(4, 8))                  # the pale belly at the waterline
    for sd in (-1, 1):
        b.sphere(WHITE, (sd * 205, -640, 60), 30, steps=(3, 6))              # the eyes
        b.sphere(EYE, (sd * 226, -648, 60), 15, steps=(3, 5))
        tilt_box(b, NAVY, (sd * 150, -905, 20), (sd * 236, -700, 0), 10, 10) # the smile
        if not calf:
            tilt_box(b, BLUE, (sd * 235, -300, 0), (sd * 430, -250, 250 if sd > 0 else 60), 160, 30)   # the fins: one waves
    tilt_box(b, BLUE, (0, 430, 205), (0, 520, 265), 24, 80)                  # the small back fin
    if not calf:
        for dx, dy in ((40, -640), (-70, -560), (10, -480), (90, -430)):
            b.sphere(CREAM, (dx, dy, 218), 20, scale=(1, 1, 0.5), steps=(3, 5))                # barnacles on the brow
    t = f.part(13.0, piv, 0.0)                                               # the tail: the same beat, further
    tilt_box(t, BLUE, (0, 740, 20), (0, 1100, 270), 130, 120)
    for sd in (-1, 1):
        tilt_box(t, BLUE, (0, 1090, 268), (sd * 270, 1190, 262), 160, 30)
    sp = f.part(pivot=(0, -380, 0), bob=300.0 * s)            # the blow: it rises out of the back and sinks
    sp.cyl(WHITE, (0, -380, -140), 24, 200, steps=6)
    sp.sphere(WHITE, (0, -380, 50), 56, steps=(3, 6))
    for sd in (-1, 1):
        sp.sphere(SKY, (sd * 62, -380, 20), 40, steps=(3, 5))


def whale(sc):
    """The one great animal: a blue whale and her calf, far out. She rolls, lifts her flukes and blows."""
    whale_at(sc, 0, 60, 1.0, 0.0, 0.07)
    whale_at(sc, -250, -620, 0.42, 0.35, 0.1, calf=True)


# ------------------------------------------------------------------------------------------ the sky (far=True)
def spoonbills(sc):
    """Seven pink spoonbills flying the road's way in a slanting line; they come round again."""
    for k in range(7):
        P.bird(sc, 450 - k * 150, -260 + k * 85, (k % 3) * 45, heading=0, span=240, body=PINK, wing=HOT, beak=CREAM, rate=1.1 + 0.03 * (k % 3),
               flap=30, phase=0.13 * k, travel=(4000, 700))


def tern_ring(sc):
    """Eight white terns wheeling slowly on a wide ring (18 m, one round in 36 s), at four heights."""
    r = random.Random(17)
    for k in range(8):
        ang = k * 45 + r.uniform(-9, 9)
        rad = 1850 + r.uniform(-90, 0)
        a = math.radians(ang)
        P.bird(sc, rad * math.cos(a), rad * math.sin(a), (k % 4) * 110, heading=ang + 90, span=250, body=WHITE, wing=STEEL, beak=RED, rate=1.5,
               flap=26, phase=r.random(), orbit=(0, 0, 0.028))


LOW = dict(grow=(30, 30, 20), grow_down=(30, 30, 0))                         # the parts stay close: bounds under 2.5 m
JOBS = [
    ('SM_S1ANIMALS_PelicanPosts', pelican_posts, LOW),
    ('SM_S1ANIMALS_Sandpipers', sandpipers, dict(grow=(1250, 20, 20), grow_down=(1250, 20, 0))),
    ('SM_S1ANIMALS_Crabs', crabs, dict(grow=(140, 60, 15), grow_down=(140, 60, 0))),
    ('SM_S1ANIMALS_MacawPerch', macaw_perch, LOW),
    ('SM_S1ANIMALS_CatsPots', cats_pots, LOW),
    ('SM_S1ANIMALS_Peacocks', peacocks, dict(grow=(70, 30, 15), grow_down=(70, 30, 0))),
    ('SM_S1ANIMALS_GoatWall', goat_wall, LOW),
    ('SM_S1ANIMALS_SealRock', seal_rock, LOW),
    ('SM_S1ANIMALS_BeachDog', beach_dog, dict(grow=(1250, 20, 30), grow_down=(1250, 20, 0))),
    ('SM_S1ANIMALS_Doves', doves, LOW),
    ('SM_S1ANIMALS_HeronReeds', heron_reeds, LOW),
    ('SM_S1ANIMALS_Donkey', donkey, LOW),
    ('SM_S1ANIMALS_CarriageHorse', carriage_horse, LOW),
    ('SM_S1ANIMALS_Tortoise', tortoise, LOW),
    ('SM_S1ANIMALS_SwanPool', swan_pool, LOW),
    ('SM_S1ANIMALS_Turtles', turtles, dict(far=True, grow=(120, 120, 20), grow_down=(120, 120, 20))),
    ('SM_S1ANIMALS_FlyingFish', flying_fish, dict(far=True, grow=(1250, 30, 260), grow_down=(1250, 30, 20))),
    ('SM_S1ANIMALS_Whale', whale, dict(far=True, grow=(150, 80, 340), grow_down=(150, 80, 60))),
    ('SM_S1ANIMALS_Spoonbills', spoonbills, dict(far=True, grow=(1650, 40, 60), grow_down=(1650, 40, 60))),
    ('SM_S1ANIMALS_TernRing', tern_ring, dict(far=True, grow=(100, 100, 80), grow_down=(100, 100, 60))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; NEVER build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
