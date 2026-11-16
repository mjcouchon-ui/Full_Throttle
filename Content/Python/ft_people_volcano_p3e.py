"""FULL THROTTLE (Unreal) the Volcano, population pass 3, builder p3e: FAR AND SKY. /Game/People/Volcano/SM_P3E_*.

What the first two rounds left free past 150 m and over it (they have the skeins of cranes and herons, the vultures'
kettle, the goats' crag, the ridge line, the rim survey, the caravans, herds and camps):

  the air    SM_P3E_Balloons_A   the survey's hot-air balloons riding the mountain's thermal: three, circling slowly
             SM_P3E_Balloons_B   three more, drifting down the wind in a line and coming round again
             SM_P3E_Airship      the film unit's camera airship crossing along the road, a pink-and-white streamer behind
             SM_P3E_Gliders_A    hang gliders wheeling on a thermal (eight delta wings, a pilot under each)
             SM_P3E_Gliders_B    paragliders in a string along the slope, canopies in two colours
  the slopes SM_P3E_Kites        the village children's signal kites flown from the head of a basalt stack
             SM_P3E_Climbers     a rope team on an obsidian needle 74 m tall, a flag waved on the summit, one abseiling
             SM_P3E_FarCrowd_A   the refuge terraces: three stepped rows of villagers, hopping and waving, banners
             SM_P3E_FarCrowd_B   the ring dance on a mesa's head round a ribboned pole, drummers, onlookers on a ledge
             SM_P3E_Cableway     the evacuation cableway: two pylons 52 m tall, six open cabins of people crossing

Every piece is saved with the always-moving master (far=True), is built four to five times life size (it stands 150 m
and more from the road) and carries a little steady light of its own (the slopes are dark; nothing blinks).
Nothing here is red and nothing is a road vehicle (the boss of the level is ENGINE 13, a red fire engine).

Run in the editor:  import ft_people_volcano_p3e as K; K.run()        K.run(['SM_P3E_Airship'])
Placed by the block C:/ftwork/p3/L3e.inl (pasted by the integrator at the end of Source/FullThrottle/FTPeopleL3.inl).
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import tilt_box

DIR = '/Game/People/Volcano'
BAS, BAS_L, BAS_D, ASHCAP, OBS = '#3e3244', '#5a4e5e', '#2c2433', '#b0a088', '#4a3a6a'
WHITE, DARK, YEL, PINK, CYAN, LIME, VIOLET, MINT, ORANGE, BLUE = ('#fff6ea', '#22202a', '#ffc93c', '#ff5fa2', '#2ee6ff', '#b8ff5a',
                                                                  '#8f6bff', '#7dffb0', '#ff7a3c', '#3a5fa8')
WOOD, PLANK, STEEL, STEEL_D = '#c9a070', '#e8d8b8', '#8f9bb3', '#5a5870'
CLOTH = ('#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#fff6ea', '#8f6bff', '#57c7ff', '#f4a6ff', '#b8ff5a')
ROOT = -2500.0          # how far a standing piece is rooted into the slope it is put on


def tv(length, speed):
    """A travel for a part drawn by hand (figure() takes (length, cm/s) itself)."""
    return (float(length), float(speed) / float(length))


def lifted(fn, g=0.12):
    """A far piece's colours given a little steady light of their own (parts that glow already keep theirs)."""
    def build(sc):
        fn(sc)
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
    return build


def villager(r, **fixed):
    d = P.look(r, tops=CLOTH, wear=r.choice(('trousers', 'dress', 'skirt', 'coat', 'trousers')),
               hat=r.choice((None, 'sun', 'cap', 'beanie')), hat_col=r.choice(CLOTH), sleeves=r.choice(('short', 'long')))
    d.update(fixed)
    return d


def blob(sc, r, x, y, z, S, mo, yaw=0.0, arms=True, legs=True, top=None, hat=None):
    """A far person in five or six boxes (48 to 72 triangles): legs, a bright body, a head, hair or a hat, two arms
    thrown up. Drawn into the part that moves as `mo` says, so people who share a motion share a part."""
    m = sc.part(mo, (x, y, z), yaw, S)
    top = top or r.choice(CLOTH)
    if legs:
        m.box(r.choice(P.BOTTOMS), (0, 0, 33), (36, 26, 66))
        m.box(top, (0, 0, 96), (48, 28, 60))
    else:
        m.box(top, (0, 0, 63), (48, 28, 126))
    m.box(r.choice(P.SKINS), (0, 0, 148), (40, 38, 44))
    m.box(hat or r.choice(P.HAIRS), (0, 3, 175), (46, 44, 14))
    if arms:
        for side in (-1, 1):
            tilt_box(m, top, (side * 28, 0, 120), (side * 54, 0, 178), 13, 13)
    return m


