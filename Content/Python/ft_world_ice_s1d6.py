"""Ice Mountain, Stage 1, builder s1d6: district 6 "The resort" (14.3 to 17.0 km). /Game/World/IceMountain/SM_S1D6_*.
Run: import ft_world_ice_s1d6 as K; K.run()

THE GRAND SPA. A belle-epoque spa resort on the mountain: palace hotels of five and six storeys under blue mansard
roofs, corner turrets with pointed caps, long iron balconies, striped awnings, colonnades, symmetrical fronts. Every
roof and awning blue, every wall cream over a timber base, warm windows (steady), dark glass, pink only in the flower
boxes. Nothing gabled like the valley's chalets, nothing on stilts like the summit's pods.

Eight buildings (the palace hotel, the spa hotel, the turret villa, the pump room, the bandstand hall, the colonnade
walk, the funicular's lower station, the kurhaus), eight props (bench pair, luggage cart, flower balustrade: LOW,
1.25 m and under; frozen fountain, changing cabins, kiosk, globe lamp, towel line: tall) and the hero, THE PALACE OF
BATHS (a blue-domed rotunda 40 m high between two turrets, colonnades, a steaming open-air pool on a terrace in front).

What the hill asks (FTDressL5.inl): the road falls 7 in 100 and the land with it, so every building has 3 m of base
under z = 0 and the Palace's terrace 7 m. Low props stay 1.25 m and under and are short along the road.
No figures (the people are s1people's), no vehicle but a hand cart of trunks and the funicular's timber car (nothing
blue with white), nothing orange, nothing white and round in the air (the steam is leaning plumes, not balls), no words.
What moves (all in the vertex shader): pennants on the turrets, weather vanes, towels on the line, the kiosk's
pennant, the pump room's vent steam, the Palace's steam and pennants. Light: steady warm windows and lamp globes only,
on tall pieces (every one is placed 30 m or more out); the three low props carry no light and do not move.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/IceMountain'
BLUE, BLUE_D, CREAM, WOOD, WOOD_D = '#2e6bff', '#4f8fd2', '#f4efe6', '#9a6a44', '#6a4c36'
WARM, GLASS, PINK, IRON, STEEL = '#ffe28a', '#1d2740', '#ff4fb8', '#3b3f52', '#9aa4b8'
ICE_D, MIST, PINE, STONE = '#7fb0e0', '#cdd6f1', '#2f6b52', '#aab3c4'
STILL = dict(grow=(0, 0, 0), grow_down=(0, 0, 0))     # no moving part: true bounds
LIT = 0.55                                             # a warm window's steady glow
FOOT = -300.0                                          # how far a building's base goes into the ground


# ------------------------------------------------------------------------------------------ helpers
def pol(r, a):
    """(x, y) at radius r, angle a in degrees (0 = +X, -90 = the front)."""
    return (r * math.cos(math.radians(a)), r * math.sin(math.radians(a)))


def frustum(g, col, cx, cy, z, w, d, h, k, glow=0.0):
    """A hipped or mansard roof on a w x d rectangle about (cx, cy), h tall, its top k times its base.
    (Rewritten AFTER the one generate run of 16 Nov 2026 and not run since: the first version stretched a square cone
    through the model's own transform, and Unreal scales before it turns, so on a plan that is not square the roof
    came out a lozenge. The meshes on disk still have that roof until this file is run again.)
    Near enough square: a true square cone. A pointed roof on a long plan: a ridge roof along the long side.
    Otherwise a mansard: four leaning slabs round a core, corners cut."""
    if max(w, d) <= 1.25 * min(w, d):
        r = math.sqrt(w * d) * 0.70711
        g.cyl(col, (cx, cy, z), r, h, rot=(0, 45, 0), steps=4, top_r=max(r * k, 0.5), glow=glow)
    elif k < 0.4:
        if d >= w:
            g.side(col, [(-w / 2.0, 0.0), (w / 2.0, 0.0), (0.0, h)], -d / 2.0, d / 2.0, glow=glow, loc=(cx, cy, z))
        else:
            for sgn in (-1, 1):
                tilt_box(g, col, (cx, cy + sgn * d / 2.0, z), (cx, cy, z + h), w, 30.0, glow=glow)
            g.box(col, (cx, cy, z + h * 0.3), (w - 20.0, d * 0.6, h * 0.6), glow=glow)
    else:
        g.box(col, (cx, cy, z + h / 2.0), (w * k, d * k, h), glow=glow)
        for sgn in (-1, 1):
            tilt_box(g, col, (cx, cy + sgn * d / 2.0, z), (cx, cy + sgn * d * k / 2.0, z + h), w * k, 40.0, glow=glow)
            tilt_box(g, col, (cx + sgn * w / 2.0, cy, z), (cx + sgn * w * k / 2.0, cy, z + h), d * k, 40.0, glow=glow)


def onface(g, col, face, w, d, a, z, bw, bh, t=8.0, out=0.0, glow=0.0, c=(0.0, 0.0)):
    """A flat panel bw x bh on a wall of a w x d block about c: face 'f' (front, -Y), 'b', 'l' (-X), 'r' (+X);
    a = where along the wall, out = how far it stands off it."""
    if face == 'f':
        g.box(col, (c[0] + a, c[1] - d / 2.0 - out, z), (bw, t, bh), glow=glow)
    elif face == 'b':
        g.box(col, (c[0] + a, c[1] + d / 2.0 + out, z), (bw, t, bh), glow=glow)
    elif face == 'r':
        g.box(col, (c[0] + w / 2.0 + out, c[1] + a, z), (t, bw, bh), glow=glow)
    else:
        g.box(col, (c[0] - w / 2.0 - out, c[1] + a, z), (t, bw, bh), glow=glow)


def window(g, face, w, d, a, z, bw=110.0, bh=170.0, lit=False, c=(0.0, 0.0)):
    """A window: dark glass, and a warm pane in it when lit."""
    onface(g, GLASS, face, w, d, a, z, bw, bh, c=c)
    if lit:
        onface(g, WARM, face, w, d, a, z, bw - 26.0, bh - 30.0, out=3.0, glow=LIT, c=c)


def awning(g, face, w, d, a, z, bw=150.0, reach=80.0, drop=45.0, col=BLUE, c=(0.0, 0.0)):
    """A striped cloth awning over a window: a slab from the wall down and out, a stripe of the other colour on it."""
    other = CREAM if col == BLUE else BLUE
    for cl, ww, up in ((col, bw, 0.0), (other, bw * 0.24, 4.0)):
        if face == 'f':
            tilt_box(g, cl, (c[0] + a, c[1] - d / 2.0, z + up), (c[0] + a, c[1] - d / 2.0 - reach - up, z - drop + up), ww, 8.0)
        elif face == 'r':
            tilt_box(g, cl, (c[0] + w / 2.0, c[1] + a, z + up), (c[0] + w / 2.0 + reach + up, c[1] + a, z - drop + up), ww, 8.0)
        else:
            tilt_box(g, cl, (c[0] - w / 2.0, c[1] + a, z + up), (c[0] - w / 2.0 - reach - up, c[1] + a, z - drop + up), ww, 8.0)


def cresting(g, x, y, z, length, n=4):
    """Iron cresting along a roof's flat: a rail on posts, a spike at each."""
    g.box(IRON, (x, y, z + 30.0), (length, 5.0, 6.0))
    for i in range(n):
        g.box(IRON, (x - length / 2.0 + length * i / (n - 1.0), y, z + 24.0), (6.0, 6.0, 60.0))


def lantern(g, x, y, z):
    """A wall lantern on an iron bracket (steady)."""
    g.box(IRON, (x, y - 22.0, z + 30.0), (6.0, 50.0, 6.0))
    g.box(IRON, (x, y - 44.0, z + 18.0), (26.0, 26.0, 6.0))
    g.box(WARM, (x, y - 44.0, z - 4.0), (20.0, 20.0, 38.0), glow=0.8)


def urn(g, x, y, z, s=1.0):
    """A cream urn of pink flowers on a pier."""
    g.cyl(CREAM, (x, y, z), 14.0 * s, 34.0 * s, steps=6, top_r=26.0 * s)
    g.box(PINE, (x, y, z + 40.0 * s), (40.0 * s, 40.0 * s, 14.0 * s))
    g.box(PINK, (x, y, z + 54.0 * s), (30.0 * s, 30.0 * s, 16.0 * s))


def balcony(g, face, w, d, z, length, a=0.0, c=(0.0, 0.0), deep=46.0):
    """A long iron balcony: a cream slab and a dark rail."""
    onface(g, CREAM, face, w, d, a, z, length, 10.0, t=deep, out=deep / 2.0, c=c)
    onface(g, IRON, face, w, d, a, z + 34.0, length, 58.0, t=5.0, out=deep - 3.0, c=c)


def flowers(g, face, w, d, a, z, bw=110.0, c=(0.0, 0.0)):
    """A pink flower box under a window."""
    onface(g, PINK, face, w, d, a, z, bw, 22.0, t=26.0, out=13.0, c=c)


