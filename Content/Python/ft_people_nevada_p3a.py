"""FULL THROTTLE (Unreal) the population of Nevada, pass 3, builder p3a: THE GAPS. /Game/People/Nevada/SM_P3A_*.

The general cast of Highway 93, for every stretch of road that had nobody within 60 m: people at work, at rest and
passing through, in pieces that fit any district. The land lies a metre under this road behind a metre of barrier,
so (as in ft_people_nevada.py) everybody is RAISED: on an earth bank, a timber deck or the canyon's rim. All TALL.

    on a bank or a deck, 30 m and more from the road
      SM_P3A_RoadCrew_A    a highway gang: one digging, a flagger with an orange flag, one leaning on a paddle, a barrow
      SM_P3A_RoadCrew_B    the gang's other half: a breaker hammering, two lifting a striped plank, the foreman, the water
      SM_P3A_Picnic        travellers resting: a blanket, a parasol, a cool box, a child hopping, the dog, one filming
      SM_P3A_Lookout       a scenic overlook's deck: the coin telescope, tourists pointing and photographing
      SM_P3A_FenceMenders  ranch hands mending fence: a hammer swinging, wire pulled, a horse grazing, a rider waving
      SM_P3A_Campers       a camp: a dome tent, a fire, a guitar, washing on a line, somebody waving a mug
      SM_P3A_Painters      painters at their easels, backs to the road, the canvases turned to it; one turns to wave
      SM_P3A_Tourists      a coach party behind their guide's striped paddle: a small crowd hopping out of step
    the canyon's rim (ft_people_nevada's 16.5 m column of the canyon's own rock: inside a rock cut its cap stands on
    the rim, in the open it is a hoodoo)
      SM_P3A_RimHikers     hikers who made the top: a tent, a flag, binoculars, one jumping
      SM_P3A_RimSurvey     surveyors on the rim: the instrument on its tripod, a striped pole, a big parasol
      SM_P3A_RimBighorns   bighorn sheep on the rim: one pacing the edge, one hopping, a lamb

Everything moves through M_People (ft_people.py). No words, no marks, no light on any piece. The placing rules are the
block C:/ftwork/p3/L2a.inl (pasted by the integrator at the end of FTPeopleL2.inl).
Run in the editor:  import ft_people_nevada_p3a; ft_people_nevada_p3a.run()
"""
import random
import ft_people as P
import ft_people_nevada as N
from ft_desert import rod, tilt_box

DIR = '/Game/People/Nevada'

SAND, SANDST, SANDST_D, SANDST_L = N.SAND, N.SANDST, N.SANDST_D, N.SANDST_L
WOOD, BARK, BARK_D, STEEL, CHROME = N.WOOD, N.BARK, N.BARK_D, N.STEEL, N.CHROME
CREAM, WHITE, GOLD, RED, TEAL, MAG, PURP, DENIM, LEATHER = N.CREAM, N.WHITE, N.GOLD, N.RED, N.TEAL, N.MAG, N.PURP, N.DENIM, N.LEATHER
MINT, PINK, LEMON, CYAN = N.MINT, N.PINK, N.LEMON, N.CYAN
ORANGE, HIVIS, SKY = '#ff7a3c', '#ffe14d', '#57c7ff'
RIM_H = N.RIM_H


def bank(g, r, L=1100.0, D=560.0, H=175.0):
    """An earth bank with a flat sand top at H. Returns the top's safe half sizes."""
    return N.rock_stack(g, r, L, D, H, tones=(SANDST_D, SANDST, SANDST_L), band=90.0, top=0.86, cap=SAND)


def cone(g, x, y, z):
    g.box(ORANGE, (x, y, z + 3), (34, 34, 6))
    g.cyl(ORANGE, (x, y, z + 6), 13, 30, steps=6, top_r=7)
    g.cyl(WHITE, (x, y, z + 36), 7, 10, steps=6, top_r=5)
    g.cyl(ORANGE, (x, y, z + 46), 5, 12, steps=6, top_r=3)


def hold_shovel():
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] - 22), (6, 6, 124))
        m.box(STEEL, (h[0], h[1] - 3, h[2] - 92), (24, 7, 32))
    return f


def hold_paddle(col=RED, bar=WHITE):
    """A flagger's paddle: a square of colour with a bar across it (never a word)."""
    def f(m, h, side):
        m.box(STEEL, (h[0], h[1], h[2] + 30), (5, 5, 110))
        m.box(col, (h[0], h[1], h[2] + 104), (52, 6, 52))
        m.box(bar, (h[0], h[1] - 3.5, h[2] + 104), (38, 2, 12))
    return f


