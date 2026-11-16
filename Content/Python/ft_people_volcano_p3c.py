"""FULL THROTTLE (Unreal) the population of level 3, the Volcano: pass 3, builder p3c, THE MOVERS (/Game/People/Volcano/SM_P3C_*).

Everything here TRAVELS, along the road (the mesh's X) or round a point of its own, and never towards the road:
  NEAR, on a narrow catwalk 1.8 m up (the land lies 0.75 m under the road behind a 1.6 m barrier), written at 25 m:
    SM_P3C_Runners_A / _B   villagers running for the assembly point with what they could grab, a dog, a goat
    SM_P3C_Cyclists         couriers on bicycles, a crate on every rack
    SM_P3C_HoseRun          the fire crew at the double, hauling hose reels (yellow coats, NO vehicle, nothing red)
  BEHIND THE LAVA, on a low corduroy road or a narrow-gauge track of their own, half as big again, 87 to 93 m:
    SM_P3C_MuleTrain        pack mules, their drovers, a child riding, goats behind
    SM_P3C_OreTubs          miners pushing ore tubs along the rails
    SM_P3C_Handcar          two pump trolleys seesawing along the rails, one towing a flat car
    SM_P3C_GoatRun          a herd of goats running the dyke, the herd's child and the dog after them
    SM_P3C_Ropeway          a ropeway: open cars going both ways between three pylons, passengers waving
    SM_P3C_Whim             a mule gin: two mules walking a winding drum round and round, a bucket rising at the shaft
  OVER THE LAVA CHANNELS (45 m left, 53.5 m right), 5 to 9 m up:
    SM_P3C_Skimmers         a string of terns skimming the channel's heat

No glow nearer than 24 m (the near pieces are written at 25 m and carry the level's small steady lift, like the main
cast's), nothing blinks, nothing is red (ENGINE 13 is a red fire engine), no vehicle with an engine.

Run in the editor:  import ft_people_volcano_p3c as K; K.run()
Placed by the block C:/ftwork/p3/L3c.inl (the integrator pastes it into FTPeopleL3.inl).
"""
import math
import random
import unreal
import ft_people as P
import ft_people_volcano as V
from ft_desert import rod, tilt_box

DIR = '/Game/People/Volcano'
DECK = 180.0
PLANK, PLANK_D, STEEL, STEEL_D = '#e8d8b8', '#c9a070', '#8f9bb3', '#5a5870'
PALE, WHITE, DARK, YEL, LIME, PINK = '#b0a088', '#fff6ea', '#22202a', '#ffc93c', '#b8ff5a', '#ff5fa2'
WOOD, TEAL, SKY = '#c9a070', '#2ee6ff', '#57c7ff'
CLOTH = V.CLOTH
tv = V.tv


def yaw_of(v):
    return 90.0 if v > 0 else -90.0


# ------------------------------------------------------------------------------------------ what they travel on
def catwalk(g, L):
    """The near pieces' deck: a narrow trestle 1.8 m up, open underneath, a rail on the road side."""
    V.trestle(g, L, 170.0, bays=500.0)


def corduroy(g, L, W=260.0, h=100.0, top=PLANK, edge=PLANK_D, rails=False):
    """A low raised road behind the lava: planks on short piles rooted 2 m into the ground. rails: a narrow-gauge track."""
    g.box(top, (0, 0, h - 8), (L, W, 16))
    g.box(edge, (0, -W / 2.0 - 4, h - 14), (L, 8, 28))
    g.box(edge, (0, W / 2.0 + 4, h - 14), (L, 8, 28))
    n = max(int(L / 450.0), 2)
    for k in range(n + 1):
        x = -L / 2.0 + 20 + k * (L - 40) / n
        for sy in (-1, 1):
            g.box(STEEL_D, (x, sy * (W / 2.0 - 14), (h - 16 - 200) / 2.0), (16, 16, h - 16 + 200))
    if rails:
        for sy in (-1, 1):
            g.box(STEEL, (0, sy * 42, h + 4), (L, 8, 8))
        m = max(int(L / 200.0), 2)
        for k in range(m):
            g.box(DARK, (-L / 2.0 + (k + 0.5) * L / m, 0, h + 1), (18, 120, 4))