def turret(g, x, y, r, z0, z1, cap, wins=()):
    """A round corner turret (eight-sided, a flat to the front): cream on a timber foot, an iron ring under a
    pointed blue cap, a window at each height in wins. Returns the top of its mast."""
    rt = (0, 22.5, 0)
    g.cyl(CREAM, (x, y, z0), r, z1 - z0, rot=rt, steps=8)
    g.cyl(WOOD, (x, y, z0), r + 8.0, max(330.0 - z0, 60.0), rot=rt, steps=8)
    g.cyl(IRON, (x, y, z1 - 26.0), r + 22.0, 26.0, rot=rt, steps=8)
    g.cyl(BLUE, (x, y, z1), r + 36.0, cap, rot=rt, steps=8, top_r=5.0)
    for k, z in enumerate(wins):
        g.box(GLASS, (x, y - r * 0.924 - 1.0, z), (70.0, 10.0, 150.0))
        if k % 2 == 0:
            g.box(WARM, (x, y - r * 0.924 - 4.0, z), (48.0, 8.0, 122.0), glow=LIT)
    g.cyl(STEEL, (x, y, z1 + cap - 10.0), 5.0, 150.0, steps=6)
    return z1 + cap + 140.0


def pennant(sc, x, y, z, col=BLUE, length=150.0, h=56.0, phase=0.0, rate=0.35, swing=16.0):
    """A pennant turning on its mast."""
    f = sc.part(P.motion(swing=swing, pivot=(x, y, z), axis=P.UP, rate=rate, phase=phase))
    f.box(col, (x + length / 2.0 + 6.0, y, z - h / 2.0), (length, 5.0, h))
    f.box(CREAM, (x + length * 0.3, y, z - h / 2.0), (length * 0.16, 6.0, h))


def vane(sc, x, y, z, phase=0.0):
    """A weather vane: an arrow turning a little on its spindle (no words, no figure)."""
    f = sc.part(P.motion(swing=24.0, pivot=(x, y, z), axis=P.UP, rate=0.22, phase=phase))
    f.box(IRON, (x, y, z + 40.0), (170.0, 6.0, 8.0))
    f.box(IRON, (x - 74.0, y, z + 40.0), (36.0, 6.0, 44.0))
    f.box(WARM, (x + 84.0, y, z + 40.0), (30.0, 8.0, 30.0), rot=(45, 0, 0))


def plume(sc, x, y, z, r, h, phase, lean=10.0, rate=0.16, n=3):
    """Steam: a leaning, flaring column in pieces that waver out of step (never a ball)."""
    for k in range(n):
        f = sc.part(P.motion(swing=7.0 + 3.0 * k, pivot=(x, y, z), axis=90.0, rate=rate + 0.03 * k, phase=phase + 0.23 * k, bob=-(10.0 + 8.0 * k)))
        zz = z + h * k / n
        f.cyl(MIST, (x + lean * 4.0 * k, y, zz), r * (0.55 + 0.38 * k), h / n * 0.94, rot=(-lean, 0, 0), steps=6, top_r=r * (0.9 + 0.42 * k))


def storeys(g, w, d, z0, n, fh, cols, sides, seed, c=(0.0, 0.0), bw=110.0, bh=170.0, box_floor=0, balc=()):
    """n floors of windows from z0: cols across the front, sides across each end wall; some lit; a balcony on the
    front of each floor in balc; flower boxes under the windows of floor box_floor."""
    r = random.Random(seed)
    for f in range(n):
        z = z0 + f * fh + fh * 0.55
        for face, k in (('f', cols), ('l', sides), ('r', sides)):
            span = (w if face == 'f' else d) - 150.0
            for j in range(k):
                a = (-span / 2.0 + span * (j + 0.5) / k)
                window(g, face, w, d, a, z, bw, bh, r.random() < 0.42, c)
                if f == box_floor and face == 'f':
                    flowers(g, face, w, d, a, z - bh / 2.0 - 14.0, bw, c)
        if f in balc:
            balcony(g, 'f', w, d, z - bh / 2.0 - 30.0, w - 70.0, c=c)


# ------------------------------------------------------------------------------------------ buildings
def palace_hotel(sc):
    """THE PALACE HOTEL: 14 x 10 m, six storeys, a blue mansard with dormers, two corner turrets with pennants,
    a long iron balcony to every floor, striped awnings over the timber ground floor."""
    g = sc.still()
    w, d, fh = 1400.0, 1000.0, 320.0
    top = 380.0 + 5 * fh
    g.box(WOOD, (0, 0, (FOOT + 380.0) / 2.0), (w + 30.0, d + 30.0, 380.0 - FOOT))
    g.box(CREAM, (0, 0, (380.0 + top) / 2.0), (w, d, top - 380.0))
    g.box(WOOD_D, (0, 0, 392.0), (w + 60.0, d + 60.0, 24.0))
    g.box(CREAM, (0, 0, top + 20.0), (w + 90.0, d + 90.0, 40.0))
    frustum(g, BLUE, 0, 0, top + 40.0, w + 40.0, d + 40.0, 330.0, 0.84)
    g.box(BLUE_D, (0, 0, top + 380.0), (w * 0.84, d * 0.8, 20.0))
    cresting(g, 0, -d * 0.4, top + 390.0, w * 0.8, 5)
    for s in (-1, 1):                                      # timber quoins up the back corners, lanterns and urns by the porch
        g.box(WOOD, (s * (w / 2.0 - 10.0), d / 2.0 - 10.0, (380.0 + top) / 2.0), (44.0, 44.0, top - 380.0))
        lantern(g, s * 250.0, -d / 2.0, 300.0)
        urn(g, s * 160.0, -d / 2.0 - 300.0, 0.0, 1.4)
    g.box(WOOD_D, (0, -d / 2.0 - 300.0, -130.0), (460.0, 90.0, 260.0))        # the porch's step
    for face in ('l', 'r'):
        for a in (-283.0, 0.0, 283.0):
            flowers(g, face, w, d, a, 380.0 + fh * 0.55 - 99.0, 110.0)
        balcony(g, face, w, d, 380.0 + 2 * fh + fh * 0.55 - 115.0, d - 320.0, a=90.0)
        balcony(g, face, w, d, 380.0 + 4 * fh + fh * 0.55 - 115.0, d - 320.0, a=90.0)
    storeys(g, w, d, 380.0, 5, fh, 5, 3, 61, box_floor=0, balc=(0, 1, 2, 3, 4))
    for j in range(5):                                     # the ground floor: tall dark doors under striped awnings
        a = -520.0 + j * 260.0
        onface(g, GLASS, 'f', w, d, a, 190.0, 150.0, 300.0)
        if j != 2:
            onface(g, WARM, 'f', w, d, a, 200.0, 110.0, 230.0, out=3.0, glow=LIT)
        awning(g, 'f', w, d, a, 372.0, 190.0, 110.0, 60.0, BLUE if j % 2 == 0 else CREAM)
    for face in ('l', 'r'):
        for a in (-250.0, 250.0):
            onface(g, GLASS, face, w, d, a, 190.0, 170.0, 280.0)
            awning(g, face, w, d, a, 372.0, 210.0, 90.0, 50.0)
    g.box(BLUE, (0, -d / 2.0 - 120.0, 420.0), (360.0, 240.0, 16.0))          # the porch
    for s in (-1, 1):
        g.cyl(CREAM, (s * 160.0, -d / 2.0 - 220.0, FOOT), 14.0, 420.0 - FOOT, steps=6)
    for j in range(3):                                     # dormers in the mansard
        a = -420.0 + j * 420.0
        g.box(CREAM, (a, -d / 2.0 + 20.0, top + 190.0), (150.0, 130.0, 170.0))
        g.box(WARM, (a, -d / 2.0 - 47.0, top + 185.0), (96.0, 8.0, 110.0), glow=LIT)
        frustum(g, BLUE, a, -d / 2.0 + 20.0, top + 275.0, 180.0, 160.0, 80.0, 0.1)
    for s in (-1, 1):
        g.box(WOOD_D, (s * 460.0, 180.0, top + 440.0), (90.0, 90.0, 150.0))  # chimneys
        g.box(CREAM, (s * 460.0, 180.0, top + 522.0), (110.0, 110.0, 16.0))
    for k, s in enumerate((-1, 1)):
        tip = turret(g, s * (w / 2.0 - 20.0), -d / 2.0 + 20.0, 150.0, FOOT, top + 250.0, 430.0, (top + 90.0, top - 500.0, top - 1140.0))
        pennant(sc, s * (w / 2.0 - 20.0), -d / 2.0 + 20.0, tip, phase=0.2 + 0.4 * k)