def hold_hammer():
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 20), (5, 5, 52))
        m.box(STEEL, (h[0], h[1] - 4, h[2] + 46), (10, 26, 12))
    return f


def hold_brush(col=MAG):
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1], h[2] + 14), (4, 4, 36))
        m.box(col, (h[0], h[1], h[2] + 36), (6, 6, 10))
    return f


def hold_palette():
    def f(m, h, side):
        m.box(WOOD, (h[0], h[1] - 8, h[2] + 6), (40, 28, 4))
        for k, c in enumerate((RED, GOLD, TEAL)):
            m.box(c, (h[0] - 12 + k * 12, h[1] - 8, h[2] + 9), (8, 8, 3))
    return f


def hold_binoculars():
    def f(m, h, side):
        m.box(LEATHER, (h[0] - side * 8, h[1] - 6, h[2] + 8), (28, 18, 12))
    return f


def worker(r, s=1.18, **fixed):
    """Highway gang: hi-vis top with a pale band, a hard hat."""
    d = P.look(r, tops=(HIVIS, ORANGE, HIVIS, '#b8ff5a'), bottoms=(DENIM, '#5a4a3c', N.NAVY), hat='hard',
               hat_col=r.choice((WHITE, HIVIS, ORANGE)), band=WHITE, shoes='#5a4a3c')
    d['s'] = s * r.uniform(0.96, 1.05)
    d['detail'] = 1
    d.update(fixed)
    return d


# ------------------------------------------------------------------------------------------ people at work
def road_crew(seed, kind):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        H = 175.0
        hx, hy = bank(g, r, 1150, 560, H)
        for k in range(5):
            cone(g, -hx + 30 + k * (2 * hx - 60) / 4.0, -hy + 18, H)
        if kind == 0:
            # a heap of fill and the hole beside it
            g.sphere(SANDST, (-150, 40, H + 10), 70, scale=(1.3, 1, 0.6), steps=(4, 6))
            g.box(SANDST_D, (-40, 30, H + 1), (90, 70, 3))
            P.figure(sc, -45, -30, z=H, yaw=20, legs='wide', arm_l='carry', arm_r='carry', hold_r=hold_shovel(), swing_l=24, swing_r=24,
                     axis_l='x', axis_r='x', rate=0.8, phase=0.1, **worker(r, build='stocky'))
            # the flagger: the orange flag swung wide
            P.figure(sc, -290, -60, z=H, yaw=-14, arm_r='wave', hold_r=P.hold_flag(col=ORANGE, w=90), swing_r=30, arm_l='hip', rate=1.1, phase=0.4,
                     react=True, bob=3, **worker(r))
            # leaning on the paddle, a wave of the free hand
            P.figure(sc, 300, -70, z=H, yaw=12, arm_l='hold', hold_l=hold_paddle(), arm_r='wave', swing_r=22, rate=0.9, phase=0.7, 
                     **worker(r, hair='long'))
            # the barrow, wheeled the length of the bank and round again
            tl = 2 * hx - 120
            P.figure(sc, 0, 95, z=H, yaw=90, legs='stride', arm_l='push', arm_r='push', stride=24, rate=1.6, bob=2, phase=0.2, travel=(tl, 110),
                     **worker(r))
            b = sc.part(P.motion(phase=0.2, rate=1.6, bob=2, base=(0, 95), travel=(tl, 110.0 / tl), pivot=(60, 95, H + 40)))
            b.box(TEAL, (78, 95, H + 52), (70, 54, 30))
            b.cyl(LEATHER, (118, 95 - 5, H + 18), 18, 10, rot=(90, 0, 0), steps=8)
            b.sphere(SANDST, (78, 95, H + 70), 26, scale=(1.2, 1, 0.6), steps=(3, 6))
            tilt_box(b, STEEL, (30, 95, H + 78), (70, 95, H + 44), 5, 40)
        else:
            # the breaker: a short hard hammering
            br = sc.part(P.motion(bob=5, rate=2.6, phase=0.0))
            br.box(STEEL, (-250, -50, H + 50), (9, 9, 100))
            br.box(ORANGE, (-250, -50, H + 104), (46, 12, 14))
            P.figure(sc, -250, -18, z=H, yaw=0, legs='wide', arm_l='carry', arm_r='carry', bob=5, rate=2.6, phase=0.0,
                     **worker(r, build='stocky', hat_col=ORANGE))
            # two lifting a striped plank on to its trestles
            pl = sc.part(P.motion(bob=16, rate=0.5, phase=0.3))
            pl.box(WHITE, (60, -70, H + 118), (300, 8, 24))
            for k in range(5):
                pl.box(RED, (60 - 120 + k * 60, -74.5, H + 118), (30, 2, 24))
            g = sc.still()
            for sx in (-1, 1):
                tilt_box(g, STEEL, (60 + sx * 170 - 16, 10, H), (60 + sx * 170, 10, H + 80), 6, 6)
                tilt_box(g, STEEL, (60 + sx * 170 + 16, 10, H), (60 + sx * 170, 10, H + 80), 6, 6)
            for sx in (-1, 1):
                P.figure(sc, 60 + sx * 120, -40, z=H, yaw=sx * -8, arm_l='carry', arm_r='carry', bob=16, rate=0.5, phase=0.3,
                         **worker(r, hair=('pony', 'short')[sx > 0]))
            # the foreman: pointing down the road, a board under the arm
            P.figure(sc, 320, 40, z=H, yaw=30, arm_l='point', swing_l=12, axis_l='x', arm_r='hold', hold_r=P.hold_box(col=WHITE, w=30), rate=0.7,
                     phase=0.5, **worker(r, hat_col=WHITE, top=WHITE, band=ORANGE))
            # the water: a cooler on a trestle, one drinking
            g = sc.still()
            g.box(WOOD, (-60, 120, H + 36), (90, 44, 8))
            for sx in (-1, 1):
                g.box(WOOD, (-60 + sx * 36, 120, H + 16), (8, 40, 32))
            g.cyl(SKY, (-60, 120, H + 40), 20, 44, steps=8)
            g.cyl(WHITE, (-60, 120, H + 84), 21, 6, steps=8)
            P.figure(sc, -150, 110, z=H, yaw=-30, arm_r='face', hold_r=P.hold_cup(WHITE), swing_r=8, axis_r='x', arm_l='hip', rate=0.5, phase=0.8,
                     **worker(r, hat_col=HIVIS))
    return build


