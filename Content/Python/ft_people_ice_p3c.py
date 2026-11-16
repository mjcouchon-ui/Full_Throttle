"""FULL THROTTLE (Unreal) the population, level 5: Ice Mountain, pass 3, builder p3c: THE MOVERS.

Everything here TRAVELS: along the piece's X (which the placement lays along the road) and round again, in a ring
about a point of the piece, or down a hill of the piece's own. Nothing travels across: a line is never turned
towards the road by its rule, and no line as placed is longer than its own stretch of road (30 to 34 m as built).

This hill falls 7 in 100 ALONG the road and a piece stands level: so every line runs on the level crest of a snow bank
of the piece's own, footed 3 m into the ground (the first cast's dog sled does the same), and the rink stands on a
plinth of snow. People are 1.3 times life size (the cel pass inks a life-size figure 30 m off to a scribble).

  by the road (38.5 m, where the road has no banked bend)
    SM_P3C_SkiTrail_A     cross-country skiers in a classic track: three one way, two the other, poles going   30 m
    SM_P3C_SkiTrail_B     a skating pair, a parent towing a child in a pulk, a dog running with them           30 m
    SM_P3C_Snowshoe_A     a guided party on snowshoes with packs and poles, the guide's pennant, a big dog     28 m
    SM_P3C_FatBikes_A     fat-tyre bikes on a groomed track: three one way, one with a pennant the other       32 m
  the edge of the level snow (76 m, placed at 1.25)
    SM_P3C_Skidoos_A      three snowmobiles in convoy, pennants, the last towing a sled of crates              34 m
    SM_P3C_Skidoos_B      the patrol: two snowmobiles the other way, one towing a rescue sled                  34 m
    SM_P3C_Sleigh_A       two reindeer in tandem, a red sleigh of waving passengers, a skier towed behind      32 m
    SM_P3C_Skaters_A      a round rink on a snow plinth: eight skaters circling a flagged island, one spinning
    SM_P3C_Toboggan_A     a toboggan hill: five sledders come down 5 m and run out (for the right of the road)
    SM_P3C_TobogganL_A    its twin for the left
  the slope above (110 m, placed at 1.5; the always-moving master)
    SM_P3C_Herd_A         seven reindeer at a trot along a snow bank, a herder on a snowmobile, a dog          32 m
    SM_P3C_Kites_A        two snowkiters: a boarder and a skier hauled along under big bright kites            32 m
  the sky (80 to 130 m out, 24 m up; the always-moving master)
    SM_P3C_Geese_A        a skein of nine geese in a V, flying a 120 m line along the road

Rules kept here: nothing glows, nothing blinks, no words or marks, nothing big, white and furry on two legs (the boss
is THE ABOMINABLE), no pink chest band (that is the film crew's).

The placing rules: C:/ftwork/p3/L5c.inl (the block "p3c" of the level's table).
Run in the editor:  import ft_people_ice_p3c as K; K.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/IceMountain'

SNOW, SNOW_SH, ICE, ICE_D = '#f4f9ff', '#cdd6f1', '#bfe3ff', '#7fb0e0'
WOOD, WOOD_D, STEEL, STEEL_D = '#9a6a44', '#6a4c36', '#9aa4b8', '#5a6478'
RED, BLUE, ORANGE, PINK, TEAL, YEL, LIME, VIOLET = '#ff2d3c', '#2e6bff', '#ff8a2a', '#ff4fb8', '#19b5c9', '#ffd23f', '#b8ff5a', '#8f6bff'
DARK, WHITE, CREAM = '#22202a', '#fff6ea', '#f1e4c8'
PINE, PINE_D = '#2f6b52', '#234f40'
SUITS = (RED, '#ff5fa2', ORANGE, YEL, LIME, '#2ee6ff', BLUE, VIOLET, PINK, '#7dffb0')
LEGS = ('#3a5fa8', WHITE, VIOLET, TEAL)
DEER = ('#9a6a44', '#8e5a3a', '#b87a50', '#6a4c36')
BIG = 1.3            # the size of people in this kit
PZ = 12000.0         # how far under a sledder its swing's pivot lies (see ft_people_ice.py: DOWNHILL)


# ------------------------------------------------------------------------------------------ helpers
def suit(r, **fixed):
    """Somebody in a ski suit: a hot colour head to foot or over dark trousers, a helmet, a cap or a bobble hat."""
    c = r.choice(SUITS)
    d = P.look(r, sleeves='long', wear='trousers', top=c, bottom=r.choice((c, c) + LEGS), hat=r.choice(('cap', 'beanie', 'beanie', 'helmet')),
               hat_col=r.choice(SUITS), shoes=c, hair='short', s=BIG * r.uniform(0.95, 1.05))
    d.update(fixed)
    return d


def fig(sc, x, y, look, **kw):
    """P.figure with a look and the pose merged into one set of values (a value given twice would be an error)."""
    v = dict(look)
    v.update(kw)
    return P.figure(sc, x, y, **v)


def prop(sc, x, y, L, v, z=0.0, **kw):
    """The model for a thing that travels the piece's X with whoever is at (x, y): v cm/s (negative: towards -X),
    round a line L long. It is drawn in the mesh's own space, where it stands at the start."""
    return sc.part(P.motion(pivot=(x, y, z), base=(x, y), travel=(float(L), float(v) / float(L)), **kw))