def spa_hotel(sc):
    """THE SPA HOTEL: a narrow hotel of six storeys, 8.6 x 7.4 m: a bay up the middle of the front under a small
    blue dome, balconies either side of it, a blue mansard."""
    g = sc.still()
    w, d, fh = 860.0, 740.0, 290.0
    top = 340.0 + 5 * fh
    g.box(WOOD, (0, 0, (FOOT + 340.0) / 2.0), (w + 24.0, d + 24.0, 340.0 - FOOT))
    g.box(CREAM, (0, 0, (340.0 + top) / 2.0), (w, d, top - 340.0))
    g.box(CREAM, (0, 0, top + 16.0), (w + 70.0, d + 70.0, 32.0))
    frustum(g, BLUE, 0, 0, top + 32.0, w + 30.0, d + 30.0, 290.0, 0.8)
    g.box(BLUE_D, (0, 0, top + 330.0), (w * 0.8, d * 0.8, 16.0))
    g.box(CREAM, (0, -d / 2.0 - 40.0, (FOOT + top + 150.0) / 2.0), (260.0, 110.0, top + 150.0 - FOOT))   # the bay
    g.sphere(BLUE, (0, -d / 2.0 - 40.0, top + 150.0), 150.0, scale=(1.0, 0.62, 1.1), steps=(4, 8))
    g.cyl(STEEL, (0, -d / 2.0 - 40.0, top + 290.0), 5.0, 120.0, steps=6)
    r = random.Random(62)
    for f in range(5):
        z = 340.0 + f * fh + fh * 0.55
        window(g, 'f', w, d + 190.0, 0.0, z, 120.0, 170.0, f % 2 == 0)          # on the bay
        for s in (-1, 1):
            window(g, 'f', w, d, s * 290.0, z, 110.0, 165.0, r.random() < 0.4)
            onface(g, IRON, 'f', w, d, s * 290.0, z - 78.0, 190.0, 54.0, t=5.0, out=36.0)
            onface(g, CREAM, 'f', w, d, s * 290.0, z - 108.0, 190.0, 10.0, t=40.0, out=20.0)
            if f == 0:
                flowers(g, 'f', w, d, s * 290.0, z - 60.0, 150.0)
        for face in ('l', 'r'):
            for a in (-170.0, 170.0):
                window(g, face, w, d, a, z, 110.0, 165.0, r.random() < 0.4)
    onface(g, GLASS, 'f', w, d + 190.0, 0.0, 170.0, 150.0, 290.0)
    awning(g, 'f', w, d + 190.0, 0.0, 345.0, 250.0, 100.0, 55.0)
    for s in (-1, 1):
        onface(g, GLASS, 'f', w, d, s * 290.0, 180.0, 170.0, 220.0)
        onface(g, WARM, 'f', w, d, s * 290.0, 180.0, 130.0, 170.0, out=3.0, glow=LIT)
        awning(g, 'f', w, d, s * 290.0, 320.0, 210.0, 80.0, 45.0, CREAM)
    for face in ('l', 'r'):
        onface(g, GLASS, face, w, d, 0.0, 180.0, 300.0, 220.0)
        awning(g, face, w, d, 0.0, 320.0, 340.0, 80.0, 45.0)
        g_dormer = (w / 2.0 - 30.0) * (1 if face == 'r' else -1)
        g.box(CREAM, (g_dormer, 0, top + 170.0), (110.0, 130.0, 150.0))
        onface(g, WARM, face, w, d, 0.0, top + 170.0, 90.0, 100.0, out=28.0, glow=LIT)
    g.box(WOOD_D, (-250.0, 200.0, top + 380.0), (80.0, 80.0, 130.0))
    g.box(CREAM, (-250.0, 200.0, top + 452.0), (100.0, 100.0, 14.0))
    for s in (-1, 1):
        lantern(g, s * 150.0, -d / 2.0, 290.0)
        g.box(CREAM, (s * 290.0, -d / 2.0 + 20.0, top + 170.0), (120.0, 120.0, 150.0))     # front dormers
        g.box(WARM, (s * 290.0, -d / 2.0 - 42.0, top + 166.0), (80.0, 8.0, 96.0), glow=LIT)
        frustum(g, BLUE, s * 290.0, -d / 2.0 + 20.0, top + 245.0, 150.0, 150.0, 70.0, 0.1)
    pennant(sc, 0, -d / 2.0 - 40.0, top + 405.0, phase=0.6)


def turret_villa(sc):
    """THE TURRET VILLA: four storeys, 8.6 x 7.4 m, a steep blue hipped roof, a round turret on one front corner
    with a tall pointed cap and a weather vane, a glazed bay with a balcony on it."""
    g = sc.still()
    w, d, fh = 860.0, 740.0, 300.0
    top = 330.0 + 3 * fh
    g.box(WOOD, (0, 0, (FOOT + 330.0) / 2.0), (w + 24.0, d + 24.0, 330.0 - FOOT))
    g.box(CREAM, (0, 0, (330.0 + top) / 2.0), (w, d, top - 330.0))
    g.box(WOOD_D, (0, 0, 342.0), (w + 44.0, d + 44.0, 20.0))
    g.box(CREAM, (0, 0, top + 14.0), (w + 80.0, d + 80.0, 28.0))
    frustum(g, BLUE, 0, 0, top + 28.0, w + 60.0, d + 60.0, 460.0, 0.2)
    g.box(BLUE_D, (0, 0, top + 492.0), (w * 0.22, d * 0.22, 14.0))
    g.box(WOOD_D, (250.0, 150.0, top + 330.0), (80.0, 80.0, 260.0))
    g.box(CREAM, (250.0, 150.0, top + 466.0), (100.0, 100.0, 14.0))
    storeys(g, w, d, 330.0, 3, fh, 3, 2, 63, box_floor=0, balc=(1,))
    onface(g, GLASS, 'f', w, d, 60.0, 180.0, 150.0, 290.0)
    awning(g, 'f', w, d, 60.0, 340.0, 230.0, 95.0, 55.0)
    onface(g, GLASS, 'f', w, d, 290.0, 200.0, 150.0, 190.0)
    onface(g, WARM, 'f', w, d, 290.0, 200.0, 112.0, 150.0, out=3.0, glow=LIT)
    for face in ('l', 'r'):
        onface(g, GLASS, face, w, d, 80.0, 190.0, 260.0, 210.0)
        awning(g, face, w, d, 80.0, 322.0, 300.0, 80.0, 45.0, CREAM)
    g.box(CREAM, (0, -d / 2.0 + 30.0, top + 200.0), (170.0, 150.0, 190.0))    # the roof's dormer
    g.box(WARM, (0, -d / 2.0 - 47.0, top + 195.0), (110.0, 8.0, 120.0), glow=LIT)
    frustum(g, BLUE, 0, -d / 2.0 + 30.0, top + 295.0, 200.0, 180.0, 110.0, 0.08)
    tx, ty = -w / 2.0 + 10.0, -d / 2.0 + 10.0
    tip = turret(g, tx, ty, 175.0, FOOT, top + 290.0, 560.0, (top + 120.0, top - 135.0, top - 435.0, top - 735.0))
    vane(sc, tx, ty, tip - 40.0, 0.3)
    cresting(g, 0, 0, top + 498.0, w * 0.2, 3)
    hx, hy = w / 2.0 - 6.0, -d / 2.0 + 6.0                 # a hanging turret on the other front corner
    g.cyl(CREAM, (hx, hy, 330.0 + fh + 40.0), 90.0, 2 * fh, rot=(0, 22.5, 0), steps=8)
    g.cyl(WOOD, (hx, hy, 330.0 + fh - 90.0), 20.0, 130.0, rot=(0, 22.5, 0), steps=8, top_r=92.0)
    g.cyl(BLUE, (hx, hy, 330.0 + 3 * fh + 40.0), 110.0, 300.0, rot=(0, 22.5, 0), steps=8, top_r=4.0)
    for f in (1, 2):
        g.box(GLASS, (hx, hy - 84.0, 330.0 + f * fh + 180.0), (50.0, 8.0, 120.0))
    g.box(WARM, (hx, hy - 87.0, 330.0 + fh + 180.0), (32.0, 8.0, 96.0), glow=LIT)
    lantern(g, -70.0, -d / 2.0, 280.0)
    for face in ('l', 'r'):
        for a in (-147.5, 147.5):
            flowers(g, face, w, d, a, 330.0 + fh * 0.55 - 99.0, 110.0)
        onface(g, WOOD, face, w, d, d / 2.0 - 14.0, (330.0 + top) / 2.0, 28.0, top - 330.0, t=10.0)   # timber quoins
    g.box(WOOD_D, (60.0, -d / 2.0 - 60.0, -110.0), (260.0, 90.0, 240.0))      # the door's steps
    for s in (-1, 1):
        urn(g, 60.0 + s * 150.0, -d / 2.0 - 60.0, 10.0, 1.2)


