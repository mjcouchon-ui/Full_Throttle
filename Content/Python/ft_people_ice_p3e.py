"""FULL THROTTLE (Unreal) the population of level 5, Ice Mountain, pass 3, builder p3e: FAR AND SKY
(/Game/People/IceMountain/SM_P3E_*).

What lives 150 m and further from the road and in the air over it. Everything is drawn two to five times life size,
in hot colours on white, and saved with far=True (the always-moving master). Every piece is TALL: none is written
nearer than 150 m. The table block is C:/ftwork/p3/L5e.inl.

  the sky     SM_P3E_Balloons_A     three hot-air balloons drifting round, two people waving from each basket
              SM_P3E_Balloons_B     two great balloons, three aboard each, a streamer of colour blocks under the basket
              SM_P3E_HangGliders_A  six delta-wing hang gliders wheeling, a pilot hanging under each
              SM_P3E_Geese_A        two skeins of geese in a V, flying along the road and coming round again
              SM_P3E_Choughs_A      twelve mountain birds (yellow and lime since the fix round: not black) swirling as a flock
  the slopes  SM_P3E_SnowKites_A    three snow-kiters riding a bench of snow, each under an arc kite 30 m up
              SM_P3E_SnowKites_B    two more going the other way; one of them jumps
              SM_P3E_FarCrowd_A     a crowd on a terraced bank over a wall of colour blocks, hopping, flags over them
              SM_P3E_FarCrowd_B     a gallery on stilts: one long row doing a wave from end to end
              SM_P3E_SkidooTrain_A  five snowmobiles, two up, running a causeway from one snow shed to the other
              SM_P3E_Skaters_A      a rink on a terrace: nine skaters lapping it, two spinning in the middle
  the edge    SM_P3E_RidgeTrek_A    a caravan of ski tourers with packs walking a snow crest, a pulk sled, a cairn

The hill falls 7 in 100 along the road and a piece stands level, so every ground piece carries its people on a deck
of its own well above z = 0 and has a foot 7 to 9 m below it: a long piece is neither buried at its uphill end nor
in the air at its downhill end.

Rules kept: nothing glows or blinks; no words or marks (colour blocks and stripes only); nobody big, white and furry
(the boss is THE ABOMINABLE): every person wears a hot colour, every creature is a bird (the geese dark, the circling flock yellow: a dark bird circling 170 m off read as the enemy chopper). Travel runs along the
piece's X (along the road, 150 m and more from it), never towards it.

Run in the editor:  import ft_people_ice_p3e as K; K.run()        (or K.run(['SM_P3E_Geese_A']))
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/IceMountain'

SNOW, SNOW_SH, SNOW_D, ICE, ICE_D = '#f4f9ff', '#cdd6f1', '#a9b8e0', '#bfe3ff', '#7fb0e0'
ROCK, ROCK_D, ROCK_DD = '#6a7488', '#4a5268', '#343a4e'
RED, BLUE, ORANGE, PINK, TEAL, YEL, LIME, VIOLET = '#ff2d3c', '#2e6bff', '#ff8a2a', '#ff4fb8', '#19b5c9', '#ffd23f', '#b8ff5a', '#8f6bff'
DARK, WHITE, WOOD, STEEL = '#22202a', '#fff6ea', '#9a6a44', '#5a6478'
SUITS = (RED, ORANGE, YEL, PINK, LIME, '#2ee6ff', BLUE, VIOLET)


def suit(r, s, **fixed):
    """Somebody in a ski suit of one hot colour, head to foot; the hair's colour reads as a hat from far off."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear='trousers', top=c, bottom=c, shoes=DARK, hair='short', hair_col=r.choice(SUITS), s=s)
    d.update(fixed)
    return d


def mot(x=0.0, y=0.0, travel=None, orbit=None, **kw):
    """P.motion with figure()'s own words: travel=(length cm, speed cm/s) from (x, y), or orbit=(cx, cy, rounds/s)."""
    if travel:
        kw['base'] = (x, y)
        kw['travel'] = (float(travel[0]), float(travel[1]) / float(travel[0]))
    elif orbit:
        kw['base'] = (orbit[0], orbit[1])
        kw['travel'] = (-1.0, float(orbit[2]))
    return P.motion(**kw)