# ------------------------------------------------------------------------------------------ who travels
def runner(sc, r, x, y, L, v, z=DECK, look=None, detail=0, carry=None, **kw):
    """Someone running the deck's length (v cm/s: negative runs towards -X) and coming round again."""
    d = dict(look or V.villager(r))
    bent = ((20, 30), (70, 85))
    pose = dict(stride=38, swing_l=34, swing_r=-34, axis_l='x', axis_r='x', arm_l=bent, arm_r=bent, rate=abs(v) / 190.0, bob=5.0)
    if carry == 'bundle':
        pose.update(arm_r='up', swing_r=0, hold_r=V.hold_bundle(r.choice(CLOTH)))
    elif carry == 'bag':
        pose.update(arm_r='down', hold_r=P.hold_bag(r.choice(CLOTH)), swing_r=-10)
    elif carry == 'roll':
        pose.update(arm_r='face', swing_r=0, hold_r=V.hold_roll(r.choice(CLOTH)))
    elif carry == 'pack':
        d['pack'] = r.choice(CLOTH)
    elif carry == 'wave':
        pose.update(arm_l='wave', swing_l=26, axis_l='y')
    elif carry == 'back':
        pose.update(arm_l='back', arm_r='back', swing_l=0, swing_r=0)
    elif carry == 'push':
        pose.update(arm_l='push', arm_r='push', swing_l=0, swing_r=0)
    pose.update(kw)
    d.update(pose)
    P.figure(sc, x, y, z, yaw=yaw_of(v), detail=detail, phase=r.random(), travel=(L, v), **d)


def dog(sc, r, x, y, z, L, v, body='#e8c26a'):
    P.quadruped(sc, x, y, z, yaw=yaw_of(v), length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=30,
                rate=min(2.0, abs(v) / 100.0), bob=4.0, phase=r.random(), travel=(L, v), body=body, ears='#5a4a3c')


def mule(sc, r, x, y, z, L, v, pack=True, s=1.0):
    P.quadruped(sc, x, y, z, yaw=yaw_of(v), s=s, length=120, height=100, girth=38, neck=40, neck_up=48, head=34,
                body=r.choice(('#a8703a', '#8e5a3a', '#cfcfd8')), mane='#3b2a22', tail=34, stride=20, rate=abs(v) / 110.0, phase=r.random(),
                travel=(L, v))
    if pack:
        pk = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, z + 100 * s)))
        for sy in (-1, 1):
            pk.box(r.choice(CLOTH), (x, y + sy * 28 * s, z + 92 * s), (70 * s, 22 * s, 40 * s))
        pk.sphere(r.choice(CLOTH), (x, y, z + 124 * s), 28 * s, scale=(1.4, 1.0, 0.7), steps=(4, 7))


def running_goat(sc, r, x, y, z, L, v, s=1.0):
    V.goat(sc, r, x, y, z, yaw=yaw_of(v), s=s, stride=28, wag=30, rate=min(2.0, abs(v) / 80.0), phase=r.random(), travel=(L, v), bob=6.0)


# ------------------------------------------------------------------------------------------ NEAR: the catwalk
def runners(seed, v, with_dog=True):
    def build(sc):
        r = random.Random(seed)
        L = 2000.0
        catwalk(sc.still(), L)
        xs = [-860, -610, -370, -140, 110, 350, 590, 830]
        acts = ['bundle', 'wave', 'pack', 'roll', 'bag', 'bundle', None, 'pack']
        r.shuffle(acts)
        for i, x in enumerate(xs):
            y = 10.0 if i % 2 else 34.0
            if i == 3:                                           # a child, hand up, beside the next grown-up
                runner(sc, r, x, y, L, v, carry='wave', build='kid', s=r.uniform(0.62, 0.7), detail=1)
                continue
            if i == 6:
                if with_dog:
                    dog(sc, r, x, 20.0, DECK, L, v)
                else:
                    running_goat(sc, r, x, 20.0, DECK, L, v)
                continue
            runner(sc, r, x, y, L, v, carry=acts[i], detail=1 if i in (0, 4) else 0)
    return build


