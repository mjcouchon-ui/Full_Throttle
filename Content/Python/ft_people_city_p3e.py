"""FULL THROTTLE (Unreal) the population, level 7: City Center, pass 3, builder e: FAR AND SKY.
/Game/People/CityCenter/SM_P3E_*.

Life from 150 m out to the horizon and in the air above it. The first far kit (ft_people_city_far.py) has the band from
100 to 150 m (tower parties, a concert, a loft, a running track) and the sky over the first row of towers (geese,
hang-gliders); nothing here is nearer than 150 m, and nothing here stands on the dressing.

HOW THE LAND LIES, and so what a piece here must be (FTDressL7.inl's header, FTPeopleL7.inl's "far" block): the avenue
is a canyon at midnight, towers 46 to 98 m tall in every slot to 82 m out, mid-rise blocks to 160 m, the skyline
behind. What is seen from the road is what stands HIGH and is LIT, down the avenue and from the climbs. So:
  - the six STANDS bring their own height: a plain dark shaft with a few lit windows, the people on its crown 52 to
    64 m up as built (the table stands them at 2 to 2.3 times that), drawn 2.2 to 2.6 times life size;
  - the seven SKY pieces are built from z = 0 up and hung by the table 90 m and more over the road (a negative drop):
    paper lanterns, hot-air balloons, starlings, a dragon kite. Each is tens of metres across;
  - steady light only (lit floors, bulbs, lanterns, a cinema screen) and a faint light on every figure so that a
    crowd's colours read against the night. Nothing blinks; nothing is black and gold; no chevrons (the Syndicate's);
  - everything is saved with far=True (the always-moving master).

    SM_P3E_HighWire     two slim towers and a wire of bulbs between them: a wire-walker under a parasol slides out
                        and back, a crowd hops on both crowns
    SM_P3E_KiteRoof     three giants fly lit kites from a tower's crown; the kites swing on their lines
    SM_P3E_RoofCinema   an open-air picture show on a wide roof: a lit screen (a little car runs to and fro across
                        it), three rows of watchers, the back row on its feet
    SM_P3E_Abseil       five climbers on ropes down a tower's face, swinging and riding up and down; riggers above
    SM_P3E_FarStand     a grandstand on a tall plinth: a dense band of colour, hopping out of step, flags at its ends
    SM_P3E_Viaduct      a high road between two blocks: open-top night-tour buses with waving passengers, taxis
    SM_P3E_Lanterns_A/B a drift of paper lanterns, each riding up and down on the air, the whole drift going by
    SM_P3E_Balloons_A/B hot-air balloons lit from inside, passengers waving from the baskets, going by slowly
    SM_P3E_Murmur_A     starlings wheeling in three rings, wings beating
    SM_P3E_Murmur_B     a ribbon of starlings that travels and ripples along its length
    SM_P3E_DragonKite   a dragon kite 40 m long: a head and eighteen lit diamonds that ripple one after the other

Run in the editor:  import ft_people_city_p3e as K; K.run()        (or K.run(['SM_P3E_HighWire']))
"""
import math
import random
import unreal
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/People/CityCenter'
NAVY, DARK, GREY, STEEL, WHITE, CREAM = '#2b2350', '#22202a', '#3f3d50', '#8f9bb3', '#fff6ea', '#fff1d6'
PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, BLUE = '#ff5fa2', '#2ee6ff', '#ffc93c', '#7dffb0', '#ff2d3c', '#ff7a3c', '#8f6bff', '#3a5fa8'
WARM, WARM_L, WOOD = '#ffe28a', '#fff1c0', '#9a6a44'
SHAFT, SHAFT_L = '#262447', '#3a3766'
HOT = (PINK, CYAN, YEL, MINT, RED, ORANGE, VIOLET, WHITE, '#f4a6ff', '#b8ff5a', '#57c7ff')
G_BULB, G_FLOOR, G_RIM, G_WIN, G_FIG = 2.2, 0.55, 1.3, 0.9, 0.22


def lit(sc, g=G_FIG):
    """Night: everything of a piece that carries no light of its own gets a faint one. Every piece of this kit
    stands 150 m and more from the road (the light rule asks 24 m)."""
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


def giant(r, s, **fixed):
    """A citizen in colours that read at night, drawn `s` times life size."""
    d = P.look(r, tops=HOT, **fixed)
    d['s'] = s * r.uniform(0.94, 1.05)
    return d


