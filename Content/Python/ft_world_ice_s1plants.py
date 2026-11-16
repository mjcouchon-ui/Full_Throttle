"""Ice Mountain, Stage 1, builder s1plants: what grows on the mountain, district by district.
/Game/World/IceMountain/SM_S1PLANTS_*. Run: import ft_world_ice_s1plants as K; K.run()

The level's own pines (plain dark cones) are everywhere already, so nothing here is a plain pine. Twenty pieces:
  the summit and the ridges   SnowFir, FirGrove, StormFir, DwarfPine, SnowGrass
  the valley town             FrostBirch, BirchCopse, GoldLarch, Rowan, AlpineRose
  the spa resort              BlueSpruce, JuniperCone, JuniperHedge, RoseBoxes
  the aurora ridge            Heather, Edelweiss, LichenRock
  the glacier and the mine    FrostFern, IceReed, GlowMoss
Every piece is one ft_people.Scene (one section, M_People). Crowns rock about the foot of the trunk, more the higher
the tier; reeds, grass, heather and fronds lean and tremble about their own roots. No actor, no tick: the movement is
the builder's motion parameters only. LOW pieces are drawn to top 1.1 m or less; the three that glow (FrostFern,
GlowMoss, BlueSpruce's lamps) are placed 25 m out and more by the table block.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'

# the kit's palette (ft_dress_ice.py), nothing else
SNOW, SNOW_SH, ICE_D, ICE_DEEP = '#f4f9ff', '#cdd6f1', '#7fb0e0', '#4f8fd2'
ROCK, ROCK_D, CUT, CUT_D = '#6a7488', '#5a6478', '#8b93b4', '#7a83a8'
PINE, PINE_D, WOOD, WOOD_D = '#2f6b52', '#234f40', '#9a6a44', '#6a4c36'
RED, BLUE, NET, CONC = '#ff2d3c', '#2e6bff', '#3b3f52', '#aab3c4'
LIT, PINK, YEL, TEAL_LIT = '#ffe28a', '#ff4fb8', '#ffd23f', '#2fc6c0'
CREAM, MINT, LILAC = '#f4efe6', '#8ff0cf', '#b9a2ff'

WIND = 20.0     # the axis every crown rocks about (degrees in the ground plane): one wind for the whole mountain
LOW = dict(grow=(25, 25, 15), grow_down=(25, 25, 0))
TALL = dict(grow=(90, 90, 30), grow_down=(90, 90, 0))


def blob(m, col, c, rx, ry, rz, steps=(4, 7), rot=(0, 0, 0), glow=0.0):
    m.sphere(col, c, 100, scale=(rx / 100.0, ry / 100.0, rz / 100.0), steps=steps, rot=rot, glow=glow)


def sway(swing, pivot, rate=0.3, phase=0.0, axis=WIND):
    return P.motion(swing=swing, pivot=pivot, axis=axis, rate=rate, phase=phase)


def _rock(m, x, y, w, d, h, seed, col=ROCK, col2=ROCK_D, sink=30.0):
    """A faceted boulder w x d on the ground, h tall, a third of it underground."""
    r = random.Random(seed)
    m.sphere(col, (x, y, (h - sink) / 2.0), 100, scale=(w / 200.0, d / 200.0, (h + sink) / 200.0), steps=(4, 6), rot=(0, r.uniform(0, 60), 0))
    m.sphere(col2, (x + w * 0.3, y - d * 0.12, (h * 0.6 - sink) / 2.0), 100, scale=(w / 330.0, d / 280.0, (h * 0.6 + sink) / 200.0), steps=(4, 5),
             rot=(0, r.uniform(0, 60), 0))


# ------------------------------------------------------------------------------------------ firs
def _fir(sc, x, y, h, rad, seed, tiers=4, pillows=2, lean=0.05, rate=0.3, cones=0):
    """A fir bowed under snow: tiers of dark cones, each with a mantle of snow and pillows hanging off its rim, the
    tip bent right over under a last lump. The tiers rock about the foot, the higher the more."""
    r = random.Random(seed)
    g = sc.still()
    g.cyl(WOOD_D, (x, y, -220), h * 0.03 + 10, 220 + h * 0.3, steps=6, top_r=h * 0.018 + 6)
    piv = (x, y, 0)
    th = h * 1.26 / tiers
    top = (x, y, h)
    for i in range(tiers):
        t = i / float(tiers)
        z0 = h * (0.16 + 0.66 * t)
        ra = rad * (1.0 - 0.72 * t)
        dx = lean * t * t * h
        c = sc.part(sway(1.4 + 2.4 * t, piv, rate, 0.06 * i))
        c.cyl(PINE_D if i % 2 == 0 else PINE, (x + dx, y, z0), ra, th, steps=7, top_r=ra * 0.12)
        c.cyl(PINE_D, (x + dx, y, z0 - 20), ra * 0.7, 21, steps=7, top_r=ra * 0.96)                 # the dark underside
        c.cyl(SNOW, (x + dx + ra * 0.05, y, z0 + th * 0.3), ra * 0.82, th * 0.72, steps=7, top_r=ra * 0.15)   # the mantle
        for k in range(pillows):
            a = r.uniform(0, 2 * math.pi)
            blob(c, SNOW if k == 0 else SNOW_SH, (x + dx + math.cos(a) * ra * 0.74, y + math.sin(a) * ra * 0.74, z0 + th * 0.1),
                 ra * 0.4, ra * 0.34, ra * 0.2 + 8)
        for k in range(cones):                                          # brown cones hanging under the rim
            a = r.uniform(0, 2 * math.pi)
            blob(c, WOOD, (x + dx + math.cos(a) * ra * 0.6, y + math.sin(a) * ra * 0.6, z0 - 26), 10, 10, 22, steps=(4, 5))
        top = (x + dx, y, z0 + th * 0.96)
    tip = sc.part(sway(5.0, piv, rate, 0.3))
    k1 = (top[0] + h * 0.05, top[1], top[2] + h * 0.07)
    k2 = (top[0] + h * 0.14, top[1], top[2] + h * 0.045)
    rod(tip, PINE, top, k1, 9, steps=5)
    rod(tip, PINE, k1, k2, 7, steps=5)
    blob(tip, SNOW, (k2[0] + 6, k2[1], k2[2] + 6), 30, 24, 17)


def snow_fir(sc):
    _fir(sc, 0, 0, 930, 250, 11, tiers=5, pillows=3, cones=2)
    g = sc.still()
    blob(g, SNOW_SH, (30, -20, 0), 260, 210, 34)                    # the drift round its foot
    blob(g, SNOW, (-150, 60, 0), 120, 100, 26)


def fir_grove(sc):
    _fir(sc, -40, 60, 960, 250, 21, tiers=4, pillows=1, lean=0.05)
    _fir(sc, 330, -70, 690, 190, 22, tiers=3, pillows=1, lean=0.07, rate=0.34)
    _fir(sc, -390, -60, 520, 150, 23, tiers=3, pillows=1, lean=0.04, rate=0.38)
    g = sc.still()
    blob(g, SNOW_SH, (0, 0, -6), 620, 250, 40)
    s = sc.part(sway(6.0, (150, -170, 0), 0.5, 0.2))                 # a sapling just out of the snow
    s.cyl(PINE, (150, -170, 10), 34, 120, steps=6, top_r=4)
    blob(s, SNOW, (150, -170, 74), 24, 22, 12)


def storm_fir(sc):
    """A flagged fir: the wind has left it branches on the lee side only, the trunk leans the same way."""
    h = 820.0
    g = sc.still()
    a, b, c = (0, 0, -220), (70, 0, 0.5 * h), (190, 0, h)
    rod(g, WOOD_D, a, b, 19, steps=6)
    _rock(g, -70, 30, 190, 150, 70, 31)
    blob(g, SNOW_SH, (60, -20, -4), 240, 170, 30)
    piv = (0, 0, 0)
    up = sc.part(sway(1.6, piv, 0.5, 0.0))
    rod(up, WOOD_D, b, c, 12, steps=6)
    for i in range(6):
        t = i / 5.0
        z = h * (0.26 + 0.125 * i)
        xt = 70 * min(1.0, z / (0.5 * h)) if z < 0.5 * h else 70 + 120 * (z - 0.5 * h) / (0.5 * h)
        L = h * 0.5 * (1.0 - 0.11 * i)
        m = sc.part(sway(2.2 + 2.6 * t, piv, 0.5, 0.05 * i))
        blob(m, PINE_D if i % 2 == 0 else PINE, (xt + L * 0.5, 0, z), L * 0.56, L * 0.2 + 14, 30, rot=(7, 0, 0))
        blob(m, PINE, (xt + L * 0.92, (-1) ** i * 18, z + 4), L * 0.2, L * 0.13 + 8, 22, rot=(12, 0, 0))     # the flag's ragged end
        blob(m, SNOW, (xt + L * 0.45, 0, z + 26), L * 0.36, L * 0.14 + 8, 13, rot=(7, 0, 0))
        if i % 2 == 0:
            tilt_box(g if i < 3 else up, WOOD_D, (xt, 0, z - 6), (xt - 62 + 6 * i, (-1) ** i * 14, z + 12), 8)    # a dead stub to windward
    m = sc.part(sway(5.2, piv, 0.5, 0.35))
    blob(m, PINE, (c[0] + 40, 0, c[2] + 6), 70, 26, 24, rot=(14, 0, 0))
    blob(m, SNOW, (c[0] + 30, 0, c[2] + 26), 40, 18, 10, rot=(14, 0, 0))


def dwarf_pine(sc):
    """LOW: a mat of creeping pine, snow lying on it, candle shoots through the snow."""
    r = random.Random(41)
    g = sc.still()
    blob(g, SNOW_SH, (0, 0, -8), 330, 150, 26)
    for n, (cx, cy) in enumerate(((-205, 20), (-70, -40), (60, 35), (200, -15))):
        m = sc.part(sway(2.6, (cx, cy, 0), 0.7, 0.2 * n))
        for k in range(2):
            ox, oy = cx + (k * 2 - 1) * 52, cy + r.uniform(-28, 28)
            rx, ry = r.uniform(72, 96), r.uniform(58, 76)
            blob(m, PINE_D if (n + k) % 2 == 0 else PINE, (ox, oy, 16), rx, ry, 46)
            blob(m, SNOW, (ox + 8, oy + 6, 55), rx * 0.62, ry * 0.6, 13)
        s = sc.part(sway(8.0, (cx, cy, 40), 0.9, 0.2 * n + 0.1))
        for k in range(4):
            sx, sy = cx + r.uniform(-90, 90), cy + r.uniform(-50, 50)
            s.cyl(PINE, (sx, sy, 48), 9, r.uniform(30, 42), steps=5, top_r=3)
        blob(s, WOOD, (cx + 20, cy - 44, 52), 11, 9, 14, steps=(4, 5))      # one cone


def snow_grass(sc):
    """LOW: tussocks of dry grass through the snow, every one leaning with the wind."""
    r = random.Random(51)
    g = sc.still()
    for n in range(6):
        cx, cy = -230 + n * 92 + r.uniform(-18, 18), r.uniform(-55, 55)
        blob(g, SNOW_SH if n % 2 else SNOW, (cx + 6, cy, -4), 58, 50, 20)
        m = sc.part(sway(8.0, (cx, cy, 6), 0.8, r.random()))
        for k in range(6):
            a = k * 60 + r.uniform(-20, 20)
            L = r.uniform(52, 84)
            out = L * 0.42
            tip = (cx + math.cos(math.radians(a)) * out + 12, cy + math.sin(math.radians(a)) * out, L * 0.9)
            tilt_box(m, WOOD if k % 3 else LIT, (cx, cy, 4), tip, 7, 5)
        m.cyl(WOOD_D, (cx, cy, 4), 6, 62, steps=5)                              # a seed head on a straight stalk
        blob(m, WOOD_D, (cx, cy, 72), 9, 9, 15, steps=(4, 5))


# ------------------------------------------------------------------------------------------ the valley town
def _birch(sc, x, y, h, seed, twigs=7, rate=0.36, bands=4, rime=2):
    """A frost birch: a pale trunk with dark bands, three limbs, a haze of dark twigs with rime on their ends."""
    r = random.Random(seed)
    g = sc.still()
    g.cyl(CREAM, (x, y, -200), 15, 200 + h * 0.42, steps=6, top_r=11)
    for k in range(bands):
        g.cyl(NET, (x, y, h * (0.07 + 0.36 * k / bands)), 15.5 - k * 0.8, 9, steps=6)
    piv = (x, y, 0)
    lim = sc.part(sway(2.0, piv, rate, 0.0))
    out = sc.part(sway(3.8, piv, rate, 0.12))
    fork = (x, y, h * 0.42)
    for j in range(3):
        a = math.radians(j * 120 + r.uniform(-25, 25))
        e = (x + math.cos(a) * h * 0.15, y + math.sin(a) * h * 0.15, h * r.uniform(0.66, 0.74))
        rod(lim, CREAM, fork, e, 8, steps=5)
        for k in range(twigs):
            b = a + math.radians(r.uniform(-80, 80))
            L = h * r.uniform(0.16, 0.26)
            up = r.uniform(0.55, 0.95)
            t1 = (e[0] + math.cos(b) * L * (1 - up * 0.6), e[1] + math.sin(b) * L * (1 - up * 0.6), e[2] + L * up)
            tilt_box(lim, NET, e, t1, 6, 5)
            t2 = (t1[0] + math.cos(b) * L * 0.5, t1[1] + math.sin(b) * L * 0.5, t1[2] - L * 0.22)      # the drooping end
            tilt_box(out, NET, t1, t2, 5, 5)
            if k % rime == 0:
                blob(out, SNOW_SH, t2, 13, 12, 10, steps=(4, 5))
    blob(g, SNOW_SH, (x + 10, y - 6, -6), 110, 90, 22)


def frost_birch(sc):
    _birch(sc, 0, 0, 760, 61, twigs=8)


def birch_copse(sc):
    _birch(sc, -230, 50, 720, 62, twigs=3, bands=3, rime=3)
    _birch(sc, 40, -60, 800, 63, twigs=3, rate=0.32, bands=3, rime=3)
    _birch(sc, 270, 70, 600, 64, twigs=3, rate=0.4, bands=3, rime=3)
    g = sc.still()
    blob(g, SNOW_SH, (20, 0, -10), 420, 190, 30)


def gold_larch(sc):
    """A larch in its yellow needles: airy tiers that sweep down, tassels hanging off their rims, the trunk showing
    between them."""
    h = 1020.0
    r = random.Random(71)
    g = sc.still()
    g.cyl(WOOD_D, (0, 0, -220), 27, 220 + h * 0.5, steps=6, top_r=15)
    piv = (0, 0, 0)
    up = sc.part(sway(1.6, piv, 0.28, 0.0))
    up.cyl(WOOD_D, (0, 0, h * 0.5), 15, h * 0.46, steps=6, top_r=5)
    for i in range(6):
        t = i / 5.0
        z = h * (0.2 + 0.125 * i)
        ra = 265 * (1.0 - 0.7 * t)
        m = sc.part(sway(1.4 + 2.6 * t, piv, 0.28, 0.05 * i))
        m.cyl(YEL if i % 2 == 0 else LIT, (0, 0, z), ra, h * 0.085, steps=8, top_r=ra * 0.16)
        m.cyl(YEL, (0, 0, z - 16), ra * 0.55, 17, steps=8, top_r=ra * 0.97)
        for k in range(5):
            a = math.radians(k * 72 + i * 31)
            px, py = math.cos(a) * ra * 0.86, math.sin(a) * ra * 0.86
            m.box(YEL if k % 2 else LIT, (px, py, z - 32), (15, 15, r.uniform(54, 82)), rot=(0, math.degrees(a), 0))
        if i % 2 == 1:
            blob(m, SNOW, (ra * 0.3, ra * 0.2, z + h * 0.05), ra * 0.36, ra * 0.3, 12)
    m = sc.part(sway(4.6, piv, 0.28, 0.33))
    m.cyl(YEL, (0, 0, h * 0.9), 46, h * 0.16, steps=6, top_r=3)
    blob(g, SNOW_SH, (10, 0, -8), 200, 170, 28)
    for k in range(5):                                                  # shed needles on the snow
        a = math.radians(k * 72 + 20)
        blob(g, YEL, (math.cos(a) * 150, math.sin(a) * 120, 16), 30, 24, 5, steps=(4, 5))


def rowan(sc):
    """A bare rowan: an umbrella of dark limbs, heavy bunches of red berries, snow in every fork."""
    r = random.Random(81)
    g = sc.still()
    g.cyl(WOOD_D, (0, 0, -200), 20, 400, steps=6, top_r=14)
    blob(g, SNOW_SH, (0, 0, -8), 170, 140, 26)
    piv = (0, 0, 0)
    lim = sc.part(sway(2.0, piv, 0.4, 0.0))
    ber = sc.part(sway(3.3, piv, 0.4, 0.1))
    fork = (0, 0, 195)
    blob(lim, SNOW, (0, 0, 212), 34, 30, 14)
    for j in range(6):
        a = math.radians(j * 60 + r.uniform(-14, 14))
        R1, z1 = r.uniform(120, 160), r.uniform(360, 420)
        e = (math.cos(a) * R1, math.sin(a) * R1, z1)
        rod(lim, WOOD_D, fork, e, 10, steps=5)
        blob(lim, SNOW, (e[0] * 0.55, e[1] * 0.55, 195 + (z1 - 195) * 0.55 + 12), 30, 22, 10, steps=(4, 5))
        for k in range(2):
            b = a + math.radians((k * 2 - 1) * 34 + r.uniform(-10, 10))
            R2, z2 = R1 + r.uniform(70, 120), z1 + r.uniform(60, 140)
            e2 = (math.cos(b) * R2, math.sin(b) * R2, z2)
            tilt_box(lim, WOOD_D, e, e2, 8, 7)
            hang = (e2[0], e2[1], e2[2] - 30)
            ber.box(WOOD_D, (hang[0], hang[1], e2[2] - 13), (5, 5, 30))
            blob(ber, RED, hang, 27, 27, 21, steps=(4, 6))
            blob(ber, RED, (hang[0] + 16, hang[1] - 12, hang[2] - 16), 18, 18, 15, steps=(4, 5))
            if (j + k) % 3 == 0:
                blob(lim, SNOW, (e2[0], e2[1], e2[2] + 9), 22, 20, 9, steps=(4, 5))
    for k in range(4):                                                  # fallen berries
        blob(g, RED, (r.uniform(-130, 130), r.uniform(-110, 110), 18), 12, 12, 8, steps=(4, 5))


def alpine_rose(sc):
    """LOW: cushions of alpine rose pushing through the snow, in flower."""
    r = random.Random(91)
    g = sc.still()
    for n, (cx, cy) in enumerate(((-170, 15), (0, -30), (165, 25))):
        blob(g, SNOW_SH, (cx, cy + 20, -4), 118, 96, 26)
        lf = sc.part(sway(2.4, (cx, cy, 0), 0.6, 0.3 * n))
        fl = sc.part(sway(4.6, (cx, cy, 0), 0.6, 0.3 * n + 0.08))
        for k in range(2):
            ox, oy = cx + (k * 2 - 1) * 38, cy + r.uniform(-16, 16)
            blob(lf, PINE_D if k else PINE, (ox, oy, 32), 62, 52, 44)
            for q in range(4):
                a = math.radians(q * 90 + r.uniform(0, 70))
                d = r.uniform(0.15, 0.7)
                fx, fy = ox + math.cos(a) * 62 * d, oy + math.sin(a) * 52 * d
                fz = 32 + 44 * math.sqrt(max(0.0, 1 - d * d))
                blob(fl, PINK, (fx, fy, fz + 3), 15, 15, 10, steps=(4, 5))
        blob(lf, SNOW, (cx - 10, cy + 30, 60), 44, 30, 10)


# ------------------------------------------------------------------------------------------ the spa resort
def blue_spruce(sc):
    """A formal blue spruce in a cream tub, clipped in five strict tiers, small warm lamps in it (steady)."""
    g = sc.still()
    g.cyl(CREAM, (0, 0, -160), 98, 232, steps=8, top_r=106)
    g.cyl(BLUE, (0, 0, 58), 112, 16, steps=8)
    g.cyl(BLUE, (0, 0, 0), 104, 12, steps=8)
    g.cyl(WOOD, (0, 0, -150), 122, 152, steps=8)                      # the timber plinth it stands on
    blob(g, SNOW, (0, 0, 76), 92, 92, 12)
    g.cyl(WOOD_D, (0, 0, 70), 15, 120, steps=6)
    piv = (0, 0, 72)
    for i in range(5):
        t = i / 4.0
        z = 150 + 100 * i
        ra = 215 * (1.0 - 0.17 * i)
        m = sc.part(sway(0.9 + 1.5 * t, piv, 0.3, 0.05 * i))
        m.cyl(CUT_D if i % 2 == 0 else CUT, (0, 0, z), ra, 150, steps=8, top_r=ra * 0.3)
        m.cyl(SNOW, (0, 0, z - 1), ra * 1.03, 9, steps=8, top_r=ra * 0.9)       # a clean ring of snow on the clipped edge
        for k in range(2):
            a = math.radians(k * 180 + i * 67 + 20)
            lx, ly = math.cos(a) * ra * 0.87, math.sin(a) * ra * 0.87
            m.box(LIT, (lx, ly, z + 30), (15, 15, 15), rot=(0, 45 + i * 20, 0), glow=0.9)
    m = sc.part(sway(2.8, piv, 0.3, 0.3))
    m.cyl(CUT, (0, 0, 640), 40, 110, steps=6, top_r=4)
    m.cyl(BLUE, (0, 0, 742), 7, 46, steps=5)                          # a plain finial
    blob(m, SNOW, (0, 0, 742), 15, 15, 9, steps=(4, 5))


def juniper_cone(sc):
    """A clipped juniper spiral in a timber tub with a blue band; snow lies on every turn."""
    g = sc.still()
    g.cyl(WOOD, (0, 0, -160), 66, 222, steps=8, top_r=74)
    g.cyl(BLUE, (0, 0, 30), 77, 14, steps=8)
    g.cyl(WOOD_D, (0, 0, 60), 80, 8, steps=8)
    g.cyl(WOOD_D, (0, 0, 60), 11, 60, steps=6)
    piv = (0, 0, 62)
    for i in range(6):
        t = i / 5.0
        a = math.radians(i * 70)
        ra = 96 * (1.0 - 0.13 * i)
        z = 105 + i * 50
        m = sc.part(sway(1.0 + 1.6 * t, piv, 0.35, 0.05 * i))
        cx, cy = math.cos(a) * 14, math.sin(a) * 14
        m.cyl(PINE_D if i % 2 == 0 else PINE, (cx, cy, z), ra, 54, steps=8, top_r=ra * 0.74)
        m.cyl(SNOW, (cx, cy, z + 54), ra * 0.74, 7, steps=8, top_r=ra * 0.6)
    m = sc.part(sway(3.0, piv, 0.35, 0.3))
    blob(m, PINE, (0, 0, 432), 26, 26, 30, steps=(4, 6))
    blob(m, SNOW, (0, 0, 458), 16, 16, 8, steps=(4, 5))


def juniper_hedge(sc):
    """LOW: six metres of clipped juniper hedge between two clipped balls, snow along the top, a timber kerb."""
    r = random.Random(111)
    g = sc.still()
    g.box(WOOD_D, (0, 0, -24), (640, 110, 60))
    g.box(PINE_D, (0, 0, 40), (520, 80, 72))
    g.box(PINE, (0, 0, 80), (506, 56, 12))
    g.box(SNOW, (0, 0, 89), (480, 42, 8))
    for sx in (-1, 1):
        blob(g, PINE, (sx * 286, 0, 46), 46, 46, 46, steps=(5, 7))
        blob(g, SNOW, (sx * 286, 0, 90), 28, 28, 8, steps=(4, 5))
        g.box(WOOD, (sx * 326, 0, 12), (14, 122, 36))
    for n in range(3):                                                  # the shoots the shears missed: they stir
        m = sc.part(sway(9.0, (-170 + n * 170, 0, 76), 0.9, 0.31 * n))
        for k in range(4):
            x = -170 + n * 170 + r.uniform(-70, 70)
            m.cyl(PINE, (x, r.choice((-34, 34)), 66), 7, r.uniform(20, 30), steps=5, top_r=3)
    for k in range(5):                                                  # berries along the front
        blob(g, ICE_DEEP, (-200 + k * 100 + r.uniform(-20, 20), -41, r.uniform(36, 64)), 7, 5, 7, steps=(4, 5))


def rose_boxes(sc):
    """LOW: the resort's flower boxes: alpine rose in a long timber box with a blue band, between two cream posts."""
    r = random.Random(121)
    g = sc.still()
    g.box(WOOD, (0, 0, -10), (470, 78, 110))
    g.box(BLUE, (0, -40, 26), (470, 5, 14))
    g.box(WOOD_D, (0, 0, 47), (486, 92, 8))
    for sx in (-1, 1):
        g.box(CREAM, (sx * 256, 0, -6), (40, 96, 150))
        g.cyl(BLUE, (sx * 256, 0, 69), 34, 24, steps=4, top_r=4, rot=(0, 45, 0))
        blob(g, SNOW, (sx * 256, 0, 72), 20, 20, 7, steps=(4, 5))
    for n in range(4):
        cx = -168 + n * 112
        lf = sc.part(sway(2.6, (cx, 0, 50), 0.7, 0.27 * n))
        fl = sc.part(sway(5.5, (cx, 0, 50), 0.7, 0.27 * n + 0.06))
        blob(lf, PINE_D if n % 2 else PINE, (cx, 0, 58), 56, 36, 30)
        for q in range(5):
            a = math.radians(q * 72 + r.uniform(0, 40))
            d = r.uniform(0.2, 0.75)
            blob(fl, PINK, (cx + math.cos(a) * 56 * d, math.sin(a) * 36 * d, 60 + 30 * math.sqrt(1 - d * d) + 2), 13, 13, 9, steps=(4, 5))
        tilt_box(lf, PINE, (cx + 30, -30, 58), (cx + 46, -50, 30), 9, 6)       # a spray hanging over the front
    blob(g, SNOW, (-60, 22, 52), 150, 16, 7)