def bike(sc, r, x, y, z, L, v, col):
    """A bicycle and its rider, travelling. The rider sits on the saddle 96 cm up."""
    d = 1.0 if v > 0 else -1.0
    m = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, z + 60.0)), (x, y, z), 0.0, 1.0)
    for sx in (-1, 1):
        rod(m, DARK, (sx * 52, -4, 34), (sx * 52, 4, 34), 34, steps=8)
    tilt_box(m, col, (-d * 52, 0, 34), (-d * 12, 0, 92), 6, 6)
    tilt_box(m, col, (-d * 12, 0, 90), (d * 38, 0, 98), 6, 6)
    tilt_box(m, col, (d * 38, 0, 104), (d * 52, 0, 34), 6, 6)
    tilt_box(m, col, (0, 0, 36), (-d * 12, 0, 92), 6, 6)
    m.box(DARK, (-d * 12, 0, 97), (28, 14, 7))
    m.box(STEEL, (d * 36, 0, 110), (7, 48, 7))
    m.box(r.choice(CLOTH), (-d * 54, 0, 88), (40, 36, 30))           # the crate on the rack
    m.box(WHITE, (-d * 54, 0, 105), (42, 8, 5))
    look = V.villager(r, hat='helmet', hat_col=r.choice(CLOTH), wear='shorts', pack=r.choice((None, r.choice(CLOTH))))
    P.figure(sc, x - d * 12, y, z + 50.0, yaw=yaw_of(v), legs='sit', seat=46.0, arm_l='push', arm_r='push', rock=3.0, rate=1.6,
             phase=r.random(), detail=0, travel=(L, v), **look)


def cyclists(sc):
    r = random.Random(303)
    L, v = 2000.0, 520.0
    catwalk(sc.still(), L)
    cols = [PINK, TEAL, YEL, LIME, '#8f6bff']
    for i, x in enumerate((-800, -420, -60, 370, 760)):
        bike(sc, r, x + r.uniform(-30, 30), 22.0, DECK, L, v, cols[i])


def reel_cart(sc, x, y, z, L, v):
    """A hose reel on two wheels, towed by hand."""
    d = 1.0 if v > 0 else -1.0
    m = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, z + 50.0)), (x, y, z), 0.0, 1.0)
    for sy in (-1, 1):
        rod(m, DARK, (0, sy * 42 - 4, 30), (0, sy * 42 + 4, 30), 30, steps=8)
        rod(m, YEL, (0, sy * 30 - 3, 66), (0, sy * 30 + 3, 66), 44, steps=8)
    rod(m, '#d7e3f0', (0, -27, 66), (0, 27, 66), 36, steps=8)
    m.box(STEEL_D, (0, 0, 32), (12, 70, 12))
    m.box(STEEL_D, (0, 0, 48), (10, 66, 36))
    tilt_box(m, STEEL_D, (0, 0, 40), (d * 110, 0, 92), 6, 6)
    m.box(STEEL_D, (d * 110, 0, 92), (6, 60, 6))


def hose_run(sc):
    r = random.Random(404)
    L, v = 2000.0, -300.0
    catwalk(sc.still(), L)
    for i, x in enumerate((-520, 420)):
        reel_cart(sc, x, 20.0, DECK, L, v)
        for k, sy in enumerate((-1, 1)):                        # the two who tow it, a hand back to the bar
            runner(sc, r, x - 150 - k * 30, 20.0 + sy * 30, L, v, look=V.fire_look(r), carry='back', detail=1 if (i + k) % 2 else 0)
    for i, x in enumerate((-880, -160, 60, 800)):
        look = V.fire_look(r, pack=STEEL if i % 2 else None)
        runner(sc, r, x, 10.0 if i % 2 else 34.0, L, v, look=look, carry='wave' if i == 1 else None, detail=1 if i == 2 else 0)


# ------------------------------------------------------------------------------------------ BEHIND THE LAVA
def mule_train(sc):
    r = random.Random(505)
    L, v, H = 3600.0, 150.0, 100.0
    corduroy(sc.still(), L, 280.0, H)
    V.walker(sc, r, 1500, 0, L, v, z=H, carry='staff', detail=1)                      # the drover in front
    for i, x in enumerate((1180, 760, 340, -80)):
        mule(sc, r, x, r.uniform(-20, 20), H, L, v, pack=(i != 3))
    look = V.villager(r, build='kid', s=0.66)                                           # a child rides the last mule
    P.figure(sc, -80, 0, H + 100 - 30, yaw=yaw_of(v), legs='sit', seat=46.0, arm_l='wave', swing_l=26, axis_l='y', rate=1.1,
             phase=r.random(), detail=1, travel=(L, v), **look)
    V.walker(sc, r, 560, -80, L, v, z=H, carry='bundle')
    V.walker(sc, r, -420, 60, L, v, z=H, carry='wave', detail=1)
    for k in range(3):
        V.goat(sc, r, -760 - k * 190, -50 + k * 50, H, yaw=yaw_of(v), stride=22, wag=30, rate=abs(v) / 60.0, phase=r.random(),
               travel=(L, v), bob=2.0)
    V.walker(sc, r, -1420, 10, L, v, z=H, carry='staff')