def berm(sc, L, H, w=640.0, lanes=(), col=SNOW):
    """A snow bank along X with a level crest half its width, footed 3 m into the ground; a groomed band a lane."""
    g = sc.still(yaw=90.0)
    g.side(col, [(-w / 2.0, -300.0), (w / 2.0, -300.0), (w / 4.0, H), (-w / 4.0, H)], -L / 2.0, L / 2.0)
    g = sc.still()
    for y in lanes:
        g.box(SNOW_SH, (0, y, H + 1.0), (L - 60.0, 46.0, 4.0))
    return g


def pine(g, x, y, z, h=380.0):
    """A small pine with snow on it: what a traveller comes out from behind at the end of its line."""
    g.cyl(WOOD_D, (x, y, z - 40.0), 13.0, h * 0.3 + 40.0, steps=5)
    g.cyl(PINE, (x, y, z + h * 0.2), h * 0.3, h * 0.45, steps=6, top_r=h * 0.12)
    g.cyl(PINE_D, (x, y, z + h * 0.52), h * 0.2, h * 0.42, steps=6, top_r=5.0)
    g.cyl(SNOW, (x, y, z + h * 0.9), h * 0.06, h * 0.12, steps=5, top_r=2.0)


def ends(g, L, H, y=-175.0, poles=(RED, BLUE)):
    """A pine and a striped marker pole at each end of a line, on the bank's road-side shoulder."""
    for sx in (-1, 1):
        pine(g, sx * (L / 2.0 - 60.0), y, H - 50.0, 360.0)
        g.box(poles[sx > 0], (sx * (L / 2.0 - 170.0), y + 25.0, H + 30.0), (10, 10, 150))
        g.box(WHITE, (sx * (L / 2.0 - 170.0), y + 25.0, H + 70.0), (12, 12, 26))


def markers(g, L, H, y, n=7, cols=(ORANGE, PINK)):
    """Piste poles along the back of a line: orange and pink by turns."""
    for k in range(n):
        g.box(cols[k % 2], (-L / 2.0 + 220.0 + k * (L - 440.0) / float(n - 1), y, H + 50.0), (9, 9, 120))


def hold_pole(col=DARK):
    """A ski pole from the hand back to the snow (the figure's frame: +Y is behind it, z = 0 its feet)."""
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1], h[2] + 8.0), (h[0] + side * 4.0, h[1] + 40.0, 4.0), 4.0, 4.0)
        m.box(col, (h[0] + side * 4.0, h[1] + 38.0, 9.0), (12, 12, 3))
    return f


XC_ARM = ((42, 80), (72, 86))


def leg_part(sc, x, y, z, yaw, s, side, stride, ph, rate, bob, L, v):
    """The model that swings with one leg of a figure built by P.figure with these values (skis, snowshoes)."""
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    hx = side * 9.0
    piv = (x + s * hx * cy, y + s * hx * sy, z + s * 66.0)
    return sc.part(P.motion(phase=ph, rate=rate, bob=bob, swing=side * stride, pivot=piv, axis=yaw, base=(x, y), travel=(float(L), float(v) / float(L))),
                   (x, y, z), yaw, s)


def xc_skier(sc, r, x, y, d, L, v, z, look=None, stride=18.0, rate=1.1, swing=26.0, ski=None, detail=1):
    """A cross-country skier going d (+1: +X) along the piece's line: legs and skis stride, arms and poles swing."""
    look = look or suit(r)
    s, ph, yaw = look['s'], r.random(), 90.0 * d
    zf = z + 6.0 * s
    fig(sc, x, y, look, z=zf, yaw=yaw, legs='stride', stride=stride, arm_l=XC_ARM, arm_r=XC_ARM, swing_l=swing, swing_r=-swing, axis_l='x',
        axis_r='x', hold_l=hold_pole(), hold_r=hold_pole(), rate=rate, phase=ph, bob=2.0, travel=(L, v * d), detail=detail)
    col = ski or r.choice(SUITS)
    for side in (-1, 1):
        m = leg_part(sc, x, y, zf, yaw, s, side, stride, ph, rate, 2.0, L, v * d)
        m.box(col, (side * 9.0, -14.0 * side - 14.0, -2.5), (9, 150, 5))