# ------------------------------------------------------------------------------------------ the aurora ridge
def heather(sc):
    """LOW: violet heather between slate stones; every tuft leans with the wind."""
    r = random.Random(131)
    g = sc.still()
    _rock(g, -150, 40, 130, 100, 42, 132, ROCK_D, ROCK)
    _rock(g, 60, 50, 150, 110, 48, 133, ROCK_D, ROCK)
    _rock(g, 215, -30, 100, 90, 34, 134, ROCK_D, ROCK)
    blob(g, SNOW_SH, (0, 30, -8), 300, 120, 20)
    for n in range(7):
        cx, cy = -240 + n * 80 + r.uniform(-14, 14), -38 + r.uniform(-34, 30)
        m = sc.part(sway(7.5, (cx, cy, 6), 0.85, r.random()))
        blob(m, PINE_D, (cx, cy, 12), 34, 30, 22, steps=(4, 6))
        for k in range(5):
            a = math.radians(k * 72 + r.uniform(0, 50))
            m.cyl(LILAC, (cx + math.cos(a) * 16, cy + math.sin(a) * 14, 20), 9, r.uniform(36, 54), steps=5, top_r=3,
                  rot=(r.uniform(-12, 12), 0, r.uniform(-12, 12)))


def edelweiss(sc):
    """LOW: a clump of cream stars on a rock."""
    r = random.Random(141)
    g = sc.still()
    _rock(g, 30, 40, 270, 190, 62, 142)
    blob(g, SNOW, (60, 60, 52), 80, 60, 12)
    blob(g, SNOW_SH, (-20, -40, -8), 210, 110, 20)
    for n, (cx, cy) in enumerate(((-120, -50), (-10, -75), (110, -55))):
        m = sc.part(sway(6.5, (cx, cy, 6), 0.8, 0.3 * n))
        blob(m, CONC, (cx, cy, 10), 30, 24, 12, steps=(4, 6))                    # felted leaves
        for k in range(3 if n != 1 else 2):
            sx, sy = cx + (k - 1) * 30 + r.uniform(-6, 6), cy + r.uniform(-14, 14)
            L = r.uniform(48, 80)
            m.box(PINE, (sx, sy, L / 2.0 + 6), (5, 5, L))
            for q in range(4):
                m.box(CREAM, (sx, sy, L + 8), (40, 10, 5), rot=(0, q * 45, 0))
            blob(m, YEL, (sx, sy, L + 12), 8, 8, 5, steps=(4, 5))


