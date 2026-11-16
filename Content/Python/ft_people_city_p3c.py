"""FULL THROTTLE (Unreal) the population, level 7: City Center, pass 3, builder p3c: the MOVERS. /Game/People/CityCenter/SM_P3C_*.

Everything here TRAVELS: along the avenue's pavement (the piece's X, laid along the road), away from the avenue down a
side street (a Set turns the piece 90 degrees: its +X then points away from the road), or along a bank's roof. Nothing
crosses the road. Placed by the block C:/ftwork/p3/L7c.inl (pasted into Source/FullThrottle/FTPeopleL7.inl).

WHERE THEY GO (read from FTDressL7.inl and ft_dress_metro.py; not looked at in the game):
  THE DOOR LANE  24.42 m out, 30 m long, centred on a podium (the slot's anchor). The commuters of the first cast walk
                 23.5 to 24.1 m; the podiums' fronts are 25.5 m out, their awnings (3 m up) reach 24.44 m, a bank's
                 steps and the dear doors' tree tubs begin at 24.75 m. So 24.06 to 24.78 m is a second lane, under the
                 awnings' edge, before the lit windows. Pieces for it are at most 0.72 m deep. (Not before a garage:
                 its ramp comes down to 24.5 m.) LOW pieces (under 2 m as built) and two TALL ones (riders, a march
                 with placards: 24.42 m is past 24 m; they carry no light).
  SIDE STREETS   SM_FloorCross: 11 m of tarmac (its top 28 cm up) between kerbs 5.5 to 6.3 m from its middle, cars
                 parked 3.25 to 5.15 m from the middle, the dressing's traffic 0.55 to 2.45 m. SM_P3C_SideStreet_*
                 are 31 m long and stand 37 to 68 m out (the dressing's own traffic starts 37 m out, so that its box
                 stays past 24 m on the tightest bend): walkers on the kerbs both ways, cyclists riding AWAY between
                 the traffic and the parked cars.
  BANK ROOFS     SM_BankFront's roof, flat, 10.36 m up, 32 m long, no first-cast people on it: SM_P3C_RoofRun_* run
                 its front edge, 0.8 m back from the cornice (26.8 m out), leaping. A faint steady light on them.

Banners, placards and flags carry stripes and squares: never words, never a chevron (the Syndicate's mark).

Run in the editor:  import ft_people_city_p3c; ft_people_city_p3c.run()        (or run(['SM_P3C_Runners_A']))
"""
import random
import unreal
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/CityCenter'
NAVY, DARK, GREY, STEEL, WHITE, CREAM = '#2b2350', '#22202a', '#3f3d50', '#8f9bb3', '#fff6ea', '#fff1d6'
PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, BLUE = '#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#ff2d3c', '#ff7a3c', '#8f6bff', '#3a5fa8'
GOLD, WOOD, BLACK, BRASS = '#ffc93c', '#9a6a44', '#1c1a26', '#e8c26a'
POLICE = '#3a5fa8'
HOT = (PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, WHITE, '#f4a6ff', '#b8ff5a', '#57c7ff')     # coats that read at night
LANE = 3000.0        # a door-lane piece: 30 m of pavement
SIDE = 3100.0        # a side-street piece: 31 m, away from the avenue
ROOF = 2800.0        # a bank's roof: 28 m of its 32


def lit(sc, g=0.1):
    """Night: everything of a piece that carries no light of its own gets a faint steady one. ONLY for pieces the table
    stands 24 m or more from the road (the light rule: no glow nearer)."""
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


def ends(sc, L, z=1.0):
    """The mesh is as long as the line its people travel."""
    g = sc.still()
    for sx in (-1, 1):
        g.box(GREY, (sx * L / 2.0, 0, z), (6, 6, 2))
    return g


def person(r, **fixed):
    """A citizen in a coat that reads at night, never over 1.72 m (the low pieces stay under 2 m)."""
    d = P.look(r, tops=HOT, **fixed)
    d['s'] = fixed.get('s', r.uniform(0.9, 0.99))
    if d.get('build') == 'kid':
        d['s'] = fixed.get('s', r.uniform(0.6, 0.7))
    if d.get('hair') in ('afro', 'bun') and d.get('hat'):
        d['hair'] = 'short'
    return d


