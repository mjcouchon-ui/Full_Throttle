"""FULL THROTTLE (Unreal) the population, level 0: Bayshore, pass 3, builder p3a: THE GAPS (/Game/People/Bayshore/SM_P3A_*).

The general cast of the town, for the places the first cast left empty (read off FTPeopleL0.inl and FTDressL0.inl):

  the boardwalk's ends   The dressing's beach edge (windbreaks, 30 m long) fills the middle of every 45 m slot right behind
                         the barrier, and the first cast's strollers walk the seaward lanes of the deck in some slots only.
                         The last 7 m of every slot's boardwalk, road side, had nobody: four pieces stand there, ON the
                         deck (a Set lifts them 0.48 m, as the strollers are), 6.4 m long and 0.9 m deep, all LOW.
                           SM_P3A_WalkRest_A   a bench of people, a family, someone on a cool box            (at rest)
                           SM_P3A_WalkRest_B   a busker and the people who stopped to listen                (at rest)
                           SM_P3A_WalkWork_A   a sweeper, a porter and his hand truck, a painter, a gardener (at work)
                           SM_P3A_WalkWork_B   an ice-cream cart, its seller and the queue                  (at work)
  the town's pavement    Between the kerb's cars (to 19.8 m) and the shoppers under the awnings (from 23.6 m), 15 m along
                         from a slot's middle (the film crew sets up in the middle): a low step of pale stone that people
                         have climbed to see over the barrier. LOW (the step is 30 cm, nobody on it is over 1.64 m).
                           SM_P3A_Pave_A       seven people on the step, cheering the chase
                           SM_P3A_Pave_B       a living statue, a photographer and a couple posing, onlookers
  the side streets       A side street runs back from the road every fourth slot, and nobody walked it. 22 m of its
                         pavement, turned to run AWAY from the road (the Set turns it 90 degrees; 37 to 59 m out): the
                         walkers travel along the street and never towards the carriageway of the main road.
                           SM_P3A_SideSt_A / _B / _C   walkers, a pram, joggers, dogs, a skater, people who stopped
  the open sand          36 to 50 m out on the bay side, in the districts and slots the first cast left bare (the Marina,
                         Hotel Row's odd slots, the Point, downtown's waterfront).
                           SM_P3A_Sand_A       a game of catch over a child in the middle, a dog running rings, a parasol
                           SM_P3A_Sand_B       an exercise class on mats, its instructor on a box, a banner
                           SM_P3A_Sand_C       an ice seller pushing a cart along the sand, children after it, rakers
                           SM_P3A_Sand_D       a sandcastle and the family building it

Every piece moves (arms, hops, walkers that travel, a ball in flight, a cart, pennants). No glow anywhere, nothing
blinks. Placed by the block C:\\ftwork\\p3\\L0a.inl (pasted into FTPeopleL0.inl by the level's integrator).

Run in the editor:  import ft_people_bay_p3a as K; K.run()        (or K.run(['SM_P3A_Sand_A']))
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/Bayshore'

WOOD, WOOD_D, WHITE, CREAM, SAND, DSAND = '#c9a06a', '#9a744a', '#fffaf2', '#fff1d6', '#f2dcae', '#d9b676'
CORAL, TEAL, YEL, HOTPINK, MINT, SKY, NAVY, RED, LAV = '#ff7a8a', '#19c5c0', '#ffc93c', '#ff4fb8', '#7dffb0', '#57c7ff', '#2b2350', '#ff2d3c', '#b9a4ff'
DARK, GREY, STEEL, LEAF, SILVER, LIME = '#22202a', '#8f9bb3', '#c9c4d6', '#3fae5a', '#c9c4d6', '#b8ff5a'
HOT = ('#ff4fb8', '#ff2d3c', '#ffc93c', '#2ee6ff', '#ff7a3c', '#7dffb0', '#8f6bff', '#fffaf2', '#b8ff5a', '#ff5fa2')
SWIM = ('#ff2d3c', '#2ee6ff', '#ffc93c', '#ff4fb8', '#2b2350', '#7dffb0', '#ff7a3c', '#8f6bff')
LIFT = 15.0            # the track sinks a piece 15 cm into the land: what stands on the land is drawn this much up
LOW_HAIR = ('short', 'short', 'long', 'pony', 'bald')      # no bun, no afro: they add to a head's height


# ------------------------------------------------------------------------------------------ looks and small helpers
def town_look(r, **fixed):
    d = P.look(r, tops=HOT, wear=r.choice(('shorts', 'trousers', 'skirt', 'dress', 'shorts')), sleeves='short')
    d['hair'] = r.choice(LOW_HAIR)
    hat = r.choice((None, None, 'cap', 'sun', None, 'cap'))
    if hat:
        d.update(hat=hat, hat_col=r.choice(HOT))
    d['s'] = r.uniform(0.92, 1.0)
    d.update(fixed)
    return d


def beach_look(r, **fixed):
    d = P.look(r, tops=HOT, bottoms=SWIM, wear=r.choice(('swim', 'shorts', 'shorts', 'swim', 'skirt')), sleeves=r.choice(('none', 'short', 'none')))
    hat = r.choice((None, None, 'cap', 'sun', None))
    if hat:
        d.update(hat=hat, hat_col=r.choice(HOT))
    d['s'] = r.uniform(0.92, 1.02)
    d.update(fixed)
    return d


def work_look(r, **fixed):
    """Someone at work on the front: a pale shirt, a lime band across the chest, a sun hat or a cap."""
    d = P.look(r, tops=(WHITE, SKY, CREAM), bottoms=(NAVY, '#3a5fa8', '#5a4a3c'), wear=r.choice(('shorts', 'trousers')), sleeves='short')
    d['hair'] = r.choice(LOW_HAIR)
    d.update(hat=r.choice(('sun', 'cap')), hat_col=r.choice((YEL, WHITE, TEAL)), band=LIME, s=r.uniform(0.93, 1.0))
    d.update(fixed)
    return d


def kid(r, look_fn=town_look, **fixed):
    d = look_fn(r, build='kid', s=r.uniform(0.62, 0.7))
    d.update(fixed)
    return d


def person(sc, x, y, lk, **kw):
    d = dict(lk)
    d.update(kw)
    return P.figure(sc, x, y, **d)


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=6, rate=1.6),
    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=6, rate=1.4),
    dict(arm_r='wave', swing_r=28, rate=1.3, bob=4),
    dict(arm_l='wave', arm_r='wave', swing_l=-24, swing_r=24, rate=1.5, bob=5),
    dict(arm_r='point', arm_l='hip', swing_r=10, axis_r='x', rate=0.7, bob=3),
    dict(arm_r='face', hold_r=P.hold_phone(), arm_l='down', rate=0.4, bob=-1.5),
    dict(arm_l='hold', arm_r='hold', swing_l=13, swing_r=-13, axis_l='z', axis_r='z', rate=2.6, bob=3),      # clapping
    dict(arm_r='up', arm_l='hip', swing_r=20, rate=1.7, bob=6),
)


def walker(sc, r, x, y, d, L, speed=130.0, z=0.0, rate=0.95, bob=3.0, look=None, **kw):
    """Someone walking the piece's X (d = +1 or -1) and coming round again."""
    v = dict(stride=26, swing_l=20, swing_r=-20, axis_l='x', axis_r='x', detail=1)
    v.update(kw)
    lk = dict(look or town_look(r))
    lk.update(v)
    return P.figure(sc, x, y, z=z, yaw=90.0 * d, rate=rate, bob=bob, phase=r.random(), travel=(L, speed * d), **lk)