def column(g, x, y, rad, top, k=0, root=ROOT, cap=True):
    g.cyl((BAS, BAS_L, BAS_D)[k % 3], (x, y, root), rad, top - root, steps=6)
    if cap:
        g.cyl(ASHCAP, (x, y, top), rad * 0.92, 10, steps=6)


# ------------------------------------------------------------------------------------------ balloons
def balloon(sc, r, x, y, z, R, c1, c2, c3, common, fig, S=3.0):
    """A hot-air balloon whose envelope's middle is at (x, y, z): envelope R across the half, two bands, the throat, a
    burner alight (a steady light), a basket on four lines and two people in it. common: the motion of every part;
    fig: the same motion in figure()'s own words."""
    m = sc.part(P.motion(pivot=(x, y, z), **common))
    m.base = None
    m.sphere(c1, (x, y, z), R, scale=(1, 1, 1.08), steps=(6, 10))
    m.cyl(c2, (x, y, z - 0.18 * R), 1.015 * R, 0.36 * R, steps=10)
    m.cyl(c3, (x, y, z + 0.5 * R), 0.9 * R, 0.22 * R, steps=10, top_r=0.76 * R)
    m.cyl(c1, (x, y, z - 1.5 * R), 0.2 * R, 0.95 * R, steps=10, top_r=0.84 * R)
    m.cyl(c2, (x, y, z - 1.53 * R), 0.24 * R, 0.1 * R, steps=8)
    zb = z - 2.05 * R
    m.box(WOOD, (x, y, zb + 0.14 * R), (0.36 * R, 0.34 * R, 0.28 * R))
    m.box(c2, (x, y, zb + 0.27 * R), (0.4 * R, 0.38 * R, 0.05 * R))
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, DARK, (x + sx * 0.17 * R, y + sy * 0.16 * R, zb + 0.28 * R), (x + sx * 0.19 * R, y + sy * 0.19 * R, z - 1.5 * R),
                     0.035 * R, 0.035 * R)
    m.box(YEL, (x, y, z - 1.62 * R), (0.1 * R, 0.1 * R, 0.14 * R), glow=1.2)
    for k, dx in enumerate((-0.085 * R, 0.085 * R)):
        P.figure(sc, x + dx, y - 0.02 * R, zb + 0.03 * R, yaw=r.uniform(-25, 25), detail=0, arm_r=('wave', 'up')[k], arm_l=('down', 'wave')[k],
                 swing_r=22, swing_l=(0, -18)[k], **dict(fig, **villager(r, s=S)))


def balloons_a(sc):
    """Three balloons riding the thermal: each circles the piece's middle at its own height and pace, rising and
    sinking a little. 31 to 113 m up as built."""
    r = random.Random(3501)
    spec = ((2600.0, 20.0, 5200.0, 1000.0, 0.012, (PINK, WHITE, YEL)),
            (4200.0, 150.0, 7600.0, 1150.0, -0.009, (CYAN, YEL, WHITE)),
            (3400.0, 265.0, 10200.0, 950.0, 0.015, (LIME, VIOLET, WHITE)))
    for i, (ro, a, z, R, rate, cols) in enumerate(spec):
        x, y = ro * math.cos(math.radians(a)), ro * math.sin(math.radians(a))
        ph = r.random()
        common = dict(phase=ph, rate=0.1, bob=-150.0, base=(0.0, 0.0), travel=(-1.0, rate))
        fig = dict(phase=ph, rate=0.1, bob=-150.0, orbit=(0.0, 0.0, rate))
        balloon(sc, r, x, y, z, R, cols[0], cols[1], cols[2], common, fig)


def balloons_b(sc):
    """Three balloons going down the wind in a loose line (along the piece's X: 180 m and round again), each at a
    speed and a height of its own; the lowest trails a string of signal pennants."""
    r = random.Random(3502)
    L = 18000.0
    spec = ((-4600.0, -300.0, 6200.0, 1200.0, 260.0, (YEL, PINK, CYAN)),
            (900.0, 500.0, 9300.0, 1000.0, 210.0, (WHITE, PINK, VIOLET)),
            (5200.0, -100.0, 4300.0, 820.0, 310.0, (MINT, WHITE, ORANGE)))
    for i, (x, y, z, R, v, cols) in enumerate(spec):
        ph = r.random()
        common = dict(phase=ph, rate=0.09, bob=-120.0, base=(x, y), travel=tv(L, v))
        fig = dict(phase=ph, rate=0.09, bob=-120.0, travel=(L, v))
        balloon(sc, r, x, y, z, R, cols[0], cols[1], cols[2], common, fig)
        if i == 2:      # the pennants: a line under the basket, hung with squares of cloth
            m = sc.part(P.motion(pivot=(x, y, z), **common))
            m.base = None
            zb = z - 2.05 * R
            m.box(WHITE, (x, y, zb - 420), (10, 10, 840))
            for k in range(5):
                m.box(CLOTH[(k * 2) % len(CLOTH)], (x + 70, y, zb - 90 - k * 165), (130, 10, 110))


