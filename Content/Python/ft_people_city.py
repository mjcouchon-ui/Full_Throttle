"""FULL THROTTLE (Unreal) the population, level 7: City Center (Midtown at midnight). /Game/People/CityCenter/.

Who lives here: the city has come out to watch the last scene being shot, and to see the Chairman fall. Onlookers behind
police barricades, a march behind its long striped banner, commuters going home past the lit shop windows, a pretzel
cart and its queue, a picture-house queue behind a gold rope, parties on the podiums' roofs under strings of lights,
rallies up there with banners on poles, window cleaners in a cradle on the glass infills, a rally on the plaza before the monument, the Syndicate's own men on its black walls and at its barrier
rows, pigeons on the pavement and over the gaps, a great flock wheeling over the avenue's roofs.
Placed by Source/FullThrottle/FTPeopleL7.inl (which says where each piece may stand and why).

HOW THE STREET LIES (FTDressL7.inl, ft_dress_metro.py), and so where people can stand:
  - the kerb (17 to 22.6 m) is full of the dressing's parked cars, benches, bus shelters, cafes and road works, slot after
    slot. What is free in EVERY slot is a POCKET in front of the gap between two podiums: 17.5 to 24.5 m along from
    the slot's anchor and 19.4 to 23.1 m out. The low pieces here are made for it: at most 6.4 m long and 3.4 m deep;
  - a strip 23.2 to 24.0 m out, under the awnings' edge, is free the whole length of every block: the commuters walk it;
  - nothing over 2 m may stand nearer than 24 m, and the podiums' fronts are 25.5 m out: everything tall of ours stands
    ON the dressing (a podium's roof, an alley's fire escape, a glass infill's face) or in a gap (the plaza before a
    monument, 24 to 27.9 m out), by Site sets that copy the dressing's own every-and-phase;
  - it is night. A piece at 24 m or more carries steady light of its own (strings of bulbs, a lit floor, a lit window
    behind) and its figures a faint light (lit(), below); nearer pieces carry none (the light rule) and read by strong
    colour under the street's lamps.
Banners, placards and flags carry stripes, discs and squares: never words, never a chevron (the Syndicate's mark).

Run in the editor:  import ft_people_city; ft_people_city.run()        (or run(['SM_RoofParty_A']))
"""
import math
import random
import unreal
import ft_people as P
import ft_people_crew as C
from ft_desert import rod, tilt_box

DIR = '/Game/People/CityCenter'
NAVY, DARK, GREY, STEEL, WHITE, CREAM = '#2b2350', '#22202a', '#3f3d50', '#8f9bb3', '#fff6ea', '#fff1d6'
PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, BLUE = '#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#ff2d3c', '#ff7a3c', '#8f6bff', '#3a5fa8'
WARM, WARM_L, GOLD, WOOD, BLACK = '#ffe28a', '#fff1c0', '#ffc93c', '#9a6a44', '#1c1a26'
POLICE, POLICE_L = '#3a5fa8', '#57c7ff'
HOT = (PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, WHITE, '#f4a6ff', '#b8ff5a', '#57c7ff')     # coats that read at night
G_BULB, G_FLOOR, G_PANEL, G_FIG = 1.6, 0.3, 0.7, 0.05


def lit(sc, g=G_FIG):
    """Night: everything of a piece that carries no light of its own gets a faint one (a figure under the party's
    lamps). ONLY for pieces the table stands 24 m or more from the road (the light rule: no glow nearer)."""
    for key in sc.order:
        model = sc.parts[key][0]
        new = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, glow if glow > 0.0 else g)
            if k2 in new:
                unreal.GeometryScript_MeshEdits.append_mesh(new[k2], pm, unreal.Transform())
            else:
                new[k2] = pm
        model.meshes = new


def person(r, s_max=1.0, **fixed):
    """A citizen in a coat that reads at night, never over 1.72 m (the low pieces must stay under 2 m with arms up)."""
    d = P.look(r, tops=HOT, **fixed)
    d['s'] = fixed.get('s', r.uniform(0.9, s_max))
    if d.get('build') == 'kid':
        d['s'] = r.uniform(0.6, 0.7)
    return d


