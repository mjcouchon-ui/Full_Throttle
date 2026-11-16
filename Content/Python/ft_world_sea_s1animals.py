"""Under the Sea, Stage 1, builder s1animals: the animals of the eight districts. /Game/World/UnderTheSea/SM_S1ANIMALS_*.
Run: import ft_world_sea_s1animals as K; K.run()

Sixteen pieces, each one small scene that tells which district it stands in. On this level the sand lies under the
road's level behind the barrier, so every ground piece brings its own table of the level's rose rock (1.1 to 1.3 m)
and its animals are drawn big:
  ground  HermitCrabs (three painted shells parading: the reef, the Pearl Quarter), Starfish (five stars turning and
          creeping on a ledge: the reef, the Pearl Quarter, the Crown), SeaSlugs (two frilled slugs: the kelp forest,
          the Crown), Flounders (three flat fish on a sand bar, one lifts and settles: the research station, Lamp Row),
          Shrimps (a cleaning station, a pink client fish: the Bubble Works, Lamp Row), SeaPigs (a herd of pink sea
          pigs, P.quadruped: the salvage field, the Bubble Works), PigKeeper (the herd's keeper with a crook, a pig
          walking round her, a hand sledge of dried weed: the approach, the Bubble Works), SpiderCrabs (two decorated
          crabs on long legs over ship's plates and pipes: the salvage field).
  water   ParrotFish (four, pink and green, grazing a coral head: the reef, the Pearl Quarter, the Crown), Cuttlefish
          (a striped pair, skirts rippling: the kelp forest, the Bubble Works), Puffers (four spotted, one blown up:
          the research station, the Crown), Lionfish (three, red and white fans: Lamp Row), Seals (three looping round
          a banded post, 3.5 to 8.5 m up: the Pearl Quarter, Lamp Row, the Crown).
  far     BaitBall (a silver ball of small fish, seething: the kelp forest, the salvage field), Sunfish (one huge pale
          disc drifting with three pink followers: the Pearl Quarter, Lamp Row), SeaAngels (seven pale winged
          swimmers, P.bird, on a STRAIGHT line along the road: the Bubble Works, the Crown).

Rules kept here: no shark, no fin standing out of a back (backs carry a low ridge or a comb of short spines; the
sunfish's two paddles are round), no open jaw, no blue fish, no long-billed or torpedo fish, nothing that leaps;
nothing orange, no green or mint box; no glow at all; nothing dark in the water overhead. What walks or drifts goes
along the piece's X (the road's own direction): no path can cross the road. Only the seals and the bait ball circle,
round their own middle (4.2 m and 5 m), and stand 72 m and 100 m or more out.
Colours: ft_dress_abyss.py's and the zone plan's; '#c9c4d6' (silver grey) is ft_people's own wing grey.
Movement only by ft_people's motion values (the vertex shader).
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/UnderTheSea'
SAND, SAND_D = '#d9c89a', '#b9a77e'
ROCK, ROCK_D, ROCK_L = '#9a6a7e', '#6a4562', '#c79aa2'
CORAL, MAG, LAV, PINK, CREAM = '#ff7a6a', '#ff4fb8', '#b9a4ff', '#ff9ec7', '#fff1c0'
WHITE, BONE, GREY, YEL, STRAW = '#e6eef2', '#f4efe0', '#c9c4d6', '#ffd23f', '#c9b44a'
RUST, RUST_D, HULL, TEAL, WOOD_D = '#c8683a', '#7a3f2a', '#8f6450', '#3f8a9a', '#6a4c36'
GRASS, KELP_L, DARK, RED, PLUM = '#3fae7a', '#7fcf6a', '#3a3140', '#ff3355', '#6a4562'
UP = P.UP


# ------------------------------------------------------------------------------------------ helpers
class Cr(object):
    """A creature's own frame: +X is its nose, drawn at (x, y, z), turned by yaw, sized by s. Every part() of it
    shares phase, rate, bob and the travel or the orbit, so the parts stay together.
    travel: (length cm, speed cm/s) along the MESH's X (yaw 0 with a speed > 0, yaw 180 with a speed < 0);
    orbit: (centre x, centre y, rounds a second)."""

    def __init__(self, sc, x, y, z, yaw=0.0, s=1.0, phase=0.0, rate=0.0, bob=0.0, travel=None, orbit=None):
        self.sc, self.o, self.yaw, self.s = sc, (x, y, z), yaw, s
        self.cy, self.sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        tv, base = (0.0, 0.0), (x, y)
        if travel:
            tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
        elif orbit:
            base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
        self.common = dict(phase=phase, rate=rate, bob=bob, base=base, travel=tv)

    def W(self, p):
        x, y, z = self.o
        s = self.s
        return (x + s * (p[0] * self.cy - p[1] * self.sy), y + s * (p[0] * self.sy + p[1] * self.cy), z + s * p[2])

    def part(self, swing=0.0, pivot=(0.0, 0.0, 0.0), axis=0.0, phase=None):
        """The model of the parts that swing `swing` degrees about `pivot` (the creature's frame) round `axis`
        (degrees from its nose in the ground plane: 0 = a roll, 90 = a nod; or UP = a wag)."""
        kw = dict(self.common)
        if phase is not None:
            kw['phase'] = phase
        mo = P.motion(swing=swing, pivot=self.W(pivot), axis=(UP if axis == UP else self.yaw + axis), **kw)
        return self.sc.part(mo, self.o, self.yaw, self.s)


def table(g, r, w, d, h, top=ROCK_L, n=5, foot=150.0):
    """The piece's own ground: a table of the level's rose rock, lumps side by side, `foot` cm of footing. Returns
    the height of its top."""
    for k in range(n):
        x = (k + 0.5 - n / 2.0) * w / n + r.uniform(-18, 18)
        ww, dd = w / n * r.uniform(1.25, 1.6), d * r.uniform(0.86, 1.0)
        hh = h * (1.0 if k % 2 == 0 else r.uniform(0.8, 0.93))
        g.box(ROCK if k % 2 else ROCK_D, (x, r.uniform(-10, 10), (hh - foot) / 2.0), (ww, dd, hh + foot), rot=(0, r.uniform(-7, 7), 0))
    g.box(top, (0, 0, h - 4), (w * 0.9, d * 0.78, 10))
    return h + 1.0


def sprig(g, x, y, z, col, h):
    """A branch of coral: a stem and two arms."""
    g.cyl(col, (x, y, z - 6), h * 0.15, h, steps=5, top_r=h * 0.08)
    rod(g, col, (x, y, z + h * 0.4), (x + h * 0.42, y, z + h * 0.92), max(h * 0.08, 2.5), steps=5)
    rod(g, col, (x, y, z + h * 0.55), (x - h * 0.36, y + h * 0.1, z + h * 1.02), max(h * 0.07, 2.5), steps=5)


def tuft(sc, x, y, z, col, h, r, n=3):
    """Weed that sways about its foot."""
    m = sc.part(P.motion(swing=r.uniform(5, 9), pivot=(x, y, z), axis=r.uniform(0, 180), rate=r.uniform(0.25, 0.4), phase=r.random()))
    for k in range(n):
        dx = (k - (n - 1) / 2.0) * h * 0.2
        tilt_box(m, col, (x + dx, y, z - 4), (x + dx * 2.2, y + r.uniform(-6, 6), z + h * r.uniform(0.75, 1.0)), max(h * 0.12, 5.0), 4.5)


def fish(sc, x, y, z, yaw=0.0, s=1.0, L=110.0, H=52.0, W=30.0, body=PINK, belly=None, fin=None, tail=None, lips=None, bands=None,
         wag=22.0, flap=24.0, nod=0.0, **mv):
    """A chunky round fish, nose to +X: a body, a low ridge along the back (never a fin standing up), two fins that
    paddle, a tail that wags. nod: instead, the whole fish pecks about its mouth (a grazer). bands: ((place along the
    body -0.4..0.4, colour), ...). mv: Cr's phase, rate, bob, travel, orbit."""
    c = Cr(sc, x, y, z, yaw, s, **mv)
    fin, tail = fin or body, tail or fin or body

    def body_part():
        return c.part(nod, (L * 0.5, 0, 0), 90.0) if nod else c.part()
    b = body_part()
    b.sphere(body, (0, 0, 0), 50.0, scale=(L / 100.0, W / 100.0, H / 100.0), steps=(4, 8))
    if belly:
        b.sphere(belly, (L * 0.04, 0, -H * 0.13), 50.0, scale=(L * 0.8 / 100.0, W * 0.94 / 100.0, H * 0.8 / 100.0), steps=(3, 6))
    b.box(fin, (-L * 0.05, 0, H * 0.48), (L * 0.42, max(4.5, W * 0.14), max(4.5, H * 0.13)))
    for sd in (-1, 1):
        b.box(BONE, (L * 0.3, sd * W * 0.38, H * 0.12), (max(5.0, L * 0.09), 5, max(5.0, H * 0.2)))
        b.box(DARK, (L * 0.32, sd * (W * 0.38 + 2.6), H * 0.12), (max(4.0, L * 0.05), 4, max(4.0, H * 0.11)))
    if lips:
        b.box(lips, (L * 0.48, 0, -H * 0.05), (L * 0.07, W * 0.42, H * 0.2))
    for at, col in (bands or ()):
        f = math.sqrt(max(0.05, 1.0 - (2.0 * at) ** 2))
        b.box(col, (at * L, 0, 0), (L * 0.07, W * f + 2.5, H * f + 2.5))
    t = body_part() if nod else c.part(wag, (-L * 0.42, 0, 0), UP)
    tilt_box(t, tail, (-L * 0.4, 0, 0), (-L * 0.68, 0, H * 0.34), max(4.5, W * 0.16), H * 0.24)
    tilt_box(t, tail, (-L * 0.4, 0, 0), (-L * 0.68, 0, -H * 0.34), max(4.5, W * 0.16), H * 0.24)
    for sd in (-1, 1):
        p = body_part() if nod else c.part(sd * flap, (L * 0.12, sd * W * 0.42, -H * 0.12), 0.0)
        tilt_box(p, fin, (L * 0.12, sd * W * 0.42, -H * 0.12), (-L * 0.1, sd * (W * 0.42 + L * 0.18), -H * 0.3), L * 0.14, 4.5)
    return c


# ------------------------------------------------------------------------------------------ ground
def _hermit(sc, x, y, z, s, shell, stripe, phase, travel):
    """A hermit crab under a painted spiral shell, walking +X: claws that open, four legs that step, eyes on stalks."""
    c = Cr(sc, x, y, z, 0.0, s, phase=phase, rate=0.9, bob=3.0, travel=travel)
    b = c.part()
    b.sphere(shell, (-22, 0, 62), 50.0, scale=(1.1, 1.0, 1.0), steps=(4, 8))
    b.sphere(stripe, (-22, 0, 62), 50.0, scale=(1.15, 0.3, 1.05), steps=(3, 8))
    b.sphere(shell, (-50, 0, 106), 30.0, steps=(3, 6))
    b.sphere(stripe, (-64, 0, 130), 18.0, steps=(3, 5))
    b.cyl(shell, (-68, 0, 140), 10, 22, steps=5, top_r=3)
    b.box(CORAL, (38, 0, 30), (40, 44, 26))
    for sd in (-1, 1):
        b.box(CORAL, (52, sd * 12, 52), (6, 6, 24))
        b.sphere(BONE, (52, sd * 12, 67), 7.0, steps=(3, 5))
        b.box(DARK, (58, sd * 12, 67), (5, 5, 5))
        cl = c.part(sd * 14.0, (46, sd * 22, 28), UP)
        tilt_box(cl, CORAL, (46, sd * 22, 28), (84, sd * 34, 24), 16, 14)
        cl.box(CREAM, (93, sd * 34, 24), (18, 20, 16))
        for j in range(2):
            lg = c.part((1 if j == 0 else -1) * sd * 16.0, (30 - j * 18, sd * 22, 30), 90.0)
            tilt_box(lg, CORAL, (30 - j * 18, sd * 22, 30), (34 - j * 20, sd * 52, 0), 7, 7)


def hermit_crabs(sc):
    """Three hermit crabs in painted shells parading along a ledge of rock, one behind another."""
    r = random.Random(61)
    top = table(sc.still(), r, 1060, 300, 125)
    g = sc.still()
    sprig(g, -455, 105, top, CORAL, 64); sprig(g, 420, 112, top, MAG, 52); sprig(g, 60, 122, top, LAV, 44)
    g.sphere(ROCK_L, (-250, 110, top + 8), 34.0, scale=(1.2, 1.0, 0.7), steps=(3, 6))
    for x in (-380, 150):                                 # the shells nobody took
        g.sphere(CREAM, (x, -118 + (x % 7), top + 9), 13.0, scale=(1.2, 1.0, 0.8), steps=(3, 5))
    for k, (shell, stripe, s) in enumerate(((CREAM, MAG, 1.0), (LAV, CREAM, 0.82), (PINK, PLUM, 1.14))):
        _hermit(sc, -320 + k * 320, -18 + 34 * (k % 2), top, s, shell, stripe, r.random(), (920.0, 24.0))
    tuft(sc, -500, 60, top, KELP_L, 60, r); tuft(sc, 495, -40, top, KELP_L, 50, r)


def _star(sc, r, x, y, z, R, col, dot, creep=None):
    """A sea star lying flat: it turns a little one way and back; creep = (length, cm/s): it also creeps along X."""
    kw = dict(pivot=(x, y, z), swing=r.uniform(18, 30), axis=UP, rate=r.uniform(0.05, 0.09), phase=r.random())
    if creep:
        kw.update(base=(x, y), travel=(float(creep[0]), float(creep[1]) / float(creep[0])))
    m = sc.part(P.motion(**kw))
    m.cyl(col, (x, y, z), R * 0.36, 13, steps=5)
    a0 = r.uniform(0, 72)
    for a in range(5):
        an = math.radians(a0 + a * 72)
        ca, sa = math.cos(an), math.sin(an)
        tilt_box(m, col, (x + ca * R * 0.2, y + sa * R * 0.2, z + 7), (x + ca * R, y + sa * R, z + 5), R * 0.3, 10)
        m.box(dot, (x + ca * R * 0.55, y + sa * R * 0.55, z + 11.5), (R * 0.15, R * 0.15, 5), rot=(0, a0 + a * 72, 0))
    m.box(dot, (x, y, z + 14), (R * 0.2, R * 0.2, 5))


def starfish(sc):
    """Five big sea stars on a ledge: three turning slowly where they lie, two creeping along its front."""
    r = random.Random(62)
    top = table(sc.still(), r, 620, 320, 115)
    g = sc.still()
    g.sphere(ROCK_L, (-205, 100, top + 14), 46.0, scale=(1.15, 1.0, 0.75), steps=(4, 7))        # a brain coral
    g.sphere(CREAM, (-205, 100, top + 20), 46.0, scale=(1.0, 0.25, 0.72), steps=(3, 6))
    for k, h in enumerate((54, 70, 46)):                                                         # tube sponges
        g.cyl(LAV, (190 + k * 30, 112 - k * 12, top - 4), 13, h, steps=6, top_r=16)
    sprig(g, 30, 128, top, CORAL, 48)
    _star(sc, r, -150, -10, top, 60, MAG, CREAM)
    _star(sc, r, 60, 50, top, 52, YEL, RUST_D)
    _star(sc, r, 215, 0, top, 46, LAV, BONE)
    _star(sc, r, -60, -92, top, 50, PINK, PLUM, creep=(470.0, 4.0))
    _star(sc, r, 170, -98, top, 44, CORAL, CREAM, creep=(470.0, 4.0))
    tuft(sc, -285, -20, top, KELP_L, 44, r)


def _slug(sc, x, y, z, s, body, frill, horn, dot, phase0, travel):
    """A sea slug, nose to +X: two horns, two rows of frills that ripple from head to tail, a plume of gills."""
    c = Cr(sc, x, y, z, 0.0 if travel[1] > 0 else 180.0, s, rate=0.5, travel=travel)
    b = c.part()
    b.box(frill, (0, 0, 5), (150, 76, 8))
    b.sphere(body, (0, 0, 26), 50.0, scale=(1.5, 0.62, 0.5), steps=(4, 8))
    b.sphere(body, (64, 0, 27), 30.0, scale=(1.0, 1.0, 0.8), steps=(3, 6))
    for dx, dy in ((30, 0), (-4, 9), (-36, -6), (-62, 4)):
        b.box(dot, (dx, dy, 50 - abs(dx) * 0.1), (11, 11, 5))
    h = c.part(10.0, (74, 0, 40), 90.0, phase=phase0)
    for sd in (-1, 1):
        tilt_box(h, horn, (72, sd * 12, 40), (90, sd * 19, 76), 7, 7)
    for j in range(5):
        for sd in (-1, 1):
            fr = c.part(sd * 16.0, (40 - j * 24, sd * 18, 44), 0.0, phase=phase0 + j * 0.18)
            tilt_box(fr, frill, (40 - j * 24, sd * 18, 42), (36 - j * 24, sd * 35, 78), 10, 8)
            fr.box(horn, (36 - j * 24, sd * 35, 81), (11, 11, 11))
    gl = c.part(12.0, (-58, 0, 40), 90.0, phase=phase0 + 0.5)
    for k in range(3):
        tilt_box(gl, horn, (-58, (k - 1) * 9, 40), (-72 - abs(k - 1) * 4, (k - 1) * 20, 74), 7, 7)


def sea_slugs(sc):
    """Two bright frilled sea slugs, very slow, on a ledge with ruffles of sea lettuce: one each way."""
    r = random.Random(63)
    top = table(sc.still(), r, 780, 320, 120)
    g = sc.still()
    for x, y in ((-330, 105), (-180, 122), (110, 118), (300, 100)):
        g.sphere(KELP_L, (x, y, top + 6), 30.0, scale=(1.3, 0.9, 0.5), steps=(3, 6))
    _slug(sc, -160, -52, top, 1.25, MAG, YEL, CREAM, WHITE, r.random(), (560.0, 5.0))
    _slug(sc, 170, 48, top, 1.1, BONE, LAV, PINK, MAG, r.random(), (560.0, -4.0))
    tuft(sc, -365, -30, top, KELP_L, 70, r); tuft(sc, 365, 40, top, GRASS, 56, r)


def _flounder(sc, x, y, z, yaw, s, col, spot, rim, phase, rate=0.45, bob=0.0, ripple=True, travel=None):
    """A flat fish, nose to +X, both eyes on top: its rim of fins ripples (or, when it lifts, beats together)."""
    c = Cr(sc, x, y, z, yaw, s, phase=phase, rate=rate, bob=bob, travel=travel)
    b = c.part()
    b.sphere(col, (0, 0, 10), 50.0, scale=(1.5, 0.9, 0.2), steps=(3, 8))
    b.box(rim, (-88, 0, 9), (30, 46, 6))
    for dx, dy in ((20, 14), (-10, -16), (-36, 10), (38, -12), (-56, -4)):
        b.box(spot, (dx, dy, 16.5), (12, 12, 5))
    for dy in (-9, 13):
        b.sphere(BONE, (52, dy, 22), 7.5, steps=(3, 5))
        b.box(DARK, (55, dy, 27), (5, 5, 5))
    for sd in (-1, 1):
        for j in range(5):
            xj = 50 - j * 27
            f = c.part(sd * 14.0, (xj, sd * 38, 9), 0.0, phase=(phase + j * 0.2) if ripple else None)
            f.box(rim, (xj, sd * 51, 9), (28, 26, 5))


def flounders(sc):
    """Three flounders on a sand bar: one lies and ripples, one glides along it, one lifts off the sand and settles."""
    r = random.Random(64)
    top = table(sc.still(), r, 860, 360, 110, top=SAND)
    g = sc.still()
    for k in range(6):                                                                           # the ripples of the bar
        g.box(SAND_D, (-340 + k * 135, 20 + (k % 2) * 60, top + 1), (120, 12, 5), rot=(0, 12, 0))
    g.sphere(ROCK_L, (-380, 130, top + 6), 30.0, scale=(1.2, 1.0, 0.7), steps=(3, 6))
    _flounder(sc, -230, 60, top, 20.0, 1.35, HULL, CREAM, ROCK_L, r.random())
    _flounder(sc, 0, -90, top + 3, 0.0, 1.2, ROCK_L, BONE, PINK, r.random(), travel=(720.0, 9.0))
    _flounder(sc, 250, 50, top + 4, -160.0, 1.3, STRAW, RUST_D, CREAM, r.random(), rate=0.11, bob=48.0, ripple=False)
    tuft(sc, 390, 130, top, KELP_L, 58, r); tuft(sc, -60, 150, top, GRASS, 46, r)


def _shrimp(sc, x, y, z, yaw, s, phase):
    """A cleaner shrimp, red and white bands, nose to +X: four long white feelers that never stop waving."""
    c = Cr(sc, x, y, z, yaw, s, phase=phase, rate=0.7)
    b = c.part()
    tilt_box(b, RED, (-30, 0, 22), (-8, 0, 30), 16, 16)
    tilt_box(b, WHITE, (-8, 0, 30), (14, 0, 30), 18, 18)
    tilt_box(b, RED, (14, 0, 30), (34, 0, 25), 16, 16)
    b.box(WHITE, (-40, 0, 17), (16, 26, 5))
    for sd in (-1, 1):
        b.box(DARK, (36, sd * 7, 31), (5, 5, 5))
        for k in range(3):
            b.box(WHITE, (-14 + k * 14, sd * 9, 10), (5, 5, 22))
        tilt_box(b, RED, (26, sd * 9, 20), (52, sd * 22, 22), 7, 7)
        b.box(WHITE, (58, sd * 24, 22), (12, 8, 8))
        f = c.part(sd * 22.0, (34, sd * 5, 28), UP)
        tilt_box(f, WHITE, (34, sd * 5, 28), (112, sd * 30, 72), 4.5, 4.5)
        f2 = c.part(sd * 15.0, (34, sd * 8, 26), 90.0, phase=phase + 0.3)
        tilt_box(f2, WHITE, (34, sd * 8, 26), (94, sd * 52, 36), 4.5, 4.5)


def shrimps(sc):
    """A cleaning station: four banded shrimps on a knob of rock among anemones, feelers waving, and a pink fish that
    has come to be cleaned, hanging still over them."""
    r = random.Random(65)
    top = table(sc.still(), r, 460, 340, 110)
    g = sc.still()
    g.sphere(ROCK, (0, 50, top), 90.0, scale=(1.25, 1.0, 0.8), steps=(4, 8))
    knob = top + 72
    for k in range(7):                                                                           # an anemone that sways
        an = math.radians(k * 51.4)
        m = sc.part(P.motion(swing=9.0, pivot=(-150, 100, top), axis=k * 51.4, rate=0.3, phase=k * 0.14))
        tilt_box(m, PINK, (-150 + math.cos(an) * 10, 100 + math.sin(an) * 10, top), (-150 + math.cos(an) * 30, 100 + math.sin(an) * 30, top + 46), 8, 8)
    sc.still().cyl(MAG, (-150, 100, top - 4), 22, 16, steps=7)
    sprig(sc.still(), 165, 110, top, LAV, 50)
    _shrimp(sc, 0, 40, knob, -90.0, 1.5, r.random())
    _shrimp(sc, -120, -60, top, -60.0, 1.3, r.random())
    _shrimp(sc, 120, -50, top, -120.0, 1.4, r.random())
    _shrimp(sc, 30, -120, top, -80.0, 1.15, r.random())
    fish(sc, 10, -40, knob + 95, 180.0, 1.0, L=130, H=56, W=34, body=PINK, belly=CREAM, fin=MAG, tail=MAG, lips=CORAL,
         wag=10.0, flap=14.0, phase=r.random(), rate=0.4, bob=-6.0)
    tuft(sc, 195, -20, top, KELP_L, 52, r)


def _pig(sc, x, y, z, yaw, s, **q):
    """A sea pig (P.quadruped: a round pink body on short legs) with the long feelers of its back drawn onto its body
    part (fetched with the very motion values quadruped() gave it, so they move with it)."""
    d = dict(body=PINK, belly=CREAM, length=70.0, height=46.0, girth=40.0, neck=8.0, neck_up=-15.0, head=34.0, ears=ROCK_L, tail=0.0,
             legs_col=ROCK_L, snout=CORAL)
    d.update(q)
    P.quadruped(sc, x, y, z, yaw, s, **d)
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def Wf(p):
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])
    tv, base = (0.0, 0.0), (x, y)
    if d.get('travel'):
        tv = (float(d['travel'][0]), float(d['travel'][1]) / float(d['travel'][0]))
    elif d.get('orbit'):
        base, tv = (d['orbit'][0], d['orbit'][1]), (-1.0, float(d['orbit'][2]))
    mo = P.motion(swing=0.0, pivot=Wf((0, 0, d['height'])), axis=yaw + 0.0, phase=d.get('phase', 0.0), rate=d.get('rate', 0.0),
                  bob=d.get('bob', 0.0), base=base, travel=tv)
    m = sc.part(mo, (x, y, z), yaw, s)
    for sd in (-1, 1):                                    # the quadruped's head is towards -Y
        tilt_box(m, ROCK_L, (sd * 10, -14, 44), (sd * 17, -24, 80), 6, 6)
        tilt_box(m, ROCK_L, (sd * 9, 18, 44), (sd * 14, 30, 68), 6, 6)