def pump_room(sc):
    """THE PUMP ROOM: a glazed pavilion 9 x 7 m where the waters are taken: slim cream piers and dark glass, a blue
    hipped roof, a lit glass lantern under a blue cap on top, a vent that steams."""
    g = sc.still()
    w, d = 900.0, 700.0
    g.box(WOOD, (0, 0, (FOOT + 70.0) / 2.0), (w + 80.0, d + 80.0, 70.0 - FOOT))
    g.box(WOOD_D, (0, -d / 2.0 - 70.0, 20.0), (300.0, 70.0, 40.0))             # steps
    g.box(GLASS, (0, 0, 290.0), (w - 30.0, d - 30.0, 440.0))
    g.box(WARM, (0, 0, 250.0), (w - 22.0, d - 22.0, 150.0), glow=LIT * 0.8)      # the lit room behind the glass
    for i in range(5):                                     # piers
        x = -w / 2.0 + i * w / 4.0
        for y in (-d / 2.0, d / 2.0):
            g.box(CREAM, (x, y, 290.0), (46.0, 46.0, 440.0))
    for y in (-d / 6.0, d / 6.0):
        for s in (-1, 1):
            g.box(CREAM, (s * w / 2.0, y, 290.0), (46.0, 46.0, 440.0))
    g.box(CREAM, (0, 0, 420.0), (w + 6.0, d + 6.0, 16.0))                      # the transom
    g.box(CREAM, (0, 0, 535.0), (w + 80.0, d + 80.0, 50.0))
    for i in range(4):                                     # a scalloped valance, blue and cream by turns
        x = -w / 2.0 + (i + 0.5) * w / 4.0
        g.box(BLUE if i % 2 == 0 else CREAM, (x, -d / 2.0 - 44.0, 492.0), (w / 4.0 - 20.0, 6.0, 46.0))
    frustum(g, BLUE, 0, 0, 560.0, w + 120.0, d + 120.0, 210.0, 0.46)
    g.box(CREAM, (0, 0, 815.0), (380.0, 290.0, 130.0))                         # the lantern
    g.box(WARM, (0, 0, 815.0), (390.0, 300.0, 86.0), glow=LIT)
    for s in (-1, 1):
        g.box(CREAM, (s * 190.0, 0, 815.0), (30.0, 306.0, 130.0))
    g.box(CREAM, (0, 0, 815.0), (30.0, 306.0, 130.0))
    frustum(g, BLUE, 0, 0, 880.0, 470.0, 380.0, 170.0, 0.06)
    g.sphere(WARM, (0, 0, 1068.0), 22.0, steps=(3, 6))
    g.cyl(STEEL, (0, 0, 1040.0), 5.0, 30.0, steps=6)
    for s in (-1, 1):                                      # flower tubs by the steps
        g.box(WOOD_D, (s * 230.0, -d / 2.0 - 70.0, 60.0), (80.0, 80.0, 60.0))
        g.box(PINK, (s * 230.0, -d / 2.0 - 70.0, 100.0), (70.0, 70.0, 26.0))
    for i in range(4):                                     # fanlights over the front's bays, a lantern on every pier
        x = -w / 2.0 + (i + 0.5) * w / 4.0
        g.cyl(WARM, (x, -d / 2.0 - 8.0, 430.0), 70.0, 8.0, rot=(-90, -90, 0), steps=8, glow=LIT)
    for i in range(5):
        lantern(g, -w / 2.0 + i * w / 4.0, -d / 2.0 - 23.0, 330.0)
    for sx in (-1, 1):
        for sy in (-1, 1):
            urn(g, sx * (w / 2.0 + 20.0), sy * (d / 2.0 + 20.0), 560.0, 1.3)
    cresting(g, 0, -150.0, 880.0, 380.0, 4)
    g.cyl(STONE, (-250.0, -d / 2.0 - 90.0, 0.0), 40.0, 90.0, steps=8, top_r=60.0)   # the drinking fountain, its spout frozen
    g.cyl(ICE_D, (-250.0, -d / 2.0 - 90.0, 90.0), 52.0, 8.0, steps=8)
    g.cyl(BLUE_D, (-250.0, -d / 2.0 - 90.0, 98.0), 8.0, 50.0, steps=5, top_r=3.0)
    g.cyl(STEEL, (300.0, 200.0, 640.0), 22.0, 150.0, steps=6)                   # the vent
    g.cyl(IRON, (300.0, 200.0, 790.0), 32.0, 14.0, steps=6)
    plume(sc, 300.0, 200.0, 810.0, 34.0, 330.0, 0.1, n=3)


def bandstand_hall(sc):
    """THE BANDSTAND HALL: an octagon 8.4 m across, open between eight cream columns, two blue caps one above the
    other (the upper one on a lit drum), a weather vane, music stands and chairs on the floor (nobody on them)."""
    g = sc.still()
    g.cyl(WOOD, (0, 0, FOOT), 420.0, 110.0 - FOOT, rot=(0, 22.5, 0), steps=8)
    g.cyl(WOOD_D, (0, 0, 100.0), 440.0, 16.0, rot=(0, 22.5, 0), steps=8)
    g.box(WOOD_D, (0, -440.0, 30.0), (240.0, 110.0, 60.0))
    for k in range(8):
        x, y = pol(385.0, 22.5 + k * 45.0)
        g.cyl(CREAM, (x, y, 110.0), 17.0, 430.0, steps=6)
        g.box(CREAM, (x, y, 548.0), (52.0, 52.0, 18.0))
        if k != 5:                                         # an iron rail between the columns, open at the front
            x2, y2 = pol(385.0, 22.5 + (k + 1) * 45.0)
            tilt_box(g, IRON, (x, y, 190.0), (x2, y2, 190.0), 5.0, 70.0)
    g.cyl(CREAM, (0, 0, 556.0), 430.0, 56.0, rot=(0, 22.5, 0), steps=8)
    for k in range(8):                                     # the valance: blue and cream panels by turns
        x, y = pol(400.0, k * 45.0)
        g.box(BLUE if k % 2 == 0 else CREAM, (x, y, 528.0), (6.0, 250.0, 44.0), rot=(0, k * 45.0, 0))
    g.cyl(BLUE, (0, 0, 612.0), 500.0, 250.0, rot=(0, 22.5, 0), steps=8, top_r=170.0)
    g.cyl(CREAM, (0, 0, 862.0), 165.0, 110.0, rot=(0, 22.5, 0), steps=8)
    g.cyl(WARM, (0, 0, 884.0), 170.0, 62.0, rot=(0, 22.5, 0), steps=8, glow=LIT)
    g.cyl(BLUE, (0, 0, 972.0), 230.0, 170.0, rot=(0, 22.5, 0), steps=8, top_r=5.0)
    g.cyl(STEEL, (0, 0, 1130.0), 5.0, 110.0, steps=6)
    for k in range(5):                                     # music stands and chairs
        x, y = pol(210.0, 200.0 + k * 35.0)
        g.box(IRON, (x, y, 170.0), (6.0, 6.0, 120.0))
        g.box(CREAM, (x, y - 4.0, 236.0), (44.0, 6.0, 32.0))
        x, y = pol(290.0, 200.0 + k * 35.0)
        g.box(WOOD_D, (x, y, 140.0), (40.0, 40.0, 60.0))
    for s in (-1, 1):
        g.box(WOOD_D, (s * 190.0, -450.0, 70.0), (70.0, 70.0, 60.0))
        g.box(PINK, (s * 190.0, -450.0, 110.0), (60.0, 60.0, 24.0))
    vane(sc, 0, 0, 1200.0, 0.7)
    for k in range(8):                                     # lanterns and flower baskets hung by turns under the eaves
        x, y = pol(350.0, k * 45.0)
        g.box(IRON, (x, y, 520.0), (5.0, 5.0, 70.0))
        if k % 2 == 0:
            g.box(WARM, (x, y, 468.0), (24.0, 24.0, 36.0), glow=0.8)
        else:
            g.cyl(WOOD_D, (x, y, 452.0), 12.0, 26.0, steps=6, top_r=24.0)
            g.box(PINK, (x, y, 486.0), (40.0, 40.0, 16.0))
    g.cyl(CREAM, (0, 0, 116.0), 60.0, 40.0, steps=8)                             # the conductor's rostrum
    g.box(IRON, (0, -40.0, 200.0), (6.0, 6.0, 130.0))
    g.box(CREAM, (0, -44.0, 270.0), (50.0, 6.0, 36.0))


def colonnade_walk(sc):
    """THE COLONNADE WALK: 12 m of open arcade for the promenade: seven cream columns along the front, a back
    wall with lit windows, a blue lean-to roof with a striped valance, a little pointed pavilion at each end."""
    g = sc.still()
    w, d = 1200.0, 440.0
    g.box(WOOD, (0, 0, (FOOT + 50.0) / 2.0), (w + 40.0, d + 60.0, 50.0 - FOOT))
    g.box(CREAM, (0, d / 2.0 - 20.0, 270.0), (w - 300.0, 40.0, 440.0))          # the back wall
    g.box(WOOD, (0, d / 2.0 - 24.0, 110.0), (w - 300.0, 44.0, 120.0))
    for i in range(7):
        x = -390.0 + i * 130.0
        g.cyl(CREAM, (x, -d / 2.0 + 30.0, 50.0), 16.0, 400.0, steps=6)
        g.box(CREAM, (x, -d / 2.0 + 30.0, 458.0), (48.0, 48.0, 16.0))
        if i < 6:
            g.box(BLUE if i % 2 == 0 else CREAM, (x + 65.0, -d / 2.0 - 4.0, 432.0), (116.0, 6.0, 40.0))
            if i % 2 == 0:
                g.box(GLASS, (x + 65.0, d / 2.0 - 42.0, 300.0), (90.0, 8.0, 200.0))
                g.box(WARM, (x + 65.0, d / 2.0 - 46.0, 300.0), (64.0, 8.0, 170.0), glow=LIT)
            else:
                g.box(PINK, (x + 65.0, -d / 2.0 + 30.0, 82.0), (90.0, 30.0, 24.0))
                g.box(WOOD_D, (x + 65.0, -d / 2.0 + 30.0, 62.0), (100.0, 36.0, 24.0))
    g.box(CREAM, (0, 0, 482.0), (w - 290.0, d + 20.0, 34.0))
    for i in range(3):                                     # benches along the back wall, lamps hung from the roof
        x = -260.0 + i * 260.0
        g.box(WOOD, (x, d / 2.0 - 80.0, 96.0), (170.0, 44.0, 8.0))
        g.box(IRON, (x, d / 2.0 - 80.0, 72.0), (150.0, 30.0, 40.0))
        g.box(IRON, (x, 0, 440.0), (5.0, 5.0, 60.0))
        g.sphere(WARM, (x, 0, 394.0), 20.0, steps=(3, 6), glow=0.8)
    cresting(g, 0, d / 2.0 - 6.0, 590.0, w - 300.0, 6)
    g.box(BLUE, (0, -14.0, 540.0), (w - 270.0, d + 90.0, 14.0), rot=(0, 0, -11))
    g.box(BLUE_D, (0, d / 2.0 - 6.0, 590.0), (w - 270.0, 12.0, 26.0))
    for k, s in enumerate((-1, 1)):                        # the end pavilions
        x = s * (w / 2.0 - 90.0)
        g.box(CREAM, (x, 0, 300.0), (180.0, d, 500.0))
        g.box(WOOD, (x, 0, 110.0), (188.0, d + 8.0, 120.0))
        g.box(CREAM, (x, 0, 560.0), (220.0, d + 40.0, 24.0))
        frustum(g, BLUE, x, 0, 572.0, 250.0, d + 70.0, 260.0, 0.04)
        g.cyl(STEEL, (x, 0, 820.0), 5.0, 90.0, steps=6)
        g.sphere(WARM, (x, 0, 916.0), 16.0, steps=(3, 6))
        window(g, 'f', 180.0, d, 0.0, 320.0, 90.0, 190.0, True, (x, 0.0))
        window(g, 'r' if s > 0 else 'l', 180.0, d, 0.0, 320.0, 150.0, 190.0, k == 0, (x, 0.0))
        awning(g, 'r' if s > 0 else 'l', 180.0, d, 0.0, 440.0, 190.0, 70.0, 40.0, c=(x, 0.0))