def dog(sc, x, y, d, L, speed=130.0, z=0.0, body='#e8c26a', s=1.0, phase=0.0):
    P.quadruped(sc, x, y, z=z, yaw=90.0 * d, s=s, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=24,
                rate=2.2, phase=phase, travel=(L, speed * d), body=body, ears='#5a4a3c', snout=DARK)


def rider(x, y, z, L, speed):
    """The motion of a thing that travels the piece's X with the walkers (a cart, a pram, a board)."""
    return P.motion(pivot=(x, y, z), base=(x, y), travel=(float(L), speed / float(L)))


def pennant(sc, x, y, z, col, w=70.0, h=40.0, phase=0.0, d=1):
    """A pennant that swings about its pole (the pole itself is drawn by the caller, 8 cm or more)."""
    fl = sc.part(P.motion(swing=16.0, pivot=(x, y, z), axis=P.UP, rate=0.9, phase=phase))
    fl.box(col, (x + d * (w / 2.0 + 4.0), y, z), (w, 5, h))


def parasol(g, x, y, col, z=0.0, h=225.0, r=150.0, col2=None):
    g.cyl(WHITE, (x, y, z), 5, h, steps=6)
    g.cyl(col, (x, y, z + h - 10), r, 46, steps=8, top_r=8)
    if col2:
        g.cyl(col2, (x, y, z + h + 4), r * 0.5, 30, steps=8, top_r=6)


# ---- what a hand holds
def hold_long(col=WOOD_D, head=YEL, head_w=44.0):
    """A broom or a rake, held in both hands in front of the body, its head on the ground ahead (drawn on the right hand)."""
    def f(m, h, side):
        if side < 0:
            return
        a = (h[0] - 22, h[1] + 26, h[2] + 56)
        b = (h[0] - 22, h[1] - 46, max(h[2] - 78, 5.0))
        tilt_box(m, col, a, b, 4.5, 4.5)
        m.box(head, (b[0], b[1], b[2] + 1), (head_w, 9, 9))
    return f


def hold_roller(col=WOOD_D, head=CORAL):
    def f(m, h, side):
        tilt_box(m, col, (h[0], h[1] + 4, h[2] - 6), (h[0], h[1] - 26, h[2] + 34), 4.5, 4.5)
        m.box(head, (h[0], h[1] - 28, h[2] + 37), (28, 9, 9))
    return f


def hold_can(col=TEAL):
    def f(m, h, side):
        m.box(col, (h[0], h[1] - 4, h[2] - 10), (16, 26, 20))
        tilt_box(m, col, (h[0], h[1] - 14, h[2] - 8), (h[0], h[1] - 40, h[2] + 4), 5, 5)
    return f


def hold_spade(col=RED):
    def f(m, h, side):
        tilt_box(m, WOOD_D, (h[0], h[1], h[2] + 14), (h[0], h[1] - 12, h[2] - 34), 4.5, 4.5)
        m.box(col, (h[0], h[1] - 14, h[2] - 42), (16, 5, 18))
    return f


def hold_cone(scoop=HOTPINK):
    def f(m, h, side):
        m.cyl(DSAND, (h[0], h[1] - 2, h[2] + 2), 3.5, 15, steps=6, top_r=7.0)
        m.sphere(scoop, (h[0], h[1] - 2, h[2] + 22), 8.5, steps=(3, 6))
    return f


def hold_guitar(body=WOOD, neck=WOOD_D):
    def f(m, h, side):
        if side < 0:
            return
        m.box(body, (h[0] - 16, h[1] - 9, h[2] - 8), (42, 10, 34))
        m.box(DARK, (h[0] - 16, h[1] - 14.5, h[2] - 8), (12, 1.5, 12))
        tilt_box(m, neck, (h[0] - 34, h[1] - 9, h[2] - 2), (h[0] - 86, h[1] - 9, h[2] + 20), 5, 6)
    return f