def mini(sc, r, x, y, z, s=2.2, up=True, legs=True, bob=30.0, rate=1.4, phase=None, travel=None, top=None):
    """One of a far crowd, in four to seven boxes: a coat, a head, hair, arms up. It hops (bob), out of step with
    its neighbours (eight phases, so a crowd is a handful of parts). travel: (length, rounds a second) to ride along
    the mesh's X with a vehicle."""
    ph = (r.randrange(8) / 8.0) if phase is None else phase
    kw = dict(phase=ph, rate=rate, bob=bob)
    if travel:
        kw.update(travel=travel, base=(x, y), pivot=(x, y, z))
    m = sc.part(P.motion(**kw))
    top = top or r.choice(HOT)
    z0 = z
    if legs:
        m.box(r.choice(P.BOTTOMS), (x, y, z + 33 * s), (32 * s, 22 * s, 66 * s))
        z0 = z + 66 * s
    m.box(top, (x, y, z0 + 28 * s), (46 * s, 26 * s, 56 * s))
    m.box(r.choice(P.SKINS), (x, y, z0 + 78 * s), (38 * s, 38 * s, 42 * s))
    m.box(r.choice(P.HAIRS), (x, y + 3 * s, z0 + 104 * s), (40 * s, 40 * s, 12 * s))
    if up:
        for sx in (-1, 1):
            m.box(top, (x + sx * 31 * s, y, z0 + 78 * s), (11 * s, 11 * s, 60 * s), rot=(sx * 16.0, 0, 0))
    return m


def tower(g, W, D, H, seed, x=0.0, y=0.0, z_win=2400.0, pitch=620.0, cols=(WARM, CYAN), p=0.5):
    """The stand a far piece brings: a plain dark shaft with lit windows on its upper floors."""
    r = random.Random(seed)
    g.box(SHAFT, (x, y, H / 2.0), (W, D, H))
    z = z_win
    while z < H - 500.0:
        col = cols[int(z / pitch) % 2]
        for k in range(4):
            if r.random() < p:
                g.box(col, (x - W * 0.36 + k * W * 0.24, y - D / 2.0 - 3, z), (W * 0.18, 6, 200), glow=G_WIN)
        z += pitch


def deck(g, W, D, H, x=0.0, y=0.0, rim=PINK, rim2=CYAN, tiles=(PINK, CYAN, YEL, VIOLET), n=6, rail=240.0):
    """A crown: a slab wider than the shaft, a lit rim, a lit floor of squares along its front, a rail."""
    g.box(GREY, (x, y, H + 35), (W, D, 70))
    g.box(rim, (x, y - D / 2.0 - 5, H + 30), (W + 10, 10, 80), glow=G_RIM)
    for sx in (-1, 1):
        g.box(rim2, (x + sx * (W / 2.0 + 5), y, H + 30), (10, D, 80), glow=G_RIM)
    tw = W / n
    for i in range(n):
        g.box(tiles[i % len(tiles)], (x - W / 2.0 + (i + 0.5) * tw, y - D / 2.0 + D * 0.3, H + 73), (tw - 20, D * 0.55, 6), glow=G_FLOOR)
    g.box(STEEL, (x, y - D / 2.0 + 12, H + 70 + rail), (W, 14, 14))
    for k in range(4):
        g.box(STEEL, (x - W / 2.0 + 7 + k * (W - 14) / 3.0, y - D / 2.0 + 12, H + 70 + rail / 2.0), (12, 12, rail))
    return H + 76.0


def bulbs(g, a, b, n, sag=120.0, cols=(WARM, PINK, CYAN, YEL), size=46.0):
    """A string of big bulbs from a to b (steady light)."""
    for k in range(n):
        t = (k + 0.5) / n
        s = sag * 4.0 * t * (1.0 - t)
        g.box(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t - s), (size, size, size), glow=G_BULB)


def hold_parasol(col=PINK, stick=WHITE):
    def f(m, h, side):
        m.box(stick, (h[0], h[1], h[2] + 55), (5, 5, 120))
        m.cyl(col, (h[0], h[1], h[2] + 108), 92, 24, steps=8, top_r=10, glow=0.5)
    return f


def hold_flag(col=PINK, pole=WHITE, w=110.0):
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + 60), (5, 5, 150))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + 112), (w, 4, 46), glow=0.5)
    return f