def sea_pigs(sc):
    """A herd of five pink sea pigs on a sand bar: three nose the sand, two trot along it one behind the other."""
    r = random.Random(66)
    top = table(sc.still(), r, 900, 360, 110, top=SAND)
    g = sc.still()
    for k in range(5):
        g.box(SAND_D, (-330 + k * 160, 60 + (k % 2) * 50, top + 1), (110, 12, 5), rot=(0, -10, 0))
    g.sphere(ROCK_L, (385, 130, top + 6), 32.0, scale=(1.2, 1.0, 0.7), steps=(3, 6))
    _pig(sc, -290, 70, top, 25.0, 1.5, graze=12.0, rate=0.45, phase=r.random())
    _pig(sc, -40, 110, top, -150.0, 1.3, graze=9.0, rate=0.38, phase=r.random())
    _pig(sc, 250, 60, top, 70.0, 1.6, graze=11.0, rate=0.5, phase=r.random())
    _pig(sc, -150, -95, top, 90.0, 1.45, stride=15.0, rate=1.0, phase=r.random(), travel=(780.0, 14.0))
    _pig(sc, 60, -110, top, 90.0, 1.2, stride=17.0, rate=1.15, phase=r.random(), travel=(780.0, 14.0))
    tuft(sc, -410, 120, top, KELP_L, 54, r)