def athlete(r, **fixed):
    kw = dict(wear='shorts', sleeves=r.choice(('short', 'short', 'none')), shoes=r.choice((WHITE, RED, YEL)), build=r.choice(('slim', 'avg', 'slim')),
              band=r.choice((None, WHITE, DARK)))
    kw.update(fixed)
    return person(r, **kw)


def officer(r, **fixed):
    """The city's police (as the first cast draws them): a blue coat, a white band, a peaked cap."""
    d = P.look(r, tops=(POLICE,), bottoms=(NAVY,), sleeves='long', wear='trousers', hat='peak', hat_col=POLICE, band=WHITE, shoes=DARK)
    d['s'] = r.uniform(0.93, 0.98)
    d['hair'] = r.choice(('short', 'pony', 'bald'))
    d.update(fixed)
    return d


def syn_man(r, **fixed):
    """One of the Chairman's men, as the first cast draws them: a gold jacket, black cap, trousers and band. No chevron."""
    d = P.look(r, tops=(GOLD,), bottoms=(BLACK,), sleeves='long', wear='trousers', hat='peak', hat_col=BLACK, band=BLACK, shoes=DARK)
    d['s'] = r.uniform(0.94, 0.99)
    d['hair'] = r.choice(('short', 'bald', 'short', 'pony'))
    d.update(fixed)
    return d


def go(sc, r, x, y, way, v, L, d, z=0.0, stride=24.0, swing=18.0, bob=3.0, rate=None, detail=1, phase=None, **kw):
    """One figure travelling the piece's X at v cm/s (way +1: towards +X, -1: towards -X), striding, and round again."""
    a = dict(d)
    a.update(kw)
    a.setdefault('swing_l', swing)
    a.setdefault('swing_r', -swing)
    a.setdefault('axis_l', 'x')
    a.setdefault('axis_r', 'x')
    return P.figure(sc, x, y, z=z, yaw=90.0 * way, stride=stride, rate=rate if rate is not None else v / 150.0, bob=bob,
                    phase=r.random() if phase is None else phase, travel=(L, v * way), detail=detail, **a)


def tow(sc, x, y, way, v, L, pz=50.0, **mo):
    """The model for a thing that travels WITH the figure standing at (x, y): the same line, the same moment of coming
    round. Draw into it in the mesh's own space."""
    mo.setdefault('pivot', (x, y, pz))
    return sc.part(P.motion(base=(x, y), travel=(L, v * way / L), **mo))


def dog(sc, r, x, y, way, v, L, size=1.0, body=None, z=0.0):
    P.quadruped(sc, x, y, z=z, yaw=90.0 * way, s=size, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=24,
                rate=min(v / 75.0, 3.0), phase=r.random(), travel=(L, v * way), body=body or r.choice((BRASS, WHITE, '#8e5a3a', '#cfcfd8', DARK)))


def lead(sc, x, y, way, v, L, a, b, col=RED):
    m = tow(sc, x, y, way, v, L)
    tilt_box(m, col, a, b, 4, 4)


# ------------------------------------------------------------------------------------------ what a hand holds
def hold_paddle(col=YEL, rim=RED):
    """A crossing paddle: a striped square on a short stick (under 1.9 m in a hand at the chest)."""
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 30), (4, 4, 66))
        m.box(rim, (h[0], h[1], h[2] + 72), (34, 4, 34))
        m.box(col, (h[0], h[1] - 2.6, h[2] + 72), (24, 1.5, 24))
    return f


def hold_drum(col=RED, skin=WHITE):
    def f(m, h, side):
        if side > 0:
            m.cyl(col, (h[0] - 24, h[1] - 12, h[2] - 30), 23, 26, steps=8)
            m.cyl(skin, (h[0] - 24, h[1] - 12, h[2] - 4), 24, 4, steps=8)
        m.box(WOOD, (h[0], h[1] - 6, h[2] + 6), (4, 22, 4))
    return f


def hold_torch():
    """A watchman's torch, dark (no light: it is carried 24 m from the road and nearer)."""
    def f(m, h, side):
        m.box(DARK, (h[0], h[1] - 10, h[2] + 4), (7, 26, 7))
        m.box(STEEL, (h[0], h[1] - 24, h[2] + 4), (10, 5, 10))
    return f


