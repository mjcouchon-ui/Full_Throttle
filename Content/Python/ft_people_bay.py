"""FULL THROTTLE (Unreal) the population, level 0: Bayshore (/Game/People/Bayshore/).

A seaside boardwalk city on a hot day, and the chase is the event of the afternoon. The bay is on the LEFT of travel
(the boardwalk at 22 m, the beach falling to the sea 355 m out), the town on the RIGHT (shopfronts at 27.5 m).
Placed by Source/FullThrottle/FTPeopleL0.inl; the film crew is ft_people_crew's, wardrobe 'Sun'.

What the game's frames taught (the chase camera rides 3.5 m over a road whose barrier is 1.6 m high, and the land
lies 0.7 to 0.9 m under the road): a person standing on the ground nearer than about 30 m is hidden behind the
barrier. So by the road the cast is RAISED (on balconies over the shop awnings, on stepped sun terraces, on a
lifeguard tower, on a bandstand) or it is the heads and hats of the boardwalk's walkers; on the open beach it is
in GROUPS round strong colours; far out it is BIG (a wheeling flock, surfers on the bar, dolphins).

    by the road (17-30 m)   SM_Strollers_A/B   walkers, joggers, a skater, dogs on the boardwalk (they travel)   LOW
                            SM_Shoppers_A      the town's pavement in front of the shopfronts                    LOW
                            SM_SunDeck_A/B     a stepped sun terrace full of people cheering the chase            TALL
                            SM_Balcony_A       people on a balcony over the shop awnings                          TALL
                            SM_GuardTower       the lifeguard tower: binoculars, a flag, the rescue boards         TALL
                            SM_Bandstand       a beach band on its stand, and the people dancing to it            TALL
    middle (30-150 m)       SM_Volley_A        a volleyball game: the ball flies, the players jump
                            SM_BeachDay_A/B    parasols, sunbathers, children and a ball, a dog running rings
                            SM_SurfWalk        surfers carrying their boards down to the water
                            SM_Anglers         anglers along a pier's rail (stood on the dressing's pier)
    far (150 m and out)     SM_Gulls           a flock wheeling over the beach                                   far
                            SM_SurfRiders      surfers riding the bar (afloat)                                   far
                            SM_Dolphins        a school of dolphins leaping (afloat)                             far

Run in the editor:  import ft_people_bay; ft_people_bay.run()        (or run(['SM_Volley_A']))
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/Bayshore'

# the level's own colours (ft_dress_bay / ft_city), and the hot ones people wear
WOOD, WOOD_D, WHITE, CREAM, SAND = '#c9a06a', '#9a744a', '#fffaf2', '#fff1d6', '#f2dcae'
CORAL, TEAL, YEL, HOTPINK, MINT, SKY, NAVY, RED, LAV = '#ff7a8a', '#19c5c0', '#ffc93c', '#ff4fb8', '#7dffb0', '#57c7ff', '#2b2350', '#ff2d3c', '#b9a4ff'
HOT = ('#ff4fb8', '#ff2d3c', '#ffc93c', '#2ee6ff', '#ff7a3c', '#7dffb0', '#8f6bff', '#fffaf2', '#b8ff5a', '#ff5fa2')
SWIM = ('#ff2d3c', '#2ee6ff', '#ffc93c', '#ff4fb8', '#2b2350', '#7dffb0', '#ff7a3c', '#8f6bff')
LIFT = 15.0          # the track sinks a piece 15 cm into the land: what stands on the sand is drawn this much up


def beach_look(r, **fixed):
    """Someone dressed for the beach: swimwear or shorts, a hot colour, often a hat."""
    wear = r.choice(('swim', 'shorts', 'shorts', 'swim', 'skirt'))
    d = P.look(r, tops=HOT, bottoms=SWIM, wear=wear, sleeves=r.choice(('none', 'short', 'none')))
    hat = r.choice((None, None, 'cap', 'sun', None))
    if hat:
        d.update(hat=hat, hat_col=r.choice(HOT))
    d.update(fixed)
    return d


def town_look(r, **fixed):
    d = P.look(r, tops=HOT, wear=r.choice(('shorts', 'trousers', 'skirt', 'dress', 'shorts')), sleeves='short')
    hat = r.choice((None, None, 'cap', 'sun', None, 'cap'))
    if hat:
        d.update(hat=hat, hat_col=r.choice(HOT))
    d.update(fixed)
    return d


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=9, rate=1.6),
    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=11, rate=1.4),
    dict(arm_r='wave', swing_r=28, rate=1.3, bob=4),
    dict(arm_l='wave', arm_r='wave', swing_l=-24, swing_r=24, rate=1.5, bob=6),
    dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7, bob=3),
    dict(arm_r='face', hold_r=P.hold_phone(), arm_l='down', rate=0.4, bob=-1.5),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.6, bob=3),      # clapping
    dict(arm_r='up', arm_l='hip', swing_r=20, rate=1.7, bob=8),
)


def walker(sc, r, x, y, d, L, speed=130.0, z=0.0, rate=0.95, bob=3.0, look=None, **kw):
    """Someone walking the piece's X (d = +1 or -1) and coming round again."""
    v = dict(stride=26, swing_l=20, swing_r=-20, axis_l='x', axis_r='x', detail=1)
    v.update(kw)
    return P.figure(sc, x, y, z=z, yaw=90.0 * d, rate=rate, bob=bob, phase=r.random(), travel=(L, speed * d), **dict(look or town_look(r), **v))