def lichen_rock(sc):
    """LOW: a boulder with yellow and mint lichen and a cap of snow, dry grass in its lee."""
    r = random.Random(151)
    g = sc.still()
    cx, cy, cz, rx, ry, rz = 0.0, 0.0, 18.0, 150.0, 118.0, 66.0
    g.sphere(ROCK, (cx, cy, cz), 100, scale=(rx / 100, ry / 100, rz / 100), steps=(5, 7), rot=(0, 15, 0))
    _rock(g, 150, -60, 150, 120, 46, 152, ROCK_D, ROCK)
    blob(g, SNOW, (-10, 14, 78), 100, 78, 13)
    blob(g, SNOW_SH, (-40, 70, -8), 230, 120, 22)
    for k in range(12):
        th, ph = math.radians(-90 + (k - 5.5) * 24 + r.uniform(-8, 8)), math.radians(r.uniform(50, 82))
        n = (math.sin(ph) * math.cos(th), math.sin(ph) * math.sin(th), math.cos(ph))
        s = (cx + rx * n[0], cy + ry * n[1], cz + rz * n[2])
        rr = r.uniform(15, 27)
        rod(g, YEL if k % 3 else MINT, (s[0] - n[0] * 8, s[1] - n[1] * 8, s[2] - n[2] * 8), (s[0] + n[0] * 4, s[1] + n[1] * 4, s[2] + n[2] * 4), rr, steps=6)
    m = sc.part(sway(8.0, (-150, -70, 4), 0.8, 0.4))
    for k in range(6):
        a = math.radians(k * 60 + 10)
        L = r.uniform(46, 70)
        tilt_box(m, WOOD if k % 2 else LIT, (-150, -70, 2), (-150 + math.cos(a) * L * 0.4 + 10, -70 + math.sin(a) * L * 0.4, L * 0.9), 7, 5)