# ------------------------------------------------------------------------------------------ things on wheels
def wheel(m, x, y, z, R=30.0, w=5.0, col=DARK):
    """A wheel that rolls along X (its axle along Y), its axle at (x, y, z)."""
    m.cyl(col, (x, y - w / 2.0, z), R, w, rot=(0, 0, 90), steps=8)


def cyclist(sc, r, x, y, way, v, L, z=0.0, frame=None, d=None, detail=1):
    """A rider on a bicycle, leaning from side to side as they pedal. Top 1.9 m."""
    sway = dict(swing=3.5, pivot=(x, y, z), axis=180.0)
    ph, rate = r.random(), v / 260.0
    m = tow(sc, x, y, way, v, L, phase=ph, rate=rate, **sway)
    frame = frame or r.choice(HOT)
    for sx in (-1, 1):
        wheel(m, x + sx * 52, y, z + 31, R=31, w=5)
    tilt_box(m, frame, (x - way * 52, y, z + 31), (x - way * 10, y, z + 84), 5, 5)          # seat stay
    tilt_box(m, frame, (x - way * 10, y, z + 84), (x + way * 40, y, z + 78), 5, 5)          # top tube
    tilt_box(m, frame, (x - way * 10, y, z + 84), (x + way * 2, y, z + 34), 5, 5)           # seat tube
    tilt_box(m, frame, (x + way * 2, y, z + 34), (x + way * 40, y, z + 78), 5, 5)           # down tube
    tilt_box(m, STEEL, (x + way * 52, y, z + 31), (x + way * 38, y, z + 100), 5, 5)         # fork and stem
    m.box(DARK, (x + way * 38, y, z + 102), (5, 46, 5))                                      # the bars
    m.box(DARK, (x - way * 12, y, z + 90), (26, 12, 6))                                      # the saddle
    d = d or person(r, wear=r.choice(('shorts', 'trousers')), hat='helmet', hat_col=r.choice(HOT), hair='short', s=0.92, pack=r.choice((None, r.choice(HOT))))
    P.figure(sc, x - way * 8, y, z=z + 34, yaw=90.0 * way, legs='sit', seat=58, arm_l='push', arm_r='push', phase=ph, rate=rate, sway=sway,
             travel=(L, v * way), detail=detail, **d)


def skater(sc, r, x, y, way, v, L, d=None):
    """A skateboarder: standing wide on the board, arms out, rocking; the board goes with them."""
    sway = dict(swing=6.0, pivot=(x, y, 0.0), axis=180.0)
    ph, rate = r.random(), r.uniform(0.5, 0.8)
    m = tow(sc, x, y, way, v, L, phase=ph, rate=rate, **sway)
    m.box(r.choice(HOT), (x, y, 10), (82, 22, 4))
    for sx in (-1, 1):
        m.box(DARK, (x + sx * 28, y, 4), (10, 20, 8))
    d = d or person(r, wear=r.choice(('shorts', 'trousers')), hat=r.choice(('cap', 'beanie', None)), hat_col=r.choice(HOT), s=r.uniform(0.86, 0.94))
    if d.get('hat') and d.get('hair') in ('afro', 'bun'):
        d['hair'] = 'short'
    P.figure(sc, x, y, z=12, yaw=90.0 * way, legs='wide', arm_l='out', arm_r=r.choice(('out', 'hip', 'wave')), phase=ph, rate=rate, sway=sway,
             travel=(L, v * way), detail=1, **d)