def dog(sc, x, y, d, L, v, z, body='#6a7488', s=1.5, phase=0.0, big=False):
    if big:
        P.quadruped(sc, x, y, z=z, yaw=90.0 * d, s=s, length=96, height=62, girth=30, neck=18, neck_up=30, head=24, tail=26, wag=24, stride=22,
                    rate=1.6, bob=3, phase=phase, travel=(L, v * d), body=body, belly=WHITE, snout=WHITE, ears=WOOD_D)
    else:
        P.quadruped(sc, x, y, z=z, yaw=90.0 * d, s=s, length=74, height=50, girth=22, neck=16, neck_up=35, head=20, tail=24, wag=30, stride=30,
                    rate=2.4, bob=5, phase=phase, travel=(L, v * d), body=body, belly=WHITE, snout=WHITE, ears=DARK)


def reindeer(sc, r, x, y, d, L, v, z, s=1.3, rate=2.0, bob=5.0, col=None):
    col = col or r.choice(DEER)
    P.quadruped(sc, x, y, z=z, yaw=90.0 * d, s=s, length=120, height=100, girth=36, neck=38, neck_up=42, head=30, tail=10, horns=CREAM,
                body=col, belly=CREAM, snout=DARK, ears=col, legs_col=WOOD_D, stride=28, rate=rate, bob=bob, phase=r.random(), travel=(L, v * d))


def skidoo(sc, r, x, y, d, L, v, z, col, k=1.25, flag=None, look=None, wave=False):
    """A snowmobile heading d along the piece's line, its rider, a pennant on a whip, snow thrown up behind."""
    m = prop(sc, x, y, L, v * d, z)
    m.box(col, (x, y, z + 46 * k), (230 * k, 84 * k, 44 * k))
    m.box(WHITE, (x + d * 60 * k, y, z + 58 * k), (60 * k, 86 * k, 12 * k))
    m.box(DARK, (x - d * 20 * k, y, z + 76 * k), (120 * k, 60 * k, 16 * k))
    m.box(DARK, (x - d * 20 * k, y, z + 14 * k), (200 * k, 60 * k, 28 * k))
    tilt_box(m, ICE_D, (x + d * 70 * k, y, z + 68 * k), (x + d * 94 * k, y, z + 114 * k), 70 * k, 5)
    m.box(STEEL_D, (x + d * 62 * k, y, z + 96 * k), (6, 70 * k, 6))
    for sy in (-1, 1):
        m.box(DARK, (x + d * 118 * k, y + sy * 40 * k, z + 6 * k), (120 * k, 12 * k, 8 * k))
    if flag:
        m.box(WHITE, (x - d * 100 * k, y, z + 150 * k), (5, 5, 170 * k))
        m.box(flag, (x - d * 124 * k, y, z + 216 * k), (46 * k, 5, 30 * k))
    sp = prop(sc, x, y, L, v * d, z, rate=2.6, bob=22.0, phase=r.random())
    sp.sphere(SNOW_SH, (x - d * 160 * k, y, z + 16 * k), 30 * k, scale=(1.7, 1.0, 0.7), steps=(3, 6))
    look = look or suit(r, hat='helmet')
    s = look['s']
    kw = dict(arm_l='push', arm_r='push')
    if wave:
        kw = dict(arm_l='push', arm_r='wave', swing_r=22.0)
    fig(sc, x - d * 14 * k, y, look, z=z + 84 * k - 46.0 * s, yaw=90.0 * d, legs='sit', seat=46.0, rate=1.2, phase=r.random(), travel=(L, v * d),
        detail=1, **kw)


def cargo_sled(sc, x, y, d, L, v, z, k=1.25, rescue=False):
    """A sled on a tow bar behind a snowmobile at x + 300 k d: crates under a net, or the patrol's rescue sled."""
    m = prop(sc, x, y, L, v * d, z)
    for sy in (-1, 1):
        m.box(STEEL_D, (x, y + sy * 34 * k, z + 6 * k), (190 * k, 8 * k, 10 * k))
    m.box(STEEL_D, (x + d * 140 * k, y, z + 20 * k), (110 * k, 6, 6))
    if rescue:
        m.box(ORANGE, (x, y, z + 30 * k), (180 * k, 62 * k, 36 * k))
        m.box(WHITE, (x, y, z + 50 * k), (60 * k, 64 * k, 6))
        m.box(RED, (x - d * 96 * k, y, z + 70 * k), (6, 6, 110 * k))
        m.box(RED, (x - d * 112 * k, y, z + 112 * k), (34 * k, 5, 22 * k))
    else:
        m.box(WOOD, (x, y, z + 20 * k), (180 * k, 66 * k, 16 * k))
        m.box(TEAL, (x - d * 40 * k, y, z + 52 * k), (70 * k, 56 * k, 50 * k))
        m.box(YEL, (x + d * 42 * k, y, z + 46 * k), (64 * k, 54 * k, 38 * k))
        m.box(RED, (x + d * 30 * k, y, z + 80 * k), (40 * k, 40 * k, 30 * k))