def fence_menders(sc):
    r = random.Random(207)
    g = sc.still()
    H = 175.0
    hx, hy = bank(g, r, 1250, 580, H)
    fy = -hy + 24
    n = 6
    for k in range(n):
        x = -hx + 20 + k * (2 * hx - 40) / (n - 1.0)
        lean = 14 if k == 3 else 0
        tilt_box(g, BARK, (x, fy, H), (x + lean, fy, H + 118), 12, 12)
    for z in (H + 50, H + 96):
        g.box(STEEL, (-hx * 0.42, fy, z), (2 * hx * 0.56, 4, 4))
    # hammering the leaning post back
    px = -hx + 20 + 3 * (2 * hx - 40) / (n - 1.0)
    P.figure(sc, px - 50, fy + 60, z=H, yaw=-70, legs='wide', arm_r=((120, 60), (150, 70)), hold_r=hold_hammer(), swing_r=40, axis_r='x',
             arm_l='hip', rate=1.2, phase=0.1, **N.cowhand(r, 1.18, top=RED))
    # hauling the wire tight: leaning back and forth on it
    g = sc.still()
    g.cyl(STEEL, (px + 190, fy + 70, H), 26, 22, steps=8)
    w = sc.part(P.motion(swing=5, pivot=(px + 120, fy + 40, H), axis=0.0, rate=0.6, phase=0.4))
    tilt_box(w, STEEL, (px + 20, fy, H + 96), (px + 120, fy + 36, H + 104), 4, 4)
    P.figure(sc, px + 120, fy + 60, z=H, yaw=-60, legs='stride', arm_l='fwd', arm_r='fwd', rock=6, rate=0.6, phase=0.4,
             **N.cowhand(r, 1.18, top=SKY))
    # the boss in the saddle, hat in the air
    d = N.cowhand(r, 1.15, top=GOLD)
    d.update(arm_r='wave', hold_r=N.hold_hat(d['hat_col']), hat=None, swing_r=28)
    N.mount(sc, r, -250, 70, H, yaw=-35, s=1.25, coat='#e8c26a', mane=WHITE, rider=d, phase=0.3, rate=0.8, graze=6, neck_up=50)
    # the mender's own horse, tied and grazing; the dog
    P.quadruped(sc, 340, 110, H, yaw=60, s=1.2, body='#5e3b28', mane='#22202a', neck_up=-25, graze=10, wag=20, rate=0.5, phase=0.6)
    P.quadruped(sc, 130, 150, H, yaw=-20, s=1.2, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=34, rate=2.2,
                body=WHITE, belly=CREAM, ears=LEATHER)
    g = sc.still()
    N.sage(g, r, hx - 50, hy - 50, H, 0.9)