def funicular(sc):
    """THE FUNICULAR'S LOWER STATION: a cream station house on a timber base, and behind it the incline going up
    under a blue roof in three steps; the timber car waits at the foot of the rails; a signal mast with a pennant."""
    g = sc.still()
    w, d = 1000.0, 1100.0
    g.box(WOOD, (0, 0, (FOOT + 120.0) / 2.0), (w + 30.0, d + 30.0, 120.0 - FOOT))
    g.box(CREAM, (0, -300.0, 330.0), (w, 500.0, 420.0))                          # the booking hall
    g.box(WOOD, (0, -300.0, 180.0), (w + 10.0, 510.0, 120.0))
    g.box(CREAM, (0, -300.0, 556.0), (w + 80.0, 580.0, 32.0))
    frustum(g, BLUE, 0, -300.0, 572.0, w + 60.0, 560.0, 210.0, 0.8)
    g.box(BLUE_D, (0, -300.0, 790.0), (w * 0.5, 270.0, 16.0))
    for j in range(4):
        a = -360.0 + j * 240.0
        if j in (1, 2):
            onface(g, GLASS, 'f', w, 500.0, a, 310.0, 150.0, 300.0, c=(0.0, -300.0))
        else:
            window(g, 'f', w, 500.0, a, 370.0, 130.0, 190.0, True, (0.0, -300.0))
            flowers(g, 'f', w, 500.0, a, 262.0, 130.0, (0.0, -300.0))
    awning(g, 'f', w, 500.0, 0.0, 500.0, 470.0, 120.0, 60.0, c=(0.0, -300.0))
    for face in ('l', 'r'):
        window(g, face, w, 500.0, 0.0, 370.0, 200.0, 190.0, face == 'l', (0.0, -300.0))
        awning(g, face, w, 500.0, 0.0, 490.0, 240.0, 80.0, 45.0, CREAM, (0.0, -300.0))
    g.box(CREAM, (0, -300.0, 900.0), (150.0, 150.0, 220.0))                      # the clock turret: a plain lit face
    g.cyl(WARM, (0, -378.0, 910.0), 50.0, 8.0, rot=(-90, -90, 0), steps=8, glow=LIT)
    frustum(g, BLUE, 0, -300.0, 1010.0, 210.0, 210.0, 200.0, 0.04)
    for k in range(3):                                     # the incline's roof, three steps up the slope
        y = 80.0 + k * 170.0
        z = 560.0 + k * 150.0
        g.box(BLUE, (0, y, z), (w * 0.62, 190.0, 18.0), rot=(0, 0, -14))
        g.box(CREAM, (0, y - 88.0, z - 44.0), (w * 0.62, 12.0, 60.0))
        for s in (-1, 1):
            g.box(CREAM, (s * w * 0.29, y, (120.0 + z) / 2.0 - 10.0), (30.0, 30.0, z - 120.0))
    for s in (-1, 1):                                      # the rails, on a timber trestle
        tilt_box(g, IRON, (s * 80.0, -40.0, 150.0), (s * 80.0, 540.0, 600.0), 12.0, 12.0)
    tilt_box(g, WOOD_D, (0, -40.0, 120.0), (0, 540.0, 570.0), 240.0, 24.0)
    for k in range(3):
        y = 150.0 + k * 170.0
        g.box(WOOD_D, (0, y, (120.0 + 250.0 + k * 130.0) / 2.0), (220.0, 26.0, 130.0 + k * 130.0))
    cy, cz = 110.0, 300.0                                  # the car: timber, stepped like its track
    for k in range(2):
        g.box(WOOD, (0, cy + k * 130.0, cz + k * 100.0), (230.0, 130.0, 170.0))
        g.box(WARM, (0, cy + k * 130.0, cz + k * 100.0 + 20.0), (238.0, 90.0, 70.0), glow=LIT)
        g.box(WOOD_D, (0, cy + k * 130.0, cz + k * 100.0 + 92.0), (250.0, 150.0, 14.0))
    g.box(PINK, (0, cy - 68.0, cz - 40.0), (236.0, 8.0, 26.0))
    g.cyl(STEEL, (w / 2.0 - 60.0, 420.0, 120.0), 7.0, 880.0, steps=6)
    for s in (-1, 1):
        lantern(g, s * 250.0, -550.0, 430.0)
        urn(g, s * 420.0, -590.0, 120.0, 1.3)
        g.box(WOOD_D, (s * 420.0, -590.0, 60.0), (70.0, 70.0, 120.0))
    g.box(WOOD_D, (0, -610.0, -90.0), (420.0, 120.0, 300.0))                    # the steps
    g.box(WOOD_D, (0, -700.0, -120.0), (420.0, 80.0, 180.0))
    cresting(g, 0, -300.0, 792.0, w * 0.46, 4)
    g.cyl(IRON, (-w * 0.29 - 30.0, 480.0, 420.0), 110.0, 14.0, rot=(0, 0, 90), steps=10)   # the winding wheel on the top frame
    g.cyl(WOOD_D, (-w * 0.29 - 34.0, 480.0, 420.0), 30.0, 22.0, rot=(0, 0, 90), steps=6)
    g.box(WOOD, (-300.0, -640.0, 160.0), (90.0, 50.0, 60.0))                    # trunks waiting on the step
    g.box(PINK, (-300.0, -640.0, 202.0), (60.0, 40.0, 24.0))
    pennant(sc, w / 2.0 - 60.0, 420.0, 990.0, phase=0.5)


def kurhaus(sc):
    """THE KURHAUS: the resort's assembly hall, 14 x 10 m: tall lit windows, a portico of four columns under a
    pediment, a blue hipped roof and a square blue dome on a lit drum, two flag masts."""
    g = sc.still()
    w, d = 1400.0, 1000.0
    top = 1040.0
    g.box(WOOD, (0, 0, (FOOT + 300.0) / 2.0), (w + 30.0, d + 30.0, 300.0 - FOOT))
    g.box(CREAM, (0, 0, (300.0 + top) / 2.0), (w, d, top - 300.0))
    g.box(WOOD_D, (0, 0, 312.0), (w + 50.0, d + 50.0, 20.0))
    g.box(CREAM, (0, 0, top + 20.0), (w + 100.0, d + 100.0, 40.0))
    frustum(g, BLUE, 0, 0, top + 40.0, w + 60.0, d + 60.0, 330.0, 0.8)
    g.box(CREAM, (0, 0, top + 470.0), (480.0, 400.0, 260.0))                    # the drum
    for face in ('f', 'l', 'r'):
        onface(g, WARM, face, 480.0, 400.0, 0.0, top + 480.0, 250.0, 130.0, out=2.0, glow=LIT)
    g.box(CREAM, (0, 0, top + 612.0), (540.0, 460.0, 24.0))
    g.sphere(BLUE, (0, 0, top + 624.0), 250.0, scale=(1.0, 0.86, 1.2), steps=(5, 8))
    g.cyl(STEEL, (0, 0, top + 900.0), 6.0, 170.0, steps=6)
    g.sphere(WARM, (0, 0, top + 1080.0), 20.0, steps=(3, 6))
    for j in range(4):                                     # the tall windows either side of the portico
        a = (-560.0, -380.0, 380.0, 560.0)[j]
        onface(g, GLASS, 'f', w, d, a, 640.0, 120.0, 520.0)
        onface(g, WARM, 'f', w, d, a, 620.0, 86.0, 440.0, out=3.0, glow=LIT)
        flowers(g, 'f', w, d, a, 366.0, 130.0)
        awning(g, 'f', w, d, a, 920.0, 150.0, 70.0, 40.0, BLUE if j % 2 == 0 else CREAM)
    for face in ('l', 'r'):
        for k, a in enumerate((-300.0, 0.0, 300.0)):
            onface(g, GLASS, face, w, d, a, 640.0, 130.0, 520.0)
            if (k + (face == 'l')) % 2 == 0:
                onface(g, WARM, face, w, d, a, 620.0, 96.0, 440.0, out=3.0, glow=LIT)
            awning(g, face, w, d, a, 920.0, 160.0, 70.0, 40.0)
    for i in range(8):                                     # a balustrade along the roof's front edge, urns at the corners
        g.box(CREAM, (-w / 2.0 - 20.0 + i * (w + 40.0) / 7.0, -d / 2.0 - 30.0, top + 70.0), (30.0, 30.0, 60.0))
    g.box(CREAM, (0, -d / 2.0 - 30.0, top + 100.0), (w + 80.0, 26.0, 12.0))
    for sx in (-1, 1):
        urn(g, sx * (w / 2.0 + 20.0), -d / 2.0 - 30.0, top + 106.0, 1.6)
        lantern(g, sx * 200.0, -d / 2.0, 520.0)
        for sy in (-1, 1):
            g.box(WOOD, (sx * (w / 2.0 - 8.0), sy * (d / 2.0 - 8.0), (300.0 + top) / 2.0), (40.0, 40.0, top - 300.0))
    py = -d / 2.0 - 150.0                                  # the portico
    g.box(WOOD_D, (0, py, (FOOT + 300.0) / 2.0), (640.0, 300.0, 300.0 - FOOT))
    g.box(WOOD, (0, py - 190.0, (FOOT + 150.0) / 2.0), (520.0, 90.0, 150.0 - FOOT))
    for i in range(4):
        g.cyl(CREAM, (-240.0 + i * 160.0, py - 100.0, 300.0), 24.0, 620.0, steps=6)
    g.box(CREAM, (0, py - 10.0, 950.0), (660.0, 300.0, 60.0))
    g.side(CREAM, [(-340.0, 980.0), (340.0, 980.0), (0.0, 1160.0)], py - 160.0, py + 140.0)
    g.side(BLUE, [(-250.0, 1000.0), (250.0, 1000.0), (0.0, 1132.0)], py - 166.0, py - 158.0)
    onface(g, GLASS, 'f', w, d, 0.0, 560.0, 260.0, 500.0)
    onface(g, WARM, 'f', w, d, 0.0, 720.0, 220.0, 150.0, out=3.0, glow=LIT)
    balcony(g, 'f', 660.0, 300.0, 990.0, 620.0, c=(0.0, py - 10.0), deep=30.0)   # an iron rail on the portico's roof
    for k, s in enumerate((-1, 1)):
        g.cyl(STEEL, (s * 620.0, -d / 2.0 + 60.0, top + 40.0), 6.0, 420.0, steps=6)
        pennant(sc, s * 620.0, -d / 2.0 + 60.0, top + 450.0, phase=0.15 + 0.5 * k, length=170.0)
        g.box(WOOD_D, (s * 480.0, 250.0, top + 330.0), (90.0, 90.0, 190.0))