# ------------------------------------------------------------------------------------------ the glacier and the mine
def _fern(sc, cx, cy, fronds, s, seed):
    r = random.Random(seed)
    for f in range(fronds):
        yaw = f * 360.0 / fronds + r.uniform(-12, 12)
        mo = P.motion(swing=5.0, pivot=(cx, cy, 8), axis=yaw + 90.0, rate=0.6, phase=r.random())
        m = sc.part(mo, at=(cx, cy, 0), yaw=yaw, s=s)
        pts = ((8, 0, 8), (44, 0, 52), (84, 0, 78), (124, 0, 84), (156, 0, 70))
        th = max(5.0, 5.6 / s)                                          # 5 cm and more after the frond's own scale
        for k in range(4):
            tilt_box(m, TEAL_LIT, pts[k], pts[k + 1], 15 - k * 3, th, glow=0.8)
        for k in range(1, 4):
            w = 40 - k * 8
            for sy in (-1, 1):
                m.box(TEAL_LIT, (pts[k][0] + 6, sy * (w / 2.0 + 3), pts[k][2]), (9, w, th), rot=(0, sy * 35, 0), glow=0.8)


def frost_fern(sc):
    """LOW: fern-shaped growths of ice, lit teal from inside, on a crust of frost between dark stones."""
    g = sc.still()
    g.cyl(SNOW_SH, (30, 0, -30), 250, 34, steps=9)
    _rock(g, -210, 60, 120, 90, 40, 161, ROCK_D, ROCK)
    _rock(g, 250, -50, 100, 80, 32, 162, ROCK_D, ROCK)
    _fern(sc, -40, 0, 6, 1.0, 163)
    _fern(sc, 170, 30, 4, 0.72, 164)
    g.cyl(TEAL_LIT, (-40, 0, 0), 20, 14, steps=6, top_r=10, glow=0.8)
    g.cyl(TEAL_LIT, (170, 30, 0), 15, 11, steps=6, top_r=8, glow=0.8)