# ------------------------------------------------------------------------------------------ the door lane (LOW)
def runners(seed, way=1, pram=False):
    """LOW, the door lane: a running club in bright kit, a pack going one way down the pavement and round again.
    pram: one pushes a three-wheeled running pram, a dog runs with them."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, LANE)
        n = 5 if pram else 7
        v = 345.0
        x0 = -1250.0
        for i in range(n):
            x = x0 + i * 118 + r.uniform(-18, 18)
            y = (-12 if i % 2 else 12) + r.uniform(-3, 3)
            go(sc, r, x, y, way, v + r.uniform(-6, 6), LANE, athlete(r, hat=r.choice((None, None, 'cap')), hat_col=r.choice(HOT)), stride=38, swing=32, bob=5,
               rate=2.4)
        # two who cannot keep up, further back
        for k in range(2):
            x = 300 + k * 520 + r.uniform(-60, 60)
            go(sc, r, x, 10 - k * 20, way, 285.0, LANE, athlete(r, build='stocky'), stride=30, swing=26, bob=4, rate=2.0)
        if pram:
            x, y = -420.0, -8.0
            go(sc, r, x, y, way, 330.0, LANE, athlete(r), stride=36, bob=4, rate=2.3, arm_l='push', arm_r='push', swing_l=0, swing_r=0)
            m = tow(sc, x, y, way, 330.0, LANE)
            px = x + way * 92
            m.box(CYAN, (px, y, 62), (64, 40, 34))
            m.box(WHITE, (px + way * 10, y, 86), (40, 42, 16))
            tilt_box(m, STEEL, (px - way * 30, y, 66), (x + way * 34, y, 104), 4, 4)
            m.box(STEEL, (x + way * 34, y, 104), (4, 40, 4))
            wheel(m, px + way * 44, y, 22, R=22, w=5)
            for sy in (-1, 1):
                wheel(m, px - way * 26, y + sy * 21, 22, R=22, w=5)
            dog(sc, r, -900.0, 14.0, way, 345.0, LANE, size=1.1)
    return build


def skaters(seed):
    """LOW, the door lane: skateboarders gliding one way, roller skaters striding the other."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, LANE)
        for i in range(3):
            skater(sc, r, -1150 + i * 900 + r.uniform(-120, 120), -13, 1, r.uniform(400, 470), LANE)
        for i in range(3):
            d = person(r, wear=r.choice(('shorts', 'skirt', 'trousers')), sleeves='short', shoes=r.choice((PINK, CYAN, YEL)), hat=r.choice(('helmet', None)),
                       hat_col=r.choice(HOT), hair=r.choice(('short', 'long', 'pony')))
            go(sc, r, -800 + i * 850 + r.uniform(-120, 120), 13, -1, r.uniform(360, 420), LANE, d, stride=16, swing=30, bob=2, rate=0.9,
               arm_l='out', arm_r='out', axis_l='z', axis_r='z')
    return build