def blob(sc, r, x, y, z, s, yaw=0.0, arms=True, hat=False, top=None, **mo):
    """A person of a far crowd in five or six boxes (60 to 72 triangles): legs, body, head, two arms up, a hat."""
    top = top or r.choice(SUITS)
    mo.setdefault('pivot', (x, y, z))
    m = sc.part(mot(x, y, **mo), (x, y, z), yaw, s)
    m.box(r.choice((DARK, BLUE, '#2b2350', top)), (0, 0, 33), (38, 26, 66))
    m.box(top, (0, 0, 96), (50, 30, 60))
    m.box(r.choice(P.SKINS), (0, 0, 146), (40, 38, 40))
    if hat:
        m.box(r.choice(SUITS), (0, 2, 172), (44, 42, 16))
    if arms:
        tilt_box(m, top, (-30, 0, 118), (-52, 0, 182), 13)
        tilt_box(m, top, (30, 0, 118), (52, 0, 182), 13)


def flag(sc, x, y, z, h, w, col, col2=None, ph=0.0, thick=30.0):
    """A pole from z up h, and a cloth of plain colour (a block of a second colour at the fly) that swings about it."""
    g = sc.still()
    g.box(DARK, (x, y, z + h / 2.0), (thick, thick, h))
    fl = sc.part(P.motion(swing=16.0, pivot=(x, y, z + h * 0.8), axis=P.UP, rate=0.8, phase=ph))
    fl.box(col, (x + w / 2.0, y, z + h * 0.8), (w, thick * 0.7, w * 0.55))
    if col2:
        fl.box(col2, (x + w * 0.84, y - thick * 0.4, z + h * 0.8), (w * 0.32, thick * 0.5, w * 0.55))


# ------------------------------------------------------------------------------------------ the sky
def balloon(sc, r, cx, cy, cz, R, cols, w, ph, crew=2, streamer=None):
    """A hot-air balloon whose basket floor is at cz: an envelope of radius R in bands of colour, a basket, people in
    it. It floats up and down and circles the piece's middle, w rounds a second."""
    base = dict(phase=ph, rate=0.12, bob=-0.22 * R)
    m = sc.part(mot(orbit=(0.0, 0.0, w), pivot=(cx, cy, cz), **base))
    z = cz + 0.95 * R
    for i, (a, b, h) in enumerate(((0.26, 0.66, 0.5), (0.66, 0.96, 0.5), (0.96, 1.0, 0.36), (1.0, 0.78, 0.36), (0.78, 0.24, 0.3))):
        m.cyl(cols[i % len(cols)], (cx, cy, z), a * R, h * R, steps=10, top_r=b * R)
        z += h * R
    m.box(WOOD, (cx, cy, cz + 0.13 * R), (0.36 * R, 0.36 * R, 0.26 * R))
    m.box(DARK, (cx, cy, cz + 0.27 * R), (0.4 * R, 0.4 * R, 0.04 * R))
    m.box(DARK, (cx, cy, cz + 0.62 * R), (0.12 * R, 0.12 * R, 0.1 * R))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, DARK, (cx + sx * 0.17 * R, cy + sy * 0.17 * R, cz + 0.27 * R), (cx + sx * 0.22 * R, cy + sy * 0.22 * R, cz + 0.97 * R), 0.035 * R)
    s = R / 375.0
    for k in range(crew):
        P.figure(sc, cx + (k - (crew - 1) / 2.0) * 0.17 * R, cy - 0.07 * R, cz + 0.03 * R, detail=0, arm_l='down', arm_r='wave', swing_r=28,
                 orbit=(0.0, 0.0, w), **dict(suit(r, s), **base))
    if streamer:
        st = sc.part(mot(orbit=(0.0, 0.0, w), pivot=(cx, cy, cz), swing=7.0, axis=0.0, **base))
        for k, col in enumerate(streamer):
            st.box(col, (cx, cy, cz - (k + 0.5) * 0.3 * R), (0.3 * R, 0.035 * R, 0.28 * R))