# ------------------------------------------------------------------------------------------ the boardwalk's ends
# Drawn from z = 0 = the deck (a Set lifts the piece 0.48 m). 6.4 m along X; everybody stands between y = -35 and
# +30 (the banner mast of the boardwalk is 1.1 m nearer the road, the strollers' first lane 0.6 m further out).
def walk_rest(variant=0):
    def build(sc):
        r = random.Random(101 + variant * 7)
        g = sc.still()
        if variant == 0:
            bx = -195.0
            g.box(TEAL, (bx, 22, 43), (236, 46, 7))                      # a bench: its sitters face the road
            g.box(TEAL, (bx, 46, 76), (236, 7, 44))
            for sx in (-1, 1):
                g.box(WHITE, (bx + sx * 104, 22, 20), (8, 40, 40))
                g.box(WHITE, (bx + sx * 104, 46, 50), (8, 8, 100))
            g.box(SKY, (262, 8, 20), (72, 44, 40))                       # a cool box
            g.box(WHITE, (262, 8, 42), (76, 48, 5))
            g.box(YEL, (120, 34, 14), (40, 30, 28))                      # a beach bag
            person(sc, -272, 18, town_look(r, top=YEL), legs='sit', arm_r='face', hold_r=P.hold_cup(HOTPINK), arm_l='down', rate=0.35, bob=-1.0,
                   phase=r.random(), detail=1)
            person(sc, -196, 18, town_look(r, top=HOTPINK, hat='sun', hat_col=WHITE), legs='sit', arm_r='wave', swing_r=26, arm_l='down', rate=1.2,
                   phase=r.random(), react=True, detail=1)
            person(sc, -122, 18, kid(r, top=RED), z=16.0, legs='sit', arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.6, phase=r.random(),
                   react=True, detail=1)
            person(sc, -20, -8, town_look(r, top='#2ee6ff'), yaw=r.uniform(-12, 12), phase=r.random(), react=True, detail=1, **CHEER[0])
            person(sc, 52, -26, kid(r, top=LIME), yaw=8, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=8, rate=1.7, phase=r.random(), react=True, detail=1)
            person(sc, 128, -4, town_look(r, top='#ff7a3c'), yaw=-10, phase=r.random(), detail=1, **CHEER[5])
            person(sc, 262, 6, town_look(r, top='#8f6bff', hat='cap', hat_col=YEL), yaw=-14, legs='sit', seat=43.0, arm_r='point', swing_r=10, axis_r='x',
                   arm_l='hip', rate=0.7, phase=r.random(), react=True, detail=1)
        else:
            g.box(DARK, (-304, 22, 23), (46, 38, 46))                    # the busker's amplifier
            g.box(GREY, (-304, 2.5, 25), (34, 1.5, 30))
            g.box('#5a4a3c', (-214, -34, 4), (112, 30, 8))               # the open guitar case, and what has been thrown into it
            g.box(YEL, (-226, -34, 9), (30, 12, 2.5))
            g.box(HOTPINK, (-190, -34, 9), (16, 12, 2.5))
            person(sc, -226, 16, town_look(r, top=RED, bottom=NAVY, wear='trousers', hat='cowboy', hat_col=CREAM, s=0.98), yaw=18, arm_l='hold', arm_r='hold',
                   hold_r=hold_guitar(), rock=5, rate=1.1, phase=0.0, detail=2)
            person(sc, -96, 4, town_look(r, top=YEL), yaw=-24, phase=r.random(), detail=1, **CHEER[6])
            person(sc, -20, -22, kid(r, top=HOTPINK), yaw=-20, arm_l='vee', arm_r='vee', turn=24, bob=8, rate=1.1, phase=0.25, detail=1)
            person(sc, 58, 10, town_look(r, top='#2ee6ff', wear='dress'), yaw=-16, arm_l='out', arm_r='out', turn=20, bob=5, rate=1.1, phase=0.5, detail=1)
            person(sc, 138, -12, town_look(r, top=MINT), yaw=-12, arm_l='hip', arm_r='hip', rock=7, rate=0.55, phase=0.1, detail=1)
            person(sc, 196, -6, town_look(r, top='#ff7a3c', hat='sun', hat_col=HOTPINK), yaw=-8, arm_l='hip', arm_r='down', rock=7, rate=0.55, phase=0.1, detail=1)
            person(sc, 280, 6, town_look(r, top='#8f6bff'), yaw=-30, phase=r.random(), detail=1, **CHEER[5])
    return build


