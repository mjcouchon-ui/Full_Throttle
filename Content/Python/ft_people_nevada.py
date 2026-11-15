"""FULL THROTTLE (Unreal) the population of level 3, Nevada (level index 2): /Game/People/Nevada/.

Highway 93 to the strip. The road is level and straight and the land lies a metre under it behind a metre of
barrier, so a person on the ground nearer than 33 m is a hat at best. Everybody here is therefore RAISED: on a
roadhouse's beer deck, on a diner's terrace, on an earth bank behind a crash rail, on a casino's carpeted steps, on a
grandstand, in the saddle, or up on the canyon's rim against the sky.

    by the road (24 to 34 m, all TALL: decks and banks two metres up, their people from the feet up over the barrier)
      SM_RoadhouseDeck   the Snake Eyes' beer deck: bikers cheering the chase, the club's colours on a pole
      SM_DinerDeck       a diner's terrace: a family at a parasol table, a waitress on skates, a trucker waving his cap
      SM_Hitchers_A/_B   hitch-hikers on an earth bank BEHIND a crash rail, well back: thumbs, a striped sign, a dog
      SM_Prospectors     a claim on a rock ledge: a pick swinging, a pan, an ore cart on its rails, the mule
      SM_Lizards         a sun-baked boulder and its gila monsters doing press-ups
      SM_Riders          ranch hands riding the fence line, hats in the air, a longhorn trotting ahead of them
      SM_StripStand_A/_B the strip's grandstands: a crowd of gamblers and tourists hopping out of step, flags
      SM_Valets          a casino's carpeted steps: valets running with keys, a doorman, a couple coming down
      SM_Wedding         a chapel-of-chance wedding on its dais: bride, groom, the guests throwing confetti
      SM_Gamblers        a terrace of slot machines: arms pulling levers, one jackpot, the coins hopping
    the canyon's rim (a 16.5 m column of the canyon's own rock: inside a rock cut it is hidden and its cap stands on
    the rim; in the open it is a hoodoo)
      SM_RimCoyotes      coyotes on the rim, one howling
      SM_RimPosse        a posse sitting their horses on the rim, hats waving
    far life (saved far=True: it keeps moving past 180 m; built big)
      SM_WildHorses      a herd at the gallop across 140 m of flat, dust behind it
      SM_Cattle          a grazing herd, a rider circling it

Everything moves through M_People (ft_people.py). No words, no marks, no light on any piece. The placing rules are
Source/FullThrottle/FTPeopleL2.inl.   Run in the editor:  import ft_people_nevada; ft_people_nevada.run()
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/Nevada'

# the desert's own palette (ft_desert / ft_dress_nevada), so that a deck or a bank belongs to the dressing beside it
SAND, SANDST, SANDST_D, SANDST_L, ROCK_R, TALUS = '#f2c58a', '#e0784a', '#b8553a', '#f59a62', '#c9603e', '#a04a34'
SAGE, SAGE_D, BARK, BARK_D, WOOD = '#8fb86a', '#5f8f4a', '#8a6a4a', '#6a4c36', '#9a6a44'
LEATHER, NAVY, PURP, MAG, TEAL, RED, CYAN = '#22202a', '#2b2350', '#7b3bd6', '#ff4fb8', '#2fc6c0', '#ff2d3c', '#2ee6ff'
CREAM, WHITE, GOLD, YEL, STEEL, CHROME, CONC = '#f4efe6', '#fff6ea', '#ffc93c', '#ffe14d', '#8f9bb3', '#d7e3f0', '#c9b8a0'
DENIM, RUST, MINT, PINK, LEMON = '#3a5fa8', '#b8553a', '#7fe3c4', '#ff9ec4', '#ffe9a8'
ROCK_TONES = (TALUS, SANDST_D, ROCK_R, SANDST, SANDST_L, SANDST, ROCK_R, SANDST_L)
HOT = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a', '#ff2d3c')


# ------------------------------------------------------------------------------------------ what people stand on
def ring(r, cx, cy, rx, ry, n=10, rough=0.16):
    pts = []
    for k in range(n):
        a = 2 * math.pi * k / n
        q = 1.0 + r.uniform(-rough, rough)
        pts.append((cx + math.cos(a) * rx * q, cy + math.sin(a) * ry * q))
    return pts


def rock_stack(g, r, L, D, H, tones=ROCK_TONES, band=170.0, top=0.84, foot=-120.0, cap=None, y0=0.0, n=10, rough=0.14):
    """Banded rock from `foot` (under the ground: the land may fall away) up to a flat top at H, each bed a lumpy
    outline of its own stepping in a little. top: the top bed's size as a share of the base. Returns the top's half
    sizes (x, y) that are safe to stand on."""
    k = max(2, int(round(H / band)))
    base = ring(r, 0.0, 0.0, L / 2.0, D / 2.0, n, rough)
    zs = [foot] + [H * (i + 1.0) / k for i in range(k)]
    for i in range(k):
        f = 1.0 - (1.0 - top) * (i / (k - 1.0))
        pts = [(x * f * (1.0 + r.uniform(-0.03, 0.03)), y0 + y * f * (1.0 + r.uniform(-0.03, 0.03))) for (x, y) in base]
        col = cap if (cap and i == k - 1) else tones[i % len(tones)]
        g.plan(col, pts, zs[i], zs[i + 1])
    return L / 2.0 * top * (1.0 - rough - 0.03), D / 2.0 * top * (1.0 - rough - 0.03)


def deck(g, L, D, H, top=WOOD, post=BARK_D, rail=CREAM, skirt=None, stripe=None, y=0.0, front_rail=True, steps_at=None):
    """A timber deck on posts, its floor at H: a rail all round (the front one two thin bars, so the people behind it
    show), cross braces, and a skirt board along the front in the house's colours."""
    g.box(top, (0, y, H - 9), (L, D, 18))
    n = max(2, int(L / 300.0) + 1)
    for k in range(n):
        x = -L / 2.0 + 14 + k * (L - 28) / (n - 1.0)
        for sy in (-1, 1):
            g.box(post, (x, y + sy * (D / 2.0 - 12), (H - 18) / 2.0), (18, 18, H - 18))
        if k < n - 1:
            x2 = -L / 2.0 + 14 + (k + 1) * (L - 28) / (n - 1.0)
            if k % 2 == 0:
                tilt_box(g, post, (x, y - D / 2.0 + 12, 20), (x2, y - D / 2.0 + 12, H - 40), 8, 8)
            else:
                tilt_box(g, post, (x, y - D / 2.0 + 12, H - 40), (x2, y - D / 2.0 + 12, 20), 8, 8)
    if skirt:
        g.box(skirt, (0, y - D / 2.0 - 2, H - 34), (L + 8, 6, 52))
        if stripe:
            g.box(stripe, (0, y - D / 2.0 - 5.5, H - 34), (L + 8, 2, 14))
    # the rail: posts at the back and the sides, a board along the back; in front two bars
    for k in range(n):
        x = -L / 2.0 + 8 + k * (L - 16) / (n - 1.0)
        g.box(rail, (x, y + D / 2.0 - 6, H + 55), (9, 9, 110))
        if front_rail:
            g.box(rail, (x, y - D / 2.0 + 6, H + 52), (7, 7, 104))
    g.box(rail, (0, y + D / 2.0 - 6, H + 106), (L, 9, 9))
    g.box(rail, (0, y + D / 2.0 - 6, H + 58), (L, 6, 7))
    for sx in (-1, 1):
        g.box(rail, (sx * (L / 2.0 - 6), y, H + 106), (9, D, 9))
    if front_rail:
        g.box(rail, (0, y - D / 2.0 + 6, H + 104), (L, 7, 7))
        g.box(rail, (0, y - D / 2.0 + 6, H + 56), (L, 5, 5))