def balloons_a(sc):
    r = random.Random(31)
    balloon(sc, r, -2600, 400, 4500, 850, (PINK, YEL, PINK, TEAL, PINK), 0.006, 0.0)
    balloon(sc, r, 3400, 3600, 7400, 1000, (ORANGE, WHITE, RED, WHITE, ORANGE), -0.0045, 0.35)
    balloon(sc, r, 1500, -6200, 5600, 800, (TEAL, LIME, VIOLET, LIME, TEAL), 0.0038, 0.7)


def balloons_b(sc):
    r = random.Random(32)
    balloon(sc, r, -3600, 1200, 5200, 1150, (YEL, RED, YEL, BLUE, YEL), 0.005, 0.2, crew=3, streamer=(PINK, WHITE, TEAL, WHITE, PINK))
    balloon(sc, r, 4400, -2600, 8200, 950, (VIOLET, PINK, WHITE, PINK, VIOLET), -0.004, 0.6, crew=3, streamer=(YEL, ORANGE, YEL, ORANGE))


def glider(sc, r, a_deg, Rr, z, w, span, col, col2, ph):
    """A delta-wing hang glider circling the piece's middle at radius Rr, its pilot hanging prone under the keel."""
    a = math.radians(a_deg)
    x, y = Rr * math.cos(a), Rr * math.sin(a)
    hdeg = a_deg + (90.0 if w > 0 else -90.0)
    hd = math.radians(hdeg)
    tx, ty = math.cos(hd), math.sin(hd)
    m = sc.part(mot(orbit=(0.0, 0.0, w), phase=ph, rate=0.22, swing=10.0, bob=-120.0, pivot=(x, y, z), axis=hdeg))
    rx, ry = x + tx * span * 0.14, y + ty * span * 0.14
    Lh = span * 0.56
    for side in (-1, 1):
        yaw = hdeg + side * 116.0
        dx, dy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
        m.box(col, (rx + dx * Lh * 0.5, ry + dy * Lh * 0.5, z + span * 0.03), (Lh, span * 0.3, 14.0), rot=(0, yaw, 0))
        m.box(col2, (rx + dx * Lh * 0.72, ry + dy * Lh * 0.72, z + span * 0.03 + 8.0), (Lh * 0.3, span * 0.31, 14.0), rot=(0, yaw, 0))
    tilt_box(m, DARK, (x + tx * span * 0.3, y + ty * span * 0.3, z + span * 0.035), (x - tx * span * 0.28, y - ty * span * 0.28, z + span * 0.035), 16.0)
    px, py, pz = x - tx * span * 0.02, y - ty * span * 0.02, z - span * 0.11
    sc_col = r.choice(SUITS)
    m.box(sc_col, (px, py, pz), (span * 0.13, span * 0.05, span * 0.042), rot=(0, hdeg, 0))
    m.box(r.choice(P.SKINS), (px + tx * span * 0.085, py + ty * span * 0.085, pz + span * 0.01), (span * 0.04, span * 0.04, span * 0.04), rot=(0, hdeg, 0))
    m.box(DARK, (px - tx * span * 0.11, py - ty * span * 0.11, pz), (span * 0.1, span * 0.04, span * 0.032), rot=(0, hdeg, 0))
    tilt_box(m, DARK, (px, py, pz + span * 0.02), (x, y, z + span * 0.03), 12.0)
    m.box(DARK, (px + tx * span * 0.06, py + ty * span * 0.06, pz - span * 0.03), (12.0, span * 0.14, 12.0), rot=(0, hdeg, 0))


def hang_gliders(sc):
    r = random.Random(33)
    wings = ((RED, WHITE), (YEL, VIOLET), (PINK, TEAL), (ORANGE, BLUE), (LIME, PINK), (TEAL, YEL))
    for i, (Rr, z, w) in enumerate(((2600, 3600, 0.03), (4200, 5200, -0.024), (5600, 6800, 0.02), (3400, 8200, -0.028), (6000, 4400, 0.018), (1800, 6000, 0.034))):
        glider(sc, r, i * 67.0, Rr, z, w, 1500.0, wings[i][0], wings[i][1], r.random())


