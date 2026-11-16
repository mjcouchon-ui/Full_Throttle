"""Theme Park, Stage 1, builder s1animals: the animals of the eight districts. /Game/World/ThemePark/SM_S1ANIMALS_*.
Run: import ft_world_park_s1animals as K; K.run()

Where they stand (the table block C:\\ftwork\\s1\\L4_s1animals.inl): the road has a 1.6 m barrier and the land lies under
it, so no animal of this kit stands on the bare ground: each scene brings what it stands on (a garden wall, a kerb
wall, mooring piles, a rock, a paddock on a plinth, a knoll, an ice shelf, a plank on flowerpots, a rimmed pool).
Over the lagoon the land is 26 m under the rails: the three water pieces are rimmed pools on a deep foot, pale and
lit, made to be seen from above at 2.5 times their size. The sky pieces fly a straight line ALONG the mesh's X (the
road's direction as placed) and come round again: nothing circles, nothing is dark, nothing flies over the road.

Every piece: ft_people.Scene, one section, M_People (far=True: M_PeopleFar, the lagoon's pools and the sky). Movement
only by the builders' motion values. A NIGHT level: every colour carries a small steady glow (0.09; the birds of the
sky 0.3, flat near-white all over: shaded birds read as crows here), so every rule of the block starts 36 m out or
more. Nothing here is a balloon, a clown, a crate or chest, nothing is orange or green and box-shaped, nothing hovers.
"""
import math
import random
import ft_people as P
from ft_desert import rod, tilt_box

DIR = '/Game/World/ThemePark'

# the kit's own palette (ft_dress_park.py)
CREAM, WHITE, RED, MAG, AMBER, GOLD = '#fff1d6', '#fff6ea', '#ff2d3c', '#ff4fb8', '#ffd23f', '#ffc93c'
CYAN, TEAL, LAV, PURPLE, PLUM, NAVY = '#2ee6ff', '#2fc6c0', '#b9a4ff', '#3a2f6a', '#4a3a78', '#2b2350'
WOOD, WOOD_D, TIN, TIN_D, PEACH, KRAKEN = '#9a6a44', '#6a4c36', '#a8b4d0', '#7f8aa6', '#ffc59e', '#c2408f'
POOL, SPRAY, LAWN, LAWN_D = '#3a5fa8', '#dff6ff', '#4f9a6a', '#2f6b52'
EYE = '#22202a'


def lift(sc, g):
    """Every colour of a piece gets a steady glow of its own (the park's own way, ft_people_park.lift): in this night
    level a piece in plain colours reads as a dark shape. Only for pieces that stand 24 m or more from the road."""
    for key in sc.order:
        model = sc.parts[key][0]
        out = {}
        for (col, glow), pm in model.meshes.items():
            k2 = (col, max(glow, g))
            while k2 in out:
                k2 = (col, k2[1] + 0.0002)
            out[k2] = pm
        model.meshes = out


def lit(fn, g=0.09):
    def build(sc):
        fn(sc)
        lift(sc, g)
    build.__doc__ = fn.__doc__
    return build


class Fr(object):
    """A creature's own frame in the piece: at (x, y, z), turned by yaw (front = -Y at yaw 0), k times life size."""

    def __init__(self, sc, x, y, z=0.0, yaw=0.0, k=1.0):
        self.sc, self.at, self.yaw, self.k = sc, (x, y, z), yaw, k
        self.c, self.s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))

    def W(self, p):
        x, y, z = self.at
        return (x + self.k * (p[0] * self.c - p[1] * self.s), y + self.k * (p[0] * self.s + p[1] * self.c), z + self.k * p[2])

    def still(self):
        return self.sc.still(self.at, self.yaw, self.k)

    def part(self, pivot=(0, 0, 0), axis=0.0, **mo):
        """pivot in the creature's frame; axis: degrees from the creature's own X in the ground plane, or P.UP.
        base (travel, orbit) is given in the MESH's space, as the shader reads it."""
        return self.sc.part(P.motion(pivot=self.W(pivot), axis=(P.UP if axis == P.UP else self.yaw + axis), **mo), self.at, self.yaw, self.k)


# ------------------------------------------------------------------------------------------ creatures of plain shapes
def fowl(m, L=40.0, body=WHITE, neck=18.0, lean=0.3, beak=AMBER, beak_len=None, leg=0.0, legcol=AMBER, wing=None, tail=0.25,
         tail_up=0.15, eye=True, steps=(3, 6), bend=0.0, head=None, k=1.0):
    """A bird standing or afloat, drawn into m (front = -Y): L = the body's length, neck = the neck's length, lean =
    how far it leans ahead (0 upright, 1 level), leg = the legs' length (0: afloat), bend: an S in the neck (a swan).
    Returns the head's place in m's frame."""
    mn = 4.3 / k                                 # nothing thinner than 4 cm as built (k: the frame's size)
    zb = leg + L * 0.28
    lw = max(L * 0.07, mn)
    if leg:
        for s in (-1, 1):
            m.box(legcol, (s * L * 0.12, 0, leg / 2.0), (lw, lw, leg + 2.0))
    m.sphere(body, (0, 0, zb), L * 0.3, scale=(0.85, 1.6, 0.85), steps=steps)
    if tail:
        tilt_box(m, wing or body, (0, L * 0.36, zb + L * 0.04), (0, L * (0.36 + tail * 1.2), zb + L * (0.04 + tail_up)), L * 0.24, max(L * 0.1, mn))
    n0 = (0, -L * 0.38, zb + L * 0.08)
    n1 = (0, n0[1] - neck * lean, n0[2] + neck * math.sqrt(max(1.0 - lean * lean, 0.0)))
    nt = max(L * 0.15, mn)
    if bend:
        mid = (0, (n0[1] + n1[1]) / 2.0 + bend, (n0[2] + n1[2]) / 2.0)
        tilt_box(m, body, n0, mid, nt, nt)
        tilt_box(m, body, mid, n1, nt, nt)
    else:
        tilt_box(m, body, n0, n1, nt, nt)
    m.box(head or body, (0, n1[1] - L * 0.05, n1[2] + L * 0.04), (L * 0.2, L * 0.28, L * 0.2))
    bl = beak_len or L * 0.2
    m.box(beak, (0, n1[1] - L * 0.19 - bl / 2.0, n1[2] + L * 0.02), (max(L * 0.1, mn), bl, max(L * 0.09, mn)))
    if eye:
        e = max(L * 0.05, mn)
        for s in (-1, 1):
            m.box(EYE, (s * L * 0.1, n1[1] - L * 0.1, n1[2] + L * 0.08), (e, e, e))
    for s in (-1, 1):
        m.box(wing or body, (s * L * 0.25, L * 0.03, zb + L * 0.03), (max(L * 0.08, mn), L * 0.6, L * 0.3))
    return n1


def rabbit(sc, x, y, z, yaw=0.0, k=1.6, col=WHITE, ear_in=PEACH, phase=0.0, rate=1.3, bob=0.0, up=False, twitch=14.0):
    """A white rabbit (45 cm at k 1): it hops in place (bob), or sits up and twitches its ears."""
    f = Fr(sc, x, y, z, yaw, k)
    m = f.part(pivot=(0, 0, 0), phase=phase, rate=rate, bob=bob)
    if up:
        m.sphere(col, (0, 4, 17), 13, scale=(1.0, 1.0, 1.35), steps=(3, 6))
        hz, hy = 40.0, -3.0
        m.box(col, (0, -10, 22), (14, 6, 6))
    else:
        m.sphere(col, (0, 4, 13), 13, scale=(1.0, 1.5, 1.0), steps=(3, 6))
        hz, hy = 24.0, -14.0
    m.box(col, (0, hy, hz), (15, 17, 14))
    m.box(PEACH, (0, hy - 9.5, hz - 1), (5, 3, 4))
    for s in (-1, 1):
        m.box(EYE, (s * 7, hy - 5, hz + 2), (3, 3, 3))
        m.box(col, (s * 8, 2 if up else -4, 3), (6, 14, 6))
    m.sphere(col, (0, 20 if not up else 16, 12), 5, steps=(3, 5))
    e = f.part(pivot=(0, hy + 3, hz + 6), axis=0.0, phase=phase, rate=rate, bob=bob, swing=(twitch if up else 0.0))
    for s in (-1, 1):
        tilt_box(e, col, (s * 4, hy + 3, hz + 6), (s * 6, hy + 8, hz + 30), 6, 4)
        tilt_box(e, ear_in, (s * 4, hy + 0.5, hz + 9), (s * 6, hy + 5.5, hz + 27), 3, 3)