def _hold_crook(col=WOOD_D):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1], h[2] - 96), (h[0], h[1] - 6, h[2] + 62), 5, 5)
        tilt_box(m, col, (h[0], h[1] - 6, h[2] + 62), (h[0], h[1] - 26, h[2] + 74), 5, 5)
        tilt_box(m, col, (h[0], h[1] - 26, h[2] + 74), (h[0], h[1] - 34, h[2] + 56), 5, 5)
    return f


def pig_keeper(sc):
    """The sea pigs' keeper on her rock, a crook in the hand, waving; one pig walks round her, one feeds at a stone
    trough, one looks on; her hand sledge with a bale of dried weed."""
    r = random.Random(67)
    top = table(sc.still(), r, 680, 400, 115, top=SAND)
    g = sc.still()
    g.box(ROCK, (-150, 70, top + 14), (150, 120, 30), rot=(0, 8, 0))                              # her step of rock
    kz = top + 30
    P.figure(sc, -150, 70, kz, yaw=12.0, s=1.0, skin='#d9a070', top='#ff5fa2', bottom='#ff5fa2', shoes='#22202a', hair='bald',
             sleeves='long', hat='helmet', hat_col='#f4f9ff', pack='#8f9bb3', arm_l='wave', swing_l=14.0, arm_r='hold',
             hold_r=_hold_crook(), detail=2, rate=0.6, bob=-3.0, phase=r.random())
    _pig(sc, -150 + 175, 70, top, 180.0, 1.35, stride=15.0, rate=1.0, phase=r.random(), orbit=(-150.0, 70.0, 0.035))
    g = sc.still()
    g.box(CREAM, (150, -60, top + 12), (150, 54, 24)); g.box(STRAW, (150, -60, top + 26), (130, 38, 8))      # the trough
    _pig(sc, 150, 20, top, 0.0, 1.5, graze=12.0, rate=0.5, phase=r.random())
    _pig(sc, 40, -130, top, 60.0, 1.15, graze=6.0, rate=0.3, phase=r.random())
    g = sc.still()                                                                                # the hand sledge
    for sd in (-1, 1):
        g.box(WOOD_D, (230, 110 + sd * 32, top + 6), (170, 9, 10))
        tilt_box(g, WOOD_D, (310, 110 + sd * 32, top + 6), (336, 110 + sd * 32, top + 34), 9, 9)
    g.box(WOOD_D, (230, 110, top + 15), (150, 74, 8))
    g.box(STRAW, (215, 110, top + 42), (96, 62, 46)); g.box(CREAM, (215, 110, top + 42), (8, 66, 50))
    tilt_box(g, CREAM, (336, 110, top + 34), (382, 100, top + 4), 5, 5)
    tuft(sc, -300, 150, top, KELP_L, 62, r); tuft(sc, 300, -150, top, GRASS, 44, r)


