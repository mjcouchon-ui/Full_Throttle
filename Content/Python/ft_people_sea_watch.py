"""FULL THROTTLE (Unreal) the population of level 6, Under the Sea: THE WATCHERS (/Game/People/UnderTheSea/SM_Watch*).

Nereida has come out to see the chase. These are the crowds, each brought with the thing it stands on (the dressing's
own domes and hulls have no decks a figure could stand on, so every piece here is its own small building):

    SM_WatchBalcony_A   a habitat drum on a pylon, a balcony all round it 4.3 m over the sand, a dozen aquanauts at the
                        rail hopping (slowly: this is water) and waving, two more hanging in the water with flags.
    SM_WatchBalcony_B   the same with a second crowd on the drum's roof.
    SM_WatchAirlock     an airlock tower with two landing platforms, a crowd on each, one diver rising beside it.
    SM_WatchDome        a viewing dome: a crowd on the walkway round its rim, some sitting on the edge.
    SM_WatchWall        no stand at all: divers hanging upright in the water in three rows, a long striped banner
                        held between them.
    SM_WatchFarGallery  for the trench walls: a long gallery, big figures, a band of colour that hops (far=True).

The same aquanaut as the main kit's crowds (ft_people_sea.py: a suit of one strong colour, never blue; the head is the
white helmet with a dark visor; a pack), so the two kits meet without a seam.

What reads from this road: the sand lies 0.9 m under the road and the barrier is a metre high, so every deck here is
2.6 m or more over the sand, the figures are a quarter larger than life, and the crowds are massed. All TALL: 24 m or
more from the centre line. Steady light only (the drum's ports), and nothing here is blue-grey with a fin.

A piece is drawn like every prop: cm; z = 0 is the sand; the middle of the footprint is the origin; the FRONT faces -Y
(the road); long pieces run along X (the road).

Run in the editor:  import ft_people_sea_watch as K; K.run()        (or K.run(['SM_WatchAirlock']))
The placing rules: the "watch" block of Source/FullThrottle/FTPeopleL6.inl.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/UnderTheSea'

SUITS = ('#ffc93c', '#ff7a3c', '#ff5fa2', '#fff6ea', '#b8ff5a', '#ff2d3c', '#f4a6ff', '#7dffb0')
HELM, VISOR, STEEL, DARK, WHITE, YEL = '#f4f9ff', '#22202a', '#8f9bb3', '#22202a', '#fff6ea', '#ffc93c'
DECK, DECK_D = '#cfe3ea', '#7fa2b4'              # the station's metal (ft_dress_abyss.py: STATION, STATION_D)
HULL_D, PORT, LAMP = '#3d4763', '#0a1a2a', '#fff1c0'
MAG, RED, ORANGE, LIME, PINK = '#ff4fb8', '#ff2d3c', '#ff7a3c', '#b8ff5a', '#ff5fa2'
PANELS = (MAG, WHITE, YEL, WHITE, ORANGE, WHITE)
DOME = '#8fe6ee'

# what a watcher does. A hop (bob > 0) is slow here, and grows as the chase comes by (react); a float (bob < 0) does not.
ACTS = (
    dict(arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=26, rate=0.55),
    dict(arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=-16, rate=0.6),
    dict(arm_r='wave', swing_r=30, arm_l='hip', bob=18, rate=0.8),
    dict(arm_l='wave', swing_l=28, arm_r='up', swing_r=-14, bob=30, rate=0.5),
    dict(arm_r='point', swing_r=12, axis_r='x', arm_l='down', bob=-8, rate=0.6),
    dict(arm_l='up', arm_r='wave', swing_l=14, swing_r=-30, bob=22, rate=0.7),
)
SIT = (
    dict(arm_l='up', arm_r='up', swing_l=18, swing_r=-18, rate=0.6),
    dict(arm_r='wave', swing_r=30, arm_l='down', rate=0.8),
    dict(arm_l='wave', swing_l=28, arm_r='point', rate=0.5),
)
FLOAT = (
    dict(arm_l='up', arm_r='up', swing_l=20, swing_r=-20, bob=-34, rate=0.4),
    dict(arm_l='out', arm_r='wave', swing_r=32, bob=-24, rate=0.5),
    dict(arm_l='vee', arm_r='vee', swing_l=16, swing_r=-16, bob=-44, rate=0.33),
    dict(arm_l='wave', swing_l=30, arm_r='out', bob=-28, rate=0.45),
)


def watcher(sc, r, x, y, z, yaw=0.0, s=1.3, **kw):
    """One aquanaut of a crowd, about 120 triangles: figure() at detail 0 with the head itself as the white helmet,
    and the dark visor drawn on (it takes the body's own motion values)."""
    suit = kw.pop('suit', None) or r.choice(SUITS)
    fins = kw.pop('fins', None)
    d = dict(skin=HELM, hair='bald', hat=None, top=suit, bottom=suit, shoes=DARK, sleeves='long', wear='trousers',
             build=r.choice(('avg', 'stocky', 'avg', 'slim')), pack=r.choice((STEEL, YEL, DARK)), detail=0, s=s)
    d.update(kw)
    P.figure(sc, x, y, z=z, yaw=yaw, **d)
    if d.get('rock') or d.get('turn') or d.get('sway') or d.get('travel'):
        return
    drop = (66.0 - d.get('seat', 46.0)) if d.get('legs') == 'sit' else 0.0
    mo = P.motion(phase=d.get('phase', 0.0), rate=d.get('rate', 0.0), bob=d.get('bob', 0.0), react=d.get('react', False), base=(x, y),
                  pivot=(x, y, z + d['s'] * 100.0), axis=yaw)
    v = sc.part(mo, (x, y, z), yaw, d['s'])
    v.box(VISOR, (0, -21, 149 - drop), (34, 8, 16))
    if fins:                                       # hanging in the water: the fins point down and a little ahead
        for sd in (-1, 1):
            tilt_box(v, fins, (sd * 10, -4, 6), (sd * 13, -22, -46), 26, 7)


def crowd(sc, r, spots, seed=0, s=1.42, kid_every=6, **kw):
    """spots: [(x, y, z, yaw)]. Each a watcher with its own act, out of step with the next."""
    for i, (x, y, z, yaw) in enumerate(spots):
        d = dict(ACTS[(i * 5 + seed) % len(ACTS)])
        ss = s * r.uniform(0.94, 1.04)
        if kid_every and i % kid_every == kid_every - 2:
            d.update(build='kid')
            ss = s * 0.7
        d.update(kw)
        watcher(sc, r, x, y, z, yaw=yaw + r.uniform(-10, 10), s=ss, phase=r.random(), react=True, **d)


def pennant(sc, x, y, z0, h, col, stripe, phase=0.0, w=130.0, hang=260.0):
    """A tall pennant on a pole: flat colour that reads from far off; it swings a little about the pole."""
    g = sc.still()
    g.box(DECK_D, (x, y, z0 + h / 2.0), (9, 9, h))
    f = sc.part(P.motion(phase=phase, rate=0.4, swing=10.0, pivot=(x, y, z0 + h), axis=P.UP))
    cx = x + w / 2.0 + (5 if w > 0 else -5)
    f.box(col, (cx, y, z0 + h - hang / 2.0), (abs(w), 5, hang))
    f.box(stripe, (cx, y, z0 + h - hang / 2.0), (abs(w) + 2, 7, hang * 0.26))


def floater(sc, r, x, y, z, col, yaw=0.0, s=1.35, w=120.0, phase=0.0):
    """One who hangs in the water over the crowd with a flag, rising and settling."""
    watcher(sc, r, x, y, z, yaw=yaw, s=s, arm_r='up', arm_l='out', hold_r=P.hold_flag(col=col, w=w), swing_r=16, bob=-34, rate=0.33, phase=phase)


def fascia(g, x0, x1, y, z, h=70.0, n=6, off=0):
    """A deck's front edge as blocks of colour (stripes and shapes, never words)."""
    w = (x1 - x0) / float(n)
    for k in range(n):
        g.box(PANELS[(k + off) % len(PANELS)], (x0 + (k + 0.5) * w, y, z), (w - 8, 6, h))


# ------------------------------------------------------------------------------------------ the pieces
def balcony(seed=1, roof=False):
    """A habitat drum on a pylon with a balcony all round, 4.3 m over the sand: a crowd at the rail."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z, R, RB = 430.0, 290.0, 540.0           # the balcony's deck, the drum, the balcony's edge
        g.cyl(HULL_D, (0, 0, -40), 95, Z + 20, steps=8)
        for a in (30, 150, 270):
            ca, sa = math.cos(math.radians(a)), math.sin(math.radians(a))
            rod(g, DECK_D, (ca * 80, sa * 80, Z - 150), (ca * 430, sa * 430, -30), 22, steps=5)
            g.box(HULL_D, (ca * 430, sa * 430, 0), (120, 120, 50))
            rod(g, DECK_D, (ca * 90, sa * 90, Z - 200), (ca * (RB - 60), sa * (RB - 60), Z - 20), 16, steps=5)
        g.cyl(DECK, (0, 0, Z - 18), RB, 18, steps=14)
        g.cyl(DECK_D, (0, 0, Z - 40), RB - 30, 22, steps=14)
        g.cyl(DECK, (0, 0, Z), R, 330, steps=12)
        g.cyl(YEL if seed % 2 else ORANGE, (0, 0, Z + 250), R + 8, 44, steps=12)
        g.cyl(DECK_D, (0, 0, Z + 330), R + 24, 26, steps=12)
        if not roof:
            g.cyl(DECK, (0, 0, Z + 356), R * 0.7, 60, steps=12, top_r=R * 0.4)
        for k in range(6):                         # ports with a steady light behind them (the piece stands 24 m out or more)
            a = math.radians(k * 60 + 30)
            g.box(PORT, (math.cos(a) * (R - 2), math.sin(a) * (R - 2), Z + 160), (12, 110, 80), rot=(0, math.degrees(a), 0))
            g.box(LAMP, (math.cos(a) * (R + 3), math.sin(a) * (R + 3), Z + 160), (6, 78, 48), rot=(0, math.degrees(a), 0), glow=0.5)
        # the rail: posts all round, panels of colour on the road's side
        n = 14
        for k in range(n):
            a = math.radians(k * 360.0 / n)
            g.box(DECK_D, (math.sin(a) * (RB - 10), -math.cos(a) * (RB - 10), Z + 55), (8, 8, 110))
            am = math.radians((k + 0.5) * 360.0 / n)
            px, py = math.sin(am) * (RB - 14), -math.cos(am) * (RB - 14)
            chord = 2 * (RB - 10) * math.sin(math.pi / n)
            yawp = math.degrees(am)
            g.box(DECK_D, (px, py, Z + 110), (chord, 7, 7), rot=(0, yawp, 0))
            if math.cos(am) > -0.3:
                g.box(PANELS[(k + seed) % len(PANELS)], (px, py, Z + 40), (chord - 10, 5, 56), rot=(0, yawp, 0))
        spots = []
        m = 12 if not roof else 10
        for k in range(m):
            a = -112.0 + k * 224.0 / (m - 1)
            rr = 440.0 if k % 2 else 395.0
            spots.append((math.sin(math.radians(a)) * rr, -math.cos(math.radians(a)) * rr, Z, a * 0.55))
        crowd(sc, r, spots, seed)
        if roof:
            ZR = Z + 356.0
            g.cyl(DECK_D, (0, 0, ZR + 96), R + 20, 7, steps=12)
            for k in range(8):
                a = math.radians(k * 45.0)
                g.box(DECK_D, (math.sin(a) * (R + 16), -math.cos(a) * (R + 16), ZR + 50), (7, 7, 100))
            crowd(sc, r, [(math.sin(math.radians(a)) * 190, -math.cos(math.radians(a)) * 190, ZR, a * 0.5) for a in (-80, -40, 0, 40, 80)], seed + 2, kid_every=0)
            pennant(sc, 0, 120, ZR, 520, RED, WHITE, 0.3, w=170.0, hang=300.0)
        else:
            floater(sc, r, -250, -80, Z + 520, RED, yaw=10, phase=0.2)
            floater(sc, r, 280, -40, Z + 600, YEL, yaw=-12, phase=0.7)
        pennant(sc, -RB + 20, 60, Z, 560, YEL, RED, 0.1, w=-150.0)
        pennant(sc, RB - 20, 60, Z, 500, WHITE, MAG, 0.6, w=150.0)
    return build


def airlock(seed=1):
    """An airlock tower, 10 m, with two landing platforms on the road's side (3.0 and 6.0 m over the sand), a crowd on
    each; one diver rises and sinks beside the tower as if just out of the lock."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(DECK, (0, 120, -40), 190, 1000, steps=10)
        g.cyl(HULL_D, (0, 120, 960), 205, 40, steps=10)
        g.cyl(DECK_D, (0, 120, 1000), 120, 70, steps=10, top_r=60)
        for z in (250, 550, 850):
            g.cyl(MAG if z != 550 else YEL, (0, 120, z), 198, 30, steps=10)
        Z1, Z2 = 300.0, 600.0
        L1, L2 = 1100.0, 760.0
        # the decks
        g.box(DECK, (0, -270, Z1 - 9), (L1, 460, 18))
        g.box(DECK, (0, -130, Z2 - 9), (L2, 240, 18))
        fascia(g, -L1 / 2.0, L1 / 2.0, -503, Z1 + 34, 64, 6, seed)
        fascia(g, -L2 / 2.0, L2 / 2.0, -253, Z2 + 34, 64, 4, seed + 3)
        for (L, yf, z) in ((L1, -500, Z1), (L2, -250, Z2)):
            g.box(DECK_D, (0, yf, z + 112), (L, 7, 7))
            for k in range(5):
                g.box(DECK_D, (-L / 2.0 + k * L / 4.0, yf, z + 56), (8, 8, 112))
        for sx in (-1, 1):
            g.box(DECK_D, (sx * (L1 / 2.0 - 30), -460, Z1 / 2.0 - 20), (18, 18, Z1 + 20))
            rod(g, DECK_D, (sx * 150, 60, Z1 - 200), (sx * (L1 / 2.0 - 60), -300, Z1 - 20), 16, steps=5)
            rod(g, DECK_D, (sx * 150, 60, Z2 - 220), (sx * (L2 / 2.0 - 40), -180, Z2 - 20), 16, steps=5)
        # the hatches and their lamps (steady)
        for z in (Z1, Z2):
            g.box(PORT, (0, -68, z + 110), (120, 14, 200))
            g.box(HULL_D, (0, -72, z + 220), (150, 16, 18))
            g.box(LAMP, (0, -80, z + 250), (60, 6, 22), glow=0.5)
        # the ladder between the decks
        for sx in (-1, 1):
            g.box(DECK_D, (330 + sx * 26, -20, (Z1 + Z2) / 2.0), (6, 6, Z2 - Z1))
        for k in range(6):
            g.box(DECK_D, (330, -20, Z1 + 30 + k * 48), (52, 6, 6))
        low = [(-L1 / 2.0 + 90 + k * (L1 - 180) / 6.0, -440 + (k % 2) * 90, Z1, 0.0) for k in range(7)]
        high = [(-L2 / 2.0 + 80 + k * (L2 - 160) / 4.0, -190 + (k % 2) * 70, Z2, 0.0) for k in range(5)]
        crowd(sc, r, low, seed)
        crowd(sc, r, high, seed + 3, kid_every=0)
        # just out of the lock: rising beside the tower, a flag in the hand
        watcher(sc, r, -360, 100, 760, yaw=14, s=1.4, arm_r='up', arm_l='out', hold_r=P.hold_flag(col=LIME, w=130.0), swing_r=16, bob=-90, rate=0.16, phase=0.4)
        pennant(sc, L1 / 2.0 - 10, -440, Z1, 420, RED, WHITE, 0.2, w=140.0, hang=230.0)
        pennant(sc, -L1 / 2.0 + 10, -440, Z1, 380, YEL, MAG, 0.7, w=-140.0, hang=230.0)
    return build


def dome(seed=1):
    """A viewing dome on its drum: a walkway round the rim 3.2 m over the sand, a crowd standing on it and a row
    sitting on its edge with their legs over the side; three more on the crown."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z, RD, RB = 320.0, 430.0, 700.0
        g.cyl(DECK_D, (0, 0, -40), 500, Z + 20, steps=12)
        g.cyl(HULL_D, (0, 0, 120), 508, 40, steps=12)
        g.cyl(DECK, (0, 0, Z - 20), RB, 20, steps=14)
        g.cyl(MAG, (0, 0, Z - 44), RB - 14, 24, steps=14)
        g.sphere(DOME, (0, 0, Z), RD, scale=(1, 1, 0.8), steps=(5, 12))
        g.cyl(WHITE, (0, 0, Z), RD + 10, 30, steps=12)
        g.cyl(WHITE, (0, 0, Z + RD * 0.8 * 0.62), RD * 0.79 + 8, 16, steps=12)
        ZC = Z + RD * 0.8 - 6
        g.cyl(DECK, (0, 0, ZC - 30), 170, 30, steps=10)
        g.cyl(DECK_D, (0, 0, ZC + 96), 172, 7, steps=10)
        for k in range(6):
            a = math.radians(k * 60.0)
            g.box(DECK_D, (math.sin(a) * 166, -math.cos(a) * 166, ZC + 50), (7, 7, 100))
        for k in range(8):                         # struts under the walkway
            a = math.radians(k * 45.0 + 22.0)
            rod(g, DECK_D, (math.cos(a) * 490, math.sin(a) * 490, 60), (math.cos(a) * (RB - 40), math.sin(a) * (RB - 40), Z - 30), 14, steps=5)
        stand, sit = [], []
        for k in range(9):
            a = -100.0 + k * 200.0 / 8.0
            stand.append((math.sin(math.radians(a)) * 545, -math.cos(math.radians(a)) * 545, Z, a * 0.6))
        for k in range(6):
            a = -62.0 + k * 124.0 / 5.0
            sit.append((math.sin(math.radians(a)) * (RB + 6), -math.cos(math.radians(a)) * (RB + 6), Z, a))
        crowd(sc, r, stand, seed)
        for i, (x, y, z, a) in enumerate(sit):     # sitting on the edge: the seat is the deck
            ss = 1.25
            d = dict(SIT[(i + seed) % len(SIT)])
            watcher(sc, r, x, y, z - 46.0 * ss, yaw=a, s=ss, legs='sit', phase=r.random(), react=True, **d)
        crowd(sc, r, [(-80, -60, ZC, -20), (70, -80, ZC, 14), (0, 40, ZC, 0)], seed + 1, kid_every=0)
        pennant(sc, 0, 120, ZC, 460, YEL, RED, 0.4, w=170.0, hang=280.0)
        pennant(sc, -RB + 30, 150, Z, 430, WHITE, MAG, 0.1, w=-140.0)
        pennant(sc, RB - 30, 150, Z, 480, RED, WHITE, 0.6, w=140.0)
    return build


def wall(seed=1):
    """No stand at all: fourteen divers hanging upright in the water in three rows, 3 to 9 m over the sand, rising and
    settling out of step, waving; a long striped banner floats between the rows on two anchor lines."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L = 1500.0
        for sx in (-1, 1):                         # the anchors and their lines
            g.box(HULL_D, (sx * (L / 2.0 + 40), 0, 20), (110, 110, 60))
            g.box(DECK_D, (sx * (L / 2.0 + 40), 0, 300), (7, 7, 520))
            g.sphere(ORANGE if sx < 0 else YEL, (sx * (L / 2.0 + 40), 0, 600), 46, steps=(4, 8))
        b = sc.part(P.motion(rate=0.25, bob=-14.0, phase=0.3))
        n = 10
        for k in range(n):
            b.box(PANELS[(k + seed) % len(PANELS)], (-L / 2.0 + (k + 0.5) * L / n, 0, 470), (L / n, 6, 120))
        b.box(RED, (0, -1, 470), (L, 8, 26))
        for sx in (-1, 1):
            b.box(DECK_D, (sx * (L / 2.0 + 20), 0, 500), (44, 6, 6))
        rows = ((250.0, 5, -70.0), (560.0, 5, 60.0), (800.0, 4, 130.0))
        i = 0
        for z, m, y in rows:
            for k in range(m):
                x = -L / 2.0 + 130 + (k + (0.5 if m == 4 else 0.0)) * (L - 260) / 4.0 + r.uniform(-30, 30)
                d = dict(FLOAT[(i * 3 + seed) % len(FLOAT)])
                hold = None
                if i in (6, 12):
                    d.update(arm_r='up', swing_r=16)
                    hold = P.hold_flag(col=(LIME, RED)[i % 2], w=130.0)
                watcher(sc, r, x, y + r.uniform(-20, 20), z + r.uniform(-30, 30), yaw=r.uniform(-16, 16), s=r.uniform(1.35, 1.5), phase=r.random(), react=True,
                        fins=r.choice((RED, YEL, DARK, PINK, WHITE)), hold_r=hold, **d)
                i += 1
    return build


def far_gallery(seed=1):
    """For the trench walls, 110 m out and more: a gallery 42 m long on stilts, a wall of colour blocks behind it,
    twenty-two figures two and a half times life size hopping out of step. A band of colour that moves."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        L, Z = 4200.0, 520.0
        g.box(DECK, (0, 0, Z - 20), (L, 520, 40))
        g.box(DECK_D, (0, -250, Z - 70), (L, 20, 80))
        for k in range(6):
            x = -L / 2.0 + 200 + k * (L - 400) / 5.0
            g.box(HULL_D, (x, 0, Z / 2.0 - 60), (70, 70, Z + 80))
            g.box(HULL_D, (x, 190, Z / 2.0 - 60), (50, 50, Z + 80))
        n = 12
        for k in range(n):                         # the back wall: blocks of colour 5 m tall
            g.box(PANELS[(k + seed) % len(PANELS)], (-L / 2.0 + (k + 0.5) * L / n, 250, Z + 250), (L / n - 20, 14, 500))
        g.box(HULL_D, (0, 258, Z + 250), (L, 10, 520))
        g.box(DECK_D, (0, -255, Z + 150), (L, 14, 14))
        for k in range(8):
            g.box(DECK_D, (-L / 2.0 + k * L / 7.0, -255, Z + 75), (14, 14, 150))
        acts = (dict(arm_l='up', arm_r='up', swing_l=18, swing_r=-18, bob=70, rate=0.5), dict(arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=50, rate=0.62),
                dict(arm_r='wave', swing_r=30, arm_l='up', bob=85, rate=0.45), dict(arm_l='wave', swing_l=30, arm_r='up', bob=60, rate=0.55))
        m = 22
        for i in range(m):
            x = -L / 2.0 + 170 + i * (L - 340) / (m - 1) + r.uniform(-20, 20)
            y = -150.0 if i % 2 else 60.0
            suit = SUITS[(i * 3 + seed) % len(SUITS)]
            P.figure(sc, x, y, z=Z, yaw=r.uniform(-12, 12), s=r.uniform(2.3, 2.7), skin=HELM, hair='bald', top=suit, bottom=suit, sleeves='long', detail=0,
                     build=r.choice(('avg', 'stocky')), phase=r.random(), **acts[(i * 3 + seed) % len(acts)])
        pennant(sc, -L / 2.0 + 60, 200, Z, 1300, YEL, RED, 0.1, w=-420.0, hang=640.0)
        pennant(sc, L / 2.0 - 60, 200, Z, 1300, RED, WHITE, 0.6, w=420.0, hang=640.0)
        pennant(sc, 0, 200, Z, 1500, WHITE, MAG, 0.35, w=420.0, hang=640.0)
    return build


JOBS = [
    ('SM_WatchBalcony_A', balcony(1), dict(grow=(80.0, 80.0, 140.0))),
    ('SM_WatchAirlock', airlock(1), dict(grow=(80.0, 80.0, 200.0), grow_down=(80.0, 80.0, 100.0))),
]
REST = [
    ('SM_WatchBalcony_B', balcony(2, roof=True), dict(grow=(80.0, 80.0, 140.0))),
    ('SM_WatchDome', dome(1), dict(grow=(80.0, 80.0, 140.0))),
    ('SM_WatchWall', wall(1), dict(grow=(80.0, 80.0, 160.0), grow_down=(80.0, 80.0, 60.0))),
    ('SM_WatchFarGallery', far_gallery(1), dict(far=True, grow=(120.0, 120.0, 320.0))),
]


def run(only=None, everything=False):
    P.build_core()
    return P.export(DIR, JOBS + (REST if everything else []), only)


def paths(everything=False):
    return [DIR + '/' + j[0] for j in JOBS + (REST if everything else [])]
