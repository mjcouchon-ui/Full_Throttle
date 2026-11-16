"""FULL THROTTLE (Unreal) the population, level 7: City Center, THE WATCHERS. /Game/People/CityCenter/SM_Watch*.

The city has come out to watch the last scene being shot, and it has climbed onto everything: footbridges thrown over the
side streets, scaffold stands in the alleys' mouths with a police line on the first step, scaffold balconies up the brick
walls behind the alleys, the open decks of the parking garages, the roof terraces of the dear doors. Crowds of 14 to 24,
hopping out of step, arms up, flags and placards, long banners hung under them (stripes, bars and discs: never words,
never a chevron, which is the Syndicate's mark). Placed by the "watch" block of Source/FullThrottle/FTPeopleL7.inl.

WHERE THEY STAND (FTDressL7.inl, ft_dress_metro.py; the main cast, ft_people_city.py, has the kerb's pockets, the strip
under the awnings, most podium roofs, the glass infills and the plazas before the monuments: none of that is used here):
  SM_WatchBridge_A/B   a footbridge on its own legs across the mouth of a side street (G_Street: the gap between two
                       podiums, 13 m; the dressing's cars drive away from 33 m out, under and beyond it). Deck 5.2 m up,
                       25.4 to 27.8 m out. B has the city's police among the crowd.
  SM_WatchSteps_A      a scaffold stand of four steps in an alley's mouth (G_Alley: 9 m between its walls), 24.9 to 28 m
                       out, a police line on the first step, a barricade before it, a banner on two poles behind.
  SM_WatchBalcony_A    a scaffold up the brick building behind an alley (SM_Infill1: the same origin; its face is
                       y = -130), three decks 13, 20 and 26 m up, each full of people behind a cloth, banners under them.
                       People a third over life size: it is read from 40 to 150 m down the street, over the podiums.
  SM_WatchDeck_A       the garage's open decks (SM_GarageRamp: the same origin): people along the parapets of the third
                       and second decks, banners hung from the third.
  SM_WatchRoof_A       a roof's edge (the dear doors, W_Pent: 9 m of its terrace, the half the main cast leaves free):
                       a crowd behind a striped cloth, tall flags, banners down the front, a string of lights.
Every piece is TALL and stands 24 m or more from the road; it is night, so each carries steady light of its own (the
figures a faint one, the banners a little more, a string of bulbs): nothing blinks.

A watcher piece is a crowd: 1,700 to 2,000 triangles a piece, over the 1,500 guide (the back rows are drawn by mini(),
below: 60 triangles a person, the part of a person that shows over a rail or the row in front).

Run in the editor:  import ft_people_city_watch as K; K.run()        (or K.run(['SM_WatchBridge_A']))
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/CityCenter'
NAVY, DARK, GREY, STEEL, WHITE, CREAM = '#2b2350', '#22202a', '#3f3d50', '#8f9bb3', '#fff6ea', '#fff1d6'
PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, BLUE = '#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#ff2d3c', '#ff7a3c', '#8f6bff', '#3a5fa8'
WARM, WOOD, CONC = '#ffe28a', '#9a6a44', '#8a879c'
POLICE = '#3a5fa8'
HOT = (PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, WHITE, '#f4a6ff', '#b8ff5a', '#57c7ff')     # coats that read at night
G_BULB, G_CLOTH, G_FIG = 1.6, 0.17, 0.11


def lit(sc, g=G_FIG):
    """Night: everything of a piece that carries no light of its own gets a faint one (the main cast's way, so the two
    sit side by side). Only for pieces that stand 24 m or more from the road (the light rule)."""
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


def person(r, **fixed):
    d = P.look(r, tops=HOT, **fixed)
    d['s'] = fixed.get('s', r.uniform(0.93, 1.03))
    return d


def officer(r, **fixed):
    """The city's police: a blue coat, a white band, a peaked cap (no real force's dress)."""
    d = P.look(r, tops=(POLICE,), bottoms=(NAVY,), sleeves='long', wear='trousers', hat='peak', hat_col=POLICE, band=WHITE, shoes=DARK)
    d['s'] = r.uniform(0.96, 1.02)
    d.update(fixed)
    return d


# what a watcher does: (left arm, right arm, degrees the left swings, the right, hop cm, cycles a second)
ACTS = (
    ('up', 'up', 14, -14, 8, 1.6),
    ('vee', 'vee', 12, -12, 9, 1.4),
    ('wave', 'down', -26, 0, 4, 1.2),
    ('down', 'wave', 0, 26, 4, 1.3),
    ('up', 'down', 20, 0, 7, 1.8),
    ('wave', 'wave', -22, 22, 6, 1.1),
    ('down', 'up', 0, -20, 7, 1.7),
)
CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=14, swing_r=-14, bob=8, rate=1.6),
    dict(arm_r='wave', swing_r=26, rate=1.3, bob=4),
    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=9, rate=1.4),
    dict(arm_l='wave', arm_r='wave', swing_l=-22, swing_r=22, rate=1.1, bob=6),
    dict(arm_r='up', arm_l='down', swing_r=20, rate=1.8, bob=7),
    dict(arm_r='point', arm_l='hip', swing_r=12, axis_r='x', rate=0.8, bob=2),
)


def mini(sc, r, x, y, z=0.0, s=1.0, yaw=0.0, act=None, legs=False, hold=None, top=None, react=True):
    """The part of a person that shows over a rail, a cloth or the row in front: a coat, a head, hair, two arms that
    wave, hopping (60 triangles; legs=True adds a block for the legs). hold: f(model, hand, side) for the right hand."""
    d = person(r)
    a = act or ACTS[r.randrange(len(ACTS))]
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(p):
        return (x + s * (p[0] * cy - p[1] * sy), y + s * (p[0] * sy + p[1] * cy), z + s * p[2])

    common = dict(phase=r.random(), rate=a[5], bob=a[4], react=react)
    b = sc.part(P.motion(**common), (x, y, z), yaw, s)
    col = top or d['top']
    if legs:
        b.box(d['bottom'], (0, 0, 33), (36, 24, 66))
    b.box(col, (0, 0, 93), (46, 26, 58))
    b.box(d['skin'], (0, 0, 146), (38, 36, 44))
    b.box(d['hair_col'], (0, 3, 172), (41, 39, 13))
    for side, pose, sw in ((-1, a[0], a[2]), (1, a[1], a[3])):
        up, low = P.ARMS[pose]
        sh = (side * 28.0, 0.0, 115.0)
        d1, d2 = P._dir(up[0], up[1], side), P._dir(low[0], low[1], side)
        hand = (sh[0] + (d1[0] + d2[0]) * 27.0, sh[1] + (d1[1] + d2[1]) * 27.0, sh[2] + (d1[2] + d2[2]) * 27.0)
        m = sc.part(P.motion(swing=sw, pivot=W(sh), axis=yaw + 90.0, **common), (x, y, z), yaw, s) if sw else b
        tilt_box(m, col, sh, hand, 13, 13)
        if hold and side == 1:
            hold(m, hand, side)


def big_flag(col=PINK, bar=WHITE, pole=WHITE, w=120.0, h=76.0, L=230.0):
    """A big flag on a long pole in the right hand: one colour and a bar."""
    def f(m, h0, side):
        m.box(pole, (h0[0], h0[1], h0[2] + L / 2.0 - 30), (5, 5, L))
        m.box(col, (h0[0] + w / 2.0, h0[1], h0[2] + L - 30 - h / 2.0), (w, 3, h), glow=G_CLOTH)
        m.box(bar, (h0[0] + w / 2.0, h0[1] - 2.2, h0[2] + L - 30 - h / 2.0), (w * 0.9, 1.5, h * 0.3), glow=G_CLOTH)
    return f


def lean_flag(col=PINK, bar=WHITE, pole=WHITE, w=120.0, h=70.0, out=150.0, up=95.0):
    """A flag on a pole held OUT over a parapet (towards the road and up): for a deck with a slab close overhead, where an
    upright flag would stand inside the slab. It waves side to side with the arm."""
    def f(m, h0, side):
        tip = (h0[0], h0[1] - out, h0[2] + up)
        tilt_box(m, pole, (h0[0], h0[1] + 12, h0[2] - 8), tip, 5, 5)
        m.box(col, (tip[0] + w / 2.0, tip[1] + 6, tip[2] - h / 2.0), (w, 3, h), glow=G_CLOTH)
        m.box(bar, (tip[0] + w / 2.0, tip[1] + 3.8, tip[2] - h / 2.0), (w * 0.9, 1.5, h * 0.3), glow=G_CLOTH)
    return f


def placard(col=CREAM, bar=PINK, w=84.0, h=58.0):
    def f(m, h0, side):
        m.box(WOOD, (h0[0], h0[1], h0[2] + 45), (5, 5, 110))
        m.box(col, (h0[0], h0[1] - 2, h0[2] + 110), (w, 4, h), glow=G_CLOTH)
        m.box(bar, (h0[0], h0[1] - 4.6, h0[2] + 110), (w * 0.74, 1.5, h * 0.34), glow=G_CLOTH)
    return f


def cloth(m, x0, x1, y, z0, z1, base=CREAM, bars=(PINK, CYAN, YEL), kind=0, glow=G_CLOTH, t=5.0):
    """A banner between x0 and x1, from z0 up to z1: stripes and shapes, never words. kind 0: bars across and a disc;
    1: bars up and down; 2: a broad band and a disc; 3: halves."""
    w, h, xc, zc = x1 - x0, z1 - z0, (x0 + x1) / 2.0, (z0 + z1) / 2.0
    m.box(base, (xc, y, zc), (w, t, h), glow=glow)
    f = y - t / 2.0 - 0.8
    if kind == 0:
        for k, c in enumerate(bars[:2]):
            m.box(c, (xc, f, z0 + h * (0.16 + 0.26 * k)), (w * 0.9, 1.6, h * 0.16), glow=glow)
        m.cyl(bars[-1], (xc, f + 0.8, z0 + h * 0.74), min(w, h) * 0.2, 1.6, rot=(0, 0, 90), steps=8, glow=glow)
    elif kind == 1:
        n = 5
        for k in range(n):
            if k % 2 == 0:
                m.box(bars[(k // 2) % len(bars)], (x0 + (k + 0.5) * w / n, f, zc), (w / n, 1.6, h * 0.92), glow=glow)
    elif kind == 2:
        m.box(bars[0], (xc, f, zc), (w * 0.94, 1.6, h * 0.4), glow=glow)
        m.cyl(bars[1], (xc, f - 0.2, zc), min(w, h) * 0.3, 1.8, rot=(0, 0, 90), steps=8, glow=glow)
    else:
        m.box(bars[0], (x0 + w * 0.25, f, zc), (w * 0.5, 1.6, h * 0.96), glow=glow)
        m.box(bars[1], (x0 + w * 0.75, f, z0 + h * 0.5), (w * 0.4, 1.6, h * 0.3), glow=glow)


def drop(sc, r, x, y, z_top, w, h, base, bars, kind):
    """A banner hung by its top edge: it stirs."""
    m = sc.part(P.motion(swing=4.0, pivot=(x, y, z_top), axis=0.0, rate=r.uniform(0.3, 0.5), phase=r.random()))
    cloth(m, x - w / 2.0, x + w / 2.0, y, z_top - h, z_top, base=base, bars=bars, kind=kind)


def rail(m, x0, x1, y, z0, h=104.0, col=STEEL, n=4):
    for k in range(n + 1):
        m.box(col, (x0 + k * (x1 - x0) / n, y, z0 + h / 2.0), (6, 6, h))
    m.box(col, ((x0 + x1) / 2.0, y, z0 + h), (x1 - x0 + 6, 7, 7))
    m.box(col, ((x0 + x1) / 2.0, y, z0 + h * 0.55), (x1 - x0, 5, 5))


def bulbs(m, a, b, n, sag=40.0, cols=(WARM, PINK, CYAN, YEL)):
    """A string of lights from a to b, sagging: steady."""
    for k in range(1, n + 1):
        t = k / float(n + 1)
        p = (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t - sag * 4.0 * t * (1.0 - t))
        m.box(cols[k % len(cols)], p, (18, 18, 18), glow=G_BULB)


def sawhorse(m, x, y, L=190.0, col=POLICE):
    """A police barricade: two A-frames and a board in blue and white bars, 1.05 m high."""
    for sx in (-1, 1):
        tilt_box(m, col, (x + sx * (L / 2.0 - 16), y - 26, 0), (x + sx * (L / 2.0 - 16), y, 100), 7, 7)
        tilt_box(m, col, (x + sx * (L / 2.0 - 16), y + 26, 0), (x + sx * (L / 2.0 - 16), y, 100), 7, 7)
    n = 6
    for k in range(n):
        m.box(WHITE if k % 2 else col, (x - L / 2.0 + (k + 0.5) * L / n, y, 92), (L / n, 6, 22))


SETS = (((PINK, CYAN, YEL), CREAM), ((YEL, VIOLET, MINT), WHITE), ((CYAN, RED, WHITE), NAVY), ((ORANGE, WHITE, CYAN), VIOLET))


# ------------------------------------------------------------------------------------------ the footbridge
def bridge(seed=1, police=False, girder=YEL):
    """TALL: a footbridge on its own legs across the mouth of a side street, 12.4 m long, its deck 5.2 m up. Eight
    people at the rail and seven behind on a step, flags, three banners hung under the deck, a string of lights.
    police: three of the city's police among them, arms out."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Z = 520.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(GREY, (sx * 585, sy * 95, (Z - 30) / 2.0), (26, 26, Z - 30))
            tilt_box(g, GREY, (sx * 585, -95, 60), (sx * 585, 95, Z - 60), 9, 9)
        g.box(CONC, (0, 0, Z - 15), (1240, 250, 30))
        for sy in (-1, 1):
            g.box(girder, (0, sy * 127, Z - 25), (1240, 8, 90))
        rail(g, -612, 612, -118, Z)
        g.box(GREY, (0, 50, Z + 17), (1160, 90, 34))                        # the back row's step
        tilt_box(g, GREY, (-560, 190, 0), (300, 190, Z - 12), 110, 14)      # the stair behind
        g.box(GREY, (430, 190, Z - 15), (270, 110, 30))
        for sx in (-1, 1):
            g.box(STEEL, (sx * 606, -118, Z + 170), (7, 7, 340))
        g.box(DARK, (0, -118, Z + 338), (1212, 4, 4))
        bulbs(g, (-606, -118, Z + 326), (606, -118, Z + 326), 9, sag=0)
        for k, x in enumerate((-400.0, 0.0, 400.0)):
            bars, base = SETS[(seed + k) % len(SETS)]
            drop(sc, r, x, -133, Z - 72, 270.0, 250.0, base, bars, (k + seed) % 4)
        n = 8
        for i in range(n):
            x = -500 + i * 1000.0 / (n - 1) + r.uniform(-8, 8)
            if police and i in (1, 4, 6):
                P.figure(sc, x, -78, z=Z, yaw=r.uniform(-8, 8), arm_l='out', arm_r='out', rock=3, rate=0.4, phase=r.random(), detail=0, **officer(r))
                continue
            d = person(r)
            if i in (2, 5):
                d.update(arm_r='wave', swing_r=22, rate=0.9, bob=4, hold_r=big_flag(col=(PINK, CYAN, YEL)[(i + seed) % 3], bar=WHITE))
            else:
                d.update(CHEER[(i * 3 + seed) % len(CHEER)])
            P.figure(sc, x, -78, z=Z, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=0, **d)
        for i in range(7):
            x = -430 + i * 143 + r.uniform(-10, 10)
            mini(sc, r, x, 50, z=Z + 34, yaw=r.uniform(-12, 12), legs=True,
                 hold=placard(col=(CREAM, YEL, WHITE)[i % 3], bar=(PINK, VIOLET, RED)[i % 3]) if i in (1, 5) else None,
                 act=('down', 'wave', 0, 10, 8, 1.3) if i in (1, 5) else None)
        lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the stand in an alley's mouth
