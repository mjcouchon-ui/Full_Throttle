"""FULL THROTTLE (Unreal) the population, level 7: City Center, FAR LIFE AND CREATURES. /Game/People/CityCenter/SM_Far*.

The extra worker's kit for this level (focus "far"): everything living from about 90 m out to the horizon, and the
level's creatures in the sky. The main worker's kit (ft_people_city.py) has the street, the podiums' roofs and a
pigeon flock; nothing here stands nearer than 60 m, and nothing here stands on the dressing.

HOW THE LAND LIES, and so what a far piece must be (FTDressL7.inl's header): the avenue is a canyon. Podiums 5.5 to
12.4 m tall stand 25.5 m out, a tower behind every one, more towers at 51 to 82 m, mid-rise blocks 32 and 44 m tall at
96 to 160 m. It is midnight. Far life on the ground is never seen from the road. So every piece here
  - BRINGS ITS OWN HEIGHT: a plain dark tower or plinth with a few lit windows (not a building of the dressing's: a
    stand for people, as a watchers' piece brings its balcony), its people on the crown, 40 to 60 m up;
  - is LIT: steady light only (a lit floor, strings of bulbs, a lit rim), and a faint light on every figure, so that
    the colours of a crowd read against the night. Nothing blinks. Everything stands 60 m and more from the road;
  - is BIG: figures are drawn at 2 to 2.8 times life size (a dancer on a far roof is 4.5 m tall), birds 3 to 5 m
    across. At 100 to 200 m that is what a person-sized thing would be at 40 to 70 m;
  - keeps moving: every piece is saved with far=True (the always-moving master).
Stripes, discs and squares only: never words, never a chevron (the Syndicate's mark).

Run in the editor:  import ft_people_city_far as K; K.run()        (or K.run(['SM_FarTowerParty']))
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
    """Night: everything of a piece that carries no light of its own gets a faint one (figures under the party's
    lamps). Every piece of this kit stands 60 m and more from the road (the light rule asks 24 m)."""
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
    """A citizen in a coat that reads at night, drawn `s` times life size."""
    d = P.look(r, tops=HOT, **fixed)
    d['s'] = s * r.uniform(0.92, 1.05)
    return d


def tower(g, W, D, H, seed, z_win=2200.0, pitch=520.0, cols=(WARM, CYAN)):
    """The stand a far piece brings: a plain dark shaft W by D by H with lit windows on its upper floors (the lower
    ones are behind the street wall) and a lit band under the crown."""
    r = random.Random(seed)
    g.box(SHAFT, (0, 0, H / 2.0), (W, D, H))
    z = z_win
    while z < H - 500.0:
        col = cols[int(z / pitch) % 2]
        for k in range(4):
            if r.random() < 0.62:
                g.box(col, (-W * 0.36 + k * W * 0.24, -D / 2.0 - 3, z), (W * 0.18, 6, 190), glow=G_WIN)
        for sx in (-1, 1):
            if r.random() < 0.5:
                g.box(col, (sx * (W / 2.0 + 3), r.choice((-1, 0, 1)) * D * 0.27, z), (6, D * 0.2, 190), glow=G_WIN)
        z += pitch


def deck(g, W, D, H, y=0.0, rim=PINK, rim2=CYAN, tiles=(PINK, CYAN, YEL, VIOLET), n=8, rail=260.0):
    """A crown: a slab wider than the shaft, a lit rim round it, a lit floor of squares along its front, a rail."""
    g.box(GREY, (0, y, H + 35), (W, D, 70))
    g.box(rim, (0, y - D / 2.0 - 5, H + 30), (W + 10, 10, 80), glow=G_RIM)
    for sx in (-1, 1):
        g.box(rim2, (sx * (W / 2.0 + 5), y, H + 30), (10, D, 80), glow=G_RIM)
    tw = W / n
    for i in range(n):
        g.box(tiles[i % len(tiles)], (-W / 2.0 + (i + 0.5) * tw, y - D / 2.0 + D * 0.3, H + 73), (tw - 20, D * 0.55, 6), glow=G_FLOOR)
    g.box(STEEL, (0, y - D / 2.0 + 12, H + 70 + rail), (W, 12, 12))
    for k in range(7):
        g.box(STEEL, (-W / 2.0 + 6 + k * (W - 12) / 6.0, y - D / 2.0 + 12, H + 70 + rail / 2.0), (10, 10, rail))
    return H + 76.0


def bulbs(g, a, b, n, sag=120.0, cols=(WARM, PINK, CYAN, YEL), size=46.0):
    """A string of big bulbs from a to b (steady light)."""
    for k in range(n):
        t = (k + 0.5) / n
        s = sag * 4.0 * t * (1.0 - t)
        g.box(cols[k % len(cols)], (a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t, a[2] + (b[2] - a[2]) * t - s), (size, size, size), glow=G_BULB)


CHEER = (
    dict(arm_l='up', arm_r='up', swing_l=16, swing_r=-16, bob=26, rate=1.5),
    dict(arm_r='wave', swing_r=30, rate=1.2, bob=10),
    dict(arm_l='vee', arm_r='vee', swing_l=12, swing_r=-12, bob=30, rate=1.3),
    dict(arm_l='wave', arm_r='wave', swing_l=-24, swing_r=24, rate=1.0, bob=14),
    dict(arm_r='up', arm_l='hip', swing_r=20, rate=1.7, bob=18),
)
DANCE = (
    dict(arm_l='vee', arm_r='vee', swing_l=18, swing_r=-18, bob=30, rate=1.4),
    dict(arm_l='out', arm_r='up', turn=28, rate=0.8),
    dict(arm_l='up', arm_r='up', rock=11, rate=0.9),
    dict(arm_l='hip', arm_r='wave', swing_r=30, rate=1.4, bob=20),
    dict(arm_l='up', arm_r='out', swing_l=20, bob=32, rate=1.5),
)


# ------------------------------------------------------------------------------------------ the pieces
def tower_party(seed=1, H=5600.0):
    """A party on the crown of a far tower, 56 m up: giants dancing on a lit floor under strings of bulbs, a row at the
    rail waving at the chase, bunches of balloons riding at both ends."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        tower(g, 1500.0, 1300.0, H, seed)
        DW, DD = 2700.0, 1500.0
        top = deck(g, DW, DD, H, y=-100.0)
        h = DW / 2.0
        for sx in (-1, 1):
            g.box(STEEL, (sx * (h - 20), -800, top + 550), (16, 16, 1100))
        g.box(STEEL, (0, 500, top + 700), (18, 18, 1400))
        bulbs(g, (-h + 20, -800, top + 1090), (h - 20, -800, top + 1090), 11, sag=170)
        bulbs(g, (-h + 20, -800, top + 1090), (0, 500, top + 1390), 6, sag=90)
        bulbs(g, (h - 20, -800, top + 1090), (0, 500, top + 1390), 6, sag=90)
        # the desk and its stacks at the back
        g.box(DARK, (0, 380, top + 130), (620, 160, 260))
        for k in range(5):
            g.box((PINK, CYAN, YEL, CYAN, PINK)[k], (-240 + k * 120, 296, top + 130), (90, 8, 60 + 34 * (k % 3)), glow=1.2)
        for sx in (-1, 1):
            g.box(DARK, (sx * 520, 400, top + 230), (220, 200, 460))
        P.figure(sc, 0, 520, z=top, arm_l='fwd', arm_r='up', swing_r=18, rate=1.5, bob=14, phase=r.random(), detail=0,
                 **giant(r, 2.5, hat='headset', hat_col=PINK))
        # balloons: bunches that ride on the air at both ends
        for sx in (-1, 1):
            for k in range(4):
                mo = P.motion(bob=-45.0, rate=0.22 + 0.05 * k, phase=r.random(), swing=5.0, pivot=(sx * (h - 20), -800, top + 300), axis=0.0)
                b = sc.part(mo)
                bx, bz = sx * (h + 60 + (k % 2) * 190), top + 620 + k * 230
                b.sphere(HOT[(k * 3 + seed + (sx > 0)) % len(HOT)], (bx, -800, bz), 150, steps=(4, 7), glow=0.5)
                tilt_box(b, WHITE, (sx * (h - 20), -800, top + 300), (bx, -800, bz - 140), 6, 6)
        n = 13
        for i in range(n):
            atrail = i % 2 == 0
            x = -h + 170 + i * (DW - 340) / (n - 1) + r.uniform(-25, 25)
            d = giant(r, 2.6)
            d.update(CHEER[(i + seed) % len(CHEER)] if atrail else DANCE[(i * 3 + seed) % len(DANCE)])
            P.figure(sc, x, (-740 if atrail else -330) + r.uniform(-20, 20), z=top, yaw=r.uniform(-25, 25), phase=r.random(), detail=0, **d)
        lit(sc)
    return build


