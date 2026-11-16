"""FULL THROTTLE (Unreal) the population of level 3, Nevada (level index 2), pass 3, builder p3c: THE MOVERS.
/Game/People/Nevada/SM_P3C_*.

Everything in this kit TRAVELS: along the road on a path of its own (a berm, a boardwalk, a trail, a rail bed) and comes
round again, or round a ring (the round pen). Nothing crosses the road: a path lies along the mesh's X, which the
placement lays along the road, 33 m out and more.

  By the road (a 2.3 m berm of the canyon's rock with a pale top: the barrier hides the ground itself out to 33 m)
    SM_P3C_Runners_A     a desert foot race: seven runners with bibs passing one another, course flags, a water table
    SM_P3C_Runners_B     the same race going the other way, a dog running with it, two race walkers
    SM_P3C_Hikers        a file of hikers with packs and poles, the leader pointing, a loaded burro at the tail
    SM_P3C_Cyclists_A    four touring cyclists (pale thin wheels, bright frames, helmets), a pennant, one waving
    SM_P3C_Parade        a marching band: a drum major, flags, drums and horns, in step (the strip districts)
    SM_P3C_StripWalkers  a boardwalk: strollers both ways, a pedicab with two passengers, a vendor pushing a cart
  Middle distance (twice life size and more, on a pale trail the piece brings)
    SM_P3C_Cyclists_B    a peloton of seven, strung out
    SM_P3C_MuleTrain     a prospector on foot, a rider waving his hat, five pack mules with bright loads
    SM_P3C_Stagecoach    a red coach behind four horses at the gallop, an outrider, the dust behind
    SM_P3C_Handcar       a rail bed: two people pumping a hand car along it, a dog after them, a flagman
    SM_P3C_LandYachts    three land yachts under tall two-coloured sails, heeling as they run (the flats)
    SM_P3C_RoundPen      a round pen: two riders cantering round it, the trainer turning in the middle, kids on the rail

Looks only; movement by the vertex shader (ft_people's motion values); no light, no words, no marks.
The placing rules are the block C:/ftwork/p3/L2c.inl (pasted into Source/FullThrottle/FTPeopleL2.inl by the integrator).
Run in the editor:  import ft_people_nevada_p3c; ft_people_nevada_p3c.run()
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box
import ft_people_nevada_far as F

DIR = '/Game/People/Nevada'

swell, bush, puff, beast, hand_look = F.swell, F.bush, F.puff, F.beast, F.hand_look

SAND, SANDST, SANDST_D, ROCK_R, TALUS = '#f2c58a', '#e0784a', '#b8553a', '#c9603e', '#a04a34'
PALE, DUST, SALT = '#f7cf98', '#fff1dc', '#fff6ea'
WOOD, BARK_D, GRAVEL, STEEL = '#9a6a44', '#6a4c36', '#b8a898', '#8f9bb3'
INK, NAVY, TEAL, RED, MAG, CYAN = '#22202a', '#2b2350', '#2fc6c0', '#ff2d3c', '#ff4fb8', '#2ee6ff'
CREAM, WHITE, GOLD, YEL, DENIM, LIME, ORANGE, VIOLET = '#f4efe6', '#fff6ea', '#ffc93c', '#ffe14d', '#3a5fa8', '#b8ff5a', '#ff7a3c', '#8f6bff'
HOT = (RED, CYAN, YEL, MAG, LIME, ORANGE, VIOLET, TEAL, WHITE)
MULES = ('#cfcfd8', '#f4efe6', '#d9a070', '#5e3b28')


# ------------------------------------------------------------------------------------------ helpers
def tvl(L, sp):
    return (float(L), float(sp) / float(L))


def car(sc, x, y, z, yaw, s, L, sp, phase=0.0, rate=0.0, bob=0.0, swing=0.0, pivot=(0.0, 0.0, 0.0), axis=0.0):
    """The model for a thing that travels (length L cm along the mesh's X at sp cm/s, negative = towards -X), drawn
    in its own frame at (x, y, z), turned by yaw, scaled by s. swing about pivot (own frame) round axis (degrees from
    the frame's own X in the ground plane, or P.UP)."""
    c, sn = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    piv = (x + s * (pivot[0] * c - pivot[1] * sn), y + s * (pivot[0] * sn + pivot[1] * c), z + s * pivot[2])
    mo = P.motion(phase=phase, rate=rate, bob=bob, swing=swing, pivot=piv, axis=(P.UP if axis == P.UP else yaw + axis), base=(x, y), travel=tvl(L, sp))
    return sc.part(mo, (x, y, z), yaw, s)


def walker(sc, r, x, y, z, d, L, sp, s=1.25, stride=28.0, arms=24.0, rate=1.5, bob=4.0, **kw):
    """Somebody on foot going along the piece: d = +1 towards +X, -1 towards -X."""
    a = dict(P.look(r))
    a['s'] = s * r.uniform(0.95, 1.05)
    a.update(yaw=(90.0 if d > 0 else -90.0), stride=stride, swing_l=arms, swing_r=-arms, axis_l='x', axis_r='x', rate=rate, bob=bob,
             phase=r.random(), travel=(L, d * sp), detail=1)
    a.update(kw)
    return P.figure(sc, x, y, z, **a)


def berm(sc, r, L, H=230.0, D=660.0, top=PALE):
    """The bank a near piece stands on: the canyon's rock, a pale flat top. Returns the still model."""
    g = sc.still()
    swell(g, r, L, D, H, tones=(TALUS, SANDST_D, ROCK_R, top), top=0.62, rough=0.1, step=110.0)
    return g


def trail(sc, r, L, D, Z, S, tones=(SANDST_D, SANDST, PALE), bushes=6):
    """The pale swell a middle-distance piece runs on."""
    g = sc.still()
    swell(g, r, L + 1200.0, D, Z, tones=tones, top=0.55)
    for k in range(bushes):
        bush(g, r, -L / 2.0 + 500 + k * (L - 1000.0) / max(bushes - 1, 1) + r.uniform(-200, 200), r.choice((-1, 1)) * D * 0.25, Z - 60, 0.5 * S)
    return g


def flags(g, L, H, y, n=5, cols=(MAG, CYAN, YEL)):
    for k in range(n):
        fx = -L / 2.0 + 150.0 + k * (L - 300.0) / (n - 1.0)
        g.box(CREAM, (fx, y, H + 115.0), (6, 6, 230))
        g.box(cols[k % len(cols)], (fx + 32.0, y, H + 204.0), (60, 5, 44))


def cyclist(sc, r, x, y, z, d, L, sp, s, col, detail=1, flag=None, wave=False, phase=0.0, top=None):
    """A bicycle and its rider, going along the piece. The bicycle is drawn in the rider's frame (front = -Y)."""
    yaw = 90.0 if d > 0 else -90.0
    m = car(sc, x, y, z, yaw, s, L, d * sp, phase=phase, rate=2.0, bob=2.0)
    for wy in (-54.0, 54.0):
        rod(m, CREAM, (-2.5, wy, 35.0), (2.5, wy, 35.0), 35.0, steps=(8 if detail else 6))
    tilt_box(m, col, (0, 54, 35), (0, 22, 92), 6)
    tilt_box(m, col, (0, 22, 92), (0, -42, 98), 6)
    tilt_box(m, col, (0, -42, 100), (0, -54, 35), 6)
    if detail:
        tilt_box(m, col, (0, 6, 38), (0, 22, 92), 6)
        tilt_box(m, col, (0, 6, 38), (0, -42, 96), 6)
    m.box(INK, (0, -44, 106), (46, 6, 6))
    m.box(INK, (0, 24, 96), (12, 26, 6))
    if flag:
        m.box(CREAM, (0, 58, 150), (4, 4, 200))
        m.box(flag, (0, 76, 236), (4, 40, 28))
    c, sn = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    off = 22.0
    a = dict(P.look(r, wear='shorts', hat='helmet', hat_col=r.choice(HOT)))
    a['s'] = s
    if top:
        a['top'] = top
    P.figure(sc, x - s * off * sn, y + s * off * c, z + s * 50.0, yaw=yaw, legs='sit', arm_l='push', arm_r=('wave' if wave else 'push'),
             swing_r=(26.0 if wave else 0.0), axis_r='y', phase=phase, rate=2.0, bob=2.0, detail=detail, travel=(L, d * sp), **a)


def pack_load(sc, x, y, z, yaw, s, L, sp, phase, rate, bob, height, girth, col, col2):
    """A load on an animal's back (the frame of ft_people_nevada_far.beast: head towards +X at yaw 0)."""
    m = car(sc, x, y, z, yaw, s, L, sp, phase=phase, rate=rate, bob=bob)
    m.box(col, (-6, 0, height + 20), (84, girth + 22, 44))
    m.box(col2, (-6, 0, height + 20), (30, girth + 26, 46))
    m.box(CREAM, (-6, 0, height + 48), (60, girth * 0.7, 16))


# ------------------------------------------------------------------------------------------ by the road
def _runners(sc, seed, d, variant):
    r = random.Random(seed)
    L, H = 2600.0, 230.0
    g = berm(sc, r, L, H)
    TL = L - 300.0
    flags(g, TL, H, 165.0)
    ys = (-80, 10, -30, 60, -60, 0, 40)
    n = 7 if variant == 'a' else 5
    for i in range(n):
        x = -TL / 2.0 + (i + 0.5) * TL / n + r.uniform(-90, 90)
        walker(sc, r, x, ys[i], H, d, TL, r.uniform(360.0, 480.0), stride=42.0, arms=44.0, rate=r.uniform(2.3, 2.8), bob=9.0, wear='shorts',
               band=WHITE, top=HOT[(i * 2 + (0 if variant == 'a' else 3)) % 8], hat=(None, 'cap', None)[i % 3], hat_col=HOT[(i + 4) % 8], sleeves='none')
    if variant == 'a':
        # the water table at the back, a helper holding a cup out to the race
        g.box(WHITE, (700, 150, H + 78), (200, 70, 8))
        g.box(CYAN, (700, 150, H + 40), (190, 62, 70))
        for k in range(4):
            g.cyl(CREAM, (630 + k * 46, 140, H + 82), 7, 16, steps=6)
        P.figure(sc, 560, 110, H, yaw=20, arm_r='fwd', swing_r=16, axis_r='x', hold_r=P.hold_cup(), arm_l='wave', swing_l=24, rate=1.4, phase=0.3,
                 react=True, detail=1, **P.look(r, top=LIME, hat='cap', hat_col=WHITE, s=1.25))
    else:
        # two race walkers, elbows out, and a dog that runs the course with them
        for k in range(2):
            walker(sc, r, -600.0 + k * 1100.0, 110.0, H, d, TL, 210.0, stride=30.0, arms=34.0, rate=3.0, bob=3.0, wear='shorts', band=WHITE,
                   top=(MAG, YEL)[k], arm_l='hip', arm_r='hip', hat='sun', hat_col=WHITE)
        P.quadruped(sc, 200.0, -110.0, H, yaw=(90.0 if d > 0 else -90.0), s=1.5, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22,
                    wag=30, stride=34, rate=3.2, bob=8, phase=0.4, travel=(TL, d * 520.0), body=WHITE, ears=INK, tail_col=INK, snout=INK)


def runners_a(sc):
    _runners(sc, 4201, 1, 'a')


def runners_b(sc):
    _runners(sc, 4202, -1, 'b')


def hikers(sc):
    """A file of six hikers heading -X: packs, hats, poles, the leader pointing the way; a loaded burro at the tail."""
    r = random.Random(4203)
    L, H = 2600.0, 230.0
    g = berm(sc, r, L, H)
    TL, sp, d = L - 300.0, 170.0, -1
    for cx in (-900.0, 300.0, 1000.0):          # cairns at the back of the trail
        g.cyl(SANDST_D, (cx, 170, H), 34, 30, steps=6, top_r=26)
        g.cyl(ROCK_R, (cx, 170, H + 30), 24, 26, steps=6, top_r=18)
        g.cyl(SAND, (cx, 170, H + 56), 15, 22, steps=6, top_r=10)
    packs = (RED, CYAN, YEL, MAG, LIME, ORANGE)
    for i in range(6):
        x = -800.0 + i * 290.0 + r.uniform(-30, 30)
        kw = dict(pack=packs[i], hat=('sun', 'cap', 'cowboy')[i % 3], hat_col=(CREAM, RED, '#5a4a3c')[i % 3], sleeves='long')
        if i == 0:
            kw.update(arm_l='point', swing_l=8.0)
        elif i % 2:
            kw.update(hold_r=P.hold_tool(col=CREAM, grip=INK, length=70.0))
        walker(sc, r, x, (-50, 30)[i % 2], H, d, TL, sp, stride=24.0, arms=18.0, rate=1.5, bob=3.0, **kw)
    bx, by = 1010.0, -10.0
    P.quadruped(sc, bx, by, H, yaw=-90.0, s=1.25, body='#cfcfd8', belly=WHITE, length=110, height=92, girth=38, neck=30, neck_up=35, head=30, ears=INK,
                tail=30, tail_col=INK, snout=WHITE, stride=22, rate=1.5, bob=3, phase=0.2, travel=(TL, d * sp))
    m = car(sc, bx, by, H, -90.0, 1.25, TL, d * sp, phase=0.2, rate=1.5, bob=3.0)
    m.box(TEAL, (0, 0, 104), (64, 60, 30))
    m.box(GOLD, (0, 0, 104), (68, 22, 32))
    m.box(CREAM, (0, 0, 124), (40, 44, 12))


def cyclists_a(sc):
    """Four touring cyclists heading +X along the berm, the last with a pennant on a whip, the second waving."""
    r = random.Random(4204)
    L, H = 3000.0, 230.0
    g = berm(sc, r, L, H)
    TL = L - 300.0
    flags(g, TL, H, 170.0, n=4, cols=(ORANGE, WHITE))
    cols = (RED, CYAN, YEL, MAG)
    for i in range(4):
        cyclist(sc, r, 900.0 - i * 560.0 + r.uniform(-40, 40), (-60, 30, -20, 50)[i], H, 1, TL, 760.0, 1.25, cols[i], detail=1,
                flag=(ORANGE if i == 3 else None), wave=(i == 1), phase=i * 0.23, top=cols[(i + 2) % 4])


def hold_drum(col=WHITE, trim=RED):
    def f(m, h, side):
        if side > 0:
            m.cyl(col, (h[0] - 27, h[1] - 8, h[2] - 22), 23, 28, steps=8)
            m.cyl(trim, (h[0] - 27, h[1] - 8, h[2] - 10), 24.5, 6, steps=8)
    return f


def hold_horn(col=GOLD):
    def f(m, h, side):
        if side > 0:
            tilt_box(m, col, (h[0] - 14, h[1], h[2]), (h[0] - 14, h[1] - 46, h[2] + 8), 7, 7)
            m.box(col, (h[0] - 14, h[1] - 52, h[2] + 9), (20, 10, 20))
    return f


def parade(sc):
    """A marching band heading +X in step: the drum major's baton going up and down, two flags, two drums, three
    horns. Red coats, white trousers, tall caps."""
    r = random.Random(4205)
    L, H = 2600.0, 230.0
    g = berm(sc, r, L, H)
    TL, sp, rate = L - 300.0, 150.0, 1.9
    for k in range(6):                                      # bunting posts at the back
        fx = -TL / 2.0 + 150.0 + k * (TL - 300.0) / 5.0
        g.box(WHITE, (fx, 170, H + 100), (7, 7, 200))
        g.box((GOLD, RED)[k % 2], (fx, 170, H + 206), (26, 8, 14))
    uni = dict(top=RED, bottom=WHITE, shoes=INK, sleeves='long', hat='chef', hat_col=WHITE, band=GOLD)

    def member(x, y, **kw):
        a = dict(uni)
        a.update(kw)
        walker(sc, r, x, y, H, 1, TL, sp, stride=30.0, arms=a.pop('arms', 0.0), rate=rate, bob=5.0, phase=0.0, **a)

    member(800.0, 0.0, arm_r='up', swing_r=30.0, axis_r='x', hold_r=P.hold_tool(col=GOLD, grip=WHITE, length=60.0), arm_l='hip', top=WHITE, bottom=RED, band=RED)
    for k, y in enumerate((-70.0, 70.0)):
        member(520.0, y, arm_r='up', swing_r=14.0, axis_r='y', hold_r=P.hold_flag(col=(CYAN, MAG)[k], pole=WHITE, w=80.0))
    for k, y in enumerate((-70.0, 70.0)):
        member(200.0, y, arm_l='carry', arm_r='carry', swing_l=22.0, swing_r=-22.0, hold_r=hold_drum(WHITE, (CYAN, GOLD)[k]))
    for k, (x, y) in enumerate(((-120.0, -75.0), (-120.0, 75.0), (-420.0, 0.0))):
        member(x, y, arm_r='face', arm_l='face', hold_r=hold_horn(GOLD), swing_r=6.0, swing_l=6.0)
    # two children skipping after the band
    for k in range(2):
        walker(sc, r, -760.0 - k * 140.0, (-40.0, 50.0)[k], H, 1, TL, sp, s=0.84, build='kid', stride=40.0, arms=40.0, rate=rate, bob=16.0,
               arm_r='up', top=(YEL, CYAN)[k])


def strip_walkers(sc):
    """A boardwalk in front of the strip: strollers both ways, a pedicab with two passengers under a striped canopy,
    a vendor pushing a cart with a parasol."""
    r = random.Random(4206)
    L, H = 2800.0, 230.0
    g = sc.still()
    g.box(BARK_D, (0, 0, H / 2.0 - 10), (L - 80, 400, H - 20))
    g.box('#c9a070', (0, 0, H - 8), (L, 440, 16))
    for k in range(8):                                      # the rail at the back, pennants on it
        fx = -L / 2.0 + 100.0 + k * (L - 200.0) / 7.0
        g.box(WHITE, (fx, 205, H + 60), (9, 9, 120))
        g.box(HOT[k % 8], (fx, 205, H + 136), (30, 6, 30))
    g.box(WHITE, (0, 205, H + 112), (L - 200, 8, 9))
    TL = L - 300.0
    # the pedicab, +X
    px, py, sp = 500.0, -110.0, 330.0
    cyclist(sc, r, px, py, H, 1, TL, sp, 1.25, RED, detail=1, phase=0.1, top=WHITE)
    m = car(sc, px - 1.25 * 150.0, py, H, 0.0, 1.25, TL, sp, phase=0.1, rate=2.0, bob=2.0)
    m.box(TEAL, (0, 0, 58), (120, 110, 34))
    m.box(TEAL, (-54, 0, 100), (14, 110, 70))
    for side in (-1, 1):
        rod(m, CREAM, (-10, side * 60, 35), (-10, side * 65, 35), 35, steps=8)
        m.box(WHITE, (-52, side * 50, 150), (6, 6, 150))
        m.box(WHITE, (52, side * 50, 150), (6, 6, 150))
        m.box((MAG, YEL)[side > 0], (-14, side * 22, 104), (36, 40, 56))       # the passengers
        m.box(P.SKINS[2 if side > 0 else 4], (-14, side * 22, 146), (28, 28, 28))
        m.box((WHITE, INK)[side > 0], (-14, side * 22, 164), (44, 44, 6))
    m.box(MAG, (0, 0, 228), (140, 128, 10))
    m.box(WHITE, (0, 0, 229), (48, 130, 10))
    tilt_box(m, INK, (60, 0, 50), (120, 0, 70), 6)
    # the vendor and his cart, -X
    vx, vy, vsp = -300.0, 90.0, 120.0
    walker(sc, r, vx, vy, H, -1, TL, vsp, stride=22.0, arms=0.0, rate=1.4, bob=3.0, phase=0.0, arm_l='push', arm_r='push', top=WHITE, hat='chef', hat_col=WHITE,
           band=CYAN)
    m = car(sc, vx - 1.25 * 80.0, vy, H, 180.0, 1.25, TL, -vsp, phase=0.0, rate=1.4, bob=3.0)
    m.box(WHITE, (0, 0, 66), (90, 64, 60))
    m.box(CYAN, (0, 0, 66), (30, 66, 62))
    for side in (-1, 1):
        rod(m, CREAM, (0, side * 34, 26), (0, side * 39, 26), 26, steps=8)
    m.box(WHITE, (0, 0, 150), (5, 5, 110))
    m.cyl(MAG, (0, 0, 196), 78, 30, steps=8, top_r=6)
    # strollers
    walker(sc, r, -900.0, -60.0, H, 1, TL, 130.0, hold_r=P.hold_bag(MAG), wear='dress', hat='sun', hat_col=CREAM)
    walker(sc, r, -810.0, -40.0, H, 1, TL, 130.0, top=CYAN, hat='cowboy', hat_col=WHITE, hold_l=P.hold_camera())
    walker(sc, r, 1000.0, 20.0, H, -1, TL, 150.0, hold_l=P.hold_bag(YEL), hold_r=P.hold_bag(CYAN), top=LIME)
    walker(sc, r, 150.0, 150.0, H, -1, TL, 190.0, s=0.84, build='kid', stride=38.0, arms=36.0, rate=2.4, bob=12.0, top=ORANGE, hat='cap', hat_col=RED)
    walker(sc, r, 60.0, 120.0, H, -1, TL, 190.0, arm_r='wave', swing_r=24.0, axis_r='y', react=True, top=VIOLET)


# ------------------------------------------------------------------------------------------ middle distance
def cyclists_b(sc):
    """A peloton of seven heading -X, strung out along a pale trail; the leader's jersey gold, a pennant at the tail."""
    r = random.Random(4207)
    S, Z, L = 2.2, 200.0, 7000.0
    trail(sc, r, L, 1700.0, Z, S)
    cols = (GOLD, RED, CYAN, MAG, WHITE, LIME, ORANGE)
    for i in range(7):
        x = -1900.0 + i * 620.0 + r.uniform(-80, 80)
        cyclist(sc, r, x, (-180, 60, -60, 200, -220, 0, 120)[i], Z, -1, L, 1300.0, S, cols[(i + 3) % 7], detail=0, flag=(ORANGE if i == 6 else None),
                wave=(i == 3), phase=i * 0.19, top=cols[i])
    tv = tvl(L, -1300.0)
    for k in range(2):
        puff(sc, 2700.0 + k * 500.0, (-80, 90)[k], Z, 150 - k * 30, k * 0.4, 0.7, tv)


def mule_train(sc):
    """A pack train heading +X: the prospector on foot in front with his shovel, five mules nose to tail under bright
    loads, a rider at the tail waving his hat."""
    r = random.Random(4208)
    S, Z, L, sp, rate = 2.2, 200.0, 6500.0, 300.0, 1.2
    trail(sc, r, L, 1700.0, Z, S)
    walker(sc, r, 2250.0, -60.0, Z, 1, L, sp, s=S, stride=24.0, arms=18.0, rate=rate, bob=6.0, phase=0.0, top=RED, bottom=DENIM, hat='cowboy', hat_col=CREAM,
           hold_r=P.hold_tool(col=STEEL, grip=WOOD, length=80.0), pack=GOLD)
    loads = ((RED, WHITE), (CYAN, GOLD), (MAG, WHITE), (YEL, RED), (TEAL, WHITE))
    for i in range(5):
        x = 1500.0 - i * 640.0
        y = (-40, 40, -20, 30, -50)[i]
        ph = i * 0.31
        beast(sc, x, y, Z, s=S, coat=MULES[i % 4], mane=INK, ears=INK, legs_col=INK, length=150, height=116, girth=52, neck=46, neck_up=32, head=42, tail=44,
              phase=ph, rate=rate, bob=7, stride=20, pitch=2, travel=(L, sp))
        pack_load(sc, x, y, Z, 0.0, S, L, sp, ph, rate, 7.0, 116.0, 52.0, loads[i][0], loads[i][1])
    beast(sc, -1900.0, 60.0, Z, s=S * 1.05, coat=GOLD, mane=WHITE, tail=62, tail_w=16, neck_up=48, phase=0.6, rate=rate * 1.3, bob=12, stride=24, pitch=3,
          travel=(L, sp), rider=hand_look(r, 1, 'hat', hat=CREAM, top=CYAN))
    puff(sc, -2500.0, 0.0, Z, 140, 0.2, 0.6, tvl(L, sp))


def stagecoach(sc):
    """A red coach heading -X behind four horses at the gallop: yellow wheels, luggage on the roof, the driver up
    front, an outrider behind waving his hat, the dust rolling after them."""
    r = random.Random(4209)
    S, Z, L, sp, rate = 2.2, 200.0, 8000.0, 1100.0, 2.0
    trail(sc, r, L, 1700.0, Z, S)
    d = -1
    x, y = 300.0, 0.0
    m = car(sc, x, y, Z, 180.0, S, L, d * sp, phase=0.0, rate=rate, bob=-5.0)
    m.box(RED, (0, 0, 190), (290, 150, 140))
    m.box(GOLD, (0, 0, 128), (300, 156, 22))
    m.box(CREAM, (0, 0, 266), (320, 164, 14))
    m.box(INK, (-60, 0, 208), (56, 154, 56))
    m.box(INK, (40, 0, 208), (56, 154, 56))
    m.box(TEAL, (-70, 0, 290), (90, 100, 36))                # the luggage
    m.box(WOOD, (40, 20, 286), (70, 70, 28))
    m.box(WOOD, (190, 0, 150), (90, 130, 16))                # the box seat
    m.box(NAVY, (186, -26, 200), (30, 44, 66))               # the driver
    m.box(P.SKINS[3], (188, -26, 248), (28, 30, 30))
    m.box(CREAM, (188, -26, 266), (70, 70, 7))
    m.box(CREAM, (188, -26, 277), (32, 34, 18))
    m.box(BARK_D, (330, 0, 96), (240, 9, 9))                 # the tongue
    for wx, wr in ((-96, 80), (118, 62)):
        for side in (-1, 1):
            rod(m, YEL, (wx, side * 78, wr), (wx, side * 92, wr), wr, steps=8)
            m.box(RED, (wx, side * 94, wr), (wr * 0.6, 5, wr * 0.6))
    coats = ((WHITE, '#cfcfd8'), (INK, INK), ('#c94f2e', WHITE), (WHITE, INK))
    for k in range(4):
        row, side = k // 2, (-1 if k % 2 else 1)
        beast(sc, x + d * S * (400.0 + row * 250.0), y + side * S * 46.0, Z, yaw=180.0, s=S, coat=coats[k][0], mane=coats[k][1], tail=64, tail_w=18, tail_dip=10,
              neck_up=34, phase=k * 0.27, rate=rate, bob=30, stride=32, pitch=6, travel=(L, d * sp))
    beast(sc, x - d * S * 1100.0, 160.0, Z, yaw=180.0, s=S, coat=GOLD, mane=WHITE, tail=64, tail_w=18, tail_dip=10, neck_up=40, phase=0.5, rate=rate, bob=30,
          stride=32, pitch=6, travel=(L, d * sp), rider=hand_look(r, 3, 'hat', hat=WHITE, top=MAG))
    tv = tvl(L, d * sp)
    for k in range(3):
        puff(sc, x - d * S * (420.0 + k * 300.0), (k - 1) * 120.0, Z, 220 - k * 30, k * 0.3, 0.8, tv)


def handcar(sc):
    """A rail bed, and a hand car going +X along it: two people pumping the lever up and down between them, a dog
    after them, a flagman by the line swinging his flag."""
    r = random.Random(4210)
    S, Z, L, sp, rate = 2.2, 200.0, 7000.0, 520.0, 1.3
    g = sc.still()
    swell(g, r, L + 1200.0, 1500.0, Z, tones=(SANDST_D, SANDST, GRAVEL), top=0.5)
    gauge = 64.0 * S
    for k in range(18):
        g.box(BARK_D, (-L / 2.0 - 400.0 + k * (L + 800.0) / 17.0, 0, Z + 5), (50, gauge * 2 + 110, 10))
    for side in (-1, 1):
        g.box(STEEL, (0, side * gauge, Z + 15), (L + 900.0, 14, 12))
    x, y = 400.0, 0.0
    m = car(sc, x, y, Z, 0.0, S, L, sp, phase=0.0, rate=rate)
    m.box(RED, (0, 0, 54), (230, 150, 16))
    m.box(YEL, (0, 0, 44), (236, 154, 6))
    for wx in (-76, 76):
        for side in (-1, 1):
            rod(m, INK, (wx, side * 60, 28), (wx, side * 68, 28), 28, steps=8)
    tilt_box(m, INK, (-26, 0, 62), (0, 0, 156), 10)
    tilt_box(m, INK, (26, 0, 62), (0, 0, 156), 10)
    lv = car(sc, x, y, Z, 0.0, S, L, sp, phase=0.0, rate=rate, swing=17.0, pivot=(0, 0, 156), axis=90.0)
    lv.box(YEL, (0, 0, 156), (250, 10, 10))
    for ex in (-122, 122):
        lv.box(YEL, (ex, 0, 156), (8, 76, 8))
    for k, (ex, yaw, sw) in enumerate(((-96.0, 90.0, -17.0), (96.0, -90.0, 17.0))):
        P.figure(sc, x + S * ex, y, Z + S * 62.0, yaw=yaw, arm_l=((66, 80), (70, 86)), arm_r=((66, 80), (70, 86)), swing_l=sw, swing_r=sw, axis_l='x', axis_r='x',
                 legs='wide', phase=0.0, rate=rate, detail=1, travel=(L, sp),
                 **P.look(r, top=(CYAN, MAG)[k], bottom=DENIM, hat=('cap', 'cowboy')[k], hat_col=(RED, CREAM)[k], s=S))
    P.quadruped(sc, x - 1300.0, 0.0, Z + 10, yaw=90.0, s=S * 1.3, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=30, stride=34,
                rate=3.0, bob=10, phase=0.3, travel=(L, sp), body=GOLD, ears=INK, tail_col=WHITE, snout=WHITE)
    P.figure(sc, -1500.0, -430.0, Z, yaw=10, arm_r='up', swing_r=26, axis_r='y', hold_r=P.hold_flag(col=RED, pole=WHITE, w=70.0), arm_l='hip', rate=1.1,
             react=True, detail=1, **P.look(r, top=ORANGE, bottom=NAVY, hat='peak', hat_col=NAVY, s=S))


def land_yachts(sc):
    """Three land yachts heading +X over a strip of salt: three wheels, a pilot low in the pod, a tall two-coloured
    sail that heels to and fro as it runs. Each at its own speed."""
    r = random.Random(4211)
    S, Z, L = 2.0, 180.0, 9000.0
    trail(sc, r, L, 2600.0, Z, S, tones=(SANDST_D, '#f4e0c4', SALT), bushes=0)
    sails = ((MAG, WHITE), (CYAN, YEL), (YEL, RED))
    for i in range(3):
        x, y = (1800.0, -500.0, -2800.0)[i], (-350.0, 60.0, 400.0)[i]
        sp = (1500.0, 1750.0, 1350.0)[i]
        ph = i * 0.31
        m = car(sc, x, y, Z, 0.0, S, L, sp, phase=ph, rate=0.5, swing=7.0, pivot=(0, 0, 0), axis=0.0)
        m.box(INK, (10, 0, 36), (280, 14, 10))
        m.box(INK, (-100, 0, 36), (14, 210, 10))
        for side in (-1, 1):
            rod(m, INK, (-100, side * 100, 34), (-100, side * 112, 34), 34, steps=8)
        rod(m, INK, (140, -6, 30), (140, 6, 30), 30, steps=8)
        m.box(sails[i][0], (-10, 0, 60), (160, 50, 36))
        m.box(HOT[(i * 3 + 1) % 8], (-44, 0, 100), (28, 36, 46))             # the pilot
        m.box(P.SKINS[(i * 2) % 6], (-44, 0, 134), (24, 26, 24))
        m.box(WHITE, (-44, 0, 150), (28, 30, 12))
        m.box(CREAM, (34, 0, 310), (9, 9, 520))                              # the mast
        m.side(sails[i][0], [(28, 100), (28, 560), (-210, 100)], -3.0, 3.0)
        m.side(sails[i][1], [(29, 99), (29, 320), (-100, 99)], -4.5, 4.5)
        m.box(sails[i][1], (20, 0, 574), (40, 4, 18))                        # the pennant at the masthead
        puff(sc, x - S * 260.0, y, Z, 150, ph, 0.7, tvl(L, sp), col=SALT)


def round_pen(sc):
    """A round pen: two riders cantering round it nose to tail across the ring, the trainer turning in the middle with
    the lunge line out to the lead horse, three children sitting on the rail on the road's side, waving."""
    r = random.Random(4212)
    S, Z, R = 2.0, 150.0, 1200.0
    g = sc.still()
    g.cyl(SANDST_D, (0, 0, -150), R + 520.0, 150.0 + Z * 0.5, steps=12, top_r=R + 360.0)
    g.cyl(PALE, (0, 0, Z * 0.5), R + 300.0, Z * 0.5, steps=12, top_r=R + 200.0)
    n = 12
    pts = [(math.cos(2 * math.pi * k / n) * R, math.sin(2 * math.pi * k / n) * R) for k in range(n)]
    rail_z = Z + 62.0 * S
    for k in range(n):
        a, b = pts[k], pts[(k + 1) % n]
        g.box(WHITE, (a[0], a[1], Z + 34.0 * S), (9 * S, 9 * S, 68 * S))
        tilt_box(g, WHITE, (a[0], a[1], rail_z), (b[0], b[1], rail_z), 6 * S, 7 * S)
        tilt_box(g, RED, (a[0], a[1], Z + 30.0 * S), (b[0], b[1], Z + 30.0 * S), 5 * S, 6 * S)
    rr, turn, rate = R * 0.68, 0.085, 1.9
    coats = ((WHITE, '#cfcfd8', INK), (INK, GOLD, GOLD))
    for k, (hx, yaw) in enumerate(((rr, 180.0), (-rr, 0.0))):
        ph = k * 0.4
        P.quadruped(sc, hx, 0.0, Z, yaw=yaw, s=S, body=coats[k][0], mane=coats[k][1], tail_col=coats[k][2], ears=coats[k][0], stride=26, rate=rate, bob=14,
                    phase=ph, orbit=(0.0, 0.0, turn))
        P.figure(sc, hx, 0.0, Z + S * 78.0, yaw=yaw, legs='sit', arm_l='hold', arm_r=('wave' if k else 'hold'), swing_r=(26.0 if k else 0.0), axis_r='y',
                 phase=ph, rate=rate, bob=14, detail=1, orbit=(0.0, 0.0, turn),
                 **P.look(r, top=(MAG, CYAN)[k], bottom=DENIM, hat='cowboy', hat_col=(CREAM, RED)[k], s=S))

    def lunge(m, h, side):
        tilt_box(m, CREAM, h, (h[0], -(rr / S) + 60.0, 118.0), 4.5, 4.5)

    P.figure(sc, 0.0, 0.0, Z, yaw=90.0, arm_r='fwd', hold_r=lunge, arm_l='hip', detail=1, orbit=(0.0, 0.0, turn),
             **P.look(r, top=YEL, bottom=NAVY, hat='cowboy', hat_col=WHITE, s=S))
    for k, ang in enumerate((-112.0, -90.0, -68.0)):
        a = math.radians(ang)
        P.figure(sc, math.cos(a) * (R + 10.0), math.sin(a) * (R + 10.0), rail_z - 46.0 * S * 0.7, yaw=ang + 90.0, legs='sit', arm_r='wave', swing_r=26.0, axis_r='y',
                 arm_l=('wave' if k == 1 else 'down'), swing_l=(22.0 if k == 1 else 0.0), rate=1.3 + k * 0.2, phase=k * 0.3, react=True, detail=1,
                 **P.look(r, build='kid', s=S * 0.7, top=(LIME, ORANGE, CYAN)[k], hat=('cap', None, 'cowboy')[k], hat_col=(RED, RED, CREAM)[k]))


NEAR = dict(grow=(120.0, 80.0, 120.0), grow_down=(120.0, 80.0, 0.0))
MID = dict(grow=(200.0, 120.0, 200.0), grow_down=(200.0, 120.0, 0.0))
JOBS = [
    ('SM_P3C_Runners_A', runners_a, NEAR),
    ('SM_P3C_Runners_B', runners_b, NEAR),
    ('SM_P3C_Hikers', hikers, NEAR),
    ('SM_P3C_Cyclists_A', cyclists_a, NEAR),
    ('SM_P3C_Parade', parade, NEAR),
    ('SM_P3C_StripWalkers', strip_walkers, NEAR),
    ('SM_P3C_Cyclists_B', cyclists_b, MID),
    ('SM_P3C_MuleTrain', mule_train, MID),
    ('SM_P3C_Stagecoach', stagecoach, MID),
    ('SM_P3C_Handcar', handcar, MID),
    ('SM_P3C_LandYachts', land_yachts, MID),
    ('SM_P3C_RoundPen', round_pen, MID),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