def geese(sc):
    """Two skeins in a V, flying +X along a line of 300 m and coming round again; the second half a line behind."""
    r = random.Random(34)
    L, v = 30000.0, 1200.0
    for x0, z0, n in ((0.0, 4600.0, 4), (-15000.0, 5800.0, 2)):
        for k in range(-n, n + 1):
            i = abs(k)
            P.bird(sc, x0 - i * 680.0, k * 540.0, z0 + i * 45.0, heading=0.0, span=520, body='#6a5a4a', wing='#2a2230', beak=ORANGE,
                   phase=r.random(), rate=1.5, flap=30, travel=(L, v))


def choughs(sc):
    r = random.Random(35)
    for i in range(12):
        Rr = 700.0 + (i * 613) % 2300
        w = (0.07 + 0.006 * (i % 5)) * (1 if i % 3 else -1)
        a_deg = i * 73.0
        a = math.radians(a_deg)
        P.bird(sc, Rr * math.cos(a), Rr * math.sin(a), 2600.0 + (i * 417) % 2400, heading=a_deg + (90 if w > 0 else -90), span=380,
               body=YEL, wing=LIME, beak=RED, phase=r.random(), rate=2.3, flap=34, orbit=(0.0, 0.0, w))
    # (fix round, 16 Nov: these were black. In the game a black bird 3.8 m across, 170 m off, has the size and tone of
    # the enemy chopper on the horizon, and the flock circled round it during the CHOPPER! warning: now yellow and lime.)


# ------------------------------------------------------------------------------------------ the slopes
def snow_kites(seed, d, n, jump):
    """Snow-kiters riding a level bench of snow 6 m over the piece's foot, d = 1 towards +X, -1 towards -X. Each is
    3.4 times life size under an arc kite 14 m across, 24 to 34 m over the bench, on two lines."""
    def build(sc):
        r = random.Random(seed)
        L, v, Zb = 12000.0, 850.0, 600.0
        g = sc.still()
        g.box(SNOW_SH, (0, 0, (Zb - 700.0) / 2.0), (L + 2600.0, 1700.0, Zb + 700.0))
        g.box(SNOW, (0, 0, Zb - 10.0), (L + 2700.0, 1800.0, 40.0))
        g.box(ORANGE, (0, -862.0, Zb - 170.0), (L + 2000.0, 24.0, 130.0))          # safety netting: the bench's line reads from far off
        for sx in (-1, 1):
            g.cyl(SNOW, (sx * (L / 2.0 + 250.0), 0, Zb - 100.0), 800.0, 900.0, steps=6, top_r=350.0)
        for k in range(6):
            flag(sc, -L / 2.0 + (k + 0.5) * L / 6.0, 720.0, Zb, 620.0, 360.0, (PINK, TEAL, YEL)[k % 3], ph=0.37 * k, thick=24.0)
        for i in range(n):
            bx = -L / 2.0 + (i + 0.5) * L / n + r.uniform(-300, 300)
            by = -350.0 + i * 330.0
            s = 3.4
            ph = r.random()
            high = bool(jump and i == 0)
            mv = dict(phase=ph, rate=0.22, bob=420.0) if high else dict(phase=ph, rate=0.9, bob=30.0)
            col = SUITS[(seed + i * 3) % len(SUITS)]
            up = ((118, 60), (128, 70))
            P.figure(sc, bx, by, Zb + 8.0 * s, yaw=90.0 * d, legs='wide', detail=1, arm_l=up, arm_r=up, travel=(L, d * v),
                     **dict(suit(r, s, top=col, bottom=col), **mv))
            b = sc.part(mot(bx, by, travel=(L, d * v), pivot=(bx, by, Zb), **mv), (bx, by, Zb), 90.0 * d, s)
            b.box(SUITS[(seed + i * 3 + 4) % len(SUITS)], (0, 0, 4), (36, 175, 8))
            kx, ky, kz = bx + d * 1500.0, by + 500.0, Zb + 2400.0 + i * 450.0
            kmv = mv if high else dict(phase=ph, rate=0.3, bob=-120.0)
            km = sc.part(mot(bx, by, travel=(L, d * v), pivot=(bx, by, Zb + 350.0), swing=9.0, axis=0.0, **kmv))
            kc = (col, WHITE)
            for j in range(-2, 3):
                km.box(kc[j % 2], (kx, ky + j * 270.0, kz - j * j * 75.0), (430.0, 285.0, 18.0), rot=(0, 0, j * 16.0))
            for sy in (-1, 1):
                tilt_box(km, DARK, (bx + d * 60.0, by, Zb + 420.0), (kx, ky + sy * 560.0, kz - 320.0), 22.0)
    return build