def cat(sc, x, y, z, yaw=0.0, k=1.8, col=WHITE, patch=WOOD, eye=AMBER, muzzle=5.0, phase=0.0, rate=0.5, wag=22.0, rings=None, mask=None):
    """A sitting cat (45 cm at k 1; rings and mask: a raccoon). Only its tail moves."""
    f = Fr(sc, x, y, z, yaw, k)
    g = f.still()
    g.box(col, (0, 8, 11), (24, 28, 22))
    g.cyl(col, (0, -4, 0), 11, 34, steps=6, top_r=8)
    g.box(patch, (0, -13, 17), (10, 5, 16))
    g.box(col, (0, -7, 42), (22, 18, 17))
    g.box(patch, (0, -16 - muzzle / 2.0, 39), (10, muzzle, 7))
    g.box(EYE, (0, -17 - muzzle, 41), (4, 4, 4))
    if mask:
        g.box(mask, (0, -15.5, 46), (23, 3, 6))
    for s in (-1, 1):
        g.box(eye, (s * 6, -16.5 if mask else -15, 46), (4, 4, 4))
        g.cyl(col, (s * 7, -6, 50), 4.5, 9, steps=4, top_r=1.0)
        g.box(patch, (s * 6, -13, 3), (7, 8, 6))
    t = f.part(pivot=(0, 20, 5), axis=P.UP, phase=phase, rate=rate, swing=wag)
    tilt_box(t, col, (0, 20, 5), (20, 30, 6), 6, 6)
    tilt_box(t, rings or col, (20, 30, 6), (30, 22, 13), 6, 6)
    if rings:
        t.box(rings, (10, 25, 6), (5, 8, 8))


def q_values(d):
    """quadruped()'s own defaults for the sizes this file reads back."""
    v = dict(length=150.0, height=120.0, girth=42.0, neck=50.0, neck_up=55.0, head=34.0)
    v.update({k2: d[k2] for k2 in v if k2 in d})
    return v


def q_body(sc, x, y, z=0.0, yaw=0.0, s=1.0, **d):
    """The model quadruped() draws the BODY of that animal into (the same motion values give the same part), in the
    animal's own frame: what is drawn here goes wherever the body goes. For ribbons, harness, antlers on a still head."""
    v = q_values(d)
    phase, rate, bob, travel = d.get('phase', 0.0), d.get('rate', 0.0), d.get('bob', 0.0), d.get('travel')
    cy, sy = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    piv = (x + s * (0 * cy - 0 * sy), y + s * (0 * sy + 0 * cy), z + s * v['height'])
    tv = (float(travel[0]), float(travel[1]) / float(travel[0])) if travel else (0.0, 0.0)
    return sc.part(P.motion(swing=0.0, pivot=piv, axis=yaw + 0.0, phase=phase, rate=rate, bob=bob, base=(x, y), travel=tv), (x, y, z), yaw, s), v


def antlers(m, v, col, s=1.0, tall=1.0):
    """Tines on the two prongs quadruped() draws (horns=col), for an animal that does not nod (graze 0)."""
    hl, g, hd = v['length'] / 2.0, v['girth'], v['head']
    a = math.radians(v['neck_up'])
    n1y = -hl + g * 0.25 - math.cos(a) * v['neck']
    n1z = v['height'] - g * 0.35 + math.sin(a) * v['neck']
    w = max(hd * 0.13, 4.4 / s)
    for side in (-1, 1):
        tip = (side * g * 0.42, n1y + hd * 0.1, n1z + hd * 0.8)
        top = (side * g * 0.8, n1y + hd * 0.3, n1z + hd * (0.8 + 0.8 * tall))
        tilt_box(m, col, tip, top, w, w)
        tilt_box(m, col, tip, (side * g * 0.62, n1y - hd * 0.45, n1z + hd * (0.8 + 0.5 * tall)), w, w)
        tilt_box(m, col, top, (side * g * 1.1, n1y + hd * 0.2, n1z + hd * (0.8 + 1.15 * tall)), w, w)
        tilt_box(m, col, top, (side * g * 0.7, n1y - hd * 0.1, n1z + hd * (0.8 + 1.25 * tall)), w, w)


def sky_bird(sc, x, y, z, span, travel, phase, rate, flap, bob=0.0, col=WHITE, neck=0.0, legs=0.0, face=None):
    """A bird of the sky flying +X along the mesh, one flat pale colour all over. neck: a neck stretched ahead (a swan,
    an egret tucks it: 0); legs: legs trailing behind; face: a flat face disc's colour (an owl)."""
    P.bird(sc, x, y, z, heading=0.0, span=span, body=col, wing=col, beak=col, phase=phase, rate=rate, flap=flap, travel=travel, bob=bob)
    tv = (float(travel[0]), float(travel[1]) / float(travel[0]))
    b = sc.part(P.motion(pivot=(x, y, z), phase=phase, rate=rate, bob=bob, base=(x, y), travel=tv))
    b.base = None
    L = span * 0.42
    if neck:
        tilt_box(b, col, (x + L * 0.5, y, z + L * 0.06), (x + L * 0.5 + neck, y, z + L * 0.1), L * 0.14, L * 0.14)
        b.box(col, (x + L * 0.5 + neck + L * 0.1, y, z + L * 0.1), (L * 0.26, L * 0.16, L * 0.16))
    if legs:
        tilt_box(b, col, (x - L * 0.5, y, z - L * 0.04), (x - L * 0.5 - legs, y, z - L * 0.08), L * 0.1, L * 0.07)
    if face:
        b.cyl(face, (x + L * 0.66, y, z + L * 0.14), L * 0.24, L * 0.1, rot=(-90, 0, 0), steps=8)
        for s in (-1, 1):
            b.box(EYE, (x + L * 0.78, y + s * L * 0.09, z + L * 0.17), (4, L * 0.07, L * 0.07))


def pool(g, r=400.0, rim=WOOD_D, wall=POOL, water=CYAN, foot=260.0, lip=60.0, stones=10, glow=0.3):
    """A round rimmed pool on a deep foot: the wall goes `foot` cm under z = 0 (the lagoon's land falls away and is
    under 1.15 m of water in the hollows), the lit water lies `lip` cm up, a ring of stones on the rim."""
    g.cyl(wall, (0, 0, -foot), r, foot + lip - 6.0, steps=12)
    g.cyl(water, (0, 0, lip - 6.0), r - 14.0, 6.0, steps=12, glow=glow)
    for k in range(stones):
        a = 2.0 * math.pi * (k + 0.5) / stones
        w = 2.0 * math.pi * r / stones * 0.82
        g.box(rim if k % 2 else TIN_D, (math.cos(a) * (r + 4), math.sin(a) * (r + 4), lip - 14.0), (44.0, w, 52.0 + 10.0 * (k % 3)), rot=(0, math.degrees(a), 0))


def reeds(g, x, y, z, n, r, h=150.0, col=TEAL, head=WOOD_D, spread=40.0):
    for k in range(n):
        a = r.uniform(0, 6.283)
        d = r.uniform(0, spread)
        hh = h * r.uniform(0.7, 1.15)
        px, py = x + math.cos(a) * d, y + math.sin(a) * d
        tilt_box(g, col, (px, py, z), (px + r.uniform(-14, 14), py + r.uniform(-14, 14), z + hh), 5, 5)
        if head and k % 2 == 0:
            g.box(head, (px, py, z + hh * 0.86), (9, 9, 30))


# ------------------------------------------------------------------------------------------ 1. white peacocks on a garden wall
def peacock_wall(sc):
    """Front Gate, Crescent Keep. A cream garden wall 7 m long and 2.3 m high with a gold coping, an arched niche and
    two urns of clipped box; two WHITE peacocks on it: one trails its train down the front and tips to and fro, one stands with
    its fan up, the fan shimmering (it tips a little to and fro)."""
    g = sc.still()
    g.box(CREAM, (0, 0, 40), (700, 56, 380))                     # the wall, 1.5 m of footing
    g.box(GOLD, (0, 0, 236), (730, 76, 12))
    g.box(WHITE, (0, 0, 20), (716, 68, 40))
    g.box(NAVY, (0, -29, 105), (120, 6, 170))
    g.cyl(NAVY, (0, -32, 190), 60, 6, rot=(0, 0, 90), steps=10)      # roll 90: the disc's axis lies along +Y
    g.box(GOLD, (0, -31, 60), (60, 6, 10))
    for s in (-1, 1):
        g.box(CREAM, (s * 330, 0, 125), (72, 72, 250))
        g.cyl(TEAL, (s * 330, 0, 250), 30, 44, steps=8, top_r=40)
        g.sphere(LAWN, (s * 330, 0, 330), 44, steps=(4, 8))
        for k in range(3):
            g.box(RED if k % 2 else MAG, (s * (120 + k * 62), -29, 150), (36, 6, 36), rot=(45, 0, 0))
    # the peacock with its train down the front
    f = Fr(sc, -150, -6, 242, yaw=12, k=1.7)
    b = f.part(pivot=(0, 0, 0), axis=0.0, phase=0.2, rate=0.35, swing=4)
    fowl(b, L=44, neck=30, lean=0.15, beak=GOLD, leg=16, legcol=GOLD, tail=0.0, steps=(4, 6), k=1.7)
    tilt_box(b, WHITE, (0, 14, 26), (10, 30, -70), 26, 8)
    tilt_box(b, SPRAY, (4, 22, -10), (16, 36, -112), 34, 6)
    for k in range(4):
        b.box(LAV, (6 + k * 3, 24 + k * 3.5, -30 - k * 22), (9, 4, 9), glow=0.2)
    for k in range(3):
        b.box(CYAN, ((k - 1) * 6, -23, 70 + (2 if k == 1 else 0)), (4, 4, 10), glow=0.25)
    # the peacock with its fan up
    f = Fr(sc, 150, 0, 242, yaw=-8, k=1.7)
    b = f.still()
    hd = fowl(b, L=44, neck=30, lean=0.15, beak=GOLD, leg=16, legcol=GOLD, tail=0.0, steps=(4, 6), k=1.7)
    for k in range(3):
        b.box(CYAN, ((k - 1) * 6, hd[1] - 2, hd[2] + 14 + (2 if k == 1 else 0)), (4, 4, 10), glow=0.25)
    fan = f.part(pivot=(0, 12, 26), axis=0.0, phase=0.0, rate=0.45, swing=7)
    for k in range(9):
        a = math.radians(-80 + k * 20)
        tip = (math.sin(a) * 86, 14, 26 + math.cos(a) * 86)
        tilt_box(fan, WHITE if k % 2 else SPRAY, (math.sin(a) * 8, 14, 30), tip, 5, 24)
        fan.box(LAV, (tip[0] * 0.9, 10.5, 26 + (tip[2] - 26) * 0.9), (10, 4, 10), glow=0.25)