# ------------------------------------------------------------------------------------------ the film unit's airship
def airship(sc):
    """The film unit's camera airship, 58 m long, 85 m up, crossing along the piece's X (300 m, 8 m a second, and
    round again): a white hull with a pink band and cyan rings, pink fins, a gondola, an open camera deck under the
    nose with the operator and two of the crew (pink chest bands), and a pink-and-white streamer towed behind."""
    r = random.Random(3511)
    L, v = 30000.0, 800.0
    z0 = 8500.0
    common = dict(phase=0.0, rate=0.06, bob=-180.0, base=(0.0, 0.0), travel=tv(L, v))
    m = sc.part(P.motion(pivot=(0.0, 0.0, z0), **common))
    m.base = None
    m.sphere(WHITE, (0, 0, z0), 850, scale=(3.4, 1, 1), steps=(6, 12))
    m.cyl(PINK, (-250, 0, z0), 862, 500, rot=(-90, 0, 0), steps=12)
    m.cyl(CYAN, (900, 0, z0), 820, 400, rot=(-90, 0, 0), steps=12, top_r=772)
    m.cyl(CYAN, (-1300, 0, z0), 772, 400, rot=(-90, 0, 0), steps=12, top_r=820)
    m.sphere(YEL, (2780, 0, z0), 150, steps=(4, 8))                                  # the nose cap
    for (cz, sz) in ((z0 + 560, (760, 40, 820)), (z0 - 560, (760, 40, 820))):          # the fins
        m.box(PINK, (-2450, 0, cz), sz)
    m.box(PINK, (-2450, 0, z0), (760, 1900, 40))
    m.box(BLUE, (-100, 0, z0 - 960), (1200, 330, 300))                               # the gondola
    m.box(WHITE, (-100, 0, z0 - 920), (1100, 342, 80))
    m.box(STEEL_D, (-100, 0, z0 - 820), (60, 60, 120))
    zd = z0 - 1040.0                                                                 # the camera deck under the nose
    m.box(PLANK, (900, -40, zd - 15), (760, 420, 30))
    m.box(WHITE, (900, -246, zd + 150), (760, 12, 16))
    for px in (540, 1260):
        m.box(STEEL_D, (px, 0, (zd + z0 - 800) / 2.0), (26, 26, z0 - 800 - zd))
    m.box(DARK, (1120, -130, zd + 210), (200, 150, 150))                             # the film camera on its head
    m.cyl(STEEL, (1120, -205, zd + 210), 50, 150, rot=(0, 0, 90), steps=6)
    m.box(STEEL_D, (1120, -100, zd + 70), (40, 40, 140))
    fig = dict(phase=0.0, rate=0.06, bob=-180.0, travel=(L, v), detail=0)
    P.figure(sc, 1120, 60, zd, yaw=0, arm_l='hold', arm_r='hold', **dict(fig, **P.look(r, band=PINK, hat='cap', hat_col=PINK, s=3.4)))
    P.figure(sc, 820, -60, zd, yaw=-15, arm_r='wave', swing_r=24, **dict(fig, **P.look(r, band=PINK, hat='headset', hat_col=DARK, s=3.4)))
    P.figure(sc, 600, -40, zd, yaw=10, arm_r='point', arm_l='hip', **dict(fig, **P.look(r, band=PINK, s=3.4)))
    # the streamer: squares of cloth on a tow line, swaying
    st = sc.part(P.motion(swing=5.0, pivot=(-2890.0, 0.0, z0), axis=P.UP, **common))
    st.base = None
    st.box(DARK, (-3150, 0, z0), (520, 12, 12))
    for k in range(7):
        st.box(PINK if k % 2 == 0 else WHITE, (-3400 - 215 - k * 430, 0, z0), (430, 16, 270))