def dog(sc, x, y, d, L, speed=130.0, z=0.0, body='#e8c26a', s=1.0, phase=0.0):
    P.quadruped(sc, x, y, z=z, yaw=90.0 * d, s=s, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=24,
                rate=2.2, phase=phase, travel=(L, speed * d), body=body, ears='#5a4a3c', snout='#22202a')


def skater(sc, r, x, y, d, L, speed=420.0, z=0.0, board=HOTPINK, look=None):
    """A skateboarder rolling along the walk, side on, arms out, leaning with the board."""
    lk = dict(look or town_look(r, wear='shorts', hat='cap', hat_col=r.choice(HOT)))
    lk['s'] = min(lk.get('s', 1.0), 0.96)
    P.figure(sc, x, y, z=z + 12, yaw=90.0 * d + 62.0, legs='wide', arm_l='out', arm_r='out', rock=7, rate=0.5, phase=r.random(),
             travel=(L, speed * d), **lk)
    b = sc.part(P.motion(base=(x, y), travel=(float(L), speed * d / float(L))))
    b.box(board, (x, y, z + 9), (84, 24, 5))
    for sx in (-28, 28):
        b.box('#22202a', (x + sx, y, z + 4), (12, 26, 8))


# ------------------------------------------------------------------------------------------ by the road: the boardwalk
def strollers(seed=11, variant=0):
    """30 m of the boardwalk's traffic. Stood ON the dressing's boardwalk by a Set (up 0.48 m: its deck), so it is
    drawn from z = 0 and nothing in it is 2 m tall. From the road it is hats, heads and shoulders going by."""
    def build(sc):
        r = random.Random(seed)
        L = 3000.0
        n = 0
        lanes = (-70.0, -10.0, 55.0, 110.0)
        if variant == 0:
            # a couple, a child between them; a jogger the other way; a skater; a dog and its walker
            walker(sc, r, -1250, lanes[1], 1, L, look=town_look(r, hat='sun', hat_col=HOTPINK, wear='dress', top=YEL, s=0.97))
            walker(sc, r, -1190, lanes[2], 1, L, look=town_look(r, top='#2ee6ff', hat=None, s=1.0))
            walker(sc, r, -1120, lanes[1] - 10, 1, L, rate=1.5, look=town_look(r, build='kid', s=0.66, top=RED, hat='cap', hat_col=YEL))
            walker(sc, r, -500, lanes[3], -1, L, speed=330, rate=1.7, bob=7, stride=38, swing_l=34, swing_r=-34,
                   look=town_look(r, wear='shorts', sleeves='none', top='#b8ff5a', hat=None, band=None, s=0.98))
            skater(sc, r, -200, lanes[0], 1, L)
            walker(sc, r, 300, lanes[2], -1, L, look=town_look(r, top=HOTPINK, hat='cap', hat_col=WHITE, s=0.99), arm_r='down', swing_r=0)
            dog(sc, 240, lanes[2] - 55, -1, L, phase=0.3)
            walker(sc, r, 800, lanes[0], 1, L, speed=110, arm_l='carry', swing_l=0, hold_l=P.hold_bag(YEL),
                   look=town_look(r, wear='skirt', top='#ff7a3c', hat='sun', hat_col=WHITE, s=0.96))
            walker(sc, r, 1250, lanes[3], -1, L, speed=120, look=town_look(r, build='stocky', top='#8f6bff', hat='cap', hat_col=RED, s=0.98))
        else:
            # two joggers together, a skater the other way, a family in a row, a man and two dogs
            for k in range(2):
                walker(sc, r, -1300 + k * 90, lanes[0] + k * 50, 1, L, speed=340, rate=1.7, bob=7, stride=38, swing_l=34, swing_r=-34,
                       look=town_look(r, wear='shorts', sleeves='none', top=(HOTPINK, '#2ee6ff')[k], hat=(None, 'cap')[k], hat_col=YEL, s=0.97))
            skater(sc, r, 900, lanes[3], -1, L, board=YEL)
            for k in range(3):
                walker(sc, r, -400 + k * 75, lanes[1] + k * 45, -1, L, speed=105, rate=(0.9, 1.4, 0.9)[k],
                       look=town_look(r, build=('avg', 'kid', 'slim')[k], s=(1.0, 0.66, 0.97)[k], top=(RED, YEL, MINT)[k],
                                      hat=('cap', 'sun', 'sun')[k], hat_col=(WHITE, HOTPINK, '#ff7a3c')[k]))
            walker(sc, r, 350, lanes[2], 1, L, speed=140, look=town_look(r, top='#ff7a3c', hat='cowboy', hat_col=CREAM, s=0.95), arm_l='down', swing_l=0)
            dog(sc, 430, lanes[2] + 50, 1, L, speed=140, body='#5a4a3c', phase=0.1)
            dog(sc, 470, lanes[2] - 45, 1, L, speed=140, body=WHITE, s=0.8, phase=0.6)
            walker(sc, r, 1350, lanes[0], 1, L, speed=120, arm_r='face', swing_r=0, hold_r=P.hold_cup(HOTPINK),
                   look=town_look(r, wear='dress', top='#2ee6ff', hat='sun', hat_col=YEL, s=0.96))
    return build