def porters(seed):
    """LOW, the door lane: the street's deliveries. A hand truck stacked with boxes, a rail of bright coats on castors, a
    bellhop with a brass luggage cart, a courier with a parcel. Nothing over 1.9 m."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, LANE)
        # the hand truck, going +X
        x, y, v = -900.0, -13.0, 108.0
        go(sc, r, x, y, 1, v, LANE, person(r, wear='trousers', hat='cap', hat_col=ORANGE, top=ORANGE, hair='short'), arm_l='push', arm_r='push',
           swing_l=0, swing_r=0)
        m = tow(sc, x, y, 1, v, LANE)
        tx = x + 62
        m.box(RED, (tx - 20, y, 70), (5, 40, 130))
        m.box(RED, (tx + 4, y, 8), (50, 40, 5))
        wheel(m, tx - 16, y - 20, 13, R=13, w=5)
        wheel(m, tx - 16, y + 20, 13, R=13, w=5)
        for k, c in enumerate((WOOD, CREAM, WOOD, YEL)):
            m.box(c, (tx + 6 - k * 2, y, 26 + k * 32), (44 - k * 3, 38 - k * 2, 30))
        # the rail of coats, going +X further on
        x, v = 450.0, 108.0
        go(sc, r, x, y, 1, v, LANE, person(r, wear='trousers', top=WHITE, hair='bun'), arm_l='push', arm_r='push', swing_l=0, swing_r=0)
        m = tow(sc, x, y, 1, v, LANE)
        rx = x + 120
        m.box(STEEL, (rx, y, 10), (150, 36, 5))
        for sx in (-1, 1):
            m.box(STEEL, (rx + sx * 72, y, 96), (5, 5, 172))
            wheel(m, rx + sx * 66, y, 7, R=7, w=30, col=DARK)
        m.box(STEEL, (rx, y, 180), (150, 5, 5))
        for k in range(6):
            m.box(HOT[(k * 3 + seed) % len(HOT)], (rx - 55 + k * 22, y, 124), (16, 40, 100))
        # the bellhop and the luggage cart, going -X
        x, y, v = 950.0, 13.0, 112.0
        go(sc, r, x, y, -1, v, LANE, person(r, wear='trousers', sleeves='long', top=RED, bottom=NAVY, hat='peak', hat_col=RED, band=GOLD, hair='short'),
           arm_l='push', arm_r='push', swing_l=0, swing_r=0)
        m = tow(sc, x, y, -1, v, LANE)
        cx = x - 118
        m.box(RED, (cx, y, 22), (130, 40, 8))
        for sx in (-1, 1):
            m.box(BRASS, (cx + sx * 60, y, 100), (5, 5, 160))
            wheel(m, cx + sx * 50, y, 9, R=9, w=34)
        m.box(BRASS, (cx, y, 182), (126, 5, 5))
        for k, (c, w, h) in enumerate(((NAVY, 60, 44), (WOOD, 52, 36), (PINK, 44, 30))):
            m.box(c, (cx - 26 + k * 6, y, 26 + sum((44, 36, 30)[:k]) + h / 2.0), (w, 36, h - 2))
        m.box(CYAN, (cx + 38, y, 62), (30, 34, 72))
        # the courier with a parcel, going -X, quick
        go(sc, r, -300.0, 13.0, -1, 185.0, LANE, person(r, wear='shorts', hat='cap', hat_col=CYAN, pack=CYAN), arm_l='carry', arm_r='carry',
           hold_r=P.hold_box(col=WOOD), swing_l=0, swing_r=0)
        go(sc, r, -1250.0, 13.0, -1, 150.0, LANE, person(r, wear='coat', sleeves='long'), hold_r=P.hold_bag(r.choice(HOT)))
    return build


def school(seed):
    """LOW, the door lane: a school walk. A teacher with a crossing paddle in front, the children two by two holding the
    rope, hopping out of step, another grown-up at the back."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, LANE)
        v, way = 104.0, 1
        x0 = 150.0
        go(sc, r, x0 + 150, 0, way, v, LANE, person(r, wear='coat', sleeves='long', top=MINT, hair='long'), arm_r='hold', hold_r=hold_paddle(), swing_r=0,
           arm_l='wave', swing_l=14, axis_l='y')
        for k in range(4):
            x = x0 - k * 82
            for sy in (-1, 1):
                d = person(r, build='kid', wear=r.choice(('trousers', 'skirt', 'shorts')), hat=r.choice((None, 'cap', 'beanie')), hat_col=r.choice(HOT),
                           pack=r.choice(HOT), hair=r.choice(('short', 'long', 'pony')))
                go(sc, r, x, sy * 14, way, v, LANE, d, stride=28, swing=20, bob=r.choice((4, 6, 7)), rate=r.uniform(1.4, 1.9), detail=1 if sy < 0 else 0,
                   **({'arm_l': 'wave', 'swing_l': 24, 'axis_l': 'y'} if (k + sy) % 3 == 0 else {}))
            m = tow(sc, x, 14, way, v, LANE)
            m.box(YEL, (x + 4, 0, 46), (86, 4, 4))                                   # their length of the rope
        go(sc, r, x0 - 4 * 82 - 30, 0, way, v, LANE, person(r, wear='trousers', top=PINK, hat='cap', hat_col=WHITE, hair='short', build='stocky'),
           hold_l=P.hold_bag(YEL))
        # coming the other way: an older child on the run, late
        go(sc, r, -900.0, 14.0, -1, 300.0, LANE, person(r, build='kid', s=0.74, pack=RED, hair='pony'), stride=36, swing=30, bob=5, rate=2.3)
    return build