# ------------------------------------------------------------------------------------------ gliders
def hang_glider(sc, x, y, z, heading, span, sail, stripe, pilot, common):
    """A delta wing flying along `heading` with its pilot prone under it; it rocks a little about its line of flight."""
    hd = math.radians(heading)
    tx, ty = math.cos(hd), math.sin(hd)
    rx, ry = -ty, tx

    def at(f, s=0.0, u=0.0):
        return (x + tx * f + rx * s, y + ty * f + ry * s, z + u)

    m = sc.part(P.motion(swing=7.0, pivot=(x, y, z), axis=heading, **common))
    m.base = None
    half = span * 0.5
    for side in (-1, 1):
        tilt_box(m, sail, at(0.12 * span, side * 20), at(-0.2 * span, side * half, 50), 0.3 * span, 14)
        tilt_box(m, stripe, at(0.12 * span, side * 20, 12), at(-0.2 * span, side * half, 62), 0.09 * span, 10)
    tilt_box(m, DARK, at(0.3 * span), at(-0.3 * span), 18, 18)
    tilt_box(m, pilot, at(60, 0, -150), at(-190, 0, -170), 62, 52)
    m.box(WHITE, at(105, 0, -138), (58, 58, 58))
    tilt_box(m, DARK, at(-190, 0, -170), at(-380, 0, -158), 46, 40)
    tilt_box(m, DARK, at(-60, 0, -150), at(-60, 0, 0), 10, 10)


def gliders_a(sc):
    """Eight hang gliders wheeling on one thermal, each on a circle and at a height of its own, both ways round."""
    r = random.Random(3521)
    sails = ((PINK, WHITE), (CYAN, YEL), (YEL, PINK), (WHITE, CYAN), (LIME, VIOLET), (VIOLET, YEL), (ORANGE, WHITE), (MINT, PINK))
    for i in range(8):
        R = 1400.0 + (i % 3) * 1100.0 + r.uniform(-150, 150)
        a0 = math.radians(i * 137.0)
        z = 3200.0 + i * 450.0
        x, y = R * math.cos(a0), R * math.sin(a0)
        rate = (0.04 + 0.008 * (i % 4)) * (1 if i % 2 else -1)
        heading = math.degrees(a0) + (90.0 if rate > 0 else -90.0)
        common = dict(phase=r.random(), rate=0.12, bob=-80.0, base=(0.0, 0.0), travel=(-1.0, rate))
        hang_glider(sc, x, y, z, heading, r.uniform(1400.0, 1650.0), sails[i][0], sails[i][1], CLOTH[(i * 3) % len(CLOTH)], common)


def paraglider(sc, r, x, y, z, c1, c2, L, v, ph):
    """A paraglider flying along +X: a curved canopy of five panels in two colours, two lines, and the pilot sitting
    in the harness under it with both hands on the brakes."""
    S = 3.6
    common = dict(phase=ph, rate=0.07, bob=-260.0, base=(x, y), travel=tv(L, v))
    m = sc.part(P.motion(pivot=(x, y, z + 600.0), **common))
    m.base = None
    Rc, zc = 900.0, z + 300.0

    def arc(a):
        return (x, y + Rc * math.sin(math.radians(a)), zc + Rc * math.cos(math.radians(a)))

    for k in range(5):
        tilt_box(m, c1 if k % 2 == 0 else c2, arc(-60 + 24 * k), arc(-36 + 24 * k), 380, 46)
    for a in (-54, 54):
        tilt_box(m, WHITE, arc(a), (x, y, z + 420.0), 12, 12)
    m.box(DARK, (x, y + 30, z + 150.0), (130, 150, 60))
    P.figure(sc, x, y, z, yaw=90, legs='sit', arm_l='up', arm_r='up', detail=0, phase=ph, rate=0.07, bob=-260.0, travel=(L, v),
             **P.look(r, hat='helmet', hat_col=c2, top=c1, s=S))


def gliders_b(sc):
    """Five paragliders in a string along the slope (220 m and round again), rising and sinking out of step."""
    r = random.Random(3522)
    L, v = 22000.0, 550.0
    cols = ((PINK, WHITE), (YEL, CYAN), (CYAN, WHITE), (LIME, PINK), (VIOLET, YEL))
    for i in range(5):
        x = -8000.0 + i * 4000.0 + r.uniform(-300, 300)
        y = (300.0, -300.0)[i % 2]
        z = 3000.0 + 800.0 * math.sin(i * 1.3) + i * 320.0
        paraglider(sc, r, x, y, z, cols[i][0], cols[i][1], L, v, r.random())