def officer(r, **fixed):
    """The city's police: a blue coat, a white band, a peaked cap, white gloves are not drawn (no real force's dress)."""
    d = P.look(r, tops=(POLICE,), bottoms=(NAVY,), sleeves='long', wear='trousers', hat='peak', hat_col=POLICE, band=WHITE, shoes=DARK)
    d['s'] = r.uniform(0.94, 1.0)
    d.update(fixed)
    return d


def sawhorse(m, x, y, L=190.0, col=POLICE):
    """A police barricade: two A-frames and a board in blue and white bars, 1.05 m high."""
    for sx in (-1, 1):
        tilt_box(m, col, (x + sx * (L / 2.0 - 16), y - 26, 0), (x + sx * (L / 2.0 - 16), y, 100), 7, 7)
        tilt_box(m, col, (x + sx * (L / 2.0 - 16), y + 26, 0), (x + sx * (L / 2.0 - 16), y, 100), 7, 7)
    n = 6
    for k in range(n):
        m.box(WHITE if k % 2 else col, (x - L / 2.0 + (k + 0.5) * L / n, y, 92), (L / n, 6, 22))


def bulbs(m, a, b, n, sag=40.0, cols=(WARM, PINK, CYAN, YEL), glow=G_BULB, wire=True):
    """A string of lights from a to b, sagging: steady (a party's own light)."""
    prev = a
    for k in range(1, n + 1):
        t = k / float(n + 1)
        p = (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t - sag * 4.0 * t * (1.0 - t))
        m.box(cols[k % len(cols)], p, (17, 17, 17), glow=glow)
        if wire:
            tilt_box(m, DARK, prev, p, 4, 4)
        prev = p
    if wire:
        tilt_box(m, DARK, prev, b, 4, 4)


def cloth(m, x0, x1, y, z0, z1, base=CREAM, bars=(PINK, CYAN, YEL), kind=0, glow=0.0, t=5.0):
    """A banner between x0 and x1: stripes and shapes, never words. kind 0: three bars and a disc; 1: bars up and down
    and a square; 2: a broad band and two discs; 3: halves."""
    w, h, xc, zc = x1 - x0, z1 - z0, (x0 + x1) / 2.0, (z0 + z1) / 2.0
    m.box(base, (xc, y, zc), (w, t, h), glow=glow)
    f = y - t / 2.0 - 0.8
    if kind == 0:
        for k, c in enumerate(bars):
            m.box(c, (xc + w * 0.14, f, z0 + h * (0.2 + 0.3 * k)), (w * 0.6, 1.6, h * 0.17), glow=glow)
        m.cyl(bars[0], (x0 + w * 0.13, f + 0.8, zc), h * 0.3, 1.6, rot=(0, 0, 90), steps=10, glow=glow)
    elif kind == 1:
        n = 7
        for k in range(n):
            if k % 2 == 0:
                m.box(bars[(k // 2) % len(bars)], (x0 + (k + 0.5) * w / n, f, zc), (w / n, 1.6, h * 0.9), glow=glow)
    elif kind == 2:
        m.box(bars[0], (xc, f, zc), (w * 0.96, 1.6, h * 0.4), glow=glow)
        for sx in (-1, 1):
            m.cyl(bars[1], (xc + sx * w * 0.3, f - 0.2, zc), h * 0.3, 1.8, rot=(0, 0, 90), steps=10, glow=glow)
    else:
        m.box(bars[0], (x0 + w * 0.25, f, zc), (w * 0.5, 1.6, h * 0.96), glow=glow)
        m.box(bars[1], (x0 + w * 0.75, f, z0 + h * 0.3), (w * 0.4, 1.6, h * 0.24), glow=glow)
        m.box(bars[2], (x0 + w * 0.75, f, z0 + h * 0.7), (w * 0.4, 1.6, h * 0.24), glow=glow)


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=7, rate=1.6),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.4),          # clapping
    dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7),
    dict(arm_r='wave', swing_r=26, rate=1.3, bob=3),
    dict(arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.4),
    dict(arm_l='wave', arm_r='wave', swing_l=-22, swing_r=22, rate=1.1, bob=5),
    dict(arm_r='up', arm_l='down', swing_r=18, rate=1.8, bob=6),                                           # a fist pumping
)
DANCE = (
    dict(arm_l='vee', arm_r='vee', swing_l=16, swing_r=-16, bob=9, rate=1.5),
    dict(arm_l='out', arm_r='up', turn=24, rate=0.8),
    dict(arm_l='up', arm_r='up', rock=9, rate=0.9),
    dict(arm_l='hip', arm_r='wave', swing_r=28, rate=1.5, bob=6),
    dict(arm_l='wave', arm_r='hip', turn=30, rate=0.75),
    dict(arm_l='up', arm_r='out', swing_l=18, bob=10, rate=1.5),
)