def ice_reed(sc):
    """LOW: frozen reeds and bulrushes round a melt pool."""
    r = random.Random(171)
    g = sc.still()
    g.cyl(ICE_DEEP, (0, 0, -44), 235, 48, steps=10)
    g.cyl(ICE_D, (40, 20, -40), 120, 48, steps=8)
    for k in range(5):
        a = math.radians(k * 72 + 30)
        blob(g, SNOW_SH if k % 2 else SNOW, (math.cos(a) * 240, math.sin(a) * 235, 0), 90, 70, 20)
    for n, (cx, cy) in enumerate(((-150, -120), (60, -175), (190, -40), (-190, 90))):
        m = sc.part(sway(6.0, (cx, cy, 2), 0.75, 0.23 * n))
        for k in range(5):
            sx, sy = cx + r.uniform(-40, 40), cy + r.uniform(-30, 30)
            L = r.uniform(62, 94)
            m.box(WOOD, (sx, sy, L / 2.0), (5, 5, L))
            if k < 3:
                m.cyl(WOOD_D, (sx, sy, L - 30), 8, 24, steps=5)
                if k == 0:
                    blob(m, SNOW_SH, (sx, sy, L - 2), 7, 7, 5, steps=(4, 5))
            else:
                tilt_box(m, WOOD, (sx, sy, L * 0.6), (sx + 26, sy + 8, L * 0.3), 6, 5)       # a broken blade