# ------------------------------------------------------------------------------------------ kites from a basalt stack
def kites(sc):
    """A stack of basalt columns 24 m tall; on its head four villagers and a child fly great signal kites 50 to 85 m
    up: diamonds with tails, and a dragon of cloth squares. Each kite swings on its line about the flyer's hand."""
    r = random.Random(3531)
    g = sc.still()
    TOP = 2400.0
    column(g, 0.0, 0.0, 980.0, TOP, 0)
    for k, (x, y, rad, top) in enumerate(((-1050, 250, 620, 1700), (1000, 300, 660, 1900), (-600, -820, 520, 1100), (640, -780, 500, 900),
                                          (0, 900, 700, 2100), (-1500, -500, 420, 600), (1480, -420, 440, 700))):
        column(g, float(x), float(y), float(rad), float(top), k + 1)
    g.box(WHITE, (0, -820, TOP + 120), (1300, 14, 18))
    for px in (-640, 0, 640):
        g.box(WHITE, (px, -820, TOP + 60), (18, 18, 120))
    S = 4.2
    z = TOP + 12
    flyers = ((-560.0, -330.0), (-190.0, -470.0), (190.0, -450.0), (560.0, -310.0))
    kcols = ((PINK, WHITE), (CYAN, YEL), (YEL, PINK), (LIME, VIOLET))
    for i, (fx, fy) in enumerate(flyers):
        ph, rate = r.random(), 0.14 + 0.03 * i
        hand = (fx + 30.0, fy, z + 172.0 * S)
        kx, ky, kz = fx * 4.8 + r.uniform(-200, 200), fy + 500.0 + 150.0 * i, 5200.0 + 780.0 * ((i * 2) % 4) + r.uniform(-150, 150)
        m = sc.part(P.motion(swing=6.0 + i, pivot=hand, axis=90.0, rate=rate, phase=ph))
        m.base = None
        c1, c2 = kcols[i]
        if i == 1:      # the dragon: a round head and a body of squares falling away under it
            tilt_box(m, WHITE, hand, (kx, ky, kz - 120.0), 14, 14)
            m.cyl(c1, (kx, ky + 10, kz), 300, 22, rot=(0, 0, 90), steps=8)
            m.box(DARK, (kx - 100, ky - 6, kz + 60), (60, 10, 60))
            m.box(DARK, (kx + 100, ky - 6, kz + 60), (60, 10, 60))
            for j in range(8):
                m.box(c2 if j % 2 == 0 else c1, (kx + 150.0 * math.sin(j * 0.9), ky, kz - 380.0 - j * 250.0), (300 - j * 18, 12, 230))
        else:
            tilt_box(m, WHITE, hand, (kx, ky, kz - 300.0), 14, 14)
            m.box(c1, (kx, ky, kz), (540, 16, 540), rot=(45, 0, 0))
            m.box(c2, (kx, ky - 12, kz), (760, 8, 60))
            m.box(c2, (kx, ky - 12, kz), (60, 8, 760))
            for j in range(5):
                m.box(c2 if j % 2 == 0 else c1, (kx + 70.0 * math.sin(j * 1.2), ky, kz - 470.0 - j * 180.0), (100, 10, 130))
        P.figure(sc, fx, fy, z, yaw=r.uniform(-20, 20), arm_r='up', arm_l=('hip', 'up')[i % 2], rock=3.0, rate=rate, phase=ph, detail=1,
                 **villager(r, s=S))
    P.figure(sc, 0.0, -640.0, z, yaw=0, arm_r='wave', arm_l='point', swing_r=26, rate=1.1, bob=30.0, phase=r.random(), detail=1,
             **villager(r, build='kid', s=S * 0.68))