def flag(sc, x, y, z0, height, col, patch=None, w=170.0, h=110.0, pole=STEEL, phase=0.0):
    """A pole and a flag that swings about it in the wind (a square patch on it at most: never a mark)."""
    g = sc.still()
    g.box(pole, (x, y, z0 + height / 2.0), (8, 8, height))
    f = sc.part(P.motion(swing=13.0, pivot=(x, y, z0), axis=P.UP, rate=0.7, phase=phase))
    f.box(col, (x + w / 2.0 + 4, y, z0 + height - h / 2.0 - 6), (w, 5, h))
    if patch:
        f.box(patch, (x + w / 2.0 + 4, y, z0 + height - h / 2.0 - 6), (w * 0.3, 7, h * 0.4))


def parasol(g, x, y, z0, cols=(MAG, CREAM), r=165.0, h=250.0):
    g.box(STEEL, (x, y, z0 + h / 2.0), (6, 6, h))
    for k in range(8):
        a0, a1 = 2 * math.pi * k / 8.0, 2 * math.pi * (k + 1) / 8.0
        g.plan(cols[k % 2], [(x, y), (x + math.cos(a0) * r, y + math.sin(a0) * r), (x + math.cos(a1) * r, y + math.sin(a1) * r)], z0 + h - 14, z0 + h)


def bunting(g, x0, x1, y, z, n=12, drop=62.0, w=30.0):
    """A string of pennants (what says 'people here' from 200 m): a line and n triangles in the hot colours."""
    g.box(CREAM, ((x0 + x1) / 2.0, y, z), (abs(x1 - x0), 5, 5))
    for k in range(n):
        px = x0 + (k + 0.5) * (x1 - x0) / n
        g.side(HOT[k % len(HOT)], [(px - w, z - 2), (px + w, z - 2), (px, z - drop)], y - 3, y + 3)


def balloons(sc, x, y, z0, height=330.0, phase=0.0, n=6):
    """A bunch of balloons on their strings, bobbing."""
    g = sc.still()
    g.box(CREAM, (x, y, z0 + height / 2.0), (4, 4, height))
    b = sc.part(P.motion(bob=-10.0, rate=0.5, phase=phase))
    for k in range(n):
        a = 2 * math.pi * k / n
        b.sphere(HOT[(k * 3) % len(HOT)], (x + math.cos(a) * 26, y + math.sin(a) * 26, z0 + height + 20 + (k % 3) * 22), 27, scale=(1, 1, 1.2), steps=(4, 6))


def sage(g, r, x, y, z, s=1.0):
    g.sphere(SAGE if r.random() < 0.5 else SAGE_D, (x, y, z + 22 * s), 46 * s, scale=(1, 1, 0.7), steps=(4, 6))


# ------------------------------------------------------------------------------------------ the cast
def person(r, s=1.15, **fixed):
    fixed.setdefault('tops', HOT)
    d = P.look(r, **fixed)
    d['s'] = fixed.get('s', s * r.uniform(0.95, 1.05))
    d.setdefault('detail', 1)      # the cel pass inks every edge: eyes and fingers are ink at 40 m
    return d


def biker(r, s=1.15, **fixed):
    """A Snake Eye off the bike: leather or denim cut, bare arms, a bright band across the chest."""
    d = P.look(r, tops=(LEATHER, NAVY, DENIM, LEATHER, '#5a4a3c'), bottoms=(DENIM, LEATHER, NAVY, '#5a4a3c'),
               build=r.choice(('stocky', 'stocky', 'avg', 'slim')), sleeves=r.choice(('none', 'none', 'short')),
               hair=r.choice(('mohawk', 'bald', 'long', 'pony', 'short', 'afro')), band=r.choice((MAG, TEAL, WHITE, GOLD, RED)),
               hat=r.choice((None, None, 'cap', 'helmet')), hat_col=r.choice((RED, LEATHER, TEAL, PURP)), shoes=LEATHER)
    d['s'] = s * r.uniform(0.96, 1.06)
    d['detail'] = 1
    d.update(fixed)
    return d


def cowhand(r, s=1.15, **fixed):
    d = P.look(r, tops=(RED, '#57c7ff', CREAM, GOLD, MINT, '#ff7a3c'), bottoms=(DENIM, '#5a4a3c', NAVY), sleeves='long',
               hat='cowboy', hat_col=r.choice((CREAM, '#8e5a3a', LEATHER, '#c9a070')), band=r.choice((None, None, WHITE, LEATHER)), shoes='#5a4a3c')
    d['s'] = s * r.uniform(0.96, 1.05)
    d['detail'] = 1
    d.update(fixed)
    return d


def hold_bottle(col='#7dffb0'):
    def f(m, h, side):
        m.cyl(col, (h[0], h[1] - 2, h[2] + 2), 5.0, 20, steps=6)
        m.cyl(col, (h[0], h[1] - 2, h[2] + 22), 2.6, 9, steps=5)
    return f


def hold_hat(col=CREAM):
    """A cowboy hat waved in the hand."""
    def f(m, h, side):
        m.cyl(col, (h[0], h[1], h[2] + 8), 40, 4, steps=8)
        m.cyl(col, (h[0], h[1], h[2] + 10), 20, 16, steps=8, top_r=16)
    return f


def mount(sc, r, x, y, z=0.0, yaw=90.0, s=1.25, coat='#8e5a3a', mane='#3b2a22', rider=None, phase=0.0, rate=0.0, bob=0.0, stride=0.0,
          travel=None, orbit=None, saddle='#5a4a3c', graze=0.0, **horse):
    """A horse and, with rider=dict(figure values), somebody in its saddle: both carry the same phase, hop and travel,
    so they stay together. yaw 90 heads along +X. Returns the rider's head height."""
    kw = dict(phase=phase, rate=rate, bob=bob, travel=travel, orbit=orbit)
    P.quadruped(sc, x, y, z, yaw=yaw, s=s, body=coat, mane=mane, stride=stride, wag=(14 if rate else 0), graze=graze, **dict(kw, **horse))
    if rider is None:
        return z + s * 175.0
    back = 120.0 * s
    tv, base = (0.0, 0.0), (x, y)
    if travel:
        tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    elif orbit:
        base, tv = (orbit[0], orbit[1]), (-1.0, float(orbit[2]))
    m = sc.part(P.motion(phase=phase, rate=rate, bob=bob, base=base, travel=tv, pivot=(x, y, z)), (x, y, z), yaw, s)
    m.box(saddle, (0, 6, 122), (46, 50, 8))                                   # the saddle and its blanket
    m.box(rider.get('band') or RED, (0, 8, 119), (47, 64, 4))
    fs = rider.get('s', 1.1)
    leg = rider.get('bottom', DENIM)
    for side in (-1, 1):                                                      # the rider's legs down the horse's flanks
        tilt_box(m, leg, (side * 16, 4, 124), (side * 26, -4, 78), 14, 15)
        m.box(rider.get('shoes', '#5a4a3c'), (side * 27, -10, 72), (13, 26, 11))
    d = dict(rider)
    d['s'] = fs
    return _torso(sc, x, y, z + back + 2.0 * s, yaw, d, kw)


def _torso(sc, x, y, z, yaw, d, kw):
    """A figure from the hips up (a rider, somebody behind a counter): figure() drawn sitting, sunk to its seat."""
    s = d.get('s', 1.0)
    d = dict(d, legs='sit', seat=46.0)
    for k2 in ('phase', 'rate', 'bob', 'travel', 'orbit'):
        d.pop(k2, None)
    # sitting, the hips are at seat + 6 = 52 cm above the figure's own ground: put that at z
    return P.figure(sc, x, y, z - 46.0 * s, yaw=yaw, phase=kw['phase'], rate=kw['rate'], bob=kw['bob'], travel=kw['travel'], orbit=kw['orbit'], **d)


def dust(sc, x, y, z, size, phase, rate, travel, col=SAND):
    """A puff of dust that goes along with a runner: a flattened ball, swelling and sinking."""
    tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    m = sc.part(P.motion(phase=phase, rate=rate, bob=-size * 0.25, base=(x, y), travel=tv, pivot=(x, y, z)))
    m.sphere(col, (x, y, z + size * 0.45), size, scale=(1.5, 1.0, 0.55), steps=(4, 6))
    m.sphere(col, (x - size * 1.3, y + size * 0.3, z + size * 0.3), size * 0.66, scale=(1.4, 1.0, 0.55), steps=(3, 6))