# ------------------------------------------------------------------------------------------ the stands
def high_wire(seed=1, L=3000.0, H=6000.0):
    """Two slim towers and a wire of bulbs between their crowns, 60 m up. A wire-walker under a parasol slides out
    to the middle and back; on each crown a small crowd hops and a marshal waves a flag."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h = L / 2.0
        zw = H + 330.0
        for sx in (-1, 1):
            cx = sx * (h + 480)
            tower(g, 900.0, 1000.0, H, seed + (sx > 0), x=cx, cols=(WARM, CYAN) if sx < 0 else (CYAN, WARM))
            top = deck(g, 1300.0, 1200.0, H, x=cx, rim=(PINK, CYAN)[sx > 0], rim2=YEL, n=3)
            g.box(STEEL, (sx * (h + 10), 0, H + 70 + 150), (22, 22, 300))                     # the wire's post
            for k in range(5):
                mini(sc, r, cx - 420 + k * 210 + r.uniform(-20, 20), -430 + (k % 2) * 150, top, s=2.2, bob=34.0, rate=1.4)
            P.figure(sc, cx + sx * 360, -120, z=top, yaw=-sx * 30, arm_r='up', swing_r=26, hold_r=hold_flag(col=HOT[(seed + (sx > 0)) % 5]), arm_l='hip',
                     rate=0.9, bob=10, phase=r.random(), detail=0, **giant(r, 2.5))
        n = 13
        for k in range(n):                                                                # the wire: a line of bulbs
            g.box((WARM, PINK, WARM, CYAN)[k % 4], (-h + (k + 0.5) * L / n, 0, zw - 34), (46, 46, 46), glow=G_BULB)
        g.box(STEEL, (0, 0, zw - 8), (L + 20, 14, 14))
        d = giant(r, 2.6, top=RED, bottom=WHITE, wear='trousers', sleeves='long', hair='pony')
        P.figure(sc, 0, 0, z=zw, yaw=90, legs='stride', arm_r='up', hold_r=hold_parasol(), arm_l='out', detail=1,
                 sway=dict(swing=1.85, pivot=(0.0, 0.0, zw - 40000.0), axis=90.0), rate=0.035, phase=r.random(), **d)
        lit(sc)
    return build


def kite_roof(seed=2, H=6000.0):
    """Three giants fly lit kites from a tower's crown, a child points up at them. Each kite (a diamond 5 m across
    with a tail) swings on its line about the flyer's hand."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        tower(g, 1400.0, 1400.0, H, seed)
        DW, DD = 2400.0, 1500.0
        top = deck(g, DW, DD, H, rim=CYAN, rim2=PINK, tiles=(MINT, YEL, CYAN, PINK))
        pairs = ((PINK, WHITE), (CYAN, YEL), (YEL, RED))
        for i, fx in enumerate((-760.0, 0.0, 760.0)):
            s = 2.6
            d = giant(r, s, wear=r.choice(('coat', 'trousers')), sleeves='long')
            s = d['s']
            P.figure(sc, fx, -330, z=top, yaw=r.uniform(-15, 15), arm_r='up', swing_r=9, arm_l=('hip', 'up', 'hip')[i], rate=0.3 + 0.05 * i,
                     phase=r.random(), detail=1, **d)
            hand = (fx + 39 * s, -330.0, top + 170 * s)
            kx, ky, kz = fx + (-500, 240, 620)[i], 260.0 + 160 * i, top + 2300 + 420 * ((i * 2) % 3)
            a, b = pairs[(i + seed) % 3]
            k = sc.part(P.motion(swing=(11.0, -8.0, 13.0)[i], pivot=hand, axis=90.0, rate=0.22 + 0.06 * i, phase=r.random()))
            k.box(a, (kx, ky, kz), (360, 10, 360), rot=(45, 0, 0), glow=0.7)
            k.box(b, (kx, ky - 7, kz), (30, 6, 500), glow=0.9)
            k.box(b, (kx, ky - 7, kz), (500, 6, 30), glow=0.9)
            for t in range(5):                                                            # the tail
                k.box((b, a)[t % 2], (kx + 40 * math.sin(t * 1.3), ky, kz - 330 - t * 120), (90, 8, 60), glow=0.7)
            tilt_box(k, WHITE, hand, (kx, ky, kz - 250), 9, 9)
        P.figure(sc, 380, -520, z=top, yaw=10, arm_r='point', swing_r=10, axis_r='x', arm_l='up', swing_l=16, rate=1.1, bob=16, phase=r.random(), detail=0,
                 **giant(r, 1.8, build='kid', top=YEL))
        for k in range(3):
            mini(sc, r, -1000 + k * 170, -560, top, s=2.2, bob=24.0, rate=1.2)
        lit(sc)
    return build