def concert(seed=2, H=2400.0):
    """A far crowd on a lit plaza: a raised square (its own plinth, 24 m up, over the podiums' roofs), a stage at the
    back under a wall of light bars, a band of three giants, and a crowd of twenty-four with its arms up, hopping out
    of step, backs to the avenue."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        W, D = 3400.0, 2400.0
        g.box(SHAFT, (0, 0, H / 2.0), (W, D, H))
        for k in range(6):                                                    # a band of lit doors and windows under the square
            g.box((WARM, CYAN)[k % 2], (-W * 0.4 + k * W * 0.16, -D / 2.0 - 3, H - 420), (W * 0.11, 6, 240), glow=G_WIN)
        top = deck(g, W, D, H, tiles=(VIOLET, PINK, CYAN, PINK), n=10, rail=200.0)
        # the stage: a platform at the back, a wall of light bars behind it, a truss over it, stacks at both ends
        sy, sz = D / 2.0 - 360.0, top + 300.0
        g.box(DARK, (0, sy, top + 150), (2300, 620, 300))
        g.box(YEL, (0, sy - 312, top + 150), (2300, 6, 60), glow=1.0)
        for k in range(9):
            hh = 620 + 260 * ((k * 5 + seed) % 3)
            g.box((PINK, CYAN, YEL, VIOLET, CYAN)[k % 5], (-960 + k * 240, sy + 290, sz + hh / 2.0), (150, 14, hh), glow=1.5)
        for sx in (-1, 1):
            g.box(STEEL, (sx * 1180, sy - 200, sz + 600), (26, 26, 1200))
            g.box(DARK, (sx * 1420, sy - 120, top + 380), (340, 300, 760))
            g.cyl(STEEL, (sx * 1420, sy - 275, top + 520), 110, 8, rot=(0, 0, 90), steps=8)
        g.box(STEEL, (0, sy - 200, sz + 1200), (2400, 30, 30))
        for k in range(6):
            g.box(WARM_L, (-1000 + k * 400, sy - 200, sz + 1160), (90, 90, 60), glow=2.4)
        # the band
        P.figure(sc, 0, sy - 180, z=sz, arm_r='face', arm_l='up', swing_l=22, rock=0, rate=1.1, bob=22, phase=0.1, detail=1,
                 **giant(r, 2.5, top=RED, hair='long'))
        P.figure(sc, -640, sy - 120, z=sz, arm_l='out', arm_r='hold', swing_r=34, axis_r='x', rate=2.2, bob=0, phase=0.4, detail=1,
                 hold_r=lambda m, hd, side: m.box(CYAN, (hd[0] - 34, hd[1] - 8, hd[2] + 6), (96, 10, 30)), **giant(r, 2.4, top=WHITE, hair='mohawk', hair_col=PINK))
        g.cyl(PINK, (640, sy - 110, sz), 110, 150, steps=8, glow=0.4)
        g.cyl(YEL, (780, sy - 150, sz + 210), 90, 8, steps=8, glow=0.4)
        P.figure(sc, 640, sy + 60, z=sz, arm_l='fwd', arm_r='fwd', swing_l=30, swing_r=-30, axis_l='x', axis_r='x', rate=2.6, phase=0.7, detail=1,
                 **giant(r, 2.3, top=YEL))
        # the crowd: three rows, facing the stage (backs to the road), arms up
        k = 0
        for row in range(3):
            y = -D / 2.0 + 330 + row * 330
            for i in range(8):
                x = -W / 2.0 + 330 + i * (W - 660) / 7.0 + (row % 2) * 110 + r.uniform(-30, 30)
                d = giant(r, 1.9)
                d.update(CHEER[(k * 2 + seed) % len(CHEER)])
                if k % 4 == 0:
                    d.update(arm_r='wave', swing_r=18, hold_r=P.hold_flag(col=HOT[k % len(HOT)], pole=WHITE, w=90.0), rate=0.9, bob=12)
                P.figure(sc, x, y + r.uniform(-25, 25), z=top, yaw=180 + r.uniform(-20, 20), phase=r.random(), detail=0, **d)
                k += 1
        lit(sc)
    return build


def hold_long_flag(col=PINK, pole=WHITE, length=420.0, w=170.0, glow=0.5):
    """The pigeon keeper's cane: a long pole with a rag on its end, swung to keep the flock up."""
    def f(m, h, side):
        m.box(pole, (h[0], h[1], h[2] + length / 2.0 - 30), (7, 7, length))
        m.box(col, (h[0] + side * w / 2.0, h[1], h[2] + length - 70), (w, 5, 90), glow=glow)
    return f