# ------------------------------------------------------------------------------------------ by the road
def ski_trail(seed, variant):
    """30 m of cross-country track on a snow bank: two lanes, one each way."""
    def build(sc):
        r = random.Random(seed)
        L, H = 3000.0, 120.0
        ya, yb = -60.0, 75.0
        g = berm(sc, L, H, lanes=(ya, yb))
        ends(g, L, H)
        markers(g, L, H, 150.0, n=6)
        h = L / 2.0 - 220.0
        if variant == 0:
            for k, bx in enumerate((-h, -h * 0.35, h * 0.45)):
                xc_skier(sc, r, bx, ya, 1, L, 330.0, H)
            xc_skier(sc, r, h, yb, -1, L, 300.0, H)
            xc_skier(sc, r, -h * 0.1, yb, -1, L, 300.0, H, look=suit(r, build='kid', s=0.95, hat='beanie'), rate=1.5)
        else:
            # a skating pair, fast, side by side
            for k, (bx, by) in enumerate(((-h, ya - 20.0), (-h + 190.0, ya + 40.0))):
                xc_skier(sc, r, bx, by, 1, L, 520.0, H, stride=24.0, rate=1.4, swing=34.0, look=suit(r, hat='helmet'))
            xc_skier(sc, r, h * 0.5, ya, 1, L, 330.0, H)
            # a parent towing a pulk with a child in it, and the dog
            px = h * 0.2
            xc_skier(sc, r, px, yb, -1, L, 260.0, H, look=suit(r, hat='beanie'))
            m = prop(sc, px, yb, L, -260.0, H)
            m.box(RED, (px + 250.0, yb, H + 14.0), (150, 56, 22))
            m.box(YEL, (px + 310.0, yb, H + 34.0), (26, 56, 44))
            tilt_box(m, DARK, (px + 20.0, yb, H + 118.0), (px + 180.0, yb, H + 26.0), 5.0, 5.0)
            fig(sc, px + 262.0, yb, suit(r, build='kid', s=0.82, hat='beanie'), z=H + 22.0 - 22.0 * 0.82, yaw=-90.0, legs='sit', seat=22.0,
                arm_l='up', arm_r='up', rate=1.0, bob=2.0, phase=r.random(), travel=(L, -260.0), detail=1)
            dog(sc, px - 230.0, yb + 55.0, -1, L, 260.0, H, body='#3b3f52', s=1.4, phase=0.4)
    return build