# ------------------------------------------------------------------------------------------ by the barrier (low, unlit)
def onlookers(seed=1, kids=(3,)):
    """LOW, a pocket piece (6 m by 2 m): the city behind a police barricade, cheering the chase on. Ten people two
    deep, the back row on the kerb stone."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 1):
            sawhorse(g, sx * 150, -150, L=280.0)
        g.box(GREY, (0, -2, 9), (580, 64, 18))                      # the back row stands on the kerb stone
        for i in range(10):
            front = i % 2 == 0
            x = -262 + i * 58 + r.uniform(-7, 7)
            y = (-92 if front else -2) + r.uniform(-7, 7)
            d = person(r, build='kid') if i in kids else person(r)
            d.update(CHEER[(i * 3 + seed) % len(CHEER)])
            if not front:
                d['hair'] = r.choice(('short', 'long', 'pony', 'bald'))
                d['hat'] = None
            P.figure(sc, x, y, z=0 if front else 18, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=1 if front else 0, **d)
    return build


def march(seed=2, kind=0, bars=(PINK, CYAN, YEL), police=False):
    """LOW, a pocket piece: a march behind its long banner (stripes and a disc: never words), fists up behind it.
    police: the city's police in a line before it, arms out, holding it at the kerb."""
    def build(sc):
        r = random.Random(seed)
        ban = sc.part(P.motion(swing=5.0, pivot=(0.0, -112.0, 150.0), axis=0.0, rate=0.5, phase=r.random()))
        cloth(ban, -280, 280, -112, 78, 150, bars=bars, kind=kind)
        for i in range(5):
            x = -240 + i * 120
            P.figure(sc, x, -80, yaw=r.uniform(-6, 6), arm_l='carry', arm_r='carry', rate=0.5, bob=-1.5, phase=r.random(), detail=0, **person(r))
        for i in range(6 if police else 7):
            x = -270 + i * (108 if police else 90) + r.uniform(-8, 8)
            d = person(r)
            d.update(CHEER[(0, 6, 4, 6, 3, 0, 6)[i]])
            P.figure(sc, x, 8 + r.uniform(-10, 10), yaw=r.uniform(-12, 12), phase=r.random(), react=True, detail=0, **d)
        if police:
            for i in range(4):
                x = -225 + i * 150
                P.figure(sc, x, -196, yaw=180 + r.uniform(-8, 8), arm_l='out', arm_r='out', rock=3, rate=0.4, phase=r.random(), detail=1, **officer(r))
    return build