# ------------------------------------------------------------------------------------------ people at rest
def picnic(sc):
    r = random.Random(211)
    g = sc.still()
    H = 175.0
    hx, hy = bank(g, r, 1150, 580, H)
    # the blanket: a chequer of two colours
    for i in range(4):
        for j in range(3):
            g.box((RED, WHITE)[(i + j) % 2], (-200 + i * 70, -40 + j * 70, H + 2), (70, 70, 4))
    g.box(SKY, (60, 60, H + 22), (70, 46, 44))
    g.box(WHITE, (60, 60, H + 47), (74, 50, 8))
    N.parasol(g, 130, 110, H, (LEMON, MAG), r=160, h=270)
    # two on the blanket: one waving a sandwich, one pointing at the chase
    P.figure(sc, -150, 20, z=H, yaw=-20, legs='sit', seat=12, arm_r='wave', swing_r=20, arm_l='down', rate=0.9, phase=0.1,
             **N.person(r, 1.18, top='#ffc93c', hat='sun', hat_col=PINK))
    P.figure(sc, -20, 40, z=H, yaw=18, legs='sit', seat=12, arm_l='point', swing_l=10, axis_l='x', arm_r='face', hold_r=P.hold_cup(), rate=0.7,
             phase=0.5, **N.person(r, 1.18, top='#2ee6ff', hair='afro'))
    # filming it all
    P.figure(sc, 290, -60, z=H, yaw=14, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=5, rate=0.4, phase=0.3,
             **N.person(r, 1.2, top='#b8ff5a', wear='shorts', hat='cap', hat_col=RED))
    # the child and the dog, both beside themselves
    P.figure(sc, -300, -90, z=H, yaw=-10, arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=14, rate=1.9, phase=0.2, react=True,
             **N.person(r, build='kid', s=0.8, top='#ff5fa2'))
    P.quadruped(sc, -330, 60, H, yaw=-40, s=1.25, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=36, rate=2.4,
                bob=5, phase=0.6, body='#e8c26a', belly=CREAM)
    # one standing, stretching both arms up at the cameras
    P.figure(sc, 150, -100, z=H, yaw=-6, arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=6, rate=1.2, phase=0.8, react=True,
             **N.person(r, 1.2, top='#8f6bff', hair='long'))
    g = sc.still()
    N.sage(g, r, hx - 40, hy - 40, H, 0.9)


def lookout(sc):
    r = random.Random(223)
    g = sc.still()
    H = 200.0
    N.deck(g, 900, 340, H, top=WOOD, post=BARK_D, rail=CREAM, skirt=TEAL, stripe=LEMON)
    # the coin telescope on its post
    g.box(STEEL, (-250, -110, H + 55), (12, 12, 110))
    tel = sc.part(P.motion(swing=18, pivot=(-250, -110, H + 110), axis=P.UP, rate=0.25, phase=0.1))
    rod(tel, CHROME, (-250, -80, H + 124), (-250, -160, H + 132), 11, steps=7)
    P.figure(sc, -250, -40, z=H, yaw=0, arm_l='hold', arm_r='hold', sway=dict(swing=18, pivot=(-250, -110, H), axis=P.UP), rate=0.25, phase=0.1,
             **N.person(r, 1.15, top='#ff5fa2', hat='sun', hat_col=LEMON))
    # pointing; photographing; a couple with a phone held out; the child on tiptoe
    P.figure(sc, -90, -90, z=H, yaw=-18, arm_r='point', swing_r=12, axis_r='x', arm_l='hip', rate=0.8, phase=0.3,
             **N.person(r, 1.15, top='#ffc93c', build='stocky', hat='cap', hat_col=TEAL))
    P.figure(sc, 70, -100, z=H, yaw=8, arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=5, rate=0.5, phase=0.6,
             **N.person(r, 1.15, top='#2ee6ff', wear='shorts', hair='pony'))
    P.figure(sc, 230, -80, z=H, yaw=170, arm_r='wave', hold_r=P.hold_phone(), swing_r=6, arm_l='hip', rate=0.6, phase=0.2,
             **N.person(r, 1.15, top='#7dffb0', hair='long'))
    P.figure(sc, 300, -70, z=H, yaw=165, arm_l='wave', swing_l=24, rate=1.0, phase=0.7, **N.person(r, 1.15, top='#8f6bff', hair='bald'))
    P.figure(sc, 380, -110, z=H, yaw=-10, arm_l='vee', arm_r='vee', bob=12, rate=1.8, phase=0.4, react=True,
             **N.person(r, build='kid', s=0.8, top=RED))
    # one walking the deck's length behind them and round again
    P.figure(sc, 0, 70, z=H, yaw=90, legs='stride', stride=24, swing_l=18, swing_r=-18, axis_l='x', axis_r='x', rate=1.4, bob=2, travel=(780, 110),
             pack=ORANGE, **N.person(r, 1.15, top=WHITE, hat='cowboy', hat_col=CREAM))
    N.flag(sc, 440, 160, H, 420, MAG, patch=LEMON, w=180, h=110, pole=CREAM, phase=0.3)