def walk_work(variant=0):
    def build(sc):
        r = random.Random(131 + variant * 7)
        g = sc.still()
        if variant == 0:
            g.cyl(TEAL, (-208, 26, 0), 20, 62, steps=8)                  # a litter bin
            g.cyl(DARK, (-208, 26, 62), 21, 5, steps=8)
            g.box(WHITE, (196, 40, 96), (170, 7, 8))                     # a length of rail being painted
            g.box(CORAL, (150, 40, 60), (78, 7, 8))
            g.box(WHITE, (240, 40, 60), (82, 7, 8))
            for sx in (-1, 1):
                g.box(WHITE, (196 + sx * 82, 40, 50), (8, 8, 100))
            g.cyl(YEL, (120, 6, 0), 11, 20, steps=8)                     # the paint
            g.box(CREAM, (306, 30, 26), (56, 50, 52))                    # a tub of flowers
            g.sphere(LEAF, (306, 30, 70), 34, scale=(1, 1, 0.7), steps=(4, 6))
            for k, c in enumerate((HOTPINK, YEL, CORAL)):
                g.sphere(c, (290 + k * 16, 22 + (k % 2) * 14, 92), 9, steps=(3, 5))
            # the porter and his hand truck slide a metre each way (a swing of one degree about a pivot 60 m under the deck)
            slide = dict(swing=1.0, pivot=(-40.0, 0.0, -6000.0), axis=90.0)
            t = sc.part(P.motion(rate=0.12, phase=0.1, **slide))
            t.box(STEEL, (-76, 0, 8), (48, 44, 6))
            t.box(STEEL, (-50, 0, 62), (6, 40, 112))
            t.box(DARK, (-54, 0, 12), (20, 52, 20))
            for k, c in enumerate((CORAL, YEL, SKY)):
                t.box(c, (-78, 0, 27 + k * 32), (44, 40, 30))
            person(sc, -8, 0, work_look(r), yaw=-90, arm_l='push', arm_r='push', rate=0.12, phase=0.1, sway=slide, detail=1)
            person(sc, -272, 0, work_look(r, hat='sun', hat_col=YEL), yaw=28, arm_l='carry', arm_r='carry', hold_r=hold_long(head=YEL),
                   swing_l=12, swing_r=12, axis_l='z', axis_r='z', rate=0.8, phase=r.random(), detail=1)
            person(sc, -140, -6, work_look(r, top=NAVY, band=None, hat='cap', hat_col=WHITE), yaw=-30, arm_r='point', swing_r=10, axis_r='x', arm_l='hip',
                   rate=0.6, phase=r.random(), react=True, detail=1)
            person(sc, 190, -2, work_look(r), yaw=180, arm_r='fwd', hold_r=hold_roller(), swing_r=14, axis_r='x', arm_l='hip', rate=1.0, phase=r.random(), detail=1)
            person(sc, 300, -22, work_look(r, hat='sun', hat_col=WHITE), yaw=180, arm_r='fwd', hold_r=hold_can(), swing_r=9, axis_r='x', arm_l='down',
                   rate=0.5, phase=r.random(), detail=1)
        else:
            cx = -176.0
            g.box(WHITE, (cx, 4, 62), (96, 66, 70))                      # the ice-cream cart: a striped box on wheels, a great cone on top
            for k in range(4):
                g.box(HOTPINK if k % 2 else MINT, (cx - 36 + k * 24, -30, 62), (24, 3, 70))
            g.box(TEAL, (cx, 4, 100), (104, 74, 7))
            for sx in (-1, 1):
                g.box(DARK, (cx + sx * 34, 4, 16), (26, 74, 26))
            g.cyl(DSAND, (cx, 4, 103), 5, 44, steps=8, top_r=22)
            g.sphere(HOTPINK, (cx, 4, 160), 23, steps=(4, 8))
            g.box(WHITE, (cx + 44, 34, 128), (8, 8, 50))                 # a pennant on the cart
            pennant(sc, cx + 44, 34, 142, YEL, w=44, h=22)
            person(sc, -262, 2, work_look(r, top=WHITE, band=HOTPINK, hat='cap', hat_col=HOTPINK), yaw=90, arm_r='fwd', hold_r=hold_cone(MINT), swing_r=8,
                   axis_r='x', arm_l='hip', rate=0.7, phase=0.0, detail=1)
            person(sc, -88, 2, town_look(r, top=YEL), yaw=-90, arm_r='fwd', swing_r=8, axis_r='x', arm_l='down', rate=0.7, phase=0.5, detail=1)
            person(sc, -10, -8, kid(r, top=RED), yaw=-90, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, bob=8, rate=1.8, phase=r.random(), detail=1)
            person(sc, 58, 6, town_look(r, top='#2ee6ff', hat='sun', hat_col=YEL), yaw=-90, arm_l='hip', arm_r='down', rock=4, rate=0.4, phase=r.random(), detail=1)
            person(sc, 134, -4, town_look(r, top='#8f6bff'), yaw=-72, arm_r='face', hold_r=P.hold_phone(), arm_l='down', rate=0.3, bob=-1.0,
                   phase=r.random(), detail=1)
            person(sc, 212, 4, kid(r, top=LIME), yaw=-40, arm_r='face', hold_r=hold_cone(YEL), arm_l='down', rate=0.5, bob=3, phase=r.random(), detail=1)
            person(sc, 286, -2, town_look(r, top='#ff7a3c', hat='cap', hat_col=WHITE), yaw=-8, phase=r.random(), react=True, detail=1, **CHEER[2])
    return build


# ------------------------------------------------------------------------------------------ the town's pavement
STEP = 30.0            # the stone step's top as built (15 cm over the pavement once the track has sunk the piece)


def _step(g, c1, c2):
    g.box(CREAM, (0, 0, STEP / 2.0 - 10), (640, 140, STEP + 20))
    g.box(c1, (0, -71, STEP - 8), (640, 3, 10))
    g.box(c2, (0, -71, STEP - 21), (640, 3, 6))
    for sx in (-1, 1):                                                   # a tub of flowers at each end
        g.box(WHITE, (sx * 296, 30, STEP + 22), (44, 60, 44))
        g.sphere(LEAF, (sx * 296, 30, STEP + 56), 30, scale=(1, 1, 0.7), steps=(4, 6))
        g.sphere(HOTPINK if sx < 0 else YEL, (sx * 296, 22, STEP + 76), 9, steps=(3, 5))