def commuters(seed=3, n=7, jog=False):
    """LOW, 30 m of pavement under the awnings' edge: people going home, both ways, each at their own pace, and they
    come round again. A bag, a cup, a pack, a dog on its way; jog: a runner and a child towed along."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 1):
            g.box(GREY, (sx * 1500, 0, 1), (6, 6, 2))               # the mesh is as long as the walk
        for i in range(n):
            way = 1 if i % 2 else -1
            x = -1350 + i * 2700.0 / (n - 1) + r.uniform(-90, 90)
            y = (22 if way > 0 else -22) + r.uniform(-4, 4)
            v = r.uniform(125, 185)
            d = person(r, wear=r.choice(('coat', 'coat', 'trousers', 'skirt', 'dress')), sleeves='long')
            hold = r.choice((None, P.hold_bag(r.choice(HOT)), P.hold_bag(WOOD), P.hold_cup()))
            if jog and i == 2:
                v, hold = 330.0, None
                d = person(r, wear='shorts', sleeves='short', band=WHITE)
            P.figure(sc, x, y, yaw=90 * way, stride=30 if v > 300 else 24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=v / 150.0, bob=3,
                     phase=r.random(), travel=(3000, v * way), hold_r=hold, pack=r.choice((None, None, r.choice(HOT))), detail=1, **d)
            if jog and i == 5:                                       # a child towed along beside
                P.figure(sc, x + 52 * way, y, yaw=90 * way, stride=30, swing_l=22, swing_r=-22, axis_l='x', axis_r='x', rate=v / 100.0, bob=4,
                         phase=r.random(), travel=(3000, v * way), detail=1, **person(r, build='kid'))
        P.quadruped(sc, r.uniform(-900, 900), -24, yaw=-90, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=22,
                    rate=2.2, travel=(3000, -170), body=r.choice(('#e8c26a', '#fff6ea', '#8e5a3a')))
    return build


def vendor(seed=5):
    """LOW, a pocket piece: a pretzel cart under its low striped roof, steam off the pot, the seller, a queue with cups,
    two who have theirs and watch the chase."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        cx = -170.0
        g.box(WHITE, (cx, 0, 62), (180, 84, 76))
        for k in range(6):
            g.box(RED if k % 2 else WHITE, (cx - 75 + k * 30, -43, 62), (30, 3, 70))
        for sx in (-1, 1):
            g.cyl(DARK, (cx + sx * 60, -46, 24), 24, 8, rot=(0, 0, 90), steps=8)
            for sy in (-1, 1):
                g.box(STEEL, (cx + sx * 86, sy * 38, 143), (5, 5, 90))
        for k in range(6):
            g.box(PINK if k % 2 else WHITE, (cx - 80 + k * 32, 0, 191), (32, 100, 7))
        g.cyl(STEEL, (cx - 40, 0, 100), 24, 26, steps=8)
        g.box(YEL, (cx + 40, 0, 108), (60, 50, 16))
        puff = sc.part(P.motion(bob=-9.0, rate=0.45))
        puff.sphere(WHITE, (cx - 40, 0, 146), 15, steps=(3, 5))
        puff.sphere(WHITE, (cx - 30, 6, 166), 10, steps=(3, 5))
        P.figure(sc, cx + 6, 66, arm_r='fwd', swing_r=10, axis_r='x', arm_l='hold', rate=0.7, detail=1,
                 **person(r, s=0.9, hat='chef', hat_col=WHITE, top=WHITE, band=RED))
        for k in range(4):
            x = cx + 150 + k * 78
            d = person(r, build='kid') if k == 2 else person(r)
            P.figure(sc, x, -16 + (k % 2) * 22, yaw=-90 + r.uniform(-25, 25), arm_r='hold', hold_r=P.hold_cup() if k != 2 else None,
                     arm_l='down' if k % 2 else 'hip', rock=3 if k % 2 else 0, rate=0.35, bob=0 if k % 2 else -1.2, phase=r.random(), detail=1 if k < 2 else 0, **d)
        for k in range(2):
            d = person(r)
            d.update(CHEER[(2, 3)[k]])
            P.figure(sc, 150 + k * 90, -110, yaw=r.uniform(-20, 20), hold_l=P.hold_cup(), phase=r.random(), react=True, detail=1, **d)
    return build