def snowshoe(seed):
    """A guided party on snowshoes: five walkers with packs and poles behind a guide with a pennant, a big dog."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 2800.0, 120.0, 150.0
        g = berm(sc, L, H, lanes=(0.0,))
        ends(g, L, H, poles=(ORANGE, TEAL))
        markers(g, L, H, 140.0, n=5, cols=(TEAL, YEL))
        h = L / 2.0 - 260.0
        for k in range(5):
            bx = h - k * 250.0
            by = (-18.0, 22.0)[k % 2]
            guide = k == 0
            look = suit(r, hat='beanie', pack=r.choice((ORANGE, TEAL, VIOLET, YEL)), top=(RED if guide else r.choice(SUITS)))
            s, ph, st = look['s'], r.random(), 22.0
            zf = H + 5.0 * s
            kw = dict(hold_l=hold_pole(), hold_r=hold_pole(), arm_l=XC_ARM, arm_r=XC_ARM)
            if guide:
                kw = dict(hold_l=hold_pole(), arm_l=XC_ARM, arm_r=((30, 60), (110, 80)), hold_r=P.hold_flag(YEL, STEEL, 54.0))
            fig(sc, bx, by, look, z=zf, yaw=90.0, legs='stride', stride=st, swing_l=18.0, swing_r=(0.0 if guide else -18.0), axis_l='x', axis_r='x',
                rate=0.8, phase=ph, bob=4.0, travel=(L, v), detail=1, **kw)
            for side in (-1, 1):
                m = leg_part(sc, bx, by, zf, 90.0, s, side, st, ph, 0.8, 4.0, L, v)
                m.box(ORANGE if guide else LIME, (side * 10.0, -14.0 * side - 6.0, -2.0), (22, 62, 4))
        dog(sc, h - 5 * 250.0 - 60.0, 0.0, 1, L, v, H, body='#b87a50', s=1.5, phase=0.2, big=True)
    return build


def bike(m, x, y, d, col, z, k=1.25):
    """A fat bike heading d (+1: +X), its saddle over (x - 8 k d, y) at 80 k."""
    for dx in (-52, 54):
        rod(m, DARK, (x + d * dx * k, y - 9.0, z + 36 * k), (x + d * dx * k, y + 9.0, z + 36 * k), 36 * k, steps=8)
    tilt_box(m, col, (x - d * 52 * k, y, z + 36 * k), (x - d * 10 * k, y, z + 76 * k), 6, 6)
    tilt_box(m, col, (x - d * 10 * k, y, z + 72 * k), (x + d * 40 * k, y, z + 88 * k), 6, 6)
    tilt_box(m, col, (x + d * 38 * k, y, z + 98 * k), (x + d * 54 * k, y, z + 36 * k), 6, 6)
    m.box(WHITE, (x + d * 38 * k, y, z + 100 * k), (6, 50 * k, 6))
    m.box(DARK, (x - d * 10 * k, y, z + 80 * k), (28 * k, 13 * k, 6))


def fat_bikes(seed):
    """Fat-tyre bikes on a groomed track: three one way, one with a pennant the other; helmets, hot jackets."""
    def build(sc):
        r = random.Random(seed)
        L, H, k = 3200.0, 120.0, 1.25
        ya, yb = -55.0, 70.0
        g = berm(sc, L, H, lanes=(ya, yb))
        ends(g, L, H, poles=(PINK, YEL))
        markers(g, L, H, 150.0, n=6, cols=(BLUE, ORANGE))
        h = L / 2.0 - 240.0
        for bx, by, d, v, col, flag in ((-h, ya, 1, 620.0, RED, None), (-h * 0.3, ya, 1, 620.0, TEAL, None), (h * 0.55, ya, 1, 560.0, YEL, None),
                                        (h * 0.2, yb, -1, 520.0, VIOLET, PINK)):
            m = prop(sc, bx, by, L, v * d, H)
            bike(m, bx, by, d, col, H, k)
            if flag:
                m.box(WHITE, (bx - d * 54 * k, by, H + 130 * k), (5, 5, 190 * k))
                m.box(flag, (bx - d * 78 * k, by, H + 212 * k), (44 * k, 5, 28 * k))
            look = suit(r, hat='helmet', s=1.25)
            fig(sc, bx - d * 8 * k, by, look, z=H + 80 * k - 66.0 * look['s'], yaw=90.0 * d, arm_l='push', arm_r='push', stride=30.0, rate=1.4,
                phase=r.random(), travel=(L, v * d), detail=1)
    return build


# ------------------------------------------------------------------------------------------ the edge of the level snow
def skidoos(seed, variant):
    """Snowmobiles in convoy along a snow bank."""
    def build(sc):
        r = random.Random(seed)
        L, H, k = 3400.0, 130.0, 1.25
        g = berm(sc, L, H, w=700.0, lanes=(0.0,))
        ends(g, L, H, y=-195.0, poles=(ORANGE, RED))
        markers(g, L, H, 165.0, n=7)
        h = L / 2.0 - 300.0
        if variant == 0:
            v = 900.0
            skidoo(sc, r, h, 0.0, 1, L, v, H, YEL, k, flag=PINK)
            skidoo(sc, r, h - 520.0, 20.0, 1, L, v, H, RED, k, flag=TEAL, wave=True)
            skidoo(sc, r, -h + 520.0, -10.0, 1, L, v, H, VIOLET, k)      # (fix round: was blue with the white bonnet, the ICE PATROL bikes' colours)
            cargo_sled(sc, -h + 520.0 - 300.0 * k, -10.0, 1, L, v, H, k)
        else:
            v = 760.0
            patrol = dict(top=RED, bottom=DARK, hat='helmet', hat_col=WHITE, band=WHITE)
            skidoo(sc, r, -h, 0.0, -1, L, v, H, ORANGE, k, flag=RED, look=suit(r, **patrol))
            cargo_sled(sc, -h + 300.0 * k, 0.0, -1, L, v, H, k, rescue=True)
            skidoo(sc, r, h * 0.35, 15.0, -1, L, v, H, LIME, k, flag=ORANGE, look=suit(r, **patrol), wave=True)
            xc_skier(sc, r, h * 0.9, -120.0, 1, L, 330.0, H)
    return build


def sleigh(seed):
    """Two reindeer in tandem and a red sleigh: the driver, two passengers waving, a skier towed behind on a rope."""
    def build(sc):
        r = random.Random(seed)
        L, H, v, k = 3200.0, 130.0, 430.0, 1.25
        g = berm(sc, L, H, w=700.0, lanes=(0.0,))
        ends(g, L, H, y=-195.0, poles=(RED, YEL))
        markers(g, L, H, 165.0, n=6, cols=(RED, YEL))
        x0 = 150.0
        for i, dx in enumerate((330.0, 560.0)):
            reindeer(sc, r, x0 + dx, 0.0, 1, L, v, H, s=1.35, col=DEER[i])
        m = prop(sc, x0, 0.0, L, v, H)
        for sy in (-1, 1):
            m.box(YEL, (x0, sy * 40 * k, H + 6 * k), (230 * k, 7 * k, 9 * k))
            tilt_box(m, YEL, (x0 + 115 * k, sy * 40 * k, H + 6 * k), (x0 + 150 * k, sy * 40 * k, H + 52 * k), 7 * k, 9 * k)
            m.box(DARK, (x0 + 330.0, sy * 26.0, H + 118.0), (420, 5, 5))            # the traces
        m.box(RED, (x0, 0, H + 34 * k), (190 * k, 84 * k, 40 * k))
        m.box(RED, (x0 - 92 * k, 0, H + 70 * k), (12 * k, 84 * k, 50 * k))
        m.box(WHITE, (x0, 0, H + 56 * k), (192 * k, 86 * k, 7 * k))
        m.box(RED, (x0 + 96 * k, 0, H + 62 * k), (14 * k, 84 * k, 34 * k))
        for i, (dx, kw) in enumerate(((52.0, dict(arm_l='push', arm_r='push')), (-22.0, dict(arm_l='wave', arm_r='hold', swing_l=24.0)),
                                      (-64.0, dict(arm_l='hold', arm_r='wave', swing_r=24.0)))):
            look = suit(r, hat='beanie', s=1.2)
            fig(sc, x0 + dx * k, (0.0, -18.0, 18.0)[i], look, z=H + 58 * k - 46.0 * look['s'], yaw=90.0, legs='sit', seat=46.0, rate=1.1,
                phase=r.random(), travel=(L, v), detail=1, **kw)
        # the skier on the rope
        sx = x0 - 520.0
        look = suit(r, hat='helmet')
        fig(sc, sx, 0.0, look, z=H + 6.0 * look['s'], yaw=90.0, legs='wide', arm_l='fwd', arm_r='fwd', rate=0.9, bob=3.0, phase=0.3, travel=(L, v),
            detail=1)
        m.box(DARK, (sx + 210.0, 0.0, H + 128.0), (400, 5, 5))
        for sy in (-1, 1):
            m.box(LIME, (sx + 10.0, sy * 14.0, H + 3.0), (200, 12, 5))
    return build


def skaters(seed):
    """A round rink on a plinth of snow (the hill falls away under it): eight skaters circling an island with a
    pennant mast, a pair hand in hand, a child, and one spinning on the spot."""
    def build(sc):
        r = random.Random(seed)
        R, Z = 820.0, 80.0
        g = sc.still()
        g.cyl(SNOW, (0, 0, -300.0), R + 90.0, 300.0 + Z - 12.0, steps=12, top_r=R + 40.0)
        g.cyl(ICE, (0, 0, Z - 14.0), R, 14.0, steps=12)
        g.cyl(ICE_D, (0, 0, Z - 13.0), R * 0.55, 14.0, steps=10)
        g.cyl(SNOW, (0, 0, Z), 130.0, 34.0, steps=8, top_r=100.0)
        g.box(STEEL_D, (0, 0, Z + 230.0), (9, 9, 420))
        for k, col in enumerate((RED, YEL, TEAL)):
            fl = sc.part(P.motion(swing=14.0, pivot=(0.0, 0.0, Z + 360.0 - k * 46.0), axis=P.UP, rate=0.8, phase=0.3 * k))
            fl.box(col, (48.0, 0.0, Z + 410.0 - k * 46.0), (90, 5, 34))
        g = sc.still()
        for k in range(12):                                   # a rope of pennant posts round the edge
            a = math.radians(k * 30.0 + 15.0)
            g.box((PINK, ORANGE)[k % 2], ((R + 20.0) * math.cos(a), (R + 20.0) * math.sin(a), Z + 30.0), (10, 10, 90))

        def skater(rad, ang, rps, look, **kw):
            a = math.radians(ang)
            v = dict(legs='stride', stride=24.0, swing_l=30.0, swing_r=-30.0, axis_l='x', axis_r='x', rate=0.9, phase=r.random(), detail=1)
            v.update(kw)
            fig(sc, rad * math.cos(a), rad * math.sin(a), look, z=Z, yaw=ang + (180.0 if rps > 0 else 0.0), orbit=(0.0, 0.0, rps), **v)

        skater(700.0, 10.0, 0.085, suit(r))
        skater(690.0, 130.0, 0.085, suit(r, hat='beanie'))
        skater(650.0, 250.0, 0.085, suit(r))
        skater(540.0, 60.0, 0.11, suit(r), arm_r='out', arm_l='down', swing_r=0.0)            # the pair, hand in hand
        skater(460.0, 60.0, 0.11, suit(r, hat='beanie'), arm_l='out', arm_r='down', swing_l=0.0)
        skater(520.0, 200.0, 0.11, suit(r, build='kid', s=0.9, hat='helmet'), arm_l='out', arm_r='out', swing_l=10.0, swing_r=10.0, axis_l='y',
               axis_r='y', rate=1.6)
        skater(330.0, 300.0, 0.14, suit(r, hat='helmet'), stride=30.0, rate=1.2)
        fig(sc, 300.0, -40.0, suit(r, hat=None, hair='bun'), z=Z, arm_l='up', arm_r='up', turn=170.0, rate=0.45, phase=0.1, detail=1)   # spinning
    return build


def run_point(f, L, S, A, c=-0.15, d=1):
    """Where a sledder is at fraction f of its round (ft_people_ice.py: DOWNHILL): (x along the mesh, height)."""
    a = 2.0 * math.pi * (f + c * d)
    return ((f - 0.5) * L - S * math.sin(a), A * d * math.sin(a))


def run_curve(L, S, A, c=-0.15, d=1, n=12):
    fs = [0.4 + 0.6 * i / float(n) for i in range(n + 1)]
    return [run_point(f if d > 0 else 1.0 - f, L, S, A, c, d) for f in fs]


def toboggan(d=1, seed=1):
    """A toboggan hill: five sledders come down 5 m out of a striped shelter, run out and come round again (they
    climb back unseen inside the hill). 26 m long, 6 m deep. d = -1 for the right of the road, +1 for the left."""
    def build(sc):
        r = random.Random(seed)
        L, S, A, v, c = 2400.0, 360.0, 260.0, 540.0, -0.15
        g = sc.still()
        pts = [(x, z + A) for x, z in run_curve(L, S, A, c, d)]
        top = pts[0]
        back = top[0] - d * 480.0
        poly = [(back, -300.0), (back, top[1])] + pts + [(pts[-1][0] + d * 80.0, -300.0)]
        g.side(SNOW, poly, -300.0, 300.0)
        for k in range(len(pts) - 1):
            a, b = pts[k], pts[k + 1]
            for sy in (-1, 1):
                tilt_box(g, (ORANGE, PINK)[k % 2], (a[0], sy * 292.0, a[1] + 20.0), (b[0], sy * 292.0, b[1] + 20.0), 8, 40)
            if k % 2:
                tilt_box(g, SNOW_SH, (a[0], 0.0, a[1] + 1.0), (b[0], 0.0, b[1] + 1.0), 560, 5)
        hx = top[0] - d * 230.0
        g.box(WOOD, (hx, 0, top[1] + 130.0), (400, 520, 260))
        g.box(DARK, (hx + d * 201.0, 0, top[1] + 110.0), (4, 380, 210))
        g.box(SNOW, (hx, 0, top[1] + 276.0), (460, 580, 36))
        for k in range(5):
            g.box((TEAL, WHITE)[k % 2], (hx + d * 232.0, -208.0 + k * 104.0, top[1] + 246.0), (60, 104, 10), rot=(-16 * d, 0, 0))
        fl = sc.part(P.motion(swing=16.0, pivot=(hx, 0.0, top[1] + 420.0), axis=P.UP, rate=0.9, phase=0.2))
        fl.box(YEL, (hx + d * 60.0, 0.0, top[1] + 430.0), (120, 5, 70))
        g = sc.still()
        g.box(STEEL_D, (hx, 0, top[1] + 370.0), (8, 8, 200))
        th = -S / PZ
        px = -(A * d) / th
        n = 5
        lanes = (-200.0, 110.0, -60.0, 210.0, -130.0)
        for i in range(n):
            bx, by = -L / 2.0 + (i + 0.5) * L / n, lanes[i]
            sway = dict(swing=math.degrees(th), pivot=(bx - px, by, A - PZ), axis=90.0)
            rate = d * v / L
            ph = (bx / L + 0.5 + c * d) % 1.0
            look = suit(r, hat=r.choice(('beanie', 'helmet')), s=(1.0 if i % 2 else 1.3), build=('kid' if i % 2 else 'avg'))
            s = look['s']
            fig(sc, bx, by, look, z=A + 12.0, yaw=90.0 * d, legs='sit', seat=20.0, arm_l='up', arm_r='up', detail=1, rate=rate, phase=ph, sway=sway,
                travel=(L, d * v))
            m = sc.part(P.motion(phase=ph, rate=rate, base=(bx, by), travel=(L, rate), **sway), (bx, by, A), 90.0 * d, s)
            m.box(r.choice((RED, YEL, BLUE, LIME)), (0, -18, 9), (52, 124, 12))
            m.box(WOOD_D, (0, -82, 16), (52, 10, 22))
        # the people at the bottom: one filming, one waving the next one down
        ex = pts[-1][0] - d * 120.0
        fig(sc, ex, -250.0, suit(r, hat='beanie'), z=pts[-1][1], yaw=-90.0 * d, arm_l='up', arm_r='up', swing_l=16.0, swing_r=-16.0, bob=8.0, rate=1.3,
            phase=0.2, detail=1)
        fig(sc, ex + d * 90.0, 250.0, suit(r), z=pts[-1][1], yaw=-90.0 * d, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rate=0.5, bob=2.0,
            phase=0.6, detail=1)
    return build


# ------------------------------------------------------------------------------------------ the slope above, the sky
def herd(seed):
    """Seven reindeer at a trot along a wide snow bank, the herder on a snowmobile behind them, a dog at the flank."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 3200.0, 130.0, 400.0
        g = berm(sc, L, H, w=960.0)
        markers(g, L, H, 225.0, n=6, cols=(RED, ORANGE))
        for i, (dx, dy) in enumerate(((1100, -40), (900, 110), (800, -150), (560, 20), (420, 160), (330, -120), (120, 40))):
            reindeer(sc, r, dx, dy, 1, L, v, H, s=1.5 if i % 3 else 1.7, rate=2.0 + 0.2 * (i % 3), bob=6.0, col=DEER[i % 4])
        skidoo(sc, r, -600.0, 0.0, 1, L, v, H, RED, 1.3, flag=YEL, look=suit(r, top=BLUE, hat='beanie', hat_col=RED), wave=True)
        dog(sc, -150.0, -170.0, 1, L, v, H, body='#22202a', s=1.7, phase=0.5)
    return build