def pave(variant=0):
    def build(sc):
        r = random.Random(161 + variant * 7)
        g = sc.still()
        if variant == 0:
            _step(g, CORAL, TEAL)
            xs = (-236, -160, -84, -10, 66, 144, 222)
            for i, x in enumerate(xs):
                small = i in (2, 5)
                lk = kid(r, top=HOT[(i * 3) % len(HOT)]) if small else town_look(r, top=HOT[(i * 3 + 1) % len(HOT)], s=r.uniform(0.88, 0.95))
                act = dict(CHEER[(i * 3 + 1) % len(CHEER)])
                person(sc, x, (-18 if i % 2 else 14), lk, z=STEP, yaw=r.uniform(-14, 14), phase=r.random(), react=True, detail=1, **act)
        else:
            _step(g, HOTPINK, SKY)
            g.cyl(DARK, (-214, -40, STEP), 13, 9, steps=8, top_r=15)       # the statue's hat for coins
            g.box(YEL, (-214, -40, STEP + 9), (12, 12, 2.5))
            # the living statue: silver from head to foot, turning slowly
            person(sc, -214, 6, dict(skin=SILVER, top=SILVER, bottom=SILVER, shoes=SILVER, hair='short', hair_col=SILVER, hat='cap', hat_col=SILVER,
                                     build='slim', sleeves='long', wear='trousers', s=0.93),
                   z=STEP, arm_l='hip', arm_r='point', turn=30, rate=0.12, phase=0.0, detail=1)
            person(sc, -120, -6, kid(r, top=RED), z=STEP, yaw=70, arm_r='point', swing_r=12, axis_r='x', arm_l='down', bob=6, rate=1.4, phase=r.random(), detail=1)
            person(sc, -50, 12, town_look(r, top=YEL, s=0.93), z=STEP, yaw=60, phase=r.random(), detail=1, **CHEER[6])
            # the photographer at a tripod, and the couple posing for it with the chase behind them
            for a in (90, 210, 330):
                rod(g, STEEL, (40, 0, STEP + 112), (40 + math.cos(math.radians(a)) * 34, math.sin(math.radians(a)) * 34, STEP), 3.0, steps=5)
            g.box(DARK, (40, 0, STEP + 120), (20, 30, 18))
            g.cyl(GREY, (50, 0, STEP + 120), 6, 12, rot=(-90, 0, 0), steps=6)
            person(sc, 2, 6, town_look(r, top='#2ee6ff', hat='cap', hat_col=DARK, s=0.92), z=STEP, yaw=90, arm_l='hold', arm_r='hold', rate=0.3, bob=-1.0,
                   phase=r.random(), detail=1)
            person(sc, 150, -4, town_look(r, top=HOTPINK, wear='dress', s=0.92), z=STEP, yaw=-80, arm_l='hip', arm_r='vee', swing_r=12, rate=0.8,
                   phase=0.2, detail=1)
            person(sc, 206, 4, town_look(r, top='#ff7a3c', s=0.95), z=STEP, yaw=-80, arm_l='wave', swing_l=-22, arm_r='hip', rate=0.8, phase=0.7,
                   react=True, detail=1)
    return build