# ------------------------------------------------------------------------------------------ 2. geese in file with their goose-herd
def geese_line(sc):
    """Front Gate, Sweet Street. A striped promenade kerb wall 15 m long and 1.1 m high; six white geese waddle along
    it in file and come round again, a goose-herd with a crook walking behind them."""
    g = sc.still()
    g.box(CREAM, (0, 0, -20), (1500, 110, 260))
    g.box(WHITE, (0, 0, 104), (1520, 126, 12))
    for k in range(10):
        g.box(RED if k % 2 else MAG, (-675 + k * 150, -56, 50), (75, 5, 100))
    for s in (-1, 1):
        g.box(CREAM, (s * 760, 0, 60), (60, 140, 220))
        g.cyl(AMBER, (s * 760, 0, 170), 26, 20, steps=8, top_r=8)
    SPEED, LEN = 60.0, 1380.0
    for k in range(6):
        x = 330 - k * 150
        f = Fr(sc, x, -8 + (k % 2) * 16, 110, yaw=90, k=1.45)
        m = f.part(pivot=(0, 0, 0), axis=90.0, phase=k * 0.31, rate=1.5, swing=6, base=(x, f.at[1]), travel=(LEN, SPEED / LEN))
        fowl(m, L=46, neck=26, lean=0.25, beak=AMBER, leg=14, legcol=AMBER, wing=SPRAY if k % 3 == 0 else None, k=1.45)
    r = random.Random(41)
    P.figure(sc, -640, 6, 110, yaw=90, top=TEAL, bottom=NAVY, hat='cowboy', hat_col=CREAM, wear='skirt', sleeves='long', detail=1,
             arm_r='fwd', hold_r=lambda mm, h, side: tilt_box(mm, WOOD_D, (h[0], h[1] + 10, h[2] - 60), (h[0], h[1] - 30, h[2] + 90), 5, 5), stride=20, swing_l=18, axis_l='x', rate=1.1, phase=r.random(), travel=(LEN, SPEED),
             skin=P.SKINS[2], hair='pony', hair_col=P.HAIRS[3])


# ------------------------------------------------------------------------------------------ 3. pelicans on mooring piles
def pelican_posts(sc):
    """Pirate Cove. Three tarred mooring piles (2.6 to 4 m) on a stub of plank jetty, rope slung between them, a net
    of silver fish; a white pelican on each: one opens and folds its wings, two rock on their feet."""
    g = sc.still()
    g.box(WOOD, (0, 20, 40), (760, 220, 16))
    for k in range(6):
        g.box(WOOD_D, (-330 + k * 132, 20, -70), (20, 20, 240))
        g.box(WOOD_D, (-315 + k * 126, -92, 44), (110, 6, 22))
    piles = ((-250, 0, 300), (10, 30, 400), (270, -10, 260))
    for x, y, h in piles:
        g.cyl(WOOD_D, (x, y, -150), 30, h + 150, steps=8, top_r=26)
        g.cyl(TEAL, (x, y, h - 40), 31, 18, steps=8)
        g.cyl(CREAM, (x, y, h - 90), 30, 10, steps=8)
    tilt_box(g, CREAM, (-250, 0, 250), (-120, 15, 196), 6, 6)
    tilt_box(g, CREAM, (-120, 15, 196), (10, 30, 300), 6, 6)
    tilt_box(g, CREAM, (10, 30, 330), (150, 10, 200), 6, 6)
    tilt_box(g, CREAM, (150, 10, 200), (270, -10, 220), 6, 6)
    g.sphere(TIN, (140, -40, 78), 46, scale=(1.2, 1.0, 0.7), steps=(4, 6))          # the net of fish
    for k in range(5):
        g.box(SPRAY, (104 + k * 18, -60 - (k % 2) * 14, 100 + (k % 3) * 6), (26, 9, 9), rot=(0, 30 * k, 0))
    g.cyl(WOOD, (-150, -40, 48), 34, 22, steps=8)                                    # a coil of rope
    g.cyl(CREAM, (-150, -40, 70), 30, 14, steps=8)
    for i, (x, y, h) in enumerate(piles):
        f = Fr(sc, x, y, h, yaw=(-20, 10, 35)[i], k=1.55)
        m = f.part(pivot=(0, 0, 0), axis=0.0, phase=i * 0.37, rate=0.4, swing=(0 if i == 1 else 5))
        hd = fowl(m, L=52, neck=30, lean=0.2, beak=PEACH, beak_len=30, leg=12, legcol=PEACH, steps=(4, 6), tail=0.18, k=1.55)
        tilt_box(m, PEACH, (0, hd[1] - 12, hd[2] - 3), (0, hd[1] - 38, hd[2] - 9), 7, 12)   # the pouch
        if i == 1:
            for s in (-1, 1):
                w = f.part(pivot=(s * 11, 0, 36), axis=90.0, phase=0.1, rate=0.4, swing=s * 30)
                tilt_box(w, WHITE, (s * 11, 0, 36), (s * 58, 4, 52), 30, 6)
                tilt_box(w, TIN, (s * 58, 4, 52), (s * 96, 8, 50), 24, 5)


# ------------------------------------------------------------------------------------------ 4. crabs on a rock, the ship's cat
def crab_rock(sc):
    """Pirate Cove. A wet rock 2 m high with a rope-bound barrel beside it: four pink crabs on the rock, two sidling
    to and fro, two raising their claws; the ship's cat on the barrel, its tail going; a starfish."""
    g = sc.still()
    g.sphere(PLUM, (-40, 30, 40), 190, scale=(1.3, 0.9, 0.9), steps=(4, 8))
    g.sphere(TIN_D, (150, -10, 20), 120, scale=(1.2, 1.0, 0.9), steps=(4, 6))
    g.sphere(PURPLE, (-210, -30, 0), 90, scale=(1.1, 1.0, 0.9), steps=(3, 6))
    g.box(TIN_D, (-40, 30, -110), (420, 260, 220))
    for k in range(5):
        a = k * 1.256
        tilt_box(g, PEACH, (120, -96, 96), (120 + math.cos(a) * 26, -104, 96 + math.sin(a) * 26), 9, 6)   # the starfish
    g.cyl(WOOD, (300, 10, -100), 46, 210, steps=8, top_r=42)
    g.cyl(WOOD_D, (300, 10, 20), 48, 10, steps=8)
    g.cyl(WOOD_D, (300, 10, 78), 46, 10, steps=8)
    cat(sc, 300, 10, 110, yaw=-25, k=1.9, col=WHITE, patch=WOOD_D, eye=AMBER, phase=0.3)
    spots = ((-130, -60, 172, 0, 0), (40, -52, 181, 0, 1), (-40, 60, 210, 15, 0), (170, -40, 124, -10, 1))
    for i, (x, y, z, yaw, slide) in enumerate(spots):
        f = Fr(sc, x, y, z, yaw=yaw, k=1.5)
        if slide:
            m = sc.part(P.motion(pivot=(x, y, z - 2600.0), axis=90.0, swing=0.9, phase=i * 0.4, rate=0.45), (x, y, z), yaw, 1.5)
            claw = m
        else:
            m = f.still()
            claw = None
        m.sphere(KRAKEN, (0, 0, 9), 15, scale=(1.3, 0.9, 0.6), steps=(3, 6))
        for s in (-1, 1):
            m.box(EYE, (s * 6, -12, 16), (3.4, 3.4, 8))
            for j in range(3):
                tilt_box(m, KRAKEN, (s * 14, -6 + j * 7, 8), (s * 28, -8 + j * 9, 0), 3.4, 3.4)
            c = claw or f.part(pivot=(s * 14, -10, 10), axis=90.0, phase=i * 0.3 + (0.5 if s > 0 else 0), rate=0.9, swing=s * 24)
            tilt_box(c, PEACH, (s * 14, -10, 10), (s * 26, -16, 22), 6, 6)
            c.box(PEACH, (s * 28, -18, 27), (9, 7, 11))