def far_crowd_a(sc):
    """A terraced bank: a wall of colour blocks 10 m tall, three snow steps on it, 24 people three times life size
    hopping out of step with their arms up, three flags behind them."""
    r = random.Random(36)
    g = sc.still()
    g.box(ROCK_D, (0, 150, 200), (5600, 2000, 2000))                 # z -800 .. 1200
    for k in range(7):
        g.box((PINK, TEAL, YEL)[k % 3], (-2400 + k * 800, -862, 640), (700, 24, 900))
    for k in range(3):
        g.box((SNOW, SNOW_SH, SNOW)[k], (0, -500 + k * 480, 1220 + k * 320 - 150), (5700, 520, 300))
    for k in range(3):
        flag(sc, -2100 + k * 2100, 950, 1840, 1500, 760, (ORANGE, PINK, TEAL)[k], (WHITE, YEL, WHITE)[k], ph=0.3 * k, thick=34.0)
    for row in range(3):
        for i in range(8):
            x = -2380 + i * 680 + (170 if row % 2 else 0) + r.uniform(-60, 60)
            blob(sc, r, x, -500 + row * 480, 1220 + row * 320, 3.0, arms=(i + row) % 3 != 0, hat=(i + row) % 3 == 1,
                 phase=r.random(), rate=r.uniform(1.0, 1.5), bob=r.uniform(70, 150))


def far_crowd_b(sc):
    """A gallery on stilts, 70 m long, its deck 11 m up: 24 people in one row doing a wave from end to end; a rail
    of colour blocks under them, a flag at each end."""
    r = random.Random(37)
    g = sc.still()
    Ld, Zd = 7000.0, 1100.0
    g.box(STEEL, (0, 0, Zd - 30), (Ld + 200, 560, 60))
    for k in range(8):
        g.box(ROCK_DD, (-Ld / 2.0 + 300 + k * (Ld - 600) / 7.0, 60, (Zd - 800) / 2.0 - 30), (70, 70, Zd + 800))
    cols = (PINK, WHITE, TEAL, WHITE, YEL, WHITE, ORANGE, WHITE, VIOLET, WHITE)
    for k in range(10):
        g.box(cols[k], (-Ld / 2.0 + (k + 0.5) * Ld / 10.0, -292, Zd + 150), (Ld / 10.0 - 30, 24, 300))
    flag(sc, -Ld / 2.0 - 40, 0, Zd, 1500, 700, RED, WHITE, ph=0.1, thick=34.0)
    flag(sc, Ld / 2.0 - 760, 0, Zd, 1500, 700, BLUE, YEL, ph=0.6, thick=34.0)
    n = 24
    for i in range(n):
        x = -Ld / 2.0 + (i + 0.5) * Ld / n
        blob(sc, r, x, 20, Zd, 3.0, hat=i % 2 == 0, phase=1.0 - i / float(n), rate=0.45, bob=260.0)


def skidoo(sc, r, bx, by, z, S, col, L, v, ph):
    """A snowmobile, three times life size, two people on it, running the piece's X."""
    m = sc.part(mot(bx, by, travel=(L, v), pivot=(bx, by, z), phase=ph, rate=3.0, bob=10.0), (bx, by, z), 0.0 if v > 0 else 180.0, S)
    m.box(DARK, (-50, 0, 16), (170, 70, 32))
    m.box(col, (0, 0, 48), (260, 88, 44))
    m.box(col, (110, 0, 78), (90, 78, 36))
    m.box(ICE, (72, 0, 112), (12, 70, 52))
    m.box(DARK, (-45, 0, 80), (130, 58, 26))
    for sy in (-1, 1):
        m.box(DARK, (125, sy * 46, 8), (130, 14, 10))
    for k, px in enumerate((-10, -72)):
        top = r.choice(SUITS)
        m.box(top, (px, 0, 124), (40, 52, 64))
        m.box(r.choice(P.SKINS), (px, 0, 174), (36, 36, 36))
        m.box(r.choice(SUITS), (px, 0, 196), (40, 40, 14))
        if k == 0:
            tilt_box(m, top, (px + 10, 0, 140), (px + 70, 0, 118), 50, 14)