# ------------------------------------------------------------------------------------------ the side streets
def side_street(variant=0):
    """22 m of a side street's pavement. The Set turns it 90 degrees, so its X runs away from the road: the walkers go
    up the street and come back down it, 37 to 59 m from the centre line."""
    def build(sc):
        r = random.Random(191 + variant * 7)
        L, z = 2200.0, LIFT
        lanes = (-42.0, 2.0, 46.0)
        if variant == 0:
            walker(sc, r, -900, lanes[0], 1, L, z=z, look=town_look(r, top=HOTPINK, hat='sun', hat_col=WHITE, wear='dress'))
            walker(sc, r, -830, lanes[1], 1, L, z=z, look=town_look(r, top='#2ee6ff'))
            walker(sc, r, -200, lanes[2], -1, L, z=z, speed=110, arm_l='carry', swing_l=0, hold_l=P.hold_bag(YEL), look=town_look(r, top='#ff7a3c'))
            walker(sc, r, 380, lanes[0], -1, L, z=z, speed=150, look=town_look(r, top=LIME, hat='cap', hat_col=RED), arm_r='down', swing_r=0)
            dog(sc, 320, lanes[1], -1, L, speed=150, z=z, phase=0.3)
            walker(sc, r, 760, lanes[2], 1, L, z=z, speed=120, arm_r='face', swing_r=0, hold_r=P.hold_cup(HOTPINK), look=town_look(r, top='#8f6bff'))
            person(sc, 60, lanes[2] + 10, town_look(r, top=YEL), z=z, yaw=40, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.6, phase=r.random(), detail=1)
            person(sc, 130, lanes[2] - 6, town_look(r, top=RED, hat='sun', hat_col=YEL), z=z, yaw=-50, arm_l='hip', arm_r='down', rock=5, rate=0.4,
                   phase=r.random(), detail=1)
        elif variant == 1:
            # a pram and the parent pushing it; two joggers; an older couple, slowly; a porter with a box
            px, py, sp = -700.0, lanes[1], 95.0
            pr = sc.part(rider(px + 70, py, z, L, sp))
            pr.box(TEAL, (px + 74, py, z + 62), (62, 44, 34))
            pr.box(WHITE, (px + 92, py, z + 88), (30, 46, 22))
            pr.box(STEEL, (px + 38, py, z + 86), (6, 40, 6))
            for sx in (-1, 1):
                pr.box(DARK, (px + 74 + sx * 24, py, z + 16), (22, 50, 22))
                pr.box(STEEL, (px + 74 + sx * 20, py, z + 38), (5, 36, 24))
            walker(sc, r, px, py, 1, L, z=z, speed=sp, arm_l='push', arm_r='push', swing_l=0, swing_r=0, look=town_look(r, top=CORAL, hat='sun', hat_col=WHITE))
            for k in range(2):
                walker(sc, r, 300 + k * 100, lanes[0] + k * 44, -1, L, z=z, speed=330, rate=1.7, bob=7, stride=38, swing_l=34, swing_r=-34,
                       look=town_look(r, wear='shorts', sleeves='none', top=(LIME, HOTPINK)[k], hat=(None, 'cap')[k], hat_col=YEL))
            for k in range(2):
                walker(sc, r, -100 + k * 20, lanes[2] - k * 44, 1, L, z=z, speed=70, rate=0.7, stride=18, swing_l=10, swing_r=-10,
                       look=town_look(r, top=(SKY, YEL)[k], hair_col='#f4f1ea', hat=('sun', 'cap')[k], hat_col=(HOTPINK, NAVY)[k], wear=('skirt', 'trousers')[k]))
            walker(sc, r, 800, lanes[2], -1, L, z=z, speed=140, arm_l='carry', arm_r='carry', swing_l=0, swing_r=0, hold_r=P.hold_box(col='#c9a070'),
                   look=work_look(r))
            person(sc, -420, lanes[0] - 8, kid(r, top=RED), z=z, yaw=-90, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=9, rate=1.7,
                   phase=r.random(), detail=1)
        else:
            # a skater; a family in a row; a man and two dogs; someone on the phone who has stopped
            sx0, sy0, sp = 600.0, lanes[0], 400.0
            lk = town_look(r, wear='shorts', hat='cap', hat_col=HOTPINK, top=YEL, s=0.95)
            lk.update(legs='wide', arm_l='out', arm_r='out', rock=7, rate=0.5, phase=r.random(), detail=1)
            P.figure(sc, sx0, sy0, z=z + 12, yaw=-90.0 + 62.0, travel=(L, -sp), **lk)
            b = sc.part(rider(sx0, sy0, z, L, -sp))
            b.box(HOTPINK, (sx0, sy0, z + 9), (84, 24, 5))
            for dx in (-28, 28):
                b.box(DARK, (sx0 + dx, sy0, z + 4), (12, 26, 8))
            for k in range(3):
                walker(sc, r, -800 + k * 70, lanes[0] + k * 44, 1, L, z=z, speed=105, rate=(0.9, 1.4, 0.9)[k],
                       look=town_look(r, build=('avg', 'kid', 'slim')[k], s=(0.98, 0.66, 0.95)[k], top=(RED, LIME, '#2ee6ff')[k],
                                      hat=('cap', 'sun', 'sun')[k], hat_col=(WHITE, HOTPINK, '#ff7a3c')[k]))
            walker(sc, r, 150, lanes[1], -1, L, z=z, speed=135, look=town_look(r, top='#ff7a3c', hat='cap', hat_col=NAVY), arm_l='down', swing_l=0)
            dog(sc, 80, lanes[1] + 40, -1, L, speed=135, z=z, body='#5a4a3c', phase=0.1)
            dog(sc, 50, lanes[1] - 38, -1, L, speed=135, z=z, body=WHITE, s=0.8, phase=0.6)
            person(sc, -260, lanes[2] + 8, town_look(r, top='#8f6bff'), z=z, yaw=-60, arm_r='face', hold_r=P.hold_phone(), arm_l='hip', rock=4, rate=0.35,
                   phase=r.random(), detail=1)
    return build


# ------------------------------------------------------------------------------------------ the open sand
def sand_catch(sc):
    """A game of catch along the beach: the ball flies an arc between two players (it swings about a point under the
    sand), a child jumps for it in the middle, a dog runs rings round them all; a family under a parasol behind."""
    r = random.Random(221)
    z = LIFT
    g = sc.still()
    parasol(g, -150, 350, HOTPINK, z=z, col2=WHITE)
    g.box(YEL, (-150, 340, z + 2), (230, 170, 4))
    g.box(WHITE, (-150, 340, z + 3), (230, 40, 4))
    g.box(TEAL, (-40, 400, z + 16), (46, 34, 32))
    ball = sc.part(P.motion(swing=28.0, pivot=(0.0, 0.0, z - 385.0), axis=90.0, rate=0.35, phase=0.0))
    ball.sphere(RED, (0, 0, z + 245), 28, steps=(4, 8))
    ball.box(WHITE, (0, 0, z + 245), (58, 58, 9))
    person(sc, -356, 0, beach_look(r, top=YEL), z=z, yaw=90, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, axis_l='x', axis_r='x', bob=7, rate=0.35,
           phase=0.25, detail=1)
    person(sc, 356, 0, beach_look(r, top='#2ee6ff'), z=z, yaw=-90, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, axis_l='x', axis_r='x', bob=7, rate=0.35,
           phase=0.75, detail=1)
    person(sc, 0, 46, kid(r, beach_look, top=HOTPINK), z=z, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, bob=14, rate=0.7, phase=0.5, detail=1)
    P.quadruped(sc, 236, 0, z=z, yaw=180, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=26, rate=2.4,
                orbit=(0.0, 0.0, 0.13), body='#e8c26a', ears='#5a4a3c', snout=DARK)
    person(sc, -206, 336, beach_look(r, top=CORAL, hat='sun', hat_col=WHITE), z=z, legs='sit', seat=12.0, arm_r='wave', swing_r=22, arm_l='down', rate=1.1,
           phase=r.random(), react=True, detail=1)
    person(sc, -100, 344, beach_look(r, top=MINT), z=z, yaw=10, legs='sit', seat=12.0, arm_r='face', hold_r=P.hold_cup(YEL), arm_l='down', rate=0.3, bob=-1.0,
           phase=r.random(), detail=1)
    person(sc, 130, 318, beach_look(r, top='#8f6bff'), z=z, yaw=-10, phase=r.random(), react=True, detail=1, **CHEER[5])
    person(sc, 226, 330, kid(r, beach_look, top=LIME), z=z, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=10, rate=1.6, phase=r.random(),
           react=True, detail=1)