def _spider(sc, x, y, s, yaw, shell, leg, phase, travel=None):
    """A decorator crab on eight long legs, nose to +X, its back planted with sponges and an anemone."""
    c = Cr(sc, x, y, 0.0, yaw, s, phase=phase, rate=0.45, travel=travel)
    b = c.part(3.0, (0, 0, 0), 0.0)
    b.sphere(shell, (0, 0, 190), 60.0, scale=(1.0, 0.85, 0.6), steps=(4, 8))
    b.sphere(CREAM, (6, 0, 176), 52.0, scale=(1.0, 0.84, 0.5), steps=(3, 6))
    b.sphere(YEL, (-18, 14, 226), 17.0, steps=(3, 6))
    b.cyl(LAV, (10, -18, 216), 9, 34, steps=5, top_r=12)
    b.cyl(PINK, (-34, -12, 214), 12, 20, steps=6, top_r=17)
    for sd in (-1, 1):
        b.box(shell, (54, sd * 14, 204), (6, 6, 24))
        b.sphere(BONE, (55, sd * 14, 220), 7.0, steps=(3, 5))
        b.box(DARK, (61, sd * 14, 220), (5, 5, 5))
        cl = c.part(sd * 12.0, (50, sd * 26, 182), UP)
        tilt_box(cl, leg, (50, sd * 26, 182), (112, sd * 40, 150), 11, 11)
        cl.box(CREAM, (122, sd * 42, 146), (24, 14, 14))
        for j, a in enumerate((38.0, 72.0, 108.0, 142.0)):
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            hip, knee, foot = (ca * 46, sd * sa * 40, 186), (ca * 122, sd * sa * 122, 272), (ca * 192, sd * sa * 192, 0)
            lg = c.part((1 if (j + (sd > 0)) % 2 else -1) * 8.0, hip, UP, phase=phase + (0.5 if j % 2 else 0.0))
            tilt_box(lg, leg, hip, knee, 11, 11)
            tilt_box(lg, leg, knee, foot, 9, 9)