def roof_cinema(seed=3, H=5600.0):
    """An open-air picture show on a wide roof: a lit screen at the back (bands of colour, a sun, and a little car
    that runs to and fro across it: the film is a chase), a projector's hut, three rows of watchers on steps, the
    back row on its feet with its arms up."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        W, D = 3200.0, 2400.0
        g.box(SHAFT, (0, 0, H / 2.0), (W, D, H))
        for k in range(6):
            g.box((WARM, CYAN)[k % 2], (-W * 0.4 + k * W * 0.16, -D / 2.0 - 3, H - 460), (W * 0.11, 6, 240), glow=G_WIN)
            if k % 2:
                g.box((CYAN, WARM)[(k // 2) % 2], (-W * 0.4 + k * W * 0.16, -D / 2.0 - 3, H - 1100), (W * 0.11, 6, 240), glow=G_WIN)
        top = deck(g, W + 300, D + 200, H, tiles=(VIOLET, PINK, CYAN, PINK), n=8, rail=180.0)
        # the screen
        sy = D / 2.0 - 120.0
        sz = top + 1050.0
        SW, SH = 2500.0, 1300.0
        for sx in (-1, 1):
            g.box(STEEL, (sx * (SW / 2.0 - 60), sy + 40, top + 200), (40, 40, 400))
        g.box(DARK, (0, sy, sz), (SW + 80, 50, SH + 80))
        f = sy - 28.0
        g.box(VIOLET, (0, f, sz + SH * 0.25), (SW, 6, SH * 0.5), glow=1.2)                   # the sky
        g.box(PINK, (0, f, sz - SH * 0.1), (SW, 6, SH * 0.2), glow=1.3)
        g.box(ORANGE, (0, f, sz - SH * 0.35), (SW, 6, SH * 0.3), glow=1.2)                   # the road
        g.box(YEL, (SW * 0.22, f - 5, sz + SH * 0.16), (300, 6, 300), rot=(45, 0, 0), glow=2.0)   # the sun
        for k in range(4):
            g.box(NAVY, (-SW * 0.4 + k * SW * 0.2, f - 5, sz - SH * 0.02 + 60 * (k % 2)), (SW * 0.12, 6, 200 + 120 * (k % 2)), glow=0.3)
        car = sc.part(P.motion(swing=1.25, pivot=(0.0, f, sz - 40000.0), axis=90.0, rate=0.11, phase=0.2))
        car.box(CYAN, (0, f - 10, sz - SH * 0.3), (380, 6, 110), glow=2.2)
        car.box(WHITE, (-30, f - 10, sz - SH * 0.3 + 100), (190, 6, 90), glow=2.2)
        # the projector's hut at the front corner, its lamp
        g.box(DARK, (-W / 2.0 + 260, -D / 2.0 + 420, top + 170), (360, 320, 340))
        g.box(WARM_L, (-W / 2.0 + 260, -D / 2.0 + 590, top + 250), (120, 20, 90), glow=2.4)
        # the steps and the watchers (backs to the road, faces to the screen)
        for row in range(3):
            y = -D / 2.0 + 560 + row * 380
            zr = top + row * 110.0
            if row:
                g.box(GREY, (120, y + 40, top + row * 55.0), (2500, 380, row * 110.0))
            for i in range(8):
                x = -1000 + i * 320 + (row % 2) * 120 + r.uniform(-30, 30)
                stand = row == 2 or (i + row) % 5 == 0
                mini(sc, r, x, y, zr, s=2.2, up=stand, legs=stand, bob=(34.0 if stand else 9.0), rate=1.3)
        lit(sc)
    return build


def abseil(seed=4, H=6400.0):
    """Five climbers on ropes down the road face of a tower: each hangs at a height of its own, swings like a
    pendulum from the roof and rides slowly up and down. Two riggers at the rail above, one pointing, one waving."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        W, D = 1500.0, 1300.0
        tower(g, W, D, H, seed, cols=(CYAN, WARM), p=0.4)
        top = deck(g, W + 500, D + 300, H, rim=YEL, rim2=MINT, tiles=(MINT, YEL, CYAN, YEL), n=5)
        F = -D / 2.0 - 150.0 - 70.0                                                        # in front of the crown's edge
        ropes = (PINK, YEL, CYAN, MINT, ORANGE)
        for i in range(5):
            x = -720 + i * 360.0
            g.box(STEEL, (x, F + 110, H + 110), (16, 240, 16))                              # the davit
            zf = H - 1000 - ((i * 3 + seed) % 5) * 760.0
            mo = dict(phase=r.random(), rate=0.09 + 0.02 * ((i * 2) % 3))
            sway = dict(swing=(5.0, -6.5, 4.0, 6.0, -5.0)[i], pivot=(x, F, H + 110.0), axis=90.0)
            rp = sc.part(P.motion(swing=sway['swing'], pivot=sway['pivot'], axis=90.0, **mo))
            rp.box(ropes[i], (x, F, (H + 110 + zf - 500) / 2.0), (10, 10, H + 110 - zf + 500), glow=0.6)
            d = giant(r, 2.5, top=ropes[(i + 2) % 5], sleeves='long', wear='trousers', hat='helmet', hat_col=ropes[i], pack=ropes[(i + 1) % 5])
            P.figure(sc, x, F - 40, z=zf, yaw=180, legs='wide', arm_l='up', arm_r='up', detail=0, sway=sway, bob=-380.0, **dict(d, **mo))
        P.figure(sc, -420, -D / 2.0 - 60, z=top, yaw=0, arm_r='point', swing_r=10, axis_r='x', arm_l='hip', rate=0.8, phase=0.1, detail=1,
                 **giant(r, 2.4, top=ORANGE, hat='hard', hat_col=WHITE, band=WHITE, sleeves='long'))
        P.figure(sc, 380, -D / 2.0 - 60, z=top, yaw=0, arm_r='wave', swing_r=28, arm_l='up', swing_l=-14, rate=1.2, bob=12, phase=0.6, detail=1,
                 **giant(r, 2.4, top=ORANGE, hat='hard', hat_col=YEL, band=WHITE, sleeves='long'))
        lit(sc)
    return build