def queue(seed=6):
    """LOW, 12 m by 1 m, for the strip before a picture house: the queue for the late show behind a gold-posted red
    rope on a red carpet. Evening coats and dresses, an usher in red waving them on; two of them wave at the chase."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(RED, (0, 0, 1.5), (1200, 84, 3))
        for k in range(7):
            x = -600 + k * 200
            g.cyl(GOLD, (x, -44, 0), 5, 92, steps=6)
            g.cyl(GOLD, (x, -44, 0), 13, 5, steps=6)
            if k < 6:
                tilt_box(g, RED, (x, -44, 86), (x + 100, -44, 72), 5, 5)
                tilt_box(g, RED, (x + 100, -44, 72), (x + 200, -44, 86), 5, 5)
        acts = (dict(arm_l='hip', rock=3, rate=0.3), dict(arm_r='face', hold_r=P.hold_phone(), rate=0.25, bob=-1.0),
                dict(arm_r='wave', swing_r=26, rate=1.3, bob=3, react=True), dict(arm_l='hold', arm_r='hold', hold_r=P.hold_cup(), turn=14, rate=0.3),
                dict(turn=22, rate=0.28), dict(arm_r='point', swing_r=10, axis_r='x', rate=0.7, react=True))
        for i in range(9):
            x = -520 + i * 118 + r.uniform(-10, 10)
            d = person(r, wear=r.choice(('dress', 'coat', 'skirt', 'coat')), sleeves='long')
            d.update(acts[(i * 5 + seed) % len(acts)])
            d.setdefault('phase', r.random())
            P.figure(sc, x, 6 + r.uniform(-6, 6), yaw=r.choice((-60, -35, 0, 0, 30)), detail=1 if i % 2 == 0 else 0, **d)
        P.figure(sc, 566, -8, yaw=40, arm_r='wave', swing_r=20, arm_l='back', rate=0.9, detail=1,
                 **person(r, top=RED, bottom=NAVY, sleeves='long', wear='trousers', hat='peak', hat_col=RED, band=GOLD, s=0.96))
    return build


def syndicate(seed=7):
    """LOW, a pocket piece, Syndicate Territory only: the Chairman's men at their post in black and gold. Two ranks,
    one scanning the street through glasses, one pointing the car out, their dog. (Gold caps and bands: they have to
    read against black walls by the street's own light. No chevron on them: that mark stays on the dressing.)"""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(BLACK, (0, 40, 12), (560, 90, 24))
        g.box(GOLD, (0, -6, 12), (560, 4, 8))
        suit = dict(tops=(BLACK,), bottoms=(BLACK,), sleeves='long', wear='trousers', hat='peak', hat_col=GOLD, band=GOLD, shoes=DARK)
        acts = (dict(arm_l='back', arm_r='back', rock=2, rate=0.25), dict(arm_l='hip', arm_r='face', hold_r=P.hold_camera(col=DARK, lens=GOLD), turn=32, rate=0.22),
                dict(arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.8, react=True), dict(arm_l='hip', arm_r='hip', turn=12, rate=0.3),
                dict(arm_l='back', arm_r='face', hold_r=P.hold_phone(col=DARK, screen=GOLD), rock=3, rate=0.3), dict(arm_l='back', arm_r='back', turn=18, rate=0.2))
        for i in range(6):
            back = i % 2 == 1
            d = P.look(r, **suit)
            d['s'] = r.uniform(0.94, 1.0)
            d['hair'] = r.choice(('short', 'bald', 'short', 'pony'))
            d.update(acts[i])
            P.figure(sc, -240 + i * 96, 40 if back else -70, z=24 if back else 0, yaw=r.uniform(-10, 10), phase=r.random(), detail=1, **d)
        P.quadruped(sc, 250, -90, yaw=20, length=84, height=58, girth=24, neck=20, neck_up=40, head=22, tail=24, wag=14, graze=6, rate=0.8,
                    body=DARK, snout='#8e5a3a', legs_col='#8e5a3a', ears=DARK)
    return build


def pigeons(seed=8):
    """LOW, a pocket piece: pigeons on the pavement, an old hand on a crate throwing them crumbs, and four of them up
    and round at knee height (nothing here rises over 1.6 m)."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(WOOD, (-150, 60, 22), (60, 46, 44))
        P.figure(sc, -150, 60, z=0, yaw=20, legs='sit', seat=44, arm_r='fwd', swing_r=22, axis_r='x', arm_l='hold', hold_l=P.hold_bag(CREAM), rate=0.6, detail=1,
                 **person(r, wear='coat', sleeves='long', hat='beanie', hat_col=RED, hair_col='#cfcfd8', s=0.95))
        cols = (('#c9c4d6', '#8f9bb3'), ('#fff6ea', '#c9c4d6'), ('#8f9bb3', '#5a5870'), ('#c9c4d6', '#7a8fb8'))
        for i in range(8):
            a = r.uniform(0, 6.283)
            d = r.uniform(40, 230)
            b, w = cols[i % 4]
            P.bird(sc, -30 + math.cos(a) * d, -40 + math.sin(a) * d * 0.6, 14, heading=r.uniform(0, 360), span=62, body=b, wing=w, beak=ORANGE,
                   phase=r.random(), rate=r.uniform(1.4, 2.4), flap=3.0, bob=5.0)
        for i in range(4):
            a = i * 90.0 + r.uniform(-20, 20)
            R = r.uniform(140, 200)
            b, w = cols[i % 4]
            P.bird(sc, 40 + math.cos(math.radians(a)) * R, -20 + math.sin(math.radians(a)) * R, 70 + i * 22, heading=a + 90.0, span=78, body=b, wing=w,
                   beak=ORANGE, phase=r.random(), rate=3.0, flap=38.0, orbit=(40, -20, 0.22))
    return build


# ------------------------------------------------------------------------------------------ on the roofs (tall, lit)
def rail(m, x0, x1, y, h=104.0, col=STEEL):
    """A roof's rail: posts and two bars."""
    n = max(int((x1 - x0) / 250.0), 2)
    for k in range(n + 1):
        m.box(col, (x0 + k * (x1 - x0) / n, y, h / 2.0), (6, 6, h))
    m.box(col, ((x0 + x1) / 2.0, y, h), (x1 - x0 + 6, 7, 7))
    m.box(col, ((x0 + x1) / 2.0, y, h * 0.55), (x1 - x0, 5, 5))


def roof_party(seed=4, L=1000.0):
    """TALL, stands on a podium's roof (the set's "up" is the roof), its people along the front edge: a party under
    strings of lights on a lit floor. Dancers, people at the rail waving at the chase, a DJ. y = 0 is the podium's
    middle: the roof's edge is at y = -350."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h = L / 2.0
        rail(g, -h, h, -340)
        nt = int(L / 200.0)
        for i in range(nt):                                                  # the lit floor: squares of two colours
            g.box((PINK, CYAN, YEL, VIOLET)[(i + seed) % 4], (-h + 100 + i * 200, -230, 2.5), (190, 190, 5), glow=G_FLOOR)
        for sx in (-1, 1):
            g.box(STEEL, (sx * h, -330, 170), (7, 7, 340))
        g.box(STEEL, (0, -60, 190), (7, 7, 380))
        bulbs(g, (-h, -330, 336), (h, -330, 336), 9, sag=50)
        bulbs(g, (-h, -330, 336), (0, -60, 376), 5, sag=30)
        bulbs(g, (h, -330, 336), (0, -60, 376), 5, sag=30)
        # the DJ's desk and its two stacks
        dx = h - 130
        g.box(DARK, (dx, -120, 48), (190, 60, 96))
        cloth(g, dx - 86, dx + 86, -152, 14, 86, base=DARK, bars=(PINK, CYAN, YEL), kind=1, glow=G_PANEL)
        for sx in (-1, 1):
            g.box(DARK, (dx + sx * 132, -110, 80), (64, 60, 160))
            g.cyl(STEEL, (dx + sx * 132, -141, 112), 22, 4, rot=(0, 0, 90), steps=8)
        P.figure(sc, dx, -76, arm_l='fwd', arm_r='up', swing_r=16, rate=1.5, bob=5, phase=r.random(), detail=0, **person(r, hat='headset', hat_col=PINK))
        n = int((L - 330) / 78.0)
        for i in range(n):
            x = -h + 50 + i * 78 + r.uniform(-8, 8)
            atrail = i % 3 == 0
            d = person(r)
            d.update(CHEER[(i + seed) % len(CHEER)] if atrail else DANCE[(i * 5 + seed) % len(DANCE)])
            P.figure(sc, x, (-306 if atrail else -235 + (i % 2) * 60) + r.uniform(-6, 6), yaw=r.uniform(-25, 25), phase=r.random(), react=atrail,
                     detail=0, **d)
        lit(sc)
    return build


def roof_rally(seed=9, L=900.0, bars=(PINK, YEL, CYAN), kind=3):
    """TALL, stands on a podium's roof like the party: the neighbours have come up with a banner on two poles (bars and
    squares: never words), flags, and a string of lights along the rail. They cheer the chase on."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h = L / 2.0
        rail(g, -h, h, -340)
        for sx in (-1, 1):
            g.box(STEEL, (sx * h, -330, 150), (7, 7, 300))
            g.box(WOOD, (sx * 300, -190, 170), (8, 8, 340))
        bulbs(g, (-h, -330, 296), (h, -330, 296), 9, sag=44)
        ban = sc.part(P.motion(swing=4.0, pivot=(0.0, -190.0, 336.0), axis=0.0, rate=0.4, phase=r.random()))
        cloth(ban, -300, 300, -190, 216, 336, bars=bars, kind=kind, glow=0.12)
        n = 10
        for i in range(n):
            x = -h + 45 + i * (L - 90) / (n - 1) + r.uniform(-8, 8)
            d = person(r, build='kid') if i == 6 else person(r)
            if i in (1, 8):
                d.update(arm_r='wave', swing_r=24, hold_r=P.hold_flag(col=(PINK, CYAN)[i % 2], pole=WHITE, w=70.0), rate=0.9, bob=4)
            else:
                d.update(CHEER[(i * 3 + seed) % len(CHEER)])
            P.figure(sc, x, (-304 if i % 2 == 0 else -246) + r.uniform(-6, 6), yaw=r.uniform(-20, 20), phase=r.random(), react=True, detail=0, **d)
        lit(sc)
    return build