def tub(sc, r, x, y, z, L, v, col):
    m = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, z + 60.0)), (x, y, z), 0.0, 1.0)
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(DARK, (sx * 46, sy * 42, 16), (30, 9, 30))
    m.box(STEEL_D, (0, 0, 30), (150, 70, 10))
    m.box(col, (0, 0, 74), (140, 96, 78))
    m.box(WHITE, (0, 0, 110), (146, 102, 10))
    m.sphere(r.choice(('#5a5870', '#b0a088', '#3a3848')), (0, 0, 112), 50, scale=(1.25, 0.85, 0.6), steps=(4, 7))


def miner(r, **fixed):
    d = P.look(r, tops=(TEAL, '#7dffb0', SKY, '#f4a6ff'), bottoms=('#2b2350', '#3a5fa8', '#5a4a3c'), wear='trousers', sleeves='short',
               hat='hard', hat_col=r.choice((YEL, WHITE, LIME)), band=WHITE)
    d.update(fixed)
    return d


def ore_tubs(sc):
    r = random.Random(606)
    L, v, H = 3000.0, -140.0, 100.0
    corduroy(sc.still(), L, 220.0, H, top='#cfcfd8', edge=STEEL_D, rails=True)
    for i, x in enumerate((-1150, -420, 330, 1060)):
        tub(sc, r, x, 0, H + 8, L, v, (SKY, PINK, LIME, '#8f6bff')[i])
        runner(sc, r, x + 135, 0, L, v, z=H + 8, look=miner(r), carry='push', stride=24, rate=abs(v) / 110.0, bob=2.0, detail=1 if i % 2 else 0)
    runner(sc, r, 700, 60, L, v, z=H + 8, look=miner(r), carry='wave', stride=24, rate=abs(v) / 110.0, bob=2.0, detail=1,
           hold_r=P.hold_tool())
    runner(sc, r, -780, -60, L, v, z=H + 8, look=miner(r, pack=YEL), stride=24, rate=abs(v) / 110.0, bob=2.0)


def flat_car(sc, x, y, z, L, v, col=STEEL_D):
    m = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, z + 40.0)), (x, y, z), 0.0, 1.0)
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(DARK, (sx * 70, sy * 42, 16), (30, 9, 30))
    m.box(col, (0, 0, 37), (230, 120, 14))
    return m


def handcar(sc, r, x, y, z, L, v, ph):
    """A pump trolley: the beam seesaws, the two who work it lean in turn."""
    m = flat_car(sc, x, y, z, L, v, YEL)
    m.box(STEEL_D, (0, 0, 84), (22, 22, 80))
    top = z + 126.0
    rate = 1.3
    b = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, top), swing=15.0, axis=90.0, rate=rate, phase=ph), (x, y, top), 0.0, 1.0)
    b.box(DARK, (0, 0, 0), (210, 10, 10))
    for sx in (-1, 1):
        b.box(STEEL, (sx * 102, 0, 0), (8, 70, 8))
    for k, sx in enumerate((-1, 1)):
        fx = x + sx * 82
        P.figure(sc, fx, y, z + 44.0, yaw=90.0 if sx < 0 else -90.0, legs='wide', arm_l='push', arm_r='push',
                 sway=dict(swing=-sx * 9.0, pivot=(fx, y, z + 44.0), axis=90.0), rate=rate, phase=ph, detail=1, travel=(L, v), **miner(r))


def handcars(sc):
    r = random.Random(707)
    L, v, H = 3000.0, 330.0, 100.0
    corduroy(sc.still(), L, 220.0, H, top='#cfcfd8', edge=STEEL_D, rails=True)
    handcar(sc, r, 700, 0, H + 8, L, v, 0.1)
    m = flat_car(sc, 430, 0, H + 8, L, v)
    m.box(TEAL, (-50, 10, 68), (70, 70, 48))
    m.box(PINK, (-50, 6, 108), (50, 50, 32))
    m.box(STEEL, (118, 0, 40), (60, 8, 8))
    P.figure(sc, 475, 0, H + 8 + 44, yaw=0.0, legs='sit', seat=30.0, arm_r='wave', swing_r=26, rate=1.2, react=True, phase=r.random(),
             detail=1, travel=(L, v), **V.villager(r, build='kid', s=0.68))
    handcar(sc, r, -760, 0, H + 8, L, v, 0.6)