def far_stand(seed=5, H=5200.0):
    """A grandstand on a tall plinth: three stepped rows of eight in every hot colour, arms up, hopping out of step
    (a dense band of colour that jumps), a flag at each end, a string of bulbs over it."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        W, D = 3600.0, 1800.0
        g.box(SHAFT, (0, 0, H / 2.0), (W, D, H))
        for k in range(7):
            g.box((CYAN, WARM)[k % 2], (-W * 0.42 + k * W * 0.14, -D / 2.0 - 3, H - 420 - 520 * (k % 2)), (W * 0.09, 6, 240), glow=G_WIN)
        top = deck(g, W + 200, D + 200, H, rim=YEL, rim2=PINK, tiles=(PINK, YEL, CYAN, YEL), n=9, rail=170.0)
        for row in range(3):
            y = -D / 2.0 + 420 + row * 400
            zr = top + row * 170.0
            if row:
                g.box(GREY, (0, y + 100, top + row * 85.0), (3400, 600, row * 170.0))
            for i in range(8):
                x = -1420 + i * 405 + (row % 2) * 150 + r.uniform(-35, 35)
                mini(sc, r, x, y, zr, s=2.3, up=True, legs=(row == 0), bob=30.0 + 8.0 * ((i + row) % 3), rate=1.45)
        for sx in (-1, 1):
            g.box(STEEL, (sx * 1760, 500, top + 700), (18, 18, 1400))
            fl = sc.part(P.motion(swing=14.0, pivot=(sx * 1760.0, -640.0, top), axis=90.0, rate=0.7, phase=0.3 * (sx > 0)))
            fl.box(WHITE, (sx * 1760, -640, top + 500), (14, 14, 1000))
            fl.box((PINK, CYAN)[sx > 0], (sx * 1760 - sx * 190, -640, top + 880), (380, 8, 220), glow=0.8)
        bulbs(g, (-1760, 500, top + 1390), (1760, 500, top + 1390), 10, sag=260)
        lit(sc)
    return build


def _bus(sc, r, x, y, z, way, L, speed, col):
    """An open-top night-tour bus going `way` (+1 = +X) along the viaduct: a lit window band, lamps, five passengers
    on the top deck hopping and waving. Every part rides the same line."""
    tv = (L, way * speed / L)
    b = sc.part(P.motion(travel=tv, base=(x, y), pivot=(x, y, z + 150)))
    b.box(col, (x, y, z + 190), (1000, 280, 300))
    b.box(WARM_L, (x, y, z + 230), (900, 286, 90), glow=1.6)
    b.box(DARK, (x, y, z + 25), (860, 240, 50))
    b.box(WHITE, (x, y, z + 360), (1000, 290, 40))
    b.box(WARM_L, (x + way * 502, y, z + 130), (8, 200, 50), glow=3.0)
    b.box(RED, (x - way * 502, y, z + 130), (8, 200, 40), glow=1.6)
    for k in range(5):
        mini(sc, r, x - 380 + k * 190, y, z + 380, s=1.9, up=True, legs=False, bob=14.0, rate=1.3, travel=tv)


def _taxi(sc, x, y, z, way, L, speed):
    t = sc.part(P.motion(travel=(L, way * speed / L), base=(x, y), pivot=(x, y, z + 80)))
    t.box(YEL, (x, y, z + 80), (460, 200, 110))
    t.box(DARK, (x - way * 20, y, z + 170), (240, 190, 80))
    t.box(WHITE, (x - way * 20, y, z + 225), (70, 60, 30), glow=2.0)
    t.box(WARM_L, (x + way * 232, y, z + 80), (8, 170, 40), glow=3.0)
    t.box(RED, (x - way * 232, y, z + 80), (8, 170, 30), glow=1.6)


def viaduct(seed=6, L=7000.0, H=5200.0):
    """Far traffic with people on it: a high road between two blocks, 52 m up. Two open-top night-tour buses (one
    each way, their top decks full of waving passengers) and taxis with lit roof lamps run it and come round again."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h = L / 2.0
        for sx in (-1, 1):
            cx = sx * (h + 450)
            tower(g, 900.0, 1100.0, H + 900, seed + (sx > 0), x=cx, p=0.45)
            g.box((PINK, CYAN)[sx > 0], (cx, 0, H + 930), (960, 1160, 60), glow=G_RIM)
        g.box(GREY, (0, 0, H - 60), (L, 760, 120))
        g.box(SHAFT_L, (0, 0, H - 260), (L, 240, 280))
        g.box(MINT, (0, -385, H - 40), (L, 10, 150), glow=G_RIM)
        for k in range(3):
            g.box(SHAFT, (-h * 0.5 + k * h * 0.5, 0, (H - 400) / 2.0), (320, 320, H - 400))
        for k in range(6):
            x = -h + (k + 0.5) * L / 6.0
            g.box(STEEL, (x, 360, H + 300), (14, 14, 600))
            g.box(WARM_L, (x, 320, H + 600), (70, 130, 40), glow=2.4)
        z = H
        _bus(sc, r, -h * 0.45, -170, z, 1, L, 800.0, RED)
        _bus(sc, r, h * 0.3, 170, z, -1, L, 700.0, CYAN)
        for k, x in enumerate((-h * 0.85, h * 0.1, h * 0.7)):
            _taxi(sc, x, -170, z, 1, L, 800.0)
        for k, x in enumerate((-h * 0.6, -h * 0.1, h * 0.8)):
            _taxi(sc, x, 170, z, -1, L, 700.0)
        lit(sc)
    return build