# ------------------------------------------------------------------------------------------ by the road
def roadhouse_deck(sc):
    """The Snake Eyes' beer deck: eight bikers at the rail cheering the chase, bottles up, the club's colours flying."""
    r = random.Random(21)
    g = sc.still()
    H = 200.0
    deck(g, 1000, 330, H, top=BARK, post=BARK_D, rail=CREAM, skirt=NAVY, stripe=MAG)
    g.box(BARK_D, (0, 200, H / 2.0), (1000, 12, H))                                          # a plank back under the deck
    for (x, col) in ((-430, TEAL), (-372, RUST), (440, RUST)):                             # drums for tables
        g.cyl(col, (x, 80, H), 29, 88, steps=7)
    flag(sc, -486, 150, H, 520, PURP, patch=WHITE, phase=0.3, w=260, h=170)
    flag(sc, 486, 150, H, 440, MAG, phase=0.7, w=210, h=130)
    bunting(g, -480, 480, 158, H + 250, n=14)
    for sx in (-1, 1):
        g.box(CREAM, (sx * 480, 158, H + 180), (7, 7, 150))
    acts = (
        dict(arm_r='up', arm_l='hip', swing_r=22, axis_r='x', bob=7, rate=1.5, react=True),                  # the fist in the air
        dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=9, rate=1.3, react=True),
        dict(arm_r='face', hold_r=hold_bottle(), arm_l='hip', rock=5, rate=0.5),
        dict(arm_r='wave', hold_r=hold_bottle('#ffc93c'), swing_r=22, rate=1.2, bob=4, react=True),
        dict(arm_r='point', arm_l='down', swing_r=10, axis_r='x', rate=0.7),
        dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=8, rate=1.7, react=True),
        dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4, react=True),   # clapping
        dict(arm_r='up', hold_r=P.hold_flag(col=MAG, w=70), swing_r=20, rate=0.9, arm_l='hip', bob=3),
    )
    xs = (-300, -170, -40, 90, 220, 350)
    for i, x in enumerate(xs):
        d = biker(r)
        d['top'] = r.choice((LEATHER, DENIM, WHITE, RED, GOLD, TEAL))
        d.update(acts[(i * 3 + 1) % len(acts)])
        P.figure(sc, x + r.uniform(-10, 10), (-95 if i % 2 else -40) + r.uniform(-8, 8), z=H, yaw=r.uniform(-18, 18), phase=r.random(), **d)


def hitchers(seed, kind):
    def build(sc):
        """Hitch-hikers on an earth bank, well back, BEHIND a crash rail: nobody stands at the verge on this road."""
        r = random.Random(seed)
        g = sc.still()
        H = 190.0
        hx, hy = rock_stack(g, r, 900, 520, H, tones=(SANDST_D, SANDST, SANDST_L), band=95.0, top=0.86, cap=SAND)
        # the crash rail along the front of the top: posts and a steel beam
        ry = -hy + 18
        for k in range(5):
            g.box(STEEL, (-hx + 20 + k * (2 * hx - 40) / 4.0, ry, H + 34), (10, 10, 70))
        g.box(CHROME, (0, ry - 6, H + 56), (2 * hx - 10, 6, 26))
        g.box(STEEL, (0, ry - 9.5, H + 56), (2 * hx - 10, 2, 8))
        sage(g, r, hx * 0.7, hy * 0.6, H)
        sage(g, r, -hx * 0.75, hy * 0.6, H, 0.8)
        y1 = ry + 95
        parasol(g, hx * 0.55, hy * 0.45, H, ((LEMON, TEAL), (PINK, CREAM))[kind], r=150, h=290)
        if kind == 0:
            # the thumb out, a striped card in the other hand
            P.figure(sc, -210, y1, z=H, yaw=-12, arm_r='out', swing_r=13, axis_r='y', arm_l='hold', hold_l=P.hold_sign(col=CREAM, stripe=MAG, w=120, h2=80),
                     rate=1.0, phase=0.1, pack='#ff7a3c', **person(r, 1.2, top='#2ee6ff', hat='sun', hat_col=LEMON, wear='shorts'))
            # one jumping with both arms: they have seen the cameras
            P.figure(sc, -60, y1 + 40, z=H, yaw=8, arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=12, rate=1.4, phase=0.6, react=True,
                     pack=PURP, **person(r, 1.2, top='#ffc93c', hair='long'))
            # one sitting it out on the cases, a guitar case stood beside
            g.box('#c94f6a', (150, y1 + 30, H + 23), (84, 54, 46))
            g.box(CREAM, (150, y1 + 2, H + 30), (60, 3, 8))
            g.box(LEATHER, (232, y1 + 60, H + 64), (40, 18, 128))
            P.figure(sc, 150, y1 + 30, z=H, yaw=20, legs='sit', seat=46, arm_r='wave', swing_r=20, rate=0.9, arm_l='down', phase=0.3,
                     **person(r, 1.2, top='#ff5fa2', hat='cowboy', hat_col=CREAM))
            P.quadruped(sc, 300, y1 + 10, H, yaw=-30, s=1.2, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=34, rate=2.2,
                        body='#e8c26a', belly=CREAM)
        else:
            # three with one big striped card held up between two of them, the third waving a hat
            card = sc.part(P.motion(bob=6, rate=1.1, phase=0.2))
            card.box(CREAM, (-120, y1 - 6, H + 268), (250, 5, 84))
            for k in range(4):
                card.box((MAG, TEAL, GOLD, MAG)[k], (-120 - 90 + k * 60, y1 - 9.5, H + 268), (34, 2, 60))
            P.figure(sc, -235, y1, z=H, yaw=-6, arm_l='up', arm_r='up', bob=6, rate=1.1, phase=0.2, pack=TEAL,
                     **person(r, 1.2, top='#ff5fa2', wear='shorts', hair='afro'))
            P.figure(sc, -5, y1, z=H, yaw=6, arm_l='up', arm_r='up', bob=6, rate=1.1, phase=0.2, pack='#ff7a3c',
                     **person(r, 1.2, top='#7dffb0', hat='cap', hat_col=RED))
            P.figure(sc, 170, y1 + 30, z=H, yaw=14, arm_r='wave', hold_r=hold_hat(CREAM), swing_r=26, rate=1.3, bob=4, arm_l='hip', phase=0.5, react=True,
                     **person(r, 1.2, top='#57c7ff', hair='pony'))
            P.figure(sc, 285, y1 + 10, z=H, yaw=20, build='kid', s=0.8, arm_l='vee', arm_r='vee', bob=14, rate=1.8, phase=0.8, react=True,
                     **{k2: v for k2, v in person(r, top='#ffc93c').items() if k2 not in ('build', 's')})
            g.box('#3a5fa8', (-330, y1 + 70, H + 26), (70, 50, 52))
            g.box('#ffc93c', (-330, y1 + 70, H + 62), (58, 40, 20))
    return build


# ------------------------------------------------------------------------------------------ the canyon's rim
RIM_H = 1650.0


def rim_column(g, r, L, D):
    """16.5 m of the canyon's own beds (the rock cut is 15 m: this stands a cap rock over its rim; alone it is a
    hoodoo). Returns the top's safe half sizes."""
    return rock_stack(g, r, L, D, RIM_H, tones=ROCK_TONES, band=190.0, top=0.9, foot=-250.0, cap=SAND, rough=0.1)