def steps(seed=3):
    """TALL: a scaffold stand of four steps, 8 m wide and 3 m deep, a police barricade before it and a police line on
    the first step (arms out, holding the crowd back); three rows of the city above them, placards and flags, and a
    banner on two poles behind. y = 0 is the stand's middle; its front (the barricade) is y = -221."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for k in range(4):
            g.box((GREY, CONC)[k % 2], (0, -117 + 78 * k, 25 * (k + 1)), (800, 78, 50 * (k + 1)))
        for sx in (-1, 1):
            g.box(STEEL, (sx * 404, 0, 130), (8, 300, 8))
            g.box(STEEL, (sx * 404, 150, 150), (8, 8, 300))
            g.box(WOOD, (sx * 340, 165, 290), (9, 9, 580))
        sawhorse(g, -205, -195, L=380.0)
        sawhorse(g, 205, -195, L=380.0)
        bars, base = SETS[seed % len(SETS)]
        ban = sc.part(P.motion(swing=4.0, pivot=(0.0, 165.0, 576.0), axis=0.0, rate=0.4, phase=r.random()))
        cloth(ban, -340, 340, 165, 452, 576, base=base, bars=bars, kind=3)
        for i in range(4):
            P.figure(sc, -300 + i * 200 + r.uniform(-8, 8), -117, z=50, yaw=r.uniform(-6, 6), arm_l='out', arm_r='out', rock=3, rate=0.4,
                     phase=r.random(), detail=0, **officer(r))
        for k in range(1, 4):
            n = 6 if k != 2 else 5
            for i in range(n):
                x = -(n - 1) * 65.0 + i * 130.0 + r.uniform(-9, 9)
                hold, act = None, None
                if k == 3 and i in (1, 4):
                    hold, act = big_flag(col=(CYAN, PINK)[i % 2], bar=WHITE), ('down', 'wave', 0, 22, 4, 0.9)
                elif k == 2 and i in (0, 3):
                    hold, act = placard(col=(CREAM, YEL)[i % 2], bar=(RED, VIOLET)[i % 2]), ('down', 'wave', 0, 10, 8, 1.3)
                mini(sc, r, x, -117 + 78 * k, z=50 * (k + 1), yaw=r.uniform(-12, 12), legs=(k == 1), hold=hold, act=act)
        lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the scaffold up the brick wall
def balcony(seed=5):
    """TALL: a scaffold up the face of the brick building behind an alley (SM_Infill1's own origin: its face is
    y = -130), three decks 13.2, 19.8 and 26.4 m up, each with seven people behind a cloth in its own colour and two
    banners hung under it. The people are 1.3 times life size: this is read from far down the street."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        S = 1.3
        for x in (-588.0, -196.0, 196.0, 588.0):
            g.box(STEEL, (x, -252, 1460), (10, 10, 2920))
        for k, z in enumerate((1320.0, 1980.0, 2640.0)):
            bars, base = SETS[(seed + k) % len(SETS)]
            g.box(GREY, (0, -195, z + 9), (1190, 124, 18))
            cloth(g, -595, 595, -259, z + 18, z + 112, base=base, bars=bars, kind=1)
            g.box(STEEL, (0, -252, z + 250), (1190, 8, 8))
            for j, x in enumerate((-340.0, 340.0)):
                b2, base2 = SETS[(seed + k + j + 1) % len(SETS)]
                drop(sc, r, x, -262, z - 6, 230.0, 330.0, base2, b2, (k + j) % 3 if (k + j) % 3 != 1 else 2)
            for i in range(7):
                x = -490 + i * 163.3 + r.uniform(-10, 10)
                hold, act = None, None
                if i == (1 + 2 * k) % 7:
                    hold, act = big_flag(col=(PINK, YEL, CYAN)[k], bar=WHITE, w=140.0, h=90.0, L=250.0), ('down', 'wave', 0, 22, 4, 0.9)
                mini(sc, r, x, -200, z=z + 18, s=S, yaw=r.uniform(-12, 12), hold=hold, act=act)
        lit(sc, 0.13)
    return build