def glow_moss(sc):
    """LOW: cushions of glowing mint moss on a dark rock, spore stalks trembling over them."""
    r = random.Random(181)
    g = sc.still()
    cx, cy, cz, rx, ry, rz = 0.0, 0.0, 6.0, 160.0, 115.0, 64.0
    g.sphere(NET, (cx, cy, cz), 100, scale=(rx / 100, ry / 100, rz / 100), steps=(5, 7), rot=(0, 25, 0))
    _rock(g, 170, 40, 150, 110, 44, 182, NET, ROCK_D)
    blob(g, SNOW_SH, (-30, 60, -8), 240, 110, 20)
    st = sc.part(sway(9.0, (0, 0, 60), 1.1, 0.2))
    for k in range(7):
        th, ph = math.radians(-90 + (k - 3) * 38 + r.uniform(-10, 10)), math.radians(r.uniform(18, 62))
        n = (math.sin(ph) * math.cos(th), math.sin(ph) * math.sin(th), math.cos(ph))
        s = (cx + rx * n[0], cy + ry * n[1], cz + rz * n[2])
        rr = r.uniform(24, 40)
        blob(g, MINT, s, rr, rr * 0.85, 11, steps=(4, 6), glow=0.5)
        if k % 2 == 0:
            for q in range(3):
                st.cyl(TEAL_LIT, (s[0] + (q - 1) * 11, s[1] + r.uniform(-8, 8), s[2] + 6), 5, r.uniform(14, 20), steps=5, top_r=3, glow=0.5)