def kites(seed):
    """Two snowkiters hauled along a snow bank under big kites: a boarder and a skier, lines taut, kites floating."""
    def build(sc):
        r = random.Random(seed)
        L, H, v = 3200.0, 130.0, 820.0
        g = berm(sc, L, H, w=760.0, lanes=(-40.0, 90.0))
        markers(g, L, H, 180.0, n=6, cols=(TEAL, PINK))
        up = ((128, 30), (150, 20))
        for i, (bx, by, cols) in enumerate(((-700.0, -40.0, (PINK, YEL, PINK, YEL, PINK)), (800.0, 90.0, (TEAL, LIME, WHITE, LIME, TEAL)))):
            look = suit(r, hat='helmet')
            s, ph = look['s'], r.random()
            board = i == 0
            fig(sc, bx, by, look, z=H + 7.0 * s, yaw=(0.0 if board else 90.0), legs='wide', arm_l=up, arm_r=up, rate=0.5, bob=4.0, phase=ph,
                travel=(L, v), detail=1)
            m = prop(sc, bx, by, L, v, H)
            if board:
                m.box(cols[0], (bx, by, H + 4.0), (200, 40, 7))
            else:
                for sy in (-1, 1):
                    m.box(cols[0], (bx + 20.0, by + sy * 22.0, H + 3.0), (220, 13, 5))
            kx, ky, kz = bx + 420.0, by + 60.0, H + 1050.0 + i * 160.0
            kt = prop(sc, bx, by, L, v, H, rate=0.35, bob=-30.0, phase=ph)
            arc = [(ky + j * 130.0, kz - (j * j) * 28.0) for j in (-2.5, -1.5, -0.5, 0.5, 1.5, 2.5)]
            for j in range(5):
                tilt_box(kt, cols[j], (kx, arc[j][0], arc[j][1]), (kx, arc[j + 1][0], arc[j + 1][1]), 190.0, 12.0)
            hand = (bx + (0.0 if board else 30.0), by, H + 215.0)
            for j in (0, 5):
                tilt_box(kt, DARK, hand, (kx, arc[j][0], arc[j][1]), 6.0, 6.0)
            sp = prop(sc, bx, by, L, v, H, rate=2.2, bob=18.0, phase=r.random())
            sp.sphere(SNOW_SH, (bx - 150.0, by, H + 16.0), 34.0, scale=(1.7, 1.0, 0.7), steps=(3, 6))
    return build