# ------------------------------------------------------------------------------------------ 5. swans on a rimmed pool
def swan(sc, a_deg, rad, rps, k=1.5, col=WHITE, beak=PEACH, cx=0.0, cy=0.0, z=60.0, L=58.0, neck=44.0):
    """A swan (or a cygnet) afloat at angle a_deg on a circle rad round (cx, cy), swimming round it."""
    a = math.radians(a_deg)
    x, y = cx + math.cos(a) * rad, cy + math.sin(a) * rad
    yaw = a_deg + (180.0 if rps >= 0 else 0.0)
    m = sc.part(P.motion(pivot=(x, y, z), base=(cx, cy), travel=(-1.0, rps)), (x, y, z - L * 0.14 * k), yaw, k)
    if neck > 20:
        hd = fowl(m, L=L, body=col, neck=neck, lean=0.1, beak=beak, steps=(3, 6), bend=-9.0, tail=0.2, tail_up=0.3, k=k)
        m.box(EYE, (0, hd[1] - L * 0.17, hd[2] + L * 0.1), (L * 0.1, L * 0.08, L * 0.08))
        for s in (-1, 1):
            tilt_box(m, col, (s * L * 0.2, L * 0.0, L * 0.4), (s * L * 0.26, L * 0.5, L * 0.72), max(L * 0.07, 4.3 / k), L * 0.34)
    else:
        fowl(m, L=L, body=col, neck=neck, lean=0.2, beak=beak, steps=(3, 6), tail=0.15, k=k)


def swans(sc):
    """The Lagoon, Crescent Keep. A stone-rimmed pool 8 m across on a deep foot, its water lit; a reed islet with a
    nest in the middle; three white swans swim round it, wings half raised, three grey cygnets after the first."""
    r = random.Random(52)
    g = sc.still()
    pool(g, r=400, rim=LAV, wall=POOL)
    g.sphere(LAWN_D, (0, 0, 56), 80, scale=(1.0, 1.0, 0.45), steps=(3, 8))
    g.cyl(WOOD, (10, 0, 80), 42, 14, steps=8, top_r=48)
    for k in range(3):
        g.sphere(CREAM, (10 + (k - 1) * 14, (k % 2) * 12 - 6, 98), 9, scale=(1, 1, 1.25), steps=(3, 5))
    reeds(g, -30, 20, 70, 9, r, h=170, spread=50)
    for k in range(4):
        a = k * 1.7 + 0.4
        g.cyl(TEAL, (math.cos(a) * 330, math.sin(a) * 330, 60), 36, 5, steps=7)
        if k % 2:
            g.cyl(MAG, (math.cos(a) * 330, math.sin(a) * 330, 65), 12, 14, steps=5, top_r=18, glow=0.2)
    swan(sc, 20, 235, 0.035)
    for k in range(3):
        swan(sc, 20 - 22 - k * 13, 235 + (k % 2) * 26 - 13, 0.035, k=1.4, col=TIN, beak=TIN_D, L=26, neck=12)
    swan(sc, 150, 300, 0.028)
    swan(sc, 265, 200, -0.03)


# ------------------------------------------------------------------------------------------ 6. koi leaping, turtles on a log
def koi_leap(sc):
    """The Lagoon. A rimmed lily pool on a deep foot, lit: four big koi (white, red and gold patches) leap out of the
    water in an arc and fall back; a log across the rim with three turtles on it, one on another's back, necks out."""
    g = sc.still()
    pool(g, r=380, rim=WOOD_D, wall=POOL)
    for k in range(6):
        a = k * 1.05 + 0.2
        d = 150 + (k % 3) * 80
        g.cyl(TEAL, (math.cos(a) * d, math.sin(a) * d, 60), 44 + (k % 2) * 12, 5, steps=7)
    for a, d in ((0.9, 150), (3.6, 230)):
        g.cyl(MAG, (math.cos(a) * d, math.sin(a) * d, 65), 14, 16, steps=6, top_r=24, glow=0.2)
        g.cyl(PEACH, (math.cos(a) * d, math.sin(a) * d, 78), 8, 14, steps=5, top_r=12, glow=0.2)
    leaps = ((-150, -120, 0, 0.0), (120, -200, 25, 0.37), (-170, 130, 10, 0.18), (160, 150, 5, 0.55))
    for i, (x, y, yaw, ph) in enumerate(leaps):
        R = 120.0
        f = Fr(sc, x, y, 44.0, yaw=yaw, k=1.0)
        m = f.part(pivot=(0, 0, 0), axis=90.0, phase=ph, rate=0.3 + 0.03 * i, swing=118)
        patch = (RED, GOLD, KRAKEN, RED)[i]
        m.sphere(WHITE, (0, 0, R), 24, scale=(2.3, 0.75, 1.0), steps=(4, 6))
        m.box(patch, (14, 0, R + 16), (40, 30, 14))
        m.box(patch, (-30, 0, R + 8), (22, 26, 22))
        tilt_box(m, WHITE, (-50, 0, R), (-86, 0, R + 22), 8, 34)
        tilt_box(m, WHITE, (-50, 0, R), (-86, 0, R - 22), 8, 34)
        m.box(patch, (0, 0, R + 30), (36, 6, 16))
        for s in (-1, 1):
            m.box(EYE, (44, s * 14, R + 4), (6, 5, 6))
            tilt_box(m, WHITE, (16, s * 16, R - 10), (0, s * 34, R - 22), 6, 16)
        g.cyl(SPRAY, (x, y, 61), 60, 5, steps=8, glow=0.3)                               # the ring it leaps from
    # the log and its turtles
    tilt_box(g, WOOD, (-420, -250, 96), (-60, -330, 80), 56, 50)
    g.cyl(WOOD_D, (-250, -290, 100), 12, 40, steps=5, top_r=6)
    for i, (t, up, k) in enumerate(((0.2, 0, 1.0), (0.55, 0, 1.15), (0.55, 36, 0.7))):
        x, y = -420 + 360 * t, -250 - 80 * t
        f = Fr(sc, x, y, 116 - 16 * t + up, yaw=-78 + i * 9, k=1.5 * k)
        s = f.still()
        s.sphere(TEAL, (0, 0, 10), 22, scale=(1.0, 1.25, 0.62), steps=(3, 6))
        s.box(LAWN_D, (0, 0, 22), (20, 26, 5))
        for sx in (-1, 1):
            for sy in (-1, 1):
                s.box(PEACH, (sx * 20, sy * 18, 4), (12, 10, 6))
        n = f.part(pivot=(0, -24, 8), axis=0.0, phase=i * 0.4, rate=0.3, swing=16)
        tilt_box(n, PEACH, (0, -24, 8), (0, -42, 20), 8, 8)
        n.box(PEACH, (0, -46, 24), (11, 13, 10))
        for sx in (-1, 1):
            n.box(EYE, (sx * 5, -50, 26), (3 / k, 3 / k, 3 / k))