def rim_coyotes(sc):
    """Coyotes on the rim: one howling at the sky, one pacing the edge, a pup sitting. Built big: they are seen
    from 60 m below against the sky."""
    r = random.Random(33)
    g = sc.still()
    hx, hy = rim_column(g, r, 900, 400)
    H = RIM_H
    coat, belly, dark = '#8a7460', '#e8d8b8', '#3b2a22'
    kw = dict(length=85, height=58, girth=24, neck=22, head=26, tail=46, body=coat, belly=belly, ears=dark, tail_col=dark, snout=belly)
    P.quadruped(sc, -150, 10, H, yaw=-70, s=3.6, neck_up=78, graze=9, wag=10, rate=0.45, phase=0.0, **kw)          # the howl
    P.quadruped(sc, 120, -40, H, yaw=90, s=3.0, neck_up=25, stride=24, wag=16, rate=1.6, bob=3, travel=(2 * hx - 60, 170), **kw)
    P.quadruped(sc, 260, 70, H, yaw=-100, s=2.3, neck_up=50, wag=30, graze=6, rate=1.4, phase=0.4, **kw)
    sage(g, r, hx - 70, hy - 40, H, 1.2)
    g.sphere(SANDST_L, (-hx + 60, hy - 60, H + 20), 60, scale=(1.2, 1, 0.7), steps=(4, 6))


# ------------------------------------------------------------------------------------------ far life
def wild_horses(sc):
    """A wild herd at the gallop: nine horses and a foal crossing 140 m of flat and coming round again, dust behind."""
    r = random.Random(41)
    L = 14000.0
    coats = (('#8e5a3a', '#3b2a22'), ('#2a2230', '#22202a'), ('#e8c26a', '#fff6ea'), ('#e8e0f0', '#cfcfd8'), ('#b87a50', '#3b2a22'),
             ('#6a4428', '#2a2230'), ('#fff6ea', '#e8c26a'), ('#8e5a3a', '#e8c26a'), ('#5e3b28', '#22202a'))
    for i, (coat, mane) in enumerate(coats):
        x = -1900 + i * 470 + r.uniform(-110, 110)
        y = r.uniform(-520, 520)
        ph = r.random()
        s = 2.2 * r.uniform(0.92, 1.08)
        P.quadruped(sc, x, y, 0, yaw=90, s=s, body=coat, mane=mane, tail=60, stride=34, wag=10, rate=2.3, bob=14, phase=ph, travel=(L, 1250),
                    neck_up=38, belly=(CREAM if i % 4 == 2 else None))
        dust(sc, x - 340, y, 0, 150, ph, 2.3, (L, 1250))
    P.quadruped(sc, 300, 520, 0, yaw=90, s=1.1, body='#b87a50', mane='#3b2a22', stride=38, rate=3.0, bob=10, phase=0.3, travel=(L, 1250), neck_up=45)


# ------------------------------------------------------------------------------------------ by the road, second batch
def table(g, x, y, z0, top=CREAM, r=46.0, h=74.0):
    g.box(STEEL, (x, y, z0 + h / 2.0), (7, 7, h))
    g.cyl(top, (x, y, z0 + h), r, 6, steps=8)


def stool(g, x, y, z0, col=RED, h=46.0):
    g.box(STEEL, (x, y, z0 + h / 2.0), (6, 6, h))
    g.cyl(col, (x, y, z0 + h - 6), 17, 6, steps=7)


def hold_tray(col=CHROME):
    def f(m, h, side):
        if side > 0:
            m.cyl(col, (h[0] - 22, h[1] - 8, h[2] + 8), 26, 4, steps=8)
            m.cyl('#ff5fa2', (h[0] - 30, h[1] - 8, h[2] + 12), 6, 16, steps=6)
            m.cyl('#fff6ea', (h[0] - 12, h[1] - 8, h[2] + 12), 6, 13, steps=6)
    return f


def diner_deck(sc):
    """A diner's terrace: a family under a parasol, a waitress on skates going the length of it with a tray, a
    trucker waving his cap, the cook pointing, a tourist filming the chase."""
    r = random.Random(61)
    g = sc.still()
    H = 200.0
    deck(g, 1000, 360, H, top=CONC, post=STEEL, rail=CHROME, skirt=TEAL, stripe=CREAM)
    g.box(CREAM, (0, 215, H / 2.0), (1000, 12, H))
    bunting(g, -490, 490, -172, H + 300, n=14)
    for sx in (-1, 1):
        g.box(CHROME, (sx * 490, -172, H + 200), (7, 7, 200))
    for (x, cols) in ((-300, (MAG, CREAM)), (310, (TEAL, CREAM))):
        table(g, x, 60, H)
        parasol(g, x, 60, H, cols)
    # the family: two sitting, a child hopping beside the table
    for (x, yaw, top, kw) in ((-372, 25, '#ffc93c', dict(hair='long', arm_r='wave', swing_r=20)), (-228, -25, '#57c7ff', dict(hat='cap', hat_col=RED, arm_l='point', swing_l=9, axis_l='x'))):
        stool(g, x, 50, H)
        P.figure(sc, x, 50, z=H, yaw=yaw, legs='sit', seat=46, rate=0.9, phase=r.random(), **person(r, 1.12, top=top, **kw))
    d = person(r, top='#ff5fa2')
    d.update(build='kid', s=0.78)
    P.figure(sc, -150, -70, z=H, arm_l='vee', arm_r='vee', bob=13, rate=1.9, phase=0.2, react=True, **d)
    # the waitress on skates, the length of the terrace and round again
    P.figure(sc, 0, -30, z=H + 7, yaw=90, legs='stride', arm_l='carry', arm_r='carry', hold_r=hold_tray(), stride=12, rate=1.2, travel=(880, 260),
             **person(r, 1.12, top=PINK, bottom=PINK, wear='dress', band=WHITE, hair='pony', hat='cap', hat_col=PINK, shoes=WHITE))
    # the trucker, the cook, the tourist
    P.figure(sc, 120, -110, z=H, yaw=-10, arm_r='wave', hold_r=hold_hat(RED), swing_r=26, rate=1.2, bob=4, arm_l='hip', react=True, phase=0.4,
             **person(r, 1.15, top=RED, bottom=DENIM, build='stocky', hair='bald'))
    P.figure(sc, 420, -100, z=H, yaw=12, arm_r='point', swing_r=11, axis_r='x', arm_l='hip', rate=0.7, phase=0.7,
             **person(r, 1.15, top=WHITE, bottom=CREAM, hat='chef', hat_col=WHITE, band=None))
    P.figure(sc, -440, -105, z=H, yaw=-16, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.4,
             **person(r, 1.15, top='#b8ff5a', hat='sun', hat_col=LEMON, wear='shorts'))
    P.figure(sc, 300, 40, z=H, yaw=-150, legs='sit', seat=46, arm_r='face', hold_r=P.hold_cup(), rate=0.3, bob=-1.0, **person(r, 1.12, top='#8f6bff'))
    stool(g, 300, 40, H)


def hold_pick():
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 38), (6, 6, 92))
        tilt_box(m, STEEL, (h[0], h[1] - 34, h[2] + 76), (h[0], h[1] + 34, h[2] + 76), 8, 9)
        m.box(STEEL, (h[0], h[1], h[2] + 82), (9, 18, 12))
    return f


def hold_pan():
    def f(m, h, side):
        if side > 0:
            m.cyl(STEEL, (h[0] - 24, h[1] - 10, h[2] + 4), 30, 7, steps=8, top_r=36)
            m.cyl(GOLD, (h[0] - 24, h[1] - 10, h[2] + 10), 14, 3, steps=6)
    return f