def geese(seed):
    """A skein of nine geese in a V, 24 m up, flying a 120 m line along the road and coming round again."""
    def build(sc):
        r = random.Random(seed)
        L, v = 12000.0, 950.0
        for k in range(9):
            j = (k + 1) // 2
            sy = -1 if k % 2 else 1
            P.bird(sc, 600.0 - j * 190.0, sy * j * 170.0, 2400.0 + r.uniform(-40.0, 40.0) + j * 12.0, heading=0.0, span=330.0, body='#b87a50', wing='#6a4c36',
                   beak=ORANGE, phase=r.random(), rate=1.7, flap=30.0, travel=(L, v))
    return build


FAR = dict(far=True)
JOBS = [
    ('SM_P3C_SkiTrail_A', ski_trail(31, 0)),
    ('SM_P3C_SkiTrail_B', ski_trail(32, 1)),
    ('SM_P3C_Snowshoe_A', snowshoe(33)),
    ('SM_P3C_FatBikes_A', fat_bikes(34)),
    ('SM_P3C_Skidoos_A', skidoos(35, 0)),
    ('SM_P3C_Skidoos_B', skidoos(36, 1)),
    ('SM_P3C_Sleigh_A', sleigh(37)),
    ('SM_P3C_Skaters_A', skaters(38)),
    ('SM_P3C_Toboggan_A', toboggan(-1, 39)),      # for the right of the road: the slope faces the car coming down the road
    ('SM_P3C_TobogganL_A', toboggan(1, 40)),      # its twin for the left
    ('SM_P3C_Herd_A', herd(41), FAR),
    ('SM_P3C_Kites_A', kites(42), dict(far=True, grow=(60.0, 60.0, 120.0))),
    ('SM_P3C_Geese_A', geese(43), dict(far=True, grow=(6400.0, 120.0, 160.0), grow_down=(6400.0, 120.0, 160.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