# ------------------------------------------------------------------------------------------ props
def bench_pair(sc):
    """LOW (1.15 m). Two cast-iron benches with timber slats either side of a tub of pink alpine roses."""
    g = sc.still()
    for s in (-1, 1):
        x = s * 135.0
        g.box(WOOD, (x, 0, 46.0), (170.0, 46.0, 8.0))
        g.box(WOOD, (x, 24.0, 82.0), (170.0, 8.0, 46.0))
        g.box(WOOD_D, (x, 24.0, 110.0), (176.0, 9.0, 8.0))
        for e in (-1, 1):
            g.box(IRON, (x + e * 80.0, 4.0, 22.0), (8.0, 54.0, 44.0))
            g.box(IRON, (x + e * 80.0, 26.0, 64.0), (8.0, 10.0, 84.0))
            g.box(IRON, (x + e * 80.0, -6.0, 60.0), (8.0, 44.0, 6.0))
    g.box(WOOD_D, (0, 4.0, 32.0), (70.0, 70.0, 64.0))
    g.box(BLUE, (0, 4.0, 58.0), (76.0, 76.0, 8.0))
    g.box(PINE, (0, 4.0, 76.0), (60.0, 60.0, 24.0))
    for dx, dy in ((-16, -14), (16, 12), (-12, 16), (14, -16), (0, 0)):
        g.box(PINK, (dx, 4.0 + dy, 96.0), (20.0, 20.0, 16.0))


def luggage_cart(sc):
    """LOW (1.22 m). A porter's hand cart loaded with the guests' trunks: timber deck, iron hoop, trunks, a hat box."""
    g = sc.still()
    g.box(WOOD_D, (0, 0, 34.0), (220.0, 84.0, 10.0))
    for sx in (-70.0, 70.0):
        for sy in (-1, 1):
            g.cyl(IRON, (sx, sy * 46.0, 16.0), 16.0, 8.0, rot=(0, 0, 90 * sy), steps=8)
    for e in (-1, 1):                                      # the hoop at each end
        for sy in (-1, 1):
            g.box(IRON, (e * 106.0, sy * 36.0, 78.0), (6.0, 6.0, 80.0))
        g.box(IRON, (e * 106.0, 0, 119.0), (6.0, 78.0, 6.0))
    g.box(IRON, (128.0, 0, 96.0), (40.0, 6.0, 6.0))
    g.box(WOOD, (-52.0, 0, 62.0), (90.0, 70.0, 46.0))                           # trunks
    g.box(WOOD_D, (-52.0, 0, 62.0), (8.0, 74.0, 50.0))
    g.box(CREAM, (40.0, -6.0, 56.0), (80.0, 56.0, 34.0))
    g.box(WOOD_D, (40.0, -6.0, 56.0), (84.0, 8.0, 38.0))
    g.box(PINK, (-58.0, 4.0, 98.0), (64.0, 44.0, 24.0))
    g.box(WOOD, (34.0, 0, 88.0), (60.0, 44.0, 28.0))
    g.cyl(BLUE, (72.0, 8.0, 74.0), 20.0, 30.0, steps=8)                          # the hat box
    g.cyl(CREAM, (72.0, 8.0, 104.0), 21.0, 6.0, steps=8)


def flower_rail(sc):
    """LOW (1.2 m). Five metres of the promenade's stone balustrade: cream rail on short balusters, a pier at each
    end with a bowl of pink flowers."""
    g = sc.still()
    g.box(STONE, (0, 0, 8.0), (500.0, 40.0, 16.0))
    g.box(CREAM, (0, 0, 80.0), (500.0, 34.0, 12.0))
    for i in range(9):
        g.box(CREAM, (-200.0 + i * 50.0, 0, 45.0), (16.0, 16.0, 58.0))
    for s in (-1, 1):
        g.box(CREAM, (s * 240.0, 0, 46.0), (46.0, 46.0, 92.0))
        g.box(WOOD_D, (s * 240.0, 0, 97.0), (40.0, 40.0, 10.0))
        g.box(PINK, (s * 240.0, 0, 110.0), (34.0, 34.0, 18.0))
    g.box(BLUE, (0, -19.0, 80.0), (120.0, 5.0, 26.0))                           # a plain blue cartouche, no word on it


def fountain(sc):
    """TALL (3.2 m). The promenade's fountain, frozen: three bowls on a stem over an eight-sided basin, the water
    stopped in blue ice in each, icicles under the rims, pink flower tubs at the corners."""
    g = sc.still()
    g.cyl(STONE, (0, 0, -60.0), 190.0, 120.0, steps=8)
    g.cyl(CREAM, (0, 0, 54.0), 200.0, 14.0, steps=8)
    g.cyl(ICE_D, (0, 0, 46.0), 172.0, 14.0, steps=8)
    g.cyl(CREAM, (0, 0, 60.0), 26.0, 250.0, steps=6, top_r=14.0)
    for r, z, n in ((100.0, 140.0, 6), (62.0, 220.0, 5)):
        g.cyl(CREAM, (0, 0, z), 30.0, 26.0, steps=8, top_r=r)
        g.cyl(ICE_D, (0, 0, z + 24.0), r - 8.0, 8.0, steps=8)
        for k in range(n):
            x, y = pol(r - 6.0, k * 360.0 / n + 20.0)
            g.cyl(BLUE_D, (x, y, z + 24.0), 9.0, 60.0 + 14.0 * (k % 2), rot=(180, 0, 0), steps=5, top_r=2.0)
    g.cyl(BLUE, (0, 0, 300.0), 16.0, 24.0, steps=6, top_r=4.0)
    for k in range(4):
        x, y = pol(250.0, 45.0 + k * 90.0)
        g.box(WOOD_D, (x, y, 26.0), (56.0, 56.0, 52.0))
        g.box(PINK, (x, y, 60.0), (46.0, 46.0, 18.0))


def changing_cabins(sc):
    """TALL (3.4 m). Two bathers' cabins in blue and cream stripes, a pointed blue roof and a ball each, on a timber
    duckboard; a pink towel on a rail between them."""
    g = sc.still()
    g.box(WOOD, (0, 0, 0.0), (420.0, 200.0, 40.0))
    for s in (-1, 1):
        x = s * 110.0
        for k in range(5):
            g.box(BLUE if k % 2 == 0 else CREAM, (x - 64.0 + k * 32.0, 10.0, 130.0), (32.0, 150.0, 220.0))
        g.box(WOOD_D, (x, -68.0, 115.0), (70.0, 8.0, 180.0))
        g.box(CREAM, (x, -70.0, 170.0), (26.0, 8.0, 26.0), rot=(45, 0, 0))
        g.box(CREAM, (x, 10.0, 246.0), (180.0, 170.0, 12.0))
        frustum(g, BLUE, x, 10.0, 252.0, 200.0, 190.0, 70.0, 0.05)
        g.sphere(CREAM, (x, 10.0, 332.0), 11.0, steps=(3, 6))
        g.box(WOOD_D, (x, -100.0, 24.0), (90.0, 40.0, 16.0))
    g.box(IRON, (0, -60.0, 150.0), (60.0, 5.0, 5.0))
    g.box(PINK, (0, -62.0, 122.0), (44.0, 6.0, 56.0))