def skidoo_train(sc):
    """Five snowmobiles in convoy along a level causeway of snow, out of one snow shed and into the other; orange
    marker poles along its edge, a pennant on the last sled."""
    r = random.Random(38)
    L, v, H = 16000.0, 1500.0, 1400.0
    g = sc.still()
    g.side(SNOW, [(-L / 2.0 - 1700, -900), (-L / 2.0 - 500, H), (L / 2.0 + 500, H), (L / 2.0 + 1700, -900)], -480, 520)
    g.side(SNOW_D, [(-L / 2.0 - 2300, -900), (-L / 2.0 - 900, H * 0.6), (L / 2.0 + 900, H * 0.6), (L / 2.0 + 2300, -900)], -900, -480)
    for sx in (-1, 1):
        g.box(ROCK_D, (sx * (L / 2.0 + 60), 20, H + 330), (1300, 940, 700))
        g.box(SNOW, (sx * (L / 2.0 + 60), 20, H + 710), (1400, 1040, 70))
    for k in range(8):
        g.box(ORANGE, (-L / 2.0 + 1000 + k * 2000, -440, H + 170), (30, 30, 340))
    cols = (RED, YEL, PINK, TEAL, ORANGE)
    for i in range(5):
        bx = -L / 2.0 + (i + 0.5) * L / 5.0 + r.uniform(-250, 250)
        skidoo(sc, r, bx, 0.0, H, 3.0, cols[i], L, v, r.random())
    bx = -L / 2.0 + 0.5 * L / 5.0 - 900.0                      # a sled behind one of them, a pennant on it
    sl = sc.part(mot(bx, 0.0, travel=(L, v), pivot=(bx, 0, H)))
    sl.box(WOOD, (bx, 0, H + 70), (520, 220, 110))
    sl.box(DARK, (bx + 420, 0, H + 80), (360, 24, 24))
    sl.box(DARK, (bx - 200, 0, H + 420), (26, 26, 640))
    pn = sc.part(mot(bx, 0.0, travel=(L, v), swing=14.0, pivot=(bx - 200, 0, H + 620), axis=P.UP, rate=1.1))
    pn.box(PINK, (bx - 470, 0, H + 620), (520, 20, 280))


def skaters(sc):
    """A rink on a round terrace 52 m across, 9 m over the piece's foot, boards in hot colours round it: nine skaters
    3.2 times life size lapping it the same way round at their own speeds, two spinning in the middle."""
    r = random.Random(39)
    g = sc.still()
    Rk, Zk = 2600.0, 900.0
    g.cyl(STEEL, (0, 0, -800), Rk, Zk + 800.0, steps=10)
    g.cyl(ICE, (0, 0, Zk), Rk - 80.0, 24.0, steps=10)
    seg = 2.0 * Rk * math.sin(math.radians(18.0))
    for k in range(10):
        a = math.radians(k * 36.0 + 18.0)
        rr = Rk * math.cos(math.radians(18.0))
        g.box(SUITS[k % len(SUITS)], (rr * math.cos(a), rr * math.sin(a), Zk + 90.0), (seg, 36.0, 180.0), rot=(0, k * 36.0 + 108.0, 0))
    for i in range(9):
        Rr = 800.0 + (i * 380) % 1500
        w = 0.05 + 0.007 * ((i * 3) % 5)
        a_deg = i * 83.0
        a = math.radians(a_deg)
        P.figure(sc, Rr * math.cos(a), Rr * math.sin(a), Zk + 24.0, yaw=a_deg + 180.0, legs='stride', detail=0, stride=24, arm_l='out' if i % 4 == 0 else 'down',
                 arm_r='out' if i % 4 == 0 else 'down', swing_l=22, swing_r=-22, axis_l='x', axis_r='x', rate=1.0, phase=r.random(),
                 orbit=(0.0, 0.0, w), **suit(r, 3.2))
    for k, x in enumerate((-190.0, 210.0)):
        P.figure(sc, x, 40.0 * k, Zk + 24.0, detail=0, arm_l='up', arm_r='up', turn=175.0, rate=0.35, phase=0.5 * k, **suit(r, 3.0))