def shoppers(seed=21, variant=0):
    """30 m of the town's pavement, in front of the shopfronts and under their awnings (24.6 m): shoppers with bags
    walking both ways, people stopped to watch the chase."""
    def build(sc):
        r = random.Random(seed)
        L = 3000.0
        z = LIFT
        g = sc.still()
        for i in range(7 if variant == 0 else 6):
            d = 1 if i % 2 else -1
            x = -1350 + i * 420 + r.uniform(-60, 60)
            y = (-30, 40, 90)[i % 3]
            lk = town_look(r, s=r.uniform(0.92, 1.0))
            kw = {}
            if i % 3 == 0:
                kw = dict(arm_l='carry', swing_l=0, hold_l=P.hold_bag(r.choice(HOT)))
            elif i % 3 == 1:
                kw = dict(arm_r='face', swing_r=0, hold_r=P.hold_cup(r.choice(HOT)))
            walker(sc, r, x, y, d, L, speed=r.uniform(95, 150), z=z, look=lk, **kw)
        # three who have stopped at the kerb side to watch: they wave as the chase comes by
        for i in range(3):
            x = (-900, 150, 1000)[i] + (200 if variant else 0)
            d = town_look(r, s=r.uniform(0.92, 1.0))
            d.update(CHEER[(i * 3 + seed) % len(CHEER)])
            d['bob'] = min(d.get('bob', 0), 7)
            P.figure(sc, x, -95, z=z, yaw=r.uniform(-20, 20), phase=r.random(), react=True, **d)
        if variant:
            dog(sc, -600, 60, 1, L, speed=120, z=z, body=WHITE)
            walker(sc, r, -680, 20, 1, L, speed=120, z=z, look=town_look(r, top=HOTPINK, s=0.96), arm_r='down', swing_r=0)
    return build


# ------------------------------------------------------------------------------------------ by the road: raised
def _steps(g, length, rows, z0, rise, depth, cols):
    """A stepped timber terrace: `rows` steps from z0 up, each `rise` higher and `depth` deep, the lowest in front (-Y)."""
    for k in range(rows):
        top = z0 + k * rise
        y = -((rows - 1) / 2.0) * depth + k * depth
        g.box(WOOD, (0, y, top / 2.0 - 20), (length, depth, top + 40))
        g.box(cols[k % len(cols)], (0, y - depth / 2.0 - 1.5, top - 16), (length + 4, 3, 32))
        g.box(WOOD_D, (0, y, top + 1.5), (length, depth - 6, 3))