def dog_walk(seed):
    """LOW, the door lane: a dog walker towed along by four dogs, and coming the other way someone with a very big dog
    and someone with a very small one."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, LANE)
        x, y, v = -500.0, 14.0, 150.0
        go(sc, r, x, y, 1, v, LANE, person(r, wear='trousers', hat='beanie', hat_col=PINK, hair='short', top=CYAN), arm_r='fwd', arm_l='fwd',
           swing_l=0, swing_r=0, stride=28)
        cols = (BRASS, WHITE, '#8e5a3a', DARK)
        for k in range(4):
            dx, dy, sz = 95 + k * 62, -22 + (k % 2) * 16, (0.8, 1.15, 0.7, 1.0)[k]
            dog(sc, r, x + dx, y + dy, 1, v, LANE, size=sz, body=cols[k])
            lead(sc, x, y, 1, v, LANE, (x + 34, y - 6, 112), (x + dx - 20, y + dy, 48 * sz), col=HOT[k])
        # the very big dog, going -X
        x, y, v = 800.0, 14.0, 135.0
        go(sc, r, x, y, -1, v, LANE, person(r, wear='coat', sleeves='long', build='slim'), arm_l='fwd', swing_l=0, stride=26)
        P.quadruped(sc, x - 130, -12, yaw=-90.0, length=118, height=88, girth=34, neck=26, neck_up=40, head=30, tail=34, wag=22, stride=22,
                    rate=v / 110.0, phase=r.random(), travel=(LANE, -v), body=CREAM, snout=DARK)
        lead(sc, x, y, -1, v, LANE, (x - 34, y - 8, 112), (x - 100, -12, 84), col=RED)
        # the very small one, going -X, trotting to keep up
        x, v = -1100.0, 120.0
        go(sc, r, x, 14.0, -1, v, LANE, person(r, wear='dress', hair='bun', build='stocky'), hold_r=P.hold_bag(PINK))
        dog(sc, r, x - 70, -8.0, -1, v, LANE, size=0.55, body=WHITE)
    return build


def patrol(seed, file_of=3):
    """LOW, the door lane, Syndicate Territory only: the Chairman's men walking their beat before his black walls, in
    gold and black (the first cast's own dress for them), hands behind their backs, a dog at heel."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, LANE)
        v = 96.0
        x0 = -700.0
        step = r.random()
        for i in range(file_of):                                                 # in file and in step
            go(sc, r, x0 + i * 105, -13, 1, v, LANE, syn_man(r), stride=26, swing=22 if file_of > 3 else 0, phase=step, rate=0.9,
               **({} if file_of > 3 else dict(arm_l='back', arm_r='back')))
        dog(sc, r, x0 + file_of * 105 + 30, -10, 1, v, LANE, size=1.2, body=DARK)
        lead(sc, x0 + (file_of - 1) * 105, -13, 1, v, LANE, (x0 + (file_of - 1) * 105 + 20, -13, 96), (x0 + file_of * 105 + 12, -10, 58), col=GOLD)
        # the other way: a pair, one with a dark torch, one with glasses to the eyes
        x = 900.0
        go(sc, r, x, 13, -1, 88.0, LANE, syn_man(r), arm_r='fwd', hold_r=hold_torch(), swing_r=0, arm_l='back', swing_l=0)
        go(sc, r, x + 100, 13, -1, 88.0, LANE, syn_man(r), arm_r='face', hold_r=P.hold_camera(col=DARK, lens=GOLD), swing_r=0, arm_l='hip', swing_l=0)
        if file_of <= 3:
            go(sc, r, -100.0, 13, -1, 92.0, LANE, syn_man(r), arm_l='back', arm_r='back', swing_l=0, swing_r=0)
    return build


# ------------------------------------------------------------------------------------------ the door lane (TALL: 24.42 m is past 24 m; no light)
def mounted(seed):
    """TALL (2.3 m), the door lane: the city's mounted police riding the pavement in file, an officer on foot ahead."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, LANE)
        v, way = 150.0, 1
        coats = ('#8e5a3a', '#cfcfd8', '#3b2a22')
        for i in range(3):
            x = -1100.0 + i * 800 + r.uniform(-60, 60)
            ph = r.random()
            P.quadruped(sc, x, 0, yaw=90.0 * way, body=coats[i], mane=(DARK, WHITE, DARK)[i], legs_col=coats[i], stride=20, rate=1.0, phase=ph, bob=4,
                        travel=(LANE, v * way), wag=10)
            m = tow(sc, x, 0, way, v, LANE, phase=ph, rate=1.0, bob=4)
            m.box(POLICE, (x, 0, 122), (70, 46, 6))                               # the saddle cloth, a white edge
            m.box(WHITE, (x, 0, 119), (74, 48, 3))
            P.figure(sc, x, 0, z=58, yaw=90.0 * way, legs='wide', arm_l='hold', arm_r='hold', phase=ph, rate=1.0, bob=4, travel=(LANE, v * way), detail=1,
                     **officer(r, hat='helmet', hat_col=POLICE, s=0.94))
        go(sc, r, 1250.0, 0, way, v, LANE, officer(r), arm_r='point', swing_r=10, arm_l='hip', swing_l=0)
    return build


def march_move(seed, bars=(PINK, CYAN, YEL)):
    """TALL (2.2 m: the placards), the door lane: a march on the move down the pavement. Placards and flags in stripes and
    squares (never words), a drummer, fists going up."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, LANE)
        v, way = 98.0, 1
        acts = (
            dict(arm_r='hold', hold_r=P.hold_sign(col=CREAM, stripe=bars[0], w=62.0, h2=42.0), swing_r=0, arm_l='up', swing_l=16, axis_l='y'),
            dict(arm_l='hold', hold_l=P.hold_flag(col=bars[1], w=54.0), swing_l=0, arm_r='up', swing_r=-16, axis_r='y'),
            dict(arm_l='hold', arm_r='hold', hold_r=hold_drum(), swing_l=14, swing_r=-14, axis_l='x', axis_r='x'),
            dict(arm_r='hold', hold_r=P.hold_sign(col=bars[2], stripe=NAVY, w=58.0, h2=46.0), swing_r=0),
            dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, axis_l='y', axis_r='y'),
            dict(arm_r='hold', hold_r=P.hold_flag(col=bars[0], w=54.0), swing_r=0, arm_l='wave', swing_l=22, axis_l='y'),
            dict(arm_l='hold', hold_l=P.hold_sign(col=WHITE, stripe=bars[1], w=62.0, h2=42.0), swing_l=0, arm_r='up', swing_r=-16, axis_r='y'),
        )
        for i in range(7):
            x = 420.0 - i * 108 + r.uniform(-10, 10)
            d = person(r, s=r.uniform(0.9, 0.96))
            d.update(acts[i])
            go(sc, r, x, -13 if i % 2 else 13, way, v, LANE, d, stride=22, bob=4, rate=r.uniform(0.9, 1.3))
    return build