def spider_crabs(sc):
    """Two decorator crabs on long legs: one stands astride a heap of ship's plates and pipes, one steps along behind."""
    r = random.Random(68)
    g = sc.still()
    pts = [(math.cos(math.radians(k * 36)) * (470 + (k % 3) * 22), math.sin(math.radians(k * 36)) * (300 + (k % 2) * 20)) for k in range(10)]
    g.plan(SAND_D, pts, -140.0, 8.0)
    tilt_box(g, RUST, (-300, -40, 6), (-90, -30, 92), 150, 8)                                   # plates
    tilt_box(g, RUST_D, (-190, 30, 6), (20, 10, 60), 180, 8)
    tilt_box(g, HULL, (-40, -60, 6), (90, -40, 110), 120, 8)
    g.box(RUST_D, (-150, -90, 12), (230, 110, 10), rot=(0, 14, 0))
    rod(g, TEAL, (-260, 40, 20), (60, 70, 34), 15, steps=7)                                     # pipes
    rod(g, TEAL, (-80, -110, 16), (130, -90, 16), 12, steps=7)
    rod(g, STRAW, (-20, 40, 16), (-20, 40, 150), 8, steps=6)                                    # a rail post and its ring
    g.cyl(STRAW, (-20, 40, 150), 16, 8, steps=7)
    g.cyl(BONE, (-170, -122, 8), 34, 10, steps=8); g.cyl(DARK, (-170, -122, 15), 22, 5, steps=8)   # a porthole, lost
    for x, y in ((180, -60), (230, 20), (-340, 60)):
        g.sphere(ROCK, (x, y, 8), 30.0, scale=(1.2, 1.0, 0.7), steps=(3, 6))
    _spider(sc, -110, -20, 1.0, 14.0, CORAL, ROCK_L, r.random())
    _spider(sc, 120, 190, 0.8, 0.0, CREAM, CORAL, r.random(), travel=(760.0, 16.0))
    tuft(sc, 330, -120, 8, KELP_L, 70, r); tuft(sc, -400, -60, 8, GRASS, 54, r)