def goat_run(sc):
    r = random.Random(808)
    L, v, H = 3200.0, 320.0, 90.0
    corduroy(sc.still(), L, 300.0, H, top=PALE, edge='#8f8470')
    for i, (x, y) in enumerate(((1300, -40), (1090, 60), (860, -70), (640, 30), (400, -30), (190, 70), (-40, -60))):
        running_goat(sc, r, x, y, H, L, v)
    dog(sc, r, -330, 40, H, L, v, body='#3b2a22')
    runner(sc, r, -620, -10, L, v, z=H, carry='wave', build='kid', s=0.7, detail=1, hold_r=V.hold_staff())
    runner(sc, r, -1150, 30, L, v, z=H, carry='bundle', detail=1)


def ropeway(sc):
    r = random.Random(909)
    L, TOP = 3200.0, 1150.0
    g = sc.still()
    for x in (-1540, 0, 1540):
        for sy in (-1, 1):
            tilt_box(g, STEEL_D, (x, sy * 260, -150), (x, sy * 40, TOP), 26, 26)
        g.box(STEEL_D, (x, 0, TOP - 18), (30, 330, 22))
        g.box(YEL, (x, 0, TOP + 16), (40, 110, 44))
        g.box(STEEL_D, (x, 0, 520), (22, 300, 16))
    for sy in (-1, 1):
        g.box(DARK, (0, sy * 130, TOP - 34), (L, 6, 6))
    cars = ((-1150, -130, 210.0, PINK), (420, -130, 210.0, TEAL), (-380, 130, -210.0, YEL), (1180, 130, -210.0, '#8f6bff'))
    for (x, y, v, col) in cars:
        F = TOP - 290.0                                          # the car's floor
        m = sc.part(P.motion(travel=tv(L, v), base=(x, y), pivot=(x, y, TOP - 150.0), bob=-5.0, rate=0.5, phase=r.random()), (x, y, 0.0), 0.0, 1.0)
        m.box(STEEL_D, (0, 0, TOP - 100), (8, 8, 130))
        m.box(DARK, (0, 0, TOP - 34), (46, 14, 18))
        m.box(WHITE, (0, 0, TOP - 166), (236, 124, 10))
        m.box(col, (0, 0, F + 4), (216, 110, 10))
        m.box(col, (0, -55, F + 40), (216, 6, 70))
        m.box(col, (0, 55, F + 40), (216, 6, 70))
        for sx in (-1, 1):
            m.box(col, (sx * 106, 0, F + 40), (6, 110, 70))
            m.box(WHITE, (sx * 104, 0, F + 100), (7, 7, 60))
        for k, sx in enumerate((-1, 1)):
            look = V.villager(r)
            P.figure(sc, x + sx * 52, y - 12, F + 8, yaw=r.uniform(-25, 25), arm_l=('wave' if k else 'down'), arm_r=('down' if k else 'wave'),
                     swing_l=(24 if k else 0), swing_r=(0 if k else 24), rate=1.2, phase=r.random(), detail=0, travel=(L, v), **look)