def kiosk(sc):
    """TALL (5.1 m). The promenade's kiosk: eight-sided, blue, a cream band, a lit serving hatch under a striped
    canopy, shape posters (no words), a pointed roof with a ball and a pennant."""
    g = sc.still()
    g.cyl(WOOD, (0, 0, -80.0), 140.0, 130.0, rot=(0, 22.5, 0), steps=8)
    g.cyl(BLUE, (0, 0, 50.0), 126.0, 250.0, rot=(0, 22.5, 0), steps=8)
    g.cyl(CREAM, (0, 0, 130.0), 130.0, 30.0, rot=(0, 22.5, 0), steps=8)
    g.box(GLASS, (0, -116.0, 205.0), (86.0, 12.0, 90.0))
    g.box(WARM, (0, -122.0, 205.0), (74.0, 10.0, 66.0), glow=LIT)
    g.box(WOOD_D, (0, -138.0, 156.0), (110.0, 40.0, 8.0))
    for k, a in enumerate((-135.0, -45.0, 0.0, 180.0)):   # posters: a disc, bars, a stripe
        x, y = pol(117.0, a)
        g.box(CREAM, (x, y, 210.0), (8.0, 60.0, 90.0), rot=(0, a, 0))
        g.box(PINK if k % 2 == 0 else BLUE_D, (x * 1.03, y * 1.03, 220.0 - 10.0 * k), (8.0, 36.0, 30.0), rot=(0, a, 0))
    g.cyl(CREAM, (0, 0, 300.0), 190.0, 16.0, rot=(0, 22.5, 0), steps=8)
    for k in range(8):
        x, y = pol(178.0, k * 45.0)
        g.box(BLUE if k % 2 == 0 else CREAM, (x, y, 280.0), (6.0, 120.0, 34.0), rot=(0, k * 45.0, 0))
    g.cyl(BLUE, (0, 0, 316.0), 200.0, 130.0, rot=(0, 22.5, 0), steps=8, top_r=6.0)
    g.sphere(WARM, (0, 0, 458.0), 14.0, steps=(3, 6))
    g.cyl(STEEL, (0, 0, 440.0), 4.5, 70.0, steps=6)
    pennant(sc, 0, 0, 510.0, length=70.0, h=30.0, phase=0.4, rate=0.5)


def globe_lamp(sc):
    """TALL (6.6 m). The promenade's lamp: an iron post on a timber foot, four arms and a crown with five globes
    (steady), a basket of pink flowers hung half-way up."""
    g = sc.still()
    g.box(WOOD_D, (0, 0, -40.0), (56.0, 56.0, 140.0))
    g.cyl(IRON, (0, 0, 30.0), 16.0, 60.0, steps=6, top_r=9.0)
    g.cyl(IRON, (0, 0, 90.0), 8.0, 500.0, steps=6)
    g.cyl(IRON, (0, 0, 318.0), 13.0, 10.0, steps=6)
    for k in range(4):
        x, y = pol(74.0, 45.0 + k * 90.0)
        tilt_box(g, IRON, (0, 0, 520.0), (x, y, 548.0), 6.0, 6.0)
        g.cyl(IRON, (x, y, 546.0), 9.0, 8.0, steps=6)
        g.sphere(WARM, (x, y, 580.0), 27.0, steps=(4, 6), glow=0.8)
    g.cyl(IRON, (0, 0, 590.0), 9.0, 8.0, steps=6)
    g.sphere(WARM, (0, 0, 628.0), 31.0, steps=(4, 6), glow=0.8)
    g.cyl(IRON, (0, 0, 656.0), 4.5, 16.0, steps=5, top_r=2.0)
    for s in (-1, 1):                                      # the flower baskets
        g.box(IRON, (s * 26.0, 0, 322.0), (52.0, 5.0, 5.0))
        g.cyl(WOOD_D, (s * 50.0, 0, 268.0), 12.0, 28.0, steps=6, top_r=22.0)
        g.box(PINK, (s * 50.0, 0, 304.0), (40.0, 40.0, 18.0))


def towel_line(sc):
    """TALL (2.7 m). The bath house's drying line: two timber posts with blue knobs, a line, six bath towels in
    blue, cream and pink that swing in the wind, a wicker basket under it."""
    g = sc.still()
    r = random.Random(64)
    for s in (-1, 1):
        g.box(WOOD, (s * 220.0, 0, 85.0), (14.0, 14.0, 370.0))
        g.sphere(BLUE, (s * 220.0, 0, 276.0), 13.0, steps=(3, 6))
        tilt_box(g, WOOD_D, (s * 220.0, 0, 120.0), (s * 280.0, 0, -20.0), 8.0, 8.0)
    g.box(IRON, (0, 0, 246.0), (440.0, 4.0, 4.0))
    cols = (BLUE, CREAM, PINK, BLUE, CREAM, BLUE)
    for k in range(6):
        x = -170.0 + k * 68.0
        f = sc.part(P.motion(swing=9.0 + 4.0 * r.random(), pivot=(x, 0, 246.0), axis=0.0, rate=0.45 + 0.1 * r.random(), phase=r.random()))
        f.box(cols[k], (x, 0, 246.0 - 70.0 - 8.0 * (k % 2)), (54.0, 5.0, 140.0 + 16.0 * (k % 2)))
        f.box(CREAM if cols[k] != CREAM else BLUE, (x, 0, 246.0 - 118.0), (54.0, 6.0, 14.0))
    g.cyl(WOOD, (120.0, -50.0, 0.0), 30.0, 36.0, steps=8, top_r=38.0)
    g.box(CREAM, (120.0, -50.0, 40.0), (50.0, 44.0, 12.0))