# ------------------------------------------------------------------------------------------ water
def _anchor(sc, r, w=220.0, d=170.0, h=80.0, weeds=(KELP_L, GRASS)):
    """What a piece of swimmers stands on: a small mound of rock with weed, so it has a place on the sand."""
    g = sc.still()
    g.sphere(ROCK, (0, 0, 0), 100.0, scale=(w / 200.0, d / 200.0, h / 100.0), steps=(4, 8))
    g.sphere(ROCK_D, (w * 0.3, d * 0.2, 0), 100.0, scale=(w / 320.0, d / 300.0, h / 160.0), steps=(3, 6))
    g.box(ROCK_D, (0, 0, -90), (w * 0.8, d * 0.8, 100))
    for k, col in enumerate(weeds):
        tuft(sc, (k - 0.5) * w * 0.5, d * 0.1, h * 0.6, col, 80 + k * 30, r)


def parrot_fish(sc):
    """A coral head and the four parrot fish that graze it, pink and green: two peck at it, two hang by and watch."""
    r = random.Random(71)
    g = sc.still()
    g.sphere(ROCK, (0, 40, 0), 100.0, scale=(2.0, 1.6, 2.3), steps=(5, 9))
    g.sphere(ROCK_D, (150, 70, 0), 100.0, scale=(1.3, 1.1, 1.2), steps=(4, 7))
    g.box(ROCK_D, (0, 40, -110), (330, 260, 120))
    sprig(g, -70, 30, 222, CORAL, 96); sprig(g, 60, 70, 214, MAG, 80); sprig(g, 150, 60, 116, LAV, 70); sprig(g, -150, 10, 150, PINK, 60)
    g.cyl(CREAM, (10, -70, 150), 9, 40, steps=5); g.cyl(CREAM, (10, -70, 188), 70, 9, steps=9)      # a plate coral
    g.cyl(CREAM, (-120, 90, 150), 8, 50, steps=5); g.cyl(CREAM, (-120, 90, 198), 54, 9, steps=9)
    look_a = dict(body=PINK, belly=CREAM, fin=GRASS, tail=GRASS, lips=CREAM, bands=((-0.18, MAG),))
    look_b = dict(body=GRASS, belly=PINK, fin=MAG, tail=MAG, lips=YEL, bands=((0.1, KELP_L),))
    fish(sc, -235, -30, 250, 8.0, 1.35, L=130, H=62, W=36, nod=11.0, phase=r.random(), rate=0.9, **look_a)
    fish(sc, 60, -200, 215, 80.0, 1.2, L=130, H=62, W=36, nod=9.0, phase=r.random(), rate=1.1, **look_b)
    fish(sc, 250, -90, 400, 170.0, 1.45, L=130, H=62, W=36, phase=r.random(), rate=0.5, bob=-14.0, **look_b)
    fish(sc, -120, -160, 470, 20.0, 1.1, L=130, H=62, W=36, phase=r.random(), rate=0.6, bob=-10.0, **look_a)
    tuft(sc, 230, -40, 30, KELP_L, 90, r)


def _cuttle(sc, x, y, z, s, body, stripe, skirt, phase0, travel):
    """A cuttlefish, nose (its arms) to +X: a striped mantle, a skirt of fin that ripples down both sides, eight arms."""
    c = Cr(sc, x, y, z, 0.0 if travel[1] > 0 else 180.0, s, rate=0.7, travel=travel)
    b = c.part()
    b.sphere(body, (-20, 0, 0), 50.0, scale=(1.7, 0.8, 0.55), steps=(4, 8))
    for k in range(5):
        xk = -80 + k * 30
        f = math.sqrt(max(0.05, 1.0 - ((xk + 20) / 85.0) ** 2))
        b.box(stripe, (xk, 0, 5), (9, 62 * f, 55 * f - 6))
    b.sphere(body, (72, 0, -4), 28.0, scale=(1.1, 1.1, 0.9), steps=(3, 6))
    for sd in (-1, 1):
        b.sphere(BONE, (78, sd * 22, 4), 9.0, steps=(3, 5))
        b.box(DARK, (82, sd * 28, 4), (6, 5, 5))
    a = c.part(8.0, (92, 0, -6), 90.0, phase=phase0)
    for k in range(6):
        yy = (k - 2.5) * 7.0
        tilt_box(a, body, (92, yy, -6), (150 + (k % 2) * 14, yy * 1.7, -30 - (k % 3) * 8), 8, 8)
    for sd in (-1, 1):
        tilt_box(a, skirt, (96, sd * 10, -2), (188, sd * 16, -8), 6, 6)
        a.box(skirt, (196, sd * 16, -8), (22, 12, 6))
        for j in range(7):
            xj = -95 + j * 25
            fj = math.sqrt(max(0.1, 1.0 - ((xj + 20) / 85.0) ** 2))
            f = c.part(sd * 20.0, (xj, sd * 36 * fj, 0), 0.0, phase=phase0 + j * 0.14)
            f.box(skirt, (xj, sd * (36 * fj + 14), 0), (26, 30, 5))


def cuttlefish(sc):
    """A pair of striped cuttlefish hanging over a mound of weed, their skirts rippling, drifting past each other."""
    r = random.Random(72)
    _anchor(sc, r, 260, 190, 90)
    _cuttle(sc, -120, -30, 400, 1.5, CREAM, PLUM, LAV, r.random(), (520.0, 12.0))
    _cuttle(sc, 140, 60, 640, 1.25, PINK, MAG, CREAM, r.random(), (520.0, -10.0))


def _puffer(sc, x, y, z, yaw, R, phase, puffed=False):
    """A spotted puffer, nose to +X: a white ball with brown spots, a cream belly, pink lips, three small fins."""
    c = Cr(sc, x, y, z, yaw, 1.0, phase=phase, rate=0.35, bob=-14.0)
    b = c.part()
    b.sphere(WHITE, (0, 0, 0), R, scale=(1.08, 1.0, 1.0), steps=(4, 8))
    b.sphere(CREAM, (0, 0, -R * 0.12), R * 0.94, scale=(1.05, 1.0, 1.0), steps=(3, 6))
    rr = random.Random(int(R * 7))
    for k in range(6):
        el, az = math.radians(rr.uniform(12, 70)), math.radians(k * 60 + rr.uniform(-14, 14))
        b.sphere(HULL, (R * 1.0 * math.cos(el) * math.cos(az), R * 0.96 * math.cos(el) * math.sin(az), R * 0.96 * math.sin(el)), max(R * 0.13, 5.0), steps=(3, 5))
    if puffed:
        for k in range(14):
            el, az = math.radians(-50 + (k % 4) * 32), math.radians(k * 77.0)
            dx, dy, dz = math.cos(el) * math.cos(az), math.cos(el) * math.sin(az), math.sin(el)
            if dx > 0.8:
                continue
            rod(b, CREAM, (R * 0.96 * dx, R * 0.96 * dy, R * 0.96 * dz), (R * 1.22 * dx, R * 1.22 * dy, R * 1.22 * dz), 3.5, steps=4)
    b.box(PINK, (R * 1.04, 0, -R * 0.1), (R * 0.2, R * 0.36, R * 0.22))
    for sd in (-1, 1):
        b.sphere(BONE, (R * 0.72, sd * R * 0.52, R * 0.36), max(R * 0.17, 6.0), steps=(3, 5))
        b.box(DARK, (R * 0.82, sd * R * 0.6, R * 0.38), (max(R * 0.1, 4.0), max(R * 0.1, 4.0), max(R * 0.12, 4.0)))
        p = c.part(sd * 28.0, (R * 0.1, sd * R * 0.9, -R * 0.1), 0.0)
        p.box(CREAM, (R * 0.05, sd * (R * 0.9 + R * 0.22), -R * 0.1), (R * 0.34, R * 0.44, 5))
    t = c.part(24.0, (-R * 0.95, 0, 0), UP)
    t.box(CREAM, (-R * 1.2, 0, 0), (R * 0.5, 5, R * 0.56))
    t.box(HULL, (-R * 1.42, 0, 0), (R * 0.1, 5.5, R * 0.6))