def sand_class(sc):
    """An exercise class on the sand: eight on their mats, two rows out of step with each other, the instructor on a
    box with her back to the road, a banner and two pennants behind."""
    r = random.Random(241)
    z = LIFT
    g = sc.still()
    mats = (HOTPINK, TEAL, YEL, CORAL, MINT, LAV, SKY, '#ff7a3c')
    spots = []
    for row in range(2):
        for k in range(4):
            x, y = -330 + k * 220 + row * 60, 40 + row * 210
            g.box(mats[row * 4 + k], (x, y, z + 2), (80, 170, 4))
            spots.append((x, y, row))
    g.box(WHITE, (0, -230, z + 20), (130, 110, 40))                      # the instructor's box
    g.box(HOTPINK, (0, -286, z + 20), (130, 3, 14))
    g.box(DARK, (-190, -230, z + 30), (50, 44, 60))                      # the music
    g.box(GREY, (-190, -253, z + 34), (36, 2, 40))
    for sx in (-1, 1):                                                   # the banner behind the class: stripes, no words
        g.box(WHITE, (sx * 300, 420, z + 150), (9, 9, 300))
        pennant(sc, sx * 300, 420, z + 286, YEL if sx < 0 else MINT, w=60, h=30, phase=0.2 * sx, d=sx)
    g = sc.still()
    g.box(HOTPINK, (0, 420, z + 215), (592, 6, 70))
    g.box(WHITE, (0, 416.5, z + 215), (592, 1.5, 22))
    g.box(TEAL, (0, 416.5, z + 240), (592, 1.5, 8))
    for i, (x, y, row) in enumerate(spots):
        lk = beach_look(r, top=HOT[(i * 3 + row) % len(HOT)], wear=r.choice(('shorts', 'swim', 'shorts')))
        person(sc, x, y, lk, z=z, arm_l='out', arm_r='out', swing_l=42, swing_r=-42, bob=7, rate=0.6, phase=0.5 * row, detail=0)
    person(sc, 0, -230, beach_look(r, top=LIME, bottom=NAVY, wear='shorts', hat='cap', hat_col=HOTPINK, s=0.97), z=z + 40, yaw=180, arm_l='out', arm_r='out',
           swing_l=42, swing_r=-42, bob=7, rate=0.6, phase=0.0, detail=1)
    person(sc, 400, -120, kid(r, beach_look, top=RED), z=z, yaw=-30, arm_l='out', arm_r='out', swing_l=42, swing_r=-42, bob=9, rate=0.6, phase=0.3, detail=1)


def sand_work(sc):
    """The beach at work: an ice seller pushes a cart along the sand under its parasol (it travels 14 m and comes round
    again), two children run after it; two beach hands rake the sand, a third picks litter by a handcart of sacks."""
    r = random.Random(261)
    z = LIFT
    L, sp, cy, cx = 1400.0, 55.0, -170.0, -100.0
    g = sc.still()
    for k in range(4):                                                   # raked sand: broad dark stripes, flat
        g.box(DSAND, (-240 + k * 10, 130 + k * 46, z + 1.5), (420, 22, 3))
    g.box(TEAL, (330, 250, z + 30), (120, 70, 44))                       # a handcart of sacks
    for sx in (-1, 1):
        g.box(DARK, (330 + sx * 40, 250, z + 14), (24, 80, 24))
    for k, c in enumerate((WHITE, CREAM, WHITE)):
        g.sphere(c, (298 + k * 32, 250, z + 62), 22, scale=(1, 1, 0.8), steps=(3, 6))
    g.box(RED, (-470, 60, z + 60), (9, 9, 120))                          # a marker post and its pennant
    pennant(sc, -470, 60, z + 106, RED, w=50, h=26)
    cart = sc.part(rider(cx, cy, z, L, sp))
    cart.box(WHITE, (cx, cy, z + 56), (120, 70, 66))
    for k in range(5):
        cart.box(HOTPINK if k % 2 else SKY, (cx - 48 + k * 24, cy - 36, z + 56), (24, 3, 66))
    for sx in (-1, 1):
        cart.box(DARK, (cx + sx * 42, cy, z + 14), (28, 78, 28))
    cart.box(WHITE, (cx, cy, z + 160), (9, 9, 150))
    cart.cyl(YEL, (cx, cy, z + 226), 96, 34, steps=8, top_r=8)
    cart.cyl(HOTPINK, (cx, cy, z + 238), 50, 26, steps=8, top_r=6)
    walker(sc, r, cx - 96, cy, 1, L, z=z, speed=sp, rate=0.6, stride=18, arm_l='push', arm_r='push', swing_l=0, swing_r=0,
           look=work_look(r, top=WHITE, band=HOTPINK, hat='cap', hat_col=HOTPINK))
    walker(sc, r, cx - 230, cy + 30, 1, L, z=z, speed=sp, rate=1.7, bob=8, stride=30, arm_l='vee', arm_r='vee', swing_l=10, swing_r=-10, axis_l='y', axis_r='y',
           look=kid(r, beach_look, top=RED))
    walker(sc, r, cx - 300, cy - 24, 1, L, z=z, speed=sp, rate=1.7, bob=8, stride=30, arm_l='down', arm_r='up', swing_l=20, swing_r=18, axis_r='y',
           look=kid(r, beach_look, top=LIME))
    person(sc, -250, 190, work_look(r), z=z, yaw=20, arm_l='carry', arm_r='carry', hold_r=hold_long(head=TEAL, head_w=60), swing_l=12, swing_r=12,
           axis_l='x', axis_r='x', rate=0.7, phase=0.0, detail=1)
    person(sc, 40, 300, work_look(r, hat='sun', hat_col=YEL), z=z, yaw=-25, arm_l='carry', arm_r='carry', hold_r=hold_long(head=TEAL, head_w=60),
           swing_l=12, swing_r=12, axis_l='x', axis_r='x', rate=0.7, phase=0.5, detail=1)
    person(sc, 420, 150, work_look(r), z=z, yaw=-40, arm_l='carry', hold_l=P.hold_bag(DARK), arm_r='fwd', hold_r=hold_roller(col=STEEL, head=DARK),
           swing_r=12, axis_r='x', rate=0.6, phase=r.random(), detail=1)
    person(sc, 200, 60, beach_look(r, top='#8f6bff'), z=z, yaw=-20, phase=r.random(), react=True, detail=1, **CHEER[2])


