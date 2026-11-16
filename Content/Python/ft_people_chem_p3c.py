"""FULL THROTTLE (Unreal) the population, pass 3, Chemical Works, builder p3c: THE MOVERS (/Game/People/ChemicalWorks/SM_P3C_*).

Everything here TRAVELS: along the mesh's X (which the placement lays along the road), round a point, or straight up
and down. Nothing crosses the road. This road runs in a trench of its own steel (the kerb to 23 m, pipe racks 7.6 m
tall and tank rows 11 m tall at the sight line), so nobody walks on the ground: the works' own ways are carried
12 to 26 m up on trestles, and the people on them are drawn one and a half to twice life size.

  SM_P3C_Walkers_A     the shift crossing a high pipe bridge both ways between two stair towers           TALL, near
  SM_P3C_Walkers_B     the same bridge: two drum trolleys pushed along it, walkers passing                  TALL, near
  SM_P3C_Cyclists_A    the works' yellow bicycles on a cycle gallery, both ways                             TALL, near
  SM_P3C_Cyclists_B    the gallery again: a cargo trike with drums, two riders                              TALL, near
  SM_P3C_PipeGang      five of the shift carrying one long pipe on their shoulders, the foreman ahead       TALL, near
  SM_P3C_FireDrill     the works' fire team at the run with its hose reel                                   TALL, near
  SM_P3C_ManLift       a mast hoist: two cages of workers riding up and down, out of step                   TALL, near
  SM_P3C_Ropeway       an aerial ropeway between two stations 26 m up: cabins of workers both ways          TALL, far
  SM_P3C_RailCart      the man-rider train on its trestle, 19 m up, shed to shed                            TALL, far
  SM_P3C_TankPatrol    security walking the rim of a tank roof with the dog, two of the shift jogging it    TALL, far
  SM_P3C_KerbBikes     LOW and unlit: two cyclists and a runner on the service path                         low, 24 m

A group that must stay together while it travels (a gang and its pipe, a train and its riders, a cabin and the two in
it) is drawn into a scratch Scene and moved into the piece with ONE travel base (rigid(), below): the shader then
moves and wraps every part of it as one. Every travel path ends inside a tower, a shed or a station, so nothing is
seen to pop. Steady light only (work lamps, lit sheeting, a little light on the figures); the low piece has none.

Placed by C:/ftwork/p3/L1c.inl (pasted into Source/FullThrottle/FTPeopleL1.inl by the integrator).
Run in the editor:  import ft_people_chem_p3c; ft_people_chem_p3c.run()
A piece is drawn like every prop: cm; it stands on z = 0; its FRONT faces -Y (the road); long pieces run along X.
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/ChemicalWorks'

# the works' palette (ft_dress_chem / ft_people_chem) and what the shift wears
STEELB, STEELB_D, TEAL, HAZ, NAVY, MAG, VIOLET, WHITE = '#2a3f8f', '#1c2a66', '#19b5c9', '#ffd23f', '#1c1f4a', '#e84aff', '#b44dff', '#eef6ff'
PAVE, PAVE_D, DARK = '#7f8fc6', '#56659f', '#22202a'
POLE = '#aab6e0'
ORANGE, LIME, YEL, RED, PINK = '#ff7a3c', '#b8ff5a', '#ffc93c', '#ff2d3c', '#ff5fa2'
LAMP = '#fff1c0'
HIVIS = (ORANGE, ORANGE, LIME, YEL, '#fff6ea', PINK)
HATS = (YEL, '#fff6ea', ORANGE, '#2ee6ff', RED)
DEEP = 350.0          # legs go this far under z = 0: beside a banked bend the land is cut away under a piece


def worker(r, **fixed):
    """One of the shift: long sleeves in hi-vis, dark trousers, a hard hat, a reflective band."""
    top = r.choice(HIVIS)
    d = P.look(r, tops=HIVIS, bottoms=('#2b2350', '#3a5fa8', DARK, '#5a4a3c'), sleeves='long', wear='trousers', hat='hard',
               hat_col=r.choice(HATS), band=('#fff6ea' if top != '#fff6ea' else ORANGE))
    d['top'] = top
    d['s'] = r.uniform(0.9, 1.0)
    d.update(fixed)
    return d


def guard(r, **fixed):
    d = P.look(r, sleeves='long', wear='trousers', hat='peak', hat_col=RED, band='#fff6ea')
    d.update(top=RED, bottom=DARK, shoes=DARK)
    d.update(fixed)
    return d


def fireman(r, **fixed):
    d = P.look(r, sleeves='long', wear='coat', hat='hard', hat_col=RED, band='#fff6ea')
    d.update(top=YEL, bottom=YEL, shoes=DARK)
    d.update(fixed)
    return d


def hold_torch(glow=0.0):
    def f(m, h, side):
        m.box(DARK, (h[0], h[1], h[2] + 4), (6, 6, 16))
        m.box(ORANGE, (h[0], h[1], h[2] + 32), (8, 8, 44), glow=glow)
    return f


def hold_bottle(col=RED):
    """A fire extinguisher carried by its handle."""
    def f(m, h, side):
        m.cyl(col, (h[0], h[1], h[2] - 44), 10, 40, steps=6)
        m.box(DARK, (h[0], h[1], h[2] - 2), (5, 5, 8))
    return f


def lit(fn, glow=0.06):
    """Wrap a piece: everything in it that moves or travels is given a little steady light of its own, as hi-vis has
    under work lamps (the works' night leaves a figure on the shadow side nearly black). Only for pieces that stand
    24 m and more from the centre line."""
    def build(sc):
        fn(sc)
        for key in sc.order:
            model, mo = sc.parts[key]
            if not (mo['rate'] or mo['travel'][0]):
                continue
            out = {}
            for (col, gl), mesh in model.meshes.items():
                k2 = (col, max(gl, glow))
                if k2 in out:
                    unreal.GeometryScript_MeshEdits.append_mesh(out[k2], mesh, unreal.Transform())
                else:
                    out[k2] = mesh
            model.meshes = out
    return build


def rigid(sc, base, draw):
    """draw(tmp) draws a group into a scratch Scene; its parts are moved into sc with ONE travel base, so the shader
    moves and wraps the whole group together (a figure's own base is its own place: a gang would wrap man by man)."""
    tmp = P.Scene()
    draw(tmp)
    for key in tmp.order:
        model, mo = tmp.parts[key]
        mo2 = dict(mo)
        mo2['base'] = (float(base[0]), float(base[1]))
        tgt = sc.part(P.motion(**mo2))
        tgt.base = None
        for k2, mesh in model.meshes.items():
            if k2 in tgt.meshes:
                unreal.GeometryScript_MeshEdits.append_mesh(tgt.meshes[k2], mesh, unreal.Transform())
            else:
                tgt.meshes[k2] = mesh


def tv(length, speed):
    """figure()'s own arithmetic for a travel: (length cm, rounds a second)."""
    return (float(length), float(speed) / float(length))


# ------------------------------------------------------------------------------------------ steel to travel on
def bridge(g, L, H, D=220.0, tw=260.0, col=POLE, sheet_h=330.0, lamp=0.8, tower=STEELB):
    """A pipe bridge: a deck L long (X) and D deep, H up on raking trestle legs, lit sheeting behind it (the pale
    ground the walkers' colours read against), a toe board and one rail in front, two work lamps, and at each end a
    tower `tw` long that the travel paths end inside. Returns the travel length (tower middle to tower middle)."""
    n = max(int(round(L / 700.0)), 2)
    for k in range(n + 1):
        x = -L / 2.0 + L * k / n
        for sy in (-1, 1):
            tilt_box(g, col, (x, sy * (D / 2.0 + 80), -DEEP), (x, sy * D / 2.0, H - 20), 14, 14)
        g.box(col, (x, 0, H * 0.45), (10, D + 90, 10))
    g.box(PAVE_D, (0, 0, H - 14), (L, D + 30, 28))
    g.box(HAZ, (0, -D / 2.0 - 17, H + 10), (L, 5, 22))
    g.box(col, (0, -D / 2.0 - 14, H + 118), (L, 8, 8))
    g.box(WHITE, (0, D / 2.0 + 22, H + sheet_h / 2.0 + 4), (L - 20, 5, sheet_h), glow=0.1)
    g.box(ORANGE, (0, D / 2.0 + 18.5, H + sheet_h - 24), (L - 20, 2, 18))
    rod(g, TEAL, (-L / 2.0, D / 2.0 + 60, H - 80), (L / 2.0, D / 2.0 + 60, H - 80), 26, steps=6)      # the pipe the bridge is for
    for sx in (-1, 1):
        x = sx * (L / 2.0 - 70)
        g.box(col, (x, D / 2.0 + 14, H + sheet_h + 60), (9, 9, 120))
        g.box(DARK, (x, D / 2.0, H + sheet_h + 122), (70, 50, 26))
        g.box(LAMP, (x, D / 2.0 - 16, H + sheet_h + 108), (60, 30, 5), glow=lamp)
        tx = sx * (L / 2.0 + tw / 2.0)
        top = H + sheet_h + 110
        g.box(tower, (tx, 0, (top - DEEP) / 2.0), (tw, D + 70, top + DEEP))
        g.box(HAZ, (tx, -D / 2.0 - 36, H + 6), (tw, 3, 26))
        g.box(LAMP, (tx, -D / 2.0 - 36, H + 220), (tw * 0.5, 3, 70), glow=0.6)
        g.box(STEELB_D, (tx, 0, top + 8), (tw + 30, D + 100, 16))
    return L + tw


def act(kind, road, r):
    """What a walker's arms do. road: 'l' or 'r', the arm on the road's side."""
    far = 'r' if road == 'l' else 'l'
    sg = -1 if road == 'l' else 1
    if kind == 1:
        return {'arm_' + road: 'wave', 'swing_' + road: 28 * sg, 'swing_' + far: 20, 'axis_' + far: 'x', 'react': True}
    if kind == 2:
        return dict(arm_l='carry', arm_r='carry', hold_r=P.hold_box(col=r.choice((TEAL, '#c9a070', PAVE))))
    if kind == 3:
        return {'arm_' + road: 'out', 'swing_' + road: 12, 'axis_' + road: 'z', 'swing_' + far: 20, 'axis_' + far: 'x'}
    if kind == 4:
        return {'arm_' + road: 'wave', 'swing_' + road: 24 * sg, 'hold_' + far: P.hold_bag(col=r.choice((YEL, PINK, TEAL))), 'react': True}
    if kind == 5:
        return dict(arm_l='vee', arm_r='vee', swing_l=14, swing_r=-14, react=True)
    return dict(swing_l=20, swing_r=-20, axis_l='x', axis_r='x')


def trolley(sc, x, y, z, s, sgn, travel):
    """A drum trolley in front of its pusher (who stands on (x, y) and shares this travel)."""
    m = sc.part(P.motion(base=(x, y), travel=travel, pivot=(x + sgn * 120 * s, y, z + 60 * s)))
    m.base = None
    cx = x + sgn * 118 * s
    m.box(PAVE, (cx, y, z + 26 * s), (120 * s, 78 * s, 12 * s))
    for k in (-1, 1):
        m.box(DARK, (cx + k * 40 * s, y, z + 11 * s), (22 * s, 84 * s, 22 * s))
        m.cyl(TEAL if k * sgn > 0 else WHITE, (cx + k * 28 * s, y, z + 32 * s), 26 * s, 84 * s, steps=6)
        m.cyl(MAG, (cx + k * 28 * s, y, z + 84 * s), 27 * s, 30 * s, steps=6)
    tilt_box(m, POLE, (cx - sgn * 60 * s, y, z + 30 * s), (cx - sgn * 76 * s, y, z + 104 * s), 7 * s, 60 * s)


def walkers(seed, L=2400.0, H=1400.0, trolleys=0, s=1.5):
    def build(sc):
        r = random.Random(seed)
        TL = bridge(sc.still(), L, H)
        n = 6
        for i in range(n):
            fwd = i % 2 == 0
            sgn = 1 if fwd else -1
            x = -TL / 2.0 + (i + 0.5) * TL / n + r.uniform(-110, 110)
            y = -48.0 if fwd else 46.0
            s1 = s * r.uniform(0.94, 1.04)
            kw = worker(r, s=s1)
            if i in (0, 3)[:trolleys]:
                v = sgn * r.uniform(120, 150)
                kw.update(arm_l='push', arm_r='push')
                trolley(sc, x, y, H, s1, sgn, tv(TL, v))
            else:
                v = sgn * r.uniform(150, 215)
                kw.update(act((i + seed) % 6, 'l' if fwd else 'r', r))
            P.figure(sc, x, y, z=H, yaw=90 * sgn, stride=26, rate=r.uniform(0.9, 1.15), bob=4, phase=r.random(), travel=(TL, v), detail=1, **kw)
    return build


def bike(sc, x, y, z, yaw, s, travel, col=YEL, trike=False):
    """A works bicycle under a seated rider at (x, y, z) who shares this travel. Drawn in the rider's own frame
    (front = -Y). trike: a load box with two drums over a pair of front wheels."""
    m = sc.part(P.motion(base=(x, y), travel=travel, pivot=(x, y, z + 90 * s)), (x, y, z), yaw, s)
    rod(m, DARK, (-4, 58, 34), (4, 58, 34), 34, steps=8)
    if trike:
        for k in (-1, 1):
            rod(m, DARK, (k * 40 - 4, -70, 30), (k * 40 + 4, -70, 30), 30, steps=8)
        m.box(TEAL, (0, -74, 62), (92, 70, 40))
        m.cyl(WHITE, (-22, -74, 82), 20, 56, steps=6)
        m.cyl(MAG, (22, -74, 82), 20, 56, steps=6)
    else:
        rod(m, DARK, (-4, -56, 34), (4, -56, 34), 34, steps=8)
        tilt_box(m, col, (0, -56, 34), (0, -34, 100), 7, 7)
    m.box(col, (0, -34, 104), (48, 7, 7))
    tilt_box(m, col, (0, -34, 86), (0, 20, 50), 8, 8)
    tilt_box(m, col, (0, 20, 50), (0, 58, 34), 7, 7)
    tilt_box(m, col, (0, 20, 50), (0, 14, 90), 7, 7)
    m.box(DARK, (0, 14, 92), (18, 28, 7))


def rider(sc, r, x, y, z, sgn, s, speed, TL, kind=0, trike=False, detail=1, col=YEL):
    kw = worker(r, s=s)
    kw.update(arm_l='push', arm_r='push')
    if kind == 1:
        road = 'l' if sgn > 0 else 'r'
        kw.update({'arm_' + road: 'wave', 'swing_' + road: 26, 'react': True})
    elif kind == 2:
        kw.update(rock=3.0)
    bike(sc, x, y, z, 90 * sgn, s, tv(TL, sgn * speed), col=col, trike=trike)
    P.figure(sc, x, y, z=z, yaw=90 * sgn, legs='sit', seat=88.0, rate=r.uniform(1.1, 1.5), phase=r.random(), travel=(TL, sgn * speed),
             detail=detail, **kw)


def cyclists(seed, L=2600.0, H=1350.0, trike=False, s=1.5):
    def build(sc):
        r = random.Random(seed)
        TL = bridge(sc.still(), L, H, tower=STEELB_D)
        n = 4
        for i in range(n):
            fwd = i % 2 == 0
            sgn = 1 if fwd else -1
            x = -TL / 2.0 + (i + 0.5) * TL / n + r.uniform(-150, 150)
            tk = trike and i == 0
            rider(sc, r, x, -50.0 if fwd else 50.0, H, sgn, s * r.uniform(0.95, 1.03), r.uniform(240, 300) if tk else r.uniform(380, 520), TL,
                  kind=(i + seed) % 3, trike=tk, col=(YEL, PINK, LIME, WHITE)[(i + seed) % 4])
    return build


def pipe_gang(seed, L=2200.0, H=1300.0, s=1.5):
    def build(sc):
        r = random.Random(seed)
        G = 760.0
        TL = bridge(sc.still(), L, H, tw=G + 120.0)
        y0, v = 12.0, 120.0
        yp, zp = y0 - s * 58.0, H + s * 162.0

        def draw(t):
            for i in range(5):
                kw = worker(r, s=s * r.uniform(0.96, 1.02))
                kw.update(arm_l=((150, 0), (172, 0)), swing_r=-18, axis_r='x')
                P.figure(t, (i - 2) * 125.0, y0, z=H, yaw=90, stride=22, rate=1.0, bob=0, phase=(i * 0.5) % 1.0, travel=(TL, v), detail=1, **kw)
            kw = worker(r, s=s, top='#fff6ea', band=ORANGE, hat_col='#fff6ea')
            kw.update(arm_l='wave', swing_l=-24, hold_l=P.hold_flag(col=PINK, w=70.0), swing_r=-18, axis_r='x', react=True)
            P.figure(t, 385.0, y0, z=H, yaw=90, stride=22, rate=1.0, bob=3, phase=0.25, travel=(TL, v), detail=1, **kw)
            m = t.part(P.motion(base=(0.0, y0), travel=tv(TL, v), pivot=(0.0, yp, zp)))
            m.base = None
            rod(m, TEAL, (-335.0, yp, zp), (335.0, yp, zp), 19.0, steps=8)
            for k in (-1, 1):
                rod(m, MAG, (k * 338.0 - 12, yp, zp), (k * 338.0 + 12, yp, zp), 25.0, steps=8)
        rigid(sc, (0.0, y0), draw)
    return build


def fire_drill(seed, L=2400.0, H=1250.0, s=1.45):
    def build(sc):
        r = random.Random(seed)
        TL = bridge(sc.still(), L, H, tw=660.0, tower=RED)
        v = 330.0
        run = dict(yaw=90, stride=38, rate=1.9, bob=7, travel=(TL, v), detail=1)

        def draw(t):
            kw = fireman(r, s=s)
            kw.update(arm_l='out', swing_l=10, axis_l='z', arm_r='up', swing_r=-18, react=True)
            P.figure(t, 250.0, -10.0, z=H, phase=0.0, **dict(run, **kw))
            for k in (-1, 1):
                kw = fireman(r, s=s * r.uniform(0.95, 1.02))
                kw.update(arm_l='back', arm_r='back')
                P.figure(t, 70.0, k * 52.0, z=H, phase=0.25 + 0.25 * k, **dict(run, **kw))
            kw = fireman(r, s=s)
            kw.update(hold_r=hold_bottle(), arm_l=((20, 30), (80, 90)), swing_l=30, axis_l='x')
            P.figure(t, -270.0, 20.0, z=H, phase=0.5, **dict(run, **kw))
            m = t.part(P.motion(base=(0.0, 0.0), travel=tv(TL, v), pivot=(-90.0, 0.0, H + 70.0)))       # the hose reel
            m.base = None
            for k in (-1, 1):
                rod(m, DARK, (-100.0, k * 62.0 - 6, H + 48.0), (-100.0, k * 62.0 + 6, H + 48.0), 48.0, steps=8)
            rod(m, RED, (-100.0, -50.0, H + 74.0), (-100.0, 50.0, H + 74.0), 46.0, steps=8)
            rod(m, WHITE, (-100.0, -40.0, H + 74.0), (-100.0, 40.0, H + 74.0), 52.0, steps=6)
            m.box(RED, (-100.0, 0.0, H + 26.0), (110.0, 130.0, 12.0))
            tilt_box(m, POLE, (-50.0, 0.0, H + 40.0), (60.0, 0.0, H + 110.0), 104.0, 8.0)
        rigid(sc, (0.0, 0.0), draw)
    return build


def man_lift(seed, top=1900.0, mid=900.0, amp=400.0, s=1.35):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(STEELB, (0, 40, (top - DEEP) / 2.0), (90, 90, top + DEEP))
        k = 150.0
        while k < top:
            g.box(HAZ, (0, 40, k), (112, 112, 12))
            k += 250.0
        g.box(DARK, (0, 20, top + 16), (300, 130, 32))
        for sx in (-1, 1):
            g.box(LAMP, (sx * 100, -20, top - 4), (80, 50, 6), glow=0.9)
        g.box(PAVE_D, (0, 40, 12), (640, 300, 24))
        for ci, sx in enumerate((-1, 1)):
            ph = 0.5 * ci + 0.07
            cx = sx * 200.0
            c = sc.part(P.motion(phase=ph, rate=0.07, bob=-amp, pivot=(cx, 0.0, mid + 130.0)))
            c.base = None
            c.box(PAVE, (cx, 0, mid), (300, 200, 14))
            c.box(HAZ, (cx, -100, mid + 18), (300, 5, 24))
            c.box(POLE, (cx, -100, mid + 112 * s), (300, 8, 8))
            c.box(WHITE, (cx, 100, mid + 150), (300, 5, 290), glow=0.12)
            c.box(ORANGE, (cx, 96, mid + 270), (300, 2, 18))
            c.box(HAZ, (cx, 0, mid + 300), (316, 216, 14))
            for px in (-1, 1):
                for py in (-1, 1):
                    c.box(POLE, (cx + px * 146, py * 96, mid + 150), (8, 8, 300))
            c.box(DARK, (cx - sx * 150, 40, mid + 150), (40, 60, 40))            # the cage's grip on the mast
            for i in range(3):
                kw = worker(r, s=s * r.uniform(0.94, 1.02))
                kw.update((dict(arm_r='wave', swing_r=40), dict(arm_l='vee', arm_r='vee', swing_l=24, swing_r=-24),
                           dict(arm_l='out', swing_l=20, axis_l='z', arm_r='hip'))[(i + ci + seed) % 3])
                P.figure(sc, cx - 96 + i * 96, -20 + (18 if i == 1 else 0), z=mid + 7, yaw=r.uniform(-14, 14), phase=ph, rate=0.07, bob=-amp,
                         detail=1, **kw)
    return build


def ropeway(seed, L=6000.0, Z=2620.0, s=2.2, cabins=3):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        for sx in (-1, 1):
            x = sx * L / 2.0
            for sy in (-1, 1):
                tilt_box(g, POLE, (x, sy * 520, -DEEP), (x, sy * 120, Z - 660), 40, 40)
            g.box(POLE, (x, 0, Z * 0.5), (26, 560, 26))
            g.box(STEELB, (x, 0, Z - 300), (760, 520, 720))                      # the station the cabins run into
            g.box(HAZ, (x, -262, Z - 636), (760, 4, 40))
            g.box(LAMP, (x, -262, Z - 250), (420, 4, 150), glow=0.6)
            g.box(STEELB_D, (x, 0, Z + 80), (820, 580, 40))
        for sy in (-1, 1):
            g.box(DARK, (0, sy * 130, Z), (L, 14, 14))
        for i in range(cabins):
            fwd = i % 2 == 0
            sgn = 1 if fwd else -1
            gx = -L / 2.0 + (i + 0.5) * L / cabins + r.uniform(-200, 200)
            gy = -130.0 if fwd else 130.0
            v = sgn * r.uniform(300, 380)
            fz = Z - 620.0
            col = (YEL, WHITE, PINK)[i % 3]

            def draw(t, gx=gx, gy=gy, v=v, fz=fz, col=col):
                m = t.part(P.motion(base=(gx, gy), travel=tv(L, v), pivot=(gx, gy, fz + 250)))
                m.base = None
                m.box(DARK, (gx, gy, Z - 90), (16, 16, 180))
                m.box(DARK, (gx, gy, Z + 6), (90, 30, 30))
                m.box(col, (gx, gy, fz + 466), (360, 230, 28))
                m.box(PAVE_D, (gx, gy, fz - 8), (340, 210, 16))
                m.box(col, (gx, gy - 105, fz + 60), (340, 8, 120))
                m.box(TEAL, (gx, gy - 110, fz + 96), (340, 3, 26))
                m.box(col, (gx, gy + 105, fz + 230), (340, 8, 460), glow=0.15)
                for px in (-1, 1):
                    m.box(col, (gx + px * 164, gy - 100, fz + 230), (12, 12, 460))
                for k in (-1, 1):
                    kw = worker(r, s=s * r.uniform(0.93, 1.0))
                    kw.update((dict(arm_r='wave', swing_r=30), dict(arm_l='vee', arm_r='vee', swing_l=16, swing_r=-16, bob=6),
                               dict(arm_l='wave', swing_l=-30))[(i + k + seed) % 3])
                    P.figure(t, gx + k * 80, gy + 10, z=fz, yaw=k * 10, rate=r.uniform(1.0, 1.4), phase=r.random(), travel=(L, v), detail=1, **kw)
            rigid(sc, (gx, gy), draw)
    return build


def rail_cart(seed, L=4400.0, H=1900.0, s=2.0):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        G = 1040.0
        TW = G + 120.0
        TL = L + TW
        n = 4
        for k in range(n + 1):
            x = -L / 2.0 + L * k / n
            g.box(PAVE, (x, 0, (H - 60 - DEEP) / 2.0), (90, 150, H - 60 + DEEP))
            g.box(PAVE_D, (x, 0, H - 90), (150, 260, 40))
        g.box(STEELB, (0, 0, H - 36), (L + 2 * TW, 200, 60))
        g.box(HAZ, (0, -102, H - 30), (L, 3, 24))
        g.box(DARK, (0, -60, H + 6), (L, 10, 12))
        for sx in (-1, 1):
            tx = sx * (L / 2.0 + TW / 2.0)
            g.box(STEELB_D, (tx, 0, H + 240), (TW, 300, 480))
            g.box(LAMP, (tx, -152, H + 300), (TW * 0.5, 4, 110), glow=0.6)
            g.box(HAZ, (tx, -152, H + 30), (TW, 4, 36))
            g.box(NAVY, (tx, 0, H + 496), (TW + 60, 360, 32))
        v = 560.0

        def draw(t):
            m = t.part(P.motion(base=(0.0, 0.0), travel=tv(TL, v), pivot=(0.0, 0.0, H + 120.0)))
            m.base = None
            m.box(TEAL, (360, 0, H + 110), (300, 170, 170))                     # the little locomotive
            m.box(WHITE, (300, 0, H + 250), (150, 160, 120))
            m.box(HAZ, (512, 0, H + 70), (10, 172, 80))
            m.box(LAMP, (512, 0, H + 160), (6, 70, 40), glow=0.8)
            m.box(DARK, (360, 0, H + 18), (260, 180, 30))
            for cx in (-340.0, 10.0):                                            # two open man-rider cars
                m.box(DARK, (cx, 0, H + 18), (260, 180, 30))
                m.box(YEL, (cx, 0, H + 44), (320, 180, 22))
                m.box(YEL, (cx, -88, H + 100), (320, 8, 96))
                m.box(MAG, (cx, -93, H + 120), (320, 3, 20))
                m.box(YEL, (cx, 88, H + 100), (320, 8, 96))
                for i in range(3):
                    kw = worker(r, s=s * r.uniform(0.92, 1.0))
                    kw.update((dict(arm_r='wave', swing_r=32), dict(arm_l='vee', arm_r='vee', swing_l=16, swing_r=-16, bob=6),
                               dict(arm_l='wave', swing_l=-30), dict(arm_r='out', swing_r=14, axis_r='z', arm_l='hip'))[(i + int(cx) + seed) % 4])
                    P.figure(t, cx - 100 + i * 100, 10.0, z=H + 55, yaw=r.uniform(-12, 12), rate=r.uniform(1.0, 1.4), phase=r.random(),
                             travel=(TL, v), detail=(1 if i != 1 else 0), **kw)
        rigid(sc, (0.0, 0.0), draw)
    return build


def tank_patrol(seed, R=650.0, H=2000.0, s=2.3):
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.cyl(PAVE, (0, 0, -DEEP), R, H + DEEP, steps=14)
        g.cyl(TEAL, (0, 0, H * 0.72), R + 6, 130, steps=14)
        g.cyl(HAZ, (0, 0, H - 20), R + 10, 50, steps=14)
        g.box(POLE, (0, 0, H + 260), (24, 24, 520))
        g.box(DARK, (0, 0, H + 540), (150, 150, 40))
        g.box(LAMP, (0, 0, H + 512), (120, 120, 8), glow=0.9)
        g.cyl(STEELB, (0, 0, H), 150, 120, steps=8)
        rr = R - 130.0

        def place(th, ccw):
            a = math.radians(th)
            return rr * math.cos(a), rr * math.sin(a), (th + 180.0) if ccw else th

        walk = dict(stride=26, swing_l=20, swing_r=-20, axis_l='x', axis_r='x', rate=1.0, bob=4, detail=1)
        rps = 0.055
        for i, th in enumerate((250.0, 268.0)):                                # two guards, side by side, the dog ahead
            x, y, yaw = place(th, True)
            kw = guard(r, s=s * r.uniform(0.96, 1.02))
            if i == 0:
                kw.update(arm_l='wave', hold_l=hold_torch(glow=1.0))
            P.figure(sc, x, y, z=H, yaw=yaw, phase=0.5 * i, orbit=(0.0, 0.0, rps), **dict(walk, **(dict(kw, swing_l=24, axis_l='y') if i == 0 else kw)))
        x, y, yaw = place(300.0, True)
        P.quadruped(sc, x, y, z=H, yaw=yaw, s=s, length=80, height=56, girth=24, neck=18, neck_up=40, head=22, tail=26, wag=30, stride=24,
                    rate=2.4, bob=3, body='#c9a070', snout=DARK, ears=DARK, orbit=(0.0, 0.0, rps))
        for i, th in enumerate((80.0, 110.0, 200.0)):                          # the shift at its exercise, the other way round
            x, y, yaw = place(th, False)
            kw = worker(r, s=s * r.uniform(0.92, 1.0))
            kw.update(arm_l=((20, 30), (80, 90)), arm_r=((20, 30), (80, 90)), swing_l=30, swing_r=-30, axis_l='x', axis_r='x')
            if i == 1:
                kw.update(arm_l='wave', swing_l=-26, axis_l='y', react=True)
            P.figure(sc, x, y, z=H, yaw=yaw, stride=38, rate=1.8, bob=7, phase=r.random(), orbit=(0.0, 0.0, -0.085), detail=1, **kw)
    return build


def kerb_bikes(seed, L=1800.0):
    """LOW and unlit: the service path behind the kerb, two cyclists and a runner on it. Nothing glows."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        g.box(PAVE_D, (0, 0, 2), (L, 96, 4))
        for k in range(6):
            g.box(HAZ, (-L / 2.0 + (k + 0.5) * L / 6.0, -44, 5), (L / 12.0, 6, 3))
        rider(sc, r, -520.0, -18.0, 0.0, 1, 0.88, 430.0, L, kind=1, detail=2, col=YEL)
        rider(sc, r, 380.0, 22.0, 0.0, -1, 0.88, 390.0, L, kind=2, detail=2, col=PINK)
        kw = worker(r, s=0.92, top=LIME, band='#fff6ea')
        kw.update(arm_l=((20, 30), (80, 90)), arm_r=((20, 30), (80, 90)), swing_l=30, swing_r=-30, axis_l='x', axis_r='x')
        P.figure(sc, 60.0, -20.0, yaw=90, stride=38, rate=1.9, bob=6, phase=r.random(), travel=(L, 260.0), **kw)
    return build


FAR = dict(far=True)
JOBS = [
    ('SM_P3C_Walkers_A', lit(walkers(31))),
    ('SM_P3C_Walkers_B', lit(walkers(32, L=2200.0, H=1250.0, trolleys=2))),
    ('SM_P3C_Cyclists_A', lit(cyclists(33))),
    ('SM_P3C_Cyclists_B', lit(cyclists(34, L=2300.0, H=1250.0, trike=True))),
    ('SM_P3C_PipeGang', lit(pipe_gang(35))),
    ('SM_P3C_FireDrill', lit(fire_drill(36))),
    ('SM_P3C_ManLift', lit(man_lift(37)), dict(grow=(60.0, 60.0, 460.0), grow_down=(60.0, 60.0, 0.0))),
    ('SM_P3C_Ropeway', lit(ropeway(38)), FAR),
    ('SM_P3C_RailCart', lit(rail_cart(39)), FAR),
    ('SM_P3C_TankPatrol', lit(tank_patrol(40)), FAR),
    ('SM_P3C_KerbBikes', kerb_bikes(41)),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