# ------------------------------------------------------------------------------------------ the hero
def palace_of_baths(sc):
    """THE PALACE OF BATHS (the district's landmark, 130 m out on the left). On a timber-and-stone terrace 66 m long:
    a cream rotunda under a blue dome and a lit lantern (40 m over its terrace), a portico of six columns, a
    colonnade either side to a round turret with a pointed blue cap and a pennant, and in front, a step down, the
    open-air pool: blue water in a cream rim, parasols, and steam rising off it and off three hot cascades that
    spill over the terrace wall."""
    g = sc.still()
    Y0 = 350.0                                             # the rotunda's centre; the footprint is y -2150 .. +2150
    UP = 260.0                                             # the upper terrace
    g.box(WOOD, (0, -1225.0, -350.0), (6600.0, 1850.0, 700.0))                  # the pool terrace
    g.box(WOOD, (0, 925.0, (UP - 700.0) / 2.0), (6600.0, 2450.0, UP + 700.0))    # the upper terrace
    g.box(STONE, (0, -2150.0, -40.0), (6640.0, 40.0, 80.0))
    g.box(WOOD_D, (0, -2156.0, -330.0), (6640.0, 30.0, 60.0))
    g.box(CREAM, (0, -2130.0, 60.0), (6600.0, 30.0, 14.0))                      # the front balustrade
    for i in range(8):
        x = -3270.0 + i * 934.3
        g.box(CREAM, (x, -2130.0, 40.0), (60.0, 60.0, 110.0))
        if i % 2 == 0:
            g.box(PINK, (x, -2130.0, 106.0), (50.0, 50.0, 24.0))
    g.box(CREAM, (0, -300.0, UP + 50.0), (6600.0, 30.0, 14.0))                   # the upper balustrade
    for i in range(6):
        x = -3270.0 + i * 1308.0
        g.box(CREAM, (x, -300.0, UP + 34.0), (56.0, 56.0, 96.0))
        g.box(PINK, (x, -300.0, UP + 92.0), (46.0, 46.0, 22.0))
    for k in range(4):                                     # the stair from the pool up to the portico
        g.box(CREAM, (0, -340.0 - k * 50.0, UP - 32.0 - k * 65.0), (760.0, 60.0, 65.0))
    # ---- the pool
    oval = [(2050.0 * math.cos(math.radians(a)), 620.0 * math.sin(math.radians(a))) for a in range(0, 360, 24)]
    g.plan(CREAM, oval, 0.0, 46.0, loc=(0, -1230.0, 0))
    inner = [(1960.0 * math.cos(math.radians(a)), 540.0 * math.sin(math.radians(a))) for a in range(0, 360, 24)]
    g.plan(BLUE_D, inner, 40.0, 54.0, loc=(0, -1230.0, 0))
    g.cyl(CREAM, (0, -1230.0, 40.0), 90.0, 90.0, steps=8, top_r=50.0)            # the spring's spout in the middle
    for k, (x, y, col) in enumerate(((-2500.0, -1700.0, BLUE), (-2350.0, -780.0, CREAM), (2500.0, -1700.0, CREAM),
                                     (2350.0, -780.0, BLUE))):
        g.cyl(IRON, (x, y, 0.0), 6.0, 230.0, steps=5)
        g.cyl(col, (x, y, 200.0), 150.0, 60.0, steps=8, top_r=8.0)
        g.box(WOOD, (x + 120.0, y, 30.0), (170.0, 60.0, 14.0))                   # a deck chair by it
        g.box(BLUE if col == CREAM else CREAM, (x + 190.0, y, 62.0), (60.0, 60.0, 8.0), rot=(40, 0, 0))
    for k, x in enumerate((-2200.0, 0.0, 2200.0)):         # the hot cascades over the terrace wall
        g.box(BLUE_D, (x, -2176.0, -330.0), (260.0, 16.0, 660.0))
        g.box(ICE_D, (x, -2180.0, -330.0), (120.0, 16.0, 660.0))
        g.box(CREAM, (x, -2160.0, 16.0), (340.0, 70.0, 40.0))
        plume(sc, x, -2230.0, -600.0, 150.0, 900.0, 0.2 + 0.3 * k, lean=8.0, n=2)
    for k, (x, y) in enumerate(((-1300.0, -1230.0), (-450.0, -1100.0), (500.0, -1330.0), (1350.0, -1180.0))):
        plume(sc, x, y, 60.0, 130.0 + 20.0 * (k % 2), 1100.0 + 150.0 * (k % 3), 0.13 * k, lean=9.0 + k, n=3)
    # ---- the rotunda
    R = 1050.0
    g.cyl(WOOD, (0, Y0, UP - 40.0), R + 20.0, 300.0, steps=12)
    g.cyl(CREAM, (0, Y0, UP + 260.0), R, 1440.0, steps=12)
    g.cyl(CREAM, (0, Y0, UP + 1700.0), R + 80.0, 60.0, steps=12)
    g.cyl(IRON, (0, Y0, UP + 1760.0), R + 60.0, 50.0, steps=12)
    for k in range(12):                                    # tall arched windows round the drum (the back three left out)
        a = -90.0 + 15.0 + k * 30.0
        if 45.0 < (a % 360.0) < 135.0:
            continue
        x, y = pol(R * 0.966, a)
        g.box(GLASS, (x, Y0 + y, UP + 900.0), (14.0, 250.0, 800.0), rot=(0, a, 0))
        if k % 3 != 1:
            x, y = pol(R * 0.966 + 5.0, a)
            g.box(WARM, (x, Y0 + y, UP + 860.0), (12.0, 190.0, 680.0), rot=(0, a, 0), glow=LIT)
        x, y = pol(R * 0.966 + 34.0, a)
        g.box(PINK, (x, Y0 + y, UP + 480.0), (40.0, 230.0, 26.0), rot=(0, a, 0))
    g.cyl(CREAM, (0, Y0, UP + 1810.0), 900.0, 560.0, steps=12)                   # the attic drum
    for k in range(12):
        a = -75.0 + k * 30.0
        if 45.0 < (a % 360.0) < 135.0:
            continue
        x, y = pol(872.0, a)
        g.box(WARM, (x, Y0 + y, UP + 2090.0), (12.0, 130.0, 130.0), rot=(0, a, 0), glow=LIT)
    g.cyl(CREAM, (0, Y0, UP + 2370.0), 960.0, 50.0, steps=12)
    g.sphere(BLUE, (0, Y0, UP + 2420.0), 900.0, scale=(1.0, 1.0, 1.12), steps=(8, 12))
    g.cyl(BLUE_D, (0, Y0, UP + 2420.0 + 470.0), 812.0, 34.0, steps=12)           # a band round the dome
    g.cyl(CREAM, (0, Y0, UP + 3380.0), 170.0, 250.0, steps=8)                    # the lantern
    g.cyl(WARM, (0, Y0, UP + 3440.0), 176.0, 130.0, steps=8, glow=LIT)
    g.cyl(CREAM, (0, Y0, UP + 3630.0), 210.0, 26.0, steps=8)
    g.cyl(BLUE, (0, Y0, UP + 3656.0), 220.0, 300.0, steps=8, top_r=6.0)
    g.cyl(STEEL, (0, Y0, UP + 3940.0), 7.0, 160.0, steps=6)
    g.sphere(WARM, (0, Y0, UP + 3930.0), 30.0, steps=(4, 6))
    pennant(sc, 0, Y0, UP + 4100.0, length=260.0, h=90.0, phase=0.1, rate=0.3)
    # ---- the portico
    py = Y0 - R - 170.0
    g.box(CREAM, (0, py + 60.0, UP + 1010.0), (1100.0, 460.0, 70.0))
    for i in range(6):
        g.cyl(CREAM, (-475.0 + i * 190.0, py - 110.0, UP), 36.0, 980.0, steps=6)
    g.side(CREAM, [(-580.0, UP + 1045.0), (580.0, UP + 1045.0), (0.0, UP + 1340.0)], py - 170.0, py + 290.0)
    g.side(BLUE, [(-430.0, UP + 1080.0), (430.0, UP + 1080.0), (0.0, UP + 1290.0)], py - 180.0, py - 168.0)
    g.box(GLASS, (0, Y0 - R + 4.0, UP + 400.0), (320.0, 30.0, 700.0))
    g.box(WARM, (0, Y0 - R - 8.0, UP + 520.0), (250.0, 20.0, 380.0), glow=LIT)
    # ---- the colonnades and the turrets
    for k, s in enumerate((-1, 1)):
        cx = s * 1650.0
        g.box(CREAM, (cx, Y0 + 100.0, UP + 380.0), (1300.0, 60.0, 760.0))        # the back wall
        g.box(WOOD, (cx, Y0 + 96.0, UP + 120.0), (1300.0, 70.0, 240.0))
        g.box(CREAM, (cx, Y0 - 150.0, UP + 790.0), (1340.0, 620.0, 60.0))
        g.box(BLUE, (cx, Y0 - 170.0, UP + 880.0), (1360.0, 700.0, 20.0), rot=(0, 0, -9))
        for i in range(5):
            x = cx - 520.0 + i * 260.0
            g.cyl(CREAM, (x, Y0 - 400.0, UP), 30.0, 760.0, steps=6)
            g.box(BLUE if i % 2 == 0 else CREAM, (x, Y0 - 470.0, UP + 730.0), (200.0, 8.0, 70.0))
            if i < 4:
                g.box(GLASS, (x + 130.0, Y0 + 66.0, UP + 440.0), (130.0, 14.0, 420.0))
                if (i + k) % 2 == 0:
                    g.box(WARM, (x + 130.0, Y0 + 58.0, UP + 430.0), (96.0, 12.0, 350.0), glow=LIT)
        tx = s * 2650.0
        g.cyl(WOOD, (tx, Y0, UP - 40.0), 370.0, 320.0, rot=(0, 22.5, 0), steps=8)
        g.cyl(CREAM, (tx, Y0, UP + 280.0), 350.0, 1900.0, rot=(0, 22.5, 0), steps=8)
        g.cyl(IRON, (tx, Y0, UP + 1500.0), 410.0, 16.0, rot=(0, 22.5, 0), steps=8)                 # a balcony ring
        g.cyl(IRON, (tx, Y0, UP + 1516.0), 404.0, 70.0, rot=(0, 22.5, 0), steps=8)
        g.cyl(CREAM, (tx, Y0, UP + 1516.0), 398.0, 72.0, rot=(0, 22.5, 0), steps=8)
        g.cyl(CREAM, (tx, Y0, UP + 2180.0), 400.0, 50.0, rot=(0, 22.5, 0), steps=8)
        g.cyl(BLUE, (tx, Y0, UP + 2230.0), 420.0, 820.0, rot=(0, 22.5, 0), steps=8, top_r=8.0)
        g.cyl(STEEL, (tx, Y0, UP + 3030.0), 7.0, 200.0, steps=6)
        for f in range(3):
            z = UP + 620.0 + f * 520.0
            for a in (-90.0, -135.0, -45.0):
                x, y = pol(321.0, a)
                g.box(GLASS, (tx + x, Y0 + y, z), (14.0, 120.0, 220.0), rot=(0, a, 0))
                if (f + int(a)) % 2 == 0:
                    x, y = pol(326.0, a)
                    g.box(WARM, (tx + x, Y0 + y, z), (12.0, 84.0, 180.0), rot=(0, a, 0), glow=LIT)
        pennant(sc, tx, Y0, UP + 3220.0, length=220.0, h=80.0, phase=0.35 + 0.3 * k, rate=0.32)
    for s in (-1, 1):                                      # blue spruces in tubs either side of the stair
        g.box(WOOD_D, (s * 520.0, -420.0, UP + 40.0), (110.0, 110.0, 80.0))
        g.cyl(PINE, (s * 520.0, -420.0, UP + 80.0), 90.0, 330.0, steps=6, top_r=6.0)


JOBS = [
    ('SM_S1D6_PalaceHotel', palace_hotel), ('SM_S1D6_SpaHotel', spa_hotel), ('SM_S1D6_TurretVilla', turret_villa),
    ('SM_S1D6_PumpRoom', pump_room), ('SM_S1D6_BandstandHall', bandstand_hall), ('SM_S1D6_ColonnadeWalk', colonnade_walk, STILL),
    ('SM_S1D6_Funicular', funicular), ('SM_S1D6_Kurhaus', kurhaus),
    ('SM_S1D6_BenchPair', bench_pair, STILL), ('SM_S1D6_LuggageCart', luggage_cart, STILL), ('SM_S1D6_FlowerRail', flower_rail, STILL),
    ('SM_S1D6_Fountain', fountain, STILL), ('SM_S1D6_ChangingCabins', changing_cabins, STILL), ('SM_S1D6_Kiosk', kiosk),
    ('SM_S1D6_GlobeLamp', globe_lamp, STILL), ('SM_S1D6_TowelLine', towel_line),
    ('SM_S1D6_PalaceOfBaths', palace_of_baths, dict(far=True, grow=(260, 260, 200), grow_down=(260, 260, 0))),
]


def run(only=None):
    P.build_core()                                         # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