def prospectors(sc):
    """A claim on a rock ledge: one swinging a pick, one panning, one running the ore cart along its rails, the mule
    nodding under its packs, a windlass over the shaft with the bucket going up and down."""
    r = random.Random(71)
    g = sc.still()
    H = 210.0
    hx, hy = rock_stack(g, r, 1100, 600, H, band=105.0, top=0.86, cap=SANDST_L)
    old = dict(sleeves='long', hat='cowboy', bottom=DENIM, band=DENIM, shoes='#5a4a3c')
    # the windlass: an A-frame at each end, the drum, the rope and the bucket
    wx, wy = -300, 70
    g.box(LEATHER, (wx, wy, H + 2), (120, 120, 4))
    for sx in (-1, 1):
        tilt_box(g, BARK, (wx + sx * 80, wy - 40, H), (wx + sx * 80, wy, H + 190), 10, 10)
        tilt_box(g, BARK, (wx + sx * 80, wy + 40, H), (wx + sx * 80, wy, H + 190), 10, 10)
    rod(g, WOOD, (wx - 86, wy, H + 186), (wx + 86, wy, H + 186), 11, steps=7)
    bk = sc.part(P.motion(bob=-38.0, rate=0.22))
    bk.box('#c9a070', (wx, wy, H + 130), (5, 5, 110))
    bk.cyl(RUST, (wx, wy, H + 44), 24, 36, steps=7, top_r=28)
    bk.cyl(GOLD, (wx, wy, H + 80), 20, 6, steps=6)
    P.figure(sc, wx + 150, wy + 10, z=H, yaw=-80, arm_l='push', arm_r='push', swing_l=26, swing_r=26, axis_l='x', axis_r='x', rate=0.44, phase=0.1,
             **person(r, 1.18, top='#ffc93c', hat_col=CREAM, **old))
    # the pick
    P.figure(sc, 30, -60, z=H, yaw=-20, legs='wide', arm_r=((140, 70), (150, 70)), hold_r=hold_pick(), swing_r=42, axis_r='x', arm_l='hip', rate=1.0, phase=0.5,
             **person(r, 1.2, top=RED, hat_col='#8e5a3a', build='stocky', **old))
    g.sphere(SANDST, (40, -160, H + 24), 44, scale=(1.3, 1, 0.8), steps=(4, 6))
    g.sphere(GOLD, (70, -175, H + 50), 13, steps=(3, 5))
    # the pan: sitting on a rock, rocking it
    g.sphere(SANDST_D, (250, -40, H + 20), 40, scale=(1, 1, 0.9), steps=(4, 6))
    P.figure(sc, 250, -40, z=H - 6, yaw=14, legs='sit', seat=46, arm_l='carry', arm_r='carry', hold_r=hold_pan(), rock=7, rate=1.1,
             **person(r, 1.18, top=MINT, hat_col=LEATHER, **old))
    # the mule under its packs
    mh = P.quadruped(sc, 390, 120, H, yaw=-60, s=1.25, body='#8f8478', belly=CREAM, mane=LEATHER, length=140, height=112, girth=44, neck=44, neck_up=40,
                     head=36, ears=LEATHER, tail=40, graze=10, wag=22, rate=0.5, phase=0.3, snout=CREAM)
    pk = sc.part(None, (390, 120, H), -60, 1.25)
    pk.box(RED, (0, 6, 116), (48, 60, 8))
    for side in (-1, 1):
        pk.box('#c9a070', (side * 32, 10, 96), (24, 56, 44))
    pk.box(TEAL, (0, 14, 132), (40, 70, 22))
    tilt_box(pk, WOOD, (-10, -26, 140), (20, 50, 150), 5, 5)
    sage(g, r, -hx * 0.6, -hy * 0.5, H, 0.9)
    flag(sc, hx * 0.8, hy * 0.6, H, 430, RED, patch=GOLD, w=190, h=120, pole=BARK, phase=0.5)


def lizard(sc, x, y, z, yaw=0.0, s=2.0, body=TEAL, bands=YEL, head=YEL, phase=0.0, rate=1.2, press=8.0, travel=None):
    """A collared lizard, its head towards -Y at yaw 0: it does press-ups (the whole of it hops), its tail sweeps."""
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])
    tv = (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)
    common = dict(phase=phase, rate=rate, bob=press * s, base=(x, y), travel=tv)
    b = sc.part(P.motion(pivot=W((0, 0, 0)), **common), (x, y, z), yaw, s)
    b.box(body, (0, 0, 17), (26, 82, 15))
    for k in range(3):
        b.box(bands, (0, -22 + k * 24, 17.5), (27.5, 8, 16))
    b.box(LEATHER, (0, -42, 18), (25, 6, 16.5))                                   # the collar
    tilt_box(b, head, (0, -42, 20), (0, -74, 27), 22, 14)
    for sx in (-1, 1):
        b.box(P.EYE, (sx * 10, -64, 31), (4, 5, 4))
        tilt_box(b, body, (sx * 12, -30, 16), (sx * 30, -40, 0), 7, 7)            # the front legs: propped up
        tilt_box(b, body, (sx * 12, 30, 14), (sx * 32, 40, 0), 8, 8)
    t = sc.part(P.motion(swing=24.0, pivot=W((0, 40, 14)), axis=P.UP, **common), (x, y, z), yaw, s)
    tilt_box(t, body, (0, 38, 16), (0, 82, 10), 15, 11)
    tilt_box(t, bands, (0, 82, 10), (0, 132, 5), 9, 7)


def lizards(sc):
    """A sun-baked boulder and what lives on it: three collared lizards doing press-ups, one scuttling, a tortoise."""
    r = random.Random(81)
    g = sc.still()
    H = 215.0
    hx, hy = rock_stack(g, r, 760, 520, H, tones=(SANDST_D, SANDST, SANDST_L), band=108.0, top=0.8, cap=SAND)
    g.sphere(SANDST_L, (-hx + 30, hy - 40, H + 30), 95, scale=(1.2, 1, 0.75), steps=(4, 7))
    lizard(sc, -90, -40, H, yaw=-25, s=3.0, phase=0.0, rate=1.3)
    lizard(sc, 170, 40, H, yaw=30, s=2.5, body='#b8ff5a', bands=TEAL, head='#ff7a3c', phase=0.45, rate=1.5)
    lizard(sc, -hx + 30, hy - 40, H + 95, yaw=10, s=2.1, body='#57c7ff', bands=WHITE, head=YEL, phase=0.7, rate=1.1)
    lizard(sc, 0, -hy + 50, H, yaw=90, s=1.5, body='#ff7a3c', bands=LEATHER, head='#ff7a3c', phase=0.2, rate=3.2, press=2.0, travel=(2 * hx - 80, 190))
    # the tortoise: a dome, a head that nods out and in
    tx, ty = hx - 110, hy - 90
    g.sphere('#8fb86a', (tx, ty, H + 26), 52, scale=(1.2, 1, 0.75), steps=(4, 7))
    g.sphere('#5f8f4a', (tx, ty, H + 44), 34, scale=(1.2, 1, 0.6), steps=(3, 6))
    hd = sc.part(P.motion(swing=3.0, pivot=(tx, ty, H - 300.0), axis=90.0, rate=0.3))
    hd.box('#d8b060', (tx, ty - 62, H + 26), (20, 34, 18))
    sage(g, r, hx * 0.6, -hy * 0.6, H, 0.9)


def riders(sc):
    """Ranch hands riding the line on a low dyke: three at the trot, hats in the air, a longhorn trotting ahead of
    them and the dog behind. They go the length of the piece (38 m) and come round again."""
    r = random.Random(91)
    g = sc.still()
    L, Z = 3800.0, 95.0
    pts = [(-L / 2.0 - 150, -170), (-L / 4.0, -200), (0, -180), (L / 4.0, -205), (L / 2.0 + 150, -170), (L / 2.0 + 150, 170), (L / 4.0, 200), (0, 185), (-L / 4.0, 205), (-L / 2.0 - 150, 170)]
    g.plan('#e8a868', [(px, py * 1.25) for (px, py) in pts], -120, Z * 0.5)
    g.plan(SAND, pts, Z * 0.5, Z)
    for k in range(5):
        sage(g, r, -L / 2.0 + 300 + k * 800 + r.uniform(-150, 150), r.choice((-1, 1)) * 175, Z - 20, 0.8)
    sp = 560.0
    coats = (('#fff6ea', '#cfcfd8'), ('#e8c26a', '#fff6ea'), ('#b87a50', '#fff6ea'))
    for i, (coat, mane) in enumerate(coats):
        d = cowhand(r, 1.18, top=(RED, CYAN, GOLD)[i])
        d.update(arm_r='wave', hold_r=hold_hat(d['hat_col']), hat=None, swing_r=26) if i != 1 else d.update(arm_r='up', hold_r=P.hold_flag(col=MAG, w=120), swing_r=14)
        mount(sc, r, -500 + i * 470, (-40, 50, -10)[i], Z, yaw=90, s=1.3, coat=coat, mane=mane, rider=d, phase=i * 0.31, rate=2.0, bob=7, stride=24,
              travel=(L, sp), neck_up=48)
    P.quadruped(sc, 800, 20, Z, yaw=90, s=1.35, length=190, height=125, girth=62, neck=34, neck_up=14, head=40, body='#fff6ea', belly='#b8553a', horns=CREAM,
                snout='#eebf98', ears='#b8553a', tail=50, tail_col='#b8553a', stride=24, wag=16, rate=2.0, bob=5, phase=0.2, travel=(L, sp))
    P.quadruped(sc, -1000, -60, Z, yaw=90, s=1.3, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=30, rate=3.4, bob=6,
                travel=(L, sp), body=LEATHER, belly=WHITE)