def window_cleaners(seed=11):
    """TALL, stands at a glass infill (SM_Infill2: the same origin; its face is at y = -130): a cradle on two cables
    from davits on the roof, 34 m up, going slowly up and down the glass 6 to 12 m over the street, two cleaners in
    yellow at work in it, a work lamp."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        F = -132.0
        for sx in (-1, 1):
            g.box(STEEL, (sx * 190, F - 62, 2000), (6, 6, 2860))            # the cables: the cradle slides on them
            tilt_box(g, STEEL, (sx * 190, 40, 3420), (sx * 190, F - 76, 3470), 12, 12)
            g.box(YEL, (sx * 190, 60, 3440), (40, 60, 40))
        mo = dict(bob=-290.0, rate=0.085, phase=0.0)
        z0 = 900.0
        c = sc.part(P.motion(**mo))
        c.box(STEEL, (0, F - 62, z0), (460, 96, 10))
        for sy in (-1, 1):
            c.box(YEL, (0, F - 62 + sy * 46, z0 + 96), (460, 6, 8))
            c.box(YEL, (0, F - 62 + sy * 46, z0 + 50), (460, 5, 6))
        for sx in (-1, 0, 1):
            for sy in (-1, 1):
                c.box(YEL, (sx * 227, F - 62 + sy * 46, z0 + 50), (6, 6, 100))
        for k in range(5):
            c.box(DARK if k % 2 else YEL, (-184 + k * 92, F - 111, z0 + 20), (92, 4, 30))
        c.cyl(CYAN, (60, F - 62, z0 + 5), 16, 30, steps=7)
        c.box(WARM_L, (-205, F - 100, z0 + 118), (26, 16, 22), glow=1.0)
        for k, x in enumerate((-110.0, 130.0)):
            d = person(r, top=YEL, bottom=NAVY, sleeves='long', wear='trousers', hat='hard', hat_col=WHITE, band=WHITE, s=0.97)
            d.update(mo)
            P.figure(sc, x, F - 56, z=z0 + 5, yaw=180, arm_r='up', swing_r=30, axis_r='y', hold_r=P.hold_tool(col=STEEL, grip=CYAN, length=30.0),
                     arm_l='hip' if k else 'out', detail=1, **d)
        lit(sc, 0.07)
    return build


def rally(seed=12):
    """TALL, 8 m by 3 m, for the plaza before a monument (24 to 27.5 m out): a rally. A speaker on a crate with a
    megaphone, placards and flags (stripes only), a banner on two poles behind, everyone hopping out of step."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 1):
            g.box(WOOD, (sx * 290, 130, 180), (8, 8, 360))
        ban = sc.part(P.motion(swing=4.0, pivot=(0.0, 130.0, 356.0), axis=0.0, rate=0.4))
        cloth(ban, -290, 290, 130, 236, 356, bars=(CYAN, PINK, YEL), kind=2, glow=0.12)
        g.box(WOOD, (0, -120, 28), (80, 60, 56))
        P.figure(sc, 0, -120, z=56, arm_r='face', hold_r=C.hold_megaphone(), arm_l='up', swing_l=16, rate=1.0, bob=3, detail=1,
                 **person(r, top=RED, s=1.0))
        k = 0
        for row, y in enumerate((-40, 50)):
            for i in range(6):
                x = -330 + i * 132 + (row * 60) + r.uniform(-10, 10)
                if abs(x) < 70 and row == 0:
                    continue
                d = person(r)
                if k % 3 == 0:
                    col, st = r.choice(((CREAM, PINK), (YEL, VIOLET), (CYAN, WHITE), (WHITE, RED)))
                    d.update(arm_r='wave', swing_r=10, hold_r=P.hold_sign(col=col, stripe=st, w=76.0, h2=52.0), bob=8, rate=1.3)
                elif k % 5 == 1:
                    d.update(arm_r='wave', swing_r=24, hold_r=P.hold_flag(col=r.choice((PINK, CYAN, YEL)), pole=WHITE, w=72.0), rate=0.9, bob=4)
                else:
                    d.update(CHEER[(k + seed) % len(CHEER)])
                P.figure(sc, x, y + r.uniform(-8, 8), yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=0, **d)
                k += 1
        lit(sc)
    return build