def sun_deck(seed=31, variant=0):
    """A stepped sun terrace on the sand by the boardwalk, 9 m long: three rows of people who have turned from the sea
    to watch the chase. They are over the barrier's line and they move: the piece that says 'the town is watching'."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Lg, rows, z0, rise, depth = 900.0, 3, 105.0, 52.0, 88.0
        cols = (CORAL, TEAL, YEL) if variant == 0 else (HOTPINK, WHITE, SKY)
        _steps(g, Lg, rows, z0, rise, depth, cols)
        for sx in (-1, 1):                                                   # end posts and a pennant on each
            g.box(WHITE, (sx * (Lg / 2.0 + 8), depth, 190), (12, 12, 380))
            fl = sc.part(P.motion(swing=14.0, pivot=(sx * (Lg / 2.0 + 8), depth, 350.0), axis=P.UP, rate=0.9, phase=0.3 * sx))
            fl.box(cols[0] if sx < 0 else cols[2], (sx * (Lg / 2.0 + 8) + 46, depth, 350), (84, 5, 50))
        g.box(WHITE, (0, depth * 1.5 - 4, z0 + 2 * rise + 95), (Lg, 8, 8))       # the back rail
        for k in range(6):
            g.box(WHITE, (-Lg / 2.0 + 20 + k * (Lg - 40) / 5.0, depth * 1.5 - 4, z0 + 2 * rise + 48), (8, 8, 96))
        per = (6, 5, 5) if variant == 0 else (5, 6, 4)
        for k in range(rows):
            top = z0 + k * rise
            y = -depth + k * depth
            for i in range(per[k]):
                x = -Lg / 2.0 + (i + 0.5) * Lg / per[k] + r.uniform(-18, 18)
                d = beach_look(r)
                if (i + k) % 6 == 2:
                    d.update(build='kid', s=r.uniform(0.62, 0.7))
                act = dict(CHEER[(i * 3 + k * 5 + seed) % len(CHEER)])
                if k == 0 and i % 3 == 1:                                    # some of the front row sit on the step above
                    act = dict(legs='sit', seat=rise, arm_r='wave', swing_r=24, rate=1.1, bob=0)
                    d.update(act)
                    P.figure(sc, x, y + depth * 0.55, z=top, yaw=r.uniform(-10, 10), phase=r.random(), react=True, detail=1, **d)
                    continue
                d.update(act)
                P.figure(sc, x, y + r.uniform(-8, 12), z=top + 3, yaw=r.uniform(-16, 16), phase=r.random(), react=True, detail=(1 if k == 0 else 0), **d)
    return build


BALC_Z = 452.0          # the balcony's floor: over the awnings (their top edge is at 372 on the wall)


def balcony(seed=41, variant=0):
    """A balcony over a shop's awning, in front of its upper windows: people leaning out to watch, waving, filming.
    Drawn where the dressing's shop row has its wall (the row's own origin: the wall is at y = -150), so a Set with
    the row's offsets puts it on the wall. It stands on two posts of its own as well: on a bend the track moves a
    row back from the road by its own measure and this piece by another, and then it is a veranda before the shop."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        Lb, y0, y1 = 620.0, -262.0, -150.0
        yc = (y0 + y1) / 2.0
        col, trim = ((TEAL, WHITE), (CORAL, WHITE))[variant]
        g.box(WHITE, (0, yc, BALC_Z - 9), (Lb, y1 - y0, 18))
        g.box(col, (0, y0 + 3, BALC_Z - 24), (Lb, 6, 14))
        for sx in (-1, 1):                                                   # brackets down the wall, posts to the ground
            tilt_box(g, col, (sx * (Lb / 2.0 - 40), y0 + 10, BALC_Z - 14), (sx * (Lb / 2.0 - 40), y1, BALC_Z - 100), 10, 10)
            g.box(WHITE, (sx * (Lb / 2.0 - 8), y0 + 8, (BALC_Z - 18) / 2.0 - 20), (14, 14, BALC_Z - 18 + 40))
        g.box(col, (0, y0 + 4, BALC_Z + 96), (Lb, 8, 9))                         # the rail and its bars
        g.box(col, (0, y0 + 4, BALC_Z + 30), (Lb, 6, 7))
        for k in range(9):
            g.box(col, (-Lb / 2.0 + 4 + k * (Lb - 8) / 8.0, y0 + 4, BALC_Z + 50), (6, 6, 100))
        for sx in (-1, 1):
            g.box(col, (sx * (Lb / 2.0 - 3), yc, BALC_Z + 96), (6, y1 - y0, 9))
        # a towel over the rail, a window box
        tw = sc.part(P.motion(swing=7.0, pivot=(-170.0, y0, BALC_Z + 98.0), axis=0.0, rate=0.7))
        tw.box((HOTPINK, YEL)[variant], (-170, y0 + 1, BALC_Z + 62), (86, 4, 74))
        g.box(trim, (200, y0 + 12, BALC_Z + 106), (150, 20, 16))
        for k in range(4):
            g.sphere((RED, HOTPINK, YEL, RED)[k], (146 + k * 36, y0 + 12, BALC_Z + 122), 13, steps=(3, 5))
        n = 5
        acts = (CHEER[0], CHEER[2], CHEER[5], CHEER[3], CHEER[4]) if variant == 0 else (CHEER[3], CHEER[7], CHEER[1], CHEER[5], CHEER[2])
        for i in range(n):
            x = -Lb / 2.0 + (i + 0.5) * Lb / n + r.uniform(-14, 14)
            d = town_look(r, s=r.uniform(0.94, 1.02))
            if i == 2:
                d.update(build='kid', s=0.68)
            d.update(acts[i])
            d['bob'] = min(d.get('bob', 0), 6)
            P.figure(sc, x, yc + 14, z=BALC_Z, yaw=r.uniform(-18, 18), phase=r.random(), react=True, **d)
    return build