# ------------------------------------------------------------------------------------------ climbers on a needle
def climbers(sc):
    """An obsidian needle 74 m tall with a lower spire at its shoulder. A rope team of six climbs its road face on
    ledges, each reaching and hauling out of step, a bright rope between them; on the summit one waves a flag; one
    more slides up and down a second rope (an abseil)."""
    r = random.Random(3541)
    g = sc.still()
    segs = ((ROOT, 2600.0, 950.0, 720.0, BAS_D), (2600.0, 5200.0, 720.0, 480.0, OBS), (5200.0, 7400.0, 480.0, 230.0, BAS))
    for z0, z1, r0, r1, col in segs:
        g.cyl(col, (0, 0, z0), r0, z1 - z0, steps=6, top_r=r1)
    g.cyl(ASHCAP, (0, 0, 7400.0), 240, 14, steps=6)
    g.cyl(BAS, (1250, 350, ROOT), 640, 3800.0 - ROOT, steps=6, top_r=210)
    g.cyl(BAS_L, (-1100, 420, ROOT), 520, 2100.0 - ROOT, steps=6, top_r=260)

    def rad(z):
        for z0, z1, r0, r1, _ in segs:
            if z <= z1:
                return r0 + (r1 - r0) * (z - z0) / (z1 - z0)
        return 230.0

    S = 4.4
    packs = (PINK, CYAN, YEL, LIME, VIOLET, ORANGE)
    prev = None
    for k in range(6):
        z = 700.0 + k * 1080.0
        x = -380.0 + 150.0 * k
        face = -rad(z) * 0.9
        y = face - 70.0
        g.box(ASHCAP, (x, face + 30.0, z - 12), (340, 360, 24))
        P.figure(sc, x, y, z, yaw=180, arm_l='up', arm_r='vee', swing_l=14, swing_r=-14, axis_l='x', axis_r='x', rate=0.32 + 0.03 * (k % 3),
                 bob=50.0, phase=(k * 0.17) % 1.0, detail=(1 if k % 2 == 0 else 0), pack=packs[k],
                 **P.look(r, hat='helmet', hat_col=packs[(k + 2) % 6], sleeves='long', s=S))
        waist = (x, y - 40.0, z + 80.0 * S)
        if prev:
            tilt_box(g, PINK, prev, waist, 14, 14)
        prev = waist
    tilt_box(g, PINK, prev, (60.0, -200.0, 7400.0), 14, 14)
    P.figure(sc, 0.0, -30.0, 7414.0, yaw=0, arm_r='up', swing_r=30, arm_l='hip', rate=0.8, bob=14.0, phase=0.3, detail=1,
             hold_r=P.hold_flag(col=PINK, w=130.0), **P.look(r, hat='helmet', hat_col=YEL, top=CYAN, sleeves='long', s=S))
    ax = 520.0
    ay = -rad(3400.0) * 0.9 - 110.0
    tilt_box(g, CYAN, (ax, -rad(6900.0) * 0.9 - 20.0, 6900.0), (ax, ay - 30.0, 2300.0), 14, 14)
    P.figure(sc, ax, ay, 3300.0, yaw=180, legs='wide', arm_l='fwd', arm_r='fwd', rate=0.1, bob=-700.0, phase=0.6, detail=1, pack=WHITE,
             **P.look(r, hat='helmet', hat_col=PINK, top=YEL, sleeves='long', s=S))