def flock(seed=13, n=14):
    """TALL and always moving (far=True): pigeons wheeling, built BIG (2.4 to 3.2 m across at scale 1: the table stands
    it small over a gap between two podiums and at twice the size over the far roofs). Pale, with a faint light of
    their own: the city's glow on them against the night."""
    def build(sc):
        r = random.Random(seed)
        cols = (('#f4f1ea', '#c9c4d6'), ('#c9c4d6', '#8f9bb3'), ('#fff6ea', '#a39cc2'))
        for i in range(n):
            ring = i % 3
            R = (520.0, 820.0, 1150.0)[ring] + r.uniform(-60, 60)
            a = i * (360.0 / n) * 2.3 + r.uniform(-14, 14)
            way = 1 if ring != 1 else -1
            b, w = cols[i % 3]
            P.bird(sc, math.cos(math.radians(a)) * R, math.sin(math.radians(a)) * R, 120 + ring * 170 + r.uniform(-60, 60), heading=a + 90.0 * way,
                   span=r.uniform(240, 320), body=b, wing=w, beak=ORANGE, phase=r.random(), rate=r.uniform(1.7, 2.3), flap=34.0,
                   orbit=(0, 0, way * (0.075, 0.055, 0.04)[ring]))
        lit(sc, 0.14)
    return build


JOBS = [
    ('SM_Onlookers_A', onlookers(1)),
    ('SM_Onlookers_B', onlookers(21, kids=(2, 6))),
    ('SM_March_A', march(2, 0)),
    ('SM_March_B', march(22, 2, bars=(YEL, VIOLET, MINT), police=True)),
    ('SM_Commuters_A', commuters(3)),
    ('SM_Commuters_B', commuters(23, jog=True)),
    ('SM_Vendor_A', vendor(5)),
    ('SM_Queue_A', queue(6)),
    ('SM_Syndicate_A', syndicate(7)),
    ('SM_Pigeons_A', pigeons(8)),
    ('SM_RoofParty_A', roof_party(4)),
    ('SM_RoofParty_B', roof_party(24, L=800.0)),
    ('SM_RoofRally_A', roof_rally(9)),
    ('SM_WindowCleaners_A', window_cleaners(11)),
    ('SM_Rally_A', rally(12)),
    ('SM_PigeonFlock', flock(13), dict(far=True)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