# ------------------------------------------------------------------------------------------ the garage's decks
def deck(seed=7):
    """TALL: stands at a parking garage (SM_GarageRamp's own origin; its parapets are y = -444, its decks 9.37 and
    6.37 m up): sixteen people along the third deck's parapet, eight on the second in the two middle bays, banners
    hung from the third deck over the outer bays and cloths on its parapet."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        S = 1.12
        z3, z2 = 937.0, 637.0
        for b, cx in enumerate((-1155.0, -385.0, 385.0, 1155.0)):
            for j, dx in enumerate((-270.0, -90.0, 90.0, 270.0)):
                hold, act = None, None
                if (b * 4 + j) % 7 == 3:
                    # held out over the parapet: the roof slab is 2.66 m over this deck (an upright flag stood inside it)
                    hold, act = lean_flag(col=(PINK, CYAN, YEL)[b % 3], bar=WHITE), ('down', 'wave', 0, 20, 3, 0.9)
                # the garage's PARK blade stands before x = 1385 to 1515: nobody behind it
                mini(sc, r, min(cx + dx + r.uniform(-14, 14), 1335.0), -408, z=z3, s=S, yaw=r.uniform(-12, 12), hold=hold, act=act)
            bars, base = SETS[(seed + b) % len(SETS)]
            if b in (0, 3):
                drop(sc, r, cx + (45.0 if b == 0 else -45.0), -456, z3 - 6, 420.0, 250.0, base, bars, (0, 2)[b % 2])   # clear of the PARK blade
            else:
                cloth(g, cx - 300, cx + 300, -453, z3 + 4, z3 + 84, base=base, bars=bars, kind=1)
                for dx in (-270.0, -90.0, 90.0, 270.0):
                    mini(sc, r, cx + dx + r.uniform(-14, 14), -408, z=z2, s=S, yaw=r.uniform(-12, 12))
        lit(sc)
    return build


# ------------------------------------------------------------------------------------------ a roof's edge
def roof(seed=9, L=900.0):
    """TALL: stands on a podium's roof (the set's "up" is the roof; y = 0 is the podium's middle, the roof's edge
    y = -350): nine people behind a striped cloth at the edge and seven behind them on a step, four tall flags, three
    banners down the front, a string of lights."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h = L / 2.0
        bars, base = SETS[seed % len(SETS)]
        cloth(g, -h, h, -342, 0, 98, base=base, bars=bars, kind=1)
        g.box(GREY, (0, -262, 18), (L - 80, 60, 36))
        for sx in (-1, 1):
            g.box(STEEL, (sx * h, -338, 160), (7, 7, 320))
        g.box(DARK, (0, -338, 318), (L, 4, 4))
        bulbs(g, (-h, -338, 306), (h, -338, 306), 8, sag=0)
        # the outer two 3.3 m from the middle: the door's raking flag pole comes through the facade's line 2 m along
        for k, x in enumerate((-h + 120, 0.0, h - 120)):
            b2, base2 = SETS[(seed + k + 1) % len(SETS)]
            drop(sc, r, x, -380, -6, 220.0, 330.0, base2, b2, (0, 2, 3)[k])
        for i in range(9):
            x = -h + 50 + i * (L - 100) / 8.0 + r.uniform(-8, 8)
            hold, act = None, None
            if i in (1, 3, 5, 7):
                hold, act = big_flag(col=(PINK, CYAN, YEL, MINT)[i // 2], bar=WHITE, L=300.0, w=150.0, h=92.0), ('down', 'wave', 0, 20, 4, 0.8 + 0.1 * (i // 2))
            mini(sc, r, x, -306, z=0, s=1.1, yaw=r.uniform(-12, 12), hold=hold, act=act)
        # the back row stands either side of the terrace's parasol (its rim, 1.5 m round x = -95, reaches this row from x = -143 to -47)
        for x in (-340.0, -262.0, -186.0, 10.0, 120.0, 230.0, 340.0):
            mini(sc, r, x + r.uniform(-8, 8), -262, z=36, s=1.1, yaw=r.uniform(-12, 12))
        lit(sc)
    return build


JOBS = [
    ('SM_WatchBridge_A', bridge(1)),
    ('SM_WatchBridge_B', bridge(21, police=True, girder=CYAN)),
    ('SM_WatchSteps_A', steps(3)),
    ('SM_WatchBalcony_A', balcony(5)),
    ('SM_WatchDeck_A', deck(7)),
    ('SM_WatchRoof_A', roof(9)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