def rim_posse(sc):
    """A posse sitting their horses on the canyon's rim, watching the chase go through below: hats waving, a banner."""
    r = random.Random(95)
    g = sc.still()
    hx, hy = rim_column(g, r, 1900, 520)
    H = RIM_H
    coats = (('#fff6ea', '#cfcfd8'), ('#8e5a3a', '#3b2a22'), ('#2a2230', '#22202a'), ('#e8c26a', '#fff6ea'))
    for i, (coat, mane) in enumerate(coats):
        d = cowhand(r, 1.9, detail=0)
        if i == 1:
            d.update(arm_r='up', hold_r=P.hold_flag(col=RED, w=90), swing_r=14, rate=0.8)
        elif i == 3:
            d.update(arm_l='point', swing_l=9, axis_l='x', arm_r='hip')
        else:
            d.update(arm_r='wave', hold_r=hold_hat(d['hat_col']), hat=None, swing_r=28)
        mount(sc, r, -hx + 150 + i * (2 * hx - 300) / 3.0, r.uniform(-30, 40), H, yaw=r.uniform(-25, 25), s=2.0, coat=coat, mane=mane, rider=d,
              phase=r.random(), rate=0.6 + 0.1 * i, graze=7, neck_up=50)
    sage(g, r, hx - 40, hy - 40, H, 1.2)


def cattle(sc):
    """A herd on the range: a dozen head grazing and drifting, calves, and a hand riding slow circles round them."""
    r = random.Random(101)
    g = sc.still()
    kinds = (dict(body='#fff6ea', belly='#b8553a', ears='#b8553a', tail_col='#b8553a'), dict(body='#b8553a', snout=WHITE, belly=WHITE, ears='#b8553a'),
             dict(body='#2a2230', snout='#cfcfd8', ears='#2a2230'), dict(body='#e8c26a', belly=CREAM, ears='#e8c26a'), dict(body='#fff6ea', ears='#2a2230', tail_col='#2a2230'))
    cow = dict(length=190, height=125, girth=62, neck=36, head=42, tail=52)
    n = 12
    for i in range(n):
        a = 2 * math.pi * i / n + r.uniform(-0.2, 0.2)
        rad = r.uniform(250, 1050)
        x, y = math.cos(a) * rad * 1.35, math.sin(a) * rad * 0.7
        k = dict(kinds[i % len(kinds)])
        s = 1.7 * r.uniform(0.9, 1.08) * (0.6 if i % 5 == 4 else 1.0)
        if i % 4 == 1:      # walking, the length of the herd
            P.quadruped(sc, x, y, 0, yaw=90, s=s, neck_up=8, stride=18, wag=18, rate=0.9, phase=r.random(), travel=(3400, 110), horns=(CREAM if i % 2 else None), **dict(cow, **k))
        else:
            P.quadruped(sc, x, y, 0, yaw=r.uniform(0, 360), s=s, neck_up=-28, graze=r.uniform(10, 18), wag=24, rate=r.uniform(0.25, 0.45), phase=r.random(),
                        horns=(CREAM if i % 3 == 0 else None), **dict(cow, **k))
    # the trough
    g.box(STEEL, (0, 0, 40), (260, 80, 60))
    g.box('#57c7ff', (0, 0, 71), (240, 62, 4))
    d = cowhand(r, 1.6, top=RED)
    d.update(arm_r='wave', hold_r=hold_hat(d['hat_col']), hat=None, swing_r=24)
    mount(sc, r, 2100, 0, 0, yaw=180, s=1.8, coat='#8e5a3a', mane='#3b2a22', rider=d, rate=1.8, bob=8, stride=24, orbit=(0.0, 0.0, 0.04), neck_up=48)
    P.quadruped(sc, -1900, 300, 0, yaw=0, s=1.6, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=30, rate=3.2, bob=6,
                orbit=(0.0, 0.0, 0.04), body=LEATHER, belly=WHITE)


# ------------------------------------------------------------------------------------------ the strip
def stand(seed, kind):
    def build(sc):
        """The strip's grandstand: four tiers of gamblers and tourists hopping out of step, arms and flags up, pennants
        along the back. kind 1: the front row holds striped cards over its head and a wave runs along the stand."""
        r = random.Random(seed)
        g = sc.still()
        L, rows, step, rise, H0 = 1150.0, 4, 82.0, 56.0, 150.0
        c1, c2, c3 = ((PURP, GOLD, MAG), (TEAL, CREAM, GOLD))[kind]
        y0 = -rows * step / 2.0
        for k in range(rows):
            h = H0 + k * rise
            g.box(c1 if k % 2 == 0 else c2, (0, y0 + (k + 0.5) * step, h / 2.0), (L, step, h))
        g.box(c3, (0, y0 - 3, H0 - 26), (L + 10, 6, 52))
        for k in range(8):
            g.box(c2 if k % 2 else c1, (-L / 2.0 + (k + 0.5) * L / 8.0, y0 - 6.5, H0 - 26), (L / 8.0 * 0.5, 2, 30))
        for sx in (-1, 1):
            g.box(c1, (sx * (L / 2.0 + 6), 0, (H0 + rows * rise) / 2.0), (12, rows * step, H0 + rows * rise))
        g.box(CREAM, (0, y0 + 5, H0 + 50), (L, 6, 6))
        # the back wall and its pennants
        top = H0 + (rows - 1) * rise
        yb = -y0 + 5
        g.box(c1, (0, yb, (top + 150) / 2.0), (L + 24, 10, top + 150))
        for sx in (-1, 0, 1):
            g.box(CREAM, (sx * (L / 2.0 - 10), yb, top + 205), (8, 8, 110))
        g.box(CREAM, (0, yb, top + 256), (L, 5, 5))
        for k in range(14):
            px = -L / 2.0 + 40 + k * (L - 80) / 13.0
            g.side(HOT[k % len(HOT)], [(px - 28, top + 254), (px + 28, top + 254), (px, top + 184)], yb - 3, yb + 3)
        acts = (
            dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=9, rate=1.6),
            dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=11, rate=1.3),
            dict(arm_r='wave', swing_r=26, rate=1.3, bob=5),
            dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.5, bob=3),
            dict(arm_r='up', hold_r=P.hold_flag(col=r.choice(HOT), w=64), swing_r=22, rate=1.0, bob=6),
            dict(arm_r='point', swing_r=10, axis_r='x', rate=0.8, bob=4),
        )
        per = 5
        for k in range(rows):
            for i in range(per):
                x = -L / 2.0 + (i + 0.5 + (0.25 if k % 2 else -0.25) * 0.6) * L / per + r.uniform(-18, 18)
                d = P.look(r, tops=HOT)
                d['s'] = 1.2 * r.uniform(0.94, 1.06)
                if r.random() < 0.16:
                    d.update(build='kid', s=0.8)
                if r.random() < 0.3:
                    d.update(hat=r.choice(('cowboy', 'cap', 'sun')), hat_col=r.choice((CREAM, RED, GOLD, WHITE)))
                if r.random() < 0.25:
                    d.update(wear='dress')
                ph = r.random()
                if kind == 1 and k == 0:
                    ph = i / float(per) * 0.9                                   # the wave runs along the front row
                    a = dict(arm_l='up', arm_r='up', bob=16, rate=0.7, hold_r=_card(HOT[(i * 3) % len(HOT)], HOT[(i * 3 + 4) % len(HOT)]))
                else:
                    a = dict(acts[(i * 5 + k * 3 + seed) % len(acts)])
                d.update(a)
                P.figure(sc, x, y0 + (k + 0.5) * step, z=H0 + k * rise, yaw=r.uniform(-14, 14), phase=ph, react=(kind == 0 or k > 0),
                         detail=(1 if k == 0 else 0), **d)
    return build