def sand_castle(sc):
    """A sandcastle as tall as its builders: three children digging, one on top of the world, a parent under a parasol,
    someone taking its picture. Pennants on the towers."""
    r = random.Random(281)
    z = LIFT
    g = sc.still()
    g.cyl(DSAND, (0, 150, z - 10), 250, 40, steps=10, top_r=210)         # the mound
    g.box(DSAND, (0, 150, z + 55), (300, 220, 60))
    for k in range(5):                                                   # the wall's teeth, road side
        g.box(DSAND, (-120 + k * 60, 42, z + 96), (30, 14, 22))
    g.cyl(DSAND, (0, 170, z + 85), 70, 110, steps=8, top_r=56)           # the keep
    g.cyl(CORAL, (0, 170, z + 195), 64, 60, steps=8, top_r=4)
    towers = ((-150, 40), (150, 40), (-150, 260), (150, 260))
    for (x, y) in towers:
        g.cyl(DSAND, (x, y, z + 20), 38, 120, steps=8, top_r=32)
        g.cyl(TEAL, (x, y, z + 140), 38, 44, steps=8, top_r=4)
        g.box(WHITE, (x, y, z + 205), (8, 8, 60))
    for k, (x, y) in enumerate(towers):
        pennant(sc, x, y, z + 222, HOT[k], w=46, h=24, phase=0.2 * k)
    g = sc.still()
    g.box(DARK, (0, 44, z + 52), (56, 6, 70))                            # the gate
    parasol(g, -400, 150, YEL, z=z, col2=HOTPINK)
    g.box(SKY, (-400, 130, z + 2), (200, 150, 4))
    for k, c in enumerate((RED, YEL, HOTPINK)):                          # buckets
        g.cyl(c, (-250 + k * 250, -90 - (k % 2) * 30, z), 15, 26, steps=8, top_r=19)
    person(sc, -210, -40, kid(r, beach_look, top=RED), z=z, yaw=150, arm_r='fwd', hold_r=hold_spade(RED), swing_r=22, axis_r='x', arm_l='down', rate=1.1,
           phase=r.random(), detail=1)
    person(sc, 30, -50, kid(r, beach_look, top='#2ee6ff'), z=z, yaw=180, arm_r='fwd', hold_r=hold_spade(YEL), swing_r=22, axis_r='x', arm_l='hip', rate=0.9,
           phase=r.random(), detail=1)
    person(sc, 250, 10, beach_look(r, top=LIME, s=0.94), z=z, yaw=120, arm_l='carry', arm_r='carry', hold_r=hold_long(col=WOOD_D, head=RED, head_w=22),
           swing_l=12, swing_r=12, axis_l='x', axis_r='x', rate=0.8, phase=r.random(), detail=1)
    person(sc, 0, 300, kid(r, beach_look, top=HOTPINK), z=z + 115, arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=10, rate=1.5, phase=r.random(),
           react=True, detail=1)
    person(sc, -420, 126, beach_look(r, top=CORAL, hat='sun', hat_col=WHITE), z=z, yaw=30, legs='sit', seat=12.0, arm_r='wave', swing_r=20, arm_l='down',
           rate=1.0, phase=r.random(), react=True, detail=1)
    person(sc, 380, -120, beach_look(r, top='#ff7a3c', hat='cap', hat_col=NAVY), z=z, yaw=150, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rate=0.3,
           bob=-1.5, phase=r.random(), detail=1)
    person(sc, 330, 190, beach_look(r, top=YEL), z=z, yaw=-60, phase=r.random(), react=True, detail=1, **CHEER[3])


LOW = dict(grow=(60.0, 60.0, 24.0))                                       # by the road: small hops only, small bounds
JOBS = [
    ('SM_P3A_WalkRest_A', walk_rest(0), LOW),
    ('SM_P3A_WalkRest_B', walk_rest(1), LOW),
    ('SM_P3A_WalkWork_A', walk_work(0), dict(grow=(130.0, 60.0, 24.0), grow_down=(130.0, 60.0, 0.0))),
    ('SM_P3A_WalkWork_B', walk_work(1), LOW),
    ('SM_P3A_Pave_A', pave(0), LOW),
    ('SM_P3A_Pave_B', pave(1), LOW),
    ('SM_P3A_SideSt_A', side_street(0)),
    ('SM_P3A_SideSt_B', side_street(1)),
    ('SM_P3A_SideSt_C', side_street(2)),
    ('SM_P3A_Sand_A', sand_catch),
    ('SM_P3A_Sand_B', sand_class),
    ('SM_P3A_Sand_C', sand_work),
    ('SM_P3A_Sand_D', sand_castle),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