# ------------------------------------------------------------------------------------------ the sky
def lanterns(seed=7, n=22, L=14000.0, speed=240.0, cols=(WARM, ORANGE, CREAM, PINK), way=1, rise=1.0):
    """A drift of paper lanterns let go from the roofs: each a lit paper shade 3 m tall with its flame under it,
    riding up and down on the air at a pace of its own; the whole drift goes by along the mesh's X, climbing as it
    goes, and comes round again. Built from z = 0 to about 55 m: the table hangs it in the sky."""
    def build(sc):
        r = random.Random(seed)
        for i in range(n):
            x = -L / 2.0 + (i + r.uniform(0.15, 0.85)) * L / n
            y = r.uniform(-1500, 1500)
            z = 2600.0 + rise * way * 2000.0 * (x / (L / 2.0)) + r.uniform(-600, 600)
            s = r.uniform(0.85, 1.25)
            m = sc.part(P.motion(travel=(L, way * speed / L), base=(x, y), pivot=(x, y, z), bob=-r.uniform(160, 420), rate=r.uniform(0.05, 0.09), phase=r.random()))
            m.cyl(cols[i % len(cols)], (x, y, z), 105 * s, 290 * s, steps=6, top_r=160 * s, glow=1.7)
            m.box(YEL, (x, y, z - 22 * s), (70 * s, 70 * s, 44 * s), glow=3.0)
    return build


def _balloon(sc, r, x, y, z, a, b, tv, s=1.0):
    """One hot-air balloon, its envelope 16 m across and lit from inside in two colours, a basket, two passengers
    waving over its rim. base of the basket at (x, y, z)."""
    m = sc.part(P.motion(travel=tv, base=(x, y), pivot=(x, y, z + 900 * s)))
    e0 = z + 560 * s
    for (r0, r1, hh, col, gl) in ((170, 700, 800, a, 0.8), (700, 820, 360, b, 0.9), (820, 700, 360, a, 0.8), (700, 260, 380, b, 0.9), (260, 50, 120, a, 0.8)):
        m.cyl(col, (x, y, e0), r0 * s, hh * s, steps=10, top_r=r1 * s, glow=gl)
        e0 += hh * s
    m.box(WOOD, (x, y, z + 80 * s), (280 * s, 220 * s, 160 * s))
    m.box(a, (x, y, z + 166 * s), (300 * s, 240 * s, 16 * s), glow=0.5)
    for sx in (-1, 1):
        for sy in (-1, 1):
            tilt_box(m, WHITE, (x + sx * 130 * s, y + sy * 100 * s, z + 160 * s), (x + sx * 150 * s, y + sy * 150 * s, z + 600 * s), 9 * s, 9 * s)
    m.box(STEEL, (x, y, z + 420 * s), (90 * s, 90 * s, 60 * s))
    m.box(ORANGE, (x, y, z + 500 * s), (60 * s, 60 * s, 110 * s), glow=3.0)
    for k, sx in enumerate((-1, 1)):
        d = giant(r, 1.9 * s)
        P.figure(sc, x + sx * 70 * s, y - 40 * s, z=z + 20 * s, yaw=sx * 20, arm_r='wave' if sx > 0 else 'up', swing_r=28, arm_l='up' if sx > 0 else 'hip',
                 swing_l=-18 if sx > 0 else 0, rate=1.1 + 0.2 * k, phase=r.random(), detail=0, travel=(tv[0], tv[1] * tv[0]), **d)