def _card(col, stripe):
    """A big card held overhead in both hands (drawn from the right hand, centred over the head)."""
    def f(m, h, side):
        m.box(col, (h[0] - 30, h[1], h[2] + 34), (86, 4, 60))
        m.box(stripe, (h[0] - 30, h[1] - 3, h[2] + 34), (86, 2, 18))
    return f


def podium(g, L, D, H, body=CREAM, edge=GOLD, carpet=RED, steps=2, y=0.0):
    """A casino's raised front: a marble block with a gilded edge, steps down the front with a carpet over them."""
    g.box(body, (0, y, H / 2.0 - 60), (L, D, H + 120))
    g.box(edge, (0, y - D / 2.0 - 2, H - 12), (L + 8, 5, 24))
    for k in range(steps):
        h = H * (steps - k) / (steps + 1.0)
        g.box(body, (0, y - D / 2.0 - 22 - k * 44, h / 2.0 - 60), (360, 46, h + 120))
        g.box(carpet, (0, y - D / 2.0 - 22 - k * 44, h + 1.5), (200, 46, 3))
    g.box(carpet, (0, y, H + 1.5), (200, D, 3))


def rope_posts(g, xs, y, z0, rope=PURP):
    for i, x in enumerate(xs):
        g.cyl(GOLD, (x, y, z0), 7, 92, steps=6)
        g.sphere(GOLD, (x, y, z0 + 98), 10, steps=(3, 6))
        if i:
            g.box(rope, ((x + xs[i - 1]) / 2.0, y, z0 + 76), (abs(x - xs[i - 1]), 6, 7))


def valets(sc):
    """A casino's carpeted steps: two valets running with keys, the doorman ushering, a couple in evening dress
    coming down, a bellhop sliding the luggage cart, under a striped marquee."""
    r = random.Random(111)
    g = sc.still()
    H = 190.0
    podium(g, 1000, 360, H)
    rope_posts(g, (-470, -330, -190), -150, H)
    rope_posts(g, (190, 330, 470), -150, H)
    # the marquee: four gold posts, a purple and gold striped top
    for sx in (-1, 1):
        for sy in (-1, 1):
            g.box(GOLD, (sx * 300, 40 + sy * 120, H + 150), (10, 10, 300))
    for k in range(6):
        g.box(PURP if k % 2 else GOLD, (-250 + k * 100, 40, H + 306), (100, 290, 12))
    g.box(MAG, (0, -108, H + 290), (620, 6, 26))
    uni = dict(top=RED, bottom=LEATHER, sleeves='long', band=GOLD, hat='peak', hat_col=RED, shoes=LEATHER)
    P.figure(sc, -200, -95, z=H, yaw=90, legs='stride', stride=30, swing_l=26, swing_r=-26, axis_l='x', axis_r='x', rate=2.2, bob=5, travel=(900, 420),
             **person(r, 1.15, **uni))
    P.figure(sc, 250, 150, z=H, yaw=-90, legs='stride', stride=30, swing_l=26, swing_r=-26, axis_l='x', axis_r='x', rate=2.2, bob=5, travel=(900, -380),
             phase=0.4, **person(r, 1.15, **uni))
    P.figure(sc, -120, 30, z=H, yaw=-14, arm_r='fwd', swing_r=14, axis_r='z', arm_l='back', rate=0.6,
             **person(r, 1.2, top=PURP, bottom=PURP, wear='coat', sleeves='long', band=GOLD, hat='peak', hat_col=PURP, build='stocky'))
    P.figure(sc, 40, -20, z=H, yaw=6, arm_r='wave', swing_r=22, rate=1.1, bob=3, arm_l='down', react=True,
             **person(r, 1.15, top=LEATHER, bottom=LEATHER, sleeves='long', band=WHITE, hair='short'))
    P.figure(sc, 110, -10, z=H, yaw=-8, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=6, rate=1.3, react=True, phase=0.3,
             **person(r, 1.12, top=MAG, wear='dress', sleeves='none', hair='bun'))
    # the luggage cart, sliding a metre each way, and its bellhop
    slide = dict(swing=1.0, pivot=(380.0, 60.0, -6000.0), axis=90.0)
    ct = sc.part(P.motion(rate=0.16, **slide))
    ct.box(GOLD, (380, 60, H + 16), (120, 66, 8))
    for sx in (-1, 1):
        ct.box(GOLD, (380 + sx * 56, 60, H + 100), (6, 6, 170))
    ct.box(GOLD, (380, 60, H + 186), (118, 6, 6))
    for k, col in enumerate((TEAL, '#c94f6a', '#ffc93c')):
        ct.box(col, (380 + (k - 1) * 10, 60, H + 36 + k * 30), (84 - k * 12, 52, 28))
    P.figure(sc, 470, 60, z=H, yaw=90, arm_l='push', arm_r='push', rate=0.16, sway=slide, **person(r, 1.12, **uni))


def wedding(sc):
    """A chapel-of-chance wedding out on its dais: the couple jumping for joy hand in hand, the officiant behind
    them, guests throwing confetti, a photographer."""
    r = random.Random(121)
    g = sc.still()
    H = 190.0
    podium(g, 900, 380, H, body=WHITE, edge=PINK, carpet=PINK)
    # the arch: two posts, a beam, flowers along it
    for sx in (-1, 1):
        g.box(WHITE, (sx * 150, 90, H + 150), (14, 14, 300))
    g.box(WHITE, (0, 90, H + 304), (330, 16, 14))
    for k in range(9):
        g.sphere((PINK, WHITE, MAG)[k % 3], (-140 + k * 35, 86, H + 318 + (8 if k % 2 else 0)), 20, steps=(3, 6))
    for sx in (-1, 1):
        for k in range(3):
            g.sphere((MAG, WHITE, PINK)[k], (sx * 150, 84, H + 250 - k * 50), 17, steps=(3, 6))
    balloons(sc, -430, 150, H, phase=0.1)
    balloons(sc, 430, 150, H, phase=0.6)
    j = dict(bob=11, rate=1.2, phase=0.0)
    P.figure(sc, -42, -20, z=H, yaw=8, arm_l='up', arm_r='hip', pack=WHITE, hold_r=None,
             **dict(person(r, 1.15, top=WHITE, bottom=WHITE, wear='dress', sleeves='none', hair='long', hat=None), **j))
    P.figure(sc, 42, -20, z=H, yaw=-8, arm_r='up', arm_l='wave', swing_l=18,
             **dict(person(r, 1.2, top='#57c7ff', bottom='#57c7ff', sleeves='long', band=WHITE, hair='short'), **j))
    P.figure(sc, 0, 80, z=H + 30, arm_l='hold', arm_r='hold', hold_l=C.hold_clipboard(), rock=3, rate=0.4,
             **person(r, 1.15, top=GOLD, bottom=LEATHER, sleeves='long', hat='cowboy', hat_col=WHITE))
    g.box(WHITE, (0, 80, H + 15), (70, 60, 30))
    spots = ((-330, -60), (-250, 30), (-390, 60), (250, 30), (330, -60), (390, 60))
    for i, (x, y) in enumerate(spots):
        d = person(r, 1.12, tops=HOT)
        if i % 3 == 0:
            d.update(wear='dress')
        if i == 2:
            d.update(build='kid', s=0.78)
        P.figure(sc, x, y, z=H, yaw=(28 if x < 0 else -28), arm_l='vee', arm_r='vee', swing_l=16, swing_r=-16, bob=8, rate=1.5, phase=r.random(), react=True, **dict(d, detail=0))
    P.figure(sc, 200, -120, z=H, yaw=110, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.5, **person(r, 1.12, top=LEATHER))
    # the confetti: four showers, floating out of step
    for k in range(4):
        cf = sc.part(P.motion(bob=-26.0, rate=0.55 + 0.07 * k, phase=k * 0.27))
        for n in range(7):
            cf.box(HOT[(k * 7 + n) % len(HOT)], (r.uniform(-300, 300), r.uniform(-120, 60), H + r.uniform(215, 330)), (9, 9, 5), rot=(r.uniform(-40, 40), r.uniform(0, 90), 0))