# ------------------------------------------------------------------------------------------ middle distance: the beach
def volley(seed=51):
    """A volleyball game on the sand, the court laid ALONG the road (the net across it), so both teams show side by
    side: the ball flies over the net in an arc, the players jump out of step, the people watching cheer."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        z = LIFT
        CL, CW = 1700.0, 860.0
        g.box(WHITE, (0, 0, z + 2), (CL, CW, 30))
        g.box('#ffe9b8', (0, 0, z + 3), (CL - 40, CW - 40, 32))
        for sy in (-1, 1):
            g.cyl(WOOD_D, (0, sy * (CW / 2.0 + 20), z - 30), 9, 300, steps=6)
        g.box(WHITE, (0, 0, z + 205), (5, CW + 40, 86))
        g.box(HOTPINK, (0, 0, z + 250), (7, CW + 40, 9))
        # the ball: a lob over the net and back (it swings about a point on the sand under the net)
        ball = sc.part(P.motion(swing=52.0, pivot=(0.0, 0.0, z - 60.0), axis=90.0, rate=0.42))
        ball.sphere(YEL, (0, 0, z + 500), 30, steps=(5, 8))
        ball.sphere(RED, (0, 0, z + 500), 31, scale=(1.0, 0.35, 1.0), steps=(4, 8))
        teams = ((RED, -1), ('#2ee6ff', 1))
        for col, sx in teams:
            for i in range(3):
                x = sx * (220 + (i % 2) * 330 + r.uniform(-30, 30))
                y = (-260, 20, 280)[i] + r.uniform(-30, 30)
                d = beach_look(r, top=col, wear='swim' if i % 2 else 'shorts', hat=None, s=r.uniform(1.0, 1.08))
                jump = i == 0
                P.figure(sc, x, y, z=z + 18, yaw=-90.0 * sx + r.uniform(-25, 25), arm_l='up' if jump else 'vee', arm_r='up' if jump else 'fwd',
                         swing_l=14, swing_r=-14, bob=46 if jump else 14, rate=0.42 if jump else 0.84, phase=(0.25 if sx > 0 else 0.75) if jump else r.random(),
                         legs='stand' if jump else 'wide', **d)
        # the referee on a high chair, and people along the road side of the court
        g.box(CORAL, (0, CW / 2.0 + 110, z + 110), (60, 60, 220))
        g.box(WHITE, (0, CW / 2.0 + 110, z + 224), (84, 84, 10))
        P.figure(sc, 0, CW / 2.0 + 110, z=z + 229 - 46, legs='sit', arm_r='up', swing_r=10, rate=0.42, **beach_look(r, top=WHITE, hat='cap', hat_col=NAVY))
        for i in range(6):
            x = -700 + i * 280 + r.uniform(-40, 40)
            d = beach_look(r)
            d.update(CHEER[(i * 3 + 1) % len(CHEER)])
            P.figure(sc, x, -CW / 2.0 - 110 + r.uniform(-20, 20), z=z, yaw=180 + r.uniform(-25, 25), phase=r.random(), detail=1, **d)
    return build


TIGHT = dict(grow=(20.0, 20.0, 30.0), grow_down=(20.0, 0.0, 0.0))     # a balcony's bounds: no further towards the road than it is built

JOBS = [
    ('SM_Strollers_A', strollers(11, 0)),
    ('SM_Strollers_B', strollers(12, 1)),
    ('SM_Shoppers_A', shoppers(21, 0)),
    ('SM_SunDeck_A', sun_deck(31, 0)),
    ('SM_SunDeck_B', sun_deck(32, 1)),
    ('SM_Balcony_A', balcony(41, 0), TIGHT),
    ('SM_Volley_A', volley(51)),
]


def jobs():
    import ft_people_bay_beach as B2
    return JOBS + B2.JOBS


def run(only=None):
    P.build_core()
    return P.export(DIR, jobs(), only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in jobs() if not only or j[0] in only]