# ------------------------------------------------------------------------------------------ 7. herons and otters
def herons_otters(sc):
    """The Lagoon. A reedy bank on a deep foot: two white herons stand in the lit shallows and strike (the neck
    dips); a smooth rock slide between two hollow stumps with two otters going down it one after the other; a third
    otter floats on its back, rocking."""
    r = random.Random(57)
    g = sc.still()
    g.box(POOL, (0, 0, -105), (980, 520, 310))
    g.box(CYAN, (0, -70, 53), (940, 350, 6), glow=0.3)
    g.box(LAWN_D, (0, 180, 70), (980, 170, 60))
    for k in range(7):
        g.sphere(TIN_D if k % 2 else PLUM, (-450 + k * 150, 110 + (k % 3) * 14, 62), 46 + (k % 3) * 10, scale=(1.3, 0.9, 0.7), steps=(3, 6))
    reeds(g, -380, 190, 96, 9, r, h=190, spread=60)
    reeds(g, 60, 200, 96, 8, r, h=170, spread=70)
    reeds(g, 400, 190, 96, 7, r, h=200, spread=50)
    # the slide: a pale rock shelf with a hollow stump at each end
    g.box(SPRAY, (0, 40, 86), (620, 70, 16))
    g.box(TIN, (0, 40, 66), (640, 84, 30))
    for s in (-1, 1):
        g.cyl(WOOD_D, (s * 330, 40, 50), 62, 130, steps=8, top_r=54)
        g.box(NAVY, (s * 286, 40, 118), (30, 60, 56))
    for i in range(2):
        x = -120 + i * 240
        f = Fr(sc, x, 40, 95, yaw=90, k=1.5)
        m = f.part(pivot=(0, 0, 8), phase=0.0, rate=0.0, base=(x, 40), travel=(640.0, 150.0 / 640.0))
        otter(m)
    f = Fr(sc, 250, -150, 57, yaw=60, k=1.5)
    m = f.part(pivot=(0, 0, 0), axis=90.0, phase=0.2, rate=0.35, swing=9)
    otter(m, back=True)
    for i, (x, y, yaw) in enumerate(((-300, -90, -25), (-60, -170, 20))):
        f = Fr(sc, x, y, 56, yaw=yaw, k=1.7)
        b = f.still()
        for s in (-1, 1):
            b.box(GOLD, (s * 5, 2 * s, 30), (4, 4, 62))
        b.sphere(WHITE, (0, 4, 72), 17, scale=(1.0, 1.6, 0.9), steps=(4, 6))
        tilt_box(b, SPRAY, (0, 24, 72), (0, 46, 60), 14, 6)
        n = f.part(pivot=(0, -18, 78), axis=0.0, phase=0.3 + i * 0.45, rate=0.28, swing=24)
        tilt_box(n, WHITE, (0, -16, 76), (0, -32, 106), 7, 7)
        tilt_box(n, WHITE, (0, -32, 106), (0, -22, 130), 7, 7)
        n.box(WHITE, (0, -27, 134), (10, 15, 10))
        tilt_box(n, GOLD, (0, -34, 134), (0, -58, 124), 5, 5)
        tilt_box(n, SPRAY, (0, -22, 139), (0, -2, 132), 5, 4)
        for s in (-1, 1):
            n.box(EYE, (s * 5, -30, 136), (3, 3, 3))


def otter(m, back=False):
    """An otter 80 cm long drawn into m, lying along -Y (on its belly, or floating on its back with its paws up)."""
    m.sphere(WOOD, (0, 6, 9), 12, scale=(1.0, 2.7, 0.8), steps=(3, 6))
    m.box(WOOD, (0, -30, 12), (17, 16, 14))
    m.box(CREAM, (0, -39, 10), (10, 5, 7))
    m.box(EYE, (0, -42, 12), (4, 3, 3))
    tilt_box(m, WOOD_D, (0, 34, 8), (0, 62, 5), 9, 6)
    for s in (-1, 1):
        m.box(EYE, (s * 6, -34, 17), (3, 3, 3))
        if back:
            m.box(CREAM, (s * 7, -12, 20), (5, 5, 8))
        else:
            m.box(WOOD_D, (s * 11, -14, 3), (5, 10, 5))
    if back:
        m.box(CREAM, (0, 0, 17), (14, 36, 4))


# ------------------------------------------------------------------------------------------ 8. white rabbits on a mound
def rabbits(sc):
    """Rocket Land and Funhouse, Giant's Garden. A lawn mound 1.4 m high with a burrow door, clover and daisies and
    a row of giant turnips; five white rabbits: three hop in place, one sits up and twitches its ears, one keeps
    watch on the top."""
    r = random.Random(63)
    g = sc.still()
    g.cyl(LAWN, (0, 0, -150), 330, 240, steps=12, top_r=290)
    g.cyl(LAWN, (0, 0, 90), 290, 50, steps=12, top_r=170)
    g.cyl(LAWN_D, (0, 0, 88), 296, 6, steps=12)
    g.cyl(WOOD_D, (0, -318, 20), 44, 12, rot=(0, 0, 90), steps=8)                      # the burrow's round door (roll 90: +Y)
    g.cyl(NAVY, (0, -322, 20), 34, 8, rot=(0, 0, 90), steps=8)
    g.box(GOLD, (16, -325, 22), (6, 5, 6))

    def top(x, y):                                                                       # the mound's own height at (x, y)
        d = math.hypot(x, y)
        return 140.0 if d < 170 else 90.0 + 50.0 * (290.0 - min(d, 290.0)) / 120.0

    for k in range(4):                                                                   # turnips: white, magenta shoulders
        a = math.radians(212 + k * 11)
        x, y = math.cos(a) * 262, math.sin(a) * 262
        z = top(x, y)
        g.sphere(WHITE, (x, y, z + 6), 18, scale=(1, 1, 0.9), steps=(3, 6))
        g.cyl(MAG, (x, y, z + 14), 15, 8, steps=6, top_r=8)
        tilt_box(g, TEAL, (x, y, z + 20), (x + 8, y, z + 52), 8, 5)
    for k in range(8):
        a, d = r.uniform(0, 6.283), r.uniform(120, 280)
        g.box(WHITE if k % 3 else AMBER, (math.cos(a) * d, math.sin(a) * d, top(math.cos(a) * d, math.sin(a) * d) + 3), (9, 9, 9))
    rabbit(sc, -120, -90, top(-120, -90), yaw=-30, phase=0.0, rate=1.2, bob=16)
    rabbit(sc, 60, -150, top(60, -150), yaw=25, phase=0.4, rate=1.4, bob=12)
    rabbit(sc, 170, 30, top(170, 30), yaw=80, phase=0.7, rate=1.1, bob=18)
    rabbit(sc, 150, -110, top(150, -110), yaw=-10, phase=0.3, rate=1.6, up=True)
    rabbit(sc, -10, 20, 140, yaw=10, k=1.9, phase=0.6, rate=0.9, up=True, twitch=9)


# ------------------------------------------------------------------------------------------ 9. the back lot's raccoons
def backlot_raccoons(sc):
    """The back lot. A crooked plank fence and three dented tin bins: a raccoon sits on a bin's rim, one pops up out
    of another with the lid on its head, a third on the fence; the lot's white cat on the fence post. No lamps."""
    g = sc.still()
    for k in range(9):
        h = 190 + (k * 37) % 40
        g.box(WOOD_D if k % 2 else WOOD, (-320 + k * 80, 60, h / 2.0 - 60), (74, 10, h + 120), rot=((k % 3 - 1) * 3, 0, 0))
    g.box(WOOD_D, (0, 54, 60), (700, 8, 14))
    g.box(WOOD_D, (0, 54, 150), (700, 8, 14))
    g.box(WOOD_D, (0, 60, 224), (720, 16, 10))                                           # the cap rail
    g.box(WOOD, (-372, 60, 60), (22, 22, 400))
    g.box(TIN, (250, 52, 120), (90, 6, 60), rot=(8, 0, 0))                               # a tin patch nailed on
    bins = ((-210, -30, 0), (0, -50, 6), (200, -20, -5))
    for i, (x, y, lean) in enumerate(bins):
        g.cyl(TIN, (x, y, -60), 50, 170, steps=8, top_r=56, rot=(0, 0, lean))
        g.cyl(TIN_D, (x, y, 20), 54, 8, steps=8)
        g.cyl(TIN_D, (x, y, 70), 57, 8, steps=8)
        g.cyl(NAVY, (x, y, 104), 50, 6, steps=8)
    g.cyl(TIN_D, (-292, -80, 6), 58, 8, steps=8, rot=(0, 0, 14))                         # a lid on the ground
    g.box(TIN, (-292, -80, 16), (20, 6, 8))
    g.box(CREAM, (90, -110, 6), (46, 6, 5))                                              # a fish bone
    for k in range(4):
        g.box(CREAM, (74 + k * 11, -110, 6), (4, 20, 4))
    cat(sc, -210, -74, 108, yaw=15, k=1.7, col=TIN_D, patch=CREAM, eye=WHITE, muzzle=9, rings=NAVY, mask=NAVY, phase=0.1, rate=0.6)
    cat(sc, 120, 60, 229, yaw=-12, k=1.6, col=TIN_D, patch=CREAM, eye=WHITE, muzzle=9, rings=NAVY, mask=NAVY, phase=0.5, rate=0.45)
    cat(sc, -372, 60, 260, yaw=20, k=1.8, col=WHITE, patch=PEACH, eye=CYAN, phase=0.7, rate=0.4)
    f = Fr(sc, 0, -50, 12, yaw=0, k=1.7)                                                 # the one in the bin: the lid rests on the rim, then up it comes
    m = f.part(pivot=(0, 0, 0), phase=0.0, rate=0.22, bob=58)
    m.box(TIN_D, (0, 0, 14), (24, 22, 30))
    m.box(TIN_D, (0, -2, 36), (24, 19, 16))
    m.box(NAVY, (0, -11.5, 39), (25, 3, 6))
    m.box(CREAM, (0, -15, 33), (10, 9, 7))
    m.box(EYE, (0, -20.5, 35), (4, 3, 3))
    for s in (-1, 1):
        m.box(WHITE, (s * 6, -12.5, 39), (4, 3, 4))
        m.box(TIN_D, (s * 9, 0, 46), (6, 5, 7))
        m.box(CREAM, (s * 13, -14, 22), (6, 8, 6))
    m.cyl(TIN, (0, 0, 50), 33, 5, steps=8)
    m.box(TIN_D, (0, 0, 57), (12, 4, 6))