def campers(sc):
    r = random.Random(229)
    g = sc.still()
    H = 175.0
    hx, hy = bank(g, r, 1200, 600, H)
    # the dome tent and its door
    g.sphere(TEAL, (-230, 100, H), 135, scale=(1.25, 1.0, 0.85), steps=(4, 8))
    g.box(LEMON, (-230, -34, H + 46), (70, 6, 92))
    # the fire: a ring of stones, logs, a flame that floats (no light: 24 m and more, and still none)
    for k in range(6):
        g.box(STEEL, (100 + (k % 3 - 1) * 34, -30 + (k // 3) * 60 - 30, H + 7), (26, 22, 14))
    tilt_box(g, BARK, (70, -30, H + 8), (130, -40, H + 30), 12, 12)
    tilt_box(g, BARK, (130, -20, H + 8), (72, -44, H + 30), 12, 12)
    fl = sc.part(P.motion(bob=-7.0, rate=1.3, phase=0.2))
    fl.cyl(ORANGE, (100, -34, H + 22), 20, 50, steps=6, top_r=3)
    fl.cyl(GOLD, (100, -34, H + 22), 11, 34, steps=5, top_r=2)
    # the washing line: two posts, the line, four things flapping on it
    lx0, lx1, ly, lz = 200.0, hx - 10, 150.0, H + 190.0
    for x in (lx0, lx1):
        g.box(BARK, (x, ly, H + 100), (9, 9, 200))
    g.box(CREAM, ((lx0 + lx1) / 2.0, ly, lz), (lx1 - lx0, 4, 4))
    cols = (RED, WHITE, SKY, LEMON)
    for k in range(4):
        cx = lx0 + (k + 0.5) * (lx1 - lx0) / 4.0
        c = sc.part(P.motion(swing=16.0, pivot=(cx, ly, lz), axis=0.0, rate=0.8, phase=0.23 * k))
        c.box(cols[k], (cx, ly, lz - 38), ((lx1 - lx0) / 4.0 - 14, 4, 72))
    # the guitar, on a log; two more by the fire; one up and waving a mug
    g.box(BARK_D, (-40, 40, H + 18), (150, 36, 36))
    def guitar(m, h, side):
        if side > 0:
            m.box(WOOD, (h[0] - 26, h[1] - 8, h[2] - 4), (44, 12, 34))
            tilt_box(m, BARK_D, (h[0] - 44, h[1] - 8, h[2]), (h[0] - 96, h[1] - 8, h[2] + 26), 7, 7)
    P.figure(sc, -70, 40, z=H, yaw=20, legs='sit', seat=36, arm_l='carry', arm_r='carry', hold_r=guitar, rock=7, rate=0.9, phase=0.1,
             **N.person(r, 1.18, top='#ff5fa2', hair='long', hat='cowboy', hat_col=CREAM))
    P.figure(sc, 10, 44, z=H, yaw=-25, legs='sit', seat=36, arm_l='up', arm_r='up', swing_l=14, swing_r=-14, rate=1.0, phase=0.5, react=True,
             **N.person(r, 1.18, top='#ffc93c', hair='bun'))
    P.figure(sc, 250, -70, z=H, yaw=20, arm_r='wave', hold_r=P.hold_cup(RED), swing_r=24, arm_l='hip', bob=4, rate=1.1, phase=0.7, react=True,
             **N.person(r, 1.2, top='#2ee6ff', hat='beanie', hat_col=MAG, wear='shorts'))
    P.figure(sc, -330, -120, z=H, yaw=-12, arm_l='face', hold_l=hold_binoculars(), arm_r='point', swing_r=10, axis_r='x', rate=0.6, phase=0.3,
             **N.person(r, 1.2, top='#b8ff5a', pack=PURP))
    g = sc.still()
    g.cyl(STEEL, (150, -80, H), 14, 20, steps=7)
    N.sage(g, r, -hx + 40, hy - 40, H, 0.9)


def painters(sc):
    r = random.Random(233)
    g = sc.still()
    H = 175.0
    hx, hy = bank(g, r, 1200, 580, H)
    pics = ((SKY, ORANGE, SAND), (PINK, PURP, GOLD), (LEMON, RED, TEAL))
    xs = (-260, 20, 290)
    for i, x in enumerate(xs):
        y = 70 + (i % 2) * 30
        # the easel: three legs and the canvas, its painted face to the road
        tilt_box(g, WOOD, (x - 36, y, H), (x, y + 8, H + 190), 6, 6)
        tilt_box(g, WOOD, (x + 36, y, H), (x, y + 8, H + 190), 6, 6)
        tilt_box(g, WOOD, (x, y + 60, H), (x, y + 8, H + 190), 6, 6)
        g.box(WHITE, (x, y - 4, H + 130), (104, 5, 84))
        sky, land, sun = pics[i]
        g.box(sky, (x, y - 7.5, H + 150), (96, 2, 36))
        g.box(land, (x, y - 7.5, H + 112), (96, 2, 40))
        g.box(sun, (x + 22 - i * 20, y - 8.5, H + 142), (40, 2, 20))
        g.box(SAND, (x - 20 + i * 14, y - 8.5, H + 128), (24, 2, 26))
    # two painting (backs to the road), a dab at a time; the third has turned round to wave the brush
    P.figure(sc, xs[0] + 8, -10, z=H, yaw=180, arm_r='fwd', hold_r=hold_brush(MAG), swing_r=12, axis_r='y', arm_l='hip', rate=0.8, phase=0.1,
             **N.person(r, 1.18, top=WHITE, hat='sun', hat_col=LEMON, wear='dress', bottom=PINK))
    P.figure(sc, xs[1] + 8, 20, z=H, yaw=176, legs='sit', seat=46, arm_r='fwd', hold_r=hold_brush(TEAL), swing_r=10, axis_r='x', arm_l='down',
             rate=0.6, phase=0.5, **N.person(r, 1.18, top='#8f6bff', hat='cap', hat_col=CREAM, build='stocky'))
    g = sc.still()
    N.stool(g, xs[1] + 8, 20, H, col=TEAL)
    P.figure(sc, xs[2] - 70, -60, z=H, yaw=-12, arm_r='wave', hold_r=hold_brush(RED), swing_r=28, arm_l='hold', hold_l=hold_palette(),
             bob=4, rate=1.1, phase=0.3, react=True, **N.person(r, 1.2, top='#ff5fa2', hair='bun', sleeves='long'))
    # somebody looking over a shoulder, and the model: a dog told to sit, wagging
    P.figure(sc, -110, 30, z=H, yaw=150, arm_l='hip', arm_r='point', swing_r=8, axis_r='x', rate=0.5, phase=0.7, 
             **N.person(r, 1.18, top='#ffc93c', wear='shorts', hair='afro'))
    P.quadruped(sc, 420, -90, H, yaw=30, s=1.25, length=70, height=48, girth=20, neck=16, neck_up=50, head=18, tail=22, wag=36, rate=2.0,
                phase=0.2, body=LEATHER, belly=WHITE)
    g = sc.still()
    N.parasol(g, -400, 120, H, (CREAM, TEAL), r=150, h=280)
    g.box(WOOD, (160, -110, H + 14), (60, 40, 28))


# ------------------------------------------------------------------------------------------ people passing through
def tourists(sc):
    r = random.Random(239)
    g = sc.still()
    H = 175.0
    hx, hy = bank(g, r, 1250, 600, H)
    # a rope on posts along the front: they have been told to stay behind it
    fy = -hy + 20
    for k in range(5):
        g.box(CREAM, (-hx + 20 + k * (2 * hx - 40) / 4.0, fy, H + 45), (9, 9, 90))
    g.box(PURP, (0, fy, H + 84), (2 * hx - 40, 5, 5))
    # the guide: the striped paddle held high, the other arm sweeping at the view
    P.figure(sc, -hx + 90, -40, z=H, yaw=-40, arm_l='up', hold_l=hold_paddle(TEAL, LEMON), swing_l=8, arm_r='out', swing_r=16, axis_r='z', rate=0.6,
             phase=0.1, **N.person(r, 1.22, top=RED, bottom=N.NAVY, hat='peak', hat_col=RED, sleeves='long', band=GOLD))
    acts = [dict(arm_l='hold', arm_r='hold', hold_r=P.hold_camera(), rock=4, rate=0.5),
            dict(arm_r='wave', swing_r=24, rate=1.1, bob=4, react=True),
            dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=9, rate=1.5, react=True),
            dict(arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.7),
            dict(arm_r='face', hold_r=hold_binoculars(), arm_l='face', rock=5, rate=0.4),
            dict(arm_l='wave', arm_r='wave', swing_l=20, swing_r=-20, bob=6, rate=1.3, react=True)]
    hats = (('sun', LEMON), ('cap', TEAL), (None, RED), ('sun', PINK), ('cap', WHITE), ('cowboy', CREAM), (None, RED), ('sun', MINT))
    n = 8
    for i in range(n):
        x = -hx + 230 + i * (2 * hx - 300) / (n - 1.0) + r.uniform(-14, 14)
        y = (-70 if i % 2 == 0 else 40) + r.uniform(-12, 12)
        d = N.person(r, 1.16, hat=hats[i][0], hat_col=hats[i][1], detail=(1 if i % 2 == 0 else 0))
        if i == 5:
            d.update(build='kid', s=0.8)
        d.update(acts[i % len(acts)])
        P.figure(sc, x, y, z=H, yaw=r.uniform(-16, 16), phase=r.random(), **d)
    N.flag(sc, hx - 30, hy - 40, H, 400, LEMON, patch=TEAL, w=170, h=100, pole=CREAM, phase=0.6)


# ------------------------------------------------------------------------------------------ the canyon's rim
def rim_hikers(sc):
    r = random.Random(241)
    g = sc.still()
    hx, hy = N.rim_column(g, r, 1500, 480)
    H = RIM_H
    S = 2.0
    g.sphere(MAG, (hx - 190, 40, H), 150, scale=(1.3, 1.0, 0.85), steps=(4, 8))
    g.box(LEMON, (hx - 190, -112, H + 50), (80, 6, 100))
    # a cairn at the other end
    for k, (w, z) in enumerate(((110, 22), (84, 62), (58, 96), (36, 124))):
        g.box((SANDST_L, SANDST, SANDST_D, SANDST_L)[k], (-hx + 80, 60, H + z), (w, w * 0.9, 40))
    P.figure(sc, -hx + 250, -20, z=H, yaw=-10, arm_r='up', hold_r=P.hold_flag(col=CYAN, w=110), swing_r=16, arm_l='hip', bob=4, rate=0.9, phase=0.1,
             pack=ORANGE, **N.person(r, S, top=RED, hat='cap', hat_col=WHITE, wear='shorts', detail=0))
    P.figure(sc, -hx + 520, 10, z=H, yaw=6, arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, bob=22, rate=1.3, phase=0.5, react=True,
             pack=TEAL, **N.person(r, S, top=LEMON, hair='pony', detail=0))
    P.figure(sc, -hx + 790, -30, z=H, yaw=-4, arm_l='face', arm_r='face', hold_r=hold_binoculars(), rock=6, rate=0.4, phase=0.3,
             pack=PURP, **N.person(r, S, top='#7dffb0', hat='sun', hat_col=CREAM, detail=0))
    P.figure(sc, -hx + 1010, 20, z=H, yaw=12, legs='sit', seat=40, arm_r='wave', swing_r=24, arm_l='down', rate=1.0, phase=0.8,
             **N.person(r, S, top=PINK, hat='beanie', hat_col=TEAL, detail=0))
    g = sc.still()
    g.box(SANDST_D, (-hx + 1010, 30, H + 36), (130, 110, 72))


def rim_survey(sc):
    r = random.Random(251)
    g = sc.still()
    hx, hy = N.rim_column(g, r, 1300, 480)
    H = RIM_H
    S = 2.0
    # the instrument on its tripod (thick legs: it is read from 60 m below)
    tx, ty = -120.0, -40.0
    for (ax, ay) in ((-70, -40), (70, -40), (0, 80)):
        tilt_box(g, HIVIS, (tx + ax, ty + ay, H), (tx, ty, H + 250), 12, 12)
    ins = sc.part(P.motion(swing=24, pivot=(tx, ty, H + 250), axis=P.UP, rate=0.2, phase=0.1))
    ins.box(TEAL, (tx, ty, H + 272), (64, 44, 44))
    rod(ins, CHROME, (tx, ty + 40, H + 280), (tx, ty - 56, H + 280), 13, steps=7)
    N.parasol(g, tx + 40, ty + 150, H, (ORANGE, WHITE), r=260, h=470)
    P.figure(sc, tx, ty + 110, z=H, yaw=0, arm_l='hold', arm_r='hold', sway=dict(swing=24, pivot=(tx, ty, H), axis=P.UP), rate=0.2, phase=0.1,
             **worker(r, S, detail=0))
    # the staff: a tall pole in bands of red and white, held upright and walked a pace to and fro
    sx = hx - 150
    pole = sc.part(P.motion(swing=1.2, pivot=(sx, -30, H - 3000.0), axis=90.0, rate=0.3, phase=0.4))
    for k in range(6):
        pole.box((RED, WHITE)[k % 2], (sx - 60, -50, H + 40 + k * 80), (14, 14, 80))
    P.figure(sc, sx, -30, z=H, yaw=-10, arm_l='fwd', arm_r='hip', sway=dict(swing=1.2, pivot=(sx, -30, H - 3000.0), axis=90.0), rate=0.3, phase=0.4,
             **worker(r, S, detail=0, hat_col=ORANGE))
    # the one who is meant to be writing it down, waving both arms at the chase
    P.figure(sc, -hx + 170, -10, z=H, yaw=-14, arm_l='vee', arm_r='vee', swing_l=18, swing_r=-18, bob=12, rate=1.2, phase=0.6, react=True,
             **worker(r, S, detail=0, hair='long'))
    g = sc.still()
    g.box(ORANGE, (-hx + 330, 70, H + 30), (120, 80, 60))


def rim_bighorns(sc):
    r = random.Random(257)
    g = sc.still()
    hx, hy = N.rim_column(g, r, 1500, 460)
    H = RIM_H
    kw = dict(length=96, height=72, girth=36, neck=22, head=26, tail=10, body='#b89a78', belly=CREAM, ears='#8a7460', snout=CREAM, legs_col='#8a7460')
    # the ram, head up, turning it slowly; one pacing the edge; one on its hind springs; the lamb
    P.quadruped(sc, -hx + 200, 10, H, yaw=-60, s=3.2, neck_up=62, horns=CREAM, graze=8, wag=12, rate=0.4, phase=0.0, **kw)
    P.quadruped(sc, 0, -50, H, yaw=90, s=2.8, neck_up=35, horns=CREAM, stride=22, wag=10, rate=1.4, bob=3, phase=0.3, travel=(2 * hx - 120, 150), **kw)
    P.quadruped(sc, hx - 330, 50, H, yaw=-110, s=2.6, neck_up=48, horns=CREAM, bob=26, rate=0.9, phase=0.5, wag=18, **kw)
    P.quadruped(sc, hx - 120, -20, H, yaw=-80, s=1.7, neck_up=50, bob=16, rate=1.6, phase=0.1, wag=30, **kw)
    g = sc.still()
    N.sage(g, r, -hx + 60, hy - 40, H, 1.2)
    g.sphere(SANDST_L, (hx - 60, hy - 60, H + 20), 60, scale=(1.2, 1, 0.7), steps=(4, 6))


JOBS = [
    ('SM_P3A_RoadCrew_A', road_crew(301, 0)),
    ('SM_P3A_RoadCrew_B', road_crew(302, 1)),
    ('SM_P3A_Picnic', picnic),
    ('SM_P3A_Lookout', lookout),
    ('SM_P3A_FenceMenders', fence_menders),
    ('SM_P3A_Campers', campers),
    ('SM_P3A_Painters', painters),
    ('SM_P3A_Tourists', tourists),
    ('SM_P3A_RimHikers', rim_hikers, dict(grow=(80.0, 80.0, 120.0))),
    ('SM_P3A_RimSurvey', rim_survey, dict(grow=(80.0, 80.0, 80.0))),
    ('SM_P3A_RimBighorns', rim_bighorns, dict(grow=(120.0, 80.0, 120.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