def balloons(seed=9, L=24000.0, speed=210.0, way=1, pairs=((PINK, WHITE), (CYAN, YEL)), three=False):
    """Hot-air balloons on a night flight over the far roofs, lit from inside (a steady light), two passengers
    waving from each basket. They go by along the mesh's X, 240 m of it, and come round again. Built from z = 0."""
    def build(sc):
        r = random.Random(seed)
        tv = (L, way * speed / L)
        spots = [(-L * 0.27, -300.0, 0.0, 1.0), (L * 0.2, 500.0, 1500.0, 0.85)]
        if three:
            spots.append((-L * 0.02, 1400.0, 2700.0, 0.7))
        for i, (x, y, z, s) in enumerate(spots):
            a, b = pairs[i % len(pairs)]
            _balloon(sc, r, x, y, z, a, b, tv, s)
        lit(sc)
    return build


PALE = (('#f4f1ea', '#c9c4d6'), ('#c9c4d6', '#a39cc2'), ('#fff6ea', '#d7e3f0'))


def murmur_a(seed=11, n=36):
    """Starlings wheeling over the roofs: three rings that turn at paces of their own (the middle one the other
    way), every bird 3.6 m across with its wings beating. Built from z = 0 to about 20 m."""
    def build(sc):
        r = random.Random(seed)
        for i in range(n):
            ring = i % 3
            R = (700.0, 1500.0, 2400.0)[ring] + r.uniform(-260, 260)
            a = i * (360.0 / n) * 2.6 + r.uniform(-12, 12)
            way = 1 if ring != 1 else -1
            rounds = way * (0.075, 0.05, 0.036)[ring]
            x, y = math.cos(math.radians(a)) * R, math.sin(math.radians(a)) * R
            z = 300 + ring * 420 + r.uniform(0, 800)
            hd = a + 90.0 * way
            tx, ty = math.cos(math.radians(hd)), math.sin(math.radians(hd))
            col, wcol = PALE[i % 3]
            common = dict(phase=r.random(), rate=r.uniform(2.2, 3.0), base=(0.0, 0.0), travel=(-1.0, rounds))
            b = sc.part(P.motion(pivot=(x, y, z), **common))
            tilt_box(b, col, (x - tx * 80, y - ty * 80, z), (x + tx * 80, y + ty * 80, z + 8), 44, 40)
            for side in (-1, 1):
                w = sc.part(P.motion(swing=side * 34.0, pivot=(x, y, z + 10), axis=hd, **common))
                wx, wy = -ty * side, tx * side
                tilt_box(w, wcol, (x + wx * 18, y + wy * 18, z + 12), (x + wx * 180 - tx * 40, y + wy * 180 - ty * 40, z + 30), 78, 8)
        lit(sc, 0.32)
    return build


def murmur_b(seed=12, L=20000.0, speed=900.0, slices=13, per=5):
    """A ribbon of starlings on the move: sixty-five birds in a band 36 m long that snakes through the air, each
    slice of it rising and falling a beat after the one ahead (a wave runs down the ribbon). It travels the mesh's
    X, 200 m of it, and comes round again. Built round z = 10 m."""
    def build(sc):
        r = random.Random(seed)
        tv = (L, speed / L)
        for k in range(slices):
            cx = (slices / 2.0 - k) * 280.0
            cy = 520.0 * math.sin(k * 0.55)
            cz = 1000.0 + 420.0 * math.sin(k * 0.45 + 1.0)
            m = sc.part(P.motion(travel=tv, base=(cx, cy), pivot=(cx, cy, cz), bob=-330.0, rate=0.3, phase=(k * 0.11) % 1.0))
            fat = 1.0 - 0.5 * abs(k - slices / 2.0) / (slices / 2.0)
            for j in range(per):
                x = cx + r.uniform(-130, 130)
                y = cy + r.uniform(-380, 380) * fat
                z = cz + r.uniform(-330, 330) * fat
                col = PALE[(k + j) % 3][j % 2]
                for side in (-1, 1):
                    tilt_box(m, col, (x + 40, y, z), (x - 70, y + side * 175, z + 26), 62, 8)
        lit(sc, 0.32)
    return build