def whim(sc):
    """A mule gin at a shaft head: two mules walk the beam round, the drum turns, a bucket rises and falls at the frame."""
    r = random.Random(1010)
    RATE, R = 0.07, 430.0
    g = sc.still()
    g.cyl(PALE, (0, 0, -26), 620, 30, steps=12)                  # the trodden ring
    g.cyl(STEEL_D, (0, 0, 0), 20, 350, steps=6)
    for sy in (-1, 1):
        g.box(WOOD, (0, sy * 660, 80), (26, 26, 560))
    g.box(WOOD, (0, 0, 352), (24, 1340, 24))
    apex = (0.0, 900.0, 560.0)
    for a in (30, 150, 270):
        tilt_box(g, WOOD, (apex[0] + math.cos(math.radians(a)) * 260, apex[1] + math.sin(math.radians(a)) * 260, -100), apex, 20, 20)
    rod(g, YEL, (-10, apex[1], apex[2] - 30), (10, apex[1], apex[2] - 30), 34, steps=8)
    tilt_box(g, WHITE, (0, 60, 270), (0, apex[1], apex[2] - 30), 6, 6)
    g.box(DARK, (0, 900, 6), (150, 150, 12))                     # the shaft's mouth
    turn = P.motion(base=(0.0, 0.0), travel=(-1.0, RATE))
    m = sc.part(turn)
    m.cyl(WOOD, (0, 0, 240), 90, 70, steps=8)
    for k in range(4):
        a = math.radians(k * 90 + 45)
        m.box(WHITE, (math.cos(a) * 88, math.sin(a) * 88, 275), (16, 16, 74))
    m.box(WOOD, (0, 0, 210), (2 * R + 80, 18, 18))
    for sx in (-1, 1):
        m.box(STEEL_D, (sx * R, 0, 170), (10, 10, 70))
    for sx in (-1, 1):
        P.quadruped(sc, sx * R, 0, 0, yaw=180.0 if sx > 0 else 0.0, length=120, height=100, girth=38, neck=40, neck_up=48, head=34,
                    body=('#a8703a' if sx > 0 else '#cfcfd8'), mane='#3b2a22', tail=34, stride=20, rate=1.2, phase=r.random(), orbit=(0.0, 0.0, RATE))
    th = math.radians(-28.0)                                     # the driver walks behind the first mule
    P.figure(sc, math.cos(th) * 400, math.sin(th) * 400, 0, yaw=180.0 - 28.0, stride=22, swing_l=16, axis_l='x', arm_r=((36, 70), (72, 84)),
             hold_r=V.hold_staff(), rate=1.1, bob=2.0, phase=r.random(), detail=1, orbit=(0.0, 0.0, RATE), **miner(r))
    P.figure(sc, -210, 0, 219 - 31, yaw=0.0, legs='sit', seat=46.0, arm_l='wave', swing_l=26, rate=1.1, phase=r.random(), detail=1,
             orbit=(0.0, 0.0, RATE), **V.villager(r, build='kid', s=0.68))
    bk = sc.part(P.motion(bob=-110.0, rate=0.12, pivot=(0.0, 900.0, 300.0)))
    bk.cyl(TEAL, (0, 900, 250), 42, 70, steps=8, top_r=50)
    bk.box(WHITE, (0, 900, 420), (6, 6, 200))
    P.figure(sc, 150, 880, 0, yaw=90.0, arm_l='up', arm_r='up', swing_l=12, swing_r=-12, rate=0.6, phase=0.2, detail=1, **miner(r))
    P.figure(sc, -260, 760, 0, yaw=-20.0, arm_r='wave', swing_r=26, arm_l='hip', rate=1.2, bob=4.0, react=True, phase=0.5, detail=1, **miner(r))


def skimmers(sc):
    r = random.Random(1111)
    L, v = 5000.0, 900.0
    for i in range(7):
        x = -1900 + i * 600 + r.uniform(-120, 120)
        P.bird(sc, x, r.uniform(-220, 220), 520 + (i % 3) * 150 + r.uniform(-40, 40), heading=0.0, span=r.uniform(270, 350), body='#e6fbff',
               wing=SKY, beak='#ff7a3c', phase=r.random(), rate=2.3, flap=34.0, travel=(L, v), bob=-25.0)


NEAR_LIFT = 0.035        # as the main cast's near pieces: a small steady lift (these are written at 25 m: over 24)
FAR_LIFT = 0.05

# The catwalk's runners travel only along X: the bounds grow 8 cm across the road's direction, not the default 60, so that
# the bounds' road-side edge (25.2 - 0.93 - 0.08 = 24.19 m) stays outside the track's 24 m sight line and nothing is pushed out.
CATWALK = dict(grow=(60.0, 8.0, 60.0), grow_down=(60.0, 8.0, 0.0))

JOBS = [
    ('SM_P3C_Runners_A', V.lifted(runners(101, 300.0), NEAR_LIFT), CATWALK),
    ('SM_P3C_Runners_B', V.lifted(runners(202, -280.0, with_dog=False), NEAR_LIFT), CATWALK),
    ('SM_P3C_Cyclists', V.lifted(cyclists, NEAR_LIFT), CATWALK),
    ('SM_P3C_HoseRun', V.lifted(hose_run, NEAR_LIFT), CATWALK),
    ('SM_P3C_MuleTrain', V.lifted(mule_train, FAR_LIFT)),
    ('SM_P3C_OreTubs', V.lifted(ore_tubs, FAR_LIFT)),
    ('SM_P3C_Handcar', V.lifted(handcars, FAR_LIFT)),
    ('SM_P3C_GoatRun', V.lifted(goat_run, FAR_LIFT)),
    ('SM_P3C_Ropeway', V.lifted(ropeway, FAR_LIFT)),
    ('SM_P3C_Whim', V.lifted(whim, FAR_LIFT)),
    ('SM_P3C_Skimmers', V.lifted(skimmers, 0.07)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