# ------------------------------------------------------------------------------------------ 10. ponies with ribbons
PONY = dict(length=105.0, height=88.0, girth=40.0, neck=34.0, neck_up=48.0, head=32.0, tail=44.0)


def ribbon_ponies(sc):
    """Sweet Street. A paddock on a chocolate plinth with an icing rim and a candy-striped rail: three cream ponies
    with ribbons in mane and tail, two graze and swish their tails, one walks the length of the paddock and comes
    round again; a groom in an apron holds out a hand to it; a trough and a hay net."""
    g = sc.still()
    g.box(WOOD_D, (0, 40, -45), (1300, 560, 210))
    g.box(WHITE, (0, 40, 56), (1320, 580, 10))
    for k in range(5):
        g.sphere(WHITE, (-600 + k * 300, -248, 50), 20, scale=(1.0, 0.5, 1.5), steps=(3, 5))  # icing drips down the front
    for k in range(8):
        x = -630 + k * 180
        g.box(PEACH if k % 2 else WHITE, (x, 300, 120), (18, 18, 124))
        g.cyl(MAG, (x, 300, 182), 14, 14, steps=6, top_r=4)
    for z in (110, 160):
        for k in range(7):
            g.box(MAG if (k + z // 50) % 2 else WHITE, (-540 + k * 180, 300, z), (180, 8, 14))
    g.box(WOOD, (480, 230, 82), (150, 50, 40))
    g.box(CYAN, (480, 230, 101), (136, 38, 4))
    g.box(WOOD, (-520, 270, 150), (8, 8, 180))
    g.sphere(AMBER, (-520, 250, 190), 34, scale=(1.0, 0.7, 1.2), steps=(3, 6))
    coats = ((CREAM, MAG, KRAKEN), (WHITE, CYAN, LAV), (PEACH, KRAKEN, MAG))
    spots = ((-330, 120, 200, dict(neck_up=-18, graze=12, wag=24, rate=0.5, phase=0.1)),
             (330, 150, 150, dict(neck_up=-18, graze=10, wag=20, rate=0.42, phase=0.6)),
             (0, -110, 90, dict(stride=17, rate=1.2, phase=0.3, travel=(1120, 70))))
    for i, (x, y, yaw, mv) in enumerate(spots):
        body, mane, bow = coats[i]
        d = dict(PONY)
        d.update(mv)
        P.quadruped(sc, x, y, 61, yaw=yaw, s=1.25, body=body, belly=WHITE, mane=mane, ears=body, snout=PEACH, legs_col=body, tail_col=mane, **d)
        m, v = q_body(sc, x, y, 61, yaw=yaw, s=1.25, **d)
        hl = v['length'] / 2.0
        for s in (-1, 1):
            m.box(bow, (s * 7, hl - 2, v['height'] + 5), (12, 6, 9))                      # a bow at the root of the tail
            m.box(bow, (s * 21, -hl * 0.2, v['height'] - 16), (4, 30, 26))                # a saddle cloth
        m.box(bow, (0, -hl * 0.2, v['height'] + 1), (42, 30, 4))
    r = random.Random(71)
    P.figure(sc, -470, -90, 61, yaw=100, s=1.0, top=WHITE, bottom=KRAKEN, band=MAG, hat='chef', hat_col=WHITE, sleeves='short', detail=1,
             arm_r='fwd', arm_l='hip', swing_r=10, axis_r='x', rate=0.6, phase=r.random(), skin=P.SKINS[3], hair='bun', hair_col=P.HAIRS[2])


# ------------------------------------------------------------------------------------------ 11. the white deer
DEER = dict(length=120.0, height=105.0, girth=34.0, neck=44.0, head=32.0, tail=14.0)


def white_deer(sc):
    """Crescent Keep. A lawn knoll with a lavender bed and a gold crescent on a post: a white stag with gold
    antlers stands on the top, ears and tail flicking; two hinds graze; a fawn skips in place."""
    r = random.Random(77)
    g = sc.still()
    g.cyl(LAWN, (0, 30, -150), 560, 260, steps=12, top_r=500)
    g.cyl(LAWN, (0, 30, 110), 500, 40, steps=12, top_r=330)
    g.cyl(LAWN, (60, 60, 150), 240, 60, steps=10, top_r=150)
    g.cyl(LAWN_D, (0, 30, 108), 506, 6, steps=12)
    for k in range(9):
        a = math.radians(200 + k * 18)
        g.sphere(LAV, (math.cos(a) * 420, 30 + math.sin(a) * 420, 124), 26, scale=(1.2, 1.0, 1.1), steps=(3, 5))
    g.box(PURPLE, (-380, 240, 210), (14, 14, 200))                                       # the crescent on its post
    for k in range(7):
        a = math.radians(-70 + k * 40)
        g.box(GOLD, (-380 + math.cos(a) * 46, 240, 350 + math.sin(a) * 46), (14, 10, 40), rot=(math.degrees(a), 0, 0), glow=0.3)
    stag = dict(DEER)
    stag.update(length=140.0, height=122.0, girth=40.0, neck=52.0, neck_up=62.0, head=36.0)
    P.quadruped(sc, 60, 60, 210, yaw=-35, s=1.3, body=WHITE, belly=CREAM, ears=WHITE, horns=GOLD, snout=PEACH, legs_col=WHITE, tail_col=CREAM,
                wag=30, rate=0.7, phase=0.2, **stag)
    m, v = q_body(sc, 60, 60, 210, yaw=-35, s=1.3, phase=0.2, rate=0.7, **stag)
    antlers(m, v, GOLD, s=1.3, tall=1.2)
    m.box(GOLD, (0, -v['length'] / 2.0 + 12, v['height'] - 2), (v['girth'] + 4, 8, 8))    # a gold collar
    P.quadruped(sc, -230, -60, 150, yaw=40, s=1.2, body=WHITE, belly=CREAM, ears=WHITE, snout=PEACH, tail_col=CREAM,
                neck_up=-22, graze=13, wag=22, rate=0.45, phase=0.5, **DEER)
    P.quadruped(sc, 290, -40, 150, yaw=-150, s=1.15, body=CREAM, belly=WHITE, ears=CREAM, snout=PEACH, tail_col=WHITE,
                neck_up=-22, graze=10, wag=18, rate=0.38, phase=0.9, **DEER)
    fawn = dict(length=72.0, height=66.0, girth=24.0, neck=26.0, neck_up=50.0, head=35.0, tail=10.0)
    P.quadruped(sc, 60, -200, 150, yaw=70, s=1.0, body=WHITE, belly=CREAM, ears=PEACH, snout=PEACH, tail_col=CREAM,
                bob=14, rate=1.3, phase=r.random(), **fawn)


# ------------------------------------------------------------------------------------------ 12. penguins on the ice
def penguin(m, lie=False):
    """A penguin 70 cm tall drawn into m (front = -Y), standing, or on its belly heading -Y."""
    if lie:
        m.sphere(NAVY, (0, 0, 15), 15, scale=(1.0, 2.2, 0.9), steps=(3, 6))
        m.box(WHITE, (0, -2, 5), (22, 50, 6))
        m.box(NAVY, (0, -36, 18), (19, 17, 17))
        m.box(AMBER, (0, -49, 16), (7, 11, 6))
        for s in (-1, 1):
            m.box(WHITE, (s * 7, -42, 21), (5, 5, 5))
            tilt_box(m, NAVY, (s * 13, -10, 14), (s * 30, 12, 12), 12, 4)
            m.box(AMBER, (s * 8, 34, 10), (9, 12, 5))
        return
    m.sphere(NAVY, (0, 2, 32), 18, scale=(1.0, 0.95, 1.7), steps=(3, 6))
    m.box(WHITE, (0, -13, 30), (22, 8, 42))
    m.box(NAVY, (0, 0, 64), (22, 22, 18))
    m.box(AMBER, (0, -15, 62), (7, 10, 6))
    for s in (-1, 1):
        m.box(WHITE, (s * 7, -10.5, 67), (5, 4, 5))
        m.box(AMBER, (s * 8, -6, 2), (10, 16, 5))
        tilt_box(m, NAVY, (s * 17, 0, 50), (s * 24, 2, 20), 12, 5)


def penguins(sc):
    """Snowglobe Square. An ice shelf 13 m long and 1.5 m high with a polished slide between two igloo mouths: four
    penguins go down it on their bellies one after another and come round again; four more stand rocking on the
    step behind; a grey seal on an ice block lifts and lowers its head."""
    g = sc.still()
    g.box(SPRAY, (0, 0, -5), (1300, 320, 310))
    g.box(WHITE, (0, 115, 160), (1320, 110, 22))
    g.box(CYAN, (0, -60, 152), (1080, 96, 6), glow=0.2)                                   # the slide
    g.box(WHITE, (0, 130, 215), (900, 150, 100))                                          # the step behind
    g.box(SPRAY, (0, 130, 268), (916, 164, 8))
    for k in range(6):
        g.cyl(SPRAY, (-550 + k * 220, -164, 150), 14, 60, rot=(180, 0, 0), steps=5, top_r=3)   # icicles under the lip
    for s in (-1, 1):
        g.sphere(WHITE, (s * 590, -50, 150), 120, scale=(1.0, 1.0, 0.95), steps=(4, 8))
        g.box(NAVY, (s * 520, -60, 186), (60, 84, 66))
        g.box(SPRAY, (s * 516, -60, 226), (70, 100, 14))
        g.box(POOL, (s * 640, 120, 60), (40, 60, 420))
    LEN, SPEED = 1120.0, 190.0
    for k in range(4):
        x = -420 + k * 280
        f = Fr(sc, x, -60 + (k % 2) * 14 - 7, 156, yaw=90, k=1.5)
        m = f.part(pivot=(0, 0, 10), phase=0.0, rate=0.0, base=(x, f.at[1]), travel=(LEN, SPEED / LEN))
        penguin(m, lie=True)
    for k in range(4):
        x = -380 + k * 150
        f = Fr(sc, x, 90 + (k % 2) * 30, 272, yaw=(-20, 10, -5, 25)[k], k=1.5 if k != 2 else 1.0)
        m = f.part(pivot=(0, 0, 0), axis=90.0, phase=k * 0.27, rate=0.9 + k * 0.1, swing=8)
        penguin(m)
    # the seal on its block
    g.box(CYAN, (330, 120, 300), (190, 130, 60), glow=0.15)
    g.box(WHITE, (330, 120, 334), (200, 140, 10))
    f = Fr(sc, 330, 120, 339, yaw=-60, k=1.6)
    b = f.still()
    b.sphere(TIN, (0, 16, 16), 20, scale=(1.0, 2.2, 0.8), steps=(4, 6))
    tilt_box(b, TIN_D, (0, 52, 12), (0, 78, 20), 26, 6)
    for s in (-1, 1):
        tilt_box(b, TIN_D, (s * 14, -10, 8), (s * 34, -18, 2), 14, 5)
    n = f.part(pivot=(0, -16, 16), axis=0.0, phase=0.0, rate=0.4, swing=14)
    tilt_box(n, TIN, (0, -14, 18), (0, -34, 52), 24, 22)
    n.box(TIN, (0, -40, 60), (22, 24, 20))
    n.box(CREAM, (0, -53, 56), (14, 6, 9))
    n.box(EYE, (0, -57, 59), (5, 3, 4))
    for s in (-1, 1):
        n.box(EYE, (s * 8, -50, 65), (4, 4, 4))


# ------------------------------------------------------------------------------------------ 13. reindeer in their pen
REIN = dict(length=130.0, height=108.0, girth=40.0, neck=40.0, neck_up=40.0, head=34.0, tail=12.0)


def reindeer(sc):
    """Snowglobe Square. A snowy knoll with a blue rail pen under snow caps and a hay rack: three reindeer in red
    harness with gold bells, two stand (ears and tails flicking), one walks the pen's length and comes round again;
    their keeper in coat, scarf and bobble hat holds a bucket."""
    g = sc.still()
    g.box(WHITE, (0, 40, -25), (1300, 600, 250))
    g.box(SPRAY, (0, 40, 102), (1240, 540, 8))
    for k in range(4):
        g.sphere(WHITE, (-540 + k * 360, -250, 70), 60, scale=(1.4, 0.7, 0.6), steps=(3, 6))  # drifts along the front
    for k in range(8):
        x = -630 + k * 180
        g.box(POOL, (x, 320, 165), (18, 18, 130))
        g.box(WHITE, (x, 320, 234), (30, 30, 14))
    for z in (150, 200):
        g.box(POOL, (0, 320, z), (1270, 8, 14))
    g.box(WHITE, (0, 320, 210), (1270, 12, 6))
    for s in (-1, 1):                                                                     # the hay rack
        g.box(POOL, (440 + s * 60, 250, 150), (10, 10, 100))
    g.box(POOL, (440, 250, 190), (150, 50, 10))
    g.sphere(AMBER, (440, 250, 214), 40, scale=(1.6, 0.6, 0.6), steps=(3, 6))
    spots = ((-330, 130, 205, dict(wag=26, rate=0.6, phase=0.2)),
             (300, 150, 160, dict(wag=22, rate=0.5, phase=0.7)),
             (0, -110, 90, dict(stride=16, rate=1.1, phase=0.4, travel=(1100, 75))))
    for i, (x, y, yaw, mv) in enumerate(spots):
        d = dict(REIN)
        d.update(mv)
        P.quadruped(sc, x, y, 106, yaw=yaw, s=1.25, body=WOOD, belly=CREAM, ears=WOOD_D, horns=CREAM, snout=CREAM, legs_col=WOOD_D,
                    tail_col=CREAM, mane=CREAM, **d)
        m, v = q_body(sc, x, y, 106, yaw=yaw, s=1.25, **d)
        antlers(m, v, CREAM, s=1.25, tall=1.0 if i else 1.3)
        hl = v['length'] / 2.0
        m.box(RED, (0, -hl + 14, v['height'] - 18), (v['girth'] + 4, 9, 38))
        m.box(RED, (0, 0, v['height'] + 1), (v['girth'] + 4, 26, 4))
        for s in (-1, 1):
            m.box(GOLD, (s * (v['girth'] / 2.0 + 3), -hl + 14, v['height'] - 30), (5, 6, 6), glow=0.2)
    r = random.Random(83)
    P.figure(sc, -500, -80, 106, yaw=110, top=POOL, bottom=NAVY, band=RED, hat='beanie', hat_col=RED, wear='coat', sleeves='long', detail=1,
             arm_r='carry', arm_l='wave', swing_l=14, rate=0.7, phase=r.random(), skin=P.SKINS[1], hair='long', hair_col=P.HAIRS[4],
             hold_r=lambda mm, h, side: mm.cyl(TIN, (h[0], h[1] - 6, h[2] - 26), 13, 24, steps=6, top_r=16))


# ------------------------------------------------------------------------------------------ 14. the garden's small folk
def garden_folk(sc):
    """Giant's Garden. A giant's plank laid on two upturned flowerpots, 13 m long: a white duck and five yellow
    ducklings waddle along it in file and come round again; a saucer of lit water on a third pot with a frog that
    hops on its lily pad; two hedgehogs nosing about, each on a small pot of its own."""
    g = sc.still()
    for s in (-1, 1):
        g.cyl(WOOD, (s * 470, 0, -120), 130, 250, steps=10, top_r=96)
        g.cyl(PEACH, (s * 470, 0, -10), 138, 30, steps=10)
    g.box(CREAM, (0, 0, 141), (1340, 110, 22))
    for k in range(13):
        g.box(WOOD_D, (-600 + k * 100, -56, 141), (5, 4, 12 if k % 5 else 22))            # a ruler's marks along the edge
    g.cyl(WOOD, (0, 330, -120), 170, 230, steps=10, top_r=130)                            # the third pot and its saucer
    g.cyl(PEACH, (0, 330, 110), 230, 26, steps=12, top_r=250)
    g.cyl(CYAN, (0, 330, 132), 232, 6, steps=12, glow=0.3)
    g.cyl(TEAL, (40, 310, 138), 70, 5, steps=8)
    g.cyl(MAG, (-120, 380, 138), 16, 16, steps=6, top_r=26, glow=0.2)
    for s in (-1, 1):                                                                     # two small pots: the hedgehogs' seats
        g.cyl(WOOD, (s * 600, 120, -120), 64, 310, steps=8, top_r=50)
        g.cyl(PEACH, (s * 600, 120, 150), 56, 16, steps=8)
    f = Fr(sc, 40, 310, 143, yaw=20, k=1.8)
    m = f.part(pivot=(0, 0, 0), phase=0.2, rate=0.8, bob=26)
    m.sphere(TEAL, (0, 2, 10), 13, scale=(1.1, 1.3, 0.8), steps=(3, 6))
    m.box(TEAL, (0, -13, 16), (20, 12, 10))
    for s in (-1, 1):
        m.box(WHITE, (s * 7, -15, 23), (6, 6, 6))
        m.box(EYE, (s * 7, -18.5, 23), (3, 2.5, 3))
        tilt_box(m, LAWN_D, (s * 10, 6, 6), (s * 22, -6, 3), 7, 5)
    LEN, SPEED = 1240.0, 45.0
    for k in range(6):
        x = 250 - k * (130 if k < 2 else 100) - (40 if k >= 2 else 0)
        big = (k == 0)
        f = Fr(sc, x, 0 + (k % 2) * 12 - 6, 152, yaw=90, k=1.6 if big else 1.5)
        m = f.part(pivot=(0, 0, 0), axis=90.0, phase=k * 0.29, rate=1.6, swing=7, base=(x, f.at[1]), travel=(LEN, SPEED / LEN))
        if big:
            fowl(m, L=44, neck=20, lean=0.25, beak=AMBER, leg=10, legcol=AMBER, k=1.6)
        else:
            fowl(m, L=24, body=AMBER, neck=10, lean=0.2, beak=PEACH, leg=6, legcol=PEACH, eye=False, k=1.5)
    for i, (x, y, yaw) in enumerate(((-600, 120, -30), (600, 120, 40))):
        f = Fr(sc, x, y, 190, yaw=yaw, k=1.8)
        h = f.part(pivot=(0, 0, 0), axis=0.0, phase=i * 0.5, rate=0.7, swing=7)
        h.sphere(WOOD_D, (0, 4, 13), 16, scale=(1.0, 1.35, 0.85), steps=(3, 6))
        for k in range(5):
            tilt_box(h, WOOD, ((k - 2) * 6, 4 + (k % 2) * 6, 22), ((k - 2) * 9, 12 + (k % 2) * 6, 34), 4, 4)
        h.box(PEACH, (0, -19, 9), (13, 14, 11))
        h.box(EYE, (0, -27, 9), (4, 3, 4))
        for s in (-1, 1):
            h.box(EYE, (s * 5, -22, 14), (3, 3, 3))


# ------------------------------------------------------------------------------------------ 17. butterflies over a flower bed
def butterflies(sc):
    """Front Gate, Giant's Garden. A scalloped bed of tall hollyhocks (2.5 to 3.8 m) and six big pale butterflies
    over them: each beats its wings and drifts up and down where it is (none circles, none travels)."""
    r = random.Random(91)
    g = sc.still()
    g.cyl(WOOD_D, (0, 0, -150), 210, 210, steps=10)
    g.cyl(CREAM, (0, 0, 60), 222, 14, steps=10)
    g.cyl(LAWN_D, (0, 0, 74), 200, 6, steps=10)
    for k in range(10):
        a = k * 0.6283
        g.sphere(CREAM, (math.cos(a) * 214, math.sin(a) * 214, 60), 34, scale=(1, 1, 0.8), steps=(3, 5))
    tops = []
    for k in range(6):
        a = k * 1.047 + 0.3
        d = 60 + (k % 3) * 50
        x, y, h = math.cos(a) * d, math.sin(a) * d, 250 + (k * 53) % 130
        g.box(TEAL, (x, y, 78 + h / 2.0), (8, 8, h))
        col = (MAG, LAV, PEACH, WHITE, KRAKEN, MAG)[k]
        for j in range(5):
            z = 78 + h - 20 - j * 34
            g.cyl(col, (x + (10 if j % 2 else -10), y - 6, z), 9, 22, steps=5, top_r=17 + j * 2, rot=(60, 90 if j % 2 else -90, 0), glow=0.15)
        g.box(TEAL, (x + 16, y, 78 + h * 0.3), (40, 26, 5), rot=(20, k * 40, 0))
        tops.append((x, y, 78 + h))
    cols = ((WHITE, LAV), (SPRAY, CYAN), (CREAM, PEACH), (WHITE, MAG), (LAV, WHITE), (SPRAY, LAV))
    for k in range(6):
        x, y, z = tops[k]
        x, y, z = x + r.uniform(-60, 60), y - 50 - r.uniform(0, 60), z + r.uniform(10, 90)
        ph, rate = r.random(), r.uniform(1.3, 1.8)
        f = Fr(sc, x, y, z, yaw=r.uniform(-40, 40), k=1.0)
        b = f.part(pivot=(0, 0, 0), phase=ph, rate=rate, bob=-14)
        tilt_box(b, NAVY, (0, -16, 0), (0, 16, 4), 6, 6)
        wc, sp = cols[k]
        for s in (-1, 1):
            w = f.part(pivot=(0, 0, 2), axis=90.0, phase=ph, rate=rate, bob=-14, swing=s * 52)
            w.box(wc, (s * 22, -9, 2), (40, 26, 4), rot=(0, s * 18, 0), glow=0.25)
            w.box(wc, (s * 17, 12, 2), (30, 22, 4), rot=(0, -s * 14, 0), glow=0.25)
            w.box(sp, (s * 28, -10, 4.2), (12, 10, 4), glow=0.3)


# ------------------------------------------------------------------------------------------ the sky: straight lines, pale birds
def egrets(sc):
    """Pirate Cove, the Lagoon. Five white egrets in a slanting line, 4 m across the wings, legs trailing, beating
    slowly along the mesh's X (56 m, 7 m a second) and coming round again."""
    r = random.Random(101)
    for k in range(5):
        sky_bird(sc, 720 - k * 360, (k % 2) * 260 - 130, k * 60, span=400, travel=(5600, 700), phase=r.random(), rate=0.9, flap=22, legs=110)


def owls(sc):
    """Rocket Land and Funhouse, Crescent Keep, Snowglobe Square. Two white barn owls gliding low, 3.7 m across the
    wings, heart faces, on slow soft beats along the mesh's X (56 m, 4.5 m a second), one behind and above the other."""
    sky_bird(sc, 420, -120, 0, span=380, travel=(5600, 450), phase=0.1, rate=0.8, flap=16, bob=-30, face=CREAM)
    sky_bird(sc, -420, 140, 160, span=360, travel=(5600, 450), phase=0.6, rate=0.9, flap=16, bob=-30, face=CREAM)


def swan_flight(sc):
    """The Lagoon, Crescent Keep. Three white swans in line astern, 4.6 m across the wings, necks stretched out,
    flying the mesh's X (56 m, 9 m a second) and coming round again."""
    for k in range(3):
        sky_bird(sc, 560 - k * 620, (k - 1) * 200, k * 60, span=460, travel=(5600, 900), phase=k * 0.31, rate=1.0, flap=26, neck=130, legs=40)


def songbirds(sc):
    """Sweet Street, Giant's Garden. Five big pale songbirds (white and ice blue), 3.6 m across the wings (the park
    is toy-big, and a thinner wing flickers), bounding along the mesh's X in a loose string (56 m, 8 m a second): each
    rises and dips as it beats."""
    r = random.Random(113)
    for k in range(5):
        sky_bird(sc, 700 - k * 350 + r.uniform(-50, 50), r.uniform(-220, 220), r.uniform(0, 240), span=360, travel=(5600, 800),
                 phase=r.random(), rate=1.7, flap=30, bob=-45, col=(WHITE, SPRAY, WHITE, SPRAY, WHITE)[k])


WALK = dict(grow=(60.0, 60.0, 60.0), grow_down=(60.0, 60.0, 0.0))
# a sky piece flies 28 m either side of the mesh's middle; with these its cull radius stays under 40 m at scale 1
SKY = dict(far=True, grow=(2050.0, 120.0, 120.0), grow_down=(2050.0, 120.0, 120.0))
JOBS = [
    ('SM_S1ANIMALS_PeacockWall', lit(peacock_wall)),
    ('SM_S1ANIMALS_GeeseLine', lit(geese_line), WALK),
    ('SM_S1ANIMALS_PelicanPosts', lit(pelican_posts)),
    ('SM_S1ANIMALS_CrabRock', lit(crab_rock)),
    ('SM_S1ANIMALS_Swans', lit(swans), dict(far=True)),
    ('SM_S1ANIMALS_KoiLeap', lit(koi_leap), dict(far=True, grow=(60.0, 60.0, 140.0))),
    ('SM_S1ANIMALS_HeronsOtters', lit(herons_otters), dict(far=True)),
    ('SM_S1ANIMALS_Rabbits', lit(rabbits)),
    ('SM_S1ANIMALS_BackLotRaccoons', lit(backlot_raccoons)),
    ('SM_S1ANIMALS_RibbonPonies', lit(ribbon_ponies), WALK),
    ('SM_S1ANIMALS_WhiteDeer', lit(white_deer)),
    ('SM_S1ANIMALS_Penguins', lit(penguins), WALK),
    ('SM_S1ANIMALS_Reindeer', lit(reindeer), WALK),
    ('SM_S1ANIMALS_GardenFolk', lit(garden_folk), WALK),
    ('SM_S1ANIMALS_Butterflies', lit(butterflies)),
    ('SM_S1ANIMALS_Egrets', lit(egrets, 0.3), SKY),
    ('SM_S1ANIMALS_Owls', lit(owls, 0.3), dict(far=True, grow=(2350.0, 120.0, 120.0), grow_down=(2350.0, 120.0, 120.0))),
    ('SM_S1ANIMALS_SwanFlight', lit(swan_flight, 0.3), SKY),
    ('SM_S1ANIMALS_Songbirds', lit(songbirds, 0.3), SKY),
]


def run(only=None):
    P.build_core()                              # loads the two masters; never rebuilt here
    return P.export(DIR, JOBS, only)


def paths():
    return [DIR + '/' + j[0] for j in JOBS]