def puffers(sc):
    """Four spotted puffers hanging over a mound, white with brown spots, each bobbing to its own time; one is blown
    up round and prickly."""
    r = random.Random(73)
    _anchor(sc, r, 240, 180, 80, weeds=(KELP_L, LAV))
    _puffer(sc, -200, -20, 300, 20.0, 46.0, r.random())
    _puffer(sc, 40, -90, 520, -150.0, 80.0, r.random(), puffed=True)
    _puffer(sc, 230, 30, 330, 170.0, 52.0, r.random())
    _puffer(sc, -90, 70, 690, -30.0, 40.0, r.random())


def _lion(sc, x, y, z, yaw, s, phase):
    """A lionfish, nose to +X: white with red bands, a comb of short spines down the back, two fans of long spines."""
    c = Cr(sc, x, y, z, yaw, s, phase=phase, rate=0.4, bob=-12.0)
    b = c.part()
    b.sphere(WHITE, (0, 0, 0), 50.0, scale=(1.0, 0.36, 0.5), steps=(4, 8))
    for at in (-0.3, -0.1, 0.12, 0.3):
        f = math.sqrt(1.0 - (2.0 * at) ** 2)
        b.box(RED, (at * 100, 0, 0), (7, 36 * f + 2.5, 50 * f + 2.5))
    for sd in (-1, 1):
        b.box(BONE, (34, sd * 12, 8), (8, 5, 9)); b.box(DARK, (36, sd * 14.5, 8), (5, 4, 5))
    sp = c.part(6.0, (0, 0, 22), 90.0)
    for k in range(7):
        xk = -32 + k * 11
        tilt_box(sp, RED if k % 2 else WHITE, (xk, 0, 20), (xk - 9, 0, 54 - abs(k - 3) * 4), 5, 5)
    t = c.part(20.0, (-44, 0, 0), UP)
    for k in range(3):
        tilt_box(t, RED if k % 2 else WHITE, (-44, 0, 0), (-80, 0, (k - 1) * 22), 5, 12)
    for sd in (-1, 1):
        p = c.part(sd * 16.0, (18, sd * 14, -6), 0.0)
        for k in range(5):
            tip = (-14 + k * 14, sd * (14 + 62), -42 + k * 22)
            tilt_box(p, RED if k % 2 else WHITE, (18, sd * 14, -6), tip, 6, 6)


def lionfish(sc):
    """Three lionfish drifting over a mound, red and white, their fans of spines opening and closing."""
    r = random.Random(74)
    _anchor(sc, r, 250, 180, 85, weeds=(GRASS, KELP_L))
    _lion(sc, -190, -40, 330, 15.0, 1.6, r.random())
    _lion(sc, 110, 40, 520, -165.0, 1.4, r.random())
    _lion(sc, 10, -110, 700, 40.0, 1.2, r.random())


def _seal(sc, ang, R, z, body, belly, spot, phase, orate):
    """A plump seal with a round head, a snout and four flippers, swimming a circle round the piece's middle."""
    x, y = R * math.cos(math.radians(ang)), R * math.sin(math.radians(ang))
    c = Cr(sc, x, y, z, ang + 90.0, 1.0, phase=phase, rate=0.5, bob=-22.0, orbit=(0.0, 0.0, orate))
    b = c.part()
    b.sphere(body, (0, 0, 0), 50.0, scale=(2.0, 0.76, 0.72), steps=(4, 8))
    b.sphere(belly, (8, 0, -9), 50.0, scale=(1.7, 0.72, 0.62), steps=(3, 6))
    b.sphere(body, (98, 0, 12), 30.0, scale=(1.05, 0.95, 0.9), steps=(4, 6))
    b.box(belly, (126, 0, 5), (18, 26, 18)); b.box(DARK, (136, 0, 9), (5, 10, 7))
    for sd in (-1, 1):
        b.box(DARK, (112, sd * 24, 18), (6, 5, 7))
    for dx, dy in ((30, 12), (-10, -16), (-44, 8), (56, -8)):
        b.box(spot, (dx, dy, 34 - abs(dx) * 0.14), (13, 13, 5))
    for sd in (-1, 1):
        f = c.part(sd * 24.0, (40, sd * 34, -12), 0.0)
        tilt_box(f, body, (40, sd * 34, -12), (10, sd * 80, -34), 30, 6)
    t = c.part(18.0, (-96, 0, 0), UP)
    for sd in (-1, 1):
        tilt_box(t, body, (-92, sd * 8, 0), (-142, sd * 22, 0), 7, 28)


def seals(sc):
    """Three seals looping one behind another round a banded post with a weed pennant, 3.5 to 8.5 m up."""
    r = random.Random(75)
    g = sc.still()
    g.sphere(ROCK, (0, 0, 0), 100.0, scale=(1.3, 1.2, 0.9), steps=(4, 8))
    g.box(ROCK_D, (0, 0, -90), (200, 180, 100))
    g.cyl(BONE, (0, 0, 40), 26, 960, steps=8)
    for z in (230, 520, 810):
        g.cyl(PINK, (0, 0, z), 30, 64, steps=8)
    g.cyl(STRAW, (0, 0, 1000), 38, 14, steps=8)
    g.sphere(STRAW, (0, 0, 1050), 42.0, steps=(4, 8))
    pn = sc.part(P.motion(swing=14.0, pivot=(0, 0, 950), axis=UP, rate=0.3, phase=0.2))
    pn.box(KELP_L, (95, 0, 950), (180, 6, 46)); pn.box(GRASS, (215, 0, 944), (70, 6, 30))
    _seal(sc, 0.0, 420.0, 380.0, GREY, CREAM, PLUM, r.random(), 0.07)
    _seal(sc, 120.0, 420.0, 610.0, ROCK_L, CREAM, ROCK_D, r.random(), 0.07)
    _seal(sc, 240.0, 420.0, 830.0, BONE, CREAM, HULL, r.random(), 0.07)
    tuft(sc, 110, -60, 40, KELP_L, 90, r)