# ------------------------------------------------------------------------------------------ the side streets (37 m out and more; faintly lit)
def side_street(seed, cab=False):
    """Stands in a side street, turned 90 degrees by its Set: +X points AWAY from the avenue, the street's tarmac is 28 cm
    up. Walkers on the two kerbs (5.9 m either side of the middle), both ways; cyclists riding away between the traffic
    and the parked cars (2.85 m either side). cab: a pedal cab with two passengers under its striped roof instead of
    two of the cyclists, and runners on one kerb."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, SIDE, z=29.0)
        Z = 28.0
        for i in range(2 if cab else 3):                                         # the left kerb, going away
            if cab:
                go(sc, r, -900 + i * 130, -590, 1, 330.0, SIDE, athlete(r), z=Z, stride=36, swing=30, bob=5, rate=2.3, detail=0)
            else:
                go(sc, r, -1300 + i * 950 + r.uniform(-100, 100), -590, 1, r.uniform(120, 170), SIDE, person(r, wear=r.choice(('coat', 'trousers', 'dress')),
                   sleeves='long'), z=Z, detail=0, hold_r=r.choice((None, P.hold_bag(r.choice(HOT)))))
        for i in range(2):                                                       # the right kerb, coming back
            go(sc, r, -700 + i * 1300 + r.uniform(-100, 100), 590, -1, r.uniform(120, 170), SIDE, person(r, wear=r.choice(('coat', 'trousers', 'skirt')),
               sleeves='long'), z=Z, detail=0, pack=r.choice((None, r.choice(HOT))))
        if cab:
            x, y, v = -300.0, 285.0, 300.0
            cyclist(sc, r, x, y, 1, v, SIDE, z=Z, frame=YEL)
            m = tow(sc, x, y, 1, v, SIDE)
            bx = x - 150
            m.box(YEL, (bx, y, Z + 46), (96, 76, 12))                              # the bench
            m.box(YEL, (bx - 44, y, Z + 78), (8, 76, 60))
            for sy in (-1, 1):
                wheel(m, bx - 6, y + sy * 40, Z + 31, R=31, w=5)
                for sx in (-1, 1):
                    m.box(STEEL, (bx + sx * 44, y + sy * 35, Z + 118), (4, 4, 140))
            for k in range(4):
                m.box(PINK if k % 2 else WHITE, (bx - 36 + k * 24, y, Z + 190), (24, 82, 6))
            tilt_box(m, STEEL, (bx + 48, y, Z + 46), (x - 60, y, Z + 40), 5, 5)
            for sy in (-1, 1):
                P.figure(sc, bx - 6, y + sy * 18, z=Z + 6, yaw=90.0, legs='sit', seat=46, arm_l='down', arm_r='wave' if sy > 0 else 'hold',
                         swing_r=24 if sy > 0 else 0, rate=1.1, phase=r.random(), travel=(SIDE, v), detail=0, **person(r, s=0.9))
            cyclist(sc, r, 900.0, -285.0, 1, 520.0, SIDE, z=Z)
        else:
            cyclist(sc, r, -1100.0, 285.0, 1, 500.0, SIDE, z=Z)
            cyclist(sc, r, -950.0, 285.0, 1, 500.0, SIDE, z=Z)
            cyclist(sc, r, 300.0, -285.0, 1, 560.0, SIDE, z=Z)
        lit(sc, 0.12)
    return build


# ------------------------------------------------------------------------------------------ the banks' roofs (26.8 m out, 10.4 m up; faintly lit)
def roof_run(seed, way=1, guard=False):
    """Stands on a bank's flat roof (its Set lifts it 10.4 m): free-runners in bright tracksuits going along the roof's
    front edge in great leaps, one after the other, and round again. guard: a watchman after them, waving."""
    def build(sc):
        r = random.Random(seed)
        ends(sc, ROOF)
        n = 4 if guard else 5
        for i in range(n):
            x = -way * 300 + way * i * 260 + r.uniform(-40, 40)
            c = HOT[(i * 2 + seed) % len(HOT)]
            d = person(r, wear='trousers', sleeves='long', top=c, bottom=c, build='slim', hat=r.choice(('beanie', 'cap', None)), hat_col=WHITE,
                       hair=r.choice(('short', 'pony')), shoes=WHITE, band=WHITE, s=r.uniform(0.9, 0.97))
            go(sc, r, x, r.uniform(-10, 10), way, r.uniform(430, 470), ROOF, d, stride=44, swing=40, bob=r.choice((34, 46, 58)), rate=r.uniform(0.7, 0.9),
               **({'arm_l': 'vee', 'arm_r': 'vee', 'swing_l': 20, 'swing_r': -20, 'axis_l': 'y', 'axis_r': 'y'} if i == n - 1 else {}))
        if guard:
            go(sc, r, -way * 900.0, 0, way, 380.0, ROOF, person(r, wear='trousers', sleeves='long', top=NAVY, bottom=NAVY, hat='peak', hat_col=NAVY,
               band=YEL, build='stocky', hair='short'), stride=36, bob=5, rate=2.0, arm_r='up', swing_r=22, axis_r='y', arm_l='fwd', swing_l=0)
        lit(sc, 0.14)
    return build


JOBS = [
    ('SM_P3C_Runners_A', runners(31, way=1)),
    ('SM_P3C_Runners_B', runners(32, way=-1, pram=True)),
    ('SM_P3C_Skaters_A', skaters(33)),
    ('SM_P3C_Porters_A', porters(34)),
    ('SM_P3C_School_A', school(35)),
    ('SM_P3C_DogWalk_A', dog_walk(36)),
    ('SM_P3C_Patrol_A', patrol(37, file_of=3)),
    ('SM_P3C_Patrol_B', patrol(38, file_of=4)),
    ('SM_P3C_Mounted_A', mounted(39)),
    ('SM_P3C_MarchMove_A', march_move(40)),
    ('SM_P3C_SideStreet_A', side_street(41)),
    ('SM_P3C_SideStreet_B', side_street(42, cab=True)),
    ('SM_P3C_RoofRun_A', roof_run(43, way=1), dict(grow=(60.0, 60.0, 120.0))),
    ('SM_P3C_RoofRun_B', roof_run(44, way=-1, guard=True), dict(grow=(60.0, 60.0, 120.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