JOBS = [
    ('SM_S1PLANTS_SnowFir', snow_fir, TALL), ('SM_S1PLANTS_FirGrove', fir_grove, TALL), ('SM_S1PLANTS_StormFir', storm_fir, TALL),
    ('SM_S1PLANTS_DwarfPine', dwarf_pine, LOW), ('SM_S1PLANTS_SnowGrass', snow_grass, LOW),
    ('SM_S1PLANTS_FrostBirch', frost_birch, TALL), ('SM_S1PLANTS_BirchCopse', birch_copse, TALL), ('SM_S1PLANTS_GoldLarch', gold_larch, TALL),
    ('SM_S1PLANTS_Rowan', rowan, TALL), ('SM_S1PLANTS_AlpineRose', alpine_rose, LOW),
    ('SM_S1PLANTS_BlueSpruce', blue_spruce, TALL), ('SM_S1PLANTS_JuniperCone', juniper_cone, TALL),
    ('SM_S1PLANTS_JuniperHedge', juniper_hedge, LOW), ('SM_S1PLANTS_RoseBoxes', rose_boxes, LOW),
    ('SM_S1PLANTS_Heather', heather, LOW), ('SM_S1PLANTS_Edelweiss', edelweiss, LOW), ('SM_S1PLANTS_LichenRock', lichen_rock, LOW),
    ('SM_S1PLANTS_FrostFern', frost_fern, LOW), ('SM_S1PLANTS_IceReed', ice_reed, LOW), ('SM_S1PLANTS_GlowMoss', glow_moss, LOW),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never build_core(rebuild=True)
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