# ------------------------------------------------------------------------------------------ far water
def bait_ball(sc):
    """A silver ball of ninety-six small fish, 10 m across: eight bands of them wheel round its middle, some one way
    and some the other, tilting and rising and falling each to its own time, so the ball seethes and breathes."""
    r = random.Random(81)
    Z0 = 600.0
    for k in range(8):
        sgn = 1 if k % 2 == 0 else -1
        mo = P.motion(pivot=(0, 0, Z0), base=(0, 0), travel=(-1.0, sgn * r.uniform(0.05, 0.11)), bob=-r.uniform(30, 70), rate=r.uniform(0.15, 0.3),
                      phase=k / 8.0, swing=sgn * r.uniform(8, 13), axis=k * 45.0)
        m = sc.part(mo)
        for j in range(12):
            rad = r.uniform(230, 500)
            el, az = math.radians(r.uniform(-62, 62)), math.radians(r.uniform(0, 360))
            x, y, z = rad * math.cos(el) * math.cos(az), rad * math.cos(el) * math.sin(az), Z0 + rad * math.sin(el)
            tx, ty = -math.sin(az) * sgn, math.cos(az) * sgn
            col = (WHITE, GREY, WHITE, BONE, WHITE, PINK)[(j + k) % 6]
            tilt_box(m, col, (x - tx * 24, y - ty * 24, z), (x + tx * 24, y + ty * 24, z + r.uniform(-4, 4)), 9, 17)


def sunfish(sc):
    """One huge sunfish, a pale disc 4.2 m long and 3.8 m tall with two round paddles, drifting along the road's
    line; three small pink fish keep it company."""
    r = random.Random(82)
    mv = dict(phase=0.2, rate=0.18, bob=-30.0, travel=(2400.0, 9.0))
    c = Cr(sc, 0, 0, 420, 0.0, 1.0, **mv)
    b = c.part()
    b.sphere(GREY, (0, 0, 0), 50.0, scale=(4.2, 0.7, 3.8), steps=(6, 12))
    b.sphere(PINK, (10, 0, -34), 50.0, scale=(3.9, 0.76, 3.0), steps=(5, 10))
    for k in range(5):
        b.sphere(ROCK_L, (-200, 0, -124 + k * 62), 36.0, scale=(0.7, 0.5, 1.0), steps=(3, 6))
    for dx, dz in ((40, 110), (-60, 60), (-110, 130), (90, 40), (-20, 150)):
        half = 35.0 * math.sqrt(max(0.1, 1.0 - (dx / 210.0) ** 2 - (dz / 190.0) ** 2))      # the disc's half thickness there
        b.sphere(BONE, (dx, 0, dz), 16.0, scale=(1.0, (half + 3.0) / 16.0, 1.0), steps=(3, 6))
    for sd in (-1, 1):
        b.sphere(BONE, (150, sd * 22, 40), 17.0, steps=(3, 6))
        b.box(DARK, (156, sd * 37, 40), (10, 5, 12))
        p = c.part(sd * 20.0, (86, sd * 30, -20), 90.0)
        p.sphere(PINK, (70, sd * 36, -34), 30.0, scale=(1.0, 0.25, 0.7), steps=(3, 6))
    b.sphere(CREAM, (207, 0, 0), 15.0, scale=(0.6, 1.2, 1.2), steps=(3, 6))
    for sgn in (1, -1):                                   # the two paddles: round, alike above and below
        f = c.part(sgn * 14.0, (-60, 0, sgn * 170), 0.0)
        f.sphere(PINK, (-72, 0, sgn * 238), 50.0, scale=(1.5, 0.4, 1.9), steps=(4, 7))
    for x, y, z in ((-250, 110, 300), (-330, -90, 520), (-180, -130, 640)):
        fish(sc, x, y, z, 0.0, 1.0, L=64, H=30, W=18, body=PINK, belly=CREAM, fin=MAG, tail=MAG, wag=24.0, flap=20.0,
             phase=r.random(), rate=0.18, bob=-30.0, travel=(2400.0, 9.0))


def sea_angels(sc):
    """Seven sea angels, pale winged swimmers (P.bird), beating slowly along one straight line with the road."""
    r = random.Random(83)
    for k in range(7):
        x, y, z = -330 + k * 110 + r.uniform(-30, 30), r.uniform(-260, 260), 80 + r.uniform(0, 520)
        P.bird(sc, x, y, z, heading=0.0, span=r.uniform(355, 400), body=BONE, wing=(LAV, PINK, CREAM)[k % 3], beak=PINK,
               phase=r.random(), rate=r.uniform(0.8, 1.0), flap=30.0, travel=(4400.0, 85.0), bob=-18.0)


G1 = dict(grow=(60, 60, 30), grow_down=(60, 60, 0))
JOBS = [
    ('SM_S1ANIMALS_HermitCrabs', hermit_crabs, G1), ('SM_S1ANIMALS_Starfish', starfish, G1), ('SM_S1ANIMALS_SeaSlugs', sea_slugs, G1),
    ('SM_S1ANIMALS_Flounders', flounders, dict(grow=(60, 60, 70), grow_down=(60, 60, 0))), ('SM_S1ANIMALS_Shrimps', shrimps, G1),
    ('SM_S1ANIMALS_SeaPigs', sea_pigs, G1), ('SM_S1ANIMALS_PigKeeper', pig_keeper, G1),
    ('SM_S1ANIMALS_SpiderCrabs', spider_crabs, dict(grow=(260, 60, 20), grow_down=(260, 60, 0))),
    ('SM_S1ANIMALS_ParrotFish', parrot_fish, G1),
    ('SM_S1ANIMALS_Cuttlefish', cuttlefish, dict(grow=(200, 60, 30), grow_down=(200, 60, 0))),
    ('SM_S1ANIMALS_Puffers', puffers, G1), ('SM_S1ANIMALS_Lionfish', lionfish, G1),
    ('SM_S1ANIMALS_Seals', seals, dict(grow=(180, 180, 40), grow_down=(180, 180, 0))),
    ('SM_S1ANIMALS_BaitBall', bait_ball, dict(far=True, grow=(120, 120, 90), grow_down=(120, 120, 90))),
    ('SM_S1ANIMALS_Sunfish', sunfish, dict(far=True, grow=(1300, 80, 60), grow_down=(1300, 80, 60))),
    ('SM_S1ANIMALS_SeaAngels', sea_angels, dict(far=True, grow=(2300, 60, 60), grow_down=(2300, 60, 60))),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