PIGEON = (('#f4f1ea', '#c9c4d6'), ('#c9c4d6', '#8f9bb3'), ('#fff6ea', '#a39cc2'))


def tower_loft(seed=3, H=4800.0):
    """A pigeon loft on the crown of a far tower: the keeper swings a long cane with a rag on it, a child points, the
    flock wheels over the roof in three rings (birds 4 m across), a row sits on the rail. On the tower's face a
    window cleaners' cradle rides slowly up and down its cables, two giants in yellow at work in it."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        W, D = 1400.0, 1400.0
        tower(g, W, D, H, seed, cols=(CYAN, WARM))
        DW, DD = 2200.0, 1500.0
        top = deck(g, DW, DD, H, rim=YEL, rim2=PINK, tiles=(MINT, YEL, CYAN, YEL), n=6)
        # the loft: a shed on legs with a striped roof, a landing board, a lit door
        lx, ly = 560.0, 300.0
        for sx in (-1, 1):
            for sy in (-1, 1):
                g.box(WOOD, (lx + sx * 300, ly + sy * 200, top + 90), (24, 24, 180))
        g.box(WOOD, (lx, ly, top + 400), (700, 500, 460))
        for k in range(5):
            g.box((RED, WHITE)[k % 2], (lx - 280 + k * 140, ly, top + 660), (140, 560, 60))
        g.box(WARM_L, (lx, ly - 253, top + 420), (360, 6, 240), glow=1.4)
        g.box(WOOD, (lx, ly - 330, top + 280), (520, 160, 14))
        bulbs(g, (-DW / 2.0 + 20, -DD / 2.0 + 20, top + 700), (lx - 350, ly - 250, top + 640), 6, sag=110, cols=(WARM, YEL))
        g.box(STEEL, (-DW / 2.0 + 20, -DD / 2.0 + 20, top + 350), (14, 14, 700))
        P.figure(sc, -420, -300, z=top, yaw=10, arm_r='up', swing_r=32, axis_r='y', hold_r=hold_long_flag(), arm_l='hip', rate=0.55, phase=0.2, detail=1,
                 **giant(r, 2.7, top=ORANGE, hat='cap', hat_col=RED, wear='coat', sleeves='long'))
        P.figure(sc, 40, -480, z=top, yaw=-10, arm_r='point', swing_r=12, axis_r='x', arm_l='up', swing_l=18, rate=1.1, bob=18, phase=0.6, detail=1,
                 **giant(r, 1.8, build='kid', top=CYAN))
        for k in range(7):                                                   # the row on the rail
            b, w = PIGEON[k % 3]
            P.bird(sc, -DW / 2.0 + 260 + k * 250 + r.uniform(-40, 40), -DD / 2.0 + 12, top + 290, heading=r.choice((-90, -70, -110)), span=190, body=b, wing=w,
                   beak=ORANGE, phase=r.random(), rate=r.uniform(0.9, 1.6), flap=4.0, bob=10.0)
        n = 15
        for i in range(n):                                                   # the flock, wheeling
            ring = i % 3
            R = (800.0, 1250.0, 1750.0)[ring] + r.uniform(-90, 90)
            a = i * (360.0 / n) * 2.3 + r.uniform(-14, 14)
            way = 1 if ring != 1 else -1
            b, w = PIGEON[i % 3]
            P.bird(sc, math.cos(math.radians(a)) * R, math.sin(math.radians(a)) * R, top + 900 + ring * 330 + r.uniform(-120, 120), heading=a + 90.0 * way,
                   span=r.uniform(360, 460), body=b, wing=w, beak=ORANGE, phase=r.random(), rate=r.uniform(1.6, 2.2), flap=36.0,
                   orbit=(0, 0, way * (0.085, 0.06, 0.045)[ring]))
        # the cradle on the face
        F = -D / 2.0
        for sx in (-1, 1):
            g.box(STEEL, (sx * 430, F - 150, H - 1500), (12, 12, 3000))
            tilt_box(g, STEEL, (sx * 430, F + 60, H + 76), (sx * 430, F - 160, H + 10), 22, 22)
        mo = dict(bob=-620.0, rate=0.07, phase=0.3)
        z0 = H - 1900.0
        c = sc.part(P.motion(**mo))
        c.box(STEEL, (0, F - 150, z0), (1040, 220, 22))
        for sy in (-1, 1):
            c.box(YEL, (0, F - 150 + sy * 104, z0 + 230), (1040, 14, 18), glow=0.5)
            c.box(YEL, (0, F - 150 + sy * 104, z0 + 120), (1040, 12, 14), glow=0.5)
        for sx in (-1, 0, 1):
            c.box(YEL, (sx * 512, F - 254, z0 + 120), (14, 14, 240), glow=0.5)
        for k in range(5):
            c.box(DARK if k % 2 else YEL, (-416 + k * 208, F - 262, z0 + 50), (208, 8, 70), glow=0.0 if k % 2 else 0.5)
        c.box(WARM_L, (-470, F - 240, z0 + 290), (60, 40, 50), glow=2.2)
        for k, x in enumerate((-250.0, 280.0)):
            d = giant(r, 2.25, top=YEL, bottom=NAVY, sleeves='long', wear='trousers', hat='hard', hat_col=WHITE, band=WHITE)
            d.update(mo)
            P.figure(sc, x, F - 140, z=z0 + 11, yaw=180, arm_r='up', swing_r=30, axis_r='y', hold_r=P.hold_tool(col=STEEL, grip=CYAN, length=30.0),
                     arm_l='hip' if k else 'out', detail=1, **d)
        lit(sc)
    return build


def dog(sc, r, x, y, z, s, tv, phase):
    col = r.choice(('#e8c26a', '#fff6ea', '#c9a070', '#8e5a3a', '#cfcfd8'))
    P.quadruped(sc, x, y, z, yaw=90 if tv[1] > 0 else -90, s=s, length=70, height=48, girth=20, neck=16, neck_up=35, head=18, tail=22, wag=34, stride=26,
                rate=2.8, phase=phase, travel=tv, body=col, ears=col)


def sky_run(seed=4, L=6400.0, H=3000.0):
    """The roof-top running track: a lit bridge between two slim towers, 30 m up. Joggers in bright kit run it one way,
    dog walkers go the other with their dogs ahead of them (all at 2.4 times life size), and come round again."""
    def build(sc):
        r = random.Random(seed)
        g = sc.still()
        h = L / 2.0
        for sx in (-1, 1):
            g.box(SHAFT, (sx * (h + 350), 0, (H + 900) / 2.0), (700, 900, H + 900))
            for k in range(4):
                g.box((WARM, CYAN)[(k + (sx > 0)) % 2], (sx * (h + 350), -453, H - 1500 + k * 560), (420, 6, 200), glow=G_WIN)
            g.box((PINK, CYAN)[sx > 0], (sx * (h + 350), 0, H + 930), (760, 960, 60), glow=G_RIM)
        g.box(GREY, (0, 0, H - 60), (L, 620, 120))
        g.box(SHAFT_L, (0, 0, H - 260), (L, 200, 280))
        g.box(MINT, (0, -315, H - 60), (L, 10, 120), glow=G_RIM)
        n = 8
        for i in range(n):                                                    # the track: lit lanes
            g.box((ORANGE, RED)[i % 2], (-h + (i + 0.5) * L / n, -60, H + 3), (L / n - 30, 420, 6), glow=G_FLOOR)
        g.box(STEEL, (0, -300, H + 250), (L, 12, 12))
        for k in range(9):
            x = -h + k * L / 8.0
            g.box(STEEL, (x, -300, H + 125), (10, 10, 250))
            if k % 2 == 1:
                g.box(STEEL, (x, 290, H + 350), (14, 14, 700))
                g.box(WARM_L, (x, 250, H + 700), (70, 130, 40), glow=2.4)
        top = H + 6.0
        nj = 5
        for i in range(nj):                                                   # joggers, towards +X
            x = -h + (i + r.uniform(0.2, 0.8)) * L / nj
            d = giant(r, 2.4, wear='shorts', sleeves='short', hat=r.choice((None, 'cap', None)), hat_col=r.choice(HOT),
                      band=r.choice((WHITE, YEL, None)))
            P.figure(sc, x, -150 + (i % 2) * 110, z=top, yaw=90, legs='stride', stride=38, arm_l='carry', arm_r='carry', swing_l=30, swing_r=-30,
                     axis_l='x', axis_r='x', rate=2.4, bob=14, phase=r.random(), travel=(L, 760.0), detail=1, **d)
        nw = 3
        for i in range(nw):                                                   # dog walkers, towards -X, the dogs ahead on the lead
            x = -h + (i + r.uniform(0.2, 0.8)) * L / nw
            tv = (L, -300.0)
            ph = r.random()
            d = giant(r, 2.4, wear=r.choice(('coat', 'trousers', 'dress')), sleeves='long', hat=r.choice((None, 'beanie')), hat_col=r.choice(HOT))
            P.figure(sc, x, 120, z=top, yaw=-90, legs='stride', stride=22, arm_l='down', arm_r='fwd', swing_l=16, axis_l='x', rate=1.0, bob=6, phase=ph,
                     travel=tv, detail=1, **d)
            nd = 1 + (i + seed) % 3
            for k in range(nd):
                dog(sc, r, x - 330 - k * 150, 60 + k * 90, top, 2.2 if k != 1 else 1.5, tv, r.random())
        lit(sc)
    return build


def glider(sc, x, y, z, cols, phase, tv, span=1200.0, suit=YEL):
    """One hang-glider flying the mesh's +X: a delta wing in two colours, lit from the street below, its pilot
    hanging prone under it in a bright suit, one arm waving. It rides up and down on the air and rocks."""
    mo = dict(travel=tv, base=(x, y), bob=-170.0, rate=0.16, phase=phase, swing=9.0, pivot=(x, y, z), axis=0.0)
    w = sc.part(P.motion(**mo))
    hs = span / 2.0
    w.plan(cols[0], [(x + 520, y), (x - 330, y + hs), (x - 150, y), (x - 330, y - hs)], z, z + 14, glow=0.55)
    w.plan(cols[1], [(x + 300, y), (x - 250, y + hs * 0.55), (x - 160, y), (x - 250, y - hs * 0.55)], z - 6, z + 20, glow=0.7)
    w.box(STEEL, (x + 100, y, z + 10), (760, 10, 10))
    for sy in (-1, 1):
        tilt_box(w, STEEL, (x + 120, y, z), (x + 60, y + sy * 110, z - 190), 8, 8)
    w.box(STEEL, (x + 60, y, z - 190), (8, 230, 8))
    w.box(suit, (x - 40, y, z - 150), (190, 56, 46), glow=0.4)                    # the pilot, prone
    w.box(NAVY, (x - 220, y, z - 146), (190, 44, 34), glow=0.15)
    w.sphere(WHITE, (x + 90, y, z - 140), 34, steps=(4, 6), glow=0.4)
    a = sc.part(P.motion(travel=tv, base=(x, y), bob=-170.0, rate=0.16 * 8, phase=phase, swing=38.0, pivot=(x + 40, y - 30, z - 150), axis=0.0))
    tilt_box(a, suit, (x + 40, y - 30, z - 150), (x + 70, y - 130, z - 210), 16, 16)


def gliders(seed=5, L=30000.0, speed=1150.0):
    """A flight of four hang-gliders over the avenue, wings 12 m across in hot colours: the stunt unit's flying display.
    They fly the mesh's X (along the avenue), 300 m of it, and come round again."""
    def build(sc):
        r = random.Random(seed)
        tv = (L, speed / L)
        pairs = ((PINK, WHITE), (CYAN, YEL), (ORANGE, WHITE), (MINT, VIOLET))
        for i, (x, y, z) in enumerate(((0, -200, 300), (-1700, 420, 620), (-3100, -560, 60), (-4900, 240, 820))):
            glider(sc, x, y, z, pairs[(i + seed) % 4], r.random(), tv, suit=(YEL, PINK, CYAN, WHITE)[i])
    return build