# ------------------------------------------------------------------------------------------ far crowds
def far_crowd_a(sc):
    """The refuge terraces: three stepped benches of basalt, 46 m long, their heads 15, 23 and 31 m up, and on each a
    row of nine villagers four and a half times life size, hopping out of step with their arms up. Two flag bearers
    on the lowest bench, two great pennants on the highest."""
    r = random.Random(3551)
    g = sc.still()
    W = 4600.0
    S = 4.5
    benches = ((1500.0, -900.0), (2300.0, -150.0), (3100.0, 600.0))
    for t, (top, y) in enumerate(benches):
        g.box((BAS, BAS_L, BAS)[t], (0, y + 250.0, (top + ROOT) / 2.0), (W, 1250.0, top - ROOT))
        g.box(ASHCAP, (0, y + 250.0, top + 6), (W + 160, 1300.0, 12))
        g.box((PINK, WHITE, CYAN)[t], (0, y - 395.0, top - 60), (W + 160, 14, 90))
        for k in range(4):
            column(g, -W / 2.0 + 300.0 + k * (W - 600.0) / 3.0 + (t % 2) * 380.0, y - 520.0, 300.0, top - r.uniform(250, 600), k + t, cap=False)
        n = 9
        for k in range(n):
            x = -W / 2.0 + (k + 0.5) * W / n + r.uniform(-40, 40)
            if t == 0 and k in (2, 6):
                continue
            mo = P.motion(phase=((k * 5 + t * 2) % 6) / 6.0, rate=(0.9, 1.1)[(k + t) % 2], bob=70.0)
            blob(sc, r, x, y + r.uniform(-40, 60), top + 12, S, mo, yaw=r.uniform(-15, 15), arms=(t == 0 or k % 2 == 0), legs=(t == 0))
    top, y = benches[0]
    for k, x in ((2, -W / 2.0 + 2.5 * W / 9.0), (6, -W / 2.0 + 6.5 * W / 9.0)):
        P.figure(sc, x, y, top + 12, yaw=0, arm_r='up', swing_r=32, arm_l='hip', rate=0.9, bob=30.0, phase=r.random(), detail=1,
                 hold_r=P.hold_flag(col=(PINK, YEL)[k // 4], w=130.0), **villager(r, s=S))
    top, y = benches[2]
    for sx in (-1, 1):
        px = sx * (W / 2.0 - 150.0)
        g.box(WHITE, (px, y + 500.0, top + 700.0), (24, 24, 1400))
        f = sc.part(P.motion(swing=24.0, pivot=(px, y + 500.0, top + 1200.0), axis=P.UP, rate=0.5, phase=0.5 * (sx + 1) / 2.0))
        f.box((YEL, PINK)[(sx + 1) // 2], (px - sx * 330.0, y + 500.0, top + 1180.0), (640, 14, 380))
        f.box(WHITE, (px - sx * 330.0, y + 492.0, top + 1180.0), (460, 6, 110))


def far_crowd_b(sc):
    """The ring dance: a round mesa 20 m tall; on its head fourteen dancers circle a pole hung with ribbons that turn
    with them, two drummers beat at its foot, and eight onlookers hop on a ledge below the rim."""
    r = random.Random(3552)
    g = sc.still()
    TOP, RM = 2000.0, 1950.0
    S = 4.5
    g.cyl(BAS, (0, 0, ROOT), RM + 150.0, TOP - ROOT, steps=10, top_r=RM)
    g.cyl(ASHCAP, (0, 0, TOP), RM, 12, steps=10)
    g.cyl(PINK, (0, 0, TOP - 110.0), RM + 12.0, 70, steps=10)
    z = TOP + 12
    rate_o = 0.035
    n = 14
    Rr = 1380.0
    for i in range(n):
        a = 2.0 * math.pi * i / n
        mo = P.motion(phase=(i % 4) / 4.0, rate=1.0, bob=60.0, base=(0.0, 0.0), travel=(-1.0, rate_o))
        blob(sc, r, Rr * math.cos(a), Rr * math.sin(a), z, S, mo, yaw=math.degrees(a) + 180.0)
    g.box(WHITE, (0, 0, z + 800.0), (30, 30, 1600))
    g.sphere(YEL, (0, 0, z + 1640.0), 70, steps=(4, 6))
    rb = sc.part(P.motion(base=(0.0, 0.0), travel=(-1.0, rate_o), pivot=(0.0, 0.0, z + 1500.0)))
    rb.base = None
    for i in range(7):
        a = 2.0 * math.pi * (i + 0.5) / 7.0
        tilt_box(rb, CLOTH[i % len(CLOTH)], (0.0, 0.0, z + 1580.0), (Rr * 0.92 * math.cos(a), Rr * 0.92 * math.sin(a), z + 760.0), 46, 10)
    for k, dx in enumerate((-260.0, 260.0)):
        g.cyl((CYAN, YEL)[k], (dx, -330.0, z), 120, 300, steps=8)
        g.cyl(WHITE, (dx, -330.0, z + 300.0), 124, 14, steps=8)
        P.figure(sc, dx, -130.0, z, yaw=0, arm_l='fwd', arm_r='fwd', swing_l=24, swing_r=-24, axis_l='x', axis_r='x', rate=1.6, bob=12.0,
                 phase=0.5 * k, detail=1, **villager(r, s=S))
    # the onlookers' ledge on the road side, 9 m under the rim
    LZ = TOP - 900.0
    g.box(BAS_L, (0, -RM - 420.0, (LZ + ROOT) / 2.0), (3400, 900, LZ - ROOT))
    g.box(ASHCAP, (0, -RM - 420.0, LZ + 6), (3500, 940, 12))
    for k in range(8):
        mo = P.motion(phase=((k * 3) % 5) / 5.0, rate=(0.95, 1.15)[k % 2], bob=70.0)
        blob(sc, r, -1500.0 + (k + 0.5) * 3000.0 / 8.0, -RM - 520.0 + r.uniform(-40, 40), LZ + 12, S, mo, yaw=r.uniform(-15, 15))


# ------------------------------------------------------------------------------------------ the evacuation cableway
def cabin(sc, r, x, y, H, L, v, col, col2, S=3.4):
    """An open cabin hanging from the cable at height H, travelling the piece's X, two people standing in it."""
    mo = P.motion(base=(x, y), travel=tv(L, v), pivot=(x, y, H - 500.0))
    m = sc.part(mo)
    m.base = None
    zf = H - 900.0
    m.box(STEEL_D, (x, y, H + 10.0), (220, 60, 50))
    m.box(STEEL_D, (x, y, H - 140.0), (40, 40, 280))
    m.box(col2, (x, y, H - 290.0), (720, 430, 40))
    m.box(col, (x, y, zf + 110.0), (640, 360, 220))
    m.box(WHITE, (x, y, zf + 236.0), (670, 390, 34))
    for sx in (-1, 1):
        for sy in (-1, 1):
            m.box(WHITE, (x + sx * 300.0, y + sy * 160.0, (zf + 250.0 + H - 300.0) / 2.0), (26, 26, H - 300.0 - zf - 250.0))
    for dx in (-150.0, 150.0):
        blob(sc, r, x + dx, y, zf + 6.0, S, mo, yaw=r.uniform(-20, 20), legs=False)


def cableway(sc):
    """Two lattice pylons 52 m tall, 260 m apart, a cable out and a cable back, and six open cabins of villagers
    crossing between them (three each way, 6.5 m a second), a cabin hut on each pylon."""
    r = random.Random(3561)
    g = sc.still()
    X, H = 13000.0, 5200.0
    for sx in (-1, 1):
        px = sx * X
        for lx in (-1, 1):
            for ly in (-1, 1):
                tilt_box(g, STEEL, (px + lx * 520.0, ly * 520.0, ROOT), (px + lx * 130.0, ly * 130.0, H), 70, 70)
        for zt in (900.0, 2300.0, 3700.0):
            w = 520.0 - 390.0 * (zt - ROOT) / (H - ROOT)
            g.box(STEEL_D, (px, 0, zt), (2.0 * w + 80.0, 2.0 * w + 80.0, 50))
        g.box(YEL, (px, 0, H + 60.0), (420, 1500, 90))
        g.box(PLANK, (px, 0, H - 930.0), (1300, 1800, 40))
        g.box(CYAN, (px, 0, H - 600.0), (1000, 1500, 620))
        g.box(WHITE, (px, 0, H - 270.0), (1120, 1620, 40))
        g.box(PINK, (px, -756.0, H - 600.0), (700, 12, 200))
    for cy in (-450.0, 450.0):
        g.box(DARK, (0, cy, H), (2.0 * X, 14, 14))
    L, v = 2.0 * X, 650.0
    cols = ((PINK, WHITE), (YEL, CYAN), (LIME, WHITE), (CYAN, PINK), (ORANGE, WHITE), (VIOLET, YEL))
    for k in range(3):
        x = -X + (k + 0.5) * L / 3.0
        cabin(sc, r, x, -450.0, H, L, v, cols[k][0], cols[k][1])
        cabin(sc, r, x + L / 7.0, 450.0, H, L, -v, cols[k + 3][0], cols[k + 3][1])


SKY, LAND = 0.25, 0.12
JOBS = [
    ('SM_P3E_Balloons_A', lifted(balloons_a, SKY), dict(far=True, grow=(4500.0, 4500.0, 400.0), grow_down=(4500.0, 4500.0, 400.0))),
    ('SM_P3E_Balloons_B', lifted(balloons_b, SKY), dict(far=True, grow=(9500.0, 300.0, 400.0), grow_down=(9500.0, 300.0, 400.0))),
    ('SM_P3E_Airship', lifted(airship, SKY), dict(far=True, grow=(13000.0, 400.0, 400.0), grow_down=(13000.0, 400.0, 400.0))),
    ('SM_P3E_Gliders_A', lifted(gliders_a, SKY), dict(far=True, grow=(3800.0, 3800.0, 300.0), grow_down=(3800.0, 3800.0, 300.0))),
    ('SM_P3E_Gliders_B', lifted(gliders_b, SKY), dict(far=True, grow=(3500.0, 300.0, 500.0), grow_down=(3500.0, 300.0, 500.0))),
    ('SM_P3E_Kites', lifted(kites, LAND), dict(far=True, grow=(1100.0, 200.0, 300.0), grow_down=(1100.0, 200.0, 0.0))),
    ('SM_P3E_Climbers', lifted(climbers, LAND), dict(far=True, grow=(150.0, 150.0, 300.0), grow_down=(150.0, 150.0, 0.0))),
    ('SM_P3E_FarCrowd_A', lifted(far_crowd_a, LAND), dict(far=True, grow=(300.0, 300.0, 200.0), grow_down=(300.0, 300.0, 0.0))),
    ('SM_P3E_FarCrowd_B', lifted(far_crowd_b, LAND), dict(far=True, grow=(300.0, 300.0, 200.0), grow_down=(300.0, 300.0, 0.0))),
    ('SM_P3E_Cableway', lifted(cableway, LAND), dict(far=True, grow=(100.0, 100.0, 100.0), grow_down=(100.0, 100.0, 0.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