# ------------------------------------------------------------------------------------------ the edge
def ridge_trek(sc):
    """A snow crest 110 m long and 16 m high past the land's edge: nine ski tourers five times life size, packs on
    their backs, walking it in line and coming round again behind a rock horn at each end; two of them haul a pulk
    sled; a cairn with two flags in the middle of the crest."""
    r = random.Random(40)
    L, H, v = 11000.0, 1600.0, 300.0
    g = sc.still()
    g.side(SNOW, [(-L / 2.0 - 1500, -900), (-L / 2.0 - 300, H * 0.72), (-L / 2.0, H), (L / 2.0, H), (L / 2.0 + 300, H * 0.72), (L / 2.0 + 1500, -900)], -450, 550)
    g.side(ROCK_D, [(-L / 2.0 - 2300, -900), (-L / 2.0 - 700, H * 0.5), (L / 2.0 + 700, H * 0.5), (L / 2.0 + 2300, -900)], -900, -450)
    for sx in (-1, 1):
        g.cyl(ROCK_DD, (sx * (L / 2.0 + 250), 60, H - 300), 760.0, 1600.0, steps=5, top_r=240.0)
        g.cyl(SNOW, (sx * (L / 2.0 + 250), 60, H + 1290), 250.0, 90.0, steps=5, top_r=140.0)
    g.cyl(ROCK, (0, 380, H - 100), 330.0, 620.0, steps=5, top_r=170.0)
    flag(sc, -120, 380, H + 400, 1300, 640, PINK, TEAL, ph=0.2, thick=34.0)
    flag(sc, 160, 400, H + 400, 1000, 520, YEL, ph=0.7, thick=30.0)
    n = 9
    for i in range(n):
        bx = -L / 2.0 + (i + 0.5) * L / n + r.uniform(-120, 120)
        P.figure(sc, bx, r.uniform(-200, 0), H, yaw=90, detail=0, legs='stride', stride=22, swing_l=18, swing_r=-18, axis_l='x', axis_r='x',
                 rate=0.8, bob=14, phase=r.random(), travel=(L, v), **suit(r, 5.0, pack=r.choice(SUITS)))
        if i in (2, 6):                                          # the pulk: a sled on a trace behind this one
            sx = bx - 760.0
            sl = sc.part(mot(sx, -100.0, travel=(L, v), pivot=(sx, -100, H)))
            sl.box((ORANGE, RED)[i == 6], (sx, -100, H + 80), (560, 240, 150))
            sl.box(DARK, (sx + 400, -100, H + 110), (300, 22, 22))


BIG = dict(far=True, grow=(600.0, 300.0, 500.0), grow_down=(600.0, 300.0, 100.0))
JOBS = [
    ('SM_P3E_Balloons_A', balloons_a, dict(far=True, grow=(7600.0, 7600.0, 400.0), grow_down=(7600.0, 7600.0, 400.0))),
    ('SM_P3E_Balloons_B', balloons_b, dict(far=True, grow=(5600.0, 5600.0, 400.0), grow_down=(5600.0, 5600.0, 1800.0))),
    ('SM_P3E_HangGliders_A', hang_gliders, dict(far=True, grow=(6600.0, 6600.0, 300.0), grow_down=(6600.0, 6600.0, 300.0))),
    ('SM_P3E_Geese_A', geese, dict(far=True, grow=(15600.0, 300.0, 300.0), grow_down=(15600.0, 300.0, 300.0))),
    ('SM_P3E_Choughs_A', choughs, dict(far=True, grow=(3200.0, 3200.0, 300.0), grow_down=(3200.0, 3200.0, 300.0))),
    ('SM_P3E_SnowKites_A', snow_kites(41, 1, 3, False), BIG),
    ('SM_P3E_SnowKites_B', snow_kites(42, -1, 2, True), BIG),
    ('SM_P3E_FarCrowd_A', far_crowd_a, BIG),
    ('SM_P3E_FarCrowd_B', far_crowd_b, BIG),
    ('SM_P3E_SkidooTrain_A', skidoo_train, BIG),
    ('SM_P3E_Skaters_A', skaters, BIG),
    ('SM_P3E_RidgeTrek_A', ridge_trek, BIG),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