def gamblers(sc):
    """A terrace of slot machines seen side on: four players pulling the levers, one jackpot (the winner jumping,
    the coins hopping in the tray), a high roller in a white suit pointing at the chase, under a scalloped canopy."""
    r = random.Random(131)
    g = sc.still()
    H = 190.0
    podium(g, 1050, 340, H, body=PURP, edge=GOLD, carpet=MAG)
    for sx in (-1, 1):
        g.box(GOLD, (sx * 500, 130, H + 150), (10, 10, 300))
        g.box(GOLD, (sx * 500, -130, H + 150), (10, 10, 300))
    g.box(PURP, (0, 0, H + 306), (1040, 300, 12))
    for k in range(10):
        g.side(GOLD if k % 2 else MAG, [(-520 + k * 104, H + 300), (-416 + k * 104, H + 300), (-468 + k * 104, H + 262)], -153, -147)
    balloons(sc, -505, -150, H, height=250, phase=0.3, n=5)
    cols = (RED, CYAN, GOLD, MINT)
    for i in range(4):
        x = -330 + i * 230
        g.box(cols[i], (x, 40, H + 85), (60, 70, 170))
        g.box(CREAM, (x - 31, 40, H + 118), (3, 50, 44))
        g.box(LEATHER, (x - 32.5, 40, H + 118), (2, 40, 12))
        g.box(CHROME, (x - 40, 40, H + 70), (22, 60, 8))
        g.box(GOLD, (x, 40, H + 176), (64, 74, 12))
        rate, ph = 0.7 + 0.08 * i, i * 0.23
        lv = sc.part(P.motion(swing=28.0, pivot=(x, 0.0, H + 100), axis=90.0, rate=rate, phase=ph))
        lv.box(CHROME, (x, -2, H + 130), (6, 6, 64))
        lv.sphere(RED, (x, -2, H + 166), 10, steps=(3, 6))
        if i == 2:
            continue
        d = person(r, 1.15, tops=HOT)
        if i == 1:
            d.update(wear='dress', hair='bun')
        P.figure(sc, x - 86, 40, z=H, yaw=90, arm_l='fwd', swing_l=24, axis_l='x', arm_r='hold', hold_r=P.hold_cup(GOLD), rate=rate, phase=ph, **d)
    # the jackpot at the third machine
    x = -330 + 2 * 230
    P.figure(sc, x - 96, -40, z=H, yaw=20, arm_l='vee', arm_r='vee', swing_l=16, swing_r=-16, bob=14, rate=1.7, react=True, **person(r, 1.15, top='#b8ff5a', hat='cap', hat_col=RED))
    for k in range(3):
        cn = sc.part(P.motion(bob=34.0, rate=1.6 + 0.3 * k, phase=k * 0.31))
        for n in range(4):
            cn.cyl(GOLD, (x - 44 + r.uniform(-12, 12), 40 + r.uniform(-24, 24), H + 76), 8, 5, steps=6)
    P.figure(sc, 420, -90, z=H, yaw=-10, arm_r='point', swing_r=11, axis_r='x', arm_l='hold', hold_l=P.hold_cup(GOLD), rate=0.7, react=True,
             **person(r, 1.2, top=WHITE, bottom=WHITE, sleeves='long', hat='cowboy', hat_col=WHITE, band=GOLD, build='stocky'))
    P.figure(sc, -470, -90, z=H, yaw=14, arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4, react=True,
             **person(r, 1.12, top=MAG, wear='dress', hair='afro'))


def crew_riser(sc):
    """The film crew's riser: a scaffold deck two metres up, so that a camera position by this road looks over the
    barrier. The table stands a crew piece on it (a set: the piece 2 m up). A pink pennant on the corner pole."""
    g = sc.still()
    L, D, H = 1360.0, 440.0, 200.0
    y = -5.0
    g.box(C.DARK, (0, y, H - 7), (L, D, 14))
    g.box(C.YEL, (0, y - D / 2.0 - 2, H - 7), (L + 6, 5, 16))
    n = 6
    for k in range(n):
        x = -L / 2.0 + 10 + k * (L - 20) / (n - 1.0)
        for sy in (-1, 1):
            g.box(C.STEEL, (x, y + sy * (D / 2.0 - 8), (H - 14) / 2.0), (8, 8, H - 14))
        if k < n - 1:
            x2 = -L / 2.0 + 10 + (k + 1) * (L - 20) / (n - 1.0)
            tilt_box(g, C.YEL, (x, y - D / 2.0 + 8, 16), (x2, y - D / 2.0 + 8, H - 30), 6, 6)
    g.box(C.STEEL, (0, y - D / 2.0 + 8, 60), (L, 6, 6))
    for k in range(12):
        g.box(C.PINK if k % 2 else C.WHITE, (-L / 2.0 + (k + 0.5) * L / 12.0, y - D / 2.0 - 5, H - 40), (L / 12.0, 3, 26))
    # the back rail and a ladder at one end
    for k in range(n):
        x = -L / 2.0 + 10 + k * (L - 20) / (n - 1.0)
        g.box(C.STEEL, (x, y + D / 2.0 - 6, H + 55), (7, 7, 110))
    g.box(C.YEL, (0, y + D / 2.0 - 6, H + 106), (L, 7, 7))
    g.box(C.YEL, (0, y + D / 2.0 - 6, H + 56), (L, 5, 5))
    for sy in (-1, 1):
        tilt_box(g, C.STEEL, (L / 2.0 + 60, y + 60 + sy * 22, 0), (L / 2.0 + 6, y + 60 + sy * 22, H), 6, 6)
    for k in range(6):
        g.box(C.STEEL, (L / 2.0 + 60 - (k + 0.5) * 9, y + 60, (k + 0.5) * H / 6.0), (6, 44, 5))
    flag(sc, -L / 2.0 + 10, y + D / 2.0 - 6, H, 460, C.PINK, patch=C.WHITE, w=200, h=120, pole=C.STEEL, phase=0.2)


JOBS = [
    ('SM_RoadhouseDeck', roadhouse_deck, dict(grow=(60.0, 60.0, 80.0))),
    ('SM_DinerDeck', diner_deck),
    ('SM_Hitchers_A', hitchers(51, 0)),
    ('SM_Hitchers_B', hitchers(52, 1)),
    ('SM_Prospectors', prospectors),
    ('SM_Lizards', lizards),
    ('SM_Riders', riders),
    ('SM_StripStand_A', stand(141, 0)),
    ('SM_StripStand_B', stand(142, 1)),
    ('SM_Valets', valets),
    ('SM_Wedding', wedding),
    ('SM_Gamblers', gamblers),
    ('SM_RimCoyotes', rim_coyotes),
    ('SM_RimPosse', rim_posse),
    ('SM_WildHorses', wild_horses, dict(far=True, grow=(7200.0, 120.0, 120.0), grow_down=(7200.0, 120.0, 0.0))),
    ('SM_Cattle', cattle, dict(far=True, grow=(900.0, 2400.0, 120.0), grow_down=(900.0, 2400.0, 0.0))),
    ('SM_CrewRiser', crew_riser),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