def dragon_kite(seed=13, n=18):
    """A dragon kite 40 m long, flown from somewhere down among the roofs: a head with horns and eyes, and eighteen
    lit diamonds in a line behind it that rise and fall one after the other, so a wave runs down the dragon. Its
    line runs away down to the front. Built from z = 0 to about 45 m: the table hangs it in the sky."""
    def build(sc):
        r = random.Random(seed)
        foot = (2600.0, -200.0, 0.0)
        cols = (PINK, YEL, CYAN, MINT, ORANGE, VIOLET)
        hx, hz = 700.0, 4000.0

        def mo(k):
            return P.motion(swing=3.0, pivot=foot, axis=90.0, bob=-170.0, rate=0.3, phase=(k * 0.075) % 1.0)

        hd = sc.part(mo(0))
        hd.box(RED, (hx, 0, hz), (300, 60, 240), glow=0.8)
        hd.box(YEL, (hx + 170, 0, hz - 50), (120, 70, 110), glow=0.9)
        for sy in (-1, 1):
            hd.box(WHITE, (hx + 60, sy * 34, hz + 50), (56, 8, 56), glow=2.0)
            tilt_box(hd, YEL, (hx - 40, sy * 20, hz + 110), (hx - 190, sy * 60, hz + 300), 26, 26, glow=0.9)
        tilt_box(hd, WHITE, (hx + 150, 0, hz - 120), foot, 10, 10)
        for k in range(1, n + 1):
            x = hx - 160 - k * 215.0
            z = hz - 20 - k * 38.0 + 90.0 * math.sin(k * 0.8)
            sz = 250.0 - k * 6.0
            m = sc.part(mo(k))
            m.box(cols[k % len(cols)], (x, 0, z), (sz, 10, sz), rot=(45, 0, 0), glow=0.85)
            m.box(WHITE, (x, 0, z - sz * 0.9), (26, 8, 150), glow=0.6)
        tl = sc.part(mo(n + 1))
        xe = hx - 160 - (n + 1) * 215.0
        for j in range(3):
            tilt_box(tl, cols[j], (xe + 90, (j - 1) * 30, hz - 20 - (n + 1) * 38.0), (xe - 420, (j - 1) * 120, hz - 260 - (n + 1) * 38.0 - j * 90), 40, 8, glow=0.8)
        lit(sc)
    return build


FAR = dict(far=True)
JOBS = [
    ('SM_P3E_HighWire', high_wire(1), FAR),
    ('SM_P3E_KiteRoof', kite_roof(2), dict(far=True, grow=(700.0, 300.0, 500.0), grow_down=(700.0, 300.0, 0.0))),
    ('SM_P3E_RoofCinema', roof_cinema(3), FAR),
    ('SM_P3E_Abseil', abseil(4), dict(far=True, grow=(500.0, 100.0, 100.0), grow_down=(500.0, 100.0, 0.0))),
    ('SM_P3E_FarStand', far_stand(5), FAR),
    ('SM_P3E_Viaduct', viaduct(6), FAR),
    ('SM_P3E_Lanterns_A', lanterns(7), dict(far=True, grow=(300.0, 100.0, 500.0), grow_down=(300.0, 100.0, 500.0))),
    ('SM_P3E_Lanterns_B', lanterns(8, n=20, cols=(CYAN, MINT, VIOLET, YEL, PINK), way=-1, rise=0.6), dict(far=True, grow=(300.0, 100.0, 500.0), grow_down=(300.0, 100.0, 500.0))),
    ('SM_P3E_Balloons_A', balloons(9), dict(far=True, grow=(1000.0, 100.0, 100.0), grow_down=(1000.0, 100.0, 0.0))),
    ('SM_P3E_Balloons_B', balloons(10, way=-1, pairs=((ORANGE, VIOLET), (MINT, WHITE), (YEL, RED)), three=True), dict(far=True, grow=(1000.0, 100.0, 100.0), grow_down=(1000.0, 100.0, 0.0))),
    ('SM_P3E_Murmur_A', murmur_a(11), dict(far=True, grow=(400.0, 400.0, 250.0), grow_down=(400.0, 400.0, 250.0))),
    ('SM_P3E_Murmur_B', murmur_b(12), dict(far=True, grow=(10600.0, 100.0, 450.0), grow_down=(10600.0, 100.0, 450.0))),
    ('SM_P3E_DragonKite', dragon_kite(13), dict(far=True, grow=(400.0, 100.0, 300.0), grow_down=(400.0, 100.0, 300.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