def geese(seed=6, L=26000.0, speed=1500.0):
    """A skein of geese crossing the sky in a V, each 5 m across, pale in the city's glow. It flies the mesh's X (along
    the avenue), 260 m of it, and comes round again."""
    def build(sc):
        r = random.Random(seed)
        n = 11
        for i in range(n):
            k = (i + 1) // 2
            side = -1 if i % 2 else 1
            x = 1400.0 - k * 520.0 + r.uniform(-60, 60)
            y = side * k * 300.0 + r.uniform(-40, 40)
            P.bird(sc, x, y, 300 + r.uniform(-90, 90) + k * 30, heading=0.0, span=r.uniform(470, 540), body='#f4f1ea', wing='#c9c4d6', beak=ORANGE,
                   phase=(0.13 * k) % 1.0, rate=1.25, flap=30.0, travel=(L, speed))
        lit(sc, 0.3)
    return build


JOBS = [
    ('SM_FarTowerParty', tower_party(1), dict(far=True)),
    ('SM_FarConcert', concert(2), dict(far=True)),
    ('SM_FarTowerLoft', tower_loft(3), dict(far=True, grow=(900.0, 900.0, 700.0), grow_down=(900.0, 900.0, 0.0))),
    ('SM_FarSkyRun_A', sky_run(4), dict(far=True)),
    ('SM_FarGliders', gliders(5), dict(far=True, grow=(15500.0, 300.0, 400.0), grow_down=(15500.0, 300.0, 400.0))),
    ('SM_FarGeese', geese(6), dict(far=True, grow=(13500.0, 200.0, 300.0), grow_down=(13500.0, 200.0, 300.0))),
]


def run(only=None):
    P.build_core()
    return P.export(DIR, JOBS, only)


def paths(only=None):
    return [DIR + '/' + j[0] for j in JOBS if not only or j[0] in only]
